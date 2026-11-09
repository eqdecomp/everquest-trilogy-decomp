#include <stdio.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/everquest.h"
#include "trilogy/globals.h"
#include "trilogy/player.h"

// FUNCTION: TRILOGY 0x004a4c6d
// Submits a last-name change for a named player and reports it in chat.
// ORIGINAL FILE: EverQuest/SpellBookWnd.cpp (from Mac debug map)
void SendLastNameChange_guess(EQPlayer* unused, char* args)
{
    int color;
    char* text;
    char messageBuffer[128];
    char packet[94];
    char newLastName[40];
    char targetName[40];

    if (args == 0 || *args == '\0')
    {
        color = 0x0F;
        text = EQ_ARRAY(char, 0x00562798);
    }
    else
    {
        text = EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, targetName);
        text = EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(text, newLastName);
        EQ_CDECL(void, 0x005219C0, (char*, const char*))(packet, targetName);
        EQ_CDECL(void, 0x005219C0, (char*, const char*))
            (packet + 30, g_local_player_guess->name);
        EQ_CDECL(void, 0x005219C0, (char*, const char*))(packet + 60, newLastName);
        *(short*)(packet + 92) = 0;
        send_message_guess
            (g_world_connection_guess, 0x216E, packet, 0x5E, 1);
        EQ_CDECL(int, 0x005224FB, (char*, const char*, const char*, const char*))
            (messageBuffer, EQ_ARRAY(char, 0x005627C4), targetName, newLastName);
        text = messageBuffer;
        color = 0x0D;
    }
    CEverQuest_dsp_chat_guess(pinstCEverQuest_guess, 0, text, color);
}

// FUNCTION: TRILOGY 0x004cb72d
// Starts the 30 second camp countdown (OP_Camp) if sitting, or cancels a running one.
// ORIGINAL FILE: EverQuest/SpellBookWnd.cpp (from Mac debug map)
// @early-stop: register allocation and the thiscall wrapper still differ from the original.
void CEverQuest::PrepareToCamp_guess(void)
{
    if (g_local_player_guess->stand_state == 'f')
    {
        return;
    }
    if (EQ_GLOBAL(u32, 0x006B6590) != 0)
    {
        EQ_GLOBAL(u32, 0x006B6590) = 0;
        EQ_GLOBAL(u32, 0x006E4E8C) = 0;
        CEverQuest_print_chat_guess(pinstCEverQuest_guess, 0,
            EQ_ARRAY(char, 0x00568A3C));
    }
    else if (g_local_player_guess->stand_state != 'n')
    {
        CEverQuest_print_chat_guess(pinstCEverQuest_guess, 0,
            EQ_ARRAY(char, 0x0056CFB8));
    }
    else
    {
        send_message_guess
            (g_world_connection_guess, 0x2207, 0, 0, 1);
        u32 time = EQ_CDECL(u32, 0x004A461E, (void))();
        EQ_GLOBAL(u32, 0x006B6594) = time;
        time += 30000;
        EQ_GLOBAL(u32, 0x006B6590) = time;
        EQ_GLOBAL(u16, 0x006B65B4) = (u16)g_local_player_guess->cur_hp;
        EQ_GLOBAL(u8, 0x006B659C) = 1;
        CEverQuest_print_chat_guess(pinstCEverQuest_guess, 0,
            EQ_ARRAY(char, 0x0056CF80));
    }
    return;
}
