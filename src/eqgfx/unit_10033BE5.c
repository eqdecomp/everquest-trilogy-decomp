// unit_10033BE5.c - eqgfx_dx7.dll, the address run 0x10033BE5..0x10038D6B (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"

typedef struct EffectFrameDescriptor_guess EffectFrameDescriptor_guess;

typedef struct EffectInstance_guess
{
    unsigned char unknown_000[0x0c];
    unsigned char flags;
    unsigned char unknown_00d[0x03];
    float position[3];
    unsigned int attributeFlags;
    unsigned char unknown_020[0x14];
    unsigned int currentFrameData;
    unsigned char unknown_038[0x30];
    unsigned int frameIndex;
    int frameDuration;
    unsigned int lastUpdate;
    unsigned int animationState;
    float effectOffset[3];
    unsigned char unknown_084[0x04];
    EffectFrameDescriptor_guess *frameDescriptor;
    unsigned char unknown_08c[0x68];
    unsigned int cacheStamp;
} EffectInstance_guess;

typedef struct EffectFrameDescriptor_guess
{
    unsigned char unknown_000[0x14];
    unsigned int frameCount;
    unsigned int curveCount;
    float *curves;
    float horizontalScale;
    float verticalScale;
    float depthScale;
} EffectFrameDescriptor_guess;

#define EQGFX_EFFECT_CACHE_STAMP_GUESS EQ_GLOBAL(unsigned int, 0x100FBBB0)
#define t3d_angle_tree_guess EQ_GLOBAL(unsigned char *, 0x100F3FF4)
#define EQGFX_EFFECT_SQRT_GUESS ((float (__cdecl *)(float))EQ_GLOBAL(unsigned int, 0x100FC77C))
#define EQGFX_EFFECT_ANGLE_GUESS EQ_CDECL(float, 0x1004ECF9, (float, float, char *))
#define EQGFX_EFFECT_FREE_BUFFERS_GUESS EQ_CDECL(void, 0x1002FD99, (void *))
#define EQGFX_EFFECT_ALLOC_BUFFERS_GUESS EQ_CDECL(int, 0x1002FCF2, (void *, int))
#define EQGFX_EFFECT_SUBMIT_GUESS EQ_CDECL(int, 0x100477B9, (void *, int, int, void *, unsigned int))
#define EQGFX_EFFECT_SINGLE_CURVE_SENTINEL 1.40129846e-45f
#define EQGFX_EFFECT_DISTANCE_EPSILON EQ_GLOBAL(double, 0x10076300)
#define parseColorScale EQ_GLOBAL(float, 0x100762A8)
#define EQGFX_EFFECT_ANGLE_WRAP_AMOUNT EQ_GLOBAL(float, 0x100762FC)
#define c_slerpAngleScale EQ_GLOBAL(float, 0x100762F8)
#define EQGFX_EFFECT_NEGATIVE_VERTICAL_ANGLE EQ_GLOBAL(float, 0x100762F4)

typedef struct EffectVertex_guess
{
    float positionX;
    float positionY;
    float positionZ;
    unsigned int unknown_00c;
    float textureX;
    float textureY;
    float unknown_018[3];
    float colorX;
    float colorY;
    float colorZ;
} EffectVertex_guess;

typedef struct EffectFrameSet_guess
{
    unsigned char unknown_000[0x14];
    unsigned int frameCount;
    union
    {
        float oneFrameValue;
        float *frameValues;
    } values;
    float *framePositions;
} EffectFrameSet_guess;

typedef struct EffectAnimation_guess
{
    unsigned char unknown_000[0x0c];
    unsigned char flags;
    unsigned char unknown_00d[3];
    unsigned int unknown_010;
    unsigned int frameIndex;
    unsigned int frameDuration;
    unsigned int lastUpdate;
    unsigned int unknown_020;
    EffectFrameSet_guess *frameSet;
} EffectAnimation_guess;

typedef struct EffectLight_guess
{
    unsigned int type;
    unsigned char unknown_004[0x0c];
    EffectAnimation_guess *animation;
    float x;
    float y;
    float z;
    float strength;
} EffectLight_guess;

typedef struct EffectDirectionalLight_guess
{
    unsigned int type;
    unsigned char unknown_004[0x10];
    EffectAnimation_guess *animation;
    float x;
    float y;
    float z;
} EffectDirectionalLight_guess;

#define EQGFX_EFFECT_TYPE_POINT 0x1e
#define EQGFX_EFFECT_TYPE_DIRECTIONAL 0x1f
#define EQGFX_EFFECT_ANIMATION_PAUSED 0x08
#define EQGFX_EFFECT_POINT_COLOR_ENABLED 0x40
#define EQGFX_EFFECT_DIRECTIONAL_COLOR_ENABLED 0x00200000

typedef struct EffectRenderContext_guess
{
    unsigned char unknown_000[0x184];
    unsigned int currentFrame;
    unsigned char unknown_188[0x3c];
    EffectLight_guess *excludedLight;
    unsigned char unknown_1c8[0xe0];
    int lightCount;
    EffectLight_guess **lightList;
    unsigned char unknown_2b0[0x14];
    EffectVertex_guess *outputVertices;
    unsigned int unknown_2c8;
    EffectVertex_guess *vertices;
} EffectRenderContext_guess;

typedef struct RenderProjectionState_guess
{
    unsigned char unknown_000[0x0f];
    unsigned char flags;
    unsigned char unknown_010[0x58];
    float rotationMatrix[9];
    float scaledRotationMatrix[9];
    float inverseScaleRotationMatrix[9];
    unsigned char unknown_0d4[0x14];
    unsigned int projectionMarker;
} RenderProjectionState_guess;

typedef struct RenderRotationInput_guess
{
    unsigned char unknown_000[0x0c];
    float rotation[3];
} RenderRotationInput_guess;

#define EQGFX_ROUND_ST0_TO_UINT_GUESS EQ_CDECL(unsigned int, 0x1004EC36, (void))
#define EQGFX_ROTATION_INDEX_MASK 0x1ff
#define EQGFX_ROTATION_COSINE_TABLE_OFFSET 0x1000
#define EQGFX_ROTATION_SINE_TABLE_OFFSET 0x1800
#define EQGFX_PROJECTION_MARKER 0x7149f2ca

typedef struct TriangleLightingTransformInput_guess
{
    unsigned char unknown_000[0x1ac];
    float transformFactorX;
    float transformFactorY;
    float transformFactorZ;
    float outputAdjustmentX;
    float outputAdjustmentY;
    float outputAdjustmentZ;
    unsigned char unknown_1c4[0x68];
    float thirdSourceX;
    unsigned char unknown_230[0x08];
    float thirdSourceY;
    unsigned char unknown_23c[0x08];
    float thirdSourceZ;
    float firstSourceX;
    float firstSourceY;
    float firstSourceZ;
    float secondSourceX;
    float secondSourceY;
    float secondSourceZ;
    unsigned char unknown_260[0x3b0];
    unsigned int callbackArgument1;
    unsigned char unknown_614[0x08];
    unsigned int callbackArgument4;
} TriangleLightingTransformInput_guess;

#define EQGFX_VISIBILITY_MEMSET_GUESS EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))

typedef struct TriangleCullVisibilityBatch_guess
{
    unsigned short triangleCount;
    unsigned short matrixIndex;
} TriangleCullVisibilityBatch_guess;

typedef struct TriangleCullVisibilityTriangle_guess
{
    unsigned char flags;
    unsigned char unknown_01;
    unsigned short index0;
    unsigned short index1;
    unsigned short index2;
} TriangleCullVisibilityTriangle_guess;

typedef struct BufferOwner_guess BufferOwner_guess;

typedef struct TriangleCullVisibilityGeometry_guess
{
    unsigned short vertexFlagBytes;
    unsigned char unknown_02[6];
    unsigned short elementFlagBytes;
    unsigned char unknown_0A[2];
    unsigned short batchCount;
    unsigned char unknown_0E[4];
    unsigned short attenuationIndex;
    short *vertexPositions;
    TriangleCullVisibilityTriangle_guess *triangles;
    unsigned char unknown_1C[4];
    TriangleCullVisibilityBatch_guess *batches;
} TriangleCullVisibilityGeometry_guess;

typedef struct TriangleCullVisibilityOwner_guess
{
    unsigned char unknown_00[0x18];
    TriangleCullVisibilityGeometry_guess *geometry;
} TriangleCullVisibilityOwner_guess;

typedef struct TriangleCullVisibilityInput_guess
{
    unsigned char unknown_00[0x20];
    struct TriangleCullScene_guess *scene;
    BufferOwner_guess *positionOwner;
    unsigned char unknown_28[0x0c];
    TriangleCullVisibilityOwner_guess *owner;
} TriangleCullVisibilityInput_guess;

typedef struct TriangleCullVisibilityOutput_guess
{
    unsigned char unknown_000[0x618];
    unsigned char *elementVisible;
    unsigned char *vertexVisible;
} TriangleCullVisibilityOutput_guess;

typedef struct TriangleCullMatrixObject_guess
{
    unsigned char unknown_00[0x20];
    int orientationMode;
} TriangleCullMatrixObject_guess;

typedef struct TriangleCullScene_guess
{
    unsigned char unknown_00[0x18];
    TriangleCullMatrixObject_guess **matrixObjects;
} TriangleCullScene_guess;

typedef struct TriangleCullTriangle_guess
{
    unsigned char unknown_00[0x08];
    unsigned short flags;
    unsigned short index0;
    unsigned short index1;
    unsigned short index2;
} TriangleCullTriangle_guess;

typedef struct TriangleCullBatch_guess
{
    unsigned short firstElement;
    unsigned short triangleCount;
    unsigned short matrixIndex;
} TriangleCullBatch_guess;

typedef struct TriangleCullGeometry_guess
{
    unsigned int vertexFlagBytes;
    float *vertexPositions;
    unsigned char unknown_08[0x18];
    unsigned int elementFlagBytes;
    TriangleCullTriangle_guess *triangles;
    unsigned char unknown_28[0x24];
    unsigned int batchCount;
    TriangleCullBatch_guess *batches;
} TriangleCullGeometry_guess;

typedef struct TriangleCullOwner_guess
{
    unsigned char unknown_00[0x14];
    TriangleCullGeometry_guess *geometry;
} TriangleCullOwner_guess;

typedef struct TriangleCullClient_guess
{
    unsigned char unknown_00[0x20];
    TriangleCullScene_guess *scene;
    unsigned char unknown_24[0x10];
    TriangleCullOwner_guess *owner;
} TriangleCullClient_guess;

typedef struct TriangleCullRenderer_guess
{
    unsigned char unknown_00[0x618];
    unsigned char *elementVisible;
    unsigned char *vertexVisible;
} TriangleCullRenderer_guess;

#define EQGFX_DYNAMIC_LOD_ENABLED_GUESS EQ_GLOBAL(unsigned int, 0x100F2D9C)
#define EQGFX_LOD_DISTANCE_SCALE_GUESS EQ_GLOBAL(unsigned int, 0x100A16B0)
#define EQGFX_VIEW_DISTANCE_OFFSET_BITS_GUESS EQ_GLOBAL(unsigned int, 0x100A16AC)

typedef struct BufferBatch_guess
{
    unsigned short vertexCount;
    unsigned short matrixIndex;
} BufferBatch_guess;

typedef struct BufferMetadata_guess
{
    unsigned short vertexCount;
    unsigned char unknown_02[0x0a];
    short *vertexPositions;
    unsigned char unknown_10[0x02];
    unsigned short attenuationIndex;
    unsigned int sourceAddress;
    unsigned char unknown_18[0x04];
    BufferBatch_guess *batches;
    unsigned char unknown_20[0x58];
    char *flags;
} BufferMetadata_guess;

typedef struct BufferContainer_guess
{
    unsigned char unknown_00[0x18];
    BufferMetadata_guess *metadata;
} BufferContainer_guess;

struct BufferOwner_guess
{
    unsigned char unknown_00[0x18];
    int vertexStart;
    void **slotEntries;             // 0x1c
    unsigned char unknown_20[0x14];
    BufferContainer_guess *container; // 0x34
};

typedef struct MatrixScale_guess
{
    unsigned char unknown_00[0x18];
    float scaleFactor;              // 0x18
} MatrixScale_guess;

typedef struct MatrixRecordSet_guess
{
    unsigned char unknown_00[0x24];
    unsigned char *records;         // 0x24
    unsigned char unknown_28[0x34];
    MatrixScale_guess *scale;       // 0x5c
} MatrixRecordSet_guess;

typedef struct TransformedVertexBuffer_guess
{
    unsigned char unknown_00[0x610];
    unsigned int destinationAddress;
} TransformedVertexBuffer_guess;

#define EQGFX_BUILD_VERTEX_MATRIX_GUESS EQ_CDECL(void, 0x10038D6B, (int, int, int, unsigned char *))
#define EQGFX_SOURCE_VERTEX_STRIDE 0x14
#define EQGFX_DESTINATION_VERTEX_STRIDE 0x0c
#define EQGFX_MATRIX_RECORD_STRIDE 0x148

#define EQGFX_MESH_SCALE_F_GUESS EQ_ARRAY(float, 0x100FB868)

typedef struct LookupEntry_guess
{
    unsigned char unknown_00[0x12];
    unsigned short factorIndex;
} LookupEntry_guess;

typedef struct LookupContainer_guess
{
    unsigned char unknown_00[0x18];
    LookupEntry_guess *entry;
} LookupContainer_guess;

typedef struct ObjectTransform_guess
{
    unsigned char unknown_00[0x14];
    float x;
    float y;
    float z;
    unsigned char unknown_20[0x14];
    LookupContainer_guess *lookupContainer;
} ObjectTransform_guess;

typedef struct SourceMatrix_guess
{
    unsigned char unknown_00[0x38];
    float sphereScale;              // 0x38
    unsigned char unknown_3c[0x20];
    float basis[3][3];
    float translation[3];
} SourceMatrix_guess;

typedef struct RenderState_guess
{
    unsigned char unknown_00[0x1ac];
    float translation[3];
    unsigned char unknown_1b8[0x74];
    float zFromX;
    unsigned char unknown_230[0x08];
    float zFromY;
    unsigned char unknown_23c[0x08];
    float zFromZ;
    float xFromX;
    float xFromY;
    float xFromZ;
    float yFromX;
    float yFromY;
    float yFromZ;
    unsigned char unknown_260[0x3b0];
    float *destinationVertices;
    unsigned char unknown_614[0x04];
    unsigned char *elementVisible;
    unsigned char *vertexVisible;
} RenderState_guess;

// eqgfx_dx7.dll's own constant: the zero float the original's FP compares read from .rdata.
#define EQGFX_ZERO_F_GUESS          EQ_GLOBAL(float, 0x10076284)
#define EQGFX_PRECALC_LIGHTING_GUESS EQ_GLOBAL(unsigned int, 0x100A16B4)
#define EQGFX_SPRITE_DISTANCE_ENABLED_GUESS EQ_GLOBAL(int, 0x100F2D9C)
#define EQGFX_MINIMUM_SPRITE_DISTANCE_GUESS EQ_GLOBAL(float, 0x1007630C)
#define EQGFX_VIEW_DISTANCE_OFFSET_GUESS EQ_GLOBAL(float, 0x100A16AC)
#define EQGFX_MINIMUM_NORMALIZED_SPRITE_DISTANCE_GUESS EQ_GLOBAL(float, 0x10076308)

// The element record the mesh's element list holds (8 bytes each): a flags byte at +0x0 whose bit 0 marks
// the element as skipped, and three unsigned short vertex indices at +0x2, +0x4 and +0x6.
typedef struct Element_guess
{
    unsigned char  flags;       // 0x00
    unsigned char  unknown_01;
    unsigned short index0;      // 0x02
    unsigned short index1;      // 0x04
    unsigned short index2;      // 0x06
} Element_guess;

// An element's plane (16 bytes each): the normal's three floats at +0x0 and the distance at +0xc.
typedef struct Plane_guess
{
    float normal[3];    // 0x00
    float distance;     // 0x0c
} Plane_guess;

// One vertex's lighting entry (4 bytes each, the mesh's vertex-light array): the ambient index is its last byte.
typedef struct VertexLight_guess
{
    unsigned char unknown_00[0x03];
    unsigned char ambientIndex;     // 0x03
} VertexLight_guess;

// One source vertex (20 bytes each): the ambient index is its last byte, at +0x13.
typedef struct SourceVertex_guess
{
    unsigned char unknown_00[0x13];
    unsigned char ambientIndex;     // 0x13
} SourceVertex_guess;

// The mesh geometry the mesh object points to at +0x18: the vertex count at +0x0 (the per-vertex visible
// array's size), the source-vertex count at +0x6, the element count at +0x8, the source vertices at +0x14, the
// element array at +0x18 and the element planes at +0x74.
typedef struct Geometry_guess
{
    unsigned short vertexCount;     // 0x00
    unsigned char  unknown_02[0x04];
    unsigned short sourceVertexCount; // 0x06
    unsigned short elementCount;    // 0x08
    unsigned char  unknown_0a[0x0a];
    SourceVertex_guess *sourceVertices; // 0x14
    Element_guess *elements;        // 0x18
    unsigned char  unknown_1c[0x58];
    Plane_guess   *planes;          // 0x74
} Geometry_guess;

typedef struct SpriteDistancePosition_guess
{
    unsigned char unknown_00[0x10];
    float positionX;
    float positionY;
    float positionZ;
    unsigned char unknown_1c[0x18];
    float distanceScaleA;
    float distanceScaleB;
} SpriteDistancePosition_guess;

typedef struct ViewingDistanceContext_guess
{
    unsigned char unknown_00[0x1ac];
    float originX;
    float originY;
    float originZ;
    unsigned char unknown_1b8[0x74];
    float projectionX;
    unsigned char unknown_230[0x8];
    float projectionY;
    unsigned char unknown_23c[0x8];
    float projectionZ;
    unsigned char unknown_248[0x2e0];
    float distanceScale;
} ViewingDistanceContext_guess;

// The 16-byte element record the mesh's +0x14 geometry holds: a flags byte at +0x8 whose bit 0 marks the
// element as skipped, and three unsigned short vertex indices at +0xa, +0xc and +0xe.
typedef struct Element16_guess
{
    unsigned char  unknown_00[0x08];
    unsigned char  flags;       // 0x08
    unsigned char  unknown_09;
    unsigned short index0;      // 0x0a
    unsigned short index1;      // 0x0c
    unsigned short index2;      // 0x0e
} Element16_guess;

// The geometry the mesh holds at +0x14: the vertex count at +0x0 (the per-vertex visible array's size), the
// element count at +0x20, the 16-byte element array at +0x24 and the element planes at +0x2c.
typedef struct ElementGeometry_guess
{
    unsigned int      vertexCount;   // 0x00
    unsigned char     unknown_04[0x14];
    unsigned int      vertexLightCount; // 0x18
    VertexLight_guess *vertexLights;    // 0x1c
    unsigned int      elementCount;  // 0x20
    Element16_guess  *elements;      // 0x24
    unsigned char     unknown_28[0x04];
    Plane_guess      *planes;        // 0x2c
} ElementGeometry_guess;

// The mesh object the client holds at +0x34: its 16-byte-element geometry sits at +0x14 and the other one
// at +0x18, its flags byte at +0xe and its per-slot vertex-light arrays at +0x1c.
typedef struct Mesh_guess
{
    int                    type;                // 0x00
    unsigned char          unknown_04[0x0a];
    unsigned char          flags;               // 0x0e
    unsigned char          unknown_0f[0x05];
    ElementGeometry_guess *elementGeometry; // 0x14
    Geometry_guess        *geometry;        // 0x18
    VertexLight_guess    **vertexLightsBySlot; // 0x1c
} Mesh_guess;

// The client object: its type tag at +0x0 (0x48 when a ray hit it), its slot in its mesh's per-slot arrays at
// +0x18, its parent client at +0x28 and its mesh pointer at +0x34.
typedef struct Client_guess
{
    int           type;                 // 0x00
    unsigned char unknown_04[0x14];
    int           meshSlot;             // 0x18
    unsigned char unknown_1c[0x0c];
    struct Client_guess *parent;        // 0x28
    unsigned char unknown_2c[0x08];
    Mesh_guess   *mesh;                 // 0x34
} Client_guess;

// The sprite a collision ray hits: its type tag at +0x0 (0x18) and the client it belongs to at +0xfc.
typedef struct HitSprite_guess
{
    int           type;                 // 0x00
    unsigned char unknown_04[0xf8];
    Client_guess *client;               // 0xfc
} HitSprite_guess;

// The record the collision query writes (20 bytes): the hit sprite, the hit object (a client with tag 0x48 or
// a mesh with tag 0x47) and the entry number of the hit element.
typedef struct RayCollisionResult_guess
{
    HitSprite_guess *hitSprite;         // 0x00
    void            *hitObject;         // 0x04
    unsigned int     surfaceMask;       // 0x08
    int              entryNumber;       // 0x0c
    float            distance;          // 0x10
} RayCollisionResult_guess;

// The actor whose ambient light is cached: the dirty flag at +0xf (0x08), the position at +0x10 and the cached
// ambient at +0x110.
/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3dactor.h"

// The render target DrawActorDMSprite_guess reads: the table of actor pointers at +0x17c, indexed by the caller's actor index.
typedef struct RenderTarget_guess
{
    unsigned char unknown_000[0x08];
    unsigned int currentTime;
    unsigned char unknown_00c[0x170];
    T3D_tagACTORINSTANCE  **actors;              // 0x17c
} RenderTarget_guess;

typedef struct HierarchicalObject_guess HierarchicalObject_guess;

// The renderer object param_3 points at: the per-element visible-byte array (one byte per element) is at
// +0x618 and the per-vertex one (indexed by an element's vertex indices) at +0x61c.
typedef struct Renderer_guess
{
    unsigned char  unknown_0000[0x184];
    unsigned int   spriteTiming;             // 0x184
    unsigned char  unknown_188[0x24];
    float          translation[3];
    unsigned char  unknown_1b8[0x74];
    float          zFromX;                  // 0x22c
    unsigned char  unknown_230[0x08];
    float          zFromY;                  // 0x238
    unsigned char  unknown_23c[0x08];
    float          zFromZ;                  // 0x244
    float          transformValues[6];      // 0x248
    unsigned char  unknown_260[0x28];
    int            renderMode;              // 0x288
    unsigned char  unknown_28c[0x04];
    int            renderCount;             // 0x290
    unsigned char  unknown_294[0x1c];
    float          renderVector[3];         // 0x2b0
    unsigned int   renderBufferCount;       // 0x2bc
    unsigned char  unknown_2c0[0x04];
    float         *transformedVertices;     // 0x2c4
    unsigned char  unknown_2c8[0x04];
    float         *vertices;                // 0x2cc
    unsigned char  unknown_2d0[0x74];
    float          inverseTranslation[3];
    unsigned int   firstRenderDataValue;
    unsigned int   secondRenderDataValue;
    unsigned char  unknown_358[0x80];
    HierarchicalObject_guess *object;        // 0x3d8
    unsigned char  unknown_3dc[0x04];
    unsigned int  *attributeValues;          // 0x3e0
    unsigned char  unknown_3e4[0x18];
    int            previousVisibilityResult; // 0x3fc
    int            visibilityResult;         // 0x400
    unsigned char  unknown_404[0xe8];
    int            secondaryRenderMode;      // 0x4ec
    unsigned char  unknown_4f0[0x38];
    float          distanceScale;           // 0x528
    unsigned char  unknown_52c[0x04];
    unsigned char  visibilityBoundsContext[0xe4]; // 0x530
    void          *callbackInput;             // 0x614
    unsigned char *elementVisible;  // 0x618
    unsigned char *vertexVisible;   // 0x61c
} Renderer_guess;

// eqgfx_dx7.dll's own light list (0x100f2498) and its length (0x100f2d98), walked by the light-records
// pass below, and the 0.05f factor a point light's square radius is scaled by.
#define EQGFX_LIGHT_COUNT_GUESS        EQ_GLOBAL(int, 0x100F2D98)
#define EQGFX_LIGHT_LIST_GUESS         EQ_ARRAY(Light_guess *, 0x100F2498)
#define EQGFX_POINT_RADIUS_SCALE_GUESS  EQ_GLOBAL(float, 0x100762F0)

// A light of the 0x100f2498 list (also the record its type tag starts): the type tag at +0x0 decides which
// of the two layouts below the rest of the record has; both share this header.
typedef struct Light_guess
{
    int    type;        // 0x00
    float  outX;        // 0x04
    float  outY;        // 0x08
    float  outZ;        // 0x0c
} Light_guess;

// The vertex data a light's attachment points to at +0x24: a float per vertex at +0x18 and, when it is not
// NULL, three floats per vertex at +0x1c.
typedef struct LightVertices_guess
{
    unsigned char unknown_00[0x18];
    float        *values;       // 0x18
    float        *positions;    // 0x1c
} LightVertices_guess;

// The object a light is attached to: the vertex index at +0x14 and the vertex data at +0x24.
typedef struct LightAttachment_guess
{
    unsigned char unknown_00[0x14];
    int           vertexIndex;  // 0x14
    unsigned char unknown_18[0x0c];
    LightVertices_guess *vertices;  // 0x24
} LightAttachment_guess;

// A point light (type 0x1e): the attachment pointer at +0x10, the source position at +0x14..+0x1c and the
// radius at +0x20.
typedef struct PointLight_guess
{
    int    type;        // 0x00
    float  outX;        // 0x04
    float  outY;        // 0x08
    float  outZ;        // 0x0c
    LightAttachment_guess *attachment;  // 0x10
    float  x;           // 0x14
    float  y;           // 0x18
    float  z;           // 0x1c
    float  radius;      // 0x20
} PointLight_guess;

// A directional light (type 0x1f): one float at +0x10, the attachment pointer at +0x14 and the source
// direction at +0x18..+0x20.
typedef struct DirectionalLight_guess
{
    int    type;        // 0x00
    float  outX;        // 0x04
    float  outY;        // 0x08
    float  outZ;        // 0x0c
    float  unknown10;   // 0x10
    LightAttachment_guess *attachment;  // 0x14
    float  x;           // 0x18
    float  y;           // 0x1c
    float  z;           // 0x20
} DirectionalLight_guess;

// The record the pass writes for each list light (28 bytes each): the type tag, the transformed position
// and the fetched, attenuation-scaled vertex position.
typedef struct LightOutput_guess
{
    int    type;        // 0x00
    float  outX;        // 0x04
    float  outY;        // 0x08
    float  outZ;        // 0x0c
    float  vertexX;     // 0x10
    float  vertexY;     // 0x14
    float  vertexZ;     // 0x18
} LightOutput_guess;

// The object buildSpriteRelativeLightOutputs_guess's param_1 points at: the sprite position at +0x14..+0x1c, subtracted from each
// transformed light position.
typedef struct Sprite_guess
{
    unsigned char unknown_00[0x14];
    float x;            // 0x14
    float y;            // 0x18
    float z;            // 0x1c
} Sprite_guess;

// The object buildSpriteRelativeLightOutputs_guess's param_2 points at: a scale at +0x58, the 12-float view transform at +0x8c and
// the 9-float rotation at +0xbc (buildTransformedLightOutputs_guess's viewMatrix and rotationMatrix, embedded in one object).
typedef struct LightTransform_guess
{
    unsigned char unknown_00[0x38];
    float objectScale;          // 0x38
    unsigned char unknown_3c[0x1c];
    float lightScale;           // 0x58
    float objectToViewTransform[12]; // 0x5c
    float viewTransform[12];    // 0x8c
    float rotation[9];          // 0xbc
} LightTransform_guess;

typedef struct HierarchicalObjectData_guess HierarchicalObjectData_guess;
typedef struct TriangleLightingData_guess TriangleLightingData_guess;
typedef struct HierarchicalTransformData_guess HierarchicalTransformData_guess;

typedef struct HierarchicalObject_guess
{
    unsigned char unknown_00[0x14];
    float x;
    float y;
    float z;
    void *renderObject;
    int renderMode;
    struct HierarchicalObject_guess *sourceObject; // 0x28
    unsigned char unknown_2c[0x08];
    HierarchicalObjectData_guess *data;
} HierarchicalObject_guess;

typedef struct HierarchicalObjectData_guess
{
    unsigned char unknown_00[0x0e];
    unsigned char flags;                // 0x0e
    unsigned char unknown_0f[0x05];
    TriangleLightingData_guess *triangleLighting;
    HierarchicalTransformData_guess *vertexTransform;
} HierarchicalObjectData_guess;

// object->renderMode (+0x24) when it is set: the frame source whose triangle data gives the first output index.
typedef struct FrameSource_guess
{
    unsigned char unknown_00[0x18];
    int frameIndex;                     // 0x18
    unsigned char unknown_1c[0x18];
    HierarchicalObjectData_guess *data; // 0x34
} FrameSource_guess;

typedef struct TriangleLightingGroup_guess
{
    unsigned short vertexCount;
    unsigned short modelIndex;
} TriangleLightingGroup_guess;

// One 8-byte entry of a triangle lighting's entry list: the vertex index (12 bytes per vertex in the destination),
// a 16-bit value, the index into the owning object's slot entries and one unused short.
typedef struct TriangleSlotEntry_guess
{
    unsigned short vertexIndex;
    unsigned short value;
    unsigned short slotIndex;
    unsigned short unknown_06;
} TriangleSlotEntry_guess;

typedef struct TriangleLightingData_guess
{
    unsigned int triangleCount;
    unsigned int firstOutputIndex;
    unsigned char unknown_08[0x08];
    unsigned int renderEnabled;
    unsigned int renderDataValue;
    unsigned char unknown_18[0x08];
    unsigned int effectBufferIndex;
    unsigned char unknown_24[0x24];
    TriangleLightingGroup_guess *groups;
    unsigned char unknown_4c[0x08];
    unsigned int entryCount;            // 0x54
    TriangleSlotEntry_guess *entries;   // 0x58
    unsigned char unknown_5c[0x04];
    int *detailLevels;                  // 0x60
    unsigned char unknown_64[0x1c];
    unsigned int callbackContext;
    float boundsPoint0[3];
    float boundsPoint1[3];
} TriangleLightingData_guess;

typedef struct HierarchicalTransformData_guess
{
    unsigned short vertexCount;
    unsigned char unknown_02[0x02];
    unsigned short animationSampleCount;
    unsigned short lightingState;
    unsigned short effectBufferIndex;
    unsigned char unknown_0a[0x04];
    unsigned short groupCount;          // 0x0e
    unsigned char unknown_10[0x04];
    short *vertexInput;
    unsigned int unknown_18;
    TriangleLightingGroup_guess *groups;
    unsigned char unknown_20[0x04];
    TriangleLightingGroup_guess *groupEntries; // 0x24
    float *detailLevels;                // 0x28
    float objectPosition[3];
    unsigned char unknown_38[0x10];
    void *renderObject;
    unsigned char unknown_4c[0x0c];
    float lightingScale;                    // 0x58
    float boundsOffset0[3];
    float boundsOffset1[3];
    unsigned int vertexProcessingMode;       // 0x74
    char *vertexFlags;
} HierarchicalTransformData_guess;

typedef struct RenderWork_guess
{
    HierarchicalObjectData_guess *objectData;
    float objectMatrix[9];
    float relativePosition[3];
    float projectedPosition[3];
    float renderPosition[3];
} RenderWork_guess;

typedef struct ActorRenderWork_guess
{
    unsigned char transformedFrustum[0xd8];
    float adjustedPosition[3];
    unsigned int savedFirstOutputIndex;
    float viewPosition[3];
    float inverseScaleMatrix[9];
    float relativePosition[3];
    int rgbTrackSet;
    int restoreFirstOutputIndex;
} ActorRenderWork_guess;

typedef struct RenderSceneRoot_guess
{
    unsigned char unknown_00[0x24];
    void *sceneLink;
} RenderSceneRoot_guess;

typedef struct RenderSceneLink_guess
{
    unsigned char unknown_00[0x1c];
    HierarchicalObject_guess *object;
} RenderSceneLink_guess;

typedef void (__cdecl *HierarchicalRenderCallback_guess)(void *, unsigned int, void *);

#define EQGFX_RENDER_VISIBILITY_MODE_GUESS EQ_GLOBAL(int, 0x100A16B8)
#define g_render_mode_guess EQ_GLOBAL(int, 0x100FB8A8)
#define EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS EQ_GLOBAL(HierarchicalRenderCallback_guess, 0x100FC76C)
#define EQGFX_CALCULATE_OBJECT_BOUNDS_GUESS EQ_CDECL(int, 0x10030E4A, (void *, float *, float *))
#define EQGFX_UPDATE_RENDER_ANIMATION_GUESS EQ_CDECL(void, 0x1003B59C, (Renderer_guess *))
#define EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS EQ_CDECL(void, 0x1003D238, (Renderer_guess *, HierarchicalObjectData_guess *))
#define EQGFX_REFRESH_RENDER_OBJECT_GUESS EQ_CDECL(void, 0x1003CD0D, (HierarchicalObjectData_guess *, void *, Renderer_guess *))
#define EQGFX_APPLY_PHYSICS_EFFECTS_GUESS EQ_CDECL(void, 0x1003D118, (Renderer_guess *, float *, unsigned int, unsigned int))
#define EQGFX_RENDER_HIERARCHICAL_OBJECT_GUESS EQ_CDECL(void, 0x100396E2, (HierarchicalObject_guess *, Renderer_guess *, void *))
#define EQGFX_SUBMIT_HIERARCHICAL_TRANSFORM_GUESS EQ_CDECL(void, 0x10037885, (HierarchicalObjectData_guess *, float *, Renderer_guess *))
#define EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS EQ_CDECL(void, 0x1003CF06, (void *, Renderer_guess *))
#define EQGFX_FLUSH_EFFECT_BUFFERS_GUESS EQ_CDECL(void, 0x10045E32, (unsigned int))
#define EQGFX_RENDER_MODE_TWO_GUESS EQ_CDECL(void, 0x1003E196, (HierarchicalObjectData_guess *, void *, Renderer_guess *))
#define EQGFX_RENDER_MODE_TWO_SCENE_GUESS EQ_CDECL(void, 0x1003EECA, (void *, Renderer_guess *, HierarchicalObjectData_guess *, void *))
#define EQGFX_RENDER_MODE_ONE_GUESS EQ_CDECL(void, 0x1003E858, (HierarchicalObjectData_guess *, void *, Renderer_guess *))
#define EQGFX_RENDER_MODE_ONE_SCENE_GUESS EQ_CDECL(void, 0x1003F610, (void *, Renderer_guess *, HierarchicalObjectData_guess *, void *))
#define EQGFX_RENDER_STANDARD_SCENE_GUESS EQ_CDECL(void, 0x1003BBD9, (void *, Renderer_guess *, HierarchicalObjectData_guess *, void *))
#define EQGFX_TEST_SPRITE_BOUNDS_GUESS EQ_CDECL(int, 0x10030DAA, (void *, float *, float))
#define EQGFX_UPDATE_DM_SPRITE_TIMING_GUESS EQ_CDECL(void, 0x10010F45, (HierarchicalObject_guess *, unsigned int))
#define EQGFX_RENDER_OBJECT_MODE_A_GUESS EQ_CDECL(void, 0x10036511, (HierarchicalObject_guess *, float *, Renderer_guess *))
#define EQGFX_RENDER_OBJECT_MODE_B_GUESS EQ_CDECL(void, 0x100368B5, (HierarchicalObject_guess *, float *, Renderer_guess *))
#define EQGFX_RENDER_OBJECT_MODE_C_GUESS EQ_CDECL(void, 0x10036C98, (HierarchicalObject_guess *, float *, Renderer_guess *))
#define EQGFX_UPDATE_OBJECT_RENDER_DATA_GUESS EQ_CDECL(void, 0x1003B392, (HierarchicalObject_guess *, LightTransform_guess *, float *, Renderer_guess *))
#define EQGFX_APPLY_ACTOR_AMBIENT_GUESS EQ_CDECL(void, 0x1003A79C, (HierarchicalObject_guess *, Renderer_guess *, void *, float, float *, float *, float))

/* T3dModelMatrix_guess: the actor layout is T3dModelMatrix_guess (generated: eqgfx/layout/t3d_model_matrix.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3d_model_matrix.h"

typedef struct HierarchicalModelList_guess
{
    unsigned char unknown_00[0x24];
    T3dModelMatrix_guess *models;
} HierarchicalModelList_guess;

typedef struct HierarchicalRenderOutput_guess
{
    unsigned char unknown_00[0x610];
    unsigned int outputBase;
} HierarchicalRenderOutput_guess;

typedef void (__cdecl *TriangleLightingCallback_guess)(unsigned int, unsigned int, unsigned char *, unsigned int, unsigned int);

typedef void (__cdecl *VertexSlotCallback_guess)(void *, unsigned int, unsigned char *, void **);

#define EQGFX_BUILD_TRIANGLE_LIGHTING_GUESS EQ_CDECL(void, 0x10038FAF, (HierarchicalObject_guess *, T3dModelMatrix_guess *, HierarchicalRenderOutput_guess *, unsigned char *))
#define EQGFX_TRIANGLE_LIGHTING_CALLBACK_GUESS EQ_GLOBAL(TriangleLightingCallback_guess, 0x100FC780)
#define EQGFX_BUILD_TRANSFORM_MODEL_GUESS EQ_CDECL(void, 0x10039055, (HierarchicalObject_guess *, T3dModelMatrix_guess *, HierarchicalRenderOutput_guess *, unsigned char *))
#define EQGFX_TRANSFORM_VERTICES_BY_MATRIX_GUESS EQ_CDECL(void, 0x100383EC, (short *, float *, float *, int, char *))
#define EQGFX_TRANSFORM_VERTICES_GUESS EQ_CDECL(void, 0x10038485, (short *, float *, float *, int, char *, float))
#define EQGFX_GET_ACTOR_SCALED_AMBIENT_GUESS EQ_CDECL(float, 0x10037F34, (void *, Renderer_guess *, T3D_tagACTORINSTANCE *))
#define EQGFX_RENDER_OBJECT_PATH_A_GUESS EQ_CDECL(void, 0x1003644A, (HierarchicalObject_guess *, float *, Renderer_guess *))
#define EQGFX_RENDER_OBJECT_PATH_B_GUESS EQ_CDECL(void, 0x100365D3, (HierarchicalObject_guess *, float *, Renderer_guess *))
#define EQGFX_APPLY_OBJECT_AMBIENT_GUESS EQ_CDECL(void, 0x1003A234, (HierarchicalObject_guess *, Renderer_guess *, void *, float))
#define EQGFX_APPLY_OBJECT_LIGHTING_GUESS EQ_CDECL(void, 0x1003A51C, (HierarchicalObject_guess *, Renderer_guess *, float, float *, void *, float))
#define EQGFX_BUILD_OBJECT_TRIANGLES_GUESS EQ_CDECL(void, 0x1003B1AF, (HierarchicalObject_guess *, LightTransform_guess *, float *, Renderer_guess *))
#define EQGFX_RENDER_FLAGGED_OBJECT_GUESS EQ_CDECL(void, 0x1003B69B, (void *, Renderer_guess *, HierarchicalObject_guess *))
#define EQGFX_RENDER_MODE_TWO_PREPARE_GUESS EQ_CDECL(void, 0x1003DED4, (HierarchicalObject_guess *, Renderer_guess *))
#define EQGFX_RENDER_MODE_TWO_SCENE_FINISH_GUESS EQ_CDECL(void, 0x1003EADB, (void *, Renderer_guess *, HierarchicalObject_guess *))
#define EQGFX_RENDER_MODE_ONE_PREPARE_GUESS EQ_CDECL(void, 0x1003E3C2, (HierarchicalObject_guess *, Renderer_guess *))
#define EQGFX_RENDER_MODE_ONE_SCENE_FINISH_GUESS EQ_CDECL(void, 0x1003F28C, (void *, Renderer_guess *, HierarchicalObject_guess *))

// The plane record transformFrustumWithMatrices_guess rewrites: the normal's three floats, the distance
// and four bytes the function never touches (20 bytes each).
typedef struct FrustumPlane_guess
{
    float x;                    // 0x00
    float y;                    // 0x04
    float z;                    // 0x08
    float distance;             // 0x0c
    unsigned int unknown_10;    // 0x10
} FrustumPlane_guess;

// The frustum object transformFrustumWithMatrices_guess reads and writes: two reference points (three
// floats each, at +0xc and +0x54) and six planes at +0x64.
typedef struct Frustum_guess
{
    unsigned char unknown_00[0x0c];
    float point0[3];                // 0x0c
    unsigned char unknown_18[0x3c];
    float point1[3];                // 0x54
    unsigned int unknown_60;        // 0x60
    FrustumPlane_guess planes[6];   // 0x64
} Frustum_guess;

typedef struct SphereCullingContext_guess
{
    unsigned char unknown_000[0x1a0];
    float nearDepth;
    unsigned char unknown_1a4[4];
    float farDepth;
    float originOffset[3];
    unsigned char unknown_1b8[0x6c];
    float orientation[3][3];
    unsigned char unknown_248[0x1b8];
    unsigned int intersectionFlags;
    float horizontalProjectionScale;
    float verticalProjectionScale;
    float horizontalRadiusScale;
    float verticalRadiusScale;
} SphereCullingContext_guess;

#define EQGFX_DISTANCE_THRESHOLD_GUESS EQ_GLOBAL(float, 0x100762EC)
typedef float (__cdecl *DirectionLengthCallback_guess)(float);
#define EQGFX_DIRECTION_LENGTH_CALLBACK_GUESS EQ_GLOBAL(DirectionLengthCallback_guess, 0x100FC77C)
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280)
#define EQGFX_JITTER_SCALE_GUESS EQ_GLOBAL(float, 0x100762E8)

// FUNCTION: EQGFX 0x10033be5
// Lights polygon vertices from a base colour plus each animated point light, all sharing one surface normal.
int __cdecl computeLitVertexColorsFlatNormal_guess(EffectRenderContext_guess *renderContext, int vertexCount,
                         float *viewDirection, float colorX, float colorY, float colorZ)
{
    EffectVertex_guess *vertex;
    EffectVertex_guess *vertexEnd;
    EffectVertex_guess *outputVertex;
    EffectLight_guess **light;
    EffectLight_guess **lightEnd;
    EffectLight_guess *effect;
    EffectAnimation_guess *animation;
    EffectFrameSet_guess *frameSet;
    unsigned int frameIndex;
    unsigned int elapsed;
    unsigned int frameDuration;
    float animatedX;
    float animatedY;
    float animatedZ;
    float attenuation;
    float dx;
    float dy;
    float dz;
    float distanceSquared;
    float inverseDistance;
    float towardness;
    float colorFactor;
    int colorIncrement;

    vertex = renderContext->vertices;
    vertexEnd = vertex + vertexCount;
    for (; vertex < vertexEnd; vertex = vertex + 1)
    {
        vertex->colorX = colorX;
        vertex->colorY = colorY;
        vertex->colorZ = colorZ;
    }

    light = renderContext->lightList;
    lightEnd = light + renderContext->lightCount;
    for (; light < lightEnd; light = light + 1)
    {
        effect = *light;
        if ((effect->type == 0x1e) && (effect != renderContext->excludedLight))
        {
            animation = effect->animation;
            frameSet = animation->frameSet;
            if ((1 < frameSet->frameCount) &&
                ((animation->lastUpdate < renderContext->currentFrame) &&
                 ((animation->flags & 8) == 0)))
            {
                if (animation->unknown_020 == 0)
                {
                    animation->frameIndex = animation->frameIndex + 1;
                    if (frameSet->frameCount <= animation->frameIndex)
                    {
                        animation->frameIndex = 0;
                    }
                    animation->lastUpdate = animation->frameDuration + renderContext->currentFrame;
                }
                else
                {
                    frameDuration = animation->frameDuration;
                    elapsed = renderContext->currentFrame - animation->lastUpdate;
                    if ((frameDuration << 4) < elapsed)
                    {
                        animation->frameIndex = animation->frameIndex + elapsed / frameDuration + 1;
                        elapsed = elapsed - (elapsed / frameDuration) * animation->frameDuration;
                        animation->frameIndex = animation->frameIndex % frameSet->frameCount;
                    }
                    else
                    {
                        for (; frameDuration < elapsed; elapsed = elapsed - frameDuration)
                        {
                            animation->frameIndex = animation->frameIndex + 1;
                            if (frameSet->frameCount <= animation->frameIndex)
                            {
                                animation->frameIndex = 0;
                            }
                        }
                        animation->frameIndex = animation->frameIndex + 1;
                        if (frameSet->frameCount <= animation->frameIndex)
                        {
                            animation->frameIndex = 0;
                        }
                    }
                    animation->lastUpdate = (renderContext->currentFrame - elapsed) + animation->frameDuration;
                }
            }

            if (frameSet->framePositions != 0)
            {
                animatedX = frameSet->framePositions[animation->frameIndex * 3];
                animatedY = frameSet->framePositions[animation->frameIndex * 3 + 1];
                animatedZ = frameSet->framePositions[animation->frameIndex * 3 + 2];
            }
            else if (1 < frameSet->frameCount)
            {
                animatedX = frameSet->values.frameValues[animation->frameIndex];
                animatedY = frameSet->values.frameValues[animation->frameIndex];
                animatedZ = frameSet->values.frameValues[animation->frameIndex];
            }
            else
            {
                animatedX = frameSet->values.oneFrameValue;
                animatedY = frameSet->values.oneFrameValue;
                animatedZ = frameSet->values.oneFrameValue;
            }

            attenuation = effect->strength * effect->strength * EQGFX_POINT_RADIUS_SCALE_GUESS;
            animatedX = attenuation * animatedX;
            animatedY = attenuation * animatedY;
            animatedZ = attenuation * animatedZ;
            for (vertex = renderContext->vertices; vertex < vertexEnd; vertex = vertex + 1)
            {
                dx = effect->x - vertex->positionX;
                dy = effect->y - vertex->positionY;
                dz = effect->z - vertex->positionZ;
                distanceSquared = dx * dx + dy * dy + dz * dz;
                towardness = dx * viewDirection[0] + dy * viewDirection[1] + dz * viewDirection[2];
                if ((EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared) &&
                    ((inverseDistance = (*EQGFX_DIRECTION_LENGTH_CALLBACK_GUESS)(distanceSquared)),
                     (0.0f < towardness * inverseDistance)))
                {
                    colorFactor = (towardness * inverseDistance) /
                                  (distanceSquared + EQGFX_FLOAT_ONE);
                    colorFactor = colorFactor * EQGFX_JITTER_SCALE_GUESS;
                    __asm
                    {
                        fld colorFactor
                        fmul animatedX
                    }
                    colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                    *((unsigned int *)&vertex->colorX) = *((unsigned int *)&vertex->colorX) + colorIncrement;
                    __asm
                    {
                        fld colorFactor
                        fmul animatedY
                    }
                    colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                    *((unsigned int *)&vertex->colorY) = *((unsigned int *)&vertex->colorY) + colorIncrement;
                    __asm
                    {
                        fld colorFactor
                        fmul animatedZ
                    }
                    colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                    *((unsigned int *)&vertex->colorZ) = *((unsigned int *)&vertex->colorZ) + colorIncrement;
                }
            }
        }
    }

    if (renderContext->vertices < vertexEnd)
    {
        outputVertex = renderContext->outputVertices;
        vertex = renderContext->vertices;
        do
        {
            outputVertex->colorX = vertex->colorX;
            outputVertex->colorY = vertex->colorY;
            outputVertex->colorZ = vertex->colorZ;
            outputVertex->textureX = vertex->textureX;
            outputVertex->textureY = vertex->textureY;
            outputVertex = outputVertex + 1;
            vertex = vertex + 1;
        } while (vertex < vertexEnd);
    }
    return 1;
}

// FUNCTION: EQGFX 0x10033EBC
// Lights vertices by their own normals: base colour plus flag-enabled animated point and directional lights.
int __cdecl computeLitVertexColorsVertexNormals_guess(EffectRenderContext_guess *renderContext, int vertexCount,
                         float initialColorX, float initialColorY, float initialColorZ,
                         unsigned int flags)
{
    EffectVertex_guess *vertexEnd;
    EffectVertex_guess *vertex;
    EffectVertex_guess *outputVertex;
    EffectLight_guess **effectCursor;
    EffectLight_guess **effectEnd;
    EffectLight_guess *effect;
    EffectDirectionalLight_guess *directionalEffect;
    EffectAnimation_guess *animation;
    EffectFrameSet_guess *frameSet;
    unsigned int frameDuration;
    unsigned int elapsedFrames;
    unsigned int colorIncrement;
    float animatedX;
    float animatedY;
    float animatedZ;
    float attenuation;
    float dx;
    float dy;
    float dz;
    float distanceSquared;
    float towardness;
    float inverseDistance;
    float colorFactor;
    float colorScale;

    vertexEnd = renderContext->vertices + vertexCount;
    for (vertex = renderContext->vertices; vertex < vertexEnd; vertex = vertex + 1)
    {
        vertex->colorX = initialColorX;
        vertex->colorY = initialColorY;
        vertex->colorZ = initialColorZ;
    }

    effectCursor = renderContext->lightList;
    effectEnd = effectCursor + renderContext->lightCount;
    for (; effectCursor < effectEnd; effectCursor = effectCursor + 1)
    {
        effect = *effectCursor;
        if (effect->type == EQGFX_EFFECT_TYPE_POINT)
        {
            if (((flags & EQGFX_EFFECT_POINT_COLOR_ENABLED) != 0) &&
                (effect != renderContext->excludedLight))
            {
                animation = effect->animation;
                frameSet = animation->frameSet;
                if ((1 < frameSet->frameCount) &&
                    ((animation->lastUpdate < renderContext->currentFrame) &&
                     ((animation->flags & EQGFX_EFFECT_ANIMATION_PAUSED) == 0)))
                {
                    if (animation->unknown_020 == 0)
                    {
                        animation->frameIndex = animation->frameIndex + 1;
                        if (frameSet->frameCount <= animation->frameIndex)
                        {
                            animation->frameIndex = 0;
                        }
                        animation->lastUpdate = animation->frameDuration + renderContext->currentFrame;
                    }
                    else
                    {
                        frameDuration = animation->frameDuration;
                        elapsedFrames = renderContext->currentFrame - animation->lastUpdate;
                        if ((frameDuration << 4) < elapsedFrames)
                        {
                            animation->frameIndex = animation->frameIndex + elapsedFrames / frameDuration + 1;
                            animation->frameIndex = animation->frameIndex % frameSet->frameCount;
                            elapsedFrames = elapsedFrames - (elapsedFrames / frameDuration) * frameDuration;
                        }
                        else
                        {
                            for (; frameDuration < elapsedFrames; elapsedFrames = elapsedFrames - frameDuration)
                            {
                                animation->frameIndex = animation->frameIndex + 1;
                                if (frameSet->frameCount <= animation->frameIndex)
                                {
                                    animation->frameIndex = 0;
                                }
                            }
                            animation->frameIndex = animation->frameIndex + 1;
                            if (frameSet->frameCount <= animation->frameIndex)
                            {
                                animation->frameIndex = 0;
                            }
                        }
                        animation->lastUpdate = (frameDuration - elapsedFrames) + renderContext->currentFrame;
                    }
                }

                if (frameSet->framePositions != 0)
                {
                    animatedX = frameSet->framePositions[animation->frameIndex * 3];
                    animatedY = frameSet->framePositions[animation->frameIndex * 3 + 1];
                    animatedZ = frameSet->framePositions[animation->frameIndex * 3 + 2];
                }
                else if (1 < frameSet->frameCount)
                {
                    animatedX = frameSet->values.frameValues[animation->frameIndex];
                    animatedY = frameSet->values.frameValues[animation->frameIndex];
                    animatedZ = frameSet->values.frameValues[animation->frameIndex];
                }
                else
                {
                    animatedX = frameSet->values.oneFrameValue;
                    animatedY = frameSet->values.oneFrameValue;
                    animatedZ = frameSet->values.oneFrameValue;
                }

                attenuation = effect->strength * effect->strength * EQGFX_POINT_RADIUS_SCALE_GUESS;
                animatedX = attenuation * animatedX;
                animatedY = attenuation * animatedY;
                animatedZ = attenuation * animatedZ;
                for (vertex = renderContext->vertices; vertex < vertexEnd; vertex = vertex + 1)
                {
                    dx = effect->x - vertex->positionX;
                    dy = effect->y - vertex->positionY;
                    dz = effect->z - vertex->positionZ;
                    distanceSquared = dx * dx + dy * dy + dz * dz;
                    towardness = dx * vertex->unknown_018[0] +
                                 dy * vertex->unknown_018[1] +
                                 dz * vertex->unknown_018[2];
                    if ((EQGFX_DISTANCE_THRESHOLD_GUESS < distanceSquared) &&
                        ((inverseDistance = (*EQGFX_DIRECTION_LENGTH_CALLBACK_GUESS)(distanceSquared)),
                         (0.0f < towardness * inverseDistance)))
                    {
                        colorFactor = (towardness * inverseDistance) /
                                      (distanceSquared + EQGFX_FLOAT_ONE);
                        colorFactor = colorFactor * EQGFX_JITTER_SCALE_GUESS;
                        __asm
                        {
                            fld colorFactor
                            fmul animatedX
                        }
                        colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                        *((unsigned int *)&vertex->colorX) = *((unsigned int *)&vertex->colorX) + colorIncrement;
                        __asm
                        {
                            fld colorFactor
                            fmul animatedY
                        }
                        colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                        *((unsigned int *)&vertex->colorY) = *((unsigned int *)&vertex->colorY) + colorIncrement;
                        __asm
                        {
                            fld colorFactor
                            fmul animatedZ
                        }
                        colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                        *((unsigned int *)&vertex->colorZ) = *((unsigned int *)&vertex->colorZ) + colorIncrement;
                    }
                }
            }
        }
        else if ((effect->type == EQGFX_EFFECT_TYPE_DIRECTIONAL) &&
                 ((flags & EQGFX_EFFECT_DIRECTIONAL_COLOR_ENABLED) != 0))
        {
            directionalEffect = (EffectDirectionalLight_guess *)effect;
            animation = directionalEffect->animation;
            frameSet = animation->frameSet;
            if (1 < frameSet->frameCount)
            {
                if ((animation->lastUpdate < renderContext->currentFrame) &&
                    ((animation->flags & EQGFX_EFFECT_ANIMATION_PAUSED) == 0))
                {
                    if (animation->unknown_020 == 0)
                    {
                        animation->frameIndex = animation->frameIndex + 1;
                        if (frameSet->frameCount <= animation->frameIndex)
                        {
                            animation->frameIndex = 0;
                        }
                        animation->lastUpdate = animation->frameDuration + renderContext->currentFrame;
                    }
                    else
                    {
                        frameDuration = animation->frameDuration;
                        elapsedFrames = renderContext->currentFrame - animation->lastUpdate;
                        if ((frameDuration << 4) < elapsedFrames)
                        {
                            animation->frameIndex = animation->frameIndex + elapsedFrames / frameDuration + 1;
                            elapsedFrames = elapsedFrames - (elapsedFrames / frameDuration) * frameDuration;
                            animation->frameIndex = animation->frameIndex % frameSet->frameCount;
                        }
                        else
                        {
                            for (; frameDuration < elapsedFrames; elapsedFrames = elapsedFrames - frameDuration)
                            {
                                animation->frameIndex = animation->frameIndex + 1;
                                if (frameSet->frameCount <= animation->frameIndex)
                                {
                                    animation->frameIndex = 0;
                                }
                            }
                            animation->frameIndex = animation->frameIndex + 1;
                            if (frameSet->frameCount <= animation->frameIndex)
                            {
                                animation->frameIndex = 0;
                            }
                        }
                        animation->lastUpdate = (frameDuration - elapsedFrames) + renderContext->currentFrame;
                    }
                }
            }

            if (frameSet->framePositions != 0)
            {
                animatedX = frameSet->framePositions[animation->frameIndex * 3];
                animatedY = frameSet->framePositions[animation->frameIndex * 3 + 1];
                animatedZ = frameSet->framePositions[animation->frameIndex * 3 + 2];
            }
            else if (1 < frameSet->frameCount)
            {
                animatedX = frameSet->values.frameValues[animation->frameIndex];
                animatedY = frameSet->values.frameValues[animation->frameIndex];
                animatedZ = frameSet->values.frameValues[animation->frameIndex];
            }
            else
            {
                animatedX = frameSet->values.oneFrameValue;
                animatedY = frameSet->values.oneFrameValue;
                animatedZ = frameSet->values.oneFrameValue;
            }

            colorScale = EQGFX_JITTER_SCALE_GUESS;
            for (vertex = renderContext->vertices; vertex < vertexEnd; vertex = vertex + 1)
            {
                towardness = -(directionalEffect->x * vertex->unknown_018[0] +
                               directionalEffect->y * vertex->unknown_018[1] +
                               directionalEffect->z * vertex->unknown_018[2]);
                if (0.0f < towardness)
                {
                    __asm
                    {
                        fld towardness
                        fmul animatedX
                        fmul colorScale
                    }
                    colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                    *((unsigned int *)&vertex->colorX) = *((unsigned int *)&vertex->colorX) + colorIncrement;
                    __asm
                    {
                        fld towardness
                        fmul animatedY
                        fmul colorScale
                    }
                    colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                    *((unsigned int *)&vertex->colorY) = *((unsigned int *)&vertex->colorY) + colorIncrement;
                    __asm
                    {
                        fld towardness
                        fmul animatedZ
                        fmul colorScale
                    }
                    colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                    *((unsigned int *)&vertex->colorZ) = *((unsigned int *)&vertex->colorZ) + colorIncrement;
                }
            }
        }
    }

    if (renderContext->vertices < vertexEnd)
    {
        outputVertex = renderContext->outputVertices;
        vertex = renderContext->vertices;
        do
        {
            outputVertex->colorX = vertex->colorX;
            outputVertex->colorY = vertex->colorY;
            outputVertex->colorZ = vertex->colorZ;
            outputVertex->textureX = vertex->textureX;
            outputVertex->textureY = vertex->textureY;
            outputVertex = outputVertex + 1;
            vertex = vertex + 1;
        } while (vertex < vertexEnd);
    }
    return 1;
}

#define EQGFX_FLOAT_IS_NAN(value) ((*(unsigned int *)&(value) & 0x7fffffffu) > 0x7f800000u)

// FUNCTION: EQGFX 0x10034302
// Shades effect vertices according to how their normals face a directional light.
// __cdecl: adds the lit colour to each vertex, the normal's facing of the direction (in the render state's frame); returns 1.
int __cdecl applyDirectionalLightingToEffectVertexColors_guess(RenderState_guess *renderState, EffectVertex_guess *vertices, int vertexCount, float *direction)
{
    EffectVertex_guess *vertex;
    EffectVertex_guess *vertexEnd;
    float colorR;
    float colorG;
    float colorB;
    float frameX;
    float frameY;
    float frameZ;
    float x;
    float y;
    float z;
    float lengthSquared;
    float roundingBias;
    float colorScale;
    double towardness;
    unsigned int colorIncrement;

    colorR = *(float *)&renderState->unknown_1b8[0x00];
    colorG = *(float *)&renderState->unknown_1b8[0x04];
    colorB = *(float *)&renderState->unknown_1b8[0x08];
    frameX = direction[2] * renderState->xFromZ + direction[1] * renderState->xFromY + direction[0] * renderState->xFromX;
    frameY = direction[2] * renderState->yFromZ + direction[1] * renderState->yFromY + direction[0] * renderState->yFromX;
    frameZ = direction[2] * renderState->zFromZ + direction[1] * renderState->zFromY + direction[0] * renderState->zFromX;
    vertexEnd = vertices + vertexCount;
    if (vertices < vertexEnd)
    {
        vertex = vertices;
        do
        {
            x = vertex->positionX;
            y = vertex->positionY;
            z = vertex->positionZ;
            lengthSquared = z * z + y * y + x * x;
            if (lengthSquared > EQGFX_DISTANCE_THRESHOLD_GUESS && !EQGFX_FLOAT_IS_NAN(lengthSquared))
            {
                towardness = -(EQGFX_EFFECT_SQRT_GUESS(lengthSquared) * (frameZ * z + frameY * y + frameX * x));
                if (towardness >= EQGFX_ZERO_F_GUESS)
                {
                    goto updateVertex;
                }
            }
            else
            {
                towardness = 0.0;
            updateVertex:
                roundingBias = EQGFX_FLOAT_ONE;
                colorScale = EQGFX_JITTER_SCALE_GUESS;
                __asm {
                    fld qword ptr towardness
                    fld lengthSquared
                    fadd roundingBias
                    fdivp st(1), st(0)
                }
                __asm {
                    fld st(0)
                    fmul colorR
                    fmul colorScale
                }
                colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                *((unsigned int *)&vertex->colorX) = *((unsigned int *)&vertex->colorX) + colorIncrement;
                __asm {
                    fld st(0)
                    fmul colorG
                    fmul colorScale
                }
                colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                *((unsigned int *)&vertex->colorY) = *((unsigned int *)&vertex->colorY) + colorIncrement;
                __asm {
                    fld st(0)
                    fmul colorB
                    fmul colorScale
                }
                colorIncrement = EQGFX_ROUND_ST0_TO_UINT_GUESS();
                *((unsigned int *)&vertex->colorZ) = *((unsigned int *)&vertex->colorZ) + colorIncrement;
                __asm {
                    fstp st(0)
                }
            }
            vertex = vertex + 1;
        } while (vertex < vertexEnd);
    }
    return 1;
}

// FUNCTION: EQGFX 0x10034475
// Adds lit colour to each vertex by how its normal faces its own 0x18 vector turned into the render frame.
int __cdecl applyPerVertexDirectionLightToColors_guess(TriangleLightingTransformInput_guess *lightingContext, float *vertexBuffer,
                         int vertexCount)
{
    float outputAdjustmentX;
    float outputAdjustmentY;
    float outputAdjustmentZ;
    float firstSourceX;
    float firstSourceY;
    float firstSourceZ;
    float secondSourceX;
    float secondSourceY;
    float secondSourceZ;
    float thirdSourceX;
    float thirdSourceY;
    float thirdSourceZ;
    float normalX;
    float normalY;
    float normalZ;
    float normalLengthSquared;
    double projectionValue;
    float roundingBias;
    float colorScale;
    float *vertex;
    float *nextVertexEnd;
    int roundedIncrement;

    outputAdjustmentX = lightingContext->outputAdjustmentX;
    outputAdjustmentY = lightingContext->outputAdjustmentY;
    outputAdjustmentZ = lightingContext->outputAdjustmentZ;
    firstSourceX = lightingContext->firstSourceX;
    firstSourceY = lightingContext->firstSourceY;
    firstSourceZ = lightingContext->firstSourceZ;
    secondSourceX = lightingContext->secondSourceX;
    secondSourceY = lightingContext->secondSourceY;
    secondSourceZ = lightingContext->secondSourceZ;
    thirdSourceX = lightingContext->thirdSourceX;
    thirdSourceY = lightingContext->thirdSourceY;
    thirdSourceZ = lightingContext->thirdSourceZ;
    if (vertexBuffer < vertexBuffer + vertexCount * 0x0c) {
        vertex = vertexBuffer + 2;
        do {
            normalX = vertex[-2];
            normalY = vertex[-1];
            normalZ = *vertex;
            normalLengthSquared = normalX * normalX + normalY * normalY + normalZ * normalZ;
            if (EQGFX_DISTANCE_THRESHOLD_GUESS < normalLengthSquared) {
                projectionValue =
                    -(((vertex[4] * firstSourceX + vertex[5] * firstSourceY + vertex[6] * firstSourceZ) * normalX +
                       (vertex[4] * secondSourceX + vertex[5] * secondSourceY + vertex[6] * secondSourceZ) * normalY +
                       (vertex[4] * thirdSourceX + vertex[5] * thirdSourceY + vertex[6] * thirdSourceZ) * normalZ) *
                      EQGFX_DIRECTION_LENGTH_CALLBACK_GUESS(normalLengthSquared));
                if (EQGFX_ZERO_F_GUESS <= projectionValue)
                    goto updateVertex;
            } else {
                projectionValue = 0.0f;
            updateVertex:
                roundingBias = EQGFX_FLOAT_ONE;
                colorScale = EQGFX_JITTER_SCALE_GUESS;
                __asm {
                    fld qword ptr projectionValue
                    fld normalLengthSquared
                    fadd roundingBias
                    fdivp st(1), st(0)
                }
                __asm {
                    fld st(0)
                    fmul outputAdjustmentX
                    fmul colorScale
                }
                roundedIncrement = (int)EQGFX_ROUND_ST0_TO_UINT_GUESS();
                ((int *)vertex)[7] += roundedIncrement;
                __asm {
                    fld st(0)
                    fmul outputAdjustmentY
                    fmul colorScale
                }
                roundedIncrement = (int)EQGFX_ROUND_ST0_TO_UINT_GUESS();
                ((int *)vertex)[8] += roundedIncrement;
                __asm {
                    fld st(0)
                    fmul outputAdjustmentZ
                    fmul colorScale
                }
                roundedIncrement = (int)EQGFX_ROUND_ST0_TO_UINT_GUESS();
                ((int *)vertex)[9] += roundedIncrement;
                __asm {
                    fstp st(0)
                }
            }
            nextVertexEnd = vertex + 10;
            vertex = vertex + 0x0c;
        } while (nextVertexEnd < vertexBuffer + vertexCount * 0x0c);
    }
    return 1;
}

// FUNCTION: EQGFX 0x1003461a
// Draws an actor's 2D sprite as a camera-facing quad, the image chosen by view angle and animation frame.
int __cdecl _t3dPaint2DSprite(RenderTarget_guess *source, Renderer_guess *renderer, int *entryIndex)
{
    EffectInstance_guess *effect;
    EffectFrameDescriptor_guess *descriptor;
    T3D_tagACTORINSTANCE *selected;
    unsigned char *rotationTable;
    unsigned char angleOutput[4];
    unsigned int rawFlags;
    unsigned int renderFlags;
    unsigned int rotationIndex;
    unsigned int firstRotationIndex;
    unsigned int secondRotationIndex;
    unsigned int thirdRotationIndex;
    unsigned int frameAdvance;
    unsigned int *frameData;
    unsigned char *curveRecord;
    float *frameCurves;
    float *curveEnd;
    float *subCurves;
    float *subCurveEnd;
    float firstSine;
    float firstCosine;
    float secondSine;
    float secondCosine;
    float thirdSine;
    float thirdCosine;
    float crossTerm;
    float basisXX;
    float basisXY;
    float basisXZ;
    float basisYX;
    float basisYY;
    float basisYZ;
    float basisZX;
    float basisZY;
    float basisZZ;
    float originX;
    float originY;
    float originZ;
    float effectX;
    float effectY;
    float effectZ;
    float combinedX;
    float combinedY;
    float combinedZ;
    float viewX;
    float viewY;
    float viewZ;
    float horizontalSquared;
    float viewYSquared;
    float planeSquared;
    float planeRoot;
    float verticalAngle;
    float horizontalAngle;
    float curveAngle;
    unsigned int elapsedTime;
    unsigned int frameRemainder;
    float distanceSquared;
    float distanceRoot;
    float radiusSquared;
    float radiusRoot;
    float axisX;
    float axisY;
    float axisZ;
    float rotationScale;
    float rotationX;
    float rotationY;
    float rotationZ;
    float mixXX;
    float mixXY;
    float mixXZ;
    float mixYX;
    float mixYY;
    float mixYZ;
    float mixZX;
    float mixZY;
    float mixZZ;
    float firstExtent;
    float secondExtent;
    float thirdExtent;
    float leftX;
    float rightX;
    float lowerY;
    float upperY;
    float vertexX;
    float vertexY;
    float vertexZ;
    float sourceX;
    float sourceY;
    float sourceZ;
    float *quad;
    float *quadBase;
    float *quadVertex;
    float *quadEnd;
    float *sourceVertex;
    float *destinationVertex;
    float *destinationEnd;
    int result = 1;
    unsigned int nextUpdate;
    int frameCount;
    unsigned int curveCount;
    union
    {
        float asFloat;
        unsigned int asBits;
    } curvePointer;

    effect = (EffectInstance_guess *)renderer->object;
    selected = source->actors[*entryIndex];
    descriptor = effect->frameDescriptor;
    if (selected->frameStamp_guess == EQGFX_EFFECT_CACHE_STAMP_GUESS)
        return 1;

    rawFlags = effect->attributeFlags;
    if ((rawFlags & 0x80000000) != 0)
        rawFlags = renderer->attributeValues[rawFlags & 0x7fffffff];
    renderFlags = rawFlags & 0xffffffdf;
    if (renderFlags == 0)
    {
        result = 1;
    }
    else
    {
        if ((renderer->renderMode != 0 || renderer->secondaryRenderMode != 0) &&
            (char)(renderFlags >> 8) != 0)
            renderFlags = (rawFlags & 0xffff04df) | 0x400;

        __asm {
            mov eax, selected
            fld dword ptr [eax + 0x1c]
        }
        firstRotationIndex = EQGFX_ROUND_ST0_TO_UINT_GUESS();
        firstRotationIndex &= 0x1ff;
        __asm {
            mov eax, selected
            fld dword ptr [eax + 0x20]
        }
        secondRotationIndex = EQGFX_ROUND_ST0_TO_UINT_GUESS();
        secondRotationIndex &= 0x1ff;
        __asm {
            mov eax, selected
            fld dword ptr [eax + 0x24]
        }
        thirdRotationIndex = EQGFX_ROUND_ST0_TO_UINT_GUESS();
        thirdRotationIndex &= 0x1ff;

        rotationTable = t3d_angle_tree_guess;
        firstSine = *(float *)(rotationTable + 0x1800 + firstRotationIndex * 4);
        firstCosine = *(float *)(rotationTable + 0x1000 + firstRotationIndex * 4);
        secondSine = -*(float *)(rotationTable + 0x1800 + secondRotationIndex * 4);
        secondCosine = *(float *)(rotationTable + 0x1000 + secondRotationIndex * 4);
        thirdSine = *(float *)(rotationTable + 0x1800 + thirdRotationIndex * 4);
        thirdCosine = *(float *)(rotationTable + 0x1000 + thirdRotationIndex * 4);

        crossTerm = thirdCosine * secondSine;
        basisXX = secondCosine * firstCosine;
        basisYX = secondCosine * firstSine;
        basisZX = -secondSine;
        basisXY = thirdSine * secondSine * firstCosine - thirdCosine * firstSine;
        basisYY = thirdSine * secondSine * firstSine + thirdCosine * firstCosine;
        basisZY = thirdSine * secondCosine;
        basisXZ = thirdSine * firstSine + crossTerm * firstCosine;
        basisYZ = crossTerm * firstSine - thirdSine * firstCosine;
        originX = renderer->translation[0];
        thirdCosine = thirdCosine * secondCosine;
        originY = renderer->translation[1];
        originZ = renderer->translation[2];
        combinedX = originX + selected->position_guess[0];
        combinedY = originY + selected->position_guess[1];
        combinedZ = originZ + selected->position_guess[2];
        viewX = -(combinedX * basisXX + combinedY * basisYX + combinedZ * basisZX);
        viewY = -(combinedX * basisXY + combinedY * basisYY + combinedZ * basisZY);
        viewZ = -(combinedX * basisXZ + combinedY * basisYZ + combinedZ * thirdCosine);
        basisZZ = thirdCosine;

        if (1 < descriptor->frameCount && effect->lastUpdate < source->currentTime &&
            (effect->flags & 0x08) == 0)
        {
            if (effect->animationState == 0)
            {
                effect->frameIndex = effect->frameIndex + 1;
                if (descriptor->frameCount <= effect->frameIndex)
                    effect->frameIndex = 0;
                nextUpdate = effect->frameDuration + source->currentTime;
            }
            else
            {
                elapsedTime = renderer->spriteTiming - effect->lastUpdate;
                if ((unsigned int)(effect->frameDuration << 4) < elapsedTime)
                {
                    frameAdvance = elapsedTime / (unsigned int)effect->frameDuration;
                    effect->frameIndex = effect->frameIndex + frameAdvance + 1;
                    frameRemainder = elapsedTime - frameAdvance * effect->frameDuration;
                    effect->frameIndex = effect->frameIndex % descriptor->frameCount;
                }
                else
                {
                    frameRemainder = elapsedTime;
                    if ((unsigned int)effect->frameDuration < elapsedTime)
                    {
                        do
                        {
                            effect->frameIndex = effect->frameIndex + 1;
                            if (descriptor->frameCount <= effect->frameIndex)
                                effect->frameIndex = 0;
                            frameRemainder = frameRemainder - (unsigned int)effect->frameDuration;
                        } while ((unsigned int)effect->frameDuration < frameRemainder);
                    }
                    effect->frameIndex = effect->frameIndex + 1;
                    if (descriptor->frameCount <= effect->frameIndex)
                        effect->frameIndex = 0;
                }
                nextUpdate = (renderer->spriteTiming - frameRemainder) + effect->frameDuration;
            }
            effect->lastUpdate = nextUpdate;
        }

        viewYSquared = viewY * viewY;
        horizontalSquared = viewX * viewX + viewYSquared;
        planeSquared = horizontalSquared;
        verticalAngle = viewZ * viewZ;
        planeRoot = EQGFX_EFFECT_SQRT_GUESS(planeSquared);
        frameCurves = descriptor->curves;
        if (1 < descriptor->curveCount)
        {
            if (planeSquared <= EQGFX_EFFECT_DISTANCE_EPSILON)
            {
                if (viewZ <= 0.0f)
                    curveAngle = EQGFX_EFFECT_NEGATIVE_VERTICAL_ANGLE;
                else
                    curveAngle = c_slerpAngleScale;
            }
            else
            {
                curveAngle = EQGFX_EFFECT_ANGLE_GUESS(viewZ, planeRoot * planeSquared, (char *)angleOutput);
                if (parseColorScale < curveAngle)
                    curveAngle = curveAngle - EQGFX_EFFECT_ANGLE_WRAP_AMOUNT;
            }
            curveEnd = frameCurves + descriptor->curveCount * 4 - 4;
            while (frameCurves < curveEnd && *frameCurves <= curveAngle)
                frameCurves = frameCurves + 4;
        }

        curvePointer.asFloat = frameCurves[2];
        if (curvePointer.asBits == 1)
        {
            curvePointer.asFloat = frameCurves[3];
            curveRecord = (unsigned char *)curvePointer.asBits;
            curveRecord = (unsigned char *)*(unsigned int *)(curveRecord + 4);
            frameData = *(unsigned int **)(curveRecord + effect->frameIndex * 4);
        }
        else
        {
            if (planeSquared <= EQGFX_EFFECT_DISTANCE_EPSILON)
                horizontalAngle = 0.0f;
            else
                horizontalAngle = EQGFX_EFFECT_ANGLE_GUESS(viewY, -viewX, (char *)angleOutput);
            curvePointer.asFloat = frameCurves[3];
            subCurves = (float *)curvePointer.asBits;
            curveCount = curvePointer.asBits;
            subCurveEnd = subCurves + curveCount * 2 - 2;
            while (subCurves < subCurveEnd && *subCurves <= horizontalAngle)
                subCurves = subCurves + 2;
            curvePointer.asFloat = subCurves[1];
            curveRecord = (unsigned char *)curvePointer.asBits;
            frameData = *(unsigned int **)(curveRecord + effect->frameIndex * 4);
        }
        effect->currentFrameData = *frameData;

        if (renderer->renderBufferCount < 10)
        {
            EQGFX_EFFECT_FREE_BUFFERS_GUESS(renderer);
            result = EQGFX_EFFECT_ALLOC_BUFFERS_GUESS(renderer, 0x1a);
            if (result < 0)
                goto finish;
        }

        radiusSquared = planeSquared + verticalAngle;
        effectX = effect->effectOffset[0];
        effectY = effect->effectOffset[1];
        effectZ = effect->effectOffset[2];
        radiusRoot = planeRoot;
        if (frameCurves[1] == 0.0f)
        {
            if (radiusSquared <= EQGFX_EFFECT_DISTANCE_EPSILON)
            {
                radiusRoot = rotationY;
            }
            else
            {
                radiusRoot = EQGFX_EFFECT_SQRT_GUESS(radiusSquared);
                axisX = radiusRoot * viewX;
                axisY = radiusRoot * viewY;
                crossTerm = radiusRoot * viewZ;
                rotationZ = radiusRoot * viewZ;
                rotationY = 0.0f;
                if (planeSquared <= EQGFX_EFFECT_DISTANCE_EPSILON)
                {
                    viewYSquared = 0.0f;
                    rotationX = 0.0f;
                    rotationScale = 0.0f;
                    planeSquared = 1.0f;
                }
                else
                {
                    viewYSquared = -(planeRoot * viewY);
                    planeSquared = planeRoot * viewX;
                    crossTerm = axisX * planeRoot * viewZ;
                    rotationX = -(axisY * planeRoot * viewZ);
                    rotationScale = radiusRoot / planeRoot;
                }
                crossTerm = -crossTerm;
                distanceRoot = rotationScale;
            }

            firstExtent = descriptor->horizontalScale * selected->scaleFactor_guess;
            secondExtent = descriptor->verticalScale * selected->scaleFactor_guess;
            quad = renderer->vertices;
            vertexZ = effectZ - secondExtent;
            quad[0] = effectX;
            quad[1] = effectY - firstExtent;
            quad[2] = vertexZ;
            quad[12] = effectX;
            quad[13] = effectY - firstExtent;
            vertexZ = secondExtent + effectZ;
            quad[14] = vertexZ;
            quad[24] = effectX;
            quad[25] = effectY + firstExtent;
            quad[26] = vertexZ;
            quad[36] = effectX;
            quad[37] = effectY + firstExtent;
            quad[38] = effectZ - secondExtent;
        }
        else
        {
            if (radiusSquared <= EQGFX_EFFECT_DISTANCE_EPSILON)
            {
                radiusRoot = rotationY;
            }
            else
            {
                radiusRoot = EQGFX_EFFECT_SQRT_GUESS(radiusSquared);
                rotationY = radiusRoot;
                rotationZ = verticalAngle + viewYSquared;
                viewYSquared = 0.0f;
                crossTerm = radiusRoot * viewX;
                rotationX = radiusRoot * viewY;
                rotationScale = radiusRoot * viewZ;
                if (viewZ < 0.0f)
                    rotationScale = -rotationScale;
                distanceRoot = rotationScale;
                if (rotationZ <= EQGFX_EFFECT_DISTANCE_EPSILON)
                {
                    axisX = 0.0f;
                    axisY = 0.0f;
                    rotationY = 0.0f;
                    planeSquared = 1.0f;
                    rotationZ = distanceRoot;
                }
                else
                {
                    radiusRoot = EQGFX_EFFECT_SQRT_GUESS(rotationZ);
                    thirdExtent = radiusRoot;
                    radiusRoot = rotationY;
                    axisX = radiusRoot / thirdExtent;
                    axisY = -(thirdExtent * rotationX * viewX);
                    planeSquared = thirdExtent * viewZ;
                    if (viewZ < 0.0f)
                        planeSquared = -planeSquared;
                    rotationY = -(thirdExtent * viewY);
                    rotationZ = thirdExtent * distanceRoot * viewX;
                }
                rotationZ = -rotationZ;
            }

            thirdExtent = descriptor->depthScale * selected->scaleFactor_guess;
            firstExtent = descriptor->horizontalScale * selected->scaleFactor_guess;
            if (viewZ < 0.0f)
            {
                leftX = effectX - thirdExtent;
                quad = renderer->vertices;
                quad[0] = leftX;
                quad[2] = effectZ;
                quad[1] = effectY - firstExtent;
                rightX = thirdExtent + effectX;
                quad[12] = rightX;
                quad[13] = effectY - firstExtent;
                quad[14] = effectZ;
            }
            else
            {
                leftX = thirdExtent + effectX;
                quad = renderer->vertices;
                quad[0] = leftX;
                quad[2] = effectZ;
                quad[1] = effectY - firstExtent;
                rightX = effectX - thirdExtent;
                quad[12] = rightX;
                quad[13] = effectY - firstExtent;
                quad[14] = effectZ;
            }
            quad[24] = rightX;
            quad[25] = effectY + firstExtent;
            quad[26] = effectZ;
            quad[36] = leftX;
            quad[37] = effectY + firstExtent;
            quad[38] = effectZ;
        }

        if (EQGFX_EFFECT_DISTANCE_EPSILON < radiusSquared)
        {
            mixXX = rotationZ * basisXZ;
            axisZ = basisZY;
            mixXY = viewYSquared * basisXX;
            mixXZ = rotationY * basisXZ;
            basisXZ = distanceRoot * basisXZ + crossTerm * basisXX + rotationX * basisXY;
            mixYX = rotationZ * basisYZ;
            mixYY = viewYSquared * basisYX;
            mixYZ = rotationY * basisYZ;
            basisYZ = distanceRoot * basisYZ + crossTerm * basisYX + rotationX * basisYY;
            mixZX = rotationZ * basisZZ;
            mixZY = viewYSquared * basisZX;
            mixZZ = rotationY * basisZZ;
            basisZZ = distanceRoot * basisZZ + crossTerm * basisZX + rotationX * axisZ;
            basisZX = axisX * basisZX + axisY * axisZ + mixZX;
            basisXX = axisX * basisXX + axisY * basisXY + mixXX;
            basisYX = axisX * basisYX + axisY * basisYY + mixYX;
            basisXY = planeSquared * basisXY + mixXZ + mixXY;
            basisZY = planeSquared * axisZ + mixZZ + mixZY;
            basisYY = planeSquared * basisYY + mixYZ + mixYY;
        }

        quadBase = quad;
        quadVertex = quadBase + 2;
        quadEnd = quadBase + 0x30;
        while (quad < quadEnd)
        {
            vertexX = quadVertex[-2];
            vertexY = quadVertex[-1];
            vertexZ = *quadVertex;
            quadVertex[-2] = vertexX * basisXX + vertexY * basisXY + vertexZ * basisXZ + selected->position_guess[0];
            quadVertex[-1] = vertexX * basisYX + vertexY * basisYY + vertexZ * basisYZ + selected->position_guess[1];
            *quadVertex = vertexX * basisZX + vertexY * basisZY + vertexZ * basisZZ + selected->position_guess[2];
            quad = quadVertex + 10;
            quadVertex = quadVertex + 12;
        }

        rotationIndex = renderFlags & 0xff00;
        if (rotationIndex == 0x500 || rotationIndex == 0x400 || rotationIndex == 0x200)
        {
            quadBase[4] = 0.0f;
            quadBase[5] = 0.0f;
            quadBase[0x10] = 0.0f;
            quadBase[0x11] = 1.0f;
            quadBase[0x1c] = 1.0f;
            quadBase[0x1d] = 1.0f;
            quadBase[0x28] = 1.0f;
            quadBase[0x29] = 0.0f;
        }

        destinationVertex = renderer->transformedVertices;
        sourceVertex = renderer->vertices;
        destinationEnd = destinationVertex + 0x30;
        for (; destinationVertex < destinationEnd; destinationVertex = destinationVertex + 0x0c)
        {
            sourceX = originX + *sourceVertex;
            sourceY = originY + sourceVertex[1];
            sourceZ = originZ + sourceVertex[2];
            sourceVertex = sourceVertex + 0x0c;
            *destinationVertex = sourceX * renderer->transformValues[0] + sourceY * renderer->transformValues[1] + sourceZ * renderer->transformValues[2];
            destinationVertex[1] = sourceX * renderer->transformValues[3] + sourceY * renderer->transformValues[4] + sourceZ * renderer->transformValues[5];
            destinationVertex[2] = sourceX * renderer->zFromX + sourceY * renderer->zFromY + sourceZ * renderer->zFromZ;
        }

        if (radiusSquared == 0.0f)
        {
            renderer->renderVector[0] = 0.0f;
            renderer->renderVector[1] = 0.0f;
            radiusRoot = 0.0f;
        }
        else
        {
            renderer->renderVector[0] = -(radiusRoot * combinedX);
            renderer->renderVector[1] = -(radiusRoot * combinedY);
            radiusRoot = -(radiusRoot * combinedZ);
        }
        renderer->renderVector[2] = radiusRoot;
        result = EQGFX_EFFECT_SUBMIT_GUESS(renderer, 0, 4, (unsigned char *)effect + 0x18, renderFlags);
        if (0 < result)
            renderer->renderCount = renderer->renderCount + 1;
    }

finish:
    selected->frameStamp_guess = EQGFX_EFFECT_CACHE_STAMP_GUESS;
    return result;
}

// FUNCTION: EQGFX 0x10035058
// Prepares an object's rotation and scale transforms for placement in the scene.
// @early-stop: generated __ftol calls, frame size, and x87 scheduling still differ.
/* fake match: retain the last-element pointers because direct indexing changed VC6's frame and register order. */
int __cdecl _t3dComputeObjectToWorldCoefficients_guess(RenderProjectionState_guess *renderState, float distanceScale, RenderRotationInput_guess *rotationInput)
{
    float firstCosine;
    float secondSine;
    float secondCosine;
    float thirdSine;
    float inverseDistanceScale;
    float *lastRotationElement;
    float *lastInverseScaleElement;
    unsigned int firstIndex;
    unsigned int secondIndex;
    unsigned int thirdIndex;

    renderState->flags = renderState->flags & 0xdf;
    renderState->projectionMarker = EQGFX_PROJECTION_MARKER;
    firstIndex = rotationInput->rotation[0];
    secondIndex = rotationInput->rotation[1];
    thirdIndex = rotationInput->rotation[2];
    firstIndex = firstIndex & EQGFX_ROTATION_INDEX_MASK;
    secondIndex = secondIndex & EQGFX_ROTATION_INDEX_MASK;
    thirdIndex = thirdIndex & EQGFX_ROTATION_INDEX_MASK;
    firstCosine = ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_COSINE_TABLE_OFFSET))[firstIndex];
    secondSine = -((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_SINE_TABLE_OFFSET))[secondIndex];
    secondCosine = ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_COSINE_TABLE_OFFSET))[secondIndex];
    thirdSine = ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_SINE_TABLE_OFFSET))[thirdIndex];
    renderState->rotationMatrix[0] = secondCosine * firstCosine;
    renderState->rotationMatrix[1] = ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_SINE_TABLE_OFFSET))[firstIndex] * secondCosine;
    renderState->rotationMatrix[2] = -secondSine;
    renderState->rotationMatrix[3] = thirdSine * secondSine * firstCosine - ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_COSINE_TABLE_OFFSET))[thirdIndex] * ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_SINE_TABLE_OFFSET))[firstIndex];
    renderState->rotationMatrix[4] = thirdSine * secondSine * ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_SINE_TABLE_OFFSET))[firstIndex] + ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_COSINE_TABLE_OFFSET))[thirdIndex] * firstCosine;
    renderState->rotationMatrix[5] = thirdSine * secondCosine;
    renderState->rotationMatrix[6] = thirdSine * ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_SINE_TABLE_OFFSET))[firstIndex] + ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_COSINE_TABLE_OFFSET))[thirdIndex] * secondSine * firstCosine;
    renderState->rotationMatrix[7] = ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_COSINE_TABLE_OFFSET))[thirdIndex] * secondSine * ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_SINE_TABLE_OFFSET))[firstIndex] - thirdSine * firstCosine;
    renderState->rotationMatrix[8] = ((float *)(t3d_angle_tree_guess + EQGFX_ROTATION_COSINE_TABLE_OFFSET))[thirdIndex] * secondCosine;
    inverseDistanceScale = 1.0 / distanceScale;
    renderState->inverseScaleRotationMatrix[0] = secondCosine * firstCosine * inverseDistanceScale;
    renderState->inverseScaleRotationMatrix[3] = inverseDistanceScale * renderState->rotationMatrix[1];
    renderState->inverseScaleRotationMatrix[6] = inverseDistanceScale * renderState->rotationMatrix[2];
    renderState->inverseScaleRotationMatrix[1] = inverseDistanceScale * renderState->rotationMatrix[3];
    renderState->inverseScaleRotationMatrix[4] = inverseDistanceScale * renderState->rotationMatrix[4];
    renderState->inverseScaleRotationMatrix[7] = inverseDistanceScale * renderState->rotationMatrix[5];
    renderState->inverseScaleRotationMatrix[2] = inverseDistanceScale * renderState->rotationMatrix[6];
    renderState->inverseScaleRotationMatrix[5] = inverseDistanceScale * renderState->rotationMatrix[7];
    lastRotationElement = &renderState->rotationMatrix[8];
    lastInverseScaleElement = &renderState->inverseScaleRotationMatrix[8];
    *lastInverseScaleElement = inverseDistanceScale * *lastRotationElement;
    renderState->scaledRotationMatrix[0] = distanceScale * renderState->rotationMatrix[0];
    renderState->scaledRotationMatrix[1] = distanceScale * renderState->rotationMatrix[1];
    renderState->scaledRotationMatrix[2] = distanceScale * renderState->rotationMatrix[2];
    renderState->scaledRotationMatrix[3] = distanceScale * renderState->rotationMatrix[3];
    renderState->scaledRotationMatrix[4] = distanceScale * renderState->rotationMatrix[4];
    renderState->scaledRotationMatrix[5] = distanceScale * renderState->rotationMatrix[5];
    renderState->scaledRotationMatrix[6] = distanceScale * renderState->rotationMatrix[6];
    renderState->scaledRotationMatrix[7] = distanceScale * renderState->rotationMatrix[7];
    renderState->scaledRotationMatrix[8] = distanceScale * renderState->rotationMatrix[8];
    return 1;
}

// FUNCTION: EQGFX 0x10035245
// Tests a sphere against camera bounds and records which view planes it intersects.
// @early-stop: radius scaling and shared return-block layout still differ.
/* fake match: negate the bounds tests to preserve unordered-float failures at the early exit. */
int __cdecl testSphereAgainstViewFrustum_guess(SphereCullingContext_guess *cullingContext, float sphereCenterX, float sphereCenterY, float sphereCenterZ, float sphereRadius)
{
    int result = 1;
    float transformedX;
    float transformedY;
    float transformedZ;
    float side;
    float scaledDepth;
    float edgeDistance;
    unsigned int intersectionFlags = 0;

    transformedX = sphereCenterX + cullingContext->originOffset[0];
    transformedY = sphereCenterY + cullingContext->originOffset[1];
    transformedZ = sphereCenterZ + cullingContext->originOffset[2];
    sphereCenterX = transformedZ * cullingContext->orientation[2][2] +
                    transformedY * cullingContext->orientation[1][2] +
                    transformedX * cullingContext->orientation[0][2];
    if (!(sphereCenterX + sphereRadius >= cullingContext->nearDepth) ||
        !(sphereCenterX - sphereRadius <= cullingContext->farDepth))
    {
        result = 0;
        return result;
    }
    sphereCenterZ = sphereRadius * sphereRadius * cullingContext->verticalRadiusScale;
    side = transformedZ * cullingContext->orientation[2][1] +
           transformedY * cullingContext->orientation[1][1] +
           transformedX * cullingContext->orientation[0][1];
    scaledDepth = sphereCenterX * cullingContext->verticalProjectionScale;
    edgeDistance = scaledDepth + side;
    if (edgeDistance * edgeDistance <= sphereCenterZ)
    {
        intersectionFlags = intersectionFlags | result;
    }
    else if (edgeDistance < EQGFX_ZERO_F_GUESS)
    {
        result = 0;
        return result;
    }
    edgeDistance = scaledDepth - side;
    if (!(sphereCenterZ < edgeDistance * edgeDistance))
    {
        intersectionFlags = intersectionFlags | 2;
    }
    else if (edgeDistance < EQGFX_ZERO_F_GUESS)
    {
        result = 0;
        return result;
    }
    side = transformedZ * cullingContext->orientation[2][0] +
           transformedY * cullingContext->orientation[1][0] +
           transformedX * cullingContext->orientation[0][0];
    scaledDepth = sphereCenterX * cullingContext->horizontalProjectionScale;
    sphereCenterZ = sphereRadius * sphereRadius * cullingContext->horizontalRadiusScale;
    edgeDistance = side + scaledDepth;
    if (!(sphereCenterZ < edgeDistance * edgeDistance))
    {
        intersectionFlags = intersectionFlags | 4;
    }
    else if (edgeDistance < EQGFX_ZERO_F_GUESS)
    {
        result = 0;
        return result;
    }
    edgeDistance = scaledDepth - side;
    if (!(sphereCenterZ < edgeDistance * edgeDistance))
    {
        intersectionFlags = intersectionFlags | 8;
    }
    else if (edgeDistance < EQGFX_ZERO_F_GUESS)
    {
        result = 0;
        return result;
    }
    if (sphereCenterX - sphereRadius <= cullingContext->nearDepth)
        intersectionFlags = intersectionFlags | 0x10;
    if (sphereCenterX + sphereRadius >= cullingContext->farDepth)
        intersectionFlags = intersectionFlags | 0x20;
    cullingContext->intersectionFlags = intersectionFlags;
    return result;
}

// FUNCTION: EQGFX 0x100353E4
// Sets whether dynamic level of detail is active and adjusts its distance range.
void __cdecl s3dSetDynamicLOD(unsigned int dynamicLodEnabled, unsigned int lodDistanceScale, unsigned int viewDistanceOffset)
{
    EQGFX_DYNAMIC_LOD_ENABLED_GUESS = dynamicLodEnabled;
    EQGFX_VIEW_DISTANCE_OFFSET_BITS_GUESS = viewDistanceOffset;
    EQGFX_LOD_DISTANCE_SCALE_GUESS = lodDistanceScale;
    return;
}

// FUNCTION: EQGFX 0x10035400
// Checks whether a sprite lies beyond the current scaled viewing distance.
int __cdecl isSpriteOutsideViewingDistance_guess(SpriteDistancePosition_guess *spritePosition, ViewingDistanceContext_guess *viewContext)
{
    float projectedDistance;

    if ((EQGFX_SPRITE_DISTANCE_ENABLED_GUESS == 1) &&
        (projectedDistance = (viewContext->originX + spritePosition->positionX) * viewContext->projectionX +
                            (viewContext->originY + spritePosition->positionY) * viewContext->projectionY +
                            (viewContext->originZ + spritePosition->positionZ) * viewContext->projectionZ,
         EQGFX_MINIMUM_SPRITE_DISTANCE_GUESS <= projectedDistance) &&
        (EQGFX_MINIMUM_NORMALIZED_SPRITE_DISTANCE_GUESS <
         (projectedDistance - EQGFX_VIEW_DISTANCE_OFFSET_GUESS) /
         (viewContext->distanceScale * spritePosition->distanceScaleB * spritePosition->distanceScaleA)))
    {
        return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10035479
// Selects whether scene rendering uses precalculated lighting.
void __cdecl s3dSetPrecalcLighting(unsigned int precalcLighting)
{
    EQGFX_PRECALC_LIGHTING_GUESS = precalcLighting;
    return;
}

// FUNCTION: EQGFX 0x10035483
// Renders a hierarchical object with triangle lighting and its animated pose.
int __cdecl renderHierarchicalObjectData14_guess(void *renderTarget, Renderer_guess *renderer, T3D_tagACTORINSTANCE *actor,
                         HierarchicalObject_guess *object, LightTransform_guess *transform)
{
    HierarchicalObjectData_guess *objectData;
    FrameSource_guess *frameSource;
    unsigned int savedFirstOutputIndex;
    int restoreFirstOutputIndex;
    int visibility;
    float viewPosition[3];
    float viewRotation[9];
    float relative[3];
    float reverse[3];
    float ambient;

    restoreFirstOutputIndex = 0;
    objectData = object->data;
    if (objectData->triangleLighting == 0)
        return 0;
    if (objectData->triangleLighting->entries == 0 ||
        *(unsigned int *)&objectData->triangleLighting->unknown_4c[4] == 0)
    {
        if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
            renderer->visibilityResult = renderer->previousVisibilityResult;
        return 1;
    }
    viewPosition[0] = object->z * transform->objectToViewTransform[6] +
                      object->y * transform->objectToViewTransform[3] +
                      object->x * transform->objectToViewTransform[0] +
                      transform->objectToViewTransform[9];
    viewPosition[1] = object->z * transform->objectToViewTransform[7] +
                      object->y * transform->objectToViewTransform[4] +
                      object->x * transform->objectToViewTransform[1] +
                      transform->objectToViewTransform[10];
    viewPosition[2] = object->z * transform->objectToViewTransform[8] +
                      object->y * transform->objectToViewTransform[5] +
                      object->x * transform->objectToViewTransform[2] +
                      transform->objectToViewTransform[11];
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        visibility = EQGFX_TEST_SPRITE_BOUNDS_GUESS(renderer->visibilityBoundsContext, viewPosition,
                                                    *(float *)&objectData->triangleLighting->unknown_24[0xc] *
                                                    actor->scaleFactor_guess * transform->objectScale);
        if (visibility == -1)
            return 1;
        renderer->visibilityResult = visibility;
    }
    relative[0] = transform->viewTransform[9] - object->x;
    viewRotation[0] = transform->viewTransform[0];
    viewRotation[1] = transform->viewTransform[1];
    viewRotation[2] = transform->viewTransform[2];
    relative[1] = transform->viewTransform[10] - object->y;
    viewRotation[3] = transform->viewTransform[3];
    viewRotation[4] = transform->viewTransform[4];
    viewRotation[5] = transform->viewTransform[5];
    relative[2] = transform->viewTransform[11] - object->z;
    viewRotation[6] = transform->viewTransform[6];
    viewRotation[7] = transform->viewTransform[7];
    viewRotation[8] = transform->viewTransform[8];
    reverse[0] = viewRotation[6] * -renderer->translation[2] + viewRotation[3] * -renderer->translation[1] +
                 viewRotation[0] * -renderer->translation[0] + relative[0];
    reverse[1] = viewRotation[7] * -renderer->translation[2] + viewRotation[4] * -renderer->translation[1] +
                 viewRotation[1] * -renderer->translation[0] + relative[1];
    reverse[2] = viewRotation[8] * -renderer->translation[2] + viewRotation[5] * -renderer->translation[1] +
                 viewRotation[2] * -renderer->translation[0] + relative[2];
    EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, objectData);
    EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
    EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, viewPosition, 0x30000000, 3);
    ambient = EQGFX_GET_ACTOR_SCALED_AMBIENT_GUESS(renderTarget, renderer, actor);
    EQGFX_UPDATE_DM_SPRITE_TIMING_GUESS(object, renderer->spriteTiming);
    frameSource = (FrameSource_guess *)object->renderMode;
    if (frameSource != 0)
    {
        restoreFirstOutputIndex = 1;
        savedFirstOutputIndex = objectData->triangleLighting->firstOutputIndex;
        objectData->triangleLighting->firstOutputIndex =
            (*(unsigned int **)frameSource->data->triangleLighting->unknown_08)[frameSource->frameIndex];
    }
    if (object->renderMode == 0 && *(unsigned int *)&objectData->triangleLighting->unknown_24[0x8] != 0)
        EQGFX_RENDER_OBJECT_PATH_A_GUESS(object, reverse, renderer);
    else
        EQGFX_RENDER_OBJECT_PATH_B_GUESS(object, reverse, renderer);
    EQGFX_APPLY_OBJECT_AMBIENT_GUESS(object, renderer, renderTarget, ambient);
    if (object->data->triangleLighting->renderDataValue != 0)
        EQGFX_APPLY_OBJECT_LIGHTING_GUESS(object, renderer, actor->scaleFactor_guess, viewRotation,
                                          transform->rotation, ambient);
    EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS(renderer->callbackInput, objectData->triangleLighting->triangleCount,
                                             renderer->vertexVisible);
    EQGFX_BUILD_OBJECT_TRIANGLES_GUESS(object, transform, viewPosition, renderer);
    EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
    EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(objectData->triangleLighting->effectBufferIndex);
    if (renderer->visibilityResult == 0)
    {
        if (g_render_mode_guess == 2)
        {
            EQGFX_RENDER_MODE_TWO_PREPARE_GUESS(object, renderer);
            EQGFX_RENDER_MODE_TWO_SCENE_FINISH_GUESS(renderTarget, renderer, object);
        }
        else if (g_render_mode_guess == 1)
        {
            EQGFX_RENDER_MODE_ONE_PREPARE_GUESS(object, renderer);
            EQGFX_RENDER_MODE_ONE_SCENE_FINISH_GUESS(renderTarget, renderer, object);
        }
    }
    else
    {
        EQGFX_RENDER_FLAGGED_OBJECT_GUESS(renderTarget, renderer, object);
    }
    if (restoreFirstOutputIndex == 1)
        objectData->triangleLighting->firstOutputIndex = savedFirstOutputIndex;
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
        renderer->visibilityResult = renderer->previousVisibilityResult;
    return 1;
}

// FUNCTION: EQGFX 0x100357ef
// Renders the vertex-transformed part of an animated model with ambient light and effects.
int __cdecl renderHierarchicalObjectData18_guess(void *renderTarget, Renderer_guess *renderer, T3D_tagACTORINSTANCE *actor,
                         HierarchicalObject_guess *object, LightTransform_guess *transform)
{
    RenderWork_guess work;
    LightTransform_guess *originalTransform;
    float objectX;
    float objectY;
    float objectZ;
    float reverseX;
    float reverseY;
    float reverseZ;
    float ambient;

    work.objectData = object->data;
    if (work.objectData->vertexTransform == 0 ||
        work.objectData->vertexTransform->vertexCount == 0)
    {
        return 0;
    }
    originalTransform = transform;
    objectX = object->x;
    objectY = object->y;
    objectZ = object->z;
    work.projectedPosition[0] = objectX * transform->objectToViewTransform[0] +
                           objectY * transform->objectToViewTransform[3] +
                           objectZ * transform->objectToViewTransform[6] +
                           transform->objectToViewTransform[9];
    work.projectedPosition[1] = objectX * transform->objectToViewTransform[1] +
                           objectY * transform->objectToViewTransform[4] +
                           objectZ * transform->objectToViewTransform[7] +
                           transform->objectToViewTransform[10];
    work.projectedPosition[2] = objectX * transform->objectToViewTransform[2] +
                           objectY * transform->objectToViewTransform[5] +
                           objectZ * transform->objectToViewTransform[8] +
                           transform->objectToViewTransform[11];
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        transform = (LightTransform_guess *)EQGFX_TEST_SPRITE_BOUNDS_GUESS(
            renderer->visibilityBoundsContext, work.projectedPosition,
            work.objectData->vertexTransform->lightingScale * actor->scaleFactor_guess *
                transform->objectScale);
        if ((int)transform == -1)
        {
            return 1;
        }
        renderer->visibilityResult = (int)transform;
    }
    work.relativePosition[0] = originalTransform->viewTransform[9] - object->x;
    work.objectMatrix[0] = originalTransform->viewTransform[0];
    work.objectMatrix[1] = originalTransform->viewTransform[1];
    work.objectMatrix[2] = originalTransform->viewTransform[2];
    work.relativePosition[1] = originalTransform->viewTransform[10] - object->y;
    work.objectMatrix[3] = originalTransform->viewTransform[3];
    work.objectMatrix[4] = originalTransform->viewTransform[4];
    work.objectMatrix[5] = originalTransform->viewTransform[5];
    work.relativePosition[2] = originalTransform->viewTransform[11] - object->z;
    work.objectMatrix[6] = originalTransform->viewTransform[6];
    work.objectMatrix[7] = originalTransform->viewTransform[7];
    work.objectMatrix[8] = originalTransform->viewTransform[8];
    reverseX = -renderer->translation[0];
    reverseY = -renderer->translation[1];
    reverseZ = -renderer->translation[2];
    work.renderPosition[0] = work.objectMatrix[0] * reverseX +
                             work.objectMatrix[3] * reverseY +
                             work.objectMatrix[6] * reverseZ + work.relativePosition[0];
    work.renderPosition[1] = work.objectMatrix[1] * reverseX +
                             work.objectMatrix[4] * reverseY +
                             work.objectMatrix[7] * reverseZ + work.relativePosition[1];
    work.renderPosition[2] = work.objectMatrix[2] * reverseX +
                             work.objectMatrix[5] * reverseY +
                             work.objectMatrix[8] * reverseZ + work.relativePosition[2];
    EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer,
                                             work.objectData);
    EQGFX_UPDATE_DM_SPRITE_TIMING_GUESS(object, renderer->spriteTiming);
    if (object->renderMode == 0)
    {
        if (work.objectData->vertexTransform->vertexProcessingMode == 0)
        {
            EQGFX_RENDER_OBJECT_MODE_B_GUESS(object, work.renderPosition, renderer);
        }
        else
        {
            EQGFX_RENDER_OBJECT_MODE_A_GUESS(object, work.renderPosition, renderer);
        }
    }
    else
    {
        EQGFX_RENDER_OBJECT_MODE_C_GUESS(object, work.renderPosition, renderer);
    }
    EQGFX_UPDATE_OBJECT_RENDER_DATA_GUESS(object, originalTransform,
                                         work.projectedPosition, renderer);
    if (transform != 0 && EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        transform = (LightTransform_guess *)EQ_CDECL(
            int, 0x1003CDF0, (Renderer_guess *, HierarchicalObjectData_guess *))(
                renderer, work.objectData);
        if ((int)transform == -1)
        {
            goto restore_visibility;
        }
        renderer->visibilityResult = (int)transform;
    }
    EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
    EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, work.projectedPosition, 0x30000000, 3);
    ambient = EQ_CDECL(float, 0x10037F34, (void *, void *, T3D_tagACTORINSTANCE *))(
        renderTarget, renderer, actor);
    if (work.objectData->vertexTransform->animationSampleCount != 0)
    {
        EQGFX_APPLY_ACTOR_AMBIENT_GUESS(object, renderer, renderTarget,
                                        actor->scaleFactor_guess * originalTransform->objectScale,
                                        work.objectMatrix, originalTransform->rotation, ambient);
    }
    EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS(renderer->callbackInput,
                                             work.objectData->vertexTransform->vertexCount,
                                             renderer->vertexVisible);
    EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(work.objectData->vertexTransform->effectBufferIndex);
    EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
    if (renderer->visibilityResult == 0)
    {
        if (g_render_mode_guess == 2)
        {
            EQGFX_RENDER_MODE_TWO_GUESS(work.objectData, object->renderObject, renderer);
            EQGFX_RENDER_MODE_TWO_SCENE_GUESS(renderTarget, renderer,
                                               work.objectData, object->renderObject);
        }
        else if (g_render_mode_guess == 1)
        {
            EQGFX_RENDER_MODE_ONE_GUESS(work.objectData, object->renderObject, renderer);
            EQGFX_RENDER_MODE_ONE_SCENE_GUESS(renderTarget, renderer,
                                               work.objectData, object->renderObject);
        }
    }
    else if (EQGFX_RENDER_VISIBILITY_MODE_GUESS == 0)
    {
        EQGFX_RENDER_STANDARD_SCENE_GUESS(renderTarget, renderer, work.objectData,
                                         object->renderObject);
    }
    else
    {
        EQ_CDECL(void, 0x1003C119,
                 (void *, Renderer_guess *, HierarchicalObjectData_guess *, void *))(
            renderTarget, renderer, work.objectData, object->renderObject);
    }
restore_visibility:
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        renderer->visibilityResult = renderer->previousVisibilityResult;
    }
    return 1;
}

#define EQGFX_RENDER_FRAME_STAMP_GUESS EQ_GLOBAL(unsigned int, 0x100FBBB0)
#define EQGFX_UPDATE_ACTOR_PROJECTION_GUESS EQ_CDECL(int, 0x10035058, (T3D_tagACTORINSTANCE *, float, float *))
#define EQGFX_TRANSFORM_FRUSTUM_GUESS EQ_CDECL(void, 0x10037093, (Frustum_guess *, float *, float *, Frustum_guess *))
#define EQGFX_PROJECT_OBJECT_POSITION_GUESS EQ_CDECL(void, 0x1003AF14, (HierarchicalObject_guess *, T3D_tagACTORINSTANCE *, float *, Renderer_guess *))
#define EQGFX_SET_DM_SPRITE_RGB_TRACK_GUESS EQ_CDECL(void, 0x100106A5, (void *, HierarchicalObject_guess *, unsigned int))

// FUNCTION: EQGFX 0x10035b4e
// Draws an actor's DM sprite (data +0x14) once per frame: cull, RGB track, frame swap, ground ambient, lights.
int __cdecl _t3dPaintDMSprite_guess(void *renderTarget, Renderer_guess *renderer, int *actorIndex)
{
    ActorRenderWork_guess work;
    T3D_tagACTORINSTANCE *actor;
    HierarchicalObject_guess *object;
    HierarchicalObjectData_guess *objectData;
    TriangleLightingData_guess *triangleLighting;
    FrameSource_guess *frameSource;
    float objectX;
    float objectY;
    float objectZ;
    float actorReverseX;
    float actorReverseY;
    float actorReverseZ;
    float reverseX;
    float reverseY;
    float reverseZ;
    float ambient;
    int visibility;
    unsigned int physicsFlags;

    work.rgbTrackSet = 0;
    work.restoreFirstOutputIndex = 0;
    object = renderer->object;
    objectData = object->data;
    triangleLighting = objectData->triangleLighting;
    if (triangleLighting == 0)
    {
        return 0;
    }
    actor = ((RenderTarget_guess *)renderTarget)->actors[*actorIndex];
    if (actor->frameStamp_guess != EQGFX_RENDER_FRAME_STAMP_GUESS)
    {
        actor->frameStamp_guess = EQGFX_RENDER_FRAME_STAMP_GUESS;
        if (triangleLighting->entries == 0 ||
            *(unsigned int *)&triangleLighting->unknown_4c[4] == 0)
        {
            renderer->visibilityResult = renderer->previousVisibilityResult;
        }
        else
        {
            visibility = EQGFX_TEST_SPRITE_BOUNDS_GUESS(
                renderer->visibilityBoundsContext, actor->position_guess,
                actor->boundingRadius_guess * actor->scaleFactor_guess);
            if (visibility == -1)
            {
                return 1;
            }
            renderer->visibilityResult = visibility;
            if ((actor->flags_guess[3] & 0x20) != 0)
            {
                EQGFX_UPDATE_ACTOR_PROJECTION_GUESS(actor, actor->scaleFactor_guess,
                                                    actor->position_guess);
            }
            objectX = object->x;
            objectY = object->y;
            objectZ = object->z;
            work.viewPosition[0] = objectX * actor->scaledRotationMatrix_guess[0] +
                                   objectY * actor->scaledRotationMatrix_guess[3] +
                                   objectZ * actor->scaledRotationMatrix_guess[6] + actor->position_guess[0];
            work.viewPosition[1] = objectX * actor->scaledRotationMatrix_guess[1] +
                                   objectY * actor->scaledRotationMatrix_guess[4] +
                                   objectZ * actor->scaledRotationMatrix_guess[7] + actor->position_guess[1];
            work.viewPosition[2] = objectX * actor->scaledRotationMatrix_guess[2] +
                                   objectY * actor->scaledRotationMatrix_guess[5] +
                                   objectZ * actor->scaledRotationMatrix_guess[8] + actor->position_guess[2];
            work.inverseScaleMatrix[0] = actor->inverseScaleRotationMatrix_guess[0];
            work.inverseScaleMatrix[1] = actor->inverseScaleRotationMatrix_guess[1];
            work.inverseScaleMatrix[2] = actor->inverseScaleRotationMatrix_guess[2];
            work.inverseScaleMatrix[3] = actor->inverseScaleRotationMatrix_guess[3];
            work.inverseScaleMatrix[4] = actor->inverseScaleRotationMatrix_guess[4];
            work.inverseScaleMatrix[5] = actor->inverseScaleRotationMatrix_guess[5];
            work.inverseScaleMatrix[6] = actor->inverseScaleRotationMatrix_guess[6];
            work.inverseScaleMatrix[7] = actor->inverseScaleRotationMatrix_guess[7];
            work.inverseScaleMatrix[8] = actor->inverseScaleRotationMatrix_guess[8];
            actorReverseX = -actor->position_guess[0];
            actorReverseY = -actor->position_guess[1];
            actorReverseZ = -actor->position_guess[2];
            work.relativePosition[0] = work.inverseScaleMatrix[0] * actorReverseX +
                                       work.inverseScaleMatrix[3] * actorReverseY +
                                       work.inverseScaleMatrix[6] * actorReverseZ - object->x;
            work.relativePosition[1] = work.inverseScaleMatrix[1] * actorReverseX +
                                       work.inverseScaleMatrix[4] * actorReverseY +
                                       work.inverseScaleMatrix[7] * actorReverseZ - object->y;
            work.relativePosition[2] = work.inverseScaleMatrix[2] * actorReverseX +
                                       work.inverseScaleMatrix[5] * actorReverseY +
                                       work.inverseScaleMatrix[8] * actorReverseZ - object->z;
            if (visibility != 0)
            {
                EQGFX_TRANSFORM_FRUSTUM_GUESS(
                    (Frustum_guess *)renderer->visibilityBoundsContext,
                    work.inverseScaleMatrix, actor->rotationMatrix_guess,
                    (Frustum_guess *)work.transformedFrustum);
                visibility = EQGFX_CALCULATE_OBJECT_BOUNDS_GUESS(
                    work.transformedFrustum, triangleLighting->boundsPoint0,
                    triangleLighting->boundsPoint1);
                if (visibility == -1)
                {
                    return 1;
                }
                renderer->visibilityResult = visibility;
            }
            if (actor->dmRgbTrack_bits_guess != 0)
            {
                EQGFX_SET_DM_SPRITE_RGB_TRACK_GUESS(renderTarget, object,
                                                    actor->dmRgbTrack_bits_guess);
                work.rgbTrackSet = 1;
            }
            EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, objectData);
            EQGFX_UPDATE_DM_SPRITE_TIMING_GUESS(object, renderer->spriteTiming);
            frameSource = (FrameSource_guess *)object->renderMode;
            if (frameSource != 0)
            {
                work.restoreFirstOutputIndex = 1;
                work.savedFirstOutputIndex = triangleLighting->firstOutputIndex;
                triangleLighting->firstOutputIndex =
                    (*(unsigned int **)frameSource->data->triangleLighting->unknown_08)[frameSource->frameIndex];
            }
            EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
            if (actor->dmRgbTrack_bits_guess == 0)
            {
                physicsFlags = 0x30000000;
            }
            else
            {
                physicsFlags = 0x20000000;
            }
            EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, work.viewPosition,
                                              physicsFlags, 3);
            ambient = *(float *)&visibility;
            if (((*(unsigned int *)triangleLighting->unknown_18 == 0) ||
                 ((objectData->flags & 0x20) == 0)) &&
                ((object->sourceObject == 0) ||
                 ((object->sourceObject->data->flags & 0x80) == 0)))
            {
                ambient = EQGFX_GET_ACTOR_SCALED_AMBIENT_GUESS(renderTarget,
                                                               renderer, actor);
            }
            reverseX = -renderer->translation[0];
            reverseY = -renderer->translation[1];
            reverseZ = -renderer->translation[2];
            work.adjustedPosition[0] = work.inverseScaleMatrix[0] * reverseX +
                                       work.inverseScaleMatrix[3] * reverseY +
                                       work.inverseScaleMatrix[6] * reverseZ +
                                       work.relativePosition[0];
            work.adjustedPosition[1] = work.inverseScaleMatrix[1] * reverseX +
                                       work.inverseScaleMatrix[4] * reverseY +
                                       work.inverseScaleMatrix[7] * reverseZ +
                                       work.relativePosition[1];
            work.adjustedPosition[2] = work.inverseScaleMatrix[2] * reverseX +
                                       work.inverseScaleMatrix[5] * reverseY +
                                       work.inverseScaleMatrix[8] * reverseZ +
                                       work.relativePosition[2];
            if (object->renderMode == 0 &&
                *(unsigned int *)&triangleLighting->unknown_24[0x08] != 0)
            {
                EQGFX_RENDER_OBJECT_PATH_A_GUESS(object, work.adjustedPosition,
                                                 renderer);
            }
            else
            {
                EQGFX_RENDER_OBJECT_PATH_B_GUESS(object, work.adjustedPosition,
                                                 renderer);
            }
            EQGFX_APPLY_OBJECT_AMBIENT_GUESS(object, renderer, renderTarget, ambient);
            if (triangleLighting->renderDataValue != 0)
            {
                EQGFX_APPLY_OBJECT_LIGHTING_GUESS(object, renderer,
                                                  actor->scaleFactor_guess,
                                                  work.inverseScaleMatrix,
                                                  actor->rotationMatrix_guess, ambient);
            }
            EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS(
                renderer->callbackInput, triangleLighting->triangleCount,
                renderer->vertexVisible);
            EQ_CDECL(void, 0x1003ACE0,
                     (HierarchicalObject_guess *, T3D_tagACTORINSTANCE *, float *, Renderer_guess *))(
                object, actor, work.viewPosition, renderer);
            EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
            EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(triangleLighting->effectBufferIndex);
            if (renderer->visibilityResult == 0)
            {
                if (g_render_mode_guess == 2)
                {
                    EQGFX_RENDER_MODE_TWO_PREPARE_GUESS(object, renderer);
                    EQGFX_RENDER_MODE_TWO_SCENE_FINISH_GUESS(renderTarget,
                                                             renderer, object);
                }
                else if (g_render_mode_guess == 1)
                {
                    EQGFX_RENDER_MODE_ONE_PREPARE_GUESS(object, renderer);
                    EQGFX_RENDER_MODE_ONE_SCENE_FINISH_GUESS(renderTarget,
                                                             renderer, object);
                }
            }
            else
            {
                EQGFX_RENDER_FLAGGED_OBJECT_GUESS(renderTarget, renderer, object);
            }
            if (work.rgbTrackSet == 1)
            {
                EQGFX_SET_DM_SPRITE_RGB_TRACK_GUESS(renderTarget, object, 0);
            }
            if (work.restoreFirstOutputIndex == 1)
            {
                triangleLighting->firstOutputIndex = work.savedFirstOutputIndex;
            }
            renderer->visibilityResult = renderer->previousVisibilityResult;
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x10035fe2
// Draws an actor's DM sprite (data +0x18) once per frame: cull, vertex clip test, RGB track, ground ambient.
// __cdecl (the callee pops nothing; ECX is not an input), returns 1, or 0 when the object has no vertex transform.
// When the ambient is not sampled, the seventh argument of 0x1003A79C is the visibility word read back as a float.
int __cdecl DrawActorDMSprite_guess(void *renderTarget, Renderer_guess *renderer, int *actorIndex)
{
    RenderWork_guess work;
    Frustum_guess frustum;
    T3D_tagACTORINSTANCE *actor;
    HierarchicalObject_guess *object;
    float objectX;
    float objectY;
    float objectZ;
    float actorReverseX;
    float actorReverseY;
    float actorReverseZ;
    float reverseX;
    float reverseY;
    float reverseZ;
    int visibility;
    int visibilityPassResult;
    float ambient;
    int rgbTrackSet;

    rgbTrackSet = 0;
    actor = ((RenderTarget_guess *)renderTarget)->actors[*actorIndex];
    if (actor->frameStamp_guess == EQGFX_RENDER_FRAME_STAMP_GUESS)
    {
        return 1;
    }
    actor->frameStamp_guess = EQGFX_RENDER_FRAME_STAMP_GUESS;
    object = renderer->object;
    work.objectData = object->data;
    if (work.objectData->vertexTransform == 0)
    {
        return 0;
    }
    visibility = EQGFX_TEST_SPRITE_BOUNDS_GUESS(renderer->visibilityBoundsContext, actor->position_guess,
                                                actor->boundingRadius_guess * actor->scaleFactor_guess);
    if (visibility == -1)
    {
        return 1;
    }
    renderer->visibilityResult = visibility;
    if ((actor->flags_guess[3] & 0x20) != 0)
    {
        EQGFX_UPDATE_ACTOR_PROJECTION_GUESS(actor, actor->scaleFactor_guess, actor->position_guess);
    }
    objectX = object->x;
    objectY = object->y;
    objectZ = object->z;
    work.projectedPosition[0] = objectZ * actor->scaledRotationMatrix_guess[6] + objectY * actor->scaledRotationMatrix_guess[3] +
                                objectX * actor->scaledRotationMatrix_guess[0] + actor->position_guess[0];
    work.projectedPosition[1] = objectZ * actor->scaledRotationMatrix_guess[7] + objectY * actor->scaledRotationMatrix_guess[4] +
                                objectX * actor->scaledRotationMatrix_guess[1] + actor->position_guess[1];
    work.projectedPosition[2] = objectZ * actor->scaledRotationMatrix_guess[8] + objectY * actor->scaledRotationMatrix_guess[5] +
                                objectX * actor->scaledRotationMatrix_guess[2] + actor->position_guess[2];
    work.objectMatrix[0] = actor->inverseScaleRotationMatrix_guess[0];
    work.objectMatrix[1] = actor->inverseScaleRotationMatrix_guess[1];
    work.objectMatrix[2] = actor->inverseScaleRotationMatrix_guess[2];
    work.objectMatrix[3] = actor->inverseScaleRotationMatrix_guess[3];
    work.objectMatrix[4] = actor->inverseScaleRotationMatrix_guess[4];
    work.objectMatrix[5] = actor->inverseScaleRotationMatrix_guess[5];
    work.objectMatrix[6] = actor->inverseScaleRotationMatrix_guess[6];
    work.objectMatrix[7] = actor->inverseScaleRotationMatrix_guess[7];
    work.objectMatrix[8] = actor->inverseScaleRotationMatrix_guess[8];
    actorReverseX = -actor->position_guess[0];
    actorReverseY = -actor->position_guess[1];
    actorReverseZ = -actor->position_guess[2];
    work.relativePosition[0] = work.objectMatrix[6] * actorReverseZ + work.objectMatrix[3] * actorReverseY +
                               work.objectMatrix[0] * actorReverseX - object->x;
    work.relativePosition[1] = work.objectMatrix[7] * actorReverseZ + work.objectMatrix[4] * actorReverseY +
                               work.objectMatrix[1] * actorReverseX - object->y;
    work.relativePosition[2] = work.objectMatrix[8] * actorReverseZ + work.objectMatrix[5] * actorReverseY +
                               work.objectMatrix[2] * actorReverseX - object->z;
    if (visibility != 0)
    {
        EQGFX_TRANSFORM_FRUSTUM_GUESS((Frustum_guess *)renderer->visibilityBoundsContext, work.objectMatrix,
                                      actor->rotationMatrix_guess, &frustum);
        visibility = EQGFX_CALCULATE_OBJECT_BOUNDS_GUESS(&frustum, work.objectData->vertexTransform->boundsOffset0,
                                                         work.objectData->vertexTransform->boundsOffset1);
        if (visibility == -1)
        {
            return 1;
        }
        renderer->visibilityResult = visibility;
    }
    if (actor->dmRgbTrack_bits_guess != 0)
    {
        EQGFX_SET_DM_SPRITE_RGB_TRACK_GUESS(renderTarget, object, actor->dmRgbTrack_bits_guess);
        rgbTrackSet = 1;
    }
    EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, work.objectData);
    EQGFX_UPDATE_DM_SPRITE_TIMING_GUESS(object, renderer->spriteTiming);
    reverseX = -renderer->translation[0];
    reverseY = -renderer->translation[1];
    reverseZ = -renderer->translation[2];
    work.renderPosition[0] = work.objectMatrix[6] * reverseZ + work.objectMatrix[3] * reverseY +
                             work.objectMatrix[0] * reverseX + work.relativePosition[0];
    work.renderPosition[1] = work.objectMatrix[7] * reverseZ + work.objectMatrix[4] * reverseY +
                             work.objectMatrix[1] * reverseX + work.relativePosition[1];
    work.renderPosition[2] = work.objectMatrix[8] * reverseZ + work.objectMatrix[5] * reverseY +
                             work.objectMatrix[2] * reverseX + work.relativePosition[2];
    if (object->renderMode == 0)
    {
        if (work.objectData->vertexTransform->vertexProcessingMode == 0)
        {
            EQGFX_RENDER_OBJECT_MODE_B_GUESS(object, work.renderPosition, renderer);
        }
        else
        {
            EQGFX_RENDER_OBJECT_MODE_A_GUESS(object, work.renderPosition, renderer);
        }
    }
    else
    {
        EQGFX_RENDER_OBJECT_MODE_C_GUESS(object, work.renderPosition, renderer);
    }
    EQGFX_PROJECT_OBJECT_POSITION_GUESS(object, actor, work.projectedPosition, renderer);
    if (visibility != 0)
    {
        visibilityPassResult = EQ_CDECL(int, 0x1003CDF0, (Renderer_guess *, HierarchicalObjectData_guess *))(
            renderer, work.objectData);
        if (visibilityPassResult == -1)
        {
            goto finish_render;
        }
        renderer->visibilityResult = visibilityPassResult;
    }
    EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
    EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, work.projectedPosition,
                                      (actor->dmRgbTrack_bits_guess == 0) ? 0x30000000 : 0x20000000, 3);
    ambient = *(float *)&visibility;
    if ((work.objectData->vertexTransform->lightingState == 0 || (work.objectData->flags & 0x20) == 0) &&
        (object->sourceObject == 0 || (object->sourceObject->data->flags & 0x80) == 0))
    {
        ambient = EQGFX_GET_ACTOR_SCALED_AMBIENT_GUESS(renderTarget, renderer, actor);
    }
    if (work.objectData->vertexTransform->animationSampleCount != 0)
    {
        EQGFX_APPLY_ACTOR_AMBIENT_GUESS(object, renderer, renderTarget, actor->scaleFactor_guess,
                                        work.objectMatrix, actor->rotationMatrix_guess, ambient);
    }
    EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS(renderer->callbackInput,
                                             work.objectData->vertexTransform->vertexCount, renderer->vertexVisible);
    EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(work.objectData->vertexTransform->effectBufferIndex);
    EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
    if (renderer->visibilityResult == 0)
    {
        if (g_render_mode_guess == 2)
        {
            EQGFX_RENDER_MODE_TWO_GUESS(work.objectData, object->renderObject, renderer);
            EQGFX_RENDER_MODE_TWO_SCENE_GUESS(renderTarget, renderer, work.objectData, object->renderObject);
        }
        else if (g_render_mode_guess == 1)
        {
            EQGFX_RENDER_MODE_ONE_GUESS(work.objectData, object->renderObject, renderer);
            EQGFX_RENDER_MODE_ONE_SCENE_GUESS(renderTarget, renderer, work.objectData, object->renderObject);
        }
    }
    else
    {
        EQ_CDECL(void, 0x1003C119, (void *, Renderer_guess *, HierarchicalObjectData_guess *, void *))(
            renderTarget, renderer, work.objectData, object->renderObject);
    }
finish_render:
    if (rgbTrackSet == 1)
    {
        EQGFX_SET_DM_SPRITE_RGB_TRACK_GUESS(renderTarget, object, 0);
    }
    renderer->visibilityResult = renderer->previousVisibilityResult;
    return 1;
}

// FUNCTION: EQGFX 0x1003644a
// Marks camera-facing triangles in the mesh's element geometry for drawing.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image). __cdecl (the callee pops
// nothing; ECX is not an input), returns 1: clears the visible-byte arrays, then marks the elements in front.
int __cdecl markFrontFacingMeshElements_guess(Client_guess *client, float *viewPosition, Renderer_guess *renderer)
{
    Mesh_guess *mesh;
    unsigned char *vertexVisible;
    unsigned char *elementVisible;
    Element16_guess *element;
    Plane_guess *plane;
    Element16_guess *elementEnd;
    unsigned short index0;
    unsigned short index1;
    unsigned short index2;

    mesh = client->mesh;
    vertexVisible = renderer->vertexVisible;
    elementVisible = renderer->elementVisible;
    element = mesh->elementGeometry->elements;
    plane = mesh->elementGeometry->planes;
    elementEnd = element + mesh->elementGeometry->elementCount;
    EQGFX_VISIBILITY_MEMSET_GUESS(vertexVisible, 0, mesh->elementGeometry->vertexCount);
    EQGFX_VISIBILITY_MEMSET_GUESS(elementVisible, 0, mesh->elementGeometry->elementCount);
    for (; element < elementEnd; element++)
    {
        if ((element->flags & 1) == 0)
        {
            index0 = element->index0;
            index1 = element->index1;
            index2 = element->index2;
            if (EQGFX_ZERO_F_GUESS < plane->normal[2] * viewPosition[2] + plane->normal[1] * viewPosition[1] +
                                       plane->normal[0] * viewPosition[0] + plane->distance)
            {
                *elementVisible = 1;
                vertexVisible[index0] = 1;
                vertexVisible[index1] = 1;
                vertexVisible[index2] = 1;
            }
        }
        elementVisible = elementVisible + 1;
        plane = plane + 1;
    }
    return 1;
}

// FUNCTION: EQGFX 0x10036511
// Marks camera-facing triangles in the mesh's main geometry for drawing.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image). __cdecl (the callee pops
// nothing; ECX is not an input), returns 1: clears the visible-byte arrays, then marks the elements in front.
int __cdecl t3dMarkFrontFacingMeshElements_guess(Client_guess *client, float *viewPosition, Renderer_guess *renderer)
{
    Mesh_guess *mesh;
    unsigned char *vertexVisible;
    unsigned char *elementVisible;
    Element_guess *element;
    Plane_guess *plane;
    Element_guess *elementEnd;
    unsigned short index2;
    unsigned short index0;
    unsigned short index1;

    mesh = client->mesh;
    vertexVisible = renderer->vertexVisible;
    elementVisible = renderer->elementVisible;
    element = mesh->geometry->elements;
    plane = mesh->geometry->planes;
    elementEnd = element + mesh->geometry->elementCount;
    EQGFX_VISIBILITY_MEMSET_GUESS(vertexVisible, 0, mesh->geometry->vertexCount);
    EQGFX_VISIBILITY_MEMSET_GUESS(elementVisible, 0, mesh->geometry->elementCount);
    for (; element < elementEnd; element++)
    {
        if ((element->flags & 1) == 0)
        {
            index2 = element->index2;
            index0 = element->index0;
            index1 = element->index1;
            if (EQGFX_ZERO_F_GUESS < plane->normal[2] * viewPosition[2] + plane->normal[1] * viewPosition[1] +
                                       plane->normal[0] * viewPosition[0] + plane->distance)
            {
                *elementVisible = 1;
                vertexVisible[index0] = 1;
                vertexVisible[index1] = 1;
                vertexVisible[index2] = 1;
            }
        }
        elementVisible = elementVisible + 1;
        plane = plane + 1;
    }
    return 1;
}

// FUNCTION: EQGFX 0x100365d3
// Classifies batched mesh faces as front- or back-facing to guide visibility and drawing.
int __cdecl t3dClassifyBatchedTriangleFacing_guess(TriangleCullClient_guess *client, float *viewPosition,
                         TriangleCullRenderer_guess *renderer)
{
    TriangleCullOwner_guess *owner;
    TriangleCullScene_guess *scene;
    TriangleCullGeometry_guess *geometry;
    TriangleCullBatch_guess *batch;
    TriangleCullBatch_guess *batchEnd;
    TriangleCullTriangle_guess *triangle;
    unsigned char *vertexVisible;
    unsigned char *elementVisible;
    unsigned int triangleCount;
    unsigned short index0;
    unsigned short index1;
    unsigned short index2;
    float *vertex0;
    float *vertex1;
    float *vertex2;
    float edge1X;
    float edge1Y;
    float edge2X;
    float edge2Y;
    float edge2Z;
    float normalX;
    float normalY;
    float normalZ;

    owner = client->owner;
    scene = client->scene;
    geometry = owner->geometry;
    batch = geometry->batches;
    vertexVisible = renderer->vertexVisible;
    elementVisible = renderer->elementVisible;
    batchEnd = batch + geometry->batchCount;
    EQGFX_VISIBILITY_MEMSET_GUESS(vertexVisible, 0, geometry->vertexFlagBytes);
    EQGFX_VISIBILITY_MEMSET_GUESS(elementVisible, 0, geometry->elementFlagBytes);
    for (; batch < batchEnd; batch++)
    {
        triangleCount = batch->triangleCount;
        triangle = geometry->triangles + batch->firstElement;
        elementVisible = renderer->elementVisible + batch->firstElement;
        if (scene->matrixObjects[batch->matrixIndex]->orientationMode == 0)
        {
            if (triangleCount != 0)
            {
                do
                {
                    if ((triangle->flags & 1) == 0)
                    {
                        index0 = triangle->index0;
                        index1 = triangle->index1;
                        index2 = triangle->index2;
                        edge1X = geometry->vertexPositions[index1 * 3] -
                                 geometry->vertexPositions[index0 * 3];
                        vertex1 = geometry->vertexPositions + index1 * 3;
                        vertex0 = geometry->vertexPositions + index0 * 3;
                        edge1Y = geometry->vertexPositions[index1 * 3 + 1] - vertex0[1];
                        edge2X = geometry->vertexPositions[index2 * 3] - *vertex1;
                        vertex2 = geometry->vertexPositions + index2 * 3;
                        edge2Y = vertex2[1] - vertex1[1];
                        edge2Z = vertex2[2] - vertex1[2];
                        normalX = edge2Y * (vertex1[2] - vertex0[2]) - edge2Z * edge1Y;
                        normalY = -(edge2X * (vertex1[2] - vertex0[2]) - edge2Z * edge1X);
                        normalZ = edge2X * edge1Y - edge2Y * edge1X;
                        if (EQGFX_ZERO_F_GUESS <
                            normalX * viewPosition[0] +
                            normalY * viewPosition[1] +
                            normalZ * viewPosition[2] +
                            -(normalZ * vertex0[2] + normalY * vertex0[1] + normalX * *vertex0))
                        {
                            *elementVisible = 1;
                            vertexVisible[index0] = 1;
                            vertexVisible[index1] = 1;
                            vertexVisible[index2] = 1;
                        }
                    }
                    elementVisible++;
                    triangle++;
                } while (--triangleCount != 0);
            }
        }
        else if (triangleCount != 0)
        {
            do
            {
                if ((triangle->flags & 1) == 0)
                {
                    index1 = triangle->index1;
                    index0 = triangle->index0;
                    index2 = triangle->index2;
                    edge1X = geometry->vertexPositions[index1 * 3] -
                             geometry->vertexPositions[index0 * 3];
                    vertex1 = geometry->vertexPositions + index1 * 3;
                    vertex0 = geometry->vertexPositions + index0 * 3;
                    edge1Y = geometry->vertexPositions[index1 * 3 + 1] - vertex0[1];
                    edge2X = geometry->vertexPositions[index2 * 3] - *vertex1;
                    vertex2 = geometry->vertexPositions + index2 * 3;
                    edge2Y = vertex2[1] - vertex1[1];
                    edge2Z = vertex2[2] - vertex1[2];
                    normalX = edge2Y * (vertex1[2] - vertex0[2]) - edge2Z * edge1Y;
                    normalY = -(edge2X * (vertex1[2] - vertex0[2]) - edge2Z * edge1X);
                    normalZ = edge2X * edge1Y - edge2Y * edge1X;
                    if (normalX * viewPosition[0] +
                        normalY * viewPosition[1] +
                        normalZ * viewPosition[2] +
                        -(normalZ * vertex0[2] + normalY * vertex0[1] + normalX * *vertex0) <=
                        EQGFX_ZERO_F_GUESS)
                    {
                        *elementVisible = 2;
                    }
                    else
                    {
                        *elementVisible = 1;
                    }
                    vertexVisible[index0] = 1;
                    vertexVisible[index1] = 1;
                    vertexVisible[index2] = 1;
                }
                elementVisible++;
                triangle++;
            } while (--triangleCount != 0);
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x100368b5
// Checks which triangles face the viewer in scaled models for visibility rendering.
// @early-stop: setup registers and several x87 instructions still differ (similarity 0.4189).
/* fake match: keep the normal pair in the original stack slots and assign the count after orientation selection. */
int __cdecl t3dClassifyScaledTriangleFacing_guess(TriangleCullVisibilityInput_guess *input, float *viewPosition,
                         TriangleCullVisibilityOutput_guess *output)
{
    TriangleCullScene_guess *scene;
    TriangleCullMatrixObject_guess **matrixObjects;
    TriangleCullVisibilityGeometry_guess *geometry;
    short *vertexPositions;
    TriangleCullVisibilityBatch_guess *batch;
    TriangleCullVisibilityBatch_guess *batchEnd;
    TriangleCullVisibilityTriangle_guess *triangle;
    unsigned char *vertexVisible;
    unsigned char *elementVisible;
    unsigned short index0;
    unsigned short index1;
    unsigned short index2;
    short *vertex0;
    short *vertex1;
    short *vertex2;
    float scale;
    float edge1X;
    float edge1Y;
    float edge1Z;
    float edge2X;
    float edge2Y;
    float edge2Z;
    float normalXY[2];
    float normalZ;
    float triangleZero;

    {
        TriangleCullVisibilityOwner_guess *owner;

        owner = input->owner;
        scene = input->scene;
        matrixObjects = scene->matrixObjects;
        geometry = owner->geometry;
        batch = geometry->batches;
        vertexPositions = geometry->vertexPositions;
        vertexVisible = output->vertexVisible;
        elementVisible = output->elementVisible;
        batchEnd = batch + geometry->batchCount;
        triangle = geometry->triangles;
        scale = EQGFX_MESH_SCALE_F_GUESS[geometry->attenuationIndex];
        EQGFX_VISIBILITY_MEMSET_GUESS(vertexVisible, 0, geometry->vertexFlagBytes);
        EQGFX_VISIBILITY_MEMSET_GUESS(elementVisible, 0, owner->geometry->elementFlagBytes);
    }
    if (batch < batchEnd)
    {
        triangleZero = EQGFX_ZERO_F_GUESS;
        do
        {
            unsigned int triangleCount;

        if (matrixObjects[batch->matrixIndex]->orientationMode == 0)
        {
            triangleCount = (unsigned int)batch->triangleCount;
            for (; triangleCount != 0; triangleCount--)
            {
                if ((triangle->flags & 1) == 0)
                {
                    index1 = triangle->index1;
                    index2 = triangle->index2;
                    index0 = triangle->index0;
                    vertex1 = vertexPositions + index1 * 10;
                    vertex0 = vertexPositions + index0 * 10;
                    vertex2 = vertexPositions + index2 * 10;
                    edge1X = (float)(int)*vertex1 * scale - (float)(int)*vertex0 * scale;
                    edge1Y = (float)(int)vertex1[1] * scale - (float)(int)vertex0[1] * scale;
                    edge1Z = (float)(int)vertex1[2] * scale - (float)(int)vertex0[2] * scale;
                    edge2X = (float)(int)*vertex2 * scale - (float)(int)*vertex1 * scale;
                    edge2Y = (float)(int)vertex2[1] * scale - (float)(int)vertex1[1] * scale;
                    edge2Z = (float)(int)vertex2[2] * scale - (float)(int)vertex1[2] * scale;
                    normalXY[0] = edge2Y * edge1Z - edge2Z * edge1Y;
                    normalXY[1] = -(edge2X * edge1Z - edge2Z * edge1X);
                    normalZ = edge2X * edge1Y - edge2Y * edge1X;
                    if (normalXY[0] * viewPosition[0] +
                        normalXY[1] * viewPosition[1] +
                        normalZ * viewPosition[2] +
                        -((float)(int)*vertex0 * scale * normalXY[0] +
                          (float)(int)vertex0[1] * scale * normalXY[1] +
                          (float)(int)vertex0[2] * scale * normalZ) >
                        triangleZero)
                    {
                        *elementVisible = 1;
                        vertexVisible[index0] = 1;
                        vertexVisible[index1] = 1;
                        vertexVisible[index2] = 1;
                    }
                }
                elementVisible++;
                triangle++;
            }
        }
        else
        {
            triangleCount = (unsigned int)batch->triangleCount;
            for (; triangleCount != 0; triangleCount--)
            {
                if ((triangle->flags & 1) == 0)
                {
                    index1 = triangle->index1;
                    index0 = triangle->index0;
                    index2 = triangle->index2;
                    vertex0 = vertexPositions + index0 * 10;
                    vertex1 = vertexPositions + index1 * 10;
                    vertex2 = vertexPositions + index2 * 10;
                    edge1X = (float)(int)*vertex1 * scale - (float)(int)*vertex0 * scale;
                    edge1Y = (float)(int)vertex1[1] * scale - (float)(int)vertex0[1] * scale;
                    edge1Z = (float)(int)vertex1[2] * scale - (float)(int)vertex0[2] * scale;
                    edge2X = (float)(int)*vertex2 * scale - (float)(int)*vertex1 * scale;
                    edge2Y = (float)(int)vertex2[1] * scale - (float)(int)vertex1[1] * scale;
                    edge2Z = (float)(int)vertex2[2] * scale - (float)(int)vertex1[2] * scale;
                    normalXY[0] = edge2Y * edge1Z - edge2Z * edge1Y;
                    normalXY[1] = -(edge2X * edge1Z - edge2Z * edge1X);
                    normalZ = edge2X * edge1Y - edge2Y * edge1X;
                    if (normalXY[0] * viewPosition[0] +
                        normalXY[1] * viewPosition[1] +
                        normalZ * viewPosition[2] +
                        -((float)(int)*vertex0 * scale * normalXY[0] +
                          (float)(int)vertex0[1] * scale * normalXY[1] +
                          (float)(int)vertex0[2] * scale * normalZ) >
                        triangleZero)
                    {
                        *elementVisible = 1;
                    }
                    else
                    {
                        *elementVisible = 2;
                    }
                    vertexVisible[index0] = 1;
                    vertexVisible[index1] = 1;
                    vertexVisible[index2] = 1;
                }
                elementVisible++;
                triangle++;
            }
        }
        } while (++batch < batchEnd);
    }
    return 1;
}

// FUNCTION: EQGFX 0x10036c98
// Checks which triangles face the viewer in shared-buffer models for visibility rendering.
// @early-stop: frame size (0x3c vs 0x44) and x87 evaluation order still differ (330 rewrite vs 356 original instructions).
/* fake match: reread owner->geometry for the original second memset load chain. */
int __cdecl t3dClassifySharedBufferTriangleFacing_guess(TriangleCullVisibilityInput_guess *input, float *viewPosition,
                         TriangleCullVisibilityOutput_guess *output)
{
    TriangleCullVisibilityOwner_guess *owner;
    TriangleCullScene_guess *scene;
    TriangleCullMatrixObject_guess **matrixObjects;
    TriangleCullVisibilityGeometry_guess *geometry;
    BufferOwner_guess *positionOwner;
    BufferMetadata_guess *positionMetadata;
    TriangleCullVisibilityBatch_guess *batch;
    TriangleCullVisibilityBatch_guess *batchEnd;
    TriangleCullVisibilityTriangle_guess *triangle;
    short *vertexPositions;
    unsigned char *vertexVisible;
    unsigned char *elementVisible;
    unsigned int index0;
    unsigned int index1;
    unsigned int index2;
    short *vertex0;
    short *vertex1;
    short *vertex2;
    float scale;
    float edge1X;
    float edge1Y;
    float edge1Z;
    float edge2X;
    float edge2Y;
    float edge2Z;
    float normalX;
    float normalY;
    float normalZ;

    owner = input->owner;
    geometry = owner->geometry;
    scene = input->scene;
    matrixObjects = scene->matrixObjects;
    vertexVisible = output->vertexVisible;
    scale = EQGFX_MESH_SCALE_F_GUESS[geometry->attenuationIndex];
    positionOwner = input->positionOwner;
    positionMetadata = positionOwner->container->metadata;
    vertexPositions = (short *)((unsigned int)positionMetadata->vertexPositions +
                                positionOwner->vertexStart * (unsigned int)positionMetadata->vertexCount * 6);
    batch = geometry->batches;
    batchEnd = batch + geometry->batchCount;
    triangle = geometry->triangles;
    elementVisible = output->elementVisible;
    EQGFX_VISIBILITY_MEMSET_GUESS(vertexVisible, 0, geometry->vertexFlagBytes);
    EQGFX_VISIBILITY_MEMSET_GUESS(elementVisible, 0, owner->geometry->elementFlagBytes);
    for (; batch < batchEnd; batch++)
    {
        if (matrixObjects[batch->matrixIndex]->orientationMode == 0)
        {
            unsigned int triangleCount = (unsigned int)batch->triangleCount;
            for (; triangleCount != 0; triangleCount--)
            {
                if ((triangle->flags & 1) == 0)
                {
                    index1 = triangle->index1;
                    index2 = triangle->index2;
                    index0 = triangle->index0;
                    vertex1 = vertexPositions + index1 * 3;
                    vertex0 = vertexPositions + index0 * 3;
                    vertex2 = vertexPositions + index2 * 3;
                    edge1X = (float)(int)*vertex1 * scale - (float)(int)*vertex0 * scale;
                    edge1Y = (float)(int)vertex1[1] * scale - (float)(int)vertex0[1] * scale;
                    edge1Z = (float)(int)vertex1[2] * scale - (float)(int)vertex0[2] * scale;
                    edge2X = (float)(int)*vertex2 * scale - (float)(int)*vertex1 * scale;
                    edge2Y = (float)(int)vertex2[1] * scale - (float)(int)vertex1[1] * scale;
                    edge2Z = (float)(int)vertex2[2] * scale - (float)(int)vertex1[2] * scale;
                    normalX = edge2Y * edge1Z - edge2Z * edge1Y;
                    normalY = -(edge2X * edge1Z - edge2Z * edge1X);
                    normalZ = edge2X * edge1Y - edge2Y * edge1X;
                    if (normalX * viewPosition[0] +
                        normalY * viewPosition[1] +
                        normalZ * viewPosition[2] +
                        -((float)(int)*vertex0 * scale * normalX +
                          (float)(int)vertex0[1] * scale * normalY +
                          (float)(int)vertex0[2] * scale * normalZ) >
                        EQGFX_ZERO_F_GUESS)
                    {
                        *elementVisible = 1;
                        vertexVisible[index0] = 1;
                        vertexVisible[index1] = 1;
                        vertexVisible[index2] = 1;
                    }
                }
                elementVisible++;
                triangle++;
            }
        }
        else
        {
            unsigned int triangleCount = (unsigned int)batch->triangleCount;
            for (; triangleCount != 0; triangleCount--)
            {
                if ((triangle->flags & 1) == 0)
                {
                    index1 = triangle->index1;
                    index0 = triangle->index0;
                    index2 = triangle->index2;
                    vertex0 = vertexPositions + index0 * 3;
                    vertex1 = vertexPositions + index1 * 3;
                    vertex2 = vertexPositions + index2 * 3;
                    edge1X = (float)(int)*vertex1 * scale - (float)(int)*vertex0 * scale;
                    edge1Y = (float)(int)vertex1[1] * scale - (float)(int)vertex0[1] * scale;
                    edge1Z = (float)(int)vertex1[2] * scale - (float)(int)vertex0[2] * scale;
                    edge2X = (float)(int)*vertex2 * scale - (float)(int)*vertex1 * scale;
                    edge2Y = (float)(int)vertex2[1] * scale - (float)(int)vertex1[1] * scale;
                    edge2Z = (float)(int)vertex2[2] * scale - (float)(int)vertex1[2] * scale;
                    normalX = edge2Y * edge1Z - edge2Z * edge1Y;
                    normalY = -(edge2X * edge1Z - edge2Z * edge1X);
                    normalZ = edge2X * edge1Y - edge2Y * edge1X;
                    if (normalX * viewPosition[0] +
                        normalY * viewPosition[1] +
                        normalZ * viewPosition[2] +
                        -((float)(int)*vertex0 * scale * normalX +
                          (float)(int)vertex0[1] * scale * normalY +
                          (float)(int)vertex0[2] * scale * normalZ) >
                        EQGFX_ZERO_F_GUESS)
                    {
                        *elementVisible = 1;
                    }
                    else
                    {
                        *elementVisible = 2;
                    }
                    vertexVisible[index0] = 1;
                    vertexVisible[index1] = 1;
                    vertexVisible[index2] = 1;
                }
                elementVisible++;
                triangle++;
            }
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x10037093
// Reorients the view clipping volume for transformed models and sprites.
// __cdecl (the callee pops no arguments, ECX is not an input), void (the original never writes EAX before
// RET). Transforms the frustum's two points with the 4x3 matrix and its six planes' normals with the 3x3
// matrix, then rebuilds each plane's distance from a transformed point (the original's term order).
void __cdecl transformFrustumWithMatrices_guess(Frustum_guess *frustum, float *matrix, float *rotation,
                                                Frustum_guess *result)
{
    float x;
    float y;
    float z;
    float *source;
    float *destination;
    int count;

    x = frustum->point0[0];
    y = frustum->point0[1];
    z = frustum->point0[2];
    result->point0[0] = x * matrix[0] + z * matrix[6] + y * matrix[3] + matrix[9];
    result->point0[1] = x * matrix[1] + z * matrix[7] + y * matrix[4] + matrix[10];
    result->point0[2] = x * matrix[2] + z * matrix[8] + y * matrix[5] + matrix[11];
    x = frustum->point1[0];
    y = frustum->point1[1];
    z = frustum->point1[2];
    result->point1[0] = x * matrix[0] + z * matrix[6] + y * matrix[3] + matrix[9];
    result->point1[1] = x * matrix[1] + z * matrix[7] + y * matrix[4] + matrix[10];
    result->point1[2] = x * matrix[2] + z * matrix[8] + y * matrix[5] + matrix[11];
    source = &frustum->planes[0].y;
    destination = &result->planes[0].z;
    count = 6;
    do
    {
        x = source[-1];
        y = source[0];
        z = source[1];
        destination[-2] = z * rotation[2] + y * rotation[1] + x * rotation[0];
        destination[-1] = x * rotation[3] + z * rotation[5] + y * rotation[4];
        destination[0] = x * rotation[6] + z * rotation[8] + y * rotation[7];
        source += 5;
        destination += 5;
        count--;
    } while (count != 0);
    result->planes[4].distance = result->planes[4].z * result->point0[2] +
                                 result->planes[4].y * result->point0[1] +
                                 result->planes[4].x * result->point0[0];
    result->planes[1].distance = result->planes[1].z * result->point0[2] +
                                 result->planes[1].y * result->point0[1] +
                                 result->planes[1].x * result->point0[0];
    result->planes[3].distance = result->planes[3].z * result->point0[2] +
                                 result->planes[3].y * result->point0[1] +
                                 result->planes[3].x * result->point0[0];
    result->planes[5].distance = result->planes[5].z * result->point1[2] +
                                 result->planes[5].y * result->point1[1] +
                                 result->planes[5].x * result->point1[0];
    result->planes[0].distance = result->planes[0].y * result->point1[1] +
                                 result->planes[0].x * result->point1[0] +
                                 result->planes[0].z * result->point1[2];
    result->planes[2].distance = result->planes[2].z * result->point1[2] +
                                 result->planes[2].y * result->point1[1] +
                                 result->planes[2].x * result->point1[0];
}

// FUNCTION: EQGFX 0x1003728c
// Prepares and draws a triangle-lit object, including associated effects and selection rendering.
int __cdecl s3dPaintHierarchicalObject_guess(void *renderTarget, Renderer_guess *renderer, RenderSceneRoot_guess *sceneRoot)
{
    int result;
    int boundsResult;
    int selectionMode;
    RenderSceneLink_guess *sceneLink;
    HierarchicalObject_guess *object;
    HierarchicalObjectData_guess *objectData;
    TriangleLightingData_guess *lightingData;

    result = 1;
    sceneLink = (RenderSceneLink_guess *)sceneRoot->sceneLink;
    object = sceneLink->object;
    objectData = object->data;
    lightingData = objectData->triangleLighting;
    if (lightingData == 0)
        return 0;
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        boundsResult = EQGFX_CALCULATE_OBJECT_BOUNDS_GUESS(renderer->visibilityBoundsContext,
                                                            lightingData->boundsPoint0,
                                                            lightingData->boundsPoint1);
        if (boundsResult == -1)
            goto restoreVisibilityResult;
        renderer->visibilityResult = boundsResult;
    }
    renderer->inverseTranslation[0] = -renderer->translation[0];
    renderer->inverseTranslation[1] = -renderer->translation[1];
    renderer->inverseTranslation[2] = -renderer->translation[2];
    lightingData = objectData->triangleLighting;
    if (lightingData->renderEnabled > 0)
    {
        EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
        EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, objectData);
        EQ_CDECL(void, 0x1003CC23, (HierarchicalObject_guess *, Renderer_guess *))(object, renderer);
        EQGFX_APPLY_OBJECT_AMBIENT_GUESS(
            object, renderer, renderTarget, 1.0f);
        EQ_CDECL(void, 0x1003A097, (HierarchicalObject_guess *, Renderer_guess *))(object, renderer);
        EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS(renderer->callbackInput,
                                                  lightingData->triangleCount,
                                                  renderer->vertexVisible);
        EQ_CDECL(void, 0x100377A3, (HierarchicalObject_guess *, Renderer_guess *))(object, renderer);
        EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
        EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(lightingData->effectBufferIndex);
        if (renderer->visibilityResult == 0)
        {
            selectionMode = g_render_mode_guess;
            if (selectionMode == 2)
            {
                EQGFX_RENDER_MODE_TWO_PREPARE_GUESS(object,
                                                                                            renderer);
                EQGFX_RENDER_MODE_TWO_SCENE_FINISH_GUESS(
                    renderTarget, renderer, object);
            }
            else if (selectionMode == 1)
            {
                EQGFX_RENDER_MODE_ONE_PREPARE_GUESS(object,
                                                                                            renderer);
                EQGFX_RENDER_MODE_ONE_SCENE_FINISH_GUESS(
                    renderTarget, renderer, object);
            }
        }
        else
        {
            EQGFX_RENDER_FLAGGED_OBJECT_GUESS(
                renderTarget, renderer, object);
        }
    }
    else
    {
        renderer->firstRenderDataValue = lightingData->firstOutputIndex;
        renderer->secondRenderDataValue = lightingData->renderDataValue;
        result = EQ_CDECL(int, 0x1003D353, (void *, Renderer_guess *))(renderTarget, renderer);
    }

restoreVisibilityResult:
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
        renderer->visibilityResult = renderer->previousVisibilityResult;
    return result;
}

// FUNCTION: EQGFX 0x10037411
// Renders animated hierarchical scene objects through their associated render object and selection mode.
int __cdecl t3dRenderHierarchicalObjectScene_guess(void *renderTarget, Renderer_guess *renderer, RenderSceneRoot_guess *sceneRoot)
{
    RenderSceneLink_guess *sceneLink;
    HierarchicalObject_guess *object;
    HierarchicalObjectData_guess *objectData;
    HierarchicalTransformData_guess *transformData;
    float boundsPoint0[3];
    float boundsPoint1[3];

    sceneLink = (RenderSceneLink_guess *)sceneRoot->sceneLink;
    object = sceneLink->object;
    objectData = object->data;
    transformData = objectData->vertexTransform;
    if (transformData == 0)
        return 0;
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        boundsPoint0[0] = transformData->boundsOffset0[0] + transformData->objectPosition[0];
        boundsPoint0[1] = transformData->boundsOffset0[1] + transformData->objectPosition[1];
        boundsPoint0[2] = transformData->boundsOffset0[2] + transformData->objectPosition[2];
        boundsPoint1[0] = transformData->boundsOffset1[0] + transformData->objectPosition[0];
        boundsPoint1[1] = transformData->boundsOffset1[1] + transformData->objectPosition[1];
        boundsPoint1[2] = transformData->boundsOffset1[2] + transformData->objectPosition[2];
        renderer->visibilityResult = EQGFX_CALCULATE_OBJECT_BOUNDS_GUESS(
            renderer->visibilityBoundsContext, boundsPoint0, boundsPoint1);
        if (renderer->visibilityResult == -1)
            goto restore_visibility;
    }
    if (transformData->animationSampleCount != 0)
    {
        EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
        EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, objectData);
        EQGFX_REFRESH_RENDER_OBJECT_GUESS(objectData, object->renderObject, renderer);
        EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, &object->x,
            transformData->lightingState == 0 ? 0x30000000 : 0x20000000, 0x40);
        EQGFX_RENDER_HIERARCHICAL_OBJECT_GUESS(object, renderer, renderTarget);
        EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS(
            renderer->callbackInput, transformData->vertexCount, renderer->vertexVisible);
        EQGFX_SUBMIT_HIERARCHICAL_TRANSFORM_GUESS(objectData, &object->x, renderer);
        EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
        EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(transformData->effectBufferIndex);
        if (EQGFX_RENDER_VISIBILITY_MODE_GUESS == 0 || renderer->visibilityResult != 0)
        {
            EQGFX_RENDER_STANDARD_SCENE_GUESS(
                renderTarget, renderer, objectData, object->renderObject);
        }
        else if (g_render_mode_guess == 2)
        {
            EQGFX_RENDER_MODE_TWO_GUESS(objectData, object->renderObject, renderer);
            EQGFX_RENDER_MODE_TWO_SCENE_GUESS(
                renderTarget, renderer, objectData, object->renderObject);
        }
        else if (g_render_mode_guess == 1)
        {
            EQGFX_RENDER_MODE_ONE_GUESS(objectData, object->renderObject, renderer);
            EQGFX_RENDER_MODE_ONE_SCENE_GUESS(
                renderTarget, renderer, objectData, object->renderObject);
        }
    }
restore_visibility:
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
        renderer->visibilityResult = renderer->previousVisibilityResult;
    return 1;
}

#define EQGFX_VISIBILITY_PASS_GUESS EQ_CDECL(int, 0x1003CDF0, (Renderer_guess *, HierarchicalObjectData_guess *))
#define EQGFX_BUILD_OBJECT_VERTICES_GUESS EQ_CDECL(void, 0x10039BE2, (HierarchicalObjectData_guess *, Renderer_guess *, void *))
#define EQGFX_RENDER_UNSELECTED_SCENE_GUESS EQ_CDECL(void, 0x1003C119, (void *, Renderer_guess *, HierarchicalObjectData_guess *, void *))

// Visibility mode off: visibilityResult holds the scene root (never 0), and only its zero test below matters.
// FUNCTION: EQGFX 0x100375c1
// Builds animated object vertices after triangle visibility testing, then draws geometry or a selection overlay.
int __cdecl t3dRenderObjectWithTriangleVisibility_guess(void *renderTarget, Renderer_guess *renderer, RenderSceneRoot_guess *sceneRoot)
{
    RenderSceneLink_guess *sceneLink;
    HierarchicalObjectData_guess *objectData;
    HierarchicalTransformData_guess *transformData;
    float boundsPoint0[3];
    float boundsPoint1[3];
    int visibilityResult;

    sceneLink = (RenderSceneLink_guess *)sceneRoot->sceneLink;
    objectData = (HierarchicalObjectData_guess *)sceneLink->object;
    transformData = objectData->vertexTransform;
    if (transformData == 0)
        return 0;
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        boundsPoint0[0] = transformData->boundsOffset0[0] + transformData->objectPosition[0];
        boundsPoint0[1] = transformData->boundsOffset0[1] + transformData->objectPosition[1];
        boundsPoint0[2] = transformData->boundsOffset0[2] + transformData->objectPosition[2];
        boundsPoint1[0] = transformData->boundsOffset1[0] + transformData->objectPosition[0];
        boundsPoint1[1] = transformData->boundsOffset1[1] + transformData->objectPosition[1];
        boundsPoint1[2] = transformData->boundsOffset1[2] + transformData->objectPosition[2];
        visibilityResult = EQGFX_CALCULATE_OBJECT_BOUNDS_GUESS(
            renderer->visibilityBoundsContext, boundsPoint0, boundsPoint1);
        if (visibilityResult == -1)
            goto restore_visibility;
        renderer->visibilityResult = visibilityResult;
    }
    else
        visibilityResult = (int)sceneRoot;
    if (transformData->animationSampleCount != 0)
    {
        EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
        EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, objectData);
        EQGFX_REFRESH_RENDER_OBJECT_GUESS(objectData, transformData->renderObject, renderer);
        EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, transformData->objectPosition,
            transformData->lightingState == 0 ? 0x30000000 : 0x20000000, 0x40);
        EQGFX_SUBMIT_HIERARCHICAL_TRANSFORM_GUESS(objectData, transformData->objectPosition, renderer);
        if (visibilityResult != 0)
        {
            EQGFX_VISIBILITY_PASS_GUESS(renderer, objectData);
            if (renderer->visibilityResult == -1)
                goto restore_visibility;
        }
        EQGFX_BUILD_OBJECT_VERTICES_GUESS(objectData, renderer, renderTarget);
        EQGFX_HIERARCHICAL_RENDER_CALLBACK_GUESS(
            renderer->callbackInput, transformData->vertexCount, renderer->vertexVisible);
        EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(transformData->renderObject, renderer);
        EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(transformData->effectBufferIndex);
        if (EQGFX_RENDER_VISIBILITY_MODE_GUESS == 0 || renderer->visibilityResult != 0)
        {
            EQGFX_RENDER_UNSELECTED_SCENE_GUESS(
                renderTarget, renderer, objectData, transformData->renderObject);
        }
        else if (g_render_mode_guess == 2)
        {
            EQGFX_RENDER_MODE_TWO_GUESS(objectData, transformData->renderObject, renderer);
            EQGFX_RENDER_MODE_TWO_SCENE_GUESS(
                renderTarget, renderer, objectData, transformData->renderObject);
        }
        else if (g_render_mode_guess == 1)
        {
            EQGFX_RENDER_MODE_ONE_GUESS(objectData, transformData->renderObject, renderer);
            EQGFX_RENDER_MODE_ONE_SCENE_GUESS(
                renderTarget, renderer, objectData, transformData->renderObject);
        }
    }
restore_visibility:
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
        renderer->visibilityResult = renderer->previousVisibilityResult;
    return 1;
}

// FUNCTION: EQGFX 0x100377a3
// Passes transformed triangle coordinates to the object's triangle-lighting callback.
int __cdecl t3dInvokeTriangleLightingTransformCallback_guess(HierarchicalObject_guess *object,
                         TriangleLightingTransformInput_guess *transformInput)
{
    float callbackValues[12];
    float transformFactorX;
    float transformFactorY;
    float transformFactorZ;

    callbackValues[0] = transformInput->firstSourceX;
    transformFactorX = transformInput->transformFactorX;
    transformFactorY = transformInput->transformFactorY;
    transformFactorZ = transformInput->transformFactorZ;
    callbackValues[3] = transformInput->firstSourceY;
    callbackValues[6] = transformInput->firstSourceZ;
    callbackValues[1] = transformInput->secondSourceX;
    callbackValues[4] = transformInput->secondSourceY;
    callbackValues[7] = transformInput->secondSourceZ;
    callbackValues[2] = transformInput->thirdSourceX;
    callbackValues[5] = transformInput->thirdSourceY;
    callbackValues[9] = transformFactorX * callbackValues[0] +
                        transformFactorY * callbackValues[3] +
                        transformFactorZ * callbackValues[6];
    callbackValues[8] = transformInput->thirdSourceZ;
    callbackValues[10] = transformFactorX * callbackValues[1] +
                         transformFactorY * callbackValues[4] +
                         transformFactorZ * callbackValues[7];
    callbackValues[11] = transformFactorX * callbackValues[2] +
                         transformFactorY * callbackValues[5] +
                         transformFactorZ * callbackValues[8];
    EQGFX_TRIANGLE_LIGHTING_CALLBACK_GUESS(
        object->data->triangleLighting->firstOutputIndex,
        transformInput->callbackArgument1,
        &callbackValues[0],
        object->data->triangleLighting->triangleCount,
        transformInput->callbackArgument4);
    return 1;
}

// FUNCTION: EQGFX 0x10037885
// Transforms a mesh's packed vertices by the render matrix and translation+offset, scaled by the table entry.
int __cdecl transformBufferVerticesWithOffset_guess(BufferContainer_guess *sourceContainer, float *translationOffset,
                         RenderState_guess *renderState)
{
    BufferMetadata_guess *metadata;
    float matrixAndPosition[12];
    float x;
    float y;
    float z;

    matrixAndPosition[0] = renderState->xFromX;
    matrixAndPosition[3] = renderState->xFromY;
    matrixAndPosition[6] = renderState->xFromZ;
    matrixAndPosition[1] = renderState->yFromX;
    matrixAndPosition[4] = renderState->yFromY;
    matrixAndPosition[7] = renderState->yFromZ;
    matrixAndPosition[2] = renderState->zFromX;
    matrixAndPosition[5] = renderState->zFromY;
    matrixAndPosition[8] = renderState->zFromZ;
    x = renderState->translation[0] + translationOffset[0];
    y = renderState->translation[1] + translationOffset[1];
    z = renderState->translation[2] + translationOffset[2];
    metadata = sourceContainer->metadata;
    matrixAndPosition[9] = x * matrixAndPosition[0] + y * matrixAndPosition[3] + z * matrixAndPosition[6];
    matrixAndPosition[10] = x * matrixAndPosition[1] + y * matrixAndPosition[4] + z * matrixAndPosition[7];
    matrixAndPosition[11] = x * matrixAndPosition[2] + y * matrixAndPosition[5] + z * matrixAndPosition[8];
    EQGFX_TRANSFORM_VERTICES_GUESS((short *)metadata->sourceAddress, renderState->destinationVertices,
                                   matrixAndPosition, metadata->vertexCount,
                                   (char *)renderState->vertexVisible,
                                   EQGFX_MESH_SCALE_F_GUESS[metadata->attenuationIndex]);
    return 1;
}

#define EQGFX_SPHERE_CULL_TEST_GUESS EQ_CDECL(int, 0x10035245, (Renderer_guess *, float, float, float, float))
#define EQGFX_ADJUST_DM_SPRITE_MESH_DETAIL_GUESS EQ_CDECL(void, 0x10011290, (HierarchicalObjectData_guess *, float))
#define EQGFX_GET_ACTOR_SCALED_AMBIENT_GUESS EQ_CDECL(float, 0x10037F34, (void *, void *, T3D_tagACTORINSTANCE *))
#define EQGFX_PREPARE_OBJECT_GROUPS_GUESS EQ_CDECL(void, 0x10038162, (HierarchicalObject_guess *, unsigned int, MatrixRecordSet_guess *, Renderer_guess *))
#define EQGFX_PREPARE_OBJECT_TRANSFORMS_GUESS EQ_CDECL(void, 0x1003C6FA, (HierarchicalObject_guess *, Renderer_guess *))
#define EQGFX_RENDER_OBJECT_OUTPUT_GUESS EQ_CDECL(void, 0x10039134, (HierarchicalObject_guess *, MatrixRecordSet_guess *, T3D_tagACTORINSTANCE *, Renderer_guess *, float))
#define EQGFX_VERTEX_SLOT_CALLBACK_GUESS EQ_GLOBAL(VertexSlotCallback_guess, 0x100FC778)
#define EQGFX_LOD_DISTANCE_SCALE_FLOAT_GUESS EQ_GLOBAL(float, 0x100A16B0)

// FUNCTION: EQGFX 0x10037979
// Draws an actor's skinned mesh with lighting, detail adjustment, and visible triangle slots.
// __cdecl (the callee pops nothing; ECX is not an input), int: 1 drawn, 0 without triangle lighting, 0xffff3d3e when
// the lighting is not render-enabled. The third view output's z term reads basis[2][0], as the original does.
int __cdecl s3dPaintSkinDMSprite_guess(HierarchicalObject_guess *object, void *world, Renderer_guess *renderer,
                         T3D_tagACTORINSTANCE *actor, MatrixRecordSet_guess *matrixSet)
{
    int result;
    HierarchicalObjectData_guess *data;
    TriangleSlotEntry_guess *entry;
    TriangleSlotEntry_guess *entryEnd;
    BufferOwner_guess *renderObject;
    SourceMatrix_guess *matrix;
    float positionX;
    float positionY;
    float positionZ;
    float viewPosition[3];
    float sphereRadius;
    float detailScale;
    float ambient;

    result = 1;
    data = object->data;
    if (data->triangleLighting == 0)
    {
        return 0;
    }
    renderObject = (BufferOwner_guess *)object->renderObject;
    positionX = object->x;
    positionY = object->y;
    positionZ = object->z;
    matrix = (SourceMatrix_guess *)matrixSet->records;
    viewPosition[0] = positionY * matrix->basis[1][0] + positionZ * matrix->basis[2][0]
                      + positionX * matrix->basis[0][0] + matrix->translation[0];
    viewPosition[1] = positionZ * matrix->basis[2][1] + positionY * matrix->basis[1][1]
                      + positionX * matrix->basis[0][1] + matrix->translation[1];
    viewPosition[2] = positionX * matrix->basis[0][2] + positionY * matrix->basis[1][2]
                      + positionZ * matrix->basis[2][0] + matrix->translation[2];
    sphereRadius = matrixSet->scale->scaleFactor * actor->scaleFactor_guess;
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS == 0 ||
        EQGFX_SPHERE_CULL_TEST_GUESS(renderer, viewPosition[0], viewPosition[1], viewPosition[2], sphereRadius) != 0)
    {
        if (data->triangleLighting->detailLevels != 0)
        {
            if (EQGFX_SPRITE_DISTANCE_ENABLED_GUESS == 1)
            {
                detailScale = (((renderer->translation[2] + actor->position_guess[2]) * renderer->zFromZ
                                + (renderer->translation[1] + actor->position_guess[1]) * renderer->zFromY
                                + (renderer->translation[0] + actor->position_guess[0]) * renderer->zFromX)
                               - EQGFX_VIEW_DISTANCE_OFFSET_GUESS)
                              / (renderer->distanceScale * actor->boundingRadius_guess * actor->scaleFactor_guess)
                              * EQGFX_LOD_DISTANCE_SCALE_FLOAT_GUESS;
            }
            else
            {
                detailScale = 0.0f;
            }
            EQGFX_ADJUST_DM_SPRITE_MESH_DETAIL_GUESS(data, detailScale);
        }
        EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
        EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, data);
        ambient = EQGFX_GET_ACTOR_SCALED_AMBIENT_GUESS(world, renderer, actor);
        EQGFX_PREPARE_OBJECT_GROUPS_GUESS(object, (unsigned int)actor, matrixSet, renderer);
        EQGFX_PREPARE_OBJECT_TRANSFORMS_GUESS(object, renderer);
        if (data->triangleLighting->renderEnabled == 0)
        {
            result = 0xffff3d3e;
        }
        else
        {
            EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, viewPosition, 0x10000000, 3);
            EQGFX_APPLY_OBJECT_AMBIENT_GUESS(object, renderer, world, ambient);
            EQGFX_RENDER_OBJECT_OUTPUT_GUESS(object, matrixSet, actor, renderer, ambient);
            entryEnd = data->triangleLighting->entries + data->triangleLighting->entryCount;
            for (entry = data->triangleLighting->entries; entry < entryEnd; entry++)
            {
                EQGFX_VERTEX_SLOT_CALLBACK_GUESS((unsigned char *)renderer->callbackInput
                                                 + entry->vertexIndex * EQGFX_DESTINATION_VERTEX_STRIDE,
                                                 entry->value,
                                                 (unsigned char *)renderer->vertexVisible + entry->vertexIndex,
                                                 renderObject->slotEntries + entry->slotIndex);
            }
            EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
            EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(data->triangleLighting->effectBufferIndex);
            if (EQGFX_RENDER_VISIBILITY_MODE_GUESS == 0 || renderer->visibilityResult != 0)
            {
                EQGFX_RENDER_FLAGGED_OBJECT_GUESS(world, renderer, object);
            }
            else if (g_render_mode_guess == 2)
            {
                EQGFX_RENDER_MODE_TWO_PREPARE_GUESS(object, renderer);
                EQGFX_RENDER_MODE_TWO_SCENE_FINISH_GUESS(world, renderer, object);
            }
            else if (g_render_mode_guess == 1)
            {
                EQGFX_RENDER_MODE_ONE_PREPARE_GUESS(object, renderer);
                EQGFX_RENDER_MODE_ONE_SCENE_FINISH_GUESS(world, renderer, object);
            }
        }
    }
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        renderer->visibilityResult = renderer->previousVisibilityResult;
    }
    return result;
}

#define EQGFX_ADJUST_DM_SPRITE_MESH_DETAIL2_GUESS EQ_CDECL(void, 0x10011408, (HierarchicalObjectData_guess *, float))
#define EQGFX_TRANSFORM_VERTICES_BY_BATCH_GUESS EQ_CDECL(void, 0x100385C0, (HierarchicalObject_guess *, unsigned int, MatrixRecordSet_guess *, Renderer_guess *))
#define EQGFX_PREPARE_OBJECT_VERTEX_BUFFERS_GUESS EQ_CDECL(void, 0x1003C99D, (HierarchicalObject_guess *, Renderer_guess *))
#define EQGFX_PROCESS_SPRITE_VERTEX_LIGHTING_GUESS EQ_CDECL(void, 0x10039401, (HierarchicalObject_guess *, MatrixRecordSet_guess *, T3D_tagACTORINSTANCE *, Renderer_guess *, void *, float))

// FUNCTION: EQGFX 0x10037c24
// Renders a hierarchical model with its animation, lighting, effects, and visibility checks.
// __cdecl (the callee pops nothing; ECX is not an input), int: -1 without a vertex transform, 0xffff3d3e without
// animation samples, else 1. The third view output's z term reads basis[2][0], as the original does.
int __cdecl renderHierarchicalObject_guess(HierarchicalObject_guess *object, void *world, Renderer_guess *renderer,
                         T3D_tagACTORINSTANCE *actor, MatrixRecordSet_guess *matrixSet)
{
    int result;
    BufferOwner_guess *renderObject;
    HierarchicalObjectData_guess *data;
    SourceMatrix_guess *matrix;
    TriangleLightingGroup_guess *group;
    TriangleLightingGroup_guess *groupEnd;
    unsigned int vertexOffset;
    int visibility;
    float positionX;
    float positionY;
    float positionZ;
    float viewPosition[3];
    float sphereRadius;
    float detailScale;
    float ambient;

    result = 1;
    data = object->data;
    if (data->vertexTransform == 0)
    {
        return -1;
    }
    renderObject = (BufferOwner_guess *)object->renderObject;
    positionX = object->x;
    positionY = object->y;
    positionZ = object->z;
    matrix = (SourceMatrix_guess *)matrixSet->records;
    viewPosition[0] = positionY * matrix->basis[1][0] + positionZ * matrix->basis[2][0]
                      + positionX * matrix->basis[0][0] + matrix->translation[0];
    viewPosition[1] = positionZ * matrix->basis[2][1] + positionY * matrix->basis[1][1]
                      + positionX * matrix->basis[0][1] + matrix->translation[1];
    viewPosition[2] = positionY * matrix->basis[1][2] + positionX * matrix->basis[0][2]
                      + positionZ * matrix->basis[2][0] + matrix->translation[2];
    sphereRadius = matrixSet->scale->scaleFactor * matrix->sphereScale * actor->scaleFactor_guess;
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS == 0 ||
        EQGFX_SPHERE_CULL_TEST_GUESS(renderer, viewPosition[0], viewPosition[1], viewPosition[2], sphereRadius) != 0)
    {
        if (data->vertexTransform->detailLevels != 0)
        {
            if (EQGFX_SPRITE_DISTANCE_ENABLED_GUESS == 1)
            {
                detailScale = (((renderer->translation[2] + actor->position_guess[2]) * renderer->zFromZ
                                + (renderer->translation[1] + actor->position_guess[1]) * renderer->zFromY
                                + (renderer->translation[0] + actor->position_guess[0]) * renderer->zFromX)
                               - EQGFX_VIEW_DISTANCE_OFFSET_GUESS)
                              / (renderer->distanceScale * actor->boundingRadius_guess * actor->scaleFactor_guess)
                              * EQGFX_LOD_DISTANCE_SCALE_FLOAT_GUESS;
            }
            else
            {
                detailScale = 0.0f;
            }
            EQGFX_ADJUST_DM_SPRITE_MESH_DETAIL2_GUESS(data, detailScale);
        }
        EQGFX_PREPARE_HIERARCHICAL_RENDER_GUESS(renderer, data);
        EQGFX_TRANSFORM_VERTICES_BY_BATCH_GUESS(object, (unsigned int)actor, matrixSet, renderer);
        EQGFX_PREPARE_OBJECT_VERTEX_BUFFERS_GUESS(object, renderer);
        if (data->vertexTransform->animationSampleCount == 0)
        {
            result = 0xffff3d3e;
        }
        else
        {
            if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0 && renderer->visibilityResult != 0)
            {
                visibility = EQGFX_VISIBILITY_PASS_GUESS(renderer, data);
                renderer->visibilityResult = visibility;
                if (visibility == -1)
                {
                    goto finish;
                }
            }
            EQGFX_UPDATE_RENDER_ANIMATION_GUESS(renderer);
            ambient = EQGFX_GET_ACTOR_SCALED_AMBIENT_GUESS(world, renderer, actor);
            EQGFX_APPLY_PHYSICS_EFFECTS_GUESS(renderer, viewPosition, 0x10000000, 3);
            EQGFX_PROCESS_SPRITE_VERTEX_LIGHTING_GUESS(object, matrixSet, actor, renderer, world, ambient);
            group = data->vertexTransform->groupEntries;
            vertexOffset = 0;
            groupEnd = group + data->vertexTransform->groupCount;
            for (; group < groupEnd; group++)
            {
                EQGFX_VERTEX_SLOT_CALLBACK_GUESS((unsigned char *)renderer->callbackInput
                                                 + vertexOffset * EQGFX_DESTINATION_VERTEX_STRIDE,
                                                 group->vertexCount,
                                                 renderer->vertexVisible + vertexOffset,
                                                 renderObject->slotEntries + group->modelIndex);
                vertexOffset += group->vertexCount;
            }
            EQGFX_FLUSH_EFFECT_BUFFERS_GUESS(data->vertexTransform->effectBufferIndex);
            EQGFX_RENDER_ASSOCIATED_OBJECT_GUESS(object->renderObject, renderer);
            if (EQGFX_RENDER_VISIBILITY_MODE_GUESS == 0 || renderer->visibilityResult != 0)
            {
                EQGFX_RENDER_UNSELECTED_SCENE_GUESS(world, renderer, data, object->renderObject);
            }
            else if (g_render_mode_guess == 2)
            {
                EQGFX_RENDER_MODE_TWO_GUESS(data, object->renderObject, renderer);
                EQGFX_RENDER_MODE_TWO_SCENE_GUESS(world, renderer, data, object->renderObject);
            }
            else if (g_render_mode_guess == 1)
            {
                EQGFX_RENDER_MODE_ONE_GUESS(data, object->renderObject, renderer);
                EQGFX_RENDER_MODE_ONE_SCENE_GUESS(world, renderer, data, object->renderObject);
            }
        }
    }
finish:
    if (EQGFX_RENDER_VISIBILITY_MODE_GUESS != 0)
    {
        renderer->visibilityResult = renderer->previousVisibilityResult;
    }
    return result;
}

#define g_byteFloatTable_guess EQ_ARRAY(float, 0x100FA068)
#define EQGFX_CORNER_AVERAGE_SCALE_GUESS EQ_GLOBAL(float, 0x10076310)
#define EQGFX_WRAP_SPAN_GUESS EQ_GLOBAL(float, 0x1007630C)
#define EQGFX_COLLIDE_RAY_SEGMENT_WITH_WORLD_GUESS EQ_CDECL(int, 0x1000482E, (void *, void *, float *, float *, unsigned int, int, void *, void *, int, RayCollisionResult_guess *))
#define EQGFX_HIT_SPRITE_TAG 0x18
#define EQGFX_HIT_CLIENT_TAG 0x48
#define EQGFX_HIT_MESH_TAG 0x47
#define EQGFX_AMBIENT_DIRTY_FLAG 0x08

// FUNCTION: EQGFX 0x10037F34
// @early-stop: owner/geometry register allocation and block order still differ (161 rewrite vs 171 original instructions).
// Ambient of an actor, cached in actor->ambient: a probe ray 400 units down, the hit's three corner lights averaged.
/* fake match: the shared labels preserve the original's common ambient and cached-result paths. */
float __cdecl s3dGetActorScaledAmbient(void *world, void *textureTable, T3D_tagACTORINSTANCE *actor)
{
    RayCollisionResult_guess collision;
    float probeEnd[3];
    T3D_tagACTORINSTANCE *originalActor;
    int *sprite;
    int *hitObject;
    Client_guess *hitClient;
    Mesh_guess *hitMesh;
    Client_guess *owner;
    Mesh_guess *ownerMesh;
    int *geometryData;
    Element16_guess *element16;
    Element_guess *element;
    VertexLight_guess *vertexLights;
    unsigned int index0;
    unsigned int index1;
    unsigned int index2;
    float ambient;

    originalActor = actor;
    sprite = 0;
    if ((originalActor->flags_guess[3] & EQGFX_AMBIENT_DIRTY_FLAG) == 0)
        return originalActor->ambient_guess;
    probeEnd[0] = originalActor->position_guess[0];
    probeEnd[1] = originalActor->position_guess[1];
    probeEnd[2] = originalActor->position_guess[2] - EQGFX_WRAP_SPAN_GUESS;
    if (EQGFX_COLLIDE_RAY_SEGMENT_WITH_WORLD_GUESS(world, textureTable, originalActor->position_guess, probeEnd, 0, 1,
                                                   (void *)0x10037F12, (void *)0x10041651, 0, &collision) == 0)
        goto use_default_ambient;

    hitObject = (int *)collision.hitObject;
    hitClient = (Client_guess *)hitObject;
    if (hitClient->type == EQGFX_HIT_CLIENT_TAG)
    {
        hitMesh = hitClient->mesh;
    }
    else
    {
        if (hitClient->type != EQGFX_HIT_MESH_TAG)
            goto use_default_ambient;
        hitClient = 0;
        hitMesh = (Mesh_guess *)hitObject;
    }

    if (collision.hitSprite->type == EQGFX_HIT_SPRITE_TAG)
        sprite = (int *)collision.hitSprite;

    geometryData = (int *)hitMesh->elementGeometry;
    if (geometryData != 0)
    {
        element16 = ((ElementGeometry_guess *)geometryData)->elements + collision.entryNumber;
        index0 = element16->index0;
        index1 = element16->index1;
        index2 = element16->index2;
        if ((sprite == 0 || (owner = (Client_guess *)sprite[63]) == 0 ||
             (ownerMesh = owner->mesh, (ownerMesh->flags & 0x80) == 0)) &&
            (hitClient == 0 || (owner = hitClient->parent) == 0 ||
             (ownerMesh = owner->mesh, (ownerMesh->flags & 0x80) == 0)))
        {
            if (((ElementGeometry_guess *)geometryData)->vertexLightCount == 0 ||
                (hitMesh->flags & 0x20) == 0)
                goto use_default_ambient;
            vertexLights = ((ElementGeometry_guess *)geometryData)->vertexLights;
            ambient = g_byteFloatTable_guess[vertexLights[index2].ambientIndex]
                    + g_byteFloatTable_guess[vertexLights[index1].ambientIndex];
            index1 = vertexLights[index0].ambientIndex;
        }
        else
        {
            vertexLights = ownerMesh->vertexLightsBySlot[owner->meshSlot];
            ambient = g_byteFloatTable_guess[vertexLights[index2].ambientIndex]
                    + g_byteFloatTable_guess[vertexLights[index1].ambientIndex];
            index1 = vertexLights[index0].ambientIndex;
        }
        goto average_ambient;
    }

    geometryData = (int *)hitMesh->geometry;
    if (geometryData == 0)
        goto finish_ambient;
    element = ((Geometry_guess *)geometryData)->elements + collision.entryNumber;
    index0 = element->index0;
    index1 = element->index1;
    index2 = element->index2;
    if ((sprite == 0 || (owner = (Client_guess *)sprite[63]) == 0 ||
         (ownerMesh = owner->mesh, (ownerMesh->flags & 0x80) == 0)) &&
        (hitClient == 0 || (owner = hitClient->parent) == 0 ||
         (ownerMesh = owner->mesh, (ownerMesh->flags & 0x80) == 0)))
    {
        if (hitMesh == 0 || ((Geometry_guess *)geometryData)->sourceVertexCount == 0 ||
            (hitMesh->flags & 0x20) == 0)
            goto use_default_ambient;
        ambient = g_byteFloatTable_guess[((Geometry_guess *)geometryData)->sourceVertices[index2].ambientIndex]
                + g_byteFloatTable_guess[((Geometry_guess *)geometryData)->sourceVertices[index0].ambientIndex];
        index1 = ((Geometry_guess *)geometryData)->sourceVertices[index1].ambientIndex;
    }
    else
    {
        vertexLights = ownerMesh->vertexLightsBySlot[owner->meshSlot];
        ambient = g_byteFloatTable_guess[vertexLights[index2].ambientIndex]
                + g_byteFloatTable_guess[vertexLights[index0].ambientIndex];
        index1 = vertexLights[index1].ambientIndex;
    }
    goto average_ambient;

use_default_ambient:
    ambient = 1.0f;
    goto store_ambient;
average_ambient:
    ambient = (ambient + g_byteFloatTable_guess[index1]) * EQGFX_CORNER_AVERAGE_SCALE_GUESS;
store_ambient:
    originalActor->ambient_guess = ambient;
finish_ambient:
    ambient = originalActor->ambient_guess;
    originalActor->flags_guess[3] &= ~EQGFX_AMBIENT_DIRTY_FLAG;
    return ambient;
}

// The source vertex: 10 shorts, of which only the x, y, z at +0x0/+0x2/+0x4 are read. No include/ struct
// has this layout; proposed here as a comment, the code indexes the shorts directly.

// FUNCTION: EQGFX 0x10038162
// Prepares triangle-lighting data for each model group in the current render pass.
void __cdecl transformTriangleLightingGroups_guess(HierarchicalObject_guess *object, unsigned int unusedArgument,
                          MatrixRecordSet_guess *matrixSet, RenderState_guess *renderState)
{
    TriangleLightingData_guess *lighting;
    TriangleLightingGroup_guess *group;
    SourceMatrix_guess *matrix;
    float transformed[12];
    float spriteX;
    float spriteY;
    float spriteZ;
    float xFromX;
    float xFromY;
    float xFromZ;
    float yFromX;
    float yFromY;
    float yFromZ;
    float zFromX;
    float zFromY;
    float zFromZ;
    float translationX;
    float translationY;
    float translationZ;
    float basisX;
    float basisY;
    float basisZ;
    float transformedX;
    float transformedY;
    float transformedZ;
    float *outputBase;
    unsigned int firstOutputIndex;
    unsigned int endOutputIndex;
    unsigned int callbackContext;
    unsigned int vertexCount;

    spriteX = object->x;
    spriteY = object->y;
    spriteZ = object->z;
    lighting = object->data->triangleLighting;
    xFromX = renderState->xFromX;
    xFromY = renderState->xFromY;
    xFromZ = renderState->xFromZ;
    yFromX = renderState->yFromX;
    yFromY = renderState->yFromY;
    yFromZ = renderState->yFromZ;
    zFromX = renderState->zFromX;
    zFromY = renderState->zFromY;
    zFromZ = renderState->zFromZ;
    firstOutputIndex = lighting->firstOutputIndex;
    translationX = renderState->translation[0];
    translationY = renderState->translation[1];
    group = lighting->groups;
    translationZ = renderState->translation[2];
    outputBase = renderState->destinationVertices;
    callbackContext = lighting->callbackContext;
    endOutputIndex = firstOutputIndex + lighting->triangleCount * EQGFX_DESTINATION_VERTEX_STRIDE;
    if (firstOutputIndex < endOutputIndex)
    {
        do
        {
            matrix = (SourceMatrix_guess *)(matrixSet->records + group->modelIndex * EQGFX_MATRIX_RECORD_STRIDE);
            basisX = matrix->basis[0][0];
            basisY = matrix->basis[0][1];
            basisZ = matrix->basis[0][2];
            transformed[0] = basisX * xFromX + basisY * xFromY + basisZ * xFromZ;
            transformed[1] = basisX * yFromX + basisY * yFromY + basisZ * yFromZ;
            transformed[2] = basisX * zFromX + basisY * zFromY + basisZ * zFromZ;
            basisX = matrix->basis[1][0];
            basisY = matrix->basis[1][1];
            basisZ = matrix->basis[1][2];
            transformed[3] = basisX * xFromX + basisY * xFromY + basisZ * xFromZ;
            transformed[4] = basisX * yFromX + basisY * yFromY + basisZ * yFromZ;
            transformed[5] = basisX * zFromX + basisY * zFromY + basisZ * zFromZ;
            basisX = matrix->basis[2][0];
            basisY = matrix->basis[2][1];
            basisZ = matrix->basis[2][2];
            transformed[6] = basisX * xFromX + basisY * xFromY + basisZ * xFromZ;
            transformed[7] = basisX * yFromX + basisY * yFromY + basisZ * yFromZ;
            transformed[8] = basisX * zFromX + basisY * zFromY + basisZ * zFromZ;
            transformedX = spriteX * matrix->basis[0][0] + spriteY * matrix->basis[1][0] + spriteZ * matrix->basis[2][0] + matrix->translation[0] + translationX;
            transformedY = spriteX * matrix->basis[0][1] + spriteY * matrix->basis[1][1] + spriteZ * matrix->basis[2][1] + matrix->translation[1] + translationY;
            transformedZ = spriteY * matrix->basis[1][2] + spriteZ * matrix->basis[2][2] + spriteX * matrix->basis[0][2] + matrix->translation[2] + translationZ;
            transformed[9] = transformedX * xFromX + transformedY * xFromY + transformedZ * xFromZ;
            transformed[10] = transformedX * yFromX + transformedY * yFromY + transformedZ * yFromZ;
            transformed[11] = transformedX * zFromX + transformedY * zFromY + transformedZ * zFromZ;
            vertexCount = group->vertexCount;
            EQGFX_TRIANGLE_LIGHTING_CALLBACK_GUESS(firstOutputIndex, (unsigned int)outputBase,
                                                   (unsigned char *)transformed, vertexCount, callbackContext);
            callbackContext = callbackContext + vertexCount;
            outputBase = (float *)((unsigned char *)outputBase + vertexCount * EQGFX_DESTINATION_VERTEX_STRIDE);
            firstOutputIndex = firstOutputIndex + vertexCount * EQGFX_DESTINATION_VERTEX_STRIDE;
            group = group + 1;
        } while (firstOutputIndex < endOutputIndex);
    }
    return;
}

// FUNCTION: EQGFX 0x100383ec
// __cdecl (the callee pops no arguments, ECX is not an input), void (the original never writes EAX before
// RET). Transforms `count` 10-short vertices by the 12-float matrix (4 rows of 3: x axis, y axis, z axis,
// translation) into 3 floats each; a vertex whose flag byte is 0 keeps its old destination value.
void __cdecl transformVerticesByMatrix_guess(short *vertices, float *out, float *matrix, int count, char *flags)
{
    for (; count != 0; count--)
    {
        if (*flags != '\0')
        {
            float x = (float)vertices[0];
            float y = (float)vertices[1];
            float z = (float)vertices[2];

            out[0] = x * matrix[0] + y * matrix[3] + z * matrix[6] + matrix[9];
            out[1] = x * matrix[1] + y * matrix[4] + z * matrix[7] + matrix[10];
            out[2] = x * matrix[2] + y * matrix[5] + z * matrix[8] + matrix[11];
        }
        vertices += 10;
        out += 3;
        flags++;
    }
}

// FUNCTION: EQGFX 0x10038485
// Applies model scale when placing active mesh vertices in render space.
// __cdecl (the callee pops no arguments, ECX is not an input), void (the original never writes EAX before
// RET). Like 0x100383ec, but each source axis is scaled by `scale` before the matrix; a vertex whose flag
// byte is 0 keeps its old destination value.
void __cdecl transformVertices_guess(short *vertices, float *out, float *matrix, int count, char *flags, float scale)
{
    for (; count != 0; count--)
    {
        if (*flags != '\0')
        {
            float x = (float)vertices[0] * scale;
            float y = (float)vertices[1] * scale;
            float z = (float)vertices[2] * scale;

            out[0] = x * matrix[0] + y * matrix[3] + z * matrix[6] + matrix[9];
            out[1] = x * matrix[1] + y * matrix[4] + z * matrix[7] + matrix[10];
            out[2] = x * matrix[2] + y * matrix[5] + z * matrix[8] + matrix[11];
        }
        vertices += 10;
        out += 3;
        flags++;
    }
}

// FUNCTION: EQGFX 0x10038527
// __cdecl (the callee pops no arguments, ECX is not an input), void (the original never writes EAX before
// RET). Like 0x100383ec's transform, but each source vertex is 3 shorts (6 bytes): flag byte 0 keeps the value.
void __cdecl transformShortVerticesByMatrix_guess(short *vertices, float *out, float *matrix, int count, char *flags)
{
    for (; count != 0; count--)
    {
        if (*flags != '\0')
        {
            float x = (float)vertices[0];
            float y = (float)vertices[1];
            float z = (float)vertices[2];

            out[0] = x * matrix[0] + y * matrix[3] + z * matrix[6] + matrix[9];
            out[1] = x * matrix[1] + y * matrix[4] + z * matrix[7] + matrix[10];
            out[2] = x * matrix[2] + y * matrix[5] + z * matrix[8] + matrix[11];
        }
        vertices += 3;
        out += 3;
        flags++;
    }
}

// FUNCTION: EQGFX 0x100385c0
// Transforms a model's vertex batches using their assigned matrices for rendering.
void __cdecl transformVerticesByBatch_guess(BufferOwner_guess *renderObject, unsigned int unusedParameter, MatrixRecordSet_guess *matrixRecords, TransformedVertexBuffer_guess *vertexBuffer)
{
    unsigned char scratch[48];
    unsigned int sourceEnd;
    BufferBatch_guess *batch;
    int flags;
    unsigned int sourceAddress;
    unsigned int vertexCount;
    unsigned int destinationAddress;

    destinationAddress = vertexBuffer->destinationAddress;
    sourceAddress = renderObject->container->metadata->sourceAddress;
    batch = renderObject->container->metadata->batches;
    flags = (int)renderObject->container->metadata->flags;
    sourceEnd = sourceAddress + (unsigned int)renderObject->container->metadata->vertexCount * EQGFX_SOURCE_VERTEX_STRIDE;
    if (sourceAddress < sourceEnd)
    {
        do
        {
            EQGFX_BUILD_VERTEX_MATRIX_GUESS((int)renderObject, (int)(matrixRecords->records + batch->matrixIndex * EQGFX_MATRIX_RECORD_STRIDE), (int)vertexBuffer, scratch);
            vertexCount = batch->vertexCount;
            EQGFX_TRANSFORM_VERTICES_BY_MATRIX_GUESS((short *)sourceAddress, (float *)destinationAddress, (float *)scratch, (int)vertexCount, (char *)flags);
            flags += vertexCount;
            batch++;
            sourceAddress += vertexCount * EQGFX_SOURCE_VERTEX_STRIDE;
            destinationAddress += vertexCount * EQGFX_DESTINATION_VERTEX_STRIDE;
        } while (sourceAddress < sourceEnd);
    }
    return;
}

// FUNCTION: EQGFX 0x10038651
// Builds triangle-lighting output for each model part in a hierarchical object.
void __cdecl transformTriangleLightingForHierarchicalObject_guess(HierarchicalObject_guess *object, unsigned int unusedValue, HierarchicalModelList_guess *modelList, HierarchicalRenderOutput_guess *output)
{
    unsigned int outputIndex;
    TriangleLightingGroup_guess *group;
    unsigned int callbackContext;
    unsigned int outputEnd;
    unsigned int vertexCount;
    unsigned char scratch[48];
    unsigned int outputBase;
    TriangleLightingData_guess *triangleData;

    outputBase = output->outputBase;
    triangleData = object->data->triangleLighting;
    outputIndex = triangleData->firstOutputIndex;
    group = triangleData->groups;
    callbackContext = triangleData->callbackContext;
    outputEnd = outputIndex + triangleData->triangleCount * 12;
    if (outputIndex < outputEnd)
    {
        do
        {
            EQGFX_BUILD_TRIANGLE_LIGHTING_GUESS(object, &modelList->models[group->modelIndex], output, scratch);
            vertexCount = group->vertexCount;
            EQGFX_TRIANGLE_LIGHTING_CALLBACK_GUESS(outputIndex, outputBase, scratch, vertexCount, callbackContext);
            callbackContext = callbackContext + vertexCount;
            outputBase = outputBase + vertexCount * 12;
            outputIndex = outputIndex + vertexCount * 12;
            group++;
        } while (outputIndex < outputEnd);
    }
}

// FUNCTION: EQGFX 0x100386e0
// Places each posed model part's vertices into scene space.
void __cdecl transformHierarchicalVerticesByModel_guess(HierarchicalObject_guess *object, unsigned int unusedValue, HierarchicalModelList_guess *modelList, HierarchicalRenderOutput_guess *output)
{
    short *vertexInput;
    short *vertexInputEnd;
    TriangleLightingGroup_guess *group;
    unsigned int vertexCount;
    char *vertexFlags;
    unsigned char scratch[48];
    float *vertexOutput;
    HierarchicalTransformData_guess *transformData;

    vertexOutput = (float *)output->outputBase;
    transformData = object->data->vertexTransform;
    vertexInput = transformData->vertexInput;
    group = transformData->groups;
    vertexFlags = transformData->vertexFlags;
    vertexInputEnd = vertexInput + transformData->vertexCount * 10;
    if (vertexInput < vertexInputEnd)
    {
        do
        {
            EQGFX_BUILD_TRANSFORM_MODEL_GUESS(object, &modelList->models[group->modelIndex], output, scratch);
            vertexCount = group->vertexCount;
            EQGFX_TRANSFORM_VERTICES_BY_MATRIX_GUESS(vertexInput, vertexOutput, (float *)scratch, vertexCount, vertexFlags);
            vertexFlags = vertexFlags + vertexCount;
            vertexOutput = vertexOutput + vertexCount * 3;
            vertexInput = vertexInput + vertexCount * 10;
            group++;
        } while (vertexInput < vertexInputEnd);
    }
    return;
}

// FUNCTION: EQGFX 0x10038771
// Adjusts point and directional lighting inputs for a sprite's position and size.
// __cdecl (the callee pops nothing; ECX is not an input), void. Like buildRelativeLightOutputs_guess's pass, but each light's
// position is transformed by the transform matrices, made relative to the sprite, and scaled by pointScale^2.
void __cdecl buildSpriteRelativeLightOutputs_guess(Sprite_guess *sprite, LightTransform_guess *transform, float pointScale, float directionalScale, LightOutput_guess *lights)
{
    Light_guess **light;
    Light_guess **lightEnd;
    LightOutput_guess *out;
    PointLight_guess *point;
    DirectionalLight_guess *directional;
    LightAttachment_guess *attachment;
    float attenuation;
    float x;
    float y;
    float z;

    light = EQGFX_LIGHT_LIST_GUESS;
    lightEnd = light + EQGFX_LIGHT_COUNT_GUESS;
    if (lightEnd > light)
    {
        out = lights;
        do
        {
            if ((*light)->type == 0x1e)
            {
                out->type = 0x1e;
                point = (PointLight_guess *)(*light);
                attachment = point->attachment;
                if (attachment->vertices->positions != 0)
                {
                    out->vertexX = attachment->vertices->positions[attachment->vertexIndex * 3];
                    out->vertexY = attachment->vertices->positions[attachment->vertexIndex * 3 + 1];
                    out->vertexZ = attachment->vertices->positions[attachment->vertexIndex * 3 + 2];
                }
                else
                {
                    out->vertexX = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexY = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexZ = attachment->vertices->values[attachment->vertexIndex];
                }
                attenuation = point->radius * transform->lightScale * point->radius * pointScale * pointScale;
                attenuation = attenuation * EQGFX_POINT_RADIUS_SCALE_GUESS;
                out->vertexX = attenuation * out->vertexX;
                out->vertexY = attenuation * out->vertexY;
                out->vertexZ = attenuation * out->vertexZ;
                x = point->x;
                y = point->y;
                z = point->z;
                out->outX = x * transform->viewTransform[0] + y * transform->viewTransform[3] + z * transform->viewTransform[6] + transform->viewTransform[9] - sprite->x;
                out->outY = x * transform->viewTransform[1] + y * transform->viewTransform[4] + z * transform->viewTransform[7] + transform->viewTransform[10] - sprite->y;
                out->outZ = x * transform->viewTransform[2] + y * transform->viewTransform[5] + z * transform->viewTransform[8] + transform->viewTransform[11] - sprite->z;
            }
            else if ((*light)->type == 0x1f)
            {
                out->type = 0x1f;
                directional = (DirectionalLight_guess *)(*light);
                attachment = directional->attachment;
                if (attachment->vertices->positions != 0)
                {
                    out->vertexX = attachment->vertices->positions[attachment->vertexIndex * 3];
                    out->vertexY = attachment->vertices->positions[attachment->vertexIndex * 3 + 1];
                    out->vertexZ = attachment->vertices->positions[attachment->vertexIndex * 3 + 2];
                }
                else
                {
                    out->vertexX = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexY = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexZ = attachment->vertices->values[attachment->vertexIndex];
                }
                out->vertexX = directionalScale * out->vertexX;
                out->vertexY = directionalScale * out->vertexY;
                out->vertexZ = directionalScale * out->vertexZ;
                x = directional->x;
                y = directional->y;
                z = directional->z;
                out->outX = x * transform->rotation[0] + y * transform->rotation[1] + z * transform->rotation[2];
                out->outY = x * transform->rotation[3] + y * transform->rotation[4] + z * transform->rotation[5];
                out->outZ = x * transform->rotation[6] + y * transform->rotation[7] + z * transform->rotation[8];
            }
            light++;
            out++;
        } while (light < lightEnd);
    }
}

// FUNCTION: EQGFX 0x100389df
// Copies each scene light into a record; point-light positions are made relative to the given position.
// __cdecl (the callee pops nothing; ECX is not an input), void: one 28-byte record per 0x100f2498 light. Type
// 0x1e scales the attached vertex by radius^2 * 0.05 and offsets its position by viewPosition, type 0x1f copies.
void __cdecl buildRelativeLightOutputs_guess(float *viewPosition, LightOutput_guess *lights)
{
    Light_guess **light;
    Light_guess **lightEnd;
    LightOutput_guess *out;
    LightAttachment_guess *attachment;
    PointLight_guess *point;
    DirectionalLight_guess *directional;
    float attenuation;

    light = EQGFX_LIGHT_LIST_GUESS;
    lightEnd = light + EQGFX_LIGHT_COUNT_GUESS;
    if (lightEnd > light)
    {
        out = lights;
        do
        {
            if ((*light)->type == 0x1e)
            {
                out->type = 0x1e;
                point = (PointLight_guess *)(*light);
                attachment = point->attachment;
                if (attachment->vertices->positions != 0)
                {
                    out->vertexX = attachment->vertices->positions[attachment->vertexIndex * 3];
                    out->vertexY = attachment->vertices->positions[attachment->vertexIndex * 3 + 1];
                    out->vertexZ = attachment->vertices->positions[attachment->vertexIndex * 3 + 2];
                }
                else
                {
                    out->vertexX = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexY = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexZ = attachment->vertices->values[attachment->vertexIndex];
                }
                attenuation = point->radius * point->radius * EQGFX_POINT_RADIUS_SCALE_GUESS;
                out->vertexX = attenuation * out->vertexX;
                out->vertexY = attenuation * out->vertexY;
                out->vertexZ = attenuation * out->vertexZ;
                out->outX = point->x - viewPosition[0];
                out->outY = point->y - viewPosition[1];
                out->outZ = point->z - viewPosition[2];
            }
            else if ((*light)->type == 0x1f)
            {
                out->type = 0x1f;
                directional = (DirectionalLight_guess *)(*light);
                attachment = directional->attachment;
                if (attachment->vertices->positions != 0)
                {
                    out->vertexX = attachment->vertices->positions[attachment->vertexIndex * 3];
                    out->vertexY = attachment->vertices->positions[attachment->vertexIndex * 3 + 1];
                    out->vertexZ = attachment->vertices->positions[attachment->vertexIndex * 3 + 2];
                }
                else
                {
                    out->vertexX = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexY = attachment->vertices->values[attachment->vertexIndex];
                    out->vertexZ = attachment->vertices->values[attachment->vertexIndex];
                }
                *(int *)&out->outX = *(int *)&directional->x;
                *(int *)&out->outY = *(int *)&directional->y;
                *(int *)&out->outZ = *(int *)&directional->z;
            }
            light++;
            out++;
        } while (light < lightEnd);
    }
}

// FUNCTION: EQGFX 0x10038b5f
// Copies each scene light into a record, transforming point lights by a 3x4 matrix and directions by a 3x3.
// __cdecl (the callee pops nothing; ECX is not an input), void (the original never writes EAX before RET).
// Walks the 0x100f2498 light list: a type-0x1e point light gets its attached vertex fetched and scaled by
// radius^2 * (1/scale)^2 * 0.05 and its source position transformed by the full viewMatrix; a type-0x1f
// directional light gets its source direction transformed by the 3x3 rotationMatrix only.
void __cdecl buildTransformedLightOutputs_guess(int unused, LightOutput_guess *lights, float scale, float *viewMatrix, float *rotationMatrix)
{
    Light_guess **light;
    Light_guess **lightEnd;
    Light_guess *record;
    LightOutput_guess *out;
    PointLight_guess *point;
    DirectionalLight_guess *directional;
    LightAttachment_guess *attachment;
    LightVertices_guess *vertices;
    int index;
    float invScale;
    float attenuation;
    float x;
    float y;
    float z;

    light = EQGFX_LIGHT_LIST_GUESS;
    lightEnd = light + EQGFX_LIGHT_COUNT_GUESS;
    invScale = 1.0f / scale;
    out = lights;
    for (; light < lightEnd; light++)
    {
        record = *light;
        if (record->type == 0x1e)
        {
            point = (PointLight_guess *)record;
            out->type = 0x1e;
            attachment = point->attachment;
            index = attachment->vertexIndex;
            vertices = attachment->vertices;
            if (vertices->positions != 0)
            {
                out->vertexX = vertices->positions[index * 3];
                out->vertexY = vertices->positions[index * 3 + 1];
                out->vertexZ = vertices->positions[index * 3 + 2];
            }
            else
            {
                out->vertexX = vertices->values[index];
                out->vertexY = vertices->values[index];
                out->vertexZ = vertices->values[index];
            }
            attenuation = point->radius * point->radius * invScale * invScale * EQGFX_POINT_RADIUS_SCALE_GUESS;
            out->vertexX = attenuation * out->vertexX;
            out->vertexY = attenuation * out->vertexY;
            out->vertexZ = attenuation * out->vertexZ;
            x = point->x;
            y = point->y;
            z = point->z;
            out->outX = x * viewMatrix[0] + y * viewMatrix[3] + z * viewMatrix[6] + viewMatrix[9];
            out->outY = x * viewMatrix[1] + y * viewMatrix[4] + z * viewMatrix[7] + viewMatrix[10];
            out->outZ = x * viewMatrix[2] + y * viewMatrix[5] + z * viewMatrix[8] + viewMatrix[11];
        }
        else if (record->type == 0x1f)
        {
            directional = (DirectionalLight_guess *)record;
            out->type = 0x1f;
            attachment = directional->attachment;
            index = attachment->vertexIndex;
            vertices = attachment->vertices;
            if (vertices->positions != 0)
            {
                out->vertexX = vertices->positions[index * 3];
                out->vertexY = vertices->positions[index * 3 + 1];
                out->vertexZ = vertices->positions[index * 3 + 2];
            }
            else
            {
                out->vertexX = vertices->values[index];
                out->vertexY = vertices->values[index];
                out->vertexZ = vertices->values[index];
            }
            x = directional->x;
            y = directional->y;
            z = directional->z;
            out->outX = x * rotationMatrix[0] + y * rotationMatrix[1] + z * rotationMatrix[2];
            out->outY = x * rotationMatrix[3] + y * rotationMatrix[4] + z * rotationMatrix[5];
            out->outZ = x * rotationMatrix[6] + y * rotationMatrix[7] + z * rotationMatrix[8];
        }
        out++;
    }
}

// FUNCTION: EQGFX 0x10038d6b
// Combines an object's placement and view orientation into the scaled transform for its vertices.
void __cdecl buildObjectVertexTransformMatrix_guess(ObjectTransform_guess *object, SourceMatrix_guess *transform, RenderState_guess *state, float *output)
{
    LookupContainer_guess *lookupContainer;
    LookupEntry_guess *lookupEntry;
    float factor;
    float x;
    float y;
    float z;
    float worldX;
    float worldY;
    float worldZ;

    lookupContainer = object->lookupContainer;
    lookupEntry = lookupContainer->entry;
    factor = EQGFX_MESH_SCALE_F_GUESS[lookupEntry->factorIndex];
    x = transform->basis[0][0];
    y = transform->basis[0][1];
    z = transform->basis[0][2];
    output[0] = ((z * state->xFromZ + x * state->xFromX) + y * state->xFromY) * factor;
    output[1] = ((z * state->yFromZ + x * state->yFromX) + y * state->yFromY) * factor;
    output[2] = ((x * state->zFromX + y * state->zFromY) + z * state->zFromZ) * factor;
    x = transform->basis[1][0];
    y = transform->basis[1][1];
    z = transform->basis[1][2];
    output[3] = ((z * state->xFromZ + x * state->xFromX) + y * state->xFromY) * factor;
    output[4] = ((z * state->yFromZ + x * state->yFromX) + y * state->yFromY) * factor;
    output[5] = ((x * state->zFromX + y * state->zFromY) + z * state->zFromZ) * factor;
    x = transform->basis[2][0];
    y = transform->basis[2][1];
    z = transform->basis[2][2];
    output[6] = ((z * state->xFromZ + x * state->xFromX) + y * state->xFromY) * factor;
    output[7] = ((z * state->yFromZ + x * state->yFromX) + y * state->yFromY) * factor;
    output[8] = ((x * state->zFromX + y * state->zFromY) + z * state->zFromZ) * factor;
    worldX = object->z * transform->basis[2][0] + transform->basis[0][0] * object->x + object->y * transform->basis[1][0] + transform->translation[0] + state->translation[0];
    worldY = (object->y * transform->basis[1][1] + object->z * transform->basis[2][1]) + object->x * transform->basis[0][1] + transform->translation[1] + state->translation[1];
    worldZ = (object->z * transform->basis[2][2] + object->y * transform->basis[1][2]) + object->x * transform->basis[0][2] + transform->translation[2] + state->translation[2];
    output[9] = worldZ * state->xFromZ + worldX * state->xFromX + worldY * state->xFromY;
    output[10] = worldZ * state->yFromZ + worldX * state->yFromX + worldY * state->yFromY;
    output[11] = worldX * state->zFromX + worldY * state->zFromY + worldZ * state->zFromZ;
    return;
}
