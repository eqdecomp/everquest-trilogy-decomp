// LootWnd.cpp - CLootWnd, the old UI's loot window.
// ORIGINAL FILE: Source/PC Source/EverQuest/LootWnd.cpp (from Mac debug map)
// Field layout: unrecovered (displacements are what the code reads); all accesses stay raw offsets.

#include "trilogy/common.h"

/* No header declares CLootWnd yet; local declaration, member functions only. */
class CLootWnd
{
public:
    bool RequestLootSlot_guess();
};

// FUNCTION: TRILOGY 0x00434096
// Reports whether an unidentified loot-window state is active.
// ORIGINAL FILE: EverQuest/LootWnd.cpp (from Mac debug map)
bool CLootWnd::RequestLootSlot_guess()
{
    bool temp = (*((char*)this + 0xd6) == 1);
    return (bool)temp;
}
