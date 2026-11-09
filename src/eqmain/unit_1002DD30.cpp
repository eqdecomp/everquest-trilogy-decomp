// unit_1002DD30.cpp - eqmain.dll, the address run 0x1002DD30..0x1002DE10 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1002DD30
// Returns the dword at +0x6d4 of a record. __cdecl; the record's layout is unknown.
unsigned int __cdecl GetRecordDword6D4_guess(unsigned char* record)
{
    return *(unsigned int*)(record + 0x6d4);
}

// FUNCTION: EQMAIN 0x1002DD40
// Updates an unidentified record; its role in the game is unknown.
// Clears the word at +0x18 and the dword at +0x8 of a record, stores value at +0x12. __cdecl; the record's layout is unknown.
void __cdecl SetRecordWord12_guess(unsigned char* record, unsigned short value)
{
    *(unsigned short*)(record + 0x18) = 0;
    *(unsigned short*)(record + 0x12) = value;
    *(unsigned int*)(record + 0x8) = 0;
}
