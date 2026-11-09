/*
 * eqnamegen.h - EverQuest name generation helpers (Mac: Source/PC Source/EverQuest/EQNameGen.cpp).
 *
 * Free __cdecl helpers; layouts for the banned-word rows are fixed client
 * addresses (see EQNameGen.cpp).
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQNAMEGEN_H
#define TRILOGY_EQNAMEGEN_H

#include "common.h"

u8 __cdecl IsNameVowel_guess(char c);                                   /* 0x004AF5E7 */
u8 __cdecl NameFailsOnBasicChecks(char *name);                     /* 0x004AF621 */
u8 __cdecl NameContainsBannedWord_guess(const char *name, int length);  /* 0x004AF6F9 */
bool __cdecl NameContains_guess(char *haystack, char *needle, char flag); /* 0x004AF747 */
u8 __cdecl NameContains_guess(char *name, short category);                /* 0x004AF7D7 */
u8 __cdecl CreateName3_guess(void);                                       /* 0x004AF841 */
void __cdecl HearBabel(char *source, char *destination, u8 chance); /* 0x004AFF13 */

#endif /* TRILOGY_EQNAMEGEN_H */
