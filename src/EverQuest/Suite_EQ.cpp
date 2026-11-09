// Suite_EQ.cpp - free functions of the EverQuest/Suite_EQ subsystem.
// ORIGINAL FILE: Source/PC Source/EverQuest/Suite_EQ.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/globals.h"

// FUNCTION: TRILOGY 0x004c53c2
// Sets the dword at +0xEB8 of the object at 0x006E4E44 to 1 if that object exists; ignores ECX and its argument.
// ORIGINAL FILE: EverQuest/Suite_EQ.cpp (from Mac debug map)
// Callee pops one stack argument (RET 4) and never reads it or ECX. Sets the redraw flag of the UI object at
// 0x006E4E44.
void __stdcall SaveToUIIniFile_guess(int unused)
{
    if (g_eqmain_guess != NULL)
        *(s32*)(g_eqmain_guess + 0xEB8) = 1;
}
