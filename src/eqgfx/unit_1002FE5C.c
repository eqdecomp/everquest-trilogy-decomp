// unit_1002FE5C.c - eqgfx_dx7.dll, the address run 0x1002FE5C..0x100338B2 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include "eqgfx/layout/t3d_texinfo.h"
#include "eqgfx/layout/t3dactor.h"

#define EQGFX_SPRITE_TRANSFORM_STAMP_10032462 EQ_GLOBAL(int, 0x100FBBB0)
#define EQGFX_RENDER_VISIBILITY_MODE_GUESS EQ_GLOBAL(int, 0x100A16B8)

typedef struct EqGfxStream_10031157_guess
{
    char reserved0[8];
    unsigned int cursor;
    unsigned int available;
    unsigned int aligned;
    unsigned int remaining;
} EqGfxStream_10031157_guess;

typedef struct EqGfxClipPlane_10030daa_guess
{
    float normalX;
    float normalY;
    float normalZ;
    float distance;
    float reserved;
} EqGfxClipPlane_10030daa_guess;

typedef struct EqGfxClipVolume_10030daa_guess
{
    char reserved[0x64];
    EqGfxClipPlane_10030daa_guess planes[6];
} EqGfxClipVolume_10030daa_guess;

typedef struct EqGfxDrawMode_10032638_guess
{
    char reserved[0x14];
    int mode;
} EqGfxDrawMode_10032638_guess;

typedef struct EqGfxBatchItem_10032638_guess
{
    char reserved[0x34];
    EqGfxDrawMode_10032638_guess *drawMode;
} EqGfxBatchItem_10032638_guess;

typedef struct EqGfxCommandRecord_10032638_guess
{
    char reserved0[8];
    struct EqGfxCommandList_10032638_guess *operation;
    char reserved1[0x13c];
} EqGfxCommandRecord_10032638_guess;

typedef struct EqGfxCommandList_10032638_guess
{
    int type;
    char reserved0[0x1c];
    unsigned int commandCount;
    EqGfxCommandRecord_10032638_guess *commandRecords;
    char reserved1[0x0c];
    EqGfxDrawMode_10032638_guess *drawMode;
    char reserved2[8];
    unsigned int batchCount;
    char reserved3[4];
    EqGfxBatchItem_10032638_guess **batchItems;
    char reserved4[0x10];
    void *renderData;
} EqGfxCommandList_10032638_guess;

typedef struct EqGfxRenderContext_10032638_guess
{
    char reserved0[0x1ac];
    float sourcePosition[3];
    char reserved1[0x18c];
    float renderPosition[3];
    char reserved2[0x88];
    void *currentOperation;
    char reserved3[0x20];
    unsigned int sourceState;
    unsigned int destinationState;
} EqGfxRenderContext_10032638_guess;

#define EQGFX_OPERATION_NESTED_TYPE_10032638 0x14
#define EQGFX_OPERATION_RENDER_TYPE_10032638 0x48
#define EQGFX_OPERATION_IGNORED_TYPE_10032638 0x4d
#define EQGFX_OPERATION_STATE_TYPE_10032638 0x51
#define EQGFX_COMMAND_RECORD_STRIDE_10032638 0x148
#define EQGFX_RECURSIVE_RENDER_ERROR_10032638 (-0x2504)

typedef struct EqGfxRenderContext_10033578_guess EqGfxRenderContext_10033578_guess;
typedef struct EqGfxPointLight_10033578_guess EqGfxPointLight_10033578_guess;

typedef struct EqGfxBatch_10033578_guess
{
    char pad0[4];
    unsigned int itemCount;
    int *itemIndices;
    char pad1[4];
    float direction[3];
} EqGfxBatch_10033578_guess;

typedef struct EqGfxScratchRecord_10033578_guess
{
    unsigned int positionBits[3];
    unsigned int pad0[3];
    unsigned int normalBits[3];
    int result[3];
} EqGfxScratchRecord_10033578_guess;

typedef struct EqGfxOutputRecord_10033578_guess
{
    unsigned int positionBits[3];
    unsigned int pad0;
    unsigned int value0;
    unsigned int value1;
    unsigned int normalBits[3];
    int result[3];
} EqGfxOutputRecord_10033578_guess;

struct EqGfxPointLight_10033578_guess
{
    unsigned int type;
    char pad0[0x0c];
    struct EqGfxAnimationState_guess *animation;
    float x;
    float y;
    float z;
    float radius;
};

struct EqGfxRenderContext_10033578_guess
{
    char pad0[0x184];
    unsigned int currentFrameTime;
    char pad1[0x3c];
    EqGfxPointLight_10033578_guess *excludedPointLight;
    char pad2[0xe0];
    unsigned int pointLightCount;
    EqGfxPointLight_10033578_guess **pointLights;
    char pad3[0x14];
    EqGfxOutputRecord_10033578_guess *outputRecords;
    char pad4[4];
    EqGfxScratchRecord_10033578_guess *scratchRecords;
};

#define EQGFX_POINT_LIGHT_TYPE_10033578_GUESS 0x1e

typedef struct EqGfxAnimationSequence_guess
{
    char pad0[0x14];
    unsigned int frameCount;
    union
    {
        float singleValue;
        float *frameValues;
    } values;
    float *positions;
} EqGfxAnimationSequence_guess;

typedef struct EqGfxAnimationState_guess
{
    char pad0[0x0c];
    unsigned char flags;
    char pad1[7];
    unsigned int frameIndex;
    unsigned int framePeriod;
    unsigned int lastFrameTime;
    unsigned int playbackMode;
    EqGfxAnimationSequence_guess *sequence;
} EqGfxAnimationState_guess;

typedef struct EqGfxDirectionalEffect_guess
{
    unsigned int type;
    char pad0[0x10];
    EqGfxAnimationState_guess *animation;
    float directionX;
    float directionY;
    float directionZ;
} EqGfxDirectionalEffect_guess;

typedef struct EqGfxPointEffect_guess
{
    unsigned int type;
    char pad0[0x0c];
    EqGfxAnimationState_guess *animation;
    float x;
    float y;
    float z;
    float radius;
} EqGfxPointEffect_guess;

typedef struct EqGfxScratchRecord_guess
{
    float position[3];
    float pad[3];
    float normal[3];
    float result[3];
} EqGfxScratchRecord_guess;

typedef struct EqGfxOutputRecord_guess
{
    char pad0[0x24];
    float result[3];
    char pad1[0x18];
} EqGfxOutputRecord_guess;

typedef struct EqGfxRenderContext_guess
{
    char pad0[0x184];
    unsigned int currentFrameTime;
    char pad1[0x3c];
    EqGfxPointEffect_guess *excludedEffect;
    char pad2[0xe0];
    int effectCount;
    EqGfxPointEffect_guess **effects;
    char pad3[0x14];
    EqGfxOutputRecord_guess *outputRecords;
    char pad4[0x04];
    EqGfxScratchRecord_guess *scratchRecords;
} EqGfxRenderContext_guess;

typedef float (__cdecl *EqGfxDistanceFactor_guess_t)(float distanceSquared);
#define EQGFX_DISTANCE_FACTOR_GUESS EQ_GLOBAL(EqGfxDistanceFactor_guess_t, 0x100FC77C)
#define EQGFX_DISTANCE_THRESHOLD_GUESS EQ_GLOBAL(float, 0x100762EC)
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280)
#define EQGFX_JITTER_SCALE_GUESS EQ_GLOBAL(float, 0x100762E8)
#define EQGFX_POINT_RADIUS_SCALE_GUESS EQ_GLOBAL(float, 0x100762F0)

// Import table entry of this image (call dword ptr [va], filled by the loader).
typedef void (__stdcall *grTexDownloadMipMap_guess_t)(void *tex, void *mip, int level, void *data);
typedef void (__stdcall *grTexCombine_guess_t)(int tmu, int rgbFunc, int rgbFactor, int alphaFunc, int alphaFactor, int unk1, int unk2);
typedef void (__stdcall *grTexSource_guess_t)(void *tex, void *mip, int level, void *data);
typedef void (__stdcall *grTexMipMapMode_guess_t)(void *tex, int mode1, int mode2);
typedef unsigned int (__stdcall *grTexMinAddress_guess_t)(int tmu);
typedef unsigned int (__stdcall *grTexMaxAddress_guess_t)(int tmu);
typedef void (__stdcall *grTexClampMode_guess_t)(int tmu, int clampS, int clampT);

#define pfn_grTexDownloadMipMap_guess EQ_GLOBAL(grTexDownloadMipMap_guess_t, 0x100F3F80)
#define pfn_grTexCombine_guess EQ_GLOBAL(grTexCombine_guess_t, 0x100F3F78)
#define pfn_grTexSource_guess EQ_GLOBAL(grTexSource_guess_t, 0x100F3FB4)
#define pfn_grTexMipMapMode_guess EQ_GLOBAL(grTexMipMapMode_guess_t, 0x100F3FA4)
#define pfn_grTexMinAddress_guess EQ_GLOBAL(grTexMinAddress_guess_t, 0x100F3FA0)
#define pfn_grTexMaxAddress_guess EQ_GLOBAL(grTexMaxAddress_guess_t, 0x100F3F9C)
#define pfn_grTexClampMode_guess EQ_GLOBAL(grTexClampMode_guess_t, 0x100F3F74)

typedef struct ClipCtx_guess
{
    char pad0[0x1a0];
    float nearPlane;
    char pad1[0x4];
    float farPlane;
    char pad2[0x2bc - 0x1ac];
    int stride;
    float *distBase;
    float *vertA;
    float *vertB;
    char pad3[0x3d4 - 0x2cc];
    int currentFrame;
    struct SpriteHolder_guess_s *currentSprite;
    char pad3b[0x400 - 0x3dc];
    unsigned int clipFlags;
    char pad4[0x530 - 0x404];
    /* frustum: size not known, only its address is taken */
    char frustum[1];
} ClipCtx_guess;

typedef struct ClipVertex_guess
{
    float position[3];
    float unknown_0c[6];
    int channel[3];
} ClipVertex_guess;

#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define EQGFX_FIXED_16_16_SCALE_10030620 EQ_GLOBAL(float, 0x100762E4)
#define EQGFX_MIN_CLIP_VERTS_10030620 3
#define EQGFX_CLIP_PLANE_COUNT_10030620 6
#define EQGFX_CHANNEL_MAX_10030620 0xff
#define EQGFX_CLIP_FAILURE_RESULT_100309C6 (-0x2400)
#define EQGFX_CLIP_MIN_VERTS_100309C6 3
#define EQGFX_CLIP_PLANE_COUNT_100309C6 6
#define EQGFX_FIXED_SCALE_ADDRESS_100309C6 100762E4h
#define EQGFX_FIX16_FROM_ST0 EQ_CDECL(int, 0x1004EC36, (void))
typedef void (__stdcall *ClipDebugBreak_guess_t)(void);
#define pfn_ClipDebugBreak_10030620 EQ_GLOBAL(ClipDebugBreak_guess_t, 0x100760BC)
#define FUN_10031868_SLOT_STRIDE 0x28
#define FUN_10031868_SPLIT_RESERVE 0x10

typedef struct FUN_10030f6e_object_guess_s
{
    char unknown_000[0x1b8];
    void *renderObjectHead;
    char unknown_1bc[0x1c4 - 0x1bc];
    int initialCount;
    char unknown_1c8[0x1d0 - 0x1c8];
    void *allocation_1d0;
    int activeCount;
    unsigned int slotCapacity;
    void *allocation_1dc;
    unsigned int freeSlotHead;
    unsigned int alignment;
} FUN_10030f6e_object_guess;

typedef struct FUN_10031411_slot_guess_s
{
    unsigned int unknown_000;
    unsigned int unknown_004;
    unsigned int cursor;
    unsigned int limit;
    unsigned int alignedCursor;
    unsigned int remaining;
    int state;
    int previousIndex;
    int nextIndex;
    unsigned int nextFree;
} FUN_10031411_slot_guess;

typedef struct RenderContextResources_guess_s
{
    char unknown_000[0x434];
    int unknown_434;
    void *resource_438;
} RenderContextResources_guess;

typedef struct FUN_10030e0a_context_guess_s
{
    char unknown_000[0xc8];
    float normal_x;
    float normal_y;
    float normal_z;
    float plane_distance;
} FUN_10030e0a_context_guess;

typedef struct BoundingBox_guess_s
{
    char unknown_000[0x0e];
    unsigned char flags;
    char unknown_00f[0x20 - 0x0f];
    float boxMin[3];
    float boxMax[3];
} BoundingBox_guess;

typedef struct SpriteHolder_guess_s
{
    char unknown_000[0x28];
    float position[3];
    char unknown_034[0x38 - 0x34];
    int lastFrame;
    char unknown_03c[0x5c - 0x3c];
    BoundingBox_guess *bounds;
} SpriteHolder_guess;

typedef struct ParentTransform_guess_s
{
    char unknown_000[0x0f];
    unsigned char updateFlags;
    float position[3];
    char unknown_01c[0x34 - 0x1c];
    float radius;
    float radiusScale;
    char unknown_03c[0x68 - 0x3c];
    float matrix[9];
    char unknown_08c[0xb0 - 0x8c];
    float rotation[9];
    char unknown_0d4[0x0c];
    int frame;
    char unknown_0e4[0x10];
    int transformStamp;
} ParentTransform_guess;

typedef struct SpriteUpdateRequest_guess_s
{
    char unknown_000[0x8];
    int frame;
} SpriteUpdateRequest_guess;

typedef struct VertexLightingRequest_guess_s
{
    char unknown_000[0x4];
    int count;
    int *indices;
    char unknown_00c[0x10 - 0xc];
    float direction[3];
} VertexLightingRequest_guess;

typedef struct EqGfxRenderObject_100314bc_guess
{
    char reserved0[0x1c];
    int textureFormat;
    int textureWidth;
    int textureMipMapMode;
    int textureDepth;
    char reserved1[0x0c];
    char textureInfo[0x10];
    char reserved2[0x30];
    struct EqGfxRenderObject_100314bc_guess *next;
} EqGfxRenderObject_100314bc_guess;

typedef unsigned int (__stdcall *grTexTextureMemRequired_100314bc_guess_t)(int mipMapLevel, void *textureInfo);
#define pfn_grTexTextureMemRequired_100314bc_guess EQ_GLOBAL(grTexTextureMemRequired_100314bc_guess_t, 0x100F3FB8)
#define EQGFX_RENDER_OBJECT_SCAN_LIMIT_100314BC 10

#define t3dAllocateMemory_guess EQ_CDECL(void *, 0x10050C48, (unsigned int size))
#define t3dFreeMemory_guess EQ_CDECL(void, 0x10050CBB, (void *memory))
#define t3dReallocateMemory_guess EQ_CDECL(void *, 0x10050C8C, (void *memory, unsigned int size))

// FUNCTION: EQGFX 0x1002fe5c
// Copies dwords +0x10/+0x14 of each 0x30-byte vertex from the list at +0x2cc into the clip buffer vertA.
// __cdecl. Copies two dwords (+0x10/+0x14) of each 0x30-byte entry from the
// list at +0x2cc into the list at +0x2c4, for `count` entries. Returns 1.
int __cdecl copyClipVertexFields_guess(char *ctx, int count)
{
    unsigned int destBase;
    unsigned int srcCursor;
    unsigned int endCursor;
    unsigned int *dst;
    unsigned int first;

    destBase = *(unsigned int *)(ctx + 0x2c4);
    srcCursor = *(unsigned int *)(ctx + 0x2cc);
    endCursor = count * 0x30 + srcCursor;
    if (srcCursor < endCursor)
    {
        dst = (unsigned int *)(destBase + 0x14);
        do
        {
            first = *(unsigned int *)(srcCursor + 0x10);
            srcCursor += 0x30;
            dst[-1] = first;
            *dst = *(unsigned int *)(srcCursor - 0x1c);
            dst += 0xc;
        } while (srcCursor < endCursor);
    }
    return 1;
}

// FUNCTION: EQGFX 0x1002fe9a
// Fills clip buffer vertA: xyz from an indexed 12-byte vertex array, +0x10/+0x14 from a list of float pairs.
// __cdecl. Expands indexed 0xc-byte entries: for each of `count` indices in the
// list at +0x8 of `idxCtx` (count at +0x4), copies 3 dwords from `srcBase[idx]`
// into dwords +0x0/+0x4/+0x8 of each 0x30-byte entry at *(ctx+0x2c4), and 2
// dwords from `extra` into +0x10/+0x14. Returns 1.
int __cdecl fillClipVerticesByIndex_guess(char *ctx, char *srcBase, char *idxCtx, unsigned int *extra)
{
    unsigned int *dest;
    int *cursor;
    int *end;
    int idx;
    unsigned int *src;

    cursor = *(int **)(idxCtx + 8);
    end = cursor + *(int *)(idxCtx + 4);
    dest = *(unsigned int **)(ctx + 0x2c4);
    if (cursor < end)
    {
        do
        {
            idx = *cursor;
            cursor++;
            src = (unsigned int *)(srcBase + idx * 0xc);
            dest[0] = src[0];
            dest[1] = src[1];
            dest[2] = src[2];
            dest[4] = extra[0];
            dest[5] = extra[1];
            extra += 2;
            dest += 0xc;
        } while (cursor < end);
    }
    return 1;
}

// FUNCTION: EQGFX 0x1002fef7
// __cdecl. Clips count 0x30-byte verts against up to six planes, swapping two buffers.
int __cdecl t3dClipPolygonFrustumUV_guess(char *ctxRaw, unsigned int count, float **outVerts)
{
    ClipCtx_guess *ctx;
    float *readBase;
    float *writeBase;
    float *distRow;
    int stride;
    unsigned int flags;
    float nearVal;
    float farVal;
    float *vert;
    float *end;
    float *dist;
    float *curVert;
    float *nextVert;
    float *writePtr;
    float curDist;
    float nextDist;
    float *distNext;
    int more;
    int copyIdx;
    float frac;
    float newW;
    float *swapTmp;

    ctx = (ClipCtx_guess *)ctxRaw;
    stride = ctx->stride;
    readBase = ctx->vertA;
    if (count < 3)
    {
        *outVerts = readBase;
        return 0;
    }
    flags = ctx->clipFlags;
    writeBase = ctx->vertB;
    distRow = ctx->distBase;
    if (flags == 0)
    {
        *outVerts = readBase;
        return count;
    }
    nearVal = ctx->nearPlane;
    farVal = ctx->farPlane;
    if ((flags & 1) != 0)
    {
        vert = readBase;
        end = (float *)((char *)readBase + count * 0x30);
        dist = distRow;
        if (vert < end)
        {
            do
            {
                *dist = vert[2] + vert[1];
                dist++;
                vert = (float *)((char *)vert + 0x30);
            } while (vert < end);
        }
        curVert = readBase;
        writePtr = writeBase;
        curDist = distRow[0];
        distNext = distRow + 1;
        nextVert = (float *)((char *)curVert + 0x30);
        more = 1;
        do
        {
            if (nextVert == end)
            {
                nextDist = distRow[0];
                nextVert = readBase;
                more = 0;
            }
            else
            {
                nextDist = *distNext;
                distNext++;
            }
            if (curDist >= EQGFX_ZERO_F_GUESS)
            {
                for (copyIdx = 0; copyIdx < 12; copyIdx++)
                    writePtr[copyIdx] = curVert[copyIdx];
                writePtr += 12;
            }
            if (((*(int *)&nextDist ^ *(int *)&curDist) & 0x80000000) != 0)
            {
                frac = curDist / (curDist - nextDist);
                newW = (nextVert[2] - curVert[2]) * frac + curVert[2];
                writePtr[2] = newW;
                writePtr[1] = -newW;
                writePtr[0] = (nextVert[0] - curVert[0]) * frac + curVert[0];
                writePtr[4] = (nextVert[4] - curVert[4]) * frac + curVert[4];
                writePtr[5] = (nextVert[5] - curVert[5]) * frac + curVert[5];
                writePtr += 12;
            }
            curVert = nextVert;
            nextVert = (float *)((char *)curVert + 0x30);
            curDist = nextDist;
        } while (more != 0);
        count = (unsigned int)(writePtr - writeBase) / 12;
        swapTmp = readBase;
        readBase = writeBase;
        writeBase = swapTmp;
        if (count < 3)
        {
            *outVerts = readBase;
            return 0;
        }
    }
    distRow += stride;
    if ((flags & 2) != 0)
    {
        vert = readBase;
        end = (float *)((char *)readBase + count * 0x30);
        dist = distRow;
        if (vert < end)
        {
            do
            {
                *dist = vert[2] - vert[1];
                dist++;
                vert = (float *)((char *)vert + 0x30);
            } while (vert < end);
        }
        curVert = readBase;
        writePtr = writeBase;
        curDist = distRow[0];
        distNext = distRow + 1;
        nextVert = (float *)((char *)curVert + 0x30);
        more = 1;
        do
        {
            if (nextVert == end)
            {
                nextDist = distRow[0];
                nextVert = readBase;
                more = 0;
            }
            else
            {
                nextDist = *distNext;
                distNext++;
            }
            if (curDist >= EQGFX_ZERO_F_GUESS)
            {
                for (copyIdx = 0; copyIdx < 12; copyIdx++)
                    writePtr[copyIdx] = curVert[copyIdx];
                writePtr += 12;
            }
            if (((*(int *)&nextDist ^ *(int *)&curDist) & 0x80000000) != 0)
            {
                frac = curDist / (curDist - nextDist);
                newW = (nextVert[2] - curVert[2]) * frac + curVert[2];
                writePtr[2] = newW;
                writePtr[1] = newW;
                writePtr[0] = (nextVert[0] - curVert[0]) * frac + curVert[0];
                writePtr[4] = (nextVert[4] - curVert[4]) * frac + curVert[4];
                writePtr[5] = (nextVert[5] - curVert[5]) * frac + curVert[5];
                writePtr += 12;
            }
            curVert = nextVert;
            nextVert = (float *)((char *)curVert + 0x30);
            curDist = nextDist;
        } while (more != 0);
        count = (unsigned int)(writePtr - writeBase) / 12;
        swapTmp = readBase;
        readBase = writeBase;
        writeBase = swapTmp;
        if (count < 3)
        {
            *outVerts = readBase;
            return 0;
        }
    }
    distRow += stride;
    if ((flags & 4) != 0)
    {
        vert = readBase;
        end = (float *)((char *)readBase + count * 0x30);
        dist = distRow;
        if (vert < end)
        {
            do
            {
                *dist = vert[2] + vert[0];
                dist++;
                vert = (float *)((char *)vert + 0x30);
            } while (vert < end);
        }
        curVert = readBase;
        writePtr = writeBase;
        curDist = distRow[0];
        distNext = distRow + 1;
        nextVert = (float *)((char *)curVert + 0x30);
        more = 1;
        do
        {
            if (nextVert == end)
            {
                nextDist = distRow[0];
                nextVert = readBase;
                more = 0;
            }
            else
            {
                nextDist = *distNext;
                distNext++;
            }
            if (curDist >= EQGFX_ZERO_F_GUESS)
            {
                for (copyIdx = 0; copyIdx < 12; copyIdx++)
                    writePtr[copyIdx] = curVert[copyIdx];
                writePtr += 12;
            }
            if (((*(int *)&nextDist ^ *(int *)&curDist) & 0x80000000) != 0)
            {
                frac = curDist / (curDist - nextDist);
                newW = (nextVert[2] - curVert[2]) * frac + curVert[2];
                writePtr[2] = newW;
                writePtr[0] = -newW;
                writePtr[1] = (nextVert[1] - curVert[1]) * frac + curVert[1];
                writePtr[4] = (nextVert[4] - curVert[4]) * frac + curVert[4];
                writePtr[5] = (nextVert[5] - curVert[5]) * frac + curVert[5];
                writePtr += 12;
            }
            curVert = nextVert;
            nextVert = (float *)((char *)curVert + 0x30);
            curDist = nextDist;
        } while (more != 0);
        count = (unsigned int)(writePtr - writeBase) / 12;
        swapTmp = readBase;
        readBase = writeBase;
        writeBase = swapTmp;
        if (count < 3)
        {
            *outVerts = readBase;
            return 0;
        }
    }
    distRow += stride;
    if ((flags & 8) != 0)
    {
        vert = readBase;
        end = (float *)((char *)readBase + count * 0x30);
        dist = distRow;
        if (vert < end)
        {
            do
            {
                *dist = vert[2] - vert[0];
                dist++;
                vert = (float *)((char *)vert + 0x30);
            } while (vert < end);
        }
        curVert = readBase;
        writePtr = writeBase;
        curDist = distRow[0];
        distNext = distRow + 1;
        nextVert = (float *)((char *)curVert + 0x30);
        more = 1;
        do
        {
            if (nextVert == end)
            {
                nextDist = distRow[0];
                nextVert = readBase;
                more = 0;
            }
            else
            {
                nextDist = *distNext;
                distNext++;
            }
            if (curDist >= EQGFX_ZERO_F_GUESS)
            {
                for (copyIdx = 0; copyIdx < 12; copyIdx++)
                    writePtr[copyIdx] = curVert[copyIdx];
                writePtr += 12;
            }
            if (((*(int *)&nextDist ^ *(int *)&curDist) & 0x80000000) != 0)
            {
                frac = curDist / (curDist - nextDist);
                newW = (nextVert[2] - curVert[2]) * frac + curVert[2];
                writePtr[2] = newW;
                writePtr[0] = newW;
                writePtr[1] = (nextVert[1] - curVert[1]) * frac + curVert[1];
                writePtr[4] = (nextVert[4] - curVert[4]) * frac + curVert[4];
                writePtr[5] = (nextVert[5] - curVert[5]) * frac + curVert[5];
                writePtr += 12;
            }
            curVert = nextVert;
            nextVert = (float *)((char *)curVert + 0x30);
            curDist = nextDist;
        } while (more != 0);
        count = (unsigned int)(writePtr - writeBase) / 12;
        swapTmp = readBase;
        readBase = writeBase;
        writeBase = swapTmp;
        if (count < 3)
        {
            *outVerts = readBase;
            return 0;
        }
    }
    distRow += stride;
    if ((flags & 0x10) != 0)
    {
        vert = readBase;
        end = (float *)((char *)readBase + count * 0x30);
        dist = distRow;
        if (vert < end)
        {
            do
            {
                *dist = vert[2] - nearVal;
                dist++;
                vert = (float *)((char *)vert + 0x30);
            } while (vert < end);
        }
        curVert = readBase;
        writePtr = writeBase;
        curDist = distRow[0];
        distNext = distRow + 1;
        nextVert = (float *)((char *)curVert + 0x30);
        more = 1;
        do
        {
            if (nextVert == end)
            {
                nextDist = distRow[0];
                nextVert = readBase;
                more = 0;
            }
            else
            {
                nextDist = *distNext;
                distNext++;
            }
            if (curDist >= EQGFX_ZERO_F_GUESS)
            {
                for (copyIdx = 0; copyIdx < 12; copyIdx++)
                    writePtr[copyIdx] = curVert[copyIdx];
                writePtr += 12;
            }
            if (((*(int *)&nextDist ^ *(int *)&curDist) & 0x80000000) != 0)
            {
                frac = curDist / (curDist - nextDist);
                writePtr[2] = nearVal;
                writePtr[0] = (nextVert[0] - curVert[0]) * frac + curVert[0];
                writePtr[1] = (nextVert[1] - curVert[1]) * frac + curVert[1];
                writePtr[4] = (nextVert[4] - curVert[4]) * frac + curVert[4];
                writePtr[5] = (nextVert[5] - curVert[5]) * frac + curVert[5];
                writePtr += 12;
            }
            curVert = nextVert;
            nextVert = (float *)((char *)curVert + 0x30);
            curDist = nextDist;
        } while (more != 0);
        count = (unsigned int)(writePtr - writeBase) / 12;
        swapTmp = readBase;
        readBase = writeBase;
        writeBase = swapTmp;
        if (count < 3)
        {
            *outVerts = readBase;
            return 0;
        }
    }
    distRow += stride;
    if ((flags & 0x20) != 0)
    {
        vert = readBase;
        end = (float *)((char *)readBase + count * 0x30);
        dist = distRow;
        if (vert < end)
        {
            do
            {
                *dist = farVal - vert[2];
                dist++;
                vert = (float *)((char *)vert + 0x30);
            } while (vert < end);
        }
        curVert = readBase;
        writePtr = writeBase;
        curDist = distRow[0];
        distNext = distRow + 1;
        nextVert = (float *)((char *)curVert + 0x30);
        more = 1;
        do
        {
            if (nextVert == end)
            {
                nextDist = distRow[0];
                nextVert = readBase;
                more = 0;
            }
            else
            {
                nextDist = *distNext;
                distNext++;
            }
            if (curDist >= EQGFX_ZERO_F_GUESS)
            {
                for (copyIdx = 0; copyIdx < 12; copyIdx++)
                    writePtr[copyIdx] = curVert[copyIdx];
                writePtr += 12;
            }
            if (((*(int *)&nextDist ^ *(int *)&curDist) & 0x80000000) != 0)
            {
                frac = curDist / (curDist - nextDist);
                writePtr[2] = farVal;
                writePtr[0] = (nextVert[0] - curVert[0]) * frac + curVert[0];
                writePtr[1] = (nextVert[1] - curVert[1]) * frac + curVert[1];
                writePtr[4] = (nextVert[4] - curVert[4]) * frac + curVert[4];
                writePtr[5] = (nextVert[5] - curVert[5]) * frac + curVert[5];
                writePtr += 12;
            }
            curVert = nextVert;
            nextVert = (float *)((char *)curVert + 0x30);
            curDist = nextDist;
        } while (more != 0);
        count = (unsigned int)(writePtr - writeBase) / 12;
        swapTmp = readBase;
        readBase = writeBase;
        writeBase = swapTmp;
        if (count < 3)
        {
            *outVerts = readBase;
            return 0;
        }
    }
    *outVerts = readBase;
    return count;
}

// FUNCTION: EQGFX 0x10030620
// Frustum-clips a polygon (4 sides, near, far), interpolating position and colour only; clamps colour to 255.
// __cdecl. Clips the vertices at vertA against the planes set in clipFlags (bits 0-5, ping-pong with vertB). Returns the count left.
unsigned int __cdecl t3dClipPolygonFrustumRGB_guess(ClipCtx_guess *clip, unsigned int count, ClipVertex_guess **outVerts)
{
    int stride;
    unsigned int planeMask;
    unsigned int planeIndex;
    float nearPlane;
    float farPlane;
    float *distRow;
    float *distNext;
    float *dist;
    ClipVertex_guess *readBase;
    ClipVertex_guess *writeBase;
    ClipVertex_guess *end;
    ClipVertex_guess *vert;
    ClipVertex_guess *writePtr;
    ClipVertex_guess *curVert;
    ClipVertex_guess *nextVert;
    ClipVertex_guess *swapTmp;
    float curDist;
    float nextDist;
    float frac;
    int fixedFrac;
    int more;

    stride = clip->stride;
    if (count < EQGFX_MIN_CLIP_VERTS_10030620)
    {
        goto emptyInput;
    }
    planeMask = clip->clipFlags;
    readBase = (ClipVertex_guess *)clip->vertA;
    if (planeMask != 0)
    {
        nearPlane = clip->nearPlane;
        planeIndex = 0;
        farPlane = clip->farPlane;
        writeBase = (ClipVertex_guess *)clip->vertB;
        distRow = clip->distBase;
        do
        {
            if ((planeMask & 1) != 0)
            {
                end = readBase + count;
                switch (planeIndex)
                {
                case 0:
                    dist = distRow;
                    for (vert = readBase; vert < end; vert++)
                    {
                        *dist = vert->position[2] + vert->position[1];
                        dist++;
                    }
                    break;
                case 1:
                    dist = distRow;
                    for (vert = readBase; vert < end; vert++)
                    {
                        *dist = vert->position[2] - vert->position[1];
                        dist++;
                    }
                    break;
                case 2:
                    dist = distRow;
                    for (vert = readBase; vert < end; vert++)
                    {
                        *dist = vert->position[2] + vert->position[0];
                        dist++;
                    }
                    break;
                case 3:
                    dist = distRow;
                    for (vert = readBase; vert < end; vert++)
                    {
                        *dist = vert->position[2] - vert->position[0];
                        dist++;
                    }
                    break;
                case 4:
                    dist = distRow;
                    for (vert = readBase; vert < end; vert++)
                    {
                        *dist = vert->position[2] - nearPlane;
                        dist++;
                    }
                    break;
                case 5:
                    dist = distRow;
                    for (vert = readBase; vert < end; vert++)
                    {
                        *dist = farPlane - vert->position[2];
                        dist++;
                    }
                    break;
                default:
                    pfn_ClipDebugBreak_10030620();
                    break;
                }
                curVert = readBase;
                writePtr = writeBase;
                more = 1;
                curDist = *distRow;
                distNext = distRow + 1;
                do
                {
                    nextVert = curVert + 1;
                    if (nextVert == end)
                    {
                        nextDist = *distRow;
                        nextVert = readBase;
                        more = 0;
                    }
                    else
                    {
                        nextDist = *distNext;
                        distNext++;
                    }
                    if (curDist >= EQGFX_ZERO_F_GUESS)
                    {
                        *writePtr = *curVert;
                        writePtr++;
                    }
                    if (((*(int *)&nextDist ^ *(int *)&curDist) & 0x80000000) != 0)
                    {
                        frac = curDist / (curDist - nextDist);
                        fixedFrac = (int)(frac * EQGFX_FIXED_16_16_SCALE_10030620);
                        switch (planeIndex)
                        {
                        case 0:
                            writePtr->position[2] = (nextVert->position[2] - curVert->position[2]) * frac + curVert->position[2];
                            writePtr->position[1] = -writePtr->position[2];
                            writePtr->position[0] = (nextVert->position[0] - curVert->position[0]) * frac + curVert->position[0];
                            break;
                        case 1:
                            writePtr->position[2] = (nextVert->position[2] - curVert->position[2]) * frac + curVert->position[2];
                            writePtr->position[1] = writePtr->position[2];
                            writePtr->position[0] = (nextVert->position[0] - curVert->position[0]) * frac + curVert->position[0];
                            break;
                        case 2:
                            writePtr->position[2] = (nextVert->position[2] - curVert->position[2]) * frac + curVert->position[2];
                            writePtr->position[0] = -writePtr->position[2];
                            writePtr->position[1] = (nextVert->position[1] - curVert->position[1]) * frac + curVert->position[1];
                            break;
                        case 3:
                            writePtr->position[2] = (nextVert->position[2] - curVert->position[2]) * frac + curVert->position[2];
                            writePtr->position[0] = writePtr->position[2];
                            writePtr->position[1] = (nextVert->position[1] - curVert->position[1]) * frac + curVert->position[1];
                            break;
                        case 4:
                            writePtr->position[2] = nearPlane;
                            writePtr->position[0] = (nextVert->position[0] - curVert->position[0]) * frac + curVert->position[0];
                            writePtr->position[1] = (nextVert->position[1] - curVert->position[1]) * frac + curVert->position[1];
                            break;
                        case 5:
                            writePtr->position[2] = farPlane;
                            writePtr->position[0] = (nextVert->position[0] - curVert->position[0]) * frac + curVert->position[0];
                            writePtr->position[1] = (nextVert->position[1] - curVert->position[1]) * frac + curVert->position[1];
                            break;
                        default:
                            pfn_ClipDebugBreak_10030620();
                            break;
                        }
                        writePtr->channel[0] = (((nextVert->channel[0] - curVert->channel[0]) * fixedFrac) >> 16) + curVert->channel[0];
                        writePtr->channel[1] = (((nextVert->channel[1] - curVert->channel[1]) * fixedFrac) >> 16) + curVert->channel[1];
                        writePtr->channel[2] = (((nextVert->channel[2] - curVert->channel[2]) * fixedFrac) >> 16) + curVert->channel[2];
                        writePtr++;
                    }
                    curVert = nextVert;
                    curDist = nextDist;
                } while (more != 0);
                count = (unsigned int)(writePtr - writeBase);
                swapTmp = readBase;
                readBase = writeBase;
                writeBase = swapTmp;
                if (count < EQGFX_MIN_CLIP_VERTS_10030620)
                {
                    goto emptyOutput;
                }
            }
            planeMask >>= 1;
            planeIndex++;
            distRow += stride;
        } while (planeIndex < EQGFX_CLIP_PLANE_COUNT_10030620);
    }
    goto clamp;
emptyInput:
    readBase = (ClipVertex_guess *)clip->vertA;
emptyOutput:
    count = 0;
clamp:
    end = readBase + count;
    if (readBase < end)
    {
        vert = readBase;
        do
        {
            if (vert->channel[0] > EQGFX_CHANNEL_MAX_10030620)
            {
                vert->channel[0] = EQGFX_CHANNEL_MAX_10030620;
            }
            if (vert->channel[1] > EQGFX_CHANNEL_MAX_10030620)
            {
                vert->channel[1] = EQGFX_CHANNEL_MAX_10030620;
            }
            if (vert->channel[2] > EQGFX_CHANNEL_MAX_10030620)
            {
                vert->channel[2] = EQGFX_CHANNEL_MAX_10030620;
            }
            vert++;
        } while (vert < end);
    }
    *outVerts = readBase;
    return count;
}

// FUNCTION: EQGFX 0x100309c6
// Frustum-clips a polygon (4 sides, near, far), interpolating position, colour and the floats at +0x10/+0x14.
unsigned int __cdecl t3dClipPolygonFrustumRGBUV_guess(ClipCtx_guess *clip, unsigned int count, ClipVertex_guess **outVerts)
{
    unsigned int planeMask;
    unsigned int planeIndex;
    unsigned int distanceStride;
    int fixedFraction;
    int more;
    float nearPlane;
    float farPlane;
    float currentDistance;
    float nextDistanceValue;
    float fraction;
    float *distanceRow;
    float *nextDistance;
    float *distanceOut;
    ClipVertex_guess *readBase;
    ClipVertex_guess *writeBase;
    ClipVertex_guess *end;
    ClipVertex_guess *vertex;
    ClipVertex_guess *current;
    ClipVertex_guess *next;
    ClipVertex_guess *writePtr;
    ClipVertex_guess *swap;

    distanceStride = (unsigned int)clip->stride;
    if (count < EQGFX_CLIP_MIN_VERTS_100309C6)
    {
        readBase = (ClipVertex_guess *)clip->vertA;
        goto emptyOutput;
    }
    if (distanceStride < count + EQGFX_CLIP_PLANE_COUNT_100309C6)
    {
        count = EQGFX_CLIP_FAILURE_RESULT_100309C6;
        goto clamp;
    }

    planeMask = clip->clipFlags;
    readBase = (ClipVertex_guess *)clip->vertA;
    writeBase = (ClipVertex_guess *)clip->vertB;
    if (planeMask != 0)
    {
        nearPlane = clip->nearPlane;
        farPlane = clip->farPlane;
        distanceRow = clip->distBase;
        planeIndex = 0;
        do
        {
            if ((planeMask & 1) != 0)
            {
                end = readBase + count;
                distanceOut = distanceRow;
                switch (planeIndex)
                {
                case 0:
                    if (readBase < end)
                    {
                        vertex = readBase;
                        do
                        {
                            *distanceOut = vertex->position[2] + vertex->position[1];
                            distanceOut++;
                            vertex++;
                        } while (vertex < end);
                    }
                    break;
                case 1:
                    if (readBase < end)
                    {
                        vertex = readBase;
                        do
                        {
                            *distanceOut = vertex->position[2] - vertex->position[1];
                            distanceOut++;
                            vertex++;
                        } while (vertex < end);
                    }
                    break;
                case 2:
                    for (vertex = readBase; vertex < end; vertex++)
                    {
                        *distanceOut = vertex->position[2] + vertex->position[0];
                        distanceOut++;
                    }
                    break;
                case 3:
                    for (vertex = readBase; vertex < end; vertex++)
                    {
                        *distanceOut = vertex->position[2] - vertex->position[0];
                        distanceOut++;
                    }
                    break;
                case 4:
                    for (vertex = readBase; vertex < end; vertex++)
                    {
                        *distanceOut = vertex->position[2] - nearPlane;
                        distanceOut++;
                    }
                    break;
                case 5:
                    for (vertex = readBase; vertex < end; vertex++)
                    {
                        *distanceOut = farPlane - vertex->position[2];
                        distanceOut++;
                    }
                    break;
                default:
                    pfn_ClipDebugBreak_10030620();
                    break;
                }

                writePtr = writeBase;
                currentDistance = *distanceRow;
                nextDistance = distanceRow + 1;
                current = readBase;
                more = 1;
                do
                {
                    if (current + 1 == end)
                    {
                        nextDistanceValue = *distanceRow;
                        more = 0;
                        next = readBase;
                    }
                    else
                    {
                        nextDistanceValue = *nextDistance;
                        next = current + 1;
                        nextDistance++;
                    }

                    if (EQGFX_ZERO_F_GUESS <= currentDistance)
                    {
                        *writePtr = *current;
                        writePtr++;
                    }
                    if ((((*(unsigned int *)&nextDistanceValue ^ *(unsigned int *)&currentDistance) & 0x80000000) != 0))
                    {
                        __asm
                        {
                            mov eax, EQGFX_FIXED_SCALE_ADDRESS_100309C6
                            fld currentDistance
                            fsub nextDistanceValue
                            fdivr currentDistance
                            fst fraction
                            fld st(0)
                            fmul dword ptr [eax]
                        }
                        fixedFraction = EQGFX_FIX16_FROM_ST0();

                        switch (planeIndex)
                        {
                        case 0:
                            __asm
                            {
                                mov eax, next
                                mov ecx, current
                                mov edx, writePtr
                                fld dword ptr [eax+8]
                                fsub dword ptr [ecx+8]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+8]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+8]
                                fld dword ptr [edx+8]
                                fchs
                                fstp dword ptr [edx+4]
                                fld dword ptr [eax]
                                fsub dword ptr [ecx]
                                fmul st(0), st(1)
                                fld dword ptr [ecx]
                                faddp st(1), st(0)
                                fstp dword ptr [edx]
                            }
                            break;
                        case 1:
                            __asm
                            {
                                mov eax, next
                                mov ecx, current
                                mov edx, writePtr
                                fld dword ptr [eax+8]
                                fsub dword ptr [ecx+8]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+8]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+8]
                                mov eax, dword ptr [edx+8]
                                mov dword ptr [edx+4], eax
                            }
                            __asm
                            {
                                mov eax, next
                                mov ecx, current
                                mov edx, writePtr
                                fld dword ptr [eax]
                                fsub dword ptr [ecx]
                                fmul st(0), st(1)
                                fld dword ptr [ecx]
                                faddp st(1), st(0)
                                fstp dword ptr [edx]
                            }
                            break;
                        case 2:
                            __asm
                            {
                                mov eax, next
                                mov ecx, current
                                mov edx, writePtr
                                fld dword ptr [eax+8]
                                fsub dword ptr [ecx+8]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+8]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+8]
                                fld dword ptr [edx+8]
                                fchs
                                fstp dword ptr [edx]
                                fld dword ptr [eax+4]
                                fsub dword ptr [ecx+4]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+4]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+4]
                            }
                            break;
                        case 3:
                            __asm
                            {
                                mov eax, next
                                mov ecx, current
                                mov edx, writePtr
                                fld dword ptr [eax+8]
                                fsub dword ptr [ecx+8]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+8]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+8]
                                mov eax, dword ptr [edx+8]
                                mov dword ptr [edx], eax
                                mov eax, next
                                fld dword ptr [eax+4]
                                fsub dword ptr [ecx+4]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+4]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+4]
                            }
                            break;
                        case 4:
                            writePtr->position[2] = nearPlane;
                            __asm
                            {
                                mov eax, next
                                mov ecx, current
                                mov edx, writePtr
                                fld dword ptr [eax]
                                fsub dword ptr [ecx]
                                fmul st(0), st(1)
                                fld dword ptr [ecx]
                                faddp st(1), st(0)
                                fstp dword ptr [edx]
                                fld dword ptr [eax+4]
                                fsub dword ptr [ecx+4]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+4]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+4]
                            }
                            break;
                        case 5:
                            writePtr->position[2] = farPlane;
                            __asm
                            {
                                mov eax, next
                                mov ecx, current
                                mov edx, writePtr
                                fld dword ptr [eax]
                                fsub dword ptr [ecx]
                                fmul st(0), st(1)
                                fld dword ptr [ecx]
                                faddp st(1), st(0)
                                fstp dword ptr [edx]
                                fld dword ptr [eax+4]
                                fsub dword ptr [ecx+4]
                                fmul st(0), st(1)
                                fld dword ptr [ecx+4]
                                faddp st(1), st(0)
                                fstp dword ptr [edx+4]
                            }
                            break;
                        default:
                            pfn_ClipDebugBreak_10030620();
                            fraction = currentDistance / (currentDistance - nextDistanceValue);
                            break;
                        }
                        writePtr->channel[0] = ((next->channel[0] - current->channel[0]) * fixedFraction >> 16) + current->channel[0];
                        writePtr->channel[1] = ((next->channel[1] - current->channel[1]) * fixedFraction >> 16) + current->channel[1];
                        writePtr->channel[2] = ((next->channel[2] - current->channel[2]) * fixedFraction >> 16) + current->channel[2];
                        __asm
                        {
                            mov eax, next
                            mov ecx, current
                            mov edx, writePtr
                            fld dword ptr [eax+10h]
                            fsub dword ptr [ecx+10h]
                            fmul st(0), st(1)
                            fld dword ptr [ecx+10h]
                            faddp st(1), st(0)
                            fstp dword ptr [edx+10h]
                            fld dword ptr [eax+14h]
                            fsub dword ptr [ecx+14h]
                            fmul st(0), st(1)
                            fld dword ptr [ecx+14h]
                            faddp st(1), st(0)
                            fstp dword ptr [edx+14h]
                            fstp st(0)
                        }
                        writePtr++;
                    }
                    currentDistance = nextDistanceValue;
                    current = next;
                } while (more != 0);

                count = (unsigned int)(writePtr - writeBase);
                swap = readBase;
                readBase = writeBase;
                writeBase = swap;
                if (count < EQGFX_CLIP_MIN_VERTS_100309C6)
                {
                    goto emptyOutput;
                }
            }
            planeMask >>= 1;
            planeIndex++;
            distanceRow += distanceStride;
        } while (planeIndex < EQGFX_CLIP_PLANE_COUNT_100309C6);
    }
    goto clamp;

emptyOutput:
    count = 0;
clamp:
    end = readBase + count;
    if (readBase < end)
    {
        vertex = readBase;
        do
        {
            if (vertex->channel[0] > EQGFX_CHANNEL_MAX_10030620)
            {
                vertex->channel[0] = EQGFX_CHANNEL_MAX_10030620;
            }
            if (vertex->channel[1] > EQGFX_CHANNEL_MAX_10030620)
            {
                vertex->channel[1] = EQGFX_CHANNEL_MAX_10030620;
            }
            if (vertex->channel[2] > EQGFX_CHANNEL_MAX_10030620)
            {
                vertex->channel[2] = EQGFX_CHANNEL_MAX_10030620;
            }
            vertex++;
        } while (vertex < end);
    }
    *outVerts = readBase;
    return count;
}

// FUNCTION: EQGFX 0x10030daa
// Rejects objects whose bounding spheres fall outside the view frustum.
// @early-stop: the outside-return block is laid out before the shared epilogue.
// __cdecl. Tests the sphere at `pos` with `radius` against the six planes
// at `ctx+0x68`, returning the mask of planes it touches or 0xffffffff if outside.
unsigned int __cdecl _t3dClipWorldSpaceSpherical(EqGfxClipVolume_10030daa_guess *ctx, float *pos, float radius)
{
    unsigned int mask;
    unsigned int bit;
    unsigned int index;
    float *plane;
    float dist;
    float negativeRadius;

    mask = 0;
    bit = 1;
    index = 0;
    negativeRadius = -radius;
    plane = &ctx->planes[0].normalY;
    do
    {
        dist = plane[-1] * pos[0] + plane[1] * pos[2] + pos[1] * plane[0] - plane[2];
        if (dist >= negativeRadius)
        {
            if (dist < radius)
                mask |= bit;
            bit <<= 1;
            index++;
            plane += 5;
        }
        else
        {
            return 0xffffffff;
        }
    } while (index < 6);
    return mask;
}

// FUNCTION: EQGFX 0x10030e0a
// __cdecl. Returns -1 when the point is outside the plane by more than `radius`.
int __cdecl isSphereOutsidePlane_guess(FUN_10030e0a_context_guess *context, float *position, float radius)
{
    if (context->normal_x * position[0] + context->normal_y * position[1] + context->normal_z * position[2] - context->plane_distance < -radius)
        return 0xffffffff;
    return 0;
}

// FUNCTION: EQGFX 0x10030e4a
// Rejects objects whose bounding boxes fall outside the view frustum.
// __cdecl. Tests the box at `boxMin`/`boxMax` against the six planes
// at `ctx+0x64`, returning the mask of planes it touches or 0xffffffff if outside.
// @early-stop: plane pointer offsets and reject-return block layout still differ.
/* fake match: preserve x87 global-zero compare order and dot-product scheduling. */
unsigned int __cdecl testBoxAgainstFrustum_guess(EqGfxClipVolume_10030daa_guess *ctx, float *boxMin, float *boxMax)
{
    float *plane;
    float *base;
    const float *zero;
    unsigned int count;
    unsigned int mask;
    unsigned int bit;
    int left;
    float distance;
    float px;
    float py;
    float pz;

    count = 0;
    base = &ctx->planes[0].normalZ;
    bit = 1;
    mask = 0;
    plane = base;
    zero = &EQGFX_ZERO_F_GUESS;
    while (1)
    {
        if (!(plane[-2] < EQGFX_ZERO_F_GUESS))
            px = boxMax[0];
        else
            px = boxMin[0];
        if (plane[-1] >= EQGFX_ZERO_F_GUESS)
            py = boxMax[1];
        else
            py = boxMin[1];
        if (plane[0] >= EQGFX_ZERO_F_GUESS)
            pz = boxMax[2];
        else
            pz = boxMin[2];
        distance = py * plane[-1];
        distance = distance + px * plane[-2];
        distance = distance + pz * plane[0];
        distance = distance - plane[1];
        if (distance < *zero)
            break;
        count++;
        plane += 5;
        if (count >= 6)
        {
            plane = base;
            left = 6;
            do
            {
                if (!(plane[-2] < EQGFX_ZERO_F_GUESS))
                    px = boxMin[0];
                else
                    px = boxMax[0];
                if (plane[-1] >= EQGFX_ZERO_F_GUESS)
                    py = boxMin[1];
                else
                    py = boxMax[1];
                if (plane[0] >= EQGFX_ZERO_F_GUESS)
                    pz = boxMin[2];
                else
                    pz = boxMax[2];
                distance = pz * plane[0];
                distance = distance + py * plane[-1];
                distance = distance + px * plane[-2];
                distance = distance - plane[1];
                if (distance < *zero)
                    mask |= bit;
                bit <<= 1;
                plane += 5;
                left--;
            } while (left != 0);
            return mask;
        }
    }
    return 0xffffffff;
}

// FUNCTION: EQGFX 0x10030f46
// Combines two setup checks for the object; the purpose is unclear.
// __cdecl. Returns 0xffff3d3e when initializeTextureMemorySlotTable_guess or initializeTextureMemoryHeaps_guess returns nonzero, else 0.
int __cdecl RunTwoSetupCalls_guess(FUN_10030f6e_object_guess *object)
{
    int result;

    result = 0;
    if (EQ_CDECL(int, 0x10030F8F, (FUN_10030f6e_object_guess *))(object) != 0 ||
        EQ_CDECL(int, 0x10030FFE, (FUN_10030f6e_object_guess *))(object) != 0)
    {
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x10030f6e
// Releases two memory allocations; their specific use is unclear.
int __cdecl freeObjectAllocations_guess(FUN_10030f6e_object_guess *object)
{
    t3dFreeMemory_guess(object->allocation_1dc);
    t3dFreeMemory_guess(object->allocation_1d0);
    return 0;
}

#define FUN_10030F8F_ALLOCATION_ERROR 0xffff3d3e

// FUNCTION: EQGFX 0x10030f8f
// Creates the reusable allocation-slot pool used by the renderer.
int __cdecl initializeTextureMemorySlotTable_guess(FUN_10030f6e_object_guess *object)
{
    int result;
    void *allocation;
    unsigned int slotIndex;

    result = 0;
    object->slotCapacity = object->initialCount * 0x32;
    allocation = t3dAllocateMemory_guess(object->slotCapacity * 0x28);
    object->allocation_1dc = allocation;
    if (allocation == 0)
    {
        result = FUN_10030F8F_ALLOCATION_ERROR;
    }
    else
    {
        object->freeSlotHead |= 0xffffffff;
        slotIndex = 0;
        object->activeCount = object->slotCapacity;
        if (object->slotCapacity > 0)
        {
            do
            {
                EQ_CDECL(void, 0x1003138A, (void *, unsigned int))(object, slotIndex);
                EQ_CDECL(void, 0x100313B7, (void *, unsigned int))(object, slotIndex);
                slotIndex++;
            } while (slotIndex < object->slotCapacity);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10030ffe
// Partitions graphics texture memory into allocatable ranges for each texture unit.
int __cdecl initializeTextureMemoryHeaps_guess(FUN_10030f6e_object_guess *resource)
{
    unsigned int groupIndex;
    unsigned int alignment;
    unsigned int count;
    unsigned int minAddress;
    unsigned int remaining;
    unsigned int slotIndex;
    unsigned int previousSlot;
    unsigned int *mapping;
    FUN_10031411_slot_guess *slots;
    int initializeMapping;
    int result;
    void *allocation;

    groupIndex = 0;
    alignment = resource->alignment;
    result = 0;
    count = resource->initialCount;
    allocation = t3dAllocateMemory_guess(count << 3);
    resource->allocation_1d0 = allocation;
    if (allocation == 0)
    {
        result = FUN_10030F8F_ALLOCATION_ERROR;
    }
    else if (count > 0)
    {
        do
        {
            initializeMapping = 1;
            mapping = (unsigned int *)resource->allocation_1d0 + groupIndex * 2;
            minAddress = pfn_grTexMinAddress_guess(groupIndex);
            remaining = pfn_grTexMaxAddress_guess(groupIndex);
            remaining = remaining + (1 - minAddress);
            previousSlot = 0xffffffff;
            do
            {
                EQ_CDECL(void, 0x10031411, (FUN_10030f6e_object_guess *, unsigned int *))(resource, &slotIndex);
                slots = (FUN_10031411_slot_guess *)resource->allocation_1dc;
                if (initializeMapping)
                {
                    initializeMapping = 0;
                    mapping[1] = slotIndex;
                    mapping[0] = groupIndex;
                }
                pfn_grTexClampMode_guess(mapping[0], 0, 0);
                slots[slotIndex].unknown_004 = groupIndex;
                slots[slotIndex].cursor = minAddress;
                slots[slotIndex].unknown_000 = slots[slotIndex].unknown_000 | 1;
                if (remaining < 0x200001)
                {
                    slots[slotIndex].limit = remaining;
                    remaining = 0;
                }
                else
                {
                    minAddress = minAddress + 0x200000;
                    remaining = remaining - 0x200000;
                    slots[slotIndex].limit = 0x200000;
                }
                EQ_CDECL(void, 0x10031157, (char *, unsigned int))((char *)(slots + slotIndex), alignment);
                slots[slotIndex].previousIndex = previousSlot;
                if (previousSlot != 0xffffffff)
                {
                    slots[previousSlot].nextIndex = slotIndex;
                }
                previousSlot = slotIndex;
            } while (remaining != 0);
            groupIndex = groupIndex + 1;
        } while (groupIndex < count);
    }
    return result;
}

// FUNCTION: EQGFX 0x10031157
// Prepares an aligned span of memory for a renderer allocation.
// __cdecl (the callee pops 0 bytes; ECX is scratch, overwritten first). Rounds the cursor at +0x8
// up to the `align` boundary into +0x10, then sets +0x14 to the remaining bytes (+0xc minus the
// padding), or 0 when the padding reaches past +0xc.
/* fake match: Compute the mask first to keep it in ECX and the cursor in EDX, matching the original register use. */
void __cdecl alignBlockAndComputeUsable_guess(EqGfxStream_10031157_guess *stream, unsigned int align)
{
    unsigned int cursor;
    unsigned int mask;

    mask = align - 1;
    cursor = stream->cursor;
    if ((mask & cursor) != 0)
    {
        stream->aligned = (align + cursor) & ~mask;
    }
    else
    {
        stream->aligned = cursor;
    }
    cursor = stream->aligned - cursor;
    if (cursor < stream->available)
    {
        stream->remaining = stream->available - cursor;
        return;
    }
    stream->remaining = 0;
}

// FUNCTION: EQGFX 0x10031190
// Reclaims the memory ranges assigned to a render object.
// __cdecl. Detaches renderObj from its list, then frees the table slots named
// by its +0x4c/+0x50/+0x54 indices. Returns 0 and clears the indices on
// success, 0xffff3d3e when a free fails.
int __cdecl ReleaseRenderObjectAllocations_guess(char *holder, char *renderObj, unsigned int *outFirst, unsigned int *outSecond, unsigned int *outThird)
{
    int result;

    result = 0;
    if (outFirst != 0)
        *outFirst = 0xffffffff;
    if (outSecond != 0)
        *outSecond = 0xffffffff;
    if (outThird != 0)
        *outThird = 0xffffffff;
    EQ_CDECL(void, 0x10046006, (char *))(renderObj);
    if (((T3dTexInfo_guess*)renderObj)->slotIndex_guess == -1)
        goto checkSecond;
    if (EQ_CDECL(int, 0x10031229, (char *, unsigned int, unsigned int *))(holder, ((T3dTexInfo_guess*)renderObj)->slotIndex_guess, outFirst) != 0)
        goto fail;
checkSecond:
    if (((T3dTexInfo_guess*)renderObj)->evenMipSlotIndex_guess == -1)
        goto checkThird;
    if (EQ_CDECL(int, 0x10031229, (char *, unsigned int, unsigned int *))(holder, ((T3dTexInfo_guess*)renderObj)->evenMipSlotIndex_guess, outSecond) != 0)
        goto fail;
checkThird:
    if (((T3dTexInfo_guess*)renderObj)->oddMipSlotIndex_guess != -1)
        {
    }
    else
        goto success;
    if (EQ_CDECL(int, 0x10031229, (char *, unsigned int, unsigned int *))(holder, ((T3dTexInfo_guess*)renderObj)->oddMipSlotIndex_guess, outThird) == 0)
        goto success;
fail:
    result = 0xffff3d3e;
    goto done;
success:
    ((T3dTexInfo_guess*)renderObj)->slotIndex_guess = -1;
    ((T3dTexInfo_guess*)renderObj)->evenMipSlotIndex_guess = -1;
    ((T3dTexInfo_guess*)renderObj)->oddMipSlotIndex_guess = -1;
done:
    return result;
}

// FUNCTION: EQGFX 0x10031229
// Combines adjacent free ranges in the texture-memory pool for later reuse.
// __cdecl. Merges the 0x28-byte slot at `index` in the table at +0x1dc with a
// neighbour (prev at +0x1c, next at +0x20) when flags allow, then frees slots.
int __cdecl coalesceTextureMemorySlots_guess(char *holder, unsigned int index, unsigned int *outIndex)
{
    char *table;
    char *slot;
    char *prev;
    char *next;
    int prevIndex;
    int nextIndex;
    unsigned int align;

    align = *(unsigned int *)(holder + 0x1e4);
    table = *(char **)(holder + 0x1dc);
    prev = (char *)0;
    next = (char *)0;
    prevIndex = *(int *)(table + index * 0x28 + 0x1c);
    *(unsigned int *)(table + index * 0x28 + 0x18) = 0;
    slot = table + index * 0x28;
    if (prevIndex != -1)
        prev = table + prevIndex * 0x28;
    nextIndex = *(int *)(slot + 0x20);
    if (nextIndex != -1)
        next = table + nextIndex * 0x28;
    if ((*slot & 1) == 0 && prev != (char *)0 && *(unsigned int *)(prev + 0x18) == 0)
    {
        if (next != (char *)0 && *(unsigned int *)(next + 0x18) == 0 && (*next & 1) == 0)
        {
            *(int *)(prev + 0x20) = *(int *)(next + 0x20);
            if (*(int *)(next + 0x20) != -1)
                *(unsigned int *)(table + *(int *)(next + 0x20) * 0x28 + 0x1c) = *(unsigned int *)(slot + 0x1c);
            *(unsigned int *)prev |= *(unsigned int *)next;
            *(unsigned int *)(prev + 0xc) += *(unsigned int *)(slot + 0xc);
            *(unsigned int *)(prev + 0xc) = *(unsigned int *)(next + 0xc) + *(unsigned int *)(prev + 0xc);
            if (outIndex != (unsigned int *)0)
                *outIndex = *(unsigned int *)(slot + 0x1c);
            EQ_CDECL(void, 0x10031157, (char *, unsigned int))(prev, align);
            EQ_CDECL(void, 0x100313B7, (char *, int))(holder, *(int *)(slot + 0x20));
        }
        else
        {
            *(int *)(prev + 0x20) = nextIndex;
            if (next != (char *)0)
                *(unsigned int *)(next + 0x1c) = *(unsigned int *)(slot + 0x1c);
            if (outIndex != (unsigned int *)0)
                *outIndex = *(unsigned int *)(slot + 0x1c);
            *(unsigned int *)prev |= *(unsigned int *)slot;
            *(unsigned int *)(prev + 0xc) += *(unsigned int *)(slot + 0xc);
            EQ_CDECL(void, 0x10031157, (char *, unsigned int))(prev, align);
        }
        EQ_CDECL(void, 0x100313B7, (char *, int))(holder, (int)index);
    }
    else if (next != (char *)0 && *(unsigned int *)(next + 0x18) == 0 && (*next & 1) == 0)
    {
        if (prev != (char *)0)
            *(int *)(prev + 0x20) = nextIndex;
        *(unsigned int *)(next + 0x1c) = *(unsigned int *)(slot + 0x1c);
        *(unsigned int *)next |= *(unsigned int *)slot;
        *(unsigned int *)(next + 0xc) += *(unsigned int *)(slot + 0xc);
        *(unsigned int *)(next + 0x8) = *(unsigned int *)(slot + 0x8);
        if (outIndex != (unsigned int *)0)
            *outIndex = *(unsigned int *)(slot + 0x20);
        EQ_CDECL(void, 0x10031157, (char *, unsigned int))(next, align);
        EQ_CDECL(void, 0x100313B7, (char *, int))(holder, (int)index);
    }
    else
    {
        if (outIndex != (unsigned int *)0)
            *outIndex = index;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1003138a
// Resets an allocation slot before it can be assigned again.
// __cdecl (the callee pops 0 bytes; ECX is scratch). Resets the 0x28-byte slot
// at `index` in the table at +0x1dc: clears +0x0 and +0x18, sets +0x4, +0x1c
// and +0x20 to -1, and returns the slot.
char *__cdecl resetTextureMemorySlot_guess(char *holder, int index)
{
    char *base;

    base = *(char **)(holder + 0x1dc);
    *(unsigned int *)(base + index * 0x28) = 0;
    *(unsigned int *)(base + index * 0x28 + 0x18) = 0;
    *(unsigned int *)(base + index * 0x28 + 0x20) = 0xffffffff;
    *(unsigned int *)(base + index * 0x28 + 0x1c) = 0xffffffff;
    *(unsigned int *)(base + index * 0x28 + 0x4) = 0xffffffff;
    return base + index * 0x28;
}

// FUNCTION: EQGFX 0x100313b7
// Returns an unused allocation slot to the manager's free list.
// __cdecl (the callee pops 0 bytes; ECX is scratch). Unlinks the 0x28-byte slot
// at `index` in the table at +0x1dc from the 8-byte entry at +0x1d0 when it is
// the current head, then clears the slot and pushes it onto the +0x1e0 chain.
void __cdecl releaseSlotToFreeList_guess(char *holder, int index)
{
    char *slot;
    int linked;
    char *list;
    char *entry;

    slot = *(char **)(holder + 0x1dc) + index * 0x28;
    linked = *(int *)(slot + 4);
    if (linked != -1)
    {
        list = *(char **)(holder + 0x1d0);
        entry = list + linked * 8;
        if (index == *(int *)(entry + 4))
            *(int *)(entry + 4) = *(int *)(slot + 0x20);
    }
    *(unsigned int *)(slot + 0x18) = 0;
    *(int *)(slot + 4) = -1;
    *(int *)(slot + 0x20) = -1;
    *(int *)(slot + 0x1c) = -1;
    *(int *)(slot + 0x24) = *(int *)(holder + 0x1e0);
    (*(int *)(holder + 0x1d4))--;
    *(int *)(holder + 0x1e0) = index;
}

// FUNCTION: EQGFX 0x10031411
// Obtains an allocation slot, growing the slot table when its free list is empty.
int __cdecl allocateTextureMemorySlot_guess(FUN_10030f6e_object_guess *holder, unsigned int *slotIndexOut)
{
    int result;
    unsigned int slotIndex;
    unsigned int oldCapacity;
    unsigned int newCapacity;

    slotIndex = holder->freeSlotHead;
    result = 0;
    if (slotIndex == 0xffffffff)
    {
        oldCapacity = holder->slotCapacity;
        newCapacity = oldCapacity * 2;
        holder->allocation_1dc = t3dReallocateMemory_guess(holder->allocation_1dc, newCapacity * 0x28);
        if (holder->allocation_1dc == 0)
        {
            result = 0xffff3d3e;
            goto done;
        }
        holder->slotCapacity = newCapacity;
        holder->activeCount += newCapacity - oldCapacity - 1;
        slotIndex = oldCapacity;
        while (slotIndex = slotIndex + 1, slotIndex < newCapacity)
        {
            EQ_CDECL(char *, 0x1003138A, (FUN_10030f6e_object_guess *, unsigned int))(holder, slotIndex);
            EQ_CDECL(void, 0x100313B7, (char *, int))((char *)holder, slotIndex);
        }
        slotIndex = oldCapacity;
    }
    else
    {
        holder->freeSlotHead = ((FUN_10031411_slot_guess *)holder->allocation_1dc)[slotIndex].nextFree;
    }
    holder->activeCount++;
    EQ_CDECL(char *, 0x1003138A, (FUN_10030f6e_object_guess *, unsigned int))(holder, slotIndex);
    *slotIndexOut = slotIndex;

done:
    return result;
}

// FUNCTION: EQGFX 0x100314bc
// Loads a texture's mipmap data into graphics memory, reclaiming old allocations if needed.
int __cdecl LoadTextureIntoMemory_guess(int holderAddress, int objectAddress)
{
    int result;
    int firstSlot;
    int secondSlot;
    int secondTextureSize;
    int thirdTextureSize;
    int scanCount;
    int currentObjectAddress;
    FUN_10030f6e_object_guess *holder;
    EqGfxRenderObject_100314bc_guess *renderObject;
    EqGfxRenderObject_100314bc_guess *currentObject;

    holder = (FUN_10030f6e_object_guess *)holderAddress;
    renderObject = (EqGfxRenderObject_100314bc_guess *)objectAddress;
    firstSlot = -1;
    secondSlot = -1;
    objectAddress = -1;
    result = 0;
    if (renderObject->textureMipMapMode == 0 || holder->initialCount == 1)
    {
        holderAddress = pfn_grTexTextureMemRequired_100314bc_guess(3, renderObject->textureInfo);
        secondTextureSize = 0;
        thirdTextureSize = 0;
    }
    else
    {
        holderAddress = 0;
        secondTextureSize = pfn_grTexTextureMemRequired_100314bc_guess(1, renderObject->textureInfo);
        thirdTextureSize = pfn_grTexTextureMemRequired_100314bc_guess(2, renderObject->textureInfo);
    }
    result = EQ_CDECL(int, 0x1003167F, (char *, unsigned int, unsigned int, unsigned int, int *, int *, int *))((char *)holder, holderAddress, secondTextureSize, thirdTextureSize, &firstSlot, &secondSlot, &objectAddress);
    if (result == 0)
    {
        if (firstSlot != -1)
            goto useFirstSlot;
        if (secondSlot != -1)
            goto useSecondSlot;
        if (objectAddress != -1)
            goto failed;

        currentObjectAddress = (int)holder->renderObjectHead;
        scanCount = EQGFX_RENDER_OBJECT_SCAN_LIMIT_100314BC;
        do
        {
            if (currentObjectAddress == 0)
                break;
            currentObject = (EqGfxRenderObject_100314bc_guess *)currentObjectAddress;
            if (currentObject->textureFormat == renderObject->textureFormat &&
                currentObject->textureWidth == renderObject->textureWidth &&
                currentObject->textureMipMapMode == renderObject->textureMipMapMode &&
                currentObject->textureDepth == renderObject->textureDepth)
            {
                result = EQ_CDECL(int, 0x10031190, (char *, char *, int *, int *, int *))((char *)holder, (char *)currentObject, &firstSlot, &secondSlot, &objectAddress);
                goto afterRemoval;
            }
            currentObjectAddress = (int)currentObject->next;
            scanCount--;
        } while (scanCount != 0);

        do
        {
            if (holder->renderObjectHead == 0)
                return 0xffff3d3e;
            result = EQ_CDECL(int, 0x10031190, (char *, char *, int *, int *, int *))((char *)holder, (char *)holder->renderObjectHead, 0, 0, 0);
            if (result != 0)
                return 0xffff3d3e;
            result = EQ_CDECL(int, 0x1003167F, (char *, unsigned int, unsigned int, unsigned int, int *, int *, int *))((char *)holder, holderAddress, secondTextureSize, thirdTextureSize, &firstSlot, &secondSlot, &objectAddress);
        afterRemoval:
            if (result != 0)
                return 0xffff3d3e;
            if (firstSlot != -1)
                goto useFirstSlot;
            if (secondSlot != -1)
                goto useSecondSlot;
        } while (objectAddress == -1);
    }
    goto failed;

useFirstSlot:
    result = EQ_CDECL(int, 0x10031868, (char *, int, int))((char *)holder, firstSlot, holderAddress);
    if (result == 0)
    {
        EQ_CDECL(void, 0x1003174E, (char *, char *, int))((char *)holder, (char *)renderObject, firstSlot);
        return 0;
    }
    goto failed;

useSecondSlot:
    if (objectAddress != -1 && EQ_CDECL(int, 0x10031868, (char *, int, int))((char *)holder, secondSlot, secondTextureSize) == 0)
    {
        EQ_CDECL(void, 0x10031796, (char *, char *, int))((char *)holder, (char *)renderObject, secondSlot);
        result = EQ_CDECL(int, 0x10031868, (char *, int, int))((char *)holder, objectAddress, thirdTextureSize);
        if (result == 0)
        {
            EQ_CDECL(void, 0x100317DA, (char *, char *, int))((char *)holder, (char *)renderObject, objectAddress);
            return 0;
        }
    }

failed:
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x1003167f
// Finds free memory blocks for a request with one or two size requirements.
// __cdecl (the callee pops 0 bytes; ECX is scratch). Scans `total` entries
// at +0x1c4: with `size1` it looks up one slot and stops on first hit,
// otherwise looks up `size2`/`size3` slots (clearing each size after its
// was found for the first output clears the other two. Returns 0 or
// 0xffff3d3e when a lookup fails.
int __cdecl findTextureSlotsForSizes_guess(char *holder, unsigned int size1, unsigned int size2, unsigned int size3, int *outFirst, int *outSecond, int *outThird)
{
    int result;
    unsigned int total;
    unsigned int idx;

    result = 0;
    *outFirst = -1;
    *outSecond = -1;
    *outThird = -1;
    total = *(unsigned int *)(holder + 0x1c4);
    if (total != 0)
    {
        idx = 0;
        do
        {
            if (size1 != 0)
            {
                if (EQ_CDECL(int, 0x1003181E, (char *, int, unsigned int, int *))(holder, idx, size1, outFirst) != 0)
                {
                    result = 0xffff3d3e;
                    goto done;
                }
                if (*outFirst != -1)
                    break;
            }
            else
            {
                if (size2 != 0)
                {
                    if (EQ_CDECL(int, 0x1003181E, (char *, int, unsigned int, int *))(holder, idx, size2, outSecond) != 0)
                    {
                        result = 0xffff3d3e;
                        goto done;
                    }
                    if (*outSecond != -1)
                    {
                        size2 = 0;
                        goto next;
                    }
                }
                if (size3 != 0)
                {
                    if (EQ_CDECL(int, 0x1003181E, (char *, int, unsigned int, int *))(holder, idx, size3, outThird) != 0)
                    {
                        result = 0xffff3d3e;
                        goto done;
                    }
                    if (*outThird != -1)
                        size3 = 0;
                }
next:;
            }
            idx++;
        } while (idx < total);
    }
    if (*outFirst == -1 && (*outSecond == -1 || *outThird == -1))
    {
        *outSecond = -1;
        *outThird = -1;
    }
done:
    return result;
}

// FUNCTION: EQGFX 0x1003174e
// Uploads both even and odd mipmap levels of a texture to graphics memory.
// __cdecl (the callee pops 0 bytes; ECX is scratch). Downloads mip level 3 of
// the 0x28-byte slot at `index` in the table at +0x1dc through the entry at
// +0x1d0, then points the slot at `tex` and tags `tex` with the slot index.
void __cdecl downloadTextureMipMapWithMode3_guess(char *holder, char *tex, int index)
{
    char *slot;
    char *list;

    slot = *(char **)(holder + 0x1dc) + index * 0x28;
    list = *(char **)(holder + 0x1d0);
    pfn_grTexDownloadMipMap_guess(*(void **)(list + *(int *)(slot + 4) * 8), *(void **)(slot + 0x10), 3, tex + 0x38);
    *(char **)(slot + 0x18) = tex;
    *(int *)(tex + 0x50) |= -1;
    *(int *)(tex + 0x54) |= -1;
    *(int *)(tex + 0x4c) = index;
}

// FUNCTION: EQGFX 0x10031796
// Uploads a texture's even mipmap levels to graphics memory.
// __cdecl (the callee pops 0 bytes; ECX is scratch). Downloads mip level 1 of
// the 0x28-byte slot at `index` in the table at +0x1dc through the entry at
// +0x1d0, then points the slot at `tex` and tags `tex` with the slot index.
void __cdecl downloadTextureMipMapWithMode1_guess(char *holder, char *tex, int index)
{
    char *slot;
    char *list;

    slot = *(char **)(holder + 0x1dc) + index * 0x28;
    list = *(char **)(holder + 0x1d0);
    pfn_grTexDownloadMipMap_guess(*(void **)(list + *(int *)(slot + 4) * 8), *(void **)(slot + 0x10), 1, tex + 0x38);
    *(char **)(slot + 0x18) = tex;
    *(int *)(tex + 0x4c) |= -1;
    *(int *)(tex + 0x50) = index;
}

// FUNCTION: EQGFX 0x100317da
// Uploads a texture's odd mipmap levels to graphics memory.
// __cdecl (the callee pops 0 bytes; ECX is scratch). Downloads mip level 2 of
// the 0x28-byte slot at `index` in the table at +0x1dc through the entry at
// +0x1d0, then points the slot at `tex` and tags `tex` with the slot index.
void __cdecl downloadTextureMipMapWithMode2_guess(char *holder, char *tex, int index)
{
    char *slot;
    char *list;

    slot = *(char **)(holder + 0x1dc) + index * 0x28;
    list = *(char **)(holder + 0x1d0);
    pfn_grTexDownloadMipMap_guess(*(void **)(list + *(int *)(slot + 4) * 8), *(void **)(slot + 0x10), 2, tex + 0x38);
    *(char **)(slot + 0x18) = tex;
    *(int *)(tex + 0x4c) |= -1;
    *(int *)(tex + 0x54) = index;
}

// FUNCTION: EQGFX 0x1003181e
// Finds one free memory block large enough for a requested allocation.
// __cdecl (the callee pops 0 bytes; ECX is scratch). Walks the chain at
// +0x1d0 from `listIndex` for a free 0x28-byte slot (+0x18 == 0) whose
// +0x14 holds at least `minSize`; stores the slot index in `*outIndex`
// (-1 when none) and returns 0.
int __cdecl findFreeTextureSlotBySize_guess(char *holder, int listIndex, unsigned int minSize, int *outIndex)
{
    char *list;
    char *slotBase;
    char *slot;
    int slotIndex;

    list = *(char **)(holder + 0x1d0);
    slotBase = *(char **)(holder + 0x1dc);
    *outIndex |= -1;
    slotIndex = *(int *)(list + listIndex * 8 + 4);
    while (1)
    {
        if (slotIndex == -1)
            return 0;
        slot = slotBase + slotIndex * 0x28;
        if (*(int *)(slot + 0x18) == 0 && *(unsigned int *)(slot + 0x14) >= minSize)
            break;
        slotIndex = *(int *)(slot + 0x20);
    }
    *outIndex = slotIndex;
    return 0;
}

// FUNCTION: EQGFX 0x10031868
// Splits an allocation block to reserve a requested region.
int __cdecl SplitAllocationSlot_guess(FUN_10030f6e_object_guess *holder, int sourceIndex, unsigned int size)
{
    int result;
    unsigned int available;
    int originalIndex;
    unsigned int sourceOffset;
    FUN_10031411_slot_guess *slots;
    FUN_10031411_slot_guess *sourceSlot;
    FUN_10031411_slot_guess *splitSlot;
    FUN_10031411_slot_guess *nextSlot;

    result = 0;
    originalIndex = sourceIndex;
    sourceOffset = sourceIndex * FUN_10031868_SLOT_STRIDE;
    slots = (FUN_10031411_slot_guess *)holder->allocation_1dc;
    sourceSlot = (FUN_10031411_slot_guess *)((char *)slots + sourceOffset);
    available = sourceSlot->remaining;
    if (available < size + FUN_10031868_SPLIT_RESERVE)
    {
        if (size > available)
            result = 0xffff3d3e;
    }
    else
    {
        if (EQ_CDECL(int, 0x10031411, (FUN_10030f6e_object_guess *, int *))(holder, &sourceIndex) == 0)
        {
            slots = (FUN_10031411_slot_guess *)holder->allocation_1dc;
            sourceSlot = (FUN_10031411_slot_guess *)((char *)slots + sourceOffset);
            splitSlot = &slots[sourceIndex];
            splitSlot->cursor = sourceSlot->alignedCursor + size;
            splitSlot->limit = (sourceSlot->limit - sourceSlot->alignedCursor) - size + sourceSlot->cursor;
            sourceSlot->limit = (sourceSlot->alignedCursor - sourceSlot->cursor) + size;
            splitSlot->unknown_004 = sourceSlot->unknown_004;
            splitSlot->previousIndex = (int)originalIndex;
            splitSlot->nextIndex = sourceSlot->nextIndex;
            if (sourceSlot->nextIndex != -1)
            {
                nextSlot = &slots[sourceSlot->nextIndex];
                nextSlot->previousIndex = (int)sourceIndex;
            }
            sourceSlot->nextIndex = (int)sourceIndex;
            EQ_CDECL(void, 0x10031157, (char *, unsigned int))((char *)splitSlot, holder->alignment);
            EQ_CDECL(void, 0x10031157, (char *, unsigned int))((char *)sourceSlot, holder->alignment);
        }
        else
            result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x1003193d
// __cdecl. Derives mip/scale fields at +0x38..+0x5c from extents at +0x1c/+0x20.
int __cdecl t3dComputeTexLodAspect_guess(char *texInfo)
{
    unsigned int extentA;
    unsigned int extentB;
    unsigned int dim;
    int shift;
    int result;

    extentA = *(unsigned int *)(texInfo + 0x20);
    extentB = *(unsigned int *)(texInfo + 0x1c);
    result = 0;
    if (extentA < extentB)
    {
        switch (extentB / extentA)
        {
        case 1:
            *(int *)(texInfo + 0x40) = 0;
            *(float *)(texInfo + 0x58) = 256.0f;
            *(float *)(texInfo + 0x5c) = 256.0f;
            break;
        case 2:
            *(int *)(texInfo + 0x40) = 1;
            *(float *)(texInfo + 0x58) = 256.0f;
            *(float *)(texInfo + 0x5c) = 128.0f;
            break;
        case 4:
            *(int *)(texInfo + 0x40) = 2;
            *(float *)(texInfo + 0x58) = 256.0f;
            *(float *)(texInfo + 0x5c) = 64.0f;
            break;
        case 8:
            *(int *)(texInfo + 0x40) = 3;
            *(float *)(texInfo + 0x58) = 256.0f;
            *(float *)(texInfo + 0x5c) = 32.0f;
            break;
        default:
            result = (int)0xffff3d3e;
            goto done;
        }
        dim = extentB;
    }
    else
    {
        switch (extentA / extentB)
        {
        case 1:
            *(int *)(texInfo + 0x40) = 0;
            *(float *)(texInfo + 0x58) = 256.0f;
            *(float *)(texInfo + 0x5c) = 256.0f;
            break;
        case 2:
            *(int *)(texInfo + 0x40) = -1;
            *(float *)(texInfo + 0x58) = 128.0f;
            *(float *)(texInfo + 0x5c) = 256.0f;
            break;
        case 4:
            *(int *)(texInfo + 0x40) = -2;
            *(float *)(texInfo + 0x58) = 64.0f;
            *(float *)(texInfo + 0x5c) = 256.0f;
            break;
        case 8:
            *(int *)(texInfo + 0x40) = -3;
            *(float *)(texInfo + 0x58) = 32.0f;
            *(float *)(texInfo + 0x5c) = 256.0f;
            break;
        default:
            result = (int)0xffff3d3e;
            goto done;
        }
        dim = extentA;
    }
    if (dim > 0x10)
    {
        switch (dim)
        {
        case 0x20:
            *(int *)(texInfo + 0x3c) = 5;
            break;
        case 0x40:
            *(int *)(texInfo + 0x3c) = 6;
            break;
        case 0x80:
            *(int *)(texInfo + 0x3c) = 7;
            break;
        case 0x100:
            *(int *)(texInfo + 0x3c) = 8;
            break;
        default:
            result = (int)0xffff3d3e;
            goto done;
        }
    }
    else
    {
        switch (dim)
        {
        case 0x10:
            *(int *)(texInfo + 0x3c) = 4;
            break;
        case 1:
            *(int *)(texInfo + 0x3c) = 0;
            break;
        case 2:
            *(int *)(texInfo + 0x3c) = 1;
            break;
        case 4:
            *(int *)(texInfo + 0x3c) = 2;
            break;
        case 8:
            *(int *)(texInfo + 0x3c) = 3;
            break;
        default:
            result = (int)0xffff3d3e;
            goto done;
        }
    }
    for (shift = *(int *)(texInfo + 0x24); shift != 0; shift--)
        dim >>= 1;
    if (dim > 0x10)
    {
        switch (dim)
        {
        case 0x20:
            *(int *)(texInfo + 0x38) = 5;
            break;
        case 0x40:
            *(int *)(texInfo + 0x38) = 6;
            break;
        case 0x80:
            *(int *)(texInfo + 0x38) = 7;
            break;
        case 0x100:
            *(int *)(texInfo + 0x38) = 8;
            break;
        default:
            result = (int)0xffff3d3e;
            goto done;
        }
    }
    else
    {
        switch (dim)
        {
        case 0x10:
            *(int *)(texInfo + 0x38) = 4;
            break;
        case 1:
            *(int *)(texInfo + 0x38) = 0;
            break;
        case 2:
            *(int *)(texInfo + 0x38) = 1;
            break;
        case 4:
            *(int *)(texInfo + 0x38) = 2;
            break;
        case 8:
            *(int *)(texInfo + 0x38) = 3;
            break;
        default:
            result = (int)0xffff3d3e;
            goto done;
        }
    }
    *(int *)(texInfo + 0x44) = 0xb;
    *(int *)(texInfo + 0x48) = *(int *)(texInfo + 0x14);
done:
    return result;
}

// FUNCTION: EQGFX 0x10031b42
// __cdecl. Sets up texture combine/source stages for `tex`.
int __cdecl setupTextureCombineStages_guess(char *holder, char *texInfo)
{
    char *slotTable;
    char *slot;
    char *slotA;
    char *slotB;
    char *list;
    int *entry;
    int unit;
    int result;
    unsigned int valA;
    unsigned int valB;

    slotTable = *(char **)(holder + 0x1dc);
    result = 0;
    if (*(int *)(texInfo + 0x4c) != -1)
    {
        slot = slotTable + *(int *)(texInfo + 0x4c) * 0x28;
        list = *(char **)(holder + 0x1d0);
        entry = (int *)(list + *(int *)(slot + 4) * 8);
        if (*entry > 0)
        {
            for (unit = 0; unit < *entry; unit++)
                pfn_grTexCombine_guess(unit, 3, 8, 3, 8, 0, 0);
        }
        pfn_grTexCombine_guess(*entry, 1, 0, 1, 0, 0, 0);
        pfn_grTexSource_guess((void *)*entry, *(void **)(slot + 0x10), 3, texInfo + 0x38);
        pfn_grTexMipMapMode_guess((void *)*entry, 1, 0);
    }
    else if (*(int *)(texInfo + 0x50) == -1 || *(int *)(texInfo + 0x54) == -1)
    {
        result = 0xffff3d3e;
    }
    else
    {
        slotA = slotTable + *(int *)(texInfo + 0x50) * 0x28;
        slotB = slotTable + *(int *)(texInfo + 0x54) * 0x28;
        valA = *(unsigned int *)(slotA + 4);
        valB = *(unsigned int *)(slotB + 4);
        if (valA < valB)
        {
            list = *(char **)(holder + 0x1d0);
            entry = (int *)(list + valB * 8);
            if (*entry > 0)
            {
                for (unit = 0; unit < *entry; unit++)
                {
                    if (unit != (int)valA)
                        pfn_grTexCombine_guess(unit, 3, 8, 3, 8, 0, 0);
                }
            }
            entry = (int *)(list + valA * 8);
            pfn_grTexCombine_guess(*entry, 7, 5, 7, 5, 0, 0);
            pfn_grTexSource_guess((void *)*entry, *(void **)(slotA + 0x10), 1, texInfo + 0x38);
            pfn_grTexMipMapMode_guess((void *)*entry, 1, 1);
            entry = (int *)(list + valB * 8);
            pfn_grTexCombine_guess(*entry, 1, 0, 1, 0, 0, 0);
            pfn_grTexSource_guess((void *)*entry, *(void **)(slotB + 0x10), 2, texInfo + 0x38);
            pfn_grTexMipMapMode_guess((void *)*entry, 1, 1);
        }
        else
        {
            list = *(char **)(holder + 0x1d0);
            entry = (int *)(list + valA * 8);
            if (*entry > 0)
            {
                for (unit = 0; unit < *entry; unit++)
                {
                    if (unit != (int)valB)
                        pfn_grTexCombine_guess(unit, 3, 8, 3, 8, 0, 0);
                }
            }
            entry = (int *)(list + valB * 8);
            pfn_grTexCombine_guess(*entry, 7, 13, 7, 13, 0, 0);
            pfn_grTexSource_guess((void *)*entry, *(void **)(slotB + 0x10), 2, texInfo + 0x38);
            pfn_grTexMipMapMode_guess((void *)*entry, 1, 1);
            entry = (int *)(list + valA * 8);
            pfn_grTexCombine_guess(*entry, 1, 0, 1, 0, 0, 0);
            pfn_grTexSource_guess((void *)*entry, *(void **)(slotA + 0x10), 1, texInfo + 0x38);
            pfn_grTexMipMapMode_guess((void *)*entry, 1, 1);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10031d7d
// __cdecl. Stores count on the context, allocates count * 8 bytes into resource_438; -1 on failure, else 1.
int __cdecl allocateRenderContextResourceArray_guess(RenderContextResources_guess *renderContext, int count)
{
    void *allocation;

    renderContext->unknown_434 = count;
    allocation = t3dAllocateMemory_guess(count << 3);
    renderContext->resource_438 = allocation;
    if (allocation == 0)
    {
        renderContext->unknown_434 = 0;
        return -1;
    }
    return 1;
}

// FUNCTION: EQGFX 0x10031db0
// Releases a render context's allocated resources.
int __cdecl freeRenderContextResources_guess(RenderContextResources_guess *renderContext)
{
    renderContext->unknown_434 = 0;
    if (renderContext->resource_438 != 0)
    {
        t3dFreeMemory_guess(renderContext->resource_438);
        renderContext->resource_438 = 0;
    }
    return 1;
}

typedef void (__stdcall *DebugBreak_guess_t)(void);
#define pfn_DebugBreak_guess EQ_GLOBAL(DebugBreak_guess_t, 0x100760BC)

// FUNCTION: EQGFX 0x10031ddb
// __cdecl. Recursively composes hierarchical sprite transforms.
unsigned int __cdecl t3dHierarchicalSpriteBuildTransforms_guess(T3D_tagACTORINSTANCE *parent, char *holder, int param3, int *accum, void *next, int param6)
{
    float parentX;
    float parentY;
    float parentZ;
    float localX;
    float localY;
    float localZ;
    float scale;
    unsigned int cursor;
    unsigned int end;
    int count;
    char *entry;
    float *slot;
    void *slotNext;
    char *link;
    int kind;
    float negY;
    float negZ;
    float dx;
    float dy;
    float dz;
    float ax;
    float bx;
    float cx;
    float w;

    parentX = parent->position_guess[0];
    parentY = parent->position_guess[1];
    parentZ = parent->position_guess[2];
    count = *(int *)(*(char **)(holder + 0x5c) + 0x14);
    *accum += count;
    cursor = *(unsigned int *)(holder + 0x24);
    end = cursor + (unsigned int)count * 0x148;
    if (cursor >= end)
        return *(unsigned int *)(holder + 0x24);
    do
    {
        entry = (char *)cursor;
        slot = (float *)(entry + 0x64);
        slot[0x24] = 0.0f;
        slot[0x25] = 0.0f;
        if (cursor != end - 0x148)
            slotNext = (void *)(slot + 0x39);
        else
            slotNext = next;
        *(void **)((char *)slot + 0x98) = slotNext;
        scale = *(float *)(entry + 0x58);
        link = *(char **)(entry + 0x8);
        kind = *(int *)link;
        if (kind == 0)
        {
            localX = 0.0f;
            localY = 0.0f;
            localZ = 0.0f;
        }
        else if (kind == 0x14)
        {
            localX = *(float *)(link + 0x28);
            localY = *(float *)(link + 0x2c);
            localZ = *(float *)(link + 0x30);
            *(unsigned int *)((char *)slot + 0x98) =
                t3dHierarchicalSpriteBuildTransforms_guess(parent, link, param3, accum, slotNext, param6);
        }
        else if (kind == 0x48 || kind == 0x4d || kind == 0x51)
        {
            localX = 0.0f;
            localY = 0.0f;
            localZ = 0.0f;
        }
        else
        {
            pfn_DebugBreak_guess();
        }
        slot[10] = scale * slot[-2];
        slot[0xb] = scale * slot[1];
        slot[0xc] = scale * slot[4];
        slot[0xd] = scale * slot[-1];
        slot[0xe] = scale * slot[2];
        slot[0xf] = scale * slot[5];
        slot[0x10] = scale * slot[0];
        slot[0x11] = scale * slot[3];
        slot[0x12] = scale * slot[6];
        slot[0x13] = -localX;
        negY = -localY;
        slot[0x14] = negY;
        negZ = -localZ;
        slot[0x15] = negZ;
        dx = -*(float *)(holder + 0x28) - slot[7];
        dy = -*(float *)(holder + 0x2c) - slot[8];
        dz = -*(float *)(holder + 0x30) - slot[9];
        slot[0x13] = dy * slot[0xd] + dx * slot[10] + dz * slot[0x10] + slot[0x13];
        slot[0x14] = dy * slot[0xe] + dz * slot[0x11] + dx * slot[0xb] + negY;
        slot[0x15] = dy * slot[0xf] + dz * slot[0x12] + dx * slot[0xc] + negZ;
        slot[7] = localZ * slot[4] + localY * slot[1] + localX * slot[-2] + slot[7] + *(float *)(holder + 0x28);
        slot[8] = localZ * slot[5] + localY * slot[2] + localX * slot[-1] + slot[8] + *(float *)(holder + 0x2c);
        slot[9] = localZ * slot[6] + localY * slot[3] + localX * slot[0] + slot[9] + *(float *)(holder + 0x30);
        ax = slot[-2];
        bx = slot[-1];
        cx = slot[0];
        w = slot[-4];
        slot[-2] = bx * parent->scaledRotationMatrix_guess[3] + cx * parent->scaledRotationMatrix_guess[6] + ax * parent->scaledRotationMatrix_guess[0];
        slot[-1] = bx * parent->scaledRotationMatrix_guess[4] + cx * parent->scaledRotationMatrix_guess[7] + ax * parent->scaledRotationMatrix_guess[1];
        slot[0] = cx * parent->scaledRotationMatrix_guess[8] + ax * parent->scaledRotationMatrix_guess[2] + bx * parent->scaledRotationMatrix_guess[5];
        slot[0x16] = (bx * parent->rotationMatrix_guess[3] + ax * parent->rotationMatrix_guess[0] + cx * parent->rotationMatrix_guess[6]) * w;
        slot[0x17] = (cx * parent->rotationMatrix_guess[7] + ax * parent->rotationMatrix_guess[1] + bx * parent->rotationMatrix_guess[4]) * w;
        slot[0x18] = (ax * parent->rotationMatrix_guess[2] + bx * parent->rotationMatrix_guess[5] + cx * parent->rotationMatrix_guess[8]) * w;
        ax = slot[1];
        bx = slot[2];
        cx = slot[3];
        slot[1] = bx * parent->scaledRotationMatrix_guess[3] + cx * parent->scaledRotationMatrix_guess[6] + ax * parent->scaledRotationMatrix_guess[0];
        slot[2] = bx * parent->scaledRotationMatrix_guess[4] + cx * parent->scaledRotationMatrix_guess[7] + ax * parent->scaledRotationMatrix_guess[1];
        slot[3] = cx * parent->scaledRotationMatrix_guess[8] + ax * parent->scaledRotationMatrix_guess[2] + bx * parent->scaledRotationMatrix_guess[5];
        slot[0x19] = (bx * parent->rotationMatrix_guess[3] + ax * parent->rotationMatrix_guess[0] + cx * parent->rotationMatrix_guess[6]) * w;
        slot[0x1a] = (cx * parent->rotationMatrix_guess[7] + ax * parent->rotationMatrix_guess[1] + bx * parent->rotationMatrix_guess[4]) * w;
        slot[0x1b] = (ax * parent->rotationMatrix_guess[2] + bx * parent->rotationMatrix_guess[5] + cx * parent->rotationMatrix_guess[8]) * w;
        ax = slot[4];
        bx = slot[5];
        cx = slot[6];
        slot[4] = bx * parent->scaledRotationMatrix_guess[3] + cx * parent->scaledRotationMatrix_guess[6] + ax * parent->scaledRotationMatrix_guess[0];
        slot[5] = bx * parent->scaledRotationMatrix_guess[4] + cx * parent->scaledRotationMatrix_guess[7] + ax * parent->scaledRotationMatrix_guess[1];
        slot[6] = cx * parent->scaledRotationMatrix_guess[8] + ax * parent->scaledRotationMatrix_guess[2] + bx * parent->scaledRotationMatrix_guess[5];
        slot[0x1c] = (bx * parent->rotationMatrix_guess[3] + ax * parent->rotationMatrix_guess[0] + cx * parent->rotationMatrix_guess[6]) * w;
        slot[0x1d] = (cx * parent->rotationMatrix_guess[7] + ax * parent->rotationMatrix_guess[1] + bx * parent->rotationMatrix_guess[4]) * w;
        slot[0x1e] = (ax * parent->rotationMatrix_guess[2] + bx * parent->rotationMatrix_guess[5] + cx * parent->rotationMatrix_guess[8]) * w;
        ax = slot[7];
        bx = slot[8];
        cx = slot[9];
        slot[7] = ax * parent->scaledRotationMatrix_guess[0] + cx * parent->scaledRotationMatrix_guess[6] + bx * parent->scaledRotationMatrix_guess[3] + parentX;
        slot[8] = ax * parent->scaledRotationMatrix_guess[1] + cx * parent->scaledRotationMatrix_guess[7] + bx * parent->scaledRotationMatrix_guess[4] + parentY;
        slot[9] = cx * parent->scaledRotationMatrix_guess[8] + ax * parent->scaledRotationMatrix_guess[2] + bx * parent->scaledRotationMatrix_guess[5] + parentZ;
        ax = slot[10];
        bx = slot[0xd];
        cx = slot[0x10];
        slot[10] = bx * parent->inverseScaleRotationMatrix_guess[1] + ax * parent->inverseScaleRotationMatrix_guess[0] + cx * parent->inverseScaleRotationMatrix_guess[2];
        slot[0xd] = bx * parent->inverseScaleRotationMatrix_guess[4] + ax * parent->inverseScaleRotationMatrix_guess[3] + cx * parent->inverseScaleRotationMatrix_guess[5];
        slot[0x10] = bx * parent->inverseScaleRotationMatrix_guess[7] + cx * parent->inverseScaleRotationMatrix_guess[8] + ax * parent->inverseScaleRotationMatrix_guess[6];
        slot[0x13] = slot[0x13] - (parentY * slot[0xd] + parentX * slot[10] + parentZ * slot[0x10]);
        ax = slot[0xb];
        bx = slot[0xe];
        cx = slot[0x11];
        slot[0xb] = bx * parent->inverseScaleRotationMatrix_guess[1] + ax * parent->inverseScaleRotationMatrix_guess[0] + cx * parent->inverseScaleRotationMatrix_guess[2];
        slot[0xe] = bx * parent->inverseScaleRotationMatrix_guess[4] + ax * parent->inverseScaleRotationMatrix_guess[3] + cx * parent->inverseScaleRotationMatrix_guess[5];
        slot[0x11] = bx * parent->inverseScaleRotationMatrix_guess[7] + cx * parent->inverseScaleRotationMatrix_guess[8] + ax * parent->inverseScaleRotationMatrix_guess[6];
        slot[0x14] = slot[0x14] - (parentX * slot[0xb] + parentZ * slot[0x11] + parentY * slot[0xe]);
        ax = slot[0xc];
        bx = slot[0xf];
        cx = slot[0x12];
        slot[0xc] = bx * parent->inverseScaleRotationMatrix_guess[1] + ax * parent->inverseScaleRotationMatrix_guess[0] + cx * parent->inverseScaleRotationMatrix_guess[2];
        slot[0xf] = bx * parent->inverseScaleRotationMatrix_guess[4] + ax * parent->inverseScaleRotationMatrix_guess[3] + cx * parent->inverseScaleRotationMatrix_guess[5];
        slot[0x12] = bx * parent->inverseScaleRotationMatrix_guess[7] + cx * parent->inverseScaleRotationMatrix_guess[8] + ax * parent->inverseScaleRotationMatrix_guess[6];
        slot[0x15] = slot[0x15] - (parentX * slot[0xc] + parentZ * slot[0x12] + parentY * slot[0xf]);
        cursor += 0x148;
    } while (cursor < end);
    return *(unsigned int *)(holder + 0x24);
}

#define transformFrustumWithMatrices EQ_CDECL(void, 0x10037093, (void *frustum, float *transform, float *matrix, char *output))
#define testBoxAgainstPlanes EQ_CDECL(unsigned int, 0x10030E4A, (char *ctx, float *boxMin, float *boxMax))

// FUNCTION: EQGFX 0x10032462
// Updates a hierarchical sprite and processes its render work when it passes view checks.
int __cdecl updateAndRenderHierarchicalSprite_guess(SpriteUpdateRequest_guess *request, ClipCtx_guess *renderContext, ParentTransform_guess *parentTransform)
{
    int frameStamp;
    SpriteHolder_guess *sprite;
    int updateResult;

    frameStamp = EQGFX_SPRITE_TRANSFORM_STAMP_10032462;
    if (parentTransform->transformStamp != frameStamp)
    {
        parentTransform->transformStamp = frameStamp;
        sprite = renderContext->currentSprite;
        if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 1 ||
            (updateResult = EQ_CDECL(int, 0x10030DAA, (char *, float *, float))(
                renderContext->frustum, parentTransform->position,
                parentTransform->radiusScale * parentTransform->radius), updateResult != -1))
        {
            if (EQ_CDECL(int, 0x10035400, (ParentTransform_guess *, ClipCtx_guess *))(
                parentTransform, renderContext) != 1)
            {
                if ((parentTransform->updateFlags & 0x10) != 0 &&
                    (sprite->bounds->flags & 0x40) != 0)
                {
                    updateResult = EQ_CDECL(int, 0x10035058,
                        (ParentTransform_guess *, float, float *))(
                            parentTransform, parentTransform->radius,
                            parentTransform->position);
                    if (EQ_CDECL(int, 0x1003252C,
                        (SpriteUpdateRequest_guess *, ClipCtx_guess *, SpriteHolder_guess *, ParentTransform_guess *))(
                            request, renderContext, sprite, parentTransform) != 0)
                        return updateResult;
                }
                parentTransform->frame = renderContext->currentFrame;
                EQ_CDECL(int, 0x100327C3,
                    (SpriteHolder_guess *, SpriteUpdateRequest_guess *, ParentTransform_guess *))(
                        sprite, request, parentTransform);
                EQ_CDECL(int, 0x10032638,
                    (ParentTransform_guess *, SpriteHolder_guess *, ClipCtx_guess *, SpriteUpdateRequest_guess *))(
                        parentTransform, sprite, renderContext, request);
            }
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x1003252c
// Checks a sprite's bounds for view-frustum visibility.
// __cdecl. Returns 1 when the holder's box is outside the frustum; else stores the plane mask in clipFlags and returns 0.
int __cdecl testSpriteBoundsAgainstFrustum_guess(int unused, ClipCtx_guess *ctx, SpriteHolder_guess *holder, ParentTransform_guess *parent)
{
    float transform[12];
    char localFrustum[216];
    float negX;
    float negY;
    float negZ;
    int result;

    transform[0] = parent->rotation[0];
    transform[1] = parent->rotation[1];
    transform[2] = parent->rotation[2];
    transform[3] = parent->rotation[3];
    transform[4] = parent->rotation[4];
    transform[5] = parent->rotation[5];
    transform[6] = parent->rotation[6];
    transform[7] = parent->rotation[7];
    transform[8] = parent->rotation[8];
    negX = -parent->position[0];
    negY = -parent->position[1];
    negZ = -parent->position[2];
    transform[9] = negZ * transform[6] + negY * transform[3] + negX * transform[0] - holder->position[0];
    transform[10] = negZ * transform[7] + negY * transform[4] + negX * transform[1] - holder->position[1];
    transform[11] = negZ * transform[8] + negY * transform[5] + negX * transform[2] - holder->position[2];
    transformFrustumWithMatrices(ctx->frustum, transform, parent->matrix, localFrustum);
    result = testBoxAgainstPlanes(localFrustum, holder->bounds->boxMin, holder->bounds->boxMax);
    if (result == -1)
        return 1;
    ctx->clipFlags = result;
    return 0;
}

// FUNCTION: EQGFX 0x10032638
// Draws a hierarchical sprite's batch items, then each record's object by type; type 0x14 recurses.
int __cdecl drawHierarchicalSpriteParts_guess(void *device, EqGfxCommandList_10032638_guess *commandList, EqGfxRenderContext_10032638_guess *renderContext, void *renderParameters)
{
    EqGfxBatchItem_10032638_guess *batchItem;
    EqGfxCommandRecord_10032638_guess *commandCursor;
    EqGfxCommandRecord_10032638_guess *commandEnd;
    EqGfxCommandList_10032638_guess *operation;
    unsigned int batchIndex;
    int operationType;
    int result;

    batchIndex = 0;
    result = 1;
    if (commandList->batchCount != 0)
    {
        do
        {
            batchItem = commandList->batchItems[batchIndex];
            renderContext->renderPosition[0] = -renderContext->sourcePosition[0];
            renderContext->renderPosition[1] = -renderContext->sourcePosition[1];
            renderContext->renderPosition[2] = -renderContext->sourcePosition[2];
            renderContext->currentOperation = batchItem;
            if (batchItem->drawMode->mode == 0)
            {
                result = EQ_CDECL(int, 0x10037C24, (EqGfxBatchItem_10032638_guess *, void *, EqGfxRenderContext_10032638_guess *, void *, EqGfxCommandList_10032638_guess *))(batchItem, renderParameters, renderContext, device, commandList);
            }
            else
            {
                result = EQ_CDECL(int, 0x10037979, (EqGfxBatchItem_10032638_guess *, void *, EqGfxRenderContext_10032638_guess *, void *, EqGfxCommandList_10032638_guess *))(batchItem, renderParameters, renderContext, device, commandList);
            }
            if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
                renderContext->destinationState = renderContext->sourceState;
            if (result < 0)
                return EQGFX_RECURSIVE_RENDER_ERROR_10032638;
            batchIndex++;
        } while (batchIndex < commandList->batchCount);
    }
    commandCursor = commandList->commandRecords;
    commandEnd = commandCursor + commandList->commandCount;
    do
    {
        if (commandEnd <= commandCursor)
            return result;
        operation = commandCursor->operation;
        operationType = operation->type;
        if (operationType != 0)
        {
            if (operationType == EQGFX_OPERATION_NESTED_TYPE_10032638)
            {
                drawHierarchicalSpriteParts_guess(device, operation, renderContext, renderParameters);
            }
            else if (operationType == EQGFX_OPERATION_RENDER_TYPE_10032638)
            {
                if (operation->drawMode->mode == 0)
                {
                    result = EQ_CDECL(int, 0x100357EF, (void *, EqGfxRenderContext_10032638_guess *, void *, EqGfxCommandList_10032638_guess *, unsigned int))(renderParameters, renderContext, device, operation, (unsigned int)commandCursor);
                }
                else
                {
                    result = EQ_CDECL(int, 0x10035483, (void *, EqGfxRenderContext_10032638_guess *, void *, EqGfxCommandList_10032638_guess *, unsigned int))(renderParameters, renderContext, device, operation, (unsigned int)commandCursor);
                }
                if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
                    renderContext->destinationState = renderContext->sourceState;
                if (result < 0)
                    return result;
            }
            else if (operationType != EQGFX_OPERATION_IGNORED_TYPE_10032638)
            {
                if (operationType == EQGFX_OPERATION_STATE_TYPE_10032638)
                {
                    renderContext->currentOperation = operation;
                    result = EQ_CDECL(int, 0x1003F961, (void *, EqGfxRenderContext_10032638_guess *))((char *)commandCursor + 0x5c, renderContext);
                    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
                        renderContext->destinationState = renderContext->sourceState;
                    if (result < 0)
                        return result;
                    renderContext->currentOperation = commandList;
                }
                else
                {
                    pfn_DebugBreak_guess();
                }
            }
        }
        commandCursor++;
    } while (1);
}

// FUNCTION: EQGFX 0x100327C3
// Refreshes a hierarchical sprite's frame state and full transform tree for display.
// __cdecl. Returns 0xffff3d3e when _t3dComputeObjectToWorldCoefficients_guess fails; else stores the frame, updates through t3dHierarchicalSpriteBuildTransforms_guess and returns 0.
int __cdecl t3dHierarchicalSpriteUpdateAllTransformations(SpriteHolder_guess *holder, SpriteUpdateRequest_guess *request, ParentTransform_guess *parent)
{
    int entryCount;
    int scratch;

    if (EQ_CDECL(int, 0x10035058, (ParentTransform_guess *, float, float *))(parent, parent->radius, parent->position) < 0)
        return 0xffff3d3e;
    scratch = 0;
    entryCount = 0;
    EQ_CDECL(void, 0x10011CEC, (SpriteHolder_guess *, int))(holder, request->frame);
    EQ_CDECL(unsigned int, 0x10031DDB, (ParentTransform_guess *, SpriteHolder_guess *, int *, int *, void *, int))(parent, holder, &scratch, &entryCount, 0, request->frame);
    holder->lastFrame = request->frame;
    return 0;
}

// FUNCTION: EQGFX 0x10032828
// Lights indexed vertices: base colour plus animated point lights (type 0x1e), one normal for all.
// __cdecl, returns 1. Copies indexed vertices into the output and scratch records, advances point animations, adds int light.
int __cdecl t3dLightIndexedVerticesSharedNormal_guess(EqGfxRenderContext_guess *context, float *vertices, float *lightingPositions,
                         VertexLightingRequest_guess *request, int ambientRed, int ambientGreen, int ambientBlue)
{
    int *indexCursor;
    int *indexEnd;
    int vertexIndex;
    unsigned int *sourceWords;
    unsigned int *destinationWords;
    EqGfxScratchRecord_guess *output;
    EqGfxScratchRecord_guess *record;
    EqGfxScratchRecord_guess *recordEnd;
    EqGfxPointEffect_guess **effectCursor;
    EqGfxPointEffect_guess **effectEnd;
    EqGfxPointEffect_guess *effect;
    EqGfxAnimationState_guess *animation;
    EqGfxAnimationSequence_guess *sequence;
    float *frame;
    float value;
    unsigned int framePeriod;
    unsigned int elapsed;
    float pointX;
    float pointY;
    float pointZ;
    float frameOffset[3];
    float directionX;
    float directionY;
    float directionZ;
    float deltaX;
    float deltaY;
    float deltaZ;
    float distanceSquared;

    indexCursor = request->indices;
    indexEnd = indexCursor + request->count;
    output = (EqGfxScratchRecord_guess *)context->outputRecords;
    record = context->scratchRecords;
    while (indexCursor < indexEnd)
    {
        vertexIndex = *indexCursor;
        sourceWords = (unsigned int *)(vertices + vertexIndex * 3);
        destinationWords = (unsigned int *)output;
        destinationWords[0] = sourceWords[0];
        destinationWords[1] = sourceWords[1];
        destinationWords[2] = sourceWords[2];
        output++;
        sourceWords = (unsigned int *)(lightingPositions + vertexIndex * 3);
        destinationWords = (unsigned int *)record;
        destinationWords[0] = sourceWords[0];
        destinationWords[1] = sourceWords[1];
        destinationWords[2] = sourceWords[2];
        *(int *)&record->result[0] = ambientRed;
        *(int *)&record->result[1] = ambientGreen;
        *(int *)&record->result[2] = ambientBlue;
        record++;
        indexCursor++;
    }

    recordEnd = context->scratchRecords + request->count;
    directionX = request->direction[0];
    directionY = request->direction[1];
    directionZ = request->direction[2];
    effectCursor = context->effects;
    effectEnd = effectCursor + context->effectCount;
    while (effectCursor < effectEnd)
    {
        effect = *effectCursor;
        if (effect->type == 0x1e && effect != context->excludedEffect)
        {
            pointX = effect->x;
            pointY = effect->y;
            pointZ = effect->z;
            animation = effect->animation;
            sequence = animation->sequence;
            if (1 < sequence->frameCount && animation->lastFrameTime < context->currentFrameTime &&
                (animation->flags & 8) == 0)
            {
                if (animation->playbackMode == 0)
                {
                    animation->frameIndex++;
                    if (sequence->frameCount <= animation->frameIndex)
                    {
                        animation->frameIndex = 0;
                    }
                    animation->lastFrameTime = animation->framePeriod + context->currentFrameTime;
                }
                else
                {
                    framePeriod = animation->framePeriod;
                    elapsed = context->currentFrameTime - animation->lastFrameTime;
                    if (framePeriod << 4 < elapsed)
                    {
                        animation->frameIndex += elapsed / framePeriod + 1;
                        animation->frameIndex %= sequence->frameCount;
                        elapsed -= (elapsed / framePeriod) * framePeriod;
                    }
                    else
                    {
                        while (framePeriod < elapsed)
                        {
                            animation->frameIndex++;
                            if (sequence->frameCount <= animation->frameIndex)
                            {
                                animation->frameIndex = 0;
                            }
                            elapsed -= framePeriod;
                        }
                        animation->frameIndex++;
                        if (sequence->frameCount <= animation->frameIndex)
                        {
                            animation->frameIndex = 0;
                        }
                    }
                    animation->lastFrameTime = context->currentFrameTime - elapsed + framePeriod;
                }
            }

            if (sequence->positions != 0)
            {
                if (1 < sequence->frameCount)
                {
                    frame = sequence->positions + animation->frameIndex * 3;
                }
                else
                {
                    frame = sequence->positions;
                }
                frameOffset[0] = frame[0];
                frameOffset[1] = frame[1];
                frameOffset[2] = frame[2];
            }
            else
            {
                if (1 < sequence->frameCount)
                {
                    value = sequence->values.frameValues[animation->frameIndex];
                }
                else
                {
                    value = sequence->values.singleValue;
                }
                frameOffset[0] = value;
                frameOffset[1] = value;
                frameOffset[2] = value;
            }
            frameOffset[0] = effect->radius * effect->radius * EQGFX_POINT_RADIUS_SCALE_GUESS * frameOffset[0];
            frameOffset[1] = effect->radius * effect->radius * EQGFX_POINT_RADIUS_SCALE_GUESS * frameOffset[1];
            frameOffset[2] = effect->radius * effect->radius * EQGFX_POINT_RADIUS_SCALE_GUESS * frameOffset[2];

            for (record = context->scratchRecords; record < recordEnd; record++)
            {
                deltaX = pointX - record->position[0];
                deltaY = pointY - record->position[1];
                deltaZ = pointZ - record->position[2];
                distanceSquared = (pointZ - record->position[2]) * (pointZ - record->position[2]) +
                                  (pointY - record->position[1]) * (pointY - record->position[1]) +
                                  (pointX - record->position[0]) * (pointX - record->position[0]);
                if (EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared)
                {
                    __asm
                    {
                        fld distanceSquared
                        push ecx
                        fstp dword ptr [esp]
                        mov edx, 0x100FC77C
                        call dword ptr [edx]
                        fld deltaZ
                        fmul directionZ
                        fld deltaY
                        fmul directionY
                        pop ecx
                        faddp st(1), st
                        fld deltaX
                        fmul directionX
                        faddp st(1), st
                        fmulp st(1), st
                        mov edx, 0x10076284
                        fcom dword ptr [edx]
                        fnstsw ax
                        sahf
                        jbe lightingSkip
                        fld distanceSquared
                        mov edx, 0x10076280
                        fadd dword ptr [edx]
                        fdivp st(1), st
                        mov edx, 0x100762E8
                        fld dword ptr [edx]
                        fld st(1)
                        lea edx, frameOffset
                        fmul dword ptr [edx]
                        fmul st(0), st(1)
                        mov edx, 0x1004EC36
                        call edx
                        fld st(1)
                        lea edx, frameOffset
                        fmul dword ptr [edx + 4]
                        mov ecx, record
                        add dword ptr [ecx + 0x24], eax
                        fmul st(0), st(1)
                        mov edx, 0x1004EC36
                        call edx
                        fld st(1)
                        lea edx, frameOffset
                        fmul dword ptr [edx + 8]
                        mov ecx, record
                        add dword ptr [ecx + 0x28], eax
                        fmul st(0), st(1)
                        mov edx, 0x1004EC36
                        call edx
                        mov ecx, record
                        add dword ptr [ecx + 0x2c], eax
                        fstp st(0)
                        fstp st(0)
                        jmp lightingDone
lightingSkip:
                        fstp st(0)
lightingDone:
                    }
                }
            }
        }
        effectCursor++;
    }

    record = context->scratchRecords;
    output = (EqGfxScratchRecord_guess *)context->outputRecords;
    while (record < recordEnd)
    {
        *(int *)&output->result[0] = *(int *)&record->result[0];
        *(int *)&output->result[1] = *(int *)&record->result[1];
        *(int *)&output->result[2] = *(int *)&record->result[2];
        record++;
        output++;
    }
    return 1;
}

// FUNCTION: EQGFX 0x10032B51
// __cdecl, returns 1. Fills scratch records from indexed vertices; point effects advance and add int light to them.
int __cdecl processVerticesWithLighting_guess(EqGfxRenderContext_guess *context, float *vertices, float *lightingPositions,
                                              VertexLightingRequest_guess *request, int ambientRed, int ambientGreen,
                                              int ambientBlue)
{
    int *indexCursor;
    int *indexEnd;
    int vertexIndex;
    unsigned int *sourceWords;
    unsigned int *destinationWords;
    EqGfxScratchRecord_guess *output;
    EqGfxScratchRecord_guess *record;
    EqGfxScratchRecord_guess *recordEnd;
    EqGfxPointEffect_guess **effectCursor;
    EqGfxPointEffect_guess **effectEnd;
    EqGfxPointEffect_guess *effect;
    EqGfxAnimationState_guess *animation;
    EqGfxAnimationSequence_guess *sequence;
    float *frame;
    float value;
    unsigned int framePeriod;
    unsigned int elapsed;
    float pointX;
    float pointY;
    float pointZ;
    float scale;
    float frameOffset[3];
    float dx;
    float dy;
    float dz;
    float distanceSquared;
    float distanceFactor;
    float facing;
    float intensity;

    indexCursor = request->indices;
    indexEnd = indexCursor + request->count;
    output = (EqGfxScratchRecord_guess *)context->outputRecords;
    record = context->scratchRecords;
    while (indexCursor < indexEnd)
    {
        vertexIndex = *indexCursor;
        sourceWords = (unsigned int *)(vertices + vertexIndex * 3);
        destinationWords = (unsigned int *)output;
        destinationWords[0] = sourceWords[0];
        destinationWords[1] = sourceWords[1];
        destinationWords[2] = sourceWords[2];
        output++;
        sourceWords = (unsigned int *)(lightingPositions + vertexIndex * 3);
        destinationWords = (unsigned int *)record;
        destinationWords[0] = sourceWords[0];
        destinationWords[1] = sourceWords[1];
        destinationWords[2] = sourceWords[2];
        *(int *)&record->result[0] = ambientRed;
        *(int *)&record->result[1] = ambientGreen;
        *(int *)&record->result[2] = ambientBlue;
        record++;
        indexCursor++;
    }

    recordEnd = context->scratchRecords + request->count;
    effectCursor = context->effects;
    effectEnd = effectCursor + context->effectCount;
    while (effectCursor < effectEnd)
    {
        effect = *effectCursor;
        if (effect->type == 0x1e && effect != context->excludedEffect)
        {
            pointX = effect->x;
            pointY = effect->y;
            pointZ = effect->z;
            animation = effect->animation;
            sequence = animation->sequence;
            if (1 < sequence->frameCount && animation->lastFrameTime < context->currentFrameTime &&
                (animation->flags & 8) == 0)
            {
                if (animation->playbackMode == 0)
                {
                    animation->frameIndex++;
                    if (sequence->frameCount <= animation->frameIndex)
                    {
                        animation->frameIndex = 0;
                    }
                    animation->lastFrameTime = animation->framePeriod + context->currentFrameTime;
                }
                else
                {
                    framePeriod = animation->framePeriod;
                    elapsed = context->currentFrameTime - animation->lastFrameTime;
                    if (framePeriod << 4 < elapsed)
                    {
                        animation->frameIndex += elapsed / framePeriod + 1;
                        animation->frameIndex %= sequence->frameCount;
                        elapsed -= (elapsed / framePeriod) * framePeriod;
                    }
                    else
                    {
                        while (framePeriod < elapsed)
                        {
                            animation->frameIndex++;
                            if (sequence->frameCount <= animation->frameIndex)
                            {
                                animation->frameIndex = 0;
                            }
                            elapsed -= framePeriod;
                        }
                        animation->frameIndex++;
                        if (sequence->frameCount <= animation->frameIndex)
                        {
                            animation->frameIndex = 0;
                        }
                    }
                    animation->lastFrameTime = context->currentFrameTime - elapsed + framePeriod;
                }
            }

            if (sequence->positions != 0)
            {
                if (1 < sequence->frameCount)
                {
                    frame = sequence->positions + animation->frameIndex * 3;
                }
                else
                {
                    frame = sequence->positions;
                }
                frameOffset[0] = frame[0];
                frameOffset[1] = frame[1];
                frameOffset[2] = frame[2];
            }
            else
            {
                if (1 < sequence->frameCount)
                {
                    value = sequence->values.frameValues[animation->frameIndex];
                }
                else
                {
                    value = sequence->values.singleValue;
                }
                frameOffset[0] = value;
                frameOffset[1] = value;
                frameOffset[2] = value;
            }
            scale = effect->radius * effect->radius * EQGFX_POINT_RADIUS_SCALE_GUESS;
            frameOffset[0] = frameOffset[0] * scale;
            frameOffset[1] = frameOffset[1] * scale;
            frameOffset[2] = frameOffset[2] * scale;

            for (record = context->scratchRecords; record < recordEnd; record++)
            {
                dx = pointX - record->position[0];
                dy = pointY - record->position[1];
                dz = pointZ - record->position[2];
                distanceSquared = dz * dz + dy * dy + dx * dx;
                if (EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared)
                {
                    distanceFactor = EQGFX_DISTANCE_FACTOR_GUESS(distanceSquared);
                    facing = (dz * record->normal[2] + dy * record->normal[1] + dx * record->normal[0]) * distanceFactor;
                    if (facing > EQGFX_ZERO_F_GUESS)
                    {
                        intensity = facing / (distanceSquared + EQGFX_FLOAT_ONE);
                        *(int *)&record->result[0] += (int)(intensity * frameOffset[0] * EQGFX_JITTER_SCALE_GUESS);
                        *(int *)&record->result[1] += (int)(intensity * frameOffset[1] * EQGFX_JITTER_SCALE_GUESS);
                        *(int *)&record->result[2] += (int)(intensity * frameOffset[2] * EQGFX_JITTER_SCALE_GUESS);
                    }
                }
            }
        }
        effectCursor++;
    }

    record = context->scratchRecords;
    output = (EqGfxScratchRecord_guess *)context->outputRecords;
    while (record < recordEnd)
    {
        *(int *)&output->result[0] = *(int *)&record->result[0];
        *(int *)&output->result[1] = *(int *)&record->result[1];
        *(int *)&output->result[2] = *(int *)&record->result[2];
        *(unsigned int *)&output->normal[0] = *(unsigned int *)&record->normal[0];
        *(unsigned int *)&output->normal[1] = *(unsigned int *)&record->normal[1];
        *(unsigned int *)&output->normal[2] = *(unsigned int *)&record->normal[2];
        record++;
        output++;
    }
    return 1;
}

// FUNCTION: EQGFX 0x10032e73
// Lights scratch vertices already gathered: base colour plus animated point lights (type 0x1e), one normal.
int __cdecl t3dAddPointLightsSharedNormal_guess(EqGfxRenderContext_guess *context, int recordCount, float *direction,
                         float initialX, float initialY, float initialZ)
{
    EqGfxScratchRecord_guess *record;
    EqGfxScratchRecord_guess *recordEnd;
    EqGfxPointEffect_guess **effect;
    EqGfxPointEffect_guess **effectEnd;
    EqGfxPointEffect_guess *point;
    EqGfxAnimationState_guess *animation;
    EqGfxAnimationSequence_guess *sequence;
    EqGfxOutputRecord_guess *output;
    EqGfxScratchRecord_guess *scratch;
    unsigned int framePeriod;
    unsigned int elapsed;
    float sample[3];
    float attenuation;
    float directionX;
    float directionY;
    float directionZ;
    float deltaX;
    float deltaY;
    float deltaZ;
    float distanceSquared;
    float distanceFactor;
    float forwardAmount;
    float jitterBase;
    float roundInput;
    int roundResult;

    record = context->scratchRecords;
    recordEnd = record + recordCount;
    for (; record < recordEnd; record = record + 1)
    {
        record->result[0] = initialX;
        record->result[1] = initialY;
        record->result[2] = initialZ;
    }
    directionX = direction[0];
    directionY = direction[1];
    directionZ = direction[2];
    effect = context->effects;
    effectEnd = effect + context->effectCount;
    for (; effect < effectEnd; effect = effect + 1)
    {
        point = *effect;
        if ((point->type == 0x1e) && (point != context->excludedEffect))
        {
            animation = point->animation;
            sequence = animation->sequence;
            if (1 < sequence->frameCount)
            {
                if ((animation->lastFrameTime < context->currentFrameTime) &&
                    ((animation->flags & 8) == 0))
                {
                    if (animation->playbackMode == 0)
                    {
                        animation->frameIndex = animation->frameIndex + 1;
                        if (sequence->frameCount <= animation->frameIndex)
                            animation->frameIndex = 0;
                        animation->lastFrameTime = animation->framePeriod + context->currentFrameTime;
                    }
                    else
                    {
                        framePeriod = animation->framePeriod;
                        elapsed = context->currentFrameTime - animation->lastFrameTime;
                        if ((framePeriod << 4) < elapsed)
                        {
                            animation->frameIndex = animation->frameIndex + elapsed / framePeriod + 1;
                            elapsed = elapsed - (elapsed / framePeriod) * animation->framePeriod;
                            animation->frameIndex = animation->frameIndex % sequence->frameCount;
                        }
                        else
                        {
                            for (; framePeriod < elapsed; elapsed = elapsed - framePeriod)
                            {
                                animation->frameIndex = animation->frameIndex + 1;
                                if (sequence->frameCount <= animation->frameIndex)
                                    animation->frameIndex = 0;
                            }
                            animation->frameIndex = animation->frameIndex + 1;
                            if (sequence->frameCount <= animation->frameIndex)
                                animation->frameIndex = 0;
                        }
                        animation->lastFrameTime = (context->currentFrameTime - elapsed) + animation->framePeriod;
                    }
                }

                if (sequence->positions != 0)
                {
                    sample[0] = sequence->positions[animation->frameIndex * 3];
                    sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                    sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
                }
                else
                {
                    sample[0] = sequence->values.frameValues[animation->frameIndex];
                    sample[1] = sample[0];
                    sample[2] = sample[0];
                }
            }
            else if (sequence->positions != 0)
            {
                sample[0] = sequence->positions[0];
                sample[1] = sequence->positions[1];
                sample[2] = sequence->positions[2];
            }
            else
            {
                sample[0] = sequence->values.singleValue;
                sample[1] = sequence->values.singleValue;
                sample[2] = sequence->values.singleValue;
            }

            attenuation = point->radius * point->radius * EQGFX_POINT_RADIUS_SCALE_GUESS;
            sample[0] = attenuation * sample[0];
            sample[1] = attenuation * sample[1];
            sample[2] = attenuation * sample[2];
            record = context->scratchRecords;
            for (; record < recordEnd; record = record + 1)
            {
                deltaX = point->x - record->position[0];
                deltaY = point->y - record->position[1];
                deltaZ = point->z - record->position[2];
                distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
                if (EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared)
                {
                    distanceFactor = EQGFX_DISTANCE_FACTOR_GUESS(distanceSquared);
                    forwardAmount = (deltaX * directionX + deltaY * directionY + deltaZ * directionZ) * distanceFactor;
                    if (EQGFX_ZERO_F_GUESS < forwardAmount)
                    {
                        jitterBase = forwardAmount / (distanceSquared + EQGFX_FLOAT_ONE);
                        jitterBase = jitterBase * EQGFX_JITTER_SCALE_GUESS;

                        roundInput = jitterBase * sample[0];
                        roundResult = (int)roundInput;
                        ((int *)record->result)[0] = ((int *)record->result)[0] + roundResult;

                        roundInput = jitterBase * sample[1];
                        roundResult = (int)roundInput;
                        ((int *)record->result)[1] = ((int *)record->result)[1] + roundResult;

                        roundInput = jitterBase * sample[2];
                        roundResult = (int)roundInput;
                        ((int *)record->result)[2] = ((int *)record->result)[2] + roundResult;
                    }
                }
            }
        }
    }
    if (context->scratchRecords < recordEnd)
    {
        output = context->outputRecords;
        record = context->scratchRecords;
        do
        {
            output->result[0] = record->result[0];
            output->result[1] = record->result[1];
            output->result[2] = record->result[2];
            output = (EqGfxOutputRecord_guess *)((char *)output + 0x30);
            record = record + 1;
        } while (record < recordEnd);
    }
    return 1;
}
// FUNCTION: EQGFX 0x1003313e
// Lights already-gathered vertices by their own normals: base light plus point lights and directional lights
int __cdecl t3dAddPointAndDirectionalLightsPerNormal_guess(EqGfxRenderContext_guess *context, int recordCount,
    float directionX, float directionY, float directionZ, unsigned int flags)
{
    EqGfxScratchRecord_guess *record;
    EqGfxScratchRecord_guess *recordEnd;
    EqGfxOutputRecord_guess *output;
    EqGfxPointEffect_guess **effectEntry;
    EqGfxPointEffect_guess **effectEnd;
    EqGfxPointEffect_guess *point;
    EqGfxDirectionalEffect_guess *directional;
    EqGfxAnimationState_guess *animation;
    EqGfxAnimationSequence_guess *sequence;
    unsigned int framePeriod;
    unsigned int elapsed;
    float sample[3];
    float attenuation;
    float deltaX;
    float deltaY;
    float deltaZ;
    float distanceSquared;
    float distanceFactor;
    float forwardAmount;
    float jitterBase;
    float roundInput;
    int roundResult;

    recordEnd = context->scratchRecords + recordCount;
    for (record = context->scratchRecords; record < recordEnd; record = record + 1)
    {
        record->result[0] = directionX;
        record->result[1] = directionY;
        record->result[2] = directionZ;
    }

    effectEntry = context->effects;
    effectEnd = effectEntry + context->effectCount;
    for (; effectEntry < effectEnd; effectEntry = effectEntry + 1)
    {
        point = *effectEntry;
        if (point->type == 0x1e)
        {
            if ((flags & 0x40) != 0 && point != context->excludedEffect)
            {
                animation = point->animation;
                sequence = animation->sequence;
                if (sequence->frameCount > 1 &&
                    animation->lastFrameTime < context->currentFrameTime &&
                    (animation->flags & 8) == 0)
                {
                    if (animation->playbackMode == 0)
                    {
                        animation->frameIndex = animation->frameIndex + 1;
                        if (sequence->frameCount <= animation->frameIndex)
                            animation->frameIndex = 0;
                        animation->lastFrameTime = context->currentFrameTime + animation->framePeriod;
                    }
                    else
                    {
                        framePeriod = animation->framePeriod;
                        elapsed = context->currentFrameTime - animation->lastFrameTime;
                        if ((framePeriod << 4) < elapsed)
                        {
                            animation->frameIndex = animation->frameIndex + elapsed / framePeriod + 1;
                            animation->frameIndex = animation->frameIndex % sequence->frameCount;
                            elapsed = elapsed - (elapsed / framePeriod) * framePeriod;
                        }
                        else
                        {
                            for (; framePeriod < elapsed; elapsed = elapsed - framePeriod)
                            {
                                animation->frameIndex = animation->frameIndex + 1;
                                if (sequence->frameCount <= animation->frameIndex)
                                    animation->frameIndex = 0;
                            }
                            animation->frameIndex = animation->frameIndex + 1;
                            if (sequence->frameCount <= animation->frameIndex)
                                animation->frameIndex = 0;
                        }
                        animation->lastFrameTime = (framePeriod - elapsed) + context->currentFrameTime;
                    }
                }

                if (sequence->frameCount > 1)
                {
                    if (sequence->positions != 0)
                    {
                        sample[0] = sequence->positions[animation->frameIndex * 3];
                        sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                        sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
                    }
                    else
                    {
                        sample[0] = sequence->values.frameValues[animation->frameIndex];
                        sample[1] = sample[0];
                        sample[2] = sample[0];
                    }
                }
                else if (sequence->positions != 0)
                {
                    sample[0] = sequence->positions[animation->frameIndex * 3];
                    sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                    sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
                }
                else
                {
                    sample[0] = sequence->values.singleValue;
                    sample[1] = sequence->values.singleValue;
                    sample[2] = sequence->values.singleValue;
                }

                attenuation = point->radius * point->radius * EQGFX_POINT_RADIUS_SCALE_GUESS;
                sample[0] = attenuation * sample[0];
                sample[1] = attenuation * sample[1];
                sample[2] = attenuation * sample[2];
                for (record = context->scratchRecords; record < recordEnd; record = record + 1)
                {
                    deltaX = point->x - record->position[0];
                    deltaY = point->y - record->position[1];
                    deltaZ = point->z - record->position[2];
                    distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
                    if (EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared)
                    {
                        distanceFactor = EQGFX_DISTANCE_FACTOR_GUESS(distanceSquared);
                        forwardAmount = (deltaX * record->normal[0] + deltaY * record->normal[1] + deltaZ * record->normal[2]) * distanceFactor;
                        if (EQGFX_ZERO_F_GUESS < forwardAmount)
                        {
                            jitterBase = forwardAmount / (distanceSquared + EQGFX_FLOAT_ONE);
                            jitterBase = jitterBase * EQGFX_JITTER_SCALE_GUESS;

                            roundInput = jitterBase * sample[0];
                            __asm
                            {
                                fld roundInput
                            }
                            roundResult = EQGFX_FIX16_FROM_ST0();
                            ((int *)record->result)[0] = ((int *)record->result)[0] + roundResult;

                            roundInput = jitterBase * sample[1];
                            __asm
                            {
                                fld roundInput
                            }
                            roundResult = EQGFX_FIX16_FROM_ST0();
                            ((int *)record->result)[1] = ((int *)record->result)[1] + roundResult;

                            roundInput = jitterBase * sample[2];
                            __asm
                            {
                                fld roundInput
                            }
                            roundResult = EQGFX_FIX16_FROM_ST0();
                            ((int *)record->result)[2] = ((int *)record->result)[2] + roundResult;
                        }
                    }
                }
            }
        }
        else if (point->type == 0x1f && (flags & 0x200000) != 0)
        {
            directional = (EqGfxDirectionalEffect_guess *)point;
            animation = directional->animation;
            sequence = animation->sequence;
            if (sequence->frameCount > 1)
            {
                if (animation->lastFrameTime < context->currentFrameTime &&
                    (animation->flags & 8) == 0)
                {
                    if (animation->playbackMode == 0)
                    {
                        animation->frameIndex = animation->frameIndex + 1;
                        if (sequence->frameCount <= animation->frameIndex)
                            animation->frameIndex = 0;
                        animation->lastFrameTime = animation->framePeriod + context->currentFrameTime;
                    }
                    else
                    {
                        framePeriod = animation->framePeriod;
                        elapsed = context->currentFrameTime - animation->lastFrameTime;
                        if ((framePeriod << 4) < elapsed)
                        {
                            animation->frameIndex = animation->frameIndex + elapsed / framePeriod + 1;
                            elapsed = elapsed - (elapsed / framePeriod) * framePeriod;
                            animation->frameIndex = animation->frameIndex % sequence->frameCount;
                        }
                        else
                        {
                            for (; framePeriod < elapsed; elapsed = elapsed - framePeriod)
                            {
                                animation->frameIndex = animation->frameIndex + 1;
                                if (sequence->frameCount <= animation->frameIndex)
                                    animation->frameIndex = 0;
                            }
                            animation->frameIndex = animation->frameIndex + 1;
                            if (sequence->frameCount <= animation->frameIndex)
                                animation->frameIndex = 0;
                        }
                        animation->lastFrameTime = (framePeriod - elapsed) + context->currentFrameTime;
                    }
                }

                if (sequence->positions != 0)
                {
                    sample[0] = sequence->positions[animation->frameIndex * 3];
                    sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                    sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
                }
                else
                {
                    sample[0] = sequence->values.frameValues[animation->frameIndex];
                    sample[1] = sample[0];
                    sample[2] = sample[0];
                }
            }
            else if (sequence->positions != 0)
            {
                sample[0] = sequence->positions[animation->frameIndex * 3];
                sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
            }
            else
            {
                sample[0] = sequence->values.singleValue;
                sample[1] = sequence->values.singleValue;
                sample[2] = sequence->values.singleValue;
            }

            for (record = context->scratchRecords; record < recordEnd; record = record + 1)
            {
                forwardAmount = -(directional->directionX * record->normal[0] +
                    directional->directionY * record->normal[1] +
                    directional->directionZ * record->normal[2]);
                if (EQGFX_ZERO_F_GUESS < forwardAmount)
                {
                    roundInput = forwardAmount * sample[0] * EQGFX_JITTER_SCALE_GUESS;
                    __asm
                    {
                        fld roundInput
                    }
                    roundResult = EQGFX_FIX16_FROM_ST0();
                    ((int *)record->result)[0] = ((int *)record->result)[0] + roundResult;

                    roundInput = forwardAmount * sample[1] * EQGFX_JITTER_SCALE_GUESS;
                    __asm
                    {
                        fld roundInput
                    }
                    roundResult = EQGFX_FIX16_FROM_ST0();
                    ((int *)record->result)[1] = ((int *)record->result)[1] + roundResult;

                    roundInput = forwardAmount * sample[2] * EQGFX_JITTER_SCALE_GUESS;
                    __asm
                    {
                        fld roundInput
                    }
                    roundResult = EQGFX_FIX16_FROM_ST0();
                    ((int *)record->result)[2] = ((int *)record->result)[2] + roundResult;
                }
            }
        }
    }

    if (context->scratchRecords < recordEnd)
    {
        output = context->outputRecords;
        record = context->scratchRecords;
        do
        {
            ((unsigned int *)output->result)[0] = ((unsigned int *)record->result)[0];
            ((unsigned int *)output->result)[1] = ((unsigned int *)record->result)[1];
            ((unsigned int *)output->result)[2] = ((unsigned int *)record->result)[2];
            output = (EqGfxOutputRecord_guess *)((char *)output + 0x30);
            record = record + 1;
        } while (record < recordEnd);
    }
    return 1;
}

// FUNCTION: EQGFX 0x10033578
// Point-lights a batch's indexed vertices using one shared batch normal; also copies two values per vertex
int __cdecl t3dLightIndexedVerticesSharedNormalCopyPairs_guess(EqGfxRenderContext_10033578_guess *renderer,
    unsigned int *sourcePositions, unsigned int *scratchPositions,
    EqGfxBatch_10033578_guess *batch, unsigned int *recordValues,
    unsigned int resultValue0, unsigned int resultValue1, unsigned int resultValue2)
{
    unsigned int itemCount;
    int *itemIndex;
    int *itemEnd;
    unsigned int itemOffset;
    unsigned int *sourceItem;
    unsigned int *scratchItem;
    EqGfxOutputRecord_10033578_guess *outputRecord;
    EqGfxScratchRecord_10033578_guess *scratchRecord;
    EqGfxScratchRecord_10033578_guess *recordEnd;
    float directionX;
    float directionY;
    float directionZ;
    EqGfxPointLight_10033578_guess **light;
    EqGfxPointLight_10033578_guess **lightEnd;
    EqGfxPointLight_10033578_guess *point;
    EqGfxAnimationState_guess *animation;
    EqGfxAnimationSequence_guess *sequence;
    unsigned int framePeriod;
    unsigned int elapsed;
    float sample[3];
    float attenuation;
    float deltaX;
    float deltaY;
    float deltaZ;
    float distanceSquared;
    float distanceFactor;
    float forwardAmount;
    float jitterBase;
    float roundInput;
    int roundResult;
    float *scratchPosition;

    itemCount = batch->itemCount;
    itemEnd = batch->itemIndices + itemCount;
    outputRecord = renderer->outputRecords;
    scratchRecord = renderer->scratchRecords;
    for (itemIndex = batch->itemIndices; itemIndex < itemEnd; itemIndex = itemIndex + 1)
    {
        itemOffset = (unsigned int)*itemIndex * 3;
        sourceItem = sourcePositions + itemOffset;
        scratchItem = scratchPositions + itemOffset;
        outputRecord->positionBits[0] = sourceItem[0];
        outputRecord->positionBits[1] = sourceItem[1];
        outputRecord->positionBits[2] = sourceItem[2];
        outputRecord = outputRecord + 1;
        scratchRecord->positionBits[0] = scratchItem[0];
        scratchRecord->positionBits[1] = scratchItem[1];
        scratchRecord->positionBits[2] = scratchItem[2];
        scratchRecord->result[0] = resultValue0;
        scratchRecord->result[1] = resultValue1;
        scratchRecord->result[2] = resultValue2;
        scratchRecord = scratchRecord + 1;
    }

    directionX = batch->direction[0];
    directionY = batch->direction[1];
    directionZ = batch->direction[2];
    recordEnd = (EqGfxScratchRecord_10033578_guess *)((char *)renderer->scratchRecords + itemCount * 0x30);
    light = renderer->pointLights;
    lightEnd = light + renderer->pointLightCount;
    for (; light < lightEnd; light = light + 1)
    {
        point = *light;
        if (point->type == EQGFX_POINT_LIGHT_TYPE_10033578_GUESS && point != renderer->excludedPointLight)
        {
            animation = point->animation;
            sequence = animation->sequence;
            if (sequence->frameCount > 1 && animation->lastFrameTime < renderer->currentFrameTime &&
                (animation->flags & 8) == 0)
            {
                if (animation->playbackMode == 0)
                {
                    animation->frameIndex = animation->frameIndex + 1;
                    if (sequence->frameCount <= animation->frameIndex)
                    {
                        animation->frameIndex = 0;
                    }
                    animation->lastFrameTime = animation->framePeriod + renderer->currentFrameTime;
                }
                else
                {
                    framePeriod = animation->framePeriod;
                    elapsed = renderer->currentFrameTime - animation->lastFrameTime;
                    if ((framePeriod << 4) < elapsed)
                    {
                        animation->frameIndex = animation->frameIndex + elapsed / framePeriod + 1;
                        animation->frameIndex = animation->frameIndex % sequence->frameCount;
                        elapsed = elapsed - (elapsed / framePeriod) * framePeriod;
                    }
                    else
                    {
                        for (; framePeriod < elapsed; elapsed = elapsed - framePeriod)
                        {
                            animation->frameIndex = animation->frameIndex + 1;
                            if (sequence->frameCount <= animation->frameIndex)
                            {
                                animation->frameIndex = 0;
                            }
                        }
                        animation->frameIndex = animation->frameIndex + 1;
                        if (sequence->frameCount <= animation->frameIndex)
                        {
                            animation->frameIndex = 0;
                        }
                    }
                    animation->lastFrameTime = (framePeriod - elapsed) + renderer->currentFrameTime;
                }
            }

            if (sequence->frameCount > 1)
            {
                if (sequence->positions != 0)
                {
                    sample[0] = sequence->positions[animation->frameIndex * 3];
                    sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                    sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
                }
                else
                {
                    sample[0] = sequence->values.frameValues[animation->frameIndex];
                    sample[1] = sample[0];
                    sample[2] = sample[0];
                }
            }
            else if (sequence->positions != 0)
            {
                sample[0] = sequence->positions[animation->frameIndex * 3];
                sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
            }
            else
            {
                sample[0] = sequence->values.singleValue;
                sample[1] = sequence->values.singleValue;
                sample[2] = sequence->values.singleValue;
            }

            attenuation = point->radius * point->radius * EQGFX_POINT_RADIUS_SCALE_GUESS;
            sample[0] = attenuation * sample[0];
            sample[1] = attenuation * sample[1];
            sample[2] = attenuation * sample[2];
            scratchRecord = renderer->scratchRecords;
            for (; scratchRecord < recordEnd; scratchRecord = scratchRecord + 1)
            {
                scratchPosition = (float *)scratchRecord->positionBits;
                deltaX = point->x - scratchPosition[0];
                deltaY = point->y - scratchPosition[1];
                deltaZ = point->z - scratchPosition[2];
                distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
                if (EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared)
                {
                    distanceFactor = EQGFX_DISTANCE_FACTOR_GUESS(distanceSquared);
                    forwardAmount = (deltaX * directionX + deltaY * directionY + deltaZ * directionZ) * distanceFactor;
                    if (EQGFX_ZERO_F_GUESS < forwardAmount)
                    {
                        jitterBase = forwardAmount / (distanceSquared + EQGFX_FLOAT_ONE);
                        jitterBase = jitterBase * EQGFX_JITTER_SCALE_GUESS;

                        roundInput = jitterBase * sample[0];
                        roundResult = (int)roundInput;
                        scratchRecord->result[0] = scratchRecord->result[0] + roundResult;

                        roundInput = jitterBase * sample[1];
                        roundResult = (int)roundInput;
                        scratchRecord->result[1] = scratchRecord->result[1] + roundResult;

                        roundInput = jitterBase * sample[2];
                        roundResult = (int)roundInput;
                        scratchRecord->result[2] = scratchRecord->result[2] + roundResult;
                    }
                }
            }
        }
    }
    if (renderer->scratchRecords < recordEnd)
    {
        outputRecord = renderer->outputRecords;
        scratchRecord = renderer->scratchRecords;
        do
        {
            outputRecord->result[0] = scratchRecord->result[0];
            outputRecord->result[1] = scratchRecord->result[1];
            outputRecord->result[2] = scratchRecord->result[2];
            outputRecord->value0 = *recordValues;
            outputRecord->value1 = recordValues[1];
            recordValues = recordValues + 2;
            outputRecord = (EqGfxOutputRecord_10033578_guess *)((char *)outputRecord + 0x30);
            scratchRecord = scratchRecord + 1;
        } while (scratchRecord < recordEnd);
    }
    return 1;
}

// FUNCTION: EQGFX 0x100338b2
// Point-lights a batch's indexed vertices using each vertex's own normal; copies normals and two values each
// __cdecl, returns 1. Batch items copy their source and scratch positions into the output and scratch records; each point
// light of type 0x1e (other than the excluded one) advances its animation, then adds its jitter to the scratch results.
int __cdecl t3dLightIndexedVerticesPerNormalCopyPairs_guess(EqGfxRenderContext_10033578_guess *renderer,
    unsigned int *sourcePositions, unsigned int *scratchPositions,
    EqGfxBatch_10033578_guess *batch, unsigned int *recordValues,
    unsigned int resultValue0, unsigned int resultValue1, unsigned int resultValue2)
{
    unsigned int itemCount;
    int *itemIndex;
    int *itemEnd;
    unsigned int itemOffset;
    unsigned int *sourceItem;
    unsigned int *scratchItem;
    EqGfxOutputRecord_10033578_guess *outputRecord;
    EqGfxScratchRecord_10033578_guess *scratchRecord;
    EqGfxScratchRecord_10033578_guess *recordEnd;
    EqGfxPointLight_10033578_guess **light;
    EqGfxPointLight_10033578_guess **lightEnd;
    EqGfxPointLight_10033578_guess *point;
    EqGfxAnimationState_guess *animation;
    EqGfxAnimationSequence_guess *sequence;
    unsigned int framePeriod;
    unsigned int elapsed;
    float pointX;
    float pointY;
    float pointZ;
    float sample[3];
    float attenuation;
    float *scratchPosition;
    float *scratchNormal;
    float deltaX;
    float deltaY;
    float deltaZ;
    float distanceSquared;
    float distanceFactor;
    float forwardAmount;
    float jitterBase;
    float roundInput;
    int roundResult;

    itemCount = batch->itemCount;
    itemEnd = batch->itemIndices + itemCount;
    outputRecord = renderer->outputRecords;
    scratchRecord = renderer->scratchRecords;
    for (itemIndex = batch->itemIndices; itemIndex < itemEnd; itemIndex = itemIndex + 1)
    {
        itemOffset = (unsigned int)*itemIndex * 3;
        sourceItem = sourcePositions + itemOffset;
        scratchItem = scratchPositions + itemOffset;
        outputRecord->positionBits[0] = sourceItem[0];
        outputRecord->positionBits[1] = sourceItem[1];
        outputRecord->positionBits[2] = sourceItem[2];
        outputRecord = outputRecord + 1;
        scratchRecord->positionBits[0] = scratchItem[0];
        scratchRecord->positionBits[1] = scratchItem[1];
        scratchRecord->positionBits[2] = scratchItem[2];
        scratchRecord->result[0] = resultValue0;
        scratchRecord->result[1] = resultValue1;
        scratchRecord->result[2] = resultValue2;
        scratchRecord = scratchRecord + 1;
    }

    recordEnd = (EqGfxScratchRecord_10033578_guess *)((char *)renderer->scratchRecords + itemCount * 0x30);
    light = renderer->pointLights;
    lightEnd = light + renderer->pointLightCount;
    for (; light < lightEnd; light = light + 1)
    {
        point = *light;
        if (point->type == EQGFX_POINT_LIGHT_TYPE_10033578_GUESS && point != renderer->excludedPointLight)
        {
            pointX = point->x;
            animation = point->animation;
            pointY = point->y;
            sequence = animation->sequence;
            pointZ = point->z;
            if (sequence->frameCount > 1)
            {
                if (animation->lastFrameTime < renderer->currentFrameTime && (animation->flags & 8) == 0)
                {
                    if (animation->playbackMode == 0)
                    {
                        animation->frameIndex = animation->frameIndex + 1;
                        if (sequence->frameCount <= animation->frameIndex)
                        {
                            animation->frameIndex = 0;
                        }
                        animation->lastFrameTime = animation->framePeriod + renderer->currentFrameTime;
                    }
                    else
                    {
                        framePeriod = animation->framePeriod;
                        elapsed = renderer->currentFrameTime - animation->lastFrameTime;
                        if ((framePeriod << 4) < elapsed)
                        {
                            animation->frameIndex = animation->frameIndex + elapsed / framePeriod + 1;
                            animation->frameIndex = animation->frameIndex % sequence->frameCount;
                            elapsed = elapsed - (elapsed / framePeriod) * framePeriod;
                        }
                        else
                        {
                            for (; framePeriod < elapsed; elapsed = elapsed - framePeriod)
                            {
                                animation->frameIndex = animation->frameIndex + 1;
                                if (sequence->frameCount <= animation->frameIndex)
                                {
                                    animation->frameIndex = 0;
                                }
                            }
                            animation->frameIndex = animation->frameIndex + 1;
                            if (sequence->frameCount <= animation->frameIndex)
                            {
                                animation->frameIndex = 0;
                            }
                        }
                        animation->lastFrameTime = (renderer->currentFrameTime - elapsed) + framePeriod;
                    }
                }
                if (sequence->positions != 0)
                {
                    sample[0] = sequence->positions[animation->frameIndex * 3];
                    sample[1] = sequence->positions[animation->frameIndex * 3 + 1];
                    sample[2] = sequence->positions[animation->frameIndex * 3 + 2];
                }
                else
                {
                    sample[0] = sequence->values.frameValues[animation->frameIndex];
                    sample[1] = sample[0];
                    sample[2] = sample[0];
                }
            }
            else if (sequence->positions != 0)
            {
                sample[0] = sequence->positions[0];
                sample[1] = sequence->positions[1];
                sample[2] = sequence->positions[2];
            }
            else
            {
                sample[0] = sequence->values.singleValue;
                sample[1] = sample[0];
                sample[2] = sample[0];
            }

            attenuation = point->radius * point->radius * EQGFX_POINT_RADIUS_SCALE_GUESS;
            sample[0] = attenuation * sample[0];
            sample[1] = attenuation * sample[1];
            sample[2] = attenuation * sample[2];
            for (scratchRecord = renderer->scratchRecords; scratchRecord < recordEnd; scratchRecord = scratchRecord + 1)
            {
                scratchPosition = (float *)scratchRecord->positionBits;
                scratchNormal = (float *)scratchRecord->normalBits;
                deltaX = pointX - scratchPosition[0];
                deltaY = pointY - scratchPosition[1];
                deltaZ = pointZ - scratchPosition[2];
                distanceSquared = deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX;
                if (EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared)
                {
                    distanceFactor = EQGFX_DISTANCE_FACTOR_GUESS(distanceSquared);
                    forwardAmount = (deltaZ * scratchNormal[2] + deltaY * scratchNormal[1] + deltaX * scratchNormal[0]) * distanceFactor;
                    if (EQGFX_ZERO_F_GUESS < forwardAmount)
                    {
                        jitterBase = forwardAmount / (distanceSquared + EQGFX_FLOAT_ONE);

                        roundInput = jitterBase * sample[0] * EQGFX_JITTER_SCALE_GUESS;
                        __asm
                        {
                            fld roundInput
                        }
                        roundResult = EQGFX_FIX16_FROM_ST0();
                        scratchRecord->result[0] = scratchRecord->result[0] + roundResult;

                        roundInput = jitterBase * sample[1] * EQGFX_JITTER_SCALE_GUESS;
                        __asm
                        {
                            fld roundInput
                        }
                        roundResult = EQGFX_FIX16_FROM_ST0();
                        scratchRecord->result[1] = scratchRecord->result[1] + roundResult;

                        roundInput = jitterBase * sample[2] * EQGFX_JITTER_SCALE_GUESS;
                        __asm
                        {
                            fld roundInput
                        }
                        roundResult = EQGFX_FIX16_FROM_ST0();
                        scratchRecord->result[2] = scratchRecord->result[2] + roundResult;
                    }
                }
            }
        }
    }

    if (renderer->scratchRecords < recordEnd)
    {
        outputRecord = renderer->outputRecords;
        scratchRecord = renderer->scratchRecords;
        do
        {
            outputRecord->result[0] = scratchRecord->result[0];
            outputRecord->result[1] = scratchRecord->result[1];
            outputRecord->result[2] = scratchRecord->result[2];
            outputRecord->normalBits[0] = scratchRecord->normalBits[0];
            outputRecord->normalBits[1] = scratchRecord->normalBits[1];
            outputRecord->normalBits[2] = scratchRecord->normalBits[2];
            outputRecord->value0 = *recordValues;
            outputRecord->value1 = recordValues[1];
            recordValues = recordValues + 2;
            outputRecord = outputRecord + 1;
            scratchRecord = scratchRecord + 1;
        } while (scratchRecord < recordEnd);
    }
    return 1;
}

