// unit_10038FAF.c - eqgfx_dx7.dll, the address run 0x10038FAF..0x1003FD33 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include "eqgfx/layout/t3d_model_matrix.h"
#include <string.h>

typedef struct RendererScrollMaterial_guess RendererScrollMaterial_guess;
typedef struct RendererMaterialAnimation_guess RendererMaterialAnimation_guess;
typedef struct RendererAnimationFrames_guess RendererAnimationFrames_guess;

typedef struct RendererScrollList_guess {
    unsigned char pad0000[0x14];
    unsigned int count;
    RendererScrollMaterial_guess **materials;
    unsigned char pad001C[4];
    int dirty;
    unsigned int lastClock;
} RendererScrollList_guess;

typedef struct RendererClockOwner_guess {
    unsigned char pad0000[0x184];
    unsigned int clock;
    unsigned char pad0188[0x258];
    unsigned int *flagMap;
} RendererClockOwner_guess;

struct RendererScrollMaterial_guess {
    unsigned char pad0000[0x14];
    unsigned int flags;
    unsigned char pad0018[0xc];
    RendererMaterialAnimation_guess *animation;
    unsigned char pad0028[8];
    float scrollU;
    float scrollV;
    float offsetU;
    float offsetV;
};

struct RendererMaterialAnimation_guess {
    unsigned char pad0000[0xc];
    unsigned char control;
    unsigned char pad000D[7];
    int frameValue;
    unsigned int frameIndex;
    unsigned int interval;
    unsigned int nextClock;
    int mode;
    RendererAnimationFrames_guess *frameDef;
};

struct RendererAnimationFrames_guess {
    unsigned char pad0000[0x14];
    unsigned int count;
    int *frames;
};

typedef struct RendererBufferState_guess {
    unsigned char pad0000[0x1ac];
    float viewOffset[3];
    unsigned char pad01B8[0x74];
    float viewM20;
    unsigned char pad0230[0x8];
    float viewM21;
    unsigned char pad023C[0x8];
    float viewM22;
    float viewM00;
    float viewM01;
    float viewM02;
    float viewM10;
    float viewM11;
    float viewM12;
    unsigned char pad0260[0x30];
    unsigned int submittedPrimitiveCount;
    unsigned char pad0294[0x1c];
    float faceNormal[3];
    unsigned int primitiveCapacity;
    void *bufferAt2C0;
    float *primitiveVertices;
    void *bufferAt2C8;
    void *bufferAt2CC;
    unsigned char pad02D0[0x74];
    float planeNormal[3];
    unsigned char *vertexPositions;
    unsigned char *vertexNormals;
    unsigned char pad0358[0x80];
    struct RendererDrawRequest_guess *drawRequest;
    unsigned char pad03DC[0x4];
    unsigned int *indexRemap;
    unsigned char pad03E4[0x1c];
    unsigned int vertexClipFlags;
    unsigned char pad0404[0x18];
    unsigned int savedPrimitiveState;
    unsigned char pad0420[0x1e8];
    unsigned int primaryCapacity;
    unsigned int secondaryCapacity;
    void *buffer12A;
    void *buffer12B;
    void *bufferSecondary;
    void *bufferBytesA;
    void *bufferBytesB;
    void *buffer4A;
    void *buffer4B;
} RendererBufferState_guess;

typedef struct RendererBufferRequest_guess {
    unsigned char pad0000[0x14];
    unsigned int *requestedCounts;
    unsigned short *fallbackCounts;
} RendererBufferRequest_guess;

typedef struct RendererMaterialEntry_guess {
    unsigned char pad0000[0x14];
    unsigned int flags;
    unsigned int renderWord18;
    unsigned int renderWord1C;
    unsigned int diffuseValue;
    unsigned int textureValue;
    unsigned int renderWord28;
    unsigned char renderByte2C;
    unsigned char pad002D[0xb];
    float textureUOffset;
    float textureVOffset;
} RendererMaterialEntry_guess;

typedef struct RendererTextureUse_guess {
    unsigned char pad0000[0x64];
    unsigned int opaqueNext;
    unsigned int translucentNext;
    unsigned int maskedNext;
} RendererTextureUse_guess;

typedef struct RendererMaterialTextureBinding_guess {
    unsigned char pad0000[0x14];
    RendererTextureUse_guess **textureUse;
} RendererMaterialTextureBinding_guess;

typedef struct RendererTextureDefinition_guess {
    unsigned char pad0000[0x58];
    unsigned int textureU;
    unsigned int textureV;
} RendererTextureDefinition_guess;

typedef struct RendererMaterialTable_guess {
    unsigned char pad0000[0x0c];
    float *textureCoordinates;
    unsigned char pad0010[0x0c];
    unsigned char *colorValues;
    unsigned int stripRecordCount;
    unsigned char *stripRecords;
    unsigned char pad0028[0x24];
    unsigned int stripCount;
    unsigned short *stripIndices;
} RendererMaterialTable_guess;

typedef struct RendererBatchContext_guess {
    unsigned char pad0000[0x198];
    float baseX;
    float baseY;
    unsigned int tileB;
    unsigned int tileA;
    unsigned char pad01A8[0x238];
    unsigned int *indexRemap;
    unsigned char pad03E4[0x13c];
    float offsetX;
    float offsetY;
    unsigned char pad0528[0xe8];
    float *positions;
    unsigned int *tint;
    unsigned char pad0618[4];
    char *active;
    unsigned char pad0620[4];
    int *remap;
} RendererBatchContext_guess;

typedef struct RendererBatchMaterialList_guess {
    unsigned char pad0000[0x18];
    RendererMaterialEntry_guess **materialEntries;
} RendererBatchMaterialList_guess;

typedef struct RendererBatchInner_guess RendererBatchInner_guess;

typedef struct RendererBatchGroup_guess {
    unsigned char pad0000[0x14];
    RendererBatchInner_guess *inner;
} RendererBatchGroup_guess;

typedef struct RendererBatch_guess {
    unsigned char pad0000[0x20];
    RendererBatchMaterialList_guess *materialList;
    unsigned char pad0024[0x10];
    RendererBatchGroup_guess *group;
} RendererBatch_guess;

typedef struct RendererBatchRecord_guess {
    unsigned short start;
    unsigned short count;
    unsigned short materialIndex;
    unsigned short pad0006;
} RendererBatchRecord_guess;

struct RendererBatchInner_guess {
    unsigned char pad0000[0x0c];
    unsigned char *shape;
    unsigned char pad0010[0x44];
    unsigned int count;
    RendererBatchRecord_guess *records;
};

typedef struct RendererPrimitiveState_guess {
    unsigned char pad0000[0x14];
    RendererMaterialTable_guess *materials;
} RendererPrimitiveState_guess;

typedef struct RendererColorOverride_guess {
    unsigned char pad0000[0x18];
    unsigned int colorIndex;
    unsigned char pad001C[0x18];
    RendererMaterialTable_guess *materials;
} RendererColorOverride_guess;

typedef struct RendererMeshState_guess {
    unsigned char pad0000[0x18];
    RendererMaterialEntry_guess **materialEntries;
} RendererMeshState_guess;

typedef struct RendererDrawRequest_guess {
    unsigned char pad0000[0x20];
    RendererMeshState_guess *meshState;
    unsigned char pad0024[0x4];
    RendererColorOverride_guess *colorOverride;
    unsigned char pad002C[0x8];
    RendererPrimitiveState_guess *primitiveState;
} RendererDrawRequest_guess;

typedef struct RendererPrimitiveDescription_guess {
    unsigned int words[6];
    unsigned char renderState;
    unsigned char pad0019[3];
    unsigned int textureValue;
    unsigned char pad0020[0x28];
    unsigned int renderWord48;
    unsigned int diffuseValue;
} RendererPrimitiveDescription_guess;

typedef struct RendererStripBatch_guess {
    unsigned char pad0000[0x0c];
    unsigned short stripCount;
    unsigned char pad000e[6];
    unsigned char *textureVertices;
    unsigned char *stripRecords;
    unsigned char pad001c[4];
    unsigned short *stripGroups;
} RendererStripBatch_guess;

typedef struct RendererStripRequest_guess {
    unsigned char pad0000[0x18];
    RendererStripBatch_guess *stripBatch;
} RendererStripRequest_guess;

typedef struct RendererStripMaterialLookup_guess {
    unsigned char pad0000[0x18];
    RendererMaterialEntry_guess **materialEntries;
} RendererStripMaterialLookup_guess;

typedef struct RendererStripPrimitiveDescription_guess {
    unsigned int primitiveType;
    unsigned char pad0004[0x10];
    unsigned char vertexState;
    unsigned char pad0015[3];
    unsigned int materialTexture;
    unsigned char pad001c[0x2c];
    unsigned int materialDiffuse;
} RendererStripPrimitiveDescription_guess;

typedef struct RendererStripRecord_guess {
    unsigned char pad0000[0x4];
    unsigned int materialIndex;
    unsigned char flags;
    unsigned char pad0009;
    unsigned short vertexB;
    unsigned short vertexA;
    unsigned short vertexC;
} RendererStripRecord_guess;

#define prepareRendererPrimitive_guess EQ_CDECL(void, 0x1003CF06, (int, int))
#define freeRenderBuffersByAddress_guess EQ_CDECL(void, 0x1002FD99, (int))
#define allocateRendererBuffers_guess EQ_CDECL(int, 0x1002FCF2, (int, int))
#define submitRendererPrimitive_guess EQ_CDECL(int, 0x100477B9, (int, int, int, void *, unsigned int))
#define RENDER_INDEX_REMAP_FLAG 0x80000000U
#define RENDER_STRIP_RECORD_BYTES 0x10
#define RENDER_VERTEX_WORDS 3
#define RENDER_REVERSED_VERTEX_MARKER 1

#define t3dReallocateMemory_guess EQ_CDECL(void *, 0x10050C8C, (void *, unsigned int))
#define EQGFX_T3D_FLOAT_SQRT EQ_CDECL(float, 0x1004EF68, (float))
#define RENDER_BUFFER_ALLOCATION_ERROR ((int)0xffff3d3e)

#define EQGFX_TRANSFORM_VERTICES_BY_MATRIX_GUESS EQ_CDECL(void, 0x100383EC, (short *, float *, float *, int, char *))
#define transformPackedVerticesAf14_guess EQ_CDECL(void, 0x10038527, (short *, float *, float *, int, char *))

typedef struct SpriteLightingGeometry_guess {
    unsigned short vertexCount;
    unsigned char pad0002[4];
    unsigned short vertexColorFlags;
    unsigned char pad0008[0x0a];
    unsigned short scaleIndex;
    short *vertices;
    unsigned short pad0018[2];
    unsigned short *spans;
    unsigned char pad0020[0x0c];
    unsigned char lightingParameters[1];
} SpriteLightingGeometry_guess;

typedef struct SpriteLightingVertex_guess {
    short positionX;
    short positionY;
    short positionZ;
    unsigned char pad0006[8];
    short packedNormal;
    unsigned char colorX;
    unsigned char colorY;
    unsigned char colorZ;
    unsigned char colorIntensity;
} SpriteLightingVertex_guess;

typedef struct SpriteLightingVertexSource_guess {
    unsigned char pad0000[0x0e];
    unsigned char flags;
    unsigned char pad000f[9];
    SpriteLightingGeometry_guess *geometry;
} SpriteLightingVertexSource_guess;

typedef struct SpriteLightingResource_guess {
    unsigned char pad0000[0x0e];
    unsigned char lightingFlags;
    unsigned char pad000f[9];
    SpriteLightingGeometry_guess *geometry;
} SpriteLightingResource_guess;

typedef struct SpriteLightingObject_guess {
    unsigned char pad0000[0x18];
    float positionX;
    float positionY;
    float positionZ;
    void *alternateMesh;
    void *attachment;
    unsigned char pad002c[4];
    unsigned char colorX;
    unsigned char colorY;
    unsigned char colorZ;
    unsigned char pad0033;
    SpriteLightingResource_guess *resource;
} SpriteLightingObject_guess;

typedef struct SpriteLightingScale_guess {
    unsigned char pad0000[0x34];
    float divisor;
} SpriteLightingScale_guess;

typedef struct SpriteLightingAlternateGeometry_guess {
    unsigned short vertexCount;
    unsigned char pad0002[0x0a];
    short *vertices;
} SpriteLightingAlternateGeometry_guess;

typedef struct SpriteLightingAlternateResource_guess {
    unsigned char pad0000[0x18];
    SpriteLightingAlternateGeometry_guess *geometry;
} SpriteLightingAlternateResource_guess;

typedef struct SpriteLightingAlternateObject_guess {
    unsigned char pad0000[0x18];
    int groupIndex;
    unsigned char pad001c[0x18];
    SpriteLightingAlternateResource_guess *resource;
} SpriteLightingAlternateObject_guess;

typedef struct SpriteLightingMatrixContext_guess {
    unsigned char pad0000[0x24];
    float *matrixRows;
    unsigned char pad0028[0x280];
} SpriteLightingMatrixContext_guess;

typedef struct SpriteLightingOutputContext_guess {
    unsigned char pad0000[0x29c];
    float ambientX;
    float ambientY;
    float ambientZ;
    unsigned char pad02a8[0x36c];
    float *output;
    unsigned char pad0618[4];
    char *flags;
} SpriteLightingOutputContext_guess;

typedef struct SpriteLightingActorTint_guess {
    unsigned char pad0000[0x360];
    unsigned char color0;
    unsigned char color1;
    unsigned char color2;
    unsigned char pad0363;
    unsigned char color3;
    unsigned char color4;
    unsigned char color5;
} SpriteLightingActorTint_guess;

typedef struct SpriteLightingAttachmentState_guess {
    unsigned char pad0000[0x0e];
    unsigned char flags;
    unsigned char pad000f[0x0d];
    unsigned char **colorRows;
} SpriteLightingAttachmentState_guess;

typedef struct SpriteLightingAttachment_guess {
    unsigned char pad0000[0x18];
    int colorRowIndex;
    unsigned char pad001c[0x18];
    SpriteLightingAttachmentState_guess *state;
} SpriteLightingAttachment_guess;

typedef struct SpriteLightingEffect_guess {
    float type;
    float pointX;
    float pointY;
    float pointZ;
    float addX;
    float addY;
    float addZ;
} SpriteLightingEffect_guess;

typedef float (__cdecl *spriteLightingSqrt_t_guess)(float);
#define g_packedNormalTable_guess EQ_ARRAY(float, 0x100FB468)
#define g_spriteLightingSqrt_guess EQ_GLOBAL(spriteLightingSqrt_t_guess, 0x100FC77C)
#define buildSpriteLightingPlanes_guess EQ_CDECL(void, 0x10038771, (void *, void *, float, float, float *))
#define prepareSpriteLightingEffects_guess EQ_CDECL(void, 0x100389DF, (void *, void *))

#define g_scaleTable_guess EQ_ARRAY(float, 0x100FB868)
#define g_spriteLightingEffects_guess EQ_ARRAY(SpriteLightingEffect_guess, 0x100F2698)
#define updateSpriteLightingEffects_guess EQ_CDECL(void, 0x10038B5F, (void *, void *, float, int, int))
#define SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG 4.34403e-44f
#define SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG_BITS 0x1fU

#define g_effectCount_guess EQ_GLOBAL(unsigned int, 0x100F2D98)
#define g_effectScores_guess EQ_ARRAY(float, 0x100F2598)
#define g_effectEntries_guess EQ_ARRAY(int, 0x100F2498)
#define EQGFX_POINT_RADIUS_SCALE_GUESS EQ_GLOBAL(float, 0x100762F0)
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280)
#define c_fullStrength_guess EQ_GLOBAL(float, 0x10076318)
#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define c_colorBias_guess EQ_GLOBAL(float, 0x1007631C)
#define c_anchorLift_guess EQ_GLOBAL(float, 0x10076320)
#define EQGFX_MOTION_HALF_GUESS EQ_GLOBAL(float, 0x10076290)
#define EQGFX_JITTER_SCALE_GUESS EQ_GLOBAL(float, 0x100762E8)
#define g_frameFractions_guess EQ_ARRAY(float, 0x100FA468)
#define testSphereInViewVolume_guess EQ_CDECL(int, 0x1003FD33, (ClipContext_guess *, float, float, float, float, float))
#define g_opaqueEntries_guess EQ_ARRAY(unsigned int, 0x10332E40)
#define g_opaqueIndices_guess EQ_ARRAY(unsigned short, 0x1083C040)
#define g_opaqueEntryCount_guess EQ_GLOBAL(unsigned int, 0x100FC788)
#define g_opaqueIndexCount_guess EQ_GLOBAL(unsigned int, 0x1025C0A0)
#define g_opaqueHead_guess EQ_GLOBAL(unsigned int, 0x100FBBBC)
#define g_transEntries_guess EQ_ARRAY(unsigned int, 0x106BF220)
#define g_transIndices_guess EQ_ARRAY(unsigned short, 0x1054C060)
#define g_transEntryCount_guess EQ_GLOBAL(unsigned int, 0x100FC790)
#define g_transIndexCount_guess EQ_GLOBAL(unsigned int, 0x107037E0)
#define g_transHead_guess EQ_GLOBAL(unsigned int, 0x100FBBC0)
#define markTextureAsUsedInEffects_guess EQ_CDECL(void, 0x1004609A, (int))
#define transformVerticesByMatrix_guess EQ_CDECL(void, 0x100383EC, (int *, int, float *, int, int))
#define g_opaqueVertBuf_guess EQ_ARRAY(unsigned int, 0x100FC7A0)
#define g_opaqueVertCount_guess EQ_GLOBAL(unsigned int, 0x10377400)
#define g_transVertBuf_guess EQ_ARRAY(unsigned int, 0x1030BD40)
#define g_transVertCount_guess EQ_GLOBAL(unsigned int, 0x1054C040)
#define g_primCount_guess EQ_GLOBAL(unsigned int, 0x100FBBC8)
typedef void (__stdcall *grDrawLine_t_guess)(int, int);
#define grDrawLine_guess EQ_GLOBAL(grDrawLine_t_guess, 0x100F3ED4)
#define g_emitOpaquePool_guess EQ_ARRAY(unsigned char, 0x101238A0)
#define g_emitOpaqueUsed_guess EQ_GLOBAL(unsigned int, 0x1079FC00)
#define g_emitTransPool_guess EQ_ARRAY(unsigned char, 0x1055F900)
#define g_emitTransUsed_guess EQ_GLOBAL(unsigned int, 0x104AFC20)
typedef int (__cdecl *emitPlain_t_guess)(int, char *, float *, unsigned int *, void *, void *, int);
#define g_emitPlain_guess EQ_GLOBAL(emitPlain_t_guess, 0x100FC75C)
typedef int (__cdecl *emitTextured_t_guess)(int, char *, float *, char *, unsigned int *, void *, void *, int);
#define g_emitTextured_guess EQ_GLOBAL(emitTextured_t_guess, 0x100FC774)
#define g_xformU3c2_guess EQ_GLOBAL(unsigned int, 0x100FBB98)
#define g_xformV3c2_guess EQ_GLOBAL(unsigned int, 0x100FBB94)
#define g_renderMode3c2_guess EQ_GLOBAL(unsigned int, 0x100FBBA4)
typedef int (__cdecl *emitPlainUsed_t_guess)(int, unsigned int, char *, float *, unsigned int *, void *, void *, int);
#define g_emitPlainStd3c2_guess EQ_GLOBAL(emitPlainUsed_t_guess, 0x100FC764)
#define g_emitPlainAlt3c2_guess EQ_GLOBAL(emitPlainUsed_t_guess, 0x100FC760)
typedef int (__cdecl *emitTexturedUsed_t_guess)(int, unsigned int, char *, float *, char *, unsigned int *, void *, void *, int);
#define g_emitTexturedStd3c2_guess EQ_GLOBAL(emitTexturedUsed_t_guess, 0x100FC750)
#define g_emitTexturedAlt3c2_guess EQ_GLOBAL(emitTexturedUsed_t_guess, 0x100FC754)
#define g_byteFloatTable_guess EQ_ARRAY(float, 0x100FA068)
#define g_renderDisable_guess EQ_GLOBAL(int, 0x100A16B4)
typedef void (__cdecl *fxEmitA51c_t_guess)(float *, float *, unsigned int, unsigned int, void *, unsigned int, void *);
#define g_fxEmitA51c_guess EQ_GLOBAL(fxEmitA51c_t_guess, 0x100FC770)
typedef void (__cdecl *fxEmitB51c_t_guess)(float *, float *, unsigned int, int, void *, unsigned int, void *);
#define g_fxEmitB51c_guess EQ_GLOBAL(fxEmitB51c_t_guess, 0x100FC74C)
typedef void (__cdecl *fxEmitC51c_t_guess)(float *, float *, unsigned int, void *, unsigned int, void *);
#define g_fxEmitC51c_guess EQ_GLOBAL(fxEmitC51c_t_guess, 0x100FC768)
typedef struct FxVec51c_guess
{
    float x; // 0x0
    float y; // 0x4
    float z; // 0x8
} FxVec51c_guess;
/* FxLink51c_guess: the actor layout is FxLink51c_guess (generated: eqgfx/layout/t3d_anim_entry.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3d_anim_entry.h"
typedef struct FxHolder51c_guess
{
    unsigned char pad00[0x18]; // 0x0
    float *samples; // 0x18
    float *triples; // 0x1c
} FxHolder51c_guess;
typedef struct FxEntry51c_guess
{
    int kind; // 0x0
    unsigned char pad04[0xb]; // 0x4
    unsigned char flags; // 0xf
    int refA; // 0x10
    float fxA; // 0x14
    float fyA; // 0x18
    float fzA; // 0x1c
    float rad; // 0x20
} FxEntry51c_guess;

typedef struct NodeA234_guess
{
    unsigned char pad00[0x28]; // 0x0
    int sibling; // 0x28
    unsigned char pad2c[4]; // 0x2c
    unsigned char c0; // 0x30
    unsigned char c1; // 0x31
    unsigned char c2; // 0x32
    unsigned char pad33[1]; // 0x33
    int host; // 0x34
} NodeA234_guess;

typedef struct Ctx234_guess
{
    unsigned char pad00[0x29c]; // 0x0
    float vecX; // 0x29c
    float vecY; // 0x2a0
    float vecZ; // 0x2a4
    unsigned char pad2a8[0x36c]; // 0x2a8
    float *out; // 0x614
    unsigned char pad618[4]; // 0x618
    char *active; // 0x61c
} Ctx234_guess;

typedef struct Pal234_guess
{
    unsigned char pad00[0x360]; // 0x0
    unsigned char v0; // 0x360
    unsigned char v1; // 0x361
    unsigned char v2; // 0x362
    unsigned char pad363[1]; // 0x363
    unsigned char v3; // 0x364
    unsigned char v4; // 0x365
    unsigned char v5; // 0x366
} Pal234_guess;

typedef struct Host234_guess
{
    unsigned char pad00[0x0e]; // 0x0
    unsigned char flags; // 0x0e
    unsigned char pad0f[5]; // 0x0f
    int *list; // 0x14
} Host234_guess;

typedef struct List234_guess
{
    int count; // 0x0
    unsigned char pad04[0x14]; // 0x4
    int flag; // 0x18
    unsigned char *codes; // 0x1c
} List234_guess;

typedef struct Sib234_guess
{
    unsigned char pad00[0x18]; // 0x0
    int index; // 0x18
    unsigned char pad1c[0x18]; // 0x1c
    int *node; // 0x34
} Sib234_guess;

typedef struct NodeG234_guess
{
    unsigned char pad00[0x0e]; // 0x0
    unsigned char flags; // 0x0e
    unsigned char pad0f[0x0d]; // 0x0f
    int *rows; // 0x1c
} NodeG234_guess;

typedef struct EmitXform196_guess
{
    unsigned int alpha; // 0x0 (local_50)
    unsigned int texU; // 0x4 (local_4c)
    unsigned int texV; // 0x8 (local_48)
    unsigned int scrollU; // 0xc (local_44)
    unsigned int scrollV; // 0x10 (local_40)
    float baseX; // 0x14 (local_3c)
    float baseY; // 0x18 (local_38)
    float sumX; // 0x1c (local_34)
    float sumY; // 0x20 (local_30)
    unsigned int tileA; // 0x24 (local_2c)
    unsigned int tileB; // 0x28 (local_28)
} EmitXform196_guess;

typedef struct SpriteAnchor_guess
{
    unsigned char pad0000[0x24]; // 0x0
    float positionX; // 0x24
    float positionY; // 0x28
    float positionZ; // 0x2c
} SpriteAnchor_guess;

typedef struct SpriteFrames_guess
{
    unsigned char pad0000[0x14]; // 0x0
    int firstFrame; // 0x14
    unsigned char pad0018[4]; // 0x18
    int heightIndex; // 0x1c
    int *frameIndices; // 0x20
    char *uvTableA; // 0x24
    char *uvTableB; // 0x28
    unsigned int *texturePtr; // 0x2c
} SpriteFrames_guess;

typedef struct SpriteDesc_guess
{
    unsigned char pad0000[0x14]; // 0x0
    SpriteFrames_guess *frames; // 0x14
    unsigned char *frameSequence; // 0x18
    unsigned char pad001c[4]; // 0x1c
    unsigned int renderFlags; // 0x20
    unsigned char pad0024[4]; // 0x24
    float nearScale; // 0x28
    float scaleDistance; // 0x2c
    int useMaxSize; // 0x30
    float maxSize; // 0x34
    float sizeFactor; // 0x38
    int originXIndex; // 0x3c
    int originYIndex; // 0x40
    float radiusFactor; // 0x44
    unsigned char blueScale; // 0x48
    unsigned char greenScale; // 0x49
    unsigned char redScale; // 0x4a
} SpriteDesc_guess;

/* SpriteVertex_guess: the actor layout is SpriteVertex_guess (generated: eqgfx/layout/s3d_primitive.h); the old local view's members map by offset. */
#include "eqgfx/layout/s3d_primitive.h"

typedef struct ClipContext_guess
{
    unsigned char pad00[0x198]; // 0x0
    float baseX; // 0x198
    float baseY; // 0x19c
    float nearLimit; // 0x1a0
    float depthScale; // 0x1a4
    float farLimit; // 0x1a8
    float originX; // 0x1ac
    float originY; // 0x1b0
    float originZ; // 0x1b4
    unsigned char pad1b8[0x6c]; // 0x1b8
    float viewMatrix[9]; // 0x224, row-major 3x3
    unsigned char pad248[0x54]; // 0x248
    float ambientX; // 0x29c
    float ambientY; // 0x2a0
    float ambientZ; // 0x2a4
    unsigned char pad2a8[0x1c]; // 0x2a8
    SpriteVertex_guess *vertexOutput; // 0x2c4
    unsigned char pad2c8[0x110]; // 0x2c8
    SpriteDesc_guess *sprite; // 0x3d8
    unsigned char pad3dc[4]; // 0x3dc
    unsigned int *flagMap; // 0x3e0
    unsigned char pad3e4[0x1c]; // 0x3e4
    unsigned int planeMask; // 0x400
    float zFactorY; // 0x404
    float zFactorX; // 0x408
    float radiusFactorY; // 0x40c
    float radiusFactorX; // 0x410
    unsigned char pad414[0x10c]; // 0x414
    float offsetX; // 0x520
    float offsetY; // 0x524
    float viewScaleX; // 0x528
    float viewScaleY; // 0x52c
    unsigned char pad530[0xe0]; // 0x530
    float *positions; // 0x610
    unsigned int *tint; // 0x614
    unsigned char pad618[4]; // 0x618
    char *active; // 0x61c
    unsigned char *codes; // 0x620
    unsigned char pad624[4]; // 0x624
    int *remap; // 0x628
} ClipContext_guess;

typedef struct VertCountRef_guess
{
    unsigned char pad00[0x18]; // 0x0
    unsigned short *countPtr; // 0x18
} VertCountRef_guess;

typedef struct PhysEffectEntry_guess
{
    int type; // 0x0
    unsigned char pad04[8]; // 0x4
    unsigned int flags; // 0xc
    unsigned char pad10[4]; // 0x10
    float x; // 0x14
    float y; // 0x18
    float z; // 0x1c
    float radius; // 0x20
} PhysEffectEntry_guess;

typedef struct PhysEffectQueue_guess
{
    unsigned char pad00[0x2a8]; // 0x0
    unsigned int count; // 0x2a8
    PhysEffectEntry_guess **entries; // 0x2ac
} PhysEffectQueue_guess;

typedef struct Basis1af_guess
{
    unsigned char pad00[0x5c]; // 0x0
    float f5c; // 0x5c
    float f60; // 0x60
    float f64; // 0x64
    float f68; // 0x68
    float f6c; // 0x6c
    float f70; // 0x70
    float f74; // 0x74
    float f78; // 0x78
    float f7c; // 0x7c
} Basis1af_guess;

typedef struct BasisAce0_guess
{
    unsigned char pad00[0x8c]; // 0x0
    float f8c; // 0x8c
    float f90; // 0x90
    float f94; // 0x94
    float f98; // 0x98
    float f9c; // 0x9c
    float fa0; // 0xa0
    float fa4; // 0xa4
    float fa8; // 0xa8
    float fac; // 0xac
} BasisAce0_guess;

typedef struct Frame1af_guess
{
    unsigned char pad00[0x1ac]; // 0x0
    float t1ac; // 0x1ac
    float t1b0; // 0x1b0
    float t1b4; // 0x1b4
    unsigned char pad1b8[0x74]; // 0x1b8
    float t22c; // 0x22c
    unsigned char pad230[0x8]; // 0x230
    float t238; // 0x238
    unsigned char pad23c[0x8]; // 0x23c
    float t244; // 0x244
    float t248; // 0x248
    float t24c; // 0x24c
    float t250; // 0x250
    float t254; // 0x254
    float t258; // 0x258
    float t25c; // 0x25c
    unsigned char pad260[0x3b0]; // 0x260
    int t610; // 0x610
    unsigned char pad614[0x8]; // 0x614
    int t61c; // 0x61c
} Frame1af_guess;

typedef struct Outer1af_guess
{
    unsigned char pad00[0x34]; // 0x0
    int middle; // 0x34
} Outer1af_guess;

typedef struct Slot1af_guess
{
    unsigned short count; // 0x0
    unsigned char pad02[0x10]; // 0x2
    unsigned short scaleIndex; // 0x12
    int *verts; // 0x14
} Slot1af_guess;

typedef struct Middle1af_guess
{
    unsigned char pad00[0x14]; // 0x0
    int *slot; // 0x14
    Slot1af_guess *vertSlot; // 0x18
} Middle1af_guess;

typedef struct MeshCd0d_guess
{
    unsigned short vertCount; // 0x0
    unsigned char pad02[6]; // 0x2
    unsigned short faceCount; // 0x8
    unsigned char pad0a[14]; // 0xa
    unsigned char *need; // 0x18
    unsigned char pad1c[16]; // 0x1c
    float centerX; // 0x2c
    float centerY; // 0x30
    float centerZ; // 0x34
    unsigned char pad38[60]; // 0x38
    float *planes; // 0x74
} MeshCd0d_guess;

typedef struct FrameCd0d_guess
{
    unsigned char pad00[0x1ac]; // 0x0
    float posX; // 0x1ac
    float posY; // 0x1b0
    float posZ; // 0x1b4
    unsigned char pad1b8[0x460]; // 0x1b8
    unsigned char *faceCodes; // 0x618
    unsigned char *vertMarks; // 0x61c
} FrameCd0d_guess;

typedef struct OwnerCd0d_guess
{
    unsigned char pad00[0x18]; // 0x0
    MeshCd0d_guess *mesh; // 0x18
} OwnerCd0d_guess;

typedef struct MeshCc23_guess
{
    unsigned int vertMarkSize; // 0x0
    unsigned char pad04[0x1c]; // 0x4
    unsigned int entryCount; // 0x20
    unsigned char *entries; // 0x24
    unsigned char pad28[4]; // 0x28
    float *planes; // 0x2c
} MeshCc23_guess;

typedef struct ProjectXform_guess
{
    unsigned int alpha;
    float fogScale;
    float fogStart;
    float uOffset;
    float vOffset;
    float scaleX;
    float scaleY;
    float centerX;
    float centerY;
    float depthScale;
    float depthNear;
} ProjectXform_guess;

typedef struct ScreenVert_guess
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned int color;
    unsigned int specular;
    float tu;
    float tv;
} ScreenVert_guess;

typedef struct MatEntry_guess
{
    unsigned char pad00[0x14]; // 0x0
    unsigned int flags; // 0x14
    unsigned char pad18[0x20]; // 0x18
    unsigned int scrollU; // 0x38
    unsigned int scrollV; // 0x3c
} MatEntry_guess;

typedef struct Run_guess
{
    unsigned short vertCount; // 0x0
    unsigned short matIndex; // 0x2
} Run_guess;

typedef struct RunGroup_guess
{
    unsigned char pad00[0xe]; // 0x0
    unsigned short runCount; // 0xe
    unsigned char pad10[4]; // 0x10
    char *shape; // 0x14
    unsigned char pad18[0xc]; // 0x18
    Run_guess *runs; // 0x24
} RunGroup_guess;

typedef struct Batch_guess
{
    unsigned char pad00[0x18]; // 0x0
    RunGroup_guess *group; // 0x18
} Batch_guess;

typedef struct MatList_guess
{
    unsigned char pad00[0x18]; // 0x0
    MatEntry_guess **entries; // 0x18
} MatList_guess;

typedef void (__cdecl *Draw1af_t_guess)(int, int, float *, int, int);
#define g_draw1af_guess EQ_GLOBAL(Draw1af_t_guess, 0x100FC780)

// FUNCTION: EQGFX 0x10038faf
// Places a model point in the scene using its current pose.
// __cdecl, four stack args (the third is unused). Copies the 9 floats at
// matrix+0x5c..0x7c to out[0..8], then transforms the point at
// vert+0x14/0x18/0x1c by the 3x4 columns at matrix+0x5c..0x88 into out[9..11].
void __cdecl buildMatrixAndTransformPoint_guess(int vert, int matrix, int unused, float *out)
{
    (void)unused;
    out[0] = ((T3dModelMatrix_guess*)matrix)->f5c;
    ((int *)out)[1] = ((T3dModelMatrix_guess*)matrix)->position_bits_guess[0];
    ((int *)out)[2] = ((T3dModelMatrix_guess*)matrix)->position_bits_guess[1];
    ((int *)out)[3] = ((T3dModelMatrix_guess*)matrix)->position_bits_guess[2];
    ((int *)out)[4] = ((T3dModelMatrix_guess*)matrix)->f6c_bits_guess;
    ((int *)out)[5] = ((T3dModelMatrix_guess*)matrix)->f70_bits_guess;
    ((int *)out)[6] = ((T3dModelMatrix_guess*)matrix)->f74_bits_guess;
    ((int *)out)[7] = ((T3dModelMatrix_guess*)matrix)->f78_bits_guess;
    ((int *)out)[8] = ((T3dModelMatrix_guess*)matrix)->f7c_bits_guess;
    out[10] = ((T3dModelMatrix_guess*)matrix)->f6c * *(float *)(vert + 0x18) +
              ((T3dModelMatrix_guess*)matrix)->f78 * *(float *)(vert + 0x1c) +
              *(float *)(vert + 0x14) * ((T3dModelMatrix_guess*)matrix)->position[0] +
              ((T3dModelMatrix_guess*)matrix)->translationY_guess;
    out[11] = ((T3dModelMatrix_guess*)matrix)->f7c * *(float *)(vert + 0x1c) +
              ((T3dModelMatrix_guess*)matrix)->position[1] * *(float *)(vert + 0x14) +
              ((T3dModelMatrix_guess*)matrix)->f70 * *(float *)(vert + 0x18) +
              ((T3dModelMatrix_guess*)matrix)->translationZ_guess;
    out[9] = ((T3dModelMatrix_guess*)matrix)->position[2] * *(float *)(vert + 0x18) +
             ((T3dModelMatrix_guess*)matrix)->f74 * *(float *)(vert + 0x1c) +
             ((T3dModelMatrix_guess*)matrix)->f5c * *(float *)(vert + 0x14) +
             ((T3dModelMatrix_guess*)matrix)->translationX_guess;
}

// FUNCTION: EQGFX 0x10039055
// Places a model point in the scene with its size setting applied to the transform.
// __cdecl, four stack args (the third is unused). Scales the 9 floats at
// matrix+0x5c..0x7c by the table entry at 0x100fb868 indexed through
// vert+0x34 -> +0x18 -> +0x12 into out[0..8], then transforms the point at
// vert+0x14/0x18/0x1c by the 3x4 columns at matrix+0x5c..0x88 into out[9..11].
void __cdecl buildScaledMatrixAndTransformPoint_guess(int vert, int matrix, int unused, float *out)
{
    float scale;
    (void)unused;
    scale = g_scaleTable_guess[*(unsigned short *)(*(int *)(*(int *)(vert + 0x34) + 0x18) + 0x12)];
    out[0] = ((T3dModelMatrix_guess*)matrix)->f5c * scale;
    out[1] = ((T3dModelMatrix_guess*)matrix)->position[0] * scale;
    out[2] = ((T3dModelMatrix_guess*)matrix)->position[1] * scale;
    out[3] = ((T3dModelMatrix_guess*)matrix)->position[2] * scale;
    out[4] = ((T3dModelMatrix_guess*)matrix)->f6c * scale;
    out[5] = ((T3dModelMatrix_guess*)matrix)->f70 * scale;
    out[6] = ((T3dModelMatrix_guess*)matrix)->f74 * scale;
    out[7] = ((T3dModelMatrix_guess*)matrix)->f78 * scale;
    out[8] = ((T3dModelMatrix_guess*)matrix)->f7c * scale;
    out[10] = ((T3dModelMatrix_guess*)matrix)->f6c * *(float *)(vert + 0x18) +
              ((T3dModelMatrix_guess*)matrix)->f78 * *(float *)(vert + 0x1c) +
              *(float *)(vert + 0x14) * ((T3dModelMatrix_guess*)matrix)->position[0] +
              ((T3dModelMatrix_guess*)matrix)->translationY_guess;
    out[11] = ((T3dModelMatrix_guess*)matrix)->f7c * *(float *)(vert + 0x1c) +
              ((T3dModelMatrix_guess*)matrix)->position[1] * *(float *)(vert + 0x14) +
              ((T3dModelMatrix_guess*)matrix)->f70 * *(float *)(vert + 0x18) +
              ((T3dModelMatrix_guess*)matrix)->translationZ_guess;
    out[9] = ((T3dModelMatrix_guess*)matrix)->position[2] * *(float *)(vert + 0x18) +
             ((T3dModelMatrix_guess*)matrix)->f74 * *(float *)(vert + 0x1c) +
             ((T3dModelMatrix_guess*)matrix)->f5c * *(float *)(vert + 0x14) +
             ((T3dModelMatrix_guess*)matrix)->translationX_guess;
}

// FUNCTION: EQGFX 0x10039134
// Applies point and directional effects to each transformed vertex span as an object is drawn.
void __cdecl emitEffectsForTransformedVertexSpans_guess(int vert, int matrixCtx, int *effectInfo, int destCtx, float shade)
{
    float inv;
    float baseX;
    float baseY;
    float baseZ;
    FxVec51c_guess pos;
    FxVec51c_guess tint;
    unsigned int curOff;
    unsigned int endOff;
    int extra;
    unsigned int span;
    int *fence;
    unsigned short *codes;
    int *slot;
    int rowsBase;
    float *matrix;

    fence = g_effectEntries_guess + g_effectCount_guess;
    inv = 1.0f / *(float *)((char *)effectInfo + 0x34);
    baseX = *(float *)(vert + 0x14);
    baseY = *(float *)(vert + 0x18);
    baseZ = *(float *)(vert + 0x1c);
    slot = *(int **)(*(int *)(vert + 0x34) + 0x14);
    codes = *(unsigned short **)((char *)slot + 0x48);
    curOff = (unsigned int)slot[1];
    vert = slot[5];
    rowsBase = *(int *)(matrixCtx + 0x24);
    endOff = curOff + (unsigned int)(slot[0] * 12);
    extra = *(int *)(destCtx + 0x61c);
    destCtx = *(int *)(destCtx + 0x614);
    if (curOff < endOff) {
        do {
            matrix = (float *)(rowsBase + (unsigned int)codes[1] * 0x148);
            span = (unsigned int)codes[0];
            effectInfo = g_effectEntries_guess;
            if (fence > effectInfo) {
                do {
                    FxEntry51c_guess *entry = (FxEntry51c_guess *)*effectInfo;
                    if (entry->kind == 0x1e) {
                        FxLink51c_guess *link = (FxLink51c_guess *)entry->refA;
                        FxHolder51c_guess *holder = (FxHolder51c_guess *)link->holder_int_guess;
                        int index = link->index_int_guess;
                        float factor;
                        if (holder->triples == 0) {
                            float single = holder->samples[index];
                            tint.x = single;
                            tint.y = single;
                            tint.z = single;
                        }
                        else {
                            tint = ((FxVec51c_guess *)holder->triples)[index];
                        }
                        factor = inv * matrix[22] * inv * entry->rad * entry->rad * EQGFX_POINT_RADIUS_SCALE_GUESS;
                        tint.x = factor * tint.x;
                        tint.y = factor * tint.y;
                        tint.z = factor * tint.z;
                        pos.x = entry->fxA * matrix[35] + entry->fyA * matrix[38] + entry->fzA * matrix[41] + matrix[44] - baseX;
                        pos.y = entry->fxA * matrix[36] + entry->fyA * matrix[39] + entry->fzA * matrix[42] + matrix[45] - baseY;
                        pos.z = entry->fxA * matrix[37] + entry->fyA * matrix[40] + entry->fzA * matrix[43] + matrix[46] - baseZ;
                        g_fxEmitA51c_guess(&pos.x, &tint.x, curOff, (unsigned int)vert, (void *)destCtx, span, (void *)extra);
                    }
                    else if (entry->kind == 0x1f) {
                        FxLink51c_guess *link = (FxLink51c_guess *)(*(int *)&entry->fxA);
                        FxHolder51c_guess *holder = (FxHolder51c_guess *)link->holder_int_guess;
                        int index = link->index_int_guess;
                        float fx;
                        float fy;
                        float fz;
                        if (holder->triples == 0) {
                            float single = holder->samples[index];
                            tint.x = single;
                            tint.y = single;
                            tint.z = single;
                        }
                        else {
                            tint = ((FxVec51c_guess *)holder->triples)[index];
                        }
                        fx = entry->fyA;
                        fy = entry->fzA;
                        fz = entry->rad;
                        pos.x = fx * matrix[47] + fy * matrix[48] + fz * matrix[49];
                        pos.y = fx * matrix[50] + fy * matrix[51] + fz * matrix[52];
                        pos.z = fx * matrix[53] + fy * matrix[54] + fz * matrix[55];
                        tint.x = tint.x * shade;
                        tint.y = tint.y * shade;
                        tint.z = tint.z * shade;
                        g_fxEmitC51c_guess(&pos.x, &tint.x, (unsigned int)vert, (void *)destCtx, span, (void *)extra);
                    }
                    effectInfo++;
                } while (effectInfo < fence);
            }
            extra += (int)span;
            curOff += span * 12;
            vert += (int)(span * 12);
            destCtx += (int)(span * 12);
            codes += 2;
        } while (curOff < endOff);
    }
}

// FUNCTION: EQGFX 0x10039401
// Lights active sprite vertices using ambient light and queued light sources.
void __cdecl processSpriteVertexLighting_guess(
    SpriteLightingObject_guess *sprite,
    SpriteLightingMatrixContext_guess *matrixContext,
    SpriteLightingScale_guess *scaleSource,
    SpriteLightingOutputContext_guess *outputContext,
    SpriteLightingActorTint_guess *actorTint,
    float intensity)
{
    float reciprocalScale;
    SpriteLightingGeometry_guess *geometry;
    unsigned short *spanData;
    short *vertices;
    short *vertexEnd;
    float *output;
    char *flags;
    unsigned int effectCount;
    float scale;
    float ambientX;
    float ambientY;
    float ambientZ;
    float lightingBuffer[21];

    reciprocalScale = 1.0f / scaleSource->divisor;
    geometry = sprite->resource->geometry;
    spanData = geometry->spans;
    vertices = geometry->vertices;
    vertexEnd = vertices + (unsigned int)geometry->vertexCount * 10;
    output = outputContext->output;
    flags = outputContext->flags;
    effectCount = g_effectCount_guess;
    scale = g_scaleTable_guess[geometry->scaleIndex];
    ambientX = outputContext->ambientX * intensity +
        g_byteFloatTable_guess[sprite->colorZ] +
        g_byteFloatTable_guess[actorTint->color5] +
        g_byteFloatTable_guess[actorTint->color2];
    ambientY = outputContext->ambientY * intensity +
        g_byteFloatTable_guess[sprite->colorY] +
        g_byteFloatTable_guess[actorTint->color4] +
        g_byteFloatTable_guess[actorTint->color1];
    ambientZ = outputContext->ambientZ * intensity +
        g_byteFloatTable_guess[sprite->colorX] +
        g_byteFloatTable_guess[actorTint->color3] +
        g_byteFloatTable_guess[actorTint->color0];

    do {
        unsigned short spanLength;
        void *lightMatrix;

        if (vertexEnd <= vertices)
            return;
        spanLength = *spanData;
        lightMatrix = (char *)matrixContext->matrixRows + (unsigned int)spanData[1] * 0x148;
        buildSpriteLightingPlanes_guess(sprite, lightMatrix, reciprocalScale, intensity, lightingBuffer);
        if (spanLength != 0) {
            unsigned int remainingVertices = (unsigned int)spanLength;
            do {
                if (*flags != '\0') {
                    unsigned int packedNormal = (unsigned int)(int)vertices[7];
                    float normalX = g_packedNormalTable_guess[packedNormal & 0x1f];
                    float normalY = g_packedNormalTable_guess[((int)packedNormal >> 5) & 0x1f];
                    float normalZ = g_packedNormalTable_guess[((int)packedNormal >> 10) & 0x1f];
                    float sourceX = (float)(int)vertices[0] * scale;
                    float sourceY = (float)(int)vertices[1] * scale;
                    float sourceZ = (float)(int)vertices[2] * scale;
                    float outputX = ambientX;
                    float outputY = ambientY;
                    float outputZ = ambientZ;

                    if (effectCount != 0) {
                        float *plane = lightingBuffer + 2;
                        int remainingEffects = (int)effectCount;
                        do {
                            if (*(unsigned int *)(plane - 2) == 0x1f) {
                                float amount = -(normalY * plane[0] +
                                    normalX * plane[-1] + normalZ * plane[1]);
                                if (amount > EQGFX_ZERO_F_GUESS) {
                                    outputX = amount * plane[2] + outputX;
                                    outputY = amount * plane[3] + outputY;
                                    outputZ = amount * plane[4] + outputZ;
                                }
                            }
                            else {
                                float dx = plane[-1] - sourceX;
                                float dy = plane[0] - sourceY;
                                float dz = plane[1] - sourceZ;
                                float facing = dx * normalX + dy * normalY + dz * normalZ;
                                if (facing > EQGFX_ZERO_F_GUESS) {
                                    float amount = g_spriteLightingSqrt_guess(
                                        dx * dx + dy * dy + dz * dz);
                                    amount = amount * amount * amount * facing;
                                    outputX = amount * plane[2] + outputX;
                                    outputY = amount * plane[3] + outputY;
                                    outputZ = amount * plane[4] + outputZ;
                                }
                            }
                            plane += 7;
                            remainingEffects--;
                        } while (remainingEffects != 0);
                    }
                    output[0] = outputX;
                    output[1] = outputY;
                    output[2] = outputZ;
                }
                flags++;
                vertices += 10;
                remainingVertices--;
                output += 3;
            } while (remainingVertices != 0);
        }
        spanData += 2;
    } while (1);
}

// FUNCTION: EQGFX 0x100396e2
// Lights an object's active mesh vertices (RGB): ambient + object colour + tints, directional and point lights
void __cdecl lightSpriteVerticesWithObjectColor_guess(SpriteLightingObject_guess *sprite,
    SpriteLightingOutputContext_guess *outputContext,
    SpriteLightingActorTint_guess *actorTint)
{
    SpriteLightingResource_guess *resource;
    SpriteLightingGeometry_guess *geometry;
    SpriteLightingVertex_guess *vertex;
    SpriteLightingVertex_guess *vertexEnd;
    float *output;
    char *flags;
    float scale;
    unsigned int effectCount;

    resource = sprite->resource;
    geometry = resource->geometry;
    vertex = (SpriteLightingVertex_guess *)geometry->vertices;
    scale = g_scaleTable_guess[geometry->scaleIndex];
    vertexEnd = vertex + (unsigned int)geometry->vertexCount;
    output = outputContext->output;
    flags = outputContext->flags;
    prepareSpriteLightingEffects_guess((char *)sprite + 0x14,
        g_spriteLightingEffects_guess);
    effectCount = g_effectCount_guess;

    if (geometry->vertexColorFlags == 0 || (resource->lightingFlags & 0x20) == 0) {
        float ambientX = outputContext->ambientX +
            g_byteFloatTable_guess[sprite->colorZ] +
            g_byteFloatTable_guess[actorTint->color5] +
            g_byteFloatTable_guess[actorTint->color2];
        float ambientY = outputContext->ambientY +
            g_byteFloatTable_guess[sprite->colorY] +
            g_byteFloatTable_guess[actorTint->color4] +
            g_byteFloatTable_guess[actorTint->color1];
        float ambientZ = outputContext->ambientZ +
            g_byteFloatTable_guess[sprite->colorX] +
            g_byteFloatTable_guess[actorTint->color3] +
            g_byteFloatTable_guess[actorTint->color0];

        for (; vertex < vertexEnd; vertex++) {
            if (*flags != '\0') {
                int packedNormal = (int)vertex->packedNormal;
                float normalX = g_packedNormalTable_guess[packedNormal & 0x1f];
                float normalY = g_packedNormalTable_guess[(packedNormal >> 5) & 0x1f];
                float normalZ = g_packedNormalTable_guess[(packedNormal >> 10) & 0x1f];
                float sourceX = (float)(int)vertex->positionX * scale;
                float sourceY = (float)(int)vertex->positionY * scale;
                float sourceZ = (float)(int)vertex->positionZ * scale;
                float outputX = ambientX;
                float outputY = ambientY;
                float outputZ = ambientZ;

                if (effectCount != 0) {
                    SpriteLightingEffect_guess *effect = g_spriteLightingEffects_guess;
                    int remainingEffects = (int)effectCount;
                    do {
                        if (*(unsigned int *)&effect->type ==
                            SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG_BITS) {
                            float amount = -(normalY * effect->pointY +
                                normalX * effect->pointX + normalZ * effect->pointZ);
                            if (amount > EQGFX_ZERO_F_GUESS) {
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        else {
                            float dx = effect->pointX - sourceX;
                            float dy = effect->pointY - sourceY;
                            float dz = effect->pointZ - sourceZ;
                            float facing = dx * normalX + dy * normalY + dz * normalZ;
                            if (facing > EQGFX_ZERO_F_GUESS) {
                                float amount = g_spriteLightingSqrt_guess(
                                    dx * dx + dy * dy + dz * dz);
                                amount = amount * amount * amount * facing;
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        effect++;
                        remainingEffects--;
                    } while (remainingEffects != 0);
                }
                output[0] = outputX;
                output[1] = outputY;
                output[2] = outputZ;
            }
            flags++;
            output += 3;
        }
    }
    else {
        float ambientX = outputContext->ambientX;
        float ambientY = outputContext->ambientY;
        float ambientZ = outputContext->ambientZ;
        float baseTintX = g_byteFloatTable_guess[sprite->colorZ] +
            g_byteFloatTable_guess[actorTint->color5] +
            g_byteFloatTable_guess[actorTint->color2];
        float baseTintY = g_byteFloatTable_guess[sprite->colorY] +
            g_byteFloatTable_guess[actorTint->color4] +
            g_byteFloatTable_guess[actorTint->color1];
        float baseTintZ = g_byteFloatTable_guess[sprite->colorX] +
            g_byteFloatTable_guess[actorTint->color3] +
            g_byteFloatTable_guess[actorTint->color0];

        for (; vertex < vertexEnd; vertex++) {
            if (*flags != '\0') {
                int packedNormal = (int)vertex->packedNormal;
                float normalX = g_packedNormalTable_guess[packedNormal & 0x1f];
                float normalY = g_packedNormalTable_guess[(packedNormal >> 5) & 0x1f];
                float normalZ = g_packedNormalTable_guess[(packedNormal >> 10) & 0x1f];
                float sourceX = (float)(int)vertex->positionX * scale;
                float sourceY = (float)(int)vertex->positionY * scale;
                float sourceZ = (float)(int)vertex->positionZ * scale;
                float vertexIntensity = g_byteFloatTable_guess[vertex->colorIntensity];
                float outputX = baseTintX + g_byteFloatTable_guess[vertex->colorX] +
                    vertexIntensity * ambientX;
                float outputY = baseTintY + g_byteFloatTable_guess[vertex->colorY] +
                    vertexIntensity * ambientY;
                float outputZ = baseTintZ + g_byteFloatTable_guess[vertex->colorZ] +
                    vertexIntensity * ambientZ;

                if (effectCount != 0) {
                    SpriteLightingEffect_guess *effect = g_spriteLightingEffects_guess;
                    int remainingEffects = (int)effectCount;
                    do {
                        if (*(unsigned int *)&effect->type ==
                            SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG_BITS) {
                            float amount = -(normalY * effect->pointY +
                                normalX * effect->pointX + normalZ * effect->pointZ);
                            if (amount > EQGFX_ZERO_F_GUESS) {
                                amount = amount * vertexIntensity;
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        else {
                            float dx = effect->pointX - sourceX;
                            float dy = effect->pointY - sourceY;
                            float dz = effect->pointZ - sourceZ;
                            float facing = dx * normalX + dy * normalY + dz * normalZ;
                            if (facing > EQGFX_ZERO_F_GUESS) {
                                float amount = g_spriteLightingSqrt_guess(
                                    dx * dx + dy * dy + dz * dz);
                                amount = amount * amount * amount * facing;
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        effect++;
                        remainingEffects--;
                    } while (remainingEffects != 0);
                }
                output[0] = outputX;
                output[1] = outputY;
                output[2] = outputZ;
            }
            flags++;
            output += 3;
        }
    }
}

// FUNCTION: EQGFX 0x10039be2
// Lights a mesh resource's active vertices, lights set up from its geometry: ambient + tints, no object colour
void __cdecl lightGeometryVerticesWithTint_guess(SpriteLightingVertexSource_guess *lightingSource,
    SpriteLightingOutputContext_guess *outputContext,
    SpriteLightingActorTint_guess *actorTint)
{
    SpriteLightingGeometry_guess *geometry;
    SpriteLightingVertex_guess *vertex;
    SpriteLightingVertex_guess *vertexEnd;
    float *output;
    char *flags;
    float scale;
    unsigned int effectCount;
    float ambientX;
    float ambientY;
    float ambientZ;

    geometry = lightingSource->geometry;
    vertex = (SpriteLightingVertex_guess *)geometry->vertices;
    vertexEnd = vertex + (unsigned int)geometry->vertexCount;
    output = outputContext->output;
    flags = outputContext->flags;
    scale = g_scaleTable_guess[geometry->scaleIndex];
    prepareSpriteLightingEffects_guess(geometry->lightingParameters,
        g_spriteLightingEffects_guess);
    effectCount = g_effectCount_guess;
    if (geometry->vertexColorFlags == 0 || (lightingSource->flags & 0x20) == 0) {
        ambientX = outputContext->ambientX +
            g_byteFloatTable_guess[actorTint->color5] +
            g_byteFloatTable_guess[actorTint->color2];
        ambientY = outputContext->ambientY +
            g_byteFloatTable_guess[actorTint->color4] +
            g_byteFloatTable_guess[actorTint->color1];
        ambientZ = outputContext->ambientZ +
            g_byteFloatTable_guess[actorTint->color3] +
            g_byteFloatTable_guess[actorTint->color0];
        for (; vertex < vertexEnd; vertex++) {
            if (*flags != '\0') {
                unsigned int packedNormal = (unsigned int)(int)vertex->packedNormal;
                float normalX = g_packedNormalTable_guess[packedNormal & 0x1f];
                float normalY = g_packedNormalTable_guess[((int)packedNormal >> 5) & 0x1f];
                float normalZ = g_packedNormalTable_guess[((int)packedNormal >> 10) & 0x1f];
                short sourceShortX = vertex->positionX;
                short sourceShortY = vertex->positionY;
                short sourceShortZ = vertex->positionZ;
                float sourceX = (float)(int)sourceShortX * scale;
                float sourceY = (float)(int)sourceShortY * scale;
                float sourceZ = (float)(int)sourceShortZ * scale;
                float outputX = ambientX;
                float outputY = ambientY;
                float outputZ = ambientZ;

                if (effectCount != 0) {
                    SpriteLightingEffect_guess *effect = g_spriteLightingEffects_guess;
                    int remainingEffects = (int)effectCount;
                    do {
                        if (*(unsigned int *)&effect->type ==
                            SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG_BITS) {
                            float amount = -(normalY * effect->pointY +
                                normalX * effect->pointX + normalZ * effect->pointZ);
                            if (amount > EQGFX_ZERO_F_GUESS) {
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        else {
                            float dx = effect->pointX - sourceX;
                            float dy = effect->pointY - sourceY;
                            float dz = effect->pointZ - sourceZ;
                            float facing = dx * normalX + dy * normalY + dz * normalZ;
                            if (facing > EQGFX_ZERO_F_GUESS) {
                                float amount = g_spriteLightingSqrt_guess(
                                    dx * dx + dy * dy + dz * dz);
                                amount = amount * amount * amount * facing;
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        effect++;
                        remainingEffects--;
                    } while (remainingEffects != 0);
                }
                output[0] = outputX;
                output[1] = outputY;
                output[2] = outputZ;
            }
            flags++;
            output += 3;
        }
    }
    else {
        float ambientBaseX = outputContext->ambientX;
        float ambientBaseY = outputContext->ambientY;
        float ambientBaseZ = outputContext->ambientZ;
        float tintX = g_byteFloatTable_guess[actorTint->color5] +
            g_byteFloatTable_guess[actorTint->color2];
        float tintY = g_byteFloatTable_guess[actorTint->color4] +
            g_byteFloatTable_guess[actorTint->color1];
        float tintZ = g_byteFloatTable_guess[actorTint->color3] +
            g_byteFloatTable_guess[actorTint->color0];

        for (; vertex < vertexEnd; vertex++) {
            if (*flags != '\0') {
                unsigned int packedNormal = (unsigned int)(int)vertex->packedNormal;
                float normalX = g_packedNormalTable_guess[packedNormal & 0x1f];
                float normalY = g_packedNormalTable_guess[((int)packedNormal >> 5) & 0x1f];
                float normalZ = g_packedNormalTable_guess[((int)packedNormal >> 10) & 0x1f];
                short sourceShortX = vertex->positionX;
                short sourceShortY = vertex->positionY;
                short sourceShortZ = vertex->positionZ;
                float vertexIntensity = g_byteFloatTable_guess[vertex->colorIntensity];
                float sourceX = (float)(int)sourceShortX * scale;
                float sourceY = (float)(int)sourceShortY * scale;
                float sourceZ = (float)(int)sourceShortZ * scale;
                float outputX = tintX + g_byteFloatTable_guess[vertex->colorX] +
                    vertexIntensity * ambientBaseX;
                float outputY = tintY + g_byteFloatTable_guess[vertex->colorY] +
                    vertexIntensity * ambientBaseY;
                float outputZ = tintZ + g_byteFloatTable_guess[vertex->colorZ] +
                    vertexIntensity * ambientBaseZ;

                if (effectCount != 0) {
                    SpriteLightingEffect_guess *effect = g_spriteLightingEffects_guess;
                    int remainingEffects = (int)effectCount;
                    do {
                        if (*(unsigned int *)&effect->type ==
                            SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG_BITS) {
                            float amount = -(normalY * effect->pointY +
                                normalX * effect->pointX + normalZ * effect->pointZ);
                            if (amount > EQGFX_ZERO_F_GUESS) {
                                amount = amount * vertexIntensity;
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        else {
                            float dx = effect->pointX - sourceX;
                            float dy = effect->pointY - sourceY;
                            float dz = effect->pointZ - sourceZ;
                            float facing = dx * normalX + dy * normalY + dz * normalZ;
                            if (facing > EQGFX_ZERO_F_GUESS) {
                                float amount = g_spriteLightingSqrt_guess(
                                    dx * dx + dy * dy + dz * dz);
                                amount = amount * amount * amount * facing;
                                outputX = amount * effect->addX + outputX;
                                outputY = amount * effect->addY + outputY;
                                outputZ = amount * effect->addZ + outputZ;
                            }
                        }
                        effect++;
                        remainingEffects--;
                    } while (remainingEffects != 0);
                }
                output[0] = outputX;
                output[1] = outputY;
                output[2] = outputZ;
            }
            flags++;
            output += 3;
        }
    }
    return;
}

// FUNCTION: EQGFX 0x1003a097
// Adds queued effects to an object's draw data, using extra color detail when available.
void __cdecl emitQueuedEffectsForObject_guess(int info, int context)
{
    Host234_guess *host;
    int useExtra;
    int detail;
    FxVec51c_guess tint;
    int **cursor;
    int **fence;

    host = (Host234_guess *)((NodeA234_guess *)info)->host;
    useExtra = 0;
    detail = 0;
    if (((List234_guess *)host->list)->flag != 0 && (host->flags & 0x20) != 0) {
        detail = (int)((List234_guess *)host->list)->codes;
        useExtra = 1;
    }
    else {
        int sibling = ((NodeA234_guess *)info)->sibling;
        if (sibling != 0 && (((NodeG234_guess *)((Sib234_guess *)sibling)->node)->flags & 0x80) != 0) {
            useExtra = 1;
            detail = ((NodeG234_guess *)((Sib234_guess *)sibling)->node)->rows[((Sib234_guess *)sibling)->index];
        }
    }
    cursor = ((PhysEffectQueue_guess *)context)->entries;
    fence = cursor + ((PhysEffectQueue_guess *)context)->count;
    if (cursor < fence) {
        do {
            FxEntry51c_guess *entry = (FxEntry51c_guess *)*cursor;
            if (entry->kind == 0x1e) {
                if ((entry->flags & 0x20) != 0) {
                    FxLink51c_guess *link = (FxLink51c_guess *)entry->refA;
                    FxHolder51c_guess *holder = (FxHolder51c_guess *)link->holder_int_guess;
                    int index = link->index_int_guess;
                    float atten;
                    if (holder->triples == 0) {
                        float single = holder->samples[index];
                        tint.x = single;
                        tint.y = single;
                        tint.z = single;
                    }
                    else {
                        tint = ((FxVec51c_guess *)holder->triples)[index];
                    }
                    atten = entry->rad * entry->rad * EQGFX_POINT_RADIUS_SCALE_GUESS;
                    tint.x = atten * tint.x;
                    tint.y = atten * tint.y;
                    tint.z = atten * tint.z;
                    {
                        int *slotVals = (int *)host->list;
                        g_fxEmitA51c_guess(&entry->fxA, &tint.x, (unsigned int)slotVals[1], (unsigned int)slotVals[5], ((Ctx234_guess *)context)->out, (unsigned int)slotVals[0], ((Ctx234_guess *)context)->active);
                    }
                }
            }
            else if (entry->kind == 0x1f) {
                if ((entry->flags & 0x20) != 0) {
                    FxLink51c_guess *link = (FxLink51c_guess *)(*(int *)&entry->fxA);
                    FxHolder51c_guess *holder = (FxHolder51c_guess *)link->holder_int_guess;
                    int index = link->index_int_guess;
                    if (holder->triples == 0) {
                        float single = holder->samples[index];
                        tint.x = single;
                        tint.y = single;
                        tint.z = single;
                    }
                    else {
                        tint = ((FxVec51c_guess *)holder->triples)[index];
                    }
                    {
                        int *slotVals = (int *)host->list;
                        if (useExtra == 1) {
                            g_fxEmitB51c_guess(&entry->fyA, &tint.x, (unsigned int)slotVals[5], detail, ((Ctx234_guess *)context)->out, (unsigned int)slotVals[0], ((Ctx234_guess *)context)->active);
                        }
                        else {
                            g_fxEmitC51c_guess(&entry->fyA, &tint.x, (unsigned int)slotVals[5], ((Ctx234_guess *)context)->out, (unsigned int)slotVals[0], ((Ctx234_guess *)context)->active);
                        }
                    }
                }
            }
            cursor++;
        } while (cursor < fence);
    }
}

// FUNCTION: EQGFX 0x1003a234
// Combines an object's color inputs into per-vertex colors for drawing.
// @early-stop: frame size, register allocation, and x87/branch scheduling still differ.
void __cdecl computePerVertexColorValues_guess(int info, int context, int palette, float blend)
{
    NodeA234_guess *prim;
    Ctx234_guess *target;
    Pal234_guess *extra;
    Host234_guess *host;
    List234_guess *list;
    float dirX;
    float dirY;
    float dirZ;
    float addX;
    float addY;
    float addZ;
    float *out;
    float *end;
    char *active;
    unsigned char *codes;
    int sibling;
    float level;
    float preX;
    float preY;
    float preZ;
    Sib234_guess *sib;
    NodeG234_guess *node;

    prim = (NodeA234_guess *)info;
    target = (Ctx234_guess *)context;
    extra = (Pal234_guess *)palette;
    dirY = target->vecY;
    dirZ = target->vecZ;
    dirX = target->vecX;
    sibling = prim->sibling;
    addX = g_byteFloatTable_guess[prim->c2] + g_byteFloatTable_guess[extra->v5];
    addY = g_byteFloatTable_guess[prim->c1] + g_byteFloatTable_guess[extra->v4];
    addZ = g_byteFloatTable_guess[prim->c0] + g_byteFloatTable_guess[extra->v3];
    host = (Host234_guess *)prim->host;
    out = target->out;
    active = target->active;
    list = (List234_guess *)host->list;
    end = out + list->count * 3;
    if ((list->flag == 0) || (g_renderDisable_guess == 0 && (host->flags & 0x20) == 0)) {
        if (sibling == 0 || (g_renderDisable_guess == 0 && (((NodeG234_guess *)(*(Sib234_guess *)sibling).node)->flags & 0x80) == 0)) {
            preX = dirX * blend + addX + g_byteFloatTable_guess[extra->v2];
            preY = dirY * blend + addY + g_byteFloatTable_guess[extra->v1];
            preZ = dirZ * blend + addZ + g_byteFloatTable_guess[extra->v0];
            for (; out < end; out += 3) {
                if (*active != 0) {
                    out[0] = preX;
                    out[1] = preY;
                    out[2] = preZ;
                }
                active++;
            }
        }
        else {
            sib = (Sib234_guess *)sibling;
            node = (NodeG234_guess *)sib->node;
            codes = (unsigned char *)node->rows[sib->index];
            if ((node->flags & 0x80) == 0) {
                preX = dirX * blend + addX;
                preY = dirY * blend + addY;
                preZ = dirZ * blend + addZ;
                for (; out < end; out += 3) {
                    if (*active != 0) {
                        out[0] = preX + g_byteFloatTable_guess[codes[2]];
                        out[1] = preY + g_byteFloatTable_guess[codes[1]];
                        out[2] = preZ + g_byteFloatTable_guess[codes[0]];
                    }
                    active++;
                    codes += 4;
                }
            }
            else {
                for (; out < end; out += 3) {
                    if (*active != 0) {
                        level = g_byteFloatTable_guess[codes[3]];
                        out[0] = level * dirX + g_byteFloatTable_guess[codes[2]] + addX;
                        out[1] = level * dirY + g_byteFloatTable_guess[codes[1]] + addY;
                        out[2] = level * dirZ + g_byteFloatTable_guess[codes[0]] + addZ;
                    }
                    active++;
                    codes += 4;
                }
            }
        }
    }
    else {
        codes = list->codes;
        if ((host->flags & 0x20) == 0) {
            preX = dirX * blend + addX;
            preY = dirY * blend + addY;
            preZ = dirZ * blend + addZ;
            for (; out < end; out += 3) {
                if (*active != 0) {
                    out[0] = preX + g_byteFloatTable_guess[codes[2]];
                    out[1] = preY + g_byteFloatTable_guess[codes[1]];
                    out[2] = preZ + g_byteFloatTable_guess[codes[0]];
                }
                codes += 4;
                active++;
            }
        }
        else {
            for (; out < end; out += 3) {
                if (*active != 0) {
                    level = g_byteFloatTable_guess[codes[3]];
                    out[0] = level * dirX + g_byteFloatTable_guess[codes[2]] + addX;
                    out[1] = level * dirY + g_byteFloatTable_guess[codes[1]] + addY;
                    out[2] = level * dirZ + g_byteFloatTable_guess[codes[0]] + addZ;
                }
                codes += 4;
                active++;
            }
        }
    }
}

// FUNCTION: EQGFX 0x1003a51c
// Applies queued effects at positions transformed by the object's current pose.
void __cdecl emitQueuedEffectsWithStageTransform_guess(int info, int context, float divisor, float *stage, float *basis, float shade)
{
    Host234_guess *owner;
    int useExtra;
    FxVec51c_guess pos;
    FxVec51c_guess tint;
    int *fence;
    int *cursor;
    float inv;
    int detail;

    inv = 1.0f / divisor;
    owner = (Host234_guess *)((NodeA234_guess *)info)->host;
    useExtra = 0;
    detail = 0;
    if (((List234_guess *)owner->list)->flag != 0 && (owner->flags & 0x20) != 0) {
        detail = (int)((List234_guess *)owner->list)->codes;
        useExtra = 1;
    }
    else {
        int sibling = ((NodeA234_guess *)info)->sibling;
        if (sibling != 0 && (((NodeG234_guess *)((Sib234_guess *)sibling)->node)->flags & 0x80) != 0) {
            useExtra = 1;
            detail = ((NodeG234_guess *)((Sib234_guess *)sibling)->node)->rows[((Sib234_guess *)sibling)->index];
        }
    }
    cursor = g_effectEntries_guess;
    fence = g_effectEntries_guess + g_effectCount_guess;
    if (fence > cursor) {
        do {
            FxEntry51c_guess *entry = (FxEntry51c_guess *)*cursor;
            if (entry->kind == 0x1e) {
                FxLink51c_guess *link = (FxLink51c_guess *)entry->refA;
                FxHolder51c_guess *holder = (FxHolder51c_guess *)link->holder_int_guess;
                int index = link->index_int_guess;
                if (holder->triples == 0) {
                    float single = holder->samples[index];
                    tint.x = single;
                    tint.y = single;
                    tint.z = single;
                }
                else {
                    tint = ((FxVec51c_guess *)holder->triples)[index];
                }
                pos.x = entry->fxA * stage[0] + entry->fyA * stage[3] + entry->fzA * stage[6] + stage[9];
                pos.y = entry->fxA * stage[1] + entry->fyA * stage[4] + entry->fzA * stage[7] + stage[10];
                pos.z = entry->fxA * stage[2] + entry->fyA * stage[5] + entry->fzA * stage[8] + stage[11];
                tint.x = entry->rad * entry->rad * inv * inv * EQGFX_POINT_RADIUS_SCALE_GUESS * tint.x;
                tint.y = entry->rad * entry->rad * inv * inv * EQGFX_POINT_RADIUS_SCALE_GUESS * tint.y;
                tint.z = entry->rad * entry->rad * inv * inv * EQGFX_POINT_RADIUS_SCALE_GUESS * tint.z;
                {
                    int *slotVals = (int *)owner->list;
                    g_fxEmitA51c_guess(&pos.x, &tint.x, (unsigned int)slotVals[1], (unsigned int)slotVals[5], ((Ctx234_guess *)context)->out, (unsigned int)slotVals[0], ((Ctx234_guess *)context)->active);
                }
            }
            else if (entry->kind == 0x1f) {
                FxLink51c_guess *link = (FxLink51c_guess *)(*(int *)&entry->fxA);
                FxHolder51c_guess *holder = (FxHolder51c_guess *)link->holder_int_guess;
                int index = link->index_int_guess;
                float fx;
                float fy;
                float fz;
                if (holder->triples == 0) {
                    float single = holder->samples[index];
                    tint.x = single;
                    tint.y = single;
                    tint.z = single;
                }
                else {
                    tint = ((FxVec51c_guess *)holder->triples)[index];
                }
                fx = entry->fyA;
                fy = entry->fzA;
                fz = entry->rad;
                pos.x = fx * basis[0] + fy * basis[1] + fz * basis[2];
                pos.y = fx * basis[3] + fy * basis[4] + fz * basis[5];
                pos.z = fx * basis[6] + fy * basis[7] + fz * basis[8];
                if (useExtra == 1) {
                    int *slotVals = (int *)owner->list;
                    g_fxEmitB51c_guess(&pos.x, &tint.x, (unsigned int)slotVals[5], detail, ((Ctx234_guess *)context)->out, (unsigned int)slotVals[0], ((Ctx234_guess *)context)->active);
                }
                else {
                    int *slotVals = (int *)owner->list;
                    tint.x = tint.x * shade;
                    tint.y = tint.y * shade;
                    tint.z = tint.z * shade;
                    g_fxEmitC51c_guess(&pos.x, &tint.x, (unsigned int)slotVals[5], ((Ctx234_guess *)context)->out, (unsigned int)slotVals[0], ((Ctx234_guess *)context)->active);
                }
            }
            cursor++;
        } while (cursor < fence);
    }
}

// FUNCTION: EQGFX 0x1003a79c
// Lights an object's active vertices with scaled ambient; baked colours come from its attachment's colour row
void __cdecl lightSpriteVerticesScaledAmbient_guess(SpriteLightingObject_guess *lightingObject, SpriteLightingOutputContext_guess *context,
                          SpriteLightingActorTint_guess *palette, float effectScale, int effectFlags,
                          int effectLimit, float ambientScale)
{
    SpriteLightingGeometry_guess *geometry;
    SpriteLightingAttachment_guess *attachment;
    short *vertex;
    short *vertexEnd;
    float *output;
    char *activeFlags;
    float vertexScale;
    int effectCount;

    geometry = lightingObject->resource->geometry;
    vertex = geometry->vertices;
    vertexEnd = vertex + geometry->vertexCount * 10;
    vertexScale = g_scaleTable_guess[geometry->scaleIndex];
    output = context->output;
    activeFlags = context->flags;
    updateSpriteLightingEffects_guess(lightingObject, &g_spriteLightingEffects_guess[0], effectScale, effectFlags, effectLimit);
    effectCount = (int)g_effectCount_guess;
    attachment = (SpriteLightingAttachment_guess *)lightingObject->attachment;
    if (attachment == 0 || (attachment->state->flags & 0x80) == 0) {
        float ambientOutputX;
        float ambientOutputY;
        float ambientOutputZ;

        ambientOutputX = context->ambientX * ambientScale + g_byteFloatTable_guess[lightingObject->colorZ] +
                         g_byteFloatTable_guess[palette->color5] + g_byteFloatTable_guess[palette->color2];
        ambientOutputY = context->ambientY * ambientScale + g_byteFloatTable_guess[lightingObject->colorY] +
                         g_byteFloatTable_guess[palette->color4] + g_byteFloatTable_guess[palette->color1];
        ambientOutputZ = context->ambientZ * ambientScale + g_byteFloatTable_guess[lightingObject->colorX] +
                         g_byteFloatTable_guess[palette->color3] + g_byteFloatTable_guess[palette->color0];
        for (; vertex < vertexEnd; vertex += 10, output += 3, activeFlags++) {
            if (*activeFlags != '\0') {
                int normalCode = (int)vertex[7];
                float normalX = g_packedNormalTable_guess[(unsigned int)normalCode & 0x1f];
                float normalY = g_packedNormalTable_guess[(normalCode >> 5) & 0x1f];
                float normalZ = g_packedNormalTable_guess[(normalCode >> 10) & 0x1f];
                short vertexX = vertex[0];
                short vertexY = vertex[1];
                short vertexZ = vertex[2];
                float outputX = ambientOutputX;
                float outputY = ambientOutputY;
                float outputZ = ambientOutputZ;

                if (effectCount != 0) {
                    SpriteLightingEffect_guess *effect = &g_spriteLightingEffects_guess[0];
                    int remainingEffects = effectCount;

                    do {
                        if (effect->type == SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG) {
                            float direction = -(normalY * effect->pointY + normalX * effect->pointX + normalZ * effect->pointZ);
                            if (direction > 0.0f) {
                                outputX = direction * effect->addX + outputX;
                                outputY = direction * effect->addY + outputY;
                                outputZ = direction * effect->addZ + outputZ;
                            }
                        }
                        else {
                            float distanceX = effect->pointX - (float)vertexX * vertexScale;
                            float distanceY = effect->pointY - (float)vertexY * vertexScale;
                            float distanceZ = effect->pointZ - (float)vertexZ * vertexScale;
                            float facing = distanceX * normalX + distanceY * normalY + distanceZ * normalZ;

                            if (facing > 0.0f) {
                                float distance = g_spriteLightingSqrt_guess(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
                                float contribution = distance * distance * distance * facing;
                                outputX = contribution * effect->addX + outputX;
                                outputY = contribution * effect->addY + outputY;
                                outputZ = contribution * effect->addZ + outputZ;
                            }
                        }
                        effect++;
                        remainingEffects--;
                    } while (remainingEffects != 0);
                }
                output[0] = outputX;
                output[1] = outputY;
                output[2] = outputZ;
            }
        }
    }
    else {
        unsigned char *colorBytes;
        float objectColorX;
        float objectColorY;
        float objectColorZ;
        float paletteColorX1;
        float paletteColorX2;
        float paletteColorY1;
        float paletteColorY2;
        float paletteColorZ1;
        float paletteColorZ2;

        colorBytes = attachment->state->colorRows[attachment->colorRowIndex];
        objectColorX = g_byteFloatTable_guess[lightingObject->colorZ];
        objectColorY = g_byteFloatTable_guess[palette->color5];
        objectColorZ = g_byteFloatTable_guess[palette->color2];
        paletteColorX1 = g_byteFloatTable_guess[lightingObject->colorY];
        paletteColorX2 = g_byteFloatTable_guess[palette->color4];
        paletteColorY1 = g_byteFloatTable_guess[palette->color1];
        paletteColorY2 = g_byteFloatTable_guess[lightingObject->colorX];
        paletteColorZ1 = g_byteFloatTable_guess[palette->color3];
        paletteColorZ2 = g_byteFloatTable_guess[palette->color0];

        for (; vertex < vertexEnd; vertex += 10, output += 3, activeFlags++, colorBytes += 4) {
            if (*activeFlags != '\0') {
                int normalCode = (int)vertex[7];
                float normalX = g_packedNormalTable_guess[(unsigned int)normalCode & 0x1f];
                float normalY = g_packedNormalTable_guess[(normalCode >> 5) & 0x1f];
                float normalZ = g_packedNormalTable_guess[(normalCode >> 10) & 0x1f];
                short vertexX = vertex[0];
                short vertexY = vertex[1];
                short vertexZ = vertex[2];
                float paletteIntensity = g_byteFloatTable_guess[colorBytes[3]];
                float outputX = paletteIntensity * context->ambientX + objectColorX + objectColorY + objectColorZ +
                                g_byteFloatTable_guess[colorBytes[2]];
                float outputY = paletteIntensity * context->ambientY + paletteColorX1 + paletteColorX2 + paletteColorY1 +
                                g_byteFloatTable_guess[colorBytes[1]];
                float outputZ = paletteIntensity * context->ambientZ + paletteColorY2 + paletteColorZ1 + paletteColorZ2 +
                                g_byteFloatTable_guess[colorBytes[0]];

                if (effectCount != 0) {
                    SpriteLightingEffect_guess *effect = &g_spriteLightingEffects_guess[0];
                    int remainingEffects = effectCount;

                    do {
                        if (effect->type == SPRITE_LIGHTING_DIRECTIONAL_EFFECT_TAG) {
                            float direction = -(normalY * effect->pointY + normalX * effect->pointX + normalZ * effect->pointZ);
                            if (direction > 0.0f) {
                                direction = direction * paletteIntensity;
                                outputX = direction * effect->addX + outputX;
                                outputY = direction * effect->addY + outputY;
                                outputZ = direction * effect->addZ + outputZ;
                            }
                        }
                        else {
                            float distanceX = effect->pointX - (float)vertexX * vertexScale;
                            float distanceY = effect->pointY - (float)vertexY * vertexScale;
                            float distanceZ = effect->pointZ - (float)vertexZ * vertexScale;
                            float facing = distanceX * normalX + distanceY * normalY + distanceZ * normalZ;

                            if (facing > 0.0f) {
                                float distance = g_spriteLightingSqrt_guess(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
                                float contribution = distance * distance * distance * facing;
                                outputX = contribution * effect->addX + outputX;
                                outputY = contribution * effect->addY + outputY;
                                outputZ = contribution * effect->addZ + outputZ;
                            }
                        }
                        effect++;
                        remainingEffects--;
                    } while (remainingEffects != 0);
                }
                output[0] = outputX;
                output[1] = outputY;
                output[2] = outputZ;
            }
        }
    }
    return;
}

// FUNCTION: EQGFX 0x1003ace0
// Builds the 12-float stage matrix from basis and frame, then draws it.
void __cdecl transformVerticesWithBasis8c_guess(int holder, int basis, float *shift, int frame)
{
    BasisAce0_guess *rows;
    Frame1af_guess *stage;
    float xfer[12];
    float sumX;
    float sumY;
    float sumZ;
    int *slot;

    rows = (BasisAce0_guess *)basis;
    stage = (Frame1af_guess *)frame;
    xfer[0] = rows->f8c * stage->t248 + rows->f94 * stage->t250 + rows->f90 * stage->t24c;
    xfer[1] = stage->t258 * rows->f90 + stage->t25c * rows->f94 + rows->f8c * stage->t254;
    xfer[2] = stage->t244 * rows->f94 + rows->f8c * stage->t22c + rows->f90 * stage->t238;
    xfer[3] = stage->t24c * rows->f9c + rows->f98 * stage->t248 + stage->t250 * rows->fa0;
    xfer[4] = stage->t25c * rows->fa0 + stage->t258 * rows->f9c + rows->f98 * stage->t254;
    xfer[5] = stage->t244 * rows->fa0 + rows->f98 * stage->t22c + rows->f9c * stage->t238;
    xfer[6] = stage->t250 * rows->fac + stage->t248 * rows->fa4 + rows->fa8 * stage->t24c;
    xfer[7] = stage->t258 * rows->fa8 + stage->t254 * rows->fa4 + stage->t25c * rows->fac;
    xfer[8] = rows->fa8 * stage->t238 + stage->t22c * rows->fa4 + stage->t244 * rows->fac;
    sumX = stage->t1ac + shift[0];
    sumY = stage->t1b0 + shift[1];
    sumZ = stage->t1b4 + shift[2];
    xfer[9] = sumX * stage->t248 + sumZ * stage->t250 + sumY * stage->t24c;
    xfer[10] = sumY * stage->t258 + sumZ * stage->t25c + sumX * stage->t254;
    xfer[11] = sumZ * stage->t244 + sumX * stage->t22c + sumY * stage->t238;
    slot = ((Middle1af_guess *)((Outer1af_guess *)holder)->middle)->slot;
    g_draw1af_guess(slot[1], stage->t610, xfer, slot[0], stage->t61c);
}

// FUNCTION: EQGFX 0x1003af14
// Positions sprite mesh vertices for the current pose, including alternate appearances.
void __cdecl transformScaledSpriteVertices_guess(SpriteLightingObject_guess *sprite, BasisAce0_guess *basis, float *shift, Frame1af_guess *frame)
{
    SpriteLightingResource_guess *resource;
    SpriteLightingGeometry_guess *geometry;
    SpriteLightingAlternateObject_guess *alternateMesh;
    SpriteLightingAlternateResource_guess *alternateResource;
    SpriteLightingAlternateGeometry_guess *alternateGeometry;
    float scale;
    float matrix[12];
    float translatedX;
    float translatedY;
    float translatedZ;

    resource = sprite->resource;
    geometry = resource->geometry;
    scale = g_scaleTable_guess[geometry->scaleIndex];
    matrix[0] = (basis->f8c * frame->t248 + frame->t24c * basis->f90 + frame->t250 * basis->f94) * scale;
    matrix[1] = (basis->f8c * frame->t254 + frame->t25c * basis->f94 + frame->t258 * basis->f90) * scale;
    matrix[2] = (frame->t244 * basis->f94 + frame->t238 * basis->f90 + basis->f8c * frame->t22c) * scale;
    matrix[3] = (frame->t24c * basis->f9c + basis->f98 * frame->t248 + frame->t250 * basis->fa0) * scale;
    matrix[4] = (basis->f98 * frame->t254 + frame->t258 * basis->f9c + frame->t25c * basis->fa0) * scale;
    matrix[5] = (frame->t238 * basis->f9c + frame->t244 * basis->fa0 + basis->f98 * frame->t22c) * scale;
    matrix[6] = (frame->t250 * basis->fac + basis->fa4 * frame->t248 + basis->fa8 * frame->t24c) * scale;
    matrix[7] = (basis->fa4 * frame->t254 + frame->t258 * basis->fa8 + frame->t25c * basis->fac) * scale;
    matrix[8] = (basis->fac * frame->t244 + basis->fa4 * frame->t22c + frame->t238 * basis->fa8) * scale;
    translatedX = frame->t1ac + shift[0];
    translatedY = frame->t1b0 + shift[1];
    translatedZ = frame->t1b4 + shift[2];
    matrix[9] = translatedX * frame->t248 + translatedY * frame->t24c + translatedZ * frame->t250;
    matrix[10] = translatedX * frame->t254 + translatedZ * frame->t25c + translatedY * frame->t258;
    matrix[11] = translatedX * frame->t22c + translatedZ * frame->t244 + translatedY * frame->t238;
    alternateMesh = (SpriteLightingAlternateObject_guess *)sprite->alternateMesh;
    if (alternateMesh == 0)
    {
        EQGFX_TRANSFORM_VERTICES_BY_MATRIX_GUESS(geometry->vertices, (float *)frame->t610, matrix, geometry->vertexCount, (char *)frame->t61c);
    }
    else
    {
        alternateResource = alternateMesh->resource;
        alternateGeometry = alternateResource->geometry;
        transformPackedVerticesAf14_guess(alternateGeometry->vertices + alternateMesh->groupIndex * (unsigned int)alternateGeometry->vertexCount * 3, (float *)frame->t610, matrix, geometry->vertexCount, (char *)frame->t61c);
    }
    return;
}

// FUNCTION: EQGFX 0x1003b1af
// Builds the 12-float stage matrix from basis and frame, then draws it.
void __cdecl transformVerticesWithBasis5c_guess(int holder, int basis, float *shift, int frame)
{
    Basis1af_guess *rows;
    Frame1af_guess *stage;
    float xfer[12];
    float sumX;
    float sumY;
    float sumZ;
    int *slot;

    rows = (Basis1af_guess *)basis;
    stage = (Frame1af_guess *)frame;
    xfer[0] = rows->f5c * stage->t248 + rows->f64 * stage->t250 + rows->f60 * stage->t24c;
    xfer[1] = stage->t258 * rows->f60 + stage->t25c * rows->f64 + rows->f5c * stage->t254;
    xfer[2] = stage->t244 * rows->f64 + rows->f5c * stage->t22c + rows->f60 * stage->t238;
    xfer[3] = stage->t24c * rows->f6c + rows->f68 * stage->t248 + stage->t250 * rows->f70;
    xfer[4] = stage->t25c * rows->f70 + stage->t258 * rows->f6c + rows->f68 * stage->t254;
    xfer[5] = stage->t244 * rows->f70 + rows->f68 * stage->t22c + rows->f6c * stage->t238;
    xfer[6] = stage->t250 * rows->f7c + stage->t248 * rows->f74 + rows->f78 * stage->t24c;
    xfer[7] = stage->t258 * rows->f78 + stage->t254 * rows->f74 + stage->t25c * rows->f7c;
    xfer[8] = rows->f78 * stage->t238 + stage->t22c * rows->f74 + stage->t244 * rows->f7c;
    sumX = stage->t1ac + shift[0];
    sumY = stage->t1b0 + shift[1];
    sumZ = stage->t1b4 + shift[2];
    xfer[9] = sumX * stage->t248 + sumZ * stage->t250 + sumY * stage->t24c;
    xfer[10] = sumY * stage->t258 + sumZ * stage->t25c + sumX * stage->t254;
    xfer[11] = sumZ * stage->t244 + sumX * stage->t22c + sumY * stage->t238;
    slot = ((Middle1af_guess *)((Outer1af_guess *)holder)->middle)->slot;
    g_draw1af_guess(slot[1], stage->t610, xfer, slot[0], stage->t61c);
}

// FUNCTION: EQGFX 0x1003b392
// Updates a model's mesh vertices from its scaled scene pose.
// Builds transformVerticesWithBasis5c_guess's stage matrix with its 3x3 part scaled by the slot's table entry, then transforms the slot's vertices.
void __cdecl transformVerticesWithScaledBasis_guess(int holder, int basis, float *shift, int frame)
{
    Middle1af_guess *middle;
    Basis1af_guess *rows;
    Frame1af_guess *stage;
    Slot1af_guess *vertSlot;
    float xfer[12];
    float scale;
    float sumX;
    float sumY;
    float sumZ;

    rows = (Basis1af_guess *)basis;
    middle = (Middle1af_guess *)((Outer1af_guess *)holder)->middle;
    scale = g_scaleTable_guess[middle->vertSlot->scaleIndex];
    stage = (Frame1af_guess *)frame;
    xfer[0] = (stage->t250 * rows->f64 + stage->t24c * rows->f60 + rows->f5c * stage->t248) * scale;
    xfer[1] = (stage->t258 * rows->f60 + stage->t25c * rows->f64 + rows->f5c * stage->t254) * scale;
    xfer[2] = (rows->f5c * stage->t22c + stage->t238 * rows->f60 + stage->t244 * rows->f64) * scale;
    xfer[3] = (stage->t250 * rows->f70 + rows->f68 * stage->t248 + stage->t24c * rows->f6c) * scale;
    xfer[4] = (stage->t25c * rows->f70 + stage->t258 * rows->f6c + rows->f68 * stage->t254) * scale;
    xfer[5] = (rows->f68 * stage->t22c + stage->t244 * rows->f70 + stage->t238 * rows->f6c) * scale;
    xfer[6] = (stage->t24c * rows->f78 + rows->f74 * stage->t248 + stage->t250 * rows->f7c) * scale;
    xfer[7] = (rows->f7c * stage->t25c + stage->t258 * rows->f78 + rows->f74 * stage->t254) * scale;
    xfer[8] = (stage->t238 * rows->f78 + rows->f74 * stage->t22c + rows->f7c * stage->t244) * scale;
    sumX = stage->t1ac + shift[0];
    sumY = stage->t1b0 + shift[1];
    sumZ = stage->t1b4 + shift[2];
    xfer[9] = sumZ * stage->t250 + sumY * stage->t24c + sumX * stage->t248;
    xfer[10] = sumY * stage->t258 + sumZ * stage->t25c + sumX * stage->t254;
    xfer[11] = sumY * stage->t238 + sumZ * stage->t244 + sumX * stage->t22c;
    vertSlot = middle->vertSlot;
    transformVerticesByMatrix_guess(vertSlot->verts, stage->t610, xfer, vertSlot->count, stage->t61c);
}

// FUNCTION: EQGFX 0x1003b59c
// Advances each queued 0x1e/0x1f entry's animation frame from the clock at owner+0x184.
void __cdecl updateAnimationFrames_guess(int animOwner)
{
    int **cursor;
    int **fence;
    unsigned int elapsed;
    int *entry;
    int anim;
    unsigned int interval;
    unsigned int steps;

    cursor = *(int ***)(animOwner + 0x2ac);
    fence = cursor + *(int *)(animOwner + 0x2a8);
    if (cursor >= fence)
        return;
    for (; cursor < fence; ++cursor)
    {
        entry = *cursor;
        if (*entry == 0x1e)
            anim = entry[4];
        else if (*entry == 0x1f)
            anim = entry[5];
        else
            continue;
        if (anim == 0)
            continue;
        if (((FxLink51c_guess*)anim)->holder_int_guess == 0 || *(unsigned int *)(((FxLink51c_guess*)anim)->holder_int_guess + 0x14) <= 1)
        {
            ((FxLink51c_guess*)anim)->index_int_guess = 0;
            continue;
        }
        if (((FxLink51c_guess*)anim)->nextFrameTime_guess >= *(unsigned int *)(animOwner + 0x184))
            continue;
        if ((((FxLink51c_guess*)anim)->animationFlags_guess & 8) != 0)
            continue;
        interval = ((FxLink51c_guess*)anim)->frameInterval_guess;
        elapsed = *(unsigned int *)(animOwner + 0x184) - ((FxLink51c_guess*)anim)->nextFrameTime_guess;
        if (elapsed > (interval << 4))
        {
            steps = elapsed / interval;
            ((FxLink51c_guess*)anim)->index += steps + 1;
            ((FxLink51c_guess*)anim)->index %= *(unsigned int *)(((FxLink51c_guess*)anim)->holder_int_guess + 0x14);
            elapsed -= steps * interval;
        }
        else
        {
            for (; elapsed > interval; elapsed -= interval)
            {
                ++((FxLink51c_guess*)anim)->index;
                if (((FxLink51c_guess*)anim)->index >= *(unsigned int *)(((FxLink51c_guess*)anim)->holder_int_guess + 0x14))
                    ((FxLink51c_guess*)anim)->index_int_guess = 0;
            }
            ++((FxLink51c_guess*)anim)->index;
            if (((FxLink51c_guess*)anim)->index >= *(unsigned int *)(((FxLink51c_guess*)anim)->holder_int_guess + 0x14))
                ((FxLink51c_guess*)anim)->index_int_guess = 0;
        }
        ((FxLink51c_guess*)anim)->nextFrameTime_guess = (interval - elapsed) + *(unsigned int *)(animOwner + 0x184);
    }
}

// FUNCTION: EQGFX 0x1003b69b
// Submits each visible triangle of a lit mesh per material, adding the material's UV offset when textured
// @early-stop: vertex-copy ordering and stack/register allocation still differ.
/* fake match: paired offsets shape the frame; goto shares state restoration. */
int __cdecl submitMeshTrianglesByMaterial_guess(int ignoredCallerValue, RendererBufferState_guess *renderer, RendererDrawRequest_guess *request)
{
    unsigned int *positionVertices;
    unsigned int *lightVertices;
    RendererMeshState_guess *meshState;
    RendererPrimitiveState_guess *primitiveState;
    RendererMaterialEntry_guess **materialEntries;
    RendererMaterialEntry_guess *material;
    unsigned short *stripCursor;
    unsigned short *stripEnd;
    unsigned char *stripRecord;
    unsigned char *stripRecordEnd;
    char *markerCursor;
    unsigned int materialFlags;
    unsigned int firstIndex;
    unsigned int secondIndex;
    unsigned int thirdIndex;
    unsigned int *outputWords;
    float *outputValues;
    float *textureCoordinates;
    unsigned int drawFlags;
    RendererPrimitiveDescription_guess description;
    unsigned int savedPrimitiveState;
    int result;

    savedPrimitiveState = renderer->savedPrimitiveState;
    positionVertices = (unsigned int *)renderer->buffer12A;
    lightVertices = (unsigned int *)renderer->buffer12B;
    primitiveState = request->primitiveState;
    result = 1;
    meshState = request->meshState;
    materialEntries = meshState->materialEntries;
    textureCoordinates = primitiveState->materials->textureCoordinates;
    prepareRendererPrimitive_guess((int)meshState, (int)renderer);
    if (renderer->primitiveCapacity < 9)
    {
        freeRenderBuffersByAddress_guess((int)renderer);
        result = allocateRendererBuffers_guess((int)renderer, 0x19);
        if (result < 0)
            goto restore_state;
    }
    stripCursor = primitiveState->materials->stripIndices;
    stripEnd = stripCursor + primitiveState->materials->stripCount * 3;
    if (stripCursor < stripEnd)
    {
        do
        {
            material = materialEntries[stripCursor[2]];
            materialFlags = material->flags;
            if ((materialFlags & RENDER_INDEX_REMAP_FLAG) != 0)
            {
                materialFlags = renderer->indexRemap[materialFlags & 0x7fffffffU];
            }
            if (materialFlags != 0)
            {
                stripRecord = primitiveState->materials->stripRecords +
                    (unsigned int)*stripCursor * RENDER_STRIP_RECORD_BYTES;
                markerCursor = (char *)renderer->bufferSecondary + *stripCursor;
                stripRecordEnd = stripRecord + (unsigned int)stripCursor[1] * RENDER_STRIP_RECORD_BYTES;
                if ((materialFlags & 0xff00U) != 0)
                {
                    float textureOffsets[2];
                    textureOffsets[0] = material->textureUOffset;
                    textureOffsets[1] = material->textureVOffset;
                    drawFlags = materialFlags & 0xffdfffbf | 0x40000000;
                    description.words[0] = 0x37;
                    description.textureValue = material->textureValue;
                    description.diffuseValue = material->diffuseValue;
                    description.renderState = 0;
                    for (; stripRecord < stripRecordEnd;
                        stripRecord += RENDER_STRIP_RECORD_BYTES)
                    {
                        if (*markerCursor != '\0')
                        {
                            secondIndex = *(unsigned short *)(stripRecord + 0xc);
                            firstIndex = *(unsigned short *)(stripRecord + 0xa);
                            thirdIndex = *(unsigned short *)(stripRecord + 0xe);
                            outputWords = (unsigned int *)renderer->primitiveVertices;
                            outputValues = (float *)renderer->primitiveVertices;
                            if (*markerCursor == RENDER_REVERSED_VERTEX_MARKER)
                            {
                                outputWords[0] = positionVertices[firstIndex * 3];
                                outputWords[1] = positionVertices[firstIndex * 3 + 1];
                                outputWords[2] = positionVertices[firstIndex * 3 + 2];
                                outputValues[4] = textureOffsets[0] + textureCoordinates[firstIndex * 2];
                                outputValues[5] = textureOffsets[1] + textureCoordinates[firstIndex * 2 + 1];
                                outputWords[9] = lightVertices[firstIndex * 3];
                                outputWords[10] = lightVertices[firstIndex * 3 + 1];
                                outputWords[0xb] = lightVertices[firstIndex * 3 + 2];
                                outputWords[0xc] = positionVertices[secondIndex * 3];
                                outputWords[0xd] = positionVertices[secondIndex * 3 + 1];
                                outputWords[0xe] = positionVertices[secondIndex * 3 + 2];
                                outputValues[0x10] = textureOffsets[0] + textureCoordinates[secondIndex * 2];
                                outputValues[0x11] = textureOffsets[1] + textureCoordinates[secondIndex * 2 + 1];
                                outputWords[0x15] = lightVertices[secondIndex * 3];
                                outputWords[0x16] = lightVertices[secondIndex * 3 + 1];
                                outputWords[0x17] = lightVertices[secondIndex * 3 + 2];
                                outputWords[0x18] = positionVertices[thirdIndex * 3];
                                outputWords[0x19] = positionVertices[thirdIndex * 3 + 1];
                                outputWords[0x1a] = positionVertices[thirdIndex * 3 + 2];
                                outputValues[0x1c] = textureOffsets[0] + textureCoordinates[thirdIndex * 2];
                                outputValues[0x1d] = textureOffsets[1] + textureCoordinates[thirdIndex * 2 + 1];
                                outputWords[0x21] = lightVertices[thirdIndex * 3];
                                outputWords[0x22] = lightVertices[thirdIndex * 3 + 1];
                                outputWords[0x23] = lightVertices[thirdIndex * 3 + 2];
                            }
                            else
                            {
                                outputWords[0] = positionVertices[thirdIndex * 3];
                                outputWords[1] = positionVertices[thirdIndex * 3 + 1];
                                outputWords[2] = positionVertices[thirdIndex * 3 + 2];
                                outputValues[4] = textureOffsets[0] + textureCoordinates[thirdIndex * 2];
                                outputValues[5] = textureOffsets[1] + textureCoordinates[thirdIndex * 2 + 1];
                                outputWords[9] = lightVertices[thirdIndex * 3];
                                outputWords[10] = lightVertices[thirdIndex * 3 + 1];
                                outputWords[0xb] = lightVertices[thirdIndex * 3 + 2];
                                outputWords[0xc] = positionVertices[secondIndex * 3];
                                outputWords[0xd] = positionVertices[secondIndex * 3 + 1];
                                outputWords[0xe] = positionVertices[secondIndex * 3 + 2];
                                outputValues[0x10] = textureOffsets[0] + textureCoordinates[secondIndex * 2];
                                outputValues[0x11] = textureOffsets[1] + textureCoordinates[secondIndex * 2 + 1];
                                outputWords[0x15] = lightVertices[secondIndex * 3];
                                outputWords[0x16] = lightVertices[secondIndex * 3 + 1];
                                outputWords[0x17] = lightVertices[secondIndex * 3 + 2];
                                outputWords[0x18] = positionVertices[firstIndex * 3];
                                outputWords[0x19] = positionVertices[firstIndex * 3 + 1];
                                outputWords[0x1a] = positionVertices[firstIndex * 3 + 2];
                                outputValues[0x1c] = textureOffsets[0] + textureCoordinates[firstIndex * 2];
                                outputValues[0x1d] = textureOffsets[1] + textureCoordinates[firstIndex * 2 + 1];
                                outputWords[0x21] = lightVertices[firstIndex * 3];
                                outputWords[0x22] = lightVertices[firstIndex * 3 + 1];
                                outputWords[0x23] = lightVertices[firstIndex * 3 + 2];
                            }
                            result = submitRendererPrimitive_guess((int)renderer, 0, 3, &description, drawFlags);
                            if (result > 0)
                            {
                                ++renderer->submittedPrimitiveCount;
                            }
                            else if (result < 0)
                                goto restore_state;
                        }
                        ++markerCursor;
                    }
                }
                else
                {
                    description.words[0] = 0x37;
                    drawFlags = materialFlags & 0xffdfffbf | 0x40000000;
                    description.diffuseValue = material->diffuseValue;
                    description.renderState = 0;
                    for (; stripRecord < stripRecordEnd;
                        stripRecord += RENDER_STRIP_RECORD_BYTES)
                    {
                        if (*markerCursor != '\0')
                        {
                            secondIndex = *(unsigned short *)(stripRecord + 0xc);
                            firstIndex = *(unsigned short *)(stripRecord + 0xa);
                            thirdIndex = *(unsigned short *)(stripRecord + 0xe);
                            outputWords = (unsigned int *)renderer->primitiveVertices;
                            if (*markerCursor == RENDER_REVERSED_VERTEX_MARKER)
                            {
                                outputWords[0] = positionVertices[firstIndex * 3];
                                outputWords[1] = positionVertices[firstIndex * 3 + 1];
                                outputWords[2] = positionVertices[firstIndex * 3 + 2];
                                outputWords[9] = lightVertices[firstIndex * 3];
                                outputWords[10] = lightVertices[firstIndex * 3 + 1];
                                outputWords[0xb] = lightVertices[firstIndex * 3 + 2];
                                outputWords[0xc] = positionVertices[secondIndex * 3];
                                outputWords[0xd] = positionVertices[secondIndex * 3 + 1];
                                outputWords[0xe] = positionVertices[secondIndex * 3 + 2];
                                outputWords[0x15] = lightVertices[secondIndex * 3];
                                outputWords[0x16] = lightVertices[secondIndex * 3 + 1];
                                outputWords[0x17] = lightVertices[secondIndex * 3 + 2];
                            }
                            else
                            {
                                outputWords[0] = positionVertices[thirdIndex * 3];
                                outputWords[1] = positionVertices[thirdIndex * 3 + 1];
                                outputWords[2] = positionVertices[thirdIndex * 3 + 2];
                                outputWords[9] = lightVertices[thirdIndex * 3];
                                outputWords[10] = lightVertices[thirdIndex * 3 + 1];
                                outputWords[0xb] = lightVertices[thirdIndex * 3 + 2];
                                outputWords[0xc] = positionVertices[secondIndex * 3];
                                outputWords[0xd] = positionVertices[secondIndex * 3 + 1];
                                outputWords[0xe] = positionVertices[secondIndex * 3 + 2];
                                outputWords[0x15] = lightVertices[secondIndex * 3];
                                outputWords[0x16] = lightVertices[secondIndex * 3 + 1];
                                outputWords[0x17] = lightVertices[secondIndex * 3 + 2];
                                thirdIndex = firstIndex;
                            }
                            outputWords[0x18] = positionVertices[thirdIndex * 3];
                            outputWords[0x19] = positionVertices[thirdIndex * 3 + 1];
                            outputWords[0x1a] = positionVertices[thirdIndex * 3 + 2];
                            outputWords[0x21] = lightVertices[thirdIndex * 3];
                            outputWords[0x22] = lightVertices[thirdIndex * 3 + 1];
                            outputWords[0x23] = lightVertices[thirdIndex * 3 + 2];
                            result = submitRendererPrimitive_guess((int)renderer, 0, 3, &description, drawFlags);
                            if (result > 0)
                            {
                                ++renderer->submittedPrimitiveCount;
                            }
                            else if (result < 0)
                                goto restore_state;
                        }
                        ++markerCursor;
                    }
                }
            }
            stripCursor += 3;
        } while (stripCursor < stripEnd);
    }
restore_state:
    renderer->savedPrimitiveState = savedPrimitiveState;
    return result;
}

// FUNCTION: EQGFX 0x1003bbd9
// Submits a mesh's marked triangles material by material (UVs scrolled if textured), with no clip-code culling
// @early-stop: local slots, textured copy scheduling, and loop/register layout still differ.
/* fake match: plain vertices advance by 0x30 bytes; both error branches share the restore exit. */
int __cdecl s3dSubmitStripTrianglesNoClipTest_guess(int ignoredOwner, RendererBufferState_guess *renderer,
    RendererStripRequest_guess *request, char *materialLookup)
{
    unsigned int *positionVertices;
    unsigned int *lightVertices;
    RendererMaterialEntry_guess **materialEntries;
    RendererMaterialEntry_guess *material;
    unsigned short *groupCursor;
    unsigned short *groupEnd;
    unsigned char *recordCursor;
    unsigned int recordEnd;
    unsigned char *textureVertices;
    unsigned int materialFlags;
    unsigned int firstIndex;
    unsigned int secondIndex;
    unsigned int thirdIndex;
    unsigned int recordCount;
    unsigned int drawFlags;
    unsigned int *outputWords;
    float *outputValues;
    RendererPrimitiveDescription_guess description;
    unsigned int savedPrimitiveState;
    unsigned int result;

    savedPrimitiveState = renderer->savedPrimitiveState;
    positionVertices = (unsigned int *)renderer->buffer12A;
    lightVertices = (unsigned int *)renderer->buffer12B;
    materialEntries = ((RendererStripMaterialLookup_guess *)materialLookup)->materialEntries;
    result = 1;
    textureVertices = request->stripBatch->textureVertices;
    prepareRendererPrimitive_guess((int)materialLookup, (int)renderer);
    if (renderer->primitiveCapacity < 9)
    {
        freeRenderBuffersByAddress_guess((int)renderer);
        result = allocateRendererBuffers_guess((int)renderer, 0x19);
        if ((int)result < 0)
            goto restore_state;
    }
    groupCursor = request->stripBatch->stripGroups;
    groupEnd = groupCursor + (unsigned int)request->stripBatch->stripCount * 2;
    recordCursor = request->stripBatch->stripRecords;
    materialLookup = (char *)renderer->bufferSecondary;
    if (groupCursor < groupEnd)
    {
        do
        {
            material = materialEntries[groupCursor[1]];
            materialFlags = material->flags;
            if ((materialFlags & RENDER_INDEX_REMAP_FLAG) != 0)
                materialFlags = renderer->indexRemap[materialFlags & 0x7fffffffU];
            if (materialFlags == 0)
            {
                recordCount = groupCursor[0];
                materialLookup += recordCount;
                recordCursor += recordCount * 8;
            }
            else
            {
                recordEnd = (unsigned int)recordCursor + (unsigned int)groupCursor[0] * 8;
                drawFlags = (materialFlags & 0xffdfffbfU) | 0x40000000U;
                if ((materialFlags & 0xff00U) != 0)
                {
                    float textureUOffset;
                    float textureVOffset;
                    textureUOffset = material->textureUOffset;
                    textureVOffset = material->textureVOffset;
                    description.words[0] = 0x37;
                    description.textureValue = material->textureValue;
                    description.diffuseValue = material->diffuseValue;
                    description.renderState = 0;
                    while ((unsigned int)recordCursor < recordEnd)
                    {
                        if (*materialLookup != 0)
                        {
                            thirdIndex = *(unsigned short *)(recordCursor + 6);
                            firstIndex = *(unsigned short *)(recordCursor + 2);
                            secondIndex = *(unsigned short *)(recordCursor + 4);
                            outputWords = (unsigned int *)renderer->primitiveVertices;
                            outputValues = (float *)outputWords;
                            if (*materialLookup == RENDER_REVERSED_VERTEX_MARKER)
                            {
                                outputWords[0] = positionVertices[firstIndex * 3];
                                outputWords[1] = positionVertices[firstIndex * 3 + 1];
                                outputWords[2] = positionVertices[firstIndex * 3 + 2];
                                outputValues[4] = textureUOffset + *(float *)(textureVertices + firstIndex * 0x14 + 6);
                                outputValues[5] = textureVOffset + *(float *)(textureVertices + firstIndex * 0x14 + 10);
                                outputWords[9] = lightVertices[firstIndex * 3];
                                outputWords[10] = lightVertices[firstIndex * 3 + 1];
                                outputWords[0xb] = lightVertices[firstIndex * 3 + 2];
                                outputWords[0xc] = positionVertices[secondIndex * 3];
                                outputWords[0xd] = positionVertices[secondIndex * 3 + 1];
                                outputWords[0xe] = positionVertices[secondIndex * 3 + 2];
                                outputValues[0x10] = textureUOffset + *(float *)(textureVertices + secondIndex * 0x14 + 6);
                                outputValues[0x11] = textureVOffset + *(float *)(textureVertices + secondIndex * 0x14 + 10);
                                outputWords[0x15] = lightVertices[secondIndex * 3];
                                outputWords[0x16] = lightVertices[secondIndex * 3 + 1];
                                outputWords[0x17] = lightVertices[secondIndex * 3 + 2];
                                outputWords[0x18] = positionVertices[thirdIndex * 3];
                                outputWords[0x19] = positionVertices[thirdIndex * 3 + 1];
                                outputWords[0x1a] = positionVertices[thirdIndex * 3 + 2];
                                outputValues[0x1c] = textureUOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 6);
                                outputValues[0x1d] = textureVOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 10);
                                outputWords[0x21] = lightVertices[thirdIndex * 3];
                                outputWords[0x22] = lightVertices[thirdIndex * 3 + 1];
                                outputWords[0x23] = lightVertices[thirdIndex * 3 + 2];
                            }
                            else
                            {
                                outputWords[0] = positionVertices[thirdIndex * 3];
                                outputWords[1] = positionVertices[thirdIndex * 3 + 1];
                                outputWords[2] = positionVertices[thirdIndex * 3 + 2];
                                outputValues[4] = textureUOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 6);
                                outputValues[5] = textureVOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 10);
                                outputWords[9] = lightVertices[thirdIndex * 3];
                                outputWords[10] = lightVertices[thirdIndex * 3 + 1];
                                outputWords[0xb] = lightVertices[thirdIndex * 3 + 2];
                                outputWords[0xc] = positionVertices[secondIndex * 3];
                                outputWords[0xd] = positionVertices[secondIndex * 3 + 1];
                                outputWords[0xe] = positionVertices[secondIndex * 3 + 2];
                                outputValues[0x10] = textureUOffset + *(float *)(textureVertices + secondIndex * 0x14 + 6);
                                outputValues[0x11] = textureVOffset + *(float *)(textureVertices + secondIndex * 0x14 + 10);
                                outputWords[0x15] = lightVertices[secondIndex * 3];
                                outputWords[0x16] = lightVertices[secondIndex * 3 + 1];
                                outputWords[0x17] = lightVertices[secondIndex * 3 + 2];
                                outputWords[0x18] = positionVertices[firstIndex * 3];
                                outputWords[0x19] = positionVertices[firstIndex * 3 + 1];
                                outputWords[0x1a] = positionVertices[firstIndex * 3 + 2];
                                outputValues[0x1c] = textureUOffset + *(float *)(textureVertices + firstIndex * 0x14 + 6);
                                outputValues[0x1d] = textureVOffset + *(float *)(textureVertices + firstIndex * 0x14 + 10);
                                outputWords[0x21] = lightVertices[firstIndex * 3];
                                outputWords[0x22] = lightVertices[firstIndex * 3 + 1];
                                outputWords[0x23] = lightVertices[firstIndex * 3 + 2];
                            }
                            result = submitRendererPrimitive_guess((int)renderer, 0, 3, &description, drawFlags);
                            if ((int)result > 0)
                            {
                                ++renderer->submittedPrimitiveCount;
                            }
                            else if ((int)result < 0)
                                goto restore_state;
                        }
                        ++materialLookup;
                        recordCursor += 8;
                    }
                }
                else
                {
                    description.words[0] = 0x37;
                    description.renderState = 0;
                    description.diffuseValue = material->diffuseValue;
                    if ((unsigned int)recordCursor < recordEnd)
                    {
                        do
                        {
                            if (*materialLookup != 0)
                            {
                                thirdIndex = *(unsigned short *)(recordCursor + 6);
                                firstIndex = *(unsigned short *)(recordCursor + 2);
                                secondIndex = *(unsigned short *)(recordCursor + 4);
                                outputWords = (unsigned int *)renderer->primitiveVertices;
                                if (*materialLookup == RENDER_REVERSED_VERTEX_MARKER)
                                {
                                    outputWords[0] = positionVertices[firstIndex * 3];
                                    outputWords[1] = positionVertices[firstIndex * 3 + 1];
                                    outputWords[2] = positionVertices[firstIndex * 3 + 2];
                                    outputWords[9] = lightVertices[firstIndex * 3];
                                    outputWords[10] = lightVertices[firstIndex * 3 + 1];
                                    outputWords[0xb] = lightVertices[firstIndex * 3 + 2];
                                    outputWords += 0xc;
                                    outputWords[0] = positionVertices[secondIndex * 3];
                                    outputWords[1] = positionVertices[secondIndex * 3 + 1];
                                    outputWords[2] = positionVertices[secondIndex * 3 + 2];
                                    outputWords[9] = lightVertices[secondIndex * 3];
                                    outputWords[10] = lightVertices[secondIndex * 3 + 1];
                                    outputWords[0xb] = lightVertices[secondIndex * 3 + 2];
                                    outputWords += 0xc;
                                    outputWords[0] = positionVertices[thirdIndex * 3];
                                    outputWords[1] = positionVertices[thirdIndex * 3 + 1];
                                    outputWords[2] = positionVertices[thirdIndex * 3 + 2];
                                    outputWords[9] = lightVertices[thirdIndex * 3];
                                    outputWords[10] = lightVertices[thirdIndex * 3 + 1];
                                    outputWords[0xb] = lightVertices[thirdIndex * 3 + 2];
                                }
                                else
                                {
                                    outputWords[0] = positionVertices[thirdIndex * 3];
                                    outputWords[1] = positionVertices[thirdIndex * 3 + 1];
                                    outputWords[2] = positionVertices[thirdIndex * 3 + 2];
                                    outputWords[9] = lightVertices[thirdIndex * 3];
                                    outputWords[10] = lightVertices[thirdIndex * 3 + 1];
                                    outputWords[0xb] = lightVertices[thirdIndex * 3 + 2];
                                    outputWords += 0xc;
                                    outputWords[0] = positionVertices[secondIndex * 3];
                                    outputWords[1] = positionVertices[secondIndex * 3 + 1];
                                    outputWords[2] = positionVertices[secondIndex * 3 + 2];
                                    outputWords[9] = lightVertices[secondIndex * 3];
                                    outputWords[10] = lightVertices[secondIndex * 3 + 1];
                                    outputWords[0xb] = lightVertices[secondIndex * 3 + 2];
                                    outputWords += 0xc;
                                    outputWords[0] = positionVertices[firstIndex * 3];
                                    outputWords[1] = positionVertices[firstIndex * 3 + 1];
                                    outputWords[2] = positionVertices[firstIndex * 3 + 2];
                                    outputWords[9] = lightVertices[firstIndex * 3];
                                    outputWords[10] = lightVertices[firstIndex * 3 + 1];
                                    outputWords[0xb] = lightVertices[firstIndex * 3 + 2];
                                }
                                result = submitRendererPrimitive_guess((int)renderer, 0, 3, &description, drawFlags);
                            if ((int)result > 0)
                                {
                                    ++renderer->submittedPrimitiveCount;
                                }
                            else if ((int)result < 0)
                                goto restore_state;
                            }
                            ++materialLookup;
                            recordCursor += 8;
                        } while ((unsigned int)recordCursor < recordEnd);
                    }
                }
            }
            groupCursor += 2;
        } while (groupCursor < groupEnd);
    }
restore_state:
    renderer->savedPrimitiveState = savedPrimitiveState;
    return result;
}

// FUNCTION: EQGFX 0x1003c119
// Submits a mesh's marked triangles per material, dropping ones all outside one clip plane; forwards clip codes
// @early-stop: prologue, stack-slot selection, and vertex-copy order still differ.
/* fake match: Both negative paths share the original state-restore exit. */
int __cdecl s3dSubmitStripTrianglesClipTested_guess(int ignoredOwner, RendererBufferState_guess *renderer,
    unsigned int request, char *materialLookup)
{
    unsigned int *positionVertices;
    unsigned int *lightVertices;
    RendererStripBatch_guess *stripBatch;
    RendererMaterialEntry_guess **materialEntries;
    RendererMaterialEntry_guess *material;
    unsigned short *groupCursor;
    unsigned short *groupEnd;
    unsigned int recordEnd;
    unsigned char *vertexMarks;
    unsigned int materialFlags;
    unsigned int firstIndex;
    unsigned int secondIndex;
    unsigned int thirdIndex;
    unsigned int vertexOffset;
    unsigned int recordCount;
    unsigned int drawFlags;
    unsigned int *outputWords;
    float *outputValues;
    unsigned char *textureVertices;
    RendererStripPrimitiveDescription_guess description;
    float textureUOffset;
    float textureVOffset;
    unsigned int savedPrimitiveState;
    int result;

    savedPrimitiveState = renderer->savedPrimitiveState;
    positionVertices = (unsigned int *)renderer->buffer12A;
    lightVertices = (unsigned int *)renderer->buffer12B;
    materialEntries = ((RendererStripMaterialLookup_guess *)materialLookup)->materialEntries;
    result = 1;
    textureVertices = ((RendererStripRequest_guess *)request)->stripBatch->textureVertices;
    vertexMarks = (unsigned char *)renderer->bufferBytesB;
    prepareRendererPrimitive_guess((int)materialLookup, (int)renderer);
    if (renderer->primitiveCapacity < 9)
    {
        freeRenderBuffersByAddress_guess((int)renderer);
        result = allocateRendererBuffers_guess((int)renderer, 0x19);
        if (result < 0)
            goto restore_state;
    }
    stripBatch = ((RendererStripRequest_guess *)request)->stripBatch;
    groupCursor = stripBatch->stripGroups;
    groupEnd = groupCursor + (unsigned int)stripBatch->stripCount * 2;
    request = (unsigned int)stripBatch->stripRecords;
    materialLookup = (char *)renderer->bufferSecondary;
    if (groupCursor < groupEnd)
    {
        do
        {
            material = materialEntries[groupCursor[1]];
            materialFlags = material->flags;
            if ((materialFlags & RENDER_INDEX_REMAP_FLAG) != 0)
                materialFlags = renderer->indexRemap[materialFlags & 0x7fffffffU];
            if (materialFlags == 0)
            {
                recordCount = groupCursor[0];
                materialLookup += recordCount;
                request += recordCount * 8;
            }
            else
            {
                recordEnd = request + (unsigned int)groupCursor[0] * 8;
                drawFlags = (materialFlags & 0xffdfffbfU) | 0x40000000U;
                if ((char)(materialFlags >> 8) == '\0')
                {
                    description.primitiveType = 0x37;
                    description.materialDiffuse = material->diffuseValue;
                    description.vertexState = 0;
                    if (request < recordEnd)
                    {
                        do
                        {
                            if (*materialLookup != 0)
                            {
                                thirdIndex = *(unsigned short *)(request + 6);
                                firstIndex = *(unsigned short *)(request + 2);
                                secondIndex = *(unsigned short *)(request + 4);
                                if ((vertexMarks[thirdIndex] & vertexMarks[secondIndex] & vertexMarks[firstIndex]) == 0)
                                {
                                    outputWords = (unsigned int *)renderer->primitiveVertices;
                                    if (*materialLookup == 1)
                                    {
                                        vertexOffset = firstIndex * 0xc;
                                        outputWords[0] = positionVertices[vertexOffset / 4];
                                        outputWords[1] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[2] = positionVertices[vertexOffset / 4 + 2];
                                        outputWords[9] = lightVertices[vertexOffset / 4];
                                        outputWords[10] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0xb] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = secondIndex * 0xc;
                                        outputWords[0xc] = positionVertices[vertexOffset / 4];
                                        outputWords[0xd] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0xe] = positionVertices[vertexOffset / 4 + 2];
                                        outputWords[0x15] = lightVertices[vertexOffset / 4];
                                        outputWords[0x16] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x17] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = thirdIndex * 0xc;
                                        outputWords[0x18] = positionVertices[vertexOffset / 4];
                                        outputWords[0x19] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0x1a] = positionVertices[vertexOffset / 4 + 2];
                                        outputWords[0x21] = lightVertices[vertexOffset / 4];
                                        outputWords[0x22] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x23] = lightVertices[vertexOffset / 4 + 2];
                                    }
                                    else
                                    {
                                        vertexOffset = thirdIndex * 0xc;
                                        outputWords[0] = positionVertices[vertexOffset / 4];
                                        outputWords[1] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[2] = positionVertices[vertexOffset / 4 + 2];
                                        outputWords[9] = lightVertices[vertexOffset / 4];
                                        outputWords[10] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0xb] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = secondIndex * 0xc;
                                        outputWords[0xc] = positionVertices[vertexOffset / 4];
                                        outputWords[0xd] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0xe] = positionVertices[vertexOffset / 4 + 2];
                                        outputWords[0x15] = lightVertices[vertexOffset / 4];
                                        outputWords[0x16] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x17] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = firstIndex * 0xc;
                                        outputWords[0x18] = positionVertices[vertexOffset / 4];
                                        outputWords[0x19] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0x1a] = positionVertices[vertexOffset / 4 + 2];
                                        outputWords[0x21] = lightVertices[vertexOffset / 4];
                                        outputWords[0x22] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x23] = lightVertices[vertexOffset / 4 + 2];
                                    }
                                    renderer->vertexClipFlags = (unsigned char)(vertexMarks[firstIndex] | vertexMarks[secondIndex] | vertexMarks[thirdIndex]);
                                    result = submitRendererPrimitive_guess((int)renderer, 0, 3, &description, drawFlags);
                                    if (result > 0)
                                    {
                                        ++renderer->submittedPrimitiveCount;
                                    }
                                    else if (result < 0)
                                        goto restore_state;
                                }
                            }
                            ++materialLookup;
                            request += 8;
                        } while (request < recordEnd);
                    }
                }
                else
                {
                    textureUOffset = material->textureUOffset;
                    textureVOffset = material->textureVOffset;
                    description.primitiveType = 0x37;
                    description.materialTexture = material->textureValue;
                    description.materialDiffuse = material->diffuseValue;
                    description.vertexState = 0;
                    if (request < recordEnd)
                    {
                        do
                        {
                            if (*materialLookup != 0)
                            {
                                thirdIndex = *(unsigned short *)(request + 6);
                                firstIndex = *(unsigned short *)(request + 2);
                                secondIndex = *(unsigned short *)(request + 4);
                            if ((vertexMarks[thirdIndex] & vertexMarks[secondIndex] & vertexMarks[firstIndex]) == 0)
                                {
                                    outputWords = (unsigned int *)renderer->primitiveVertices;
                                    outputValues = (float *)outputWords;
                                    if (*materialLookup == 1)
                                    {
                                        vertexOffset = firstIndex * 0xc;
                                        outputWords[0] = positionVertices[vertexOffset / 4];
                                        outputWords[1] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[2] = positionVertices[vertexOffset / 4 + 2];
                                        outputValues[4] = textureUOffset + *(float *)(textureVertices + firstIndex * 0x14 + 6);
                                        outputValues[5] = textureVOffset + *(float *)(textureVertices + firstIndex * 0x14 + 10);
                                        outputWords[9] = lightVertices[vertexOffset / 4];
                                        outputWords[10] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0xb] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = secondIndex * 0xc;
                                        outputWords[0xc] = positionVertices[vertexOffset / 4];
                                        outputWords[0xd] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0xe] = positionVertices[vertexOffset / 4 + 2];
                                        outputValues[0x10] = textureUOffset + *(float *)(textureVertices + secondIndex * 0x14 + 6);
                                        outputValues[0x11] = textureVOffset + *(float *)(textureVertices + secondIndex * 0x14 + 10);
                                        outputWords[0x15] = lightVertices[vertexOffset / 4];
                                        outputWords[0x16] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x17] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = thirdIndex * 0xc;
                                        outputWords[0x18] = positionVertices[vertexOffset / 4];
                                        outputWords[0x19] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0x1a] = positionVertices[vertexOffset / 4 + 2];
                                        outputValues[0x1c] = textureUOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 6);
                                        outputValues[0x1d] = textureVOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 10);
                                        outputWords[0x21] = lightVertices[vertexOffset / 4];
                                        outputWords[0x22] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x23] = lightVertices[vertexOffset / 4 + 2];
                                    }
                                    else
                                    {
                                        vertexOffset = thirdIndex * 0xc;
                                        outputWords[0] = positionVertices[vertexOffset / 4];
                                        outputWords[1] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[2] = positionVertices[vertexOffset / 4 + 2];
                                        outputValues[4] = textureUOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 6);
                                        outputValues[5] = textureVOffset + *(float *)(textureVertices + thirdIndex * 0x14 + 10);
                                        outputWords[9] = lightVertices[vertexOffset / 4];
                                        outputWords[10] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0xb] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = secondIndex * 0xc;
                                        outputWords[0xc] = positionVertices[vertexOffset / 4];
                                        outputWords[0xd] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0xe] = positionVertices[vertexOffset / 4 + 2];
                                        outputValues[0x10] = textureUOffset + *(float *)(textureVertices + secondIndex * 0x14 + 6);
                                        outputValues[0x11] = textureVOffset + *(float *)(textureVertices + secondIndex * 0x14 + 10);
                                        outputWords[0x15] = lightVertices[vertexOffset / 4];
                                        outputWords[0x16] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x17] = lightVertices[vertexOffset / 4 + 2];
                                        vertexOffset = firstIndex * 0xc;
                                        outputWords[0x18] = positionVertices[vertexOffset / 4];
                                        outputWords[0x19] = positionVertices[vertexOffset / 4 + 1];
                                        outputWords[0x1a] = positionVertices[vertexOffset / 4 + 2];
                                        outputValues[0x1c] = textureUOffset + *(float *)(textureVertices + firstIndex * 0x14 + 6);
                                        outputValues[0x1d] = textureVOffset + *(float *)(textureVertices + firstIndex * 0x14 + 10);
                                        outputWords[0x21] = lightVertices[vertexOffset / 4];
                                        outputWords[0x22] = lightVertices[vertexOffset / 4 + 1];
                                        outputWords[0x23] = lightVertices[vertexOffset / 4 + 2];
                                    }
                                    renderer->vertexClipFlags = (unsigned char)(vertexMarks[firstIndex] | vertexMarks[secondIndex] | vertexMarks[thirdIndex]);
                                    result = submitRendererPrimitive_guess((int)renderer, 0, 3, &description, drawFlags);
                                    if (result > 0)
                                    {
                                        ++renderer->submittedPrimitiveCount;
                                    }
                                    else if (result < 0)
                                        goto restore_state;
                                }
                            }
                        ++materialLookup;
                        request += 8;
                        } while (request < recordEnd);
                    }
                }
            }
            groupCursor += 2;
        } while (groupCursor < groupEnd);
    }
restore_state:
    renderer->savedPrimitiveState = savedPrimitiveState;
    return result;
}

// FUNCTION: EQGFX 0x1003c6fa
// Marks front-facing tris and their verts by testing each tri against the viewer.
int __cdecl classifyGroupedTriangleVisibility_guess(int owner, int frame)
{
    int middle;
    int mesh;
    int matBase;
    unsigned short *groupCur;
    unsigned short *groupEnd;
    float *verts;
    unsigned char *vertMark;
    unsigned char *faceCur;
    unsigned short *tri;
    unsigned int count;
    int mat;
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
    float *p0;
    float *p1;
    float *p2;

    middle = *(int *)(owner + 0x34);
    mesh = *(int *)(middle + 0x14);
    matBase = *(int *)(*(int *)(owner + 0x20) + 0x18);
    groupCur = *(unsigned short **)(mesh + 0x50);
    verts = *(float **)(frame + 0x610);
    vertMark = *(unsigned char **)(frame + 0x61c);
    groupEnd = groupCur + *(unsigned int *)(mesh + 0x4c) * 3;
    memset(vertMark, 0, *(unsigned int *)mesh);
    memset(*(unsigned char **)(frame + 0x618), 0, *(unsigned int *)(mesh + 0x20));
    if (groupCur >= groupEnd)
        return 1;
    for (; groupCur < groupEnd; groupCur += 3)
    {
        tri = (unsigned short *)(*(int *)(mesh + 0x24) + (unsigned int)*groupCur * 0x10 + 10);
        faceCur = *(unsigned char **)(frame + 0x618) + *groupCur;
        count = (unsigned int)groupCur[1];
        mat = *(int *)(matBase + (unsigned int)groupCur[2] * 4);
        if (*(int *)(mat + 0x20) == 0)
        {
            if (count != 0)
            {
                do
                {
                    if ((tri[-1] & 1) == 0)
                    {
                        v0 = (unsigned int)tri[0];
                        v1 = (unsigned int)tri[1];
                        v2 = (unsigned int)tri[2];
                        p1 = verts + v1 * 3;
                        p0 = verts + v0 * 3;
                        p2 = verts + v2 * 3;
                        if (-(((p2[2] - p1[2]) * (p1[1] - p0[1]) - (p2[1] - p1[1]) * (p1[2] - p0[2])) * p0[0] - ((p2[2] - p1[2]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[2] - p0[2])) * p0[1] + ((p2[1] - p1[1]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[1] - p0[1])) * p0[2]) > EQGFX_ZERO_F_GUESS)
                        {
                            *faceCur = 1;
                            vertMark[v0] = 1;
                            vertMark[v1] = 1;
                            vertMark[v2] = 1;
                        }
                    }
                    ++faceCur;
                    tri += 8;
                    --count;
                } while (count != 0);
            }
        }
        else
        {
            if (count != 0)
            {
                do
                {
                    if ((tri[-1] & 1) == 0)
                    {
                        v0 = (unsigned int)tri[0];
                        v1 = (unsigned int)tri[1];
                        v2 = (unsigned int)tri[2];
                        p1 = verts + v1 * 3;
                        p0 = verts + v0 * 3;
                        p2 = verts + v2 * 3;
                        if (-(((p2[2] - p1[2]) * (p1[1] - p0[1]) - (p2[1] - p1[1]) * (p1[2] - p0[2])) * p0[0] - ((p2[2] - p1[2]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[2] - p0[2])) * p0[1] + ((p2[1] - p1[1]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[1] - p0[1])) * p0[2]) > EQGFX_ZERO_F_GUESS)
                            *faceCur = 1;
                        else
                            *faceCur = 2;
                        vertMark[v0] = 1;
                        vertMark[v1] = 1;
                        vertMark[v2] = 1;
                    }
                    ++faceCur;
                    tri += 8;
                    --count;
                } while (count != 0);
            }
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x1003c99d
// Marks front-facing tris and their verts by testing each tri against the viewer.
int __cdecl classifyRequiredTriangleVisibility_guess(int owner, int frame)
{
    int middle;
    int mesh;
    int matBase;
    unsigned short *groupCur;
    unsigned short *groupEnd;
    float *verts;
    unsigned char *vertMark;
    unsigned char *faceCur;
    char *need;
    unsigned short *tri;
    unsigned int count;
    int mat;
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
    float *p0;
    float *p1;
    float *p2;

    middle = *(int *)(owner + 0x34);
    mesh = *(int *)(middle + 0x18);
    matBase = *(int *)(*(int *)(owner + 0x20) + 0x18);
    groupCur = *(unsigned short **)(mesh + 0x20);
    tri = *(unsigned short **)(mesh + 0x18);
    need = *(char **)(mesh + 0x7c);
    verts = *(float **)(frame + 0x610);
    faceCur = *(unsigned char **)(frame + 0x618);
    vertMark = *(unsigned char **)(frame + 0x61c);
    groupEnd = groupCur + (unsigned int)*(unsigned short *)(mesh + 0xc) * 2;
    memset(vertMark, 0, (unsigned int)*(unsigned short *)mesh);
    memset(faceCur, 0, (unsigned int)*(unsigned short *)(mesh + 8));
    if (groupCur >= groupEnd)
        return 1;
    for (; groupCur < groupEnd; groupCur += 2)
    {
        count = (unsigned int)groupCur[0];
        mat = *(int *)(matBase + (unsigned int)groupCur[1] * 4);
        if (*(int *)(mat + 0x20) == 0)
        {
            if (count != 0)
            {
                do
                {
                    if (*need != 0)
                    {
                        v1 = (unsigned int)tri[2];
                        v0 = (unsigned int)tri[1];
                        v2 = (unsigned int)tri[3];
                        p1 = verts + v1 * 3;
                        p0 = verts + v0 * 3;
                        p2 = verts + v2 * 3;
                        if (-(((p2[2] - p1[2]) * (p1[1] - p0[1]) - (p2[1] - p1[1]) * (p1[2] - p0[2])) * p0[0] - ((p2[2] - p1[2]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[2] - p0[2])) * p0[1] + ((p2[1] - p1[1]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[1] - p0[1])) * p0[2]) > EQGFX_ZERO_F_GUESS)
                        {
                            *faceCur = 1;
                            vertMark[v0] = 1;
                            vertMark[v1] = 1;
                            vertMark[v2] = 1;
                        }
                    }
                    ++need;
                    ++faceCur;
                    tri += 4;
                    --count;
                } while (count != 0);
            }
        }
        else
        {
            if (count != 0)
            {
                do
                {
                    if (*need != 0)
                    {
                        v1 = (unsigned int)tri[2];
                        v0 = (unsigned int)tri[1];
                        v2 = (unsigned int)tri[3];
                        p1 = verts + v1 * 3;
                        p0 = verts + v0 * 3;
                        p2 = verts + v2 * 3;
                        if (-(((p2[2] - p1[2]) * (p1[1] - p0[1]) - (p2[1] - p1[1]) * (p1[2] - p0[2])) * p0[0] - ((p2[2] - p1[2]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[2] - p0[2])) * p0[1] + ((p2[1] - p1[1]) * (p1[0] - p0[0]) - (p2[0] - p1[0]) * (p1[1] - p0[1])) * p0[2]) > EQGFX_ZERO_F_GUESS)
                            *faceCur = 1;
                        else
                            *faceCur = 2;
                        vertMark[v0] = 1;
                        vertMark[v1] = 1;
                        vertMark[v2] = 1;
                    }
                    ++need;
                    ++faceCur;
                    tri += 4;
                    --count;
                } while (count != 0);
            }
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x1003cc23
// Marks front-facing planes and their verts by testing each plane equation.
int __cdecl markFrontFacingPlaneFaces_guess(int owner, int frame)
{
    int middle;
    MeshCc23_guess *mesh;
    FrameCd0d_guess *stage;
    unsigned char *vertMark;
    unsigned char *faceCur;
    unsigned char *entry;
    unsigned char *end;
    float *plane;
    float dx;
    float dy;
    float dz;
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;

    middle = ((Outer1af_guess *)owner)->middle;
    stage = (FrameCd0d_guess *)frame;
    vertMark = stage->vertMarks;
    dx = -stage->posX;
    faceCur = stage->faceCodes;
    dy = -stage->posY;
    mesh = (MeshCc23_guess *)((Middle1af_guess *)middle)->slot;
    dz = -stage->posZ;
    entry = mesh->entries;
    plane = mesh->planes;
    end = entry + (unsigned int)mesh->entryCount * 0x10;
    memset(vertMark, 0, mesh->vertMarkSize);
    memset(faceCur, 0, (unsigned int)mesh->entryCount);
    for (; entry < end; ++faceCur, entry += 0x10, plane += 4)
    {
        if ((entry[8] & 1) == 0)
        {
            v2 = (unsigned int)*(unsigned short *)(entry + 0xe);
            v0 = (unsigned int)*(unsigned short *)(entry + 0xa);
            v1 = (unsigned int)*(unsigned short *)(entry + 0xc);
            if (dz * plane[2] + dy * plane[1] + dx * plane[0] + plane[3] > EQGFX_ZERO_F_GUESS)
            {
                *faceCur = 1;
                vertMark[v0] = 1;
                vertMark[v1] = 1;
                vertMark[v2] = 1;
            }
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x1003cd0d
// Marks front-facing planes and their verts by testing each plane equation.
int __cdecl markFrontFacingCenteredPlaneFaces_guess(int owner, int unused, int frame)
{
    unsigned char *needEnd;
    float dz;
    float dy;
    float dx;
    MeshCd0d_guess *mesh;
    FrameCd0d_guess *stage;
    unsigned char *faceCur;
    unsigned char *vertMark;
    unsigned char *needCur;
    float *plane;
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;

    (void)unused;
    stage = (FrameCd0d_guess *)frame;
    mesh = ((OwnerCd0d_guess *)owner)->mesh;
    faceCur = stage->faceCodes;
    vertMark = stage->vertMarks;
    dx = -stage->posX - mesh->centerX;
    dy = -stage->posY - mesh->centerY;
    dz = -stage->posZ - mesh->centerZ;
    needCur = mesh->need;
    plane = mesh->planes;
    needEnd = needCur + (unsigned int)mesh->faceCount * 8;
    memset(vertMark, 0, (unsigned int)mesh->vertCount);
    memset(faceCur, 0, (unsigned int)mesh->faceCount);
    for (; needCur < needEnd; ++faceCur, needCur += 8, plane += 4)
    {
        if ((*needCur & 1) == 0)
        {
            v2 = (unsigned int)*(unsigned short *)(needCur + 6);
            v0 = (unsigned int)*(unsigned short *)(needCur + 2);
            v1 = (unsigned int)*(unsigned short *)(needCur + 4);
            if (dz * plane[2] + dy * plane[1] + dx * plane[0] + plane[3] > EQGFX_ZERO_F_GUESS)
            {
                *faceCur = 1;
                vertMark[v0] = 1;
                vertMark[v1] = 1;
                vertMark[v2] = 1;
            }
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x1003cdf0
// Tests queued vertices against enabled clip planes, accumulating codes.
unsigned int __cdecl classifyQueuedVerticesAgainstClipPlanes_guess(ClipContext_guess *ctx, VertCountRef_guess *mesh)
{
    unsigned int planeMask;
    float *vert;
    float *fence;
    char *activeFlag;
    unsigned char *outCode;
    unsigned int combined;
    unsigned int common;
    unsigned int code;
    unsigned short count;

    combined = 0;
    planeMask = ctx->planeMask;
    vert = ctx->positions;
    activeFlag = ctx->active;
    outCode = ctx->codes;
    count = *mesh->countPtr;
    fence = vert + (unsigned int)count * 3;
    common = planeMask;
    if (vert < fence)
    {
        do
        {
            if (*activeFlag != 0)
            {
                code = 0;
                if ((planeMask & 1) != 0 && vert[2] + vert[1] < EQGFX_ZERO_F_GUESS)
                    code = 1;
                if ((planeMask & 2) != 0 && vert[2] - vert[1] < EQGFX_ZERO_F_GUESS)
                    code |= 2;
                if ((planeMask & 4) != 0 && vert[2] + vert[0] < EQGFX_ZERO_F_GUESS)
                    code |= 4;
                if ((planeMask & 8) != 0 && vert[2] - vert[0] < EQGFX_ZERO_F_GUESS)
                    code |= 8;
                if ((planeMask & 0x10) != 0 && vert[2] - ctx->nearLimit < EQGFX_ZERO_F_GUESS)
                    code |= 0x10;
                if ((planeMask & 0x20) != 0 && ctx->farLimit - vert[2] < EQGFX_ZERO_F_GUESS)
                    code |= 0x20;
                combined |= code;
                common &= code;
                *outCode = (unsigned char)code;
            }
            ++activeFlag;
            vert += 3;
            ++outCode;
        } while (vert < fence);
        if (combined != 0)
        {
            if (common != 0)
                combined = 0xffffffff;
            return combined;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1003cf06
// Syncs queued materials to the clock at owner+0x184, stepping frames and scrolling UVs.
void __cdecl s3dUpdateMaterialPaletteAnimations(RendererScrollList_guess *scrollList, RendererClockOwner_guess *clockOwner)
{
    float deltaTime;
    int changed;
    unsigned int pos;
    RendererScrollMaterial_guess *mat;
    unsigned int flags;
    RendererMaterialAnimation_guess *anim;
    RendererAnimationFrames_guess *frameDef;
    unsigned int interval;
    unsigned int elapsed;
    unsigned int steps;

    if (scrollList->lastClock != clockOwner->clock)
    {
        if (scrollList->dirty == 0)
        {
            scrollList->lastClock = clockOwner->clock;
        }
        else
        {
            deltaTime = (float)(unsigned int)(clockOwner->clock - scrollList->lastClock);
            changed = 0;
            scrollList->lastClock = clockOwner->clock;
            pos = 0;
            if (scrollList->count > 0)
            {
                do
                {
                    mat = scrollList->materials[pos];
                    flags = mat->flags;
                    if ((flags & 0x80000000) != 0)
                        flags = clockOwner->flagMap[flags & 0x7fffffff];
                    if ((flags & 0xff00) != 0)
                    {
                        anim = mat->animation;
                        frameDef = anim->frameDef;
                        if (frameDef != 0 && frameDef->count > 1)
                        {
                            changed = 1;
                            if (anim->nextClock < clockOwner->clock && (anim->control & 8) == 0)
                            {
                                if (anim->mode != 0)
                                {
                                    interval = anim->interval;
                                    elapsed = clockOwner->clock - anim->nextClock;
                                    if (elapsed > (interval << 4))
                                    {
                                        steps = elapsed / interval;
                                        anim->frameIndex += steps + 1;
                                        anim->frameIndex %= frameDef->count;
                                        elapsed -= steps * interval;
                                    }
                                    else
                                    {
                                        for (; elapsed > interval; elapsed -= interval)
                                        {
                                            ++anim->frameIndex;
                                            if (anim->frameIndex >= frameDef->count)
                                                anim->frameIndex = 0;
                                        }
                                        ++anim->frameIndex;
                                        if (anim->frameIndex >= frameDef->count)
                                            anim->frameIndex = 0;
                                    }
                                    anim->nextClock = (interval - elapsed) + clockOwner->clock;
                                }
                                else
                                {
                                    ++anim->frameIndex;
                                    if (anim->frameIndex >= frameDef->count)
                                        anim->frameIndex = 0;
                                    anim->nextClock = anim->interval + clockOwner->clock;
                                }
                                anim->frameValue = frameDef->frames[anim->frameIndex];
                            }
                        }
                    }
                    if (mat->scrollU != 0.0f)
                    {
                        changed = 1;
                        mat->offsetU = deltaTime * mat->scrollU + mat->offsetU;
                        while (mat->offsetU > 1.0f)
                            mat->offsetU -= 1.0f;
                        while (mat->offsetU < -1.0f)
                            mat->offsetU += 1.0f;
                    }
                    if (mat->scrollV != 0.0f)
                    {
                        changed = 1;
                        mat->offsetV = deltaTime * mat->scrollV + mat->offsetV;
                        while (mat->offsetV > 1.0f)
                            mat->offsetV -= 1.0f;
                        while (mat->offsetV < -1.0f)
                            mat->offsetV += 1.0f;
                    }
                    ++pos;
                } while (pos < scrollList->count);
            }
            scrollList->dirty = changed;
        }
    }
}

// FUNCTION: EQGFX 0x1003d118
// @early-stop: register assignment and x87 load order remain different (103 vs 99 instructions).
// Collects queued physics effects under the mask, scoring 0x1e by distance and 0x1f as full.
/* fake match: reuse the dead queue parameter slot for the computed end pointer. */
/* fake match: keep the copied position values and byte-offset ranking loop used by VC6. */
void __cdecl processPhysicsEffects_guess(PhysEffectQueue_guess *queue, float *pos, float base, unsigned int maxCount)
{
    PhysEffectEntry_guess **cursor;
    PhysEffectEntry_guess *entry;
    float strength;
    volatile float pointYZ[2];
    unsigned int count;
    unsigned int slot;
    float dx;
    float dy;
    float dz;

    count = 0;
    g_effectCount_guess = 0;
    cursor = queue->entries;
    queue = (PhysEffectQueue_guess *)(cursor + queue->count);
    if (cursor >= (PhysEffectEntry_guess **)queue)
        return;
    strength = base;
    do
    {
        entry = *cursor;
        if (entry->type == 0x1e)
        {
            if ((entry->flags & *(unsigned int *)&base) != 0)
            {
                dx = pos[0] - entry->x;
                pointYZ[0] = pos[1];
                pointYZ[1] = pos[2];
                dy = pointYZ[0] - entry->y;
                dz = pointYZ[1] - entry->z;
                strength = (entry->radius * entry->radius * EQGFX_POINT_RADIUS_SCALE_GUESS) /
                           (dx * dx + dy * dy + dz * dz + EQGFX_FLOAT_ONE);
            }
            else
            {
                ++cursor;
                continue;
            }
        }
        else
        {
            if (entry->type == 0x1f)
            {
                if ((entry->flags & *(unsigned int *)&base) == 0)
                {
                    ++cursor;
                    continue;
                }
                strength = c_fullStrength_guess;
            }
        }
        slot = count;
        if (count > 0)
        {
            int offset;

            offset = count * 4 - 4;
            do
            {
                unsigned int *score = (unsigned int *)((char *)g_effectScores_guess + offset);

                if (strength < *(float *)score)
                    break;
                if (slot != maxCount)
                {
                    score[1] = score[0];
                    *(unsigned int *)((char *)g_effectEntries_guess + offset + 4) =
                        *(unsigned int *)((char *)g_effectEntries_guess + offset);
                }
                --slot;
                offset -= 4;
            } while (slot != 0);
        }
        if (slot < maxCount)
        {
            g_effectScores_guess[slot] = strength;
            g_effectEntries_guess[slot] = (int)*cursor;
            if (slot == count)
            {
                g_effectCount_guess = ++count;
            }
        }
        ++cursor;
    } while (cursor < (PhysEffectEntry_guess **)queue);
}

// FUNCTION: EQGFX 0x1003d238
// Prepares renderer buffer space for the requested scene geometry.
int __cdecl s3dReallocateDMSpriteRenderContextPools(RendererBufferState_guess *buffers, RendererBufferRequest_guess *request)
{
    unsigned int *requestedCounts;
    unsigned short *fallbackCounts;
    unsigned int primaryCount;
    unsigned int secondaryCount;
    int result;

    requestedCounts = request->requestedCounts;
    result = 0;
    if (requestedCounts != 0)
    {
        secondaryCount = requestedCounts[8];
        primaryCount = requestedCounts[0];
    }
    else
    {
        fallbackCounts = request->fallbackCounts;
        primaryCount = (unsigned int)fallbackCounts[0];
        secondaryCount = (unsigned int)fallbackCounts[4];
    }
    if (buffers->primaryCapacity < primaryCount)
    {
        buffers->bufferBytesA = t3dReallocateMemory_guess(buffers->bufferBytesA, primaryCount);
        if (buffers->bufferBytesA == 0)
            goto allocationFailed;
        buffers->bufferBytesB = t3dReallocateMemory_guess(buffers->bufferBytesB, primaryCount);
        if (buffers->bufferBytesB == 0)
            goto allocationFailed;
        buffers->buffer12A = t3dReallocateMemory_guess(buffers->buffer12A, primaryCount * 0xc);
        if (buffers->buffer12A == 0)
            goto allocationFailed;
        buffers->buffer12B = t3dReallocateMemory_guess(buffers->buffer12B, primaryCount * 0xc);
        if (buffers->buffer12B == 0)
            goto allocationFailed;
        buffers->buffer4A = t3dReallocateMemory_guess(buffers->buffer4A, primaryCount << 2);
        if (buffers->buffer4A == 0)
            goto allocationFailed;
        buffers->buffer4B = t3dReallocateMemory_guess(buffers->buffer4B, primaryCount << 2);
        if (buffers->buffer4B == 0)
            goto allocationFailed;
        buffers->primaryCapacity = primaryCount;
    }
    if (buffers->secondaryCapacity < secondaryCount)
    {
        buffers->bufferSecondary = t3dReallocateMemory_guess(buffers->bufferSecondary, secondaryCount);
        if (buffers->bufferSecondary == 0)
            goto allocationFailed;
        buffers->secondaryCapacity = secondaryCount;
    }
    goto done;

allocationFailed:
    result = RENDER_BUFFER_ALLOCATION_ERROR;
done:
    return result;
}

// FUNCTION: EQGFX 0x1003d353
// Transforms the strip records of a draw request into the renderer primitive buffers and submits each triangle.
// For each strip not flagged skip, resolves the material (index remap through renderer->indexRemap).
// Computes the face normal from the three positions and a side test against the plane; culls back faces
// unless the material has a diffuse value.
// Fills uv, normal and color per vertex, grows the buffers when capacity is low, view-transforms the corners
// with the offset and matrix, and calls submitRendererPrimitive_guess. Returns the last submit status.
int __cdecl t3dTransformAndDeferMeshTriangles_guess(int ignoredCallerValue, RendererBufferState_guess *renderer)
{
    RendererDrawRequest_guess *request;
    RendererPrimitiveState_guess *primitiveState;
    RendererMaterialTable_guess *materialTable;
    RendererColorOverride_guess *colorOverride;
    RendererStripRecord_guess *stripRecord;
    RendererMaterialEntry_guess *material;
    RendererPrimitiveDescription_guess description;
    unsigned char *positionBytes;
    unsigned char *normalBytes;
    unsigned char *uvBytes;
    unsigned char *colorBytes;
    unsigned char *colorA;
    unsigned char *colorB;
    unsigned char *colorC;
    unsigned char *tailColor;
    float *posA;
    float *posB;
    float *posC;
    float *uvA;
    float *uvB;
    float *uvC;
    float *normalA;
    float *normalB;
    float *normalC;
    float *scratch;
    float *outVertices;
    unsigned int *outWords;
    float *outCursor;
    float *outEnd;
    float *source;
    unsigned int materialFlags;
    unsigned int vertexA;
    unsigned int vertexB;
    unsigned int vertexC;
    int offsetA;
    int offsetB;
    int offsetC;
    unsigned int stripIndex;
    int stripOffset;
    unsigned int savedState;
    int result;
    float offsetX;
    float offsetY;
    float offsetZ;
    float planeX;
    float planeY;
    float planeZ;
    float m00;
    float m01;
    float m02;
    float m10;
    float m11;
    float m12;
    float m20;
    float m21;
    float m22;
    float edgeACx;
    float edgeACy;
    float normalX;
    float normalY;
    float normalZ;
    float side;
    float length;
    float sourceX;
    float sourceY;
    float sourceZ;

    result = 1;
    savedState = renderer->savedPrimitiveState;
    request = renderer->drawRequest;
    offsetX = renderer->viewOffset[0];
    offsetY = renderer->viewOffset[1];
    offsetZ = renderer->viewOffset[2];
    planeX = renderer->planeNormal[0];
    planeY = renderer->planeNormal[1];
    planeZ = renderer->planeNormal[2];
    m00 = renderer->viewM00;
    m01 = renderer->viewM01;
    m02 = renderer->viewM02;
    m10 = renderer->viewM10;
    m11 = renderer->viewM11;
    m12 = renderer->viewM12;
    m20 = renderer->viewM20;
    m21 = renderer->viewM21;
    m22 = renderer->viewM22;
    positionBytes = renderer->vertexPositions;
    normalBytes = renderer->vertexNormals;
    primitiveState = request->primitiveState;
    materialTable = primitiveState->materials;
    uvBytes = (unsigned char *)materialTable->textureCoordinates;
    colorOverride = request->colorOverride;
    if (colorOverride == 0)
    {
        colorBytes = materialTable->colorValues;
    }
    else
    {
        colorBytes = (unsigned char *)*(unsigned int *)(colorOverride->materials->colorValues + colorOverride->colorIndex * 4);
    }
    prepareRendererPrimitive_guess((int)request->meshState, (int)renderer);
    materialTable = primitiveState->materials;
    stripIndex = 0;
    if (materialTable->stripRecordCount != 0)
    {
        stripOffset = 0;
        do
        {
            stripRecord = (RendererStripRecord_guess *)(materialTable->stripRecords + stripOffset);
            if ((stripRecord->flags & 1) == 0)
            {
                material = request->meshState->materialEntries[stripRecord->materialIndex];
                materialFlags = material->flags;
                if ((materialFlags & RENDER_INDEX_REMAP_FLAG) != 0)
                {
                    materialFlags = renderer->indexRemap[materialFlags & ~RENDER_INDEX_REMAP_FLAG];
                }
                if (materialFlags != 0)
                {
                    vertexA = stripRecord->vertexA;
                    vertexB = stripRecord->vertexB;
                    vertexC = stripRecord->vertexC;
                    offsetA = vertexA * 12;
                    offsetB = vertexB * 12;
                    offsetC = vertexC * 12;
                    posA = (float *)(positionBytes + offsetA);
                    posB = (float *)(positionBytes + offsetB);
                    posC = (float *)(positionBytes + offsetC);
                    edgeACx = posC[0] - posA[0];
                    edgeACy = posC[1] - posA[1];
                    normalX = edgeACy * (posA[2] - posB[2]) - (posC[2] - posA[2]) * (posA[1] - posB[1]);
                    normalY = -(edgeACx * (posA[2] - posB[2]) - (posC[2] - posA[2]) * (posA[0] - posB[0]));
                    normalZ = edgeACx * (posA[1] - posB[1]) - edgeACy * (posA[0] - posB[0]);
                    side = -(normalY * posB[1] + normalZ * posB[2] + normalX * posB[0]) + normalZ * planeZ + normalY * planeY + normalX * planeX;
                    if (EQGFX_ZERO_F_GUESS < side || material->diffuseValue != 0)
                    {
                        description.words[0] = 0x37;
                        description.words[2] = material->renderWord18;
                        description.words[3] = material->renderWord1C;
                        description.pad0019[1] = material->renderByte2C;
                        description.renderWord48 = material->renderWord28;
                        description.textureValue = material->textureValue;
                        description.diffuseValue = material->diffuseValue;
                        description.renderState = 0;
                        if ((materialFlags & 0xff00) != 0)
                        {
                            materialFlags = (materialFlags & 0xffff00ff) | 0x400;
                            uvB = (float *)(uvBytes + vertexB * 8);
                            uvA = (float *)(uvBytes + vertexA * 8);
                            uvC = (float *)(uvBytes + vertexC * 8);
                        }
                        if ((materialFlags & 0x20) != 0)
                        {
                            if (normalBytes == 0)
                            {
                                materialFlags = materialFlags & 0xffffffdf;
                            }
                            else
                            {
                                normalB = (float *)(normalBytes + offsetB);
                                normalA = (float *)(normalBytes + offsetA);
                                normalC = (float *)(normalBytes + offsetC);
                            }
                        }
                        if (colorBytes == 0)
                        {
                            materialFlags = materialFlags & 0xbfffffff;
                        }
                        else
                        {
                            colorB = colorBytes + vertexB * 4;
                            colorA = colorBytes + vertexA * 4;
                            colorC = colorBytes + vertexC * 4;
                            materialFlags = materialFlags & 0xffdfffbf | 0x40000000;
                        }
                        if (renderer->primitiveCapacity < 9)
                        {
                            freeRenderBuffersByAddress_guess((int)renderer);
                            result = allocateRendererBuffers_guess((int)renderer, 0x19);
                            if (result < 0)
                            {
                                break;
                            }
                        }
                        if ((materialFlags & 0x200040) != 0 && (materialFlags & 0x20) == 0)
                        {
                            length = EQGFX_T3D_FLOAT_SQRT(normalZ * normalZ + normalY * normalY + normalX * normalX);
                            if (EQGFX_ZERO_F_GUESS < length)
                            {
                                normalX = normalX / length;
                                normalY = normalY / length;
                                normalZ = normalZ / length;
                            }
                            if (side <= EQGFX_ZERO_F_GUESS)
                            {
                                renderer->faceNormal[0] = -normalX;
                                renderer->faceNormal[1] = -normalY;
                                renderer->faceNormal[2] = -normalZ;
                            }
                            else
                            {
                                renderer->faceNormal[0] = normalX;
                                renderer->faceNormal[1] = normalY;
                                renderer->faceNormal[2] = normalZ;
                            }
                        }
                        if ((materialFlags & 0x40000000) == 0)
                        {
                            outVertices = (float *)renderer->primitiveVertices;
                            scratch = (float *)renderer->bufferAt2CC;
                            if ((materialFlags & 0x20) == 0)
                            {
                                if ((materialFlags & 0xff00) == 0x400)
                                {
                                    if (side <= EQGFX_ZERO_F_GUESS)
                                    {
                                        scratch[0] = posC[0];
                                        scratch[1] = posC[1];
                                        scratch[2] = posC[2];
                                        scratch[4] = uvC[0];
                                        scratch[5] = uvC[1];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x10] = uvA[0];
                                        scratch[0x11] = uvA[1];
                                        scratch[0x18] = posB[0];
                                        scratch[0x19] = posB[1];
                                        scratch[0x1a] = posB[2];
                                        scratch[0x1c] = uvB[0];
                                        scratch[0x1d] = uvB[1];
                                    }
                                    else
                                    {
                                        scratch[0] = posB[0];
                                        scratch[1] = posB[1];
                                        scratch[2] = posB[2];
                                        scratch[4] = uvB[0];
                                        scratch[5] = uvB[1];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x10] = uvA[0];
                                        scratch[0x11] = uvA[1];
                                        scratch[0x18] = posC[0];
                                        scratch[0x19] = posC[1];
                                        scratch[0x1a] = posC[2];
                                        scratch[0x1c] = uvC[0];
                                        scratch[0x1d] = uvC[1];
                                    }
                                }
                                else
                                {
                                    if (side <= EQGFX_ZERO_F_GUESS)
                                    {
                                        scratch[0] = posC[0];
                                        scratch[1] = posC[1];
                                        scratch[2] = posC[2];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x18] = posB[0];
                                        scratch[0x19] = posB[1];
                                        scratch[0x1a] = posB[2];
                                    }
                                    else
                                    {
                                        scratch[0] = posB[0];
                                        scratch[1] = posB[1];
                                        scratch[2] = posB[2];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x18] = posC[0];
                                        scratch[0x19] = posC[1];
                                        scratch[0x1a] = posC[2];
                                    }
                                }
                            }
                            else
                            {
                                if ((materialFlags & 0x400) == 0)
                                {
                                    if (side <= EQGFX_ZERO_F_GUESS)
                                    {
                                        scratch[0] = posC[0];
                                        scratch[1] = posC[1];
                                        scratch[2] = posC[2];
                                        scratch[6] = -normalB[0];
                                        scratch[7] = -normalB[1];
                                        scratch[8] = -normalB[2];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x12] = -normalA[0];
                                        scratch[0x13] = -normalA[1];
                                        scratch[0x14] = -normalA[2];
                                        scratch[0x18] = posB[0];
                                        scratch[0x19] = posB[1];
                                        scratch[0x1a] = posB[2];
                                        scratch[0x1e] = -normalC[0];
                                        scratch[0x1f] = -normalC[1];
                                        scratch[0x20] = -normalC[2];
                                    }
                                    else
                                    {
                                        scratch[0] = posB[0];
                                        scratch[1] = posB[1];
                                        scratch[2] = posB[2];
                                        scratch[6] = normalB[0];
                                        scratch[7] = normalB[1];
                                        scratch[8] = normalB[2];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x12] = normalA[0];
                                        scratch[0x13] = normalA[1];
                                        scratch[0x14] = normalA[2];
                                        scratch[0x18] = posC[0];
                                        scratch[0x19] = posC[1];
                                        scratch[0x1a] = posC[2];
                                        scratch[0x1e] = normalC[0];
                                        scratch[0x1f] = normalC[1];
                                        scratch[0x20] = normalC[2];
                                    }
                                }
                                else
                                {
                                    if (side <= EQGFX_ZERO_F_GUESS)
                                    {
                                        scratch[0] = posC[0];
                                        scratch[1] = posC[1];
                                        scratch[2] = posC[2];
                                        scratch[4] = uvC[0];
                                        scratch[5] = uvC[1];
                                        scratch[6] = -normalB[0];
                                        scratch[7] = -normalB[1];
                                        scratch[8] = -normalB[2];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x10] = uvA[0];
                                        scratch[0x11] = uvA[1];
                                        scratch[0x12] = -normalA[0];
                                        scratch[0x13] = -normalA[1];
                                        scratch[0x14] = -normalA[2];
                                        scratch[0x18] = posB[0];
                                        scratch[0x19] = posB[1];
                                        scratch[0x1a] = posB[2];
                                        scratch[0x1c] = uvB[0];
                                        scratch[0x1d] = uvB[1];
                                        scratch[0x1e] = -normalC[0];
                                        scratch[0x1f] = -normalC[1];
                                        scratch[0x20] = -normalC[2];
                                    }
                                    else
                                    {
                                        scratch[0] = posB[0];
                                        scratch[1] = posB[1];
                                        scratch[2] = posB[2];
                                        scratch[4] = uvB[0];
                                        scratch[5] = uvB[1];
                                        scratch[6] = normalB[0];
                                        scratch[7] = normalB[1];
                                        scratch[8] = normalB[2];
                                        scratch[0xc] = posA[0];
                                        scratch[0xd] = posA[1];
                                        scratch[0xe] = posA[2];
                                        scratch[0x10] = uvA[0];
                                        scratch[0x11] = uvA[1];
                                        scratch[0x12] = normalA[0];
                                        scratch[0x13] = normalA[1];
                                        scratch[0x14] = normalA[2];
                                        scratch[0x18] = posC[0];
                                        scratch[0x19] = posC[1];
                                        scratch[0x1a] = posC[2];
                                        scratch[0x1c] = uvC[0];
                                        scratch[0x1d] = uvC[1];
                                        scratch[0x1e] = normalC[0];
                                        scratch[0x1f] = normalC[1];
                                        scratch[0x20] = normalC[2];
                                    }
                                }
                            }
                            source = scratch;
                        }
                        else
                        {
                            outVertices = (float *)renderer->primitiveVertices;
                            outWords = (unsigned int *)outVertices;
                            tailColor = colorB;
                            if ((materialFlags & 0xff00) == 0x400)
                            {
                                if (EQGFX_ZERO_F_GUESS < side)
                                {
                                    outVertices[0] = posB[0];
                                    outVertices[1] = posB[1];
                                    outVertices[2] = posB[2];
                                    outVertices[4] = uvB[0];
                                    outVertices[5] = uvB[1];
                                    outWords[9] = colorB[2];
                                    outWords[0xa] = colorB[1];
                                    outWords[0xb] = colorB[0];
                                    outVertices[0xc] = posA[0];
                                    outVertices[0xd] = posA[1];
                                    outVertices[0xe] = posA[2];
                                    outVertices[0x10] = uvA[0];
                                    outVertices[0x11] = uvA[1];
                                    outWords[0x15] = colorA[2];
                                    outWords[0x16] = colorA[1];
                                    outWords[0x17] = colorA[0];
                                    outVertices[0x18] = posC[0];
                                    outVertices[0x19] = posC[1];
                                    outVertices[0x1a] = posC[2];
                                    outVertices[0x1c] = uvC[0];
                                    outVertices[0x1d] = uvC[1];
                                    tailColor = colorC;
                                }
                                else
                                {
                                    outVertices[0] = posC[0];
                                    outVertices[1] = posC[1];
                                    outVertices[2] = posC[2];
                                    outVertices[4] = uvC[0];
                                    outVertices[5] = uvC[1];
                                    outWords[9] = colorC[2];
                                    outWords[0xa] = colorC[1];
                                    outWords[0xb] = colorC[0];
                                    outVertices[0xc] = posA[0];
                                    outVertices[0xd] = posA[1];
                                    outVertices[0xe] = posA[2];
                                    outVertices[0x10] = uvA[0];
                                    outVertices[0x11] = uvA[1];
                                    outWords[0x15] = colorA[2];
                                    outWords[0x16] = colorA[1];
                                    outWords[0x17] = colorA[0];
                                    outVertices[0x18] = posB[0];
                                    outVertices[0x19] = posB[1];
                                    outVertices[0x1a] = posB[2];
                                    outVertices[0x1c] = uvB[0];
                                    outVertices[0x1d] = uvB[1];
                                }
                            }
                            else
                            {
                                if (side <= EQGFX_ZERO_F_GUESS)
                                {
                                    outVertices[0] = posC[0];
                                    outVertices[1] = posC[1];
                                    outVertices[2] = posC[2];
                                    outWords[9] = colorC[2];
                                    outWords[0xa] = colorC[1];
                                    outWords[0xb] = colorC[0];
                                    outVertices[0xc] = posA[0];
                                    outVertices[0xd] = posA[1];
                                    outVertices[0xe] = posA[2];
                                    outWords[0x15] = colorA[2];
                                    outWords[0x16] = colorA[1];
                                    outWords[0x17] = colorA[0];
                                    outVertices[0x18] = posB[0];
                                    outVertices[0x19] = posB[1];
                                    outVertices[0x1a] = posB[2];
                                }
                                else
                                {
                                    outVertices[0] = posB[0];
                                    outVertices[1] = posB[1];
                                    outVertices[2] = posB[2];
                                    outWords[9] = colorB[2];
                                    outWords[0xa] = colorB[1];
                                    outWords[0xb] = colorB[0];
                                    outVertices[0xc] = posA[0];
                                    outVertices[0xd] = posA[1];
                                    outVertices[0xe] = posA[2];
                                    outWords[0x15] = colorA[2];
                                    outWords[0x16] = colorA[1];
                                    outWords[0x17] = colorA[0];
                                    outVertices[0x18] = posC[0];
                                    outVertices[0x19] = posC[1];
                                    outVertices[0x1a] = posC[2];
                                    tailColor = colorC;
                                }
                            }
                            outWords[0x21] = tailColor[2];
                            outWords[0x22] = tailColor[1];
                            outWords[0x23] = tailColor[0];
                            source = outVertices;
                        }
                        outCursor = outVertices;
                        outEnd = outVertices + 0x24;
                        for (; outCursor < outEnd; outCursor += 0xc)
                        {
                            sourceX = source[0];
                            sourceY = source[1];
                            sourceZ = source[2];
                            source += 0xc;
                            outCursor[0] = (offsetZ + sourceZ) * m02 + (offsetY + sourceY) * m01 + (offsetX + sourceX) * m00;
                            outCursor[1] = (offsetZ + sourceZ) * m12 + (offsetY + sourceY) * m11 + (offsetX + sourceX) * m10;
                            outCursor[2] = (offsetZ + sourceZ) * m22 + (offsetY + sourceY) * m21 + (offsetX + sourceX) * m20;
                        }
                        result = submitRendererPrimitive_guess((int)renderer, 0, 3, &description, materialFlags);
                        if (result < 1)
                        {
                            if (result < 0)
                            {
                                break;
                            }
                        }
                        else
                        {
                            renderer->submittedPrimitiveCount++;
                        }
                    }
                }
            }
            stripIndex++;
            stripOffset += RENDER_STRIP_RECORD_BYTES;
            materialTable = primitiveState->materials;
        } while (stripIndex < materialTable->stripRecordCount);
    }
    renderer->savedPrimitiveState = savedState;
    return result;
}

// FUNCTION: EQGFX 0x1003ded4
// Emits indexed batches, routing opaque and translucent materials to pools.
// @early-stop: register assignments, the textured-byte test, and pool-delta updates still differ.
/* fake match: keep the pool base in its own frame slot for pointer-delta accounting. */
void __cdecl emitBatchMaterialRecordsToPools_guess(int batch, int ctx)
{
    RendererBatchContext_guess *context;
    EmitXform196_guess xform;
    RendererBatchRecord_guess *cursor;
    RendererBatchRecord_guess *fence;
    RendererBatchGroup_guess *set;
    RendererMaterialEntry_guess **listBase;
    RendererMaterialEntry_guess *matEntry;
    unsigned int blendBit;
    unsigned int transBit;
    void * volatile outBase;
    unsigned int count;
    unsigned int start;
    unsigned int shade;
    int endOut;
    RendererTextureDefinition_guess *texDef;
    float *pos;
    unsigned int *tint;
    char *active;
    int *remap;
    char *shape;

    set = ((RendererBatch_guess *)batch)->group;
    context = (RendererBatchContext_guess *)ctx;
    xform.baseX = context->baseX;
    xform.baseY = context->baseY;
    xform.sumX = xform.baseX + context->offsetX;
    xform.tileA = context->tileA;
    listBase = ((RendererBatch_guess *)batch)->materialList->materialEntries;
    xform.tileB = context->tileB;
    xform.sumY = xform.baseY + context->offsetY;
    cursor = set->inner->records;
    fence = cursor + set->inner->count;
    if (cursor < fence)
    {
        do
        {
            count = (unsigned int)cursor->count;
            matEntry = listBase[cursor->materialIndex];
            ctx = (int)matEntry->flags;
            if (((unsigned int)ctx & 0x80000000) != 0)
                ctx = (int)context->indexRemap[(unsigned int)ctx & 0x7fffffff];
            if (ctx == 0)
            {
                cursor++;
                continue;
            }
            start = (unsigned int)cursor->start;
            shape = (char *)set->inner->shape + start * 8;
            pos = context->positions + start * 3;
            tint = context->tint + start * 3;
            active = context->active + start;
            remap = context->remap + start;
            blendBit = (unsigned int)ctx & 0x1000000;
            transBit = (unsigned int)ctx & 0x80;
            if (transBit == 0 && blendBit == 0)
            {
                batch = (int)(g_emitOpaquePool_guess + g_emitOpaqueUsed_guess * 0x20);
                outBase = (void *)batch;
            }
            else
            {
                batch = (int)(g_emitTransPool_guess + g_emitTransUsed_guess * 0x20);
                outBase = (void *)batch;
            }
            if (blendBit == 0)
                xform.alpha = 0xff;
            else
            {
                if ((((unsigned char *)&ctx)[2] & 0x10) != 0)
                    shade = (((unsigned int)ctx >> 16) & 0xf) + 1;
                else
                    shade = ((unsigned int)ctx >> 16) & 0xf;
                xform.alpha = shade * 0xff >> 4;
            }
            if ((((unsigned char *)&ctx)[1] & 0xff) != 0)
            {
                xform.scrollU = *(unsigned int *)&matEntry->textureUOffset;
                xform.scrollV = *(unsigned int *)&matEntry->textureVOffset;
                texDef = (RendererTextureDefinition_guess *)*((RendererMaterialTextureBinding_guess *)matEntry->textureValue)->textureUse;
                xform.texU = texDef->textureU;
                xform.texV = texDef->textureV;
                endOut = (int)g_emitTextured_guess((int)count, active, pos, shape, tint, &xform, (void *)batch, (int)remap);
            }
            else
                endOut = (int)g_emitPlain_guess((int)count, active, pos, tint, &xform, (void *)batch, (int)remap);
            if (transBit == 0 && blendBit == 0)
                g_emitOpaqueUsed_guess += (unsigned int)((endOut - (int)outBase) >> 5);
            else
                g_emitTransUsed_guess += (unsigned int)((endOut - (int)outBase) >> 5);
            cursor++;
        } while (cursor < fence);
    }
}

// FUNCTION: EQGFX 0x1003e0da
// Projects queued vertices, packing color and emitting screen vertices for active flags.
float *__cdecl projectActiveVerticesToScreen_guess(int count, char *active, float *pos, char *shape, unsigned int *tint, float *xform, float *out, float **outSlot)
{
    char *edge;
    float inv;
    unsigned int packed;

    if (count != 0)
    {
        edge = shape + 10;
        do
        {
            if (*active != 0)
            {
                inv = 1.0f / pos[2];
                packed = (((unsigned int)(*(int *)xform << 8) | tint[0]) << 8 | tint[1]) << 8 | tint[2];
                ((int *)out)[4] = (int)packed;
                out[3] = inv;
                out[6] = (*(float *)(edge - 4) + xform[3]) * xform[1] * inv;
                out[7] = (*(float *)edge + xform[4]) * xform[2] * inv;
                out[0] = xform[5] * pos[0] * inv + xform[7];
                out[1] = xform[8] - pos[1] * xform[6] * inv;
                out[2] = xform[9] - xform[10] * xform[9] * inv;
                *outSlot = out;
                out += 8;
            }
            outSlot++;
            pos += 3;
            edge += 20;
            tint += 3;
            active++;
            count--;
        } while (count != 0);
        return out;
    }
    return out;
}

// FUNCTION: EQGFX 0x1003e196
// Projects queued verts, emitting screen verts to opaque or translucent pools.
// @early-stop: prologue register order, setup loads, argument pushes, and loop advances still differ.
/* fake match: retain VC6's pool-start slot and stack comparison for blendBit. */
void __cdecl emitStripMaterialRunsToPools_guess(char *batch, char *matList, char *ctx)
{
    EmitXform196_guess xform;
    unsigned short *cursor;
    unsigned short *fence;
    RendererMaterialEntry_guess **listBase;
    char *shape;
    float *pos;
    char *active;
    unsigned int *tint;
    int *remap;
    int group;
    RendererMaterialEntry_guess *matEntry;
    unsigned int matFlags;
    volatile unsigned int blendBit;
    unsigned int transBit;
    void *out;
    void * volatile outBase;
    unsigned int count;
    unsigned int shade;
    int endOut;
    int texLink;
    int texDef;

    remap = (int *)((RendererBufferState_guess *)ctx)->buffer4A;
    xform.baseX = *(float *)(ctx + 0x198);
    xform.baseY = *(float *)(ctx + 0x19c);
    xform.sumX = xform.baseX + *(float *)(ctx + 0x520);
    xform.tileA = *(unsigned int *)(ctx + 0x1a4);
    xform.tileB = *(unsigned int *)(ctx + 0x1a0);
    xform.sumY = xform.baseY + *(float *)(ctx + 0x524);
    group = (int)((RendererStripRequest_guess *)batch)->stripBatch;
    listBase = ((RendererStripMaterialLookup_guess *)matList)->materialEntries;
    cursor = *(unsigned short **)(group + 0x24);
    shape = *(char **)(group + 0x14);
    tint = (unsigned int *)((RendererBufferState_guess *)ctx)->buffer12B;
    active = (char *)((RendererBufferState_guess *)ctx)->bufferBytesA;
    fence = cursor + (unsigned int)*(unsigned short *)(group + 0xe) * 2;
    pos = (float *)((RendererBufferState_guess *)ctx)->buffer12A;
    if (cursor >= fence)
        return;
    do
    {
        count = (unsigned int)*cursor;
        matEntry = listBase[(unsigned int)cursor[1]];
        matFlags = matEntry->flags;
        if ((matFlags & 0x80000000) != 0)
            matFlags = *(((RendererBufferState_guess *)ctx)->indexRemap + (matFlags & 0x7fffffff));
        if (matFlags == 0)
        {
            shape += count * 0x14;
            pos += count * 3;
            active += count;
            tint += count * 3;
            remap += count;
            cursor += 2;
            continue;
        }
        blendBit = matFlags & 0x1000000;
        transBit = matFlags & 0x80;
        if (transBit == 0 && blendBit == 0)
        {
            out = g_emitOpaquePool_guess + g_emitOpaqueUsed_guess * 0x20;
            outBase = out;
            xform.alpha = 0xff;
        }
        else
        {
            out = g_emitTransPool_guess + g_emitTransUsed_guess * 0x20;
            outBase = out;
            if (blendBit == 0)
                xform.alpha = 0xff;
            else
            {
                if ((matFlags & 0x100000) != 0)
                    shade = ((matFlags >> 0x10) & 0xf) + 1;
                else
                    shade = (matFlags >> 0x10) & 0xf;
                xform.alpha = shade * 0xff >> 4;
            }
        }
        if ((matFlags & 0xff00) != 0)
        {
            xform.scrollU = *(unsigned int *)&matEntry->textureUOffset;
            xform.scrollV = *(unsigned int *)&matEntry->textureVOffset;
            texLink = matEntry->textureValue;
            texLink = *(int *)(texLink + 0x14);
            texDef = *(int *)texLink;
            xform.texU = *(unsigned int *)(texDef + 0x58);
            xform.texV = *(unsigned int *)(texDef + 0x5c);
            endOut = (int)EQ_CDECL(float *, 0x1003E0DA, (int, char *, float *, char *, unsigned int *, float *, float *, float **))((int)count, active, pos, shape, tint, (float *)&xform, (float *)out, (float **)remap);
        }
        else
        {
            endOut = (int)g_emitPlain_guess((int)count, active, pos, tint, &xform, out, (int)remap);
        }
        if (transBit == 0 && blendBit == 0)
            g_emitOpaqueUsed_guess += (unsigned int)((endOut - (int)outBase) >> 5);
        else
            g_emitTransUsed_guess += (unsigned int)((endOut - (int)outBase) >> 5);
        shape += count * 0x14;
        active += count;
        pos += count * 3;
        tint += count * 3;
        remap += count;
        cursor += 2;
    } while (cursor < fence);
}

// FUNCTION: EQGFX 0x1003e3c2
// @early-stop: frame slots, register choices, and indirect call/update blocks still differ.
// Projects queued verts, emitting screen verts to opaque or translucent pools.
/* fake match: retain the modeBit spill slot VC6 otherwise removes from this frame. */
void __cdecl emitIndexedBatchMaterialRecordsToPools_guess(int batch, int ctx)
{
    int context = ctx;
    int batchAddress = batch;
    EmitXform196_guess xform;
    unsigned short *cursor;
    unsigned short *fence;
    int parent;
    int listBase;
    float *pos;
    unsigned int *tint;
    char *active;
    char *shape;
    int *remap;
    int matEntry;
    unsigned int matFlags;
    unsigned int blendBit;
    unsigned int transBit;
    unsigned int modeBit;
    void *out;
    unsigned int count;
    unsigned int vert;
    unsigned int shade;
    volatile unsigned int stackPad;
    int endOut;

    xform.baseX = *(float *)(context + 0x198);
    xform.baseY = *(float *)(context + 0x19c);
    xform.sumX = xform.baseX + *(float *)(context + 0x520);
    xform.tileA = *(unsigned int *)(context + 0x1a4);
    parent = *(int *)(batchAddress + 0x34);
    xform.sumY = xform.baseY + *(float *)(context + 0x524);
    xform.tileB = *(unsigned int *)(context + 0x1a0);
    xform.texU = g_xformU3c2_guess;
    xform.texV = g_xformV3c2_guess;
    listBase = *(int *)(*(int *)(batchAddress + 0x20) + 0x18);
    cursor = *(unsigned short **)(*(int *)(parent + 0x14) + 0x58);
    fence = cursor + *(int *)(*(int *)(parent + 0x14) + 0x54) * 4;
    if (cursor >= fence)
        return;
    do
    {
        count = (unsigned int)cursor[1];
        matEntry = *(int *)(listBase + (unsigned int)cursor[2] * 4);
        matFlags = *(unsigned int *)(matEntry + 0x14);
        if ((matFlags & 0x80000000) != 0)
            matFlags = *(*(unsigned int **)(context + 0x3e0) + (matFlags & 0x7fffffff));
        if (matFlags == 0)
        {
            cursor += 4;
            continue;
        }
        ctx = (int)matFlags;
        vert = (unsigned int)*cursor;
        shape = *(char **)(*(int *)(parent + 0x14) + 0xc);
        pos = *(float **)(context + 0x610) + vert * 3;
        remap = *(int **)(context + 0x628) + vert;
        shape += vert * 8;
        tint = *(unsigned int **)(context + 0x614) + vert * 3;
        active = *(char **)(context + 0x61c) + vert;
        blendBit = (unsigned int)ctx & 0x1000000;
        transBit = (unsigned int)ctx & 0x80;
        if (transBit == 0 && blendBit == 0)
        {
            out = g_emitOpaquePool_guess + g_emitOpaqueUsed_guess * 0x20;
            batch = g_emitOpaqueUsed_guess;
        }
        else
        {
            out = g_emitTransPool_guess + g_emitTransUsed_guess * 0x20;
            batch = g_emitTransUsed_guess;
        }
        if (blendBit == 0)
        {
            xform.alpha = 0xff;
        }
        else
        {
            modeBit = (unsigned int)ctx & 0x100000;
            if (modeBit == 0)
                shade = ((unsigned int)ctx >> 0x10) & 0xf;
            else
                shade = (((unsigned int)ctx >> 0x10) & 0xf) + 1;
            xform.alpha = shade * 0xff >> 4;
        }
        stackPad = modeBit;
        if (((unsigned int)ctx & 0xff00) == 0)
        {
            if (modeBit == 0 || g_renderMode3c2_guess == 3)
                endOut = (int)g_emitPlainStd3c2_guess((int)count, (unsigned int)batch, active, pos, tint, &xform, out, (int)remap);
            else
                endOut = (int)g_emitPlainAlt3c2_guess((int)count, (unsigned int)batch, active, pos, tint, &xform, out, (int)remap);
        }
        else
        {
            xform.scrollU = *(unsigned int *)(matEntry + 0x38);
            xform.scrollV = *(unsigned int *)(matEntry + 0x3c);
            if (modeBit == 0 || g_renderMode3c2_guess == 3)
                endOut = (int)g_emitTexturedStd3c2_guess((int)count, (unsigned int)batch, active, pos, shape, tint, &xform, out, (int)remap);
            else
                endOut = (int)g_emitTexturedAlt3c2_guess((int)count, (unsigned int)batch, active, pos, shape, tint, &xform, out, (int)remap);
        }
        if (transBit == 0 && blendBit == 0)
            g_emitOpaqueUsed_guess += (unsigned int)((endOut - (int)out) >> 5);
        else
            g_emitTransUsed_guess += (unsigned int)((endOut - (int)out) >> 5);
        (void)stackPad;
        cursor += 4;
    } while (cursor < fence);
}

// FUNCTION: EQGFX 0x1003e631
// Projects each active vert to a screen vert at out, writing its output index to remap; returns the cursor.
ScreenVert_guess *__cdecl projectActiveVerticesWithFogToScreen_guess(int count, int nextIndex, char *active, float *pos, char *shape, unsigned int *tint, ProjectXform_guess *xform, ScreenVert_guess *out, int *remap)
{
    char *texCoord;
    float invZ;
    int fogLevel;

    if (count != 0)
    {
        texCoord = shape + 10;
        do
        {
            if (*active != 0)
            {
                invZ = 1.0f / pos[2];
                out->color = ((xform->alpha << 8 | tint[0]) << 8 | tint[1]) << 8 | tint[2];
                EQ_CDECL(void, 0x1004EC8D, (float, int *))((pos[2] - xform->fogStart) * xform->fogScale, &fogLevel);
                if (fogLevel > 0xff)
                {
                    fogLevel = 0xff;
                }
                else if (fogLevel < 0)
                {
                    fogLevel = 0;
                }
                fogLevel = 0xff - fogLevel;
                out->specular = fogLevel << 24;
                out->rhw = invZ;
                out->tu = *(float *)(texCoord - 4) + xform->uOffset;
                out->tv = xform->vOffset + *(float *)texCoord;
                out->x = xform->centerX + (xform->scaleX * pos[0] * invZ);
                out->y = xform->centerY - pos[1] * xform->scaleY * invZ;
                out->z = xform->depthScale - invZ * xform->depthNear * xform->depthScale;
                *remap = nextIndex;
                nextIndex++;
                out++;
            }
            texCoord += 20;
            tint += 3;
            remap++;
            pos += 3;
            active++;
            count--;
        } while (count != 0);
    }
    return out;
}

// FUNCTION: EQGFX 0x1003e738
// Prepares tinted, fogged vertices for display on screen.
// Projects each active vert to a screen vert at out with the tint-scaled colour, writing its output index to remap; returns the cursor.
ScreenVert_guess *__cdecl projectTintedActiveVerticesWithFogToScreen_guess(int count, int nextIndex, char *active, float *pos, char *shape, int *tint, ProjectXform_guess *xform, ScreenVert_guess *out, int *remap)
{
    char *texCoord;
    float invZ;
    int fogLevel;
    int alpha;
    int remaining;

    if (count != 0)
    {
        texCoord = shape + 10;
        remaining = count;
        do
        {
            if (*active != 0)
            {
                alpha = (int)xform->alpha;
                invZ = 1.0f / pos[2];
                out->color = ((tint[0] * alpha & 0xffffff00 | 0xffff0000) << 8) | (tint[2] * alpha >> 8) | (tint[1] * alpha & 0xffffff00);
                EQ_CDECL(void, 0x1004EC8D, (float, int *))((pos[2] - xform->fogStart) * xform->fogScale, &fogLevel);
                if (fogLevel > 0xff)
                {
                    fogLevel = 0xff;
                }
                else if (fogLevel < 0)
                {
                    fogLevel = 0;
                }
                fogLevel = 0xff - fogLevel;
                out->specular = fogLevel << 24;
                out->rhw = invZ;
                out->tu = *(float *)(texCoord - 4) + xform->uOffset;
                out->tv = xform->vOffset + *(float *)texCoord;
                out->x = invZ * (xform->scaleX * pos[0]) + xform->centerX;
                out->y = xform->centerY - pos[1] * xform->scaleY * invZ;
                out->z = xform->depthScale - invZ * xform->depthNear * xform->depthScale;
                *remap = nextIndex;
                nextIndex++;
                out++;
            }
            texCoord += 20;
            tint += 3;
            remap++;
            pos += 3;
            active++;
            remaining--;
        } while (remaining != 0);
    }
    return out;
}

// FUNCTION: EQGFX 0x1003e858
// Projects each material run's active verts to screen verts, appended to the opaque or translucent pool.
// @early-stop: frame setup, register allocation, and call/update blocks still differ.
/* fake match: reuse the argument slots and keep the run cursor local as in the original. */
void __cdecl emitBatchMaterialRunsToPools_guess(int batchTint, int materialActive, int contextPosition)
{
    EmitXform196_guess xform;
    int ctxAddress;
    int remap;
    MatEntry_guess **entries;
    MatEntry_guess *matEntry;
    RunGroup_guess *group;
    Run_guess * volatile cursor;
    Run_guess *fence;
    char *shape;
    unsigned int count;
    unsigned int matFlags;
    unsigned int blendBit;
    unsigned int transBit;
    unsigned int altBit;
    unsigned int shade;
    unsigned int poolUsed;
    void *out;
    int endOut;

    ctxAddress = contextPosition;
    xform.baseX = ((ClipContext_guess *)ctxAddress)->baseX;
    remap = (int)((ClipContext_guess *)ctxAddress)->remap;
    xform.baseY = ((ClipContext_guess *)ctxAddress)->baseY;
    xform.sumX = xform.baseX + ((ClipContext_guess *)ctxAddress)->offsetX;
    xform.tileA = *(unsigned int *)&((ClipContext_guess *)ctxAddress)->depthScale;
    xform.tileB = *(unsigned int *)&((ClipContext_guess *)ctxAddress)->nearLimit;
    xform.sumY = xform.baseY + ((ClipContext_guess *)ctxAddress)->offsetY;
    xform.texU = g_xformU3c2_guess;
    xform.texV = g_xformV3c2_guess;
    entries = ((MatList_guess *)materialActive)->entries;
    group = ((Batch_guess *)batchTint)->group;
    cursor = group->runs;
    shape = group->shape;
    fence = cursor + group->runCount;
    batchTint = (int)((ClipContext_guess *)ctxAddress)->tint;
    materialActive = (int)((ClipContext_guess *)ctxAddress)->active;
    contextPosition = (int)((ClipContext_guess *)ctxAddress)->positions;
    if (cursor < fence)
    {
        do
        {
            count = cursor->vertCount;
            matEntry = entries[cursor->matIndex];
            matFlags = matEntry->flags;
            if ((matFlags & 0x80000000) != 0)
            {
                matFlags = ((ClipContext_guess *)ctxAddress)->flagMap[matFlags & 0x7fffffff];
            }
            if (matFlags == 0)
            {
                shape += count * 20;
            }
            else
            {
                blendBit = matFlags & 0x1000000;
                transBit = matFlags & 0x80;
                if (transBit == 0 && blendBit == 0)
                {
                    poolUsed = g_emitOpaqueUsed_guess;
                    out = g_emitOpaquePool_guess + poolUsed * 0x20;
                    xform.alpha = 0xff;
                }
                else
                {
                    poolUsed = g_emitTransUsed_guess;
                    out = g_emitTransPool_guess + poolUsed * 0x20;
                    if (blendBit == 0)
                    {
                        xform.alpha = 0xff;
                    }
                    else
                    {
                        altBit = matFlags & 0x100000;
                        if (altBit == 0)
                        {
                            shade = (matFlags >> 0x10) & 0xf;
                        }
                        else
                        {
                            shade = ((matFlags >> 0x10) & 0xf) + 1;
                        }
                        xform.alpha = shade * 0xff >> 4;
                    }
                }
                if ((matFlags & 0xff00) == 0)
                {
                    if (altBit == 0 || g_renderMode3c2_guess == 3)
                    {
                        endOut = g_emitPlainStd3c2_guess(count, poolUsed, (char *)materialActive, (float *)contextPosition, (unsigned int *)batchTint, &xform, out, remap);
                    }
                    else
                    {
                        endOut = g_emitPlainAlt3c2_guess(count, poolUsed, (char *)materialActive, (float *)contextPosition, (unsigned int *)batchTint, &xform, out, remap);
                    }
                }
                else
                {
                    xform.scrollU = matEntry->scrollU;
                    xform.scrollV = matEntry->scrollV;
                    if (altBit == 0 || g_renderMode3c2_guess == 3)
                    {
                        endOut = (int)EQ_CDECL(ScreenVert_guess *, 0x1003E631, (int, int, char *, float *, char *, unsigned int *, ProjectXform_guess *, ScreenVert_guess *, int *))(count, poolUsed, (char *)materialActive, (float *)contextPosition, shape, (unsigned int *)batchTint, (ProjectXform_guess *)&xform, (ScreenVert_guess *)out, (int *)remap);
                    }
                    else
                    {
                        endOut = (int)EQ_CDECL(ScreenVert_guess *, 0x1003E738, (int, int, char *, float *, char *, int *, ProjectXform_guess *, ScreenVert_guess *, int *))(count, poolUsed, (char *)materialActive, (float *)contextPosition, shape, (int *)batchTint, (ProjectXform_guess *)&xform, (ScreenVert_guess *)out, (int *)remap);
                    }
                }
                if (transBit == 0 && blendBit == 0)
                {
                    g_emitOpaqueUsed_guess += (endOut - (int)out) >> 5;
                }
                else
                {
                    g_emitTransUsed_guess += (endOut - (int)out) >> 5;
                }
                shape += count * 20;
            }
            contextPosition += count * 12;
            materialActive += count;
            batchTint += count * 12;
            cursor++;
            remap += count * 4;
        } while (cursor < fence);
    }
}

// FUNCTION: EQGFX 0x1003eadb
// Queues each active triangle's 3 remapped indices to opaque or translucent lists; (flags&3)==2 draws edges.
// @early-stop: startup register allocation and loop block order still differ.
int __cdecl t3dQueueTrianglesByMaterialRange_guess(int unused, RendererBufferState_guess *ctx, unsigned int req)
{
    int listBase;
    int parent;
    int remapBase;
    int group;
    unsigned short *cursor;
    unsigned short *fence;
    RendererStripRecord_guess *vert;
    RendererStripRecord_guess *vertEnd;
    int entry;
    unsigned int start;
    unsigned int count;
    unsigned int matFlags;
    unsigned int flags;
    RendererTextureUse_guess *tex;
    unsigned int forceTrans;
    int kind;
    int textured;
    int masked;
    unsigned int *slot;
    unsigned int *dest;
    unsigned int *destBase;
    unsigned short idx0;
    unsigned short idx1;
    unsigned short idx2;
    int emitted;
    int order;
    unsigned char *active;

    (void)unused;
    parent = *(int *)(req + 0x34);
    remapBase = (int)ctx->buffer4A;
    listBase = *(int *)(*(int *)(req + 0x20) + 0x18);
    group = *(int *)(parent + 0x14);
    cursor = *(unsigned short **)(group + 0x50);
    fence = cursor + (unsigned int)*(int *)(group + 0x4c) * 3;
    for (; cursor < fence; cursor += 3)
    {
        entry = *(int *)(listBase + (unsigned int)cursor[2] * 4);
        matFlags = *(unsigned int *)(entry + 0x14);
        if ((matFlags & 0x80000000) != 0)
            matFlags = ctx->indexRemap[matFlags & 0x7fffffff];
        if (matFlags != 0)
        {
            start = (unsigned int)*cursor;
            vert = (RendererStripRecord_guess *)(*(int *)(*(int *)(parent + 0x14) + 0x24) + start * 0x10);
            active = (unsigned char *)ctx->bufferSecondary + start;
            count = (unsigned int)cursor[1];
            forceTrans = matFlags & 0x80;
            kind = ((matFlags & 3) == 2);
            textured = ((matFlags & 0xff00) != 0);
            masked = (int)(matFlags & 0x100000);
            vertEnd = vert + count;
            if (textured == 0)
                tex = 0;
            else
            {
                RendererMaterialTextureBinding_guess *binding =
                    (RendererMaterialTextureBinding_guess *)((RendererMaterialEntry_guess *)entry)->textureValue;
                tex = *binding->textureUse;
            }
            flags = 0;
            if (kind != 0)
                flags = 1;
            if ((matFlags & 0x1000000) != 0)
            {
                if (masked == 0)
                    flags |= 2;
                else
                    flags |= 4;
            }
            if (forceTrans != 0)
                flags |= 8;
            else if ((matFlags & 0x1000000) == 0)
            {
                    slot = g_opaqueEntries_guess + g_opaqueEntryCount_guess * 7;
                    dest = g_opaqueVertBuf_guess + g_opaqueVertCount_guess;
                    destBase = dest;
                    if (kind != 0)
                    {
                        if (vert < vertEnd)
                        {
                            do
                            {
                                if (*active != 0)
                                {
                                    idx0 = vert->vertexB;
                                    idx1 = vert->vertexA;
                                    idx2 = vert->vertexC;
                                    grDrawLine_guess(*(int *)(remapBase + (unsigned int)idx0 * 4),
                                        *(int *)(remapBase + (unsigned int)idx1 * 4));
                                    grDrawLine_guess(*(int *)(remapBase + (unsigned int)idx1 * 4),
                                        *(int *)(remapBase + (unsigned int)idx2 * 4));
                                    grDrawLine_guess(*(int *)(remapBase + (unsigned int)idx2 * 4),
                                        *(int *)(remapBase + (unsigned int)idx0 * 4));
                                    ctx->submittedPrimitiveCount++;
                                    g_primCount_guess = g_primCount_guess + 1;
                                }
                                active++;
                                vert++;
                            }
                            while (vert < vertEnd);
                        }
                    }
                    else
                    {
                        if (vert < vertEnd)
                        {
                            do
                            {
                                order = *active;
                                if (order != 0)
                                {
                                    idx0 = vert->vertexB;
                                    idx1 = vert->vertexA;
                                    idx2 = vert->vertexC;
                                    slot[0] = flags;
                                    slot[3] = 3;
                                    slot[5] = (unsigned int)dest;
                                    slot[1] = (unsigned int)tex;
                                    if (textured == 0)
                                    {
                                        slot[6] = g_opaqueHead_guess;
                                        g_opaqueHead_guess = (unsigned int)slot;
                                    }
                                    else
                                    {
                                        slot[6] = tex->opaqueNext;
                                        tex->opaqueNext = (unsigned int)slot;
                                    }
                                    if (order == 1)
                                    {
                                        *dest = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                                        dest++;
                                        *dest = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                                        dest++;
                                        *dest = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                                        dest++;
                                    }
                                    else
                                    {
                                        *dest = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                                        dest++;
                                        *dest = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                                        dest++;
                                        *dest = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                                        dest++;
                                    }
                                    slot += 7;
                                    g_opaqueEntryCount_guess = g_opaqueEntryCount_guess + 1;
                                    ctx->submittedPrimitiveCount++;
                                }
                                active++;
                                vert++;
                            }
                            while (vert < vertEnd);
                        }
                        emitted = (int)(dest - destBase);
                        if (emitted != 0)
                        {
                            if (textured != 0)
                                markTextureAsUsedInEffects_guess((int)tex);
                            g_opaqueVertCount_guess = g_opaqueVertCount_guess + (unsigned int)emitted;
                        }
                    }
                    continue;
            }
            slot = g_transEntries_guess + g_transEntryCount_guess * 7;
            dest = g_transVertBuf_guess + g_transVertCount_guess;
            destBase = dest;
            if (vert < vertEnd)
            {
                do
                {
                    order = *active;
                    if (order != 0)
                    {
                        idx0 = vert->vertexB;
                        idx1 = vert->vertexA;
                        idx2 = vert->vertexC;
                        slot[0] = flags;
                        slot[3] = 3;
                        slot[5] = (unsigned int)dest;
                        slot[1] = (unsigned int)tex;
                        if (textured == 0)
                        {
                            slot[6] = g_transHead_guess;
                            g_transHead_guess = (unsigned int)slot;
                        }
                        else if (flags == 8)
                        {
                            slot[6] = tex->translucentNext;
                            tex->translucentNext = (unsigned int)slot;
                        }
                        else
                        {
                            slot[6] = tex->maskedNext;
                            tex->maskedNext = (unsigned int)slot;
                        }
                        if (order == 1)
                        {
                            *dest = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                            dest++;
                            *dest = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                            dest++;
                            *dest = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                            dest++;
                        }
                        else
                        {
                            *dest = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                            dest++;
                            *dest = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                            dest++;
                            *dest = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                            dest++;
                        }
                        slot += 7;
                        g_transEntryCount_guess = g_transEntryCount_guess + 1;
                        ctx->submittedPrimitiveCount++;
                    }
                    active++;
                    vert++;
                }
                while (vert < vertEnd);
            }
            emitted = (int)(dest - destBase);
            if (emitted != 0 && textured != 0)
                markTextureAsUsedInEffects_guess((int)tex);
            g_transVertCount_guess = g_transVertCount_guess + (unsigned int)emitted;
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x1003eeca
// Queues each active 8-byte triangle's 3 remapped indices to opaque/translucent lists; line mode draws edges.
// @early-stop: startup register allocation and the translucent loop layout still differ (best structural ratio 0.7073).
int __cdecl t3dQueuePackedTrianglesByMaterialRun_guess(int unused, RendererBufferState_guess *ctx, RendererStripRequest_guess *batch, unsigned char *matList)
{
    RendererMaterialEntry_guess **listBase;
    int remapBase;
    RendererStripBatch_guess *group;
    unsigned short *cursor;
    unsigned short *fence;
    unsigned int vert;
    unsigned int vertEnd;
    RendererMaterialEntry_guess *entry;
    unsigned int count;
    unsigned int matFlags;
    unsigned int flags;
    unsigned int tex;
    int kind;
    int textured;
    int masked;
    unsigned int *slot;
    unsigned int *dest;
    unsigned int *destBase;
    unsigned int idx0;
    unsigned int idx1;
    unsigned int idx2;
    int emitted;
    int order;

    listBase = ((RendererStripMaterialLookup_guess *)matList)->materialEntries;
    remapBase = (int)ctx->buffer4A;
    group = batch->stripBatch;
    cursor = group->stripGroups;
    vert = (unsigned int)group->stripRecords;
    matList = (unsigned char *)ctx->bufferSecondary;
    fence = cursor + (unsigned int)group->stripCount * 2;
    if (cursor >= fence)
        return 1;
    do
    {
        entry = listBase[(unsigned int)cursor[1]];
        count = (unsigned int)*cursor;
        matFlags = entry->flags;
        if ((matFlags & 0x80000000) != 0)
            matFlags = *(ctx->indexRemap + (matFlags & 0x7fffffff));
        if (matFlags == 0)
        {
            matList += count;
            vert += count * 8;
        }
        else
        {
            vertEnd = vert + count * 8;
            kind = ((matFlags & 3) == 2);
            textured = ((matFlags & 0xff00) != 0);
            flags = 0;
            if (kind != 0)
                flags = 1;
            if (textured == 0)
                tex = 0;
            else
                tex = **(unsigned int **)((int)entry->textureValue + 0x14);
            masked = (int)(matFlags & 0x100000);
            if ((matFlags & 0x1000000) != 0)
            {
                if (masked == 0)
                    flags |= 2;
                else
                    flags |= 4;
            }
            if ((matFlags & 0x80) != 0)
            {
                flags |= 8;
            }
            else if ((matFlags & 0x1000000) == 0)
            {
                slot = g_opaqueEntries_guess + g_opaqueEntryCount_guess * 7;
                dest = g_opaqueVertBuf_guess + g_opaqueVertCount_guess;
                destBase = dest;
                if (kind == 0)
                {
                    while (vert < vertEnd)
                    {
                        order = *matList;
                        if (order != 0)
                        {
                            idx0 = *(unsigned short *)(vert + 2);
                            idx1 = *(unsigned short *)(vert + 4);
                            idx2 = *(unsigned short *)(vert + 6);
                            slot[0] = flags;
                            slot[3] = 3;
                            slot[5] = (unsigned int)dest;
                            slot[1] = tex;
                            if (textured == 0)
                            {
                                slot[6] = g_opaqueHead_guess;
                                g_opaqueHead_guess = (unsigned int)slot;
                            }
                            else
                            {
                                slot[6] = *(unsigned int *)(tex + 0x64);
                                *(unsigned int *)(tex + 0x64) = (unsigned int)slot;
                            }
                            if (order == 1)
                            {
                                *dest++ = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                                *dest++ = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                                *dest++ = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                            }
                            else
                            {
                                *dest++ = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                                *dest++ = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                                *dest++ = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                            }
                            slot += 7;
                            g_opaqueEntryCount_guess = g_opaqueEntryCount_guess + 1;
                            ctx->submittedPrimitiveCount++;
                        }
                        matList++;
                        vert += 8;
                    }
                    emitted = (int)(dest - destBase);
                    if (emitted != 0)
                    {
                        if (textured != 0)
                            markTextureAsUsedInEffects_guess((int)tex);
                        g_opaqueVertCount_guess = g_opaqueVertCount_guess + (unsigned int)emitted;
                    }
                }
                else
                {
                    while (vert < vertEnd)
                    {
                        if (*matList != 0)
                        {
                            idx0 = *(unsigned short *)(vert + 2);
                            idx1 = *(unsigned short *)(vert + 4);
                            idx2 = *(unsigned short *)(vert + 6);
                            grDrawLine_guess(*(int *)(remapBase + (unsigned int)idx0 * 4),
                                *(int *)(remapBase + (unsigned int)idx1 * 4));
                            grDrawLine_guess(*(int *)(remapBase + (unsigned int)idx1 * 4),
                                *(int *)(remapBase + (unsigned int)idx2 * 4));
                            grDrawLine_guess(*(int *)(remapBase + (unsigned int)idx2 * 4),
                                *(int *)(remapBase + (unsigned int)idx0 * 4));
                            ctx->submittedPrimitiveCount++;
                            g_primCount_guess = g_primCount_guess + 1;
                        }
                        matList++;
                        vert += 8;
                    }
                }
                /* fake match: skip translucent routing after queuing opaque data. */
                goto advanceCursor;
            }
            slot = g_transEntries_guess + g_transEntryCount_guess * 7;
            dest = g_transVertBuf_guess + g_transVertCount_guess;
            destBase = dest;
            while (vert < vertEnd)
            {
                order = *matList;
                if (order != 0)
                {
                    idx0 = *(unsigned short *)(vert + 2);
                    idx1 = *(unsigned short *)(vert + 4);
                    idx2 = *(unsigned short *)(vert + 6);
                    slot[0] = flags;
                    slot[3] = 3;
                    slot[5] = (unsigned int)dest;
                    slot[1] = tex;
                    if (textured == 0)
                    {
                        slot[6] = g_transHead_guess;
                        g_transHead_guess = (unsigned int)slot;
                    }
                    else if (flags == 8)
                    {
                        slot[6] = *(unsigned int *)(tex + 0x68);
                        *(unsigned int *)(tex + 0x68) = (unsigned int)slot;
                    }
                    else
                    {
                        slot[6] = *(unsigned int *)(tex + 0x6c);
                        *(unsigned int *)(tex + 0x6c) = (unsigned int)slot;
                    }
                    if (order == 1)
                    {
                        dest[0] = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                        dest[1] = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                        dest[2] = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                    }
                    else
                    {
                        dest[0] = *(unsigned int *)(remapBase + (unsigned int)idx2 * 4);
                        dest[1] = *(unsigned int *)(remapBase + (unsigned int)idx1 * 4);
                        dest[2] = *(unsigned int *)(remapBase + (unsigned int)idx0 * 4);
                    }
                    slot += 7;
                    dest += 3;
                    g_transEntryCount_guess = g_transEntryCount_guess + 1;
                    ctx->submittedPrimitiveCount++;
                }
                matList++;
                vert += 8;
            }
            emitted = (int)(dest - destBase);
            if (emitted != 0 && textured != 0)
                markTextureAsUsedInEffects_guess((int)tex);
            g_transVertCount_guess = g_transVertCount_guess + (unsigned int)emitted;
        }
advanceCursor:
        cursor += 2;
    } while (cursor < fence);
    return 1;
}

// FUNCTION: EQGFX 0x1003f28c
// Queues each (start, count, material) group's triangles into two index lists by material flags.
// @early-stop: loop layout and register allocation still differ.
int __cdecl queueTrianglesFromMaterialRanges_guess(int unused, RendererBufferState_guess *ctx, unsigned int req)
{
    RendererPrimitiveState_guess *parent;
    RendererMaterialEntry_guess **listBase;
    unsigned char *remapBase;
    RendererMaterialTable_guess *group;
    unsigned short *cursor;
    unsigned short *fence;
    unsigned char *vert;
    unsigned char *vertEnd;
    RendererMaterialEntry_guess *entry;
    unsigned int start;
    unsigned int matFlags;
    unsigned int masked;
    unsigned int tex;
    int textured;
    unsigned int *slot;
    unsigned short *dest;
    unsigned short *destBase;
    unsigned short idx0;
    unsigned short idx1;
    unsigned short idx2;
    int emitted;
    int order;
    unsigned char *batch;

    parent = ((RendererDrawRequest_guess *)req)->primitiveState;
    remapBase = (unsigned char *)ctx->buffer4B;
    listBase = ((RendererDrawRequest_guess *)req)->meshState->materialEntries;
    group = parent->materials;
    cursor = group->stripIndices;
    fence = cursor + group->stripCount * 3;
    if (cursor < fence)
    {
        do
        {
            entry = listBase[cursor[2]];
            matFlags = entry->flags;
            if ((matFlags & 0x80000000) != 0)
                matFlags = ctx->indexRemap[matFlags & 0x7fffffff];
            if (matFlags == 0)
            {
                cursor += 3;
                continue;
            }
            {
                start = (unsigned int)*cursor;
                vert = parent->materials->stripRecords + start * 0x10;
                batch = (unsigned char *)ctx->bufferSecondary + start;
                vertEnd = vert + cursor[1] * 0x10;
                textured = (matFlags & 0xff00) != 0;
                if (textured == 0)
                    tex = 0;
                else
                    tex = **(unsigned int **)(entry->textureValue + 0x14);
                req = (unsigned int)((matFlags & 3) == 2);
                masked = matFlags & 0x100000;
                if ((matFlags & 0x1000000) != 0)
                {
                    if (masked == 0)
                        req |= 2;
                    else
                        req |= 4;
                }
                if ((matFlags & 0x80) != 0)
                    req |= 8;
                if ((matFlags & 0x80) == 0 && (matFlags & 0x1000000) == 0)
                {
                    slot = g_opaqueEntries_guess + g_opaqueEntryCount_guess * 7;
                    dest = g_opaqueIndices_guess + g_opaqueIndexCount_guess;
                    destBase = dest;
                    if (vert < vertEnd)
                    {
                        do
                        {
                            order = *batch;
                            if (order != 0)
                            {
                                idx0 = *(unsigned short *)(vert + 0xa);
                                idx1 = *(unsigned short *)(vert + 0xc);
                                idx2 = *(unsigned short *)(vert + 0xe);
                                slot[0] = req;
                                slot[3] = 3;
                                slot[5] = (unsigned int)dest;
                                slot[1] = tex;
                                if (textured == 0)
                                {
                                    slot[6] = g_opaqueHead_guess;
                                    g_opaqueHead_guess = (unsigned int)slot;
                                }
                                else
                                {
                                    slot[6] = *(unsigned int *)(tex + 0x64);
                                    *(unsigned int *)(tex + 0x64) = (unsigned int)slot;
                                }
                                if (order == 1)
                                {
                                    dest[0] = *(unsigned short *)(remapBase + (unsigned int)idx0 * 4);
                                    dest[1] = *(unsigned short *)(remapBase + (unsigned int)idx1 * 4);
                                    dest[2] = *(unsigned short *)(remapBase + (unsigned int)idx2 * 4);
                                }
                                else
                                {
                                    dest[0] = *(unsigned short *)(remapBase + (unsigned int)idx2 * 4);
                                    dest[1] = *(unsigned short *)(remapBase + (unsigned int)idx1 * 4);
                                    dest[2] = *(unsigned short *)(remapBase + (unsigned int)idx0 * 4);
                                }
                                slot += 7;
                                dest += 3;
                                g_opaqueEntryCount_guess = g_opaqueEntryCount_guess + 1;
                                ctx->submittedPrimitiveCount++;
                            }
                            batch++;
                            vert += 0x10;
                        } while (vert < vertEnd);
                    }
                    emitted = (int)(dest - destBase);
                    if (emitted != 0 && textured != 0)
                        markTextureAsUsedInEffects_guess((int)tex);
                    g_opaqueIndexCount_guess = g_opaqueIndexCount_guess + (unsigned int)emitted;
                }
                else
                {
                    slot = g_transEntries_guess + g_transEntryCount_guess * 7;
                    dest = g_transIndices_guess + g_transIndexCount_guess;
                    destBase = dest;
                    if (vert < vertEnd)
                    {
                        do
                        {
                            order = *batch;
                            if (order != 0)
                            {
                                idx0 = *(unsigned short *)(vert + 0xa);
                                idx1 = *(unsigned short *)(vert + 0xc);
                                idx2 = *(unsigned short *)(vert + 0xe);
                                slot[0] = req;
                                slot[3] = 3;
                                slot[5] = (unsigned int)dest;
                                slot[1] = tex;
                                if (textured == 0)
                                {
                                    slot[6] = g_transHead_guess;
                                    g_transHead_guess = (unsigned int)slot;
                                }
                                else if (req == 8)
                                {
                                    slot[6] = *(unsigned int *)(tex + 0x68);
                                    *(unsigned int *)(tex + 0x68) = (unsigned int)slot;
                                }
                                else
                                {
                                    slot[6] = *(unsigned int *)(tex + 0x6c);
                                    *(unsigned int *)(tex + 0x6c) = (unsigned int)slot;
                                }
                                if (order == 1)
                                {
                                    dest[0] = *(unsigned short *)(remapBase + (unsigned int)idx0 * 4);
                                    dest[1] = *(unsigned short *)(remapBase + (unsigned int)idx1 * 4);
                                    dest[2] = *(unsigned short *)(remapBase + (unsigned int)idx2 * 4);
                                }
                                else
                                {
                                    dest[0] = *(unsigned short *)(remapBase + (unsigned int)idx2 * 4);
                                    dest[1] = *(unsigned short *)(remapBase + (unsigned int)idx1 * 4);
                                    dest[2] = *(unsigned short *)(remapBase + (unsigned int)idx0 * 4);
                                }
                                slot += 7;
                                dest += 3;
                                g_transEntryCount_guess = g_transEntryCount_guess + 1;
                                ctx->submittedPrimitiveCount++;
                            }
                            batch++;
                            vert += 0x10;
                        } while (vert < vertEnd);
                    }
                    emitted = (int)(dest - destBase);
                    if (emitted != 0 && textured != 0)
                        markTextureAsUsedInEffects_guess((int)tex);
                    g_transIndexCount_guess = g_transIndexCount_guess + (unsigned int)emitted;
                }
            }
            cursor += 3;
        } while (cursor < fence);
    }
    return 1;
}

/* fake match: reuse matList for output flags and converge the cursor paths with gotos. */
// FUNCTION: EQGFX 0x1003f610
// Queues triangles of consecutive (count, material) runs into two index lists by material flags.
// @early-stop: frame size, register allocation, and loop blocks still differ.
int __cdecl queueTrianglesFromMaterialRuns_guess(int unused, RendererBufferState_guess *ctx, unsigned char *batch, unsigned int matList)
{
    RendererMaterialEntry_guess **listBase;
    int remapBase;
    RendererStripBatch_guess *group;
    unsigned short *cursor;
    unsigned short *fence;
    unsigned char *vert;
    unsigned char *vertEnd;
    unsigned int count;
    unsigned int matFlags;
    RendererMaterialEntry_guess *entry;
    RendererTextureUse_guess *tex;
    int kind;
    int textured;
    int masked;
    unsigned int *slot;
    unsigned short *dest;
    unsigned short *destBase;
    unsigned int idx0;
    unsigned int idx1;
    unsigned int idx2;
    int emitted;
    int order;

    (void)unused;
    listBase = ((RendererStripMaterialLookup_guess *)matList)->materialEntries;
    remapBase = (int)ctx->buffer4B;
    group = ((RendererStripRequest_guess *)batch)->stripBatch;
    cursor = group->stripGroups;
    vert = (unsigned char *)group->stripRecords;
    batch = (unsigned char *)ctx->bufferSecondary;
    fence = cursor + (unsigned int)group->stripCount * 2;
    if (cursor >= fence)
        return 1;
    do
    {
        entry = listBase[cursor[1]];
        count = (unsigned int)*cursor;
        matFlags = entry->flags;
        if ((matFlags & 0x80000000) != 0)
            matFlags = ctx->indexRemap[matFlags & 0x7fffffff];
        if (matFlags == 0)
        {
            batch += count;
            cursor += 2;
            vert += count * 8;
            goto checkFence;
        }
        else
        {
            vertEnd = vert + count * 8;
            kind = ((matFlags & 3) == 2);
            textured = ((matFlags & 0xff00) != 0);
            matList = 0;
            if (kind != 0)
                matList = 1;
            masked = (int)(matFlags & 0x100000);
            if ((matFlags & 0x1000000) != 0)
            {
                if (masked == 0)
                    matList |= 2;
                else
                    matList |= 4;
            }
            if (textured == 0)
                tex = 0;
            else
                tex = *((RendererMaterialTextureBinding_guess *)entry->textureValue)->textureUse;
            if ((matFlags & 0x80) != 0)
                matList |= 8;
            else if ((matFlags & 0x1000000) == 0)
            {
                    slot = g_opaqueEntries_guess + g_opaqueEntryCount_guess * 7;
                    dest = g_opaqueIndices_guess + g_opaqueIndexCount_guess;
                    destBase = dest;
                    while (vert < vertEnd)
                    {
                        order = *batch;
                        if (order != 0)
                        {
                            idx0 = *(unsigned short *)(vert + 2);
                            idx1 = *(unsigned short *)(vert + 4);
                            idx2 = *(unsigned short *)(vert + 6);
                            slot[0] = matList;
                            slot[3] = 3;
                            slot[5] = (unsigned int)dest;
                            slot[1] = (unsigned int)tex;
                            if (textured == 0)
                            {
                                slot[6] = g_opaqueHead_guess;
                                g_opaqueHead_guess = (unsigned int)slot;
                            }
                            else
                            {
                                slot[6] = tex->opaqueNext;
                                tex->opaqueNext = (unsigned int)slot;
                            }
                            if (order == 1)
                            {
                                *dest++ = *(unsigned short *)(remapBase + idx0 * 4);
                                *dest++ = *(unsigned short *)(remapBase + idx1 * 4);
                                *dest++ = *(unsigned short *)(remapBase + idx2 * 4);
                            }
                            else
                            {
                                *dest++ = *(unsigned short *)(remapBase + idx2 * 4);
                                *dest++ = *(unsigned short *)(remapBase + idx1 * 4);
                                *dest++ = *(unsigned short *)(remapBase + idx0 * 4);
                            }
                            slot += 7;
                            g_opaqueEntryCount_guess = g_opaqueEntryCount_guess + 1;
                            ctx->submittedPrimitiveCount = ctx->submittedPrimitiveCount + 1;
                        }
                        batch++;
                        vert += 8;
                    }
                    emitted = (int)(dest - destBase);
                    if (emitted != 0 && textured != 0)
                        markTextureAsUsedInEffects_guess((int)tex);
                    g_opaqueIndexCount_guess = g_opaqueIndexCount_guess + (unsigned int)emitted;
                    goto advanceCursor;
            }
            slot = g_transEntries_guess + g_transEntryCount_guess * 7;
            dest = g_transIndices_guess + g_transIndexCount_guess;
            destBase = dest;
            while (vert < vertEnd)
            {
                order = *batch;
                if (order != 0)
                {
                    idx0 = *(unsigned short *)(vert + 2);
                    idx1 = *(unsigned short *)(vert + 4);
                    idx2 = *(unsigned short *)(vert + 6);
                    slot[0] = matList;
                    slot[3] = 3;
                    slot[5] = (unsigned int)dest;
                    slot[1] = (unsigned int)tex;
                    if (textured == 0)
                    {
                        slot[6] = g_transHead_guess;
                        g_transHead_guess = (unsigned int)slot;
                    }
                    else if (matList == 8)
                    {
                        slot[6] = tex->translucentNext;
                        tex->translucentNext = (unsigned int)slot;
                    }
                    else
                    {
                        slot[6] = tex->maskedNext;
                        tex->maskedNext = (unsigned int)slot;
                    }
                    if (order == 1)
                    {
                        *dest++ = *(unsigned short *)(remapBase + idx0 * 4);
                        *dest++ = *(unsigned short *)(remapBase + idx1 * 4);
                        *dest++ = *(unsigned short *)(remapBase + idx2 * 4);
                    }
                    else
                    {
                        *dest++ = *(unsigned short *)(remapBase + idx2 * 4);
                        *dest++ = *(unsigned short *)(remapBase + idx1 * 4);
                        *dest++ = *(unsigned short *)(remapBase + idx0 * 4);
                    }
                    slot += 7;
                    g_transEntryCount_guess = g_transEntryCount_guess + 1;
                    ctx->submittedPrimitiveCount = ctx->submittedPrimitiveCount + 1;
                }
                batch++;
                vert += 8;
            }
            emitted = (int)(dest - destBase);
            if (emitted != 0 && textured != 0)
                markTextureAsUsedInEffects_guess((int)tex);
            g_transIndexCount_guess = g_transIndexCount_guess + (unsigned int)emitted;
        }
advanceCursor:
        cursor += 2;
checkFence: ;
    } while (cursor < fence);
    return 1;
}

// FUNCTION: EQGFX 0x1003F961
// Emits the lit quads of one sprite (one per frame of its sequence) when its sphere is in the view volume.
int emitAnimatedSpriteQuads_guess(SpriteAnchor_guess *anchor, ClipContext_guess *ctx)
{
    SpriteDesc_guess *sprite;
    SpriteFrames_guess *frames;
    RendererPrimitiveDescription_guess record;
    SpriteVertex_guess *vertices;
    unsigned char *frameSequence;
    unsigned char frameId;
    int firstFrame;
    int frameDelta;
    int frameOffset;
    unsigned int renderFlags;
    int red;
    int green;
    int blue;
    float greenLevel;
    float blueLevel;
    float farLimit;
    float viewX;
    float viewY;
    float viewZ;
    float scale;
    volatile float width;
    volatile float height;
    float sphereRadius;
    float left;
    volatile float top;
    volatile float bottom;
    float frameFraction;

    sprite = ctx->sprite;
    viewX = ctx->viewMatrix[6] * (ctx->originZ + anchor->positionZ) +
            ctx->viewMatrix[3] * (ctx->originY + anchor->positionY) +
            ctx->viewMatrix[0] * (ctx->originX + anchor->positionX);
    viewY = ctx->viewMatrix[7] * (ctx->originZ + anchor->positionZ) +
            ctx->viewMatrix[4] * (ctx->originY + anchor->positionY) +
            ctx->viewMatrix[1] * (ctx->originX + anchor->positionX) +
            c_anchorLift_guess;
    viewZ = ctx->viewMatrix[8] * (ctx->originZ + anchor->positionZ) +
            ctx->viewMatrix[5] * (ctx->originY + anchor->positionY) +
            ctx->viewMatrix[2] * (ctx->originX + anchor->positionX);
    if (sprite->useMaxSize == 0 || ctx->farLimit <= sprite->maxSize)
        farLimit = ctx->farLimit;
    else
        farLimit = sprite->maxSize;
    if (EQGFX_ZERO_F_GUESS < viewZ)
    {
        if (sprite->scaleDistance <= viewZ)
        {
            scale = 1.0f;
            sphereRadius = sprite->radiusFactor;
        }
        else
        {
            scale = sprite->nearScale / (1.0f / viewZ);
            sphereRadius = scale * sprite->radiusFactor;
        }
        if (testSphereInViewVolume_guess(ctx, viewX, viewY, viewZ, farLimit, sphereRadius) != 0)
        {
            viewX = viewX * ctx->viewScaleX;
            renderFlags = sprite->renderFlags;
            frames = sprite->frames;
            viewY = viewY * ctx->viewScaleY;
            if ((renderFlags & RENDER_INDEX_REMAP_FLAG) != 0)
                renderFlags = ctx->flagMap[renderFlags & ~RENDER_INDEX_REMAP_FLAG];
            greenLevel = ctx->ambientY + c_colorBias_guess;
            renderFlags = (renderFlags & 0xffdfffbf) | 0x40000000;
            blueLevel = ctx->ambientZ + c_colorBias_guess;
            EQ_CDECL(void, 0x1004EC8D, (float, int *))((ctx->ambientX + c_colorBias_guess) * EQGFX_JITTER_SCALE_GUESS, &red);
            EQ_CDECL(void, 0x1004EC8D, (float, int *))(greenLevel * EQGFX_JITTER_SCALE_GUESS, &green);
            EQ_CDECL(void, 0x1004EC8D, (float, int *))(blueLevel * EQGFX_JITTER_SCALE_GUESS, &blue);
            if ((red & 0xffffff00) != 0)
                red = 0xff;
            if ((green & 0xffffff00) != 0)
                green = 0xff;
            if ((blue & 0xffffff00) != 0)
                blue = 0xff;
            red = (int)((unsigned int)sprite->redScale * red) >> 8;
            green = (int)((unsigned int)sprite->greenScale * green) >> 8;
            blue = (int)((unsigned int)sprite->blueScale * blue) >> 8;
            width = scale * sprite->sizeFactor * ctx->viewScaleX;
            record.textureValue = *frames->texturePtr;
            height = scale * sprite->sizeFactor * ctx->viewScaleY;
            record.diffuseValue = 0;
            record.renderState = 0;
            record.pad0019[1] = 0;
            record.words[0] = 0x37;
            left = viewX - width * g_frameFractions_guess[sprite->originXIndex] * EQGFX_MOTION_HALF_GUESS;
            top = height * g_frameFractions_guess[sprite->originYIndex] * EQGFX_MOTION_HALF_GUESS + viewY;
            *(float *)&record.words[3] = 1.0f;
            *(float *)&record.words[2] = 1.0f;
            firstFrame = frames->firstFrame;
            frameSequence = sprite->frameSequence;
            frameId = *frameSequence;
            if (frameId != 0)
            {
                bottom = top - height * g_frameFractions_guess[frames->heightIndex];
                do
                {
                    frameDelta = (int)frameId - firstFrame;
                    frameOffset = frameDelta * 8;
                    frameFraction = g_frameFractions_guess[frames->frameIndices[frameDelta]];
                    vertices = ctx->vertexOutput;
                    vertices[0].y_float_guess = bottom;
                    vertices[0].x_float_guess = left;
                    vertices[0].z_float_guess = viewZ;
                    vertices[0].u = *(float *)(frames->uvTableA + frameOffset);
                    vertices[0].v = *(float *)(frames->uvTableB + frameOffset + 4);
                    vertices[0].red_signed_guess = red;
                    vertices[0].green_signed_guess = green;
                    vertices[0].blue_signed_guess = blue;
                    vertices[1].y_float_guess = top;
                    vertices[1].x_float_guess = left;
                    vertices[1].z_float_guess = viewZ;
                    vertices[1].u = *(float *)(frames->uvTableA + frameOffset);
                    vertices[1].v = *(float *)(frames->uvTableA + frameOffset + 4);
                    vertices[1].red_signed_guess = red;
                    vertices[1].green_signed_guess = green;
                    left = width * frameFraction + left;
                    vertices[1].blue_signed_guess = blue;
                    vertices[2].y_float_guess = top;
                    vertices[2].z_float_guess = viewZ;
                    vertices[2].x_float_guess = left;
                    vertices[2].u = *(float *)(frames->uvTableB + frameOffset);
                    vertices[2].v = *(float *)(frames->uvTableA + frameOffset + 4);
                    vertices[2].red_signed_guess = red;
                    vertices[2].green_signed_guess = green;
                    vertices[2].blue_signed_guess = blue;
                    vertices[3].x_float_guess = left;
                    vertices[3].y_float_guess = bottom;
                    vertices[3].z_float_guess = viewZ;
                    vertices[3].u = *(float *)(frames->uvTableB + frameOffset);
                    vertices[3].v = *(float *)(frames->uvTableB + frameOffset + 4);
                    vertices[3].red_signed_guess = red;
                    vertices[3].green_signed_guess = green;
                    vertices[3].blue_signed_guess = blue;
                    submitRendererPrimitive_guess((int)ctx, 0, 4, &record, renderFlags);
                    frameId = frameSequence[1];
                    frameSequence = frameSequence + 1;
                } while (frameId != 0);
            }
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1003fd33
// Tests a sphere against the view volume, storing clip flags at ctx+0x400.
int __cdecl testSphereAgainstViewVolume_guess(ClipContext_guess *ctx, float posY, float posX, float posZ, float farPos, float radius)
{
    unsigned int flags;
    float scaled;
    float shifted;
    float bound;

    flags = 0;
    if (ctx->nearLimit > posZ + radius)
        return 0;
    if (posZ - radius > farPos)
        return 0;
    scaled = posZ * ctx->zFactorX;
    bound = radius * radius * ctx->radiusFactorX;
    shifted = scaled + posX;
    if (shifted * shifted > bound)
    {
        if (shifted < EQGFX_ZERO_F_GUESS)
            return 0;
    }
    else
        flags = 1;
    scaled -= posX;
    if (scaled * scaled > bound)
    {
        if (scaled < EQGFX_ZERO_F_GUESS)
            return 0;
    }
    else
        flags |= 2;
    scaled = posZ * ctx->zFactorY;
    bound = radius * radius * ctx->radiusFactorY;
    shifted = scaled + posY;
    if (shifted * shifted > bound)
    {
        if (shifted < EQGFX_ZERO_F_GUESS)
            return 0;
    }
    else
        flags |= 4;
    scaled -= posY;
    if (scaled * scaled > bound)
    {
        if (scaled < EQGFX_ZERO_F_GUESS)
            return 0;
    }
    else
        flags |= 8;
    if (posZ - radius <= ctx->nearLimit)
        flags |= 0x10;
    if (farPos <= posZ + radius)
        flags |= 0x20;
    ctx->planeMask = flags;
    return 1;
}
