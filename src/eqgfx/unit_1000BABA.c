// unit_1000BABA.c - eqgfx_dx7.dll, the address run 0x1000BABA..0x1000D9DB (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <stdio.h>
#include <string.h>

typedef struct BitmapSearchList_guess
{
    char pad0[0x24];
    int additionalPathCount;
    char pad1[0x8];
    void *pathStorage;
} BitmapSearchList_guess;

typedef unsigned int (__stdcall *GfxGetEnvironmentVariableA_guess_t)(const char *, char *, unsigned int);
typedef unsigned int (__stdcall *GfxSearchPathA_guess_t)(const char *, const char *, const char *, unsigned int, char *, char **);
typedef void (__cdecl *GfxCompositeDebugBreak_guess_t)(void);

typedef struct CompositeSpriteUndefineComponent_guess
{
    void *sprite;
    char padding[0x9c];
} CompositeSpriteUndefineComponent_guess;

typedef struct CompositeSpriteUndefineDefinition_guess
{
    char padding0[0x8];
    void *entry;
    unsigned char flags;
    char padding1[3];
    int referenceCount;
    void *volume;
    char padding2[0x10];
    CompositeSpriteUndefineComponent_guess *components;
} CompositeSpriteUndefineDefinition_guess;

typedef struct CompositeSpriteUndefineSprite_guess
{
    char padding0[0x8];
    void *entry;
    unsigned char flags;
    char padding1[3];
    int state;
    char padding2[4];
    int componentCount;
    void *ownedBuffer;
    char padding3[4];
    CompositeSpriteUndefineDefinition_guess *definition;
} CompositeSpriteUndefineSprite_guess;

#define GFX_WLD_PATH EQ_GLOBAL(const char *, 0x100A35D4)
#define GFX_GET_ENVIRONMENT_VARIABLE_A EQ_GLOBAL(GfxGetEnvironmentVariableA_guess_t, 0x10076074)
#define GFX_SEARCH_PATH_A EQ_GLOBAL(GfxSearchPathA_guess_t, 0x10076070)
#define EQGFX_VISIBILITY_MEMSET_GUESS EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))
#define GFX_PATH_STAT EQ_CDECL(int, 0x100685B4, (const char *, void *))
#define GFX_PATH_STRCHR EQ_CDECL(char *, 0x10068440, (const char *, int))
#define strlen_EQGFX EQ_CDECL(unsigned int, 0x10068160, (const char *))
#define GFX_PATH_COPY_STRING EQ_CDECL(void, 0x10068070, (char *, const char *))
#define EQGFX_T3D_FREE_MEMORY_GUESS EQ_CDECL(void, 0x10050CBB, (void *))
#define GFX_BITMAP_BUILD_MIP_LEVEL EQ_CDECL(int, 0x1000CA0F, (unsigned short *, unsigned int, unsigned int, unsigned int, unsigned short *, unsigned int))
#define GFX_BITMAP_INDEXED_MODE EQ_GLOBAL(int, 0x100F3FE8)
#define GFX_COMPOSITE_ALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C48, (unsigned int))
#define GFX_COMPOSITE_DUPLICATE_VOLUME EQ_CDECL(void *, 0x1001D516, (void *, void *))
#define GFX_COMPOSITE_DESTROY_VOLUME EQ_CDECL(void, 0x1001D5A1, (void *, void *))
#define EQGFX_MEMCPY_GUESS EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
#define GFX_COMPOSITE_INITIALIZE_SPRITE EQ_CDECL(int, 0x1001564B, (void *, T3DCompositeSprite_guess *))
#define GFX_COMPOSITE_SHARE_OBJECT EQ_CDECL(void, 0x10017CE8, (void *))
#define GFX_COMPOSITE_DEBUG_BREAK EQ_GLOBAL(GfxCompositeDebugBreak_guess_t, 0x100760BC)
#define GFX_EMPTY_OBJECT EQ_GLOBAL(T3DCompositeSpriteOwnedSprite_guess, 0x100F401C)
#define parseLookup EQ_CDECL(void *, 0x1000E7FE, (void *, char *))
#define parseAddToDictionary EQ_CDECL(int, 0x1000DC08, (void *, void *))
#define GFX_COMPOSITE_REMOVE_ENTRY EQ_CDECL(void, 0x1000E334, (void *, char *))
#define GFX_COMPOSITE_RELEASE_OBJECT EQ_CDECL(int, 0x10015D81, (void *, void *))
#define GFX_COMPOSITE_UNSHARE_OBJECT EQ_CDECL(void, 0x10017DAD, (void *))
#define GFX_COMPOSITE_INITIALIZE_OBJECT EQ_CDECL(int, 0x1001564B, (void *, void *))
#define COMPOSITE_SPRITE_COMPONENT_TYPE 0x0c
#define COMPOSITE_SPRITE_UNDEFINE_ERROR 0xffff3d44

#define GFX_PATH_BUFFER_SIZE 0x105
#define GFX_PATH_ALLOCATION_ERROR 0xffff3d3e

#define COMPOSITE_SPRITE_CREATE_SIZE 0x3c
#define COMPOSITE_SPRITE_CREATE_TEMPLATE_WORD_COUNT 0x0f
#define COMPOSITE_SPRITE_CREATE_COMPONENT_SIZE 0xa0
#define COMPOSITE_SPRITE_CREATE_SHARED_TYPE 0x0c
#define COMPOSITE_SPRITE_DEFINITION_SIZE 0x28
#define COMPOSITE_SPRITE_PART_SIZE 0x34

typedef struct Holder_guess
{
    char pad0[0x14];
    void *src;
    int totalSize;
    unsigned int width;
    unsigned int height;
    int mipCount;
    int bitDepth;
    char pad1[0x8];
    void *palette;
    char pad2[0x28];
    void *surface;
    char pad3[0x24];
    unsigned int palCount;
} Holder_guess;

typedef struct Sprite2D_guess
{
    char pad0[0x1c];
    int renderMethod;
    char pad1[0x48];
    int currentFrame;
} Sprite2D_guess;

typedef struct DMRGBTrack_guess
{
    char pad[0x24];
    int numCyclesToPlay;
} DMRGBTrack_guess;

typedef struct Camera_guess
{
    char pad[0x38];
    int imageBufferWidth;
} Camera_guess;

typedef struct BMInfo_guess
{
    char pad[0x10];
    int numShares;
    void *imageData;
    int imageDataSize;
    int width;
    int height;
    char pad3[0x4];
    int depth;
} BMInfo_guess;

#define BMINFO_FLAG_IN_DICTIONARY 0x01
#define BMINFO_FLAG_DEFER_FREE 0x20
#define BMINFO_FLAG_EXTERNAL_BITMAPS 0x80000000
#define BMINFO_EXPECTED_FORMAT 0x1B

#define g_render_mode_guess EQ_GLOBAL(int, 0x100FB8A8)
#define GFX_SURFACE_FLAGS EQ_GLOBAL(unsigned char, 0x100FBAF4)
#define g_color_depth_guess EQ_GLOBAL(int, 0x100FBA38)
#define GFX_PIXEL_FORMAT_ALPHA_MODE EQ_GLOBAL(int, 0x100FBA3C)
#define g_direct_draw_guess EQ_GLOBAL(void *, 0x100FBA2C)
#define g_render_device_guess EQ_GLOBAL(void *, 0x100FBA6C)
#define GFX_PART_SCALE_FUNCTION EQ_GLOBAL(PartScaleFunction_guess, 0x100FC77C)
#define GFX_DEBUG_BREAK EQ_GLOBAL(void *, 0x100760BC)
#define GFX_TEXTURE_LOG_NAME EQ_ARRAY(char, 0x10086164)
#define GFX_TEXTURE_LOG_MODE EQ_ARRAY(char, 0x10086170)
#define GFX_D3DX_FAILED_FORMAT EQ_ARRAY(char, 0x1008626C)
#define GFX_TEXTURE_SURFACE_FAILED_FORMAT EQ_ARRAY(char, 0x1008621C)
#define GFX_SURFACE_FAILED_FORMAT EQ_ARRAY(char, 0x100861C8)
#define GFX_OPEN_LOG_FILE EQ_CDECL(FILE *, 0x10068412, (const char *, const char *))
#define EQGFX_DMSPRITE_FPRINTF EQ_CDECL(int, 0x100683A5, (FILE *, const char *, ...))
#define GFX_LOG_CLOSE EQ_CDECL(int, 0x10068328, (FILE *))
#define GFX_BITMAP_UNABLE_LOAD_FORMAT EQ_ARRAY(char, 0x100861A0)
#define GFX_BITMAP_INVALID_SIZE_FORMAT EQ_ARRAY(char, 0x10086174)
#define g_auto_mipmapping_enabled_guess EQ_GLOBAL(int, 0x100FBA58)
#define g_resource_total_guess EQ_GLOBAL(int, 0x100F3E54)
#define g_resource_released_guess EQ_GLOBAL(int, 0x100F3E58)
#define GFX_BITMAP_READ_MIP_DATA EQ_CDECL(int, 0x1004CF66, (int, void **, T3DBitmapInfo_guess *, int, int))
#define GFX_BITMAP_LAST_MIP_INDEX EQ_CDECL(int, 0x1000BDDE, (T3DBitmapInfo_guess *))
#define GFX_BITMAP_CREATE_SURFACE EQ_CDECL(int, 0x1000C001, (T3DBitmapInfo_guess *, int, int))
#define GFX_BITMAP_UPLOAD_ROWS EQ_CDECL(int, 0x1000C4C2, (T3DBitmapInfo_guess *, int, int))
#define GFX_BITMAP_GENERATE_MIPMAPS EQ_CDECL(int, 0x1000C289, (T3DBitmapInfo_guess *, int))
#define GFX_BITMAP_DERIVE_SCALE_FIELDS EQ_CDECL(int, 0x1003193D, (T3DBitmapInfo_guess *))
#define GFX_BITMAP_COPY_TO_SURFACE EQ_STDCALL(int, 0x1005A268, (void *, void *, int, void *, int, int, unsigned int, unsigned int *, int))
#define BMINFO_LOAD_ERROR 0xffff3d3e
#define GFX_CREATE_TEXTURE EQ_STDCALL(int, 0x1005A080, (void *, unsigned int *, unsigned int *, unsigned int *, int *, int, void **, unsigned int *))
#define GFX_TEXTURE_ERROR_TEXT EQ_STDCALL(void, 0x1005AE37, (int, unsigned int, char *))
#define SURFACE_PLAIN_DESCRIPTION_FLAGS 0x1007
#define SURFACE_PLAIN_CAPABILITIES 0x840
#define SURFACE_PLAIN_PIXEL_FORMAT_FLAGS 0x40

#define SURFACE_DESCRIPTION_SIZE 0x7c
#define SURFACE_DESCRIPTION_FLAGS 0x21007
#define SURFACE_PIXEL_FORMAT_FLAGS 0x41
#define SURFACE_CAPABILITIES 0x401008
#define SURFACE_TEXTURE_STAGE 0x10
#define SURFACE_BLIT_FLAGS 0x1000000
#define SURFACE_RESIZE_ERROR 0xffff3d3e
#define SURFACE_COPY_ERROR 0xffff3d3e

#define COMPOSITE_SPRITE_DEFINITION_TYPE 0x11
#define SPRITE_DEFINITION_TYPE 0x12
#define BLIT_SPRITE_TINT_INDIRECT_FLAG 0x80000000
#define BLIT_SPRITE_TINT_INDIRECT_INDEX_MASK 0x7fffffff
#define BLIT_SPRITE_TINT_ALPHA_ENABLED 0x01000000
#define BLIT_SPRITE_TINT_ALPHA_PLUS_ONE 0x00100000
#define BLIT_SPRITE_TINT_ALPHA_NIBBLE_MASK 0x0f
#define BLIT_SPRITE_FLAG_HAS_TINT 0x01
#define BLIT_SPRITE_FULL_ALPHA 0xff
#define BLIT_SPRITE_DEFINITION_ENTRY_SIZE 0x18
#define BLIT_SPRITE_OBJECT_SIZE 0x70
#define BLIT_SPRITE_DEFINITION_TYPE 0x07
#define BLIT_SPRITE_OBJECT_TYPE 0x08

typedef struct T3D2DSpriteDefinition_guess {
    int type;
    unsigned char unknown04[0x10];
    void *volume;
    unsigned char unknown18[0x10];
    void *components;
} T3D2DSpriteDefinition_guess;

typedef struct T3DCompositeSpriteDefinition_guess {
    int type;
    unsigned char unknown04[0x04];
    char *name;
    unsigned int flags;
    int referenceCount;
    int width;
    int componentCount;
    void *parts;
    int definitionWord20;
    T3D2DSpriteDefinition_guess *spriteDefinition;
} T3DCompositeSpriteDefinition_guess;

typedef struct T3DCompositeSpriteCreateContext_guess {
    unsigned char unknown00[0x08];
    int contextOffset;
} T3DCompositeSpriteCreateContext_guess;

typedef struct T3DBlitSpriteReference_guess {
    unsigned char unknown00[0x10];
    int referenceCount;
} T3DBlitSpriteReference_guess;

typedef struct T3DBlitSprite_guess {
    unsigned int type;
    unsigned char unknown04[0x04];
    void *dictionaryEntry;
    unsigned char flags;
    unsigned char unknown0d[0x03];
    int destructionState;
    void *simpleSprite;
    unsigned char unknown18[0x18];
    unsigned char unknown30;
    unsigned char unknown31[0x37];
    T3DBlitSpriteReference_guess *reference68;
    T3DBlitSpriteReference_guess *reference6c;
} T3DBlitSprite_guess;
typedef struct T3DBlitSpriteDefinition_guess {
    unsigned int type;
    unsigned char unknown04[0x04];
    char *name;
    unsigned int unknown0c;
    int referenceCount;
    T3DBlitSprite_guess *sprite;
} T3DBlitSpriteDefinition_guess;

typedef struct T3DBlitSpriteInput_guess {
    unsigned char unknown00[0x14];
    unsigned int *spriteData;
} T3DBlitSpriteInput_guess;

typedef struct T3DBitmapInfo_guess {
    int format;
    unsigned char unknown04[0x04];
    void *dictionaryEntryName;
    unsigned int flags;
    int activeReferenceCount;
    void *allocation14;
    unsigned int textureBytes;
    unsigned int width;
    unsigned int height;
    unsigned int lastBitmapIndex;
    int bitDepth;
    int unknown2c;
    void **bitmapAllocations;
    void *allocation34;
    unsigned char unknown38[0x28];
    void *surfaceObject;
    unsigned char unknown64[0x10];
    void *renderObject;
    unsigned char unknown78[0x10];
    unsigned int paletteEntryCount;
} T3DBitmapInfo_guess;

typedef struct T3DCompositeSpritePart_guess {
    float rotation[9];
    float translation[3];
    float scale;
} T3DCompositeSpritePart_guess;

typedef struct T3DCompositeSpriteFrameData_guess {
    int type;
    unsigned char unknown04[0x0c];
    int referenceCount;
    unsigned int frameCount;
    unsigned int partsPerFrame;
    T3DCompositeSpritePart_guess *parts;
} T3DCompositeSpriteFrameData_guess;

typedef struct T3DCompositeSprite_guess {
    int type;
    unsigned char unknown04[0x04];
    void *entry;
    unsigned int flags;
    int state;
    void *volume;
    unsigned int currentFrame;
    int updateInterval;
    int contextOffset;
    int unknown24;
    void *components;
    float centerOffsetX;
    float centerOffsetY;
    float centerOffsetZ;
    T3DCompositeSpriteFrameData_guess *frameData;
} T3DCompositeSprite_guess;

typedef struct T3DCompositeSpriteCenterOffset_guess {
    float x;
    float y;
    float z;
} T3DCompositeSpriteCenterOffset_guess;

typedef struct T3DCompositeSpriteOwnedSprite_guess {
    int type;
} T3DCompositeSpriteOwnedSprite_guess;

typedef struct T3DCompositeSpriteDestroyChild_guess {
    T3DCompositeSpriteOwnedSprite_guess *sprite;
    unsigned char unknown04[0x9c];
} T3DCompositeSpriteDestroyChild_guess;

typedef struct T3DCompositeSpriteDestroyDefinition_guess {
    unsigned char unknown00[0x10];
    int referenceCount;
    unsigned char unknown14[0x04];
    int childCount;
} T3DCompositeSpriteDestroyDefinition_guess;

typedef struct T3DCompositeSpriteDestroy_guess {
    unsigned char unknown00[0x08];
    void *entry;
    unsigned char flags;
    unsigned char unknown0d[0x03];
    int state;
    void *volume;
    unsigned char unknown18[0x10];
    T3DCompositeSpriteDestroyChild_guess *children;
    unsigned char unknown2c[0x0c];
    T3DCompositeSpriteDestroyDefinition_guess *definition;
} T3DCompositeSpriteDestroy_guess;

typedef struct T3DTextureResizeContext_guess {
    unsigned char unknown00[0x1C];
    unsigned int surfaceWidth;
    unsigned int surfaceHeight;
    unsigned int mipMapLevelCount;
    unsigned char unknown28[0x38];
    void *surfaceObject;
} T3DTextureResizeContext_guess;

typedef struct T3DSurfaceDescription_guess {
    unsigned int size;
    unsigned int flags;
    unsigned int height;
    unsigned int width;
    unsigned char unknown10[0x08];
    unsigned int mipMapCount;
    unsigned char unknown1c[0x2C];
    unsigned int pixelFormatSize;
    unsigned int pixelFormatFlags;
    unsigned int unknown50;
    unsigned int pixelFormatBits;
    unsigned int redMask;
    unsigned int greenMask;
    unsigned int blueMask;
    unsigned int alphaMask;
    unsigned int surfaceCapabilities;
    unsigned int textureStage;
    unsigned char unknown70[0x0C];
} T3DSurfaceDescription_guess;

typedef struct T3DBlitSpriteTintContext_guess {
    unsigned char unknown00[0x3e0];
    unsigned int *blitSpriteTintTable;
} T3DBlitSpriteTintContext_guess;

typedef struct T3DBlitSpriteTintTarget_guess {
    unsigned char unknown00[0x14];
    unsigned int tintModeFlags;
    unsigned char unknown18[0x10];
    unsigned int renderFlags;
    unsigned int tintColor;
    unsigned char tintMode;
    unsigned char tintAlpha;
} T3DBlitSpriteTintTarget_guess;

// FUNCTION: EQGFX 0x1000baba
// Loads bitmap imagery and prepares it for use by the active renderer.
// __cdecl. Reads the mip chain, then prepares (textureType 0) or uploads the bitmap; returns the read result, 0 or BMINFO_LOAD_ERROR.
int __cdecl _t3dLoadBMInfo_guess(void *dictionaryContext, T3DBitmapInfo_guess *bitmapInfo, int textureType)
{
    int result;
    void ***allocationSlot;
    void **readTarget;
    FILE *logFile;
    unsigned int width;
    unsigned int height;
    unsigned int byteCount;

    if (bitmapInfo->allocation14 != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
        bitmapInfo->allocation14 = 0;
    }
    allocationSlot = &bitmapInfo->bitmapAllocations;
    readTarget = (void **)allocationSlot;
    if (bitmapInfo->lastBitmapIndex != 0)
        readTarget = *allocationSlot;
    result = GFX_BITMAP_READ_MIP_DATA(bitmapInfo->lastBitmapIndex + 1, readTarget, bitmapInfo, bitmapInfo->unknown2c, textureType);
    if (textureType == 1 && bitmapInfo->lastBitmapIndex == 0 && g_auto_mipmapping_enabled_guess == 1)
    {
        if (g_render_mode_guess == 1)
            bitmapInfo->flags |= BMINFO_FLAG_EXTERNAL_BITMAPS;
        else
        {
            bitmapInfo->lastBitmapIndex = GFX_BITMAP_LAST_MIP_INDEX(bitmapInfo);
            if (bitmapInfo->lastBitmapIndex != 0)
                bitmapInfo->flags |= BMINFO_FLAG_EXTERNAL_BITMAPS;
        }
    }
    if (result != 0)
    {
        logFile = GFX_OPEN_LOG_FILE(GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
        if (logFile == NULL)
            return result;
        EQGFX_DMSPRITE_FPRINTF(logFile, GFX_BITMAP_UNABLE_LOAD_FORMAT, *allocationSlot);
        GFX_LOG_CLOSE(logFile);
        return result;
    }
    if (textureType != 0)
    {
        width = bitmapInfo->width;
        height = bitmapInfo->height;
        if ((height < width && (height << 3) < width) || (width < height && (width << 3) < height))
        {
            logFile = GFX_OPEN_LOG_FILE(GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
            if (logFile == NULL)
                return 0;
            EQGFX_DMSPRITE_FPRINTF(logFile, GFX_BITMAP_INVALID_SIZE_FORMAT, *allocationSlot);
            GFX_LOG_CLOSE(logFile);
            return result;
        }
    }
    if (g_render_mode_guess != 1)
    {
        if (g_render_mode_guess != 2)
            return 0;
        if (textureType != 0)
        {
            GFX_BITMAP_GENERATE_MIPMAPS(bitmapInfo, (bitmapInfo->flags & BMINFO_FLAG_EXTERNAL_BITMAPS) != 0);
            GFX_BITMAP_DERIVE_SCALE_FIELDS(bitmapInfo);
            return 0;
        }
        result = GFX_BITMAP_CREATE_SURFACE(bitmapInfo, 0, 0);
        if (result != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
            bitmapInfo->allocation14 = 0;
            return BMINFO_LOAD_ERROR;
        }
        result = GFX_BITMAP_UPLOAD_ROWS(bitmapInfo, 0, 0);
        EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
        bitmapInfo->allocation14 = 0;
        if (result == 0)
            return 0;
        return BMINFO_LOAD_ERROR;
    }
    result = GFX_BITMAP_CREATE_SURFACE(bitmapInfo, textureType, GFX_BITMAP_INDEXED_MODE);
    if (result != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
        bitmapInfo->allocation14 = 0;
        return BMINFO_LOAD_ERROR;
    }
    if (textureType == 0)
    {
        result = GFX_BITMAP_UPLOAD_ROWS(bitmapInfo, 0, 0);
        EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
        bitmapInfo->allocation14 = 0;
        if (result == 0)
            return 0;
        return BMINFO_LOAD_ERROR;
    }
    GFX_BITMAP_GENERATE_MIPMAPS(bitmapInfo, 0);
    result = GFX_BITMAP_COPY_TO_SURFACE(g_render_device_guess, bitmapInfo->surfaceObject, -1, bitmapInfo->allocation14, 0, 8, bitmapInfo->width << 1, 0, 2);
    if (result != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
        bitmapInfo->allocation14 = 0;
        return BMINFO_LOAD_ERROR;
    }
    if (GFX_BITMAP_INDEXED_MODE == 1 && bitmapInfo->width > 3 && bitmapInfo->height > 3)
        byteCount = (bitmapInfo->height * bitmapInfo->width) >> 1;
    else
        byteCount = bitmapInfo->height * bitmapInfo->width * 2;
    bitmapInfo->textureBytes = byteCount;
    if ((bitmapInfo->flags & BMINFO_FLAG_EXTERNAL_BITMAPS) != 0)
        bitmapInfo->textureBytes = (byteCount << 2) / 3;
    g_resource_total_guess += bitmapInfo->textureBytes;
    g_resource_released_guess += bitmapInfo->textureBytes;
    EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
    bitmapInfo->allocation14 = 0;
    return 0;
}

// FUNCTION: EQGFX 0x1000bcf0
// Removes an unused bitmap resource and releases its image storage and rendering surface.
int t3dUndefineBMInfo(void *dictionaryContext, T3DBitmapInfo_guess **bitmapInfoHandle)
{
    T3DBitmapInfo_guess *bitmapInfo;
    unsigned int bitmapIndex;
    int result;

    bitmapInfo = *bitmapInfoHandle;
    result = (int)dictionaryContext;
    if (bitmapInfo->activeReferenceCount != 0)
    {
        result = 0xffff3dab;
    }
    else
    {
        if (bitmapInfo->renderObject != 0
            && EQ_CDECL(int, 0x10045F85, (T3DBitmapInfo_guess *))(bitmapInfo) != 0)
        {
            result = 0xffff3d3e;
        }
        else
        {
            if (bitmapInfo->surfaceObject != 0)
            {
                ((void (__stdcall *)(void *))(*(void ***)bitmapInfo->surfaceObject)[2])(bitmapInfo->surfaceObject);
                bitmapInfo->surfaceObject = 0;
            }
            if (bitmapInfo->allocation34 != 0)
                EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation34);
            if (bitmapInfo->allocation14 != 0)
                EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->allocation14);
            if ((bitmapInfo->flags & BMINFO_FLAG_IN_DICTIONARY) != 0)
                EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionaryContext, bitmapInfo->dictionaryEntryName);

            result = EQ_CDECL(int, 0x10015D81, (void *, T3DBitmapInfo_guess *))(dictionaryContext, bitmapInfo);
            if ((bitmapInfo->flags & BMINFO_FLAG_DEFER_FREE) == 0)
            {
                if (bitmapInfo->lastBitmapIndex == 0
                    || bitmapInfo->bitmapAllocations == 0
                    || (bitmapInfo->flags & BMINFO_FLAG_EXTERNAL_BITMAPS) != 0)
                {
                    if (bitmapInfo->bitmapAllocations != 0)
                        EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->bitmapAllocations);
                }
                else
                {
                    bitmapIndex = 0;
                    do
                    {
                        if (bitmapInfo->bitmapAllocations[bitmapIndex] != 0)
                            EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->bitmapAllocations[bitmapIndex]);
                        bitmapIndex++;
                    } while (bitmapIndex <= bitmapInfo->lastBitmapIndex);
                    EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->bitmapAllocations);
                }
                if (bitmapInfo->dictionaryEntryName != 0)
                    EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo->dictionaryEntryName);
                EQGFX_T3D_FREE_MEMORY_GUESS(bitmapInfo);
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000bdde
// Chooses how many mip levels a bitmap can use based on its dimensions.
// __cdecl (the callee pops 0 bytes; ECX is not an input). One stack argument, a pointer to a
// struct with two unsigned dimensions at +0x1c/+0x20; returns how many times both can be
// halved while both stay >= 8, capped at 6 (mip level count guess).
int __cdecl _t3dGetBMInfoLastMipIndex_guess(char *info)
{
    unsigned int firstDim;
    unsigned int secondDim;
    unsigned int smallDim;
    unsigned int largeDim;
    unsigned int mipLevels;

    mipLevels = 0;
    firstDim = *(unsigned int *)(info + 0x1c);
    secondDim = *(unsigned int *)(info + 0x20);
    if (firstDim > secondDim)
    {
        smallDim = secondDim;
        largeDim = firstDim;
    }
    else
    {
        smallDim = firstDim;
        largeDim = secondDim;
    }
    while (smallDim >= 8 && largeDim >= 8 && mipLevels < 6)
    {
        mipLevels++;
        smallDim >>= 1;
        largeDim >>= 1;
    }
    return (int)mipLevels;
}

// FUNCTION: EQGFX 0x1000be0f
// Keeps a shared bitmap resource alive for an additional user.
int __cdecl t3dShareBMInfo(int *bmInfoRef)
{
    T3DBitmapInfo_guess *bmInfo;

    bmInfo = (T3DBitmapInfo_guess *)*bmInfoRef;
    ++bmInfo->activeReferenceCount;
    return 0;
}

// FUNCTION: EQGFX 0x1000be1b
// Ends one user's share of a bitmap resource.
// __cdecl (the callee pops 0 bytes; ECX is not an input). One stack argument, a pointer to the
// BMInfo pointer; decrements the dword at +0x10 of the BMInfo (a share/refcount) and returns 0.
int __cdecl t3dUnshareBMInfo(int *bmInfoRef)
{
    --*(int *)(*(int *)bmInfoRef + 0x10);
    return 0;
}

// FUNCTION: EQGFX 0x1000BE31
// Reports how many mip levels are stored in a bitmap resource.
int __cdecl t3dGetBMInfoNumLevels(T3DBitmapInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->lastBitmapIndex + 1;
}

// FUNCTION: EQGFX 0x1000BE6E
// Reports the length of a bitmap's asset tag.
int __cdecl t3dGetBMInfoTagLength(T3DBitmapInfo_guess **bitmapInfoRef)
{
    char *tag;

    tag = (char *)(*bitmapInfoRef)->dictionaryEntryName;
    if (tag == 0)
        return 0;
    return EQ_CDECL(int, 0x10068160, (const char *))(tag);
}

// FUNCTION: EQGFX 0x1000BE84
// Provides the asset tag used to identify a bitmap resource.
int __cdecl t3dGetBMInfoTag(T3DBitmapInfo_guess **bitmapInfoRef, char *tagBuffer)
{
    if ((*bitmapInfoRef)->dictionaryEntryName == 0)
    {
        tagBuffer[0] = 0;
    }
    else
    {
        GFX_PATH_COPY_STRING(tagBuffer, (*bitmapInfoRef)->dictionaryEntryName);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000BEA9
// Checks whether a bitmap resource has the expected format.
int __cdecl t3dIsBMInfoValid(T3DBitmapInfo_guess **bmInfoRef)
{
    int result;

    result = 0;
    if ((*bmInfoRef)->format != BMINFO_EXPECTED_FORMAT)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x1000BEBC
// Checks whether a bitmap resource is registered in the asset dictionary.
unsigned int __cdecl t3dIsBMInfoInDictionary(T3DBitmapInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->flags & BMINFO_FLAG_IN_DICTIONARY;
}

// FUNCTION: EQGFX 0x1000be27
// __cdecl. Returns the share count at +0x10 of the BMInfo.
int __cdecl t3dGetBMInfoNumShares(BMInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->numShares;
}

// FUNCTION: EQGFX 0x1000be3c
// __cdecl. Returns the height at +0x20 of the BMInfo.
int __cdecl t3dGetBMInfoHeight(BMInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->height;
}

// FUNCTION: EQGFX 0x1000be46
// __cdecl. Returns the width at +0x1c of the BMInfo.
int __cdecl t3dGetBMInfoWidth(BMInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->width;
}

// FUNCTION: EQGFX 0x1000be50
// __cdecl. Returns the depth at +0x28 of the BMInfo.
int __cdecl t3dGetBMInfoDepth(BMInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->depth;
}

// FUNCTION: EQGFX 0x1000be5a
// __cdecl. Returns the image data size at +0x18 of the BMInfo.
int __cdecl t3dGetBMInfoImageDataSize(BMInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->imageDataSize;
}

// FUNCTION: EQGFX 0x1000be64
// __cdecl. Returns the image data pointer at +0x14 of the BMInfo.
void *__cdecl t3dGetBMInfoImageData(BMInfo_guess **bmInfoRef)
{
    return (*bmInfoRef)->imageData;
}

// FUNCTION: EQGFX 0x1000bec9
// Resolves missing asset files through an environment-specified search directory.
int __cdecl _t3dResolveBitmapSearchPaths_guess(int unused, BitmapSearchList_guess *searchList)
{
    unsigned int pathIndex;
    unsigned int pathCount;
    char *lastComponent;
    char statBuffer[36];
    char searchDirectory[264];
    char resolvedPath[264];
    void **pathEntries;
    void **currentEntry;
    void **cleanupEntry;
    unsigned int remainingPaths;
    char *path;
    int statResult;
    unsigned int allocationSize;
    void *replacement;

    pathCount = (unsigned int)searchList->additionalPathCount + 1;
    if (pathCount < 2)
        pathEntries = &searchList->pathStorage;
    else
        pathEntries = (void **)searchList->pathStorage;

    EQGFX_VISIBILITY_MEMSET_GUESS(searchDirectory, 0, GFX_PATH_BUFFER_SIZE);
    if (GFX_WLD_PATH != 0 &&
        GFX_GET_ENVIRONMENT_VARIABLE_A(GFX_WLD_PATH, searchDirectory, GFX_PATH_BUFFER_SIZE) != 0)
    {
        pathIndex = 0;
        currentEntry = pathEntries;
        if (pathCount != 0)
        {
            do
            {
                path = (char *)*currentEntry;
                statResult = GFX_PATH_STAT(path, statBuffer);
                if (statResult != 0 &&
                    (GFX_PATH_STRCHR(path, 0x5c) == 0 || GFX_PATH_STRCHR(path, 0x2f) != 0) &&
                    GFX_SEARCH_PATH_A(searchDirectory, path, 0, GFX_PATH_BUFFER_SIZE, resolvedPath, &lastComponent) != 0)
                {
                    allocationSize = strlen_EQGFX(resolvedPath) + 1;
                    replacement = GFX_COMPOSITE_ALLOCATE_MEMORY(allocationSize);
                    if (replacement == 0)
                    {
                        cleanupEntry = pathEntries;
                        remainingPaths = pathCount;
                        do
                        {
                            EQGFX_T3D_FREE_MEMORY_GUESS(*cleanupEntry);
                            *cleanupEntry = 0;
                            --remainingPaths;
                            ++cleanupEntry;
                        } while (remainingPaths != 0);
                        if (pathCount > 1)
                            EQGFX_T3D_FREE_MEMORY_GUESS(pathEntries);
                        return GFX_PATH_ALLOCATION_ERROR;
                    }
                    EQGFX_T3D_FREE_MEMORY_GUESS(*currentEntry);
                    *currentEntry = replacement;
                    GFX_PATH_COPY_STRING((char *)replacement, resolvedPath);
                }
                ++pathIndex;
                ++currentEntry;
            } while (pathIndex < pathCount);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000C001
// Allocates rendering storage for a bitmap, as either a plain image surface or a texture.
// __cdecl. Creates a BMInfo's surface (textureType 0) or texture; failures are logged to texture.txt. Arg 1 is reused for the height.
int __cdecl _t3dCreateBMInfoSurface_guess(unsigned int bitmapInfoArg, int textureType, int allowMipMaps)
{
    typedef int (__stdcall *CreateSurfaceFn_guess)(void *ddraw, T3DSurfaceDescription_guess *desc, void **surface, void *outer);
    unsigned int flags;
    unsigned int width;
    T3DSurfaceDescription_guess desc;
    char errorText[256];
    T3DBitmapInfo_guess *bitmapInfo;
    FILE *logFile;
    const char *format;
    void *failureName;
    int result;

    if (textureType == 0)
    {
        EQGFX_VISIBILITY_MEMSET_GUESS(&desc, 0, SURFACE_DESCRIPTION_SIZE);
        desc.size = SURFACE_DESCRIPTION_SIZE;
        bitmapInfo = (T3DBitmapInfo_guess *)bitmapInfoArg;
        desc.flags = SURFACE_PLAIN_DESCRIPTION_FLAGS;
        desc.surfaceCapabilities = SURFACE_PLAIN_CAPABILITIES;
        desc.width = bitmapInfo->width;
        desc.height = bitmapInfo->height;
        desc.pixelFormatSize = 0x20;
        desc.pixelFormatFlags = SURFACE_PLAIN_PIXEL_FORMAT_FLAGS;
        if (g_color_depth_guess == 0x10)
        {
            desc.pixelFormatBits = 0x10;
            if (GFX_PIXEL_FORMAT_ALPHA_MODE != 0)
            {
                desc.redMask = 0xf800;
                desc.greenMask = 0x7e0;
            }
            else
            {
                desc.redMask = 0x7c00;
                desc.greenMask = 0x3e0;
            }
            desc.blueMask = 0x1f;
        }
        else
        {
            desc.pixelFormatBits = 0x20;
            desc.redMask = 0xff0000;
            desc.greenMask = 0xff00;
            desc.blueMask = 0xff;
        }
        result = ((CreateSurfaceFn_guess)(*(void ***)g_direct_draw_guess)[6])(g_direct_draw_guess, &desc, &bitmapInfo->surfaceObject, 0);
        if (result >= 0)
            return 0;
        logFile = GFX_OPEN_LOG_FILE(GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
        if (logFile == 0)
            return 0xffff3d3e;
        failureName = bitmapInfo->bitmapAllocations;
        format = GFX_SURFACE_FAILED_FORMAT;
    }
    else
    {
        if (g_render_mode_guess == 1)
        {
            bitmapInfo = (T3DBitmapInfo_guess *)bitmapInfoArg;
            textureType = 8;
            width = bitmapInfo->width;
            flags = ~bitmapInfo->flags >> 0x17 & 0x100;
            if (allowMipMaps != 0 && width > 3 && bitmapInfo->height > 3)
                textureType = 0x12;
            bitmapInfoArg = bitmapInfo->height;
            result = GFX_CREATE_TEXTURE(g_render_device_guess, &flags, &width, &bitmapInfoArg, &textureType, 0, &bitmapInfo->surfaceObject, &bitmapInfo->lastBitmapIndex);
            if (result >= 0)
                return 0;
            GFX_TEXTURE_ERROR_TEXT(result, sizeof(errorText), errorText);
            logFile = GFX_OPEN_LOG_FILE(GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
            if (logFile == 0)
                return 0xffff3d3e;
            EQGFX_DMSPRITE_FPRINTF(logFile, GFX_D3DX_FAILED_FORMAT, bitmapInfo->bitmapAllocations, bitmapInfo->width, bitmapInfo->height, errorText);
            GFX_LOG_CLOSE(logFile);
            return 0xffff3d3e;
        }
        EQGFX_VISIBILITY_MEMSET_GUESS(&desc, 0, SURFACE_DESCRIPTION_SIZE);
        desc.size = SURFACE_DESCRIPTION_SIZE;
        desc.flags = SURFACE_DESCRIPTION_FLAGS;
        desc.surfaceCapabilities = SURFACE_CAPABILITIES;
        desc.textureStage = SURFACE_TEXTURE_STAGE;
        desc.pixelFormatBits = 0x10;
        bitmapInfo = (T3DBitmapInfo_guess *)bitmapInfoArg;
        desc.width = bitmapInfo->width;
        desc.mipMapCount = bitmapInfo->lastBitmapIndex + 1;
        desc.height = bitmapInfo->height;
        desc.pixelFormatSize = 0x20;
        desc.pixelFormatFlags = SURFACE_PIXEL_FORMAT_FLAGS;
        desc.redMask = 0x7c00;
        desc.greenMask = 0x3e0;
        desc.blueMask = 0x1f;
        desc.alphaMask = 0x8000;
        result = ((CreateSurfaceFn_guess)(*(void ***)g_direct_draw_guess)[6])(g_direct_draw_guess, &desc, &bitmapInfo->surfaceObject, 0);
        if (result >= 0)
            return 0;
        logFile = GFX_OPEN_LOG_FILE(GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
        if (logFile == 0)
            return 0xffff3d3e;
        failureName = bitmapInfo->bitmapAllocations;
        format = GFX_TEXTURE_SURFACE_FAILED_FORMAT;
    }
    EQGFX_DMSPRITE_FPRINTF(logFile, format, failureName);
    GFX_LOG_CLOSE(logFile);
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x1000c289
int __cdecl _t3dConvertBMInfo8To16Bit_guess(unsigned int bitmapInfoAddress, unsigned int generateMipmaps)
{
    T3DBitmapInfo_guess *bitmapInfo;
    unsigned char *sourcePixels;
    unsigned char *sourcePixel;
    unsigned char *colorTable;
    unsigned char *colorEntry;
    unsigned char colorIndex;
    unsigned short *allocatedPixels;
    unsigned short *destinationPixel;
    unsigned short *sourceMipPixels;
    unsigned short *nextMipPixels;
    unsigned short *destinationMipPixels;
    unsigned short redComponent;
    unsigned int allocationSize;
    unsigned int bitmapWidth;
    unsigned int bitmapHeight;
    unsigned int mipWidth;
    unsigned int column;
    unsigned int row;
    unsigned int mipColumn;
    unsigned int mipRow;
    int mipLevel;
    int opaquePixelCount;
    int pixelCount;
    int result;

    bitmapInfo = (T3DBitmapInfo_guess *)bitmapInfoAddress;
    result = 0;
    sourcePixels = (unsigned char *)bitmapInfo->allocation14;
    if (bitmapInfo->bitDepth == 8)
    {
        bitmapWidth = bitmapInfo->width;
        bitmapHeight = bitmapInfo->height;
        allocationSize = bitmapWidth * bitmapHeight * 2;
        if (generateMipmaps != 0)
        {
            for (mipLevel = bitmapInfo->lastBitmapIndex; mipLevel != 0; --mipLevel)
            {
                bitmapWidth >>= 1;
                bitmapHeight >>= 1;
                if (bitmapWidth == 0)
                    bitmapWidth = 1;
                if (bitmapHeight == 0)
                    bitmapHeight = 1;
                allocationSize += bitmapWidth * bitmapHeight * 2;
            }
        }
        allocatedPixels = (unsigned short *)GFX_COMPOSITE_ALLOCATE_MEMORY(allocationSize);
        if (allocatedPixels == 0)
        {
            result = 0xffff3d3e;
        }
        else
        {
            opaquePixelCount = 0;
            row = 0;
            sourcePixel = sourcePixels;
            destinationPixel = allocatedPixels;
            if (bitmapInfo->height != 0)
            {
                do
                {
                    bitmapInfoAddress = 0;
                    if (bitmapInfo->width != 0)
                    {
                        do
                        {
                            colorIndex = *sourcePixel;
                            if (colorIndex < bitmapInfo->paletteEntryCount
                                && (colorTable = (unsigned char *)bitmapInfo->allocation34) != 0)
                            {
                                if (colorIndex == 0)
                                {
                                    ++opaquePixelCount;
                                    colorEntry = colorTable;
                                    if (colorTable[0] < 0xe1 || colorTable[1] > 0xf
                                        || colorTable[2] < 0xe1)
                                    {
                                        redComponent = (unsigned short)(colorTable[0] & 0xf8);
                                        goto encode_pixel;
                                    }
                                }
                                else
                                {
                                    colorEntry = colorTable + (unsigned int)colorIndex * 4;
                                    if (colorEntry[0] < 0xe1 || colorEntry[1] > 0xf
                                        || colorEntry[2] < 0xe1)
                                    {
                                        redComponent = (unsigned short)(0xff00 | (colorEntry[0] & 0xf8));
                                        goto encode_pixel;
                                    }
                                }
                                *destinationPixel = 0;
                            }
                            else
                            {
                                *destinationPixel = 0x8000;
                            }
                            goto next_pixel;

encode_pixel:
                            *destinationPixel = (unsigned short)((((unsigned int)redComponent << 5)
                                | (colorEntry[1] & 0xf8)) << 2 | (colorEntry[2] >> 3));

next_pixel:
                            ++sourcePixel;
                            ++destinationPixel;
                            ++bitmapInfoAddress;
                        } while (bitmapInfoAddress < bitmapInfo->width);
                    }
                    ++row;
                } while (row < bitmapInfo->height);
            }
            if (GFX_BITMAP_INDEXED_MODE == 1)
            {
                bitmapWidth = bitmapInfo->width;
                colorTable = (unsigned char *)bitmapInfo->allocation34;
                pixelCount = bitmapWidth * bitmapInfo->height;
                if (colorTable != 0)
                {
                    if (colorTable[0] > 0xe0 && colorTable[1] < 0x10 && colorTable[2] > 0xe0)
                        pixelCount = 0;
                    if (colorTable[0] < 8 && colorTable[1] < 8 && colorTable[2] < 8)
                        pixelCount = 0;
                }
                if (pixelCount > 0x80 && opaquePixelCount < pixelCount / 0x14
                    && (mipRow = 0, destinationPixel = allocatedPixels, bitmapInfo->height != 0))
                {
                    do
                    {
                        mipColumn = 0;
                        if (bitmapWidth != 0)
                        {
                            do
                            {
                                ((unsigned char *)destinationPixel)[1] |= 0x80;
                                bitmapWidth = bitmapInfo->width;
                                ++destinationPixel;
                                ++mipColumn;
                            } while (mipColumn < bitmapWidth);
                        }
                        ++mipRow;
                    } while (mipRow < bitmapInfo->height);
                }
            }
            if (generateMipmaps != 0)
            {
                generateMipmaps = bitmapInfo->height;
                mipWidth = bitmapInfo->width;
                nextMipPixels = allocatedPixels + generateMipmaps * mipWidth;
                sourceMipPixels = allocatedPixels;
                for (mipLevel = bitmapInfo->lastBitmapIndex,
                    destinationMipPixels = nextMipPixels;
                    mipLevel != 0;
                    --mipLevel)
                {
                    result = GFX_BITMAP_BUILD_MIP_LEVEL(sourceMipPixels, mipWidth * 2,
                        mipWidth, generateMipmaps, destinationMipPixels, mipWidth);
                    if (result != 0)
                        return 0xffff3dad;
                    generateMipmaps >>= 1;
                    mipWidth >>= 1;
                    if (mipWidth == 0)
                        mipWidth = 1;
                    if (generateMipmaps == 0)
                        generateMipmaps = 1;
                    nextMipPixels = destinationMipPixels + generateMipmaps * mipWidth;
                    sourceMipPixels = destinationMipPixels;
                    destinationMipPixels = nextMipPixels;
                }
            }
            EQGFX_T3D_FREE_MEMORY_GUESS(sourcePixels);
            bitmapInfo->allocation14 = allocatedPixels;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000c4c2
// __cdecl. Uploads texture rows with palette and flip handling, then builds mipmaps.
int __cdecl _t3dCopyBMInfoToSurface_guess(Holder_guess *holder, int topDown, int genMips)
{
    typedef int (__stdcall *LockFn_guess)(void *surface, int rect, unsigned int *desc, int flags, int event);
    typedef int (__stdcall *RestoreFn_guess)(void *surface);
    typedef int (__stdcall *UnlockFn_guess)(void *surface, int flags);
    typedef int (__stdcall *SimpleFn_guess)(void *surface);
    typedef int (__stdcall *SurfaceGetAttachedSurfaceFn)(void *surface, unsigned int *query, void **outSurf);
    unsigned int mipDesc[31];
    unsigned int queryDesc[4];
    unsigned int surfDesc[31];
    void *mipSurf;
    void *curSurf;
    int result;
    int status;
    unsigned int row;
    unsigned int col;
    unsigned int wide;
    unsigned int tall;
    int pitch;
    unsigned char *srcByte;
    unsigned short *srcWord;
    unsigned short *dstWord;
    unsigned int *dstDword;
    unsigned char *entry;
    unsigned int red;
    unsigned int green;
    unsigned int mixed;
    unsigned int blue;
    unsigned int srcVal;
    unsigned int idx;
    int mipsLeft;
    int copy;

    memset(queryDesc, 0, 0x10);
    memset(mipDesc, 0, 0x7c);
    mipDesc[0] = 0x7c;
    memset(surfDesc, 0, 0x7c);
    surfDesc[0] = 0x7c;
    result = 0;
    status = ((LockFn_guess)(*(void ***)holder->surface)[0x19])(holder->surface, 0, surfDesc, 1, 0);
    if (status < 0)
    {
        if (status != (int)0x887601c2)
        {
            result = (int)0xffff3dad;
            return result;
        }
        status = ((RestoreFn_guess)(*(void ***)holder->surface)[0x1b])(holder->surface);
        if (status < 0)
        {
            result = (int)0xffff3dad;
            return result;
        }
        status = ((LockFn_guess)(*(void ***)holder->surface)[0x19])(holder->surface, 0, surfDesc, 1, 0);
        if (status < 0)
        {
            result = (int)0xffff3dad;
            return result;
        }
    }
    wide = holder->width;
    tall = holder->height;
    holder->totalSize = (int)(wide * tall * 2);
    if (topDown != 0)
    {
        if (holder->bitDepth == 8)
        {
            dstWord = (unsigned short *)surfDesc[9];
            srcByte = (unsigned char *)holder->src;
            pitch = (int)surfDesc[4];
            row = 0;
            if (tall != 0)
            {
                do
                {
                    col = 0;
                    if (wide != 0)
                    {
                        do
                        {
                            idx = (unsigned int)*srcByte;
                            if (idx < holder->palCount && holder->palette != 0)
                            {
                                entry = (unsigned char *)holder->palette + idx * 4;
                                if (idx == 0)
                                {
                                    if (entry[0] <= 0xe0 || entry[1] >= 0x10 || entry[2] <= 0xe0)
                                    {
                                        red = (unsigned int)(entry[0] & 0xf8);
                                        green = (unsigned int)(entry[1] & 0xf8);
                                        mixed = (red << 5) | green;
                                        blue = (unsigned int)(entry[2] >> 3);
                                        *dstWord = (unsigned short)((mixed << 2) | blue);
                                    }
                                    else
                                    {
                                        *dstWord = 0;
                                    }
                                }
                                else
                                {
                                    if (entry[0] <= 0xe0 || entry[1] >= 0x10 || entry[2] <= 0xe0)
                                    {
                                        red = ((unsigned int)(entry[0] & 0xf8)) | 0xff00;
                                        green = (unsigned int)(entry[1] & 0xf8);
                                        mixed = (red << 5) | green;
                                        blue = (unsigned int)(entry[2] >> 3);
                                        *dstWord = (unsigned short)((mixed << 2) | blue);
                                    }
                                    else
                                    {
                                        *dstWord = 0;
                                    }
                                }
                            }
                            else
                            {
                                *dstWord = 0x8000;
                            }
                            srcByte++;
                            dstWord++;
                            col++;
                        } while (col < wide);
                    }
                    dstWord += ((pitch >> 1) - (int)col);
                    row++;
                } while (row < tall);
            }
        }
        else if (holder->bitDepth == 0x10)
        {
            srcWord = (unsigned short *)holder->src;
            dstWord = (unsigned short *)surfDesc[9];
            pitch = (int)surfDesc[4];
            row = 0;
            if (tall != 0)
            {
                do
                {
                    col = 0;
                    if (wide != 0)
                    {
                        do
                        {
                            *dstWord = (unsigned short)(*srcWord | 0x8000);
                            srcWord++;
                            dstWord++;
                            col++;
                        } while (col < wide);
                    }
                    dstWord += ((pitch >> 1) - (int)col);
                    row++;
                } while (row < tall);
            }
        }
    }
    else
    {
        if (holder->bitDepth == 8)
        {
            pitch = (int)surfDesc[4];
            dstWord = (unsigned short *)surfDesc[9] + (tall - 1) * (pitch >> 1);
            srcByte = (unsigned char *)holder->src;
            row = 0;
            if (tall != 0)
            {
                do
                {
                    col = 0;
                    if (wide != 0)
                    {
                        do
                        {
                            idx = (unsigned int)*srcByte;
                            entry = (unsigned char *)holder->palette + idx * 4;
                            if (GFX_PIXEL_FORMAT_ALPHA_MODE == 0)
                            {
                                red = (unsigned int)(entry[0] & 0xf8);
                                green = (unsigned int)(entry[1] & 0xf8);
                                mixed = (red << 5) | green;
                                blue = (unsigned int)(entry[2] >> 3);
                                *dstWord = (unsigned short)((mixed << 2) | blue);
                            }
                            else
                            {
                                red = (unsigned int)(entry[0] & 0xf8);
                                green = (unsigned int)(entry[1] & 0xfc);
                                mixed = (red << 5) | green;
                                blue = (unsigned int)(entry[2] >> 3);
                                *dstWord = (unsigned short)((mixed << 3) | blue);
                            }
                            srcByte++;
                            dstWord++;
                            col++;
                        } while (col < wide);
                    }
                    dstWord -= pitch;
                    row++;
                } while (row < tall);
            }
            ((UnlockFn_guess)(*(void ***)holder->surface)[0x20])(holder->surface, 0);
            return result;
        }
        else if (holder->bitDepth == 0x10)
        {
            if (surfDesc[21] == 0x20)
            {
                holder->totalSize = (int)(wide * tall * 4);
                pitch = (int)surfDesc[4];
                dstDword = (unsigned int *)surfDesc[9] + (tall - 1) * (pitch >> 2);
                srcWord = (unsigned short *)holder->src;
                row = 0;
                if (tall != 0)
                {
                    do
                    {
                        col = 0;
                        if (wide != 0)
                        {
                            do
                            {
                                srcVal = (unsigned int)*srcWord;
                                mixed = ((srcVal & 0x7c00) << 3) | (srcVal & 0x3e0);
                                mixed = (mixed << 2) | (srcVal & 0x7000);
                                mixed = (mixed << 1) | (srcVal & 0x1f);
                                mixed = (mixed << 2) | (srcVal & 0x380);
                                *dstDword = (mixed << 1) | ((srcVal >> 2) & 7);
                                srcWord++;
                                dstDword++;
                                col++;
                            } while (col < wide);
                        }
                        dstDword -= (pitch >> 1);
                        row++;
                    } while (row < tall);
                }
                ((UnlockFn_guess)(*(void ***)holder->surface)[0x20])(holder->surface, 0);
                return result;
            }
            else
            {
                pitch = (int)surfDesc[4];
                dstWord = (unsigned short *)surfDesc[9] + (tall - 1) * (pitch >> 1);
                srcWord = (unsigned short *)holder->src;
                row = 0;
                if (tall != 0)
                {
                    do
                    {
                        col = 0;
                        if (wide != 0)
                        {
                            do
                            {
                                if (GFX_PIXEL_FORMAT_ALPHA_MODE == 0)
                                {
                                    *dstWord = (unsigned short)(*srcWord & 0x7fff);
                                }
                                else
                                {
                                    srcVal = (unsigned int)*srcWord;
                                    *dstWord = (unsigned short)(((srcVal & 0xffe0) << 1) | (srcVal & 0x1f));
                                }
                                srcWord++;
                                dstWord++;
                                col++;
                            } while (col < wide);
                        }
                        dstWord -= pitch;
                        row++;
                    } while (row < tall);
                }
                ((UnlockFn_guess)(*(void ***)holder->surface)[0x20])(holder->surface, 0);
                return result;
            }
        }
        else
        {
            ((UnlockFn_guess)(*(void ***)holder->surface)[0x20])(holder->surface, 0);
            return result;
        }
    }
    if (topDown == 0 || genMips == 0 || holder->mipCount == 0)
    {
        ((UnlockFn_guess)(*(void ***)holder->surface)[0x20])(holder->surface, 0);
        return result;
    }
    curSurf = holder->surface;
    mipsLeft = holder->mipCount;
    status = ((SimpleFn_guess)(*(void ***)curSurf)[1])(curSurf);
    if (status < 0)
    {
        return result;
    }
    while (mipsLeft != 0)
    {
        queryDesc[0] = 0x401000;
        status = ((SurfaceGetAttachedSurfaceFn)(*(void ***)curSurf)[0xc])(curSurf, queryDesc, &mipSurf);
        if (status < 0)
        {
            return result;
        }
        status = ((LockFn_guess)(*(void ***)mipSurf)[0x19])(mipSurf, 0, mipDesc, 1, 0);
        if (status < 0)
        {
            result = (int)0xffff3dad;
            return result;
        }
        status = EQ_CDECL(int, 0x1000CA0F, (unsigned short *, int, unsigned int, unsigned int, unsigned short *, int))((unsigned short *)surfDesc[9], (int)surfDesc[4], surfDesc[3], surfDesc[2], (unsigned short *)mipDesc[9], (int)mipDesc[4]);
        if (status != 0)
        {
            result = (int)0xffff3dad;
            return result;
        }
        holder->totalSize += (int)(mipDesc[2] * mipDesc[3] * 2);
        mipsLeft--;
        status = ((UnlockFn_guess)(*(void ***)curSurf)[0x20])(curSurf, 0);
        if (status < 0)
        {
            result = (int)0xffff3dad;
            return result;
        }
        status = ((SimpleFn_guess)(*(void ***)curSurf)[2])(curSurf);
        if (status < 0)
        {
            result = (int)0xffff3dad;
            return result;
        }
        for (copy = 0; copy < 31; copy++)
        {
            surfDesc[copy] = mipDesc[copy];
        }
        curSurf = mipSurf;
    }
    status = ((UnlockFn_guess)(*(void ***)curSurf)[0x20])(curSurf, 0);
    if (status < 0)
    {
        result = (int)0xffff3dad;
        return result;
    }
    status = ((SimpleFn_guess)(*(void ***)curSurf)[2])(curSurf);
    if (status < 0)
    {
        result = (int)0xffff3dad;
        return result;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000ca0f
// __cdecl. 2x2 box downsample of 15-bit pixels, skipping magenta.
int __cdecl _t3dDownsampleRGB555_guess(unsigned short *src, int srcStride, unsigned int srcWidth, unsigned int srcHeight, unsigned short *dst, int dstStride)
{
    unsigned int outRows;
    unsigned int outCols;
    unsigned int colsLeft;
    unsigned short *row0;
    unsigned short *row1;
    unsigned short *out;
    unsigned short *srcBase;
    unsigned short *dstRow;
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int opaque;
    unsigned short pix;
    int pair;

    outRows = srcHeight >> 1;
    if (outRows == 0)
        return 0;
    outCols = srcWidth >> 1;
    srcBase = src;
    dstRow = dst;
    do
    {
        row0 = srcBase;
        row1 = (unsigned short *)((char *)srcBase + srcStride);
        out = dstRow;
        if (outCols != 0)
        {
            colsLeft = outCols;
            do
            {
                red = 0;
                green = 0;
                blue = 0;
                opaque = 0;
                pair = 2;
                do
                {
                    pix = *row0;
                    row0++;
                    if (pix != 0x7c1f && pix != 0xfc1f)
                    {
                        red += (pix >> 10) & 0x1f;
                        green += (pix >> 5) & 0x1f;
                        blue += pix & 0x1f;
                    }
                    if ((pix & 0x8000) == 0)
                        opaque++;
                    pix = *row1;
                    row1++;
                    if (pix != 0x7c1f && pix != 0xfc1f)
                    {
                        red += (pix >> 10) & 0x1f;
                        green += (pix >> 5) & 0x1f;
                        blue += pix & 0x1f;
                    }
                    if ((pix & 0x8000) == 0)
                        opaque++;
                } while (--pair != 0);
                red >>= 2;
                red &= 0x1f;
                green >>= 2;
                red <<= 5;
                green &= 0x1f;
                red |= green;
                blue >>= 2;
                red <<= 5;
                blue &= 0x1f;
                red |= blue;
                *out = (unsigned short)red;
                if (opaque < 3)
                    *out = (unsigned short)(red | 0x8000);
                out++;
            } while (--colsLeft != 0);
        }
        srcBase = (unsigned short *)((char *)srcBase + srcStride * 2);
        dstRow = (unsigned short *)((char *)dstRow + dstStride);
    } while (--outRows != 0);
    return 0;
}

// FUNCTION: EQGFX 0x1000cb51
// __cdecl. Copies a locked 16-bit surface into a tight buffer, then unlocks and releases it.
unsigned int __cdecl _t3dReadBackBMInfoSurface_guess(Holder_guess *holder)
{
    typedef unsigned int (__stdcall *ReleaseSurfaceFn_guess)(void *);
    T3DSurfaceDescription_guess description;
    unsigned int result;
    unsigned short *pixels;
    unsigned short *dst;
    unsigned short *nextDst;
    unsigned short *src;
    unsigned int row;
    unsigned int col;
    unsigned short pixel;

    result = 0;
    pixels = 0;
    EQGFX_VISIBILITY_MEMSET_GUESS(&description, 0, SURFACE_DESCRIPTION_SIZE);
    if (EQ_CDECL(int, 0x1000CD5B, (Holder_guess *, T3DSurfaceDescription_guess *))(holder, &description) == 0)
    {
        if (holder->bitDepth == 0x10)
        {
            pixels = (unsigned short *)GFX_COMPOSITE_ALLOCATE_MEMORY(holder->width * holder->height * 2);
            if (pixels == 0)
                goto copy_failed;
            src = *(unsigned short **)(description.unknown1c + 0x08);
            row = 0;
            dst = pixels;
            if (holder->height != 0)
            {
                do
                {
                    col = 0;
                    if (holder->width != 0)
                    {
                        do
                        {
                            pixel = *src;
                            nextDst = dst + 1;
                            src = src + 1;
                            col = col + 1;
                            *dst = pixel;
                            dst = nextDst;
                        } while (col < holder->width);
                    }
                    row = row + 1;
                    src = src + ((*(int *)description.unknown10 >> 1) - col);
                } while (row < holder->height);
            }
        }
        EQ_CDECL(int, 0x1000CDB3, (void *))(holder);
        ((ReleaseSurfaceFn_guess)(*(void ***)holder->surface)[2])(holder->surface);
        holder->surface = 0;
        holder->src = pixels;
    }
    else
    {
copy_failed:
        result = SURFACE_COPY_ERROR;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000cc0d
// Resizes rectangular texture imagery to a square when the renderer requires it.
int __cdecl _t3dMakeTextureSurfaceSquare_guess(T3DTextureResizeContext_guess *texture)
{
    typedef int (__stdcall *CreateSurfaceFn_guess)(void *, T3DSurfaceDescription_guess *, void **, void *);
    typedef int (__stdcall *BlitSurfaceFn_guess)(void *, void *, void *, void *, unsigned int, void *);
    typedef unsigned int (__stdcall *ReleaseSurfaceFn_guess)(void *);
    T3DTextureResizeContext_guess *context;
    unsigned int adjustedWidth;
    unsigned int adjustedHeight;
    T3DSurfaceDescription_guess description;
    void *directDraw;

    context = texture;
    if (g_render_mode_guess == 1)
    {
        adjustedWidth = context->surfaceWidth;
        adjustedHeight = context->surfaceHeight;
        if ((GFX_SURFACE_FLAGS & 0x20) != 0)
        {
            if (adjustedWidth < adjustedHeight)
            {
                adjustedWidth = adjustedHeight;
            }
            else
                adjustedHeight = adjustedWidth;
        }
        if (adjustedWidth != context->surfaceWidth || adjustedHeight != context->surfaceHeight)
        {
            EQGFX_VISIBILITY_MEMSET_GUESS(
                &description, 0, SURFACE_DESCRIPTION_SIZE);
            description.size = SURFACE_DESCRIPTION_SIZE;
            description.flags = SURFACE_DESCRIPTION_FLAGS;
            description.surfaceCapabilities = SURFACE_CAPABILITIES;
            description.height = adjustedHeight;
            description.width = adjustedWidth;
            description.textureStage = SURFACE_TEXTURE_STAGE;
            description.pixelFormatSize = 0x20;
            description.pixelFormatFlags = SURFACE_PIXEL_FORMAT_FLAGS;
            if (g_color_depth_guess == 0x10)
            {
                description.pixelFormatBits = 0x10;
                if (GFX_PIXEL_FORMAT_ALPHA_MODE == 0)
                {
                    description.redMask = 0x7c00;
                    description.greenMask = 0x3e0;
                    description.blueMask = 0x1f;
                    description.alphaMask = 0x8000;
                }
                else
                {
                    description.redMask = 0xf800;
                    description.greenMask = 0x7e0;
                    description.blueMask = 0x1f;
                }
            }
            else
            {
                description.pixelFormatBits = 0x20;
                description.redMask = 0xff0000;
                description.greenMask = 0xff00;
                description.blueMask = 0xff;
            }
            description.mipMapCount = context->mipMapLevelCount + 1;
            directDraw = g_direct_draw_guess;
            if (((CreateSurfaceFn_guess)(*(void ***)directDraw)[6])(
                    directDraw, &description, (void **)&texture, 0) >= 0)
            {
                if (((BlitSurfaceFn_guess)(*(void ***)texture)[5])(
                        texture, 0, context->surfaceObject, 0, SURFACE_BLIT_FLAGS, 0) >= 0)
                {
                    ((ReleaseSurfaceFn_guess)(*(void ***)context->surfaceObject)[2])(
                        context->surfaceObject);
                    context->surfaceWidth = adjustedWidth;
                    context->surfaceObject = texture;
                    context->surfaceHeight = adjustedHeight;
                    return 0;
                }
                ((ReleaseSurfaceFn_guess)(*(void ***)context->surfaceObject)[2])(
                    context->surfaceObject);
                context->surfaceObject = 0;
            }
            return SURFACE_RESIZE_ERROR;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000cd5b
// __cdecl. Locks the surface at +0x60, restoring it when lost.
int __cdecl lockTextureMemory_guess(Holder_guess *holder, unsigned int *desc)
{
    typedef int (__stdcall *LockFn_guess)(void *surface, int rect, unsigned int *surfaceDesc, int flags, int event);
    typedef int (__stdcall *RestoreFn_guess)(void *surface);
    int status;

    *desc = 0x7c;
    status = ((LockFn_guess)(*(void ***)holder->surface)[0x19])(holder->surface, 0, desc, 1, 0);
    if (status < 0)
    {
        if (status != (int)0x887601c2)
            return (int)0xffff3d3e;
        status = ((RestoreFn_guess)(*(void ***)holder->surface)[0x1b])(holder->surface);
        if (status < 0)
            return (int)0xffff3d3e;
        status = ((LockFn_guess)(*(void ***)holder->surface)[0x19])(holder->surface, 0, desc, 1, 0);
        if (status < 0)
            return (int)0xffff3d3e;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000cdb3
// __cdecl. Unlocks the surface at +0x60 through its vtable slot 0x20.
int __cdecl unlockTextureMemory_guess(void *holder)
{
    typedef void (__stdcall *UnlockFn_guess)(void *, int);

    ((UnlockFn_guess)(*(void ***) *(void **)((char *)holder + 0x60))[0x20])(*(void **)((char *)holder + 0x60), 0);
    return 0;
}

// FUNCTION: EQGFX 0x1000cdc8
// Registers a named blit sprite template for reuse, keeping an existing matching definition.
T3DBlitSpriteDefinition_guess *__cdecl t3dDefineBlitSprite(void *context, char *name, void *simpleSprite, T3DBlitSpriteReference_guess *reference)
{
    T3DBlitSpriteDefinition_guess *definitionEntry;
    T3DBlitSprite_guess *sprite;
    char *nameBuffer;
    size_t nameLength;
    int status;

    definitionEntry = (T3DBlitSpriteDefinition_guess *)parseLookup(context, name);
    if (definitionEntry == 0)
    {
        definitionEntry = (T3DBlitSpriteDefinition_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(BLIT_SPRITE_DEFINITION_ENTRY_SIZE);
        if (definitionEntry == 0)
            return 0;

        definitionEntry->unknown0c = 0;
        definitionEntry->type = BLIT_SPRITE_DEFINITION_TYPE;
        definitionEntry->referenceCount = 1;
        nameLength = strlen_EQGFX(name);
        nameBuffer = (char *)GFX_COMPOSITE_ALLOCATE_MEMORY(nameLength + 1);
        definitionEntry->name = nameBuffer;
        if (nameBuffer == 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry);
        }
        else
        {
            GFX_PATH_COPY_STRING(nameBuffer, name);
            status = GFX_COMPOSITE_INITIALIZE_OBJECT(context, definitionEntry);
            if (status == 0)
            {
                status = parseAddToDictionary(context, definitionEntry);
                if (status < 0)
                {
                    GFX_COMPOSITE_RELEASE_OBJECT(context, definitionEntry);
                    EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry->name);
                    EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry);
                }
                else
                {
                    sprite = (T3DBlitSprite_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(BLIT_SPRITE_OBJECT_SIZE);
                    if (sprite == 0)
                    {
                        GFX_COMPOSITE_REMOVE_ENTRY(context, name);
                        GFX_COMPOSITE_RELEASE_OBJECT(context, definitionEntry);
                        EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry->name);
                        EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry);
                    }
                    else
                    {
                        definitionEntry->sprite = sprite;
                        sprite->dictionaryEntry = 0;
                        *(unsigned int *)&sprite->flags = 0;
                        sprite->unknown30 = 0;
                        sprite->simpleSprite = simpleSprite;
                        sprite->type = BLIT_SPRITE_OBJECT_TYPE;
                        sprite->destructionState = 1;
                        sprite->reference68 = reference;
                        sprite->reference6c = (T3DBlitSpriteReference_guess *)definitionEntry;
                        status = GFX_COMPOSITE_INITIALIZE_OBJECT(context, sprite);
                        if (status == 0)
                        {
                            reference->referenceCount = reference->referenceCount + 1;
                            return definitionEntry;
                        }

                        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
                        GFX_COMPOSITE_REMOVE_ENTRY(context, name);
                        GFX_COMPOSITE_RELEASE_OBJECT(context, definitionEntry);
                        EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry->name);
                        EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry);
                    }
                }
            }
            else
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry->name);
                EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry);
            }
        }
    }
    else if (definitionEntry->type == BLIT_SPRITE_DEFINITION_TYPE)
    {
        return definitionEntry;
    }

    return 0;
}

// FUNCTION: EQGFX 0x1000cf2a
// Removes a blit sprite definition and releases its sprite resources.
int __cdecl t3dUndefineBlitSprite(void *context, T3DBlitSpriteDefinition_guess *definitionEntry)
{
    T3DBlitSprite_guess *sprite;
    int status;

    status = 0;
    sprite = definitionEntry->sprite;
    if (definitionEntry->referenceCount == 1 && sprite->destructionState == 1)
    {
        sprite->reference68->referenceCount = sprite->reference68->referenceCount - 1;
        if ((sprite->flags & BMINFO_FLAG_IN_DICTIONARY) != 0)
            GFX_COMPOSITE_REMOVE_ENTRY(context, (char *)sprite->dictionaryEntry);
        GFX_COMPOSITE_RELEASE_OBJECT(context, sprite);
        if (sprite->dictionaryEntry != 0)
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->dictionaryEntry);
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
        if ((definitionEntry->unknown0c & BMINFO_FLAG_IN_DICTIONARY) != 0)
            GFX_COMPOSITE_REMOVE_ENTRY(context, definitionEntry->name);
        GFX_COMPOSITE_RELEASE_OBJECT(context, definitionEntry);
        if (definitionEntry->name != 0)
            EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry->name);
        EQGFX_T3D_FREE_MEMORY_GUESS(definitionEntry);
    }
    else
    {
        status = COMPOSITE_SPRITE_UNDEFINE_ERROR;
    }
    return status;
}

// FUNCTION: EQGFX 0x1000cfbf
// Creates a blit sprite instance from a sprite definition.
T3DBlitSprite_guess *__cdecl t3dCreateBlitSprite(void *context, T3DBlitSpriteInput_guess *definition)
{
    T3DBlitSprite_guess *sprite;
    int result;

    sprite = (T3DBlitSprite_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(0x70);
    if (sprite != 0)
    {
        *sprite = *(T3DBlitSprite_guess *)definition->spriteData;
        sprite->dictionaryEntry = 0;
        *(unsigned int *)&sprite->flags = 0;
        sprite->destructionState = 0;
        result = EQ_CDECL(int, 0x1001564B, (void *, T3DBlitSprite_guess *))(context, sprite);
        if (result != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
            sprite = 0;
        }
        else
        {
            sprite->reference6c->referenceCount++;
            sprite->reference68->referenceCount++;
        }
    }
    return sprite;
}

// FUNCTION: EQGFX 0x1000d016
// Creates a second blit sprite instance that shares the source's backing resources.
// __cdecl. Copies source into a new 0x70-byte sprite, clears its header fields and takes a reference on both of its references.
T3DBlitSprite_guess *__cdecl t3dDuplicateBlitSprite(void *context, T3DBlitSprite_guess *source)
{
    T3DBlitSprite_guess *duplicate;
    T3DBlitSpriteReference_guess *reference;
    unsigned int *sourceWords;
    unsigned int *destinationWords;
    int wordCount;
    int result;

    duplicate = (T3DBlitSprite_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(0x70);
    if (duplicate != 0)
    {
        sourceWords = (unsigned int *)source;
        destinationWords = (unsigned int *)duplicate;
        for (wordCount = 0x1c; wordCount != 0; wordCount = wordCount + -1)
        {
            *destinationWords = *sourceWords;
            sourceWords = sourceWords + 1;
            destinationWords = destinationWords + 1;
        }
        duplicate->dictionaryEntry = 0;
        *(unsigned int *)&duplicate->flags = 0;
        duplicate->destructionState = 0;
        result = EQ_CDECL(int, 0x1001564B, (void *, T3DBlitSprite_guess *))(context, duplicate);
        if (result == 0)
        {
            reference = duplicate->reference6c;
            reference->referenceCount = reference->referenceCount + 1;
            reference = duplicate->reference68;
            reference->referenceCount = reference->referenceCount + 1;
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(duplicate);
            duplicate = 0;
        }
    }
    return duplicate;
}

// FUNCTION: EQGFX 0x1000d06a
// Destroys a blit sprite and releases its shared resources.
int __cdecl t3dDestroyBlitSprite(int dictionary, T3DBlitSprite_guess *sprite)
{
    T3DBlitSpriteReference_guess *reference;
    int result;

    if (sprite->destructionState != 0)
    {
        result = 0xffff3d45;
    }
    else
    {
        reference = sprite->reference6c;
        reference->referenceCount = reference->referenceCount + -1;
        reference = sprite->reference68;
        reference->referenceCount = reference->referenceCount + -1;
        if ((sprite->flags & 1) != 0)
            EQ_CDECL(void, 0x1000E334, (int, void *))(dictionary, sprite->dictionaryEntry);
        result = EQ_CDECL(int, 0x10015D81, (int, T3DBlitSprite_guess *))(dictionary, sprite);
        if (sprite->dictionaryEntry != 0)
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->dictionaryEntry);
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
    }
    return result;
}

// FUNCTION: EQGFX 0x1000d0c5
// Rebinds a blit sprite to a different shared sprite resource.
int __cdecl t3dSetBlitSpriteSimpleSprite(T3DBlitSprite_guess *sprite, T3DBlitSpriteReference_guess *reference)
{
    T3DBlitSpriteReference_guess *oldReference;

    oldReference = sprite->reference68;
    oldReference->referenceCount = oldReference->referenceCount - 1;
    sprite->reference68 = reference;
    reference->referenceCount = reference->referenceCount + 1;
    return 0;
}

// FUNCTION: EQGFX 0x1000d0e4
// Applies a color tint and alpha setting to a blit sprite.
int __cdecl t3dSetBlitSpriteTint(T3DBlitSpriteTintContext_guess *context, T3DBlitSpriteTintTarget_guess *sprite, unsigned int *tintColor)
{
    unsigned int tintModeFlags;

    if (sprite->tintMode == 0) {
        tintModeFlags = sprite->tintModeFlags;
        if ((tintModeFlags & BLIT_SPRITE_TINT_INDIRECT_FLAG) != 0) {
            tintModeFlags = context->blitSpriteTintTable[tintModeFlags & BLIT_SPRITE_TINT_INDIRECT_INDEX_MASK];
        }
        if ((tintModeFlags & BLIT_SPRITE_TINT_ALPHA_ENABLED) == 0) {
            sprite->tintAlpha |= BLIT_SPRITE_FULL_ALPHA;
            sprite->tintMode = 1;
        } else {
            if ((tintModeFlags & BLIT_SPRITE_TINT_ALPHA_PLUS_ONE) == 0) {
                sprite->tintMode = 2;
                tintModeFlags = (tintModeFlags >> 0x10) & BLIT_SPRITE_TINT_ALPHA_NIBBLE_MASK;
            } else {
                sprite->tintMode = 3;
                tintModeFlags = ((tintModeFlags >> 0x10) & BLIT_SPRITE_TINT_ALPHA_NIBBLE_MASK) + 1;
            }
            sprite->tintAlpha = (unsigned char)((tintModeFlags * BLIT_SPRITE_FULL_ALPHA) >> 4);
        }
    }
    sprite->renderFlags |= BLIT_SPRITE_FLAG_HAS_TINT;
    sprite->tintColor = *tintColor;
    return 0;
}

// FUNCTION: EQGFX 0x1000d0dc
// __cdecl. Returns the sprite's current frame at +0x68.
int __cdecl t3dGet2DSpriteCurrentFrame(Sprite2D_guess *sprite)
{
    return sprite->currentFrame;
}

// FUNCTION: EQGFX 0x1000d159
// Defines a reusable multipart sprite animation with frame data, timing, and shared component sprites.
// __cdecl (twelve stack arguments; the callee pops 0). Returns the type-0x11 definition named name, creating it with its template sprite when absent; 0 otherwise.
T3DCompositeSpriteDefinition_guess *__cdecl t3dDefineCompositeSprite_guess(void *dictionary, char *name, int width, int height, void *partData, unsigned int currentFrame, int updateInterval, int unknown24, T3DCompositeSpriteDestroyChild_guess *components, T3DCompositeSpriteCenterOffset_guess *centerOffset, void *volume, int definitionWord20)
{
    T3DCompositeSpriteDefinition_guess *definition;
    T3DCompositeSprite_guess *sprite;
    T3DCompositeSpriteDestroyChild_guess *component;
    T3DCompositeSpriteDestroyChild_guess *componentEnd;
    size_t nameLength;
    size_t partBytes;
    /* fake match: these locals sit in an inner block for the original's stack layout */
    {
        char *nameBuffer;
        void *partBuffer;
        int status;
        int componentType;

        definition = (T3DCompositeSpriteDefinition_guess *)parseLookup(dictionary, name);
        if (definition != 0)
        {
            if (definition->type == COMPOSITE_SPRITE_DEFINITION_TYPE)
            {
                return definition;
            }
            return 0;
        }
        definition = GFX_COMPOSITE_ALLOCATE_MEMORY(COMPOSITE_SPRITE_DEFINITION_SIZE);
        if (definition == 0)
        {
            return definition;
        }
        definition->flags = 0;
        definition->type = COMPOSITE_SPRITE_DEFINITION_TYPE;
        definition->referenceCount = 1;
        definition->width = width;
        definition->componentCount = height;
        definition->definitionWord20 = definitionWord20;
        nameLength = strlen_EQGFX(name);
        nameBuffer = GFX_COMPOSITE_ALLOCATE_MEMORY(nameLength + 1);
        definition->name = nameBuffer;
        if (nameBuffer == 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(definition);
            return 0;
        }
        GFX_PATH_COPY_STRING(nameBuffer, name);
        partBytes = width * height * COMPOSITE_SPRITE_PART_SIZE;
        partBuffer = GFX_COMPOSITE_ALLOCATE_MEMORY(partBytes);
        definition->parts = partBuffer;
        if (partBuffer == 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->name);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition);
            return 0;
        }
        EQGFX_MEMCPY_GUESS(partBuffer, partData, partBytes);
        status = GFX_COMPOSITE_INITIALIZE_OBJECT(dictionary, definition);
        if (status != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->name);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->parts);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition);
            return 0;
        }
        status = parseAddToDictionary(dictionary, definition);
        if (status < 0)
        {
            GFX_COMPOSITE_RELEASE_OBJECT(dictionary, definition);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->name);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->parts);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition);
            return 0;
        }
        sprite = GFX_COMPOSITE_ALLOCATE_MEMORY(COMPOSITE_SPRITE_CREATE_SIZE);
        if (sprite == 0)
        {
            GFX_COMPOSITE_REMOVE_ENTRY(dictionary, name);
            GFX_COMPOSITE_RELEASE_OBJECT(dictionary, definition);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->name);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->parts);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition);
            return 0;
        }
        definition->spriteDefinition = (T3D2DSpriteDefinition_guess *)sprite;
        sprite->currentFrame = currentFrame;
        sprite->entry = 0;
        sprite->updateInterval = updateInterval;
        sprite->flags = 0;
        sprite->contextOffset = 0;
        sprite->type = SPRITE_DEFINITION_TYPE;
        sprite->state = 1;
        sprite->unknown24 = unknown24;
        *(T3DCompositeSpriteCenterOffset_guess *)&sprite->centerOffsetX = *centerOffset;
        sprite->frameData = (T3DCompositeSpriteFrameData_guess *)definition;
        sprite->volume = volume;
        sprite->components = components;
        status = GFX_COMPOSITE_INITIALIZE_OBJECT(dictionary, sprite);
        if (status != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
            GFX_COMPOSITE_REMOVE_ENTRY(dictionary, name);
            GFX_COMPOSITE_RELEASE_OBJECT(dictionary, definition);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->name);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->parts);
            EQGFX_T3D_FREE_MEMORY_GUESS(definition);
            return 0;
        }
        component = components;
        componentEnd = components + height;
        while (component < componentEnd)
        {
            if (component->sprite == 0)
            {
                component->sprite = &GFX_EMPTY_OBJECT;
            }
            componentType = component->sprite->type;
            if (componentType != 0)
            {
                if (componentType != COMPOSITE_SPRITE_CREATE_SHARED_TYPE)
                {
                    component--;
                    while (component >= sprite->components)
                    {
                        if (component->sprite->type == COMPOSITE_SPRITE_CREATE_SHARED_TYPE)
                        {
                            GFX_COMPOSITE_UNSHARE_OBJECT(component->sprite);
                        }
                        component--;
                    }
                    GFX_COMPOSITE_RELEASE_OBJECT(dictionary, sprite);
                    EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
                    GFX_COMPOSITE_REMOVE_ENTRY(dictionary, name);
                    GFX_COMPOSITE_RELEASE_OBJECT(dictionary, definition);
                    EQGFX_T3D_FREE_MEMORY_GUESS(definition->name);
                    EQGFX_T3D_FREE_MEMORY_GUESS(definition->parts);
                    EQGFX_T3D_FREE_MEMORY_GUESS(definition);
                    return 0;
                }
                GFX_COMPOSITE_SHARE_OBJECT(component->sprite);
            }
            component++;
        }
        return definition;
    }
}

// FUNCTION: EQGFX 0x1000d3e2
// Removes a composite sprite definition and its template when no instances still use it.
int __cdecl t3dUndefineCompositeSprite(void *dictionary, CompositeSpriteUndefineSprite_guess *sprite)
{
    int result;
    CompositeSpriteUndefineDefinition_guess *definition;
    CompositeSpriteUndefineComponent_guess *component;
    CompositeSpriteUndefineComponent_guess *componentEnd;
    int componentType;

    definition = sprite->definition;
    if (sprite->state == 1 && definition->referenceCount == 1)
    {
        GFX_COMPOSITE_DESTROY_VOLUME(dictionary, definition->volume);
        if ((definition->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, definition->entry);
        }
        GFX_COMPOSITE_RELEASE_OBJECT(dictionary, definition);
        component = definition->components;
        componentEnd = component + sprite->componentCount;
        for (; component < componentEnd; component++)
        {
            componentType = *(int *)component->sprite;
            if (componentType != 0)
            {
                if (componentType == COMPOSITE_SPRITE_COMPONENT_TYPE)
                {
                    GFX_COMPOSITE_UNSHARE_OBJECT(component->sprite);
                    EQ_CDECL(void, 0x1000879D, (void *, void *))(dictionary, component->sprite);
                }
                else
                {
                    GFX_COMPOSITE_DEBUG_BREAK();
                }
            }
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(definition->components);
        if (definition->entry != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(definition->entry);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(definition);
        if ((sprite->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, sprite->entry);
        }
        result = GFX_COMPOSITE_RELEASE_OBJECT(dictionary, sprite);
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite->ownedBuffer);
        if (sprite->entry != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->entry);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
    }
    else
    {
        result = COMPOSITE_SPRITE_UNDEFINE_ERROR;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000d4e2
// Creates a composite sprite instance from a definition, sharing its component sprites.
T3DCompositeSprite_guess *__cdecl t3dCreateCompositeSprite(T3DCompositeSpriteCreateContext_guess *context, T3DCompositeSpriteDefinition_guess *definition, void *volume)
{
    T3DCompositeSprite_guess *sprite;
    unsigned int *sourceWord;
    unsigned int *destinationWord;
    int copyCount;
    size_t componentBytes;
    void *componentData;
    T3DCompositeSpriteDestroyChild_guess *component;
    T3DCompositeSpriteDestroyChild_guess *componentEnd;
    int componentType;

    sprite = (T3DCompositeSprite_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(COMPOSITE_SPRITE_CREATE_SIZE);
    if (sprite == 0)
    {
        return 0;
    }
    sourceWord = (unsigned int *)definition->spriteDefinition;
    destinationWord = (unsigned int *)sprite;
    for (copyCount = COMPOSITE_SPRITE_CREATE_TEMPLATE_WORD_COUNT; copyCount != 0; copyCount--)
    {
        *destinationWord = *sourceWord;
        sourceWord++;
        destinationWord++;
    }
    sprite->entry = 0;
    sprite->flags = 0;
    sprite->state = 0;
    sprite->contextOffset = sprite->updateInterval + context->contextOffset;
    if (volume == 0)
    {
        volume = definition->spriteDefinition->volume;
    }
    sprite->volume = GFX_COMPOSITE_DUPLICATE_VOLUME(context, volume);
    if (sprite->volume != 0)
    {
        componentBytes = definition->componentCount * COMPOSITE_SPRITE_CREATE_COMPONENT_SIZE;
        componentData = GFX_COMPOSITE_ALLOCATE_MEMORY((unsigned int)componentBytes);
        sprite->components = componentData;
        if (componentData != 0)
        {
            EQGFX_MEMCPY_GUESS(componentData, definition->spriteDefinition->components, (unsigned int)componentBytes);
            if (GFX_COMPOSITE_INITIALIZE_SPRITE(context, sprite) != 0)
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(sprite->components);
                EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
                return 0;
            }
            component = (T3DCompositeSpriteDestroyChild_guess *)sprite->components;
            componentEnd = component + definition->componentCount;
            for (; component < componentEnd; component++)
            {
                componentType = component->sprite->type;
                if (componentType != 0)
                {
                    if (componentType == COMPOSITE_SPRITE_CREATE_SHARED_TYPE)
                    {
                        GFX_COMPOSITE_SHARE_OBJECT(component->sprite);
                    }
                    else
                    {
                        GFX_COMPOSITE_DEBUG_BREAK();
                    }
                }
            }
            definition->referenceCount++;
            return sprite;
        }
    }
    EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
    return 0;
}

// FUNCTION: EQGFX 0x1000d5d6
// Duplicates an existing composite sprite instance, retaining its shared components and definition.
T3DCompositeSprite_guess *__cdecl t3dDuplicateCompositeSprite(T3DCompositeSpriteCreateContext_guess *context, T3DCompositeSprite_guess *spriteTemplate, void *volume)
{
    T3DCompositeSprite_guess *sprite;
    unsigned int *sourceWord;
    unsigned int *destinationWord;
    int copyCount;
    size_t componentBytes;
    void *componentData;
    T3DCompositeSpriteDestroyChild_guess *component;
    T3DCompositeSpriteDestroyChild_guess *componentEnd;
    int componentType;

    sprite = (T3DCompositeSprite_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(COMPOSITE_SPRITE_CREATE_SIZE);
    if (sprite != 0)
    {
        sourceWord = (unsigned int *)spriteTemplate;
        destinationWord = (unsigned int *)sprite;
        for (copyCount = COMPOSITE_SPRITE_CREATE_TEMPLATE_WORD_COUNT; copyCount != 0; copyCount--)
        {
            *destinationWord = *sourceWord;
            sourceWord++;
            destinationWord++;
        }
        sprite->entry = 0;
        sprite->flags = 0;
        sprite->state = 0;
        sprite->contextOffset = sprite->updateInterval + context->contextOffset;
        if (volume == 0)
        {
            volume = spriteTemplate->volume;
        }
        sprite->volume = GFX_COMPOSITE_DUPLICATE_VOLUME(context, volume);
        if (sprite->volume != 0)
        {
            componentBytes = sprite->frameData->partsPerFrame * COMPOSITE_SPRITE_CREATE_COMPONENT_SIZE;
            componentData = GFX_COMPOSITE_ALLOCATE_MEMORY((unsigned int)componentBytes);
            sprite->components = componentData;
            if (componentData != 0)
            {
                EQGFX_MEMCPY_GUESS(componentData, spriteTemplate->components, (unsigned int)componentBytes);
                if (GFX_COMPOSITE_INITIALIZE_SPRITE(context, sprite) == 0)
                {
                    component = (T3DCompositeSpriteDestroyChild_guess *)sprite->components;
                    componentEnd = component + sprite->frameData->partsPerFrame;
                    for (; component < componentEnd; component++)
                    {
                        componentType = component->sprite->type;
                        if (componentType != 0)
                        {
                            if (componentType == COMPOSITE_SPRITE_COMPONENT_TYPE)
                            {
                                GFX_COMPOSITE_SHARE_OBJECT(component->sprite);
                            }
                            else
                            {
                                GFX_COMPOSITE_DEBUG_BREAK();
                            }
                        }
                    }
                    sprite->frameData->referenceCount++;
                    return sprite;
                }
                GFX_COMPOSITE_DESTROY_VOLUME(context, sprite->volume);
                EQGFX_T3D_FREE_MEMORY_GUESS(sprite->components);
                EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
            }
            else
            {
                GFX_COMPOSITE_DESTROY_VOLUME(context, sprite->volume);
                EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
            }
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000d6e5
// Destroys a composite sprite instance and releases its component references while keeping its definition.
int __cdecl t3dDestroyCompositeSprite(void *context, T3DCompositeSpriteDestroy_guess *sprite)
{
    int result;
    int childType;
    int childCount;
    int *childWords;
    int *end;
    T3DCompositeSpriteDestroyChild_guess *childRecord;

    if (sprite->state != 0)
    {
        result = 0xffff3d45;
    }
    else
    {
        sprite->definition->referenceCount--;
        GFX_COMPOSITE_DESTROY_VOLUME(context, sprite->volume);
        if ((sprite->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(context, sprite->entry);
        }
        result = GFX_COMPOSITE_RELEASE_OBJECT(context, sprite);
        childCount = sprite->definition->childCount;
        childWords = (int *)sprite->children;
        end = childWords + childCount * 0x28;
        for (; childWords < end; childWords += 0x28)
        {
            childRecord = (T3DCompositeSpriteDestroyChild_guess *)childWords;
            childType = childRecord->sprite->type;
            if (childType != 0)
            {
                if (childType == 0x0c)
                {
                    GFX_COMPOSITE_UNSHARE_OBJECT(childRecord->sprite);
                    EQ_CDECL(void, 0x1000879D, (void *, void *))(context, childRecord->sprite);
                }
                else
                {
                    ((void (__stdcall *)(void))GFX_DEBUG_BREAK)();
                }
            }
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite->children);
        if (sprite->entry != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(sprite->entry);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(sprite);
    }
    return result;
}

// FUNCTION: EQGFX 0x1000d7be
// __cdecl (callee pops 0 bytes; ECX is not an input). Tag length at +0x8, 0 when null.
int __cdecl t3dGet2DSpriteDefinitionTagLength(char *definition)
{
    char *tag;

    tag = *(char **)(definition + 8);
    if (tag == 0)
        return 0;
    return EQ_CDECL(int, 0x10068160, (const char *))(tag);
}

typedef float (__cdecl *PartScaleFunction_guess)(float value);

// FUNCTION: EQGFX 0x1000d7d2
// Reads a composite sprite part's position, rotation, and scale for a frame.
// __cdecl (callee pops 0 bytes; ECX is not an input). Part frameIndex * partsPerFrame + partIndex: rotation scaled by the scale function, heading/pitch/roll out, translation out; status or SURFACE_RESIZE_ERROR.
int __cdecl t3dGetCompositeSpriteDefinitionTransformation(T3DCompositeSpriteFrameData_guess *frameData, unsigned int partIndex, unsigned int frameIndex, float *scaleOut, float *headingOut, float *pitchOut, float *rollOut, float *translationOut)
{
    T3DCompositeSpritePart_guess *part;
    float scaledRotation[9];
    float scale;
    int status;

    if (partIndex < frameData->partsPerFrame && frameIndex < frameData->frameCount)
    {
        part = frameData->parts + (frameData->partsPerFrame * frameIndex + partIndex);
        scale = GFX_PART_SCALE_FUNCTION(part->scale);
        *scaleOut = scale;
        scaledRotation[0] = scale * part->scale * part->rotation[0];
        scaledRotation[1] = scale * part->scale * part->rotation[1];
        scaledRotation[2] = scale * part->scale * part->rotation[2];
        scaledRotation[3] = scale * part->scale * part->rotation[3];
        scaledRotation[4] = scale * part->scale * part->rotation[4];
        scaledRotation[5] = scale * part->scale * part->rotation[5];
        scaledRotation[6] = scale * part->scale * part->rotation[6];
        scaledRotation[7] = scale * part->scale * part->rotation[7];
        scaledRotation[8] = scale * part->scale * part->rotation[8];
        status = EQ_CDECL(int, 0x1004F049, (float *, float *, float *, float *))(scaledRotation, headingOut, pitchOut, rollOut);
        if (status >= 0)
        {
            translationOut[0] = part->translation[0];
            translationOut[1] = part->translation[1];
            translationOut[2] = part->translation[2];
            return status;
        }
    }
    return SURFACE_RESIZE_ERROR;
}

// FUNCTION: EQGFX 0x1000d79f
// Checks that a composite sprite definition links to a valid 2D sprite definition.
int __cdecl t3dIsCompositeSpriteDefinitionValid(T3DCompositeSpriteDefinition_guess *definition)
{
    int result;

    result = 1;
    if (definition == 0 || definition->spriteDefinition == 0
        || definition->type != COMPOSITE_SPRITE_DEFINITION_TYPE
        || definition->spriteDefinition->type != SPRITE_DEFINITION_TYPE) {
        result = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000d893
// Sets a composite sprite part's position, rotation, and scale for a frame.
// __cdecl (callee pops 0 bytes; ECX is not an input). Part partsPerFrame * frameIndex + partIndex gets the callee's coefficients scaled by scale; status or SURFACE_RESIZE_ERROR.
int __cdecl t3dSetCompositeSpriteDefinitionTransformation(T3DCompositeSpriteFrameData_guess *frameData, unsigned int partIndex, unsigned int frameIndex, float scale, float heading, float pitch, float roll, float *translation)
{
    T3DCompositeSpritePart_guess *part;
    int status;

    if (partIndex < frameData->partsPerFrame && frameIndex < frameData->frameCount)
    {
        part = frameData->parts + (frameData->partsPerFrame * frameIndex + partIndex);
        status = EQ_CDECL(int, 0x1004F2DD, (float, float, float, float *))(heading, pitch, roll, part->rotation);
        if (status >= 0)
        {
            part->rotation[0] = scale * part->rotation[0];
            part->rotation[1] = scale * part->rotation[1];
            part->rotation[2] = scale * part->rotation[2];
            part->rotation[3] = scale * part->rotation[3];
            part->rotation[4] = scale * part->rotation[4];
            part->rotation[5] = scale * part->rotation[5];
            part->rotation[6] = scale * part->rotation[6];
            part->rotation[7] = scale * part->rotation[7];
            part->rotation[8] = scale * part->rotation[8];
            part->translation[0] = translation[0];
            part->translation[1] = translation[1];
            part->translation[2] = translation[2];
            part->scale = 1.0f / (scale * scale);
            return status;
        }
    }
    return SURFACE_RESIZE_ERROR;
}

// FUNCTION: EQGFX 0x1000d95e
// Checks that a composite sprite instance has valid frame data.
int __cdecl t3dIsCompositeSpriteValid(T3DCompositeSprite_guess *sprite)
{
    int result;

    result = 1;
    if (sprite == 0 || sprite->frameData == 0
        || sprite->frameData->type != COMPOSITE_SPRITE_DEFINITION_TYPE
        || sprite->type != SPRITE_DEFINITION_TYPE) {
        result = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000d97d
// Selects the frame shown by a composite sprite.
int __cdecl t3dSetCompositeSpriteCurrentFrame(T3DCompositeSprite_guess *sprite, unsigned int frameIndex)
{
    if (sprite->frameData->frameCount <= frameIndex)
        return SURFACE_RESIZE_ERROR;
    sprite->currentFrame = frameIndex;
    return 0;
}

// FUNCTION: EQGFX 0x1000d9a3
// Sets the delay between composite sprite frame updates.
int __cdecl t3dSetCompositeSpriteUpdateInterval(T3DCompositeSprite_guess *sprite, int interval)
{
    sprite->updateInterval = interval;
    return 0;
}

// FUNCTION: EQGFX 0x1000D9C6
// Sets the composite sprite's center offset.
int __cdecl t3dSetCompositeSpriteCenterOffset(T3DCompositeSprite_guess *sprite, float *centerOffset)
{
    sprite->centerOffsetX = centerOffset[0];
    sprite->centerOffsetY = centerOffset[1];
    sprite->centerOffsetZ = centerOffset[2];
    return 0;
}

// FUNCTION: EQGFX 0x1000d9b1
// Reports the composite sprite's center offset.
int __cdecl t3dGetCompositeSpriteCenterOffset(T3DCompositeSprite_guess *sprite, float *centerOffset)
{
    centerOffset[0] = sprite->centerOffsetX;
    centerOffset[1] = sprite->centerOffsetY;
    centerOffset[2] = sprite->centerOffsetZ;
    return 0;
}
// FUNCTION: EQGFX 0x1000d956
// __cdecl (callee pops 0 bytes; ECX is not an input). Cycle count at +0x24.
int __cdecl s3dGetDMRGBTrackNumCyclesToPlay(DMRGBTrack_guess *track)
{
    return track->numCyclesToPlay;
}

// FUNCTION: EQGFX 0x1000d99b
// __cdecl. Render method at +0x1c.
int __cdecl t3dGet2DSpriteRenderMethod(Sprite2D_guess *sprite)
{
    return sprite->renderMethod;
}

// FUNCTION: EQGFX 0x1000d9db
// __cdecl. Image buffer width at +0x38.
int __cdecl t3dGetCameraImageBufferWidth(Camera_guess *camera)
{
    return camera->imageBufferWidth;
}
