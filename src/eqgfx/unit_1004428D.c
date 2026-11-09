// unit_1004428D.c - eqgfx_dx7.dll, the address run 0x1004428D..0x10045E32 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

/* One sky layer, reached through the sky's +0x18 pointer: the sample count at +0, the scale index at +0x12, the sample
 * block at +0x14 (a 4-byte header, then the samples 0x14 bytes apart) and the float bias at +0x34. */
typedef struct SkyLayer_guess
{
    unsigned short sampleCount;         // +0x00, the loop bound
    unsigned char unknown_02[0x06];
    unsigned short vertexCount;         // +0x08, transform mask length
    unsigned char unknown_0a[0x08];
    unsigned short scaleIndex;          // +0x12, index into g_layer_scale_table
    unsigned char *samples;             // +0x14, header then SkyLayerSample_guess entries
    unsigned char unknown_18[0x1c];
    float bias;                         // +0x34
    unsigned char unknown_38[0x10];
    void *paintData;                    // +0x48
} SkyLayer_guess;

/* One 0x14-byte sample of a sky layer: the signed value at +0. */
typedef struct SkyLayerSample_guess
{
    short value;                        // +0
    unsigned char unknown_02[0x12];
} SkyLayerSample_guess;

typedef struct SkyDrawState_guess
{
    unsigned char unknown_00[0x0f];
    unsigned char flags;                // +0x0f
    float x;                            // +0x10
    float y;                            // +0x14
    float z;                            // +0x18
    unsigned char unknown_1c[0x18];
    float distance;                     // +0x34
} SkyDrawState_guess;

/* The sky object as this unit reads it: the layer pointer at +0x18, the color row and its weight at +0x28 and +0x2c,
 * the dual-layer dword at +0x7c, the mode dword at +0x80 and the color table pointer at +0x45c are known; the rest is
 * unmodelled. */
typedef struct T3DSky_guess
{
    unsigned char unknown_00[0x18];
    SkyLayer_guess *layer;              // +0x18
    unsigned char unknown_1c[0x0c];
    int colorRow;                       // +0x28, 0..59
    float colorRowWeight;               // +0x2c, weight of colorRow (colorRow + 1 gets 1 - this)
    unsigned char unknown_30[0x4c];
    int dualLayer;                      // +0x7c
    unsigned int skyMode;               // +0x80
    struct SkyDrawLayer_guess *layerMode0; // +0x84
    SkyDrawState_guess drawStateModes0To4; // +0x88
    unsigned char unknown_0c0[0xdc];
    struct SkyAnimationTable_guess *animationModes0To1; // +0x19c
    struct SkyDrawLayer_guess *secondLayerMode0; // +0x1a0
    struct SkyDrawLayer_guess *layerMode1; // +0x1a4
    struct SkyDrawLayer_guess *secondLayerMode1; // +0x1a8
    struct SkyDrawLayer_guess *layerMode2; // +0x1ac
    SkyDrawState_guess drawStateMode2; // +0x1b0
    unsigned char unknown_1e8[0xdc];
    struct SkyAnimationTable_guess *animationMode2; // +0x2c4
    struct SkyDrawLayer_guess *secondLayerMode2; // +0x2c8
    struct SkyDrawLayer_guess *layerMode3; // +0x2cc
    struct SkyDrawLayer_guess *secondLayerMode3; // +0x2d0
    struct SkyDrawLayer_guess *layerMode4; // +0x2d4
    struct SkyDrawLayer_guess *secondLayerMode4; // +0x2d8
    struct SkyDrawLayer_guess *layerMode5; // +0x2dc
    SkyDrawState_guess drawStateModes5To8; // +0x2e0
    unsigned char unknown_318[0xdc];
    struct SkyAnimationTable_guess *animationModes5To8; // +0x3f4
    struct SkyDrawLayer_guess *secondLayerMode5; // +0x3f8
    struct SkyDrawLayer_guess *layerMode6; // +0x3fc
    struct SkyDrawLayer_guess *secondLayerMode6; // +0x400
    struct SkyDrawLayer_guess *layerMode7; // +0x404
    struct SkyDrawLayer_guess *secondLayerMode7; // +0x408
    struct SkyDrawLayer_guess *layerMode8; // +0x40c
    struct SkyDrawLayer_guess *secondLayerMode8; // +0x410
    void *colorInputs[18];              // +0x414, two color inputs per mode
    unsigned char *vertexColors;        // +0x45c, 60 rows of 18 four-byte colors (bytes 2, 1, 0 are read)
} T3DSky_guess;

typedef struct SkyDrawLayer_guess
{
    unsigned char unknown_00[0x0c];
    unsigned int flags;                 // +0x0c
    unsigned char unknown_10[0x04];
    float directionX;                    // +0x14
    float directionY;                    // +0x18
    float directionZ;                    // +0x1c
    int renderId;                        // +0x20
    unsigned char unknown_24[0x10];
    struct SkyPaintData_guess *paintData; // +0x34
} SkyDrawLayer_guess;

typedef struct SkyPaintData_guess
{
    unsigned char unknown_00[0x18];
    SkyLayer_guess *layer;              // +0x18
} SkyPaintData_guess;

typedef struct SkyAnimationTable_guess
{
    float timeScale;                     // +0x00
    unsigned char unknown_04[0x1c];
    int trackCount;                      // +0x20
    unsigned char *tracks;               // +0x24
} SkyAnimationTable_guess;

typedef struct SkyAnimationTrack_guess
{
    unsigned char unknown_00[0x14];
    void *track;                         // +0x14
    unsigned char unknown_18[0xf0];
    int frameRate;                       // +0x108
} SkyAnimationTrack_guess;

typedef struct T3DAnimationContext_guess
{
    unsigned char unknown_00[0x08];
    int frameTime;                       // +0x08
} T3DAnimationContext_guess;

typedef struct T3DRenderContext_guess
{
    unsigned char unknown_000[0x1ac];
    float skyOriginX;
    float skyOriginY;
    float skyOriginZ;
    unsigned char unknown_1b8[0x74];
    float matrix_22c;
    unsigned char unknown_230[0x08];
    float matrix_238;
    unsigned char unknown_23c[0x08];
    float matrix_244;
    float matrix_248;
    float matrix_24c;
    float matrix_250;
    float matrix_254;
    float matrix_258;
    float matrix_25c;
    unsigned char unknown_260[0x48];
    int skyFlags;
    unsigned char unknown_2ac[0x12c];
    int activeSkyAnimation;
    unsigned char unknown_3dc[0x20];
    int cachedDepthMode;
    int cachedDepthWriteMode;
    unsigned char unknown_404[0x20c];
    void *transformedVertices;
    void *interpolatedColors;
    void *sampleMask;
    void *vertexMask;
} T3DRenderContext_guess;

typedef void (__cdecl *SkyPaintCallback_guess)(void *, unsigned int, void *);

#define EQGFX_SKY_DISTANCE_GUESS       5000.0f
#define EQGFX_SKY_DISTANCE_GLOBAL_GUESS EQ_GLOBAL(float, 0x10076368)
#define EQGFX_SKY_PAINT_CALLBACK_GUESS  EQ_GLOBAL(SkyPaintCallback_guess, 0x100FC76C)
#define EQGFX_RENDER_VISIBILITY_MODE_GUESS  EQ_GLOBAL(int, 0x100A16B8)
#define EQGFX_SKY_TRACK_STRIDE_GUESS    0x148
#define EQGFX_SKY_FTOL_ADDRESS_GUESS    0x1004EC36

/* The method the object keeps at vtable offset 0x54: __stdcall with the object first on the stack (the
 * original cleans neither argument), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextClearFn)(void *object, int value);

/* The method the object keeps at vtable offset 0x08: __stdcall with the object first on the stack (the
 * original cleans nothing after the call), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextReleaseFn)(void *object);

/* The method the object keeps at vtable offset 0x0c: __stdcall with the object first on the stack (the
 * original cleans nothing after the call), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextGetDDFn)(void *object);

/* The method the object keeps at vtable offset 0x10: __stdcall with the object first on the stack (the
 * original cleans nothing after the call), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextGetD3DFn)(void *object);

/* The method the object keeps at vtable offset 0x14: __stdcall with the object first on the stack (the
 * original cleans nothing after the call), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextGetD3DDeviceFn)(void *object);

/* The method the object keeps at vtable offset 0x18: __stdcall with the object first on the stack (the
 * original cleans nothing after the call), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextGetPrimaryFn)(void *object);

/* The method the object keeps at vtable offset 0x20: __stdcall with the object first on the stack (the
 * original cleans neither argument), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextGetBackBufferFn)(void *object, int value);

/* The method the object keeps at vtable offset 0x44: __stdcall with the object first on the stack (the
 * original cleans nothing after the call), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextRestoreSurfacesFn)(void *object);

/* The method the object keeps at vtable offset 0x4c: __stdcall with the object first on the stack (the
 * original cleans neither argument), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextUpdateFrameFn)(void *object, int value);

/* The method the object keeps at vtable offset 0x58: __stdcall with the object first on the stack (the
 * original cleans neither argument), the result in EAX. The method's own name is unknown. */
typedef int (__stdcall *D3DXContextSetClearColorFn)(void *object, int value);

/* eqgfx_dx7.dll's own globals this function uses. */
#define g_render_mode_guess   EQ_GLOBAL(int, 0x100FB8A8)    // the render mode
#define g_fog_enabled_guess   EQ_GLOBAL(int, 0x100FBA1C)    // the enable flag the last t3dSetFog cached
#define g_saved_fog_enabled_guess EQ_GLOBAL(int, 0x100F3DAC) // the fog enable the dropping pass saved
#define g_render_device_guess EQ_GLOBAL(void *, 0x100FBA6C) // the render-state device (vtable at +0x00)
#define g_byteFloatTable_guess    EQ_ARRAY(float, 0x100FA068)    // 256 floats, one per color byte value
#define EQGFX_MESH_SCALE_F_GUESS   EQ_ARRAY(float, 0x100FB868)    // float scale per layer scale index

/* The Glide entry points it calls in render mode 2 (filled from GetProcAddress at start-up; the loader is
 * 0x1004BE4E): __stdcall with one int argument, the @N suffix of each name is its argument bytes. */
typedef void (__stdcall *GrDepthMaskFn_guess)(int mask);
typedef void (__stdcall *GrDepthBufferModeFn_guess)(int mode);
typedef void (__stdcall *GrFogModeFn_guess)(int mode);
#define g_gr_depth_mask_fn_guess        EQ_GLOBAL(GrDepthMaskFn_guess, 0x100F3EC0)       // "_grDepthMask@4"
#define g_gr_depth_buffer_mode_fn_guess EQ_GLOBAL(GrDepthBufferModeFn_guess, 0x100F3EBC) // "_grDepthBufferMode@4"
#define g_gr_fog_mode_fn_guess          EQ_GLOBAL(GrFogModeFn_guess, 0x100F3EFC)         // "_grFogMode@4"

/* Vtable slot 0x50 of the render device: __stdcall (the original cleans none of the 3 arguments), stack
 * `device` first, then a D3D render-state id and its value. */
typedef void (__stdcall *D3DDev7SetRenderStateFn)(void *device, int state_id, int state);

/* The D3D render-state ids render mode 1 sends through that slot (D3DRS_ZENABLE, D3DRS_ZWRITEENABLE). */
#define EQGFX_Z_ENABLE_STATE_GUESS       0x7
#define EQGFX_Z_WRITE_ENABLE_STATE_GUESS 0xe

/* The sky color table's shape: rows 0..59, 18 columns (0..17) per row. */
#define SKY_LAST_ROW     59
#define SKY_LAST_COLUMN  17
#define SKY_COLUMNS      18

typedef struct SkyMaterialSamples_guess
{
    unsigned char unknown_00[0x14];
    unsigned int sampleCount;           // +0x14
    unsigned int *materialIds;          // +0x18
} SkyMaterialSamples_guess;

typedef struct SkyAnimatedDefinition_guess
{
    unsigned int vertexColorCount;      // +0x00
    unsigned char unknown_04[0x70];
    SkyMaterialSamples_guess *materials; // +0x74
} SkyAnimatedDefinition_guess;

typedef struct SkyTextureDefinition_guess
{
    unsigned short vertexColorCount;    // +0x00
    unsigned char unknown_02[0x46];
    SkyMaterialSamples_guess *materials; // +0x48
} SkyTextureDefinition_guess;

typedef struct SkySpriteDefinition_guess
{
    unsigned char unknown_00[0x14];
    SkyAnimatedDefinition_guess *animatedDefinition; // +0x14
    SkyTextureDefinition_guess *textureDefinition;    // +0x18
} SkySpriteDefinition_guess;

typedef struct SkyAnimationPartObject_guess
{
    int type;                           // +0x00
    unsigned char unknown_04[0x2c];
    unsigned char whiteComponents[3];    // +0x30
} SkyAnimationPartObject_guess;

typedef struct SkyAnimationElement_guess
{
    unsigned char unknown_00[0x08];
    SkyAnimationPartObject_guess *object; // +0x08
    unsigned char unknown_0c[0x13c];
} SkyAnimationElement_guess;

typedef struct SkyHierarchicalSprite_guess
{
    unsigned char unknown_00[0x0c];
    unsigned int flags;                 // +0x0c
    unsigned char unknown_10[0x10];
    int elementCount;                   // +0x20
    SkyAnimationElement_guess *elements; // +0x24
} SkyHierarchicalSprite_guess;

#define EQGFX_SKY_UV_RATE_A_GUESS       EQ_GLOBAL(float, 0x10076364)
#define EQGFX_SKY_UV_RATE_B_GUESS       EQ_GLOBAL(float, 0x10076360)
#define EQGFX_SKY_UV_RATE_C_GUESS       EQ_GLOBAL(float, 0x1007635C)
#define EQGFX_SKY_LAYER_FLAG_GUESS      0x10
#define EQGFX_SKY_PART_TYPE_GUESS       0x48
#define EQGFX_SKY_HIERARCHICAL_FLAG_GUESS 0x20000000
#define EQGFX_SKY_INIT_ERROR_GUESS      0xFFFF3D3E

#define SKY_INIT_DM_LAYER(definitionAddress, missingMessage, layerMember, createMessage, inputIndex, uvRate, allocationMessage, failureLabel) \
    do { \
        definition = (SkySpriteDefinition_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))(dictionary, (const char *)(definitionAddress)); \
        if (definition == 0) { failureMessage = (const char *)(missingMessage); goto failureLabel; } \
        drawLayer = (SkyDrawLayer_guess *)EQ_CDECL(void *, 0x1001034C, (void *, void *, int))(dictionary, definition, 0); \
        sky->layerMember = drawLayer; \
        if (drawLayer == 0) { failureMessage = (const char *)(createMessage); goto failureLabel; } \
        if (definition->animatedDefinition == 0) materials = definition->textureDefinition->materials; \
        else materials = definition->animatedDefinition->materials; \
        skyAddress = 0; \
        if (materials->sampleCount > 0) { \
            do { \
                EQ_CDECL(void, 0x10016EDA, (void *, unsigned int, float, float))(dictionary, materials->materialIds[skyAddress], uvRate, 0.0f); \
                skyAddress = skyAddress + 1; \
            } while (skyAddress < materials->sampleCount); \
        } \
        if (definition->animatedDefinition == 0) colorCount = (unsigned int)definition->textureDefinition->vertexColorCount; \
        else colorCount = definition->animatedDefinition->vertexColorCount; \
        colorBuffer = EQ_CDECL(void *, 0x10050C48, (unsigned int))(colorCount << 2); \
        sky->colorInputs[inputIndex] = colorBuffer; \
        if (colorBuffer == 0) { failureMessage = (const char *)(allocationMessage); goto failureLabel; } \
        EQ_CDECL(void, 0x10045375, (void *, void *))(definition, colorBuffer); \
        sky->layerMember->flags |= EQGFX_SKY_LAYER_FLAG_GUESS; \
    } while (0)

// FUNCTION: EQGFX 0x1004428D
// Builds the sky for all nine modes from the sprite dictionary; arguments are the dictionary, sky address, color table.
// SKY_INIT_DM_LAYER looks up each layer's sprite definition (0x1000e7fe), creates it, sets the UV scroll rate on its
// materials, and allocates a color buffer into sky->colorInputs[0..17].
// Hierarchical sprites for modes 0-1, 2 and 5-8 are created by 0x10012a60 and their type-0x48 parts set to white.
// Success stores colorTable in sky->vertexColors with skyMode 0 and dualLayer 1. A failure before layer 5 logs and
// returns 0xffff3d3e; a failure from layer 5 on is logged and still returns 0.
int __cdecl t3dInitSky(void *dictionary, unsigned int skyAddress, void *colorTable)
{
    T3DSky_guess *sky;
    SkySpriteDefinition_guess *definition;
    SkyMaterialSamples_guess *materials;
    SkyDrawLayer_guess *drawLayer;
    SkyHierarchicalSprite_guess *hierarchicalSprite;
    SkyAnimationElement_guess *element;
    SkyAnimationElement_guess *elementEnd;
    SkyAnimationPartObject_guess *partObject;
    unsigned int colorCount;
    void *colorBuffer;
    const char *failureMessage;

    SKY_INIT_DM_LAYER(0x100A25AC, 0x100A2580, layerMode0, 0x100A2544, 0,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A250C, sky_init_failure);

    definition = (SkySpriteDefinition_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))(dictionary, (const char *)0x100A24FC);
    if (definition == 0)
    {
        failureMessage = (const char *)0x100A24C4;
        goto sky_init_failure;
    }
    *(unsigned int *)&sky->drawStateModes0To4.unknown_00[0x0c] = EQGFX_SKY_HIERARCHICAL_FLAG_GUESS;
    sky->drawStateModes0To4.x = 0.0f;
    sky->drawStateModes0To4.y = 0.0f;
    sky->drawStateModes0To4.z = 0.0f;
    ((float *)&sky->drawStateModes0To4.unknown_1c)[0] = 0.0f;
    ((float *)&sky->drawStateModes0To4.unknown_1c)[1] = 0.0f;
    ((float *)&sky->drawStateModes0To4.unknown_1c)[2] = 0.0f;
    sky->drawStateModes0To4.distance = 1.0f;
    hierarchicalSprite = (SkyHierarchicalSprite_guess *)EQ_CDECL(void *, 0x10012A60, (void *, void *, int))(dictionary, definition, 0);
    sky->animationModes0To1 = (SkyAnimationTable_guess *)hierarchicalSprite;
    if (hierarchicalSprite == 0)
    {
        failureMessage = (const char *)0x100A2488;
        goto sky_init_failure;
    }
    element = hierarchicalSprite->elements;
    elementEnd = (SkyAnimationElement_guess *)((unsigned char *)element + hierarchicalSprite->elementCount * EQGFX_SKY_TRACK_STRIDE_GUESS);
    for (; element < elementEnd; element = (SkyAnimationElement_guess *)((unsigned char *)element + EQGFX_SKY_TRACK_STRIDE_GUESS))
    {
        EQ_CDECL(void, 0x10013A76, (void *, int))(element, 1);
        partObject = element->object;
        if (partObject != 0 && partObject->type == EQGFX_SKY_PART_TYPE_GUESS)
        {
            partObject->whiteComponents[0] |= 0xFF;
            partObject->whiteComponents[1] |= 0xFF;
            partObject->whiteComponents[2] |= 0xFF;
        }
    }
    hierarchicalSprite->flags |= EQGFX_SKY_LAYER_FLAG_GUESS;

    SKY_INIT_DM_LAYER(0x100A2474, 0x100A2448, secondLayerMode0, 0x100A240C, 1,
                      EQGFX_SKY_UV_RATE_B_GUESS, 0x100A23D4, sky_init_failure);
    SKY_INIT_DM_LAYER(0x100A23C0, 0x100A2394, layerMode1, 0x100A2358, 2,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A2320, sky_init_failure);
    SKY_INIT_DM_LAYER(0x100A230C, 0x100A22E0, secondLayerMode1, 0x100A22A4, 3,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A226C, sky_init_failure);
    SKY_INIT_DM_LAYER(0x100A2258, 0x100A222C, layerMode2, 0x100A21F0, 4,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A21B8, sky_init_failure);

    definition = (SkySpriteDefinition_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))(dictionary, (const char *)0x100A21A8);
    if (definition == 0)
    {
        failureMessage = (const char *)0x100A24C4;
        goto sky_init_failure;
    }
    else
    {
        *(unsigned int *)&sky->drawStateMode2.unknown_00[0x0c] = EQGFX_SKY_HIERARCHICAL_FLAG_GUESS;
        sky->drawStateMode2.x = 0.0f;
        sky->drawStateMode2.y = 0.0f;
        sky->drawStateMode2.z = 0.0f;
        ((float *)&sky->drawStateMode2.unknown_1c)[0] = 0.0f;
        ((float *)&sky->drawStateMode2.unknown_1c)[1] = 0.0f;
        ((float *)&sky->drawStateMode2.unknown_1c)[2] = 0.0f;
        sky->drawStateMode2.distance = 1.0f;
        hierarchicalSprite = (SkyHierarchicalSprite_guess *)EQ_CDECL(void *, 0x10012A60, (void *, void *, int))(dictionary, definition, 0);
        sky->animationMode2 = (SkyAnimationTable_guess *)hierarchicalSprite;
        if (hierarchicalSprite == 0)
        {
            failureMessage = (const char *)0x100A216C;
            goto sky_init_failure;
        }
        element = hierarchicalSprite->elements;
        elementEnd = (SkyAnimationElement_guess *)((unsigned char *)element + hierarchicalSprite->elementCount * EQGFX_SKY_TRACK_STRIDE_GUESS);
        for (; element < elementEnd; element = (SkyAnimationElement_guess *)((unsigned char *)element + EQGFX_SKY_TRACK_STRIDE_GUESS))
        {
            EQ_CDECL(void, 0x10013A76, (void *, int))(element, 1);
            partObject = element->object;
            if (partObject != 0 && partObject->type == EQGFX_SKY_PART_TYPE_GUESS)
            {
                partObject->whiteComponents[0] |= 0xFF;
                partObject->whiteComponents[1] |= 0xFF;
                partObject->whiteComponents[2] |= 0xFF;
            }
        }
        hierarchicalSprite->flags |= EQGFX_SKY_LAYER_FLAG_GUESS;
    }

    SKY_INIT_DM_LAYER(0x100A2158, 0x100A212C, secondLayerMode2, 0x100A20F0, 5,
                      EQGFX_SKY_UV_RATE_B_GUESS, 0x100A20B8, sky_init_failure);
    SKY_INIT_DM_LAYER(0x100A20A4, 0x100A2078, layerMode3, 0x100A203C, 6,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A2004, sky_init_failure);
    SKY_INIT_DM_LAYER(0x100A1FF0, 0x100A1FC4, secondLayerMode3, 0x100A1F88, 7,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A1F50, sky_init_failure);
    SKY_INIT_DM_LAYER(0x100A1F3C, 0x100A1F10, layerMode4, 0x100A1ED4, 8,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A1E9C, sky_init_failure);
    SKY_INIT_DM_LAYER(0x100A1E88, 0x100A1E5C, secondLayerMode4, 0x100A1E20, 9,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A1DE8, sky_init_failure);

    SKY_INIT_DM_LAYER(0x100A1DD4, 0x100A1DA8, layerMode5, 0x100A1D6C, 10,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A1D34, sky_init_logged_success);

    definition = (SkySpriteDefinition_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))(dictionary, (const char *)0x100A1D24);
    if (definition == 0)
    {
        failureMessage = (const char *)0x100A1CEC;
        goto sky_init_logged_success;
    }
    else
    {
        ((float *)&sky->drawStateModes5To8.unknown_1c)[0] = 0.0f;
        *(unsigned int *)&sky->drawStateModes5To8.unknown_00[0x0c] = EQGFX_SKY_HIERARCHICAL_FLAG_GUESS;
        sky->drawStateModes5To8.x = 0.0f;
        sky->drawStateModes5To8.y = 0.0f;
        sky->drawStateModes5To8.z = 0.0f;
        ((float *)&sky->drawStateModes5To8.unknown_1c)[1] = 0.0f;
        ((float *)&sky->drawStateModes5To8.unknown_1c)[2] = 0.0f;
        sky->drawStateModes5To8.distance = 1.0f;
        hierarchicalSprite = (SkyHierarchicalSprite_guess *)EQ_CDECL(void *, 0x10012A60, (void *, void *, int))(dictionary, definition, 0);
        sky->animationModes5To8 = (SkyAnimationTable_guess *)hierarchicalSprite;
        if (hierarchicalSprite == 0)
        {
            failureMessage = (const char *)0x100A1CB0;
            goto sky_init_logged_success;
        }
        element = hierarchicalSprite->elements;
        elementEnd = (SkyAnimationElement_guess *)((unsigned char *)element + hierarchicalSprite->elementCount * EQGFX_SKY_TRACK_STRIDE_GUESS);
        for (; element < elementEnd; element = (SkyAnimationElement_guess *)((unsigned char *)element + EQGFX_SKY_TRACK_STRIDE_GUESS))
        {
            EQ_CDECL(void, 0x10013A76, (void *, int))(element, 1);
            partObject = element->object;
            if (partObject != 0 && partObject->type == EQGFX_SKY_PART_TYPE_GUESS)
            {
                partObject->whiteComponents[0] |= 0xFF;
                partObject->whiteComponents[1] |= 0xFF;
                partObject->whiteComponents[2] |= 0xFF;
            }
        }
        hierarchicalSprite->flags |= EQGFX_SKY_LAYER_FLAG_GUESS;
    }

    SKY_INIT_DM_LAYER(0x100A1C9C, 0x100A1C70, secondLayerMode5, 0x100A1C34, 11,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A1BFC, sky_init_logged_success);
    SKY_INIT_DM_LAYER(0x100A1BE8, 0x100A1BBC, layerMode6, 0x100A1B80, 12,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A1B48, sky_init_logged_success);
    SKY_INIT_DM_LAYER(0x100A1B34, 0x100A1B08, secondLayerMode6, 0x100A1ACC, 13,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A1A94, sky_init_logged_success);
    SKY_INIT_DM_LAYER(0x100A1A80, 0x100A1A54, layerMode7, 0x100A1A18, 14,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A19E0, sky_init_logged_success);
    SKY_INIT_DM_LAYER(0x100A19CC, 0x100A19A0, secondLayerMode7, 0x100A1964, 15,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A192C, sky_init_logged_success);
    SKY_INIT_DM_LAYER(0x100A1918, 0x100A18EC, layerMode8, 0x100A18B0, 16,
                      EQGFX_SKY_UV_RATE_A_GUESS, 0x100A1878, sky_init_logged_success);
    SKY_INIT_DM_LAYER(0x100A1864, 0x100A1838, secondLayerMode8, 0x100A17FC, 17,
                      EQGFX_SKY_UV_RATE_C_GUESS, 0x100A17C4, sky_init_logged_success);

    sky->vertexColors = (unsigned char *)colorTable;
    sky->skyMode = 0;
    sky->dualLayer = 1;
    return 0;

sky_init_failure:
    EQ_CDECL(void, 0x100509EE, (const char *))(failureMessage);
    return EQGFX_SKY_INIT_ERROR_GUESS;

sky_init_logged_success:
    EQ_CDECL(void, 0x100509EE, (const char *))(failureMessage);
    sky->vertexColors = (unsigned char *)colorTable;
    sky->skyMode = 0;
    sky->dualLayer = 1;
    return 0;
}

#undef SKY_INIT_DM_LAYER

// FUNCTION: EQGFX 0x10045375
// Prepares per-sample sky shading levels for drawing a sky layer.
// __cdecl (callee pops 0 bytes; ECX is not an input). For each sample of the sky's layer, passes (sample * scale + bias) * 128
// to t3dFloatRoundToInt_guess, clamps its int result to 0..16 and stores 16 minus it in levels[].
void __cdecl t3dComputeSkyLayerLevels_guess(T3DSky_guess *sky, int *levels)
{
    SkyLayer_guess *layer;
    SkyLayerSample_guess *sample;
    int *out;
    unsigned int index;
    float scale;
    float bias;
    int level;

    layer = sky->layer;
    scale = EQGFX_MESH_SCALE_F_GUESS[layer->scaleIndex];
    if (layer != 0)
    {
        bias = layer->bias;
        index = 0;
        if (layer->sampleCount != 0)
        {
            sample = (SkyLayerSample_guess *)(layer->samples + 4);
            out = levels;
            do
            {
                EQ_CDECL(void, 0x1004EC8D, (float, int *))(((float)sample->value * scale + bias) * 128.0f, &level);
                if (level < 0)
                    level = 0;
                else if (level >= 16)
                    level = 16;
                sample++;
                level = 16 - level;
                *out = level;
                out++;
                index++;
            } while (index < sky->layer->sampleCount);
        }
    }
}

// FUNCTION: EQGFX 0x10045409
// Selects the sky's rendering mode.
// __cdecl (callee pops 0 bytes; ECX is not an input). Stores the mode at +0x80 of the sky object when it is below 9,
// and returns 0; otherwise clears +0x80 and returns 0xffff3d3e.
unsigned int __cdecl t3dSetSky(T3DSky_guess *sky, unsigned int mode)
{
    if (mode < 9)
    {
        sky->skyMode = mode;
        return 0;
    }
    sky->skyMode = 0;
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x10045430
// Controls whether sky drawing uses a second layer.
// __cdecl (callee pops 0 bytes; ECX is not an input). Stores the second argument at +0x7c of the sky object and
// returns the constant 0xffff3d3e.
unsigned int __cdecl t3dSetSkyDualLayer(T3DSky_guess *sky, int dualLayer)
{
    sky->dualLayer = dualLayer;
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x10045441
// Renders the selected sky mode's colored layers and animation, with an optional second layer.
int __cdecl t3dPaintSky(T3DAnimationContext_guess *animationContext, T3DRenderContext_guess *renderContext, T3DSky_guess *sky)
{
    SkyDrawLayer_guess *primaryLayer;
    SkyDrawLayer_guess *secondaryLayer;
    SkyDrawState_guess *drawState;
    SkyAnimationTable_guess *animationTable;
    SkyPaintData_guess *paintData;
    SkyLayer_guess *layerData;
    unsigned char *trackPointer;
    unsigned int trackEnd;
    unsigned int sampleCount;
    unsigned int vertexCount;
    unsigned int mode;
    void *colorInput;
    void *secondColorInput;
    float distanceFactor;
    float skyTransform[12];
    float transformedDirectionX;
    float transformedDirectionY;
    float transformedDirectionZ;
    int frameCount;
    int frameProduct;
    int frameOffset;

    primaryLayer = 0;
    secondaryLayer = 0;
    drawState = 0;
    animationTable = 0;
    colorInput = 0;
    secondColorInput = 0;
    distanceFactor = EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
    renderContext->skyFlags = 0;
    g_saved_fog_enabled_guess = g_fog_enabled_guess;
    mode = sky->skyMode;

    skyTransform[0] = renderContext->matrix_248 * distanceFactor;
    skyTransform[1] = renderContext->matrix_254 * distanceFactor;
    skyTransform[2] = renderContext->matrix_22c * distanceFactor;
    skyTransform[3] = renderContext->matrix_24c * distanceFactor;
    skyTransform[4] = renderContext->matrix_258 * distanceFactor;
    skyTransform[5] = renderContext->matrix_238 * distanceFactor;
    skyTransform[6] = renderContext->matrix_250 * distanceFactor;
    skyTransform[7] = renderContext->matrix_25c * distanceFactor;
    skyTransform[8] = renderContext->matrix_244 * distanceFactor;

    if (mode == 0)
    {
        primaryLayer = sky->layerMode0;
        secondaryLayer = sky->secondLayerMode0;
        drawState = &sky->drawStateModes0To4;
        animationTable = sky->animationModes0To1;
        colorInput = sky->colorInputs[0];
        secondColorInput = sky->colorInputs[1];
    }
    else if (mode == 1)
    {
        primaryLayer = sky->layerMode1;
        secondaryLayer = sky->secondLayerMode1;
        drawState = &sky->drawStateModes0To4;
        animationTable = sky->animationModes0To1;
        colorInput = sky->colorInputs[2];
        secondColorInput = sky->colorInputs[3];
    }
    else if (mode == 2)
    {
        primaryLayer = sky->layerMode2;
        secondaryLayer = sky->secondLayerMode2;
        drawState = &sky->drawStateModes0To4;
        animationTable = sky->animationMode2;
        colorInput = sky->colorInputs[4];
        secondColorInput = sky->colorInputs[5];
    }
    else if (mode == 3)
    {
        primaryLayer = sky->layerMode3;
        secondaryLayer = sky->secondLayerMode3;
        drawState = &sky->drawStateModes0To4;
        animationTable = sky->animationModes0To1;
        colorInput = sky->colorInputs[6];
        secondColorInput = sky->colorInputs[7];
    }
    else if (mode == 4)
    {
        primaryLayer = sky->layerMode4;
        secondaryLayer = sky->secondLayerMode4;
        drawState = &sky->drawStateModes0To4;
        animationTable = sky->animationModes0To1;
        colorInput = sky->colorInputs[8];
        secondColorInput = sky->colorInputs[9];
    }
    else if (mode == 5)
    {
        primaryLayer = sky->layerMode5;
        secondaryLayer = sky->secondLayerMode5;
        drawState = &sky->drawStateModes5To8;
        animationTable = sky->animationModes5To8;
        colorInput = sky->colorInputs[10];
        secondColorInput = sky->colorInputs[11];
    }
    else if (mode == 6)
    {
        primaryLayer = sky->layerMode6;
        secondaryLayer = sky->secondLayerMode6;
        drawState = &sky->drawStateModes5To8;
        animationTable = sky->animationModes5To8;
        colorInput = sky->colorInputs[12];
        secondColorInput = sky->colorInputs[13];
    }
    else if (mode == 7)
    {
        primaryLayer = sky->layerMode7;
        secondaryLayer = sky->secondLayerMode7;
        drawState = &sky->drawStateModes5To8;
        animationTable = sky->animationModes5To8;
        colorInput = sky->colorInputs[14];
        secondColorInput = sky->colorInputs[15];
    }
    else if (mode == 8)
    {
        primaryLayer = sky->layerMode8;
        secondaryLayer = sky->secondLayerMode8;
        drawState = &sky->drawStateModes5To8;
        animationTable = sky->animationModes5To8;
        colorInput = sky->colorInputs[16];
        secondColorInput = sky->colorInputs[17];
    }
    else
    {
        goto FinishSkyPaint;
    }

    if (primaryLayer != 0)
    {
        EQ_CDECL(void, 0x10045BBD, (void))( );
        renderContext->cachedDepthMode = -33;
        renderContext->cachedDepthWriteMode = -33;
        paintData = primaryLayer->paintData;
        layerData = paintData->layer;
        distanceFactor = EQGFX_MESH_SCALE_F_GUESS[layerData->scaleIndex];
        EQ_CDECL(void, 0x1003D238, (void *, void *))(renderContext, paintData);

        skyTransform[0] = renderContext->matrix_248 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[1] = renderContext->matrix_254 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[2] = renderContext->matrix_22c * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[3] = renderContext->matrix_24c * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[4] = renderContext->matrix_258 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[5] = renderContext->matrix_238 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[6] = renderContext->matrix_250 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[7] = renderContext->matrix_25c * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[8] = renderContext->matrix_244 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;

        transformedDirectionX = skyTransform[3] * primaryLayer->directionY
            + skyTransform[0] * primaryLayer->directionX + skyTransform[6] * primaryLayer->directionZ;
        transformedDirectionY = skyTransform[4] * primaryLayer->directionY
            + skyTransform[1] * primaryLayer->directionX + skyTransform[7] * primaryLayer->directionZ;
        transformedDirectionZ = skyTransform[5] * primaryLayer->directionY
            + skyTransform[2] * primaryLayer->directionX + skyTransform[8] * primaryLayer->directionZ;
        skyTransform[9] = transformedDirectionX;
        skyTransform[10] = transformedDirectionY;
        skyTransform[11] = transformedDirectionZ;

        skyTransform[0] = distanceFactor * skyTransform[0];
        skyTransform[1] = distanceFactor * skyTransform[1];
        skyTransform[2] = distanceFactor * skyTransform[2];
        skyTransform[3] = distanceFactor * skyTransform[3];
        skyTransform[4] = distanceFactor * skyTransform[4];
        skyTransform[5] = distanceFactor * skyTransform[5];
        skyTransform[6] = distanceFactor * skyTransform[6];
        skyTransform[7] = distanceFactor * skyTransform[7];
        skyTransform[8] = distanceFactor * skyTransform[8];

        sampleCount = layerData->sampleCount;
        vertexCount = layerData->vertexCount;
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(renderContext->sampleMask, 1, vertexCount);
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(renderContext->vertexMask, 1, sampleCount);
        EQ_CDECL(void, 0x100383EC, (void *, void *, float *, unsigned int, void *))(
            layerData->samples, renderContext->transformedVertices, skyTransform, sampleCount, renderContext->vertexMask);
        EQ_CDECL(void, 0x10045C77, (void *, void *, void *, unsigned int))(
            colorInput, renderContext->interpolatedColors, sky, sampleCount);
        EQGFX_SKY_PAINT_CALLBACK_GUESS(renderContext->interpolatedColors, sampleCount, renderContext->vertexMask);
        EQ_CDECL(void, 0x1003CF06, (void *, void *))(layerData->paintData, renderContext);
        EQ_CDECL(void, 0x1003CDF0, (void *, void *))(renderContext, paintData);
        EQ_CDECL(void, 0x1003C119, (void *, void *, void *, int))(
            animationContext, renderContext, paintData, primaryLayer->renderId);
        EQ_CDECL(void, 0x100460CB, (void))( );
        EQ_CDECL(void, 0x10046165, (void))( );
    }

    if ((animationTable != 0) && (drawState != 0))
    {
        EQ_CDECL(void, 0x10045BBD, (void))( );
        renderContext->cachedDepthMode = -33;
        renderContext->cachedDepthWriteMode = -33;
        drawState->x = -renderContext->skyOriginX;
        drawState->y = -renderContext->skyOriginY;
        drawState->flags = (unsigned char)(drawState->flags | 0x20);
        drawState->distance = EQGFX_SKY_DISTANCE_GUESS;
        drawState->z = -renderContext->skyOriginZ;
        renderContext->activeSkyAnimation = (int)animationTable;

        trackPointer = animationTable->tracks;
        trackEnd = (unsigned int)trackPointer + (unsigned int)animationTable->trackCount * EQGFX_SKY_TRACK_STRIDE_GUESS;
        if ((unsigned int)trackPointer < trackEnd)
        {
            do
            {
                frameCount = EQ_CDECL(int, 0x1001CE1E, (void *))(((SkyAnimationTrack_guess *)trackPointer)->track);
                frameProduct = ((SkyAnimationTrack_guess *)trackPointer)->frameRate * frameCount;
                __asm
                {
                    fild dword ptr frameProduct
                    fmul dword ptr animationTable
                    mov eax, EQGFX_SKY_FTOL_ADDRESS_GUESS
                    call eax
                    mov frameOffset, eax
                }
                EQ_CDECL(void, 0x10013C06, (void *, int, int))(
                    trackPointer, 0, animationContext->frameTime - frameOffset);
                trackPointer += EQGFX_SKY_TRACK_STRIDE_GUESS;
            } while ((unsigned int)trackPointer < trackEnd);
        }

        EQGFX_RENDER_VISIBILITY_MODE_GUESS = 0;
        EQ_CDECL(void, 0x10032462, (void *, void *, void *))(animationContext, renderContext, drawState);
        EQGFX_RENDER_VISIBILITY_MODE_GUESS = 1;
        EQ_CDECL(void, 0x100460CB, (void))( );
        EQ_CDECL(void, 0x10046165, (void))( );
    }

    if ((sky->dualLayer == 1) && (secondaryLayer != 0))
    {
        EQ_CDECL(void, 0x10045BBD, (void))( );
        renderContext->cachedDepthMode = -33;
        renderContext->cachedDepthWriteMode = -33;
        paintData = secondaryLayer->paintData;
        layerData = paintData->layer;
        distanceFactor = EQGFX_MESH_SCALE_F_GUESS[layerData->scaleIndex];
        EQ_CDECL(void, 0x1003D238, (void *, void *))(renderContext, paintData);

        skyTransform[0] = renderContext->matrix_248 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[1] = renderContext->matrix_254 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[2] = renderContext->matrix_22c * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[3] = renderContext->matrix_24c * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[4] = renderContext->matrix_258 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[5] = renderContext->matrix_238 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[6] = renderContext->matrix_250 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[7] = renderContext->matrix_25c * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;
        skyTransform[8] = renderContext->matrix_244 * EQGFX_SKY_DISTANCE_GLOBAL_GUESS;

        transformedDirectionX = skyTransform[3] * secondaryLayer->directionY
            + skyTransform[0] * secondaryLayer->directionX + skyTransform[6] * secondaryLayer->directionZ;
        transformedDirectionY = skyTransform[4] * secondaryLayer->directionY
            + skyTransform[1] * secondaryLayer->directionX + skyTransform[7] * secondaryLayer->directionZ;
        transformedDirectionZ = skyTransform[5] * secondaryLayer->directionY
            + skyTransform[2] * secondaryLayer->directionX + skyTransform[8] * secondaryLayer->directionZ;
        skyTransform[9] = transformedDirectionX;
        skyTransform[10] = transformedDirectionY;
        skyTransform[11] = transformedDirectionZ;

        skyTransform[0] = distanceFactor * skyTransform[0];
        skyTransform[1] = distanceFactor * skyTransform[1];
        skyTransform[2] = distanceFactor * skyTransform[2];
        skyTransform[3] = distanceFactor * skyTransform[3];
        skyTransform[4] = distanceFactor * skyTransform[4];
        skyTransform[5] = distanceFactor * skyTransform[5];
        skyTransform[6] = distanceFactor * skyTransform[6];
        skyTransform[7] = distanceFactor * skyTransform[7];
        skyTransform[8] = distanceFactor * skyTransform[8];

        sampleCount = layerData->sampleCount;
        vertexCount = layerData->vertexCount;
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(renderContext->sampleMask, 1, vertexCount);
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(renderContext->vertexMask, 1, sampleCount);
        EQ_CDECL(void, 0x100383EC, (void *, void *, float *, unsigned int, void *))(
            layerData->samples, renderContext->transformedVertices, skyTransform, sampleCount, renderContext->vertexMask);
        EQ_CDECL(void, 0x10045C77, (void *, void *, void *, unsigned int))(
            secondColorInput, renderContext->interpolatedColors, sky, sampleCount);
        EQGFX_SKY_PAINT_CALLBACK_GUESS(renderContext->interpolatedColors, sampleCount, renderContext->vertexMask);
        EQ_CDECL(void, 0x1003CF06, (void *, void *))(layerData->paintData, renderContext);
        EQ_CDECL(void, 0x1003CDF0, (void *, void *))(renderContext, paintData);
        EQ_CDECL(void, 0x1003C119, (void *, void *, void *, int))(
            animationContext, renderContext, paintData, secondaryLayer->renderId);
        EQ_CDECL(void, 0x100460CB, (void))( );
        EQ_CDECL(void, 0x10046165, (void))( );
    }

FinishSkyPaint:
    EQ_CDECL(void, 0x10045C11, (void))( );
    return 0;
}

// FUNCTION: EQGFX 0x10045bbd
// Disables depth and fog while the sky is painted.
// __cdecl (callee pops 0 bytes; ECX is not an input), no arguments, void (the callers never read EAX after
// the call). Mode 1 drops the device's z states and clears the cached special-effects state; mode 2 turns
// the Glide depth and fog states off.
void __cdecl _t3dDisableSkyDepthAndFog_guess(void)
{
    g_fog_enabled_guess = 0;
    switch (g_render_mode_guess)
    {
    case 1:
        if (g_render_device_guess != 0)
        {
            ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_Z_ENABLE_STATE_GUESS, 0);
            ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_Z_WRITE_ENABLE_STATE_GUESS, 0);
        }
        EQ_CDECL(void, 0x1004A23B, (int))(0);
        break;
    case 2:
        g_gr_depth_mask_fn_guess(0);
        g_gr_depth_buffer_mode_fn_guess(0);
        g_gr_fog_mode_fn_guess(0);
        return;
    }
}

// FUNCTION: EQGFX 0x10045c11
// Restores the saved depth and fog state after the sky is painted.
// __cdecl, no arguments, void (the callers never read EAX after the call): puts the saved fog enable back
// into the cache and restores what the mode's disable pass dropped (mode 1 device z states, mode 2 Glide fog).
void __cdecl t3dRestoreFogState_guess(void)
{
    g_fog_enabled_guess = g_saved_fog_enabled_guess;
    switch (g_render_mode_guess)
    {
    case 1:
        if (g_render_device_guess != 0)
        {
            ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_Z_ENABLE_STATE_GUESS, 1);
            ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_Z_WRITE_ENABLE_STATE_GUESS, 1);
        }
        EQ_CDECL(void, 0x1004A23B, (int))(g_fog_enabled_guess);
        break;
    case 2:
        g_gr_depth_mask_fn_guess(1);
        g_gr_depth_buffer_mode_fn_guess(2);
        if (g_fog_enabled_guess != 0)
        {
            g_gr_fog_mode_fn_guess(2);
            return;
        }
        break;
    }
}

// FUNCTION: EQGFX 0x10045c77
// Provides smoothly blended sky colors for the requested sky vertices.
// __cdecl (callee pops 0 bytes), void. Blends each column's color from sky rows row and row + 1 (weight rowWeight for row,
// the rest for row + 1) into three floats per vertex; the sky argument is reused as the remaining-vertex count.
void __cdecl interpolateVertexColors_guess(unsigned int *columns, float *outColors, int sky, int count)
{
    unsigned char *vertexColors;
    float rowWeight;
    float nextWeight;
    int row;
    int nextRow;
    unsigned int column;
    unsigned char *rowColor;
    unsigned char *nextRowColor;

    vertexColors = ((T3DSky_guess *)sky)->vertexColors;
    if (sky != 0 && vertexColors != 0)
    {
        rowWeight = ((T3DSky_guess *)sky)->colorRowWeight;
        row = ((T3DSky_guess *)sky)->colorRow;
        nextWeight = 1.0 - rowWeight;
        if (row > SKY_LAST_ROW)
        {
            row = 0;
        }
        nextRow = row + 1;
        if (nextRow > SKY_LAST_ROW)
        {
            nextRow = 0;
        }
        if (count != 0)
        {
            sky = count;
            do
            {
                column = *columns;
                if (column > SKY_LAST_COLUMN)
                {
                    column = 0;
                }
                nextRowColor = vertexColors + (nextRow * SKY_COLUMNS + column) * 4;
                rowColor = vertexColors + (column + row * SKY_COLUMNS) * 4;
                columns++;
                *outColors = rowWeight * g_byteFloatTable_guess[rowColor[2]] + nextWeight * g_byteFloatTable_guess[nextRowColor[2]];
                sky--;
                outColors[1] = rowWeight * g_byteFloatTable_guess[rowColor[1]] + nextWeight * g_byteFloatTable_guess[nextRowColor[1]];
                outColors[2] = rowWeight * g_byteFloatTable_guess[*rowColor] + nextWeight * g_byteFloatTable_guess[*nextRowColor];
                outColors += 3;
            } while (sky != 0);
        }
    }
}

// FUNCTION: EQGFX 0x10045d5b
// Thunk for vtable slot 0x0c; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer; forwards it to vtable slot 0x0c of the
// interface behind the object and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextGetDD(void *object)
{
    return ((D3DXContextGetDDFn)(*(void ***)object)[3])(object); // +0x0c
}

// FUNCTION: EQGFX 0x10045d66
// Thunk for vtable slot 0x10; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer; forwards it to vtable slot 0x10 of the
// interface behind the object and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextGetD3D(void *object)
{
    return ((D3DXContextGetD3DFn)(*(void ***)object)[4])(object); // +0x10
}

// FUNCTION: EQGFX 0x10045d71
// Thunk for vtable slot 0x14; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer; forwards it to vtable slot 0x14 of the
// interface behind the object and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextGetD3DDevice(void *object)
{
    return ((D3DXContextGetD3DDeviceFn)(*(void ***)object)[5])(object); // +0x14
}

// FUNCTION: EQGFX 0x10045d7c
// Thunk for vtable slot 0x18; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer; forwards it to vtable slot 0x18 of the
// interface behind the object and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextGetPrimary(void *object)
{
    return ((D3DXContextGetPrimaryFn)(*(void ***)object)[6])(object); // +0x18
}

// FUNCTION: EQGFX 0x10045d87
// Thunk for vtable slot 0x20; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer and a value; forwards both to vtable slot
// 0x20 of `object` and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextGetBackBuffer(void *object, int which)
{
    return ((D3DXContextGetBackBufferFn)(*(void ***)object)[8])(object, which); // +0x20
}

// FUNCTION: EQGFX 0x10045d96
// Thunk for vtable slot 0x4c; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer and a value; forwards both to vtable slot
// 0x4c of `object` and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextUpdateFrame(void *object, int value)
{
    return ((D3DXContextUpdateFrameFn)(*(void ***)object)[19])(object, value);
}

// FUNCTION: EQGFX 0x10045da5
// Thunk for vtable slot 0x44; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer; forwards it to vtable slot 0x44 of the
// interface behind the object, with the object itself as the method's argument.
void __cdecl t3dD3DXContextRestoreSurfaces(void *object)
{
    ((D3DXContextRestoreSurfacesFn)(*(void ***)object)[17])(object);
}

// FUNCTION: EQGFX 0x10045dce
// Thunk for vtable slot 0x08; what the method releases is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer; forwards it to vtable slot 0x08 of the
// interface behind the object and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextRelease(void *object)
{
    return ((D3DXContextReleaseFn)(*(void ***)object)[2])(object); // +0x08
}

// FUNCTION: EQGFX 0x10045dd9
// Prepares renderer state for a partial scene render.
// __cdecl (callee pops 0 bytes; ECX is not an input), no arguments and its EAX result is dead: bumps the
// renderer counter at 0x100fbbb0 and clears sixteen .data words of the same DLL, in the order written.
void __cdecl clearRendererState_guess(void)
{
    EQ_GLOBAL(int, 0x100FBBB0)++;
    EQ_GLOBAL(int, 0x100FBBC8) = 0;
    EQ_GLOBAL(int, 0x100FBBB4) = 0;
    EQ_GLOBAL(int, 0x100FC788) = 0;
    EQ_GLOBAL(int, 0x100FC790) = 0;
    EQ_GLOBAL(int, 0x1079FC00) = 0;
    EQ_GLOBAL(int, 0x104AFC20) = 0;
    EQ_GLOBAL(int, 0x107037E0) = 0;
    EQ_GLOBAL(int, 0x1025C0A0) = 0;
    EQ_GLOBAL(int, 0x10377400) = 0;
    EQ_GLOBAL(int, 0x1054C040) = 0;
    EQ_GLOBAL(int, 0x1055F8E4) = 0;
    EQ_GLOBAL(int, 0x100FC78C) = 0;
    EQ_GLOBAL(int, 0x1055F8E0) = 0;
    EQ_GLOBAL(int, 0x106BF200) = 0;
    EQ_GLOBAL(int, 0x1083C020) = 0;
    EQ_GLOBAL(int, 0x10377404) = 0;
}

// FUNCTION: EQGFX 0x10045db0
// Thunk for vtable slot 0x54; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer and a value; forwards both to vtable slot
// 0x54 of `object` and returns the method's result (the callers read EAX after the call).
int __cdecl t3dD3DXContextClear(void *object, int value)
{
    return ((D3DXContextClearFn)(*(void ***)object)[21])(object, value); // +0x54
}

// FUNCTION: EQGFX 0x10045dbf
// Thunk for vtable slot 0x58; the method's game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input), a pointer and a value; forwards both to vtable slot
// 0x58 of `object` and returns the method's result.
int __cdecl t3dD3DXContextSetClearColor(void *object, int color)
{
    return ((D3DXContextSetClearColorFn)(*(void ***)object)[22])(object, color);
}

#define g_100fc788_guess   EQ_GLOBAL(int, 0x100FC788)
#define g_1079fc00_guess   EQ_GLOBAL(int, 0x1079FC00)
#define g_100fc790_guess   EQ_GLOBAL(int, 0x100FC790)
#define g_104afc20_guess   EQ_GLOBAL(int, 0x104AFC20)
#define flushEffectBufferA EQ_CDECL(void, 0x100460CB, (void))
#define flushEffectBufferB EQ_CDECL(void, 0x10046165, (void))

// FUNCTION: EQGFX 0x10045e32
// __cdecl, one int (the entries about to be added): flushes buffer A, then B, when either count passes its limit.
void __cdecl checkAndFlushEffectBuffers_guess(int count)
{
    if ((unsigned int)(g_100fc788_guess + count) > 10000 || (unsigned int)(g_1079fc00_guess + count * 9) > 40000) {
        flushEffectBufferA();
    }
    if ((unsigned int)(g_100fc790_guess + count) > 10000 || (unsigned int)(g_104afc20_guess + count * 9) > 40000) {
        flushEffectBufferB();
    }
}
