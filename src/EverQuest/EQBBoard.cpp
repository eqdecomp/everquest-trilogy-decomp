// EQBBoard.cpp - EQBBoard bulletin-board list.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQBBoard.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_bboard.h"

// FUNCTION: TRILOGY 0x0049062d
// Adds a bulletin-board entry initialized from a supplied record or left empty.
// ORIGINAL FILE: EverQuest/EQBBoard.cpp (from Mac debug map)
EQBBoard::EQBBoard(const void* record)
{
    void* head;

    head = g_bboard_head_guess;
    if (head == NULL)
    {
        g_bboard_head_guess = this;
        *(void**)((char*)this + 4) = NULL;
        *(void**)this = NULL;
    }
    else
    {
        *(void**)head = this;
        head = *(void* volatile*)&g_bboard_head_guess;
        *(void**)((char*)this + 4) = head;
        *(void**)this = NULL;
        g_bboard_head_guess = this;
    }

    if (record == NULL)
    {
        *(u32*)((char*)this + 8) = 0;
        *(u8*)((char*)this + 0x0C) = 0;
        *(u8*)((char*)this + 0x1C) = 0;
        *(u8*)((char*)this + 0x3A) = 0;
        *(u8*)((char*)this + 0x3B) = 0;
    }
    else
    {
        *(u32*)((char*)this + 8) = *(u32*)((const char*)record + 8);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            ((char*)this + 0x0C, (const char*)record + 0x0C);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            ((char*)this + 0x1C, (const char*)record + 0x1C);
        *(u8*)((char*)this + 0x3A) = *(u8*)((const char*)record + 0x3A);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            ((char*)this + 0x3B, (const char*)record + 0x3B);
    }
}

// FUNCTION: TRILOGY 0x004906b1
// Adjacent list destructor; uses the same 0x006E4D70 head as the Mac-mapped constructor above.
EQBBoard::~EQBBoard()
{
    int *self;

    self = (int *)this;
    if (self[0] != 0 && self[1] != 0)
    {
        *(int *)(self[0] + 4) = self[1];
        *(int *)self[1] = self[0];
    }
    else if (self[0] == 0 && self[1] != 0)
    {
        *(int *)self[1] = 0;
        g_bboard_head_guess = (void *)self[1];
    }
    else if (self[1] == 0 && self[0] != 0)
    {
        *(int *)(self[0] + 4) = 0;
    }
    else
    {
        self[1] = 0;
        self[0] = 0;
        g_bboard_head_guess = 0;
    }
}
