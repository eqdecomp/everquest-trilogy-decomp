// eqmain: the same original object as trilogy's src/Network/rdp/bitops.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
// CFLAGS: /O2
/* bitops.c - fixed-size bit arrays of the RDP layer (acknowledgement windows: 2048 bits, MSB first).
 * ORIGINAL FILE: Source/PC Source/Network/rdp/bitops.c (from Mac debug map)
 *
 * C, like the rest of the rdp library. The rdp code was built for speed with intrinsics (like /O2: no EBP
 * frames, byte spills into argument slots, memset as rep stos), unlike the game code (/O1); the CFLAGS line below
 * gives every rdp file /O2 (same objects as the earlier optimize("tgy") + intrinsic pragmas).
 */
#include <string.h>

#include "eqmain/rdp.h"

// FUNCTION: EQMAIN 0x10030d50
// Resets the connection's packet tracking state during initialization.
// Cloned from trilogy bitarray_clear_guess 0x004F7C70 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Clears the 32 dwords (0x80 bytes) of the bit array and the dword at +0x80.
void bitarray_clear_guess(unsigned int *ba)
{
    memset(ba, 0, 0x80);
    ba[0x20] = 0;
}

// FUNCTION: EQMAIN 0x10030d70
// Prepares a selected range of packet tracking bits for acknowledgment processing.
// Cloned from trilogy copyBitfieldToBuffer_guess 0x004F7C90 (the same original code: BinDiff sibling, names/siblings/).
// Differs only in the array size: 0x400 bits here (0x800 in trilogy), the `MOV EAX,0x400` at 10030d7e.
// __cdecl. Copies `count` bytes of the 1024-bit array (0x80 bytes) starting at bit `bit` into `dst`, realigned to a
// byte boundary. Bytes past the end of the array (bit 1024) are zero; the last partial byte keeps only the bits
// still inside the array. Note: the in-range loop reads one byte past its last source byte (src[1]).
void copyBitfieldToBuffer_guess(const unsigned char *bits, unsigned char *dst, unsigned int bit,
                                unsigned int count)
{
    unsigned int n;
    const unsigned char *src;
    unsigned char shift;

    src = bits + (bit >> 3);
    n = (0x400 - bit) >> 3;
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

// FUNCTION: EQMAIN 0x10030e30
// Tracks an outgoing packet position in the connection's network state.
// Cloned from trilogy bitarray_setbit_guess 0x004F7D50 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Sets bit `n` (MSB first within each byte) and returns its previous value (0/1, in AL).
unsigned char bitarray_setbit_guess(unsigned char *bits, unsigned int n)
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

// FUNCTION: EQMAIN 0x10030e70
// Removes an acknowledged packet position from tracking and reports its prior state.
// Cloned from trilogy clear_bit_and_return_previous_value_guess 0x004F7D90 (the same original code: BinDiff sibling, names/siblings/).
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

// FUNCTION: EQMAIN 0x10030eb0
// Checks whether a packet position is present while validating acknowledgments.
// Cloned from trilogy getbit_guess 0x004F7DD0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Returns bit `n` (0/1, in AL).
unsigned char getbit_guess(const unsigned char *bits, unsigned int n)
{
    return (char)(bits[n >> 3] >> (7 - (n & 7))) & 1;
}
