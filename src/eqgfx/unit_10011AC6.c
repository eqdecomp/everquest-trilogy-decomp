// unit_10011AC6.c - eqgfx_dx7.dll, the address run 0x10011AC6..0x10013993 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its names are unknown).

#include "trilogy/eqaddr.h"

#define EQGFX_OBJECT_TYPE_DM_SPRITE_ALIAS 0x4C

#define EQGFX_T3D_DESTROY_VOLUME(dictionary, volume) \
    EQ_CDECL(void, 0x1001D5A1, (void *, void *))(dictionary, volume)
#define EQGFX_T3D_REMOVE_ENTRY(dictionary, entry) \
    EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, entry)
#define EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, object) \
    EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, object)
#define EQGFX_T3D_UNSHARE_OBJECT(object) \
    EQ_CDECL(void, 0x10017DAD, (void *))(object)
#define EQGFX_S3D_DESTROY_DM_SPRITE(dictionary, sprite) \
    EQ_CDECL(void, 0x10010575, (void *, void *))(dictionary, sprite)
#define EQGFX_T3D_FREE_MEMORY(memory) \
    EQ_CDECL(void, 0x10050CBB, (void *))(memory)
#define EQGFX_S3D_DESTROY_STRING_SPRITE(dictionary, sprite) \
    EQ_CDECL(void, 0x1001ACF8, (void *, void *))(dictionary, sprite)
#define EQGFX_T3D_DESTROY_HIERARCHICAL_SPRITE(dictionary, sprite) \
    EQ_CDECL(void, 0x100132A2, (void *, void *))(dictionary, sprite)
#define EQGFX_T3D_DESTROY_3D_SPRITE(dictionary, sprite) \
    EQ_CDECL(void, 0x1000879D, (void *, void *))(dictionary, sprite)
#define EQGFX_T3D_REALLOCATE_MEMORY(memory, size) \
    EQ_CDECL(void *, 0x10050C8C, (void *, unsigned int))(memory, size)
#define EQGFX_T3D_SHARE_OBJECT(object) \
    EQ_CDECL(void, 0x10017CE8, (void *))(object)

typedef void (__stdcall *EQGFX_OutputDebugStringA_t)(const char *);
#define EQGFX_T3D_UPDATE_ANIMATION_CYCLE(cycle, state, frame) \
    EQ_CDECL(void, 0x1001D251, (void *, void *, unsigned int))(cycle, state, frame)
#define EQGFX_T3D_UNPACK_ANIMATION_FRAME(source, destination) \
    EQ_CDECL(void, 0x100123BE, (const void *, void *))(source, destination)
#define EQGFX_T3D_INTERPOLATE_VECTOR(start, end, blend, result) \
    EQ_CDECL(void, 0x10012336, (const float *, const float *, float, float *))(start, end, blend, result)
#define EQGFX_T3D_INTERPOLATE_SCALE(start, end, blend, result) \
    EQ_CDECL(void, 0x10012398, (float, float, float, float *))(start, end, blend, result)
#define EQGFX_T3D_SLERP_QUATERNIONS(start, end, blend, result) \
    EQ_CDECL(void, 0x100500A4, (const float *, const float *, float, float *))(start, end, blend, result)
#define EQGFX_T3D_ROTATE_POINT(point, quaternion, result) \
    EQ_CDECL(void, 0x1004FF92, (const float *, const float *, float *))(point, quaternion, result)
#define EQGFX_T3D_MULTIPLY_QUATERNIONS(first, second, result) \
    EQ_CDECL(void, 0x1004FD59, (const float *, const float *, float *))(first, second, result)
#define EQGFX_T3D_QUATERNION_TO_MATRIX(quaternion, matrix) \
    EQ_CDECL(void, 0x1004F9A2, (const float *, float *))(quaternion, matrix)
#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define EQGFX_OUTPUT_DEBUG_STRING EQ_GLOBAL(EQGFX_OutputDebugStringA_t, 0x100760C4)
#define EQGFX_HIERARCHICAL_SPRITE_SCALE_MESSAGE ((const char *)0x10086664)
#define EQGFX_T3D_ALLOCATE_MEMORY(size) \
    EQ_CDECL(void *, 0x10050C48, (unsigned int))(size)
#define EQGFX_T3D_DUPLICATE_VOLUME(dictionary, volume) \
    EQ_CDECL(void *, 0x1001D516, (void *, void *))(dictionary, volume)
#define EQGFX_T3D_SHARE_OBJECT(object) \
    EQ_CDECL(void, 0x10017CE8, (void *))(object)
#define EQGFX_T3D_UNSHARE_OBJECT(object) \
    EQ_CDECL(void, 0x10017DAD, (void *))(object)
#define EQGFX_T3D_DESTROY_PARTICLE_CLOUD(dictionary, slot) \
    EQ_CDECL(void, 0x10040217, (void *, void *))(dictionary, slot)
#define EQGFX_T3D_CREATE_PARTICLE_CLOUD(dictionary, source, offset, arg4, arg5) \
    EQ_CDECL(void *, 0x1003FFE2, (void *, void *, EQGfxVector3_guess *, int, int))(dictionary, source, offset, arg4, arg5)
#define EQGFX_T3D_BIND_PARTICLE_CLOUD_SPAWN(cloud, dag) \
    EQ_CDECL(void, 0x1004013B, (void *, void *))(cloud, dag)
#define EQGFX_T3D_ADD_HIERARCHICAL_SPRITE(dictionary, sprite) \
    EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, sprite)
#define EQGFX_T3D_SETUP_SPRITE_HIERARCHY(sprite, dag, parent) \
    EQ_CDECL(void, 0x10012E50, (void *, void *, void *))(sprite, dag, parent)
#define EQGFX_T3D_COPY_MEMORY(destination, source, size) \
    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(destination, source, size)
#define EQGFX_STRLEN(string) \
    EQ_CDECL(unsigned int, 0x10068160, (const char *))(string)
#define EQGFX_COPY_STRING(destination, source) \
    EQ_CDECL(void, 0x10068070, (void *, const char *))(destination, source)
#define EQGFX_T3D_GET_POINTER(dictionary, name) \
    EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))(dictionary, name)
#define EQGFX_T3D_ADD_ENTRY(dictionary, entry) \
    EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, entry)
#define EQGFX_T3D_CREATE_SPRITE_ALLOC_FAILED ((const char *)0x10086ACC)
#define EQGFX_T3D_CREATE_SPRITE_VOLUME_FAILED ((const char *)0x10086A84)
#define EQGFX_T3D_CREATE_SPRITE_CHILD_LIST_FAILED ((const char *)0x10086A38)
#define EQGFX_T3D_CREATE_SPRITE_LINK_LIST_FAILED ((const char *)0x100869E8)
#define EQGFX_T3D_CREATE_SPRITE_DAG_LIST_FAILED ((const char *)0x100869AC)
#define EQGFX_T3D_CREATE_SPRITE_ADD_ITEM_FAILED ((const char *)0x1008697C)
#define EQGFX_T3D_CREATE_SPRITE_SUBDAG_LIST_FAILED ((const char *)0x10086940)
#define EQGFX_T3D_DUPLICATE_SPRITE_ALLOC_FAILED ((const char *)0x10086B88)
#define EQGFX_T3D_DUPLICATE_SPRITE_VOLUME_FAILED ((const char *)0x10086B50)
#define EQGFX_T3D_DUPLICATE_SPRITE_DEFAULT_ADD_FAILED ((const char *)0x10086B08)
#define EQGFX_HIERARCHICAL_SPRITE_TAG_CONFLICT_MESSAGE ((const char *)0x100868FC)
#define EQGFX_HIERARCHICAL_SPRITE_ALLOC_FAILED_MESSAGE ((const char *)0x100868C8)
#define EQGFX_HIERARCHICAL_SPRITE_TAG_ALLOC_FAILED_MESSAGE ((const char *)0x1008688C)
#define EQGFX_HIERARCHICAL_SPRITE_ADD_ITEM_FAILED_MESSAGE ((const char *)0x1008685C)
#define EQGFX_HIERARCHICAL_SPRITE_ADD_ENTRY_FAILED_MESSAGE ((const char *)0x10086824)
#define EQGFX_HIERARCHICAL_SPRITE_INSTANCE_ALLOC_FAILED_MESSAGE ((const char *)0x100867E0)
#define EQGFX_HIERARCHICAL_SPRITE_SKIN_ALLOC_FAILED_MESSAGE ((const char *)0x10086794)
#define EQGFX_HIERARCHICAL_SPRITE_INSTANCE_ADD_ITEM_FAILED_MESSAGE ((const char *)0x1008674C)
#define EQGFX_HIERARCHICAL_SPRITE_INVALID_SKIN_TYPE_MESSAGE ((const char *)0x10086714)
#define EQGFX_HIERARCHICAL_SPRITE_SUBDAG_ALLOC_FAILED_MESSAGE ((const char *)0x100866C4)
#define EQGFX_HIERARCHICAL_SPRITE_INVALID_SPRITE_TYPE_MESSAGE ((const char *)0x10086694)

#define EQGFX_T3D_PACKED_FRAME_SIZE 0x10
#define EQGFX_T3D_FULL_FRAME_SIZE 0x24

typedef struct T3DHierarchicalSpriteInstance_guess
{
    int type;
    unsigned char pad_04[4];
    void *dictionaryEntry;
    unsigned char flags;
    unsigned char pad_0d[3];
    int state;
    unsigned int elementCount;
    unsigned int definitionFlags;
    struct T3DHierarchicalSprite_guess *definition;
} T3DHierarchicalSpriteInstance_guess;

typedef struct T3DInitializationRecord_guess
{
    unsigned int type;
    char *name;
    unsigned int value08;
    unsigned int value0C;
    unsigned int value10;
    void *sourceRecord;
    unsigned int value18;
    float scaleX;
    float offsetX;
    float offsetY;
    float offsetZ;
    float angleX;
    float angleY;
    float angleZ;
    float scaleY;
    unsigned int reserved3C;
    unsigned int resetValue40;
    unsigned char pad44[0xa8];
    int selector;
    void *optionalData;
    unsigned char padF4[0x0c];
    unsigned int copiedValues[8];
} T3DInitializationRecord_guess;

// The three-float vector the original moves as one 12-byte block (its MOVSD trio). Local stand-in:
// trilogy/utf8.h's EQVector3_guess is C++ (declares a member) and this file is C.
typedef struct EQGfxVector3_guess
{
    float x;    // 0x0
    float y;    // 0x4
    float z;    // 0x8
} EQGfxVector3_guess;

// The four-float quaternion the original moves as one 16-byte block (its MOVSD quartet). No include/ header matches it yet.
typedef struct EQGfxQuaternion_guess
{
    float w;    // 0x0
    float x;    // 0x4
    float y;    // 0x8
    float z;    // 0xc
} EQGfxQuaternion_guess;

// The collision volume a sprite points at: its type is the first dword. No include/ header matches it yet.
typedef struct T3DHierarchicalSpriteDefinition_guess
{
    unsigned char pad_00[0x10];
    int referenceCount;
    int elementCount;
} T3DHierarchicalSpriteDefinition_guess;

typedef struct T3DHierarchicalSpriteElement_guess
{
    unsigned char pad_00[0x4];
    void *elementBuffer;
    int *object;
    unsigned char pad_0c[0xe0];
    unsigned int bufferCount;
    void *buffer;
    unsigned char pad_f4[0x54];
} T3DHierarchicalSpriteElement_guess;

typedef struct T3DHierarchicalSprite_guess
{
    int type;
    int dictionaryIndex;
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad_0d[0x3];
    int state;
    void *volume;
    unsigned char pad_18[0x8];
    unsigned int dagCount;
    T3DHierarchicalSpriteElement_guess *elements;
    EQGfxVector3_guess offset;
    int processFlags;                   // 0x34: passed to the dag processor
    unsigned int reserved_38;
    unsigned int reserved_3c;
    unsigned int childSpriteCount;
    unsigned int childSpriteCapacity;
    void **childSprites;
    void **childSpriteBuffer;
    int particleCloudCount;
    unsigned int reserved_54;
    int **particleCloudSlots;
    T3DHierarchicalSpriteDefinition_guess *definition;
} T3DHierarchicalSprite_guess;

#define EQGFX_OBJECT_TYPE_3D_SPRITE 0x0C
#define EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE_ENTRY 0x13
#define EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE 0x14
#define EQGFX_OBJECT_TYPE_DM_SPRITE 0x48
#define EQGFX_OBJECT_TYPE_PARTICLE_CLOUD 0x4D
#define EQGFX_OBJECT_TYPE_STRING_SPRITE 0x51
#define EQGFX_HIERARCHICAL_SPRITE_IN_USE 0xFFFF3D45
#define EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE 0x148
#define EQGFX_PARTICLE_CLOUD_SOURCE_OFFSET 0x11C
#define EQGFX_EMPTY_DAG_OBJECT ((int *)0x100F401C)

typedef void (__stdcall *DebugBreak_t)(void);
#define EQGFX_DEBUG_BREAK EQ_GLOBAL(DebugBreak_t, 0x100760BC)

typedef struct T3DCollisionVolume_guess
{
    int           type;             // 0x0
} T3DCollisionVolume_guess;         // 0x4

// The sprite object the getters read: its type tag is the first dword, its collision volume pointer sits at +0x14, its pending track at +0x18,
// its palette at +0x3C, its skin count at +0x40, its skin list at +0x48, its skin DAG index list at +0x4C and its definition pointer at +0x5C. No include/ header matches it yet.
typedef struct T3DSprite_guess
{
    int                       type;             // 0x00
    unsigned char             pad_04[0x4];      // 0x04
    struct T3DSpriteDefinition_guess *dag;      // 0x08: the sprite's definition (t3dSetDagSprite)
    void                     *parent;           // 0x0c: stored into dag+0x18 when the dag's tag is 0x14
    unsigned char             pad_10[0x4];      // 0x10
    T3DCollisionVolume_guess *collisionVolume;  // 0x14
    void                     *pendingTrack;     // 0x18
    unsigned char             pad_1c[0x4];      // 0x1c
    int                       dagCount;         // 0x20
    void                     *dagArray;         // 0x24
    EQGfxVector3_guess        centerOffset;     // 0x28
    unsigned char             pad_34[0x8];      // 0x34
    void                     *palette;          // 0x3c
    unsigned int              skinCount;        // 0x40
    unsigned int              skinCapacity;     // 0x44
    void                    **skinList;         // 0x48
    int                      *skinDagIndex;     // 0x4c
    unsigned char             pad_50[0xc];      // 0x50
    void                     *definition;       // 0x5c
    unsigned char             pad_60[0xc0];     // 0x60
    int                       fadeTicks;        // 0x120
    int                       fadeRate;         // 0x124
} T3DSprite_guess;                              // 0x128

// The fade object the fade test reads: its bound is the unsigned dword at +0x8. No include/ header matches it yet.
typedef struct T3DDagFade_guess
{
    unsigned char pad_00[0x8];      // 0x00
    unsigned int  limit;            // 0x08
} T3DDagFade_guess;

// The polygon the plane comes from: its plane sits at +0x10 (the normal, then the distance at +0x1c). No include/ header matches it yet.
typedef struct T3DPolygon_guess
{
    unsigned char      pad_00[0x10];    // 0x00
    EQGfxVector3_guess normal;          // 0x10
    float              distance;        // 0x1c
} T3DPolygon_guess;

// The plane the getter fills: a tag dword (set to 0x2f), the normal, then the distance. No include/ header matches it yet.
typedef struct T3DPlane_guess
{
    int                type;            // 0x00
    EQGfxVector3_guess normal;          // 0x04
    float              distance;        // 0x10
} T3DPlane_guess;                       // 0x14

// The 8-dword block a dag track carries at +0x18: copied whole into the sprite by one REP MOVSD. No include/ header matches it yet.
typedef struct T3DTrackBlock_guess
{
    unsigned int dword[8];          // 0x00
} T3DTrackBlock_guess;              // 0x20

// The dag track the setter reads: its block sits at +0x18. No include/ header matches it yet.
typedef struct T3DDagTrack_guess
{
    unsigned char       pad_00[0x18];   // 0x00
    T3DTrackBlock_guess block;          // 0x18
} T3DDagTrack_guess;                    // 0x38

typedef struct T3DAnimationState_guess
{
    unsigned short firstFrame;
    unsigned short secondFrame;
    float blend;
    unsigned int reserved_08[3];
    float phase;
    unsigned int reserved_18[2];
} T3DAnimationState_guess;

typedef struct T3DAnimationFrameSet_guess
{
    int frameCount;
    void *frameData;
    unsigned char pad_08[0x0c];
    int format;
} T3DAnimationFrameSet_guess;

typedef struct T3DAnimationSequence_guess
{
    unsigned char pad_00[0x14];
    T3DAnimationFrameSet_guess *fullFrames;
    T3DAnimationFrameSet_guess *packedFrames;
} T3DAnimationSequence_guess;

typedef struct T3DAnimationClip_guess
{
    unsigned char pad_00[0x14];
    T3DAnimationSequence_guess *sequence;
} T3DAnimationClip_guess;

typedef struct T3DAnimationTransform_guess
{
    float quaternion[4];
    EQGfxVector3_guess position;
    float scale;
    float reserved;
} T3DAnimationTransform_guess;

// The DAG entry transformed by the renderer (0x148 bytes). No include/ header matches it yet.
typedef struct T3DDagObject_guess
{
    unsigned char      pad_00[0x4];     // 0x00
    char              *name;            // 0x4
    T3DHierarchicalSprite_guess *hierarchicalSprite; // 0x8
    unsigned char      pad_0c[0x8];      // 0x0c
    T3DAnimationClip_guess *currentAnimation; // 0x14
    T3DAnimationClip_guess *activeCycle; // 0x18
    float              frameQuat[4];    // 0x1c: w, x, y, z
    EQGfxVector3_guess framePosition;   // 0x2c
    float              frameScale;      // 0x38
    float              frameMetadata;  // 0x3c
    int                animationMode;  // 0x40
    float              resultQuat[4];   // 0x44
    float              inverseScale;   // 0x54
    float              inverseScaleSquared; // 0x58
    float              rotation[9];     // 0x5c
    EQGfxVector3_guess translation;     // 0x80
    unsigned char      pad_8c[0x60];    // 0x8c
    unsigned int       childCount;      // 0xec
    struct T3DDagObject_guess **children; // 0xf0
    unsigned char      pad_f4[0x0c];    // 0xf4
    T3DAnimationState_guess currentState; // 0x100
    int                transitionStartFrame; // 0x120
    int                transitionDuration; // 0x124
    T3DAnimationState_guess transitionState; // 0x128
} T3DDagObject_guess;                   // 0x148

// The polygon's position list read at polygon+0x8: 0x14-byte entries whose position pointer sits at
// +0x4; entries 0, 1 and 2 hold the edge's two endpoints and the corner the normal is taken from.
typedef struct EQGfxVertexList_guess
{
    int unknown_0;                       // 0x0
    EQGfxVector3_guess *startPosition;   // 0x4  (entry 0)
    int unknown_8;                       // 0x8
    int unknown_c;                       // 0xc
    int unknown_10;                      // 0x10
    int unknown_14;                      // 0x14
    EQGfxVector3_guess *cornerPosition;  // 0x18 (entry 1)
    int unknown_1c;                      // 0x1c
    int unknown_20;                      // 0x20
    int unknown_24;                      // 0x24
    int unknown_28;                      // 0x28
    EQGfxVector3_guess *endPosition;     // 0x2c (entry 2)
} EQGfxVertexList_guess;

// The edge polygon object: the normal at +0xc..+0x14 and the plane distance at +0x18 are its outputs.
typedef struct EQGfxEdgePolygon_guess
{
    int unknown_0;                      // 0x0
    unsigned int vertexCount;           // 0x4
    EQGfxVertexList_guess *vertexList;  // 0x8
    EQGfxVector3_guess normal;          // 0xc
    float distance;                     // 0x18
} EQGfxEdgePolygon_guess;

// An indexed polygon's vertex list read at polygon+0x8: three ints that index the polygon's shared
// position array (start vertex, corner vertex, end vertex, in that slot order).
typedef struct EQGfxVertexIndices_guess
{
    int startIndex;   // 0x0
    int cornerIndex;  // 0x4
    int endIndex;     // 0x8
} EQGfxVertexIndices_guess;

// The indexed polygon object: its position array pointer at +0xc, the scaled normal at +0x10..+0x18
// and the plane distance at +0x1c are its outputs.
typedef struct EQGfxPolygon_guess
{
    int unknown_0;                         // 0x0
    unsigned int vertexCount;              // 0x4
    EQGfxVertexIndices_guess *vertexList;  // 0x8
    float *positions;                      // 0xc
    EQGfxVector3_guess normal;             // 0x10
    float distance;                        // 0x1c
} EQGfxPolygon_guess;

// FUNCTION: EQGFX 0x10011aea
// Builds the plane normal for an edge polygon from its three stored points.
// __cdecl (the callee pops 0; ECX is not an input), int (EAX is read after 1 of the 2 call sites).
// Edge normal: (start - corner) x (end - corner) scaled by the inverse square root, distance = -(normal . corner).
int t3dCalculateNormalToEdgePolygon(EQGfxEdgePolygon_guess *polygon)
{
    int result = 1;

    if (polygon->vertexCount < 2)
        result = -1;
    else
    {
        EQGfxVertexList_guess *list = polygon->vertexList;
        EQGfxVector3_guess *corner = list->cornerPosition;
        float cornerX = corner->x;
        float cornerY = corner->y;
        float cornerZ = corner->z;
        EQGfxVector3_guess *start = list->startPosition;
        EQGfxVector3_guess *end = list->endPosition;
        float firstX = start->x - cornerX;
        float firstY = start->y - cornerY;
        float firstZ = start->z - cornerZ;
        float secondX = end->x - cornerX;
        float secondY = end->y - cornerY;
        float secondZ = end->z - cornerZ;
        float normalX = secondZ * firstY - secondY * firstZ;
        float normalY = secondX * firstZ - secondZ * firstX;
        float normalZ = secondY * firstX - secondX * firstY;
        float scale = EQ_CDECL(float, 0x1004EFDA, (float))(normalZ * normalZ + normalY * normalY + normalX * normalX);

        polygon->normal.x = scale * normalX;
        polygon->normal.y = scale * normalY;
        polygon->normal.z = scale * normalZ;
        polygon->distance =
            -(scale * normalZ * cornerZ + scale * normalY * cornerY + scale * normalX * cornerX);
    }
    return result;
}

// FUNCTION: EQGFX 0x10011bde
// Builds a mesh polygon's plane normal from its indexed vertices.
// __cdecl (the callee pops 0; ECX is not an input), int (EAX is read after the call).
// Indexed-polygon normal: (start - corner) x (end - corner) scaled by the inverse square root;
// the plane distance is -(scaled normal . corner); fewer than 3 vertices return -1.
int t3dCalculateNormalToPolygon(EQGfxPolygon_guess *polygon)
{
    int result = 1;

    if (polygon->vertexCount < 3)
        result = -1;
    else
    {
        EQGfxVertexIndices_guess *indices = polygon->vertexList;
        float *positions = polygon->positions;
        EQGfxVector3_guess *corner = (EQGfxVector3_guess *)&positions[indices->cornerIndex * 3];
        float cornerX = corner->x;
        float cornerY = corner->y;
        float cornerZ = corner->z;
        EQGfxVector3_guess *start = (EQGfxVector3_guess *)&positions[indices->startIndex * 3];
        EQGfxVector3_guess *end = (EQGfxVector3_guess *)&positions[indices->endIndex * 3];
        float firstX = start->x - cornerX;
        float firstY = start->y - cornerY;
        float firstZ = start->z - cornerZ;
        float secondX = end->x - cornerX;
        float secondY = end->y - cornerY;
        float secondZ = end->z - cornerZ;
        float normalX = secondZ * firstY - secondY * firstZ;
        float normalY = secondX * firstZ - secondZ * firstX;
        float normalZ = secondY * firstX - secondX * firstY;
        float scale = EQ_CDECL(float, 0x1004EFDA, (float))(normalZ * normalZ + normalY * normalY + normalX * normalX);
        float scaledX = scale * normalX;
        float scaledY = scale * normalY;
        float scaledZ = scale * normalZ;

        polygon->distance = -(scaledZ * cornerZ + scaledY * cornerY + scaledX * cornerX);
        polygon->normal.x = scaledX;
        polygon->normal.y = scaledY;
        polygon->normal.z = scaledZ;
    }
    return result;
}

// FUNCTION: EQGFX 0x10011AC6
// __cdecl (the callee pops 0; ECX is not an input), int (always 0; no call site seen to confirm).
int t3dGetPolygonPlane(T3DPolygon_guess *polygon, T3DPlane_guess *plane)
{
    plane->type = 0x2f;
    plane->normal = polygon->normal;
    plane->distance = polygon->distance;
    return 0;
}

// FUNCTION: EQGFX 0x10011CEC
// __cdecl (the callee pops 0; ECX is not an input), void (EAX is not read after the call).
void t3dHierarchicalSpriteUpdate(T3DHierarchicalSprite_guess *hierarchicalSprite, unsigned int animationFrame)
{
    EQGfxQuaternion_guess orientation = { 1.0f, 0.0f, 0.0f, 0.0f };
    EQGfxVector3_guess position = { 0.0f, 0.0f, 0.0f };

    EQ_CDECL(void, 0x10011D4C, (T3DDagObject_guess *, EQGfxVector3_guess, EQGfxQuaternion_guess, float, unsigned int, int))(
        (T3DDagObject_guess *)hierarchicalSprite->elements, position, orientation, 1.0f, animationFrame,
        hierarchicalSprite->processFlags);
}

// FUNCTION: EQGFX 0x10011d4c
// Poses a dag subtree at a frame: blends keyframes and any fade to a new track, chains the parent transform
void _t3dHierarchicalSpriteProcessDags_guess(T3DDagObject_guess *dag,
    float positionX, float positionY, float positionZ,
    float parentQuatW, float parentQuatX, float parentQuatY, float parentQuatZ,
    float parentScale, unsigned int animationFrame, int flags)
{
    T3DAnimationClip_guess *activeCycle;
    T3DAnimationClip_guess *completedCycle;
    T3DAnimationClip_guess *currentAnimation;
    T3DAnimationSequence_guess *currentSequence;
    T3DAnimationSequence_guess *transitionSequence;
    T3DAnimationTransform_guess *fullFrameA;
    T3DAnimationTransform_guess *fullFrameB;
    T3DHierarchicalSprite_guess *hierarchicalSprite;
    float decodedFrameA[8];
    float decodedFrameB[8];
    T3DAnimationTransform_guess transitionTransform;
    float rotatedPosition[3];
    float transitionFraction;
    float currentBlend;
    float transitionBlend;
    float *matrixValue;
    unsigned int currentFirstFrame;
    unsigned int currentSecondFrame;
    unsigned int transitionFirstFrame;
    unsigned int transitionSecondFrame;
    unsigned int childIndex;
    int cycleDuration;
    int frameCount;
    int matrixCount;

    activeCycle = dag->activeCycle;
    hierarchicalSprite = dag->hierarchicalSprite;
    currentAnimation = dag->currentAnimation;
    if (activeCycle != 0)
    {
        if (animationFrame < (unsigned int)(dag->transitionStartFrame + dag->transitionDuration))
        {
            EQGFX_T3D_UPDATE_ANIMATION_CYCLE(activeCycle, &dag->transitionState, animationFrame);
            cycleDuration = dag->transitionDuration;
            transitionFraction = (float)(animationFrame - (unsigned int)dag->transitionStartFrame) /
                (float)cycleDuration;
            transitionSequence = activeCycle->sequence;
        }
        else
        {
            completedCycle = activeCycle;
            dag->activeCycle = 0;
            activeCycle = 0;
            dag->currentState = dag->transitionState;
            dag->currentAnimation = completedCycle;
            currentAnimation = completedCycle;
        }
    }
    currentSequence = currentAnimation->sequence;
    if (flags == 0 && activeCycle == 0)
        EQGFX_T3D_UPDATE_ANIMATION_CYCLE(currentAnimation, &dag->currentState, animationFrame);

    if (dag->currentState.phase <= EQGFX_ZERO_F_GUESS)
    {
        if (currentSequence->fullFrames == 0)
            frameCount = currentSequence->packedFrames->frameCount;
        else
            frameCount = currentSequence->fullFrames->frameCount;
        currentFirstFrame = frameCount - (unsigned int)dag->currentState.firstFrame - 1;
        currentSecondFrame = frameCount - (unsigned int)dag->currentState.secondFrame - 1;
    }
    else
    {
        currentFirstFrame = dag->currentState.firstFrame;
        currentSecondFrame = dag->currentState.secondFrame;
    }
    currentBlend = dag->currentState.blend;

    if (dag->animationMode == 0)
    {
        if (activeCycle != 0)
        {
            if (dag->transitionState.phase <= EQGFX_ZERO_F_GUESS)
            {
                if (transitionSequence->fullFrames == 0)
                    frameCount = transitionSequence->packedFrames->frameCount;
                else
                    frameCount = transitionSequence->fullFrames->frameCount;
                transitionFirstFrame = frameCount - (unsigned int)dag->transitionState.firstFrame - 1;
                transitionSecondFrame = frameCount - (unsigned int)dag->transitionState.secondFrame - 1;
            }
            else
            {
                transitionFirstFrame = dag->transitionState.firstFrame;
                transitionSecondFrame = dag->transitionState.secondFrame;
            }
            transitionBlend = dag->transitionState.blend;
            if (transitionFirstFrame == transitionSecondFrame)
            {
                if (transitionSequence->fullFrames == 0)
                {
                    EQGFX_T3D_UNPACK_ANIMATION_FRAME(
                        (const unsigned char *)transitionSequence->packedFrames->frameData +
                            transitionFirstFrame * EQGFX_T3D_PACKED_FRAME_SIZE,
                        &transitionTransform);
                }
                else
                {
                    fullFrameA = (T3DAnimationTransform_guess *)
                        ((unsigned char *)transitionSequence->fullFrames->frameData +
                         transitionFirstFrame * EQGFX_T3D_FULL_FRAME_SIZE);
                    transitionTransform = *fullFrameA;
                }
            }
            else if (transitionSequence->fullFrames == 0)
            {
                EQGFX_T3D_UNPACK_ANIMATION_FRAME(
                    (const unsigned char *)transitionSequence->packedFrames->frameData +
                        transitionFirstFrame * EQGFX_T3D_PACKED_FRAME_SIZE,
                    decodedFrameA);
                EQGFX_T3D_UNPACK_ANIMATION_FRAME(
                    (const unsigned char *)transitionSequence->packedFrames->frameData +
                        transitionSecondFrame * EQGFX_T3D_PACKED_FRAME_SIZE,
                    decodedFrameB);
                EQGFX_T3D_INTERPOLATE_VECTOR(&decodedFrameA[4], &decodedFrameB[4],
                    transitionBlend, &transitionTransform.position.x);
                EQGFX_T3D_INTERPOLATE_SCALE(decodedFrameA[7], decodedFrameB[7],
                    transitionBlend, &transitionTransform.scale);
                EQGFX_T3D_SLERP_QUATERNIONS(decodedFrameA, decodedFrameB,
                    transitionBlend, transitionTransform.quaternion);
            }
            else
            {
                fullFrameA = (T3DAnimationTransform_guess *)
                    ((unsigned char *)transitionSequence->fullFrames->frameData +
                     transitionFirstFrame * EQGFX_T3D_FULL_FRAME_SIZE);
                fullFrameB = (T3DAnimationTransform_guess *)
                    ((unsigned char *)transitionSequence->fullFrames->frameData +
                     transitionSecondFrame * EQGFX_T3D_FULL_FRAME_SIZE);
                EQGFX_T3D_INTERPOLATE_VECTOR(&fullFrameA->position.x, &fullFrameB->position.x,
                    transitionBlend, &transitionTransform.position.x);
                EQGFX_T3D_INTERPOLATE_SCALE(fullFrameA->scale, fullFrameB->scale,
                    transitionBlend, &transitionTransform.scale);
                EQGFX_T3D_SLERP_QUATERNIONS(fullFrameA->quaternion, fullFrameB->quaternion,
                    transitionBlend, transitionTransform.quaternion);
            }
        }

        if (currentFirstFrame == currentSecondFrame)
        {
            if (currentSequence->fullFrames == 0)
            {
                EQGFX_T3D_UNPACK_ANIMATION_FRAME(
                    (const unsigned char *)currentSequence->packedFrames->frameData +
                        currentFirstFrame * EQGFX_T3D_PACKED_FRAME_SIZE,
                    dag->frameQuat);
            }
            else
            {
                fullFrameA = (T3DAnimationTransform_guess *)
                    ((unsigned char *)currentSequence->fullFrames->frameData +
                     currentFirstFrame * EQGFX_T3D_FULL_FRAME_SIZE);
                *(T3DAnimationTransform_guess *)dag->frameQuat = *fullFrameA;
            }
        }
        else if (currentSequence->fullFrames == 0)
        {
            EQGFX_T3D_UNPACK_ANIMATION_FRAME(
                (const unsigned char *)currentSequence->packedFrames->frameData +
                    currentFirstFrame * EQGFX_T3D_PACKED_FRAME_SIZE,
                decodedFrameA);
            EQGFX_T3D_UNPACK_ANIMATION_FRAME(
                (const unsigned char *)currentSequence->packedFrames->frameData +
                    currentSecondFrame * EQGFX_T3D_PACKED_FRAME_SIZE,
                decodedFrameB);
            EQGFX_T3D_INTERPOLATE_VECTOR(&decodedFrameA[4], &decodedFrameB[4],
                currentBlend, &dag->framePosition.x);
            EQGFX_T3D_INTERPOLATE_SCALE(decodedFrameA[7], decodedFrameB[7],
                currentBlend, &dag->frameScale);
            EQGFX_T3D_SLERP_QUATERNIONS(decodedFrameA, decodedFrameB,
                currentBlend, dag->frameQuat);
        }
        else
        {
            fullFrameA = (T3DAnimationTransform_guess *)
                ((unsigned char *)currentSequence->fullFrames->frameData +
                 currentFirstFrame * EQGFX_T3D_FULL_FRAME_SIZE);
            fullFrameB = (T3DAnimationTransform_guess *)
                ((unsigned char *)currentSequence->fullFrames->frameData +
                 currentSecondFrame * EQGFX_T3D_FULL_FRAME_SIZE);
            EQGFX_T3D_INTERPOLATE_VECTOR(&fullFrameA->position.x, &fullFrameB->position.x,
                currentBlend, &dag->framePosition.x);
            EQGFX_T3D_INTERPOLATE_SCALE(fullFrameA->scale, fullFrameB->scale,
                currentBlend, &dag->frameScale);
            EQGFX_T3D_SLERP_QUATERNIONS(fullFrameA->quaternion, fullFrameB->quaternion,
                currentBlend, dag->frameQuat);
        }

        if (activeCycle != 0)
        {
            EQGFX_T3D_INTERPOLATE_VECTOR(&dag->framePosition.x,
                &transitionTransform.position.x, transitionFraction, &dag->framePosition.x);
            EQGFX_T3D_INTERPOLATE_SCALE(dag->frameScale, transitionTransform.scale,
                transitionFraction, &dag->frameScale);
            EQGFX_T3D_SLERP_QUATERNIONS(dag->frameQuat, transitionTransform.quaternion,
                transitionFraction, dag->frameQuat);
        }
    }

    dag->frameScale = parentScale * dag->frameScale;
    if (dag->frameScale == EQGFX_ZERO_F_GUESS)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_SCALE_MESSAGE);
        dag->frameScale = 1.0f;
        dag->inverseScale = 1.0f;
        dag->inverseScaleSquared = 1.0f;
    }
    else
    {
        dag->inverseScale = 1.0f / dag->frameScale;
        dag->inverseScaleSquared = dag->inverseScale * dag->inverseScale;
    }

    EQGFX_T3D_ROTATE_POINT(&dag->framePosition.x, &parentQuatW, rotatedPosition);
    positionX = rotatedPosition[0] * parentScale + positionX;
    positionY = rotatedPosition[1] * parentScale + positionY;
    positionZ = rotatedPosition[2] * parentScale + positionZ;
    EQGFX_T3D_MULTIPLY_QUATERNIONS(&parentQuatW, dag->frameQuat, dag->resultQuat);
    if (hierarchicalSprite->type == EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE)
    {
        _t3dHierarchicalSpriteProcessDags_guess(
            (T3DDagObject_guess *)hierarchicalSprite->elements,
            positionX, positionY, positionZ,
            dag->resultQuat[0], dag->resultQuat[1], dag->resultQuat[2], dag->resultQuat[3],
            dag->frameScale, animationFrame, flags);
    }
    EQGFX_T3D_QUATERNION_TO_MATRIX(dag->resultQuat, dag->rotation);
    matrixValue = dag->rotation;
    matrixCount = 9;
    do
    {
        *matrixValue = dag->frameScale * *matrixValue;
        ++matrixValue;
        --matrixCount;
    } while (matrixCount != 0);
    parentScale = 0.0f;
    dag->translation.x = positionX;
    dag->translation.y = positionY;
    dag->translation.z = positionZ;
    if (dag->childCount != 0)
    {
        childIndex = 0;
        do
        {
            _t3dHierarchicalSpriteProcessDags_guess(dag->children[childIndex],
                positionX, positionY, positionZ,
                dag->resultQuat[0], dag->resultQuat[1], dag->resultQuat[2], dag->resultQuat[3],
                dag->frameScale, animationFrame, flags);
            ++childIndex;
        } while (childIndex < dag->childCount);
    }
}

// FUNCTION: EQGFX 0x10012336
// Blends a 3D vector between two values used in a sprite transform.
// __cdecl (the callee pops 0; ECX is not an input), void (no caller reads EAX after the call).
// Three-float endpoints: equal ones are copied verbatim (the original's MOVSD trio), otherwise
// result[i] = (end[i] - start[i]) * blend + start[i].
void t3dInterpolateVector_guess(float *start, float *end, float blend, float *result)
{
    if (*start == *end && start[1] == end[1] && start[2] == end[2])
        *(EQGfxVector3_guess *)result = *(EQGfxVector3_guess *)start;
    else
    {
        *result = (*end - *start) * blend + *start;
        result[1] = (end[1] - start[1]) * blend + start[1];
        result[2] = (end[2] - start[2]) * blend + start[2];
    }
}

// FUNCTION: EQGFX 0x10012398
// Blends a sprite scale value between two animation samples.
// __cdecl (the callee pops 0; ECX is not an input), void (all 5 callers overwrite EAX before reading it).
// Linear interpolation: equal endpoints keep `start` bit for bit, otherwise start + (end - start) * blend;
// the single store through `result` is shared by both arms (the original's FSTP is at the merge point).
void t3dInterpolateFloat_guess(float start, float end, float blend, float *result)
{
    if (start != end)
        start = (end - start) * blend + start;
    *result = start;
}

// eqgfx_dx7.dll's own .rdata conversion scales: the float pool entries the original's x87 multiplies read.
#define EQGFX_FRAME_QUATERNION_SCALE EQ_GLOBAL(float, 0x100762A0)    // 2^-14 = 1/16384
#define EQGFX_FRAME_POSITION_SCALE  EQ_GLOBAL(float, 0x1007629C)    // 2^-8 = 1/256

// FUNCTION: EQGFX 0x100123be
// Expands packed animation-frame values into floats for sprite posing.
// __cdecl (the callee pops 0; ECX is not an input), void (no caller reads EAX after the call).
// Widens the 8 packed shorts at src into 8 floats at dst: src[0..3] scaled by 2^-14, then src[4..6]
// sign-extended and src[7] zero-extended (the original's only MOVZX), all scaled by 2^-8.
void t3dUnpackAnimationFrame_guess(short *src, float *dst)
{
    dst[0] = src[0] * EQGFX_FRAME_QUATERNION_SCALE;
    dst[1] = src[1] * EQGFX_FRAME_QUATERNION_SCALE;
    dst[2] = src[2] * EQGFX_FRAME_QUATERNION_SCALE;
    dst[3] = src[3] * EQGFX_FRAME_QUATERNION_SCALE;
    dst[4] = src[4] * EQGFX_FRAME_POSITION_SCALE;
    dst[5] = src[5] * EQGFX_FRAME_POSITION_SCALE;
    dst[6] = src[6] * EQGFX_FRAME_POSITION_SCALE;
    dst[7] = (unsigned short)src[7] * EQGFX_FRAME_POSITION_SCALE;
}

// FUNCTION: EQGFX 0x1001244F
// Registers a named hierarchical-sprite definition in the world: its dags, attached skins and collision volume
// __cdecl (the callee pops 0). Defines the hierarchical sprite `name` and returns its entry, or 0 after undoing every step.
T3DHierarchicalSpriteInstance_guess *__cdecl DefineHierarchicalSprite_guess(void *dictionary, char *name,
    unsigned int elementCount, T3DHierarchicalSpriteElement_guess *elements, EQGfxVector3_guess *offset,
    void *volume, unsigned int definitionFlags, unsigned int skinCount, void **skins, int *skinValues)
{
    T3DHierarchicalSpriteInstance_guess *sprite;
    T3DHierarchicalSprite_guess *defaultInstance;
    T3DHierarchicalSpriteElement_guess *element;
    T3DHierarchicalSpriteElement_guess *elementEnd;
    T3DHierarchicalSpriteElement_guess *previous;
    T3DHierarchicalSpriteElement_guess *freeElement;
    int *existing;
    int *skin;
    int *childBuffer;
    const char *message;
    unsigned int skinCapacity;
    unsigned int skinIndex;
    unsigned int elementIndex;
    unsigned int childIndex;
    int objectType;

    existing = (int *)EQGFX_T3D_GET_POINTER(dictionary, name);
    if (existing == 0 || *existing == EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE_ENTRY)
    {
        sprite = (T3DHierarchicalSpriteInstance_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x38);
        if (sprite != 0)
        {
            sprite->type = EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE_ENTRY;
            sprite->elementCount = elementCount;
            sprite->state = 1;
            sprite->definitionFlags = definitionFlags;
            sprite->dictionaryEntry = EQGFX_T3D_ALLOCATE_MEMORY(EQGFX_STRLEN(name) + 1);
            if (sprite->dictionaryEntry == 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_TAG_ALLOC_FAILED_MESSAGE);
                EQGFX_T3D_FREE_MEMORY(sprite);
                return 0;
            }
            EQGFX_COPY_STRING(sprite->dictionaryEntry, name);
            if (EQGFX_T3D_ADD_HIERARCHICAL_SPRITE(dictionary, sprite) != 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_ADD_ITEM_FAILED_MESSAGE);
                EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
                EQGFX_T3D_FREE_MEMORY(sprite);
                return 0;
            }
            if (EQGFX_T3D_ADD_ENTRY(dictionary, sprite) < 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_ADD_ENTRY_FAILED_MESSAGE);
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
                EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
                EQGFX_T3D_FREE_MEMORY(sprite);
                return 0;
            }
            defaultInstance = (T3DHierarchicalSprite_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x60);
            if (defaultInstance == 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_INSTANCE_ALLOC_FAILED_MESSAGE);
                EQGFX_T3D_REMOVE_ENTRY(dictionary, name);
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
                EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
                EQGFX_T3D_FREE_MEMORY(sprite);
                return 0;
            }
            sprite->definition = defaultInstance;
            defaultInstance->dictionaryKey = 0;
            *(int *)&defaultInstance->flags = 0;
            defaultInstance->state = 1;
            defaultInstance->type = EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE;
            defaultInstance->offset = *offset;
            defaultInstance->definition = (T3DHierarchicalSpriteDefinition_guess *)sprite;
            defaultInstance->processFlags = 0;
            defaultInstance->reserved_38 = 0;
            defaultInstance->reserved_3c = 0;
            defaultInstance->childSpriteCount = 0;
            defaultInstance->childSpriteCapacity = 0;
            defaultInstance->childSprites = 0;
            skinCapacity = skinCount + 5;
            defaultInstance->childSprites = EQGFX_T3D_ALLOCATE_MEMORY(skinCapacity * 4);
            if (defaultInstance->childSprites == 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_SKIN_ALLOC_FAILED_MESSAGE);
                EQGFX_T3D_FREE_MEMORY(defaultInstance);
                EQGFX_T3D_REMOVE_ENTRY(dictionary, name);
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
                EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
                EQGFX_T3D_FREE_MEMORY(sprite);
                return 0;
            }
            defaultInstance->childSpriteBuffer = EQGFX_T3D_ALLOCATE_MEMORY(skinCapacity * 4);
            if (defaultInstance->childSpriteBuffer == 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_SKIN_ALLOC_FAILED_MESSAGE);
                EQGFX_T3D_FREE_MEMORY(defaultInstance->childSprites);
                EQGFX_T3D_FREE_MEMORY(defaultInstance);
                EQGFX_T3D_REMOVE_ENTRY(dictionary, name);
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
                EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
                EQGFX_T3D_FREE_MEMORY(sprite);
                return 0;
            }
            defaultInstance->childSpriteCapacity = skinCapacity;
            defaultInstance->childSpriteCount = skinCount;
            skinIndex = 0;
            if (skinCount != 0)
            {
                do
                {
                    skin = (int *)skins[skinIndex];
                    if (*skin != EQGFX_OBJECT_TYPE_DM_SPRITE)
                    {
                        message = EQGFX_HIERARCHICAL_SPRITE_INVALID_SKIN_TYPE_MESSAGE;
                        goto DEFAULT_INSTANCE_FAILED;
                    }
                    defaultInstance->childSprites[skinIndex] = skin;
                    skinIndex++;
                } while (skinIndex < skinCount);
            }
            skinIndex = 0;
            if (skinCount != 0)
            {
                do
                {
                    ((int *)defaultInstance->childSpriteBuffer)[skinIndex] = skinValues[skinIndex];
                    skinIndex++;
                } while (skinIndex < skinCount);
            }
            defaultInstance->volume = volume;
            defaultInstance->elements = elements;
            defaultInstance->dagCount = elementCount;
            if (EQGFX_T3D_ADD_HIERARCHICAL_SPRITE(dictionary, defaultInstance) != 0)
            {
                message = EQGFX_HIERARCHICAL_SPRITE_INSTANCE_ADD_ITEM_FAILED_MESSAGE;
DEFAULT_INSTANCE_FAILED:
                EQGFX_OUTPUT_DEBUG_STRING(message);
                EQGFX_T3D_FREE_MEMORY(defaultInstance->childSpriteBuffer);
                EQGFX_T3D_FREE_MEMORY(defaultInstance->childSprites);
                EQGFX_T3D_FREE_MEMORY(defaultInstance);
                EQGFX_T3D_REMOVE_ENTRY(dictionary, name);
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
                EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
                EQGFX_T3D_FREE_MEMORY(sprite);
                return 0;
            }
            elementIndex = 0;
            if (elementCount != 0)
            {
                element = elements;
                do
                {
                    if ((element->bufferCount << 2) == 0)
                    {
                        element->buffer = 0;
                    }
                    else
                    {
                        childBuffer = (int *)EQGFX_T3D_ALLOCATE_MEMORY(element->bufferCount << 2);
                        childIndex = 0;
                        if (childBuffer == 0)
                        {
                            EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_SUBDAG_ALLOC_FAILED_MESSAGE);
                            if (elementIndex != 0)
                            {
                                freeElement = elements;
                                do
                                {
                                    EQGFX_T3D_FREE_MEMORY(freeElement->buffer);
                                    freeElement++;
                                    elementIndex--;
                                } while (elementIndex != 0);
                            }
                            goto ELEMENT_FAILED;
                        }
                        if (element->bufferCount != 0)
                        {
                            do
                            {
                                childBuffer[childIndex] = ((((int *)element->buffer)[childIndex] - (int)elements)
                                    / EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE) * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE
                                    + (int)elements;
                                childIndex++;
                            } while (childIndex < element->bufferCount);
                        }
                        element->buffer = childBuffer;
                    }
                    elementIndex++;
                    element++;
                } while (elementIndex < elementCount);
            }
            element = elements;
            elementEnd = element + sprite->elementCount;
            while (element < elementEnd)
            {
                if (element->object == 0)
                {
                    element->object = EQGFX_EMPTY_DAG_OBJECT;
                }
                objectType = *element->object;
                if (objectType != 0)
                {
                    if (objectType != EQGFX_OBJECT_TYPE_3D_SPRITE && objectType != EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE
                        && objectType != EQGFX_OBJECT_TYPE_DM_SPRITE && objectType != EQGFX_OBJECT_TYPE_DM_SPRITE_ALIAS
                        && objectType != EQGFX_OBJECT_TYPE_STRING_SPRITE)
                    {
                        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_HIERARCHICAL_SPRITE_INVALID_SPRITE_TYPE_MESSAGE);
                        for (previous = element - 1; (unsigned int)previous >= (unsigned int)elements; previous--)
                        {
                            objectType = *previous->object;
                            if (objectType == EQGFX_OBJECT_TYPE_3D_SPRITE || objectType == EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE
                                || objectType == EQGFX_OBJECT_TYPE_DM_SPRITE || objectType == EQGFX_OBJECT_TYPE_DM_SPRITE_ALIAS
                                || objectType == EQGFX_OBJECT_TYPE_STRING_SPRITE)
                            {
                                EQGFX_T3D_UNSHARE_OBJECT(previous->object);
                            }
                        }
ELEMENT_FAILED:
                        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, defaultInstance);
                        EQGFX_T3D_FREE_MEMORY(defaultInstance->childSpriteBuffer);
                        EQGFX_T3D_FREE_MEMORY(defaultInstance->childSprites);
                        EQGFX_T3D_FREE_MEMORY(defaultInstance);
                        EQGFX_T3D_REMOVE_ENTRY(dictionary, name);
                        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
                        EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
                        EQGFX_T3D_FREE_MEMORY(sprite);
                        return 0;
                    }
                    EQGFX_T3D_SHARE_OBJECT(element->object);
                }
                element++;
            }
            skinIndex = 0;
            if (defaultInstance->childSpriteCount != 0)
            {
                do
                {
                    EQGFX_T3D_SHARE_OBJECT(defaultInstance->childSprites[skinIndex]);
                    skinIndex++;
                } while (skinIndex < defaultInstance->childSpriteCount);
            }
            return sprite;
        }
        message = EQGFX_HIERARCHICAL_SPRITE_ALLOC_FAILED_MESSAGE;
    }
    else
    {
        message = EQGFX_HIERARCHICAL_SPRITE_TAG_CONFLICT_MESSAGE;
    }
    EQGFX_OUTPUT_DEBUG_STRING(message);
    return 0;
}

// The node data the walk visits: definition +0x8 (its first dword is a type tag), the parent +0xC and sprite
// +0x10 the walk assigns, and the child-node list +0xEC/+0xF0. No include/ header matches this layout yet.
typedef struct T3DSpriteDefinition_guess
{
    int           type;                 // 0x00: 0x14 makes the walk also store the parent at +0x18
    unsigned char pad_04[0x14];         // 0x04
    void         *parent;               // 0x18
    struct T3DSpriteDefinition_guess *child;   // 0x1c: read by the validity check; its tag must be 0x14
} T3DSpriteDefinition_guess;            // 0x20

typedef struct T3DSpriteNode_guess
{
    unsigned char pad_00[8];                    // 0x00
    T3DSpriteDefinition_guess *definition;      // 0x08
    void         *parent;                       // 0x0c: the node the recursion came from
    void         *sprite;                       // 0x10: the hierarchy root every call carries
    unsigned char pad_14[0xec - 0x14];          // 0x14
    unsigned int  childCount;                   // 0xec
    struct T3DSpriteNode_guess **children;      // 0xf0
} T3DSpriteNode_guess;                          // 0xf4

// FUNCTION: EQGFX 0x100128BB
// Removes a hierarchical sprite definition and releases its component sprites.
int __cdecl t3dUndefineHierarchicalSprite(void *dictionary, T3DHierarchicalSpriteInstance_guess *sprite)
{
    T3DHierarchicalSprite_guess *definition;
    T3DHierarchicalSpriteElement_guess *element;
    T3DHierarchicalSpriteElement_guess *elementEnd;
    int *object;
    unsigned int index;
    int objectType;
    int result;

    definition = sprite->definition;
    if (sprite->state != 1 || definition->state != 1)
        return 0xFFFF3D44;

    EQGFX_T3D_DESTROY_VOLUME(dictionary, definition->volume);
    if ((definition->flags & 1) != 0)
        EQGFX_T3D_REMOVE_ENTRY(dictionary, definition->dictionaryKey);

    EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, definition);

    index = 0;
    if (definition->childSpriteCount > 0)
    {
        do
        {
            EQGFX_T3D_UNSHARE_OBJECT(definition->childSprites[index]);
            EQGFX_S3D_DESTROY_DM_SPRITE(dictionary, definition->childSprites[index]);
            index++;
        } while (index < definition->childSpriteCount);
    }

    EQGFX_T3D_FREE_MEMORY(definition->childSprites);
    EQGFX_T3D_FREE_MEMORY(definition->childSpriteBuffer);

    element = definition->elements;
    elementEnd = element + sprite->elementCount;
    for (; element < elementEnd; element++)
    {
        object = element->object;
        objectType = *object;
        if (objectType != 0)
        {
            switch (objectType)
            {
                case EQGFX_OBJECT_TYPE_STRING_SPRITE:
                    EQGFX_T3D_UNSHARE_OBJECT(object);
                    EQGFX_S3D_DESTROY_STRING_SPRITE(dictionary, element->object);
                    break;
                case EQGFX_OBJECT_TYPE_DM_SPRITE_ALIAS:
                    EQGFX_T3D_UNSHARE_OBJECT(object);
                    break;
                case EQGFX_OBJECT_TYPE_DM_SPRITE:
                    EQGFX_T3D_UNSHARE_OBJECT(object);
                    EQGFX_S3D_DESTROY_DM_SPRITE(dictionary, element->object);
                    break;
                case EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE:
                    EQGFX_T3D_UNSHARE_OBJECT(object);
                    EQGFX_T3D_DESTROY_HIERARCHICAL_SPRITE(dictionary, element->object);
                    break;
                case EQGFX_OBJECT_TYPE_3D_SPRITE:
                    EQGFX_T3D_UNSHARE_OBJECT(object);
                    EQGFX_T3D_DESTROY_3D_SPRITE(dictionary, element->object);
                    break;
                default:
                    EQGFX_DEBUG_BREAK();
                    break;
            }
        }

        if (element->elementBuffer != 0)
            EQGFX_T3D_FREE_MEMORY(element->elementBuffer);
        EQGFX_T3D_FREE_MEMORY(element->buffer);
    }

    EQGFX_T3D_FREE_MEMORY(definition->elements);
    if (definition->dictionaryKey != 0)
        EQGFX_T3D_FREE_MEMORY(definition->dictionaryKey);
    EQGFX_T3D_FREE_MEMORY(definition);

    if ((sprite->flags & 1) != 0)
        EQGFX_T3D_REMOVE_ENTRY(dictionary, sprite->dictionaryEntry);

    result = EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
    if (sprite->dictionaryEntry != 0)
        EQGFX_T3D_FREE_MEMORY(sprite->dictionaryEntry);
    EQGFX_T3D_FREE_MEMORY(sprite);
    return result;
}

// FUNCTION: EQGFX 0x10012A60
// Instantiates a hierarchical sprite from its definition, with its own dag tree, volume and particle clouds
// __cdecl (three stack arguments; the callee pops 0 bytes). Copies and initializes a hierarchical sprite, then cleans up partial allocations on failure.
T3DHierarchicalSprite_guess *__cdecl t3dCreateHierarchicalSprite(
    void *dictionary,
    T3DHierarchicalSpriteInstance_guess *instance,
    unsigned int volume)
{
    T3DHierarchicalSprite_guess *sprite;
    T3DHierarchicalSprite_guess *definition;
    T3DDagObject_guess *sourceDags;
    T3DDagObject_guess *dag;
    T3DDagObject_guess *dagEnd;
    T3DDagObject_guess *cleanupDag;
    void *allocatedMemory;
    char *errorMessage;
    unsigned int dagIndex;
    unsigned int linkIndex;
    unsigned int sourceChildOffset;
    unsigned int allocationSize;
    int createdDagCount;
    int remainingCleanupCount;
    int sourceOffset;
    int sourceIndex;
    int objectType;
    EQGfxVector3_guess spawnOffset;

    createdDagCount = 0;
    sprite = (T3DHierarchicalSprite_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x60);
    if (sprite == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_CREATE_SPRITE_ALLOC_FAILED);
        return 0;
    }

    definition = instance->definition;
    *sprite = *definition;
    dagIndex = 0;
    sprite->dictionaryKey = 0;
    sprite->flags = 0;
    sprite->state = 0;
    sprite->volume = 0;
    sprite->childSpriteCount = 0;
    sprite->childSprites = 0;
    sprite->childSpriteBuffer = 0;
    sprite->dagCount = 0;
    sprite->elements = 0;
    sprite->particleCloudCount = 0;
    sprite->reserved_54 = 0;
    sprite->particleCloudSlots = 0;

    if (volume == 0)
        volume = (unsigned int)definition->volume;

    sprite->volume = EQGFX_T3D_DUPLICATE_VOLUME(dictionary, (void *)volume);
    if (sprite->volume == 0)
    {
        errorMessage = (char *)EQGFX_T3D_CREATE_SPRITE_VOLUME_FAILED;
        goto report_error;
    }

    allocationSize = definition->childSpriteCapacity << 2;
    sprite->childSprites = (void **)EQGFX_T3D_ALLOCATE_MEMORY(allocationSize);
    if (sprite->childSprites == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_CREATE_SPRITE_CHILD_LIST_FAILED);
        goto cleanup;
    }

    if (definition->childSpriteCount != 0)
    {
        do
        {
            sprite->childSprites[dagIndex] = definition->childSprites[dagIndex];
            EQGFX_T3D_SHARE_OBJECT(definition->childSprites[dagIndex]);
            sprite->childSpriteCount++;
            dagIndex++;
        } while (dagIndex < definition->childSpriteCount);
    }

    allocationSize = definition->childSpriteCapacity << 2;
    sprite->childSpriteBuffer = EQGFX_T3D_ALLOCATE_MEMORY(allocationSize);
    dagIndex = 0;
    if (sprite->childSpriteBuffer == 0)
    {
        errorMessage = (char *)EQGFX_T3D_CREATE_SPRITE_LINK_LIST_FAILED;
        goto report_error;
    }

    if (definition->childSpriteCount != 0)
    {
        do
        {
            sprite->childSpriteBuffer[dagIndex] = definition->childSpriteBuffer[dagIndex];
            dagIndex++;
        } while (dagIndex < definition->childSpriteCount);
    }

    allocationSize = instance->elementCount * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE;
    sprite->elements = (T3DHierarchicalSpriteElement_guess *)EQGFX_T3D_ALLOCATE_MEMORY(allocationSize);
    if (sprite->elements == 0)
    {
        errorMessage = (char *)EQGFX_T3D_CREATE_SPRITE_DAG_LIST_FAILED;
        goto report_error;
    }

    EQGFX_T3D_COPY_MEMORY(sprite->elements, definition->elements, allocationSize);
    volume = 0;
    if (instance->elementCount != 0)
    {
        sourceChildOffset = 0;
        do
        {
            dag = (T3DDagObject_guess *)((char *)sprite->elements + sourceChildOffset);
            allocationSize = dag->childCount << 2;
            if (allocationSize == 0)
            {
                dag->children = 0;
            }
            else
            {
                dag->children = (T3DDagObject_guess **)EQGFX_T3D_ALLOCATE_MEMORY(allocationSize);
                if (dag->children == 0)
                {
                    errorMessage = (char *)EQGFX_T3D_CREATE_SPRITE_SUBDAG_LIST_FAILED;
                    goto report_error;
                }

                sourceDags = (T3DDagObject_guess *)definition->elements;
                linkIndex = 0;
                if (dag->childCount != 0)
                {
                    do
                    {
                        sourceOffset = (int)((char *)sourceDags[sourceChildOffset / EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE].children[linkIndex] - (char *)definition->elements);
                        sourceIndex = sourceOffset / EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE;
                        dag->children[linkIndex] = (T3DDagObject_guess *)((char *)sprite->elements + sourceIndex * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
                        linkIndex++;
                    } while (linkIndex < dag->childCount);
                }
            }

            volume++;
            sourceChildOffset += EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE;
            sprite->dagCount++;
        } while (volume < instance->elementCount);
    }

    if (EQGFX_T3D_ADD_HIERARCHICAL_SPRITE(dictionary, sprite) == 0)
    {
        EQGFX_T3D_SETUP_SPRITE_HIERARCHY(sprite, sprite->elements, 0);
        dag = (T3DDagObject_guess *)sprite->elements;
        dagEnd = (T3DDagObject_guess *)((char *)sprite->elements + instance->elementCount * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
        for (;;)
        {
            if (dagEnd <= dag)
            {
                instance->state++;
                return sprite;
            }

            objectType = *(int *)dag->hierarchicalSprite;
            if (objectType != 0)
            {
                if (objectType == EQGFX_OBJECT_TYPE_3D_SPRITE ||
                    objectType == EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE ||
                    objectType == EQGFX_OBJECT_TYPE_DM_SPRITE)
                {
                    EQGFX_T3D_SHARE_OBJECT(dag->hierarchicalSprite);
                }
                else if (objectType == EQGFX_OBJECT_TYPE_DM_SPRITE_ALIAS)
                {
                    spawnOffset.x = 0.0f;
                    spawnOffset.y = 0.0f;
                    spawnOffset.z = 0.0f;
                    dag->hierarchicalSprite = (T3DHierarchicalSprite_guess *)EQGFX_T3D_CREATE_PARTICLE_CLOUD(
                        dictionary, dag->hierarchicalSprite, &spawnOffset, 0, 0);
                    if (dag->hierarchicalSprite == 0)
                        goto cleanup;
                    EQGFX_T3D_BIND_PARTICLE_CLOUD_SPAWN(dag->hierarchicalSprite, dag);
                }
                else if (objectType == EQGFX_OBJECT_TYPE_STRING_SPRITE)
                {
                    EQGFX_T3D_SHARE_OBJECT(dag->hierarchicalSprite);
                }
                else
                {
                    EQGFX_DEBUG_BREAK();
                }
            }

            dag = (T3DDagObject_guess *)((char *)dag + EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
            createdDagCount++;
        }
    }

    errorMessage = (char *)EQGFX_T3D_CREATE_SPRITE_ADD_ITEM_FAILED;
report_error:
    EQGFX_OUTPUT_DEBUG_STRING(errorMessage);

cleanup:
    if (sprite->elements != 0 && createdDagCount != 0)
    {
        cleanupDag = (T3DDagObject_guess *)sprite->elements;
        remainingCleanupCount = createdDagCount;
        do
        {
            objectType = *(int *)cleanupDag->hierarchicalSprite;
            if (objectType != 0)
            {
                if (objectType == EQGFX_OBJECT_TYPE_3D_SPRITE ||
                    objectType == EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE ||
                    objectType == EQGFX_OBJECT_TYPE_DM_SPRITE ||
                    objectType == EQGFX_OBJECT_TYPE_STRING_SPRITE)
                {
                    EQGFX_T3D_UNSHARE_OBJECT(cleanupDag->hierarchicalSprite);
                }
                else if (objectType == EQGFX_OBJECT_TYPE_PARTICLE_CLOUD)
                {
                    EQGFX_T3D_DESTROY_PARTICLE_CLOUD(dictionary, &cleanupDag->hierarchicalSprite);
                }
                else
                {
                    EQGFX_DEBUG_BREAK();
                }
            }
            cleanupDag = (T3DDagObject_guess *)((char *)cleanupDag + EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
            remainingCleanupCount--;
        } while (remainingCleanupCount != 0);
    }

    if (sprite->childSprites != 0)
    {
        dagIndex = 0;
        if (sprite->childSpriteCount != 0)
        {
            do
            {
                EQGFX_T3D_UNSHARE_OBJECT(sprite->childSprites[dagIndex]);
                dagIndex++;
            } while (dagIndex < sprite->childSpriteCount);
        }
        EQGFX_T3D_FREE_MEMORY(sprite->childSprites);
    }

    if (sprite->childSpriteBuffer != 0)
        EQGFX_T3D_FREE_MEMORY(sprite->childSpriteBuffer);

    if (sprite->elements != 0)
    {
        volume = 0;
        if (sprite->dagCount != 0)
        {
            cleanupDag = (T3DDagObject_guess *)sprite->elements;
            do
            {
                allocatedMemory = cleanupDag->children;
                if (allocatedMemory != 0)
                    EQGFX_T3D_FREE_MEMORY(allocatedMemory);
                volume++;
                cleanupDag++;
            } while (volume < sprite->dagCount);
        }
        EQGFX_T3D_FREE_MEMORY(sprite->elements);
    }

    if (sprite->volume != 0)
        EQGFX_T3D_DESTROY_VOLUME(dictionary, sprite->volume);

    if (sprite->particleCloudSlots != 0)
        EQGFX_T3D_FREE_MEMORY(sprite->particleCloudSlots);

    if (sprite->dictionaryIndex != -1)
        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);

    if ((sprite->flags & 1) != 0)
        EQGFX_T3D_REMOVE_ENTRY(dictionary, sprite->dictionaryKey);

    EQGFX_T3D_FREE_MEMORY(sprite);
    return 0;
}

// FUNCTION: EQGFX 0x10012e50
// Builds parent links through every part of a hierarchical sprite's model tree.
// __cdecl (caller pops 3 args; ECX is not an input), void (no caller reads EAX). Sets the parent at node+0xC
// and the sprite at node+0x10; a node whose definition tag is 0x14 also stores the parent at def+0x18.
void __cdecl setupSpriteHierarchy_guess(void *sprite, T3DSpriteNode_guess *node, void *parent)
{
    unsigned int i;

    if (node->definition->type == 0x14)
        node->definition->parent = parent;

    node->parent = parent;
    node->sprite = sprite;

    for (i = 0; i < node->childCount; i++)
        setupSpriteHierarchy_guess(sprite, node->children[i], node);
}

// FUNCTION: EQGFX 0x10012E9A
// __cdecl (three stack arguments; the callee pops 0). Returns the copy, or 0 after undoing a partial copy.
T3DHierarchicalSprite_guess *__cdecl t3dDuplicateHierarchicalSprite(void *dictionary, T3DHierarchicalSprite_guess *source, void *volume)
{
    T3DHierarchicalSprite_guess *sprite;
    T3DSpriteNode_guess *node;
    T3DSpriteNode_guess *sourceNode;
    T3DSpriteNode_guess *nodeEnd;
    EQGfxVector3_guess zeroOffset;
    unsigned int nodeIndex;
    unsigned int childIndex;
    int objectType;
    int walkedNodeCount;

    walkedNodeCount = 0;
    sprite = EQGFX_T3D_ALLOCATE_MEMORY(0x60);
    if (sprite == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_DUPLICATE_SPRITE_ALLOC_FAILED);
        return 0;
    }
    *sprite = *source;
    sprite->dictionaryKey = 0;
    *(int *)&sprite->flags = 0;
    sprite->state = 0;
    sprite->volume = 0;
    sprite->childSpriteCount = 0;
    sprite->childSprites = 0;
    sprite->childSpriteBuffer = 0;
    sprite->dagCount = 0;
    sprite->elements = 0;
    sprite->particleCloudCount = 0;
    sprite->reserved_54 = 0;
    sprite->particleCloudSlots = 0;
    sprite->volume = EQGFX_T3D_DUPLICATE_VOLUME(dictionary, volume != 0 ? volume : source->volume);
    if (sprite->volume == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_DUPLICATE_SPRITE_VOLUME_FAILED);
        goto cleanup;
    }
    sprite->childSprites = EQGFX_T3D_ALLOCATE_MEMORY(source->childSpriteCapacity << 2);
    if (sprite->childSprites == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_CREATE_SPRITE_CHILD_LIST_FAILED);
        goto cleanup;
    }
    childIndex = 0;
    if (source->childSpriteCount != 0)
    {
        do
        {
            sprite->childSprites[childIndex] = source->childSprites[childIndex];
            EQGFX_T3D_SHARE_OBJECT(source->childSprites[childIndex]);
            sprite->childSpriteCount++;
            childIndex++;
        } while (childIndex < source->childSpriteCount);
    }
    sprite->childSpriteBuffer = EQGFX_T3D_ALLOCATE_MEMORY(source->childSpriteCapacity << 2);
    if (sprite->childSpriteBuffer == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_CREATE_SPRITE_CHILD_LIST_FAILED);
        goto cleanup;
    }
    childIndex = 0;
    if (source->childSpriteCount != 0)
    {
        do
        {
            sprite->childSpriteBuffer[childIndex] = source->childSpriteBuffer[childIndex];
            childIndex++;
        } while (childIndex < source->childSpriteCount);
    }
    sprite->elements = EQGFX_T3D_ALLOCATE_MEMORY(source->dagCount * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
    if (sprite->elements == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_CREATE_SPRITE_DAG_LIST_FAILED);
        goto cleanup;
    }
    EQGFX_T3D_COPY_MEMORY(sprite->elements, source->elements, source->dagCount * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
    if (EQGFX_T3D_ADD_HIERARCHICAL_SPRITE(dictionary, sprite) != 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_CREATE_SPRITE_ADD_ITEM_FAILED);
        goto cleanup;
    }
    nodeIndex = 0;
    if (source->dagCount != 0)
    {
        node = (T3DSpriteNode_guess *)sprite->elements;
        sourceNode = (T3DSpriteNode_guess *)source->elements;
        do
        {
            if ((node->childCount << 2) == 0)
            {
                node->children = 0;
            }
            else
            {
                node->children = EQGFX_T3D_ALLOCATE_MEMORY(node->childCount << 2);
                if (node->children == 0)
                {
                    EQGFX_OUTPUT_DEBUG_STRING(EQGFX_T3D_DUPLICATE_SPRITE_DEFAULT_ADD_FAILED);
                    goto cleanup;
                }
                childIndex = 0;
                if (node->childCount != 0)
                {
                    do
                    {
                        node->children[childIndex] = (T3DSpriteNode_guess *)
                            ((((char *)sourceNode->children[childIndex] - (char *)source->elements)
                              / EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE)
                             * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE + (char *)sprite->elements);
                        childIndex++;
                    } while (childIndex < node->childCount);
                }
            }
            nodeIndex++;
            sprite->dagCount++;
            node = (T3DSpriteNode_guess *)((char *)node + EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
            sourceNode = (T3DSpriteNode_guess *)((char *)sourceNode + EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
        } while (nodeIndex < source->dagCount);
    }
    EQGFX_T3D_SETUP_SPRITE_HIERARCHY(sprite, sprite->elements, 0);
    node = (T3DSpriteNode_guess *)sprite->elements;
    nodeEnd = (T3DSpriteNode_guess *)((char *)sprite->elements + source->dagCount * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
    for (;;)
    {
        if (node >= nodeEnd)
        {
            sprite->definition->referenceCount++;
            return sprite;
        }
        objectType = node->definition->type;
        if (objectType != 0)
        {
            if (objectType == EQGFX_OBJECT_TYPE_3D_SPRITE || objectType == EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE ||
                objectType == EQGFX_OBJECT_TYPE_DM_SPRITE || objectType == EQGFX_OBJECT_TYPE_DM_SPRITE_ALIAS)
            {
                EQGFX_T3D_SHARE_OBJECT(node->definition);
            }
            else if (objectType == EQGFX_OBJECT_TYPE_PARTICLE_CLOUD)
            {
                zeroOffset.x = 0.0f;
                zeroOffset.y = 0.0f;
                zeroOffset.z = 0.0f;
                node->definition = EQGFX_T3D_CREATE_PARTICLE_CLOUD(dictionary,
                    *(void **)((char *)node->definition + EQGFX_PARTICLE_CLOUD_SOURCE_OFFSET), &zeroOffset, 0, 0);
                if (node->definition == 0)
                {
                    goto cleanup;
                }
                EQGFX_T3D_BIND_PARTICLE_CLOUD_SPAWN(node->definition, node);
            }
            else if (objectType == EQGFX_OBJECT_TYPE_STRING_SPRITE)
            {
                EQGFX_T3D_SHARE_OBJECT(node->definition);
            }
            else
            {
                EQGFX_DEBUG_BREAK();
            }
        }
        node = (T3DSpriteNode_guess *)((char *)node + EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
        walkedNodeCount++;
    }
cleanup:
    if (sprite->elements != 0 && walkedNodeCount != 0)
    {
        nodeIndex = walkedNodeCount;
        node = (T3DSpriteNode_guess *)sprite->elements;
        do
        {
            objectType = node->definition->type;
            if (objectType != 0)
            {
                if (objectType == EQGFX_OBJECT_TYPE_3D_SPRITE || objectType == EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE ||
                    objectType == EQGFX_OBJECT_TYPE_DM_SPRITE)
                {
                    EQGFX_T3D_UNSHARE_OBJECT(node->definition);
                }
                else if (objectType == EQGFX_OBJECT_TYPE_PARTICLE_CLOUD)
                {
                    EQGFX_T3D_DESTROY_PARTICLE_CLOUD(dictionary, &node->definition);
                }
                else if (objectType == EQGFX_OBJECT_TYPE_STRING_SPRITE)
                {
                    EQGFX_T3D_UNSHARE_OBJECT(node->definition);
                }
                else
                {
                    EQGFX_DEBUG_BREAK();
                }
            }
            node = (T3DSpriteNode_guess *)((char *)node + EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
            nodeIndex--;
        } while (nodeIndex != 0);
    }
    if (sprite->childSprites != 0)
    {
        childIndex = 0;
        if (sprite->childSpriteCount != 0)
        {
            do
            {
                EQGFX_T3D_UNSHARE_OBJECT(sprite->childSprites[childIndex]);
                childIndex++;
            } while (childIndex < sprite->childSpriteCount);
        }
        EQGFX_T3D_FREE_MEMORY(sprite->childSprites);
    }
    if (sprite->childSpriteBuffer != 0)
    {
        EQGFX_T3D_FREE_MEMORY(sprite->childSpriteBuffer);
    }
    if (sprite->elements != 0)
    {
        if (sprite->dagCount != 0)
        {
            nodeIndex = 0;
            node = (T3DSpriteNode_guess *)sprite->elements;
            do
            {
                if (node->children != 0)
                {
                    EQGFX_T3D_FREE_MEMORY(node->children);
                }
                node = (T3DSpriteNode_guess *)((char *)node + EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
                nodeIndex++;
            } while (nodeIndex < sprite->dagCount);
        }
        EQGFX_T3D_FREE_MEMORY(sprite->elements);
    }
    if (sprite->volume != 0)
    {
        EQGFX_T3D_DESTROY_VOLUME(dictionary, sprite->volume);
    }
    if (sprite->particleCloudSlots != 0)
    {
        EQGFX_T3D_FREE_MEMORY(sprite->particleCloudSlots);
    }
    if (sprite->dictionaryIndex != -1)
    {
        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, sprite);
    }
    if ((sprite->flags & 1) != 0)
    {
        EQGFX_T3D_REMOVE_ENTRY(dictionary, sprite->dictionaryKey);
    }
    EQGFX_T3D_FREE_MEMORY(sprite);
    return 0;
}

// FUNCTION: EQGFX 0x100132a2
// Destroys a hierarchical sprite, releasing its component objects, particle clouds, volume and allocated lists.
int __cdecl t3dDestroyHierarchicalSprite(void *dictionary, T3DHierarchicalSprite_guess *sprite)
{
    unsigned int childIndex;
    int objectType;
    int remainingParticleClouds;
    int *partObject;
    int **particleCloudSlot;
    int result;
    T3DHierarchicalSpriteElement_guess *element;
    T3DHierarchicalSpriteElement_guess *elementEnd;

    childIndex = 0;
    if (sprite->state != 0)
    {
        return EQGFX_HIERARCHICAL_SPRITE_IN_USE;
    }

    sprite->definition->referenceCount--;
    EQ_CDECL(void, 0x1001D5A1, (void *, void *))(dictionary, sprite->volume);
    if (sprite->childSpriteCount > 0)
    {
        do
        {
            EQ_CDECL(void, 0x10017DAD, (void *))(sprite->childSprites[childIndex]);
            EQ_CDECL(void, 0x10010575, (void *, void *))(dictionary, sprite->childSprites[childIndex]);
            childIndex++;
        } while (childIndex < sprite->childSpriteCount);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(sprite->childSprites);
    EQ_CDECL(void, 0x10050CBB, (void *))(sprite->childSpriteBuffer);

    element = sprite->elements;
    elementEnd = (T3DHierarchicalSpriteElement_guess *)((char *)element + sprite->definition->elementCount * EQGFX_HIERARCHICAL_SPRITE_ELEMENT_SIZE);
    for (; element < elementEnd; element++)
    {
        partObject = element->object;
        objectType = *partObject;
        switch (objectType)
        {
            case EQGFX_OBJECT_TYPE_3D_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(partObject);
                EQ_CDECL(void, 0x1000879D, (void *, void *))(dictionary, element->object);
                break;
            case EQGFX_OBJECT_TYPE_HIERARCHICAL_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(partObject);
                t3dDestroyHierarchicalSprite(dictionary, (T3DHierarchicalSprite_guess *)element->object);
                break;
            case EQGFX_OBJECT_TYPE_DM_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(partObject);
                EQ_CDECL(void, 0x10010575, (void *, void *))(dictionary, element->object);
                break;
            case EQGFX_OBJECT_TYPE_PARTICLE_CLOUD:
            case 0:
                break;
            case EQGFX_OBJECT_TYPE_STRING_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(partObject);
                EQ_CDECL(void, 0x1001ACF8, (void *, void *))(dictionary, element->object);
                break;
            default:
                EQGFX_DEBUG_BREAK();
                break;
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(element->buffer);
    }

    particleCloudSlot = sprite->particleCloudSlots;
    for (remainingParticleClouds = sprite->particleCloudCount;
         remainingParticleClouds != 0;
         remainingParticleClouds--)
    {
        if (*particleCloudSlot != 0)
        {
            if (**particleCloudSlot != EQGFX_OBJECT_TYPE_PARTICLE_CLOUD)
            {
                particleCloudSlot++;
            }
            else
            {
                EQ_CDECL(void, 0x10040217, (void *, void *))(dictionary, particleCloudSlot);
            }
        }
    }
    if (sprite->particleCloudSlots != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->particleCloudSlots);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(sprite->elements);
    if ((sprite->flags & 1) != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, sprite->dictionaryKey);
    }
    result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, sprite);
    if (sprite->dictionaryKey != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->dictionaryKey);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
    return result;
}

// kernel32 OutputDebugStringA through eqgfx_dx7.dll's import slot (the original's CALL dword ptr [0x100760c4]).
typedef void (__stdcall *OutputDebugStringA_t)(const char *);
#define EQGFX_OUTPUT_DEBUG_STRING_A EQ_GLOBAL(OutputDebugStringA_t, 0x100760C4)

// FUNCTION: EQGFX 0x1001342C
// Adds a skin to a hierarchical sprite and records the DAG link it follows.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Appends skin and dagIndex at skinCount; at skinCount == skinCapacity both lists grow by one slot first, and a failed grow reports to OutputDebugStringA and returns 0xffff3d3e (else 0).
int __cdecl s3dAttachSkinToHierarchicalSprite(T3DSprite_guess *sprite, void *skin, int dagIndex)
{
    int result;
    void *newSkinList;
    int *newSkinDagIndexList;
    char *message;

    result = 0;
    if (sprite->skinCount == sprite->skinCapacity)
    {
        newSkinList = EQGFX_T3D_REALLOCATE_MEMORY(sprite->skinList, sprite->skinCapacity * 4 + 8);
        if (newSkinList == 0)
        {
            message = EQ_ARRAY(char, 0x10086C18);
            goto reportFailure;
        }
        newSkinDagIndexList = EQGFX_T3D_REALLOCATE_MEMORY(sprite->skinDagIndex, sprite->skinCapacity * 4 + 8);
        if (newSkinDagIndexList == 0)
        {
            message = EQ_ARRAY(char, 0x10086BC4);
reportFailure:
            EQGFX_OUTPUT_DEBUG_STRING_A(message);
            result = 0xffff3d3e;
            goto done;
        }
        sprite->skinCapacity++;
        sprite->skinList = newSkinList;
        sprite->skinDagIndex = newSkinDagIndexList;
    }
    sprite->skinList[sprite->skinCount] = skin;
    EQGFX_T3D_SHARE_OBJECT(skin);
    sprite->skinDagIndex[sprite->skinCount] = dagIndex;
    sprite->skinCount++;
done:
    return result;
}

// FUNCTION: EQGFX 0x100134BA
// Returns one attached skin by index from a hierarchical sprite.
// __cdecl (callee pops 0 bytes; ECX is not an input), void * (callers read EAX). Returns skinList[skinIndex] of the sprite; an index at or past skinCount reports to OutputDebugStringA and returns 0.
void *__cdecl s3dGetSkinAttachedToHierarchicalSprite(unsigned int skinIndex, T3DSprite_guess *sprite)
{
    if (skinIndex >= sprite->skinCount)
    {
        EQGFX_OUTPUT_DEBUG_STRING_A(EQ_ARRAY(char, 0x10086C18));
        return 0;
    }
    return sprite->skinList[skinIndex];
}

// FUNCTION: EQGFX 0x100134DC
// Returns the DAG link index associated with an attached skin.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Returns skinDagIndex[skinIndex] of the sprite; an index at or past skinCount reports to OutputDebugStringA and returns -1.
int __cdecl s3dGetSkinAttachedToHierarchicalSpriteLinkUpdatesToDAGIndex(unsigned int skinIndex, T3DSprite_guess *sprite)
{
    if (skinIndex >= sprite->skinCount)
    {
        EQGFX_OUTPUT_DEBUG_STRING_A(EQ_ARRAY(char, 0x10086C18));
        return -1;
    }
    return sprite->skinDagIndex[skinIndex];
}

// FUNCTION: EQGFX 0x100134FF
// Removes an attached skin from a hierarchical sprite and destroys its DM sprite.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Detaches skin `skinIndex`: unshares and destroys its DM sprite, moves the later skinList and skinDagIndex entries down one slot (not when it is the last), then decrements skinCount; an index at or past skinCount reports to OutputDebugStringA and returns 0xffff3d3e (else 0).
int __cdecl s3dDetachSkinFromHierarchicalSprite(void *dictionary, unsigned int skinIndex, T3DSprite_guess *sprite)
{
    int result;
    unsigned int skinByteOffset;
    void **skinSlot;
    void *skin;

    result = 0;
    if (skinIndex >= sprite->skinCount)
    {
        EQGFX_OUTPUT_DEBUG_STRING_A(EQ_ARRAY(char, 0x10086C18));
        result = 0xffff3d3e;
        goto done;
    }
    skinByteOffset = skinIndex * 4;
    skinSlot = (void **)((char *)sprite->skinList + skinByteOffset);
    skin = *skinSlot;
    if (skinIndex < sprite->skinCount - 1)
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (skinSlot, sprite->skinList + skinIndex + 1, (sprite->skinCount - skinIndex - 1) * 4);
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            ((char *)sprite->skinDagIndex + skinByteOffset, sprite->skinDagIndex + skinIndex + 1,
             (sprite->skinCount - skinIndex - 1) * 4);
    }
    EQGFX_T3D_UNSHARE_OBJECT(skin);
    EQGFX_S3D_DESTROY_DM_SPRITE(dictionary, skin);
    sprite->skinCount--;
done:
    return result;
}

// FUNCTION: EQGFX 0x100135A5
// Checks that a hierarchical sprite definition has a child of the expected kind.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). 1 when definition is set, its tag is 0x13, its child (+0x1c) is set and the child's tag is 0x14; 0 otherwise.
int t3dIsHierarchicalSpriteDefinitionValid(T3DSpriteDefinition_guess *definition)
{
    if (definition == 0)
        return 0;
    if (definition->child == 0)
        return 0;
    if (definition->type != 0x13)
        return 0;
    if (definition->child->type == 0x14)
        return 1;
    return 0;
}

// FUNCTION: EQGFX 0x100135C7
// Checks that a hierarchical sprite points to a definition of the expected kind.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). 1 when sprite is set, its tag is 0x14, its definition (+0x5C) is set and the definition's tag is 0x13; 0 otherwise.
int t3dIsHierarchicalSpriteValid(T3DSprite_guess *sprite)
{
    T3DSpriteDefinition_guess *definition;

    if (sprite == 0)
        return 0;
    if (sprite->type != 0x14)
        return 0;
    definition = sprite->definition;
    if (definition != 0)
        return definition->type == 0x13;
    return 0;
}

// FUNCTION: EQGFX 0x100135E9
// Gets the center offset used to position a hierarchical sprite.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Copies sprite+0x28 (the center offset) to offset, returns 0.
int t3dGetHierarchicalSpriteCenterOffset(T3DSprite_guess *sprite, EQGfxVector3_guess *offset)
{
    *offset = sprite->centerOffset;
    return 0;
}

// FUNCTION: EQGFX 0x100135FE
// Sets the center offset used to position a hierarchical sprite.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Copies offset to sprite+0x28 (the center offset), returns 0.
int t3dSetHierarchicalSpriteCenterOffset(T3DSprite_guess *sprite, EQGfxVector3_guess *offset)
{
    sprite->centerOffset = *offset;
    return 0;
}

// FUNCTION: EQGFX 0x10013613
// __cdecl (caller pops 1 arg; ECX is not an input), void * (callers read EAX). Returns the definition at sprite+0x5C.
void *t3dGetHierarchicalSpriteDefinition(T3DSprite_guess *sprite)
{
    return sprite->definition;
}

// FUNCTION: EQGFX 0x1001361B
// Gets the collision volume type assigned to a 2D sprite.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Returns the type of the collision volume at sprite+0x14.
int t3dGet2DSpriteCollisionVolumeType(T3DSprite_guess *sprite)
{
    return sprite->collisionVolume->type;
}

// FUNCTION: EQGFX 0x10013625
// Gives a hierarchical sprite its own copy of a collision volume.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Replaces the collision volume at sprite+0x14 with a duplicate of sourceVolume and destroys the old one; returns 0, or 0xffff3d41 after a debug message when the duplicate can't be allocated.
int t3dSetHierarchicalSpriteCollisionVolume(void *context, T3DSprite_guess *sprite, T3DCollisionVolume_guess *sourceVolume)
{
    T3DCollisionVolume_guess *newVolume;

    newVolume = EQ_CDECL(T3DCollisionVolume_guess *, 0x1001D516, (void *, T3DCollisionVolume_guess *))(context, sourceVolume);
    if (newVolume == 0)
    {
        EQGFX_OUTPUT_DEBUG_STRING_A(EQ_ARRAY(char, 0x10086C68));
        return 0xffff3d41;
    }
    EQ_CDECL(void, 0x1001D5A1, (void *, T3DCollisionVolume_guess *))(context, sprite->collisionVolume);
    sprite->collisionVolume = newVolume;
    return 0;
}

// FUNCTION: EQGFX 0x10013668
// Selects the palette used to color a hierarchical sprite.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (callers read EAX). Stores the palette at sprite+0x3c, returns 0.
int t3dSetHierarchicalSpritePalette(T3DSprite_guess *sprite, void *palette)
{
    sprite->palette = palette;
    return 0;
}

// FUNCTION: EQGFX 0x10013676
// Initializes a named type-0x3B record with source data and default transforms; its game role is unclear.
void __cdecl t3dInitializeRecord_guess(T3DInitializationRecord_guess *definition, char *name, unsigned int value08,
                          void *sourceRecord, int selector, void *optionalData)
{
    unsigned int nameLength;
    void *allocatedName;
    unsigned int *sourceValues;
    unsigned int *destinationValues;
    int valueIndex;

    definition->selector = selector;
    if (selector == 0)
        definition->optionalData = 0;
    else
        definition->optionalData = optionalData;
    definition->value08 = value08;
    definition->sourceRecord = sourceRecord;
    sourceValues = (unsigned int *)((unsigned char *)sourceRecord + 0x18);
    destinationValues = definition->copiedValues;
    for (valueIndex = 8; valueIndex != 0; valueIndex = valueIndex - 1)
    {
        *destinationValues = *sourceValues;
        sourceValues = sourceValues + 1;
        destinationValues = destinationValues + 1;
    }
    definition->value18 = 0;
    definition->type = 0x3b;
    if (name == 0)
    {
        definition->name = 0;
    }
    else
    {
        nameLength = EQ_CDECL(unsigned int, 0x10068160, (char *))(name);
        allocatedName = EQ_CDECL(void *, 0x10050C48, (unsigned int))(nameLength + 1);
        definition->name = (char *)allocatedName;
        EQ_CDECL(void, 0x10068070, (void *, char *))(allocatedName, name);
    }
    definition->value0C = 0;
    definition->resetValue40 = 0;
    definition->scaleX = 1.0f;
    definition->offsetX = 0.0f;
    definition->offsetY = 0.0f;
    definition->offsetZ = 0.0f;
    definition->angleX = 0.0f;
    definition->angleY = 0.0f;
    definition->angleZ = 0.0f;
    definition->scaleY = 1.0f;
    return;
}

// FUNCTION: EQGFX 0x1001371F
// Finds a hierarchical sprite's model part by its DAG object name.
// __cdecl (callee pops 0 bytes; ECX is not an input), void * (callers read EAX). Returns the first DAG entry whose name equals name (strcmp), or NULL; only type 0x14 sprites are searched.
void *t3dGetHierarchicalSpriteDagByName(T3DSprite_guess *sprite, char *name)
{
    T3DDagObject_guess *dagEntry;
    unsigned int dagIndex;

    if (sprite->type == 0x14)
    {
        dagEntry = sprite->dagArray;
        for (dagIndex = 0; dagIndex < sprite->dagCount; dagIndex++)
        {
            if (dagEntry->name != 0 && EQ_CDECL(int, 0x10068A20, (char *, char *))(dagEntry->name, name) == 0)
                return dagEntry;
            dagEntry++;
        }
    }
    return 0;
}

#define T3D_DAG_ENTRY_SIZE 0x148

#define T3D_DAG_TRACK_OFFSET 0x14
#define T3D_TRACK_NAME_OFFSET 0x8

// FUNCTION: EQGFX 0x10013762
// Finds a hierarchical sprite part by the name of its associated animation track.
// __cdecl (callee pops 0 bytes; ECX is not an input), void * (callers read EAX). Returns the first DAG entry whose track's name equals trackName (strcmp), or NULL; a null track name is skipped, only type 0x14 sprites are searched.
void *t3dGetHierarchicalSpriteDagByTrackName(T3DSprite_guess *sprite, char *trackName)
{
    T3DDagObject_guess *dagEntry;
    unsigned int dagIndex;
    void *track;
    char *entryTrackName;

    if (sprite->type == 0x14)
    {
        dagEntry = sprite->dagArray;
        for (dagIndex = 0; dagIndex < sprite->dagCount; dagIndex++)
        {
            track = *(void **)((char *)dagEntry + T3D_DAG_TRACK_OFFSET);
            entryTrackName = *(char **)((char *)track + T3D_TRACK_NAME_OFFSET);
            if (entryTrackName != 0 && EQ_CDECL(int, 0x10068A20, (char *, char *))(entryTrackName, trackName) == 0)
                return dagEntry;
            dagEntry++;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100137A8
// Gets a model part from a hierarchical sprite by its list index.
// __cdecl (caller pops 2 args; ECX is not an input), void * (callers read EAX). Returns the DAG entry at index, NULL when index >= dagCount.
void *t3dGetHierarchicalSpriteDagByIndex(T3DSprite_guess *sprite, unsigned int index)
{
    if (index < sprite->dagCount)
        return (char *)sprite->dagArray + index * T3D_DAG_ENTRY_SIZE;
    return 0;
}

// FUNCTION: EQGFX 0x100137C2
// Gets a DAG model part's world rotation and position for rendering.
// __cdecl (callee pops 0 bytes; ECX is not an input), void. Copies dagObject's rotation (0x24 bytes at +0x5C) to rotation and its translation (+0x80) to position.
void t3dGetCurrentDagObjectToWorldTransform(T3DDagObject_guess *dagObject, void *rotation, EQGfxVector3_guess *position)
{
    EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(rotation, dagObject->rotation, sizeof(dagObject->rotation));
    position->x = dagObject->translation.x;
    *(unsigned int *)&position->y = *(unsigned int *)&dagObject->translation.y;
    *(unsigned int *)&position->z = *(unsigned int *)&dagObject->translation.z;
}

// FUNCTION: EQGFX 0x100137F9
// Reads a DAG object's frame position, orientation, and scale.
// __cdecl (callee pops 0 bytes; ECX is not an input), void. Converts dagObject's frame quaternion (+0x1c) to heading, pitch and roll, copies its frame position (+0x2c) to position and its frame scale (+0x38) to scale.
void t3dGetCurrentDagFrameTransform(T3DDagObject_guess *dagObject, float *scale, float *heading, float *pitch, float *roll, EQGfxVector3_guess *position)
{
    EQ_CDECL(void, 0x1004FBEB, (float *, float *, float *, float *))(dagObject->frameQuat, pitch, roll, heading);
    *position = dagObject->framePosition;
    *scale = dagObject->frameScale;
}

// FUNCTION: EQGFX 0x1001382D
// Sets a DAG object's frame position, orientation, and scale.
// __cdecl (callee pops 0 bytes; ECX is not an input), void. Builds the frame quaternion (+0x1c) from pitch, roll and heading (0x1004F89A), then stores scale at +0x38 and copies position to the frame position (+0x2c).
void t3dSetCurrentDagFrameTransformation(T3DDagObject_guess *dagObject, float scale, float heading, float pitch, float roll, EQGfxVector3_guess *position)
{
    EQ_CDECL(void, 0x1004F89A, (float, float, float, float *))(pitch, roll, heading, dagObject->frameQuat);
    dagObject->frameScale = scale;
    dagObject->framePosition = *position;
}

// FUNCTION: EQGFX 0x10013869
// __cdecl (caller pops 1 arg; ECX is not an input), int (callers read EAX). Returns the skin count at sprite+0x40.
int s3dGetNumSkinsAttachedToHierarchicalSprite(T3DSprite_guess *sprite)
{
    return sprite->skinCount;
}

// FUNCTION: EQGFX 0x10013871
// Sets the value stored in the sprite's skin count field.
// __cdecl (caller pops 2 args; ECX is not an input), int (EAX = 0). Stores the value at sprite+0x40 (the skin count field).
int t3dSetDagDynamic(T3DSprite_guess *sprite, int value)
{
    sprite->skinCount = value;
    return 0;
}

#define T3D_EMPTY_DAG_ADDR 0x100F401C

// FUNCTION: EQGFX 0x1001387F
// Assigns the definition DAG that supplies a sprite's model structure.
// __cdecl (caller pops 2 args; ECX is not an input), void. Unshares the sprite's old dag, then stores dag (the empty-dag global when null) and shares it; a dag tagged 0x14 takes the sprite's parent at +0x18.
void t3dSetDagSprite(T3DSprite_guess *sprite, T3DSpriteDefinition_guess *dag)
{
    if (sprite->dag != 0 && sprite->dag->type != 0)
    {
        EQ_CDECL(void, 0x10017DAD, (T3DSpriteDefinition_guess *))(sprite->dag);
        if (sprite->dag->type == 0x14)
            sprite->dag->parent = 0;
    }
    if (dag != 0)
    {
        sprite->dag = dag;
        if (dag->type != 0)
        {
            EQ_CDECL(void, 0x10017CE8, (T3DSpriteDefinition_guess *))(dag);
            if (sprite->dag->type == 0x14)
            {
                sprite->dag->parent = sprite->parent;
                return;
            }
        }
    }
    else
    {
        sprite->dag = (T3DSpriteDefinition_guess *)T3D_EMPTY_DAG_ADDR;
    }
    return;
}

#define T3D_SPRITE_TRACK_BLOCK_OFFSET 0x100

// FUNCTION: EQGFX 0x100138D3
// Applies an animation track to a sprite's DAG.
// __cdecl (caller pops 2 args; ECX is not an input), void. Does nothing when track is null; else stores track at sprite+0x14 and copies track's 8-dword block (+0x18) to sprite+0x100.
void t3dSetDagTrack(T3DSprite_guess *sprite, T3DDagTrack_guess *track)
{
    if (track != 0)
    {
        sprite->collisionVolume = (T3DCollisionVolume_guess *)track;
        *(T3DTrackBlock_guess *)((unsigned char *)sprite + T3D_SPRITE_TRACK_BLOCK_OFFSET) = track->block;
    }
    return;
}

#define T3D_SPRITE_PENDING_BLOCK_OFFSET 0x128

// FUNCTION: EQGFX 0x100138F5
// Starts a timed fade from the active DAG track to a new animation track.
// __cdecl (caller pops 4 args; ECX is not an input), int (callers read EAX). Returns 0xffff3d3e when track is null or sprite+0x14 is zero, else 0. Moves a pending track to sprite+0x14 (its block to +0x100), makes track pending (block to +0x128), calls 0x10013BBC, then stores fade's limit at +0x120 and fadeRate at +0x124.
int t3dDagFadeToNewTrack(T3DSprite_guess *sprite, T3DDagTrack_guess *track, T3DDagFade_guess *fade, int fadeRate)
{
    int result;

    result = 0;
    if (track == 0 || sprite->collisionVolume == 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        if (sprite->pendingTrack != 0)
        {
            sprite->collisionVolume = sprite->pendingTrack;
            *(T3DTrackBlock_guess *)((unsigned char *)sprite + T3D_SPRITE_TRACK_BLOCK_OFFSET) = *(T3DTrackBlock_guess *)((unsigned char *)sprite + T3D_SPRITE_PENDING_BLOCK_OFFSET);
        }
        sprite->pendingTrack = track;
        *(T3DTrackBlock_guess *)((unsigned char *)sprite + T3D_SPRITE_PENDING_BLOCK_OFFSET) = track->block;
        EQ_CDECL(void, 0x10013BBC, (T3DSprite_guess *, unsigned int))(sprite, fade->limit);
        sprite->fadeTicks = fade->limit;
        sprite->fadeRate = fadeRate;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001396D
// Checks whether a sprite's pending animation track fade has completed.
// __cdecl (caller pops 2 args; ECX is not an input), int (callers read EAX). Returns 1 when sprite+0x18 is zero, else 1 when fadeRate+fadeTicks < fade+0x8 (unsigned).
int t3dDagFadeIsComplete(T3DDagFade_guess *fade, T3DSprite_guess *sprite)
{
    if (sprite->pendingTrack == 0)
    {
        return 1;
    }
    return (unsigned int)(sprite->fadeRate + sprite->fadeTicks) < fade->limit;
}

// FUNCTION: EQGFX 0x10013993
// Reports whether a sprite has a pending animation track.
// __cdecl (caller pops 1 arg; ECX is not an input), int (callers read EAX). Returns 1 when sprite+0x18 is non-zero, else 0.
int t3dDagHasPendingTrack(T3DSprite_guess *sprite)
{
    return sprite->pendingTrack != 0;
}
