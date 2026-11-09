// EQMoneyList.cpp - money actor list lookups.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMoneyList.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_actor_lists.h"

// FUNCTION: TRILOGY 0x004ad938
// Initializes a money list entry with the default sword actor definition.
// ORIGINAL FILE: EverQuest/EQMoneyList.cpp (from Mac debug map)
// EQMoneyList constructor (this in ECX; the header declares no constructor, so a __fastcall free function).
// Links the node in as the new list head (0x006E4D60; +0 next, +4 prev), sets the floats at +0x38..+0x4C
// (+0x3C = 20.0f), copies "SWORD_ACTORDEF" (0x0056015C) to +0x50 and zeroes the rest; returns this.
int* __fastcall EQMoneyList_guess(int *self)
{
    if (EQ_GLOBAL(int, 0x006E4D60) == 0)
    {
        EQ_GLOBAL(int, 0x006E4D60) = (int)self;
        self[1] = 0;
        self[0] = 0;
    }
    else
    {
        *(int *)EQ_GLOBAL(int, 0x006E4D60) = (int)self;
        self[1] = EQ_GLOBAL(int, 0x006E4D60);
        self[0] = 0;
        EQ_GLOBAL(int, 0x006E4D60) = (int)self;
    }
    *(float *)&self[0xE] = 0.0f;
    *(float *)&self[0xF] = 20.0f;
    *(float *)&self[0x10] = 0.0f;
    *(float *)&self[0x11] = 0.0f;
    *(float *)&self[0x12] = 0.0f;
    *(float *)&self[0x13] = 0.0f;
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char *)&self[0x14], EQ_ARRAY(char, 0x0056015C));
    self[5] = 0;
    self[4] = 0;
    self[3] = 0;
    self[2] = 0;
    self[6] = 0;
    self[0xC] = 0;
    self[0xD] = 0;
    return self;
}

// FUNCTION: TRILOGY 0x004ad9b0
// Removes a money list entry; the associated handle's cleanup purpose is unverified.
// ORIGINAL FILE: EverQuest/EQMoneyList.cpp (from Mac debug map)
// EQMoneyList destructor (this in ECX; the header declares no destructor, so a __fastcall free function).
// Unlinks the node (+0 prev, +4 next, list head 0x006E4D60), then deletes its actor at +0x34.
void __fastcall EQMoneyList_dtor_guess(int *self)
{
    if (self[0] != 0 && self[1] != 0)
    {
        *(int *)(self[0] + 4) = self[1];
        *(int *)self[1] = self[0];
    }
    else if (self[0] == 0 && self[1] != 0)
    {
        *(int *)self[1] = 0;
        EQ_GLOBAL(int, 0x006E4D60) = self[1];
    }
    else if (self[1] == 0 && self[0] != 0)
    {
        *(int *)(self[0] + 4) = 0;
    }
    else
    {
        self[1] = 0;
        self[0] = 0;
        EQ_GLOBAL(int, 0x006E4D60) = 0;
    }
    if (self[0xd] != 0)
        EQ_METHOD(1, void, 0x00411715, (void *, int), EQ_GLOBAL(void *, 0x006E4DE0))(self[0xd]);
}

// FUNCTION: TRILOGY 0x004ada11
// Search the list through its +4 link, comparing the key at +0x18.
int EQMoneyList::find_money_actor_guess(int actor)
{
    int money = EQ_GLOBAL(int, 0x006E4D60);
    while (true)
    {
        if (money == 0)
            return 0;
        if (*(int *)(money + 0x18) == actor)
            break;
        money = *(int *)(money + 4);
    }
    return money;
}

// FUNCTION: TRILOGY 0x004ada2d
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMoneyList.cpp (from Mac debug map)
// The list's actor pointer is at +0x34.
int EQMoneyList::is_money_actor(int actor)
{
    int money = EQ_GLOBAL(int, 0x006E4D60);
    while (true)
    {
        if (money == 0)
            return 0;
        if (*(int *)(money + 0x34) == actor)
            break;
        money = *(int *)(money + 4);
    }
    return money;
}
