// unit_1002EAB3.c - eqgfx_dx7.dll, the address run 0x1002EAB3..0x1002FE13 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"
#include <stdio.h>

#define T3D_CAMERA_HALF EQ_ARRAY(float, 0x10076290)
#define T3D_CAMERA_ONE EQ_ARRAY(float, 0x10076280)
#define T3D_CAMERA_TRIG_TABLE EQ_GLOBAL(float *, 0x100F3FF4)
#define T3D_CAMERA_TRIG_SCALE_OFFSET 0x608c
#define T3D_CAMERA_TRIG_SCALE_STRIDE 0x18
#define EQGFX_ROUND_ST0_TO_UINT_GUESS EQ_CDECL(unsigned int, 0x1004EC36, (void))
#define T3D_FLOAT_COSINE EQ_CDECL(float, 0x1004EEA3, (float))
#define T3D_FLOAT_SINE EQ_CDECL(float, 0x1004EE37, (float))

/* T3DDevice7_guess: the T3DDevice7_guess layout is T3DDevice7_guess (generated: eqgfx/layout/d3d_device7.h); the old local view's members map by offset. */
#include "eqgfx/layout/d3d_device7.h"

// Local layout guesses (no include/ struct has any of these layouts).

// The index list the third argument points at: a count at +0x4 and the array of indexes at +0x8.
typedef struct T3DIndexList_guess
{
    int unknown_0000; // 0x0
    int count;        // 0x4
    int *indexes;     // 0x8
} T3DIndexList_guess;

// One 0xc-byte source record: a point.
/* T3DPoint_guess: now in eqgfx/layout/t3dactor.h. */
#include "eqgfx/layout/t3dactor.h"

// One destination slot: the point plus 9 floats this function leaves alone; the slot is 0x30 bytes wide.
typedef struct T3DPointSlot_guess
{
    float x;          // 0x0
    float y;          // 0x4
    float z;          // 0x8
    float spare[9];   // 0xc
} T3DPointSlot_guess;

// The context the first argument points at: the freeable buffers freeRenderBuffers_guess releases sit at
// +0x2c0..+0x2cc (the +0x2bc field before them is only zeroed) and the destination slot array
// t3dCopyIndexedPointsToRenderSlots_guess fills is destSlots. _t3dRenderBlitSprite reads its third argument as this context too.
typedef struct T3DRenderContext_guess
{
    char unknown_0000[0x1ac];
    float origin[3];               // 0x1ac, added to each particle position
    char unknown_01b8[0x74];       // 0x1b8
    float transformZ0;             // 0x22c
    float unknown_0230[2];         // 0x230
    float transformZ1;             // 0x238
    float unknown_023c[2];         // 0x23c
    float transformZ2;             // 0x244
    float transformX[3];           // 0x248
    float transformY[3];           // 0x254
    char unknown_0260[0x5c];       // 0x260
    int unknown_2bc;               // 0x2bc
    void *renderBuffer_2c0;        // 0x2c0
    T3DPointSlot_guess *destSlots; // 0x2c4
    void *renderBuffer_2c8;        // 0x2c8
    void *renderBuffer_2cc;        // 0x2cc
    char unknown_02d0[0x110];      // 0x2d0
    unsigned int *indexTable;      // 0x3e0
    char unknown_03e4[0x1c];       // 0x3e4
    int pointIndex;                // 0x400
    char unknown_0404[0x124];      // 0x404
    float particleScaleX;          // 0x528
    float particleScaleY;          // 0x52c
    char lightPool[4];             // 0x530, the pool _t3dClipWorldSpaceSpherical searches (its size is unknown)
} T3DRenderContext_guess;

// The writer context the first argument of t3dWriteBytes_guess points at: the CRT stream being written to at +0x4.
typedef struct T3DWriteContext_guess
{
    int unknown_0000; // 0x0
    FILE *file;       // 0x4
} T3DWriteContext_guess;

// The actor the argument of t3dGetActorCurrentAction and t3dSetCameraAssociatedActor points at: the table
// index at +0x4 (the value a camera's actorIndex holds), the position at +0x10, three floats at +0x1c and the
// current action at +0x3c.
/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */

// The plane the first argument points at: the unit normal at +0x4..+0xc and the plane distance (normal . B) at +0x10.
typedef struct T3DPlane_guess
{
    int unknown_0000; // 0x0
    float normalX;    // 0x4
    float normalY;    // 0x8
    float normalZ;    // 0xc
    float distance;   // 0x10
} T3DPlane_guess;

// The DM sprite the argument of s3dGetDMSpriteDefinition points at: the definition pointer at +0x34.
typedef struct T3DDMSprite_guess
{
    char unknown_0000[0x34];
    void *definition; // 0x34
} T3DDMSprite_guess;

// The camera the argument of t3dGetCameraLocation, t3dGetCameraViewAngle, t3dGetCameraYon, t3dGetCameraRenderBufferPtr,
// t3dGetCameraRenderBufferWidth and t3dGetCameraRenderBufferHeight points at: an int at +0x4, the position at +0x8,
// three floats at +0x14, the view angle at +0x20, the yon distance at +0x30, the render buffer pointer at +0x44, the
// render buffer width at +0x48, the render buffer height at +0x4c and the associated actor index at +0x50.
// t3dSetCameraFrustum also reads the aspect at +0x24, the near distance at +0x28 and the 3x3 transform at +0x6c.
typedef struct T3DCamera_guess
{
    char unknown_0000[0x4];
    int unknown_0004;              // 0x4
    T3DPoint_guess position;       // 0x8
    float unknown_0014[3];         // 0x14
    float viewAngle;               // 0x20
    float aspect;                  // 0x24
    float nearDistance;            // 0x28
    float depthScale;              // 0x2c
    float yon;                     // 0x30
    void *imageBuffer;             // 0x34
    int imageWidth;                // 0x38
    int imageHeight;               // 0x3c
    int imageStride;               // 0x40
    void *renderBuffer;            // 0x44
    int renderBufferWidth;         // 0x48
    int renderBufferHeight;        // 0x4c
    int actorIndex;                // 0x50
    char unknown_0054[0x8];        // 0x54
    float worldToEyeScale;         // 0x5c
    float viewScale;               // 0x60
    float halfRenderBufferWidth;   // 0x64
    float halfRenderBufferHeight;  // 0x68
    float transform[3][3];         // 0x6c
    float horizontalCoefficients[3]; // 0x90
    float verticalCoefficients[3]; // 0x9c
    float horizontalPixelCoefficients[3]; // 0xa8
    float verticalPixelCoefficients[3]; // 0xb4
    float viewScaleCopy;           // 0xc0
    float worldToEyeScaleCopy;     // 0xc4
    float worldToEyeViewScaleProduct; // 0xc8
    float inverseWorldToEyeScaleSquared; // 0xcc
    float inverseViewScaleSquared; // 0xd0
    float frustumDistance;         // 0xd4
} T3DCamera_guess;

// The location the second argument of t3dGetCameraLocation points at: the camera's position at +0x0, three floats at +0xc
// and the camera's int at +0x18.
typedef struct T3DCameraLocation_guess
{
    T3DPoint_guess position; // 0x0
    float unknown_000c[3];   // 0xc
    int unknown_0018;        // 0x18
} T3DCameraLocation_guess;

// The camera actor the first argument of s3dSetCameraActorYon points at: the yon distance at +0xd4.
typedef struct T3DCameraActor_guess
{
    char unknown_0000[0xd4];
    float yon; // 0xd4
} T3DCameraActor_guess;

// The scene the first argument of t3dGetCameraAssociatedActor points at: the actor table at +0x17c, indexed by a camera's actor index.
typedef struct T3DScene_guess
{
    char unknown_0000[0x17c];
    T3D_tagACTORINSTANCE **actorTable; // 0x17c
} T3DScene_guess;

// The frustum the second argument of t3dSetCameraFrustum points at: eight corners of 0xc bytes at +0x0 (the four near
// corners, then the four far ones) and six planes of 0x14 bytes at +0x60.
typedef struct T3DFrustum_guess
{
    T3DPoint_guess corners[8]; // 0x0
    T3DPlane_guess planes[6];  // 0x60
} T3DFrustum_guess;

// One 0x10-byte slot of the deferred line table at 0x100ea790 (2000 of them): a queued line is the two
// vertices from firstVertex on.
typedef struct T3DDeferredLine_guess
{
    int inUse;        // 0x0, 1 once queued
    int firstVertex;  // 0x4
    int unknown_0008; // 0x8, not written by t3dDeferLine
    int unknown_000c; // 0xc, 0 once queued
} T3DDeferredLine_guess;

// One 0x20-byte vertex of the deferred vertex table at 0x100abf90 (8000 of them): x and y are raw dwords
// copied from the points; the rest is the transformed-vertex layout.
typedef struct T3DVertex_guess
{
    unsigned int x;        // 0x0
    unsigned int y;        // 0x4
    float z;               // 0x8
    float rhw;             // 0xc
    unsigned int color;    // 0x10
    unsigned int specular; // 0x14
    float tu;              // 0x18
    float tv;              // 0x1c
} T3DVertex_guess;

// The particle the first argument of _t3dRenderBlitSprite points at: its material id at +0x14 (bit 31 set: an
// index into the context's table at +0x3e0) and its frame cycle at +0x68.
typedef struct T3DParticle_guess
{
    char unknown_0000[0x14];
    unsigned int materialId;              // 0x14
    char unknown_0018[0x50];              // 0x18
    struct T3DParticleCycle_guess *cycle; // 0x68
} T3DParticle_guess;

// The frame cycle at +0x68 of a particle: the current frame pointer at +0x14, the frame index at +0x18, the frame duration
// at +0x1c, the time the next frame is due at +0x20, a flag at +0x24 (0: one frame per step, else skip by the elapsed time)
// and the frame sequence at +0x28.
typedef struct T3DParticleCycle_guess
{
    char unknown_0000[0x14];
    unsigned int *currentFrame;              // 0x14
    unsigned int frameIndex;                 // 0x18
    unsigned int frameDuration;              // 0x1c
    unsigned int nextFrameTime;              // 0x20
    int skipFrames;                          // 0x24
    struct T3DFrameSequence_guess *sequence; // 0x28
} T3DParticleCycle_guess;

// The frame sequence at +0x28 of a cycle: the frame count at +0x14 and the table of frame pointers at +0x18.
typedef struct T3DFrameSequence_guess
{
    char unknown_0000[0x14];
    unsigned int frameCount; // 0x14
    unsigned int **frames;   // 0x18
} T3DFrameSequence_guess;

// The clock the second argument points at: the current time at +0x8.
typedef struct T3DFrameClock_guess
{
    char unknown_0000[0x8];
    unsigned int currentTime; // 0x8
} T3DFrameClock_guess;

// The record the fourth argument points at: three colour bytes at +0x14..+0x16 and the first dword of the cycle's current
// frame at +0x1c.
typedef struct T3DParticleRecord_guess
{
    char unknown_0000[0x14];
    unsigned char color[3]; // 0x14
    char unknown_0017[0x5]; // 0x17
    unsigned int frameId;   // 0x1c
} T3DParticleRecord_guess;

// The deferred line table holds 2000 lines and the vertex table 8000 vertices.
#define T3D_DEFER_LINE_MAX 2000
#define T3D_DEFER_VERTEX_MAX 8000

// The sine and cosine tables at the pointer stored at 0x100f3ff4: 0x200 floats each, cosine at +0x1000 and sine at +0x1800.
// The index is the rounded rotation plus a quarter turn (0x40), wrapped to 0x200.
#define T3D_TRIG_COUNT 0x200
#define T3D_TRIG_PHASE 0x40
#define T3D_TRIG_COS_OFFSET 0x1000
#define T3D_TRIG_SIN_OFFSET 0x1800
// Particle material ids: bit 31 selects an entry of the context's table at +0x3e0, bit 30 is passed on as a draw flag.
#define T3D_PARTICLE_INDIRECT 0x80000000
#define T3D_PARTICLE_DRAW_FLAG 0x40000000

// FUNCTION: EQGFX 0x1002eab3
// Writes an integer in a compact format to the data stream.
// __cdecl (two stack arguments, no value back; ECX is no input): writes the signed `value` as a T3D
// variable-length integer through t3dWriteBytes_guess - one byte below 0x80, else 0x80|high byte then low byte.
void __cdecl t3dWriteVarInt_guess(T3DWriteContext_guess *context, int value)
{
    if (value < 0x80)
    {
        ((unsigned char *)&value)[2] = (unsigned char)value;
        EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(
            context, (unsigned char *)&value + 2, 1);
    }
    else
    {
        ((unsigned char *)&value)[2] = (unsigned char)(value / 0x100) + 0x80;
        ((unsigned char *)&value)[3] = (unsigned char)(value % 0x100);
        EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(
            context, (unsigned char *)&value + 2, 2);
    }
}

// FUNCTION: EQGFX 0x1002eb07
// Writes a width-tagged integer field in T3D data.
// __cdecl, two stack arguments (a write context, a value) and zero back: takes the value's encoded width
// from t3dGetSignedValueByteWidth_guess, writes the width tag 8/9/10 through t3dWriteVarInt_guess, then that many bytes of the value
// through t3dWriteValueBytes_guess, and reports 0.
#pragma optimize("y", off) // the original keeps a frame pointer here
int __cdecl t3dWriteTaggedInteger_guess(T3DWriteContext_guess *context, int value)
{
    int width = EQ_CDECL(int, 0x1002EBD0, (int))(value);

    switch (width)
    {
    case 1:
        EQ_CDECL(void, 0x1002EAB3, (T3DWriteContext_guess *, int))(context, 8);
        break;
    case 2:
        EQ_CDECL(void, 0x1002EAB3, (T3DWriteContext_guess *, int))(context, 9);
        break;
    case 3:
        width = 4;
    default:
        EQ_CDECL(void, 0x1002EAB3, (T3DWriteContext_guess *, int))(context, 10);
        break;
    }
    EQ_CDECL(int, 0x1002EBF3, (T3DWriteContext_guess *, int, size_t))(context, value, width);
    return 0;
}
#pragma optimize("y", on)

// FUNCTION: EQGFX 0x1002eb4a
// Writes tag 0xc and a 4-byte value (callers pass floats: positions, world Y) to T3D data.
// __cdecl, two stack arguments (a write context, a value) and zero back: writes the variable-length
// integer 0xc through t3dWriteVarInt_guess, then the four bytes of `value` through t3dWriteBytes_guess, and reports 0.
int __cdecl t3dWriteTaggedFloat_guess(T3DWriteContext_guess *context, int value)
{
    EQ_CDECL(void, 0x1002EAB3, (T3DWriteContext_guess *, int))(context, 0xc);
    EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(context, &value, 4);
    return 0;
}

// FUNCTION: EQGFX 0x1002eb6b
// Writes a tag-4 byte payload in the T3D data format.
// __cdecl, three stack arguments and a value back (ECX is no input): writes the variable-length integer 4
// through t3dWriteVarInt_guess, then the `size` bytes of `data` through t3dWriteXorEncodedData_guess, and reports 0.
int __cdecl t3dWriteTag4Data_guess(T3DWriteContext_guess *context, unsigned char *data, int size)
{
    EQ_CDECL(void, 0x1002EAB3, (T3DWriteContext_guess *, int))(context, 4);
    EQ_CDECL(int, 0x1002EC0B, (T3DWriteContext_guess *, unsigned char *, int))(context, data, size);
    return 0;
}

// FUNCTION: EQGFX 0x1002eb8d
// Writes a size-tagged byte payload in the T3D data format.
// __cdecl, three stack arguments and a value back (ECX is no input): writes the variable-length integer
// 0x18, or 0x98 once `size` reaches 0x100, through t3dWriteVarInt_guess, then the `size` bytes of `data` through
// t3dWriteXorEncodedData_guess, and reports 0.
int __cdecl t3dWriteSizeTaggedData_guess(T3DWriteContext_guess *context, unsigned char *data, int size)
{
    EQ_CDECL(void, 0x1002EAB3, (T3DWriteContext_guess *, int))(context, ((size >= 0x100) - 1 & 0xffffff80) + 0x98);
    EQ_CDECL(int, 0x1002EC0B, (T3DWriteContext_guess *, unsigned char *, int))(context, data, size);
    return 0;
}

// FUNCTION: EQGFX 0x1002ebc2
// __cdecl, one stack argument and nothing back: writes the variable-length integer 3 through t3dWriteVarInt_guess.
void __cdecl t3dWriteMarker3_guess(T3DWriteContext_guess *context)
{
    EQ_CDECL(void, 0x1002EAB3, (T3DWriteContext_guess *, int))(context, 3);
}

// FUNCTION: EQGFX 0x1002ebd0
// Chooses one, two, or four bytes to represent a signed integer.
// __cdecl, one stack argument and a width back: 1 when the value fits a signed char, else 2 when it fits a
// signed short, else 4 - the number of bytes this value occupies in the writer's encoding.
int __cdecl t3dGetSignedValueByteWidth_guess(int value)
{
    if (value == (char)value)
    {
        return 1;
    }
    return ((value != (short)value) - 1 & 0xfffffffe) + 4;
}

// FUNCTION: EQGFX 0x1002ebf3
// Writes the selected bytes of an integer field to T3D data.
// __cdecl, three stack arguments and a value back (ECX is no input): writes `count` bytes of the second
// parameter `value` through t3dWriteBytes_guess - the parameter's own stack slot is the data - and reports 0.
int __cdecl t3dWriteValueBytes_guess(T3DWriteContext_guess *context, int value, size_t count)
{
    EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(context, &value, count);
    return 0;
}

#define T3D_HUNK_KEY_GUESS EQ_ARRAY(unsigned char, 0x100762C0) // T3D_HUNK_KEY_GUESS: the 8 payload XOR bytes

// FUNCTION: EQGFX 0x1002ec0b
// Applies the repeating payload key used by T3D data records.
// __cdecl (three stack arguments and a value back; ECX is no input): writes `size` as its low byte plus a
// second byte when the quotient is nonzero, then the `size` bytes of `data` XORed with the 8-byte key above.
int __cdecl t3dWriteXorEncodedData_guess(T3DWriteContext_guess *context, unsigned char *data, int size)
{
    unsigned char buffer[8];
    int remaining;
    unsigned int count;

    remaining = size;
    count = 0;
    ((unsigned char *)&size)[3] = (unsigned char)(remaining % 0x100);
    EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(
        context, (unsigned char *)&size + 3, 1);
    ((unsigned char *)&size)[3] = (unsigned char)(remaining / 0x100);
    if (((unsigned char *)&size)[3] != 0)
    {
        EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(
            context, (unsigned char *)&size + 3, 1);
    }
    while (remaining-- > 0)
    {
        buffer[count] = T3D_HUNK_KEY_GUESS[count] ^ *data;
        data++;
        count++;
        if (count >= 8)
        {
            EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(
                context, buffer, 8);
            count = 0;
        }
    }
    if ((int)count > 0)
    {
        EQ_CDECL(unsigned int, 0x1002ECA4, (T3DWriteContext_guess *, void *, size_t))(context, buffer, count);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002eca4
// Writes raw data bytes to the active output stream.
// __cdecl, three stack arguments and a value back (ECX is no input): writes `count` bytes of `data` to the CRT
// stream at context->file, returning 0xFFFF3D3E when _fwrite takes fewer than `count` of them and 0 otherwise.
unsigned int __cdecl t3dWriteBytes_guess(T3DWriteContext_guess *context, void *data, size_t count)
{
    size_t written;

    written = EQ_CDECL(size_t, 0x10069A2A, (void *, size_t, size_t, FILE *))(data, 1, count, context->file);
    return written != count ? 0xFFFF3D3E : 0;
}

// FUNCTION: EQGFX 0x1002ecca
// Adds a filled colored rectangle to the later 2D draw pass.
// __cdecl (three stack arguments, callee pops 0 bytes; ECX is no input): queues one quad, a line record (inUse 2) and four
// vertices (x and y raw from the points, z 0, rhw 1, colour = the alpha picked by flags | the 24 bits at color). Returns -1 when the line table is full, else 0.
int __cdecl t3dDefer2DQuad(unsigned int flags, unsigned char *color, unsigned int *points)
{
    int lineIndex;
    int vertexIndex;
    int remaining;
    unsigned int argb;

    lineIndex = EQ_GLOBAL(int, 0x100F2494);
    if (lineIndex >= T3D_DEFER_LINE_MAX)
        return -1;
    vertexIndex = EQ_GLOBAL(int, 0x100F2490);
    EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790)[lineIndex].inUse = 2;
    EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790)[lineIndex].firstVertex = vertexIndex;
    EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790)[lineIndex].unknown_000c = 0;
    argb = ((color[2] << 8) | color[1]) << 8 | color[0];
    if ((flags & 0x10C0000) == 0x10C0000)
        argb |= 0xCF000000;
    else if ((flags & 0x1080000) == 0x1080000)
        argb |= 0x7F000000;
    else if ((flags & 0x1040000) == 0x1040000)
        argb |= 0x3F000000;
    else
        argb |= 0xFF000000;
    remaining = 4;
    do
    {
        if (vertexIndex < T3D_DEFER_VERTEX_MAX)
        {
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].x = points[0];
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].y = points[1];
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].z = 0.0f;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].rhw = 1.0f;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].color = argb;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].specular = 0;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].tu = 0.0f;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].tv = 0.0f;
        }
        vertexIndex++;
        points += 3;
        remaining--;
        EQ_GLOBAL(int, 0x100F2490) = vertexIndex;
    } while (remaining != 0);
    EQ_GLOBAL(int, 0x100F2494)++;
    return 0;
}

// FUNCTION: EQGFX 0x1002edc5
// Adds a colored 2D line to the later draw pass.
// __cdecl (two stack arguments, callee pops 0 bytes; ECX is no input): queues one line, its line record and then two
// vertices (x and y raw from the points, z 0, rhw 1, colour 0xFF000000 | the low 24 bits of color). Returns -1 when the line table is full, else 0.
int __cdecl t3dDeferLine(unsigned int *points, unsigned int color)
{
    int lineIndex;
    int vertexIndex;
    int remaining;
    unsigned int argb;

    lineIndex = EQ_GLOBAL(int, 0x100F2494);
    if (lineIndex >= T3D_DEFER_LINE_MAX)
        return -1;
    vertexIndex = EQ_GLOBAL(int, 0x100F2490);
    EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790)[lineIndex].inUse = 1;
    EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790)[lineIndex].firstVertex = vertexIndex;
    EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790)[lineIndex].unknown_000c = 0;
    argb = (((unsigned char)(color >> 16) | 0xffffff00) << 8 | (unsigned char)(color >> 8)) << 8 | (unsigned char)color;
    remaining = 2;
    do
    {
        if (vertexIndex < T3D_DEFER_VERTEX_MAX)
        {
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].x = points[0];
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].y = points[1];
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].z = 0.0f;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].rhw = 1.0f;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].color = argb;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].specular = 0;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].tu = 0.0f;
            EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[vertexIndex].tv = 0.0f;
        }
        vertexIndex++;
        points += 3;
        remaining--;
        EQ_GLOBAL(int, 0x100F2490) = vertexIndex;
    } while (remaining != 0);
    EQ_GLOBAL(int, 0x100F2494)++;
    return 0;
}

#define T3D_RENDER_STATE_ZENABLE_GUESS 7
#define T3D_RENDER_STATE_ZWRITEENABLE_GUESS 14
#define T3D_RENDER_STATE_SOURCEBLEND_GUESS 19
#define T3D_RENDER_STATE_DESTBLEND_GUESS 20
#define T3D_RENDER_STATE_TEXTUREPERSPECTIVE_GUESS 0x1b
#define T3D_D3DFVF_TLVERTEX_GUESS 0x1c4

// FUNCTION: EQGFX 0x1002ee82
// Displays the pending 2D lines and rectangles during scene rendering.
int __cdecl t3dRenderDeferredLines_guess(void)
{
    typedef void (__stdcall *D3DDev7SetRenderStateLocalFn)(T3DDevice7_guess *, int, int);
    typedef void (__stdcall *DeviceDrawPrimitiveFn_guess)(T3DDevice7_guess *, int, int, void *, int, int);
    typedef void (__stdcall *GrFogModeFn_guess)(int);
    typedef void (__stdcall *GrDepthBufferModeFn_guess)(int);
    typedef void (__stdcall *GrAlphaBlendFunctionFn_guess)(int, int, int, int);
    typedef void (__stdcall *GrAlphaCombineFn_guess)(int, int, int, int, int);
    typedef void (__stdcall *GrDrawLineFn_guess)(void *, void *);
    typedef void (__stdcall *GrDrawVertexArrayFn_guess)(int, int, void **);
    int renderMode;
    int lastTexture;
    int recordIndex;
    T3DDeferredLine_guess *line;
    T3DDevice7_guess *device;
    void *drawVertices[4];

    if (EQ_GLOBAL(int, 0x100F2494) > 0)
    {
        EQ_CDECL(void, 0x10045ED4, (int))(0);
        renderMode = EQ_GLOBAL(int, 0x100FB8A8);
        if (renderMode == 1)
        {
            EQ_CDECL(void, 0x1004A23B, (int))(0);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_ZENABLE_GUESS, 0);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_ZWRITEENABLE_GUESS, 0);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_TEXTUREPERSPECTIVE_GUESS, 1);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_SOURCEBLEND_GUESS, 5);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_DESTBLEND_GUESS, 6);
            EQ_CDECL(void, 0x1004A25E, (int))(EQ_GLOBAL(int, 0x100FBBA0));
            lastTexture = 0;
            recordIndex = 0;
            if (0 < EQ_GLOBAL(int, 0x100F2494))
            {
                line = EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790);
                do
                {
                    if (lastTexture != line->unknown_000c)
                    {
                        EQ_CDECL(void, 0x10045ED4, (int))(line->unknown_000c);
                        lastTexture = line->unknown_000c;
                    }
                    switch (line->inUse)
                    {
                    case 1:
                        device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
                        ((DeviceDrawPrimitiveFn_guess)device->vtable[25])(
                            device, 2, T3D_D3DFVF_TLVERTEX_GUESS,
                            &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex], 2, 0);
                        break;
                    case 2:
                        device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
                        ((DeviceDrawPrimitiveFn_guess)device->vtable[25])(
                            device, 5, T3D_D3DFVF_TLVERTEX_GUESS,
                            &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex], 4, 0);
                        break;
                    }
                    recordIndex++;
                    line++;
                } while (recordIndex < EQ_GLOBAL(int, 0x100F2494));
            }
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_SOURCEBLEND_GUESS, 2);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_DESTBLEND_GUESS, 1);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_TEXTUREPERSPECTIVE_GUESS, 0);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_ZENABLE_GUESS, 1);
            device = EQ_GLOBAL(T3DDevice7_guess *, 0x100FBA6C);
            ((D3DDev7SetRenderStateLocalFn)device->vtable[20])(
                device, T3D_RENDER_STATE_ZWRITEENABLE_GUESS, 1);
            if (EQ_GLOBAL(int, 0x100FBA1C) != 0)
                EQ_CDECL(void, 0x1004A23B, (int))(1);
            EQ_CDECL(void, 0x1004A25E, (int))(EQ_GLOBAL(int, 0x100FBB9C));
        }
        else if (renderMode == 2)
        {
            ((GrFogModeFn_guess)EQ_GLOBAL(void *, 0x100F3EFC))(0);
            ((GrDepthBufferModeFn_guess)EQ_GLOBAL(void *, 0x100F3EBC))(0);
            ((GrAlphaBlendFunctionFn_guess)EQ_GLOBAL(void *, 0x100F3E78))(1, 5, 0, 0);
            ((GrAlphaCombineFn_guess)EQ_GLOBAL(void *, 0x100F3E7C))(1, 8, 0, 2, 0);
            lastTexture = 0;
            recordIndex = 0;
            if (0 < EQ_GLOBAL(int, 0x100F2494))
            {
                line = EQ_ARRAY(T3DDeferredLine_guess, 0x100EA790);
                do
                {
                    if (lastTexture != line->unknown_000c)
                    {
                        EQ_CDECL(void, 0x10045ED4, (int))(line->unknown_000c);
                        lastTexture = line->unknown_000c;
                    }
                    switch (line->inUse)
                    {
                    case 1:
                        ((GrDrawLineFn_guess)EQ_GLOBAL(void *, 0x100F3ED4))(
                            &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex],
                            &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex + 1]);
                        break;
                    case 2:
                        drawVertices[0] = &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex];
                        drawVertices[1] = &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex + 1];
                        drawVertices[2] = &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex + 2];
                        drawVertices[3] = &EQ_ARRAY(T3DVertex_guess, 0x100ABF90)[line->firstVertex + 3];
                        ((GrDrawVertexArrayFn_guess)EQ_GLOBAL(void *, 0x100F3EE0))(4, 4, drawVertices);
                        break;
                    }
                    recordIndex++;
                    line++;
                } while (recordIndex < EQ_GLOBAL(int, 0x100F2494));
            }
            ((GrDepthBufferModeFn_guess)EQ_GLOBAL(void *, 0x100F3EBC))(2);
            if (EQ_GLOBAL(int, 0x100FBA1C) != 0)
                ((GrFogModeFn_guess)EQ_GLOBAL(void *, 0x100F3EFC))(2);
        }
        EQ_GLOBAL(int, 0x100F2494) = 0;
        EQ_GLOBAL(int, 0x100F2490) = 0;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002f0eb
// Builds a rotated particle billboard at a world position for scene drawing.
// __cdecl (seven stack arguments, callee pops 0 bytes; ECX is no input): places one point light particle as four 0x30-byte slots of destSlots.
// rotation goes to t3dFloatTruncateToInt_guess in ST0 (popped there) and picks the sin/cos entries; each slot's z is the centre's z. Returns 0.
int __cdecl _t3dRenderBlitSprite(T3DParticle_guess *particle, T3DFrameClock_guess *clock, T3DRenderContext_guess *context, T3DParticleRecord_guess *record, float *position, float rotation, float radius)
{
    int pointIndex;
    unsigned int materialId;
    T3DParticleCycle_guess *cycle;
    T3DFrameSequence_guess *sequence;
    unsigned int frameDuration;
    unsigned int elapsed;
    unsigned int framesSkipped;
    int roundedRotation;
    unsigned int tableIndex;
    float scaleX;
    float scaleY;
    float pointX;
    float pointY;
    float pointZ;
    double centerX;
    double centerY;
    double centerZ;
    float cosValue;
    float sinValue;
    double cosScaleX;
    double sinScaleX;
    double sinScaleY;
    double cosScaleY;
    volatile float sinScaleXRounded;
    volatile float sinScaleYRounded;
    volatile float cosScaleYRounded;
    T3DPointSlot_guess *slots;

    pointIndex = EQ_CDECL(int, 0x10030DAA, (void *, float *, float))(context->lightPool, position, (float)(radius * EQ_GLOBAL(float, 0x100762D0)));
    if (pointIndex != -1)
    {
        context->pointIndex = pointIndex;
        materialId = particle->materialId;
        if ((materialId & T3D_PARTICLE_INDIRECT) != 0)
            materialId = context->indexTable[materialId & ~T3D_PARTICLE_INDIRECT];
        cycle = particle->cycle;
        sequence = cycle->sequence;
        if (sequence->frameCount > 1)
        {
            if (cycle->nextFrameTime < clock->currentTime)
            {
                if (cycle->skipFrames == 0)
                {
                    cycle->frameIndex++;
                    if (sequence->frameCount <= cycle->frameIndex)
                        cycle->frameIndex = 0;
                    cycle->nextFrameTime = cycle->frameDuration + clock->currentTime;
                }
                else
                {
                    frameDuration = cycle->frameDuration;
                    elapsed = clock->currentTime - cycle->nextFrameTime;
                    if ((frameDuration << 4) < elapsed)
                    {
                        framesSkipped = elapsed / frameDuration;
                        cycle->frameIndex += framesSkipped + 1;
                        frameDuration = cycle->frameDuration;
                        elapsed -= framesSkipped * frameDuration;
                        cycle->frameIndex %= sequence->frameCount;
                    }
                    else
                    {
                        while (frameDuration < elapsed)
                        {
                            cycle->frameIndex++;
                            if (sequence->frameCount <= cycle->frameIndex)
                                cycle->frameIndex = 0;
                            elapsed -= frameDuration;
                        }
                        cycle->frameIndex++;
                        if (sequence->frameCount <= cycle->frameIndex)
                            cycle->frameIndex = 0;
                    }
                    cycle->nextFrameTime = (clock->currentTime - elapsed) + frameDuration;
                }
                cycle->currentFrame = sequence->frames[cycle->frameIndex];
            }
        }
        record->frameId = *cycle->currentFrame;
        scaleX = radius * context->particleScaleX;
        scaleY = radius * context->particleScaleY;
        pointX = context->origin[0] + position[0];
        pointY = context->origin[1] + position[1];
        pointZ = context->origin[2] + position[2];
        centerX = context->transformX[2] * pointZ + context->transformX[1] * pointY + context->transformX[0] * pointX;
        centerY = context->transformY[2] * pointZ + context->transformY[1] * pointY + context->transformY[0] * pointX;
        centerZ = context->transformZ2 * pointZ + context->transformZ1 * pointY + context->transformZ0 * pointX;
        __asm fld DWORD PTR rotation
        roundedRotation = EQ_CDECL(int, 0x1004EC36, (void))();
        tableIndex = roundedRotation + T3D_TRIG_PHASE;
        if ((int)tableIndex >= T3D_TRIG_COUNT)
            tableIndex -= (tableIndex >> 9) * T3D_TRIG_COUNT;
        sinValue = *(float *)(EQ_GLOBAL(char *, 0x100F3FF4) + T3D_TRIG_SIN_OFFSET + tableIndex * 4);
        cosValue = *(float *)(EQ_GLOBAL(char *, 0x100F3FF4) + T3D_TRIG_COS_OFFSET + tableIndex * 4);
        cosScaleX = cosValue * scaleX;
        sinScaleX = sinValue * scaleX;
        sinScaleY = sinValue * scaleY;
        cosScaleY = cosValue * scaleY;
        sinScaleXRounded = sinScaleX;
        sinScaleYRounded = sinScaleY;
        cosScaleYRounded = cosScaleY;
        slots = context->destSlots;
        slots[0].x = (float)(cosScaleX + centerX);
        slots[0].y = (float)(sinScaleY + centerY);
        slots[0].z = (float)centerZ;
        slots[0].spare[1] = 1.0f;
        slots[0].spare[2] = 1.0f;
        *(unsigned int *)&slots[0].spare[6] = record->color[2];
        *(unsigned int *)&slots[0].spare[7] = record->color[1];
        *(unsigned int *)&slots[0].spare[8] = record->color[0];
        slots[1].x = (float)(sinScaleX + centerX);
        slots[1].y = (float)(centerY - cosScaleY);
        slots[1].z = (float)centerZ;
        slots[1].spare[1] = 1.0f;
        slots[1].spare[2] = 0.0f;
        *(unsigned int *)&slots[1].spare[6] = record->color[2];
        *(unsigned int *)&slots[1].spare[7] = record->color[1];
        *(unsigned int *)&slots[1].spare[8] = record->color[0];
        slots[2].x = (float)(centerX - cosScaleX);
        slots[2].y = (float)(centerY - sinScaleYRounded);
        slots[2].z = (float)centerZ;
        slots[2].spare[1] = 0.0f;
        slots[2].spare[2] = 0.0f;
        *(unsigned int *)&slots[2].spare[6] = record->color[2];
        *(unsigned int *)&slots[2].spare[7] = record->color[1];
        *(unsigned int *)&slots[2].spare[8] = record->color[0];
        slots[3].x = (float)(centerX - sinScaleXRounded);
        slots[3].y = (float)(cosScaleYRounded + centerY);
        slots[3].z = (float)centerZ;
        slots[3].spare[1] = 0.0f;
        slots[3].spare[2] = 1.0f;
        *(unsigned int *)&slots[3].spare[6] = record->color[2];
        *(unsigned int *)&slots[3].spare[7] = record->color[1];
        *(unsigned int *)&slots[3].spare[8] = record->color[0];
        EQ_CDECL(void, 0x100477B9, (T3DRenderContext_guess *, int, int, T3DParticleRecord_guess *, unsigned int))(context, 0, 4, record, materialId | T3D_PARTICLE_DRAW_FLAG);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002f3f9
// Paints the context particle at the selected scene actor's position.
// __cdecl (three stack arguments, callee pops 0 bytes; ECX is no input): places the context's particle at the actor the index at actorIndex selects from the scene's actor table; returns what _t3dRenderBlitSprite returns.
int __cdecl _t3dPaintBlitSprite(T3DScene_guess *scene, T3DRenderContext_guess *context, int *actorIndex)
{
    T3D_tagACTORINSTANCE *actor = scene->actorTable[*actorIndex];
    T3DParticle_guess *particle = *(T3DParticle_guess **)((char *)context + 0x3d8);

    return EQ_CDECL(int, 0x1002F0EB, (T3DParticle_guess *, T3DFrameClock_guess *, T3DRenderContext_guess *, T3DParticleRecord_guess *, float *, float, float))(
        particle, (T3DFrameClock_guess *)scene, context, (T3DParticleRecord_guess *)particle->unknown_0018, &actor->point_guess.x, actor->orientation_guess[2], *(float *)((char *)actor + 0x34));
}

// FUNCTION: EQGFX 0x1002f43a
// Allocates a camera object for scene viewing.
// __cdecl (no stack arguments; ECX is no input): allocates a 0xd8-byte camera block through t3dAllocateMemory and stores 0x39 at its first dword.
// Clears the float at +0xd4 (frustumDistance); returns nothing, EAX still holds the block.
void __cdecl t3dCreateCamera(void)
{
    T3DCamera_guess *camera;

    camera = (T3DCamera_guess *)EQ_CDECL(void *, 0x10050C48, (size_t))(0xd8);
    *(int *)camera->unknown_0000 = 0x39;
    camera->frustumDistance = 0.0f;
}

// FUNCTION: EQGFX 0x1002f454
// Releases a camera object.
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): frees the camera block through t3dFreeMemory and returns 0.
int __cdecl t3dDestroyCamera(void *camera)
{
    EQ_CDECL(void, 0x10050CBB, (void *))(camera);
    return 0;
}

// FUNCTION: EQGFX 0x1002f461
// Finds the scene actor linked to a camera.
// __cdecl (two stack arguments, callee pops 0 bytes; ECX is no input): the actor the camera's actor index at +0x50 selects from the scene's actor table at +0x17c; 0 for a negative index.
T3D_tagACTORINSTANCE *__cdecl t3dGetCameraAssociatedActor(T3DScene_guess *scene, T3DCamera_guess *camera)
{
    if (camera->actorIndex < 0)
        return 0;
    return scene->actorTable[camera->actorIndex];
}

// FUNCTION: EQGFX 0x1002f47d
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): the 32-bit field at actor+0x3c.
int __cdecl t3dGetActorCurrentAction(T3D_tagACTORINSTANCE *actor)
{
    return actor->currentAction_int_guess;
}

// FUNCTION: EQGFX 0x1002f485
// Returns the definition linked to a DM sprite.
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): the pointer at sprite+0x34, the sprite's definition.
void *__cdecl s3dGetDMSpriteDefinition(T3DDMSprite_guess *sprite)
{
    return sprite->definition;
}

// FUNCTION: EQGFX 0x1002f48d
// Copies a camera's position and related location data into a location record.
// __cdecl (two stack arguments, callee pops 0 bytes; ECX is no input): copies the camera's int, position and three floats into the location; returns 0.
int __cdecl t3dGetCameraLocation(T3DCamera_guess *camera, T3DCameraLocation_guess *location)
{
    location->unknown_0018 = camera->unknown_0004;
    location->position = camera->position;
    location->unknown_000c[0] = camera->unknown_0014[0];
    location->unknown_000c[1] = camera->unknown_0014[1];
    location->unknown_000c[2] = camera->unknown_0014[2];
    return 0;
}

// FUNCTION: EQGFX 0x1002f4bc
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): the render buffer height at camera+0x4c.
int __cdecl t3dGetCameraRenderBufferHeight(T3DCamera_guess *camera)
{
    return camera->renderBufferHeight;
}

// FUNCTION: EQGFX 0x1002f4c4
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): the render buffer pointer at camera+0x44.
void *__cdecl t3dGetCameraRenderBufferPtr(T3DCamera_guess *camera)
{
    return camera->renderBuffer;
}

// FUNCTION: EQGFX 0x1002f4cc
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): the render buffer width at camera+0x48.
int __cdecl t3dGetCameraRenderBufferWidth(T3DCamera_guess *camera)
{
    return camera->renderBufferWidth;
}

// FUNCTION: EQGFX 0x1002f4d4
// Reports the camera's full field of view.
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): twice the camera's view angle at camera+0x20, returned on the x87 stack.
float __cdecl t3dGetCameraViewAngle(T3DCamera_guess *camera)
{
    return camera->viewAngle + camera->viewAngle;
}

// FUNCTION: EQGFX 0x1002f4de
// Reports the camera's view distance limit.
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input): the camera's yon distance at camera+0x30, returned on the x87 stack.
float __cdecl t3dGetCameraYon(T3DCamera_guess *camera)
{
    return camera->yon;
}

// FUNCTION: EQGFX 0x1002f4e6
// Links a camera to the scene actor associated with its viewpoint.
// __cdecl (two stack arguments, callee pops 0 bytes; ECX is no input): stores the actor's index at +0x4 in camera+0x50, or -1 for a null actor (the original ORs it in), and returns 0.
int __cdecl t3dSetCameraAssociatedActor(T3DCamera_guess *camera, T3D_tagACTORINSTANCE *actor)
{
    if (actor == 0)
    {
        camera->actorIndex |= -1;
    }
    else
    {
        camera->actorIndex = actor->listIndex_guess;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002f505
// Sets the camera's image destination and its size for rendering.
// __cdecl (four stack arguments, callee pops 0 bytes): stores the buffer, width and height at camera+0x34, +0x38 and +0x3c, the width again at +0x40, and returns 0, or -1 for a null camera.
int __cdecl t3dSetCameraImageBuffer(T3DCamera_guess *camera, void *buffer, int width, int height)
{
    int result;

    result = 0;
    if (camera == 0)
    {
        result = -1;
        return result;
    }
    camera->imageBuffer = buffer;
    camera->imageWidth = width;
    camera->imageStride = width;
    camera->imageHeight = height;
    return result;
}

// FUNCTION: EQGFX 0x1002f52c
// Sets the camera's scene render surface and its dimensions.
// __cdecl (four stack arguments, callee pops 0 bytes): stores the buffer, width and height at camera+0x44, +0x48 and +0x4c, and returns 0, or -1 for a null camera.
int __cdecl t3dSetCameraRenderBuffer(T3DCamera_guess *camera, void *buffer, int width, int height)
{
    int result;

    result = 0;
    if (camera == 0)
    {
        result = -1;
        return result;
    }
    camera->renderBuffer = buffer;
    camera->renderBufferWidth = width;
    camera->renderBufferHeight = height;
    return result;
}

// FUNCTION: EQGFX 0x1002f550
// Sets how far the camera actor can see.
// __cdecl (two stack arguments, callee pops 0 bytes; ECX is no input): stores `yon` at actor+0xd4 and returns 0, or -1 for a null actor.
int __cdecl s3dSetCameraActorYon(T3DCameraActor_guess *actor, float yon)
{
    int result;

    result = 0;
    if (actor == 0)
    {
        result = -1;
        return result;
    }
    actor->yon = yon;
    return result;
}

// FUNCTION: EQGFX 0x1002f569
// Sets the camera's field of view and near and far viewing range.
// __cdecl (five stack arguments, callee pops 0 bytes; ECX is no input): stores the aspect, the near distance, half the view angle, yon and the depth scale 20000/(20000-near) in the camera, and returns 0, -1 for a null camera or -2 when near >= yon.
int __cdecl t3dSetCameraLens(T3DCamera_guess *camera, float fullViewAngle, float aspect, float nearDistance, float yon)
{
    int result;

    result = 0;
    if (camera == 0)
    {
        result = -1;
    }
    else if (nearDistance >= yon)
    {
        result = -2;
    }
    else
    {
        camera->viewAngle = fullViewAngle * 0.5f;
        camera->aspect = aspect;
        camera->nearDistance = nearDistance;
        camera->yon = yon;
        camera->depthScale = 20000.0f / (20000.0f - nearDistance);
    }
    return result;
}

// FUNCTION: EQGFX 0x1002f5be
// Places the camera at a world location and applies that location's viewing orientation.
// __cdecl (three stack arguments, callee pops 0 bytes; ECX is no input): stores the region of the location's point in the camera's int at +0x4, copies the point to the camera's position and the location's three floats at +0xc to the camera's three floats at +0x14, and returns 0, -1 for a null camera, -2 for a null location or -3 when the region lookup returns -1.
int __cdecl t3dSetCameraLocation(T3DScene_guess *scene, T3DCamera_guess *camera, T3DCameraLocation_guess *location)
{
    int result;
    int region;

    result = 0;
    if (camera == 0)
    {
        result = -1;
    }
    else if (location == 0)
    {
        result = -2;
    }
    else
    {
        region = EQ_CDECL(int, 0x100192C4, (T3DScene_guess *, T3DPoint_guess *))(scene, &location->position);
        if (region == -1)
        {
            result = -3;
        }
        else
        {
            camera->unknown_0004 = region;
            camera->position = location->position;
            camera->unknown_0014[0] = location->unknown_000c[0];
            camera->unknown_0014[1] = location->unknown_000c[1];
            camera->unknown_0014[2] = location->unknown_000c[2];
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1002f625
// Matches the camera's position and orientation to its associated scene actor.
// __cdecl (two stack arguments, callee pops 0 bytes; ECX is no input): stores the actor's region in the camera's int at +0x4, copies its position and three floats at +0x1c into the camera, and returns 0, -1 for a null camera or -2 for a negative actor index.
int __cdecl t3dSetCameraToAssociatedActorLocation(T3DScene_guess *scene, T3DCamera_guess *camera)
{
    int result;
    T3D_tagACTORINSTANCE *actor;

    result = 0;
    if (camera == 0)
    {
        result = -1;
    }
    else if (camera->actorIndex < 0)
    {
        result = -2;
    }
    else
    {
        actor = scene->actorTable[camera->actorIndex];
        camera->unknown_0004 = EQ_CDECL(int, 0x100192C4, (T3DScene_guess *, T3DPoint_guess *))(
            scene, &actor->point_guess);
        camera->position = actor->point_guess;
        camera->unknown_0014[0] = actor->orientation_guess[0];
        camera->unknown_0014[1] = actor->orientation_guess[1];
        camera->unknown_0014[2] = actor->orientation_guess[2];
    }
    return result;
}

// FUNCTION: EQGFX 0x1002f68d
// Prepares camera view and projection values to map the scene into the render view.
int __cdecl t3dComputeCameraWorldToEyeCoefficients(T3DCamera_guess *camera)
{
    unsigned int angleIndex;
    float cosineAtAngle0;
    float cosineAtAngle2;
    float cosineAtAngle1;
    float sineAtAngle0;
    float sineAtAngle2;
    float sineAtAngle1;
    float coefficient00;
    float coefficient10;
    float coefficient01;
    float coefficient11;
    float scaledCoefficient;

    if ((camera->renderBufferWidth != 0) && (camera->renderBufferHeight != 0))
    {
        camera->halfRenderBufferWidth = (float)(unsigned int)camera->renderBufferWidth * *T3D_CAMERA_HALF;
        camera->halfRenderBufferHeight = (float)(unsigned int)camera->renderBufferHeight * *T3D_CAMERA_HALF;
        __asm
        {
            mov eax, camera
            fld dword ptr [eax + 20h]
        }
        angleIndex = EQGFX_ROUND_ST0_TO_UINT_GUESS();
        if ((angleIndex != 0) && (angleIndex < 0x80))
        {
            cosineAtAngle0 = T3D_FLOAT_COSINE(camera->unknown_0014[0]);
            cosineAtAngle2 = T3D_FLOAT_COSINE(camera->unknown_0014[2]);
            cosineAtAngle1 = T3D_FLOAT_COSINE(camera->unknown_0014[1]);
            sineAtAngle0 = T3D_FLOAT_SINE(camera->unknown_0014[0]);
            sineAtAngle2 = T3D_FLOAT_SINE(camera->unknown_0014[2]);
            sineAtAngle1 = T3D_FLOAT_SINE(camera->unknown_0014[1]);
            camera->transform[2][2] = sineAtAngle1;
            camera->transform[0][2] = cosineAtAngle1 * cosineAtAngle0;
            camera->transform[1][2] = sineAtAngle0 * cosineAtAngle1;
            camera->transform[2][0] = -(sineAtAngle2 * cosineAtAngle1);
            camera->transform[2][1] = cosineAtAngle1 * cosineAtAngle2;
            coefficient00 = sineAtAngle0 * cosineAtAngle2 + (sineAtAngle1 * sineAtAngle2) * cosineAtAngle0;
            camera->transform[0][0] = coefficient00;
            coefficient10 = (sineAtAngle1 * sineAtAngle2) * sineAtAngle0 + -(cosineAtAngle2 * cosineAtAngle0);
            camera->transform[1][0] = coefficient10;
            coefficient01 = sineAtAngle2 * sineAtAngle0 - (sineAtAngle1 * cosineAtAngle2) * cosineAtAngle0;
            camera->transform[0][1] = coefficient01;
            coefficient11 = -(sineAtAngle2 * cosineAtAngle0) - (sineAtAngle1 * cosineAtAngle2) * sineAtAngle0;
            camera->transform[1][1] = coefficient11;
            camera->worldToEyeScale = *(float *)((char *)T3D_CAMERA_TRIG_TABLE + T3D_CAMERA_TRIG_SCALE_OFFSET + angleIndex * T3D_CAMERA_TRIG_SCALE_STRIDE);
            scaledCoefficient = camera->worldToEyeScale * coefficient00;
            camera->horizontalCoefficients[0] = scaledCoefficient;
            camera->horizontalPixelCoefficients[0] = scaledCoefficient * camera->halfRenderBufferWidth;
            scaledCoefficient = coefficient10 * camera->worldToEyeScale;
            camera->horizontalCoefficients[1] = scaledCoefficient;
            camera->horizontalPixelCoefficients[1] = scaledCoefficient * camera->halfRenderBufferWidth;
            scaledCoefficient = -(sineAtAngle2 * cosineAtAngle1) * camera->worldToEyeScale;
            camera->horizontalCoefficients[2] = scaledCoefficient;
            camera->horizontalPixelCoefficients[2] = scaledCoefficient * camera->halfRenderBufferWidth;
            scaledCoefficient = camera->aspect * camera->worldToEyeScale;
            camera->viewScale = scaledCoefficient;
            scaledCoefficient = scaledCoefficient * coefficient01;
            camera->verticalCoefficients[0] = scaledCoefficient;
            camera->viewScaleCopy = camera->viewScale;
            camera->worldToEyeScaleCopy = camera->worldToEyeScale;
            camera->verticalPixelCoefficients[0] = scaledCoefficient * camera->halfRenderBufferHeight;
            scaledCoefficient = coefficient11 * camera->viewScale;
            camera->verticalCoefficients[1] = scaledCoefficient;
            camera->verticalPixelCoefficients[1] = scaledCoefficient * camera->halfRenderBufferHeight;
            scaledCoefficient = cosineAtAngle1 * cosineAtAngle2 * camera->viewScale;
            camera->verticalCoefficients[2] = scaledCoefficient;
            camera->verticalPixelCoefficients[2] = scaledCoefficient * camera->halfRenderBufferHeight;
            camera->worldToEyeViewScaleProduct = camera->worldToEyeScale * camera->viewScale;
            camera->inverseWorldToEyeScaleSquared = *T3D_CAMERA_ONE / (camera->worldToEyeScale * camera->worldToEyeScale);
            camera->inverseViewScaleSquared = *T3D_CAMERA_ONE / (camera->viewScale * camera->viewScale);
            return 1;
        }
    }
    return -1;
}

// FUNCTION: EQGFX 0x1002f8c9
// __cdecl (four stack arguments, callee pops 0 bytes): unit normal of (A - B) x (C - B) and its distance (normal . B).
void __cdecl t3dComputePlaneFromPoints_guess(T3DPlane_guess *plane, T3DPoint_guess *pointA, T3DPoint_guess *pointB, T3DPoint_guess *pointC)
{
    T3DPoint_guess toA;
    T3DPoint_guess toC;
    double inverseLength;
    float normalX;
    float normalY;
    float normalZ;

    toA.x = pointA->x - pointB->x;
    toA.y = pointA->y - pointB->y;
    toA.z = pointA->z - pointB->z;
    toC.x = pointC->x - pointB->x;
    toC.y = pointC->y - pointB->y;
    toC.z = pointC->z - pointB->z;
    plane->normalX = toC.z * toA.y - toC.y * toA.z;
    plane->normalY = toC.x * toA.z - toC.z * toA.x;
    plane->normalZ = toC.y * toA.x - toC.x * toA.y;
    inverseLength = 1.0 / EQ_CDECL(double, 0x10069984, (double))(
        plane->normalX * plane->normalX + (plane->normalZ * plane->normalZ + plane->normalY * plane->normalY));
    normalX = inverseLength * plane->normalX;
    plane->normalX = normalX;
    normalY = inverseLength * plane->normalY;
    plane->normalY = normalY;
    normalZ = inverseLength * plane->normalZ;
    plane->normalZ = normalZ;
    plane->distance = normalY * pointB->y + normalZ * pointB->z + normalX * pointB->x;
}

// FUNCTION: EQGFX 0x1002f9a0
// Defines the camera's view volume for deciding which parts of the scene are visible.
// __cdecl (two stack arguments, callee pops 0 bytes; no value back): the camera's eight frustum corners (near four,
// then far four), moved by its transform and position, then the six planes built from them.
void __cdecl t3dSetCameraFrustum(T3DCamera_guess *camera, T3DFrustum_guess *frustum)
{
    double halfAngleTangent;
    double nearHalfWidth;
    float nearHalfHeight;
    double farHalfWidth;
    float farHalfHeight;
    T3DPoint_guess *point;
    int pointsLeft;
    float pointX;
    float pointY;
    float pointZ;

    halfAngleTangent = EQ_CDECL(double, 0x10069B84, (double))(camera->viewAngle * EQ_GLOBAL(float, 0x100762E0));
    nearHalfWidth = halfAngleTangent * camera->nearDistance;
    nearHalfHeight = nearHalfWidth / camera->aspect;
    frustum->corners[0].x = -nearHalfWidth;
    frustum->corners[0].y = nearHalfHeight;
    frustum->corners[0].z = camera->nearDistance;
    frustum->corners[1].x = nearHalfWidth;
    frustum->corners[1].y = nearHalfHeight;
    frustum->corners[2].x = nearHalfWidth;
    frustum->corners[2].y = -nearHalfHeight;
    frustum->corners[1].z = camera->nearDistance;
    frustum->corners[3].y = -nearHalfHeight;
    frustum->corners[2].z = camera->nearDistance;
    frustum->corners[3].x = -nearHalfWidth;
    frustum->corners[3].z = camera->nearDistance;
    farHalfWidth = halfAngleTangent * camera->yon;
    farHalfHeight = farHalfWidth / camera->aspect;
    frustum->corners[4].x = -farHalfWidth;
    frustum->corners[4].y = farHalfHeight;
    frustum->corners[4].z = camera->yon;
    frustum->corners[5].x = farHalfWidth;
    frustum->corners[5].y = farHalfHeight;
    frustum->corners[6].x = farHalfWidth;
    frustum->corners[6].y = -farHalfHeight;
    frustum->corners[5].z = camera->yon;
    frustum->corners[7].y = -farHalfHeight;
    frustum->corners[6].z = camera->yon;
    frustum->corners[7].x = -farHalfWidth;
    frustum->corners[7].z = camera->yon;

    point = frustum->corners;
    pointsLeft = 8;
    do
    {
        pointX = point->x;
        pointY = point->y;
        pointZ = point->z;
        point->x = pointZ * camera->transform[0][2] + pointX * camera->transform[0][0] +
                   pointY * camera->transform[0][1] + camera->position.x;
        point->y = pointX * camera->transform[1][0] + pointY * camera->transform[1][1] +
                   pointZ * camera->transform[1][2] + camera->position.y;
        point->z = pointY * camera->transform[2][1] + pointZ * camera->transform[2][2] +
                   pointX * camera->transform[2][0] + camera->position.z;
        point++;
        pointsLeft--;
    } while (pointsLeft != 0);

    EQ_CDECL(void, 0x1002F8C9, (T3DPlane_guess *, T3DPoint_guess *, T3DPoint_guess *, T3DPoint_guess *))
        (&frustum->planes[4], &frustum->corners[2], &frustum->corners[1], &frustum->corners[0]);
    EQ_CDECL(void, 0x1002F8C9, (T3DPlane_guess *, T3DPoint_guess *, T3DPoint_guess *, T3DPoint_guess *))
        (&frustum->planes[1], &frustum->corners[0], &frustum->corners[1], &frustum->corners[5]);
    EQ_CDECL(void, 0x1002F8C9, (T3DPlane_guess *, T3DPoint_guess *, T3DPoint_guess *, T3DPoint_guess *))
        (&frustum->planes[3], &frustum->corners[1], &frustum->corners[2], &frustum->corners[6]);
    EQ_CDECL(void, 0x1002F8C9, (T3DPlane_guess *, T3DPoint_guess *, T3DPoint_guess *, T3DPoint_guess *))
        (&frustum->planes[0], &frustum->corners[2], &frustum->corners[3], &frustum->corners[7]);
    EQ_CDECL(void, 0x1002F8C9, (T3DPlane_guess *, T3DPoint_guess *, T3DPoint_guess *, T3DPoint_guess *))
        (&frustum->planes[2], &frustum->corners[3], &frustum->corners[0], &frustum->corners[4]);
    EQ_CDECL(void, 0x1002F8C9, (T3DPlane_guess *, T3DPoint_guess *, T3DPoint_guess *, T3DPoint_guess *))
        (&frustum->planes[5], &frustum->corners[7], &frustum->corners[4], &frustum->corners[5]);
}

// FUNCTION: EQGFX 0x1002fb46
// Updates the far clipping plane to limit scene visibility at the camera's frustum distance.
void __cdecl t3dComputeFarFrustumPlane_guess(T3DCamera_guess *camera, T3DFrustum_guess *frustum)
{
    T3DPoint_guess *firstPoint;
    T3DPoint_guess *secondPoint;
    T3DPoint_guess *thirdPoint;
    double halfAngleTangent;
    double halfWidthWide;
    float halfWidth;
    double halfHeightWide;
    double negativeHalfWidth;
    double distance;
    float thirdX;
    float thirdY;
    float thirdZ;

    halfAngleTangent = EQ_CDECL(double, 0x10069B84, (double))
        ((double)(camera->viewAngle * EQ_GLOBAL(float, 0x100762E0)));
    halfWidthWide = halfAngleTangent * camera->frustumDistance;
    halfWidth = (float)halfWidthWide;
    halfHeightWide = halfWidthWide / camera->aspect;
    negativeHalfWidth = -(double)halfWidth;

    firstPoint = &frustum->corners[4];
    secondPoint = &frustum->corners[5];
    thirdPoint = &frustum->corners[7];
    firstPoint->x = (float)negativeHalfWidth;
    firstPoint->y = (float)halfHeightWide;
    firstPoint->z = camera->frustumDistance;
    secondPoint->x = halfWidth;
    secondPoint->y = (float)halfHeightWide;
    secondPoint->z = camera->frustumDistance;
    thirdPoint->x = (float)negativeHalfWidth;
    thirdPoint->y = (float)-halfHeightWide;
    thirdPoint->z = camera->frustumDistance;

    distance = firstPoint->z;
    firstPoint->x = (float)(negativeHalfWidth * camera->transform[0][0] +
                            distance * camera->transform[0][2] +
                            halfHeightWide * camera->transform[0][1] + camera->position.x);
    firstPoint->y = (float)(halfHeightWide * camera->transform[1][1] +
                            negativeHalfWidth * camera->transform[1][0] +
                            distance * camera->transform[1][2] + camera->position.y);
    firstPoint->z = (float)(distance * camera->transform[2][2] +
                            halfHeightWide * camera->transform[2][1] +
                            negativeHalfWidth * camera->transform[2][0] + camera->position.z);

    negativeHalfWidth = secondPoint->x;
    distance = secondPoint->z;
    secondPoint->x = (float)(negativeHalfWidth * camera->transform[0][0] +
                             distance * camera->transform[0][2] +
                             halfHeightWide * camera->transform[0][1] + camera->position.x);
    secondPoint->y = (float)(halfHeightWide * camera->transform[1][1] +
                             negativeHalfWidth * camera->transform[1][0] +
                             distance * camera->transform[1][2] + camera->position.y);
    secondPoint->z = (float)(distance * camera->transform[2][2] +
                             halfHeightWide * camera->transform[2][1] +
                             negativeHalfWidth * camera->transform[2][0] + camera->position.z);

    thirdX = thirdPoint->x;
    thirdY = thirdPoint->y;
    thirdZ = thirdPoint->z;
    thirdPoint->x = thirdX * camera->transform[0][0] +
                    thirdZ * camera->transform[0][2] +
                    thirdY * camera->transform[0][1] + camera->position.x;
    thirdPoint->y = thirdY * camera->transform[1][1] +
                    thirdX * camera->transform[1][0] +
                    thirdZ * camera->transform[1][2] + camera->position.y;
    thirdPoint->z = thirdZ * camera->transform[2][2] +
                    thirdY * camera->transform[2][1] +
                    thirdX * camera->transform[2][0] + camera->position.z;

    EQ_CDECL(void, 0x1002F8C9, (T3DPlane_guess *, T3DPoint_guess *, T3DPoint_guess *, T3DPoint_guess *))
        (&frustum->planes[5], thirdPoint, firstPoint, secondPoint);
}

// FUNCTION: EQGFX 0x1002fcf2
// Prepares the render context's point and scratch buffers for drawing scene geometry.
int __cdecl _t3dCreateClippingLists_guess(T3DRenderContext_guess *context, int count)
{
    int result;
    void *allocation;

    result = 1;
    context->unknown_2bc = count;
    allocation = EQ_CDECL(void *, 0x10050C48, (int))(count * 0x18);
    context->renderBuffer_2c0 = allocation;
    if (allocation == 0)
    {
        context->unknown_2bc &= (int)allocation;
        result |= -1;
    }
    else
    {
        count = count * 0x30;
        allocation = EQ_CDECL(void *, 0x10050C48, (int))(count);
        context->destSlots = (T3DPointSlot_guess *)allocation;
        if (allocation == 0)
        {
            context->unknown_2bc &= (int)allocation;
            result = -2;
        }
        else
        {
            allocation = EQ_CDECL(void *, 0x10050C48, (int))(count);
            context->renderBuffer_2c8 = allocation;
            if (allocation == 0)
            {
                context->unknown_2bc &= (int)allocation;
                result = -3;
            }
            else
            {
                allocation = EQ_CDECL(void *, 0x10050C48, (int))(count);
                context->renderBuffer_2cc = allocation;
                if (allocation == 0)
                {
                    context->unknown_2bc &= (int)allocation;
                    result = -4;
                }
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1002fd99
// Releases a render context's temporary scene-rendering buffers.
// __cdecl (one stack argument, callee pops 0 bytes; ECX is no input), returns 1. Frees the four render
// buffers at +0x2c0..+0x2cc through t3dFreeMemory, zeroing +0x2bc and every pointer it frees.
int __cdecl freeRenderBuffers_guess(T3DRenderContext_guess *context)
{
    context->unknown_2bc = 0;
    if (context->renderBuffer_2c0 != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(context->renderBuffer_2c0);
        context->renderBuffer_2c0 = 0;
    }
    if (context->destSlots != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(context->destSlots);
        context->destSlots = 0;
    }
    if (context->renderBuffer_2c8 != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(context->renderBuffer_2c8);
        context->renderBuffer_2c8 = 0;
    }
    if (context->renderBuffer_2cc != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(context->renderBuffer_2cc);
        context->renderBuffer_2cc = 0;
    }
    return 1;
}

// FUNCTION: EQGFX 0x1002fe13
// Copies indexed scene points into render slots for later drawing.
// __cdecl (three stack arguments, the callee pops 0 bytes; ECX is no input), returns 1. For every index in
// `indexList`, copies the 0xc-byte point `records[index]` into the next 0x30-byte destination slot of
// `context->destSlots`; only the first 0xc bytes of a slot are written.
int __cdecl t3dCopyIndexedPointsToRenderSlots_guess(T3DRenderContext_guess *context, T3DPoint_guess *records, T3DIndexList_guess *indexList)
{
    int *index = indexList->indexes;
    int count = indexList->count;
    T3DPointSlot_guess *dest = context->destSlots;
    int *indexEnd = index + count;

    for (; index < indexEnd; index++)
    {
        T3DPoint_guess *source = &records[*index];

        dest->x = source->x;
        dest->y = source->y;
        dest->z = source->z;
        dest++;
    }
    return 1;
}
