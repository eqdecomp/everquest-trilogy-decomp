// unit_10024B08.c - eqgfx_dx7.dll, the address run 0x10024B08..0x10026565 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <string.h>

typedef struct MaterialDefinitionRecord_guess
{
    int definitionOffset;
    unsigned int flags;
    int materialValue;
    unsigned int surfaceFlags;
    float constantIntensity;
    float ambientScalar;
    int simpleSpriteOffset;
    float uvShiftPerMs[2];
} MaterialDefinitionRecord_guess;

typedef struct MaterialDefinitionFileContext_guess
{
    unsigned int reserved00;
    void *dictionary;
    unsigned char reserved08[0x30];
    int fileSize;
    unsigned char reserved3c[0x8008];
    MaterialDefinitionRecord_guess *record;
    unsigned int reserved8048;
    void **dataByOffset;
    unsigned char reserved8050[0x14];
    int setMaterialFlag;
} MaterialDefinitionFileContext_guess;

typedef struct MaterialDefinition_guess
{
    unsigned char reserved00[0x0c];
    unsigned int flags;
    unsigned char reserved10[4];
    int materialValue;
    unsigned char reserved18[0x14];
    unsigned char hasSurfaceFlags;
} MaterialDefinition_guess;

typedef struct ParticleCloudRecord_guess
{
    int objectTag;
    unsigned int flags;
    int parameter08;
    int parameter0c;
    int parameter10;
    int parameter14;
    float parameter18;
    int vector1c[3];
    int parameter28;
    float parameter2c;
    float parameter30;
    int parameter34;
    float parameter38;
    unsigned char vector3c[12];
    int parameter48;
    float parameter4c;
    void *parameter50;
    unsigned char payload54[1];
} ParticleCloudRecord_guess;

typedef struct ParticleCloudContext_guess
{
    unsigned char reserved00[4];
    void *dictionary;
    unsigned char reserved08[0x30];
    int objectBase;
    unsigned char reserved3c[0x8008];
    ParticleCloudRecord_guess *particleRecord;
    unsigned int reserved8048;
    void **objectTable;
    unsigned char reserved8050[0x14];
    int useAmbientFlag;
} ParticleCloudContext_guess;

typedef struct ParticleCloudObject_guess
{
    unsigned char reserved00[0x0c];
    unsigned int flags;
} ParticleCloudObject_guess;

typedef struct MaterialPaletteRecord_guess
{
    int tagOffset;
    unsigned int reserved04;
    unsigned int entryCount;
    int dataOffsets[1];
} MaterialPaletteRecord_guess;

typedef struct MaterialPaletteLoadContext_guess
{
    unsigned int reserved00;
    void *handle;
    unsigned char reserved08[0x30];
    int fileSize;
    unsigned char reserved3c[0x8008];
    MaterialPaletteRecord_guess *record;
    unsigned int reserved8048;
    void **dataByOffset;
    unsigned char reserved8050[0x14];
    int setPaletteFlag;
} MaterialPaletteLoadContext_guess;

typedef struct MaterialPalette_guess
{
    int type;
    char *tagAlt;
    char *tag;
    unsigned int flags;
} MaterialPalette_guess;

typedef struct ResourceOwner_guess
{
    unsigned char reserved00[0x10];
    int referenceCount;
} ResourceOwner_guess;

typedef struct ResourcePayload_guess
{
    unsigned int elementCount;
    void *data;
} ResourcePayload_guess;

typedef struct ResourceSubBlock_guess
{
    unsigned int typeCode;
    unsigned int reserved04;
    unsigned int capacity;
    void *allocation;
    unsigned int reserved10;
    unsigned int entryCount;
    void *entryAllocation;
    unsigned int reserved1c;
    unsigned int totalCount;
    unsigned int reserved24;
    unsigned int referenceSlots[3];
    unsigned int reserved34;
    float initialValue;
    unsigned int flags;
} ResourceSubBlock_guess;

typedef struct ResourceTailBlock_guess
{
    unsigned int typeCode;
} ResourceTailBlock_guess;

typedef struct ResourceStorage_guess
{
    unsigned int typeCode;
    ResourceOwner_guess *owner;
    ResourcePayload_guess *payload;
    unsigned char reserved0c[0x0c];
    int kind;
    int optionalValue;
    ResourceSubBlock_guess mainBlock;
    ResourceTailBlock_guess tailBlock;
    unsigned char trailingData[1];
} ResourceStorage_guess;

typedef struct ResourceDescriptor_guess
{
    unsigned int typeCode;
    unsigned int reserved04;
    char *label;
    unsigned int flags;
    unsigned int reserved10;
    int values[3];
    unsigned int extraValue;
    ResourceStorage_guess *storage;
    ResourceTailBlock_guess *tailBlock;
    ResourceSubBlock_guess *mainBlock;
    unsigned int reserved30[2];
} ResourceDescriptor_guess;

typedef struct ResourceRecord_guess
{
    int labelRef;
    unsigned int flags;
    int ownerRef;
    unsigned char reserved0c[0x18];
    int entryCount;
    unsigned char reserved28[4];
    unsigned short entries[1];
} ResourceRecord_guess;

typedef struct ResourceLoadContext_guess
{
    unsigned char reserved00[4];
    void *dictionary;
    unsigned char reserved08[0x30];
    int objectBase;
    unsigned char reserved3c[0x8044 - 0x3c];
    ResourceRecord_guess *record;
    unsigned int reserved8048;
    void **objectTable;
    unsigned int itemCount;
    unsigned int nextItem;
    ResourceDescriptor_guess *descriptors;
    unsigned char reserved805c[0x8064 - 0x805c];
    int setDescriptorFlag;
} ResourceLoadContext_guess;

typedef struct RegionAmbientEntry_guess
{
    int objectTag;
    int objectIndex;
    unsigned int reserved08;
    unsigned int regionCount;
    int regionNumbers[1];
} RegionAmbientEntry_guess;

typedef struct RegionAmbientObject_guess
{
    unsigned char reserved00[0x0c];
    unsigned int flags;
} RegionAmbientObject_guess;

typedef struct RegionAmbientContext_guess
{
    unsigned int reserved00;
    void *dictionary;
    unsigned char reserved08[0x30];
    int objectBase;
    unsigned char reserved3c[0x8008];
    RegionAmbientEntry_guess *ambientEntry;
    unsigned int reserved8048;
    int *objectTable;
    unsigned char reserved8050[0x14];
    int useAmbientFlag;
} RegionAmbientContext_guess;

typedef struct ZoneFileEntry_guess
{
    int objectTag;
    int reserved04;
    unsigned int regionCount;
    int regionNumbers[1];
} ZoneFileEntry_guess;

typedef struct OpenFileObject_guess
{
    int fileType;
    int archiveId;
    void *handle;
    char *path;
    int fileSize;
    unsigned int savedPosition;
} OpenFileObject_guess;

typedef struct VISLoadContext_guess
{
    const char *fileName;
    int reserved04;
    int archiveId;
    char reserved0C[0x8054];
    void *loadedData;
} VISLoadContext_guess;

typedef struct VISFileData_guess
{
    unsigned int reserved00;
    unsigned int reserved04;
    unsigned int reserved08;
    void *data;
} VISFileData_guess;

typedef void *(__stdcall *CreateFileA_guess_t)(const char *, unsigned int, unsigned int, void *, unsigned int, unsigned int, void *);

#define t3dFastFileOpen_10021791 EQ_CDECL(int, 0x10021791, (int, const char *))
#define t3dFastFileGetEOF_100219A3 EQ_CDECL(unsigned int, 0x100219A3, (void *))
#define strncpy_100688E0 EQ_CDECL(char *, 0x100688E0, (char *, const char *, unsigned int))
#define strrchr_10069200 EQ_CDECL(char *, 0x10069200, (const char *, int))
#define s_VISExtension_10087188 EQ_ARRAY(char, 0x10087188)
#define pfn_CreateFileA_guess EQ_GLOBAL(CreateFileA_guess_t, 0x10076098)
#define FILE_GENERIC_READ 0x80000000
#define FILE_SHARE_READ 1
#define FILE_OPEN_EXISTING 3
#define FILE_ATTRIBUTE_NORMAL 0x80
#define INVALID_FILE_HANDLE ((void *)-1)

typedef struct FreeObjectData_guess
{
    char reserved00[0x04];
    void *handle;
    char reserved08[0x04];
    void *data;
} FreeObjectData_guess;

typedef struct FilePositionSource_guess
{
    int sourceType;
    int reserved04;
    void *file;
    char *path;
    int fileSize;
    unsigned int savedPosition;
} FilePositionSource_guess;

typedef struct WorldAmbientSource_guess
{
    char reserved00[4];
    unsigned int ambientValueAt04;
    char reserved08[0x803c];
    unsigned int ambientValueAt8044;
} WorldAmbientSource_guess;

typedef struct BspSourceNode_guess
{
    unsigned int value00;
    unsigned int value04;
    unsigned int value08;
    unsigned int value0c;
    unsigned int value10;
    int leftNodeIndex;
    int rightNodeIndex;
} BspSourceNode_guess;

typedef struct BspSourceTree_guess
{
    int reserved00;
    int nodeCount;
    BspSourceNode_guess nodes[1];
} BspSourceTree_guess;

typedef struct BspTreeStorage_guess
{
    char reserved00[0x0c];
    struct BspNode_guess *nodes;
    int alignmentOffset;
} BspTreeStorage_guess;

typedef struct BspNode_guess
{
    unsigned int value00;
    unsigned int value04;
    unsigned int value08;
    unsigned int value0c;
    struct BspNode_guess *leftChild;
    struct BspNode_guess *rightChild;
    unsigned int value18;
    unsigned int reserved1c;
} BspNode_guess;

typedef struct BspBuildContext_guess
{
    char reserved00[0x04];
    BspTreeStorage_guess *treeStorage;
    char reserved08[0x803c];
    BspSourceTree_guess *sourceTree;
} BspBuildContext_guess;

typedef struct ParserObject_guess
{
    char reserved00[0x08];
    int errorCount;
    char reserved0c[0x20];
    int worldLoadMode;
    int worldContext;
    int optionalWorldData;
    int optionalWorldMetadata;
    char reserved3c[0x04];
} ParserObject_guess;

typedef struct DMTrack2Record_guess
{
    int offset;
    int dataOffset;
    short field08;
    short field0a;
    unsigned short field0c;
    short field0e;
    short field10;
    unsigned char data[1];
} DMTrack2Record_guess;

typedef struct DMTrackArrayRecord_guess
{
    int offset;
    int reserved04;
    int entryStride;
    unsigned int entryCount;
    int field10;
    int field14;
    unsigned char entryData[1];
} DMTrackArrayRecord_guess;

typedef struct DMTrackFileContext_guess
{
    char reserved00[0x04];
    void *handle;
    char reserved08[0x30];
    int fileSize;
    char reserved3c[0x8044 - 0x3c];
    DMTrack2Record_guess *record;
    char reserved8048[0x04];
    void **dataByOffset;
    char reserved8050[0x8064 - 0x8050];
    int setTrackFlag;
} DMTrackFileContext_guess;

typedef struct DMTrack2_guess
{
    char reserved00[0x0c];
    unsigned int flags;
} DMTrack2_guess;

typedef struct DMTrackDef_guess
{
    char reserved00[0x14];
    int vertexCount;
} DMTrackDef_guess;

typedef struct DMSpriteMaterialData_guess
{
    int entryCount;
    char reserved04[0x4c];
    int materialId;
    char reserved54[0x04];
    int textureId;
} DMSpriteMaterialData_guess;

typedef struct DetailedMeshSprite_guess
{
    char reserved00[0x08];
    const char *name;
    unsigned int flags;
    char reserved10[0x04];
    DMSpriteMaterialData_guess *materialData;
} DetailedMeshSprite_guess;

typedef struct DMSpriteDefinitionRecord_guess
{
    int offset;
    unsigned int flags;
    int vertexCount;
    int polygonCount;
    int normalCount;
    int textureCoordinateCount;
    int textureCount;
    int paletteCount;
    int materialCount;
    int volumeDataOffset;
    int auxiliaryDataOffset;
    int reserved2c;
    int volumeDefinitionOffset;
    float baseVector[3];
    float scale;
    int data[1];
} DMSpriteDefinitionRecord_guess;

typedef struct DMSpriteFileContext_guess
{
    int reserved00;
    void *dictionary;
    unsigned char reserved08[0x2f];
    unsigned char fileFlags;
    int objectBase;
    char reserved3c[0x8044 - 0x3c];
    DMSpriteDefinitionRecord_guess *record;
    int reserved8048;
    void **dataByOffset;
    char reserved8050[0x14];
    int setSpriteFlag;
} DMSpriteFileContext_guess;

typedef struct DMSprite2DefinitionRecord_guess
{
    int offset;
    unsigned int flags;
    int volumeDataOffset;
    int auxiliaryDataOffset;
    int volumeDefinitionDataOffset;
    int volumeDefinitionOffset;
    float baseVector[3];
    float lightVector[3];
    float scale;
    float optionalVector[3];
    unsigned char reserved40[0x0c];
    unsigned short vertexCount;
    unsigned short polygonCount;
    unsigned short normalCount;
    unsigned short indexCount;
    unsigned short textureCoordinateCount;
    unsigned short textureCount;
    unsigned short paletteCount;
    unsigned short materialCount;
    unsigned short auxiliaryCount;
    unsigned short trailingCount;
    unsigned char data[1];
} DMSprite2DefinitionRecord_guess;

#define EQGFX_T3D_FREE_MEMORY_GUESS EQ_CDECL(void, 0x10050CBB, (void *))
#define t3dFastFileTell_1002191D EQ_CDECL(unsigned int, 0x1002191D, (void *))
#define ftell_100693F7 EQ_CDECL(long, 0x100693F7, (void *))
#define s3dSetWorldConstantAmbient_100202E2 EQ_CDECL(void, 0x100202E2, (unsigned int, unsigned int))
#define t3dFastFileClose_10021838 EQ_CDECL(void, 0x10021838, (void *))
#define fclose_10068328 EQ_CDECL(int, 0x10068328, (void *))
#define GFX_COMPOSITE_ALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C48, (unsigned int))
#define EQGFX_MEMCPY_GUESS EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
#define getFilePosition_10026525 EQ_CDECL(unsigned int, 0x10026525, (FilePositionSource_guess *))
#define strlen_EQGFX EQ_CDECL(unsigned int, 0x10068160, (const char *))
#define GFX_PATH_COPY_STRING EQ_CDECL(void, 0x10068070, (char *, const char *))
#define fread_10069227 EQ_CDECL(size_t, 0x10069227, (void *, size_t, size_t, void *))
#define t3dFastFileRead_1002186A EQ_CDECL(void, 0x1002186A, (void *, void *, unsigned int))
#define t3dFastFileSeek_10021897 EQ_CDECL(unsigned int, 0x10021897, (void *, int, int))
#define fseek_1006933E EQ_CDECL(int, 0x1006933E, (void *, long, int))

typedef unsigned int (__stdcall *SetFilePointer_guess_t)(void *file, long distanceToMove, long *distanceToMoveHigh, unsigned int moveMethod);
#define pfn_SetFilePointer_guess EQ_GLOBAL(SetFilePointer_guess_t, 0x100760A0)

// Import table entry of this image (call dword ptr [va], filled by the loader).
typedef int (__stdcall *MessageBoxA_guess_t)(void *hwnd, const char *text, const char *caption, unsigned int type);

#define pfn_MessageBoxA_guess EQ_GLOBAL(MessageBoxA_guess_t, 0x100761F8)
#define s_WorldFileParser_guess EQ_ARRAY(char, 0x100871EC) // s_World_File_Parser_100871ec: "..."
#define WORLD_PARSER_MSGBOX_FLAGS 0x12010
#define g_numObsoleteObjects_guess EQ_GLOBAL(int, 0x100AADE8) // EQGFX_TRACK_DEF2_ALLOCATION_ATTEMPTS: pending obsolete-object count

typedef int (__stdcall *CloseHandle_guess_t)(void *handle);
#define pfn_CloseHandle_guess EQ_GLOBAL(CloseHandle_guess_t, 0x100760AC)
typedef int (__stdcall *ReadFile_guess_t)(void *file, void *buffer, unsigned int count, unsigned int *bytesRead, void *overlapped);
#define pfn_ReadFile_100760A8 EQ_GLOBAL(ReadFile_guess_t, 0x100760A8)

#define t3dGetPointerFromDictionary_1000E7FE EQ_CDECL(int, 0x1000E7FE, (void *, int))
#define t3dSetObjectTag_100174B8 EQ_CDECL(int, 0x100174B8, (void *, int, int))
#define t3dAddEntryToDictionary_1000DC08 EQ_CDECL(void, 0x1000DC08, (void *, int))
#define t3dGetRegionFromRegionNumber_10019346 EQ_CDECL(int, 0x10019346, (void *, int, int))
#define t3dSetRegionAmbientLight_100194A2 EQ_CDECL(void, 0x100194A2, (int))

// FUNCTION: EQGFX 0x10024b08
// Applies a loaded ambient light to the regions listed in its world record.
int __cdecl setRegionAmbientLights_guess(RegionAmbientContext_guess *context)
{
    RegionAmbientEntry_guess *ambientEntry;
    int ambientObject;
    int objectTag;
    int region;
    unsigned int regionIndex;
    int *regionNumber;

    regionIndex = 0;
    ambientEntry = context->ambientEntry;
    ambientObject = ambientEntry->objectIndex;
    if (ambientObject < 0)
    {
        ambientObject = t3dGetPointerFromDictionary_1000E7FE(context->dictionary, context->objectBase - ambientObject);
    }
    else
    {
        ambientObject = context->objectTable[ambientObject];
    }
    if (ambientObject != 0)
    {
        objectTag = ambientEntry->objectTag;
        if (objectTag != 0)
        {
            if (objectTag < 0)
                objectTag = context->objectBase - objectTag;
            else
                objectTag = 0;
            if (t3dSetObjectTag_100174B8(context->dictionary, ambientObject, objectTag) == 0)
                t3dAddEntryToDictionary_1000DC08(context->dictionary, ambientObject);
        }
        regionNumber = ambientEntry->regionNumbers;
        if (ambientEntry->regionCount != 0)
        {
            do
            {
                region = t3dGetRegionFromRegionNumber_10019346(context->dictionary, *regionNumber, ambientObject);
                t3dSetRegionAmbientLight_100194A2(region);
                regionIndex = regionIndex + 1;
                regionNumber = regionNumber + 1;
            } while (regionIndex < ambientEntry->regionCount);
        }
        if (context->useAmbientFlag != 0)
            ((RegionAmbientObject_guess *)ambientObject)->flags = ((RegionAmbientObject_guess *)ambientObject)->flags | 0x10;
    }
    return ambientObject;
}

// FUNCTION: EQGFX 0x10024bbd
// Loads a binary tree from stored node records; its rendering purpose is unclear.
BspNode_guess * __cdecl createBSPTreeFromFileContext_guess(BspBuildContext_guess *context)
{
    BspSourceTree_guess *sourceTree;
    BspTreeStorage_guess *treeStorage;
    BspSourceNode_guess *sourceNode;
    BspNode_guess *allocatedNodes;
    BspNode_guess *outputNodes;
    BspNode_guess *outputNode;
    BspNode_guess *outputEnd;
    int nodeCount;

    sourceTree = context->sourceTree;
    allocatedNodes = (BspNode_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY((sourceTree->nodeCount + 1) * 0x20);
    if (allocatedNodes != 0)
    {
        treeStorage = context->treeStorage;
        if (treeStorage->nodes != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS((char *)treeStorage->nodes - treeStorage->alignmentOffset);
        }
        treeStorage->alignmentOffset = 0x20 - ((unsigned int)allocatedNodes & 0x1f);
        outputNodes = (BspNode_guess *)((char *)allocatedNodes + treeStorage->alignmentOffset);
        allocatedNodes = outputNodes;
        treeStorage->nodes = outputNodes;
        nodeCount = sourceTree->nodeCount;
        outputEnd = outputNodes + nodeCount;
        if (outputNodes < outputEnd)
        {
            outputNode = outputNodes;
            sourceNode = sourceTree->nodes;
            do
            {
                EQGFX_MEMCPY_GUESS(outputNode, sourceNode, 0x0c);
                outputNode->value0c = sourceNode->value0c;
                outputNode->value18 = sourceNode->value10;
                if (sourceNode->leftNodeIndex == 0)
                    outputNode->leftChild = 0;
                else
                    outputNode->leftChild = outputNodes + sourceNode->leftNodeIndex - 1;
                if (sourceNode->rightNodeIndex == 0)
                    outputNode->rightChild = 0;
                else
                    outputNode->rightChild = outputNodes + sourceNode->rightNodeIndex - 1;
                sourceNode++;
                outputNode++;
            } while (outputNode < outputEnd);
        }
    }
    return allocatedNodes;
}

// FUNCTION: EQGFX 0x10024C93
// Loads a rendering resource's data and references from a world record; its specific purpose is unclear.
// Fills the next descriptor from the current record; returns it, or 0. The parameter slot is reused for the optional value.
#pragma optimize("y", off)
ResourceDescriptor_guess * __cdecl t3dCreateResourceDescriptorFromFileContext_guess(int tmp)
{
    ResourceLoadContext_guess *context;
    ResourceRecord_guess *record;
    ResourceDescriptor_guess *descriptor;
    ResourceStorage_guess *storage;
    char *label;
    int kind;
    unsigned int elementCount;
    int bufferSize;
    unsigned short *trailer;
    unsigned short *sourceData;
    ResourceOwner_guess *owner;
    int *values;
    unsigned int extraValue;
    int *cursor;
    unsigned int flags;
    int payloadSize;
    int refId;

    context = (ResourceLoadContext_guess *)tmp;
    tmp = 0;
    record = context->record;
    label = 0;
    owner = 0;
    kind = 0;
    if (context->nextItem < context->itemCount)
    {
        descriptor = context->descriptors + context->nextItem;
        if (record->labelRef != 0)
        {
            if (record->labelRef < 0)
                label = (char *)(context->objectBase - record->labelRef);
            else
                label = 0;
        }
        if (record->ownerRef != 0)
        {
            if (record->ownerRef < 0)
                owner = (ResourceOwner_guess *)t3dGetPointerFromDictionary_1000E7FE(context->dictionary, context->objectBase - record->ownerRef);
            else
                owner = context->objectTable[record->ownerRef];
        }
        trailer = record->entries;
        if (record->entryCount != 0)
            trailer += record->entryCount * 0xe;
        flags = record->flags;
        if ((flags & 0x20) == 0)
        {
            if ((flags & 0x80) != 0)
                kind = 2;
        }
        else
            kind = 1;
        elementCount = *trailer;
        sourceData = trailer + 1;
        if (kind != 2)
            cursor = (int *)((char *)sourceData + elementCount * 2);
        else
            cursor = (int *)((char *)sourceData + elementCount);
        if ((flags & 1) != 0)
        {
            values = cursor;
            extraValue = (unsigned int)cursor[3];
            cursor += 3;
            cursor += 1;
        }
        else
            values = 0;
        if ((flags & 2) != 0)
            cursor += 1;
        if ((flags & 4) != 0)
            cursor += 1;
        payloadSize = *cursor;
        cursor += 1;
        if (payloadSize != 0)
            cursor = (int *)((char *)cursor + payloadSize);
        if ((flags & 0x40) != 0)
        {
            refId = *cursor;
            if (refId < 0)
                tmp = t3dGetPointerFromDictionary_1000E7FE(context->dictionary, context->objectBase - refId);
            else
                tmp = (int)context->objectTable[refId];
            cursor += 1;
        }
        if ((((unsigned char *)record)[5] & 1) != 0)
        {
            refId = *cursor;
            if (refId < 0)
                tmp = t3dGetPointerFromDictionary_1000E7FE(context->dictionary, context->objectBase - refId);
            else
                tmp = (int)context->objectTable[refId];
        }
        bufferSize = EQ_CDECL(int, 0x10024E56, (const char *, int, int))(label, kind, elementCount);
        storage = (ResourceStorage_guess *)EQ_CDECL(void *, 0x10019A5C, (void *, unsigned int, int))(context->dictionary, bufferSize, context->setDescriptorFlag);
        if (storage == 0)
            return descriptor;
        if (EQ_CDECL(int, 0x10024E99, (void *, ResourceDescriptor_guess *, ResourceStorage_guess *, char *, int, size_t, const void *, ResourceOwner_guess *, int *, float, int))(context->dictionary, descriptor, storage, label, kind, elementCount, sourceData, owner, values, *(float *)&extraValue, tmp) == 0)
        {
            if (context->setDescriptorFlag != 0)
                descriptor->flags |= 0x10;
            context->nextItem++;
            return descriptor;
        }
    }
    return 0;
}
#pragma optimize("y", on)

// FUNCTION: EQGFX 0x10024E56
// Calculates storage space for a labeled resource payload.
// __cdecl. Returns the buffer size for kind 0/1 (value * 2 + 108) or kind 2
// (value + 108), plus the label and its terminator when label is not NULL.
#pragma optimize("y", off)
int __cdecl getResourceStorageSize_guess(const char *label, int kind, int elementCount)
{
    int size;

    size = 100;
    if (kind == 0)
        size = elementCount * 2 + 100;
    else if (kind == 1)
        size = elementCount * 2 + 100;
    else if (kind == 2)
        size = elementCount + 100;
    size += 8;
    if (label != 0)
        size += strlen(label) + 1;
    return size;
}
#pragma optimize("y", on)

#define resourceStorageSize_10024E56 EQ_CDECL(int, 0x10024E56, (const char *, int, int))
#define t3dInitReferenceList_parse EQ_CDECL(int, 0x10019012, (void *, int))
#define GFX_COMPOSITE_INITIALIZE_OBJECT EQ_CDECL(int, 0x1001564B, (void *, void *))
#define parseAddToDictionary EQ_CDECL(int, 0x1000DC08, (void *, void *))
#define GFX_COMPOSITE_RELEASE_OBJECT EQ_CDECL(int, 0x10015D81, (void *, void *))
#define parseMemset EQ_CDECL(void *, 0x100682D0, (void *, int, size_t))
#define RESOURCE_CREATE_ERROR 0xFFFF3D3E

// FUNCTION: EQGFX 0x10024e99
// Creates and registers a named rendering resource with copied data; its specific purpose is unclear.
int __cdecl t3dCreateResourceDescriptor_guess(void *dictionary,
                         ResourceDescriptor_guess *descriptor,
                         ResourceStorage_guess *providedStorage,
                         char *label,
                         int kind,
                         size_t elementCount,
                         const void *sourceData,
                         ResourceOwner_guess *owner,
                         int *values,
                         unsigned int extraValue,
                         int value)
{
    ResourceStorage_guess *storage;
    ResourceSubBlock_guess *mainBlock;
    ResourceTailBlock_guess *tailBlock;
    ResourcePayload_guess *payload;
    size_t copySize;
    int result;

    storage = providedStorage;
    mainBlock = 0;
    parseMemset(descriptor, 0, 0x38);
    descriptor->typeCode = 0x19;
    if (storage == 0)
    {
        storage = (ResourceStorage_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(
            resourceStorageSize_10024E56(label, kind, (int)elementCount));
        if (storage == 0)
            goto cleanup;
    }

    mainBlock = &storage->mainBlock;
    tailBlock = &storage->tailBlock;
    payload = (ResourcePayload_guess *)storage->trailingData;
    if (providedStorage != 0)
        descriptor->flags |= 0x20;
    if (label == 0)
    {
        descriptor->label = 0;
    }
    else
    {
        descriptor->label = (char *)payload;
        payload = (ResourcePayload_guess *)((char *)payload + strlen_EQGFX(label) + 1);
        GFX_PATH_COPY_STRING(descriptor->label, label);
    }
    parseMemset(storage, 0, 0x20);
    storage->typeCode = 0x25;
    descriptor->storage = storage;
    parseMemset(mainBlock, 0, 0x40);
    mainBlock->typeCode = 0x27;
    descriptor->mainBlock = mainBlock;
    parseMemset(tailBlock, 0, 4);
    tailBlock->typeCode = 0x26;
    descriptor->tailBlock = tailBlock;
    storage->owner = 0;
    if (owner != 0)
    {
        storage->owner = owner;
        owner->referenceCount++;
    }
    storage->payload = payload;
    storage->kind = kind;
    payload->elementCount = (unsigned int)elementCount;
    payload->data = payload + 1;
    copySize = elementCount;
    if (kind != 2)
        copySize *= 2;
    EQGFX_MEMCPY_GUESS(payload->data, sourceData, (unsigned int)copySize);
    descriptor->reserved10 = 0x2b;
    descriptor->values[0] = values[0];
    descriptor->values[1] = values[1];
    descriptor->values[2] = values[2];
    descriptor->extraValue = extraValue;
    if (value != 0)
        storage->optionalValue = value;
    mainBlock->flags = 0;
    mainBlock->initialValue = 0.0f;
    mainBlock->allocation = GFX_COMPOSITE_ALLOCATE_MEMORY(0x40);
    if (mainBlock->allocation != 0)
    {
        mainBlock->capacity = 0x10;
        mainBlock->entryAllocation = GFX_COMPOSITE_ALLOCATE_MEMORY(0x0c);
        if (mainBlock->entryAllocation != 0)
        {
            mainBlock->reserved24 = 0;
            mainBlock->entryCount = 3;
            mainBlock->totalCount = 3;
            result = t3dInitReferenceList_parse(mainBlock->referenceSlots, 3);
            if (result == 0 && GFX_COMPOSITE_INITIALIZE_OBJECT(dictionary, descriptor) == 0)
            {
                if (label == 0 || parseAddToDictionary(dictionary, descriptor) >= 0)
                    return 0;
                GFX_COMPOSITE_RELEASE_OBJECT(dictionary, descriptor);
            }
        }
    }

cleanup:
    if (descriptor != 0)
    {
        if (mainBlock != 0)
        {
            if (mainBlock->entryAllocation != 0)
                EQGFX_T3D_FREE_MEMORY_GUESS(mainBlock->entryAllocation);
            if (mainBlock->allocation != 0)
                EQGFX_T3D_FREE_MEMORY_GUESS(mainBlock->allocation);
        }
        if (storage == 0)
            return RESOURCE_CREATE_ERROR;
        if (storage->owner != 0)
            storage->owner->referenceCount--;
    }
    if (storage != 0 && providedStorage == 0)
        EQGFX_T3D_FREE_MEMORY_GUESS(storage);
    return RESOURCE_CREATE_ERROR;
}

#define t3dCreateZone_1002055A EQ_CDECL(RegionAmbientObject_guess *, 0x1002055A, (void *))
#define t3dSetZoneObjectTag_100174B8 EQ_CDECL(int, 0x100174B8, (void *, RegionAmbientObject_guess *, int))
#define t3dAddZoneEntryToDictionary_1000DC08 EQ_CDECL(void, 0x1000DC08, (void *, RegionAmbientObject_guess *))
#define parseRegionFromNumber EQ_CDECL(void *, 0x10019346, (void *, int))
#define t3dAddRegionToZone_10020674 EQ_CDECL(void, 0x10020674, (void *, void *, RegionAmbientObject_guess *))
#define t3dDestroyZone_100205AE EQ_CDECL(void, 0x100205AE, (void *, RegionAmbientObject_guess *))
#define decryptZoneData_100223F5 EQ_CDECL(void, 0x100223F5, (void *, const void *, unsigned int))
#define t3dSetZoneLightSkipFrames_10020666 EQ_CDECL(void, 0x10020666, (RegionAmbientObject_guess *, void *))
#define ZONE_LIGHT_SKIP_FLAG 0x10

#define fillNextResourceDescriptor_10024C93 EQ_CDECL(ResourceDescriptor_guess *, 0x10024C93, (int))

// FUNCTION: EQGFX 0x100250A6
// Stamps type 0x29 on the main block of the descriptor filled next; returns that descriptor, or 0.
ResourceDescriptor_guess * __cdecl fillNextResourceDescriptorType29_guess(int contextArg)
{
    ResourceDescriptor_guess *descriptor;

    descriptor = fillNextResourceDescriptor_10024C93(contextArg);
    if (descriptor != 0)
        descriptor->mainBlock->typeCode = 0x29;
    return descriptor;
}

// FUNCTION: EQGFX 0x100250be
// Loads a renderer zone with its region membership and optional light skip data.
int __cdecl createZoneFromFileContext_guess(RegionAmbientContext_guess *context)
{
    ZoneFileEntry_guess *zoneEntry;
    RegionAmbientObject_guess *zone;
    int *regionNumber;
    unsigned int regionIndex;
    int objectTag;
    unsigned int lightSkipSize;
    void *lightSkipFrames;

    zoneEntry = (ZoneFileEntry_guess *)context->ambientEntry;
    regionNumber = zoneEntry->regionNumbers;
    zone = t3dCreateZone_1002055A(context->dictionary);
    if (zone != 0)
    {
        objectTag = zoneEntry->objectTag;
        if (objectTag != 0)
        {
            if (objectTag < 0)
                objectTag = context->objectBase - objectTag;
            else
                objectTag = 0;
            if (t3dSetZoneObjectTag_100174B8(context->dictionary, zone, objectTag) == 0)
                t3dAddZoneEntryToDictionary_1000DC08(context->dictionary, zone);
        }
        if (context->useAmbientFlag != 0)
            zone->flags |= ZONE_LIGHT_SKIP_FLAG;
        regionIndex = 0;
        if (zoneEntry->regionCount > regionIndex)
        {
            do
            {
                void *region;

                region = parseRegionFromNumber(context->dictionary, *regionNumber++);
                t3dAddRegionToZone_10020674(context->dictionary, region, zone);
                regionIndex = regionIndex + 1;
            } while (regionIndex < zoneEntry->regionCount);
        }
        lightSkipSize = *regionNumber;
        if (lightSkipSize > 0)
        {
            lightSkipFrames = GFX_COMPOSITE_ALLOCATE_MEMORY(lightSkipSize);
            if (lightSkipFrames == 0)
            {
                t3dDestroyZone_100205AE(context->dictionary, zone);
                zone = 0;
            }
            else
            {
                regionNumber++;
                decryptZoneData_100223F5(lightSkipFrames, regionNumber, lightSkipSize);
                t3dSetZoneLightSkipFrames_10020666(zone, lightSkipFrames);
            }
        }
    }
    return (int)zone;
}

#define s3dDefineDMSprite_1000EEEB EQ_CDECL(DetailedMeshSprite_guess *, 0x1000EEEB, (void *, int, int, int *, int, int *, int, int *, int, int *, int, int *, int, int *, void *, int, int *, int, int *, int, int *, int, int *, int, int, float *, float *, void *, float, unsigned int))
#define GFX_COMPOSITE_DESTROY_VOLUME EQ_CDECL(void, 0x1001D5A1, (void *, void *))
#define sprintf_10067992 EQ_CDECL(int, 0x10067992, (char *, const char *, ...))
#define openFileWithPathForTexture_10068412 EQ_CDECL(unsigned int, 0x10068412, (int, char *))
#define fprintf_100683A5 EQ_CDECL(int, 0x100683A5, (void *, const char *, ...))
#define s3dComputeDMSpriteDefPlanarEquations_parse EQ_CDECL(void, 0x1000FCC0, (void *, int))
#define g_defaultDMSpriteVolume_100F401C EQ_GLOBAL(int, 0x100F401C)
#define s_DMSpriteRebuildWarning_10087118 EQ_ARRAY(char, 0x10087118)
#define GFX_TEXTURE_LOG_NAME EQ_ARRAY(char, 0x10086164)
#define g_textureTextState_10086170 EQ_GLOBAL(int, 0x10086170)
#define s_textureTextFormat_10087114 EQ_ARRAY(char, 0x10087114)

// FUNCTION: EQGFX 0x1002519f
// Builds a DM sprite mesh from a layout-1 binary file record (32-bit counts); counts it as an obsolete object.
int __cdecl _t3dParseBinaryDMSpriteDef_guess(DMSpriteFileContext_guess *context)
{
    DMSpriteDefinitionRecord_guess *record;
    unsigned int definitionFlags;
    int *vertexData;
    int *polygonData;
    int *normalData;
    int *textureCoordinateData;
    int *textureData;
    int *paletteData;
    int *materialData;
    int *dataCursor;
    int *optionalIndexData;
    int *optionalPaletteData;
    int *optionalMaterialData;
    int optionalIndexCount;
    int optionalPaletteCount;
    int optionalMaterialCount;
    void *volumeData;
    void *auxiliaryData;
    void *volumeDefinition;
    int objectOffset;
    float baseVector[3];
    float lightVector[3];
    float spriteScale;
    DetailedMeshSprite_guess *sprite;
    DMSpriteMaterialData_guess *spriteMaterialData;
    char warningBuffer[256];
    void *outputFile;

    record = context->record;
    optionalIndexCount = 0;
    optionalIndexData = 0;
    optionalPaletteCount = 0;
    optionalPaletteData = 0;
    optionalMaterialCount = 0;
    optionalMaterialData = 0;
    vertexData = record->data;
    polygonData = vertexData + record->vertexCount * 3;
    normalData = polygonData + record->polygonCount * 2;
    textureCoordinateData = normalData + record->normalCount * 3;
    textureData = textureCoordinateData + record->textureCoordinateCount;
    paletteData = textureData + record->textureCount * 4;
    materialData = paletteData + record->paletteCount * 3;
    dataCursor = materialData + record->materialCount;
    definitionFlags = record->flags;
    if ((definitionFlags & 0x400) != 0)
    {
        optionalIndexCount = *dataCursor;
        dataCursor = dataCursor + 1;
        if (optionalIndexCount != 0)
        {
            optionalIndexData = dataCursor;
            dataCursor = dataCursor + optionalIndexCount;
        }
    }
    if ((definitionFlags & 0x800) != 0)
    {
        optionalPaletteCount = *dataCursor;
        dataCursor = dataCursor + 1;
        if (optionalPaletteCount != 0)
        {
            optionalPaletteData = dataCursor;
            dataCursor = dataCursor + optionalPaletteCount;
        }
    }
    if ((definitionFlags & 0x1000) != 0)
    {
        optionalMaterialCount = *dataCursor;
        dataCursor = dataCursor + 1;
        if (optionalMaterialCount != 0)
        {
            optionalMaterialData = dataCursor;
            dataCursor = dataCursor + optionalMaterialCount;
        }
    }
    if (record->volumeDataOffset < 0)
    {
        volumeData = (void *)t3dGetPointerFromDictionary_1000E7FE(
            context->dictionary, context->objectBase - record->volumeDataOffset);
    }
    else
    {
        volumeData = context->dataByOffset[record->volumeDataOffset];
    }
    if (record->auxiliaryDataOffset == 0)
    {
        auxiliaryData = 0;
    }
    else if (record->auxiliaryDataOffset < 0)
    {
        auxiliaryData = (void *)t3dGetPointerFromDictionary_1000E7FE(
            context->dictionary, context->objectBase - record->auxiliaryDataOffset);
    }
    else
    {
        auxiliaryData = context->dataByOffset[record->auxiliaryDataOffset];
    }
    if (record->volumeDefinitionOffset == 0)
    {
        volumeDefinition = &g_defaultDMSpriteVolume_100F401C;
    }
    else if (record->volumeDefinitionOffset < 0)
    {
        volumeDefinition = (void *)t3dGetPointerFromDictionary_1000E7FE(
            context->dictionary, context->objectBase - record->volumeDefinitionOffset);
    }
    else
    {
        volumeDefinition = context->dataByOffset[record->volumeDefinitionOffset];
    }
    if ((definitionFlags & 1) == 0)
    {
        parseMemset(baseVector, 0, 0x0c);
    }
    else
    {
        EQGFX_MEMCPY_GUESS(baseVector, record->baseVector, 0x0c);
    }
    if ((definitionFlags & 2) == 0)
    {
        spriteScale = 1.0f;
    }
    else
    {
        spriteScale = record->scale;
    }
    if ((definitionFlags & 0x2000) == 0)
    {
        parseMemset(lightVector, 0, 0x0c);
    }
    else
    {
        EQGFX_MEMCPY_GUESS(lightVector, dataCursor, 0x0c);
        dataCursor = dataCursor + 3;
    }
    if (record->offset < 0)
    {
        objectOffset = context->objectBase - record->offset;
    }
    else
    {
        objectOffset = 0;
    }
    sprite = s3dDefineDMSprite_1000EEEB(
        context->dictionary, objectOffset, record->vertexCount, vertexData,
        record->polygonCount, polygonData, record->normalCount, normalData,
        optionalIndexCount, optionalIndexData, record->textureCoordinateCount,
        textureCoordinateData, record->textureCount, textureData, volumeData,
        record->materialCount, materialData, optionalPaletteCount, optionalPaletteData,
        optionalMaterialCount, optionalMaterialData, record->paletteCount, paletteData,
        (int)auxiliaryData, 0, baseVector, lightVector, volumeDefinition, spriteScale,
        -(unsigned int)((definitionFlags & 0x4000U) != 0) & (unsigned int)dataCursor);
    if (sprite == 0)
    {
        GFX_COMPOSITE_DESTROY_VOLUME(context->dictionary, volumeDefinition);
    }
    else
    {
        if ((((unsigned char *)&record->flags)[1] & 0x80) != 0)
        {
            *((unsigned char *)&sprite->flags + 2) =
                *((unsigned char *)&sprite->flags + 2) | 0x20;
        }
        spriteMaterialData = sprite->materialData;
        if (spriteMaterialData != 0 &&
            ((spriteMaterialData->textureId == 0 || spriteMaterialData->materialId == 0) &&
             (spriteMaterialData->entryCount != 0)))
        {
            sprintf_10067992(warningBuffer, s_DMSpriteRebuildWarning_10087118, sprite->name);
            outputFile = (void *)openFileWithPathForTexture_10068412(
                (int)GFX_TEXTURE_LOG_NAME, (char *)&g_textureTextState_10086170);
            if (outputFile != 0)
            {
                fprintf_100683A5(outputFile, s_textureTextFormat_10087114, warningBuffer);
                fclose_10068328(outputFile);
            }
        }
        if (auxiliaryData == 0 && record->materialCount == 0)
        {
            s3dComputeDMSpriteDefPlanarEquations_parse(sprite, 1);
        }
        g_numObsoleteObjects_guess = g_numObsoleteObjects_guess + 1;
        if (context->setSpriteFlag != 0)
        {
            sprite->flags = sprite->flags | 0x10;
        }
    }
    return (int)sprite;
}

#define s3dCalcDMSpriteDef2AllocSize_1000F40D EQ_CDECL(unsigned int, 0x1000F40D, (int, int, int, int, int, int, int, int, int, int, int, unsigned int))
#define s3dAllocateMemoryFromPool2_10019A5C EQ_CDECL(void *, 0x10019A5C, (void *, unsigned int))
#define s3dDefineDMSprite2_1000F4A5 EQ_CDECL(int, 0x1000F4A5, (void *, void *, int, int, int, int, int, int, int, int, int, int, int, void *, void *, void *, void *, void *, void *, void *, void *, void *, int, int, int, float *, float *, void *, float, void *))
#define EQGFX_FRAME_POSITION_SCALE EQ_GLOBAL(float, 0x1007629C)
#define g_sprite2AlternateVolume_100A35CC EQ_GLOBAL(int, 0x100A35CC)

// FUNCTION: EQGFX 0x10025472
// Builds a DM sprite mesh from a layout-2 binary file record (16-bit counts, packed coords) in pool memory.
int __cdecl _t3dParseBinaryDMSpriteDef2_guess(DMSpriteFileContext_guess *context)
{
    unsigned short polygonCount;
    int coordinateOrVolumeData;
    int spriteResult;
    int objectOffset;
    unsigned int requiredSize;
    unsigned int loopCount;
    unsigned int countHighBits;
    float *nextData;
    float *vertexData;
    float *polygonSource;
    float *polygonDestination;
    float *polygonWrite;
    float *normalData;
    float *indexData;
    float *textureCoordinateData;
    float *textureData;
    float *paletteData;
    float *materialData;
    float *auxiliaryDataList;
    void *auxiliaryData;
    void *volumeDefinitionData;
    void *volumeDefinition;
    void *allocation;
    float *optionalVector;
    float vectorStorage[6];
    float spriteScale;
    DMSprite2DefinitionRecord_guess *record;
    DetailedMeshSprite_guess *sprite;

    record = (DMSprite2DefinitionRecord_guess *)context->record;
    spriteResult = 0;
    vertexData = 0;
    normalData = 0;
    indexData = 0;
    textureCoordinateData = 0;
    textureData = 0;
    paletteData = 0;
    materialData = 0;
    auxiliaryDataList = 0;
    polygonDestination = 0;
    nextData = (float *)record->data;
    if (record->vertexCount != 0)
    {
        vertexData = nextData;
        nextData = (float *)((char *)nextData + (unsigned int)record->vertexCount * 6);
    }
    polygonCount = record->polygonCount;
    if (polygonCount != 0)
    {
        if ((context->fileFlags & 0x10) == 0)
        {
            polygonDestination = (float *)GFX_COMPOSITE_ALLOCATE_MEMORY((unsigned int)polygonCount << 3);
            loopCount = 0;
            polygonSource = nextData;
            polygonWrite = polygonDestination;
            nextData = (float *)((char *)nextData + (unsigned int)polygonCount * 4);
            if (polygonCount != 0)
            {
                do
                {
                    loopCount = loopCount + 1;
                    coordinateOrVolumeData = (int)*(short *)polygonSource;
                    polygonWrite[0] = (float)coordinateOrVolumeData * EQGFX_FRAME_POSITION_SCALE;
                    coordinateOrVolumeData = (int)*(short *)((char *)polygonSource + 2);
                    polygonWrite[1] = (float)coordinateOrVolumeData * EQGFX_FRAME_POSITION_SCALE;
                    polygonSource = polygonSource + 1;
                    polygonWrite = polygonWrite + 2;
                } while (loopCount < (unsigned int)polygonCount);
            }
        }
        else
        {
            polygonDestination = nextData;
            nextData = (float *)((char *)nextData + (unsigned int)polygonCount * 8);
        }
    }
    if (record->normalCount != 0)
    {
        normalData = nextData;
        nextData = (float *)((char *)nextData + (unsigned int)record->normalCount * 3);
    }
    if (record->indexCount != 0)
    {
        indexData = nextData;
        nextData = (float *)((char *)nextData + (unsigned int)record->indexCount * 4);
    }
    if (record->textureCoordinateCount != 0)
    {
        textureCoordinateData = nextData;
        nextData = (float *)((char *)nextData + (unsigned int)record->textureCoordinateCount * 8);
    }
    if (record->textureCount != 0)
    {
        textureData = nextData;
        nextData = (float *)((char *)nextData + (unsigned int)record->textureCount * 4);
    }
    if (record->paletteCount != 0)
    {
        paletteData = nextData;
        nextData = (float *)((char *)nextData + (unsigned int)record->paletteCount * 4);
    }
    if (record->materialCount != 0)
    {
        materialData = nextData;
        nextData = (float *)((char *)nextData + (unsigned int)record->materialCount * 4);
    }
    if (record->auxiliaryCount != 0)
        auxiliaryDataList = nextData;

    if (record->volumeDataOffset < 0)
        coordinateOrVolumeData = t3dGetPointerFromDictionary_1000E7FE(
            context->dictionary, context->objectBase - record->volumeDataOffset);
    else
        coordinateOrVolumeData = (int)context->dataByOffset[record->volumeDataOffset];

    if (record->auxiliaryDataOffset == 0)
        auxiliaryData = 0;
    else if (record->auxiliaryDataOffset < 0)
        auxiliaryData = (void *)t3dGetPointerFromDictionary_1000E7FE(
            context->dictionary, context->objectBase - record->auxiliaryDataOffset);
    else
        auxiliaryData = context->dataByOffset[record->auxiliaryDataOffset];

    if (record->volumeDefinitionDataOffset == 0)
        volumeDefinitionData = 0;
    else if (record->volumeDefinitionDataOffset < 0)
        volumeDefinitionData = (void *)t3dGetPointerFromDictionary_1000E7FE(
            context->dictionary, context->objectBase - record->volumeDefinitionDataOffset);
    else
        volumeDefinitionData = context->dataByOffset[record->volumeDefinitionDataOffset];

    if ((record->flags & 0x10000) != 0)
        volumeDefinition = &g_sprite2AlternateVolume_100A35CC;
    else if (record->volumeDefinitionOffset == 0)
        volumeDefinition = &g_defaultDMSpriteVolume_100F401C;
    else if (record->volumeDefinitionOffset < 0)
        volumeDefinition = (void *)t3dGetPointerFromDictionary_1000E7FE(
            context->dictionary, context->objectBase - record->volumeDefinitionOffset);
    else
        volumeDefinition = context->dataByOffset[record->volumeDefinitionOffset];

    if ((record->flags & 1) == 0)
        parseMemset(vectorStorage, 0, 0x0c);
    else
        EQGFX_MEMCPY_GUESS(vectorStorage, record->baseVector, 0x0c);
    if ((record->flags & 0x2000) == 0)
        parseMemset(vectorStorage + 3, 0, 0x0c);
    else
        EQGFX_MEMCPY_GUESS(vectorStorage + 3, record->lightVector, 0x0c);
    if ((record->flags & 2) == 0)
        spriteScale = 1.0f;
    else
        spriteScale = record->scale;
    if ((record->flags & 0x4000) == 0)
        optionalVector = 0;
    else
        optionalVector = record->optionalVector;

    if (record->offset < 0)
        objectOffset = context->objectBase - record->offset;
    else
        objectOffset = 0;
    requiredSize = s3dCalcDMSpriteDef2AllocSize_1000F40D(
        objectOffset, record->vertexCount, record->polygonCount, record->normalCount,
        record->indexCount, record->textureCoordinateCount, record->textureCount,
        record->paletteCount, record->materialCount, record->auxiliaryCount,
        auxiliaryData != 0, context->setSpriteFlag);
    allocation = s3dAllocateMemoryFromPool2_10019A5C(context->dictionary, requiredSize);
    if (allocation != 0)
    {
        if (record->offset < 0)
            objectOffset = context->objectBase - record->offset;
        else
            objectOffset = 0;
        countHighBits = (unsigned int)vectorStorage & 0xffff0000U;
        spriteResult = s3dDefineDMSprite2_1000F4A5(
            context->dictionary, allocation, objectOffset,
            countHighBits | record->vertexCount, countHighBits | record->polygonCount,
            countHighBits | record->normalCount, countHighBits | record->indexCount,
            countHighBits | record->textureCoordinateCount, countHighBits | record->textureCount,
            countHighBits | record->paletteCount, countHighBits | record->materialCount,
            countHighBits | record->auxiliaryCount, countHighBits | record->trailingCount,
            vertexData, polygonDestination, normalData, indexData, textureCoordinateData,
            textureData, paletteData, materialData, auxiliaryDataList,
            coordinateOrVolumeData, (int)auxiliaryData, (int)volumeDefinitionData,
            vectorStorage, vectorStorage + 3, volumeDefinition, spriteScale, optionalVector);
        if (spriteResult == 0)
        {
            GFX_COMPOSITE_DESTROY_VOLUME(context->dictionary, volumeDefinition);
        }
        else
        {
            sprite = (DetailedMeshSprite_guess *)spriteResult;
            if ((((unsigned char *)&record->flags)[1] & 0x80) != 0)
                *((unsigned char *)&sprite->flags + 2) = *((unsigned char *)&sprite->flags + 2) | 0x20;
            if (context->setSpriteFlag != 0)
                sprite->flags = sprite->flags | 0x10;
            if ((context->fileFlags & 0x10) == 0 && polygonDestination != 0)
                EQGFX_T3D_FREE_MEMORY_GUESS(polygonDestination);
        }
    }
    return spriteResult;
}

#define t3dDefineParticleCloud_1003FE47 EQ_CDECL(ParticleCloudObject_guess *, 0x1003FE47, (void *, int, int, int, int, float, void *, void *, void *, int, int, void *, void *, void *, float, float, int, float, int, float, void *, void *))

// FUNCTION: EQGFX 0x1002580c
// Loads a reusable particle cloud effect definition from a binary world record.
ParticleCloudObject_guess *__cdecl t3dParseBinaryPCloudDef(ParticleCloudContext_guess *context)
{
    ParticleCloudRecord_guess *record;
    unsigned int recordFlags;
    unsigned char *vectorStart;
    unsigned char *lookupIndex;
    void *frameData;
    void *frameSource;
    void *skippedFrames;
    void *skippedData;
    void *lookupResult;
    int objectTag;
    ParticleCloudObject_guess *cloud;

    record = context->particleRecord;
    frameSource = 0;
    frameData = 0;
    skippedFrames = 0;
    skippedData = 0;
    recordFlags = record->flags;
    vectorStart = record->payload54;
    if ((recordFlags & 1) != 0)
    {
        frameData = vectorStart + 0x0c;
        frameSource = vectorStart;
        vectorStart = vectorStart + 0x18;
    }
    if ((recordFlags & 2) != 0)
    {
        skippedFrames = vectorStart;
        skippedData = vectorStart + 0x0c;
        lookupIndex = vectorStart + 0x18;
    }
    else
    {
        lookupIndex = vectorStart;
    }
    lookupResult = 0;
    if ((recordFlags & 4) != 0)
        lookupResult = context->objectTable[*lookupIndex];
    if (record->objectTag < 0)
        objectTag = context->objectBase - record->objectTag;
    else
        objectTag = 0;
    cloud = t3dDefineParticleCloud_1003FE47(context->dictionary, objectTag,
        record->parameter08, record->parameter10, record->parameter14,
        record->parameter18, record->vector1c, skippedFrames, skippedData,
        record->parameter0c, record->parameter28, frameSource, frameData,
        record->vector3c, record->parameter2c, record->parameter30,
        record->parameter34, record->parameter38, record->parameter48,
        record->parameter4c, &record->parameter50, lookupResult);
    if (cloud != 0 && context->useAmbientFlag != 0)
        cloud->flags |= 0x10;
    return cloud;
}

// FUNCTION: EQGFX 0x100258FD
// Sets the world's constant ambient lighting from parser state.
int __cdecl s3dSetWorldConstantAmbientFromSource_guess(WorldAmbientSource_guess *context)
{
    s3dSetWorldConstantAmbient_100202E2(context->ambientValueAt04, context->ambientValueAt8044);
    return (int)0xdddddddd;
}

#define t3dGetPointerFromDictionary_1000E7FE EQ_CDECL(void *, 0x1000E7FE, (void *, int))
#define s3dCreateDMSprite_1001034C EQ_CDECL(DetailedMeshSprite_guess *, 0x1001034C, (void *, void *, int))
#define t3dSetObjectTag_100174B8 EQ_CDECL(int, 0x100174B8, (void *, DetailedMeshSprite_guess *, int))
#define t3dAddEntryToDictionary_1000DC08 EQ_CDECL(void, 0x1000DC08, (void *, DetailedMeshSprite_guess *))

// FUNCTION: EQGFX 0x10025917
// Adds a detailed mesh sprite to the world with its optional name tag.
int __cdecl createDetailedMeshSpriteWithTag_guess(DMTrackFileContext_guess *context)
{
    DMTrack2Record_guess *record;
    int dataOffset;
    void *meshData;
    DetailedMeshSprite_guess *sprite;
    int tagOffset;
    int tagResult;

    record = context->record;
    dataOffset = record->dataOffset;
    if (dataOffset < 0)
    {
        meshData = t3dGetPointerFromDictionary_1000E7FE(context->handle,
            context->fileSize - dataOffset);
    }
    else
    {
        meshData = context->dataByOffset[dataOffset];
    }
    sprite = s3dCreateDMSprite_1001034C(context->handle, meshData, 0);
    if (sprite != 0)
    {
        tagOffset = record->offset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->fileSize - tagOffset;
            else
                tagOffset = 0;
            tagResult = t3dSetObjectTag_100174B8(context->handle, sprite, tagOffset);
            if (tagResult == 0)
                t3dAddEntryToDictionary_1000DC08(context->handle, sprite);
        }
        if (context->setTrackFlag != 0)
            sprite->flags |= 0x10;
    }
    return (int)sprite;
}

#define s3dDefineDMTrack_1001075C EQ_CDECL(DMTrack2_guess *, 0x1001075C, (void *, int, int, int, int, int, void **))
#define createDMTrackFromFileContext_guess createDMTrackFromFileContext_1002599A_guess

// FUNCTION: EQGFX 0x1002599A
// Loads a mesh animation track definition from stored vertex frames.
int __cdecl createDMTrackFromFileContext_guess(DMTrackFileContext_guess *context)
{
    DMTrackFileContext_guess *fileContext;
    DMTrackArrayRecord_guess *record;
    int *entryData;
    void **entryCursor;
    unsigned int entryIndex;
    int fileOffset;
    DMTrack2_guess *track;

    fileContext = context;
    context = 0;
    record = (DMTrackArrayRecord_guess *)fileContext->record;
    entryData = (int *)record->entryData;
    if (record->entryCount > 0)
    {
        context = (DMTrackFileContext_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(record->entryCount << 2);
        entryIndex = 0;
        if (context == 0)
            return 0;
        entryCursor = (void **)context;
        if (record->entryCount > 0)
        {
            do
            {
                *entryCursor = entryData;
                entryIndex++;
                entryCursor++;
                entryData += record->entryStride * 3;
            } while (entryIndex < record->entryCount);
        }
    }
    if (record->offset < 0)
        fileOffset = fileContext->fileSize - record->offset;
    else
        fileOffset = 0;
    track = s3dDefineDMTrack_1001075C(fileContext->handle, fileOffset, record->entryStride,
        record->entryCount, record->field14, record->field10, (void **)context);
    if (track != 0 && fileContext->setTrackFlag != 0)
        track->flags |= 0x10;
    if (record->entryCount > 0)
        EQGFX_T3D_FREE_MEMORY_GUESS(context);
    return (int)track;
}

#undef createDMTrackFromFileContext_guess

#define s3dDefineDMTrack2_1001090B EQ_CDECL(DMTrack2_guess *, 0x1001090B, (void *, int, int, short, short, short, short, unsigned int, void *))

// FUNCTION: EQGFX 0x10025A3D
// Defines a DM track from its world record data.
// __cdecl. Defines the DM track from context->record (a negative record offset counts back from fileSize) and returns it; flag 0x10 is set on it when setTrackFlag is nonzero.
DMTrack2_guess *__cdecl defineDMTrack2FromFileContext_guess(DMTrackFileContext_guess *context)
{
    DMTrack2Record_guess *record;
    int recordOffset;
    unsigned char *recordData;
    int dataOffset;
    DMTrack2_guess *track;

    record = context->record;
    recordOffset = record->offset;
    recordData = record->data;
    if (recordOffset < 0)
        dataOffset = context->fileSize - recordOffset;
    else
        dataOffset = 0;
    track = s3dDefineDMTrack2_1001090B(context->handle, 0, dataOffset, record->field08, record->field0a,
        record->field0e, record->field10, record->field0c, recordData);
    if (track != 0 && context->setTrackFlag != 0)
        track->flags |= 0x10;
    return track;
}

#define s3dCreateDMTrack_10010AD8 EQ_CDECL(DMTrack2_guess *, 0x10010AD8, (void *, void *, int, float))
#define t3dSetDMTrackObjectTag_100174B8 EQ_CDECL(int, 0x100174B8, (void *, void *, int))
#define t3dAddDMTrackEntryToDictionary_1000DC08 EQ_CDECL(void, 0x1000DC08, (void *, void *))

// FUNCTION: EQGFX 0x10025A97
// Creates a DM track instance from referenced track data.
int __cdecl createDMTrackFromFileContext_guess(DMTrackFileContext_guess *context)
{
    DMTrack2Record_guess *record;
    int dataOffset;
    void *trackData;
    DMTrack2_guess *track;
    int tagOffset;
    int tagResult;

    record = context->record;
    dataOffset = record->dataOffset;
    if (dataOffset < 0)
    {
        trackData = t3dGetPointerFromDictionary_1000E7FE(context->handle,
            context->fileSize - dataOffset);
    }
    else
    {
        trackData = context->dataByOffset[dataOffset];
    }
    track = s3dCreateDMTrack_10010AD8(context->handle, trackData, -1, 1.0f);
    if (track != 0)
    {
        tagOffset = record->offset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->fileSize - tagOffset;
            else
                tagOffset = 0;
            tagResult = t3dSetDMTrackObjectTag_100174B8(context->handle, track, tagOffset);
            if (tagResult == 0)
                t3dAddDMTrackEntryToDictionary_1000DC08(context->handle, track);
        }
        if (context->setTrackFlag != 0)
            track->flags |= 0x10;
    }
    return (int)track;
}

#define g_lightingState_guess EQ_GLOBAL(unsigned int, 0x100AADEC)
#define s__DMSPRITEDEF_10087178 EQ_ARRAY(char, 0x10087178)
#define s3dGetDMSpriteDefinitionNumVertices_10010AAA EQ_CDECL(int, 0x10010AAA, (void *))
#define s3dPatchDMSpriteDefLighting_1001156D EQ_CDECL(void, 0x1001156D, (void *, void *))
#define s3dPatchDMTrackDefLighting_100115DC EQ_CDECL(void, 0x100115DC, (void *, void *))
#define s3dDefineDMRGBTrack_10010C30 EQ_CDECL(DMTrack2_guess *, 0x10010C30, (void *, int, int, int, int, int, void **))

// FUNCTION: EQGFX 0x10025B20
// Loads a vertex color track definition, or applies its first frame as existing mesh lighting.
DMTrack2_guess *__cdecl t3dParseBinaryDMRGBTrackDef(DMTrackFileContext_guess *context)
{
    DMTrackFileContext_guess *fileContext;
    DMTrackArrayRecord_guess *record;
    int *entryData;
    void **entryCursor;
    unsigned int entryIndex;
    int fileOffset;
    DMTrack2_guess *track;
    char *nameSource;
    unsigned int nameLength;
    void *definition;
    int lightingPatched;
    char name[256];

    fileContext = context;
    context = 0;
    lightingPatched = 0;
    record = (DMTrackArrayRecord_guess *)fileContext->record;
    entryData = (int *)record->entryData;
    if (record->entryCount > 0)
    {
        context = (DMTrackFileContext_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(record->entryCount << 2);
        entryIndex = 0;
        if (context == 0)
        {
            track = 0;
            goto done;
        }
        if (record->entryCount > 0)
        {
            entryCursor = (void **)context;
            do
            {
                *entryCursor = entryData;
                entryIndex++;
                entryCursor++;
                entryData += record->entryStride;
            } while (entryIndex < record->entryCount);
        }
    }
    if (g_lightingState_guess != 0)
    {
        if (record->offset < 0)
            nameSource = (char *)(fileContext->fileSize - record->offset);
        else
            nameSource = 0;
        if (nameSource != 0)
        {
            if (*nameSource == 'R')
            {
                nameLength = strlen_EQGFX(nameSource);
                if (nameLength > 0xc)
                {
                    GFX_PATH_COPY_STRING(name, nameSource);
                    GFX_PATH_COPY_STRING(name + (strlen_EQGFX(nameSource) - 0xb), s__DMSPRITEDEF_10087178);
                    definition = t3dGetPointerFromDictionary_1000E7FE(fileContext->handle, (int)name);
                    if (definition != 0 && record->entryStride == s3dGetDMSpriteDefinitionNumVertices_10010AAA(definition))
                    {
                        lightingPatched = 1;
                        s3dPatchDMSpriteDefLighting_1001156D(definition, *(void **)context);
                    }
                }
            }
            else if (*nameSource == 'P' && (nameLength = strlen_EQGFX(nameSource), nameLength > 2))
            {
                GFX_PATH_COPY_STRING(name, nameSource + 2);
                definition = t3dGetPointerFromDictionary_1000E7FE(fileContext->handle, (int)name);
                if (definition != 0 && record->entryStride == ((DMTrackDef_guess *)definition)->vertexCount)
                {
                    lightingPatched = 1;
                    s3dPatchDMTrackDefLighting_100115DC(definition, *(void **)context);
                }
            }
        }
        if (g_lightingState_guess != 0 && lightingPatched != 0)
        {
            /* the patched path hands back the entry array, as the original does */
            track = (DMTrack2_guess *)context;
            goto cleanup;
        }
    }
    if (record->offset < 0)
        fileOffset = fileContext->fileSize - record->offset;
    else
        fileOffset = 0;
    track = s3dDefineDMRGBTrack_10010C30(fileContext->handle, fileOffset, record->entryStride,
        record->entryCount, record->field14, record->field10, (void **)context);
    if (track != 0)
    {
        if (*(unsigned char *)&record->reserved04 & 1)
            *((unsigned char *)&track->flags + 2) |= 0x80;
        if (fileContext->setTrackFlag != 0)
            track->flags |= 0x10;
    }
cleanup:
    if (record->entryCount > 0)
        EQGFX_T3D_FREE_MEMORY_GUESS(context);
done:
    return track;
}

// FUNCTION: EQGFX 0x10025cdb
// Creates a vertex color track instance from a referenced definition and assigns its world name.
int __cdecl createDMRGBTrackFromFileContext_guess(DMTrackFileContext_guess *context)
{
    DMTrack2Record_guess *record;
    int dataOffset;
    void *trackData;
    DMTrack2_guess *track;
    int tagOffset;
    int tagResult;

    record = context->record;
    dataOffset = record->dataOffset;
    if (dataOffset < 0)
    {
        trackData = t3dGetPointerFromDictionary_1000E7FE(context->handle,
            context->fileSize - dataOffset);
    }
    else
    {
        trackData = context->dataByOffset[dataOffset];
    }
    track = EQ_CDECL(DMTrack2_guess *, 0x10010E08, (void *, void *, int, float))(
        context->handle, trackData, -1, 1.0f);
    if (track != 0)
    {
        tagOffset = record->offset;
        if (tagOffset != 0)
        {
            if (tagOffset < 0)
                tagOffset = context->fileSize - tagOffset;
            else
                tagOffset = 0;
            tagResult = t3dSetDMTrackObjectTag_100174B8(
                context->handle, track, tagOffset);
            if (tagResult == 0)
                t3dAddDMTrackEntryToDictionary_1000DC08(context->handle, track);
        }
        if (context->setTrackFlag != 0)
            track->flags |= 0x10;
    }
    return (int)track;
}

#define s3dIsMaterialDefinitionValid_10016CC9 EQ_CDECL(int, 0x10016CC9, (void *))
#define s3dCalcMaterialDefAllocSize_10016CDA EQ_CDECL(unsigned int, 0x10016CDA, (int, int))
#define s3dAllocateMemoryFromPool_10019A5C EQ_CDECL(void *, 0x10019A5C, (void *, unsigned int, int))
#define s3dDefineMaterial_10016CF2 EQ_CDECL(MaterialDefinition_guess *, 0x10016CF2, (void *, void *, int))
#define s3dSetMaterialSimpleSprite_parse EQ_CDECL(void, 0x10016E7D, (void *, void *, void *))
#define s3dSetMaterialAmbientScalar_parse EQ_CDECL(void, 0x10016EB0, (void *, void *, float))
#define s3dSetMaterialIntensity_parse EQ_CDECL(void, 0x10016EBE, (void *, void *, float))
#define s3dSetMaterialTwoSided_parse EQ_CDECL(void, 0x10016ECC, (void *, void *, int))
#define s3dSetMaterialUVShift_parse EQ_CDECL(void, 0x10016EDA, (void *, void *, float, float))

// FUNCTION: EQGFX 0x10025d64
// Loads a material's sprite, lighting, sidedness and optional texture scrolling from a world record.
int __cdecl createMaterialDefinitionFromFileContext_guess(MaterialDefinitionFileContext_guess *context)
{
    MaterialDefinitionRecord_guess *record;
    void *simpleSprite;
    MaterialDefinition_guess *definition;
    int fileOffset;
    unsigned int allocationSize;
    void *allocation;
    float *uvShift;
    float *recordUvShift;
    int twoSided;

    twoSided = 0;
    uvShift = 0;
    record = context->record;
    recordUvShift = record->uvShiftPerMs;
    if (record->simpleSpriteOffset < 0)
    {
        simpleSprite = t3dGetPointerFromDictionary_1000E7FE(context->dictionary,
            context->fileSize - record->simpleSpriteOffset);
    }
    else
    {
        simpleSprite = context->dataByOffset[record->simpleSpriteOffset];
    }
    if ((record->flags & 1) != 0)
        twoSided = 1;
    if ((record->flags & 2) != 0)
        uvShift = recordUvShift;
    if (record->definitionOffset < 0)
        fileOffset = context->fileSize - record->definitionOffset;
    else
        fileOffset = 0;
    definition = (MaterialDefinition_guess *)t3dGetPointerFromDictionary_1000E7FE(
        context->dictionary, fileOffset);
    if (definition == 0 || s3dIsMaterialDefinitionValid_10016CC9(definition) != 0)
    {
        if (record->definitionOffset < 0)
            fileOffset = context->fileSize - record->definitionOffset;
        else
            fileOffset = 0;
        allocationSize = s3dCalcMaterialDefAllocSize_10016CDA(fileOffset,
            context->setMaterialFlag);
        allocation = s3dAllocateMemoryFromPool_10019A5C(context->dictionary, allocationSize,
            context->setMaterialFlag);
        if (allocation != 0)
        {
            if (record->definitionOffset < 0)
                fileOffset = context->fileSize - record->definitionOffset;
            else
                fileOffset = 0;
            definition = s3dDefineMaterial_10016CF2(context->dictionary, allocation,
                fileOffset);
            if (definition != 0)
            {
                if (context->setMaterialFlag != 0)
                    definition->flags |= 0x10;
                definition->materialValue = record->materialValue;
                s3dSetMaterialSimpleSprite_parse(context->dictionary, definition,
                    simpleSprite);
                s3dSetMaterialAmbientScalar_parse(context->dictionary, definition,
                    record->ambientScalar);
                s3dSetMaterialIntensity_parse(context->dictionary, definition,
                    record->constantIntensity);
                s3dSetMaterialTwoSided_parse(context->dictionary, definition, twoSided);
                definition->hasSurfaceFlags =
                    !!(record->surfaceFlags & 0xff000000u);
                if (uvShift != 0)
                {
                    s3dSetMaterialUVShift_parse(context->dictionary, definition,
                        uvShift[0], uvShift[1]);
                }
            }
        }
    }
    return (int)definition;
}

// FUNCTION: EQGFX 0x10025ec8
// Builds a world material palette from the record's material references and optional name tag.
int __cdecl t3dParseBinaryMaterialPal(MaterialPaletteLoadContext_guess *context)
{
    MaterialPaletteRecord_guess *record;
    unsigned int *destination;
    int fileOffset;
    void *allocatedData;
    int result;
    int tagOffset;
    int tagResult;
    unsigned int index;

    result = 0;
    record = context->record;
    allocatedData = GFX_COMPOSITE_ALLOCATE_MEMORY(record->entryCount * 4 + 4);
    if (allocatedData != 0)
    {
        index = 0;
        destination = (unsigned int *)allocatedData;
        if (record->entryCount != 0)
        {
            do
            {
                fileOffset = record->dataOffsets[index];
                if (fileOffset < 0)
                    *destination = (unsigned int)t3dGetPointerFromDictionary_1000E7FE(
                        context->handle, context->fileSize - fileOffset);
                else
                    *destination = (unsigned int)context->dataByOffset[fileOffset];
                destination++;
                index++;
            } while (index < record->entryCount);
        }
        result = EQ_CDECL(int, 0x10016F39, (void *, unsigned int, void *, int))(
            context->handle, record->entryCount, allocatedData, 1);
        if (result == 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(allocatedData);
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(allocatedData);
            tagOffset = record->tagOffset;
            if (tagOffset != 0)
            {
                if (tagOffset < 0)
                    tagOffset = context->fileSize - tagOffset;
                else
                    tagOffset = 0;
                tagResult = t3dSetObjectTag_100174B8(context->handle, result, tagOffset);
                if (tagResult == 0)
                    t3dAddEntryToDictionary_1000DC08(context->handle, result);
            }
            if (context->setPaletteFlag != 0)
                ((MaterialPalette_guess *)result)->flags |= 0x10;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10025fb1
// Loads an accompanying file from the same archive; its rendering purpose is unclear.
int __cdecl t3dLoadCompanionFile_guess(VISLoadContext_guess *context)
{
    char fileName[14];
    char *extension;
    int fileHandle;
    VISFileData_guess *fileData;
    unsigned int fileSize;

    strncpy_100688E0(fileName, context->fileName, 0x0d);
    fileName[13] = 0;
    extension = strrchr_10069200(fileName, 0x2e);
    if (extension != 0)
    {
        if ((unsigned int)(extension - fileName) < 10)
        {
            GFX_PATH_COPY_STRING(extension, s_VISExtension_10087188);
            if ((context->archiveId != 0) &&
                ((fileHandle = t3dFastFileOpen_10021791(context->archiveId, fileName)) != 0))
            {
                fileData = (VISFileData_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(0x10);
                if (fileData == 0)
                {
                    t3dFastFileClose_10021838((void *)fileHandle);
                }
                else
                {
                    fileData->reserved00 = 0;
                    fileData->reserved04 = 0;
                    fileData->reserved08 = 0;
                    fileSize = t3dFastFileGetEOF_100219A3((void *)fileHandle);
                    fileData->data = GFX_COMPOSITE_ALLOCATE_MEMORY(fileSize);
                    if (fileData->data != 0)
                    {
                        t3dFastFileRead_1002186A((void *)fileHandle, fileData->data, fileSize);
                        t3dFastFileClose_10021838((void *)fileHandle);
                        context->loadedData = fileData;
                        return 0;
                    }
                    t3dFastFileClose_10021838((void *)fileHandle);
                    EQGFX_T3D_FREE_MEMORY_GUESS(fileData);
                }
            }
        }
    }
    return 0xffff3d5c;
}

// FUNCTION: EQGFX 0x1002607f
// Releases a loaded file resource, its data buffer, and its open handle.
void __cdecl freeObjectDataAndHandle_guess(FreeObjectData_guess *object)
{
    if (object->data != 0)
        EQGFX_T3D_FREE_MEMORY_GUESS(object->data);
    if (object->handle != 0)
        pfn_CloseHandle_guess(object->handle);
    EQGFX_T3D_FREE_MEMORY_GUESS(object);
    return;
}

#define createParserObject_10028FDF EQ_CDECL(ParserObject_guess *, 0x10028FDF, (int, int))
#define createFilePositionSource_100263A6 EQ_CDECL(FilePositionSource_guess *, 0x100263A6, (void *, const char *, int, int))
#define t3dCreateWorldFileReader_guess EQ_CDECL(int, 0x10028D54, (ParserObject_guess *, int, int))
#define _t3dParseTokenStream EQ_CDECL(void, 0x100290CE, (ParserObject_guess *, int))
#define destroyFileQueueOwner_guess EQ_CDECL(void, 0x10028E20, (int))
#define freeFileParser_guess EQ_CDECL(void, 0x10029020, (ParserObject_guess *))
#define logParserError_1002902B EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, const char *))
#define freeObjectAndData_10026411 EQ_CDECL(void, 0x10026411, (int))
#define s_unknown_100871E0 EQ_ARRAY(char, 0x100871E0)
#define s_outOfMemory_100871D0 EQ_ARRAY(char, 0x100871D0)
#define WORLD_PARSE_FAILED ((unsigned int)0xFFFF3D3E)
#define WORLD_PARSE_FALLBACK_REQUIRED ((unsigned int)0xFFFF3D5A)
#define checkWorldPath_10021E85 EQ_CDECL(unsigned int, 0x10021E85, (const char *, int, int, int, int, int))
#define openWorldFile_100262F0 EQ_CDECL(OpenFileObject_guess *, 0x100262F0, (const char *, int))
#define closeWorldFile_1002642D EQ_CDECL(void, 0x1002642D, (FilePositionSource_guess *))
#define logWorldOpenFailure_1002902B EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, const char *, const char *))
#define EQGFX_WORLD_LOAD_MODE EQ_GLOBAL(int, 0x100AADE4)
#define s_cannotOpenWorldFile_10087190 EQ_ARRAY(char, 0x10087190)

// FUNCTION: EQGFX 0x100260c7
// Thunk for reading a world for palette use with the requested load mode.
#define t3dParseReadWorld_100260E7_forPalette EQ_CDECL(unsigned int, 0x100260E7, (const char *, int, int, int, int, int, int))
void __cdecl t3dParseReadWorldWithPalette(const char *worldFilePath, int world, int archiveId,
                                          int worldLoadMode, int worldContext)
{
    t3dParseReadWorld_100260E7_forPalette(worldFilePath, world, archiveId, worldLoadMode,
                                          worldContext, 0, 0);
    return;
}

// FUNCTION: EQGFX 0x100260a9
// Thunk for reading a world file with default load settings.
void __cdecl t3dParseReadWorldFromFile(const char *worldFilePath, int world, int archiveId, int worldContext)
{
    t3dParseReadWorld_100260E7_forPalette(worldFilePath, world, archiveId, 0, worldContext, 0, 0);
    return;
}

// FUNCTION: EQGFX 0x100260e7
// Loads a world from a file path, falling back to token parsing when the initial loader requests it.
unsigned int __cdecl t3dParseReadWorld(const char *worldFilePath, int world, int archiveId,
                                      int worldLoadMode, int worldContext, int optionalWorldData,
                                      int optionalWorldMetadata)
{
    unsigned int result;
    ParserObject_guess *parser;
    OpenFileObject_guess *worldFile;
    int parsedWorld;

    result = checkWorldPath_10021E85(worldFilePath, world, archiveId, worldContext,
                                     optionalWorldData, optionalWorldMetadata);
    if (result == WORLD_PARSE_FALLBACK_REQUIRED)
    {
        parser = createParserObject_10028FDF(world, archiveId);
        if (parser == 0)
            return WORLD_PARSE_FAILED;

        parser->worldLoadMode = worldLoadMode;
        EQGFX_WORLD_LOAD_MODE = worldContext;
        parser->worldContext = worldContext;
        if (optionalWorldData != 0)
        {
            parser->optionalWorldData = optionalWorldData;
            parser->optionalWorldMetadata = optionalWorldMetadata;
        }

        worldFile = openWorldFile_100262F0(worldFilePath, archiveId);
        if (worldFile == 0)
        {
            logWorldOpenFailure_1002902B(parser, s_cannotOpenWorldFile_10087190, worldFilePath);
            freeFileParser_guess(parser);
            result = WORLD_PARSE_FAILED;
        }
        else
        {
            parsedWorld = t3dCreateWorldFileReader_guess(parser, (int)worldFile, 0);
            if (parsedWorld != 0)
            {
                _t3dParseTokenStream(parser, parsedWorld);
                result = -(unsigned int)(parser->errorCount != 0) & WORLD_PARSE_FAILED;
                closeWorldFile_1002642D((FilePositionSource_guess *)worldFile);
                freeObjectAndData_10026411((int)worldFile);
                destroyFileQueueOwner_guess(parsedWorld);
                freeFileParser_guess(parser);
            }
            else
            {
                freeFileParser_guess(parser);
                freeObjectAndData_10026411((int)worldFile);
                result = WORLD_PARSE_FAILED;
            }
        }
    }

    EQGFX_WORLD_LOAD_MODE = 0;
    return result;
}

// FUNCTION: EQGFX 0x100261E4
// __cdecl. Returns 0 on success, WORLD_PARSE_FAILED when setup or the parse failed.
unsigned int __cdecl t3dParseReadWorldFromStream(void *file, int world, int reservedValue)
{
    ParserObject_guess *parser;
    int reader;
    unsigned int result;

    parser = createParserObject_10028FDF(world, reservedValue);
    if (parser != 0)
    {
        reservedValue = (int)createFilePositionSource_100263A6(file, s_unknown_100871E0, 0, reservedValue);
        if (reservedValue == 0)
        {
            logParserError_1002902B(parser, s_outOfMemory_100871D0);
            freeFileParser_guess(parser);
        }
        else
        {
            reader = t3dCreateWorldFileReader_guess(parser, reservedValue, 0);
            if (reader == 0)
            {
                result = WORLD_PARSE_FAILED;
            }
            else
            {
                _t3dParseTokenStream(parser, reader);
                result = -(unsigned int)(parser->errorCount != 0) & WORLD_PARSE_FAILED;
                destroyFileQueueOwner_guess(reader);
            }
            freeFileParser_guess(parser);
            freeObjectAndData_10026411(reservedValue);
            return result;
        }
    }
    return WORLD_PARSE_FAILED;
}

// FUNCTION: EQGFX 0x10026280
// Shows the player a world-file parsing error.
// __cdecl (callee pops 0 bytes; callers do not set ECX). Shows message in a
// "..." error box (NULL owner, MB_SETFOREGROUND | MB_TASKMODAL |
// MB_ICONERROR). The first argument is unused. Always returns 1.
int __cdecl _t3dParseOnError(int unused, const char *displayMessage)
{
    pfn_MessageBoxA_guess(0, displayMessage, s_WorldFileParser_guess, WORLD_PARSER_MSGBOX_FLAGS);
    return 1;
}

#define g_lightingState_guess EQ_GLOBAL(unsigned int, 0x100AADEC)
#define g_textureFile_guess EQ_GLOBAL(unsigned int, 0x100AADF0)
#define g_textureFileMode_guess EQ_ARRAY(char, 0x10087200)
#define fclose_10068328 EQ_CDECL(int, 0x10068328, (unsigned int))
#define PATCH_LIGHTING_OPEN_FAILED ((unsigned int)0xFFFF3D3E)

// FUNCTION: EQGFX 0x100262A7
// Configures patch lighting and its optional data file.
unsigned int __cdecl s3dParsePatchLighting(unsigned int lightingState, int filePath)
{
    g_lightingState_guess = lightingState;
    if (g_textureFile_guess != 0)
    {
        fclose_10068328(g_textureFile_guess);
        g_textureFile_guess = 0;
    }
    if (filePath != 0)
    {
        g_textureFile_guess = openFileWithPathForTexture_10068412(filePath, g_textureFileMode_guess);
        if (g_textureFile_guess == 0)
            return PATCH_LIGHTING_OPEN_FAILED;
    }
    return 0;
}

// FUNCTION: EQGFX 0x100262f0
// Opens a world file for loading from disk or an archive.
OpenFileObject_guess *__cdecl _t3dParseFileOpen(const char *path, int archiveId)
{
    OpenFileObject_guess *fileObject;
    int fileHandle;
    unsigned int pathLength;
    char *allocatedPath;

    fileObject = GFX_COMPOSITE_ALLOCATE_MEMORY(0x18);
    if (fileObject == 0)
        return 0;

    fileObject->archiveId = archiveId;
    if (archiveId == 0)
    {
        fileObject->fileType = 1;
        fileObject->handle = pfn_CreateFileA_guess(path, FILE_GENERIC_READ, FILE_SHARE_READ, 0, FILE_OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (fileObject->handle == INVALID_FILE_HANDLE)
            goto failed;
    }
    else
    {
        fileObject->fileType = 0;
        fileHandle = t3dFastFileOpen_10021791(archiveId, path);
        fileObject->handle = (void *)fileHandle;
        if (fileHandle == 0)
            goto failed;
        t3dFastFileSeek_10021897(fileObject->handle, 0, 2);
        fileObject->fileSize = t3dFastFileTell_1002191D(fileObject->handle);
        fileObject->savedPosition = 0;
        t3dFastFileSeek_10021897(fileObject->handle, 0, 0);
    }

    pathLength = strlen_EQGFX(path);
    allocatedPath = GFX_COMPOSITE_ALLOCATE_MEMORY(pathLength + 1);
    fileObject->path = allocatedPath;
    if (allocatedPath != 0)
    {
        GFX_PATH_COPY_STRING(allocatedPath, path);
        return fileObject;
    }

failed:
    EQGFX_T3D_FREE_MEMORY_GUESS(fileObject);
    return 0;
}

// FUNCTION: EQGFX 0x100263a6
// Wraps a file stream as a position-aware source for world parsing.
FilePositionSource_guess *__cdecl createFilePositionSource_guess(void *file, char *path, int unusedValue, int reservedValue)
{
    FilePositionSource_guess *source;
    unsigned int pathLength;
    unsigned int savedPosition;
    int allocatedPathAddress;

    source = GFX_COMPOSITE_ALLOCATE_MEMORY(0x18);
    if (source != 0)
    {
        source->reserved04 = reservedValue;
        source->sourceType = 2;
        source->file = file;
        savedPosition = getFilePosition_10026525(source);
        source->path = 0;
        source->savedPosition = savedPosition;
        if (path != 0)
        {
            pathLength = strlen_EQGFX(path);
            allocatedPathAddress = (int)GFX_COMPOSITE_ALLOCATE_MEMORY(pathLength + 1);
            source->path = (char *)allocatedPathAddress;
            if (allocatedPathAddress != 0)
                GFX_PATH_COPY_STRING((char *)allocatedPathAddress, path);
            else
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(source);
                return 0;
            }
        }
        return source;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10026411
// Frees a parser data source and its copied path string (see createFilePositionSource_guess).
void __cdecl freeObjectAndData_guess(FreeObjectData_guess *object)
{
    if (object->data != 0)
        EQGFX_T3D_FREE_MEMORY_GUESS(object->data);
    EQGFX_T3D_FREE_MEMORY_GUESS(object);
    return;
}

// FUNCTION: EQGFX 0x1002642D
// Closes the file backing a parser data source.
void __cdecl closeFileHandle_guess(FilePositionSource_guess *source)
{
    switch (source->sourceType)
    {
    case 0:
        t3dFastFileClose_10021838(source->file);
        break;
    case 1:
        pfn_CloseHandle_guess(source->file);
        return;
    case 2:
        fclose_10068328(source->file);
        return;
    }
    return;
}

// FUNCTION: EQGFX 0x1002645C
// Supplies world parsing with bytes from its active data source.
// __cdecl, size_t back. Reads up to count bytes into buffer by source->sourceType: 0 fast file (clipped to the bytes left),
// 1 ReadFile, 2 fread; any other kind returns 0xFFFFFFFF.
size_t __cdecl t3dReadSourceBytes_guess(FilePositionSource_guess *source, void *buffer, size_t count)
{
    int bytesLeft;
    size_t bytesToRead;
    size_t bytesRead;

    bytesRead = 0xffffffff;
    switch (source->sourceType)
    {
    case 0:
        bytesLeft = source->fileSize - source->savedPosition;
        bytesToRead = count;
        if (bytesLeft < (int)count)
            bytesToRead = bytesLeft;
        t3dFastFileRead_1002186A(source->file, buffer, bytesToRead);
        source->savedPosition += bytesToRead;
        bytesRead = bytesToRead;
        break;
    case 1:
        pfn_ReadFile_100760A8(source->file, buffer, count, &count, 0);
        bytesRead = count;
        break;
    case 2:
        bytesRead = fread_10069227(buffer, 1, count, source->file);
        break;
    }
    return bytesRead;
}

// FUNCTION: EQGFX 0x100264CA
// Seeks a parser data source by its sourceType (fast file, Win32 handle or stdio stream).
// __cdecl, unsigned int back. Seeks to the start by sourceType: 0 fast file (clears savedPosition), 1 SetFilePointer, 2 fseek.
unsigned int __cdecl seekFilePositionSource_guess(FilePositionSource_guess *source)
{
    int sourceType;
    int seekResult;
    unsigned int newPosition;
    unsigned int result;

    result = 0;
    sourceType = source->sourceType;
    switch (sourceType)
    {
    case 0:
        result = t3dFastFileSeek_10021897(source->file, 0, 0);
        source->savedPosition = 0;
        break;
    case 1:
        newPosition = pfn_SetFilePointer_guess(source->file, 0, 0, 0);
        if (newPosition != 0xffffffff)
            result = 1;
        break;
    case 2:
        seekResult = fseek_1006933E(source->file, 0, 0);
        result = (seekResult == 0);
        break;
    }
    return result;
}

// FUNCTION: EQGFX 0x10026525
// Gets the current read position in a parser data source.
unsigned int __cdecl GetSourcePosition_guess(FilePositionSource_guess *source)
{
    int sourceType;
    unsigned int position;

    sourceType = source->sourceType;
    if (sourceType == 0)
    {
        position = t3dFastFileTell_1002191D(source->file);
        return position;
    }
    if (sourceType != 1)
    {
        if (sourceType != 2)
            return 0xffffffff;
        position = (unsigned int)ftell_100693F7(source->file);
        return position;
    }
    position = pfn_SetFilePointer_guess(source->file, 0, 0, 1);
    return position;
}

// FUNCTION: EQGFX 0x1002629A
// __cdecl. Reads and clears the pending obsolete-object count.
int __cdecl s3dGetNumObsoleteObjects(void)
{
    int count = g_numObsoleteObjects_guess;
    g_numObsoleteObjects_guess &= 0;
    return count;
}

// FUNCTION: EQGFX 0x1002655d
// __cdecl (callee pops 0 bytes; callers do not set ECX). Returns the 4-byte value at +0x0C of the holder.
unsigned int __cdecl getValueAtOffset12_guess(int sourceObject)
{
    return *(unsigned int *)(sourceObject + 0x0c);
}
