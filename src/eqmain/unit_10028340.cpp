// unit_10028340.cpp - eqmain.dll, the address run 0x10028340..0x10028750 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include "eqmain/rdp.h"

// The list and its node, as this function uses them (the rest of each is unknown). Not RdpList_guess /
// RdpLink_guess: the node's links are at 0x00 (prev), 0x08 (next), not 0x00/0x04.
struct ListNode3_guess;

struct ListHead3_guess
{
    ListNode3_guess *head;          // 0x00
    ListNode3_guess *tail;          // 0x04
    int count;                      // 0x08
};

struct ListNode3_guess
{
    ListNode3_guess *prev;          // 0x00
    int data;                       // 0x04  returned by the unlink
    ListNode3_guess *next;          // 0x08
};

// FUNCTION: EQMAIN 0x10028340
// Adds a client entry at the tail of its list or after a selected entry.
// @early-stop: similarity 0.5349; remaining differences are mainly list/node register allocation and branch cleanup.
// __cdecl, void (callers never read EAX). Allocates a 12-byte node and stores `data` in its middle dword.
// A null `after` appends it at the tail (a null tail makes it the head too). Otherwise it goes after
// `after`: node->next takes after->next, and the original stores that same value back into after->next,
// so after->next never points at the node (bugs included). No checks: a bad pointer faults.
void __cdecl list3_insert_guess(ListHead3_guess *list, ListNode3_guess *after, int value)
{
    ListNode3_guess *node;
    ListNode3_guess *tail;

    node = (ListNode3_guess *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x0c);
    if (node != 0)
    {
        node->data = value;
        if (after != 0)
        {
            node->prev = after;
            node->next = after->next;
            after->next = node->next;
            if (node->next != 0)
            {
                node->next->prev = node;
                list->count = list->count + 1;
                return;
            }
        }
        else
        {
            tail = list->tail;
            node->next = 0;
            node->prev = tail;
            if (tail != 0)
            {
                tail->next = node;
                list->count = list->count + 1;
                list->tail = node;
                return;
            }
            list->head = node;
        }
        list->count = list->count + 1;
        list->tail = node;
    }
}

// FUNCTION: EQMAIN 0x100283B0
// Adds a client entry before a selected entry, or at the head when none is selected.
// @early-stop: list/data load order and neighbor/count block layout still differ.
// __cdecl, 3 stack args. Allocates a 12-byte node, stores `data`, links it before `before` (or at the head
// when `before` is null), increments the count, and returns the node; allocation failure returns null.
// The tail changes only when the list was empty. No checks on `before`: a bad pointer faults.
ListNode3_guess *list3_insert_before_guess(ListHead3_guess *list, ListNode3_guess *before, int value)
{
    ListNode3_guess *node;
    ListNode3_guess *prev;

    node = (ListNode3_guess *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0xc);
    if (node != 0)
    {
        node->data = value;
        if (before != 0)
        {
            prev = before->prev;
            node->next = before;
            node->prev = prev;
            if (prev != 0)
            {
                prev->next = node;
                node->next->prev = node;
                list->count = list->count + 1;
                return node;
            }
            list->head = node;
            node->next->prev = node;
            list->count = list->count + 1;
            return node;
        }
        node->prev = 0;
        node->next = list->head;
        list->head = node;
        if (node->next != 0)
        {
            node->next->prev = node;
            list->count = list->count + 1;
            return node;
        }
        list->tail = node;
        list->count = list->count + 1;
    }
    return node;
}

// FUNCTION: EQMAIN 0x10028430
// Removes one client entry from its list.
// __cdecl. Unlinks a node from a list of three-dword nodes, frees it, drops the list's count and returns
// the node's data dword (read before the free). No checks: a bad node faults.
int __cdecl list3_unlink_free_guess(ListHead3_guess *list, ListNode3_guess *node)
{
    int data;

    data = node->data;
    if (node->prev != 0)
    {
        node->prev->next = node->next;
    }
    else
    {
        list->head = node->next;
    }
    if (node->next != 0)
    {
        node->next->prev = node->prev;
    }
    else
    {
        list->tail = node->prev;
    }
    EQ_CDECL(void, 0x10033568, (void *))(node);
    list->count = list->count - 1;
    return data;
}

// FUNCTION: EQMAIN 0x10028480
// __cdecl. Calls list3_unlink_free_guess on the head node until the list's count is 0. Nothing is returned.
void list3_free_all_guess(ListHead3_guess *list)
{
    while (list->count != 0)
    {
        EQ_CDECL(int, 0x10028430, (ListHead3_guess *, ListNode3_guess *))(list, list->head);
    }
}

// FUNCTION: EQMAIN 0x100284B0
// Decodes a compressed buffer (input length cut to 16 bits); true when the decoder reports an error.
// __cdecl, returns bool. Calls lzo1x_decompress_guess (0x100284E0) with `in`, `inputLength & 0xffff`, `out`, and `&out`:
// the decoder's out_len lands in this function's own `out` parameter slot, not in the caller's variable. The original
// also pushes a 0 fifth argument that the callee never reads. 1 when the decoder returns nonzero (an error), else 0.
bool lzo1x_decompress_failed_guess(const unsigned char *in, unsigned int inputLength, unsigned char *out)
{
    int status;

    status = EQ_CDECL(int, 0x100284E0, (const unsigned char *, int, unsigned char *, int *, int))(
        in, inputLength & 0xffff, out, (int *)&out, 0);
    return status != 0;
}

// FUNCTION: EQMAIN 0x100284E0
// Unpacks a compressed byte stream into a caller-sized buffer; what game data it carries is not shown here.
// __cdecl, returns int. LZO1X-style decompressor (name is a guess from the token layout: literal runs, short and
// long back-references, and an end marker that is a back-reference of distance 0). Decodes `in_len` bytes at `in`
// into `out` (no output bound: the caller sizes it) and stores the byte count in `*out_len` (0 first). Returns 0
// when the marker ends exactly at `in + in_len`, -8 when input is left over, -4 when the stream ran past it.
// The labels are the original's block addresses, and the blocks stay in address order.
int lzo1x_decompress_guess(const unsigned char *in, int in_len, unsigned char *out, int *out_len)
{
    const unsigned char *ip;        // ESI: input
    const unsigned char *ip_end;    // EBP
    unsigned char *op;              // EAX: output
    const unsigned char *m;         // EDI: back-reference source
    unsigned int t;                 // ECX: token, then the run length
    unsigned int d;                 // EDX: scratch

    *out_len = 0;
    ip = in;
    ip_end = in + in_len;
    op = out;
    t = *ip;
    if (t <= 0x11)
        goto next_token;
    t -= 0x11;
    ip++;
    if (t < 4)
        goto match_next;
    do
    {
        *op++ = *ip++;
    } while (--t != 0);
    goto first_literal_run;

next_token:
    t = *ip;
    ip++;
    if (t >= 0x10)
        goto match;
    if (t != 0)
        goto copy_literals;
    if (*ip != 0)
        goto literal_len_byte;
    do
    {
        d = ip[1];
        t += 0xff;
        ip++;
    } while (d == 0);
literal_len_byte:
    d = *ip;
    ip++;
    t += d + 0xf;
copy_literals:
    *(unsigned int *)op = *(const unsigned int *)ip;
    ip += 4;
    op += 4;
    if (--t == 0)
        goto first_literal_run;
    if (t < 4)
        goto literal_tail_bytes;
literal_copy4:
    *(unsigned int *)op = *(const unsigned int *)ip;
    t -= 4;
    op += 4;
    ip += 4;
    if (t >= 4)
        goto literal_copy4;
    if (t == 0)
        goto first_literal_run;
    do
    {
        *op++ = *ip++;
    } while (--t != 0);
    goto first_literal_run;
literal_tail_bytes:
    do
    {
        *op++ = *ip++;
    } while (--t != 0);

first_literal_run:
    t = *ip;
    ip++;
    if (t >= 0x10)
        goto match;
    m = op - (t >> 2) - (*ip << 2) - 0x801;
    ip++;
    *op = *m;
    op++;
    m++;
short_match_copy:
    *op = *m;
    op[1] = m[1];
    op += 2;
match_done:
    t = ip[-2] & 3;
    if (t == 0)
        goto next_token;
match_next:
    do
    {
        *op++ = *ip++;
    } while (--t != 0);
    t = *ip;
    ip++;
match:
    if (t < 0x40)
        goto try_m3_match;
    m = op - ((t >> 2) & 7) - (*ip << 3) - 1;
    ip++;
    t = (t >> 5) - 1;
copy_match:
    *op = *m;
    op[1] = m[1];
    op += 2;
    m += 2;
    do
    {
        *op++ = *m++;
    } while (--t != 0);
    goto match_done;
try_m3_match:
    if (t < 0x20)
        goto try_m4_match;
    t &= 0x1f;
    if (t != 0)
        goto m3_distance;
    if (*ip != 0)
        goto m3_len_byte;
    do
    {
        d = ip[1];
        t += 0xff;
        ip++;
    } while (d == 0);
m3_len_byte:
    d = *ip;
    ip++;
    t += d + 0x1f;
m3_distance:
    m = op - (*(const unsigned short *)ip >> 2) - 1;
    ip += 2;
    goto long_match_copy;
try_m4_match:
    if (t < 0x10)
        goto m1_match;
    m = op - ((t & 8) << 11);
    t &= 7;
    if (t != 0)
        goto m4_distance;
    if (*ip != 0)
        goto m4_len_byte;
    do
    {
        d = ip[1];
        t += 0xff;
        ip++;
    } while (d == 0);
m4_len_byte:
    d = *ip;
    ip++;
    t += d + 7;
m4_distance:
    d = *(const unsigned short *)ip >> 2;
    ip += 2;
    m -= d;
    if (m == op)
        goto eof_found;
    m -= 0x4000;
long_match_copy:
    if (t < 6)
        goto copy_match;
    if ((int)op - (int)m < 4)
        goto copy_match;
    *(unsigned int *)op = *(const unsigned int *)m;
    m += 4;
    op += 4;
    t -= 2;
    do
    {
        *(unsigned int *)op = *(const unsigned int *)m;
        t -= 4;
        op += 4;
        m += 4;
    } while (t >= 4);
    if (t == 0)
        goto match_done;
    do
    {
        *op++ = *m++;
    } while (--t != 0);
    goto match_done;
m1_match:
    m = op - (t >> 2) - (*ip << 2) - 1;
    ip++;
    goto short_match_copy;
eof_found:
    *out_len = (int)(op - out);
    if (ip == ip_end)
        return 0;
    return (-(unsigned int)(ip < ip_end) & 0xfffffffc) - 4;
}

// FUNCTION: EQMAIN 0x10028720
// Formats the client version and build date into the front-end version string.
// No arguments, so __cdecl and __stdcall are the same bytes (the caller would clean up either way); the
// bundle's __stdcall label is a guess. Formats "v1.2 Aug 20 2001" into the global buffer at 0x1013C8E4
// with sprintf (cdecl, 5 pushed args) and returns that buffer.
char *version_string_guess(void)
{
    EQ_CDECL(int, 0x10034117, (char *, char *, ...))(EQ_ARRAY(char, 0x1013C8E4), EQ_ARRAY(char, 0x10048A30), 1, 2, EQ_ARRAY(char, 0x10048A3C));
    return EQ_ARRAY(char, 0x1013C8E4);
}

typedef void (__stdcall *InitializeCriticalSection_t)(void *lock);   // the IAT slot's type (0x100400A8)

// FUNCTION: EQMAIN 0x10028750
// Initializes the client allocation pool used for front-end data blocks.
// __cdecl, void. Starts the fast_malloc pool (RdpPool_guess, include/eqmain/rdp.h): stores `size` at 0x1013C9E4,
// then sets up the four size classes at 0x1013D720 (0x3C apart): block size 0x248 halved per class, the payload
// (block size - 8), and the lock, initialized through the IAT slot at 0x100400A8. One malloc of
// ceil(size / 0x248) * 0x248 + 8 bytes goes on the list at 0x1013D820; each 0x248-byte piece of it goes on the
// free list of class 3 (0x1013D7E0) and is counted in 0x1013D7DC. The last loop's sum is never read.
void initMemoryPool_guess(unsigned size)
{
    unsigned blockSize;             // uVar5: the class's block size, then halved
    unsigned total;                 // uVar5 again: the rounded size
    unsigned off;                   // uVar6
    unsigned sum;                   // ECX in the last loop
    unsigned lock;                  // p_Var4 / lpCriticalSection: a class's lock (0x24 into its RdpPool_guess).
                                    // Integers, not pointers: VC6 strength-reduces a pointer's +const uses into a
                                    // second loop register, which the original does not have.
    unsigned buf;                   // pvVar2
    unsigned blocks;                // buf + 8: the first 0x248-byte piece, after the 8-byte header. Computed once here:
                                    // a +8 inside the loop makes VC6 keep a second loop register for it.
    u32 *count;                     // puVar3
    InitializeCriticalSection_t initCs;

    EQ_GLOBAL(unsigned, 0x1013C9E4) = size;
    EQ_CDECL(void, 0x1002DEA0, (void *, u32, u32, u32))(EQ_ARRAY(void, 0x1013D820), 0, 0, 0);
    initCs = EQ_GLOBAL(InitializeCriticalSection_t, 0x100400A8);
    blockSize = 0x248;
    lock = (unsigned)EQ_ARRAY(u8, 0x1013D834);
    do {
        lock -= 0x3c;
        EQ_CDECL(void, 0x1002DEA0, (void *, u32, u32, u32))((void *)(lock - 0x18), 0, 0, 0);
        initCs((void *)lock);
        *(u32 *)(lock - 0x24) = blockSize;
        *(u32 *)(lock - 0x20) = (blockSize - 8) & 0xfffffff8;
        blockSize = (blockSize >> 1) & 0x7ffffff8;
    } while (lock != (unsigned)EQ_ARRAY(u8, 0x1013D744));

    total = ((unsigned)(size % 0x248 != 0) + size / 0x248) * 0x248;
    buf = (unsigned)EQ_CDECL(void *, 0x1003342E, (u32))(total + 8);
    EQ_CDECL(void, 0x1002DF20, (void *, void *))(EQ_ARRAY(void, 0x1013D820), (void *)buf);
    blocks = buf + 8;
    off = 0;
    if (total > 0) {
        do {
            EQ_CDECL(void, 0x1002DF20, (void *, void *))(EQ_ARRAY(void, 0x1013D7E0), (void *)(off + blocks));
            EQ_GLOBAL(u32, 0x1013D7DC) = EQ_GLOBAL(u32, 0x1013D7DC) + 1;
            off += 0x248;
        } while (off < total);
    }
    sum = 0;
    count = EQ_ARRAY(u32, 0x1013D728);
    do {
        sum = (sum >> 1) + *count;
        count += 0xf;
    } while (count < EQ_ARRAY(u32, 0x1013D818));
}
