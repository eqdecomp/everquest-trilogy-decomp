// EQ_Equipment.cpp - EQ_Equipment predicates: weapon/scroll classification.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Equipment.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x00436099
// Initializes equipment with default Fist item properties.
// ORIGINAL FILE: EverQuest/EQ_Equipment.cpp (from Mac debug map)
EQ_Equipment* __fastcall EQ_Equipment_guess(EQ_Equipment* self)
{
    u16* pair;
    int count;
    int zero;

    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)self, EQ_ARRAY(char, 0x00558A08));
    zero = 0;
    self->damage = 2;
    self->stackable = (s8)zero;
    self->click_type = (s8)zero;
    self->magic = (s8)zero;
    self->delay = 20;
    self->range = (u8)zero;
    self->item_type = 13;
    *(u16*)((char*)self + 0xCC) = (u16)zero;
    self->material = (u8)zero;
    self->color = (u32)zero;
    self->unknown0xBC = (u8)zero;
    *(u8*)((char*)self + 0xBA) = (u8)zero;
    self->item.weight = (u8)zero;
    self->item.equipable_slots = (u32)zero;
    *(u32*)((char*)self + 0x8C) = (u32)zero;
    *(u8*)((char*)self + 0x7E) = (u8)zero;
    self->light = (u8)zero;
    *(f32*)((char*)self + 0x9C) = 0.0f;
    *(f32*)((char*)self + 0x98) = 0.0f;
    *(f32*)((char*)self + 0x94) = 0.0f;
    *(f32*)((char*)self + 0x90) = 0.0f;
    *(u16*)((char*)self + 0x86) = (u16)zero;
    *(u16*)((char*)self + 0x82) = (u16)zero;
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)self + 0x5F, EQ_ARRAY(char, 0x0053E4BC));
    self->click_effect_id |= 0xFFFF;
    *(u16*)((char*)self + 0x84) = (u16)zero;
    self->proc_spell_id = (u16)zero;
    count = (self->STR = (s8)zero) + 4;
    self->sell_rate_guess = 1.0f;
    self->STA = (s8)zero;
    self->CHA = (s8)zero;
    self->DEX = (s8)zero;
    self->INT = (s8)zero;
    self->AGI = (s8)zero;
    self->WIS = (s8)zero;
    self->MR = (s8)zero;
    self->FR = (s8)zero;
    self->CR = (s8)zero;
    self->DR = (s8)zero;
    self->PR = (s8)zero;
    self->HP = (s8)zero;
    self->click_effect_type = (u8)zero;
    self->level = (u8)zero;
    self->charges = (u8)zero;
    self->effect_type = (u8)zero;
    *(u16*)((char*)self + 0xDE) = (u16)zero;
    *(u16*)((char*)self + 0xE0) = (u16)zero;
    *(u32*)((char*)self + 0xF8) = (u32)zero;
    self->skill_mod_id = (u8)zero;
    self->skill_mod_percent = (s16)zero;
    *(u16*)((char*)self + 0x100) = (u16)zero;
    *(u16*)((char*)self + 0x102) = (u16)zero;
    self->baneDamage_guess = (u8)zero;
    *(u16*)((char*)self + 0x106) = (u16)zero;
    self->recommended_level = (s8)zero;
    *(u8*)((char*)self + 0x109) = (u8)zero;
    *(u8*)((char*)self + 0x10A) = (u8)zero;
    self->elementalDamageType_guess = (u8)zero;
    self->elementalDamage_guess = (u8)zero;
    pair = (u16*)((char*)self + 0x116);
    do
    {
        pair[-4] = (u16)zero;
        *pair = (u16)zero;
        pair++;
        count--;
    } while (count != 0);
    self->deity = (u32)zero;
    return self;
}

// FUNCTION: TRILOGY 0x0043624b
// ORIGINAL FILE: EverQuest/EQ_Equipment.cpp (from Mac debug map)
// __thiscall. Weapon unless the type is excluded (0x1B, 0x1C, 0x2B, 0x2C) or damage is zero.
int EQ_Equipment::IsWeapon(void)
{
    if (item_type == 0x1B)
        return 0;
    int cond = item_type == 0x1C;
    if (!cond)
        {
        int cond2 = item_type == 0x2B;
        if (cond2)
            return 0;
        if (item_type == 0x2C)
            return 0;
        return (damage != 0);
    }
    else
    return 0;
}

// FUNCTION: TRILOGY 0x00436270
// ORIGINAL FILE: EverQuest/EQ_Equipment.cpp (from Mac debug map)
// __thiscall. Scroll when the type is 0x14..0x16.
int EQ_Equipment::IsScroll(void)
{
    if (item_type >= 0x14 && item_type < 0x17)
        return 1;
    return 0;
}
