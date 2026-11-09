// unit_1000650E.c - eqgfx_dx7.dll, the address run 0x1000650E..0x10007EE5 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include <string.h>
#include "trilogy/eqaddr.h"
#define EQGFX_MEMCPY_GUESS EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
#define EQGFX_T3D_ALLOCATE_MEMORY_GUESS EQ_CDECL(void *, 0x10050C48, (unsigned int))
#define EQGFX_T3D_REALLOCATE_MEMORY_GUESS EQ_CDECL(void *, 0x10050C8C, (void *, unsigned int))
#define EQGFX_T3D_FREE_MEMORY_GUESS EQ_CDECL(void, 0x10050CBB, (void *))
#define EQGFX__T3D_DELETE_OBJECT_FROM_LIST_GUESS EQ_CDECL(int, 0x100070BC, (int, int, int))
#define EQGFX_S3D_FIND_OR_APPEND_LIST_ENTRY_GUESS EQ_CDECL(int *, 0x100072F8, (int, FUN_100072f8Owner_guess *, FUN_100072f8Item_guess *))
#define EQGFX__t3dComputeObjectToWorldCoefficients EQ_CDECL(void, 0x10035058, (void *, float, float *))
#define EQGFX_ROUND_ST0_TO_UINT_GUESS EQ_CDECL(unsigned int, 0x1004EC36, (void))
#define EQGFX_T3D_PICK_COMPUTE_VIEW_GUESS EQ_CDECL(int, 0x1004333C, (void *, void *))
#define EQGFX_S3D_PICK_COLLIDE_GUESS EQ_CDECL(int, 0x1000482E, (void *, void *, float *, float *, int, int, S3DPickFilter_guess, S3DPickFilter_guess, void *, int **))
#define EQGFX_S3D_PICK_FILTER_A_GUESS EQ_CDECL(int, 0x10006E90, (int, unsigned int))
#define EQGFX_S3D_PICK_FILTER_B_GUESS EQ_CDECL(int, 0x10006E9E, (int, unsigned int))
#define EQGFX_S3D_PICK_FILTER_COMMON_GUESS EQ_CDECL(int, 0x10006EB7, (int, unsigned int))
#define EQGFX_S3D_PICK_RENDERER_GUESS EQ_GLOBAL(S3DPickRenderer_guess *, 0x100F3FF4)
#define EQGFX_S3D_PICK_NORMALIZE_GUESS EQ_GLOBAL(S3DPickNormalizeFunction_guess, 0x100FC77C)
#define EQGFX_MOTION_HALF_GUESS EQ_GLOBAL(float, 0x10076290)
#define S3D_PICK_RESULT_TYPE_18 0x18

typedef int (__cdecl *S3DPickFilter_guess)(int, unsigned int);
typedef float (__cdecl *S3DPickNormalizeFunction_guess)(float);

typedef struct S3DPickWorld_guess
{
    unsigned char pad000[0x224];
    float screenRayTransform[3][3];
} S3DPickWorld_guess;

typedef struct S3DPickCamera_guess
{
    unsigned char pad000[0x8];
    float origin[3];
    unsigned char pad014[0xc];
    float projectionParameter;
    float verticalProjectionScale;
    unsigned char pad028[0x8];
    float rayLength;
    unsigned char pad034[0x14];
    unsigned int viewportWidth;
    unsigned int viewportHeight;
} S3DPickCamera_guess;

typedef struct S3DPickRenderer_guess
{
    unsigned char pad000[0x608c];
    float projectionScale[512][6];
} S3DPickRenderer_guess;

typedef struct S3DClipSegmentStack_guess
{
    unsigned char pad00[0x18];
    int segment_count;
    unsigned char pad1c[0x33c];
    unsigned int capacity;
    void *segments;
} S3DClipSegmentStack_guess;

typedef struct T3D2DSpriteFrameSet_guess
{
    unsigned int type;
    unsigned char pad04[0x0c];
    int referenceCount;
    unsigned int frameCount;
} T3D2DSpriteFrameSet_guess;

typedef struct T3D2DSprite_guess
{
    unsigned int type;                    // +0x0
    unsigned char pad04[0x04];
    void *entry;                           // +0x8
    union {
        struct {
            unsigned char flags;           // +0xc
            unsigned char pad0d[0x03];
        };
        unsigned int flagsAndPadding;
    };
    int useCount;                         // +0x10
    void *volume;                         // +0x14
    unsigned int unknown18;               // +0x18
    unsigned int renderMethod;
    float constantIntensity;              // +0x20
    float ambientScaler;                  // +0x24
    unsigned char pad28[0x0c];            // +0x28
    unsigned int unknown34;               // +0x34
    unsigned char pad38[0x30];
    unsigned int currentFrame;            // +0x68
    int updateInterval;                   // +0x6c
    int nextUpdateTime;                   // +0x70
    unsigned int unknown74;               // +0x74
    unsigned int centerOffset[3];         // +0x78
    unsigned int unknown84;               // +0x84
    union {                               // +0x88 (the code reads it as either)
        struct T3D2DSpriteDefinition_guess *definition;
        T3D2DSpriteFrameSet_guess *frameSet;
    };
} T3D2DSprite_guess;

typedef struct S3DDMRGBTrack_guess
{
    unsigned char pad00[0x28];            // +0x0
    float playSpeed;                      // +0x28
    unsigned char pad2C[0x4];             // +0x2C
    int updateInterval;                   // +0x30
} S3DDMRGBTrack_guess;

typedef struct FUN_100072f8List_guess
{
    unsigned char pad00[0x4];
    unsigned int count;
    unsigned int capacity;
    int *entries;
} FUN_100072f8List_guess;

typedef struct FUN_100072f8Owner_guess
{
    unsigned char pad00[0x2c];
    FUN_100072f8List_guess *list;
} FUN_100072f8Owner_guess;

typedef struct FUN_100072f8Item_guess
{
    unsigned int unknown_00;
    int id;
    unsigned char pad08[0x07];
    unsigned char flags;
    float x;
    float y;
    float z;
    unsigned char pad1c[0x0c];
    int current_region;
    unsigned char pad2c[0x08];
    float rotation;
    unsigned char pad38[0x10];
    unsigned int region_count;
    unsigned int region_capacity;
    int *regions;
    unsigned char pad54[0x38];
    float view_matrix[9];
} FUN_100072f8Item_guess;

typedef struct FUN_10007118Node_guess
{
    float normal_x;
    float normal_y;
    float normal_z;
    float distance;
    struct FUN_10007118Node_guess *positive_child;
    struct FUN_10007118Node_guess *negative_child;
    int region_tag;
} FUN_10007118Node_guess;

typedef struct FUN_10007118World_guess
{
    unsigned char pad00[0x0c];
    FUN_10007118Node_guess *root;
    unsigned char pad10[0x14];
    FUN_100072f8Owner_guess **region_owners;
    unsigned char pad28[0x10];
    unsigned int pending_capacity;
    FUN_10007118Node_guess **pending_nodes;
} FUN_10007118World_guess;


typedef struct T3D2DSpriteRenderMethodSource_guess
{
    unsigned char pad00[0x3DC];
    unsigned int renderMethodCount;
} T3D2DSpriteRenderMethodSource_guess;

typedef struct T3D2DSpriteDefinitionPitchEntry_guess
{
    float lastAngle;
    unsigned int pitchTopOrBottom;
    unsigned int numHeadingViews;
    unsigned int *headingBMInfoWords;
} T3D2DSpriteDefinitionPitchEntry_guess;

typedef struct T3D2DSpriteDefinitionPitch_guess
{
    unsigned char pad00[0x14];
    unsigned int bitmapInfoDwordCount;
    unsigned int pitchEntryCount;
    T3D2DSpriteDefinitionPitchEntry_guess *pitchEntries;
} T3D2DSpriteDefinitionPitch_guess;

typedef struct T3D2DSpriteDefinitionPayload_guess
{
    unsigned int type;
    unsigned char pad04[0x10];
    void *defaultVolume;                  // +0x14
    unsigned char pad18[0x70];
    T3D2DSpriteFrameSet_guess *frameSet;  // +0x88
} T3D2DSpriteDefinitionPayload_guess;

typedef struct T3D2DSpriteContext_guess
{
    unsigned char pad00[0x08];
    int currentTime;
} T3D2DSpriteContext_guess;

typedef struct T3D2DSpriteDefinition_guess
{
    unsigned int type;                    // +0x00
    unsigned char pad04[0x04];
    void *entry;
    unsigned char flags;
    unsigned char pad0d[0x03];
    int numShares;                        // +0x10
    void *volume;
    unsigned int pitchEntryCount;         // +0x18
    T3D2DSpriteDefinitionPitchEntry_guess *pitchEntries; // +0x1c
    unsigned char pad20[0x10];
    T3D2DSpriteDefinitionPayload_guess *payload; // +0x30
} T3D2DSpriteDefinition_guess;

typedef struct T3D2DSpriteUnshareObject_guess
{
    void *sharedObject;
} T3D2DSpriteUnshareObject_guess;

typedef struct T3D2DSpriteUnshareArray_guess
{
    unsigned int count;
    T3D2DSpriteUnshareObject_guess **objects;
} T3D2DSpriteUnshareArray_guess;

typedef struct T3D2DSpriteCleanupFrame_guess
{
    unsigned char pad00[0x08];
    int arrayCount;
    T3D2DSpriteUnshareArray_guess *arrays;
} T3D2DSpriteCleanupFrame_guess;

typedef struct T3D2DSpriteCleanupInstance_guess
{
    unsigned int type;                    // +0x00
    unsigned char pad04[0x04];
    void *entry;                          // +0x08
    union {
        struct {
            unsigned char flags;          // +0xc
            unsigned char pad0d[0x03];
        };
        unsigned int flagsAndPadding;
    };
    int state;                            // +0x10
    int objectCount;                      // +0x14
    int frameCount;                       // +0x18
    T3D2DSpriteCleanupFrame_guess *frames; // +0x1c
    int unknown20;                        // +0x20
    int unknown24;                        // +0x24
    int unknown28;                        // +0x28
    int unknown2c;                        // +0x2c
    T3D2DSpriteDefinition_guess *definition;
} T3D2DSpriteCleanupInstance_guess;

typedef struct T3DQuery_guess
{
    float origin[3];
    unsigned char pad0C[0x18];
    float direction[3];
    float distanceScale;
    unsigned char pad34[0x1C];
    float distanceLimit;
    unsigned char pad54[0x08];
    int hitTriangle;
} T3DQuery_guess;

typedef struct T3D2DSpriteRenderState_guess
{
    unsigned int dwords[0x14];            // the 0x50-byte block copied to +0x18
} T3D2DSpriteRenderState_guess;

typedef struct T3D2DCenterOffset_guess
{
    unsigned int values[3];               // +0x78
} T3D2DCenterOffset_guess;

#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define EQGFX_T3D_FLOAT_SQRT EQ_CDECL(float, 0x1004EF68, (float))
#define T3D2D_SPRITE_FRAME_OUT_OF_RANGE ((int)0xffff3d3e)
#define T3D2D_SPRITE_COPY_DWORD_COUNT 0x23
#define T3D2D_SPRITE_ALLOCATION_SIZE 0x8c

// FUNCTION: EQGFX 0x1000650e
// __cdecl. Casts the segment against the mesh triangles, keeping the closest hit.
int __cdecl s3dIntersectSegmentWithMesh_guess(float *ray, int holder)
{
    int hit;
    int triNo;
    int outer;
    int list;
    unsigned char *tri;
    unsigned char *end;
    unsigned short *group;
    unsigned int groupCount;
    int mat;
    unsigned int matFlags;
    int vertBase;
    float *v0p;
    float *v1p;
    float *v2p;
    float dx1;
    float dy1;
    float dz1;
    float dx2;
    float dy2;
    float dz2;
    float nx;
    float ny;
    float nz;
    float planeD;
    float distOrigin;
    float distEnd;
    float t;
    float hitX;
    float hitY;
    float hitZ;
    int filter;

    hit = 0;
    triNo = 0;
    outer = *(int *)(holder + 0x34);
    list = *(int *)(outer + 0x18);
    tri = *(unsigned char **)(list + 0x18);
    end = tri + *(unsigned short *)(list + 8) * 8;
    group = *(unsigned short **)(list + 0x20);
    while (tri < end)
    {
        groupCount = *group;
        mat = *(int *)(*(int *)(*(int *)(holder + 0x20) + 0x18) + group[1] * 4);
        matFlags = *(unsigned int *)(mat + 0x14);
        if ((matFlags & 0x80000000) != 0)
            matFlags = *(unsigned int *)(*(int *)(((int *)ray)[0xE] + 0x3E0) + (matFlags & 0x7FFFFFFF) * 4);
        filter = ((int *)ray)[0x10];
        if (filter == 0 || ((int (__cdecl *)(int, int, int))filter)(((int *)ray)[0x13], matFlags, ((int *)ray)[0x11]) != 0)
        {
            for (; groupCount != 0; groupCount--)
            {
                if (((int *)ray)[0x12] == 1 || (tri[0] & 0x11) == 0)
                {
                    vertBase = *(int *)(((int *)ray)[0xE] + 0x610);
                    v0p = (float *)(vertBase + *(unsigned short *)(tri + 2) * 12);
                    v1p = (float *)(vertBase + *(unsigned short *)(tri + 4) * 12);
                    v2p = (float *)(vertBase + *(unsigned short *)(tri + 6) * 12);
                    dx1 = v1p[0] - v0p[0];
                    dy1 = v1p[1] - v0p[1];
                    dz1 = v1p[2] - v0p[2];
                    dx2 = v2p[0] - v1p[0];
                    dy2 = v2p[1] - v1p[1];
                    dz2 = v2p[2] - v1p[2];
                    nx = dy2 * dz1 - dz2 * dy1;
                    ny = -(dx2 * dz1 - dz2 * dx1);
                    nz = dx2 * dy1 - dy2 * dx1;
                    planeD = -(ny * v0p[1] + nz * v0p[2] + nx * v0p[0]);
                    distOrigin = nz * ray[2] + ny * ray[1] + nx * ray[0] + planeD;
                    distEnd = nz * ray[5] + ny * ray[4] + nx * ray[3] + planeD;
                    if (((0.0f < distOrigin && distEnd < 0.0f) || (*(int *)(mat + 0x20) == 1 && distOrigin < 0.0f && 0.0f < distEnd)))
                    {
                        t = distOrigin / (distOrigin - distEnd);
                        if (t < ray[0x14])
                        {
                            hitX = t * ray[6] + ray[0];
                            hitY = t * ray[7] + ray[1];
                            hitZ = t * ray[8] + ray[2];
                            if ((0.0f < (hitX - v0p[0]) * (nz * dy1 - ny * dz1) + (hitZ - v0p[2]) * (ny * dx1 - nx * dy1) + (hitY - v0p[1]) * (-(nz * dx1 - nx * dz1)) &&
                                0.0f < (hitY - v1p[1]) * (-(nz * dx2 - nx * dz2)) + (hitX - v1p[0]) * (nz * dy2 - ny * dz2) + (hitZ - v1p[2]) * (ny * dx2 - nx * dy2)) &&
                                0.0f < (hitX - v2p[0]) * ((v0p[1] - v2p[1]) * nz - (v0p[2] - v2p[2]) * ny) + (hitY - v2p[1]) * (-((v0p[0] - v2p[0]) * nz - (v0p[2] - v2p[2]) * nx)) + (hitZ - v2p[2]) * ((v0p[0] - v2p[0]) * ny - (v0p[1] - v2p[1]) * nx))
                            {
                                hit = 1;
                                ray[0x14] = t;
                                ((int *)ray)[0x17] = triNo + 1;
                            }
                        }
                    }
                }
                tri += 8;
                triNo++;
            }
        }
        else
        {
            triNo += groupCount;
            tri += groupCount * 8;
        }
        group += 2;
    }
    return hit;
}

// FUNCTION: EQGFX 0x10006862
// Updates the nearest sphere hit in a directed world query.
int __cdecl s3dUpdateQuerySphereIntersection_guess(T3DQuery_guess *query, float *center, float radius)
{
    float dx;
    float dy;
    float dz;
    float projection;
    float discriminant;
    float candidateDistance;

    dx = query->origin[0] - center[0];
    dy = query->origin[1] - center[1];
    dz = query->origin[2] - center[2];
    projection = dx * query->direction[0] + dy * query->direction[1] + dz * query->direction[2];
    if (projection < EQGFX_ZERO_F_GUESS)
    {
        discriminant = projection * projection - ((dx * dx + dy * dy + dz * dz) - radius * radius);
        if (discriminant <= EQGFX_ZERO_F_GUESS)
            return 2;
        candidateDistance = -(EQGFX_T3D_FLOAT_SQRT(discriminant) + projection) * query->distanceScale;
        if (candidateDistance < query->distanceLimit)
        {
            if (candidateDistance < EQGFX_ZERO_F_GUESS)
                candidateDistance = EQGFX_ZERO_F_GUESS;
            query->distanceLimit = candidateDistance;
            query->hitTriangle = 0;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10006916
// __cdecl. Tests a sphere against a directed query and returns 0, 1, or 2.
int __cdecl s3dTestQuerySphereIntersection_guess(T3DQuery_guess *query, float *center, float radius)
{
    float dx;
    float dy;
    float dz;
    float discriminant;
    float projection;

    dx = query->origin[0] - center[0];
    dy = query->origin[1] - center[1];
    dz = query->origin[2] - center[2];
    discriminant = (dx * dx + dy * dy + dz * dz) - radius * radius;
    if (discriminant <= EQGFX_ZERO_F_GUESS)
    {
        return 1;
    }
    projection = dx * query->direction[0] + dy * query->direction[1] + dz * query->direction[2];
    if (projection < EQGFX_ZERO_F_GUESS)
    {
        discriminant = projection * projection - discriminant;
        if (discriminant <= EQGFX_ZERO_F_GUESS)
        {
            return 2;
        }
        if (-(EQGFX_T3D_FLOAT_SQRT(discriminant) + projection) * query->distanceScale < query->distanceLimit)
        {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100069c5
// __cdecl. Tests the segment's endpoints against the box's bounds.
int __cdecl s3dIsSegmentOutsideBoundingBox_guess(float *segmentEnds, int boxHolder)
{
    int outer;
    int box;

    outer = *(int *)(boxHolder + 0x34);
    box = *(int *)(outer + 0x14);
    if (box != 0)
    {
        if (segmentEnds[0] > *(float *)(box + 0x90))
        {
            if (segmentEnds[3] > *(float *)(box + 0x90))
                return 1;
        }
        if (segmentEnds[0] < *(float *)(box + 0x84))
        {
            if (segmentEnds[3] < *(float *)(box + 0x84))
                return 1;
        }
        if (segmentEnds[1] > *(float *)(box + 0x94))
        {
            if (segmentEnds[4] > *(float *)(box + 0x94))
                return 1;
        }
        if (segmentEnds[1] < *(float *)(box + 0x88))
        {
            if (segmentEnds[4] < *(float *)(box + 0x88))
                return 1;
        }
        if (segmentEnds[2] > *(float *)(box + 0x98))
        {
            if (segmentEnds[5] > *(float *)(box + 0x98))
                return 1;
        }
        if (segmentEnds[2] < *(float *)(box + 0x8c))
        {
            if (segmentEnds[5] < *(float *)(box + 0x8c))
                return 1;
        }
        return 0;
    }
    box = *(int *)(outer + 0x18);
    if (segmentEnds[0] > *(float *)(box + 0x68))
    {
        if (segmentEnds[3] > *(float *)(box + 0x68))
            return 1;
    }
    if (segmentEnds[0] < *(float *)(box + 0x5c))
    {
        /* fake match: this one comparison goes through a local, as the original's x87 flag handling */
        int endBelowMinX = segmentEnds[3] < *(float *)(box + 0x5c);
        if (endBelowMinX)
            return 1;
    }
    if (segmentEnds[1] > *(float *)(box + 0x6c))
    {
        if (segmentEnds[4] > *(float *)(box + 0x6c))
            return 1;
    }
    if (segmentEnds[1] < *(float *)(box + 0x60))
    {
        if (segmentEnds[4] < *(float *)(box + 0x60))
            return 1;
    }
    if (segmentEnds[2] > *(float *)(box + 0x70))
    {
        if (segmentEnds[5] > *(float *)(box + 0x70))
            return 1;
    }
    if (segmentEnds[2] < *(float *)(box + 0x64))
    {
        if (segmentEnds[5] < *(float *)(box + 0x64))
            return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10006b34
// __cdecl. Tests the segment's endpoints against the box's bounds.
int __cdecl isRayOutsideBoundingBox_guess(float *segmentEnds, int boxHolder)
{
    int boundsBase;

    boundsBase = *(int *)(boxHolder + 0x5c) + 0x20;
    if (segmentEnds[0] > *(float *)(boundsBase + 0xc))
    {
        if (segmentEnds[3] > *(float *)(boundsBase + 0xc))
            return 1;
    }
    if (segmentEnds[0] < *(float *)(boundsBase + 0x0))
    {
        if (segmentEnds[3] < *(float *)(boundsBase + 0x0))
            return 1;
    }
    if (segmentEnds[1] > *(float *)(boundsBase + 0x10))
    {
        if (segmentEnds[4] > *(float *)(boundsBase + 0x10))
            return 1;
    }
    if (segmentEnds[1] < *(float *)(boundsBase + 0x4))
    {
        if (segmentEnds[4] < *(float *)(boundsBase + 0x4))
            return 1;
    }
    if (segmentEnds[2] > *(float *)(boundsBase + 0x14))
    {
        if (segmentEnds[5] > *(float *)(boundsBase + 0x14))
            return 1;
    }
    if (segmentEnds[2] < *(float *)(boundsBase + 0x8))
    {
        if (segmentEnds[5] < *(float *)(boundsBase + 0x8))
            return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10006eb7
// Applies shared object-flag checks when picking a world object.
int __cdecl s3dPassObjectPickFilter_guess(int mode, unsigned int flags)
{
    if (mode == 0)
    {
        if ((flags & 3) != 3)
            return 0;
        flags = flags & 0x01100000;
    }
    else
    {
        if ((flags & 3) != 3)
            return 0;
        flags = flags & 0x80;
    }
    if (flags != 0)
        return 0;
    return 1;
}

// FUNCTION: EQGFX 0x10006bc9
// __cdecl. Tests the radius-expanded segment against the box's bounds.
int __cdecl s3dIsSweptSphereOutsideBoundingBox_guess(float *segmentEnds, int boxHolder)
{
    float radius = segmentEnds[15];
    int outer;
    int box;

    outer = *(int *)(boxHolder + 0x34);
    box = *(int *)(outer + 0x14);
    if (box != 0)
    {
        if (segmentEnds[2] - radius > *(float *)(box + 0x90))
        {
            if (segmentEnds[5] - radius > *(float *)(box + 0x90))
                return 1;
        }
        if (radius + segmentEnds[2] < *(float *)(box + 0x84))
        {
            if (radius + segmentEnds[5] < *(float *)(box + 0x84))
                return 1;
        }
        if (segmentEnds[3] - radius > *(float *)(box + 0x94))
        {
            if (segmentEnds[6] - radius > *(float *)(box + 0x94))
                return 1;
        }
        if (radius + segmentEnds[3] < *(float *)(box + 0x88))
        {
            if (radius + segmentEnds[6] < *(float *)(box + 0x88))
                return 1;
        }
        if (segmentEnds[4] - radius > *(float *)(box + 0x98))
        {
            if (segmentEnds[7] - radius > *(float *)(box + 0x98))
                return 1;
        }
        if (radius + segmentEnds[4] < *(float *)(box + 0x8c))
        {
            if (radius + segmentEnds[7] < *(float *)(box + 0x8c))
                return 1;
        }
        return 0;
    }
    else
    {
        box = *(int *)(outer + 0x18);
        if (segmentEnds[2] - radius > *(float *)(box + 0x68))
        {
            if (segmentEnds[5] - radius > *(float *)(box + 0x68))
                return 1;
        }
        if (radius + segmentEnds[2] < *(float *)(box + 0x5c))
        {
            if (radius + segmentEnds[5] < *(float *)(box + 0x5c))
                return 1;
        }
        if (segmentEnds[3] - radius > *(float *)(box + 0x6c))
        {
            if (segmentEnds[6] - radius > *(float *)(box + 0x6c))
                return 1;
        }
        if (radius + segmentEnds[3] < *(float *)(box + 0x60))
        {
            if (radius + segmentEnds[6] < *(float *)(box + 0x60))
                return 1;
        }
        if (segmentEnds[4] - radius > *(float *)(box + 0x70))
        {
            if (segmentEnds[7] - radius > *(float *)(box + 0x70))
                return 1;
        }
        if (radius + segmentEnds[4] < *(float *)(box + 0x64))
        {
            if (radius + segmentEnds[7] < *(float *)(box + 0x64))
                return 1;
        }
        return 0;
    }
}

// FUNCTION: EQGFX 0x10006d73
// __cdecl. Tests the radius-expanded segment against the box bounds.
/* fake match: the zero-tested combined bounds check preserves VC6's branch layout. */
int __cdecl s3dIsSweptSphereOutsideBoundsBlock_guess(float *segmentEnds, int boxHolder)
{
    int boundsBase;
    float radius = segmentEnds[15];

    boundsBase = *(int *)(boxHolder + 0x5c) + 0x20;
    if (segmentEnds[2] - radius > *(float *)(boundsBase + 0xc))
    {
        if (segmentEnds[5] - radius > *(float *)(boundsBase + 0xc))
            return 1;
    }
    if ((*(float *)boundsBase > (radius + segmentEnds[2]) &&
         radius + segmentEnds[5] < *(float *)boundsBase) == 0)
    {
        if (segmentEnds[3] - radius > *(float *)(boundsBase + 0x10))
        {
            if (segmentEnds[6] - radius > *(float *)(boundsBase + 0x10))
                return 1;
        }
        if (radius + segmentEnds[3] < *(float *)(boundsBase + 0x4))
        {
            if (radius + segmentEnds[6] < *(float *)(boundsBase + 0x4))
                return 1;
        }
        if (segmentEnds[4] - radius > *(float *)(boundsBase + 0x14))
        {
            if (segmentEnds[7] - radius > *(float *)(boundsBase + 0x14))
                return 1;
        }
        if (radius + segmentEnds[4] < *(float *)(boundsBase + 0x8))
        {
            if (radius + segmentEnds[7] < *(float *)(boundsBase + 0x8))
                return 1;
        }
    }
    else
        return 1;
    return 0;
}

// FUNCTION: EQGFX 0x10006E31
// Expands temporary segment storage used while tracing collisions through the world.
int __cdecl s3dReallocateClipSegmentStack(S3DClipSegmentStack_guess *stack)
{
    void *segments;
    int result;
    unsigned int required_capacity;

    result = 0;
    segments = stack->segments;
    required_capacity = (unsigned int)stack->segment_count + 100;
    if (segments != 0)
    {
        if (required_capacity <= stack->capacity)
            return 0;
        if (segments != 0)
        {
            segments = EQGFX_T3D_REALLOCATE_MEMORY_GUESS(segments, required_capacity * 0x18);
            goto reallocation_complete;
        }
    }
    segments = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(required_capacity * 0x18);
reallocation_complete:
    stack->segments = segments;
    if (segments == 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        stack->capacity = required_capacity;
    }
    return result;
}

// FUNCTION: EQGFX 0x10006EE8
// Picks the world object under a screen position, with an optional preference for type-18 hits.
int * __cdecl s3dPickObjectOnScreen(void *collisionContext, S3DPickWorld_guess *world,
                                    S3DPickCamera_guess *camera, void *query,
                                    unsigned int screenX, int screenY, int preferType18)
{
    float projectionParameter;
    unsigned int viewIndex;
    float viewportHalfWidth;
    float viewportHalfHeight;
    float projectionScale;
    float screenRayX;
    float screenRayY;
    float worldRayX;
    float worldRayY;
    float worldRayZ;
    float rayScale;
    float rayStart[3];
    float rayEnd[3];
    int *hitResult[5];
    int collisionResult;

    projectionParameter = camera->projectionParameter;
    viewportHalfWidth = (float)camera->viewportWidth * EQGFX_MOTION_HALF_GUESS;
    viewportHalfHeight = (float)camera->viewportHeight * EQGFX_MOTION_HALF_GUESS;
    __asm fld projectionParameter
    viewIndex = EQGFX_ROUND_ST0_TO_UINT_GUESS();
    projectionScale = EQGFX_S3D_PICK_RENDERER_GUESS->projectionScale[viewIndex & 0x1ff][0];
    screenRayX = ((float)screenX - viewportHalfWidth + EQGFX_MOTION_HALF_GUESS)
               / (projectionScale * viewportHalfWidth);
    screenRayY = (viewportHalfHeight - EQGFX_MOTION_HALF_GUESS - (float)screenY)
               / (projectionScale * camera->verticalProjectionScale * viewportHalfHeight);

    collisionResult = EQGFX_T3D_PICK_COMPUTE_VIEW_GUESS(world, camera);
    if (collisionResult < 0)
    {
        hitResult[0] = 0;
    }
    else
    {
        worldRayX = screenRayX * world->screenRayTransform[0][0]
                  + screenRayY * world->screenRayTransform[0][1]
                  + world->screenRayTransform[0][2];
        worldRayY = screenRayX * world->screenRayTransform[1][0]
                  + screenRayY * world->screenRayTransform[1][1]
                  + world->screenRayTransform[1][2];
        worldRayZ = screenRayX * world->screenRayTransform[2][0]
                  + screenRayY * world->screenRayTransform[2][1]
                  + world->screenRayTransform[2][2];
        rayScale = EQGFX_S3D_PICK_NORMALIZE_GUESS(worldRayX * worldRayX
                                                 + worldRayY * worldRayY
                                                 + worldRayZ * worldRayZ);
        worldRayX = rayScale * worldRayX;
        worldRayY = rayScale * worldRayY;
        rayStart[0] = camera->origin[0];
        rayStart[1] = camera->origin[1];
        rayStart[2] = camera->origin[2];
        rayEnd[0] = worldRayX * camera->rayLength + camera->origin[0];
        rayEnd[1] = worldRayY * camera->rayLength + camera->origin[1];
        rayEnd[2] = rayScale * worldRayZ * camera->rayLength + camera->origin[2];

        collisionResult = EQGFX_S3D_PICK_COLLIDE_GUESS(
            collisionContext, world, rayStart, rayEnd, 0, 1,
            EQGFX_S3D_PICK_FILTER_A_GUESS, EQGFX_S3D_PICK_FILTER_COMMON_GUESS,
            query, hitResult);
        if (collisionResult == 0)
        {
            if (preferType18 != 1)
                goto no_hit;
        }
        else
        {
            if (preferType18 != 1)
                return hitResult[0];
            if (*hitResult[0] == S3D_PICK_RESULT_TYPE_18)
                return hitResult[0];
        }

        collisionResult = EQGFX_S3D_PICK_COLLIDE_GUESS(
            collisionContext, world, rayStart, rayEnd, 0, 1,
            EQGFX_S3D_PICK_FILTER_B_GUESS, EQGFX_S3D_PICK_FILTER_COMMON_GUESS,
            query, hitResult);
        hitResult[0] = (int *)((-(unsigned int)(collisionResult != 0))
                             & (unsigned int)hitResult[0]);
    }
    return hitResult[0];

no_hit:
    hitResult[0] = 0;
    return hitResult[0];
}

// FUNCTION: EQGFX 0x100070bc
// Removes an actor from a world region's touched-object list.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Three stack arguments, the first unused.
// holder points at +0x2c to a list header holding a count at +0x4 and an int array at +0xc;
// item holds the id to remove at +0x4. Removes the first matching entry, shifting the tail down
// with memcpy and decrementing the count. Returns 1 when an entry was removed (or when the id
// is not present), 0xFFFFD8C0 when the list is empty.
int __cdecl _t3dDeleteObjectFromList_guess(int unused_arg, int holder, int item)
{
    int list;
    unsigned int count;
    int *entries;
    int *cursor;
    int *end;
    int target_id;
    unsigned int index;
    unsigned int new_count;

    list = *(int *)(holder + 0x2c);
    count = *(unsigned int *)(list + 4);
    if (count == 0)
        return 0xFFFFD8C0;
    entries = *(int **)(list + 0xc);
    end = entries + count;
    target_id = *(int *)(item + 4);
    index = 0;
    cursor = entries;
    while (1)
    {
        if (cursor >= end)
            return 1;
        if (*cursor != target_id)
        {
            index++;
            cursor++;
            continue;
        }
        break;
    }
    new_count = count - 1;
    *(unsigned int *)(list + 4) = new_count;
    if (index == new_count)
        return 1;
    memcpy(cursor, cursor + 1, (new_count - index) * sizeof(int));
    return 1;
}

// FUNCTION: EQGFX 0x10007118
// Updates an actor's region memberships as its sphere moves through the world.
int __cdecl s3dUpdateTouchedRegionsForSphere_guess(FUN_10007118World_guess *world, FUN_100072f8Item_guess *state, float radius, int previous_region)
{
    int result;
    unsigned int pending_capacity;
    unsigned int region_capacity;
    int current_region;
    int *region_entries;
    int *region_cursor;
    int *region_end;
    FUN_10007118Node_guess **pending_cursor;
    unsigned int pending_count;
    FUN_10007118Node_guess *node;
    float point_x;
    float point_y;
    float point_z;
    float plane_distance;
    int update_only_current;
    int region_index;

    pending_capacity = world->pending_capacity;
    region_capacity = state->region_capacity;
    region_entries = state->regions;
    current_region = state->current_region;
    result = 1;
    if (previous_region != current_region)
    {
        EQGFX__T3D_DELETE_OBJECT_FROM_LIST_GUESS((int)world, (int)world->region_owners[previous_region], (int)state);
    }
    if ((state->flags & 0x80) == 0)
    {
        region_cursor = region_entries;
        region_end = region_entries + state->region_count;
        for (; region_cursor < region_end; region_cursor++)
        {
            EQGFX__T3D_DELETE_OBJECT_FROM_LIST_GUESS((int)world, (int)world->region_owners[*region_cursor], (int)state);
        }
        state->region_count = 0;
        update_only_current = 0;
    }
    else
    {
        update_only_current = 1;
    }
    point_x = state->x;
    point_y = state->y;
    point_z = state->z;
    pending_count = 0;
    pending_cursor = world->pending_nodes;
    node = world->root;
    for (;;)
    {
        if (node == 0)
            goto done;
        do
        {
            if (node->region_tag != 0)
                break;
            plane_distance = point_z * node->normal_z + point_y * node->normal_y + point_x * node->normal_x + node->distance;
            if (plane_distance < -radius)
            {
                node = node->negative_child;
            }
            else
            {
                if (plane_distance <= radius && node->negative_child != 0)
                {
                    if (pending_capacity <= pending_count)
                    {
                        pending_capacity = pending_capacity + 1 + (pending_capacity >> 2);
                        pending_cursor = (FUN_10007118Node_guess **)EQGFX_T3D_REALLOCATE_MEMORY_GUESS(
                            world->pending_nodes, pending_capacity * 4);
                        if (pending_cursor == 0)
                        {
                            result = T3D2D_SPRITE_FRAME_OUT_OF_RANGE;
                            goto done;
                        }
                        world->pending_nodes = pending_cursor;
                        world->pending_capacity = pending_capacity;
                        pending_cursor = pending_cursor + pending_count;
                    }
                    *pending_cursor++ = node;
                    pending_count++;
                }
                node = node->positive_child;
            }
        } while (node != 0);
        if (node != 0)
        {
            region_index = node->region_tag - 1;
            if (!update_only_current || region_index == current_region)
            {
                if (EQGFX_S3D_FIND_OR_APPEND_LIST_ENTRY_GUESS((int)world, world->region_owners[region_index], state) == 0)
                {
                    result = -1;
                    goto done;
                }
                if (region_index != current_region)
                {
                    if (region_capacity <= state->region_count)
                    {
                        region_capacity = region_capacity + (region_capacity >> 2);
                        region_entries = (int *)EQGFX_T3D_REALLOCATE_MEMORY_GUESS(
                            state->regions, region_capacity * 4);
                        if (region_entries == 0)
                        {
                            result = -1;
                            goto done;
                        }
                        state->regions = region_entries;
                        state->region_capacity = region_capacity;
                    }
                    region_entries[state->region_count] = region_index;
                    state->region_count = state->region_count + 1;
                }
            }
            node = 0;
        }
        if (pending_count != 0)
        {
            pending_cursor--;
            pending_count--;
            node = pending_cursor[0]->negative_child;
        }
    }

done:
    return result;
}

// FUNCTION: EQGFX 0x100072f8
// __cdecl. Returns the matching or appended entry, or null when allocation fails.
int *__cdecl s3dFindOrAppendListEntry_guess(int unused_arg, FUN_100072f8Owner_guess *owner, FUN_100072f8Item_guess *item)
{
    int *end;
    FUN_100072f8List_guess *list;
    unsigned int capacity;
    int *cursor;
    void *new_entries;
    int new_capacity;
    unsigned int count;
    int *entries;
    int target_id;

    list = owner->list;
    entries = list->entries;
    count = list->count;
    target_id = item->id;
    end = entries + count;
    for (cursor = entries; cursor < end; cursor++)
    {
        if (*cursor == target_id)
            return cursor;
    }
    capacity = list->capacity;
    if (count >= capacity)
    {
        new_capacity = capacity + (capacity >> 2);
        new_entries = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(new_capacity * 4);
        if (new_entries == 0)
            return 0;
        EQGFX_MEMCPY_GUESS(new_entries, list->entries, list->count << 2);
        EQGFX_T3D_FREE_MEMORY_GUESS(list->entries);
        list->capacity = new_capacity;
        list->entries = (int *)new_entries;
    }
    count = list->count;
    entries = list->entries;
    cursor = entries + count;
    *cursor = item->id;
    list->count++;
    return cursor;
}

#define T3D_REGION_CANDIDATE_ALLOCATION_FAILED ((int)0xffff3d3e)
#define T3D_REGION_UPDATE_FAILED (-1)

// FUNCTION: EQGFX 0x10007379
// Moves an object's region membership to the BSP leaf regions its transformed bounding box overlaps.
int __cdecl s3dUpdateTouchedRegionsForBoundingBox(FUN_10007118World_guess *world, FUN_100072f8Item_guess *context, float *origin, float *box, int region)
{
    int result;
    unsigned int candidate_capacity;
    unsigned int region_capacity;
    FUN_10007118Node_guess **candidate_write;
    FUN_10007118Node_guess *node;
    FUN_10007118Node_guess **candidate_end;
    int *pending_regions;
    int *region_entry;
    int *region_end;
    int current_region;
    unsigned int candidate_count;
    int positive_side;
    int negative_side;
    int corner_index;
    int region_index;
    int *region_result;
    int new_capacity;
    FUN_10007118Node_guess **new_candidates;
    int *new_regions;
    float vertices[24];
    float *vertex;
    float *point;
    float x;
    float y;
    float z;

    result = 1;
    candidate_capacity = world->pending_capacity;
    pending_regions = context->regions;
    region_capacity = context->region_capacity;
    current_region = context->current_region;
    EQGFX__t3dComputeObjectToWorldCoefficients(context, context->rotation, &context->x);
    vertices[0] = box[0];
    vertices[1] = box[1];
    vertices[2] = box[2];
    vertices[3] = box[3];
    vertices[4] = box[1];
    vertices[5] = box[2];
    vertices[6] = box[0];
    vertices[7] = box[4];
    vertices[8] = box[2];
    vertices[9] = box[3];
    vertices[10] = box[4];
    vertices[11] = box[2];
    vertices[12] = box[0];
    vertices[13] = box[1];
    vertices[14] = box[5];
    vertices[15] = box[3];
    vertices[16] = box[1];
    vertices[17] = box[5];
    vertices[18] = box[0];
    vertices[19] = box[4];
    vertices[20] = box[5];
    vertices[21] = box[3];
    vertices[22] = box[4];
    vertices[23] = box[5];
    candidate_count = 0;
    vertex = vertices + 1;
    corner_index = 8;
    do
    {
        x = origin[0] + vertex[-1];
        y = *vertex + origin[1];
        z = origin[2] + vertex[1];
        vertex[-1] = y * context->view_matrix[3] + z * context->view_matrix[6] + x * context->view_matrix[0] + context->x;
        *vertex = y * context->view_matrix[4] + z * context->view_matrix[7] + x * context->view_matrix[1] + context->y;
        corner_index--;
        vertex[1] = x * context->view_matrix[2] + y * context->view_matrix[5] + z * context->view_matrix[8] + context->z;
        vertex += 3;
    } while (corner_index != 0);
    if (region != current_region)
        EQGFX__T3D_DELETE_OBJECT_FROM_LIST_GUESS((int)world, (int)world->region_owners[region], (int)context);
    region_entry = pending_regions;
    region_end = pending_regions + context->region_count;
    for (; region_entry < region_end; region_entry++)
        EQGFX__T3D_DELETE_OBJECT_FROM_LIST_GUESS((int)world, (int)world->region_owners[*region_entry], (int)context);
    context->region_count = 0;
    candidate_write = world->pending_nodes;
    node = world->root;
    do
    {
        if (node == 0)
            return result;
        do
        {
            if (node->region_tag != 0)
                break;
            corner_index = 0;
            positive_side = 0;
            negative_side = 0;
            point = vertices + 1;
            do
            {
                if (positive_side && negative_side)
                    break;
                if (point[-1] * node->normal_x + node->normal_y * *point + point[1] * node->normal_z + node->distance < 0.0f)
                    negative_side = 1;
                else
                    positive_side = 1;
                corner_index++;
                point += 3;
            } while (corner_index < 8);
            if (positive_side)
            {
                if (negative_side && node->negative_child != 0)
                {
                    if (candidate_capacity <= candidate_count)
                    {
                        new_capacity = candidate_capacity + 1 + (candidate_capacity >> 2);
                        new_candidates = (FUN_10007118Node_guess **)EQGFX_T3D_REALLOCATE_MEMORY_GUESS(world->pending_nodes, new_capacity * 4);
                        if (new_candidates == 0)
                            return T3D_REGION_CANDIDATE_ALLOCATION_FAILED;
                        world->pending_nodes = new_candidates;
                        world->pending_capacity = new_capacity;
                        candidate_capacity = new_capacity;
                        candidate_write = new_candidates + candidate_count;
                    }
                    candidate_count++;
                    *candidate_write = node;
                    candidate_write++;
                }
                node = node->positive_child;
            }
            else
                node = node->negative_child;
        } while (node != 0);
        if (node != 0)
        {
            region_index = node->region_tag - 1;
            region_result = EQGFX_S3D_FIND_OR_APPEND_LIST_ENTRY_GUESS((int)world, world->region_owners[region_index], context);
            if (region_result == 0)
                return T3D_REGION_UPDATE_FAILED;
            if (region_index != current_region)
            {
                if (region_capacity <= context->region_count)
                {
                    region_capacity = region_capacity + (region_capacity >> 2);
                    new_regions = (int *)EQGFX_T3D_REALLOCATE_MEMORY_GUESS(context->regions, region_capacity * 4);
                    if (new_regions == 0)
                        return T3D_REGION_UPDATE_FAILED;
                    context->regions = new_regions;
                    context->region_capacity = region_capacity;
                    pending_regions = new_regions;
                }
                pending_regions[context->region_count] = region_index;
                context->region_count = context->region_count + 1;
            }
            node = 0;
        }
        if (candidate_count != 0)
        {
            candidate_write--;
            candidate_count--;
            candidate_end = candidate_write;
            node = (*candidate_end)->negative_child;
        }
    } while (1);
}

// FUNCTION: EQGFX 0x10007681
// Defines a named 2D sprite from shared frame objects plus a type-10 instance holding frame timing and offset.
// __cdecl. Returns the type-9 sprite already registered under name; else builds, shares and registers one (0 on failure).
T3D2DSpriteCleanupInstance_guess *__cdecl t3dDefine2DSprite_guess(void *dictionary, char *name, int object_count, int frame_count, T3D2DSpriteCleanupFrame_guess *frames, int unknown20, int unknown24, int unknown28, T3D2DSpriteRenderState_guess *render_state, unsigned int current_frame, int update_interval, unsigned int unknown74, T3D2DCenterOffset_guess *center_offset, void *volume, int unknown2c)
{
    T3D2DSpriteCleanupInstance_guess *sprite;
    T3D2DSprite_guess *definition;
    size_t name_length;
    char *entry_name;
    int registration_result;
    int add_result;
    void *object_copy;
    unsigned int frame_end_address;
    unsigned int next_frame_address;
    T3D2DSpriteCleanupFrame_guess *frame;
    unsigned int arrays_address;
    unsigned int array_end_address;
    unsigned int next_array_address;
    T3D2DSpriteUnshareArray_guess *array;
    T3D2DSpriteUnshareObject_guess **object_end;

    sprite = (T3D2DSpriteCleanupInstance_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, char *))(dictionary, name);
    if (sprite != 0)
    {
        if (sprite->type == 9)
            return sprite;
        return 0;
    }
    sprite = (T3D2DSpriteCleanupInstance_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(sizeof(T3D2DSpriteCleanupInstance_guess));
    if (sprite == 0)
        return 0;
    sprite->objectCount = object_count;
    sprite->unknown20 = unknown20;
    sprite->flagsAndPadding = 0;
    sprite->unknown24 = unknown24;
    sprite->unknown28 = unknown28;
    sprite->type = 9;
    sprite->state = 1;
    sprite->frameCount = frame_count;
    sprite->unknown2c = unknown2c;
    name_length = EQ_CDECL(size_t, 0x10068160, (const char *))(name);
    entry_name = (char *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(name_length + 1);
    sprite->entry = entry_name;
    if (entry_name == 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
    }
    else
    {
        EQ_CDECL(void, 0x10068070, (char *, const char *))(entry_name, name);
        registration_result = EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, sprite);
        if (registration_result == 0)
        {
            add_result = EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, sprite);
            if (add_result >= 0)
            {
                frame_count = frame_count * 0x10;
                sprite->frames = frames;
                next_frame_address = frame_count + (unsigned int)frames;
                do
                {
                    do
                    {
                        frame_end_address = next_frame_address;
                        next_frame_address = frame_end_address - 0x10;
                        if (frame_end_address <= (unsigned int)frames)
                        {
                            frame_end_address = frame_count + (unsigned int)sprite->frames;
                            if ((unsigned int)sprite->frames < frame_end_address)
                            {
                                do
                                {
                                    next_frame_address = frame_end_address - 0x10;
                                    frame = (T3D2DSpriteCleanupFrame_guess *)next_frame_address;
                                    array_end_address = (unsigned int)frame->arrays + frame->arrayCount * 8;
                                    if ((unsigned int)frame->arrays < array_end_address)
                                    {
                                        do
                                        {
                                            next_array_address = array_end_address - 8;
                                            array = (T3D2DSpriteUnshareArray_guess *)next_array_address;
                                            object_end = array->objects + object_count;
                                            if (array->objects < object_end)
                                            {
                                                do
                                                {
                                                    object_end--;
                                                    EQ_CDECL(void, 0x10017CE8, (void *))(object_end[0]->sharedObject);
                                                } while (array->objects < object_end);
                                            }
                                            array_end_address = next_array_address;
                                        } while ((unsigned int)frame->arrays < next_array_address);
                                    }
                                    frame_end_address = next_frame_address;
                                } while ((unsigned int)sprite->frames < next_frame_address);
                            }
                            definition = (T3D2DSprite_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D2D_SPRITE_ALLOCATION_SIZE);
                            if (definition == 0)
                            {
                                frame_end_address = frame_count + (unsigned int)sprite->frames;
                                if ((unsigned int)sprite->frames < frame_end_address)
                                {
                                    do
                                    {
                                        next_frame_address = frame_end_address - 0x10;
                                        frame = (T3D2DSpriteCleanupFrame_guess *)next_frame_address;
                                        array_end_address = (unsigned int)frame->arrays + frame->arrayCount * 8;
                                        if ((unsigned int)frame->arrays < array_end_address)
                                        {
                                            do
                                            {
                                                next_array_address = array_end_address - 8;
                                                array = (T3D2DSpriteUnshareArray_guess *)next_array_address;
                                                object_end = array->objects + object_count;
                                                if (array->objects < object_end)
                                                {
                                                    do
                                                    {
                                                        object_end--;
                                                        EQ_CDECL(void, 0x10017DAD, (void *))(object_end[0]->sharedObject);
                                                    } while (array->objects < object_end);
                                                }
                                                EQGFX_T3D_FREE_MEMORY_GUESS(array->objects);
                                                array_end_address = next_array_address;
                                            } while ((unsigned int)frame->arrays < next_array_address);
                                        }
                                        frame_end_address = next_frame_address;
                                    } while ((unsigned int)sprite->frames < next_frame_address);
                                }
                            }
                            else
                            {
                                sprite->definition = (T3D2DSpriteDefinition_guess *)definition;
                                definition->entry = 0;
                                definition->flagsAndPadding = 0;
                                definition->type = 10;
                                definition->useCount = 1;
                                *(T3D2DSpriteRenderState_guess *)&definition->unknown18 = *render_state;
                                definition->currentFrame = current_frame;
                                definition->unknown18 = 0x37;
                                definition->updateInterval = update_interval;
                                definition->nextUpdateTime = 0;
                                definition->unknown74 = unknown74;
                                *(T3D2DCenterOffset_guess *)definition->centerOffset = *center_offset;
                                definition->unknown34 = 0;
                                definition->unknown84 = 0;
                                definition->definition = (T3D2DSpriteDefinition_guess *)sprite;
                                definition->volume = volume;
                                registration_result = EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, definition);
                                if (registration_result == 0)
                                    return sprite;
                                EQGFX_T3D_FREE_MEMORY_GUESS(definition);
                                frame_end_address = frame_count + (unsigned int)sprite->frames;
                                if ((unsigned int)sprite->frames < frame_end_address)
                                {
                                    do
                                    {
                                        next_frame_address = frame_end_address - 0x10;
                                        frame = (T3D2DSpriteCleanupFrame_guess *)next_frame_address;
                                        array_end_address = (unsigned int)frame->arrays + frame->arrayCount * 8;
                                        if ((unsigned int)frame->arrays < array_end_address)
                                        {
                                            do
                                            {
                                                next_array_address = array_end_address - 8;
                                                array = (T3D2DSpriteUnshareArray_guess *)next_array_address;
                                                object_end = array->objects + object_count;
                                                if (array->objects < object_end)
                                                {
                                                    do
                                                    {
                                                        object_end--;
                                                        EQ_CDECL(void, 0x10017DAD, (void *))(object_end[0]->sharedObject);
                                                    } while (array->objects < object_end);
                                                }
                                                EQGFX_T3D_FREE_MEMORY_GUESS(array->objects);
                                                array_end_address = next_array_address;
                                            } while ((unsigned int)frame->arrays < next_array_address);
                                        }
                                        frame_end_address = next_frame_address;
                                    } while ((unsigned int)sprite->frames < next_frame_address);
                                }
                            }
discard_sprite:
                            EQ_CDECL(void, 0x1000E334, (void *, char *))(dictionary, name);
                            EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, sprite);
                            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->entry);
                            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
                            return 0;
                        }
                        frame = (T3D2DSpriteCleanupFrame_guess *)next_frame_address;
                        arrays_address = (unsigned int)frame->arrays;
                        array_end_address = arrays_address + frame->arrayCount * 8;
                    } while (array_end_address <= arrays_address);
                    do
                    {
                        next_array_address = array_end_address - 8;
                        array = (T3D2DSpriteUnshareArray_guess *)next_array_address;
                        object_copy = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(object_count << 2);
                        if (object_copy == 0)
                        {
                            for (array = (T3D2DSpriteUnshareArray_guess *)(arrays_address + frame->arrayCount * 8 - 8);
                                 next_array_address < (unsigned int)array; array--)
                            {
                                EQGFX_T3D_FREE_MEMORY_GUESS(array->objects);
                            }
                            frame_end_address = frame_count + (unsigned int)sprite->frames;
                            while (next_frame_address < frame_end_address)
                            {
                                frame = (T3D2DSpriteCleanupFrame_guess *)(frame_end_address - 0x10);
                                arrays_address = (unsigned int)frame->arrays;
                                array_end_address = arrays_address + frame->arrayCount * 8;
                                if (arrays_address < array_end_address)
                                {
                                    do
                                    {
                                        next_array_address = array_end_address - 8;
                                        array = (T3D2DSpriteUnshareArray_guess *)next_array_address;
                                        EQGFX_T3D_FREE_MEMORY_GUESS(array->objects);
                                        array_end_address = next_array_address;
                                    } while (arrays_address < next_array_address);
                                }
                                frame_end_address = frame_end_address - 0x10;
                            }
                            goto discard_sprite;
                        }
                        EQGFX_MEMCPY_GUESS(object_copy, array->objects, object_count << 2);
                        array->objects = (T3D2DSpriteUnshareObject_guess **)object_copy;
                        array_end_address = next_array_address;
                    } while (arrays_address < next_array_address);
                } while (1);
            }
            EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, sprite);
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->entry);
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->entry);
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10007A5B
// Unregisters a 2D sprite and frees its shared definition resources after the final share.
int __cdecl t3dUndefine2DSprite(void *dictionary, T3D2DSpriteCleanupInstance_guess *sprite)
{
    int result;
    T3D2DSpriteDefinition_guess *definition;
    unsigned int frame_end_address;
    unsigned int next_frame_address;
    T3D2DSpriteCleanupFrame_guess *frame;
    unsigned int array_end_address;
    unsigned int next_array_address;
    T3D2DSpriteUnshareArray_guess *array;
    T3D2DSpriteUnshareObject_guess **object_cursor;
    T3D2DSpriteUnshareObject_guess **object_end;

    result = 0;
    definition = sprite->definition;
    if (sprite->state == 1 && definition->numShares == 1)
    {
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(dictionary, definition->volume);
        if ((definition->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, definition->entry);
        }
        EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, definition);
        if (definition->entry != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->entry);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(definition);
        if ((sprite->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, sprite->entry);
        }
        EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, sprite);
        frame_end_address = sprite->frameCount * 0x10 + (unsigned int)sprite->frames;
        if ((unsigned int)sprite->frames < frame_end_address)
        {
            do
            {
                next_frame_address = frame_end_address - 0x10;
                frame = (T3D2DSpriteCleanupFrame_guess *)next_frame_address;
                array_end_address = (unsigned int)frame->arrays + frame->arrayCount * 8;
                if ((unsigned int)frame->arrays < array_end_address)
                {
                    do
                    {
                        next_array_address = array_end_address - 8;
                        array = (T3D2DSpriteUnshareArray_guess *)next_array_address;
                        object_end = array->objects + sprite->objectCount;
                        if (array->objects < object_end)
                        {
                            do
                            {
                                object_end--;
                                EQ_CDECL(void, 0x10017DAD, (void *))(object_end[0]->sharedObject);
                            } while (array->objects < object_end);
                        }
                        EQGFX_T3D_FREE_MEMORY_GUESS(array->objects);
                        array_end_address = next_array_address;
                    } while ((unsigned int)frame->arrays < next_array_address);
                }
                EQGFX_T3D_FREE_MEMORY_GUESS(frame->arrays);
                frame_end_address = next_frame_address;
            } while ((unsigned int)sprite->frames < next_frame_address);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite->frames);
        if (sprite->entry != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->entry);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
    }
    else
    {
        result = 0xffff3d44;
    }
    return result;
}

// FUNCTION: EQGFX 0x10007B8A
// Checks that a 2D sprite definition has the expected payload type.
int __cdecl t3dIs2DSpriteDefinitionValid(T3D2DSpriteDefinition_guess *definition)
{
    int is_valid;

    is_valid = 1;
    if (definition == 0 || definition->payload == 0 || definition->type != 9 || definition->payload->type != 10)
        is_valid = 0;
    return is_valid;
}

// FUNCTION: EQGFX 0x10007B82
// __cdecl. Returns the sprite definition's share count at +0x10.
int __cdecl t3dGet2DSpriteDefinitionNumShares(T3D2DSpriteDefinition_guess *definition)
{
    return definition->numShares;
}

// FUNCTION: EQGFX 0x10007BA9
// __cdecl. Returns the RGB track update interval at +0x30.
int __cdecl s3dGetDMRGBTrackUpdateInterval(S3DDMRGBTrack_guess *track)
{
    return track->updateInterval;
}

// FUNCTION: EQGFX 0x10007BB1
// __cdecl. Returns the sprite's constant intensity float at +0x20.
float __cdecl t3dGet2DSpriteConstantIntensity(T3D2DSprite_guess *sprite)
{
    return sprite->constantIntensity;
}

// FUNCTION: EQGFX 0x10007BC9
// Gets the number of heading views for one pitch entry in a sprite definition.
unsigned int __cdecl t3dGet2DSpriteDefinitionPitchNumHeadingViews(T3D2DSpriteDefinition_guess *definition, unsigned int pitch_index)
{
    if (!(pitch_index < definition->pitchEntryCount))
        {
    }
    else
        return definition->pitchEntries[pitch_index].numHeadingViews;
    return 0;
}

// FUNCTION: EQGFX 0x10007BE4
// Gets the last pitch angle for a selected sprite pitch entry.
float __cdecl t3dGet2DSpriteDefinitionPitchLastAngle(T3D2DSpriteDefinition_guess *definition, unsigned int pitch_index)
{
    return pitch_index >= definition->pitchEntryCount
        ? 0.0f
        : definition->pitchEntries[pitch_index].lastAngle;
}

// FUNCTION: EQGFX 0x10007BFE
// Reports whether a sprite pitch entry represents a top or bottom view.
unsigned int __cdecl t3dIs2DSpriteDefinitionPitchTopOrBottom(T3D2DSpriteDefinition_guess *definition, unsigned int pitch_index)
{
    unsigned int pitch_top_or_bottom = 0;

    if (pitch_index < definition->pitchEntryCount)
        pitch_top_or_bottom = definition->pitchEntries[pitch_index].pitchTopOrBottom;

    return pitch_top_or_bottom;
}

// FUNCTION: EQGFX 0x10007BB9
// __cdecl. Returns the sprite's ambient scaler float at +0x24.
float __cdecl t3dGet2DSpriteAmbientScaler(T3D2DSprite_guess *sprite)
{
    return sprite->ambientScaler;
}

// FUNCTION: EQGFX 0x10007BC1
// __cdecl. Returns the RGB track play speed float at +0x28.
float __cdecl s3dGetDMRGBTrackPlaySpeed(S3DDMRGBTrack_guess *track)
{
    return track->playSpeed;
}

// FUNCTION: EQGFX 0x10007C18
// Gets one indexed float from a sprite heading table; its exact angle role is unclear.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Three stack arguments: the definition pointer and two
// unsigned indices. The definition stores its entry count at +0x18 and a table of 0x10-byte entries at +0x1c;
// entry [entry_index] stores its frame count at +8 and a float frame array at +0xc, stepped 8 bytes per frame.
// Returns the frame float when both indices are in range (unsigned compares, so a huge index reads nothing),
// 0.0f otherwise.
float __cdecl t3dGet2DSpriteDefinitionHeadingLastAngle_guess(int definition, unsigned int entry_index, unsigned int frame_index)
{
    if (entry_index < *(unsigned int *)(definition + 0x18))
    {
        int entry = entry_index * 0x10 + *(int *)(definition + 0x1c);

        if (frame_index < *(unsigned int *)(entry + 8))
            return *(float *)(*(int *)(entry + 0xc) + frame_index * 8);
    }
    return 0.0f;
}

// FUNCTION: EQGFX 0x10007C40
// Provides bitmap metadata for one pitch and heading view of a sprite.
int __cdecl t3dGet2DSpriteDefinitionPitchHeadingBMInfoList(
    T3D2DSpriteDefinitionPitch_guess *definition,
    unsigned int pitchEntryIndex,
    unsigned int headingIndex,
    void *destination)
{
    int result = 0;

    if (pitchEntryIndex < definition->pitchEntryCount)
    {
        T3D2DSpriteDefinitionPitchEntry_guess *pitchEntry = definition->pitchEntries + pitchEntryIndex;

        if (headingIndex < pitchEntry->numHeadingViews)
        {
            EQGFX_MEMCPY_GUESS(
                destination,
                (void *)pitchEntry->headingBMInfoWords[headingIndex * 2 + 1],
                definition->bitmapInfoDwordCount << 2);
        }
        else
            result = T3D2D_SPRITE_FRAME_OUT_OF_RANGE;
    }
    else
        result = T3D2D_SPRITE_FRAME_OUT_OF_RANGE;

    return result;
}

// FUNCTION: EQGFX 0x10007c86
// Creates a drawable 2D sprite instance from a sprite definition.
// @early-stop: failure returns clear EAX directly instead of zeroing EBX and joining the common return.
T3D2DSprite_guess *__cdecl t3dCreate2DSprite(
    T3D2DSpriteContext_guess *context,
    T3D2DSpriteDefinition_guess *definition,
    void *volume)
{
    T3D2DSprite_guess *sprite;
    void *duplicatedVolume;
    int initializeResult;

    sprite = (T3D2DSprite_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D2D_SPRITE_ALLOCATION_SIZE);
    if (sprite != 0)
    {
        *sprite = *(T3D2DSprite_guess *)definition->payload;
        sprite->entry = 0;
        sprite->flagsAndPadding = 0;
        sprite->useCount = 0;
        sprite->nextUpdateTime = sprite->updateInterval + context->currentTime;
        if (volume == 0)
            duplicatedVolume = EQ_CDECL(void *, 0x1001D516, (void *, void *))(
                context, ((T3D2DSprite_guess *)definition->payload)->volume);
        else
            duplicatedVolume = EQ_CDECL(void *, 0x1001D516, (void *, void *))(context, volume);
        sprite->volume = duplicatedVolume;
        if (duplicatedVolume == 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
            sprite = 0;
            return sprite;
        }
        initializeResult = EQ_CDECL(int, 0x1001564B, (void *, void *))(context, sprite);
        if (initializeResult != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
            sprite = 0;
            return sprite;
        }
        sprite->frameSet->referenceCount++;
    }
    return sprite;
}

// FUNCTION: EQGFX 0x10007D08
// Creates another drawable 2D sprite using the source sprite's settings.
T3D2DSprite_guess *__cdecl t3dDuplicate2DSprite(
    T3D2DSpriteContext_guess *context,
    T3D2DSprite_guess *source,
    void *volume)
{
    T3D2DSprite_guess *sprite;
    void *duplicatedVolume;
    int initializeResult;

    sprite = (T3D2DSprite_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D2D_SPRITE_ALLOCATION_SIZE);
    if (sprite != 0)
    {
        *sprite = *source;
        sprite->entry = 0;
        sprite->flagsAndPadding = 0;
        sprite->useCount = 0;
        sprite->nextUpdateTime = sprite->updateInterval + context->currentTime;
        if (volume == 0)
            volume = source->volume;
        duplicatedVolume = EQ_CDECL(void *, 0x1001D516, (void *, void *))(context, volume);
        sprite->volume = duplicatedVolume;
        if (duplicatedVolume == 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
            sprite = 0;
        }
        else
        {
            initializeResult = EQ_CDECL(int, 0x1001564B, (void *, void *))(context, sprite);
            if (initializeResult != 0)
            {
                EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, sprite->volume);
                EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
                sprite = 0;
            }
            else
            {
                sprite->frameSet->referenceCount++;
            }
        }
    }
    return sprite;
}

// FUNCTION: EQGFX 0x10007D99
// Removes a 2D sprite instance and releases its volume and frame-set reference.
int __cdecl t3dDestroy2DSprite(void *context, T3D2DSprite_guess *sprite)
{
    int result;

    if (sprite->useCount != 0)
    {
        result = (int)0xffff3d45;
    }
    else
    {
        sprite->frameSet->referenceCount--;
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, sprite->volume);
        if ((sprite->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(context, sprite->entry);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(context, sprite);
        if (sprite->entry != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->entry);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
    }

    return result;
}

// FUNCTION: EQGFX 0x10007DFF
// Checks that a live 2D sprite references the expected frame set and type.
int __cdecl t3dIs2DSpriteValid(T3D2DSprite_guess *sprite)
{
    int isValid = 1;

    if (sprite == 0 || sprite->frameSet == 0 || sprite->frameSet->type != 9 || sprite->type != 10)
        isValid = 0;

    return isValid;
}

// FUNCTION: EQGFX 0x10007E21
// Selects which animation frame a 2D sprite displays.
int __cdecl t3dSet2DSpriteCurrentFrame(T3D2DSprite_guess *sprite, unsigned int currentFrame)
{
    if (currentFrame >= sprite->frameSet->frameCount)
        return T3D2D_SPRITE_FRAME_OUT_OF_RANGE;

    sprite->currentFrame = currentFrame;
    return 0;
}

// FUNCTION: EQGFX 0x10007E63
// Gets a 2D sprite's three-coordinate center offset.
int __cdecl t3dGet2DSpriteCenterOffset(T3D2DSprite_guess *sprite, unsigned int *centerOffset)
{
    centerOffset[0] = sprite->centerOffset[0];
    centerOffset[1] = sprite->centerOffset[1];
    centerOffset[2] = sprite->centerOffset[2];
    return 0;
}

// FUNCTION: EQGFX 0x10007E78
// Sets a 2D sprite's three-coordinate center offset.
int __cdecl t3dSet2DSpriteCenterOffset(T3D2DSprite_guess *sprite, unsigned int *centerOffset)
{
    sprite->centerOffset[0] = centerOffset[0];
    sprite->centerOffset[1] = centerOffset[1];
    sprite->centerOffset[2] = centerOffset[2];
    return 0;
}

#define T3D2D_RENDER_METHOD_INDEXED_FLAG 0x80000000U
#define T3D2D_RENDER_METHOD_INDEX_MASK 0x7FFFFFFFU
#define T3D2D_RENDER_METHOD_VALID_MASK 0xFEEFFFFFU
#define T3D2D_RENDER_METHOD_OUT_OF_RANGE ((int)0xFFFF3D3E)

// FUNCTION: EQGFX 0x10007E8D
// Chooses the rendering method used to draw a 2D sprite.
int __cdecl t3dSet2DSpriteRenderMethod(T3D2DSpriteRenderMethodSource_guess *methodSource, T3D2DSprite_guess *sprite, unsigned int renderMethod)
{
    if ((renderMethod & T3D2D_RENDER_METHOD_INDEXED_FLAG) != 0 &&
        (renderMethod & T3D2D_RENDER_METHOD_INDEX_MASK) >= methodSource->renderMethodCount)
        return T3D2D_RENDER_METHOD_OUT_OF_RANGE;

    if ((renderMethod & T3D2D_RENDER_METHOD_INDEXED_FLAG) == 0 &&
        T3D2D_RENDER_METHOD_INDEX_MASK < (renderMethod & T3D2D_RENDER_METHOD_VALID_MASK))
        return T3D2D_RENDER_METHOD_OUT_OF_RANGE;

    sprite->renderMethod = renderMethod;
    return 0;
}

// FUNCTION: EQGFX 0x10007ED7
// Sets the constant intensity used to draw a 2D sprite.
int __cdecl t3dSet2DSpriteConstantIntensity(T3D2DSprite_guess *sprite, float intensity)
{
    sprite->constantIntensity = intensity;
    return 0;
}

// FUNCTION: EQGFX 0x10007EE5
// Sets the ambient-light scaling factor for a 2D sprite.
int __cdecl t3dSet2DSpriteAmbientScaler(T3D2DSprite_guess *sprite, float ambientScaler)
{
    sprite->ambientScaler = ambientScaler;
    return 0;
}
// FUNCTION: EQGFX 0x10007E42
// __cdecl. Returns the sprite's update interval at +0x6c.
int __cdecl t3dGet2DSpriteUpdateInterval(T3D2DSprite_guess *sprite)
{
    return sprite->updateInterval;
}

// FUNCTION: EQGFX 0x10007E4A
// __cdecl. Sets the sprite's update interval at +0x6c.
int __cdecl t3dSet2DSpriteUpdateInterval(T3D2DSprite_guess *sprite, int interval)
{
    sprite->updateInterval = interval;
    return 0;
}

// FUNCTION: EQGFX 0x10007E58
// __cdecl. Returns the sprite's definition pointer at +0x88.
T3D2DSpriteDefinition_guess *__cdecl t3dGet2DSpriteDefinition(T3D2DSprite_guess *sprite)
{
    return sprite->definition;
}
