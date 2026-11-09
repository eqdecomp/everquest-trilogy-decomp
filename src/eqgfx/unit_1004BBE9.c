// unit_1004BBE9.c - eqgfx_dx7.dll, the address run 0x1004BBE9..0x1004E435 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <string.h>

typedef struct
{
    unsigned int tag; // 0x0
    unsigned char unk4[6]; // 0x4
    unsigned short fieldA; // 0xa
    unsigned char unkC[2]; // 0xc
    unsigned short fieldE; // 0xe
    unsigned char unk10[0x10]; // 0x10
    unsigned int field20; // 0x20
} Record_guess;

// BITMAPFILEHEADER layout, packed to 2: the type word at 0, the size dword at 2 (unaligned), 14 bytes.
#pragma pack(push, 2)
typedef struct BitmapFileHeader_guess
{
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offBits;
} BitmapFileHeader_guess;
#pragma pack(pop)

__int64 __emul(int left, int right);
#pragma intrinsic(__emul)

typedef struct EqgfxFix16SqrtTable_guess
{
    unsigned char reserved[0x2000];
    unsigned short evenScale[0x1000];
    unsigned short oddScale[0x1000];
} EqgfxFix16SqrtTable_guess;

typedef struct EqgfxAngleArcNode_guess
{
    int ratio;
    unsigned int reserved04[3];
    struct EqgfxAngleArcNode_guess *lower;
    struct EqgfxAngleArcNode_guess *upper;
} EqgfxAngleArcNode_guess;

typedef void (__cdecl *EqgfxDebugBreakFn_guess)(void);

typedef struct EqgfxVertexTransform_guess
{
    unsigned int packedPrefix;
    float contributionScale;
    float referenceDepth;
    unsigned char reserved[8];
    float horizontalScale;
    float verticalScale;
    float horizontalOffset;
    float verticalOffset;
    float depthOffset;
    float depthScale;
} EqgfxVertexTransform_guess;

typedef struct EqgfxVertexProjector_guess
{
    unsigned int packedPrefix;
    float horizontalScale;
    float verticalScale;
    float horizontalOffset;
    float verticalOffset;
    float horizontalPositionScale;
    float verticalPositionScale;
    float horizontalPositionOffset;
    float verticalPositionOffset;
    float depthValue;
    float depthCorrection;
} EqgfxVertexProjector_guess;

typedef struct EqgfxFix16Matrix_guess
{
    int values[9];
} EqgfxFix16Matrix_guess;

typedef struct EqgfxGlideDevice_guess
{
    int type; // 0x0
    unsigned char unk4[4]; // 0x4
    char name[0x40]; // 0x8
    int unk48; // 0x48
    unsigned char unk4C[0x10C]; // 0x4c
    unsigned int resolutionMask; // 0x158
} EqgfxGlideDevice_guess;

typedef struct EqgfxGrResolution_guess
{
    int resolution; // 0x0
    int refresh; // 0x4
    int colorBuffers; // 0x8
    int auxBuffers; // 0xc
} EqgfxGrResolution_guess;

typedef struct EqgfxSharedBitmap_guess
{
    unsigned char unk0[8]; // 0x0
    const char *sharedName; // 0x8
    unsigned char unkC[8]; // 0xc
    void *sharedView; // 0x14
    unsigned char unk18[4]; // 0x18
    unsigned int width; // 0x1c
    unsigned int height; // 0x20
} EqgfxSharedBitmap_guess;

// DDSURFACEDESC2 as read from a .dds file after its 'DDS ' magic (0x7c bytes).
typedef struct DdsSurfaceDesc_guess
{
    unsigned int size; // 0x0
    unsigned int flags; // 0x4
    unsigned int height; // 0x8
    unsigned int width; // 0xc
    unsigned int linearSize; // 0x10
    unsigned char unk14[0x38]; // 0x14
    unsigned int pixelFormatFlags; // 0x4c
    unsigned int fourCC; // 0x50
    unsigned int bitCount; // 0x54
    unsigned char unk58[0x24]; // 0x58
} DdsSurfaceDesc_guess;

typedef struct T3dMipData_guess
{
    unsigned char unk0[0x14]; // 0x0
    void *pixels; // 0x14
    unsigned int compressedSize; // 0x18
    unsigned int width; // 0x1c
    unsigned int height; // 0x20
    unsigned char unk24[4]; // 0x24
    unsigned int format; // 0x28
    unsigned char unk2C[8]; // 0x2c
    unsigned char *palette; // 0x34
    unsigned char unk38[0x50]; // 0x38
    unsigned int paletteCount; // 0x88
} T3dMipData_guess;

#define EQGFX_FIX16_FRACTION_BITS 16
#define EQGFX_FIX16_MULTIPLY(output, left, right) \
    product.value = __emul((int)(left), (int)(right)); \
    output = (int)((product.parts.lowWord >> EQGFX_FIX16_FRACTION_BITS) | \
                   (product.parts.highWord << EQGFX_FIX16_FRACTION_BITS))

typedef void (__stdcall *EqgfxGrFinishFn)(void);
typedef void (__stdcall *EqgfxGrSstWinCloseFn)(unsigned int windowHandle);

#define EQGFX_FIX16_SQRT_TABLE EQ_GLOBAL(EqgfxFix16SqrtTable_guess *, 0x100F3FF4)
#define t3d_angle_tree_guess EQ_GLOBAL(unsigned char *, 0x100F3FF4)
#define EQGFX_FIXED_POINT_SAFE_DIVIDE EQ_CDECL(int, 0x1004E752, (int, int))
#define EQGFX_DEBUG_BREAK EQ_GLOBAL(EqgfxDebugBreakFn_guess, 0x100760BC)
#define EQGFX_ANGLE_ARC_TREE_ROOT_OFFSET 0x6680
#define EQGFX_ANGLE_ARC_NODE_BASE_OFFSET 0x6080
#define EQGFX_ANGLE_ARC_NODE_SIZE 0x18
#define EQGFX_ANGLE_ARC_QUARTER_TURN 0x80
#define EQGFX_ANGLE_ARC_HALF_TURN 0x100
#define EQGFX_ANGLE_ARC_THREE_QUARTER_TURN 0x180
#define EQGFX_ANGLE_ARC_FULL_TURN 0x200
#define EQGFX_FIX16_SQRT_ERROR 0x80000001u
#define EQGFX_GR_FINISH EQ_GLOBAL(EqgfxGrFinishFn, 0x100F3EF0)
#define EQGFX_RENDERER_STATE EQ_ARRAY(unsigned char, 0x100FB8A8)
#define EQGFX_RENDERER_MEMORY EQ_GLOBAL(void *, 0x100FBA94)
#define EQGFX_WINDOW_HANDLE EQ_GLOBAL(unsigned int, 0x100FBA68)
#define EQGFX_GR_SST_WIN_CLOSE EQ_GLOBAL(EqgfxGrSstWinCloseFn, 0x100F3F68)
#define EQGFX_FINISH_STATE EQ_CDECL(void, 0x10045FDC, (void *))
#define EQGFX_RELEASE_STATE EQ_CDECL(void, 0x10030F6E, (void *))

#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define g_byteFloatTable_guess EQ_ARRAY(float, 0x100FA068)
#define EQGFX_FREE_MEMORY EQ_CDECL(void, 0x10050CBB, (void *))
#define EQGFX_ALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C48, (unsigned int))

typedef float (__cdecl *EqgfxInvSqrtFn_guess)(float squaredLength);
#define EQGFX_INV_SQRT EQ_GLOBAL(EqgfxInvSqrtFn_guess, 0x100FC77C)
// Renderer function-pointer slots, 4 bytes apart from 0x100fc74c (0x100fc77c is EQGFX_INV_SQRT).
#define EQGFX_RENDER_FN_74C EQ_GLOBAL(void *, 0x100FC74C)
#define EQGFX_RENDER_FN_750 EQ_GLOBAL(void *, 0x100FC750)
#define EQGFX_RENDER_FN_754 EQ_GLOBAL(void *, 0x100FC754)
#define EQGFX_RENDER_FN_758 EQ_GLOBAL(void *, 0x100FC758)
#define EQGFX_RENDER_FN_75C EQ_GLOBAL(void *, 0x100FC75C)
#define EQGFX_RENDER_FN_760 EQ_GLOBAL(void *, 0x100FC760)
#define EQGFX_RENDER_FN_764 EQ_GLOBAL(void *, 0x100FC764)
#define EQGFX_RENDER_FN_768 EQ_GLOBAL(void *, 0x100FC768)
#define EQGFX_RENDER_FN_76C EQ_GLOBAL(void *, 0x100FC76C)
#define EQGFX_RENDER_FN_770 EQ_GLOBAL(void *, 0x100FC770)
#define EQGFX_RENDER_FN_774 EQ_GLOBAL(void *, 0x100FC774)
#define EQGFX_RENDER_FN_778 EQ_GLOBAL(void *, 0x100FC778)
#define EQGFX_RENDER_FN_780 EQ_GLOBAL(void *, 0x100FC780)
// Nonzero when the CPU has AMD K3D (3DNow!); the start-up CPU check (unit_1004F9A2.c) sets it.
#define g_100f400c_guess EQ_GLOBAL(int, 0x100F400C)
#define EQGFX_LOG_DEBUG_MESSAGE EQ_CDECL(void, 0x100509EE, (const char *))
#define EQGFX_USING_AMDK3D_MESSAGE ((const char *)0x100a34a0)
#define EQGFX_USING_INTEL_MESSAGE ((const char *)0x100a3490)

typedef void * (__stdcall *EqgfxLoadLibraryAFn)(const char *libraryName);
typedef void * (__stdcall *EqgfxGetProcAddressFn)(void *module, const char *exportName);
typedef int (__stdcall *EqgfxLopenFn)(const char *fileName, int openMode);
typedef unsigned int (__stdcall *EqgfxLreadFn)(int file, void *buffer, unsigned int count);
typedef int (__stdcall *EqgfxLcloseFn)(int file);

#define EQGFX_LOAD_LIBRARY EQ_GLOBAL(EqgfxLoadLibraryAFn, 0x1007608C)
#define EQGFX_GET_PROC_ADDRESS EQ_GLOBAL(EqgfxGetProcAddressFn, 0x1007609C)
#define EQGFX_LOPEN EQ_GLOBAL(EqgfxLopenFn, 0x10076084)
#define EQGFX_LREAD EQ_GLOBAL(EqgfxLreadFn, 0x10076090)
#define EQGFX_LCLOSE EQ_GLOBAL(EqgfxLcloseFn, 0x10076094)
#define EQGFX_FAST_FILE_OPEN EQ_CDECL(int *, 0x10021791, (void *, const char *))
#define EQGFX_FAST_FILE_READ EQ_CDECL(int, 0x1002186A, (void *, void *, int))
#define EQGFX_FAST_FILE_CLOSE EQ_CDECL(int, 0x10021838, (void *))
#define EQGFX_BMP_FILE_TYPE EQ_GLOBAL(unsigned short, 0x100A3434)
#define EQGFX_BMP_FILE_HEADER_SIZE 0x0e
#define EQGFX_BMP_READ_CHUNK 0x8000
#define EQGFX_OF_SHARE_DENY_WRITE 0x20
#define EQGFX_DIB_HEADER_SIZE EQ_CDECL(unsigned int, 0x1004CB7A, (void *))
#define EQGFX_DIB_WIDTH EQ_CDECL(unsigned int, 0x1004CB81, (void *))
#define EQGFX_DIB_HEIGHT EQ_CDECL(unsigned int, 0x1004CB9D, (void *))
#define EQGFX_DIB_PIXELS EQ_CDECL(void *, 0x1004CBB9, (void *))
#define EQGFX_ATTACH_SHARED_MEMORY EQ_CDECL(int, 0x10050BBD, (void *, void *, const char *))
#define EQGFX_CREATE_SHARED_MEMORY EQ_CDECL(int, 0x10050B77, (void *, void *, unsigned int, const char *))
#define EQGFX_MEMCPY EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
#define EQGFX_FOPEN EQ_CDECL(void *, 0x10068412, (const char *, const char *))
#define EQGFX_FREAD EQ_CDECL(unsigned int, 0x10069227, (void *, unsigned int, unsigned int, void *))
#define EQGFX_FCLOSE EQ_CDECL(int, 0x10068328, (void *))
#define EQGFX_SPRINTF EQ_CDECL(int, 0x10067992, (char *, const char *, ...))
#define EQGFX_DIB_COLOR_COUNT EQ_CDECL(unsigned short, 0x1004CCEF, (void *))
#define EQGFX_MIP_FILE_MODE ((const char *)0x100a3438)
#define EQGFX_MIP_INVALID_DEPTH_FORMAT ((const char *)0x100a343c)
#define EQGFX_DDS_MAGIC_LOW 0x4444
#define EQGFX_DDS_MAGIC_HIGH 0x2053
#define EQGFX_DDPF_FOURCC 4
#define EQGFX_FOURCC_DXT1 0x31545844
#define EQGFX_FOURCC_DXT3 0x33545844
#define EQGFX_FOURCC_DXT5 0x35545844
#define EQGFX_MIP_FORMAT_DXT1 0x29
#define EQGFX_MIP_FORMAT_DXT3 0x2b
#define EQGFX_MIP_FORMAT_DXT5 0x2d
#define EQGFX_MIP_MAX_PALETTE 0x100
#define EQGFX_MIP_ERROR_OPEN ((int)0xffff3dac)
#define EQGFX_MIP_ERROR_READ ((int)0xffff3dad)
#define EQGFX_MIP_ERROR_NOT_BITMAP ((int)0xffff3dae)
#define EQGFX_MIP_ERROR_BAD_HEADER ((int)0xffff3daf)
#define EQGFX_MIP_ERROR_NO_MEMORY ((int)0xffff3d41)
#define EQGFX_GLIDE_MODULE EQ_GLOBAL(void *, 0x100F3E70)
#define EQGFX_GLIDE_ENTRY_POINTS EQ_ARRAY(void *, 0x100F3E74)
#define EQGFX_GLIDE_LIBRARY_NAME ((const char *)0x100a342c)
#define EQGFX_EXPORT_GRAADRAWTRIANGLE_24 ((const char *)0x100a3414)
#define EQGFX_EXPORT_GRALPHABLENDFUNCTION_16 ((const char *)0x100a33f8)
#define EQGFX_EXPORT_GRALPHACOMBINE_20 ((const char *)0x100a33e4)
#define EQGFX_EXPORT_GRALPHACONTROLSITRGBLIGHTING_4 ((const char *)0x100a33c4)
#define EQGFX_EXPORT_GRALPHATESTFUNCTION_4 ((const char *)0x100a33ac)
#define EQGFX_EXPORT_GRALPHATESTREFERENCEVALUE_4 ((const char *)0x100a338c)
#define EQGFX_EXPORT_GRBUFFERCLEAR_12 ((const char *)0x100a3378)
#define EQGFX_EXPORT_GRBUFFERSWAP_4 ((const char *)0x100a3368)
#define EQGFX_EXPORT_GRCHROMAKEYMODE_4 ((const char *)0x100a3354)
#define EQGFX_EXPORT_GRCHROMAKEYVALUE_4 ((const char *)0x100a3340)
#define EQGFX_EXPORT_GRCLIPWINDOW_16 ((const char *)0x100a332c)
#define EQGFX_EXPORT_GRCOLORCOMBINE_20 ((const char *)0x100a3318)
#define EQGFX_EXPORT_GRCOLORMASK_8 ((const char *)0x100a3308)
#define EQGFX_EXPORT_GRCONSTANTCOLORVALUE_4 ((const char *)0x100a32f0)
#define EQGFX_EXPORT_GRCOORDINATESPACE_4 ((const char *)0x100a32d8)
#define EQGFX_EXPORT_GRCULLMODE_4 ((const char *)0x100a32c8)
#define EQGFX_EXPORT_GRDEPTHBIASLEVEL_4 ((const char *)0x100a32b4)
#define EQGFX_EXPORT_GRDEPTHBUFFERFUNCTION_4 ((const char *)0x100a3298)
#define EQGFX_EXPORT_GRDEPTHBUFFERMODE_4 ((const char *)0x100a3280)
#define EQGFX_EXPORT_GRDEPTHMASK_4 ((const char *)0x100a3270)
#define EQGFX_EXPORT_GRDEPTHRANGE_8 ((const char *)0x100a3260)
#define EQGFX_EXPORT_GRDISABLE_4 ((const char *)0x100a3250)
#define EQGFX_EXPORT_GRDISABLEALLEFFECTS_0 ((const char *)0x100a3238)
#define EQGFX_EXPORT_GRDITHERMODE_4 ((const char *)0x100a3228)
#define EQGFX_EXPORT_GRDRAWLINE_8 ((const char *)0x100a3218)
#define EQGFX_EXPORT_GRDRAWPOINT_4 ((const char *)0x100a3208)
#define EQGFX_EXPORT_GRDRAWTRIANGLE_12 ((const char *)0x100a31f4)
#define EQGFX_EXPORT_GRDRAWVERTEXARRAY_12 ((const char *)0x100a31dc)
#define EQGFX_EXPORT_GRDRAWVERTEXARRAYCONTIGUOUS_16 ((const char *)0x100a31bc)
#define EQGFX_EXPORT_GRENABLE_4 ((const char *)0x100a31b0)
#define EQGFX_EXPORT_GRERRORSETCALLBACK_4 ((const char *)0x100a3198)
#define EQGFX_EXPORT_GRFINISH_0 ((const char *)0x100a318c)
#define EQGFX_EXPORT_GRFLUSH_0 ((const char *)0x100a3180)
#define EQGFX_EXPORT_GRFOGCOLORVALUE_4 ((const char *)0x100a316c)
#define EQGFX_EXPORT_GRFOGMODE_4 ((const char *)0x100a315c)
#define EQGFX_EXPORT_GRFOGTABLE_4 ((const char *)0x100a314c)
#define EQGFX_EXPORT_GRGET_12 ((const char *)0x100a3140)
#define EQGFX_EXPORT_GRGETPROCADDRESS_4 ((const char *)0x100a312c)
#define EQGFX_EXPORT_GRGETSTRING_4 ((const char *)0x100a311c)
#define EQGFX_EXPORT_GRGLIDEGETSTATE_4 ((const char *)0x100a3108)
#define EQGFX_EXPORT_GRGLIDEGETVERTEXLAYOUT_4 ((const char *)0x100a30ec)
#define EQGFX_EXPORT_GRGLIDEINIT_0 ((const char *)0x100a30dc)
#define EQGFX_EXPORT_GRGLIDESETSTATE_4 ((const char *)0x100a30c8)
#define EQGFX_EXPORT_GRGLIDESETVERTEXLAYOUT_4 ((const char *)0x100a30ac)
#define EQGFX_EXPORT_GRGLIDESHUTDOWN_0 ((const char *)0x100a3098)
#define EQGFX_EXPORT_GRLFBCONSTANTALPHA_4 ((const char *)0x100a3080)
#define EQGFX_EXPORT_GRLFBCONSTANTDEPTH_4 ((const char *)0x100a3068)
#define EQGFX_EXPORT_GRLFBLOCK_24 ((const char *)0x100a3058)
#define EQGFX_EXPORT_GRLFBREADREGION_28 ((const char *)0x100a3044)
#define EQGFX_EXPORT_GRLFBUNLOCK_8 ((const char *)0x100a3034)
#define EQGFX_EXPORT_GRLFBWRITECOLORFORMAT_4 ((const char *)0x100a3018)
#define EQGFX_EXPORT_GRLFBWRITECOLORSWIZZLE_8 ((const char *)0x100a2ffc)
#define EQGFX_EXPORT_GRLFBWRITEREGION_36 ((const char *)0x100a2fe4)
#define EQGFX_EXPORT_GRLOADGAMMATABLE_16 ((const char *)0x100a2fcc)
#define EQGFX_EXPORT_GRQUERYRESOLUTIONS_8 ((const char *)0x100a2fb4)
#define EQGFX_EXPORT_GRRENDERBUFFER_4 ((const char *)0x100a2fa0)
#define EQGFX_EXPORT_GRRESET_4 ((const char *)0x100a2f94)
#define EQGFX_EXPORT_GRSELECTCONTEXT_4 ((const char *)0x100a2f80)
#define EQGFX_EXPORT_GRSPLASH_20 ((const char *)0x100a2f70)
#define EQGFX_EXPORT_GRSSTORIGIN_4 ((const char *)0x100a2f60)
#define EQGFX_EXPORT_GRSSTSELECT_4 ((const char *)0x100a2f50)
#define EQGFX_EXPORT_GRSSTWINCLOSE_4 ((const char *)0x100a2f3c)
#define EQGFX_EXPORT_GRSSTWINOPEN_28 ((const char *)0x100a2f28)
#define EQGFX_EXPORT_GRTEXCALCMEMREQUIRED_16 ((const char *)0x100a2f0c)
#define EQGFX_EXPORT_GRTEXCLAMPMODE_12 ((const char *)0x100a2ef8)
#define EQGFX_EXPORT_GRTEXCOMBINE_28 ((const char *)0x100a2ee4)
#define EQGFX_EXPORT_GRTEXDETAILCONTROL_16 ((const char *)0x100a2ecc)
#define EQGFX_EXPORT_GRTEXDOWNLOADMIPMAP_16 ((const char *)0x100a2eb4)
#define EQGFX_EXPORT_GRTEXDOWNLOADMIPMAPLEVEL_32 ((const char *)0x100a2e94)
#define EQGFX_EXPORT_GRTEXDOWNLOADMIPMAPLEVELPARTIAL ((const char *)0x100a2e70)
#define EQGFX_EXPORT_GRTEXDOWNLOADTABLE_8 ((const char *)0x100a2e58)
#define EQGFX_EXPORT_GRTEXDOWNLOADTABLEPARTIAL_16 ((const char *)0x100a2e38)
#define EQGFX_EXPORT_GRTEXFILTERMODE_12 ((const char *)0x100a2e24)
#define EQGFX_EXPORT_GRTEXLODBIASVALUE_8 ((const char *)0x100a2e0c)
#define EQGFX_EXPORT_GRTEXMAXADDRESS_4 ((const char *)0x100a2df8)
#define EQGFX_EXPORT_GRTEXMINADDRESS_4 ((const char *)0x100a2de4)
#define EQGFX_EXPORT_GRTEXMIPMAPMODE_12 ((const char *)0x100a2dd0)
#define EQGFX_EXPORT_GRTEXMULTIBASE_8 ((const char *)0x100a2dbc)
#define EQGFX_EXPORT_GRTEXMULTIBASEADDRESS_20 ((const char *)0x100a2da0)
#define EQGFX_EXPORT_GRTEXNCCTABLE_4 ((const char *)0x100a2d8c)
#define EQGFX_EXPORT_GRTEXSOURCE_16 ((const char *)0x100a2d7c)
#define EQGFX_EXPORT_GRTEXTEXTUREMEMREQUIRED_8 ((const char *)0x100a2d60)
#define EQGFX_EXPORT_GRVERTEXLAYOUT_12 ((const char *)0x100a2d4c)
#define EQGFX_EXPORT_GRVIEWPORT_16 ((const char *)0x100a2d3c)
#define EQGFX_EXPORT_GU3DFGETINFO_8 ((const char *)0x100a2d2c)
#define EQGFX_EXPORT_GU3DFLOAD_8 ((const char *)0x100a2d1c)
#define EQGFX_EXPORT_GUFOGGENERATEEXP_8 ((const char *)0x100a2d08)
#define EQGFX_EXPORT_GUFOGGENERATEEXP2_8 ((const char *)0x100a2cf0)
#define EQGFX_EXPORT_GUFOGGENERATELINEAR_12 ((const char *)0x100a2cd8)
#define EQGFX_EXPORT_GUFOGTABLEINDEXTOW_4 ((const char *)0x100a2cc0)
#define EQGFX_EXPORT_GUGAMMACORRECTIONRGB_12 ((const char *)0x100a2ca4)
#define EQGFX_RESOLVE_GLIDE_ENTRY(index, exportName) \
    if ((EQGFX_GLIDE_ENTRY_POINTS[index] = getProcAddress(EQGFX_GLIDE_MODULE, exportName)) == 0) \
        goto lookupFailed

typedef void (__stdcall *EqgfxGrGlideInitFn)(void);
typedef int (__stdcall *EqgfxGrGetFn)(unsigned int pname, unsigned int plength, void *params);
typedef const char * (__stdcall *EqgfxGrGetStringFn)(unsigned int pname);
typedef void (__stdcall *EqgfxGrGlideShutdownFn)(void);
typedef int (__stdcall *EqgfxGrQueryResolutionsFn)(const EqgfxGrResolution_guess *filter, EqgfxGrResolution_guess *resolutions);

#define EQGFX_GR_GLIDE_INIT EQ_GLOBAL(EqgfxGrGlideInitFn, 0x100F3F18)
#define EQGFX_GR_GET EQ_GLOBAL(EqgfxGrGetFn, 0x100F3F04)
#define EQGFX_GR_GET_STRING EQ_GLOBAL(EqgfxGrGetStringFn, 0x100F3F0C)
#define EQGFX_GR_GLIDE_SHUTDOWN EQ_GLOBAL(EqgfxGrGlideShutdownFn, 0x100F3F24)
#define EQGFX_GR_QUERY_RESOLUTIONS EQ_GLOBAL(EqgfxGrQueryResolutionsFn, 0x100F3F4C)
#define g_t3d_device_count_guess EQ_GLOBAL(int, 0x100F3FF0)
#define EQGFX_GLIDE_DEVICES EQ_GLOBAL(EqgfxGlideDevice_guess *, 0x100FC784)
#define EQGFX_COPY_STRING EQ_CDECL(void, 0x10068070, (char *, const char *))
#define EQGFX_STRCMP EQ_CDECL(int, 0x10068A20, (const char *, const char *))
#define EQGFX_STRCAT EQ_CDECL(char *, 0x10068080, (char *, const char *))
#define EQGFX_LOAD_GLIDE EQ_CDECL(int, 0x1004BE4E, (void))
#define EQGFX_GLIDE_NO_DEVICES_MESSAGE ((const char *)0x100a2c14)
#define EQGFX_GLIDE_NO_COMPATIBLE_MESSAGE ((const char *)0x100a2c48)
#define EQGFX_GLIDE_DRIVER_SUFFIX ((const char *)0x100a2c88)
#define EQGFX_GLIDE_VOODOO_RUSH_NAME ((const char *)0x100a2c98)
#define EQGFX_GLIDE_MAX_DEVICES 0x10
#define EQGFX_GLIDE_ENUM_ERROR 0xffff3d3eu
#define EQGFX_GLIDE_PNAME_NUM_BOARDS 0x0f
#define EQGFX_GLIDE_STRING_BOARD_NAME 0xa1
#define EQGFX_GLIDE_RES_640X480 0x07
#define EQGFX_GLIDE_RES_800X600 0x08
#define EQGFX_GLIDE_RES_1024X768 0x0c
#define EQGFX_GLIDE_RES_1280X1024 0x0d
#define EQGFX_GLIDE_RES_1600X1200 0x0e

// FUNCTION: EQGFX 0x1004BBE9
// Adds the Glide 3 board to the device list (at most 0x10 entries) and records its resolution mask.
int __cdecl t3dEnumGlideDevices(void)
{
    int result;
    int boardCount;
    EqgfxGrResolution_guess resolutionFilter;
    EqgfxGrResolution_guess *resolutions;
    EqgfxGrResolution_guess *entry;
    unsigned int *deviceMask;
    int resolutionCount;

    result = 0;
    boardCount = 0;
    if (g_t3d_device_count_guess < EQGFX_GLIDE_MAX_DEVICES && EQGFX_LOAD_GLIDE() != 0)
    {
        EQGFX_GR_GET(EQGFX_GLIDE_PNAME_NUM_BOARDS, sizeof(int), &boardCount);
        if (boardCount == 0)
        {
            result = EQGFX_GLIDE_ENUM_ERROR;
            EQGFX_LOG_DEBUG_MESSAGE(EQGFX_GLIDE_NO_DEVICES_MESSAGE);
        }
        else
        {
            EQGFX_GR_GLIDE_INIT();
            EQGFX_COPY_STRING(EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].name,
                              EQGFX_GR_GET_STRING(EQGFX_GLIDE_STRING_BOARD_NAME));
            if (EQGFX_STRCMP(EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].name, EQGFX_GLIDE_VOODOO_RUSH_NAME) == 0)
            {
                result = EQGFX_GLIDE_ENUM_ERROR;
                EQGFX_LOG_DEBUG_MESSAGE(EQGFX_GLIDE_NO_COMPATIBLE_MESSAGE);
            }
            else
            {
                EQGFX_STRCAT(EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].name, EQGFX_GLIDE_DRIVER_SUFFIX);
                EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].type = 2;
                EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].unk48 = 0;
                resolutionFilter.resolution = -1;
                resolutionFilter.refresh = 0;
                resolutionFilter.colorBuffers = 2;
                resolutionFilter.auxBuffers = 1;
                resolutionCount = EQGFX_GR_QUERY_RESOLUTIONS(&resolutionFilter, 0);
                resolutions = EQGFX_ALLOCATE_MEMORY(resolutionCount << 4);
                if (resolutions == 0)
                    return EQGFX_GLIDE_ENUM_ERROR;
                EQGFX_GR_QUERY_RESOLUTIONS(&resolutionFilter, resolutions);
                EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].resolutionMask = 0;
                if (0 < resolutionCount)
                {
                    entry = resolutions;
                    do
                    {
                        if (entry->resolution == EQGFX_GLIDE_RES_640X480)
                        {
                            deviceMask = &EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].resolutionMask;
                            *deviceMask = *deviceMask | 0x1;
                        }
                        else if (entry->resolution == EQGFX_GLIDE_RES_800X600)
                        {
                            deviceMask = &EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].resolutionMask;
                            *deviceMask = *deviceMask | 0x2;
                        }
                        else if (entry->resolution == EQGFX_GLIDE_RES_1024X768)
                        {
                            deviceMask = &EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].resolutionMask;
                            *deviceMask = *deviceMask | 0x4;
                        }
                        else if (entry->resolution == EQGFX_GLIDE_RES_1280X1024)
                        {
                            deviceMask = &EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].resolutionMask;
                            *deviceMask = *deviceMask | 0x8;
                        }
                        else if (entry->resolution == EQGFX_GLIDE_RES_1600X1200)
                        {
                            deviceMask = &EQGFX_GLIDE_DEVICES[g_t3d_device_count_guess].resolutionMask;
                            *deviceMask = *deviceMask | 0x10;
                        }
                        resolutionCount = resolutionCount - 1;
                        entry = entry + 1;
                    } while (resolutionCount != 0);
                }
                EQGFX_FREE_MEMORY(resolutions);
                g_t3d_device_count_guess = g_t3d_device_count_guess + 1;
            }
            EQGFX_GR_GLIDE_SHUTDOWN();
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1004be4e
// Loads the graphics library and connects its rendering entry points.
int __cdecl _t3dLoadGlideLibrary_guess(void)
{
    EqgfxGetProcAddressFn getProcAddress;

    EQGFX_GLIDE_MODULE = EQGFX_LOAD_LIBRARY(EQGFX_GLIDE_LIBRARY_NAME);
    if (EQGFX_GLIDE_MODULE == 0)
        return 0;
    getProcAddress = EQGFX_GET_PROC_ADDRESS;
    EQGFX_RESOLVE_GLIDE_ENTRY(0, EQGFX_EXPORT_GRAADRAWTRIANGLE_24);
    EQGFX_RESOLVE_GLIDE_ENTRY(1, EQGFX_EXPORT_GRALPHABLENDFUNCTION_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(2, EQGFX_EXPORT_GRALPHACOMBINE_20);
    EQGFX_RESOLVE_GLIDE_ENTRY(3, EQGFX_EXPORT_GRALPHACONTROLSITRGBLIGHTING_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(4, EQGFX_EXPORT_GRALPHATESTFUNCTION_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(5, EQGFX_EXPORT_GRALPHATESTREFERENCEVALUE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(6, EQGFX_EXPORT_GRBUFFERCLEAR_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(7, EQGFX_EXPORT_GRBUFFERSWAP_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(8, EQGFX_EXPORT_GRCHROMAKEYMODE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(9, EQGFX_EXPORT_GRCHROMAKEYVALUE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(10, EQGFX_EXPORT_GRCLIPWINDOW_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(11, EQGFX_EXPORT_GRCOLORCOMBINE_20);
    EQGFX_RESOLVE_GLIDE_ENTRY(12, EQGFX_EXPORT_GRCOLORMASK_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(13, EQGFX_EXPORT_GRCONSTANTCOLORVALUE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(14, EQGFX_EXPORT_GRCOORDINATESPACE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(15, EQGFX_EXPORT_GRCULLMODE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(16, EQGFX_EXPORT_GRDEPTHBIASLEVEL_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(17, EQGFX_EXPORT_GRDEPTHBUFFERFUNCTION_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(18, EQGFX_EXPORT_GRDEPTHBUFFERMODE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(19, EQGFX_EXPORT_GRDEPTHMASK_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(20, EQGFX_EXPORT_GRDEPTHRANGE_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(21, EQGFX_EXPORT_GRDISABLE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(22, EQGFX_EXPORT_GRDISABLEALLEFFECTS_0);
    EQGFX_RESOLVE_GLIDE_ENTRY(23, EQGFX_EXPORT_GRDITHERMODE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(24, EQGFX_EXPORT_GRDRAWLINE_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(25, EQGFX_EXPORT_GRDRAWPOINT_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(26, EQGFX_EXPORT_GRDRAWTRIANGLE_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(27, EQGFX_EXPORT_GRDRAWVERTEXARRAY_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(28, EQGFX_EXPORT_GRDRAWVERTEXARRAYCONTIGUOUS_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(29, EQGFX_EXPORT_GRENABLE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(30, EQGFX_EXPORT_GRERRORSETCALLBACK_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(31, EQGFX_EXPORT_GRFINISH_0);
    EQGFX_RESOLVE_GLIDE_ENTRY(32, EQGFX_EXPORT_GRFLUSH_0);
    EQGFX_RESOLVE_GLIDE_ENTRY(33, EQGFX_EXPORT_GRFOGCOLORVALUE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(34, EQGFX_EXPORT_GRFOGMODE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(35, EQGFX_EXPORT_GRFOGTABLE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(36, EQGFX_EXPORT_GRGET_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(37, EQGFX_EXPORT_GRGETPROCADDRESS_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(38, EQGFX_EXPORT_GRGETSTRING_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(39, EQGFX_EXPORT_GRGLIDEGETSTATE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(40, EQGFX_EXPORT_GRGLIDEGETVERTEXLAYOUT_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(41, EQGFX_EXPORT_GRGLIDEINIT_0);
    EQGFX_RESOLVE_GLIDE_ENTRY(42, EQGFX_EXPORT_GRGLIDESETSTATE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(43, EQGFX_EXPORT_GRGLIDESETVERTEXLAYOUT_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(44, EQGFX_EXPORT_GRGLIDESHUTDOWN_0);
    EQGFX_RESOLVE_GLIDE_ENTRY(45, EQGFX_EXPORT_GRLFBCONSTANTALPHA_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(46, EQGFX_EXPORT_GRLFBCONSTANTDEPTH_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(47, EQGFX_EXPORT_GRLFBLOCK_24);
    EQGFX_RESOLVE_GLIDE_ENTRY(48, EQGFX_EXPORT_GRLFBREADREGION_28);
    EQGFX_RESOLVE_GLIDE_ENTRY(49, EQGFX_EXPORT_GRLFBUNLOCK_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(50, EQGFX_EXPORT_GRLFBWRITECOLORFORMAT_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(51, EQGFX_EXPORT_GRLFBWRITECOLORSWIZZLE_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(52, EQGFX_EXPORT_GRLFBWRITEREGION_36);
    EQGFX_RESOLVE_GLIDE_ENTRY(53, EQGFX_EXPORT_GRLOADGAMMATABLE_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(54, EQGFX_EXPORT_GRQUERYRESOLUTIONS_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(55, EQGFX_EXPORT_GRRENDERBUFFER_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(56, EQGFX_EXPORT_GRRESET_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(57, EQGFX_EXPORT_GRSELECTCONTEXT_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(58, EQGFX_EXPORT_GRSPLASH_20);
    EQGFX_RESOLVE_GLIDE_ENTRY(59, EQGFX_EXPORT_GRSSTORIGIN_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(60, EQGFX_EXPORT_GRSSTSELECT_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(61, EQGFX_EXPORT_GRSSTWINCLOSE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(62, EQGFX_EXPORT_GRSSTWINOPEN_28);
    EQGFX_RESOLVE_GLIDE_ENTRY(63, EQGFX_EXPORT_GRTEXCALCMEMREQUIRED_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(64, EQGFX_EXPORT_GRTEXCLAMPMODE_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(65, EQGFX_EXPORT_GRTEXCOMBINE_28);
    EQGFX_RESOLVE_GLIDE_ENTRY(66, EQGFX_EXPORT_GRTEXDETAILCONTROL_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(67, EQGFX_EXPORT_GRTEXDOWNLOADMIPMAP_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(68, EQGFX_EXPORT_GRTEXDOWNLOADMIPMAPLEVEL_32);
    EQGFX_RESOLVE_GLIDE_ENTRY(69, EQGFX_EXPORT_GRTEXDOWNLOADMIPMAPLEVELPARTIAL);
    EQGFX_RESOLVE_GLIDE_ENTRY(70, EQGFX_EXPORT_GRTEXDOWNLOADTABLE_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(71, EQGFX_EXPORT_GRTEXDOWNLOADTABLEPARTIAL_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(72, EQGFX_EXPORT_GRTEXFILTERMODE_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(73, EQGFX_EXPORT_GRTEXLODBIASVALUE_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(74, EQGFX_EXPORT_GRTEXMAXADDRESS_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(75, EQGFX_EXPORT_GRTEXMINADDRESS_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(76, EQGFX_EXPORT_GRTEXMIPMAPMODE_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(77, EQGFX_EXPORT_GRTEXMULTIBASE_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(78, EQGFX_EXPORT_GRTEXMULTIBASEADDRESS_20);
    EQGFX_RESOLVE_GLIDE_ENTRY(79, EQGFX_EXPORT_GRTEXNCCTABLE_4);
    EQGFX_RESOLVE_GLIDE_ENTRY(80, EQGFX_EXPORT_GRTEXSOURCE_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(81, EQGFX_EXPORT_GRTEXTEXTUREMEMREQUIRED_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(82, EQGFX_EXPORT_GRVERTEXLAYOUT_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(83, EQGFX_EXPORT_GRVIEWPORT_16);
    EQGFX_RESOLVE_GLIDE_ENTRY(84, EQGFX_EXPORT_GU3DFGETINFO_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(85, EQGFX_EXPORT_GU3DFLOAD_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(86, EQGFX_EXPORT_GUFOGGENERATEEXP_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(87, EQGFX_EXPORT_GUFOGGENERATEEXP2_8);
    EQGFX_RESOLVE_GLIDE_ENTRY(88, EQGFX_EXPORT_GUFOGGENERATELINEAR_12);
    EQGFX_RESOLVE_GLIDE_ENTRY(89, EQGFX_EXPORT_GUFOGTABLEINDEXTOW_4);
    EQGFX_GLIDE_ENTRY_POINTS[90] = getProcAddress(
        EQGFX_GLIDE_MODULE, EQGFX_EXPORT_GUGAMMACORRECTIONRGB_12);
    if (EQGFX_GLIDE_ENTRY_POINTS[90] != 0)
        return 1;
    return 0;

lookupFailed:
    return 0;
}


typedef void (__stdcall *EqgfxGrSstSelectFn)(int boardNumber);
typedef unsigned int (__stdcall *EqgfxGrSstWinOpenFn)(unsigned int windowHandle, int resolution, int refresh, int colorFormat, int originLocation, int colorBuffers, int auxBuffers);
typedef void (__stdcall *EqgfxGrDepthBufferModeFn)(int mode);
typedef void (__stdcall *EqgfxGrDepthMaskFn)(int enable);
typedef void (__stdcall *EqgfxGrDepthBufferFunctionFn)(int function);
typedef void (__stdcall *EqgfxGrTexFilterModeFn)(int tmu, int minFilter, int magFilter);
typedef void (__stdcall *EqgfxGrBufferClearFn)(unsigned int color, unsigned int alpha, unsigned int depth);
typedef void (__stdcall *EqgfxGrBufferSwapFn)(int swapInterval);
typedef void (__stdcall *EqgfxGrCoordinateSpaceFn)(int coordinateSpace);
typedef void (__stdcall *EqgfxGrVertexLayoutFn)(int param, int offset, int mode);
typedef void (__stdcall *EqgfxGrCullModeFn)(int mode);

typedef struct EqgfxRect
{
    int left;
    int top;
    int right;
    int bottom;
} EqgfxRect;

typedef int (__stdcall *EqgfxSetWindowLongAFn)(unsigned int windowHandle, int index, unsigned int newLong);
typedef int (__stdcall *EqgfxSetWindowPosFn)(unsigned int windowHandle, unsigned int insertAfter, int x, int y, int cx, int cy, unsigned int flags);
typedef int (__stdcall *EqgfxSendMessageAFn)(unsigned int windowHandle, unsigned int message, unsigned int wParam, int lParam);
typedef int (__stdcall *EqgfxAdjustWindowRectFn)(EqgfxRect *rect, unsigned int style, int menu);

#define EQGFX_GR_SST_SELECT EQ_GLOBAL(EqgfxGrSstSelectFn, 0x100F3F64)
#define EQGFX_GR_SST_WIN_OPEN EQ_GLOBAL(EqgfxGrSstWinOpenFn, 0x100F3F6C)
#define EQGFX_GR_DEPTH_BUFFER_MODE EQ_GLOBAL(EqgfxGrDepthBufferModeFn, 0x100F3EBC)
#define EQGFX_GR_DEPTH_MASK EQ_GLOBAL(EqgfxGrDepthMaskFn, 0x100F3EC0)
#define EQGFX_GR_DEPTH_BUFFER_FUNCTION EQ_GLOBAL(EqgfxGrDepthBufferFunctionFn, 0x100F3EB8)
#define EQGFX_GR_TEX_FILTER_MODE EQ_GLOBAL(EqgfxGrTexFilterModeFn, 0x100F3F94)
#define EQGFX_GR_BUFFER_CLEAR EQ_GLOBAL(EqgfxGrBufferClearFn, 0x100F3E8C)
#define EQGFX_GR_BUFFER_SWAP EQ_GLOBAL(EqgfxGrBufferSwapFn, 0x100F3E90)
#define EQGFX_GR_COORDINATE_SPACE EQ_GLOBAL(EqgfxGrCoordinateSpaceFn, 0x100F3EAC)
#define EQGFX_GR_VERTEX_LAYOUT EQ_GLOBAL(EqgfxGrVertexLayoutFn, 0x100F3FBC)
#define EQGFX_GR_CULL_MODE EQ_GLOBAL(EqgfxGrCullModeFn, 0x100F3EB0)
#define EQGFX_SET_WINDOW_LONG EQ_GLOBAL(EqgfxSetWindowLongAFn, 0x10076200)
#define EQGFX_SET_WINDOW_POS EQ_GLOBAL(EqgfxSetWindowPosFn, 0x10076204)
#define EQGFX_SEND_MESSAGE EQ_GLOBAL(EqgfxSendMessageAFn, 0x100761F0)
#define EQGFX_ADJUST_WINDOW_RECT EQ_GLOBAL(EqgfxAdjustWindowRectFn, 0x10076208)
#define EQGFX_INIT_STATE EQ_CDECL(void, 0x10030F46, (void *))
#define EQGFX_SET_GAMMA_LEVEL EQ_CDECL(void, 0x1004A0AA, (float))
#define EQGFX_SET_COLOR_KEY EQ_CDECL(void, 0x10049A52, (unsigned int))
#define EQGFX_SET_FOG EQ_CDECL(void, 0x10049F0D, (int, float, float, unsigned int, unsigned int, unsigned int))
#define EQGFX_DISPLAY_WIDTH EQ_GLOBAL(int, 0x100FBA40)
#define g_display_height_guess EQ_GLOBAL(int, 0x100FBA44)
#define g_current_fullscreen_mode_guess EQ_GLOBAL(int, 0x100FB8AC)
#define EQGFX_MAIN_WINDOW EQ_GLOBAL(unsigned int, 0x100FB8BC)
#define g_100fba6c_guess EQ_GLOBAL(int, 0x100FBA6C)
#define EQGFX_QUERY9_FLAG EQ_GLOBAL(unsigned int, 0x100FBA9C)
#define EQGFX_MULTI_TMU_FLAG EQ_GLOBAL(unsigned int, 0x100FBA54)
#define g_auto_mipmapping_supported_guess EQ_GLOBAL(int, 0x100FBA50)
#define EQGFX_QUERY_BUFFER_27 EQ_ARRAY(unsigned char, 0x100FBA70)
#define EQGFX_QUERY_VALUE_24 EQ_GLOBAL(int, 0x100FBA8C)
#define EQGFX_DEPTH_CLEAR_VALUE EQ_GLOBAL(unsigned int, 0x100FBA74)
#define EQGFX_RENDERER_MEMORY_SIZE EQ_GLOBAL(unsigned int, 0x100FBA90)
#define g_last_color_key_guess EQ_GLOBAL(unsigned int, 0x100FBBB8)
#define EQGFX_GAMMA_LEVEL EQ_GLOBAL(float, 0x100FBA4C)
#define g_fog_enabled_guess EQ_GLOBAL(int, 0x100FBA1C)
#define g_fog_near_guess EQ_GLOBAL(float, 0x100FBA24)
#define g_fog_far_guess EQ_GLOBAL(float, 0x100FBA28)
#define g_fog_color_raw_guess EQ_GLOBAL(unsigned int, 0x100FBA20)
#define EQGFX_RESOLUTION_640X480 0x7
#define EQGFX_RESOLUTION_800X600 0x8
#define EQGFX_RESOLUTION_1024X768 0xc
#define EQGFX_RESOLUTION_1280X1024 0xd
#define EQGFX_RESOLUTION_1600X1200 0xe
#define EQGFX_WINDOWED_STYLE 0x10ca0000
#define EQGFX_FULLSCREEN_STYLE 0x91000000
#define EQGFX_GWL_STYLE (-16)
#define EQGFX_HWND_TOPMOST 0xffffffffu
#define EQGFX_HWND_NOTOPMOST 0xfffffffeu
#define EQGFX_SWP_NOMOVE 0x2
#define EQGFX_WM_SYSCOMMAND 0x112
#define EQGFX_SC_MAXIMIZE 0xf030
#define EQGFX_SETUP_FAILED 0xffff3d3e

// FUNCTION: EQGFX 0x1004c795
// glideInit doubles as the TMU loop index, as in the original.
int __cdecl _t3dSetupGlideDisplay_guess(int glideInit)
{
    int resolution;
    int queryValue;
    int result;
    EqgfxRect rect;

    result = 0;
    if (EQGFX_DISPLAY_WIDTH == 0x280)
    {
        if (g_display_height_guess != 0x1e0)
            return EQGFX_SETUP_FAILED;
        resolution = EQGFX_RESOLUTION_640X480;
    }
    else if (EQGFX_DISPLAY_WIDTH == 0x320)
    {
        if (g_display_height_guess != 0x258)
            return EQGFX_SETUP_FAILED;
        resolution = EQGFX_RESOLUTION_800X600;
    }
    else if (EQGFX_DISPLAY_WIDTH == 0x400)
    {
        if (g_display_height_guess != 0x300)
            return EQGFX_SETUP_FAILED;
        resolution = EQGFX_RESOLUTION_1024X768;
    }
    else if (EQGFX_DISPLAY_WIDTH == 0x500)
    {
        if (g_display_height_guess != 0x400)
            return EQGFX_SETUP_FAILED;
        resolution = EQGFX_RESOLUTION_1280X1024;
    }
    else
    {
        if (EQGFX_DISPLAY_WIDTH != 0x640)
            return EQGFX_SETUP_FAILED;
        if (g_display_height_guess != 0x4b0)
            return EQGFX_SETUP_FAILED;
        resolution = EQGFX_RESOLUTION_1600X1200;
    }
    if (glideInit != 0)
    {
        EQGFX_GR_GLIDE_INIT();
        EQGFX_GR_SST_SELECT(0);
    }
    if (g_current_fullscreen_mode_guess != 0)
    {
        EQGFX_SET_WINDOW_LONG(EQGFX_MAIN_WINDOW, EQGFX_GWL_STYLE, EQGFX_FULLSCREEN_STYLE);
        EQGFX_SET_WINDOW_POS(EQGFX_MAIN_WINDOW, EQGFX_HWND_TOPMOST, 0, 0,
                             EQGFX_DISPLAY_WIDTH, g_display_height_guess, EQGFX_SWP_NOMOVE);
        EQGFX_SEND_MESSAGE(EQGFX_MAIN_WINDOW, EQGFX_WM_SYSCOMMAND, EQGFX_SC_MAXIMIZE, 0);
    }
    else
    {
        EQGFX_SET_WINDOW_LONG(EQGFX_MAIN_WINDOW, EQGFX_GWL_STYLE, EQGFX_WINDOWED_STYLE);
        rect.right = EQGFX_DISPLAY_WIDTH;
        rect.bottom = g_display_height_guess;
        rect.left = 0;
        rect.top = 0;
        EQGFX_ADJUST_WINDOW_RECT(&rect, EQGFX_WINDOWED_STYLE, 0);
        EQGFX_SET_WINDOW_POS(EQGFX_MAIN_WINDOW, EQGFX_HWND_NOTOPMOST, 0, 0,
                             rect.right - rect.left, rect.bottom - rect.top, EQGFX_SWP_NOMOVE);
    }
    EQGFX_WINDOW_HANDLE = EQGFX_GR_SST_WIN_OPEN(EQGFX_MAIN_WINDOW, resolution, 0, 0, 0, 2, 1);
    if (EQGFX_WINDOW_HANDLE != 0)
    {
        EQGFX_GR_GET(0x13, 4, &g_100fba6c_guess);
        EQGFX_GR_DEPTH_BUFFER_MODE(2);
        EQGFX_GR_DEPTH_MASK(1);
        EQGFX_GR_DEPTH_BUFFER_FUNCTION(3);
        EQGFX_GR_GET(9, 4, &queryValue);
        glideInit = 0;
        EQGFX_QUERY9_FLAG = (queryValue == 1);
        if (0 < g_100fba6c_guess)
        {
            do
            {
                EQGFX_GR_TEX_FILTER_MODE(glideInit, 1, 1);
                glideInit = glideInit + 1;
            } while (glideInit < g_100fba6c_guess);
        }
        EQGFX_GR_GET(0x27, 8, EQGFX_QUERY_BUFFER_27);
        EQGFX_GR_GET(0x24, 4, &EQGFX_QUERY_VALUE_24);
        EQGFX_GR_BUFFER_CLEAR(0, 0, EQGFX_DEPTH_CLEAR_VALUE);
        EQGFX_GR_BUFFER_SWAP(1);
        EQGFX_GR_BUFFER_CLEAR(0, 0, EQGFX_DEPTH_CLEAR_VALUE);
        EQGFX_GR_COORDINATE_SPACE(0);
        EQGFX_GR_VERTEX_LAYOUT(1, 0, 1);
        EQGFX_GR_VERTEX_LAYOUT(4, 0xc, 1);
        EQGFX_GR_VERTEX_LAYOUT(0x30, 0x10, 1);
        glideInit = 0;
        if (0 < g_100fba6c_guess)
        {
            do
            {
                EQGFX_GR_VERTEX_LAYOUT(glideInit + 0x40, 0x18, 1);
                glideInit = glideInit + 1;
            } while (glideInit < g_100fba6c_guess);
        }
        EQGFX_INIT_STATE(EQGFX_RENDERER_STATE);
        EQGFX_MULTI_TMU_FLAG = (1 < g_100fba6c_guess);
        g_auto_mipmapping_supported_guess = 1;
        EQGFX_GR_GET(4, 4, &EQGFX_RENDERER_MEMORY_SIZE);
        EQGFX_RENDERER_MEMORY = EQGFX_ALLOCATE_MEMORY(EQGFX_RENDERER_MEMORY_SIZE);
        if (EQGFX_RENDERER_MEMORY != 0)
        {
            EQGFX_GR_CULL_MODE(1);
            EQGFX_SET_GAMMA_LEVEL(EQGFX_GAMMA_LEVEL);
            EQGFX_SET_COLOR_KEY(g_last_color_key_guess);
            EQGFX_SET_FOG(g_fog_enabled_guess, g_fog_near_guess, g_fog_far_guess, g_fog_color_raw_guess,
                          (g_fog_color_raw_guess >> 8) & 0xff, g_fog_color_raw_guess >> 0x10);
            return result;
        }
    }
    return EQGFX_SETUP_FAILED;
}

// FUNCTION: EQGFX 0x1004caaa
// Shuts down the active 3D renderer and closes its graphics window.
int __cdecl s3dShutdownRenderer_guess(void)
{
    unsigned char *rendererState;

    EQGFX_GR_FINISH();
    rendererState = EQGFX_RENDERER_STATE;
    EQGFX_FINISH_STATE(rendererState);
    EQGFX_RELEASE_STATE(rendererState);
    EQGFX_FREE_MEMORY(EQGFX_RENDERER_MEMORY);
    EQGFX_GR_SST_WIN_CLOSE(EQGFX_WINDOW_HANDLE);
    return 0;
}

// FUNCTION: EQGFX 0x1004cae0
// __cdecl. Gets the surface pixel format; for 16-bit records 565 vs 555.
int __cdecl _t3dValidateSurfacePixelFormat_guess(int bitDepth)
{
    typedef int (__stdcall *GetPixelFormatFn)(void *self, void *pixfmt);
    typedef struct
    {
        unsigned int size; // 0x0
        unsigned int flags; // 0x4
        unsigned int fourCC; // 0x8
        unsigned int bitCount; // 0xc
        unsigned int redMask; // 0x10
        unsigned int greenMask; // 0x14
        unsigned int blueMask; // 0x18
        unsigned int alphaMask; // 0x1c
    } PixelFormat;
    PixelFormat pixfmt;

    memset(&pixfmt, 0, 0x20);
    pixfmt.size = 0x20;
    if (((GetPixelFormatFn)(*(void ***)EQ_GLOBAL(void *, 0x100FBA88))[21])(EQ_GLOBAL(void *, 0x100FBA88), &pixfmt) < 0)
        return -1;
    if (bitDepth == 0x10)
    {
        if ((pixfmt.flags & 0x40) == 0 || pixfmt.bitCount != 0x10 || pixfmt.redMask != 0xf800 || pixfmt.greenMask != 0x7e0 || pixfmt.blueMask != 0x1f)
        {
            if ((pixfmt.flags & 0x40) == 0)
                return -1;
            if (pixfmt.bitCount != 0x10)
                return -1;
            if (pixfmt.redMask != 0x7c00)
                return -1;
            if (pixfmt.greenMask != 0x3e0)
                return -1;
            if (pixfmt.blueMask != 0x1f)
                return -1;
            EQ_GLOBAL(int, 0x100FBA3C) = 0;
        }
        else
        {
            EQ_GLOBAL(int, 0x100FBA3C) = 1;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1004cb7a
// Reads the leading header value used to interpret a bitmap image.
// __cdecl (the callee pops 0 bytes; ECX is not an input). One pointer argument,
// returns the first 4-byte value it points at.
unsigned int __cdecl getFirstValueFromPointer_guess(unsigned int *pointer)
{
    return *pointer;
}

// FUNCTION: EQGFX 0x1004cb81
// Gets a bitmap image's width for texture loading.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Reads the tag at
// +0x0 through 0x1004cb7a; when it is 0xc returns the u16 at +0x4, else the
// u32 at +0x4.
unsigned int __cdecl t3dGetDibWidth_guess(unsigned int *record)
{
    if (EQ_CDECL(unsigned int, 0x1004CB7A, (unsigned int *))(record) == 0xc)
        return *(unsigned short *)((char *)record + 4);
    return *(unsigned int *)((char *)record + 4);
}

// FUNCTION: EQGFX 0x1004cb9d
// Gets a bitmap image's height for texture loading.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Reads the tag at
// +0x0 through 0x1004cb7a; when it is 0xc returns the u16 at +0x6, else the
// u32 at +0x8.
unsigned int __cdecl t3dGetDibHeight_guess(unsigned int *record)
{
    if (EQ_CDECL(unsigned int, 0x1004CB7A, (unsigned int *))(record) == 0xc)
        return *(unsigned short *)((char *)record + 6);
    return *(unsigned int *)((char *)record + 8);
}

// FUNCTION: EQGFX 0x1004cbb9
// Locates the pixel data in a bitmap image used by the renderer.
// __cdecl. Size plus base: tag 0xc uses the u16 at +0xa,
// otherwise the u32 at +0x20 (or 1<<bitcount), scaled, added to tag and base.
unsigned int __cdecl getBitmapPixelData_guess(Record_guess *record)
{
    unsigned int extra;
    unsigned short bitDepth;

    if (EQ_CDECL(unsigned int, 0x1004CB7A, (unsigned int *))((unsigned int *)record) == 0xc)
    {
        if (record->fieldA == 0x18)
            extra = 0;
        else
            extra = 1u << record->fieldA;
        extra = extra * 3;
    }
    else
    {
        bitDepth = record->fieldE;
        if (EQ_CDECL(unsigned int, 0x1004CB7A, (unsigned int *))((unsigned int *)record) < 0x24)
            extra = 0;
        else
            extra = record->field20;
        if (extra == 0)
        {
            if (bitDepth == 0x18)
                extra = 0;
            else
                extra = 1u << bitDepth;
        }
        extra <<= 2;
    }
    return EQ_CDECL(unsigned int, 0x1004CB7A, (unsigned int *))((unsigned int *)record) + extra + (unsigned int)record;
}

// FUNCTION: EQGFX 0x1004cc24
// Loads a BMP image directly from a disk file for renderer use.
// __cdecl. Reads a BMP file's DIB (the 14-byte file header skipped) into a t3dAllocateMemory block (the
// fileName slot is reused for it). Returns the block when its first u32 is 0xc or 0x10 and up, else frees it.
void *__cdecl t3dReadDibFromFile_guess(const char *fileName)
{
    BitmapFileHeader_guess header;
    int file;
    int openedFile;
    unsigned int bytesLeft;
    unsigned int offset;
    unsigned int chunk;
    unsigned int bytesRead;
    unsigned int infoHeaderSize;

    file = EQGFX_LOPEN(fileName, EQGFX_OF_SHARE_DENY_WRITE);
    openedFile = file;
    if (file == -1)
        return 0;
    if (EQGFX_LREAD(file, &header, EQGFX_BMP_FILE_HEADER_SIZE) != EQGFX_BMP_FILE_HEADER_SIZE)
        goto closeAndReturnZero;
    if (header.type != EQGFX_BMP_FILE_TYPE)
        goto closeAndReturnZero;
    bytesLeft = header.size - EQGFX_BMP_FILE_HEADER_SIZE;
    fileName = EQGFX_ALLOCATE_MEMORY(bytesLeft);
    if (fileName == 0)
    {
closeAndReturnZero:
        EQGFX_LCLOSE(file);
        return 0;
    }
    offset = 0;
    while (bytesLeft > 0)
    {
        chunk = EQGFX_BMP_READ_CHUNK;
        if (bytesLeft <= chunk)
            chunk = bytesLeft;
        chunk &= 0xffff;
        bytesRead = EQGFX_LREAD(openedFile, (void *)(fileName + offset), chunk);
        if (chunk != bytesRead)
            goto readFailed;
        bytesLeft -= chunk;
        offset += chunk;
    }
    file = openedFile;
    EQGFX_LCLOSE(file);
    infoHeaderSize = EQ_CDECL(unsigned int, 0x1004CB7A, (unsigned int *))((unsigned int *)fileName);
    if (infoHeaderSize < 0xc)
        goto freeBlock;
    if (infoHeaderSize == 0xc)
        return (void *)fileName;
    if (infoHeaderSize < 0x10)
        goto freeBlock;
    return (void *)fileName;

readFailed:
    EQGFX_LCLOSE(openedFile);
freeBlock:
    EQGFX_FREE_MEMORY((void *)fileName);
    return 0;
}

// FUNCTION: EQGFX 0x1004ccef
// Determines how many palette colors to load from a bitmap image.
// __cdecl. Bitmap header (0x28 = BITMAPINFOHEADER) -> palette entry count:
// the u32 at +0x20 (biClrUsed) when nonzero, else 1/4/8-bit -> 2/0x10/0x100,
// else 0.
/* fake match: the duplicate size read keeps VC6's cmp edx,edx in the nested branch. */
unsigned short __cdecl getPaletteColorCountFromHeader_guess(Record_guess *header)
{
    unsigned int headerSize;
    unsigned int colorsUsed;
    unsigned int checkedHeaderSize;
    unsigned short bitCount;

    headerSize = header->tag;
    if (headerSize == 0x28)
    {
        colorsUsed = header->field20;
        if (colorsUsed != 0)
            return (unsigned short)colorsUsed;
        checkedHeaderSize = header->tag;
        if (headerSize != checkedHeaderSize)
            bitCount = header->fieldA;
        else
            bitCount = header->fieldE;
    }
    else
    {
        bitCount = header->fieldA;
    }

    switch (bitCount)
    {
    case 1:
        return 2;
    case 4:
        return 0x10;
    case 8:
        return 0x100;
    default:
        return 0;
    }
}

// FUNCTION: EQGFX 0x1004cd32
// Loads bitmap image data through the engine's fast-file path.
// __cdecl. Opens the file with t3dFastFileOpen and reads its BITMAPFILEHEADER; a 'BM' file's DIB goes into a
// t3dAllocateMemory block that is returned when its first u32 is 0xc or 0x10 and up, else freed. Returns 0 on failure.
void *__cdecl t3dFastFileLoadBitmapDIB_guess(void *fileReference, const char *filePathName)
{
    BitmapFileHeader_guess header;
    int *file;
    unsigned int dibSize;
    void *block;
    unsigned int infoHeaderSize;

    file = EQGFX_FAST_FILE_OPEN(fileReference, filePathName);
    if (file == 0)
        return 0;
    if (EQGFX_FAST_FILE_READ(file, &header, EQGFX_BMP_FILE_HEADER_SIZE) != 1)
        goto closeAndReturnZero;
    if (header.type != EQGFX_BMP_FILE_TYPE)
        goto closeAndReturnZero;
    dibSize = header.size - EQGFX_BMP_FILE_HEADER_SIZE;
    block = EQGFX_ALLOCATE_MEMORY(dibSize);
    if (block == 0)
    {
closeAndReturnZero:
        EQGFX_FAST_FILE_CLOSE(file);
        return 0;
    }
    EQGFX_FAST_FILE_READ(file, block, dibSize);
    EQGFX_FAST_FILE_CLOSE(file);
    infoHeaderSize = EQ_CDECL(unsigned int, 0x1004CB7A, (unsigned int *))((unsigned int *)block);
    if (infoHeaderSize >= 0xc)
    {
        if (infoHeaderSize <= 0xc)
            return block;
        if (infoHeaderSize >= 0x10)
            return block;
    }
    EQGFX_FREE_MEMORY(block);
    return 0;
}

// FUNCTION: EQGFX 0x1004cdbf
// Makes bitmap pixels available through a named shared image buffer.
// __cdecl (the callee pops 0). Reads a BMP's DIB into a t3dAllocateMemory block, then attaches the named shared section or
// creates it and copies the pixels in. Returns 0 or -1..-7; the file, read offset and result share the argument slots.
int __cdecl _t3dReadDibIntoSharedMemory_guess(void *fileReference, int filePathName, EqgfxSharedBitmap_guess *bitmap)
{
    BitmapFileHeader_guess header;
    void *mappingHandle;
    unsigned int dibSize;
    void *dib;
    unsigned int dibBytesLeft;
    unsigned int readSize;
    unsigned int chunkSize;
    unsigned int height;
    unsigned int infoHeaderSize;
    void **viewSlot;

    fileReference = EQGFX_FAST_FILE_OPEN(fileReference, (const char *)filePathName);
    if (fileReference == 0)
    {
        filePathName = -1;
        goto finish;
    }
    if (EQGFX_FAST_FILE_READ(fileReference, &header, EQGFX_BMP_FILE_HEADER_SIZE) != 1)
    {
        EQGFX_FAST_FILE_CLOSE(fileReference);
        filePathName = -2;
        goto finish;
    }
    if (header.type != EQGFX_BMP_FILE_TYPE)
    {
        EQGFX_FAST_FILE_CLOSE(fileReference);
        filePathName = -3;
        goto finish;
    }
    dibBytesLeft = header.size - EQGFX_BMP_FILE_HEADER_SIZE;
    dib = EQGFX_ALLOCATE_MEMORY(dibBytesLeft);
    dibSize = dibBytesLeft;
    if (dib == 0)
    {
        EQGFX_FAST_FILE_CLOSE(fileReference);
        filePathName = -4;
        goto finish;
    }
    filePathName = 0;
    for (; dibBytesLeft > 0; dibBytesLeft -= chunkSize)
    {
        readSize = EQGFX_BMP_READ_CHUNK;
        if (dibBytesLeft <= readSize)
            readSize = dibBytesLeft;
        chunkSize = (unsigned short)readSize;
        if (EQGFX_FAST_FILE_READ(fileReference, (char *)dib + filePathName, chunkSize) != 1)
        {
            EQGFX_FAST_FILE_CLOSE(fileReference);
            EQGFX_FREE_MEMORY(dib);
            filePathName = -5;
            goto finish;
        }
        filePathName += chunkSize;
    }
    EQGFX_FAST_FILE_CLOSE(fileReference);
    infoHeaderSize = EQGFX_DIB_HEADER_SIZE(dib);
    if (infoHeaderSize >= 0xc && (infoHeaderSize <= 0xc || infoHeaderSize >= 0x10))
    {
        viewSlot = &bitmap->sharedView;
        filePathName = EQGFX_ATTACH_SHARED_MEMORY(&mappingHandle, viewSlot, bitmap->sharedName);
        if (filePathName == 0)
        {
            bitmap->width = EQGFX_DIB_WIDTH(dib);
            bitmap->height = EQGFX_DIB_HEIGHT(dib);
        }
        else
        {
            filePathName = EQGFX_CREATE_SHARED_MEMORY(&mappingHandle, viewSlot, dibSize, bitmap->sharedName);
            if (filePathName != 0)
            {
                filePathName = -7;
                goto finish;
            }
            bitmap->width = EQGFX_DIB_WIDTH(dib);
            height = EQGFX_DIB_HEIGHT(dib);
            bitmap->height = height;
            EQGFX_MEMCPY(*viewSlot, EQGFX_DIB_PIXELS(dib), bitmap->width * height);
        }
        EQGFX_FREE_MEMORY(dib);
    }
    else
    {
        filePathName = -6;
    }
finish:
    return filePathName;
}

// FUNCTION: EQGFX 0x1004cf66
// Loads a DDS (DXT1/3/5 or raw) or 8/16-bit BMP texture file into a mip record: size, format, pixels, palette.
// __cdecl. Loads a 'DDS ' or BMP texture (t3dFastFile when fileReference is set, else fopen) into mipData; 0 or an
// error code. An unknown size (DDS bit depth) allocates the bitmapName pointer's value, as the original's stale slot does.
int __cdecl _t3dFastFileReadMipData(void *unused, char **bitmapName, T3dMipData_guess *mipData, void *fileReference)
{
    char message[512];
    DdsSurfaceDesc_guess desc;
    BitmapFileHeader_guess header;
    unsigned int width;
    unsigned int height;
    unsigned int magic;
    int result;
    void *file;
    void *dib;
    unsigned int dibSize;
    unsigned int infoHeaderSize;
    unsigned int bitDepth;
    unsigned int paletteCount;
    unsigned char *palette;
    unsigned char *paletteEnd;
    unsigned char *source;
    unsigned char *destination;
    unsigned int size;
    unsigned char *pixels;
    unsigned int offset;
    unsigned int readSize;
    unsigned int chunkSize;

    result = 0;
    magic = 0;
    if (fileReference != 0)
    {
        file = EQGFX_FAST_FILE_OPEN(fileReference, *bitmapName);
        if (file == 0)
            goto openFailed;
        if (EQGFX_FAST_FILE_READ(file, &magic, 2) == 0)
            goto fastReadFailed;
        if ((unsigned short)magic == EQGFX_DDS_MAGIC_LOW)
        {
            if (EQGFX_FAST_FILE_READ(file, &magic, 2) == 0)
                goto fastReadFailed;
            if ((unsigned short)magic != EQGFX_DDS_MAGIC_HIGH)
                goto fastReadFailed;
            if (EQGFX_FAST_FILE_READ(file, &desc, sizeof(desc)) == 0)
                goto fastReadFailed;
            goto readDds;
        }
        header.type = (unsigned short)magic;
        if (EQGFX_FAST_FILE_READ(file, &header.size, EQGFX_BMP_FILE_HEADER_SIZE - 2) == 0)
            goto fastReadFailed;
        goto readBitmap;
    }
    file = EQGFX_FOPEN(*bitmapName, EQGFX_MIP_FILE_MODE);
    if (file == 0)
    {
openFailed:
        result = EQGFX_MIP_ERROR_OPEN;
        goto done;
    }
    if (EQGFX_FREAD(&magic, 1, 2, file) != 2)
        goto stdReadFailed;
    if ((unsigned short)magic == EQGFX_DDS_MAGIC_LOW)
    {
        if (EQGFX_FREAD(&magic, 1, 2, file) != 2)
            goto stdReadFailed;
        if ((unsigned short)magic != EQGFX_DDS_MAGIC_HIGH)
            goto stdReadFailed;
        if (EQGFX_FREAD(&desc, 1, sizeof(desc), file) != sizeof(desc))
            goto stdReadFailed;
        goto readDds;
    }
    header.type = (unsigned short)magic;
    if (EQGFX_FREAD(&header.size, 1, EQGFX_BMP_FILE_HEADER_SIZE - 2, file) != EQGFX_BMP_FILE_HEADER_SIZE - 2)
        goto stdReadFailed;
    goto readBitmap;

readDds:
    mipData->paletteCount = 0;
    mipData->width = desc.width;
    mipData->height = desc.height;
    mipData->format = desc.bitCount;
    if ((desc.pixelFormatFlags & EQGFX_DDPF_FOURCC) == EQGFX_DDPF_FOURCC)
    {
        if (desc.fourCC == EQGFX_FOURCC_DXT1)
        {
            size = desc.linearSize;
            mipData->format = EQGFX_MIP_FORMAT_DXT1;
            mipData->compressedSize = size;
        }
        else if (desc.fourCC == EQGFX_FOURCC_DXT3)
        {
            size = desc.linearSize;
            mipData->format = EQGFX_MIP_FORMAT_DXT3;
            mipData->compressedSize = size;
        }
        else if (desc.fourCC == EQGFX_FOURCC_DXT5)
        {
            size = desc.linearSize;
            mipData->format = EQGFX_MIP_FORMAT_DXT5;
            mipData->compressedSize = size;
        }
        else
        {
            if (fileReference == 0)
                goto stdReadFailed;
            goto fastReadFailed;
        }
    }
    else if (desc.bitCount == 4)
        size = (desc.width >> 1) * desc.height;
    else if (desc.bitCount == 8)
        size = desc.height * desc.width;
    else if (desc.bitCount == 0x10)
        size = desc.height * desc.width * 2;
    else if (desc.bitCount == 0x18)
        size = desc.height * desc.width * 3;
    else if (desc.bitCount == 0x20)
        size = desc.height * desc.width * 4;
    else
        size = (unsigned int)bitmapName;
    pixels = (unsigned char *)EQGFX_ALLOCATE_MEMORY(size);
    mipData->pixels = pixels;
    if (pixels == 0)
        goto noMemory;
    if (fileReference != 0)
    {
        for (offset = 0; size > 0; size -= chunkSize)
        {
            readSize = EQGFX_BMP_READ_CHUNK;
            if (size <= readSize)
                readSize = size;
            chunkSize = (unsigned short)readSize;
            if (EQGFX_FAST_FILE_READ(file, pixels + offset, chunkSize) == 0)
                goto fastPixelsFailed;
            offset += chunkSize;
        }
        goto fastDone;
    }
    for (offset = 0; size > 0; size -= chunkSize)
    {
        readSize = EQGFX_BMP_READ_CHUNK;
        if (size <= readSize)
            readSize = size;
        chunkSize = (unsigned short)readSize;
        if (EQGFX_FREAD(pixels + offset, 1, chunkSize, file) != chunkSize)
            goto stdPixelsFailed;
        offset += chunkSize;
    }
    goto stdDone;

readBitmap:
    if (header.type != EQGFX_BMP_FILE_TYPE)
    {
        if (fileReference != 0)
            EQGFX_FAST_FILE_CLOSE(file);
        else
            EQGFX_FCLOSE(file);
        result = EQGFX_MIP_ERROR_NOT_BITMAP;
        goto done;
    }
    dibSize = header.offBits - EQGFX_BMP_FILE_HEADER_SIZE;
    dib = EQGFX_ALLOCATE_MEMORY(dibSize);
    if (dib == 0)
        goto noMemory;
    if (fileReference != 0)
    {
        if (EQGFX_FAST_FILE_READ(file, dib, dibSize) != 1)
        {
            EQGFX_FREE_MEMORY(dib);
            goto fastReadFailed;
        }
    }
    else if (EQGFX_FREAD(dib, 1, dibSize, file) != dibSize)
    {
        EQGFX_FREE_MEMORY(dib);
        goto stdReadFailed;
    }
    infoHeaderSize = EQGFX_DIB_HEADER_SIZE(dib);
    if (infoHeaderSize < 0xc || (infoHeaderSize > 0xc && infoHeaderSize < 0x10))
    {
        result = EQGFX_MIP_ERROR_BAD_HEADER;
        goto done;
    }
    width = EQGFX_DIB_WIDTH(dib);
    mipData->width = width;
    height = EQGFX_DIB_HEIGHT(dib);
    mipData->height = height;
    bitDepth = ((Record_guess *)dib)->fieldE;
    mipData->paletteCount = 0;
    mipData->format = bitDepth;
    if (bitDepth == 8)
    {
        paletteCount = EQGFX_DIB_COLOR_COUNT(dib);
        if (paletteCount > EQGFX_MIP_MAX_PALETTE)
            paletteCount = EQGFX_MIP_MAX_PALETTE;
        mipData->paletteCount = paletteCount;
        if (paletteCount != 0)
        {
            palette = (unsigned char *)EQGFX_ALLOCATE_MEMORY(paletteCount * 4);
            mipData->palette = palette;
            if (palette == 0)
            {
                if (fileReference != 0)
                    EQGFX_FAST_FILE_CLOSE(file);
                else
                    EQGFX_FCLOSE(file);
                EQGFX_FREE_MEMORY(dib);
                goto readFailed;
            }
            paletteEnd = palette + paletteCount * 4;
            source = (unsigned char *)dib + EQGFX_DIB_HEADER_SIZE(dib);
            for (destination = mipData->palette; destination < paletteEnd; destination += 4)
            {
                destination[0] = source[2];
                destination[1] = source[1];
                destination[2] = source[0];
                source += 4;
            }
        }
        else
        {
            mipData->palette = 0;
        }
    }
    else if (bitDepth != 0x10)
    {
        if (fileReference != 0)
            EQGFX_FAST_FILE_CLOSE(file);
        else
            EQGFX_FCLOSE(file);
        EQGFX_FREE_MEMORY(dib);
        EQGFX_SPRINTF(message, EQGFX_MIP_INVALID_DEPTH_FORMAT, *bitmapName);
        goto readFailed;
    }
    EQGFX_FREE_MEMORY(dib);
    if (bitDepth == 8)
        size = height * width;
    else if (bitDepth == 0x10)
        size = height * width * 2;
    else
        size = (unsigned int)bitmapName;
    pixels = (unsigned char *)EQGFX_ALLOCATE_MEMORY(size);
    mipData->pixels = pixels;
    if (pixels == 0)
        goto noMemory;
    if (fileReference != 0)
    {
        for (offset = 0; size > 0; size -= chunkSize)
        {
            readSize = EQGFX_BMP_READ_CHUNK;
            if (size <= readSize)
                readSize = size;
            chunkSize = (unsigned short)readSize;
            if (EQGFX_FAST_FILE_READ(file, pixels + offset, chunkSize) == 0)
                goto fastPixelsFailed;
            offset += chunkSize;
        }
        goto fastDone;
    }
    for (offset = 0; size > 0; size -= chunkSize)
    {
        readSize = EQGFX_BMP_READ_CHUNK;
        if (size <= readSize)
            readSize = size;
        chunkSize = (unsigned short)readSize;
        if (EQGFX_FREAD(pixels + offset, 1, chunkSize, file) != chunkSize)
            goto stdPixelsFailed;
        offset += chunkSize;
    }

stdDone:
    EQGFX_FCLOSE(file);
    goto done;
fastDone:
    EQGFX_FAST_FILE_CLOSE(file);
    goto done;
noMemory:
    if (fileReference != 0)
        EQGFX_FAST_FILE_CLOSE(file);
    else
        EQGFX_FCLOSE(file);
    result = EQGFX_MIP_ERROR_NO_MEMORY;
    goto done;
fastPixelsFailed:
    EQGFX_FAST_FILE_CLOSE(file);
    EQGFX_FREE_MEMORY(pixels);
    goto readFailed;
stdPixelsFailed:
    EQGFX_FCLOSE(file);
    EQGFX_FREE_MEMORY(mipData->pixels);
    goto readFailed;
fastReadFailed:
    EQGFX_FAST_FILE_CLOSE(file);
    goto readFailed;
stdReadFailed:
    EQGFX_FCLOSE(file);
readFailed:
    result = EQGFX_MIP_ERROR_READ;
done:
    return result;
}

// FUNCTION: EQGFX 0x1004d534
// Chooses CPU-specific geometry routines for the renderer.
// __cdecl, no arguments, void. Points the renderer's slots at the AMD K3D routines; unless the K3D flag is set,
// logs the Intel path and repoints the eight slots that have Intel routines.
void __cdecl s3dSetJumpTable(void)
{
    EQGFX_RENDER_FN_76C = (void *)0x1004d726;
    EQGFX_RENDER_FN_778 = (void *)0x1004d7fa;
    EQGFX_RENDER_FN_780 = (void *)0x1004d631;
    EQGFX_RENDER_FN_758 = (void *)0x1004d6af;
    EQGFX_RENDER_FN_768 = (void *)0x1004de48;
    EQGFX_RENDER_FN_770 = (void *)0x1004df6d;
    EQGFX_RENDER_FN_774 = (void *)0x1004d902;
    EQGFX_RENDER_FN_75C = (void *)0x1004d9bb;
    EQGFX_RENDER_FN_750 = (void *)0x1004da4e;
    EQGFX_RENDER_FN_764 = (void *)0x1004dc63;
    EQGFX_RENDER_FN_754 = (void *)0x1004db4c;
    EQGFX_RENDER_FN_760 = (void *)0x1004dd49;
    EQGFX_INV_SQRT = (EqgfxInvSqrtFn_guess)0x1004efda;
    EQGFX_RENDER_FN_74C = (void *)0x1004decf;

    if (g_100f400c_guess != 0)
    {
        EQGFX_LOG_DEBUG_MESSAGE(EQGFX_USING_AMDK3D_MESSAGE);
        return;
    }
    EQGFX_LOG_DEBUG_MESSAGE(EQGFX_USING_INTEL_MESSAGE);
    EQGFX_RENDER_FN_758 = (void *)0x10050d00;
    EQGFX_RENDER_FN_780 = (void *)0x10050d7a;
    EQGFX_RENDER_FN_76C = (void *)0x10050e01;
    EQGFX_RENDER_FN_778 = (void *)0x10050e8c;
    EQGFX_RENDER_FN_768 = (void *)0x10050fda;
    EQGFX_RENDER_FN_770 = (void *)0x10051081;
    EQGFX_RENDER_FN_74C = (void *)0x1005118b;
    EQGFX_RENDER_FN_774 = (void *)0x1005124b;
}

// FUNCTION: EQGFX 0x1004d631
// Transforms only flagged 3D points for a rendering pass.
void __cdecl s3dTransformEnabledPoints3D_guess(float *sourcePoints, float *destinationPoints, float *transform, int pointCount, char *pointFlags)
{
    float x;
    float y;
    float z;

    for (; pointCount != 0; pointCount = pointCount - 1)
    {
        if (*pointFlags != '\0')
        {
            x = sourcePoints[0];
            y = sourcePoints[1];
            z = sourcePoints[2];
            destinationPoints[0] = x * transform[0] + y * transform[3] + z * transform[6] + transform[9];
            destinationPoints[1] = x * transform[1] + y * transform[4] + z * transform[7] + transform[10];
            destinationPoints[2] = x * transform[2] + y * transform[5] + z * transform[8] + transform[11];
        }
        sourcePoints = sourcePoints + 3;
        destinationPoints = destinationPoints + 3;
        pointFlags = pointFlags + 1;
    }
    return;
}

// FUNCTION: EQGFX 0x1004d6af
// Transforms every 3D point into its destination position.
void __cdecl s3dTransformPoints3D_guess(float *sourcePoints, float *destinationPoints, float *transform, int pointCount)
{
    float x;
    float y;
    float z;

    for (; pointCount != 0; pointCount = pointCount - 1)
    {
        x = sourcePoints[0];
        y = sourcePoints[1];
        z = sourcePoints[2];
        sourcePoints = sourcePoints + 3;
        destinationPoints[0] = x * transform[0] + z * transform[6] + y * transform[3] + transform[9];
        destinationPoints[1] = x * transform[1] + z * transform[7] + y * transform[4] + transform[10];
        destinationPoints[2] = x * transform[2] + z * transform[8] + y * transform[5] + transform[11];
        destinationPoints = destinationPoints + 3;
    }
    return;
}

#define EQGFX_JITTER_SCALE_GUESS EQ_GLOBAL(float, 0x100762E8)
#define EQGFX_ROUND_CHANNEL EQ_CDECL(void, 0x1004EC8D, (float, int *))
#define EQGFX_CHANNEL_SIGN_BIT 0x80000000u
#define EQGFX_CHANNEL_UPPER_BITS 0xffffff00u
#define EQGFX_CHANNEL_MAX_BITS 0xffu

// FUNCTION: EQGFX 0x1004d726
// Converts enabled three-component values to byte-range integers; their rendering role is uncertain.
unsigned int __cdecl s3dConvertVertexColorChannels_guess(float *sourcePoints, int pointCount, char *vertexEnabled)
{
    float *endPoint;
    float *thirdChannel;
    float *middleChannel;
    unsigned int returnValue;
    unsigned int channelBits;

    endPoint = sourcePoints + pointCount * 3;
    if (sourcePoints >= endPoint)
    {
        return (unsigned int)endPoint;
    }
    thirdChannel = sourcePoints + 2;
    do
    {
        returnValue = (unsigned int)vertexEnabled;
        if (*vertexEnabled != '\0')
        {
            EQGFX_ROUND_CHANNEL(*sourcePoints * EQGFX_JITTER_SCALE_GUESS, (int *)sourcePoints);
            channelBits = *(unsigned int *)sourcePoints;
            if ((channelBits & EQGFX_CHANNEL_SIGN_BIT) == 0)
            {
                if ((channelBits & EQGFX_CHANNEL_UPPER_BITS) != 0)
                {
                    *(unsigned int *)sourcePoints = EQGFX_CHANNEL_MAX_BITS;
                }
            }
            else
            {
                *sourcePoints = 0.0f;
            }

            middleChannel = thirdChannel - 1;
            EQGFX_ROUND_CHANNEL(*middleChannel * EQGFX_JITTER_SCALE_GUESS, (int *)middleChannel);
            channelBits = *(unsigned int *)middleChannel;
            if ((channelBits & EQGFX_CHANNEL_SIGN_BIT) == 0)
            {
                if ((channelBits & EQGFX_CHANNEL_UPPER_BITS) != 0)
                {
                    *(unsigned int *)middleChannel = EQGFX_CHANNEL_MAX_BITS;
                }
            }
            else
            {
                *middleChannel = 0.0f;
            }

            EQGFX_ROUND_CHANNEL(*thirdChannel * EQGFX_JITTER_SCALE_GUESS, (int *)thirdChannel);
            channelBits = *(unsigned int *)thirdChannel;
            returnValue = channelBits;
            if ((channelBits & EQGFX_CHANNEL_SIGN_BIT) == 0)
            {
                if ((channelBits & EQGFX_CHANNEL_UPPER_BITS) != 0)
                {
                    *(unsigned int *)thirdChannel = EQGFX_CHANNEL_MAX_BITS;
                }
            }
            else
            {
                *thirdChannel = 0.0f;
            }
        }
        sourcePoints = sourcePoints + 3;
        thirdChannel = thirdChannel + 3;
        vertexEnabled = vertexEnabled + 1;
    } while (sourcePoints < endPoint);
    return returnValue;
}

// FUNCTION: EQGFX 0x1004d7fa
// Converts enabled triples to byte-range integers modulated by a supplied three-byte color.
void __cdecl s3dConvertTintedVertexColorChannels_guess(float *colorValues, int vertexCount, char *vertexEnabled, unsigned char *tintColor)
{
    float *endValues;
    float *thirdChannel;
    float *middleChannel;
    unsigned int channelBits;
    unsigned int firstTint;
    unsigned int secondTint;
    unsigned int thirdTint;

    firstTint = tintColor[2];
    secondTint = tintColor[1];
    thirdTint = tintColor[0];
    endValues = colorValues + vertexCount * 3;
    if (colorValues >= endValues)
    {
        return;
    }
    thirdChannel = colorValues + 2;
    do
    {
        if (*vertexEnabled != '\0')
        {
            EQGFX_ROUND_CHANNEL(*colorValues * EQGFX_JITTER_SCALE_GUESS, (int *)colorValues);
            channelBits = *(unsigned int *)colorValues;
            if ((channelBits & EQGFX_CHANNEL_SIGN_BIT) == 0)
            {
                if ((channelBits & EQGFX_CHANNEL_UPPER_BITS) != 0)
                {
                    *(unsigned int *)colorValues = firstTint;
                }
                else
                {
                    *(unsigned int *)colorValues = (unsigned int)(((int)channelBits * (int)firstTint) >> 8);
                }
            }
            else
            {
                *colorValues = 0.0f;
            }
            middleChannel = thirdChannel - 1;
            EQGFX_ROUND_CHANNEL(*middleChannel * EQGFX_JITTER_SCALE_GUESS, (int *)middleChannel);
            channelBits = *(unsigned int *)middleChannel;
            if ((channelBits & EQGFX_CHANNEL_SIGN_BIT) == 0)
            {
                if ((channelBits & EQGFX_CHANNEL_UPPER_BITS) != 0)
                {
                    *(unsigned int *)middleChannel = secondTint;
                }
                else
                {
                    *(unsigned int *)middleChannel = (unsigned int)(((int)channelBits * (int)secondTint) >> 8);
                }
            }
            else
            {
                *middleChannel = 0.0f;
            }
            EQGFX_ROUND_CHANNEL(*thirdChannel * EQGFX_JITTER_SCALE_GUESS, (int *)thirdChannel);
            channelBits = *(unsigned int *)thirdChannel;
            if ((channelBits & EQGFX_CHANNEL_SIGN_BIT) == 0)
            {
                if ((channelBits & EQGFX_CHANNEL_UPPER_BITS) != 0)
                {
                    *(unsigned int *)thirdChannel = thirdTint;
                }
                else
                {
                    *(unsigned int *)thirdChannel = (unsigned int)(((int)channelBits * (int)thirdTint) >> 8);
                }
            }
            else
            {
                *thirdChannel = 0.0f;
            }
        }
        colorValues = colorValues + 3;
        thirdChannel = thirdChannel + 3;
        vertexEnabled = vertexEnabled + 1;
    } while (colorValues < endValues);
    return;
}

// FUNCTION: EQGFX 0x1004d902
// Projects enabled textured vertices onto the screen and provides a pointer for each emitted vertex.
float *__cdecl s3dProjectTexturedVerticesToPointers_guess(int vertexCount, char *vertexEnabled, float *sourceCoordinates,
                            float *textureCoordinates, unsigned int *vertexComponents,
                            EqgfxVertexProjector_guess *projector, float *outputVertices,
                            float **outputPointers)
{
    float reciprocalDepth;
    unsigned int packedComponents;

    if (vertexCount == 0)
    {
        return outputVertices;
    }
    do
    {
        if (*vertexEnabled != '\0')
        {
            reciprocalDepth = 1.0f / sourceCoordinates[2];
            packedComponents = (projector->packedPrefix << 8) | vertexComponents[0];
            packedComponents = (packedComponents << 8) | vertexComponents[1];
            packedComponents = (packedComponents << 8) | vertexComponents[2];
            ((unsigned int *)outputVertices)[4] = packedComponents;
            outputVertices[3] = reciprocalDepth;
            outputVertices[6] = (projector->horizontalOffset + textureCoordinates[0]) *
                                projector->horizontalScale * reciprocalDepth;
            outputVertices[7] = (textureCoordinates[1] + projector->verticalOffset) *
                                projector->verticalScale * reciprocalDepth;
            outputVertices[0] = projector->horizontalPositionScale * sourceCoordinates[0] *
                                reciprocalDepth + projector->horizontalPositionOffset;
            outputVertices[1] = projector->verticalPositionOffset - sourceCoordinates[1] *
                                projector->verticalPositionScale * reciprocalDepth;
            outputVertices[2] = projector->depthValue - projector->depthCorrection *
                                projector->depthValue * reciprocalDepth;
            *outputPointers = outputVertices;
            outputVertices = outputVertices + 8;
        }
        outputPointers = outputPointers + 1;
        sourceCoordinates = sourceCoordinates + 3;
        textureCoordinates = textureCoordinates + 2;
        vertexComponents = vertexComponents + 3;
        vertexEnabled = vertexEnabled + 1;
        vertexCount = vertexCount + -1;
    } while (vertexCount != 0);
    return outputVertices;
}

// FUNCTION: EQGFX 0x1004d9bb
// Projects enabled 3D vertices into screen coordinates for rasterization.
float *__cdecl s3dProjectEnabledVertices_guess(int vertexCount, char *vertexEnabled, float *sourceCoordinates,
                            unsigned int *vertexComponents, EqgfxVertexTransform_guess *screenTransform,
                            float *outputVertices, float **outputPointers)
{
    float reciprocalDepth;

    if (vertexCount == 0)
    {
        return outputVertices;
    }
    do
    {
        if (*vertexEnabled != '\0')
        {
            reciprocalDepth = 1.0f / sourceCoordinates[2];
            ((unsigned int *)outputVertices)[4] = ((screenTransform->packedPrefix << 8 |
                                                    vertexComponents[0]) << 8 |
                                                   vertexComponents[1]) << 8 | vertexComponents[2];
            outputVertices[3] = reciprocalDepth;
            outputVertices[0] = screenTransform->horizontalScale * sourceCoordinates[0] * reciprocalDepth +
                                screenTransform->horizontalOffset;
            outputVertices[1] = screenTransform->verticalOffset - sourceCoordinates[1] *
                                screenTransform->verticalScale * reciprocalDepth;
            outputVertices[2] = screenTransform->depthOffset - screenTransform->depthScale *
                                screenTransform->depthOffset * reciprocalDepth;
            *outputPointers = outputVertices;
            outputVertices = outputVertices + 8;
        }
        sourceCoordinates = sourceCoordinates + 3;
        vertexComponents = vertexComponents + 3;
        vertexEnabled = vertexEnabled + 1;
        outputPointers = outputPointers + 1;
        vertexCount = vertexCount + -1;
    } while (vertexCount != 0);
    return outputVertices;
}

#define EQGFX_ATTENUATION_MAX 0xff
#define EQGFX_ATTENUATION_SHIFT 24

// FUNCTION: EQGFX 0x1004da4e
// Prepares indexed textured vertices for screen rendering with distance fog.
float *__cdecl s3dProjectIndexedTexturedFogVertices_guess(int vertexCount, int outputIndex, char *vertexEnabled,
                            float *sourceCoordinates, float *textureCoordinates,
                            unsigned int *vertexComponents, EqgfxVertexProjector_guess *projector,
                            float *outputVertices, int *outputIndices)
{
    int remainingVertices;
    int attenuation;
    float reciprocalDepth;
    float *outputCursor;
    float *nextOutput;

    if (vertexCount == 0)
    {
        outputCursor = outputVertices;
        return outputCursor;
    }
    remainingVertices = vertexCount;
    nextOutput = outputVertices;
    do
    {
        outputCursor = nextOutput;
            if (*vertexEnabled != '\0')
            {
                reciprocalDepth = 1.0f / sourceCoordinates[2];
                ((unsigned int *)nextOutput)[4] = (((projector->packedPrefix << 8 |
                                                      vertexComponents[0]) << 8 |
                                                     vertexComponents[1]) << 8 |
                                                    vertexComponents[2]);
                EQGFX_ROUND_CHANNEL(
                    (sourceCoordinates[2] - projector->verticalScale) * projector->horizontalScale,
                    &attenuation);
                if (attenuation < EQGFX_ATTENUATION_MAX)
                {
                    if (attenuation < 0)
                        attenuation = 0;
                }
                else
                {
                    attenuation = EQGFX_ATTENUATION_MAX;
                }
                attenuation = EQGFX_ATTENUATION_MAX - attenuation;
                ((unsigned int *)nextOutput)[5] = attenuation << EQGFX_ATTENUATION_SHIFT;
                nextOutput[3] = reciprocalDepth;
                nextOutput[6] = projector->horizontalOffset + textureCoordinates[0];
                nextOutput[7] = textureCoordinates[1] + projector->verticalOffset;
                nextOutput[0] = projector->horizontalPositionScale * sourceCoordinates[0] *
                                reciprocalDepth + projector->horizontalPositionOffset;
                outputCursor = nextOutput + 8;
                nextOutput[1] = projector->verticalPositionOffset - sourceCoordinates[1] *
                                projector->verticalPositionScale * reciprocalDepth;
                nextOutput[2] = projector->depthValue - projector->depthCorrection *
                                projector->depthValue * reciprocalDepth;
                *outputIndices = outputIndex;
                outputIndex = outputIndex + 1;
            }
            textureCoordinates = textureCoordinates + 2;
            vertexComponents = vertexComponents + 3;
            outputIndices = outputIndices + 1;
            sourceCoordinates = sourceCoordinates + 3;
            vertexEnabled = vertexEnabled + 1;
            remainingVertices = remainingVertices + -1;
            nextOutput = outputCursor;
        } while (remainingVertices != 0);
    return outputCursor;
}

// FUNCTION: EQGFX 0x1004db4c
float *__cdecl s3dProjectIndexedTexturedFogVerticesWithColorScale_guess(int vertexCount, int outputIndex, char *vertexEnabled,
                            float *sourceCoordinates, float *textureCoordinates,
                            unsigned int *vertexComponents, EqgfxVertexProjector_guess *projector,
                            float *outputVertices, int *outputIndices)
{
    int remainingVertices;
    int attenuation;
    float reciprocalDepth;
    float *outputCursor;
    float *nextOutput;

    if (vertexCount == 0)
    {
        outputCursor = outputVertices;
        return outputCursor;
    }
    remainingVertices = vertexCount;
    nextOutput = outputVertices;
    do
    {
        outputCursor = nextOutput;
            if (*vertexEnabled != '\0')
            {
                reciprocalDepth = 1.0f / sourceCoordinates[2];
                ((unsigned int *)nextOutput)[4] = (unsigned int)(((((int)vertexComponents[0] * (int)projector->packedPrefix & 0xffffff00) | 0xffff0000) << 8 | (int)vertexComponents[2] * (int)projector->packedPrefix >> 8) | ((int)vertexComponents[1] * (int)projector->packedPrefix & 0xffffff00));
                EQGFX_ROUND_CHANNEL(
                    (sourceCoordinates[2] - projector->verticalScale) * projector->horizontalScale,
                    &attenuation);
                if (attenuation < EQGFX_ATTENUATION_MAX)
                {
                    if (attenuation < 0)
                        attenuation = 0;
                }
                else
                {
                    attenuation = EQGFX_ATTENUATION_MAX;
                }
                attenuation = EQGFX_ATTENUATION_MAX - attenuation;
                ((unsigned int *)nextOutput)[5] = attenuation << EQGFX_ATTENUATION_SHIFT;
                nextOutput[3] = reciprocalDepth;
                nextOutput[6] = projector->horizontalOffset + textureCoordinates[0];
                nextOutput[7] = textureCoordinates[1] + projector->verticalOffset;
                nextOutput[0] = projector->horizontalPositionScale * sourceCoordinates[0] *
                                reciprocalDepth + projector->horizontalPositionOffset;
                outputCursor = nextOutput + 8;
                nextOutput[1] = projector->verticalPositionOffset - sourceCoordinates[1] *
                                projector->verticalPositionScale * reciprocalDepth;
                nextOutput[2] = projector->depthValue - projector->depthCorrection *
                                projector->depthValue * reciprocalDepth;
                *outputIndices = outputIndex;
                outputIndex = outputIndex + 1;
            }
            textureCoordinates = textureCoordinates + 2;
            vertexComponents = vertexComponents + 3;
            outputIndices = outputIndices + 1;
            sourceCoordinates = sourceCoordinates + 3;
            vertexEnabled = vertexEnabled + 1;
            remainingVertices = remainingVertices + -1;
            nextOutput = outputCursor;
        } while (remainingVertices != 0);
    return outputCursor;
}

// FUNCTION: EQGFX 0x1004dc63
// Prepares indexed untextured vertices for screen rendering with distance fog.
float *__cdecl s3dProjectIndexedUntexturedFogVertices_guess(int vertexCount, int outputIndex, char *vertexEnabled,
                            float *sourceCoordinates, unsigned int *vertexComponents,
                            EqgfxVertexTransform_guess *transform, float *outputVertices,
                            int *outputIndices)
{
    int remainingVertices;
    int attenuation;
    float reciprocalDepth;
    float *outputCursor;
    float *nextOutput;

    outputCursor = outputVertices;
    if (vertexCount != 0)
    {
        remainingVertices = vertexCount;
        nextOutput = outputVertices;
        do
        {
            outputCursor = nextOutput;
            if (*vertexEnabled != '\0')
            {
                reciprocalDepth = 1.0f / sourceCoordinates[2];
                ((unsigned int *)nextOutput)[4] = (((transform->packedPrefix << 8 |
                                                      vertexComponents[0]) << 8 |
                                                     vertexComponents[1]) << 8 |
                                                    vertexComponents[2]);
                EQGFX_ROUND_CHANNEL(
                    (sourceCoordinates[2] - transform->referenceDepth) * transform->contributionScale,
                    &attenuation);
                if (attenuation < EQGFX_ATTENUATION_MAX)
                {
                    if (attenuation < 0)
                        attenuation = 0;
                }
                else
                {
                    attenuation = EQGFX_ATTENUATION_MAX;
                }
                attenuation = EQGFX_ATTENUATION_MAX - attenuation;
                ((unsigned int *)nextOutput)[5] = attenuation << EQGFX_ATTENUATION_SHIFT;
                nextOutput[3] = reciprocalDepth;
                nextOutput[0] = transform->horizontalScale * sourceCoordinates[0] * reciprocalDepth +
                                transform->horizontalOffset;
                outputCursor = nextOutput + 8;
                nextOutput[1] = transform->verticalOffset - sourceCoordinates[1] *
                                transform->verticalScale * reciprocalDepth;
                nextOutput[2] = transform->depthOffset - transform->depthScale *
                                transform->depthOffset * reciprocalDepth;
                *outputIndices = outputIndex;
                outputIndex = outputIndex + 1;
            }
            vertexComponents = vertexComponents + 3;
            outputIndices = outputIndices + 1;
            sourceCoordinates = sourceCoordinates + 3;
            vertexEnabled = vertexEnabled + 1;
            remainingVertices = remainingVertices + -1;
            nextOutput = outputCursor;
        } while (remainingVertices != 0);
    }
    return outputCursor;
}

// FUNCTION: EQGFX 0x1004dd49
float *__cdecl s3dProjectIndexedUntexturedFogVerticesWithColorScale_guess(int vertexCount, int outputIndex, char *vertexEnabled,
                            float *sourceCoordinates, unsigned int *vertexComponents,
                            EqgfxVertexTransform_guess *transform, float *outputVertices,
                            int *outputIndices)
{
    int remainingVertices;
    int attenuation;
    float reciprocalDepth;
    float *outputCursor;
    float *nextOutput;

    outputCursor = outputVertices;
    remainingVertices = vertexCount;
    if (remainingVertices != 0)
    {
        nextOutput = outputVertices;
        do
        {
            outputCursor = nextOutput;
            if (*vertexEnabled != '\0')
            {
                reciprocalDepth = 1.0f / sourceCoordinates[2];
                ((unsigned int *)nextOutput)[4] = (unsigned int)(((((int)vertexComponents[0] * (int)transform->packedPrefix & 0xffffff00) | 0xffff0000) << 8 | (int)vertexComponents[2] * (int)transform->packedPrefix >> 8) | ((int)vertexComponents[1] * (int)transform->packedPrefix & 0xffffff00));
                EQGFX_ROUND_CHANNEL(
                    (sourceCoordinates[2] - transform->referenceDepth) * transform->contributionScale,
                    &attenuation);
                if (attenuation < EQGFX_ATTENUATION_MAX)
                {
                    if (attenuation < 0)
                        attenuation = 0;
                }
                else
                {
                    attenuation = EQGFX_ATTENUATION_MAX;
                }
                attenuation = EQGFX_ATTENUATION_MAX - attenuation;
                ((unsigned int *)nextOutput)[5] = attenuation << EQGFX_ATTENUATION_SHIFT;
                nextOutput[3] = reciprocalDepth;
                nextOutput[0] = transform->horizontalScale * sourceCoordinates[0] * reciprocalDepth +
                                transform->horizontalOffset;
                outputCursor = nextOutput + 8;
                nextOutput[1] = transform->verticalOffset - sourceCoordinates[1] *
                                transform->verticalScale * reciprocalDepth;
                nextOutput[2] = transform->depthOffset - transform->depthScale *
                                transform->depthOffset * reciprocalDepth;
                *outputIndices = outputIndex;
                outputIndex = outputIndex + 1;
            }
            vertexComponents = vertexComponents + 3;
            outputIndices = outputIndices + 1;
            sourceCoordinates = sourceCoordinates + 3;
            vertexEnabled = vertexEnabled + 1;
            remainingVertices = remainingVertices + -1;
            nextOutput = outputCursor;
        } while (remainingVertices != 0);
    }
    return outputCursor;
}

// FUNCTION: EQGFX 0x1004de48
// Applies a positive plane-based effect to enabled vertices during rendering.
void __cdecl s3dApplyPositivePlaneContribution_guess(float *planeCoefficients, float *contributionScale,
                          float *sourceVertices, float *destinationVertices,
                          int vertexCount, char *vertexEnabled)
{
    float coefficientX;
    float coefficientY;
    float coefficientZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    float contribution;

    coefficientX = planeCoefficients[0];
    coefficientY = planeCoefficients[1];
    coefficientZ = planeCoefficients[2];
    scaleX = contributionScale[0];
    scaleY = contributionScale[1];
    scaleZ = contributionScale[2];
    for (; vertexCount != 0; vertexCount = vertexCount + -1)
    {
        if ((*vertexEnabled != '\0') &&
            (contribution = -(coefficientX * sourceVertices[0] +
                              coefficientY * sourceVertices[1] +
                              coefficientZ * sourceVertices[2]),
             contribution > EQGFX_ZERO_F_GUESS))
        {
            destinationVertices[0] = contribution * scaleX + destinationVertices[0];
            destinationVertices[1] = contribution * scaleY + destinationVertices[1];
            destinationVertices[2] = contribution * scaleZ + destinationVertices[2];
        }
        sourceVertices = sourceVertices + 3;
        destinationVertices = destinationVertices + 3;
        vertexEnabled = vertexEnabled + 1;
    }
    return;
}

// FUNCTION: EQGFX 0x1004decf
// Adds weighted directional contributions to enabled vertex triples; the lighting role is uncertain.
void __cdecl s3dAccumulateWeightedDirectionalLighting_guess(float *planeCoefficients, float *contributionScale,
                          float *sourceVertices, int weightOffset,
                          float *destinationVertices, int vertexCount,
                          char *vertexEnabled)
{
    float coefficientX;
    float coefficientY;
    float coefficientZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    float contribution;
    unsigned char *weightIndices;

    coefficientX = planeCoefficients[0];
    coefficientY = planeCoefficients[1];
    coefficientZ = planeCoefficients[2];
    scaleX = contributionScale[0];
    scaleY = contributionScale[1];
    scaleZ = contributionScale[2];
    if (vertexCount != 0)
    {
        weightIndices = (unsigned char *)(weightOffset + 3);
        do
        {
            if ((*vertexEnabled != '\0') &&
                (contribution = -(coefficientX * sourceVertices[0] +
                                  coefficientY * sourceVertices[1] +
                                  coefficientZ * sourceVertices[2]),
                 EQGFX_ZERO_F_GUESS < contribution))
            {
                contribution = contribution * g_byteFloatTable_guess[*weightIndices];
                destinationVertices[0] = contribution * scaleX + destinationVertices[0];
                destinationVertices[1] = contribution * scaleY + destinationVertices[1];
                destinationVertices[2] = contribution * scaleZ + destinationVertices[2];
            }
            sourceVertices = sourceVertices + 3;
            destinationVertices = destinationVertices + 3;
            weightIndices = weightIndices + 4;
            vertexEnabled = vertexEnabled + 1;
            vertexCount = vertexCount + -1;
        } while (vertexCount != 0);
    }
    return;
}

// FUNCTION: EQGFX 0x1004DF6D
// Adds point-light contributions to enabled vertices, fading with distance and surface angle.
void __cdecl t3dAccumulatePointLight_guess(float *lightPosition, float *lightColor, float *sourceVertices,
                          float *vertexNormals, float *destinationVertices,
                          int vertexCount, char *vertexEnabled)
{
    float lightX;
    float lightY;
    float lightZ;
    float colorX;
    float colorY;
    float colorZ;
    float dot;
    float invSqrt;

    lightX = lightPosition[0];
    lightY = lightPosition[1];
    lightZ = lightPosition[2];
    colorX = lightColor[0];
    colorY = lightColor[1];
    colorZ = lightColor[2];
    if (vertexCount != 0)
    {
        do
        {
            if (*vertexEnabled != '\0')
            {
                dot = (lightX - sourceVertices[0]) * vertexNormals[0] +
                    (lightY - sourceVertices[1]) * vertexNormals[1] +
                    (lightZ - sourceVertices[2]) * vertexNormals[2];
                if (EQGFX_ZERO_F_GUESS < dot)
                {
                    invSqrt = EQGFX_INV_SQRT((lightX - sourceVertices[0]) * (lightX - sourceVertices[0]) +
                        (lightY - sourceVertices[1]) * (lightY - sourceVertices[1]) +
                        (lightZ - sourceVertices[2]) * (lightZ - sourceVertices[2]));
                    destinationVertices[0] = invSqrt * invSqrt * invSqrt * dot * colorX +
                        destinationVertices[0];
                    destinationVertices[1] = invSqrt * invSqrt * invSqrt * dot * colorY +
                        destinationVertices[1];
                    destinationVertices[2] = invSqrt * invSqrt * invSqrt * dot * colorZ +
                        destinationVertices[2];
                }
            }
            sourceVertices = sourceVertices + 3;
            vertexNormals = vertexNormals + 3;
            destinationVertices = destinationVertices + 3;
            vertexEnabled = vertexEnabled + 1;
            vertexCount = vertexCount + -1;
        } while (vertexCount != 0);
    }
    return;
}

// FUNCTION: EQGFX 0x1004E11F
// Multiplies fixed-point values used in renderer calculations.
unsigned int __cdecl t3dFix16Mult(int left, int right)
{
    union
    {
        __int64 value;
        struct
        {
            unsigned int lowWord;
            unsigned int highWord;
        } parts;
    } product;

    product.value = (__int64)left * (__int64)right;
    return (product.parts.lowWord >> 16) | (product.parts.highWord << 16);
}

typedef void (__stdcall *EqgfxOutputDebugStringAFn)(const char *message);

#define EQGFX_FIX16_SAFE_DIV_OUTPUT_DEBUG_STRING_A EQ_GLOBAL(EqgfxOutputDebugStringAFn, 0x100760C4)
#define EQGFX_FIX16_SAFE_DIV_MIN_RESULT 0x80000001u
#define EQGFX_FIX16_SAFE_DIV_MAX_RESULT 0x7fffffffu
#define EQGFX_FIX16_SAFE_DIV_OVERFLOW_MASK 0xffff8000u
#define EQGFX_FIX16_SAFE_DIV_DIVIDE_BY_ZERO_MESSAGE ((const char *)0x100a355c)
#define EQGFX_FIX16_SAFE_DIV_POSITIVE_OVERFLOW_MESSAGE ((const char *)0x100a3534)
#define EQGFX_FIX16_SAFE_DIV_NUMERATOR_OVERFLOW_MESSAGE ((const char *)0x100a3508)
#define EQGFX_FIX16_SAFE_DIV_DENOMINATOR_OVERFLOW_MESSAGE ((const char *)0x100a34d8)
#define EQGFX_FIX16_SAFE_DIV_NEGATIVE_OVERFLOW_MESSAGE ((const char *)0x100a34b0)

// FUNCTION: EQGFX 0x1004E130
// Divides 16.16 fixed-point values with safeguards for zero divisors and overflow.
int __cdecl t3dFix16SafeDiv(int numerator, int denominator)
{
    unsigned int numeratorMagnitude;
    unsigned int denominatorMagnitude;
    unsigned int quotientMagnitude;
    unsigned int wholePart;
    unsigned int fractionalPart;
    unsigned int remainder;
    unsigned int fixedResult;
    unsigned int fractionBit;

    if (denominator == 0)
    {
        EQGFX_FIX16_SAFE_DIV_OUTPUT_DEBUG_STRING_A(EQGFX_FIX16_SAFE_DIV_DIVIDE_BY_ZERO_MESSAGE);
        if (numerator < 0)
            numerator = (int)EQGFX_FIX16_SAFE_DIV_MIN_RESULT;
        else if (numerator != 0)
            numerator = (int)EQGFX_FIX16_SAFE_DIV_MAX_RESULT;
    }
    else if (numerator != 0)
    {
        if ((numerator | denominator) < 0)
        {
            numeratorMagnitude = (unsigned int)numerator;
            if (numerator < 0)
            {
                numeratorMagnitude = 0u - (unsigned int)numerator;
                if (numeratorMagnitude == 0x80000000u)
                {
                    EQGFX_FIX16_SAFE_DIV_OUTPUT_DEBUG_STRING_A(EQGFX_FIX16_SAFE_DIV_NUMERATOR_OVERFLOW_MESSAGE);
                    numeratorMagnitude = EQGFX_FIX16_SAFE_DIV_MAX_RESULT;
                    numerator = (int)EQGFX_FIX16_SAFE_DIV_MIN_RESULT;
                }
            }

            denominatorMagnitude = (unsigned int)denominator;
            if (denominator < 0)
            {
                denominatorMagnitude = 0u - (unsigned int)denominator;
                if (denominatorMagnitude == 0x80000000u)
                {
                    EQGFX_FIX16_SAFE_DIV_OUTPUT_DEBUG_STRING_A(EQGFX_FIX16_SAFE_DIV_DENOMINATOR_OVERFLOW_MESSAGE);
                    denominatorMagnitude = EQGFX_FIX16_SAFE_DIV_MAX_RESULT;
                    denominator = (int)EQGFX_FIX16_SAFE_DIV_MIN_RESULT;
                }
            }

            if ((denominatorMagnitude & 0xffff0000u) == 0 &&
                (numeratorMagnitude & EQGFX_FIX16_SAFE_DIV_OVERFLOW_MASK) != 0)
            {
                quotientMagnitude = (unsigned int)((int)numeratorMagnitude / (int)denominatorMagnitude);
                if ((quotientMagnitude & EQGFX_FIX16_SAFE_DIV_OVERFLOW_MASK) != 0)
                {
                    if ((numerator ^ denominator) < 0)
                    {
                        EQGFX_FIX16_SAFE_DIV_OUTPUT_DEBUG_STRING_A(EQGFX_FIX16_SAFE_DIV_NEGATIVE_OVERFLOW_MESSAGE);
                        numerator = (int)EQGFX_FIX16_SAFE_DIV_MIN_RESULT;
                    }
                    else
                    {
                        EQGFX_FIX16_SAFE_DIV_OUTPUT_DEBUG_STRING_A(EQGFX_FIX16_SAFE_DIV_POSITIVE_OVERFLOW_MESSAGE);
                        numerator = (int)EQGFX_FIX16_SAFE_DIV_MAX_RESULT;
                    }
                    goto finish;
                }
            }
        }
        else if ((denominator & 0xffff0000) == 0 &&
                 ((unsigned int)numerator & EQGFX_FIX16_SAFE_DIV_OVERFLOW_MASK) != 0 &&
                 ((unsigned int)numerator > 0x00ffffffu || (unsigned int)denominator < 0x200u))
        {
            quotientMagnitude = (unsigned int)(numerator / denominator);
            if ((quotientMagnitude & EQGFX_FIX16_SAFE_DIV_OVERFLOW_MASK) != 0)
            {
                EQGFX_FIX16_SAFE_DIV_OUTPUT_DEBUG_STRING_A(EQGFX_FIX16_SAFE_DIV_POSITIVE_OVERFLOW_MESSAGE);
                numerator = (int)EQGFX_FIX16_SAFE_DIV_MAX_RESULT;
                goto finish;
            }
        }

        numeratorMagnitude = numerator < 0 ? 0u - (unsigned int)numerator : (unsigned int)numerator;
        denominatorMagnitude = denominator < 0 ? 0u - (unsigned int)denominator : (unsigned int)denominator;
        wholePart = numeratorMagnitude / denominatorMagnitude;
        remainder = numeratorMagnitude % denominatorMagnitude;
        fractionalPart = 0;
        for (fractionBit = 0; fractionBit < 16; fractionBit++)
        {
            fractionalPart = fractionalPart << 1;
            if (remainder >= denominatorMagnitude - remainder)
            {
                remainder = remainder - (denominatorMagnitude - remainder);
                fractionalPart = fractionalPart | 1;
            }
            else
            {
                remainder = remainder + remainder;
            }
        }
        fixedResult = (wholePart << 16) | fractionalPart;
        if ((numerator ^ denominator) < 0)
            fixedResult = 0u - fixedResult;
        numerator = (int)fixedResult;
    }

finish:
    return numerator;
}

// FUNCTION: EQGFX 0x1004E268
// Divides fixed-point values used in renderer calculations.
int __cdecl t3dFix16Div(int dividend, int divisor)
{
    union
    {
        __int64 value;
        struct
        {
            unsigned int lowWord;
            unsigned int highWord;
        } parts;
    } scaled;

    scaled.parts.lowWord = (unsigned int)dividend << 16;
    scaled.parts.highWord = (unsigned int)(dividend >> 16);
    return (int)(scaled.value / (__int64)divisor);
}

// FUNCTION: EQGFX 0x1004E280
// Computes a square root in 16.16 fixed-point arithmetic.
unsigned int __cdecl t3dFix16Sqrt(unsigned int value)
{
    unsigned int highestSetBit;
    unsigned int normalizationShift;
    unsigned short tableValue;
    int normalizedValue;
    char halfBitIndex;
    unsigned int rootEstimate;
    int squareError;
    union
    {
        __int64 value;
        struct
        {
            unsigned int lowWord;
            unsigned int highWord;
        } parts;
    } squareProduct;

    if (value == 0)
    {
        rootEstimate = 0;
    }
    else
    {
        if ((int)value >= 0)
        {
            highestSetBit = 0x1e;
            do
            {
                if ((value & (1 << (highestSetBit & 0x1f))) != 0)
                    break;
                highestSetBit--;
            } while (highestSetBit > 0xb);

            normalizationShift = highestSetBit - 0xb;
            normalizedValue = (int)value >> (normalizationShift & 0x1f);
            if (normalizedValue <= 0xfff)
            {
                if (highestSetBit < 0xf)
                {
                    halfBitIndex = (char)(highestSetBit >> 1);
                    if ((normalizationShift & 1) == 0)
                        tableValue = EQGFX_FIX16_SQRT_TABLE->evenScale[normalizedValue];
                    else
                        tableValue = EQGFX_FIX16_SQRT_TABLE->oddScale[normalizedValue];
                    rootEstimate = (unsigned int)(tableValue >> (7U - halfBitIndex & 0x1f));
                }
                else
                {
                    if ((normalizationShift & 1) == 0)
                        tableValue = EQGFX_FIX16_SQRT_TABLE->evenScale[normalizedValue];
                    else
                        tableValue = EQGFX_FIX16_SQRT_TABLE->oddScale[normalizedValue];
                    rootEstimate = (unsigned int)tableValue << ((char)(highestSetBit >> 1) - 7U & 0x1f);
                }

                if (highestSetBit < 0xc)
                    return rootEstimate;

                squareProduct.value = __emul((int)rootEstimate, (int)rootEstimate);
                squareError = (int)((squareProduct.parts.lowWord >> 16 | squareProduct.parts.highWord << 16) - value);
                __asm
                {
                    mov edx, squareError
                    xor eax, eax
                    shrd eax, edx, 16
                    sar edx, 16
                    mov ebx, rootEstimate
                    lea ebx, [ebx + ebx]
                    idiv ebx
                    sub rootEstimate, eax
                }
                return rootEstimate;
            }
        }
        rootEstimate = EQGFX_FIX16_SQRT_ERROR;
    }
    return rootEstimate;
}

// FUNCTION: EQGFX 0x1004E35C
// Finds a direction angle from two fixed-point components and marks exact table matches.
int __cdecl t3dAngleArcTangentFix16(int horizontal, int vertical, int *isExact)
{
    int quadrant;
    int ratio;
    int tableIndex;
    EqgfxAngleArcNode_guess *node;
    EqgfxAngleArcNode_guess *previousNode;

    quadrant = 1;
    if (horizontal < 0)
    {
        if (vertical >= 0)
        {
            quadrant = 4;
            if (vertical == 0)
            {
                quadrant = EQGFX_ANGLE_ARC_THREE_QUARTER_TURN;
                goto setExactAndReturn;
            }
            goto makeVerticalPositive;
        }
        quadrant = 3;
    }
    else if (vertical <= 0)
    {
        quadrant = 2;
        if (vertical == 0)
        {
            quadrant = EQGFX_ANGLE_ARC_QUARTER_TURN;
            goto setExactAndReturn;
        }
makeVerticalPositive:
        vertical = -vertical;
    }

    ratio = EQGFX_FIXED_POINT_SAFE_DIVIDE(horizontal, vertical);
    node = (EqgfxAngleArcNode_guess *)(t3d_angle_tree_guess + EQGFX_ANGLE_ARC_TREE_ROOT_OFFSET);
    do
    {
        previousNode = node;
        if (ratio < previousNode->ratio)
        {
            node = previousNode->lower;
        }
        else
        {
            if (ratio <= previousNode->ratio)
            {
                *isExact = 1;
                goto foundNode;
            }
            node = previousNode->upper;
        }
    } while (node != previousNode);
    *isExact = 0;
foundNode:
    tableIndex = ((int)previousNode - (int)t3d_angle_tree_guess - EQGFX_ANGLE_ARC_NODE_BASE_OFFSET) /
        EQGFX_ANGLE_ARC_NODE_SIZE;
    if (quadrant == 1)
    {
        if (previousNode->ratio <= ratio)
            return tableIndex;
    }
    else
    {
        if (quadrant == 2)
        {
            quadrant = EQGFX_ANGLE_ARC_HALF_TURN;
        }
        else
        {
            if (quadrant == 3)
            {
                tableIndex = tableIndex + EQGFX_ANGLE_ARC_HALF_TURN;
                goto checkLowerAngle;
            }
            if (quadrant != 4)
            {
                EQGFX_DEBUG_BREAK();
                return tableIndex;
            }
            quadrant = EQGFX_ANGLE_ARC_FULL_TURN;
        }
        tableIndex = quadrant - tableIndex;
        if (ratio <= previousNode->ratio)
            return tableIndex;
        return tableIndex - 1;
    }
checkLowerAngle:
    if (ratio >= previousNode->ratio)
        return tableIndex;
    return tableIndex - 1;

setExactAndReturn:
    *isExact = 1;
    return quadrant;
}

// FUNCTION: EQGFX 0x1004E435
// Combines two 3-by-3 fixed-point matrices by matrix multiplication.
unsigned int __cdecl t3dMultFix16Coefficients(EqgfxFix16Matrix_guess *resultMatrix,
                                               const EqgfxFix16Matrix_guess *sourceMatrix,
                                               const EqgfxFix16Matrix_guess *coefficientMatrix)
{
    int source0;
    int source1;
    int source2;
    int product0;
    int product1;
    int product2;
    union
    {
        __int64 value;
        struct
        {
            unsigned int lowWord;
            unsigned int highWord;
        } parts;
    } product;

    source0 = sourceMatrix->values[0];
    source1 = sourceMatrix->values[1];
    source2 = sourceMatrix->values[2];
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[6]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[3]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[0]);
    resultMatrix->values[0] = product2 + product1 + product0;
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[7]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[4]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[1]);
    resultMatrix->values[1] = product2 + product1 + product0;
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[8]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[5]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[2]);
    resultMatrix->values[2] = product2 + product1 + product0;

    source0 = sourceMatrix->values[3];
    source1 = sourceMatrix->values[4];
    source2 = sourceMatrix->values[5];
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[6]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[3]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[0]);
    resultMatrix->values[3] = product2 + product1 + product0;
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[7]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[4]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[1]);
    resultMatrix->values[4] = product2 + product1 + product0;
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[8]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[5]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[2]);
    resultMatrix->values[5] = product2 + product1 + product0;

    source0 = sourceMatrix->values[6];
    source1 = sourceMatrix->values[7];
    source2 = sourceMatrix->values[8];
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[6]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[3]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[0]);
    resultMatrix->values[6] = product2 + product1 + product0;
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[7]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[4]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[1]);
    resultMatrix->values[7] = product2 + product1 + product0;
    EQGFX_FIX16_MULTIPLY(product0, source2, coefficientMatrix->values[8]);
    EQGFX_FIX16_MULTIPLY(product1, source1, coefficientMatrix->values[5]);
    EQGFX_FIX16_MULTIPLY(product2, source0, coefficientMatrix->values[2]);
    resultMatrix->values[8] = product2 + product1 + product0;
    return 1;
}
#define EQGFX_FIX16_FROM_FLOAT_UPPER_LIMIT EQ_GLOBAL(float, 0x10076384)
#define EQGFX_FIX16_FROM_FLOAT_LOWER_LIMIT EQ_GLOBAL(float, 0x10076380)
#define EQGFX_FIX16_FROM_FLOAT_SCALE_ADDRESS 100762e4h
#define EQGFX_FIX16_FROM_ST0 EQ_CDECL(int, 0x1004EC36, (void))

// FUNCTION: EQGFX 0x1004e05d
// Converts single-precision values into the engine's 16.16 format.
int __cdecl t3dFix16FromFloat(float value)
{
    int result;
    int negateResult;

    if (value > EQGFX_FIX16_FROM_FLOAT_UPPER_LIMIT)
        return 0x7fffffff;
    if (value < EQGFX_FIX16_FROM_FLOAT_LOWER_LIMIT)
        return -0x7fffffff;
    negateResult = 0;
    if (value < EQGFX_ZERO_F_GUESS)
    {
        value = -value;
        negateResult = 1;
    }
    __asm
    {
        fld value
        fmul dword ptr EQ_ASM_GLOBAL(0x100762E4)
    }
    result = EQGFX_FIX16_FROM_ST0();
    if (negateResult)
        result = -result;
    return result;
}

#define EQGFX_FIX16_FROM_DOUBLE_UPPER_BOUND EQ_GLOBAL(double, 0x10076398)
#define EQGFX_FIX16_FROM_DOUBLE_LOWER_BOUND EQ_GLOBAL(double, 0x10076390)
#define EQGFX_FIX16_FROM_DOUBLE_ZERO EQ_GLOBAL(double, 0x100762C8)
#define EQGFX_FIX16_FROM_DOUBLE_SCALE_ADDRESS EQ_GLOBAL(double, 0x10076388)

// FUNCTION: EQGFX 0x1004e0b3
// Converts double-precision values into the engine's 16.16 format.
int __cdecl t3dFix16FromDouble(double value)
{
    int result;
    int negateResult;

    if (value > EQGFX_FIX16_FROM_DOUBLE_UPPER_BOUND)
        return 0x7fffffff;
    if (value < EQGFX_FIX16_FROM_DOUBLE_LOWER_BOUND)
        return -0x7fffffff;
    negateResult = 0;
    if (value < EQGFX_FIX16_FROM_DOUBLE_ZERO)
    {
        value = -value;
        negateResult = 1;
    }
    __asm
    {
        fld value
        fmul qword ptr EQ_ASM_GLOBAL(0x10076388)
    }
    result = EQGFX_FIX16_FROM_ST0();
    if (negateResult)
        result = -result;
    return result;
}

// FUNCTION: EQGFX 0x1004e109
// __cdecl, float in ST0. 16.16 fixed-point -> float (times 1/65536).
float __cdecl t3dFloatFromFix16(int fixedValue)
{
    return (float)fixedValue * 1.5258789e-05f;
}

// FUNCTION: EQGFX 0x1004e114
// __cdecl, double in ST0. 16.16 fixed-point -> double (times 1/65536).
double __cdecl t3dDoubleFromFix16(int fixedValue)
{
    return (double)fixedValue * EQ_GLOBAL(double, 0x100763A8);
}
