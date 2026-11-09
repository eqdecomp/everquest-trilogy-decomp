/* heap.c - a binary min-heap (priority queue) of elements that track their own index; the rdp layer keeps
 * its connections in one, ordered by their next timeout.
 * ORIGINAL FILE: unknown. The Mac map names nothing between msg_arrival.c (0x004F4CC0) and the connection
 * hash at 0x004F5020..; these functions form one object there (the queue's graph label is rdp).
 *
 * C. The rdp code was built for speed (see bitops.c); the sift-up loop is inlined into its three users.
 */
#include <stdlib.h>

#include "trilogy/rdp.h"

// CFLAGS: /O2



// Moves items[i] up while it compares lower than its parent.
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

// FUNCTION: TRILOGY 0x004f4cd0
// Set up the priority queue for pending RDP work.
// ORIGINAL FILE: unknown (heap object between msg_arrival.c and the connection hash)
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

// FUNCTION: TRILOGY 0x004f4d10
// Adds an element to the RDP priority queue.
// ORIGINAL FILE: unknown (heap object between msg_arrival.c and the connection hash)
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

// FUNCTION: TRILOGY 0x004f4dc0
// Remove an element from the heap (the last element fills its slot and is re-sifted) and return its data.
// ORIGINAL FILE: unknown (heap object between msg_arrival.c and the connection hash)
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

// FUNCTION: TRILOGY 0x004f4e50
// Maintain priority order so the most urgent RDP work is first.
// ORIGINAL FILE: unknown (heap object between msg_arrival.c and the connection hash)
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

// FUNCTION: TRILOGY 0x004f4f30
// ORIGINAL FILE: unknown (heap object between msg_arrival.c and the connection hash)
// __cdecl. Restores the order after `elem`'s key changed: sift up, then sift down from its old index.
void updateHeapElement_guess(RdpHeap_guess *heap, RdpHeapElem_guess *elem)
{
    u32 i;

    i = elem->index;
    heap_sift_up_inl(heap, i);
    heapifyDown_guess(heap, i);
}

// FUNCTION: TRILOGY 0x004f4fa0
// ORIGINAL FILE: unknown (heap object between msg_arrival.c and the connection hash)
// __cdecl. Removes the top element and returns its data; NULL when empty.
void *Heap_extractMinFromHeap_guess(RdpHeap_guess *heap)
{
    RdpHeapElem_guess **items;
    void *data = 0;

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

// FUNCTION: TRILOGY 0x004f4fe0
// ORIGINAL FILE: unknown (heap object between msg_arrival.c and the connection hash)
// __cdecl. The top element's data, NULL when empty.
void *Heap_getTopHeapElement_guess(RdpHeap_guess *heap)
{
    void *data;

    data = 0;
    if (heap->count > 0)
        data = heap->items[0]->data;
    return data;
}

// kernel32 InitializeCriticalSection import slot without <windows.h>.
typedef void (__stdcall *InitializeCriticalSectionFn_guess)(void *section);

// FUNCTION: TRILOGY 0x004F72E0
// __cdecl. obj is a heap at 0x00 followed by a CRITICAL_SECTION at 0x14.
int initializeCriticalSectionAndBuffer_guess(u8 *obj)
{
    EQ_GLOBAL(InitializeCriticalSectionFn_guess, 0x005340E0)(obj + 0x14);
    return EQ_CDECL(int, 0x004F4CD0, (RdpHeap_guess *, u32, RdpListCompare_guess))(
        (RdpHeap_guess *)obj, 1, (RdpListCompare_guess)0x004F7310);
}

/* 0x004F66D0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(u32,0x004F66D0, (void))

// FUNCTION: TRILOGY 0x004f7350
// Report how long remains until the next queued deadline.
// ORIGINAL FILE: unknown (locked heap object after the rdp thread helpers)
// __cdecl. How long until the top element's deadline, as a timeval-like {seconds, microseconds} in `tv`
// (zeroes when already past; signed difference). NULL when the heap is empty or the top's +0x30 flag is
// set. The top's data has no header struct yet: +0x30 a flag, +0x34 a timeGetTime deadline.
u32 *get_next_connection_timeout_guess(RdpHeap_guess *heap, u32 *tv)
{
    u8 *top;
    u32 remaining;

    top = (u8 *)Heap_getTopHeapElement_guess(heap);
    if (top != 0 && *(u32 *)(top + 0x30) == 0)
    {
        remaining = *(u32 *)(top + 0x34) - rdp_timeGetTime_guess();
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
