#include "packfs_shared.h"

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004c4813
// __fastcall (this in ECX, no stack args). Deletes the object when the count hits zero.
void __fastcall decrementReferenceCount_guess(PackFsRefCounted_guess *obj)
{
    obj->refCount--;
    if (obj->refCount == 0 && obj != 0)
        obj->destroy_guess(1);
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004c488e
// __fastcall (this in ECX, no stack args). Releases the referenced object when there is one.
void __fastcall releaseObjectIfNotNull_guess(PackFsRefHandle_guess *handle)
{
    if (handle->object != 0)
        EQ_METHOD(0, void, 0x004C4813, (PackFsRefCounted_guess *), handle->object)();
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004c489a
// Copies a string value while keeping its shared backing storage alive.
// __thiscall, 1 stack arg (the source reference). Passes a copy of the source's shared handle to 0x004DD93D (which
// releases the copy), then copies the value field and returns this.
/* fake match: the copy's destructor never runs here, but declaring it gives VC6 the EBP frame and the copy's frame slot. */
PackFsStringReference_guess *PackFsStringReference_guess::copyStringReference_guess(const PackFsStringReference_guess *source)
{
    EQ_FASTCALL(void, 0x004DD93D, (PackFsStringReference_guess *, PackFsRefCopy_guess))(this, *(const PackFsRefCopy_guess *)source);
    value_guess = source->value_guess;
    return this;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
struct DataHandle_guess {
    NormalizeFilePathDataObject_guess *object;

    // EXTERNAL: TRILOGY 0x004DD808 DataHandle_guess::DataHandle_guess
    DataHandle_guess(int size);

    ~DataHandle_guess()
    {
        if (object != 0)
            EQ_METHOD(0, void, 0x004C4813, (PackFsRefCounted_guess *), object)();
    }
};

struct StringReference_guess {
    PackFsRefCounted_guess *shared;
    const char *value;

    // EXTERNAL: TRILOGY 0x004C4823 StringReference_guess::StringReference_guess
    StringReference_guess(const char *text);

    ~StringReference_guess()
    {
        if (shared != 0)
            EQ_METHOD(0, void, 0x004C4813, (PackFsRefCounted_guess *), shared)();
    }
};

// FUNCTION: TRILOGY 0x004c48cb
// Lowercases a path and collapses repeated separators into single forward slashes.
NormalizeFilePathOwner_guess *NormalizeFilePathOwner_guess::normalizeFilePath_guess(const NormalizeFilePathSource_guess *source)
{
    EQ_METHOD(1, void, 0x004C4823, (PackFsStringReference_guess *, const char *), (PackFsStringReference_guess *)this)(0);

    int length = 0;
    if (source->object != 0)
        length = source->object->length;
    if (length == 0)
        length = 0;
    else
        length = EQ_METHOD(0, int, 0x004DD993, (const NormalizeFilePathSource_guess *), source)() - 1;

    DataHandle_guess normalizedData(length + 1);

    int outputIndex = 0;
    if (source->object != 0)
        length = source->object->length;
    else
        length = 0;
    if (length == 0)
        length = 0;
    else
        length = EQ_METHOD(0, int, 0x004DD993, (const NormalizeFilePathSource_guess *), source)() - 1;

    int inputIndex = 0;
    bool previousWasSeparator = false;
    if (length > 0) {
        do {
            char value = source->object->data[inputIndex];
            if ((value < 'A') || ('Z' < value)) {
                if ((value != '\\') && (value != '/')) {
                    normalizedData.object->data[outputIndex] = value;
                    previousWasSeparator = false;
                    outputIndex += 1;
                } else if (!previousWasSeparator) {
                    normalizedData.object->data[outputIndex] = '/';
                    previousWasSeparator = true;
                    outputIndex += 1;
                }
            } else {
                value = value + ' ';
                normalizedData.object->data[outputIndex] = value;
                previousWasSeparator = false;
                outputIndex += 1;
            }
            inputIndex += 1;
        } while (inputIndex < length);
    }

    normalizedData.object->data[outputIndex] = 0;
    StringReference_guess normalizedString(normalizedData.object->data);
    EQ_METHOD(1, PackFsStringReference_guess *, 0x004C489A, (PackFsStringReference_guess *, const PackFsStringReference_guess *), (PackFsStringReference_guess *)this)((const PackFsStringReference_guess *)&normalizedString);
    return this;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004DD8D4
// Cleans up a byte-buffer object and frees the object when requested.
PackFSData_guess *PackFSData_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x004DD8F0, (PackFSData_guess *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004dd8f0
// Releases the byte allocation held by the buffer object.
// @early-stop: VC6 leaves an extra no-op local cleanup call; its exact emission differs from the original.
PackFSData_guess::~PackFSData_guess()
{
    struct EmptyCleanup_guess {
        PackFSData_guess *savedThis;
        ~EmptyCleanup_guess() {}
    };
    typedef void (__cdecl *DeleteFunction_guess)(void *);
    DeleteFunction_guess deleteFunction = EQ_CDECL(void, 0x005218BB, (void *));

    *(unsigned int *)this = 0x005360DC;
    {
        /* fake match: save this in the EH slot while the no-op cleanup scopes the free. */
        EmptyCleanup_guess cleanup = { this };
        deleteFunction(data);
    }
    *(unsigned int *)this = 0x005360F4;
    EQ_METHOD(0, void, 0x004C4813, (void *), this)();
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004dd932
// __fastcall (this in ECX, no stack args). Sets the vtable pointer, then tail-calls decrementReferenceCount_guess.
void __fastcall initializeWithDefaultHandler_guess(void *self)
{
    *(unsigned int *)self = 0x005360F4;
    EQ_METHOD(0, void, 0x004C4813, (void *), self)();
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004dd93d
// __thiscall, one shared handle passed by value. Replaces the target and releases the copy on return.
// @early-stop: two catch-frame pushes remain; state stores use [EBP-0x14] MOVs instead of [EBP-4] AND/OR.
PackFsRefHandle_guess &PackFsRefHandle_guess::incrementReferenceCount_guess(PackFsRefHandle_guess other)
{
    volatile int cleanupState = 0; /* fake match: model the original EH state transitions */

    try
    {
        if (&other == this)
        {
            cleanupState |= -1;
            return *this;
        }
        else
        {
            if (object != 0)
                EQ_METHOD(0, void, 0x004C4813, (PackFsRefCounted_guess *), object)();
            object = other.object;
            if (other.object != 0)
                other.object->refCount++;
            cleanupState |= -1;
        }
        return *this;
    }
    catch (...)
    {
        // Preserve exception propagation while retaining the original EH frame.
        throw;
    }
}

#undef g_packfs_tree2_nil_guess
#pragma optimize("", on)
// FUNCTION: TRILOGY 0x004dd993
// __fastcall (this in ECX, no stack args). Length of the string, 0 when there is no data.
unsigned int __fastcall CXStr_getStringLength_guess(PackFsString_guess *string)
{
    if (string->data != 0)
        return string->data->length;
    return 0;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x006F03A0)
#pragma optimize("", on)
