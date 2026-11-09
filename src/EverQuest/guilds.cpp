// guilds.cpp - EverQuest guild list helpers.
// ORIGINAL FILE: Source/PC Source/EverQuest/guilds.cpp (from Mac debug map)

#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "trilogy/common.h"
#include "trilogy/net_structs.h"
#include "trilogy/guilds.h"

// FUNCTION: TRILOGY 0x004dfe29
// Finds the ID of a guild whose name starts with the supplied text.
// ORIGINAL FILE: Source/PC Source/EverQuest/guilds.cpp (from Mac debug map)
u32 FindGuildIndexName(char *name)
{
    int i;
    char *entry;

    if (name == 0)
        return 0xFFFFFFFF;
    if (strlen(name) > 30)
        return 0xFFFFFFFF;
    i = 0;
    if (i >= g_guild_count_guess)
        return 0xFFFFFFFF;
    for (entry = g_guilds_guess[0].name; i < g_guild_count_guess; i++, entry += 0x3C)
    {
        if (*(u32 *)(entry + 0x24) != 0)
        {
            if (!_strnicmp(entry, name, strlen(name)))
            {
                if (i == -1)
                    break;
                return g_guilds_guess[i].guild_id;
            }
        }
    }
    return 0xFFFFFFFF;
}

// FUNCTION: TRILOGY 0x004dfe8d
// Locates a guild roster entry by guild ID.
// ORIGINAL FILE: Source/PC Source/EverQuest/guilds.cpp (from Mac debug map)
int FindGuildIndex(u32 id)
{
    int result = -1;
    int n;
    int i;

    if ((int)id < 0)
        return result;
    n = g_guild_count_guess;
    for (i = 0; i < n; i++)
    {
        /* the original tests the dword at +0x28 (exists and the 3 bytes after it) */
        if (*(u32 *)&g_guilds_guess[i].exists != 0 && g_guilds_guess[i].guild_id == id)
        {
            result = i;
            break;
        }
    }
    return result;
}

// FUNCTION: TRILOGY 0x004dfec4
// Save the guild list to guilds2.dat.
// ORIGINAL FILE: EverQuest/guilds.cpp (inferred from mapped address neighbors)
int SaveGuildListFile_guess(void)
{
    char filename[256];
    FILE *f;
    int rc;

    sprintf(filename, EQ_ARRAY(char, 0x00575FF8));
    f = fopen(filename, g_guild_file_mode_guess);
    if (f == 0)
    {
        EQ_CDECL(int, 0x005229E8, (const char *, const char *))
            (EQ_ARRAY(char, 0x00575FE0), filename);
        return 0;
    }
    if (fwrite(&g_guild_count_guess, 4, 1, f) != 1)
    {
        EQ_CDECL(int, 0x005229E8, (const char *, const char *))
            (EQ_ARRAY(char, 0x00575FC8), filename);
        rc = 0;
    }
    else
    {
        rc = 1;
        int & n = g_guild_count_guess;

        if (n > 512)
        {
            n = 512;
        }
        if (n > 0)
        {
            if (fwrite(g_guilds_guess, 0x3C, n, f) != (u32)g_guild_count_guess)
            {
                EQ_CDECL(int, 0x005229E8, (const char *, const char *))
                    (EQ_ARRAY(char, 0x00575FB0), filename);
                rc = 0;
            }
        }
    }
    fclose(f);
    return rc;
}

// FUNCTION: TRILOGY 0x004dff8b
// Guild operation is unclear; retries an operation that takes an integer argument.
// ORIGINAL FILE: EverQuest/guilds.cpp (from Mac debug map)
int save_guilds_guess(int arg)
{
    int attempt;
    int result;

    attempt = 0;
    do
    {
        result = EQ_CDECL(int, 0x004DFEC4, (int))(arg);
        if (0 == result)
            {
        }
        else
            return 1;
        Sleep(100);
        attempt++;
    } while (attempt <= 9);
    return 0;
}

// FUNCTION: TRILOGY 0x004dffb3
// Guild operation is unclear; retries an operation with no explicit argument.
// ORIGINAL FILE: EverQuest/guilds.cpp (from Mac debug map)
int load_guilds(void)
{
    int result;
    int attempt;

    attempt = 0;
    while (1)
    {
        result = EQ_CDECL(int, 0x004DFFD8, (void))();
        if (result != 0)
            return 1;
        attempt++;
        if (attempt >= 10)
            break;
        Sleep(100);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004DFFD8
// Initialize the guild list and load cached guild records from guilds2.dat.
// ORIGINAL FILE: EverQuest/guilds.cpp (from Mac debug map)
// @early-stop: loop base entry+4 vs entry+0; constant 1 not kept in ebx; dead cmp after second fread missing
int load_guilds_ex(void)
{
    GuildListEntry *entry;
    int result;
    int guildCount;
    int readCount;
    char fileName[256];
    char statBuffer[36];
    FILE *file;

    g_guild_count_guess = 0x200;
    EQ_CDECL(int, 0x005224FB, (char *, const char *))
        (fileName, EQ_ARRAY(char, 0x00575FF8));
    entry = g_guilds_guess;
    do
    {
        *(u32 *)entry->unknown0x24 = 0xFFFFFFFF;
        entry->guild_id = 0xFFFFFFFF;
        entry->name[0] = '\0';
        *(u32 *)&entry->exists = 0;
        *(u32 *)entry->unknown0x34 = 0;
        *(u32 *)(entry->unknown0x34 + 4) = 0;
        *(u32 *)entry->unknown0x30 = 0xFFFFFFFF;
        *(u32 *)(entry->unknown0x29 + 3) = 0;
        entry++;
    } while ((int)entry < 0x006ECC90);

    result = EQ_CDECL(int, 0x00523BE0, (const char *, void *))(fileName, statBuffer);
    if (result != 0)
    {
        EQ_CDECL(int, 0x004DFF8B, (int))(-1);
    }
    else
    {
        file = EQ_CDECL(FILE *, 0x005223BC, (const char *, const char *))
            (fileName, EQ_ARRAY(char, 0x0053C2BC));
        if (file == 0)
        {
            EQ_CDECL(int, 0x005229E8, (const char *, const char *))
                (EQ_ARRAY(char, 0x00575FE0), fileName);
        }
        else
        {
            readCount = EQ_CDECL(size_t, 0x00522038, (void *, size_t, size_t, FILE *))
                (&guildCount, 4, 1, file);
            if (readCount != 1)
            {
                EQ_CDECL(int, 0x005229E8, (const char *, const char *))
                    (EQ_ARRAY(char, 0x00575FC8), fileName);
                EQ_CDECL(int, 0x00521FBB, (FILE *))(file);
            }
            else
            {
                guildCount = 0x200;
                EQ_CDECL(size_t, 0x00522038, (void *, size_t, size_t, FILE *))
                    (g_guilds_guess, 0x3C, guildCount, file);
                EQ_CDECL(int, 0x00521FBB, (FILE *))(file);
                g_guild_count_guess = 0x200;
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e00d9
// Creates a change-check value for guild roster data.
// ORIGINAL FILE: Source/PC Source/EverQuest/guilds.cpp (from Mac debug map)
u32 GuildFileChecksum(void)
{
    int n = g_guild_count_guess;
    u32 result = 0;

    if (n != 0)
        result = crc32mem(g_guilds_guess, (s32)((u32)n * 0x3C));
    return result;
}

// FUNCTION: TRILOGY 0x004e00f6
// Gets a guild's name for display, using "..." when no match exists.
// ORIGINAL FILE: EverQuest/guilds.cpp (from Mac debug map)
char *GetPlayerGuildName(short id)
{
    int index;

    index = EQ_CDECL(int, 0x004DFE8D, (int))((int)id);
    if (index == -1)
        return (char*)0x00576004;
    if (g_guilds_guess[index].name[0] == 0)
        return (char*)0x00576004;
    return g_guilds_guess[index].name;
}
