// unit_10048FC5.c - eqgfx_dx7.dll, the address run 0x10048FC5..0x1004B579 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

typedef struct DirectDrawUpdateObject_guess
{
    void **vtable;
} DirectDrawUpdateObject_guess;

typedef struct T3DDeviceInfo_guess
{
    int deviceType;
    int adapterIndex;
    char name[0x40];
    int hardwareAcceleration;
    unsigned char driverIdentifier[0x10];
    unsigned char deviceIdentifier[0x10];
    unsigned char reserved[0xec];
    unsigned int resolutionFlags;
} T3DDeviceInfo_guess;

typedef struct T3DDeviceDescription_guess
{
    unsigned int size;
    int deviceClass;
    unsigned char driverIdentifier[0x10];
    unsigned char deviceIdentifier[0x20];
    char name[260];
    int hardwareAcceleration;
} T3DDeviceDescription_guess;

typedef struct T3DDisplayMode_guess
{
    int width;
    int height;
    int bitDepth;
} T3DDisplayMode_guess;

#define g_t3d_device_count_guess EQ_GLOBAL(int, 0x100F3FF0)
#define g_t3d_device_records_guess EQ_GLOBAL(T3DDeviceInfo_guess *, 0x100FC784)
#define g_t3d_device_enumeration_initialized_guess EQ_GLOBAL(int, 0x100F3FE0)
#define T3D_DEVICE_LIMIT 0x10
#define T3D_DEVICE_CLASS_PRIMARY (-5)

typedef int (__stdcall *DDTestCooperativeLevelFn)(void *);
typedef void (__stdcall *GrBufferSwapFn_guess)(int);

#define g_gr_buffer_swap_fn_guess EQ_GLOBAL(GrBufferSwapFn_guess, 0x100F3E90)

#define g_previous_color_depth_guess EQ_GLOBAL(int, 0x100FBA34)
#define g_last_fullscreen_mode_guess EQ_GLOBAL(int, 0x100FB8B0)
#define g_current_fullscreen_mode_guess EQ_GLOBAL(int, 0x100FB8AC)
#define g_resolution_mode_changed_guess EQ_GLOBAL(int, 0x100FB8B4)
#define T3D_RESOLUTION_UNSUPPORTED (-49858)
#define g_initialize_fog_far_bits_guess EQ_GLOBAL(unsigned int, 0x100FBA28)
#define g_initialize_render_window_guess EQ_GLOBAL(void *, 0x100FB8BC)
#define g_initialize_resolution_mask_guess EQ_GLOBAL(unsigned int, 0x100FBA48)
#define g_initialize_window_state_guess EQ_GLOBAL(int, 0x100FB8B8)
#define g_initialize_d3d_instance_guess EQ_GLOBAL(void *, 0x100FB8C4)
#define g_initialize_context_guess EQ_GLOBAL(int, 0x100F3FE4)
#define g_direct3d7 EQ_GLOBAL(void *, 0x100FBA68)
#define g_initialize_texture_flags_guess EQ_GLOBAL(unsigned int, 0x100FBB4C)
#define g_initialize_clear_depth_flags_guess EQ_GLOBAL(unsigned int, 0x100FBAFC)
#define g_initialize_auto_flags_guess EQ_GLOBAL(unsigned int, 0x100FBAF8)
#define EQGFX_MULTI_TMU_FLAG EQ_GLOBAL(unsigned int, 0x100FBA54)
#define g_100fbb9c_guess EQ_GLOBAL(int, 0x100FBB9C)
#define g_initialize_texture_state1_guess EQ_GLOBAL(int, 0x100FBBA0)
#define g_100fbba4_guess EQ_GLOBAL(int, 0x100FBBA4)
#define g_initialize_texture_state3_guess EQ_GLOBAL(int, 0x100FBBA8)
#define g_initialize_texture_state4_guess EQ_GLOBAL(int, 0x100FBBAC)

#define T3D_DEFAULT_RENDER_WIDTH 0x280
#define T3D_DEFAULT_RENDER_HEIGHT 0x1e0
#define T3D_DEFAULT_FOG_FAR_BITS 0x44160000
#define T3D_WINDOW_STYLE_INDEX (-0x10)
#define T3D_POPUP_WINDOW_STYLE 0x10ca0000
#define T3D_WINDOWED_STYLE ((int)0x91000000)
#define T3D_TOPMOST_WINDOW ((void *)0xfffffffe)

typedef struct D3DInterface_guess
{
    void **vtable;
} D3DInterface_guess;

typedef struct WindowRect_guess
{
    int left;
    int top;
    int right;
    int bottom;
} WindowRect_guess;

typedef struct T3DDeviceParameters_guess
{
    int deviceMode;
    int reserved004[0x11];
    int useDefaultAdapter;
    unsigned int adapterGuid[4];
    int reserved05c[0x3f];
    unsigned int resolutionMask;
} T3DDeviceParameters_guess;

typedef int (__stdcall *D3DReleaseFn_guess)(void *);
typedef int (__stdcall *D3DDev7SetTextureFn)(void *, int, void *);
typedef int (__stdcall *DDGetAvailableVidMemFn)(void *, void *, void *, void *);
typedef int (__stdcall *DDSetCooperativeLevelFn_guess)(void *, void *, unsigned int);
typedef void *(__stdcall *CreateICAFn_guess)(const char *, const char *, const char *, void *);
typedef int (__stdcall *GetDeviceCapsFn_guess)(void *, int);
typedef int (__stdcall *DeleteDCFn_guess)(void *);
typedef int (__stdcall *SetWindowLongAFn_guess)(void *, int, int);
typedef int (__stdcall *AdjustWindowRectFn_guess)(WindowRect_guess *, unsigned int, int);
typedef int (__stdcall *SetWindowPosFn_guess)(void *, void *, int, int, int, int, unsigned int);
typedef void *(__stdcall *SetCursorFn_guess)(void *);

typedef struct ClearRenderRegion_guess
{
    unsigned char reserved000[0x48];
    int width;
    int height;
    unsigned char reserved050[4];
    float x;
    float y;
} ClearRenderRegion_guess;

typedef struct DDBltEffects_guess
{
    unsigned int size;
    unsigned char reserved004[0x4c];
    unsigned int fillColor;
    unsigned char reserved054[0x10];
} DDBltEffects_guess;

typedef struct OverlayClearObject_guess
{
    int reserved_00;
    int height;
    int reserved_08;
    void *surface;
} OverlayClearObject_guess;

typedef struct RenderFrameState_guess
{
    unsigned char reserved000[0x184];
    int frameTime;
    unsigned char reserved188[0x108];
    unsigned int polygonCount;
    unsigned int regionCount;
    unsigned char reserved298[0x288];
    float overlayX;
    float overlayY;
} RenderFrameState_guess;

typedef struct RenderRect_guess
{
    int left;
    int top;
    int right;
    int bottom;
} RenderRect_guess;

typedef int (__stdcall *SurfaceBltFn_guess)(void *surface, RenderRect_guess *destinationRect,
    void *sourceSurface, RenderRect_guess *sourceRect, unsigned int flags, DDBltEffects_guess *effects);
typedef int (__stdcall *D3DDev7ClearFn)(void *device, int count, RenderRect_guess *rects,
    int flags, int color, float z, int stencil);

typedef struct OverlayFrameStatsLocals_guess
{
    RenderFrameState_guess *state;
    unsigned int elapsedTicks;
    unsigned char reserved008[0xc];
    char frameText[0x40];
    char statsText[0x40];
    RenderRect_guess rectangle;
    union
    {
        __int64 frameCounterAndElapsed;
        struct
        {
            int firstCoordinate;
            int secondCoordinate;
        } coordinates;
        struct
        {
            unsigned int geometryCount;
            int releasedResourceCount;
        } resourceCounts;
    } temporary;
    void *deviceContext;
} OverlayFrameStatsLocals_guess;

typedef int (__stdcall *SetRectFn_guess)(RenderRect_guess *, int, int, int, int);
typedef unsigned int (__stdcall *SetTextColorFn_guess)(void *, unsigned int);
typedef int (__stdcall *SetBkModeFn_guess)(void *, int);
typedef int (__stdcall *ExtTextOutAFn_guess)(void *, int, int, unsigned int,
                                             const RenderRect_guess *, const char *,
                                             unsigned int, const int *);
typedef int (__stdcall *SurfaceGetDCFn_guess)(void *, void **);
typedef int (__stdcall *SurfaceReleaseDCFn_guess)(void *, void *);

#define g_frame_counter_guess          EQ_GLOBAL(unsigned int, 0x100F3E5C)
#define g_last_frame_time_guess        EQ_GLOBAL(int, 0x100F3E68)
#define g_current_overlay_guess       EQ_GLOBAL(OverlayClearObject_guess *, 0x100F3E2C)
#define g_fps_tx_memory_guess          EQ_GLOBAL(int, 0x100FBB8C)
#define g_fps_swap_count_guess         EQ_GLOBAL(int, 0x100FBB84)
#define g_primCount_guess EQ_GLOBAL(unsigned int, 0x100FBBC8)
#define g_hardware_interrupt_count_guess EQ_GLOBAL(int, 0x100F3FF8)
#define EQGFX_PARTICLE_TICK_SCALE_GUESS EQ_GLOBAL(float, 0x10076344)
#define g_set_rect_fn_guess            EQ_GLOBAL(SetRectFn_guess, 0x100761FC)
#define g_set_text_color_fn_guess      EQ_GLOBAL(SetTextColorFn_guess, 0x10076014)
#define g_set_bk_mode_fn_guess         EQ_GLOBAL(SetBkModeFn_guess, 0x10076024)
#define g_ext_text_out_a_fn_guess      EQ_GLOBAL(ExtTextOutAFn_guess, 0x10076068)
#define g_fps_format_guess             EQ_ARRAY(char, 0x100A2858)
#define g_geometry_format_guess       EQ_ARRAY(char, 0x100A2838)
#define g_resource_format_guess       EQ_ARRAY(char, 0x100A2818)

typedef struct CollisionSphereRenderState_guess
{
    unsigned char reserved000[0x1ac];
    float positionX;
    float positionY;
    float positionZ;
    unsigned char reserved1b8[0x74];
    float zFromX;
    unsigned char reserved230[8];
    float zFromY;
    unsigned char reserved23c[8];
    float zFromZ;
    float xFromX;
    float xFromY;
    float xFromZ;
    float yFromX;
    float yFromY;
    float yFromZ;
    unsigned char reserved260[0x50];
    float sphereOffset[3];
    unsigned char reserved2bc[8];
    float *outputVertices;
    unsigned char reserved2c8[4];
    float *sphereVertices;
    unsigned char reserved2d0[0x12c];
    int sourceStateValue;
    int copiedStateValue;
} CollisionSphereRenderState_guess;

#define g_collision_sphere_axis_order_guess EQ_ARRAY(int, 0x100A2728)
#define g_collision_sphere_basis_guess EQ_ARRAY(float, 0x100A2698)

// eqgfx_dx7.dll's own globals (VAs of this image, not trilogy's).
#define g_render_device_guess         EQ_GLOBAL(void *, 0x100FBA6C)  // the render-state device (vtable at +0x0)
#define g_fog_enable_state EQ_GLOBAL(int, 0x100F3E34)     // the state last sent to the device
#define g_mode_state_guess            EQ_GLOBAL(int, 0x100A25C0)     // the mode last sent to the device
#define g_auto_mipmapping_supported_guess EQ_GLOBAL(int, 0x100FBA50) // nonzero when auto mipmapping can be used
#define g_auto_mipmapping_enabled_guess   EQ_GLOBAL(int, 0x100FBA58) // the auto-mipmapping flag being maintained
#define g_render_mode_guess               EQ_GLOBAL(int, 0x100FB8A8) // the render mode (2 is mipmapping-relevant)
#define g_direct_draw_guess               EQ_GLOBAL(void *, 0x100FBA2C) // the IDirectDraw object (GetCaps at vtable +0x2c)
#define g_primary_surface             EQ_GLOBAL(void *, 0x100FBA88) // the surface whose QueryInterface gives the gamma control
#define g_gamma_correction_rgb_guess      EQ_GLOBAL(void *, 0x100F3FDC) // _guGammaCorrectionRGB@12 (GetProcAddress'd at start-up)
#define g_gamma_level_guess               EQ_GLOBAL(int, 0x100FBA4C)    // the gamma level last set (the float's bits)
#define g_gr_buffer_clear_guess           EQ_GLOBAL(void *, 0x100F3E8C) // _grBufferClear@12 (GetProcAddress'd at start-up)
#define g_clear_depth_guess               EQ_GLOBAL(int, 0x100FBA74)    // the depth grBufferClear is given (mode 2)
#define g_clear_helper_object_guess       EQ_GLOBAL(void *, 0x100F3FE4) // the first argument of t3dD3DXContextClear
#define g_display_height_guess            EQ_GLOBAL(int, 0x100FBA44)           // the mode height in pixels (1024 and 1200 are tested)
#define g_color_depth_guess               EQ_GLOBAL(int, 0x100FBA38)           // the colour depth in bits (0x10 = 16-bit)

#define g_render_width_guess              EQ_GLOBAL(unsigned int, 0x100FBA40) // the render width the viewport gets
#define g_render_height_guess             EQ_GLOBAL(unsigned int, 0x100FBA44) // the render height the viewport gets
#define g_viewport_guess                  EQ_GLOBAL(D3DViewport7_guess, 0x100FBA70) // the viewport sent to the device
#define g_color_key_guess                 EQ_GLOBAL(unsigned int, 0x100FBA30) // the packed color key sent to the device
#define g_bits_per_pixel_guess            EQ_GLOBAL(unsigned int, 0x100FBA38) // the render surface's bits per pixel (0x10)
#define g_565_format_guess                EQ_GLOBAL(unsigned int, 0x100FBA3C) // nonzero: 16bpp packs 565, zero: 555
#define g_last_color_key_guess            EQ_GLOBAL(unsigned int, 0x100FBBB8) // the raw color of the last t3dSetColorKey
#define g_chromakey_value_fn_guess        EQ_GLOBAL(ChromakeyValueFn_guess, 0x100F3E98) // the grChromakeyValue hook
#define g_overlay_desc_guess              EQ_GLOBAL(DDSurfaceDesc2_guess, 0x100F3DB0) // the shared surface lock descriptor
#define g_overlay_desc_ready_guess        EQ_GLOBAL(int, 0x100F3E6C) // nonzero once the descriptor has been zeroed
#define g_fog_enabled_guess               EQ_GLOBAL(int, 0x100FBA1C)          // the enable flag the last t3dSetFog cached
#define g_fog_color_raw_guess             EQ_GLOBAL(unsigned int, 0x100FBA20) // the raw color the last t3dSetFog cached
#define g_fog_near_guess                  EQ_GLOBAL(float, 0x100FBA24)        // the near distance the last t3dSetFog cached
#define g_fog_far_guess                   EQ_GLOBAL(float, 0x100FBA28)        // the far distance the last t3dSetFog cached
#define g_fog_color_guess                 EQ_GLOBAL(unsigned int, 0x100FBB90) // the fog color sent to the render device
#define g_100fbb94_guess                 EQ_GLOBAL(float, 0x100FBB94)        // the fog start sent to the render device
#define g_100fbb98_guess                 EQ_GLOBAL(float, 0x100FBB98)        // 255 / (far - near)
#define g_overlay_ptr_guess               EQ_GLOBAL(Overlay_guess *, 0x100F3E30) // the overlay t3dReleaseDevice destroys (cleared after)
#define EQGFX_RENDERER_MEMORY                 EQ_GLOBAL(void *, 0x100FBA94)       // the fog table the Glide entry points get
#define g_fog_color_value_guess           EQ_GLOBAL(unsigned int, 0x100FBA98) // the color last handed to _grFogColorValue
#define g_fog_vertex_mode_pending            EQ_GLOBAL(int, 0x100A2690)          // 1: the linear fog vertex mode is still to send
#define EQGFX_JITTER_SCALE_GUESS           EQ_GLOBAL(float, 0x100762E8)        // the module's 255.0 constant (the fog range divisor)
#define g_gr_fog_mode_fn_guess            EQ_GLOBAL(GrFogModeFn_guess, 0x100F3EFC)           // GetProcAddress "_grFogMode@4"
#define g_gr_fog_color_value_fn_guess     EQ_GLOBAL(GrFogColorValueFn_guess, 0x100F3EF8)     // GetProcAddress "_grFogColorValue@4"
#define g_gu_fog_generate_linear_fn_guess EQ_GLOBAL(GuFogGenerateLinearFn_guess, 0x100F3FD4) // GetProcAddress "_guFogGenerateLinear@12"
#define g_gr_fog_table_fn_guess           EQ_GLOBAL(GrFogTableFn_guess, 0x100F3F00)           // GetProcAddress "_grFogTable@4"
#define EQGFX_GAMMA_LEVEL         EQ_GLOBAL(float, 0x100FBA4C)        // the gamma level last set, read as the float it holds
#define g_resource_total_guess            EQ_GLOBAL(int, 0x100F3E54)          // the resource count t3dReinitializeDevice reduces
#define g_resource_released_guess         EQ_GLOBAL(int, 0x100F3E58)          // the resources released since the last reinit (cleared there)
#define g_gr_finish_fn_guess              EQ_GLOBAL(GrFinishFn_guess, 0x100F3EF0)        // GetProcAddress "_grFinish@0"
#define g_gr_lfb_read_region_fn_guess     EQ_GLOBAL(GrLfbReadRegionFn_guess, 0x100F3F34) // GetProcAddress "_grLfbReadRegion@28"
#define g_back_buffer_surface              EQ_GLOBAL(void *, 0x100FBA8C)                  // the surface the capture locks (Lock at +0x64, Unlock at +0x80)
#define g_gr_depth_buffer_mode_fn_guess   EQ_GLOBAL(void *, 0x100F3EBC)
#define g_gr_lfb_constant_depth_fn_guess  EQ_GLOBAL(void *, 0x100F3F2C)
#define g_gr_lfb_constant_alpha_fn_guess  EQ_GLOBAL(void *, 0x100F3F28)
#define g_gr_alpha_blend_function_fn_guess EQ_GLOBAL(void *, 0x100F3E78)
#define g_gr_color_combine_fn_guess       EQ_GLOBAL(void *, 0x100F3EA0)
#define g_gr_chromakey_mode_fn_guess      EQ_GLOBAL(void *, 0x100F3E94)
#define g_gr_lfb_write_region_fn_guess    EQ_GLOBAL(void *, 0x100F3F44)

/* Vtable slot 0x50 of the render device: __stdcall (the original cleans none of the 3 arguments), stack `this`
 * first, then the render-state id and its value. */
typedef void (__stdcall *D3DDev7SetRenderStateFn)(void *device, int state_id, int state);
/* The Glide fog entry points at 0x100F3EFC.. (filled from GetProcAddress at start-up; the loader is
 * 0x1004BE4E): __stdcall (the original cleans none of their arguments), the @N suffix of each name says the
 * argument bytes (_grFogMode@4, _grFogColorValue@4, _guFogGenerateLinear@12, _grFogTable@4). t3dSetFog uses
 * them in render mode 2. */
typedef void (__stdcall *GrFogModeFn_guess)(int mode);
typedef void (__stdcall *GrFogColorValueFn_guess)(unsigned int color);
typedef void (__stdcall *GuFogGenerateLinearFn_guess)(void *table, float near_dist, float far_dist);
typedef void (__stdcall *GrFogTableFn_guess)(void *table);

/* The render-state id the fog-enable state is sent with (D3DRENDERSTATE_FOGENABLE is 28). */
#define EQGFX_RS_FOGENABLE 0x1c

/* The D3DRENDERSTATETYPE ids t3dSetFog sends through that slot (FOGVERTEXMODE, FOGCOLOR, FOGSTART, FOGEND). */
#define EQGFX_RS_FOGVERTEXMODE 0x8c
#define EQGFX_FOG_COLOR_STATE_GUESS      0x22
#define EQGFX_FOG_START_STATE_GUESS      0x24
#define EQGFX_FOG_END_STATE_GUESS        0x25

/* x * 0x7fffffff * 2 == -2 * x: the negative-edge factor the row length (right - left) * 2 is written with. */
#define EQGFX_NEGATIVE_EDGE_FACTOR 0x7fffffff

/* Vtable slot 0x8c of the render device (T3DDevice7_guess order: SetTexture): __stdcall (the original cleans none
 * of the 3 arguments), stack `device` first, then the texture stage and the texture (0 = none). */
typedef void (__stdcall *SetTextureFn_guess)(void *device, int stage, void *texture);

/* Vtable slot 0x94 of the render device: __stdcall (the original cleans none of the 4 arguments), stack
 * `device` first, then two fixed ids (0, 4) and the mode-derived value. */
typedef void (__stdcall *D3DDev7SetTextureStageStateFn)(void *device, int stage, int state, int value);

/* Vtable slot 0x94 of the render device with a float value: the mip LOD bias (D3DTSS_MIPMAPLODBIAS, 0x13) is sent as one. */
typedef void (__stdcall *D3DDev7SetTextureStageStateFloatFn)(void *device, int stage, int state, float value);

/* Vtable slot 0x80 of the surface an overlay keeps at +0x0c (a DirectDraw surface's Unlock): __stdcall (the
 * original cleans none of the 2 arguments), stack `surface` first, then the rect (NULL = all of it). */
typedef void (__stdcall *SurfaceUnlockFn_guess)(void *surface, void *rect);

/* Vtable slots 13 and 15 of the render device (T3DDevice7_guess order: SetViewport, GetViewport): __stdcall (the
 * original cleans none of the 2 arguments), stack `device` first, then the viewport. */
typedef int (__stdcall *DeviceViewportFn_guess)(void *device, void *viewport);

/* The device's viewport slots, and the full-size viewport global the clear runs under (24 bytes, like the saved copy). */
#define EQGFX_DEVICE_SET_VIEWPORT_SLOT_GUESS 13
#define EQGFX_DEVICE_GET_VIEWPORT_SLOT_GUESS 15
#define EQGFX_FULL_VIEWPORT_GUESS ((void *)0x100FBA70)

/* _grBufferClear@12 (Glide): __stdcall, the original cleans its 3 arguments (color, alpha, depth). */
typedef void (__stdcall *GrBufferClearFn_guess)(int color, int alpha, int depth);

/* IDirectDraw::GetCaps' DDCAPS (DX7, 0x17c bytes): the gamma check reads dwCaps2 (0x08). */
typedef struct DDCAPS_guess
{
    unsigned int dwSize;
    unsigned int dwCaps;
    unsigned int dwCaps2;
    unsigned char rest[0x17c - 0x0c];
} DDCAPS_guess;

/* The DDGAMMARAMP IDirectDrawGammaControl::SetGammaRamp takes: 256 WORDs each of red, green and blue (0x600 bytes). */
typedef struct DDGAMMARAMP_guess
{
    unsigned short red[256];
    unsigned short green[256];
    unsigned short blue[256];
} DDGAMMARAMP_guess;

/* Vtable slots (stdcall; the callee pops its arguments): IUnknown::QueryInterface +0x00, Release +0x08,
 * IDirectDraw::GetCaps +0x2c, IDirectDrawGammaControl::SetGammaRamp +0x10. */
typedef int (__stdcall *QueryInterfaceFn_guess)(void *object, const void *riid, void **out);
typedef int (__stdcall *ReleaseFn_guess)(void *object);
typedef int (__stdcall *GetCapsFn_guess)(void *object, DDCAPS_guess *driver_caps, DDCAPS_guess *hel_caps);
typedef int (__stdcall *SetGammaRampFn_guess)(void *control, unsigned int flags, DDGAMMARAMP_guess *ramp);

/* _guGammaCorrectionRGB@12: __stdcall, three floats, no result. */
typedef void (__stdcall *GammaCorrectionRGBFn_guess)(float red, float green, float blue);

/* The IID the gamma control is queried with (IDirectDrawGammaControl), and the DDCAPS2 bit the gamma check reads. */
#define EQGFX_IID_GAMMA_CONTROL_GUESS ((const void *)0x10078248)
#define EQGFX_DDCAPS2_PRIMARYGAMMA_GUESS 0x00020000

/* The camera's render window: the size as ints (0x48 width, 0x4c height), the origin as floats (0x54 left, 0x58 top). */
typedef struct T3dCamera_guess
{
    unsigned char head[0x48];
    int render_width;
    int render_height;
    unsigned char gap_50[0x54 - 0x50];
    float render_left;
    float render_top;
} T3dCamera_guess;

/* T3DDevice7_guess's D3DVIEWPORT7 (DX7, 24 bytes): the origin and size as DWORDs, then the depth range as floats. */
typedef struct D3DVIEWPORT7_guess
{
    unsigned int dwX;
    unsigned int dwY;
    unsigned int dwWidth;
    unsigned int dwHeight;
    float dvMinZ;
    float dvMaxZ;
} D3DVIEWPORT7_guess;

/* The D3DVIEWPORT7-shaped viewport at 0x100FBA70: x, y, width, height, minZ, maxZ. */
typedef struct D3DViewport7_guess
{
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    float minZ;
    float maxZ;
} D3DViewport7_guess;

/* Vtable slot 0x34 of the render device (a Direct3D 7 device's SetViewport): __stdcall (the original cleans
 * none of the 2 arguments), stack `device` first, then the viewport; returns an HRESULT. */
typedef int (__stdcall *SetViewportFn_guess)(void *device, void *viewport);

/* The grChromakeyValue hook at 0x100F3E98 (filled from GetProcAddress "_grChromakeyValue@4"): __stdcall (the
 * original cleans none of the 1 argument), the raw color; t3dSetColorKey calls it in render mode 2. */
typedef void (__stdcall *ChromakeyValueFn_guess)(unsigned int color);

/* The overlay object the t3d overlay functions pass around: its DirectDraw surface pointer sits at +0x0c. */
typedef struct Overlay_guess
{
    unsigned int width;            // 0x00
    unsigned int height;           // 0x04
    unsigned char flags;           // 0x08: bit 0x2 set = destroy keeps the surface alive
    unsigned char unknown_09[0x3]; // +0x09: the rest of the overlay
    void *surface;                 // 0x0c
} Overlay_guess;

/* The bitmap t3dOverlayOpen wraps: its size sits at +0x1c/+0x20 and its DirectDraw surface at +0x60. */
typedef struct BMInfo_guess
{
    char unknown_00[0x1c];         // the bitmap's own fields
    unsigned int width;            // 0x1c
    unsigned int height;           // 0x20
    char unknown_24[0x3c];         // the rest of the bitmap's fields
    void *surface;                 // 0x60: moved to the overlay by t3dOverlayOpen, then cleared
} BMInfo_guess;

typedef struct OverlayRect_guess
{
    int left;
    int top;
    int right;
    int bottom;
} OverlayRect_guess;

typedef struct OverlayApplyLocals_guess
{
    OverlayRect_guess destination;
    int surface_or_pitch;
    void *pixels;
} OverlayApplyLocals_guess;

typedef int (__stdcall *SurfaceBltFn_guess)(void *surface, OverlayRect_guess *destination,
    void *source, OverlayRect_guess *sourceRect, unsigned int flags, void *effects);
/* Vtable slot 0x1c of a DirectDraw surface (BltFast): x, y, source surface, source rect, flags. */
typedef int (__stdcall *SurfaceBltFastFn_guess)(void *surface, int x, int y,
    void *source, OverlayRect_guess *sourceRect, unsigned int flags);
typedef void (__stdcall *GrDepthBufferModeFn_guess)(int mode);
typedef void (__stdcall *GrLfbConstantDepthFn_guess)(int depth);
typedef void (__stdcall *GrLfbConstantAlphaFn_guess)(int alpha);
typedef void (__stdcall *GrAlphaBlendFunctionFn_guess)(int source, int destination, int sourceFunction, int destinationFunction);
typedef void (__stdcall *GrColorCombineFn_guess)(int function, int factor, int local, int other, int invert);
typedef void (__stdcall *GrChromakeyModeFn_guess)(int mode);
typedef void (__stdcall *GrLfbWriteRegionFn_guess)(int buffer, int x, int y, int format,
    int width, int height, int pixelPipeline, int stride, void *pixels);

/* The DDSURFACEDESC2-shaped scratch descriptor at 0x100F3DB0 (0x7c bytes): dwSize 0x00, lPitch 0x10,
 * lpSurface 0x24; the unnamed fields between are the rest of the descriptor's own layout. */
typedef struct DDSurfaceDesc2_guess
{
    unsigned int dwSize;               // 0x00
    unsigned int dwFlags;              // 0x04
    unsigned int dwHeight;             // 0x08
    unsigned int dwWidth;              // 0x0c
    int lPitch;                        // 0x10
    unsigned int dwBackBufferCount;    // 0x14
    unsigned int dwMipMapCount;        // 0x18
    unsigned int dwAlphaBitDepth;      // 0x1c
    unsigned int dwReserved;           // 0x20
    void *lpSurface;                   // 0x24
    unsigned int ddckCKDestOverlay[2]; // 0x28
    unsigned int ddckCKDestBlt[2];     // 0x30
    unsigned int ddckCKSrcOverlay[2];  // 0x38
    unsigned int ddckCKSrcBlt[2];      // 0x40
    unsigned int pixelFormatSize;      // 0x48
    unsigned int pixelFormatFlags;     // 0x4c
    unsigned int pixelFormatFourCC;    // 0x50
    unsigned int pixelFormatRgbBitCount; // 0x54
    unsigned int pixelFormatRedMask;   // 0x58
    unsigned int pixelFormatGreenMask; // 0x5c
    unsigned int pixelFormatBlueMask;  // 0x60
    unsigned int pixelFormatAlphaMask; // 0x64
    unsigned int ddsCaps[4];           // 0x68
    unsigned int dwTextureStage;       // 0x78
} DDSurfaceDesc2_guess;                // 0x7c

/* Vtable slot 0x64 of the surface an overlay keeps at +0x0c (a DirectDraw surface's Lock): __stdcall (the
 * original cleans none of the 5 arguments), stack `surface` first, then the rect (NULL = the whole surface),
 * the DDSURFACEDESC2 to fill, the lock flags (1 = DDLOCK_WAIT) and the event (NULL); returns an HRESULT. */
typedef int (__stdcall *SurfaceLockFn_guess)(void *surface, void *rect, void *desc, unsigned int flags, void *event);

/* Vtable slot 0x08 of the surface an overlay keeps at +0x0c (a DirectDraw surface's Release): __stdcall (the
 * original cleans none of the 1 argument), stack `surface` first. */
typedef void (__stdcall *SurfaceReleaseFn_guess)(void *surface);

/* GetProcAddress'd "_grFinish@0" (__stdcall, no arguments). */
typedef void (__stdcall *GrFinishFn_guess)(void);

/* GetProcAddress'd "_grLfbReadRegion@28" (__stdcall): buffer, x, y, width, height, stride, destination. */
typedef void (__stdcall *GrLfbReadRegionFn_guess)(int buffer, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int stride, void *data);

/* The 14-byte BITMAPFILEHEADER, packed: bfSize sits at offset 2. */
#pragma pack(push, 1)
typedef struct BitmapFileHeader_guess
{
    unsigned short bfType;             // 0x00: 'BM'
    unsigned int bfSize;               // 0x02
    unsigned short bfReserved1;        // 0x06
    unsigned short bfReserved2;        // 0x08
    unsigned int bfOffBits;            // 0x0a
} BitmapFileHeader_guess;
#pragma pack(pop)

/* The 40-byte BITMAPINFOHEADER written after the file header. */
typedef struct BitmapInfoHeader_guess
{
    unsigned int biSize;               // 0x00
    unsigned int biWidth;              // 0x04
    unsigned int biHeight;             // 0x08
    unsigned short biPlanes;           // 0x0c
    unsigned short biBitCount;         // 0x0e
    unsigned int biCompression;        // 0x10
    unsigned int biSizeImage;          // 0x14
    int biXPelsPerMeter;               // 0x18
    int biYPelsPerMeter;               // 0x1c
    unsigned int biClrUsed;            // 0x20
    unsigned int biClrImportant;       // 0x24
} BitmapInfoHeader_guess;

// FUNCTION: EQGFX 0x10048fc5
// Displays frame rate, geometry counts and resource statistics over the scene.
int __cdecl t3dDrawFrameStatsOverlay_guess(RenderFrameState_guess *renderState)
{
    OverlayFrameStatsLocals_guess locals;
    RenderFrameState_guess *state;
    float fpsRate;
    float overlayCoordinate;

    state = renderState;
    locals.state = state;
    ++g_frame_counter_guess;
    g_set_rect_fn_guess(&locals.rectangle, 0, 0, 0x13f, 99);
    locals.elapsedTicks = (unsigned int)(state->frameTime - g_last_frame_time_guess);
    if (g_current_overlay_guess == 0)
    {
        EQ_CDECL(void, 0x10048F96, (void))();
        if (g_current_overlay_guess == 0)
            return 0xffff3d3e;
    }

    if (locals.elapsedTicks >= 500)
    {
        locals.temporary.frameCounterAndElapsed = (unsigned int)g_frame_counter_guess;
        fpsRate =
            (float)((double)locals.temporary.frameCounterAndElapsed /
                    (double)(int)locals.elapsedTicks *
                    (double)EQGFX_PARTICLE_TICK_SCALE_GUESS);
        EQ_CDECL(void, 0x10049692, (void *))(g_current_overlay_guess);
        if (((SurfaceGetDCFn_guess)(*(void ***)g_current_overlay_guess->surface)[17])(
                 g_current_overlay_guess->surface, &locals.deviceContext) == 0)
        {
            EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(
                locals.frameText, 0, 0x40);
            EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(
                locals.frameText, g_fps_format_guess,
                (double)fpsRate,
                g_fps_tx_memory_guess, g_fps_swap_count_guess);
            g_set_text_color_fn_guess(locals.deviceContext, 0x64c800);
            g_set_bk_mode_fn_guess(locals.deviceContext, 1);
            g_ext_text_out_a_fn_guess(locals.deviceContext, 0, 0, 0,
                                      &locals.rectangle, locals.frameText,
                                      (unsigned int)EQ_CDECL(unsigned int, 0x10068160, (char *))(
                                          locals.frameText), 0);
            EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(
                locals.statsText, g_geometry_format_guess,
                state->polygonCount, g_primCount_guess,
                g_hardware_interrupt_count_guess);
            g_ext_text_out_a_fn_guess(locals.deviceContext, 0, 0x14, 0,
                                      &locals.rectangle, locals.statsText,
                                      (unsigned int)EQ_CDECL(unsigned int, 0x10068160, (char *))(
                                          locals.statsText), 0);
            locals.temporary.resourceCounts.geometryCount =
                (unsigned int)g_resource_total_guess;
            locals.temporary.resourceCounts.releasedResourceCount =
                g_resource_released_guess;
            if ((int)locals.temporary.resourceCounts.geometryCount ==
                locals.temporary.resourceCounts.releasedResourceCount)
            {
                locals.temporary.resourceCounts.releasedResourceCount = 0;
                g_resource_released_guess = 0;
            }
            locals.temporary.resourceCounts.geometryCount -=
                (unsigned int)locals.temporary.resourceCounts.releasedResourceCount;
            EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(
                locals.statsText, g_resource_format_guess,
                state->regionCount, locals.temporary.resourceCounts.geometryCount,
                locals.temporary.resourceCounts.releasedResourceCount);
            g_ext_text_out_a_fn_guess(locals.deviceContext, 0, 0x28, 0,
                                      &locals.rectangle, locals.statsText,
                                      (unsigned int)EQ_CDECL(unsigned int, 0x10068160, (char *))(
                                          locals.statsText), 0);
            if (((SurfaceReleaseDCFn_guess)(*(void ***)g_current_overlay_guess->surface)[26])(
                     g_current_overlay_guess->surface, locals.deviceContext) == 0)
            {
                g_frame_counter_guess = 0;
                g_last_frame_time_guess = state->frameTime;
                goto apply_overlay;
            }
        }

        return 0xffff3d3e;
    }

apply_overlay:
    overlayCoordinate = state->overlayY;
    __asm { fld overlayCoordinate }
    locals.temporary.coordinates.firstCoordinate = EQ_CDECL(int, 0x1004EC36, (void))();
    overlayCoordinate = state->overlayX;
    __asm { fld overlayCoordinate }
    locals.temporary.coordinates.secondCoordinate = EQ_CDECL(int, 0x1004EC36, (void))();
    EQ_CDECL(void, 0x100493BF, (void *, int, int, RenderRect_guess *))(
        g_current_overlay_guess, locals.temporary.coordinates.secondCoordinate,
        locals.temporary.coordinates.firstCoordinate,
        &locals.rectangle);
    return 0;
}

// FUNCTION: EQGFX 0x100491ce
// __cdecl, void. Destroys the overlay the global holds, if any, then clears the global.
void __cdecl t3dDestroyGlobalObject_guess(void)
{
    if (g_overlay_ptr_guess != 0)
    {
        EQ_CDECL(void, 0x10049387, (Overlay_guess *))(g_overlay_ptr_guess);
        g_overlay_ptr_guess = 0;
    }
    return;
}

// FUNCTION: EQGFX 0x100491E6
// Creates a sized overlay for screen graphics, with optional color-key transparency.
Overlay_guess *__cdecl t3dOverlayCreate(unsigned int width, unsigned int height, unsigned int overlayFlags)
{
    typedef int (__stdcall *CreateSurfaceFn_guess)(void *, DDSurfaceDesc2_guess *, void **, void *);
    typedef int (__stdcall *SetColorKeyFn_guess)(void *, unsigned int, void *);
    Overlay_guess *overlay;
    DDSurfaceDesc2_guess surfaceDesc;
    unsigned int mode;
    unsigned int colorKey[2];
    int result;

    overlay = EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x10);
    if (overlay != 0)
    {
        overlay->width = width;
        overlay->height = height;
        *(unsigned int *)&overlay->flags = overlayFlags;
    }

    mode = (unsigned int)g_render_mode_guess;
    if (mode > 0 && mode <= 2)
    {
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(&surfaceDesc, 0, 0x7c);
        surfaceDesc.dwSize = 0x7c;
        surfaceDesc.dwFlags = 0x1007;
        surfaceDesc.ddsCaps[0] = 0x840;
        surfaceDesc.dwWidth = width;
        surfaceDesc.dwHeight = height;
        surfaceDesc.pixelFormatSize = 0x20;
        surfaceDesc.pixelFormatFlags = 0x40;

        if (g_bits_per_pixel_guess == 0x10)
        {
            surfaceDesc.pixelFormatRgbBitCount = 0x10;
            if (g_565_format_guess != 0)
            {
                surfaceDesc.pixelFormatRedMask = 0xf800;
                surfaceDesc.pixelFormatGreenMask = 0x7e0;
            }
            else
            {
                surfaceDesc.pixelFormatRedMask = 0x7c00;
                surfaceDesc.pixelFormatGreenMask = 0x3e0;
            }
            surfaceDesc.pixelFormatBlueMask = 0x1f;
        }
        else
        {
            surfaceDesc.pixelFormatRgbBitCount = 0x20;
            surfaceDesc.pixelFormatRedMask = 0xff0000;
            surfaceDesc.pixelFormatGreenMask = 0xff00;
            surfaceDesc.pixelFormatBlueMask = 0xff;
        }

        result = ((CreateSurfaceFn_guess)(*(void ***)g_direct_draw_guess)[6])(
            g_direct_draw_guess, &surfaceDesc, &overlay->surface, 0);
        if (result < 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(overlay);
            overlay = 0;
        }
        else if ((overlay->flags & 1) != 0)
        {
            unsigned int colorKeyValue = g_color_key_guess;
            colorKey[0] = colorKeyValue;
            colorKey[1] = colorKeyValue;
            ((SetColorKeyFn_guess)(*(void ***)overlay->surface)[29])(
                overlay->surface, 8, colorKey);
        }
    }

    return overlay;
}

// FUNCTION: EQGFX 0x10049308
// Creates an overlay from a bitmap, optionally treating a chosen color as transparent.
// __cdecl (callee pops 0 bytes; ECX is not an input), returns the new overlay or 0. In render modes 1-2 it takes the
// bitmap's surface (+0x60, then cleared there), sets the color key when flag bit 0x1 is set, then calls t3dUndefineBMInfo.
Overlay_guess *__cdecl t3dOverlayOpen(unsigned int bmHandle, BMInfo_guess *bmInfo, unsigned int overlayFlags)
{
    typedef int (__stdcall *SetColorKeyFn_guess)(void *, unsigned int, void *);
    Overlay_guess *overlay;
    unsigned int colorKey[2];
    unsigned int mode;
    unsigned int height;

    overlay = EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x10);
    if (overlay != 0)
    {
        overlay->width = bmInfo->width;
        height = bmInfo->height;
        *(unsigned int *)&overlay->flags = overlayFlags;
        overlay->height = height;

        mode = (unsigned int)g_render_mode_guess;
        if (mode > 0 && mode <= 2)
        {
            overlay->surface = bmInfo->surface;
            if ((overlayFlags & 1) != 0)
            {
                colorKey[0] = g_color_key_guess;
                colorKey[1] = g_color_key_guess;
                ((SetColorKeyFn_guess)(*(void ***)overlay->surface)[29])(
                    overlay->surface, 8, colorKey);
            }
            bmInfo->surface = 0;
            EQ_CDECL(void, 0x1000BCF0, (unsigned int, BMInfo_guess **))(bmHandle, &bmInfo);
        }
        return overlay;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10049387
// Discards an overlay and releases its backing image when the overlay owns it.
// __cdecl (callee pops 0 bytes; ECX is not an input), one overlay, void. In render modes 1-2 releases the
// overlay's surface (vtable +0x8) and zeroes the overlay's first 0x10 bytes, unless flag bit 0x2 is set;
// then frees the overlay itself.
void __cdecl t3dOverlayDestroy(Overlay_guess *overlay)
{
    unsigned int mode = g_render_mode_guess;

    if (mode > 0 && mode <= 2 && (overlay->flags & 0x2) == 0)
    {
        ((SurfaceReleaseFn_guess)(*(void ***)overlay->surface)[2])(overlay->surface);
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(overlay, 0, 0x10);
    }

    EQ_CDECL(void, 0x10050CBB, (void *))(overlay);
}

// FUNCTION: EQGFX 0x100493BF
// Draws a selected overlay rectangle at a screen position, honoring its transparency flag.
void __cdecl t3dOverlayApply(Overlay_guess *overlay, int x, int y, OverlayRect_guess *sourceRect)
{
    OverlayApplyLocals_guess locals;
    unsigned int blitFlags;
    int sourceWidth;
    int sourceHeight;

    locals.surface_or_pitch = (int)overlay->surface;
    if (locals.surface_or_pitch != 0)
    {
        if (g_render_mode_guess == 1)
        {
            locals.destination.left = x;
            locals.destination.top = y;
            locals.destination.right = sourceRect->right - sourceRect->left + x;
            locals.destination.bottom = sourceRect->bottom - sourceRect->top + y;
            if ((overlay->flags & 1) == 0)
                blitFlags = 0x1000000;
            else
                blitFlags = 0x1008000;
            ((SurfaceBltFn_guess)(*(void ***)g_back_buffer_surface)[5])(
                g_back_buffer_surface, &locals.destination, (void *)locals.surface_or_pitch,
                sourceRect, blitFlags, 0);
        }
        else if (g_render_mode_guess == 2)
        {
            EQ_CDECL(void, 0x10049504, (Overlay_guess *, void **, unsigned int *))(
                overlay, &locals.pixels, (unsigned int *)&locals.surface_or_pitch);
            sourceHeight = sourceRect->bottom - sourceRect->top;
            locals.pixels = (char *)locals.pixels + sourceRect->top * locals.surface_or_pitch + sourceRect->left * 2;
            sourceWidth = sourceRect->right - sourceRect->left;
            if ((overlay->flags & 1) == 0)
            {
                ((GrLfbWriteRegionFn_guess)g_gr_lfb_write_region_fn_guess)(
                    1, x, y, 0, sourceWidth, sourceHeight, 0, locals.surface_or_pitch, locals.pixels);
            }
            else
            {
                ((GrDepthBufferModeFn_guess)g_gr_depth_buffer_mode_fn_guess)(0);
                ((GrLfbConstantDepthFn_guess)g_gr_lfb_constant_depth_fn_guess)(g_clear_depth_guess);
                ((GrLfbConstantAlphaFn_guess)g_gr_lfb_constant_alpha_fn_guess)(0xff);
                ((GrAlphaBlendFunctionFn_guess)g_gr_alpha_blend_function_fn_guess)(4, 0, 0, 0);
                ((GrColorCombineFn_guess)g_gr_color_combine_fn_guess)(1, 0, 0, 2, 0);
                ((GrChromakeyModeFn_guess)g_gr_chromakey_mode_fn_guess)(1);
                ((GrLfbWriteRegionFn_guess)g_gr_lfb_write_region_fn_guess)(
                    1, x, y, 0, sourceWidth, sourceHeight, 1, locals.surface_or_pitch, locals.pixels);
                ((GrDepthBufferModeFn_guess)g_gr_depth_buffer_mode_fn_guess)(2);
                ((GrChromakeyModeFn_guess)g_gr_chromakey_mode_fn_guess)(0);
            }
            EQ_CDECL(void, 0x10049576, (Overlay_guess *))(overlay);
        }
    }
}

// FUNCTION: EQGFX 0x10049504
// Gives drawing code access to an overlay's pixels for painting.
// __cdecl (callee pops 0 bytes; ECX is not an input), an overlay and two out pointers, void. Locks the
// overlay's surface (vtable +0x64, NULL rect, DDLOCK_WAIT) into the shared descriptor in render modes 1-2.
void __cdecl t3dOverlayLock(Overlay_guess *overlay, void **pixels, unsigned int *pitch)
{
    if (g_overlay_desc_ready_guess == 0)
    {
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(&g_overlay_desc_guess, 0, sizeof(DDSurfaceDesc2_guess));
        g_overlay_desc_ready_guess = 1;
    }

    if (overlay->surface != 0)
    {
        unsigned int mode = g_render_mode_guess;

        if (mode > 0 && mode <= 2)
        {
            g_overlay_desc_guess.dwSize = sizeof(DDSurfaceDesc2_guess);
            ((SurfaceLockFn_guess)(*(void ***)overlay->surface)[25])(overlay->surface, 0, &g_overlay_desc_guess, 1, 0);
            *pixels = g_overlay_desc_guess.lpSurface;
            *pitch = g_overlay_desc_guess.lPitch;
        }
    }
}

// FUNCTION: EQGFX 0x10049576
// Finishes painting an overlay so its surface can be used again.
// __cdecl (callee pops 0 bytes; ECX is not an input), one pointer to an overlay; unlocks its surface
// (vtable +0x80, NULL rect) while the render mode (0x100FB8A8) is 1 or 2 (unsigned).
void __cdecl t3dOverlayUnlock(void *overlay)
{
    void *surface = *(void **)((char *)overlay + 0xc); // 0x0c

    if (surface != 0)
    {
        unsigned int mode = g_render_mode_guess;

        if (mode > 0 && mode <= 2)
            ((SurfaceUnlockFn_guess)(*(void ***)surface)[32])(surface, 0); // +0x80
    }
}

// FUNCTION: EQGFX 0x1004959C
// Fills an entire overlay with a chosen color, falling back to black if the fill is unsupported.
void __cdecl t3dOverlayClear(int overlay_handle, unsigned int color)
{
    OverlayClearObject_guess *overlay = (OverlayClearObject_guess *)overlay_handle;
    unsigned int blt_effects[25];
    void *surface_bits;
    int blt_result;
    unsigned int fill_color;
    unsigned int upper_color;
    unsigned int render_mode;
    void *surface = overlay->surface;

    if (surface != 0)
    {
        render_mode = (unsigned int)g_render_mode_guess;
        if (render_mode > 0 && render_mode <= 2)
        {
            EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(blt_effects, 0, 100);
            blt_effects[0] = 100;
            if (g_bits_per_pixel_guess == 0x10)
            {
                if (g_565_format_guess == 0)
                    fill_color = (((color >> 8) & 0xf8) | ((color & 0xf8) << 5)) << 2;
                else
                    fill_color = (((color >> 8) & 0xfc) | ((color & 0xf8) << 5)) << 3;
                upper_color = (color >> 16 & 0xff) >> 3;
            }
            else
            {
                fill_color = ((color & 0xff) << 16) | (color & 0xff00);
                upper_color = (color >> 16) & 0xff;
            }
            fill_color |= upper_color;
            blt_effects[20] = fill_color;
            blt_result = ((int (__stdcall *)(void *, void *, void *, void *, unsigned int, void *))(*(void ***)surface)[5])
                (surface, 0, 0, 0, 0x1000400, blt_effects);
            if (blt_result == -0x7789fdc1)
            {
                EQ_CDECL(void, 0x10049504, (void *, void **, int *))(overlay, &surface_bits, &overlay_handle);
                EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))
                    (surface_bits, 0, overlay->height * overlay_handle);
                EQ_CDECL(void, 0x10049576, (void *))(overlay);
            }
        }
    }
    return;
}

// FUNCTION: EQGFX 0x10049692
// __cdecl, one overlay handle, void: clears it with the last color key (0x100FBBB8) in render modes 1-2.
void __cdecl t3dOverlayClearWithColorKey(int overlay_handle)
{
    unsigned int render_mode = (unsigned int)g_render_mode_guess;

    if (render_mode > 0 && render_mode <= 2)
    {
        EQ_CDECL(void, 0x1004959C, (int, unsigned int))(overlay_handle, g_last_color_key_guess);
    }
    return;
}

// FUNCTION: EQGFX 0x100496b2
// Fills a rectangular overlay area with a chosen color, falling back to black if the fill is unsupported.
// __cdecl (callee pops 0 bytes), an overlay, a rectangle and a packed color, void: fills the rectangle's
// rows of the locked surface with the repacked color through a Blt effect (0x1000400) in render modes 1-2.
void __cdecl t3dOverlayClearArea(void *overlay_handle, OverlayRect_guess *rect, unsigned int color)
{
    unsigned int blt_effects[25];
    Overlay_guess *overlay;
    int pitch;
    int row;
    int blt_result;
    unsigned int render_mode;

    overlay = (Overlay_guess *)overlay_handle;
    if (overlay->surface == 0)
        return;
    render_mode = (unsigned int)g_render_mode_guess;
    if (render_mode <= 0)
        return;
    if (render_mode > 2)
        return;

    EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(blt_effects, 0, 100);
    blt_effects[0] = 100;
    if (g_bits_per_pixel_guess == 0x10)
    {
        if (g_565_format_guess != 0)
            blt_effects[20] = (((color >> 8) & 0xfc) | ((color & 0xf8) << 5)) << 3 | (color >> 16 & 0xff) >> 3;
        else
            blt_effects[20] = (((color >> 8) & 0xf8) | ((color & 0xf8) << 5)) << 2 | (color >> 16 & 0xff) >> 3;
    }
    else
    {
        blt_effects[20] = (((color >> 8) & 0xff) | ((color & 0xff) << 8)) << 8 | (color >> 16 & 0xff);
    }

    blt_result = ((int (__stdcall *)(void *, void *, void *, void *, unsigned int, void *))(*(void ***)overlay->surface)[5])
        (overlay->surface, rect, 0, 0, 0x1000400, blt_effects);
    if (blt_result == -0x7789fdc1)
    {
        EQ_CDECL(void, 0x10049504, (void *, void **, int *))(overlay, &overlay_handle, &pitch);
        row = rect->top;
        overlay_handle = (char *)overlay_handle + row * pitch + rect->left * 2;
        for (; row < rect->bottom; row++)
        {
            EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))
                (overlay_handle, 0, (rect->left * EQGFX_NEGATIVE_EDGE_FACTOR + rect->right) * 2);
            overlay_handle = (char *)overlay_handle + pitch;
        }
        EQ_CDECL(void, 0x10049576, (void *))(overlay);
    }
    return;
}

// FUNCTION: EQGFX 0x100497e9
// __cdecl, two overlays and their rectangles, void: BltFast, then a manual (color-keyed) pixel copy, then unlock.
void __cdecl t3dOverlayMerge(void *source_handle, void *dest_handle, OverlayRect_guess *source_rect,
    OverlayRect_guess *dest_rect)
{
    Overlay_guess *source;
    Overlay_guess *dest;
    OverlayRect_guess *source_bounds;
    OverlayRect_guess *dest_bounds;
    unsigned short *source_pixels;
    unsigned short *dest_pixels;
    unsigned int source_pitch;
    unsigned int dest_pitch;
    int row;
    int column;

    source = (Overlay_guess *)source_handle;
    dest = (Overlay_guess *)dest_handle;
    source_bounds = source_rect;
    dest_bounds = dest_rect;
    if (source->surface == 0)
        return;
    if (dest->surface == 0)
        return;
    if ((unsigned int)g_render_mode_guess <= 0)
        return;
    if ((unsigned int)g_render_mode_guess > 2)
        return;

    if ((source->flags & 1) != 0)
    {
        if (((SurfaceBltFastFn_guess)(*(void ***)dest->surface)[7])(dest->surface, dest_bounds->left,
                dest_bounds->top, source->surface, source_bounds, 0x11) != -0x7789fdc1)
            return;

        EQ_CDECL(void, 0x10049504, (Overlay_guess *, void **, unsigned int *))(
            source, (void **)&dest_rect, &source_pitch);
        if (dest == source)
        {
            dest_pitch = source_pitch;
            dest_pixels = (unsigned short *)dest_rect;
        }
        else
        {
            EQ_CDECL(void, 0x10049504, (Overlay_guess *, void **, unsigned int *))(
                dest, (void **)&source_rect, &dest_pitch);
            dest_pixels = (unsigned short *)source_rect;
        }

        source_pixels = (unsigned short *)dest_rect;
        source_pixels = source_pixels +
            (source_pitch >> 1) * source_bounds->top + source_bounds->left;
        dest_pixels = dest_pixels +
            (dest_pitch >> 1) * dest_bounds->top + dest_bounds->left;
        for (row = dest_bounds->top; row < dest_bounds->bottom; row++)
        {
            for (column = dest_bounds->left; column < dest_bounds->right; column++)
            {
                if (*source_pixels != 0)
                    *dest_pixels = *source_pixels;
                source_pixels++;
                dest_pixels++;
            }
            source_pixels += (source_pitch >> 1) - dest_bounds->right + dest_bounds->left;
            dest_pixels += (dest_pitch >> 1) - dest_bounds->right + dest_bounds->left;
        }
    }
    else
    {
        if (((SurfaceBltFastFn_guess)(*(void ***)dest->surface)[7])(dest->surface, dest_bounds->left,
                dest_bounds->top, source->surface, source_bounds, 0x10) != -0x7789fdc1)
            return;

        EQ_CDECL(void, 0x10049504, (Overlay_guess *, void **, unsigned int *))(
            source, (void **)&dest_rect, &source_pitch);
        if (dest == source)
        {
            dest_pitch = source_pitch;
            dest_pixels = (unsigned short *)dest_rect;
        }
        else
        {
            EQ_CDECL(void, 0x10049504, (Overlay_guess *, void **, unsigned int *))(
                dest, (void **)&source_rect, &dest_pitch);
            dest_pixels = (unsigned short *)source_rect;
        }

        source_pixels = (unsigned short *)dest_rect;
        source_pixels = source_pixels +
            (source_pitch >> 1) * source_bounds->top + source_bounds->left;
        dest_pixels = dest_pixels +
            (dest_pitch >> 1) * dest_bounds->top + dest_bounds->left;
        for (row = dest_bounds->top; row < dest_bounds->bottom; row++)
        {
            EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(
                dest_pixels, source_pixels,
                (source_bounds->left * EQGFX_NEGATIVE_EDGE_FACTOR + source_bounds->right) * 2);
            source_pixels += source_pitch >> 1;
            dest_pixels += dest_pitch >> 1;
        }
    }

    if (dest != source)
        EQ_CDECL(void, 0x10049576, (void *))(dest);
    EQ_CDECL(void, 0x10049576, (void *))(source);
    return;
}

// FUNCTION: EQGFX 0x100499dd
// Presents the current frame to the screen and handles display update failures.
void __cdecl t3dUpdateDisplay(void)
{
    int result;

    switch (g_render_mode_guess)
    {
    case 1:
        g_fps_swap_count_guess = 0;
        g_fps_tx_memory_guess = 0;
        result = EQ_CDECL(int, 0x10045D96, (int, int))(g_initialize_context_guess, 0);
        if ((result == -0x7789fe3e) || (result == -0x7789fe52))
        {
            result = ((DDTestCooperativeLevelFn)
                ((DirectDrawUpdateObject_guess *)g_direct_draw_guess)->vtable[0x68 / 4])(
                    g_direct_draw_guess);
            if (result >= 0)
            {
                EQ_CDECL(void, 0x10045DA5, (int))(g_initialize_context_guess);
                return;
            }
            if (result == -0x7789fdb5)
            {
                EQ_CDECL(void, 0x1004AF92, (void))();
                return;
            }
        }
        break;
    case 2:
        g_fps_swap_count_guess = 0;
        g_fps_tx_memory_guess = 0;
        g_gr_buffer_swap_fn_guess(1);
        return;
    }
    return;
}

// FUNCTION: EQGFX 0x10049a52
// Sets the color treated as transparent in bitmap overlays.
// __cdecl (callee pops 0 bytes; ECX is not an input), one packed color argument, void (callers never read EAX
// after the call). Repacks the color into the color key at 0x100FBA30 for render modes 1 and 2 (555/565 at
// 16bpp, byte-swapped otherwise) and, in mode 2, hands it to the grChromakeyValue hook; caches it at 0x100FBBB8.
void __cdecl t3dSetColorKey(unsigned int color)
{
    unsigned int packed_color;

    switch (g_render_mode_guess)
    {
    case 1:
        if (g_bits_per_pixel_guess == 0x10)
        {
            if (g_565_format_guess != 0)
                packed_color = (((color >> 8) & 0xfc) | ((color & 0xf8) << 5)) << 3;
            else
                packed_color = (((color >> 8) & 0xf8) | ((color & 0xf8) << 5)) << 2;
            g_color_key_guess = packed_color | (((color >> 16) & 0xff) >> 3);
        }
        else
        {
            g_color_key_guess = (((color >> 8) & 0xff) | ((color & 0xff) << 8)) << 8 | ((color >> 16) & 0xff);
        }
        break;
    case 2:
        if (g_565_format_guess != 0)
            packed_color = (((color >> 8) & 0xfc) | ((color & 0xf8) << 5)) << 3;
        else
            packed_color = (((color >> 8) & 0xf8) | ((color & 0xf8) << 5)) << 2;
        g_color_key_guess = packed_color | (((color >> 16) & 0xff) >> 3);
        g_chromakey_value_fn_guess(color);
        break;
    }

    g_last_color_key_guess = color;
}

// FUNCTION: EQGFX 0x10049f0d
// Controls distance fog visibility, color and the range over which the scene fades into fog.
// __cdecl (callee pops 0 bytes; ECX is not an input), one enable flag, two float distances and three color
// bytes, returns 0. Mode 2 drives the Glide fog entry points, mode 1 the device fog states at vtable slot 0x50.
int __cdecl t3dSetFog(int enabled, float near_dist, float far_dist, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned int fog_color;

    switch (g_render_mode_guess)
    {
    case 1:
        if (enabled != 0)
        {
            EQ_CDECL(void, 0x1004A23B, (int))(1);
            if (g_fog_vertex_mode_pending == 1)
            {
                g_fog_vertex_mode_pending = 0;
                ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_RS_FOGVERTEXMODE, 3);
            }
            fog_color = ((red << 8) | green) << 8 | blue;
            ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_FOG_COLOR_STATE_GUESS, fog_color);
            ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_FOG_START_STATE_GUESS, *(unsigned int *)&near_dist);
            ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, EQGFX_FOG_END_STATE_GUESS, *(unsigned int *)&far_dist);
            if (g_bits_per_pixel_guess == 0x10)
            {
                if (g_565_format_guess != 0)
                    fog_color = (((red & 0xf8) << 5) | (green & 0xfc)) << 3;
                else
                    fog_color = (((red & 0xf8) << 5) | (green & 0xf8)) << 2;
                g_fog_color_guess = fog_color | (blue >> 3);
            }
            else
            {
                g_fog_color_guess = fog_color;
            }
            g_100fbb94_guess = near_dist;
            g_100fbb98_guess = EQGFX_JITTER_SCALE_GUESS / (far_dist - near_dist);
        }
        else
        {
            EQ_CDECL(void, 0x1004A23B, (int))(0);
        }
        break;
    case 2:
        if (enabled != 0)
        {
            g_gr_fog_mode_fn_guess(2);
            fog_color = ((red << 8) | green) << 8 | blue;
            g_fog_color_value_guess = fog_color;
            g_gr_fog_color_value_fn_guess(fog_color);
            g_gu_fog_generate_linear_fn_guess(EQGFX_RENDERER_MEMORY, near_dist, far_dist);
            g_gr_fog_table_fn_guess(EQGFX_RENDERER_MEMORY);
        }
        else
        {
            g_gr_fog_mode_fn_guess(0);
        }
        break;
    }

    g_fog_enabled_guess = enabled;
    g_fog_color_raw_guess = ((blue << 8) | green) << 8 | red;
    g_fog_near_guess = near_dist;
    g_fog_far_guess = far_dist;
    return 0;
}

// FUNCTION: EQGFX 0x10049B2F
// Sets the camera's screen area for rendering.
// __cdecl (callee pops 0 bytes; ECX is not an input); window = (left, top, right, bottom). Mode 1 also sets the viewport.
void __cdecl t3dSetCameraRenderWindow(T3dCamera_guess *camera, int *window)
{
    D3DVIEWPORT7_guess viewport;

    camera->render_left = (float)window[0];
    camera->render_top = (float)window[1];
    camera->render_width = window[2] - window[0];
    camera->render_height = window[3] - window[1];
    switch (g_render_mode_guess)
    {
    case 1:
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(&viewport, 0, sizeof(viewport));
        viewport.dwX = window[0];
        viewport.dwY = window[1];
        viewport.dwWidth = window[2] - window[0];
        viewport.dwHeight = window[3] - window[1];
        viewport.dvMinZ = 0.0f;
        viewport.dvMaxZ = 1.0f;
        ((DeviceViewportFn_guess)(*(void ***)g_render_device_guess)[EQGFX_DEVICE_SET_VIEWPORT_SLOT_GUESS])(g_render_device_guess, &viewport);
        break;
    }
}

// FUNCTION: EQGFX 0x10049ba7
// Saves a screenshot of the rendered screen as a 24-bit BMP file at the given path.
// __cdecl (callee pops 0 bytes). Writes the render target to a 24-bit BMP at path: mode 1 locks the surface, mode 2 reads
// the front buffer back through the Glide LFB. path is reused for the FILE* (the original stores it in its argument slot).
void __cdecl t3dScreenCapture(void *path)
{
    char bgr_row[12288];
    unsigned short lfb_row[4096];
    DDSurfaceDesc2_guess desc;
    BitmapFileHeader_guess file_header;
    BitmapInfoHeader_guess info_header;
    unsigned int row;
    int pixels_left;
    char *out;
    unsigned short *src16;
    char *src8;
    unsigned int pixel;

    path = EQ_CDECL(void *, 0x10068412, (const char *, const char *))(path, EQ_ARRAY(char, 0x100A2878));
    if (path != 0)
    {
        unsigned int image_size;

        info_header.biHeight = g_render_height_guess;
        image_size = g_render_height_guess * g_render_width_guess * 3;
        info_header.biSizeImage = image_size;
        file_header.bfSize = image_size + 0x36;
        info_header.biSize = sizeof(info_header);
        info_header.biWidth = g_render_width_guess;
        info_header.biPlanes = 1;
        info_header.biBitCount = 0x18;
        info_header.biCompression = 0;
        info_header.biXPelsPerMeter = 0;
        info_header.biYPelsPerMeter = 0;
        info_header.biClrUsed = 0;
        info_header.biClrImportant = 0;
        file_header.bfType = 0x4d42;
        file_header.bfOffBits = 0x36;
        file_header.bfReserved1 = 0;
        file_header.bfReserved2 = 0;
        EQ_CDECL(void, 0x10069A2A, (void *, unsigned int, unsigned int, void *))(&file_header, sizeof(file_header), 1, path);
        EQ_CDECL(void, 0x10069A2A, (void *, unsigned int, unsigned int, void *))(&info_header, sizeof(info_header), 1, path);
        switch (g_render_mode_guess)
        {
        case 1:
            EQ_CDECL(void, 0x100682D0, (void *, int, unsigned int))(&desc, 0, sizeof(desc));
            desc.dwSize = sizeof(desc);
            if (((SurfaceLockFn_guess)(*(void ***)g_back_buffer_surface)[25])(g_back_buffer_surface, 0, &desc, 1, 0) == 0)
            {
                if (g_bits_per_pixel_guess == 0x10)
                {
                    if (g_565_format_guess != 0)
                    {
                        row = 0;
                        if (info_header.biHeight > 0)
                        {
                            src16 = (unsigned short *)((char *)desc.lpSurface + (info_header.biHeight - 1) * desc.lPitch);
                            do
                            {
                                out = bgr_row;
                                if (info_header.biWidth > 0)
                                {
                                    pixels_left = info_header.biWidth;
                                    do
                                    {
                                        out[0] = (char)*src16 << 3;
                                        out[1] = (unsigned char)(*src16 >> 3) & 0xfc;
                                        out[2] = *((unsigned char *)src16 + 1) & 0xf8;
                                        out += 3;
                                        src16++;
                                        pixels_left--;
                                    } while (pixels_left != 0);
                                }
                                src16 = src16 + -((desc.lPitch >> 1) + info_header.biWidth);
                                EQ_CDECL(void, 0x10069A2A, (void *, unsigned int, unsigned int, void *))(bgr_row, g_render_width_guess * 3, 1, path);
                                row++;
                            } while (row < info_header.biHeight);
                        }
                    }
                    else
                    {
                        row = 0;
                        if (info_header.biHeight > 0)
                        {
                            src16 = (unsigned short *)((char *)desc.lpSurface + (info_header.biHeight - 1) * desc.lPitch);
                            do
                            {
                                out = bgr_row;
                                if (info_header.biWidth > 0)
                                {
                                    pixels_left = info_header.biWidth;
                                    do
                                    {
                                        out[0] = (char)*src16 << 3;
                                        out[1] = (unsigned char)(*src16 >> 2) & 0xf8;
                                        out[2] = (unsigned char)(*src16 >> 7) & 0xf8;
                                        out += 3;
                                        src16++;
                                        pixels_left--;
                                    } while (pixels_left != 0);
                                }
                                src16 = src16 + -((desc.lPitch >> 1) + info_header.biWidth);
                                EQ_CDECL(void, 0x10069A2A, (void *, unsigned int, unsigned int, void *))(bgr_row, g_render_width_guess * 3, 1, path);
                                row++;
                            } while (row < info_header.biHeight);
                        }
                    }
                }
                else
                {
                    row = 0;
                    if (info_header.biHeight > 0)
                    {
                        src8 = (char *)desc.lpSurface + (info_header.biHeight - 1) * desc.lPitch;
                        do
                        {
                            out = bgr_row;
                            if (info_header.biWidth > 0)
                            {
                                pixels_left = info_header.biWidth;
                                do
                                {
                                    *out = *src8;
                                    out[1] = (char)(*(unsigned int *)src8 >> 8);
                                    pixel = *(unsigned int *)src8;
                                    src8 += 4;
                                    out[2] = (char)(pixel >> 16);
                                    out += 3;
                                    pixels_left--;
                                } while (pixels_left != 0);
                            }
                            src8 = src8 + ((desc.lPitch >> 2) + info_header.biWidth) * -4;
                            EQ_CDECL(void, 0x10069A2A, (void *, unsigned int, unsigned int, void *))(bgr_row, g_render_width_guess * 3, 1, path);
                            row++;
                        } while (row < info_header.biHeight);
                    }
                }
                ((SurfaceUnlockFn_guess)(*(void ***)g_back_buffer_surface)[32])(g_back_buffer_surface, 0);
            }
            break;
        case 2:
            g_gr_finish_fn_guess();
            row = 0;
            if (g_render_height_guess > 0)
            {
                do
                {
                    g_gr_lfb_read_region_fn_guess(1, 0, g_render_height_guess - row - 1, g_render_width_guess, 1, g_render_width_guess * 2, lfb_row);
                    out = bgr_row;
                    if (g_render_width_guess > 0)
                    {
                        src16 = lfb_row;
                        pixels_left = g_render_width_guess;
                        do
                        {
                            out[0] = (char)*src16 << 3;
                            pixel = *src16;
                            out[1] = (unsigned char)(pixel >> 3) & 0xfc;
                            out[2] = (unsigned char)(pixel >> 8) & 0xf8;
                            out += 3;
                            src16++;
                            pixels_left--;
                        } while (pixels_left != 0);
                    }
                    EQ_CDECL(void, 0x10069A2A, (void *, unsigned int, unsigned int, void *))(bgr_row, g_render_width_guess * 3, 1, path);
                    row++;
                } while (row < g_render_height_guess);
            }
            break;
        }
        EQ_CDECL(void, 0x10068328, (void *))(path);
    }
}

// FUNCTION: EQGFX 0x1004A0AA
// Adjusts display brightness through gamma correction applied equally to red, green and blue.
// __cdecl (callee pops 0 bytes; ECX is not an input), one float level, int result (0, or 0xffff3d3e when the gamma
// cannot be set). Render mode 1 uploads a 256-entry ramp to the gamma surface; mode 2 calls _guGammaCorrectionRGB.
int __cdecl t3dSetGammaLevel(float level)
{
    DDCAPS_guess driver_caps;
    DDCAPS_guess hel_caps;
    DDGAMMARAMP_guess ramp;
    double inverse_gamma;
    unsigned int index;
    unsigned int ramp_out;
    unsigned short ramp_value;
    unsigned short *cursor;
    void *gamma_control;
    int hr;

    switch (g_render_mode_guess)
    {
    case 2:
        if (level <= 0.0f)
            level = 0.0001f;
        if (20.0f <= level)
            level = 20.0f;
        ((GammaCorrectionRGBFn_guess)g_gamma_correction_rgb_guess)(level, level, level);
        break;
    case 1:
        if (g_primary_surface == 0)
            return 0xffff3d3e;
        driver_caps.dwSize = sizeof(DDCAPS_guess);
        hel_caps.dwSize = sizeof(DDCAPS_guess);
        hr = ((GetCapsFn_guess)(*(void ***)g_direct_draw_guess)[11])(
            g_direct_draw_guess, &driver_caps, &hel_caps);
        if (hr < 0)
            return 0xffff3d3e;
        if ((driver_caps.dwCaps2 & EQGFX_DDCAPS2_PRIMARYGAMMA_GUESS) == 0)
            return 0xffff3d3e;
        if (level == 0.0f)
            level = 0.0001f;
        inverse_gamma = 1.0 / level;
        index = 0;
        cursor = ramp.green;
        do
        {
            EQ_CDECL(void, 0x1004EC8D, (float, void *))(
                (float)(EQ_CDECL(double, 0x10069DC9, (double, double))((double)index * 0.00390625, inverse_gamma) * 65535.0f),
                &ramp_out);
            index++;
            ramp_value = (unsigned short)ramp_out;
            cursor[-0x100] = ramp_value;
            *cursor = ramp_value;
            cursor[0x100] = ramp_value;
            cursor++;
        } while (index < 0x100);
        hr = ((QueryInterfaceFn_guess)(*(void ***)g_primary_surface)[0])(
            g_primary_surface, EQGFX_IID_GAMMA_CONTROL_GUESS, &gamma_control);
        if (hr < 0)
            return 0xffff3d3e;
        hr = ((SetGammaRampFn_guess)(*(void ***)gamma_control)[4])(gamma_control, 0, &ramp);
        if (hr < 0)
        {
            ((ReleaseFn_guess)(*(void ***)gamma_control)[2])(gamma_control);
            return 0xffff3d3e;
        }
        if (((ReleaseFn_guess)(*(void ***)gamma_control)[2])(gamma_control) < 0)
            return 0xffff3d3e;
        break;
    default:
        return 0;
    }
    g_gamma_level_guess = *(int *)&level;
    return 0;
}

// FUNCTION: EQGFX 0x1004a229
// Sets the renderer's baseline cached effect state and texture mode.
// __cdecl (callee pops 0 bytes; ECX is not an input), no arguments, void (callers never read EAX after the
// call). Clears the cached special-effects state (the original ANDs it with 0) and sets the cached mode to 2.
void __cdecl t3dResetRenderStateCache_guess(void)
{
    g_fog_enable_state &= 0;
    g_mode_state_guess = 2;
}

// FUNCTION: EQGFX 0x1004a23b
// Turns distance fog on or off on the render device, skipping the call when the cached value matches.
// __cdecl (callee pops 0 bytes; ECX is not an input), one int argument, void (callers never read EAX after the
// call). Caches the special-effects state at 0x100F3E34: while the cached value differs, tell the render device
// through vtable slot 0x50 (device, 0x1c, state) and update the cache.
void __cdecl setFogEnableState_guess(int state)
{
    void *device;

    if (g_fog_enable_state != state)
    {
        device = g_render_device_guess;
        ((D3DDev7SetRenderStateFn)(*(void ***)device)[20])(device, EQGFX_RS_FOGENABLE, state); // +0x50
        g_fog_enable_state = state;
    }
}

// FUNCTION: EQGFX 0x1004a25e
// Sets texture stage 0's alpha operation (2 = select arg1, 3/4 = modulate), skipping unchanged values.
// __cdecl (callee pops 0 bytes; ECX is not an input), one int argument, void (callers never read EAX after the
// call). Caches the mode at 0x100A25C0: mode 2 tells the render device 2, modes 3 and 4 tell it 4 through
// vtable slot 0x94 (device, 0, 4, value); any other mode only updates the cache.
/* fake match: separate equivalent cases preserve VC6's original decrement-chain switch lowering. */
void __cdecl t3dSetTextureAlphaOp_guess(int mode)
{
    void *device;

    if (mode == g_mode_state_guess)
        return;

    switch (mode)
    {
    case 2:
        device = g_render_device_guess;
        ((D3DDev7SetTextureStageStateFn)(*(void ***)device)[37])(device, 0, 4, 2);
        break;
    case 3:
        device = g_render_device_guess;
        ((D3DDev7SetTextureStageStateFn)(*(void ***)device)[37])(device, 0, 4, 4);
        break;
    case 4:
        device = g_render_device_guess;
        ((D3DDev7SetTextureStageStateFn)(*(void ***)device)[37])(device, 0, 4, 4);
        break;
    default:
        g_mode_state_guess = mode;
        return;
    }

    g_mode_state_guess = mode;
}

// FUNCTION: EQGFX 0x1004a29e
// Builds and submits triangles for a sphere used by collision-volume visualization.
int __cdecl renderCollisionSphere_guess(void *collisionVolume,
                                       CollisionSphereRenderState_guess *renderState,
                                       float *centerPosition, float radius)
{
    char helperScratch[80];
    float *sphereVertex;
    float *outputVertex;
    int *axisIndex;
    int vertexCount;
    int axis;
    float worldX;
    float worldY;
    float worldZ;

    axisIndex = g_collision_sphere_axis_order_guess;
    EQ_CDECL(void, 0x10035245,
             (CollisionSphereRenderState_guess *, float, float, float, float))(
        renderState, centerPosition[0], centerPosition[1], centerPosition[2], radius);
    do
    {
        sphereVertex = renderState->sphereVertices;
        outputVertex = renderState->outputVertices;
        vertexCount = 3;
        do
        {
            sphereVertex[0] = radius * g_collision_sphere_basis_guess[*axisIndex * 3] + centerPosition[0];
            sphereVertex[1] = radius * g_collision_sphere_basis_guess[*axisIndex * 3 + 1] + centerPosition[1];
            axis = *axisIndex;
            axisIndex++;
            worldZ = radius * g_collision_sphere_basis_guess[axis * 3 + 2] + centerPosition[2];
            sphereVertex[2] = worldZ;
            worldX = renderState->positionX + sphereVertex[0];
            worldY = sphereVertex[1] + renderState->positionY;
            worldZ = worldZ + renderState->positionZ;
            sphereVertex += 12;
            outputVertex[0] = worldY * renderState->xFromY +
                              worldZ * renderState->xFromZ + worldX * renderState->xFromX;
            outputVertex[1] = worldY * renderState->yFromY +
                              worldZ * renderState->yFromZ + worldX * renderState->yFromX;
            outputVertex[2] = worldY * renderState->zFromY +
                              worldZ * renderState->zFromZ + worldX * renderState->zFromX;
            ((int *)outputVertex)[10] = 0;
            ((int *)outputVertex)[11] = 0;
            vertexCount--;
            ((int *)outputVertex)[9] = 0xff;
            outputVertex += 12;
        } while (vertexCount != 0);
        renderState->sphereOffset[0] = 0.0f;
        renderState->sphereOffset[1] = 0.0f;
        renderState->sphereOffset[2] = 0.0f;
        EQ_CDECL(void, 0x100477B9,
                 (CollisionSphereRenderState_guess *, int, int, void *, int))(
            renderState, 0, 3, helperScratch, 0x10c0007);
    } while ((int)axisIndex < 0x100A2818);
    renderState->copiedStateValue = renderState->sourceStateValue;
    return 0;
}

// A collision volume the renderer tracks: its sphere center at +0x10 and radius at +0x34 (no include/ header covers it).
typedef struct CollisionVolume_guess
{
    unsigned char reserved_00[0x10];
    float centerPosition[3];
    unsigned char reserved_1c[0x18];
    float radius;
} CollisionVolume_guess;

// The object whose collision-volume pointer array sits at +0x17c (no include/ header covers it yet).
typedef struct CollisionVolumeOwner_guess
{
    unsigned char reserved_00[0x17c];
    CollisionVolume_guess **volumes;
} CollisionVolumeOwner_guess;

// FUNCTION: EQGFX 0x1004a413
// Paints the collision volume the owner's entry `*index` points at, when `paintEnabled` is nonzero.
int __cdecl t3dPaintCollisionVolume(void *owner, void *renderState, int *index, int paintEnabled)
{
    CollisionVolumeOwner_guess *volumeOwner;
    CollisionVolume_guess *volume;

    volumeOwner = (CollisionVolumeOwner_guess *)owner;
    volume = volumeOwner->volumes[*index];
    if (paintEnabled != 0)
    {
        EQ_CDECL(void, 0x1004A29E, (void *, void *, float *, float))(
            owner, renderState, volume->centerPosition, volume->radius);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004a448
// Clears a specified render region to black and resets its depth; also supports clearing the full view.
void __cdecl t3dClearRenderToBlack(ClearRenderRegion_guess *renderRegion)
{
    DDBltEffects_guess blitEffects;
    char savedViewport[24];
    RenderRect_guess destinationRect;
    int x;
    int y;
    int right;
    int bottom;
    int helperResult;
    float roundCoordinate;

    switch (g_render_mode_guess)
    {
    case 1:
        if (renderRegion == 0)
        {
            ((DeviceViewportFn_guess)(*(void ***)g_render_device_guess)[EQGFX_DEVICE_GET_VIEWPORT_SLOT_GUESS])(
                g_render_device_guess, savedViewport);
            ((DeviceViewportFn_guess)(*(void ***)g_render_device_guess)[EQGFX_DEVICE_SET_VIEWPORT_SLOT_GUESS])(
                g_render_device_guess, &g_viewport_guess);
            helperResult = EQ_CDECL(int, 0x10045DB0, (void *, int))(g_clear_helper_object_guess, 3);
            if (helperResult == -0x7789f7ea)
                EQ_CDECL(int, 0x10045DB0, (void *, int))(g_clear_helper_object_guess, 1);
            ((DeviceViewportFn_guess)(*(void ***)g_render_device_guess)[EQGFX_DEVICE_SET_VIEWPORT_SLOT_GUESS])(
                g_render_device_guess, savedViewport);
        }
        else
        {
            roundCoordinate = renderRegion->x;
            __asm { fld roundCoordinate }
            x = EQ_CDECL(int, 0x1004EC36, (void))();
            roundCoordinate = renderRegion->y;
            __asm { fld roundCoordinate }
            y = EQ_CDECL(int, 0x1004EC36, (void))();
            right = renderRegion->width + x;
            bottom = renderRegion->height + y;
            EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(&blitEffects, 0, 100);
            blitEffects.size = 100;
            blitEffects.fillColor = 0;
            destinationRect.left = x;
            destinationRect.top = y;
            destinationRect.right = right;
            destinationRect.bottom = bottom;
            ((SurfaceBltFn_guess)(*(void ***)g_back_buffer_surface)[5])(
                g_back_buffer_surface, &destinationRect, 0, 0, 0x01000400, &blitEffects);
            ((D3DDev7ClearFn)(*(void ***)g_render_device_guess)[10])(
                g_render_device_guess, 1, &destinationRect, 2, 0, 1.0f, 0);
        }
        break;
    case 2:
        ((GrBufferClearFn_guess)g_gr_buffer_clear_guess)(0, 0, g_clear_depth_guess);
        break;
    }
    return;
}

// FUNCTION: EQGFX 0x1004a570
// __cdecl, no arguments, void. Mode 1: full viewport around t3dD3DXContextClear (retry on 0x88760816); mode 2: grBufferClear.
void __cdecl t3dClearBackBuffer(void)
{
    char savedViewport[24];
    int helperResult;

    switch (g_render_mode_guess)
    {
    case 1:
        ((DeviceViewportFn_guess)(*(void ***)g_render_device_guess)[EQGFX_DEVICE_GET_VIEWPORT_SLOT_GUESS])(
            g_render_device_guess, savedViewport);
        ((DeviceViewportFn_guess)(*(void ***)g_render_device_guess)[EQGFX_DEVICE_SET_VIEWPORT_SLOT_GUESS])(
            g_render_device_guess, EQGFX_FULL_VIEWPORT_GUESS);
        helperResult = EQ_CDECL(int, 0x10045DB0, (void *, int))(g_clear_helper_object_guess, 3);
        if (helperResult == -0x7789f7ea)
            EQ_CDECL(int, 0x10045DB0, (void *, int))(g_clear_helper_object_guess, 1);
        ((DeviceViewportFn_guess)(*(void ***)g_render_device_guess)[EQGFX_DEVICE_SET_VIEWPORT_SLOT_GUESS])(
            g_render_device_guess, savedViewport);
        break;
    case 2:
        ((GrBufferClearFn_guess)g_gr_buffer_clear_guess)(0, 0, g_clear_depth_guess);
        return;
    }
    return;
}

// FUNCTION: EQGFX 0x1004a5e8
// Clears the back buffer, presents the frame, then clears it for the next frame.
void __cdecl t3dClearDisplayBuffers(void *displayContext)
{
    EQ_CDECL(void, 0x1004A570, (void *))(displayContext);
    EQ_CDECL(void, 0x100499DD, (void *))(displayContext);
    EQ_CDECL(void, 0x1004A570, (void *))(displayContext);
    return;
}

// FUNCTION: EQGFX 0x1004a604
// Turns auto-mipmapping on when requested and supported (mode 2 also via the force flag), else off.
// __cdecl (callee pops 0 bytes; ECX is only scratch), two int arguments, void (callers never read EAX).
// Recomputes the auto-mipmapping flag at 0x100FBA58 from the request, the capability flag and render mode 2.
void __cdecl t3dEnableAutoMipMapping(int enable, int forceEnable)
{
    int supported = g_auto_mipmapping_supported_guess;

    if ((enable == 1 && supported != 0)
        || (g_auto_mipmapping_enabled_guess &= 0, g_render_mode_guess == 2 && forceEnable == 1 && supported != 0))
    {
        g_auto_mipmapping_enabled_guess = 1;
    }
}

// FUNCTION: EQGFX 0x1004a637
// Starts the renderer for the game window: default fog/gamma/color key, then the D3DX or Glide device.
int __cdecl t3dInitializeDevice(T3DDeviceParameters_guess *deviceParameters, void *window,
                                int fullscreenMode, int colorDepth, void *deviceContext)
{
    unsigned int deviceCapabilities[4];
    unsigned int surfaceCapabilities;
    char errorDetail[0x100];
    WindowRect_guess windowRect;
    const char *failureText;
    int *sourceSettings;
    int *destinationSettings;
    int settingCount;
    int isFullscreen;
    int status;
    int result;
    int deviceMode;
    D3DInterface_guess *directDraw;

    EQ_CDECL(void, 0x10045FCD, (void))();
    EQ_CDECL(void, 0x10045E85, (void))();
    g_color_depth_guess = colorDepth;
    g_fog_near_guess = 0.0f;
    EQ_GLOBAL(int, 0x100FBBB0) = 1;
    EQGFX_GAMMA_LEVEL = 1.0f;
    g_fog_enabled_guess = 1;
    g_fog_color_raw_guess = 0x00ffffff;
    g_initialize_fog_far_bits_guess = T3D_DEFAULT_FOG_FAR_BITS;
    g_last_color_key_guess = 0x00ff00ff;
    g_render_width_guess = T3D_DEFAULT_RENDER_WIDTH;
    g_display_height_guess = T3D_DEFAULT_RENDER_HEIGHT;
    g_auto_mipmapping_enabled_guess = 0;
    EQ_GLOBAL(void *, 0x100F3FE8) = deviceContext;
    g_resource_total_guess = 0;
    g_resource_released_guess = 0;

    deviceMode = deviceParameters->deviceMode;
    if (deviceMode == 0)
    {
        g_render_mode_guess = 0;
        goto complete;
    }

    if (deviceMode != 1)
    {
        if (deviceMode == 2)
        {
            g_color_depth_guess = 0x10;
            g_last_fullscreen_mode_guess = 1;
            g_current_fullscreen_mode_guess = 1;
            g_initialize_resolution_mask_guess = deviceParameters->resolutionMask;
            g_initialize_render_window_guess = window;
            g_565_format_guess = 1;
            g_render_mode_guess = 2;
            g_initialize_window_state_guess = 0;
            sourceSettings = &deviceParameters->deviceMode;
            destinationSettings = EQ_ARRAY(int, 0x100FB8C0);
            for (settingCount = 0x57; settingCount != 0; settingCount--)
            {
                *destinationSettings = *sourceSettings;
                sourceSettings++;
                destinationSettings++;
            }

            if (deviceParameters->useDefaultAdapter == 0)
            {
                status = EQ_STDCALL(int, 0x10059A80,
                    (const void *, void **, const void *, void *))(
                        deviceParameters->adapterGuid, &g_direct_draw_guess,
                        (const void *)0x10078258, 0);
                if (status >= 0)
                {
                    goto configure_direct_draw;
                }
                failureText = EQ_ARRAY(char, 0x100A2A08);
            }
            else
            {
                status = EQ_STDCALL(int, 0x10059A80,
                    (const void *, void **, const void *, void *))(
                        0, &g_direct_draw_guess, (const void *)0x10078258, 0);
                if (status < 0)
                {
                    failureText = EQ_ARRAY(char, 0x100A2A40);
                    goto log_failure;
                }
                goto configure_direct_draw;
            }
        }
        else
        {
            failureText = EQ_ARRAY(char, 0x100A2A78);
            goto log_failure;
        }
        goto log_failure;
    }

    colorDepth = 0;
    if (g_t3d_device_enumeration_initialized_guess != 1)
    {
        status = EQ_CDECL(int, 0x1005A3F4, (void))();
        if (status != 0)
        {
            result = -1;
            goto return_result;
        }
        g_t3d_device_enumeration_initialized_guess = 1;
    }

    g_render_mode_guess = 1;
    g_resolution_mode_changed_guess = 0;
    if (deviceParameters->useDefaultAdapter != 0)
    {
        deviceContext = ((CreateICAFn_guess)EQ_GLOBAL(void *, 0x10076058))(
            EQ_ARRAY(char, 0x100A29C0), 0, 0, 0);
        g_previous_color_depth_guess =
            ((GetDeviceCapsFn_guess)EQ_GLOBAL(void *, 0x1007602C))(deviceContext, 0x0c);
        ((DeleteDCFn_guess)EQ_GLOBAL(void *, 0x10076060))(deviceContext);
        if (g_previous_color_depth_guess == g_color_depth_guess)
        {
            g_resolution_mode_changed_guess = 1;
        }
    }

    g_last_fullscreen_mode_guess = fullscreenMode;
    g_current_fullscreen_mode_guess = fullscreenMode;
    if (g_resolution_mode_changed_guess == 0)
    {
        g_current_fullscreen_mode_guess = 1;
    }
    g_initialize_window_state_guess = 0;
    g_initialize_render_window_guess = window;
    sourceSettings = &deviceParameters->deviceMode;
    destinationSettings = EQ_ARRAY(int, 0x100FB8C0);
    for (settingCount = 0x57; settingCount != 0; settingCount--)
    {
        *destinationSettings = *sourceSettings;
        sourceSettings++;
        destinationSettings++;
    }
    g_initialize_resolution_mask_guess = deviceParameters->resolutionMask;
    isFullscreen = g_current_fullscreen_mode_guess != 1;
    if (isFullscreen != 0)
    {
        ((SetWindowLongAFn_guess)EQ_GLOBAL(void *, 0x10076200))(
            window, T3D_WINDOW_STYLE_INDEX, T3D_POPUP_WINDOW_STYLE);
        windowRect.right = (int)g_render_width_guess;
        windowRect.bottom = g_display_height_guess;
        windowRect.left = 0;
        windowRect.top = 0;
        ((AdjustWindowRectFn_guess)EQ_GLOBAL(void *, 0x10076208))(
            &windowRect, T3D_POPUP_WINDOW_STYLE, 0);
        ((SetWindowPosFn_guess)EQ_GLOBAL(void *, 0x10076204))(
            window, T3D_TOPMOST_WINDOW, 0, 0,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top, 2);
    }
    else
    {
        ((SetWindowLongAFn_guess)EQ_GLOBAL(void *, 0x10076200))(
            window, T3D_WINDOW_STYLE_INDEX, T3D_WINDOWED_STYLE);
    }
    ((SetCursorFn_guess)EQ_GLOBAL(void *, 0x100761F4))(0);
    status = EQ_STDCALL(int, 0x1005AC39,
        (void *, int, void *, int, int, int, unsigned int, int, int,
         unsigned int, int, unsigned int, int *))(
            g_initialize_d3d_instance_guess, !isFullscreen,
            g_initialize_render_window_guess, 0, g_color_depth_guess,
            0, 0xffffffff, 0, 1, g_render_width_guess,
            g_display_height_guess, 0xffffffff, &colorDepth);
    if (status < 0)
    {
        EQ_STDCALL(void, 0x1005AE37, (int, unsigned int, char *))(
            status, 0x100, errorDetail);
        EQ_CDECL(void, 0x100509EE, (const char *))(
            EQ_ARRAY(char, 0x100A298C));
        EQ_CDECL(void, 0x100509EE, (const char *))(errorDetail);
        result = -1;
        goto return_result;
    }

    g_initialize_context_guess = colorDepth;
    g_direct_draw_guess = EQ_CDECL(void *, 0x10045D5B, (int))(colorDepth);
    g_direct3d7 = EQ_CDECL(void *, 0x10045D66, (int))(colorDepth);
    g_render_device_guess = EQ_CDECL(void *, 0x10045D71, (int))(colorDepth);
    g_back_buffer_surface = EQ_CDECL(void *, 0x10045D87, (int, int))(
        colorDepth, 0);
    g_primary_surface = EQ_CDECL(void *, 0x10045D7C, (int))(colorDepth);
    EQ_CDECL(void, 0x10045DB0, (int, int))(g_initialize_context_guess, 1);
    EQ_STDCALL(int, 0x1005AAFB, (void *, int, unsigned int *, int, int))(
        g_initialize_d3d_instance_guess, 0,
        EQ_ARRAY(unsigned int, 0x100FBA98), 0, 0);
    status = EQ_CDECL(int, 0x1004CAE0, (int))(g_color_depth_guess);
    if (status < 0)
    {
        failureText = EQ_ARRAY(char, 0x100A2958);
        goto log_failure;
    }

    status = EQ_CDECL(int, 0x1004B51C, (void))();
    if (status < 0)
    {
        failureText = EQ_ARRAY(char, 0x100A2924);
        goto log_failure;
    }

    if ((g_initialize_texture_flags_guess & 0x0d) == 0)
    {
        g_100fbb9c_guess = 2;
        g_initialize_texture_state1_guess = 2;
        g_initialize_texture_state3_guess = 2;
        if ((g_initialize_clear_depth_flags_guess & 4) == 0)
        {
            g_100fbba4_guess = 2;
            g_initialize_texture_state4_guess = 0;
            goto configure_mipmapping;
        }
        g_100fbba4_guess = 3;
        g_initialize_texture_state4_guess = 1;
    }
    else
    {
        g_100fbb9c_guess = 4;
        g_initialize_texture_state1_guess = 4;
        g_100fbba4_guess = 4;
        g_initialize_texture_state3_guess = 4;
        g_initialize_texture_state4_guess = 0;
    }

configure_mipmapping:
    if ((g_initialize_auto_flags_guess & 0x20) != 0
        || (g_initialize_auto_flags_guess & 8) != 0)
    {
        EQGFX_MULTI_TMU_FLAG =
            (unsigned int)((g_initialize_auto_flags_guess & 0x20) != 0);
        g_auto_mipmapping_supported_guess = 1;
    }
    EQ_CDECL(void, 0x1004B274, (void))();
    EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(
        deviceCapabilities, 0, 0x10);
    deviceCapabilities[0] = 0x5000;
    directDraw = (D3DInterface_guess *)g_direct_draw_guess;
    ((DDGetAvailableVidMemFn)directDraw->vtable[0x5c / 4])(
        g_direct_draw_guess, deviceCapabilities, &surfaceCapabilities,
        EQ_ARRAY(unsigned int, 0x100FBB88));
    goto complete;

configure_direct_draw:
    directDraw = (D3DInterface_guess *)g_direct_draw_guess;
    status = ((DDSetCooperativeLevelFn_guess)directDraw->vtable[0x50 / 4])(
        g_direct_draw_guess, window, 0x808);
    if (status >= 0)
    {
        EQ_CDECL(int, 0x1004C795, (int))(1);
        goto complete;
    }
    failureText = EQ_ARRAY(char, 0x100A29C8);
    goto log_failure;

log_failure:
    EQ_CDECL(void, 0x100509EE, (const char *))(failureText);
    result = -1;
    goto return_result;

complete:
    EQ_CDECL(void, 0x100509EE, (const char *))(
        EQ_ARRAY(char, 0x100A28F4));
    result = 0;

return_result:
    return result;
}

// FUNCTION: EQGFX 0x1004aa80
// Rebuilds drawing setup after device resources are released so the game can render again.
// __cdecl (callee pops 0 bytes; ECX is only scratch), no arguments. Mode 1 releases the device resources and
// rebuilds the viewport and state; returns 0 for modes 1 and 2 and -1 for any other mode.
int __cdecl t3dReinitializeDevice(void)
{
    switch (g_render_mode_guess)
    {
    case 1:
        EQ_CDECL(void, 0x10045FDC, (void *))(&g_render_mode_guess);
        EQ_CDECL(int, 0x1004B51C, (void))();
        EQ_CDECL(int, 0x1004B274, (void))();
        if (g_resource_total_guess != g_resource_released_guess)
        {
            g_resource_total_guess = g_resource_total_guess - g_resource_released_guess;
        }
        g_resource_released_guess = 0;
        break;
    case 2:
        break;
    default:
        return -1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004aac1
// Shuts down the active renderer and releases its display resources.
// __cdecl, no arguments. Mode 1 releases the D3D device, surfaces and DirectDraw object; mode 2 releases
// the DirectDraw object and shuts Glide down. Returns 0 for modes 1 and 2 and -1 for any other mode.
int __cdecl t3dReleaseDevice(void)
{
    typedef void (__stdcall *GrGlideShutdownFn_guess)(void);

    EQ_CDECL(void, 0x10048FAD, (void))();
    EQ_CDECL(void, 0x100491CE, (void))();
    switch (g_render_mode_guess)
    {
    case 1:
        ((D3DDev7SetTextureFn)(*(void ***)g_render_device_guess)[35])(g_render_device_guess, 0, 0);
        ((ReleaseFn_guess)(*(void ***)g_back_buffer_surface)[2])(g_back_buffer_surface);
        g_back_buffer_surface = 0;
        ((ReleaseFn_guess)(*(void ***)g_primary_surface)[2])(g_primary_surface);
        g_primary_surface = 0;
        ((ReleaseFn_guess)(*(void ***)g_render_device_guess)[2])(g_render_device_guess);
        g_render_device_guess = 0;
        ((ReleaseFn_guess)(*(void ***)g_direct3d7)[2])(g_direct3d7);
        g_direct3d7 = 0;
        ((ReleaseFn_guess)(*(void ***)g_direct_draw_guess)[2])(g_direct_draw_guess);
        g_direct_draw_guess = 0;
        EQ_CDECL(int, 0x10045DCE, (void *))(g_clear_helper_object_guess);
        g_clear_helper_object_guess = 0;
        EQ_CDECL(void, 0x1005A4CD, (void))();
        g_t3d_device_enumeration_initialized_guess = 0;
        break;
    case 2:
        EQ_CDECL(void, 0x1004CAAA, (void))();
        ((ReleaseFn_guess)(*(void ***)g_direct_draw_guess)[2])(g_direct_draw_guess);
        EQ_GLOBAL(GrGlideShutdownFn_guess, 0x100F3F24)();
        break;
    default:
        return -1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004ab7e
// Checks whether any requested screen-size option is supported.
// __cdecl (the argument is on the stack; the original's RET pops 0 bytes), one unsigned bitmask argument.
// Nonzero (0/1) when the device's resolution-support mask has any of the requested bits.
int __cdecl t3dDeviceSupportsResolution(unsigned int resolutions)
{
    return (g_initialize_resolution_mask_guess & resolutions) != 0;
}

// FUNCTION: EQGFX 0x1004ab8e
// Changes the display mode to the resolution and 16/32-bit depth a mask bit names, if the device supports it.
int __cdecl t3dChangeDeviceResolution(unsigned int resolutionMask)
{
    unsigned int height;
    unsigned int colorDepth;
    int result;

    result = 0;
    if (EQ_CDECL(int, 0x1004AB7E, (unsigned int))(resolutionMask) == 0)
    {
        EQ_CDECL(void, 0x100509EE, (const char *))(EQ_ARRAY(char, 0x100A2AA4));
        result = T3D_RESOLUTION_UNSUPPORTED;
    }
    else
    {
        colorDepth = 0x10;
        if (resolutionMask <= 0x10000)
        {
            if (resolutionMask == 0x10000)
            {
                resolutionMask = 0x280;
                height = 0x1e0;
                colorDepth = 0x20;
            }
            else if (resolutionMask <= 0x20)
            {
                switch (resolutionMask)
                {
                case 0x20:
                    resolutionMask = 0x480;
                    height = 0x360;
                    break;
                case 1:
                    resolutionMask = 0x280;
                    height = 0x1e0;
                    break;
                case 2:
                    resolutionMask = 0x320;
                    height = 0x258;
                    break;
                case 4:
                    resolutionMask = 0x400;
                    height = 0x300;
                    break;
                case 8:
                    resolutionMask = 0x500;
                    height = 0x400;
                    break;
                case 0x10:
                    resolutionMask = 0x640;
                    height = 0x4b0;
                    break;
                default:
                    height = resolutionMask;
                    colorDepth = resolutionMask;
                    break;
                }
            }
            else if (resolutionMask == 0x40)
            {
                resolutionMask = 0x780;
                height = 0x5a0;
            }
            else if (resolutionMask == 0x80)
            {
                resolutionMask = 0x800;
                height = 0x600;
            }
            else if (resolutionMask == 0x100)
            {
                resolutionMask = 0x500;
                height = 0x2d0;
            }
            else if (resolutionMask == 0x200)
            {
                resolutionMask = 0x640;
                height = 900;
            }
            else
            {
                height = resolutionMask;
                colorDepth = resolutionMask;
            }
        }
        else if (resolutionMask <= 0x200000)
        {
            if (resolutionMask == 0x200000)
            {
                resolutionMask = 0x480;
                height = 0x360;
                colorDepth = 0x20;
            }
            else if (resolutionMask == 0x20000)
            {
                resolutionMask = 0x320;
                height = 600;
                colorDepth = 0x20;
            }
            else if (resolutionMask == 0x40000)
            {
                resolutionMask = 0x400;
                height = 0x300;
                colorDepth = 0x20;
            }
            else if (resolutionMask == 0x80000)
            {
                resolutionMask = 0x500;
                height = 0x400;
                colorDepth = 0x20;
            }
            else if (resolutionMask == 0x100000)
            {
                resolutionMask = 0x640;
                height = 0x4b0;
                colorDepth = 0x20;
            }
            else
            {
                height = resolutionMask;
                colorDepth = resolutionMask;
            }
        }
        else if (resolutionMask == 0x400000)
        {
            resolutionMask = 0x780;
            height = 0x5a0;
            colorDepth = 0x20;
        }
        else if (resolutionMask == 0x800000)
        {
            resolutionMask = 0x800;
            height = 0x600;
            colorDepth = 0x20;
        }
        else if (resolutionMask == 0x1000000)
        {
            resolutionMask = 0x500;
            height = 0x2d0;
            colorDepth = 0x20;
        }
        else if (resolutionMask == 0x2000000)
        {
            resolutionMask = 0x640;
            height = 900;
            colorDepth = 0x20;
        }
        else
        {
            height = resolutionMask;
            colorDepth = resolutionMask;
        }

        if (g_render_width_guess != resolutionMask
            || g_display_height_guess != height
            || g_color_depth_guess != colorDepth)
        {
            if (g_render_mode_guess == 1)
            {
                g_current_fullscreen_mode_guess = g_last_fullscreen_mode_guess;
                if (g_previous_color_depth_guess != colorDepth)
                    g_current_fullscreen_mode_guess = 1;
                g_resolution_mode_changed_guess = (g_previous_color_depth_guess == colorDepth);
                g_color_depth_guess = colorDepth;
                g_render_width_guess = resolutionMask;
                g_display_height_guess = height;
                EQ_CDECL(void, 0x1004AF92, (void))();
            }
            else if (g_render_mode_guess == 2)
            {
                EQ_CDECL(void, 0x1004CAAA, (void))();
                g_color_depth_guess = 0x10;
                g_render_width_guess = resolutionMask;
                g_display_height_guess = height;
                EQ_CDECL(void, 0x1004C795, (int))(0);
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1004b51c
// Sets the screen area used for drawing to the current render dimensions.
// __cdecl (callee pops 0 bytes; ECX is not an input), no arguments. Rebuilds the viewport at 0x100FBA70
// (x, y zero; the render size; minZ 0.0f, maxZ 1.0f), hands it to the render device (vtable slot 0x34);
// returns 0 when the device accepted it and -1 otherwise.
int __cdecl t3dD3DInitializeViewport_guess(void)
{
    D3DViewport7_guess *viewport = &g_viewport_guess;
    void *device;
    int hr;

    EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(viewport, 0, 0x18);
    g_viewport_guess.x &= 0;
    g_viewport_guess.y &= 0;
    g_viewport_guess.width = g_render_width_guess;
    g_viewport_guess.height = g_render_height_guess;
    g_viewport_guess.minZ = 0.0f;
    g_viewport_guess.maxZ = 1.0f;
    device = g_render_device_guess;
    hr = ((SetViewportFn_guess)(*(void ***)device)[13])(device, viewport);
    return (hr >= 0) - 1;
}
// FUNCTION: EQGFX 0x1004adff
// Resolution code: a constant and a mask, chosen by width and depth (height too at 1280 and 1600).
int __cdecl t3dGetDeviceResolution(void)
{
    switch (g_render_width_guess)
    {
    case 0x280:
        return ((g_color_depth_guess != 0x10) - 1 & 0xffff0001) + 0x10000;
    case 0x320:
        return ((g_color_depth_guess != 0x10) - 1 & 0xfffe0002) + 0x20000;
    case 0x400:
        return ((g_color_depth_guess != 0x10) - 1 & 0xfffc0004) + 0x40000;
    case 0x480:
        return ((g_color_depth_guess != 0x10) - 1 & 0xffe00020) + 0x200000;
    case 0x500:
        if (g_color_depth_guess == 0x10)
        {
            return ((g_display_height_guess != 0x400) - 1 & 0xffffff08) + 0x100;
        }
        return ((g_display_height_guess != 0x400) - 1 & 0xff080000) + 0x1000000;
    case 0x640:
        if (g_color_depth_guess == 0x10)
        {
            return ((g_display_height_guess != 0x4b0) - 1 & 0xfffffe10) + 0x200;
        }
        return ((g_display_height_guess != 0x4b0) - 1 & 0xfe100000) + 0x2000000;
    case 0x780:
        return ((g_color_depth_guess != 0x10) - 1 & 0xffc00040) + 0x400000;
    case 0x800:
        return ((g_color_depth_guess != 0x10) - 1 & 0xff800080) + 0x800000;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004AF54
// Chooses the next fullscreen setting, then applies the matching video mode.
int __cdecl t3dSwitchMode(void)
{
    int status;

    if (g_render_mode_guess == 1)
    {
        if (g_current_fullscreen_mode_guess == 0)
        {
            g_current_fullscreen_mode_guess = 1;
        }
        else if (g_resolution_mode_changed_guess != 0)
        {
            g_current_fullscreen_mode_guess = 0;
        }
        status = EQ_CDECL(int, 0x1004AF92, (void))();
        if (status < 0)
        {
            return T3D_RESOLUTION_UNSUPPORTED;
        }
    }
    return 0;
}

#define g_mode_switch_active_guess       EQ_GLOBAL(int, 0x100F3FEC)
#define g_direct3d7              EQ_GLOBAL(void *, 0x100FBA68)
#define g_d3d_instance_guess             EQ_GLOBAL(void *, 0x100FB8C4)
#define g_created_surface_guess          EQ_GLOBAL(void *, 0x100FBB88)
#define g_set_window_long_a_fn_guess     EQ_GLOBAL(void *, 0x10076200)
#define g_set_window_pos_fn_guess        EQ_GLOBAL(void *, 0x10076204)
#define g_adjust_window_rect_fn_guess    EQ_GLOBAL(void *, 0x10076208)
#define g_set_cursor_fn_guess            EQ_GLOBAL(void *, 0x100761F4)

// FUNCTION: EQGFX 0x1004AF92
// Recreates the Direct3D context, surfaces and viewport for the new display mode (size, depth, fullscreen).
int __cdecl t3dSwitchD3DVideoMode(void)
{
    int result;
    int status;
    int context;
    int fullScreen;
    int releaseResult;
    char errorMessage[256];
    unsigned int surfaceDescription[4];
    void *createdSurface;
    WindowRect_guess windowRect;

    if (g_mode_switch_active_guess == 1)
    {
        EQ_CDECL(void, 0x100509EE, (const char *))(EQ_ARRAY(char, 0x100A2BCC));
        return 0;
    }
    g_mode_switch_active_guess = 1;
    if (g_render_mode_guess == 1)
    {
        context = 0;
        EQ_CDECL(void, 0x10045FDC, (int *))(&g_render_mode_guess);
        if (g_render_device_guess != 0)
        {
            D3DInterface_guess *renderDevice = (D3DInterface_guess *)g_render_device_guess;
            ((D3DDev7SetTextureFn)renderDevice->vtable[0x8c / 4])(
                g_render_device_guess, 0, 0);
        }
        if (g_back_buffer_surface != 0)
        {
            D3DInterface_guess *lockSurface = (D3DInterface_guess *)g_back_buffer_surface;
            ((D3DReleaseFn_guess)lockSurface->vtable[2])(g_back_buffer_surface);
            g_back_buffer_surface = 0;
        }
        if (g_primary_surface != 0)
        {
            D3DInterface_guess *gammaSurface = (D3DInterface_guess *)g_primary_surface;
            ((D3DReleaseFn_guess)gammaSurface->vtable[2])(g_primary_surface);
            g_primary_surface = 0;
        }
        if (g_render_device_guess != 0)
        {
            D3DInterface_guess *renderDevice = (D3DInterface_guess *)g_render_device_guess;
            ((D3DReleaseFn_guess)renderDevice->vtable[2])(g_render_device_guess);
            g_render_device_guess = 0;
        }
        if (g_direct3d7 != 0)
        {
            void *direct3d7 = g_direct3d7;
            D3DInterface_guess *direct3d7Object = (D3DInterface_guess *)direct3d7;
            ((D3DReleaseFn_guess)direct3d7Object->vtable[2])(direct3d7);
            g_direct3d7 = 0;
        }
        if (g_direct_draw_guess != 0)
        {
            D3DInterface_guess *directDraw = (D3DInterface_guess *)g_direct_draw_guess;
            ((D3DReleaseFn_guess)directDraw->vtable[2])(g_direct_draw_guess);
            g_direct_draw_guess = 0;
        }
        if (g_clear_helper_object_guess != 0)
        {
            releaseResult = EQ_CDECL(int, 0x10045DCE, (int))(g_clear_helper_object_guess);
            if (releaseResult != 0)
            {
                EQ_CDECL(void, 0x100509EE, (const char *))(EQ_ARRAY(char, 0x100A2B88));
                releaseResult = EQ_CDECL(int, 0x10045DCE, (int))(g_clear_helper_object_guess);
                while (releaseResult != 0)
                {
                    releaseResult = EQ_CDECL(int, 0x10045DCE, (int))(g_clear_helper_object_guess);
                }
            }
            g_clear_helper_object_guess = 0;
        }
        fullScreen = g_current_fullscreen_mode_guess != 1;
        if (fullScreen != 0)
        {
            ((SetWindowLongAFn_guess)g_set_window_long_a_fn_guess)(
                g_initialize_render_window_guess, -0x10, 0x10ca0000);
            windowRect.right = (int)g_render_width_guess;
            windowRect.bottom = g_display_height_guess;
            windowRect.left = 0;
            windowRect.top = 0;
            ((AdjustWindowRectFn_guess)g_adjust_window_rect_fn_guess)(
                &windowRect, 0x10ca0000, 0);
            ((SetWindowPosFn_guess)g_set_window_pos_fn_guess)(
                g_initialize_render_window_guess, (void *)0xfffffffe, 0, 0,
                windowRect.right - windowRect.left,
                windowRect.bottom - windowRect.top, 2);
        }
        ((SetCursorFn_guess)g_set_cursor_fn_guess)(0);
        status = EQ_STDCALL(int, 0x1005AC39,
            (void *, int, void *, int, int, int, unsigned int, int, int,
             unsigned int, int, unsigned int, int *))(
                 g_d3d_instance_guess, !fullScreen,
                 g_initialize_render_window_guess, 0, g_color_depth_guess,
                 0, 0xffffffff, 0, 1, g_render_width_guess,
                 g_display_height_guess, 0xffffffff, &context);
        if (status < 0)
        {
            if (status == (int)0x8876017c)
            {
                g_mode_switch_active_guess = 0;
                return -0x2a;
            }
            EQ_STDCALL(void, 0x1005AE37, (int, unsigned int, char *))(
                status, 0x100, errorMessage);
            EQ_CDECL(void, 0x100509EE, (const char *))(EQ_ARRAY(char, 0x100A2B54));
            EQ_CDECL(void, 0x100509EE, (const char *))(errorMessage);
        }
        else
        {
            g_clear_helper_object_guess = (void *)context;
            EQ_CDECL(void, 0x10045DB0, (int, int))(context, 1);
            g_direct_draw_guess = EQ_CDECL(void *, 0x10045D5B, (int))(g_clear_helper_object_guess);
            g_direct3d7 = EQ_CDECL(void *, 0x10045D66, (int))(g_clear_helper_object_guess);
            g_render_device_guess = EQ_CDECL(void *, 0x10045D71, (int))(g_clear_helper_object_guess);
            g_back_buffer_surface = EQ_CDECL(void *, 0x10045D87, (int, int))(
                g_clear_helper_object_guess, 0);
            g_primary_surface = EQ_CDECL(void *, 0x10045D7C, (int))(g_clear_helper_object_guess);
            EQ_STDCALL(int, 0x1005AAFB, (void *, int, unsigned int *, int, int))(
                 g_d3d_instance_guess, 0,
                &g_fog_color_value_guess, 0, 0);
            status = EQ_CDECL(int, 0x1004CAE0, (int))(g_color_depth_guess);
            if (status >= 0)
            {
                status = EQ_CDECL(int, 0x1004B51C, (void))();
                if (status >= 0)
                {
                    EQ_CDECL(void, 0x1004B274, (void))();
                    EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(
                        surfaceDescription, 0, 0x10);
                    surfaceDescription[0] = 0x5000;
                    ((DDGetAvailableVidMemFn)((D3DInterface_guess *)g_direct_draw_guess)->vtable[0x5c / 4])(
                        g_direct_draw_guess, surfaceDescription, &createdSurface,
                        &g_created_surface_guess);
                    goto success;
                }
                EQ_CDECL(void, 0x100509EE, (const char *))(EQ_ARRAY(char, 0x100A2AE4));
            }
            else
            {
                EQ_CDECL(void, 0x100509EE, (const char *))(EQ_ARRAY(char, 0x100A2B1C));
            }
        }
        result = -1;
    }
    else
    {
        if (g_render_mode_guess != 2)
        {
            g_mode_switch_active_guess = 0;
            return -1;
        }
success:
        result = 0;
    }
    g_mode_switch_active_guess = 0;
    return result;
}

// FUNCTION: EQGFX 0x1004B274
// Establishes baseline scene rendering settings and reapplies the current display and texture options.
// __cdecl (callee pops 0 bytes; ECX is not an input), no arguments, returns 0 (callers never read EAX). Resets the render states, the texture stages and the fog, then sets the mip LOD bias of both stages.
int __cdecl t3dD3DSetDefaultRenderState(void)
{
    float lodBias;

    EQ_CDECL(void, 0x1004A229, (void))();
    ((SetTextureFn_guess)(*(void ***)g_render_device_guess)[35])(g_render_device_guess, 0, 0);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 7, 1);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0xe, 1);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x17, 4);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x19, 8);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0xf, 0);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x18, 0xc0);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 9, 2);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 4, 1);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x1a, 1);
    EQ_CDECL(void, 0x1004A25E, (int))(g_100fbb9c_guess);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x88, 0);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x89, 0);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x8a, 0);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 0, 0x10, 2);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 0, 0x11, 2);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 0, 0x12, 2);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 0x10, 2);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 0x11, 2);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 0x12, 2);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 2, 2);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 1, 0xe);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 3, 1);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 4, 3);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 6, 0);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 0xb, 1);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 1, 1);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 4, 1);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 2, 1, 1);
    ((D3DDev7SetTextureStageStateFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 2, 4, 1);
    ((D3DDev7SetRenderStateFn)(*(void ***)g_render_device_guess)[20])(g_render_device_guess, 0x3c, 0x80808080);
    EQ_CDECL(int, 0x1004A0AA, (float))(EQGFX_GAMMA_LEVEL);
    EQ_CDECL(void, 0x10049A52, (unsigned int))(g_last_color_key_guess);
    EQ_CDECL(int, 0x10049F0D, (int, float, float, unsigned int, unsigned int, unsigned int))(
        g_fog_enabled_guess, g_fog_near_guess, g_fog_far_guess, g_fog_color_raw_guess,
        (g_fog_color_raw_guess >> 8) & 0xff, g_fog_color_raw_guess >> 16);
    EQ_CDECL(void, 0x1004A604, (int, int))(g_auto_mipmapping_enabled_guess, g_auto_mipmapping_enabled_guess);
    lodBias = -1.0f;
    ((D3DDev7SetTextureStageStateFloatFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 0, 0x13, lodBias);
    ((D3DDev7SetTextureStageStateFloatFn)(*(void ***)g_render_device_guess)[37])(g_render_device_guess, 1, 0x13, lodBias);
    return 0;
}

// FUNCTION: EQGFX 0x1004b579
// Lists the video devices (Glide board, then D3D adapters) with their supported 16/32-bit resolutions; max 16.
int __cdecl t3dEnumDevices(int deviceTableAddress, int glideParameter)
{
    int deviceIndex;
    int deviceLimit;
    int deviceResult;
    int modeIndex;
    int modeCount;
    int modeResult;
    T3DDeviceDescription_guess deviceDescription;
    T3DDisplayMode_guess displayMode;

    g_t3d_device_count_guess = 0;
    g_t3d_device_records_guess = (T3DDeviceInfo_guess *)deviceTableAddress;
    EQ_CDECL(void, 0x1004BBE9, (int, int))(deviceTableAddress, glideParameter);
    if (g_t3d_device_enumeration_initialized_guess != 1)
    {
        deviceResult = EQ_CDECL(int, 0x1005A3F4, (void))();
        if (deviceResult != 0)
            return g_t3d_device_count_guess;
        g_t3d_device_enumeration_initialized_guess = 1;
    }
    deviceLimit = EQ_CDECL(int, 0x1005A597, (void))();
    deviceIndex = 0;
    if (0 < deviceLimit)
    {
        do
        {
            modeIndex = 0;
            deviceResult = EQ_STDCALL(int, 0x1005A64D, (int, T3DDeviceDescription_guess *))(
                deviceIndex, &deviceDescription);
            if ((deviceResult == 0) && (deviceDescription.deviceClass == T3D_DEVICE_CLASS_PRIMARY))
            {
                g_t3d_device_records_guess[g_t3d_device_count_guess].adapterIndex = deviceIndex;
                EQ_CDECL(char *, 0x100688E0, (char *, const char *, unsigned int))(
                    g_t3d_device_records_guess[g_t3d_device_count_guess].name,
                    deviceDescription.name, 0x40);
                g_t3d_device_records_guess[g_t3d_device_count_guess].name[0x3f] = 0;
                deviceResult = EQ_CDECL(int, 0x100689E0, (const char *, const char *, unsigned int))(
                    deviceDescription.name, EQ_ARRAY(char, 0x100A2BFC), 0x16);
                if (deviceResult == 0)
                    g_t3d_device_records_guess[g_t3d_device_count_guess].name[0x17] = 0;
                g_t3d_device_records_guess[g_t3d_device_count_guess].deviceType = 1;
                EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
                    g_t3d_device_records_guess[g_t3d_device_count_guess].driverIdentifier,
                    deviceDescription.driverIdentifier, 0x10);
                EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
                    g_t3d_device_records_guess[g_t3d_device_count_guess].deviceIdentifier,
                    deviceDescription.deviceIdentifier, 0x10);
                if (deviceDescription.hardwareAcceleration == 0)
                    g_t3d_device_records_guess[g_t3d_device_count_guess].hardwareAcceleration = 0;
                else
                    g_t3d_device_records_guess[g_t3d_device_count_guess].hardwareAcceleration = 1;
                g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags = 0;
                modeCount = EQ_STDCALL(int, 0x1005A789, (int, int))(deviceIndex, 0);
                if (0 < modeCount)
                {
                    do
                    {
                        modeResult = EQ_STDCALL(int, 0x1005A86C, (int, int, int, T3DDisplayMode_guess *))(
                            deviceIndex, 0, modeIndex, &displayMode);
                        if (modeResult == 0)
                        {
                            if (displayMode.bitDepth == 0x10)
                            {
                                if (displayMode.width == 0x280)
                                {
                                    if (displayMode.height == 0x1e0)
                                        g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags |= 1;
                                }
                                else if (displayMode.width == 800)
                                {
                                    if (displayMode.height == 600)
                                        g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags |= 2;
                                }
                                else if (displayMode.width == 0x400)
                                {
                                    if (displayMode.height == 0x300)
                                        g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags |= 4;
                                }
                                else if (displayMode.width == 0x500)
                                {
                                    if (displayMode.height == 0x400)
                                        g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags |= 8;
                                    else if (displayMode.height == 0x2d0)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[1] |= 1;
                                }
                                else if (displayMode.width == 0x640)
                                {
                                    if (displayMode.height == 0x4b0)
                                        g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags |= 0x10;
                                    else if (displayMode.height == 900)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[1] |= 2;
                                }
                                else if (displayMode.width == 0x480)
                                {
                                    if (displayMode.height == 0x360)
                                        g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags |= 0x20;
                                }
                                else if (displayMode.width == 0x780)
                                {
                                    if (displayMode.height == 0x5a0)
                                        g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags |= 0x40;
                                }
                                else if ((displayMode.width == 0x800) && (displayMode.height == 0x5f6))
                                {
                                    ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[0] |= 0x80;
                                }
                            }
                            else if (displayMode.bitDepth == 0x20)
                            {
                                if (displayMode.width == 0x280)
                                {
                                    if (displayMode.height == 0x1e0)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 1;
                                }
                                else if (displayMode.width == 800)
                                {
                                    if (displayMode.height == 600)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 2;
                                }
                                else if (displayMode.width == 0x400)
                                {
                                    if (displayMode.height == 0x300)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 4;
                                }
                                else if (displayMode.width == 0x500)
                                {
                                    if (displayMode.height == 0x400)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 8;
                                    else if (displayMode.height == 0x2d0)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[3] |= 1;
                                }
                                else if (displayMode.width == 0x640)
                                {
                                    if (displayMode.height == 0x4b0)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 0x10;
                                    else if (displayMode.height == 900)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[3] |= 2;
                                }
                                else if (displayMode.width == 0x480)
                                {
                                    if (displayMode.height == 0x360)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 0x20;
                                }
                                else if (displayMode.width == 0x780)
                                {
                                    if (displayMode.height == 0x5a0)
                                        ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 0x40;
                                }
                                else if ((displayMode.width == 0x800) && (displayMode.height == 0x5f6))
                                {
                                    ((unsigned char *)&g_t3d_device_records_guess[g_t3d_device_count_guess].resolutionFlags)[2] |= 0x80;
                                }
                            }
                        }
                        modeIndex = modeIndex + 1;
                    } while (modeIndex < modeCount);
                }
                g_t3d_device_count_guess = g_t3d_device_count_guess + 1;
                if (0xf < g_t3d_device_count_guess)
                {
                    EQ_CDECL(void, 0x1005A4CD, (void))();
                    g_t3d_device_enumeration_initialized_guess = 0;
                    return g_t3d_device_count_guess;
                }
            }
            deviceIndex = deviceIndex + 1;
        } while (deviceIndex < deviceLimit);
    }
    EQ_CDECL(void, 0x1005A4CD, (void))();
    g_t3d_device_enumeration_initialized_guess = 0;
    return g_t3d_device_count_guess;
}
