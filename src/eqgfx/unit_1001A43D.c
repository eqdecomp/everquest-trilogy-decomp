// unit_1001A43D.c - eqgfx_dx7.dll, the address run 0x1001A43D..0x1001B7EF (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include <string.h>
#include "trilogy/eqaddr.h"

#define EQGFX_HALF_SCALE (*(const float *)0x10076290)
#define EQGFX_DEFAULT_SPRITE_SCALE_BITS (*(unsigned int *)0x10086DB0)
#define SPRITE_DEFINITION_INVALID 0xFFFF3D3E
#define EQGFX_FONT_SUFFIX ((const char *)0x10086DB4)
#define g_render_mode_guess EQ_GLOBAL(int, 0x100FB8A8)
#define EQGFX_ONE (*(const float *)0x10076280)

typedef struct SpriteType12RadiusData_guess
{
    unsigned char pad00[0x20];
    float radius;
} SpriteType12RadiusData_guess;

typedef struct SpriteType10RadiusData_guess
{
    unsigned char pad00[0x2c];
    float radius;
} SpriteType10RadiusData_guess;

typedef struct SpriteType0cRadiusData_guess
{
    unsigned char pad00[0x28];
    float radius;
} SpriteType0cRadiusData_guess;

typedef struct SpriteType14RadiusData_guess
{
    unsigned char pad00[0x18];
    float radius;
} SpriteType14RadiusData_guess;

typedef struct SpriteType48PrimaryRadiusData_guess
{
    unsigned char pad00[0x30];
    float radius;
} SpriteType48PrimaryRadiusData_guess;

typedef struct SpriteType48FallbackRadiusData_guess
{
    unsigned char pad00[0x58];
    float radius;
} SpriteType48FallbackRadiusData_guess;

typedef struct SpriteType48RadiusData_guess
{
    unsigned char pad00[0x14];
    SpriteType48PrimaryRadiusData_guess *primary;
    SpriteType48FallbackRadiusData_guess *fallback;
} SpriteType48RadiusData_guess;

typedef struct SpriteDef_guess
{
    unsigned int spriteType;
    union
    {
        struct { unsigned char pad04[0x10]; unsigned int center[3]; } type48;
        struct { unsigned char pad04[0x18]; unsigned int center[3]; } type0c;
        struct { unsigned char pad04[0x24]; unsigned int center[3]; } type14;
        struct { unsigned char pad04[0x28]; unsigned int center[3]; } type12;
        struct { unsigned char pad04[0x74]; unsigned int center[3]; } type10;
        struct { unsigned char pad04[0x28]; unsigned int definition; } type0cDefinition;
        struct { unsigned char pad04[0x34]; unsigned int definition; } type12Definition;
        struct { unsigned char pad04[0x84]; unsigned int definition; } type10Definition;
        struct { unsigned char pad04[0x58]; unsigned int definition; } type14Definition;
        struct { unsigned char pad04[0x30]; unsigned int definition; } type48Definition;
        struct { unsigned char pad04[0x34]; SpriteType12RadiusData_guess *radiusData; } type12Radius;
        struct { unsigned char pad04[0x84]; SpriteType10RadiusData_guess *radiusData; } type10Radius;
        struct { unsigned char pad04[0x28]; SpriteType0cRadiusData_guess *radiusData; } type0cRadius;
        struct { unsigned char pad04[0x58]; SpriteType14RadiusData_guess *radiusData; } type14Radius;
        struct { unsigned char pad04[0x30]; SpriteType48RadiusData_guess *radiusData; } type48Radius;
    } centerData;
} SpriteDef_guess;

typedef struct FontTable_guess
{
    unsigned char pad00[0x14];
    int baseChar;
    unsigned char pad18[4];
    int value1c;
    int *widths;
} FontTable_guess;

typedef struct StringSprite_guess
{
    unsigned char pad00[8];
    char *dictionaryKey;
    unsigned char flags;
    unsigned char pad0d[7];
    FontTable_guess *font;
    char *text;
    unsigned char pad1c[8];
    int maxScaleFlag24;
    float maxScaleFactor28;
    float invMaxScaleFactor2c;
    int yonClip30;
    int yonClip34;
    float scale38;
    int totalWidth;
    int value40;
    float halfDiagonal44;
    int tint;
} StringSprite_guess;

typedef struct FontTexture_guess
{
    unsigned int typeTag;
    unsigned int reserved04;
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad0d[3];
    int useCount;
    unsigned int minChar;
    unsigned int maxChar;
    unsigned int fontHeight;
    void *buffer20;
    void *buffer24;
    void *buffer28;
    void *bitmapInfo;
} FontTexture_guess;

typedef struct FontCharABC_guess
{
    int abcA;
    unsigned int abcB;
    int abcC;
} FontCharABC_guess;

typedef struct FontBitmapDevice_guess FontBitmapDevice_guess;

typedef int (__stdcall *FontBitmapGetDC_guess_t)(FontBitmapDevice_guess *, void **);
typedef int (__stdcall *FontBitmapReleaseDC_guess_t)(FontBitmapDevice_guess *, void *);

typedef struct FontBitmapDeviceVTable_guess
{
    unsigned char pad00[0x44];
    FontBitmapGetDC_guess_t getDC;
    unsigned char pad48[0x20];
    FontBitmapReleaseDC_guess_t releaseDC;
} FontBitmapDeviceVTable_guess;

struct FontBitmapDevice_guess
{
    FontBitmapDeviceVTable_guess *vtable;
};

typedef struct FontBitmapSurface_guess
{
    unsigned char pad00[0x1c];
    unsigned int width;
    unsigned int height;
    unsigned char pad24[0x3c];
    FontBitmapDevice_guess *device;
} FontBitmapSurface_guess;

typedef struct FontBitmapInfo_guess
{
    FontBitmapSurface_guess *surface;
} FontBitmapInfo_guess;

typedef struct FontLogFont_guess
{
    int height;
    int width;
    int escapement;
    int orientation;
    int weight;
    unsigned char italic;
    unsigned char underline;
    unsigned char strikeOut;
    unsigned char charSet;
    unsigned char outPrecision;
    unsigned char clipPrecision;
    unsigned char quality;
    unsigned char pitchAndFamily;
    char faceName[32];
} FontLogFont_guess;

typedef struct FontRect_guess
{
    int left;
    int top;
    int right;
    int bottom;
} FontRect_guess;

typedef struct FontSurfaceLockDescription_guess
{
    unsigned char pad00[0x10];
    int pitch;
    unsigned char pad14[0x10];
    unsigned short *pixels;
    unsigned char pad28[0x54];
} FontSurfaceLockDescription_guess;

typedef void *(__stdcall *EqgfxCreateCompatibleDC_guess_t)(void *);
typedef void *(__stdcall *EqgfxCreateFontIndirectA_guess_t)(FontLogFont_guess *);
typedef void *(__stdcall *EqgfxSelectObject_guess_t)(void *, void *);
typedef int (__stdcall *EqgfxDeleteDC_guess_t)(void *);
typedef int (__stdcall *EqgfxDeleteObject_guess_t)(void *);
typedef int (__stdcall *EqgfxGetCharABCWidthsA_guess_t)(void *, unsigned int, unsigned int, FontCharABC_guess *);
typedef unsigned int (__stdcall *EqgfxSetTextColor_guess_t)(void *, unsigned int);
typedef int (__stdcall *EqgfxSetBkMode_guess_t)(void *, int);
typedef unsigned int (__stdcall *EqgfxSetBkColor_guess_t)(void *, unsigned int);
typedef void *(__stdcall *EqgfxCreateSolidBrush_guess_t)(unsigned int);
typedef int (__stdcall *EqgfxFillRect_guess_t)(void *, FontRect_guess *, void *);
typedef int (__stdcall *EqgfxTextOutA_guess_t)(void *, int, int, const char *, int);

#define EQGFX_CREATE_COMPATIBLE_DC EQ_GLOBAL(EqgfxCreateCompatibleDC_guess_t, 0x10076050)
#define EQGFX_CREATE_FONT_INDIRECT_A EQ_GLOBAL(EqgfxCreateFontIndirectA_guess_t, 0x1007604C)
#define EQGFX_SELECT_OBJECT EQ_GLOBAL(EqgfxSelectObject_guess_t, 0x10076018)
#define EQGFX_DELETE_DC EQ_GLOBAL(EqgfxDeleteDC_guess_t, 0x10076060)
#define EQGFX_DELETE_OBJECT EQ_GLOBAL(EqgfxDeleteObject_guess_t, 0x10076044)
#define EQGFX_GET_CHAR_ABC_WIDTHS_A EQ_GLOBAL(EqgfxGetCharABCWidthsA_guess_t, 0x10076010)
#define EQGFX_SET_TEXT_COLOR EQ_GLOBAL(EqgfxSetTextColor_guess_t, 0x10076014)
#define EQGFX_SET_BK_MODE EQ_GLOBAL(EqgfxSetBkMode_guess_t, 0x10076024)
#define EQGFX_SET_BK_COLOR EQ_GLOBAL(EqgfxSetBkColor_guess_t, 0x1007601C)
#define EQGFX_CREATE_SOLID_BRUSH EQ_GLOBAL(EqgfxCreateSolidBrush_guess_t, 0x10076020)
#define EQGFX_FILL_RECT EQ_GLOBAL(EqgfxFillRect_guess_t, 0x1007625C)
#define EQGFX_TEXT_OUT_A EQ_GLOBAL(EqgfxTextOutA_guess_t, 0x10076030)
#define EQGFX_MEMSET(destination, value, size) EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(destination, value, size)
#define EQGFX_COPY_STRING(destination, source) EQ_CDECL(void, 0x10068070, (char *, const char *))(destination, source)
#define EQGFX_STR_CAT(destination, source) EQ_CDECL(char *, 0x10068080, (char *, const char *))(destination, source)
#define EQGFX_STRING_LENGTH(string) EQ_CDECL(unsigned int, 0x10068160, (char *))(string)
#define EQGFX_ALLOCATE_MEMORY(size) EQ_CDECL(void *, 0x10050C48, (unsigned int))(size)
#define EQGFX_FREE_MEMORY(memory) EQ_CDECL(void, 0x10050CBB, (void *))(memory)
#define EQGFX_DEFINE_BITMAP_INFO(context, name, width, height) EQ_CDECL(FontBitmapInfo_guess *, 0x1000B92F, (void *, const char *, unsigned int, unsigned int))(context, name, width, height)
#define EQGFX_VALIDATE_SURFACE(surface) EQ_CDECL(int, 0x1000CC0D, (FontBitmapSurface_guess *))(surface)
#define EQGFX_LOCK_SURFACE(surface, description) EQ_CDECL(int, 0x1000CD5B, (FontBitmapSurface_guess *, void *))(surface, description)
#define EQGFX_UNLOCK_SURFACE(surface) EQ_CDECL(void, 0x1000CDB3, (FontBitmapSurface_guess *))(surface)
#define EQGFX_CHECK_SURFACE(surface) EQ_CDECL(int, 0x1000CB51, (FontBitmapSurface_guess *))(surface)
#define EQGFX_FINALIZE_SURFACE(surface) EQ_CDECL(void, 0x1003193D, (FontBitmapSurface_guess *))(surface)
#define EQGFX_FLOAT_TO_INT() EQ_CDECL(int, 0x1004EC36, (void))()

typedef struct FontRange_guess
{
    unsigned char pad00[0x14];
    unsigned char minChar;
    unsigned char pad15[3];
    unsigned char maxChar;
} FontRange_guess;

typedef struct SphereListDefinition_guess
{
    int type;
    unsigned char pad04[4];
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad0d[3];
    int useCount;
    int definitionValue;
    int count;
    void *centerList;
    void *radiusList;
    void *childEntry;
} SphereListDefinition_guess;

typedef struct SphereListChildEntry_guess
{
    int type;
    unsigned char pad04[4];
    int field08;
    int field0c;
    int field10;
    int entryValue;
    SphereListDefinition_guess *definition;
} SphereListChildEntry_guess;

typedef struct SphereListInstance_guess
{
    unsigned char pad00[8];
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad0d[3];
    int state;
    unsigned char pad14[8];
    void *centerList;
    void *radiusList;
    SphereListDefinition_guess *definition;
} SphereListInstance_guess;

typedef struct SimpleSpriteInfo_guess
{
    int kind;
    unsigned char pad04[4];
    void *dictionaryKey;
    int flags;
    int useCount;
    void **sourceObject;
    int sourceIndex;
    int configuration;
    int reserved20;
    int optionFlags;
    struct SimpleSpriteDefinition_guess *definition;
} SimpleSpriteInfo_guess;

typedef struct SimpleSpriteDefinition_guess
{
    int kind;
    unsigned char pad04[4];
    void *dictionaryKey;
    int flags;
    int useCount;
    int infoCount;
    void *infoList;
    SimpleSpriteInfo_guess *info;
} SimpleSpriteDefinition_guess;

typedef struct SimpleSpriteObject_guess
{
    unsigned char pad00[8];
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad0d[3];
    int useCount;
    int objectCount;
    void **objects;
    SimpleSpriteDefinition_guess *definition;
} SimpleSpriteObject_guess;

typedef struct SphereWorldScale_guess
{
    unsigned char pad00[0x10];
    float worldPosition[3];
    unsigned char pad1c[0x18];
    float worldScale;
    unsigned char pad38[0x30];
    float worldMatrix[9];
} SphereWorldScale_guess;

typedef struct SphereListObject_guess
{
    unsigned char pad00[8];
    void *dictionaryKey;
    unsigned int flags;
    SphereWorldScale_guess *scaleData;
    float localScale;
    SphereListDefinition_guess *definition;
} SphereListObject_guess;

typedef struct SphereListCreateInfo_guess
{
    unsigned char pad00[0x24];
    SphereListObject_guess *prototype;
} SphereListCreateInfo_guess;

typedef struct SpriteWorldScale_guess
{
    unsigned char pad00[0x10];
    float worldPosition[3];
    unsigned char pad1c[0x18];
    float worldScale;
    unsigned char pad38[0x54];
    float worldMatrix[9];
} SpriteWorldScale_guess;

typedef struct Vector3_guess
{
    float x;
    float y;
    float z;
} Vector3_guess;

// FUNCTION: EQGFX 0x1001a43d
// Copies a 28-byte value; its engine purpose is unclear.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Copies 7 dwords
// (28 bytes) from srcWords to destWords; does nothing when destWords is NULL.
void __cdecl t3dSetCurrentLanguage_guess(unsigned int *destWords, unsigned int *srcWords)
{
    int count;

    if (destWords != 0)
    {
        count = 7;
        do
        {
            *destWords = *srcWords;
            destWords++;
            srcWords++;
            count--;
        } while (count != 0);
    }
}

 // FUNCTION: EQGFX 0x1001a453
// Returns 0 when the header tag is 0x50, else 0xffff3d3e.
int __cdecl s3dIsFontTextureValid(int *record)
{
    int result;

    result = 0;
    if (record == 0)
        result = 0xffff3d3e;
    if (*record != 0x50)
        result = 0xffff3d3e;
    return result;
}

// FUNCTION: EQGFX 0x1001a46c
// Creates a font texture for rendering a chosen character range at the requested font height.
FontTexture_guess *__cdecl s3dCreateFontTexture(void *renderer, char *textureName, unsigned short *fontName, unsigned int fontHeight, int minChar, int maxChar)
{
    FontTexture_guess *texture;
    size_t nameLength;
    int allocationResult;
    int glyphCount;

    texture = (FontTexture_guess *)EQGFX_ALLOCATE_MEMORY(0x30);
    if (texture == 0)
        return (FontTexture_guess *)0;
    texture->minChar = minChar;
    texture->typeTag = 0x50;
    texture->reserved04 = 0;
    texture->dictionaryKey = 0;
    *(unsigned int *)&texture->flags = 0;
    texture->useCount = 0;
    texture->maxChar = maxChar;
    texture->fontHeight = fontHeight;
    texture->buffer24 = 0;
    texture->buffer28 = 0;
    texture->bitmapInfo = 0;
    if (textureName != 0)
    {
        nameLength = EQGFX_STRING_LENGTH(textureName);
        allocationResult = (int)EQGFX_ALLOCATE_MEMORY((unsigned int)(nameLength + 1));
        texture->dictionaryKey = (void *)allocationResult;
        if (allocationResult == 0)
            goto cleanup;
        EQGFX_COPY_STRING((char *)texture->dictionaryKey, textureName);
    }
    glyphCount = (maxChar - minChar) + 1;
    allocationResult = (int)EQGFX_ALLOCATE_MEMORY(glyphCount * 0x10);
    texture->buffer24 = (void *)allocationResult;
    if (allocationResult != 0)
    {
        allocationResult = (int)EQGFX_ALLOCATE_MEMORY(glyphCount * 8);
        texture->buffer28 = (void *)allocationResult;
        if (allocationResult != 0)
        {
            allocationResult = (int)EQGFX_ALLOCATE_MEMORY(glyphCount * 4);
            texture->buffer20 = (void *)allocationResult;
            if (allocationResult != 0 &&
                (allocationResult = EQ_CDECL(int, 0x1001A5B0, (void *, FontTexture_guess *, unsigned short *, unsigned int))(renderer, texture, fontName, fontHeight)) == 0 &&
                (allocationResult = EQ_CDECL(int, 0x1001564B, (void *, FontTexture_guess *))(renderer, texture)) == 0)
            {
                if (textureName != 0)
                {
                    allocationResult = EQ_CDECL(int, 0x1000DC08, (void *, FontTexture_guess *))(renderer, texture);
                    if (allocationResult < 0)
                    {
                        EQ_CDECL(int, 0x10015D81, (void *, FontTexture_guess *))(renderer, texture);
                        goto cleanup;
                    }
                }
                EQ_CDECL(void, 0x10017CE8, (void *))(*(void **)texture->bitmapInfo);
                return texture;
            }
        }
    }
cleanup:
    if (texture->dictionaryKey != 0)
        EQGFX_FREE_MEMORY(texture->dictionaryKey);
    if (texture->buffer20 != 0)
        EQGFX_FREE_MEMORY(texture->buffer20);
    if (texture->buffer24 != 0)
        EQGFX_FREE_MEMORY(texture->buffer24);
    if (texture->buffer28 != 0)
        EQGFX_FREE_MEMORY(texture->buffer28);
    EQGFX_FREE_MEMORY(texture);
    return (FontTexture_guess *)0;
}

// FUNCTION: EQGFX 0x1001a5b0
// Draws the glyphs into a 256-wide texture (black made transparent); stores each glyph's UV box and width.
int __cdecl s3dCreateFontTextureBMInfo(void *renderer, FontTexture_guess *fontTexture, unsigned short *fontName, unsigned int fontHeight)
{
    FontTexture_guess *texture;
    FontLogFont_guess fontSpec;
    FontCharABC_guess *allocatedABC;
    FontCharABC_guess *abcWidths;
    FontBitmapInfo_guess *bitmapInfo;
    FontBitmapSurface_guess *surface;
    FontBitmapDevice_guess *device;
    FontSurfaceLockDescription_guess lockDescription;
    FontRect_guess rectangle;
    void *deviceContext;
    void *font;
    void *brush;
    unsigned int glyphCount;
    unsigned int characterCode;
    unsigned int row;
    unsigned int column;
    unsigned int lineTop;
    unsigned int lineX;
    unsigned int glyphWidth;
    unsigned int bitmapHeight;
    int *glyphAdvances;
    float *firstCoordinates;
    float *secondCoordinates;
    unsigned short *rowPixels;
    unsigned short *rowStartPixels;
    char textureName[256];
    const char *characterString;
    int characterLength;
    int x;
    int y;
    int result;
    float inverseHeight;

    texture = fontTexture;
    glyphCount = texture->maxChar - texture->minChar + 1;
    deviceContext = EQGFX_CREATE_COMPATIBLE_DC(0);
    if (deviceContext == 0)
        return SPRITE_DEFINITION_INVALID;

    EQGFX_MEMSET(&fontSpec, 0, 0x3c);
    fontSpec.height = fontHeight;
    fontSpec.weight = 800;
    fontSpec.quality = 2;
    EQGFX_COPY_STRING(fontSpec.faceName, (const char *)fontName);
    font = EQGFX_CREATE_FONT_INDIRECT_A(&fontSpec);
    if (font == 0)
    {
        EQGFX_DELETE_DC(deviceContext);
        return SPRITE_DEFINITION_INVALID;
    }

    if (EQGFX_SELECT_OBJECT(deviceContext, font) == 0)
    {
        EQGFX_DELETE_DC(deviceContext);
        EQGFX_DELETE_OBJECT(font);
        return SPRITE_DEFINITION_INVALID;
    }

    allocatedABC = (FontCharABC_guess *)EQGFX_ALLOCATE_MEMORY(glyphCount * 0x0c);
    if (allocatedABC != 0)
    {
        result = EQGFX_GET_CHAR_ABC_WIDTHS_A(deviceContext, texture->minChar, texture->maxChar, allocatedABC);
        if (result != 0)
        {
            lineTop = 0;
            firstCoordinates = (float *)texture->buffer24;
            secondCoordinates = (float *)texture->buffer28;
            characterCode = texture->minChar;
            lineX = 0x100;
            glyphAdvances = (int *)texture->buffer20;
            abcWidths = allocatedABC;
            if (characterCode < texture->maxChar)
            {
                do
                {
                    glyphWidth = abcWidths->abcC + abcWidths->abcB + 2 + abcWidths->abcA;
                    lineX = lineX + glyphWidth;
                    if (0x100 < lineX)
                    {
                        lineTop = lineTop + 2 + fontHeight;
                        lineX = glyphWidth + 2;
                    }

                    firstCoordinates[0] = (float)(lineX - glyphWidth);
                    firstCoordinates[1] = (float)((lineTop - 2) - fontHeight);
                    secondCoordinates[0] = (float)(lineX - 2);
                    secondCoordinates[1] = (float)(lineTop - 2);
                    *glyphAdvances = glyphWidth - 2;
                    firstCoordinates += 2;
                    secondCoordinates += 2;
                    abcWidths++;
                    characterCode++;
                    glyphAdvances++;
                }
                while (characterCode < texture->maxChar);
            }

            bitmapHeight = 1;
            for (lineTop = lineTop + 1; lineTop != 0; lineTop = lineTop >> 1)
                bitmapHeight = bitmapHeight << 1;

            EQGFX_COPY_STRING(textureName, (const char *)fontName);
            EQGFX_STR_CAT(textureName, EQGFX_FONT_SUFFIX);
            bitmapInfo = EQGFX_DEFINE_BITMAP_INFO(renderer, textureName, 0x100, bitmapHeight);
            texture->bitmapInfo = bitmapInfo;
            abcWidths = allocatedABC;
            if (bitmapInfo != 0)
            {
                surface = bitmapInfo->surface;
                EQGFX_DELETE_DC(deviceContext);
                if ((g_render_mode_guess == 1 && EQGFX_VALIDATE_SURFACE(surface) != 0)
                    || surface->device->vtable->getDC(surface->device, &deviceContext) != 0
                    || EQGFX_SELECT_OBJECT(deviceContext, font) == 0)
                {
                    EQGFX_FREE_MEMORY(allocatedABC);
                }
                else
                {
                    fontHeight = (unsigned int)texture->buffer24;
                    fontTexture = (FontTexture_guess *)((unsigned int)fontTexture & 0x00ffffff);
                    EQGFX_SET_TEXT_COLOR(deviceContext, 0x00ffffff);
                    EQGFX_SET_BK_MODE(deviceContext, 2);
                    EQGFX_SET_BK_COLOR(deviceContext, 0);
                    brush = EQGFX_CREATE_SOLID_BRUSH(0);
                    EQGFX_SELECT_OBJECT(deviceContext, brush);
                    rectangle.left = 0;
                    rectangle.top = 0;
                    rectangle.right = surface->width;
                    rectangle.bottom = surface->height;
                    EQGFX_FILL_RECT(deviceContext, &rectangle, brush);
                    EQGFX_DELETE_OBJECT(brush);

                    for (characterCode = texture->minChar; characterCode < texture->maxChar; characterCode++)
                    {
                        characterString = (const char *)&fontTexture + 2;
                        characterLength = 1;
                        ((unsigned char *)&fontTexture)[2] = (unsigned char)characterCode;
                        __asm mov eax, fontHeight
                        __asm fld dword ptr [eax + 4]
                        y = EQGFX_FLOAT_TO_INT();
                        __asm mov eax, fontHeight
                        __asm fld dword ptr [eax]
                        x = EQGFX_FLOAT_TO_INT();
                        result = EQGFX_TEXT_OUT_A(deviceContext, x, y, characterString, characterLength);
                        if (result == 0)
                        {
                            device = surface->device;
                            device->vtable->releaseDC(device, deviceContext);
                            goto release_bitmap_memory;
                        }
                        fontHeight = fontHeight + 8;
                    }

                    result = surface->device->vtable->releaseDC(surface->device, deviceContext);
                    if (result == 0)
                    {
                        EQGFX_MEMSET(&lockDescription, 0, 0x7c);
                        result = EQGFX_LOCK_SURFACE(surface, &lockDescription);
                        if (result == 0)
                        {
                            row = 0;
                            if (surface->height != 0)
                            {
                                do
                                {
                                    rowStartPixels = lockDescription.pixels;
                                    rowPixels = rowStartPixels;
                                    column = 0;
                                    if (surface->width != 0)
                                    {
                                        do
                                        {
                                            if (*rowPixels == 0)
                                                *rowPixels = 0x7fff;
                                            else
                                                *rowPixels = *rowPixels | 0x8000;
                                            rowPixels++;
                                            column++;
                                        }
                                        while (column < surface->width);
                                    }
                                    row++;
                                    lockDescription.pixels = rowStartPixels + (lockDescription.pitch >> 1);
                                }
                                while (row < surface->height);
                            }

                            EQGFX_UNLOCK_SURFACE(surface);
                            inverseHeight = EQGFX_ONE / (float)surface->height;
                            firstCoordinates = (float *)texture->buffer24;
                            secondCoordinates = (float *)texture->buffer28;
                            for (characterCode = texture->minChar; characterCode < texture->maxChar; characterCode++)
                            {
                                *firstCoordinates = (EQGFX_ONE / (float)surface->width) * *firstCoordinates;
                                firstCoordinates[1] = inverseHeight * firstCoordinates[1];
                                *secondCoordinates = (EQGFX_ONE / (float)surface->width) * *secondCoordinates;
                                secondCoordinates[1] = inverseHeight * secondCoordinates[1];
                                firstCoordinates += 2;
                                secondCoordinates += 2;
                            }
                            EQGFX_FREE_MEMORY(allocatedABC);
                            EQGFX_DELETE_OBJECT(font);
                            if (g_render_mode_guess == 2)
                            {
                                result = EQGFX_CHECK_SURFACE(surface);
                                if (result != 0)
                                    return SPRITE_DEFINITION_INVALID;
                                EQGFX_FINALIZE_SURFACE(surface);
                            }
                            return 0;
                        }
                    }
release_bitmap_memory:
                    EQGFX_FREE_MEMORY(allocatedABC);
                }
                EQGFX_DELETE_OBJECT(font);
                return SPRITE_DEFINITION_INVALID;
            }
        }
        EQGFX_FREE_MEMORY(allocatedABC);
    }
    EQGFX_DELETE_OBJECT(font);
    EQGFX_DELETE_DC(deviceContext);
    return SPRITE_DEFINITION_INVALID;
}

// FUNCTION: EQGFX 0x1001a9ec
// Checks that every character in text falls within the font's supported character range.
int __cdecl s3dCheckFontTextureCharRange(FontRange_guess *range, char *text)
{
    unsigned char lowChar;
    unsigned char highChar;
    char curChar;

    lowChar = range->minChar;
    highChar = range->maxChar;
    curChar = *text;
    if (curChar != 0)
    {
        do
        {
            if (curChar < lowChar || *text > highChar)
                return 0xffff3d3e;
            curChar = text[1];
            text++;
        } while (curChar != 0);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001aa2a
// __cdecl Measures the string at +0x18 with the font at +0x14. Returns 0.
int __cdecl s3dCalcStringSpriteWidth_guess(StringSprite_guess *sprite)
{
    FontTable_guess *font;
    char *text;
    int base;
    int stored;

    font = sprite->font;
    text = sprite->text;
    stored = font->value1c;
    base = font->baseChar;
    sprite->totalWidth = 0;
    sprite->value40 = stored;
    while (*text != 0)
    {
        sprite->totalWidth += font->widths[*text - base];
        text++;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001aa66
// Releases a font texture and its bitmap resources when it has no users.
int __cdecl s3dDestroyFontTexture(void *dictionary, FontTexture_guess *texture)
{
    int result;

    if (texture->useCount != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        if (texture->buffer20 != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(texture->buffer20);
        if (texture->buffer24 != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(texture->buffer24);
        if (texture->buffer28 != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(texture->buffer28);
        if (texture->bitmapInfo != 0)
        {
            EQ_CDECL(void, 0x1000BE1B, (void *))(texture->bitmapInfo);
            EQ_CDECL(void, 0x1000BCF0, (void *, void *))(dictionary, texture->bitmapInfo);
        }
        if ((texture->flags & 1) != 0)
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, texture->dictionaryKey);
        result = EQ_CDECL(int, 0x10015D81, (void *, FontTexture_guess *))(dictionary, texture);
        if (texture->dictionaryKey != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(texture->dictionaryKey);
        EQ_CDECL(void, 0x10050CBB, (void *))(texture);
    }
    return result;
}

// FUNCTION: EQGFX 0x1001aafb
// __cdecl. Returns the new string sprite, or 0 after freeing the partial one when a check or a step fails.
StringSprite_guess *__cdecl s3dCreateStringSprite(void *dictionary, char *key, FontTable_guess *font,
    char *text, int value20)
{
    StringSprite_guess *sprite;
    size_t length;
    char *textCopy;
    char *keyCopy;
    unsigned int defaultScaleBits;

    if (text == 0)
        return 0;
    if (EQ_CDECL(int, 0x1001A9EC, (FontTable_guess *, char *))(font, text) != 0)
        return 0;
    sprite = (StringSprite_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x4C);
    if (sprite == 0)
        return 0;
    ((unsigned int *)sprite)[0] = 0x51;
    ((unsigned int *)sprite)[1] = 0;
    sprite->dictionaryKey = 0;
    ((unsigned int *)sprite)[3] = 0;
    ((unsigned int *)sprite)[4] = 0;
    sprite->font = 0;
    defaultScaleBits = EQGFX_DEFAULT_SPRITE_SCALE_BITS;
    ((unsigned char *)&sprite->tint)[2] |= 0xff;
    ((unsigned char *)&sprite->tint)[0] |= 0xff;
    ((unsigned char *)&sprite->tint)[1] |= 0xff;
    *(unsigned int *)&sprite->scale38 = defaultScaleBits;
    sprite->text = 0;
    ((unsigned int *)sprite)[8] = value20;
    ((unsigned int *)sprite)[7] = 0;
    length = EQ_CDECL(size_t, 0x10068160, (char *))(text);
    textCopy = (char *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(length + 1);
    sprite->text = textCopy;
    if (textCopy != 0)
    {
        EQ_CDECL(void, 0x10068070, (char *, char *))(textCopy, text);
        ((unsigned int *)sprite)[7] = EQ_CDECL(size_t, 0x10068160, (char *))(text);
        sprite->font = font;
        if (EQ_CDECL(int, 0x1001AC3A, (StringSprite_guess *))(sprite) == 0
            && EQ_CDECL(int, 0x1001AA2A, (StringSprite_guess *))(sprite) == 0)
        {
            if (key != 0)
            {
                length = EQ_CDECL(size_t, 0x10068160, (char *))(key);
                keyCopy = (char *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(length + 1);
                sprite->dictionaryKey = keyCopy;
                if (keyCopy == 0)
                    goto freeSprite;
                EQ_CDECL(void, 0x10068070, (char *, char *))(keyCopy, key);
            }
            if (EQ_CDECL(int, 0x1001564B, (void *, StringSprite_guess *))(dictionary, sprite) == 0)
            {
                if (key == 0 || EQ_CDECL(int, 0x1000DC08, (void *, StringSprite_guess *))(dictionary, sprite) >= 0)
                {
                    EQ_CDECL(void, 0x10017CE8, (void *))(sprite->font);
                    return sprite;
                }
                EQ_CDECL(int, 0x10015D81, (void *, StringSprite_guess *))(dictionary, sprite);
            }
        }
    }
freeSprite:
    if (sprite->text != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->text);
    if (sprite->dictionaryKey != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->dictionaryKey);
    EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
    return 0;
}

// FUNCTION: EQGFX 0x1001ac3a
// Computes the scaled half-diagonal of a string sprite's text bounds.
int __cdecl s3dCalcStringSpriteHalfDiagonal_guess(StringSprite_guess *sprite)
{
    FontTable_guess *font;
    char *text;
    char character;
    int fontHeight;
    int widthSum;

    widthSum = 0;
    font = sprite->font;
    fontHeight = font->value1c;
    text = sprite->text;
    character = *text;
    while (character != 0)
    {
        widthSum += font->widths[character - font->baseChar];
        character = text[1];
        text++;
    }
    sprite->halfDiagonal44 = EQ_CDECL(float, 0x1004EF68, (float))
        ((float)(unsigned int)(widthSum * widthSum + fontHeight * fontHeight))
        * sprite->scale38 * EQGFX_HALF_SCALE;
    return 0;
}

// FUNCTION: EQGFX 0x1001aca3
// __cdecl Sets the max scale at +0x28 and its reciprocal at +0x2c. Returns 0.
int __cdecl s3dSetStringSpriteMaxScaleFactor(StringSprite_guess *sprite, int scaleFlag, float maxScale)
{
    sprite->maxScaleFlag24 = scaleFlag;
    sprite->maxScaleFactor28 = maxScale;
    if (maxScale != 0.0f)
        sprite->invMaxScaleFactor2c = 1.0f / maxScale;
    else
        sprite->invMaxScaleFactor2c = 0.0f;
    return 0;
}

// FUNCTION: EQGFX 0x1001acd3
// __cdecl Sets the yon clip values at +0x30 and +0x34. Returns 0.
int __cdecl s3dSetStringSpriteYonClip(StringSprite_guess *sprite, int yonFirst, int yonSecond)
{
    sprite->yonClip30 = yonFirst;
    sprite->yonClip34 = yonSecond;
    return 0;
}

// FUNCTION: EQGFX 0x1001ace8
// __cdecl Sets the tint at +0x48 from *tintSrc. Returns 0.
int __cdecl s3dSetStringSpriteTint(StringSprite_guess *sprite, int *tintSrc)
{
    sprite->tint = *tintSrc;
    return 0;
}

// FUNCTION: EQGFX 0x1001acf8
// Releases a string sprite and its shared font resources.
int __cdecl s3dDestroyStringSprite(void *dictionary, StringSprite_guess *sprite)
{
    int result;

    result = 0;
    if (sprite->text != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->text);
    }
    EQ_CDECL(void, 0x10017DAD, (void *))(sprite->font);
    if ((sprite->flags & 1) != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, sprite->dictionaryKey);
    }
    if (EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, sprite) != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        if (sprite->dictionaryKey != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->dictionaryKey);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
    }
    return result;
}

// FUNCTION: EQGFX 0x1001AD5C
// Defines a reusable named group of spheres from their centers and radii, or reuses an existing definition.
SphereListDefinition_guess *__cdecl t3dDefineSphereList(void *dictionary, char *key, int definitionValue,
    int count, void *centerData, void *radiusData, int entryValue)
{
    SphereListDefinition_guess *definition;
    void *allocatedMemory;

    definition = (SphereListDefinition_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, char *))(dictionary, key);
    if (definition == 0)
    {
        definition = (SphereListDefinition_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x28);
        if (definition == 0)
        {
            return 0;
        }
        *((int *)&definition->flags) &= 0;
        definition->type = 3;
        definition->useCount = 1;
        definition->definitionValue = definitionValue;
        definition->count = count;
        definition->dictionaryKey = EQ_CDECL(void *, 0x10050C48, (unsigned int))(
            (unsigned int)(EQ_CDECL(size_t, 0x10068160, (char *))(key) + 1));
        if (definition->dictionaryKey == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(definition);
        }
        else
        {
            EQ_CDECL(void, 0x10068070, (void *, char *))(definition->dictionaryKey, key);
            if (EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, definition) == 0)
            {
                if (EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, definition) < 0)
                {
                    EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, definition);
                    EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
                    EQ_CDECL(void, 0x10050CBB, (void *))(definition);
                }
                else
                {
                    allocatedMemory = EQ_CDECL(void *, 0x10050C48, (unsigned int))(count * 0x0c);
                    if (allocatedMemory == 0)
                    {
                        EQ_CDECL(void, 0x1000E334, (void *, char *))(dictionary, key);
                        EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, definition);
                        EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
                        EQ_CDECL(void, 0x10050CBB, (void *))(definition);
                    }
                    else
                    {
                        definition->centerList = allocatedMemory;
                        EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(
                            definition->centerList, centerData, count * 0x0c);
                        allocatedMemory = EQ_CDECL(void *, 0x10050C48, (unsigned int))(count << 2);
                        if (allocatedMemory == 0)
                        {
                            EQ_CDECL(void, 0x1000E334, (void *, char *))(dictionary, key);
                            EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, definition);
                            EQ_CDECL(void, 0x10050CBB, (void *))(definition->centerList);
                            EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
                            EQ_CDECL(void, 0x10050CBB, (void *))(definition);
                        }
                        else
                        {
                            definition->radiusList = allocatedMemory;
                            EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(
                                definition->radiusList, radiusData, count << 2);
                            allocatedMemory = EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x1c);
                            if (allocatedMemory == 0)
                            {
                                EQ_CDECL(void, 0x1000E334, (void *, char *))(dictionary, key);
                                EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, definition);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition->radiusList);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition->centerList);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition);
                            }
                            else
                            {
                                SphereListChildEntry_guess *childEntry;

                                definition->childEntry = allocatedMemory;
                                childEntry = (SphereListChildEntry_guess *)definition->childEntry;
                                childEntry->type = 4;
                                childEntry->field08 = 0;
                                childEntry->field0c = 0;
                                childEntry->field10 = 0;
                                childEntry->entryValue = entryValue;
                                childEntry->definition = definition;
                                if (EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, childEntry) >= 0)
                                {
                                    return definition;
                                }
                                EQ_CDECL(void, 0x10050CBB, (void *))(childEntry);
                                EQ_CDECL(void, 0x1000E334, (void *, char *))(dictionary, key);
                                EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, definition);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition->radiusList);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition->centerList);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
                                EQ_CDECL(void, 0x10050CBB, (void *))(definition);
                            }
                        }
                    }
                }
            }
            else
            {
                EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
                EQ_CDECL(void, 0x10050CBB, (void *))(definition);
            }
        }
    }
    else if (definition->type == 3)
    {
        return definition;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001af77
// Removes a sphere-list object and frees its sphere centers, radii, and definition.
int __cdecl t3dUndefineSphereList(void *dictionary, SphereListInstance_guess *instance)
{
    int result;

    if (instance->state != 1)
    {
        result = 0xffff3d3e;
    }
    else
    {
        SphereListDefinition_guess *definition;

        definition = instance->definition;
        if ((definition->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, definition->dictionaryKey);
        }
        EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, definition);
        if (definition->dictionaryKey != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(definition);
        EQ_CDECL(void, 0x10050CBB, (void *))(instance->centerList);
        EQ_CDECL(void, 0x10050CBB, (void *))(instance->radiusList);
        if ((instance->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, instance->dictionaryKey);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, instance);
        if (instance->dictionaryKey != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(instance->dictionaryKey);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(instance);
    }
    return result;
}

// FUNCTION: EQGFX 0x1001b00a
// __cdecl Copies the center list at +0x1c (count at +0x18 vec3s). Returns 0.
int __cdecl t3dGetSphereListDefinitionCenterList(SphereListDefinition_guess *definition, void *centerOut)
{
    EQ_CDECL(void, 0x10067D30, (void *, const void *, unsigned int))(centerOut, definition->centerList, definition->count * 12);
    return 0;
}

// FUNCTION: EQGFX 0x1001b02a
// __cdecl Copies the radius list at +0x20 (count at +0x18 floats). Returns 0.
int __cdecl t3dGetSphereListDefinitionRadiusList(SphereListDefinition_guess *definition, void *radiusOut)
{
    EQ_CDECL(void, 0x10067D30, (void *, const void *, unsigned int))(radiusOut, definition->radiusList, definition->count << 2);
    return 0;
}

// FUNCTION: EQGFX 0x1001B047
// Creates an instance of a sphere-list definition for an object.
// __cdecl. Copies the prototype at +0x24 of the create info, registers the copy; returns it, or 0 (copy freed) on failure.
SphereListObject_guess *__cdecl t3dCreateSphereList(void *dictionary, SphereListCreateInfo_guess *createInfo)
{
    SphereListObject_guess *object;

    object = (SphereListObject_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(SphereListObject_guess));
    if (object != 0)
    {
        *object = *createInfo->prototype;
        object->dictionaryKey = 0;
        object->flags = 0;
        object->scaleData = 0;
        if (EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, object) != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(object);
            object = 0;
        }
        else
        {
            object->definition->useCount++;
        }
    }
    return object;
}

// FUNCTION: EQGFX 0x1001b098
// Gets the tag used to identify a 2D sprite definition.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Copies the tag at +0x8
// into tagOut, empty string when NULL. Returns 0.
int __cdecl t3dGet2DSpriteDefinitionTag(char *definition, char *tagOut)
{
    char *tag;

    tag = *(char **)(definition + 8);
    if (tag == 0)
    {
        *tagOut = 0;
    }
    else
    {
        EQ_CDECL(void, 0x10068070, (char *, const char *))(tagOut, tag);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001b0bb
// Sets the bounding radius stored with a polyhedron definition.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Stores the radius at +0x14
// of the polyhedron definition. Returns 0.
int __cdecl t3dSetPolyhedronDefinitionBoundingRadius(char *polyhedronDef, float radius)
{
    *(float *)(polyhedronDef + 0x14) = radius;
    return 0;
}

// FUNCTION: EQGFX 0x1001B0C9
// Gets the world-space position of a sphere in an object's sphere list.
int __cdecl t3dGetSphereListWorldXYZ(SphereListObject_guess *object, unsigned int index, float *worldXYZOut)
{
    SphereListDefinition_guess *definition;
    SphereWorldScale_guess *scaleData;
    float *center;
    float worldScale;
    float rowSum;
    int result;

    definition = object->definition;
    if (index < (unsigned int)definition->count && (scaleData = object->scaleData) != 0)
    {
        result = EQ_CDECL(int, 0x10035058, (void *, float, float *))(
            scaleData, scaleData->worldScale, scaleData->worldPosition);
        if (result >= 0)
        {
            center = (float *)definition->centerList + index * 3;
            worldScale = object->localScale * scaleData->worldScale;
            worldXYZOut[0] = (scaleData->worldMatrix[6] * center[2] +
                scaleData->worldMatrix[3] * center[1] + center[0] * scaleData->worldMatrix[0]) * worldScale +
                scaleData->worldPosition[0];
            rowSum = scaleData->worldMatrix[1] * center[0] +
                scaleData->worldMatrix[7] * center[2];
            rowSum = rowSum + scaleData->worldMatrix[4] * center[1];
            worldXYZOut[1] = rowSum * worldScale + scaleData->worldPosition[1];
            rowSum = scaleData->worldMatrix[2] * center[0] +
                scaleData->worldMatrix[8] * center[2];
            rowSum = rowSum + scaleData->worldMatrix[5] * center[1];
            worldXYZOut[2] = rowSum * worldScale + scaleData->worldPosition[2];
            return result;
        }
    }
    return SPRITE_DEFINITION_INVALID;
}

// FUNCTION: EQGFX 0x1001b184
// __cdecl Scales the definition radius at the index by the object scales.
int __cdecl t3dGetSphereListWorldRadius(SphereListObject_guess *object, unsigned int index, float *radiusOut)
{
    SphereListDefinition_guess *definition;

    definition = object->definition;
    if (index >= (unsigned int)definition->count || object->scaleData == 0)
        return 0xFFFF3D3E;
    *radiusOut = object->scaleData->worldScale * object->localScale * ((float *)definition->radiusList)[index];
    return 0;
}

// FUNCTION: EQGFX 0x1001b1b9
// Gets the definition-space center of one sphere in an object's sphere list.
// __cdecl Copies the center at the index into posOut.
// Returns 0xFFFF3D3E when the index is out of range, 0 on success.
int __cdecl t3dGetSphereListObjectXYZ(SphereListObject_guess *object, unsigned int index, float *posOut)
{
    SphereListDefinition_guess *definition;
    float *center;

    definition = object->definition;
    if (index >= (unsigned int)definition->count)
        return 0xFFFF3D3E;
    center = (float *)definition->centerList + index * 3;
    posOut[0] = center[0];
    posOut[1] = center[1];
    posOut[2] = center[2];
    return 0;
}

// FUNCTION: EQGFX 0x1001b1e6
// Gets the radius assigned to one sphere in an object's sphere list.
// __cdecl Copies the radius at the index into radiusOut.
// Returns 0xFFFF3D3E when the index is out of range, 0 on success.
int __cdecl t3dGetSphereListObjectRadius(SphereListObject_guess *object, unsigned int index, float *radiusOut)
{
    SphereListDefinition_guess *definition;

    definition = object->definition;
    if (index >= (unsigned int)definition->count)
        return 0xFFFF3D3E;
    *radiusOut = ((float *)definition->radiusList)[index];
    return 0;
}

// FUNCTION: EQGFX 0x1001b20b
// Retrieves the sprite's collision-volume reference; the volume's shape is not established here.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Copies the
// collision volume for the sprite type into volumeOut.
int __cdecl t3dGetSpriteCollisionVolume(char *spriteDef, unsigned int *volumeOut)
{
    unsigned int spriteType;
    unsigned int volume;

    spriteType = *(unsigned int *)spriteDef;
    if (spriteType <= 0x12)
    {
        if (spriteType == 0x12)
        {
            volume = *(unsigned int *)(spriteDef + 0x14);
        }
        else
        {
            if (spriteType == 0 || spriteType == 8)
            {
                *volumeOut = 0;
                return 0;
            }
            if (spriteType == 10)
            {
                volume = *(unsigned int *)(spriteDef + 0x14);
            }
            else
            {
                if (spriteType != 0xc)
                {
                    *volumeOut = 0;
                    return 0xFFFF3D3E;
                }
                volume = *(unsigned int *)(spriteDef + 0x14);
            }
        }
    }
    else if (spriteType == 0x14)
    {
        volume = *(unsigned int *)(spriteDef + 0x14);
    }
    else
    {
        if (spriteType != 0x48)
        {
            if (spriteType != 0x4c && spriteType != 0x51)
            {
                *volumeOut = 0;
                return 0xFFFF3D3E;
            }
            *volumeOut = 0;
            return 0;
        }
        volume = *(unsigned int *)(spriteDef + 0x2c);
    }
    *volumeOut = volume;
    return 0;
}

// FUNCTION: EQGFX 0x1001b453
// Retrieves the shared definition associated with a sprite instance.
int __cdecl t3dGetSpriteDefinition(SpriteDef_guess *spriteDef, unsigned int *definitionOut)
{
    unsigned int spriteType;
    unsigned int definition;

    spriteType = spriteDef->spriteType;
    if (spriteType <= 0x12)
    {
        if (spriteType == 0x12)
        {
            definition = spriteDef->centerData.type12Definition.definition;
        }
        else
        {
            if (spriteType == 0 || spriteType == 8)
            {
                *definitionOut = 0;
                return 0;
            }
            if (spriteType == 10)
            {
                definition = spriteDef->centerData.type10Definition.definition;
            }
            else
            {
                if (spriteType != 0xc)
                {
                    *definitionOut = 0;
                    return SPRITE_DEFINITION_INVALID;
                }
                definition = spriteDef->centerData.type0cDefinition.definition;
            }
        }
    }
    else if (spriteType == 0x14)
    {
        definition = spriteDef->centerData.type14Definition.definition;
    }
    else
    {
        if (spriteType != 0x48)
        {
            if (spriteType != 0x4c && spriteType != 0x51)
            {
                *definitionOut = 0;
                return SPRITE_DEFINITION_INVALID;
            }
            *definitionOut = 0;
            return 0;
        }
        definition = spriteDef->centerData.type48Definition.definition;
    }
    *definitionOut = definition;
    return 0;
}

// FUNCTION: EQGFX 0x1001B36E
// __cdecl. Position plus the center offset times the world matrix; a failed or all-zero offset copies the position.
int __cdecl t3dGetSpritePosition(SpriteWorldScale_guess *sprite, SpriteDef_guess *spriteDef, float *positionOut)
{
    float centerOffset[3];
    float *spritePosition;
    int result;

    spritePosition = sprite->worldPosition;
    result = EQ_CDECL(int, 0x1001B263, (SpriteDef_guess *, float *))(spriteDef, centerOffset);
    if (result == 0 && (centerOffset[0] != 0.0f || centerOffset[1] != 0.0f || centerOffset[2] != 0.0f))
    {
        EQ_CDECL(int, 0x10035058, (void *, float, float *))(sprite, sprite->worldScale, spritePosition);
        positionOut[0] = centerOffset[2] * sprite->worldMatrix[6] + centerOffset[1] * sprite->worldMatrix[3] +
            centerOffset[0] * sprite->worldMatrix[0] + spritePosition[0];
        positionOut[1] = centerOffset[2] * sprite->worldMatrix[7] + centerOffset[1] * sprite->worldMatrix[4] +
            centerOffset[0] * sprite->worldMatrix[1] + spritePosition[1];
        positionOut[2] = centerOffset[2] * sprite->worldMatrix[8] + centerOffset[1] * sprite->worldMatrix[5] +
            centerOffset[0] * sprite->worldMatrix[2] + spritePosition[2];
    }
    else
    {
        *(Vector3_guess *)positionOut = *(Vector3_guess *)spritePosition;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001b2e7
// Retrieves the sprite definition's bounding radius without applying instance scale.
int __cdecl t3dGetSpriteBoundingRadius(SpriteDef_guess *spriteDef, float *radiusOut)
{
    unsigned int spriteType;
    float radius;
    SpriteType48PrimaryRadiusData_guess *primaryRadiusData;

    spriteType = spriteDef->spriteType;
    if (spriteType < 0x13)
    {
        if (spriteType == 0x12)
        {
            radius = spriteDef->centerData.type12Radius.radiusData->radius;
            goto writeRadius;
        }
        if (spriteType != 0 && spriteType != 8)
        {
            if (spriteType == 10)
            {
                radius = spriteDef->centerData.type10Radius.radiusData->radius;
            }
            else
            {
                if (spriteType != 0xc)
                    goto invalidSpriteType;
                radius = spriteDef->centerData.type0cRadius.radiusData->radius;
            }
            goto writeRadius;
        }
    }
    else
    {
        if (spriteType == 0x14)
        {
            radius = spriteDef->centerData.type14Radius.radiusData->radius;
            goto writeRadius;
        }
        if (spriteType == 0x48)
        {
            primaryRadiusData = spriteDef->centerData.type48Radius.radiusData->primary;
            if (primaryRadiusData == 0)
            {
                radius = spriteDef->centerData.type48Radius.radiusData->fallback->radius;
            }
            else
            {
                radius = primaryRadiusData->radius;
            }
            goto writeRadius;
        }
        if (spriteType != 0x4c && spriteType != 0x51)
            goto invalidSpriteType;
    }
    radius = 0.0f;
writeRadius:
    *radiusOut = radius;
    return 0;

invalidSpriteType:
    *radiusOut = 0;
    return SPRITE_DEFINITION_INVALID;
}

// FUNCTION: EQGFX 0x1001b263
// Retrieves the sprite's local center offset used to determine its world-space center.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Copies the 3-float
// center offset for the sprite type into centerOut. Types 0, 8, 0x4c and 0x51
// have a zero center (returns 0); any other unknown type zeroes centerOut and
// returns 0xFFFF3D3E. Returns 0 on success.
// @early-stop: remaining low-type switch dispatch and branch layout differ.
/* fake match: retain the original EBP frame and emit its three MOVSD copies. */
#pragma optimize("y", off)
#pragma intrinsic(memcpy)
int __cdecl t3dGetSpriteCenterOffset(SpriteDef_guess *spriteDef, float *centerOut)
{
    int result = 0;
    unsigned int spriteType;

    spriteType = spriteDef->spriteType;
    if (spriteType <= 0x12)
    {
        switch (spriteType)
        {
        case 0:
        case 8:
            centerOut[0] = 0.0f;
            centerOut[1] = 0.0f;
            centerOut[2] = 0.0f;
            break;
        case 0xc:
            memcpy(centerOut, (float *)((char *)spriteDef + 0x1c), 0x0c);
            break;
        case 10:
            memcpy(centerOut, (float *)((char *)spriteDef + 0x78), 0x0c);
            break;
        case 0x12:
            memcpy(centerOut, (float *)((char *)spriteDef + 0x2c), 0x0c);
            break;
        default:
            centerOut[0] = 0.0f;
            centerOut[1] = 0.0f;
            centerOut[2] = 0.0f;
            result = 0xFFFF3D3E;
            break;
        }
    }
    else
    {
        switch (spriteType)
        {
        case 0x14:
            memcpy(centerOut, (float *)((char *)spriteDef + 0x28), 0x0c);
            break;
        case 0x48:
            memcpy(centerOut, (float *)((char *)spriteDef + 0x14), 0x0c);
            break;
        case 0x4c:
        case 0x51:
            centerOut[0] = 0.0f;
            centerOut[1] = 0.0f;
            centerOut[2] = 0.0f;
            break;
        default:
            centerOut[0] = 0.0f;
            centerOut[1] = 0.0f;
            centerOut[2] = 0.0f;
            result = 0xFFFF3D3E;
            break;
        }
    }
    return result;
}
#pragma function(memcpy)
#pragma optimize("y", on)

// FUNCTION: EQGFX 0x1001B4BD
// __cdecl. Calls the setter for the sprite's type (2D, hierarchical or DM); 0xFFFF3D3E for a type with none.
int __cdecl t3dSetSpriteCollisionVolume(int renderer, SpriteDef_guess *spriteDef, int sourceVolume)
{
    int result;

    switch (spriteDef->spriteType)
    {
    case 0:
    case 8:
        return SPRITE_DEFINITION_INVALID;
    case 10:
        result = EQ_CDECL(int, 0x1000DA80, (int, void *, int))(renderer, spriteDef, sourceVolume);
        break;
    case 12:
        result = EQ_CDECL(int, 0x1000DA80, (int, void *, int))(renderer, spriteDef, sourceVolume);
        break;
    case 0x12:
        result = EQ_CDECL(int, 0x1000DA80, (int, void *, int))(renderer, spriteDef, sourceVolume);
        break;
    case 0x14:
        result = EQ_CDECL(int, 0x10013625, (int, void *, int))(renderer, spriteDef, sourceVolume);
        break;
    case 0x48:
        result = EQ_CDECL(int, 0x1001070B, (int, void *, int))(renderer, spriteDef, sourceVolume);
        break;
    default:
        return SPRITE_DEFINITION_INVALID;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001b538
// Changes the sprite's local center offset for sprite types that support one.
int __cdecl t3dSetSpriteCenterOffset(SpriteDef_guess *spriteDef, unsigned int *centerSource)
{
    unsigned int spriteType;
    unsigned int *centerDest;

    spriteType = spriteDef->spriteType;
    if (spriteType < 0x13)
    {
        if (spriteType == 0x12)
        {
            centerDest = spriteDef->centerData.type12.center;
        }
        else
        {
            if (spriteType == 0 || spriteType == 8)
                return 0;
            if (spriteType == 10)
            {
                centerDest = spriteDef->centerData.type10.center;
            }
            else
            {
                if (spriteType != 0xc)
                    return 0xffff3d3e;
                centerDest = spriteDef->centerData.type0c.center;
            }
        }
    }
    else if (spriteType == 0x14)
    {
        centerDest = spriteDef->centerData.type14.center;
    }
    else
    {
        if (spriteType != 0x48)
        {
            if (spriteType == 0x4c || spriteType == 0x51)
                return 0;
            return 0xffff3d3e;
        }
        centerDest = spriteDef->centerData.type48.center;
    }
    centerDest[0] = centerSource[0];
    centerDest[1] = centerSource[1];
    centerDest[2] = centerSource[2];
    return 0;
}

// FUNCTION: EQGFX 0x1001b598
// Calculates the storage needed to define a simple sprite.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Returns the allocation
// size for a simple sprite definition: count * 4 + 0x4c, plus the name and its
int __cdecl s3dCalcSimpleSpriteDefAllocSize(char *spriteName, int spriteCount)
{
    int allocSize;

    allocSize = spriteCount * 4 + 0x4c;
    if (spriteName != 0)
        allocSize += strlen(spriteName) + 1;
    return allocSize;
}

#define T3D_ERROR_SPRITE_IN_USE 0xffff3d44

#define SIMPLE_SPRITE_DEFINITION_KIND 5
#define SIMPLE_SPRITE_INFO_KIND 6

// FUNCTION: EQGFX 0x1001b5bd
// Defines a reusable simple sprite from source object references and configuration, or reuses a named definition.
SimpleSpriteDefinition_guess *__cdecl t3dDefineSimpleSprite(int renderer,
    SimpleSpriteDefinition_guess *providedDefinition, char *spriteName, int spriteCount,
    void ***infoObjectReferences, int infoIndex, int infoConfiguration, int infoOptionFlags)
{
    SimpleSpriteDefinition_guess *definition;
    SimpleSpriteInfo_guess *info;
    char *allocationCursor;
    size_t nameLength;
    int infoBytes;
    int result;
    void ***objectCursor;
    void ***objectEnd;

    definition = EQ_CDECL(SimpleSpriteDefinition_guess *, 0x1000E7FE, (int, char *))(renderer, spriteName);
    if (definition != 0)
    {
        if (definition->kind != SIMPLE_SPRITE_DEFINITION_KIND)
            return 0;
        return definition;
    }
    definition = providedDefinition;
    if (providedDefinition == 0)
    {
        result = EQ_CDECL(int, 0x1001B598, (char *, int))(spriteName, spriteCount);
        definition = EQ_CDECL(SimpleSpriteDefinition_guess *, 0x10050C48, (int))(result);
        if (definition == 0)
            return 0;
    }
    definition->kind = SIMPLE_SPRITE_DEFINITION_KIND;
    definition->flags = 0;
    definition->useCount = 1;
    definition->infoCount = spriteCount;
    if (providedDefinition != 0)
        definition->flags |= 0x20;
    allocationCursor = (char *)(definition + 1);
    if (spriteName == 0)
    {
        definition->dictionaryKey = 0;
    }
    else
    {
        definition->dictionaryKey = allocationCursor;
        nameLength = EQ_CDECL(size_t, 0x10068160, (const char *))(spriteName);
        allocationCursor += nameLength + 1;
        EQ_CDECL(void, 0x10068070, (void *, const char *))(definition->dictionaryKey, spriteName);
    }
    definition->infoList = allocationCursor;
    infoBytes = spriteCount * sizeof(void *);
    info = (SimpleSpriteInfo_guess *)(allocationCursor + infoBytes);
    EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
        definition->infoList, infoObjectReferences, infoBytes);
    result = EQ_CDECL(int, 0x1001564B, (int, void *))(renderer, definition);
    if (result == 0)
    {
        result = EQ_CDECL(int, 0x1000DC08, (int, void *))(renderer, definition);
        if (result >= 0)
        {
            definition->info = info;
            info->kind = SIMPLE_SPRITE_INFO_KIND;
            info->dictionaryKey = 0;
            info->flags = 0;
            info->useCount = 1;
            info->sourceObject = infoObjectReferences[infoIndex];
            info->sourceIndex = infoIndex;
            info->configuration = infoConfiguration;
            info->reserved20 = 0;
            info->optionFlags = infoOptionFlags;
            info->definition = definition;
            result = EQ_CDECL(int, 0x1001564B, (int, void *))(renderer, info);
            if (result != 0)
            {
                EQ_CDECL(void, 0x1000E334, (int, char *))(renderer, spriteName);
                EQ_CDECL(void, 0x10015D81, (int, void *))(renderer, definition);
            }
            objectCursor = infoObjectReferences + spriteCount;
            objectEnd = infoObjectReferences;
            while (objectEnd < objectCursor)
            {
                EQ_CDECL(void, 0x10017CE8, (void *))(*objectCursor[-1]);
                objectCursor--;
            }
            return definition;
        }
        EQ_CDECL(void, 0x10015D81, (int, void *))(renderer, definition);
    }
    if (definition != 0 && providedDefinition == 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(definition);
    return 0;
}

// FUNCTION: EQGFX 0x1001b72e
// Removes an unused simple sprite and releases its referenced objects.
int __cdecl t3dUndefineSimpleSprite(int renderer, SimpleSpriteObject_guess *sprite)
{
    SimpleSpriteDefinition_guess *definition;
    void **objectCursor;
    int result;

    definition = sprite->definition;
    if (sprite->useCount == 1 && definition->useCount == 1)
    {
        if ((definition->flags & 1) != 0)
            EQ_CDECL(void, 0x1000E334, (int, void *))(renderer, definition->dictionaryKey);
        EQ_CDECL(int, 0x10015D81, (int, void *))(renderer, definition);
        if (definition->dictionaryKey != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
        if ((sprite->flags & 1) != 0)
            EQ_CDECL(void, 0x1000E334, (int, void *))(renderer, sprite->dictionaryKey);
        result = EQ_CDECL(int, 0x10015D81, (int, void *))(renderer, sprite);
        objectCursor = sprite->objects + sprite->objectCount;
        if (sprite->objects < objectCursor)
        {
            do
            {
                objectCursor--;
                EQ_CDECL(void, 0x10017DAD, (void *))(*(void **)*objectCursor);
            } while (sprite->objects < objectCursor);
        }
        if ((sprite->flags & 0x20) == 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
    }
    else
    {
        result = T3D_ERROR_SPRITE_IN_USE;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001b7d0
// __cdecl. Returns 1 when the sprite definition and its info carry the expected kinds.
int __cdecl t3dIsSimpleSpriteDefinitionValid(SimpleSpriteDefinition_guess *spriteDef)
{
    int isValid = 1;

    if (spriteDef == 0 || spriteDef->info == 0 || spriteDef->kind != 5 || spriteDef->info->kind != 6)
        isValid = 0;
    return isValid;
}

// FUNCTION: EQGFX 0x1001b7ef
// __cdecl. Copies the info list into dest; returns 0.
int __cdecl t3dGetSimpleSpriteDefinitionBMInfoList(SimpleSpriteDefinition_guess *spriteDef, void *dest)
{
    memcpy(dest, spriteDef->infoList, spriteDef->infoCount << 2);
    return 0;
}
