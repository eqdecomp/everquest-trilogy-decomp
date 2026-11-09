/*
 * eqmain/rdp.h - the RDP network library as eqmain.dll (2001-08-20) links it.
 *
 * eqmain.dll links another version of the rdp library than eqgame.exe (include/trilogy/rdp.h): the same
 * code, but its lists are intrusive (a list records where the link and the key sit in an item; a link is
 * only next/prev), its bit windows are 1024 bits, and a few structs gained or lost fields. Every struct
 * and field keeps the name it has in trilogy/rdp.h, so a function cloned from eqgame.exe (by a script)
 * compiles unchanged against this header; only the offsets differ. Each field's comment gives its eqmain
 * offset first and, where it moved, the eqgame.exe offset after "eqgame". Layouts come from the same-shape
 * sibling pairs (names/siblings/eqmain_trilogy.csv): every displacement that differs between the two
 * listings of a pair (clone.offset_map), plus eqmain's own list code (0x1002DE90..0x1002E170), tx_init
 * (0x1002B000), rx_init (0x1002E930) and rdp_create (0x1002A460). "unknownXXX" fields are named after
 * their eqmain offset. Addresses are eqmain.dll virtual addresses.
 *
 * Use: sources under src/eqmain/Network/rdp/ include this header instead of trilogy/rdp.h (the script maps
 * the include when it creates such a file; see tools/docs/EQMAIN.md "rdp structs").
 */
#ifndef EQMAIN_RDP_H
#define EQMAIN_RDP_H

#ifdef TRILOGY_RDP_H
#error "eqmain/rdp.h and trilogy/rdp.h describe two versions of the same structs: include one"
#endif

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

/* bitops.c: a 1024-bit window, MSB first within each byte (eqgame: 2048 bits). bitarray_clear 0x10030D50
 * clears 0x20 dwords and the count at +0x80; copyBitfieldToBuffer 0x10030D70 wraps at 0x400 bits. */
typedef struct RdpBitArray_guess
{
    u8  bits[128];          /* 0x00 */
    u32 count;              /* 0x80  (eqgame 0x100) cleared with the bits */
} RdpBitArray_guess;
C_ASSERT(sizeof(RdpBitArray_guess) == 0x84);

/* list.c: compare(item key a, key b) -> <0, 0, >0. __cdecl. */
typedef int (__cdecl *RdpListCompare_guess)(void *a, void *b);

/* list.c: one link, embedded in its item at the list's link_offset. eqgame's link also carries data and
 * key pointers (0x10 bytes); eqmain's list functions take and return the item itself (link - link_offset)
 * and find the key at item + key_offset. Verified against eqmain 0x1002DED0 (add head), 0x1002DF20 (add
 * tail), 0x1002E130 (remove head). */
typedef struct RdpLink_guess
{
    struct RdpLink_guess *next;     /* 0x00 */
    struct RdpLink_guess *prev;     /* 0x04 */
} RdpLink_guess;
C_ASSERT(sizeof(RdpLink_guess) == 0x8);

/* list.c: an intrusive doubly linked list. Verified against eqmain 0x1002DEA0 (init: list, link_offset,
 * key_offset, compare; head, tail and count zeroed), 0x1002E0C0 (find: compare(item + key_offset, key)
 * until it is >= 0). eqgame's is 0x14 bytes (head, tail, count 0x08, compare 0x0C, sorted 0x10); every
 * struct holding a list grows by 4 per list. */
typedef struct RdpList_guess
{
    RdpLink_guess *head;            /* 0x00 */
    RdpLink_guess *tail;            /* 0x04 */
    u32 link_offset;                /* 0x08  the link's offset in an item */
    u32 key_offset;                 /* 0x0C  the key's offset in an item (passed to compare) */
    int count;                      /* 0x10  (eqgame 0x08) */
    RdpListCompare_guess compare;   /* 0x14  (eqgame 0x0C) */
} RdpList_guess;
C_ASSERT(sizeof(RdpList_guess) == 0x18);

/* A parsed packet header: no displacement into it differs between the siblings (connection_parse_and_
 * validate_arrival 0x1002D3C0, the rx validators); as trilogy/rdp.h. */
typedef struct RdpPacketInfo_guess
{
    u16 flags;              /* 0x00  0x4000 = fin (msg_arrival_has_fin) */
    u16 seq;                /* 0x02 */
    u8  unknown04[4];       /* 0x04 */
    u16 frag_seq;           /* 0x08  sequence number of this fragment */
    u16 msgid;              /* 0x0A  the message the fragment belongs to */
    u16 frag_index;         /* 0x0C  0-based fragment number within the message */
    u16 frag_count;         /* 0x0E */
    u8  channel;            /* 0x10 */
    u8  channel_seq;        /* 0x11 */
    u16 overhead;           /* 0x12 */
    u16 data_len;           /* 0x14  payload bytes of this fragment */
} RdpPacketInfo_guess;

#define RDP_FRAGMENT_SIZE_guess 0x200

/* msg_arrival.c: a message being reassembled; the payload follows the header. Everything after the link
 * moved by -8 (the 8-byte link). Verified against eqmain 0x1002DE30 (sender, first_frag_seq, frag_count),
 * 0x1002DE70 (size, data), 0x1002F440 (seq, channel), 0x1002F550 (first_frag_seq); rx_init 0x1002E930 keys
 * the message list by +0x12 (msgid) and the channel queues by +0x17 (channel_seq). */
typedef struct RdpMsgArrival_guess
{
    RdpLink_guess link;     /* 0x00  in RdpConnection_guess.msg_list / chan_queue[] */
    void *sender;           /* 0x08  (eqgame 0x10) the connection; set by the first fragment */
    u16 flags;              /* 0x0C  (eqgame 0x14) copied from fragment 0 */
    u16 seq;                /* 0x0E  (eqgame 0x16) copied from fragment 0 */
    u16 first_frag_seq;     /* 0x10  (eqgame 0x18) frag_seq of fragment 0 */
    u16 msgid;              /* 0x12  (eqgame 0x1A) the msg_list key */
    u16 frag_count;         /* 0x14  (eqgame 0x1C) */
    u8  channel;            /* 0x16  (eqgame 0x1E) */
    u8  channel_seq;        /* 0x17  (eqgame 0x1F) the chan_queue key */
    u16 frags_received;     /* 0x18  (eqgame 0x20) */
    u8  unknown1A[2];       /* 0x1A */
    u32 size;               /* 0x1C  (eqgame 0x24) set when the last fragment arrives */
    u8  unknown20[0x14];    /* 0x20 */
    u8  data[1];            /* 0x34  (eqgame 0x3C) frag_count * RDP_FRAGMENT_SIZE_guess bytes */
} RdpMsgArrival_guess;
C_ASSERT(offsetof(RdpMsgArrival_guess, sender) == 0x8);
C_ASSERT(offsetof(RdpMsgArrival_guess, size) == 0x1C);
C_ASSERT(offsetof(RdpMsgArrival_guess, data) == 0x34);

/* heap.c: a binary min-heap of element pointers. The heap functions (0x1002E200..0x1002E530) are
 * byte-identical in shape to eqgame's: as trilogy/rdp.h. */
typedef struct RdpHeapElem_guess
{
    u32 index;              /* 0x00  position in items[] */
    void *data;             /* 0x04 */
    void *key;              /* 0x08 */
} RdpHeapElem_guess;

typedef struct RdpHeap_guess
{
    RdpHeapElem_guess **items;      /* 0x00  malloc'd */
    u32 count;                      /* 0x04 */
    u32 capacity;                   /* 0x08 */
    u32 grow;                       /* 0x0C */
    RdpListCompare_guess compare;   /* 0x10 */
} RdpHeap_guess;
C_ASSERT(sizeof(RdpHeap_guess) == 0x14);

/* connhash.c: a sockaddr as the rdp layer keys connections by it. */
typedef struct RdpAddr_guess
{
    u16 family;             /* 0x00 */
    u16 port;               /* 0x02 */
    u32 addr;               /* 0x04 */
    u8  rest[8];            /* 0x08 */
} RdpAddr_guess;
C_ASSERT(sizeof(RdpAddr_guess) == 0x10);

/* connhash.c: one bucket = a lock and a list of connections (link at conn+0x10, key conn+0x478: eqmain
 * HashTable_initializeHashTable 0x1002E5D0). cleanup_hash_table 0x1002E680 steps 0x30 (eqgame 0x2C). */
typedef struct RdpHashBucket_guess
{
    u8  lock[0x18];         /* 0x00  CRITICAL_SECTION */
    RdpList_guess list;     /* 0x18 */
} RdpHashBucket_guess;
C_ASSERT(sizeof(RdpHashBucket_guess) == 0x30);

typedef struct RdpConnHash_guess
{
    u16 size;                       /* 0x00  a power of two */
    u8  unknown02[2];               /* 0x02 */
    RdpHashBucket_guess *buckets;   /* 0x04  malloc'd, size entries */
} RdpConnHash_guess;
C_ASSERT(sizeof(RdpConnHash_guess) == 0x8);

/* bandwidth.c: a leaky bucket; the bandwidth functions are the same shape as eqgame's. */
typedef struct RdpBandwidth_guess
{
    u32 bytes;              /* 0x00 */
    u32 time;               /* 0x04 */
    u32 rate;               /* 0x08 */
} RdpBandwidth_guess;
C_ASSERT(sizeof(RdpBandwidth_guess) == 0xC);

/* The rdp layer's global 64-bit counters, reached through the pointer at 0x100498B4 (eqgame 0x0057F698;
 * eqmain NetworkConnection_SendPacket 0x1002CDDA). One more counter than eqgame's: offsets up to 0xC0 are
 * the same (rx_record_seqnum_arrival 0x1002EBD0 keeps 0xC0), from 0x170 on they are eqgame's + 8 (the
 * Disconnect/stats/SendPacket siblings); where between the extra counter sits is not known. Index by
 * eqmain byte offset (the script translates the eqgame offsets it can prove). */
typedef unsigned __int64 RdpU64_guess;
typedef struct RdpStats_guess
{
    RdpU64_guess c[271];            /* 0x000  0x10F counters, the last bump is list.c +0x870 */
    u8  unknown878[8];              /* 0x878  up to g_rdp_pools_guess 0x1013D720 (0x1013CEA0 + 0x880) */
} RdpStats_guess;
C_ASSERT(sizeof(RdpStats_guess) == 0x880);
#define g_rdp_stats_guess  EQ_GLOBAL(RdpStats_guess *, 0x100498B4)
#define RDP_STAT_guess(off)  (g_rdp_stats_guess->c[(off) / 8])

/* tx.c / rx.c: the rate estimator of a connection (0x120 bytes at conn+0x588). The estimator functions
 * (0x10030A10, 0x10030A80) are the same shape as eqgame's, and conn+0x69E/0x6A0 are its mean and deviation
 * as eqgame's conn+0x746/0x748: as trilogy/rdp.h. */
typedef struct RdpRttSample_guess
{
    u16 value;              /* 0x0 */
    u16 weight;             /* 0x2 */
} RdpRttSample_guess;

typedef struct RdpRttStats_guess
{
    RdpRttSample_guess samples[64]; /* 0x000 */
    u32 next;                       /* 0x100 */
    u8  unknown104[4];              /* 0x104 */
    u32 sumsq_lo;                   /* 0x108 */
    u32 sumsq_hi;                   /* 0x10C */
    u32 sum;                        /* 0x110 */
    u16 weight_sum;                 /* 0x114 */
    u16 mean;                       /* 0x116 */
    u16 deviation;                  /* 0x118 */
    u8  unknown11A[6];              /* 0x11A */
} RdpRttStats_guess;
C_ASSERT(sizeof(RdpRttStats_guess) == 0x120);

/* rdp.c / connection.c: one connection, known up to 0x73C (eqgame 0x7E4). The 8-byte hash link takes 8 off
 * everything after 0x10, the 1024-bit windows 0x80 each, the 0x18-byte lists add 4 each, and eqgame's
 * unknown dwords at 0x61C and 0x62C have no eqmain counterpart. Offsets verified against the eqmain
 * functions named per field (tx_init 0x1002B000 and rx_init 0x1002E930 write most of them). */
struct RdpState_guess;
typedef struct RdpConnection_guess
{
    u8  unknown000[0xC];            /* 0x000 */
    struct RdpState_guess *rdp;     /* 0x00C */
    RdpLink_guess hash_link;        /* 0x010  in its connhash bucket (link_offset 0x10, key_offset 0x478) */
    s32 refcount;                   /* 0x018  (eqgame 0x020) */
    union
    {
        u8  unknown01C[4];              /* 0x01C  (eqgame 0x024) RdpHeapElem_guess.index (rdp_resort 0x10028AE0) */
        u32     heapIndex_guess;    /* 0x01C* */
    };
    void *self_028;                 /* 0x020  (eqgame 0x028) */
    void *ptr_02C;                  /* 0x024  (eqgame 0x02C) */
    s32 field_030;                  /* 0x028  (eqgame 0x030) rdp_resort, get_next_connection_timeout 0x100305F0 */
    s32 field_034;                  /* 0x02C  (eqgame 0x034) */
    s32 state;                      /* 0x030  (eqgame 0x038) process_connection_state 0x1002DAE0 */
    u8  lock[0x18];                 /* 0x034  (eqgame 0x03C) CleanupNetworkConnectionResources 0x1002D210 */
    s32 timeout_armed;              /* 0x04C  (eqgame 0x054) isConnectionTimedOut 0x1002DD00 */
    u32 timeout_time;               /* 0x050  (eqgame 0x058) */
    u32 field_05C;                  /* 0x054  (eqgame 0x05C) */
    u32 field_060;                  /* 0x058  (eqgame 0x060) */
    u32 field_064;                  /* 0x05C  (eqgame 0x064) getConnectionStatus 0x1002D230 */
    u32 stat[0x43];                 /* 0x060  (eqgame 0x068) rx_init 0x1002E930; RDP_CSTAT_guess */
    u8  disconnect_reason;          /* 0x16C  (eqgame 0x174) pprocess_network_packet_stats 0x1002D870 */
    u8  disconnect_reason2;         /* 0x16D  (eqgame 0x175) */
    u8  unknown16E[2];              /* 0x16E */
    u32 field_178;                  /* 0x170  (eqgame 0x178) */
    u32 field_17C;                  /* 0x174  (eqgame 0x17C) */
    u32 disconnect_value;           /* 0x178  (eqgame 0x180) */
    u16 rx_base;                    /* 0x17C  (eqgame 0x184) process_sequence_number 0x1002F170 */
    u16 rx_max;                     /* 0x17E  (eqgame 0x186) */
    RdpBitArray_guess rx_ack_bits;  /* 0x180  (eqgame 0x188) */
    u16 ack_count;                  /* 0x204  (eqgame 0x28C) */
    u16 ack_min;                    /* 0x206  (eqgame 0x28E) */
    u16 ack_max;                    /* 0x208  (eqgame 0x290) */
    u8  unknown20A[2];              /* 0x20A */
    u32 rx_last_activity;           /* 0x20C  (eqgame 0x294) */
    u32 rx_last_arrival;            /* 0x210  (eqgame 0x298) */
    RdpList_guess msg_list;         /* 0x214  (eqgame 0x29C) link_offset 0, key_offset 0x12 (msgid) */
    u16 chan_base_seq;              /* 0x22C  (eqgame 0x2B0) */
    u16 chan_seq[20];               /* 0x22E  (eqgame 0x2B2) */
    u8  chan_order[20];             /* 0x256  (eqgame 0x2DA) */
    u8  unknown26A[2];              /* 0x26A */
    RdpList_guess chan_queue[20];   /* 0x26C  (eqgame 0x2F0) key_offset 0x17 (channel_seq) */
    u16 msgid_valid;                /* 0x44C  (eqgame 0x480) */
    u16 first_msgid;                /* 0x44E  (eqgame 0x482) */
    u16 pending_flag;               /* 0x450  (eqgame 0x484) */
    u16 pending_seq;                /* 0x452  (eqgame 0x486) */
    struct RdpMsgArrival_guess *pending;    /* 0x454  (eqgame 0x488) */
    u16 recv_seq;                   /* 0x458  (eqgame 0x48C) */
    u8  unknown45A[6];              /* 0x45A */
    RdpU64_guess recv_mask;         /* 0x460  (eqgame 0x490) */
    u32 field_498;                  /* 0x468  (eqgame 0x498) */
    u32 field_49C;                  /* 0x46C  (eqgame 0x49C) */
    u32     savedTtlClock_guess; /* 0x470* (eqgame 0x4A0) initializeRDPConnection 0x1002CFC0 writes it */
    u32 field_4A4;                  /* 0x474  (eqgame 0x4A4) */
    RdpAddr_guess remote;           /* 0x478  (eqgame 0x4A8) the hash key */
    u16 tx_acked;                   /* 0x488  (eqgame 0x4B8) */
    u16 tx_serial;                  /* 0x48A  (eqgame 0x4BA) */
    u16 tx_next;                    /* 0x48C  (eqgame 0x4BC) */
    union
    {
        u16 field_4BE;                  /* 0x48E  (eqgame 0x4BE) */
        short   field_4BE_signed_guess; /* 0x48E* */
    };
    u32 tx_time;                    /* 0x490  (eqgame 0x4C0) */
    RdpBitArray_guess tx_sent_bits; /* 0x494  (eqgame 0x4C4) */
    RdpList_guess tx_sent;          /* 0x518  (eqgame 0x5C8) */
    u32 tx_sent_bytes;              /* 0x530  (eqgame 0x5DC) */
    RdpList_guess tx_wait;          /* 0x534  (eqgame 0x5E0) */
    u32 tx_wait_bytes;              /* 0x54C  (eqgame 0x5F4) */
    RdpList_guess tx_hold;          /* 0x550  (eqgame 0x5F8) */
    u32 tx_hold_bytes;              /* 0x568  (eqgame 0x60C) */
    RdpBandwidth_guess bandwidth;   /* 0x56C  (eqgame 0x610) */
    u8  unknown578[4];              /* 0x578  (eqgame 0x61C) */
    u32 field_620;                  /* 0x57C  (eqgame 0x620) tx_init: 8000 */
    u32 field_624;                  /* 0x580  (eqgame 0x624) */
    u32 field_628;                  /* 0x584  (eqgame 0x628); eqgame's unknown62C has no counterpart */
    RdpRttStats_guess rtt;          /* 0x588  (eqgame 0x630) */
    u8  unknown6A8[0x14];           /* 0x6A8  (eqgame 0x750) per channel byte counters (connection_sendv) */
    s32 field_764;                  /* 0x6BC  (eqgame 0x764) */
    s32 field_768;                  /* 0x6C0  (eqgame 0x768) */
    u16 field_76C;                  /* 0x6C4  (eqgame 0x76C) */
    u8  unknown6C6[2];              /* 0x6C6 */
    s32 field_770;                  /* 0x6C8  (eqgame 0x770) */
    s32 field_774;                  /* 0x6CC  (eqgame 0x774) */
    u16 field_778;                  /* 0x6D0  (eqgame 0x778) */
    u8  unknown6D2[2];              /* 0x6D2 */
    s32 connected;                  /* 0x6D4  (eqgame 0x77C) */
    s32 field_780;                  /* 0x6D8  (eqgame 0x780) */
    u32 disconnect_code;            /* 0x6DC  (eqgame 0x784) */
    s32 field_788;                  /* 0x6E0  (eqgame 0x788) */
    u32 field_78C;                  /* 0x6E4  (eqgame 0x78C) */
    u32 field_790;                  /* 0x6E8  (eqgame 0x790) */
    s32 delayed_ack;                /* 0x6EC  (eqgame 0x794) */
    u32 delayed_ack_time;           /* 0x6F0  (eqgame 0x798) */
    u16 last_rtt;                   /* 0x6F4  (eqgame 0x79C) */
    u8  unknown6F6[2];              /* 0x6F6 */
    void **close_event;             /* 0x6F8  (eqgame 0x7A0) */
    s32 *close_result;              /* 0x6FC  (eqgame 0x7A4) */
    RdpAddr_guess ttl_addr;         /* 0x700  (eqgame 0x7A8) */
    u32 socket;                     /* 0x710  (eqgame 0x7B8) */
    u32 ttl_default;                /* 0x714  (eqgame 0x7BC) */
    union
    {
        void *ttl_log;                  /* 0x718  (eqgame 0x7C0) */
        int     unk_0x718;          /* 0x718* */
    };
    u32 field_7C4;                  /* 0x71C  (eqgame 0x7C4) */
    u32 field_7C8;                  /* 0x720  (eqgame 0x7C8) */
    u32     ttlStartTime_guess; /* 0x724* (eqgame 0x7CC) */
    union
    {
        u32     ttlStartClock_guess; /* 0x728* */
        long    ttlStartClock_signed_guess; /* 0x728* */
    };
    u32 ttl_value;                  /* 0x72C  (eqgame 0x7D4) */
    u32 ttl_max;                    /* 0x730  (eqgame 0x7D8) */
    u32 field_7DC;                  /* 0x734  (eqgame 0x7DC) */
    u32 ttl_index;                  /* 0x738  (eqgame 0x7E0) */
} RdpConnection_guess;
C_ASSERT(offsetof(RdpConnection_guess, savedTtlClock_guess) == 0x470);
C_ASSERT(offsetof(RdpConnection_guess, ttlStartTime_guess) == 0x724);
C_ASSERT(offsetof(RdpConnection_guess, ttlStartClock_guess) == 0x728);
#define RDP_CSTAT_guess(conn, off)  ((conn)->stat[((off) - 0x60) / 4])
C_ASSERT(offsetof(RdpConnection_guess, refcount) == 0x18);
C_ASSERT(offsetof(RdpConnection_guess, state) == 0x30);
C_ASSERT(offsetof(RdpConnection_guess, stat) == 0x60);
C_ASSERT(offsetof(RdpConnection_guess, disconnect_reason) == 0x16C);
C_ASSERT(offsetof(RdpConnection_guess, rx_base) == 0x17C);
C_ASSERT(offsetof(RdpConnection_guess, ack_count) == 0x204);
C_ASSERT(offsetof(RdpConnection_guess, msg_list) == 0x214);
C_ASSERT(offsetof(RdpConnection_guess, chan_order) == 0x256);
C_ASSERT(offsetof(RdpConnection_guess, chan_queue) == 0x26C);
C_ASSERT(offsetof(RdpConnection_guess, msgid_valid) == 0x44C);
C_ASSERT(offsetof(RdpConnection_guess, recv_mask) == 0x460);
C_ASSERT(offsetof(RdpConnection_guess, remote) == 0x478);
C_ASSERT(offsetof(RdpConnection_guess, tx_acked) == 0x488);
C_ASSERT(offsetof(RdpConnection_guess, tx_sent_bits) == 0x494);
C_ASSERT(offsetof(RdpConnection_guess, tx_sent) == 0x518);
C_ASSERT(offsetof(RdpConnection_guess, tx_hold_bytes) == 0x568);
C_ASSERT(offsetof(RdpConnection_guess, bandwidth) == 0x56C);
C_ASSERT(offsetof(RdpConnection_guess, field_620) == 0x57C);
C_ASSERT(offsetof(RdpConnection_guess, rtt) == 0x588);
C_ASSERT(offsetof(RdpConnection_guess, field_764) == 0x6BC);
C_ASSERT(offsetof(RdpConnection_guess, connected) == 0x6D4);
C_ASSERT(offsetof(RdpConnection_guess, close_event) == 0x6F8);
C_ASSERT(offsetof(RdpConnection_guess, ttl_addr) == 0x700);
C_ASSERT(offsetof(RdpConnection_guess, ttl_index) == 0x738);
C_ASSERT(offsetof(RdpConnection_guess, ttl_index) + 4 == 0x73C);

/* connection_get_disconnect_info_guess's output (12 bytes); not checked in eqmain. */
typedef struct RdpDisconnectInfo_guess
{
    u32 code;               /* 0x00 */
    u8  reason;             /* 0x04 */
    u8  reason2;            /* 0x05 */
    u8  unknown06[2];       /* 0x06 */
    u32 value;              /* 0x08 */
} RdpDisconnectInfo_guess;
C_ASSERT(sizeof(RdpDisconnectInfo_guess) == 0xC);

/* eventq.c: the serial link's queue (rdp+0x140). The item layout is not checked in eqmain. */
typedef struct RdpSerialItem_guess
{
    u8  unknown00[0x24];    /* 0x00 */
    u32 size;               /* 0x24 */
    u32 time;               /* 0x28 */
} RdpSerialItem_guess;

typedef struct RdpSerialQueue_guess
{
    u8  lock[0x18];         /* 0x00  CRITICAL_SECTION */
    u32 value;              /* 0x18 */
    u8  unknown1C[0x1C];    /* 0x1C */
    RdpList_guess list;     /* 0x38  is_connection_inactive 0x10030410 reads its count at +0x48 (eqgame +0x40) */
    u32 queued_bytes;       /* 0x50  (eqgame 0x4C) after the list, as eqgame's (RdpState_guess's size agrees) */
} RdpSerialQueue_guess;
C_ASSERT(offsetof(RdpSerialQueue_guess, list) == 0x38);
C_ASSERT(sizeof(RdpSerialQueue_guess) == 0x54);

/* rdp.c: the rdp instance (one per socket), 0x1D8 bytes (rdp_create 0x1002A460 allocates it; eqgame 0x1AC).
 * 0x0C, 0x44, 0x50, 0x58 (the timeout heap), 0x6C and 0x84 are as eqgame's (rdp_resort, rdp_create); eqgame's
 * 0x110/0x114 are at 0x118/0x11C (rdp_create, SendPacket 0x1002CD70) and the serial queue at 0x140 (+0x28).
 * Only partly known. */
typedef struct RdpState_guess
{
    int     wsaStarted_guess;   /* 0x000* */
    int     udpSocket_guess;    /* 0x004* */
    int     icmpSocket_guess;   /* 0x008* */
    u32 socket;                     /* 0x00C */
    int     ipxSocket_guess;    /* 0x010* */
    RdpAddr_guess localAddr_guess;    /* 0x014* */
    RdpAddr_guess ttlAddr_guess;      /* 0x024* */
    u8      ipxAddr_guess[0xE]; /* 0x034* */
    u8      unknown0x042[2];    /* 0x042  */
    u32 defaultTtl;           /* 0x044 */
    int     ipxBroadcast_guess; /* 0x048* */
    int     udpBroadcast_guess; /* 0x04C* */
    RdpConnHash_guess hash;         /* 0x050 */
    RdpHeap_guess timeout_heap;       /* 0x058* the connections by deadline (same offset as eqgame's) */
    char    timeout_lock[0x18]; /* 0x06C* */
    int     wakeupPending_guess; /* 0x084* */
    int     destroyerFrees_guess; /* 0x088* */
    int     running_guess;      /* 0x08C* */
    void*   thread_guess;       /* 0x090* */
    u8      unknown0x094[0xC];  /* 0x094  */
    void*   messageSemaphore_guess; /* 0x0A0* */
    RdpList_guess messageList_guess;  /* 0x0A4* */
    char    messageLock_guess[0x18]; /* 0x0BC* */
    RdpList_guess readyList_guess;    /* 0x0D4* */
    u8      readyLock_guess[0x18]; /* 0x0EC* */
    u32     inputBytes_guess;   /* 0x104* */
    u32     skippedBytes_guess; /* 0x108* */
    u32     rateTime_guess;     /* 0x10C* */
    u32 input_rate;                 /* 0x110  (eqgame 0x108) input bytes per second: rdp.c:1166 writes it from
                                               inputBytes_guess / rateTime_guess; no eqmain function reads it */
    u32     skippedRate_guess;  /* 0x114* */
    int unk_0x118;                  /* 0x118  (eqgame 0x110) int (census int x4, tx.c:1310) */
    int unk_0x11C;                  /* 0x11C  (eqgame 0x114) int (census int x3) */
    char    connectionLock_guess[0x18]; /* 0x120* */
    u32     connectionCount_guess; /* 0x138* */
    u32     maxConnections_guess; /* 0x13C* */
    RdpSerialQueue_guess serial;    /* 0x140  (eqgame 0x118) check_connection_status 0x1002AFC0 */
    u8  unknown0x194[0x44];           /* 0x194 */
} RdpState_guess;
C_ASSERT(offsetof(RdpState_guess, hash) == 0x50);
C_ASSERT(offsetof(RdpState_guess, serial) == 0x140);
C_ASSERT(sizeof(RdpState_guess) == 0x1D8);
C_ASSERT(offsetof(RdpState_guess, wsaStarted_guess) == 0x000);
C_ASSERT(offsetof(RdpState_guess, udpSocket_guess) == 0x004);
C_ASSERT(offsetof(RdpState_guess, icmpSocket_guess) == 0x008);
C_ASSERT(offsetof(RdpState_guess, ipxSocket_guess) == 0x010);
C_ASSERT(offsetof(RdpState_guess, localAddr_guess) == 0x014);
C_ASSERT(offsetof(RdpState_guess, ttlAddr_guess) == 0x024);
C_ASSERT(offsetof(RdpState_guess, ipxAddr_guess) == 0x034);
C_ASSERT(offsetof(RdpState_guess, ipxBroadcast_guess) == 0x048);
C_ASSERT(offsetof(RdpState_guess, udpBroadcast_guess) == 0x04C);
C_ASSERT(offsetof(RdpState_guess, timeout_heap) == 0x058);
C_ASSERT(offsetof(RdpState_guess, timeout_lock) == 0x06C);
C_ASSERT(offsetof(RdpState_guess, wakeupPending_guess) == 0x084);
C_ASSERT(offsetof(RdpState_guess, destroyerFrees_guess) == 0x088);
C_ASSERT(offsetof(RdpState_guess, running_guess) == 0x08C);
C_ASSERT(offsetof(RdpState_guess, thread_guess) == 0x090);
C_ASSERT(offsetof(RdpState_guess, messageSemaphore_guess) == 0x0A0);
C_ASSERT(offsetof(RdpState_guess, messageList_guess) == 0x0A4);
C_ASSERT(offsetof(RdpState_guess, messageLock_guess) == 0x0BC);
C_ASSERT(offsetof(RdpState_guess, readyList_guess) == 0x0D4);
C_ASSERT(offsetof(RdpState_guess, readyLock_guess) == 0x0EC);
C_ASSERT(offsetof(RdpState_guess, inputBytes_guess) == 0x104);
C_ASSERT(offsetof(RdpState_guess, skippedBytes_guess) == 0x108);
C_ASSERT(offsetof(RdpState_guess, rateTime_guess) == 0x10C);
C_ASSERT(offsetof(RdpState_guess, skippedRate_guess) == 0x114);
C_ASSERT(offsetof(RdpState_guess, connectionLock_guess) == 0x120);
C_ASSERT(offsetof(RdpState_guess, connectionCount_guess) == 0x138);
C_ASSERT(offsetof(RdpState_guess, maxConnections_guess) == 0x13C);

/* ringbuf.c: a circular byte buffer; the ring functions are the same shape as eqgame's. */
typedef struct RdpRing_guess
{
    u8 *base;               /* 0x00 */
    u32 size;               /* 0x04 */
    u8 *rd;                 /* 0x08 */
    u8 *wr;                 /* 0x0C */
    u32 count;              /* 0x10 */
} RdpRing_guess;
C_ASSERT(sizeof(RdpRing_guess) == 0x14);

/* fast_malloc.c: one size class of the pool allocator, 0x3C apart (cleanupMemoryPool 0x10028850; eqgame
 * 0x38): the list is 4 bytes longer, so the lock follows at 0x24. */
typedef struct RdpPool_guess
{
    u32 block_size;         /* 0x00 */
    u32 payload;            /* 0x04 */
    u32 count;              /* 0x08 */
    RdpList_guess list;     /* 0x0C  free blocks */
    u8  lock[0x18];         /* 0x24  (eqgame 0x20) CRITICAL_SECTION */
} RdpPool_guess;
C_ASSERT(sizeof(RdpPool_guess) == 0x3C);

/* Rewritten rdp functions called from other rdp files, where eqmain's sibling has the same signature (the
 * list.c functions have none: eqmain's list API takes and returns items, not links). */
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
unsigned char bitarray_setbit_guess(unsigned char *bits, unsigned int n);
unsigned char clear_bit_and_return_previous_value_guess(unsigned char *bits, unsigned int n);
unsigned char getbit_guess(const unsigned char *bits, unsigned int n);
/* eventq.c */
u32 getDebugInfoFromCriticalSection_guess(RdpSerialQueue_guess *q);
int is_connection_inactive_guess(RdpSerialQueue_guess *q);
unsigned long calculateAdjustedClientTime_guess(RdpSerialQueue_guess *q);
unsigned long ascending_timeout_data_cmp_guess(RdpSerialQueue_guess *q);
/* uevent.c (HANDLE * parameters) */
void uevent_destroy_guess(void **ev);
void uevent_init_guess(void **ev);
int uevent_create_guess(void **ev);
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
void initializeDataPointer_guess(RdpMsgArrival_guess *msg);
int process_network_message_packet_guess(RdpMsgArrival_guess *msg, void *sender, RdpPacketInfo_guess *info, const void *data);
int ValidateFragmentConsistency_guess(RdpMsgArrival_guess *msg, RdpPacketInfo_guess *info);
/* rdp.c */
int check_connection_status_guess(RdpState_guess *rdp);
u32 getTimeFromEQClient_guess(RdpState_guess *rdp);
u32 rdp_get_serial_stall_time_guess(RdpState_guess *rdp);
void format_sockaddr_guess(char *out, const RdpAddr_guess *addr);
/* log.c (cdecl, varargs) */
void discard_log_append_guess(const char *format, ...);
/* rx.c */
RdpMsgArrival_guess *dequeue_message_by_sequence_guess(RdpConnection_guess *conn, u8 channel);
int validateAndUpdateMessageSequence_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg);
RdpMsgArrival_guess *clearAndReturnPendingMessage_guess(RdpConnection_guess *conn);
void markMessageForPendingProcessing_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg);
/* fast_malloc.c */
void fast_malloc_init_guess(u32 size);
void cleanupMemoryPool_guess(void);
void *fast_malloc_guess(u32 size);
void *allocate_memory_from_pool_guess(int idx);
void fast_free_guess(void *p);
/* ringbuf.c */
void initializeNetworkBufferHeader_guess(RdpRing_guess *ring);
int allocate_memory_buffer_guess(RdpRing_guess *ring, u32 size);
void freeRingBufferMemory_guess(RdpRing_guess *ring);
void read_network_data_into_buffer_guess(RdpRing_guess *ring, void *file);
void read_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n);
void copy_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n);

#endif /* EQMAIN_RDP_H */
