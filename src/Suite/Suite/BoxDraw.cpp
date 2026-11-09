// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// BoxDraw.cpp - small-block pool deallocation used by the Suite containers.

#include <string.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/suite_hash_table.h"

// Free lists for blocks of up to 128 bytes, one per 8-byte size class.
#define pool_free_lists_guess   EQ_ARRAY(unsigned int *, 0x0058FB58)
#define pool_reserved_bytes_guess EQ_GLOBAL(unsigned int, 0x0058FB98)
#define pool_current_guess      EQ_GLOBAL(unsigned int *, 0x0058FB9C)
#define pool_end_guess         EQ_GLOBAL(unsigned int, 0x0058FBA0)

struct BoxDrawPoolMember_guess
{
    unsigned int unused;

    ~BoxDrawPoolMember_guess()
    {
        EQ_METHOD(0, void, 0x00401167, (void *), this)();
    }
};

struct BoxDrawOwner_guess
{
    unsigned int field_0_guess;
    unsigned int field_4_guess;
    BoxDrawPoolMember_guess field_8_guess;

    ~BoxDrawOwner_guess();
};

// The hash table of BoxDrawContainer_guess's hash_map; its bucket initialisation is 0x0040112C.
class BoxDrawContainerTable_guess
{
public:
    SuiteHasher_guess hash;                 // 0x00
    SuiteKeyEqual_guess equals;             // 0x01
    SuiteKeyExtractor_guess getKey;         // 0x02
    SuiteBucketVector_guess buckets;        // 0x04
    SuiteAllocProxy_guess<unsigned int> elementCount; // 0x10

    BoxDrawContainerTable_guess(unsigned int bucketCount, const SuiteHasher_guess& hasher,
        const SuiteKeyEqual_guess& keyEqual, const SuiteAllocator_guess& allocator)
        : hash(hasher), equals(keyEqual), getKey(SuiteKeyExtractor_guess()), buckets(allocator), elementCount(allocator, 0)
    {
        InitializeHashBuckets_guess(bucketCount);
    }
    void InitializeHashBuckets_guess(unsigned int count);
};

// A Suite hash_map: its table with 100 initial buckets.
struct BoxDrawContainer_guess
{
    BoxDrawContainerTable_guess table;           // 0x00

    BoxDrawContainer_guess();
};

struct BoxDrawEHCleanup_guess
{
    ~BoxDrawEHCleanup_guess()
    {
    }
};

// FUNCTION: TRILOGY 0x0040107b
// Initializes the embedded storage member used by a Suite container owner.
u32* __fastcall InitializeContainerMember_guess(u32* obj)
{
    EQ_METHOD(0, void, 0x004010D7, (void *), (void *)((unsigned char *)obj + 4))();
    *obj = 0;
    return obj;
}

// FUNCTION: TRILOGY 0x0040108d
// Releases entries attached to a value owner during teardown.
// @early-stop: EH state stores and the adjusted ESI base still differ.
/* fake match: the volatile pointer preserves VC6's adjusted-table spill. */
BoxDrawOwner_guess::~BoxDrawOwner_guess()
{
    unsigned char * volatile table;

    table = (unsigned char *)this + 4;

    {
        EQ_METHOD(0, void, 0x0040345A, (void *), table)();
    }
}

// FUNCTION: TRILOGY 0x004010D7
// Sets up an empty Suite hash_map with storage for 100 table entries.
BoxDrawContainer_guess::BoxDrawContainer_guess()
    : table(100, SuiteHasher_guess(), SuiteKeyEqual_guess(), SuiteAllocator_guess())
{
}

// Three-pointer buffer header (first / last / end), as the callers' vector-like members lay it out.
struct PoolBuffer_guess
{
    unsigned int *first;    // 0x0
    unsigned int *last;     // 0x4
    unsigned int *end;      // 0x8
};

// FUNCTION: TRILOGY 0x00401127
// A 5-byte tail jump to pool_free_guess (0x00401167); ECX = buffer passes straight through.
void __fastcall pool_free_thunk_guess(PoolBuffer_guess *buf)
{
    EQ_METHOD(0, void, 0x00401167, (PoolBuffer_guess *), buf)();
}

// FUNCTION: TRILOGY 0x0040112c
// Prepares a container's hash buckets for the requested table size.
// The original keeps an EBP frame (push ebp; mov ebp,esp) with no local slot; the reason is not visible
// in the asm, so FPO is off for this function only.
#pragma optimize("y", off)
void BoxDrawContainerTable_guess::InitializeHashBuckets_guess(unsigned int count)
{
    unsigned int new_size;

    new_size = EQ_STDCALL(unsigned int, 0x0040119E, (unsigned int))(count);
    EQ_METHOD(1, void, 0x004011CA, (void *, unsigned int), (char *)this + 4)(new_size);
    count = 0;
    EQ_METHOD(3, void, 0x00401261, (void *, unsigned int, unsigned int, unsigned int *), (char *)this + 4)(*(unsigned int *)((char *)this + 8), new_size, &count);
    *(unsigned int *)((char *)this + 0x10) = 0;
    return;
}
#pragma optimize("y", on)

// FUNCTION: TRILOGY 0x00401167
// Reclaims a container's backing buffer for reuse by the Suite allocator.
// __thiscall (ECX = buffer). Blocks over 128 bytes go back to the client's cdecl operator delete
// (0x005218BB); smaller ones are pushed on the free list of their 8-byte size class.
void __fastcall pool_free_guess(PoolBuffer_guess *buf)
{
    unsigned int *p;
    unsigned int size;

    p = buf->first;
    if (p != 0)
    {
        size = (buf->end - p) * 4;
        if (size > 0x80)
        {
            operator delete(p);
            return;
        }
        size = (size - 1) >> 3;
        *p = (unsigned int)pool_free_lists_guess[size];
        pool_free_lists_guess[size] = p;
    }
}

// Sorted table of 28 hash bucket counts (primes, STL __stl_prime_list shape) in .rdata, 0x00534380..0x005343F0.
#define prime_list_guess        EQ_ARRAY(unsigned int, 0x00534380)
#define prime_list_end_guess    EQ_ARRAY(unsigned int, 0x005343F0)
#define prime_list_last_guess   EQ_GLOBAL(unsigned int, 0x005343EC)

// FUNCTION: TRILOGY 0x0040119E
// Picks a supported hash-table capacity for a container's requested count.
// __stdcall (never touches ECX); STL __stl_next_prime shape (last entry 0xFFFFFFFB). Returns
// the first table entry not below n, or the last entry when n is above all of them. lower_bound_guess is
// called with a fourth (unused) 0 argument.
unsigned int __stdcall next_prime_guess(unsigned int requestedCount)
{
    unsigned int *pos;

    pos = EQ_CDECL(unsigned int *, 0x0040158C, (unsigned int *, unsigned int *, const unsigned int *, int))(
        prime_list_guess, prime_list_end_guess, &requestedCount, 0);
    return pos == prime_list_end_guess ? prime_list_last_guess : *pos;
}

// FUNCTION: TRILOGY 0x004011ca
// Expands a container's backing storage while preserving its current entries.
void __fastcall ReserveBufferCapacity_guess(int *buffer, int, unsigned int requestedCapacity)
{
    unsigned int *oldFirst;
    unsigned int *oldEnd;
    unsigned int *newFirst;
    int oldCount;
    unsigned int oldBytes;

    oldFirst = (unsigned int *)buffer[0];
    if ((unsigned int)(buffer[2] - (int)oldFirst >> 2) < requestedCapacity)
    {
        oldEnd = (unsigned int *)buffer[1];
        oldCount = (int)oldEnd - (int)oldFirst >> 2;
        if (oldFirst != 0)
        {
            newFirst = (unsigned int *)EQ_METHOD(3, unsigned int *, 0x004014F2, (int *, unsigned int, int *, int *), buffer)(requestedCapacity, (int *)oldFirst, (int *)oldEnd);
            oldFirst = (unsigned int *)buffer[0];
            if (oldFirst != 0)
            {
                oldBytes = (unsigned int)((buffer[2] - (int)oldFirst >> 2) * 4);
                if (oldBytes > 0x80)
                {
                    operator delete(oldFirst);
                }
                else
                {
                    oldBytes = (oldBytes - 1) >> 3;
                    *oldFirst = (unsigned int)pool_free_lists_guess[oldBytes];
                    pool_free_lists_guess[oldBytes] = oldFirst;
                }
            }
        }
        else
        {
            if (requestedCapacity != 0)
            {
                newFirst = (unsigned int *)EQ_CDECL(void *, 0x00401366, (unsigned int))(requestedCapacity << 2);
            }
            else
            {
                newFirst = 0;
            }
        }
        buffer[0] = (int)newFirst;
        buffer[1] = (int)newFirst + oldCount * 4;
        buffer[2] = (int)newFirst + requestedCapacity * 4;
    }
    return;
}

// FUNCTION: TRILOGY 0x00401261
// Inserts repeated entries at a chosen point in a Suite dword buffer.
// @early-stop: Register allocation, pointer induction, and spill slots still differ; frame is 12 rather than 8 bytes.
void __fastcall InsertFillBuffer_guess(int *container, int, unsigned int *position, unsigned int count, unsigned int *value)
{
    unsigned int fill;
    unsigned int insertCount;
    unsigned int *dest;
    unsigned int tailCount;
    int extraCount;
    unsigned int *first;
    int copyCount;
    unsigned int *oldEnd;

    insertCount = count;
    if (count != 0)
    {
        if ((unsigned int)(container[2] - container[1] >> 2) >= count)
        {
            tailCount = container[1] - (int)position >> 2;
            fill = *value;
            oldEnd = (unsigned int *)container[1];
            if (tailCount != 0 && count <= tailCount - 1)
            {
                copyCount = count * 4;
                first = oldEnd + -count;
                count = (int)oldEnd - (int)first >> 2;
                dest = oldEnd;
                value = first;
                for (int n = (int)count; n > 0; --n)
                {
                    *dest = *value;
                    dest++;
                    value++;
                }
                container[1] = container[1] + copyCount;
                copyCount = (int)first - (int)position >> 2;
                dest = oldEnd;
                for (; copyCount > 0; --copyCount)
                {
                    --first;
                    --dest;
                    *dest = *first;
                }
                oldEnd = position + insertCount;
                for (; position != oldEnd; position = position + 1)
                {
                    *position = fill;
                }
            }
            else
            {
                extraCount = count - tailCount;
                copyCount = extraCount;
                dest = oldEnd;
                if (extraCount != 0)
                {
                    for (; copyCount != 0; copyCount = copyCount + -1)
                    {
                        *dest = fill;
                        dest = dest + 1;
                    }
                }
                container[1] = container[1] + extraCount * 4;
                dest = (unsigned int *)container[1];
                first = position;
                for (int n = ((int)oldEnd - (int)position) >> 2; n > 0; --n)
                {
                    *dest = *first;
                    dest++;
                    first++;
                }
                container[1] = container[1] + tailCount * 4;
                for (; position != oldEnd; position = position + 1)
                {
                    *position = fill;
                }
            }
        }
        else
        {
            EQ_THISCALL(void, 0x004013B7, (int *, int, unsigned int *, unsigned int *, unsigned int))(
                /* fake match: the ignored EDX argument retains the receiver there, as in the original. */
                container, (int)container, position, value, count);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00401366
// Supplies reusable small blocks for Suite container storage.
void *pool_alloc_guess(unsigned int byteCount)
{
    unsigned int sizeClass;
    unsigned int *block;

    // 0x80 = largest pooled size; bigger blocks go to operator new.
    if (0x80 < byteCount)
    {
        return operator new(byteCount);
    }
    // Size class n holds requests of 8n+1 .. 8n+8 bytes.
    sizeClass = (byteCount - 1) >> 3;
    block = pool_free_lists_guess[sizeClass];
    if (block != 0)
    {
        pool_free_lists_guess[sizeClass] = (unsigned int *)*block;
        return block;
    }
    // Free list empty: allocate a new block.
    return EQ_CDECL(void *, 0x0040152F, (unsigned int))(byteCount);
}

// Head of the 8-byte size class free list (pool_free_lists_guess[1]), addressed on its own here.
#define pool_free_list8_guess   EQ_GLOBAL(unsigned int *, 0x0058FB5C)

// FUNCTION: TRILOGY 0x0040139F
// Returns a freed chain-table node to the client pool for later reuse.
// __thiscall method that ignores this (callee pops the one argument). Pushes a block on the free list
// at 0x0058FB5C.
void __stdcall pool_push_list8_guess(unsigned int *p)
{
    if (p != 0)
    {
        *p = (unsigned int)pool_free_list8_guess;
        pool_free_list8_guess = p;
    }
}

struct BoxDrawDwordBuffer_guess
{
    unsigned int *first;
    unsigned int *end;
    unsigned int *limit;
};

struct BoxDrawFrameMember_guess
{
    ~BoxDrawFrameMember_guess()
    {
    }
};

// FUNCTION: TRILOGY 0x004013b7
// Adds repeated entries while replacing a full Suite dword buffer.
// @early-stop: EH prolog/frame and insertion-loop register/layout still differ after eight non-improving iterations.
void __fastcall ReallocateAndInsertFillBuffer_guess(BoxDrawDwordBuffer_guess *buffer, void *edx,
                             unsigned int *source, unsigned int *value,
                             unsigned int count)
{
    unsigned int capacity;
    unsigned int *growth;
    int totalCapacity;
    unsigned int *newFirst;
    unsigned int *write;
    unsigned int *tailSource;
    int tailCount;
    unsigned int *oldFirst;
    unsigned int oldBytes;

    capacity = (unsigned int)(((int)buffer->end - (int)buffer->first) >> 2);
    growth = &count;
    if (capacity >= *growth)
    {
        growth = &capacity;
    }
    totalCapacity = *growth + capacity;
    if (totalCapacity != 0)
    {
        newFirst = (unsigned int *)EQ_CDECL(void *, 0x00401366, (unsigned int))(totalCapacity * 4);
    }
    else
    {
        newFirst = 0;
    }

    PoolBuffer_guess cleanup;
    cleanup.first = newFirst;
    cleanup.last = 0;

    try
    {
        write = (unsigned int *)EQ_CDECL(void *, 0x004016C4,
                                         (unsigned int *, unsigned int *, unsigned int *, unsigned int, unsigned int))(
            buffer->first, source, newFirst, count, 0);
        if (count == 1)
        {
            if (write != 0)
            {
                *write = *value;
            }
            write++;
        }
        else
        {
            int repeatCount = (int)count;
            while (repeatCount != 0)
            {
                repeatCount--;
                *write = *value;
                write++;
            }
        }

        tailSource = source;
        tailCount = ((int)buffer->end - (int)tailSource) >> 2;
        while (tailCount > 0)
        {
            *write = *tailSource;
            tailSource++;
            write++;
            tailCount--;
        }

        oldFirst = buffer->first;
        if (oldFirst != 0)
        {
            oldBytes = (unsigned int)((((int)buffer->limit - (int)oldFirst) >> 2) * 4);
            if (oldBytes > 0x80)
            {
                operator delete(oldFirst);
            }
            else
            {
                oldBytes = (oldBytes - 1) >> 3;
                *oldFirst = (unsigned int)pool_free_lists_guess[oldBytes];
                pool_free_lists_guess[oldBytes] = oldFirst;
            }
        }

        buffer->end = write;
        buffer->first = newFirst;
        buffer->limit = newFirst + totalCapacity;
    }
    catch (...)
    {
        if (newFirst != 0)
        {
            cleanup.end = newFirst + totalCapacity;
            EQ_METHOD(0, void, 0x00401167, (void *), &cleanup)();
        }
        throw;
    }
}

// FUNCTION: TRILOGY 0x004014f2
// Builds replacement dword storage and carries existing entries into it.
// @early-stop: The source load uses the equivalent SIB address with base and index registers reversed.
unsigned int *__stdcall alloc_and_copy_dwords_guess(int capacity, unsigned char *first, unsigned char *last)
{
    unsigned int *buffer;
    unsigned int *dst;
    int remaining;

    switch (capacity)
    {
    default:
        buffer = (unsigned int *)EQ_CDECL(void *, 0x00401366, (unsigned int))(capacity << 2);
        break;
    case 0:
        buffer = 0;
        break;
    }
    remaining = (int)(last - first) >> 2;
    dst = buffer;
    if (0 < remaining)
    {
        first = (unsigned char *)((int)first - (int)buffer);
        do
        {
            *dst = *(unsigned int *)((int)first + (int)dst);
            dst = dst + 1;
            remaining = remaining + -1;
        } while (remaining != 0);
    }
    return buffer;
}

/* fake match: Duplicate the first-node expression and precompute its successor to match VC6's register flow. */
// FUNCTION: TRILOGY 0x0040152f
// Restocks the free list for the requested small allocation size.
// @early-stop: Register assignment, local initialization order, and table/pointer operands still differ.
void PoolRefillFreeList_guess(int requestedSize)
{
    unsigned int blockCount = 0x14;
    unsigned int *freeBlock;
    int allocationBase;
    unsigned int *nextFreeBlock;
    unsigned int blockIndex;
    unsigned int elementSize;

    elementSize = requestedSize + 7U & 0xfffffff8;
    allocationBase = EQ_CDECL(unsigned int, 0x004015C6, (unsigned int, unsigned int *))(
        elementSize, &blockCount);
    blockIndex = 1;
    if (blockCount == 1)
        return;

    pool_free_lists_guess[(elementSize - 1 >> 3)] = (unsigned int *)(allocationBase + elementSize);
    nextFreeBlock = (unsigned int *)(allocationBase + elementSize);
    freeBlock = nextFreeBlock;
    nextFreeBlock = (unsigned int *)((int)freeBlock + elementSize);
    if (blockCount != 2)
    {
        do
        {
            *freeBlock = (unsigned int)nextFreeBlock;
            blockIndex++;
            freeBlock = nextFreeBlock;
            nextFreeBlock = (unsigned int *)((int)freeBlock + elementSize);
        } while (blockCount - 1 != blockIndex);
    }
    *freeBlock = 0;
}

// FUNCTION: TRILOGY 0x0040158C
// Locates a requested bucket count in the container's supported capacity table.
// __cdecl. Binary search (std::lower_bound shape) over the sorted unsigned range [first, last): returns
// the first element not below *value, or last.
unsigned int *__cdecl lower_bound_guess(unsigned int *first, unsigned int *last, const unsigned int *value)
{
    int len;
    int half;
    unsigned int *mid;

    len = last - first;
    if (len > 0)
    {
        do
        {
            half = len >> 1;
            mid = first + half;
            if (*mid < *value)
            {
                first = mid + 1;
                len += -1 - half;
            }
            else
                len = half;
        } while (len > 0);
    }
    return first;
}

// FUNCTION: TRILOGY 0x004015c6
// Provides backing blocks to refill the Suite allocator's small-block pool.
/* fake match: reuses the free-list index local for the pool end so VC6 keeps the shared value in EDX. */
unsigned int *__cdecl PoolAllocateBlocks_guess(unsigned int elementSize, unsigned int *blockCount)
{
    unsigned int *block;
    unsigned int **freeListSlot;
    unsigned int indexOrEnd;
    unsigned int byteCount;
    unsigned int available;

    byteCount = *blockCount * elementSize;
    for (;;)
    {
        available = pool_end_guess - (unsigned int)pool_current_guess;
        if (available >= byteCount)
        {
            block = pool_current_guess;
            pool_current_guess = (unsigned int *)((unsigned int)pool_current_guess + byteCount);
            return block;
        }
        if (available >= elementSize)
        {
            *blockCount = available / elementSize;
            block = pool_current_guess;
            pool_current_guess = (unsigned int *)((unsigned int)pool_current_guess +
                (available / elementSize) * elementSize);
            return block;
        }
        byteCount = (((pool_reserved_bytes_guess >> 4) + 7) & 0xfffffff8) + byteCount * 2;
        if (available > 0)
        {
            available = (available - 1) >> 3;
            freeListSlot = &pool_free_lists_guess[available];
            *pool_current_guess = (unsigned int)*freeListSlot;
            *freeListSlot = pool_current_guess;
        }
        pool_current_guess = (unsigned int *)operator new(byteCount);
        if (pool_current_guess == (unsigned int *)0)
        {
            available = elementSize;
            block = pool_current_guess;
            for (; available <= 0x80; available = available + 8)
            {
                indexOrEnd = (available - 1) >> 3;
                freeListSlot = &pool_free_lists_guess[indexOrEnd];
                block = *freeListSlot;
                if (block != (unsigned int *)0)
                {
                    break;
                }
            }
            if (block == (unsigned int *)0)
            {
                pool_end_guess = 0;
                pool_current_guess = (unsigned int *)operator new(byteCount);
                pool_reserved_bytes_guess = pool_reserved_bytes_guess + byteCount;
                indexOrEnd = (unsigned int)pool_current_guess + byteCount;
            }
            else
            {
                indexOrEnd = available + (unsigned int)block;
                *freeListSlot = (unsigned int *)*block;
                pool_current_guess = block;
            }
        }
        else
        {
            pool_reserved_bytes_guess = pool_reserved_bytes_guess + byteCount;
            indexOrEnd = (unsigned int)pool_current_guess + byteCount;
        }
        pool_end_guess = indexOrEnd;
        byteCount = *blockCount * elementSize;
    }
}

// FUNCTION: TRILOGY 0x004016C4
// Transfers a range of container entries during Suite storage updates.
// __cdecl. Copies the dwords of [first, last) forward to dest (std::copy shape); returns the end of the
// copy. Nothing is copied when last <= first.
unsigned int *__cdecl copy_dwords_guess(unsigned int *source, unsigned int *sourceEnd, unsigned int *dest)
{
    int n;

    for (n = sourceEnd - source; n > 0; n--)
    {
        *dest = *source;
        dest++;
        source++;
    }
    return dest;
}

// Object type of the scalar deleting destructor at 0x00401700 (layout not known).
struct BoxDeleting_guess
{
    BoxDeleting_guess *ScalarDeletingDtor_guess(unsigned char flags);
};

// FUNCTION: TRILOGY 0x00401700
// Releases an internal client object and its memory when requested.
// __thiscall (callee pops flags). Scalar deleting destructor: runs the destructor at 0x0040171C, frees the
// object with the cdecl operator delete when bit 0 of flags is set, returns this.
BoxDeleting_guess *BoxDeleting_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x0040171C, (void *), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// Cleared by 0x00401C0B: a head dword, a 0x40-byte table, three trailing dwords (size not known).
struct BoxTable_guess
{
    unsigned int head;          // 0x0
    unsigned int unknown4;      // 0x4 (not touched)
    unsigned char table[0x40];  // 0x8
    unsigned int tail0;         // 0x48
    unsigned int tail1;         // 0x4C
    unsigned int tail2;         // 0x50
};

// FUNCTION: TRILOGY 0x0040171c
// Cleans up an internal client object whose game-facing role is unclear.
// Cloned from initializeWithDefaultHandler_guess 0x004DD932 (same original code).
// __fastcall (this in ECX, no stack args). Sets the vtable pointer, then tail-calls decrementReferenceCount_guess.
void __fastcall CXStr_Destructor_guess(void *self)
{
    *(unsigned int *)self = 0x005343F0;
    EQ_METHOD(0, void, 0x00403E27, (void *), self)();
}

// FUNCTION: TRILOGY 0x00401C0B
// __thiscall (ECX = this). Zeroes head, the 0x40-byte table at +8 and the three dwords at +0x48.
void __fastcall box_table_clear_guess(BoxTable_guess *t)
{
    t->head = 0;
    memset(t->table, 0, 0x40);
    t->tail0 = 0;
    t->tail1 = 0;
    t->tail2 = 0;
}

// Owner of the member below; layout not recovered.
class UiElementTree_guess
{
public:
    int * CreateUIElementTree_guess(int *parsedValue, int parent);    /* 0x004024E6 */
};

// FUNCTION: TRILOGY 0x004024E6
// Builds a UI control hierarchy from parsed layout data, including text entries.
// __thiscall (ECX = this), callee pops 8. Makes a node for parsedValue via 0x0040257F, pushes it into the vector
// at parent + 0xc when parent is set, calls vtable slot 0x38 for parsedValue's +0x2c list, then recurses over
// its +0xc list (a child of type 3 gets the string at 0x0053C2C0 instead). Returns the node (0 if none).
// @early-stop: four `xor edx,edx` (EDX dummy of the fastcall-spelled thiscalls) remain; the store to parsedValue is emitted after the push of &parsedValue (VC6 sinks it).
int * UiElementTree_guess::CreateUIElementTree_guess(int *parsedValue, int parent)
{
    int * state = (int *)this;
    int *node;
    int child;
    int entry;

    if (parsedValue == (int *)0x0)
        return (int *)0x0;
    child = parsedValue[3];
    entry = parsedValue[0xb];
    node = EQ_STDCALL(int *, 0x0040257F, (int))(parsedValue[2]);
    if (node != (int *)0x0)
    {
        if (parent != 0)
        {
            parsedValue = node;
            EQ_FASTCALL(void, 0x004026CD, (void *, unsigned int, int **))
                ((char *)parent + 0xc, 0, &parsedValue);
        }
        node[2] = parent;
        node[1] = *state;
        for (; entry != 0; entry = *(int *)(entry + 0x18))
        {
            (*(void (__fastcall **)(int *, unsigned int, int, int))(*node + 0x38))
                (node, 0, *(int *)(entry + 8), *(int *)(*(int *)(entry + 0xc) + 0x28));
        }
        for (; child != 0; child = *(int *)(child + 0x18))
        {
            if (*(int *)(child + 4) == 3)
            {
                (*(void (__fastcall **)(int *, unsigned int, int, int))(*node + 0x38))
                    (node, 0, 0x0053C2C0, *(int *)(child + 0x28));
            }
            else
            {
                CreateUIElementTree_guess((int *)child, (int)node);
            }
        }
    }
    return node;
}

struct BoxDrawVector_guess
{
    unsigned int *first;
    unsigned int *last;
    unsigned int *end;
};

// FUNCTION: TRILOGY 0x004026cd
// Appends one dword to a Suite vector, reallocating through 0x004026F6 when it is full.
void __fastcall AppendDword_guess(BoxDrawVector_guess *vec, unsigned int edx_dummy,
                                  unsigned int *value)
{
    unsigned int *position;

    position = vec->last;
    if (position != vec->end)
    {
        if (position != 0)
            *position = *value;
        vec->last++;
    }
    else
    {
        EQ_FASTCALL(void, 0x004026F6,
                    (BoxDrawVector_guess *, unsigned int, unsigned int *,
                     unsigned int *, unsigned int))(vec, edx_dummy, position, value, 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x004026f6
// Inserts count copies at position, using the value pointer directly for a single insertion.
// @early-stop: the EH stack marker, register assignment, and empty range walk still differ.
/* fake match: the local state struct reserves the original four-dword frame. */
/* fake match: the rethrowing scope and empty cleanup object retain VC6's EH frame. */
void __fastcall ReallocateAndInsertFillVector_guess(BoxDrawVector_guess *vec, unsigned int edx_dummy,
                                  unsigned int *position, unsigned int *value,
                                  unsigned int count)
{
    BoxDrawEHCleanup_guess cleanup;
    struct InsertState_guess
    {
        unsigned int *new_first;
        int new_size;
        unsigned int old_size;
    } insert_state;
    unsigned int old_size;
    unsigned int *new_last;
    unsigned int *size_source;
    unsigned int *old_cursor;
    unsigned int *p;
    unsigned int bytes;

    try
    {
    old_size = (unsigned int)(vec->last - vec->first);
    insert_state.old_size = old_size;
    size_source = &count;
    if (old_size >= count)
        size_source = &insert_state.old_size;
    insert_state.new_size = *size_source + old_size;

    if (insert_state.new_size != 0)
        insert_state.new_first = EQ_CDECL(unsigned int *, 0x00401366, (unsigned int))(insert_state.new_size * 4);
    else
        insert_state.new_first = 0;

    new_last = EQ_CDECL(unsigned int *, 0x00402836,
                        (unsigned int *, unsigned int *, unsigned int *, unsigned int))
                   (vec->first, position, insert_state.new_first, count);
    if (count == 1)
    {
        if (new_last != 0)
            *new_last = *value;
        new_last = new_last + 1;
    }
    else
    {
        new_last = EQ_CDECL(unsigned int *, 0x00402855,
                            (unsigned int *, unsigned int, unsigned int *, unsigned int))
                       (new_last, count, value, count);
    }
    count = (unsigned int)EQ_CDECL(unsigned int *, 0x00402836,
                                   (unsigned int *, unsigned int *, unsigned int *, unsigned int))
                        (position, vec->last, new_last, count);

    for (old_cursor = vec->first; old_cursor != vec->last; old_cursor++)
    {
    }

    p = vec->first;
    if (p != 0)
    {
        bytes = (unsigned int)((vec->end - p) * 4);
        if (bytes > 0x80)
        {
            operator delete(p);
        }
        else
        {
            bytes = (bytes - 1) >> 3;
            *p = (unsigned int)pool_free_lists_guess[bytes];
            pool_free_lists_guess[bytes] = p;
        }
    }

    vec->last = (unsigned int *)count;
    vec->first = insert_state.new_first;
    vec->end = insert_state.new_first + insert_state.new_size;
    }
    catch (...)
    {
        throw;
    }
}

// FUNCTION: TRILOGY 0x00402836
// Copies a dword range to new storage, skipping stores while dest is null (uninitialized_copy shape).
// __cdecl. Copies the dwords of [first, last) to dest, skipping the store (but not the advance) while
// dest is null (uninitialized-copy shape).
unsigned int *__cdecl uninit_copy_dwords_guess(unsigned int *first, unsigned int *last, unsigned int *dest)
{
    unsigned int *current_dest = dest;

    while (first != last)
    {
        if (current_dest)
            *current_dest = *first;
        first = 1 + first;
        current_dest = 1 + current_dest;
    }
    return current_dest;
}

// FUNCTION: TRILOGY 0x00402855
// Initializes repeated entries in newly built UI vector storage.
// __cdecl. Stores *value into count consecutive dwords at dest, skipping the store (but not the advance)
// while dest is null (uninitialized-fill shape). count is unsigned; returns the advanced dest.
unsigned int *__cdecl uninit_fill_dwords_guess(unsigned int *dest, unsigned int count, const unsigned int *value)
{
    unsigned int *current_dest = dest;

    while (count > 0)
    {
        if (0 != current_dest)
            *current_dest = *value;
        ++current_dest;
        --count;
    }
    return current_dest;
}

struct ChainTableBucketStorage_guess
{
    unsigned int *first;
    unsigned int *last;
    unsigned int *end;

    ~ChainTableBucketStorage_guess()
    {
        EQ_METHOD(0, void, 0x00401167, (PoolBuffer_guess *), (PoolBuffer_guess *)this)();
    }
};

struct ChainTableOwner_guess
{
    unsigned int field_0_guess;
    ChainTableBucketStorage_guess buckets;
    unsigned int count;

    ~ChainTableOwner_guess();
};

// FUNCTION: TRILOGY 0x004028F1
// Clears the entries held by a standalone Suite chain-table owner.
// @early-stop: the saved-this spill is one slot too deep and state 1 before clear is still missing.
/* fake match: volatile unwind_this preserves VC6's saved-this slot for the rethrow handler. */
ChainTableOwner_guess::~ChainTableOwner_guess()
{
    ChainTableOwner_guess * volatile unwind_this = this;

    try
    {
        EQ_THISCALL(void, 0x0040345A, (void *))(this);
    }
    catch (...)
    {
        (void)unwind_this;
        throw;
    }
}

struct BoxDrawOwnerState_guess
{
    unsigned int field_0_guess;
    unsigned int field_4_guess;
    unsigned int field_8_guess[4];
    unsigned int field_18_guess[4];
    unsigned int field_28_guess;
    BoxDrawPoolMember_guess field_2c_guess;
    unsigned int field_30_guess;
    unsigned int field_34_guess;
    unsigned int field_38_guess;
    unsigned int field_3c_guess;

    ~BoxDrawOwnerState_guess();
};

// FUNCTION: TRILOGY 0x00402927
// Resets the owner's client state and stored entries during teardown.
// @early-stop: EH state stores and adjusted ESI base still differ.
/* fake match: volatile table preserves the adjusted pointer's stack spill. */
BoxDrawOwnerState_guess::~BoxDrawOwnerState_guess()
{
    unsigned char * volatile table;

    field_8_guess[0] = 0;
    field_8_guess[1] = 0;
    field_8_guess[2] = 0;
    field_8_guess[3] = 0;

    table = (unsigned char *)this + 0x28;
    try
    {
        EQ_THISCALL(void, 0x0040345A, (void *))(table);
    }
    catch (...)
    {
        throw;
    }
}

// EQGfx t3dDestroyFont slot (GetProcAddress'd by Init_EQGfx_Dx8_guess 0x004DDE87).
typedef int (__cdecl *t3dDestroyFont_guess_t)(unsigned int font);
#define t3dDestroyFont_guess    EQ_GLOBAL(t3dDestroyFont_guess_t, 0x006E51DC)

// FUNCTION: TRILOGY 0x004030EA
// Cleans up shared box-drawing data and its font after the final owner lets go.
// __thiscall method that ignores this (callee pops the one argument). Drops one reference on the
// BoxTable_guess at *ref (head is the count); at zero destroys its font (+0x50), clears and frees it.
// *ref is cleared either way once it was non-null.
void __stdcall box_table_release_guess(BoxTable_guess **tableRef)
{
    BoxTable_guess *t;

    if (tableRef != 0 && *tableRef != 0)
    {
        (*tableRef)->head--;
        if ((*tableRef)->head == 0)
        {
            t3dDestroyFont_guess((*tableRef)->tail2);
            t = *tableRef;
            if (t != 0)
            {
                EQ_METHOD(0, void, 0x00401C0B, (BoxTable_guess *), t)();
                operator delete(t);
            }
        }
        *tableRef = 0;
    }
}

// The hash table of BoxDrawCtor403250_guess's hash_map; its bucket initialisation is 0x004032A1.
class BoxDrawHashTable403250_guess
{
public:
    SuiteHasher_guess hash;                 // 0x00
    SuiteKeyEqual_guess equals;             // 0x01
    SuiteKeyExtractor_guess getKey;         // 0x02
    SuiteBucketVector_guess buckets;        // 0x04
    SuiteAllocProxy_guess<unsigned int> elementCount; // 0x10

    BoxDrawHashTable403250_guess(unsigned int bucketCount, const SuiteHasher_guess& hasher,
        const SuiteKeyEqual_guess& keyEqual, const SuiteAllocator_guess& allocator)
        : hash(hasher), equals(keyEqual), getKey(SuiteKeyExtractor_guess()), buckets(allocator), elementCount(allocator, 0)
    {
        InitializeBuckets_guess(bucketCount);
    }
    void InitializeBuckets_guess(unsigned int requestedCount);
};

// A Suite hash_map: its table with 100 initial buckets.
struct BoxDrawCtor403250_guess
{
    BoxDrawHashTable403250_guess table;     // 0x00

    BoxDrawCtor403250_guess();
};

// FUNCTION: TRILOGY 0x00403250
// Starts an empty Suite hash_map with storage for 100 table entries.
BoxDrawCtor403250_guess::BoxDrawCtor403250_guess()
    : table(100, SuiteHasher_guess(), SuiteKeyEqual_guess(), SuiteAllocator_guess())
{
}

struct BoxDrawHashTable_guess
{
    unsigned int unknown0_guess;
    unsigned int **buckets_begin_guess;  // 0x4
    unsigned int **buckets_end_guess;    // 0x8
    unsigned int unknownC_guess;
    unsigned int count_guess;            // 0x10
    unsigned int* find_or_add_guess(unsigned int *entry);    /* 0x004033E1 */
};

struct BoxDrawInsertVector4032dc_guess
{
    unsigned int unknown0_guess;
    unsigned int *current_guess;  // 0x4
    unsigned int *end_guess;      // 0x8

    void InsertFillVector_guess(unsigned int *param_2, unsigned int param_3, unsigned int *param_4);
};

// FUNCTION: TRILOGY 0x004032a1
// Prepares an empty bucket array sized for the requested table capacity.
void BoxDrawHashTable403250_guess::InitializeBuckets_guess(unsigned int requestedCount)
{
    unsigned int bucketCount;
    unsigned int fill;

    bucketCount = EQ_STDCALL(unsigned int, 0x0040119E, (unsigned int))(requestedCount);
    EQ_METHOD(1, void, 0x004011CA, (void *, unsigned int), (char *)this + 4)(bucketCount);
    // Fill value passed by address. A local, not a write to requestedCount: VC6 puts it in requestedCount's dead slot.
    fill = 0;
    EQ_METHOD(3, void, 0x004032DC, (BoxDrawInsertVector4032dc_guess *, unsigned int *, unsigned int, unsigned int *), (BoxDrawInsertVector4032dc_guess *)((char *)this + 4))(
         *(unsigned int **)((char *)this + 8), bucketCount, &fill);
    *(unsigned int *)((char *)this + 0x10) = 0;
    return;
}

// FUNCTION: TRILOGY 0x004032dc
// Vector fill-insert: inserts count copies of *value at position, shifting the tail; grows via 0x004034A3.
// @early-stop: Forward-copy induction, register allocation and 12-byte frame still differ.
void BoxDrawInsertVector4032dc_guess::InsertFillVector_guess(
    unsigned int *insertPosition, unsigned int requestedCount, unsigned int *fillValuePtr)
{
    if (requestedCount != 0)
    {
        if ((unsigned int)(this->end_guess - this->current_guess) >= requestedCount)
        {
            unsigned int trailing = this->current_guess - insertPosition;
            unsigned int value = *fillValuePtr;
            unsigned int *old_end = this->current_guess;
            if (trailing > requestedCount)
            {
                unsigned int *tail = old_end - requestedCount;
                unsigned int *dest = old_end;
                fillValuePtr = tail;
                for (int count = old_end - fillValuePtr; count > 0; --count)
                {
                    *dest++ = *fillValuePtr++;
                }
                this->current_guess += requestedCount;
                unsigned int *source = tail;
                dest = old_end;
                for (count = source - insertPosition; count > 0; --count)
                    *--dest = *--source;
                unsigned int *fill_end = insertPosition + requestedCount;
                for (; insertPosition != fill_end; ++insertPosition)
                {
                    *insertPosition = value;
                }
            }
            else
            {
                unsigned int extra = requestedCount - trailing;
                unsigned int *dest = old_end;
                for (unsigned int remaining = extra; remaining != 0; --remaining)
                {
                    *dest++ = value;
                }
                this->current_guess += extra;
                dest = this->current_guess;
                unsigned int *source = insertPosition;
                for (int count = old_end - source; count > 0; --count)
                {
                    *dest++ = *source++;
                }
                this->current_guess += trailing;
                for (; insertPosition != old_end; ++insertPosition)
                {
                    *insertPosition = value;
                }
            }
        }
        else
        {
            EQ_THISCALL(void, 0x004034A3,
                (BoxDrawInsertVector4032dc_guess *, int, unsigned int *, unsigned int *, unsigned int))(
                    /* fake match: the ignored EDX slot carries this to preserve the original's pointer register. */
                    this, (int)this, insertPosition, fillValuePtr, requestedCount);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004033e1
// Finds a case-insensitive key in the hash bucket or prepends a newly allocated entry.
// @early-stop: entry is pushed before the divisor sub in the original; EDX is zeroed before the 2nd and 3rd calls (the EQ_THISCALL 0 slot); bucket load/store address as [eax+edi]/[ecx+edi], not [edi+eax]/[edi+ecx].
unsigned int* BoxDrawHashTable_guess::find_or_add_guess(unsigned int *entry)
{
    unsigned int *bucket_entry;
    unsigned int hash;
    int bucket_count;
    unsigned int bucket_offset;
    unsigned int *node;

    EQ_METHOD(1, void, 0x004035DE, (BoxDrawHashTable_guess*, int), this)(this->count_guess + 1);
    bucket_count = this->buckets_end_guess - this->buckets_begin_guess;
    hash = EQ_METHOD(1, unsigned int, 0x004037D4, (BoxDrawHashTable_guess*, unsigned int*), this)(entry);
    bucket_offset = (hash % (unsigned int)bucket_count) * 4;
    bucket_entry = *(unsigned int**)((int)this->buckets_begin_guess + bucket_offset);
    for (node = bucket_entry; node != 0; node = (unsigned int*)*node)
    {
        if (_strcmpi(
            (const char*)node[1], (const char*)*entry) == 0)
            return node + 1;
    }
    node = EQ_METHOD(1, unsigned int*, 0x00404631, (BoxDrawHashTable_guess*, unsigned int*), this)(entry);
    *node = (unsigned int)bucket_entry;
    *(unsigned int**)(bucket_offset + (char*)this->buckets_begin_guess) = node;
    this->count_guess++;
    return node + 1;
}

// Hash table of singly linked chains (link in the first dword of each node); bucket vector at 0x4/0x8.
struct ChainTable_guess
{
    unsigned int unknown0;          // 0x0 (not touched)
    unsigned int **first;           // 0x4
    unsigned int **last;            // 0x8
    unsigned int unknownC;          // 0xC (not touched)
    unsigned int count;             // 0x10
};

// FUNCTION: TRILOGY 0x0040345A
// Removes all entries from a chain table and resets its count.
// __thiscall (ECX = table). Pushes every node of every bucket chain on the 8-byte free list
// (pool_push_list8_guess 0x0040139F, this still in ECX), clears each bucket, then zeroes the count.
void __fastcall chain_table_clear_guess(ChainTable_guess *t)
{
    unsigned int i;
    unsigned int *node;
    unsigned int *next;
    int bucketCount;

    // Read the bounds separately, including after callbacks, before advancing the bucket.
    // Volatile accesses preserve the original load order and prevent reuse of the bucket base.
    i = 0;
    bucketCount = (int)((volatile ChainTable_guess *)t)->last;
    bucketCount -= (int)((volatile ChainTable_guess *)t)->first;
    bucketCount >>= 2;
    if (i < (unsigned int)bucketCount)
    {
        do
        {
            node = t->first[i];
            while (node != 0)
            {
                next = (unsigned int *)*node;
                EQ_METHOD(1, void, 0x0040139F, (ChainTable_guess *, unsigned int *), t)(node);
                node = next;
            }
            t->first[i] = 0;
            bucketCount = (int)((volatile ChainTable_guess *)t)->last;
            bucketCount -= (int)((volatile ChainTable_guess *)t)->first;
            i++;
            bucketCount >>= 2;
        } while (i < (unsigned int)bucketCount);
    }
    t->count = 0;
}

struct BoxDrawInsertArray_guess
{
    unsigned int *first_guess;
    unsigned int *last_guess;
    unsigned int *capacity_guess;
};

// FUNCTION: TRILOGY 0x004034a3
// Grows a container array while inserting repeated values at a chosen position.
// @early-stop: the frame, loop layout, and insertion-loop block order still differ.
/* fake match: pointer call + empty guard give the EH frame; volatile args re-read the stack. */
void __fastcall ReallocateAndInsertFillArray_guess(BoxDrawInsertArray_guess *array, void *, unsigned int *volatile source,
                                    unsigned int *value, volatile unsigned int requestedCount)
{
    unsigned int newCount;
    volatile unsigned int *countToAdd;
    unsigned int *newFirst;
    unsigned int *newLast;
    unsigned int *src;
    unsigned int *dest;
    unsigned int *oldFirst;
    unsigned int size;
    unsigned int i;
    int suffixCount;
    unsigned int *(__cdecl *copy_dwords)(unsigned int *, unsigned int *, unsigned int *, unsigned int, int) =
        EQ_FUNC(unsigned int *, __cdecl, 0x004016C4, (unsigned int *, unsigned int *, unsigned int *, unsigned int, int));

    countToAdd = &requestedCount;
    unsigned int oldCount = (unsigned int)(((int)array->last_guess - (int)array->first_guess) >> 2);
    if (oldCount >= requestedCount)
    {
        countToAdd = &oldCount;
    }
    newCount = *countToAdd + oldCount;
    if (newCount != 0)
    {
        newFirst = (unsigned int *)EQ_CDECL(void *, 0x00401366, (unsigned int))(newCount * 4);
    }
    else
    {
        newFirst = 0;
    }

    BoxDrawEHCleanup_guess cleanup;
    oldFirst = array->first_guess;
    newLast = copy_dwords(oldFirst, source, newFirst, requestedCount, 0);
    if (requestedCount == 1)
    {
        if (newLast != 0)
        {
            *newLast = *value;
        }
        newLast++;
    }
    else
    {
        i = requestedCount;
        while (i > 0)
        {
            i--;
            *newLast = *value;
            newLast++;
        }
    }

    src = source;
    dest = newLast;
    for (suffixCount = ((int)array->last_guess - (int)src) >> 2; suffixCount > 0; suffixCount--)
    {
        *dest++ = *src++;
    }

    oldFirst = array->first_guess;
    if (oldFirst != 0)
    {
        size = (unsigned int)(((int)array->capacity_guess - (int)oldFirst) >> 2) * 4;
        if (size > 0x80)
        {
            operator delete(oldFirst);
        }
        else
        {
            size = (size - 1) >> 3;
            *oldFirst = (unsigned int)pool_free_lists_guess[size];
            pool_free_lists_guess[size] = oldFirst;
        }
    }

    array->first_guess = newFirst;
    array->last_guess = dest;
    array->capacity_guess = newFirst + newCount;
}

struct BoxDrawRehashTable_guess
{
    unsigned int field_0_guess;
    unsigned int **begin_guess;
    unsigned int **end_guess;
    unsigned int **capacity_guess;
    void RehashBucketTable_guess(unsigned int requestedCount);    /* 0x004035DE */
};

struct BoxDrawRehashArray_guess
{
    unsigned int **begin_guess;
    unsigned int **end_guess;
    unsigned int **capacity_guess;

    // EXTERNAL: TRILOGY 0x00401167 BoxDrawRehashArray_guess::~BoxDrawRehashArray_guess
    ~BoxDrawRehashArray_guess();
};

// FUNCTION: TRILOGY 0x004035DE
// Expands a bucket table and redistributes its existing entries.
// @early-stop: EH stack-save/state, dummy EDX setup, store scheduling, and epilogue order still differ.
/* fake match: catch/rethrow retains the EH frame; the volatile index alias preserves the repeated loop test. */
void BoxDrawRehashTable_guess::RehashBucketTable_guess(unsigned int requestedCount)
{
    int oldCount;
    unsigned int newCount;
    unsigned int i;
    unsigned int *node;
    unsigned int *next;
    unsigned int hash;
    unsigned int bucket;
    unsigned int **oldBegin;
    unsigned int **oldEnd;
    unsigned int **oldCapacity;
    unsigned int ***beginField;

    oldCount = ((int)this->end_guess - (int)this->begin_guess) >> 2;
    beginField = &this->begin_guess;
    if (requestedCount <= (unsigned int)oldCount)
        return;
    newCount = EQ_STDCALL(unsigned int, 0x0040119E, (unsigned int))(requestedCount);
    if (newCount <= (unsigned int)oldCount)
        return;
    BoxDrawRehashArray_guess newArray;
    void *countAddress;
    unsigned int **clear;
    unsigned int **newEnd;
    unsigned int clearIndex;

    try
    {
        countAddress = EQ_THISCALL(void *, 0x00403729,
            (unsigned int ***, void *, void *))(beginField, 0, ((char *)&requestedCount) + 3);
        EQ_THISCALL(void, 0x00403785,
            (BoxDrawRehashArray_guess *, void *, unsigned int, void *))(&newArray, 0, newCount, countAddress);

        clear = newArray.begin_guess;
        for (clearIndex = 0; clearIndex < newCount; clearIndex++)
            *clear++ = 0;
        newEnd = clear;
        i = 0;
        newArray.end_guess = newEnd;
        volatile unsigned int &loopIndex = i;
        while (loopIndex < (unsigned int)oldCount)
        {
            while ((node = beginField[0][i]) != 0)
            {
                hash = EQ_THISCALL(unsigned int, 0x004037D4,
                    (BoxDrawRehashTable_guess *, void *, unsigned int *))(this, 0, node + 1);
                next = (unsigned int *)*node;
                bucket = hash % newCount;
                beginField[0][i] = next;
                *node = (unsigned int)newArray.begin_guess[bucket];
                newArray.begin_guess[bucket] = node;
            }
            ++i;
        }

        oldBegin = beginField[0];
        beginField[0] = newArray.begin_guess;
        newArray.begin_guess = oldBegin;
        oldEnd = beginField[1];
        beginField[1] = newArray.end_guess;
        newArray.end_guess = oldEnd;
        oldCapacity = beginField[2];
        beginField[2] = newArray.capacity_guess;
        newArray.capacity_guess = oldCapacity;
    }
    catch (...)
    {
        throw;
    }
}

// FUNCTION: TRILOGY 0x00403729
// No game-specific behavior is visible; this helper forwards a supplied value unchanged.
// __thiscall method that ignores this (callee pops the one argument). Returns its argument; the frame
// keeps a zeroed dword local that is never read.
/* fake match: global optimization is off around this function, so VC6 keeps the dead zero store as AND. */
#pragma optimize("g", off)
unsigned int __stdcall identity_guess(unsigned int value)
{
    unsigned int unused = 0;   /* fake match: the never-read frame dword the original zeroes with AND */

    return value;
}
#pragma optimize("g", on)

struct BoxDrawArray_guess
{
    int *begin_guess;
    int *end_guess;
    int *capacity_guess;

    BoxDrawArray_guess *initialize_guess(int count, int unused);
};

// Destructible local with no cleanup: holds the owner pointer in its frame slot.
struct BoxDrawEHMarker_guess
{
    void *owner_guess;

    BoxDrawEHMarker_guess(void *owner) : owner_guess(owner)
    {
    }
    ~BoxDrawEHMarker_guess()
    {
    }
};

// FUNCTION: TRILOGY 0x00403785
// Creates storage sized for a table's requested bucket count.
// @early-stop: the EH state store remains before the capacity clear instead of after the count load.
/* fake match: a volatile count snapshot preserves load order; the destructible marker supplies the EH frame and this slot. */
BoxDrawArray_guess *BoxDrawArray_guess::initialize_guess(volatile int count, int unused)
{
    int *data = 0;
    int savedCount;
    void *(__cdecl *allocate)(unsigned int) = EQ_FUNC(void *, __cdecl, 0x00401366, (unsigned int));

    begin_guess = 0;
    end_guess = 0;
    BoxDrawEHMarker_guess eh_marker(this);
    capacity_guess = 0;
    savedCount = count;
    if (savedCount != 0)
        data = (int *)allocate(savedCount << 2);
    begin_guess = data;
    end_guess = data;
    capacity_guess = data + savedCount;
    return this;
}

// FUNCTION: TRILOGY 0x004037D4
// ORIGINAL FILE: Suite/Suite/BoxDraw.cpp (from Mac debug map)
// __thiscall method that ignores this (callee pops the one argument; one caller leaves ECX unset).
// Case-insensitive string hash of *key: lowercases a heap copy, then h = h * 5 + c over its chars.
int __stdcall hash_string_nocase_guess(char **key)
{
    int len;
    char *lower;
    int i;
    char *p;

    len = strlen(*key);
    lower = (char *)operator new(len + 1);
    for (i = 0; i < len; i++)
        lower[i] = (char)EQ_CDECL(int, 0x005225BC, (int))((*key)[i]);
    lower[len] = '\0';
    p = lower;
    i = 0;
    while (*p != '\0')
    {
        i = i * 5 + *p;
        p++;
    }
    operator delete(lower);
    return i;
}

struct BoxDrawPoolOwnerBase_guess
{
    ~BoxDrawPoolOwnerBase_guess()
    {
    }
};

struct BoxDrawPoolBuffer_guess
{
    unsigned int first_guess;
    unsigned int last_guess;
    unsigned int end_guess;

    ~BoxDrawPoolBuffer_guess()
    {
        EQ_METHOD(0, void, 0x00401167, (void *), this)();
    }
};

struct BoxDrawPoolOwner_guess
{
    unsigned int field_0_guess;
    BoxDrawPoolBuffer_guess pool_guess;
    BoxDrawPoolOwnerBase_guess tail_guess;

    ~BoxDrawPoolOwner_guess();
};

struct StaticHeaderElementScalarDeletingDtor_guess
{
    StaticHeaderElementScalarDeletingDtor_guess *ScalarDeletingDtor_guess(unsigned char flags_guess);
};

// FUNCTION: TRILOGY 0x00403dd5
// Destroys a static header element and optionally frees its memory.
StaticHeaderElementScalarDeletingDtor_guess *StaticHeaderElementScalarDeletingDtor_guess::ScalarDeletingDtor_guess(unsigned char flags_guess)
{
    EQ_METHOD(0, void, 0x00403E27, (void *), this)();
    if (flags_guess & 1)
        operator delete(this);
    return this;
}

// FUNCTION: TRILOGY 0x00403df1
// Clears the entries held by the pool owner during cleanup.
/* fake match: pointer call + empty base force the original's EH frame/state */
BoxDrawPoolOwner_guess::~BoxDrawPoolOwner_guess()
{
    EQ_METHOD(0, void, 0x0040345A, (void *), this)();
}

struct StaticHeaderChild_guess
{
    virtual void destroy_guess(unsigned int flags_guess);
};

struct StaticHeaderNode_guess
{
    StaticHeaderNode_guess *next_guess;
    void *first_guess;
    void *second_guess;
};

struct StaticHeaderBuffer_guess
{
    unsigned int *first_guess;
    unsigned int *last_guess;
    unsigned int *end_guess;

    ~StaticHeaderBuffer_guess()
    {
        /* fake match: the volatile copy gives the original's first-pointer spill before the free call. */
        unsigned int * volatile saved_first_guess = first_guess;
        /* fake match: the volatile copy gives the original's first-pointer spill before the free call. */
        EQ_METHOD(0, void, 0x00401167, (StaticHeaderBuffer_guess *), this)();
    }
};

struct StaticHeaderPool_guess
{
    unsigned int *first_guess;
    unsigned int *last_guess;
    unsigned int *end_guess;

    ~StaticHeaderPool_guess()
    {
        EQ_METHOD(0, void, 0x00401167, (StaticHeaderPool_guess *), this)();
    }
};

struct StaticHeaderChain_guess
{
    unsigned int field_0_guess;
    StaticHeaderPool_guess pool_guess;

    ~StaticHeaderChain_guess()
    {
        /* fake match: a no-op cleanup gives the original's EH state 4 around the clear call. */
        BoxDrawEHCleanup_guess cleanup_guess;
        EQ_METHOD(0, void, 0x0040345A, (StaticHeaderChain_guess *), this)();
    }
};

struct StaticHeaderGauge_guess
{
    unsigned char data_guess[0x6c];

    ~StaticHeaderGauge_guess()
    {
        EQ_METHOD(0, void, 0x004049CA, (StaticHeaderGauge_guess *), this)();
    }
};

struct StaticHeaderIterator_guess
{
    StaticHeaderNode_guess *node_guess;
    unsigned int bucket_guess;

    StaticHeaderIterator_guess()
    {
    }

    StaticHeaderIterator_guess(const StaticHeaderIterator_guess &other_guess)
    {
        node_guess = other_guess.node_guess;
        bucket_guess = other_guess.bucket_guess;
    }

    StaticHeaderIterator_guess operator++(int)
    {
        StaticHeaderIterator_guess old_guess = *this;
        node_guess = node_guess->next_guess;
        if (node_guess == 0)
            node_guess = EQ_METHOD(0, StaticHeaderNode_guess *, 0x0040451B, (StaticHeaderIterator_guess *), this)();
        return old_guess;
    }
};

struct StaticHeaderElement_guess
{
    unsigned int vtable_guess;
    unsigned int field_4_guess;
    unsigned int field_8_guess;
    StaticHeaderBuffer_guess buffer_guess;
    StaticHeaderGauge_guess gauge_guess;
    void *field_84_guess;
    StaticHeaderChain_guess chain_guess;

    ~StaticHeaderElement_guess();
};

// FUNCTION: TRILOGY 0x00403e27
// Destroys stored child objects and clears their associated entries.
// @early-stop: edi/ebx swapped (buffer pointer vs child cursor), vtable store scheduled late, iterator advance stores the node early.
/* fake match: buffer_ptr_guess keeps the buffer address in a register; the copy ctor and no-op cleanup shape the by-value argument slot and EH states. */
StaticHeaderElement_guess::~StaticHeaderElement_guess()
{
    StaticHeaderIterator_guess iterator_guess;
    StaticHeaderBuffer_guess *buffer_ptr_guess = &buffer_guess;

    vtable_guess = 0x005344B8;
    field_4_guess &= 0;
    field_8_guess &= 0;

    unsigned int *item_guess = buffer_ptr_guess->first_guess;
    unsigned int *last_guess = buffer_ptr_guess->last_guess;
    if (item_guess != last_guess)
    {
        do
        {
            if (*item_guess != 0)
                ((StaticHeaderChild_guess *)*item_guess)->destroy_guess(1);
            item_guess += 1;
        } while (item_guess != buffer_guess.last_guess);
    }

    EQ_METHOD(0, void, 0x0040429B, (StaticHeaderBuffer_guess *), buffer_ptr_guess)();
    operator delete(field_84_guess);

    EQ_METHOD(1, void, 0x00404343, (StaticHeaderChain_guess *, StaticHeaderIterator_guess *), &chain_guess)(&iterator_guess);
    while (StaticHeaderNode_guess *node_guess = iterator_guess.node_guess)
    {
        void *first_guess = node_guess->first_guess;
        void *second_guess = node_guess->second_guess;
        EQ_METHOD(1, void, 0x004042F6, (StaticHeaderChain_guess *, StaticHeaderIterator_guess), &chain_guess)(iterator_guess++);
        operator delete(first_guess);
        operator delete(second_guess);
    }
}

// FUNCTION: TRILOGY 0x0040428D
// Releases the allocated storage owned by a container buffer.
// @early-stop: VC6 still uses an EBP local and a second argument push instead of the original outgoing-slot store.
// __thiscall (ECX = this). Calls pool_free_guess (0x00401167) with the first dword of this on the stack;
// ECX still holds this for the callee.
void __fastcall pool_free_first_guess(PoolBuffer_guess *buf)
{
    /* fake match: retain a stack local so VC6 loads the first field through EAX before the call. */
    unsigned int * volatile first;

    first = buf->first;
    EQ_CDECL(void, 0x00401167, (unsigned int *))(first);
}

// FUNCTION: TRILOGY 0x0040429B
// Clears all entries from an ordered container buffer.
// __thiscall (ECX = this). Calls buffer_erase_guess (0x00404314) with this still in ECX and
// (first, last) on the stack.
void __fastcall buffer_erase_all_guess(PoolBuffer_guess *buf)
{
    EQ_METHOD(2, unsigned int *, 0x00404314, (PoolBuffer_guess *, unsigned int *, unsigned int *), buf)(buf->first, buf->last);
}

// FUNCTION: TRILOGY 0x004042F6
// Applies an iterator's erase operation to its owning chain table.
// __thiscall (ECX = this, callee pops 8). Copies its two stack arguments into a local pair and passes the
// pair's address to 0x004045D3, with ECX still holding this.
void __fastcall buffer_pair_call_guess(void *self, int edx_unused, unsigned int a, unsigned int b)
{
    unsigned int pair[2];

    pair[0] = a;
    pair[1] = b;
    EQ_METHOD(1, void, 0x004045D3, (void *, unsigned int *), self)(pair);
}

// FUNCTION: TRILOGY 0x00404314
// Removes a selected range of entries from an ordered container buffer.
// __thiscall (ECX = buffer, callee pops 8). Vector erase shape: moves the dwords of [src, last) down to
// dest, sets last to the end of the copy and returns dest.
unsigned int *__fastcall buffer_erase_guess(PoolBuffer_guess *buf, int edx_unused, unsigned int *dest, unsigned int *src)
{
    unsigned int *p;
    int n;
    int off;

    n = buf->last - src;
    p = dest;
    if (n > 0)
    {
        off = (int)src - (int)p;
        do
        {
            *p = *(unsigned int *)((int)p + off);
            p++;
            n--;
        } while (n != 0);
    }
    buf->last = p;
    return dest;
}

// Vector-like member with a leading dword (allocator slot, not touched) before first / last.
struct DwordVector_guess
{
    unsigned int unknown0;          // 0x0 (not touched)
    unsigned int *first;            // 0x4
    unsigned int *last;             // 0x8
};

// Result of the first-nonzero search: the element found (0 when none) and the vector searched.
struct BufferFind_guess
{
    unsigned int value;             // 0x0
    DwordVector_guess *owner;       // 0x4
};

// FUNCTION: TRILOGY 0x00404343
// Finds the first nonempty bucket head in a table's bucket array.
// @early-stop: Bounds reloads, register allocation, and several load and branch positions still differ.
// __thiscall (ECX = vector, callee pops 4). Scans [first, last) for the first nonzero dword; stores it
// (or 0) in out->value and the vector in out->owner.
void __fastcall buffer_find_nonzero_guess(DwordVector_guess *buf, int edx_unused, BufferFind_guess *out)
{
    unsigned int i;
    unsigned int *p;
    int count;

    i = 0;
    count = buf->last - buf->first;
    if (count != 0)
    {
        p = buf->first;
        do
        {
            if (*p == 0)
            {
                i++;
                p++;
            }
            else
            {
                out->value = buf->first[i];
                out->owner = buf;
                return;
            }
        } while (i < (unsigned int)count);
    }
    out->value = 0;
    out->owner = buf;
}

// FUNCTION: TRILOGY 0x004043DB
// The zeroed out value is a local, not requestedCount: its address-taken local is what makes VC6 keep the EBP frame.
void __fastcall FUN_004043db_guess(void *object, int edx_unused, unsigned int requestedCount)
{
    unsigned int count;
    unsigned int zero;

    count = EQ_STDCALL(unsigned int, 0x0040119E, (unsigned int))(requestedCount);
    EQ_METHOD(1, void, 0x004011CA, (void *, unsigned int), (char *)object + 4)(count);
    zero = 0;
    EQ_METHOD(3, void, 0x00404416, (void *, unsigned int *, unsigned int, unsigned int *), (char *)object + 4)(*(unsigned int **)((char *)object + 8), count, &zero);
    *(unsigned int *)((char *)object + 0x10) = 0;
    return;
}

struct BoxDrawInsertVector_guess
{
    unsigned int field_0_guess;
    unsigned int *last;
    unsigned int *capacity;
};

// FUNCTION: TRILOGY 0x00404416
// Inserts repeated values into a container's ordered data buffer.
// @early-stop: Register allocation, 12-byte versus 8-byte frame, and copy-loop induction addressing still differ.
// __thiscall (ECX = buffer, callee pops 12). Inserts count copies of *value before position, growing the
// buffer through 0x00404656 when its remaining capacity is too small.
void __fastcall box_draw_fill_insert_guess(BoxDrawInsertVector_guess *self, int edx_unused,
                                           unsigned int *position, unsigned int count,
                                           unsigned int *value)
{
    if (count != 0)
    {
        if ((unsigned int)(self->capacity - self->last) >= count)
        {
            unsigned int suffix = self->last - position;
            unsigned int savedValue = *value;
            unsigned int *oldEnd = self->last;
            if (suffix > count)
            {
                unsigned int *tail = oldEnd - count;
                unsigned int *dest = oldEnd;
                value = tail;
                int n;
                for (n = oldEnd - value; n > 0; --n)
                {
                    *dest = *value;
                    ++dest;
                    ++value;
                }
                self->last += count;
                for (n = tail - position; n > 0; --n)
                {
                    *--oldEnd = *--tail;
                }
                unsigned int *fillEnd = position + count;
                for (; position != fillEnd; ++position)
                {
                    *position = savedValue;
                }
            }
            else
            {
                unsigned int extra = count - suffix;
                unsigned int *dest = oldEnd;
                unsigned int left;
                for (left = extra; left != 0; --left)
                {
                    *dest = savedValue;
                    ++dest;
                }
                self->last += extra;
                dest = self->last;
                unsigned int *first = position;
                int n;
                for (n = oldEnd - first; n > 0; --n)
                {
                    *dest = *first;
                    ++dest;
                    ++first;
                }
                self->last += suffix;
                for (; position != oldEnd; ++position)
                {
                    *position = savedValue;
                }
            }
        }
        else
        {
            EQ_METHOD(3, void, 0x00404656, (BoxDrawInsertVector_guess *, unsigned int *, unsigned int *, unsigned int), self)(position, value, count);
        }
    }
}

// Hash table iterator shape: current node (key string pointer at +4) and the table searched.
struct ChainIter_guess
{
    unsigned int *node;             // 0x0
    ChainTable_guess *table;        // 0x4
};

// FUNCTION: TRILOGY 0x0040451B
// Lets a chain-table scan continue at the next occupied bucket.
// ORIGINAL FILE: Suite/Suite/BoxDraw.cpp (from Mac debug map)
// __thiscall (ECX = iterator). Hashes the current node's key (hash_string_nocase_guess 0x004037D4, ECX = table) into a
// bucket, then returns the first nonzero bucket head after it, or 0 when none is left. Mac/TAKP name
// CTAFrameDraw::Draw (low, shared with 0x004037D4); renamed to avoid overloading hash_string_nocase_guess.
/* fake match: v is a volatile alias of t so the loop reloads first[] instead of sharing the count's load (VC6 CSE); t keeps the count a memory operand. */
unsigned int *__fastcall chain_iter_next_bucket_guess(ChainIter_guess *it, int edx_unused)
{
    ChainTable_guess *t;
    volatile ChainTable_guess *v;
    unsigned int n;         // bucket count before the call: the hash is reduced by it
    unsigned int count;     // bucket count after the call (the callee may change the table)
    unsigned int i;
    unsigned int *p;

    n = it->table->last - it->table->first;
    i = EQ_THISCALL(unsigned int, 0x004037D4, (ChainTable_guess *, int, unsigned int *))(it->table, edx_unused, it->node + 1);
    t = it->table;
    i %= n;
    count = t->last - t->first;
    v = t;
    p = 0;
    while (p == 0)
    {
        i++;
        if (i >= count)
            break;
        p = v->first[i];
    }
    return p;
}

// FUNCTION: TRILOGY 0x0040455a
// Finds or creates the table entry for a case-insensitive string key.
// @early-stop: the EQ_THISCALL dummy EDX (xor edx,edx) before the hash and insert calls; VC6 also loads the volatile first into EAX before the push, where the original folds it into the sub.
// __thiscall (ECX = table, callee pops 4). Finds the matching key in its hash bucket,
// or allocates and prepends a node, returning the address of the node's key field.
/* fake match: v is a volatile alias of t so the bucket base is loaded again after the hash call, not reused from the bucket count's read (VC6 CSEs plain loads across the call). */
unsigned int *__fastcall chain_table_find_or_insert_guess(ChainTable_guess *t, int edx_unused, unsigned int *entry)
{
    unsigned int *bucket_entry;
    unsigned int *node;
    volatile ChainTable_guess *v;
    int bucket_count;
    int bucket_offset;

    EQ_THISCALL(void, 0x00404791, (ChainTable_guess *, int, unsigned int))(t, edx_unused, t->count + 1);
    v = t;
    bucket_count = v->last - v->first;
    bucket_offset = (EQ_METHOD(1, unsigned int, 0x004037D4, (ChainTable_guess *, unsigned int *), t)(entry) % bucket_count) * 4;
    bucket_entry = *(unsigned int **)((int)t->first + bucket_offset);
    for (node = bucket_entry; node != 0; node = (unsigned int *)*node)
    {
        if (_strcmpi(
            (const char *)node[1], (const char *)*entry) == 0)
            return node + 1;
    }
    node = EQ_METHOD(1, unsigned int *, 0x00404631, (ChainTable_guess *, unsigned int *), t)(entry);
    *node = (unsigned int)bucket_entry;
    *(unsigned int **)((int)t->first + bucket_offset) = node;
    t->count++;
    return node + 1;
}

// FUNCTION: TRILOGY 0x004045D3
// Removes a selected string-keyed entry from the chain table.
// __thiscall (ECX = table, callee pops 4). Erases the node *pos: hashes its key (hash_string_nocase_guess 0x004037D4,
// ECX = table) into a bucket, unlinks the node from that chain, pushes it on the 8-byte free list
// (pool_push_list8_guess 0x0040139F) and decrements the count. Nothing happens when the node is not found.
void __fastcall chain_table_erase_guess(ChainTable_guess *t, int edx_unused, unsigned int **nodeRef)
{
    unsigned int *node;
    unsigned int n;
    unsigned int i;
    unsigned int *cur;
    unsigned int *next;

    node = *nodeRef;
    if (node != 0)
    {
        n = t->last - t->first;
        i = EQ_THISCALL(unsigned int, 0x004037D4, (ChainTable_guess *, int, unsigned int *))(t, edx_unused, node + 1) % n;
        cur = t->first[i];
        if (cur == node)
        {
            t->first[i] = (unsigned int *)*cur;
            EQ_METHOD(1, void, 0x0040139F, (ChainTable_guess *, unsigned int *), t)(cur);
            t->count--;
            return;
        }
        for (next = (unsigned int *)*cur; next != 0; next = (unsigned int *)*next)
        {
            if (next == node)
            {
                *cur = *next;
                EQ_METHOD(1, void, 0x0040139F, (ChainTable_guess *, unsigned int *), t)(next);
                t->count--;
                return;
            }
            cur = next;
        }
    }
}

// FUNCTION: TRILOGY 0x00404631
// Creates the storage record for a table entry.
// __thiscall (ECX and EDX ignored, callee pops 4). Allocates a three-dword record, clears its
// first dword, then copies the two input dwords when record + 1 is non-null. Returns the record.
unsigned int *__fastcall CreateThreeDwordRecord_guess(void *self, int edx_unused, unsigned int *entry)
{
    unsigned int *record;
    unsigned int *recordFields;

    record = (unsigned int *)EQ_CDECL(void *, 0x00401366, (unsigned int))(0x0c);
    *record = 0;
    recordFields = record + 1;
    if (recordFields != (unsigned int *)0)
    {
        recordFields[0] = *entry;
        recordFields[1] = entry[1];
    }
    return record;
}

struct BoxDrawArray04656_guess
{
    unsigned int *first;
    unsigned int *last;
    unsigned int *limit;
};

// FUNCTION: TRILOGY 0x00404656
// Game-facing use is unclear; grows an entry list while inserting repeated values.
void __fastcall ReallocateAndInsertFillArrayStorage_guess(BoxDrawArray04656_guess *self, int edx_unused,
    unsigned int *position, unsigned int *value, volatile unsigned int count)
{
    int newCount;
    volatile int &newCountSlot = newCount; /* fake match: retain the original's spilled capacity local */
    unsigned int *newFirst;
    unsigned int *newLast;
    unsigned int *oldFirst;
    unsigned int size;
    unsigned int listIndex;
    int i;

    {
        int oldCount;
        volatile int *growthCount;

        growthCount = (volatile int *)&count;
        oldCount = ((int)self->last - (int)self->first) >> 2;
        if ((unsigned int)oldCount >= (unsigned int)*growthCount)
            growthCount = &oldCount;
        newCount = *growthCount + oldCount;
    }
    newCountSlot = newCount;
    if (newCount != 0)
        newFirst = (unsigned int *)EQ_CDECL(void *, 0x00401366, (unsigned int))(newCount * 4);
    else
        newFirst = 0;

    try
    {
        newLast = (unsigned int *)EQ_CDECL_CPP(unsigned int *, 0x004016C4,
            (unsigned int *, unsigned int *, unsigned int *, unsigned int, unsigned int))(
                self->first, position, newFirst, count, 0);
        if (count == 1)
        {
            if (newLast != 0)
                *newLast = *value;
            newLast++;
        }
        else
        {
            for (i = count; i != 0; i--)
            {
                *newLast = *value;
                newLast++;
            }
        }
        for (i = ((int)self->last - (int)position) >> 2; i > 0; i--)
        {
            *newLast = *position;
            position++;
            newLast++;
        }
    }
    catch (...)
    {
        /* The new storage goes back to the small-block pool (or the heap) before the exception moves on. */
        if (newFirst != 0)
        {
            size = newCount * 4;
            if (size > 0x80)
                EQ_CDECL(void, 0x005218BB, (void *))(newFirst);
            else
            {
                listIndex = (size - 1) >> 3;
                *newFirst = (unsigned int)pool_free_lists_guess[listIndex];
                pool_free_lists_guess[listIndex] = newFirst;
            }
        }
        throw;
    }

    oldFirst = self->first;
    if (oldFirst != 0)
    {
        size = (self->limit - oldFirst) * 4;
        if (size > 0x80)
            EQ_CDECL(void, 0x005218BB, (void *))(oldFirst);
        else
        {
            listIndex = (size - 1) >> 3;
            *oldFirst = (unsigned int)pool_free_lists_guess[listIndex];
            pool_free_lists_guess[listIndex] = oldFirst;
        }
    }

    self->first = newFirst;
    self->last = newLast;
    self->limit = newFirst + newCount;
}

struct BoxDrawArrayTemp04791_guess
{
    unsigned int **first_guess;
    unsigned int **last_guess;
    unsigned int **limit_guess;

    ~BoxDrawArrayTemp04791_guess()
    {
        EQ_METHOD(0, void, 0x00401167, (void *), this)();
    }
};

struct RehashArray_guess
{
    unsigned int **first_guess;
    unsigned int **last_guess;
    unsigned int **limit_guess;

    // EXTERNAL: TRILOGY 0x00401167 RehashArray_guess::~RehashArray_guess
    ~RehashArray_guess();
};

// FUNCTION: TRILOGY 0x00404791
// Rebuilds a string-keyed lookup table when its bucket count grows.
// @early-stop: stack slots, fastcall adapters, loop increment, and EH cleanup tail still differ.
/* fake match: a scoped owner and indirect hash call preserve VC6's EH frame. */
void __fastcall RehashChainTable_guess(void *self, int edx_unused, unsigned int requestedCount)
{
    typedef unsigned int (__fastcall *HashFunction_guess)(void *, int, unsigned int *);
    unsigned int ***arrayFields;
    int oldCount;
    unsigned int newCount;
    unsigned int identityValue;
    /* fake match: keep the original per-iteration top test. */
    volatile unsigned int index;
    unsigned int *node;
    unsigned int hash;
    unsigned int *next;
    unsigned int **oldFirst;

    arrayFields = (unsigned int ***)((char *)self + 4);
    oldCount = ((int)arrayFields[1] - (int)arrayFields[0]) >> 2;
    if (requestedCount > oldCount)
    {
        newCount = EQ_STDCALL(unsigned int, 0x0040119E, (unsigned int))(requestedCount);
        if (newCount > oldCount)
        {
            identityValue = EQ_METHOD(1, unsigned int, 0x00403729, (void *, unsigned int *), arrayFields)(
                    (unsigned int *)((char *)&requestedCount + 3));
            RehashArray_guess newArray;

            EQ_METHOD(2, void, 0x004048DC, (void *, unsigned int, unsigned int), &newArray)(newCount, identityValue);

            unsigned int **newEnd;
            newEnd = newArray.first_guess;
            if (newCount > 0)
            {
                unsigned int **clear;
                unsigned int clearCount;
                clear = newArray.first_guess;
                clearCount = newCount;
                for (; clearCount != 0; clearCount--)
                {
                    *clear = 0;
                    clear++;
                }
                newEnd = newArray.first_guess + newCount;
            }
            newArray.last_guess = newEnd;

            HashFunction_guess volatile hashFunction = EQ_FUNC(unsigned int, __fastcall, 0x004037D4,
                (void *, int, unsigned int *));
            for (index = 0; index < oldCount; ++index)
            {
                while ((node = arrayFields[0][index]) != 0)
                {
                    hash = hashFunction(self, 0, node + 1);
                    next = (unsigned int *)*node;
                    hash = hash % newCount;
                    arrayFields[0][index] = (unsigned int *)next;
                    *node = (unsigned int)newArray.first_guess[hash];
                    newArray.first_guess[hash] = (unsigned int *)node;
                }
            }

            oldFirst = arrayFields[0];
            arrayFields[0] = newArray.first_guess;
            newArray.first_guess = oldFirst;
            oldFirst = arrayFields[1];
            arrayFields[1] = newArray.last_guess;
            newArray.last_guess = oldFirst;
            oldFirst = arrayFields[2];
            arrayFields[2] = newArray.limit_guess;
            newArray.limit_guess = oldFirst;
        }
    }
}

// Three-pointer storage used by the Suite array wrapper; offsets 0, 4, and 8.
struct BoxDrawConstructor048dc_guess
{
    int *first_guess;
    int *last_guess;
    int *limit_guess;

    BoxDrawConstructor048dc_guess *AllocateDwordArray_guess(unsigned int count, unsigned int unused);
};

// FUNCTION: TRILOGY 0x004048dc
// Game-facing use is unclear; allocates the integer array used for table entries.
// @early-stop: limit=0 is stored before the marker's this store (original: this first); after the marker VC6 sinks it past the count test.
/* fake match: VC6 gives a frame only to a call it may throw (a call through a pointer variable, not a constant address), and the marker supplies the this slot. */
BoxDrawConstructor048dc_guess *BoxDrawConstructor048dc_guess::AllocateDwordArray_guess(unsigned int count, unsigned int unused)
{
    int *allocated = 0;
    void *(__cdecl *allocate)(unsigned int) = EQ_FUNC(void *, __cdecl, 0x00401366, (unsigned int));

    first_guess = 0;
    last_guess = 0;
    limit_guess = 0;
    BoxDrawEHMarker_guess eh_marker(this);
    if (count != 0)
        allocated = (int *)allocate(count << 2);
    first_guess = allocated;
    last_guess = allocated;
    limit_guess = allocated + count;
    return this;
}

// Gauge control: vtable pointer and one heap block released by the destructor (rest of the object unknown).
struct GaugeControl_guess
{
    unsigned int vtable;    // 0x0
    void *block;            // 0x4

    GaugeControl_guess *ScalarDeletingDtor_guess(u8 flags);
};

// Listbox control: only the vtable pointer is known here.
struct ListboxControl_guess
{
    unsigned int vtable;    // 0x0

    ListboxControl_guess *ScalarDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x00404942
// Disposes of the list box UI control.
// __thiscall (TAKP name ListBoxControl::~ListBoxControl, low). Scalar deleting destructor: runs
// ListboxControl__ListBoxControl_guess (0x0040495E), frees the object when bit 0 of flags is set,
// returns this.
ListboxControl_guess *ListboxControl_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x0040495E, (void *), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// FUNCTION: TRILOGY 0x0040495e
// Releases content owned by the list box control.
// Cloned from initializeWithDefaultHandler_guess 0x004DD932 (same original code).
// __fastcall (this in ECX, no stack args). Sets the vtable pointer, then tail-calls decrementReferenceCount_guess.
void __fastcall ListboxControl__ListBoxControl_guess(void *self)
{
    *(unsigned int *)self = 0x0053450C;
    EQ_METHOD(0, void, 0x00403E27, (void *), self)();
}

// FUNCTION: TRILOGY 0x004049AE
// Ends the gauge component's lifetime.
// __thiscall (TAKP name GaugeControl::~GaugeControl, low). Scalar deleting destructor: runs
// gauge_control_destroy_guess, frees the object when bit 0 of flags is set, returns this.
GaugeControl_guess *GaugeControl_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x004049CA, (void *), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// FUNCTION: TRILOGY 0x004049CA
// Releases the gauge's backing allocation.
// __thiscall (ECX = this). Resets the vtable pointer to 0x00534560 and frees the block at +4 with the
// client's cdecl operator delete (0x005218BB).
void __fastcall gauge_control_destroy_guess(GaugeControl_guess *g)
{
    g->vtable = 0x00534560;
    operator delete(g->block);
}

struct SpellGemControlHeader_guess
{
    // EXTERNAL: TRILOGY 0x00403E27 SpellGemControlHeader_guess::~SpellGemControlHeader_guess
    virtual ~SpellGemControlHeader_guess();

    unsigned int field_4_guess;
    unsigned int field_8_guess;
    unsigned char field_0C_to_97_guess[0x8c];
};

struct SpellGemGaugeMember_guess
{
    unsigned int vtable_guess;
    void *block_guess;

    // EXTERNAL: TRILOGY 0x004049CA SpellGemGaugeMember_guess::~SpellGemGaugeMember_guess
    ~SpellGemGaugeMember_guess();
};

// EXTERNAL: TRILOGY 0x00534564 ??_7SpellGemControl_guess@@6B@
struct SpellGemControl_guess : SpellGemControlHeader_guess
{
    unsigned int field_98_guess;
    BoxTable_guess *box_table_guess;
    unsigned int field_A0_guess;
    unsigned char field_A4_to_E3_guess[0x40];
    unsigned char field_E4_to_EB_guess[8];
    SpellGemGaugeMember_guess gauge_EC_guess;
    unsigned char field_F4_to_FB_guess[8];
    SpellGemGaugeMember_guess gauge_FC_guess;

    ~SpellGemControl_guess();
    SpellGemControl_guess *ScalarDeletingDtor_guess(unsigned char flags);
};

// FUNCTION: TRILOGY 0x00404B61
// The specific game role is unclear; disposes of this control object.
SpellGemControl_guess *SpellGemControl_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x00404B7D, (void *), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// FUNCTION: TRILOGY 0x00404b7d
// Clears the control's member display state during teardown.
SpellGemControl_guess::~SpellGemControl_guess()
{
    BoxTable_guess **box_table = &box_table_guess;
    EQ_METHOD(1, void, 0x004030EA, (void *, void *), (void *)field_4_guess)(box_table);
    memset(
        field_A4_to_E3_guess, 0, 0x40);
    *box_table = 0;
}

struct InventorySlotControlBase_guess
{
    // EXTERNAL: TRILOGY 0x00403E27 InventorySlotControlBase_guess::~InventorySlotControlBase_guess
    virtual ~InventorySlotControlBase_guess();
};

struct InventorySlotGauge_guess
{
    unsigned char storage[0x10];

    // EXTERNAL: TRILOGY 0x004049CA InventorySlotGauge_guess::~InventorySlotGauge_guess
    ~InventorySlotGauge_guess();
};

// EXTERNAL: TRILOGY 0x005345C0 ??_7InventorySlotControl_guess@@6B@
struct InventorySlotControl_guess : InventorySlotControlBase_guess
{
    unsigned int field_4_guess;
    unsigned char field_8_to_9B_guess[0x94];
    InventorySlotGauge_guess gauge_control_guess;
    unsigned char field_AC_guess;
    unsigned char field_AD_guess;
    unsigned char field_AE_guess;
    unsigned char field_AF_guess;
    unsigned int box_table_guess;

    InventorySlotControl_guess *ScalarDeletingDtor_guess(unsigned char flags);
    ~InventorySlotControl_guess();
};

// FUNCTION: TRILOGY 0x00405108
// The in-game role is unclear; disposes of a control with slot and gauge state.
InventorySlotControl_guess *InventorySlotControl_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x00405124, (void *), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// FUNCTION: TRILOGY 0x00405124
// Releases the control's table reference and gauge resource.
InventorySlotControl_guess::~InventorySlotControl_guess()
{
    unsigned int *box_table = &box_table_guess;
    EQ_METHOD(1, void, 0x004030EA, (void *, void *), (void *)field_4_guess)(box_table);
    {
        unsigned char *state = &field_AC_guess;
        state[3] = 0;
        state[2] = 0;
        state[1] = 0;
        state[0] = 0;
    }
    *box_table = 0;
}
