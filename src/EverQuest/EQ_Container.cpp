// EQ_Container.cpp - EQ_Container constructor, destructor and Close.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Container.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x00434040
// Prepares an empty, closed container for use.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Container.cpp (from Mac debug map)
// __thiscall. Clears the 10 contents pointers and the open flag; the rest of
// the object (the EQ_Item base and the other container bytes) is untouched.
// Returns this in EAX.
EQ_Container::EQ_Container()
{
    int i;

    for (i = 0; i < 10; i++)
        contents[i] = 0;
    open = 0;
}

// FUNCTION: TRILOGY 0x0043405a
// Releases the items held by a container during its destruction.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Container.cpp (from Mac debug map)
// __thiscall. Frees each non-null contents slot with the client's cdecl
// operator delete (0x005218BB), ascending, clearing the slot after the call.
EQ_Container::~EQ_Container()
{
    int i;

    for (i = 0; i < 10; i++)
    {
        if (contents[i] != 0)
            EQ_CDECL(void, 0x005218BB, (void*))(contents[i]);
        contents[i] = 0;
    }
}

// FUNCTION: TRILOGY 0x0043407e
// Marks a currently open container as closed.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Container.cpp (from Mac debug map)
// __thiscall. Closes an open container: when open is exactly 1, clears it and
// returns true; otherwise returns false. Only AL is meaningful (the upper
// bytes of EAX keep the address bits of &open; callers test AL only).
bool EQ_Container::Close_guess(void)
{
    if (open == 1)
    {
        open = 0;
        return true;
    }
    return false;
}
