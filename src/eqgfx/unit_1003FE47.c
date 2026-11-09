// unit_1003FE47.c - eqgfx_dx7.dll, the address run 0x1003FE47..0x100429FF (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include "eqgfx/layout/pcloud_inst.h"

typedef struct EqGfxTransformState_guess {
    unsigned char flags00;
    unsigned char flags01;
    unsigned char reserved02[0x3A];
    struct EqGfxTransformReference_guess *reference;
    float position[3];
    unsigned char reserved4C[0x38];
    float matrix[9];
} EqGfxTransformState_guess;

typedef struct EqGfxTransformReference_guess {
    int kind;
    unsigned char reserved04[0x08];
    int valid;
    unsigned char reserved10[0x0C];
    float orientation[4];
    float position[3];
    unsigned char reserved38[0x24];
    float matrix[9];
} EqGfxTransformReference_guess;

typedef void (__cdecl *EqGfxReferenceUpdateCallback_guess)(void *, void *);

#define EQGFX_REFERENCE_UPDATE_CALLBACK_GUESS EQ_GLOBAL(EqGfxReferenceUpdateCallback_guess, 0x100A16C4)

struct PCloudInstBase_guess;
struct PCloudParticle_guess;
typedef void (__cdecl *ParticleUpdateCallback_guess)(struct PCloudInstBase_guess *, struct PCloudParticle_guess *);
typedef void (__cdecl *ParticleCloudAxisCallback_guess)(void *, float *);
typedef float (__cdecl *ParticleCloudTimerCallback_guess)(int);

#define EQGFX_PARTICLE_TICK_SCALE_GUESS EQ_GLOBAL(float, 0x10076344)
#define EQGFX_PARTICLE_UPDATE_CALLBACK_GUESS EQ_GLOBAL(ParticleUpdateCallback_guess, 0x100A16C8)
#define EQGFX_PARTICLE_CLOUD_AXIS_CALLBACK_GUESS EQ_GLOBAL(ParticleCloudAxisCallback_guess, 0x100A16CC)
#define EQGFX_PARTICLE_SPAWN_RANDOM_SCALE_GUESS EQ_GLOBAL(float, 0x1007633C)
#define EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS EQ_GLOBAL(float, 0x10076340)
#define EQGFX_PARTICLE_SPAWN_TIME_SCALE_GUESS EQ_GLOBAL(float, 0x100A16D0)
#define EQGFX_PARTICLE_CLOUD_TIMER_CALLBACK_GUESS EQ_GLOBAL(ParticleCloudTimerCallback_guess, 0x100FC77C)

typedef struct RayCollisionWorld_guess {
    void *worldGeometry;
    void *collisionData;
} RayCollisionWorld_guess;

typedef struct RayCollisionContext_guess {
    unsigned char reserved00[0x78];
    unsigned int elapsedFrames;
    unsigned char reserved7C[0x60];
    RayCollisionWorld_guess *world;
} RayCollisionContext_guess;

typedef struct RayCollisionHit_guess {
    unsigned int reserved00[2];
    unsigned int hitToken;
    unsigned int reserved0C;
    float fraction;
} RayCollisionHit_guess;

#define EQGFX_RAY_COLLISION_EPSILON_GUESS EQ_GLOBAL(float, 0x10076288)
#define EQGFX_ROUND_X87_GUESS() EQ_CDECL(int, 0x1004EC36, (void))()

// Canonical outer/base storage is in eqgfx/layout/pcloud_inst.h. Base routines receive outer +0x14.
// The bit the init sets in the byte at +0x1 while forceScale (the float at +0x10) is nonzero.
#define PCLOUD_INST_FORCE_FLAG_BIT_GUESS 0x10
#define PCLOUD_INST_GRAVITY_BIT_GUESS 0x10

// The two pooled float constants the spawn-frame degeneracy test compares the perpendicular axis against.
#define EQGFX_SPAWN_AXIS_EPS_GUESS      EQ_GLOBAL(float, 0x1007628C)   // 0.0001f
#define EQGFX_SPAWN_AXIS_NEG_EPS_GUESS  EQ_GLOBAL(float, 0x10076330)   // -0.0001f

// The float the gravity setter compares against (0.0f).
#define EQGFX_ZERO_F_GUESS        EQ_GLOBAL(float, 0x10076284)   // 0.0f

// The float constants the particle motion step uses: one half (the t^2 term), the wrap box's edges and its span.
#define EQGFX_MOTION_HALF_GUESS         EQ_GLOBAL(float, 0x10076290)   // 0.5f
#define EQGFX_WRAP_LOW_GUESS            EQ_GLOBAL(float, 0x10076338)   // -200.0f
#define EQGFX_WRAP_HIGH_GUESS           EQ_GLOBAL(float, 0x10076334)   // 200.0f
#define EQGFX_WRAP_SPAN_GUESS           EQ_GLOBAL(float, 0x1007630C)   // 400.0f
#define EQGFX_PARTICLE_RAMP_GUESS       EQ_ARRAY(float, 0x100F2DAC)
#define EQGFX_PARTICLE_RAMP_END_GUESS   0x100F3DAC
#define EQGFX_PARTICLE_RAMP_SCALE_GUESS EQ_GLOBAL(float, 0x1007632C)
#define EQGFX_FLOAT_ONE  EQ_GLOBAL(float, 0x10076280)
#define EQGFX_PARTICLE_ANGLE_STEP_GUESS EQ_GLOBAL(float, 0x10076328)
#define EQGFX_PARTICLE_DIRECTIONS_GUESS EQ_ARRAY(float, 0x1084F8C0)
#define EQGFX_PARTICLE_DIRECTION_END_GUESS 0x1084FA44
#define EQGFX_PARTICLE_COS_GUESS(angle) EQ_CDECL(double, 0x10069CF4, (double))(angle)
#define EQGFX_PARTICLE_SIN_GUESS(angle) EQ_CDECL(double, 0x10069C44, (double))(angle)
#define EQGFX_PARTICLE_SPRITE_RADIUS_SCALE_GUESS EQ_GLOBAL(float, 0x100762D0)
#define EQGFX_JITTER_SCALE_GUESS EQ_GLOBAL(float, 0x100762E8)
#define EQGFX_PARTICLE_TRAIL_DELTA_GUESS EQ_GLOBAL(float, 0x10076324)
#define EQGFX_FAR_LIMIT                 1e30f   // clamp for a squared distance; the end of the open last band
#define PARTICLE_CLOUD_RENDER_FLAGS 0x011B0007
#define PARTICLE_CLOUD_ACTOR_REFERENCE_TYPE 0x18
#define PARTICLE_CLOUD_HSPRITE_REFERENCE_TYPE 0x3B
#define PARTICLE_CLOUD_BIND_ERROR 0xFFFF3D3E

// The size of the particle-cloud record t3dCreateParticleCloud allocates (the allocator zeroes it).
#define PARTICLE_CLOUD_RECORD_SIZE 0x120

// The first dword of the record: the type _t3dAddItemToList_guess reads when it links the record into the dictionary.
#define PARTICLE_CLOUD_OBJECT_TYPE 0x4D

// The PCloudInstBase record embedded at +0x14 of the instance (t3dInitPCloudInstBase and t3dFreePCloudInstBase take it).
#define PCLOUD_INST_EMBEDDED_RECORD_OFFSET 0x14

// The descriptor block at +0x108 inside the instance; the descriptor pointer at +0xF0 points at it.
#define PCLOUD_INST_DESCRIPTOR_BLOCK_OFFSET 0x108

// The block at +0x14 of a spawn reference, which the init takes as its sixth or seventh argument.
#define PARTICLE_CLOUD_REFERENCE_BLOCK_OFFSET 0x14


typedef struct ParticleCloudSpawnReference_guess {
    int type;
    char pad04[0x0c];
    int referenceKey;
} ParticleCloudSpawnReference_guess;


typedef struct PCloudSpawnContext_guess {
    Float3_guess position;
    unsigned char reserved0C[0x0C];
    int elapsedTicks;
} PCloudSpawnContext_guess;


// The descriptor record the particle-cloud instance points at through +0xF0; +0x34 is the mode the spawn setup
// below reads, the rest is the layout t3dInitParticleCloudDefBase fills (leading dwords, six vectors, six dwords).
typedef struct PCloudDef_guess {
    int value00;                     // 0x00
    int value04;                     // 0x04
    int value08;                     // 0x08
    union {
        int value0C;                     // 0x0C
        float forceScale;                // 0x0C the same dword read as a float: the force scale (zero means none)
    };
    Float3_guess vector10;           // 0x10 zeroed when the source pointer is null
    Float3_guess vector1C;           // 0x1C zeroed when the source pointer is null
    Float3_guess vector28;           // 0x28 zeroed when the source pointer is null
    int mode;                        // 0x34
    int value38;                     // 0x38
    Float3_guess vector3C;           // 0x3C only written when the source pointer is given
    Float3_guess vector48;           // 0x48 only written when the source pointer is given
    Float3_guess vector54;           // 0x54 zeroed when the source pointer is null
    int value60;                     // 0x60
    int value64;                     // 0x64
    int value68;                     // 0x68
    int value6C;                     // 0x6C
    int value70;                     // 0x70
    int value74;                     // 0x74
    PCloudColor_guess color;         // 0x78
} PCloudDef_guess;

#define PCLOUD_DEFINITION_OBJECT_TYPE_GUESS 0x4C

typedef struct PCloudDictionaryEntry_guess {
    int type;                        // 0x00
    int reserved04;                  // 0x04
    char *name;                      // 0x08
    int dictionaryFlags;             // 0x0C
    int state;                       // 0x10
    PCloudDef_guess definition;      // 0x14
    void *sharedObject;              // 0x90
} PCloudDictionaryEntry_guess;

// One node of the pool's 0x44-byte node array at +0xC of the instance record: +0x1C links a list forward and
// +0x30 backward (the free list uses the forward link, the allocated list keeps both).
typedef struct ParticleNode_guess {
    char pad00[0x1C];
    int next;                        // 0x1C
    char pad20[0x10];
    int prev;                        // 0x30
    char pad34[0x10];
} ParticleNode_guess;


// The time base the duration is added to (only the dword at +0x8 is known here).
typedef struct PCloudTimeBase_guess {
    char pad00[0x8];
    int value;                       // 0x08
} PCloudTimeBase_guess;

// The cloud's motion settings (not PCloudDef_guess: its +0x34 is the mode, while this record's +0x34 is a bounds
// value). Flags: 0x40 wrap into the box, 0x400 track the bounds, 0x800 home on `accel`, 0x1000 apply the force.
typedef struct PCloudMotion_guess {
    unsigned int flags;              // 0x00
    char pad04[0x4];
    unsigned int boundsSamples;      // 0x08 below 2 the bounds restart at the particle's position
    char pad0C[0x4];
    float forceScale;                // 0x10
    char pad14[0x4];
    Float3_guess accel;              // 0x18 the acceleration, or the point homed on with 0x800
    Float3_guess boundsMin;          // 0x24
    Float3_guess boundsMax;          // 0x30
    char pad3C[0x7C];
    unsigned int timeBase;           // 0xB8 the frame time catches up to timeBase + the elapsed ticks
    char padBC[0x8];
    Float3_guess wrapOffset;         // 0xC4 added to the position before the wrap test
    unsigned int frameCount;         // 0xD0 frames per cycle (1 = no animation)
    int frameStep;                   // 0xD4 added to the frame time for each frame step
} PCloudMotion_guess;

// One particle: its position and velocity, then the animation frame index and the time that index has reached.
typedef struct PCloudParticle_guess {
    Float3_guess pos;                // 0x00
    Float3_guess vel;                // 0x0C
    unsigned int nextFrameTick;      // 0x18
    int nextIndex;
    int elapsedTicks;                // 0x20
    union {
        unsigned int frame;          // 0x24
        float frameValue;            // 0x24
    };
    unsigned int frameTime;          // 0x28
    unsigned int randomValue;        // 0x2C
    int previousIndex;
    Float3_guess cachedPosition;
    int specialState;
} PCloudParticle_guess;

// The render context the user-defined render method table hangs off (only +0x3DC and +0x3E0 are known here; the rest
// of the record is not in include/).
typedef struct RenderContext_guess {
    char pad00[0x3DC];
    unsigned int userMethodCount;    // 0x3DC the table's entry count: an index at or past it reads as 7
    unsigned int *userMethods;       // 0x3E0 the table of 4-byte method values
} RenderContext_guess;

// One band of a render object's distance table: 12 bytes, scanned by endDistSq. The lookup also reads the next
// band's startDistSq and methodIndex (+0xC and +0x14 of this record).
typedef struct RenderLodBand_guess {
    float startDistSq;               // 0x00 squared distance the band starts at (read only through the previous band)
    float endDistSq;                 // 0x04 squared distance the band ends at; 1e30 on the open last band
    unsigned int methodIndex;        // 0x08 the render method the band selects
} RenderLodBand_guess;

// The distance table at +0x40 of a render object: mode 1 selects the first band's method without a lookup.
typedef struct RenderLodTable_guess {
    int mode;                        // 0x00
    RenderLodBand_guess *bands;      // 0x04
} RenderLodTable_guess;

// The render object the distance lookup is called with. Only the position (+0x10), the distance table (+0x40) and
// the method index (+0xE0, bit 31 is a flag) are used here; the rest of the record is not in include/.
typedef struct RenderObject_guess {
    char pad00[0x10];
    Float3_guess pos;                // 0x10
    char pad1C[0x24];
    RenderLodTable_guess *lodTable;  // 0x40
    char pad44[0x9C];
    unsigned int methodIndex;        // 0xE0
} RenderObject_guess;

// One of the two source records t3dSetPCloudRenderSources_guess caches; only the dword it copies out of it is known.
typedef struct PCloudSrcA_guess {
    char pad00[8];
    union {
        int value08;                 // 0x8
        int frameIndex;
    };
} PCloudSrcA_guess;

// The other source record t3dSetPCloudRenderSources_guess caches; only the three consecutive dwords copied out of it are known.
typedef struct PCloudSrcB_guess {
    char pad00[0x1AC];
    union {
        int value1AC;
        float originX;
    };
    union {
        int value1B0;
        float originY;
    };
    union {
        int value1B4;
        float originZ;
    };
    char pad1B8[0x74];
    float transform22C[13];          // 0x22C through 0x25C
    char pad260[0x30];
    int drawCount;                   // 0x290
    char pad294[8];
    float tint[3];                   // 0x29C
    char pad2A8[0x1C];
    float *quadVertices;             // 0x2C4
    char pad2C8[0x118];
    unsigned int *userMethods;       // 0x3E0
    char pad3E4[0x1C];
    int currentMethod;               // 0x400
    char pad404[0x12C];
    unsigned char collisionState[4]; // 0x530
} PCloudSrcB_guess;

typedef union PCloudPaintArgument_guess {
    PCloudSrcB_guess *context;
    int integer;
    float colorChannel;
} PCloudPaintArgument_guess;

// The buffer the definition's +0x1C points to: +0x18 and +0x1C are the two dwords t3dSetPCloudBlitsprite copies out.
typedef struct BlitSpriteBuffer_guess {
    char pad00[0x18];
    int value18;                     // 0x18
    int value1C;                     // 0x1C
} BlitSpriteBuffer_guess;

// The definition the source points at through +0x28: +0x14 is copied out, +0x1C points to the buffer.
typedef struct BlitSpriteDefinition_guess {
    char pad00[0x14];
    int value14;                     // 0x14
    char pad18[0x4];
    BlitSpriteBuffer_guess *buffer;  // 0x1C
} BlitSpriteDefinition_guess;

// The source the blit sprite points at through +0x68: +0x28 holds the definition.
typedef struct BlitSpriteSource_guess {
    char pad00[0x28];
    BlitSpriteDefinition_guess *definition; // 0x28
} BlitSpriteSource_guess;

// The blit sprite t3dCreateBlitSprite returns (the instance keeps it at +0x110); only +0x68 is read here.
typedef struct BlitSprite_guess {
    char pad00[0x68];
    BlitSpriteSource_guess *source;  // 0x68
} BlitSprite_guess;

// The state record t3dSetPCloudRenderSources_guess fills: it caches both source records and the dwords copied out of them; the
// rest of the layout is not known.
struct PCloudEffectRef_guess;
struct HSpriteChainNode_guess;
struct ParticleCloudRenderObject_guess;

typedef struct PCloudCtx_guess {
    char pad00[0x14];
    union {
        unsigned int flags;          // 0x14
        unsigned char flagBytes[4];
    };
    char pad18[4];
    int state;                       // 0x1C
    PCloudParticle_guess *particles; // 0x20
    char pad24[0x14];
    Float3_guess boundsMin;          // 0x38
    Float3_guess boundsMax;          // 0x44
    struct PCloudEffectRef_guess *spawnBinding; // 0x50
    Float3_guess spawnXYZ;            // 0x54
    char pad60[0x5C];
    float scale;                     // 0xBC
    int valueC0;
    unsigned int elapsed;            // 0xC4
    int color;                       // 0xC8
    unsigned int valueCC;            // 0xCC from PCloudSrcA_guess::value08
    char padD0[8];
    int valueD8;                     // 0xD8 from PCloudSrcB_guess::value1AC
    int valueDC;                     // 0xDC from PCloudSrcB_guess::value1B0
    int valueE0;                     // 0xE0 from PCloudSrcB_guess::value1B4
    char padE4[0x14];
    int firstParticle;               // 0xF8
    char padFC[8];
    int *renderMode;                 // 0x104
    PCloudSrcA_guess *sourceA;       // 0x108
    PCloudSrcB_guess *sourceB;       // 0x10C
    struct ParticleCloudRenderObject_guess *renderObject; // 0x110
    struct PCloudCtx_guess *sharedObjectA; // 0x114
    struct PCloudCtx_guess *sharedObjectB; // 0x118
} PCloudCtx_guess;

// The dictionary the records are linked into (no struct in include/ yet): only the dword at +0x8 is read here, as the
// third argument of t3dInitPCloudInstBase.
typedef struct Dictionary_guess {
    char pad00[0x8];
    int value08;                     // 0x08
} Dictionary_guess;

// The effect reference t3dEvaluateInheritedXYZ_T3D reads through output[0]: +0x0 is the kind (0x18 actor, 0x3B hsprite),
// +0x10 is the first chain node (or the first of three dwords copied for 0x18), +0x80/+0x84/+0x88 are the
// three dwords copied for 0x3B.
typedef struct PCloudEffectRef_guess {
    int kind;                        // 0x00
    char pad04[8];
    union {
        void *alternate;
        struct {
            unsigned char pad0C[3];
            unsigned char flags0F;
        } bytes;
    } value0C;
    union {
        struct HSpriteChainNode_guess *value10;
        float value10Float;
    };
    float value14;                   // 0x14
    float value18;                   // 0x18
    float orientation[3];            // 0x1C
    char pad28[0x0C];
    float value34;                   // 0x34
    float value38;                   // 0x38
    char pad3C[0x20];
    union {
        float transform[12];         // 0x5C through 0x88
        struct {
            float transformPrefix[9];
            int value80;
            int value84;
            int value88;
        };
    };
} PCloudEffectRef_guess;

typedef struct ParticleCloudDrawResource_guess {
    char pad00[0x18];
    unsigned int *frameIds;
} ParticleCloudDrawResource_guess;

typedef struct ParticleCloudDrawState_guess {
    char pad00[0x14];
    union {
        unsigned int **data;
        unsigned int frameId;
    };
    unsigned int currentFrame;
    char pad1C[4];
    int displayFrame;
    char pad24[4];
    ParticleCloudDrawResource_guess *resource;
} ParticleCloudDrawState_guess;

typedef struct ParticleCloudRenderObject_guess {
    char pad00[0x14];
    unsigned int flags;
    char pad18[0x50];
    ParticleCloudDrawState_guess *drawState;
} ParticleCloudRenderObject_guess;

typedef struct ParticleCloudRenderPacket_guess {
    unsigned char vertexData[16];
    int enabled;
    int color;
    unsigned char flags;
    char alpha;
    unsigned char reserved[2];
    unsigned int frameReference;
} ParticleCloudRenderPacket_guess;

// The link between hierarchical-sprite chain nodes: +0x10 points at the next node.
typedef struct HSpriteChainLink_guess {
    char pad00[0x10];
    void *nextNode;                  // 0x10
} HSpriteChainLink_guess;

// One hierarchical-sprite chain node: +0x18 links forward (null ends the walk), +0x1C is passed to the update.
typedef struct HSpriteChainNode_guess {
    char pad00[0x18];
    HSpriteChainLink_guess *link;    // 0x18
    void *extra;                     // 0x1C
} HSpriteChainNode_guess;

// Three consecutive dwords (the two triplets t3dEvaluateInheritedXYZ_T3D copies).
typedef struct ThreeDwords_guess {
    int v0;
    int v1;
    int v2;
} ThreeDwords_guess;

// FUNCTION: EQGFX 0x1003FE47
// Registers a reusable particle effect definition by name, reusing an existing matching definition.
int *__cdecl t3dDefineParticleCloud(
    void *dictionary, char *name, int value00, int value04, int value08, float forceScale,
    Float3_guess *vector10, Float3_guess *vector1C, Float3_guess *vector28, int mode, int value38,
    Float3_guess *vector3C, Float3_guess *vector48, Float3_guess *vector54, int value60, float value64,
    int value68, float value6C, int value70, float value74, int *color, void *sharedObject)
{
    int *entry;
    int result;

    entry = EQ_CDECL(int *, 0x1000E7FE, (void *, char *))(dictionary, name);
    if (entry == 0)
    {
        entry = (int *)EQ_CDECL(void *, 0x10050C48, (int))(
            sizeof(PCloudDictionaryEntry_guess));
        if (entry == 0)
        {
            return 0;
        }
        EQ_CDECL(int, 0x10041802,
                 (PCloudDef_guess *, int, int, int, float, Float3_guess *, Float3_guess *, Float3_guess *,
                  int, int, Float3_guess *, Float3_guess *, Float3_guess *, int, float, int, float, int, float,
                  int *))(
            &((PCloudDictionaryEntry_guess *)entry)->definition, value00, value04, value08, forceScale,
            vector10, vector1C, vector28,
            mode, value38, vector3C, vector48, vector54, value60, value64, value68, value6C, value70,
            value74, color);
        ((PCloudDictionaryEntry_guess *)entry)->dictionaryFlags = 0;
        ((PCloudDictionaryEntry_guess *)entry)->type = PCLOUD_DEFINITION_OBJECT_TYPE_GUESS;
        ((PCloudDictionaryEntry_guess *)entry)->state = 1;
        ((PCloudDictionaryEntry_guess *)entry)->sharedObject = sharedObject;
        if (sharedObject != 0)
        {
            EQ_CDECL(void, 0x10017CE8, (void *))(sharedObject);
        }
        ((PCloudDictionaryEntry_guess *)entry)->name = (char *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(
            EQ_CDECL(unsigned int, 0x10068160, (const char *))(name) + 1);
        if (((PCloudDictionaryEntry_guess *)entry)->name != 0)
        {
            EQ_CDECL(void, 0x10068070, (void *, const char *))(
                ((PCloudDictionaryEntry_guess *)entry)->name, name);
            result = EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, entry);
            if (result == 0)
            {
                result = EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, entry);
                if (result >= 0)
                {
                    return entry;
                }
                EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, entry);
                EQ_CDECL(void, 0x10050CBB, (void *))(((PCloudDictionaryEntry_guess *)entry)->name);
            }
            else
            {
                EQ_CDECL(void, 0x10050CBB, (void *))(((PCloudDictionaryEntry_guess *)entry)->name);
            }
        }
        if (sharedObject != 0)
        {
            EQ_CDECL(void, 0x10017DAD, (void *))(sharedObject);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(entry);
    }
    else if (((PCloudDictionaryEntry_guess *)entry)->type == PCLOUD_DEFINITION_OBJECT_TYPE_GUESS)
    {
        return entry;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10040320
// Marks a particle cloud to stop.
// __cdecl (context and outer-instance pointers; the callee pops 0 bytes). Sets bit 0 of the target's flags word
// and copies the context's +0x08 word into the target's duration slot at +0xC4; returns 0.
int __cdecl t3dStopPCloudAndAutoDestroy(PCloudTimeBase_guess *source, PCloudInst_guess *target)
{
    target->base.flags |= 1;
    target->base.endTime_guess = source->value;
    return 0;
}

// FUNCTION: EQGFX 0x1003FF76
// Unregisters a particle cloud from the world.
// __cdecl (two arguments on the stack; the callee pops 0 bytes). Undefines a particle cloud in state 1 and returns
// the result of t3dFreeObjectPoolSlot_guess; any other state returns 0xFFFF3D44.
int __cdecl t3dUndefineParticleCloud(void *dictionary, PCloudDictionaryEntry_guess *instance)
{
    int result;

    if (instance->state != 1)
    {
        result = 0xffff3d44;
    }
    else
    {
        void *shared = instance->sharedObject;

        if (shared != 0)
        {
            EQ_CDECL(void, 0x10017DAD, (void *))(shared);
        }
        if ((*((unsigned char *)&instance->dictionaryFlags) & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, instance->name);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, instance);
        if (instance->name != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(instance->name);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(instance);
    }
    return result;
}

// FUNCTION: EQGFX 0x1003FFD6
// Sets the renderer's particle-cloud density multiplier.
// __cdecl (one argument on the stack; the callee pops 0 bytes). Stores the passed dword in the global at
// 0x100A16C0 (the density multiplier) and returns 0.
int __cdecl t3dSetPCloudDensityMultiplier(int density)
{
    EQ_GLOBAL(int, 0x100A16C0) = density;
    return 0;
}

// FUNCTION: EQGFX 0x1003FFE2
// Creates a particle effect instance from a shared definition, with optional reference sources and sprite.
// __cdecl (five stack arguments; the caller pops them). Builds a particle-cloud record, links it into the dictionary and
// returns it; returns 0 when the allocation or the link fails (a failed link frees the record).
// @early-stop: VC6 places the shared nulling XOR in the link-failure arm instead of after blit cleanup.
PCloudInst_guess *__cdecl t3dCreateParticleCloud(Dictionary_guess *dictionary, PCloudDictionaryEntry_guess *relatedInstance,
                                                 int baseOffset, ParticleCloudSpawnReference_guess *referenceA,
                                                 ParticleCloudSpawnReference_guess *referenceB)
{
    PCloudInst_guess *instance;
    char *referenceBlockA;
    char *referenceBlockB;
    void *embeddedRecord;
    void *sharedObject;
    BlitSprite_guess *blitSprite;
    BlitSpriteSource_guess *source;

    referenceBlockA = 0;
    referenceBlockB = 0;
    instance = (PCloudInst_guess *)EQ_CDECL(void *, 0x10050C48, (int))(PARTICLE_CLOUD_RECORD_SIZE);
    if (instance != 0)
    {
        instance->type = PARTICLE_CLOUD_OBJECT_TYPE;
        instance->entry = 0;
        instance->dictFlags = 0;
        instance->state = 0;
        instance->definition_guess = relatedInstance;
        if (referenceA != 0)
        {
            referenceBlockA = (char *)referenceA + PARTICLE_CLOUD_REFERENCE_BLOCK_OFFSET;
        }
        if (referenceB != 0)
        {
            referenceBlockB = (char *)referenceB + PARTICLE_CLOUD_REFERENCE_BLOCK_OFFSET;
        }
        embeddedRecord = &instance->base;
        EQ_CDECL(void, 0x10041903, (void *, void *, int, float, int, void *, void *))
            (embeddedRecord,
             &relatedInstance->definition, dictionary->value08,
             EQ_GLOBAL(float, 0x100A16C0), baseOffset, referenceBlockA, referenceBlockB);
        instance->base.localDescriptor_guess = (PCloudDef_guess *)((char *)instance + PCLOUD_INST_DESCRIPTOR_BLOCK_OFFSET);
        if (EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, instance) != 0)
        {
            EQ_CDECL(void, 0x10041ADF, (void *))(embeddedRecord);
            EQ_CDECL(void, 0x10050CBB, (void *))(instance);
            instance = 0;
        }
        else
        {
            EQ_CDECL(void, 0x10017CE8, (void *))(relatedInstance);
            instance->sharedObjectA = referenceA;
            if (referenceA != 0)
            {
                EQ_CDECL(void, 0x10017CE8, (void *))(referenceA);
            }
            instance->sharedObjectB = referenceB;
            if (referenceB != 0)
            {
                EQ_CDECL(void, 0x10017CE8, (void *))(referenceB);
            }
            instance->blitSprite = 0;
            sharedObject = relatedInstance->sharedObject;
            if (sharedObject != 0)
            {
                blitSprite = EQ_CDECL(BlitSprite_guess *, 0x1000CFBF, (void *, void *))(dictionary, sharedObject);
                instance->blitSprite = blitSprite;
                if (blitSprite == 0)
                {
                    EQ_CDECL(void, 0x10017DAD, (void *))(relatedInstance);
                    EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, instance);
                    EQ_CDECL(void, 0x10041ADF, (void *))(embeddedRecord);
                    EQ_CDECL(void, 0x10050CBB, (void *))(instance);
                    instance = 0;
                }
                else
                {
                    source = blitSprite->source;
                    instance->base.frameCount = source->definition->value14;
                    instance->base.frameStep = source->definition->buffer->value1C;
                    instance->base.spriteBuffer18Value_guess = source->definition->buffer->value18;
                }
            }
        }
    }
    return instance;
}

// FUNCTION: EQGFX 0x1004013B
// Changes or removes the actor or hierarchical sprite attachment used for particle spawning.
int __cdecl t3dBindParticleCloudSpawn(PCloudInst_guess *owner,
                                     ParticleCloudSpawnReference_guess *reference)
{
    ParticleCloudSpawnReference_guess *oldReference;
    int result;
    int addResult;

    result = 0;
    oldReference = (ParticleCloudSpawnReference_guess *)owner->base.gravityActorBinding;
    if (oldReference != 0)
    {
        if (oldReference->type == PARTICLE_CLOUD_ACTOR_REFERENCE_TYPE)
        {
            EQ_CDECL(void, 0x1000A4ED, (void *, void *))(oldReference, owner);
        }
        else if (oldReference->type == PARTICLE_CLOUD_HSPRITE_REFERENCE_TYPE)
        {
            EQ_CDECL(void, 0x10013D10, (int, void *))(oldReference->referenceKey, owner);
        }
    }
    if (reference != 0)
    {
        if (reference->type == PARTICLE_CLOUD_ACTOR_REFERENCE_TYPE)
        {
            owner->base.gravityActorBinding = reference;
            addResult = EQ_CDECL(int, 0x1000A453, (void *, void *))(reference, owner);
        }
        else
        {
            if (reference->type != PARTICLE_CLOUD_HSPRITE_REFERENCE_TYPE)
            {
                return 0;
            }
            owner->base.gravityActorBinding = reference;
            addResult = EQ_CDECL(int, 0x10013C91, (int, void *))(reference->referenceKey, owner);
        }
        if (addResult != 0)
        {
            result = PARTICLE_CLOUD_BIND_ERROR;
        }
    }
    return result;
}

#define PARTICLE_CLOUD_GRAVITY_BINDING_FLAG 0x08

// FUNCTION: EQGFX 0x100401A6
// Changes or removes a particle cloud's gravity reference when gravity binding is enabled.
int __cdecl t3dBindParticleCloudGravity(PCloudInst_guess *instance,
                                      ParticleCloudSpawnReference_guess *reference)
{
    ParticleCloudSpawnReference_guess *oldReference;
    int result;
    int addResult;

    result = 0;
    if ((instance->base.motionFlagBytes[1] & PARTICLE_CLOUD_GRAVITY_BINDING_FLAG) != 0)
    {
        oldReference = (ParticleCloudSpawnReference_guess *)instance->base.gravityBinding;
        if (oldReference != 0)
        {
            if (oldReference->type == PARTICLE_CLOUD_ACTOR_REFERENCE_TYPE)
            {
                EQ_CDECL(void, 0x1000A4ED, (void *, void *))(oldReference, instance);
            }
            else if (oldReference->type == PARTICLE_CLOUD_HSPRITE_REFERENCE_TYPE)
            {
                EQ_CDECL(void, 0x10013D10, (int, void *))(oldReference->referenceKey, instance);
            }
        }
        if (reference != 0)
        {
            if (reference->type == PARTICLE_CLOUD_ACTOR_REFERENCE_TYPE)
            {
                instance->base.gravityActorBinding = reference;
                addResult = EQ_CDECL(int, 0x1000A453, (void *, void *))(reference, instance);
            }
            else
            {
                if (reference->type != PARTICLE_CLOUD_HSPRITE_REFERENCE_TYPE)
                {
                    return 0;
                }
                instance->base.gravityBinding = reference;
                addResult = EQ_CDECL(int, 0x10013C91, (int, void *))(reference->referenceKey, instance);
            }
            if (addResult != 0)
            {
                result = PARTICLE_CLOUD_BIND_ERROR;
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10040217
// Destroys an unshared particle effect instance and releases its attachments, sprite, and particle storage.
// @early-stop: the state-error return and context/result register allocation still differ (29 instruction diffs).
#pragma optimize("y", off)
/* fake match: retain the EBP argument frame that /O1 otherwise omits. */
int * __cdecl t3dDestroyParticleCloud(void *context, int *instanceReference)
{
    int instanceValue;
    int *result;
    PCloudInst_guess *instance;

    instanceValue = *instanceReference;
    if ((instanceValue == 0) || ((0 < instanceValue) && (instanceValue < 50000)))
    {
        return 0;
    }

    instance = (PCloudInst_guess *)instanceValue;
    if (instance->state != 0)
    {
        return (int *)0xFFFF3D45;
    }

    if (instance->sharedObjectA != 0)
    {
        EQ_CDECL(void, 0x10017DAD, (void *))(instance->sharedObjectA);
    }
    if (instance->sharedObjectB != 0)
    {
        EQ_CDECL(void, 0x10017DAD, (void *))(instance->sharedObjectB);
    }
    if (instance->definition_guess != 0)
    {
        instance->definition_guess->state--;
    }
    if (instance->base.gravityActorBinding != 0)
    {
        EQ_CDECL(void, 0x1004013B, (void *, void *))(instance, 0);
    }
    if (instance->base.gravityBinding != 0)
    {
        EQ_CDECL(void, 0x100401A6, (void *, void *))(instance, 0);
    }
    if (instance->blitSprite != 0)
    {
        EQ_CDECL(void, 0x1000D06A, (void *, void *))(context, instance->blitSprite);
    }
    if ((instance->dictFlagsLowByte & 1) != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(context, instance->entry);
    }
    result = (int *)EQ_CDECL(int, 0x10015D81, (void *, void *))(context, instance);
    if (instance->entry != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(instance->entry);
    }
    if ((instance->base.motionFlagBytes[1] & 0x20) != 0)
    {
        EQ_CDECL(void, 0x1003FF76, (void *, void *))(context, instance->definition_guess);
    }
    EQ_CDECL(void, 0x10041ADF, (void *))(&instance->base);
    EQ_CDECL(void, 0x10050CBB, (void *))(instance);
    return result;
}
#pragma optimize("y", on)

// FUNCTION: EQGFX 0x10040308
// Sets a particle cloud's duration relative to the time base.
// __cdecl (three arguments; the callee pops 0 bytes). Stores timeBase->value + duration at +0xC4 of the instance, returns 0.
int __cdecl t3dSetPCloudDuration(PCloudTimeBase_guess *timeBase, PCloudInst_guess *instance, int duration)
{
    instance->base.endTime_guess = timeBase->value + duration;
    return 0;
}

// FUNCTION: EQGFX 0x10040338
// Sets behavior flags for a particle cloud.
// __cdecl (two arguments on the stack; the callee pops 0 bytes). Stores `flags` at +0x14 of the passed
// instance record and returns 0.
int __cdecl t3dSetPCloudFlags(PCloudInst_guess *instance, int flags)
{
    instance->base.flags = flags;
    return 0;
}

// FUNCTION: EQGFX 0x1004064b
// Connects a particle cloud's rendering state to its source records.
// __cdecl (the state record first, the two source records after; the callee pops 0 bytes). Caches both source
// records on the state record and copies a dword from the first and three dwords from the second into it.
void __cdecl t3dSetPCloudRenderSources_guess(PCloudCtx_guess *context, PCloudSrcA_guess *sourceA, PCloudSrcB_guess *sourceB)
{
    context->valueCC = sourceA->value08;
    context->valueD8 = sourceB->value1AC;
    context->valueDC = sourceB->value1B0;
    context->valueE0 = sourceB->value1B4;
    context->sourceA = sourceA;
    context->sourceB = sourceB;
}

// FUNCTION: EQGFX 0x10040693
// Updates, culls and draws one particle cloud; destroys it when expired. Returns 0.
// Resolves the spawn binding to an actor or hierarchical-sprite reference, runs 0x1004064b on the cloud and its two
// shared objects, then may destroy it (0x10040217) or skip it on the hidden and collision checks.
// A switch on the render mode (1-4) walks the particle list: mode 1 projects points (0x10046c0b), mode 2 draws
// streaks (0x10047106), modes 3 and 4 use the frame and collision helpers (0x100477b9, 0x10030daa).
int __cdecl t3dPaintParticleCloud(PCloudSrcA_guess *displayContext,
                                 PCloudPaintArgument_guess renderContextArgument,
                                 PCloudCtx_guess **cloudReference)
{
    PCloudCtx_guess *cloud;
    PCloudSrcB_guess *renderContext = renderContextArgument.context;
    PCloudEffectRef_guess *spawnBinding;
    PCloudEffectRef_guess *hierarchicalReference;
    PCloudEffectRef_guess *resolvedReference;
    HSpriteChainNode_guess *chainNode;
    ParticleCloudRenderObject_guess *renderObject;
    ParticleCloudDrawState_guess *drawState;
    PCloudParticle_guess *particle;
    PCloudColor_guess particleColor;
    ParticleCloudRenderPacket_guess drawPacket;
    union {
        PCloudEffectRef_guess *reference;
        float projectedX;
    } referenceOrProjectedX;
    union {
        PCloudParticle_guess *particle;
        float fraction;
    } particleOrFraction;
    union {
        int integer;
        float channel;
    } greenChannel;
    union {
        int integer;
        float channel;
    } blueChannel;
    unsigned int cloudFlags;
    unsigned int methodFlags;
    unsigned int remainingTicks;
    unsigned int transparencyNumerator;
    unsigned int particleIndex;
    unsigned int frameIndex;
    unsigned int frameDuration;
    int renderMode;
    int nextParticleIndex;
    int alphaValue;
    int collisionResult;
    int frameResult;
    int transformParticles;
    float elapsedTime;
    float scale;
    float worldTransform[12];
    float particlePosition[3];
    float screenPosition[3];
    float previousScreenPosition[3];
    float depthOffsetY;
    float depthOffsetZ;
    float frameFraction;
    float quadHalfSize;
    float cornerX;
    float cornerY;
    float cornerZ;
    double trailX;
    float trailY;
    float trailZ;
    float colorScale;

    cloud = *cloudReference;
    hierarchicalReference = 0;
    referenceOrProjectedX.reference = 0;
    particleIndex = 0;
    scale = 0.0f;
    transformParticles = 0;
    if (cloud == 0)
        return 0;

    spawnBinding = cloud->spawnBinding;
    if (spawnBinding != 0)
    {
        if (spawnBinding->kind == PARTICLE_CLOUD_ACTOR_REFERENCE_TYPE)
        {
            referenceOrProjectedX.reference = spawnBinding;
        }
        else if (spawnBinding->kind == PARTICLE_CLOUD_HSPRITE_REFERENCE_TYPE)
        {
            hierarchicalReference = spawnBinding;
            if ((cloud->flagBytes[1] & 0x80) != 0 && spawnBinding->value0C.alternate != 0)
                hierarchicalReference = (PCloudEffectRef_guess *)spawnBinding->value0C.alternate;
            chainNode = (HSpriteChainNode_guess *)hierarchicalReference->value10;
            while (chainNode->link != 0)
                chainNode = (HSpriteChainNode_guess *)chainNode->link->nextNode;
            resolvedReference = (PCloudEffectRef_guess *)chainNode->extra;
            if (resolvedReference != 0)
                referenceOrProjectedX.reference = resolvedReference;
        }
    }

    EQ_CDECL(void, 0x1004064B, (PCloudCtx_guess *, PCloudSrcA_guess *, PCloudSrcB_guess *))
        (cloud, displayContext, renderContext);
    if (cloud->sharedObjectA != 0)
        EQ_CDECL(void, 0x1004064B, (PCloudCtx_guess *, PCloudSrcA_guess *, PCloudSrcB_guess *))
            (cloud->sharedObjectA, displayContext, renderContext);
    if (cloud->sharedObjectB != 0)
        EQ_CDECL(void, 0x1004064B, (PCloudCtx_guess *, PCloudSrcA_guess *, PCloudSrcB_guess *))
            (cloud->sharedObjectB, displayContext, renderContext);

    EQ_CDECL(void, 0x10041EE1, (void *))((void *)&cloud->flags);
    cloudFlags = cloud->flags;
    if (((((cloudFlags & 1) != 0) && (cloud->state == 0)) && (cloud->elapsed != 0)) &&
        (cloud->elapsed <= cloud->valueCC))
    {
        if (cloud->sharedObjectA != 0)
            EQ_CDECL(void, 0x10040320, (void *, void *))(displayContext, cloud->sharedObjectA);
        if (cloud->sharedObjectB != 0)
            EQ_CDECL(void, 0x10040320, (void *, void *))(displayContext, cloud->sharedObjectB);
        EQ_CDECL(void, 0x10040217, (void *, PCloudCtx_guess **))(displayContext, cloudReference);
        return 0;
    }
    if (((cloudFlags & 0x20000) != 0) && (referenceOrProjectedX.reference != 0) &&
        ((referenceOrProjectedX.reference->value0C.bytes.flags0F & 0x40) != 0))
        return 0;

    scale = cloud->scale;
    if (((cloudFlags & 0xC000) == 0) || (cloud->spawnBinding == 0))
    {
        if (((cloudFlags & 0x200) != 0) &&
            (collisionResult = EQ_CDECL(int, 0x10030E4A, (void *, float *, float *))
                (renderContext->collisionState, (float *)&cloud->boundsMin, (float *)&cloud->boundsMax),
             collisionResult == -1))
            return 0;
        if (((cloud->flagBytes[2] & 1) == 0) || (cloud->spawnBinding == 0))
            goto render_particles;
        if (hierarchicalReference == 0)
        {
            if (referenceOrProjectedX.reference != 0)
                scale = scale * referenceOrProjectedX.reference->value34;
            goto render_particles;
        }
        if (referenceOrProjectedX.reference == 0)
            goto render_particles;
    }
    else
    {
        EQGFX_REFERENCE_UPDATE_CALLBACK_GUESS((void *)&cloud->flags, (void *)&cloud->spawnBinding);
        if ((cloud->flagBytes[1] & 2) != 0)
        {
            elapsedTime = EQGFX_PARTICLE_CLOUD_TIMER_CALLBACK_GUESS(0);
            collisionResult = EQ_CDECL(int, 0x10030DAA, (void *, float *, float))
                (renderContext->collisionState, (float *)&cloud->spawnXYZ, elapsedTime);
            if (collisionResult == -1)
                return 0;
        }
        scale = cloud->scale;
        transformParticles = 1;
        if (hierarchicalReference == 0)
        {
            if (referenceOrProjectedX.reference != 0)
            {
                EQ_CDECL(void, 0x1004F3C6, (float, float, float, float *))
                    (referenceOrProjectedX.reference->orientation[0],
                     referenceOrProjectedX.reference->orientation[1],
                     referenceOrProjectedX.reference->orientation[2], worldTransform);
                worldTransform[9] = referenceOrProjectedX.reference->value10Float;
                worldTransform[10] = referenceOrProjectedX.reference->value14;
                worldTransform[11] = referenceOrProjectedX.reference->value18;
                scale = scale * referenceOrProjectedX.reference->value34;
            }
            goto render_particles;
        }
        if (referenceOrProjectedX.reference == 0)
            goto render_particles;
        worldTransform[0] = hierarchicalReference->transform[0];
        worldTransform[1] = hierarchicalReference->transform[1];
        worldTransform[2] = hierarchicalReference->transform[2];
        worldTransform[3] = hierarchicalReference->transform[3];
        worldTransform[4] = hierarchicalReference->transform[4];
        worldTransform[5] = hierarchicalReference->transform[5];
        worldTransform[6] = hierarchicalReference->transform[6];
        worldTransform[7] = hierarchicalReference->transform[7];
        worldTransform[8] = hierarchicalReference->transform[8];
        worldTransform[9] = hierarchicalReference->transform[9];
        worldTransform[10] = hierarchicalReference->transform[10];
        worldTransform[11] = hierarchicalReference->transform[11];
        scale = referenceOrProjectedX.reference->value34 * hierarchicalReference->value38 * scale;
    }

render_particles:
    renderMode = *cloud->renderMode;
    switch (renderMode)
    {
    case 1:
        particleIndex = (unsigned int)cloud->firstParticle;
        while (particleIndex != 0xFFFFFFFF)
        {
            particleOrFraction.particle = cloud->particles + particleIndex;
            particle = particleOrFraction.particle;
            nextParticleIndex = particle->nextIndex;
            if (transformParticles != 0)
            {
                particlePosition[0] = particle->pos.x * worldTransform[0] +
                    particle->pos.y * worldTransform[3] + particle->pos.z * worldTransform[6] + worldTransform[9];
                particlePosition[1] = particle->pos.x * worldTransform[1] +
                    particle->pos.y * worldTransform[4] + particle->pos.z * worldTransform[7] + worldTransform[10];
                particlePosition[2] = particle->pos.x * worldTransform[2] +
                    particle->pos.y * worldTransform[5] + particle->pos.z * worldTransform[8] + worldTransform[11];
            }
            else
            {
                particlePosition[0] = particle->pos.x;
                particlePosition[1] = particle->pos.y;
                particlePosition[2] = particle->pos.z;
            }
            if ((cloud->flagBytes[0] & 0x80) != 0)
            {
                particlePosition[0] = particlePosition[0] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3];
                particlePosition[1] = particlePosition[1] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3 + 1];
                particlePosition[2] = particlePosition[2] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3 + 2];
            }
            particlePosition[0] = particlePosition[0] + renderContext->originX;
            particlePosition[1] = particlePosition[1] + renderContext->originY;
            particlePosition[2] = particlePosition[2] + renderContext->originZ;
            screenPosition[0] = particlePosition[0] * renderContext->transform22C[7] +
                particlePosition[1] * renderContext->transform22C[8] + particlePosition[2] * renderContext->transform22C[9];
            screenPosition[1] = particlePosition[1] * renderContext->transform22C[11] +
                particlePosition[0] * renderContext->transform22C[10] + particlePosition[2] * renderContext->transform22C[12];
            screenPosition[2] = particlePosition[1] * renderContext->transform22C[3] +
                particlePosition[0] * renderContext->transform22C[0] + particlePosition[2] * renderContext->transform22C[6];
            if ((cloud->flagBytes[1] & 1) == 0)
            {
                alphaValue = 0x80;
            }
            else
            {
                remainingTicks = particle->nextFrameTick - particle->elapsedTicks;
                if (remainingTicks == 0)
                {
                    alphaValue = 0;
                }
                else
                {
                    transparencyNumerator = (unsigned int)(displayContext->frameIndex * 0xFF +
                        particle->elapsedTicks * -0xFF);
                    alphaValue = 0xFF - transparencyNumerator / remainingTicks;
                }
            }
            particleColor.dword = cloud->color;
            particleColor.bytes.a = (unsigned char)alphaValue;
            EQ_CDECL(void, 0x10046C0B, (PCloudSrcB_guess *, float *, PCloudColor_guess *, unsigned int))
                (renderContext, screenPosition, &particleColor, PARTICLE_CLOUD_RENDER_FLAGS);
            particleIndex = (unsigned int)nextParticleIndex;
        }
        break;
    case 2:
        particleIndex = (unsigned int)cloud->firstParticle;
        while (particleIndex != 0xFFFFFFFF)
        {
            particleOrFraction.particle = cloud->particles + particleIndex;
            particle = particleOrFraction.particle;
            nextParticleIndex = particle->nextIndex;
            if (transformParticles != 0)
            {
                particlePosition[0] = particle->pos.x * worldTransform[0] +
                    particle->pos.y * worldTransform[3] + particle->pos.z * worldTransform[6] + worldTransform[9];
                particlePosition[1] = particle->pos.x * worldTransform[1] +
                    particle->pos.y * worldTransform[4] + particle->pos.z * worldTransform[7] + worldTransform[10];
                particlePosition[2] = particle->pos.x * worldTransform[2] +
                    particle->pos.y * worldTransform[5] + particle->pos.z * worldTransform[8] + worldTransform[11];
            }
            else
            {
                particlePosition[0] = particle->pos.x;
                particlePosition[1] = particle->pos.y;
                particlePosition[2] = particle->pos.z;
            }
            if ((cloud->flagBytes[0] & 0x80) != 0)
            {
                particlePosition[0] = particlePosition[0] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3];
                particlePosition[1] = particlePosition[1] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3 + 1];
                particlePosition[2] = particlePosition[2] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3 + 2];
            }
            particlePosition[0] = particlePosition[0] + renderContext->originX;
            particlePosition[1] = particlePosition[1] + renderContext->originY;
            particlePosition[2] = particlePosition[2] + renderContext->originZ;
            screenPosition[0] = particlePosition[0] * renderContext->transform22C[7] +
                particlePosition[1] * renderContext->transform22C[8] + particlePosition[2] * renderContext->transform22C[9];
            screenPosition[1] = particlePosition[1] * renderContext->transform22C[11] +
                particlePosition[0] * renderContext->transform22C[10] + particlePosition[2] * renderContext->transform22C[12];
            screenPosition[2] = particlePosition[1] * renderContext->transform22C[3] +
                particlePosition[0] * renderContext->transform22C[0] + particlePosition[2] * renderContext->transform22C[6];
            particlePosition[0] = particlePosition[0] - particle->vel.x * EQGFX_PARTICLE_TRAIL_DELTA_GUESS;
            particlePosition[1] = particlePosition[1] - particle->vel.y * EQGFX_PARTICLE_TRAIL_DELTA_GUESS;
            particlePosition[2] = particlePosition[2] - particle->vel.z * EQGFX_PARTICLE_TRAIL_DELTA_GUESS;
            previousScreenPosition[0] = particlePosition[0] * renderContext->transform22C[7] +
                particlePosition[1] * renderContext->transform22C[8] + particlePosition[2] * renderContext->transform22C[9];
            previousScreenPosition[1] = particlePosition[1] * renderContext->transform22C[11] +
                particlePosition[0] * renderContext->transform22C[10] + particlePosition[2] * renderContext->transform22C[12];
            previousScreenPosition[2] = particlePosition[1] * renderContext->transform22C[3] +
                particlePosition[0] * renderContext->transform22C[0] + particlePosition[2] * renderContext->transform22C[6];
            EQ_CDECL(void, 0x10047106, (PCloudSrcB_guess *, float *, float *, unsigned int))
                (renderContext, screenPosition, previousScreenPosition, PARTICLE_CLOUD_RENDER_FLAGS);
            particleIndex = (unsigned int)nextParticleIndex;
        }
        break;
    case 3:
        renderObject = cloud->renderObject;
        drawState = renderObject->drawState;
        drawState->displayFrame = displayContext->frameIndex;
        methodFlags = renderObject->flags;
        if ((methodFlags & 0x80000000) != 0)
            methodFlags = renderContext->userMethods[methodFlags & 0x7FFFFFFF];
        drawPacket.enabled = 1;
        if ((methodFlags & 0x01000000) == 0)
            drawPacket.flags = 2;
        else
            drawPacket.flags = (unsigned char)(methodFlags >> 0x14) & 1 | 2;
        drawPacket.color = cloud->color;
        if (cloud->renderObject != 0)
        {
            particleIndex = (unsigned int)cloud->firstParticle;
            while (particleIndex != 0xFFFFFFFF)
            {
                particleOrFraction.particle = cloud->particles + particleIndex;
                particle = particleOrFraction.particle;
                nextParticleIndex = particle->nextIndex;
                if (transformParticles != 0)
                {
                    particlePosition[0] = particle->pos.x * worldTransform[0] +
                        particle->pos.y * worldTransform[3] + particle->pos.z * worldTransform[6] + worldTransform[9];
                    particlePosition[1] = particle->pos.x * worldTransform[1] +
                        particle->pos.y * worldTransform[4] + particle->pos.z * worldTransform[7] + worldTransform[10];
                    particlePosition[2] = particle->pos.x * worldTransform[2] +
                        particle->pos.y * worldTransform[5] + particle->pos.z * worldTransform[8] + worldTransform[11];
                }
                else
                {
                    particlePosition[0] = particle->pos.x;
                    particlePosition[1] = particle->pos.y;
                    particlePosition[2] = particle->pos.z;
                }
                if ((cloud->flagBytes[0] & 0x80) != 0)
                {
                    particlePosition[0] = particlePosition[0] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3];
                    particlePosition[1] = particlePosition[1] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3 + 1];
                    particlePosition[2] = particlePosition[2] + EQGFX_PARTICLE_DIRECTIONS_GUESS[particle->randomValue * 3 + 2];
                    particle->randomValue = (particle->randomValue + 1) & 0x1F;
                }
                if ((cloud->flagBytes[1] & 1) == 0)
                {
                    drawPacket.alpha = (char)0x80;
                }
                else
                {
                    remainingTicks = particle->nextFrameTick - particle->elapsedTicks;
                    if (remainingTicks == 0)
                    {
                        drawPacket.alpha = 0;
                    }
                    else
                    {
                        transparencyNumerator = (unsigned int)(displayContext->frameIndex * 0xFF +
                            particle->elapsedTicks * -0xFF);
                        drawPacket.alpha = (char)(-1 - (char)(transparencyNumerator / remainingTicks));
                    }
                }
                frameIndex = particle->frame;
                drawState->currentFrame = frameIndex;
                drawState->frameId = drawState->resource->frameIds[frameIndex];
                EQ_CDECL(void, 0x1002F0EB, (void *, PCloudSrcA_guess *, PCloudSrcB_guess *, void *, float *, int, float))
                    (cloud->renderObject, displayContext, renderContext, &drawPacket, particlePosition, 0, scale);
                particleIndex = (unsigned int)nextParticleIndex;
            }
        }
        break;
    case 4:
        renderObject = cloud->renderObject;
        drawState = renderObject->drawState;
        if (drawState != 0)
        {
            drawState->displayFrame = displayContext->frameIndex;
            methodFlags = renderObject->flags;
            if ((methodFlags & 0x80000000) != 0)
                methodFlags = renderContext->userMethods[methodFlags & 0x7FFFFFFF];
            drawPacket.enabled = 1;
            if ((methodFlags & 0x01000000) == 0)
                drawPacket.flags = 2;
            else
                drawPacket.flags = (unsigned char)(methodFlags >> 0x14) & 1 | 2;
            drawPacket.color = cloud->color;
            methodFlags = methodFlags | 0x40000000;
            EQ_CDECL(void, 0x1004EC8D, (float, int *))
                (renderContext->tint[0] * EQGFX_JITTER_SCALE_GUESS,
                 &renderContextArgument.integer);
            EQ_CDECL(void, 0x1004EC8D, (float, int *))
                (renderContext->tint[1] * EQGFX_JITTER_SCALE_GUESS, &greenChannel.integer);
            EQ_CDECL(void, 0x1004EC8D, (float, int *))
                (renderContext->tint[2] * EQGFX_JITTER_SCALE_GUESS, &blueChannel.integer);
            if (renderContextArgument.integer < 0x100)
            {
                if (renderContextArgument.integer < 0)
                    renderContextArgument.integer = 0;
            }
            else
            {
                renderContextArgument.integer = 0xFF;
            }
            if (greenChannel.integer < 0x100)
            {
                if (greenChannel.integer < 0)
                    greenChannel.integer = 0;
            }
            else
            {
                greenChannel.integer = 0xFF;
            }
            if (blueChannel.integer < 0x100)
            {
                if (blueChannel.integer < 0)
                    blueChannel.integer = 0;
            }
            else
            {
                blueChannel.integer = 0xFF;
            }
            if ((cloud->renderObject != 0) &&
                (particleIndex = (unsigned int)cloud->firstParticle, particleIndex != 0xFFFFFFFF))
            {
                quadHalfSize = scale * EQGFX_PARTICLE_SPRITE_RADIUS_SCALE_GUESS;
                do
                {
                    particleOrFraction.particle = cloud->particles + particleIndex;
                    particle = particleOrFraction.particle;
                    nextParticleIndex = particle->nextIndex;
                    if (transformParticles != 0)
                    {
                        particlePosition[0] = particle->pos.x * worldTransform[0] +
                            particle->pos.y * worldTransform[3] + particle->pos.z * worldTransform[6] + worldTransform[9];
                        particlePosition[1] = particle->pos.x * worldTransform[1] +
                            particle->pos.y * worldTransform[4] + particle->pos.z * worldTransform[7] + worldTransform[10];
                        particlePosition[2] = particle->pos.x * worldTransform[2] +
                            particle->pos.y * worldTransform[5] + particle->pos.z * worldTransform[8] + worldTransform[11];
                    }
                    else
                    {
                        particlePosition[0] = particle->pos.x;
                        particlePosition[1] = particle->pos.y;
                        particlePosition[2] = particle->pos.z;
                    }
                    collisionResult = EQ_CDECL(int, 0x10030DAA, (void *, float *, float))
                        (renderContext->collisionState, particlePosition, quadHalfSize);
                    if (collisionResult != -1)
                    {
                        renderContext->currentMethod = collisionResult;
                        remainingTicks = displayContext->frameIndex - particle->elapsedTicks;
                        frameDuration = particle->nextFrameTick - particle->elapsedTicks;
                        particleOrFraction.fraction = (float)((double)remainingTicks / (double)(int)frameDuration);
                        if ((cloud->flagBytes[1] & 1) == 0)
                        {
                            alphaValue = 0x80;
                        }
                        else
                        {
                            EQ_CDECL(void, 0x1004EC8D, (float, int *))
                                ((1.0f - particleOrFraction.fraction) * EQGFX_JITTER_SCALE_GUESS,
                                 &alphaValue);
                        }
                        drawPacket.alpha = (char)alphaValue;
                        trailX = particleOrFraction.fraction * scale;
                        frameIndex = particle->frame;
                        drawState->currentFrame = frameIndex;
                        drawState->frameId = drawState->resource->frameIds[frameIndex];
                        cornerX = particlePosition[2] + renderContext->originZ;
                        referenceOrProjectedX.projectedX = cornerX * renderContext->transform22C[9];
                        depthOffsetY = cornerX * renderContext->transform22C[12];
                        depthOffsetZ = cornerX * renderContext->transform22C[6];
                        if (EQGFX_ZERO_F_GUESS <= cornerX)
                        {
                            cornerX = renderContext->originX + particle->pos.x + trailX;
                            cornerY = (particle->pos.y - trailX) + renderContext->originY;
                            renderContext->quadVertices[0] = cornerX * renderContext->transform22C[7] +
                                cornerY * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[1] = cornerY * renderContext->transform22C[11] +
                                cornerX * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[2] = cornerY * renderContext->transform22C[3] +
                                cornerX * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[9] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[10] = greenChannel.channel;
                            renderContext->quadVertices[4] = 0.0f;
                            renderContext->quadVertices[11] = blueChannel.channel;
                            renderContext->quadVertices[5] = 1.0f;
                            cornerX = renderContext->originX + particle->pos.x + trailX;
                            cornerY = trailX + renderContext->originY + particle->pos.y;
                            renderContext->quadVertices[12] = cornerX * renderContext->transform22C[7] +
                                cornerY * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[13] = cornerY * renderContext->transform22C[11] +
                                cornerX * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[14] = cornerY * renderContext->transform22C[3] +
                                cornerX * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[21] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[22] = greenChannel.channel;
                            renderContext->quadVertices[16] = 1.0f;
                            renderContext->quadVertices[23] = blueChannel.channel;
                            renderContext->quadVertices[17] = 1.0f;
                            cornerX = (particle->pos.x - trailX) + renderContext->originX;
                            cornerY = trailX + renderContext->originY + particle->pos.y;
                            renderContext->quadVertices[24] = cornerX * renderContext->transform22C[7] +
                                cornerY * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[25] = cornerY * renderContext->transform22C[11] +
                                cornerX * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[26] = cornerY * renderContext->transform22C[3] +
                                cornerX * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[33] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[34] = greenChannel.channel;
                            renderContext->quadVertices[28] = 1.0f;
                            renderContext->quadVertices[35] = blueChannel.channel;
                            renderContext->quadVertices[29] = 0.0f;
                            particlePosition[0] = (particle->pos.x - trailX) + renderContext->originX;
                            particlePosition[1] = (particle->pos.y - trailX) + renderContext->originY;
                            renderContext->quadVertices[36] = particlePosition[0] * renderContext->transform22C[7] +
                                particlePosition[1] * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[37] = particlePosition[1] * renderContext->transform22C[11] +
                                particlePosition[0] * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[38] = particlePosition[1] * renderContext->transform22C[3] +
                                particlePosition[0] * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[45] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[46] = greenChannel.channel;
                            renderContext->quadVertices[40] = 0.0f;
                            renderContext->quadVertices[47] = blueChannel.channel;
                            cornerZ = 0.0f;
                        }
                        else
                        {
                            cornerX = (particle->pos.x - trailX) + renderContext->originX;
                            cornerY = (particle->pos.y - trailX) + renderContext->originY;
                            renderContext->quadVertices[0] = cornerX * renderContext->transform22C[7] +
                                cornerY * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[1] = cornerY * renderContext->transform22C[11] +
                                cornerX * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[2] = cornerY * renderContext->transform22C[3] +
                                cornerX * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[9] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[10] = greenChannel.channel;
                            renderContext->quadVertices[4] = 0.0f;
                            renderContext->quadVertices[11] = blueChannel.channel;
                            renderContext->quadVertices[5] = 0.0f;
                            cornerX = (particle->pos.x - trailX) + renderContext->originX;
                            cornerY = trailX + renderContext->originY + particle->pos.y;
                            renderContext->quadVertices[12] = cornerX * renderContext->transform22C[7] +
                                cornerY * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[13] = cornerY * renderContext->transform22C[11] +
                                cornerX * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[14] = cornerY * renderContext->transform22C[3] +
                                cornerX * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[21] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[22] = greenChannel.channel;
                            renderContext->quadVertices[16] = 1.0f;
                            renderContext->quadVertices[23] = blueChannel.channel;
                            renderContext->quadVertices[17] = 0.0f;
                            cornerX = renderContext->originX + particle->pos.x + trailX;
                            cornerY = trailX + renderContext->originY + particle->pos.y;
                            renderContext->quadVertices[24] = cornerX * renderContext->transform22C[7] +
                                cornerY * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[25] = cornerY * renderContext->transform22C[11] +
                                cornerX * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[26] = cornerY * renderContext->transform22C[3] +
                                cornerX * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[33] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[34] = greenChannel.channel;
                            renderContext->quadVertices[28] = 1.0f;
                            renderContext->quadVertices[35] = blueChannel.channel;
                            renderContext->quadVertices[29] = 1.0f;
                            particlePosition[0] = renderContext->originX + particle->pos.x + trailX;
                            particlePosition[1] = (particle->pos.y - trailX) + renderContext->originY;
                            renderContext->quadVertices[36] = particlePosition[0] * renderContext->transform22C[7] +
                                particlePosition[1] * renderContext->transform22C[8] + referenceOrProjectedX.projectedX;
                            renderContext->quadVertices[37] = particlePosition[1] * renderContext->transform22C[11] +
                                particlePosition[0] * renderContext->transform22C[10] + depthOffsetY;
                            renderContext->quadVertices[38] = particlePosition[1] * renderContext->transform22C[3] +
                                particlePosition[0] * renderContext->transform22C[0] + depthOffsetZ;
                            renderContext->quadVertices[45] = renderContextArgument.colorChannel;
                            renderContext->quadVertices[46] = greenChannel.channel;
                            renderContext->quadVertices[40] = 0.0f;
                            renderContext->quadVertices[47] = blueChannel.channel;
                            cornerZ = 1.0f;
                        }
                        renderContext->quadVertices[41] = cornerZ;
                        drawPacket.frameReference = **drawState->data;
                        frameResult = EQ_CDECL(int, 0x100477B9, (PCloudSrcB_guess *, int, int, void *, unsigned int))
                            (renderContext, 0, 4, &drawPacket, methodFlags);
                        if (frameResult < 1)
                        {
                            if (frameResult < 0)
                                return 0;
                        }
                        else
                        {
                            renderContext->drawCount = renderContext->drawCount + 1;
                        }
                    }
                    particleIndex = (unsigned int)nextParticleIndex;
                } while (particleIndex != 0xFFFFFFFF);
            }
        }
        break;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004165f
// Checks a moving point against world geometry and records its collision position and time.
int __cdecl t3dTraceParticleMotionSegment_guess(RayCollisionContext_guess *context, float *rayData)
{
    unsigned int elapsedFrames;
    float elapsedTime;
    float segmentEnd[3];
    RayCollisionHit_guess hit;
    float hitFraction;
    float collisionEpsilon;
    float collisionZero;
    int collisionResult;
    int collisionFrameOffset;

    elapsedFrames = context->elapsedFrames;
    elapsedTime = (float)elapsedFrames;
    segmentEnd[0] = elapsedTime * rayData[3] * EQGFX_RAY_COLLISION_EPSILON_GUESS + rayData[0];
    segmentEnd[1] = elapsedTime * rayData[4] * EQGFX_RAY_COLLISION_EPSILON_GUESS + rayData[1];
    segmentEnd[2] = elapsedTime * rayData[5] * EQGFX_RAY_COLLISION_EPSILON_GUESS + rayData[2];
    collisionResult = EQ_CDECL(int, 0x1000482E,
        (void *, void *, float *, float *, unsigned int, int, void *, void *, int, RayCollisionHit_guess *))(
            context->world->worldGeometry, context->world->collisionData, rayData, segmentEnd,
            0xE0000000, 0, (void *)0x1004161B, (void *)0x10041651, 0, &hit);
    if (collisionResult == 0)
    {
        ((int *)rayData)[16] = 0;
        ((int *)rayData)[6] = ((int *)rayData)[8] + elapsedFrames;
    }
    else
    {
        segmentEnd[0] = segmentEnd[0] - rayData[0];
        segmentEnd[1] = segmentEnd[1] - rayData[1];
        segmentEnd[2] = segmentEnd[2] - rayData[2];
        hitFraction = hit.fraction;
        collisionEpsilon = EQGFX_RAY_COLLISION_EPSILON_GUESS;
        collisionZero = EQGFX_ZERO_F_GUESS;
        if (hit.hitToken == 0)
        {
            ((int *)rayData)[16] = 1;
        }
        else
        {
            ((int *)rayData)[16] = hit.hitToken;
        }
        __asm
        {
            mov esi, rayData
            fld dword ptr [hitFraction]
            fsub dword ptr [collisionEpsilon]
            fcom dword ptr [collisionZero]
            fnstsw ax
            sahf
            jnc ray_collision_fraction_ready
            fstp st(0)
            fldz
        ray_collision_fraction_ready:
            fld st(0)
            fmul dword ptr [segmentEnd]
            fadd dword ptr [esi]
            fstp dword ptr [esi + 52]
            fld st(0)
            fmul dword ptr [segmentEnd + 4]
            fadd dword ptr [esi + 4]
            fstp dword ptr [esi + 56]
            fld st(0)
            fmul dword ptr [segmentEnd + 8]
            fadd dword ptr [esi + 8]
            fstp dword ptr [esi + 60]
            fmul dword ptr [elapsedTime]
        }
        collisionFrameOffset = EQGFX_ROUND_X87_GUESS();
        ((int *)rayData)[6] = collisionFrameOffset + ((int *)rayData)[8];
    }
    return 1;
}

// FUNCTION: EQGFX 0x10041767
// Builds the ramp and half-scale direction table using the renderer's trigonometric helpers.
int __cdecl t3dInitParticleCloudTables(void)
{
    float rampValue = 0.0f;
    float *rampEntry;
    int index;
    double angle;
    float *directionEntry;

    rampEntry = EQGFX_PARTICLE_RAMP_GUESS;
    do
    {
        *rampEntry = rampValue * EQGFX_PARTICLE_RAMP_SCALE_GUESS;
        rampEntry = rampEntry + 1;
        rampValue = rampValue + EQGFX_FLOAT_ONE;
    } while ((int)rampEntry < EQGFX_PARTICLE_RAMP_END_GUESS);

    index = 0;
    EQGFX_PARTICLE_DIRECTIONS_GUESS[0] = 0.0f;
    directionEntry = EQGFX_PARTICLE_DIRECTIONS_GUESS + 1;
    EQGFX_PARTICLE_DIRECTIONS_GUESS[1] = 0.0f;
    EQGFX_PARTICLE_DIRECTIONS_GUESS[2] = 0.0f;
    do
    {
        angle = (double)((float)index * EQGFX_PARTICLE_ANGLE_STEP_GUESS);
        directionEntry[-1] = (float)(EQGFX_PARTICLE_COS_GUESS(angle) * EQGFX_MOTION_HALF_GUESS);
        index = index + 1;
        directionEntry[0] = (float)(EQGFX_PARTICLE_SIN_GUESS(angle) * EQGFX_MOTION_HALF_GUESS);
        directionEntry[1] = 0.0f;
        directionEntry = directionEntry + 3;
    } while ((int)directionEntry < EQGFX_PARTICLE_DIRECTION_END_GUESS);
    return 0;
}

// FUNCTION: EQGFX 0x10041802
// Sets the reusable particle effect parameters, defaulting omitted vectors to zero and omitted color to white.
// __cdecl (the definition record first, nineteen source values after; the callee pops 0 bytes). Fills the
// definition: leading dwords, six vectors (four zeroed when their pointer is null) and the color word (0xFFFFFF
// when its pointer is null). Returns 0.
int __cdecl t3dInitParticleCloudDefBase(PCloudDef_guess *definition, int value00, int value04, int value08,
                                        int value0C, Float3_guess *vector10, Float3_guess *vector1C,
                                        Float3_guess *vector28, int value34, int value38, Float3_guess *vector3C,
                                        Float3_guess *vector48, Float3_guess *vector54, int value60, int value64,
                                        int value68, int value6C, int value70, int value74, int *color)
{
    definition->value00 = value00;
    definition->value04 = value04;
    definition->value08 = value08;
    definition->value0C = value0C;
    if (vector10 != 0)
        definition->vector10 = *vector10;
    else {
        definition->vector10.x = 0.0f;
        definition->vector10.y = 0.0f;
        definition->vector10.z = 0.0f;
    }
    if (vector1C != 0)
        definition->vector1C = *vector1C;
    else {
        definition->vector1C.x = 0.0f;
        definition->vector1C.y = 0.0f;
        definition->vector1C.z = 0.0f;
    }
    if (vector28 != 0)
        definition->vector28 = *vector28;
    else {
        definition->vector28.x = 0.0f;
        definition->vector28.y = 0.0f;
        definition->vector28.z = 0.0f;
    }
    definition->mode = value34;
    definition->value38 = value38;
    if (vector3C != 0)
        definition->vector3C = *vector3C;
    if (vector48 != 0)
        definition->vector48 = *vector48;
    if (vector54 != 0)
        definition->vector54 = *vector54;
    else {
        definition->vector54.x = 0.0f;
        definition->vector54.y = 0.0f;
        definition->vector54.z = 0.0f;
    }
    definition->value60 = value60;
    definition->value64 = value64;
    definition->value68 = value68;
    definition->value6C = value6C;
    definition->value70 = value70;
    definition->value74 = value74;
    if (color != 0)
        definition->color.dword = *color;
    else {
        definition->color.bytes.b |= 0xff;
        definition->color.bytes.g |= 0xff;
        definition->color.bytes.r |= 0xff;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10040346
// Sets the particle cloud's bounding box.
// __cdecl (three arguments on the stack; the callee pops 0 bytes). Copies the three-float corners `boundsMin` and
// `boundsMax` to +0x38 and +0x44 of the passed instance record and returns 0.
int __cdecl t3dSetPCloudBBox(PCloudInst_guess *instance, Float3_guess *boundsMin, Float3_guess *boundsMax)
{
    instance->base.boundsMin = *boundsMin;
    instance->base.boundsMax = *boundsMax;
    return 0;
}

// FUNCTION: EQGFX 0x10040365
// Sets the particle cloud's gravity and whether gravity affects its motion.
// __cdecl (two arguments on the stack; the callee pops 0 bytes). Stores the float `gravity` at +0x24 of the passed
// instance record, then sets bit 0x10 of the byte at +0x15 when gravity is not 0.0 and clears it when it is. Returns 0.
int __cdecl t3dSetPCloudGravity(PCloudInst_guess *instance, float gravity)
{
    instance->base.gravity = gravity;
    if (gravity != EQGFX_ZERO_F_GUESS)
    {
        instance->base.motionFlagBytes[1] |= PCLOUD_INST_GRAVITY_BIT_GUESS;
    }
    else
    {
        instance->base.motionFlagBytes[1] &= ~PCLOUD_INST_GRAVITY_BIT_GUESS;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004038A
// Sets where new particles spawn.
// __cdecl (two arguments on the stack; the callee pops 0 bytes). Copies the three-float vector `xyz` to +0x54 of the
// passed instance record and returns 0.
int __cdecl t3dSetPCloudSpawnXYZ(PCloudInst_guess *instance, Float3_guess *xyz)
{
    instance->base.spawnXYZ = *xyz;
    return 0;
}

// FUNCTION: EQGFX 0x1004039F
// Sets the base direction used when particles spawn.
// __cdecl (two arguments on the stack; the callee pops 0 bytes). Passes the sub-record at +0x14 of the instance and the
// spawn normal to t3dSetPCloudBaseSpawnNormal and returns 0.
int __cdecl t3dSetPCloudSpawnNormal(PCloudInst_guess *instance, float *normal)
{
    EQ_CDECL(int, 0x10041AF4, (void *, float *))(&instance->base, normal);
    return 0;
}

// FUNCTION: EQGFX 0x100403B5
// Sets the radius of the particle cloud's spawn area.
// __cdecl (two arguments on the stack; the callee pops 0 bytes). Stores the float `radius` at +0x84 of the passed
// instance record and returns 0.
int __cdecl t3dSetPCloudSpawnRadius(PCloudInst_guess *instance, float radius)
{
    instance->base.spawnRadius = radius;
    return 0;
}

// FUNCTION: EQGFX 0x100403C6
// Sets the particle cloud's spawn angle.
// __cdecl (two arguments on the stack; the callee pops 0 bytes). Stores the float `angle` at +0x88 of the passed
// instance record and returns 0.
int __cdecl t3dSetPCloudSpawnAngle(PCloudInst_guess *instance, float angle)
{
    instance->base.spawnAngle = angle;
    return 0;
}

// FUNCTION: EQGFX 0x100403D7
// __cdecl. Stores `lifespan` at +0x8C of the passed instance record and returns 0.
int __cdecl t3dSetPCloudSpawnLifespan(PCloudInst_guess *instance, int lifespan)
{
    instance->base.lifespan = lifespan;
    return 0;
}

// FUNCTION: EQGFX 0x100403E8
// __cdecl. Stores the float `velocity` at +0x90 of the passed instance record and returns 0.
int __cdecl t3dSetPCloudSpawnVelocity(PCloudInst_guess *instance, float velocity)
{
    instance->base.velocity = velocity;
    return 0;
}

// FUNCTION: EQGFX 0x100403F9
// __cdecl. Stores `rate` at +0x94 of the passed instance record and returns 0.
int __cdecl t3dSetPCloudSpawnRate(PCloudInst_guess *instance, int rate)
{
    instance->base.spawnRate = rate;
    return 0;
}

// FUNCTION: EQGFX 0x1004040A
// __cdecl. Stores the float `scale` at +0xBC of the passed instance record and returns 0.
int __cdecl t3dSetPCloudSpawnScale(PCloudInst_guess *instance, float scale)
{
    instance->base.spawnScale = scale;
    return 0;
}

// FUNCTION: EQGFX 0x1004041b
// __cdecl. Copies the dword at `color` to +0xC8 of the passed instance record and returns 0.
int __cdecl t3dSetPCloudColor(PCloudInst_guess *instance, int *color)
{
    instance->base.color.dword = *color;
    return 0;
}

// FUNCTION: EQGFX 0x1004042E
// __cdecl. Replaces the blit sprite at +0x110 with one made from `spriteDesc`; returns 0 or the bind error.
int __cdecl t3dSetPCloudBlitsprite(void *dictionary, PCloudInst_guess *instance, void *spriteDesc)
{
    int result;
    BlitSprite_guess *sprite;
    BlitSpriteSource_guess *source;

    result = 0;
    if (instance->blitSprite != 0)
    {
        EQ_CDECL(void, 0x1000D06A, (void *, void *))(dictionary, instance->blitSprite);
    }
    sprite = EQ_CDECL(BlitSprite_guess *, 0x1000CFBF, (void *, void *))(dictionary, spriteDesc);
    instance->blitSprite = sprite;
    if (sprite == 0)
    {
        result = PARTICLE_CLOUD_BIND_ERROR;
    }
    else
    {
        source = sprite->source;
        instance->base.frameCount = source->definition->value14;
        instance->base.frameStep = source->definition->buffer->value1C;
        instance->base.spriteBuffer18Value_guess = source->definition->buffer->value18;
    }
    return result;
}

// FUNCTION: EQGFX 0x1004049e
// Resolves inherited coordinates from an actor or hierarchical-sprite reference.
// __cdecl. Copies three dwords through output for actor (0x18) and hsprite (0x3B) references, updating the
// hierarchical sprite first for 0x3B; any other kind or a null reference stores nothing. Returns 0.
int __cdecl t3dEvaluateInheritedXYZ_T3D(PCloudCtx_guess *context, int *output)
{
    PCloudEffectRef_guess *ref;
    HSpriteChainNode_guess *node;
    void *request;
    void *extra;

    request = *(void **)context->valueDC;
    ref = *(PCloudEffectRef_guess **)output;
    if (ref != 0)
    {
        if (ref->kind != PARTICLE_CLOUD_ACTOR_REFERENCE_TYPE)
        {
            if (ref->kind == PARTICLE_CLOUD_HSPRITE_REFERENCE_TYPE)
            {
                node = (HSpriteChainNode_guess *)ref->value10;
                while (node->link != 0)
                {
                    node = (HSpriteChainNode_guess *)node->link->nextNode;
                }
                extra = node->extra;
                if (extra != 0)
                {
                    EQ_CDECL(int, 0x100327C3, (void *, void *, void *))(node, request, extra);
                    output[1] = ref->value80;
                    output[2] = ref->value84;
                    output[3] = ref->value88;
                }
            }
        }
        else
        {
            *(ThreeDwords_guess *)(output + 1) = *(ThreeDwords_guess *)&ref->value10;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004050e
// Resolves an attached effect's orientation from its local rotation and actor or hierarchical sprite reference.
int __cdecl t3dUpdateSpawnMatrix_T3D(EqGfxTransformState_guess *state, float *outputMatrix)
{
    float localMatrix[9];
    EqGfxTransformReference_guess *reference;

    reference = state->reference;
    if (reference != 0)
    {
        EQGFX_REFERENCE_UPDATE_CALLBACK_GUESS(state, &state->reference);
        reference = state->reference;
        if (reference->kind != 0x18)
        {
            if (reference->kind != 0x3B)
                return 1;
            if ((state->flags01 & 0x80) != 0 && reference->valid != 0)
            {
                ((unsigned int *)state->position)[0] = ((unsigned int *)reference->position)[0];
                ((unsigned int *)state->position)[1] = ((unsigned int *)reference->position)[1];
                ((unsigned int *)state->position)[2] = ((unsigned int *)reference->position)[2];
                EQ_CDECL(void, 0x1004F9A2, (float *, float *))(reference->orientation, localMatrix);
                EQ_CDECL(void, 0x1004F4FB, (float *, float *, float *))(
                    state->matrix, localMatrix, outputMatrix);
                return 1;
            }
            if ((state->flags01 & 0x40) == 0)
            {
                EQ_CDECL(void, 0x1004F4FB, (float *, float *, float *))(
                    state->matrix, reference->matrix, outputMatrix);
                return 1;
            }
        }
        else if ((state->flags01 & 0x40) == 0)
        {
            goto calculateActorMatrix;
        }
        state->position[0] = 0.0f;
        state->position[1] = 0.0f;
        state->position[2] = 0.0f;
    }
    outputMatrix[0] = state->matrix[0];
    ((unsigned int *)outputMatrix)[1] = ((unsigned int *)state->matrix)[1];
    ((unsigned int *)outputMatrix)[2] = ((unsigned int *)state->matrix)[2];
    ((unsigned int *)outputMatrix)[3] = ((unsigned int *)state->matrix)[3];
    ((unsigned int *)outputMatrix)[4] = ((unsigned int *)state->matrix)[4];
    ((unsigned int *)outputMatrix)[5] = ((unsigned int *)state->matrix)[5];
    ((unsigned int *)outputMatrix)[6] = ((unsigned int *)state->matrix)[6];
    ((unsigned int *)outputMatrix)[7] = ((unsigned int *)state->matrix)[7];
    ((unsigned int *)outputMatrix)[8] = ((unsigned int *)state->matrix)[8];
    return 1;

calculateActorMatrix:
    EQ_CDECL(void, 0x1004F3C6, (float, float, float, float *))(
        reference->orientation[0], reference->orientation[1], reference->orientation[2], localMatrix);
    EQ_CDECL(void, 0x1004F4FB, (float *, float *, float *))(
        state->matrix, localMatrix, outputMatrix);
    return 1;
}

// FUNCTION: EQGFX 0x10041903
// Prepares a cloud's particle pool and emission settings from its definition, density scale, and spawn offset.
// __cdecl (seven stack arguments; the callee pops 0 bytes). Fills the instance from the descriptor (its vectors offset by
// spawnOffset) and allocates and chains the node array. Returns 0, or -1 when the allocation fails.
int __cdecl t3dInitPCloudInstBase(PCloudInstBase_guess *instance, PCloudDef_guess *descriptor, int baseOffset, float spawnScale,
                                  Float3_guess *spawnOffset, int spriteBuffer1C, int spriteBuffer18)
{
    float forceScale;
    unsigned int nodeIndex;
    int nodeOffset;
    ParticleNode_guess *nodes;
    Float3_guess *vector24;
    int baseValue;

    instance->descriptor = descriptor;
    instance->flags = descriptor->value04;
    instance->allocCount = 0;
    instance->nodeCount = (int)((float)(unsigned int)descriptor->value08 * spawnScale);
    if (instance->nodeCount == 0)
    {
        instance->nodes = 0;
    }
    else
    {
        nodes = EQ_CDECL(void *, 0x10050C48, (int))(instance->nodeCount * sizeof(ParticleNode_guess));
        instance->nodes = nodes;
        if (nodes == 0)
            return -1;
        nodeIndex = 0;
        if (instance->nodeCount != 0)
        {
            nodeOffset = 0;
            do
            {
                nodeIndex = nodeIndex + 1;
                ((ParticleNode_guess *)((char *)instance->nodes + nodeOffset))->next = nodeIndex;
                nodeOffset = nodeOffset + sizeof(ParticleNode_guess);
            } while (nodeIndex < (unsigned int)instance->nodeCount);
        }
        instance->nodes[nodeIndex - 1].next = -1;
    }
    instance->freeHead = 0;
    instance->allocHead = -1;
    forceScale = descriptor->forceScale;
    instance->gravity = forceScale;
    if (forceScale != EQGFX_ZERO_F_GUESS)
        instance->motionFlagBytes[1] |= PCLOUD_INST_FORCE_FLAG_BIT_GUESS;
    instance->gravityBinding_int_guess = 0;
    instance->vector18_guess = descriptor->vector10;
    vector24 = &instance->boundsMin;
    *vector24 = descriptor->vector1C;
    vector24->x = vector24->x + spawnOffset->x;
    vector24->y = spawnOffset->y + vector24->y;
    vector24->z = spawnOffset->z + vector24->z;
    instance->boundsMax = descriptor->vector28;
    instance->boundsMax.x = instance->boundsMax.x + spawnOffset->x;
    instance->boundsMax.y = spawnOffset->y + instance->boundsMax.y;
    instance->gravityActorBinding = 0;
    instance->boundsMax.z = spawnOffset->z + instance->boundsMax.z;
    instance->spawnXYZ = *spawnOffset;
    instance->vector4C_guess = descriptor->vector3C;
    instance->vector58_guess = descriptor->vector48;
    EQ_CDECL(int, 0x10041AF4, (void *, float *))(instance, &descriptor->vector54.x);
    instance->spawnRadius_bits = descriptor->value60;
    instance->spawnAngle_bits = descriptor->value64;
    instance->lifespan = descriptor->value68;
    instance->velocity_bits = descriptor->value6C;
    if (spawnScale <= EQGFX_ZERO_F_GUESS)
        instance->spawnRate = -1;
    else
        instance->spawnRate = (int)((float)(unsigned int)descriptor->value70 * (1.0f / spawnScale));
    instance->spawnScale_bits = descriptor->value74;
    if (descriptor->value38 == 0)
        baseValue = 0;
    else
        baseValue = descriptor->value38 + baseOffset;
    instance->endTime_guess = baseValue;
    instance->color = descriptor->color;
    instance->spriteBuffer1C = spriteBuffer1C;
    instance->spriteBuffer18 = spriteBuffer18;
    instance->lastUpdateTicks_guess = 0;
    instance->spawnTickRemainder_guess = 0;
    instance->localDescriptor_guess = 0;
    instance->frameCount = 1;
    instance->frameStep = 0;
    instance->spriteBuffer18Value_guess = 0;
    return 0;
}

// FUNCTION: EQGFX 0x10041adf
// Releases a particle cloud's allocated particle storage.
// __cdecl (one pointer argument on the stack; the callee pops 0 bytes). Frees the allocation kept at +0xC of the
// passed instance record (only when it is nonzero) and returns 0; the callers never read EAX.
int __cdecl t3dFreePCloudInstBase(PCloudInstBase_guess *instance)
{
    void *base = instance->nodes;

    if (base != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(base);
    return 0;
}

// FUNCTION: EQGFX 0x10041af4
// Sets the base particle emission direction and prepares perpendicular axes for directional spawn modes.
// __cdecl (the instance record first, the spawn normal pointer second; the callee pops 0 bytes). Copies the
// normal to +0x64 and, for descriptor modes 2/3/4/5, builds the spawn frame: the normal crossed with a unit
// axis (Z, or X when that is degenerate) and that axis crossed with the normal. Returns 0.
int __cdecl t3dSetPCloudBaseSpawnNormal(PCloudInstBase_guess *instance, float *normal)
{
    Float3_guess n;
    Float3_guess up;
    float axisX;
    float axisY;
    float axisZ;
    int mode;

    instance->spawnNormal = *(Float3_guess *)normal;
    mode = instance->descriptor->mode;
    if (mode == 4 || mode == 2 || mode == 5 || mode == 3) {
        n = *(Float3_guess *)normal;
        up.x = 0.0f;
        up.y = 0.0f;
        up.z = 1.0f;
        axisX = n.y * up.z - n.z * up.y;
        axisY = n.z * up.x - n.x * up.z;
        axisZ = n.x * up.y - n.y * up.x;
        if (axisX < EQGFX_SPAWN_AXIS_EPS_GUESS && axisX > EQGFX_SPAWN_AXIS_NEG_EPS_GUESS &&
            axisY < EQGFX_SPAWN_AXIS_EPS_GUESS && axisY > EQGFX_SPAWN_AXIS_NEG_EPS_GUESS &&
            axisZ < EQGFX_SPAWN_AXIS_EPS_GUESS && axisZ > EQGFX_SPAWN_AXIS_NEG_EPS_GUESS) {
            up.x = 1.0f;
            up.y = 0.0f;
            up.z = 0.0f;
            axisX = n.y * up.z - n.z * up.y;
            axisY = n.z * up.x - n.x * up.z;
            axisZ = n.x * up.y - n.y * up.x;
        }
        instance->cross.x = axisY * n.z - axisZ * n.y;
        instance->cross.y = axisZ * n.x - axisX * n.z;
        instance->cross.z = axisX * n.y - axisY * n.x;
        instance->axis.x = axisX;
        instance->axis.y = axisY;
        instance->axis.z = axisZ;
        instance->normal.x = n.x;
        instance->normal.y = n.y;
        instance->normal.z = n.z;
    }
    return 0;
}

// FUNCTION: EQGFX 0x100425ba
// Chooses a particle's emission direction according to the cloud's spawn mode before spawning it.
void __cdecl t3dSpawnPCloudParticleByMode_guess(PCloudInstBase_guess *instance, PCloudSpawnContext_guess *positionContext, float *spawnAxes)
{
    Float3_guess velocity;
    Float3_guess *direction;
    unsigned int remainingTicks;
    unsigned int randomValue;
    int mode;
    float timeStep;
    float angle;
    float secondAngle;
    float vertical;
    float cosine;
    float sine;

    if ((unsigned int)instance->allocCount >= (unsigned int)instance->nodeCount) {
        return;
    }
    vertical = 0.0f;
    remainingTicks = (unsigned int)instance->baseTicks - (unsigned int)positionContext->elapsedTicks;
    mode = instance->descriptor->mode;
    timeStep = (float)remainingTicks * EQGFX_PARTICLE_SPAWN_TIME_SCALE_GUESS;
    if (mode == 0) {
        direction = &instance->spawnNormal;
    } else {
        if (mode == 1) {
            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
            angle = (float)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
            secondAngle = (float)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
            sine = EQ_CDECL(float, 0x1004EE87, (float))(secondAngle);
            cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle);
            velocity.x = cosine * sine;
            sine = EQ_CDECL(float, 0x1004EE87, (float))(secondAngle);
            angle = EQ_CDECL(float, 0x1004EE87, (float))(angle);
            velocity.y = angle * sine;
            velocity.z = EQ_CDECL(float, 0x1004EEFA, (float))(secondAngle);
            EQ_CDECL(void, 0x100427BB, (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
                instance, (Float3_guess *)positionContext, &velocity, timeStep);
            return;
        }
        if (mode == 2) {
            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
            angle = (float)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
            cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle);
            sine = EQ_CDECL(float, 0x1004EE87, (float))(angle);
            velocity.x = cosine * spawnAxes[0] + sine * spawnAxes[3];
            velocity.y = cosine * spawnAxes[1] + sine * spawnAxes[4];
            velocity.z = cosine * spawnAxes[2] + sine * spawnAxes[5];
            direction = &velocity;
        } else {
            if (mode != 3) {
                if ((mode != 4) && (mode != 5)) {
                    return;
                }
                direction = &instance->spawnNormal;
            } else {
                randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))();
                vertical = EQ_CDECL(float, 0x1004EE87, (float))(
                    (float)randomValue * instance->spawnAngle * EQGFX_PARTICLE_SPAWN_RANDOM_SCALE_GUESS);
                randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
                angle = (float)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
                cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle) * vertical;
                sine = EQ_CDECL(float, 0x1004EE87, (float))(angle) * vertical;
                velocity.x = cosine * spawnAxes[0] + sine * spawnAxes[3] + spawnAxes[6];
                velocity.y = cosine * spawnAxes[1] + sine * spawnAxes[4] + spawnAxes[7];
                velocity.z = cosine * spawnAxes[2] + sine * spawnAxes[5] + spawnAxes[8];
                direction = &velocity;
            }
        }
    }
    EQ_CDECL(void, 0x100427BB, (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
        instance, (Float3_guess *)positionContext, direction, timeStep);
    return;
}

// FUNCTION: EQGFX 0x100427bb
// Spawns one cloud particle with a supplied position and velocity, advancing it by the initial time step.
void __cdecl t3dSpawnPCloudParticle_guess(PCloudInstBase_guess *instance, Float3_guess *position, Float3_guess *velocity, float timeStep)
{
    int particleIndex;
    PCloudParticle_guess *particle;
    int elapsedTicks;
    int remainingTicks;
    float scaledTimeStep;

    particleIndex = EQ_CDECL(int, 0x100428B6, (PCloudInstBase_guess *))(instance);
    particle = (PCloudParticle_guess *)&instance->nodes[particleIndex];
    particle->pos = *position;
    if ((instance->flags & 8) != 0) {
        particle->pos.x = particle->pos.x - instance->positionOffset.x;
    }
    if ((instance->flags & 0x10) != 0) {
        particle->pos.y = particle->pos.y - instance->positionOffset.y;
    }
    if ((instance->flags & 0x20) != 0) {
        particle->pos.z = particle->pos.z - instance->positionOffset.z;
    }
    particle->vel = *velocity;
    particle->vel.x = particle->vel.x * instance->velocity;
    particle->vel.y = particle->vel.y * instance->velocity;
    particle->vel.z = particle->vel.z * instance->velocity;
    scaledTimeStep = timeStep;
    scaledTimeStep = scaledTimeStep * EQGFX_PARTICLE_TICK_SCALE_GUESS;
    EQ_CDECL(void, 0x1004EC8D, (float, int *))(scaledTimeStep, &elapsedTicks);
    remainingTicks = instance->baseTicks - elapsedTicks;
    particle->elapsedTicks = remainingTicks;
    if ((instance->flags & 2) != 0) {
        EQGFX_PARTICLE_UPDATE_CALLBACK_GUESS(instance, particle);
    } else {
        particle->nextFrameTick = instance->lifespan + remainingTicks;
    }
    particle->randomValue = EQ_CDECL(int, 0x10069D8E, (void))() & 0x1F;
    if (instance->frameCount != 1) {
        particle->frameValue = instance->frameValue;
        particle->frameTime = instance->frameStep + particle->elapsedTicks;
    }
    EQ_CDECL(int, 0x10041C42, (void *, PCloudParticle_guess *, float, int))(
        instance, particle, timeStep, elapsedTicks);
    return;
}

// FUNCTION: EQGFX 0x100428b6
// Reserves a free slot for a particle in the cloud.
// __cdecl (the pool record first; the callee pops 0 bytes). Pops the free-list head off the free list and
// pushes it onto the allocated list (+0x8 counts the allocations). Returns the node index, or -1 when empty.
int __cdecl allocateParticleFromPool_guess(PCloudInstBase_guess *instance)
{
    int index;
    int previousHead;

    index = instance->freeHead;
    previousHead = instance->allocHead;
    if (index != -1) {
        instance->allocCount = instance->allocCount + 1;
        instance->freeHead = instance->nodes[index].next;
        instance->nodes[index].next = previousHead;
        instance->nodes[index].prev = -1;
        if (previousHead != -1) {
            instance->nodes[previousHead].prev = index;
        }
        instance->allocHead = index;
    }
    return index;
}

// FUNCTION: EQGFX 0x1004290a
// Returns a particle slot to the cloud's available pool.
// __cdecl (the pool record first, the node index second; the callee pops 0 bytes). Unlinks the node from the
// allocated list (+0xE4 head) and pushes it back onto the free list (+0xE0); +0x8 counts the live allocations down.
void __cdecl t3dFreeParticleToPool_guess(PCloudInstBase_guess *instance, int index)
{
    ParticleNode_guess *node;

    instance->allocCount = instance->allocCount - 1;
    node = &instance->nodes[index];
    if (node->next != -1) {
        instance->nodes[node->next].prev = node->prev;
    }
    if (node->prev != -1) {
        instance->nodes[node->prev].next = node->next;
    } else {
        instance->allocHead = node->next;
    }
    node->next = instance->freeHead;
    instance->freeHead = index;
}
// FUNCTION: EQGFX 0x10041EE1
// Per-frame cloud tick: advances live particles, retires expired ones, emits new ones by rate and shape mode
int __cdecl t3dUpdateParticleCloud(PCloudInstBase_guess *instance)
{
    float alternateAxes[9];
    float primaryAxes[9];
    float spawnAxes[9];
    PCloudParticle_guess *particle;
    int elapsedTicks;
    int currentIndex;
    int nextIndex;
    unsigned int spawnRate;
    unsigned int spawnCountFromTime;
    unsigned int randomValue;
    int randomInteger;
    float scaledElapsedTime;
    int spawnCount;
    int savedSpawnCount;
    float alternateAxesReady;
    float primaryAxesReady;
    int countForCurrentSpawn;
    float angle;
    float secondAngle;
    float vertical;
    float radius;
    float alongAxis;
    float acrossAxis;
    float cosine;
    float sine;
    Float3_guess position;
    Float3_guess velocity;
    float *minimumBounds;
    float *maximumBounds;
    void *attachedSprite;
    float *axes;
    float rampTime;

    alternateAxesReady = 0.0f;
    primaryAxesReady = 0.0f;
    savedSpawnCount = 0;
    if (instance->lastUpdateTicks_guess == 0) {
        instance->lastUpdateTicks_guess = instance->baseTicks;
    }
    elapsedTicks = instance->baseTicks - instance->lastUpdateTicks_guess;
    instance->lastUpdateTicks_guess = instance->baseTicks;
    if (instance->gravityBinding_int_guess != 0) {
        EQGFX_REFERENCE_UPDATE_CALLBACK_GUESS(instance, &instance->gravityBinding);
    }
    scaledElapsedTime = (float)(unsigned int)elapsedTicks * EQGFX_PARTICLE_SPAWN_TIME_SCALE_GUESS;
    spawnCount = 0;
    currentIndex = instance->allocHead;
    while (1) {
        while (1) {
            if (currentIndex == -1) {
                if ((instance->endTime_guess == 0) ||
                    ((unsigned int)instance->baseTicks < (unsigned int)instance->endTime_guess)) {
                    spawnRate = (unsigned int)instance->spawnRate;
                    if (spawnRate == 0) {
                        spawnCountFromTime = 1;
                    } else {
                        spawnCountFromTime =
                            (unsigned int)(instance->spawnTickRemainder_guess + elapsedTicks) / spawnRate;
                    }
                    instance->spawnTickRemainder_guess =
                        instance->spawnTickRemainder_guess + (elapsedTicks - spawnRate * spawnCountFromTime);
                    spawnCount = spawnCount + (int)spawnCountFromTime;
                    if ((unsigned int)instance->nodeCount <
                        (unsigned int)(instance->allocCount + (int)spawnCount)) {
                        spawnCount = instance->nodeCount - instance->allocCount;
                        savedSpawnCount = spawnCount;
                    }
                    if (spawnCount != 0) {
                        EQGFX_PARTICLE_CLOUD_AXIS_CALLBACK_GUESS(instance, spawnAxes);
                    }
                    switch (instance->descriptor->mode) {
                    case 0:
                        if (spawnCount != 0) {
                            minimumBounds = &instance->vector4C_guess.x;
                            maximumBounds = &instance->vector58_guess.x;
                            do {
                                randomInteger = EQ_CDECL(int, 0x10069D8E, (void))();
                                position.x = (maximumBounds[0] - minimumBounds[0]) * (float)randomInteger *
                                    EQGFX_PARTICLE_SPAWN_RANDOM_SCALE_GUESS + minimumBounds[0] + instance->spawnXYZ.x;
                                randomInteger = EQ_CDECL(int, 0x10069D8E, (void))();
                                position.y = (maximumBounds[1] - minimumBounds[1]) * (float)randomInteger *
                                    EQGFX_PARTICLE_SPAWN_RANDOM_SCALE_GUESS + minimumBounds[1] + instance->spawnXYZ.y;
                                randomInteger = EQ_CDECL(int, 0x10069D8E, (void))();
                                position.z = (maximumBounds[2] - minimumBounds[2]) * (float)randomInteger *
                                    EQGFX_PARTICLE_SPAWN_RANDOM_SCALE_GUESS + minimumBounds[2] + instance->spawnXYZ.z;
                                randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))();
                                rampTime = scaledElapsedTime * EQGFX_PARTICLE_RAMP_GUESS[randomValue & 0x3FF];
                                EQ_CDECL(void, 0x100427BB,
                                    (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
                                        instance, &position, &instance->spawnNormal, rampTime);
                                spawnCount = spawnCount - 1;
                            } while (spawnCount != 0);
                        }
                        break;
                    case 1:
                        for (; spawnCount != 0; spawnCount = spawnCount - 1) {
                            radius = instance->spawnRadius;
                            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
                            angle = (float)(int)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
                            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
                            secondAngle = (float)(int)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
                            sine = EQ_CDECL(float, 0x1004EE87, (float))(secondAngle);
                            cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle);
                            velocity.x = cosine * sine;
                            sine = EQ_CDECL(float, 0x1004EE87, (float))(secondAngle);
                            angle = EQ_CDECL(float, 0x1004EE87, (float))(angle);
                            velocity.y = angle * sine;
                            velocity.z = EQ_CDECL(float, 0x1004EEFA, (float))(secondAngle);
                            position.x = velocity.x * radius + instance->spawnXYZ.x;
                            position.y = velocity.y * radius + instance->spawnXYZ.y;
                            position.z = velocity.z * radius + instance->spawnXYZ.z;
                            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))();
                            rampTime = scaledElapsedTime * EQGFX_PARTICLE_RAMP_GUESS[randomValue & 0x3FF];
                            EQ_CDECL(void, 0x100427BB,
                                (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
                                    instance, &position, &velocity, rampTime);
                        }
                        break;
                    case 2:
                        for (; spawnCount != 0; spawnCount = spawnCount - 1) {
                            radius = instance->spawnRadius;
                            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
                            angle = (float)(int)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
                            cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle);
                            sine = EQ_CDECL(float, 0x1004EE87, (float))(angle);
                            alongAxis = cosine * radius;
                            acrossAxis = sine * radius;
                            velocity.x = spawnAxes[0] * cosine + spawnAxes[3] * sine;
                            velocity.y = spawnAxes[1] * cosine + spawnAxes[4] * sine;
                            velocity.z = spawnAxes[2] * cosine + spawnAxes[5] * sine;
                            position.x = spawnAxes[0] * alongAxis + spawnAxes[3] * acrossAxis + instance->spawnXYZ.x;
                            position.y = spawnAxes[1] * alongAxis + spawnAxes[4] * acrossAxis + instance->spawnXYZ.y;
                            position.z = spawnAxes[2] * alongAxis + spawnAxes[5] * acrossAxis + instance->spawnXYZ.z;
                            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))();
                            rampTime = scaledElapsedTime * EQGFX_PARTICLE_RAMP_GUESS[randomValue & 0x3FF];
                            EQ_CDECL(void, 0x100427BB,
                                (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
                                    instance, &position, &velocity, rampTime);
                        }
                        break;
                    case 3:
                        while ((countForCurrentSpawn = spawnCount) != 0) {
                            position.x = instance->spawnXYZ.x;
                            position.y = instance->spawnXYZ.y;
                            position.z = instance->spawnXYZ.z;
                            randomInteger = EQ_CDECL(int, 0x10069D8E, (void))();
                            vertical = (float)randomInteger * instance->spawnAngle *
                                EQGFX_PARTICLE_SPAWN_RANDOM_SCALE_GUESS;
                            vertical = EQ_CDECL(float, 0x1004EE87, (float))(vertical);
                            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
                            angle = (float)(int)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
                            cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle);
                            sine = EQ_CDECL(float, 0x1004EE87, (float))(angle);
                            alongAxis = cosine * vertical;
                            acrossAxis = sine * vertical;
                            velocity.x = spawnAxes[0] * alongAxis + spawnAxes[3] * acrossAxis + spawnAxes[6];
                            velocity.y = spawnAxes[1] * alongAxis + spawnAxes[4] * acrossAxis + spawnAxes[7];
                            velocity.z = spawnAxes[2] * alongAxis + spawnAxes[5] * acrossAxis + spawnAxes[8];
                            randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))();
                            rampTime = scaledElapsedTime * EQGFX_PARTICLE_RAMP_GUESS[randomValue & 0x3FF];
                            EQ_CDECL(void, 0x100427BB,
                                (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
                                    instance, &position, &velocity, rampTime);
                            spawnCount = countForCurrentSpawn - 1;
                        }
                        break;
                    case 4:
                        if (spawnCount != 0) {
                            do {
                                randomInteger = EQ_CDECL(int, 0x10069D8E, (void))();
                                radius = (float)randomInteger * instance->spawnRadius *
                                    EQGFX_PARTICLE_SPAWN_RANDOM_SCALE_GUESS;
                                randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
                                angle = (float)(int)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
                                cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle);
                                alongAxis = cosine * radius;
                                sine = EQ_CDECL(float, 0x1004EE87, (float))(angle);
                                acrossAxis = sine * radius;
                                position.x = spawnAxes[0] * alongAxis + spawnAxes[3] * acrossAxis + instance->spawnXYZ.x;
                                position.y = spawnAxes[1] * alongAxis + spawnAxes[4] * acrossAxis + instance->spawnXYZ.y;
                                position.z = spawnAxes[2] * alongAxis + spawnAxes[5] * acrossAxis + instance->spawnXYZ.z;
                                randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))();
                                rampTime = scaledElapsedTime * EQGFX_PARTICLE_RAMP_GUESS[randomValue & 0x3FF];
                                EQ_CDECL(void, 0x100427BB,
                                    (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
                                        instance, &position, &instance->spawnNormal, rampTime);
                                spawnCount = spawnCount - 1;
                            } while (spawnCount != 0);
                        }
                        break;
                    case 5:
                        if (spawnCount != 0) {
                            do {
                                radius = instance->spawnRadius;
                                randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))() & 0x1FF;
                                angle = (float)(int)randomValue * EQGFX_PARTICLE_SPAWN_ANGLE_STEP_GUESS;
                                cosine = EQ_CDECL(float, 0x1004EEFA, (float))(angle);
                                alongAxis = cosine * radius;
                                sine = EQ_CDECL(float, 0x1004EE87, (float))(angle);
                                acrossAxis = sine * radius;
                                position.x = spawnAxes[0] * alongAxis + spawnAxes[3] * acrossAxis + instance->spawnXYZ.x;
                                position.y = spawnAxes[1] * alongAxis + spawnAxes[4] * acrossAxis + instance->spawnXYZ.y;
                                position.z = spawnAxes[2] * alongAxis + spawnAxes[5] * acrossAxis + instance->spawnXYZ.z;
                                randomValue = (unsigned int)EQ_CDECL(int, 0x10069D8E, (void))();
                                rampTime = scaledElapsedTime * EQGFX_PARTICLE_RAMP_GUESS[randomValue & 0x3FF];
                                EQ_CDECL(void, 0x100427BB,
                                    (PCloudInstBase_guess *, Float3_guess *, Float3_guess *, float))(
                                        instance, &position, &instance->spawnNormal, rampTime);
                                spawnCount = spawnCount - 1;
                            } while (spawnCount != 0);
                        }
                        break;
                    }
                }
                return 0;
            }
            particle = (PCloudParticle_guess *)&instance->nodes[currentIndex];
            nextIndex = particle->nextIndex;
            if ((unsigned int)particle->nextFrameTick <= (unsigned int)instance->baseTicks) {
                break;
            }
            EQ_CDECL(int, 0x10041C42, (PCloudMotion_guess *, PCloudParticle_guess *, float, int))(
                (PCloudMotion_guess *)instance, particle, scaledElapsedTime, elapsedTicks);
            currentIndex = nextIndex;
        }
        if (particle->specialState == 0) {
            EQ_CDECL(void, 0x1004290A, (PCloudInstBase_guess *, int))(instance, currentIndex);
            currentIndex = nextIndex;
            if ((instance->flags & 4) != 0) {
                spawnCount = spawnCount + 1;
                savedSpawnCount = spawnCount;
            }
            continue;
        }
        if ((instance->spriteBuffer1C == 0) || (particle->specialState == (int)0x80000000u)) {
            attachedSprite = (void *)instance->spriteBuffer18;
            if (attachedSprite == 0) {
                EQ_CDECL(void, 0x1004290A, (PCloudInstBase_guess *, int))(instance, currentIndex);
                currentIndex = nextIndex;
                if ((instance->flags & 4) != 0) {
                    spawnCount = spawnCount + 1;
                    savedSpawnCount = spawnCount;
                }
                continue;
            }
            particle->pos = particle->cachedPosition;
            if (alternateAxesReady == 0.0f) {
                EQGFX_PARTICLE_CLOUD_AXIS_CALLBACK_GUESS(attachedSprite, alternateAxes);
                alternateAxesReady = 1.4013e-45f;
            }
            axes = alternateAxes;
        } else {
            attachedSprite = (void *)instance->spriteBuffer1C;
            particle->pos = particle->cachedPosition;
            if (primaryAxesReady == 0.0f) {
                EQGFX_PARTICLE_CLOUD_AXIS_CALLBACK_GUESS(attachedSprite, primaryAxes);
                primaryAxesReady = 1.4013e-45f;
            }
            axes = primaryAxes;
        }
        EQ_CDECL(void, 0x100425BA, (PCloudInstBase_guess *, PCloudSpawnContext_guess *, float *))(
            (PCloudInstBase_guess *)attachedSprite, (PCloudSpawnContext_guess *)particle, axes);
        EQ_CDECL(void, 0x1004290A, (PCloudInstBase_guess *, int))(instance, currentIndex);
        spawnCount = savedSpawnCount;
        currentIndex = nextIndex;
    }
}

// FUNCTION: EQGFX 0x10041C42
// __cdecl (four stack arguments; the callee pops 0 bytes). Advances one particle by timeStep and returns 0.
int __cdecl t3dAdvancePCloudParticle_guess(PCloudMotion_guess *motion, PCloudParticle_guess *particle, float timeStep, int elapsedTicks)
{
    float newX;
    float newY;
    float newZ;
    float toTargetX;
    float toTargetY;
    float toTargetZ;
    float wrapSum;

    newX = timeStep * particle->vel.x + particle->pos.x;
    particle->pos.x = newX;
    newY = timeStep * particle->vel.y + particle->pos.y;
    particle->pos.y = newY;
    newZ = timeStep * particle->vel.z + particle->pos.z;
    particle->pos.z = newZ;
    if (motion->flags & 0x1000) {
        if (motion->flags & 0x800) {
            toTargetX = motion->accel.x - newX;
            toTargetY = motion->accel.y - newY;
            toTargetZ = motion->accel.z - newZ;
            particle->pos.x = newX - toTargetX * motion->forceScale * timeStep * timeStep * EQGFX_MOTION_HALF_GUESS;
            particle->pos.y = newY - toTargetY * motion->forceScale * timeStep * timeStep * EQGFX_MOTION_HALF_GUESS;
            particle->pos.z = newZ - toTargetZ * motion->forceScale * timeStep * timeStep * EQGFX_MOTION_HALF_GUESS;
            particle->vel.x = toTargetX * motion->forceScale * timeStep + particle->vel.x;
            particle->vel.y = toTargetY * motion->forceScale * timeStep + particle->vel.y;
            particle->vel.z = toTargetZ * motion->forceScale * timeStep + particle->vel.z;
        } else {
            particle->pos.x = newX - motion->accel.x * motion->forceScale * timeStep * timeStep * EQGFX_MOTION_HALF_GUESS;
            particle->pos.y = newY - motion->accel.y * motion->forceScale * timeStep * timeStep * EQGFX_MOTION_HALF_GUESS;
            particle->pos.z = newZ - motion->accel.z * motion->forceScale * timeStep * timeStep * EQGFX_MOTION_HALF_GUESS;
            particle->vel.x = motion->accel.x * motion->forceScale * timeStep + particle->vel.x;
            particle->vel.y = motion->accel.y * motion->forceScale * timeStep + particle->vel.y;
            particle->vel.z = motion->accel.z * motion->forceScale * timeStep + particle->vel.z;
        }
    }
    if (motion->flags & 0x40) {
        wrapSum = motion->wrapOffset.x + particle->pos.x;
        if (wrapSum < EQGFX_WRAP_LOW_GUESS) {
            particle->pos.x = particle->pos.x + EQGFX_WRAP_SPAN_GUESS;
        } else if (wrapSum > EQGFX_WRAP_HIGH_GUESS) {
            particle->pos.x = particle->pos.x - EQGFX_WRAP_SPAN_GUESS;
        }
        wrapSum = motion->wrapOffset.y + particle->pos.y;
        if (wrapSum < EQGFX_WRAP_LOW_GUESS) {
            particle->pos.y = particle->pos.y + EQGFX_WRAP_SPAN_GUESS;
        } else if (wrapSum > EQGFX_WRAP_HIGH_GUESS) {
            particle->pos.y = particle->pos.y - EQGFX_WRAP_SPAN_GUESS;
        }
        wrapSum = motion->wrapOffset.z + particle->pos.z;
        if (wrapSum < EQGFX_WRAP_LOW_GUESS) {
            particle->pos.z = particle->pos.z + EQGFX_WRAP_SPAN_GUESS;
        } else if (wrapSum > EQGFX_WRAP_HIGH_GUESS) {
            particle->pos.z = particle->pos.z - EQGFX_WRAP_SPAN_GUESS;
        }
    }
    if (motion->flags & 0x400) {
        if (motion->boundsSamples < 2) {
            motion->boundsMin = particle->pos;
            motion->boundsMax = particle->pos;
        } else {
            if (particle->pos.x < motion->boundsMin.x) {
                motion->boundsMin.x = particle->pos.x;
            }
            if (particle->pos.y < motion->boundsMin.y) {
                motion->boundsMin.y = particle->pos.y;
            }
            if (particle->pos.z < motion->boundsMin.z) {
                motion->boundsMin.z = particle->pos.z;
            }
            if (particle->pos.x > motion->boundsMax.x) {
                motion->boundsMax.x = particle->pos.x;
            }
            if (particle->pos.y > motion->boundsMax.y) {
                motion->boundsMax.y = particle->pos.y;
            }
            if (particle->pos.z > motion->boundsMax.z) {
                motion->boundsMax.z = particle->pos.z;
            }
        }
    }
    if (motion->frameCount != 1 && particle->frameTime < motion->timeBase + elapsedTicks) {
        do {
            particle->frame++;
            if (particle->frame >= motion->frameCount) {
                particle->frame = 0;
            }
            particle->frameTime += motion->frameStep;
        } while (particle->frameTime < motion->timeBase + elapsedTicks);
    }
    return 0;
}

// FUNCTION: EQGFX 0x10042969
// Selects a render object's detail method based on its distance from a position.
// __cdecl (the callee pops 0 bytes; the first stack argument is unused). Returns the render method for the object's distance from pos.
unsigned int __cdecl t3dSelectDistanceBand_guess(int unusedArg, RenderObject_guess *renderObject, float *pos)
{
    RenderLodTable_guess *table;
    RenderLodBand_guess *band;
    float dx;
    float dy;
    float dz;
    float distSq;

    table = renderObject->lodTable;
    if (table->mode == 1) {
        return table->bands->methodIndex;
    }
    dx = renderObject->pos.x - pos[0];
    dy = renderObject->pos.y - pos[1];
    dz = renderObject->pos.z - pos[2];
    distSq = dz * dz + dy * dy + dx * dx;
    if (distSq > EQGFX_FAR_LIMIT) {
        distSq = EQGFX_FAR_LIMIT;
    }
    for (band = table->bands; band->endDistSq < distSq; band = band + 1) {
    }
    if (band->endDistSq < EQGFX_FAR_LIMIT && (band + 1)->methodIndex == (renderObject->methodIndex & 0x7fffffff) &&
        (band + 1)->startDistSq <= distSq) {
        return (band + 1)->methodIndex;
    }
    return band->methodIndex;
}

// FUNCTION: EQGFX 0x100429FF
// Returns a user-defined render method for the requested index.
// __cdecl (the callee pops 0 bytes). Returns the table entry for the index with the top bit cleared, or 7 past the table.
unsigned int __cdecl t3dGetRenderContextUserDefinedRenderMethod(RenderContext_guess *renderContext, unsigned int methodIndex)
{
    unsigned int index;

    index = methodIndex & 0x7fffffff;
    if (index >= renderContext->userMethodCount) {
        return 7;
    }
    return renderContext->userMethods[index];
}
