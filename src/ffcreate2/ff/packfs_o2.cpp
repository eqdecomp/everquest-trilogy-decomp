// CFLAGS: /O2
#include "packfs_shared.h"

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f8a60
// Clears a global value; its game-facing purpose is unclear.
// Calling convention unknown: no args, no return value, callee pops 0 bytes.
// Zeroes the global at 0x006F0394.
void reset_trace_message_buffer_guess(void)
{
    g_trace_message_buffer_guess = 0;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f8a70
// Registers the client-exit callback at 0x004F8A80; its effect is unclear.
// __cdecl, no args, no return value. Registers the handler at 0x004F8A80 with
// _atexit (0x00523AB6); the original pops that argument with POP ECX.
void register_exit_handler_guess(void)
{
    EQ_CDECL(int, 0x00523AB6, (void (*)(void)))((void (*)(void))0x004F8A80);
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f8ac0
// Static initializer: constructs the empty pack-fs registry tree at 0x006F0380.
// __cdecl, no args, no return value. Calls FlagContainer_initializeFlagContainer_guess
// (0x004F9E30, __thiscall: this = 0x006F0380 in ECX; the callee pops its two pointer
// args, &firstValue (ESP+3) first and &secondValue (ESP+2) second, so the original has no ADD ESP).
// The 4-byte frame is the one PUSH ECX reserves; POP ECX releases it.
void GetFlagValues_guess(void)
{
    unsigned char secondValue;
    unsigned char firstValue;

    EQ_METHOD(2, void, 0x004F9E30, (void *, unsigned char *, unsigned char *), (void *)0x006f0380)(&firstValue, &secondValue);
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f8ae0
// Registers a separate client-exit callback at 0x004F8AF0; its effect is unclear.
// __cdecl, no args, no return value. Registers the handler at 0x004F8AF0 with
// _atexit (0x00523AB6); the original pops that argument with POP ECX.
void register_exit_handler_2_guess(void)
{
    EQ_CDECL(int, 0x00523AB6, (void (*)(void)))((void (*)(void))0x004F8AF0);
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f8c80
// Loads one compressed pack-file block so its resource data can be read.
// Reads a packed block (sizes, then data) and inflates it
// into the buffer; returns the unpacked size, or 0 on a short read or a failed inflate.
unsigned int PackFsReader_guess::readAndInflatePackBlock_guess(void)
{
    int packedSize;
    unsigned int unpackedSize;
    char packed[0x4000];
    int bytesRead;

    EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(this->fd, &packedSize, 4);
    EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(this->fd, &unpackedSize, 4);
    if (unpackedSize > PACKFS_MAX_UNPACKED)
        return 0;
    bytesRead = EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(
        this->fd, packed, packedSize);
    this->position = EQ_CDECL(int, 0x0052525F, (int))(this->fd);
    if (bytesRead != packedSize)
        return 0;
    if (EQ_CDECL(int, 0x004FC990, (void *, unsigned int *, void *, int))(
            this->buffer, &unpackedSize, packed, packedSize) != 0)
        return 0;
    return unpackedSize;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f8d30
// __fastcall (this in ECX, no stack args). Fills the buffer from the file descriptor.
int __fastcall readFileBuffer_guess(PackFsReader_guess *reader)
{
    return EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(
        reader->fd, reader->buffer, 0x2000);
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f8d80
// Mounts a PFS archive by name: checks magic and version, indexes its directory and links it into the pack list
// Cloned from eqgfx PackFS::addFileSystem 0x10051F60 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, one by-value file name (callee pops 8). Loads the PFS file once; returns true when it loaded it.
#pragma optimize("t", on)
bool PackFS::addFileSystem(PackFsFileNameArgument_guess name)
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

    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x00534194)(EQ_ARRAY(char, 0x006F0368));
    loaded = false;
    EQ_METHOD(0, void, 0x004F94E0, (void *), (void *)&name)();
    slot.key = name.string;
    EQ_METHOD(2, void, 0x004FA5F0, (void *, PackFsTreeIterator_guess *, unsigned int *), (void *)0x006f0380)(&found, &slot.key);
    if (EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F0384) == found.node)
    {
        fileData = (PackFsFileData_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x14);
        if (fileData != 0)
        {
            EQ_METHOD(0, void, 0x004F9510, (void *), (void *)fileData)();
            fileData->map.ptr = 0;
            fileData->unknown0xC.object = 0;
            fileData->unknown0x10.object = 0;
            newMap = (PackFsMap_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x10);
            fileMap = 0;
            if (newMap != 0)
                fileMap = EQ_METHOD(2, PackFsMap_guess *, 0x004F9EE0, (PackFsMap_guess *, const char *, const char *), (void *)newMap)(&compareFunction, &allocatorObject);
            logResult = EQ_CDECL(int, 0x004F92D0, (unsigned int, unsigned int))(0x00580E58, 0x69);
            fileMap = (PackFsMap_guess *)EQ_CDECL(int, 0x004FB9A0, (int, const char *, unsigned int))((int)fileMap, (const char *)0x00580E48, logResult);
            EQ_FASTCALL(PackFsSharedPtr_guess *, 0x004FAEE0, (PackFsSharedPtr_guess *, int, PackFsMap_guess *))(&newFileMap, 0, fileMap);
            EQ_METHOD(1, void, 0x004FB610, (void *, PackFsRefValue_guess), (void *)&fileData->map)(*(PackFsRefValue_guess *)&newFileMap);
        }
        if (g_packfs_log_count_guess < 0x40)
        {
            g_packfs_log_entries_guess[g_packfs_log_count_guess].first = 0x00580E58;
            g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0xdd;
            g_packfs_log_count_guess++;
        }
        if (fileData == 0)
            EQ_CDECL(void, 0x004F8310, (int, const char *, const char *))(0, (const char *)0x00570968, (const char *)0x00580E38);
        descriptor = 0;
        if (fileData != 0)
        {
            newDescriptor = (PackFsSharedPointer2_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0xc);
            if (newDescriptor != 0)
                newDescriptor = EQ_METHOD(1, PackFsSharedPointer2_guess *, 0x004FB7F0, (void *, int), (void *)newDescriptor)((int)fileData);
            logResult = EQ_CDECL(int, 0x004F92D0, (unsigned int, unsigned int))((unsigned int)PACKFS_SHARE_H_PATH_GUESS, 0x102);
            descriptor = (PackFsSharedPointer2_guess *)EQ_CDECL(int, 0x004FB9A0, (int, const char *, unsigned int))((int)newDescriptor, (const char *)0x00580DFC, logResult);
            descriptor->refCount++;
        }
        if (EQ_FASTCALL(unsigned int, 0x004DD993, (void *))((void *)&name) == 0)
            fileNamePointer = 0;
        else
            fileNamePointer = (char *)((PackFsStringSource_guess *)name.source)->first;
        fileHandle = EQ_CDECL(int, 0x00525666, (char *, int))(fileNamePointer, 0x8000);
        if (fileHandle != -1)
        {
            EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(fileHandle, &slot.directoryOffset, 4);
            EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(fileHandle, &magic, 4);
            EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(fileHandle, &version, 4);
            if (g_packfs_log_count_guess < 0x40)
            {
                g_packfs_log_entries_guess[g_packfs_log_count_guess].first = 0x00580E58;
                g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0xea;
                g_packfs_log_count_guess++;
            }
            if (EQ_FASTCALL(unsigned int, 0x004DD993, (void *))((void *)&name) == 0)
                fileNamePointer = 0;
            else
                fileNamePointer = (char *)((PackFsStringSource_guess *)name.source)->first;
            EQ_CDECL(void, 0x004F8310, (int, const char *, char *))(
                (int)(((unsigned int)fileNamePointer & 0xFFFFFF00u) | (magic == EQ_GLOBAL(unsigned int, 0x006F0390) ? 1u : 0u)),
                (const char *)0x00580DDC, fileNamePointer);
            if (g_packfs_log_count_guess < 0x40)
            {
                g_packfs_log_entries_guess[g_packfs_log_count_guess].first = 0x00580E58;
                g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0xee;
                g_packfs_log_count_guess++;
            }
            if (EQ_FASTCALL(unsigned int, 0x004DD993, (void *))((void *)&name) == 0)
                fileNamePointer = 0;
            else
                fileNamePointer = (char *)((PackFsStringSource_guess *)name.source)->first;
            EQ_CDECL(void, 0x004F8310, (int, const char *, char *))(
                (int)((EQ_GLOBAL(unsigned int, 0x006F0390) & 0xFFFFFF00u) | (version == 0x20000 ? 1u : 0u)),
                (const char *)0x00580DBC, fileNamePointer);
            EQ_CDECL(int, 0x0052558E, (int, long, int))(fileHandle, slot.directoryOffset, 0);
            EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(fileHandle, &entryCount, 4);
            if (slot.directoryOffset != 0 && entryCount != 0)
            {
                dataBytes = entryCount * 12;
                entryBuffer = EQ_CDECL(void *, 0x005218C6, (unsigned int))(dataBytes);
                logResult = EQ_CDECL(int, 0x004F92D0, (unsigned int, unsigned int))((unsigned int)PACKFS_SHARE_H_PATH_GUESS, 0x1b0);
                entryBuffer = (void *)EQ_CDECL(int, 0x004FB9A0, (int, const char *, unsigned int))((int)entryBuffer, (const char *)0x00570958, logResult);
                newEntryArray = (PackFsSharedPointerData_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x10);
                if (newEntryArray != 0)
                    newEntryArray = EQ_METHOD(2, PackFsSharedPointerData_guess *, 0x004FB920, (PackFsSharedPointerData_guess *, int, int), (void *)newEntryArray)((int)entryBuffer, (int)entryCount);
                logResult = EQ_CDECL(int, 0x004F92D0, (unsigned int, unsigned int))((unsigned int)PACKFS_SHARE_H_PATH_GUESS, 0x1b2);
                entryArray = (PackFsSharedPointerData_guess *)EQ_CDECL(int, 0x004FB9A0, (int, const char *, unsigned int))((int)newEntryArray, (const char *)0x00570940, logResult);
                entryArray->refCount++;
                dataBytes = entryCount * 12;
                bytesRead = EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(fileHandle, (void *)entryArray->first, dataBytes);
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
                        lookupResult = EQ_METHOD(2, PackFsLookupResult_guess *, 0x004FA660, (void *, PackFsLookupResult_guess *, unsigned int *), (void *)treeMap)(&lookup, &entryKey.a);
                        node = lookupResult->node;
                        target = (PackFsFileEntry_guess *)(node->value + 4);
                        *target = entries[index];
                        index--;
                        entryIndex--;
                    } while (entryIndex != 0);
                    owner = (PackFsFileData_guess *)descriptor->value;
                    EQ_METHOD(1, PackFsStringReference_guess *, 0x004C489A, (PackFsStringReference_guess *, const PackFsStringReference_guess *), (void *)owner)((const PackFsStringReference_guess *)&name);
                    global = EQ_GLOBAL(PackFsRefCounted_guess *, 0x006F0394);
                    if (global != 0)
                    {
                        global->refCount++;
                        EQ_METHOD(1, PackFsRefHandle_guess *, 0x004FB6E0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x004FB6D0, (void *), (void *)&descriptor)() + 0xC)(global);
                        if (descriptor != 0)
                            descriptor->refCount++;
                        EQ_METHOD(1, PackFsRefHandle_guess *, 0x004FB6E0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x004FB6D0, (void *), (void *)0x006f0394)() + 0x10)((PackFsRefCounted_guess *)descriptor);
                    }
                    if (descriptor != 0)
                        descriptor->refCount++;
                    EQ_METHOD(1, PackFsRefHandle_guess *, 0x004FB6E0, (void *, PackFsRefCounted_guess *), (void *)0x006f0394)((PackFsRefCounted_guess *)descriptor);
                    EQ_METHOD(0, void, 0x004F94E0, (void *), (void *)&name)();
                    nameKey = name.string;
                    EQ_METHOD(1, PackFsSharedPtr_guess *, 0x004FB6B0, (void *, const PackFsSharedPtr_guess *), (void *)&descriptorCopy)((const PackFsSharedPtr_guess *)&descriptor);
                    EQ_METHOD(2, void, 0x004F9F90, (void *, unsigned int *, unsigned int *), (void *)0x006f0380)((unsigned int *)&lookup, &nameKey);
                    if (descriptorCopy.ptr != 0)
                        EQ_FASTCALL(void, 0x004C4813, (PackFsRefCounted_guess *))(descriptorCopy.ptr);
                    loaded = true;
                }
                if (entryArray != 0)
                    EQ_FASTCALL(void, 0x004C4813, (PackFsRefCounted_guess *))((PackFsRefCounted_guess *)entryArray);
            }
            EQ_CDECL(int, 0x005254AE, (int))(fileHandle);
        }
        if (descriptor != 0)
            EQ_FASTCALL(void, 0x004C4813, (PackFsRefCounted_guess *))((PackFsRefCounted_guess *)descriptor);
    }
    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x00534190)(EQ_ARRAY(char, 0x006F0368));
    return loaded;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f92d0
// __cdecl. Appends the two values to the table while it has room; always returns 0.
int logErrorMessage_guess(unsigned int sourcePath, unsigned int second)
{
    if (g_packfs_log_count_guess < 0x40)
    {
        PackFsLogEntry_guess entry = { sourcePath, second };
        g_packfs_log_entries_guess[g_packfs_log_count_guess] = entry;
        g_packfs_log_count_guess++;
    }
    return 0;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f9300
// Releases a shared object stored in a resource holder.
// __fastcall (this in ECX, no stack args). Drops the holder's reference, deleting the object at zero.
// Named apart from the 0x004C4813 decrementReferenceCount_guess so the test harness finds each by name.
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
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f9320
// Removes a pack-file registration and releases its shared references.
// ORIGINAL FILE: ffcreate2/ff/packfs.cpp (from Mac debug map)
// @early-stop: temporary argument construction and inline EH states still differ.
void PackFS::removeFileSystem(PackFsRemoveFileArgument_guess request)
{
    struct HandleStorage {
        union {
            PackFsRemoveValue_guess *value;
            int key;
        };
        PackFsRemoveTreeNode_guess *node;
        void *result;
    } handle;
    void *dataPointer;
    int refCount;
    PackFsRefCounted_guess *reference;
    PackFsRefCounted_guess *released;

    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x00534194)(EQ_ARRAY(char, 0x006F0368));
    EQ_METHOD(0, void, 0x004F94E0, (void *), (void *)&request)();
    handle.key = request.string;
    EQ_FASTCALL(void, 0x004FA5F0, (void *, PackFsRemoveTreeNode_guess **, PackFsRemoveTreeNode_guess **, int *))
        ((void *)0x006f0380, &handle.node, &handle.node, &handle.key);
    if (EQ_GLOBAL(PackFsRemoveTreeNode_guess *, 0x006F0384) != handle.node)
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
            dataPointer = EQ_METHOD(0, void *, 0x004FB6D0, (void *), (void *)&handle.value)();
            reference = *(PackFsRefCounted_guess **)((char *)dataPointer + 0xc);
            if (reference != 0)
                reference->refCount++;
            EQ_METHOD(1, void, 0x004FB6E0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x004FB6D0, (void *), 
                    (char *)EQ_METHOD(0, void *, 0x004FB6D0, (void *), &handle.value)() + 0x10)() + 0xc)(reference);
        }
        else
        {
            dataPointer = EQ_METHOD(0, void *, 0x004FB6D0, (void *), (void *)&handle.value)();
            reference = *(PackFsRefCounted_guess **)((char *)dataPointer + 0xc);
            if (reference != 0)
                reference->refCount++;
            EQ_METHOD(1, void, 0x004FB6E0, (void *, PackFsRefCounted_guess *), (void *)0x006f0394)(reference);
        }
        if (((PackFsRemoveChild_guess *)handle.value->child)->referenceC != 0)
        {
            dataPointer = EQ_METHOD(0, void *, 0x004FB6D0, (void *), (void *)&handle.value)();
            reference = *(PackFsRefCounted_guess **)((char *)dataPointer + 0x10);
            if (reference != 0)
                reference->refCount++;
            EQ_METHOD(1, void, 0x004FB6E0, (void *, PackFsRefCounted_guess *), (char *)EQ_METHOD(0, void *, 0x004FB6D0, (void *), 
                    (char *)EQ_METHOD(0, void *, 0x004FB6D0, (void *), &handle.value)() + 0xc)() + 0x10)(reference);
        }
        void *resultAddress = &handle.result;
        EQ_FASTCALL(void, 0x004FA1D0, (void *, void *, void *, PackFsRemoveTreeNode_guess *))
            ((void *)0x006f0380, resultAddress, resultAddress, handle.node);
        released = handle.value;
        if (released != 0 &&
            (refCount = released->refCount, released->refCount = refCount - 1, refCount - 1 == 0) &&
            released != 0)
            released->destroy_guess(1);
    }
    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x00534190)(EQ_ARRAY(char, 0x006F0368));
    return;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f94e0
// __fastcall (holder in ECX; incoming EDX is forwarded; no stack args). Makes the string on first use.
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
        holder->string = EQ_THISCALL(int, 0x004FCA30, (int *, int, int, int))(
            &temp, incomingEDX, first, second);
    }
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f9510
// @early-stop: Eight inline EH setup/teardown instructions remain absent; /O1 emits __EH_prolog when EH is retained.
// __thiscall constructor (this in ECX, no stack args): zeroes both handles, returns this.
PackFsRefPair_guess::PackFsRefPair_guess()
{
    // Volatile stores preserve the original's write-only movs instead of /O1's read-modify-write ands.
    static_cast<PackFsRefCounted_guess *volatile &>(first.object) = 0;
    static_cast<PackFsRefCounted_guess *volatile &>(second.object) = 0;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004f99d0
// Opens a named game resource from disk or a loaded pack file.
// __thiscall ignoring this (callee pops 12). Opens the name as a file, else finds it in the loaded pack files
// (moving the one that has it to the front); true and *result set when the reader's slot 0 returns above 0.
bool __fastcall OpenPackFileData_guess(void *self, void *unusedEDX, PackFsNameRef_guess value, PackFileData_guess **result)
{
    PackFileData_guess *data;
    PackFsNameText_guess *text;
    PackFsEntryNode_guess *found;
    unsigned int key;
    int fd;

    data = 0;
    fd = EQ_CDECL(int, 0x00525666, (const char *, int))(
        EQ_FASTCALL(unsigned int, 0x004DD993, (PackFsNameRef_guess *))(&value) > 0 ? value.handle.text->text : 0,
        PACKFS_O_BINARY);
    if (fd == -1)
    {
        PackFsFileSystemPtr_guess fileSystem(g_packfs_file_systems);
        PackFsFileSystemPtr_guess previous;

        while (fileSystem.holder != 0 && data == 0)
        {
            EQ_METHOD(0, void, 0x004F94E0, (PackFsNameRef_guess *), &value)();
            key = value.key;
            EQ_METHOD(2, void, 0x004FA750, (PackFsEntryTree_guess *, PackFsEntryNode_guess **, const unsigned int *),
                fileSystem.holder->fileSystem->map->tree)(&found, &key);
            if (fileSystem.holder->fileSystem->map->tree->head != found)
            {
                data = new PackFileDataArchive_guess(found->offset);
                if (g_packfs_log_count_guess < 0x40)
                {
                    g_packfs_log_entries_guess[g_packfs_log_count_guess].first = PACKFS_CPP_PATH;
                    g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0x3e9;
                    g_packfs_log_count_guess++;
                }
                if (data == 0)
                    EQ_CDECL(void, 0x004F8310, (int, const char *, const char *))(0, (const char *)0x00570968, (const char *)0x00580F6C);
                text = fileSystem.holder->fileSystem->name.handle.text;
                data->fd = EQ_CDECL(int, 0x00525666, (const char *, int))(
                    text != 0 && text->length > 0 ? text->text : 0, PACKFS_O_BINARY);
                data->size = found->size;
                EQ_CDECL(long, 0x0052558E, (int, long, int))(data->fd, data->offset, 0);
                if (previous.holder != 0)
                {
                    assignPackFsFileSystem(&packFsFileSystemOf(&previous)->next, packFsFileSystemOf(&fileSystem)->next);
                    assignPackFsFileSystem(&packFsFileSystemOf(&fileSystem)->next, g_packfs_file_systems);
                    assignPackFsFileSystem(&g_packfs_file_systems, fileSystem);
                }
            }
            else
            {
                assignPackFsFileSystem(&previous, fileSystem);
                assignPackFsFileSystem(&fileSystem, packFsFileSystemOf(&fileSystem)->next);
            }
        }
    }
    else
    {
        data = new PackFileDataRawFile_guess(0);
        if (g_packfs_log_count_guess < 0x40)
        {
            g_packfs_log_entries_guess[g_packfs_log_count_guess].first = PACKFS_CPP_PATH;
            g_packfs_log_entries_guess[g_packfs_log_count_guess].second = 0x40a;
            g_packfs_log_count_guess++;
        }
        if (data == 0)
            EQ_CDECL(void, 0x004F8310, (int, const char *, const char *))(0, (const char *)0x00570968, (const char *)0x00580F4C);
        data->fd = fd;
        data->size = EQ_CDECL(long, 0x0052594C, (int))(fd);
    }
    if (data != 0)
    {
        if (((PackFsReadable_guess *)data)->read_guess() > 0)
        {
            data->unknown0x200C = 0;
            data->unknown0x2010 = 0;
            data->name = value;
            *result = data;
            return true;
        }
        delete data;
    }
    return false;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f9e30
// Constructs an empty map tree: shares the nil node under the lock, then buys a self-linked head node.
// __thiscall, two stack args (callee pops 8), returns this. Copies *allocator_ to +0 and *compare_ to +1, clears
// +8, buys a 0x18-byte node (parent 0, colour 1), then under the lock either keeps it as the shared nil node
// (left and right cleared) or frees it, and bumps the nil's refcount. Outside the lock it buys a second node
// (parent = nil, colour 0) as the head, points its left and the head's right at itself, and clears +0xc.
FlagContainer_guess *FlagContainer_guess::initializeFlagContainer_guess(const char &compare_, const char &allocator_)
{
    FlagContainerNode_guess *tmp;
    FlagContainerNode_guess *node;

    allocator = allocator_;
    compare = compare_;
    flag = 0;
    tmp = (FlagContainerNode_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x18);
    tmp->parent = 0;
    tmp->color = 1;
    {
        PackFsLockit_guess lk;

        if (g_flagcontainer_nil_guess == 0)
        {
            g_flagcontainer_nil_guess = tmp;
            tmp->left = 0;
            tmp = 0;
            g_flagcontainer_nil_guess->right = 0;
        }
        g_flagcontainer_nilrefs_guess = g_flagcontainer_nilrefs_guess + 1;
    }
    if (tmp != 0)
    {
        EQ_CDECL(void, 0x005218BB, (void *))(tmp);
    }
    tmp = g_flagcontainer_nil_guess;
    node = (FlagContainerNode_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x18);
    node->parent = tmp;
    node->color = 0;
    head = node;
    size = 0;
    node->left = node;
    head->right = head;
    return this;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004f9ee0
// Creates the empty map used to index entries in a mounted pack file.
// __thiscall, returns this (callee pops 8). Copies allocator/comparator, shares or creates the nil node
// under the lock, then buys the red head node pointing at itself.
PackFsMap_guess *PackFsMap_guess::init_guess(const char &compare_, const char &allocator_)
{
    PackFsMapNode_guess *tmp;
    PackFsMapNode_guess *node;

    allocator = allocator_;
    compare = compare_;
    multi = 0;
    node = (PackFsMapNode_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(sizeof(PackFsMapNode_guess));
    tmp = node;
    node->parent = 0;
    node->color = 1;
    {
        PackFsLockit_guess lk;
        if (g_packfs_map_nil_guess == 0)
        {
            g_packfs_map_nil_guess = node;
            tmp = 0;
            node->left = 0;
            g_packfs_map_nil_guess->right = 0;
        }
        ++g_packfs_map_nilrefs_guess;
    }
    if (tmp != 0)
        EQ_CDECL(void, 0x005218BB, (void *))(tmp);
    tmp = g_packfs_map_nil_guess;
    node = (PackFsMapNode_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(sizeof(PackFsMapNode_guess));
    node->parent = tmp;
    node->color = 0;
    head = node;
    size = 0;
    head->left = head;
    head->right = head;
    return this;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004F9F90
// Indexes pack-file metadata by key, returning an existing match when duplicates are disabled.
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
        keyValue = (unsigned int *)EQ_METHOD(2, void *, 0x004FAB40, (PackFsTree_guess *, PackFsTreeNode_guess *, int), this)(parent, 0);
        PackFsTreeNode_guess *inserted = (PackFsTreeNode_guess *)keyValue;
        inserted->left = g_packfs_tree_nil_guess;
        inserted->right = g_packfs_tree_nil_guess;
        EQ_CDECL(void, 0x004FB780, (unsigned int *, unsigned int *))(keyValue + 3, value - 1);
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
                        EQ_METHOD(1, void, 0x004FAA80, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current);
                    }
                    ((unsigned int *)current->parent)[5] = 1;
                    ((unsigned int *)current->parent->parent)[5] = 0;
                    EQ_METHOD(1, void, 0x004FAAE0, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current->parent->parent);
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
                    EQ_METHOD(1, void, 0x004FAAE0, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current);
                }
                ((unsigned int *)current->parent)[5] = 1;
                ((unsigned int *)current->parent->parent)[5] = 0;
                EQ_METHOD(1, void, 0x004FAA80, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current->parent->parent);
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
            unsigned int *answer = EQ_METHOD(4, unsigned int *, 0x004FA810, (PackFsTree_guess *, unsigned int **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&keyValue, node, parent, value - 1);
            *result = *answer;
            *((unsigned char *)result + 4) = 1;
            return;
        }
        EQ_METHOD(0, void, 0x004FAB60, (PackFsTreeNode_guess **), &candidate)();
    }
    if (candidate->key < value[-1])
    {
        unsigned int *answer = EQ_METHOD(4, unsigned int *, 0x004FA810, (PackFsTree_guess *, unsigned int **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&keyValue, node, parent, value - 1);
        *result = *answer;
        *((unsigned char *)result + 4) = 1;
        return;
    }
    *result = (unsigned int)candidate;
    *((unsigned char *)result + 4) = 0;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fa1d0
// Removes one entry from the pack-fs tree of ref-counted objects and releases that entry's object reference.
// @early-stop: EBP frame, register allocation, and red-black fixup block layout still differ.
// The empty _Lockit occupies the top local slot, four bytes below entry ESP.
void __fastcall RedBlackTree_removeNodeFromRedBlackTree_guess(
    PackFsTree_guess *tree, int edxDummy, int *result, int *node)
{
    int temp;
    int *nodeTemp;
    int *rotationParent;
    int *ancestor;
    int *candidateLink;
    int *childLink;
    int *fixupNode;
    int *removedNode;
    int *header;
    int *parent;
    int *sibling;
    int *erased;
    int lockStorage;
    PackFsRefCounted_guess *refObject;

    erased = node;
    EQ_METHOD(0, void, 0x004FAE30, (int **), &node)();
    fixupNode = (int *)*erased;
    candidateLink = erased + 2;
    removedNode = erased;
    childLink = candidateLink;
    if (fixupNode == (int *)g_packfs_tree_nil_guess)
    {
        fixupNode = (int *)*candidateLink;
    }
    else
    {
        nodeTemp = (int *)*candidateLink;
        if (nodeTemp != (int *)g_packfs_tree_nil_guess)
        {
            removedNode = nodeTemp;
            for (childLink = (int *)*nodeTemp;
                 childLink != (int *)g_packfs_tree_nil_guess;
                 childLink = (int *)*childLink)
            {
                removedNode = childLink;
            }
            fixupNode = (int *)removedNode[2];
            childLink = removedNode + 2;
        }
    }
    EQ_METHOD(0, void, 0x00531890, (void *), &lockStorage)();
    candidateLink = erased + 2;
    if (removedNode != erased)
    {
        *(int **)(*erased + 4) = removedNode;
        *removedNode = *erased;
        if (removedNode == (int *)*candidateLink)
        {
            fixupNode[1] = (int)removedNode;
        }
        else
        {
            fixupNode[1] = removedNode[1];
            *(int **)removedNode[1] = fixupNode;
            removedNode[2] = *candidateLink;
            *(int **)(*candidateLink + 4) = removedNode;
        }
        header = *(int **)((char *)tree + 4);
        if (header[1] == (int)erased)
        {
            header[1] = (int)removedNode;
        }
        else
        {
            ancestor = (int *)erased[1];
            if ((int *)*ancestor == erased)
                *ancestor = (int)removedNode;
            else
                ancestor[2] = (int)removedNode;
        }
        nodeTemp = removedNode;
        removedNode = erased;
        nodeTemp[1] = erased[1];
        temp = nodeTemp[5];
        nodeTemp[5] = erased[5];
        erased[5] = temp;
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
            ancestor = (int *)erased[1];
            if ((int *)*ancestor == erased)
                *ancestor = (int)fixupNode;
            else
                ancestor[2] = (int)fixupNode;
        }
        childLink = *(int **)((char *)tree + 4);
        if ((int *)*childLink == erased)
        {
            if ((int *)*candidateLink == (int *)g_packfs_tree_nil_guess)
            {
                *childLink = erased[1];
            }
            else
            {
                nodeTemp = (int *)*fixupNode;
                candidateLink = fixupNode;
                while (ancestor = nodeTemp,
                       ancestor != (int *)g_packfs_tree_nil_guess)
                {
                    candidateLink = ancestor;
                    nodeTemp = (int *)*ancestor;
                }
                *childLink = (int)candidateLink;
            }
        }
        if ((*(int **)((char *)tree + 4))[2] == (int)erased)
        {
            if ((int *)*erased == (int *)g_packfs_tree_nil_guess)
            {
                candidateLink = (int *)erased[1];
            }
            else
            {
                childLink = (int *)fixupNode[2];
                candidateLink = fixupNode;
                while (nodeTemp = childLink,
                       nodeTemp != (int *)g_packfs_tree_nil_guess)
                {
                    candidateLink = nodeTemp;
                    childLink = (int *)nodeTemp[2];
                }
            }
            (*(int **)((char *)tree + 4))[2] = (int)candidateLink;
        }
    }
    int black = 0;
    if (removedNode[5] == 1)
    {
        header = *(int **)((char *)tree + 4);
        if (fixupNode != (int *)header[1])
        {
            do
            {
                if (fixupNode[5] != 1)
                    break;
                candidateLink = *(int **)fixupNode[1];
                if (fixupNode == candidateLink)
                {
                    candidateLink = (int *)((int **)fixupNode[1])[2];
                    if (candidateLink[5] == black)
                    {
                        candidateLink[5] = 1;
                        *(int *)(fixupNode[1] + 0x14) = black;
                        temp = fixupNode[1];
                        candidateLink = *(int **)(temp + 8);
                        *(int *)(temp + 8) = *candidateLink;
                        if ((int *)*candidateLink != (int *)g_packfs_tree_nil_guess)
                            ((int *)*candidateLink)[1] = temp;
                        candidateLink[1] = *(int *)(temp + 4);
                        if (temp == header[1])
                        {
                            header[1] = (int)candidateLink;
                        }
                        else
                        {
                            childLink = *(int **)(temp + 4);
                            if (temp == *childLink)
                                *childLink = (int)candidateLink;
                            else
                                childLink[2] = (int)candidateLink;
                        }
                        *candidateLink = temp;
                        *(int **)(temp + 4) = candidateLink;
                        candidateLink = *(int **)(fixupNode[1] + 8);
                    }
                    if ((*(int *)(*candidateLink + 0x14) != 1) ||
                        (*(int *)(candidateLink[2] + 0x14) != 1))
                    {
                        if (*(int *)(candidateLink[2] + 0x14) == 1)
                        {
                            *(int *)(*candidateLink + 0x14) = 1;
                            temp = *candidateLink;
                            candidateLink[5] = black;
                            *candidateLink = *(int *)(temp + 8);
                            if (*(int **)(temp + 8) !=
                                (int *)g_packfs_tree_nil_guess)
                                (*(int **)(temp + 8))[1] = (int)candidateLink;
                            *(int *)(temp + 4) = candidateLink[1];
                            if (candidateLink == (int *)header[1])
                            {
                                header[1] = temp;
                            }
                            else
                            {
                                childLink = (int *)candidateLink[1];
                                if (candidateLink == (int *)childLink[2])
                                    childLink[2] = temp;
                                else
                                    *childLink = temp;
                            }
                            *(int **)(temp + 8) = candidateLink;
                            candidateLink[1] = temp;
                            candidateLink = *(int **)(fixupNode[1] + 8);
                        }
                        candidateLink[5] = *(int *)(fixupNode[1] + 0x14);
                        *(int *)(fixupNode[1] + 0x14) = 1;
                        *(int *)(candidateLink[2] + 0x14) = 1;
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
                            candidateLink = (int *)parent[1];
                            if (parent == (int *)*candidateLink)
                            {
                                *candidateLink = (int)sibling;
                                *sibling = (int)parent;
                            }
                            else
                            {
                                candidateLink[2] = (int)sibling;
                                *sibling = (int)parent;
                            }
                        }
                        parent[1] = (int)sibling;
                        break;
                    }
                }
                else
                {
                    if (candidateLink[5] == black)
                    {
                        candidateLink[5] = 1;
                        *(int *)(fixupNode[1] + 0x14) = black;
                        candidateLink = (int *)fixupNode[1];
                        temp = *candidateLink;
                        *candidateLink = *(int *)(temp + 8);
                        if (*(int **)(temp + 8) !=
                            (int *)g_packfs_tree_nil_guess)
                            (*(int **)(temp + 8))[1] = (int)candidateLink;
                        *(int *)(temp + 4) = candidateLink[1];
                        if (candidateLink == (int *)header[1])
                        {
                            header[1] = temp;
                        }
                        else
                        {
                            childLink = (int *)candidateLink[1];
                            if (candidateLink == (int *)childLink[2])
                                childLink[2] = temp;
                            else
                                *childLink = temp;
                        }
                        *(int **)(temp + 8) = candidateLink;
                        candidateLink[1] = temp;
                        candidateLink = *(int **)fixupNode[1];
                    }
                    if ((*(int *)(candidateLink[2] + 0x14) != 1) ||
                        (*(int *)(*candidateLink + 0x14) != 1))
                    {
                        if (*(int *)(*candidateLink + 0x14) == 1)
                        {
                            *(int *)(candidateLink[2] + 0x14) = 1;
                            childLink = (int *)candidateLink[2];
                            candidateLink[5] = black;
                            candidateLink[2] = *childLink;
                            if ((int *)*childLink !=
                                (int *)g_packfs_tree_nil_guess)
                                ((int *)*childLink)[1] = (int)candidateLink;
                            childLink[1] = candidateLink[1];
                            if (candidateLink == (int *)header[1])
                            {
                                header[1] = (int)childLink;
                            }
                            else
                            {
                                nodeTemp = (int *)candidateLink[1];
                                if (candidateLink == (int *)*nodeTemp)
                                    *nodeTemp = (int)childLink;
                                else
                                    nodeTemp[2] = (int)childLink;
                            }
                            *childLink = (int)candidateLink;
                            candidateLink[1] = (int)childLink;
                            candidateLink = *(int **)fixupNode[1];
                        }
                        candidateLink[5] = *(int *)(fixupNode[1] + 0x14);
                        *(int *)(fixupNode[1] + 0x14) = 1;
                        *(int *)(*candidateLink + 0x14) = 1;
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
                            rotationParent = (int *)parent[1];
                            if (parent == (int *)rotationParent[2])
                                rotationParent[2] = (int)sibling;
                            else
                                *rotationParent = (int)sibling;
                        }
                        sibling[2] = (int)parent;
                        parent[1] = (int)sibling;
                        break;
                    }
                }
                candidateLink[5] = black;
                fixupNode = (int *)fixupNode[1];
            } while (fixupNode != (int *)header[1]);
        }
        fixupNode[5] = 1;
    }
    EQ_METHOD(0, void, 0x0053192C, (void *), &lockStorage)();
    refObject = (PackFsRefCounted_guess *)removedNode[4];
    if (refObject != 0)
    {
        temp = refObject->refCount;
        refObject->refCount = temp - 1;
        if (temp - 1 == 0 && refObject != 0)
            refObject->destroy_guess(1);
    }
    EQ_CDECL(void, 0x005218BB, (void *))(removedNode);
    temp = *(int *)((char *)tree + 0x0c);
    *(int *)((char *)tree + 0x0c) = temp - 1;
    *result = (int)node;
    return;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fa5f0
// __thiscall, iterator returned through a hidden pointer. The node holding the key, or the head node when absent.
PackFsTreeIterator_guess PackFsTree_guess::find_guess(const unsigned int &key)
{
    PackFsTreeNode_guess *x;
    PackFsTreeNode_guess *y;
    PackFsTreeIterator_guess where;
    PackFsTreeIterator_guess result;

    x = head->parent;
    y = head;
    if (x != g_packfs_tree_nil_guess)
    {
        do
        {
            if (x->key < key)
                x = x->right;
            else
            {
                y = x;
                x = x->left;
            }
        } while (x != g_packfs_tree_nil_guess);
    }
    where.node = y;
    // The conditional addresses both iterators, so they stay in stack slots as in the original. The head
    // iterator is built in the false arm, so `result.node = head` runs only on the not-found path.
    return (where.node != head && !(key < where.node->key)) ? where : (result.node = head, result);
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fa660
// Inserts an entry unless its key exists (always if flag +8 is set); returns node and inserted flag.
// @early-stop: EBP frame, key/header register allocation, comparison loads, merged helper calls, and shared epilogues remain.
RedBlackTreeFindResult_guess *RedBlackTreeLookup_guess::findNodeInRedBlackTree_guess(RedBlackTreeFindResult_guess *result, unsigned int *key)
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
        result->node = *EQ_THISCALL(PackFsTreeNode_guess **, 0x004FABC0,
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
            result->node = *EQ_THISCALL(PackFsTreeNode_guess **, 0x004FABC0,
                (RedBlackTreeLookup_guess *, int, PackFsTreeNode_guess **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, const unsigned int *))
                (this, (int)head, &insertedNodeValue, x, y, searchKey);
            result->inserted = 1;
            return result;
        }
        EQ_METHOD(0, void, 0x004FAE80, (PackFsTreeNode_guess **), &candidate)();
    }

    if (candidate->key < *searchKey)
    {
        PackFsTreeNode_guess *insertedNodeValue;
        result->node = *EQ_THISCALL(PackFsTreeNode_guess **, 0x004FABC0,
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fa750
// __thiscall, iterator returned through a hidden pointer. The node holding the key, or the head node when absent.
PackFsTreeIterator_guess PackFsTree2_guess::find_guess(const unsigned int &key)
{
    PackFsTreeNode_guess *x;
    PackFsTreeNode_guess *y;
    PackFsTreeIterator_guess where;
    PackFsTreeIterator_guess result;

    x = head->parent;
    y = head;
    if (x != g_packfs_tree2_nil_guess)
    {
        do
        {
            if (x->key < key)
                x = x->right;
            else
            {
                y = x;
                x = x->left;
            }
        } while (x != g_packfs_tree2_nil_guess);
    }
    where.node = y;
    // The conditional addresses both iterators, so they stay in stack slots as in the original. The head
    // iterator is built in the false arm, so `result.node = head` runs only on the not-found path.
    return (where.node != head && !(key < where.node->key)) ? where : (result.node = head, result);
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fa7c0
// Release the objects held by tree entries while clearing the PackFS lookup tree.
// __thiscall (this only passed on), 1 stack arg. Frees the subtree at x: recurses into each right
// subtree, then walks left, releasing the value at 0xc (0x004F9300) and deleting each node.
void PackFsTree_guess::erase_guess(PackFsTreeNode_guess *x)
{
    PackFsTreeNode_guess *y;

    for (y = x; y != g_packfs_tree_nil_guess; x = y)
    {
        erase_guess(y->right);
        y = y->left;
        EQ_METHOD(0, void, 0x004F9300, (void *), &x->key)();
        EQ_CDECL(void, 0x005218BB, (void *))(x);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004FA810
// Links a new entry (key + shared object) under the given parent in the pack-fs tree, then rebalances it.
// @early-stop: Entry frame/register allocation, node color stores, and rotation load schedules still differ.
int PackFsTreeInsert_guess::insert_guess(PackFsTreeIterator_guess *result, int hintNode, int *parentNode, unsigned int *keyValue)
{
    enum Color { Red, Black };
    struct Node {
        Node *left;
        Node *parent;
        Node *right;
        unsigned int key;
        int value;
        Color color;
    };
    Node *node = (Node *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x18);
    Node *parent = (Node *)parentNode;
    node->parent = parent;
    node->color = Red;
    node->left = (Node *)g_packfs_tree_nil_guess;
    node->right = (Node *)g_packfs_tree_nil_guess;
    EQ_CDECL(void, 0x004FB780, (void *, void *))(&node->key, keyValue);
    ++size;
    if (parent == (Node *)head || hintNode != (int)g_packfs_tree_nil_guess ||
        *keyValue < parent->key)
    {
        parent->left = node;
        Node *header = (Node *)head;
        if (parent == header)
        {
            header->parent = node;
            ((Node *)head)->right = node;
        }
        else if (parent == header->left)
            header->left = node;
    }
    else
    {
        parent->right = node;
        if (parent == ((Node *)head)->right)
            ((Node *)head)->right = node;
    }

    Node *x = node;
    while (x != ((Node *)head)->parent && x->parent->color == Red)
    {
        Node *p = x->parent;
        Node *grandparent = p->parent;
        Node *y = grandparent->left;
        if (p == y)
        {
            y = grandparent->right;
            if (y->color == Red)
            {
                p->color = Black;
                y->color = Black;
                x->parent->parent->color = Red;
                x = x->parent->parent;
            }
            else
            {
                if (x == p->right)
                {
                    x = p;
                    p = x->right;
                    x->right = p->left;
                    if (p->left != (Node *)g_packfs_tree_nil_guess)
                        p->left->parent = x;
                    p->parent = x->parent;
                    if (x == ((Node *)head)->parent)
                        ((Node *)head)->parent = p;
                    else if (x == x->parent->left)
                        x->parent->left = p;
                    else
                        x->parent->right = p;
                    // This rotation is on the grandparent's left link.
                    p->left = x;
                    x->parent = p;
                }
                x->parent->color = Black;
                x->parent->parent->color = Red;
                p = x->parent->parent;
                y = p->left;
                p->left = y->right;
                if (y->right != (Node *)g_packfs_tree_nil_guess)
                    y->right->parent = p;
                y->parent = p->parent;
                if (p == ((Node *)head)->parent)
                {
                    ((Node *)head)->parent = y;
                    y->right = p;
                }
                else if (p == p->parent->right)
                {
                    p->parent->right = y;
                    y->right = p;
                }
                else
                {
                    p->parent->left = y;
                    y->right = p;
                }
                p->parent = y;
            }
        }
        else
        {
            if (y->color == Red)
            {
                p->color = Black;
                y->color = Black;
                x->parent->parent->color = Red;
                x = x->parent->parent;
            }
            else
            {
                if (x == p->left)
                {
                    x = p;
                    p = x->left;
                    x->left = p->right;
                    if (p->right != (Node *)g_packfs_tree_nil_guess)
                        p->right->parent = x;
                    p->parent = x->parent;
                    if (x == ((Node *)head)->parent)
                        ((Node *)head)->parent = p;
                    else if (x == x->parent->right)
                        x->parent->right = p;
                    else
                        x->parent->left = p;
                    // This rotation is on the grandparent's right link.
                    p->right = x;
                    x->parent = p;
                }
                x->parent->color = Black;
                x->parent->parent->color = Red;
                p = x->parent->parent;
                y = p->right;
                p->right = y->left;
                if (y->left != (Node *)g_packfs_tree_nil_guess)
                    y->left->parent = p;
                y->parent = p->parent;
                if (p == ((Node *)head)->parent)
                    ((Node *)head)->parent = y;
                else if (p == p->parent->left)
                    p->parent->left = y;
                else
                    p->parent->right = y;
                y->left = p;
                p->parent = y;
            }
        }
    }
    ((Node *)head)->parent->color = Black;
    result->node = (PackFsTreeNode_guess *)node;
    return (int)result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fab40
// __thiscall (ignores this), 2 stack args. Allocates a 0x18-byte node and sets its parent and color.
PackFsTreeNodeAlloc_guess *PackFsTreeAllocator_guess::buyNode_guess(PackFsTreeNode_guess *parent, int color)
{
    PackFsTreeNodeAlloc_guess *node;

    node = (PackFsTreeNodeAlloc_guess *)operator new(sizeof(PackFsTreeNodeAlloc_guess));
    node->parent = parent;
    node->color = color;
    return node;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004FABC0
// Links a new 4-dword entry (keyed by its first dword) under the given parent in the pack-fs map; rebalances.
// @early-stop: Frame/register allocation, constant stores, and inner-rotation load scheduling still differ.
int ** __fastcall InsertQuadKeyTreeNode_guess(void *tree, int edxDummy, int **result, int x, int *y, unsigned int *searchKey)
{
    int uncleAddress;
    int *ancestor;
    int *node;
    int *current;
    int *parent;
    int *sibling;

    node = (int *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x20);
    node[1] = (int)y;
    node[7] = 0;
    node[0] = (int)EQ_GLOBAL(int *, 0x006F03A0);
    node[2] = (int)EQ_GLOBAL(int *, 0x006F03A0);
    EQ_CDECL(void, 0x004FB7A0, (unsigned int *, const unsigned int *))
        ((unsigned int *)(node + 3), searchKey);
    *(int *)((char *)tree + 0xC) = *(int *)((char *)tree + 0xC) + 1;
    if ((y == *(int **)((char *)tree + 4)) ||
        (x != (int)EQ_GLOBAL(int *, 0x006F03A0)) ||
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
                    if (*parent != (int)EQ_GLOBAL(int *, 0x006F03A0))
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
                if (sibling[2] != (int)EQ_GLOBAL(int *, 0x006F03A0))
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
                    if (parent[2] != (int)EQ_GLOBAL(int *, 0x006F03A0))
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
                if (*sibling != (int)EQ_GLOBAL(int *, 0x006F03A0))
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
    *result = node;
    return result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fae30
// Move the tree cursor to the next indexed entry.
// __thiscall, no stack args. Advances the iterator to the in-order successor (STL _Tree iterator _Inc):
// the leftmost node of the right subtree, else the first ancestor reached from a left child.
void PackFsTreeIterator_guess::increment_guess()
{
    PackFsTreeNode_guess *x;
    PackFsTreeNode_guess *p;

    if (node->right != g_packfs_tree_nil_guess)
    {
        x = node->right;
        while (x->left != g_packfs_tree_nil_guess)
            x = x->left;
        node = x;
    }
    else
    {
        while (node == (p = node->parent)->right)
            node = p;
        if (node->right != p)
            node = p;
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fae80
// Move this tree cursor to the previous indexed entry.
// __thiscall, no stack args. Moves the iterator to the in-order predecessor (STL _Tree iterator _Dec):
// from the head (red, its parent's parent is itself) to the maximum at its right, else the rightmost
// node of the left subtree, else the first ancestor reached from a right child.
void PackFsTree2Iterator_guess::decrement_guess()
{
    PackFsTree2Node_guess *x;
    PackFsTree2Node_guess *p;

    if (node->color == 0 && node->parent->parent == node)
        node = node->right;
    else if (node->left != g_packfs_tree2_nil_guess)
    {
        x = node->left;
        while (x->right != g_packfs_tree2_nil_guess)
            x = x->right;
        node = x;
    }
    else
    {
        while (node == (p = node->parent)->left)
            node = p;
        node = p;
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004faee0
// Create a shared handle for the file map used to register a pack file system.
// __thiscall, 1 stack arg (callee pops 4), written as __fastcall with an unused EDX slot. Constructs a shared pointer
// to a new reference counted object holding `fileMap`; the inlined log entry and the allocation check are kept.
PackFsSharedHolder_guess *__fastcall SharedPointer_SharedPointer_guess(PackFsSharedHolder_guess *self, int /* EDX unused */, PackFsMap_guess *fileMap)
{
    PackFsRefDerived_guess *object;
    unsigned int count;

    self->object = 0;
    if (fileMap != 0)
    {
        object = EQ_CDECL(PackFsRefDerived_guess *, 0x005218C6, (unsigned int))(0xc);
        if (object != 0)
        {
            object->map = fileMap;
            object->refCount = 0;
            object->vtbl = 0x005360FC;
        }
        count = g_packfs_log_count_guess;
        if (count < 0x40)
        {
            g_packfs_log_entries_guess[count].first = 0x00580E14;
            g_packfs_log_entries_guess[count].second = 0x102;
            g_packfs_log_count_guess = count + 1;
        }
        if (object == 0)
            EQ_CDECL(void, 0x004F8310, (int, const char *, const char *))(0, (const char *)0x00570968, (const char *)0x00580DFC);
        self->object = object;
        object->refCount++;
    }
    return self;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004faf70
// Run file-map object teardown and free its storage when deletion is requested.
// __thiscall (callee pops flags), written as __fastcall with an unused EDX slot so `this` arrives in ECX. Scalar
// deleting destructor of PackFsRefDerived_guess: runs the destructor at 0x004FAF90, frees the object with the cdecl
// operator delete at 0x005218BB when bit 0 of flags is set, returns this.
PackFsRefDerived_guess *__fastcall DestroyFunctionObject_guess(PackFsRefDerived_guess *self, int /* EDX unused */, unsigned char flags)
{
    EQ_METHOD(0, void, 0x004FAF90, (void *), self)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(self);
    return self;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004faf90
// Destroy a shared file-map object and release its indexed entries.
// @early-stop: the EH prolog/frame and implicit base-destructor call still differ; helper calls also need dummy EDX setup.
// __thiscall destructor of a reference counted object that owns a heap std::map at +0x8 (head at 0x4 of the
// map, size at 0xc). Stores the derived vtable, then if the map exists: inlined ~map = erase(begin(), end())
// (the whole tree is freed by 0x004FB550 when the range is the full tree, else the nodes are erased one by one
// with 0x004FB590 / 0x004FB100), the head node is freed, and the shared nil node is released under std::_Lockit
// (freed when its count 0x006F03A4 drops to zero). The map itself is freed last. Then the inlined base destructor
// stores the base vtable, drops refCount (+0x4) and calls the virtual destroy(1) when it reaches zero.
PackFsRefDerived_guess::~PackFsRefDerived_guess()
{
    PackFsMap_guess *m;
    PackFsMapNode_guess *first;
    PackFsMapNode_guess *last;
    PackFsMapNode_guess *ret;
    PackFsMapNode_guess *tmp1;
    PackFsMapNode_guess *tmp2;
    PackFsMapNode_guess *tmp3;
    PackFsMapNode_guess **p;
    PackFsMapNode_guess *t;
    int size;
    bool different;

    // Preserve the EH boundary around derived cleanup so the base destructor runs during unwinding.
    try
    {
        vtbl = 0x005360FC;
        m = map;
        if (m != 0)
        {
            last = m->head;
            size = m->size;
            first = last->left;
            if (size != 0
                && (different = first != *EQ_THISCALL(PackFsMapNode_guess **, 0x004FB0F0,
                    (PackFsMap_guess *, int, PackFsMapNode_guess **))(m, 0, &tmp1)) == 0
                && last == m->head)
            {
                EQ_THISCALL(void, 0x004FB550, (PackFsMap_guess *, int, PackFsMapNode_guess *))(m, 0, m->head->parent);
                m->head->parent = g_packfs_map_nil_guess;
                m->size = 0;
                m->head->left = m->head;
                m->head->right = m->head;
                EQ_THISCALL(PackFsMapNode_guess **, 0x004FB0F0,
                    (PackFsMap_guess *, int, PackFsMapNode_guess **))(m, 0, &ret);
            }
            else
            {
                if (first != last)
                {
                    do
                    {
                        p = EQ_THISCALL(PackFsMapNode_guess **, 0x004FB590,
                            (PackFsMapNode_guess **, int, PackFsMapNode_guess **, int))(&first, 0, &tmp2, 0);
                        EQ_THISCALL(PackFsMapNode_guess **, 0x004FB100,
                            (PackFsMap_guess *, int, PackFsMapNode_guess **, PackFsMapNode_guess *))(m, 0, &tmp3, *p);
                    } while (first != last);
                }
                ret = first;
            }
            EQ_CDECL(void, 0x005218BB, (void *))(m->head);
            m->head = 0;
            m->size = 0;
            {
                t = 0;
                PackFsLockit_guess lk;
                unsigned int nilRefCount = g_packfs_map_nilrefs_guess;
                --nilRefCount;
                g_packfs_map_nilrefs_guess = nilRefCount;

                if (nilRefCount == 0)
                {
                    t = g_packfs_map_nil_guess;
                    g_packfs_map_nil_guess = 0;
                }
            }
            if (t != 0)
                EQ_CDECL(void, 0x005218BB, (void *))(t);
            EQ_CDECL(void, 0x005218BB, (void *))(m);
        }
    }
    catch (...)
    {
        throw;
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb0f0
// Fetch the map boundary node used to choose the file-map cleanup path.
// __thiscall, 1 stack arg (RET 4, the callee pops it). Stores the dword that `target` points at into *out,
// then returns out (EAX). `target` is this+0x4.
unsigned int *PackFsValueHolder_guess::copyTargetValue_guess(unsigned int *out)
{
    *out = *target;
    return out;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb100
// Removes one 4-dword entry from the map a ref-counted pack-fs object owns (emptied by its destructor).
// @early-stop: EBP frame, splice/minimum traversal registers, constant idioms, rotation tail merging and epilogue still differ.
// __fastcall (`self` in ECX, EDX dummy), 2 stack args (`out`, `where`), callee pops 8, returns out (EAX).
// std::map erase(iterator) on the tree at self (head at 0x4, size at 0xc). `where` is first advanced in place
// to its in-order successor (PackFsTreeIterator_increment_guess) and that successor is stored to *out. The node is spliced out (its
// successor takes its place when it has two children), the tree is rebalanced under std::_Lockit (colour 1 =
// black, 0 = red), then the node is deleted and size drops by one.
PackFsMapNode_guess **__fastcall PackFsMap_erase_iterator_guess(PackFsMap_guess *self, int edxDummy,
    PackFsMapNode_guess **out, PackFsMapNode_guess *where)
{
    PackFsMapNode_guess *z;
    PackFsMapNode_guess *x;
    PackFsMapNode_guess *y;
    PackFsMapNode_guess *head;
    PackFsMapNode_guess *parent;
    PackFsMapNode_guess *w;
    PackFsMapNode_guess *c;
    int color;
    PackFsMapNode_guess *nilNode;
    PackFsMapNode_guess **right;

    z = where;
    EQ_METHOD(0, void, 0x004FB500, (PackFsMapNode_guess **), &where)();
    x = z->left;
    nilNode = g_packfs_map_nil_guess;
    right = &z->right;
    y = z;
    if (x == nilNode)
    {
        x = *right;
    }
    else if (*right != nilNode)
    {
        c = *right;
        while (c->left != nilNode)
            c = c->left;
        x = c->right;
        y = c;
    }
    {
        PackFsLockit_guess lk;

        if (y != z)
        {
            z->left->parent = y;
            y->left = z->left;
            if (y == *right)
            {
                x->parent = y;
            }
            else
            {
                x->parent = y->parent;
                y->parent->left = x;
                y->right = *right;
                (*right)->parent = y;
            }
            head = self->head;
            if (head->parent == z)
            {
                self->head->parent = y;
            }
            else if (z->parent->left == z)
            {
                z->parent->left = y;
            }
            else
            {
                z->parent->right = y;
            }
            y->parent = z->parent;
            color = z->color;
            z->color = y->color;
            y->color = color;
            y = z;
        }
        else
        {
            x->parent = y->parent;
            head = self->head;
            if (head->parent == z)
            {
                self->head->parent = x;
            }
            else if (z->parent->left == z)
            {
                z->parent->left = x;
            }
            else
            {
                z->parent->right = x;
            }
            head = self->head;
            if (head->left == z)
            {
                if (z->right == g_packfs_map_nil_guess)
                {
                    head->left = z->parent;
                }
                else
                {
                    c = x;
                    for (w = c->left; w != g_packfs_map_nil_guess; w = c->left)
                        c = w;
                    head->left = c;
                }
            }
            if (self->head->right == z)
            {
                if (z->left == g_packfs_map_nil_guess)
                {
                    self->head->right = z->parent;
                }
                else
                {
                    c = x;
                    for (w = c->right; w != g_packfs_map_nil_guess; w = c->right)
                        c = w;
                    self->head->right = c;
                }
            }
        }
        if (z->color == 1)
        {
            for (; x != self->head->parent; x = x->parent)
            {
                if (x->color != 1)
                    break;
                if (x == x->parent->left)
                {
                    w = x->parent->right;
                    if (w->color == 0)
                    {
                        w->color = 1;
                        x->parent->color = 0;
                        parent = x->parent;
                        w = parent->right;
                        parent->right = w->left;
                        if (w->left != g_packfs_map_nil_guess)
                        {
                            w->left->parent = parent;
                        }
                        w->parent = parent->parent;
                        if (parent == self->head->parent)
                        {
                            self->head->parent = w;
                        }
                        else if (parent == parent->parent->left)
                        {
                            parent->parent->left = w;
                        }
                        else
                        {
                            parent->parent->right = w;
                        }
                        w->left = parent;
                        parent->parent = w;
                        w = x->parent->right;
                    }
                    if (w->left->color == 1 && w->right->color == 1)
                    {
                        w->color = 0;
                    }
                    else
                    {
                        if (w->right->color == 1)
                        {
                            w->left->color = 1;
                            w->color = 0;
                            c = w->left;
                            w->left = c->right;
                            if (c->right != g_packfs_map_nil_guess)
                            {
                                c->right->parent = w;
                            }
                            c->parent = w->parent;
                            if (w == self->head->parent)
                            {
                                self->head->parent = c;
                            }
                            else if (w == w->parent->right)
                            {
                                w->parent->right = c;
                            }
                            else
                            {
                                w->parent->left = c;
                            }
                            c->right = w;
                            w->parent = c;
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = 1;
                        w->right->color = 1;
                        parent = x->parent;
                        w = parent->right;
                        parent->right = w->left;
                        if (w->left != g_packfs_map_nil_guess)
                        {
                            w->left->parent = parent;
                        }
                        w->parent = parent->parent;
                        if (parent == self->head->parent)
                        {
                            self->head->parent = w;
                        }
                        else if (parent == parent->parent->left)
                        {
                            parent->parent->left = w;
                        }
                        else
                        {
                            parent->parent->right = w;
                        }
                        w->left = parent;
                        parent->parent = w;
                        break;
                    }
                }
                else
                {
                    w = x->parent->left;
                    if (w->color == 0)
                    {
                        w->color = 1;
                        x->parent->color = 0;
                        parent = x->parent;
                        w = parent->left;
                        parent->left = w->right;
                        if (w->right != g_packfs_map_nil_guess)
                        {
                            w->right->parent = parent;
                        }
                        w->parent = parent->parent;
                        if (parent == self->head->parent)
                        {
                            self->head->parent = w;
                        }
                        else if (parent == parent->parent->right)
                        {
                            parent->parent->right = w;
                        }
                        else
                        {
                            parent->parent->left = w;
                        }
                        w->right = parent;
                        parent->parent = w;
                        w = x->parent->left;
                    }
                    if (w->right->color == 1 && w->left->color == 1)
                    {
                        w->color = 0;
                    }
                    else
                    {
                        if (w->left->color == 1)
                        {
                            w->right->color = 1;
                            w->color = 0;
                            c = w->right;
                            w->right = c->left;
                            if (c->left != g_packfs_map_nil_guess)
                            {
                                c->left->parent = w;
                            }
                            c->parent = w->parent;
                            if (w == self->head->parent)
                            {
                                self->head->parent = c;
                            }
                            else if (w == w->parent->left)
                            {
                                w->parent->left = c;
                            }
                            else
                            {
                                w->parent->right = c;
                            }
                            c->left = w;
                            w->parent = c;
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = 1;
                        w->left->color = 1;
                        parent = x->parent;
                        w = parent->left;
                        parent->left = w->right;
                        if (w->right != g_packfs_map_nil_guess)
                        {
                            w->right->parent = parent;
                        }
                        w->parent = parent->parent;
                        if (parent == self->head->parent)
                        {
                            self->head->parent = w;
                        }
                        else if (parent == parent->parent->right)
                        {
                            parent->parent->right = w;
                        }
                        else
                        {
                            parent->parent->left = w;
                        }
                        w->right = parent;
                        parent->parent = w;
                        break;
                    }
                }
            }
            x->color = 1;
        }
    }
    EQ_CDECL(void, 0x005218BB, (PackFsMapNode_guess *))(y);
    self->size = self->size - 1;
    *out = where;
    return out;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb500
// Advance the map cleanup cursor to its next node.
// __fastcall (`self` in ECX), no stack args, callee pops 0 bytes, no return value. Moves the iterator
// *self to the in-order successor of its node: the leftmost node of the right subtree when there is one,
// else the first ancestor reached from a left child. The nil node is loaded once, as in the original.
void __fastcall PackFsTreeIterator_increment_guess(PackFsTreeNode_guess **self)
{
    PackFsTreeNode_guess *nilNode;
    PackFsTreeNode_guess *right;
    PackFsTreeNode_guess *left;
    PackFsTreeNode_guess *up;

    nilNode = EQ_GLOBAL(PackFsTreeNode_guess *, 0x006F03A0);
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb550
// Clear the file map's index nodes during teardown.
// __thiscall (this only passed on), 1 stack arg, callee pops 4, no return value. Frees the map subtree at x:
// recurses into each right subtree, then deletes the node and continues with its left child until the shared
// nil node (0x006F03A0) is reached. Unlike the tree version at 0x004FA7C0, no value is released. The nil node
// is reloaded after each delete, as in the original.
void PackFsMap_guess::erase_guess(PackFsMapNode_guess *x)
{
    PackFsMapNode_guess *y;

    if ((y = x) != EQ_GLOBAL(PackFsMapNode_guess *, 0x006F03A0))
    {
        do
        {
            erase_guess(y->right);
            y = y->left;
            EQ_CDECL(void, 0x005218BB, (void *))(x);
            x = y;
        } while (y != EQ_GLOBAL(PackFsMapNode_guess *, 0x006F03A0));
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb590
// Return the current file-map entry while advancing its iterator.
// __thiscall (iterator in ECX), 2 stack args: the hidden return buffer (first) and an unused int (the postfix
// dummy), callee pops 8. Postfix ++ on a map iterator: stores the current node in the buffer, moves the iterator
// to its in-order successor (leftmost node of the right subtree, else the nearest ancestor whose left subtree
// holds the node), and returns the buffer pointer in EAX.
PackFsMapIterator_guess *PackFsMapIterator_guess::postIncrement_guess(PackFsMapIterator_guess *result, int)
{
    PackFsMapNode_guess *x;
    PackFsMapNode_guess *y;
    PackFsMapNode_guess *p;

    x = node;
    y = x->right;
    if (y != g_packfs_map_nil_guess)
    {
        for (p = y->left; p != g_packfs_map_nil_guess; p = p->left)
            y = p;
        node = y;
        result->node = x;
        return result;
    }
    p = x->parent;
    if (node == p->right)
    {
        do
        {
            node = p;
            p = p->parent;
        } while (node == p->right);
    }
    if (node->right != p)
        node = p;
    result->node = x;
    return result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004FB5F0
// Release one reference to a shared PackFS object, destroying it when no handles remain.
// __fastcall (the shared pointer in ECX, no stack args, no return value; callee pops 0). Drops one reference on
// the object the shared pointer holds. When the count reaches zero, calls its first virtual function with 1
// (the deleting destructor). The shared pointer itself is not cleared. Null objects are skipped.
// Named apart from 0x004c4813 (decrementReferenceCount_guess): the same name with another parameter type is
// an overload, and the test DLL can't resolve it by name.
void __fastcall decrementSharedReferenceCount_guess(PackFsSharedPtr_guess *shared)
{
    if (shared->ptr != 0)
    {
        PackFsRefCounted_guess *obj = shared->ptr;
        int count;

        count = obj->refCount - 1;
        obj->refCount = count;
        if (count == 0 && obj != 0)
            obj->destroy_guess(1);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)


#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb6b0
// Copy a PackFS shared handle and retain the object it points to.
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb6d0
// __fastcall (this in ECX, no stack args).
void * __fastcall getObjectDataPointer_guess(PackFsHandle_guess *handle)
{
    return handle->object->data;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004fb610
// Replace a shared PackFS object handle, releasing the old object and retaining the new one.
// __thiscall, 1 stack arg (the shared pointer by value; its PackFsRefValue_guess destructor releases the copy on return).
// "t" (favor speed) is set for this function only: the original writes its EH prolog out and expands the destructor inline.
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004fb6e0
// Replace a filesystem-list link's reference, releasing the old target and retaining the new one.
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb780
// Copy a pack-file tree entry while retaining its referenced object.
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb7a0
// Copy a four-word key used to index pack-file tree entries.
// __cdecl, 2 stack args. Copies four dwords into `dest` (nothing when it is null). Named apart from the
// 0x004FB780 copyKeyValue_guess so the test harness finds each by name.
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004fb7d0
// Scalar deleting destructor: calls 0x004F9300 on this, then frees this when bit 0 of flags is set.
// __thiscall, 1 stack arg. Scalar deleting destructor: releases the held reference, frees the object
// when bit 0 of flags is set, returns this.
SuiteDefaultsElement_guess *SuiteDefaultsElement_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x004F9300, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb7f0
// __thiscall constructor, 1 stack arg. Starts with a zero count, stores the value and sets the vtable.
/* fake match: the original's count store is a write-only `mov dword ptr [this+4], 0`. /O1 folds a zero store to
   `and [mem], 0`, so "t" (favor speed) is set for this function only to emit the mov. */
#pragma optimize("t", on)
PackFsSharedPointer2_guess::PackFsSharedPointer2_guess(int value_arg)
{
    refCount = 0;
    value = value_arg;
    vtable = (const void *)0x00536104;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004fb810
// Dispose of a shared pointer wrapper and free it when deletion is requested.
PackFsSharedPointer2_guess *PackFsSharedPointer2_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x004FB830, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004fb830
// Release the resources held by shared storage and drop the owning object's reference.
// @early-stop: no EH state stores 3..0 before the releases; VC6 drops each release's second null test.
// __thiscall, no args (callee pops 0). Releases the storage's held references and deletes the storage. Then drops the
// object's own count and calls slot 0 with 1 when it reaches zero.
PackFsSharedPointer2_guess::~PackFsSharedPointer2_guess()
{
    PackFsSharedPointerStorage_guess *storage;

    {
        /* fake match: an empty destructible local builds the EH frame; its members fill the two frame slots, which nothing reads. */
        PackFsEHMarker_guess marker;

        marker.self = this;
        /* fake match: the volatile store keeps the vtable store ahead of the storage load, as in the original. */
        *(const void * volatile *)this = (const void *)0x00536104;
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
                EQ_FUNC(void, __fastcall, 0x004C4813, (PackFsRefCounted_guess *))(storage->base);

            EQ_CDECL(void, 0x005218BB, (void *))(storage);
        }
    }
    int remaining = refCount - 1;
    vtable = (const void *)0x005360F4;
    refCount = remaining;
    if (remaining == 0 && this != 0)
    {
        /* fake match: a volatile vptr read keeps the vptr in EDX for the slot call, as the original's call [edx]. */
        void **slots;

        slots = *(void ** volatile *)this;
        ((void (__fastcall *)(void *, void **, int))slots[0])(this, slots, 1);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004fb920
// Initialize a shared holder for the pack file's directory buffer and entry count.
// __thiscall, 2 stack args. Starts with a zero count, stores the two values and sets the vtable. Returns
// this (EAX = ECX in the original).
PackFsSharedPointerData_guess *PackFsSharedPointerData_guess::initializeSharedPointerWithData_guess(int first_arg, int second_arg)
{
    refCount = 0;
    first = first_arg;
    second = second_arg;
    vtable = (const void *)0x0053610C;
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004fb940
// Release a resource object's data and optionally free its allocation.
// __thiscall, 1 stack arg (callee pops 4). Runs DestroyResourceAndDecrementRefCount_guess (0x004fb960) on
// this, then frees this with the client's cdecl operator delete when bit 0 of flags is set. Returns this
// (EAX = ESI in the original).
ResourceObject_guess *ResourceObject_guess::ResourceObject_DestroyAndFreeIfRequested_destructor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x004FB960, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("t", on)
// FUNCTION: TRILOGY 0x004fb960
// Free resource data and drop its object's reference, triggering final destruction at zero.
// __fastcall (this in ECX, no stack args, callee pops 0). Points the vtable at 0x0053610C, frees the data
// at 0x8 with the client's cdecl operator delete, then points the vtable at 0x005360F4 and stores
// refCount - 1. When that is zero and the pointer is not null, calls slot 0 of the vtable with 1.
void __fastcall DestroyResourceAndDecrementRefCount_guess(PackFsResourceObject_guess *obj)
{
    /* fake match: a plain local is copy-propagated, so the call pushes the field from memory ahead of the
       vtable store; the volatile read and store below keep the original's order: load, store, push */
    void *data;
    int refCount;

    // Volatile accesses pin the order: data load, vtable store, operator delete call.
    data = static_cast<void *volatile &>(obj->data);
    *(const void * volatile *)obj = (const void *)0x0053610C;
    EQ_CDECL(void, 0x005218BB, (void*))(data);
    refCount = obj->refCount;
    refCount--;
    *(const void **)obj = (const void *)0x005360F4;
    obj->refCount = refCount;
    if (refCount == 0 && obj != 0)
    {
        /* fake match: /O1 folds a plain vptr read into the 0x005360F4 just stored, so the vptr is read volatile */
        void **vtable;

        // The slot call passes the vtable in the EDX argument, where the original keeps it for call [edx].
        // The callee is thiscall and ignores EDX.
        vtable = *(void ** volatile *)obj;
        ((void (__fastcall *)(void *, void **, int))vtable[0])(obj, vtable, 1);
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004FB9A0
// Report allocation failure when a required PackFS pointer is null.
// __cdecl (callee pops 0). Returns pointer. When pointer is 0, first calls _verify (0x004F8310, cdecl,
// caller pops 0xC) with pointer, the "..." string and logResult.
int validateNonNullPointer_guess(int pointer, int logResult)
{
    if (pointer == 0)
        EQ_CDECL(void, 0x004F8310, (int, const char *, int))(pointer, (const char *)0x00570968, logResult);
    return pointer;
}

