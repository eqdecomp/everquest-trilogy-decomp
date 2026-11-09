#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x004e04a2
// Calculates a CRC checksum for a block of game data.
// ORIGINAL FILE: EverQuest/memcrc.cpp (from Mac debug map)
u32 __cdecl crc32mem(void *data, s32 length)
{
    u32 crc;
    s32 i;
    s32 b;
    u32 index;

    crc = 0xFFFFFFFF;
    i = 0;
    if (length <= 0)
        return crc;
    do {
        b = ((s8 *)data)[i];
        i = i + 1;
        index = (u32)(b ^ (s32)crc) & 0xFF;
        crc = (crc >> 8) ^ EQ_ARRAY(u32, 0x0057616C)[index];
    } while (i < length);
    return crc;
}
