/*
 * zone.h - zone-level data tables the client keeps in its globals (C, parsed by build_gdt.py via
 * trilogy.h). The tables themselves are reached through globals.h.
 */
#ifndef TRILOGY_ZONE_H
#define TRILOGY_ZONE_H

#include "common.h"

#pragma pack(push, 1)

/* One teleport destination, 0x18 bytes (g_teleport_table_guess, 0x006E3538; count at 0x006E4D50).
 * Verified against client: GetTPCoordsFromIndex (0x004C1862) steps the table by 0x18, matches the index
 * as a signed short at +0, copies the four floats and the dword. Field meaning from CDisplay::DoTeleport
 * (the only caller): not checked. */
typedef struct TeleportEntry_guess
{
    s16     index;              /* 0x00* */
    u8      unknown0x02[2];     /* 0x02  */
    f32     y;                  /* 0x04* */
    f32     x;                  /* 0x08* */
    f32     z;                  /* 0x0C* */
    f32     heading;            /* 0x10* */
    s32     zone_id;            /* 0x14* -1 when the index is not found */
} TeleportEntry_guess;          /* 0x18 */
C_ASSERT(sizeof(TeleportEntry_guess) == 0x18);
C_ASSERT(offsetof(TeleportEntry_guess, index) == 0x00);
C_ASSERT(offsetof(TeleportEntry_guess, zone_id) == 0x14);

#pragma pack(pop)

#endif /* TRILOGY_ZONE_H */
