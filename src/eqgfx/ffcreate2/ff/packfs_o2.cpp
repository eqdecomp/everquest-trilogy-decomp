// CFLAGS: /O2
#include "packfs_shared.h"

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10051e80
// Reads one block (packed size, unpacked size <= 0x2000, data), unpacks it to +8; returns unpacked size or 0.
unsigned int __fastcall readPackFileHeader_guess(PackFile_guess *file)
{
    unsigned int sourceSize;
    unsigned int decodedSize;
    char dataBuffer[0x3ffc];
    unsigned int bytesRead;
    long filePosition;
    int headerProcessed;

    EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))
        (file->fileHandle, &sourceSize, 4);
    EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))
        (file->fileHandle, &decodedSize, 4);
    if (PACK_FILE_HEADER_MAX_DECODED_SIZE < decodedSize)
        return 0;
    bytesRead = EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))
        (file->fileHandle, dataBuffer, sourceSize);
    filePosition = EQ_CDECL(long, 0x1006A833, (int))(file->fileHandle);
    file->filePosition = filePosition;
    if (bytesRead != sourceSize)
        return 0;
    headerProcessed = EQ_CDECL(int, 0x10054AC0, (void *, unsigned int *, void *, unsigned int))
        ((char *)file + 8, &decodedSize, dataBuffer, sourceSize);
    return ~-(unsigned int)(headerProcessed != 0) & decodedSize;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10051f30
// Runs the shared resource-system setup callback on first use.
// Cloned from trilogy FfSoundLock_guess::InitializeSoundCriticalSection_guess 0x004F8D50 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall (this in ECX); returns this. 0x100A3AD8 is the "not yet initialised" flag, 0x100FBBD0 the CRITICAL_SECTION.
/* fake match: the original's flag store is a write-only `mov byte ptr [0x100a3ad8], 0`. /O1 folds a zero store
   to `and [mem], 0`, so "t" (favor speed) is set for this function only to emit the mov. */
#pragma optimize("t", on)
void *__fastcall runInitializationCallbackOnce_guess(void *self)
{
    unsigned char flagCopy = EQ_GLOBAL(unsigned char, 0x100A3AD8);
    unsigned char &flag = flagCopy;
    if (flag != 0)
    {
        EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x100760A4)(EQ_ARRAY(char, 0x100FBBD0));
        EQ_GLOBAL(unsigned char, 0x100A3AD8) = 0;
    }
    return self;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10051f60
// __thiscall, one by-value file name (callee pops 8). Loads the PFS file once; returns true when it loaded it.
#pragma optimize("t", on)
bool PackFS::addFileSystem_guess(PackFsFileNameArgument_guess name)
{
    union {
        unsigned int key;
        int directoryOffset;
    } slot;
    PackFsTreeIterator_guess found;
    PackFsLookupResult_guess lookup;
    PackFsLookupResult_guess *lookupResult;
    PackFsQuad_guess entryKey;
    unsigned int nameKey;
    unsigned int keyTail;
    PackFsFileData_guess *fileData;
    PackFsFileData_guess *owner;
    PackFsMap_guess *fileMap;
    PackFsMap_guess *newMap;
    PackFsMap_guess *treeMap;
    PackFsSharedPtr_guess newFileMap;
    PackFsSharedPtr_guess descriptorCopy;
    PackFsSharedPointer2_guess *newDescriptor;
    PackFsSharedPointer2_guess *descriptor;
    PackFsSharedPointerData_guess *newEntryArray;
    PackFsSharedPointerData_guess *entryArray;
    PackFsFileEntry_guess *entries;
    PackFsFileEntry_guess *target;
    PackFsMapNode_guess *node;
    PackFsRefCounted_guess *global;
    void *entryBuffer;
    char *fileNamePointer;
    char compareFunction;
    char allocatorObject;
    bool loaded;
    unsigned int magic;
    unsigned int version;
    unsigned int entryCount;
    unsigned int dataBytes;
    unsigned int bytesRead;
    unsigned int entryIndex;
    int fileHandle;
    int logResult;
    int index;

    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x100760B4)(EQ_ARRAY(char, 0x100FBBD0));
    loaded = false;
    EQ_METHOD(0, void, 0x100526C0, (void *), (void *)&name)();
    slot.key = name.string;
    EQ_METHOD(2, void, 0x100534F0, (void *, PackFsTreeIterator_guess *, unsigned int *), (void *)0x100fbbe8)(&found, &slot.key);
    if (EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBBEC) == found.node)
    {
        fileData = (PackFsFileData_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(0x14);
        if (fileData != 0)
        {
            EQ_METHOD(0, void, 0x100526F0, (void *), (void *)fileData)();
            fileData->map.ptr = 0;
            fileData->unknown0xC.object = 0;
            fileData->unknown0x10.object = 0;
            newMap = (PackFsMap_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(0x10);
            fileMap = 0;
            if (newMap != 0)
                fileMap = EQ_METHOD(2, PackFsMap_guess *, 0x10052DD0, (PackFsMap_guess *, const char *, const char *), (void *)newMap)(&compareFunction, &allocatorObject);
            logResult = EQ_CDECL(int, 0x100524B0, (unsigned int, unsigned int))(0x100A3B68, 0x69);
            fileMap = (PackFsMap_guess *)EQ_CDECL(int, 0x10054890, (int, const char *, unsigned int))((int)fileMap, (const char *)0x100A3B58, logResult);
            EQ_FASTCALL(PackFsSharedPtr_guess *, 0x10053E00, (PackFsSharedPtr_guess *, int, PackFsMap_guess *))(&newFileMap, 0, fileMap);
            EQ_METHOD(1, void, 0x10054520, (void *, PackFsRefValue_guess), (void *)&fileData->map)(*(PackFsRefValue_guess *)&newFileMap);
        }
        if (g_packfs_log_count_guess < 0x40)
        {
            g_packfs_log_entries_guess[g_packfs_log_count_guess].first = 0x100A3B68;
            g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0xdd;
            g_packfs_log_count_guess++;
        }
        if (fileData == 0)
            EQ_CDECL(void, 0x100549A0, (int, const char *, const char *))(0, (const char *)0x10087020, (const char *)0x100A3B48);
        descriptor = 0;
        if (fileData != 0)
        {
            newDescriptor = (PackFsSharedPointer2_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(0xc);
            if (newDescriptor != 0)
                newDescriptor = EQ_METHOD(1, PackFsSharedPointer2_guess *, 0x100546E0, (void *, int), (void *)newDescriptor)((int)fileData);
            logResult = EQ_CDECL(int, 0x100524B0, (unsigned int, unsigned int))((unsigned int)EQGFX_PACKFS_SHARE_H_PATH, 0x102);
            descriptor = (PackFsSharedPointer2_guess *)EQ_CDECL(int, 0x10054890, (int, const char *, unsigned int))((int)newDescriptor, (const char *)0x10087060, logResult);
            descriptor->refCount++;
        }
        if (EQ_FASTCALL(unsigned int, 0x10021E78, (void *))((void *)&name) == 0)
            fileNamePointer = 0;
        else
            fileNamePointer = (char *)((PackFsStringSource_guess *)name.source)->first;
        fileHandle = EQ_CDECL(int, 0x1006AC3A, (char *, int))(fileNamePointer, 0x8000);
        if (fileHandle != -1)
        {
            EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))(fileHandle, &slot.directoryOffset, 4);
            EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))(fileHandle, &magic, 4);
            EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))(fileHandle, &version, 4);
            if (g_packfs_log_count_guess < 0x40)
            {
                g_packfs_log_entries_guess[g_packfs_log_count_guess].first = 0x100A3B68;
                g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0xea;
                g_packfs_log_count_guess++;
            }
            if (EQ_FASTCALL(unsigned int, 0x10021E78, (void *))((void *)&name) == 0)
                fileNamePointer = 0;
            else
                fileNamePointer = (char *)((PackFsStringSource_guess *)name.source)->first;
            EQ_CDECL(void, 0x100549A0, (int, const char *, char *))(
                (int)(((unsigned int)fileNamePointer & 0xFFFFFF00u) | (magic == EQ_GLOBAL(unsigned int, 0x100FBBF8) ? 1u : 0u)),
                (const char *)0x100A3B04, fileNamePointer);
            if (g_packfs_log_count_guess < 0x40)
            {
                g_packfs_log_entries_guess[g_packfs_log_count_guess].first = 0x100A3B68;
                g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0xee;
                g_packfs_log_count_guess++;
            }
            if (EQ_FASTCALL(unsigned int, 0x10021E78, (void *))((void *)&name) == 0)
                fileNamePointer = 0;
            else
                fileNamePointer = (char *)((PackFsStringSource_guess *)name.source)->first;
            EQ_CDECL(void, 0x100549A0, (int, const char *, char *))(
                (int)((EQ_GLOBAL(unsigned int, 0x100FBBF8) & 0xFFFFFF00u) | (version == 0x20000 ? 1u : 0u)),
                (const char *)0x100A3AE4, fileNamePointer);
            EQ_CDECL(int, 0x1006AB62, (int, long, int))(fileHandle, slot.directoryOffset, 0);
            EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))(fileHandle, &entryCount, 4);
            if (slot.directoryOffset != 0 && entryCount != 0)
            {
                dataBytes = entryCount * 12;
                entryBuffer = EQ_CDECL(void *, 0x10068C72, (unsigned int))(dataBytes);
                logResult = EQ_CDECL(int, 0x100524B0, (unsigned int, unsigned int))((unsigned int)EQGFX_PACKFS_SHARE_H_PATH, 0x1b0);
                entryBuffer = (void *)EQ_CDECL(int, 0x10054890, (int, const char *, unsigned int))((int)entryBuffer, (const char *)0x10087010, logResult);
                newEntryArray = (PackFsSharedPointerData_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(0x10);
                if (newEntryArray != 0)
                    newEntryArray = EQ_METHOD(2, PackFsSharedPointerData_guess *, 0x10054810, (PackFsSharedPointerData_guess *, int, int), (void *)newEntryArray)((int)entryBuffer, (int)entryCount);
                logResult = EQ_CDECL(int, 0x100524B0, (unsigned int, unsigned int))((unsigned int)EQGFX_PACKFS_SHARE_H_PATH, 0x1b2);
                entryArray = (PackFsSharedPointerData_guess *)EQ_CDECL(int, 0x10054890, (int, const char *, unsigned int))((int)newEntryArray, (const char *)0x10086ff8, logResult);
                entryArray->refCount++;
                dataBytes = entryCount * 12;
                bytesRead = EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))(fileHandle, (void *)entryArray->first, dataBytes);
                if (bytesRead == dataBytes)
                {
                    entries = (PackFsFileEntry_guess *)entryArray->first;
                    entryIndex = entryCount;
                    index = (int)entryCount - 1;
                    do
                    {
                        entryKey.d = keyTail;
                        entryKey.a = entries[index].first;
                        entryKey.b = (unsigned int)lookup.node;
                        entryKey.c = lookup.inserted;
                        treeMap = ((PackFsRefDerived_guess *)((PackFsFileData_guess *)descriptor->value)->map.ptr)->map;
                        lookupResult = EQ_METHOD(2, PackFsLookupResult_guess *, 0x10053560, (void *, PackFsLookupResult_guess *, unsigned int *), (void *)treeMap)(&lookup, &entryKey.a);
                        node = lookupResult->node;
                        target = (PackFsFileEntry_guess *)(node->value + 4);
                        *target = entries[index];
                        index--;
                        entryIndex--;
                    } while (entryIndex != 0);
                    owner = (PackFsFileData_guess *)descriptor->value;
                    EQ_METHOD(1, PackFsStringReference_guess *, 0x10021C94, (PackFsStringReference_guess *, const PackFsStringReference_guess *), (void *)owner)((const PackFsStringReference_guess *)&name);
                    global = EQ_GLOBAL(PackFsRefCounted_guess *, 0x100FBBFC);
                    if (global != 0)
                    {
                        global->refCount++;
                        EQ_METHOD(1, PackFsRefHandle_guess *, 0x100545D0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x100545C0, (void *), (void *)&descriptor)() + 0xC)(global);
                        if (descriptor != 0)
                            descriptor->refCount++;
                        EQ_METHOD(1, PackFsRefHandle_guess *, 0x100545D0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x100545C0, (void *), (void *)0x100fbbfc)() + 0x10)((PackFsRefCounted_guess *)descriptor);
                    }
                    if (descriptor != 0)
                        descriptor->refCount++;
                    EQ_METHOD(1, PackFsRefHandle_guess *, 0x100545D0, (void *, PackFsRefCounted_guess *), (void *)0x100fbbfc)((PackFsRefCounted_guess *)descriptor);
                    EQ_METHOD(0, void, 0x100526C0, (void *), (void *)&name)();
                    nameKey = name.string;
                    EQ_METHOD(1, PackFsSharedPtr_guess *, 0x10053DE0, (void *, const PackFsSharedPtr_guess *), (void *)&descriptorCopy)((const PackFsSharedPtr_guess *)&descriptor);
                    EQ_METHOD(2, void, 0x10052E90, (void *, unsigned int *, unsigned int *), (void *)0x100fbbe8)((unsigned int *)&lookup, &nameKey);
                    if (descriptorCopy.ptr != 0)
                        EQ_FASTCALL(void, 0x1002190D, (PackFsRefCounted_guess *))(descriptorCopy.ptr);
                    loaded = true;
                }
                if (entryArray != 0)
                    EQ_FASTCALL(void, 0x1002190D, (PackFsRefCounted_guess *))((PackFsRefCounted_guess *)entryArray);
            }
            EQ_CDECL(int, 0x1006AA82, (int))(fileHandle);
        }
        if (descriptor != 0)
            EQ_FASTCALL(void, 0x1002190D, (PackFsRefCounted_guess *))((PackFsRefCounted_guess *)descriptor);
    }
    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x100760B8)(EQ_ARRAY(char, 0x100FBBD0));
    return loaded;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100524b0
// Records a pack-file diagnostic event for later review.
// Cloned from trilogy logErrorMessage_guess 0x004F92D0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Appends the two values to the table while it has room; always returns 0.
int logErrorMessage_guess(unsigned int first, unsigned int second)
{
    if (g_packfs_log_count_guess < 0x40)
    {
        PackFsLogEntry_guess entry = { first, second };
        g_packfs_log_entries_guess[g_packfs_log_count_guess] = entry;
        g_packfs_log_count_guess++;
    }
    return 0;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100524e0
// Releases the optional object owned by a holder when its last reference ends.
// Cloned from trilogy decrementHeldReferenceCount_guess 0x004F9300 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args). Drops the holder's reference, deleting the object at zero.
// Named apart from the 0x1002190D decrementReferenceCount_guess so the test harness finds each by name.
void __fastcall decrementHeldReferenceCount_guess(PackFsRefHolder_guess *holder)
{
    if (holder->object != 0)
    {
        PackFsRefCounted_guess *object = holder->object;
        int remaining = object->refCount - 1;
        object->refCount = remaining;
        if (remaining == 0 && object != 0)
            object->destroy_guess(1);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10052500
// Unregisters a named packed archive and reconnects its neighboring archive references.
// Cloned from trilogy PackFS::removeFileSystem_guess 0x004F9320 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: temporary argument construction and inline EH states still differ.
void PackFS::removeFileSystem_guess(PackFsRemoveFileArgument_guess keyHolder)
{
    struct HandleStorage {
        union {
            PackFsRemoveValue_guess *value;
            int key;
        };
        PackFsRemoveTreeNode_guess *node;
        void *result;
    } handle;
    void *entryData;
    int refCount;
    PackFsRefCounted_guess *reference;
    PackFsRefCounted_guess *released;

    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x100760B4)(EQ_ARRAY(char, 0x100FBBD0));
    EQ_METHOD(0, void, 0x100526C0, (void *), (void *)&keyHolder)();
    handle.key = keyHolder.string;
    EQ_FASTCALL(void, 0x100534F0, (void *, PackFsRemoveTreeNode_guess **, PackFsRemoveTreeNode_guess **, int *))
        ((void *)0x100fbbe8, &handle.node, &handle.node, &handle.key);
    if (EQ_GLOBAL(PackFsRemoveTreeNode_guess *, 0x100FBBEC) != handle.node)
    {
        handle.value = handle.node->value;
        if (handle.value != 0)
        {
            // Preserve a separate count load instead of a memory increment.
            refCount = static_cast<volatile int &>(handle.value->refCount);
            handle.value->refCount = refCount + 1;
        }
        if (((PackFsRemoveChild_guess *)handle.value->child)->reference10 != 0)
        {
            entryData = EQ_METHOD(0, void *, 0x100545C0, (void *), (void *)&handle.value)();
            reference = *(PackFsRefCounted_guess **)((char *)entryData + 0xc);
            if (reference != 0)
                reference->refCount++;
            EQ_METHOD(1, void, 0x100545D0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x100545C0, (void *), 
                    (char *)EQ_METHOD(0, void *, 0x100545C0, (void *), &handle.value)() + 0x10)() + 0xc)(reference);
        }
        else
        {
            entryData = EQ_METHOD(0, void *, 0x100545C0, (void *), (void *)&handle.value)();
            reference = *(PackFsRefCounted_guess **)((char *)entryData + 0xc);
            if (reference != 0)
                reference->refCount++;
            EQ_METHOD(1, void, 0x100545D0, (void *, PackFsRefCounted_guess *), (void *)0x100fbbfc)(reference);
        }
        if (((PackFsRemoveChild_guess *)handle.value->child)->referenceC != 0)
        {
            entryData = EQ_METHOD(0, void *, 0x100545C0, (void *), (void *)&handle.value)();
            reference = *(PackFsRefCounted_guess **)((char *)entryData + 0x10);
            if (reference != 0)
                reference->refCount++;
            EQ_METHOD(1, void, 0x100545D0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x100545C0, (void *), 
                    (char *)EQ_METHOD(0, void *, 0x100545C0, (void *), &handle.value)() + 0xc)() + 0x10)(reference);
        }
        void *resultAddress = &handle.result;
        EQ_FASTCALL(void, 0x100530D0, (void *, void *, void *, PackFsRemoveTreeNode_guess *))
            ((void *)0x100fbbe8, resultAddress, resultAddress, handle.node);
        released = handle.value;
        if (released != 0 &&
            (refCount = released->refCount, released->refCount = refCount - 1, refCount - 1 == 0) &&
            released != 0)
            released->destroy_guess(1);
    }
    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x100760B8)(EQ_ARRAY(char, 0x100FBBD0));
    return;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100526c0
// Builds a deferred string when a pack-file key is first needed.
// Cloned from trilogy initializeStringIfEmpty_guess 0x004F94E0 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args). Makes the string from the source on first use.
/* fake match: repeat the zero test to retain mov/test; forward incoming EDX through EQ_THISCALL's dummy slot. */
void __fastcall initializeStringIfEmpty_guess(PackFsStringHolder_guess *holder, int incomingEDX)
{
    int temp;
    int first;
    int second;
    int string = holder->string;
    PackFsStringSource_guess *source;
    if (string == 0 && string == 0 && (source = holder->source) != 0)
    {
        second = source->second;
        first = source->first;
        holder->string = EQ_THISCALL(int, 0x10054B60, (int *, int, int, int))(
            &temp, incomingEDX, first, second);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100526f0
// Starts an empty pair of shared references for pack-file loading.
// Cloned from trilogy PackFsRefPair_guess::PackFsRefPair_guess 0x004F9510 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: Eight inline EH setup/teardown instructions remain absent; /O1 emits __EH_prolog when EH is retained.
// __thiscall constructor (this in ECX, no stack args): zeroes both handles, returns this.
PackFsRefPair_guess::PackFsRefPair_guess()
{
    // Volatile stores preserve the original's write-only movs instead of /O1's read-modify-write ands.
    static_cast<PackFsRefCounted_guess *volatile &>(first.object) = 0;
    static_cast<PackFsRefCounted_guess *volatile &>(second.object) = 0;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10052dd0
// Prepares the entry map used while loading a packed file.
// Cloned from trilogy PackFsMap_guess::init_guess 0x004F9EE0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, returns this (callee pops 8). Copies allocator/comparator, shares or creates the nil node
// under the lock, then buys the red head node pointing at itself.
PackFsMap_guess *PackFsMap_guess::init_guess(const char *compare_, const char *allocator_)
{
    PackFsMapNode_guess *spare;
    PackFsMapNode_guess *node;
    PackFsMapNode_guess *tmp;

    allocator = *allocator_;
    compare = *compare_;
    multi = 0;
    node = (PackFsMapNode_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(sizeof(PackFsMapNode_guess));
    spare = node;
    node->parent = 0;
    node->color = 1;
    EQ_METHOD(0, void, 0x10075A66, (const char **), &allocator_)();
    if (g_packfs_map_nil_guess == 0)
    {
        g_packfs_map_nil_guess = node;
        node->left = 0;
        spare = 0;
        g_packfs_map_nil_guess->right = 0;
    }
    g_packfs_map_nilrefs_guess = g_packfs_map_nilrefs_guess + 1;
    EQ_METHOD(0, void, 0x10075B02, (const char **), &allocator_)();
    if (spare != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(spare);
    tmp = g_packfs_map_nil_guess;
    node = (PackFsMapNode_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(sizeof(PackFsMapNode_guess));
    node->parent = tmp;
    node->color = 0;
    head = node;
    size = 0;
    head->left = head;
    head->right = head;
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10052e80
// Makes a stored map value available during resource cleanup.
// Cloned from trilogy FUN_004fb0f0 0x004FB0F0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 1 stack arg (RET 4, the callee pops it). Stores the dword that `target` points at into *out,
// then returns out (EAX). `target` is this+0x4.
unsigned int *PackFsValueHolder_guess::copyTargetValue_guess(unsigned int *out)
{
    *out = *target;
    return out;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10052E90
// Adds a key/value pair to an ordered index, accepting duplicate keys only when configured.
// Cloned from trilogy PackFsTree_guess::insert_guess 0x004F9F90 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: EBP frame and spills, integer comparison materialization, merged recoloring blocks, and shared epilogues remain.
void PackFsTree_guess::insert_guess(unsigned int *result, unsigned int *keyValue)
{
    PackFsTreeNode_guess *header = head;
    PackFsTreeNode_guess *parent = header;
    PackFsTreeNode_guess *node = header->parent;
    /* fake match: keep the value-field pointer distinct from the reused keyValue argument slot. */
    unsigned int *value = keyValue + 1;
    int left = 1;
    while (node != g_packfs_tree_nil_guess)
    {
        left = value[-1] < node->key;
        parent = node;
        if (left)
            node = node->left;
        else
            node = node->right;
    }

    if (*(char *)((char *)this + 8))
    {
        keyValue = (unsigned int *)EQ_METHOD(2, void *, 0x10053A40, (PackFsTree_guess *, PackFsTreeNode_guess *, int), this)(parent, 0);
        PackFsTreeNode_guess *inserted = (PackFsTreeNode_guess *)keyValue;
        inserted->left = g_packfs_tree_nil_guess;
        inserted->right = g_packfs_tree_nil_guess;
        EQ_CDECL(void, 0x10054670, (unsigned int *, unsigned int *))(keyValue + 3, value - 1);
        ++*(int *)((char *)this + 0x0C);
        if (parent == head || node != g_packfs_tree_nil_guess || value[-1] < parent->key)
        {
            parent->left = (PackFsTreeNode_guess *)keyValue;
            if (parent == head)
            {
                head->parent = (PackFsTreeNode_guess *)keyValue;
                head->right = (PackFsTreeNode_guess *)keyValue;
            }
            else if (parent == head->left)
                head->left = (PackFsTreeNode_guess *)keyValue;
        }
        else
        {
            parent->right = (PackFsTreeNode_guess *)keyValue;
            if (parent == head->right)
                head->right = (PackFsTreeNode_guess *)keyValue;
        }
        inserted = (PackFsTreeNode_guess *)keyValue;
        PackFsTreeNode_guess *current = inserted;
        while (current != head->parent)
        {
            PackFsTreeNode_guess *p = current->parent;
            if (((unsigned int *)p)[5] != 0)
                break;
            PackFsTreeNode_guess *uncle = p->parent->left;
            if (p == uncle)
            {
                uncle = p->parent->right;
                if (((unsigned int *)uncle)[5] == 0)
                {
                    ((unsigned int *)p)[5] = 1;
                    ((unsigned int *)uncle)[5] = 1;
                    ((unsigned int *)current->parent->parent)[5] = 0;
                    current = current->parent->parent;
                }
                else
                {
                    if (current == p->right)
                    {
                        current = p;
                        EQ_METHOD(1, void, 0x10053980, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current);
                    }
                    ((unsigned int *)current->parent)[5] = 1;
                    ((unsigned int *)current->parent->parent)[5] = 0;
                    EQ_METHOD(1, void, 0x100539E0, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current->parent->parent);
                }
            }
            else if (((unsigned int *)uncle)[5] == 0)
            {
                ((unsigned int *)p)[5] = 1;
                ((unsigned int *)uncle)[5] = 1;
                ((unsigned int *)current->parent->parent)[5] = 0;
                current = current->parent->parent;
            }
            else
            {
                if (current == p->left)
                {
                    current = p;
                    EQ_METHOD(1, void, 0x100539E0, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current);
                }
                ((unsigned int *)current->parent)[5] = 1;
                ((unsigned int *)current->parent->parent)[5] = 0;
                EQ_METHOD(1, void, 0x10053980, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current->parent->parent);
            }
        }
        ((unsigned int *)head->parent)[5] = 1;
        *result = (unsigned int)keyValue;
        *((unsigned char *)result + 4) = 1;
        return;
    }

    PackFsTreeNode_guess *candidate = parent;
    if (left)
    {
        if (parent == header->left)
        {
            unsigned int *answer = EQ_METHOD(4, unsigned int *, 0x10053710, (PackFsTree_guess *, unsigned int **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&keyValue, node, parent, value - 1);
            *result = *answer;
            *((unsigned char *)result + 4) = 1;
            return;
        }
        EQ_METHOD(0, void, 0x10053A60, (PackFsTreeNode_guess **), &candidate)();
    }
    if (candidate->key < value[-1])
    {
        unsigned int *answer = EQ_METHOD(4, unsigned int *, 0x10053710, (PackFsTree_guess *, unsigned int **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&keyValue, node, parent, value - 1);
        *result = *answer;
        *((unsigned char *)result + 4) = 1;
        return;
    }
    *result = (unsigned int)candidate;
    *((unsigned char *)result + 4) = 0;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100530d0
// Erases one entry from the PackFS tree of ref-counted objects and drops the entry's reference to its object
// Cloned from trilogy RedBlackTree_removeNodeFromRedBlackTree_guess 0x004FA1D0 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: EBP frame (fastcall with stack slots) shifts stack offsets and register choice; zero/one constant caching and spill slots differ.
// The empty _Lockit occupies the top local slot, four bytes below entry ESP.
void __fastcall RedBlackTree_removeNodeFromRedBlackTree_guess(
    PackFsTree_guess *tree, int edxDummy, int *resultAddress, int *node)
{
    int tmp;
    int *cursor;
    int *grandparent;
    int *erasedParent;
    int *siblingNode;
    int *tmpNode;
    int *fixupNode;
    int *removedNode;
    int *header;
    int *parent;
    int *sibling;
    int *erased;
    int lockStorage;
    PackFsRefCounted_guess *refObject;

    erased = node;
    EQ_METHOD(0, void, 0x10053D30, (int **), &node)();
    fixupNode = (int *)*erased;
    siblingNode = erased + 2;
    removedNode = erased;
    tmpNode = siblingNode;
    if (fixupNode == (int *)g_packfs_tree_nil_guess)
    {
        fixupNode = (int *)*siblingNode;
    }
    else
    {
        cursor = (int *)*siblingNode;
        if (cursor != (int *)g_packfs_tree_nil_guess)
        {
            removedNode = cursor;
            for (tmpNode = (int *)*cursor;
                 tmpNode != (int *)g_packfs_tree_nil_guess;
                 tmpNode = (int *)*tmpNode)
            {
                removedNode = tmpNode;
            }
            fixupNode = (int *)removedNode[2];
            tmpNode = removedNode + 2;
        }
    }
    EQ_METHOD(0, void, 0x10075A66, (void *), &lockStorage)();
    siblingNode = erased + 2;
    if (removedNode != erased)
    {
        *(int **)(*erased + 4) = removedNode;
        *removedNode = *erased;
        if (removedNode == (int *)*siblingNode)
        {
            fixupNode[1] = (int)removedNode;
        }
        else
        {
            fixupNode[1] = removedNode[1];
            *(int **)removedNode[1] = fixupNode;
            removedNode[2] = *siblingNode;
            *(int **)(*siblingNode + 4) = removedNode;
        }
        header = *(int **)((char *)tree + 4);
        if (header[1] == (int)erased)
        {
            header[1] = (int)removedNode;
        }
        else
        {
            erasedParent = (int *)erased[1];
            if ((int *)*erasedParent == erased)
                *erasedParent = (int)removedNode;
            else
                erasedParent[2] = (int)removedNode;
        }
        cursor = removedNode;
        removedNode = erased;
        cursor[1] = erased[1];
        tmp = cursor[5];
        cursor[5] = erased[5];
        erased[5] = tmp;
    }
    else
    {
        fixupNode[1] = removedNode[1];
        header = *(int **)((char *)tree + 4);
        if (header[1] == (int)erased)
        {
            header[1] = (int)fixupNode;
        }
        else
        {
            erasedParent = (int *)erased[1];
            if ((int *)*erasedParent == erased)
                *erasedParent = (int)fixupNode;
            else
                erasedParent[2] = (int)fixupNode;
        }
        tmpNode = *(int **)((char *)tree + 4);
        if ((int *)*tmpNode == erased)
        {
            if ((int *)*siblingNode == (int *)g_packfs_tree_nil_guess)
            {
                *tmpNode = erased[1];
            }
            else
            {
                cursor = (int *)*fixupNode;
                siblingNode = fixupNode;
                while (erasedParent = cursor,
                       erasedParent != (int *)g_packfs_tree_nil_guess)
                {
                    siblingNode = erasedParent;
                    cursor = (int *)*erasedParent;
                }
                *tmpNode = (int)siblingNode;
            }
        }
        if ((*(int **)((char *)tree + 4))[2] == (int)erased)
        {
            if ((int *)*erased == (int *)g_packfs_tree_nil_guess)
            {
                siblingNode = (int *)erased[1];
            }
            else
            {
                tmpNode = (int *)fixupNode[2];
                siblingNode = fixupNode;
                while (cursor = tmpNode,
                       cursor != (int *)g_packfs_tree_nil_guess)
                {
                    siblingNode = cursor;
                    tmpNode = (int *)cursor[2];
                }
            }
            (*(int **)((char *)tree + 4))[2] = (int)siblingNode;
        }
    }
    if (removedNode[5] == 1)
    {
        header = *(int **)((char *)tree + 4);
        if (fixupNode != (int *)header[1])
        {
            do
            {
                if (fixupNode[5] != 1)
                    break;
                siblingNode = *(int **)fixupNode[1];
                if (fixupNode == siblingNode)
                {
                    siblingNode = (int *)((int **)fixupNode[1])[2];
                    if (siblingNode[5] == 0)
                    {
                        siblingNode[5] = 1;
                        *(int *)(fixupNode[1] + 0x14) = 0;
                        tmp = fixupNode[1];
                        siblingNode = *(int **)(tmp + 8);
                        *(int *)(tmp + 8) = *siblingNode;
                        if ((int *)*siblingNode != (int *)g_packfs_tree_nil_guess)
                            ((int *)*siblingNode)[1] = tmp;
                        siblingNode[1] = *(int *)(tmp + 4);
                        if (tmp == header[1])
                        {
                            header[1] = (int)siblingNode;
                        }
                        else
                        {
                            tmpNode = *(int **)(tmp + 4);
                            if (tmp == *tmpNode)
                                *tmpNode = (int)siblingNode;
                            else
                                tmpNode[2] = (int)siblingNode;
                        }
                        *siblingNode = tmp;
                        *(int **)(tmp + 4) = siblingNode;
                        siblingNode = *(int **)(fixupNode[1] + 8);
                    }
                    if ((*(int *)(*siblingNode + 0x14) != 1) ||
                        (*(int *)(siblingNode[2] + 0x14) != 1))
                    {
                        if (*(int *)(siblingNode[2] + 0x14) == 1)
                        {
                            *(int *)(*siblingNode + 0x14) = 1;
                            tmp = *siblingNode;
                            siblingNode[5] = 0;
                            *siblingNode = *(int *)(tmp + 8);
                            if (*(int **)(tmp + 8) !=
                                (int *)g_packfs_tree_nil_guess)
                                (*(int **)(tmp + 8))[1] = (int)siblingNode;
                            *(int *)(tmp + 4) = siblingNode[1];
                            if (siblingNode == (int *)header[1])
                            {
                                header[1] = tmp;
                            }
                            else
                            {
                                tmpNode = (int *)siblingNode[1];
                                if (siblingNode == (int *)tmpNode[2])
                                    tmpNode[2] = tmp;
                                else
                                    *tmpNode = tmp;
                            }
                            *(int **)(tmp + 8) = siblingNode;
                            siblingNode[1] = tmp;
                            siblingNode = *(int **)(fixupNode[1] + 8);
                        }
                        siblingNode[5] = *(int *)(fixupNode[1] + 0x14);
                        *(int *)(fixupNode[1] + 0x14) = 1;
                        *(int *)(siblingNode[2] + 0x14) = 1;
                        parent = (int *)fixupNode[1];
                        sibling = (int *)parent[2];
                        parent[2] = *sibling;
                        if ((int *)*sibling !=
                            (int *)g_packfs_tree_nil_guess)
                            ((int *)*sibling)[1] = (int)parent;
                        sibling[1] = parent[1];
                        if (parent == (int *)header[1])
                        {
                            header[1] = (int)sibling;
                            *sibling = (int)parent;
                        }
                        else
                        {
                            siblingNode = (int *)parent[1];
                            if (parent == (int *)*siblingNode)
                            {
                                *siblingNode = (int)sibling;
                                *sibling = (int)parent;
                            }
                            else
                            {
                                siblingNode[2] = (int)sibling;
                                *sibling = (int)parent;
                            }
                        }
                        parent[1] = (int)sibling;
                        break;
                    }
                }
                else
                {
                    if (siblingNode[5] == 0)
                    {
                        siblingNode[5] = 1;
                        *(int *)(fixupNode[1] + 0x14) = 0;
                        siblingNode = (int *)fixupNode[1];
                        tmp = *siblingNode;
                        *siblingNode = *(int *)(tmp + 8);
                        if (*(int **)(tmp + 8) !=
                            (int *)g_packfs_tree_nil_guess)
                            (*(int **)(tmp + 8))[1] = (int)siblingNode;
                        *(int *)(tmp + 4) = siblingNode[1];
                        if (siblingNode == (int *)header[1])
                        {
                            header[1] = tmp;
                        }
                        else
                        {
                            tmpNode = (int *)siblingNode[1];
                            if (siblingNode == (int *)tmpNode[2])
                                tmpNode[2] = tmp;
                            else
                                *tmpNode = tmp;
                        }
                        *(int **)(tmp + 8) = siblingNode;
                        siblingNode[1] = tmp;
                        siblingNode = *(int **)fixupNode[1];
                    }
                    if ((*(int *)(siblingNode[2] + 0x14) != 1) ||
                        (*(int *)(*siblingNode + 0x14) != 1))
                    {
                        if (*(int *)(*siblingNode + 0x14) == 1)
                        {
                            *(int *)(siblingNode[2] + 0x14) = 1;
                            tmpNode = (int *)siblingNode[2];
                            siblingNode[5] = 0;
                            siblingNode[2] = *tmpNode;
                            if ((int *)*tmpNode !=
                                (int *)g_packfs_tree_nil_guess)
                                ((int *)*tmpNode)[1] = (int)siblingNode;
                            tmpNode[1] = siblingNode[1];
                            if (siblingNode == (int *)header[1])
                            {
                                header[1] = (int)tmpNode;
                            }
                            else
                            {
                                cursor = (int *)siblingNode[1];
                                if (siblingNode == (int *)*cursor)
                                    *cursor = (int)tmpNode;
                                else
                                    cursor[2] = (int)tmpNode;
                            }
                            *tmpNode = (int)siblingNode;
                            siblingNode[1] = (int)tmpNode;
                            siblingNode = *(int **)fixupNode[1];
                        }
                        siblingNode[5] = *(int *)(fixupNode[1] + 0x14);
                        *(int *)(fixupNode[1] + 0x14) = 1;
                        *(int *)(*siblingNode + 0x14) = 1;
                        parent = (int *)fixupNode[1];
                        sibling = (int *)*parent;
                        parent[0] = sibling[2];
                        if ((int *)sibling[2] !=
                            (int *)g_packfs_tree_nil_guess)
                            ((int *)sibling[2])[1] = (int)parent;
                        sibling[1] = parent[1];
                        if (parent == (int *)header[1])
                        {
                            header[1] = (int)sibling;
                        }
                        else
                        {
                            grandparent = (int *)parent[1];
                            if (parent == (int *)grandparent[2])
                                grandparent[2] = (int)sibling;
                            else
                                *grandparent = (int)sibling;
                        }
                        sibling[2] = (int)parent;
                        parent[1] = (int)sibling;
                        break;
                    }
                }
                siblingNode[5] = 0;
                fixupNode = (int *)fixupNode[1];
            } while (fixupNode != (int *)header[1]);
        }
        fixupNode[5] = 1;
    }
    EQ_METHOD(0, void, 0x10075B02, (void *), &lockStorage)();
    refObject = (PackFsRefCounted_guess *)removedNode[4];
    if (refObject != 0 && --refObject->refCount == 0 && refObject != 0)
        refObject->destroy_guess(1);
    EQ_CDECL(void, 0x10050CBB, (void *))(removedNode);
    *(int *)((char *)tree + 0x0c) =
        *(int *)((char *)tree + 0x0c) - 1;
    *resultAddress = (int)node;
    return;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100534f0
// Finds an entry with the requested key in the archive lookup index.
// Cloned from trilogy PackFsTree_guess::find_guess 0x004FA5F0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, iterator returned through a hidden pointer. The node holding the key, or the head node when absent.
// @early-stop: prologue register allocation, candidate stack spill, and duplicated return epilogues still differ.
PackFsTreeIterator_guess PackFsTree_guess::find_guess(const unsigned int &key)
{
    PackFsTreeNode_guess *x;
    PackFsTreeIterator_guess where;
    PackFsTreeIterator_guess result;

    where.node = head;
    x = head->parent;
    PackFsTreeNode_guess *nil = g_packfs_tree_nil_guess;
    if (x != nil)
    {
        do
        {
            if (x->key < key)
                x = x->right;
            else
            {
                where.node = x;
                x = x->left;
            }
        } while (x != nil);
    }
    if (where.node == head || key < where.node->key)
        result.node = head;
    else
        result.node = where.node;
    return result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053560
// Finds or inserts a key in a second ordered index; the indexed resource's meaning is unclear.
// Cloned from trilogy RedBlackTreeLookup_guess::insertOrFindNodeInRedBlackTree_guess 0x004FA660 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: EBP frame, key/header register allocation, comparison loads, merged helper calls, and shared epilogues remain.
RedBlackTreeFindResult_guess *RedBlackTreeLookup_guess::insertOrFindNodeInRedBlackTree_guess(RedBlackTreeFindResult_guess *result, unsigned int *key)
{
    PackFsTreeNode_guess *x;
    PackFsTreeNode_guess *y;
    bool less;

    const unsigned int *searchKey = key;
    /* fake match: the volatile reference releases the sentinel register and retains the original's loop-latch reload. */
    PackFsTreeNode_guess *volatile &nil = g_packfs_red_black_tree_nil_guess;
    y = head;
    x = y->parent;
    less = true;
    while (x != nil)
    {
        y = x;
        less = *searchKey < y->key;
        if (less)
            x = y->left;
        else
            x = y->right;
    }

    if (mode != 0)
    {
        PackFsTreeNode_guess *insertedNodeValue;
        result->node = *EQ_THISCALL(PackFsTreeNode_guess **, 0x10053AC0,
            (RedBlackTreeLookup_guess *, int, PackFsTreeNode_guess **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, const unsigned int *))
            (this, (int)head, &insertedNodeValue, x, y, searchKey);
        result->inserted = 1;
        return result;
    }

    // The saved key address remains available while the key parameter slot holds the iterator candidate.
    PackFsTreeNode_guess *&candidate = (PackFsTreeNode_guess *&)key;
    candidate = y;
    if (less)
    {
        if (y == head->left)
        {
            PackFsTreeNode_guess *insertedNodeValue;
            result->node = *EQ_THISCALL(PackFsTreeNode_guess **, 0x10053AC0,
                (RedBlackTreeLookup_guess *, int, PackFsTreeNode_guess **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, const unsigned int *))
                (this, (int)head, &insertedNodeValue, x, y, searchKey);
            result->inserted = 1;
            return result;
        }
        EQ_METHOD(0, void, 0x10053D80, (PackFsTreeNode_guess **), &candidate)();
    }

    if (candidate->key < *searchKey)
    {
        PackFsTreeNode_guess *insertedNodeValue;
        result->node = *EQ_THISCALL(PackFsTreeNode_guess **, 0x10053AC0,
            (RedBlackTreeLookup_guess *, int, PackFsTreeNode_guess **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, const unsigned int *))
            (this, (int)candidate->key, &insertedNodeValue, x, y, searchKey);
        result->inserted = 1;
        return result;
    }
    else
    {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053650
// Finds an entry with the requested key in the open-file lookup index.
// Cloned from trilogy PackFsTree2_guess::find_guess 0x004FA750 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, iterator returned through a hidden pointer. The node holding the key, or the head node when absent.
// @early-stop: prologue register allocation, candidate stack spill, and duplicated return epilogues still differ.
PackFsTreeIterator_guess PackFsTree2_guess::find_guess(const unsigned int &key)
{
    PackFsTreeNode_guess *x;
    PackFsTreeNode_guess *y;
    PackFsTreeIterator_guess where;
    PackFsTreeIterator_guess result;

    y = head;
    x = y->parent;
    PackFsTreeNode_guess *nil = g_packfs_tree2_nil_guess;
    while (x != nil)
    {
        if (x->key < key)
            x = x->right;
        else
        {
            y = x;
            x = x->left;
        }
    }
    where.node = y;
    if (where.node != head && !(key < where.node->key))
        return where;
    result.node = head;
    return result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053710
// Cloned from trilogy PackFsTreeInsert_guess::insert_guess 0x004FA810 (the same original code: BinDiff sibling).
PackFsTreeIterator_guess *PackFsTreeInsert_guess::insert_guess(PackFsTreeIterator_guess *result, PackFsTreeNode_guess *searchNode, PackFsTreeNode_guess *parent, unsigned int *key)
{
    PackFsTreeNode_guess *node;
    PackFsTreeNode_guess *cursor;
    PackFsTreeNode_guess *parentOfCursor;
    PackFsTreeNode_guess *grandparent;
    PackFsTreeNode_guess *uncle;
    PackFsTreeNode_guess *pivot;
    PackFsTreeNode_guess *ancestor;

    node = (PackFsTreeNode_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(sizeof(PackFsTreeNode_guess));
    node->parent = parent;
    PACKFS_TREE_COLOR(node) = PACKFS_TREE_RED;
    node->left = g_packfs_tree_nil_guess;
    node->right = g_packfs_tree_nil_guess;
    EQ_CDECL(void, 0x10054670, (unsigned int *, unsigned int *))(&node->key, key);
    size++;
    if (parent == head || searchNode != g_packfs_tree_nil_guess || *key < parent->key)
    {
        parent->left = node;
        cursor = head;
        if (parent == cursor)
        {
            cursor->parent = node;
            head->right = node;
        }
        else if (parent == cursor->left)
            cursor->left = node;
    }
    else
    {
        parent->right = node;
        cursor = head;
        if (parent == cursor->right)
            cursor->right = node;
    }

    cursor = node;
    if (node != head->parent)
    {
        do
        {
            parentOfCursor = cursor->parent;
            if (PACKFS_TREE_COLOR(parentOfCursor) != PACKFS_TREE_RED)
                break;
            grandparent = parentOfCursor->parent;
            if (parentOfCursor == grandparent->left)
            {
                uncle = grandparent->right;
                if (PACKFS_TREE_COLOR(uncle) == PACKFS_TREE_RED)
                {
                    PACKFS_TREE_COLOR(parentOfCursor) = PACKFS_TREE_BLACK;
                    PACKFS_TREE_COLOR(uncle) = PACKFS_TREE_BLACK;
                    PACKFS_TREE_COLOR(grandparent) = PACKFS_TREE_RED;
                    cursor = grandparent;
                }
                else
                {
                    if (cursor == parentOfCursor->right)
                    {
                        cursor = parentOfCursor;
                        pivot = cursor->right;
                        cursor->right = pivot->left;
                        if (pivot->left != g_packfs_tree_nil_guess)
                            pivot->left->parent = cursor;
                        pivot->parent = cursor->parent;
                        if (cursor == head->parent)
                            head->parent = pivot;
                        else
                        {
                            grandparent = cursor->parent;
                            if (cursor == grandparent->left)
                                grandparent->left = pivot;
                            else
                                grandparent->right = pivot;
                        }
                        pivot->left = cursor;
                        cursor->parent = pivot;
                    }
                    PACKFS_TREE_COLOR(cursor->parent) = PACKFS_TREE_BLACK;
                    PACKFS_TREE_COLOR(cursor->parent->parent) = PACKFS_TREE_RED;
                    grandparent = cursor->parent->parent;
                    pivot = grandparent->left;
                    grandparent->left = pivot->right;
                    if (pivot->right != g_packfs_tree_nil_guess)
                        pivot->right->parent = grandparent;
                    pivot->parent = grandparent->parent;
                    if (grandparent == head->parent)
                    {
                        head->parent = pivot;
                        pivot->right = grandparent;
                    }
                    else
                    {
                        ancestor = grandparent->parent;
                        if (grandparent == ancestor->right)
                            ancestor->right = pivot;
                        else
                            ancestor->left = pivot;
                        pivot->right = grandparent;
                    }
                    grandparent->parent = pivot;
                }
            }
            else
            {
                uncle = grandparent->left;
                if (PACKFS_TREE_COLOR(uncle) == PACKFS_TREE_RED)
                {
                    PACKFS_TREE_COLOR(parentOfCursor) = PACKFS_TREE_BLACK;
                    PACKFS_TREE_COLOR(uncle) = PACKFS_TREE_BLACK;
                    PACKFS_TREE_COLOR(grandparent) = PACKFS_TREE_RED;
                    cursor = grandparent;
                }
                else
                {
                    if (cursor == parentOfCursor->left)
                    {
                        cursor = parentOfCursor;
                        pivot = cursor->left;
                        cursor->left = pivot->right;
                        if (pivot->right != g_packfs_tree_nil_guess)
                            pivot->right->parent = cursor;
                        pivot->parent = cursor->parent;
                        if (cursor == head->parent)
                            head->parent = pivot;
                        else
                        {
                            grandparent = cursor->parent;
                            if (cursor == grandparent->right)
                                grandparent->right = pivot;
                            else
                                grandparent->left = pivot;
                        }
                        pivot->right = cursor;
                        cursor->parent = pivot;
                    }
                    PACKFS_TREE_COLOR(cursor->parent) = PACKFS_TREE_BLACK;
                    PACKFS_TREE_COLOR(cursor->parent->parent) = PACKFS_TREE_RED;
                    grandparent = cursor->parent->parent;
                    pivot = grandparent->right;
                    grandparent->right = pivot->left;
                    if (pivot->left != g_packfs_tree_nil_guess)
                        pivot->left->parent = grandparent;
                    pivot->parent = grandparent->parent;
                    if (grandparent == head->parent)
                        head->parent = pivot;
                    else
                    {
                        ancestor = grandparent->parent;
                        if (grandparent == ancestor->left)
                            ancestor->left = pivot;
                        else
                            ancestor->right = pivot;
                    }
                    pivot->left = grandparent;
                    grandparent->parent = pivot;
                }
            }
        } while (cursor != head->parent);
    }
    PACKFS_TREE_COLOR(head->parent) = PACKFS_TREE_BLACK;
    result->node = node;
    return result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053980
// Balances insertion cases that need a leftward rotation in the key index.
void PackFsTree_guess::rotateLeft_guess(PackFsTreeNode_guess *node)
{
    PackFsTreeNode_guess *right;
    PackFsTreeNode_guess *parent;

    right = node->right;
    node->right = right->left;
    if (right->left != g_packfs_tree_nil_guess)
        right->left->parent = node;
    right->parent = node->parent;
    if (node == head->parent)
    {
        head->parent = right;
        right->left = node;
        node->parent = right;
        return;
    }
    parent = node->parent;
    if (node == parent->left)
    {
        parent->left = right;
        right->left = node;
        node->parent = right;
        return;
    }
    parent->right = right;
    right->left = node;
    node->parent = right;
    return;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100539e0
// Balances insertion cases that need a rightward rotation in the key index.
void PackFsTree_guess::rotateRight_guess(PackFsTreeNode_guess *node)
{
    PackFsTreeNode_guess *left;
    PackFsTreeNode_guess *parent;

    left = node->left;
    node->left = left->right;
    if (left->right != g_packfs_tree_nil_guess)
        left->right->parent = node;
    left->parent = node->parent;
    if (node == head->parent)
    {
        head->parent = left;
        left->right = node;
        node->parent = left;
        return;
    }
    parent = node->parent;
    if (node == parent->right)
    {
        parent->right = left;
        left->right = node;
        node->parent = left;
        return;
    }
    parent->left = left;
    left->right = node;
    node->parent = left;
    return;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053a40
// Creates an index entry for a newly registered resource key.
// Cloned from trilogy PackFsTreeAllocator_guess::buyNode_guess 0x004FAB40 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall (ignores this), 2 stack args. Allocates a 0x18-byte node and sets its parent and color.
PackFsTreeNode_guess *PackFsTreeAllocator_guess::buyNode_guess(PackFsTreeNode_guess *parent, int color)
{
    PackFsTreeNode_guess *node;

    node = (PackFsTreeNode_guess *)operator new(sizeof(PackFsTreeNode_guess));
    node->parent = parent;
    node->color_int_guess = color;
    return node;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053a60
// Finds the preceding entry while the primary key index is being updated.
void __fastcall decrementTreeIterator_guess(PackFsTreeNode_guess **cursor)
{
    PackFsTreeNode_guess *node = *cursor;
    PackFsTreeNode_guess *left;
    PackFsTreeNode_guess *right;
    PackFsTreeNode_guess *parent;
    /* fake match: retain the original cursor-slot reloads in the parent climb. */
    PackFsTreeNode_guess * volatile *iterator = cursor;

    if (node->color_int_guess == 0 && node->parent->parent == node)
    {
        *cursor = node->right;
        return;
    }
    left = node->left;
    if (left != g_packfs_tree_nil_guess)
    {
        right = left->right;
        while (right != g_packfs_tree_nil_guess)
        {
            left = right;
            right = left->right;
        }
        *cursor = left;
        return;
    }
    parent = node->parent;
    if (*iterator != parent->left)
    {
        *iterator = parent;
        return;
    }
    do
    {
        *iterator = parent;
        parent = parent->parent;
    } while (*iterator == parent->left);
    *iterator = parent;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053AC0
// Cloned from trilogy FUN_004fabc0 0x004FABC0 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: Frame/register allocation, constant stores, and inner-rotation load scheduling still differ.
int ** __fastcall insertNodeInRedBlackTree_guess(void *tree, int dummy, int **insertedNodeOut, int x, int *y, unsigned int *searchKey)
{
    int uncleAddress;
    int *ancestor;
    int *node;
    int *current;
    int *parent;
    int *sibling;

    node = (int *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(0x20);
    node[1] = (int)y;
    node[7] = 0;
    node[0] = (int)EQ_GLOBAL(int *, 0x100FBC08);
    node[2] = (int)EQ_GLOBAL(int *, 0x100FBC08);
    EQ_CDECL(void, 0x10054690, (unsigned int *, const unsigned int *))
        ((unsigned int *)(node + 3), searchKey);
    *(int *)((char *)tree + 0xC) = *(int *)((char *)tree + 0xC) + 1;
    if ((y == *(int **)((char *)tree + 4)) ||
        (x != (int)EQ_GLOBAL(int *, 0x100FBC08)) ||
        (*searchKey < (unsigned int)y[3]))
    {
        *y = (int)node;
        current = *(int **)((char *)tree + 4);
        if (y == current)
        {
            current[1] = (int)node;
            *(int **)(*(int *)((char *)tree + 4) + 8) = node;
        }
        else if (y == (int *)*current)
        {
            *current = (int)node;
        }
    }
    else
    {
        y[2] = (int)node;
        if (y == *(int **)(*(int *)((char *)tree + 4) + 8))
        {
            *(int **)(*(int *)((char *)tree + 4) + 8) = node;
        }
    }
    current = node;
    while (current != *(int **)(*(int *)((char *)tree + 4) + 4) &&
           ((int *)current[1])[7] == 0)
    {
        parent = (int *)current[1];
        sibling = *(int **)parent[1];
        if (parent == sibling)
        {
            uncleAddress = ((int *)parent[1])[2];
            if (*(int *)(uncleAddress + 0x1C) == 0)
            {
                parent[7] = 1;
                *(int *)(uncleAddress + 0x1C) = 1;
                *(int *)(*(int *)(current[1] + 4) + 0x1C) = 0;
                current = *(int **)(current[1] + 4);
            }
            else
            {
                if (current == (int *)parent[2])
                {
                    current = parent;
                    parent = (int *)current[2];
                    current[2] = *parent;
                    if (*parent != (int)EQ_GLOBAL(int *, 0x100FBC08))
                        *(int **)(*parent + 4) = current;
                    parent[1] = current[1];
                    if (current == *(int **)(*(int *)((char *)tree + 4) + 4))
                    {
                        *(int **)(*(int *)((char *)tree + 4) + 4) = parent;
                    }
                    else
                    {
                        sibling = (int *)current[1];
                        if (current == (int *)*sibling)
                            *sibling = (int)parent;
                        else
                            sibling[2] = (int)parent;
                    }
                    *parent = (int)current;
                    current[1] = (int)parent;
                }
                *(int *)(current[1] + 0x1C) = 1;
                *(int *)(*(int *)(current[1] + 4) + 0x1C) = 0;
                parent = *(int **)(current[1] + 4);
                sibling = (int *)*parent;
                *parent = sibling[2];
                if (sibling[2] != (int)EQ_GLOBAL(int *, 0x100FBC08))
                    *(int **)(sibling[2] + 4) = parent;
                sibling[1] = parent[1];
                if (parent == *(int **)(*(int *)((char *)tree + 4) + 4))
                {
                    *(int **)(*(int *)((char *)tree + 4) + 4) = sibling;
                    sibling[2] = (int)parent;
                }
                else
                {
                    ancestor = (int *)parent[1];
                    if (parent == (int *)ancestor[2])
                    {
                        ancestor[2] = (int)sibling;
                        sibling[2] = (int)parent;
                    }
                    else
                    {
                        *ancestor = (int)sibling;
                        sibling[2] = (int)parent;
                    }
                }
                parent[1] = (int)sibling;
            }
        }
        else
        {
            if (sibling[7] == 0)
            {
                parent[7] = 1;
                sibling[7] = 1;
                *(int *)(*(int *)(current[1] + 4) + 0x1C) = 0;
                current = *(int **)(current[1] + 4);
            }
            else
            {
                if (current == (int *)*parent)
                {
                    current = parent;
                    parent = (int *)*current;
                    *current = parent[2];
                    if (parent[2] != (int)EQ_GLOBAL(int *, 0x100FBC08))
                        *(int **)(parent[2] + 4) = current;
                    parent[1] = current[1];
                    if (current == *(int **)(*(int *)((char *)tree + 4) + 4))
                    {
                        *(int **)(*(int *)((char *)tree + 4) + 4) = parent;
                    }
                    else
                    {
                        sibling = (int *)current[1];
                        if (current == (int *)sibling[2])
                            sibling[2] = (int)parent;
                        else
                            *sibling = (int)parent;
                    }
                    parent[2] = (int)current;
                    current[1] = (int)parent;
                }
                *(int *)(current[1] + 0x1C) = 1;
                *(int *)(*(int *)(current[1] + 4) + 0x1C) = 0;
                parent = *(int **)(current[1] + 4);
                sibling = (int *)parent[2];
                parent[2] = *sibling;
                if (*sibling != (int)EQ_GLOBAL(int *, 0x100FBC08))
                    *(int **)(*sibling + 4) = parent;
                sibling[1] = parent[1];
                if (parent == *(int **)(*(int *)((char *)tree + 4) + 4))
                {
                    *(int **)(*(int *)((char *)tree + 4) + 4) = sibling;
                }
                else
                {
                    ancestor = (int *)parent[1];
                    if (parent == (int *)*ancestor)
                        *ancestor = (int)sibling;
                    else
                        ancestor[2] = (int)sibling;
                }
                *sibling = (int)parent;
                parent[1] = (int)sibling;
            }
        }
    }
    *(int *)(*(int *)(*(int *)((char *)tree + 4) + 4) + 0x1C) = 1;
    *insertedNodeOut = node;
    return insertedNodeOut;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053d30
// Advances the archive-key cursor to the next entry.
// Cloned from trilogy PackFsTreeIterator_guess::increment_guess 0x004FAE30 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: min-loop copy propagation and parent-climb compare loads still differ.
// __thiscall, no stack args. Advances the iterator to the in-order successor (STL _Tree iterator _Inc):
// the leftmost node of the right subtree, else the first ancestor reached from a left child.
void PackFsTreeIterator_guess::increment_guess()
{
    PackFsTreeNode_guess *current = node;
    PackFsTreeNode_guess *right = current->right;
    PackFsTreeNode_guess *left;
    PackFsTreeNode_guess *parent;
    /* fake match: retain the cursor reloads in the original parent climb. */
    PackFsTreeNode_guess * volatile *iterator = &node;
    if (right != g_packfs_tree_nil_guess)
    {
        left = right->left;
        while (left != g_packfs_tree_nil_guess)
        {
            right = left;
            left = right->left;
        }
        node = right;
        return;
    }
    parent = current->parent;
    if (*iterator == parent->right)
    {
        do
        {
            *iterator = parent;
            parent = parent->parent;
        } while (*iterator == parent->right);
    }
    if ((*iterator)->right != parent)
        *iterator = parent;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053d80
// Finds the preceding entry in the secondary resource lookup index.
// Cloned from trilogy PackFsTree2Iterator_guess::decrement_guess 0x004FAE80 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, no stack args. Moves the iterator to the in-order predecessor (STL _Tree iterator _Dec):
// from the head (red, its parent's parent is itself) to the maximum at its right, else the rightmost
// node of the left subtree, else the first ancestor reached from a right child.
// @early-stop: fast-path epilogue order and the parent-climb load still differ.
void PackFsTree2Iterator_guess::decrement_guess()
{
    PackFsTree2Node_guess *left;
    PackFsTree2Node_guess *right;
    PackFsTree2Node_guess *parent;
    PackFsTree2Node_guess * volatile *iterator = &node;

    if (node->color == 0 && node->parent->parent == node)
    {
        node = node->right;
        return;
    }
    left = node->left;
    if (left != g_packfs_tree2_nil_guess)
    {
        right = left->right;
        while (right != g_packfs_tree2_nil_guess)
        {
            left = right;
            right = left->right;
        }
        node = left;
        return;
    }
    parent = node->parent;
    if (*iterator != parent->left)
    {
        *iterator = parent;
        return;
    }
    do
    {
        *iterator = parent;
        parent = parent->parent;
    } while (*iterator == parent->left);
    *iterator = parent;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053de0
// Keeps a copied descriptor alive for another pack-file owner.
// Cloned from trilogy PackFsSharedPtr_guess::incrementReferenceCount_guess 0x004FB6B0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 1 stack arg (a pointer to the source shared pointer). Copies the pointer and takes a
// reference on the object; leaves this in EAX.
PackFsSharedPtr_guess *PackFsSharedPtr_guess::incrementReferenceCount_guess(const PackFsSharedPtr_guess *other)
{
    ptr = other->ptr;
    if (ptr != 0)
        ptr->refCount++;
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053e00
// __thiscall written as __fastcall with an unused EDX slot so `this` arrives in ECX (callee pops 4). Returns self.
PackFsSharedPtr_guess *__fastcall SharedPointer_SharedPointer_guess(PackFsSharedPtr_guess *newFileMap, int /* EDX unused */, PackFsMap_guess *fileMap)
{
    PackFsRefDerived_guess *node;
    unsigned int logIndex;

    newFileMap->ptr = 0;
    if (fileMap != 0)
    {
        node = (PackFsRefDerived_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(sizeof(PackFsRefDerived_guess));
        if (node != 0)
        {
            node->map = fileMap;
            node->refCount = 0;
            node->vtbl = EQGFX_PACKFS_DERIVED_VTABLE;
        }
        else
        {
            node = 0;
        }
        logIndex = g_packfs_log_count_guess;
        if (logIndex < 0x40)
        {
            g_packfs_log_entries_guess[logIndex].first = EQGFX_PACKFS_SHARE_H_PATH;
            g_packfs_log_entries_guess[logIndex].second = 0x102;
            g_packfs_log_count_guess = logIndex + 1;
        }
        if (node == 0)
            EQ_CDECL(void, 0x100549A0, (void *, const char *, const char *))(node, (const char *)0x10087020, (const char *)0x10087060);
        newFileMap->ptr = (PackFsRefCounted_guess *)node;
        node->refCount = node->refCount + 1;
    }
    return newFileMap;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053e90
// Cleans up a derived archive-map resource and its allocated wrapper.
// Cloned from trilogy destroyRefDerivedAndMaybeFree_guess 0x004FAF70 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall (callee pops flags), written as __fastcall with an unused EDX slot so `this` arrives in ECX. Scalar
// deleting destructor of PackFsRefDerived_guess: runs the destructor at 0x10053EB0, frees the object with the cdecl
// operator delete at 0x10050CBB when bit 0 of flags is set, returns this.
PackFsRefDerived_guess *__fastcall destroyRefDerivedAndMaybeFree_guess(PackFsRefDerived_guess *self, int /* EDX unused */, unsigned char flags)
{
    EQ_METHOD(0, void, 0x10053EB0, (void *), self)();
    if (flags & 1)
        EQ_CDECL(void, 0x10050CBB, (void *))(self);
    return self;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10053EB0
// Destroys a shared file-map object: frees its std::map, then the inlined base destructor drops the reference.
PackFsRefDerived_guess::~PackFsRefDerived_guess()
{
    PackFsMap_guess *map;
    PackFsMapNode_guess *firstNode;
    PackFsMapNode_guess *headNode;
    PackFsMapNode_guess *nextNode;
    PackFsMapNode_guess *headResult;
    PackFsMapNode_guess *iteratorResult;
    PackFsMapNode_guess *eraseResult;
    PackFsMapNode_guess **iteratorNode;
    PackFsMapNode_guess *releaseNode;
    int count;
    bool isDifferent;

    {
        vtbl = EQGFX_PACKFS_DERIVED_VTABLE;
        map = this->map;
        if (map != 0)
        {
            headNode = map->head;
            count = map->size;
            firstNode = headNode->left;
            if (count != 0
                && (isDifferent = firstNode != *EQ_METHOD(1, PackFsMapNode_guess **,
                    EQGFX_PACKFS_GET_LIST_HEAD_DATA,
                    (PackFsMap_guess *, PackFsMapNode_guess **), map)(&headResult)) == 0
                && headNode == map->head)
            {
                EQ_METHOD(1, void, EQGFX_PACKFS_FREE_NODE_AND_CHILDREN,
                    (PackFsMap_guess *, PackFsMapNode_guess *), map)(map->head->parent);
                map->head->parent = g_packfs_map_nil_guess;
                map->size = 0;
                map->head->left = map->head;
                map->head->right = map->head;
                EQ_METHOD(1, PackFsMapNode_guess **, EQGFX_PACKFS_GET_LIST_HEAD_DATA,
                    (PackFsMap_guess *, PackFsMapNode_guess **), map)(&nextNode);
            }
            else
            {
                if (firstNode != headNode)
                {
                    do
                    {
                        iteratorNode = EQ_METHOD(2, PackFsMapNode_guess **,
                            EQGFX_PACKFS_MAP_ITERATOR_POST_INCREMENT,
                            (PackFsMapNode_guess **, PackFsMapNode_guess **, int), &firstNode)
                            (&iteratorResult, 0);
                        EQ_METHOD(2, void, EQGFX_PACKFS_REMOVE_NODE_FROM_RED_BLACK_TREE,
                            (PackFsMap_guess *, PackFsMapNode_guess **, PackFsMapNode_guess *), map)
                            (&eraseResult, *iteratorNode);
                    } while (firstNode != headNode);
                }
                nextNode = firstNode;
            }
            EQ_CDECL_CPP(void, EQGFX_PACKFS_FREE_MEMORY, (void *))(map->head);
            map->head = 0;
            map->size = 0;
            {
                releaseNode = 0;
                PackFsLockit_guess lock;
                unsigned int nilRefCount = g_packfs_map_nilrefs_guess;
                --nilRefCount;
                g_packfs_map_nilrefs_guess = nilRefCount;

                if (nilRefCount == 0)
                {
                    releaseNode = g_packfs_map_nil_guess;
                    g_packfs_map_nil_guess = 0;
                }
            }
            if (releaseNode != 0)
                EQ_CDECL_CPP(void, EQGFX_PACKFS_FREE_MEMORY, (void *))(releaseNode);
            EQ_CDECL_CPP(void, EQGFX_PACKFS_FREE_MEMORY, (void *))(map);
        }
    }

}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)


#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10054410
// Moves a red-black tree iterator to its in-order successor (map iterator ++); used by the tree node erase.
// Cloned from trilogy FUN_004fb500 0x004FB500 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (`self` in ECX), no stack args, callee pops 0 bytes, no return value. Moves the iterator
// *self to the in-order successor of its node: the leftmost node of the right subtree when there is one,
// else the first ancestor reached from a left child. The nil node is loaded once, as in the original.
void __fastcall PackFsTreeIterator_increment_guess(PackFsTreeNode_guess **self)
{
    PackFsTreeNode_guess *nilNode;
    PackFsTreeNode_guess *right;
    PackFsTreeNode_guess *left;
    PackFsTreeNode_guess *up;

    nilNode = EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08);
    right = (*self)->right;
    if (right != nilNode)
    {
        for (left = right->left; left != nilNode; left = left->left)
            right = left;
        *self = right;
        return;
    }
    up = (*self)->parent;
    if (*self == up->right)
    {
        do
        {
            *self = up;
            up = up->parent;
        } while (*self == up->right);
    }
    if ((*self)->right != up)
        *self = up;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10054460
// Releases all entries owned by a resource map.
// __thiscall (this passed on, unused), 1 stack arg, callee pops 4, no return value. Frees the map subtree at node: the right subtree, then the node, then the left child, until the shared nil node.
void PackFsMap_guess::freeNodeAndChildren_guess(PackFsMapNode_guess *node)
{
    PackFsMapNode_guess *y;

    if ((y = node) != g_packfs_map_nil_guess)
    {
        do
        {
            freeNodeAndChildren_guess(y->right);
            y = y->left;
            EQ_CDECL(void, 0x10050CBB, (void *))(node);
            node = y;
        } while (y != g_packfs_map_nil_guess);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100544a0
// Preserves the current map entry for cleanup while moving to the next.
// Cloned from trilogy FUN_004fb590 0x004FB590 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: VC6 still merges the exits and loop backedges, adds an iterator reload, and allocates traversal registers differently.
// __thiscall (iterator in ECX), 2 stack args: the hidden return buffer (first) and an unused int (the postfix
// dummy), callee pops 8. Postfix ++ on a map iterator: stores the current node in the buffer, moves the iterator
// to its in-order successor (leftmost node of the right subtree, else the nearest ancestor whose left subtree
// holds the node), and returns the buffer pointer in EAX.
PackFsMapIterator_guess *PackFsMapIterator_guess::postIncrement_guess(PackFsMapIterator_guess *result, int)
{
    PackFsMapNode_guess *nil = g_packfs_map_nil_guess;
    PackFsMapIterator_guess saved;
    PackFsMapNode_guess *y;

    /* fake match: preserve a distinct iterator snapshot load before traversal. */
    saved.node = *(PackFsMapNode_guess *volatile *)&node;
    y = node->right;
    if (y != nil)
    {
        while (y->left != nil)
            y = y->left;
        node = y;
        *result = saved;
        return result;
    }
    y = node->parent;
    while (node == y->right)
    {
        node = y;
        y = y->parent;
    }
    if (node->right != y)
        node = y;
    *result = saved;
    return result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10054500
// Drops one reference to a shared resource, destroying it when no references remain.
// Cloned from trilogy decrementReferenceCount_guess 0x004FB5F0 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: VC6 drops the original's second null test on obj (already known non-null); no source shape kept it.
// __fastcall (the shared pointer in ECX, no stack args, no return value; callee pops 0). Drops one reference on
// the object the shared pointer holds. When the count reaches zero, calls its first virtual function with 1
// (the deleting destructor). The shared pointer itself is not cleared. Null objects are skipped.
// Named apart from 0x1002190d (decrementReferenceCount_guess): the same name with another parameter type is
// an overload, and the test DLL can't resolve it by name.
void __fastcall decrementSharedReferenceCount_guess(PackFsSharedPtr_guess *shared)
{
    PackFsRefCounted_guess *obj;
    int count;

    obj = shared->ptr;
    if (obj != 0)
    {
        // Compare the local, not `count - 1` again: VC6 then reuses the flags of the dec and adds no test.
        // The second null test is redundant (obj was tested above) and is in the original; VC6 drops it.
        count = obj->refCount - 1;
        obj->refCount = count;
        if (count == 0 && obj != 0)
            obj->destroy_guess(1);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10054520
// Replaces a shared resource pointer while balancing ownership of the old and new objects.
// __thiscall, 1 stack arg (the shared pointer by value; its PackFsRefValue_guess destructor releases the copy on return).
// "t" (favor speed) is set for this function only: the original writes its EH prolog out and expands the destructor inline.
#pragma optimize("t", on)
PackFsSharedPtr_guess &PackFsSharedPtr_guess::updateSharedPointer_guess(PackFsRefValue_guess other)
{
    if ((void *)&other == (void *)this)
        return *this;
    if (ptr != 0)
    {
        PackFsRefCounted_guess *oldObject = ptr;
        if (--oldObject->refCount == 0 && oldObject != 0)
            oldObject->destroy_guess(1);
    }
    ptr = other.ptr;
    if (other.ptr != 0)
        other.ptr->refCount++;
    return *this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("t", on)
// FUNCTION: EQGFX 0x100545d0
// Rebinds a resource handle to another object while balancing shared ownership.
// __thiscall ABI: the unused EDX parameter lets a free function use ECX plus one stack argument in VC6.
// Assigns the shared pointer `value` to `*holder`: releases the old object (deleting it at zero) and takes a
// reference on the new one. When `value` is the holder itself (self-assignment) only the copy's destructor runs.
// Returns holder.
PackFsRefValue_guess * __fastcall updateReferenceCount_guess(
    PackFsRefValue_guess *holder, void * /* EDX unused */, PackFsRefValue_guess value)
{
    if (&value == holder)
        return holder;

    if (holder->ptr != 0)
    {
        PackFsRefCounted_guess *oldValue = holder->ptr;
        if (--oldValue->refCount == 0 && oldValue != 0)
            oldValue->destroy_guess(1);
    }
    holder->ptr = value.ptr;
    if (value.ptr != 0)
        value.ptr->refCount++;
    return holder;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100545c0
// Provides the payload pointer stored in a resource handle.
// Cloned from trilogy getObjectDataPointer_guess 0x004FB6D0 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (the handle in ECX, no stack args): returns the data pointer at 0x8 of the object it points at.
void * __fastcall getObjectDataPointer_guess(PackFsHandle_guess *handle)
{
    return handle->object->data;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)


#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10054670
// Copies a key and its resource reference for a packed-file entry.
// Cloned from trilogy copyKeyValue_guess 0x004FB780 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl, 2 stack args. Copies the key and value into `dest` (nothing when it is null) and takes a
// reference on the value.
void copyKeyValue_guess(PackFsKeyValue_guess *dest, const PackFsKeyValue_guess *src)
{
    if (dest != 0)
    {
        dest->key = src->key;
        dest->value = src->value;
        if (dest->value != 0)
            dest->value->refCount++;
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10054690
// Copies a four-word tree record; its file-index meaning is unclear.
// Cloned from trilogy copyKeyValueQuad_guess 0x004FB7A0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl, 2 stack args. Copies four dwords into `dest` (nothing when it is null). Named apart from the
// 0x10054670 copyKeyValue_guess so the test harness finds each by name.
void copyKeyValueQuad_guess(PackFsQuad_guess *dest, const PackFsQuad_guess *src)
{
    if (dest != 0)
    {
        dest->a = src->a;
        dest->b = src->b;
        dest->c = src->c;
        dest->d = src->d;
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100546c0
// Scalar deleting destructor: runs the destructor body 0x100524E0, frees this when bit 0 of flags is set.
// Cloned from trilogy SuiteDefaultsElement_guess::ScalarDeletingDtor_guess 0x004FB7D0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 1 stack arg. Scalar deleting destructor: releases the held reference, frees the object
// when bit 0 of flags is set, returns this.
SuiteDefaultsElement_guess *SuiteDefaultsElement_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x100524E0, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10050CBB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x100546e0
// Starts a shared wrapper around one opaque value.
// Cloned from trilogy PackFsSharedPointer2_guess::PackFsSharedPointer2_guess 0x004FB7F0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall constructor, 1 stack arg. Starts with a zero count, stores the value and sets the vtable.
/* fake match: the original's count store is a write-only `mov dword ptr [this+4], 0`. /O1 folds a zero store to
   `and [mem], 0`, so "t" (favor speed) is set for this function only to emit the mov. */
#pragma optimize("t", on)
PackFsSharedPointer2_guess::PackFsSharedPointer2_guess(int value_arg)
{
    refCount = 0;
    value = value_arg;
    vtable = (const void *)0x10076420;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("t", on)
// FUNCTION: EQGFX 0x10054700
// Destroys the one-value shared wrapper and frees it when deletion is requested.
// Cloned from trilogy PackFsSharedPointer2_guess::ScalarDeletingDtor_guess 0x004FB810 (the same original code: BinDiff sibling, names/siblings/).
PackFsSharedPointer2_guess *PackFsSharedPointer2_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x10054720, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10050CBB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("t", on)
// FUNCTION: EQGFX 0x10054720
// Destroys a shared storage bundle, releasing its four held objects; their resource roles are unclear.
// __thiscall destructor, no stack args. Favor speed (ADD ESP,4 after the free). Releases the storage's four references and frees it, then drops the count and calls slot 0 with 1 at zero.
#pragma optimize("t", on)
PackFsSharedPointer2_guess::~PackFsSharedPointer2_guess()
{
    PackFsSharedPointerStorage_guess *storage;
    int count;

    {
        /* fake match: an empty destructible local builds the EH frame; its members fill the two frame slots. */
        PackFsEHMarker_guess marker;

        marker.self = this;
        /* fake match: volatile store, as in DestroyResourceAndDecrementRefCount_guess (0x10054850). */
        *(const void * volatile *)this = (const void *)0x10076420;
        storage = (PackFsSharedPointerStorage_guess *)value;
        marker.storage = storage;
        if (storage != 0)
        {
            PackFsRefCounted_guess *held = storage->third;
            if (held != 0 && --held->refCount == 0 && held != 0)
                held->destroy_guess(1);

            held = storage->second;
            if (held != 0 && --held->refCount == 0 && held != 0)
                held->destroy_guess(1);

            held = storage->first;
            if (held != 0 && --held->refCount == 0 && held != 0)
                held->destroy_guess(1);

            if (storage->base != 0)
                EQ_METHOD(0, void, EQGFX_PACKFSREFHANDLE_DECREMENT, (PackFsRefCounted_guess *), storage->base)();

            EQ_CDECL(void, 0x10050CBB, (void *))(storage);
        }
        count = refCount;
    }
    count--;
    *(const void **)this = (const void *)0x10076410;
    refCount = count;
    if (count == 0 && this != 0)
    {
        void **slots;

        /* fake match: the vtable is read back volatile, so the slot call loads it from memory as the original does. */
        slots = *(void ** volatile *)this;
        ((void (__fastcall *)(void *, void **, int))slots[0])(this, slots, 1);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("t", on)
// FUNCTION: EQGFX 0x10054810
// Initializes shared storage for two values used by the filesystem map.
// Cloned from trilogy PackFsSharedPointerData_guess::initializeSharedPointerWithData_guess 0x004FB920 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 2 stack args. Starts with a zero count, stores the two values and sets the vtable. Returns
// this (EAX = ECX in the original).
PackFsSharedPointerData_guess *PackFsSharedPointerData_guess::initializeSharedPointerWithData_guess(int first_arg, int second_arg)
{
    refCount = 0;
    first = first_arg;
    second = second_arg;
    vtable = (const void *)0x10076428;
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("t", on)
// FUNCTION: EQGFX 0x10054830
// Runs resource cleanup and frees the wrapper when deletion is requested.
// Cloned from trilogy ResourceObject_DestroyAndFreeIfRequested_destructor_guess 0x004FB940 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 1 stack arg (callee pops 4). Runs DestroyResourceAndDecrementRefCount_guess (0x10054850) on
// this, then frees this with the client's cdecl operator delete when bit 0 of flags is set. Returns this
// (EAX = ESI in the original).
ResourceObject_guess *ResourceObject_guess::ResourceObject_DestroyAndFreeIfRequested_destructor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x10054850, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10050CBB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("t", on)
// FUNCTION: EQGFX 0x10054850
// Releases a resource payload and shared reference, destroying the object at zero.
// __fastcall (this in ECX, no stack args, callee pops 0). Points the vtable at 0x10076428, frees the data at 0x8, points it at
// 0x10076410 and stores refCount - 1. When that is zero and the pointer is not null, calls slot 0 of the vtable with 1.
void __fastcall DestroyResourceAndDecrementRefCount_guess(PackFsResourceObject_guess *resource)
{
    void *data;
    int refCount;

    // Volatile accesses pin the order: data load, vtable store, free call.
    data = static_cast<void *volatile &>(resource->data);
    *(const void * volatile *)resource = (const void *)0x10076428;
    EQ_CDECL(void, 0x10050CBB, (void *))(data);
    refCount = resource->refCount;
    refCount--;
    *(const void **)resource = (const void *)0x10076410;
    resource->refCount = refCount;
    if (refCount == 0 && resource != 0)
    {
        void **vtable;

        // The vtable is read back volatile: /O1 would fold it into the 0x10076410 just stored. The slot call
        // passes it in EDX, where the original keeps it for call [edx]; the callee is thiscall and ignores EDX.
        vtable = *(void ** volatile *)resource;
        ((void (__fastcall *)(void *, void **, int))vtable[0])(resource, vtable, 1);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("t", on)
// FUNCTION: EQGFX 0x10054890
// Reports an allocation failure when a packed-file allocation returns null.
// Cloned from trilogy validateNonNullPointer_guess 0x004FB9A0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl (callee pops 0). Returns allocatedPointer. When allocatedPointer is 0, first calls _verify_guess (0x100549A0, cdecl,
// caller pops 0xC) with allocatedPointer, the "..." string and logResult.
int validateNonNullPointer_guess(int allocatedPointer, int logResult)
{
    if (allocatedPointer == 0)
        EQ_CDECL(void, 0x100549A0, (int, const char *, int))(allocatedPointer, (const char *)0x10087020, logResult);
    return allocatedPointer;
}

