// EQHSprite.cpp - EQHSprite list nodes.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQHSprite.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// Layout guessed from this destructor alone (no header yet): a doubly linked node whose list head is
// the global at 0x006E4D68, followed by 0x4A owned heap blocks at 0x30.
struct EQHSprite
{
    EQHSprite* prev_guess;          // 0x00
    EQHSprite* next_guess;          // 0x04
    u8 unknown0x08[0x28];           // 0x08
    void* blocks_guess[0x4A];       // 0x30

    ~EQHSprite();
};

// FUNCTION: TRILOGY 0x004a015f
// Cleans up one list entry and its allocated data; its game-specific role is unclear.
// ORIGINAL FILE: EverQuest/EQHSprite.cpp (from Mac debug map)
// __thiscall. Unlinks the node from the 0x006E4D68 list, then frees each non-null block with the
// client's cdecl operator delete (0x005218BB), clearing the slot after the call.
EQHSprite::~EQHSprite()
{
    int i;

    if (prev_guess != 0 && next_guess != 0)
    {
        prev_guess->next_guess = next_guess;
        next_guess->prev_guess = prev_guess;
    }
    else if (prev_guess == 0 && next_guess != 0)
    {
        next_guess->prev_guess = 0;
        EQ_GLOBAL(EQHSprite*, 0x006E4D68) = next_guess;
    }
    else if (next_guess == 0 && prev_guess != 0)
    {
        prev_guess->next_guess = 0;
    }
    else
    {
        next_guess = 0;
        prev_guess = 0;
        EQ_GLOBAL(EQHSprite*, 0x006E4D68) = 0;
    }

    for (i = 0; i < 0x4A; i++)
    {
        if (blocks_guess[i] != 0)
            EQ_CDECL(void, 0x005218BB, (void*))(blocks_guess[i]);
        blocks_guess[i] = 0;
    }
}
