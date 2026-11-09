#include <stdio.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/globals.h"
#include "trilogy/player.h"

class CInventoryWnd_guess
{
public:
    void Activate_guess(void);
};

// FUNCTION: TRILOGY 0x004d0a04
// Sends the entered broadcast message to channel 6 and displays it in local chat.
// ORIGINAL FILE: EverQuest/InventoryWnd.cpp (from Mac debug map)
void CInventoryWnd_guess::Activate_guess(void)
{
    char broadcastText[0x840];
    char* text;

    if (EQ_GLOBAL(int, 0x006E4E70) == 0 &&
        EQ_GLOBAL(int, 0x006E4E6C) == 0 &&
        EQ_GLOBAL(char, 0x006B63F8) == 0 &&
        DAT_006b6494 == 0)
    {
        text = (char*)this + 0xba8;
        EQ_CDECL(int, 0x005224FB, (char*, const char*, const char*, const char*))
            (broadcastText, EQ_ARRAY(char, 0x0056F454), g_local_player_guess->name, text);
        EQ_METHOD(3, void, 0x004D0D77, (void*, int, int, char*), (void*)this)(6, 0, text);
        EQ_CDECL(int, 0x005224FB, (char*, const char*, const char*))
            (broadcastText, EQ_ARRAY(char, 0x0056F440), text);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(broadcastText, 0x111);
    }
}
