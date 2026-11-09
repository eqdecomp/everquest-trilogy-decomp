// unit_10010743.c - eqgfx_dx7.dll, the address run 0x10010743..0x10011A8E (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include <string.h>
#include <stdio.h>
#include "trilogy/eqaddr.h"

#define EQGFX_VERTEX_CACHE_ALLOCATION_ERROR ((const char *)0x100864E4)
#define EQGFX_SKIN_VERTEX_CACHE_ALLOCATION_ERROR ((const char *)0x1008652C)
#define EQGFX_OUTPUT_DEBUG_STRING_A (*(void (__stdcall **)(const char *))0x100760C4)

#define EQGFX_DMSPRITE_ALLOCATE(bytes) EQ_CDECL(void *, 0x10050C48, (int))(bytes)
#define EQGFX_DMSPRITE_FREE(memory) EQ_CDECL(void, 0x10050CBB, (void *))(memory)
#define EQGFX_DMSPRITE_STRLEN(text) EQ_CDECL(size_t, 0x10068160, (const char *))(text)
#define EQGFX_DMSPRITE_COPY_STRING(destination, source) EQ_CDECL(void, 0x10068070, (char *, const char *))(destination, source)
#define EQGFX_DMSPRITE_MEMCPY(destination, source, size) EQ_CDECL(void *, 0x10067D30, (void *, const void *, size_t))(destination, source, size)
#define EQGFX_DMSPRITE_DICTIONARY_CHECK(dictionary, track) EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, track)
#define EQGFX_DMSPRITE_ADD_TO_DICTIONARY(dictionary, track) EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, track)
#define EQGFX_DMSPRITE_DICTIONARY_UNDEFINE(dictionary, track) EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, track)
#define EQGFX_DMSPRITE_FPRINTF EQ_CDECL(int, 0x100683A5, (FILE *, const char *, ...))
#define EQGFX_DMSPRITE_LIGHT_PATCH_HEADER ((const char *)0x10086660)
#define EQGFX_DMSPRITE_TRACKDEF_HEADER ((const char *)0x10086648)
#define EQGFX_DMSPRITE_TAG_FORMAT ((const char *)0x10086638)
#define EQGFX_DMSPRITE_VERTEX_COUNT_FORMAT ((const char *)0x10086624)
#define EQGFX_DMSPRITE_FRAME_COUNT ((const char *)0x10086610)
#define EQGFX_DMSPRITE_SLEEP ((const char *)0x10086600)
#define EQGFX_DMSPRITE_FRAME_HEADER ((const char *)0x100865E4)
#define EQGFX_DMSPRITE_RGBA_COUNT_FORMAT ((const char *)0x100865D0)
#define EQGFX_DMSPRITE_RGBA_FORMAT ((const char *)0x100865A8)
#define EQGFX_DMSPRITE_FRAME_END ((const char *)0x1008658C)
#define EQGFX_DMSPRITE_TRACKDEF_END ((const char *)0x10086570)

typedef struct DMSpriteFrameIndex_guess
{
    int frame_position;                // +0x0
    int frame_index;                   // +0x4
    unsigned int elapsed_time;         // +0x8
    int elapsed_cycles;                // +0xc
    int cycle_limit;                   // +0x10
    float frame_rate;                  // +0x14
    int finished;                      // +0x18
    unsigned int time_divisor;         // +0x1c
} DMSpriteFrameIndex_guess;

typedef struct DMSpriteDefResortVertexGroup_guess
{
    unsigned short unknown_0;
    unsigned short vertex_0;
    unsigned short vertex_1;
    unsigned short vertex_2;
} DMSpriteDefResortVertexGroup_guess;

#pragma pack(push, 1)
typedef struct DMSpriteDefResortVertex_guess
{
    unsigned short unknown_0;
    unsigned short unknown_2;
    unsigned short unknown_4;
    float unknown_6;
    float unknown_a;
    unsigned short unknown_e;
    unsigned char rgba[4];
} DMSpriteDefResortVertex_guess;
#pragma pack(pop)

typedef struct DMSpriteDefResortDefinition_guess
{
    unsigned short vertex_count;
    unsigned char unknown_2[6];
    unsigned short group_count;
    unsigned short unknown_a;
    int unknown_c;
    int unknown_10;
    DMSpriteDefResortVertex_guess *vertices;
    DMSpriteDefResortVertexGroup_guess *vertex_groups;
    int unknown_1c;
    unsigned short *color_runs;
} DMSpriteDefResortDefinition_guess;

typedef struct DMSpriteDefResortObject_guess
{
    int unknown_0[2];
    const char *tag;
    int unknown_c[3];
    DMSpriteDefResortDefinition_guess *definition;
} DMSpriteDefResortObject_guess;

typedef struct DMSpriteVertexCache_guess
{
    int vertexCount;                         // +0x0
    float *sourceVertices;                   // +0x4
    int unknown_8[16];                       // +0x8 .. +0x44
    unsigned short *skinVertexRuns;           // +0x48
    int unknown_4c[20];                      // +0x4c .. +0x98
    int lastVertexMarker;                    // +0x9c
    int unknown_a0;                          // +0xa0
    float *transformedVertices;              // +0xa4
} DMSpriteVertexCache_guess;

typedef struct DMSpriteVertexCacheOwner_guess
{
    int unknown_0[5];                        // +0x0 .. +0x10
    DMSpriteVertexCache_guess *vertexCache;  // +0x14
} DMSpriteVertexCacheOwner_guess;

typedef struct DMSpriteVertexCacheSourceList_guess
{
    int unknown_0;                           // +0x0
    int sourceCount;                          // +0x4
    float **sourceVertices;                  // +0x8
} DMSpriteVertexCacheSourceList_guess;

typedef struct DMSpriteVertexCacheSourceHolder_guess
{
    int unknown_0[5];                        // +0x0 .. +0x10
    DMSpriteVertexCacheSourceList_guess *sourceList; // +0x14
} DMSpriteVertexCacheSourceHolder_guess;

typedef struct DMSpriteVertexCacheDetail_guess
{
    int unknown_0[6];                        // +0x0 .. +0x14
    int sourceIndex;                          // +0x18
    int unknown_1[6];                        // +0x1c .. +0x30
    DMSpriteVertexCacheSourceHolder_guess *sourceHolder; // +0x34
} DMSpriteVertexCacheDetail_guess;

typedef struct DMSpriteVertexCacheSprite_guess
{
    int unknown_0[5];                        // +0x0 .. +0x10
    float x;                                  // +0x14
    float y;                                  // +0x18
    float z;                                  // +0x1c
    int unknown_20;                           // +0x20
    DMSpriteVertexCacheDetail_guess *detail; // +0x24
    int unknown_28[3];                       // +0x28 .. +0x30
    DMSpriteVertexCacheOwner_guess *cacheOwner; // +0x34
} DMSpriteVertexCacheSprite_guess;

typedef struct DMSkinSpriteTransform_guess
{
    float matrix[12];                         // +0x0 .. +0x2c
    char unknown_30[0x118];                  // +0x30 .. +0x147
} DMSkinSpriteTransform_guess;              // 0x148

typedef struct DMSkinSpriteTransformBank_guess
{
    char unknown_0[0x5c];                    // +0x0 .. +0x5b
    DMSkinSpriteTransform_guess transforms[1]; // +0x5c
} DMSkinSpriteTransformBank_guess;

typedef struct DMSkinSpriteTransformOwner_guess
{
    int unknown_0[9];                        // +0x0 .. +0x23
    DMSkinSpriteTransformBank_guess *transformBank; // +0x24
} DMSkinSpriteTransformOwner_guess;

#define DMTRACK_UNDEFINE_IN_USE_RESULT 0xffff3d3e

typedef struct DMTrackDef2_guess
{
    unsigned int signature;
    unsigned int unknown_04;
    char *name;
    unsigned int unknown_0c;
    unsigned int unknown_10;
    unsigned int unknown_14;
    void *track_header;
    unsigned short count_a;
    unsigned short count_b;
    unsigned short track_value_a;
    unsigned short track_value_b;
    unsigned int track_value_c;
    void *track_data;
    char payload[1];
} DMTrackDef2_guess;

typedef struct DMTrackCreateContext_guess
{
    int unknown_0[2];
    int contextValue;                     // +0x8
} DMTrackCreateContext_guess;             // 0xc

typedef struct DMTrackPrimarySource_guess
{
    int unknown_0[3];
    int trackValue;                       // +0xc
    int sourceValue;                      // +0x10
} DMTrackPrimarySource_guess;             // 0x14

typedef struct DMTrackFallbackSource_guess
{
    int unknown_0;
    unsigned short trackValue;            // +0x4
    unsigned short unknown_6;
    int sourceValue;                      // +0x8
} DMTrackFallbackSource_guess;            // 0xc

typedef struct DMTrackCreateDefinition_guess
{
    int unknown_0[5];
    DMTrackPrimarySource_guess *primarySource;   // +0x14
    DMTrackFallbackSource_guess *fallbackSource; // +0x18
} DMTrackCreateDefinition_guess;                 // 0x1c

typedef struct DMTrackValid_guess
{
    int magic;                            // +0x0
    int unknown_4[4];                     // +0x4
    int primaryTrackValue;                // +0x14
    int secondaryTrackValue;              // +0x18
    int contextValue;                     // +0x1c
    int unknown_20;                       // +0x20
    int parameter24;                      // +0x24
    int parameter28;                      // +0x28
    int unknown_2c;                       // +0x2c
    int sourceValue;                      // +0x30
    DMTrackCreateDefinition_guess *definition; // +0x34
} DMTrackValid_guess;                     // 0x38

typedef struct DMTrackValues_guess
{
    int values[8];                        // +0x14..+0x30 (primaryTrackValue..sourceValue)
} DMTrackValues_guess;                    // 0x20

typedef struct DMTrackOwnedBlock_guess
{
    int unknown_0[2];                  // +0x0
    void **owned_data;                 // +0x8
} DMTrackOwnedBlock_guess;             // 0xc

typedef struct DMTrackInstance_guess
{
    int unknown_0[2];                  // +0x0
    void *dictionary_key;              // +0x8
    unsigned char flags;               // +0xc
    unsigned char unknown_d[3];        // +0xd
    int use_count;                     // +0x10
    DMTrackOwnedBlock_guess *owned_block; // +0x14
} DMTrackInstance_guess;               // 0x18

typedef struct DMRGBTrackValid_guess
{
    int magic;                            // +0x0
    int unknown_4;                        // +0x4
    void *dictionaryKey;                  // +0x8
    int flags;                             // +0xc
    int useCount;                         // +0x10
    int unknown_14[2];                    // +0x14
    void **ownedData;                      // +0x1c
    int unknown_20[5];                    // +0x20
    void *sharedObject;                   // +0x34
} DMRGBTrackValid_guess;                  // 0x38

typedef struct DMRGBTrackValues_guess
{
    int values[8];                        // +0x14..+0x33 (unknown_14 through unknown_20[4])
} DMRGBTrackValues_guess;                 // 0x20

#define DMRGB_TRACK_FLAG_IN_DICT 0x01

typedef struct DMRGBTrackCreateContext_guess
{
    int unknown_0[2];
    void **ownedData;
} DMRGBTrackCreateContext_guess;

typedef struct DMRGBTrackCreateSource_guess
{
    int unknown_0[8];
    int value_20;
    int value_24;
} DMRGBTrackCreateSource_guess;

// The object t3dGetEdgePolygonEdgeVertex2 reads (not in include/: the layout is guessed from the asm
// and marked). A u32 count at +0x4, a table of 0x14-byte edge entries at +0x8; an entry stores
// a pointer to its three-float vertex at +0x8.
typedef struct T3DEdgeVertex_guess
{
    float x;                            // +0x0
    float y;                            // +0x4
    float z;                            // +0x8
} T3DEdgeVertex_guess;                  // 0xc

typedef struct T3DEdge_guess
{
    int unknown_0[2];                   // +0x0
    T3DEdgeVertex_guess *vertex;        // +0x8
    float length;                       // +0xc
    int unknown_10;                     // +0x10
} T3DEdge_guess;                        // 0x14

typedef struct T3DEdgePolygon_guess
{
    int unknown_0;                      // +0x0
    unsigned int count;                 // +0x4
    T3DEdge_guess *edges;               // +0x8
    T3DEdgeVertex_guess normal;         // +0xc
    float dist;                         // +0x18
} T3DEdgePolygon_guess;

// The object t3dGetEdgePolygonEdgeVertex1 reads (not in include/: the layout is guessed from the
// asm). A u32 count at +0x4, a table of 0x14-byte edge entries at +0x8; an entry stores
// a pointer to its three-float vertex at +0x4.
typedef struct T3DEdgePoly1_guess
{
    int unknown_0;                        // +0x0
    T3DEdgeVertex_guess *vertex;          // +0x4
    int unknown_8[3];                     // +0x8
} T3DEdgePoly1_guess;                     // 0x14

typedef struct T3DEdgePolygon1_guess
{
    int unknown_0;                        // +0x0
    unsigned int count;                   // +0x4
    T3DEdgePoly1_guess *edges;             // +0x8
    T3DEdgeVertex_guess normal;           // +0xc
    float dist;                           // +0x18
} T3DEdgePolygon1_guess;

// The object s3dGetDMSpriteDefinitionNumVertices reads (not in include/: the layout is guessed from the
// asm). A u32 count pointer at +0x14, a u16 count pointer at +0x18.
typedef struct DMSpriteDefinition_guess
{
    int unknown_0[5];                       // +0x0
    unsigned int *num_vertices_ptr;         // +0x14
    unsigned short *num_vertices16_ptr;     // +0x18
} DMSpriteDefinition_guess;                 // 0x1c

// The object s3dPatchDMTrackDefLighting reads (not in include/: the layout is guessed from the
// asm). A u32 count at +0x14, a pointer to the u32 lighting buffer
// pointer at +0x1c.
typedef struct DMTrackDefLighting_guess
{
    int unknown_0[5];                       // +0x0
    unsigned int count;                     // +0x14
    int unknown_18;                         // +0x18
    unsigned int **lighting_dest_ptr;       // +0x1c
} DMTrackDefLighting_guess;                 // 0x20

// The objects s3dPatchDMSpriteDefLighting reads (not in include/: the layout is guessed from
// the asm). The sprite-def param carries a 32-bit lighting object
// pointer at +0x14 and a 16-bit lighting object pointer at +0x18; the 32-bit object holds a u32
// count at +0x18 and the u32 lighting buffer at +0x1c, the 16-bit object a u16 count at +0x0
// and a table of 0x14-byte entries at +0x14 with the 4 lighting bytes at +0x10.
typedef struct DMSprite32Lighting_guess
{
    int unknown_0[6];                       // +0x0
    unsigned int count;                     // +0x18
    unsigned int *lighting_dest;            // +0x1c
} DMSprite32Lighting_guess;                 // 0x20

typedef struct DMSprite16Entry_guess
{
    unsigned char unknown_0[0x10];          // +0x0
    unsigned char lighting[4];              // +0x10
} DMSprite16Entry_guess;                    // 0x14

typedef struct DMSprite16Lighting_guess
{
    unsigned short count;                   // +0x0
    unsigned char unknown_2[0x12];          // +0x2
    DMSprite16Entry_guess *entries;         // +0x14
} DMSprite16Lighting_guess;                 // 0x18

typedef struct DMSpriteDefLightingPatch_guess
{
    int unknown_0[5];                               // +0x0
    DMSprite32Lighting_guess *lighting32;           // +0x14
    DMSprite16Lighting_guess *lighting16;           // +0x18
} DMSpriteDefLightingPatch_guess;                   // 0x1c

// The objects s3dAdjustDMSpriteMeshDetail2 reads (not in include/: the layout is guessed from the
// asm). The holder carries the detail object at +0x18; the detail object
// holds a u16 entry count at +0x10, the 8-byte swap table at +0x18, the 6-byte entry array at +0x28,
// the current index at +0x38 and two flag byte arrays at +0x78/+0x7c. Each entry is 6 bytes: two
// u16 vertex indices at +0x0/+0x2 (a float distance when kind is 4), a sub index at +0x4 and a kind
// byte at +0x5 (1 = swap, 2/3 = flag, 4 = distance marker).
typedef struct DMSpriteMeshEntry_guess
{
    unsigned short vert0;                 // +0x0 (float distance when kind == 4)
    unsigned short vert1;                 // +0x2
    unsigned char blend;                  // +0x4
    unsigned char kind;                   // +0x5
} DMSpriteMeshEntry_guess;                // 0x6

typedef struct DMSpriteMeshSwap_guess
{
    unsigned short unused;                // +0x0
    unsigned short slot[3];               // +0x2
} DMSpriteMeshSwap_guess;                 // 0x8

typedef struct DMSpriteMeshDetail_guess
{
    int unknown_0[4];                     // +0x0
    unsigned short total;                 // +0x10
    unsigned char unknown_12[6];          // +0x12
    DMSpriteMeshSwap_guess *swap_table;   // +0x18
    int unknown_1c[3];                    // +0x1c
    DMSpriteMeshEntry_guess *entries;     // +0x28
    int unknown_2c[3];                    // +0x2c
    int cur;                              // +0x38
    int unknown_3c[15];                   // +0x3c
    unsigned char *flags0;                // +0x78
    unsigned char *flags1;                // +0x7c
} DMSpriteMeshDetail_guess;               // 0x80

typedef struct DMSpriteMeshHolder_guess
{
    int unknown_0[6];                     // +0x0
    DMSpriteMeshDetail_guess *detail;     // +0x18
} DMSpriteMeshHolder_guess;               // 0x1c

// The object s3dIsDMTrackDefinitionValid reads and s3dDefineDMTrack builds (not in include/: the layout is guessed
// from the asm). A magic id at +0x0 (0x49 when valid).
typedef struct DMTrackDefinitionValid_guess
{
    int magic;                            // +0x0
    int unknown_4;                        // +0x4
    char *track_name;                     // +0x8
    int unknown_c;                        // +0xc
    int unknown_10;                       // +0x10
    int *header;                          // +0x14, points at header_words
    int unknown_18;                       // +0x18
    int header_words[5];                  // +0x1c: vertex count, frame count, frame table, two more words
} DMTrackDefinitionValid_guess;           // 0x30

#define DMSPRITE_TIMING_SKIP_FLAG 0x08

// The objects s3dUpdateDMSpriteTiming reads (not in include/: the layout is guessed from the
// asm). The container holds two sprites at +0x24/+0x28.
typedef struct DMSpriteTimingSprite_guess
{
    int unknown_0[3];                     // +0x0
    unsigned char flags;                  // +0xc
    unsigned char unknown_d[3];            // +0xd
    int unknown_10;                       // +0x10
    DMSpriteFrameIndex_guess timing;      // +0x14
    void *definition;                     // +0x34
} DMSpriteTimingSprite_guess;              // 0x38

typedef struct DMSpriteTimingContainer_guess
{
    int unknown_0[9];                     // +0x0
    DMSpriteTimingSprite_guess *first;    // +0x24
    DMSpriteTimingSprite_guess *second;   // +0x28
} DMSpriteTimingContainer_guess;           // 0x2c

// The second definition s3dUpdateDMSpriteTiming reads (not in include/: the layout is guessed
// from the asm). The first definition is a DMTrackDefinition_guess;
// this one holds its frame count at +0x18.
typedef struct DMSpriteTimingSecondDef_guess
{
    int unknown_0[6];                     // +0x0
    unsigned int frameCount;              // +0x18
} DMSpriteTimingSecondDef_guess;           // 0x1c

// FUNCTION: EQGFX 0x10010743
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0 when valid, 0xffff3d3e otherwise.
int __cdecl s3dIsDMTrackDefinitionValid(DMTrackDefinitionValid_guess *track_def)
{
    int result;

    result = 0;
    if (track_def == 0)
        result = 0xffff3d3e;
    if (track_def->magic != 0x49)
        result = 0xffff3d3e;
    return result;
}

// FUNCTION: EQGFX 0x1001075C
// __cdecl (callee pops 0 bytes). Returns the new track definition, or 0 when a step fails.
DMTrackDefinitionValid_guess *__cdecl s3dDefineDMTrack(
    void *dictionary,
    char *trackName,
    int vertexCount,
    unsigned int frameCount,
    int headerWord3,
    int headerWord4,
    void **frameSources)
{
    DMTrackDefinitionValid_guess *track;
    int *header;
    unsigned int frameTotal;
    void *frameData;
    char *nameCopy;
    void **frameTable;

    frameData = 0;
    track = (DMTrackDefinitionValid_guess *)EQGFX_DMSPRITE_ALLOCATE(0x30);
    if (track == 0)
        return 0;
    header = track->header_words;
    track->header = header;
    track->magic = 0x49;
    track->unknown_4 = 0;
    track->track_name = 0;
    track->unknown_c = 0;
    track->unknown_10 = 0;
    frameTotal = frameCount;
    header[2] = 0;
    header[0] = vertexCount;
    header[1] = frameTotal;
    header[3] = headerWord3;
    header[4] = headerWord4;
    if (trackName != 0)
    {
        nameCopy = (char *)EQGFX_DMSPRITE_ALLOCATE(EQGFX_DMSPRITE_STRLEN(trackName) + 1);
        track->track_name = nameCopy;
        if (nameCopy == 0)
            goto cleanup;
        EQGFX_DMSPRITE_COPY_STRING(nameCopy, trackName);
    }
    if (frameTotal != 0 && frameSources != 0)
    {
        frameTable = (void **)EQGFX_DMSPRITE_ALLOCATE(frameTotal << 2);
        header[2] = (int)frameTable;
        if (frameTable == 0)
            goto cleanup;
        frameData = EQGFX_DMSPRITE_ALLOCATE(vertexCount * frameTotal * 12);
        if (frameData == 0)
            goto cleanup;
        frameCount = 0;
        headerWord3 = (int)frameData;
        if (frameTotal != 0)
        {
            vertexCount = vertexCount * 12;
            do
            {
                EQGFX_DMSPRITE_MEMCPY(frameData, frameSources[frameCount], vertexCount);
                ((void **)header[2])[frameCount] = frameData;
                frameData = (char *)frameData + vertexCount;
                frameCount++;
            } while (frameCount < frameTotal);
        }
        frameData = (void *)headerWord3;
    }
    if (EQGFX_DMSPRITE_DICTIONARY_CHECK(dictionary, track) != 0)
        goto cleanup;
    if (trackName == 0)
        return track;
    if (EQGFX_DMSPRITE_ADD_TO_DICTIONARY(dictionary, track) >= 0)
        return track;
    EQGFX_DMSPRITE_DICTIONARY_UNDEFINE(dictionary, track);
cleanup:
    if (track->track_name != 0)
        EQGFX_DMSPRITE_FREE(track->track_name);
    if (header != 0 && header[2] != 0)
        EQGFX_DMSPRITE_FREE((void *)header[2]);
    if (frameData != 0)
        EQGFX_DMSPRITE_FREE(frameData);
    EQGFX_DMSPRITE_FREE(track);
    return 0;
}

// FUNCTION: EQGFX 0x100108dd
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns the alloc size for a track def2.
int __cdecl s3dCalcDMTrackDef2AllocSize(char *trackName, unsigned short countA, unsigned short countB)
{
    int allocSize;

    allocSize = countA * countB * 6 + 0x2c;
    if (trackName != 0)
        allocSize += strlen(trackName) + 1;
    return allocSize;
}

// FUNCTION: EQGFX 0x1001090B
// Defines a DM track resource with six-byte samples and an optional dictionary name.
DMTrackDef2_guess *__cdecl s3dDefineDMTrack2(
    void *dictionary,
    DMTrackDef2_guess *existingTrackDefinition,
    char *trackName,
    unsigned int countA,
    unsigned int countB,
    unsigned short trackValueA,
    unsigned short trackValueB,
    unsigned int trackValueC,
    void *sourceData)
{
    DMTrackDef2_guess *trackDefinition;
    size_t trackNameLength;
    int allocationSize;
    int result;
    char *dataDestination;

    trackDefinition = existingTrackDefinition;
    if (trackDefinition == 0)
    {
        allocationSize = EQ_CDECL(int, 0x100108DD, (char *, unsigned short, unsigned short))(
            trackName, (unsigned short)countA, (unsigned short)countB);
        trackDefinition = (DMTrackDef2_guess *)EQ_CDECL(void *, 0x10050C48, (int))(allocationSize);
        if (trackDefinition == 0)
            return 0;
    }

    trackDefinition->signature = 0x49;
    trackDefinition->track_header = &trackDefinition->count_a;
    trackDefinition->unknown_04 = 0;
    trackDefinition->name = 0;
    trackDefinition->unknown_0c = 0;
    trackDefinition->unknown_10 = 0;
    trackDefinition->count_b = (unsigned short)countB;
    trackDefinition->count_a = (unsigned short)countA;
    trackDefinition->track_value_a = trackValueA;
    trackDefinition->track_value_c = trackValueC;
    trackDefinition->track_data = 0;
    trackDefinition->track_value_b = trackValueB;

    dataDestination = trackDefinition->payload;
    if (trackName == 0)
    {
        trackDefinition->name = 0;
    }
    else
    {
        trackDefinition->name = dataDestination;
        trackNameLength = EQ_CDECL(size_t, 0x10068160, (char *))(trackName);
        dataDestination += trackNameLength + 1;
        EQ_CDECL(void, 0x10068070, (char *, char *))(trackDefinition->name, trackName);
    }

    if (((unsigned short)countB != 0) && ((unsigned short)countA != 0))
    {
        trackDefinition->track_data = dataDestination;
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, size_t))(
            dataDestination, sourceData,
            (unsigned short)countA * (unsigned short)countB * 6);
    }

    result = EQ_CDECL(int, 0x1001564B, (void *, DMTrackDef2_guess *))(dictionary, trackDefinition);
    if (result == 0)
    {
        if (trackName == 0)
            return trackDefinition;
        result = EQ_CDECL(int, 0x1000DC08, (void *, DMTrackDef2_guess *))(dictionary, trackDefinition);
        if (result >= 0)
            return trackDefinition;
        EQ_CDECL(int, 0x10015D81, (void *, DMTrackDef2_guess *))(dictionary, trackDefinition);
    }

    if ((trackDefinition != 0) && (existingTrackDefinition == 0))
        EQ_CDECL(void, 0x10050CBB, (void *))(trackDefinition);
    return 0;
}

// FUNCTION: EQGFX 0x10010a1f
// Releases an unused animation track and its privately owned data.
int __cdecl s3dUndefineDMTrack(void *dictionary, DMTrackInstance_guess *track)
{
    int result;
    DMTrackOwnedBlock_guess *ownedBlock;

    if (track->use_count != 0)
    {
        result = DMTRACK_UNDEFINE_IN_USE_RESULT;
    }
    else
    {
        ownedBlock = track->owned_block;
        if ((ownedBlock != 0) && (ownedBlock->owned_data != 0))
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(ownedBlock->owned_data[0]);
            EQ_CDECL(void, 0x10050CBB, (void *))(ownedBlock->owned_data);
        }
        if ((track->flags & 0x01) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, track->dictionary_key);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, track);
        if ((ownedBlock != 0) && (track->dictionary_key != 0))
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(track->dictionary_key);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(track);
    }
    return result;
}

// The objects s3dGetDMTrackDefinitionNumFrames reads (not in include/: the layout is guessed from the
// asm). A u32 frame count pointer at +0x14, a u16 frame count pointer at +0x18.
typedef struct DMTrack32NumFrames_guess
{
    int unknown_0;                          // +0x0
    unsigned int num_frames;                // +0x4
} DMTrack32NumFrames_guess;                 // 0x8

typedef struct DMTrack16NumFrames_guess
{
    unsigned short unknown_0;               // +0x0
    unsigned short num_frames;              // +0x2
} DMTrack16NumFrames_guess;                 // 0x4

typedef struct DMTrackDefinition_guess
{
    int unknown_0[5];                                       // +0x0
    DMTrack32NumFrames_guess *num_frames_ptr;               // +0x14
    DMTrack16NumFrames_guess *num_frames16_ptr;             // +0x18
} DMTrackDefinition_guess;                                  // 0x1c

// FUNCTION: EQGFX 0x10010a93
// __cdecl. Returns the track definition's frame count, preferring the 32-bit count when present.
unsigned int __cdecl s3dGetDMTrackDefinitionNumFrames(DMTrackDefinition_guess *track_def)
{
    if (track_def->num_frames_ptr != 0)
        return track_def->num_frames_ptr->num_frames;
    return track_def->num_frames16_ptr->num_frames;
}

// The objects s3dAdjustDMSpriteMeshDetail reads (not in include/: the layout is guessed from the
// asm). The holder carries the detail object at +0x14; the detail object
// holds the 0x10-byte vertex table at +0x24, the entry count at +0x5c, the 0xc-byte entry array at
// +0x60, the current index at +0x64 and the flag byte array at +0x80. Each entry is 0xc bytes: a
// kind dword at +0x0 (a float distance at +0x4 when kind is 4), a vertex index at +0x4, a sub index
// at +0x8 and a value at +0xa (1 = swap, 2 = flag word, 3 = flag byte, 4 = distance marker).
typedef struct DMSpriteAdjustVtx_guess
{
    unsigned char unknown_0[8];             // +0x0
    unsigned short flag;                    // +0x8
    unsigned short cell[3];                 // +0xa
} DMSpriteAdjustVtx_guess;                  // 0x10

typedef struct DMSpriteMeshAdjustEntry_guess
{
    int kind;                               // +0x0
    int index;                              // +0x4 (float distance when kind == 4)
    unsigned short sub;                     // +0x8
    unsigned short val;                     // +0xa
} DMSpriteMeshAdjustEntry_guess;            // 0xc

typedef struct DMSpriteMeshAdjustDetail_guess
{
    int unknown_0[9];                                   // +0x0
    DMSpriteAdjustVtx_guess *vtx;                       // +0x24
    int unknown_28[13];                                 // +0x28
    int total;                                          // +0x5c
    DMSpriteMeshAdjustEntry_guess *entries;             // +0x60
    int cur;                                            // +0x64
    int unknown_68[6];                                  // +0x68
    unsigned char *flags;                               // +0x80
} DMSpriteMeshAdjustDetail_guess;                       // 0x84

typedef struct DMSpriteMeshAdjustHolder_guess
{
    int unknown_0[5];                                   // +0x0
    DMSpriteMeshAdjustDetail_guess *detail;             // +0x14
} DMSpriteMeshAdjustHolder_guess;                       // 0x18

// FUNCTION: EQGFX 0x10010aaa
// __cdecl. Returns the sprite definition's vertex count, preferring the 32-bit count when present.
unsigned int __cdecl s3dGetDMSpriteDefinitionNumVertices(DMSpriteDefinition_guess *sprite_def)
{
    if (sprite_def->num_vertices_ptr != 0)
        return *sprite_def->num_vertices_ptr;
    return *sprite_def->num_vertices16_ptr;
}

// The object s3dIsDMTrackValid reads (not in include/: the layout is guessed from the
// asm). A magic id at +0x0 (0x4a when valid).
// FUNCTION: EQGFX 0x10010abf
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0 when valid, 0xffff3d3e otherwise.
int __cdecl s3dIsDMTrackValid(DMTrackValid_guess *track)
{
    int result;

    result = 0;
    if (track == 0)
        result = 0xffff3d3e;
    if (track->magic != 0x4a)
        result = 0xffff3d3e;
    return result;
}

// FUNCTION: EQGFX 0x10010ad8
// Creates a DM track instance from a shared track definition.
DMTrackValid_guess *__cdecl s3dCreateDMTrack(DMTrackCreateContext_guess *context, DMTrackCreateDefinition_guess *definition, int parameter24, int parameter28)
{
    DMTrackValid_guess *track;
    int sourceValue;
    int validationResult;

    track = (DMTrackValid_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x38);
    if (track != 0)
    {
        track->magic = 0x4a;
        track->unknown_4[0] = 0;
        track->unknown_4[1] = 0;
        track->unknown_4[2] = 0;
        track->unknown_4[3] = 0;
        if (definition->primarySource == 0)
        {
            track->primaryTrackValue = (unsigned int)definition->fallbackSource->trackValue;
            track->secondaryTrackValue = (unsigned int)definition->fallbackSource->trackValue;
            track->contextValue = context->contextValue;
            sourceValue = definition->fallbackSource->sourceValue;
        }
        else
        {
            track->primaryTrackValue = definition->primarySource->trackValue;
            track->secondaryTrackValue = definition->primarySource->trackValue;
            track->contextValue = context->contextValue;
            sourceValue = definition->primarySource->sourceValue;
        }
        track->sourceValue = sourceValue;
        track->parameter24 = parameter24;
        track->unknown_20 = 0;
        track->parameter28 = parameter28;
        track->unknown_2c = 0;
        track->definition = definition;
        validationResult = EQ_CDECL(int, 0x1001564B, (DMTrackCreateContext_guess *, DMTrackValid_guess *))(context, track);
        if (validationResult == 0)
        {
            EQ_CDECL(void, 0x10017CE8, (DMTrackCreateDefinition_guess *))(definition);
        }
        else
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(track);
            track = 0;
        }
    }
    return track;
}

// FUNCTION: EQGFX 0x10010b8f
// Lets another animation use the same definition while keeping its own track state.
// __cdecl. Duplicates a track: copies the source's values and definition, validates the copy through _t3dAddItemToList_guess, shares the definition on success and otherwise frees the copy and returns 0.
DMTrackValid_guess *__cdecl s3dDuplicateDMTrack(DMTrackCreateContext_guess *context, DMTrackValid_guess *source)
{
    DMTrackValid_guess *track;
    int validationResult;

    track = (DMTrackValid_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x38);
    if (track != 0)
    {
        track->unknown_4[0] = 0;
        track->unknown_4[1] = 0;
        track->unknown_4[2] = 0;
        track->unknown_4[3] = 0;
        track->magic = 0x4a;
        *(DMTrackValues_guess *)&track->primaryTrackValue = *(DMTrackValues_guess *)&source->primaryTrackValue;
        track->definition = source->definition;
        validationResult = EQ_CDECL(int, 0x1001564B, (DMTrackCreateContext_guess *, DMTrackValid_guess *))(context, track);
        if (validationResult != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(track);
            track = 0;
        }
        else
        {
            EQ_CDECL(void, 0x10017CE8, (DMTrackCreateDefinition_guess *))(track->definition);
        }
    }
    return track;
}

// The object s3dResetDMRGBTrackTiming writes (not in include/: the layout is guessed from the
// asm). Timing values at +0x1c/+0x24/+0x28 with zeroes at +0x20/+0x2c.
typedef struct DMRGBTrackTiming_guess
{
    int unknown_0[7];                 // +0x0
    int field1c;                      // +0x1c
    int field20;                      // +0x20
    int field24;                      // +0x24
    int field28;                      // +0x28
    int field2c;                      // +0x2c
} DMRGBTrackTiming_guess;             // 0x30

// FUNCTION: EQGFX 0x10010bf3
// __cdecl. Resets the RGB track timing, clearing two fields and storing three timing values.
int __cdecl s3dResetDMRGBTrackTiming(DMRGBTrackTiming_guess *track, int value1c, int value24, int value28)
{
    track->field20 = 0;
    track->field2c = 0;
    track->field24 = value24;
    track->field28 = value28;
    track->field1c = value1c;
    return 0;
}

// The object s3dIsDMRGBTrackDefinitionValid reads (not in include/: the layout is guessed from the
// asm). A magic id at +0x0 (0x4e when valid).
typedef struct DMRGBTrackDefinitionValid_guess
{
    int magic;                            // +0x0
} DMRGBTrackDefinitionValid_guess;        // 0x4

// FUNCTION: EQGFX 0x10010c17
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0 when valid, 0xffff3d3e otherwise.
int __cdecl s3dIsDMRGBTrackDefinitionValid(DMRGBTrackDefinitionValid_guess *track_def)
{
    int result;

    result = 0;
    if (track_def == 0)
        result = 0xffff3d3e;
    if (track_def->magic != 0x4e)
        result = 0xffff3d3e;
    return result;
}

// FUNCTION: EQGFX 0x10010c30
// __cdecl. Returns the track, or 0 after freeing whatever was allocated. A named track is also added to the dictionary.
DMRGBTrackValid_guess *__cdecl s3dDefineDMRGBTrack(void *dictionary, char *name, int rgbaCount, unsigned int frameCount, int value20, int value24, void **sourceFrames)
{
    DMRGBTrackValid_guess *track;
    unsigned int frameIndex;
    void *frameBlock;
    void *blockBase;

    frameIndex = 0;
    frameBlock = 0;
    track = (DMRGBTrackValid_guess *)EQGFX_DMSPRITE_ALLOCATE(0x28);
    if (track == 0)
        return 0;

    track->unknown_14[0] = rgbaCount;
    track->magic = 0x4e;
    track->unknown_20[0] = value20;
    track->unknown_4 = 0;
    track->dictionaryKey = 0;
    track->flags = 0;
    track->useCount = 0;
    track->unknown_14[1] = frameCount;
    track->ownedData = 0;
    track->unknown_20[1] = value24;

    if (name != 0)
    {
        track->dictionaryKey = EQGFX_DMSPRITE_ALLOCATE(EQ_CDECL(size_t, 0x10068160, (char *))(name) + 1);
        if (track->dictionaryKey == 0)
            goto free_partial_track;
        EQ_CDECL(void, 0x10068070, (char *, char *))(track->dictionaryKey, name);
    }

    if (frameCount != 0 && sourceFrames != 0)
    {
        track->ownedData = EQGFX_DMSPRITE_ALLOCATE(frameCount << 2);
        if (track->ownedData == 0)
            goto free_partial_track;
        blockBase = EQGFX_DMSPRITE_ALLOCATE(rgbaCount * frameCount * 4);
        frameBlock = blockBase;
        if (blockBase == 0)
            goto free_partial_track;
        if (frameCount != 0)
        {
            do
            {
                EQ_CDECL(void *, 0x10067D30, (void *, const void *, size_t))(
                    frameBlock, sourceFrames[frameIndex], rgbaCount * 4);
                track->ownedData[frameIndex] = frameBlock;
                frameBlock = (char *)frameBlock + rgbaCount * 4;
                frameIndex++;
            } while (frameIndex < frameCount);
        }
        frameBlock = blockBase;
    }

    if (EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, track) == 0)
    {
        if (name == 0)
            return track;
        if (EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, track) >= 0)
            return track;
        EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, track);
    }

free_partial_track:
    if (track->dictionaryKey != 0)
        EQGFX_DMSPRITE_FREE(track->dictionaryKey);
    if (track->ownedData != 0)
        EQGFX_DMSPRITE_FREE(track->ownedData);
    if (frameBlock != 0)
        EQGFX_DMSPRITE_FREE(frameBlock);
    EQGFX_DMSPRITE_FREE(track);
    return 0;
}

// FUNCTION: EQGFX 0x10010D88
// Frees an unused RGB animation track and its owned value data.
int __cdecl s3dUndefineDMRGBTrack(void *dictionary, DMRGBTrackValid_guess *track)
{
    int result;

    if (track->useCount != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        if (track->ownedData != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(*track->ownedData);
            EQ_CDECL(void, 0x10050CBB, (void *))(track->ownedData);
        }
        if ((track->flags & DMRGB_TRACK_FLAG_IN_DICT) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, track->dictionaryKey);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, track);
        if (track->dictionaryKey != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(track->dictionaryKey);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(track);
    }
    return result;
}

// FUNCTION: EQGFX 0x10010E08
// Creates an RGB animation track from a source for rendered color changes.
DMRGBTrackValid_guess *__cdecl s3dCreateDMRGBTrack(
    DMRGBTrackCreateContext_guess *context,
    DMRGBTrackCreateSource_guess *source,
    int trackValue24,
    int trackValue28)
{
    DMRGBTrackValid_guess *track;
    int result;

    track = (DMRGBTrackValid_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x38);
    if (track != 0)
    {
        track->magic = 0x4f;
        track->unknown_4 = 0;
        track->dictionaryKey = 0;
        track->flags = 0;
        track->useCount = 0;
        track->unknown_14[0] = source->value_20;
        track->unknown_14[1] = source->value_20;
        track->ownedData = context->ownedData;
        track->unknown_20[4] = source->value_24;
        track->unknown_20[1] = trackValue24;
        track->unknown_20[0] = 0;
        track->unknown_20[2] = trackValue28;
        track->unknown_20[3] = 0;
        track->sharedObject = source;
        result = EQ_CDECL(int, 0x1001564B, (void *, DMRGBTrackValid_guess *))(context, track);
        if (result != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(track);
            track = 0;
        }
        else
        {
            EQ_CDECL(void, 0x10017CE8, (void *))(source);
        }
    }
    return track;
}

// FUNCTION: EQGFX 0x10010def
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0 when valid, 0xffff3d3e otherwise.
int __cdecl s3dIsDMRGBTrackValid(DMRGBTrackValid_guess *track)
{
    int result;

    result = 0;
    if (track == 0)
        result = 0xffff3d3e;
    if (track->magic != 0x4f)
        result = 0xffff3d3e;
    return result;
}

// FUNCTION: EQGFX 0x10010E89
// Creates a second RGB track with copied values that shares the original track's render resource.
// __cdecl. Duplicates a DMRGB track: copies the source's values block and shared object, validates the copy through _t3dAddItemToList_guess, shares the shared object on success and otherwise frees the copy and returns 0.
DMRGBTrackValid_guess *__cdecl s3dDuplicateDMRGBTrack(DMRGBTrackCreateContext_guess *context, DMRGBTrackValid_guess *source)
{
    DMRGBTrackValid_guess *track;
    int validationResult;

    track = (DMRGBTrackValid_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x38);
    if (track != 0)
    {
        track->unknown_4 = 0;
        track->dictionaryKey = 0;
        track->flags = 0;
        track->useCount = 0;
        track->magic = 0x4f;
        *(DMRGBTrackValues_guess *)&track->unknown_14 = *(DMRGBTrackValues_guess *)&source->unknown_14;
        track->sharedObject = source->sharedObject;
        validationResult = EQ_CDECL(int, 0x1001564B, (DMRGBTrackCreateContext_guess *, DMRGBTrackValid_guess *))(context, track);
        if (validationResult != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(track);
            track = 0;
        }
        else
        {
            EQ_CDECL(void, 0x10017CE8, (void *))(track->sharedObject);
        }
    }
    return track;
}

// FUNCTION: EQGFX 0x10010eed
// Releases an unused RGB track's shared rendering resource.
int __cdecl s3dDestroyDMRGBTrack(void *dictionary, DMRGBTrackValid_guess *track)
{
    int result;

    if (track->useCount != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        EQ_CDECL(void, 0x10017DAD, (void *))(track->sharedObject);
        if ((track->flags & DMRGB_TRACK_FLAG_IN_DICT) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, track->dictionaryKey);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, track);
        if (track->dictionaryKey != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(track->dictionaryKey);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(track);
    }
    return result;
}

// FUNCTION: EQGFX 0x10010f45
// Updates both active sprite animation tracks using the current frame time.
void __cdecl s3dUpdateDMSpriteTiming(DMSpriteTimingContainer_guess *container, unsigned int frameTime)
{
    DMSpriteTimingSprite_guess *first;
    DMSpriteTimingSprite_guess *second;
    DMTrackDefinition_guess *trackDef;

    first = container->first;
    second = container->second;
    if (first != 0 && (first->flags & DMSPRITE_TIMING_SKIP_FLAG) == 0)
    {
        trackDef = (DMTrackDefinition_guess *)first->definition;
        if (trackDef->num_frames_ptr != 0)
        {
            EQ_CDECL(int, 0x10010FA3, (DMSpriteFrameIndex_guess *, unsigned int, unsigned int))(&first->timing, trackDef->num_frames_ptr->num_frames, frameTime);
        }
        else
        {
            EQ_CDECL(int, 0x10010FA3, (DMSpriteFrameIndex_guess *, unsigned int, unsigned int))(&first->timing, trackDef->num_frames16_ptr->num_frames, frameTime);
        }
    }
    if (second != 0 && (second->flags & DMSPRITE_TIMING_SKIP_FLAG) == 0)
    {
        EQ_CDECL(int, 0x10010FA3, (DMSpriteFrameIndex_guess *, unsigned int, unsigned int))(&second->timing, ((DMSpriteTimingSecondDef_guess *)second->definition)->frameCount, frameTime);
    }
}

// FUNCTION: EQGFX 0x10010fa3
// Advances animation frames with elapsed time, wrapping playback and enforcing a cycle limit.
int __cdecl updateSpriteFrameIndex_guess(DMSpriteFrameIndex_guess *spriteTiming, unsigned int frameCount, unsigned int frameTime)
{
    int elapsedOffset;
    int framePosition;
    int cycleLimit;
    int incomingEax;

    __asm
    {
        mov incomingEax, eax
    }

    if (spriteTiming->finished != 0)
        return incomingEax;
    if (frameCount < 2)
        return incomingEax;
    if (frameTime < spriteTiming->elapsed_time)
        spriteTiming->elapsed_time = frameTime;

    elapsedOffset = (int)(((float)(frameTime - spriteTiming->elapsed_time) * spriteTiming->frame_rate)
        / (float)spriteTiming->time_divisor);
    cycleLimit = spriteTiming->cycle_limit;
    if (cycleLimit < 1)
    {
        framePosition = spriteTiming->frame_position + elapsedOffset;
        if (elapsedOffset < 0)
            elapsedOffset = -elapsedOffset;
        spriteTiming->elapsed_cycles = elapsedOffset / (int)frameCount;
        goto wrap_frame_position;
    }
    else
    {
        if (elapsedOffset < (int)((unsigned int)cycleLimit * frameCount))
        {
            if ((cycleLimit < 1) || ((int)-((int)((unsigned int)cycleLimit * frameCount)) < elapsedOffset))
                goto update_elapsed_cycles;
            spriteTiming->finished = 1;
            framePosition = spriteTiming->frame_position + 1;
        }
        else
        {
            spriteTiming->finished = 1;
            framePosition = spriteTiming->frame_position - 1;
            if (framePosition < 0)
                framePosition += frameCount;
        }
        spriteTiming->elapsed_cycles = cycleLimit;
        goto wrap_frame_position;
    }

update_elapsed_cycles:
    framePosition = spriteTiming->frame_position + elapsedOffset;
    if (elapsedOffset < 0)
        elapsedOffset = -elapsedOffset;
    spriteTiming->elapsed_cycles = elapsedOffset / (int)frameCount;

wrap_frame_position:
    if (framePosition < 0)
        framePosition = frameCount - framePosition;
    spriteTiming->frame_index = framePosition % (int)frameCount;
    return framePosition / (int)frameCount;
}

// FUNCTION: EQGFX 0x10011051
// Caches sprite vertex positions after applying the sprite offset and a single affine transform.
int __cdecl s3dUpdateDMSpriteVertexCacheWith4x3(DMSpriteVertexCacheSprite_guess *sprite, int vertexMarker, float *transform)
{
    DMSpriteVertexCache_guess *vertexCache;
    DMSpriteVertexCacheDetail_guess *detail;
    DMSpriteVertexCacheSourceList_guess *sourceList;
    float *sourceVertex;
    float *sourceVertexEnd;
    float *cachedVertex;
    float spriteX;
    float spriteY;
    float spriteZ;
    float x;
    float y;
    float z;

    vertexCache = sprite->cacheOwner->vertexCache;
    if (vertexCache != 0 && vertexCache->vertexCount != 0)
    {
        spriteX = sprite->x;
        spriteY = sprite->y;
        spriteZ = sprite->z;
        if (vertexCache->transformedVertices == 0)
        {
            vertexCache->transformedVertices = (float *)EQ_CDECL(void *, 0x10050C48, (int))(vertexCache->vertexCount * 3 * sizeof(float));
            if (vertexCache->transformedVertices == 0)
            {
                (*EQGFX_OUTPUT_DEBUG_STRING_A)(EQGFX_VERTEX_CACHE_ALLOCATION_ERROR);
                return DMTRACK_UNDEFINE_IN_USE_RESULT;
            }
        }
        detail = sprite->detail;
        if (detail == 0 || (sourceList = detail->sourceHolder->sourceList)->sourceCount == 0)
        {
            sourceVertex = vertexCache->sourceVertices;
        }
        else
        {
            sourceVertex = sourceList->sourceVertices[detail->sourceIndex];
        }
        sourceVertexEnd = sourceVertex + vertexCache->vertexCount * 3;
        vertexCache->lastVertexMarker = vertexMarker;
        cachedVertex = vertexCache->transformedVertices;
        for (; sourceVertex < sourceVertexEnd; sourceVertex += 3)
        {
            x = spriteX + sourceVertex[0];
            y = spriteY + sourceVertex[1];
            z = spriteZ + sourceVertex[2];
            cachedVertex[0] = x * transform[0] + z * transform[6] + y * transform[3] + transform[9];
            cachedVertex[1] = y * transform[4] + x * transform[1] + z * transform[7] + transform[10];
            cachedVertex[2] = x * transform[2] + z * transform[8] + y * transform[5] + transform[11];
            cachedVertex += 3;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10011169
// Caches skinned sprite vertex positions using the transform assigned to each vertex run.
int __cdecl s3dUpdateDMSkinSpriteVertexCache(DMSpriteVertexCacheSprite_guess *sprite, DMSkinSpriteTransformOwner_guess *transformOwner, int vertexMarker)
{
    DMSpriteVertexCache_guess *vertexCache;
    unsigned short *skinVertexRun;
    float *sourceVertex;
    float *sourceVertexEnd;
    float *cachedVertex;
    float *transform;
    float spriteX;
    float spriteY;
    float spriteZ;
    float x;
    float y;
    float z;
    unsigned int runVertexCount;
    unsigned short transformIndex;

    vertexCache = sprite->cacheOwner->vertexCache;
    if (vertexCache != 0 && vertexCache->vertexCount != 0)
    {
        if (vertexCache->transformedVertices == 0)
        {
            vertexCache->transformedVertices = (float *)EQ_CDECL(void *, 0x10050C48, (int))(vertexCache->vertexCount * 3 * sizeof(float));
            if (vertexCache->transformedVertices == 0)
            {
                (*EQGFX_OUTPUT_DEBUG_STRING_A)(EQGFX_SKIN_VERTEX_CACHE_ALLOCATION_ERROR);
                return DMTRACK_UNDEFINE_IN_USE_RESULT;
            }
        }
        sourceVertex = vertexCache->sourceVertices;
        vertexCache->lastVertexMarker = vertexMarker;
        runVertexCount = 0;
        spriteX = sprite->x;
        spriteY = sprite->y;
        spriteZ = sprite->z;
        sourceVertexEnd = sourceVertex + vertexCache->vertexCount * 3;
        skinVertexRun = vertexCache->skinVertexRuns - 2;
        cachedVertex = vertexCache->transformedVertices;
        for (; sourceVertex < sourceVertexEnd; sourceVertex = sourceVertex + 3)
        {
            if (runVertexCount == 0)
            {
                do
                {
                    transformIndex = skinVertexRun[3];
                    skinVertexRun = skinVertexRun + 2;
                    runVertexCount = (unsigned int)skinVertexRun[0];
                    transform = transformOwner->transformBank->transforms[transformIndex].matrix;
                } while (runVertexCount == 0);
            }
            x = spriteX + sourceVertex[0];
            y = spriteY + sourceVertex[1];
            z = spriteZ + sourceVertex[2];
            cachedVertex[0] = x * transform[0] + y * transform[3] + z * transform[6] + transform[9];
            runVertexCount = runVertexCount - 1;
            cachedVertex[1] = x * transform[1] + y * transform[4] + z * transform[7] + transform[10];
            cachedVertex[2] = x * transform[2] + y * transform[5] + z * transform[8] + transform[11];
            cachedVertex = cachedVertex + 3;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10011290
// __cdecl. Moves the sprite mesh detail boundary to the first distance marker beyond max_dist.
int __cdecl s3dAdjustDMSpriteMeshDetail(DMSpriteMeshAdjustHolder_guess *holder, float max_dist)
{
    DMSpriteMeshAdjustDetail_guess *detail;
    DMSpriteMeshAdjustEntry_guess *old_pos;
    DMSpriteMeshAdjustEntry_guess *new_pos;
    DMSpriteMeshAdjustEntry_guess *end;
    unsigned short saved;

    detail = holder->detail;
    if (detail == 0)
        return 1;
    if (detail->entries == 0 || detail->total == 0)
        return 0xffff3d3e;
    old_pos = detail->entries + detail->cur;
    end = detail->entries + detail->total;
    for (new_pos = old_pos; new_pos < end; new_pos++)
    {
        if (new_pos->kind == 4 && *(float *)&new_pos->index > max_dist)
            break;
    }
    while (new_pos > detail->entries)
    {
        new_pos--;
        if (new_pos->kind != 4)
            continue;
        if (*(float *)&new_pos->index >= max_dist)
            continue;
        break;
    }
    if (old_pos != new_pos)
    {
        if (old_pos < new_pos)
        {
            do
            {
                switch (old_pos->kind)
                {
                case 3:
                    detail->flags[old_pos->index] &= 0;
                    break;
                case 2:
                    detail->vtx[old_pos->index].flag = 1;
                    break;
                case 1:
                    saved = detail->vtx[old_pos->index].cell[old_pos->sub];
                    detail->flags[old_pos->val] = 1;
                    detail->vtx[old_pos->index].cell[old_pos->sub] = old_pos->val;
                    old_pos->val = saved;
                    break;
                }
                old_pos++;
            } while (old_pos < new_pos);
        }
        else
        {
            for (;;)
            {
                old_pos--;
                if (old_pos < new_pos)
                    break;
                switch (old_pos->kind)
                {
                case 3:
                    detail->flags[old_pos->index] = 1;
                    break;
                case 2:
                    detail->vtx[old_pos->index].flag &= 0;
                    break;
                case 1:
                    saved = detail->vtx[old_pos->index].cell[old_pos->sub];
                    detail->flags[saved] &= 0;
                    detail->vtx[old_pos->index].cell[old_pos->sub] = old_pos->val;
                    old_pos->val = saved;
                    break;
                }
            }
        }
        detail->cur = new_pos - detail->entries;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10011408
// __cdecl. Moves the sprite mesh detail boundary to the last distance marker within max_dist.
int __cdecl s3dAdjustDMSpriteMeshDetail2(DMSpriteMeshHolder_guess *holder, float max_dist)
{
    DMSpriteMeshDetail_guess *detail;
    DMSpriteMeshEntry_guess *old_pos;
    DMSpriteMeshEntry_guess *new_pos;
    DMSpriteMeshEntry_guess *end;
    unsigned short saved;

    detail = holder->detail;
    if (detail == 0)
        return 1;
    if (detail->entries == 0 || detail->total == 0)
        return 0xffff3d3e;
    old_pos = detail->entries + detail->cur;
    end = detail->entries + detail->total;
    for (new_pos = old_pos; new_pos < end; new_pos++)
    {
        if (new_pos->kind == 4 && *(float *)new_pos > max_dist)
            break;
    }
    while (new_pos > detail->entries)
    {
        new_pos--;
        if (new_pos->kind != 4)
            continue;
        if (*(float *)new_pos >= max_dist)
            continue;
        break;
    }
    if (old_pos != new_pos)
    {
        if (old_pos < new_pos)
        {
            do
            {
                switch (old_pos->kind)
                {
                case 3:
                    detail->flags0[old_pos->vert0] &= 0;
                    break;
                case 2:
                    detail->flags1[old_pos->vert0] &= 0;
                    break;
                case 1:
                    saved = detail->swap_table[old_pos->vert0].slot[old_pos->blend];
                    detail->flags0[old_pos->vert1] = 1;
                    detail->swap_table[old_pos->vert0].slot[old_pos->blend] = old_pos->vert1;
                    old_pos->vert1 = saved;
                    break;
                }
                old_pos++;
            } while (old_pos < new_pos);
        }
        else
        {
            for (;;)
            {
                old_pos--;
                if (old_pos < new_pos)
                    break;
                switch (old_pos->kind)
                {
                case 3:
                    detail->flags0[old_pos->vert0] = 1;
                    break;
                case 2:
                    detail->flags1[old_pos->vert0] = 1;
                    break;
                case 1:
                    saved = detail->swap_table[old_pos->vert0].slot[old_pos->blend];
                    detail->flags0[saved] &= 0;
                    detail->swap_table[old_pos->vert0].slot[old_pos->blend] = old_pos->vert1;
                    old_pos->vert1 = saved;
                    break;
                }
            }
        }
        detail->cur = new_pos - detail->entries;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001156d
// __cdecl. Patches a sprite def's lighting values from `lighting` into its lighting buffer.
int __cdecl s3dPatchDMSpriteDefLighting(DMSpriteDefLightingPatch_guess *sprite_def, unsigned int *lighting)
{
    unsigned int count_index;
    unsigned char *dest;
    unsigned char *src;

    count_index = 0;
    if (sprite_def->lighting32 != 0)
    {
        src = (unsigned char *)lighting;
        dest = (unsigned char *)sprite_def->lighting32->lighting_dest;
        for (; count_index < sprite_def->lighting32->count; count_index++)
        {
            *(unsigned int *)dest = *(unsigned int *)src;
            dest += 4;
            src += 4;
        }
    }
    else
    {
        dest = (unsigned char *)sprite_def->lighting16->entries + 0x11;
        src = (unsigned char *)lighting + 1;
        for (; count_index < sprite_def->lighting16->count; count_index++)
        {
            dest[-1] = src[1];
            dest[0] = src[0];
            dest[1] = src[-1];
            dest[2] = src[2];
            dest += 0x14;
            src += 4;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100115dc
// __cdecl. Copies the track def's lighting values from `lighting` into its lighting buffer.
int __cdecl s3dPatchDMTrackDefLighting(DMTrackDefLighting_guess *track_def, unsigned int *lighting)
{
    unsigned int count_index;
    unsigned int *lighting_dest = *track_def->lighting_dest_ptr;

    for (count_index = 0; count_index < track_def->count; count_index++)
        *lighting_dest++ = *lighting++;
    return 0;
}

// FUNCTION: EQGFX 0x10011607
// Exports a lighting patch with target vertex colors reordered to match the source sprite's vertices.
int __cdecl s3dDMSpriteDefResortForLightPatch(DMSpriteDefResortObject_guess *source,
    DMSpriteDefResortObject_guess *target, FILE *output_file)
{
    DMSpriteDefResortDefinition_guess *source_definition;
    DMSpriteDefResortDefinition_guess *target_definition;
    DMSpriteDefResortVertex_guess *source_vertex;
    DMSpriteDefResortVertex_guess *target_vertex;
    int vertex_count;
    int *source_color_map;
    int *target_color_map;
    unsigned short *run_values;
    int run_remaining = 0;
    int run_color;
    int group_index;
    int vertex_index;
    int candidate_index;
    int found;

    source_definition = source->definition;
    if (source_definition == 0)
        return -1;
    target_definition = target->definition;
    if (target_definition == 0)
        return -1;

    vertex_count = source_definition->vertex_count;
    if (target_definition->vertex_count != vertex_count)
        return DMTRACK_UNDEFINE_IN_USE_RESULT;

    source_color_map = (int *)EQGFX_DMSPRITE_ALLOCATE(vertex_count * 4);
    if (source_color_map != 0)
    {
        target_color_map = (int *)EQGFX_DMSPRITE_ALLOCATE(vertex_count * 4);
        if (target_color_map != 0)
        {
            if (vertex_count != 0)
            {
                for (vertex_index = 0; vertex_index < vertex_count; vertex_index++)
                {
                    source_color_map[vertex_index] = 0;
                    target_color_map[vertex_index] = 0;
                }
            }

            run_values = source_definition->color_runs;
            for (group_index = 0; group_index < source_definition->group_count; group_index++)
            {
                if (run_remaining == 0)
                {
                    run_color = run_values[1];
                    run_remaining = run_values[0];
                    run_values += 2;
                }
                run_remaining--;
                source_color_map[source_definition->vertex_groups[group_index].vertex_0] = run_color;
                source_color_map[source_definition->vertex_groups[group_index].vertex_1] = run_color;
                source_color_map[source_definition->vertex_groups[group_index].vertex_2] = run_color;
            }

            run_remaining = 0;
            run_values = target_definition->color_runs;
            for (group_index = 0; group_index < target_definition->group_count; group_index++)
            {
                if (run_remaining == 0)
                {
                    run_remaining = run_values[0];
                    run_color = run_values[1];
                    run_values += 2;
                }
                run_remaining--;
                target_color_map[target_definition->vertex_groups[group_index].vertex_0] = run_color;
                target_color_map[target_definition->vertex_groups[group_index].vertex_1] = run_color;
                target_color_map[target_definition->vertex_groups[group_index].vertex_2] = run_color;
            }

            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_LIGHT_PATCH_HEADER);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_TRACKDEF_HEADER);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_TAG_FORMAT, target->tag);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_VERTEX_COUNT_FORMAT, vertex_count);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_FRAME_COUNT);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_SLEEP);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_FRAME_HEADER);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_RGBA_COUNT_FORMAT, vertex_count);

            source_vertex = source_definition->vertices;
            for (vertex_index = 0; vertex_index < vertex_count; vertex_index++)
            {
                found = 0;
                target_vertex = target_definition->vertices;
                for (candidate_index = 0; candidate_index < vertex_count; candidate_index++)
                {
                    if (found != 0)
                        break;
                    if (source_color_map[vertex_index] == target_color_map[candidate_index] &&
                        source_vertex->unknown_0 == target_vertex->unknown_0 &&
                        source_vertex->unknown_2 == target_vertex->unknown_2 &&
                        source_vertex->unknown_4 == target_vertex->unknown_4 &&
                        source_vertex->unknown_6 == target_vertex->unknown_6 &&
                        source_vertex->unknown_a == target_vertex->unknown_a &&
                        source_vertex->unknown_e == target_vertex->unknown_e)
                    {
                        found = 1;
                        EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_RGBA_FORMAT,
                            (double)((float)target_vertex->rgba[0] * EQ_GLOBAL(float, 0x1007629C)),
                            (double)((float)target_vertex->rgba[1] * EQ_GLOBAL(float, 0x1007629C)),
                            (double)((float)target_vertex->rgba[2] * EQ_GLOBAL(float, 0x1007629C)),
                            (double)((float)target_vertex->rgba[3] * EQ_GLOBAL(float, 0x1007629C)));
                    }
                    target_vertex++;
                }
                if (found == 0)
                    return DMTRACK_UNDEFINE_IN_USE_RESULT;
                source_vertex++;
            }

            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_FRAME_END);
            EQGFX_DMSPRITE_FPRINTF(output_file, EQGFX_DMSPRITE_TRACKDEF_END);
            EQGFX_DMSPRITE_FREE(source_color_map);
            EQGFX_DMSPRITE_FREE(target_color_map);
            return 0;
        }
    }
    return DMTRACK_UNDEFINE_IN_USE_RESULT;
}

// The object t3dSetPlaneEquation writes (not in include/: the layout is guessed from the
// asm). A plane normal at +0x4 and distance at +0x10.
typedef struct T3DPlaneEquation_guess
{
    int unknown_0;                      // +0x0
    T3DEdgeVertex_guess normal;         // +0x4
    float distance;                     // +0x10
} T3DPlaneEquation_guess;               // 0x14

// FUNCTION: EQGFX 0x10011944
// Creates a plane that describes a 3D surface in the scene.
// __cdecl. Allocates a plane equation, copies the normal and distance into it and returns it (NULL if the allocation failed).
T3DPlaneEquation_guess *__cdecl t3dCreatePlane(T3DEdgeVertex_guess *normal, float distance)
{
    T3DPlaneEquation_guess *plane;

    plane = (T3DPlaneEquation_guess *)EQ_CDECL(void *, 0x10050C48, (int))(sizeof(T3DPlaneEquation_guess));
    if (plane != 0)
    {
        plane->unknown_0 = 0x2f;
        plane->normal = *normal;
        plane->distance = distance;
    }
    return plane;
}

// FUNCTION: EQGFX 0x1001196c
// __cdecl. Copies the plane normal into `normal` and returns its distance.
float __cdecl t3dGetPlaneEquation(T3DPlaneEquation_guess *plane, T3DEdgeVertex_guess *normal)
{
    *normal = plane->normal;
    return plane->distance;
}

// FUNCTION: EQGFX 0x10011982
// __cdecl. Copies the normal into the plane and sets its distance.
int __cdecl t3dSetPlaneEquation(T3DPlaneEquation_guess *plane, T3DEdgeVertex_guess *normal, float distance)
{
    plane->normal = *normal;
    plane->distance = distance;
    return 0;
}

// The object t3dGetVertexPosition reads (not in include/: the layout is guessed from the asm
// and marked). A three-float position at +0x4.
typedef struct T3DVertexPosition_guess
{
    int unknown_0;                      // +0x0
    T3DEdgeVertex_guess position;       // +0x4
} T3DVertexPosition_guess;              // 0x10

// FUNCTION: EQGFX 0x1001199e
// __cdecl. Copies the vertex's three-float position at +0x4 into `position`.
int __cdecl t3dGetVertexPosition(T3DVertexPosition_guess *vertex, T3DEdgeVertex_guess *position)
{
    *position = vertex->position;
    return 0;
}

// The object t3dGetEdgeVertex1 reads (not in include/: the layout is guessed from the asm
// and marked). A vertex pointer at +0x4.
typedef struct T3DEdge1_guess
{
    int unknown_0;                        // +0x0
    T3DEdgeVertex_guess *vertex;          // +0x4
} T3DEdge1_guess;                         // 0x8

// FUNCTION: EQGFX 0x100119b3
// __cdecl. Copies the edge's three-float vertex at +0x4 into `vertex`.
int __cdecl t3dGetEdgeVertex1(T3DEdge1_guess *edge, T3DEdgeVertex_guess *vertex)
{
    *vertex = *edge->vertex;
    return 0;
}

// FUNCTION: EQGFX 0x100119c8
// __cdecl. Copies the edge's three-float vertex at +0x8 into `vertex`.
int __cdecl t3dGetEdgeVertex2(T3DEdge_guess *edge, T3DEdgeVertex_guess *vertex)
{
    *vertex = *edge->vertex;
    return 0;
}

// The object t3dGetXYEdgeVertex1 reads (not in include/: the layout is guessed from the asm
// and marked). A vertex pointer at +0x10.
typedef struct T3DXYEdge1_guess
{
    int unknown_0[4];                       // +0x0
    T3DEdgeVertex_guess *vertex;            // +0x10
} T3DXYEdge1_guess;                         // 0x14

// FUNCTION: EQGFX 0x100119dd
// __cdecl. Copies the edge's three-float vertex at +0x10 into `vertex`.
int __cdecl t3dGetXYEdgeVertex1(T3DXYEdge1_guess *edge, T3DEdgeVertex_guess *vertex)
{
    *vertex = *edge->vertex;
    return 0;
}

// The object t3dGetXYEdgeVertex2 reads (not in include/: the layout is guessed from the asm
// and marked). A vertex pointer at +0x14.
typedef struct T3DXYEdge2_guess
{
    int unknown_0[5];                       // +0x0
    T3DEdgeVertex_guess *vertex;            // +0x14
} T3DXYEdge2_guess;                         // 0x18

// FUNCTION: EQGFX 0x100119f2
// __cdecl. Copies the edge's three-float vertex at +0x14 into `vertex`.
int __cdecl t3dGetXYEdgeVertex2(T3DXYEdge2_guess *edge, T3DEdgeVertex_guess *vertex)
{
    *vertex = *edge->vertex;
    return 0;
}

// FUNCTION: EQGFX 0x10011a07
// __cdecl. With `index` < count copies edge `index`'s vertex to `vertex` and returns 0, else 0xffff3d3e.
int __cdecl t3dGetEdgePolygonEdgeVertex1(T3DEdgePolygon1_guess *edge_polygon, unsigned int index,
                                         T3DEdgeVertex_guess *vertex)
{
    int rc = 0;

    if (index >= edge_polygon->count)
        return 0xffff3d3e;
    *vertex = *edge_polygon->edges[index].vertex;
    return rc;
}

// FUNCTION: EQGFX 0x10011a32
// Provides indexed polygon edge vertex data for surface geometry; which endpoint it represents is unclear.
// __cdecl (the callee pops 0 bytes; ECX is not an input). With `index` < count (an unsigned compare) copies
// edge `index`'s 12-byte (three-float) vertex to `vertex` and returns 0; otherwise copies nothing and
// returns the error code 0xffff3d3e.
int __cdecl t3dGetEdgePolygonEdgeVertex2(T3DEdgePolygon_guess *edge_polygon, unsigned int index,
                                         T3DEdgeVertex_guess *vertex)
{
    int rc = 0;

    if (index >= edge_polygon->count)
        return 0xffff3d3e;
    *vertex = *edge_polygon->edges[index].vertex;
    return rc;
}

// FUNCTION: EQGFX 0x10011a5d
// __cdecl. Returns edge `index`'s length at +0xc, or 0 when `index` >= count.
float __cdecl t3dGetEdgePolygonEdgeLength(T3DEdgePolygon_guess *edge_polygon, unsigned int index)
{
    return index >= edge_polygon->count
        ? 0.0f
        : edge_polygon->edges[index].length;
}

// FUNCTION: EQGFX 0x10011a78
// __cdecl. Copies the polygon plane normal at +0xc to `normal` and returns the plane distance at +0x18.
float __cdecl t3dGetEdgePolygonPlane(T3DEdgePolygon_guess *edge_polygon, T3DEdgeVertex_guess *normal)
{
    *normal = edge_polygon->normal;
    return edge_polygon->dist;
}

// The object t3dGetPolygonVertexList reads (not in include/: the layout is guessed from the asm
// and marked). A u32 count at +0x4, an int index table at +0x8, a three-float vertex table at +0xc.
typedef struct T3DPolygonVertexList_guess
{
    int unknown_0;                        // +0x0
    unsigned int count;                   // +0x4
    int *indices;                         // +0x8
    T3DEdgeVertex_guess *vertices;         // +0xc
} T3DPolygonVertexList_guess;

// FUNCTION: EQGFX 0x10011a8e
// __cdecl. Expands the index table into the vertex list and returns 0.
int __cdecl t3dGetPolygonVertexList(T3DPolygonVertexList_guess *polygon, T3DEdgeVertex_guess *vertex_list)
{
    T3DEdgeVertex_guess *vertices = polygon->vertices;
    int *cur = polygon->indices;
    int *end = cur + polygon->count;
    for (; cur < end; cur++, vertex_list++)
        *vertex_list = vertices[*cur];
    return 0;
}
