// eqmain: the same original code as trilogy's src/Network/rdp/cypher.c (cloned function). The table macro
// below is this image's own (DAT_10048c6c in eqmain.dll), not eqgame.exe's: any trilogy table address is wrong here.
// CFLAGS: /O2
/* cypher.cpp - the RDP layer's CRC-32 (table-driven, pre- and post-inverted). Source file: unknown, the Mac map
 * has no entry for eqmain.dll 0x1002F580 (the trilogy sibling is Network/rdp/cypher.c).
 * C, like the rest of the rdp library (see bitops.c).
 */
#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include <string.h>

/* 0x10048C6C: the CRC-32 table, 256 dwords in eqmain.dll's .data. */
#define g_rdp_crc_table_guess   EQ_ARRAY(u32, 0x10048C6C)

/* 0x1004906C: the eight SP tables (g_rdp_sp_guess..DAT_1004976c), 64 dwords each; table n is at +n*0x100. */
#define g_rdp_sp_guess          EQ_ARRAY(u32, 0x1004906C)
#define RDP_SP_GUESS(n, x)      g_rdp_sp_guess[(n) * 64 + ((x) & 0x3f)]

/* 0x10049890: the key schedule rdp_encode_guess passes to rdp_cypher_guess (the string " daddy?" starts it).
 * The original pushes the address itself, so this is EQ_ARRAY (a pointer), not a dereference. */
#define g_rdp_encode_key_guess  EQ_ARRAY(const u32, 0x10049890)

/* 0x1004986C: the key rdp_decode_guess passes to rdp_cypher_guess (the string "..." itself). The original
 * pushes the address, like the encode key above, so this is EQ_ARRAY (a pointer), not a dereference. */
#define g_rdp_decode_key_guess  EQ_ARRAY(const u32, 0x1004986C)

// FUNCTION: EQMAIN 0x1002F580
// Calculates the CRC-32 integrity value attached to RDP network data.
// Cloned from trilogy calculate_crc32_checksum_guess 0x004F6380 (the same original code: BinDiff sibling, names/siblings/).
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

// FUNCTION: EQMAIN 0x1002F6C0
// Applies the keyed RDP cipher to one data block.
// @early-stop: VC6 schedules byte-8 table reads before byte-24 reads in each group (16 instruction differences).
// Cloned from trilogy rdp_cypher_guess 0x004F64C0 (BinDiff sibling: the same code, only the table addresses differ).
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
        f = RDP_SP_GUESS(0, t >> 24) | RDP_SP_GUESS(2, t >> 16) |
            RDP_SP_GUESS(4, t >> 8) | RDP_SP_GUESS(6, t);
        t = *key++ ^ r;
        f |= RDP_SP_GUESS(1, t >> 24);
        f |= RDP_SP_GUESS(3, t >> 16);
        f |= RDP_SP_GUESS(5, t >> 8);
        f |= RDP_SP_GUESS(7, t);
        l ^= f;
        t = ((l << 28) | (l >> 4)) ^ *key++;
        f = RDP_SP_GUESS(0, t >> 24) | RDP_SP_GUESS(2, t >> 16) |
            RDP_SP_GUESS(4, t >> 8) | RDP_SP_GUESS(6, t);
        t = *key++ ^ l;
        f |= RDP_SP_GUESS(1, t >> 24);
        f |= RDP_SP_GUESS(3, t >> 16);
        f |= RDP_SP_GUESS(5, t >> 8);
        f |= RDP_SP_GUESS(7, t);
        r ^= f;
    } while (--i);
    block[0] = r;
    block[1] = l;
}

// FUNCTION: EQMAIN 0x1002F830
// Protects outgoing RDP data before packet transmission.
// Cloned from trilogy rdp_encode_guess 0x004F6630 (BinDiff sibling: the same code, only the key and callee addresses differ).
// __cdecl. Encrypts `count` 8-byte blocks in place, last to first, each block XORed with the already
// encrypted one after it before its own encryption.
void rdp_encode_guess(u32 *buffer, int count)
{
    u32 *p;
    u32 *word;

    count--;
    p = buffer + count * 2;
    EQ_CDECL(void, 0x1002F6C0, (u32 *, const u32 *))(p, g_rdp_encode_key_guess);
    if (count > 0)
    {
        do
        {
            p -= 2;
            word = p + 1;
            p[0] ^= p[2];
            word[0] ^= word[2];
            EQ_CDECL(void, 0x1002F6C0, (u32 *, const u32 *))(p, g_rdp_encode_key_guess);
        } while (--count);
    }
}

// FUNCTION: EQMAIN 0x1002F880
// Restores RDP data from its protected packet form.
// Cloned from trilogy rdp_decode_guess 0x004F6680 (BinDiff sibling: the same code, only the key and callee addresses differ).
// __cdecl. Decrypts `count` 8-byte blocks in place, first to last, each block un-XORed with the next (still
// encrypted) one after it: the inverse of rdp_encode_guess.
void rdp_decode_guess(u32 *buf, int count)
{
    count--;
    if (count > 0)
    {
        do
        {
            EQ_CDECL(void, 0x1002F6C0, (u32 *, const u32 *))(buf, g_rdp_decode_key_guess);
            buf[0] ^= buf[2];
            buf += 2;
            buf[-1] ^= buf[1];
        } while (--count);
    }
    EQ_CDECL(void, 0x1002F6C0, (u32 *, const u32 *))(buf, g_rdp_decode_key_guess);
}

// FUNCTION: EQMAIN 0x1002F8D0
// Transmits assembled RDP data packets to their network destination.
// Cloned from trilogy SendPacketWithCRC_guess 0x004F7760 (BinDiff sibling: the same code, only the callee addresses differ).
// __cdecl. Copies the `count` (source, size) pairs at segments back to back into a 600-byte buffer. With includeChecksum or
// encrypt, appends the CRC-32 (through htonl); with encrypt also pads to a multiple of 8 with rand() bytes and encrypts.
// sendto() sends the buffer: 0 when every byte went out, else 5 (WSAEWOULDBLOCK) or 1.
int SendPacketWithCRC_guess(u32 socket, int segments, int segmentCount, void *destination,
                            int encrypt, int includeChecksum)
{
    u32 buffer[150];
    u32 length = 0;
    if ((u32)segmentCount > 0)
    {
        u32 *segment = (u32 *)(segments + 4);
        u32 count = segmentCount;
        do
        {
            u32 size = *segment;
            memcpy((u8 *)buffer + length, (void *)segment[-1], size);
            length += size;
            segment += 2;
        } while (--count);
    }
    if (includeChecksum || encrypt)
    {
        u32 crc = EQ_CDECL(u32, 0x1002F580, (u32, void *, u32))(0, buffer, length);
        *(u32 *)((u8 *)buffer + length) = EQ_STDCALL(u32, 0x10032296, (u32))(crc);
        length += 4;
        if (encrypt)
        {
            int padded = (length + 8) & ~7;
            int i = length;
            while (i < padded)
            {
                ((u8 *)buffer)[i] = (u8)EQ_CDECL(int, 0x100360BF, (void))();
                ++i;
            }
            ((u8 *)buffer)[padded - 1] =
                (u8)(padded - length) | (((u8 *)buffer)[padded - 1] & 0xf0);
            EQ_CDECL(void, 0x1002F830, (u32 *, int))(buffer, padded / 8);
            length = padded;
        }
    }
    if (EQ_STDCALL(int, 0x10032290, (u32, char *, int, int, void *, int))(
        socket, (char *)buffer, length, 0, destination, 0x10) != length)
    {
        return EQ_STDCALL(int, 0x100322E4, (void))() == 0x2733 ? 5 : 1;
    }
    return 0;
}
