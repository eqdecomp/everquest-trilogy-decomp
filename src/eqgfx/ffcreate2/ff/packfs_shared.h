// Shared declarations for the size and speed optimized fast-file objects.
// eqgfx: the same original object as trilogy's src/ffcreate2/ff/packfs.cpp (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
#include <io.h>
#include "trilogy/eqaddr.h"
#include "eqgfx/layout/packfs_tree.h"

#define PACK_FILE_HEADER_MAX_DECODED_SIZE 0x2000
#define EQGFX_PACKFSREFHANDLE_EH_HANDLER 0x10075B60
#define EQGFX_PACKFSREFHANDLE_EH_SETUP 0x10069058
#define EQGFX_PACKFSREFHANDLE_DECREMENT 0x1002190D
#define EQGFX_PACKFSDATAINIT_EH_HANDLER 0x10075B38
#define EQGFX_PACKFSDATAINIT_EH_SETUP 0x10069058
#define EQGFX_PACKFSDATA_CTOR 0x100219BE
#define EQGFX_PACKFSDATAINIT_STRLEN 0x10068160
#define EQGFX_PACKFSDATAINIT_ASSIGN 0x10021AF3
#define EQGFX_PACKFSDATAINIT_COPY 0x10068070
#define EQGFX_NORMALIZEFILEPATH_INIT 0x10021938
#define EQGFX_NORMALIZEFILEPATH_COPY_REFERENCE 0x10021C94
#define EQGFX_NORMALIZEFILEPATH_GET_LENGTH 0x10021E78
#define EQ_NAKED __declspec(naked)
#define EQGFX_PACKFS_DERIVED_VTABLE 0x10076418
#define EQGFX_PACKFS_BASE_VTABLE 0x10076410
#define EQGFX_PACKFS_GET_LIST_HEAD_DATA 0x10052E80
#define EQGFX_PACKFS_FREE_NODE_AND_CHILDREN 0x10054460
#define EQGFX_PACKFS_MAP_ITERATOR_POST_INCREMENT 0x100544A0
#define EQGFX_PACKFS_REMOVE_NODE_FROM_RED_BLACK_TREE 0x10054010
#define EQGFX_PACKFS_FREE_MEMORY 0x10050CBB
#define EQGFX_PACKFS_SHARE_H_PATH 0x100A3B24

struct PackFile_guess {
    char pad0[4];
    int fileHandle;
    char pad8[0x2018];
    long filePosition;
};

// Local layout guess: a handle whose first field points at an object
// that holds a data pointer at +0x8. No header defines these yet.
struct PackFsObject_guess {
    char pad0[8];
    void *data; // 0x8
};

struct PackFsHandle_guess {
    PackFsObject_guess *object; // 0x0
};

// Local layout guess: vtable pointer at 0x0 (slot 0 is a deleting
// destructor taking a flag), reference count at 0x4.
struct PackFsRefCounted_guess {
    virtual void destroy_guess(int flags); // vtable slot 0
    int refCount; // 0x4
};

// Local layout guess: a handle whose first field points at a reference-counted object.
struct PackFsRefHandle_guess {
    PackFsRefCounted_guess *object; // 0x0

    PackFsRefHandle_guess &incrementReferenceCount_guess(PackFsRefCounted_guess *otherObject);
    ~PackFsRefHandle_guess()
    {
        if (object != 0)
            EQ_METHOD(0, void, 0x1002190D, (PackFsRefCounted_guess *), object)();
    }
};

struct PackFsStringReference_guess {
    PackFsRefCounted_guess *shared_guess; // 0x0
    int value_guess; // 0x4
    PackFsStringReference_guess *copyStringReference_guess(const PackFsStringReference_guess *source);
};

// Local layout guess: a by-value copy of a shared handle (the field at 0x0 of a string reference). Copying it
// takes a reference on the object, and its destructor drops that reference. The callee destroys its by-value
// argument, so the destructor never runs in copyStringReference_guess.
struct PackFsRefCopy_guess {
    PackFsRefCounted_guess *object; // 0x0

    inline PackFsRefCopy_guess(const PackFsRefCopy_guess &other)
    {
        object = other.object;
        if (object != 0)
            object->refCount++;
    }
    ~PackFsRefCopy_guess()
    {
        if (object != 0)
            EQ_METHOD(0, void, 0x1002190D, (PackFsRefCounted_guess *), object)();
    }
};

struct NormalizeFilePathDataObject_guess : PackFsRefCounted_guess {
    char *data; // 0x8
    int length; // 0xC
};

struct NormalizeFilePathSource_guess {
    NormalizeFilePathDataObject_guess *object; // 0x0
};

struct NormalizeFilePathOwner_guess {
    NormalizeFilePathOwner_guess *normalizeFilePath_guess(const NormalizeFilePathSource_guess *source);
};

// Local layout guess: PackFSData stores its owned data block at +0x8.
struct PackFSData_guess {
    unsigned int vtable; // 0x0
    int refCount; // 0x4
    void *data; // 0x8
    PackFSData_guess *ScalarDeletingDtor_guess(unsigned char flags);
    ~PackFSData_guess();
};

struct PackFSDataInit_guess {
    PackFSData_guess *storage; // 0x0
    int unknown0x04; // 0x4
};

// Local layout guess: a string handle whose first field points at the
// string data block; the length is at 0xc of that block.
struct PackFsStringData_guess {
    char pad0[0xc];
    unsigned int length; // 0xc
};

struct PackFsString_guess {
    PackFsStringData_guess *data; // 0x0
};

// Global at 0x100FBBFC (undefined4). Not in a header yet; name from TAKP only.
// volatile stops VC6 /O1 folding the zero store into `and [mem], 0`.
#define g_trace_message_buffer_guess EQ_GLOBAL(volatile unsigned int, 0x100FBBFC)

// Local layout guess: a buffered file reader with the file descriptor at
// 0x4 and an 0x2000-byte buffer at 0x8.
struct PackFsReader_guess {
    int unknown0; // 0x0
    int fd; // 0x4
    char buffer[0x2000]; // 0x8
};

// Local layout guess: a table of 0x40 pairs of values (8 bytes each) at
// 0x100FBC10 with its fill count at 0x100FC014. Not in a header yet.
struct PackFsLogEntry_guess {
    unsigned int first; // 0x0
    unsigned int second; // 0x4
};

#define g_packfs_log_count_guess EQ_GLOBAL(unsigned int, 0x100FC014)
#define g_packfs_log_entries_guess EQ_ARRAY(PackFsLogEntry_guess, 0x100FBC10)

// Local layout guess: a holder whose field at 0x4 points at a reference
// counted object (PackFsRefCounted_guess).
struct PackFsRefHolder_guess {
    int unknown0; // 0x0
    PackFsRefCounted_guess *object; // 0x4
};

// The by-value name owns its source reference. Its destructor also preserves
// the inline EH registration present in the speed-optimized removal function.
struct PackFsRemoveFileArgument_guess {
    PackFsRefCounted_guess *source; // 0x0
    int string; // 0x4

    ~PackFsRemoveFileArgument_guess()
    {
        if (source != 0 && --source->refCount == 0 && source != 0)
            source->destroy_guess(1);
    }
};

struct PackFsRemoveSource_guess : PackFsRefCounted_guess {
    int first; // 0x8
    int second; // 0xc
};

struct PackFsRemoveValue_guess : PackFsRefCounted_guess {
    void *child; // 0x8
};

struct PackFsRemoveTreeNode_guess {
    char pad0[0x10];
    PackFsRemoveValue_guess *value; // 0x10
};

struct PackFsRemoveChild_guess {
    char pad0[0xc];
    PackFsRemoveSource_guess *referenceC; // 0xc
    PackFsRemoveSource_guess *reference10; // 0x10
};

struct PackFsRemoveTemporary_guess {
    PackFsRemoveSource_guess *object;
};

typedef void (__stdcall *PackFsCriticalSectionFunction_guess)(void *criticalSection);

// Local layout guess: the file name the loader gets by value (8 bytes). source is the reference counted name object
// (its name pointer at 0x8 and its length at 0xc, see PackFsStringSource_guess); string is the holder's string. The
// destructor releases the source through 0x10021C88, so the callee releases its copy when it returns.
struct PackFsFileNameArgument_guess {
    PackFsRefCounted_guess *source; // 0x0
    int string; // 0x4

    ~PackFsFileNameArgument_guess()
    {
        EQ_FASTCALL(void, 0x10021C88, (PackFsFileNameArgument_guess *))(this);
    }
};

struct PackFS {
    void removeFileSystem_guess(PackFsRemoveFileArgument_guess param_1);
    bool addFileSystem_guess(PackFsFileNameArgument_guess name);
};

// Local layout guess: a string source block (value pair at 0x8 / 0xc) and
// a holder pairing it (0x0) with the string made from it (0x4).
struct PackFsStringSource_guess {
    char pad0[8];
    int first; // 0x8
    int second; // 0xc
};

struct PackFsStringHolder_guess {
    PackFsStringSource_guess *source; // 0x0
    int string; // 0x4
};

// 0x10054B60 createStringFromBytes_guess: __thiscall on a 4-byte temporary, two stack args, returns the string.
#define createStringFromBytes_guess(self, first, second) \
    EQ_METHOD(2, int, 0x10054B60, (int *, int, int), (self))((first), (second))

// Local layout guess: a reference handle (0x0) that is released on destruction
// (0x10021C88), and a pair of two of them. No header defines these yet.
struct PackFsAutoRef_guess {
    PackFsRefCounted_guess *object; // 0x0

    ~PackFsAutoRef_guess() { EQ_METHOD(0, void, 0x10021C88, (PackFsAutoRef_guess *), this)(); }
};

struct PackFsRefPair_guess {
    PackFsAutoRef_guess first; // 0x0
    PackFsAutoRef_guess second; // 0x4

    PackFsRefPair_guess();
};

// Local layout guess: an inlined VC6 std::map constructor (_Tree::_Init). Node: left 0x0, parent 0x4,
// right 0x8, value 0xc.., colour 0x1c (0 red, 1 black); the tree's shared nil node and its reference count
// live at 0x100FBC08/0x100FBC0C. std::_Lockit is called by address (ctor 0x10075A66, dtor 0x10075B02).
struct PackFsMapNode_guess {
    PackFsMapNode_guess *left; // 0x0
    PackFsMapNode_guess *parent; // 0x4
    PackFsMapNode_guess *right; // 0x8
    char value[0x10]; // 0xc
    int color; // 0x1c
};

struct PackFsMap_guess {
    char allocator; // 0x0
    char compare; // 0x1
    PackFsMapNode_guess *head; // 0x4
    char multi; // 0x8
    unsigned int size; // 0xc

    PackFsMap_guess *init_guess(const char *compare, const char *allocator);
    void erase_guess(PackFsMapNode_guess *x);
    void freeNodeAndChildren_guess(PackFsMapNode_guess *node);
};

struct PackFsLockit_guess {
    PackFsLockit_guess() { EQ_METHOD(0, void, 0x10075A66, (PackFsLockit_guess *), this)(); }
    ~PackFsLockit_guess() { EQ_METHOD(0, void, 0x10075B02, (PackFsLockit_guess *), this)(); }
};

#define g_packfs_map_nil_guess EQ_GLOBAL(PackFsMapNode_guess *, 0x100FBC08)
#define g_packfs_map_nilrefs_guess EQ_GLOBAL(int, 0x100FBC0C)

// Local layout guess: the 0x18-byte tree node this function buys: left 0x0, parent 0x4, right 0x8, value 0xc..0x13,
// colour 0x14 (1 for the shared nil node, 0 for the head). The shared nil node pointer is at 0x100FBC04 and its
// reference count at 0x006F0398; both are touched under std::_Lockit (ctor 0x10075A66, dtor 0x10075B02).
struct FlagContainerNode_guess {
    FlagContainerNode_guess *left; // 0x0
    FlagContainerNode_guess *parent; // 0x4
    FlagContainerNode_guess *right; // 0x8
    char value[8]; // 0xc
    int color; // 0x14
};

// Local layout guess: map-shaped (allocator 0x0, compare 0x1, head 0x4, flag byte 0x8, count 0xc) but with the
// 0x18-byte node above, so not PackFsMap_guess. No header defines it yet.
struct FlagContainer_guess {
    char allocator; // 0x0
    char compare; // 0x1
    FlagContainerNode_guess *head; // 0x4
    char flag; // 0x8
    unsigned int size; // 0xc

    FlagContainer_guess *initializeFlagContainer_guess(const char &compare_, const char &allocator_);
};

#define g_flagcontainer_nil_guess EQ_GLOBAL(FlagContainerNode_guess *, 0x100FBC04)
#define g_flagcontainer_nilrefs_guess EQ_GLOBAL(int, 0x006F0398)

// Local layout guess: an STL-style red-black tree (a map keyed by unsigned int).
// The head node's parent (0x4) is the root; the shared nil node is at 0x100FBC04.
struct PackFsTreeIterator_guess {
    PackFsTreeNode_guess *node; // 0x0

    void increment_guess();
};

#define g_packfs_tree_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC04)

// Local layout guess: the tree wrapper searched by insertOrFindNodeInRedBlackTree_guess.
#pragma pack(push, 1)
struct RedBlackTreeFindResult_guess {
    PackFsTreeNode_guess *node; // 0x0
    unsigned char inserted; // 0x4

    RedBlackTreeFindResult_guess(PackFsTreeNode_guess *value, unsigned char flag)
    {
        node = value;
        inserted = flag;
    }
};
#pragma pack(pop)

struct RedBlackTreeLookup_guess {
    int unknown0; // 0x0
    PackFsTreeNode_guess *head; // 0x4
    unsigned char mode; // 0x8
    unsigned int size; // 0xC

    RedBlackTreeFindResult_guess *insertOrFindNodeInRedBlackTree_guess(RedBlackTreeFindResult_guess *result, unsigned int *key);
};

#define g_packfs_red_black_tree_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)

// Local layout guess: a second map instantiation with the same node layout (unsigned int key at 0xc),
// whose shared nil node is at 0x100FBC08.
struct PackFsTree2_guess {
    int unknown0; // 0x0
    PackFsTreeNode_guess *head; // 0x4

    PackFsTreeIterator_guess find_guess(const unsigned int &key);
};

#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)

// Local layout guess for the tree insertion method: head at 0x4 and size at 0xC.
struct PackFsTreeInsert_guess {
    int unknown0; // 0x0
    PackFsTreeNode_guess *head; // 0x4
    int unknown8; // 0x8
    int size; // 0xC

    PackFsTreeIterator_guess *insert_guess(PackFsTreeIterator_guess *result, PackFsTreeNode_guess *searchNode, PackFsTreeNode_guess *parent, unsigned int *key);
};

// Local layout guess: the full 0x18-byte tree node as allocated (the 0x10-byte
// PackFsTreeNode_guess is its head); the allocator sets the parent and the color.
// A __thiscall method that ignores `this`.
struct PackFsTreeAllocator_guess {
    PackFsTreeNode_guess *buyNode_guess(PackFsTreeNode_guess *parent, int color);
};

// Local layout guess: a second STL-style red-black tree whose nodes carry the color at 0x1c
// (0 = red); its shared nil node is at 0x100FBC08. The fields between 0xc and 0x1c are unknown.
struct PackFsTree2Node_guess {
    PackFsTree2Node_guess *left; // 0x0
    PackFsTree2Node_guess *parent; // 0x4
    PackFsTree2Node_guess *right; // 0x8
    char pad0c[0x10];
    int color; // 0x1c
};

struct PackFsTree2Iterator_guess {
    PackFsTree2Node_guess *node; // 0x0

    void decrement_guess();
};

#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)

// Local layout guess: a polymorphic base kept as a plain struct so the vtable pointer values stay the client's
// (0x10076410 for the base, 0x10076418 for the derived). The base destructor is inlined into the derived one;
// it drops a reference and calls the object's slot-0 virtual destroy (flag 1) when the count reaches zero.
struct PackFsRefBase_guess {
    unsigned int vtbl; // 0x0
    int refCount; // 0x4

    ~PackFsRefBase_guess()
    {
        vtbl = 0x10076410;
        if (--refCount == 0 && this != 0)
            ((PackFsRefCounted_guess *)this)->destroy_guess(1);
    }
};

struct PackFsRefDerived_guess : PackFsRefBase_guess {
    PackFsMap_guess *map; // 0x8

    ~PackFsRefDerived_guess();
};

// Favor speed for this function only: after the operator delete call the original cleans up with ADD ESP,4,
// where the other deleting destructors in this file use POP ECX (favor size). The setting is restored after.

// Local layout guess: a holder whose pointer at 0x4 points at a dword value (the first dword is read).
struct PackFsValueHolder_guess {
    char pad0[4];
    unsigned int *target; // 0x4

    unsigned int *copyTargetValue_guess(unsigned int *out);
};

// Local layout guess: a map iterator is the current node pointer alone (0x0).
struct PackFsMapIterator_guess {
    PackFsMapNode_guess *node; // 0x0

    PackFsMapIterator_guess *postIncrement_guess(PackFsMapIterator_guess *result, int);
};

struct PackFsRefValue_guess;

// Local layout guess: a shared pointer to a reference counted object.
struct PackFsSharedPtr_guess {
    PackFsRefCounted_guess *ptr; // 0x0

    PackFsSharedPtr_guess &updateSharedPointer_guess(PackFsRefValue_guess other);
    PackFsSharedPtr_guess *incrementReferenceCount_guess(const PackFsSharedPtr_guess *other);
};

// Local layout guess: a shared pointer passed by value. Its destructor drops the reference the copy holds,
// so the callee releases the copy when it returns. That destructor is what gives the function its EH frame.
struct PackFsRefValue_guess {
    PackFsRefCounted_guess *ptr; // 0x0

    ~PackFsRefValue_guess()
    {
        if (ptr != 0)
        {
            ptr->refCount--;
            if (ptr->refCount == 0)
                ptr->destroy_guess(1);
        }
    }
};

// Favor speed for this function only: the original writes its EH prolog out (push -1, push handler, push eax,
// mov fs:[0],esp) and cleans up with ADD ESP,0xc. Favor size calls __EH_prolog, uses an EBP frame and does not
// expand the destructor inline.

// Local layout guess: a key (opaque 4 bytes) paired with a reference counted value.
struct PackFsKeyValue_guess {
    unsigned int key; // 0x0
    PackFsRefCounted_guess *value; // 0x4
};

// Local layout guess: four plain dwords.
struct PackFsQuad_guess {
    unsigned int a; // 0x0
    unsigned int b; // 0x4
    unsigned int c; // 0x8
    unsigned int d; // 0xc
};

// Local layout guess: opaque object, only its address is used.
struct SuiteDefaultsElement_guess {
    SuiteDefaultsElement_guess *ScalarDeletingDtor_guess(unsigned char flags);
};

// Favor speed for this function only: after the operator delete call the original cleans up with ADD ESP,4,
// where the other deleting destructors in this file use POP ECX (favor size). The setting is restored after.

// Local layout guess: a reference counted object with a vtable pointer (the original's vtable is at
// 0x10076420; kept as a plain pointer so the stored address is the original's), the count at 0x4 and a
// held value at 0x8.
struct PackFsSharedPointer2_guess {
    const void *vtable; // 0x0
    int refCount; // 0x4
    int value; // 0x8

    PackFsSharedPointer2_guess(int value_arg);
    ~PackFsSharedPointer2_guess();
    PackFsSharedPointer2_guess *ScalarDeletingDtor_guess(unsigned char flags);
};

// Favor speed for this function only, like the destructor above: the operator delete call is cleaned up with
// ADD ESP,4 (the other deleting destructors in this file use POP ECX, favor size). The setting is restored after.

// Local layout guess for the shared data released by this wrapper.
struct PackFsSharedPointerStorage_guess {
    PackFsRefCounted_guess *base; // 0x0
    int unknown; // 0x4
    PackFsRefCounted_guess *first; // 0x8
    PackFsRefCounted_guess *second; // 0xc
    PackFsRefCounted_guess *third; // 0x10
};

// EH marker for the destructor below: an empty destructible object that holds the two pointers the frame keeps.
// File scope on purpose (a local class's inline destructor is called out of line).
struct PackFsEHMarker_guess {
    PackFsSharedPointer2_guess *self; // 0x0
    PackFsSharedPointerStorage_guess *storage; // 0x4
    ~PackFsEHMarker_guess() {}
};

// Favor speed for this function only: the operator delete call is cleaned up with ADD ESP,4 and the EH prolog is
// written out. The setting is restored after.

// Local layout guess: like PackFsSharedPointer2_guess, with a second held value at 0xc and the vtable at
// 0x10076428.
struct PackFsSharedPointerData_guess {
    const void *vtable; // 0x0
    int refCount; // 0x4
    int first; // 0x8
    int second; // 0xc

    PackFsSharedPointerData_guess *initializeSharedPointerWithData_guess(int first_arg, int second_arg);
};

// Local layout guess: the 0x14-byte object the loader allocates per file: the name reference at 0x0 (the holder's
// source and string), the file map's shared pointer at 0x8 and two reference handles at 0xc and 0x10.
struct PackFsFileData_guess {
    PackFsStringReference_guess name; // 0x0
    PackFsSharedPtr_guess map; // 0x8
    PackFsRefHandle_guess unknown0xC; // 0xc
    PackFsRefHandle_guess unknown0x10; // 0x10
};

// Local layout guess: one 12-byte directory entry of a file; the loader copies it into the map node its key finds.
struct PackFsFileEntry_guess {
    unsigned int first; // 0x0
    unsigned int second; // 0x4
    unsigned int third; // 0x8
};

// Local layout guess: a tree lookup result, the node it found (0x0) and the word after it (0x4).
struct PackFsLookupResult_guess {
    PackFsMapNode_guess *node; // 0x0
    unsigned int inserted; // 0x4
};

// Local layout guess: PackFsRefCounted_guess (vtable at 0x0, refCount at 0x4) plus the data pointer
// that the destroy path frees. No header defines the 0x8 field yet.
struct PackFsResourceObject_guess : PackFsRefCounted_guess {
    void *data; // 0x8
};

// Local stand-in for the class that owns the destructor at 0x10054830. VC6 only accepts __thiscall on a
// member, so the function is one. No fields: the layout is PackFsResourceObject_guess's.
struct ResourceObject_guess {
    ResourceObject_guess *ResourceObject_DestroyAndFreeIfRequested_destructor_guess(unsigned char flags);
};

// Favor speed for this function only: after the operator delete call the original cleans up with ADD ESP,4,
// where the other deleting destructors in this file use POP ECX (favor size). The setting is restored after.

// Favor speed for this function too: the original cleans up the operator delete argument with ADD ESP,4.

// EH marker for the destructor below: a destructible object that holds `this`. File scope on purpose (a local class's
// inline destructor is called out of line).
struct PackFSDataEHScope_guess {
    PackFSData_guess *self;
    ~PackFSDataEHScope_guess() {}
};

// .rdata vtable of PackFSData_guess (slot 0 is ScalarDeletingDtor, 0x10021A8A).
#define EQGFX_PACKFSDATA_VTABLE 0x10076400

// .rdata slot holding the ___purecall address; the object's vtable is pointed at it (slot 0 aborts).
#define g_purecall_vtable_guess EQ_GLOBAL(void *, 0x10076410)

// The lock object is only passed through; no header defines its layout yet, and this function reads no field.
struct PackFsSoundLock_guess {
    char unused; // the flag and the CRITICAL_SECTION are globals, not fields
};

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x100FBC08)

#define PACKFS_TREE_RED 0
#define PACKFS_TREE_BLACK 1
#define PACKFS_TREE_COLOR(treeNode) (((PackFsTreeNode_guess *)(treeNode))->color_int_guess)

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x100FBC08)

#define PACKFS_MAP_RED 0
#define PACKFS_MAP_BLACK 1

