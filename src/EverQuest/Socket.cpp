#include <malloc.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/player.h"
#include "trilogy/rdp.h"

// FUNCTION: TRILOGY 0x004a4ebb
// Sends a request to boot the named player and reports the target in chat.
// @early-stop: merged cdecl cleanup (ADD 0x30 vs 0x24+0xC) and dummy EDX for dsp_chat
/* fake match: sprintf/dsp duplicated per branch to avoid promoting playerName to ESI */
void __cdecl kick_guess(int unused, char *playerName)
{
    int comparisonResult;
    char chatText[128];
    char messageData[64];

    comparisonResult = EQ_CDECL(int, 0x00522920, (const char *, const char *))
        (playerName, EQ_ARRAY(char, 0x0058FBA4));
    if (comparisonResult != 0)
    {
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(messageData, playerName);
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))
            (messageData + 30, EQ_GLOBAL(EQPlayer *, 0x006E4D8C)->name);
        EQ_CDECL(void, 0x004E1C45, (void **, u32, void *, u32, int))
            (&EQ_GLOBAL(void *, 0x006B3194), 0x206d, messageData, 0x3d, 1);
        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
            (chatText, EQ_ARRAY(char, 0x0056290C), playerName);
        EQ_METHOD(2, void, 0x004D055E, (void *, const char *, int), EQ_GLOBAL(void *, 0x006EFE2C))(chatText, 0xd);
    }
    else
    {
        if (EQ_GLOBAL(EQPlayer *, 0x006E4DAC) == (EQPlayer *)0)
            return;
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))
            (messageData, EQ_GLOBAL(EQPlayer *, 0x006E4DAC)->name);
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))
            (messageData + 30, EQ_GLOBAL(EQPlayer *, 0x006E4D8C)->name);
        EQ_CDECL(void, 0x004E1C45, (void **, u32, void *, u32, int))
            (&EQ_GLOBAL(void *, 0x006B3194), 0x206d, messageData, 0x3d, 1);
        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
            (chatText, EQ_ARRAY(char, 0x0056290C),
                EQ_GLOBAL(EQPlayer *, 0x006E4DAC)->name);
        EQ_METHOD(2, void, 0x004D055E, (void *, const char *, int), EQ_GLOBAL(void *, 0x006EFE2C))(chatText, 0xd);
    }
    return;
}

// FUNCTION: TRILOGY 0x004e1c45
// Sends a game message with optional reliability; client updates always use unreliable delivery.
// ORIGINAL FILE: EverQuest/Socket.cpp (from Mac debug map)
void __cdecl send_message_guess(void **connection, u32 opcode, void *data, u32 size, int reliable)
{
    void **connSlot;
    u32 dataSize;
    int result;

    dataSize = size;
    connSlot = connection;
    if (*connection != (void *)0)
    {
        // connection is reused as the send flag: 1 = reliable
        connection = (void **)0;
        if (reliable != 0)
            connection = (void **)1;
        // OP_ClientUpdate is never sent reliably
        if (opcode == 0x20f3)
            connection = (void **)0;

        {
            char *packet = (char *)_alloca(size + 4);
            EQ_CDECL(void *, 0x00521B10, (void *, const void *, u32))(packet, &opcode, 2);
            EQ_CDECL(void *, 0x00521B10, (void *, const void *, u32))(packet + 2, data, dataSize);
            result = EQ_CDECL(int, 0x004F3530, (void *, void *, u32, int, void *))
                (*connSlot, packet, dataSize + 2, 1, connection);
        }

        if ((result != 0) &&
            ((connection == (void **)1) || (result == 0xd) || (result == 0x10) ||
             (result == 6) || (result == 0x11)))
        {
            // result is reused for the remote address
            result = EQ_CDECL(int, 0x004F23D0, (void *))(*connSlot);
            // results unused: probably a log call that was compiled out
            EQ_STDCALL(char *, 0x00500386, (u32))(((RdpAddr_guess *)result)->addr);
            EQ_STDCALL(u16, 0x00500380, (u16))(((RdpAddr_guess *)result)->port);
            EQ_CDECL(void, 0x004F21D0, (void *, int, int, int))(*connSlot, 10000, 0, 0);
            *connSlot = (void *)0;
        }
    }
    return;
}
