// ORIGINAL FILE: EverQuest/ChannelServerApi.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/soundmanager.h"

class ChannelServerApi
{
public:
    void SendMessageA_guess(void* param_1, char* param_2);
};

// FUNCTION: TRILOGY 0x004e387d
// Builds a zone sound-bank filename, calls the sound manager, then invokes its stop-all routine.
// ORIGINAL FILE: EverQuest/ChannelServerApi.cpp (from Mac debug map)
void ChannelServerApi::SendMessageA_guess(void* ptr, char* zoneName)
{
    char fileName[100];

    if (EQ_GLOBAL(int, 0x006E4DD0) != 0)
    {
        EQ_CDECL(void, 0x004E38D0, (void))();
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(fileName, zoneName);
        EQ_CDECL(char*, 0x005219D0, (char*, const char*))(fileName, EQ_ARRAY(char, 0x0057BF24));
        EQ_METHOD(1, void, 0x004E3953, (ChannelServerApi*, char*), this)(fileName);
        if (EQ_GLOBAL(int, 0x006E4DDC) != 0)
        {
            EQ_METHOD(0, void, 0x004E25FD, (EqSoundManager_guess*), (EqSoundManager_guess*)EQ_GLOBAL(void*, 0x006E4DDC))();
        }
    }
    return;
}
