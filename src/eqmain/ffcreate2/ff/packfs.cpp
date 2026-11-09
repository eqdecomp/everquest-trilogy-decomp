// eqmain: the same original object as trilogy's src/ffcreate2/ff/packfs.cpp (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
#include <io.h>
#include <string.h>
#include "trilogy/eqaddr.h"
#include "eqmain/layout/packfs_tree.h"

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

    PackFsRefCounted_guess *scalarDeletingDtor_guess(unsigned char flags);
};

// Local layout guess: a handle whose first field points at a reference-counted object.
struct PackFsRefHandle_guess {
    PackFsRefCounted_guess *object; // 0x0

    PackFsRefHandle_guess &incrementReferenceCount_guess(PackFsRefHandle_guess other);
    ~PackFsRefHandle_guess()
    {
        if (object != 0)
            EQ_METHOD(0, void, 0x10007520, (PackFsRefCounted_guess *), object)();
    }
};

struct PackFsStringReference_guess {
    PackFsRefCounted_guess *shared_guess; // 0x0
    int value_guess; // 0x4
    PackFsStringReference_guess *initializeStringReferenceWithString_guess(const char *str);
    PackFsStringReference_guess *copyStringReference_guess(const PackFsStringReference_guess *source);
};

// Local layout guess: PackFSData stores its owned data block at +0x8.
struct PackFSData_guess {
    unsigned int vtable; // 0x0
    int refCount; // 0x4
    void *data; // 0x8
    PackFSData_guess *ScalarDeletingDtor_guess(unsigned char flags);
    ~PackFSData_guess();
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

// Local layout guess: a buffered file reader with the file descriptor at
// 0x4 and an 0x2000-byte buffer at 0x8.
struct PackFsReader_guess {
    int unknown0; // 0x0
    int fd; // 0x4
    char buffer[0x2000]; // 0x8
};

// Local layout guess: a table of 0x40 pairs of values (8 bytes each) at
// 0x006EFF58 with its fill count at 0x006F035C. Not in a header yet.
struct PackFsLogEntry_guess {
    unsigned int first; // 0x0
    unsigned int second; // 0x4
};

#define g_packfs_log_count_guess EQ_GLOBAL(unsigned int, 0x006F035C)
#define g_packfs_log_entries_guess EQ_ARRAY(PackFsLogEntry_guess, 0x006EFF58)

// Local layout guess: a holder whose field at 0x4 points at a reference
// counted object (PackFsRefCounted_guess).
struct PackFsRefHolder_guess {
    int unknown0; // 0x0
    PackFsRefCounted_guess *object; // 0x4
};

struct PackFsRemoveFileArgument_guess {
    PackFsRefCounted_guess *source; // 0x0
    int string; // 0x4

    // The callee owns the by-value argument: its destructor drops the source reference when the function returns.
    ~PackFsRemoveFileArgument_guess()
    {
        PackFsRefCounted_guess *obj = source;

        if (obj != 0 && --obj->refCount == 0 && obj != 0)
            obj->destroy_guess(1);
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

struct PackFS {
    void removeFileSystem_guess(PackFsRemoveFileArgument_guess param_1);
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

// 0x1000E5B0 createStringFromBytes_guess: __thiscall on a 4-byte temporary, two stack args, returns the string.
#define createStringFromBytes_guess(self, first, second) \
    EQ_METHOD(2, int, 0x1000E5B0, (int *, int, int), (self))((first), (second))

// Local layout guess: a reference handle (0x0) that is released on destruction
// (0x004C488E), and a pair of two of them. No header defines these yet.
struct PackFsAutoRef_guess {
    PackFsRefCounted_guess *object; // 0x0

    ~PackFsAutoRef_guess() { EQ_METHOD(0, void, 0x004C488E, (PackFsAutoRef_guess *), this)(); }
};

struct PackFsRefPair_guess {
    PackFsAutoRef_guess first; // 0x0
    PackFsAutoRef_guess second; // 0x4

    PackFsRefPair_guess();
};

// Local layout guess: an inlined VC6 std::map constructor (_Tree::_Init). Node: left 0x0, parent 0x4,
// right 0x8, value 0xc.., colour 0x1c (0 red, 1 black); the tree's shared nil node and its reference count
// live at 0x1004CF18/0x1004CF1C. std::_Lockit is called by address (ctor 0x1003E980, dtor 0x1003EA1C).
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

    PackFsMap_guess *init_guess(const char &compare, const char &allocator);
    void erase_guess(PackFsMapNode_guess *x);
};

struct PackFsLockit_guess {
    PackFsLockit_guess() { EQ_METHOD(0, void, 0x1003E980, (PackFsLockit_guess *), this)(); }
    ~PackFsLockit_guess() { EQ_METHOD(0, void, 0x1003EA1C, (PackFsLockit_guess *), this)(); }
};

#define g_packfs_map_nil_guess EQ_GLOBAL(PackFsMapNode_guess *, 0x1004CF18)
#define g_packfs_map_nilrefs_guess EQ_GLOBAL(int, 0x1004CF1C)

// Local layout guess: an STL-style red-black tree (a map keyed by unsigned int).
// The head node's parent (0x4) is the root; the shared nil node is at 0x1004CF14.
struct PackFsTreeIterator_guess {
    PackFsTreeNode_guess *node; // 0x0

    void increment_guess();
};

#define g_packfs_tree_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x1004CF14)

// Local layout guess: the tree wrapper searched by FindOrInsertNodeInRedBlackTree_guess.
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

    RedBlackTreeFindResult_guess FindOrInsertNodeInRedBlackTree_guess(unsigned int *key);
};

#define g_packfs_red_black_tree_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x1004CF18)

// Local layout guess: a second map instantiation with the same node layout (unsigned int key at 0xc),
// whose shared nil node is at 0x1004CF18.
struct PackFsTree2_guess {
    int unknown0; // 0x0
    PackFsTreeNode_guess *head; // 0x4

    PackFsTreeIterator_guess find_guess(const unsigned int &key);
};

#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x1004CF18)

// Local layout guess for the tree insertion method: head at 0x4 and size at 0xC.
struct PackFsTreeInsert_guess {
    int unknown0; // 0x0
    int *head; // 0x4
    int unknown8; // 0x8
    int size; // 0xC

    int insert_guess(PackFsTreeIterator_guess *param_2, int param_3, int *param_4, unsigned int *param_5);
};

// Local layout guess: the full 0x18-byte tree node as allocated (the 0x10-byte
// PackFsTreeNode_guess is its head); the allocator sets the parent and the color.
// A __thiscall method that ignores `this`.
struct PackFsTreeAllocator_guess {
    PackFsTreeNode_guess *buyNode_guess(PackFsTreeNode_guess *parent, int color);
};

// Local layout guess: a second STL-style red-black tree whose nodes carry the color at 0x1c
// (0 = red); its shared nil node is at 0x1004CF18. The fields between 0xc and 0x1c are unknown.
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

#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x1004CF18)

// Local layout guess: a shared pointer to a reference counted object.
struct PackFsSharedPtr_guess {
    PackFsRefCounted_guess *ptr; // 0x0

    PackFsSharedPtr_guess &updateSharedPointer_guess(PackFsSharedPtr_guess other);
    PackFsSharedPtr_guess *incrementReferenceCount_guess(const PackFsSharedPtr_guess *other);
};

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

// Local layout guess: a reference counted object with a vtable pointer (the original's vtable is at
// 0x10041F60; kept as a plain pointer so the stored address is the original's), the count at 0x4 and a
// held value at 0x8.
struct PackFsSharedPointer2_guess {
    const void *vtable; // 0x0
    int refCount; // 0x4
    int value; // 0x8

    PackFsSharedPointer2_guess(int value_arg);
    ~PackFsSharedPointer2_guess();
    PackFsSharedPointer2_guess *ScalarDeletingDtor_guess(unsigned char flags);
};

// Local layout guess for the shared data released by this wrapper.
struct PackFsSharedPointerStorage_guess {
    PackFsRefCounted_guess *base; // 0x0
    int unknown; // 0x4
    PackFsRefCounted_guess *first; // 0x8
    PackFsRefCounted_guess *second; // 0xc
    PackFsRefCounted_guess *third; // 0x10
};

// Local layout guess: like PackFsSharedPointer2_guess, with a second held value at 0xc and the vtable at
// 0x10041F68.
struct PackFsSharedPointerData_guess {
    const void *vtable; // 0x0
    int refCount; // 0x4
    int first; // 0x8
    int second; // 0xc

    PackFsSharedPointerData_guess *initializeSharedPointerWithData_guess(int first_arg, int second_arg);
};

// FUNCTION: EQMAIN 0x10007520
// Releases a shared front-end object when its last owner is done.
// __fastcall (this in ECX, no stack args), no return value. Decrements the count at +0x4; when it reaches zero and
// the object is not null, calls its vtable slot 0 (the deleting destructor) with 1.
void __fastcall decrementReferenceCount_guess(PackFsRefCounted_guess *obj)
{
    obj->refCount--;
    if (obj->refCount == 0 && obj != 0)
        obj->destroy_guess(1);
}

// FUNCTION: EQMAIN 0x10007540
// Creates a shared front-end string value from text.
// __thiscall member method, one stack arg (a C string), RET 4. Returns this. Clears the shared pointer at 0x0 and the value
// at 0x4. For a non-empty string: allocates a buffer of strlen+1 bytes and a 0x10-byte object
// (vtable 0x10040df4, count at +0x4, buffer at +0x8, size at +0xc), holds the object in a temporary shared pointer with one
// reference, passes that to updateSharedPointer_guess (0x10007FE0) by value, copies the string with its NUL into the buffer
// of the shared pointer now in self, then clears the value. Bugs kept: the buffer is not checked for NULL (the copy then
// writes through NULL), and when the object allocation fails the NULL object's count (+0x4) is incremented anyway: an
// access violation there.
// @early-stop: The /O2 EH prolog and common exit remain unmatched; saved-register order and stack argument placement differ.
PackFsStringReference_guess *PackFsStringReference_guess::initializeStringReferenceWithString_guess(const char *str)
{
    PackFsSharedPtr_guess temp;
    /* fake match: keep the by-value shared-pointer temporary in a stack slot like VC6's original */
    PackFsSharedPtr_guess * volatile tempPointer;
    PackFsSharedPointerData_guess *object;
    unsigned int len;
    unsigned int size;
    void *buffer;

    shared_guess = 0;
    value_guess = 0;
    temp.ptr = 0;
    if (str != 0)
    {
        len = strlen(str);
        if (len != 0)
        {
            tempPointer = &temp;
            size = len + 1;
            if (size != 0)
            {
                buffer = EQ_CDECL(void *, 0x100338E5, (unsigned int))(size);
                object = (PackFsSharedPointerData_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x10);
                if (object != 0)
                {
                    object->refCount = 0;
                    object->first = (int)buffer;
                    object->second = (int)size;
                    object->vtable = (const void *)0x10040DF4;
                }
                else
                    object = 0;
                tempPointer->ptr = (PackFsRefCounted_guess *)object;
                object->refCount++;
            }
            EQ_METHOD(1, void, 0x10007FE0, (PackFsSharedPtr_guess *, PackFsSharedPtr_guess), (PackFsSharedPtr_guess *)this)(*(PackFsSharedPtr_guess *)tempPointer);
            strcpy((char *)((PackFsSharedPointerData_guess *)shared_guess)->first, str);
            value_guess = 0;
        }
    }
    return this;
}

// FUNCTION: EQMAIN 0x10007620
// Shares an existing front-end string value with another holder.
// __thiscall, 1 stack arg (a pointer to the source: shared pointer at 0x0, value at 0x4). Copies the source's shared
// pointer into a temporary and takes a reference on its object, passes the temporary by value to
// PackFsSharedPtr_guess::updateSharedPointer_guess (0x10007FE0, which releases this's old object), then copies the
// value at 0x4. Returns this in EAX.
// @early-stop: the original builds the temporary in the outgoing argument slot (push ecx, stores through ecx) and spills its address; this copy goes through a register (the volatile 0 below stands in for that spill), and EQ_THISCALL zeroes the dummy EDX (xor edx,edx) the original never does.
PackFsStringReference_guess *PackFsStringReference_guess::copyStringReference_guess(const PackFsStringReference_guess *source)
{
    /* fake match: a 4-byte frame slot (VC6 allocates the first initialised local with push ecx); the original spills the temporary's address there, and this volatile 0 stands in for that spill. */
    volatile int spill = 0;
    PackFsSharedPtr_guess copy = { source->shared_guess };

    if (copy.ptr != 0)
        copy.ptr->refCount++;
    EQ_METHOD(1, void, 0x10007FE0, (PackFsSharedPtr_guess *, PackFsSharedPtr_guess), (PackFsSharedPtr_guess *)this)(copy);
    value_guess = source->value_guess;
    return this;
}

// FUNCTION: EQMAIN 0x10007650
// Normalizes a C-string path for consistent front-end file lookup.
// @early-stop: EH registration/state stores, native thiscall call setup, and input-temporary placement still differ.
// __thiscall, 1 stack arg (a C string), RET 4: written as a __fastcall with the EDX dummy. Returns self. Clears self, then
// copies the string into input_copy (strlen+1 bytes, held by a shared pointer that updateSharedPointer_guess (0x10007FE0)
// takes by value). output_buffer (count+1 bytes, count from 0x10007830) gets the normalised copy: 'A'-'Z' become lower
// case and each run of '\' or '/' becomes one '/'. output_string (0x10007540) reads that buffer, result (0x10007620) takes
// it, self (0x10007620) takes result, and the locals are released in the original's order: output_string and output_buffer
// by 0x10010B50, result and input_copy by 0x10007520 when their shared pointer is not null.
// Missing EH frame: destructible locals PackFsStringReference_guess (input_copy, result, output_string) and
// PackFsSharedPtr_guess (output_buffer); no destructor for these classes is declared in include/ or this file.
PackFsStringReference_guess *__fastcall initializeStringReferenceWithNormalizedString_guess(PackFsStringReference_guess *pakName, int unused_edx, const char *buffer)
{
    PackFsStringReference_guess input_copy;
    PackFsStringReference_guess result;
    PackFsStringReference_guess output_string;
    int count;
    int i;
    int j;
    int output_index;
    char c;
    bool sep;
    unsigned int len;
    const char *source = buffer;

    pakName->shared_guess = 0;
    pakName->value_guess = 0;
    EQ_METHOD(1, PackFsSharedPtr_guess *, 0x10007F70, (PackFsSharedPtr_guess *, unsigned int), (PackFsSharedPtr_guess *)&input_copy)(0);
    input_copy.value_guess = 0;
    if (source != 0)
    {
        len = strlen(source);
        if (len != 0)
        {
            EQ_METHOD(1, PackFsSharedPtr_guess *, 0x10007F70, (PackFsSharedPtr_guess *, unsigned int), (PackFsSharedPtr_guess *)&buffer)(len + 1);
            EQ_METHOD(1, void, 0x10007FE0, (PackFsSharedPtr_guess *, PackFsSharedPtr_guess), (PackFsSharedPtr_guess *)&input_copy)(*(PackFsSharedPtr_guess *)&buffer);
            strcpy((char *)((PackFsSharedPointerData_guess *)input_copy.shared_guess)->first, source);
            input_copy.value_guess = 0;
        }
    }
    EQ_METHOD(1, PackFsStringReference_guess *, 0x10007540, (PackFsStringReference_guess *, const char *), &result)(0);
    count = EQ_METHOD(0, int, 0x10007830, (int *), (int *)&input_copy)();
    EQ_METHOD(1, PackFsSharedPtr_guess *, 0x10007F70, (PackFsSharedPtr_guess *, unsigned int), (PackFsSharedPtr_guess *)&buffer)(count + 1);
    j = 0;
    count = EQ_METHOD(0, int, 0x10007830, (int *), (int *)&input_copy)();
    sep = false;
    i = 0;
    if (count > 0)
    {
        do
        {
            c = ((char *)((PackFsSharedPointerData_guess *)input_copy.shared_guess)->first)[i];
            if (c >= 'A' && c <= 'Z')
            {
                c += ' ';
                output_index = j;
                j++;
                ((char *)((PackFsSharedPointerData_guess *)buffer)->first)[output_index] = c;
                sep = false;
            }
            else
            {
                if (c == '\\' || c == '/')
                {
                    if (!sep)
                    {
                        output_index = j;
                        j++;
                        ((char *)((PackFsSharedPointerData_guess *)buffer)->first)[output_index] = '/';
                        sep = true;
                    }
                    i++;
                    continue;
                }
                else
                {
                    output_index = j;
                    j++;
                    ((char *)((PackFsSharedPointerData_guess *)buffer)->first)[output_index] = c;
                    sep = false;
                }
            }
            i++;
        } while (i < count);
    }
    ((char *)((PackFsSharedPointerData_guess *)buffer)->first)[j] = 0;
    EQ_METHOD(1, PackFsStringReference_guess *, 0x10007540, (PackFsStringReference_guess *, const char *), &output_string)((char *)((PackFsSharedPointerData_guess *)buffer)->first);
    EQ_METHOD(1, void *, 0x10007620, (void *, void *), &result)(&output_string);
    EQ_METHOD(0, void, 0x10010B50, (int *), (int *)&output_string)();
    EQ_METHOD(0, void, 0x10010B50, (int *), (int *)&buffer)();
    EQ_METHOD(1, void *, 0x10007620, (void *, void *), pakName)(&result);
    if (result.shared_guess != 0)
        EQ_METHOD(0, void, 0x10007520, (PackFsRefCounted_guess *), result.shared_guess)();
    if (input_copy.shared_guess != 0)
        EQ_METHOD(0, void, 0x10007520, (PackFsRefCounted_guess *), input_copy.shared_guess)();
    return pakName;
}

// FUNCTION: EQMAIN 0x10007830
// Returns a shared string's character count without its terminator.
// __fastcall, 1 arg (a pointer to a handle whose first dword points at an object), no stack args. Returns the unsigned
// dword at +0xc of that object minus one; 0 when the handle or the object is null, or the dword is 0. No header struct
// for the object yet: the offset is read straight from the asm.
int __fastcall PackFsCountMinusOne_guess(int *handle)
{
    unsigned int count;

    if (*handle != 0 && (count = *(unsigned int *)(*handle + 0xc), count > 0))
        return count - 1;
    return 0;
}

// Local stand-in for the __thiscall method at 0x10007850: the object's layout is unknown, so it has no fields.
struct PackFsStringReferenceCopier_guess {
    void *copyStringReferenceAndReturn_guess(void *source);
};

// FUNCTION: EQMAIN 0x10007850
// Thunk that forwards a value to a copy operation; its higher-level role is unclear.
// __thiscall, 1 stack arg (a source pointer, passed on unread; callee pops 4). Calls 0x10007620 (copyStringReference_guess)
// with this and the source, then returns this in EAX. Nothing else is touched.
void *PackFsStringReferenceCopier_guess::copyStringReferenceAndReturn_guess(void *source)
{
    EQ_METHOD(1, void *, 0x10007620, (void *, void *), this)(source);
    return this;
}

// FUNCTION: EQMAIN 0x10007870
// Normalizes a PackFsString path before storing it as a pack file reference.
// __thiscall, 1 stack arg (a string handle, PackFsString_guess *); written as __fastcall with the EDX dummy (callee pops 4).
// Returns self in EAX. Same shape as initializeStringReferenceWithString_guess (0x10007540), with a normalizing copy first:
// A-Z become a-z, each run of '\' or '/' becomes one '/', other bytes are copied. The loop count is the string length minus
// one (0 for an empty string). The copy goes into a new buffer, which is copied into a local reference; the local's shared
// pointer and value are then copied into self with one reference taken. Bugs kept: a null node from operator new is
// dereferenced. The locals still use explicit reference-count cleanup here.
// @early-stop: the original EH frame/state transitions and outgoing stack temporaries still differ.
PackFsStringReference_guess *__fastcall initializeStringReferenceWithNormalizedPath_guess(PackFsStringReference_guess *destination, int unused_edx, PackFsString_guess *source)
{
    PackFsStringReference_guess local;
    PackFsSharedPointerData_guess *object;
    PackFsSharedPtr_guess temp;
    PackFsSharedPtr_guess copy;
    void *buffer;
    void *node;
    unsigned int len;
    unsigned int size;
    int count;
    int out;
    int i;
    char c;
    char *text;
    unsigned int n;
    bool prevSlash;

    object = 0;
    EQ_THISCALL(PackFsSharedPtr_guess *, 0x10007F70, (PackFsSharedPtr_guess *, int, unsigned int))((PackFsSharedPtr_guess *)destination, unused_edx, 0);
    destination->value_guess = 0;
    len = EQ_FUNC(unsigned int, __fastcall, 0x10008080, (PackFsString_guess *))(source);
    if (len > 0)
        len = EQ_FUNC(unsigned int, __fastcall, 0x10008080, (PackFsString_guess *))(source) - 1;
    else
        len = 0;
    size = len + 1;
    if (size != 0)
    {
        buffer = EQ_CDECL(void *, 0x100338E5, (unsigned int))(size);
        node = EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x10);
        if (node != 0)
            node = EQ_METHOD(2, void *, 0x10008090, (void *, void *, unsigned int), node)(buffer, size);
        else
            node = 0;
        object = (PackFsSharedPointerData_guess *)node;
        object->refCount = object->refCount + 1;
    }
    out = 0;
    if (EQ_FUNC(unsigned int, __fastcall, 0x10008080, (PackFsString_guess *))(source) > 0)
        count = EQ_FUNC(unsigned int, __fastcall, 0x10008080, (PackFsString_guess *))(source) - 1;
    else
        count = 0;
    prevSlash = false;
    for (i = 0; i < count; i++)
    {
        c = ((char *)((PackFsObject_guess *)source->data)->data)[i];
        if (c < 'A' || c > 'Z')
        {
            if (c == '\\' || c == '/')
            {
                if (!prevSlash)
                {
                    ((char *)object->first)[out++] = '/';
                    prevSlash = true;
                }
            }
            else
            {
                ((char *)object->first)[out++] = c;
                prevSlash = false;
            }
        }
        else
        {
            ((char *)object->first)[out++] = c + ' ';
            prevSlash = false;
        }
    }
    ((char *)object->first)[out] = 0;
    text = (char *)object->first;
    EQ_METHOD(1, PackFsSharedPtr_guess *, 0x10007F70, (PackFsSharedPtr_guess *, unsigned int), (PackFsSharedPtr_guess *)&local)(0);
    local.value_guess = 0;
    if (text != 0)
    {
        n = strlen(text);
        if (n != 0)
        {
            EQ_METHOD(1, PackFsSharedPtr_guess *, 0x10007F70, (PackFsSharedPtr_guess *, unsigned int), (PackFsSharedPtr_guess *)&temp)(n + 1);
            EQ_METHOD(1, void, 0x10007FE0, (PackFsSharedPtr_guess *, PackFsSharedPtr_guess), (PackFsSharedPtr_guess *)&local)(temp);
            strcpy((char *)((PackFsSharedPointerData_guess *)local.shared_guess)->first, text);
            local.value_guess = 0;
        }
    }
    copy.ptr = local.shared_guess;
    if (copy.ptr != 0)
        copy.ptr->refCount++;
    EQ_METHOD(1, void, 0x10007FE0, (PackFsSharedPtr_guess *, PackFsSharedPtr_guess), (PackFsSharedPtr_guess *)destination)(copy);
    PackFsRefCounted_guess *localShared = local.shared_guess;
    int localValue = local.value_guess;
    destination->value_guess = localValue;
    if (localShared != 0 && --localShared->refCount == 0 && localShared != 0)
        localShared->destroy_guess(1);
    if (object != 0 && --object->refCount == 0)
        ((PackFsRefCounted_guess *)object)->destroy_guess(1);
    return destination;
}

// FUNCTION: EQMAIN 0x10007a80
// Releases a front-end resource record and the resource it owns.
// __cdecl, 1 stack arg, no return value. Unlinks `record` from the list at 0x1004CE90 (the node whose +0x4 is record
// goes through list3_unlink_free_guess), releases the object at record +0xc through its vtable slot 2 (stdcall) when record +0x10
// is 0, takes record[1] * record[0] * 2 off the total at 0x1004CEBC, then frees record. A null record does nothing.
// No header struct yet: record is int *; the offsets 0x0-0x10 are read straight from the asm.
#define g_packfs_list_head_guess EQ_GLOBAL(int *, 0x1004CE90)
#define g_packfs_used_bytes_guess EQ_GLOBAL(int, 0x1004CEBC)
void PackFsFreeEntry_guess(int *record)
{
    int *node;

    if (record != 0)
    {
        for (node = g_packfs_list_head_guess; node != 0; node = (int *)node[2])
        {
            if (record == (int *)node[1])
            {
                EQ_CDECL(int, 0x10028430, (int *, int *))((int *)&g_packfs_list_head_guess, node);
                break;
            }
        }
        if (record[4] == 0)
        {
            void *object = (void *)record[3];

            ((void (__stdcall *)(void *))(*(void ***)object)[2])(object);
        }
        g_packfs_used_bytes_guess = g_packfs_used_bytes_guess + record[1] * *record * -2;
        EQ_CDECL(void, 0x10033568, (void *))(record);
    }
    return;
}

// One slot of the button table (8 bytes): the bitmap name and the source cached for it (0 until first loaded).
struct PackFsButtonSlot_guess
{
    char *name;     // 0x0
    int *source;    // 0x4
};

// FUNCTION: EQMAIN 0x10007AE0
// Loads a named front-end image surface; for 25 listed names the loaded source is cached and a copy returned.
// __cdecl, 2 stack args, returns int * in EAX (0 on failure). A null name calls FatalErrorExit_guess with the string pointer
// at 0x10055A9C and returns 0. Otherwise __strcmpi (case-insensitive) matches the name against the 25 slots of the
// button table at 0x10046440 (8 bytes each: name, cached source). A hit uses the slot's cached source, loading and
// caching one through LoadFrontEndArt_guess(name, load_arg) when the slot is empty; a miss returns that loader result uncached.
// The result is a surface from CreateSurfaceRecord_guess(w, h, 0x10) that BltFastSrcRect_guess fills from the source; a 0 surface calls
// FatalErrorExit_guess with the string pointer at 0x10055AA4 and returns 0.
// @early-stop: ratio 0.678; register saves and the uncached path's register assignment still differ.
#define g_packfs_null_name_error_guess EQ_GLOBAL(char *, 0x10055A9C)
#define g_packfs_surface_error_guess EQ_GLOBAL(char *, 0x10055AA4)
#define g_packfs_button_slots_guess EQ_ARRAY(PackFsButtonSlot_guess, 0x10046440)
int *PackFsGetNamedImage_guess(char *name, int load_arg)
{
    int *source;
    int *surface;

    if (name == 0)
    {
        EQ_CDECL(void, 0x100278A0, (char *))(g_packfs_null_name_error_guess);
        return 0;
    }
    short slot;
    for (slot = 0; slot < 0x19; ++slot)
    {
        if (EQ_CDECL(int, 0x10033730, (char *, char *))(name, g_packfs_button_slots_guess[slot].name) == 0)
        {
            source = g_packfs_button_slots_guess[slot].source;
            if (source != 0)
            {
                surface = EQ_CDECL(int *, 0x10006D80, (int, int, int))(source[0], source[1], 0x10);
                if (surface != 0)
                {
                    EQ_CDECL(void, 0x10006C20, (int *, int, int, int *, int, int, int, int, unsigned int))(
                        surface, 0, 0, source, 0, 0, source[0], source[1], 0);
                    return surface;
                }
            }
            else
            {
                source = EQ_CDECL(int *, 0x10006EC0, (char *, int))(name, load_arg);
                if (source == 0)
                {
                    return source;
                }
                g_packfs_button_slots_guess[slot].source = source;
                surface = EQ_CDECL(int *, 0x10006D80, (int, int, int))(source[0], source[1], 0x10);
                if (surface != 0)
                {
                    EQ_CDECL(void, 0x10006C20, (int *, int, int, int *, int, int, int, int, unsigned int))(
                        surface, 0, 0, source, 0, 0, source[0], source[1], 0);
                    return surface;
                }
            }
            EQ_CDECL(void, 0x100278A0, (char *))(g_packfs_surface_error_guess);
            return surface;
        }
    }
    return EQ_CDECL(int *, 0x10006EC0, (char *, int))(name, load_arg);
}

// FUNCTION: EQMAIN 0x10007BF0
// Draws a named front-end image at the upper-left of the shared drawing surface.
// __cdecl, 3 stack args (arg 1 is never read), void. Gets the button bitmap for name from PackFsGetNamedImage_guess(name, load_arg).
// A miss (0) calls FatalErrorExit_guess with the string pointer at 0x10055AA8 and name, then returns. Otherwise BltFastSrcRect_guess
// copies the whole bitmap (w, h = its first two dwords) onto the surface at 0x1004CEB8 at (0, 0), then PackFsFreeEntry_guess
// is called on it.
#define g_packfs_button_error_guess EQ_GLOBAL(char *, 0x10055AA8)
#define g_packfs_dest_surface_guess EQ_GLOBAL(int *, 0x1004CEB8)
void PackFsBlitNamedImageAtOrigin_guess(int unused, char *imageName, int load_arg)
{
    int *bitmap;

    bitmap = EQ_CDECL(int *, 0x10007AE0, (char *, int))(imageName, load_arg);
    if (bitmap == 0)
    {
        EQ_CDECL(void, 0x100278A0, (char *, char *))(g_packfs_button_error_guess, imageName);
        return;
    }
    EQ_CDECL(void, 0x10006C20, (int *, int, int, int *, int, int, int, int, unsigned int))(
        g_packfs_dest_surface_guess, 0, 0, bitmap, 0, 0, bitmap[0], bitmap[1], 0);
    EQ_CDECL(void, 0x10007A80, (int *))(bitmap);
}

// FUNCTION: EQMAIN 0x10007C50
// Dims part of a 16-bit front-end surface for reduced-intensity display.
// __cdecl, 5 stack args, void. Halves a width x height block of 16-bit pixels at (x, y) of the surface in arg 1: each
// pixel becomes (pixel >> 1) & mask, mask = 0x7BEF when the global at 0x1004CEA0 == 1 (5-6-5), else 0x3DEF (5-5-5).
// The surface keeps its pixel pointer at +0x14 and its pitch in bytes (uint) at +0x18; a row is pitch / 2 pixels after
// the one above. RestoreAndLockSurface_guess(surface) runs before the rows and CallSubobjectSlot80_guess(surface) after them. Pixels are written
// only while x < x + width (signed), so a width of 0 or less, or a sum that wraps negative, changes nothing.
// @early-stop: ratio 0.6852; register and stack-slot layout and the x-count setup still differ.
void PackFsHalveRect16_guess(int surface, int x, int y, int width, int height)
{
    int xEnd = x + width;
    int yEnd = y + height;
    int mask = ((EQ_GLOBAL(int, 0x1004CEA0) - 1) ? 0xffffc200 : 0) + 0x7bef;

    EQ_CDECL(void, 0x10006CE0, (int))(surface);
    while (y < yEnd)
    {
        int rowOffset = (*(unsigned int *)(surface + 0x18) >> 1) * y;
        ++y;
        unsigned short *pixel = (unsigned short *)(*(int *)(surface + 0x14) + (rowOffset + x) * 2);
        if (x < xEnd)
        {
            int count = (xEnd - (x + 1)) + 1;
            do
            {
                *pixel = (*pixel >> 1) & mask;
                ++pixel;
            } while (--count != 0);
        }
    }
    EQ_CDECL(void, 0x10006D60, (int))(surface);
    return;
}

// FUNCTION: EQMAIN 0x10007CF0
// Dims a full 16-bit front-end surface for reduced-intensity display.
// __cdecl, 1 stack arg, void. Halves the whole surface: PackFsHalveRect16_guess(surface, 0, 0, surface[0], surface[1]),
// so the first two dwords of the surface are its width and height in pixels.
void PackFsHalveSurface16_guess(int *surface)
{
    EQ_CDECL(void, 0x10007C50, (int, int, int, int, int))((int)surface, 0, 0, surface[0], surface[1]);
}

typedef int (__stdcall *PackFsGetSurfaceDescFn_guess)(void *self, unsigned int *desc);

// FUNCTION: EQMAIN 0x10007D10
// Selects the 5-5-5 or 5-6-5 format and RGB shifts for front-end color conversion.
// @early-stop: success-path register saves and final green/blue width checks still differ.
// __cdecl, 2 stack args, returns char (callers read AL). Reads the pixel format of owner->+0xc through GetSurfaceDesc
// (vtable slot 0x58, __stdcall, DDSD_PIXELFORMAT). For red, green and blue in turn it writes the mask shifted down to
// bit 0 (layout[2], [1], [0]), its trailing-zero count (layout[10], [9], [8]) and its code (mask == 0x1f) + 2
// (layout[6], [5], [4]: 3 for a 5-bit channel, 2 otherwise). g_eqmain_intro_mode_guess becomes 0 for 5-5-5, 1 for 5-6-5, else 2.
// A failed GetSurfaceDesc is reported through FatalErrorExit_guess(DAT_10055AAC) and returns 0.
char PackFsDetectColorFormat16_guess(int record, unsigned char *layout)
{
    unsigned int desc[27]; // DDSURFACEDESC, 0x6c bytes: [0] dwSize, [1] dwFlags, [0x58] R, [0x5c] G, [0x60] B mask
    void *surface;
    int hr;
    desc[0] = 0x6c;
    desc[1] = 0x1000;
    surface = *(void **)(record + 0xc);
    hr = ((PackFsGetSurfaceDescFn_guess)(*(void ***)surface)[22])(surface, desc);
    if (hr != 0)
    {
        EQ_CDECL(void, 0x100278A0, (char *))(EQ_GLOBAL(char *, 0x10055AAC));
        return 0;
    }

    unsigned int red;
    unsigned int green;
    unsigned int blue;
    int count;
    unsigned char greenCode;
    char blueCode;
    int diff;

    red = desc[22];
    count = 0;
    for (; (red & 1) == 0; red >>= 1)
        count++;
    layout[2] = (unsigned char)red;
    layout[10] = (unsigned char)count;
    layout[6] = (red == 0x1f) + 2;

    green = desc[23];
    count = 0;
    for (; (green & 1) == 0; green >>= 1)
        count++;
    layout[1] = (unsigned char)green;
    layout[9] = (unsigned char)count;
    greenCode = (green == 0x1f) + 2;
    layout[5] = greenCode;

    blue = desc[24];
    count = 0;
    for (; (blue & 1) == 0; blue >>= 1)
        count++;
    layout[0] = (unsigned char)blue;
    layout[8] = (unsigned char)count;
    blueCode = (blue == 0x1f) + 2;
    layout[4] = blueCode;

    diff = 8 - layout[6];
    if (diff == 5 && 8 - (unsigned int)greenCode == 5 &&
        8 - (unsigned int)blueCode == 5)
    {
        EQ_GLOBAL(int, 0x1004CEA0) = 0;
        return 1;
    }
    if (diff != 5 || (diff = 8 - (unsigned int)greenCode, diff != 6) ||
        (EQ_GLOBAL(int, 0x1004CEA0) = 1, diff = 8 - (unsigned int)blueCode, diff != 5))
        EQ_GLOBAL(int, 0x1004CEA0) = 2;
    return 1;
}

// FUNCTION: EQMAIN 0x10007e60
// Converts RGB color values to the front-end surface's 16-bit pixel format.
// __cdecl, 3 stack args, returns the packed value in EAX (callers read AX). Packs red, green and blue into a 16-bit
// colour: the global at 0x1004CEA0 == 1 selects the 5-6-5 layout (green keeps 6 bits), any other value 5-5-5.
// Blue is shifted without a 5-bit mask, so blue values above 0xff spill into the green and red fields.
#define g_eqmain_intro_mode_guess EQ_GLOBAL(int, 0x1004CEA0)
unsigned int PackFsPackColor16_guess(unsigned int red, unsigned int green, unsigned int blue)
{
    if (g_eqmain_intro_mode_guess == 1)
    {
        return ((red & 0xf8) << 5 | (green & 0xfc)) << 3 | ((unsigned int)(unsigned short)blue >> 3);
    }
    return ((red & 0xf8) << 5 | (green & 0xf8)) << 2 | ((unsigned int)(unsigned short)blue >> 3);
}

// FUNCTION: EQMAIN 0x10007ec0
// Destroys a shared front-end object and frees its storage when deletion is requested.
// __thiscall (callee pops 4 bytes). Scalar deleting destructor: runs releasePurecallObjectWithMember_guess
// (0x10007EE0, __fastcall with this in ECX), frees the object with the cdecl FUN_10033800 when bit 0 of flags
// is set, returns this.
PackFsRefCounted_guess *PackFsRefCounted_guess::scalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x10007EE0, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(this);
    return this;
}

// FUNCTION: EQMAIN 0x10007EE0
// @early-stop: the initial saved-this store and EH state 0 remain unmatched; the indirect slot call also needs an EDX zeroing move.
// __fastcall, this in ECX, no stack args. Frees the member and performs the refcounted base cleanup.
void __fastcall releasePurecallObjectWithMember_guess(PackFsRefCounted_guess *self)
{
    typedef void (__fastcall *PurecallSlot_guess)(PackFsRefCounted_guess *, void *, int);
    int count;

    *(volatile unsigned int *)self = 0x10040DF4;
    {
        /* fake match: the handle supplies the /GX lifetime around the free, then is disarmed after the manual release. */
        PackFsRefHandle_guess frameState = { self };
        try
        {
            EQ_CDECL(void, 0x10033800, (void *))(((void **)self)[2]);
        }
        catch (...)
        {
            throw;
        }
        count = self->refCount;
        count--;
        frameState.object = 0;
    }
    *(volatile unsigned int *)self = 0x10040DFC;
    self->refCount = count;
    if (count == 0 && self != 0)
        ((PurecallSlot_guess *)(*(void * volatile *)self))[0](self, 0, 1);
}

// FUNCTION: EQMAIN 0x10007f50
// Drops a reference and invokes the final virtual hook at zero; its resource purpose is unclear.
// __fastcall, this in ECX, no stack args. Sets the vtable to 0x10040dfc, decrements the count at +0x4, and at zero
// (non-NULL) calls the vtable's slot 0 with 1 (__purecall).
void __fastcall releasePurecallObject_guess(PackFsRefCounted_guess *self)
{
    // The slot takes its deleting flag on the stack; keep the vtable lookup live.
    typedef void (__stdcall *PurecallSlot_guess)(int);
    int count;

    count = self->refCount;
    *(const void **)self = (const void *)0x10040DFC;
    count--;
    self->refCount = count;
    if (count == 0 && self != 0)
        ((PurecallSlot_guess *)(*(void * volatile *)self))[0](1);
}

// FUNCTION: EQMAIN 0x10007f70
// Creates reference-counted storage for pack strings and front-end asset data.
// __thiscall, 1 stack arg (the size), RET 4: written as a __fastcall with the EDX dummy. Clears self->ptr; a zero size
// returns self. Otherwise allocates the buffer, then a 0x10-byte object (vtable 0x10040df4, count at +0x4, buffer at
// +0x8, size at +0xc), points self->ptr at it, takes its first reference and returns self. Bugs kept: the buffer is not
// checked for NULL, and when the object allocation fails self->ptr is set to NULL and the NULL object's count (+0x4,
// address 4, unmapped) is incremented anyway: an access violation there.
// operator_new(size), __cdecl at 0x100338E5 (library)
#define operator_new_guess EQ_CDECL(void *, 0x100338E5, (unsigned int))
PackFsSharedPtr_guess *__fastcall allocateSharedBuffer_guess(PackFsSharedPtr_guess *self, int unused_edx, unsigned int size)
{
    void *buffer;
    PackFsSharedPointerData_guess *object;

    self->ptr = 0;
    if (size == 0)
        return self;
    buffer = operator_new_guess(size);
    object = (PackFsSharedPointerData_guess *)operator_new_guess(0x10);
    if (object != 0)
    {
        object->refCount = 0;
        object->first = (int)buffer;
        object->second = (int)size;
        object->vtable = (const void *)0x10040DF4;
        self->ptr = (PackFsRefCounted_guess *)object;
        object->refCount++;
        return self;
    }
    self->ptr = (PackFsRefCounted_guess *)object;
    object->refCount++;
    return self;
}

// FUNCTION: EQMAIN 0x10007fe0
// Changes which shared pack object a handle keeps alive.
// @early-stop: EH prologue order/slot, cleanup marker frame size, and the old-pointer null check still differ.
// Cloned from trilogy PackFsSharedPtr_guess::updateSharedPointer_guess 0x004FB610 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 1 stack arg (the shared pointer by value, released by the callee at the end). Assigns `replacement`
// to this pointer: releases the old object (deleting it at zero) and takes a reference on the new one.
// Returns this. The null handle locals keep the original /GX cleanup-state shape without releasing an extra object.
PackFsSharedPtr_guess &PackFsSharedPtr_guess::updateSharedPointer_guess(PackFsSharedPtr_guess replacement)
{
    if (&replacement == this)
    {
        {
            /* fake match: this null handle supplies the original /GX cleanup state. */
            PackFsRefHandle_guess cleanup = { 0 };
        }
        if (replacement.ptr != 0)
        {
            replacement.ptr->refCount--;
            if (replacement.ptr->refCount == 0 && replacement.ptr != 0)
                replacement.ptr->destroy_guess(1);
        }
    }
    else
    {
        {
            /* fake match: this null handle supplies the original /GX cleanup state. */
            PackFsRefHandle_guess cleanup = { 0 };

            PackFsRefCounted_guess *old = ptr;
            if (old != 0)
            {
                old->refCount--;
                if (old->refCount == 0 && old != 0)
                    old->destroy_guess(1);
            }
            ptr = replacement.ptr;
            if (replacement.ptr != 0)
                replacement.ptr->refCount++;
        }
        if (replacement.ptr != 0)
        {
            replacement.ptr->refCount--;
            if (replacement.ptr->refCount == 0)
                replacement.ptr->destroy_guess(1);
        }
    }
    return *this;
}

// FUNCTION: EQMAIN 0x10008080
// Gets the length of a pack file system string.
// Cloned from trilogy CXStr_getStringLength_guess 0x004DD993 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args). Length of the string, 0 when there is no data.
unsigned int __fastcall CXStr_getStringLength_guess(PackFsString_guess *str)
{
    if (str->data != 0)
        return str->data->length;
    return 0;
}

// FUNCTION: EQMAIN 0x1000e7e0
// Loads and decompresses the next block from a packed file.
// __fastcall (this in ECX, no stack args). Reads a block header (the data length, then the block size, which must be
// at most 0x2000) and the data, records the file position at 0x2020, and returns the block size when 0x1000e4e0
// accepts the data, 0 otherwise. The data length is not checked against the 16 KB buffer.
// 0x2020 is past the end of PackFsReader_guess (0x2008), so it is written through a raw offset until the struct has it.
unsigned int __fastcall readPackFileHeader_guess(PackFsReader_guess *reader)
{
    char data[16384];
    unsigned int dataLength;
    unsigned int blockSize;
    unsigned int bytesRead;
    unsigned int status;
    long position;

    EQ_CDECL(int, 0x10034917, (int, void *, unsigned int))(reader->fd, &dataLength, 4);
    EQ_CDECL(int, 0x10034917, (int, void *, unsigned int))(reader->fd, &blockSize, 4);
    if (blockSize > 0x2000)
        return 0;
    bytesRead = EQ_CDECL(int, 0x10034917, (int, void *, unsigned int))(reader->fd, data, dataLength);
    position = EQ_CDECL(long, 0x10034906, (int))(reader->fd);
    *(long *)((char *)reader + 0x2020) = position;
    if (bytesRead != dataLength)
        return 0;
    status = EQ_CDECL(unsigned int, 0x1000E4E0, (char *, unsigned int *, char *, unsigned int))(
        reader->buffer, &blockSize, data, dataLength);
    return (status != 0) ? 0 : blockSize;
}

// FUNCTION: EQMAIN 0x1000ecd0
// Drops the holder's reference and destroys its object when the count reaches zero.
// Cloned from trilogy decrementHeldReferenceCount_guess 0x004F9300 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args). Drops the holder's reference, deleting the object at zero.
// Named apart from the 0x10007520 decrementReferenceCount_guess so the test harness finds each by name.
void __fastcall decrementHeldReferenceCount_guess(PackFsRefHolder_guess *holder)
{
    PackFsRefCounted_guess *object;
    int count;

    if (holder->object == 0)
        return;
    object = holder->object;
    count = object->refCount;
    count--;
    object->refCount = count;
    if (count != 0)
        return;

    if (object != 0)
        object->destroy_guess(1);
}

// Local layout guess: a value the removal holds in a local (its object pointer at 0x0). Construction takes a
// reference and the destructor drops it, destroying the object at zero.
struct PackFsRemoveHolder_guess
{
    PackFsRemoveValue_guess *object; // 0x0

    PackFsRemoveHolder_guess(PackFsRemoveValue_guess *held)
    {
        object = held;
        if (object != 0)
        {
            // Preserve a separate count load instead of a memory increment.
            int count = static_cast<volatile int &>(object->refCount);
            object->refCount = count + 1;
        }
    }
    ~PackFsRemoveHolder_guess()
    {
        PackFsRemoveValue_guess *obj = object;

        if (obj != 0 && --obj->refCount == 0 && obj != 0)
            obj->destroy_guess(1);
    }
};

// Local layout guess: a by-value reference argument. Copying takes a reference; the callee releases its copy.
struct PackFsRefCopy_guess
{
    PackFsRefCounted_guess *object; // 0x0

    PackFsRefCopy_guess(const PackFsRefCopy_guess &other)
    {
        object = other.object;
        if (object != 0)
            object->refCount++;
    }
};

// FUNCTION: EQMAIN 0x1000ecf0
// Removes a named file system entry from the pack resource index and releases its linked resources.
// Cloned from trilogy PackFS::removeFileSystem_guess 0x004F9320 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: the holder's and the copies' EH state stores are not emitted, five EQ_THISCALL dummy EDX slots are zeroed (xor edx,edx), and register picks differ.
// __thiscall method (this in ECX, unused) with one 8-byte by-value argument: the callee pops 8 (RET 0x8). Returns nothing.
void PackFS::removeFileSystem_guess(PackFsRemoveFileArgument_guess removeRequest)
{
    PackFsRemoveTreeNode_guess *node;

    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x1004014C)(EQ_ARRAY(char, 0x1004CEE0));
    EQ_METHOD(0, void, 0x1000EEB0, (void *), (void *)&removeRequest)();
    {
        int key = removeRequest.string;
        EQ_METHOD(2, void, 0x1000FAD0, (void *, PackFsRemoveTreeNode_guess **, int *), (void *)0x1004CEF8)(&node, &key);
    }
    if (EQ_GLOBAL(PackFsRemoveTreeNode_guess *, 0x1004CEFC) != node)
    {
        PackFsRemoveHolder_guess value(node->value);
        if (((PackFsRemoveChild_guess *)value.object->child)->reference10 != 0)
        {
            EQ_METHOD(1, void, 0x10010B80, (void *, PackFsRefCopy_guess), (char *)EQ_METHOD(0, void *, 0x10010B70, (void *), 
                    (char *)EQ_METHOD(0, void *, 0x10010B70, (void *), &value)() + 0x10)() + 0xc)(*(PackFsRefCopy_guess *)((char *)EQ_METHOD(0, void *, 0x10010B70, (void *), (void *)&value)() + 0xc));
        }
        else
        {
            EQ_METHOD(1, void, 0x10010B80, (void *, PackFsRefCopy_guess), (void *)0x1004CF0C)(*(PackFsRefCopy_guess *)((char *)EQ_METHOD(0, void *, 0x10010B70, (void *), (void *)&value)() + 0xc));
        }
        if (((PackFsRemoveChild_guess *)value.object->child)->referenceC != 0)
        {
            EQ_METHOD(1, void, 0x10010B80, (void *, PackFsRefCopy_guess), (char *)EQ_METHOD(0, void *, 0x10010B70, (void *), 
                    (char *)EQ_METHOD(0, void *, 0x10010B70, (void *), &value)() + 0xc)() + 0x10)(*(PackFsRefCopy_guess *)((char *)EQ_METHOD(0, void *, 0x10010B70, (void *), (void *)&value)() + 0x10));
        }
        EQ_METHOD(2, void, 0x1000F6B0, (void *, void *, PackFsRemoveTreeNode_guess *), (void *)0x1004CEF8)(&node, node);
    }
    EQ_GLOBAL(PackFsCriticalSectionFunction_guess, 0x10040148)(EQ_ARRAY(char, 0x1004CEE0));
    return;
}

// FUNCTION: EQMAIN 0x1000eeb0
// __fastcall (holder in ECX; incoming EDX is forwarded; no stack args). Makes the string on first use.
/* fake match: repeat the zero test to retain mov/test; forward incoming EDX through EQ_THISCALL's dummy slot. */
void __fastcall computeStringCrcIfUnset_guess(PackFsStringHolder_guess *holder, int incomingEDX)
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
        holder->string = EQ_THISCALL(int, 0x1000E5B0, (int *, int, int, int))(
            &temp, incomingEDX, first, second);
    }
}

// FUNCTION: EQMAIN 0x1000eee0
// Prepares two empty reference handles for pack processing; their specific role is unclear.
// Cloned from trilogy PackFsRefPair_guess::PackFsRefPair_guess 0x004F9510 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: the try/rethrow frame adds a push-ecx slot (add esp, 0x10) and reorders the fs:[0] prolog and the saved-handler reload; the original's frame has neither.
// __thiscall constructor (this in ECX, no stack args): zeroes both handles, returns this.
PackFsRefPair_guess::PackFsRefPair_guess()
{
    /* fake match: the original's EH frame (state -1, no unwind code). VC6 drops the frame for an empty or return-only handler and keeps it for a rethrow; the stores cannot throw, so the rethrow never runs. */
    try {
        first.object = 0;
        second.object = 0;
    } catch (...) {
        throw;
    }
}

// FUNCTION: EQMAIN 0x1000f3c0
// Prepares an empty lookup map for pack file entries.
// Cloned from trilogy PackFsMap_guess::init_guess 0x004F9EE0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, returns this (callee pops 8). Copies allocator/comparator, shares or creates the nil node
// under the lock, then buys the red head node pointing at itself.
PackFsMap_guess *PackFsMap_guess::init_guess(const char &compare_, const char &allocator_)
{
    PackFsMapNode_guess *tmp;
    PackFsMapNode_guess *node;

    allocator = allocator_;
    compare = compare_;
    multi = 0;
    node = (PackFsMapNode_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(sizeof(PackFsMapNode_guess));
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
        EQ_CDECL(void, 0x10033800, (void *))(tmp);
    tmp = g_packfs_map_nil_guess;
    node = (PackFsMapNode_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(sizeof(PackFsMapNode_guess));
    node->parent = tmp;
    node->color = 0;
    head = node;
    size = 0;
    head->left = head;
    head->right = head;
    return this;
}

// FUNCTION: EQMAIN 0x1000F470
// Adds a resource key to the pack tree or returns its existing node when duplicates are off.
// Cloned from trilogy PackFsTree_guess::insert_guess 0x004F9F90 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: 7 EQ_THISCALL dummy-EDX xors, a spilled temp in the first loop compare, and call-argument and link register order differ.
void PackFsTree_guess::insert_guess(unsigned int *result, unsigned int *keyValue)
{
    PackFsTreeNode_guess *parent = head;
    PackFsTreeNode_guess *node = head->parent;
    // The caller's key pointer; keyValue is reassigned to the new node in the insert path.
    unsigned int *value = keyValue;
    bool left = true;
    while (node != g_packfs_tree_nil_guess)
    {
        parent = node;
        left = *value < node->key;
        if (left)
            node = node->left;
        else
            node = node->right;
    }

    if (*(char *)((char *)this + 8))
    {
        PackFsTreeNode_guess *fresh = (PackFsTreeNode_guess *)EQ_METHOD(2, void *, 0x10010020, (PackFsTree_guess *, PackFsTreeNode_guess *, int), this)(parent, 0);
        keyValue = (unsigned int *)fresh;
        fresh->left = g_packfs_tree_nil_guess;
        fresh->right = g_packfs_tree_nil_guess;
        EQ_CDECL(void, 0x10010C20, (unsigned int *, unsigned int *))(&fresh->key, value);
        ++*(int *)((char *)this + 0x0C);
        if (parent == head || node != g_packfs_tree_nil_guess || *value < parent->key)
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
        PackFsTreeNode_guess *current = (PackFsTreeNode_guess *)keyValue;
        PackFsTreeNode_guess *inserted = current;
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
                        EQ_METHOD(1, void, 0x1000FF60, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current);
                    }
                    ((unsigned int *)current->parent)[5] = 1;
                    ((unsigned int *)current->parent->parent)[5] = 0;
                    EQ_METHOD(1, void, 0x1000FFC0, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current->parent->parent);
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
                    EQ_METHOD(1, void, 0x1000FFC0, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current);
                }
                ((unsigned int *)current->parent)[5] = 1;
                ((unsigned int *)current->parent->parent)[5] = 0;
                EQ_METHOD(1, void, 0x1000FF60, (PackFsTree_guess *, PackFsTreeNode_guess *), this)(current->parent->parent);
            }
        }
        ((unsigned int *)head->parent)[5] = 1;
        *result = (unsigned int)inserted;
        *((unsigned char *)result + 4) = 1;
        return;
    }

    PackFsTreeNode_guess *candidate = parent;
    if (left)
    {
        if (parent == head->left)
        {
            unsigned int *answer = EQ_METHOD(4, unsigned int *, 0x1000FCF0, (PackFsTree_guess *, unsigned int **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&keyValue, node, parent, value);
            *result = *answer;
            *((unsigned char *)result + 4) = 1;
            return;
        }
        EQ_METHOD(0, void, 0x10010040, (PackFsTreeNode_guess **), &candidate)();
    }
    if (candidate->key < *value)
    {
        unsigned int *answer = EQ_METHOD(4, unsigned int *, 0x1000FCF0, (PackFsTree_guess *, unsigned int **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&keyValue, node, parent, value);
        *result = *answer;
        *((unsigned char *)result + 4) = 1;
        return;
    }
    *result = (unsigned int)candidate;
    *((unsigned char *)result + 4) = 0;
}

// FUNCTION: EQMAIN 0x1000f6b0
// Erases one entry of the 0x18-byte-node tree (nil 0x1004CF14), dropping a reference on the object it holds
// __thiscall in the binary (this in ECX, two stack arguments, RET 8), written __fastcall with an EDX dummy like the
// free functions around it. `removed` is the node the iterator held before the advance; the advanced iterator (the
// successor) goes back through `result`. Red-black delete: unlink, then fix up colours when a black node left.
void __fastcall RedBlackTree_removeNodeFromRedBlackTree_guess(
    PackFsTree_guess *tree, int edxDummy, int *result, int *node)
{
    int tmp;
    int *ptr;
    int *grandparent;
    int *cursor;
    int *fixSibling;
    int *tmpNode;
    int *fixNode;
    int *unlinked;
    int *header;
    int *parent;
    int *sibling;
    int *removed;
    int lockit; // the std::_Lockit object; an int, not a char, so VC6 keeps it on the original's stack slot
    PackFsRefCounted_guess *refObject;

    removed = node;
    EQ_METHOD(0, void, 0x10010310, (int **), &node)();
    fixNode = (int *)*removed;
    fixSibling = removed + 2;
    unlinked = removed;
    tmpNode = fixSibling;
    if (fixNode == (int *)g_packfs_tree_nil_guess)
    {
        fixNode = (int *)*fixSibling;
    }
    else
    {
        ptr = (int *)*fixSibling;
        if (ptr != (int *)g_packfs_tree_nil_guess)
        {
            unlinked = ptr;
            for (tmpNode = (int *)*ptr;
                 tmpNode != (int *)g_packfs_tree_nil_guess;
                 tmpNode = (int *)*tmpNode)
            {
                unlinked = tmpNode;
            }
            fixNode = (int *)unlinked[2];
            tmpNode = unlinked + 2;
        }
    }
    ptr = unlinked;
    EQ_METHOD(0, void, 0x1003E980, (void *), &lockit)();
    if (unlinked == removed)
    {
        fixNode[1] = unlinked[1];
        header = *(int **)((char *)tree + 4);
        if (header[1] == (int)removed)
        {
            header[1] = (int)fixNode;
        }
        else
        {
            cursor = (int *)removed[1];
            if ((int *)*cursor == removed)
                *cursor = (int)fixNode;
            else
                cursor[2] = (int)fixNode;
        }
        tmpNode = *(int **)((char *)tree + 4);
        if ((int *)*tmpNode == removed)
        {
            if ((int *)*fixSibling == (int *)g_packfs_tree_nil_guess)
            {
                *tmpNode = removed[1];
            }
            else
            {
                ptr = (int *)*fixNode;
                fixSibling = fixNode;
                while (cursor = ptr,
                       cursor != (int *)g_packfs_tree_nil_guess)
                {
                    fixSibling = cursor;
                    ptr = (int *)*cursor;
                }
                *tmpNode = (int)fixSibling;
            }
        }
        if ((int *)tmpNode[2] == removed)
        {
            if ((int *)*removed == (int *)g_packfs_tree_nil_guess)
            {
                fixSibling = (int *)removed[1];
            }
            else
            {
                tmpNode = (int *)fixNode[2];
                fixSibling = fixNode;
                while (ptr = tmpNode,
                       ptr != (int *)g_packfs_tree_nil_guess)
                {
                    fixSibling = ptr;
                    tmpNode = (int *)ptr[2];
                }
            }
            (*(int **)((char *)tree + 4))[2] = (int)fixSibling;
        }
    }
    else
    {
        *(int **)(*removed + 4) = unlinked;
        *unlinked = *removed;
        if (unlinked == (int *)*fixSibling)
        {
            fixNode[1] = (int)unlinked;
        }
        else
        {
            fixNode[1] = unlinked[1];
            *(int **)unlinked[1] = fixNode;
            *tmpNode = *fixSibling;
            *(int **)(*fixSibling + 4) = unlinked;
        }
        header = *(int **)((char *)tree + 4);
        if (header[1] == (int)removed)
        {
            header[1] = (int)unlinked;
        }
        else
        {
            fixSibling = (int *)removed[1];
            if ((int *)*fixSibling == removed)
                *fixSibling = (int)unlinked;
            else
                fixSibling[2] = (int)unlinked;
        }
        unlinked = removed;
        ptr[1] = removed[1];
        tmp = ptr[5];
        ptr[5] = removed[5];
        removed[5] = tmp;
    }
    if (unlinked[5] == 1)
    {
        header = *(int **)((char *)tree + 4);
        if (fixNode != (int *)header[1])
        {
            do
            {
                if (fixNode[5] != 1)
                    break;
                fixSibling = *(int **)fixNode[1];
                if (fixNode == fixSibling)
                {
                    fixSibling = (int *)((int **)fixNode[1])[2];
                    if (fixSibling[5] == 0)
                    {
                        fixSibling[5] = 1;
                        *(int *)(fixNode[1] + 0x14) = 0;
                        tmp = fixNode[1];
                        fixSibling = *(int **)(tmp + 8);
                        *(int *)(tmp + 8) = *fixSibling;
                        if ((int *)*fixSibling != (int *)g_packfs_tree_nil_guess)
                            ((int *)*fixSibling)[1] = tmp;
                        fixSibling[1] = *(int *)(tmp + 4);
                        if (tmp == header[1])
                        {
                            header[1] = (int)fixSibling;
                        }
                        else
                        {
                            tmpNode = *(int **)(tmp + 4);
                            if (tmp == *tmpNode)
                                *tmpNode = (int)fixSibling;
                            else
                                tmpNode[2] = (int)fixSibling;
                        }
                        *fixSibling = tmp;
                        *(int **)(tmp + 4) = fixSibling;
                        fixSibling = *(int **)(fixNode[1] + 8);
                    }
                    if ((*(int *)(*fixSibling + 0x14) != 1) ||
                        (*(int *)(fixSibling[2] + 0x14) != 1))
                    {
                        if (*(int *)(fixSibling[2] + 0x14) == 1)
                        {
                            *(int *)(*fixSibling + 0x14) = 1;
                            tmp = *fixSibling;
                            fixSibling[5] = 0;
                            *fixSibling = *(int *)(tmp + 8);
                            if (*(int **)(tmp + 8) !=
                                (int *)g_packfs_tree_nil_guess)
                                (*(int **)(tmp + 8))[1] = (int)fixSibling;
                            *(int *)(tmp + 4) = fixSibling[1];
                            if (fixSibling == (int *)header[1])
                            {
                                header[1] = tmp;
                            }
                            else
                            {
                                tmpNode = (int *)fixSibling[1];
                                if (fixSibling == (int *)tmpNode[2])
                                    tmpNode[2] = tmp;
                                else
                                    *tmpNode = tmp;
                            }
                            *(int **)(tmp + 8) = fixSibling;
                            fixSibling[1] = tmp;
                            fixSibling = *(int **)(fixNode[1] + 8);
                        }
                        fixSibling[5] = *(int *)(fixNode[1] + 0x14);
                        *(int *)(fixNode[1] + 0x14) = 1;
                        *(int *)(fixSibling[2] + 0x14) = 1;
                        parent = (int *)fixNode[1];
                        sibling = (int *)parent[2];
                        parent[2] = *sibling;
                        if ((int *)*sibling != (int *)g_packfs_tree_nil_guess)
                            ((int *)*sibling)[1] = (int)parent;
                        sibling[1] = parent[1];
                        if (parent == (int *)header[1])
                        {
                            header[1] = (int)sibling;
                            *sibling = (int)parent;
                        }
                        else
                        {
                            fixSibling = (int *)parent[1];
                            if (parent == (int *)*fixSibling)
                            {
                                *fixSibling = (int)sibling;
                                *sibling = (int)parent;
                            }
                            else
                            {
                                fixSibling[2] = (int)sibling;
                                *sibling = (int)parent;
                            }
                        }
                        parent[1] = (int)sibling;
                        break;
                    }
                }
                else
                {
                    if (fixSibling[5] == 0)
                    {
                        fixSibling[5] = 1;
                        *(int *)(fixNode[1] + 0x14) = 0;
                        fixSibling = (int *)fixNode[1];
                        tmp = *fixSibling;
                        *fixSibling = *(int *)(tmp + 8);
                        if (*(int **)(tmp + 8) !=
                            (int *)g_packfs_tree_nil_guess)
                            (*(int **)(tmp + 8))[1] = (int)fixSibling;
                        *(int *)(tmp + 4) = fixSibling[1];
                        if (fixSibling == (int *)header[1])
                        {
                            header[1] = tmp;
                        }
                        else
                        {
                            tmpNode = (int *)fixSibling[1];
                            if (fixSibling == (int *)tmpNode[2])
                                tmpNode[2] = tmp;
                            else
                                *tmpNode = tmp;
                        }
                        *(int **)(tmp + 8) = fixSibling;
                        fixSibling[1] = tmp;
                        fixSibling = *(int **)fixNode[1];
                    }
                    if ((*(int *)(fixSibling[2] + 0x14) != 1) ||
                        (*(int *)(*fixSibling + 0x14) != 1))
                    {
                        if (*(int *)(*fixSibling + 0x14) == 1)
                        {
                            *(int *)(fixSibling[2] + 0x14) = 1;
                            tmpNode = (int *)fixSibling[2];
                            fixSibling[5] = 0;
                            fixSibling[2] = *tmpNode;
                            if ((int *)*tmpNode !=
                                (int *)g_packfs_tree_nil_guess)
                                ((int *)*tmpNode)[1] = (int)fixSibling;
                            tmpNode[1] = fixSibling[1];
                            if (fixSibling == (int *)header[1])
                            {
                                header[1] = (int)tmpNode;
                            }
                            else
                            {
                                ptr = (int *)fixSibling[1];
                                if (fixSibling == (int *)*ptr)
                                    *ptr = (int)tmpNode;
                                else
                                    ptr[2] = (int)tmpNode;
                            }
                            *tmpNode = (int)fixSibling;
                            fixSibling[1] = (int)tmpNode;
                            fixSibling = *(int **)fixNode[1];
                        }
                        fixSibling[5] = *(int *)(fixNode[1] + 0x14);
                        *(int *)(fixNode[1] + 0x14) = 1;
                        *(int *)(*fixSibling + 0x14) = 1;
                        parent = (int *)fixNode[1];
                        sibling = (int *)*parent;
                        parent[0] = sibling[2];
                        if ((int *)sibling[2] != (int *)g_packfs_tree_nil_guess)
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
                fixSibling[5] = 0;
                fixNode = (int *)fixNode[1];
            } while (fixNode != (int *)header[1]);
        }
        fixNode[5] = 1;
    }
    EQ_METHOD(0, void, 0x1003EA1C, (void *), &lockit)();
    refObject = (PackFsRefCounted_guess *)unlinked[4];
    if (refObject != 0 &&
        (tmp = refObject->refCount,
         refObject->refCount = tmp - 1,
         tmp - 1 == 0) &&
        refObject != 0)
    {
        refObject->destroy_guess(1);
    }
    EQ_CDECL(void, 0x10033800, (void *))(unlinked);
    *(int *)((char *)tree + 0x0c) =
        *(int *)((char *)tree + 0x0c) - 1;
    *result = (int)node;
    return;
}

// FUNCTION: EQMAIN 0x1000fad0
// Looks up a key in the main pack resource tree, using the head node to mark a miss.
// Cloned from trilogy PackFsTree_guess::find_guess 0x004FA5F0 (the same original code: BinDiff sibling, names/siblings/).
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

// FUNCTION: EQMAIN 0x1000fb40
// Finds or creates a pack index node for a key, honoring the tree's duplicate insertion mode.
// @early-stop: ECX/EDX allocation, merged insertion returns, dummy EDX setup, and result-store epilogues still differ.
// Cloned from trilogy RedBlackTreeLookup_guess::FindOrInsertNodeInRedBlackTree_guess 0x004FA660 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, writes the node holding *key to result, else inserts through 0x100100A0 (inserted 1). mode forces insertion.
RedBlackTreeFindResult_guess RedBlackTreeLookup_guess::FindOrInsertNodeInRedBlackTree_guess(unsigned int *key)
{
    PackFsTreeNode_guess *header = head;
    PackFsTreeNode_guess *x = header->parent;
    PackFsTreeNode_guess *y = header;
    RedBlackTreeFindResult_guess result(0, 0);
    bool less;
    void *insertedNodeValue;
    unsigned int *originalKey;
    /* fake match: keep the sentinel global reread at the original loop latch. */
    PackFsTreeNode_guess *volatile &nil = g_packfs_red_black_tree_nil_guess;

    less = true;
    if (x != g_packfs_red_black_tree_nil_guess)
    {
        do
        {
            y = x;
            less = *key < y->key;
            if (less)
                x = y->left;
            else
                x = y->right;
        } while (x != nil);
    }
    if (mode != 0)
    {
        result = RedBlackTreeFindResult_guess(*EQ_METHOD(4, PackFsTreeNode_guess **, 0x100100A0, (RedBlackTreeLookup_guess *, void **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&insertedNodeValue, x, y, key), 1);
        return result;
    }

    // The original reuses the incoming key-pointer slot for the predecessor iterator.
    originalKey = key;
    key = (unsigned int *)y;
    if (less)
    {
        if (y == header->left)
        {
            result = RedBlackTreeFindResult_guess(*EQ_METHOD(4, PackFsTreeNode_guess **, 0x100100A0, (RedBlackTreeLookup_guess *, void **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&insertedNodeValue, x, y, originalKey), 1);
            return result;
        }
        EQ_METHOD(0, void, 0x10010360, (PackFsTreeNode_guess **), (PackFsTreeNode_guess **)&key)();
    }
    if (((PackFsTreeNode_guess *)key)->key < *originalKey)
    {
        result = RedBlackTreeFindResult_guess(*EQ_METHOD(4, PackFsTreeNode_guess **, 0x100100A0, (RedBlackTreeLookup_guess *, void **, PackFsTreeNode_guess *, PackFsTreeNode_guess *, unsigned int *), this)(&insertedNodeValue, x, y, originalKey), 1);
        return result;
    }
    result = RedBlackTreeFindResult_guess((PackFsTreeNode_guess *)key, 0);
    return result;
}

#undef g_packfs_tree2_nil_guess
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x1004CF18)
// FUNCTION: EQMAIN 0x1000fc30
// Searches the secondary pack tree for a key; its separate index role is unclear.
// Cloned from trilogy PackFsTree2_guess::find_guess 0x004FA750 (the same original code: BinDiff sibling, names/siblings/).
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

// FUNCTION: EQMAIN 0x1000fca0
// Releases entries in a pack lookup subtree as its nodes are removed.
// Cloned from trilogy PackFsTree_guess::erase_guess 0x004FA7C0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall (this only passed on), 1 stack arg. Frees the subtree at x: recurses into each right
// subtree, then walks left, releasing the value at 0xc (0x1000ECD0) and deleting each node.
void PackFsTree_guess::erase_guess(PackFsTreeNode_guess *x)
{
    PackFsTreeNode_guess *y;

    for (y = x; y != g_packfs_tree_nil_guess; x = y)
    {
        erase_guess(y->right);
        y = y->left;
        EQ_METHOD(0, void, 0x1000ECD0, (void *), &x->key)();
        EQ_CDECL(void, 0x10033800, (void *))(x);
    }
}

// FUNCTION: EQMAIN 0x1000FCF0
// Adds a new entry (0x18-byte node) to the tree whose nil is 0x1004CF14 at a chosen parent, then rebalances
// @early-stop: root-check register order and nested rotation blocks still differ from the original.
// Cloned from trilogy PackFsTreeInsert_guess::insert_guess 0x004FA810 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall (this only passed on), 4 stack args, RET 0x10. Inserts a new 0x18-byte node (color 0 = red) under y:
// as the left child when y is the head, x is not nil or *keyValue is below y's key, else as the right
// child. Then the red-black fix-up, *result = the node, and returns result (EAX).
int PackFsTreeInsert_guess::insert_guess(PackFsTreeIterator_guess *result, int x, int *y, unsigned int *keyValue)
{
    int pivot;
    int *ancestor;
    int *newNode;
    int *current;
    int *parent;
    int *child;

    newNode = (int *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x18);
    newNode[1] = (int)y;
    newNode[5] = 0;
    newNode[0] = (int)g_packfs_tree_nil_guess;
    newNode[2] = (int)g_packfs_tree_nil_guess;
    EQ_CDECL(void, 0x10010C20, (void *, void *))(&newNode[3], keyValue);
    this->size = this->size + 1;
    if ((y == this->head) || (x != (int)g_packfs_tree_nil_guess) ||
        (*keyValue < (unsigned int)y[3]))
    {
        *y = (int)newNode;
        current = this->head;
        if (y == current)
        {
            current[1] = (int)newNode;
            this->head[2] = (int)newNode;
        }
        else if (y == (int *)*current)
            *current = (int)newNode;
    }
    else
    {
        y[2] = (int)newNode;
        if (y == (int *)this->head[2])
            this->head[2] = (int)newNode;
    }
    current = newNode;
    if (current != (int *)this->head[1])
    {
        do
        {
            parent = (int *)current[1];
            if (parent[5] != 0)
                break;
            child = (int *)*(int **)parent[1];
            if (parent == child)
            {
                pivot = ((int *)parent[1])[2];
                if (*(int *)(pivot + 0x14) == 0)
                {
                    parent[5] = 1;
                    *(int *)(pivot + 0x14) = 1;
                    *(int *)(*(int *)(current[1] + 4) + 0x14) = 0;
                    current = (int *)*(int *)(current[1] + 4);
                    continue;
                }
                if (current == (int *)parent[2])
                {
                    current = (int *)parent[2];
                    parent[2] = *current;
                    if (*current != (int)g_packfs_tree_nil_guess)
                        *(int **)(*current + 4) = parent;
                    current[1] = parent[1];
                    if (parent == (int *)this->head[1])
                        this->head[1] = (int)current;
                    else
                    {
                        child = (int *)parent[1];
                        if (parent == (int *)*child)
                            *child = (int)current;
                        else
                            child[2] = (int)current;
                    }
                    *current = (int)parent;
                    parent[1] = (int)current;
                    current = parent;
                }
                *(int *)(current[1] + 0x14) = 1;
                *(int *)(*(int *)(current[1] + 4) + 0x14) = 0;
                parent = *(int **)(current[1] + 4);
                child = (int *)*parent;
                *parent = child[2];
                if (child[2] != (int)g_packfs_tree_nil_guess)
                    *(int **)(child[2] + 4) = parent;
                child[1] = parent[1];
                if (parent == (int *)this->head[1])
                {
                    this->head[1] = (int)child;
                    child[2] = (int)parent;
                }
                else
                {
                    ancestor = (int *)parent[1];
                    if (parent == (int *)ancestor[2])
                    {
                        ancestor[2] = (int)child;
                        child[2] = (int)parent;
                    }
                    else
                    {
                        *ancestor = (int)child;
                        child[2] = (int)parent;
                    }
                }
                parent[1] = (int)child;
            }
            else
            {
                if (child[5] == 0)
                {
                    parent[5] = 1;
                    child[5] = 1;
                    *(int *)(*(int *)(current[1] + 4) + 0x14) = 0;
                    current = (int *)*(int *)(current[1] + 4);
                    continue;
                }
                if (current == (int *)*parent)
                {
                    pivot = *parent;
                    *parent = *(int *)(pivot + 8);
                    if (*(int *)(pivot + 8) != (int)g_packfs_tree_nil_guess)
                        *(int **)(*(int *)(pivot + 8) + 4) = parent;
                    *(int *)(pivot + 4) = parent[1];
                    if (parent == (int *)this->head[1])
                        this->head[1] = pivot;
                    else
                    {
                        current = (int *)parent[1];
                        if (parent == (int *)current[2])
                            current[2] = pivot;
                        else
                            *current = pivot;
                    }
                    *(int **)(pivot + 8) = parent;
                    parent[1] = pivot;
                    current = parent;
                }
                *(int *)(current[1] + 0x14) = 1;
                *(int *)(*(int *)(current[1] + 4) + 0x14) = 0;
                parent = *(int **)(current[1] + 4);
                child = (int *)parent[2];
                parent[2] = *child;
                if (*child != (int)g_packfs_tree_nil_guess)
                    *(int **)(*child + 4) = parent;
                child[1] = parent[1];
                if (parent == (int *)this->head[1])
                    this->head[1] = (int)child;
                else
                {
                    ancestor = (int *)parent[1];
                    if (parent == (int *)*ancestor)
                        *ancestor = (int)child;
                    else
                        ancestor[2] = (int)child;
                }
                *child = (int)parent;
                parent[1] = (int)child;
                continue;
            }
        } while (current != (int *)this->head[1]);
    }
    *(int *)(this->head[1] + 0x14) = 1;
    result->node = (PackFsTreeNode_guess *)newNode;
    return (int)result;
}

// FUNCTION: EQMAIN 0x1000FF60
// __thiscall, 1 stack arg (the node x). Left rotation at x: its right child takes x's place, x becomes the
// child's left child, and the child's old left subtree becomes x's right. Returns nothing.
// The nil node of this image is the global at 0x1004CF14 (the trilogy one at 0x006F039C is not used here).
#define g_packfs_tree_nil_eqmain_guess EQ_GLOBAL(PackFsTreeNode_guess *, 0x1004CF14)

void __fastcall PackFsTree_rotateLeft_guess(PackFsTree_guess *self, int edx_unused, PackFsTreeNode_guess *current)
{
    PackFsTreeNode_guess *child;
    PackFsTreeNode_guess *parentNode;

    child = current->right;
    current->right = child->left;
    if (child->left != g_packfs_tree_nil_eqmain_guess)
        child->left->parent = current;
    child->parent = current->parent;
    if (current == self->head->parent)
    {
        self->head->parent = child;
        child->left = current;
        current->parent = child;
        return;
    }
    parentNode = current->parent;
    if (current == parentNode->left)
    {
        parentNode->left = child;
        child->left = current;
        current->parent = child;
        return;
    }
    parentNode->right = child;
    child->left = current;
    current->parent = child;
    return;
}

// FUNCTION: EQMAIN 0x1000FFC0
// __thiscall, 1 stack arg (the node x). Right rotation at x: its left child takes x's place, x becomes the
// child's right child, and the child's old right subtree becomes x's left. Returns nothing.
void __fastcall PackFsTree_rotateRight_guess(PackFsTree_guess *self, int edx_unused, PackFsTreeNode_guess *current)
{
    PackFsTreeNode_guess *child;
    PackFsTreeNode_guess *parentNode;

    child = current->left;
    current->left = child->right;
    if (child->right != g_packfs_tree_nil_eqmain_guess)
        child->right->parent = current;
    child->parent = current->parent;
    if (current == self->head->parent)
    {
        self->head->parent = child;
        child->right = current;
        current->parent = child;
        return;
    }
    parentNode = current->parent;
    if (current == parentNode->right)
    {
        parentNode->right = child;
        child->right = current;
        current->parent = child;
        return;
    }
    parentNode->left = child;
    child->right = current;
    current->parent = child;
    return;
}

// FUNCTION: EQMAIN 0x10010020
// Creates a blank node for adding a resource key to the pack tree.
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

// FUNCTION: EQMAIN 0x10010040
// Moves a tree cursor to its previous entry, including the end-marker case.
// @early-stop: the right-walk load base and one climb-loop compare temporary still differ after eight non-improving iterations.
// __fastcall, no stack args (the iterator's address is in ECX), returns nothing. Steps the iterator *self back one
// node in order. A node whose parent's parent is itself, with the word at +0x14 clear, steps to its right link: that
// is normally the header, and a root with the word clear matches too. A node with a left child steps to the rightmost
// node of that subtree. Otherwise it climbs the parent links while it is a left child, then steps to that parent.
// PackFsTreeNode_guess has no field at +0x14: read as an int.
void __fastcall PackFsTreeIterator_decrement_guess(int *iteratorSlot)
{
    int subtreeNode;
    int rightChild;
    int *nodePtr;
    volatile int *current = iteratorSlot; /* fake match: preserve the iterator-slot reload used by the original climb. */

    nodePtr = (int *)*iteratorSlot;
    if (nodePtr[5] == 0 && *(int **)(nodePtr[1] + 4) == nodePtr)
    {
        *iteratorSlot = nodePtr[2];
        return;
    }
    subtreeNode = *nodePtr;
    if (subtreeNode != (int)g_packfs_tree_nil_eqmain_guess)
    {
        for (rightChild = *(int *)(subtreeNode + 8); rightChild != (int)g_packfs_tree_nil_eqmain_guess;
             subtreeNode = rightChild, rightChild = *(int *)(subtreeNode + 8))
            ;
        *iteratorSlot = subtreeNode;
        return;
    }
    else
    {
        nodePtr = (int *)nodePtr[1];
        while (*current == *nodePtr)
        {
            *iteratorSlot = (int)nodePtr;
            nodePtr = (int *)nodePtr[1];
        }
        *iteratorSlot = (int)nodePtr;
        return;
    }
}

// FUNCTION: EQMAIN 0x100100A0
// Adds a new entry (0x20-byte node, 16-byte value) to the map whose nil is 0x1004CF18, then rebalances
// @early-stop: rotation-block register/load scheduling and exit-register allocation still differ.
// __thiscall in the original (self in ECX, four stack args, RET 0x10). VC6 rejects __thiscall on a free function, so
// EDX is a dummy here, as in EQ_THISCALL. Returns insertedNodeValue (the out slot, still in EAX at the RET). Adds a red node
// (0x20 bytes) under the hint y and counts it at self+0xC. It goes on the left of the hint when the hint is the
// header, x is not nil, or *key < hint key; else on the right. Then it rebalances upward (red-black insert).
// Header words: +0x0 leftmost, +0x4 root, +0x8 rightmost. Node words: +0x0 left, +0x4 parent, +0x8 right, +0xC key,
// +0x14/+0x18 take the key object's words at +0x8/+0xC (copyKeyValueQuad_guess), +0x1C color (1 black, 0 red).
// The shared nil node is at 0x1004CF18. Raw int indexing past +0xC, as the sibling does.
int ** __fastcall PackFsTree_insertNode_guess(void *self, int dummy, int **insertedNodeValue, int x, int *y, unsigned int *key)
{
    int *uncle;
    int *ancestor;
    int *node;
    int *current;
    int *parent;
    int *sibling;

    node = (int *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x20);
    node[1] = (int)y;
    node[7] = 0;
    node[0] = (int)EQ_GLOBAL(int *, 0x1004CF18);
    node[2] = (int)EQ_GLOBAL(int *, 0x1004CF18);
    EQ_CDECL(void, 0x10010C40, (unsigned int *, const unsigned int *))
        ((unsigned int *)(node + 3), key);
    *(int *)((char *)self + 0xC) = *(int *)((char *)self + 0xC) + 1;
    if ((y == *(int **)((char *)self + 4)) ||
        (x != (int)EQ_GLOBAL(int *, 0x1004CF18)) ||
        (*key < (unsigned int)y[3]))
    {
        *y = (int)node;
        current = *(int **)((char *)self + 4);
        if (y == current)
        {
            current[1] = (int)node;
            *(int **)(*(int *)((char *)self + 4) + 8) = node;
        }
        else if (y == (int *)*current)
        {
            *current = (int)node;
        }
    }
    else
    {
        y[2] = (int)node;
        if (y == *(int **)(*(int *)((char *)self + 4) + 8))
        {
            *(int **)(*(int *)((char *)self + 4) + 8) = node;
        }
    }
    current = node;
    while (current != *(int **)(*(int *)((char *)self + 4) + 4))
    {
        parent = (int *)current[1];
        if (parent[7] != 0)
            break;
        sibling = *(int **)parent[1];
        if (parent == sibling)
        {
            uncle = (int *)((int *)parent[1])[2];
            if (uncle[7] == 0)
            {
                parent[7] = 1;
                uncle[7] = 1;
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
                    if (*parent != (int)EQ_GLOBAL(int *, 0x1004CF18))
                        *(int **)(*parent + 4) = current;
                    parent[1] = current[1];
                    if (current == *(int **)(*(int *)((char *)self + 4) + 4))
                    {
                        *(int **)(*(int *)((char *)self + 4) + 4) = parent;
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
                if (sibling[2] != (int)EQ_GLOBAL(int *, 0x1004CF18))
                    *(int **)(sibling[2] + 4) = parent;
                sibling[1] = parent[1];
                if (parent == *(int **)(*(int *)((char *)self + 4) + 4))
                {
                    *(int **)(*(int *)((char *)self + 4) + 4) = sibling;
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
                    current = (int *)*parent;
                    *parent = current[2];
                    if (current[2] != (int)EQ_GLOBAL(int *, 0x1004CF18))
                        *(int **)(current[2] + 4) = parent;
                    current[1] = parent[1];
                    if (parent == *(int **)(*(int *)((char *)self + 4) + 4))
                    {
                        *(int **)(*(int *)((char *)self + 4) + 4) = current;
                    }
                    else
                    {
                        sibling = (int *)parent[1];
                        if (parent == (int *)sibling[2])
                            sibling[2] = (int)current;
                        else
                            *sibling = (int)current;
                    }
                    current[2] = (int)parent;
                    parent[1] = (int)current;
                    current = parent;
                }
                *(int *)(current[1] + 0x1C) = 1;
                *(int *)(*(int *)(current[1] + 4) + 0x1C) = 0;
                parent = *(int **)(current[1] + 4);
                sibling = (int *)parent[2];
                parent[2] = *sibling;
                if (*sibling != (int)EQ_GLOBAL(int *, 0x1004CF18))
                    *(int **)(*sibling + 4) = parent;
                sibling[1] = parent[1];
                if (parent == *(int **)(*(int *)((char *)self + 4) + 4))
                {
                    *(int **)(*(int *)((char *)self + 4) + 4) = sibling;
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
    *(int *)(*(int *)(*(int *)((char *)self + 4) + 4) + 0x1C) = 1;
    *insertedNodeValue = node;
    return insertedNodeValue;
}

// FUNCTION: EQMAIN 0x10010310
// Moves a tree cursor to the next in-order entry.
// Cloned from trilogy PackFsTreeIterator_guess::increment_guess 0x004FAE30 (the same original code: BinDiff sibling, names/siblings/).
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
#define g_packfs_tree2_nil_guess EQ_GLOBAL(PackFsTree2Node_guess *, 0x1004CF18)
// FUNCTION: EQMAIN 0x10010360
// Moves the lookup cursor to its preceding tree entry.
// Cloned from trilogy PackFsTree2Iterator_guess::decrement_guess 0x004FAE80 (the same original code: BinDiff sibling, names/siblings/).
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

// FUNCTION: EQMAIN 0x100103c0
// Copies a shared object pointer and retains the object it points to.
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

// Local layout guess: a 12-byte function object (operator_new(0xc)): vtable pointer at 0x0 (the table at
// 0x10041f58, whose slot 0 destroys the object), reference count at 0x4, the stored argument at 0x8.
struct PackFsFunctionObject_guess {
    void *vtable; // 0x0
    int refCount; // 0x4
    int argument; // 0x8
};

// Local layout guess: a holder whose first field points at a function object.
struct PackFsFunctionHolder_guess {
    PackFsFunctionObject_guess *object; // 0x0

    PackFsFunctionHolder_guess *setFunction_guess(int argument_guess);
};

// FUNCTION: EQMAIN 0x100103e0
// Stores a nonzero function value in the holder.
// __thiscall, 1 stack arg (an int). Clears the holder; for a non-zero argument, allocates a 12-byte function
// object, stores the argument in it, points the holder at it and takes a reference. When the allocation
// fails, the holder stays 0 and the refcount at +4 of the null object is incremented (a write to address 4,
// kept as the original has it). Leaves this in EAX.
PackFsFunctionHolder_guess *PackFsFunctionHolder_guess::setFunction_guess(int argument_guess)
{
    PackFsFunctionObject_guess *created;

    object = 0;
    if (argument_guess != 0)
    {
        created = (PackFsFunctionObject_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(12);
        if (created != 0)
        {
            created->refCount = 0;
            created->argument = argument_guess;
            created->vtable = EQ_ARRAY(void *, 0x10041F58);
            object = created;
            created->refCount = created->refCount + 1;
            return this;
        }
        object = 0;
        created->refCount++;
    }
    return this;
}

// Local layout guess: the base of the destructor below, a plain struct (not polymorphic: its vtable word is written
// explicitly), so the vtable values stay the client's (0x10040DFC for the base, 0x10041F58 for the derived). The base
// destructor is inlined into the derived one: it drops a reference, and calls slot-0 virtual destroy (flag 1) at zero.
struct PackFsRefBase_guess {
    unsigned int vtbl; // 0x0
    int refCount; // 0x4

    ~PackFsRefBase_guess()
    {
        vtbl = 0x10040DFC;
        if (--refCount == 0 && this != 0)
            ((PackFsRefCounted_guess *)this)->destroy_guess(1);
    }
};

// Local layout guess: the reference counted object whose destructor is below, owning a std::map at 0x8.
struct PackFsRefDerived_guess : PackFsRefBase_guess {
    PackFsMap_guess *map; // 0x8

    ~PackFsRefDerived_guess();
};

// FUNCTION: EQMAIN 0x10010430
// Runs the map owner's destructor and frees its allocation when requested.
// __thiscall (callee pops flags), written as __fastcall with an unused EDX slot so `this` arrives in ECX. Scalar
// deleting destructor of PackFsRefDerived_guess: runs the destructor at 0x10010450, frees the object with the cdecl
// free at 0x10033800 when bit 0 of flags is set, returns this.
PackFsRefDerived_guess *__fastcall DestroyFunctionObject_guess(PackFsRefDerived_guess *self, int /* EDX unused */, unsigned char flags)
{
    EQ_METHOD(0, void, 0x10010450, (void *), self)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(self);
    return self;
}

// FUNCTION: EQMAIN 0x10010450
// Releases every entry in a map owner and frees the map storage.
// Destructor of PackFsRefDerived_guess without the deleting part (__thiscall, no stack args, RET 0). Stores the derived
// vtable (0x10041F58), then if the map exists: inlined ~map = erase(begin(), end()) (the whole tree is freed by
// 0x10010A10 when the range is the full tree, else the nodes are erased one by one with 0x10010A50 / 0x100105C0), the
// head node is freed, and the shared nil node is released under std::_Lockit (freed when its count 0x1004CF1C drops
// to zero). The map itself is freed last. The base destructor runs last (inlined): it stores the base vtable
// (0x10040DFC), drops refCount and calls the slot-0 virtual destroy(1) when it reaches zero. The base subobject is
// alive across the body, so the operator delete calls (C++) keep its unwind state.
PackFsRefDerived_guess::~PackFsRefDerived_guess()
{
    PackFsMap_guess *m;
    PackFsMapNode_guess *head;
    PackFsMapNode_guess *first;
    PackFsMapNode_guess *last;
    PackFsMapNode_guess *ret;
    PackFsMapNode_guess *tmp1;
    PackFsMapNode_guess *tmp2;
    PackFsMapNode_guess *tmp3;
    PackFsMapNode_guess **p;
    PackFsMapNode_guess *t;
    unsigned int treeSize;
    bool firstDiff;

    vtbl = 0x10041F58;
    m = map;
    if (m != 0)
    {
        {
            head = m->head;
            treeSize = m->size;
            last = head;
            first = head->left;
            firstDiff = 1;
            if (treeSize != 0)
                firstDiff = first != *EQ_METHOD(1, PackFsMapNode_guess **, 0x100105B0, (PackFsMap_guess *, PackFsMapNode_guess **), m)(&tmp1);
            if (firstDiff == 0 && last == m->head)
            {
                EQ_METHOD(1, void, 0x10010A10, (PackFsMap_guess *, PackFsMapNode_guess *), m)(m->head->parent);
                m->head->parent = g_packfs_map_nil_guess;
                m->size = 0;
                m->head->left = m->head;
                m->head->right = m->head;
                EQ_METHOD(1, PackFsMapNode_guess **, 0x100105B0, (PackFsMap_guess *, PackFsMapNode_guess **), m)(&ret);
            }
            else
            {
                while (first != last)
                {
                    p = EQ_METHOD(2, PackFsMapNode_guess **, 0x10010A50, (PackFsMapNode_guess **, PackFsMapNode_guess **, int), &first)(&tmp2, 0);
                    EQ_METHOD(2, PackFsMapNode_guess **, 0x100105C0, (PackFsMap_guess *, PackFsMapNode_guess **, PackFsMapNode_guess *), m)(&tmp3, *p);
                }
                ret = first;
            }
            EQ_CDECL_CPP(void, 0x10033800, (void *))(m->head);
            m->head = 0;
            m->size = 0;
        }
        t = 0;
        {
            PackFsLockit_guess lk;

            if (--g_packfs_map_nilrefs_guess == 0)
            {
                t = g_packfs_map_nil_guess;
                g_packfs_map_nil_guess = 0;
            }
        }
        if (t != 0)
            EQ_CDECL_CPP(void, 0x10033800, (void *))(t);
        EQ_CDECL_CPP(void, 0x10033800, (void *))(m);
    }
}

// File-local stand-in for the by-value argument of PackFsRefHandle_guess::incrementReferenceCount_guess: the same
// single pointer, with the destructor the original inlines.
struct PackFsRefArgument_guess
{
    PackFsRefCounted_guess *object; // 0x0

    ~PackFsRefArgument_guess()
    {
        if (object != 0 && --object->refCount == 0)
            object->destroy_guess(1);
    }
};

// Local layout guess: a holder whose pointer at 0x4 points at a dword value (the first dword is read).
struct PackFsValueHolder_guess {
    char pad0[4];
    unsigned int *target; // 0x4

    unsigned int *copyTargetValue_guess(unsigned int *out);
};

// FUNCTION: EQMAIN 0x100105b0
// Copies the map head pointer for boundary checks.
// __thiscall, 1 stack arg (RET 4, the callee pops it). Stores the dword that `target` points at into *out,
// then returns out (EAX). `target` is this+0x4.
unsigned int *PackFsValueHolder_guess::copyTargetValue_guess(unsigned int *out)
{
    *out = *target;
    return out;
}

// FUNCTION: EQMAIN 0x100105c0
// Erases one entry of the 0x20-byte-node map (nil 0x1004CF18) and frees it; no held reference is dropped
// Same original code as trilogy PackFsMap_erase_iterator_guess 0x004FB100 (BinDiff sibling); the callees and
// globals are this image's own. __thiscall, 2 stack args (`out`, `where`), callee pops 8; written as __fastcall
// with `self` in ECX and an EDX dummy. std::map erase(iterator) on the tree at self (head at 0x4, size at 0xc).
// `where` is first advanced in place to its in-order successor (0x100109C0) and that successor is stored to *out
// and returned in EAX (the hidden return pointer). The node is spliced out (its successor takes its place when it
// has two children), the tree is rebalanced under std::_Lockit (colour 1 = black, 0 = red), then the node is
// deleted (0x10033800) and size drops by one.
PackFsMapNode_guess **__fastcall PackFsMap_erase_iterator_guess(PackFsMap_guess *self, int edxDummy,
    PackFsMapNode_guess **out, PackFsMapNode_guess *where)
{
    PackFsMapNode_guess *z;
    PackFsMapNode_guess *x;
    PackFsMapNode_guess *y;
    PackFsMapNode_guess *nilNode;
    PackFsMapNode_guess *head;
    PackFsMapNode_guess *parent;
    PackFsMapNode_guess *w;
    PackFsMapNode_guess *c;
    PackFsMapNode_guess **right;
    int color;

    z = where;
    EQ_METHOD(0, void, 0x100109C0, (PackFsMapNode_guess **), &where)();
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
        y = *right;
        for (c = y->left; c != nilNode; c = c->left)
            y = c;
        x = y->right;
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
                head->parent = y;
            }
            else
            {
                parent = z->parent;
                if (parent->left == z)
                    parent->left = y;
                else
                    parent->right = y;
            }
            y->parent = z->parent;
            color = y->color;
            y->color = z->color;
            z->color = color;
        }
        else
        {
            x->parent = z->parent;
            head = self->head;
            if (head->parent == z)
            {
                head->parent = x;
            }
            else
            {
                parent = z->parent;
                if (parent->left == z)
                    parent->left = x;
                else
                    parent->right = x;
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
                    for (w = x->left; w != g_packfs_map_nil_guess; w = w->left)
                        c = w;
                    head->left = c;
                }
            }
            head = self->head;
            if (head->right == z)
            {
                if (z->left == g_packfs_map_nil_guess)
                {
                    head->right = z->parent;
                }
                else
                {
                    c = x;
                    for (w = x->right; w != g_packfs_map_nil_guess; w = w->right)
                        c = w;
                    head->right = c;
                }
            }
        }
        if (z->color == 1)
        {
            if (x != self->head->parent)
            {
                do
                {
                    if (x->color != 1)
                        break;
                    w = x->parent->left;
                    if (x == w)
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
                                w->left->parent = parent;
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
                        if (w->left->color != 1 || w->right->color != 1)
                        {
                            if (w->right->color == 1)
                            {
                                w->left->color = 1;
                                c = w->left;
                                w->color = 0;
                                w->left = c->right;
                                if (c->right != g_packfs_map_nil_guess)
                                    c->right->parent = w;
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
                            c = parent->right;
                            parent->right = c->left;
                            if (c->left != g_packfs_map_nil_guess)
                                c->left->parent = parent;
                            c->parent = parent->parent;
                            if (parent == self->head->parent)
                            {
                                self->head->parent = c;
                            }
                            else if (parent == parent->parent->left)
                            {
                                parent->parent->left = c;
                            }
                            else
                            {
                                parent->parent->right = c;
                            }
                            c->left = parent;
                            parent->parent = c;
                            break;
                        }
                    }
                    else
                    {
                        if (w->color == 0)
                        {
                            w->color = 1;
                            x->parent->color = 0;
                            parent = x->parent;
                            w = parent->left;
                            parent->left = w->right;
                            if (w->right != g_packfs_map_nil_guess)
                                w->right->parent = parent;
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
                        if (w->right->color != 1 || w->left->color != 1)
                        {
                            if (w->left->color == 1)
                            {
                                w->right->color = 1;
                                c = w->right;
                                w->color = 0;
                                w->right = c->left;
                                if (c->left != g_packfs_map_nil_guess)
                                    c->left->parent = w;
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
                            c = parent->left;
                            parent->left = c->right;
                            if (c->right != g_packfs_map_nil_guess)
                                c->right->parent = parent;
                            c->parent = parent->parent;
                            if (parent == self->head->parent)
                            {
                                self->head->parent = c;
                            }
                            else if (parent == parent->parent->right)
                            {
                                parent->parent->right = c;
                            }
                            else
                            {
                                parent->parent->left = c;
                            }
                            c->right = parent;
                            parent->parent = c;
                            break;
                        }
                    }
                    w->color = 0;
                    x = x->parent;
                } while (x != self->head->parent);
            }
            x->color = 1;
        }
    }
    EQ_CDECL(void, 0x10033800, (void *))(z);
    self->size = self->size - 1;
    *out = where;
    return out;
}

// FUNCTION: EQMAIN 0x100109c0
// Advances a map erase cursor to its next entry.
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

    nilNode = EQ_GLOBAL(PackFsTreeNode_guess *, 0x1004CF18);
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

// FUNCTION: EQMAIN 0x10010a10
// Clears a map branch and releases all entries beneath it.
// __thiscall on PackFsMap_guess, 1 stack arg (callee pops 4). Frees the map subtree at x by recursing into each
// right subtree, then deleting each node while walking left. The nil node at 0x1004CF18 is reloaded after each delete.
void PackFsMap_guess::erase_guess(PackFsMapNode_guess *x)
{
    PackFsMapNode_guess *y;

    if ((y = x) != EQ_GLOBAL(PackFsMapNode_guess *, 0x1004CF18))
    {
        do
        {
            erase_guess(y->right);
            y = y->left;
            EQ_CDECL(void, 0x10033800, (void *))(x);
            x = y;
        } while (y != EQ_GLOBAL(PackFsMapNode_guess *, 0x1004CF18));
    }
}

// Local layout guess: a map iterator whose node pointer sits at 0x0 (the nodes are PackFsMapNode_guess).
struct PackFsMapIterator_guess
{
    PackFsMapNode_guess *node; // 0x0

    PackFsMapIterator_guess *postIncrement_guess(PackFsMapIterator_guess *result, int);
};

// FUNCTION: EQMAIN 0x10010a50
// Returns the current map entry while positioning the cursor at the next one.
// __thiscall (iterator in ECX), 2 stack args: the hidden return buffer (first) and an unused int (the postfix dummy),
// callee pops 8. Postfix ++ on a map iterator: stores the current node in the buffer, moves the iterator to its
// in-order successor (leftmost node of the right subtree, else the nearest ancestor whose left subtree holds the node),
// and returns the buffer pointer in EAX. Source as trilogy's 0x004fb590 (BinDiff 1.00 there); here only the nil
// global's address differs, and the build matches structurally, not byte-for-byte (the loop's load register).
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

// FUNCTION: EQMAIN 0x10010ab0
// Replaces a handle's object while balancing references to both objects.
// __thiscall, 1 stack arg: the new object, passed by value; the callee runs that argument's destructor at each return.
// Releases the holder's old object (destroying it when its count hits zero), stores the argument in the holder and
// takes a reference on it. When the argument is the holder itself (&arg == this), only the argument's release runs.
// Leaves this in EAX.
// Written as __fastcall with a dummy EDX (same ABI, ECX = this): the header's member takes PackFsRefHandle_guess by
// value, and that type's destructor calls 0x10007520, which this code never calls.
PackFsRefHandle_guess *__fastcall assignReferenceHandle_guess(PackFsRefHandle_guess *self, int, PackFsRefArgument_guess newReference)
{
    PackFsRefCounted_guess *old;

    if ((void *)&newReference == (void *)self)
        return self;
    if (self->object != 0)
    {
        old = self->object;
        int remaining = old->refCount - 1;
        old->refCount = remaining;
        if (remaining == 0 && old != 0)
            old->destroy_guess(1);
    }
    self->object = newReference.object;
    if (newReference.object != 0)
        newReference.object->refCount++;
    return self;
}

// FUNCTION: EQMAIN 0x10010B50
// Releases one shared object reference and destroys the object when none remain.
// Cloned from trilogy decrementReferenceCount_guess 0x004FB5F0 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (the shared pointer in ECX, no stack args, no return value; callee pops 0). Drops one reference on
// the object the shared pointer holds. When the count reaches zero, calls its first virtual function with 1
// (the deleting destructor). The shared pointer itself is not cleared. Null objects are skipped.
// Named apart from 0x10007520 (decrementReferenceCount_guess): the same name with another parameter type is
// an overload, and the test DLL can't resolve it by name.
void __fastcall decrementSharedReferenceCount_guess(PackFsSharedPtr_guess *shared)
{
    PackFsRefCounted_guess *obj;
    int count;

    if (shared->ptr == 0)
        return;
    obj = shared->ptr;

    count = obj->refCount;
    count--;
    obj->refCount = count;
    if (count != 0)
        return;

    if (obj != 0)
        obj->destroy_guess(1);
}

// FUNCTION: EQMAIN 0x10010b70
// Gets the payload pointer held by a pack object handle.
// Cloned from trilogy getObjectDataPointer_guess 0x004FB6D0 (the same original code: BinDiff sibling, names/siblings/).
// __fastcall (this in ECX, no stack args).
void * __fastcall getObjectDataPointer_guess(PackFsHandle_guess *handle)
{
    return handle->object->data;
}

// FUNCTION: EQMAIN 0x10010b80
// Replaces a stored object slot, releasing its old reference and retaining the new one.
// Cloned from trilogy updateReferenceCount_guess 0x004FB6E0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall (holder in ECX), 1 stack arg: the new object, passed by value (RET 4). The argument's destructor is the
// one the original inlines; it runs at each return, which gives the C++ EH frame. Written as __fastcall with a dummy
// EDX (same ABI), like assignReferenceHandle_guess. Replaces *holder with the argument: releases the
// old object (destroying it at zero), stores the argument and takes a reference on it. When the argument is the holder
// itself (&value == holder), only the argument's release runs. Returns the holder (EAX).
PackFsRefCounted_guess **__fastcall updateReferenceCount_guess(PackFsRefCounted_guess **holder, int, PackFsRefArgument_guess value)
{
    PackFsRefCounted_guess *old;

    if ((void *)&value == (void *)holder)
        return holder;
    if (*holder != 0)
    {
        old = *holder;
        int remaining = old->refCount - 1;
        old->refCount = remaining;
        if (remaining == 0 && old != 0)
            old->destroy_guess(1);
    }
    *holder = value.object;
    if (value.object != 0)
        value.object->refCount++;
    return holder;
}

// FUNCTION: EQMAIN 0x10010c20
// Copies a key/value pair while retaining the value object.
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

// FUNCTION: EQMAIN 0x10010c40
// Copies the four-word payload used by a tree entry.
// Cloned from trilogy copyKeyValueQuad_guess 0x004FB7A0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl, 2 stack args. Copies four dwords into `dest` (nothing when it is null). Named apart from the
// 0x10010C20 copyKeyValue_guess so the test harness finds each by name.
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

// Local layout guess: a vtable pointer at 0x0 (the original stores the address 0x10041F60 there, kept as a plain
// pointer), an unknown word at 0x4 and a held value at 0x8.
struct PackFsVtableValue_guess {
    const void *vtable; // 0x0
    int unknown; // 0x4
    int value; // 0x8

    PackFsVtableValue_guess *initializeWithValue_guess(int value_arg);
};

// FUNCTION: EQMAIN 0x10010c70
// Runs held-reference cleanup for an element and frees it when requested.
// Cloned from trilogy SuiteDefaultsElement_guess::ScalarDeletingDtor_guess 0x004FB7D0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall, 1 stack arg. Scalar deleting destructor: releases the held reference, frees the object
// when bit 0 of flags is set, returns this.
SuiteDefaultsElement_guess *SuiteDefaultsElement_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x1000ECD0, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(this);
    return this;
}

// FUNCTION: EQMAIN 0x10010c90
// Initializes an opaque holder with the supplied integer value.
// __thiscall, 1 stack arg (callee pops 4). Clears +0x4, stores the argument at +0x8, sets the vtable at +0x0 to
// 0x10041F60 and leaves this in EAX.
PackFsVtableValue_guess *PackFsVtableValue_guess::initializeWithValue_guess(int value_arg)
{
    unknown = 0;
    value = value_arg;
    vtable = (const void *)0x10041f60;
    return this;
}

// FUNCTION: EQMAIN 0x10010cb0
// Completes deletion of the shared pointer wrapper after its destructor runs.
// Cloned from trilogy PackFsSharedPointer2_guess::ScalarDeletingDtor_guess 0x004FB810 (the same original code: BinDiff sibling, names/siblings/).
PackFsSharedPointer2_guess *PackFsSharedPointer2_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x10010CD0, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(this);
    return this;
}

// Stand-in for the base subobject of this destructor. This struct has no base class, so PackFsRefBase_guess's destructor
// (drop the count, destroy at zero) runs from this local guard when the function returns. The local is what gives the
// original's C++ EH frame.
struct PackFsSharedPointerBaseGuard_guess
{
    PackFsSharedPointer2_guess *object; // 0x0

    ~PackFsSharedPointerBaseGuard_guess()
    {
        ((PackFsRefBase_guess *)object)->~PackFsRefBase_guess();
    }
};

// Local holders for the storage's held pointers. Each keeps the address of its storage field and reads the pointer when
// it goes out of scope, so the loads happen at the release, as in the original.
struct PackFsStorageBaseField_guess
{
    PackFsRefCounted_guess **field; // the storage's base pointer (0x0)

    ~PackFsStorageBaseField_guess()
    {
        PackFsRefCounted_guess *obj;

        if (*field != 0)
        {
            obj = *field;
            EQ_METHOD(0, void, 0x10007520, (PackFsRefCounted_guess *), obj)();
        }
    }
};

struct PackFsStorageField_guess
{
    PackFsRefCounted_guess **field; // the storage's first, second or third pointer (0x8, 0xc, 0x10)

    ~PackFsStorageField_guess()
    {
        PackFsRefCounted_guess *obj;
        int count;

        if (*field != 0)
        {
            obj = *field;
            count = obj->refCount;
            count--;
            obj->refCount = count;
            if (count == 0 && obj != 0)
                obj->destroy_guess(1);
        }
    }
};

// FUNCTION: EQMAIN 0x10010cd0
// Releases the shared objects held in the wrapper's storage block.
// __thiscall, no stack args, no return value (callee pops 0). Sets the vtable to 0x10041F60, releases the held
// pointers of the storage at +0x8 (+0x10, +0xC, +0x8 inline; +0x0 through decrementReferenceCount_guess), frees
// the storage, then sets the vtable to 0x10040DFC, drops the count at +0x4 and, at zero, calls vtable slot 0 with 1.
// @early-stop: EH state stores 3, 2, 1, 0 missing (sibling holder blocks emit none; nested holders spill their field pointers); the vtable store follows the storage load (the original stores it first) and the storage store precedes the test (the original's follows it); the tail calls [0x10040DFC] directly (no [edi] reload).
PackFsSharedPointer2_guess::~PackFsSharedPointer2_guess()
{
    PackFsSharedPointerStorage_guess *storage;
    PackFsSharedPointerStorage_guess *volatile frame_storage; /* fake match: the original keeps storage in a frame slot for the EH cleanup; only shapes the frame slot */
    PackFsSharedPointerBaseGuard_guess base_guard = { this };

    vtable = (const void *)0x10041f60;
    storage = (PackFsSharedPointerStorage_guess *)value;
    frame_storage = storage;
    if (storage != 0)
    {
        {
            PackFsStorageField_guess third = { &storage->third };
        }
        {
            PackFsStorageField_guess second = { &storage->second };
        }
        {
            PackFsStorageField_guess first = { &storage->first };
        }
        {
            PackFsStorageBaseField_guess base = { &storage->base };
        }
        EQ_CDECL(void, 0x10033800, (void *))(storage);
    }
}
