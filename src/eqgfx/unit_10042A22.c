// unit_10042A22.c - eqgfx_dx7.dll, the address run 0x10042A22..0x1004366C (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define T3D_FOG_DISTANCE_SCALE EQ_GLOBAL(float, 0x10076348)
#define T3D_DEFAULT_RENDER_METHODS EQ_ARRAY(unsigned char, 0x100FA028)
#define T3D_HYBRID_COLOR_ROW_FACTOR_ADDRESS 0x10076354
#define T3D_HYBRID_COLOR_COLUMN_FACTOR_ADDRESS 0x10076350
#define T3D_HYBRID_COLOR_SCALE_ADDRESS 0x100762E8
#define T3D_HYBRID_COLOR_BIAS_ADDRESS 0x1007634C
#define T3D_HYBRID_COLOR_TABLE_SIZE 0x4000
#define T3D_HYBRID_COLOR_COLUMN_COUNT 0x100
#define T3D_HYBRID_COLOR_ROW_COUNT 0x40
#define T3D_HYBRID_COLOR_MAX 0xff
#define T3D_HYBRID_COLOR_ALLOCATION_ERROR (-49858)
#define T3D_RGB_MODEL_METHOD_OFFSET 0x14
#define T3D_WORLD_TO_EYE_TABLE EQ_GLOBAL(void *, 0x100F3FF4)
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280)
#define T3D_WORLD_TO_EYE_COEFFICIENT_ERROR (-0x2015)
#define T3D_TESSELLATION_ANGLE_LIMIT_ADDRESS 0x10076358
#define T3D_TESSELLATION_ANGLE_FALLBACK EQ_GLOBAL(float, 0x10076314)
#define T3D_FLOAT_COSINE EQ_CDECL(float, 0x1004EEA3, (float))

typedef struct RenderContext_guess {
    unsigned char reserved_000_to_107[0x108];
    void *unknown108;
    unsigned char reserved_10c_to_11f[0x14];
    void *defaultRenderMethods;
    unsigned char reserved_124_to_12f[0x0c];
    int unknown130;
    int unknown134;
    void *ownedMemory138;
    unsigned char reserved_13c_to_167[0x2c];
    int unknown168;
    void *ownedMemory16c;
    unsigned char reserved_170_to_183[0x14];
    unsigned int unknown184;
    unsigned int unknown188;
    unsigned int unknown18c;
    unsigned int unknown190;
    unsigned int unknown194;
    float screenOriginX;
    float screenOriginY;
    float minimumViewDepth;
    float unknown1a4;
    float unknown1a8;
    float worldOriginX;
    float worldOriginY;
    float worldOriginZ;
    float regionColor0;
    float regionColor1;
    float regionColor2;
    void *activeRegionObject;
    int unknown1c8;
    unsigned char reserved_1cc_to_217[0x4c];
    int clearRenderBuffer;
    union {                                 // +0x21c (both names in use)
        unsigned char backgroundColor;
        unsigned char backgroundPen;
    };
    unsigned char backgroundColorValue[4];
    unsigned char transparentPen;
    unsigned char reserved_222_to_223[0x02];
    float worldToEyeRotation00;
    float worldToEyeRotation01;
    float viewDepthFromX;
    float worldToEyeRotation10;
    float worldToEyeRotation11;
    float viewDepthFromY;
    float worldToEyeRotation20;
    float worldToEyeRotation21;
    float viewDepthFromZ;
    float screenDepthFromX;
    float screenDepthFromY;
    float screenDepthFromZ;
    float scaledScreenDepthFromX;
    float scaledScreenDepthFromY;
    float scaledScreenDepthFromZ;
    float screenXFromX;
    float screenXFromY;
    float screenXFromZ;
    float screenYFromX;
    float screenYFromY;
    float screenYFromZ;
    int resourceState;
    void *allocatedMemory;
    int unknown280;
    void *ownedMemory284;
    int unknown288;
    unsigned char reserved_28c_to_28f[0x04];
    int polygonCount;                       // +0x290
    int regionCount;                        // +0x294
    float regionColorValue0;
    float regionColorValue1;
    float regionColorValue2;
    float regionColorValue3;
    int regionBoundsMinimum;
    int regionBoundsMaximum;
    unsigned char reserved_2b0_to_3c7[0x118];
    int unknown3c8;
    unsigned char reserved_3cc_to_3d3[0x08];
    int activeRenderMethodType;
    void *activeRenderMethod;
    int pendingResourceState;
    void *pendingAllocatedMemory;
    int fogControl;
    float noFogDistance;
    float maxFogDistance;
    float fogDistanceReciprocal;
    float scaledFogDistance;
    float unknown3f8;
    void *clipResult;
    void *clipResultEnd;
    float clipDepth;
    float clipDepthReciprocal;
    float clipDepthSquared;
    float clipDepthReciprocalSquared;
    int unknown414;
    unsigned int activeRegionCount;
    unsigned int skippedRegionCount;
    void *ownedMemory420;
    unsigned char reserved_424_to_427[0x04];
    int unknown428;
    void *ownedMemory42c;
    void *ownedMemory430;
    unsigned char reserved_434_to_43b[0x08];
    int zBuffering;                         // +0x43c
    int unknown440;
    unsigned char reserved_444_to_463[0x20];
    float worldToEyeCombinedScale;
    float worldToEyeBaseScale;
    float worldToEyeScaleProduct;
    float inverseWorldToEyeBaseScaleSquared;
    float inverseWorldToEyeCombinedScaleSquared;
    int mipMapsEnabled;                     // +0x478
    float mipMapSharpness;
    unsigned char reserved_480_to_49f[0x20];
    int unknown4a0;
    unsigned char reserved_4a4_to_4af[0x0c];
    int mmxEnabled;                         // +0x4b0
    int mmxRefreshPending;                  // +0x4b4
    int unknown4b8;
    unsigned char reserved_4bc_to_4c7[0x0c];
    int unknown4c8;
    int unknown4cc;
    int unknown4d0;
    int unknown4d4;
    int tessellation;
    int unknown4dc;
    float tessellationIntensityThreshold;
    float unknown4e4;
    int tessellationMultiPolygon;
    int unknown4ec;
    int unknown4f0;
    int unknown4f4;
    unsigned char reserved_4f8_to_503[0x0c];
    float textureDepthOffset;
    float textureDepthScale;
    float texture2SolidFillProportion;
    float texture2Texture4Proportion;
    int texture2ProportionsEnabled;
    void *ownedMemory518;
    int unknown51c;
    float cameraViewportLeft;
    float cameraViewportTop;
    float cameraViewportRight;
    float cameraViewportBottom;
    float cameraFrustum[54];
    int unknown608;
    int unknown60c;
    void *ownedMemory610;
    void *ownedMemory614;
    void *ownedMemory618;
    void *ownedMemory61c;
    void *ownedMemory620;
    void *ownedMemory624;
    void *ownedMemory628;
} RenderContext_guess;

typedef struct WorldToEyeParameters_guess {
    unsigned char reserved_000_to_013[0x14];
    float heading;
    float pitch;
    float roll;
    float viewAngle;
    float scale;
} WorldToEyeParameters_guess;

typedef struct WorldToEyeTableEntry_guess {
    float scale;
    unsigned char reserved_004_to_017[0x14];
} WorldToEyeTableEntry_guess;

typedef struct WorldToEyeLookupTable_guess {
    unsigned char reserved_0000_to_608b[0x608c];
    WorldToEyeTableEntry_guess entries[0x80];
} WorldToEyeLookupTable_guess;

#define EQGFX_POINT_RADIUS_SCALE_GUESS EQ_GLOBAL(float, 0x100762F0)
#define g_render_mode_guess EQ_GLOBAL(int, 0x100FB8A8)
#define T3D_GR_CLIP_WINDOW EQ_GLOBAL(void *, 0x100F3E9C)
#define g_gr_buffer_clear_guess EQ_GLOBAL(void *, 0x100F3E8C)
#define T3D_GR_BUFFER_CLEAR_CONTEXT EQ_GLOBAL(void *, 0x100FBA98)
#define EQGFX_DEPTH_CLEAR_VALUE EQ_GLOBAL(unsigned int, 0x100FBA74)
#define EQGFX_DISPLAY_WIDTH EQ_GLOBAL(int, 0x100FBA40)
#define g_display_height_guess EQ_GLOBAL(int, 0x100FBA44)
#define g_fog_color_raw_guess EQ_GLOBAL(unsigned int, 0x100FBA20)
#define g_clear_helper_object_guess EQ_GLOBAL(void *, 0x100F3FE4)
#define g_render_device_guess EQ_GLOBAL(void *, 0x100FBA6C)
#define EQGFX_BOUND_POINT_LIGHT_COUNT EQ_GLOBAL(int, 0x100AAD7C)
#define T3D_D3DERR_SCENE_IN_SCENE (-0x7789FD08)
#define T3D_D3DERR_ZBUFFER_NOTPRESENT (-0x7789F7EA)
#define T3D_RENDER_REGION_TYPE 0x25
#define T3D_RENDER_METHOD_TYPE 0x48
#define T3D_RENDER_AREA_TYPE 0x27
#define T3D_RENDER_AREA_ALT_TYPE 0x29
#define T3D_PARTICLE_CLOUD_TYPE 0x4d

typedef struct CameraParameters_guess {
    unsigned int unknown00;
    unsigned int regionArrayIndex;
    float eyeX;
    float eyeY;
    float eyeZ;
    unsigned char reserved_14_to_1f[0x0c];
    float unknown20;
    float projectionScale;
    float depthNear;
    float depthNearOffset;
    float depthFar;
    unsigned char reserved_34_to_3f[0x0c];
    unsigned int cameraValue40;
    unsigned int cameraValue44;
    unsigned int cameraValue48;
    unsigned int cameraValue4c;
    int currentRegionIndex;
    float cameraValue54;
    float cameraValue58;
    float cameraValue5c;
    float cameraValue60;
    float cameraValue64;
    float cameraValue68;
    float cameraValue6c;
    float cameraValue70;
    float cameraValue74;
    float cameraValue78;
    float cameraValue7c;
    float cameraValue80;
    float cameraValue84;
    float cameraValue88;
    float cameraValue8c;
    float cameraValue90;
    float cameraValue94;
    float cameraValue98;
    float cameraValue9c;
    float cameraValuea0;
    float cameraValuea4;
    float cameraValuea8;
    float cameraValueac;
    float cameraValueb0;
    float cameraValueb4;
    float cameraValueb8;
    float cameraValuebc;
    float cameraValuec0;
    float cameraValuec4;
    float cameraValuec8;
    float cameraValuecc;
    float cameraValueD0;
    float clippingEnabled;
} CameraParameters_guess;

typedef struct TextureFrameSet_guess {
    unsigned char reserved_00_to_13[0x14];
    unsigned int frameCount;
    union {
        float singleValue;
        float *frameValues;
    } values;
    float *rgbValues;
} TextureFrameSet_guess;

typedef struct TextureAnimation_guess {
    unsigned char reserved_00_to_0b[0x0c];
    unsigned char flags;
    unsigned char reserved_0d_to_13[0x07];
    unsigned int frameIndex;
    unsigned int frameDuration;
    unsigned int nextFrameTime;
    int loopMode;
    TextureFrameSet_guess *frameSet;
} TextureAnimation_guess;

typedef struct ZoneRegion_guess {
    unsigned char reserved_00_to_0f[0x10];
    TextureAnimation_guess *textureAnimation;
    unsigned char reserved_14_to_1f[0x0c];
    float textureRadius;
} ZoneRegion_guess;

typedef struct SceneNode_guess {
    unsigned char reserved_00_to_53[0x54];
    ZoneRegion_guess *zoneRegion;
} SceneNode_guess;

typedef struct SceneObject_guess {
    int type;
    unsigned char reserved_04_to_0e[0x0b];
    unsigned char flags;
    float x;
    float y;
    float z;
    unsigned char reserved_1c_to_23[0x08];
    void *renderData;
    unsigned char reserved_28_to_43[0x1c];
    unsigned int *vertexIndexTable;
    unsigned char reserved_48_to_5f[0x18];
    unsigned int renderCategory;
} SceneObject_guess;

typedef struct RegionAreaList_guess {
    unsigned int type;
    unsigned int areaCount;
    unsigned int unknown08;
    unsigned int *areaList;
    int areaMinimum;
    int unknown14;
    int areaMaximum;
} RegionAreaList_guess;

typedef struct RegionRenderMethodState_guess {
    unsigned char reserved_00_to_13[0x14];
    int state;
} RegionRenderMethodState_guess;

typedef struct RegionRenderMethod_guess {
    unsigned int type;
    unsigned char reserved_04_to_33[0x30];
    RegionRenderMethodState_guess *stateData;
} RegionRenderMethod_guess;

typedef struct RegionRenderData_guess {
    unsigned int type;
    TextureAnimation_guess *textureAnimation;
    unsigned char reserved_08_to_1b[0x14];
    RegionRenderMethod_guess *renderMethod;
} RegionRenderData_guess;

typedef struct SceneRegion_guess {
    unsigned char reserved_00_to_0e[0x0f];
    unsigned char flags;
    float x;
    float geometryData[3];
    unsigned int vertexCount;
    RegionRenderData_guess *renderData;
    unsigned char reserved_28_to_2b[0x04];
    RegionAreaList_guess *areaList;
    unsigned char reserved_30_to_5f[0x30];
    unsigned int renderCategory;
} SceneRegion_guess;

typedef struct Scene_guess {
    unsigned char reserved_00_to_07[0x08];
    unsigned int frameTime;
    unsigned char reserved_0c_to_23[0x18];
    SceneRegion_guess **regionTable;
    unsigned char reserved_28_to_17b[0x154];
    SceneNode_guess **sceneNodes;
    unsigned char reserved_180_to_1c3[0x44];
    unsigned int boundLightCount;
    unsigned char reserved_1c8_to_1cb[0x04];
    unsigned int *boundLightObjects;
    unsigned char reserved_1d0_to_233[0x64];
    unsigned int particleCount;
    unsigned char reserved_238_to_23b[0x04];
    SceneObject_guess **particleObjects;
    unsigned char reserved_240_to_337[0xf8];
    unsigned int regionQueueCount;
    unsigned int *regionQueue;
} Scene_guess;

/* T3DDevice7_guess: the T3DDevice7_guess layout is T3DDevice7_guess (generated: eqgfx/layout/d3d_device7.h); the old local view's members map by offset. */
#include "eqgfx/layout/d3d_device7.h"

// FUNCTION: EQGFX 0x10042a22
// Assigns a custom rendering method to a pending render-context resource.
int __cdecl t3dSetRenderContextUserDefinedRenderMethod(
    RenderContext_guess *renderContext,
    unsigned int methodIndex,
    unsigned int renderMethod)
{
    unsigned int maskedIndex;

    maskedIndex = methodIndex & 0x7fffffff;
    if ((maskedIndex < (unsigned int)renderContext->pendingResourceState) &&
        ((renderMethod & 0xfeefffff) < 0x80000000)) {
        ((unsigned int *)renderContext->pendingAllocatedMemory)[maskedIndex] = renderMethod;
        return 1;
    }
    return 0xffffdfec;
}

// FUNCTION: EQGFX 0x10042a63
// Sets resourceState, allocates resourceCount * 12 bytes; on failure clears resourceState and returns -1.
int __cdecl t3dAllocateRenderContextResourceStorage_guess(RenderContext_guess *renderContext, int resourceCount)
{
    int result;
    void *allocated;

    renderContext->resourceState = resourceCount;
    result = 1;
    allocated = EQ_CDECL(void *, 0x10050C48, (int))(resourceCount * 12);
    renderContext->allocatedMemory = allocated;
    if (allocated == 0) {
        renderContext->resourceState = 0;
        result = -1;
    }
    return result;
}

// FUNCTION: EQGFX 0x10042a9b
// Releases the active resources owned by a render context.
int __cdecl t3dFreeRenderContextResources_guess(RenderContext_guess *renderContext)
{
    renderContext->resourceState = 0;
    if (renderContext->allocatedMemory != 0) {
        EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->allocatedMemory);
        renderContext->allocatedMemory = 0;
    }
    return 1;
}

// FUNCTION: EQGFX 0x10042AC6
// Stores count in pendingResourceState, allocates count dwords into pendingAllocatedMemory and fills them with 7.
int __cdecl _t3dInitializePendingResourceArray_guess(RenderContext_guess *renderContext, unsigned int count)
{
    int result;
    int *allocated;
    int *end;
    int *cursor;

    result = 1;
    renderContext->pendingResourceState = count;
    if (count > 0) {
        allocated = (int *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(count * sizeof(int));
        renderContext->pendingAllocatedMemory = allocated;
        if (allocated == 0) {
            renderContext->pendingResourceState = 0;
            result = -1;
        } else {
            end = allocated + count;
            for (cursor = allocated; cursor < end; cursor++) {
                *cursor = 7;
            }
        }
    } else {
        renderContext->pendingAllocatedMemory = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x10042b2d
// Releases the pending resources owned by a render context.
int __cdecl t3dFreePendingRenderContextResources_guess(RenderContext_guess *renderContext)
{
    renderContext->pendingResourceState = 0;
    if (renderContext->pendingAllocatedMemory != 0) {
        EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->pendingAllocatedMemory);
        renderContext->pendingAllocatedMemory = 0;
    }
    return 1;
}

// FUNCTION: EQGFX 0x10042b69
// Sets the tessellation level used to refine rendered geometry.
int __cdecl t3dSetRenderContextTessellation(RenderContext_guess *renderContext, int tessellation)
{
    renderContext->tessellation = tessellation;
    return 0;
}

// FUNCTION: EQGFX 0x10042b7a
// Creates a render context with default drawing settings and working buffers.
unsigned int __cdecl t3dCreateRenderContext(unsigned int elementCount)
{
    int allocationResult;
    unsigned int *elementCountAddress;
    unsigned int contextAddress;
    RenderContext_guess *renderContext;

    elementCountAddress = &elementCount;
    allocationResult = EQ_CDECL(int, 0x10050C48, (int))(0x64b);
    if (allocationResult != 0)
    {
        contextAddress = ((unsigned int)allocationResult + 0x1f) & 0xffffffe0;
        renderContext = (RenderContext_guess *)contextAddress;
        renderContext->ownedMemory518 = (void *)allocationResult;
        renderContext->unknown1c8 = 0x37;
        renderContext->transparentPen = 0xfd;
        renderContext->clearRenderBuffer = 0;
        renderContext->backgroundColor = 0;
        renderContext->backgroundColorValue[2] = 0;
        renderContext->backgroundColorValue[1] = 0;
        renderContext->backgroundColorValue[0] = 0;
        renderContext->unknown288 = 0;
        renderContext->polygonCount = 0;
        renderContext->regionCount = 0;
        renderContext->unknown4ec = 0;
        renderContext->unknown4f0 = 0;
        renderContext->unknown4f4 = 0;
        renderContext->mmxRefreshPending = 0;
        renderContext->unknown4d4 = 0;
        renderContext->unknown608 = 0;
        renderContext->unknown60c = 0;
        renderContext->ownedMemory618 = 0;
        renderContext->ownedMemory61c = 0;
        renderContext->ownedMemory620 = 0;
        renderContext->ownedMemory610 = 0;
        renderContext->ownedMemory614 = 0;
        renderContext->ownedMemory624 = 0;
        renderContext->ownedMemory628 = 0;
        renderContext->unknown4a0 = 0;
        renderContext->unknown4b8 = 0;
        renderContext->unknown4c8 = 0;
        renderContext->unknown4cc = 0;
        renderContext->unknown130 = 0;
        renderContext->unknown134 = 0;
        renderContext->unknown4d0 = 0;
        renderContext->ownedMemory138 = 0;
        renderContext->unknown168 = 0;
        renderContext->ownedMemory16c = 0;
        renderContext->unknown4dc = 0;
        renderContext->tessellation = 0;
        renderContext->tessellationIntensityThreshold = 0.1f;
        renderContext->unknown4e4 = -1.0f;
        renderContext->tessellationMultiPolygon = 0;

        allocationResult = EQ_CDECL(int, 0x10043066, (void))();
        if (allocationResult != 0)
            renderContext->mmxEnabled = 1;
        else
            renderContext->mmxEnabled = 0;

        renderContext->unknown3c8 = 1;
        renderContext->unknown51c = 0;
        renderContext->ownedMemory284 = 0;
        renderContext->unknown280 = 0;

        allocationResult = EQ_CDECL(int, 0x10042A63, (RenderContext_guess *, int))(renderContext, 0x400);
        if ((allocationResult >= 0) &&
            ((allocationResult = EQ_CDECL(int, 0x1002FCF2, (RenderContext_guess *, int))(renderContext, 0x20)) >= 0) &&
            ((allocationResult = EQ_CDECL(int, 0x10031D7D, (RenderContext_guess *, int))(renderContext, 0x40)) >= 0) &&
            ((allocationResult = EQ_CDECL(int, 0x10042AC6, (RenderContext_guess *, unsigned int))(renderContext, *elementCountAddress)) >= 0))
        {
            allocationResult = EQ_CDECL(int, 0x10050C48, (int))(0x280);
            renderContext->ownedMemory420 = (void *)allocationResult;
            if (allocationResult != 0)
            {
                renderContext->unknown414 = 0x20;
                allocationResult = EQ_CDECL(int, 0x10050C48, (int))(0xc);
                renderContext->ownedMemory42c = (void *)allocationResult;
                if (allocationResult != 0)
                {
                    allocationResult = EQ_CDECL(int, 0x10050C48, (int))(4);
                    renderContext->ownedMemory430 = (void *)allocationResult;
                    if (allocationResult != 0)
                    {
                        renderContext->unknown428 = 1;
                        renderContext->unknown3f8 = 0.0f;
                        renderContext->fogControl = 0;
                        renderContext->noFogDistance = 0.0f;
                        renderContext->maxFogDistance = 1.0f;
                        renderContext->fogDistanceReciprocal = 1.0f;
                        EQ_CDECL(void, 0x1004300E, (RenderContext_guess *, float, float))(renderContext, 0.0f, 0.0f);
                        renderContext->unknown288 = 0;
                        EQ_CDECL(void, 0x10042B58, (RenderContext_guess *, int))(renderContext, 0);
                        renderContext->unknown440 = 0;
                        renderContext->defaultRenderMethods = T3D_DEFAULT_RENDER_METHODS;

                        allocationResult = EQ_CDECL(int, 0x10050C48, (int))(1000);
                        renderContext->ownedMemory618 = (void *)allocationResult;
                        if (allocationResult != 0)
                        {
                            renderContext->unknown60c = 1000;
                            allocationResult = EQ_CDECL(int, 0x10050C48, (int))(1000);
                            renderContext->ownedMemory61c = (void *)allocationResult;
                            if (allocationResult != 0)
                            {
                                allocationResult = EQ_CDECL(int, 0x10050C48, (int))(1000);
                                renderContext->ownedMemory620 = (void *)allocationResult;
                                if (allocationResult != 0)
                                {
                                    allocationResult = EQ_CDECL(int, 0x10050C48, (int))(12000);
                                    renderContext->ownedMemory610 = (void *)allocationResult;
                                    if (allocationResult != 0)
                                    {
                                        allocationResult = EQ_CDECL(int, 0x10050C48, (int))(12000);
                                        renderContext->ownedMemory614 = (void *)allocationResult;
                                        if (allocationResult != 0)
                                        {
                                            allocationResult = EQ_CDECL(int, 0x10050C48, (int))(4000);
                                            renderContext->ownedMemory624 = (void *)allocationResult;
                                            if (allocationResult != 0)
                                            {
                                                allocationResult = EQ_CDECL(int, 0x10050C48, (int))(4000);
                                                renderContext->ownedMemory628 = (void *)allocationResult;
                                                if (allocationResult != 0)
                                                    return contextAddress;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        EQ_CDECL(int, 0x10042E65, (int, RenderContext_guess *))(0, renderContext);
        *elementCountAddress = 0;
    }
    return *elementCountAddress;
}

// FUNCTION: EQGFX 0x10042e65
// Destroys a render context and releases its drawing buffers and other owned memory.
int __cdecl t3dDestroyRenderContext(int unused, RenderContext_guess *renderContext)
{
    int result;

    result = 1;
    if (renderContext != 0)
    {
        if (renderContext->ownedMemory16c != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory16c);
        if (renderContext->ownedMemory138 != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory138);
        if (renderContext->ownedMemory420 != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory420);
        if (renderContext->ownedMemory42c != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory42c);
        if (renderContext->ownedMemory430 != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory430);
        result = EQ_CDECL(int, 0x10042A9B, (RenderContext_guess *))(renderContext);
        if (result >= 0)
        {
            result = EQ_CDECL(int, 0x1002FD99, (RenderContext_guess *))(renderContext);
            if (result >= 0)
            {
                if (renderContext->ownedMemory284 != 0)
                    EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory284);
                result = EQ_CDECL(int, 0x10031DB0, (RenderContext_guess *))(renderContext);
                if (result >= 0)
                {
                    result = EQ_CDECL(int, 0x10042B2D, (RenderContext_guess *))(renderContext);
                    if (result >= 0)
                    {
                        if (renderContext->ownedMemory618 != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory618);
                        if (renderContext->ownedMemory61c != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory61c);
                        if (renderContext->ownedMemory620 != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory620);
                        if (renderContext->ownedMemory610 != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory610);
                        if (renderContext->ownedMemory614 != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory614);
                        if (renderContext->ownedMemory624 != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory624);
                        if (renderContext->ownedMemory628 != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory628);
                        EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory518);
                    }
                }
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10042fb3
// Sets which pen renders as transparent in this context.
int __cdecl t3dSetRenderContextTransparentPen(RenderContext_guess *renderContext, unsigned char transparentPen)
{
    renderContext->transparentPen = transparentPen;
    return 0;
}

// FUNCTION: EQGFX 0x10042fcf
// Sets whether the render buffer is cleared before drawing.
int __cdecl t3dSetRenderContextClearRenderBuffer(RenderContext_guess *renderContext, int clearRenderBuffer)
{
    renderContext->clearRenderBuffer = clearRenderBuffer;
    return 0;
}

// FUNCTION: EQGFX 0x10042FEB
// Reports the mip-map sharpness used for this render context.
float __cdecl t3dGetRenderContextMipMapSharpness(RenderContext_guess *renderContext)
{
    return renderContext->mipMapSharpness;
}

// FUNCTION: EQGFX 0x10042FF6
// Reports the texture 2 share used in the render context's texture blend.
float __cdecl t3dGetRenderContextTexture2SolidFillProportion(RenderContext_guess *renderContext)
{
    return 1.0f - renderContext->texture2SolidFillProportion;
}

// FUNCTION: EQGFX 0x10043003
// Reports the texture 4 share of the texture 2 and texture 4 blend.
float __cdecl t3dGetRenderContextTexture2Texture4Proportion(RenderContext_guess *renderContext)
{
    return renderContext->texture2Texture4Proportion;
}

// FUNCTION: EQGFX 0x1004300E
// Sets how texture 2 and texture 4 contribute to the surface blend.
int __cdecl t3dSetRenderContextTexture2Proportions(RenderContext_guess *renderContext, float texture2Proportion, float texture4Proportion)
{
    *(unsigned int*)&renderContext->texture2Texture4Proportion = *(unsigned int*)&texture4Proportion;
    renderContext->texture2SolidFillProportion = 1.0f - texture2Proportion;
    if ((texture2Proportion > EQGFX_ZERO_F_GUESS) || (texture4Proportion > EQGFX_ZERO_F_GUESS))
    {
        renderContext->texture2ProportionsEnabled = 1;
    }
    else
    {
        renderContext->texture2ProportionsEnabled = 0;
    }
    return 0;
}

// FUNCTION: EQGFX 0x100430FC
// Sets the render context's background pen and optional color value.
int __cdecl t3dSetRenderContextBackgroundColor(RenderContext_guess *renderContext, unsigned char backgroundColor, unsigned int *colorValue)
{
    renderContext->backgroundColor = backgroundColor;
    if (colorValue != 0)
    {
        *(unsigned int*)renderContext->backgroundColorValue = *colorValue;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004311D
// Reports which fog mode is selected for the scene.
int __cdecl t3dGetRenderContextFogControl(RenderContext_guess *renderContext)
{
    return renderContext->fogControl;
}

// FUNCTION: EQGFX 0x10043128
// Reports the maximum distance for scene fog.
float __cdecl t3dGetRenderContextMaxFogDistance(RenderContext_guess *renderContext)
{
    return renderContext->maxFogDistance;
}

// FUNCTION: EQGFX 0x10043133
// Sets the scene fog mode and its maximum distance.
int __cdecl t3dSetRenderContextFogControl(RenderContext_guess *renderContext, int fogControl, float fogDistance)
{
    if ((fogControl != 0) && (fogControl != 1) && (fogControl != 2))
    {
        return -1;
    }
    renderContext->fogControl = fogControl;
    if (fogControl != 0)
    {
        if (fogDistance <= EQGFX_ZERO_F_GUESS)
        {
            return -2;
        }
        renderContext->maxFogDistance = fogDistance;
        renderContext->fogDistanceReciprocal = 1.0f / fogDistance;
        renderContext->scaledFogDistance = fogDistance * T3D_FOG_DISTANCE_SCALE;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10043192
// Sets how far the scene stays clear before fog begins.
// Rejects NaN (fcomp sets CF, jb). The minimum is read through a pointer: with the global written directly, the
// test DLL's literal-address EQ_GLOBAL makes VC6 swap the fcomp operands (fld [min]; fcomp v; ja), which
// accepts NaN, while the match build keeps this order (VC6_QUIRKS.md "literal-address FP compare").
int __cdecl t3dSetRenderContextNoFogDistance(RenderContext_guess *renderContext, float noFogDistance)
{
    const float *minimum = &EQGFX_ZERO_F_GUESS;

    if (noFogDistance >= *minimum)
    {
        renderContext->noFogDistance = noFogDistance;
        return 0;
    }
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x100431B8
// Reports how far the scene stays clear before fog begins.
float __cdecl t3dGetRenderContextNoFogDistance(RenderContext_guess *renderContext)
{
    return renderContext->noFogDistance;
}

// FUNCTION: EQGFX 0x1004355C
// Projects a world position onto the screen.
int __cdecl t3dWorldSpaceToScreenSpace(RenderContext_guess *renderContext, float *worldPosition, float *screenX, float *screenY)
{
    float translatedX;
    float translatedY;
    float translatedZ;
    float viewDepth;
    float inverseViewDepth;

    translatedX = renderContext->worldOriginX + worldPosition[0];
    translatedY = renderContext->worldOriginY + worldPosition[1];
    translatedZ = renderContext->worldOriginZ + worldPosition[2];
    viewDepth = translatedX * renderContext->viewDepthFromX +
        translatedY * renderContext->viewDepthFromY +
        translatedZ * renderContext->viewDepthFromZ;
    if (renderContext->minimumViewDepth < viewDepth)
    {
        inverseViewDepth = 1.0f / viewDepth;
        *screenX = inverseViewDepth * (translatedX * renderContext->screenXFromX +
            translatedY * renderContext->screenXFromY +
            translatedZ * renderContext->screenXFromZ) + renderContext->screenOriginX;
        *screenY = renderContext->screenOriginY - inverseViewDepth * (translatedX * renderContext->screenYFromX +
            translatedY * renderContext->screenYFromY +
            translatedZ * renderContext->screenYFromZ);
        return 0;
    }
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x10043618
// Sets the intensity threshold that controls geometry tessellation.
int __cdecl t3dSetRenderContextTessellationIntensityThreshold(RenderContext_guess *renderContext, float tessellationIntensityThreshold)
{
    renderContext->tessellationIntensityThreshold = tessellationIntensityThreshold;
    return 0;
}

// FUNCTION: EQGFX 0x10043629
// Sets the angle cutoff used when tessellating rendered surfaces.
int __cdecl t3dSetRenderContextTessellationAngleThreshold(RenderContext_guess *renderContext, float tessellationAngleThreshold)
{
    float cosineValue;

    __asm {
        mov eax, T3D_TESSELLATION_ANGLE_LIMIT_ADDRESS
        fld tessellationAngleThreshold
        fcomp dword ptr [eax]
        fnstsw ax
        sahf
        jc tessellationAngleUsesCosine
    }
    cosineValue = T3D_TESSELLATION_ANGLE_FALLBACK;
    goto tessellationAngleValueReady;

tessellationAngleUsesCosine:
    cosineValue = T3D_FLOAT_COSINE(tessellationAngleThreshold);

tessellationAngleValueReady:
    renderContext->unknown4e4 = cosineValue;
    return 0;
}

// FUNCTION: EQGFX 0x1004365B
// Controls whether tessellated geometry can be emitted as multiple polygons.
int __cdecl t3dSetRenderContextTessellationMultiPolygon(RenderContext_guess *renderContext, int tessellationMultiPolygon)
{
    renderContext->tessellationMultiPolygon = tessellationMultiPolygon;
    return 0;
}
// FUNCTION: EQGFX 0x10042b58
// __cdecl. Sets the z-buffering flag at +0x43c of the render context. Returns 0.
int __cdecl t3dSetRenderContextZBuffering(RenderContext_guess *renderContext, int zBuffering)
{
    renderContext->zBuffering = zBuffering;
    return 0;
}

// FUNCTION: EQGFX 0x10042fa8
// __cdecl. Returns the transparent pen at +0x221 of the render context.
unsigned char __cdecl t3dGetRenderContextTransparentPen(RenderContext_guess *renderContext)
{
    return renderContext->transparentPen;
}

// FUNCTION: EQGFX 0x10042fc4
// __cdecl. Returns the clear-render-buffer flag at +0x218 of the render context.
int __cdecl t3dGetRenderContextClearRenderBuffer(RenderContext_guess *renderContext)
{
    return renderContext->clearRenderBuffer;
}

// FUNCTION: EQGFX 0x10042fe0
// __cdecl. Returns the mip-maps-enabled flag at +0x478 of the render context.
int __cdecl t3dGetRenderContextMipMapsEnabled(RenderContext_guess *renderContext)
{
    return renderContext->mipMapsEnabled;
}

// FUNCTION: EQGFX 0x1004305b
// __cdecl. Returns the MMX-enabled flag at +0x4b0 of the render context.
int __cdecl t3dGetRenderContextMMXEnabled(RenderContext_guess *renderContext)
{
    return renderContext->mmxEnabled;
}

// FUNCTION: EQGFX 0x10043066
// Probes CPUID by toggling the ID flag, then reports CPUID leaf 1 EDX bit 23 (MMX).
int __cdecl isMMXSupported_guess(void)
{
    __asm {
        push ebx
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 0x200000
        push eax
        popfd
        pushfd
        pop eax
        push ecx
        popfd
        xor eax, ecx
        jz no_cpuid
        mov eax, 1
        cpuid
        test edx, 0x800000
        jz no_cpuid
        mov eax, 1
        jmp done
    no_cpuid:
        xor eax, eax
    done:
        pop ebx
    }
}

// FUNCTION: EQGFX 0x10043096
// Controls whether this render context uses MMX-accelerated rendering.
// __cdecl. Sets the MMX-enabled flag at +0x4b0 (enabling returns 0xffff3d3e without MMX), then calls 0x100431c3 if +0x4b4 is set.
int __cdecl t3dSetRenderContextMMXEnabled(RenderContext_guess *renderContext, int enabled)
{
    int result;

    if (enabled != 0)
    {
        if (EQ_CDECL(int, 0x10043066, (void))() != 0)
        {
            renderContext->mmxEnabled = 1;
        }
        else
        {
            renderContext->mmxEnabled &= 0;
            result = 0xffff3d3e;
            goto notify;
        }
    }
    else
    {
        renderContext->mmxEnabled &= 0;
    }
    result = 0;
notify:
    if (renderContext->mmxRefreshPending != 0)
    {
        EQ_CDECL(void, 0x100431C3, (RenderContext_guess *))(renderContext);
    }
    return result;
}

// FUNCTION: EQGFX 0x100430e6
// Returns the background pen at +0x21c of the render context.
unsigned char __cdecl t3dGetRenderContextBackgroundPen(RenderContext_guess *renderContext)
{
    return renderContext->backgroundPen;
}

// FUNCTION: EQGFX 0x100430F1
// Returns the background RGB value at +0x21d of the render context.
unsigned int __cdecl t3dGetRenderContextBackgroundRGB(RenderContext_guess *renderContext)
{
    return *(unsigned int *)renderContext->backgroundColorValue;
}

// FUNCTION: EQGFX 0x100431c4
// Selects an RGB model for the render context and prepares its color lookup table when enabled.
int __cdecl t3dSetRenderContextRGBModel(
    RenderContext_guess *renderContext,
    int enabled,
    void *rgbModel)
{
    unsigned char *colorTable;
    unsigned int row;
    unsigned int column;
    unsigned int colorValue;
    __int64 rowValue;
    __int64 columnValue;
    int result;

    result = 0;
    if (enabled != 0)
    {
        if (rgbModel == 0)
        {
            result = T3D_HYBRID_COLOR_ALLOCATION_ERROR;
            goto done;
        }
        renderContext->unknown108 = (unsigned char *)rgbModel + T3D_RGB_MODEL_METHOD_OFFSET;
        if (renderContext->ownedMemory138 == 0)
        {
            colorTable = EQ_CDECL(unsigned char *, 0x10050C48, (int))(T3D_HYBRID_COLOR_TABLE_SIZE);
            if (colorTable == 0)
            {
                result = T3D_HYBRID_COLOR_ALLOCATION_ERROR;
                goto done;
            }
            renderContext->ownedMemory138 = colorTable;
            row = 0;
            do
            {
                rowValue = row;
                __asm {
                    fild qword ptr rowValue
                    mov eax, T3D_HYBRID_COLOR_ROW_FACTOR_ADDRESS
                    fmul dword ptr [eax]
                }
                column = 0;
                do
                {
                    columnValue = column;
                    __asm {
                        fild qword ptr columnValue
                        mov eax, T3D_HYBRID_COLOR_COLUMN_FACTOR_ADDRESS
                        fmul dword ptr [eax]
                        fmul st(0), st(1)
                        mov eax, T3D_HYBRID_COLOR_SCALE_ADDRESS
                        fmul dword ptr [eax]
                        mov eax, T3D_HYBRID_COLOR_BIAS_ADDRESS
                        fadd dword ptr [eax]
                    }
                    colorValue = EQ_CDECL(unsigned int, 0x1004EC36, (void))();
                    if (colorValue > T3D_HYBRID_COLOR_MAX)
                    {
                        colorValue = T3D_HYBRID_COLOR_MAX;
                    }
                    *colorTable = (unsigned char)colorValue;
                    colorTable++;
                    column++;
                } while (column < T3D_HYBRID_COLOR_COLUMN_COUNT);
                row++;
                __asm {
                    fstp st(0)
                }
            } while (row < T3D_HYBRID_COLOR_ROW_COUNT);
        }
        renderContext->unknown4b8 &= 0;
    }
    renderContext->mmxRefreshPending = enabled;
done:
    return result;
}

// FUNCTION: EQGFX 0x1004328A
// Controls hybrid color rendering, preparing the shared color lookup table when first enabled.
int __cdecl t3dSetRenderContextHybridColorModel(
    RenderContext_guess *renderContext,
    int enabled)
{
    unsigned char *colorTable;
    unsigned int row;
    unsigned int column;
    unsigned int colorValue;
    __int64 rowValue;
    __int64 columnValue;
    int result;

    result = 0;
    if ((enabled != 0) && (renderContext->ownedMemory138 == 0))
    {
        colorTable = EQ_CDECL(unsigned char *, 0x10050C48, (int))(T3D_HYBRID_COLOR_TABLE_SIZE);
        if (colorTable == 0)
        {
            result = T3D_HYBRID_COLOR_ALLOCATION_ERROR;
            goto done;
        }
        row = 0;
        renderContext->ownedMemory138 = colorTable;
        do
        {
            rowValue = row;
            __asm {
                fild qword ptr rowValue
                mov eax, T3D_HYBRID_COLOR_ROW_FACTOR_ADDRESS
                fmul dword ptr [eax]
            }
            column = 0;
            do
            {
                columnValue = column;
                __asm {
                    fild qword ptr columnValue
                    mov eax, T3D_HYBRID_COLOR_COLUMN_FACTOR_ADDRESS
                    fmul dword ptr [eax]
                    fmul st(0), st(1)
                    mov eax, T3D_HYBRID_COLOR_SCALE_ADDRESS
                    fmul dword ptr [eax]
                    mov eax, T3D_HYBRID_COLOR_BIAS_ADDRESS
                    fadd dword ptr [eax]
                }
                colorValue = EQ_CDECL(unsigned int, 0x1004EC36, (void))();
                if (colorValue > T3D_HYBRID_COLOR_MAX)
                {
                    colorValue = T3D_HYBRID_COLOR_MAX;
                }
                *colorTable = (unsigned char)colorValue;
                colorTable++;
                column++;
            } while (column < T3D_HYBRID_COLOR_COLUMN_COUNT);
            row++;
            __asm {
                fstp st(0)
            }
        } while (row < T3D_HYBRID_COLOR_ROW_COUNT);
    }
    renderContext->unknown4d4 = enabled;
done:
    return result;
}

// FUNCTION: EQGFX 0x1004333C
// Prepares camera rotation and projection coefficients from heading, pitch, roll, and view angle.
int __cdecl t3dComputeWorldToEyeCoefficients(
    RenderContext_guess *renderContext,
    WorldToEyeParameters_guess *viewParameters)
{
    float cosineHeading;
    float cosinePitch;
    float cosineRoll;
    float sineHeading;
    float sineRoll;
    float sinePitch;
    float rotation00;
    float rotation01;
    float rotation10;
    float rotation11;
    float tableScale;
    float combinedScale;
    float screenOriginX;
    float screenOriginY;
    float angleForLookup;
    unsigned int tableIndex;
    WorldToEyeLookupTable_guess *lookupTable;
    int result;

    result = 1;
    cosineHeading = T3D_FLOAT_COSINE(viewParameters->heading);
    cosineRoll = T3D_FLOAT_COSINE(viewParameters->roll);
    cosinePitch = T3D_FLOAT_COSINE(viewParameters->pitch);
    sineHeading = EQ_CDECL(float, 0x1004EE37, (float))(viewParameters->heading);
    sineRoll = EQ_CDECL(float, 0x1004EE37, (float))(viewParameters->roll);
    sinePitch = EQ_CDECL(float, 0x1004EE37, (float))(viewParameters->pitch);
    angleForLookup = viewParameters->viewAngle;
    __asm {
        fld dword ptr [angleForLookup]
    }
    tableIndex = EQ_CDECL(unsigned int, 0x1004EC36, (void))();
    if ((tableIndex == 0) || (tableIndex > 0x7f))
    {
        result = T3D_WORLD_TO_EYE_COEFFICIENT_ERROR;
    }
    else
    {
        renderContext->viewDepthFromZ = sinePitch;
        renderContext->viewDepthFromX = cosinePitch * cosineHeading;
        renderContext->viewDepthFromY = sineHeading * cosinePitch;
        renderContext->worldToEyeRotation20 = -(sineRoll * cosinePitch);
        renderContext->worldToEyeRotation21 = cosinePitch * cosineRoll;
        rotation00 = sineHeading * cosineRoll + sinePitch * sineRoll * cosineHeading;
        renderContext->worldToEyeRotation00 = rotation00;
        rotation10 = sinePitch * sineRoll * sineHeading - cosineRoll * cosineHeading;
        renderContext->worldToEyeRotation10 = rotation10;
        rotation01 = sineRoll * sineHeading - sinePitch * cosineRoll * cosineHeading;
        renderContext->worldToEyeRotation01 = rotation01;
        rotation11 = -(sineRoll * cosineHeading) - sinePitch * cosineRoll * sineHeading;
        renderContext->worldToEyeRotation11 = rotation11;
        lookupTable = (WorldToEyeLookupTable_guess *)T3D_WORLD_TO_EYE_TABLE;
        tableScale = lookupTable->entries[tableIndex].scale;
        screenOriginX = renderContext->screenOriginX;
        renderContext->screenDepthFromX = rotation00 * tableScale;
        renderContext->screenXFromX = renderContext->screenDepthFromX * screenOriginX;
        renderContext->screenDepthFromY = rotation10 * tableScale;
        renderContext->screenXFromY = renderContext->screenDepthFromY * screenOriginX;
        renderContext->screenDepthFromZ = renderContext->worldToEyeRotation20 * tableScale;
        renderContext->screenXFromZ = renderContext->screenDepthFromZ * screenOriginX;
        combinedScale = tableScale * viewParameters->scale;
        screenOriginY = renderContext->screenOriginY;
        renderContext->scaledScreenDepthFromX = rotation01 * combinedScale;
        renderContext->screenYFromX = renderContext->scaledScreenDepthFromX * screenOriginY;
        renderContext->scaledScreenDepthFromY = rotation11 * combinedScale;
        renderContext->screenYFromY = renderContext->scaledScreenDepthFromY * screenOriginY;
        renderContext->scaledScreenDepthFromZ = renderContext->worldToEyeRotation21 * combinedScale;
        renderContext->screenYFromZ = renderContext->scaledScreenDepthFromZ * screenOriginY;
        renderContext->worldToEyeCombinedScale = combinedScale;
        renderContext->worldToEyeBaseScale = tableScale;
        renderContext->worldToEyeScaleProduct = combinedScale * tableScale;
        renderContext->inverseWorldToEyeBaseScaleSquared =
            EQGFX_FLOAT_ONE / (tableScale * tableScale);
        renderContext->inverseWorldToEyeCombinedScaleSquared =
            EQGFX_FLOAT_ONE / (combinedScale * combinedScale);
    }
    return result;
}

// FUNCTION: EQGFX 0x10043546
// Returns the polygon count at +0x290 of the render context.
int __cdecl t3dGetPolygonCount(RenderContext_guess *renderContext)
{
    return renderContext->polygonCount;
}

// FUNCTION: EQGFX 0x10043551
// Returns the region count at +0x294 of the render context.
int __cdecl t3dGetRegionCount(RenderContext_guess *renderContext)
{
    return renderContext->regionCount;
}

#define g_initialize_window_state_guess EQ_GLOBAL(int, 0x100FB8B8)

// FUNCTION: EQGFX 0x1004366C
// Renders the world as seen from a camera: sky, the regions visible per the PVS, and the objects inside them.
int __cdecl t3dRenderPartialScene(
    Scene_guess *scene,
    RenderContext_guess *renderContext,
    CameraParameters_guess *camera,
    int *surfaceSelection)
{
    SceneNode_guess *sceneNode;
    ZoneRegion_guess *zoneRegion;
    TextureAnimation_guess *textureAnimation;
    TextureFrameSet_guess *textureFrames;
    SceneRegion_guess *cameraRegion;
    SceneRegion_guess *region;
    SceneObject_guess *sceneObject;
    RegionRenderData_guess *renderData;
    RegionAreaList_guess *areaList;
    RegionAreaList_guess *renderArea;
    RegionRenderMethod_guess *renderMethod;
    T3DDevice7_guess *device;
    float clipPlanes[54];
    float *depthTable;
    float textureScale;
    float regionDistance;
    float cameraOffsetX;
    float cameraOffsetY;
    float cameraOffsetZ;
    float viewScaleX;
    float viewScaleY;
    float viewScaleZ;
    float conversionValue;
    unsigned int regionIndex;
    unsigned int regionEnd;
    unsigned int frameDelta;
    unsigned int frameSteps;
    unsigned int frameRemainder;
    unsigned int boundIndex;
    unsigned int lightObject;
    unsigned int width;
    unsigned int height;
    unsigned int tableIndex;
    unsigned int *areaEntry;
    unsigned int *areaEnd;
    unsigned int *recordBuffer;
    unsigned int *recordEnd;
    unsigned int *recordCursor;
    unsigned int *recordAreaEntry;
    unsigned int recordCount;
    unsigned int vertexIndex;
    unsigned int methodType;
    unsigned int particleIndex;
    unsigned int color;
    int mmxRefreshEnabled;
    int sceneResult;
    int drawResult;
    int clipResult;
    int result;

    result = 1;
    recordBuffer = (unsigned int *)renderContext->ownedMemory420;
    mmxRefreshEnabled = renderContext->mmxRefreshPending;
    EQ_CDECL(void, 0x10045DD9, (void))();
    sceneResult = EQ_CDECL(int, 0x1002F68D, (CameraParameters_guess *))(camera);
    if (sceneResult < 0)
    {
        result = -1;
        goto release_device;
    }

    renderContext->cameraViewportLeft = camera->cameraValue54;
    renderContext->cameraViewportTop = camera->cameraValue58;
    renderContext->cameraViewportRight = camera->cameraValue5c;
    renderContext->cameraViewportBottom = camera->cameraValue60;
    renderContext->unknown188 = camera->cameraValue40;
    renderContext->unknown18c = camera->cameraValue44;
    renderContext->unknown190 = camera->cameraValue48;
    renderContext->minimumViewDepth = camera->depthNear;
    renderContext->unknown1a4 = camera->depthNearOffset;
    renderContext->unknown1a8 = camera->depthFar;
    renderContext->unknown440 = 0;
    renderContext->worldToEyeRotation00 = camera->cameraValue6c;
    renderContext->worldToEyeRotation01 = camera->cameraValue70;
    renderContext->viewDepthFromX = camera->cameraValue74;
    renderContext->worldToEyeRotation10 = camera->cameraValue78;
    renderContext->worldToEyeRotation11 = camera->cameraValue7c;
    renderContext->viewDepthFromY = camera->cameraValue80;
    renderContext->worldToEyeRotation20 = camera->cameraValue84;
    renderContext->worldToEyeRotation21 = camera->cameraValue88;
    renderContext->viewDepthFromZ = camera->cameraValue8c;
    renderContext->screenDepthFromX = camera->cameraValue90;
    renderContext->screenDepthFromY = camera->cameraValue94;
    renderContext->screenDepthFromZ = camera->cameraValue98;
    renderContext->scaledScreenDepthFromX = camera->cameraValue9c;
    renderContext->scaledScreenDepthFromY = camera->cameraValuea0;
    renderContext->scaledScreenDepthFromZ = camera->cameraValuea4;
    renderContext->screenXFromX = camera->cameraValuea8;
    renderContext->screenXFromY = camera->cameraValueac;
    renderContext->screenXFromZ = camera->cameraValueb0;
    renderContext->screenYFromX = camera->cameraValueb4;
    renderContext->screenYFromY = camera->cameraValueb8;
    renderContext->screenYFromZ = camera->cameraValuebc;
    renderContext->worldToEyeCombinedScale = camera->cameraValuec0;
    renderContext->worldToEyeBaseScale = camera->cameraValuec4;
    renderContext->worldToEyeScaleProduct = camera->cameraValuec8;
    renderContext->inverseWorldToEyeBaseScaleSquared = camera->cameraValuecc;
    renderContext->inverseWorldToEyeCombinedScaleSquared = camera->cameraValueD0;
    renderContext->unknown190 = camera->cameraValue48;
    renderContext->unknown194 = camera->cameraValue4c;
    renderContext->screenOriginX = camera->cameraValue64;
    renderContext->screenOriginY = camera->cameraValue68;
    renderContext->textureDepthScale =
        (float)renderContext->unknown194 * renderContext->texture2Texture4Proportion *
        (float)renderContext->unknown190;
    renderContext->textureDepthOffset =
        renderContext->texture2SolidFillProportion *
        (camera->depthFar - camera->depthNear) + camera->depthNear;

    depthTable = (float *)((char *)T3D_WORLD_TO_EYE_TABLE + 0x6080);
    renderContext->unknown184 = scene->frameTime;
    renderContext->polygonCount = 0;
    renderContext->regionCount = 0;
    cameraOffsetX = -camera->eyeX;
    cameraOffsetY = -camera->eyeY;
    cameraOffsetZ = -camera->eyeZ;
    cameraRegion = scene->regionTable[camera->regionArrayIndex];
    renderContext->worldOriginX = cameraOffsetX;
    renderContext->worldOriginY = cameraOffsetY;
    renderContext->worldOriginZ = cameraOffsetZ;
    regionIndex = (unsigned int)camera->currentRegionIndex;
    if (camera->currentRegionIndex < 0)
    {
        renderContext->regionColor0 = 0.0f;
        if (mmxRefreshEnabled != 0)
        {
            renderContext->regionColor1 = 0.0f;
            renderContext->regionColor2 = 0.0f;
        }
        renderContext->activeRegionObject = 0;
    }
    else
    {
        sceneNode = scene->sceneNodes[regionIndex];
        zoneRegion = sceneNode->zoneRegion;
        renderContext->activeRegionObject = zoneRegion;
        if (zoneRegion == 0)
        {
            renderContext->regionColor0 = 0.0f;
            if (mmxRefreshEnabled != 0)
            {
                renderContext->regionColor1 = 0.0f;
                renderContext->regionColor2 = 0.0f;
            }
        }
        else
        {
            textureAnimation = zoneRegion->textureAnimation;
            textureFrames = textureAnimation->frameSet;
            if (textureFrames->frameCount < 2)
            {
                if (mmxRefreshEnabled == 0)
                {
                    renderContext->regionColor0 = textureFrames->values.singleValue;
                }
                else if (textureFrames->rgbValues == 0)
                {
                    renderContext->regionColor0 = textureFrames->values.singleValue;
                    renderContext->regionColor1 = textureFrames->values.singleValue;
                    renderContext->regionColor2 = textureFrames->values.singleValue;
                }
                else
                {
                    renderContext->regionColor0 = textureFrames->rgbValues[0];
                    renderContext->regionColor1 = textureFrames->rgbValues[1];
                    renderContext->regionColor2 = textureFrames->rgbValues[2];
                }
            }
            else
            {
                if ((textureAnimation->nextFrameTime < scene->frameTime) &&
                    ((textureAnimation->flags & 8) == 0))
                {
                    if (textureAnimation->loopMode == 0)
                    {
                        textureAnimation->frameIndex = textureAnimation->frameIndex + 1;
                        if (textureFrames->frameCount <= textureAnimation->frameIndex)
                        {
                            textureAnimation->frameIndex = 0;
                        }
                        textureAnimation->nextFrameTime =
                            textureAnimation->frameDuration + scene->frameTime;
                    }
                    else
                    {
                        frameDelta = scene->frameTime - textureAnimation->nextFrameTime;
                        if ((textureAnimation->frameDuration << 4) < frameDelta)
                        {
                            frameSteps = frameDelta / textureAnimation->frameDuration;
                            textureAnimation->frameIndex =
                                textureAnimation->frameIndex + frameSteps + 1;
                            textureAnimation->frameIndex =
                                textureAnimation->frameIndex % textureFrames->frameCount;
                            frameRemainder = frameDelta - frameSteps * textureAnimation->frameDuration;
                        }
                        else
                        {
                            frameRemainder = frameDelta;
                            if (textureAnimation->frameDuration < frameRemainder)
                            {
                                do
                                {
                                    textureAnimation->frameIndex = textureAnimation->frameIndex + 1;
                                    if (textureFrames->frameCount <= textureAnimation->frameIndex)
                                    {
                                        textureAnimation->frameIndex = 0;
                                    }
                                    frameRemainder = frameRemainder - textureAnimation->frameDuration;
                                } while (textureAnimation->frameDuration < frameRemainder);
                            }
                            textureAnimation->frameIndex = textureAnimation->frameIndex + 1;
                            if (textureFrames->frameCount <= textureAnimation->frameIndex)
                            {
                                textureAnimation->frameIndex = 0;
                            }
                        }
                        textureAnimation->nextFrameTime =
                            (scene->frameTime - frameRemainder) + textureAnimation->frameDuration;
                    }
                }
                if (mmxRefreshEnabled == 0)
                {
                    renderContext->regionColor0 =
                        textureFrames->values.frameValues[textureAnimation->frameIndex];
                }
                else if (textureFrames->rgbValues == 0)
                {
                    renderContext->regionColor0 =
                        textureFrames->values.frameValues[textureAnimation->frameIndex];
                    renderContext->regionColor1 =
                        textureFrames->values.frameValues[textureAnimation->frameIndex];
                    renderContext->regionColor2 =
                        textureFrames->values.frameValues[textureAnimation->frameIndex];
                }
                else
                {
                    renderContext->regionColor0 =
                        textureFrames->rgbValues[textureAnimation->frameIndex * 3];
                    renderContext->regionColor1 =
                        textureFrames->rgbValues[textureAnimation->frameIndex * 3 + 1];
                    renderContext->regionColor2 =
                        textureFrames->rgbValues[textureAnimation->frameIndex * 3 + 2];
                }
            }
            textureScale = zoneRegion->textureRadius * zoneRegion->textureRadius *
                EQGFX_POINT_RADIUS_SCALE_GUESS;
            renderContext->regionColor0 = textureScale * renderContext->regionColor0;
            if (mmxRefreshEnabled != 0)
            {
                renderContext->regionColor1 = textureScale * renderContext->regionColor1;
                renderContext->regionColor2 = textureScale * renderContext->regionColor2;
            }
        }
    }

    if (cameraRegion->renderData->type == T3D_RENDER_REGION_TYPE)
    {
        viewScaleX = renderContext->viewDepthFromX;
        viewScaleY = renderContext->viewDepthFromY;
        viewScaleZ = renderContext->viewDepthFromZ;
        EQ_CDECL(void, 0x1002F9A0, (CameraParameters_guess *, float *))(
            camera, renderContext->cameraFrustum);
        if (EQGFX_ZERO_F_GUESS < camera->clippingEnabled)
        {
            EQ_CDECL(void, 0x1002FB46, (CameraParameters_guess *, float *))(camera, clipPlanes);
        }
        conversionValue = camera->unknown20;
        __asm {
            fld dword ptr [conversionValue]
        }
        tableIndex = EQ_CDECL(unsigned int, 0x1004EC36, (void))();
        textureScale = depthTable[tableIndex * 6 + 2];
        regionDistance = textureScale / camera->projectionScale;
        renderContext->clipDepth = textureScale;
        renderContext->clipDepthReciprocal = regionDistance;
        renderContext->clipDepthSquared = textureScale * textureScale +
            EQGFX_FLOAT_ONE;
        renderContext->clipDepthReciprocalSquared = regionDistance * regionDistance +
            EQGFX_FLOAT_ONE;
        if (renderContext->fogControl == 2)
        {
            renderContext->unknown3f8 = renderContext->noFogDistance;
        }
        else
        {
            renderContext->unknown3f8 = 0.0f;
        }
        if (g_render_mode_guess == 1)
        {
            if (surfaceSelection == 0)
            {
                color = g_fog_color_raw_guess;
                EQ_CDECL(void, 0x10045DBF, (void *, unsigned int))(
                    g_clear_helper_object_guess,
                    (((((color >> 8) & 0xff) | ((color & 0xff) << 8)) << 8) |
                     ((color >> 16) & 0xff)));
                sceneResult = EQ_CDECL(int, 0x10045DB0, (void *, int))(g_clear_helper_object_guess, 3);
                if (sceneResult == T3D_D3DERR_ZBUFFER_NOTPRESENT)
                {
                    EQ_CDECL(int, 0x10045DB0, (void *, int))(g_clear_helper_object_guess, 1);
                }
                EQ_CDECL(void, 0x10045DBF, (void *, unsigned int))(g_clear_helper_object_guess, 0);
            }
            else
            {
                EQ_CDECL(int, 0x10045DB0, (void *, int))(g_clear_helper_object_guess, 2);
            }
            device = (T3DDevice7_guess *)g_render_device_guess;
            __asm {
                mov eax, device
                mov ecx, dword ptr [eax]
                push eax
                call dword ptr [ecx + 14h]
                mov sceneResult, eax
            }
            if ((sceneResult != 0) && (sceneResult != T3D_D3DERR_SCENE_IN_SCENE))
            {
                result = -4000;
                goto release_device;
            }
        }
        else if (g_render_mode_guess == 2)
        {
            conversionValue = renderContext->cameraViewportTop;
            __asm {
                fld dword ptr [conversionValue]
            }
            width = EQ_CDECL(unsigned int, 0x1004EC36, (void))();
            conversionValue = renderContext->cameraViewportLeft;
            __asm {
                fld dword ptr [conversionValue]
            }
            height = EQ_CDECL(unsigned int, 0x1004EC36, (void))();
            ((void (__stdcall *)(int, int, int, int))T3D_GR_CLIP_WINDOW)(
                (int)height,
                (int)width,
                (int)height + renderContext->unknown190,
                (int)width + renderContext->unknown194);
            ((void (__stdcall *)(void *, int, unsigned int))g_gr_buffer_clear_guess)(
                T3D_GR_BUFFER_CLEAR_CONTEXT, 0, EQGFX_DEPTH_CLEAR_VALUE);
            ((void (__stdcall *)(int, int, int, int))T3D_GR_CLIP_WINDOW)(
                0, 0, EQGFX_DISPLAY_WIDTH, g_display_height_guess);
        }
        if (surfaceSelection != 0)
        {
            EQ_CDECL(void, 0x10045441, (Scene_guess *, RenderContext_guess *, int *))(
                scene, renderContext, surfaceSelection);
        }
        sceneResult = EQ_CDECL(int, 0x10020094, (Scene_guess *, unsigned int))(
            scene, camera->regionArrayIndex);
        if (sceneResult != 0)
        {
            result = -1;
            goto release_device;
        }
        if (EQGFX_BOUND_POINT_LIGHT_COUNT != 0)
        {
            boundIndex = 0;
            if (scene->boundLightCount != 0)
            {
                do
                {
                    lightObject = scene->boundLightObjects[boundIndex];
                    if ((scene->boundLightCount < lightObject) &&
                        (*(int *)(lightObject + 0x34) != 0))
                    {
                        EQ_CDECL(void, 0x10014995, (Scene_guess *, void *))(
                            scene, (void *)lightObject);
                    }
                    boundIndex = boundIndex + 1;
                } while (boundIndex < scene->boundLightCount);
            }
        }
        regionIndex = 0;
        if (scene->regionQueueCount != 0)
        {
            do
            {
                region = scene->regionTable[scene->regionQueue[regionIndex]];
                clipResult = (int)EQ_CDECL(void *, 0x10030DAA, (float *, void *, unsigned int))(
                    renderContext->cameraFrustum,
                    &region->geometryData[0],
                    region->vertexCount);
                if (clipResult != -1)
                {
                    if (camera->clippingEnabled <= EQGFX_ZERO_F_GUESS)
                    {
                        sceneResult = 0;
                    }
                    else
                    {
                        sceneResult = EQ_CDECL(int, 0x10030E0A, (float *, void *, unsigned int))(
                            clipPlanes,
                            &region->geometryData[0],
                            region->vertexCount);
                    }
                    renderContext->regionCount = renderContext->regionCount + 1;
                    renderContext->clipResult = (void *)clipResult;
                    renderContext->clipResultEnd = (void *)clipResult;
                    areaList = region->areaList;
                    if ((areaList->type == T3D_RENDER_AREA_TYPE) ||
                        (areaList->type == T3D_RENDER_AREA_ALT_TYPE))
                    {
                        renderContext->regionBoundsMinimum = areaList->areaMinimum;
                        renderContext->regionBoundsMaximum = areaList->areaMaximum;
                    }
                    else
                    {
                        renderContext->regionBoundsMinimum = 0;
                    }
                    renderData = region->renderData;
                    if (renderData->type == T3D_RENDER_REGION_TYPE)
                    {
                        textureAnimation = renderData->textureAnimation;
                        textureFrames = textureAnimation->frameSet;
                        if (textureFrames->frameCount < 2)
                        {
                            if (mmxRefreshEnabled == 0)
                            {
                                renderContext->regionColorValue0 = textureFrames->values.singleValue;
                            }
                            else if (textureFrames->rgbValues == 0)
                            {
                                renderContext->regionColorValue1 = textureFrames->values.singleValue;
                                renderContext->regionColorValue2 = textureFrames->values.singleValue;
                                renderContext->regionColorValue3 = textureFrames->values.singleValue;
                            }
                            else
                            {
                                renderContext->regionColorValue1 = textureFrames->rgbValues[0];
                                renderContext->regionColorValue2 = textureFrames->rgbValues[1];
                                renderContext->regionColorValue3 = textureFrames->rgbValues[2];
                            }
                        }
                        else
                        {
                            if ((textureAnimation->nextFrameTime < scene->frameTime) &&
                                ((textureAnimation->flags & 8) == 0))
                            {
                                if (textureAnimation->loopMode == 0)
                                {
                                    textureAnimation->frameIndex = textureAnimation->frameIndex + 1;
                                    if (textureFrames->frameCount <= textureAnimation->frameIndex)
                                    {
                                        textureAnimation->frameIndex = 0;
                                    }
                                    textureAnimation->nextFrameTime =
                                        textureAnimation->frameDuration + scene->frameTime;
                                }
                                else
                                {
                                    frameDelta = scene->frameTime - textureAnimation->nextFrameTime;
                                    if ((textureAnimation->frameDuration << 4) < frameDelta)
                                    {
                                        frameSteps = frameDelta / textureAnimation->frameDuration;
                                        textureAnimation->frameIndex =
                                            textureAnimation->frameIndex + frameSteps + 1;
                                        textureAnimation->frameIndex =
                                            textureAnimation->frameIndex % textureFrames->frameCount;
                                        frameRemainder = frameDelta -
                                            frameSteps * textureAnimation->frameDuration;
                                    }
                                    else
                                    {
                                        frameRemainder = frameDelta;
                                        if (textureAnimation->frameDuration < frameRemainder)
                                        {
                                            do
                                            {
                                                textureAnimation->frameIndex =
                                                    textureAnimation->frameIndex + 1;
                                                if (textureFrames->frameCount <= textureAnimation->frameIndex)
                                                {
                                                    textureAnimation->frameIndex = 0;
                                                }
                                                frameRemainder = frameRemainder -
                                                    textureAnimation->frameDuration;
                                            } while (textureAnimation->frameDuration < frameRemainder);
                                        }
                                        textureAnimation->frameIndex = textureAnimation->frameIndex + 1;
                                        if (textureFrames->frameCount <= textureAnimation->frameIndex)
                                        {
                                            textureAnimation->frameIndex = 0;
                                        }
                                    }
                                    textureAnimation->nextFrameTime =
                                        (scene->frameTime - frameRemainder) + textureAnimation->frameDuration;
                                }
                            }
                            if (mmxRefreshEnabled == 0)
                            {
                                renderContext->regionColorValue0 =
                                    textureFrames->values.frameValues[textureAnimation->frameIndex];
                            }
                            else if (textureFrames->rgbValues == 0)
                            {
                                renderContext->regionColorValue1 =
                                    textureFrames->values.frameValues[textureAnimation->frameIndex];
                                renderContext->regionColorValue2 =
                                    textureFrames->values.frameValues[textureAnimation->frameIndex];
                                renderContext->regionColorValue3 =
                                    textureFrames->values.frameValues[textureAnimation->frameIndex];
                            }
                            else
                            {
                                renderContext->regionColorValue1 = textureFrames->rgbValues[
                                    textureAnimation->frameIndex * 3];
                                renderContext->regionColorValue2 = textureFrames->rgbValues[
                                    textureAnimation->frameIndex * 3 + 1];
                                renderContext->regionColorValue3 = textureFrames->rgbValues[
                                    textureAnimation->frameIndex * 3 + 2];
                            }
                        }
                        renderMethod = renderData->renderMethod;
                        if (renderMethod != 0)
                        {
                            renderContext->activeRenderMethod = renderMethod;
                            if (renderMethod->type == T3D_RENDER_METHOD_TYPE)
                            {
                                if (renderMethod->stateData->state == 0)
                                {
                                    result = EQ_CDECL(int, 0x10037411,
                                        (Scene_guess *, RenderContext_guess *, SceneRegion_guess *))(
                                            scene, renderContext, region);
                                }
                                else
                                {
                                    result = EQ_CDECL(int, 0x1003728C, (void))();
                                }
                            }
                            else
                            {
                                result = EQ_CDECL(int, 0x100375C1,
                                    (Scene_guess *, RenderContext_guess *, SceneRegion_guess *))(
                                        scene, renderContext, region);
                            }
                            if (result < 0)
                            {
                                goto release_device;
                            }
                        }
                    }
                    else
                    {
                        renderContext->regionColorValue0 = 1.0f;
                        if (mmxRefreshEnabled != 0)
                        {
                            renderContext->regionColorValue1 = 1.0f;
                            renderContext->regionColorValue2 = 1.0f;
                            renderContext->regionColorValue3 = 1.0f;
                        }
                    }
                    areaList = region->areaList;
                    if ((areaList->type == T3D_RENDER_AREA_TYPE) ||
                        (areaList->type == T3D_RENDER_AREA_ALT_TYPE))
                    {
                        renderContext->regionBoundsMinimum = areaList->areaMinimum;
                        renderContext->regionBoundsMaximum = areaList->areaMaximum;
                        if (renderContext->unknown414 < (int)areaList->areaCount)
                        {
                            recordBuffer = (unsigned int *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(
                                areaList->areaCount * 0x14);
                            if (recordBuffer == 0)
                            {
                                result = -1;
                                goto release_device;
                            }
                            EQ_CDECL(void, 0x10050CBB, (void *))(renderContext->ownedMemory420);
                            renderContext->ownedMemory420 = recordBuffer;
                            renderContext->unknown414 = areaList->areaCount;
                        }
                        surfaceSelection = (int *)areaList->areaList;
                        areaEnd = (unsigned int *)surfaceSelection + areaList->areaCount;
                        recordCount = 0;
                        if ((unsigned int *)surfaceSelection < areaEnd)
                        {
                            recordCursor = recordBuffer + 3;
                            do
                            {
                                if (((*surfaceSelection != camera->currentRegionIndex) &&
                                    (sceneObject = scene->sceneNodes[*surfaceSelection],
                                     (sceneObject->flags & 0x40) == 0)) &&
                                    ((int)cameraRegion != -1 ||
                                     ((sceneObject->renderCategory < 0x11) &&
                                      (sceneObject->renderCategory == 4))))
                                {
                                    recordCursor[-3] = (unsigned int)surfaceSelection;
                                    recordCursor[-1] = (unsigned int)sceneObject;
                                    vertexIndex = EQ_CDECL(unsigned int, 0x10042969,
                                        (Scene_guess *, SceneObject_guess *, float *))(
                                            scene, sceneObject, &camera->eyeX);
                                    *recordCursor = vertexIndex;
                                    recordCount = recordCount + 1;
                                    recordCursor[1] = sceneObject->vertexIndexTable[vertexIndex];
                                    ((float *)recordCursor)[-2] =
                                        (cameraOffsetX + sceneObject->x) * viewScaleX +
                                        (cameraOffsetY + sceneObject->y) * viewScaleY +
                                        (cameraOffsetZ + sceneObject->z) * viewScaleZ;
                                    recordCursor = recordCursor + 5;
                                }
                                surfaceSelection = surfaceSelection + 1;
                            } while ((unsigned int *)surfaceSelection < areaEnd);
                        }
                        renderContext->skippedRegionCount = 0;
                        renderContext->activeRegionCount = recordCount;
                        recordEnd = (unsigned int *)renderContext->ownedMemory420 + recordCount * 5;
                        if (recordBuffer < recordEnd)
                        {
                            recordCursor = recordBuffer + 3;
                            do
                            {
                                renderArea = (RegionAreaList_guess *)recordCursor[1];
                                recordAreaEntry = (unsigned int *)recordCursor[-3];
                                renderContext->activeRenderMethod = renderArea;
                                methodType = renderArea->type;
                                if (methodType == 0)
                                {
                                    goto finish_region_record;
                                }
                                if (methodType == 8)
                                {
                                    result = EQ_CDECL(int, 0x1002F3F9,
                                        (Scene_guess *, RenderContext_guess *, unsigned int *))(
                                            scene, renderContext, recordAreaEntry);
                                }
                                else if (methodType == 10)
                                {
                                    result = EQ_CDECL(int, 0x1003461A,
                                        (Scene_guess *, RenderContext_guess *, unsigned int *))(
                                            scene, renderContext, recordAreaEntry);
                                }
                                else if (methodType == 12)
                                {
                                    goto finish_region_record;
                                }
                                else if (methodType == 20)
                                {
                                    renderContext->activeRenderMethodType = *recordCursor;
                                    result = EQ_CDECL(int, 0x10032462,
                                        (Scene_guess *, RenderContext_guess *, SceneNode_guess *))(
                                            scene, renderContext,
                                            scene->sceneNodes[*recordAreaEntry]);
                                }
                                else if (methodType == T3D_RENDER_METHOD_TYPE)
                                {
                                    renderContext->activeRenderMethodType = *recordCursor;
                                    if (((RegionRenderMethod_guess *)renderArea)->stateData->state == 0)
                                    {
                                        result = EQ_CDECL(int, 0x10035FE2,
                                            (Scene_guess *, RenderContext_guess *, unsigned int *))(
                                                scene, renderContext, recordAreaEntry);
                                    }
                                    else
                                    {
                                        result = EQ_CDECL(int, 0x10035B4E,
                                            (Scene_guess *, RenderContext_guess *, unsigned int *))(
                                                scene, renderContext, recordAreaEntry);
                                    }
                                }
                                else
                                {
                                    ((void (__cdecl *)(void))EQ_GLOBAL(void *, 0x100760BC))();
                                    goto finish_region_record;
                                }
                                if (result < 0)
                                {
                                    goto release_device;
                                }
finish_region_record:
                                renderContext->skippedRegionCount =
                                    renderContext->skippedRegionCount + 1;
                                recordCursor = recordCursor + 5;
                            } while (recordCursor - 3 < recordEnd);
                        }
                    }
                }
                regionIndex = regionIndex + 1;
            } while (regionIndex < scene->regionQueueCount);
        }
        particleIndex = 0;
        if (scene->particleCount != 0)
        {
            do
            {
                sceneObject = scene->particleObjects[particleIndex];
                if ((scene->particleCount < (unsigned int)sceneObject) &&
                    (sceneObject->type == T3D_PARTICLE_CLOUD_TYPE))
                {
                    EQ_CDECL(void, 0x10040693,
                        (Scene_guess *, RenderContext_guess *, SceneObject_guess **))(
                            scene, renderContext, &scene->particleObjects[particleIndex]);
                }
                particleIndex = particleIndex + 1;
            } while (particleIndex < scene->particleCount);
        }
        EQ_CDECL(void, 0x100460CB, (void))();
        EQ_CDECL(void, 0x10046165, (void))();
        EQ_CDECL(void, 0x1002EE82, (void))();
        if (g_render_mode_guess == 1)
        {
            device = (T3DDevice7_guess *)g_render_device_guess;
            __asm {
                mov eax, device
                mov ecx, dword ptr [eax]
                push eax
                call dword ptr [ecx + 18h]
            }
        }
        if (g_initialize_window_state_guess != 0)
        {
            EQ_CDECL(void, 0x10048FC5, (RenderContext_guess *))(renderContext);
        }
        EQ_CDECL(void, 0x10045E9C, (void))();
        return result;
    }
    goto release_device;

release_device:
    device = (T3DDevice7_guess *)g_render_device_guess;
    __asm {
        mov eax, device
        mov ecx, dword ptr [eax]
        push eax
        call dword ptr [ecx + 18h]
    }
    EQ_CDECL(void, 0x10045E9C, (void))();
    return result;
}
