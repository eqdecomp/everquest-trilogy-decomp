// SidlScreenWnd.cpp - Suite/Suite/SidlScreenWnd subsystem.
// ORIGINAL FILE: Suite/Suite/SidlScreenWnd.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/character.h"
#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x004a1147
// Advances the character's level, refreshes level-based stats and skills, and announces the gain.
// ORIGINAL FILE: Suite/Suite/SidlScreenWnd.cpp (from Mac debug map)
// __cdecl. Level-up handler: sets the experience floor of the new level, the level, the highest level, mana
// Always returns 1.
int HandleLevelUp_guess(CharacterProfile* pc, int level)
{
    char message[512];
    s16 hp;
    s32 mana;
    int exp;

    exp = EQ_METHOD(3, int, 0x004807FC, (CharacterProfile*, u8, u8, u16), pc)((u8)(level - 1), pc->class_, pc->race);
    pc->exp = exp + 1;
    pc->level = (u8)level;
    if ((u8)level > pc->highest_level)
        pc->highest_level = (u8)level;
    mana = EQ_METHOD(0, s32, 0x00419BCA, (CharacterProfile*), pc)();
    pc->mana = (s16)mana;
    hp = EQ_METHOD(1, s16, 0x00419A2F, (CharacterProfile*, int), pc)(0);
    pc->training_points = 0;
    pc->cur_hp = hp;
    EQ_CDECL(int, 0x005224FB, (char*, const char*, int))
        (message, EQ_ARRAY(char, 0x0056226C), level);
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
    EQ_METHOD(1, void, 0x004C53C2, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(1);
    EQ_CDECL(int, 0x004A11E3, (CharacterProfile*))(pc);
    return 1;
}
