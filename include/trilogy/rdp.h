/*
 * rdp.h - the RDP network library linked into eqgame.exe (Mac map: Source/PC Source/Network/rdp/*.c).
 *
 * Plain C. Layouts are from the client's code (eqgame.exe 2001-08-21) unless noted; most structs are
 * only partly known, unknown bytes are "unknownXXXX" after their offset.
 * Not included from trilogy.h (yet): see the Phase 1d rdp notes.
 */
#ifndef TRILOGY_RDP_H
#define TRILOGY_RDP_H

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

/* bitops.c: a 2048-bit window, MSB first within each byte. Verified against client 0x004F7C70. */
typedef struct RdpBitArray_guess
{
    u8  bits[256];          /* 0x000 */
    u32 count;              /* 0x100  cleared with the bits */
} RdpBitArray_guess;
C_ASSERT(sizeof(RdpBitArray_guess) == 0x104);

/* list.c: compare(existing link's key, other key) -> <0, 0, >0. __cdecl. */
typedef int (__cdecl *RdpListCompare_guess)(void *a, void *b);

/* list.c: one link. Verified against client 0x004F5480..0x004F5670. */
typedef struct RdpLink_guess
{
    struct RdpLink_guess *next;     /* 0x00 */
    struct RdpLink_guess *prev;     /* 0x04 */
    void *data;                     /* 0x08  returned by the remove functions */
    void *key;                      /* 0x0C  passed to the list's compare */
} RdpLink_guess;
C_ASSERT(sizeof(RdpLink_guess) == 0x10);

/* list.c: a doubly linked list. Verified against client 0x004F5440..0x004F5670. */
typedef struct RdpList_guess
{
    RdpLink_guess *head;            /* 0x00 */
    RdpLink_guess *tail;            /* 0x04 */
    int count;                      /* 0x08 */
    RdpListCompare_guess compare;   /* 0x0C */
    int sorted;                     /* 0x10  find stops at the first greater key */
} RdpList_guess;
C_ASSERT(sizeof(RdpList_guess) == 0x14);

/* A parsed packet header (connection_parse_and_validate_arrival fills it; the fragment fields are those of
 * the fragment that arrived). Partly known; offsets verified against client 0x004F4B70, 0x004F4C40. */
typedef struct RdpPacketInfo_guess
{
    u16 flags;              /* 0x00  0x4000 = fin (msg_arrival_has_fin) */
    u16 seq;                /* 0x02 */
    u8  unknown04[4];       /* 0x04 */
    u16 frag_seq;           /* 0x08  sequence number of this fragment */
    u16 msgid;              /* 0x0A  the message the fragment belongs to (the key of RdpConnection_guess.msg_list) */
    u16 frag_index;         /* 0x0C  0-based fragment number within the message */
    u16 frag_count;         /* 0x0E */
    u8  channel;            /* 0x10 */
    u8  channel_seq;        /* 0x11 */
    u16 overhead;           /* 0x12  counted with data_len in the byte statistics (header bytes?) */
    u16 data_len;           /* 0x14  payload bytes of this fragment */
} RdpPacketInfo_guess;

#define RDP_FRAGMENT_SIZE_guess 0x200

/* msg_arrival.c: a message being reassembled from fragments of RDP_FRAGMENT_SIZE_guess bytes; the payload
 * follows the header. Verified against client 0x004F4B20..0x004F4CC0. */
typedef struct RdpMsgArrival_guess
{
    RdpLink_guess link;     /* 0x00  data = the message itself, key = &msgid */
    void *sender;           /* 0x10  the connection; set by the first fragment */
    u16 flags;              /* 0x14  copied from fragment 0 */
    u16 seq;                /* 0x16  copied from fragment 0 */
    u16 first_frag_seq;     /* 0x18  frag_seq of fragment 0 */
    u16 msgid;              /* 0x1A  the list key */
    u16 frag_count;         /* 0x1C */
    u8  channel;            /* 0x1E */
    u8  channel_seq;        /* 0x1F */
    u16 frags_received;     /* 0x20 */
    u8  unknown22[2];       /* 0x22 */
    u32 size;               /* 0x24  set when the last fragment arrives */
    u8  unknown28[0x14];    /* 0x28 */
    u8  data[1];            /* 0x3C  frag_count * RDP_FRAGMENT_SIZE_guess bytes */
} RdpMsgArrival_guess;
C_ASSERT(offsetof(RdpMsgArrival_guess, data) == 0x3C);

/* heap.c (object name unknown, between msg_arrival.c and list.c): a binary min-heap of element pointers;
 * each element records its own index. Verified against client 0x004F4CD0..0x004F4FE0. */
typedef struct RdpHeapElem_guess
{
    u32 index;              /* 0x00  position in items[], kept up to date by the heap */
    void *data;             /* 0x04  returned by extract/remove/top */
    void *key;              /* 0x08  passed to the heap's compare */
} RdpHeapElem_guess;

typedef struct RdpHeap_guess
{
    RdpHeapElem_guess **items;      /* 0x00  malloc'd */
    u32 count;                      /* 0x04 */
    u32 capacity;                   /* 0x08 */
    u32 grow;                       /* 0x0C  capacity added when full (the initial capacity) */
    RdpListCompare_guess compare;   /* 0x10  <0: first key is higher priority */
} RdpHeap_guess;
C_ASSERT(sizeof(RdpHeap_guess) == 0x14);

/* connhash.c: a sockaddr as the rdp layer keys connections by it (family 2 = AF_INET, 6 = AF_IPX). */
typedef struct RdpAddr_guess
{
    u16 family;             /* 0x00 */
    u16 port;               /* 0x02  AF_INET: sin_port */
    u32 addr;               /* 0x04  AF_INET: sin_addr */
    u8  rest[8];            /* 0x08  AF_IPX: node bytes 0x08..0x0B, socket at 0x0C */
} RdpAddr_guess;
C_ASSERT(sizeof(RdpAddr_guess) == 0x10);

/* connhash.c: one bucket = a lock and a sorted list of connections. Verified against client 0x004F50A0. */
typedef struct RdpHashBucket_guess
{
    u8  lock[0x18];         /* 0x00  CRITICAL_SECTION (winnt.h is not pulled into this header) */
    RdpList_guess list;     /* 0x18  sorted by compareHashKeys_guess */
} RdpHashBucket_guess;
C_ASSERT(sizeof(RdpHashBucket_guess) == 0x2C);

typedef struct RdpConnHash_guess
{
    u16 size;                       /* 0x00  a power of two */
    u8  unknown02[2];               /* 0x02 */
    RdpHashBucket_guess *buckets;   /* 0x04  malloc'd, size entries */
} RdpConnHash_guess;
C_ASSERT(sizeof(RdpConnHash_guess) == 0x8);

/* bandwidth.c: a leaky bucket. Verified against client 0x004F7B30..0x004F7C50. */
typedef struct RdpBandwidth_guess
{
    u32 bytes;              /* 0x00  queued, drained at `rate` */
    u32 time;               /* 0x04  timeGetTime of the last update */
    u32 rate;               /* 0x08  bytes per second (3000 initially) */
} RdpBandwidth_guess;
C_ASSERT(sizeof(RdpBandwidth_guess) == 0xC);

/* The rdp layer's global counters (rdp.c): a block of 64-bit counters reached through the pointer at 0x0057F698.
 * Index by byte offset / 8; the functions that bump them say what they count. */
typedef unsigned __int64 RdpU64_guess;
typedef struct RdpStatsDwordHalvesTri_guess
{
    u8      unknown0x000[0xC8]; /* 0x000  */
    u32     counter25Low_guess; /* 0x0C8* */
    union
    {
        u32     counter25High_guess; /* 0x0CC* */
        int     counter25HighSigned_guess; /* 0x0CC* */
    };
    u32     counter26Low_guess; /* 0x0D0* */
    union
    {
        u32     counter26High_guess; /* 0x0D4* */
        int     counter26HighSigned_guess; /* 0x0D4* */
    };
    u8      unknown0x0D8[0x68]; /* 0x0D8  */
    u32     counter40Low_guess; /* 0x140* */
    union
    {
        u32     counter40High_guess; /* 0x144* */
        int     counter40HighSigned_guess; /* 0x144* */
    };
    u8      unknown0x148[0x18]; /* 0x148  */
    u32     counter44Low_guess; /* 0x160* */
    union
    {
        u32     counter44High_guess; /* 0x164* */
        int     counter44HighSigned_guess; /* 0x164* */
    };
} RdpStatsDwordHalvesTri_guess; /* size 0x168 */
C_ASSERT(sizeof(RdpStatsDwordHalvesTri_guess) == 0x168);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter25Low_guess) == 0x0C8);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter25High_guess) == 0x0CC);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter26Low_guess) == 0x0D0);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter26High_guess) == 0x0D4);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter40Low_guess) == 0x140);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter40High_guess) == 0x144);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter44Low_guess) == 0x160);
C_ASSERT(offsetof(RdpStatsDwordHalvesTri_guess, counter44High_guess) == 0x164);

typedef struct RdpStats_guess
{
    union
    {
        RdpU64_guess c[0x10E];          /* 0x000  minimum extent through observed +0x868 counter */
        RdpStatsDwordHalvesTri_guess dwordHalves_guess;  /* 0x000* */
    };
} RdpStats_guess;
#define g_rdp_stats_guess  EQ_GLOBAL(RdpStats_guess *, 0x0057F698)
/* g_rdp_stats_guess->c[] by byte offset (8-aligned). */
#define RDP_STAT_guess(off)  (g_rdp_stats_guess->c[(off) / 8])

/* tx.c / rx.c: the rate estimator of a connection (0x120 bytes at conn+0x630): a ring of 64 (value, weight)
 * samples with running sums. Verified against client 0x004F7930 and 0x004F79A0. */
typedef struct RdpRttSample_guess
{
    u16 value;              /* 0x0 */
    u16 weight;             /* 0x2 */
} RdpRttSample_guess;

typedef struct RdpRttStats_guess
{
    RdpRttSample_guess samples[64]; /* 0x000 */
    u32 next;                       /* 0x100  index of the sample replaced next */
    u8  unknown104[4];              /* 0x104 */
    u32 sumsq_lo;                   /* 0x108  sum of value * value * weight (64 bits) */
    u32 sumsq_hi;                   /* 0x10C */
    u32 sum;                        /* 0x110  sum of value * weight */
    u16 weight_sum;                 /* 0x114 */
    u16 mean;                       /* 0x116  sum / weight_sum */
    u16 deviation;                  /* 0x118 */
    u8  unknown11A[6];              /* 0x11A */
} RdpRttStats_guess;
C_ASSERT(sizeof(RdpRttStats_guess) == 0x120);

/* rdp.c / connection.c: one connection. Only partly known; fields are added as functions are ported.
 * Offsets verified against the client functions named per field. */
struct RdpState_guess;
typedef struct RdpConnection_guess
{
    u8  unknown000[0xC];            /* 0x000  connection_init zeroes the first three dwords */
    struct RdpState_guess *rdp;     /* 0x00C  the instance it belongs to (connection_init) */
    RdpLink_guess hash_link;        /* 0x010  in its connhash bucket; key = &remote */
    s32 refcount;                   /* 0x020  connhash_lock_guess */
    u32     heapIndex_guess;    /* 0x024* */
    void *self_028;                 /* 0x028  connection_init: the connection itself */
    void *ptr_02C;                  /* 0x02C  connection_init: &field_030 */
    s32 field_030;                  /* 0x030  connection_init: 1 */
    s32 field_034;                  /* 0x034  connection_init: 0 */
    s32 state;                      /* 0x038  NetworkConnection_process_connection_state_guess: 1 idle, 2, 3, 4 */
    u8  lock[0x18];                 /* 0x03C  CRITICAL_SECTION, taken by connhash_lock_guess */
    s32 timeout_armed;              /* 0x054  NetworkConnection_isConnectionTimedOut_guess */
    u32 timeout_time;               /* 0x058  timeGetTime deadline */
    u32 field_05C;                  /* 0x05C */
    u32 field_060;                  /* 0x060 */
    union
    {
        u32 field_064;                  /* 0x064  connection_init's fourth argument */
        u8      field_064_lowByte_guess; /* 0x064* */
    };
    u32 stat[0x43];                 /* 0x068  per-connection counters, RDP_CSTAT_guess(conn, byte offset) */
    u8  disconnect_reason;          /* 0x174  connection_get_disconnect_info (-> out+4) */
    u8  disconnect_reason2;         /* 0x175  (-> out+5) */
    u8  unknown176[2];              /* 0x176 */
    u32 field_178;                  /* 0x178  timeGetTime, read by NetworkConnection_Disconnect_guess */
    u32 field_17C;                  /* 0x17C */
    u32 disconnect_value;           /* 0x180  (-> out+8) */
    u16 rx_base;                    /* 0x184  rx_process_sequence_number: every packet below this has arrived */
    u16 rx_max;                     /* 0x186  highest packet number seen */
    RdpBitArray_guess rx_ack_bits;  /* 0x188  arrivals after rx_base, bit 0 = rx_base + 1 */
    u16 ack_count;                  /* 0x28C  arrivals not yet acknowledged */
    u16 ack_min;                    /* 0x28E  lowest of them */
    u16 ack_max;                    /* 0x290  highest of them */
    u8  unknown292[2];              /* 0x292 */
    u32 rx_last_activity;           /* 0x294 */
    u32 rx_last_arrival;            /* 0x298 */
    RdpList_guess msg_list;         /* 0x29C  messages being reassembled, sorted by msgid */
    u16 chan_base_seq;              /* 0x2B0 */
    u16 chan_seq[20];               /* 0x2B2  per channel: next message seq expected (validateAndUpdateMessageSequence_guess) */
    u8  chan_order[20];             /* 0x2DA  per channel: next channel_seq to deliver (dequeue_message_by_sequence_guess) */
    u8  unknown2EE[2];              /* 0x2EE */
    RdpList_guess chan_queue[20];   /* 0x2F0  per channel: messages waiting for their turn */
    u16 msgid_valid;                /* 0x480  rx_validate_msgid_arrival: set by the first packet */
    u16 first_msgid;                /* 0x482 */
    u16 pending_flag;               /* 0x484  markMessageForPendingProcessing_guess */
    u16 pending_seq;                /* 0x486  the pending message's first_frag_seq */
    struct RdpMsgArrival_guess *pending;    /* 0x488 */
    u16 recv_seq;                   /* 0x48C  newest packet seq in order */
    u8  unknown48E[2];              /* 0x48E */
    union
    {
        RdpU64_guess recv_mask;         /* 0x490  bit n = packet recv_seq - 1 - n arrived */
        u32     recv_mask_lo_guess; /* 0x490* */
    };
    u32 field_498;                  /* 0x498 */
    u32 field_49C;                  /* 0x49C */
    u32     savedTtlClock_guess; /* 0x4A0* */
    u32 field_4A4;                  /* 0x4A4  tx_init: the socket (AF_INET), the IPX socket, -1 for a comport */
    RdpAddr_guess remote;           /* 0x4A8  the hash key */
    u16 tx_acked;                   /* 0x4B8  tx_init: first tx_next - 1 */
    u16 tx_serial;                  /* 0x4BA  counts packets sent (in the header) */
    union
    {
        u16 tx_next;                    /* 0x4BC  next packet number */
        short   tx_next_signed_guess; /* 0x4BC* */
    };
    union
    {
        u16 field_4BE;                  /* 0x4BE */
        short   field_4BE_signed_guess; /* 0x4BE* */
    };
    u32 tx_time;                    /* 0x4C0  timeGetTime of the last packet number issued */
    RdpBitArray_guess tx_sent_bits; /* 0x4C4  packets sent after tx_acked and not acknowledged yet */
    RdpList_guess tx_sent;          /* 0x5C8  sent, waiting for their ack; link data = RdpTxItem_guess */
    u32 tx_sent_bytes;              /* 0x5DC */
    RdpList_guess tx_wait;          /* 0x5E0  queued by the bandwidth limit */
    u32 tx_wait_bytes;              /* 0x5F4 */
    RdpList_guess tx_hold;          /* 0x5F8  beyond the send window */
    u32 tx_hold_bytes;              /* 0x60C */
    RdpBandwidth_guess bandwidth;   /* 0x610 */
    u8  unknown61C[4];              /* 0x61C */
    u32 field_620;                  /* 0x620  FUN_004f23a0 sets it; tx_init: 8000 */
    u32 field_624;                  /* 0x624  tx_init: timeGetTime */
    u32 field_628;                  /* 0x628 */
    u8  unknown62C[4];              /* 0x62C */
    RdpRttStats_guess rtt;          /* 0x630 */
    u8  unknown750[0x14];           /* 0x750 */
    s32 field_764;                  /* 0x764 */
    s32 field_768;                  /* 0x768  set by the first ack of a reliable packet (NetworkConnection_phandle_packet_acknowledgment_guess) */
    u16 field_76C;                  /* 0x76C  tx_init: tx_next */
    u8  unknown76E[2];              /* 0x76E */
    s32 field_770;                  /* 0x770  disconnect message sent */
    s32 field_774;                  /* 0x774 */
    u16 field_778;                  /* 0x778  seq of the disconnect message */
    u8  unknown77A[2];              /* 0x77A */
    s32 connected;                  /* 0x77C  connection_connected_guess */
    s32 field_780;                  /* 0x780 */
    u32 disconnect_code;            /* 0x784  (-> out+0) */
    s32 field_788;                  /* 0x788 */
    u32 field_78C;                  /* 0x78C  tx_init: 10000 */
    u32 field_790;                  /* 0x790  tx_init: 5000 */
    s32 delayed_ack;                /* 0x794  tx_set_delayed_ack_guess */
    u32 delayed_ack_time;           /* 0x798  timeGetTime + 50 */
    u16 last_rtt;                   /* 0x79C */
    u8  unknown79E[2];              /* 0x79E */
    void **close_event;             /* 0x7A0  uevent (HANDLE *) signalled when the connection goes away */
    s32 *close_result;              /* 0x7A4  receives 1 (disconnect acknowledged) or 0 */
    RdpAddr_guess ttl_addr;         /* 0x7A8  tx_init: the remote address again, port | 0x8000 (heartbeat probe) */
    u32 socket;                     /* 0x7B8  the instance's socket */
    u32 ttl_default;                /* 0x7BC */
    void *ttl_log;                  /* 0x7C0  malloc'd array of 16-byte records (SendHeartbeat) */
    u32 field_7C4;                  /* 0x7C4 */
    u32 field_7C8;                  /* 0x7C8  timeGetTime */
    u32     ttlStartTime_guess; /* 0x7CC* */
    union
    {
        u32     ttlStartClock_guess; /* 0x7D0* */
        long    ttlStartClock_signed_guess; /* 0x7D0* */
    };
    u32 ttl_value;                  /* 0x7D4  tx init: 1 */
    u32 ttl_max;                    /* 0x7D8  30 */
    u32 field_7DC;                  /* 0x7DC */
    u32 ttl_index;                  /* 0x7E0 */
} RdpConnection_guess;
C_ASSERT(offsetof(RdpConnection_guess, heapIndex_guess) == 0x024);
C_ASSERT(offsetof(RdpConnection_guess, savedTtlClock_guess) == 0x4A0);
C_ASSERT(offsetof(RdpConnection_guess, ttlStartTime_guess) == 0x7CC);
C_ASSERT(offsetof(RdpConnection_guess, ttlStartClock_guess) == 0x7D0);
#define RDP_CSTAT_guess(conn, off)  ((conn)->stat[((off) - 0x68) / 4])
C_ASSERT(offsetof(RdpConnection_guess, rdp) == 0xC);
C_ASSERT(offsetof(RdpConnection_guess, refcount) == 0x20);
C_ASSERT(offsetof(RdpConnection_guess, state) == 0x38);
C_ASSERT(offsetof(RdpConnection_guess, lock) == 0x3C);
C_ASSERT(offsetof(RdpConnection_guess, timeout_time) == 0x58);
C_ASSERT(offsetof(RdpConnection_guess, stat) == 0x68);
C_ASSERT(offsetof(RdpConnection_guess, disconnect_reason) == 0x174);
C_ASSERT(offsetof(RdpConnection_guess, disconnect_value) == 0x180);
C_ASSERT(offsetof(RdpConnection_guess, rx_base) == 0x184);
C_ASSERT(offsetof(RdpConnection_guess, rx_ack_bits) == 0x188);
C_ASSERT(offsetof(RdpConnection_guess, ack_count) == 0x28C);
C_ASSERT(offsetof(RdpConnection_guess, rx_last_activity) == 0x294);
C_ASSERT(offsetof(RdpConnection_guess, msg_list) == 0x29C);
C_ASSERT(offsetof(RdpConnection_guess, chan_seq) == 0x2B2);
C_ASSERT(offsetof(RdpConnection_guess, chan_order) == 0x2DA);
C_ASSERT(offsetof(RdpConnection_guess, chan_queue) == 0x2F0);
C_ASSERT(offsetof(RdpConnection_guess, msgid_valid) == 0x480);
C_ASSERT(offsetof(RdpConnection_guess, pending) == 0x488);
C_ASSERT(offsetof(RdpConnection_guess, recv_mask) == 0x490);
C_ASSERT(offsetof(RdpConnection_guess, remote) == 0x4A8);
C_ASSERT(offsetof(RdpConnection_guess, tx_acked) == 0x4B8);
C_ASSERT(offsetof(RdpConnection_guess, tx_sent_bits) == 0x4C4);
C_ASSERT(offsetof(RdpConnection_guess, tx_sent) == 0x5C8);
C_ASSERT(offsetof(RdpConnection_guess, tx_hold_bytes) == 0x60C);
C_ASSERT(offsetof(RdpConnection_guess, bandwidth) == 0x610);
C_ASSERT(offsetof(RdpConnection_guess, field_620) == 0x620);
C_ASSERT(offsetof(RdpConnection_guess, rtt) == 0x630);
C_ASSERT(offsetof(RdpConnection_guess, field_764) == 0x764);
C_ASSERT(offsetof(RdpConnection_guess, connected) == 0x77C);
C_ASSERT(offsetof(RdpConnection_guess, disconnect_code) == 0x784);
C_ASSERT(offsetof(RdpConnection_guess, delayed_ack_time) == 0x798);
C_ASSERT(offsetof(RdpConnection_guess, close_event) == 0x7A0);
C_ASSERT(offsetof(RdpConnection_guess, ttl_addr) == 0x7A8);
C_ASSERT(offsetof(RdpConnection_guess, ttl_log) == 0x7C0);
C_ASSERT(offsetof(RdpConnection_guess, ttl_index) == 0x7E0);

/* connection_get_disconnect_info's output (12 bytes). */
typedef struct RdpDisconnectInfo_guess
{
    u32 code;               /* 0x00 */
    u8  reason;             /* 0x04 */
    u8  reason2;            /* 0x05 */
    u8  unknown06[2];       /* 0x06  not written */
    u32 value;              /* 0x08 */
} RdpDisconnectInfo_guess;
C_ASSERT(sizeof(RdpDisconnectInfo_guess) == 0xC);

/* ringbuf.c (object name unknown, at the end of the rdp code): a circular byte buffer. Verified against client
 * 0x004F7FD0..0x004F8216. */
typedef struct RdpRing_guess
{
    u8 *base;               /* 0x00  malloc'd, size bytes */
    u32 size;               /* 0x04 */
    u8 *rd;                 /* 0x08  next byte to read */
    u8 *wr;                 /* 0x0C  next byte to write */
    u32 count;              /* 0x10  bytes stored (0 = empty, size = full when rd == wr) */
} RdpRing_guess;
C_ASSERT(sizeof(RdpRing_guess) == 0x14);

/* eventq.c: the serial link's queue (rdp+0x118), as 0x004F70D0..0x004F7180 read it. */
typedef struct RdpSerialItem_guess
{
    u8  unknown00[0x24];    /* 0x00 */
    u32 size;               /* 0x24  bytes */
    u32 time;               /* 0x28  timeGetTime when queued */
} RdpSerialItem_guess;

typedef struct RdpSerialQueue_guess
{
    u8  lock[0x18];         /* 0x00  CRITICAL_SECTION */
    u32 value;              /* 0x18  usemaphore_decrement_guess / GetSerialQueueValue_guess */
    u8  unknown1C[0x1C];    /* 0x1C */
    RdpList_guess list;     /* 0x38  link data = RdpSerialItem_guess, oldest first */
    u32 queued_bytes;       /* 0x4C */
} RdpSerialQueue_guess;
C_ASSERT(offsetof(RdpSerialQueue_guess, list) == 0x38);
C_ASSERT(sizeof(RdpSerialQueue_guess) == 0x50);

/* rdp.c: the network thread procedure stored at rdp+0x98 (rdp_state_tri layout prelude). */
typedef void (__cdecl *RdpNetworkThreadProc_guess)(int *);

typedef struct RdpSerialStateTri_guess
{
    u8      lock[0x18];         /* 0x000* */
    u32     value;              /* 0x018* */
    union
    {
        void*   file;               /* 0x01C* */
        void*   handle;             /* 0x01C* */
    };
    u16     id;                 /* 0x020* */
    u8      unknown0x022[2];    /* 0x022  */
    RdpRing_guess ring;               /* 0x024* */
    union
    {
        RdpList_guess list;               /* 0x038* */
        RdpList_guess pending;            /* 0x038* */
    };
    union
    {
        u32     queued_bytes;       /* 0x04C* */
        int     pending_bytes;      /* 0x04C* */
    };
    u8      unknown0x050[0x10]; /* 0x050  */
    unsigned long oversized_packets;  /* 0x060* */
    long    checksum_errors;    /* 0x064* */
    long    sequence_errors;    /* 0x068* */
    unsigned long bytes_received;     /* 0x06C* */
    unsigned long syncDiscardedBytes_guess; /* 0x070* */
    int     short_writes;       /* 0x074* */
    int     full_writes;        /* 0x078* */
    int     synchronousFullWrites_guess; /* 0x07C* */
    int     errors;             /* 0x080* */
    unsigned long lastError_guess;    /* 0x084* */
    int     synchronousShortWrites_guess; /* 0x088* */
    int     resourceFailures_guess; /* 0x08C* */
    u32     state;              /* 0x090* */
} RdpSerialStateTri_guess;      /* size 0x094 */
C_ASSERT(sizeof(RdpSerialStateTri_guess) == 0x094);
C_ASSERT(offsetof(RdpSerialStateTri_guess, lock) == 0x000);
C_ASSERT(offsetof(RdpSerialStateTri_guess, value) == 0x018);
C_ASSERT(offsetof(RdpSerialStateTri_guess, file) == 0x01C);
C_ASSERT(offsetof(RdpSerialStateTri_guess, id) == 0x020);
C_ASSERT(offsetof(RdpSerialStateTri_guess, ring) == 0x024);
C_ASSERT(offsetof(RdpSerialStateTri_guess, list) == 0x038);
C_ASSERT(offsetof(RdpSerialStateTri_guess, queued_bytes) == 0x04C);
C_ASSERT(offsetof(RdpSerialStateTri_guess, oversized_packets) == 0x060);
C_ASSERT(offsetof(RdpSerialStateTri_guess, checksum_errors) == 0x064);
C_ASSERT(offsetof(RdpSerialStateTri_guess, sequence_errors) == 0x068);
C_ASSERT(offsetof(RdpSerialStateTri_guess, bytes_received) == 0x06C);
C_ASSERT(offsetof(RdpSerialStateTri_guess, syncDiscardedBytes_guess) == 0x070);
C_ASSERT(offsetof(RdpSerialStateTri_guess, short_writes) == 0x074);
C_ASSERT(offsetof(RdpSerialStateTri_guess, full_writes) == 0x078);
C_ASSERT(offsetof(RdpSerialStateTri_guess, synchronousFullWrites_guess) == 0x07C);
C_ASSERT(offsetof(RdpSerialStateTri_guess, errors) == 0x080);
C_ASSERT(offsetof(RdpSerialStateTri_guess, lastError_guess) == 0x084);
C_ASSERT(offsetof(RdpSerialStateTri_guess, synchronousShortWrites_guess) == 0x088);
C_ASSERT(offsetof(RdpSerialStateTri_guess, resourceFailures_guess) == 0x08C);
C_ASSERT(offsetof(RdpSerialStateTri_guess, state) == 0x090);

/* rdp.c: the rdp instance (one per socket). Only partly known. */
typedef struct RdpState_guess
{
    u32     wsaStarted_guess;   /* 0x000* */
    u32     udpSocket_guess;    /* 0x004* */
    u32     icmpSocket_guess;   /* 0x008* */
    u32 socket;                     /* 0x00C  copied to every connection (conn+0x7B8) */
    u32     ipxSocket_guess;    /* 0x010* */
    RdpAddr_guess localAddr_guess;    /* 0x014* */
    RdpAddr_guess ttlAddr_guess;      /* 0x024* */
    u8      ipxAddr_guess[0xE]; /* 0x034* */
    u8      unknown042[2];      /* 0x042  */
    u32 defaultTtl;           /* 0x044  copied to every connection (conn+0x7BC) */
    u32     ipxBroadcast_guess; /* 0x048* */
    u32     udpBroadcast_guess; /* 0x04C* */
    RdpConnHash_guess hash;         /* 0x050  the connections */
    RdpHeap_guess timeout_heap;     /* 0x058  the connections by deadline (get_next_connection_timeout_guess) */
    u8  timeout_lock[0x18];         /* 0x06C  CRITICAL_SECTION, taken around timeout_heap */
    int     wakeupPending_guess; /* 0x084* */
    int     destroyerFrees_guess; /* 0x088* */
    int     running_guess;      /* 0x08C* */
    void*   thread_guess;       /* 0x090* */
    u32     threadId_guess;     /* 0x094* */
    RdpNetworkThreadProc_guess threadProc_guess;   /* 0x098* */
    struct RdpState_guess* threadContext_guess; /* 0x09C* */
    void*   messageSemaphore_guess; /* 0x0A0* */
    RdpList_guess messageList_guess;  /* 0x0A4* */
    u8      messageLock_guess[0x18]; /* 0x0B8* */
    RdpList_guess readyList_guess;    /* 0x0D0* */
    u8      readyLock_guess[0x18]; /* 0x0E4* */
    u32     inputBytes_guess;   /* 0x0FC* */
    u32     skippedBytes_guess; /* 0x100* */
    u32     rateTime_guess;     /* 0x104* */
    u32 input_rate;                 /* 0x108  rdp_get_input_rate_guess */
    u32     skippedRate_guess;  /* 0x10C* */
    int     unk_0x110;          /* 0x110* */
    int     unk_0x114;          /* 0x114* */
    union
    {
        RdpSerialQueue_guess serial;    /* 0x118  the serial link's queue */
        RdpSerialStateTri_guess serialState_guess;  /* 0x118* */
    };
} RdpState_guess;
C_ASSERT(offsetof(RdpState_guess, timeout_heap) == 0x58);
C_ASSERT(offsetof(RdpState_guess, timeout_lock) == 0x6C);
C_ASSERT(offsetof(RdpState_guess, serial) == 0x118);
C_ASSERT(sizeof(RdpState_guess) == 0x1AC);
C_ASSERT(offsetof(RdpState_guess, wsaStarted_guess) == 0x000);
C_ASSERT(offsetof(RdpState_guess, udpSocket_guess) == 0x004);
C_ASSERT(offsetof(RdpState_guess, icmpSocket_guess) == 0x008);
C_ASSERT(offsetof(RdpState_guess, ipxSocket_guess) == 0x010);
C_ASSERT(offsetof(RdpState_guess, localAddr_guess) == 0x014);
C_ASSERT(offsetof(RdpState_guess, ttlAddr_guess) == 0x024);
C_ASSERT(offsetof(RdpState_guess, ipxAddr_guess) == 0x034);
C_ASSERT(offsetof(RdpState_guess, ipxBroadcast_guess) == 0x048);
C_ASSERT(offsetof(RdpState_guess, udpBroadcast_guess) == 0x04C);
C_ASSERT(offsetof(RdpState_guess, wakeupPending_guess) == 0x084);
C_ASSERT(offsetof(RdpState_guess, destroyerFrees_guess) == 0x088);
C_ASSERT(offsetof(RdpState_guess, running_guess) == 0x08C);
C_ASSERT(offsetof(RdpState_guess, thread_guess) == 0x090);
C_ASSERT(offsetof(RdpState_guess, threadId_guess) == 0x094);
C_ASSERT(offsetof(RdpState_guess, threadProc_guess) == 0x098);
C_ASSERT(offsetof(RdpState_guess, threadContext_guess) == 0x09C);
C_ASSERT(offsetof(RdpState_guess, messageSemaphore_guess) == 0x0A0);
C_ASSERT(offsetof(RdpState_guess, messageList_guess) == 0x0A4);
C_ASSERT(offsetof(RdpState_guess, messageLock_guess) == 0x0B8);
C_ASSERT(offsetof(RdpState_guess, readyList_guess) == 0x0D0);
C_ASSERT(offsetof(RdpState_guess, readyLock_guess) == 0x0E4);
C_ASSERT(offsetof(RdpState_guess, inputBytes_guess) == 0x0FC);
C_ASSERT(offsetof(RdpState_guess, skippedBytes_guess) == 0x100);
C_ASSERT(offsetof(RdpState_guess, rateTime_guess) == 0x104);
C_ASSERT(offsetof(RdpState_guess, skippedRate_guess) == 0x10C);
C_ASSERT(offsetof(RdpState_guess, unk_0x110) == 0x110);
C_ASSERT(offsetof(RdpState_guess, unk_0x114) == 0x114);

/* fast_malloc.c: one size class of the pool allocator (four, at 0x006F1320, 0x38 apart). Verified against
 * client 0x004F4740..0x004F4B10. A free block starts with its list link (next, prev, data = the block itself,
 * key); an allocated one with the 8-byte header (class index, payload size) in front of the user bytes. */
typedef struct RdpPool_guess
{
    u32 block_size;         /* 0x00  0x48, 0x90, 0x120, 0x248 */
    u32 payload;            /* 0x04  block_size - 8: the largest request served */
    u32 count;              /* 0x08  free blocks (split: +2 here, -1 in the next larger class) */
    RdpList_guess list;     /* 0x0C  free blocks */
    u8  lock[0x18];         /* 0x20  CRITICAL_SECTION */
} RdpPool_guess;
C_ASSERT(sizeof(RdpPool_guess) == 0x38);

/* Rewritten rdp functions called from other rdp files (a fixture then lists the callee's source too; the
 * harness keys the call by the callee's original address). */
/* list.c */
void list_init_guess(RdpList_guess *list);
void setLinkedListProperties_guess(RdpList_guess *list, int sorted, RdpListCompare_guess compare);
void list_add_head_guess(RdpList_guess *list, RdpLink_guess *link);
void list_add_tail_guess(RdpList_guess *list, RdpLink_guess *link);
void *list_remove_head_guess(RdpList_guess *list);
void LinkedList_insertNodeIntoSortedLinkedList_guess(RdpList_guess *list, RdpLink_guess *link);
RdpLink_guess *list_find_link_by_key_guess(RdpList_guess *list, void *key);
void *removeNodeFromLinkedList_guess(RdpList_guess *list, RdpLink_guess *link);
/* heap.c */
int initializeArrayBuffer_guess(RdpHeap_guess *heap, u32 size, RdpListCompare_guess compare);
int heapInsertElement_guess(RdpHeap_guess *heap, RdpHeapElem_guess *elem);
void *removeHeapElement_guess(RdpHeap_guess *heap, RdpHeapElem_guess *elem);
void heapifyDown_guess(RdpHeap_guess *heap, u32 i);
void updateHeapElement_guess(RdpHeap_guess *heap, RdpHeapElem_guess *elem);
void *Heap_extractMinFromHeap_guess(RdpHeap_guess *heap);
void *Heap_getTopHeapElement_guess(RdpHeap_guess *heap);
/* bitops.c */
void bitarray_clear_guess(RdpBitArray_guess *ba);
void copyBitfieldToBuffer_guess(const unsigned char *bits, unsigned char *dst, unsigned int bit, unsigned int count);
unsigned char bitarray_setbit(unsigned char *bits, unsigned int n);
unsigned char clear_bit_and_return_previous_value_guess(unsigned char *bits, unsigned int n);
unsigned char getbit_guess(const unsigned char *bits, unsigned int n);
/* eventq.c */
u32 GetSerialQueueValue_guess(RdpSerialQueue_guess *q);
int is_connection_inactive_guess(RdpSerialQueue_guess *q);
unsigned long calculateAdjustedClientTime_guess(RdpSerialQueue_guess *q);
unsigned long GetSerialQueueStallTime_guess(RdpSerialQueue_guess *q);

/* uevent.c (HANDLE * parameters) */
void uevent_destroy_guess(void **ev);
void uevent_init_guess(void **ev);
int uevent_create(void **ev);
void uevent_signal_guess(void **ev);
void uevent_wait_guess(void **ev);
/* bandwidth.c */
void bandwidth_init_guess(RdpBandwidth_guess *bw);
void UpdateBandwidthQuota_guess(RdpBandwidth_guess *bw, u32 bytes);
u32 update_token_bucket_guess(RdpBandwidth_guess *bw);
u32 bandwidth_get_time_empty_guess(RdpBandwidth_guess *bw);
/* connhash.c */
RdpConnection_guess *connhash_lock_guess(RdpConnHash_guess *hash, RdpAddr_guess *addr);
void addConnectionToHashTable_guess(RdpConnHash_guess *hash, RdpConnection_guess *conn);
RdpConnection_guess *removeConnectionFromHashTable_guess(RdpConnHash_guess *hash, RdpConnection_guess *conn);
/* msg_arrival.c */
void initializeMessageHeader_guess(RdpMsgArrival_guess *msg, u16 msgid);
void initializeChannelSequenceKey_guess(RdpMsgArrival_guess *msg);
int process_network_message_packet_guess(RdpMsgArrival_guess *msg, void *sender, RdpPacketInfo_guess *info, const void *data);
int ValidateFragmentConsistency_guess(RdpMsgArrival_guess *msg, RdpPacketInfo_guess *info);
/* rdp.c */
int CheckSerialQueueStatus_guess(RdpState_guess *rdp);
u32 getTimeFromEQClient_guess(RdpState_guess *rdp);
u32 rdp_get_serial_stall_time_guess(RdpState_guess *rdp);
void format_sockaddr(char *out, const RdpAddr_guess *addr);
/* log.c (cdecl, varargs) */
void discard_log_append(const char *format, ...);
/* rx.c */
RdpMsgArrival_guess *dequeue_message_by_sequence_guess(RdpConnection_guess *conn, u8 channel);
int validateAndUpdateMessageSequence_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg);
RdpMsgArrival_guess *clearAndReturnPendingMessage_guess(RdpConnection_guess *conn);
void markMessageForPendingProcessing_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg);
/* fast_malloc.c */
void fast_malloc_init(u32 size);
void cleanupMemoryPool_guess(void);
void *fast_malloc(u32 size);
void *allocate_memory_from_pool_guess(int idx);
void fast_free_guess(void *p);
/* ringbuf.c */
void initializeNetworkBufferHeader_guess(RdpRing_guess *ring);
int allocate_memory_buffer_guess(RdpRing_guess *ring, u32 size);
void FreeRingBuffer_guess(RdpRing_guess *ring);
void read_network_data_into_buffer_guess(RdpRing_guess *ring, void *file);
void read_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n);
void copy_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n);

#endif /* TRILOGY_RDP_H */
