#include "packfs_shared.h"

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x1002190d
// Ends a direct shared-object reference and destroys the object when no owners remain.
// Cloned from trilogy decrementReferenceCount_guess 0x004C4813 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args). Deletes the object when the count hits zero.
void __fastcall decrementReferenceCount_guess(PackFsRefCounted_guess *obj)
{
    obj->refCount--;
    if (obj->refCount == 0 && obj != 0)
        obj->destroy_guess(1);
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021938
extern "C" EQ_NAKED PackFSDataInit_guess *__fastcall initializePackFSData_guess(
    PackFSDataInit_guess *object, int edx_dummy, const char *name)
{
    __asm {
        mov eax, EQGFX_PACKFSDATAINIT_EH_HANDLER
        mov edx, EQGFX_PACKFSDATAINIT_EH_SETUP
        call edx
        push ecx
        push ecx
        push esi
        push edi
        xor edi, edi
        mov esi, ecx
        push edi
        mov dword ptr [ebp-10h], esi
        mov edx, EQGFX_PACKFSDATA_CTOR
        call edx
        cmp dword ptr [ebp+8], edi
        mov dword ptr [ebp-4], edi
        mov dword ptr [esi+4], edi
        jz no_name
        push dword ptr [ebp+8]
        mov edx, EQGFX_PACKFSDATAINIT_STRLEN
        call edx
        cmp eax, edi
        pop ecx
        jz no_name
        push ecx
        inc eax
        mov ecx, esp
        mov dword ptr [ebp-14h], esp
        push eax
        mov edx, EQGFX_PACKFSDATA_CTOR
        call edx
        mov ecx, esi
        mov edx, EQGFX_PACKFSDATAINIT_ASSIGN
        call edx
        push dword ptr [ebp+8]
        mov eax, dword ptr [esi]
        push dword ptr [eax+8]
        mov edx, EQGFX_PACKFSDATAINIT_COPY
        call edx
        pop ecx
        mov dword ptr [esi+4], edi
        pop ecx
    no_name:
        mov ecx, dword ptr [ebp-0Ch]
        mov eax, esi
        pop edi
        mov dword ptr fs:[0], ecx
        pop esi
        leave
        ret 4
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021A8A
// Deletes the allocated wrapper for a packed-file data resource.
// Cloned from trilogy PackFSData_guess::ScalarDeletingDtor_guess 0x004DD8D4 (the same original code: BinDiff sibling, names/siblings/).
PackFSData_guess *PackFSData_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x10021AA6, (PackFSData_guess *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10050CBB, (void *))(this);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021AA6
// Releases the byte buffer behind a packed-file data resource, then drops its base ownership.
PackFSData_guess::~PackFSData_guess()
{
    {
        PackFSDataEHScope_guess eh_scope = { this };
        vtable = EQGFX_PACKFSDATA_VTABLE;
        EQ_CDECL_CPP(void, EQGFX_PACKFS_FREE_MEMORY, (void *))(data);
    }
    vtable = EQGFX_PACKFS_BASE_VTABLE;
    EQ_METHOD(0, void, 0x1002190D, (PackFsRefCounted_guess *), (PackFsRefCounted_guess *)this)();
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021ae8
// Retires a base resource object before releasing its shared ownership.
// __fastcall (this in ECX, no stack args); returns nothing. Points the object's vtable slot at the .rdata
// table at 0x10076410 (its slot 0 is ___purecall), then tail-jumps to 0x1002190D, the reference-count decrement.
void __fastcall setPurecallVtableAndDecrementReferenceCount_guess(PackFsRefCounted_guess *object)
{
    *(unsigned int *)object = (unsigned int)&g_purecall_vtable_guess;
    EQ_METHOD(0, void, 0x1002190D, (PackFsRefCounted_guess *), object)();
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021af3
// Assigns a shared object handle, releasing its previous object and the incoming temporary reference.
EQ_NAKED PackFsRefHandle_guess &PackFsRefHandle_guess::incrementReferenceCount_guess(PackFsRefCounted_guess *otherObject)
{
    __asm {
        mov eax, EQGFX_PACKFSREFHANDLE_EH_HANDLER
        mov edx, EQGFX_PACKFSREFHANDLE_EH_SETUP
        call edx
        push esi
        mov esi, ecx
        and dword ptr [ebp-4], 0
        lea eax, [ebp+8]
        cmp eax, esi
        jnz assign_other
        or dword ptr [ebp-4], 0FFFFFFFFh
        mov ecx, dword ptr [ebp+8]
        jmp release_other
    assign_other:
        mov ecx, dword ptr [esi]
        test ecx, ecx
        jz old_object_released
        mov edx, EQGFX_PACKFSREFHANDLE_DECREMENT
        call edx
    old_object_released:
        mov ecx, dword ptr [ebp+8]
        test ecx, ecx
        mov dword ptr [esi], ecx
        jz new_object_assigned
        inc dword ptr [ecx+4]
    new_object_assigned:
        or dword ptr [ebp-4], 0FFFFFFFFh
    release_other:
        test ecx, ecx
        jz function_return
        mov edx, EQGFX_PACKFSREFHANDLE_DECREMENT
        call edx
    function_return:
        mov ecx, dword ptr [ebp-0Ch]
        mov eax, esi
        pop esi
        mov dword ptr fs:[0], ecx
        leave
        ret 4
    }
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021c88
// Releases the optional object held by a fast-file handle.
// Cloned from trilogy releaseObjectIfNotNull_guess 0x004C488E (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args). Releases the referenced object when there is one.
void __fastcall releaseObjectIfNotNull_guess(PackFsRefHandle_guess *handle)
{
    if (handle->object != 0)
        EQ_METHOD(0, void, 0x1002190D, (PackFsRefCounted_guess *), handle->object)();
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021c94
// Copies a shared string reference used by the pack-file system.
// Cloned from trilogy PackFsStringReference_guess::copyStringReference_guess 0x004C489A (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 1 stack arg (the source reference). Passes a copy of the source's shared handle to 0x10021AF3 (which
// releases the copy), then copies the value field and returns this.
/* fake match: the copy's destructor never runs here, but declaring it gives VC6 the EBP frame and the copy's frame slot. */
PackFsStringReference_guess *PackFsStringReference_guess::copyStringReference_guess(const PackFsStringReference_guess *source)
{
    EQ_FASTCALL(void, 0x10021AF3, (PackFsStringReference_guess *, PackFsRefCopy_guess))(this, *(const PackFsRefCopy_guess *)source);
    value_guess = source->value_guess;
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
struct DataHandle_guess {
    NormalizeFilePathDataObject_guess *object;

    DataHandle_guess(int size)
    {
        EQ_METHOD(1, void, EQGFX_PACKFSDATA_CTOR,
            (PackFSData_guess *, int), (PackFSData_guess *)this)(size);
    }

    ~DataHandle_guess()
    {
        if (object != 0)
            EQ_METHOD(0, void, EQGFX_PACKFSREFHANDLE_DECREMENT,
                (PackFsRefCounted_guess *), object)();
    }
};

struct StringReference_guess {
    PackFsRefCounted_guess *shared;
    const char *value;

    // EXTERNAL: EQGFX 0x10021938 StringReference_guess::StringReference_guess
    StringReference_guess(const char *text);

    ~StringReference_guess()
    {
        if (shared != 0)
            EQ_METHOD(0, void, EQGFX_PACKFSREFHANDLE_DECREMENT,
                (PackFsRefCounted_guess *), shared)();
    }
};

// FUNCTION: EQGFX 0x10021D55
// Normalizes file paths to lowercase with forward slashes and collapses consecutive separators.
NormalizeFilePathOwner_guess *NormalizeFilePathOwner_guess::normalizeFilePath_guess(
    const NormalizeFilePathSource_guess *source)
{
    EQ_METHOD(1, PackFSDataInit_guess *, EQGFX_NORMALIZEFILEPATH_INIT,
        (PackFSDataInit_guess *, const char *), (PackFSDataInit_guess *)this)(0);

    int length = 0;
    if (source->object != 0)
        length = source->object->length;
    if (length == 0)
        length = 0;
    else
        length = EQ_METHOD(0, int, EQGFX_NORMALIZEFILEPATH_GET_LENGTH,
            (const NormalizeFilePathSource_guess *), source)() - 1;

    DataHandle_guess normalizedData(length + 1);

    int outputIndex = 0;
    if (source->object != 0)
        length = source->object->length;
    else
        length = 0;
    if (length == 0)
        length = 0;
    else
        length = EQ_METHOD(0, int, EQGFX_NORMALIZEFILEPATH_GET_LENGTH,
            (const NormalizeFilePathSource_guess *), source)() - 1;

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
    EQ_METHOD(1, PackFsStringReference_guess *, EQGFX_NORMALIZEFILEPATH_COPY_REFERENCE,
        (PackFsStringReference_guess *, const PackFsStringReference_guess *),
        (PackFsStringReference_guess *)this)((const PackFsStringReference_guess *)&normalizedString);
    return this;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10021e78
// Supplies the character count used while preparing a string name.
// Cloned from trilogy CXStr_getStringLength_guess 0x004DD993 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args). Length of the string, 0 when there is no data.
unsigned int __fastcall CXStr_getStringLength_guess(PackFsString_guess *text)
{
    if (text->data != 0)
        return text->data->length;
    return 0;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
// FUNCTION: EQGFX 0x10054010
// Erases one node from the PackFS map of 16-byte inline values; nothing is released but the node itself
// __thiscall (callee pops 8), written as __fastcall with an unused EDX. Erases `where` from the tree; *out = its successor.
void __fastcall removeNodeFromRedBlackTree_guess(PackFsMap_guess *self, int /* EDX unused */,
    PackFsMapNode_guess **out, PackFsMapNode_guess *where)
{
    PackFsMapNode_guess *erased;
    PackFsMapNode_guess *child;
    PackFsMapNode_guess *spliced;
    PackFsMapNode_guess *header;
    PackFsMapNode_guess *extreme;
    PackFsMapNode_guess *node;
    PackFsMapNode_guess *parent;
    PackFsMapNode_guess *sibling;
    PackFsMapNode_guess *nephew;
    PackFsMapNode_guess *grandparent;
    PackFsMapNode_guess *nilNode;
    int colour;

    erased = where;
    EQ_METHOD(0, void, 0x10054410, (PackFsMapNode_guess **), &where)();
    child = erased->left;
    nilNode = g_packfs_map_nil_guess;
    spliced = erased;
    if (child == nilNode)
    {
        child = erased->right;
    }
    else if (erased->right != nilNode)
    {
        spliced = erased->right;
        while (spliced->left != nilNode)
            spliced = spliced->left;
        child = spliced->right;
    }
    {
        PackFsLockit_guess lock;

        if (spliced == erased)
        {
            child->parent = erased->parent;
            header = self->head;
            if (header->parent == erased)
                header->parent = child;
            else if (erased->parent->left == erased)
                erased->parent->left = child;
            else
                erased->parent->right = child;
            header = self->head;
            if (header->left == erased)
            {
                if (erased->right == g_packfs_map_nil_guess)
                {
                    header->left = erased->parent;
                }
                else
                {
                    extreme = child;
                    for (node = child->left; node != g_packfs_map_nil_guess; node = node->left)
                        extreme = node;
                    header->left = extreme;
                }
            }
            if (header->right == erased)
            {
                if (erased->left == g_packfs_map_nil_guess)
                {
                    extreme = erased->parent;
                }
                else
                {
                    extreme = child;
                    for (node = child->right; node != g_packfs_map_nil_guess; node = node->right)
                        extreme = node;
                }
                header->right = extreme;
            }
        }
        else
        {
            erased->left->parent = spliced;
            spliced->left = erased->left;
            if (spliced == erased->right)
            {
                child->parent = spliced;
            }
            else
            {
                child->parent = spliced->parent;
                spliced->parent->left = child;
                spliced->right = erased->right;
                erased->right->parent = spliced;
            }
            header = self->head;
            if (header->parent == erased)
                header->parent = spliced;
            else if (erased->parent->left == erased)
                erased->parent->left = spliced;
            else
                erased->parent->right = spliced;
            spliced->parent = erased->parent;
            colour = spliced->color;
            spliced->color = erased->color;
            erased->color = colour;
        }
        if (erased->color == PACKFS_MAP_BLACK)
        {
            if (child != header->parent)
            {
                do
                {
                    if (child->color != PACKFS_MAP_BLACK)
                        break;
                    parent = child->parent;
                    if (child == parent->left)
                    {
                        sibling = parent->right;
                        if (sibling->color == PACKFS_MAP_RED)
                        {
                            sibling->color = PACKFS_MAP_BLACK;
                            parent->color = PACKFS_MAP_RED;
                            parent->right = sibling->left;
                            if (sibling->left != g_packfs_map_nil_guess)
                                sibling->left->parent = parent;
                            sibling->parent = parent->parent;
                            if (parent == header->parent)
                            {
                                header->parent = sibling;
                            }
                            else
                            {
                                grandparent = parent->parent;
                                if (parent == grandparent->left)
                                    grandparent->left = sibling;
                                else
                                    grandparent->right = sibling;
                            }
                            sibling->left = parent;
                            parent->parent = sibling;
                            sibling = parent->right;
                        }
                        if (sibling->left->color != PACKFS_MAP_BLACK || sibling->right->color != PACKFS_MAP_BLACK)
                        {
                            if (sibling->right->color == PACKFS_MAP_BLACK)
                            {
                                nephew = sibling->left;
                                nephew->color = PACKFS_MAP_BLACK;
                                sibling->color = PACKFS_MAP_RED;
                                sibling->left = nephew->right;
                                if (nephew->right != g_packfs_map_nil_guess)
                                    nephew->right->parent = sibling;
                                nephew->parent = sibling->parent;
                                if (sibling == header->parent)
                                {
                                    header->parent = nephew;
                                }
                                else
                                {
                                    grandparent = sibling->parent;
                                    if (sibling == grandparent->right)
                                        grandparent->right = nephew;
                                    else
                                        grandparent->left = nephew;
                                }
                                nephew->right = sibling;
                                sibling->parent = nephew;
                                sibling = parent->right;
                            }
                            sibling->color = parent->color;
                            parent->color = PACKFS_MAP_BLACK;
                            sibling->right->color = PACKFS_MAP_BLACK;
                            parent->right = sibling->left;
                            if (sibling->left != g_packfs_map_nil_guess)
                                sibling->left->parent = parent;
                            sibling->parent = parent->parent;
                            if (parent == header->parent)
                            {
                                header->parent = sibling;
                            }
                            else
                            {
                                grandparent = parent->parent;
                                if (parent == grandparent->left)
                                    grandparent->left = sibling;
                                else
                                    grandparent->right = sibling;
                            }
                            sibling->left = parent;
                            parent->parent = sibling;
                            break;
                        }
                    }
                    else
                    {
                        sibling = parent->left;
                        if (sibling->color == PACKFS_MAP_RED)
                        {
                            sibling->color = PACKFS_MAP_BLACK;
                            parent->color = PACKFS_MAP_RED;
                            parent->left = sibling->right;
                            if (sibling->right != g_packfs_map_nil_guess)
                                sibling->right->parent = parent;
                            sibling->parent = parent->parent;
                            if (parent == header->parent)
                            {
                                header->parent = sibling;
                            }
                            else
                            {
                                grandparent = parent->parent;
                                if (parent == grandparent->right)
                                    grandparent->right = sibling;
                                else
                                    grandparent->left = sibling;
                            }
                            sibling->right = parent;
                            parent->parent = sibling;
                            sibling = parent->left;
                        }
                        if (sibling->right->color != PACKFS_MAP_BLACK || sibling->left->color != PACKFS_MAP_BLACK)
                        {
                            if (sibling->left->color == PACKFS_MAP_BLACK)
                            {
                                nephew = sibling->right;
                                nephew->color = PACKFS_MAP_BLACK;
                                sibling->color = PACKFS_MAP_RED;
                                sibling->right = nephew->left;
                                if (nephew->left != g_packfs_map_nil_guess)
                                    nephew->left->parent = sibling;
                                nephew->parent = sibling->parent;
                                if (sibling == header->parent)
                                {
                                    header->parent = nephew;
                                }
                                else
                                {
                                    grandparent = sibling->parent;
                                    if (sibling == grandparent->left)
                                        grandparent->left = nephew;
                                    else
                                        grandparent->right = nephew;
                                }
                                nephew->left = sibling;
                                sibling->parent = nephew;
                                sibling = parent->left;
                            }
                            sibling->color = parent->color;
                            parent->color = PACKFS_MAP_BLACK;
                            sibling->left->color = PACKFS_MAP_BLACK;
                            parent->left = sibling->right;
                            if (sibling->right != g_packfs_map_nil_guess)
                                sibling->right->parent = parent;
                            sibling->parent = parent->parent;
                            if (parent == header->parent)
                            {
                                header->parent = sibling;
                            }
                            else
                            {
                                grandparent = parent->parent;
                                if (parent == grandparent->right)
                                    grandparent->right = sibling;
                                else
                                    grandparent->left = sibling;
                            }
                            sibling->right = parent;
                            parent->parent = sibling;
                            break;
                        }
                    }
                    sibling->color = PACKFS_MAP_RED;
                    child = child->parent;
                } while (child != header->parent);
            }
            child->color = PACKFS_MAP_BLACK;
        }
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(erased);
    self->size--;
    *out = where;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)
#pragma optimize("", on)
