/* guilds.h - EverQuest guild list helpers (Source/PC Source/EverQuest/guilds.cpp).
 * New file for the guilds code. No recovered layout changes.
 * C++ only (VC6).
 */
#ifndef TRILOGY_GUILDS_H
#define TRILOGY_GUILDS_H

#include "common.h"
#include "eqaddr.h"
#include "net_structs.h"

/* 0x006E5490 GuildListEntry[512], 0x3C bytes each. */
#define g_guilds_guess EQ_ARRAY(GuildListEntry, 0x006E5490)
/* 0x00575FA8 signed guild count (dword). */
#define g_guild_count_guess EQ_GLOBAL(int, 0x00575FA8)
/* 0x005588DC file mode string ("wb", mutable 3 bytes incl NUL). */
#define g_guild_file_mode_guess EQ_ARRAY(char, 0x005588DC)
/* 0x004E04A2 crc32mem (tested): u32 (void*, s32). */
#define crc32mem EQ_CDECL(u32, 0x004E04A2, (void *, s32))

u32 FindGuildIndexName(char *name); /* 0x004DFE29 */
int FindGuildIndex(u32 id); /* 0x004DFE8D */
int SaveGuildListFile_guess(void); /* 0x004DFEC4 */
u32 GuildFileChecksum(void); /* 0x004E00D9 */
int save_guilds_guess(int arg); /* 0x004DFF8B */
char *GetPlayerGuildName(short id); /* 0x004E00F6 */

#endif
