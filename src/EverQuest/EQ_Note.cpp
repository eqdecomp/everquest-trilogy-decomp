// EQ_Note.cpp - EQ_Note initialiser.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Note.cpp (from Mac debug map)

#include <string.h>
#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x0044ff75
// Initializes the note item's server text-file name as empty.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Note.cpp (from Mac debug map)
// __thiscall (EQ_Note has no member declaration in item.h, so a __fastcall free function).
// Copies the string at 0x0058FBA4 (an empty string) into file; returns this.
EQ_Note* __fastcall EQ_Note_guess(EQ_Note* note)
{
    strcpy(note->file, EQ_ARRAY(char, 0x0058FBA4));
    return note;
}
