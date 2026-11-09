/* cypher.c - the rdp packet cipher (two DES-style rounds over 8-byte blocks, chained) and its CRC-32.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/cypher.c (from Mac debug map)
 *
 * The Mac map names rdp_cypher/rdp_encode/rdp_decode; the CRC-32 just below them (0x004F6380) is placed here
 * by address (the queue's graph label says rdp).
 * The tables and key schedules are the client's .data (read through their fixed addresses).
 * C. The rdp code was built for speed (see bitops.c).
 */
#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x0057EA20: the CRC-32 table (256 entries, zlib's polynomial). */
#define g_rdp_crc_table_guess   EQ_ARRAY(u32, 0x0057EA20)
/* 0x0057EE20..0x0057F520: eight 64-entry S-box/permutation tables. */
#define g_rdp_sp_tables_guess   EQ_ARRAY(RdpSpTable_guess, 0x0057EE20)
/* Key schedules (8 dwords each = 2 rounds x 4). The decoder's starts at the string "who's yo daddy?". */
#define g_rdp_decode_key_guess  EQ_ARRAY(u32, 0x0057F620)
#define g_rdp_encode_key_guess  EQ_ARRAY(u32, 0x0057F644)

typedef u32 RdpSpTable_guess[64];

#define RDP_SP(n, x)  (g_rdp_sp_tables_guess[n][(x) & 0x3f])

// FUNCTION: TRILOGY 0x004f6380
// ORIGINAL FILE: Network/rdp/cypher.c (address; queue label rdp)
// __cdecl. CRC-32 of `len` bytes continuing from `crc` (zlib-style: pre/post inverted, 8 bytes per loop).
u32 calculate_crc32_checksum_guess(u32 crc, const u8 *buf, u32 len)
{
    crc = ~crc;
    while (len >= 8)
    {
        len -= 8;
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
    }
    while (len)
    {
        crc = g_rdp_crc_table_guess[(crc & 0xff) ^ *buf++] ^ (crc >> 8);
        len--;
    }
    return ~crc;
}

// FUNCTION: TRILOGY 0x004f64c0
// @early-stop: VC6 swaps the byte-8 and byte-24 table lookups in each four-table group (16 instruction differences).
// ORIGINAL FILE: Network/rdp/cypher.c (Mac: rdp_cypher, low)
// __cdecl. Two Feistel rounds over block[0..1] with the 8-dword key schedule; the halves come out swapped.
void rdp_cypher_guess(u32 *block, const u32 *key)
{
    u32 l;
    u32 r;
    u32 t;
    u32 f;
    int i;

    l = block[0];
    r = block[1];
    i = 2;
    do
    {
        t = ((r << 28) | (r >> 4)) ^ *key++;
        f = RDP_SP(0, t >> 24) | RDP_SP(2, t >> 16) | RDP_SP(4, t >> 8) | RDP_SP(6, t);
        t = *key++ ^ r;
        f |= RDP_SP(1, t >> 24);
        f |= RDP_SP(3, t >> 16);
        f |= RDP_SP(5, t >> 8);
        f |= RDP_SP(7, t);
        l ^= f;
        t = ((l << 28) | (l >> 4)) ^ *key++;
        f = RDP_SP(0, t >> 24) | RDP_SP(2, t >> 16) | RDP_SP(4, t >> 8) | RDP_SP(6, t);
        t = *key++ ^ l;
        f |= RDP_SP(1, t >> 24);
        f |= RDP_SP(3, t >> 16);
        f |= RDP_SP(5, t >> 8);
        f |= RDP_SP(7, t);
        r ^= f;
    } while (--i);
    block[0] = r;
    block[1] = l;
}

// FUNCTION: TRILOGY 0x004f6630
// Encrypts outgoing RDP packet data for transmission.
// ORIGINAL FILE: Network/rdp/cypher.c (Mac: rdp_encode, high)
// __cdecl. Encrypts `count` 8-byte blocks in place, last to first, each block XORed with the already
// encrypted one after it before its own encryption.
void rdp_encode(u32 *buf, int count)
{
    u32 *p;
    u32 *word;

    count--;
    p = buf + count * 2;
    rdp_cypher_guess(p, g_rdp_encode_key_guess);
    if (count > 0)
    {
        do
        {
            p -= 2;
            word = p + 1;
            p[0] ^= p[2];
            word[0] ^= word[2];
            rdp_cypher_guess(p, g_rdp_encode_key_guess);
        } while (--count);
    }
}

// FUNCTION: TRILOGY 0x004f6680
// Decrypts incoming RDP packet data for processing.
// ORIGINAL FILE: Network/rdp/cypher.c (Mac: rdp_decode, high)
// __cdecl. Inverse of rdp_encode: decrypts first to last, un-XORing each block with the next (still
// encrypted) one.
void rdp_decode(u32 *buf, int count)
{
    count--;
    if (count > 0)
    {
        do
        {
            rdp_cypher_guess(buf, g_rdp_decode_key_guess);
            buf[0] ^= buf[2];
            buf += 2;
            buf[-1] ^= buf[1];
        } while (--count);
    }
    rdp_cypher_guess(buf, g_rdp_decode_key_guess);
}

// FUNCTION: TRILOGY 0x004F7760
// Sends a network packet assembled from segments, with optional CRC and encryption.
int SendPacketWithCRC_guess(u32 sock, int segments, int segmentCount, void *dest,
                            int encrypt, int addCrc)
{
    u32 packet[150];
    u32 length = 0;
    if ((u32)segmentCount > 0)
    {
        // {data pointer, byte count} pairs, copied back to back
        u32 *segment = (u32 *)(segments + 4);
        u32 remaining = segmentCount;
        do
        {
            u32 segmentSize = *segment;
            memcpy((u8 *)packet + length, (void *)segment[-1], segmentSize);
            length += segmentSize;
            segment += 2;
        } while (--remaining);
    }
    if (addCrc || encrypt)
    {
        u32 crc = EQ_CDECL(u32, 0x004F6380, (u32, void *, u32))(0, packet, length);
        // CRC is sent big-endian (htonl)
        *(u32 *)((u8 *)packet + length) = EQ_STDCALL(u32, 0x00500398, (u32))(crc);
        length += 4;
        if (encrypt)
        {
            // pad to a multiple of 8; low nibble of last byte = pad count
            int padded = (length + 8) & ~7;
            int padIndex = length;
            while (padIndex < padded)
            {
                ((u8 *)packet)[padIndex] = (u8)EQ_CDECL(int, 0x00522EAD, (void))();
                ++padIndex;
            }
            ((u8 *)packet)[padded - 1] =
                (u8)(padded - length) | (((u8 *)packet)[padded - 1] & 0xf0);
            EQ_CDECL(void, 0x004F6630, (u32 *, int))(packet, padded / 8);
            length = padded;
        }
    }
    if (EQ_STDCALL(int, 0x00500392, (u32, char *, int, int, void *, int))(
        sock, (char *)packet, length, 0, dest,
        0x10 /* sizeof(sockaddr_in) */) != length)
    {
        // 0x2733 = WSAEWOULDBLOCK (returns 5); any other error returns 1
        return EQ_STDCALL(int, 0x005003DA, (void))() == 0x2733 ? 5 : 1;
    }
    return 0;
}
