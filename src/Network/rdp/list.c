/* list.c - doubly linked lists of the RDP layer: links carry a data pointer and a key, a list can keep its
 * links sorted by a compare callback.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/list.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c); the add/remove helpers are inlined into the sorted
 * insert and the link remove, so they are __inline here.
 */
#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

#define initializeNetworkBufferHeader_guess EQ_CDECL(void, 0x004F7FD0, (RdpRing_guess *ring))
#define list_add_tail_by_addr_guess EQ_CDECL(void, 0x004F54C0, (RdpList_guess *list, RdpLink_guess *link))
#define list_remove_head_by_addr_guess EQ_CDECL(void *, 0x004F5500, (RdpList_guess *list))
#define setLinkedListProperties_by_addr_guess EQ_CDECL(void, 0x004F5460, (RdpList_guess *list, int sorted, RdpListCompare_guess compare))
#define allocate_memory_buffer_by_addr_guess EQ_CDECL(int, 0x004F7FF0, (RdpRing_guess *ring, u32 size))
#define FreeRingBuffer_by_addr_guess EQ_CDECL(void, 0x004F8030, (RdpRing_guess *ring))
#define fast_free_by_addr_guess EQ_CDECL(void, 0x004F4AC0, (void *p))
#define no_op_empty_return_by_addr_guess EQ_CDECL(void, 0x0051CF90, (RdpList_guess *list))
#define timeGetTime_by_addr_guess EQ_CDECL(unsigned int, 0x004F66D0, (void))
#define bandwidth_get_time_empty_by_addr_guess EQ_CDECL(int, 0x004F7C50, (void *ring))
#define getTimeFromEQClient_by_addr_guess EQ_CDECL(int, 0x004F17C0, (void *client))
#define NetworkConnection_SendPacket_by_addr_guess EQ_CDECL(void, 0x004F4280, (int connection, void *packet, unsigned int size, unsigned short flags))
#define TraceStart_by_addr_guess EQ_CDECL(void, 0x004F44E0, (int connection))
#define NetworkConnection_Disconnect_by_addr_guess EQ_CDECL(void, 0x004F3AC0, (int connection, int reason))
#define packet_stats_pointer_guess EQ_GLOBAL(unsigned char *, 0x0057F698)

// CFLAGS: /O2

// FUNCTION: TRILOGY 0x004f3e40
// Sends the next queued packet or resends the oldest unacked one, disconnecting when it stays unacked too long.
// @early-stop: Send flags still need explicit upper-half preservation; counter addressing and scheduling differ.
void NetworkConnection_ProcessNetworkPackets_guess(int ptr)
{
#define PACKET_MIN(a, b) ((a) < (b) ? (a) : (b))
#define PACKET_MAX(a, b) ((a) > (b) ? (a) : (b))
    int *sentList;
    unsigned char *stats;
    int temp;
    unsigned int DVar4;
    int pendingPkt;
    unsigned int elapsed;
    unsigned int DVar7;
    int sentPkt;
    unsigned int sinceFirstSend;
    unsigned int removedData;
    RdpList_guess *pendingList;
    unsigned int packetSize;
    union
    {
        unsigned int value;
        unsigned short flags;
    } sendFlags;

    temp = ptr;
    sentList = (int *)(ptr + 0x5c8);
    if (*(int *)(ptr + 0x5c8) != 0)
        ptr = *(int *)(*(int *)(ptr + 0x5c8) + 8);
    else
        ptr = 0;
    DVar4 = timeGetTime_by_addr_guess();
    pendingPkt = bandwidth_get_time_empty_by_addr_guess((void *)(temp + 0x610));
    if (*(short *)(temp + 0x4a8) == 0x45)
        pendingPkt = getTimeFromEQClient_by_addr_guess((void *)*(int *)(temp + 0xc));
    ++g_rdp_stats_guess->c[267];
    if ((int)((DVar4 - pendingPkt) + 10) >= 0)
    {
        /* A break selects the pending packet; every fallback path returns. */
        for (;;)
        {
            if (*(int *)(temp + 0x768) != 0)
            {
                elapsed = 0;
                pendingList = (RdpList_guess *)(temp + 0x5e0);
                if (pendingList->head != 0)
                    pendingPkt = (int)pendingList->head->data;
                else
                    pendingPkt = 0;
                if ((*sentList != 0) && (sentPkt = *(int *)(*sentList + 8), sentPkt != 0))
                {
                    elapsed = timeGetTime_by_addr_guess() - *(int *)(sentPkt + 0x14);
                }
                if (pendingPkt != 0)
                {
                    if (*(short *)(temp + 0x4a8) == 0x45)
                    {
                        if (elapsed <= 0x2710)
                            break;
                    }
                    else
                    {
                        sinceFirstSend = PACKET_MIN(PACKET_MAX(
                            (unsigned int)*(unsigned short *)(temp + 0x746) +
                            (unsigned int)*(unsigned short *)(temp + 0x748) * 3, 50U), 65535U);
                        if (elapsed <= (sinceFirstSend & 0xffff))
                            break;
                    }
                }
            }
            stats = packet_stats_pointer_guess;
            if (ptr != 0)
            {
                elapsed = PACKET_MIN(PACKET_MAX(
                    (unsigned int)*(unsigned short *)(temp + 0x746) +
                    (unsigned int)*(unsigned short *)(temp + 0x748) * 2, 50U), 65535U);
                pendingPkt = *(int *)(ptr + 0x14);
                pendingPkt += (unsigned short)elapsed;
                pendingPkt -= DVar4;
                pendingPkt -= 10;
                if (pendingPkt <= 0)
                {
                    elapsed = DVar4 - *(int *)(temp + 0x294);
                    sinceFirstSend = DVar4 - *(int *)(ptr + 0x10);
                    if (((sinceFirstSend >= (*(unsigned int *)(temp + 0x78c) >> 1)) ||
                        ((sinceFirstSend >= (*(unsigned int *)(temp + 0x790) >> 1) && (elapsed >= *(unsigned int *)(temp + 0x790))))) &&
                       ((*(unsigned char *)(temp + 100) & 2) != 0))
                        TraceStart_by_addr_guess(temp);
                    if ((sinceFirstSend < *(unsigned int *)(temp + 0x78c)) &&
                       ((sinceFirstSend < *(unsigned int *)(temp + 0x790) || (elapsed < *(unsigned int *)(temp + 0x790)))))
                    {
                        removedData = (unsigned int)list_remove_head_by_addr_guess((RdpList_guess *)sentList);
                        sentPkt = (int)removedData;
                        if (sentPkt != 0)
                        {
                            pendingPkt = *(int *)(sentPkt + 0x28);
                            *(int *)((char *)sentList + 0x14) -= pendingPkt;
                        }
                        *(short *)(sentPkt + 0x24) = *(short *)(sentPkt + 0x24) + 1;
                        *(unsigned int *)(sentPkt + 0x14) = DVar4;
                        if ((short)((*(short *)(sentPkt + 0x1a) - *(short *)(temp + 0x4b8)) - 0x78) < 0)
                        {
                            /* MOV DX preserves the upper half of the removed packet's size. */
                            packetSize = *(unsigned int *)(sentPkt + 0x28);
                            EQ_CDECL(void, 0x004F4280, (int, void *, unsigned int, unsigned int))
                                (temp, (void *)(sentPkt + 0x30), packetSize,
                                 *(unsigned short *)(sentPkt + 0x18) |
                                 (packetSize & 0xffff0000U));
                            stats = packet_stats_pointer_guess;
                            ++*(unsigned __int64 *)(stats + 0x20);
                            stats = packet_stats_pointer_guess;
                            *(unsigned __int64 *)(stats + 0x28) += *(unsigned int *)(sentPkt + 0x28);
                            *(int *)(temp + 0x78) = *(int *)(temp + 0x78) + 1;
                            *(int *)(temp + 0x7c) = *(int *)(temp + 0x7c) + *(int *)(sentPkt + 0x28);
                        }
                        *(int *)((char *)sentList + 0x14) += *(int *)(sentPkt + 0x28);
                        list_add_tail_by_addr_guess((RdpList_guess *)sentList, (RdpLink_guess *)sentPkt);
                        return;
                    }
                    if (*(int *)(temp + 0x77c) == 0)
                        return;
                    if (sinceFirstSend >= *(unsigned int *)(temp + 0x78c))
                    {
                        NetworkConnection_Disconnect_by_addr_guess(temp, 0x40000);
                        return;
                    }
                    NetworkConnection_Disconnect_by_addr_guess(temp, 0x50000);
                    return;
                }
            }
            if (*(int *)(temp + 0x794) != 0)
            {
                NetworkConnection_SendPacket_by_addr_guess(temp, 0, 0, 0);
                return;
            }
            stats = packet_stats_pointer_guess;
            ++*(unsigned __int64 *)(stats + 0x860);
            return;
        }
        pendingPkt = (int)list_remove_head_by_addr_guess(pendingList);
        if (pendingPkt != 0)
        {
            sentPkt = *(int *)(pendingPkt + 0x28);
            *(int *)((char *)pendingList + 0x14) = *(int *)((char *)pendingList + 0x14) - sentPkt;
        }
        DVar7 = timeGetTime_by_addr_guess();
        *(short *)(pendingPkt + 0x24) = *(short *)(pendingPkt + 0x24) + 1;
        *(unsigned int *)(pendingPkt + 0x10) = *(unsigned int *)(pendingPkt + 0x14) = DVar7;
        packetSize = *(unsigned int *)(pendingPkt + 0x28);
        /* MOV AX preserves the upper half of the timeGetTime result. */
        sendFlags.value = DVar7;
        sendFlags.flags = *(unsigned short *)(pendingPkt + 0x18);
        EQ_CDECL(void, 0x004F4280, (int, void *, unsigned int, unsigned int))
            (temp, (void *)(pendingPkt + 0x30), packetSize,
             sendFlags.value);
        stats = packet_stats_pointer_guess;
        if ((*(unsigned short *)(pendingPkt + 0x18) & 0x200) != 0)
        {
            ++*(unsigned __int64 *)(stats + 0x10);
            g_rdp_stats_guess->c[3] += *(unsigned int *)(pendingPkt + 0x28);
            *(int *)(temp + 0x70) = *(int *)(temp + 0x70) + 1;
            *(int *)(temp + 0x74) = *(int *)(temp + 0x74) + *(int *)(pendingPkt + 0x28);
            *(int *)((char *)sentList + 0x14) += *(int *)(pendingPkt + 0x28);
            list_add_tail_by_addr_guess((RdpList_guess *)sentList, (RdpLink_guess *)pendingPkt);
            return;
        }
        ++*(unsigned __int64 *)packet_stats_pointer_guess;
        g_rdp_stats_guess->c[1] += *(unsigned int *)(pendingPkt + 0x28);
        *(int *)(temp + 0x68) = *(int *)(temp + 0x68) + 1;
        *(int *)(temp + 0x6c) = *(int *)(temp + 0x6c) + *(int *)(pendingPkt + 0x28);
        fast_free_by_addr_guess((void *)pendingPkt);
        return;
    }
    else
    {
        stats = packet_stats_pointer_guess;
        ++*(unsigned __int64 *)(stats + 0x868);
    }
#undef PACKET_MIN
#undef PACKET_MAX
}

// FUNCTION: TRILOGY 0x004f5440
// ORIGINAL FILE: Network/rdp/list.c (Mac: list_init, low; TAKP: initializeLinkedList)
// __cdecl. Empty list, no compare callback, unsorted.
void list_init_guess(RdpList_guess *list)
{
    list->head = 0;
    list->tail = 0;
    list->count = 0;
    list->sorted = 0;
    list->compare = 0;
}

// FUNCTION: TRILOGY 0x004f5460
// Set key comparison and ordering rules for RDP list lookups.
// ORIGINAL FILE: Network/rdp/list.c (address: between Mac-mapped neighbours)
// __cdecl. Sets the sorted flag (find stops early once the compare says it is past the key) and the compare
// callback.
void setLinkedListProperties_guess(RdpList_guess *list, int sorted, RdpListCompare_guess compare)
{
    list->sorted = sorted;
    list->compare = compare;
}

// The add/remove helpers: inlined into the sorted insert and the link remove, and compiled out of line
// once each (the annotated wrappers below).
static __inline void list_add_head_inl(RdpList_guess *list, RdpLink_guess *link)
{
    link->next = list->head;
    link->prev = 0;
    list->head = link;
    if (list->tail == 0)
        list->tail = link;
    else
        link->next->prev = link;
    list->count++;
}

static __inline void list_add_tail_inl(RdpList_guess *list, RdpLink_guess *link)
{
    link->next = 0;
    link->prev = list->tail;
    list->tail = link;
    if (list->head == 0)
        list->head = link;
    else
        link->prev->next = link;
    list->count++;
}

static __inline void *list_remove_head_inl(RdpList_guess *list)
{
    RdpLink_guess *link;
    void *data;

    data = 0;
    link = list->head;
    if (link)
    {
        data = link->data;
        list->head = link->next;
        if (list->head == 0)
            list->tail = 0;
        else
            list->head->prev = 0;
        list->count--;
    }
    return data;
}

// Inlined into removeNodeFromLinkedList_guess only (no out-of-line copy in the client).
static __inline void *list_remove_tail_inl(RdpList_guess *list)
{
    RdpLink_guess *link;
    void *data;

    data = 0;
    link = list->tail;
    if (link)
    {
        data = link->data;
        list->tail = link->prev;
        if (list->tail == 0)
            list->head = 0;
        else
            list->tail->next = 0;
        list->count--;
    }
    return data;
}

// FUNCTION: TRILOGY 0x004f5480
// ORIGINAL FILE: Network/rdp/list.c (address: between Mac-mapped neighbours)
// __cdecl. Links `link` in at the head.
void list_add_head_guess(RdpList_guess *list, RdpLink_guess *link)
{
    list_add_head_inl(list, link);
}

// FUNCTION: TRILOGY 0x004f54c0
// ORIGINAL FILE: Network/rdp/list.c (address: between Mac-mapped neighbours)
// __cdecl. Links `link` in at the tail.
void list_add_tail_guess(RdpList_guess *list, RdpLink_guess *link)
{
    list_add_tail_inl(list, link);
}

// FUNCTION: TRILOGY 0x004f5500
// Remove the next RDP entry waiting at the front of a list.
// ORIGINAL FILE: Network/rdp/list.c (Mac: list_remove_head, low)
// __cdecl. Unlinks the head and returns its data; NULL (and nothing changes) when the list is empty. The
// link itself is not freed.
void *list_remove_head_guess(RdpList_guess *list)
{
    return list_remove_head_inl(list);
}

// FUNCTION: TRILOGY 0x004f5540
// Keep RDP entries ordered by key for later lookups.
// ORIGINAL FILE: Network/rdp/list.c (address: between Mac-mapped neighbours)
// __cdecl. Inserts `link` in key order: before the first link whose key compares greater than its key
// (compare(existing, new) > 0), so equal keys keep their arrival order.
void LinkedList_insertNodeIntoSortedLinkedList_guess(RdpList_guess *list, RdpLink_guess *link)
{
    RdpLink_guess *p;

    if (list->head == 0 || list->compare(list->head->key, link->key) > 0)
    {
        list_add_head_inl(list, link);
    }
    else if (list->compare(list->tail->key, link->key) <= 0)
    {
        list_add_tail_inl(list, link);
    }
    else
    {
        p = list->head->next;
        while (list->compare(p->key, link->key) <= 0)
            p = p->next;
        link->next = p;
        link->prev = p->prev;
        p->prev = link;
        link->prev->next = link;
        list->count++;
    }
}

// FUNCTION: TRILOGY 0x004f5630
// Find an RDP entry whose key matches the requested value.
// ORIGINAL FILE: Network/rdp/list.c (Mac: list_find_link_by_key, low)
// __cdecl. The first link whose key compares equal to `key`, or NULL. A sorted list stops at the first
// link that compares greater.
RdpLink_guess *list_find_link_by_key_guess(RdpList_guess *list, void *key)
{
    RdpLink_guess *p;
    int r;

    for (p = list->head; p; p = p->next)
    {
        r = list->compare(p->key, key);
        if (r == 0)
            break;
        if (list->sorted && r > 0)
            return 0;
    }
    return p;
}

// FUNCTION: TRILOGY 0x004f5670
// ORIGINAL FILE: Network/rdp/list.c (address: between Mac-mapped neighbours)
// __cdecl. Unlinks `link` from `list` and returns its data. The link is not freed.
void *removeNodeFromLinkedList_guess(RdpList_guess *list, RdpLink_guess *link)
{
    void *data;

    if (link->prev == 0)
        return list_remove_head_inl(list);
    if (link->next == 0)
        return list_remove_tail_inl(list);
    link->prev->next = link->next;
    link->next->prev = link->prev;
    data = link->data;
    list->count--;
    return data;
}

// FUNCTION: TRILOGY 0x004f67c0
// Reset an RDP network object's buffering state; its specific role is unclear.
// __cdecl. Resets a 0x94-byte network object (no header struct matches it; RdpSerialQueue_guess covers only
// 0x50 of it): +0x1C = -1, +0x90 = 0, list at +0x38, +0x4C = 0, ring header at +0x24, 0x40 bytes zeroed at +0x50.
void initializeNetworkBuffer_guess(void *obj)
{
    char *p = (char *)obj;

    *(int *)(p + 0x1c) = -1;
    *(int *)(p + 0x90) = 0;
    list_init_guess((RdpList_guess *)(p + 0x38));
    *(int *)(p + 0x4c) = 0;
    initializeNetworkBufferHeader_guess((RdpRing_guess *)(p + 0x24));
    memset(p + 0x50, 0, 0x40);
}

typedef void (__stdcall *NetworkQueueInitializeCriticalSectionFn_guess)(void *section);
typedef void (__stdcall *NetworkQueueEnterCriticalSectionFn_guess)(void *section);
typedef int (__stdcall *NetworkQueueGetOverlappedResultFn_guess)(void *file, void *overlapped, u32 *transferred, int wait);
typedef u32 (__stdcall *NetworkQueueGetLastErrorFn_guess)(void);
typedef int (__stdcall *NetworkQueueCloseHandleFn_guess)(void *handle);
typedef void (__stdcall *NetworkQueueLeaveCriticalSectionFn_guess)(void *section);
typedef void (__stdcall *NetworkQueueDeleteCriticalSectionFn_guess)(void *section);

// FUNCTION: TRILOGY 0x004f6810
// Queue setup and conditional I/O cleanup; the intended network lifecycle is unclear.
// @early-stop: cleanup calls merge, stack cleanup moves before subtraction, and the failed-allocation epilogue zeros EAX.
/* fake match: volatile result pins the allocation value in the reused argument slot; closeHandle keeps the cached EBP call target. */
// __cdecl. Initializes a network queue's lock, list and ring buffer, then drains completed or failed items.
// The object layout beyond the critical section and list is not represented by an existing header struct.
int initialize_network_queue_guess(char *p, u32 transferred)
{
    RdpList_guess *list;
    int allocated;
    volatile int result;
    NetworkQueueCloseHandleFn_guess closeHandle;
    char *item;
    u32 error;

    list = (RdpList_guess *)(p + 0x38);
    setLinkedListProperties_by_addr_guess(list, 0, 0);
    EQ_GLOBAL(NetworkQueueInitializeCriticalSectionFn_guess, 0x005340E0)(p);
    *(u16 *)(p + 0x20) = (u16)transferred;
    allocated = allocate_memory_buffer_by_addr_guess((RdpRing_guess *)(p + 0x24), 8000);
    result = allocated;
    if (allocated != 0)
    {
        EQ_GLOBAL(NetworkQueueEnterCriticalSectionFn_guess, 0x00534194)(p);
        EQ_GLOBAL(u32, 0x006EFED4) = EQ_STDCALL(u32, 0x004F66D0, (void))();
        closeHandle = EQ_GLOBAL(NetworkQueueCloseHandleFn_guess, 0x005341D4);
        while (list->head != 0 && (item = list->head->data) != 0)
        {
            if (EQ_GLOBAL(NetworkQueueGetOverlappedResultFn_guess, 0x005340C0)(*(void **)(p + 0x1c), item + 0x10, &transferred, 1) != 0)
            {
                char *removed;

                if (transferred == *(u32 *)(item + 0x24))
                    ++*(u32 *)(p + 0x78);
                else
                    ++*(u32 *)(p + 0x74);
                removed = list_remove_head_by_addr_guess(list);
                *(int *)((char *)list + 0x14) = *(int *)((char *)list + 0x14) - *(int *)(removed + 0x24);
                closeHandle(*(void **)(item + 0x20));
                fast_free_by_addr_guess(item);
            }
            else
            {
                char *removed;

                error = EQ_GLOBAL(NetworkQueueGetLastErrorFn_guess, 0x005341AC)();
                if (error == 0x3e4)
                    break;
                ++*(u32 *)(p + 0x80);
                removed = list_remove_head_by_addr_guess(list);
                *(int *)((char *)list + 0x14) = *(int *)((char *)list + 0x14) - *(int *)(removed + 0x24);
                closeHandle(*(void **)(item + 0x20));
                fast_free_by_addr_guess(item);
            }
        }
        FreeRingBuffer_by_addr_guess((RdpRing_guess *)(p + 0x24));
        no_op_empty_return_by_addr_guess(list);
        EQ_GLOBAL(NetworkQueueLeaveCriticalSectionFn_guess, 0x00534190)(p);
        EQ_GLOBAL(NetworkQueueDeleteCriticalSectionFn_guess, 0x005341C8)(p);
        return result;
    }
    return allocated;
}

// FUNCTION: TRILOGY 0x004f7df0
// Retire sent RDP packets that match the acknowledged sequence.
// __cdecl. Rotates the whole list once: each item whose short at +0x1A equals `seq` is taken out and its
// size (+0x28) subtracted from the byte counter that follows the list (+0x14, as RdpConnection_guess's
// tx_sent_bytes after tx_sent); the others go back to the tail. Returns the last item taken out, or NULL.
// The item is the link itself (data == link); no header struct describes its +0x1A/+0x28 fields.
void *find_and_remove_packet_by_sequence_guess(RdpList_guess *list, short seq)
{
    char *item;
    char *found;
    int n;

    found = 0;
    n = list->count;
    while (n--)
    {
        item = (char *)list_remove_head_by_addr_guess(list);
        if (*(short *)(item + 0x1a) == seq)
        {
            *(int *)((char *)list + 0x14) -= *(int *)(item + 0x28);
            found = item;
        }
        else
            list_add_tail_by_addr_guess(list, (RdpLink_guess *)item);
    }
    return found;
}

// FUNCTION: TRILOGY 0x004f7e50
// Find the oldest outstanding send time for RDP transmit stall checks.
// __cdecl. Rotates the whole list once and returns the smallest int at +0x10 of its items (wrap-safe:
// compared by the sign of the difference), starting from the head link's data. An empty list reads
// address 0x10 (bug kept). No header struct describes the item's +0x10 field.
int get_min_packet_time_guess(RdpList_guess *list)
{
    char *item;
    int n;
    int min;

    n = list->count;
    min = *(int *)((char *)(list->head != 0 ? list->head->data : 0) + 0x10);
    while (n--)
    {
        item = (char *)list_remove_head_by_addr_guess(list);
        if (min - *(int *)(item + 0x10) > 0)
            min = *(int *)(item + 0x10);
        list_add_tail_by_addr_guess(list, (RdpLink_guess *)item);
    }
    return min;
}
