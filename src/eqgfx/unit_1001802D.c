// unit_1001802D.c - eqgfx_dx7.dll, the address run 0x1001802D..0x10019355 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// eqgfx_dx7.dll's own globals used here (VAs of this image, not trilogy's).
#define g_t3d_syscolors_saved_guess   EQ_GLOBAL(int, 0x100AADCC)            // g_t3d_syscolors_saved_guess: nonzero while the system colours are changed
#define g_t3d_syscolors_rgb_guess     EQ_ARRAY(unsigned int, 0x100AAD80)    // g_t3d_syscolors_rgb_guess: the 0x13 COLORREFs put back
#define g_t3d_syscolors_index_guess   EQ_ARRAY(int, 0x10086D0C)             // g_t3d_syscolors_index_guess: the 0x13 system colour indices
#define g_t3d_syscolors_index_end_guess     EQ_ARRAY(int, 0x10086D58)             // g_t3d_syscolors_index_end_guess: one past the 0x13 indices
#define g_t3d_syscolors_replace_rgb_guess   EQ_ARRAY(unsigned int, 0x10086D58)   // g_t3d_syscolors_index_end_guess: the 0x13 COLORREFs the colours are set to
#define g_t3d_syscolors_captured_guess      EQ_GLOBAL(int, 0x100AADD0)            // g_t3d_syscolors_captured_guess: nonzero once g_t3d_syscolors_rgb_guess holds the originals
#define EQGFX_WORLD_LOAD_MODE EQ_GLOBAL(int, 0x100AADE4)

// Import table entries of this image (call dword ptr [va], filled by the loader): a typedef for the pointer type
// plus EQ_GLOBAL, as eqaddr.h shows for eqgfx_dx7.dll entry points.
typedef void *(__stdcall *GetDC_guess_t)(void *hwnd);
typedef unsigned int (__stdcall *SetSystemPaletteUse_guess_t)(void *hdc, unsigned int use);
typedef int (__stdcall *ReleaseDC_guess_t)(void *hwnd, void *hdc);
typedef int (__stdcall *SetSysColors_guess_t)(int count, const int *elements, const unsigned int *rgb_values);
typedef int (__stdcall *UnrealizeObject_guess_t)(void *gdi_object);
typedef int (__stdcall *DeleteObject_guess_t)(void *gdi_object);
typedef unsigned int (__stdcall *GetSystemPaletteUse_guess_t)(void *hdc);
typedef int (__stdcall *GetDeviceCaps_guess_t)(void *hdc, int capability);
typedef unsigned int (__stdcall *GetSysColor_guess_t)(int element);
typedef void *(__stdcall *CreatePalette_guess_t)(const void *log_palette);
typedef void *(__stdcall *SelectPalette_guess_t)(void *hdc, void *gdi_palette, int force_background);
typedef unsigned int (__stdcall *RealizePalette_guess_t)(void *hdc);

#define pfn_GetDC_guess               EQ_GLOBAL(GetDC_guess_t, 0x10076268)
#define pfn_SetSystemPaletteUse_guess EQ_GLOBAL(SetSystemPaletteUse_guess_t, 0x1007603C)
#define pfn_ReleaseDC_guess           EQ_GLOBAL(ReleaseDC_guess_t, 0x1007626C)
#define pfn_SetSysColors_guess        EQ_GLOBAL(SetSysColors_guess_t, 0x10076264)
#define pfn_UnrealizeObject_guess     EQ_GLOBAL(UnrealizeObject_guess_t, 0x10076040)
#define pfn_DeleteObject_guess        EQ_GLOBAL(DeleteObject_guess_t, 0x10076044)
#define pfn_GetSystemPaletteUse_guess EQ_GLOBAL(GetSystemPaletteUse_guess_t, 0x10076028)
#define pfn_GetDeviceCaps_guess       EQ_GLOBAL(GetDeviceCaps_guess_t, 0x1007602C)
#define pfn_GetSysColor_guess         EQ_GLOBAL(GetSysColor_guess_t, 0x10076260)
#define pfn_CreatePalette_guess       EQ_GLOBAL(CreatePalette_guess_t, 0x10076034)
#define pfn_SelectPalette_guess       EQ_GLOBAL(SelectPalette_guess_t, 0x10076038)
#define pfn_RealizePalette_guess      EQ_GLOBAL(RealizePalette_guess_t, 0x10076048)

#define EQGFX_T3D_ALLOCATE_MEMORY_GUESS EQ_CDECL(void *, 0x10050C48, (unsigned int))
#define EQGFX_T3D_REALLOCATE_MEMORY_GUESS EQ_CDECL(void *, 0x10050C8C, (void *, unsigned int))
#define EQGFX_T3D_FREE_MEMORY_GUESS EQ_CDECL(void, 0x10050CBB, (void *))
#define EQGFX_MEMCPY_GUESS EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
#define EQGFX_QSORT_GUESS EQ_CDECL(void, 0x10068AA4, (void *, unsigned int, unsigned int, int (*)(const void *, const void *)))
#define EQGFX_T3D_GET_POINTER_FROM_DICTIONARY_GUESS EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))
#define parseAddToDictionary EQ_CDECL(int, 0x1000DC08, (void *, void *))
#define EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY_GUESS EQ_CDECL(void, 0x1000E334, (void *, const char *))
#define GFX_COMPOSITE_INITIALIZE_OBJECT EQ_CDECL(int, 0x1001564B, (void *, void *))
#define EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS EQ_CDECL(void, 0x10015D81, (void *, void *))
#define GFX_PATH_COPY_STRING EQ_CDECL(void, 0x10068070, (char *, const char *))
#define strlen_EQGFX EQ_CDECL(unsigned int, 0x10068160, (const char *))
#define EQGFX_T3D_COMPARE_EDGE_KEYS_GUESS ((int (__cdecl *)(const void *, const void *))0x10018FE5)   // qsort comparator LAB_10018fe5 (orders the vertex pairs)

#define T3D_SYSPAL_STATIC             1       // SYSPAL_STATIC: the system palette use the colours are replaced under
#define T3D_SYSPAL_NOSTATIC           2       // SYSPAL_NOSTATIC: the system palette use set while they are replaced
#define T3D_DEVCAP_BITSPIXEL          0xE     // GetDeviceCaps index BITSPIXEL
#define T3D_DEVCAP_PLANES             0xC     // GetDeviceCaps index PLANES
#define T3D_REVERSE_PALETTE_SIZE      0x8000
#define T3D_SYSCOLOR_COUNT            0x13    // the system colour indices and the COLORREFs set, one each per colour
#define T3D_PALETTE_CREATE_FAILED     0xFFFF3D3E // t3dSetWindowPalette's result when CreatePalette returns NULL
#define T3D_REFERENCE_LIST_ALLOC_FAILED 0xFFFF3D3E
#define T3D_REFERENCE_NEGATIVE_INDEX_FACTOR 0x3fffffff

typedef struct t3dReferenceList_guess
{
    int unknown_00;
    int count;
    int capacity;
    unsigned int *references;
} t3dReferenceList_guess;

// The polygon t3dGetEdgePolygonNumEdges reads; no include/ header for it yet.
typedef struct t3dPolygon_guess
{
    char unknown_00[0x04];
    // 0x04: number of edges of this polygon
    int edgeCount;
} t3dPolygon_guess;

// File-local stand-ins for this image's objects (no include/ header yet). Only the fields the functions here read are
// named; the rest is padding.
typedef struct t3dPolyhedronOwner_guess
{
    char unknown_00[0x10];
    // 0x10: count of polyhedrons in this owner, decremented once per t3dDestroyPolyhedron
    int polyhedronCount;
} t3dPolyhedronOwner_guess;

// One 0x20-byte entry of a type-1 polyhedron's element array (t3dPolyhedron_guess 0x1C).
typedef struct t3dPolyhedronElement_guess
{
    char unknown_00[0x04];
    // 0x04: number of edges in the block at 0x08 (t3dGetEdgePolygonNumEdges reads it as t3dPolygon_guess.edgeCount)
    int edgeCount;
    // 0x08: heap block of t3dPolygonEdge_guess entries, freed with t3dFreeMemory
    void *block;
    char unknown_0c[0x14];
} t3dPolyhedronElement_guess;

// One 12-byte vertex, the size of the entries in t3dPolyhedron_guess's vertex list (0x24).
typedef struct t3dVertex_guess
{
    char unknown_00[0x0c];
} t3dVertex_guess;

// One 0x14-byte edge in an element's block (t3dPolyhedronElement_guess 0x08): the vertices it joins.
typedef struct t3dPolygonEdge_guess
{
    char unknown_00[0x04];
    // 0x04: first vertex
    t3dVertex_guess *vertexA;
    // 0x08: second vertex
    t3dVertex_guess *vertexB;
    char unknown_0c[0x08];
} t3dPolygonEdge_guess;

// One 0x18-byte edge of the list t3dGetPolyhedronDefinitionEdgeList returns: its two vertices.
typedef struct t3dEdge_guess
{
    t3dVertex_guess first;
    t3dVertex_guess second;
} t3dEdge_guess;

// The 0x28 block of a type-1 polyhedron: entered in the dictionary like the polyhedron, freed before it.
typedef struct t3dPolyhedronCore_guess
{
    // 0x00: 2 for a core block (t3dDefinePolyhedron writes it)
    int tag;
    char unknown_04[0x04];
    // 0x08: heap block; the dictionary key while flags bit 0 is set
    void *dictionaryKey;
    // 0x0C: bit 0 = the core is entered in the dictionary
    unsigned char flags;
    char unknown_0d[0x03];
    // 0x10: zeroed by t3dDefinePolyhedron; nothing in this unit reads it
    int unknown_10;
    // 0x14: the eighth argument of t3dDefinePolyhedron; nothing in this unit reads it
    unsigned int coreValue;
    // 0x18: the polyhedron this core belongs to
    struct t3dPolyhedron_guess *polyhedron;
} t3dPolyhedronCore_guess;

typedef struct t3dPolyhedron_guess
{
    // 0x00: 1 for a polyhedron; t3dDefinePolyhedron returns the object under a name only while this is 1
    int tag;
    char unknown_04[0x04];
    // 0x08: heap block; the dictionary key while flags bit 0 is set, freed by t3dDestroyPolyhedron
    void *dictionaryKey;
    // 0x0C: bit 0 = the polyhedron is entered in the dictionary
    unsigned char flags;
    char unknown_0d[0x03];
    // 0x10: 1 = a type-1 polyhedron, which t3dUndefinePolyhedron frees whole (core, elements, auxBlock)
    int type;
    // 0x14: the third argument of t3dDefinePolyhedron; nothing in this unit reads it
    int stateValue;
    // 0x18: the owner whose polyhedronCount is decremented (t3dDestroyPolyhedron); a type-1 polyhedron keeps its
    // element count here instead, which t3dUndefinePolyhedron reads raw
    t3dPolyhedronOwner_guess *owner;
    // 0x1C: the element array (t3dPolyhedronElement_guess entries), freed as one block
    t3dPolyhedronElement_guess *elements;
    // 0x20: vertex count of the list at 0x24, 12 bytes per entry (t3dGetPolyhedronDefinitionVertexList)
    int vertexCount;
    // 0x24: heap block, freed unconditionally; the vertex list that t3dGetPolyhedronDefinitionVertexList copies out
    void *auxBlock;
    // 0x28: the core block (t3dPolyhedronCore_guess), freed first
    t3dPolyhedronCore_guess *core;
} t3dPolyhedron_guess;

// The 0x24 payload of a world object: a tagged block whose owner's polyhedronCount is decremented.
typedef struct t3dWorldPayload_guess
{
    // 0x00: 0x25 when the owner is decremented
    int tag;
    // 0x04: the owner, whose polyhedronCount (0x10) is decremented
    t3dPolyhedronOwner_guess *owner;
} t3dWorldPayload_guess;

// The 0x2C region set of a world object: a tagged block (tag 0x27 or 0x29) with two heap blocks and the region
// reference list at 0x28, which t3dFreeReferenceList clears.
typedef struct t3dRegionSet_guess
{
    // 0x00: 0x27 or 0x29 for a set that is cleaned up here
    int tag;
    char unknown_04[0x08];
    // 0x0C: heap block, freed with t3dFreeMemory when not null
    void *firstBlock;
    char unknown_10[0x08];
    // 0x18: heap block, freed with t3dFreeMemory when not null
    void *secondBlock;
    char unknown_1c[0x0c];
    // 0x28: the region reference list (count at +0x4, spare at +0x8, array at +0xc)
    char referenceList[0x10];
} t3dRegionSet_guess;

// File-local stand-in for the world object that cleanupWorldObject_guess tears down.
typedef struct t3dWorldObject_guess
{
    char unknown_00[0x08];
    // 0x08: heap block; the dictionary key while flags bit 0 is set
    void *dictionaryKey;
    // 0x0C: bit 0 = the object is entered in the dictionary, bit 5 = the payload at 0x24 is not freed here
    unsigned char flags;
    char unknown_0d[0x17];
    // 0x24: the payload (t3dWorldPayload_guess), freed unless flags bit 5 is set
    t3dWorldPayload_guess *payload;
    char unknown_28[0x04];
    // 0x2C: the region set (t3dRegionSet_guess), cleaned up when its tag is 0x27 or 0x29
    t3dRegionSet_guess *regions;
    char unknown_30[0x04];
    // 0x34: nonzero = the helper at 0x10009237 runs first (what it does is not confirmed)
    int helperPending;
} t3dWorldObject_guess;

// File-local stand-ins for the actor table that t3dSendMessageToAllRegionActors walks (no include/ header yet).
typedef struct t3dRegionActor_guess
{
    char unknown_00[0x64];
    // 0x64: vtable of the actor's message interface; slot 8 (+0x20) is called with (message, world, actor)
    void **vtable;
} t3dRegionActor_guess;

typedef struct t3dWorld_guess
{
    char unknown_00[0x17c];
    // 0x17C: the actor table, indexed by the int entries of a region set's first block
    t3dRegionActor_guess **regionActors;
} t3dWorld_guess;

typedef void (__cdecl *t3dRegionMessage_guess)(unsigned int message, t3dWorld_guess *world, t3dRegionActor_guess *actor);

// One 4-byte entry of t3dPalette_guess.entries: the red, green and blue bytes t3dGetPaletteEntry returns, then one unread byte.
typedef struct t3dPaletteEntry_guess
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char unknown_03;
} t3dPaletteEntry_guess;

// The palette t3dDestroyPalette frees and t3dGetPaletteEntry reads (only the fields they touch are named; the object's size is not known).
typedef struct t3dPalette_guess
{
    int unknown_00;
    int unknown_04;
    // 0x08: heap block, freed with t3dFreeMemory; the dictionary key while flags bit 0 is set
    void *dictionaryKey;
    // 0x0C: bit 0 = the palette is entered in its owner's dictionary
    unsigned char flags;
    char unknown_0d[0x03];
    // 0x10: the LOGPALETTE header CreatePalette reads (palVersion, palNumEntries); the entries follow at 0x14
    unsigned char logPaletteHeader[0x04];
    // 0x14: 256 entries of 4 bytes, indexed by the palette index t3dGetPaletteEntry takes
    t3dPaletteEntry_guess entries[0x100];
    char unknown_414[0x04];
    // 0x418: set to 1 by t3dSetPaletteEntry after it writes an entry
    int entriesChanged;
    // 0x41C: GDI palette handle, unrealized then deleted
    void *hPalette;
    // 0x420, 0x424, 0x428: heap blocks, freed with t3dFreeMemory (their contents are not confirmed)
    void *block420;
    void *block424;
    void *block428;
} t3dPalette_guess;

// The DMRGB track s3dGetDMRGBTrackCurrentFrame reads (only the field it touches is named).
typedef struct s3dDMRGBTrack_guess
{
    char unknown_00[0x18];
    // 0x18: the track's current frame number
    int currentFrame;
} s3dDMRGBTrack_guess;

// Bit 0 of t3dPalette_guess.flags.
#define T3D_PALETTE_IN_DICTIONARY 0x01

// FUNCTION: EQGFX 0x1001802D
// Create an independent copy of a rendering palette.
t3dPalette_guess *__cdecl t3dDuplicatePalette(int owner, t3dPalette_guess *source)
{
    t3dPalette_guess *duplicate;
    void *reversePaletteEntries;
    int dictionaryResult;

    duplicate = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(0x42C);
    if (duplicate == 0)
    {
        goto return_duplicate;
    }
    duplicate->unknown_00 = 0x23;
    duplicate->dictionaryKey = 0;
    *(int *)&duplicate->flags = 0;
    *(int *)duplicate->unknown_414 = *(int *)source->unknown_414;
    duplicate->entriesChanged = 1;
    duplicate->hPalette = 0;
    EQGFX_MEMCPY_GUESS(duplicate->logPaletteHeader, source->logPaletteHeader, 0x404);
    if (source->block420 == 0)
    {
        duplicate->block420 = 0;
    }
    else
    {
        reversePaletteEntries = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(0x8000);
        duplicate->block420 = reversePaletteEntries;
        if (reversePaletteEntries == 0)
        {
            goto free_duplicate;
        }
        EQGFX_MEMCPY_GUESS(reversePaletteEntries, source->block420, 0x8000);
    }
    if (owner == 0)
    {
        duplicate->unknown_04 = -1;
        goto return_duplicate;
    }
    dictionaryResult = EQ_CDECL(int, 0x1001564B, (int, t3dPalette_guess *))(owner, duplicate);
    if (dictionaryResult >= 0)
    {
        goto return_duplicate;
    }
free_duplicate:
    EQGFX_T3D_FREE_MEMORY_GUESS(duplicate);
    duplicate = 0;
return_duplicate:
    return duplicate;
}

// FUNCTION: EQGFX 0x100180EB
// __cdecl (RET 0: the caller pops), int. Frees the palette; if owner is set, calls 0x10015D81 on it and returns owner.
int __cdecl t3dDestroyPalette(int owner, t3dPalette_guess *palette)
{
    if (palette->block424 != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(palette->block424);
    }
    if (palette->block428 != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(palette->block428);
    }
    if (palette->block420 != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(palette->block420);
    }
    if (palette->hPalette != 0)
    {
        pfn_UnrealizeObject_guess(palette->hPalette);
        pfn_DeleteObject_guess(palette->hPalette);
    }
    if (owner != 0)
    {
        if ((palette->flags & T3D_PALETTE_IN_DICTIONARY) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (int, void *))(owner, palette->dictionaryKey);
        }
        owner = EQ_CDECL(int, 0x10015D81, (int, t3dPalette_guess *))(owner, palette);
    }
    if (palette->dictionaryKey != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(palette->dictionaryKey);
    }
    EQGFX_T3D_FREE_MEMORY_GUESS(palette);
    return owner;
}

// FUNCTION: EQGFX 0x10018188
// Makes a palette's colors take effect in a window.
// __cdecl, int. Rebuilds the palette if entriesChanged is set; realizes it in the window's DC. Failed CreatePalette returns T3D_PALETTE_CREATE_FAILED.
int __cdecl t3dSetWindowPalette(void *hwnd, t3dPalette_guess *palette)
{
    int result;
    void *hdc;
    void *oldPalette;

    result = 0;
    if (palette->entriesChanged != 0)
    {
        if (palette->hPalette != 0)
        {
            pfn_UnrealizeObject_guess(palette->hPalette);
            pfn_DeleteObject_guess(palette->hPalette);
        }
        if ((palette->hPalette = pfn_CreatePalette_guess(palette->logPaletteHeader)) == 0)
        {
            result = T3D_PALETTE_CREATE_FAILED;
            return result;
        }
        palette->entriesChanged = 0;
    }
    else
    {
        pfn_UnrealizeObject_guess(palette->hPalette);
    }
    hdc = pfn_GetDC_guess(hwnd);
    oldPalette = pfn_SelectPalette_guess(hdc, palette->hPalette, 0);
    pfn_RealizePalette_guess(hdc);
    pfn_SelectPalette_guess(hdc, oldPalette, 0);
    pfn_ReleaseDC_guess(hwnd, hdc);
    return result;
}

// FUNCTION: EQGFX 0x1001822B
// Replace Windows system colors for rendering on an 8-bit palette display, saving the originals.
// __cdecl, int, no arguments. On an 8 bpp static palette: saves the 0x13 system colours once, then sets them to g_t3d_syscolors_index_end_guess.
int __cdecl t3dReplaceSystemColors(void)
{
    void *hdc;
    GetDeviceCaps_guess_t getDeviceCaps;
    int bitsPerPixel;
    int planes;
    int *indexPtr;
    unsigned int *rgbPtr;
    int sysColorIndex;
    unsigned int sysColor;

    hdc = pfn_GetDC_guess(0);
    if (pfn_GetSystemPaletteUse_guess(hdc) == T3D_SYSPAL_STATIC)
    {
        getDeviceCaps = pfn_GetDeviceCaps_guess;
        bitsPerPixel = getDeviceCaps(hdc, T3D_DEVCAP_BITSPIXEL);
        planes = getDeviceCaps(hdc, T3D_DEVCAP_PLANES);
        if (bitsPerPixel * planes == 8)
        {
            int *indices;

            indices = g_t3d_syscolors_index_guess;
            if (g_t3d_syscolors_captured_guess == 0)
            {
                rgbPtr = g_t3d_syscolors_rgb_guess;
                indexPtr = indices;
                do
                {
                    sysColorIndex = *indexPtr;
                    indexPtr = indexPtr + 1;
                    sysColor = pfn_GetSysColor_guess(sysColorIndex);
                    *rgbPtr = sysColor;
                    rgbPtr = rgbPtr + 1;
                } while (indexPtr < g_t3d_syscolors_index_end_guess);
                g_t3d_syscolors_captured_guess = 1;
            }
            pfn_SetSystemPaletteUse_guess(hdc, T3D_SYSPAL_NOSTATIC);
            pfn_SetSysColors_guess(T3D_SYSCOLOR_COUNT, indices, g_t3d_syscolors_replace_rgb_guess);
            g_t3d_syscolors_saved_guess = 1;
        }
    }
    pfn_ReleaseDC_guess(0, hdc);
    return 0;
}

// FUNCTION: EQGFX 0x100182C8
// Restores the original Windows system colors after renderer palette use.
// __cdecl (a guess: no arguments, so cdecl and stdcall give the same bytes here; callers unseen), int, no arguments.
// If g_t3d_syscolors_saved_guess is nonzero: takes the screen DC, sets its system palette use to 1 (SYSPAL_NOSTATIC), releases it,
// restores the 0x13 system colours from g_t3d_syscolors_rgb_guess (COLORREFs) by the indices at g_t3d_syscolors_index_guess, then clears
// g_t3d_syscolors_saved_guess. Returns 0 either way.
int __cdecl t3dRestoreSystemColors_guess(void)
{
    void *hdc;

    if (g_t3d_syscolors_saved_guess != 0)
    {
        hdc = pfn_GetDC_guess(0);
        pfn_SetSystemPaletteUse_guess(hdc, 1);
        pfn_ReleaseDC_guess(0, hdc);
        pfn_SetSysColors_guess(0x13, g_t3d_syscolors_index_guess, g_t3d_syscolors_rgb_guess);
        g_t3d_syscolors_saved_guess = 0;
    }
    return 0;
}

// The palette scanned below: 256 four-byte pens at 0x14 and a flag at 0x414 (nonzero: all 256 pens are fair game).
typedef struct t3dPalettePens_guess
{
    unsigned char reserved_0000[0x14];  // 0x00
    unsigned char entries[0x400];       // 0x14: 4-byte pens, R,G,B + one unused byte
    unsigned int scan_all;              // 0x414
} t3dPalettePens_guess;

// FUNCTION: EQGFX 0x10018586
// Match an RGB color to the nearest available palette color.
// __cdecl (four stack args, callee pops nothing); callers read AL only. Returns the index of the palette pen
// nearest to red/green/blue (squared byte deltas, a strict improvement wins, an exact hit stops the scan).
unsigned char __cdecl t3dPaletteClosestPenToColor(t3dPalettePens_guess *palette, unsigned char red,
                                                  unsigned char green, unsigned char blue)
{
    unsigned int best;
    unsigned char *pen;
    unsigned char *end;
    unsigned char index;
    unsigned char result;

    best = 0xFFFFFFFF;
    if (palette->scan_all != 0)
    {
        index = 0;
        pen = palette->entries;
        end = palette->entries + 256 * 4;
    }
    else
    {
        pen = palette->entries + 10 * 4;
        end = palette->entries + 246 * 4;
        index = 10;
    }
    while (pen < end)
    {
        unsigned int red_diff = pen[0] - red;
        unsigned int green_diff = pen[1] - green;
        unsigned int blue_diff = pen[2] - blue;
        unsigned int diff = blue_diff * blue_diff + green_diff * green_diff + red_diff * red_diff;

        if (diff < best)
        {
            result = index;
            if (diff == 0)
                break;
            best = diff;
        }
        index++;
        pen += 4;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001830B
// Applies per-channel lookup tables to the palette's colors.
// __cdecl, int, four stack arguments (palette, redTable, greenTable, blueTable). A non-null table maps its channel of each entry in range (1..254 while the dword at 0x414 is nonzero, else 10..245); entriesChanged is set to 1 and 0 is returned.
int __cdecl t3dApplyGammaCorrection(t3dPalette_guess *palette, unsigned char *redTable,
                                    unsigned char *greenTable, unsigned char *blueTable)
{
    t3dPaletteEntry_guess *entry;
    t3dPaletteEntry_guess *entriesEnd;

    if (*(int *)palette->unknown_414 != 0)
    {
        entry = &palette->entries[1];
        entriesEnd = entry + 254;
    }
    else
    {
        entry = &palette->entries[10];
        entriesEnd = entry + 236;
    }
    for (; entry < entriesEnd; entry++)
    {
        if (redTable != 0)
        {
            entry->red = redTable[entry->red];
        }
        if (greenTable != 0)
        {
            entry->green = greenTable[entry->green];
        }
        if (blueTable != 0)
        {
            entry->blue = blueTable[entry->blue];
        }
    }
    palette->entriesChanged = 1;
    return 0;
}

// FUNCTION: EQGFX 0x1001837E
// Rotate a selected palette color range one step in either direction.
// __cdecl, int, four stack arguments (palette, firstIndex, count, reverse). The count entries from firstIndex turn one slot: reverse 0 moves the first entry to the end, else the last entry moves to the front. entriesChanged is set and 0 returned, or 0xFFFF3D3E for a bad range.
int __cdecl t3dCyclePaletteEntries(t3dPalette_guess *palette, unsigned int firstIndex, int count, int reverse)
{
    unsigned int *first;
    unsigned int *last;
    unsigned int *cursor;
    unsigned int *source;
    unsigned int saved;
    unsigned int dwordCount;
    int result;

    result = 0;
    if (*(int *)palette->unknown_414 != 0)
    {
        if (count != 0 && firstIndex != 0 && firstIndex + count <= 0xff)
        {
            goto cycle;
        }
reject:
        result = 0xffff3d3e;
        return result;
    }
    if (count == 0 || firstIndex <= 9 || firstIndex + count > 0xec)
    {
        goto reject;
    }
cycle:
    first = (unsigned int *)&palette->entries[firstIndex];
    last = first + count - 1;
    if (reverse != 0)
    {
        saved = *last;
        for (cursor = last; first < cursor; cursor--)
        {
            *cursor = cursor[-1];
        }
        *first = saved;
    }
    else
    {
        saved = *first;
        if (first < last)
        {
            cursor = first;
            source = first + 1;
            for (dwordCount = ((unsigned int)last - (unsigned int)first + 3) >> 2; dwordCount != 0; dwordCount--)
            {
                *cursor++ = *source++;
            }
        }
        *last = saved;
    }
    palette->entriesChanged = 1;
    return result;
}

// FUNCTION: EQGFX 0x1001841E
// Reads one palette color's red, green, and blue values.
// __cdecl, int, five stack arguments. An index above 0xFF returns 0xFFFF3D3E; else the entry's red, green and blue bytes go out and 0 is returned.
int __cdecl t3dGetPaletteEntry(t3dPalette_guess *palette, unsigned int index, unsigned char *red,
                               unsigned char *green, unsigned char *blue)
{
    t3dPaletteEntry_guess *entry;

    if (index > 0xff)
    {
        return 0xffff3d3e;
    }
    entry = &palette->entries[index];
    *red = entry->red;
    *green = entry->green;
    *blue = entry->blue;
    return 0;
}

// FUNCTION: EQGFX 0x10018457
// Copies a range of palette colors out of the palette.
// __cdecl, int, four stack arguments. Copies count 4-byte entries from firstIndex into dest; 0xFFFF3D3E when firstIndex + count is above 0x100, else 0.
int __cdecl t3dGetPaletteEntries(t3dPalette_guess *palette, unsigned int firstIndex, unsigned int count, void *dest)
{
    int result;

    result = 0;
    if (firstIndex + count > 0x100)
    {
        result = 0xffff3d3e;
    }
    else
    {
        if (count > 0)
        {
            EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))
                (dest, &palette->entries[firstIndex], count << 2);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10018495
// Changes the red, green, and blue values of one palette color.
// __cdecl, int, five stack arguments. Index 0 and indices from 0xFF up return 0xFFFF3D3E; else the entry's red, green and blue are set, entriesChanged is set to 1, and 0 is returned.
int __cdecl t3dSetPaletteEntry(t3dPalette_guess *palette, unsigned int index, unsigned char red,
                               unsigned char green, unsigned char blue)
{
    t3dPaletteEntry_guess *entry;
    int result;

    result = 0;
    if (index != 0 && index < 0xff)
    {
        entry = &palette->entries[index];
        entry->red = red;
        entry->green = green;
        entry->blue = blue;
        palette->entriesChanged = 1;
    }
    else
    {
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x100184D8
// Brighten editable palette colors by doubling their RGB intensity, capped at full brightness.
// __cdecl, int, one stack argument (palette). Doubles the red, green and blue of the entries in range (1..254 while the dword at 0x414 is nonzero, else 10..245), saturating at 0xFF, sets entriesChanged to 1, and returns 0.
int __cdecl t3dBrightenPalette(t3dPalette_guess *palette)
{
    t3dPaletteEntry_guess *entry;
    t3dPaletteEntry_guess *entriesEnd;

    if (*(int *)palette->unknown_414 != 0)
    {
        entry = &palette->entries[1];
        entriesEnd = entry + 254;
    }
    else
    {
        entry = &palette->entries[10];
        entriesEnd = entry + 236;
    }
    for (; entry < entriesEnd; entry++)
    {
        if (entry->red >= 0x80)
        {
            entry->red = 0xff;
        }
        else
        {
            entry->red = entry->red << 1;
        }
        if (entry->green >= 0x80)
        {
            entry->green = 0xff;
        }
        else
        {
            entry->green = entry->green << 1;
        }
        if (entry->blue >= 0x80)
        {
            entry->blue = 0xff;
        }
        else
        {
            entry->blue = entry->blue << 1;
        }
    }
    palette->entriesChanged = 1;
    return 0;
}

// FUNCTION: EQGFX 0x10018547
// Halves the usable palette colors to dim the displayed scene.
// __cdecl, int, one stack argument (palette). Halves the red, green and blue of the entries in range (1..254 while the dword at 0x414 is nonzero, else 10..245), sets entriesChanged to 1, and returns 0.
int __cdecl t3dDimPalette(t3dPalette_guess *palette)
{
    t3dPaletteEntry_guess *entry;
    t3dPaletteEntry_guess *entriesEnd;

    if (*(int *)palette->unknown_414 != 0)
    {
        entry = &palette->entries[1];
        entriesEnd = entry + 254;
    }
    else
    {
        entry = &palette->entries[10];
        entriesEnd = entry + 236;
    }
    for (; entry < entriesEnd; entry++)
    {
        entry->red >>= 1;
        entry->green >>= 1;
        entry->blue >>= 1;
    }
    palette->entriesChanged = 1;
    return 0;
}

// FUNCTION: EQGFX 0x10018618
// Stores a supplied lookup table on the palette; its use is unclear.
// __cdecl, int, two stack arguments (palette, reversePalette). Copies 0x8000 bytes from reversePalette into the palette's block at +0x420, allocating that block first when it is null; returns 0, or 0xFFFF3D3E when the allocation fails (nothing is copied).
int __cdecl t3dSetPaletteReversePalette(t3dPalette_guess *palette, void *reversePalette)
{
    int result;

    result = 0;
    if (palette->block420 == 0)
    {
        palette->block420 = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D_REVERSE_PALETTE_SIZE);
        if (palette->block420 == 0)
        {
            result = 0xffff3d3e;
            goto done;
        }
    }
    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(palette->block420, reversePalette, T3D_REVERSE_PALETTE_SIZE);
done:
    return result;
}

// FUNCTION: EQGFX 0x1001865F
// Fills the 32K 15-bit colour -> nearest palette pen table; an optional colour is forced to the given pen.
int __cdecl t3dPaletteBuildReversePalette(t3dPalettePens_guess *palette, unsigned char *reversePalette,
                                          unsigned char firstPenIndex, unsigned char lastPenIndex,
                                          unsigned char highlightPenIndex, unsigned char *colorKey)
{
    unsigned char *firstPen;
    unsigned char *penEnd;
    unsigned char *pen;
    unsigned char *output;
    int *redCosts;
    int *greenCosts;
    int *blueCosts;
    int *costRow;
    int *redCost;
    int *greenCostRow;
    int *blueCostRow;
    int result;
    int intensity;
    int channelDifference;
    int redCount;
    int greenCount;
    int blueCount;
    unsigned int bestCost;
    unsigned int cost;
    unsigned char candidatePen;
    unsigned char bestPen;

    result = 0;
    if (palette->scan_all == 0)
    {
        if (firstPenIndex < 10)
            firstPenIndex = 10;
        if (lastPenIndex > 0xf5)
            lastPenIndex = 0xf5;
    }
    firstPen = palette->entries + (unsigned int)firstPenIndex * 4;
    penEnd = palette->entries + (unsigned int)lastPenIndex * 4 + 4;
    redCosts = (int *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D_REVERSE_PALETTE_SIZE);
    if (redCosts == 0 ||
        (greenCosts = (int *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D_REVERSE_PALETTE_SIZE)) == 0 ||
        (blueCosts = (int *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D_REVERSE_PALETTE_SIZE)) == 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        intensity = 0;
        costRow = redCosts;
        do
        {
            if (firstPen < penEnd)
            {
                pen = firstPen;
                do
                {
                    channelDifference = (int)pen[0] - intensity;
                    *(unsigned int *)(pen + ((char *)costRow - (char *)firstPen)) =
                        channelDifference * channelDifference;
                    pen += 4;
                } while (pen < penEnd);
            }
            intensity += 8;
            costRow += 0x100;
        } while (intensity < 0x100);

        intensity = 0;
        costRow = greenCosts;
        do
        {
            if (firstPen < penEnd)
            {
                pen = firstPen;
                do
                {
                    channelDifference = (int)pen[1] - intensity;
                    *(unsigned int *)(pen + ((char *)costRow - (char *)firstPen)) =
                        channelDifference * channelDifference;
                    pen += 4;
                } while (pen < penEnd);
            }
            intensity += 8;
            costRow += 0x100;
        } while (intensity < 0x100);

        intensity = 0;
        costRow = blueCosts;
        do
        {
            if (firstPen < penEnd)
            {
                pen = firstPen;
                do
                {
                    channelDifference = (int)pen[2] - intensity;
                    *(unsigned int *)(pen + ((char *)costRow - (char *)firstPen)) =
                        channelDifference * channelDifference;
                    pen += 4;
                } while (pen < penEnd);
            }
            intensity += 8;
            costRow += 0x100;
        } while (intensity < 0x100);

        output = reversePalette;
        blueCostRow = blueCosts;
        blueCount = 0x20;
        bestPen = (unsigned char)((unsigned int)redCosts >> 24);
        do
        {
            greenCostRow = greenCosts;
            greenCount = 0x20;
            do
            {
                costRow = redCosts;
                redCount = 0x20;
                do
                {
                    bestCost = 0xffffffff;
                    candidatePen = firstPenIndex;
                    if (firstPen < penEnd)
                    {
                        pen = firstPen;
                        redCost = costRow;
                        do
                        {
                            cost = *(unsigned int *)(pen + ((char *)greenCostRow - (char *)firstPen));
                            cost += *(unsigned int *)(pen + ((char *)blueCostRow - (char *)firstPen));
                            cost += *redCost;
                            if (cost < bestCost)
                            {
                                bestCost = cost;
                                bestPen = candidatePen;
                            }
                            candidatePen++;
                            pen += 4;
                            redCost++;
                        } while (pen < penEnd);
                    }
                    *output = bestPen;
                    output++;
                    costRow += 0x100;
                    redCount--;
                } while (redCount != 0);
                greenCostRow += 0x100;
                greenCount--;
            } while (greenCount != 0);
            blueCostRow += 0x100;
            blueCount--;
        } while (blueCount != 0);

        EQGFX_T3D_FREE_MEMORY_GUESS(redCosts);
        EQGFX_T3D_FREE_MEMORY_GUESS(greenCosts);
        EQGFX_T3D_FREE_MEMORY_GUESS(blueCosts);
        if (colorKey != 0)
        {
            unsigned int colorIndex;

            colorIndex = (((colorKey[0] & 0xf8) << 5) | (colorKey[1] & 0xf8)) << 2;
            colorIndex |= colorKey[2] >> 3;
            reversePalette[colorIndex] = highlightPenIndex;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10018883
// Builds a reverse-color table for mapping colors back to palette pens.
// __cdecl, int, four args after the palette (callee pops nothing). Allocates the null 0x8000-byte block at +0x420 (0xFFFF3D3E if that fails), then returns the build on it.
int __cdecl t3dPaletteCreateReversePalette(t3dPalette_guess *palette, unsigned int firstPenIndex, unsigned int lastPenIndex,
                                           unsigned int highlightPenIndex, unsigned char *colorKey)
{
    void *reversePalette;

    if (palette->block420 == 0)
    {
        reversePalette = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(T3D_REVERSE_PALETTE_SIZE);
        palette->block420 = reversePalette;
        if (reversePalette == 0)
        {
            return 0xffff3d3e;
        }
    }
    return EQ_CDECL(int, 0x1001865F, (void *, void *, unsigned int, unsigned int, unsigned int, unsigned char *))(
        palette, palette->block420, firstPenIndex, lastPenIndex, highlightPenIndex, colorKey);
}

// FUNCTION: EQGFX 0x100188CD
// Defines a named polyhedron from vertex and polygon lists it copies; a name already defined returns that one.
// __cdecl, pointer result. Returns a kind-1 polyhedron already under the name; else builds one (name, vertices, core, elements, edges), enters it and returns it, or unwinds and returns 0.
t3dPolyhedron_guess *__cdecl t3dDefinePolyhedron(void *context, const char *name, int stateValue, int elementCount,
                                                 void *sourceElements, int vertexCount, void *sourceVertices, unsigned int coreValue)
{
    t3dPolyhedron_guess *polyhedron;
    t3dPolyhedronCore_guess *core;
    t3dPolyhedronElement_guess *elementCopy;
    t3dPolyhedronElement_guess *elementsEnd;
    t3dPolyhedronElement_guess *elementCursor;
    t3dPolyhedronElement_guess *element;
    t3dPolygonEdge_guess *edges;
    t3dPolygonEdge_guess *edgeCursor;
    void *nameBlock;
    void *vertexCopy;
    unsigned int nameSize;
    unsigned int edgeBytes;
    unsigned int elementBytes;
    int result;

    polyhedron = (t3dPolyhedron_guess *)EQGFX_T3D_GET_POINTER_FROM_DICTIONARY_GUESS(context, name);
    if (polyhedron == 0)
    {
        polyhedron = (t3dPolyhedron_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(0x2c);
        if (polyhedron == 0)
        {
            return 0;
        }
        *(unsigned int *)&polyhedron->flags = 0;
        polyhedron->tag = 1;
        polyhedron->type = 1;
        polyhedron->stateValue = stateValue;
        polyhedron->vertexCount = vertexCount;
        polyhedron->owner = (t3dPolyhedronOwner_guess *)elementCount;
        nameSize = strlen_EQGFX(name);
        nameBlock = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(nameSize + 1);
        polyhedron->dictionaryKey = nameBlock;
        if (nameBlock == 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
        }
        else
        {
            GFX_PATH_COPY_STRING(nameBlock, name);
            result = GFX_COMPOSITE_INITIALIZE_OBJECT(context, polyhedron);
            if (result == 0)
            {
                result = parseAddToDictionary(context, polyhedron);
                if (result < 0)
                {
                    EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, polyhedron);
                    EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
                    EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
                }
                else
                {
                    vertexCopy = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(vertexCount * 12);
                    if (vertexCopy == 0)
                    {
                        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY_GUESS(context, name);
                        EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, polyhedron);
                        EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
                        EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
                    }
                    else
                    {
                        polyhedron->auxBlock = vertexCopy;
                        EQGFX_MEMCPY_GUESS(vertexCopy, sourceVertices, vertexCount * 12);
                        core = (t3dPolyhedronCore_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(0x1c);
                        if (core == 0)
                        {
                            EQGFX_T3D_FREE_MEMORY_GUESS(vertexCopy);
                            EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY_GUESS(context, name);
                            EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, polyhedron);
                            EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
                            EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
                        }
                        else
                        {
                            polyhedron->core = core;
                            core->dictionaryKey = 0;
                            *(unsigned int *)&core->flags = 0;
                            core->unknown_10 = 0;
                            core->tag = 2;
                            core->coreValue = coreValue;
                            core->polyhedron = polyhedron;
                            result = GFX_COMPOSITE_INITIALIZE_OBJECT(context, core);
                            if (result < 0)
                            {
                                EQGFX_T3D_FREE_MEMORY_GUESS(core);
                                EQGFX_T3D_FREE_MEMORY_GUESS(vertexCopy);
                                EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY_GUESS(context, name);
                                EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, polyhedron);
                                EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
                                EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
                            }
                            else
                            {
                                elementBytes = elementCount * 0x20;
                                elementCopy = (t3dPolyhedronElement_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(elementBytes);
                                if (elementCopy == 0)
                                {
                                    EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, core);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(core);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(vertexCopy);
                                    EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY_GUESS(context, name);
                                    EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, polyhedron);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
                                }
                                else
                                {
                                    polyhedron->elements = elementCopy;
                                    EQGFX_MEMCPY_GUESS(elementCopy, sourceElements, elementBytes);
                                    elementsEnd = elementCopy + elementCount;
                                    elementCursor = elementsEnd;
                                    for (;;)
                                    {
                                        element = elementCursor - 1;
                                        if (elementCursor <= elementCopy)
                                        {
                                            return polyhedron;
                                        }
                                        edgeBytes = element->edgeCount * 0x14;
                                        edges = (t3dPolygonEdge_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(edgeBytes);
                                        if (edges == 0)
                                        {
                                            break;
                                        }
                                        EQGFX_MEMCPY_GUESS(edges, element->block, edgeBytes);
                                        element->block = edges;
                                        edgeCursor = (t3dPolygonEdge_guess *)((int)edges + edgeBytes);
                                        while (edges < edgeCursor)
                                        {
                                            edgeCursor--;
                                            edgeCursor->vertexA = (t3dVertex_guess *)((int)vertexCopy +
                                                ((int)edgeCursor->vertexA - (int)sourceVertices) / 12 * 12);
                                            edgeCursor->vertexB = (t3dVertex_guess *)((int)vertexCopy +
                                                ((int)edgeCursor->vertexB - (int)sourceVertices) / 12 * 12);
                                        }
                                        elementCursor = element;
                                    }
                                    elementCursor = elementsEnd - 1;
                                    while (elementCursor > element)
                                    {
                                        EQGFX_T3D_FREE_MEMORY_GUESS(elementCursor->block);
                                        elementCursor--;
                                    }
                                    EQGFX_T3D_FREE_MEMORY_GUESS(elementCopy);
                                    EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, core);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(core);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(vertexCopy);
                                    EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY_GUESS(context, name);
                                    EQGFX_T3D_FREE_OBJECT_POOL_SLOT_GUESS(context, polyhedron);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
                                    EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
                EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
            }
        }
    }
    else if (polyhedron->tag == 1)
    {
        return polyhedron;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10018BFD
// Remove a polyhedron definition and release its polygon and vertex data.
// __cdecl, int, two stack arguments (context, polyhedron). A type-1 polyhedron is freed whole; any other type returns 0xFFFF3D3E.
int __cdecl t3dUndefinePolyhedron(void *context, t3dPolyhedron_guess *polyhedron)
{
    t3dPolyhedronCore_guess *core;
    t3dPolyhedronElement_guess *elements;
    int elementCount;
    t3dPolyhedronElement_guess *elementsEnd;
    int result;

    if (polyhedron->type != 1)
    {
        result = 0xffff3d3e;
    }
    else
    {
        core = polyhedron->core;
        if ((core->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(context, core->dictionaryKey);
        }
        EQ_CDECL(int, 0x10015D81, (void *, void *))(context, core);
        if (core->dictionaryKey != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(core->dictionaryKey);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(core);
        elementCount = *(int *)((char *)polyhedron + 0x18);
        elements = polyhedron->elements;
        elementsEnd = elements + elementCount;
        while (elements < elementsEnd)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS((elementsEnd - 1)->block);
            elementsEnd--;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(elements);
        EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->auxBlock);
        if ((polyhedron->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(context, polyhedron->dictionaryKey);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(context, polyhedron);
        if (polyhedron->dictionaryKey != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
    }
    return result;
}

// FUNCTION: EQGFX 0x10018CB7
// __cdecl, int, one stack argument (track). Returns the track's current frame (track + 0x18).
int __cdecl s3dGetDMRGBTrackCurrentFrame(s3dDMRGBTrack_guess *track)
{
    return track->currentFrame;
}

// FUNCTION: EQGFX 0x10018CBF
// Returns one polygon entry from a polyhedron definition by index.
// __cdecl, pointer, two stack arguments (polyhedron, index). Returns elements[index], or NULL when index >= the count at 0x18
// (read unsigned; the owner field of t3dPolyhedron_guess).
t3dPolyhedronElement_guess *__cdecl t3dGetPolyhedronDefinitionEdgePolygon(t3dPolyhedron_guess *polyhedron, unsigned int index)
{
    if (index >= (unsigned int)polyhedron->owner)
    {
        return 0;
    }
    return polyhedron->elements + index;
}

// FUNCTION: EQGFX 0x10018CD8
// Copies a polyhedron definition's vertex list for use by the caller.
// __cdecl, int, two stack arguments (polyhedron, vertices). Copies vertexCount 12-byte entries of the polyhedron's
// auxBlock (0x24) into vertices, returns 0.
int __cdecl t3dGetPolyhedronDefinitionVertexList(t3dPolyhedron_guess *polyhedron, void *vertices)
{
    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(vertices, polyhedron->auxBlock, polyhedron->vertexCount * 0x0c);
    return 0;
}

// FUNCTION: EQGFX 0x10018CF8
// Creates a polyhedron using the core data from a source definition.
// __cdecl, pointer result. Copies the 0x1c core of source into a new polyhedron, clears its key, flags and type, registers
// it with the context (_t3dAddItemToList_guess) and bumps the owner's count; returns 0 when the allocation or the registration fails.
t3dPolyhedron_guess *__cdecl t3dCreatePolyhedron(void *context, t3dPolyhedron_guess *source)
{
    t3dPolyhedron_guess *polyhedron;
    unsigned int *sourceWord;
    unsigned int *polyhedronWord;
    int wordCount;
    int result;

    polyhedron = (t3dPolyhedron_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x1c);
    if (polyhedron != 0)
    {
        sourceWord = (unsigned int *)source->core;
        polyhedronWord = (unsigned int *)polyhedron;
        for (wordCount = 7; wordCount != 0; wordCount = wordCount - 1)
        {
            *polyhedronWord = *sourceWord;
            sourceWord = sourceWord + 1;
            polyhedronWord = polyhedronWord + 1;
        }
        polyhedron->dictionaryKey = 0;
        *(unsigned int *)&polyhedron->flags = 0;
        polyhedron->type = 0;
        result = EQ_CDECL(int, 0x1001564B, (void *, t3dPolyhedron_guess *))(context, polyhedron);
        if (result == 0)
        {
            polyhedron->owner->polyhedronCount++;
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
            polyhedron = 0;
        }
    }
    return polyhedron;
}

// FUNCTION: EQGFX 0x10018D49
// Copies a polyhedron definition, resets its key, flags, and type, then registers it with the context.
t3dPolyhedron_guess *__cdecl t3dDuplicatePolyhedron(void *context, t3dPolyhedron_guess *source)
{
    t3dPolyhedron_guess *duplicate;
    unsigned int *sourceWord;
    unsigned int *duplicateWord;
    int wordCount;
    int flagEnabled;
    int result;

    duplicate = (t3dPolyhedron_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x1c);
    if (duplicate != 0)
    {
        sourceWord = (unsigned int *)source;
        duplicateWord = (unsigned int *)duplicate;
        for (wordCount = 7; wordCount != 0; wordCount = wordCount - 1)
        {
            *duplicateWord = *sourceWord;
            sourceWord = sourceWord + 1;
            duplicateWord = duplicateWord + 1;
        }
        duplicate->dictionaryKey = 0;
        flagEnabled = EQGFX_WORLD_LOAD_MODE != 0;
        duplicate->type = 0;
        *(unsigned int *)&duplicate->flags = -(unsigned int)flagEnabled & 0x10;
        result = EQ_CDECL(int, 0x1001564B, (void *, t3dPolyhedron_guess *))(context, duplicate);
        if (result == 0)
        {
            duplicate->owner->polyhedronCount++;
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(duplicate);
            duplicate = 0;
        }
    }
    return duplicate;
}

// FUNCTION: EQGFX 0x10018DA3
// Unregisters a polyhedron from its owner and releases the object.
// __cdecl, int, two stack arguments (context, polyhedron). Unlinks and frees one polyhedron, returns the result of t3dFreeObjectPoolSlot_guess.
int __cdecl t3dDestroyPolyhedron(void *context, t3dPolyhedron_guess *polyhedron)
{
    int result;

    polyhedron->owner->polyhedronCount--;
    if ((polyhedron->flags & 1) != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(context, polyhedron->dictionaryKey);
    }
    result = EQ_CDECL(int, 0x10015D81, (void *, void *))(context, polyhedron);
    if (polyhedron->dictionaryKey != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
    }
    EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron);
    return result;
}

#define T3D_POLYHEDRON_TAG_ALLOC_FAILED 0xFFFF3D3E

// FUNCTION: EQGFX 0x10018DEB
// __cdecl; replaces a polyhedron's dictionary key and returns the allocation error or zero.
int __cdecl t3dSetPolyhedronTag(void *context, t3dPolyhedron_guess *polyhedron, char *tag)
{
    int result;
    void *tagCopy;
    unsigned int wasInDictionary;

    result = 0;
    if (tag == 0)
    {
        tagCopy = 0;
    }
    else
    {
        tagCopy = EQ_CDECL(void *, 0x10050C48, (unsigned int))(
            strlen_EQGFX(tag) + 1);
        if (tagCopy == 0)
        {
            result = T3D_POLYHEDRON_TAG_ALLOC_FAILED;
            goto done;
        }
    }

    wasInDictionary = polyhedron->flags & 1;
    if (wasInDictionary != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(context, polyhedron->dictionaryKey);
    }
    if (polyhedron->dictionaryKey != 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(polyhedron->dictionaryKey);
    }
    if (tag == 0)
    {
        polyhedron->dictionaryKey = 0;
    }
    else
    {
        polyhedron->dictionaryKey = tagCopy;
        GFX_PATH_COPY_STRING((char *)tagCopy, tag);
        if (wasInDictionary != 0 &&
            EQGFX_T3D_GET_POINTER_FROM_DICTIONARY_GUESS(context, tag) == 0)
        {
            parseAddToDictionary(context, polyhedron->dictionaryKey);
        }
    }

done:
    return result;
}

// FUNCTION: EQGFX 0x10018e89
// Reads the renderer actor's world-space Y coordinate.
// __cdecl (one stack argument, callee pops nothing). Returns the renderer actor's world Y, the float at
// offset 0x14. No t3d actor struct is in include/ yet, so the field stays a raw offset.
float __cdecl t3dGetActorWorldY(void *actor)
{
    return *(float *)((char *)actor + 0x14);
}

// FUNCTION: EQGFX 0x10018e91
// Collect a polyhedron definition's unique edges as pairs of vertex positions.
// __cdecl, int, three stack arguments (polyhedron, edgeCount, edgeList). Writes the distinct edges of the element blocks, each as its two
// vertices, to a new block at *edgeList and their count to *edgeCount; returns 0, or 0xFFFF3D3E when the key block allocation fails.
int __cdecl t3dGetPolyhedronDefinitionEdgeList(t3dPolyhedron_guess *polyhedron, int *edgeCount, t3dEdge_guess **edgeList)
{
    int result;
    int count;
    int keyIndex;
    int offset;
    t3dPolyhedronElement_guess *element;
    t3dPolyhedronElement_guess *elementsEnd;
    t3dPolygonEdge_guess *edge;
    t3dPolygonEdge_guess *edgeEnd;
    unsigned int *edgeKeys;
    unsigned int *keyEnd;
    unsigned int *keyCursor;
    unsigned int *lastKey;

    *edgeList = 0;
    *edgeCount = 0;
    count = 0;
    element = polyhedron->elements;
    elementsEnd = element + (int)polyhedron->owner;
    result = 0xffff3d3e;
    for (; element < elementsEnd; element++)
    {
        count += element->edgeCount;
    }
    edgeKeys = (unsigned int *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(count << 3);
    if (edgeKeys != 0)
    {
        keyIndex = 0;
        for (element = polyhedron->elements; element < elementsEnd; element++)
        {
            edge = (t3dPolygonEdge_guess *)element->block;
            edgeEnd = edge + element->edgeCount;
            if (edge < edgeEnd)
            {
                keyCursor = edgeKeys + keyIndex;
                do
                {
                    if (edge->vertexA < edge->vertexB)
                    {
                        keyCursor[0] = (unsigned int)edge->vertexA;
                        keyCursor[1] = (unsigned int)edge->vertexB;
                    }
                    else
                    {
                        keyCursor[0] = (unsigned int)edge->vertexB;
                        keyCursor[1] = (unsigned int)edge->vertexA;
                    }
                    edge++;
                    keyIndex += 2;
                    keyCursor += 2;
                } while (edge < edgeEnd);
            }
        }
        EQGFX_QSORT_GUESS(edgeKeys, count, 8, EQGFX_T3D_COMPARE_EDGE_KEYS_GUESS);
        keyEnd = edgeKeys + keyIndex;
        lastKey = edgeKeys;
        count = 1;
        for (keyCursor = edgeKeys + 2; keyCursor < keyEnd; keyCursor += 2)
        {
            if (keyCursor[0] == lastKey[0] && keyCursor[1] == lastKey[1])
            {
                keyCursor[0] = 0;
            }
            else
            {
                count++;
                lastKey = keyCursor;
            }
        }
        *edgeList = (t3dEdge_guess *)EQGFX_T3D_ALLOCATE_MEMORY_GUESS(count * 0x18);
        if (edgeList != 0)
        {
            if (edgeKeys < keyEnd)
            {
                offset = 0;
                for (keyCursor = edgeKeys; keyCursor < keyEnd; keyCursor += 2)
                {
                    if (keyCursor[0] != 0)
                    {
                        *(t3dVertex_guess *)((char *)*edgeList + offset) = *(t3dVertex_guess *)keyCursor[0];
                        *(t3dVertex_guess *)((char *)*edgeList + offset + 0x0c) = *(t3dVertex_guess *)keyCursor[1];
                        offset += 0x18;
                    }
                }
            }
            result = 0;
            *edgeCount = count;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(edgeKeys);
    }
    return result;
}

// FUNCTION: EQGFX 0x10019012
// Prepares an empty reference list for world and zone queries.
int __cdecl t3dInitReferenceList(t3dReferenceList_guess *list, int capacity)
{
    int result;
    void *allocatedReferences;

    result = 0;
    list->count = 0;
    list->capacity = 0;
    list->references = 0;
    list->unknown_00 = 0x3c;
    allocatedReferences = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(capacity << 2);
    if (allocatedReferences == 0)
    {
        result = T3D_REFERENCE_LIST_ALLOC_FAILED;
    }
    else
    {
        list->references = (unsigned int *)allocatedReferences;
        list->capacity = capacity;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001905a
// Empties a reference list so world or zone queries can reuse it.
// __cdecl (one stack argument, callee pops nothing), int, returns 0. Clears the dword at offset 0x4 of the
// reference list `list`. No struct for this list is in include/ yet, so the field stays a raw offset.
int __cdecl t3dClearReferenceList(void *list)
{
    *(unsigned int *)((char *)list + 0x4) = 0;
    return 0;
}

// FUNCTION: EQGFX 0x10019065
// Adds one object reference to a world or zone query list.
// __cdecl (two stack arguments, callee pops nothing), int, returns 0 or 0xFFFF3D3E. Appends `reference` to the list;
// a full list first grows to capacity + capacity / 4 + 1 slots, and when that reallocation fails the list is unchanged.
int __cdecl t3dAddReferenceToList(unsigned int reference, t3dReferenceList_guess *list)
{
    int result;
    unsigned int new_capacity;
    unsigned int *new_references;

    result = 0;
    if ((unsigned int)list->count >= (unsigned int)list->capacity) {
        new_capacity = (unsigned int)list->capacity + ((unsigned int)list->capacity >> 2) + 1;
        new_references = (unsigned int *)EQGFX_T3D_REALLOCATE_MEMORY_GUESS(list->references, new_capacity * 4);
        if (new_references == 0) {
            result = T3D_REFERENCE_LIST_ALLOC_FAILED;
        } else {
            list->references = new_references;
            list->capacity = new_capacity;
        }
    }
    if (result == 0) {
        list->references[list->count] = reference;
        list->count++;
    }
    return result;
}

// FUNCTION: EQGFX 0x100190b6
// Finds a reference in a sorted query list or returns where it belongs.
// __cdecl (three stack arguments, callee pops nothing). Binary-searches the sorted reference list `list`
// (unsigned reference count at +0x4, array of references at +0xc) for `reference`: on a hit writes its
// index to `index_out` and returns 1, otherwise writes the index it belongs at and returns 0.
// The file-local t3dReferenceList_guess represents the list layout.
int __cdecl t3dFindReferenceInSortedList(unsigned int reference, t3dReferenceList_guess *list, int *index_out)
{
    unsigned int *references;
    unsigned int value;
    int lo;
    int hi;
    int mid;

    lo = 0;
    hi = list->count;
    if (hi > 0) {
        references = list->references;
        while (hi > lo) {
            mid = (hi + lo) >> 1;
            value = references[mid];
            if (value == reference) {
                *index_out = mid;
                return 1;
            }
            if (value > reference)
                hi = mid;
            else
                lo = mid + 1;
        }
    }
    *index_out = lo;
    return 0;
}

// FUNCTION: EQGFX 0x100190f9
// Inserts a reference at its requested position in a world or zone query list.
unsigned int __cdecl t3dInsertReferenceIntoList(unsigned int reference, t3dReferenceList_guess *list, unsigned int index)
{
    unsigned int result;
    unsigned int newCapacity;
    unsigned int count;
    unsigned int *references;
    unsigned int *element;

    result = 0;
    if ((unsigned int)list->count >= (unsigned int)list->capacity)
    {
        newCapacity = ((unsigned int)list->capacity >> 2) + 1 + (unsigned int)list->capacity;
        references = (unsigned int *)EQGFX_T3D_REALLOCATE_MEMORY_GUESS(list->references, newCapacity * 4);
        if (references == 0)
        {
            result = T3D_REFERENCE_LIST_ALLOC_FAILED;
            goto insert_reference_done;
        }
        list->references = references;
        list->capacity = newCapacity;
    }
    count = list->count;
    if (index < count)
    {
        element = list->references + index;
        EQ_CDECL(void *, 0x100679F0, (void *, const void *, unsigned int))
            (element + 1, element, (index * T3D_REFERENCE_NEGATIVE_INDEX_FACTOR + count) * 4);
        *element = reference;
    }
    else
    {
        list->references[count] = reference;
    }
    list->count++;
insert_reference_done:
    return result;
}

// FUNCTION: EQGFX 0x10019179
// Removes one reference from a world or zone query list.
// __cdecl. Removes the reference at `index` from the list (unsigned reference count at +0x4, array of
// references at +0xc) by moving the tail down one slot with memcpy; returns 0, or 0xFFFF3D3E when out of range.
unsigned int __cdecl t3dRemoveElementFromReferenceList(unsigned int index, void *list)
{
    unsigned int result;
    unsigned int lastIndex;
    unsigned int *references;
    unsigned int *element;

    result = 0;
    if (index < *(unsigned int *)((char *)list + 0x4))
    {
        lastIndex = *(unsigned int *)((char *)list + 0x4) - 1;
        *(unsigned int *)((char *)list + 0x4) = lastIndex;
        if (index < lastIndex)
        {
            references = *(unsigned int **)((char *)list + 0xc);
            element = &references[index];
            EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))
                (element, element + 1, (lastIndex - index) * 4);
        }
    }
    else
    {
        result = 0xFFFF3D3E;
    }
    return result;
}

// FUNCTION: EQGFX 0x100191bd
// Releases a reference list used by world or zone queries.
// __cdecl (one stack argument, callee pops nothing), void (callers never read EAX after the call). Frees the
// reference array at +0xc through t3dFreeMemory (0x10050CBB) and clears the list: count (+0x4), spare (+0x8)
// and the array pointer (+0xc). No struct for this list is in include/ yet, so the fields stay raw offsets.
void __cdecl t3dFreeReferenceList(void *list)
{
    EQGFX_T3D_FREE_MEMORY_GUESS(*(void **)((char *)list + 0xc));
    *(unsigned int *)((char *)list + 0x4) = 0;
    *(unsigned int *)((char *)list + 0x8) = 0;
    *(unsigned int *)((char *)list + 0xc) = 0;
}

// The region BSP tree hung off the list at +0xc: a node is a plane (four floats) with a positive and a negative
// child (+0x10/+0x14) and an int region at +0x18 (0 = interior node, a leaf stores its region number + 1).
typedef struct t3dRegionNode_guess
{
    float plane_nx_guess;                       // 0x00
    float plane_ny_guess;                       // 0x04
    float plane_nz_guess;                       // 0x08
    float plane_d_guess;                        // 0x0c
    struct t3dRegionNode_guess *positive_guess; // 0x10, taken when dot >= -plane_d
    struct t3dRegionNode_guess *negative_guess; // 0x14, taken when dot < -plane_d
    int region_plus_one_guess;                  // 0x18
} t3dRegionNode_guess;

// The region list itself: the region BSP tree hangs off it at +0xc (t3dRegionNode_guess above), the number of
// regions sits at +0x18 and the array of that many region pointers at +0x24.
typedef struct t3dRegionList_guess
{
    unsigned char reserved_0000[0x18];  // 0x00..0x17
    unsigned int region_count;          // 0x18
    unsigned char reserved_001c[0x08];  // 0x1c..0x23
    void **regions;                     // 0x24
} t3dRegionList_guess;

// FUNCTION: EQGFX 0x100191D8
// Release a world object's owned data and region references; its precise role is unclear.
// __cdecl (two stack arguments, callee pops nothing), int (always 0). Helper 0x10009237 first when +0x34 is set, then
// the dictionary entry (flags bit 0), the payload's owner count, the region set, and the payload unless flags bit 5.
int __cdecl cleanupWorldObject_guess(void *world, t3dWorldObject_guess *worldObject)
{
    t3dWorldPayload_guess *payload;
    t3dRegionSet_guess *regions;
    void *block;

    if (worldObject->helperPending != 0)
    {
        EQ_CDECL(void, 0x10009237, (void *, void *))(world, worldObject);
    }
    if ((worldObject->flags & 1) != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(world, worldObject->dictionaryKey);
    }
    payload = worldObject->payload;
    if (payload != 0 && payload->tag == 0x25 && payload->owner != 0)
    {
        payload->owner->polyhedronCount--;
    }
    regions = worldObject->regions;
    if (regions != 0)
    {
        switch (regions->tag)
        {
        case 0x27:
        case 0x29:
            block = regions->firstBlock;
            if (block != 0)
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(block);
            }
            block = regions->secondBlock;
            if (block != 0)
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(block);
            }
            EQ_CDECL(void, 0x100191BD, (void *))(regions->referenceList);
            break;
        }
    }
    if ((worldObject->flags & 0x20) == 0)
    {
        EQGFX_T3D_FREE_MEMORY_GUESS(worldObject->payload);
    }
    return 0;
}

// FUNCTION: EQGFX 0x10019266
// Sends a message to each actor associated with a region set.
// __cdecl (three stack arguments, callee pops nothing), uint. For a region set tagged 0x27 or 0x29, sends message to the actor
// of each int entry in its first block (count at +0x04), then returns the entry count; any other tag returns 0xFFFFFFFF.
unsigned int __cdecl t3dSendMessageToAllRegionActors(t3dWorld_guess *world, t3dWorldObject_guess *worldObject, unsigned int message)
{
    t3dRegionSet_guess *regions;
    int *entry;
    int *entryEnd;
    t3dRegionActor_guess *actor;
    unsigned int result;

    regions = worldObject->regions;
    result = 0;
    switch (regions->tag)
    {
    case 0x27:
    case 0x29:
        entry = (int *)regions->firstBlock;
        entryEnd = entry + *(int *)regions->unknown_04;
        if (entry < entryEnd)
        {
            result = ((unsigned int)entryEnd - (unsigned int)entry + 3) >> 2;
            do
            {
                actor = world->regionActors[*entry];
                ((t3dRegionMessage_guess)actor->vtable[8])(message, world, actor);
                entry++;
            } while (entry < entryEnd);
        }
        break;
    default:
        result = 0xffffffff;
        break;
    }
    return result;
}

// FUNCTION: EQGFX 0x100192c4
// Finds the region number containing a world-space position.
// __cdecl (two stack arguments, callee pops nothing), int. Walks the BSP tree at regionList+0xc down to the
// leaf bracketing the point xyz (dot(normal, xyz) < -d takes the negative child) and returns its region number.
int __cdecl t3dGetRegionNumberFromWorldAndXYZ(void *regionList, float *worldXYZ)
{
    const float x = worldXYZ[0];
    const float y = worldXYZ[1];
    const float z = worldXYZ[2];
    t3dRegionNode_guess *node = *(t3dRegionNode_guess **)((char *)regionList + 0xc);

    while (node != 0 && node->region_plus_one_guess == 0)
    {
        if (!(x * node->plane_nx_guess + y * node->plane_ny_guess + z * node->plane_nz_guess < -node->plane_d_guess))
            node = node->positive_guess;
        else
            node = node->negative_guess;
    }
    if (node != 0)
        return node->region_plus_one_guess - 1;
    return -1;
}

// FUNCTION: EQGFX 0x10019324
// Finds the region object containing a world-space position.
// __cdecl (two stack arguments, callee pops nothing). Walks the region BSP tree for the point and returns that
// region, or 0 when the (unsigned) region number is not below the region count at +0x18.
void *__cdecl t3dGetRegionFromWorldAndXYZ(t3dRegionList_guess *regionList, float *worldXYZ)
{
    unsigned int regionNumber = EQ_CDECL(int, 0x100192C4, (void *, float *))(regionList, worldXYZ);

    if (regionNumber < regionList->region_count)
        return regionList->regions[regionNumber];
    return 0;
}

// FUNCTION: EQGFX 0x10019346
// Looks up a region object by its region number.
// __cdecl (two stack arguments, callee pops nothing), returns the region stored at regionList[regionNumber]
// (the region array hangs off the list at +0x24). No struct for this list is in include/ yet, so 0x24 stays raw.
void *__cdecl t3dGetRegionFromRegionNumber(void *regionList, int regionNumber)
{
    return *(void **)(*(unsigned int *)((char *)regionList + 0x24) + regionNumber * 4);
}

// FUNCTION: EQGFX 0x10019355
// __cdecl (one stack argument, callee pops nothing), returns the edge count stored at +0x04 of the polygon.
int __cdecl t3dGetEdgePolygonNumEdges(t3dPolygon_guess *polygon)
{
    return polygon->edgeCount;
}
