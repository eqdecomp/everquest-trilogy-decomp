// unit_10045E85.c - eqgfx_dx7.dll, the address run 0x10045E85..0x10048FAD (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include "eqgfx/layout/t3d_texinfo.h"

// Particle cloud (render context) fields t3dDeferParticlePoint_guess reads: +0x198 halfWidth, +0x19C halfHeight, +0x1A0 nearZ,
// +0x1A4 sizeScale, +0x1A8 farZ, +0x520 viewportX, +0x524 viewportY. t3dDeferPolygon also reads +0x184 clock,
// +0x1B8 extraLight, +0x27C, +0x29C ambient, +0x2A8/+0x2AC the light list and +0x2B0 direction.
typedef struct ParticleCloud_guess
{
    unsigned char unknown_000[0x184];
    unsigned int clock;
    unsigned char unknown_188[0x198 - 0x188];
    float halfWidth;
    float halfHeight;
    float nearZ;
    float sizeScale;
    float farZ;
    unsigned char unknown_1ac[0x1B8 - 0x1AC];
    float extraLight[3];
    unsigned char unknown_1c4[0x27C - 0x1C4];
    int field27c;
    unsigned char unknown_280[0x29C - 0x280];
    float ambient[3];
    int lightCount;
    struct SceneLight_guess **lights;
    float direction[3];
    unsigned char unknown_2bc[0x520 - 0x2BC];
    float viewportX;
    float viewportY;
} ParticleCloud_guess;

// One 0x20-byte vertex record: x, y, z, w at +0x0..+0xC, color at +0x10, fog at +0x14, u, v at +0x18/+0x1C
// (written only by t3dDeferPolygon's textured paths).
typedef struct ParticleVertex_guess
{
    float x;
    float y;
    float z;
    float w;
    unsigned int color;
    unsigned int fog;
    float u;
    float v;
} ParticleVertex_guess;

// t3dDeferPolygon's inputs: a light of the context's list (type 0x1F, +0x14 animation, +0x18 direction), its
// animation (+0xC control, +0x14 frame, +0x18 interval, +0x1C next clock, +0x20 mode, +0x24 frames), the frames
// (+0x14 count, +0x18 one level or a level per frame, +0x1C colors or 0), the mesh (+0x4 count) and the material.
typedef struct LightColor_guess
{
    float red;
    float green;
    float blue;
} LightColor_guess;

typedef struct LightFrames_guess
{
    unsigned char unknown_00[0x14];
    unsigned int count;
    union
    {
        float value;
        float *values;
    } level;
    LightColor_guess *colors;
} LightFrames_guess;

typedef struct LightAnimation_guess
{
    unsigned char unknown_00[0xC];
    unsigned char control;
    unsigned char unknown_0d[0x14 - 0xD];
    unsigned int frameIndex;
    unsigned int interval;
    unsigned int nextClock;
    int mode;
    LightFrames_guess *frames;
} LightAnimation_guess;

typedef struct SceneLight_guess
{
    int type;
    unsigned char unknown_04[0x10];
    LightAnimation_guess *animation;
    float direction[3];
} SceneLight_guess;

typedef struct LitMesh_guess
{
    int unknown_00;
    int count;
    int unknown_08;
    int field0c;
} LitMesh_guess;

// One 0x30-byte transformed vertex the t3dClipPolygonFrustumRGB_guess family returns: x, y, z, u, v at +0x10, red/green/blue at +0x24.
typedef struct LitVertex_guess
{
    float x;
    float y;
    float z;
    float unknown_0c;
    float u;
    float v;
    unsigned char unknown_18[0x24 - 0x18];
    int red;
    int green;
    int blue;
} LitVertex_guess;

// Material: +0x8 brightness, +0xC ambient scale, +0x10 bit 0 = tint, +0x14..+0x16 tint blue/green/red, +0x18 type
// (0..3), +0x19 alpha, +0x1C texture, +0x44 passed to the mesh transforms.
typedef struct Material_guess
{
    unsigned char unknown_00[8];
    float brightness;
    float ambientScale;
    unsigned char flags;
    unsigned char unknown_11[3];
    unsigned char tintBlue;
    unsigned char tintGreen;
    unsigned char tintRed;
    unsigned char unknown_17;
    unsigned char type;
    unsigned char alpha;
    unsigned char unknown_1a[2];
    struct EffectsUsedNode_guess *texture;
    unsigned char unknown_20[0x44 - 0x20];
    int field44;
} Material_guess;

// eqgfx_dx7.dll's own globals used here (VAs of this image, not trilogy's).
#define g_fps_tx_memory_guess EQ_GLOBAL(int, 0x100FBB8C) // g_fps_tx_memory_guess: accumulates +0x18 of each texture first used in effects
#define g_effects_used_list_guess EQ_GLOBAL(int, 0x100FBA64) // g_effects_used_list_guess: head of the textures-used-in-effects list
#define g_render_list_head_guess EQ_GLOBAL(int, 0x100FBA5C) // g_render_list_head_guess: head of the render-object list (+0x78 prev, +0x7c next)
#define g_render_list_tail_guess EQ_GLOBAL(int, 0x100FBA60) // g_render_list_tail_guess: tail of the render-object list
#define g_render_mode_guess EQ_GLOBAL(int, 0x100FB8A8) // g_render_mode_guess: 1 = unlink path, 2 = table-free path
#define g_100fc794_guess EQ_GLOBAL(int, 0x100FC794) // DAT_100fc794: pending 6-byte vertices for the mode-1 emit
#define g_104afc24_guess EQ_GLOBAL(int, 0x104AFC24) // DAT_104afc24: pending 12-byte vertices for the mode-2 emit
#define g_100fbbc8_guess EQ_GLOBAL(int, 0x100FBBC8) // g_primCount_guess: count of emitted primitives
#define g_fps_swap_count_guess EQ_GLOBAL(int, 0x100FBB84) // g_fps_swap_count_guess: count of objects mode 2 adds to the table
#define g_1079fc00_guess EQ_GLOBAL(int, 0x1079FC00) // DAT_1079fc00: stack arg for the mode-1 emit call
#define g_100fba6c_guess EQ_GLOBAL(int, 0x100FBA6C) // g_render_device_guess: gfx object whose vtable takes slots 0x50/0x68
#define g_100fbbbc_guess EQ_GLOBAL(int, 0x100FBBBC) // DAT_100fbbbc: effect list head cleared on init
#define g_100fbbc0_guess EQ_GLOBAL(int, 0x100FBBC0) // DAT_100fbbc0: effect list head cleared on init
#define g_100fbbc4_guess EQ_GLOBAL(int, 0x100FBBC4) // DAT_100fbbc4: effect list tail/count cleared on init
#define g_vertBuf6_guess EQ_ARRAY(char, 0x102F84C0) // g_vertBuf6_guess: 6-byte vertex staging for mode 1
#define g_vertBuf12_guess EQ_ARRAY(char, 0x10698100) // g_vertBuf12_guess: 12-byte vertex staging for mode 2
#define g_101238a0_guess EQ_ARRAY(char, 0x101238A0) // DAT_101238a0: stack arg for the mode-1 emit call
#define g_initialize_window_state_guess EQ_GLOBAL(int, 0x100FB8B8) // g_initialize_window_state_guess: FPS-enable flag set by t3dFPSEnable
#define g_rain_red_guess EQ_GLOBAL(int, 0x100A2674) // g_rain_red_guess: rain color channel 0
#define g_rain_green_guess EQ_GLOBAL(int, 0x100A2678) // g_rain_green_guess: rain color channel 1
#define g_rain_blue_guess EQ_GLOBAL(int, 0x100A267C) // g_rain_blue_guess: rain color channel 2
#define g_fog_enabled_guess EQ_GLOBAL(int, 0x100FBA1C) // g_fog_enabled_guess: fog enable (grFogMode 2 runs when set)
#define g_100fbb9c_guess EQ_GLOBAL(int, 0x100FBB9C) // DAT_100fbb9c: arg of the t3dSetTextureAlphaOp_guess call at the end of mode 1
#define g_100fbba4_guess EQ_GLOBAL(int, 0x100FBBA4) // DAT_100fbba4: t3dSetTextureAlphaOp_guess arg for a mode-1 node without the fog bit
#define g_initialize_texture_state3_guess EQ_GLOBAL(int, 0x100FBBA8) // g_initialize_texture_state3_guess: t3dSetTextureAlphaOp_guess arg for a mode-1 node with the fog bit
#define g_initialize_texture_state4_guess EQ_GLOBAL(int, 0x100FBBAC) // g_initialize_texture_state4_guess: nonzero forces the state-0 call in mode 1
#define g_104afc20_guess EQ_GLOBAL(int, 0x104AFC20) // DAT_104afc20: count arg of the mode-1 vertex emit call
#define g_1055f900_guess EQ_ARRAY(char, 0x1055F900) // DAT_1055f900: pointer arg of the mode-1 vertex emit call
#define g_1055f8e4_guess EQ_GLOBAL(int, 0x1055F8E4) // DAT_1055f8e4: vertex count passed with DAT_10377420 to the gfx slot 0x64 call, zeroed after it
#define g_10377420_guess EQ_ARRAY(char, 0x10377420) // DAT_10377420: vertex buffer passed to the gfx slot 0x64 call
#define g_100fc788_guess EQ_GLOBAL(int, 0x100FC788) // DAT_100fc788: zeroed by t3dFlushDeferredPrims_guess
#define g_1025c0a0_guess EQ_GLOBAL(int, 0x1025C0A0) // DAT_1025c0a0: zeroed by t3dFlushDeferredPrims_guess
#define g_10377400_guess EQ_GLOBAL(int, 0x10377400) // DAT_10377400: zeroed by t3dFlushDeferredPrims_guess
#define g_100fc78c_guess EQ_GLOBAL(int, 0x100FC78C)
#define g_104afc40_guess EQ_ARRAY(char, 0x104AFC40) // DAT_104afc40: vertex buffer A, 0x20-byte records (count DAT_106bf200)
#define g_106bf200_guess EQ_GLOBAL(unsigned int, 0x106BF200) // DAT_106bf200: count of records in buffer A
#define g_100fbb94_guess EQ_GLOBAL(float, 0x100FBB94) // DAT_100fbb94: depth subtracted before the mode-1 fog scale
#define g_100fbb98_guess EQ_GLOBAL(float, 0x100FBB98) // DAT_100fbb98: mode-1 fog scale
#define g_1055f8e0_guess EQ_GLOBAL(int, 0x1055F8E0)
#define g_1083c020_guess EQ_GLOBAL(int, 0x1083C020)
#define g_10377404_guess EQ_GLOBAL(int, 0x10377404)
#define g_100fc790_guess EQ_GLOBAL(int, 0x100FC790)
#define g_107037e0_guess EQ_GLOBAL(int, 0x107037E0)
#define g_1054c040_guess EQ_GLOBAL(int, 0x1054C040)
#define g_initialize_texture_state1_guess EQ_GLOBAL(int, 0x100FBBA0)
#define g_10703800_guess EQ_ARRAY(char, 0x10703800)
#define g_10413820_guess EQ_ARRAY(char, 0x10413820)
#define g_1025c0c0_guess EQ_ARRAY(char, 0x1025C0C0)
#define g_1079fc20_guess EQ_ARRAY(char, 0x1079FC20)
#define g_10332e40_guess EQ_ARRAY(char, 0x10332E40) // DAT_10332e40: 0x1c-byte prim nodes of the opaque lists (DAT_100fc788 used)
#define g_100fc7a0_guess EQ_ARRAY(char, 0x100FC7A0) // DAT_100fc7a0: vertex-pointer triples of the mode-2 opaque nodes (DAT_10377400 used)
#define g_1083c040_guess EQ_ARRAY(char, 0x1083C040) // DAT_1083c040: 16-bit index triples of the mode-1 opaque nodes (DAT_1025c0a0 used)
#define g_106bf220_guess EQ_ARRAY(char, 0x106BF220) // DAT_106bf220: 0x1c-byte prim nodes of the blended lists (DAT_100fc790 used)
#define g_1030bd40_guess EQ_ARRAY(char, 0x1030BD40) // DAT_1030bd40: vertex-pointer triples of the mode-2 blended nodes (DAT_1054c040 used)
#define g_1054c060_guess EQ_ARRAY(char, 0x1054C060) // DAT_1054c060: 16-bit index triples of the mode-1 blended nodes (DAT_107037e0 used)

#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284) // EQGFX_ZERO_F_GUESS: zero used by clipping comparisons
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280) // EQGFX_FLOAT_ONE: one used by clipping parameter bounds
typedef void (__stdcall *grDrawVertexArray_t_guess)(int, int, void *); // _grDrawVertexArray@12 slot
#define grDrawVertexArray_guess EQ_GLOBAL(grDrawVertexArray_t_guess, 0x100F3EE0)
typedef void (__stdcall *grDrawLine_t_guess)(int, int); // _grDrawLine@8 slot
#define grDrawLine_guess EQ_GLOBAL(grDrawLine_t_guess, 0x100F3ED4)
typedef void (__stdcall *grDrawPoint_t_guess)(int); // _grDrawPoint@4 slot
#define grDrawPoint_guess EQ_GLOBAL(grDrawPoint_t_guess, 0x100F3ED8)
typedef void (__stdcall *grAlphaTestFunction_t_guess)(int); // _grAlphaTestFunction@4 slot
#define grAlphaTestFunction_guess EQ_GLOBAL(grAlphaTestFunction_t_guess, 0x100F3E84)
typedef void (__stdcall *grAlphaTestReferenceValue_t_guess)(int); // _grAlphaTestReferenceValue@4 slot
#define grAlphaTestReferenceValue_guess EQ_GLOBAL(grAlphaTestReferenceValue_t_guess, 0x100F3E88)
typedef void (__stdcall *grDepthMask_t_guess)(int); // _grDepthMask@4 slot
#define grDepthMask_guess EQ_GLOBAL(grDepthMask_t_guess, 0x100F3EC0)
typedef void (__stdcall *grFogMode_t_guess)(int); // _grFogMode@4 slot
#define grFogMode_guess EQ_GLOBAL(grFogMode_t_guess, 0x100F3EFC)
typedef void (__stdcall *grAlphaBlendFunction_t_guess)(int, int, int, int); // _grAlphaBlendFunction@16 slot
#define grAlphaBlendFunction_guess EQ_GLOBAL(grAlphaBlendFunction_t_guess, 0x100F3E78)
typedef void (__stdcall *grAlphaCombine_t_guess)(int, int, int, int, int); // _grAlphaCombine@20 slot
#define grAlphaCombine_guess EQ_GLOBAL(grAlphaCombine_t_guess, 0x100F3E7C)
typedef void (__stdcall *grColorCombine_t_guess)(int, int, int, int, int); // _grColorCombine@20 slot
#define grColorCombine_guess EQ_GLOBAL(grColorCombine_t_guess, 0x100F3EA0)
#define g_overlay_handle EQ_GLOBAL(void *, 0x100F3E2C) // g_current_overlay_guess: overlay handle, destroyed and cleared by releaseResourceHandle_guess
typedef void (__stdcall *gfxVtableSlot8c_t_guess)(int, int, int); // vtable slot 35 (+0x8c) of the gfx object at g_render_device_guess
typedef void (__stdcall *gfxVtableSlot50_t_guess)(int, int, int); // vtable slot 20 (+0x50) of the gfx object at g_render_device_guess
typedef void (__stdcall *gfxVtableSlot64_t_guess)(int, int, int, void *, int, int); // vtable slot 25 (+0x64) of the gfx object at g_render_device_guess

// List node walked here: +0x0 condition/flag word, +0x14 vertex source, +0x18 next link.
typedef struct PrimNode_guess
{
    int cond;
    void *texture;
    int unknown_08;
    int primType;
    int unknown_10;
    void *verts;
    struct PrimNode_guess *next;
} PrimNode_guess;

// Texture-used-in-effects node: +0x60 texture handle, +0x64/+0x68/+0x6c/+0x84 cleared, +0x70 next link, +0x74 flag.
// t3dDeferPolygon also reads +0x0 type (6 = +0x14 points at the real texture, 0x1B = texture) and +0x58/+0x5C uv scale.
typedef struct EffectsUsedNode_guess
{
    int type;
    char pad04[0x10];
    struct EffectsUsedNode_guess **frames;
    char pad18[0x58 - 0x18];
    float uScale;
    float vScale;
    int field60;
    int field64;
    int field68;
    int field6c;
    struct EffectsUsedNode_guess *next;
    int field74;
    char pad78[0xc];
    int field84;
} EffectsUsedNode_guess;

typedef struct ClipVolume_guess
{
    char pad000[0x1a0];
    float minimumZ;
    char pad1a4[4];
    float maximumZ;
} ClipVolume_guess;

// FUNCTION: EQGFX 0x10045e85
// __cdecl. Clears the three effect-list globals and the used-texture list head.
void __cdecl initializeEffectLists_guess(void)
{
    g_100fbbbc_guess = 0;
    g_100fbbc0_guess = 0;
    g_100fbbc4_guess = 0;
    g_effects_used_list_guess = 0;
}

// FUNCTION: EQGFX 0x10045e9c
// __cdecl. Clears the effect-list heads then clears four fields on each used-texture node.
void __cdecl resetListAndNodeState_guess(void)
{
    EffectsUsedNode_guess *node;

    node = (EffectsUsedNode_guess *)g_effects_used_list_guess;
    g_100fbbbc_guess = 0;
    g_100fbbc0_guess = 0;
    g_100fbbc4_guess = 0;
    while (node != 0)
    {
        node->field64 = 0;
        node->field68 = 0;
        node->field6c = 0;
        node->field84 = 0;
        node = node->next;
    }
    g_effects_used_list_guess = 0;
}

// FUNCTION: EQGFX 0x10045ED4
// __cdecl, one render-object pointer (0 allowed). Mode 1: texture to vtable slot 0x8c. Mode 2: -1 if 0x100314bc fails.
int __cdecl t3dBindTexture_guess(int renderObject)
{
    int textureHandle;

    switch (g_render_mode_guess)
    {
    case 1:
        if (renderObject == 0)
        {
            textureHandle = 0;
        }
        else
        {
            EQ_CDECL(void, 0x1004609A, (int))(renderObject);
            textureHandle = ((EffectsUsedNode_guess *)renderObject)->field60;
        }
        ((gfxVtableSlot8c_t_guess)(*(void ***)g_100fba6c_guess)[35])(g_100fba6c_guess, 0, textureHandle);
        break;
    case 2:
        if (renderObject == 0)
        {
            grColorCombine_guess(1, 0, 0, 2, 0);
        }
        else
        {
            if (((EffectsUsedNode_guess *)renderObject)->field74 == 0)
            {
                if (EQ_CDECL(int, 0x100314BC, (int *))(&g_render_mode_guess) != 0)
                {
                    return -1;
                }
                EQ_CDECL(void, 0x1004606C, (int))(renderObject);
                g_fps_swap_count_guess++;
            }
            else
            {
                EQ_CDECL(void, 0x10046006, (int))(renderObject);
                EQ_CDECL(void, 0x1004606C, (int))(renderObject);
            }
            grColorCombine_guess(3, 1, 0, 1, 0);
            EQ_CDECL(void, 0x10031B42, (int *, int))(&g_render_mode_guess, renderObject);
            EQ_CDECL(void, 0x1004609A, (int))(renderObject);
        }
        break;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10045f85
// Releases a render object's graphics resources.
// __cdecl (the callee pops 0 bytes; ECX is not an input). One render-object pointer.
// If its +0x74 flag is set: mode 1 unlinks it via 0x10046006 and clears +0x38;
// mode 2 frees its table slots via 0x10031190, returning 0xffff3d3e on failure.
// Returns 0 except for the mode-2 failure code.
int __cdecl releaseObjectResources_guess(int renderObject)
{
    int result;

    result = 0;
    if (*(int *)(renderObject + 0x74) != 0)
    {
        if (g_render_mode_guess == 1)
        {
            EQ_CDECL(void, 0x10046006, (int))(renderObject);
            if (*(int *)(renderObject + 0x38) != 0)
                *(int *)(renderObject + 0x38) = 0;
        }
        else if (g_render_mode_guess == 2)
        {
            if (EQ_CDECL(int, 0x10031190, (int *, int, int, int, int))(&g_render_mode_guess, renderObject, 0, 0, 0) != 0)
                result = 0xffff3d3e;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10045fcd
// __cdecl. Clears the render-object list head and tail.
void __cdecl clearGlobalPointers_guess(void)
{
    g_render_list_head_guess &= 0;
    g_render_list_tail_guess &= 0;
}

// FUNCTION: EQGFX 0x10045fdc
// __cdecl. Drains the list head at +0x1b4 via 0x10045f85, then clears +0x1b4/+0x1b8.
int __cdecl drainObjectList_guess(int renderContext)
{
    while (*(int *)(renderContext + 0x1b4) != 0)
    {
        EQ_CDECL(int, 0x10045F85, (int))(*(int *)(renderContext + 0x1b4));
    }
    *(int *)(renderContext + 0x1b4) &= 0;
    *(int *)(renderContext + 0x1b8) &= 0;
    return 0;
}

// FUNCTION: EQGFX 0x10046006
// Unlinks an object from the doubly linked list at 0x100fba5c/0x100fba60 and clears its +0x74 in-list flag.
// __cdecl (the callee pops 0 bytes; ECX is not an input). One render-object pointer.
// Unlinks it from the doubly linked render list (+0x78 prev, +0x7c next,
// head 0x100fba5c, tail 0x100fba60) and clears its +0x74/+0x78/+0x7c. Returns nothing.
void __cdecl removeRenderObjectFromList_guess(int renderObject)
{
    int prev;

    prev = *(int *)(renderObject + 0x78);
    ((T3dTexInfo_guess*)renderObject)->field74 = 0;
    if (prev != 0)
    {
        *(int *)(prev + 0x7c) = *(int *)(renderObject + 0x7c);
    }
    else if (renderObject == g_render_list_head_guess)
    {
        g_render_list_head_guess = *(int *)(renderObject + 0x7c);
        if (*(int *)(renderObject + 0x7c) != 0)
            *(int *)(*(int *)(renderObject + 0x7c) + 0x78) = 0;
    }
    if (*(int *)(renderObject + 0x7c) != 0)
    {
        *(int *)(*(int *)(renderObject + 0x7c) + 0x78) = *(int *)(renderObject + 0x78);
    }
    else if (renderObject == g_render_list_tail_guess)
    {
        g_render_list_tail_guess = *(int *)(renderObject + 0x78);
        if (*(int *)(renderObject + 0x78) != 0)
            *(int *)(*(int *)(renderObject + 0x78) + 0x7c) = 0;
    }
    *(int *)(renderObject + 0x7c) = 0;
    *(int *)(renderObject + 0x78) = 0;
}

// FUNCTION: EQGFX 0x1004606c
// Links an object at the head of the list at 0x100fba5c/0x100fba60 and sets its +0x74 in-list flag.
// __cdecl (the callee pops 0 bytes; ECX is not an input). One render-object pointer.
// Links it at the head of the render list (+0x78 prev, +0x7c next,
// head 0x100fba5c, tail 0x100fba60): clears prev, marks +0x74 as 1,
// pushes it on the head, fixing the old head's prev or the tail when empty.
void __cdecl linkNodeAtHead_guess(int renderObject)
{
    *(int *)(renderObject + 0x78) = 0;
    *(int *)(renderObject + 0x74) = 1;
    *(int *)(renderObject + 0x7c) = g_render_list_head_guess;
    g_render_list_head_guess = renderObject;
    if (*(int *)(renderObject + 0x7c) != 0)
    {
        *(int *)(*(int *)(renderObject + 0x7c) + 0x78) = renderObject;
        return;
    }
    g_render_list_tail_guess = renderObject;
}

// FUNCTION: EQGFX 0x1004609a
// Records textures needed by pending effect draws.
// __cdecl (the callee pops 0 bytes; ECX is not an input). One texture-object pointer.
// If its used-in-effects flag at +0x84 is clear, adds its +0x18 size to the
// effects total, sets the flag, and links it at the head of the used list
// (+0x70 is the next link). Returns nothing.
void __cdecl markTextureAsUsedInEffects_guess(int texture)
{
    if (*(int *)(texture + 0x84) == 0)
    {
        g_fps_tx_memory_guess += *(int *)(texture + 0x18);
        *(int *)(texture + 0x84) = 1;
        *(int *)(texture + 0x70) = g_effects_used_list_guess;
        g_effects_used_list_guess = texture;
    }
}

// FUNCTION: EQGFX 0x100460CB
// Completes pending effects draws for the current scene.
// __cdecl, no arguments, no return value. Sends the pending vertices to the gfx object, passes each used texture's prim list to emitPrimitiveList_guess, clears four globals.
void __cdecl t3dFlushDeferredPrims_guess(void)
{
    EffectsUsedNode_guess *node;

    if (g_100fbbbc_guess != 0)
    {
        EQ_CDECL(int, 0x10045ED4, (int))(0);
        EQ_CDECL(int, 0x10048B01, (int))(g_100fbbbc_guess);
        g_100fbbbc_guess = 0;
    }
    if (g_1055f8e4_guess != 0)
    {
        EQ_CDECL(int, 0x10045ED4, (int))(0);
        ((gfxVtableSlot64_t_guess)(*(void ***)g_100fba6c_guess)[25])(g_100fba6c_guess, 2, 0x1c4, g_10377420_guess, g_1055f8e4_guess, 0);
        g_1055f8e4_guess = 0;
    }
    for (node = (EffectsUsedNode_guess *)g_effects_used_list_guess; node != 0; node = node->next)
    {
        if (node->field64 != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))((int)node);
            EQ_CDECL(int, 0x10048B01, (int))(node->field64);
            node->field64 = 0;
        }
    }
    g_100fc788_guess = 0;
    g_1079fc00_guess = 0;
    g_1025c0a0_guess = 0;
    g_10377400_guess = 0;
}

// FUNCTION: EQGFX 0x10046165
// Draws each used texture's two deferred prim lists, then the blended and additive particle line/point buffers.
void __cdecl t3dFlushDeferredPrimitives_guess(void)
{
    EffectsUsedNode_guess *node;
    char *vertexData;
    char *pointData;
    unsigned int drawCount;
    unsigned int pointCount;

    switch (g_render_mode_guess)
    {
    case 1:
    {
        ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x1b, 1);
        EQ_CDECL(int, 0x1004A25E, (int))(g_initialize_texture_state1_guess);
        for (node = (EffectsUsedNode_guess *)g_effects_used_list_guess; node != 0; node = node->next)
        {
            if (node->field68 != 0)
            {
                EQ_CDECL(int, 0x10045ED4, (int))((int)node);
                EQ_CDECL(int, 0x10048CDF, (int))(node->field68);
                node->field68 = 0;
            }
        }
        EQ_CDECL(int, 0x1004A23B, (int))(0);
        EQ_CDECL(int, 0x1004A25E, (int))(g_100fbba4_guess);
        if (g_100fbbc0_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            EQ_CDECL(int, 0x10048CDF, (int))(g_100fbbc0_guess);
            g_100fbbc0_guess = 0;
        }
        for (node = (EffectsUsedNode_guess *)g_effects_used_list_guess; node != 0; node = node->next)
        {
            if (node->field6c != 0)
            {
                EQ_CDECL(int, 0x10045ED4, (int))((int)node);
                EQ_CDECL(int, 0x10048CDF, (int))(node->field6c);
                node->field6c = 0;
            }
        }
        g_100fc790_guess = 0;
        g_104afc20_guess = 0;
        g_107037e0_guess = 0;
        if (g_100fc78c_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            EQ_CDECL(int, 0x1004A23B, (int))(0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xf, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xe, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x13, 5);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x14, 6);
            ((gfxVtableSlot64_t_guess)(*(void ***)g_100fba6c_guess)[25])
                (g_100fba6c_guess, 2, 0x1c4, g_10703800_guess, g_100fc78c_guess, 0);
            g_100fc78c_guess = 0;
        }
        if (g_1055f8e0_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            EQ_CDECL(int, 0x1004A23B, (int))(0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xf, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xe, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x13, 2);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x14, 2);
            ((gfxVtableSlot64_t_guess)(*(void ***)g_100fba6c_guess)[25])
                (g_100fba6c_guess, 2, 0x1c4, g_10413820_guess, g_1055f8e0_guess, 0);
            g_1055f8e0_guess = 0;
        }
        if (g_1083c020_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            EQ_CDECL(int, 0x1004A23B, (int))(0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xf, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xe, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x13, 5);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x14, 6);
            ((gfxVtableSlot64_t_guess)(*(void ***)g_100fba6c_guess)[25])
                (g_100fba6c_guess, 1, 0x1c4, g_1025c0c0_guess, g_1083c020_guess, 0);
            g_1083c020_guess = 0;
        }
        if (g_10377404_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            EQ_CDECL(int, 0x1004A23B, (int))(0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xf, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xe, 0);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x13, 2);
            ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x14, 2);
            ((gfxVtableSlot64_t_guess)(*(void ***)g_100fba6c_guess)[25])
                (g_100fba6c_guess, 1, 0x1c4, g_1079fc20_guess, g_10377404_guess, 0);
            g_10377404_guess = 0;
        }
        ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x13, 2);
        ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x14, 1);
        ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x1b, 0);
        ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xe, 1);
        ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0x19, 8);
        ((gfxVtableSlot50_t_guess)(*(void ***)g_100fba6c_guess)[20])(g_100fba6c_guess, 0xf, 0);
        if (g_fog_enabled_guess != 0)
            EQ_CDECL(int, 0x1004A23B, (int))(1);
        g_100fc790_guess = 0;
        g_104afc20_guess = 0;
        g_107037e0_guess = 0;
        EQ_CDECL(int, 0x1004A25E, (int))(g_100fbb9c_guess);
    }
    break;
    case 2:
    {
        if (g_100fbbc0_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            EQ_CDECL(int, 0x10048CDF, (int))(g_100fbbc0_guess);
            g_100fbbc0_guess = 0;
        }
        for (node = (EffectsUsedNode_guess *)g_effects_used_list_guess; node != 0; node = node->next)
        {
            if (node->field68 != 0)
            {
                EQ_CDECL(int, 0x10045ED4, (int))((int)node);
                EQ_CDECL(int, 0x10048CDF, (int))(node->field68);
                node->field68 = 0;
            }
        }
        for (node = (EffectsUsedNode_guess *)g_effects_used_list_guess; node != 0; node = node->next)
        {
            if (node->field6c != 0)
            {
                EQ_CDECL(int, 0x10045ED4, (int))((int)node);
                EQ_CDECL(int, 0x10048CDF, (int))(node->field6c);
                node->field6c = 0;
            }
        }
        if (g_100fc78c_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            grFogMode_guess(0);
            grAlphaBlendFunction_guess(1, 5, 0, 0);
            grAlphaCombine_guess(1, 8, 0, 2, 0);
            grDepthMask_guess(0);
            grAlphaTestFunction_guess(7);
            vertexData = g_10703800_guess;
            drawCount = 0;
            if (g_100fc78c_guess > 0U)
            {
                do
                {
                    grDrawLine_guess((int)vertexData, (int)(vertexData + 0x20));
                    drawCount += 2;
                    vertexData += 0x40;
                }
                while (drawCount < g_100fc78c_guess);
            }
            g_100fc78c_guess = 0;
        }
        if (g_1055f8e0_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            grFogMode_guess(0);
            grAlphaBlendFunction_guess(1, 4, 0, 0);
            grAlphaCombine_guess(1, 8, 0, 2, 0);
            grDepthMask_guess(0);
            grAlphaTestFunction_guess(7);
            vertexData = g_10413820_guess;
            drawCount = 0;
            if (g_1055f8e0_guess > 0U)
            {
                do
                {
                    grDrawLine_guess((int)vertexData, (int)(vertexData + 0x20));
                    drawCount += 2;
                    vertexData += 0x40;
                }
                while (drawCount < g_1055f8e0_guess);
            }
            g_1055f8e0_guess = 0;
        }
        if (g_1083c020_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            grFogMode_guess(0);
            grAlphaBlendFunction_guess(1, 5, 0, 0);
            grAlphaCombine_guess(1, 8, 0, 2, 0);
            grDepthMask_guess(0);
            grAlphaTestFunction_guess(7);
            pointData = g_1025c0c0_guess;
            pointCount = 0;
            if (g_1083c020_guess > 0U)
            {
                do
                {
                    grDrawPoint_guess((int)pointData);
                    pointData += 0x20;
                    pointCount++;
                }
                while (pointCount < g_1083c020_guess);
            }
            g_1083c020_guess = 0;
        }
        if (g_10377404_guess != 0)
        {
            EQ_CDECL(int, 0x10045ED4, (int))(0);
            grFogMode_guess(0);
            grAlphaBlendFunction_guess(1, 4, 0, 0);
            grAlphaCombine_guess(1, 8, 0, 2, 0);
            grDepthMask_guess(0);
            grAlphaTestFunction_guess(7);
            pointData = g_1079fc20_guess;
            pointCount = 0;
            if (g_10377404_guess > 0U)
            {
                do
                {
                    grDrawPoint_guess((int)pointData);
                    pointData += 0x20;
                    pointCount++;
                }
                while (pointCount < g_10377404_guess);
            }
            g_10377404_guess = 0;
        }
        grDepthMask_guess(1);
        grAlphaTestFunction_guess(7);
        grAlphaCombine_guess(1, 0, 0, 2, 0);
        grAlphaBlendFunction_guess(4, 0, 0, 0);
        if (g_fog_enabled_guess != 0)
            grFogMode_guess(2);
        g_100fc790_guess = 0;
        g_104afc20_guess = 0;
        g_1054c040_guess = 0;
    }
    break;
    }
    return;
}

// FUNCTION: EQGFX 0x100466fa
// Clips a camera-space line segment in place against the frustum sides (|x|,|y| <= z) and near/far z planes.
int __cdecl t3dClipLineToFrustum_guess(ClipVolume_guess *clipVolume, float *start, float *end)
{
    float distance;
    float otherDistance;
    float intersectionX;
    float intersectionY;
    float intersectionZ;
    float *writePoint;
    int result;

    distance = start[1] + start[2];
    if (distance < EQGFX_ZERO_F_GUESS ||
        start[2] - start[1] < EQGFX_ZERO_F_GUESS ||
        start[0] + start[2] < EQGFX_ZERO_F_GUESS ||
        start[2] - start[0] < EQGFX_ZERO_F_GUESS ||
        start[2] - clipVolume->minimumZ < EQGFX_ZERO_F_GUESS ||
        clipVolume->maximumZ - start[2] < EQGFX_ZERO_F_GUESS ||
        end[1] + end[2] < EQGFX_ZERO_F_GUESS ||
        end[2] - end[1] < EQGFX_ZERO_F_GUESS ||
        end[0] + end[2] < EQGFX_ZERO_F_GUESS ||
        end[2] - end[0] < EQGFX_ZERO_F_GUESS ||
        end[2] - clipVolume->minimumZ < EQGFX_ZERO_F_GUESS ||
        clipVolume->maximumZ - end[2] < EQGFX_ZERO_F_GUESS)
    {
        if ((distance >= EQGFX_ZERO_F_GUESS) || (end[1] + end[2] >= EQGFX_ZERO_F_GUESS))
        {
            if (distance >= EQGFX_ZERO_F_GUESS)
            {
                if (end[1] + end[2] < EQGFX_ZERO_F_GUESS)
                {
                    distance = distance / (distance - (end[1] + end[2]));
                    intersectionZ = (end[2] - start[2]) * distance + start[2];
                    intersectionX = (end[0] - start[0]) * distance + start[0];
                    writePoint = end;
                    goto clip_y_min_write;
                }
            }
            else
            {
                distance = distance / (distance - (end[1] + end[2]));
                intersectionZ = (end[2] - start[2]) * distance + start[2];
                intersectionX = (end[0] - start[0]) * distance + start[0];
                writePoint = start;
clip_y_min_write:
                intersectionY = -intersectionZ;
                writePoint[0] = intersectionX;
                writePoint[1] = intersectionY;
                writePoint[2] = intersectionZ;
            }
            distance = start[2] - start[1];
            if ((distance >= EQGFX_ZERO_F_GUESS) || (end[2] - end[1] >= EQGFX_ZERO_F_GUESS))
            {
                if (distance >= EQGFX_ZERO_F_GUESS)
                {
                    if (end[2] - end[1] < EQGFX_ZERO_F_GUESS)
                    {
                        distance = distance / (distance - (end[2] - end[1]));
                        intersectionZ = (end[2] - start[2]) * distance + start[2];
                        intersectionX = (end[0] - start[0]) * distance + start[0];
                        writePoint = end;
                        goto clip_y_max_write;
                    }
                }
                else
                {
                    distance = distance / (distance - (end[2] - end[1]));
                    intersectionZ = (end[2] - start[2]) * distance + start[2];
                    intersectionX = (end[0] - start[0]) * distance + start[0];
                    writePoint = start;
clip_y_max_write:
                    writePoint[0] = intersectionX;
                    writePoint[1] = intersectionZ;
                    writePoint[2] = intersectionZ;
                }
                distance = start[0] + start[2];
                if ((distance >= EQGFX_ZERO_F_GUESS) || (end[0] + end[2] >= EQGFX_ZERO_F_GUESS))
                {
                    if (distance >= EQGFX_ZERO_F_GUESS)
                    {
                        if (end[0] + end[2] < EQGFX_ZERO_F_GUESS)
                        {
                            distance = distance / (distance - (end[0] + end[2]));
                            intersectionZ = (end[2] - start[2]) * distance + start[2];
                            intersectionY = (end[1] - start[1]) * distance + start[1];
                            writePoint = end;
                            goto clip_x_min_write;
                        }
                    }
                    else
                    {
                        distance = distance / (distance - (end[0] + end[2]));
                        intersectionZ = (end[2] - start[2]) * distance + start[2];
                        intersectionY = (end[1] - start[1]) * distance + start[1];
                        writePoint = start;
clip_x_min_write:
                        intersectionX = -intersectionZ;
                        writePoint[0] = intersectionX;
                        writePoint[1] = intersectionY;
                        writePoint[2] = intersectionZ;
                    }
                    distance = start[2] - start[0];
                    if ((distance >= EQGFX_ZERO_F_GUESS) || (end[2] - end[0] >= EQGFX_ZERO_F_GUESS))
                    {
                        if (distance >= EQGFX_ZERO_F_GUESS)
                        {
                            if (end[2] - end[0] >= EQGFX_ZERO_F_GUESS)
                                goto clip_x_max_done;
                            distance = distance / (distance - (end[2] - end[0]));
                            if ((distance >= EQGFX_ZERO_F_GUESS) && (distance <= EQGFX_FLOAT_ONE))
                            {
                                intersectionZ = (end[2] - start[2]) * distance + start[2];
                                intersectionY = (end[1] - start[1]) * distance + start[1];
                                writePoint = end;
                                goto clip_x_max_write;
                            }
                        }
                        else
                        {
                            distance = distance / (distance - (end[2] - end[0]));
                            if ((distance >= EQGFX_ZERO_F_GUESS) && (distance <= EQGFX_FLOAT_ONE))
                            {
                                intersectionZ = (end[2] - start[2]) * distance + start[2];
                                intersectionY = (end[1] - start[1]) * distance + start[1];
                                writePoint = start;
clip_x_max_write:
                                writePoint[0] = intersectionZ;
                                writePoint[1] = intersectionY;
                                writePoint[2] = intersectionZ;
clip_x_max_done:
                                distance = start[2] - clipVolume->minimumZ;
                                if ((distance >= EQGFX_ZERO_F_GUESS) ||
                                    (end[2] - clipVolume->minimumZ >= EQGFX_ZERO_F_GUESS))
                                {
                                    if (distance >= EQGFX_ZERO_F_GUESS)
                                    {
                                        otherDistance = end[2] - clipVolume->minimumZ;
                                        if (otherDistance < EQGFX_ZERO_F_GUESS)
                                        {
                                            intersectionZ = clipVolume->minimumZ;
                                            distance = distance / (distance - otherDistance);
                                            intersectionX = (end[0] - start[0]) * distance + start[0];
                                            intersectionY = (end[1] - start[1]) * distance + start[1];
                                            writePoint = end;
                                            goto clip_z_min_write;
                                        }
                                    }
                                    else
                                    {
                                        intersectionZ = clipVolume->minimumZ;
                                        distance = distance /
                                            (distance - (end[2] - clipVolume->minimumZ));
                                        intersectionX = (end[0] - start[0]) * distance + start[0];
                                        intersectionY = (end[1] - start[1]) * distance + start[1];
                                        writePoint = start;
clip_z_min_write:
                                        writePoint[0] = intersectionX;
                                        writePoint[1] = intersectionY;
                                        writePoint[2] = intersectionZ;
                                    }
                                    distance = clipVolume->maximumZ - start[2];
                                    if ((distance >= EQGFX_ZERO_F_GUESS) ||
                                        (clipVolume->maximumZ - end[2] >= EQGFX_ZERO_F_GUESS))
                                    {
                                        if (distance >= EQGFX_ZERO_F_GUESS)
                                        {
                                            otherDistance = clipVolume->maximumZ - end[2];
                                            if (otherDistance >= EQGFX_ZERO_F_GUESS)
                                                goto clip_success;
                                            intersectionZ = clipVolume->maximumZ;
                                            distance = distance / (distance - otherDistance);
                                            intersectionX = (end[0] - start[0]) * distance + start[0];
                                            intersectionY = (end[1] - start[1]) * distance + start[1];
                                            start = end;
                                        }
                                        else
                                        {
                                            intersectionZ = clipVolume->maximumZ;
                                            distance = distance /
                                                (distance - (clipVolume->maximumZ - end[2]));
                                            intersectionX = (end[0] - start[0]) * distance + start[0];
                                            intersectionY = (end[1] - start[1]) * distance + start[1];
                                        }
                                        start[0] = intersectionX;
                                        start[1] = intersectionY;
                                        start[2] = intersectionZ;
                                        goto clip_success;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        result = 0;
    }
    else
    {
clip_success:
        result = 1;
    }
    return result;
}

// FUNCTION: EQGFX 0x10046C0B
// Projects one particle-cloud point to screen, gives it the rain color and queues it in a flag-selected buffer.
// __cdecl, 0 when the point is culled or its buffer is full, else 1. Mode 2 emits, mode 1 emits with a fog byte, other modes emit nothing.
int __cdecl t3dDeferParticlePoint_guess(ParticleCloud_guess *cloud, float *position, unsigned char *color, unsigned int flags)
{
    int result;
    unsigned int count;
    ParticleVertex_guess *vertex;
    float halfWidth;
    float halfHeight;
    float baseX;
    float baseY;
    float sizeScale;
    float nearZ;
    float invDepth;
    int fogAlpha;
    unsigned int alphaScale;

    result = 1;
    if (position[1] + position[2] < EQGFX_ZERO_F_GUESS ||
        position[2] - position[1] < EQGFX_ZERO_F_GUESS ||
        position[0] + position[2] < EQGFX_ZERO_F_GUESS ||
        position[2] - position[0] < EQGFX_ZERO_F_GUESS ||
        position[2] - cloud->nearZ < EQGFX_ZERO_F_GUESS ||
        cloud->farZ - position[2] < EQGFX_ZERO_F_GUESS)
        return 0;
    color[2] = (unsigned char)g_rain_red_guess;
    color[1] = (unsigned char)g_rain_green_guess;
    color[0] = (unsigned char)g_rain_blue_guess;
    if (g_render_mode_guess != 1)
    {
        if (g_render_mode_guess != 2)
            goto done;
        halfWidth = cloud->halfWidth;
        halfHeight = cloud->halfHeight;
        baseX = halfWidth + cloud->viewportX;
        sizeScale = cloud->sizeScale;
        baseY = halfHeight + cloud->viewportY;
        nearZ = cloud->nearZ;
        if ((flags & 0x1000000) == 0)
        {
            count = g_106bf200_guess;
            if (count + 1 > 10000)
            {
                result = 0;
                goto done;
            }
            vertex = (ParticleVertex_guess *)(g_104afc40_guess + count * 0x20);
            invDepth = 1.0f / position[2];
            vertex->x = invDepth * position[0] * halfWidth + baseX;
            vertex->y = baseY - invDepth * position[1] * halfHeight;
            vertex->z = sizeScale - invDepth * nearZ * sizeScale;
            vertex->w = invDepth;
            vertex->color = *(unsigned int *)color;
            goto bump_a;
        }
        if ((flags & 0x100000) == 0)
        {
            count = g_1083c020_guess;
            if (count + 1 > 10000)
            {
                result = 0;
                goto done;
            }
            vertex = (ParticleVertex_guess *)(g_1025c0c0_guess + count * 0x20);
            invDepth = 1.0f / position[2];
            vertex->x = invDepth * position[0] * halfWidth + baseX;
            vertex->y = baseY - invDepth * position[1] * halfHeight;
            vertex->z = sizeScale - invDepth * nearZ * sizeScale;
            vertex->w = invDepth;
            vertex->color = *(unsigned int *)color;
            goto bump_b;
        }
        count = g_10377404_guess;
        if (count + 1 > 10000)
        {
            result = 0;
            goto done;
        }
        vertex = (ParticleVertex_guess *)(g_1079fc20_guess + count * 0x20);
        invDepth = 1.0f / position[2];
        vertex->x = invDepth * position[0] * halfWidth + baseX;
        vertex->y = baseY - invDepth * position[1] * halfHeight;
        vertex->z = sizeScale - invDepth * nearZ * sizeScale;
        vertex->w = invDepth;
        vertex->color = *(unsigned int *)color;
        goto bump_c;
    }
    halfWidth = cloud->halfWidth;
    halfHeight = cloud->halfHeight;
    baseX = halfWidth + cloud->viewportX;
    sizeScale = cloud->sizeScale;
    baseY = halfHeight + cloud->viewportY;
    nearZ = cloud->nearZ;
    if ((flags & 0x1000000) == 0)
    {
        count = g_106bf200_guess;
        if (count + 1 > 10000)
        {
            result = 0;
            goto done;
        }
        vertex = (ParticleVertex_guess *)(g_104afc40_guess + count * 0x20);
        invDepth = 1.0f / position[2];
        fogAlpha = (int)((position[2] - g_100fbb94_guess) * g_100fbb98_guess);
        if (fogAlpha > 0xff)
            fogAlpha = 0xff;
        else if (fogAlpha < 0)
            fogAlpha = 0;
        vertex->x = invDepth * position[0] * halfWidth + baseX;
        vertex->y = baseY - invDepth * position[1] * halfHeight;
        vertex->z = sizeScale - invDepth * nearZ * sizeScale;
        vertex->w = invDepth;
        vertex->color = *(unsigned int *)color;
        vertex->fog = (-(fogAlpha + 1)) << 24;
        goto bump_a;
    }
    if ((flags & 0x100000) == 0)
    {
        count = g_1083c020_guess;
        if (count + 1 > 10000)
        {
            result = 0;
            goto done;
        }
        vertex = (ParticleVertex_guess *)(g_1025c0c0_guess + count * 0x20);
        invDepth = 1.0f / position[2];
        fogAlpha = (int)((position[2] - g_100fbb94_guess) * g_100fbb98_guess);
        if (fogAlpha > 0xff)
            fogAlpha = 0xff;
        else if (fogAlpha < 0)
            fogAlpha = 0;
        vertex->x = invDepth * position[0] * halfWidth + baseX;
        vertex->y = baseY - invDepth * position[1] * halfHeight;
        vertex->z = sizeScale - invDepth * nearZ * sizeScale;
        vertex->w = invDepth;
        vertex->color = *(unsigned int *)color;
        vertex->fog = (-(fogAlpha + 1)) << 24;
        goto bump_b;
    }
    count = g_10377404_guess;
    if (count + 1 > 10000)
    {
        result = 0;
        goto done;
    }
    vertex = (ParticleVertex_guess *)(g_1079fc20_guess + count * 0x20);
    invDepth = 1.0f / position[2];
    fogAlpha = (int)((position[2] - g_100fbb94_guess) * g_100fbb98_guess);
    if (fogAlpha > 0xff)
        fogAlpha = 0xff;
    else if (fogAlpha < 0)
        fogAlpha = 0;
    vertex->x = invDepth * position[0] * halfWidth + baseX;
    vertex->y = baseY - invDepth * position[1] * halfHeight;
    vertex->z = sizeScale - invDepth * nearZ * sizeScale;
    vertex->w = invDepth;
    alphaScale = color[3];
    vertex->color = ((((color[2] * alphaScale) & 0xffffff00) | 0xffff0000) << 8) |
                    ((int)(color[0] * alphaScale) >> 8) |
                    ((color[1] * alphaScale) & 0xffffff00);
    vertex->fog = (-(fogAlpha + 1)) << 24;
    goto bump_c;
bump_a:
    g_106bf200_guess++;
    goto done;
bump_b:
    g_1083c020_guess++;
    goto done;
bump_c:
    g_10377404_guess++;
done:
    return result;
}

// FUNCTION: EQGFX 0x100470D1
// __cdecl. Sets the rain color triple by mode (1: storm tint, else grey).
int __cdecl t3dSetRainColor(int rainMode)
{
    if (rainMode != 1)
    {
        g_rain_red_guess = 0x60;
        g_rain_green_guess = 0x60;
        g_rain_blue_guess = 0x60;
        return 1;
    }
    g_rain_red_guess = 0xb4;
    g_rain_green_guess = 1;
    g_rain_blue_guess = 1;
    return 1;
}

// FUNCTION: EQGFX 0x10047106
// Clips a particle-cloud line to the frustum, projects it and queues it shading from the rain color to black.
// __cdecl, returns 1, or 0 when t3dClipLineToFrustum_guess rejects the segment or the vertex buffer is full. Mode 1 also writes fog.
int __cdecl t3dDeferParticleLine_guess(ParticleCloud_guess *cloud, float *startPos, float *endPos, unsigned int flags)
{
    float clipStart[3];
    float clipEnd[3];
    float sumX;
    float sumY;
    int clipResult;
    int result;
    unsigned int alphaFlag;
    unsigned int alpha;
    int fogA;
    int fogB;
    ParticleVertex_guess *vertA;
    ParticleVertex_guess *vertB;

    clipStart[0] = startPos[0];
    clipStart[1] = startPos[1];
    clipStart[2] = startPos[2];
    result = 1;
    clipEnd[0] = endPos[0];
    clipEnd[1] = endPos[1];
    clipEnd[2] = endPos[2];

    clipResult = EQ_CDECL(int, 0x100466FA, (ParticleCloud_guess *, float *, float *))(cloud, clipStart, clipEnd);
    if (clipResult != 1)
        return 0;

    alphaFlag = flags & 0x1000000;
    alpha = 0xff;
    if (alphaFlag != 0)
    {
        if ((flags & 0x100000) == 0)
            alpha = (flags >> 16) & 0xf;
        else
            alpha = ((flags >> 16) & 0xf) + 1;
        alpha = alpha * 0xff >> 4;
    }

    if (g_render_mode_guess != 1)
    {
        if (g_render_mode_guess != 2)
            return result;

        sumX = cloud->halfWidth + cloud->viewportX;
        sumY = cloud->halfHeight + cloud->viewportY;
        if (alphaFlag == 0)
        {
            if (20000 < g_1055f8e4_guess + 2U)
                return 0;
            vertA = (ParticleVertex_guess *)g_10377420_guess + g_1055f8e4_guess;
            vertA->x = clipStart[0] * (1.0 / clipStart[2]) * cloud->halfWidth + sumX;
            vertA->y = sumY - clipStart[1] * (1.0 / clipStart[2]) * cloud->halfHeight;
            vertA->z = cloud->sizeScale - (1.0 / clipStart[2]) * cloud->nearZ * cloud->sizeScale;
            vertA->w = 1.0 / clipStart[2];
            vertA->color = ((alpha << 8 | g_rain_red_guess) << 8 | g_rain_green_guess) << 8 | g_rain_blue_guess;
            g_1055f8e4_guess++;
            vertB = vertA + 1;
            vertB->x = clipEnd[0] * (1.0 / clipEnd[2]) * cloud->halfWidth + sumX;
            vertB->y = sumY - clipEnd[1] * (1.0 / clipEnd[2]) * cloud->halfHeight;
            vertB->z = cloud->sizeScale - (1.0 / clipEnd[2]) * cloud->nearZ * cloud->sizeScale;
            vertB->w = 1.0 / clipEnd[2];
            vertB->color = alpha << 24;
            g_1055f8e4_guess++;
            return result;
        }
        if ((flags & 0x100000) == 0)
        {
            if (20000 < g_100fc78c_guess + 2U)
                return 0;
            vertA = (ParticleVertex_guess *)g_10703800_guess + g_100fc78c_guess;
            vertA->x = clipStart[0] * (1.0 / clipStart[2]) * cloud->halfWidth + sumX;
            vertA->y = sumY - clipStart[1] * (1.0 / clipStart[2]) * cloud->halfHeight;
            vertA->z = cloud->sizeScale - (1.0 / clipStart[2]) * cloud->nearZ * cloud->sizeScale;
            vertA->w = 1.0 / clipStart[2];
            vertA->color = ((alpha << 8 | g_rain_red_guess) << 8 | g_rain_green_guess) << 8 | g_rain_blue_guess;
            g_100fc78c_guess++;
            vertB = vertA + 1;
            vertB->x = clipEnd[0] * (1.0 / clipEnd[2]) * cloud->halfWidth + sumX;
            vertB->y = sumY - clipEnd[1] * (1.0 / clipEnd[2]) * cloud->halfHeight;
            vertB->z = cloud->sizeScale - (1.0 / clipEnd[2]) * cloud->nearZ * cloud->sizeScale;
            vertB->w = 1.0 / clipEnd[2];
            vertB->color = alpha << 24;
            g_100fc78c_guess++;
            return result;
        }
        if (20000 < g_1055f8e0_guess + 2U)
            return 0;
        vertA = (ParticleVertex_guess *)g_10413820_guess + g_1055f8e0_guess;
        vertA->x = clipStart[0] * (1.0 / clipStart[2]) * cloud->halfWidth + sumX;
        vertA->y = sumY - clipStart[1] * (1.0 / clipStart[2]) * cloud->halfHeight;
        vertA->z = cloud->sizeScale - (1.0 / clipStart[2]) * cloud->nearZ * cloud->sizeScale;
        vertA->w = 1.0 / clipStart[2];
        vertA->color = ((alpha << 8 | g_rain_red_guess) << 8 | g_rain_green_guess) << 8 | g_rain_blue_guess;
        g_1055f8e0_guess++;
        vertB = vertA + 1;
        vertB->x = clipEnd[0] * (1.0 / clipEnd[2]) * cloud->halfWidth + sumX;
        vertB->y = sumY - clipEnd[1] * (1.0 / clipEnd[2]) * cloud->halfHeight;
        vertB->z = cloud->sizeScale - (1.0 / clipEnd[2]) * cloud->nearZ * cloud->sizeScale;
        vertB->w = 1.0 / clipEnd[2];
        vertB->color = alpha << 24;
        g_1055f8e0_guess++;
        return result;
    }

    sumX = cloud->halfWidth + cloud->viewportX;
    sumY = cloud->halfHeight + cloud->viewportY;
    if (alphaFlag == 0)
    {
        if (20000 < g_1055f8e4_guess + 2U)
            return 0;
        fogA = (int)((clipStart[2] - g_100fbb94_guess) * g_100fbb98_guess);
        if (fogA < 0)
            fogA = 0;
        else if (fogA > 0xff)
            fogA = 0xff;
        vertA = (ParticleVertex_guess *)g_10377420_guess + g_1055f8e4_guess;
        vertA->x = clipStart[0] * (1.0 / clipStart[2]) * cloud->halfWidth + sumX;
        vertA->y = sumY - clipStart[1] * (1.0 / clipStart[2]) * cloud->halfHeight;
        vertA->z = cloud->sizeScale - (1.0 / clipStart[2]) * cloud->nearZ * cloud->sizeScale;
        vertA->w = 1.0 / clipStart[2];
        vertA->color = ((alpha << 8 | g_rain_red_guess) << 8 | g_rain_green_guess) << 8 | g_rain_blue_guess;
        vertA->fog = (fogA + 1) * -0x1000000;
        g_1055f8e4_guess++;
        fogB = (int)((clipEnd[2] - g_100fbb94_guess) * g_100fbb98_guess);
        if (fogB < 0)
            fogB = 0;
        else if (fogB > 0xff)
            fogB = 0xff;
        vertB = vertA + 1;
        vertB->x = clipEnd[0] * (1.0 / clipEnd[2]) * cloud->halfWidth + sumX;
        vertB->y = sumY - clipEnd[1] * (1.0 / clipEnd[2]) * cloud->halfHeight;
        vertB->z = cloud->sizeScale - (1.0 / clipEnd[2]) * cloud->nearZ * cloud->sizeScale;
        vertB->w = 1.0 / clipEnd[2];
        vertB->color = alpha << 24;
        vertB->fog = (fogB + 1) * -0x1000000;
        g_1055f8e4_guess++;
        return result;
    }
    if ((flags & 0x100000) == 0)
    {
        if (20000 < g_100fc78c_guess + 2U)
            return 0;
        fogA = (int)((clipStart[2] - g_100fbb94_guess) * g_100fbb98_guess);
        if (fogA < 0)
            fogA = 0;
        else if (fogA > 0xff)
            fogA = 0xff;
        vertA = (ParticleVertex_guess *)g_10703800_guess + g_100fc78c_guess;
        vertA->x = clipStart[0] * (1.0 / clipStart[2]) * cloud->halfWidth + sumX;
        vertA->y = sumY - clipStart[1] * (1.0 / clipStart[2]) * cloud->halfHeight;
        vertA->z = cloud->sizeScale - (1.0 / clipStart[2]) * cloud->nearZ * cloud->sizeScale;
        vertA->w = 1.0 / clipStart[2];
        vertA->color = ((alpha << 8 | g_rain_red_guess) << 8 | g_rain_green_guess) << 8 | g_rain_blue_guess;
        vertA->fog = (fogA + 1) * -0x1000000;
        g_100fc78c_guess++;
        fogB = (int)((clipEnd[2] - g_100fbb94_guess) * g_100fbb98_guess);
        if (fogB < 0)
            fogB = 0;
        else if (fogB > 0xff)
            fogB = 0xff;
        vertB = vertA + 1;
        vertB->x = clipEnd[0] * (1.0 / clipEnd[2]) * cloud->halfWidth + sumX;
        vertB->y = sumY - clipEnd[1] * (1.0 / clipEnd[2]) * cloud->halfHeight;
        vertB->z = cloud->sizeScale - (1.0 / clipEnd[2]) * cloud->nearZ * cloud->sizeScale;
        vertB->w = 1.0 / clipEnd[2];
        vertB->color = alpha << 24;
        vertB->fog = (fogB + 1) * -0x1000000;
        g_100fc78c_guess++;
        return result;
    }
    if (20000 < g_1055f8e0_guess + 2U)
        return 0;
    fogA = (int)((clipStart[2] - g_100fbb94_guess) * g_100fbb98_guess);
    if (fogA < 0)
        fogA = 0;
    else if (fogA > 0xff)
        fogA = 0xff;
    vertA = (ParticleVertex_guess *)g_10413820_guess + g_1055f8e0_guess;
    vertA->x = clipStart[0] * (1.0 / clipStart[2]) * cloud->halfWidth + sumX;
    vertA->y = sumY - clipStart[1] * (1.0 / clipStart[2]) * cloud->halfHeight;
    vertA->z = cloud->sizeScale - (1.0 / clipStart[2]) * cloud->nearZ * cloud->sizeScale;
    vertA->w = 1.0 / clipStart[2];
    vertA->color = (g_rain_red_guess * alpha & 0xffffff00 | 0xffff0000) << 8 |
                   (int)(g_rain_blue_guess * alpha) >> 8 | g_rain_green_guess * alpha & 0xffffff00;
    vertA->fog = (fogA + 1) * -0x1000000;
    g_1055f8e0_guess++;
    fogB = (int)((clipEnd[2] - g_100fbb94_guess) * g_100fbb98_guess);
    if (fogB < 0)
        fogB = 0;
    else if (fogB > 0xff)
        fogB = 0xff;
    vertB = vertA + 1;
    vertB->x = clipEnd[0] * (1.0 / clipEnd[2]) * cloud->halfWidth + sumX;
    vertB->y = sumY - clipEnd[1] * (1.0 / clipEnd[2]) * cloud->halfHeight;
    vertB->z = cloud->sizeScale - (1.0 / clipEnd[2]) * cloud->nearZ * cloud->sizeScale;
    vertB->w = 1.0 / clipEnd[2];
    vertB->color = 0xff000000;
    vertB->fog = (fogB + 1) * -0x1000000;
    g_1055f8e0_guess++;
    return result;
}

// FUNCTION: EQGFX 0x100477B9
// Queues one lit, frustum-clipped effect polygon as fan triangles on the opaque or blended per-texture draw lists.
// __cdecl: lights and transforms a mesh, projects it into the mode-1/mode-2 (g_render_mode_guess) vertex buffers and queues
// its fan triangles. Returns 1, 0 when there is nothing to draw or the buffers stay full, -1 when a step fails.
int __cdecl t3dDeferPolygon(ParticleCloud_guess *context, LitMesh_guess *mesh, int count, Material_guess *material,
                         unsigned int flags)
{
    int textured;
    unsigned int blendFlag;
    int modeTwo;
    unsigned int noLighting;
    int result;
    EffectsUsedNode_guess *texture;
    unsigned int translucent;
    unsigned int alphaRoundUp;
    int tinted;
    int lit;
    unsigned int specular;
    int red;
    int green;
    int blue;
    SceneLight_guess **lightCursor;
    SceneLight_guess **lightEnd;
    SceneLight_guess *light;
    LightAnimation_guess *animation;
    LightFrames_guess *frames;
    LightColor_guess color;
    unsigned int interval;
    unsigned int elapsed;
    unsigned int steps;
    float dot;
    int addRed;
    int addGreen;
    int addBlue;
    int status;
    int source;
    LitVertex_guess *verts;
    LitVertex_guess *vertex;
    LitVertex_guess *end;
    float halfWidth;
    float halfHeight;
    float sumX;
    float sumY;
    float nearZ;
    float sizeScale;
    float uScale;
    float vScale;
    float w;
    unsigned int level;
    int alpha;
    int fog;
    unsigned int primFlags;
    int triangles;
    int i;
    ParticleVertex_guess *out;
    PrimNode_guess *node;

    textured = (flags & 0xff00) != 0;
    blendFlag = flags & 0x80;
    modeTwo = ((unsigned char)flags & 3) == 2;
    noLighting = flags & 0x40000000;
    result = 1;
    texture = 0;
    switch (material->type)
    {
    case 0:
        translucent = flags & 0x1000000;
        if (translucent != 0)
            alphaRoundUp = flags & 0x100000;
        else
            alphaRoundUp = 0;
        tinted = 0;
        break;
    case 1:
        translucent = 0;
        alphaRoundUp = 0;
        if (material->flags & 1)
            tinted = 1;
        break;
    case 2:
        translucent = 1;
        alphaRoundUp = 0;
        if (material->flags & 1)
            tinted = 1;
        break;
    case 3:
        translucent = 1;
        alphaRoundUp = 1;
        if (material->flags & 1)
            tinted = 1;
        break;
    }
    if (noLighting == 0)
    {
        lit = 1;
        if (flags & 0x40)
            specular = flags & 0x20;
        else
            specular = 0;
        switch (flags & 0x1c)
        {
        case 0:
            red = 0;
            green = 0;
            blue = 0;
            break;
        case 4:
            red = 0xff;
            green = 0xff;
            blue = 0xff;
            break;
        case 8:
            red = (int)(material->brightness * 255.0f);
            green = red;
            blue = red;
            break;
        case 0x10:
            red = (int)(context->ambient[0] * 255.0f);
            green = (int)(context->ambient[1] * 255.0f);
            blue = (int)(context->ambient[2] * 255.0f);
            break;
        case 0x14:
            red = (int)(context->ambient[0] * material->ambientScale * 255.0f);
            green = (int)(context->ambient[1] * material->ambientScale * 255.0f);
            blue = (int)(context->ambient[2] * material->ambientScale * 255.0f);
            break;
        }
        if (flags & 0x200000)
        {
            if (flags & 0x20)
            {
                lit = 1;
            }
            else
            {
                lightCursor = context->lights;
                lightEnd = lightCursor + context->lightCount;
                for (; lightCursor < lightEnd; lightCursor++)
                {
                    light = *lightCursor;
                    if (light->type != 0x1f)
                        break;
                    animation = light->animation;
                    frames = animation->frames;
                    if (frames->count > 1)
                    {
                        if (animation->nextClock < context->clock && (animation->control & 8) == 0)
                        {
                            if (animation->mode != 0)
                            {
                                interval = animation->interval;
                                elapsed = context->clock - animation->nextClock;
                                if (elapsed > interval << 4)
                                {
                                    steps = elapsed / interval;
                                    animation->frameIndex += steps + 1;
                                    animation->frameIndex %= frames->count;
                                    elapsed -= steps * interval;
                                }
                                else
                                {
                                    for (; elapsed > interval; elapsed -= interval)
                                    {
                                        animation->frameIndex++;
                                        if (animation->frameIndex >= frames->count)
                                            animation->frameIndex = 0;
                                    }
                                    animation->frameIndex++;
                                    if (animation->frameIndex >= frames->count)
                                        animation->frameIndex = 0;
                                }
                                animation->nextClock = context->clock - elapsed + interval;
                            }
                            else
                            {
                                animation->frameIndex++;
                                if (animation->frameIndex >= frames->count)
                                    animation->frameIndex = 0;
                                animation->nextClock = animation->interval + context->clock;
                            }
                        }
                        if (frames->colors != 0)
                        {
                            color = frames->colors[animation->frameIndex];
                        }
                        else
                        {
                            color.red = frames->level.values[animation->frameIndex];
                            color.green = color.red;
                            color.blue = color.red;
                        }
                    }
                    else if (frames->colors != 0)
                    {
                        color = *frames->colors;
                    }
                    else
                    {
                        color.red = frames->level.value;
                        color.green = color.red;
                        color.blue = color.red;
                    }
                    dot = -(context->direction[2] * light->direction[2] + context->direction[1] * light->direction[1] +
                            context->direction[0] * light->direction[0]);
                    if (dot > 0.0f)
                    {
                        EQ_CDECL(void, 0x1004EC8D, (float, int *))(dot * color.red * 255.0f, &addRed);
                        EQ_CDECL(void, 0x1004EC8D, (float, int *))(dot * color.green * 255.0f, &addGreen);
                        EQ_CDECL(void, 0x1004EC8D, (float, int *))(dot * color.blue * 255.0f, &addBlue);
                        red += addRed;
                        green += addGreen;
                        blue += addBlue;
                    }
                }
            }
        }
    }
    else
    {
        lit = 0;
        specular = 0;
    }

    if (textured != 0)
    {
        if (lit != 0)
        {
            if (mesh != 0)
            {
                if (specular == 0)
                    status = EQ_CDECL(int, 0x10033578, (ParticleCloud_guess *, int, int, LitMesh_guess *, int, int, int, int))(
                        context, context->field27c, mesh->field0c, mesh, material->field44, red, green, blue);
                else
                    status = EQ_CDECL(int, 0x100338B2, (ParticleCloud_guess *, int, int, LitMesh_guess *, int, int, int, int))(
                        context, context->field27c, mesh->field0c, mesh, material->field44, red, green, blue);
                if (status < 0)
                    goto failed;
                source = mesh->count;
            }
            else
            {
                source = count;
                if (specular == 0)
                    status = EQ_CDECL(int, 0x10033BE5, (ParticleCloud_guess *, int, float *, int, int, int))(
                        context, count, context->direction, red, green, blue);
                else
                    status = EQ_CDECL(int, 0x10033EBC, (ParticleCloud_guess *, int, int, int, int, unsigned int))(
                        context, count, red, green, blue, flags);
                if (status < 0)
                    goto failed;
            }
            count = EQ_CDECL(int, 0x100309C6, (ParticleCloud_guess *, int, LitVertex_guess **))(context, source, &verts);
            if (count < 0)
                goto failed;
            if (context->extraLight[0] > 0.0f || context->extraLight[1] > 0.0f || context->extraLight[2] > 0.0f)
            {
                if (specular == 0)
                    status = EQ_CDECL(int, 0x10034302, (ParticleCloud_guess *, LitVertex_guess *, int, float *))(
                        context, verts, count, context->direction);
                else
                    status = EQ_CDECL(int, 0x10034475, (ParticleCloud_guess *, LitVertex_guess *, int))(
                        context, verts, count);
                if (status < 0)
                    goto failed;
            }
        }
        else if (noLighting != 0)
        {
            count = EQ_CDECL(int, 0x100309C6, (ParticleCloud_guess *, int, LitVertex_guess **))(context, count, &verts);
            if (count < 0)
                goto failed;
        }
        else
        {
            if (mesh != 0)
            {
                status = EQ_CDECL(int, 0x1002FE9A, (ParticleCloud_guess *, int, LitMesh_guess *, int))(
                    context, context->field27c, mesh, material->field44);
                if (status < 0)
                    goto failed;
                source = mesh->count;
            }
            else
            {
                source = count;
                status = EQ_CDECL(int, 0x1002FE5C, (ParticleCloud_guess *, int))(context, count);
                if (status < 0)
                    goto failed;
            }
            count = EQ_CDECL(int, 0x1002FEF7, (ParticleCloud_guess *, int, LitVertex_guess **))(context, source, &verts);
            if (count < 0)
                goto failed;
        }
        if (count == 0)
            goto rejected;
        texture = material->texture;
        if (texture->type == 6)
            texture = *texture->frames;
        else if (texture->type != 0x1b)
            goto failed;
    }
    else
    {
        if (lit != 0)
        {
            if (mesh != 0)
            {
                if (specular == 0)
                    status = EQ_CDECL(int, 0x10032828, (ParticleCloud_guess *, int, int, LitMesh_guess *, int, int, int))(
                        context, context->field27c, mesh->field0c, mesh, red, green, blue);
                else
                    status = EQ_CDECL(int, 0x10032B51, (ParticleCloud_guess *, int, int, LitMesh_guess *, int, int, int))(
                        context, context->field27c, mesh->field0c, mesh, red, green, blue);
                if (status < 0)
                    goto failed;
                source = mesh->count;
            }
            else
            {
                source = count;
                if (specular == 0)
                    status = EQ_CDECL(int, 0x10032E73, (ParticleCloud_guess *, int, float *, int, int, int))(
                        context, count, context->direction, red, green, blue);
                else
                    status = EQ_CDECL(int, 0x1003313E, (ParticleCloud_guess *, int, int, int, int, unsigned int))(
                        context, count, red, green, blue, flags);
                if (status < 0)
                    goto failed;
            }
            count = EQ_CDECL(int, 0x10030620, (ParticleCloud_guess *, int, LitVertex_guess **))(context, source, &verts);
            if (count < 0)
                goto failed;
            if (context->extraLight[0] > 0.0f || context->extraLight[1] > 0.0f || context->extraLight[2] > 0.0f)
            {
                if (specular == 0)
                    status = EQ_CDECL(int, 0x10034302, (ParticleCloud_guess *, LitVertex_guess *, int, float *))(
                        context, verts, count, context->direction);
                else
                    status = EQ_CDECL(int, 0x10034475, (ParticleCloud_guess *, LitVertex_guess *, int))(
                        context, verts, count);
                if (status < 0)
                    goto failed;
            }
        }
        else if (noLighting != 0)
        {
            count = EQ_CDECL(int, 0x10030620, (ParticleCloud_guess *, int, LitVertex_guess **))(context, count, &verts);
            if (count < 0)
                goto failed;
        }
        else
        {
            source = count;
            if (mesh != 0)
            {
                status = EQ_CDECL(int, 0x1002FE13, (ParticleCloud_guess *, int, LitMesh_guess *))(
                    context, context->field27c, mesh);
                if (status < 0)
                    goto failed;
                source = mesh->count;
            }
            count = EQ_CDECL(int, 0x10030620, (ParticleCloud_guess *, int, LitVertex_guess **))(context, source, &verts);
            if (count < 0)
                goto failed;
        }
        if (count == 0)
            goto rejected;
    }

    halfWidth = context->halfWidth;
    halfHeight = context->halfHeight;
    end = verts + count;
    if (tinted != 0)
    {
        for (vertex = verts; vertex < end; vertex++)
        {
            vertex->red = (material->tintRed * vertex->red) >> 8;
            vertex->green = (material->tintGreen * vertex->green) >> 8;
            vertex->blue = (material->tintBlue * vertex->blue) >> 8;
        }
    }
    // types 1 and above 3 leave alpha in the material argument's stack slot: the original reads the pointer.
    alpha = (int)material;
    if (material->type == 0)
    {
        if (translucent == 0)
        {
            alpha = 0xff;
        }
        else
        {
            if (alphaRoundUp != 0)
                level = (flags >> 16 & 0xf) + 1;
            else
                level = flags >> 16 & 0xf;
            alpha = level * 0xff >> 4;
        }
    }
    else if (material->type > 1 && material->type <= 3)
    {
        alpha = material->alpha;
    }

    switch (g_render_mode_guess)
    {
    case 1:
        sumX = halfWidth + context->viewportX;
        sizeScale = context->sizeScale;
        nearZ = context->nearZ;
        primFlags = 0;
        sumY = halfHeight + context->viewportY;
        if (translucent == 0 && blendFlag == 0)
        {
            if (count + g_100fc788_guess - 2U > 10000)
            {
                EQ_CDECL(void, 0x100460CB, (void))();
                if (count + g_100fc788_guess - 2U > 10000)
                    goto rejected;
            }
            if (count * 3 + g_1025c0a0_guess - 6U > 40000)
            {
                EQ_CDECL(void, 0x100460CB, (void))();
                if (count * 3 + g_1025c0a0_guess - 6U > 40000)
                    goto rejected;
            }
            if (textured != 0)
            {
                out = (ParticleVertex_guess *)g_101238a0_guess + g_1079fc00_guess;
                for (vertex = verts; vertex < end; vertex++, out++)
                {
                    w = 1.0f / vertex->z;
                    fog = (int)((vertex->z - g_100fbb94_guess) * g_100fbb98_guess);
                    if (fog < 0)
                        fog = 0;
                    else if (fog > 0xff)
                        fog = 0xff;
                    out->x = w * vertex->x * halfWidth + sumX;
                    out->y = sumY - w * vertex->y * halfHeight;
                    out->z = sizeScale - w * nearZ * sizeScale;
                    out->w = w;
                    out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
                    out->fog = (fog + 1) * -0x1000000;
                    out->u = vertex->u;
                    out->v = vertex->v;
                }
            }
            else
            {
                out = (ParticleVertex_guess *)g_101238a0_guess + g_1079fc00_guess;
                for (vertex = verts; vertex < end; vertex++, out++)
                {
                    w = 1.0f / vertex->z;
                    fog = (int)((vertex->z - g_100fbb94_guess) * g_100fbb98_guess);
                    if (fog < 0)
                        fog = 0;
                    else if (fog > 0xff)
                        fog = 0xff;
                    out->x = w * vertex->x * halfWidth + sumX;
                    out->y = sumY - w * vertex->y * halfHeight;
                    out->z = sizeScale - w * nearZ * sizeScale;
                    out->w = w;
                    out->fog = (fog + 1) * -0x1000000;
                    out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
                }
            }
            i = 0;
            if (modeTwo != 0)
                primFlags = 1;
            triangles = count - 2;
            for (; i < triangles; i++)
            {
                node = (PrimNode_guess *)g_10332e40_guess + g_100fc788_guess;
                g_100fc788_guess++;
                node->cond = primFlags;
                node->primType = 3;
                node->verts = (short *)g_1083c040_guess + g_1025c0a0_guess;
                ((short *)node->verts)[0] = (short)g_1079fc00_guess;
                ((short *)node->verts)[1] = (short)(i + g_1079fc00_guess + 1);
                ((short *)node->verts)[2] = (short)(i + g_1079fc00_guess + 2);
                g_1025c0a0_guess += 3;
                if (textured != 0)
                {
                    node->next = (PrimNode_guess *)texture->field64;
                    texture->field64 = (int)node;
                    EQ_CDECL(void, 0x1004609A, (EffectsUsedNode_guess *))(texture);
                }
                else
                {
                    node->next = (PrimNode_guess *)g_100fbbbc_guess;
                    g_100fbbbc_guess = (int)node;
                }
            }
            g_1079fc00_guess += count;
            break;
        }
        if (count + g_100fc790_guess - 2U >= 10000)
        {
            EQ_CDECL(void, 0x10046165, (void))();
            if (count + g_100fc790_guess - 2U >= 10000)
                goto rejected;
        }
        if (count + g_104afc20_guess - 2U > 40000)
        {
            EQ_CDECL(void, 0x10046165, (void))();
            if (count + g_104afc20_guess - 2U > 40000)
                goto rejected;
        }
        if (textured != 0)
        {
            if (alphaRoundUp != 0 && g_100fbba4_guess != 3)
            {
                out = (ParticleVertex_guess *)g_1055f900_guess + g_104afc20_guess;
                for (vertex = verts; vertex < end; vertex++, out++)
                {
                    w = 1.0f / vertex->z;
                    fog = (int)((vertex->z - g_100fbb94_guess) * g_100fbb98_guess);
                    if (fog < 0)
                        fog = 0;
                    else if (fog > 0xff)
                        fog = 0xff;
                    out->x = w * vertex->x * halfWidth + sumX;
                    out->y = sumY - w * vertex->y * halfHeight;
                    out->z = sizeScale - w * nearZ * sizeScale;
                    out->w = w;
                    out->color = (vertex->red * alpha & 0xffffff00 | 0xffff0000) << 8 | (vertex->blue * alpha) >> 8 |
                                 vertex->green * alpha & 0xffffff00;
                    out->fog = (fog + 1) * -0x1000000;
                    out->u = vertex->u;
                    out->v = vertex->v;
                }
            }
            else
            {
                out = (ParticleVertex_guess *)g_1055f900_guess + g_104afc20_guess;
                for (vertex = verts; vertex < end; vertex++, out++)
                {
                    w = 1.0f / vertex->z;
                    fog = (int)((vertex->z - g_100fbb94_guess) * g_100fbb98_guess);
                    if (fog < 0)
                        fog = 0;
                    else if (fog > 0xff)
                        fog = 0xff;
                    out->x = w * vertex->x * halfWidth + sumX;
                    out->y = sumY - w * vertex->y * halfHeight;
                    out->z = sizeScale - w * nearZ * sizeScale;
                    out->w = w;
                    out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
                    out->fog = (fog + 1) * -0x1000000;
                    out->u = vertex->u;
                    out->v = vertex->v;
                }
            }
        }
        else if (alphaRoundUp != 0 && g_100fbba4_guess != 3)
        {
            out = (ParticleVertex_guess *)g_1055f900_guess + g_104afc20_guess;
            for (vertex = verts; vertex < end; vertex++, out++)
            {
                w = 1.0f / vertex->z;
                fog = (int)((vertex->z - g_100fbb94_guess) * g_100fbb98_guess);
                if (fog < 0)
                    fog = 0;
                else if (fog > 0xff)
                    fog = 0xff;
                out->x = w * vertex->x * halfWidth + sumX;
                out->y = sumY - w * vertex->y * halfHeight;
                out->z = sizeScale - w * nearZ * sizeScale;
                out->w = w;
                out->color = (vertex->red * alpha & 0xffffff00 | 0xffff0000) << 8 | (vertex->blue * alpha) >> 8 |
                             vertex->green * alpha & 0xffffff00;
                out->fog = (fog + 1) * -0x1000000;
            }
        }
        else
        {
            out = (ParticleVertex_guess *)g_1055f900_guess + g_104afc20_guess;
            for (vertex = verts; vertex < end; vertex++, out++)
            {
                w = 1.0f / vertex->z;
                fog = (int)((vertex->z - g_100fbb94_guess) * g_100fbb98_guess);
                if (fog < 0)
                    fog = 0;
                else if (fog > 0xff)
                    fog = 0xff;
                out->x = w * vertex->x * halfWidth + sumX;
                out->y = sumY - w * vertex->y * halfHeight;
                out->z = sizeScale - w * nearZ * sizeScale;
                out->w = w;
                out->fog = (fog + 1) * -0x1000000;
                out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
            }
        }
        i = 0;
        if (modeTwo != 0)
            primFlags = 1;
        if (translucent != 0)
        {
            if (alphaRoundUp != 0)
                primFlags |= 4;
            else
                primFlags |= 2;
        }
        if (blendFlag != 0)
            primFlags |= 8;
        triangles = count - 2;
        for (; i < triangles; i++)
        {
            node = (PrimNode_guess *)g_106bf220_guess + g_100fc790_guess;
            g_100fc790_guess++;
            node->cond = primFlags;
            node->primType = 3;
            node->verts = (short *)g_1054c060_guess + g_107037e0_guess;
            ((short *)node->verts)[0] = (short)g_104afc20_guess;
            ((short *)node->verts)[1] = (short)(i + g_104afc20_guess + 1);
            ((short *)node->verts)[2] = (short)(i + g_104afc20_guess + 2);
            g_107037e0_guess += 3;
            if (textured != 0)
            {
                if (primFlags == 8)
                {
                    node->texture = texture;
                    node->next = (PrimNode_guess *)texture->field68;
                    texture->field68 = (int)node;
                }
                else
                {
                    node->texture = texture;
                    node->next = (PrimNode_guess *)texture->field6c;
                    texture->field6c = (int)node;
                }
                EQ_CDECL(void, 0x1004609A, (EffectsUsedNode_guess *))(texture);
            }
            else
            {
                node->texture = 0;
                node->next = (PrimNode_guess *)g_100fbbc0_guess;
                g_100fbbc0_guess = (int)node;
            }
        }
        g_104afc20_guess += count;
        break;
    case 2:
        sumX = halfWidth + context->viewportX;
        primFlags = 0;
        sumY = halfHeight + context->viewportY;
        if (translucent == 0 && blendFlag == 0)
        {
            if (count + g_100fc788_guess - 2U > 10000)
            {
                EQ_CDECL(void, 0x100460CB, (void))();
                if (count + g_100fc788_guess - 2U > 10000)
                    goto rejected;
            }
            if (count * 3 + g_10377400_guess - 6U > 40000)
            {
                EQ_CDECL(void, 0x100460CB, (void))();
                if (count * 3 + g_10377400_guess - 6U > 40000)
                    goto rejected;
            }
            if (textured != 0)
            {
                uScale = texture->uScale;
                vScale = texture->vScale;
                out = (ParticleVertex_guess *)g_101238a0_guess + g_1079fc00_guess;
                for (vertex = verts; vertex < end; vertex++, out++)
                {
                    w = 1.0f / vertex->z;
                    out->x = w * vertex->x * halfWidth + sumX;
                    out->y = sumY - w * vertex->y * halfHeight;
                    out->w = w;
                    out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
                    out->u = uScale * vertex->u * w;
                    out->v = vScale * vertex->v * w;
                }
            }
            else
            {
                out = (ParticleVertex_guess *)g_101238a0_guess + g_1079fc00_guess;
                for (vertex = verts; vertex < end; vertex++, out++)
                {
                    w = 1.0f / vertex->z;
                    out->x = w * vertex->x * halfWidth + sumX;
                    out->y = sumY - w * vertex->y * halfHeight;
                    out->w = w;
                    out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
                }
            }
            i = 0;
            if (modeTwo != 0)
                primFlags = 1;
            triangles = count - 2;
            for (; i < triangles; i++)
            {
                node = (PrimNode_guess *)g_10332e40_guess + g_100fc788_guess;
                g_100fc788_guess++;
                node->cond = primFlags;
                node->primType = 3;
                node->verts = (ParticleVertex_guess **)g_100fc7a0_guess + g_10377400_guess;
                ((ParticleVertex_guess **)node->verts)[0] = (ParticleVertex_guess *)g_101238a0_guess + g_1079fc00_guess;
                ((ParticleVertex_guess **)node->verts)[1] =
                    (ParticleVertex_guess *)g_101238a0_guess + (g_1079fc00_guess + i) + 1;
                ((ParticleVertex_guess **)node->verts)[2] =
                    (ParticleVertex_guess *)g_101238a0_guess + (g_1079fc00_guess + i) + 2;
                g_10377400_guess += 3;
                if (textured != 0)
                {
                    node->next = (PrimNode_guess *)texture->field64;
                    texture->field64 = (int)node;
                    EQ_CDECL(void, 0x1004609A, (EffectsUsedNode_guess *))(texture);
                }
                else
                {
                    node->next = (PrimNode_guess *)g_100fbbbc_guess;
                    g_100fbbbc_guess = (int)node;
                }
            }
            g_1079fc00_guess += count;
            break;
        }
        if (count + g_100fc790_guess - 2U >= 10000)
        {
            EQ_CDECL(void, 0x10046165, (void))();
            if (count + g_100fc790_guess - 2U >= 10000)
                goto rejected;
        }
        if (count + g_1054c040_guess - 2U > 40000)
        {
            EQ_CDECL(void, 0x10046165, (void))();
            if (count + g_1054c040_guess - 2U > 40000)
                goto rejected;
        }
        if (textured != 0)
        {
            uScale = texture->uScale;
            vScale = texture->vScale;
            out = (ParticleVertex_guess *)g_1055f900_guess + g_104afc20_guess;
            for (vertex = verts; vertex < end; vertex++, out++)
            {
                w = 1.0f / vertex->z;
                out->x = w * vertex->x * halfWidth + sumX;
                out->y = sumY - w * vertex->y * halfHeight;
                out->w = w;
                out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
                out->u = uScale * vertex->u * w;
                out->v = vScale * vertex->v * w;
            }
        }
        else
        {
            out = (ParticleVertex_guess *)g_1055f900_guess + g_104afc20_guess;
            for (vertex = verts; vertex < end; vertex++, out++)
            {
                w = 1.0f / vertex->z;
                out->x = w * vertex->x * halfWidth + sumX;
                out->y = sumY - w * halfHeight * vertex->y;
                out->w = w;
                out->color = ((vertex->red | alpha << 8) << 8 | vertex->green) << 8 | vertex->blue;
            }
        }
        i = 0;
        if (modeTwo != 0)
            primFlags = 1;
        if (translucent != 0)
        {
            if (alphaRoundUp != 0)
                primFlags |= 4;
            else
                primFlags |= 2;
        }
        if (blendFlag != 0)
            primFlags |= 8;
        triangles = count - 2;
        for (; i < triangles; i++)
        {
            node = (PrimNode_guess *)g_106bf220_guess + g_100fc790_guess;
            g_100fc790_guess++;
            node->cond = primFlags;
            node->primType = 3;
            node->verts = (ParticleVertex_guess **)g_1030bd40_guess + g_1054c040_guess;
            ((ParticleVertex_guess **)node->verts)[0] = (ParticleVertex_guess *)g_1055f900_guess + g_104afc20_guess;
            ((ParticleVertex_guess **)node->verts)[1] =
                (ParticleVertex_guess *)g_1055f900_guess + (g_104afc20_guess + i) + 1;
            ((ParticleVertex_guess **)node->verts)[2] =
                (ParticleVertex_guess *)g_1055f900_guess + (g_104afc20_guess + i) + 2;
            g_1054c040_guess += 3;
            if (textured != 0)
            {
                if (primFlags == 8)
                {
                    node->texture = texture;
                    node->next = (PrimNode_guess *)texture->field68;
                    texture->field68 = (int)node;
                }
                else
                {
                    node->texture = texture;
                    node->next = (PrimNode_guess *)texture->field6c;
                    texture->field6c = (int)node;
                }
                EQ_CDECL(void, 0x1004609A, (EffectsUsedNode_guess *))(texture);
            }
            else
            {
                node->texture = 0;
                node->next = (PrimNode_guess *)g_100fbbc0_guess;
                g_100fbbc0_guess = (int)node;
            }
        }
        g_104afc20_guess += count;
        break;
    }
    return result;

rejected:
    return 0;

failed:
    return -1;
}

// FUNCTION: EQGFX 0x10048B01
// __cdecl, returns 0. Batches one prim list two ways by the mode in g_100fb8a8 (1: staged emit, 2: direct lines).
int __cdecl emitPrimitiveList_guess(PrimNode_guess *primHead)
{
    int vertCount;
    int hasDeferred;
    PrimNode_guess *node;

    hasDeferred = 0;
    switch (g_render_mode_guess)
    {
    case 1:
        vertCount = 0;
        g_100fc794_guess = 0;
        node = primHead;
        if (primHead != 0)
        {
            do
            {
                if (node->cond == 0)
                {
                    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(g_vertBuf6_guess + vertCount * 2, node->verts, 6);
                    vertCount = g_100fc794_guess + 3;
                    g_100fbbc8_guess++;
                    g_100fc794_guess = vertCount;
                }
                else
                {
                    hasDeferred = 1;
                }
                node = node->next;
            } while (node != 0);
            if (vertCount != 0)
            {
                ((void (__stdcall *)(void *, int, int, void *, int, void *, int, int))(*(void ***)g_100fba6c_guess)[0x68 / 4])((void *)g_100fba6c_guess, 4, 0x1c4, g_101238a0_guess, g_1079fc00_guess, g_vertBuf6_guess, vertCount, 0);
            }
            if (hasDeferred != 0)
            {
                g_100fc794_guess = 0;
                do
                {
                    if ((primHead->cond & 1) != 0)
                    {
                        EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(g_vertBuf6_guess + g_100fc794_guess * 2, primHead->verts, 6);
                        g_100fc794_guess += 3;
                        g_100fbbc8_guess++;
                    }
                    primHead = primHead->next;
                } while (primHead != 0);
                if (g_100fc794_guess != 0)
                {
                    ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 8, 2);
                    ((void (__stdcall *)(void *, int, int, void *, int, void *, int, int))(*(void ***)g_100fba6c_guess)[0x68 / 4])((void *)g_100fba6c_guess, 4, 0x1c4, g_101238a0_guess, g_1079fc00_guess, g_vertBuf6_guess, g_100fc794_guess, 0);
                    ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 8, 3);
                }
            }
        }
        break;
    case 2:
        vertCount = 0;
        g_104afc24_guess = 0;
        node = primHead;
        if (primHead != 0)
        {
            do
            {
                if (node->cond == 0)
                {
                    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(g_vertBuf12_guess + vertCount * 4, node->verts, 0xc);
                    vertCount = g_104afc24_guess + 3;
                    g_100fbbc8_guess++;
                    g_104afc24_guess = vertCount;
                }
                else
                {
                    hasDeferred = 1;
                }
                node = node->next;
            } while (node != 0);
            if (vertCount != 0)
            {
                grDrawVertexArray_guess(6, vertCount, g_vertBuf12_guess);
            }
            if (hasDeferred != 0)
            {
                do
                {
                    if ((primHead->cond & 1) != 0)
                    {
                        grDrawLine_guess(((int *)primHead->verts)[0], ((int *)primHead->verts)[1]);
                        grDrawLine_guess(((int *)primHead->verts)[1], ((int *)primHead->verts)[2]);
                        grDrawLine_guess(((int *)primHead->verts)[2], ((int *)primHead->verts)[0]);
                        g_100fbbc8_guess++;
                    }
                    primHead = primHead->next;
                } while (primHead != 0);
            }
        }
        break;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10048CDF
// Draws a deferred blended triangle list, one batch per state: alpha-tested, additive or alpha-blended.
// __cdecl, returns 0. Each pass takes the nodes that match the first node's cond (mode 1 stages 6-byte vertices, mode 2 12-byte ones) and defers the rest.
int __cdecl t3dDrawTriangleListByBlendMode_guess(PrimNode_guess *primHead)
{
    PrimNode_guess *node;
    PrimNode_guess *nextNode;
    PrimNode_guess *pendingList;
    int flags;
    int fogFlag;
    int stateValue;
    int combineFunction;
    int combineFactor;
    int combineOther;

    if (g_render_mode_guess == 1)
    {
        while (primHead != 0)
        {
            node = primHead;
            primHead = 0;
            g_100fc794_guess = 0;
            if (node == 0)
            {
                break;
            }
            flags = node->cond;
            if (flags != 8)
            {
                ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0x19, 8);
                ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0xf, 0);
            }
            else
            {
                ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0x19, 5);
                ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0xf, 1);
            }
            ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0xe, flags == 8);
            fogFlag = flags & 4;
            if (fogFlag == 0)
            {
                ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0x13, 5);
                stateValue = 6;
            }
            else
            {
                ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0x13, 2);
                stateValue = 2;
            }
            ((void (__stdcall *)(void *, int, int))(*(void ***)g_100fba6c_guess)[0x50 / 4])((void *)g_100fba6c_guess, 0x14, stateValue);
            if (flags != 8)
            {
                EQ_CDECL(void, 0x1004A25E, (int))((fogFlag != 0) ? g_initialize_texture_state3_guess : g_100fbba4_guess);
            }
            if (flags != 8 && (g_initialize_texture_state4_guess != 0 || fogFlag != 0))
            {
                EQ_CDECL(void, 0x1004A23B, (int))(0);
            }
            else if (g_fog_enabled_guess != 0)
            {
                EQ_CDECL(void, 0x1004A23B, (int))(1);
            }
            do
            {
                nextNode = node->next;
                if (node->cond == flags)
                {
                    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(g_vertBuf6_guess + g_100fc794_guess * 2, node->verts, 6);
                    g_100fc794_guess += 3;
                    g_100fbbc8_guess++;
                }
                else
                {
                    if (primHead != 0)
                    {
                        node->next = primHead;
                    }
                    else
                    {
                        node->next = 0;
                    }
                    primHead = node;
                }
                node = nextNode;
            } while (nextNode != 0);
            if (g_100fc794_guess != 0)
            {
                ((void (__stdcall *)(void *, int, int, void *, int, void *, int, int))(*(void ***)g_100fba6c_guess)[0x68 / 4])((void *)g_100fba6c_guess, 4, 0x1c4, g_1055f900_guess, g_104afc20_guess, g_vertBuf6_guess, g_100fc794_guess, 0);
            }
        }
        EQ_CDECL(void, 0x1004A25E, (int))(g_100fbb9c_guess);
    }
    else if (g_render_mode_guess == 2)
    {
        pendingList = primHead;
        if (pendingList == 0)
        {
            return 0;
        }
        do
        {
            node = pendingList;
            pendingList = 0;
            g_104afc24_guess = 0;
            if (node == 0)
            {
                return 0;
            }
            flags = node->cond;
            if (flags == 8)
            {
                grAlphaTestFunction_guess(4);
                grAlphaTestReferenceValue_guess(0xc0);
                grDepthMask_guess(1);
            }
            else
            {
                grDepthMask_guess(0);
                grAlphaTestFunction_guess(7);
            }
            fogFlag = flags & 4;
            if (fogFlag != 0)
            {
                grFogMode_guess(0);
            }
            grAlphaBlendFunction_guess(1, (fogFlag == 0) ? 5 : 4, 0, 0);
            if (((int *)node)[1] == 0)
            {
                combineOther = 2;
                combineFactor = 8;
                combineFunction = 1;
            }
            else
            {
                combineOther = 1;
                combineFactor = 1;
                combineFunction = 3;
            }
            grAlphaCombine_guess(combineFunction, combineFactor, 0, combineOther, 0);
            do
            {
                nextNode = node->next;
                if (node->cond == flags)
                {
                    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(g_vertBuf12_guess + g_104afc24_guess * 4, node->verts, 0xc);
                    g_104afc24_guess += 3;
                    g_100fbbc8_guess++;
                }
                else
                {
                    if (pendingList != 0)
                    {
                        node->next = pendingList;
                    }
                    else
                    {
                        node->next = 0;
                    }
                    pendingList = node;
                }
                node = nextNode;
            } while (nextNode != 0);
            grDrawVertexArray_guess(6, g_104afc24_guess, g_vertBuf12_guess);
            if (fogFlag != 0 && g_fog_enabled_guess != 0)
            {
                grFogMode_guess(2);
            }
        } while (pendingList != 0);
    }
    return 0;
}

// FUNCTION: EQGFX 0x10048F8A
// Stores a renderer setting; its specific in-game effect is unclear.
// Cloned from t3dSetPCloudDensityMultiplier 0x1003FFD6 (same original code).
// __cdecl (one argument on the stack; the callee pops 0 bytes). Stores the passed dword in the global at
// 0x100FB8B8 (the density multiplier) and returns 0.
int __cdecl t3dFPSEnable(int density)
{
    g_initialize_window_state_guess = density;
    return 0;
}

// FUNCTION: EQGFX 0x10048F96
// Creates an overlay for an unspecified in-game display.
// __cdecl, no arguments. Creates the overlay t3dOverlayCreate(0x140, 100, 1) and stores its handle in the global at 0x100F3E2C.
void __cdecl createResourceHandle_guess(void)
{
    g_overlay_handle = EQ_CDECL(void *, 0x100491E6, (int, int, int))(0x140, 100, 1);
    return;
}

// FUNCTION: EQGFX 0x10048FAD
// __cdecl, no arguments. Destroys the overlay in the global at 0x100F3E2C when one is set, then clears the global.
void __cdecl releaseResourceHandle_guess(void)
{
    void *overlay;

    overlay = g_overlay_handle;
    if (overlay != 0)
    {
        EQ_CDECL(void, 0x10049387, (void *))(overlay);
        g_overlay_handle = 0;
    }
    return;
}
