// eqmain: the same original object as trilogy's src/Network/rdp/heap.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
// CFLAGS: /O2
/* heap.c - a binary min-heap (priority queue) of elements that track their own index; the rdp layer keeps
 * its connections in one, ordered by their next timeout.
 * ORIGINAL FILE: unknown. The Mac map names nothing between msg_arrival.c (0x004F4CC0) and the connection
 * hash at 0x004F5020..; these functions form one object there (the queue's graph label is rdp).
 *
 * C. The rdp code was built for speed (see bitops.c); the sift-up loop is inlined into its three users.
 */
#include <stdlib.h>

#include "eqmain/rdp.h"

typedef struct RdpKey_guess
{
    int first;
    int second;
} RdpKey_guess;

// kernel32 InitializeCriticalSection import slot without <windows.h>.
typedef void (__stdcall *InitializeCriticalSectionFn_guess)(void *section);

/* 0x1002F9F0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(u32,0x1002F9F0, (void))

// FUNCTION: EQMAIN 0x1002e200
// Creates the empty priority queue used to schedule RDP network work.
// Cloned from trilogy initializeArrayBuffer_guess 0x004F4CD0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Empty heap with room for `size` elements (and growing by `size`). Returns 1 when malloc fails
// (items is then NULL), else 0.
int initializeArrayBuffer_guess(RdpHeap_guess *heap, u32 size, RdpListCompare_guess compare)
{
    int rc = 0;

    heap->grow = size;
    heap->count = 0;
    heap->capacity = size;
    heap->compare = compare;
    heap->items = (RdpHeapElem_guess **)malloc(size * sizeof(RdpHeapElem_guess *));
    if (heap->items == 0)
        rc = 1;
    return rc;
}

// FUNCTION: EQMAIN 0x1002e260
// Queues a new RDP work item so it can be selected by priority.
// Cloned from trilogy heapInsertElement_guess 0x004F4D10 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Adds `elem` (growing items[] by `grow` when full; returns 1 if realloc fails) and sifts it up.
// Returns 0.
int heapInsertElement_guess(RdpHeap_guess *heap, RdpHeapElem_guess *elem)
{
    RdpHeapElem_guess **items;
    RdpHeapElem_guess *t;
    u32 i;
    u32 parent;
    int rc = 0;

    for (;;)
    {
        if (heap->count == heap->capacity)
        {
            items = (RdpHeapElem_guess **)realloc(heap->items, (heap->grow + heap->capacity) * sizeof(RdpHeapElem_guess *));
            if (items == 0)
            {
                rc = 1;
                break;
            }
            heap->items = items;
            heap->capacity += heap->grow;
        }
        heap->items[heap->count] = elem;
        elem->index = heap->count;
        i = heap->count;
        while (i > 0)
        {
            parent = (i - 1) >> 1;
            if (heap->compare(heap->items[i]->key, heap->items[parent]->key) >= 0)
                break;
            t = heap->items[i];
            heap->items[i] = heap->items[parent];
            heap->items[parent] = t;
            heap->items[i]->index = i;
            heap->items[parent]->index = parent;
            i = parent;
        }
        heap->count++;
        break;
    }
    return rc;
}

static __inline void heap_sift_up_inl(RdpHeap_guess *heap, u32 i)
{
    u32 parent;
    RdpHeapElem_guess *t;

    while (i > 0)
    {
        parent = (i - 1) >> 1;
        if (heap->compare(heap->items[i]->key, heap->items[parent]->key) >= 0)
            break;
        t = heap->items[i];
        heap->items[i] = heap->items[parent];
        heap->items[parent] = t;
        heap->items[i]->index = i;
        heap->items[parent]->index = parent;
        i = parent;
    }
}

// FUNCTION: EQMAIN 0x1002e310
// Takes an RDP work item out of the queue when it is released.
// Cloned from trilogy removeHeapElement_guess 0x004F4DC0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Removes `elem` (the last element takes its place and is sifted down, then up) and returns its
// data.
void *removeHeapElement_guess(RdpHeap_guess *heap, RdpHeapElem_guess *elem)
{
    u32 i;
    u32 last;
    void *data;

    i = elem->index;
    data = heap->items[i]->data;
    last = --heap->count;
    if (last != i)
    {
        heap->items[i] = heap->items[last];
        heap->items[i]->index = i;
        heapifyDown_guess(heap, i);
        heap_sift_up_inl(heap, i);
    }
    return data;
}

// FUNCTION: EQMAIN 0x1002e3a0
// Keeps queued RDP work in priority order after an entry moves down.
// Cloned from trilogy heapifyDown_guess 0x004F4E50 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Moves items[i] down while a child compares lower: swaps with the lower child (the right one
// on a tie).
void heapifyDown_guess(RdpHeap_guess *heap, u32 i)
{
    u32 left;
    u32 right;
    u32 child;
    int cl;
    int cr;
    RdpHeapElem_guess *t;

    left = i * 2 + 1;
    right = left + 1;
    while (left < heap->count)
    {
        cl = heap->compare(heap->items[i]->key, heap->items[left]->key);
        if (right >= heap->count)
        {
            if (cl > 0)
            {
                t = heap->items[i];
                heap->items[i] = heap->items[left];
                heap->items[left] = t;
                heap->items[i]->index = i;
                heap->items[left]->index = left;
            }
            return;
        }
        cr = heap->compare(heap->items[i]->key, heap->items[right]->key);
        if (cl <= 0 && cr <= 0)
            return;
        child = heap->compare(heap->items[left]->key, heap->items[right]->key) < 0 ? left : right;
        t = heap->items[i];
        heap->items[i] = heap->items[child];
        heap->items[child] = t;
        heap->items[i]->index = i;
        heap->items[child]->index = child;
        i = child;
        left = i * 2 + 1;
        right = left + 1;
    }
}

// FUNCTION: EQMAIN 0x1002e480
// Repositions an RDP queue entry when its scheduling key changes.
// Cloned from trilogy updateHeapElement_guess 0x004F4F30 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Restores the order after elem's key changed: sifts the entry at elem->index up while it compares lower
// than its parent (the sift-up loop is inlined here), then sifts down from that old index.
void updateHeapElement_guess(RdpHeap_guess *heap, RdpHeapElem_guess *elem)
{
    u32 start;
    u32 i;
    u32 parent;
    int c;
    RdpHeapElem_guess *t;

    start = elem->index;
    i = start;
    while (i > 0)
    {
        parent = (i - 1) >> 1;
        c = heap->compare(heap->items[i]->key, heap->items[parent]->key);
        if (c >= 0)
            break;
        t = heap->items[i];
        heap->items[i] = heap->items[parent];
        heap->items[parent] = t;
        heap->items[i]->index = i;
        heap->items[parent]->index = parent;
        i = parent;
    }
    EQ_CDECL(void, 0x1002E3A0, (RdpHeap_guess *, u32))(heap, start);
}

// FUNCTION: EQMAIN 0x1002e4f0
// Removes the next scheduled RDP work item from the queue for processing.
// Cloned from trilogy Heap_extractMinFromHeap_guess 0x004F4FA0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Removes the top element and returns its data; NULL when empty.
void *Heap_extractMinFromHeap_guess(RdpHeap_guess *heap)
{
    RdpHeapElem_guess **items;
    void *data;

    data = 0;
    if (heap->count > 0)
    {
        void *topData;
        items = heap->items;
        topData = items[0]->data;
        if (--heap->count != 0)
        {
            items[0] = items[heap->count];
            heap->items[0]->index = 0;
            heapifyDown_guess(heap, 0);
        }
        data = topData;
    }
    return data;
}

// FUNCTION: EQMAIN 0x1002e530
// Looks up the next RDP work item without taking it out of the queue.
// Cloned from trilogy Heap_getTopHeapElement_guess 0x004F4FE0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. The top element's data, NULL when empty.
void *Heap_getTopHeapElement_guess(RdpHeap_guess *heap)
{
    void *data;

    data = 0;
    if (heap->count > 0)
        data = heap->items[0]->data;
    return data;
}

// FUNCTION: EQMAIN 0x10030580
// Sets up the synchronized priority queue used for RDP connection scheduling.
// Cloned from trilogy initializeCriticalSectionAndBuffer_guess 0x004F72E0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. obj is a heap at 0x00 followed by a CRITICAL_SECTION at 0x14.
int initializeCriticalSectionAndBuffer_guess(u8 *obj)
{
    EQ_GLOBAL(InitializeCriticalSectionFn_guess, 0x100400A8)(obj + 0x14);
    return EQ_CDECL(int, 0x1002E200, (RdpHeap_guess *, u32, RdpListCompare_guess))(
        (RdpHeap_guess *)obj, 1, (RdpListCompare_guess)0x100305B0);
}

// FUNCTION: EQMAIN 0x100305B0
// Orders RDP queue entries by their two-part scheduling key.
// __cdecl. Heap order callback: compares {first, second} pairs; the second only decides when both firsts are 0.
// Returns the signed difference of the firsts otherwise, else 1, -1 or 0.
int CompareRdpKeys_guess(RdpKey_guess *left, RdpKey_guess *right)
{
    int diff;

    diff = left->first - right->first;
    if (diff == 0 && left->first == 0)
    {
        diff = left->second - right->second;
        if (0 < diff)
            return 1;
        if (diff < 0)
            diff = -1;
    }
    return diff;
}

// FUNCTION: EQMAIN 0x100305F0
// Calculates how long the network loop can wait for the next scheduled connection.
// Cloned from trilogy get_next_connection_timeout_guess 0x004F7350 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. How long until the top element's deadline, as a timeval-like {seconds, microseconds} in `tv`
// (zeroes when already past; signed difference). NULL when the heap is empty or the top's +0x28 flag is
// set. The top's data has no header struct yet: +0x28 a flag, +0x2c a timeGetTime deadline. (The trilogy
// listing reads +0x30/+0x34; this image's code reads +0x28/+0x2c.)
u32 *get_next_connection_timeout_guess(RdpHeap_guess *heap, u32 *tv)
{
    u8 *top;
    u32 remaining;

    top = (u8 *)EQ_CDECL(void *, 0x1002E530, (RdpHeap_guess *))(heap);
    if (top != 0 && *(u32 *)(top + 0x28) == 0)
    {
        remaining = *(u32 *)(top + 0x2c) - rdp_timeGetTime_guess();
        if ((int)remaining < 0)
        {
            tv[0] = 0;
            tv[1] = 0;
            return tv;
        }
        tv[0] = remaining / 1000;
        tv[1] = remaining % 1000 * 1000;
        return tv;
    }
    return 0;
}
