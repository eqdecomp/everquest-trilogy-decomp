// eqmain: the same original object as trilogy's src/Network/rdp/rdp.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* rdp.c - the rdp instance: receive/send entry points, its connections, its serial link.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/rdp.c (from Mac debug map)
 *
 * Also the small OS helpers the queue labels rdp but whose object the Mac map doesn't name (each says where
 * it sits by address).
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x1003230E: the client's import thunk for WSOCK32 ioctlsocket (jmp [IAT]); called directly. */
#define rdp_ioctlsocket_guess  EQ_STDCALL(int, 0x1003230E, (u32 s, long cmd, u32 *argp))
/* 0x004F21D0: connection_close_guess (not rewritten yet). */
#define connection_close_guess EQ_CDECL(void, 0x004F21D0, (RdpState_guess *rdp, RdpConnection_guess *conn, u32 reason, HANDLE *event))
/* 0x100322BA / 0x100322B4: the import thunks for WSOCK32 ntohs and inet_ntoa. */
#define rdp_ntohs_guess        EQ_STDCALL(u16, 0x100322BA, (u16 n))
#define rdp_inet_ntoa_guess    EQ_STDCALL(char *, 0x100322B4, (u32 addr))
/* 0x1002F9F0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x1002F9F0, (void))
/* Callees of initialize_network_connection_state_guess, by address (list.c, connhash.c, uevent.c). */
#define rdp_init_network_buffer_guess EQ_CDECL(void, 0x004F67C0, (void *obj))
#define rdp_init_counter_guess        EQ_CDECL(void, 0x1002E550, (void *p))
#define rdp_uevent_init_guess         EQ_CDECL(void, 0x10030500, (void **ev))
#define rdp_list_init_guess           EQ_CDECL(void, 0x004F5440, (RdpList_guess *list))

typedef void (__stdcall *rdp_critical_section_proc_guess)(LPCRITICAL_SECTION);
#define rdp_enter_critical_section_guess EQ_GLOBAL(rdp_critical_section_proc_guess, 0x1004014C)
#define rdp_leave_critical_section_guess EQ_GLOBAL(rdp_critical_section_proc_guess, 0x10040148)

/* Address thunks for helpers implemented in other rdp translation units. */
#define removeConnectionFromHashTable_guess(hash, conn) \
    EQ_CDECL(RdpConnection_guess *, 0x1002E800, (RdpConnHash_guess *, RdpConnection_guess *))(hash, conn)
#define uevent_init_guess(ev) \
    EQ_CDECL(void, 0x10030500, (HANDLE *))(ev)
#define uevent_create_guess(ev) \
    EQ_CDECL(int, 0x004F7890, (HANDLE *))(ev)
#define uevent_wait_guess(ev) \
    EQ_CDECL(void, 0x004F78C0, (HANDLE *))(ev)
#define uevent_destroy_guess(ev) \
    EQ_CDECL(void, 0x1002E890, (void *))(ev)
#define is_connection_inactive_guess(q) \
    EQ_CDECL(int, 0x10030410, (RdpSerialQueue_guess *))(q)
#define calculateAdjustedClientTime_guess(q) \
    EQ_CDECL(DWORD, 0x10030440, (RdpSerialQueue_guess *))(q)
#define ascending_timeout_data_cmp_guess(q) \
    EQ_CDECL(DWORD, 0x004F7180, (RdpSerialQueue_guess *))(q)

/* Callees used by rdp_create_guess, recorded by their Trilogy addresses. */
#define rdp_create_malloc_guess EQ_CDECL(void *, 0x1003342E, (size_t size))
#define rdp_create_state_init_guess EQ_CDECL(void, 0x004F0B90, (void *rdp))
#define rdp_create_wsastartup_guess EQ_STDCALL(int, 0x0050038C, (WORD version, LPWSADATA data))
#define rdp_create_queue_init_guess EQ_CDECL(int, 0x004F6810, (void *queue, u32 id))
#define rdp_create_semaphore_decrement_guess EQ_CDECL(void, 0x100303D0, (void *queue, DWORD now))
#define rdp_create_hash_init_guess EQ_CDECL(int, 0x1002E5D0, (void *hash, u32 size))
#define rdp_create_buffer_init_guess EQ_CDECL(int, 0x10030580, (void *buffer, int size))
#define rdp_create_semaphore_guess EQ_CDECL(int, 0x1002E8B0, (HANDLE *semaphore))
#define rdp_create_getprotobyname_guess EQ_STDCALL(struct protoent *, 0x005003EC, (const char *name))
#define rdp_create_socket_guess EQ_STDCALL(SOCKET, 0x005003E6, (int af, int type, int protocol))
#define rdp_create_htons_guess EQ_STDCALL(u16, 0x100322AE, (u16 value))
#define rdp_create_bind_guess EQ_STDCALL(int, 0x005003E0, (SOCKET s, struct sockaddr *addr, int len))
#define rdp_create_get_last_error_guess EQ_STDCALL(int, 0x100322E4, (void))
#define rdp_create_setsockopt_guess EQ_STDCALL(int, 0x1003229C, (SOCKET s, int level, int option, const char *value, int len))
#define rdp_create_getsockname_guess EQ_STDCALL(int, 0x005003D4, (SOCKET s, struct sockaddr *addr, int *len))
#define rdp_create_nonblocking_guess EQ_CDECL(int, 0x100309D0, (SOCKET s))
#define rdp_create_net_strerror_guess EQ_CDECL(char *, 0x10030660, (int error))
#define rdp_create_getsockopt_guess EQ_STDCALL(int, 0x005003A4, (SOCKET s, int level, int option, char *value, int *len))
#define rdp_create_closesocket_guess EQ_STDCALL(int, 0x005003BC, (SOCKET s))
#define rdp_create_cleanup_guess EQ_CDECL(void, 0x10029AE0, (void *rdp))
#define rdp_create_thread_guess EQ_CDECL(int, 0x10030530, (void *thread, void *start, void *context))

/* Callees used by rdp_destroy_guess, recorded by their Trilogy addresses. */
#define rdp_destroy_htonl_guess EQ_STDCALL(u32, 0x10032296, (u32 hostlong))
#define rdp_destroy_sendto_guess EQ_STDCALL(int, 0x10032290, (SOCKET s, char *buf, int len, int flags, char *to, int tolen))
#define rdp_destroy_wait_thread_guess EQ_CDECL(void, 0x100304A0, (void *thread, DWORD *exit_code))
#define rdp_destroy_uevent_destroy_guess EQ_CDECL(void, 0x1002E890, (void *event))
#define rdp_destroy_free_guess EQ_CDECL(void, 0x10033568, (void *ptr))

#undef rdp_destroy_htonl_guess
#undef rdp_destroy_sendto_guess
#undef rdp_destroy_wait_thread_guess
#undef rdp_destroy_uevent_destroy_guess
#undef rdp_destroy_free_guess

#define rdp_create_connection_malloc_guess \
    EQ_CDECL(void *, 0x1003342E, (u32 size))
#define rdp_create_connection_free_guess \
    EQ_CDECL(void, 0x10033568, (void *p))
#define rdp_connection_init_for_create_guess \
    EQ_CDECL(void, 0x004F1800, (RdpConnection_guess *conn, RdpState_guess *rdp, u32 arg3, u32 arg4))
#define rdp_initialize_connection_resources_guess \
    EQ_CDECL(int, 0x1002D1F0, (RdpConnection_guess *conn))
#define rdp_cleanup_connection_resources_guess \
    EQ_CDECL(void, 0x1002D210, (RdpConnection_guess *conn))
#define rdp_sprintf_for_create_guess \
    EQ_CDECL(int, 0x10034117, (char *out, const char *format, ...))
#define rdp_heap_insert_connection_guess \
    EQ_CDECL(int, 0x1002E260, (void *heap, void *elem))
#define rdp_add_connection_to_hash_guess \
    EQ_CDECL(void, 0x004F52D0, (RdpConnHash_guess *hash, RdpConnection_guess *conn))
#define rdp_lock_connection_remote_guess \
    EQ_CDECL(RdpConnection_guess *, 0x1002E6E0, (RdpConnHash_guess *hash, RdpAddr_guess *addr))

#define rdp_receive_list_remove_head_guess(list) \
    EQ_CDECL(void *, 0x1002E130, (RdpList_guess *))(list)
#define rdp_receive_serial_recv_from_guess(queue, buffer, size, info) \
    EQ_CDECL(int, 0x1002FD50, (RdpSerialQueue_guess *, void *, u32, void *))(queue, buffer, size, info)
#define rdp_receive_handle_data_recv_guess(rdp, buffer, size, info) \
    EQ_CDECL(void, 0x004EF550, (RdpState_guess *, void *, int, void *))(rdp, buffer, size, info)
#define rdp_receive_usemaphore_decrement_guess(queue, time) \
    EQ_CDECL(void, 0x100303D0, (RdpSerialQueue_guess *, DWORD))(queue, time)
#define rdp_receive_wait_for_event_guess(event, timeout) \
    EQ_CDECL(int, 0x1002E8E0, (HANDLE *, int))(event, timeout)
/* 0x100291A0: the datagram handler (FUNCTION 0x100291a0 in this file), cdecl, four args; returns uint. The
 * rdp_receive_handle_data_recv_guess above is trilogy's 0x004EF550, not this image's callee. */
#define rdp_receive_process_datagram_guess(rdp, buffer, size, from) \
    EQ_CDECL(u32, 0x100291A0, (RdpState_guess *, u32 *, u32, struct sockaddr *))(rdp, buffer, size, from)

/* list.c (eqmain): 0x1002E120 reads `*list - list[2]`, the list's head item (head minus link_offset, or
 * -link_offset when empty). 0x1002DF20 adds an item at the tail. */
#define rdp_list_head_item_guess EQ_CDECL(int, 0x1002E120, (int *list))
#define rdp_list_add_tail_item_guess EQ_CDECL(void, 0x1002DF20, (int *list, void *item))
/* 0x10028A90: frees an item, free(item - 8). */
#define rdp_fast_free_guess EQ_CDECL(void, 0x10028A90, (void *item))
/* connhash.c (eqmain) 0x1002E910: signalMessageProcessing_guess, ReleaseSemaphore(*sem, 1, 0). */
#define rdp_signal_message_processing_guess EQ_CDECL(void, 0x1002E910, (HANDLE *sem))

// FUNCTION: EQMAIN 0x10028ae0
/* fake match: the status scope preserves the original load/store scheduling. */
// Wake the network loop when a changed connection reaches the heap top.
// Cloned from trilogy rdp_resort_guess 0x004EF130 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
void rdp_resort_guess(int connectionPtr, int notifyFlag)
{
    struct AddressCopy { int words[4]; } address;
    int connection = connectionPtr;
    int rdp = *(int *)(connection + 0xc);
    {
        struct StatusCopy { int low; int high; } status;
        LPCRITICAL_SECTION lock;
        struct StatusCopy *saved_status;

        EQ_CDECL(void, 0x1002D230, (int, int *))(connection, (int *)&status);
        lock = (LPCRITICAL_SECTION)(rdp + 0x6c);
        rdp_enter_critical_section_guess(lock);
        saved_status = (struct StatusCopy *)(connection + 0x28);
        if (memcmp(saved_status, &status, sizeof(status)) != 0)
        {
            *saved_status = status;
            EQ_CDECL(void, 0x1002E480, (int, int *))(rdp + 0x58, (int *)(connection + 0x1c));
            if (connection == EQ_CDECL(int, 0x1002E530, (int *))((int *)(rdp + 0x58)) &&
                *(int *)(rdp + 0x84) == 0 && notifyFlag != 0)
            {
                SOCKET local_socket = *(SOCKET *)(rdp + 4);
                connectionPtr = 0;
                if (local_socket != (SOCKET)-1)
                {
                    address = *(struct AddressCopy *)(rdp + 0x14);
                    address.words[1] = EQ_STDCALL(u32, 0x10032296, (u32))(0x7f000001);
                    EQ_STDCALL(int, 0x10032290, (SOCKET, char *, int, int, struct sockaddr *, int))(
                        *(SOCKET *)(rdp + 4), (char *)&connectionPtr, 4, 0, (struct sockaddr *)&address, 0x10);
                }
                else
                {
                    SOCKET s = *(SOCKET *)(rdp + 0x10);
                    if (s != (SOCKET)-1)
                        EQ_STDCALL(int, 0x10032290, (SOCKET, char *, int, int, struct sockaddr *, int))(
                            s, (char *)&connectionPtr, 4, 0, (struct sockaddr *)(rdp + 0x34), 0xe);
                }
                *(int *)(rdp + 0x84) = 1;
            }
        }
        rdp_leave_critical_section_guess(lock);
    }
}

// FUNCTION: EQMAIN 0x10028bf0
// Releases the connection from tracking, then frees its resources if it was detached.
void rdp_unlock_guess(void *connection)
{
    int rdp;
    int removed;
    rdp_critical_section_proc_guess leaveCriticalSection;

    leaveCriticalSection = rdp_leave_critical_section_guess;
    rdp = (int)((RdpConnection_guess *)connection)->rdp;
    leaveCriticalSection((LPCRITICAL_SECTION)(&((RdpConnection_guess *)connection)->lock));
    removed = EQ_CDECL(int, 0x1002E800, (void *, void *))(&((RdpState_guess *)rdp)->hash, connection);
    if (removed != 0)
    {
        rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(&((RdpState_guess *)rdp)->timeout_lock));
        // 0x1c: the connection's heap element
        EQ_CDECL(void, 0x1002E310, (void *, void *))(&((RdpState_guess *)rdp)->timeout_heap,
                                                      (void *)(removed + 0x1c));
        leaveCriticalSection((LPCRITICAL_SECTION)(&((RdpState_guess *)rdp)->timeout_lock));
        EQ_CDECL(void, 0x1002D210, (void *))((void *)removed);
        EQ_CDECL(void, 0x10033568, (void *))((void *)removed);
    }
}

// FUNCTION: EQMAIN 0x10028c50
// Make a ready network message available for processing.
// __cdecl. Queues `item` at the tail of the list at rdp+0xa4 under the lock at rdp+0xbc, and releases the
// semaphore at rdp+0xa0 when the list had no head item. An item whose +0x8 pointer has a non-zero dword at
// +0x4c is freed instead and not queued. Raw offsets: the trilogy RdpState_guess names nothing at 0xA0-0xBC.
void rdp_enqueue_item_guess(RdpState_guess *rdp, void *item)
{
    int head;

    if (*(int *)((char *)item + 0x8) != 0 && *(int *)(*(int *)((char *)item + 0x8) + 0x4c) != 0)
    {
        rdp_fast_free_guess(item);
        return;
    }
    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0xbc));
    head = rdp_list_head_item_guess((int *)((char *)rdp + 0xa4));
    *(int *)((char *)item + 0x20) = 0;
    rdp_list_add_tail_item_guess((int *)((char *)rdp + 0xa4), item);
    if (head == 0)
    {
        rdp_signal_message_processing_guess((HANDLE *)((char *)rdp + 0xa0));
    }
    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0xbc));
}

/* Callees of process_network_message_guess, by address (their rewrites are in rx.c, 0x1002F3B0-0x1002F550). */
#define rdp_queue_message_on_channel_guess             EQ_CDECL(void, 0x1002F3B0, (int conn, u32 msg))
#define rdp_dequeue_message_by_sequence_guess          EQ_CDECL(int, 0x1002F3E0, (int conn, u32 channel))
#define rdp_validate_and_update_message_sequence_guess EQ_CDECL(int, 0x1002F440, (int conn, u32 msg))
#define rdp_clear_and_return_pending_message_guess     EQ_CDECL(int, 0x1002F530, (int conn))
#define rdp_mark_message_for_pending_processing_guess  EQ_CDECL(void, 0x1002F550, (int conn, u32 msg))

// FUNCTION: EQMAIN 0x10028cd0
// Apply channel-specific handling and pending rules to a received message.
// @early-stop: Remaining differences are callee-saved register allocation and /O2 folding masked flag checks to byte tests.
// __cdecl, three arguments (the caller pops them). Routes message `param_3` of connection `param_2` into the rdp
// queue at rdp+0xa4 under the lock at rdp+0xbc. Flag 0x4000 parks it as the connection's pending message; flag 0x1000
// sends it through the channel's sequence check (or, with 0x200, through its channel's sorted queue); a message whose
// +0x8 pointer has a non-zero dword at +0x4c is freed instead of queued. Last, the pending message is queued when due.
// Raw offsets: no include/ struct matches this layout.
void process_network_message_guess(int rdpState, int connection, u32 message)
{
    u16 msgFlags;
    u32 arrival;
    int tmp;
    int prevHead;

    msgFlags = *(u16 *)(message + 0xc);
    if ((msgFlags & 0x4000) != 0)
    {
        rdp_mark_message_for_pending_processing_guess(connection, message);
    }
    else
    {
        arrival = message;
        if ((msgFlags & 0x1000) != 0)
        {
            if ((msgFlags & 0x200) != 0)
            {
                /* The channel byte (msg+0x16) replaces message's low byte in its argument slot; the rest is passed on. */
                *(u8 *)&message = *(u8 *)(arrival + 0x16);
                rdp_queue_message_on_channel_guess(connection, arrival);
                tmp = rdp_dequeue_message_by_sequence_guess(connection, message);
                while (tmp != 0)
                {
                    if (*(int *)(tmp + 0x8) == 0 || *(int *)(*(int *)(tmp + 0x8) + 0x4c) == 0)
                    {
                        rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
                        prevHead = rdp_list_head_item_guess((int *)(rdpState + 0xa4));
                        *(int *)(tmp + 0x20) = 0;
                        rdp_list_add_tail_item_guess((int *)(rdpState + 0xa4), (void *)tmp);
                        if (prevHead == 0)
                        {
                            rdp_signal_message_processing_guess((HANDLE *)(rdpState + 0xa0));
                        }
                        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
                    }
                    else
                    {
                        rdp_fast_free_guess((void *)tmp);
                    }
                    tmp = rdp_dequeue_message_by_sequence_guess(connection, message);
                }
            }
            else
            {
                tmp = rdp_validate_and_update_message_sequence_guess(connection, arrival);
                if (tmp == 0 || (*(int *)(arrival + 0x8) != 0 && *(int *)(*(int *)(arrival + 0x8) + 0x4c) != 0))
                {
                    rdp_fast_free_guess((void *)arrival);
                }
                else
                {
                    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
                    tmp = rdp_list_head_item_guess((int *)(rdpState + 0xa4));
                    *(int *)(arrival + 0x20) = 0;
                    rdp_list_add_tail_item_guess((int *)(rdpState + 0xa4), (void *)arrival);
                    if (tmp == 0)
                    {
                        rdp_signal_message_processing_guess((HANDLE *)(rdpState + 0xa0));
                    }
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
                }
            }
        }
        else
        {
            if (*(int *)(arrival + 0x8) != 0 && *(int *)(*(int *)(arrival + 0x8) + 0x4c) != 0)
            {
                rdp_fast_free_guess((void *)arrival);
            }
            else
            {
                rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
                tmp = rdp_list_head_item_guess((int *)(rdpState + 0xa4));
                *(int *)(arrival + 0x20) = 0;
                rdp_list_add_tail_item_guess((int *)(rdpState + 0xa4), (void *)arrival);
                if (tmp == 0)
                {
                    rdp_signal_message_processing_guess((HANDLE *)(rdpState + 0xa0));
                }
                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
            }
        }
    }
    if (*(short *)(connection + 0x450) != 0 &&
        *(short *)(connection + 0x17c) == *(short *)(connection + 0x452) &&
        *(int *)(connection + 0x454) != 0)
    {
        tmp = rdp_clear_and_return_pending_message_guess(connection);
        if (*(int *)(tmp + 0x8) != 0 && *(int *)(*(int *)(tmp + 0x8) + 0x4c) != 0)
        {
            rdp_fast_free_guess((void *)tmp);
            return;
        }
        rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
        prevHead = rdp_list_head_item_guess((int *)(rdpState + 0xa4));
        *(int *)(tmp + 0x20) = 0;
        rdp_list_add_tail_item_guess((int *)(rdpState + 0xa4), (void *)tmp);
        if (prevHead == 0)
        {
            rdp_signal_message_processing_guess((HANDLE *)(rdpState + 0xa0));
        }
        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
    }
}

/* Callees of process_packet_arrival_guess, by address (eqmain.dll). */
#define rdp_arrival_parse_guess           EQ_CDECL(int, 0x1002D3C0, (int conn, u16 *packet, int len, RdpPacketInfo_guess *info))
#define rdp_arrival_process_data_guess    EQ_CDECL(void, 0x1002D660, (int conn, RdpPacketInfo_guess *info, int *accepted))
#define rdp_arrival_fast_malloc_guess     EQ_CDECL(int *, 0x100288B0, (int size))
#define rdp_arrival_init_node_guess       EQ_CDECL(void, 0x1002DE10, (int *node, int conn))
#define rdp_arrival_get_status_guess      EQ_CDECL(void, 0x1002D230, (int conn, int *status))
#define rdp_arrival_update_heap_guess     EQ_CDECL(void, 0x1002E480, (int *heap, u32 *elem))
#define rdp_arrival_heap_top_guess        EQ_CDECL(void *, 0x1002E530, (int *heap))
#define rdp_arrival_find_message_guess    EQ_CDECL(int *, 0x1002F470, (int conn, RdpPacketInfo_guess *info, u32 *data))
#define rdp_arrival_process_message_guess EQ_CDECL(void, 0x10028CD0, (int rdp, int conn, u32 msg))

// FUNCTION: EQMAIN 0x10028f50
// Turn incoming RDP packets into connection updates and completed messages.
// @early-stop: Structural similarity 0.8969; flag-mask narrowing and register/store scheduling still differ.
// __cdecl, four arguments (the caller pops them). Parses one arriving packet (packetData, param_4 bytes) of connection
// connection into packetInfo. A valid one goes to NetworkConnection_process_network_packet_data_guess; when that accepts it
// (skipDelivery), rdp+0x108 gains its length + 0x1c. A bad one with flag 0x2000 is counted at stats 0x168. With conn+0x6d4
// and +0x6e0 both clear, a node is queued on rdp+0xa4 (freed instead when its connection's timeout is armed, +0x4c)
// and +0x6e0 is set. The status is then compared with +0x28; on a change the timeout heap at (conn+0xc)+0x58 is
// re-sorted. Last, with +0x6d4 set and +0x4c clear, an accepted packet with data or a fin goes to rx_accept_fragment_guess, and the
// message it returns goes to process_network_message_guess. The early exits count stats 0xd0 (not connected) and 0xc8
// (timeout armed). Raw offsets: rdp+0xa0..+0x108 aren't named in eqmain/rdp.h.
void process_packet_arrival_guess(int rdpState, int connection, u16 *packetData, int param_4)
{
    LPCRITICAL_SECTION lpCriticalSection;
    int parseError;
    int connRdp;
    int *tmp;
    u32 *payload;
    int skipDelivery;
    int connStatus[2];
    RdpPacketInfo_guess packetInfo;

    skipDelivery = 1;
    parseError = rdp_arrival_parse_guess(connection, packetData, param_4, &packetInfo);
    rdp_timeGetTime_guess();
    if (parseError != 0)
    {
        if ((packetInfo.flags & 0x2000) != 0)
        {
            RDP_STAT_guess(0x168)++;
        }
    }
    else
    {
        rdp_arrival_process_data_guess(connection, &packetInfo, &skipDelivery);
        if (skipDelivery != 0)
        {
            *(int *)(rdpState + 0x108) += param_4 + 0x1c;
        }
    }
    if ((*(int *)(connection + 0x6d4) == 0) && (*(int *)(connection + 0x6e0) == 0))
    {
        tmp = rdp_arrival_fast_malloc_guess(0x34);
        rdp_arrival_init_node_guess(tmp, connection);
        if ((tmp[2] == 0) || (*(int *)(tmp[2] + 0x4c) == 0))
        {
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
            connRdp = rdp_list_head_item_guess((int *)(rdpState + 0xa4));
            tmp[8] = 0;
            rdp_list_add_tail_item_guess((int *)(rdpState + 0xa4), (void *)tmp);
            if (connRdp == 0)
            {
                rdp_signal_message_processing_guess((HANDLE *)(rdpState + 0xa0));
            }
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
        }
        else
        {
            rdp_fast_free_guess((void *)tmp);
        }
        *(int *)(connection + 0x6e0) = 1;
    }
    connRdp = *(int *)(connection + 0xc);
    rdp_arrival_get_status_guess(connection, connStatus);
    lpCriticalSection = (LPCRITICAL_SECTION)(connRdp + 0x6c);
    rdp_enter_critical_section_guess(lpCriticalSection);
    if (memcmp((void *)(connection + 0x28), connStatus, sizeof(connStatus)) != 0)
    {
        *(int *)(connection + 0x28) = connStatus[0];
        *(int *)(connection + 0x2c) = connStatus[1];
        tmp = (int *)(connRdp + 0x58);
        rdp_arrival_update_heap_guess(tmp, (u32 *)(connection + 0x1c));
        rdp_arrival_heap_top_guess(tmp);
    }
    rdp_leave_critical_section_guess(lpCriticalSection);
    if (*(int *)(connection + 0x6d4) == 0)
    {
        RDP_STAT_guess(0xd0)++;
        return;
    }
    if (*(int *)(connection + 0x4c) != 0)
    {
        RDP_STAT_guess(0xc8)++;
        return;
    }
    if ((((packetInfo.flags & 1) == 0) && (skipDelivery == 0)) && (parseError == 0))
    {
        payload = (u32 *)((u32)packetData + packetInfo.overhead);
        if (packetInfo.data_len != 0 || ((packetInfo.flags & 0x4000) != 0))
        {
            tmp = rdp_arrival_find_message_guess(connection, &packetInfo, payload);
            if (tmp != (int *)0)
            {
                rdp_arrival_process_message_guess(rdpState, connection, (u32)tmp);
            }
        }
    }
    return;
}

/* Callees of process_datagram_guess, by address (eqmain.dll). 0x1002E800 is removeConnectionFromHashTable_guess
   above and 0x100288B0 is rdp_arrival_fast_malloc_guess above. */
#define rdp_datagram_htonl_guess        EQ_STDCALL(u32, 0x10032296, (u32 hostlong))
#define rdp_datagram_ntohl_guess        EQ_STDCALL(u32, 0x100322A8, (u32 netlong))
#define rdp_datagram_sprintf_guess      EQ_CDECL(int, 0x10034117, (char *buf, const char *fmt, ...))
#define rdp_datagram_log_guess          EQ_CDECL(void, 0x1002FA10, (const char *fmt, ...))
#define rdp_datagram_decode_guess       EQ_CDECL(void, 0x1002F880, (u32 *buf, int blocks))
#define rdp_datagram_crc32_guess        EQ_CDECL(u32, 0x1002F580, (u32 seed, u8 *buf, u32 len))
#define rdp_datagram_set_record_guess   EQ_CDECL(void, 0x1002DD40, (int rec, u16 value))
#define rdp_datagram_msg_data_guess     EQ_CDECL(u32 *, 0x1002DE70, (int msg))
#define rdp_datagram_queue_msg_guess    EQ_CDECL(void, 0x10028C50, (int rdp, int msg))
#define rdp_datagram_lookup_conn_guess  EQ_CDECL(int, 0x1002E6E0, (short *hash, short *addr))
#define rdp_datagram_accept_conn_guess  EQ_CDECL(int, 0x1002AAD0, (int rdp, u32 *conn, short *addr, u32 flags))
#define rdp_datagram_process_guess      EQ_CDECL(void, 0x10028F50, (int rdp, int conn, u16 *packet, int len))
#define rdp_datagram_remove_heap_guess  EQ_CDECL(int, 0x1002E310, (int *heap, u32 *elem))
#define rdp_datagram_cleanup_conn_guess EQ_CDECL(void, 0x1002D210, (void *msg))
#define rdp_datagram_send_guess         EQ_CDECL(int, 0x1002F8D0, (SOCKET s, int segs, int count, struct sockaddr *to, int flags, int extra))
#define rdp_datagram_free_guess         EQ_CDECL(void, 0x10033568, (void *ptr))

// FUNCTION: EQMAIN 0x100291a0
// Screens a received datagram (decode, CRC) and routes it: connectionless message, or its sender's connection
// __cdecl, four arguments (the caller pops them). Takes one datagram of connection bytes at datagram from param_4 for the rdp
// at rdpState. Under 2 bytes it returns 0. A datagram from anywhere but the local address (AF_INET, port rdp+0x16,
// 127.0.0.1) whose 16 address bytes differ from rdp+0x34 is the body below; any other one returns *datagram when it is
// exactly 4 bytes, else counts stats 0x138 and returns 0. The body adds its size + 0x1c to rdp+0x104 and, with rdp+0x118
// set, decodes it when it is a multiple of 8 bytes (the low nibble of its last byte is the padding; above 8 the length
// becomes 0), or else logs it as an invalid size (stats 0x140). With rdp+0x118 or +0x11c set (and no 0xffff header) and at
// least 6 bytes, its last 4 bytes must be the CRC32 of the rest; a mismatch logs an illegal message (stats 0x148). A 0xffff
// header makes a message of the rest (0x34-byte node) queued through rdp_enqueue_item_guess. Otherwise the connection for the source is
// looked up (connhash_lock_guess). With none, the datagram is counted (stats 0x158, 0x160 or 0x170) and createNetworkConnection_guess may accept a
// connection for flag 0x2000; if still none, a 0x8000 reply goes back to the source. A connection gets
// process_packet_arrival_guess when its +0x58 heap is empty; then connhash_release_guess may return its message, which leaves the
// heap, is cleaned up and freed. Raw offsets: rdp+0x104..+0x11c aren't named in eqmain/rdp.h.
u32 process_datagram_guess(int rdpState, u32 *datagram, u32 connection, struct sockaddr *param_4)
{
    u_short hostWord;
    u_long longVal;
    int connRdp;
    u32 tmp;
    char *ipText;
    int *newMsg;
    u32 *msgData;
    u32 *statCounter;
    void *_Memory;
    u32 scratch;
    struct sockaddr *peerAddr;
    u8 lastByte;
    u8 *statsBase;
    u_short replyHeader;
    u_short replyWord1;
    u_short *sendBufPtr;
    u32 sendBufLen;
    u8 ipxText[16];
    char addrDesc[64];

    scratch = connection;
    if ((int)connection < 2)
    {
        return 0;
    }
    if (((param_4->sa_family != 2) || (*(short *)param_4->sa_data != *(short *)(rdpState + 0x16))) ||
        (longVal = rdp_datagram_htonl_guess(0x7f000001), *(u_long *)(param_4->sa_data + 2) != longVal))
    {
        if (memcmp(param_4, (void *)(rdpState + 0x34), 16) != 0)
        {
            connection = 0;
            *(u32 *)(rdpState + 0x104) = *(int *)(rdpState + 0x104) + scratch + 0x1c;
            if (*(int *)(rdpState + 0x118) != 0)
            {
                if (((scratch & 7) == 0) && (scratch != 0))
                {
                    rdp_datagram_decode_guess(datagram, (int)(scratch + ((int)scratch >> 0x1f & 7U)) >> 3);
                    lastByte = *(u8 *)((int)datagram + (scratch - 1));
                    if ((lastByte & 0xf) < 9)
                    {
                        scratch = scratch - (lastByte & 0xf);
                    }
                    else
                    {
                        scratch = 0;
                    }
                }
                else
                {
                    tmp = (u32)param_4->sa_family;
                    if (tmp == 2)
                    {
                        hostWord = rdp_ntohs_guess(*(u_short *)param_4->sa_data);
                        tmp = (u32)hostWord;
                        ipText = rdp_inet_ntoa_guess(*(u_long *)(param_4->sa_data + 2));
                        rdp_datagram_sprintf_guess(addrDesc, "AF_INET %s:%u", ipText, tmp);
                    }
                    else if (tmp == 6)
                    {
                        rdp_datagram_sprintf_guess(addrDesc, "AF_IPX %s:%s:%u", ipxText, &sendBufPtr,
                                                   (u32)*(u_short *)(param_4->sa_data + 10));
                    }
                    else if (tmp == 0x45)
                    {
                        rdp_datagram_sprintf_guess(addrDesc, "AF_COMPORT %u", (int)*(short *)param_4->sa_data);
                    }
                    else
                    {
                        rdp_datagram_sprintf_guess(addrDesc, "unknown address family (%u)", tmp);
                    }
                    rdp_datagram_log_guess("%s invalid size (%u)\n", addrDesc, scratch);
                    statsBase = (u8 *)g_rdp_stats_guess;
                    tmp = *(u32 *)(statsBase + 0x140);
                    *(u32 *)(statsBase + 0x140) = tmp + 1;
                    *(int *)(statsBase + 0x144) = *(int *)(statsBase + 0x144) + (u32)(0xfffffffe < tmp);
                }
            }
            if (((*(int *)(rdpState + 0x11c) == 0) && (*(int *)(rdpState + 0x118) == 0)) ||
                (((int)scratch < 6 || (hostWord = rdp_ntohs_guess((u_short)*datagram), hostWord == 0xffff))))
            {
                goto dispatch;
            }
            if (scratch < 6)
            {
reject_message:
                scratch = 0;
            }
            else
            {
                connRdp = scratch - 4;
                scratch = scratch - 4;
                longVal = rdp_datagram_ntohl_guess(*(u_long *)(connRdp + (int)datagram));
                tmp = rdp_datagram_crc32_guess(0, (u8 *)datagram, scratch);
                if (longVal != tmp)
                {
                    goto reject_message;
                }
            }
            peerAddr = param_4;
            if (scratch != 0)
            {
dispatch:
                if (1 < (int)scratch)
                {
                    hostWord = rdp_ntohs_guess((u_short)*datagram);
                    peerAddr = param_4;
                    if (hostWord == 0xffff)
                    {
                        tmp = scratch - 2;
                        newMsg = rdp_arrival_fast_malloc_guess((int)(scratch + 0x32));
                        memset(newMsg, 0, 0x34);
                        rdp_datagram_set_record_guess((int)newMsg, 0);
                        newMsg[7] = tmp;
                        *(short*)(newMsg + 3) = -1;
                        msgData = rdp_datagram_msg_data_guess((int)newMsg);
                        memcpy(msgData, (u8 *)datagram + 2, tmp);
                        newMsg[9] = *(int *)param_4;
                        newMsg[10] = *(int *)(param_4->sa_data + 2);
                        newMsg[0xb] = *(int *)(param_4->sa_data + 6);
                        newMsg[0xc] = *(int *)(param_4->sa_data + 10);
                        rdp_datagram_queue_msg_guess(rdpState, (int)newMsg);
                        return 0;
                    }
                    connection = rdp_datagram_lookup_conn_guess((short *)(rdpState + 0x50), (short *)param_4);
                    if (connection == 0)
                    {
                        if ((hostWord & 0x2000) == 0)
                        {
                            statCounter = (u32 *)((u8 *)g_rdp_stats_guess + 0x170);
                        }
                        else
                        {
                            connRdp = rdp_datagram_accept_conn_guess(rdpState, &connection, (short *)peerAddr, 0);
                            if (connRdp == 0)
                            {
                                statCounter = (u32 *)((u8 *)g_rdp_stats_guess + 0x158);
                            }
                            else
                            {
                                statCounter = (u32 *)((u8 *)g_rdp_stats_guess + 0x160);
                            }
                        }
                        tmp = *statCounter;
                        *statCounter = tmp + 1;
                        statCounter[1] = statCounter[1] + (u32)(0xfffffffe < tmp);
                        if (connection == 0)
                        {
                            replyHeader = rdp_create_htons_guess(0x8000);
                            replyWord1 = rdp_create_htons_guess(0);
                            sendBufPtr = &replyHeader;
                            sendBufLen = 6;
                            rdp_datagram_send_guess(*(SOCKET *)(rdpState + 4), (int)&sendBufPtr, 1, peerAddr,
                                                    *(int *)(rdpState + 0x118), *(int *)(rdpState + 0x11c));
                            return 0;
                        }
                    }
                    if (*(int *)(connection + 0x58) == 0)
                    {
                        rdp_datagram_process_guess(rdpState, connection, (u_short *)datagram, scratch);
                    }
                    scratch = connection;
                    connRdp = *(int *)(connection + 0xc);
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(connection + 0x34));
                    _Memory = (void *)removeConnectionFromHashTable_guess((RdpConnHash_guess *)(connRdp + 0x50),
                                                                          (RdpConnection_guess *)scratch);
                    if (_Memory != (void *)0)
                    {
                        rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(connRdp + 0x6c));
                        rdp_datagram_remove_heap_guess((int *)(connRdp + 0x58), (u32 *)((int)_Memory + 0x1c));
                        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(connRdp + 0x6c));
                        rdp_datagram_cleanup_conn_guess(_Memory);
                        rdp_datagram_free_guess(_Memory);
                        return 0;
                    }
                }
                return 0;
            }
            scratch = (u32)param_4->sa_family;
            if (scratch == 2)
            {
                hostWord = rdp_ntohs_guess(*(u_short *)param_4->sa_data);
                scratch = (u32)hostWord;
                ipText = rdp_inet_ntoa_guess(*(u_long *)(peerAddr->sa_data + 2));
                rdp_datagram_sprintf_guess(addrDesc, "AF_INET %s:%u", ipText, scratch);
            }
            else if (scratch == 6)
            {
                rdp_datagram_sprintf_guess(addrDesc, "AF_IPX %s:%s:%u", &sendBufPtr, ipxText,
                                           (u32)*(u_short *)(param_4->sa_data + 10));
            }
            else if (scratch == 0x45)
            {
                rdp_datagram_sprintf_guess(addrDesc, "AF_COMPORT %u", (int)*(short *)param_4->sa_data);
            }
            else
            {
                rdp_datagram_sprintf_guess(addrDesc, "unknown address family (%u)", scratch);
            }
            rdp_datagram_log_guess("%s illegal message\n", addrDesc);
            statCounter = (u32 *)((u8 *)g_rdp_stats_guess + 0x148);
            goto count_stat;
        }
    }
    if (scratch == 4)
    {
        return *datagram;
    }
    statCounter = (u32 *)((u8 *)g_rdp_stats_guess + 0x138);
count_stat:
    scratch = *statCounter;
    *statCounter = scratch + 1;
    statCounter[1] = statCounter[1] + (u32)(0xfffffffe < scratch);
    return 0;
}

/* Callees of process_icmp_error_guess, by address (eqmain.dll). 0x1002E800 is removeConnectionFromHashTable_guess above. */
#define rdp_udp_lookup_connection_guess EQ_CDECL(int, 0x1002E6E0, (int hash, void *key))
#define rdp_udp_pprocess_stats_guess    EQ_CDECL(void, 0x1002D870, (int conn, char type, u8 channel, char flag, u32 flags, int param_4))
#define rdp_udp_heap_top_guess          EQ_CDECL(void, 0x1002E530, (int *heap))
#define rdp_udp_cleanup_conn_guess      EQ_CDECL(void, 0x1002D210, (int conn))
#define rdp_udp_free_guess              EQ_CDECL(void, 0x10033568, (void *ptr))

// FUNCTION: EQMAIN 0x100296e0
// Apply ICMP error information to the matching UDP connection.
// __cdecl, four arguments (the caller pops them). Adds the datagram's length (+ 0x1c) to rdp+0x104, then takes it only
// when its header lengths, protocol byte and UDP marker check out and its port is the connection's (rdp+0x16 or
// rdp+0x26). Both addresses are logged. Port +0x26 also gets a checksum, whose byte is kept in the low byte of datagram
// and passed on with it. The connection is looked up by (port, source) and fed to
// NetworkConnection_pprocess_network_packet_stats_guess; a new node is queued unless one is already waiting; the status
// is compared with +0x28 and the heap at +0x58 re-sorted; last, the entry that removeConnectionFromHashTable_guess
// returns is taken out of its heap, cleaned up and freed. Raw offsets: no include/ struct matches this layout.
void process_icmp_error_guess(RdpState_guess *rdpState, u8 *datagram, u32 received, int param_4)
{
    LPCRITICAL_SECTION lpCriticalSection;
    u8 *ipPacket;
    u8 icmpType;
    u16 remotePort;
    u32 checksumSum;
    int icmpOffset;
    int connection;
    int innerOffset;
    int *tmp;
    int *statusCursor;
    void *_Memory;
    BOOL matches;
    int connStatus[2];
    u8 local_key[16];
    char addrText[20];

    ipPacket = datagram;
    if ((received != 0xffffffff) &&
        (rdpState->inputBytes_guess = rdpState->inputBytes_guess + received + 0x1c, 0x1b < (int)received))
    {
        icmpOffset = (*ipPacket & 0xf) * 4;
        if ((u32)(icmpOffset + 8) <= received)
        {
            icmpType = ipPacket[icmpOffset];
            if ((icmpType > 2) && ((icmpType < 5) || (icmpType == 0xb)) && ((u32)(icmpOffset + 0x1c) <= received))
            {
                innerOffset = (ipPacket[icmpOffset + 8] & 0xf) * 4 + icmpOffset;
                if (((u32)(innerOffset + 0x10) <= received) && (ipPacket[icmpOffset + 0x11] == 0x11) &&
                    ((*(u16 *)(ipPacket + innerOffset + 8) == rdpState->localAddr_guess.port) ||
                     (*(u16 *)(ipPacket + innerOffset + 8) == rdpState->ttlAddr_guess.port)))
                {
                    strcpy(addrText, rdp_inet_ntoa_guess(*(u32 *)(param_4 + 4)));
                    strcpy(addrText, rdp_inet_ntoa_guess(*(u32 *)(ipPacket + icmpOffset + 0x14)));
                    strcpy(addrText, rdp_inet_ntoa_guess(*(u32 *)(ipPacket + icmpOffset + 0x18)));
                    rdp_ntohs_guess(*(u16 *)(ipPacket + innerOffset + 8));
                    rdp_ntohs_guess(*(u16 *)(ipPacket + innerOffset + 10));
                    /* The low byte of the argument slot is cleared; the stats call below passes the slot as a uint. */
                    *(u8 *)&datagram = 0;
                    if (*(u16 *)(ipPacket + innerOffset + 8) == rdpState->ttlAddr_guess.port)
                    {
                        checksumSum = (*(u32 *)(ipPacket + icmpOffset + 0x14) >> 0x10) +
                                 (u32)*(u16 *)(ipPacket + innerOffset + 0xc) * 2 +
                                 (*(u32 *)(ipPacket + icmpOffset + 0x18) >> 0x10) +
                                 (u32)*(u16 *)(ipPacket + innerOffset + 0xe) +
                                 (*(u32 *)(ipPacket + icmpOffset + 0x14) & 0xffff) +
                                 (*(u32 *)(ipPacket + icmpOffset + 0x18) & 0xffff) +
                                 (u32)ipPacket[icmpOffset + 0x11] +
                                 (u32)*(u16 *)(ipPacket + innerOffset + 10) +
                                 (u32)*(u16 *)(ipPacket + innerOffset + 8);
                        connection = ((int)checksumSum >> 0x10) + (checksumSum & 0xffff);
                        checksumSum = connection + (connection >> 0x10);
                        connection = (checksumSum & 0xff) + ((int)checksumSum >> 8);
                        /* The checksum byte becomes the low byte of datagram (its slot), passed on as the uint below. */
                        *(u8 *)&datagram = (u8)~((char)((u32)connection >> 8) + (char)connection);
                        remotePort = rdp_ntohs_guess(*(u16 *)(ipPacket + innerOffset + 10));
                        remotePort = rdp_create_htons_guess(remotePort & 0x7fff);
                        *(u16 *)(ipPacket + innerOffset + 10) = remotePort;
                    }
                    /* sockaddr_in in one 16-byte array: family 2, port (+0xa), source address (+0x18), zeros. */
                    *(u32 *)(local_key + 8) = 0;
                    *(u32 *)(local_key + 12) = 0;
                    *(u16 *)local_key = 2;
                    *(u16 *)(local_key + 2) = *(u16 *)(ipPacket + innerOffset + 10);
                    *(u32 *)(local_key + 4) = *(u32 *)(ipPacket + icmpOffset + 0x18);
                    connection = rdp_udp_lookup_connection_guess((int)rdpState + 0x50, local_key);
                    if (connection != 0)
                    {
                        rdp_udp_pprocess_stats_guess(connection, ipPacket[icmpOffset], ipPacket[icmpOffset + 1],
                                                     *(u16 *)(ipPacket + innerOffset + 8) == rdpState->ttlAddr_guess.port,
                                                     (u32)datagram, param_4);
                        if ((*(int *)(connection + 0x6d4) == 0) && (*(int *)(connection + 0x6e0) == 0))
                        {
                            tmp = rdp_arrival_fast_malloc_guess(0x34);
                            rdp_arrival_init_node_guess(tmp, connection);
                            if ((tmp[2] == 0) || (*(int *)(tmp[2] + 0x4c) == 0))
                            {
                                rdp_enter_critical_section_guess((LPCRITICAL_SECTION)((int)rdpState + 0xbc));
                                icmpOffset = rdp_list_head_item_guess((int *)((char *)rdpState + 0xa4));
                                tmp[8] = 0;
                                rdp_list_add_tail_item_guess((int *)((char *)rdpState + 0xa4), (void *)tmp);
                                if (icmpOffset == 0)
                                {
                                    rdp_signal_message_processing_guess((HANDLE *)((char *)rdpState + 0xa0));
                                }
                                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((int)rdpState + 0xbc));
                            }
                            else
                            {
                                rdp_fast_free_guess((void *)tmp);
                            }
                            *(u32 *)(connection + 0x6e0) = 1;
                        }
                        icmpOffset = *(int *)(connection + 0xc);
                        rdp_arrival_get_status_guess(connection, connStatus);
                        lpCriticalSection = (LPCRITICAL_SECTION)(icmpOffset + 0x6c);
                        rdp_enter_critical_section_guess(lpCriticalSection);
                        innerOffset = 2;
                        matches = TRUE;
                        tmp = (int *)(connection + 0x28);
                        statusCursor = connStatus;
                        do
                        {
                            if (innerOffset == 0)
                                break;
                            innerOffset = innerOffset + -1;
                            matches = *tmp == *statusCursor;
                            tmp = tmp + 1;
                            statusCursor = statusCursor + 1;
                        } while (matches);
                        if (!matches)
                        {
                            *(int *)(connection + 0x28) = connStatus[0];
                            *(int *)(connection + 0x2c) = connStatus[1];
                            tmp = (int *)(icmpOffset + 0x58);
                            rdp_arrival_update_heap_guess(tmp, (u32 *)(connection + 0x1c));
                            rdp_udp_heap_top_guess(tmp);
                        }
                        rdp_leave_critical_section_guess(lpCriticalSection);
                        icmpOffset = *(int *)(connection + 0xc);
                        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(connection + 0x34));
                        _Memory = (void *)removeConnectionFromHashTable_guess((RdpConnHash_guess *)(icmpOffset + 0x50),
                                                                              (RdpConnection_guess *)connection);
                        if (_Memory != (void *)0)
                        {
                            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(icmpOffset + 0x6c));
                            rdp_datagram_remove_heap_guess((int *)(icmpOffset + 0x58), (u32 *)((int)_Memory + 0x1c));
                            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(icmpOffset + 0x6c));
                            rdp_udp_cleanup_conn_guess((int)_Memory);
                            rdp_udp_free_guess(_Memory);
                        }
                    }
                }
            }
        }
    }
    return;
}

/* Callees of rdp_state_cleanup_guess, by address (eqmain.dll). 0x10027940 is the client's one-byte `ret`
 * (connhash.c's rdp_list_release_stub_guess); the IAT slot 0x100400F0 holds DeleteCriticalSection. */
#define rdp_cleanup_heap_extract_guess      EQ_CDECL(int, 0x1002E4F0, (int *heap))
#define rdp_cleanup_list_remove_head_guess  EQ_CDECL(int, 0x1002E130, (int *list))
#define rdp_cleanup_free_guess              EQ_CDECL(void, 0x10033568, (void *ptr))
#define rdp_cleanup_free_linked_list_guess  EQ_CDECL(void, 0x1002E240, (void *list))
#define rdp_cleanup_hash_table_guess        EQ_CDECL(void, 0x1002E680, (RdpConnHash_guess *hash))
#define rdp_enter_critical_section_timed_guess EQ_CDECL(void, 0x1002FC60, (LPCRITICAL_SECTION cs))
#define rdp_list_release_stub_guess         EQ_CDECL(void, 0x10027940, (RdpList_guess *list))
#define rdp_cleanup_closesocket_guess       EQ_STDCALL(int, 0x100322C6, (SOCKET s))
#define rdp_cleanup_wsacleanup_guess        EQ_STDCALL(int, 0x100322C0, (void))
#define rdp_delete_critical_section_guess   EQ_GLOBAL(rdp_critical_section_proc_guess, 0x100400F0)

// FUNCTION: EQMAIN 0x10029ae0
// Tears down the network state: frees every connection and queued message, closes the sockets, stops Winsock
// __cdecl, one argument (the caller pops it). Pops every connection off the heap (removed from the hash table,
// its resources cleaned, then freed), frees the queued messages of both lists, closes the four sockets and calls
// WSACleanup if it was started, then releases the hash table, the list links, the critical sections and the event.
// The block itself is freed when the flag at +0x88 is 0.
void rdp_state_cleanup_guess(int *rdpState)
{
    int *connHeap;
    int temp;
    RdpConnection_guess *_Memory;
    rdp_critical_section_proc_guess deleteCriticalSection;

    connHeap = rdpState + 0x16;
    temp = rdp_cleanup_heap_extract_guess(connHeap);
    if (temp != 0)
    {
        do
        {
            _Memory = removeConnectionFromHashTable_guess((RdpConnHash_guess *)(rdpState + 0x14), (RdpConnection_guess *)temp);
            rdp_cleanup_connection_resources_guess(_Memory);
            rdp_cleanup_free_guess(_Memory);
            temp = rdp_cleanup_heap_extract_guess(connHeap);
        } while (temp != 0);
    }
    temp = rdp_cleanup_list_remove_head_guess(rdpState + 0x29);
    while (temp != 0)
    {
        rdp_fast_free_guess((void *)temp);
        temp = rdp_cleanup_list_remove_head_guess(rdpState + 0x29);
    }
    temp = rdp_cleanup_list_remove_head_guess(rdpState + 0x35);
    while (temp != 0)
    {
        rdp_fast_free_guess((void *)temp);
        temp = rdp_cleanup_list_remove_head_guess(rdpState + 0x35);
    }
    if (rdpState[1] != 0xffffffff)
    {
        rdp_cleanup_closesocket_guess(rdpState[1]);
    }
    rdpState[1] = -1;
    if (rdpState[2] != 0xffffffff)
    {
        rdp_cleanup_closesocket_guess(rdpState[2]);
    }
    rdpState[2] = -1;
    if (rdpState[3] != 0xffffffff)
    {
        rdp_cleanup_closesocket_guess(rdpState[3]);
    }
    rdpState[3] = -1;
    if (rdpState[4] != 0xffffffff)
    {
        rdp_cleanup_closesocket_guess(rdpState[4]);
    }
    rdpState[4] = -1;
    if (*rdpState != 0)
    {
        rdp_cleanup_wsacleanup_guess();
        *rdpState = 0;
    }
    rdp_enter_critical_section_timed_guess((LPCRITICAL_SECTION)(rdpState + 0x50));
    rdp_cleanup_hash_table_guess((RdpConnHash_guess *)(rdpState + 0x14));
    rdp_cleanup_free_linked_list_guess(connHeap);
    deleteCriticalSection = rdp_delete_critical_section_guess;
    connHeap += 5;
    deleteCriticalSection((LPCRITICAL_SECTION)connHeap);
    uevent_destroy_guess(rdpState + 0x28);
    rdp_list_release_stub_guess((RdpList_guess *)(rdpState + 0x29));
    deleteCriticalSection((LPCRITICAL_SECTION)(rdpState + 0x2f));
    rdp_list_release_stub_guess((RdpList_guess *)(rdpState + 0x35));
    deleteCriticalSection((LPCRITICAL_SECTION)(rdpState + 0x3b));
    deleteCriticalSection((LPCRITICAL_SECTION)(rdpState + 0x48));
    if (rdpState[0x22] == 0)
    {
        uevent_destroy_guess(rdpState + 0x24);
        rdp_cleanup_free_guess(rdpState);
    }
    return;
}

/* Callees of rdp_network_thread_proc_guess, by address (eqmain.dll): the WSOCK32 thunks for select, recvfrom and __WSAFDIsSet, and the
 * rdp helpers no other function here calls. 0x100303F0 returns the serial queue's deadline (its value at +0x18). */
#define rdp_loop_select_guess          EQ_STDCALL(int, 0x100322D8, (int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout))
#define rdp_loop_recvfrom_guess        EQ_STDCALL(int, 0x100322CC, (SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen))
#define rdp_loop_fd_isset_guess        EQ_STDCALL(int, 0x100322D2, (SOCKET s, fd_set *set))
#define rdp_loop_next_timeout_guess    EQ_CDECL(struct timeval *, 0x100305F0, (int *heap, struct timeval *out))
#define rdp_loop_serial_deadline_guess EQ_CDECL(DWORD, 0x100303F0, (RdpSerialQueue_guess *queue))
#define rdp_loop_sleep_guess           EQ_CDECL(void, 0x1002FA00, (DWORD ms))
#define rdp_loop_process_udp_guess     EQ_CDECL(void, 0x100296E0, (int rdp, u8 *packet, u32 len, int from))
#define rdp_loop_connection_state_guess EQ_CDECL(void, 0x1002DAE0, (RdpConnection_guess *conn, DWORD now, int *status))
#define rdp_loop_timed_out_guess       EQ_CDECL(int, 0x1002DD00, (RdpConnection_guess *conn))
/* The rdp fields RdpState_guess doesn't name: sockets at +0x4/+0x8/+0x10, the heap lock +0x6c, the run flag +0x8c,
 * the message list +0xa4 (lock +0xbc, semaphore +0xa0), byte counts +0x104/+0x108 and their rates, the serial flag +0x15c. */
#define RDP_INT(rdp, off)  (*(int *)((char *)(rdp) + (off)))
#define RDP_UINT(rdp, off) (*(u32 *)((char *)(rdp) + (off)))

// FUNCTION: EQMAIN 0x10029c50
// Network thread: reads the sockets and serial link, services due connections and drops timed-out ones
// __cdecl thread procedure (rdp_create starts it): while rdp+0x8c is set, waits for the sockets or the next connection
// timeout, reads the serial link and the sockets, updates the byte rates and runs every due connection; then cleans up.
void rdp_network_thread_proc_guess(RdpState_guess *rdp)
{
    int fromlen;
    int nfds;
    struct sockaddr from;
    DWORD start;
    int ready;
    int status[2];
    struct timeval wait;
    int was_empty;
    RdpAddr_guess key;
    struct sockaddr serial_from;
    fd_set readfds;
    u32 buffer[134];
    u32 serial_buffer[134];
    struct timeval *timeout;
    DWORD sleep_ms;
    int remaining;
    int usec;
    DWORD deadline;
    int received;
    u32 handled;
    DWORD now;
    u32 elapsed;
    u32 bytes;
    int *node;
    RdpConnection_guess *conn;
    RdpConnection_guess *removed;
    RdpState_guess *owner;

    start = rdp_timeGetTime_guess();
    rdp_timeGetTime_guess();
    nfds = 1;
    if (rdp->udpSocket_guess + 1 > nfds)
    {
        nfds = rdp->udpSocket_guess + 1;
    }
    if (rdp->icmpSocket_guess + 1 > nfds)
    {
        nfds = rdp->icmpSocket_guess + 1;
    }
    if (rdp->ipxSocket_guess + 1 > nfds)
    {
        nfds = rdp->ipxSocket_guess + 1;
    }
    while (rdp->running_guess != 0)
    {
        FD_ZERO(&readfds);
        if (rdp->udpSocket_guess != -1)
        {
            FD_SET(rdp->udpSocket_guess, &readfds);
        }
        if (rdp->ipxSocket_guess != -1)
        {
            FD_SET(rdp->ipxSocket_guess, &readfds);
        }
        if (rdp->icmpSocket_guess != -1)
        {
            FD_SET(rdp->icmpSocket_guess, &readfds);
        }
        EnterCriticalSection((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
        timeout = rdp_loop_next_timeout_guess((int *)((char *)rdp + 0x58), &wait);
        if (timeout != 0)
        {
            sleep_ms = timeout->tv_sec * 1000 + timeout->tv_usec / 1000;
        }
        else
        {
            sleep_ms = -1;
        }
        rdp->wakeupPending_guess = 0;
        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
        if (RDP_INT(rdp, 0x15c) != -1)
        {
            remaining = rdp_loop_serial_deadline_guess(&rdp->serial) - rdp_timeGetTime_guess();
            if (remaining > 0)
            {
                usec = remaining * 1000;
            }
            else
            {
                usec = 0;
            }
            if (timeout == 0 || timeout->tv_usec > usec || timeout->tv_sec != 0)
            {
                timeout = &wait;
                wait.tv_sec = 0;
                wait.tv_usec = usec;
                sleep_ms = usec / 1000;
            }
        }
        if (RDP_INT(rdp, 0x15c) != -1 && rdp->ipxSocket_guess == -1 && rdp->udpSocket_guess == -1)
        {
            rdp_loop_sleep_guess(sleep_ms);
        }
        else if (timeout != 0 && timeout->tv_usec == 0 && timeout->tv_sec == 0)
        {
            elapsed = rdp_timeGetTime_guess() - start;
            if (elapsed < 4)
            {
                rdp_loop_sleep_guess(4 - elapsed);
            }
            ready = 2;
        }
        else if (rdp->ipxSocket_guess == -1 && rdp->udpSocket_guess == -1)
        {
            ready = 0;
            rdp_loop_sleep_guess(100);
        }
        else
        {
            ready = rdp_loop_select_guess(nfds, &readfds, 0, 0, timeout);
        }
        rdp_timeGetTime_guess();
        if (RDP_INT(rdp, 0x15c) != -1)
        {
            deadline = rdp_loop_serial_deadline_guess(&rdp->serial);
            if ((int)(rdp_timeGetTime_guess() - deadline) > 0)
            {
                do
                {
                    received = rdp_receive_serial_recv_from_guess(&rdp->serial, serial_buffer, 0x218, &serial_from);
                    rdp_receive_process_datagram_guess(rdp, serial_buffer, received, &serial_from);
                } while (received != -1);
                rdp_receive_usemaphore_decrement_guess(&rdp->serial, rdp_timeGetTime_guess() + 100);
            }
        }
        if (ready > 0)
        {
            if (rdp->udpSocket_guess != -1 && rdp_loop_fd_isset_guess(rdp->udpSocket_guess, &readfds) != 0)
            {
                fromlen = sizeof(from);
                received = rdp_loop_recvfrom_guess(rdp->udpSocket_guess, (char *)buffer, 0x218, 0, &from, &fromlen);
                handled = rdp_receive_process_datagram_guess(rdp, buffer, received, &from);
                while (handled == 0 && received != -1)
                {
                    fromlen = sizeof(from);
                    received = rdp_loop_recvfrom_guess(rdp->udpSocket_guess, (char *)buffer, 0x218, 0, &from, &fromlen);
                    handled = rdp_receive_process_datagram_guess(rdp, buffer, received, &from);
                }
            }
            if (rdp->ipxSocket_guess != -1 && rdp_loop_fd_isset_guess(rdp->ipxSocket_guess, &readfds) != 0)
            {
                memset(&from, 0, sizeof(from));
                fromlen = sizeof(from);
                received = rdp_loop_recvfrom_guess(rdp->ipxSocket_guess, (char *)buffer, 0x218, 0, &from, &fromlen);
                handled = rdp_receive_process_datagram_guess(rdp, buffer, received, &from);
                while (handled == 0 && received != -1)
                {
                    memset(&from, 0, sizeof(from));
                    fromlen = sizeof(from);
                    received = rdp_loop_recvfrom_guess(rdp->ipxSocket_guess, (char *)buffer, 0x218, 0, &from, &fromlen);
                    handled = rdp_receive_process_datagram_guess(rdp, buffer, received, &from);
                }
            }
            if (rdp->icmpSocket_guess != -1 && rdp_loop_fd_isset_guess(rdp->icmpSocket_guess, &readfds) != 0)
            {
                do
                {
                    memset(&from, 0, sizeof(from));
                    fromlen = sizeof(from);
                    received = rdp_loop_recvfrom_guess(rdp->icmpSocket_guess, (char *)buffer, 0x218, 0, &from, &fromlen);
                    rdp_loop_process_udp_guess((int)rdp, (u8 *)buffer, received, (int)&from);
                } while (received != -1);
            }
        }
        else
        {
            RDP_STAT_guess(0x1a8)++;
        }
        now = rdp_timeGetTime_guess();
        elapsed = now - rdp->rateTime_guess;
        if (elapsed > 1000)
        {
            bytes = rdp->inputBytes_guess;
            rdp->inputBytes_guess = 0;
            rdp->input_rate = bytes * 1000 / elapsed;
            rdp->rateTime_guess = now;
            bytes = rdp->skippedBytes_guess;
            rdp->skippedBytes_guess = 0;
            rdp->skippedRate_guess = bytes * 1000 / elapsed;
        }
        start = rdp_timeGetTime_guess();
        for (;;)
        {
            EnterCriticalSection((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
            conn = (RdpConnection_guess *)rdp_arrival_heap_top_guess((int *)((char *)rdp + 0x58));
            if (conn != 0 && conn->field_030 == 0 && (int)(start - conn->field_034) >= 0)
            {
                key = conn->remote;
            }
            else
            {
                conn = 0;
            }
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
            if (conn == 0)
            {
                break;
            }
            conn = rdp_lock_connection_remote_guess(&rdp->hash, &key);
            if (conn == 0)
            {
                continue;
            }
            rdp_loop_connection_state_guess(conn, start, status);
            if (rdp_loop_timed_out_guess(conn) != 0)
            {
                removeConnectionFromHashTable_guess(&rdp->hash, conn);
            }
            else
            {
                if (conn->connected == 0 && conn->field_788 == 0)
                {
                    node = rdp_arrival_fast_malloc_guess(0x34);
                    rdp_arrival_init_node_guess(node, (int)conn);
                    if (node[2] != 0 && *(int *)(node[2] + 0x4c) != 0)
                    {
                        rdp_fast_free_guess(node);
                    }
                    else
                    {
                        EnterCriticalSection((LPCRITICAL_SECTION)((char *)rdp + 0xbc));
                        was_empty = rdp_list_head_item_guess((int *)((char *)rdp + 0xa4));
                        node[8] = 0;
                        rdp_list_add_tail_item_guess((int *)((char *)rdp + 0xa4), node);
                        if (was_empty == 0)
                        {
                            rdp_signal_message_processing_guess((HANDLE *)((char *)rdp + 0xa0));
                        }
                        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0xbc));
                    }
                    conn->field_788 = 1;
                }
                EnterCriticalSection((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
                if (memcmp(&conn->field_030, status, 8) != 0)
                {
                    memcpy(&conn->field_030, status, 8);
                    rdp_arrival_update_heap_guess((int *)((char *)rdp + 0x58), (u32 *)conn->unknown01C);
                }
                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
            }
            owner = conn->rdp;
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)conn->lock);
            removed = removeConnectionFromHashTable_guess(&owner->hash, conn);
            if (removed != 0)
            {
                EnterCriticalSection((LPCRITICAL_SECTION)((char *)owner + 0x6c));
                rdp_datagram_remove_heap_guess((int *)((char *)owner + 0x58), (u32 *)removed->unknown01C);
                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)owner + 0x6c));
                rdp_cleanup_connection_resources_guess(removed);
                rdp_cleanup_free_guess(removed);
            }
        }
    }
    rdp_create_cleanup_guess(rdp);
}

/* Callees of rdp_state_init_guess, by address. 0x1002FAE0 and 0x1002DEA0 are unnamed (see unit_1002FAE0.cpp and
 * unit_1002DE90.cpp); the names are guesses from how this function uses them. */
#define rdp_init_queue_guess            EQ_CDECL(void, 0x1002FAE0, (int obj))
#define rdp_init_record_guess           EQ_CDECL(void, 0x1002DEA0, (u32 *rec, u32 a, u32 b, u32 c))
/* 0x100400A8: the IAT slot of kernel32 InitializeCriticalSection (as connection.c's initializeCriticalSection_import_guess). */
#define rdp_init_critical_section_guess EQ_GLOBAL(rdp_critical_section_proc_guess, 0x100400A8)

// FUNCTION: EQMAIN 0x1002a360
// Prepare the client's RDP state for network traffic.
// __cdecl, one argument (the caller pops it). Sets up the rdp state block: zeroes and -1 sentinels, then the
// sub-objects at +0x50 (counter), +0x90 and +0xa0 (events), +0xa4 and +0xd4 (records), +0xbc, +0xec and +0x120
// (critical sections), the block at +0x140, and the tick count at +0x10c.
// Dword indexes as in the Ghidra listing: rdp[N] is at +N*4 (rdp[0x4f] = +0x13c). Raw offsets: the trilogy
// RdpState_guess names none of these fields.
void rdp_state_init_guess(u32 *rdp)
{
    rdp_critical_section_proc_guess init_cs;

    rdp[0] = 0;
    rdp[1] = 0xffffffff;
    rdp[4] = 0xffffffff;
    rdp[2] = 0xffffffff;
    rdp[3] = 0xffffffff;
    memset(rdp + 5, 0, 4 * sizeof(u32));
    memset(rdp + 9, 0, 4 * sizeof(u32));
    rdp[0x4f] = 0xffffffff;
    rdp[0x11] = 0;
    rdp[0x13] = 0;
    rdp[0x12] = 0;
    rdp[0x4e] = 0;
    rdp[0x42] = 0;
    rdp[0x41] = 0;
    rdp[0x45] = 0;
    rdp_init_queue_guess((int)(rdp + 0x50));
    rdp_init_counter_guess(rdp + 0x14);
    memset(rdp + 0x16, 0, 11 * sizeof(u32));
    rdp[0x21] = 0;
    uevent_init_guess((HANDLE *)(rdp + 0x24));
    uevent_init_guess((HANDLE *)(rdp + 0x28));
    rdp_init_record_guess(rdp + 0x29, 0, 0, 0);
    init_cs = rdp_init_critical_section_guess;
    init_cs((LPCRITICAL_SECTION)(rdp + 0x2f));
    rdp_init_record_guess(rdp + 0x35, 0, 0, 0);
    init_cs((LPCRITICAL_SECTION)(rdp + 0x3b));
    init_cs((LPCRITICAL_SECTION)(rdp + 0x48));
    rdp[0x43] = rdp_timeGetTime_guess();
}

/* 0x1002a460 callees at this image's addresses (WSOCK32/WINMM thunks and the functions at 0x1002a360, 0x1002fb30). */
#define rdp_create_eq_state_init     EQ_CDECL(void, 0x1002A360, (void *rdp))
#define rdp_create_eq_wsastartup     EQ_STDCALL(int, 0x100322FC, (WORD version, LPWSADATA data))
#define rdp_create_eq_critical_init  EQ_CDECL(int, 0x1002FB30, (LPCRITICAL_SECTION cs, DWORD id))
#define rdp_create_eq_getprotobyname EQ_STDCALL(struct protoent *, 0x100322F6, (const char *name))
#define rdp_create_eq_socket         EQ_STDCALL(SOCKET, 0x100322F0, (int af, int type, int protocol))
#define rdp_create_eq_bind           EQ_STDCALL(int, 0x100322EA, (SOCKET s, struct sockaddr *addr, int len))
#define rdp_create_eq_getsockname    EQ_STDCALL(int, 0x100322DE, (SOCKET s, struct sockaddr *addr, int *len))
#define rdp_create_eq_getsockopt     EQ_STDCALL(int, 0x100322A2, (SOCKET s, int level, int option, char *value, int *len))
#define rdp_create_eq_closesocket    EQ_STDCALL(int, 0x100322C6, (SOCKET s))

// FUNCTION: EQMAIN 0x1002a460
// Starts the rdp network layer: UDP socket on localPort, raw ICMP and optional IPX sockets, its network thread
// __cdecl, four arguments (the caller pops them). Allocates the 0x1d8-byte rdp state block, starts Winsock, sets up the
// queues and the sockets (+0x4 UDP, +0x8 raw ICMP, +0xc UDP, +0x10 IPv6 when param_4 asks for it), then starts the
// worker thread. Success: 0, with the block in *rdpOut. Failure: after the cleanup, the error code or 1. A failed
// malloc hands the cleanup a NULL block, which faults (as in the original).
int rdp_create_guess(void **rdpOut, u_short localPort, int connCapacity, u32 param_4)
{
    struct sockaddr *bindAddr;
    DWORD DVar2;
    DWORD DVar5;
    u_short netPort;
    int *rdpState;
    struct protoent *protoEntry;
    SOCKET SVar8;
    u32 lastError;
    int tmp;
    int iVar11;
    int addrLen;
    char broadcastOpt[4];
    int udpProto;
    struct sockaddr tempAddr;
    int optLen;
    WSADATA wsaData;

    *rdpOut = 0;
    rdpState = rdp_create_malloc_guess(0x1d8);
    if (rdpState == 0)
    {
        iVar11 = 2;
        goto fail;
    }
    rdp_create_eq_state_init(rdpState);
    iVar11 = rdp_create_eq_wsastartup(0x101, &wsaData);
    if (iVar11 != 0)
        goto failGeneric;
    *rdpState = 1;
    DVar2 = EQ_GLOBAL(u32, 0x10048A48);
    (*(u16 *)&EQ_GLOBAL(u32, 0x10048A48))++;
    iVar11 = rdp_create_eq_critical_init((LPCRITICAL_SECTION)(rdpState + 0x50), DVar2);
    if (iVar11 != 0)
        goto fail;
    DVar5 = rdp_timeGetTime_guess();
    rdp_create_semaphore_decrement_guess(rdpState + 0x50, DVar5);
    iVar11 = rdp_create_hash_init_guess(rdpState + 0x14, (-(connCapacity != 1) & 0xbU) + 1);
    if (iVar11 != 0)
        goto fail;
    iVar11 = rdp_create_buffer_init_guess(rdpState + 0x16, connCapacity * 2);
    if (iVar11 != 0)
        goto fail;
    iVar11 = rdp_create_semaphore_guess((HANDLE *)(rdpState + 0x28));
    if (iVar11 != 0)
        goto fail;
    protoEntry = rdp_create_eq_getprotobyname(&EQ_GLOBAL(char, 0x10048ACC));
    if (protoEntry == 0)
        udpProto = 0x11;
    else
        udpProto = (int)protoEntry->p_proto;
    if ((param_4 & 2) != 0 || (param_4 & 1) != 0)
    {
        SVar8 = rdp_create_eq_socket(2, 2, udpProto);
        rdpState[1] = SVar8;
    }
    if ((param_4 & 2) != 0 && rdpState[1] == -1)
    {
        iVar11 = 10;
        goto fail;
    }
    if ((param_4 & 4) != 0 || (param_4 & 1) != 0)
    {
        SVar8 = rdp_create_eq_socket(6, 2, 1000);
        rdpState[4] = SVar8;
    }
    if ((param_4 & 4) != 0 && rdpState[4] == -1)
    {
        iVar11 = 10;
        goto fail;
    }
    if (rdpState[1] == -1)
        goto setupIpx;
    bindAddr = (struct sockaddr *)(rdpState + 5);
    bindAddr->sa_family = 0;
    bindAddr->sa_data[0] = '\0';
    bindAddr->sa_data[1] = '\0';
    rdpState[6] = 0;
    rdpState[7] = 0;
    rdpState[8] = 0;
    bindAddr->sa_family = 2;
    netPort = rdp_create_htons_guess(localPort);
    *(u_short *)((int)rdpState + 0x16) = netPort;
    iVar11 = rdp_create_eq_bind(rdpState[1], bindAddr, 0x10);
    if (iVar11 == -1)
    {
        iVar11 = rdp_create_get_last_error_guess();
        iVar11 = (-(u32)(iVar11 != 0x2740) & 0xfffffff6) + 0xb;
        goto fail;
    }
    broadcastOpt[0] = '\x01';
    broadcastOpt[1] = '\0';
    broadcastOpt[2] = '\0';
    broadcastOpt[3] = '\0';
    iVar11 = rdp_create_setsockopt_guess(rdpState[1], 0xffff, 0x20, broadcastOpt, 4);
    rdpState[0x13] = (u32)(iVar11 == 0);
    addrLen = 0x10;
    iVar11 = rdp_create_eq_getsockname(rdpState[1], bindAddr, &addrLen);
    if (iVar11 == -1)
        goto failGeneric;
    iVar11 = rdp_create_nonblocking_guess(rdpState[1]);
    if (iVar11 != 0)
        goto fail;
    protoEntry = rdp_create_eq_getprotobyname(&EQ_GLOBAL(char, 0x10048AC4));
    if (protoEntry == 0)
        iVar11 = 1;
    else
        iVar11 = (int)protoEntry->p_proto;
    SVar8 = rdp_create_eq_socket(2, 3, iVar11);
    rdpState[2] = SVar8;
    if (SVar8 == 0xffffffff)
    {
        lastError = rdp_create_get_last_error_guess();
        rdp_create_net_strerror_guess(lastError);
        goto setupIpx;
    }
    iVar11 = rdp_create_nonblocking_guess(SVar8);
    if (iVar11 != 0)
        goto fail;
    *(int *)&tempAddr = 0;
    *(int *)&tempAddr.sa_data[2] = 0;
    *(int *)&tempAddr.sa_data[6] = 0;
    *(int *)&tempAddr.sa_data[10] = 0;
    tempAddr.sa_family = 2;
    tempAddr.sa_data[0] = 0;
    tempAddr.sa_data[1] = 0;
    iVar11 = rdp_create_eq_bind(rdpState[2], &tempAddr, 0x10);
    if (iVar11 == -1)
        goto failGeneric;
    SVar8 = rdp_create_eq_socket(2, 2, udpProto);
    rdpState[3] = SVar8;
    if (SVar8 == 0xffffffff)
        goto failGeneric;
    bindAddr = (struct sockaddr *)(rdpState + 9);
    bindAddr->sa_family = 0;
    bindAddr->sa_data[0] = '\0';
    bindAddr->sa_data[1] = '\0';
    rdpState[10] = 0;
    rdpState[0xb] = 0;
    rdpState[0xc] = 0;
    SVar8 = rdpState[3];
    bindAddr->sa_family = 2;
    iVar11 = rdp_create_eq_bind(SVar8, bindAddr, 0x10);
    if (iVar11 == -1)
        goto failGeneric;
    addrLen = 0x10;
    iVar11 = rdp_create_eq_getsockname(rdpState[3], bindAddr, &addrLen);
    if (iVar11 == -1)
        goto failGeneric;
    iVar11 = rdp_create_nonblocking_guess(rdpState[3]);
    if (iVar11 != 0)
        goto fail;
    optLen = 4;
    iVar11 = rdp_create_eq_getsockopt(rdpState[3], 0, 7, (char *)(rdpState + 0x11), &optLen);
    if (iVar11 == 0)
        goto setTtl;
    iVar11 = rdp_create_get_last_error_guess();
    if (iVar11 != 0x273a)
        goto failWithError;
    rdp_create_eq_closesocket(rdpState[3]);
    rdpState[3] = -1;
setTtl:
    if (rdpState[3] == -1)
        goto setupIpx;
    iVar11 = rdp_create_setsockopt_guess(rdpState[3], 0, 7, (char *)(rdpState + 0x11), 4);
    if (iVar11 == 0)
        goto setupIpx;
    iVar11 = rdp_create_get_last_error_guess();
    if (iVar11 != 0x273a)
        goto failWithError;
    rdp_create_eq_closesocket(rdpState[3]);
    rdpState[3] = -1;
setupIpx:
    if (rdpState[4] == -1)
        goto startThread;
    *(int *)&tempAddr = 0;
    *(int *)&tempAddr.sa_data[10] = 0;
    tempAddr.sa_family = 6;
    *(int *)&tempAddr.sa_data[6] = 0;
    tempAddr.sa_data[10] = 0;
    tempAddr.sa_data[11] = 0;
    *(u16 *)&tempAddr.sa_data[10] = localPort;
    iVar11 = rdp_create_eq_bind(rdpState[4], &tempAddr, 0xe);
    if (iVar11 == -1)
    {
        iVar11 = rdp_create_get_last_error_guess();
        iVar11 = (-(u32)(iVar11 != 0x2740) & 0xfffffff6) + 0xb;
        goto fail;
    }
    iVar11 = 1;
    broadcastOpt[0] = '\x01';
    broadcastOpt[1] = '\0';
    broadcastOpt[2] = '\0';
    broadcastOpt[3] = '\0';
    tmp = rdp_create_setsockopt_guess(rdpState[4], 0xffff, 0x20, broadcastOpt, 4);
    rdpState[0x12] = (u32)(tmp == 0);
    addrLen = 0xe;
    tmp = rdp_create_eq_getsockname(rdpState[4], (struct sockaddr *)(rdpState + 0xd), &addrLen);
    if (tmp == -1)
        goto fail;
    iVar11 = rdp_create_nonblocking_guess(rdpState[4]);
    if (iVar11 != 0)
        goto fail;
startThread:
    rdpState[0x47] = (param_4 >> 0x1e) & 1;
    rdpState[0x46] = param_4 >> 0x1f;
    rdpState[0x23] = 1;
    iVar11 = rdp_create_thread_guess(rdpState + 0x24, (void *)0x10029c50, rdpState);
    if (iVar11 == 0)
    {
        *rdpOut = rdpState;
        return 0;
    }
    rdpState[0x23] = 0;
    goto fail;
failWithError:
    lastError = rdp_create_get_last_error_guess();
    rdp_create_net_strerror_guess(lastError);
failGeneric:
    iVar11 = 1;
fail:
    rdpState[0x22] = 0;
    rdp_create_cleanup_guess(rdpState);
    return iVar11;
}

#undef rdp_create_eq_state_init
#undef rdp_create_eq_wsastartup
#undef rdp_create_eq_critical_init
#undef rdp_create_eq_getprotobyname
#undef rdp_create_eq_socket
#undef rdp_create_eq_bind
#undef rdp_create_eq_getsockname
#undef rdp_create_eq_getsockopt
#undef rdp_create_eq_closesocket

// FUNCTION: EQMAIN 0x1002a950
// Stops an active RDP instance, optionally waits for its worker, then frees its state.
void rdp_destroy_guess(int networkBase, int freeInstance)
{
    int stateBase;
    int releaseInstance;
    int zero;
    SOCKET s;
    char *to;
    int tolen;

    if (*(int *)(networkBase + 0x8c) != 1)
    {
        return;
    }
    stateBase = networkBase;
    releaseInstance = freeInstance;
    zero = 0;
    if (releaseInstance != zero)
        *(int *)(stateBase + 0x88) = 1;
    else
        *(int *)(stateBase + 0x88) = zero;
    networkBase = 1;
    *(int *)(stateBase + 0x8c) = zero;
    if (*(SOCKET *)(stateBase + 4) != (SOCKET)-1)
    {
        struct sockaddr destinationAddress;
        destinationAddress = *(struct sockaddr *)(stateBase + 0x14);
        *(u32 *)((char *)&destinationAddress + 4) = EQ_STDCALL(u32, 0x10032296, (u32))(0x7f000001);
        s = *(SOCKET *)(stateBase + 4);
        to = (char *)&destinationAddress;
        tolen = 0x10;
        EQ_STDCALL(int, 0x10032290, (SOCKET, char *, int, int, char *, int))(s, (char *)&networkBase,
                                4, zero, to, tolen);
    }
    else
    {
        s = *(SOCKET *)(stateBase + 0x10);
        if (s != (SOCKET)-1)
        {
            to = (char *)(stateBase + 0x34);
            tolen = 0xe;
            EQ_STDCALL(int, 0x10032290, (SOCKET, char *, int, int, char *, int))(s, (char *)&networkBase,
                                    4, zero, to, tolen);
        }
    }
    if (releaseInstance != zero)
    {
        EQ_CDECL(void, 0x100304A0, (void *, DWORD *))((void *)(stateBase + 0x90), (DWORD *)&networkBase);
        EQ_CDECL(void, 0x1002E890, (void *))((void *)(stateBase + 0x90));
        EQ_CDECL(void, 0x10033568, (void *))((void *)stateBase);
    }
    return;
}

// FUNCTION: EQMAIN 0x1002aa20
// Formats an address as readable IP, IPX, or COM port text for diagnostics.
// Cloned from trilogy format_sockaddr_guess 0x004F1230 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Writes "AF_INET a.b.c.d:port", "AF_COMPORT n", "AF_IPX %s:%s:%u" or "unknown address family (n)".
// Quirk kept: the two %s of the IPX text are never filled in (uninitialised locals).
void format_sockaddr_guess(char *out, const RdpAddr_guess *addr)
{
    char ipx_a[16];
    char ipx_b[16];
    u32 port;

    switch (addr->family)
    {
    case 2:
        port = rdp_ntohs_guess(addr->port);
        sprintf(out, "AF_INET %s:%u", rdp_inet_ntoa_guess(addr->addr), port);
        return;
    case 6:
        sprintf(out, "AF_IPX %s:%s:%u", ipx_a, ipx_b, *(u16 *)&addr->rest[4]);
        return;
    case 0x45:
        sprintf(out, "AF_COMPORT %u", (int)(short)addr->port);
        return;
    default:
        sprintf(out, "unknown address family (%u)", addr->family);
        return;
    }
}

// FUNCTION: EQMAIN 0x1002aad0
// Adds a newly initialized remote peer to RDP tracking for packet exchange.
// @early-stop: malloc and resource failures take different epilogues, and direct sockaddr fields use ESI displacements instead of the original EBP base.
// Same steps as trilogy createNetworkConnection_guess 0x004F12E0, plus eqmain's connection count: returns 0xd
// while rdp+0x138 is at or above rdp+0x13c, and counts the connection under a second lock (rdp+0x120).
// __cdecl. Allocates and initializes a connection, registering it with the RDP heap and hash on success.
int createNetworkConnection_guess(RdpState_guess *rdp, RdpConnection_guess **out, const RdpAddr_guess *addr, u32 flags)
{
    RdpConnection_guess *conn;
    char textBuffer[16];
    char textBuffer2[16];
    char formattedAddress[64];
    /* fake match: spill the result into the incoming rdp slot as the original does. */
    volatile int rc;
    int status;
    u32 family;
    u16 network_port;
    u32 port;

    if (rdp->connectionCount_guess >= rdp->maxConnections_guess)
    {
        return 0xd;
    }
    if ((flags & 0xfffffffe) != 0)
    {
        return 6;
    }

    conn = (RdpConnection_guess *)rdp_create_malloc_guess(0x740);
    if (conn == 0)
    {
        rc = 2;
    }
    else
    {
        EQ_CDECL(void, 0x1002D180, (RdpConnection_guess *, RdpState_guess *, const RdpAddr_guess *, u32))
            (conn, rdp, addr, flags);
        status = EQ_CDECL(int, 0x1002D1F0, (RdpConnection_guess *))(conn);
        rc = status;
        if (status == 0)
        {
            family = conn->remote.family;
            switch (family)
            {
            case 2:
                network_port = rdp_ntohs_guess(conn->remote.port);
                port = network_port;
                EQ_CDECL(int, 0x10034117, (char *, const char *, ...))
                    (formattedAddress, (const char *)EQ_ARRAY(char, 0x10048A78), rdp_inet_ntoa_guess(conn->remote.addr), port);
                break;
            case 6:
                EQ_CDECL(int, 0x10034117, (char *, const char *, ...))
                    (formattedAddress, (const char *)EQ_ARRAY(char, 0x10048A88), textBuffer2, textBuffer, *(u16 *)&conn->remote.rest[4]);
                break;
            case 0x45:
                EQ_CDECL(int, 0x10034117, (char *, const char *, ...))
                    (formattedAddress, (const char *)EQ_ARRAY(char, 0x10048A98), (int)(short)conn->remote.port);
                break;
            default:
                EQ_CDECL(int, 0x10034117, (char *, const char *, ...))
                    (formattedAddress, (const char *)EQ_ARRAY(char, 0x10048AA8), family);
                break;
            }
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
            EQ_CDECL(int, 0x1002E260, (RdpHeap_guess *, RdpHeapElem_guess *))
                ((RdpHeap_guess *)((char *)rdp + 0x58), (RdpHeapElem_guess *)((char *)conn + 0x1c));
            EQ_CDECL(void, 0x1002E770, (RdpConnHash_guess *, RdpConnection_guess *))(&rdp->hash, conn);
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
            EQ_CDECL(RdpConnection_guess *, 0x1002E6E0, (RdpConnHash_guess *, RdpAddr_guess *))
                (&conn->rdp->hash, &conn->remote);
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x120));
            rdp->connectionCount_guess = rdp->connectionCount_guess + 1;
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x120));
            status = rc;
            *out = conn;
            return status;
        }
    }

    if (conn != 0)
    {
        EQ_CDECL(void, 0x1002D210, (RdpConnection_guess *))(conn);
        EQ_CDECL(void, 0x10033568, (void *))(conn);
    }
    return rc;
}

/* 0x10032308: the WSOCK32 inet_addr import thunk, called directly. */
#define rdp_inet_addr_guess             EQ_STDCALL(u32, 0x10032308, (char *cp))
/* 0x10032302: the WSOCK32 gethostbyname import thunk, called directly. */
#define rdp_gethostbyname_guess         EQ_STDCALL(struct hostent *, 0x10032302, (char *name))
/* 0x1002E310: removeHeapElement_guess (heap.c; not rewritten in this file). */
#define rdp_remove_heap_element_guess   EQ_CDECL(void, 0x1002E310, (RdpHeap_guess *heap, RdpHeapElem_guess *elem))
/* 0x1002AAD0: createNetworkConnection_guess (above). */
#define rdp_create_network_connection_guess \
    EQ_CDECL(int, 0x1002AAD0, (RdpState_guess *rdp, RdpConnection_guess **out, const RdpAddr_guess *addr, u32 flags))

// FUNCTION: EQMAIN 0x1002AC80
// Resolves a server address and creates a new RDP peer when that address is not registered.
// @early-stop: Prologue saves, stack slots, counter addressing, and register choices still differ; best similarity 0.6526.
// __cdecl. Resolves name (a dotted address, else the first address of its host name) and port to a sockaddr and
// looks it up in the hash. A connection already there is taken out (its lock left, its heap element and memory
// freed) and 0xb returned. Otherwise the stats counter at +0x150 is bumped and the connection is created by
// createNetworkConnection_guess; on success it is marked (+0x54), taken out of the hash again, and *out is set.
// 0xc when the host name does not resolve.
int createNetworkConnectionByName_guess(RdpState_guess *rdpState, RdpConnection_guess **connOut, char *hostName, int param_4, u32 connFlags)
{
    u_short netPort;
    struct hostent *hostEntry;
    RdpAddr_guess peerAddr = { 0 };

    peerAddr.family = 2;
    netPort = rdp_create_htons_guess((u_short)param_4);
    peerAddr.port = netPort;
    peerAddr.addr = rdp_inet_addr_guess(hostName);
    if (peerAddr.addr == 0xffffffff)
    {
        hostEntry = rdp_gethostbyname_guess(hostName);
        if (hostEntry == 0)
        {
            return 0xc;
        }
        peerAddr.addr = *(u32 *)*hostEntry->h_addr_list;
    }
    {
    int ownerRdp;
    int connection;
    void *released;
    int result;

    connection = EQ_CDECL(int, 0x1002E6E0, (short *, short *))((short *)&rdpState->hash, (short *)&peerAddr);
    param_4 = connection;
    if (connection != 0)
    {
        ownerRdp = *(int *)(connection + 0xc);
        result = 0xb;
        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(connection + 0x34));
        released = (void *)removeConnectionFromHashTable_guess((RdpConnHash_guess *)(ownerRdp + 0x50), (RdpConnection_guess *)connection);
        if (released != 0)
        {
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(ownerRdp + 0x6c));
            rdp_remove_heap_element_guess((RdpHeap_guess *)(ownerRdp + 0x58), (RdpHeapElem_guess *)((char *)released + 0x1c));
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(ownerRdp + 0x6c));
            rdp_cleanup_connection_resources_guess((RdpConnection_guess *)released);
            rdp_create_connection_free_guess(released);
            return result;
        }
    }
    else
    {
        RdpU64_guess *stat;

        stat = &RDP_STAT_guess(0x150);
        ++*stat;
        result = rdp_create_network_connection_guess(rdpState, (RdpConnection_guess **)&param_4,
                                                    &peerAddr, connFlags);
        connection = param_4;
        if (result == 0)
        {
            *(u32 *)(param_4 + 0x54) = 1;
            ownerRdp = *(int *)(param_4 + 0xc);
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(param_4 + 0x34));
            released = (void *)removeConnectionFromHashTable_guess((RdpConnHash_guess *)(ownerRdp + 0x50),
                                                                  (RdpConnection_guess *)connection);
            if (released != 0)
            {
                rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(ownerRdp + 0x6c));
                rdp_remove_heap_element_guess((RdpHeap_guess *)(ownerRdp + 0x58),
                                               (RdpHeapElem_guess *)((char *)released + 0x1c));
                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(ownerRdp + 0x6c));
                rdp_cleanup_connection_resources_guess((RdpConnection_guess *)released);
                rdp_create_connection_free_guess(released);
            }
            *connOut = (RdpConnection_guess *)param_4;
        }
    }
    return result;
    }
}

// FUNCTION: EQMAIN 0x1002ae10
// Forwards a connection to the RDP hash-table helper; its exact effect is unclear here.
// Cloned from trilogy close_connection_and_cleanup_guess 0x004F1580 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Takes the connection out of the instance's hash table.
void close_connection_and_cleanup_guess(RdpState_guess *rdp, RdpConnection_guess *conn)
{
    removeConnectionFromHashTable_guess(&rdp->hash, conn);
}

// FUNCTION: EQMAIN 0x1002ae30
// Delivers the next queued incoming RDP item, waiting for traffic when the queue is empty.
// @early-stop: prologue and first section differ (original keeps timeout in EBP and spills result; rewrite keeps result in EBP), and the tail block order differs; similarity 0.8465.
// Cloned from trilogy rdp_receive_guess 0x004F1600 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Returns the next item on the rdp's receive list (+0xd4). When the list is empty it drains the serial link
// (+0x140) unless +0x15C is -1, then waits up to timeout ms on the event at +0xa0 (-1 waits forever, 0 does not wait).
// On a signal it moves the pending list (+0xa4) over and takes its head. 0 when nothing arrives or rdp is null.
int rdp_receive_guess(RdpState_guess *rdp, int timeout)
{
    int result;
    int remaining;
    DWORD deadline;
    struct sockaddr from;
    u32 buf[0x86];
    rdp_critical_section_proc_guess critical_proc;
    rdp_critical_section_proc_guess leave_proc;

    result = 0;
    if (rdp != 0)
    {
        critical_proc = rdp_enter_critical_section_guess;
        critical_proc((LPCRITICAL_SECTION)((u8 *)rdp + 0xec));
        result = (int)rdp_receive_list_remove_head_guess((RdpList_guess *)((u8 *)rdp + 0xd4));
        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((u8 *)rdp + 0xec));
        if (result == 0)
        {
            remaining = timeout;
            if ((timeout != 0) && (timeout != -1))
            {
                deadline = rdp_timeGetTime_guess() + timeout;
            }
            do
            {
                do
                {
                    /* rdp+0x15C is the serial queue's unnamed +0x1C; -1 skips the serial drain */
                    if (*(int *)((u8 *)rdp + 0x15c) != -1)
                    {
                        int received;

                        do
                        {
                            received = rdp_receive_serial_recv_from_guess(&rdp->serial, buf, 0x218, &from);
                            rdp_receive_process_datagram_guess(rdp, buf, received, &from);
                        } while (received != -1);
                        rdp_receive_usemaphore_decrement_guess(&rdp->serial, rdp_timeGetTime_guess() + 100);
                    }
                    if (rdp_receive_wait_for_event_guess((HANDLE *)((u8 *)rdp + 0xa0), remaining) == 0)
                    {
                        return result;
                    }
                    critical_proc((LPCRITICAL_SECTION)((u8 *)rdp + 0xbc));
                    critical_proc((LPCRITICAL_SECTION)((u8 *)rdp + 0xec));
                    *(RdpList_guess *)((u8 *)rdp + 0xd4) = *(RdpList_guess *)((u8 *)rdp + 0xa4);
                    memset((u8 *)rdp + 0xa4, 0, sizeof(RdpList_guess));
                    result = (int)rdp_receive_list_remove_head_guess((RdpList_guess *)((u8 *)rdp + 0xd4));
                    leave_proc = rdp_leave_critical_section_guess;
                    leave_proc((LPCRITICAL_SECTION)((u8 *)rdp + 0xec));
                    leave_proc((LPCRITICAL_SECTION)((u8 *)rdp + 0xbc));
                    if ((result != 0) || (timeout == 0))
                    {
                        return result;
                    }
                    critical_proc = rdp_enter_critical_section_guess;
                } while (timeout == -1);
                remaining = (int)(deadline - rdp_timeGetTime_guess());
            } while (remaining > 0);
        }
    }
    return result;
}

// FUNCTION: EQMAIN 0x1002afc0
// Forwards the serial queue to a helper and returns that helper's status.
// Cloned from trilogy check_connection_status_guess 0x004F17A0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Whether the serial queue holds fewer than two items.
int check_connection_status_guess(RdpState_guess *rdp)
{
    return is_connection_inactive_guess(&rdp->serial);
}

// FUNCTION: EQMAIN 0x1002afe0
// Returns a value calculated from the RDP serial queue; its exact meaning is unclear here.
// Cloned from trilogy getTimeFromEQClient_guess 0x004F17C0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. When the serial queue will have drained (timeGetTime ms).
DWORD getTimeFromEQClient_guess(RdpState_guess *rdp)
{
    return calculateAdjustedClientTime_guess(&rdp->serial);
}

// FUNCTION: EQMAIN 0x1002cfc0
// Initializes a connection's 0x5a0-byte buffer and timing fields; their purpose is unclear.
int NetworkConnection_initializeRDPConnection_guess(void *connectionObject)
{
    int result = 0;
    u8 *connection = (u8 *)connectionObject;
    RdpState_guess *rdp;
    DWORD now;
    void *buffer;

    if (connectionObject == 0)
        return 6;

    rdp = *(RdpState_guess **)(connection + 0x0c);
    if (rdp->socket == (u32)-1)
        return 8;

    EQ_CDECL(RdpConnection_guess *, 0x1002E6E0,
             (RdpConnHash_guess *, RdpAddr_guess *))
        (&rdp->hash, (RdpAddr_guess *)(connection + 0x478));

    if ((connection[0x5c] & 2) != 0 &&
        rdp_timeGetTime_guess() - *(u32 *)(connection + 0x724) < *(u32 *)(connection + 0x6e8))
    {
        result = 9;
    }
    else if (*(u32 *)(connection + 0x710) == (u32)-1)
    {
        result = 8;
    }
    else
    {
        if (*(u32 *)(connection + 0x718) != 0)
        {
            if (*(u32 *)(connection + 0x468) == 0)
            {
                buffer = EQ_CDECL(void *, 0x1003342E, (u32))(*(u32 *)(connection + 0x738) << 4);
                *(void **)(connection + 0x468) = buffer;
                if (buffer == 0)
                {
                    result = 2;
                }
                else
                {
                    memcpy(buffer, *(void **)(connection + 0x718), *(u32 *)(connection + 0x738) << 4);
                    *(u32 *)(connection + 0x46c) = *(u32 *)(connection + 0x738);
                    *(u32 *)(connection + 0x470) = *(u32 *)(connection + 0x728);
                }
            }
            else
            {
                EQ_CDECL(void, 0x10033568, (void *))(*(void **)(connection + 0x718));
                *(u32 *)(connection + 0x718) = 0;
            }
        }

        if (result == 0)
        {
            if (*(u32 *)(connection + 0x718) == 0)
            {
                buffer = EQ_CDECL(void *, 0x1003342E, (u32))(0x5a0);
                *(void **)(connection + 0x718) = buffer;
                if (buffer == 0)
                {
                    result = 2;
                }
            }

            if (result == 0)
            {
                memset(*(void **)(connection + 0x718), 0, 0x5a0);
                *(u32 *)(connection + 0x71c) = 0;
                now = rdp_timeGetTime_guess();
                *(DWORD *)(connection + 0x724) = now;
                *(DWORD *)(connection + 0x720) = now;
                EQ_CDECL(long, 0x10035B24, (long *))((long *)(connection + 0x728));
                *(u32 *)(connection + 0x72c) = 1;
                *(u32 *)(connection + 0x730) = 0x1e;
                *(u32 *)(connection + 0x734) = 0;
                *(u32 *)(connection + 0x738) = 0;
                *(u32 *)(connection + 0x5c) |= 2;
            }
        }
    }
    EQ_CDECL(void, 0x10028BF0, (void *))(connectionObject);
    return result;
}

// FUNCTION: EQMAIN 0x1002fa00
// Delays processing for the requested number of milliseconds.
// Cloned from trilogy sleep_milliseconds_guess 0x004F66E0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl.
void sleep_milliseconds_guess(DWORD ms)
{
    Sleep(ms);
}

// FUNCTION: EQMAIN 0x100304a0
// Waits for a worker thread to finish and records its exit code.
// Cloned from trilogy waitForThreadAndGetExitCode_guess 0x004F71F0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Waits for the thread to end and stores its exit code (-1 when there is no thread). Returns 0;
// GetExitCodeThread's result - 1 when that isn't 0 (failure: -1, after an unused GetLastError); -1 when
// the stored code is STILL_ACTIVE (0x103).
int waitForThreadAndGetExitCode_guess(HANDLE *thread, DWORD *code)
{
    DWORD exit_code;
    {
        u32 rc;
    
        if (0 == *thread)
        {
            rc = 0;
            *code = (DWORD)-1;
        }
        else
        {
            WaitForSingleObject(*thread, INFINITE);
            rc = GetExitCodeThread(*thread, &exit_code) - 1;
            *code = exit_code;
            if (0 == rc)
            {
            }
            else
            {
                GetLastError();
                return rc;
            }
        }
        if (*code == STILL_ACTIVE)
            return -1;
        return rc;
    }
}

// FUNCTION: EQMAIN 0x10030530
// Starts a worker thread with the supplied entry point and context.
// Cloned from trilogy createThreadWithPriority_guess 0x004F7290 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Starts a thread that runs func(arg) (the client's thread stub 0x10030510), at THREAD_PRIORITY_HIGHEST
// (2). thread[0] = handle, thread[1] = id, thread[2] = func, thread[3] = arg. Returns 1 when the thread could not
// be created.
int createThreadWithPriority_guess(u32 *thread, u32 func, u32 arg)
{
    HANDLE h;

    thread[2] = func;
    thread[3] = arg;
    h = (HANDLE)_beginthreadex(0, 0, (unsigned (__stdcall *)(void *))0x10030510, thread, 0,
                               (unsigned *)&thread[1]);
    thread[0] = (u32)h;
    if (h != 0)
        SetThreadPriority(h, 2);
    return thread[0] == 0;
}

// FUNCTION: EQMAIN 0x100309d0
// Enables nonblocking I/O on a socket for RDP traffic.
// Cloned from trilogy set_socket_non_blocking_guess 0x004F7730 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. FIONBIO on; 1 when ioctlsocket fails, else 0.
int set_socket_non_blocking_guess(u32 s)
{
    u32 on;

    on = 1;
    return rdp_ioctlsocket_guess(s, 0x8004667e, &on) != 0;
}
