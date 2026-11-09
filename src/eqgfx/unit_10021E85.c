// unit_10021E85.c - eqgfx_dx7.dll, the address run 0x10021E85..0x10024A4C (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"

#define T3D_ASYNC_READ_BUFFER_SIZE 0x8000
#define T3D_ASYNC_READ_MIN_FREE_SPACE 0x2000

typedef void (__stdcall *EqgfxEnterCriticalSection_t)(void *criticalSection);
typedef void (__stdcall *EqgfxLeaveCriticalSection_t)(void *criticalSection);
typedef void (__stdcall *EqgfxInitializeCriticalSection_t)(void *criticalSection);
typedef void (__stdcall *EqgfxDeleteCriticalSection_t)(void *criticalSection);
typedef void *(__stdcall *EqgfxCreateFileA_t)(const char *fileName, unsigned int access,
    unsigned int shareMode, void *securityAttributes, unsigned int creationDisposition,
    unsigned int flagsAndAttributes, void *templateFile);
typedef int (__stdcall *EqgfxReadFile_t)(void *file, void *buffer, unsigned int bytesToRead,
    unsigned int *bytesRead, void *overlapped);
typedef void (__stdcall *EqgfxSleep_t)(unsigned int milliseconds);
typedef int (__stdcall *EqgfxCloseHandle_t)(void *handle);

#define EQGFX_ENTER_CRITICAL_SECTION EQ_GLOBAL(EqgfxEnterCriticalSection_t, 0x100760B4)
#define EQGFX_LEAVE_CRITICAL_SECTION EQ_GLOBAL(EqgfxLeaveCriticalSection_t, 0x100760B8)
#define EQGFX_INITIALIZE_CRITICAL_SECTION EQ_GLOBAL(EqgfxInitializeCriticalSection_t, 0x100760A4)
#define EQGFX_DELETE_CRITICAL_SECTION EQ_GLOBAL(EqgfxDeleteCriticalSection_t, 0x100760C0)
#define EQGFX_WORLD_LOAD_MODE EQ_GLOBAL(int, 0x100AADE4)
#define EQGFX_CREATE_FILE_A EQ_GLOBAL(EqgfxCreateFileA_t, 0x10076098)
#define EQGFX_READ_FILE EQ_GLOBAL(EqgfxReadFile_t, 0x100760A8)
#define EQGFX_SLEEP EQ_GLOBAL(EqgfxSleep_t, 0x100760B0)
#define EQGFX_CLOSE_HANDLE EQ_GLOBAL(EqgfxCloseHandle_t, 0x100760AC)

#define T3D_VOLUME_FILE_RECORD_SIZE 0xa0
#define T3D_VOLUME_POINTER_SLOT_STRIDE 0x28
#define T3D_VOLUME_FLAG_SET_BY_CONTEXT 0x10
#define GFX_TEXTURE_LOG_NAME EQ_ARRAY(char, 0x10086164)
#define GFX_TEXTURE_LOG_MODE EQ_ARRAY(char, 0x10086170)
#define EQGFX_ACTOR_MISSING_DEFINITION_MESSAGE EQ_ARRAY(char, 0x100870DC)
#define EQGFX_ACTOR_CREATE_FAILURE_MESSAGE EQ_ARRAY(char, 0x1008708C)
#define T3D_ACTOR_FILE_FLAG_ACTION 0x01
#define T3D_ACTOR_FILE_FLAG_POSITION 0x02
#define T3D_ACTOR_FILE_FLAG_VALUE_A 0x04
#define T3D_ACTOR_FILE_FLAG_VALUE_B 0x08
#define T3D_ACTOR_FILE_FLAG_AUXILIARY_CALLBACK 0x20
#define T3D_ACTOR_FILE_FLAG_VOLUME_POINTER 0x80
#define T3D_ACTOR_FILE_FLAG_COLOR_TRACK 0x100
#define T3D_ACTOR_FILE_FLAG_CREATE_OPTION 0x200
#define T3D_ACTOR_FLAG_SET_BY_CONTEXT 0x10
#define T3D_ACTOR_RESIZE_RESULT 0x12345678
#define EQGFX_DEFAULT_VOLUME_POINTER EQ_ARRAY(char, 0x100F401C)
#define EQGFX_HIERARCHICAL_SPRITE_DEFAULT_POINTER EQ_ARRAY(char, 0x100A35D0)
#define EQGFX_HIERARCHICAL_RESOURCE_POINTER EQ_ARRAY(char, 0x100A35C8)
#define T3D_HIERARCHICAL_SPRITE_RECORD_SIZE 0x148
#define T3D_HIERARCHICAL_SPRITE_RECORD_WORDS 0x52
#define T3D_PARSED_RESOURCE_FLAG_VALUE 0x01
#define T3D_PARSED_RESOURCE_FLAG_HEADER 0x02
#define T3D_PARSED_RESOURCE_FLAG_HIERARCHICAL_POINTER 0x80
#define T3D_PARSED_RESOURCE_FLAG_SET_BY_CONTEXT 0x10
#define T3D_PARSED_RESOURCE_HEADER_SIZE 0x1c

typedef void (__cdecl *T3dWorldLoadProgressCallback_guess)(void *callbackData,
    unsigned int phase, unsigned int completed, unsigned int total);

typedef struct T3dWorldLoadState_guess
{
    unsigned char unknown0000[0x18];
    void *existingResourceTable;
    unsigned char unknown001c[8];
    int *activeResourceTable;
    unsigned char unknown0028[0x32c];
    int *nodeIndexArray;
} T3dWorldLoadState_guess;

typedef struct T3dResourceContext_guess
{
    unsigned char unknown0000[8];
    int unknown0008;
} T3dResourceContext_guess;

typedef struct T3dVolumeFileDefinition_guess
{
    int dictionaryOffset;
    unsigned int flags;
    int type;
    int entryCount;
    int extraDataOffset;
    char characterData[16];
    int values[1];
} T3dVolumeFileDefinition_guess;

typedef struct RenderVolume_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[0x14];
    struct RenderVolume_guess *relatedVolume;
} RenderVolume_guess;

typedef struct T3dLoadedResource_guess
{
    unsigned char unknown_0000[0x0c];
    void *resourceData;
} T3dLoadedResource_guess;

typedef struct T3dResourceLoadContext_guess
{
    unsigned char unknown_0000[0x34];
    unsigned int versionFlags;
    void *encryptedData;
    unsigned int unknown_003c;
    unsigned char unknown_0040[0x7ffc];
    unsigned int resourceDataFlags;
    unsigned char unknown_8040[8];
    unsigned int resourceType;
    unsigned char unknown_804c[4];
    unsigned int resourceSize;
    unsigned char unknown_8054[8];
    void *resourceData;
    T3dLoadedResource_guess *loadedResource;
} T3dResourceLoadContext_guess;

typedef struct T3dResourceHeader_guess
{
    char signature[4];
    unsigned int versionFlags;
    unsigned int resourceType;
    unsigned int resourceSize;
    unsigned int resourceDataFlags;
    unsigned int encryptedDataSize;
    unsigned int unknown_0018;
} T3dResourceHeader_guess;

#define EQGFX_RESOURCE_HEADER_SIGNATURE EQ_ARRAY(char, 0x10087084)
#define T3D_RESOURCE_HEADER_SIGNATURE_ERROR 0xffff3d5a
#define T3D_RESOURCE_HEADER_VERSION_ERROR 0xffff3d5b

typedef struct T3dSimpleSpriteContext_guess
{
    char unknown_0000[4];
    void *dictionary;
    const char *bitmapName;
    char unknown_000c[0x2c];
    int spriteOffset;
    char unknown_003c[0x8008];
    int *spriteDefinition;
    char unknown_8048[4];
    void **spriteTable;
    char unknown_8050[0x14];
    void *allocationContext;
} T3dSimpleSpriteContext_guess;

typedef struct T3dSimpleSpriteFileDefinition_guess
{
    int dataOffset;
    unsigned int flags;
    int pointerCount;
    int optionalValues[1];
} T3dSimpleSpriteFileDefinition_guess;

typedef struct T3dSimpleSpriteDictionary_guess
{
    unsigned char unknown_0000[0x19c];
    void *pointerSlotBase;
} T3dSimpleSpriteDictionary_guess;

typedef struct T3dSimpleSpriteResourceEntry_guess
{
    int unknown_0000;
    int pointerSlotIndex;
} T3dSimpleSpriteResourceEntry_guess;

typedef struct T3dBitmapMipDefinition_guess
{
    int size;
    unsigned int mipCount;
    unsigned short headerLength;
    unsigned char data[1];
} T3dBitmapMipDefinition_guess;

typedef struct T3dBitmapInfo_guess
{
    unsigned char unknown_0000[0x0c];
    unsigned int flags;
} T3dBitmapInfo_guess;

#define T3D_BITMAP_HEADER_SIZE 0x0a
#define T3D_BITMAP_MIP_LEVEL_LIMIT 0x10
#define T3D_BITMAP_INFO_FLAG_SET_BY_CONTEXT 0x10

typedef struct T3dSimpleSpriteDefinition_guess
{
    int size;
    int characterOffset;
    unsigned int flags;
} T3dSimpleSpriteDefinition_guess;

typedef struct T3dSimpleSprite_guess
{
    char unknown_00[0x0c];
    unsigned int flags;
    char unknown_0010[0x0c];
    struct T3dSimpleSpriteResourceInfo_guess *resourceInfo;
} T3dSimpleSprite_guess;

typedef struct T3dSimpleSpriteResourceInfo_guess
{
    unsigned char unknown_0000[0x0c];
    unsigned int flags;
} T3dSimpleSpriteResourceInfo_guess;

// The 8-byte rotating XOR key the cipher reads at 0x10087078: its two dwords for the whole-dword part,
// then the same eight bytes one at a time for the trailing bytes.
#define EQGFX_CRYPT_KEY_GUESS EQ_ARRAY(unsigned int, 0x10087078)

#define T3D_RECORD_TYPE_ID 0x37
#define T3D_RECORD_SIZE 0x50
#define T3D_RECORD_FLAG_CHARACTER 0x01
#define T3D_RECORD_FLAG_VALUE_A 0x02
#define T3D_RECORD_FLAG_VALUE_B 0x04
#define T3D_RECORD_FLAG_POINTER 0x08
#define T3D_RECORD_FLAG_VECTORS 0x10
#define T3D_RECORD_FLAG_EXTRA_DATA 0x20
#define T3D_RECORD_FLAG_FINAL_STATE 0x40
#define T3D_RECORD_ALLOCATION_ERROR 0xffff3d3e

#define EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(dictionary, offset) \
    EQ_CDECL(void *, 0x1000E7FE, (void *, int))((dictionary), (offset))
#define EQGFX_T3D_CREATE_POLYHEDRON(dictionary, definition) \
    EQ_CDECL(void *, 0x10018CF8, (void *, void *))((dictionary), (definition))
#define EQGFX_T3D_SET_POLYHEDRON_DEFINITION_BOUNDING_RADIUS(polyhedron, radius) \
    EQ_CDECL(void, 0x1001B0BB, (void *, float))((polyhedron), (radius))
#define EQGFX_T3D_CREATE_LIGHT(dictionary, definition) \
    EQ_CDECL(void *, 0x100140F2, (void *, void *))((dictionary), (definition))
#define EQGFX_T3D_CREATE_DIRECTIONAL_LIGHT(dictionary, definition, pointerOffset) \
    EQ_CDECL(void *, 0x10014B20, (void *, void *, int *))((dictionary), (definition), (pointerOffset))
#define EQGFX_T3D_SET_DIRECTIONAL_LIGHT_STATIC_FLAG(light) \
    EQ_CDECL(void, 0x10014628, (void *))((light))
#define EQGFX_T3D_SET_POINT_LIGHT_STATIC_INFLUENCE_FLAG(light) \
    EQ_CDECL(void, 0x10014641, (void *))((light))
#define EQGFX_T3D_ADD_POINT_LIGHT_TO_REGION(dictionary, light, region) \
    EQ_CDECL(void, 0x10014673, (void *, void *, int))((dictionary), (light), (region))
#define EQGFX_T3D_SET_DIRECTIONAL_LIGHT_DYNAMIC_SCENE_INFLUENCE_FLAG(light) \
    EQ_CDECL(void, 0x1001464C, (void *))((light))
#define EQGFX_T3D_CREATE_POINT_LIGHT_EX(dictionary, definition, pointLightDefinition, value, enabled) \
    EQ_CDECL(void *, 0x10014356, (void *, void *, int *, float, int))((dictionary), (definition), (pointLightDefinition), (value), (enabled))
#define EQGFX_T3D_CREATE_SPHERE_LIST(dictionary, definition) \
    EQ_CDECL(void *, 0x1001B047, (void *, void *))((dictionary), (definition))
#define EQGFX_T3D_SET_OBJECT_TAG(dictionary, object, tag) \
    EQ_CDECL(int, 0x100174B8, (void *, void *, int))((dictionary), (object), (tag))
#define EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(dictionary, object) \
    EQ_CDECL(void, 0x1000DC08, (void *, void *))((dictionary), (object))
#define EQGFX_T3D_ADD_DIRECTIONAL_LIGHT_TO_REGION(dictionary, light, region) \
    EQ_CDECL(void, 0x10014D38, (void *, void *, int))((dictionary), (light), (region))
#define T3D_POLYHEDRON_FLAG_SET_BY_CONTEXT 0x10
#define T3D_SPHERE_LIST_FLAG_SET_BY_CONTEXT 0x10
#define T3D_LIGHT_FLAG_SET_BY_CONTEXT 0x10
#define T3D_2D_SPRITE_FLAG_SET_BY_CONTEXT 0x10
#define T3D_BLIT_SPRITE_FLAG_SET_BY_CONTEXT 0x10
#define T3D_3D_SPRITE_FLAG_SET_BY_CONTEXT 0x10
#define T3D_COMPOSITE_SPRITE_FLAG_SET_BY_CONTEXT 0x10
#define T3D_HIERARCHICAL_SPRITE_FLAG_SET_BY_CONTEXT 0x10
#define T3D_TRACK_FLAG_SET_BY_CONTEXT 0x10
#define T3D_LIGHT_DEFINITION_FLAG_VALUE_A 0x01
#define T3D_LIGHT_DEFINITION_FLAG_VALUE_B 0x02
#define T3D_LIGHT_DEFINITION_FLAG_POINTER 0x08
#define T3D_LIGHT_DEFINITION_FLAG_COLOR_INTENSITY_LIST 0x10
#define T3D_DIRECTIONAL_LIGHT_FILE_FLAG_STATIC 0x20
#define T3D_POINT_LIGHT_FILE_FLAG_STATIC 0x20
#define T3D_POINT_LIGHT_FILE_FLAG_STATIC_INFLUENCE 0x40
#define T3D_POINT_LIGHT_FILE_FLAG_REGIONS 0x80
#define T3D_POINT_LIGHT_FILE_FLAG_DYNAMIC_SCENE_INFLUENCE 0x01
#define T3D_POLYGON_FILE_FLAG_ORIGIN 0x01
#define T3D_POLYGON_FILE_FLAG_SCALE 0x02
#define T3D_POLYGON_FILE_FLAG_STORED_NORMALS 0x40
#define T3D_POLYGON_FILE_RECORD_SIZE 0x50
#define T3D_POLYGON_BUILD_RECORD_SIZE 0x2c
#define T3D_NORMAL_EDGE_POLYGON_TYPE 0x34
#define EQGFX_FLOAT_ZERO EQ_ARRAY(float, 0x10076284)

typedef struct T3dNormalEdge_guess
{
    int type;
    float *startVertex;
    float *endVertex;
    float length;
    float inverseLength;
} T3dNormalEdge_guess;

typedef struct T3dNormalEdgePolygon_guess
{
    int type;
    int edgeCount;
    T3dNormalEdge_guess *edges;
    float normalX;
    float normalY;
    float normalZ;
    int planeDistanceBits;
    int dominantNormalAxis;
} T3dNormalEdgePolygon_guess;

typedef struct T3dPolyhedronFileDefinition_guess
{
    int unknown0000;
    int pointerOffset;
    unsigned char flags;
    unsigned char unknown0009[3];
    float boundingRadius;
} T3dPolyhedronFileDefinition_guess;

typedef struct T3dPolygonFileDefinition_guess
{
    int dictionaryOffset;
    unsigned char flags;
    unsigned char unknown0005[3];
    int vertexCount;
    int polygonCount;
    int volumePointerOffset;
    float origin[3];
    float scale;
} T3dPolygonFileDefinition_guess;

typedef struct T3dPolygonBuildRecord_guess
{
    unsigned int type;
    int vertexCount;
    void *vertices;
    const int *sourceVertices;
    float normal[3];
    int planeDistanceBits;
    int polygonIndex;
    struct T3dPolygonBuildRecord_guess *relatedPolygonA;
    struct T3dPolygonBuildRecord_guess *relatedPolygonB;
} T3dPolygonBuildRecord_guess;

typedef struct T3dPolyhedron_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[0x18];
    struct T3dPolyhedron_guess *relatedObject;
    RenderVolume_guess *volume;
} T3dPolyhedron_guess;

typedef struct T3dPolyhedronBuildResult_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[0x20];
    T3dPolyhedron_guess *relatedObject;
} T3dPolyhedronBuildResult_guess;

typedef struct T3dPolyhedronData_guess
{
    int dictionaryOffset;
    unsigned int flags;
    int vertexCount;
    int polygonCount;
    float unknown0010;
    float vertexData[1];
} T3dPolyhedronData_guess;

typedef struct T3dPolyhedronInputDefinition_guess
{
    int dictionaryOffset;
    unsigned int flags;
    int referenceCount;
    int groupCount;
    float valueA;
    float valueB;
    int volumePointerOffset;
    int data[1];
} T3dPolyhedronInputDefinition_guess;

typedef struct T3dPolyhedronGroup_guess
{
    float sourceIndex;
    float hasFlag;
    int referenceCount;
    void *references;
} T3dPolyhedronGroup_guess;

typedef struct T3dPolyhedronPointerReference_guess
{
    float sourceIndex;
    int dataCursor;
} T3dPolyhedronPointerReference_guess;

typedef struct T3dParserContext_guess
{
    unsigned char unknown0000[4];
    void *dictionary;
    unsigned char unknown0008[0x30];
    int dictionarySize;
    unsigned char unknown003c[0x8000];
    unsigned int resourceBufferCapacity;
    unsigned int resourceBufferLength;
    T3dPolyhedronFileDefinition_guess *polyhedronDefinition;
    unsigned char unknown8048[4];
    void **pointerTable;
    unsigned char unknown8050[0x14];
    int setPolyhedronFlag;
} T3dParserContext_guess;

typedef struct T3dActorFileDefinition_guess
{
    int dictionaryOffset;
    int actorDefinitionOffset;
    unsigned int flags;
    int volumePointerOffset;
    float data[1];
} T3dActorFileDefinition_guess;

typedef struct T3dActorInstance_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3dActorInstance_guess;

typedef struct T3dParsedResourceFileDefinition_guess
{
    int dictionaryOffset;
    unsigned int flags;
    int pointerOffset;
    int sectionCount;
    int pointerOffsetCount;
    int volumePointerOffset;
    int sectionData[1];
} T3dParsedResourceFileDefinition_guess;

typedef struct T3dParsedResource_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[0x18];
    struct T3dParsedResource_guess *relatedResource;
} T3dParsedResource_guess;

typedef struct T3dLightFileDefinition_guess
{
    int tagOffset;
    int pointerOffset;
} T3dLightFileDefinition_guess;

typedef struct T3dLightDefinition_guess
{
    int pointerOffset;
    unsigned int flags;
    int colorIntensityCount;
    int values[1];
} T3dLightDefinition_guess;

typedef struct T3dPointLightFileDefinition_guess
{
    int tagOffset;
    int pointerOffset;
    int lightDefinitionPointerOffset;
    unsigned int lightDefinitionFlags;
    int colorIntensityCount;
    float pointLightValue;
} T3dPointLightFileDefinition_guess;

typedef struct T3dPointLightRegionFileDefinition_guess
{
    int tagOffset;
    int pointerOffset;
    unsigned char flags;
    unsigned char dynamicSceneInfluence;
    unsigned char unknown000a[2];
    int lightDefinitionPointerOffset;
    unsigned char unknown0010[8];
    float pointLightValue;
    unsigned int regionCount;
    int regionIds[1];
} T3dPointLightRegionFileDefinition_guess;

typedef struct T3dDirectionalLightFileDefinition_guess
{
    int tagOffset;
    int pointerOffset;
    int lightDefinitionPointerOffset;
    unsigned char unknown000c[8];
    int regionCount;
    int regionIds[1];
} T3dDirectionalLightFileDefinition_guess;

// Directional light definition with a flags byte at 0x8 (read by createDirectionalLightFromFileContext_guess, 0x10024984): light pointer offset at 0xc, regionCount at 0x18.
typedef struct T3dDirectionalLightFlaggedFileDefinition_guess
{
    int tagOffset;
    int pointerOffset;
    unsigned char flags;
    unsigned char unknown0009[3];
    int lightDefinitionPointerOffset;
    unsigned char unknown0010[8];
    int regionCount;
    int regionIds[1];
} T3dDirectionalLightFlaggedFileDefinition_guess;

typedef struct T3dLightColorIntensityList_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3dLightColorIntensityList_guess;

typedef struct T3dLight_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[0x10];
    T3dLightColorIntensityList_guess *colorIntensityList;
} T3dLight_guess;

typedef struct T3d2DSpriteFileContext_guess
{
    int tagOffset;
    int pointerOffset;
} T3d2DSpriteFileContext_guess;

typedef struct T3dBlitSpriteFileContext_guess
{
    int tagOffset;
    int pointerOffset;
} T3dBlitSpriteFileContext_guess;

typedef struct T3dBlitSpriteDefinitionFileContext_guess
{
    int tagOffset;
    unsigned int unknown0004;
    int pointerOffset;
    int spriteDefinition;
} T3dBlitSpriteDefinitionFileContext_guess;

typedef struct T3d3DSpriteFileContext_guess
{
    int tagOffset;
    int pointerOffset;
} T3d3DSpriteFileContext_guess;

typedef struct T3dCompositeSpriteFileContext_guess
{
    int tagOffset;
    int pointerOffset;
} T3dCompositeSpriteFileContext_guess;

typedef struct T3dHierarchicalSpriteFileContext_guess
{
    int tagOffset;
    int pointerOffset;
} T3dHierarchicalSpriteFileContext_guess;

typedef struct T3d2DSprite_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3d2DSprite_guess;

typedef struct T3dBlitSpriteAssociatedObject_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3dBlitSpriteAssociatedObject_guess;

typedef struct T3dBlitSprite_guess
{
    int type;
    unsigned int unknown0004;
    void *tag;
    unsigned int flags;
    unsigned int unknown0010;
    T3dBlitSpriteAssociatedObject_guess *associatedObject;
} T3dBlitSprite_guess;

typedef struct T3d3DSprite_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3d3DSprite_guess;

typedef struct T3dCompositeSprite_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3dCompositeSprite_guess;

typedef struct T3dHierarchicalSprite_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[0x0c];
    struct T3dHierarchicalSpriteRelatedObject_guess *relatedObject;
} T3dHierarchicalSprite_guess;

typedef struct T3dHierarchicalSpriteRelatedObject_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3dHierarchicalSpriteRelatedObject_guess;

typedef struct T3dHierarchicalSpriteFileDefinition_guess
{
    int dictionaryOffset;
    unsigned char recordFlags;
    unsigned char pointerListFlags;
    unsigned char resourceFlags;
    unsigned char unknown0007;
    int recordCount;
    int volumePointerOffset;
    int recordData[1];
} T3dHierarchicalSpriteFileDefinition_guess;

typedef struct T3dHierarchicalSpriteFileRecord_guess
{
    int dictionaryOffset;
    int unknown0004;
    int firstPointerOffset;
    int secondPointerOffset;
    int pointerCount;
    int pointerOffsets[1];
} T3dHierarchicalSpriteFileRecord_guess;

typedef struct T3dTrackFileContext_guess
{
    int dictionaryOffset;
    int pointerOffset;
    unsigned int flags;
    int skipFrames;
} T3dTrackFileContext_guess;

typedef struct T3dTrackDef2FileContext_guess
{
    int dictionaryOffset;
    unsigned char flags;
    unsigned char unknown0005[3];
    int recordCount;
    unsigned int recordData[1];
} T3dTrackDef2FileContext_guess;

typedef struct T3dTrackBranch_guess T3dTrackBranch_guess;

typedef struct T3dTrack_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[4];
    T3dTrackBranch_guess *trackBranchA;
    T3dTrackBranch_guess *trackBranchB;
} T3dTrack_guess;

typedef struct T3dTrackBranch_guess
{
    unsigned char unknown0000[8];
    T3dTrack_guess *track;
} T3dTrackBranch_guess;

#define EQGFX_TRACK_DEF2_ALLOCATION_ATTEMPTS EQ_ARRAY(unsigned int, 0x100AADE8)

typedef struct T3dEncodedRecord_guess
{
    unsigned int typeId;
    unsigned int flags;
    int values[1];
} T3dEncodedRecord_guess;

typedef struct T3dParsedRecord_guess
{
    unsigned int typeId;
    unsigned int sourceValue;
    unsigned int valueA;
    unsigned int valueB;
    unsigned char unknown0010[8];
    unsigned char enabled;
    unsigned char unknown0019;
    char characterValue;
    unsigned char unknown001b;
    void *resolvedPointer;
    float vectors[3][3];
    void *extraData;
    unsigned int unknown0048;
    unsigned int finalState;
} T3dParsedRecord_guess;

// The sphere source createSphereFromFileContext_guess reads its radius from: a float at +0x04 (no include/ header has it).
typedef struct T3dSphereSource_guess
{
    unsigned int unknown00;
    float radius;
} T3dSphereSource_guess;

// The object createSphereFromFileContext_guess is passed: the sphere source pointer at +0x8044 (no include/ header has it).
typedef struct T3dSphereOwner_guess
{
    unsigned char unknown0000[0x8044];
    T3dSphereSource_guess *sphereSource;
} T3dSphereOwner_guess;

// The sphere list definition at the parser context's +0x8044 (no include/ header has it): a tag offset, a pointer
// offset, flags (bit 0: bounding radius) and the radius.
typedef struct T3dSphereListFileDefinition_guess
{
    int tagOffset;
    int pointerOffset;
    unsigned char flags;
    unsigned char unknown0009[3];
    float boundingRadius;
} T3dSphereListFileDefinition_guess;

typedef struct T3dSphereListDataFileDefinition_guess
{
    int pointerOffset;
    unsigned char flags;
    unsigned char unknown0005[3];
    int sphereCount;
    float sphereValue;
    int recordData[1];
} T3dSphereListDataFileDefinition_guess;

typedef struct T3dSphereListData_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
} T3dSphereListData_guess;

// The sphere list object createSphereListFromFileContext_guess returns (no include/ header has it).
typedef struct T3dSphereList_guess
{
    unsigned char unknown0000[0x0c];
    unsigned int flags;
    unsigned char unknown0010[0x14];
    T3dSphereListData_guess *sphereData;
} T3dSphereList_guess;

typedef struct T3dEncryptedMemory_guess
{
    unsigned int size;
    unsigned char data[1];
} T3dEncryptedMemory_guess;

typedef struct T3dResourceData_guess
{
    unsigned char unknown0000[0x14];
    void *allocatedData;
    unsigned char unknown0018[0x30];
    void *decryptedData;
} T3dResourceData_guess;

typedef struct T3dEncryptedMemoryOwner_guess
{
    unsigned char unknown0000[4];
    T3dResourceData_guess *resourceData;
    unsigned char unknown0008[0x803c];
    T3dEncryptedMemory_guess *encryptedMemory;
} T3dEncryptedMemoryOwner_guess;

typedef struct T3dAsyncFileReader_guess
{
    const char *fileName;
    void *worldState;
    void *fastFileContext;
    unsigned char criticalSection[0x18];
    unsigned int readOffset;
    unsigned int bufferPosition;
    int readComplete;
    int threadComplete;
    unsigned char unknown0034[4];
    void *additionalAllocation;
    unsigned char buffer[T3D_ASYNC_READ_BUFFER_SIZE];
    unsigned int allocationSize;
    unsigned int unknown8040;
    void *secondaryBuffer;
    unsigned int resourceCount;
    int *resourcePointers;
    unsigned int pointerCount;
    unsigned int initializedValue;
    void *resourceTable;
    unsigned int unknown805c;
    T3dResourceContext_guess *resourceContext;
    int resultContextValue;
    T3dWorldLoadProgressCallback_guess progressCallback;
    void *callbackData;
} T3dAsyncFileReader_guess;

// FUNCTION: EQGFX 0x10021e85
// Loads world resources from a file and reports loading progress.
int t3dLoadBinaryWorldFromQueuedFile_guess(const char *fileName, T3dWorldLoadState_guess *worldState,
    void *fastFileContext, int result,
    T3dWorldLoadProgressCallback_guess progressCallback, void *callbackData)
{
    T3dAsyncFileReader_guess readContext;
    unsigned int threadResult;
    unsigned int index;
    unsigned int nextIndex;
    int resourceHandle;
    void *allocationResult;

    readContext.fileName = fileName;
    readContext.fastFileContext = fastFileContext;
    EQGFX_WORLD_LOAD_MODE = result;
    readContext.worldState = worldState;
    EQGFX_INITIALIZE_CRITICAL_SECTION(readContext.criticalSection);
    readContext.progressCallback = progressCallback;
    readContext.callbackData = callbackData;
    readContext.readOffset = 0;
    readContext.bufferPosition = 0;
    readContext.readComplete = 0;
    readContext.threadComplete = 0;
    readContext.additionalAllocation = 0;
    readContext.resourcePointers = 0;
    readContext.secondaryBuffer = 0;
    readContext.resourceTable = 0;
    readContext.resourceContext = 0;
    readContext.resultContextValue = result;

    threadResult = EQ_CDECL(unsigned int, 0x10069080,
        (void (__cdecl *)(void *), unsigned int, void *))(
            (void (__cdecl *)(void *))0x1002209A, 0, &readContext);
    if (threadResult == 0xffffffff)
        goto allocation_failure;

    result = EQ_CDECL(int, 0x100222E7, (void *))(&readContext);
    if (result != 0)
        goto cleanup;

    readContext.resourcePointers = (int *)EQ_CDECL(void *, 0x10050C48,
        (unsigned int))(readContext.resourceCount * 4 + 4);
    if (readContext.resourcePointers != 0 &&
        ((*readContext.resourcePointers = 0),
         readContext.allocationSize == 0 ||
         (readContext.secondaryBuffer = EQ_CDECL(void *, 0x10050C48,
             (unsigned int))(readContext.allocationSize)) != 0))
    {
        readContext.initializedValue = 0;
        if (readContext.pointerCount == 0)
            goto read_resources;

        if (worldState->existingResourceTable == 0 &&
            (readContext.resourceTable = EQ_CDECL(void *, 0x10050C48,
                (unsigned int))(readContext.pointerCount * 0x38)) != 0)
        {
            allocationResult = EQ_CDECL(void *, 0x10050C48,
                (unsigned int))(readContext.pointerCount << 2);
            worldState->nodeIndexArray = (int *)allocationResult;
            if (allocationResult != 0)
                goto read_resources;
        }
    }
    goto allocation_failure;

read_resources:
    index = 1;
    if (readContext.resourceCount != 0)
    {
        do
        {
            resourceHandle = EQ_CDECL(int, 0x1002253B, (void *))(&readContext);
            if (resourceHandle == 0)
                goto allocation_failure;
            readContext.resourcePointers[index] = resourceHandle;
            nextIndex = index + 1;
            if (readContext.progressCallback != 0)
                readContext.progressCallback(readContext.callbackData, 1, index,
                    readContext.resourceCount);
            index = nextIndex;
        } while (nextIndex <= readContext.resourceCount);
    }
    goto cleanup;

allocation_failure:
    result = T3D_RECORD_ALLOCATION_ERROR;
cleanup:
    EQGFX_ENTER_CRITICAL_SECTION(readContext.criticalSection);
    readContext.readComplete = 1;
    EQGFX_LEAVE_CRITICAL_SECTION(readContext.criticalSection);

    if (readContext.resourceTable != 0 &&
        *worldState->activeResourceTable != (int)readContext.resourceTable)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(readContext.resourceTable);
        if (worldState->nodeIndexArray != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(worldState->nodeIndexArray);
    }
    if (readContext.secondaryBuffer != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(readContext.secondaryBuffer);
    if (readContext.resourcePointers != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(readContext.resourcePointers);
    if (readContext.additionalAllocation != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(readContext.additionalAllocation);
    if (readContext.resourceContext != 0 && readContext.resourceContext->unknown0008 == 0)
        EQ_CDECL(void, 0x1002607F, (void *))(readContext.resourceContext);
    while (readContext.threadComplete == 0)
        EQGFX_SLEEP(0x14);
    EQGFX_DELETE_CRITICAL_SECTION(readContext.criticalSection);
    return result;
}

// FUNCTION: EQGFX 0x1002209a
// Reader thread: streams a file (fast-file or Win32) into the loader's 32 KB ring buffer until EOF or error.
int t3dReadDataFromBinaryFile(T3dAsyncFileReader_guess *readContext)
{
    T3dAsyncFileReader_guess *context;
    int fastFileHandle;
    int shortRead;
    void *fileHandle;
    unsigned int bytesRead;
    unsigned int eofBytesRemaining;
    unsigned int firstChunkSize;
    unsigned int bytesToRead;
    unsigned int readOffset;
    int readResult;

    context = readContext;
    fastFileHandle = 0;
    shortRead = 0;
    if (readContext->fastFileContext == 0)
    {
        fileHandle = EQGFX_CREATE_FILE_A(context->fileName, 0x80000000, 1, 0, 3, 0x80, 0);
        if (fileHandle != (void *)-1)
            goto read_loop;
        EQGFX_ENTER_CRITICAL_SECTION(&context->criticalSection);
        context->readComplete = 1;
    }
    else
    {
        fastFileHandle = EQ_CDECL(int, 0x10021791, (void *, const char *))
            (context->fastFileContext, context->fileName);
        if (fastFileHandle != 0)
        {
            eofBytesRemaining = EQ_CDECL(unsigned int, 0x100219A3, (int))(fastFileHandle);
            goto read_loop;
        }
        EQGFX_ENTER_CRITICAL_SECTION(&context->criticalSection);
        context->readComplete = 1;
    }
    EQGFX_LEAVE_CRITICAL_SECTION(&context->criticalSection);
    goto thread_finish;

read_loop:
    EQGFX_ENTER_CRITICAL_SECTION(&context->criticalSection);
    if (context->readComplete != 0)
    {
        EQGFX_LEAVE_CRITICAL_SECTION(&context->criticalSection);
        if (fastFileHandle == 0)
            goto close_file;
        goto close_fast_file;
    }

    readContext = (T3dAsyncFileReader_guess *)(T3D_ASYNC_READ_BUFFER_SIZE - context->bufferPosition);
    if ((unsigned int)readContext > T3D_ASYNC_READ_MIN_FREE_SPACE)
    {
        bytesToRead = (unsigned int)readContext;
        readOffset = context->readOffset + context->bufferPosition;
        EQGFX_LEAVE_CRITICAL_SECTION(&context->criticalSection);
        if (T3D_ASYNC_READ_BUFFER_SIZE < readOffset)
            readOffset -= T3D_ASYNC_READ_BUFFER_SIZE;

        if (fastFileHandle != 0)
        {
            if (eofBytesRemaining < (unsigned int)readContext)
            {
                bytesToRead = eofBytesRemaining;
                shortRead = 1;
            }
            if (bytesToRead + readOffset < T3D_ASYNC_READ_BUFFER_SIZE + 1)
                goto read_fast_file;

            firstChunkSize = T3D_ASYNC_READ_BUFFER_SIZE - readOffset;
            readResult = EQ_CDECL(int, 0x1002186A, (int, void *, unsigned int))
                (fastFileHandle, context->buffer + readOffset, firstChunkSize);
            if (readResult == 0)
                goto read_failed;
            bytesToRead -= firstChunkSize;
            readOffset = 0;

read_fast_file:
            readResult = EQ_CDECL(int, 0x1002186A, (int, void *, unsigned int))
                (fastFileHandle, context->buffer + readOffset, bytesToRead);
            if (readResult == 0)
                goto read_failed;
            eofBytesRemaining -= (unsigned int)readContext;
            goto update_buffer;
        }

        bytesToRead = (unsigned int)readContext;
        if (readOffset + (unsigned int)readContext < T3D_ASYNC_READ_BUFFER_SIZE + 1)
            goto read_file;

        firstChunkSize = T3D_ASYNC_READ_BUFFER_SIZE - readOffset;
        readResult = EQGFX_READ_FILE(fileHandle, context->buffer + readOffset,
            firstChunkSize, &bytesRead, 0);
        if (readResult == 0)
            goto wrapped_file_read_failed;
        if (firstChunkSize <= bytesRead)
        {
            bytesToRead = (unsigned int)readContext - firstChunkSize;
            readOffset = 0;
            goto read_file;
        }
        shortRead = 1;
        readContext = (T3dAsyncFileReader_guess *)bytesRead;
        goto update_buffer;

read_file:
        if (bytesToRead != 0)
        {
            readResult = EQGFX_READ_FILE(fileHandle, context->buffer + readOffset,
                bytesToRead, &bytesRead, 0);
            if (readResult == 0)
                goto read_failed;
            if (bytesRead < bytesToRead)
            {
                shortRead = 1;
                readContext = (T3dAsyncFileReader_guess *)
                    ((unsigned int)readContext + bytesRead - bytesToRead);
            }
        }

update_buffer:
        EQGFX_ENTER_CRITICAL_SECTION(&context->criticalSection);
        context->bufferPosition += (unsigned int)readContext;
        if (shortRead != 0)
            context->readComplete = 1;
    }
    EQGFX_LEAVE_CRITICAL_SECTION(&context->criticalSection);
    EQGFX_SLEEP(1);
    goto read_loop;

wrapped_file_read_failed:
    EQGFX_ENTER_CRITICAL_SECTION(&context->criticalSection);
    context->readComplete = 1;
    EQGFX_LEAVE_CRITICAL_SECTION(&context->criticalSection);
    goto close_file;

read_failed:
    EQGFX_ENTER_CRITICAL_SECTION(&context->criticalSection);
    context->readComplete = 1;
    EQGFX_LEAVE_CRITICAL_SECTION(&context->criticalSection);
    if (fastFileHandle == 0)
        goto close_file;
    goto close_fast_file;

close_file:
    EQGFX_CLOSE_HANDLE(fileHandle);
    goto thread_finish;

close_fast_file:
    EQ_CDECL(void, 0x10021838, (int))(fastFileHandle);

thread_finish:
    context->threadComplete = 1;
    EQ_CDECL(void, 0x1006918A, (void))();
    return 0;
}

// FUNCTION: EQGFX 0x100222e7
// Validates a resource file's format and prepares its resource metadata and encoded data.
int t3dReadBinaryResourceHeader_guess(T3dResourceLoadContext_guess *resource)
{
    T3dResourceHeader_guess header;
    int result;
    void *allocatedData;

    result = EQ_CDECL(int, 0x10022463,
        (T3dResourceLoadContext_guess *, T3dResourceHeader_guess *, unsigned int))
        (resource, &header, 0x1c);
    if (result == 0)
    {
        result = EQ_CDECL(int, 0x100689E0,
            (const char *, const char *, unsigned int))
            (header.signature, EQGFX_RESOURCE_HEADER_SIGNATURE, 4);
        if (result == 0)
        {
            if (((header.versionFlags & 0xffff0000) == 0x10000000)
                || ((header.versionFlags & 0xfffffffe) == 0x15500))
            {
                resource->loadedResource = 0;
                resource->resourceData = 0;
                resource->resourceType = header.resourceType;
                resource->versionFlags = header.versionFlags;
                resource->resourceSize = header.resourceSize;
                resource->resourceDataFlags = header.resourceDataFlags;
                if ((header.versionFlags & 1) != 0)
                {
                    result = EQ_CDECL(int, 0x10025FB1,
                        (T3dResourceLoadContext_guess *))(resource);
                    if (result != 0)
                        return result;
                    resource->resourceData = resource->loadedResource->resourceData;
                    result = 0;
                }
                if (header.encryptedDataSize > 0)
                {
                    allocatedData = EQ_CDECL(void *, 0x10050C48,
                        (unsigned int))(header.encryptedDataSize);
                    resource->encryptedData = allocatedData;
                    if (allocatedData == 0)
                    {
                        result = T3D_RECORD_ALLOCATION_ERROR;
                    }
                    else
                    {
                        result = EQ_CDECL(int, 0x10022463,
                            (T3dResourceLoadContext_guess *, void *, unsigned int))
                            (resource, allocatedData, header.encryptedDataSize);
                        if (result == 0)
                        {
                            EQ_CDECL(void, 0x100223F5,
                                (unsigned int *, unsigned int *, unsigned int))
                                ((unsigned int *)allocatedData, (unsigned int *)allocatedData,
                                 header.encryptedDataSize);
                        }
                        else
                        {
                            EQ_CDECL(void, 0x10050CBB, (void *))(resource->encryptedData);
                            resource->encryptedData = 0;
                        }
                    }
                }
            }
            else
            {
                result = T3D_RESOURCE_HEADER_VERSION_ERROR;
            }
        }
        else
        {
            result = T3D_RESOURCE_HEADER_SIGNATURE_ERROR;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x100223f5
// Decodes encrypted renderer data used by resource loading.
// @early-stop: EBP frame, pointer roles, and tail destination setup still differ.
// __cdecl. XORs `size` bytes from src into dst with the 8-byte key at 0x10087078: whole dwords first,
// then the trailing 0-3 bytes; the key pointer wraps after its last word (after its last byte in the tail).
void __cdecl decryptData_guess(unsigned int *dst, unsigned int *src, unsigned int size)
{
    unsigned int *key;
    unsigned int *s = src;
    unsigned int *end = src + (size >> 2);
    unsigned char *byteKey;
    unsigned char *byteEnd;

    key = EQGFX_CRYPT_KEY_GUESS;
    if (src < end)
    {
        do
        {
            *dst++ = *key ^ *s++;
            if (key < EQGFX_CRYPT_KEY_GUESS + 1)
                key++;
            else
                key = EQGFX_CRYPT_KEY_GUESS;
        } while (s < end);
    }

    byteKey = (unsigned char *)key;
    byteEnd = (unsigned char *)src + size;
    if ((unsigned char *)s < byteEnd)
    {
        int dstOffset = (int)dst - (int)s;

        do
        {
            *(unsigned char *)(dstOffset + (int)s) = *byteKey ^ *(unsigned char *)s;
            s = (unsigned int *)((unsigned char *)s + 1);
            if (byteKey < (unsigned char *)EQGFX_CRYPT_KEY_GUESS + 7)
                byteKey++;
            else
                byteKey = (unsigned char *)EQGFX_CRYPT_KEY_GUESS;
        } while ((unsigned char *)s < byteEnd);
    }
}

// The queue t3dReadBytesFromQueue_guess drains (offsets from the asm; no include/ header has it): CRITICAL_SECTION at
// +0x0C, read position +0x24, stored byte count +0x28, aborted flag +0x2C, 0x8000-byte ring +0x3C.
typedef struct QueueStream_guess
{
    unsigned char unknown0[0xc];  // 0x00
    unsigned char lock[0x18];     // 0x0C: CRITICAL_SECTION
    unsigned int position;        // 0x24: next byte of the ring to read, wraps at 0x8000
    unsigned int available;       // 0x28: bytes stored and not yet read
    int aborted;                  // 0x2C: nonzero once the producer has stopped
    unsigned char unknown30[0xc]; // 0x30
    unsigned char data[0x8000];   // 0x3C: the ring itself
} QueueStream_guess;

// Kernel32 entry points the client keeps in pointers (the import behind its jump thunk), as eqaddr.h shows.
typedef void (__stdcall *EqgfxCriticalSectionFunction_guess)(void *criticalSection);
typedef void (__stdcall *EqgfxSleepFunction_guess)(unsigned long milliseconds);

#define EQGFX_ENTER_CRITICAL_SECTION EQ_GLOBAL(EqgfxCriticalSectionFunction_guess, 0x100760B4)
#define EQGFX_LEAVE_CRITICAL_SECTION EQ_GLOBAL(EqgfxCriticalSectionFunction_guess, 0x100760B8)
#define EQGFX_SLEEP                  EQ_GLOBAL(EqgfxSleepFunction_guess, 0x100760B0)

// FUNCTION: EQGFX 0x10022463
// Supplies buffered file data to resource loading, waiting for more data when necessary.
// __cdecl. Blocks until `size` bytes are copied from the queue at `queue` into `dst` (lock at +0x0C, ring
// at +0x3C wrapping, Sleep(1) while empty); returns 0, or 0xFFFF3D3E when the queue was aborted meanwhile.
unsigned int __cdecl t3dReadBytesFromQueue_guess(unsigned int queue, void *dst, unsigned int size)
{
    QueueStream_guess *stream;
    unsigned int count;
    unsigned int position;
    unsigned int first;

    stream = (QueueStream_guess *)queue;
    while (size != 0)
    {
        EQGFX_ENTER_CRITICAL_SECTION(&stream->lock);
        if (stream->available != 0)
        {
            queue = size;
            if (stream->available < size)
                queue = stream->available;
            count = queue;
            position = stream->position;
            if (position + count > 0x8000)
            {
                first = 0x8000 - position;
                EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))
                    (dst, &stream->data[position], first);
                dst = (char *)dst + first;
                stream->position = 0;
                count = count - first;
            }
            EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))
                (dst, &stream->data[stream->position], count);
            stream->position += count;
            dst = (char *)dst + count;
            if (stream->position >= 0x8000)
                stream->position -= 0x8000;
            stream->available -= queue;
            EQGFX_LEAVE_CRITICAL_SECTION(&stream->lock);
            size -= queue;
        }
        else if (stream->aborted != 0)
        {
            EQGFX_LEAVE_CRITICAL_SECTION(&stream->lock);
            break;
        }
        else
        {
            EQGFX_LEAVE_CRITICAL_SECTION(&stream->lock);
            EQGFX_SLEEP(1);
        }
    }
    return -(size != 0) & 0xFFFF3D3E;
}

// FUNCTION: EQGFX 0x1002253b
// Reads one record (length/type header, then body) from the file stream and runs that record type's loader.
int __cdecl t3dParseBinaryRecord_guess(T3dParserContext_guess *context)
{
    struct
    {
        unsigned int resourceLength;
        unsigned int recordType;
    } header;
    int callResult;
    int result;

    result = 0;
    callResult = EQ_CDECL(int, 0x10022463, (void *, void *, unsigned int))
        (context, &header, 8);
    if (callResult == 0)
    {
        if (header.resourceLength > context->resourceBufferCapacity)
        {
            if (context->polyhedronDefinition != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(context->polyhedronDefinition);
            callResult = EQ_CDECL(int, 0x10050C48, (unsigned int))(header.resourceLength);
            context->polyhedronDefinition = (T3dPolyhedronFileDefinition_guess *)callResult;
            if (callResult == 0)
                return result;
            context->resourceBufferCapacity = header.resourceLength;
        }
        context->resourceBufferLength = header.resourceLength;
        callResult = EQ_CDECL(int, 0x10022463, (void *, void *, unsigned int))
            (context, context->polyhedronDefinition, header.resourceLength);
        if (callResult == 0)
        {
            switch (header.recordType)
            {
            case 1:
                result = EQ_CDECL(int, 0x1002288D, (T3dParserContext_guess *))(context);
                break;
            case 2:
                result = EQ_CDECL(int, 0x100228D8, (T3dParserContext_guess *))(context);
                break;
            case 3:
                result = EQ_CDECL(int, 0x1002291F, (T3dParserContext_guess *))(context);
                break;
            case 4:
                result = EQ_CDECL(int, 0x10022A45, (T3dParserContext_guess *))(context);
                break;
            case 5:
                result = EQ_CDECL(int, 0x10022B56, (T3dParserContext_guess *))(context);
                break;
            case 6:
                result = EQ_CDECL(int, 0x10022BFF, (T3dParserContext_guess *))(context);
                break;
            case 7:
                result = EQ_CDECL(int, 0x10023007, (T3dParserContext_guess *))(context);
                break;
            case 8:
                result = EQ_CDECL(int, 0x10023172, (T3dParserContext_guess *))(context);
                break;
            case 9:
                result = EQ_CDECL(int, 0x100234A1, (T3dParserContext_guess *))(context);
                break;
            case 0x0e:
                result = EQ_CDECL(int, 0x10023524, (T3dParserContext_guess *))(context);
                break;
            case 0x0f:
                result = EQ_CDECL(int, 0x100236C7, (T3dParserContext_guess *))(context);
                break;
            case 0x10:
                result = EQ_CDECL(int, 0x1002374A, (T3dParserContext_guess *))(context);
                break;
            case 0x11:
                result = EQ_CDECL(int, 0x100239BA, (T3dParserContext_guess *))(context);
                break;
            case 0x12:
                result = EQ_CDECL(int, 0x10023A3D, (T3dParserContext_guess *))(context);
                break;
            case 0x13:
                result = EQ_CDECL(int, 0x10023B9B, (T3dParserContext_guess *))(context);
                break;
            case 0x14:
                result = EQ_CDECL(int, 0x10023C71, (T3dParserContext_guess *))(context);
                break;
            case 0x15:
                result = EQ_CDECL(int, 0x10023F02, (T3dParserContext_guess *))(context);
                break;
            case 0x16:
                result = EQ_CDECL(int, 0x10024193, (T3dParserContext_guess *))(context);
                break;
            case 0x17:
                result = EQ_CDECL(int, 0x100241AB, (T3dParserContext_guess *))(context);
                break;
            case 0x18:
                result = EQ_CDECL(int, 0x100244AC, (T3dParserContext_guess *))(context);
                break;
            case 0x19:
                result = EQ_CDECL(int, 0x10024514, (T3dParserContext_guess *))(context);
                break;
            case 0x1a:
                result = EQ_CDECL(int, 0x1002461F, (T3dParserContext_guess *))(context);
                break;
            case 0x1b:
                result = EQ_CDECL(int, 0x100246B4, (T3dParserContext_guess *))(context);
                break;
            case 0x1c:
                result = EQ_CDECL(int, 0x10024755, (T3dParserContext_guess *))(context);
                break;
            case 0x1d:
                result = EQ_CDECL(int, 0x100248F6, (T3dParserContext_guess *))(context);
                break;
            case 0x21:
                result = EQ_CDECL(int, 0x10024BBD, (T3dParserContext_guess *))(context);
                break;
            case 0x22:
                result = EQ_CDECL(int, 0x10024C93, (T3dParserContext_guess *))(context);
                break;
            case 0x23:
                result = EQ_CDECL(int, 0x100250A6, (T3dParserContext_guess *))(context);
                break;
            case 0x24:
                result = EQ_CDECL(int, 0x10024C90, (T3dParserContext_guess *))(context);
                break;
            case 0x25:
                result = EQ_CDECL(int, 0x10024A4C, (T3dParserContext_guess *))(context);
                break;
            case 0x26:
                result = EQ_CDECL(int, 0x1002308A, (T3dParserContext_guess *))(context);
                break;
            case 0x27:
                result = EQ_CDECL(int, 0x100230F2, (T3dParserContext_guess *))(context);
                break;
            case 0x28:
                result = EQ_CDECL(int, 0x100247D5, (T3dParserContext_guess *))(context);
                break;
            case 0x29:
                result = EQ_CDECL(int, 0x100250BE, (T3dParserContext_guess *))(context);
                break;
            case 0x2a:
                result = EQ_CDECL(int, 0x10024B08, (T3dParserContext_guess *))(context);
                break;
            case 0x2b:
                result = EQ_CDECL(int, 0x10024984, (T3dParserContext_guess *))(context);
                break;
            case 0x2c:
                result = EQ_CDECL(int, 0x1002519F, (T3dParserContext_guess *))(context);
                break;
            case 0x2d:
                result = EQ_CDECL(int, 0x10025917, (T3dParserContext_guess *))(context);
                break;
            case 0x2e:
                result = EQ_CDECL(int, 0x1002599A, (T3dParserContext_guess *))(context);
                break;
            case 0x2f:
                result = EQ_CDECL(int, 0x10025A97, (T3dParserContext_guess *))(context);
                break;
            case 0x30:
                result = EQ_CDECL(int, 0x10025D64, (T3dParserContext_guess *))(context);
                break;
            case 0x31:
                result = EQ_CDECL(int, 0x10025EC8, (T3dParserContext_guess *))(context);
                break;
            case 0x32:
                result = EQ_CDECL(int, 0x10025B20, (T3dParserContext_guess *))(context);
                break;
            case 0x33:
                result = EQ_CDECL(int, 0x10025CDB, (T3dParserContext_guess *))(context);
                break;
            case 0x34:
                result = EQ_CDECL(int, 0x1002580C, (T3dParserContext_guess *))(context);
                break;
            case 0x35:
                result = EQ_CDECL(int, 0x100258FD, (T3dParserContext_guess *))(context);
                break;
            case 0x36:
                result = EQ_CDECL(int, 0x10025472, (T3dParserContext_guess *))(context);
                break;
            case 0x37:
                result = EQ_CDECL(int, 0x10025A3D, (T3dParserContext_guess *))(context);
                break;
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1002288d
// Makes the decrypted payload of an encrypted resource available to the renderer.
int __cdecl decryptResourceData_guess(T3dEncryptedMemoryOwner_guess *owner)
{
    unsigned short *encryptedMemory;
    int allocatedData;
    int oldData;

    encryptedMemory = (unsigned short *)owner->encryptedMemory;
    allocatedData = EQ_CDECL(int, 0x10050C48, (unsigned int))(*encryptedMemory);
    if (allocatedData != 0)
    {
        oldData = (int)owner->resourceData->decryptedData;
        if (oldData != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(oldData);
        EQ_CDECL(void, 0x100223F5, (void *, void *, unsigned int))
            (allocatedData, encryptedMemory + 1, *encryptedMemory);
        owner->resourceData->decryptedData = (void *)allocatedData;
    }
    return allocatedData;
}

// FUNCTION: EQGFX 0x100228D8
// Makes a decrypted copy of an encrypted resource buffer available to the renderer.
void *__cdecl decryptResourceBuffer_guess(T3dEncryptedMemoryOwner_guess *owner)
{
    T3dEncryptedMemory_guess *encryptedMemory;
    void *allocatedData;
    void *oldData;

    encryptedMemory = owner->encryptedMemory;
    allocatedData = EQ_CDECL(void *, 0x10050C48, (unsigned int))(encryptedMemory->size);
    if (allocatedData != 0)
    {
        EQ_CDECL(void, 0x100223F5, (void *, void *, unsigned int))
            (allocatedData, encryptedMemory->data, encryptedMemory->size);
        oldData = owner->resourceData->allocatedData;
        if (oldData != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(oldData);
        owner->resourceData->allocatedData = allocatedData;
    }
    return allocatedData;
}

// FUNCTION: EQGFX 0x1002291f
// Loads a bitmap resource with its mipmap levels for rendering at different sizes.
int __cdecl t3dCreateMIPMappedBMInfoFromFileContext_guess(T3dSimpleSpriteContext_guess *context)
{
    int bitmapSize;
    int allocationSize;
    int result;
    unsigned int levelIndex;
    unsigned int levelSize;
    unsigned short *mipHeader;
    void *bitmapLevels[T3D_BITMAP_MIP_LEVEL_LIMIT];
    void **levelWriteCursor;
    void *allocatedMemory;
    T3dBitmapMipDefinition_guess *bitmapDefinition;
    T3dBitmapInfo_guess **bitmapInfoReference;

    result = 0;
    allocatedMemory = 0;
    bitmapDefinition = (T3dBitmapMipDefinition_guess *)context->spriteDefinition;
    EQ_CDECL(void, 0x100223F5, (void *, void *, unsigned int))
        (bitmapDefinition->data, bitmapDefinition->data, bitmapDefinition->headerLength);
    if (bitmapDefinition->mipCount != 0)
    {
        mipHeader = (unsigned short *)((char *)bitmapDefinition + bitmapDefinition->headerLength + T3D_BITMAP_HEADER_SIZE);
        if (bitmapDefinition->mipCount < T3D_BITMAP_MIP_LEVEL_LIMIT)
        {
            bitmapLevels[0] = bitmapDefinition->data;
            levelIndex = 1;
            if (levelIndex <= bitmapDefinition->mipCount)
            {
                levelWriteCursor = &bitmapLevels[1];
                do
                {
                    levelSize = *mipHeader;
                    ++mipHeader;
                    EQ_CDECL(void, 0x100223F5, (void *, void *, unsigned int))
                        (mipHeader, mipHeader, levelSize);
                    *levelWriteCursor = mipHeader;
                    mipHeader = (unsigned short *)((char *)mipHeader + levelSize);
                    ++levelWriteCursor;
                    ++levelIndex;
                } while (levelIndex <= bitmapDefinition->mipCount);
            }
        }
        else
        {
            bitmapDefinition->mipCount = 0;
        }
    }
    if (bitmapDefinition->mipCount == 0)
        bitmapLevels[0] = bitmapDefinition->data;

    if (bitmapDefinition->size < 0)
        bitmapSize = context->spriteOffset - bitmapDefinition->size;
    else
        bitmapSize = 0;
    allocationSize = EQ_CDECL(int, 0x1000B570, (int, unsigned int, void **, const char *))
        (bitmapSize, bitmapDefinition->mipCount + 1, bitmapLevels, context->bitmapName);
    if ((allocationSize == 0)
        || ((allocatedMemory = EQ_CDECL(void *, 0x10019A5C, (void *, int, void *))
                (context->dictionary, allocationSize, context->allocationContext)) != 0))
    {
        if (bitmapDefinition->size < 0)
            bitmapSize = context->spriteOffset - bitmapDefinition->size;
        else
            bitmapSize = 0;
        bitmapInfoReference = EQ_CDECL(T3dBitmapInfo_guess **, 0x1000B5D6,
            (void *, void *, int, unsigned int, void **, const char *, int))
            (context->dictionary, allocatedMemory, bitmapSize, bitmapDefinition->mipCount + 1,
                bitmapLevels, context->bitmapName, 1);
        if (bitmapInfoReference != 0)
        {
            result = (int)*bitmapInfoReference;
            if (context->allocationContext != 0)
                ((T3dBitmapInfo_guess *)result)->flags |= T3D_BITMAP_INFO_FLAG_SET_BY_CONTEXT;
        }
        else
        {
            result = 0;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10022a45
// Loads a simple sprite definition and connects its referenced resources.
int __cdecl t3dCreateSimpleSpriteFromFileContext_guess(int contextAddress)
{
    T3dSimpleSpriteContext_guess *context;
    T3dSimpleSpriteFileDefinition_guess *definition;
    T3dSimpleSpriteResourceEntry_guess *resourceEntry;
    void *allocatedMemory;
    int *pointerStart;
    int *pointerCursor;
    int *pointerEnd;
    int valueA;
    int pointerOffset;
    int dataOffset;
    int result;

    result = 0;
    context = (T3dSimpleSpriteContext_guess *)contextAddress;
    definition = (T3dSimpleSpriteFileDefinition_guess *)context->spriteDefinition;
    pointerCursor = definition->optionalValues;
    if ((definition->flags & 4U) != 0)
    {
        valueA = *pointerCursor;
        ++pointerCursor;
    }
    else
    {
        valueA = 0;
    }
    if ((definition->flags & 8U) != 0)
    {
        contextAddress = *pointerCursor;
        ++pointerCursor;
    }
    else
    {
        contextAddress = 1;
    }
    pointerStart = pointerCursor;
    pointerEnd = pointerStart + definition->pointerCount;
    for (; pointerCursor < pointerEnd; ++pointerCursor)
    {
        pointerOffset = *pointerCursor;
        if (pointerOffset < 0)
        {
            resourceEntry = (T3dSimpleSpriteResourceEntry_guess *)
                EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                    (context->dictionary, context->spriteOffset - pointerOffset);
        }
        else
        {
            resourceEntry = (T3dSimpleSpriteResourceEntry_guess *)context->spriteTable[pointerOffset];
        }
        *pointerCursor = (int)((T3dSimpleSpriteDictionary_guess *)context->dictionary)->pointerSlotBase
            + resourceEntry->pointerSlotIndex * 4;
    }
    if (definition->dataOffset < 0)
        dataOffset = context->spriteOffset - definition->dataOffset;
    else
        dataOffset = 0;
    allocatedMemory = EQ_CDECL(void *, 0x10019A5C, (void *, int, void *))
        (context->dictionary,
            EQ_CDECL(int, 0x1001B598, (const char *, int))
                ((const char *)dataOffset, definition->pointerCount),
            context->allocationContext);
    if (allocatedMemory != 0)
    {
        if (definition->dataOffset < 0)
            dataOffset = context->spriteOffset - definition->dataOffset;
        else
            dataOffset = 0;
        result = (int)EQ_CDECL(void *, 0x1001B5BD,
            (void *, void *, int, int, int *, int, int, unsigned int))
            (context->dictionary, allocatedMemory, dataOffset, definition->pointerCount,
                pointerStart, valueA, contextAddress,
                (definition->flags >> 6) & 1);
        if ((result != 0) && (context->allocationContext != 0))
        {
            ((T3dSimpleSprite_guess *)result)->flags |= 0x10;
            ((T3dSimpleSprite_guess *)result)->resourceInfo->flags |= 0x10;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10022b56
// Creates a simple sprite from its resource definition.
int __cdecl createSimpleSpriteFromFileContext_guess(T3dSimpleSpriteContext_guess *context)
{
    T3dSimpleSpriteDefinition_guess *definition;
    int spriteLength;
    int result;
    unsigned int allocationSize;
    void *allocatedMemory;
    int characterOffset;
    void *sprite;

    result = 0;
    spriteLength = 0;
    definition = (T3dSimpleSpriteDefinition_guess *)context->spriteDefinition;
    if ((definition->size != 0) && (definition->size < 0))
        spriteLength = context->spriteOffset - definition->size;
    allocationSize = EQ_CDECL(unsigned int, 0x1001B80C, (int))(spriteLength);
    allocatedMemory = EQ_CDECL(void *, 0x10019A5C, (void *, unsigned int, void *))
        (context->dictionary, allocationSize, context->allocationContext);
    if (allocatedMemory != 0)
    {
        characterOffset = definition->characterOffset;
        if (characterOffset < 0)
            sprite = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(context->dictionary, context->spriteOffset - characterOffset);
        else
            sprite = context->spriteTable[characterOffset];
        result = (int)EQ_CDECL(void *, 0x1001B824, (void *, void *, int, void *))
            (context->dictionary, allocatedMemory, spriteLength, sprite);
        if (result != 0)
        {
            if ((definition->flags & 0x10) != 0)
                EQ_CDECL(void, 0x1000A99B, (void *, unsigned int))
                    ((void *)result, (definition->flags >> 6) & 1);
            if (context->allocationContext != 0)
                ((T3dSimpleSprite_guess *)result)->flags |= 0x10;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10022bff
// Loads record type 6: a named definition of nested dictionary-ref lists with scale and bounds (2D sprite?).
int __cdecl t3dParseBinary2DSpriteDef_guess(T3dParserContext_guess *context)
{
    T3dPolyhedronInputDefinition_guess *definition;
    void *volume;
    int *dataCursor;
    float scale;
    float radius;
    float valueA;
    float valueB;
    T3dPolyhedronGroup_guess *groupArray;
    T3dPolyhedronGroup_guess *groupArrayEnd;
    T3dPolyhedronGroup_guess *groupCursor;
    T3dPolyhedronPointerReference_guess *pointerArrayEnd;
    T3dPolyhedronPointerReference_guess *pointerCursor;
    T3dPolyhedronPointerReference_guess *savedPointerCursor;
    int *referenceEnd;
    int referenceOffset;
    void *pointerTarget;
    int result;
    T3dParsedRecord_guess parsedRecord;
    float vector[3];

    definition = (T3dPolyhedronInputDefinition_guess *)context->polyhedronDefinition;
    dataCursor = definition->data;
    if (definition->volumePointerOffset == 0)
    {
        volume = EQGFX_DEFAULT_VOLUME_POINTER;
    }
    else if (definition->volumePointerOffset < 0)
    {
        volume = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - definition->volumePointerOffset);
    }
    else
    {
        volume = context->pointerTable[definition->volumePointerOffset];
    }

    if ((definition->flags & 0x80) == 0)
        scale = 1.0f;
    else
    {
        scale = *(float *)dataCursor;
        dataCursor++;
    }
    if ((definition->flags & 1) == 0)
    {
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(vector, 0, 0xc);
    }
    else
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (vector, dataCursor, 0xc);
        dataCursor += 3;
    }
    if ((definition->flags & 2) == 0)
    {
        radius = EQ_CDECL(float, 0x1002E58C, (float *, float *, float))
            (vector, &definition->valueA, scale);
    }
    else
    {
        radius = *(float *)dataCursor;
        dataCursor++;
    }
    if ((definition->flags & 4) == 0)
        valueA = 0.0f;
    else
    {
        valueA = *(float *)dataCursor;
        dataCursor++;
    }
    if ((definition->flags & 8) == 0)
        valueB = 1.4013e-45f;
    else
    {
        valueB = *(float *)dataCursor;
        dataCursor++;
    }

    groupArray = (T3dPolyhedronGroup_guess *)EQ_CDECL(void *, 0x10050C48, (int))
        (definition->groupCount << 4);
    if (groupArray == 0)
        return 0;

    groupArrayEnd = groupArray + definition->groupCount;
    groupCursor = groupArray;
    if (groupArray < groupArrayEnd)
    {
        do
        {
            groupCursor->sourceIndex = (float)*dataCursor;
            dataCursor++;
            groupCursor->referenceCount = *dataCursor;
            dataCursor++;
            if ((((unsigned char *)groupCursor)[0xb] & 0x80) == 0)
            {
                groupCursor->hasFlag = 0.0f;
            }
            else
            {
                ((unsigned char *)groupCursor)[0xb] = ((unsigned char *)groupCursor)[0xb] & 0x7f;
                groupCursor->hasFlag = 1.4013e-45f;
            }
            groupCursor->references = EQ_CDECL(void *, 0x10050C48, (int))
                (groupCursor->referenceCount << 3);
            if (groupCursor->references == 0)
            {
                while (groupArray <= groupCursor - 1)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))((groupCursor - 1)->references);
                    groupCursor--;
                }
                EQ_CDECL(void, 0x10050CBB, (void *))(groupArray);
                return 0;
            }
            pointerArrayEnd = (T3dPolyhedronPointerReference_guess *)groupCursor->references
                + groupCursor->referenceCount;
            pointerCursor = (T3dPolyhedronPointerReference_guess *)groupCursor->references;
            if (pointerCursor < pointerArrayEnd)
            {
                do
                {
                    pointerCursor->sourceIndex = (float)*dataCursor;
                    dataCursor++;
                    pointerCursor->dataCursor = (int)dataCursor;
                    referenceEnd = dataCursor + definition->referenceCount;
                    savedPointerCursor = pointerCursor;
                    if (dataCursor < referenceEnd)
                    {
                        do
                        {
                            referenceOffset = *dataCursor;
                            if (referenceOffset < 0)
                            {
                                pointerTarget = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                                    (context->dictionary, context->dictionarySize - referenceOffset);
                                pointerCursor = savedPointerCursor;
                            }
                            else
                            {
                                pointerTarget = context->pointerTable[referenceOffset];
                            }
                            *dataCursor = (int)((char *)((T3dSimpleSpriteDictionary_guess *)
                                context->dictionary)->pointerSlotBase
                                + ((T3dSimpleSpriteResourceEntry_guess *)pointerTarget)->pointerSlotIndex * 4);
                            dataCursor++;
                        } while (dataCursor < referenceEnd);
                    }
                    pointerCursor++;
                    savedPointerCursor = pointerCursor;
                } while (pointerCursor < pointerArrayEnd);
            }
            groupCursor++;
        } while (groupCursor < groupArrayEnd);
    }

    result = EQ_CDECL(int, 0x10022EF4,
        (T3dParserContext_guess *, int **, T3dParsedRecord_guess *))
        (context, &dataCursor, &parsedRecord);
    if (result == 0)
    {
        if (definition->dictionaryOffset < 0)
            referenceOffset = context->dictionarySize - definition->dictionaryOffset;
        else
            referenceOffset = 0;
        result = EQ_CDECL(int, 0x10007681,
            (void *, int, int, int, void *, float, float, float,
                T3dParsedRecord_guess *, float, float, unsigned int, float *, void *, float))
            (context->dictionary, referenceOffset, definition->referenceCount, definition->groupCount,
                groupArray, definition->valueA, definition->valueB, scale, &parsedRecord,
                valueA, valueB, (definition->flags >> 6) & 1, vector, volume, radius);
        if (result != 0)
        {
            if (context->setPolyhedronFlag == 0)
                return result;
            ((T3dPolyhedronBuildResult_guess *)result)->flags |= 0x10;
            ((T3dPolyhedronBuildResult_guess *)result)->relatedObject->flags |= 0x10;
            return result;
        }
        if (parsedRecord.extraData != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(parsedRecord.extraData);
        }
    }

    EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context->dictionary, volume);
    groupCursor = groupArray;
    for (; groupCursor < groupArrayEnd; groupCursor++)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(groupCursor->references);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(groupArray);
    return 0;
}

// FUNCTION: EQGFX 0x10022EF4
// Decodes optional record values and resource references; their in-game meaning is unclear.
int __cdecl t3dParseBinaryRenderInfo(T3dParserContext_guess *context, int **cursorPointer, T3dParsedRecord_guess *output)
{
    int result;
    T3dEncodedRecord_guess *record;
    int *dataCursor;
    int tableIndex;
    unsigned int byteCount;
    void *allocatedData;

    result = 0;
    record = (T3dEncodedRecord_guess *)*cursorPointer;
    EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(output, 0, T3D_RECORD_SIZE);
    output->enabled = 0;
    output->typeId = T3D_RECORD_TYPE_ID;
    output->sourceValue = record->typeId;
    dataCursor = record->values;
    if ((*((unsigned char *)&record->flags) & T3D_RECORD_FLAG_CHARACTER) != 0)
    {
        output->characterValue = *(char *)dataCursor;
        dataCursor++;
    }
    if ((*((unsigned char *)&record->flags) & T3D_RECORD_FLAG_VALUE_A) != 0)
    {
        output->valueA = *dataCursor;
        dataCursor++;
    }
    if ((*((unsigned char *)&record->flags) & T3D_RECORD_FLAG_VALUE_B) != 0)
    {
        output->valueB = *dataCursor;
        dataCursor++;
    }
    if ((*((unsigned char *)&record->flags) & T3D_RECORD_FLAG_POINTER) != 0)
    {
        tableIndex = *dataCursor;
        if (tableIndex < 0)
            output->resolvedPointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                (context->dictionary, context->dictionarySize - tableIndex);
        else
            output->resolvedPointer = context->pointerTable[tableIndex];
        dataCursor++;
    }
    if ((*((unsigned char *)&record->flags) & T3D_RECORD_FLAG_VECTORS) != 0)
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (output->vectors[0], dataCursor, 0xc);
        dataCursor += 3;
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (output->vectors[1], dataCursor, 0xc);
        dataCursor += 3;
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (output->vectors[2], dataCursor, 0xc);
        dataCursor += 3;
    }
    if ((*((unsigned char *)&record->flags) & T3D_RECORD_FLAG_EXTRA_DATA) != 0)
    {
        tableIndex = *dataCursor;
        dataCursor++;
        byteCount = tableIndex * 8;
        allocatedData = EQ_CDECL(void *, 0x10050C48, (unsigned int))(byteCount);
        output->extraData = allocatedData;
        if (allocatedData == 0)
        {
            result = T3D_RECORD_ALLOCATION_ERROR;
            goto finish;
        }
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (allocatedData, dataCursor, byteCount);
        dataCursor = (int *)((unsigned char *)dataCursor + byteCount);
    }
    output->finalState = (record->flags >> 6) & 1;
finish:
    *cursorPointer = dataCursor;
    return result;
}

// FUNCTION: EQGFX 0x10023007
// Creates a drawable 2D sprite for a scene or interface.
int __cdecl create2DSpriteFromFileContext_guess(T3dParserContext_guess *context)
{
    T3d2DSpriteFileContext_guess *spriteFileContext;
    void *pointer;
    int pointerOffset;
    int tagOffset;
    int tagResult;
    T3d2DSprite_guess *sprite;

    spriteFileContext = (T3d2DSpriteFileContext_guess *)context->polyhedronDefinition;
    pointerOffset = spriteFileContext->pointerOffset;
    if (pointerOffset < 0)
        pointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - pointerOffset);
    else
        pointer = context->pointerTable[pointerOffset];
    sprite = EQ_CDECL(T3d2DSprite_guess *, 0x10007C86, (void *, void *, int))
        (context->dictionary, pointer, 0);
    if (sprite != 0)
    {
        tagOffset = spriteFileContext->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->dictionarySize - tagOffset;
            else
                tagOffset = 0;
            tagResult = EQ_CDECL(int, 0x100174B8, (void *, T3d2DSprite_guess *, int))
                (context->dictionary, sprite, tagOffset);
            if (tagResult == 0)
                EQ_CDECL(void, 0x1000DC08, (void *, T3d2DSprite_guess *))
                    (context->dictionary, sprite);
        }
        if (context->setPolyhedronFlag != 0)
            sprite->flags |= T3D_2D_SPRITE_FLAG_SET_BY_CONTEXT;
    }
    return (int)sprite;
}

// FUNCTION: EQGFX 0x1002308A
// Defines a blit sprite for scene or interface rendering.
int __cdecl createBlitSpriteFromParserContext_guess(T3dParserContext_guess *context)
{
    T3dBlitSpriteDefinitionFileContext_guess *spriteDefinitionContext;
    void *pointer;
    int pointerOffset;
    int tagOffset;
    T3dBlitSprite_guess *sprite;

    spriteDefinitionContext =
        (T3dBlitSpriteDefinitionFileContext_guess *)context->polyhedronDefinition;
    pointerOffset = spriteDefinitionContext->pointerOffset;
    if (pointerOffset < 0)
        pointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - pointerOffset);
    else
        pointer = context->pointerTable[pointerOffset];
    tagOffset = spriteDefinitionContext->tagOffset;
    if (tagOffset < 0)
        tagOffset = context->dictionarySize - tagOffset;
    else
        tagOffset = 0;
    sprite = EQ_CDECL(T3dBlitSprite_guess *, 0x1000CDC8, (void *, int, int, void *))
        (context->dictionary, tagOffset, spriteDefinitionContext->spriteDefinition, pointer);
    if ((sprite != 0) && (context->setPolyhedronFlag != 0))
    {
        sprite->flags |= T3D_BLIT_SPRITE_FLAG_SET_BY_CONTEXT;
        sprite->associatedObject->flags |= T3D_BLIT_SPRITE_FLAG_SET_BY_CONTEXT;
    }
    return (int)sprite;
}

// FUNCTION: EQGFX 0x100230F2
// Creates a blit sprite for scene or interface rendering.
int __cdecl createBlitSpriteFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dBlitSpriteFileContext_guess *spriteFileContext;
    void *pointer;
    int pointerOffset;
    int tagOffset;
    int tagResult;
    T3dBlitSprite_guess *sprite;

    spriteFileContext = (T3dBlitSpriteFileContext_guess *)context->polyhedronDefinition;
    pointerOffset = spriteFileContext->pointerOffset;
    if (pointerOffset < 0)
        pointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - pointerOffset);
    else
        pointer = context->pointerTable[pointerOffset];
    sprite = EQ_CDECL(T3dBlitSprite_guess *, 0x1000CFBF, (void *, void *))
        (context->dictionary, pointer);
    if (sprite != 0)
    {
        tagOffset = spriteFileContext->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->dictionarySize - tagOffset;
            else
                tagOffset = 0;
            tagResult = EQ_CDECL(int, 0x100174B8, (void *, T3dBlitSprite_guess *, int))
                (context->dictionary, sprite, tagOffset);
            if (tagResult == 0)
                EQ_CDECL(void, 0x1000DC08, (void *, T3dBlitSprite_guess *))
                    (context->dictionary, sprite);
        }
        if (context->setPolyhedronFlag != 0)
            sprite->flags |= T3D_BLIT_SPRITE_FLAG_SET_BY_CONTEXT;
    }
    return (int)sprite;
}

// FUNCTION: EQGFX 0x10023172
// Loads record type 8: a named set of vertices and polygons, two polygon links each; normals read or computed.
int __cdecl t3dParseBinary3DSpriteDef_guess(T3dParserContext_guess *context)
{
    T3dPolygonFileDefinition_guess *definition;
    T3dPolygonBuildRecord_guess *polygonRecords;
    T3dPolygonBuildRecord_guess *polygonRecord;
    T3dPolygonBuildRecord_guess *polygonRecordEnd;
    T3dParsedRecord_guess *parsedRecords;
    T3dParsedRecord_guess *parsedRecord;
    T3dPolyhedron_guess *polyhedron;
    void *volume;
    float origin[3];
    float scale;
    int *streamCursor;
    int *vertexCursor;
    int objectOffset;
    int vertexCount;
    int vertexBytes;
    int polygonIndex;
    int allocationSize;
    int result;

    definition = (T3dPolygonFileDefinition_guess *)context->polyhedronDefinition;
    result = 0;
    streamCursor = (int *)definition->origin;
    objectOffset = definition->volumePointerOffset;
    if (objectOffset == 0)
        volume = (void *)EQGFX_DEFAULT_VOLUME_POINTER;
    else if (objectOffset < 0)
        volume = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - objectOffset);
    else
        volume = context->pointerTable[objectOffset];

    if ((definition->flags & T3D_POLYGON_FILE_FLAG_ORIGIN) != 0)
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (origin, streamCursor, 0x0c);
        streamCursor += 3;
    }
    else
    {
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))
            (origin, 0, 0x0c);
    }

    if ((definition->flags & T3D_POLYGON_FILE_FLAG_SCALE) != 0)
    {
        scale = *(float *)streamCursor;
        streamCursor++;
    }
    else
    {
        scale = 1.0f;
    }

    vertexCursor = streamCursor;
    streamCursor += definition->vertexCount * 3;
    allocationSize = definition->polygonCount * T3D_POLYGON_FILE_RECORD_SIZE;
    parsedRecords = (T3dParsedRecord_guess *)EQ_CDECL(void *, 0x10050C48, (int))
        (allocationSize);
    if (parsedRecords != 0)
    {
        allocationSize = definition->polygonCount * T3D_POLYGON_BUILD_RECORD_SIZE;
        polygonRecords = (T3dPolygonBuildRecord_guess *)EQ_CDECL(void *, 0x10050C48, (int))
            (allocationSize);
        if (polygonRecords == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(parsedRecords);
        }
        else
        {
            polygonIndex = 0;
            polygonRecord = polygonRecords;
            parsedRecord = parsedRecords;
            polygonRecordEnd = polygonRecords + definition->polygonCount;
            if (polygonRecord < polygonRecordEnd)
            {
                do
                {
                    polygonRecord->type = T3D_NORMAL_EDGE_POLYGON_TYPE;
                    vertexCount = *streamCursor;
                    streamCursor++;
                    polygonRecord->vertexCount = vertexCount;
                    vertexBytes = vertexCount << 2;
                    polygonRecord->vertices = EQ_CDECL(void *, 0x10050C48, (int))
                        (vertexBytes);
                    if (polygonRecord->vertices == 0)
                    {
                        polygonRecord--;
                        if (polygonRecords <= polygonRecord)
                        {
                            do
                            {
                                if (parsedRecord[-1].extraData != 0)
                                    EQ_CDECL(void, 0x10050CBB, (void *))(parsedRecord[-1].extraData);
                                EQ_CDECL(void, 0x10050CBB, (void *))(polygonRecord->vertices);
                                parsedRecord--;
                                polygonRecord--;
                            } while (polygonRecords <= polygonRecord);
                        }
                        goto allocation_failure;
                    }
                    polygonRecord->sourceVertices = vertexCursor;
                    polygonRecord->polygonIndex = polygonIndex;
                    polygonIndex++;
                    vertexCount = *streamCursor;
                    streamCursor++;
                    if (vertexCount == 0)
                        polygonRecord->relatedPolygonA = 0;
                    else
                        polygonRecord->relatedPolygonA = polygonRecords + vertexCount - 1;
                    vertexCount = *streamCursor;
                    streamCursor++;
                    if (vertexCount == 0)
                        polygonRecord->relatedPolygonB = 0;
                    else
                        polygonRecord->relatedPolygonB = polygonRecords + vertexCount - 1;
                    EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
                        (polygonRecord->vertices, streamCursor, vertexBytes);
                    streamCursor = (int *)((char *)streamCursor + vertexBytes);
                    if (EQ_CDECL(int, 0x10022EF4,
                            (T3dParserContext_guess *, int **, T3dParsedRecord_guess *))
                            (context, &streamCursor, parsedRecord) != 0)
                    {
                        EQ_CDECL(void, 0x10050CBB, (void *))(polygonRecord->vertices);
                        polygonRecord--;
                        if (polygonRecords <= polygonRecord)
                        {
                            do
                            {
                                if (parsedRecord[-1].extraData != 0)
                                    EQ_CDECL(void, 0x10050CBB, (void *))(parsedRecord[-1].extraData);
                                EQ_CDECL(void, 0x10050CBB, (void *))(polygonRecord->vertices);
                                parsedRecord--;
                                polygonRecord--;
                            } while (polygonRecords <= polygonRecord);
                        }
                        goto allocation_failure;
                    }
                    if ((definition->flags & T3D_POLYGON_FILE_FLAG_STORED_NORMALS) == 0)
                    {
                        EQ_CDECL(void, 0x10011BDE, (T3dPolygonBuildRecord_guess *))(polygonRecord);
                    }
                    else
                    {
                        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
                            (polygonRecord->normal, streamCursor, 0x0c);
                        streamCursor += 3;
                        polygonRecord->planeDistanceBits = *streamCursor;
                        streamCursor++;
                    }
                    parsedRecord++;
                    polygonRecord++;
                } while (polygonRecord < polygonRecordEnd);
            }

            if (definition->dictionaryOffset < 0)
                objectOffset = context->dictionarySize - definition->dictionaryOffset;
            else
                objectOffset = 0;
            result = (int)EQ_CDECL(T3dPolyhedron_guess *, 0x10007F69,
                (void *, int, int, int *, int, T3dPolygonBuildRecord_guess *, int,
                 float *, T3dParsedRecord_guess *, void *, float))
                (context->dictionary, objectOffset, definition->vertexCount, vertexCursor,
                 definition->polygonCount, polygonRecords, (definition->flags >> 5) & 1,
                 origin, parsedRecords, volume, scale);
            if (result == 0)
            {
                EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context->dictionary, volume);
                polygonRecord = polygonRecords;
                parsedRecord = parsedRecords;
                polygonRecordEnd = polygonRecords + definition->polygonCount;
                if (polygonRecord < polygonRecordEnd)
                {
                    do
                    {
                        if (parsedRecord->extraData != 0)
                            EQ_CDECL(void, 0x10050CBB, (void *))(parsedRecord->extraData);
                        EQ_CDECL(void, 0x10050CBB, (void *)) (polygonRecord->vertices);
                        parsedRecord++;
                        polygonRecord++;
                    } while (polygonRecord < polygonRecordEnd);
                }
                EQ_CDECL(void, 0x10050CBB, (void *))(polygonRecords);
                EQ_CDECL(void, 0x10050CBB, (void *))(parsedRecords);
            }
            else if (context->setPolyhedronFlag != 0)
            {
                polyhedron = (T3dPolyhedron_guess *)result;
                polyhedron->flags |= T3D_POLYHEDRON_FLAG_SET_BY_CONTEXT;
                polyhedron->volume->flags |= T3D_VOLUME_FLAG_SET_BY_CONTEXT;
            }
        }
    }
    return result;

allocation_failure:
    EQ_CDECL(void, 0x10050CBB, (void *))(polygonRecords);
    EQ_CDECL(void, 0x10050CBB, (void *))(parsedRecords);
    result = 0;
    return result;
}

// FUNCTION: EQGFX 0x100234A1
// Creates a 3D sprite for placement in a scene.
int __cdecl create3DSpriteFromFileContext_guess(T3dParserContext_guess *context)
{
    T3d3DSpriteFileContext_guess *spriteFileContext;
    void *pointer;
    int pointerOffset;
    int tagOffset;
    int tagResult;
    T3d3DSprite_guess *sprite;

    spriteFileContext = (T3d3DSpriteFileContext_guess *)context->polyhedronDefinition;
    pointerOffset = spriteFileContext->pointerOffset;
    if (pointerOffset < 0)
        pointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - pointerOffset);
    else
        pointer = context->pointerTable[pointerOffset];
    sprite = EQ_CDECL(T3d3DSprite_guess *, 0x100084FF, (void *, void *, int))
        (context->dictionary, pointer, 0);
    if (sprite != 0)
    {
        tagOffset = spriteFileContext->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->dictionarySize - tagOffset;
            else
                tagOffset = 0;
            tagResult = EQ_CDECL(int, 0x100174B8, (void *, T3d3DSprite_guess *, int))
                (context->dictionary, sprite, tagOffset);
            if (tagResult == 0)
                EQ_CDECL(void, 0x1000DC08, (void *, T3d3DSprite_guess *))
                    (context->dictionary, sprite);
        }
        if (context->setPolyhedronFlag != 0)
            sprite->flags |= T3D_3D_SPRITE_FLAG_SET_BY_CONTEXT;
    }
    return (int)sprite;
}

// FUNCTION: EQGFX 0x10023524
// Loads a type-0x11 definition with component references; its in-game role is unclear.
int __cdecl t3dParseBinaryType17Definition_guess(int *contextDataCursor)
{
    T3dParserContext_guess *context = (T3dParserContext_guess *)contextDataCursor;
    T3dVolumeFileDefinition_guess *fileDefinition;
    void *extraData;
    char characterData[12];
    float valueA;
    int valueB;
    int pointerValue;
    void *allocatedRecords;
    void **recordCursor;
    void **recordEnd;
    int recordOffset;
    int result;
    unsigned int flags;

    fileDefinition = (T3dVolumeFileDefinition_guess *)context->polyhedronDefinition;
    recordOffset = fileDefinition->extraDataOffset;
    contextDataCursor = (int *)&fileDefinition->characterData[0];
    if (recordOffset != 0)
    {
        if (recordOffset < 0)
            extraData = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                (context->dictionary, context->dictionarySize - recordOffset);
        else
            extraData = context->pointerTable[recordOffset];
    }
    else
        extraData = EQGFX_DEFAULT_VOLUME_POINTER;

    if ((fileDefinition->flags & T3D_RECORD_FLAG_CHARACTER) != 0)
    {
        EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))
            (characterData, contextDataCursor, 12);
        contextDataCursor += 3;
    }
    else
    {
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))
            (characterData, 0, 12);
    }

    flags = fileDefinition->flags;
    if ((flags & T3D_RECORD_FLAG_VALUE_A) != 0)
    {
        *(int *)&valueA = *contextDataCursor;
        contextDataCursor++;
    }
    else
        valueA = 1.0f;
    if ((flags & T3D_RECORD_FLAG_VALUE_B) != 0)
    {
        valueB = *contextDataCursor;
        contextDataCursor++;
    }
    else
        valueB = 0;
    if ((flags & T3D_RECORD_FLAG_POINTER) != 0)
    {
        pointerValue = *contextDataCursor;
        contextDataCursor++;
    }
    else
        pointerValue = 1;

    allocatedRecords = EQ_CDECL(void *, 0x10050C48, (int))
        (fileDefinition->entryCount * T3D_VOLUME_FILE_RECORD_SIZE);
    if (allocatedRecords == 0)
        result = 0;
    else
    {
        recordEnd = (void **)allocatedRecords
            + fileDefinition->entryCount * T3D_VOLUME_POINTER_SLOT_STRIDE;
        recordCursor = (void **)allocatedRecords;
        while (recordCursor < recordEnd)
        {
            recordOffset = *contextDataCursor;
            if (recordOffset < 0)
                recordOffset = (int)EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                    (context->dictionary, context->dictionarySize - recordOffset);
            else
                recordOffset = (int)context->pointerTable[recordOffset];
            contextDataCursor++;
            *recordCursor = (void *)recordOffset;
            recordCursor += T3D_VOLUME_POINTER_SLOT_STRIDE;
        }
        if (fileDefinition->dictionaryOffset < 0)
            recordOffset = context->dictionarySize - fileDefinition->dictionaryOffset;
        else
            recordOffset = 0;
        result = EQ_CDECL(int, 0x1000D159,
            (void *, int, int, int, int *, int, int, unsigned int, void *, char *, void *, float))
            (context->dictionary, recordOffset, fileDefinition->type, fileDefinition->entryCount,
            contextDataCursor, valueB, pointerValue, (fileDefinition->flags >> 6) & 1, allocatedRecords,
            characterData, extraData, valueA);
        if (result == 0)
        {
            EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context->dictionary, extraData);
            EQ_CDECL(void, 0x10050CBB, (void *))(allocatedRecords);
        }
        else if (context->setPolyhedronFlag != 0)
        {
            ((RenderVolume_guess *)result)->flags |= T3D_VOLUME_FLAG_SET_BY_CONTEXT;
            ((RenderVolume_guess *)((RenderVolume_guess *)result)->relatedVolume)->flags
                |= T3D_VOLUME_FLAG_SET_BY_CONTEXT;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1002374a
// Loads record type 0x10: named node tree (hierarchical sprite?), each node with two object refs and children.
int __cdecl t3dParseBinaryHierarchicalSpriteDef_guess(T3dParserContext_guess *context)
{
    T3dParserContext_guess *parserContext;
    T3dHierarchicalSpriteFileDefinition_guess *fileDefinition;
    T3dHierarchicalSpriteFileRecord_guess *fileRecord;
    T3dHierarchicalSprite_guess *sprite;
    int *recordPointerEnd;
    int *outputEnd;
    int *pointerOffsets;
    int *sourceCursorAfterExtras;
    int *sourceCursor;
    int *outputRecord;
    int *allocatedRecords;
    int *extraPointerArray;
    int extraCount;
    int pointerCount;
    int dictionaryOffset;
    void *firstData;
    void *secondData;
    float boundingRadius;
    char characterStorage[16];
    int result;

    parserContext = context;
    fileDefinition = (T3dHierarchicalSpriteFileDefinition_guess *)parserContext->polyhedronDefinition;
    extraCount = 0;
    extraPointerArray = 0;
    sourceCursorAfterExtras = 0;
    sourceCursor = fileDefinition->recordData;
    if ((fileDefinition->resourceFlags & 2) == 0)
    {
        if (fileDefinition->volumePointerOffset == 0)
            context = (T3dParserContext_guess *)EQGFX_DEFAULT_VOLUME_POINTER;
        else if (fileDefinition->volumePointerOffset < 0)
            context = (T3dParserContext_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                (parserContext->dictionary, parserContext->dictionarySize - fileDefinition->volumePointerOffset);
        else
            context = (T3dParserContext_guess *)parserContext->pointerTable[fileDefinition->volumePointerOffset];
    }
    else
        context = (T3dParserContext_guess *)EQGFX_HIERARCHICAL_SPRITE_DEFAULT_POINTER;

    if ((fileDefinition->recordFlags & 1) == 0)
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(characterStorage + 4, 0, 0xc);
    else
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(characterStorage + 4, sourceCursor, 0xc);
        sourceCursor += 3;
    }
    if ((fileDefinition->recordFlags & 2) == 0)
        boundingRadius = 1.0f;
    else
    {
        *(int *)&boundingRadius = *sourceCursor;
        sourceCursor++;
    }

    allocatedRecords = (int *)EQ_CDECL(void *, 0x10050C48, (int))
        (fileDefinition->recordCount * T3D_HIERARCHICAL_SPRITE_RECORD_SIZE);
    if (allocatedRecords == 0)
        result = 0;
    else
    {
        outputEnd = allocatedRecords + fileDefinition->recordCount * T3D_HIERARCHICAL_SPRITE_RECORD_WORDS;
        outputRecord = allocatedRecords;
        if (outputRecord < outputEnd)
        {
            do
            {
                fileRecord = (T3dHierarchicalSpriteFileRecord_guess *)sourceCursor;
                pointerOffsets = (int *)(sourceCursor + 5);
                sourceCursor = pointerOffsets;
                pointerCount = fileRecord->pointerCount;
                recordPointerEnd = pointerOffsets + pointerCount;
                while (sourceCursor < recordPointerEnd)
                {
                    *sourceCursor = (int)(allocatedRecords + *sourceCursor * T3D_HIERARCHICAL_SPRITE_RECORD_WORDS);
                    sourceCursor++;
                }

                if (fileRecord->firstPointerOffset < 0)
                    firstData = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                        (parserContext->dictionary, parserContext->dictionarySize - fileRecord->firstPointerOffset);
                else
                    firstData = parserContext->pointerTable[fileRecord->firstPointerOffset];

                if (fileRecord->secondPointerOffset < 0)
                    secondData = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                        (parserContext->dictionary, parserContext->dictionarySize - fileRecord->secondPointerOffset);
                else
                    secondData = parserContext->pointerTable[fileRecord->secondPointerOffset];

                if (fileRecord->dictionaryOffset < 0)
                    dictionaryOffset = parserContext->dictionarySize - fileRecord->dictionaryOffset;
                else
                    dictionaryOffset = 0;

                EQ_CDECL(void, 0x10013676, (void *, int, void *, void *, int, int *))
                    (outputRecord, dictionaryOffset, secondData, firstData, fileRecord->pointerCount, pointerOffsets);
                outputRecord += T3D_HIERARCHICAL_SPRITE_RECORD_WORDS;
            } while (outputRecord < outputEnd);
        }

        if ((fileDefinition->pointerListFlags & 2) != 0)
        {
            extraCount = *sourceCursor;
            sourceCursor++;
            if (extraCount != 0)
                extraPointerArray = (int *)EQ_CDECL(void *, 0x10050C48, (int))(extraCount << 2);
            if (extraCount != 0)
            {
                outputRecord = extraPointerArray;
                pointerCount = extraCount;
                do
                {
                    if (*sourceCursor < 0)
                        *outputRecord = (int)EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                            (parserContext->dictionary, parserContext->dictionarySize - *sourceCursor);
                    else
                        *outputRecord = (int)parserContext->pointerTable[*sourceCursor];
                    outputRecord++;
                    sourceCursor++;
                    pointerCount--;
                } while (pointerCount != 0);
            }
            sourceCursorAfterExtras = sourceCursor;
        }

        if (fileDefinition->dictionaryOffset < 0)
            dictionaryOffset = parserContext->dictionarySize - fileDefinition->dictionaryOffset;
        else
            dictionaryOffset = 0;
        sprite = (T3dHierarchicalSprite_guess *)EQ_CDECL(void *, 0x1001244F,
            (void *, int, int, void *, void *, void *, float, int, void *, void *))
            (parserContext->dictionary, dictionaryOffset, fileDefinition->recordCount, allocatedRecords,
             characterStorage + 4, context, boundingRadius, extraCount, extraPointerArray, sourceCursorAfterExtras);
        result = (int)sprite;
        if (sprite == 0)
        {
            EQ_CDECL(void, 0x1001D5A1, (void *, void *))(parserContext->dictionary, context);
            EQ_CDECL(void, 0x10050CBB, (void *))(allocatedRecords);
        }
        else if (parserContext->setPolyhedronFlag != 0)
        {
            sprite->flags |= T3D_HIERARCHICAL_SPRITE_FLAG_SET_BY_CONTEXT;
            sprite->relatedObject->flags |= T3D_HIERARCHICAL_SPRITE_FLAG_SET_BY_CONTEXT;
        }
        if (extraCount != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(extraPointerArray);
    }
    return result;
}

// FUNCTION: EQGFX 0x100236C7
// Creates a composite sprite that groups visual parts as one scene object.
int __cdecl createCompositeSpriteFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dCompositeSpriteFileContext_guess *spriteFileContext;
    void *pointer;
    int pointerOffset;
    int tagOffset;
    int tagResult;
    T3dCompositeSprite_guess *sprite;

    spriteFileContext = (T3dCompositeSpriteFileContext_guess *)context->polyhedronDefinition;
    pointerOffset = spriteFileContext->pointerOffset;
    if (pointerOffset < 0)
        pointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - pointerOffset);
    else
        pointer = context->pointerTable[pointerOffset];
    sprite = EQ_CDECL(T3dCompositeSprite_guess *, 0x1000D4E2, (void *, void *, int))
        (context->dictionary, pointer, 0);
    if (sprite != 0)
    {
        tagOffset = spriteFileContext->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->dictionarySize - tagOffset;
            else
                tagOffset = 0;
            tagResult = EQ_CDECL(int, 0x100174B8, (void *, T3dCompositeSprite_guess *, int))
                (context->dictionary, sprite, tagOffset);
            if (tagResult == 0)
                EQ_CDECL(void, 0x1000DC08, (void *, T3dCompositeSprite_guess *))
                    (context->dictionary, sprite);
        }
        if (context->setPolyhedronFlag != 0)
            sprite->flags |= T3D_COMPOSITE_SPRITE_FLAG_SET_BY_CONTEXT;
    }
    return (int)sprite;
}

// FUNCTION: EQGFX 0x100239BA
// Creates a hierarchical sprite for a nested scene structure.
int __cdecl createHierarchicalSpriteFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dHierarchicalSpriteFileContext_guess *spriteFileContext;
    void *pointer;
    int pointerOffset;
    int tagOffset;
    int tagResult;
    T3dHierarchicalSprite_guess *sprite;

    spriteFileContext = (T3dHierarchicalSpriteFileContext_guess *)context->polyhedronDefinition;
    pointerOffset = spriteFileContext->pointerOffset;
    if (pointerOffset < 0)
        pointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - pointerOffset);
    else
        pointer = context->pointerTable[pointerOffset];
    sprite = EQ_CDECL(T3dHierarchicalSprite_guess *, 0x10012A60, (void *, void *, int))
        (context->dictionary, pointer, 0);
    if (sprite != 0)
    {
        tagOffset = spriteFileContext->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->dictionarySize - tagOffset;
            else
                tagOffset = 0;
            tagResult = EQ_CDECL(int, 0x100174B8, (void *, T3dHierarchicalSprite_guess *, int))
                (context->dictionary, sprite, tagOffset);
            if (tagResult == 0)
                EQ_CDECL(void, 0x1000DC08, (void *, T3dHierarchicalSprite_guess *))
                    (context->dictionary, sprite);
        }
        if (context->setPolyhedronFlag != 0)
            sprite->flags |= T3D_HIERARCHICAL_SPRITE_FLAG_SET_BY_CONTEXT;
    }
    return (int)sprite;
}

// FUNCTION: EQGFX 0x10023a3d
// Loads an animation track definition from stored frame data.
int *__cdecl t3dCreateTrackDef2FromFileContext_guess(T3dParserContext_guess *context)
{
    T3dTrackDef2FileContext_guess *trackDefinition;
    T3dTrack_guess *track;
    unsigned int *recordData;
    unsigned int dataEnd;
    int dictionaryPosition;
    int trackAllocSize;
    void *memory;
    void *poolMemory;
    unsigned char *trackData;

    memory = 0;
    trackDefinition = (T3dTrackDef2FileContext_guess *)context->polyhedronDefinition;
    if ((trackDefinition->flags & 8) == 0)
    {
        (*EQGFX_TRACK_DEF2_ALLOCATION_ATTEMPTS)++;
        memory = EQ_CDECL(void *, 0x10050C48, (int))
            (trackDefinition->recordCount * 0x24);
        if (memory == 0)
            return (int *)0;
        dataEnd = (unsigned int)memory + trackDefinition->recordCount * 0x24;
        recordData = trackDefinition->recordData;
        if ((unsigned int)memory < dataEnd)
        {
            trackData = (unsigned char *)memory + 0x10;
            do
            {
                *(unsigned int *)(trackData + 0x0c) = recordData[0];
                EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
                    (trackData, recordData + 1, 0x0c);
                EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
                    (trackData - 0x10, recordData + 4, 0x10);
                *(unsigned int *)(trackData + 0x10) = 0;
                recordData += 8;
                trackData += 0x24;
            } while ((unsigned int)(trackData - 0x10) < dataEnd);
        }
        if (trackDefinition->dictionaryOffset < 0)
            dictionaryPosition = context->dictionarySize - trackDefinition->dictionaryOffset;
        else
            dictionaryPosition = 0;
        track = EQ_CDECL(T3dTrack_guess *, 0x1001C7CC, (void *, int, int, void *))
            (context->dictionary, dictionaryPosition, trackDefinition->recordCount, memory);
    }
    else
    {
        if (trackDefinition->dictionaryOffset < 0)
            dictionaryPosition = context->dictionarySize - trackDefinition->dictionaryOffset;
        else
            dictionaryPosition = 0;
        trackAllocSize = EQ_CDECL(int, 0x1001C9B4, (int, int, int))
            (dictionaryPosition, trackDefinition->recordCount, context->setPolyhedronFlag);
        poolMemory = EQ_CDECL(void *, 0x10019A5C, (void *, int))
            (context->dictionary, trackAllocSize);
        if (poolMemory == 0)
            return (int *)trackDefinition->recordData;
        if (trackDefinition->dictionaryOffset < 0)
            dictionaryPosition = context->dictionarySize - trackDefinition->dictionaryOffset;
        else
            dictionaryPosition = 0;
        track = EQ_CDECL(T3dTrack_guess *, 0x1001C9D8, (void *, void *, int, int, unsigned int *))
            (context->dictionary, poolMemory, dictionaryPosition, trackDefinition->recordCount,
             trackDefinition->recordData);
    }
    if (track == 0)
    {
        if (memory != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(memory);
    }
    else if (context->setPolyhedronFlag != 0)
    {
        T3dTrackBranch_guess *selectedTrackBranch;

        if (track->trackBranchA == 0)
        {
            track->flags |= T3D_TRACK_FLAG_SET_BY_CONTEXT;
            selectedTrackBranch = track->trackBranchB;
        }
        else
        {
            track->flags |= T3D_TRACK_FLAG_SET_BY_CONTEXT;
            selectedTrackBranch = track->trackBranchA;
        }
        selectedTrackBranch->track->flags |= T3D_TRACK_FLAG_SET_BY_CONTEXT;
    }
    return (int *)track;
}

// FUNCTION: EQGFX 0x10023b9b
// Creates a track from the track definition at +0x8044 of the context; returns the track, or 0 if none was made.
int __cdecl t3dParseBinaryTrackInst(T3dParserContext_guess *context)
{
    T3dTrackFileContext_guess *trackFile;
    T3dTrack_guess *track;
    void *memory;
    void *pointer;
    int dictionaryPosition;
    int dictionaryOffset;
    int pointerOffset;
    int trackAllocSize;
    volatile int result;

    dictionaryPosition = 0;
    trackFile = (T3dTrackFileContext_guess *)context->polyhedronDefinition;
    result = 0;
    dictionaryOffset = trackFile->dictionaryOffset;
    if (dictionaryOffset != 0 && dictionaryOffset < 0)
        dictionaryPosition = context->dictionarySize - dictionaryOffset;
    trackAllocSize = EQ_CDECL(int, 0x1001CC18, (int, int))
        (dictionaryPosition, context->setPolyhedronFlag);
    memory = EQ_CDECL(void *, 0x10019A5C, (void *, int))
        (context->dictionary, trackAllocSize);
    if (memory != 0)
    {
        pointerOffset = trackFile->pointerOffset;
        if (pointerOffset < 0)
            pointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                (context->dictionary, context->dictionarySize - pointerOffset);
        else
            pointer = context->pointerTable[pointerOffset];
        track = EQ_CDECL(T3dTrack_guess *, 0x1001CC30, (void *, void *, int, void *))
            (context->dictionary, memory, dictionaryPosition, pointer);
        if (track != 0)
        {
            if ((trackFile->flags & 1) != 0)
                EQ_CDECL(void, 0x10020666, (void *, int))(track, trackFile->skipFrames);
            EQ_CDECL(void, 0x1001D243, (void *, float))(track, 1.0);
            EQ_CDECL(void, 0x1001D1A9, (void *, int))(track, (trackFile->flags >> 1) & 1);
            EQ_CDECL(void, 0x1001D1DE, (void *, int))(track, (trackFile->flags >> 2) & 1);
            if (context->setPolyhedronFlag != 0)
                track->flags |= T3D_TRACK_FLAG_SET_BY_CONTEXT;
        }
    }
    else
    {
        track = (T3dTrack_guess *)result;
    }
    return (int)track;
}

// FUNCTION: EQGFX 0x10023c71
// Builds a definition (probably an actor's) from a binary file record: entry lists, pointers, decrypted data
int __cdecl t3dParseBinaryActorDef_guess(T3dParserContext_guess *context)
{
    T3dParsedResourceFileDefinition_guess *definition;
    T3dParsedResource_guess *createdResource;
    int *inputCursor;
    int *recordCursor;
    int *recordTable;
    int *recordTableEnd;
    int *coordinateData;
    int *coordinateDataEnd;
    int *pointerSource;
    int *pointerDestination;
    int *pointerOffsetsStart;
    int *scratchPointer;
    int *decodedResourceData;
    int *volumePointer;
    int *relatedObject;
    int pointerOffset;
    int relatedTag;
    int headerValue;
    int dictionaryOffset;
    int index;
    unsigned char resourceHeader[T3D_PARSED_RESOURCE_HEADER_SIZE];

    definition = (T3dParsedResourceFileDefinition_guess *)context->polyhedronDefinition;
    pointerOffset = definition->pointerOffset;
    if (pointerOffset < 0)
    {
        relatedObject = (int *)EQ_CDECL(void *, 0x1000E7FE, (void *, int))
            (context->dictionary, context->dictionarySize - pointerOffset);
    }
    else
    {
        relatedObject = (int *)context->pointerTable[pointerOffset];
    }
    if (relatedObject == 0)
    {
        relatedTag = 0;
    }
    else
    {
        relatedTag = relatedObject[4];
    }
    if ((definition->flags & T3D_PARSED_RESOURCE_FLAG_HIERARCHICAL_POINTER) == 0)
    {
        pointerOffset = definition->volumePointerOffset;
        if (pointerOffset == 0)
        {
            volumePointer = (int *)EQGFX_DEFAULT_VOLUME_POINTER;
        }
        else if (pointerOffset < 0)
        {
            volumePointer = (int *)EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                (context->dictionary, context->dictionarySize - pointerOffset);
        }
        else
        {
            volumePointer = (int *)context->pointerTable[pointerOffset];
        }
    }
    else
    {
        volumePointer = (int *)EQGFX_HIERARCHICAL_RESOURCE_POINTER;
    }
    inputCursor = definition->sectionData;
    if ((definition->flags & T3D_PARSED_RESOURCE_FLAG_VALUE) == 0)
    {
        headerValue = 0;
    }
    else
    {
        headerValue = *inputCursor;
        inputCursor++;
    }
    if ((definition->flags & T3D_PARSED_RESOURCE_FLAG_HEADER) == 0)
    {
        EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))
            (resourceHeader, 0, T3D_PARSED_RESOURCE_HEADER_SIZE);
    }
    else
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (resourceHeader, inputCursor, T3D_PARSED_RESOURCE_HEADER_SIZE);
        inputCursor += 7;
    }
    recordTable = (int *)EQ_CDECL(void *, 0x10050C48, (int))
        (definition->sectionCount << 3);
    if (recordTable == 0)
    {
        return 0;
    }
    scratchPointer = 0;
    recordTableEnd = recordTable + definition->sectionCount * 2;
    recordCursor = recordTable;
    if (recordCursor < recordTableEnd)
    {
        do
        {
            index = *inputCursor;
            inputCursor++;
            *recordCursor = index;
            coordinateData = (int *)EQ_CDECL(void *, 0x10050C48, (int))
                (index * 0x0c);
            recordCursor[1] = (int)coordinateData;
            if (coordinateData == 0)
            {
                while (recordTable <= recordCursor - 2)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))(recordCursor[-1]);
                    recordCursor -= 2;
                }
                return 0;
            }
            coordinateDataEnd = coordinateData + *recordCursor * 3;
            while (coordinateData < coordinateDataEnd)
            {
                coordinateData[0] = *inputCursor;
                inputCursor++;
                coordinateData[1] = *inputCursor;
                inputCursor++;
                coordinateData[2] = (int)scratchPointer;
                scratchPointer = (int *)((int)scratchPointer + 1);
                coordinateData += 3;
            }
            recordCursor += 2;
        } while (recordCursor < recordTableEnd);
    }
    scratchPointer = inputCursor + definition->pointerOffsetCount;
    pointerSource = inputCursor;
    pointerOffsetsStart = inputCursor;
    pointerDestination = inputCursor;
    if (pointerDestination < scratchPointer)
    {
        do
        {
            index = *pointerSource;
            if (index < 0)
            {
                relatedObject = (int *)EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                    (context->dictionary, context->dictionarySize - index);
            }
            else
            {
                relatedObject = (int *)context->pointerTable[index];
            }
            *pointerDestination = (int)relatedObject;
            pointerDestination++;
            pointerSource++;
        } while (pointerDestination < scratchPointer);
    }
    index = *pointerSource;
    if (index == 0)
    {
        decodedResourceData = 0;
    }
    else
    {
        scratchPointer = (int *)EQ_CDECL(void *, 0x10050C48, (int))(index);
        decodedResourceData = recordTable;
        if (scratchPointer == 0)
        {
            scratchPointer = 0;
            recordCursor = recordTable;
            if (recordCursor < recordTableEnd)
            {
                do
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))(recordCursor[1]);
                    recordCursor += 2;
                } while (recordCursor < recordTableEnd);
            }
            EQ_CDECL(void, 0x10050CBB, (void *))(decodedResourceData);
            return 0;
        }
        EQ_CDECL(void, 0x100223F5, (void *, const void *, int))
            (scratchPointer, pointerSource + 1, index);
        decodedResourceData = scratchPointer;
    }
    if (definition->dictionaryOffset < 0)
    {
        dictionaryOffset = context->dictionarySize - definition->dictionaryOffset;
    }
    else
    {
        dictionaryOffset = 0;
    }
    createdResource = (T3dParsedResource_guess *)EQ_CDECL(void *, 0x100093D1,
        (void *, int, int, void *, int, void *, void *, void *, void *, float, float, int, void *, int))
        (context->dictionary, dictionaryOffset, definition->sectionCount, recordTable, relatedTag,
            decodedResourceData, resourceHeader, volumePointer, EQGFX_DEFAULT_VOLUME_POINTER,
            0.0f, 0.0f, headerValue, pointerOffsetsStart,
            (definition->flags >> 6) & 1);
    if (createdResource != 0)
    {
        if (context->setPolyhedronFlag != 0)
        {
            createdResource->flags |= T3D_PARSED_RESOURCE_FLAG_SET_BY_CONTEXT;
            createdResource->relatedResource->flags |= T3D_PARSED_RESOURCE_FLAG_SET_BY_CONTEXT;
        }
        return (int)createdResource;
    }
    if (decodedResourceData != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(decodedResourceData);
    }
    EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context->dictionary, volumePointer);
    recordCursor = recordTable;
    if (recordCursor < recordTableEnd)
    {
        do
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(recordCursor[1]);
            recordCursor += 2;
        } while (recordCursor < recordTableEnd);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(recordTable);
    return 0;
}

// FUNCTION: EQGFX 0x10023f02
// Places an actor instance from a binary file record and applies its RGB track and action; skips repeats
int __cdecl _t3dParseBinaryActorInst_guess(T3dParserContext_guess *context)
{
    T3dActorFileDefinition_guess *definition;
    T3dActorInstance_guess *actor;
    float *dataCursor;
    float *position;
    float currentAction;
    float valueA;
    float valueB;
    void *volumePointer;
    void *actorDefinitionPointer;
    void *allocatedData;
    void *logFile;
    unsigned int flags;
    int pointerOffset;
    int colorTrack;
    int allocationSize;
    int dictionaryOffset;
    int actorDefinitionNameOffset;
    int createOption;
    int result;

    definition = (T3dActorFileDefinition_guess *)context->polyhedronDefinition;
    if ((definition->flags & T3D_ACTOR_FILE_FLAG_VOLUME_POINTER) == 0)
    {
        pointerOffset = definition->volumePointerOffset;
        if (pointerOffset != 0)
        {
            if (pointerOffset < 0)
            {
                volumePointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                    (context->dictionary, context->dictionarySize - pointerOffset);
            }
            else
            {
                volumePointer = context->pointerTable[pointerOffset];
            }
        }
        else
        {
            volumePointer = 0;
        }
    }
    else
    {
        volumePointer = EQGFX_HIERARCHICAL_RESOURCE_POINTER;
    }
    flags = definition->flags;
    dataCursor = definition->data;
    if ((flags & T3D_ACTOR_FILE_FLAG_ACTION) != 0)
    {
        currentAction = *dataCursor;
        dataCursor++;
    }
    if ((flags & T3D_ACTOR_FILE_FLAG_POSITION) == 0)
    {
        position = 0;
    }
    else
    {
        position = dataCursor;
        dataCursor += 7;
    }
    if ((flags & T3D_ACTOR_FILE_FLAG_VALUE_A) == 0)
    {
        valueA = 0.0f;
    }
    else
    {
        valueA = *dataCursor;
        dataCursor++;
    }
    if ((flags & T3D_ACTOR_FILE_FLAG_VALUE_B) == 0)
    {
        valueB = 0.0f;
    }
    else
    {
        valueB = *dataCursor;
        dataCursor++;
    }
    if ((flags & T3D_ACTOR_FILE_FLAG_COLOR_TRACK) == 0)
    {
        colorTrack = 0;
    }
    else
    {
        pointerOffset = *(int *)dataCursor;
        if (pointerOffset < 0)
        {
            colorTrack = (int)EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                (context->dictionary, context->dictionarySize - pointerOffset);
        }
        else
        {
            colorTrack = (int)context->pointerTable[pointerOffset];
        }
        dataCursor++;
    }
    if (EQ_CDECL(int, 0x10019C11, (float *))(position) != 0)
    {
        result = T3D_ACTOR_RESIZE_RESULT;
    }
    else
    {
        allocationSize = *(int *)dataCursor;
        if (allocationSize == 0)
        {
            allocatedData = 0;
        }
        else
        {
            allocatedData = EQ_CDECL(void *, 0x10050C48, (int))(allocationSize);
            if (allocatedData == 0)
            {
                return 0;
            }
            dataCursor++;
            EQ_CDECL(void, 0x100223F5, (void *, const void *, int))
                (allocatedData, dataCursor, allocationSize);
        }
        pointerOffset = definition->actorDefinitionOffset;
        if (pointerOffset < 0)
        {
            actorDefinitionPointer = EQ_CDECL(void *, 0x1000E7FE, (void *, int))
                (context->dictionary, context->dictionarySize - pointerOffset);
        }
        else
        {
            actorDefinitionPointer = context->pointerTable[pointerOffset];
        }
        if (actorDefinitionPointer == 0)
        {
            logFile = EQ_CDECL(void *, 0x10068412, (const char *, const char *))
                (GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
            if (logFile != 0)
            {
                if (definition->actorDefinitionOffset < 0)
                {
                    actorDefinitionNameOffset = context->dictionarySize - definition->actorDefinitionOffset;
                }
                else
                {
                    actorDefinitionNameOffset = 0;
                }
                EQ_CDECL(int, 0x100683A5, (void *, const char *, int))
                    (logFile, EQGFX_ACTOR_MISSING_DEFINITION_MESSAGE, actorDefinitionNameOffset);
                EQ_CDECL(int, 0x10068328, (void *))(logFile);
            }
        }
        if (definition->dictionaryOffset < 0)
        {
            dictionaryOffset = context->dictionarySize - definition->dictionaryOffset;
        }
        else
        {
            dictionaryOffset = 0;
        }
        createOption = (definition->flags >> 9) & 1;
        actor = (T3dActorInstance_guess *)EQ_CDECL(void *, 0x10009A22,
            (void *, void *, int, float *, void *, int, float, float, int, void *))
            (context->dictionary, actorDefinitionPointer, dictionaryOffset, position,
                volumePointer, 0, valueB, valueA, createOption, allocatedData);
        if (volumePointer != 0)
        {
            EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context->dictionary, volumePointer);
        }
        result = (int)actor;
        if (actor != 0)
        {
            if (colorTrack != 0)
            {
                EQ_CDECL(void, 0x1000B402, (void *, int))(actor, colorTrack);
            }
            if ((definition->flags & T3D_ACTOR_FILE_FLAG_ACTION) != 0)
            {
                EQ_CDECL(void, 0x1000A72E, (void *, float))(actor, currentAction);
            }
            if ((definition->flags & T3D_ACTOR_FILE_FLAG_AUXILIARY_CALLBACK) != 0)
            {
                EQ_CDECL(void, 0x10008F98, (void *, void *))(context->dictionary, actor);
            }
            if (context->setPolyhedronFlag != 0)
            {
                actor->flags |= T3D_ACTOR_FLAG_SET_BY_CONTEXT;
            }
        }
        else
        {
            logFile = EQ_CDECL(void *, 0x10068412, (const char *, const char *))
                (GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
            if (logFile != 0)
            {
                if (definition->actorDefinitionOffset < 0)
                {
                    actorDefinitionNameOffset = context->dictionarySize - definition->actorDefinitionOffset;
                }
                else
                {
                    actorDefinitionNameOffset = 0;
                }
                EQ_CDECL(int, 0x100683A5, (void *, const char *, int, double, double, double))
                    (logFile, EQGFX_ACTOR_CREATE_FAILURE_MESSAGE, actorDefinitionNameOffset,
                        (double)position[0], (double)position[1], (double)position[2]);
                EQ_CDECL(int, 0x10068328, (void *))(logFile);
            }
            if (allocatedData != 0)
            {
                EQ_CDECL(void, 0x10050CBB, (void *))(allocatedData);
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10024193
// Creates a spherical scene primitive.
// __cdecl. Calls t3dCreateSphere (0x1001D37C) with the radius at +0x04 of the sphere source at +0x8044 of the
// object, and returns its result (the new sphere, or 0) unchanged in EAX.
void *__cdecl createSphereFromFileContext_guess(T3dSphereOwner_guess *sphereOwner)
{
    return EQ_CDECL(void *, 0x1001D37C, (float))(sphereOwner->sphereSource->radius);
}

// FUNCTION: EQGFX 0x100241ab
// Loads a polyhedron shape from stored vertices and indexed polygon boundaries.
int __cdecl t3dCreateIndexedPolyhedronFromFileContext_guess(T3dParserContext_guess *parser)
{
    T3dPolyhedronData_guess *definition;
    T3dNormalEdgePolygon_guess *polygonBuffer;
    T3dNormalEdgePolygon_guess *polygonBufferEnd;
    T3dNormalEdgePolygon_guess *currentPolygon;
    T3dNormalEdgePolygon_guess *previousPolygon;
    T3dPolyhedron_guess *polyhedron;
    float *vertices;
    float radius;
    int *polygonCursor;
    int useStoredPlane;
    int edgeCount;
    int result;

    definition = (T3dPolyhedronData_guess *)parser->polyhedronDefinition;
    result = 0;
    vertices = definition->vertexData;
    useStoredPlane = 0;
    if ((definition->flags & 0x02) != 0)
    {
        useStoredPlane = 1;
    }
    if ((definition->flags & 0x01) == 0)
    {
        radius = 1.0f;
    }
    else
    {
        radius = *vertices;
        vertices++;
    }
    polygonCursor = (int *)(vertices + definition->vertexCount * 3);
    polygonBuffer = (T3dNormalEdgePolygon_guess *)EQ_CDECL(void *, 0x10050C48, (int))(
        definition->polygonCount << 5);
    if (polygonBuffer != 0)
    {
        polygonBufferEnd = (T3dNormalEdgePolygon_guess *)((unsigned int)polygonBuffer
            + (definition->polygonCount << 5));
        for (currentPolygon = polygonBuffer; (unsigned int)currentPolygon < (unsigned int)polygonBufferEnd;
            currentPolygon++)
        {
            edgeCount = *polygonCursor;
            polygonCursor++;
            if (EQ_CDECL(int, 0x100242F9,
                (int, int *, int, float *, T3dNormalEdgePolygon_guess *, int *, int))(
                    (int)parser, polygonCursor, edgeCount, vertices, currentPolygon, 0, useStoredPlane) != 0)
            {
                previousPolygon = currentPolygon;
                while ((unsigned int)polygonBuffer <= (unsigned int)previousPolygon - 0x20)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))((previousPolygon - 1)->edges);
                    previousPolygon--;
                }
                EQ_CDECL(void, 0x10050CBB, (void *))(polygonBuffer);
                return 0;
            }
        }
        if (definition->dictionaryOffset < 0)
        {
            result = parser->dictionarySize - definition->dictionaryOffset;
        }
        else
        {
            result = 0;
        }
        result = (int)EQ_CDECL(T3dPolyhedron_guess *, 0x100188CD,
            (void *, int, float, int, void *, int, float *, float))(
                parser->dictionary, result, definition->unknown0010, definition->polygonCount,
                polygonBuffer, definition->vertexCount, vertices, radius);
        polygonBufferEnd = (T3dNormalEdgePolygon_guess *)((unsigned int)polygonBuffer
            + (definition->polygonCount << 5));
        for (currentPolygon = polygonBuffer; (unsigned int)currentPolygon < (unsigned int)polygonBufferEnd;
            currentPolygon++)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(currentPolygon->edges);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(polygonBuffer);
        if ((result != 0) && (parser->setPolyhedronFlag != 0))
        {
            polyhedron = (T3dPolyhedron_guess *)result;
            polyhedron->flags |= T3D_POLYHEDRON_FLAG_SET_BY_CONTEXT;
            polyhedron->relatedObject->flags |= T3D_POLYHEDRON_FLAG_SET_BY_CONTEXT;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x100242f9
// Builds a polygon's boundary and plane from indexed model vertices.
int __cdecl t3dBuildEdgePolygonFromVertexIndices_guess(int unused, int *indexCursorAddress, int edgeCount, float *vertices,
    T3dNormalEdgePolygon_guess *polygon, int *providedPlaneWords, int useStoredPlane)
{
    int result;
    int *indexCursor;
    int firstVertexIndex;
    int vertexIndex;
    T3dNormalEdge_guess *edge;
    T3dNormalEdge_guess *edgeEnd;
    float *startVertex;
    float *endVertex;
    float edgeLength;
    float absoluteNormalX;
    float absoluteNormalY;
    float absoluteNormalZ;

    result = 0;
    indexCursor = (int *)*indexCursorAddress;
    polygon->edgeCount = edgeCount;
    polygon->type = 0x33;
    polygon->edges = (T3dNormalEdge_guess *)EQ_CDECL(void *, 0x10050C48, (int))(edgeCount * 0x14);
    if (polygon->edges == 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        edgeEnd = polygon->edges + edgeCount;
        firstVertexIndex = *indexCursor;
        indexCursor = indexCursor + 1;
        vertexIndex = firstVertexIndex;
        for (edge = polygon->edges; edge < edgeEnd; edge = edge + 1)
        {
            edge->type = 0x32;
            startVertex = vertices + vertexIndex * 3;
            edge->startVertex = startVertex;
            vertexIndex = firstVertexIndex;
            if (edge < edgeEnd - 1)
            {
                vertexIndex = *indexCursor;
                indexCursor = indexCursor + 1;
            }
            endVertex = vertices + vertexIndex * 3;
            edge->endVertex = endVertex;
            edgeLength = EQ_CDECL(float, 0x1004EF68, (float))(
                (startVertex[0] - endVertex[0]) * (startVertex[0] - endVertex[0]) +
                (startVertex[1] - endVertex[1]) * (startVertex[1] - endVertex[1]) +
                (startVertex[2] - endVertex[2]) * (startVertex[2] - endVertex[2]));
            edge->length = edgeLength;
            if (edgeLength == *EQGFX_FLOAT_ZERO)
            {
                edge->inverseLength = 0.0f;
            }
            else
            {
                edge->inverseLength = 1.0f / edge->length;
            }
        }
        if (providedPlaneWords == 0)
        {
            if (useStoredPlane == 0)
            {
                EQ_CDECL(void, 0x10011AEA, (T3dNormalEdgePolygon_guess *))(polygon);
            }
            else
            {
                EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
                    &polygon->normalX, indexCursor, 0x0c);
                indexCursor = indexCursor + 3;
                polygon->planeDistanceBits = *indexCursor;
                indexCursor = indexCursor + 1;
            }
        }
        else
        {
            ((int *)&polygon->normalX)[0] = providedPlaneWords[0];
            ((int *)&polygon->normalX)[1] = providedPlaneWords[1];
            ((int *)&polygon->normalX)[2] = providedPlaneWords[2];
            polygon->planeDistanceBits = providedPlaneWords[3];
        }
        absoluteNormalX = polygon->normalX;
        if (absoluteNormalX < *EQGFX_FLOAT_ZERO)
        {
            absoluteNormalX = -absoluteNormalX;
        }
        absoluteNormalY = polygon->normalY;
        if (absoluteNormalY < *EQGFX_FLOAT_ZERO)
        {
            absoluteNormalY = -absoluteNormalY;
        }
        absoluteNormalZ = polygon->normalZ;
        if (absoluteNormalZ < *EQGFX_FLOAT_ZERO)
        {
            absoluteNormalZ = -absoluteNormalZ;
        }
        if ((absoluteNormalX < absoluteNormalY) || (absoluteNormalX < absoluteNormalZ))
        {
            if (absoluteNormalY < absoluteNormalZ)
            {
                polygon->dominantNormalAxis = 1;
            }
            else
            {
                polygon->dominantNormalAxis = 3;
            }
        }
        else
        {
            polygon->dominantNormalAxis = 2;
        }
    }
    *indexCursorAddress = (int)indexCursor;
    return result;
}

// FUNCTION: EQGFX 0x100244ac
// Creates a polyhedron scene object from its definition.
void *__cdecl createPolyhedronFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dPolyhedronFileDefinition_guess *definition;
    void *polyhedronDefinition;
    int pointerOffset;
    void *polyhedron;

    definition = context->polyhedronDefinition;
    pointerOffset = definition->pointerOffset;
    if (pointerOffset < 0)
    {
        polyhedronDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
            context->dictionary, context->dictionarySize - pointerOffset);
    }
    else
    {
        polyhedronDefinition = context->pointerTable[pointerOffset];
    }
    polyhedron = EQGFX_T3D_CREATE_POLYHEDRON(context->dictionary, polyhedronDefinition);
    if (polyhedron != 0)
    {
        if ((definition->flags & 1) != 0)
        {
            EQGFX_T3D_SET_POLYHEDRON_DEFINITION_BOUNDING_RADIUS(polyhedron, definition->boundingRadius);
        }
        if (context->setPolyhedronFlag != 0)
        {
            ((T3dPolyhedron_guess *)polyhedron)->flags |= T3D_POLYHEDRON_FLAG_SET_BY_CONTEXT;
        }
    }
    return polyhedron;
}

// FUNCTION: EQGFX 0x10024514
// Loads a sphere-list definition containing sphere centers and per-sphere data.
int __cdecl t3dCreateSphereListFromInterleavedFileContext_guess(T3dParserContext_guess *context)
{
    T3dSphereListDataFileDefinition_guess *definition;
    int *sourceCursor;
    unsigned char *coordinateCursor;
    unsigned char *coordinateEnd;
    int *indexCursor;
    void *coordinateBuffer;
    int *indexBuffer;
    int sphereCount;
    int pointerOffset;
    int result;
    float radius;

    definition = (T3dSphereListDataFileDefinition_guess *)context->polyhedronDefinition;
    sourceCursor = definition->recordData;
    if ((definition->flags & 1) != 0)
    {
        radius = *(float *)sourceCursor;
        sourceCursor++;
    }
    else
    {
        radius = 1.0f;
    }
    coordinateBuffer = EQ_CDECL(void *, 0x10050C48, (int))(definition->sphereCount * 0xc);
    if (coordinateBuffer == 0)
    {
        return 0;
    }
    indexBuffer = (int *)EQ_CDECL(void *, 0x10050C48, (int))(definition->sphereCount << 2);
    if (indexBuffer == 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(coordinateBuffer);
        return 0;
    }
    else
    {
        sphereCount = definition->sphereCount;
        coordinateCursor = (unsigned char *)coordinateBuffer;
        coordinateEnd = coordinateCursor + sphereCount * 0xc;
        indexCursor = indexBuffer;
        for (; coordinateCursor < coordinateEnd; coordinateCursor += 0xc)
        {
            EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
                coordinateCursor, sourceCursor, 0xc);
            *indexCursor = sourceCursor[3];
            sourceCursor += 4;
            indexCursor++;
        }
        if (definition->pointerOffset < 0)
        {
            pointerOffset = context->dictionarySize - definition->pointerOffset;
        }
        else
        {
            pointerOffset = 0;
        }
        result = EQ_CDECL(int, 0x1001AD5C,
            (void *, int, float, int, void *, int *, float))(
                context->dictionary, pointerOffset, definition->sphereValue,
                definition->sphereCount, coordinateBuffer, indexBuffer, radius);
        EQ_CDECL(void, 0x10050CBB, (void *))(indexBuffer);
        EQ_CDECL(void, 0x10050CBB, (void *))(coordinateBuffer);
        if (result == 0)
        {
            return 0;
        }
        if (context->setPolyhedronFlag == 0)
        {
            return result;
        }
        ((T3dSphereList_guess *)result)->flags |= T3D_SPHERE_LIST_FLAG_SET_BY_CONTEXT;
        ((T3dSphereList_guess *)result)->sphereData->flags |= T3D_SPHERE_LIST_FLAG_SET_BY_CONTEXT;
        return result;
    }
}

// FUNCTION: EQGFX 0x1002461f
// Creates a sphere-list instance from a referenced definition, with an optional tag and bounding radius.
void *__cdecl createSphereListFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dSphereListFileDefinition_guess *definition;
    int pointerOffset;
    void *sphereDefinition;
    void *sphereList;
    int tagOffset;
    int tag;

    definition = (T3dSphereListFileDefinition_guess *)context->polyhedronDefinition;
    pointerOffset = definition->pointerOffset;
    if (pointerOffset < 0)
    {
        sphereDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
            context->dictionary, context->dictionarySize - pointerOffset);
    }
    else
    {
        sphereDefinition = context->pointerTable[pointerOffset];
    }
    sphereList = EQGFX_T3D_CREATE_SPHERE_LIST(context->dictionary, sphereDefinition);
    if (sphereList != 0)
    {
        tagOffset = definition->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
            {
                tag = context->dictionarySize - tagOffset;
            }
            else
            {
                tag = 0;
            }
            if (EQGFX_T3D_SET_OBJECT_TAG(context->dictionary, sphereList, tag) == 0)
            {
                EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context->dictionary, sphereList);
            }
        }
        if ((definition->flags & 1) != 0)
        {
            EQGFX_T3D_SET_POLYHEDRON_DEFINITION_BOUNDING_RADIUS(sphereList, definition->boundingRadius);
        }
        if (context->setPolyhedronFlag != 0)
        {
            ((T3dSphereList_guess *)sphereList)->flags |= T3D_SPHERE_LIST_FLAG_SET_BY_CONTEXT;
        }
    }
    return sphereList;
}

// FUNCTION: EQGFX 0x100246b4
// Loads a light definition with optional color and intensity data.
int __cdecl createLightFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dLightDefinition_guess *definition;
    int *valueData;
    unsigned int flags;
    int pointerOffset;
    int valueA;
    int valueB;
    int light;

    definition = (T3dLightDefinition_guess *)context->polyhedronDefinition;
    flags = definition->flags;
    valueData = definition->values;
    if ((flags & T3D_LIGHT_DEFINITION_FLAG_VALUE_A) != 0)
    {
        valueA = *valueData;
        valueData++;
    }
    else
    {
        valueA = 0;
    }
    if ((flags & T3D_LIGHT_DEFINITION_FLAG_VALUE_B) != 0)
    {
        valueB = *valueData;
        valueData++;
    }
    else
    {
        valueB = 1;
    }
    if (definition->pointerOffset < 0)
    {
        pointerOffset = context->dictionarySize - definition->pointerOffset;
    }
    else
    {
        pointerOffset = 0;
    }
    light = EQ_CDECL(int, 0x10013D6D, (void *, int, int, int *, int, int, int))(
        context->dictionary,
        pointerOffset,
        definition->colorIntensityCount,
        valueData,
        valueA,
        valueB,
        (flags >> 3) & 1);
    if ((flags & T3D_LIGHT_DEFINITION_FLAG_COLOR_INTENSITY_LIST) != 0)
    {
        EQ_CDECL(void, 0x10014080, (int, int *))(
            light, valueData + definition->colorIntensityCount);
    }
    if ((light != 0) && (context->setPolyhedronFlag != 0))
    {
        ((T3dLight_guess *)light)->flags |= T3D_LIGHT_FLAG_SET_BY_CONTEXT;
        ((T3dLight_guess *)light)->colorIntensityList->flags |= T3D_LIGHT_FLAG_SET_BY_CONTEXT;
    }
    return light;
}

// FUNCTION: EQGFX 0x10024755
// Creates a light instance from a referenced definition and applies its optional tag.
int __cdecl createTaggedLightFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dLightFileDefinition_guess *definition;
    void *lightDefinition;
    int pointerOffset;
    int tagOffset;
    int light;

    definition = (T3dLightFileDefinition_guess *)context->polyhedronDefinition;
    pointerOffset = definition->pointerOffset;
    if (pointerOffset < 0)
    {
        lightDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
            context->dictionary, context->dictionarySize - pointerOffset);
    }
    else
    {
        lightDefinition = context->pointerTable[pointerOffset];
    }
    light = (int)EQGFX_T3D_CREATE_LIGHT(context->dictionary, lightDefinition);
    if (light != 0)
    {
        tagOffset = definition->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
            {
                tagOffset = context->dictionarySize - tagOffset;
            }
            else
            {
                tagOffset = 0;
            }
            if (EQGFX_T3D_SET_OBJECT_TAG(context->dictionary, (void *)light, tagOffset) == 0)
            {
                EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context->dictionary, (void *)light);
            }
        }
        if (context->setPolyhedronFlag != 0)
        {
            ((T3dLight_guess *)light)->flags |= T3D_LIGHT_FLAG_SET_BY_CONTEXT;
        }
    }
    return light;
}

// FUNCTION: EQGFX 0x100247D5
// Loads a point light with region assignments and static or dynamic influence options.
int __cdecl createPointLightFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dPointLightRegionFileDefinition_guess *definition;
    void *lightDefinition;
    int pointerOffset;
    int tagOffset;
    int pointLight;
    int *regionCountAndIds;
    unsigned int regionCount;
    unsigned int enabled;

    definition = (T3dPointLightRegionFileDefinition_guess *)context->polyhedronDefinition;
    pointerOffset = definition->pointerOffset;
    regionCountAndIds = (int *)&definition->regionCount;
    if ((definition->flags & T3D_POINT_LIGHT_FILE_FLAG_REGIONS) != 0)
    {
        if (pointerOffset < 0)
        {
            lightDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
                context->dictionary, context->dictionarySize - pointerOffset);
        }
        else
        {
            lightDefinition = context->pointerTable[pointerOffset];
        }
        enabled = 0;
    }
    else
    {
        if (pointerOffset < 0)
        {
            lightDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
                context->dictionary, context->dictionarySize - pointerOffset);
        }
        else
        {
            lightDefinition = context->pointerTable[pointerOffset];
        }
        enabled = 1;
    }
    pointLight = (int)EQGFX_T3D_CREATE_POINT_LIGHT_EX(
        context->dictionary,
        lightDefinition,
        &definition->lightDefinitionPointerOffset,
        definition->pointLightValue,
        enabled);
    if (pointLight != 0)
    {
        if ((definition->flags & T3D_POINT_LIGHT_FILE_FLAG_STATIC) != 0)
        {
            EQGFX_T3D_SET_DIRECTIONAL_LIGHT_STATIC_FLAG((void *)pointLight);
        }
        if ((definition->flags & T3D_POINT_LIGHT_FILE_FLAG_STATIC_INFLUENCE) != 0)
        {
            EQGFX_T3D_SET_POINT_LIGHT_STATIC_INFLUENCE_FLAG((void *)pointLight);
        }
        if ((definition->flags & T3D_POINT_LIGHT_FILE_FLAG_REGIONS) != 0)
        {
            for (regionCount = *regionCountAndIds; regionCount != 0; regionCount--)
            {
                regionCountAndIds++;
                EQGFX_T3D_ADD_POINT_LIGHT_TO_REGION(
                    context->dictionary, (void *)pointLight, *regionCountAndIds);
            }
        }
        tagOffset = definition->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
            {
                tagOffset = context->dictionarySize - tagOffset;
            }
            else
            {
                tagOffset = 0;
            }
            if (EQGFX_T3D_SET_OBJECT_TAG(context->dictionary, (void *)pointLight, tagOffset) == 0)
            {
                EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context->dictionary, (void *)pointLight);
            }
        }
        if ((definition->dynamicSceneInfluence &
            T3D_POINT_LIGHT_FILE_FLAG_DYNAMIC_SCENE_INFLUENCE) != 0)
        {
            EQGFX_T3D_SET_DIRECTIONAL_LIGHT_DYNAMIC_SCENE_INFLUENCE_FLAG((void *)pointLight);
        }
        if (context->setPolyhedronFlag != 0)
        {
            ((T3dLight_guess *)pointLight)->flags |= T3D_LIGHT_FLAG_SET_BY_CONTEXT;
        }
    }
    return pointLight;
}

// FUNCTION: EQGFX 0x100248F6
// Loads an immediately bound point light with an optional tag.
int __cdecl createPointLightWithTagFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dPointLightFileDefinition_guess *definition;
    void *lightDefinition;
    int pointerOffset;
    int tagOffset;
    int pointLight;

    definition = (T3dPointLightFileDefinition_guess *)context->polyhedronDefinition;
    pointerOffset = definition->pointerOffset;
    if (pointerOffset < 0)
    {
        lightDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
            context->dictionary, context->dictionarySize - pointerOffset);
    }
    else
    {
        lightDefinition = context->pointerTable[pointerOffset];
    }
    pointLight = (int)EQGFX_T3D_CREATE_POINT_LIGHT_EX(
        context->dictionary,
        lightDefinition,
        &definition->lightDefinitionPointerOffset,
        definition->pointLightValue,
        1);
    if (pointLight != 0)
    {
        tagOffset = definition->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
            {
                tagOffset = context->dictionarySize - tagOffset;
            }
            else
            {
                tagOffset = 0;
            }
            if (EQGFX_T3D_SET_OBJECT_TAG(context->dictionary, (void *)pointLight, tagOffset) == 0)
            {
                EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context->dictionary, (void *)pointLight);
            }
        }
        if (context->setPolyhedronFlag != 0)
        {
            ((T3dLight_guess *)pointLight)->flags |= T3D_LIGHT_FLAG_SET_BY_CONTEXT;
        }
    }
    return pointLight;
}

// FUNCTION: EQGFX 0x10024984
// Loads a directional light with region assignments and an optional static-light setting.
int __cdecl createDirectionalLightFromFileContext_guess(T3dParserContext_guess *context)
{
    T3dDirectionalLightFlaggedFileDefinition_guess *definition;
    void *lightDefinition;
    int pointerOffset;
    int tagOffset;
    int directionalLight;
    unsigned int regionIndex;
    int *regionIds;

    definition = (T3dDirectionalLightFlaggedFileDefinition_guess *)context->polyhedronDefinition;
    regionIds = definition->regionIds;
    pointerOffset = definition->pointerOffset;
    if (pointerOffset < 0)
    {
        lightDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
            context->dictionary, context->dictionarySize - pointerOffset);
    }
    else
    {
        lightDefinition = context->pointerTable[pointerOffset];
    }
    directionalLight = (int)EQGFX_T3D_CREATE_DIRECTIONAL_LIGHT(
        context->dictionary, lightDefinition, &definition->lightDefinitionPointerOffset);
    if (directionalLight != 0)
    {
        if ((definition->flags & T3D_DIRECTIONAL_LIGHT_FILE_FLAG_STATIC) != 0)
        {
            EQGFX_T3D_SET_DIRECTIONAL_LIGHT_STATIC_FLAG((void *)directionalLight);
        }
        tagOffset = definition->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
            {
                tagOffset = context->dictionarySize - tagOffset;
            }
            else
            {
                tagOffset = 0;
            }
            if (EQGFX_T3D_SET_OBJECT_TAG(context->dictionary, (void *)directionalLight, tagOffset) == 0)
            {
                EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context->dictionary, (void *)directionalLight);
            }
        }
        regionIndex = 0;
        if ((unsigned int)definition->regionCount > regionIndex)
        {
            do
            {
                EQGFX_T3D_ADD_DIRECTIONAL_LIGHT_TO_REGION(
                    context->dictionary, (void *)directionalLight, *regionIds++);
                regionIndex++;
            } while (regionIndex < definition->regionCount);
        }
        if (context->setPolyhedronFlag != 0)
        {
            ((T3dLight_guess *)directionalLight)->flags |= T3D_LIGHT_FLAG_SET_BY_CONTEXT;
        }
    }
    return directionalLight;
}

// FUNCTION: EQGFX 0x10024A4C
// Loads a directional light and its region assignments from a record without a static-light option.
int __cdecl createDirectionalLightFromFileContext_10024A4C_guess(T3dParserContext_guess *context)
{
    T3dDirectionalLightFileDefinition_guess *definition;
    void *lightDefinition;
    int pointerOffset;
    int tagOffset;
    int directionalLight;
    unsigned int regionIndex;
    int *regionIds;

    definition = (T3dDirectionalLightFileDefinition_guess *)context->polyhedronDefinition;
    regionIds = definition->regionIds;
    pointerOffset = definition->pointerOffset;
    if (pointerOffset < 0)
    {
        lightDefinition = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(
            context->dictionary, context->dictionarySize - pointerOffset);
    }
    else
    {
        lightDefinition = context->pointerTable[pointerOffset];
    }
    directionalLight = (int)EQGFX_T3D_CREATE_DIRECTIONAL_LIGHT(
        context->dictionary, lightDefinition, &definition->lightDefinitionPointerOffset);
    if (directionalLight != 0)
    {
        tagOffset = definition->tagOffset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
            {
                tagOffset = context->dictionarySize - tagOffset;
            }
            else
            {
                tagOffset = 0;
            }
            if (EQGFX_T3D_SET_OBJECT_TAG(context->dictionary, (void *)directionalLight, tagOffset) == 0)
            {
                EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context->dictionary, (void *)directionalLight);
            }
        }
        regionIndex = 0;
        if ((unsigned int)definition->regionCount > regionIndex)
        {
            do
            {
                EQGFX_T3D_ADD_DIRECTIONAL_LIGHT_TO_REGION(
                    context->dictionary, (void *)directionalLight, *regionIds);
                regionIndex++;
                regionIds++;
            } while (regionIndex < definition->regionCount);
        }
        if (context->setPolyhedronFlag != 0)
        {
            ((T3dLight_guess *)directionalLight)->flags |= T3D_LIGHT_FLAG_SET_BY_CONTEXT;
        }
    }
    return directionalLight;
}
