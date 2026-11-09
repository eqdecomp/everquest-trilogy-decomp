/* bitops.c - fixed-size bit arrays of the RDP layer (acknowledgement windows: 2048 bits, MSB first).
 * ORIGINAL FILE: Source/PC Source/Network/rdp/bitops.c (from Mac debug map)
 *
 * C, like the rest of the rdp library. The rdp code was built for speed with intrinsics (like /O2: no EBP
 * frames, byte spills into argument slots, memset as rep stos), unlike the game code (/O1); the CFLAGS line below
 * gives every rdp file /O2 (same objects as the earlier optimize("tgy") + intrinsic pragmas).
 */
#include <string.h>

#include "trilogy/rdp.h"

// CFLAGS: /O2

// FUNCTION: TRILOGY 0x004f7c70
// ORIGINAL FILE: Network/rdp/bitops.c (Mac: bitarray_clear, low)
// __cdecl. Clears all 2048 bits and the trailing count.
void bitarray_clear_guess(RdpBitArray_guess *ba)
{
    memset(ba->bits, 0, sizeof(ba->bits));
    ba->count = 0;
}

// FUNCTION: TRILOGY 0x004f7c90
// Provides acknowledgement-window bytes for RDP packet processing.
// ORIGINAL FILE: Network/rdp/bitops.c (address: between Mac-mapped neighbours)
// __cdecl. Copies `count` bytes of the 2048-bit array starting at bit `bit` into `dst`, realigned to a byte
// boundary. Bytes past the end of the array (bit 2048) are zero; the last partial byte keeps only the bits
// still inside the array. Note: the in-range loop reads one byte past its last source byte (src[1]).
void copyBitfieldToBuffer_guess(const unsigned char *bits, unsigned char *dst, unsigned int bit,
                                unsigned int count)
{
    unsigned int n;
    const unsigned char *src;
    unsigned char shift;

    src = bits + (bit >> 3);
    n = (0x800 - bit) >> 3;
    shift = (unsigned char)(bit & 7);
    if (n > count)
        n = count;
    count -= n;
    while (n--)
    {
        *dst++ = (unsigned char)((src[0] << shift) | (src[1] >> (8 - shift)));
        src++;
    }
    if (count)
    {
        count--;
        *dst++ = (unsigned char)(*src << shift);
    }
    while (count--)
        *dst++ = 0;
}

// FUNCTION: TRILOGY 0x004f7d50
// ORIGINAL FILE: Network/rdp/bitops.c (Mac: bitarray_setbit, high)
// __cdecl. Sets bit `n` (MSB first within each byte) and returns its previous value (0/1, in AL).
unsigned char bitarray_setbit(unsigned char *bits, unsigned int n)
{
    unsigned char *p;
    unsigned char shift;
    unsigned char old;
    unsigned char previous;

    p = bits + (n >> 3);
    shift = (unsigned char)(7 - (n & 7));
    old = *p;
    previous = (char)(old >> shift) & 1;
    *p |= (unsigned char)(1 << shift);
    return previous;
}

// FUNCTION: TRILOGY 0x004f7d90
// ORIGINAL FILE: Network/rdp/bitops.c (address: between Mac-mapped neighbours)
// __cdecl. Clears bit `n` and returns its previous value (0/1, in AL).
unsigned char clear_bit_and_return_previous_value_guess(unsigned char *bits, unsigned int n)
{
    unsigned char *p;
    unsigned char shift;
    unsigned char previous;

    p = bits + (n >> 3);
    shift = (unsigned char)(7 - (n & 7));
    previous = (unsigned char)(*p >> shift) & 1;
    *p = (unsigned char)(*p & ~(1 << shift));
    return previous;
}

// FUNCTION: TRILOGY 0x004f7dd0
// ORIGINAL FILE: Network/rdp/bitops.c (Mac: getbit, low; TAKP: bitarray_test_bit)
// __cdecl. Returns bit `n` (0/1, in AL).
unsigned char getbit_guess(const unsigned char *bits, unsigned int n)
{
    return (char)(bits[n >> 3] >> (7 - (n & 7))) & 1;
}
