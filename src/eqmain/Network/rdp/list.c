// eqmain: the same original object as trilogy's src/Network/rdp/list.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
// CFLAGS: /O2
/* list.c - doubly linked lists of the RDP layer: links carry a data pointer and a key, a list can keep its
 * links sorted by a compare callback.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/list.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c); the add/remove helpers are inlined into the sorted
 * insert and the link remove, so they are __inline here.
 */
#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

#define initializeNetworkBufferHeader_guess EQ_CDECL(void, 0x10031090, (RdpRing_guess *ring))
#define list_add_tail_by_addr_guess EQ_CDECL(void, 0x1002DF20, (RdpList_guess *list, RdpLink_guess *link))
#define list_remove_head_by_addr_guess EQ_CDECL(void *, 0x1002E130, (RdpList_guess *list))
#define setLinkedListProperties_by_addr_guess EQ_CDECL(void, 0x004F5460, (RdpList_guess *list, int sorted, RdpListCompare_guess compare))
#define allocate_memory_buffer_by_addr_guess EQ_CDECL(int, 0x100310B0, (RdpRing_guess *ring, u32 size))
#define freeRingBufferMemory_by_addr_guess EQ_CDECL(void, 0x1002E240, (RdpRing_guess *ring))
#define fast_free_by_addr_guess EQ_CDECL(void, 0x10028A90, (void *p))
#define no_op_empty_return_by_addr_guess EQ_CDECL(void, 0x10027940, (RdpList_guess *list))
#define timeGetTime_by_addr_guess EQ_CDECL(unsigned int, 0x1002F9F0, (void))
#define bandwidth_get_time_empty_by_addr_guess EQ_CDECL(int, 0x10030D30, (void *ring))
#define getTimeFromEQClient_by_addr_guess EQ_CDECL(int, 0x1002AFE0, (void *client))
#define NetworkConnection_SendPacket_by_addr_guess EQ_CDECL(void, 0x1002CD70, (int connection, void *packet, unsigned int size, unsigned short flags))
#define NetworkConnection_initializeRDPConnection_by_addr_guess EQ_CDECL(void, 0x1002CFC0, (int connection))
#define NetworkConnection_Disconnect_by_addr_guess EQ_CDECL(void, 0x1002C5B0, (int connection, int reason))
#define rdp_list_head_item_guess EQ_CDECL(int, 0x1002E120, (int *list))
#define packet_stats_pointer_guess EQ_GLOBAL(unsigned char *, 0x100498B4)

typedef void (__stdcall *NetworkQueueInitializeCriticalSectionFn_guess)(void *section);
typedef void (__stdcall *NetworkQueueEnterCriticalSectionFn_guess)(void *section);
typedef int (__stdcall *NetworkQueueGetOverlappedResultFn_guess)(void *file, void *overlapped, u32 *transferred, int wait);
typedef u32 (__stdcall *NetworkQueueGetLastErrorFn_guess)(void);
typedef int (__stdcall *NetworkQueueCloseHandleFn_guess)(void *handle);
typedef void (__stdcall *NetworkQueueLeaveCriticalSectionFn_guess)(void *section);
typedef void (__stdcall *NetworkQueueDeleteCriticalSectionFn_guess)(void *section);

// FUNCTION: EQMAIN 0x1002c930
// Maintains the peer's game-network session as outbound packets are sent or retried.
// __cdecl, void (EAX is not read after its one caller's call). One RDP pump step for a connection: sends
// the head of the send queue (peerPtr + 0x534) when the timing gate allows, or retransmits the head of
// the unacked list (peerPtr + 0x518) when its timer is due, or disconnects it when the window has passed.
// A reliable packet that went out goes to the tail of the unacked list, an unreliable one is freed.
// Stats are u64 counters in the packet_stats global. Raw offsets throughout: no list or connection
// struct in include/.
void NetworkConnection_ProcessNetworkPackets_guess(int peerPtr)
{
    int *sendQueue;
    int *unackedList;
    int pkt;
    unsigned int DVar5;
    int tmp;
    int unackedHead;
    unsigned int DVar8;
    unsigned int value;
    unsigned int other;

    sendQueue = (int *)(peerPtr + 0x534);
    rdp_list_head_item_guess(sendQueue);
    unackedList = (int *)(peerPtr + 0x518);
    pkt = rdp_list_head_item_guess(unackedList);
    DVar5 = timeGetTime_by_addr_guess();
    tmp = bandwidth_get_time_empty_by_addr_guess((void *)(peerPtr + 0x56c));
    if (*(short *)(peerPtr + 0x478) == 0x45) {
        tmp = getTimeFromEQClient_by_addr_guess((void *)*(int *)(peerPtr + 0xc));
    }
    ++g_rdp_stats_guess->c[268];
    if ((int)((DVar5 - tmp) + 10) < 0) {
        ++g_rdp_stats_guess->c[270];
        return;
    }
    if (*(int *)(peerPtr + 0x6c0) != 0) {
        tmp = rdp_list_head_item_guess(sendQueue);
        unackedHead = rdp_list_head_item_guess(unackedList);
        if (unackedHead == 0) {
            value = 0;
        }
        else {
            DVar8 = timeGetTime_by_addr_guess();
            value = DVar8 - *(int *)(unackedHead + 0xc);
        }
        if (tmp != 0) {
            if (*(short *)(peerPtr + 0x478) == 0x45) {
                if (value < 0x2711) goto sendQueued;
            }
            else {
                other = (unsigned int)*(unsigned short *)(peerPtr + 0x6a0) * 3 + (unsigned int)*(unsigned short *)(peerPtr + 0x69e);
                if (other < 0x33) {
clampSendMin:
                    other = 0x32;
                }
                else if (other < 0xffff) {
                    if (other < 0x33) goto clampSendMin;
                }
                else {
                    other = 0xffff;
                }
                if (value <= (other & 0xffff)) {
sendQueued:
                    pkt = (int)list_remove_head_by_addr_guess((RdpList_guess *)sendQueue);
                    if (pkt != 0) {
                        *(int *)(peerPtr + 0x54c) = *(int *)(peerPtr + 0x54c) - *(int *)(pkt + 0x20);
                    }
                    DVar5 = timeGetTime_by_addr_guess();
                    *(short *)(pkt + 0x1c) = *(short *)(pkt + 0x1c) + 1;
                    *(unsigned int *)(pkt + 0xc) = DVar5;
                    *(unsigned int *)(pkt + 8) = DVar5;
                    NetworkConnection_SendPacket_by_addr_guess(peerPtr, (void *)(pkt + 0x28), *(int *)(pkt + 0x20), *(unsigned short *)(pkt + 0x10));
                    if ((*(unsigned char *)(pkt + 0x11) & 2) == 0) {
                        ++*(unsigned __int64 *)(packet_stats_pointer_guess);
                        g_rdp_stats_guess->c[1] += *(unsigned int *)(pkt + 0x20);
                        *(int *)(peerPtr + 0x60) = *(int *)(peerPtr + 0x60) + 1;
                        *(int *)(peerPtr + 0x64) = *(int *)(peerPtr + 0x64) + *(int *)(pkt + 0x20);
                        fast_free_by_addr_guess((void *)pkt);
                        return;
                    }
                    ++g_rdp_stats_guess->c[2];
                    g_rdp_stats_guess->c[3] += *(unsigned int *)(pkt + 0x20);
                    *(int *)(peerPtr + 0x68) = *(int *)(peerPtr + 0x68) + 1;
                    *(int *)(peerPtr + 0x6c) = *(int *)(peerPtr + 0x6c) + *(int *)(pkt + 0x20);
                    *(int *)(peerPtr + 0x530) = *(int *)(peerPtr + 0x530) + *(int *)(pkt + 0x20);
                    list_add_tail_by_addr_guess((RdpList_guess *)unackedList, (RdpLink_guess *)pkt);
                    return;
                }
            }
        }
    }
    if (pkt == 0) goto noResend;
    value = (unsigned int)*(unsigned short *)(peerPtr + 0x69e) + (unsigned int)*(unsigned short *)(peerPtr + 0x6a0) * 2;
    if (value < 0x33) {
clampResendMin:
        value = 0x32;
    }
    else if (value < 0xffff) {
        if (value < 0x33) goto clampResendMin;
    }
    else {
        value = 0xffff;
    }
    tmp = (*(int *)(pkt + 0xc) + (value & 0xffff)) - DVar5;
    if (tmp == 10 || tmp + -10 < 0) {
        value = DVar5 - *(int *)(peerPtr + 0x20c);
        other = DVar5 - *(int *)(pkt + 8);
        if (((*(unsigned int *)(peerPtr + 0x6e4) >> 1 <= other) ||
            ((*(unsigned int *)(peerPtr + 0x6e8) >> 1 <= other && (*(unsigned int *)(peerPtr + 0x6e8) <= value)))) &&
           ((*(unsigned char *)(peerPtr + 0x5c) & 2) != 0)) {
            NetworkConnection_initializeRDPConnection_by_addr_guess(peerPtr);
        }
        if ((other < *(unsigned int *)(peerPtr + 0x6e4)) &&
           ((other < *(unsigned int *)(peerPtr + 0x6e8) || (value < *(unsigned int *)(peerPtr + 0x6e8))))) {
            pkt = (int)list_remove_head_by_addr_guess((RdpList_guess *)unackedList);
            if (pkt != 0) {
                *(int *)(peerPtr + 0x530) = *(int *)(peerPtr + 0x530) - *(int *)(pkt + 0x20);
            }
            *(short *)(pkt + 0x1c) = *(short *)(pkt + 0x1c) + 1;
            *(unsigned int *)(pkt + 0xc) = DVar5;
            if ((short)((*(short *)(pkt + 0x12) - *(short *)(peerPtr + 0x488)) + -0x78) < 0) {
                NetworkConnection_SendPacket_by_addr_guess(peerPtr, (void *)(pkt + 0x28), *(int *)(pkt + 0x20), *(unsigned short *)(pkt + 0x10));
                ++g_rdp_stats_guess->c[4];
                g_rdp_stats_guess->c[5] += *(unsigned int *)(pkt + 0x20);
                *(int *)(peerPtr + 0x70) = *(int *)(peerPtr + 0x70) + 1;
                *(int *)(peerPtr + 0x74) = *(int *)(peerPtr + 0x74) + *(int *)(pkt + 0x20);
            }
            *(int *)(peerPtr + 0x530) = *(int *)(peerPtr + 0x530) + *(int *)(pkt + 0x20);
            list_add_tail_by_addr_guess((RdpList_guess *)unackedList, (RdpLink_guess *)pkt);
            return;
        }
        if (*(int *)(peerPtr + 0x6d4) == 0) {
            return;
        }
        if (other < *(unsigned int *)(peerPtr + 0x6e4)) {
            NetworkConnection_Disconnect_by_addr_guess(peerPtr, 0x50000);
            return;
        }
        NetworkConnection_Disconnect_by_addr_guess(peerPtr, 0x40000);
        return;
    }
noResend:
    if (*(int *)(peerPtr + 0x6ec) == 0) {
        ++g_rdp_stats_guess->c[269];
        return;
    }
    NetworkConnection_SendPacket_by_addr_guess(peerPtr, (void *)0, 0, 0);
    return;
}

// FUNCTION: EQMAIN 0x10030ed0
// Removes a sent game packet with the requested sequence from the connection's pending traffic.
// Cloned from trilogy find_and_remove_packet_by_sequence_guess 0x004F7DF0 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
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
        if (*(short *)(item + 0x12) == seq)
        {
            *(int *)((char *)list + 0x18) -= *(int *)(item + 0x20);
            found = item;
        }
        else
            list_add_tail_by_addr_guess(list, (RdpLink_guess *)item);
    }
    return found;
}
