// EQ_Character.cpp - EQ_Character, the character object behind the local player (stats, skills, saves,
// the character creation tables).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (from Mac debug map)

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/player.h"
#include "trilogy/character.h"
#include "trilogy/eq_character.h"
#include "trilogy/globals.h"
#include "trilogy/everquest.h"
#include "trilogy/net_structs.h"
#include "trilogy/layout/eq_main.h"

// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char* formatString, ...);

// FUNCTION: TRILOGY 0x0041976f
// Applies an HP adjustment during damage, regeneration, and other health updates.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::ModifyCurHP, low)
// __thiscall, 2 args (the second unused). Adds delta to the current HP, clamped to a short, unless the
// character's spawn is an NPC (type 1).
void EQ_Character::ModifyCurHP_guess(int delta, int unused)
{
    if (player == 0 || player->type != 1)
    {
        int hp = cur_hp + delta;
        if (hp > 0x7FFF)
            hp = 0x7FFF;
        if (hp < -0x8000)
            hp = -0x8000;
        cur_hp = (s16)hp;
    }
}

// FUNCTION: TRILOGY 0x004197a7
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// __thiscall. Bit 0 of the expansion flags (0xAE0, read as a signed byte).
int EQ_Character::ExpansionFlag1_guess(void)
{
    if (*(s8*)&expansions_guess & 1)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004197b2
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// __thiscall. Bit 1 of the expansion flags.
u32 EQ_Character::ExpansionFlag2_guess(void)
{
    if (*(s8*)&expansions_guess & 2)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004197bf
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// __thiscall. Bit 2 of the expansion flags.
u32 EQ_Character::ExpansionFlag4_guess(void)
{
    if (*(s8*)&expansions_guess & 4)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004197cd
// Maps supported skill IDs to special-skill IDs used in spell fizzle handling.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::normal_to_special, low)
// static (__cdecl). Skill id -> its "special" counterpart: 4 -> 43, 5 -> 44, 14 -> 45, 18 -> 46,
// 24 -> 47, anything else 0.
int EQ_Character::normal_to_special(int skill)
{
    switch (skill)
    {
    case 4:
        return 0x2B;
    case 5:
        return 0x2C;
    case 14:
        return 0x2D;
    case 18:
        return 0x2E;
    case 24:
        return 0x2F;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004197ff
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::~EQ_Character, low)
// Clears the spawn's pointer back to this character.
EQ_Character::~EQ_Character()
{
    if (player != 0)
        player->char_info_guess = 0;
}

// FUNCTION: TRILOGY 0x0041980e
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::IsSpellcaster, low)
// __thiscall. False for classes 1, 7 and 9 (warrior, monk, rogue).
int EQ_Character::IsSpellcaster(void)
{
    u8 c = class_;
    if (c == 1)
    {
        return 0;
    }
    else
    {
        if (c == 7)
        {
            return 0;
        }
        else
        {
            return c != 9;
        }
    }
}

// FUNCTION: TRILOGY 0x00419826
// Sets the character's light level from the brightest light source in their inventory.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::DoLight, low)
// __thiscall, 1 arg (unused). The spawn's light = the brightest light value of the equipment items in
// inventory slots 1..29.
void EQ_Character::DoLight(int unused)
{
    u8 light = 0;
    int i;

    for (i = 1; i < 30; i++)
    {
        EQ_Item* item = inventory_items[i];
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT && ((EQ_Equipment*)item)->light > light)
            light = ((EQ_Equipment*)item)->light;
    }
    player->light = light;
}

// The header does not declare this recovered method yet. Keep a layout-compatible local type so the
// rewrite retains a real __thiscall member signature without changing the shared header.
class EQ_Character_guess : public CharacterProfile
{
public:
    u32 Skill(u32 skill);
    s16 ac_guess(int cap);
    u16 compute_tohit(u8 skill);
    s16 offense_guess(u8 skill);
    s16 defense_agility_bonus(void);
    short compute_defense(void);
    u8 BaseAttr_guess(short param_1, u32 param_2, u32 param_3);
    int Sta_guess(void);
    int Cha_guess(void);
    int Dex_guess(void);
    int Int_guess(void);
    int Agi_guess(void);
    void InitMyLanguages(void);
};

// FUNCTION: TRILOGY 0x00419867
// Effective skill value: trained level within the class cap, at least the racial innate, plus worn-item % mods.
// @early-stop: inventory loop pointer/counter in ESI/ECX swapped; innate call loads ECX after the skill push
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
u32 EQ_Character_guess::Skill(u32 skill)
{
    u8 curLevel;
    EQPlayer* pEVar2;
    u32 limit;
    u32 acc;

    acc = (u32)skills[skill];
    pEVar2 = player;
    if (((pEVar2 != 0) && (pEVar2->type == '\0')) && (pEVar2->gm == '\0'))
    {
        curLevel = level;
        acc = curLevel;
        if (curLevel < highest_level)
        {
            limit = highest_level;
            acc = curLevel + 2;
            if ((int)limit < (int)acc)
                acc = limit;
        }
        limit = EQ_CDECL(u32, 0x004A0CA1, (u32, u16, u32, CharacterProfile*))(
            acc, class_, skill, this);
        if (skills[skill] < (int)limit)
            acc = skills[skill];
        else
            acc = limit;
        limit = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), (EQ_Character*)this)(skill, race);
        if ((int)acc <= (int)limit)
            acc = limit;
    }
    if (player->type == '\0')
    {
        int slotsLeft = 0x14;
        EQ_Item** ppEVar5 = &inventory_items[1];
        do
        {
            EQ_Item* pEVar3 = *ppEVar5;
            if (((pEVar3 != 0) && (pEVar3->type == EQ_ITEM_EQUIPMENT)) &&
                (((EQ_Equipment*)pEVar3)->skill_mod_id == skill &&
                 (((EQ_Equipment*)pEVar3)->skill_mod_percent != 0)))
            {
                acc = acc + ((int)((EQ_Equipment*)pEVar3)->skill_mod_percent * (int)acc) / 100;
            }
            ppEVar5 = ppEVar5 + 1;
            slotsLeft = slotsLeft + -1;
        } while (slotsLeft != 0);
    }
    if (0xfc < (int)acc)
        acc = 0xfc;
    return acc;
}

// FUNCTION: TRILOGY 0x00419956
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::HasSpell, low)
// __thiscall. 1 when the spell is in the spell book.
int EQ_Character::HasSpell(short spell)
{
    int i;

    for (i = 0; i < 256; i++)
    {
        if (spell_book[i] == spell)
            return 1;
    }
    return 0;
}

class EQ_Character_SetBaseHPToMaxHPDifference_guess_class : public EQ_Character
{
public:
    void SetBaseHPToMaxHPDifference_guess(int unused);
};

// FUNCTION: TRILOGY 0x0041997d
// Adjusts stored health so equipment HP bonuses still produce maximum health.
void EQ_Character_SetBaseHPToMaxHPDifference_guess_class::SetBaseHPToMaxHPDifference_guess(int unused)
{
    int item;
    s16 maxHp;
    EQ_Item** inventoryItemPtr;
    int remainingCount;
    int hpBonus = 0;

    remainingCount = 0x14;
    inventoryItemPtr = inventory_items + 1;
    do
    {
        item = (int)*inventoryItemPtr;
        if (item != 0 && *(char*)(item + 0x81) == '\0')
            hpBonus = hpBonus + *(s8*)(item + 0xb8);
        inventoryItemPtr = inventoryItemPtr + 1;
        remainingCount = remainingCount - 1;
    } while (remainingCount != 0);
    maxHp = (s16)EQ_METHOD(1, int, 0x00419A2F, (EQ_Character*, int), this)(unused);
    cur_hp = maxHp - hpBonus;
    return;
}

class EQ_Character_Cur_HP_guess_class : public EQ_Character
{
public:
    int Cur_HP_guess(int unused);
};

// FUNCTION: TRILOGY 0x004199c0
// Provides current health after worn HP bonuses for health checks and display.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, 1 arg (unused). Current HP = cur_hp + the signed +0xB8 bytes of the non-null items
// among the 20 pointers at +0xE4 whose +0x81 byte is 0. Writes it to the spawn (player->cur_hp) if
// there is one, calls EQ_Character_SetBaseHPToMaxHPDifference_guess when it is above Max_HP_guess,
// and returns it capped at 0x7FFF.
int EQ_Character_Cur_HP_guess_class::Cur_HP_guess(int unused)
{
    int item;
    int* slot;
    int sum;
    int count;
    int hp;
    int max_hp;

    sum = 0;
    count = 0x14;
    slot = (int*)((char*)this + 0xe4);
    do
    {
        item = *slot;
        if ((item != 0) && (*(char*)(item + 0x81) == '\0'))
            sum += *(char*)(item + 0xb8);
        slot++;
        count--;
    } while (count != 0);
    hp = cur_hp + sum;
    if (player != 0)
        player->cur_hp = hp;
    max_hp = EQ_METHOD(1, int, 0x00419A2F, (EQ_Character*, int), this)(unused);
    if (hp > max_hp)
    {
        EQ_METHOD(1, void, 0x0041997D, (EQ_Character*, int), this)(unused);
    }
    if (hp > 0x7fff)
    {
        hp = 0x7fff;
    }
    return hp;
}

class EQ_Character_Max_HP_guess : public EQ_Character
{
public:
    int Max_HP_guess(int unused);
};

// FUNCTION: TRILOGY 0x00419a2f
// Determines maximum health from class, level, stamina, spell effects, and equipment.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
/* fake match: case 4 re-assigns baseRate from inventoryCount and staHp is split out, to get the original's register use */
int EQ_Character_Max_HP_guess::Max_HP_guess(int unused)
{
    u8 levelValue;
    EQ_Item** inventorySlot;
    EQ_Equipment* equipment;
    int inventoryCount;
    int baseRate;
    int maxHp;
    int staHp;
    int itemHpBonus;

    itemHpBonus = 0;
    inventoryCount = 0x14;
    baseRate = 0x14;
    switch (class_)
    {
    case 3:
    case 5:
        levelValue = level;
        baseRate = 0x15;
        if (0x22 < levelValue)
            baseRate = 0x16;
        if (0x2c < levelValue)
            baseRate = baseRate + 1;
        if (0x32 < levelValue)
            baseRate = baseRate + 1;
        if (0x37 < levelValue)
            baseRate = baseRate + 1;
        if (levelValue > 0x3b)
            baseRate = baseRate + 1;
        break;
    case 1:
        levelValue = level;
        if (levelValue < 0x14)
            baseRate = 0x16;
        else if (levelValue < 0x1e)
            baseRate = 0x17;
        else if (levelValue < 0x28)
            baseRate = 0x19;
        else if (levelValue < 0x35)
            baseRate = 0x1b;
        else if (levelValue < 0x39)
            baseRate = 0x1c;
        else
            baseRate = levelValue < 0x3c ? 0x1d : 0x1e;
        break;
    case 4:
        baseRate = inventoryCount;
        if (0x39 < level)
            baseRate = 0x15;
        break;
    case 7:
    case 8:
    case 9:
    case 0xf:
        levelValue = level;
        baseRate = 0x12;
        if (0x32 < levelValue)
            baseRate = 0x13;
        if (levelValue > 0x39)
            baseRate = baseRate + 1;
        break;
    case 2:
    case 6:
    case 0xa:
        baseRate = 0xf;
        break;
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
        baseRate = 0xc;
        break;
    }
    inventorySlot = &inventory_items[1];
    do
    {
        equipment = (EQ_Equipment*)*inventorySlot;
        if ((equipment != 0) && (equipment->item.type == 0))
            itemHpBonus = itemHpBonus + equipment->HP;
        inventorySlot = inventorySlot + 1;
        inventoryCount = inventoryCount + -1;
    } while (inventoryCount != 0);
    maxHp = (u32)level * (int)(short)baseRate;
    staHp = (int)(EQ_METHOD(0, u8, 0x0041B258, (EQ_Character*), this)() * maxHp) / 300;
    maxHp += (int)EQ_METHOD(1, s16, 0x004238A2, (EQ_Character*, u8), this)(0x45) + staHp
        + itemHpBonus;
    if (player != 0 && player->type == 0)
        maxHp = maxHp + 5;
    if (player != 0)
        player->max_hp = maxHp;
    if (0x7fff < maxHp)
        maxHp = 0x7fff;
    return maxHp;
}

class EQ_Character_Cur_Mana_guess : public EQ_Character
{
public:
    s16 Cur_Mana_guess(void);
};

// FUNCTION: TRILOGY 0x00419ba3
// Keeps current mana within the character's current mana capacity.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
s16 EQ_Character_Cur_Mana_guess::Cur_Mana_guess(void)
{
    s32 max_mana = EQ_METHOD(0, s32, 0x00419BCA, (EQ_Character*), this)();

    if (mana > (s16)max_mana)
        mana = (s16)max_mana;
    else if (mana < 0)
        mana = 0;

    return mana;
}

/* EQ_Character has no declaration for this recovered member in eq_character.h yet.  The
 * source-local derived type keeps its layout and thiscall ABI while avoiding a header change. */
class EQ_Character_Max_Mana_guess : public EQ_Character
{
public:
    short Max_Mana_guess(void);
};

// FUNCTION: TRILOGY 0x00419bca
// Determines mana capacity from class, level, caster stats, spell effects, and gear.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// @early-stop: the stat's movzx is scheduled after push ebx / mov ebx (the original zero-extends it right after the imul; 3 instructions)
short EQ_Character_Max_Mana_guess::Max_Mana_guess(void)
{
    short bonusMana = 0;
    u8 class_id = class_;
    int class_value = (int)class_id;
    u32 level_value = (u32)level;
    bool clamp_level = false;
    EQ_Item** item_pointer;
    EQ_Item* item;
    int slotsLeft;
    u8 level_override;
    int stat_value;
    int baseMana;
    int truncBase;

    switch (class_value)
    {
    case 1:
    case 7:
    case 9:
    case 0x10:
    case 0x20:
        return 0;
    case 3:
    case 4:
    case 5:
    case 0x0f:
        clamp_level = 1;
        break;
    }

    /* classes 3..5 and 15 treat a level of 8 or less as 0. */
    if (clamp_level && (int)(level_value - 8) <= 0)
        level_value = 0;

    item_pointer = inventory_items + 1;
    slotsLeft = 0x14;
    do
    {
        item = *item_pointer;
        if (item != 0 && item->type == '\0')
        {
            bonusMana += ((EQ_Equipment*)item)->MANA;
        }
        item_pointer++;
        slotsLeft--;
    } while (slotsLeft != 0);

    level_override = unknown0x0A46;
    if (level_override != 0 && (int)level_override > (int)level_value &&
        player != 0 && player->type == 1)
        level_value = (u32)level_override;

    if (class_id < 2 ||
        (4 < class_id && class_id != 6 && class_id != 10 && class_id != 0x0f))
    {
        stat_value = EQ_METHOD(0, u32, 0x0041B5CC, (EQ_Character*), this)();
    }
    else
    {
        stat_value = EQ_METHOD(0, u32, 0x0041B88A, (EQ_Character*), this)();
    }

    baseMana = level_value * 0x0f;
    stat_value = (u8)stat_value;
    if (200 < stat_value)
        stat_value = 200;
    if (0 < (int)(stat_value - 100))
        stat_value = stat_value + (int)((stat_value - 100) * 3) / 2;

    baseMana += (int)((int)(short)baseMana * stat_value) / 200;
    truncBase = EQ_METHOD(1, int, 0x004238A2, (EQ_Character*, int), this)(0x61);
    bonusMana = bonusMana + truncBase;
    truncBase = (int)(short)baseMana;
    if ((short)bonusMana * 2 > truncBase)
        bonusMana = truncBase / 2;
    return bonusMana + baseMana;
}

// FUNCTION: TRILOGY 0x00419cf5
// Identifies intelligence-caster classes for armor calculations.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// static (__cdecl), called from EQ_Character::ac. 1 for classes 11..14 (necromancer, wizard, magician,
// enchanter), 0 otherwise or without a character.
int EQ_Character::IsIntCaster_guess(EQ_Character* ch)
{
    if (ch != 0 && ch->class_ >= 11 && ch->class_ <= 14)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x0041a47d
// Supplies race-based innate starting levels for specific skills.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range; TAKP: GetInherentRacialSkillLevel)
// __thiscall, 2 args (HasSkill passes the race as the second; unused). The racial innate level of a skill
// (0, 50 or 100).
/* fake match: the >= 100 exit is `else return 0`; an early return drops its xor, `else level = 0` moves the 8/94 tail */
u8 EQ_Character::GetInnateSkill_guess(u8 skill, int race_unused)
{
    u8 level = 0;

    if (skill < 100)
    {
        switch (race)
        {
        case 12:
            if (skill == 57)
                level = 50;
            break;
        case 128:
            if (skill == 27)
                level = 50;
            else if (skill == 50)
                level = 100;
            break;
        case 130:
            if (skill == 39)
            {
                level = 100;
                break;
            }
        case 11:
        case 81:
            if (skill == 42 || skill == 29)
                level = 50;
            break;
        case 4:
        case 112:
            if (skill == 27)
            {
                level = 50;
                break;
            }
        case 6:
        case 77:
        case 98:
            if (skill == 29)
                level = 50;
            break;
        case 8:
        case 94:
            if (skill == 40)
                level = 50;
            break;
        }
    }
    else
        return 0;
    return level;
}

// FUNCTION: TRILOGY 0x0041a76d
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::basesave_magic, low)
// __thiscall. Base magic resist: 25, 30 for races 3 and 8; +10 for an NPC spawn above level 24.
int EQ_Character::basesave_magic(void)
{
    int save = 25;

    if (race == 3 || race == 8)
        save = 30;
    if (player != 0 && player->type == 1 && level > 24)
        save += 10;
    return save;
}

// FUNCTION: TRILOGY 0x0041a7a0
// Computes total magic resistance for a level comparison, including gear and spell effects.
// @early-stop: volatile save keeps the score in its frame slot, but VC6 then loads/adds/stores each += (the original folds it to add [mem]); the 0x1e constant load and the else-if null test are placed differently
int EQ_Character::GetMagicResist_guess(int other_level)
{
    int found = 0;
    volatile int save = 25;   /* fake match: the original keeps the score in its frame slot, not in a register */

    if (this->race == 3 || this->race == 8)
        save = 30;

    EQPlayer* player = this->player;
    if (player != 0)
    {
        if (player->type == 1 && this->level > 0x18)
            save += 10;

        if (player != 0 && player->actor_info != 0
            && *((int*)((char*)player->actor_info + 0x2dc)) == 0x1e)
        {
            u8 level = this->level;
            save += 3;
            if (0x22 < level)
                save++;
            if (0x28 < level)
                save++;
            if (0x2d < level)
            {
                int bonus = level - 0x2d;
                if (5 < bonus)
                    bonus = 5;
                save += bonus;
            }
        }
    }

    {
        EQ_Item** item_ptr = &this->inventory_items[1];
        int i = 0x14;
        do
        {
            EQ_Item* item = *item_ptr;
            if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
                save += ((EQ_Equipment*)item)->MR;
            item_ptr++;
            i--;
        } while (i != 0);
    }

    {
        EQ_Item** slot_ptr;
        int slot;
        for (slot = 0x16, slot_ptr = &this->inventory_items[0x16]; slot <= 0x1d; slot++, slot_ptr++)
        {
            EQ_Item* item = *slot_ptr;
            if (item != 0)
            {
                if (item->type == EQ_ITEM_EQUIPMENT)
                {
                    if (((EQ_Equipment*)item)->item_type == 0x0e)
                    {
                        save += ((EQ_Equipment*)item)->MR;
                        break;
                    }
                }
                else if (*slot_ptr != 0 && (*slot_ptr)->type == EQ_ITEM_CONTAINER)
                {
                    EQ_Container* container = (EQ_Container*)item;
                    int count = container->slots;
                    int inner_slot;
                    for (inner_slot = 0; inner_slot < count; inner_slot++)
                    {
                        EQ_Item* inner = container->contents[inner_slot];
                        if (inner != 0 && inner->type == EQ_ITEM_EQUIPMENT
                            && ((EQ_Equipment*)inner)->item_type == 0x0e)
                        {
                            save += ((EQ_Equipment*)inner)->MR;
                            found = 1;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
            }
        }
    }

    s16 spell_mod = EQ_METHOD(1, s16, 0x004238A2, (EQ_Character*, int), this)(0x32);
    int level_mod = EQ_METHOD(1, int, 0x0041A6D6, (EQ_Character*, int), this)(other_level);
    int result = level_mod + (spell_mod + save);
    if (this->player != 0 && this->player->type == 0 && this->class_ == 1)
        result += (u32)(this->level >> 1);
    return result;
}

// FUNCTION: TRILOGY 0x0041a92b
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::basesave_fire, low)
// __thiscall. Base fire resist: 25, 5 for race 9; +5 for race 128; +10 for an NPC spawn above level 24.
int EQ_Character::basesave_fire(void)
{
    int save = 25;

    if (race == 9)
        save = 5;
    if (race == 128)
        save += 5;
    if (player != 0 && player->type == 1 && level > 24)
        save += 10;
    return save;
}

// FUNCTION: TRILOGY 0x0041a962
// Computes total fire resistance, including gear, spell effects, and class bonuses.
// @early-stop: prologue/register saves, actor-tier arithmetic, item type checks, and call-result combination still differ.
int __fastcall GetFireResist_guess(EQ_Character* character, int edx_unused, int other_level)
{
    u8 level;
    int content;
    int found_in_bag = 0;
    s16 spell_mod;
    int item;
    EQ_Item** slot_ptr;
    EQ_Item** content_ptr;
    int total;
    volatile int save; /* fake match: keep the original stack-resident accumulator */

    // base 25; race 9 -> 5, race 0x80 -> 30
    save = 0x19;
    if (character->race == 9)
        save = 5;
    if (character->race == 0x80)
        save = 0x1e;
    total = (int)character->player;
    if (total != 0)
    {
        // NPC above level 24: +10
        if ((character->player->type == '\x01') && (0x18 < character->level))
            save = save + 10;
        // field 0x2DC == 30: +3, +1 above 34, +1 above 40, up to +5 above 45
        if ((total != 0) && (character->player->actor_info != 0)
            && (character->player->actor_info->unknown0x2DC == 0x1e))
        {
            level = character->level;
            save += 3;
            if (0x22 < level)
                save += 1;
            if (0x28 < level)
                save += 1;
            if (0x2d < level)
            {
                total = level - 0x2d;
                if (5 < total)
                    total = 5;
                save += total;
            }
        }
    }
    // equipped slots 1..20
    slot_ptr = &character->inventory_items[1];
    total = 0x14;
    do
    {
        item = (int)*slot_ptr;
        if ((item != 0) && (((EQ_Item*)item)->type == 0))
            save = save + ((EQ_Equipment*)item)->FR;
        slot_ptr = slot_ptr + 1;
        total = total + -1;
    } while (total != 0);
    int slot = 0x16;
    // general inventory slots 22..29, and the bags in them
    slot_ptr = &character->inventory_items[0x16];
    do
    {
        total = (int)*slot_ptr;
        if (total != 0)
        {
            if (((EQ_Item*)total)->type == 0)
            {
                if (((EQ_Equipment*)total)->item_type == 0xe)
                {
                    save = save + ((EQ_Equipment*)total)->FR;
                    break;
                }
            }
            else if ((total != 0) && (((EQ_Item*)total)->type == 1))
            {
                item = 0;
                int child_count = (int)(u32)((EQ_Container*)total)->slots;
                if (child_count > 0)
                {
                    content_ptr = ((EQ_Container*)total)->contents;
                    do
                    {
                        content = (int)*content_ptr;
                        if (((content != 0) && (((EQ_Item*)content)->type == 0))
                            && (((EQ_Equipment*)content)->item_type == 0xe))
                        {
                            save = save + ((EQ_Equipment*)content)->FR;
                            found_in_bag = true;
                            break;
                        }
                        item = item + 1;
                        content_ptr = content_ptr + 1;
                    } while (item < child_count);
                }
                if (found_in_bag)
                    break;
            }
        }
        slot = slot + 1;
        slot_ptr = slot_ptr + 1;
    } while (slot <= 0x1d);
    spell_mod = EQ_METHOD(1, s16, 0x004238A2, (EQ_Character*, u8), character)(0x2e);
    total = EQ_METHOD(1, int, 0x0041A6D6, (EQ_Character*, int), character)(other_level);
    total = total + spell_mod + save;
    if ((character->player != 0) && (character->player->type == '\0'))
    {
        item = total;
        if (character->class_ == '\a')
        {
            item = total + 8;
            if (0x31 < character->level)
                item = total + -0x29 + (u32)character->level;
        }
        total = item;
        if (character->class_ == '\x04')
        {
            total = item + 4;
            if (0x31 < character->level)
                total = item + -0x2d + (u32)character->level;
        }
    }
    return total;
}

// FUNCTION: TRILOGY 0x0041ab14
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::basesave_cold, low)
// __thiscall. Base cold resist: 25, 35 for race 2; -10 for race 128; +10 for an NPC spawn above level 24.
int EQ_Character::basesave_cold(void)
{
    int save = 25;

    if (race == 2)
        save = 35;
    if (race == 128)
        save -= 10;
    if (player != 0 && player->type == 1 && level > 24)
        save += 10;
    return save;
}

// FUNCTION: TRILOGY 0x0041ab4b
// Computes total cold resistance, including gear, spell effects, and class bonuses.
// @early-stop: volatile save (fake match) makes each += a load/add/store; VC6 drops the redundant item != 0 tests (before the actor read and in the else-if) and caches item->type in CL; the final sum is ordered differently
/* fake match: save keeps its stack slot as in the original; without volatile VC6 gives it EBX and spills the loop-2 pointer */
// __fastcall (ECX = this, EDX unused), one stack argument: the level difference. Base save 25 (35 for race 2,
// -10 for race 128); +10 for an NPC above level 24; when the actor state is 30, by level +3, +1 above 34,
// +1 above 40 and min(level - 45, 5) above 45. Adds the CR of equipment in inventory slots 1..20, then the CR of
// the first equipment item of item type 14 in slots 22..29 (or in a container there). Then the spell total
// (effect 0x2F) and the level-difference modifier. A player spawn (type 0) of class 4 or 15 gets +4, or
// sum + level - 45 above level 49.
int __fastcall TotalColdResist_guess(EQ_Character* currentCharacter, int unused_edx, int otherLevel)
{
    volatile int resistSum;
    u8 charLevel;
    int bagItem;
    int found = 0;
    short spellBonus;
    int tmp;
    int total;
    int* itemPtr;
    int* bagSlotPtr;
    int* slotPtr;
    int slotIndex;
    int count;
    int inner;

    resistSum = 0x19;
    if (currentCharacter->race == 2)
        resistSum = 0x23;
    if (currentCharacter->race == 0x80)
        resistSum -= 10;
    tmp = *(int*)((char*)currentCharacter + 0xa0);
    if (tmp != 0)
    {
        if ((*(char*)((char*)tmp + 0x98) == '\x01')
            && (0x18 < currentCharacter->level))
            resistSum += 10;
        if (((tmp != 0) && (*(int*)((char*)tmp + 0x70) != 0))
            && (*(int*)((char*)(*(int*)((char*)tmp + 0x70)) + 0x2dc) == 0x1e))
        {
            charLevel = currentCharacter->level;
            resistSum += 3;
            if (0x22 < charLevel)
                resistSum++;
            if (0x28 < charLevel)
                resistSum++;
            if (0x2d < charLevel)
            {
                tmp = charLevel - 0x2d;
                if (5 < tmp)
                    tmp = 5;
                resistSum += tmp;
            }
        }
    }
    itemPtr = (int*)((char*)currentCharacter + 0xe4);
    tmp = 0x14;
    do
    {
        total = *itemPtr;
        if ((total != 0) && (*(char*)((char*)total + 0x81) == '\0'))
            resistSum += *(char*)((char*)total + 0xb5);
        itemPtr = itemPtr + 1;
        tmp = tmp + -1;
    } while (tmp != 0);
    slotIndex = 0x16;
    slotPtr = (int*)((char*)currentCharacter + 0x138);
    while (slotIndex <= 0x1d)
    {
        tmp = *slotPtr;
        if (tmp != 0 && *(char*)((char*)tmp + 0x81) == '\0')
        {
            if (*(char*)((char*)tmp + 0xc2) == '\x0e')
            {
                resistSum += *(char*)((char*)tmp + 0xb5);
                break;
            }
        }
        else if (tmp != 0 && *(char*)((char*)tmp + 0x81) == '\x01')
        {
            count = *(u8*)((char*)tmp + 0xd5);
            inner = 0;
            if (count > 0)
            {
                bagSlotPtr = (int*)((char*)tmp + 0xac);
                while (inner < count)
                {
                    bagItem = *bagSlotPtr;
                    if (((bagItem != 0) && (*(char*)((char*)bagItem + 0x81) == '\0'))
                        && (*(char*)((char*)bagItem + 0xc2) == '\x0e'))
                    {
                        resistSum += *(char*)((char*)bagItem + 0xb5);
                        found = 1;
                        break;
                    }
                    inner = inner + 1;
                    bagSlotPtr = bagSlotPtr + 1;
                }
            }
            if (found)
                break;
        }
        slotIndex = slotIndex + 1;
        slotPtr = slotPtr + 1;
    }
    spellBonus = EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), currentCharacter)(0x2f);
    total = EQ_METHOD(1, int, 0x0041A6D6, (EQ_Character*, int), currentCharacter)(otherLevel);
    total = total + spellBonus + resistSum;
    tmp = total;
    if ((((*(int*)((char*)currentCharacter + 0xa0) != 0)
        && (*(char*)((char*)(*(int*)((char*)currentCharacter + 0xa0)) + 0x98) == '\0'))
        && ((currentCharacter->class_ == 4)
        || (currentCharacter->class_ == 0xf))))
    {
        tmp = total + 4;
        if (0x31 < currentCharacter->level)
            tmp = total + -0x2d + currentCharacter->level;
    }
    return tmp;
}

// FUNCTION: TRILOGY 0x0041aced
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::basesave_disease, low)
// __thiscall. Base disease resist: 15, 20 for race 11, 10 for race 3; class 7 adds level - 50 above 50.
int EQ_Character::basesave_disease(void)
{
    int save = 15;

    if (race == 11)
        save = 20;
    if (race == 3)
        save = 10;
    if (class_ == 7 && level > 50)
        save += level - 50;
    return save;
}

class EQ_Character_Unknown_0041ad1c_guess : public EQ_Character
{
public:
    int resist_value_guess(int otherLevel);
};

// FUNCTION: TRILOGY 0x0041ad1c
// Computes total disease resistance, including gear, spell effects, and class bonuses.
// @early-stop: resist volatile (fake match) keeps the stack slot; RMW forms, prologue store order and loop-2 byte reuse still differ
/* fake match: volatile keeps resist in a stack slot as the original does; without it VC6 gives it EBX and never caches 0x31 there */
int EQ_Character_Unknown_0041ad1c_guess::resist_value_guess(int otherLevel)
{
    u8 lvl;
    char cls;
    int found = 0;
    short spellBonus;
    int temp;
    int temp2;
    int* slot;
    int* contentSlot;
    // loop count for the 20 worn slots; then reused as the running value
    int value = 0x14;
    int slotIndex;
    volatile int resist;
    int count;

    resist = 0xf;
    if (race == 0xb)
    {
        resist = 0x14;
    }
    if (race == 3)
    {
        resist = 10;
    }
    if ((class_ == 7) && (0x32 < level))
    {
        resist = resist + level - 0x32;
    }
    if (((player != 0) &&
        (temp = (int)player->actor_info, temp != 0)) &&
        (*(int*)(temp + 0x2dc) == 0x1e))
    {
        lvl = level;
        resist += 3;
        if (0x22 < lvl)
        {
            resist++;
        }
        if (0x28 < lvl)
        {
            resist++;
        }
        if (0x2d < lvl)
        {
            temp = lvl - 0x2d;
            if (5 < temp)
            {
                temp = 5;
            }
            resist += temp;
        }
    }
    // worn slots 1..20
    slot = (int*)&inventory_items[1];
    do
    {
        temp = *slot;
        if ((temp != 0) && (((EQ_Item*)temp)->type == EQ_ITEM_EQUIPMENT))
        {
            resist = resist + ((EQ_Equipment*)temp)->DR;
        }
        slot = slot + 1;
        value = value + -1;
    } while (value != 0);
    slotIndex = 0x16;
    // general inventory slots 22..29, bags included
    slot = (int*)&inventory_items[22];
    for (; slotIndex <= 0x1d; slotIndex++, slot++)
    {
        value = *slot;
        if (value != 0 && ((EQ_Item*)value)->type == EQ_ITEM_EQUIPMENT)
        {
            // 14 = item type that also counts from the general inventory
            if (((EQ_Equipment*)value)->item_type == 0x0e)
            {
                resist = resist + ((EQ_Equipment*)value)->DR;
                break;
            }
        }
        else if (*slot != 0 && ((EQ_Item*)*slot)->type == EQ_ITEM_CONTAINER)
        {
            temp = 0;
            count = ((EQ_Container*)value)->slots;
            if (count > 0)
            {
                contentSlot = (int*)((EQ_Container*)value)->contents;
                for (; temp < count; temp++, contentSlot++)
                {
                    temp2 = *contentSlot;
                    if (((temp2 != 0) && (((EQ_Item*)temp2)->type == EQ_ITEM_EQUIPMENT)) &&
                        (((EQ_Equipment*)temp2)->item_type == 0x0e))
                    {
                        resist = resist + ((EQ_Equipment*)temp2)->DR;
                        found = 1;
                        break;
                    }
                }
            }
            if (found)
            {
                break;
            }
        }
    }
    // spell effect 49 (0x31): disease resist
    spellBonus = EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), this)(0x31);
    value = EQ_METHOD(1, int, 0x0041A6D6, (EQ_Character*, int), this)(otherLevel);
    value = value + (spellBonus + resist);
    if ((player != 0) && (player->type == 0))
    {
        cls = class_;
        temp = value;
        if (cls == '\x03')
        {
            temp = value + 8;
            if (0x31 < level)
            {
                temp = temp + level - 0x31;
            }
        }
        temp2 = temp;
        if (cls == '\x0f')
        {
            temp2 = temp + 4;
            if (0x31 < level)
            {
                temp2 = temp2 + level - 0x31;
            }
        }
        value = temp2;
        if (cls == '\x05')
        {
            value = temp2 + 4;
            if (0x31 < level)
            {
                value = value + level - 0x31;
            }
        }
    }
    return value;
}

// FUNCTION: TRILOGY 0x0041aee8
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::basesave_poison, low)
// __thiscall. Base poison resist: 15, 20 for races 11 and 8; class 7 adds level - 50 above 50.
int EQ_Character::basesave_poison(void)
{
    int save = 15;

    if (race == 11 || race == 8)
        save = 20;
    if (class_ == 7 && level > 50)
        save += level - 50;
    return save;
}

// FUNCTION: TRILOGY 0x0041c648
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range; TAKP: GetCharacterAttributeValue)
// __thiscall, `this` unused. The low byte of the character creation value table entry [row][col].
s16 EQ_Character::CreationTableValue_guess(short col, u8 row)
{
    return (u8)g_creation_values_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c663
// Checks whether a race can choose a class during character creation.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, `this` unused. Race/class gate: race 2 with class 4 or 10 needs paineel.s3d;
// races 127/129 use rows 12/13 of the byte table at 0x0053F314 (row width 15).
u8 EQ_Character::CheckClass_guess(short race, short cls)
{
    if (race == 2)
    {
        if ((cls == 4 || cls == 10) && !DoesFileExist("paineel.s3d"))
            return 0;
    }
    else if (race == 127)
        race = 12;
    else if (race == 129)
        race = 13;
    return EQ_ARRAY(u8, 0x0053F314)[race * 15 + cls];
}

// FUNCTION: TRILOGY 0x0041c6bb
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range; TAKP: HumanCanWorship)
// __thiscall, `this` unused. Table lookup; in mode 4 row 15 accepts column 6 and row 0 nothing.
bool EQ_Character::HumanCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (row == 15)
        {
            if (col == 6)
                return 1;
        }
        else if (row == 0)
            return 0;
    }
    return g_human_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c6f7
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::BarbarianCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 7 accepts columns 0 and 9, row 0 nothing.
bool EQ_Character::BarbarianCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (row == 7)
        {
            if (col == 0 || col == 9)
                return 1;
        }
        else if (row == 0)
            return 0;
    }
    return g_barbarian_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c739
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::EruditeCanWorship, low)
// __thiscall, `this` unused. Row 3 needs paineel.s3d; in mode 4 row 0 accepts nothing.
bool EQ_Character::EruditeCanWorship_guess(u8 col, u8 row)
{
    if (3 == row && !DoesFileExist("paineel.s3d"))
        return 0;
    else
    {
        if (g_pvpRulesetCopy_guess != 4)
        {
        }
        else
        {
            if (0 == (unsigned int)row)
                return 0;
            return g_erudite_worship_guess[row][col];
        }
        return g_erudite_worship_guess[row][col];
    }
}

// FUNCTION: TRILOGY 0x0041c779
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::ElfCanWorship, low)
// __thiscall, `this` unused. In mode 4 rows 0 and 16 accept nothing.
bool EQ_Character::ElfCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if ((row == 0 || row == 16))
            return 0;
        return g_elf_worship_guess[row][col];
    }
    return g_elf_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c7ab
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::HighElfCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 0 accepts nothing.
bool EQ_Character::HighElfCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (!row)
            return 0;
        return g_highelf_worship_guess[row][col];
    }
    return g_highelf_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c7d7
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::DarkElfCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 0 accepts nothing.
bool EQ_Character::DarkElfCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (!row)
            return 0;
        return g_darkelf_worship_guess[row][col];
    }
    return g_darkelf_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c803
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::HalfElfCanWorship, low)
// __thiscall, `this` unused. Mode 2: only row 14; mode 4: row 0 accepts nothing.
bool EQ_Character::HalfElfCanWorship_guess(u8 col, u8 row)
{
    bool canWorship;
    if (g_pvpRulesetCopy_guess == 2)
        canWorship = row == 14;
    else if (g_pvpRulesetCopy_guess == 4)
    {
        if (row == 0)
            return 0;
        canWorship = g_halfelf_worship_guess[row][col];
    }
    else
        canWorship = g_halfelf_worship_guess[row][col];
    return canWorship;
}

// FUNCTION: TRILOGY 0x0041c83e
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::DwarfCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 0 accepts nothing.
bool EQ_Character::DwarfCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (!row)
            return 0;
        return g_dwarf_worship_guess[row][col];
    }
    return g_dwarf_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c86a
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::TrollCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 0 accepts nothing.
bool EQ_Character::TrollCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (!row)
            return 0;
        return g_troll_worship_guess[row][col];
    }
    return g_troll_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c896
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::OgreCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 0 accepts nothing.
bool EQ_Character::OgreCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (!row)
            return 0;
        return g_ogre_worship_guess[row][col];
    }
    return g_ogre_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c8c2
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::HalflingCanWorship, low)
// __thiscall, `this` unused. In mode 4 rows 0 and 16 accept nothing.
bool EQ_Character::HalflingCanWorship_guess(u8 col, u8 row)
{
    if (4 == ((int)(g_pvpRulesetCopy_guess)))
    {
        if ((row == 0 || row == 16))
            return 0;
        return g_halfling_worship_guess[row][col];
    }
    return g_halfling_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c8f4
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::GnomeCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 0 accepts nothing.
bool EQ_Character::GnomeCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (!row)
            return 0;
        return g_gnome_worship_guess[row][col];
    }
    return g_gnome_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c920
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::IksarCanWorship, low)
// __thiscall, `this` unused. In mode 4 row 0 accepts nothing.
bool EQ_Character::IksarCanWorship_guess(u8 col, u8 row)
{
    if (g_pvpRulesetCopy_guess == 4)
    {
        if (!row)
            return 0;
        return g_iksar_worship_guess[row][col];
    }
    return g_iksar_worship_guess[row][col];
}

// FUNCTION: TRILOGY 0x0041c94c
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::VahShirCanWorship, low)
// __thiscall, `this` unused. Only row 0, no table.
bool EQ_Character::VahShirCanWorship_guess(u8 col, u8 row)
{
    if (row == 0)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x0041cb41
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::MinAlign, low)
// __thiscall, `this` unused.
s32 EQ_Character::MinAlign_guess(u8 index)
{
    return g_min_align_guess[index];
}

// FUNCTION: TRILOGY 0x0041cb50
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::MaxAlign, low)
// __thiscall, `this` unused.
s32 EQ_Character::MaxAlign_guess(u8 index)
{
    return g_max_align_guess[index];
}

// FUNCTION: TRILOGY 0x0041cb5f
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::ClassMin, low)
// __thiscall, `this` unused. Minimum starting stat by race: races 127 and 129 use rows 12 and 13.
u8 EQ_Character::ClassMin_guess(short race, u8 stat)
{
    if (race == 127)
        race = 12;
    else if (race == 129)
        race = 13;
    return g_class_min_guess[race][stat];
}

// FUNCTION: TRILOGY 0x0041c957
// Checks whether the selected race, class, and deity can start in a city.
// @early-stop: Switch emits a sub/dec dispatch and out-of-line case returns; the original compares each row directly and shares one table lookup.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::CityCanStart, low)
// __thiscall, `this` unused. Whether a new character may start in `city` (column of the 12-wide table at
// 0x00540058, row `row`): rows 127 and 129 only allow cities 12 and 13; rows 0 and 6 add class / deity
// exceptions (and g_pvpRulesetCopy_guess modes 2 and 4) before the table lookup.
bool EQ_Character::CityCanStart_guess(short city, short row, short cls, short deity)
{
    switch (row + 1)
    {
    case 128:
        return city == 12;
    case 130:
        return city == 13;
    }
    if (city > 11)
        return false;
    if (row + 1 == 1)
    {
        if (city != 4 && (cls == 6 || cls == 7 || cls == 4 || (deity + 1 == 7 && cls == 10)))
            return false;
        if (city != 1 &&
            (cls == 1 || cls == 8 || cls == 5 ||
             (deity + 1 == 7 && ((g_pvpRulesetCopy_guess == 4 && cls == 15) || cls == 0))))
            return false;
        if (deity + 1 == 6 && city != 1)
            return false;
        if (deity + 1 == 4 && city != 1)
            return false;
    }
    if (row + 1 == 7)
    {
        if (g_pvpRulesetCopy_guess == 2 && city != 9 && city != 10)
            return false;
        if (city != 4 && (cls == 4 || cls == 6 || cls == 7))
            return false;
        if (city != 1 && (cls == 1 || cls == 5 || cls == 8))
            return false;
        if (!((deity + 1 != 4 ||
               ((city == 1 || city == 9) && (cls == 14 || city != 9))) &&
              (deity + 1 != 1 || cls != 14 || city == 9) &&
              (deity + 1 != 9 || cls != 14 || city == 9) &&
              (deity + 1 != 6 || cls != 14 || city == 9 || city == 1)))
            return false;
        if (deity + 1 == 3 && cls == 14)
        {
            if (city != 10)
                return false;
        }
        else if (city != 10)
            return g_city_start_guess[row][city];
        if (deity + 1 == 3 && cls == 14)
            return g_city_start_guess[row][city];
        return false;
    }
    return g_city_start_guess[row][city];
}

// FUNCTION: TRILOGY 0x0041cb8b
// Initial skill list for a class: class skills at 0, others 0xFF; table-flagged class skills set to 0xFE.
// @early-stop: Final loop uses an induction counter instead of recomputing the pointer index; 6 instruction differences remain.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
void __fastcall EQ_Character_InitSkills_guess(EQ_Character* eqChar, int unused_edx, u8 classId, int unused)
{
    char* skillPtr;
    int* recordPtr;

    skillPtr = (char*)eqChar + 0x9c8;
    u32* fill = (u32*)skillPtr;
    for (int count = 25; count != 0; --count)
        *fill++ = 0xffffffff;
    eqChar->skills[19] = 0;
    eqChar->skills[33] = 0;
    eqChar->skills[9] = 0;
    eqChar->skills[50] = 0;
    eqChar->skills[40] = 0;
    eqChar->skills[28] = 0;
    eqChar->skills[15] = 0;
    eqChar->skills[60] = 0;
    eqChar->skills[55] = 0;
    eqChar->skills[63] = 0;
    eqChar->skills[61] = 0;
    eqChar->skills[65] = 0;
    eqChar->skills[64] = 0;
    eqChar->skills[67] = 0;
    eqChar->skills[66] = 0;
    eqChar->skills[69] = 0;
    eqChar->skills[68] = 0;
    switch ((u32)classId)
    {
    case 1:
        *skillPtr = '\0';
        eqChar->skills[1] = 0;
        eqChar->skills[2] = 0;
        eqChar->skills[3] = 0;
        eqChar->skills[7] = 0;
        eqChar->skills[10] = 0;
        eqChar->skills[16] = 0;
        eqChar->skills[20] = 0;
        eqChar->skills[22] = 0;
        eqChar->skills[30] = 0;
        eqChar->skills[34] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[37] = 0;
        eqChar->skills[51] = 0;
        eqChar->skills[73] = 0;
        break;
    case 2:
        *skillPtr = '\0';
        eqChar->skills[2] = 0;
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[43] = 0;
        eqChar->skills[44] = 0;
        eqChar->skills[45] = 0;
        eqChar->skills[46] = 0;
        eqChar->skills[47] = 0;
        break;
    case 3:
        *skillPtr = '\0';
        eqChar->skills[1] = 0;
        eqChar->skills[2] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[3] = 0;
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[7] = 0;
        eqChar->skills[10] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[16] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[20] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[34] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[37] = 0;
        eqChar->skills[73] = 0;
        break;
    case 5:
        *skillPtr = '\0';
        eqChar->skills[1] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[2] = 0;
        eqChar->skills[3] = 0;
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[7] = 0;
        eqChar->skills[10] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[16] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[20] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[34] = 0;
        eqChar->skills[29] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[37] = 0;
        eqChar->skills[73] = 0;
        break;
    case 4:
        *skillPtr = '\0';
        eqChar->skills[1] = 0;
        eqChar->skills[2] = 0;
        eqChar->skills[3] = 0;
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[7] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[16] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[20] = 0;
        eqChar->skills[22] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[30] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[34] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[37] = 0;
        eqChar->skills[27] = 0;
        eqChar->skills[29] = 0;
        eqChar->skills[42] = 0;
        eqChar->skills[51] = 0;
        eqChar->skills[53] = 0;
        eqChar->skills[73] = 0;
        break;
    case 6:
        *skillPtr = '\0';
        eqChar->skills[1] = 0;
        eqChar->skills[2] = 0;
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[27] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[53] = 0;
        eqChar->skills[43] = 0;
        eqChar->skills[44] = 0;
        eqChar->skills[45] = 0;
        eqChar->skills[46] = 0;
        eqChar->skills[47] = 0;
        break;
    case 7:
        *skillPtr = '\0';
        eqChar->skills[2] = 0;
        eqChar->skills[16] = 0;
        eqChar->skills[20] = 0;
        eqChar->skills[30] = 0;
        eqChar->skills[37] = 0;
        eqChar->skills[51] = 0;
        eqChar->skills[11] = 0;
        eqChar->skills[21] = 0;
        eqChar->skills[23] = 0;
        eqChar->skills[25] = 0;
        eqChar->skills[71] = 0;
        eqChar->skills[26] = 0;
        eqChar->skills[32] = 0;
        eqChar->skills[38] = 0;
        eqChar->skills[39] = 0;
        eqChar->skills[42] = 0;
        eqChar->skills[52] = 0;
        eqChar->skills[22] = 5;
        break;
    case 8:
        *skillPtr = '\0';
        eqChar->skills[1] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[51] = 0;
        eqChar->skills[22] = 0;
        eqChar->skills[34] = 0;
        eqChar->skills[12] = 0;
        eqChar->skills[17] = 0;
        eqChar->skills[27] = 0;
        eqChar->skills[29] = 0;
        eqChar->skills[71] = 0;
        eqChar->skills[35] = 0;
        eqChar->skills[39] = 0;
        eqChar->skills[41] = 0;
        eqChar->skills[42] = 0;
        eqChar->skills[49] = 0;
        eqChar->skills[53] = 0;
        eqChar->skills[54] = 0;
        eqChar->skills[70] = 0;
        eqChar->skills[62] = 0;
        eqChar->skills[37] = 0;
        break;
    case 9:
        *skillPtr = '\0';
        eqChar->skills[1] = 0;
        eqChar->skills[7] = 0;
        eqChar->skills[16] = 0;
        eqChar->skills[20] = 0;
        eqChar->skills[22] = 0;
        eqChar->skills[34] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[37] = 0;
        eqChar->skills[6] = 0;
        eqChar->skills[8] = 0;
        eqChar->skills[17] = 0;
        eqChar->skills[29] = 0;
        eqChar->skills[71] = 0;
        eqChar->skills[35] = 0;
        eqChar->skills[39] = 0;
        eqChar->skills[42] = 0;
        eqChar->skills[48] = 0;
        eqChar->skills[51] = 0;
        eqChar->skills[56] = 0;
        eqChar->skills[62] = 0;
        break;
    case 10:
        *skillPtr = '\0';
        eqChar->skills[2] = 0;
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[43] = 0;
        eqChar->skills[44] = 0;
        eqChar->skills[45] = 0;
        eqChar->skills[46] = 0;
        eqChar->skills[47] = 0;
        eqChar->skills[59] = 0;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[36] = 0;
        eqChar->skills[43] = 0;
        eqChar->skills[44] = 0;
        eqChar->skills[51] = 0;
        *skillPtr = '\0';
        eqChar->skills[45] = 0;
        eqChar->skills[46] = 0;
        eqChar->skills[47] = 0;
        eqChar->skills[58] = 0;
        eqChar->skills[2] = 0;
        break;
    case 15:
        eqChar->skills[4] = 0;
        eqChar->skills[5] = 0;
        eqChar->skills[13] = 0;
        eqChar->skills[14] = 0;
        eqChar->skills[18] = 0;
        eqChar->skills[24] = 0;
        eqChar->skills[31] = 0;
        eqChar->skills[36] = 0;
        *skillPtr = '\0';
        eqChar->skills[2] = 0;
        eqChar->skills[22] = 0;
        eqChar->skills[30] = 0;
        eqChar->skills[37] = 0;
        eqChar->skills[51] = 0;
        eqChar->skills[11] = 0;
        eqChar->skills[19] = 0;
    }
    recordPtr = (int*)g_skill_records_guess;
    int skillOffset = -0x9c8 - (int)eqChar;
    do
    {
        if ((*skillPtr == '\0') && (1 < *(u8*)(*recordPtr + classId + 0x28)))
        {
            *skillPtr = -2;
        }
        ++recordPtr;
        ++skillPtr;
    } while ((int)skillPtr + skillOffset < 100);
    return;
}

// FUNCTION: TRILOGY 0x0041d17e
// Sets race and class innate abilities (race 128: Tail Rake, not Dragon Punch) and level-1 racial skills.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
void EQ_Character::InitInnates(u16 charRace, s16 charClass)
{
    u8 innateLevel;
    int fillIndex;

    for (fillIndex = 0; fillIndex < 0x19; fillIndex = fillIndex + 1)
    {
        this->innateSkills_guess[fillIndex] = 0xff;
    }
    if ((charRace == 0x80) || (*(s16*)((char*)this + 0x34) == 0x80))
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            EQ_GLOBAL(char*, 0x006E33F8), EQ_ARRAY(const char, 0x005407A0));
        *(u16*)(EQ_GLOBAL(char*, 0x006E33F8) + 0x8a) = 0x6c;
        *(u16*)(EQ_GLOBAL(char*, 0x006E33F8) + 0x8c) = 0x6d;
    }
    else
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            EQ_GLOBAL(char*, 0x006E33F8), EQ_ARRAY(const char, 0x005407AC));
        *(u16*)(EQ_GLOBAL(char*, 0x006E33F8) + 0x8a) = 0x8a;
        *(u16*)(EQ_GLOBAL(char*, 0x006E33F8) + 0x8c) = 0x8b;
    }
    switch (charRace)
    {
    case 1:
    case 0x2c:
    case 0x37:
    case 0x43:
    case 0x47:
        break;
    case 2:
    case 0x5a:
        this->innateSkills_guess[11] = 0;
        break;
    case 3:
    case 0x4e:
        this->innateSkills_guess[8] = 0;
        break;
    case 4:
    case 0x70:
        this->innateSkills_guess[6] = 0;
        if (*(char*)((char*)this + 0x38) == '\x01')
        {
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x1b, charRace);
            this->skills[27] = innateLevel;
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x1d, charRace);
            this->skills[29] = innateLevel;
        }
        break;
    case 5:
    case 0x6a:
        this->innateSkills_guess[6] = 0;
        this->innateSkills_guess[8] = 0;
        break;
    case 6:
    case 0x4d:
    case 0x62:
        this->innateSkills_guess[13] = 0;
        if (*(char*)((char*)this + 0x38) == '\x01')
        {
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x1d, charRace);
            this->skills[29] = innateLevel;
        }
        break;
    case 7:
        this->innateSkills_guess[6] = 0;
        break;
    case 8:
    case 0x5e:
        this->innateSkills_guess[6] = 0;
        if (*(char*)((char*)this + 0x38) == '\x01')
        {
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x28, charRace);
            this->skills[40] = innateLevel;
        }
        break;
    case 9:
    case 0x5c:
        this->innateSkills_guess[10] = 0;
        this->innateSkills_guess[11] = 0;
        this->innateSkills_guess[6] = 0;
        break;
    case 10:
    case 0x5d:
        this->innateSkills_guess[6] = 0;
        this->innateSkills_guess[11] = 0;
        this->innateSkills_guess[9] = 0;
        this->innateSkills_guess[2] = 0;
        break;
    case 0xb:
    case 0x51:
        this->innateSkills_guess[6] = 0;
        if (*(char*)((char*)this + 0x38) == '\x01')
        {
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x2a, charRace);
            this->skills[42] = innateLevel;
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x1d, charRace);
            this->skills[29] = innateLevel;
        }
        break;
    case 0xc:
        this->innateSkills_guess[6] = 0;
        this->innateSkills_guess[8] = 0;
        if (*(char*)((char*)this + 0x38) == '\x01')
        {
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x39, 0xc);
            this->skills[57] = innateLevel;
        }
        break;
    case 0x1b:
    case 0x20:
    case 0x21:
    case 0x3c:
    case 0x41:
    case 0x45:
    case 0x46:
    case 0x55:
    case 0x75:
    case 0x76:
    case 0x7a:
    case 0x7b:
        this->innateSkills_guess[13] = 0;
        break;
    case 0x80:
        this->innateSkills_guess[10] = 0;
        this->innateSkills_guess[6] = 0;
        if (*(char*)((char*)this + 0x38) == '\x01')
        {
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x32, 0x80);
            this->skills[50] = innateLevel;
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x1b, 0x80);
            this->skills[27] = innateLevel;
        }
        break;
    case 0x82:
        this->innateSkills_guess[13] = 0;
        if (*(char*)((char*)this + 0x38) == '\x01')
        {
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x2a, 0x82);
            this->skills[42] = innateLevel;
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x1d, 0x82);
            this->skills[29] = innateLevel;
            innateLevel = EQ_METHOD(2, u8, 0x0041A47D, (EQ_Character*, u8, int), this)(0x27, 0x82);
            this->skills[39] = innateLevel;
        }
        break;
    default:
        this->innateSkills_guess[6] = 0;
        break;
    }

    switch ((u16)charClass)
    {
    case 3:
        this->innateSkills_guess[7] = 0;
        return;
    case 5:
        this->innateSkills_guess[5] = 0;
        return;
    case 6:
        this->innateSkills_guess[4] = 0;
        return;
    case 9:
        this->innateSkills_guess[12] = 0;
        return;
    case 0xf:
        this->innateSkills_guess[14] = 0;
        return;
    case 4:
        this->innateSkills_guess[1] = 0;
        return;
    case 7:
        this->innateSkills_guess[12] = 0;
        this->innateSkills_guess[1] = 0;
        return;
    default:
        return;
    }
}

// FUNCTION: TRILOGY 0x0041d55b
// Rolls whether an attack also swings the off hand: needs Dual Wield and no two-hander in the primary hand.
bool EQ_Character::CheckDualWield_guess(int unused)
{
    CharacterProfile* eqChar = (CharacterProfile*)this;
    char itemType;
    EQ_Equipment* pEVar3;
    u32 dualWieldChance;
    bool bVar4;

    if (EQ_METHOD(1, u8, 0x004246A9, (EQ_Character*, int), (EQ_Character*)eqChar)(0x16) == 0)
    {
        bVar4 = false;
    }
    else
    {
        pEVar3 = (EQ_Equipment*)eqChar->inventory_items[0xd];
        if ((pEVar3 != 0) && (pEVar3->item.type == 0))
        {
            itemType = (char)pEVar3->item_type;
            if ((itemType == '\x01') || ((itemType == '\x04' || (itemType == '#'))))
            {
                bVar4 = false;
                return bVar4;
            }
        }
        dualWieldChance = EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqChar)(0x16);
        dualWieldChance += eqChar->level;
        bVar4 = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 0x177) <= (int)dualWieldChance;
    }
    return bVar4;
}

// FUNCTION: TRILOGY 0x0041d5bc
// Rolls the Double Attack skill to decide whether the character gets an extra attack.
bool EQ_Character::SkillCheck_guess(int unused)
{
    CharacterProfile* eqCharacter = (CharacterProfile*)this;
    bool checkPassed;

    checkPassed = EQ_METHOD(1, bool, 0x004246A9, (EQ_Character*, int), (EQ_Character*)eqCharacter)(0x14);
    if (checkPassed)
    {
        checkPassed = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 500)
            <= (int)(EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqCharacter)(0x14) + eqCharacter->level);
    }
    return checkPassed;
}

// FUNCTION: TRILOGY 0x0041d5f4
// Combat skill base damage: up with skill tiers; by level for monks/class 15 on skill 28; backstab by weapon.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
u8 __fastcall EQ_Character_GetSkillBaseDamage_guess(CharacterProfile* eqChar, int unused_edx,
    u8 skillId)
{
    u8 charLevel;
    EQ_Item* pEVar2;
    char hasSkill;
    u32 skillValue;
    u8 acc;
    unsigned int skill;

    acc = 0;
    hasSkill = (char)EQ_METHOD(1, int, 0x004246A9, (EQ_Character*, short), (EQ_Character*)eqChar)((short)skillId);
    if (hasSkill != '\0')
    {
        skill = (unsigned int)skillId;
        acc = *((u8*)EQ_ARRAY(u8*, 0x006E33A4)[skill] + 0x88);
        if (skillId != 0x1c)
        {
            skillValue = EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqChar)(skill);
            if ((int)skillValue >= 0x19)
                acc = acc + 1;
            skillValue = EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqChar)(skill);
            if ((int)skillValue >= 0x4b)
                acc = acc + 1;
            skillValue = EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqChar)(skill);
            if ((int)skillValue >= 0x7d)
                acc = acc + 1;
            skillValue = EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqChar)(skill);
            if ((int)skillValue >= 0xaf)
                acc = acc + 1;
        }
        else
        {
            if (eqChar->class_ == '\a')
            {
                charLevel = eqChar->level;
                acc = acc + charLevel / 5 + 2;
                if (0xd < acc)
                    acc = 0xd;
                if (0x31 < charLevel)
                    acc = acc + 1;
                if (eqChar->inventory_items[0xc] == 0)
                    return acc;
                if (eqChar->inventory_items[0xc]->item_nr != 0x299c)
                    return acc;
                if (charLevel > 0x32)
                    acc = 9;
                return acc;
            }
            if (eqChar->class_ != 0x0f)
                return acc;
            acc = acc + eqChar->level / 6 + 2;
            if (0xc < acc)
                acc = 0xc;
            if (eqChar->level <= 0x31)
                return acc;
            acc = acc + 1;
        }
        if (skillId == 8)
        {
            pEVar2 = eqChar->inventory_items[0xd];
            if ((pEVar2 != 0 && eqChar->player != 0 && eqChar->player->type == '\0')
                || (eqChar->player != 0 && eqChar->player->type == '\x01'))
            {
                if (pEVar2 != 0)
                    acc = ((EQ_Equipment*)pEVar2)->damage;
                else
                    acc = 3;
                skillValue = EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqChar)(8);
                acc = (u8)(int)((double)acc * ((float)(int)skillValue * EQ_GLOBAL(float, 0x00535130)
                    + EQ_GLOBAL(float, 0x005345B8)));
                if (pEVar2 != 0)
                {
                    EQ_CDECL(void, 0x004A3D01, (const char*, int, int, int, double))(
                        EQ_ARRAY(const char, 0x005407BC), (int)acc,
                        (int)(u8)((EQ_Equipment*)pEVar2)->damage,
                        (int)EQ_METHOD(1, u32, 0x00419867, (EQ_Character*, u32), (EQ_Character*)eqChar)(8),
                        (double)((float)(int)skillValue * EQ_GLOBAL(float, 0x00535130)
                            + EQ_GLOBAL(float, 0x005345B8)));
                }
            }
        }
    }
    return acc;
}

struct SkillAppearanceMessage_guess
{
    u32 spawn_id;
    s16 type;
    u16 unknown0x06;
    u32 parameter;
};

struct FeignMessage_guess
{
    u32 spawn_id;
    u32 value;
    u8 flag;
    u8 unknown0x09[3];
};

struct DisarmMessage_guess
{
    u32 source_id;
    u32 target_id;
    u8 skill;
    u8 unknown0x09;
    u8 unknown0x0A[2];
};

struct BandageMessage_guess
{
    u32 target_id;
    u16 unknown0x04;
    u16 unknown0x06;
};

struct TauntMessage_guess
{
    u32 target_id;
    u32 source_id;
    u16 skill;
    u16 unknown0x0A;
};

struct AnimationMessage_guess
{
    u32 spawn_id;
    int animation;
    float speed;
};

struct SkillRequestMessage_guess
{
    u32 target_id;
    u32 source_id;
    u8 skill;
    u8 unknown0x09;
    u16 unknown0x0A;
    u32 unknown0x0C;
    char text[2048];
};

/* TrackEntry_guess {player, distance, index, unknown0x0C} is EQ_Main's track record: trilogy/layout/eq_main.h. */

struct TrackingWindow_guess
{
    virtual void Slot0() = 0;
    virtual void Show() = 0;
    virtual void Prepare() = 0;
};

typedef char GroupMemberName_guess[30];

#define ACT_TARGET              EQ_GLOBAL(EQPlayer*, 0x006E4DAC)
#define HWM_EQMAIN                EQ_GLOBAL(u8*, 0x006E4E44)
#define ACT_MAIN_INT(offset)    (*(int*)(EQ_GLOBAL(u8*, 0x006E4E44) + (offset)))
#define ACT_TRACK_ENTRIES       ((TrackEntry_guess*)(EQ_GLOBAL(u8*, 0x006E4E44) + 0x685F8))
#define ACT_SKILL_INFO          EQ_ARRAY(u8*, 0x006E33A4)
#define ACT_SKILL_READY         EQ_ARRAY(u8, 0x006E117C)
#define ACT_SKILL_TIMER         EQ_ARRAY(u32, 0x006E0FEC)
#define ACT_MEND_TIMER          EQ_GLOBAL(u32, 0x006E106C)
#define ACT_MEND_DELAY_SOURCE   EQ_GLOBAL(u8*, 0x006E3424)
#define ACT_CURRENT_SKILL       EQ_GLOBAL(short, 0x006B6772)
#define ACT_SKILL_TICKS         EQ_GLOBAL(int, 0x006B6780)
#define ACT_START_Y             EQ_GLOBAL(int, 0x006B65C0)
#define ACT_START_X             EQ_GLOBAL(int, 0x006B65C4)
#define ACT_START_Z             EQ_GLOBAL(int, 0x006B65C8)
#define ACT_HIDING              EQ_GLOBAL(u8, 0x006B65D8)
#define ACT_ATTACKING           EQ_GLOBAL(u8, 0x006B710C)
#define ACT_RANGED_ATTACKING    EQ_GLOBAL(u8, 0x006B710D)
#define ACT_FEIGN_VALUE         EQ_GLOBAL(u32, 0x006B6460)
#define g_zone_sky_type_guess           EQ_GLOBAL(u8, 0x006B66F4)
#define ACT_GROUP_ACTIVE        EQ_GLOBAL(u8, 0x005FE968)
#define ACT_GROUP_NAMES         EQ_ARRAY(GroupMemberName_guess, 0x005FE8BD)
#define ACT_GROUP_PLAYERS       EQ_ARRAY(EQPlayer*, 0x005FE954)
#define HWM_PLAYER_TOP          EQ_GLOBAL(EQPlayer*, 0x006E4D58)
#define ACT_DISPLAY_TIME        (*(u32*)((u8*)g_display_guess + 0xBC))
#define ACT_ACTOR_U8(player, offset)  (*((u8*)(player)->actor_info + (offset)))
#define ACT_ACTOR_U32(player, offset) (*(u32*)((u8*)(player)->actor_info + (offset)))
#define ACT_CHAT(text, color) \
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((text), (color))
#define ACT_TOKEN(text) \
    EQ_CDECL(void, 0x004A07E9, (EQ_Character*, const char*, int))(self, (text), 0x10E)
#define ACT_SEND(opcode, data, size) \
    EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))(g_world_connection_guess, (opcode), (data), (size), 1)
#define ACT_DISTANCE(from, to) \
    EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)((from), (to), 0.0f)
#define SOC_CPY              EQ_CDECL(char*, 0x005219C0, (char*, const char*))
#define SOC_FMT             EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
#define ACT_GET_AFFECT(character, effect, slot) \
    EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), (character))((effect), (slot))
#define ACT_REMOVE_AFFECT(character, affect) \
    EQ_METHOD(1, void, 0x00424E22, (void*, void*), (character))(affect)
#define ACT_NOTIFY_AFFECT(character, slot) \
    EQ_METHOD(1, void, 0x004251B9, (void*, int), (character))(slot)
#define ACT_CHANGE_STANCE(player, stance) \
    EQ_METHOD(1, void, 0x004B45CB, (void*, int), (player))(stance)
#define ACT_DO_ATTACK(player, attackSkill) \
    EQ_METHOD(3, void, 0x004B2C1E, (EQPlayer*, int, u8, EQPlayer*), (player))(100, (attackSkill), ACT_TARGET)
#define ACT_CAST(spell) \
    EQ_METHOD(4, void, 0x0041F381, (EQ_Character*, int, int, int*, int), self)(9, (spell), 0, -1)
#define ACT_RANGE_LIMIT         15.0f

// FUNCTION: TRILOGY 0x0041D790
// __fastcall on EQ_Character; uses the skill or ability with the given id (the skill buttons).
// Guards: player present, 0x004199c0 result, skill not disabled (0xfe or more), not in the 0x66 stand state.
// Ids outside the three timed abilities first check and set the skill ready/timer arrays; then a switch on the id
// sends the matching packet or starts the action (stance change, disarm, sneak, hide, attack, and so on).
// The three timed abilities check the pending-request counters and actor_info+0xf8, then cast through 0x0041f381.
void __fastcall UseSkill(EQ_Character* self, int unused_edx, u8 skill, int unused)
{
    char message[120];
    SkillRequestMessage_guess request;
    SkillAppearanceMessage_guess appearance;
    EQPlayer* target;
    EQ_Item* shield;

    if (self->player == 0)
        return;
    if (EQ_METHOD(1, int, 0x004199C0, (EQ_Character*, int), self)(0) < 1)
        return;
    if (skill < 100 && self->skills[skill] >= 0xFE)
        return;
    if (self->player->stand_state == 0x66)
    {
        ACT_CHAT(EQ_ARRAY(const char,0x005410A8), 0x0D);
        return;
    }
    target = ACT_TARGET;
    if (skill != 0x6B && skill != 0x69 && skill != 0x72)
    {
        if (skill != 0x6F && ACT_SKILL_INFO[skill][0x26] != 2)
        {
            if ((skill != 0x2A || self->player->sneaking == 0) && (skill != 0x1D || ACT_HIDING == 0))
            {
                if (ACT_SKILL_READY[skill] == 0)
                    return;
                if (skill == 0x20)
                    ACT_MEND_TIMER = ACT_DISPLAY_TIME + *(u32*)(ACT_MEND_DELAY_SOURCE + 0x20);
                else
                    ACT_SKILL_TIMER[skill] = ACT_DISPLAY_TIME;
                ACT_SKILL_READY[skill] = 0;
            }
            switch (skill)
            {
            case 0x19:
            {
                FeignMessage_guess feign;

                ACT_CHANGE_STANCE(self->player, 0x73);
                feign.spawn_id = self->player->spawn_id;
                feign.flag = 0;
                feign.value = ACT_FEIGN_VALUE;
                ACT_SEND(0x20AC, &feign, 0x0C);
                break;
            }
            case 0x11:
                ACT_SEND(0x21F3, 0, 0);
                break;
            case 0x10:
            {
                DisarmMessage_guess disarm;
                short affectSlot;

                if (target == 0)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x0054107C), 0x10E);
                    return;
                }
                if (target == self->player)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00541060), 0x10E);
                    return;
                }
                if (ACT_DISTANCE(self->player, target) > ACT_RANGE_LIMIT)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00541034), 0x10E);
                    return;
                }
                if (ACT_TARGET->type > 1)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x0054100C), 0x10E);
                    return;
                }
                if (!EQ_METHOD(2, bool, 0x004B2975, (EQPlayer*, EQPlayer*, float), self->player)(ACT_TARGET, 0.0f))
                    return;
                if (self->player->invis != 0)
                {
                    self->player->invis = 0;
                    appearance.spawn_id = self->player->spawn_id;
                    appearance.type = 3;
                    appearance.parameter = self->player->invis;
                    ACT_REMOVE_AFFECT(g_pc_guess, ACT_GET_AFFECT(g_pc_guess, 0x0C, &affectSlot));
                    if (affectSlot != -1)
                        ACT_NOTIFY_AFFECT(g_pc_guess, affectSlot);
                    ACT_REMOVE_AFFECT(g_pc_guess, ACT_GET_AFFECT(g_pc_guess, 0x1C, &affectSlot));
                    if (affectSlot != -1)
                        ACT_NOTIFY_AFFECT(g_pc_guess, affectSlot);
                    ACT_REMOVE_AFFECT(g_pc_guess, ACT_GET_AFFECT(g_pc_guess, 0x1D, &affectSlot));
                    if (affectSlot != -1)
                        ACT_NOTIFY_AFFECT(g_pc_guess, affectSlot);
                    ACT_REMOVE_AFFECT(g_pc_guess, ACT_GET_AFFECT(g_pc_guess, 0x5E, &affectSlot));
                    if (affectSlot != -1)
                        ACT_NOTIFY_AFFECT(g_pc_guess, affectSlot);
                    ACT_SEND(0x20F5, &appearance, 0x0C);
                    EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_display_guess)(g_local_player_guess);
                }
                disarm.source_id = self->player->spawn_id;
                disarm.target_id = ACT_TARGET->spawn_id;
                disarm.unknown0x09 = 0xFF;
                disarm.skill = self->skills[0x10];
                ACT_SEND(0x20AA, &disarm, 0x0C);
                break;
            }
            case 9:
            {
                BandageMessage_guess bandage;

                if (ACT_CURRENT_SKILL == 9)
                    return;
                if (target == 0)
                {
                    ACT_TOKEN(EQ_ARRAY(const char,0x00540FE4));
                    return;
                }
                if (target->type != 0)
                {
                    ACT_TOKEN(EQ_ARRAY(const char,0x00540FBC));
                    return;
                }
                if (!EQ_METHOD(3, bool, 0x00427C70, (EQ_Character*, short, short*, short*), self)(0x12, 0, 0))
                {
                    ACT_TOKEN(EQ_ARRAY(const char,0x00540F88));
                    return;
                }
                if (target->type != 0)
                    return;
                if (ACT_DISTANCE(self->player, target) < ACT_RANGE_LIMIT)
                {
                    if (HWM_EQMAIN != 0)
                    {
                        pinstEQ_Main_guess->pendingServerReplies_guess++;
                        pinstEQ_Main_guess->pendingWorldRequests_guess++;
                    }
                    bandage.target_id = target->spawn_id;
                    bandage.unknown0x04 = 0;
                    ACT_SEND(0x2093, &bandage, 8);
                    ACT_CHANGE_STANCE(self->player, 0x69);
                    if (target == g_local_player_guess)
                        SOC_CPY(message, EQ_ARRAY(const char,0x00540F68));
                    else
                        SOC_FMT(message, EQ_ARRAY(const char,0x00540F4C), target->name);
                    ACT_CHAT(message, 0x10E);
                }
                else
                {
                    SOC_FMT(message, EQ_ARRAY(const char,0x00540F28), target->name);
                    ACT_TOKEN(message);
                }
                break;
            }
            case 0x1B:
                ACT_SEND(0x2094, 0, 0);
                break;
            case 0x28:
            {
                char direction[32];
                short heading;
                short roll;

                heading = (short)self->player->heading;
                roll = (short)EQ_CDECL(int, 0x004A46F6, (int, int))(1, 100);
                if (roll <= 7)
                    roll = 0;
                if (roll > (short)self->skills[0x28] || roll > 0x5F)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540E88), 0x10E);
                    return;
                }
                SOC_CPY(direction, EQ_ARRAY(const char,0x0058FBA4));
                if (heading >= 0x1E0)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540ED8));
                else if (heading > 0x1A0)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540F1C));
                else if (heading > 0x160)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540F14));
                else if (heading > 0x120)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540F08));
                else if (heading > 0xE0)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540F00));
                else if (heading > 0xA0)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540EF4));
                else if (heading > 0x60)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540EEC));
                else if (heading > 0x20)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540EE0));
                else if (heading >= 0)
                    SOC_CPY(direction, EQ_ARRAY(const char,0x00540ED8));
                SOC_FMT(message, EQ_ARRAY(const char,0x00540EB8), direction);
                ACT_CHAT(message, 0x10E);
                ACT_SEND(0x2187, 0, 0);
                break;
            }
            case 0x20:
                ACT_SEND(0x219D, 0, 0);
                break;
            case 0x1D:
                if (ACT_CURRENT_SKILL == 0x1D)
                {
                    ACT_HIDING = 0;
                    ACT_CHAT(EQ_ARRAY(const char,0x00540E74), 0x10E);
                    EQ_METHOD(0, void, 0x004255CE, (EQ_Character*), self)();
                    ACT_CURRENT_SKILL = -1;
                    return;
                }
                if (ACT_ATTACKING == 0 && ACT_RANGED_ATTACKING == 0)
                {
                    if (self->player->dy >= -0.001f && self->player->dy <= 0.001f
                        && self->player->dx >= -0.001f && self->player->dx <= 0.001f
                        && self->player->dz >= -0.001f && self->player->dz <= 0.001f)
                    {
                        ACT_CURRENT_SKILL = 0x1D;
                        ACT_SKILL_TICKS = 0;
                        ACT_START_Y = (int)self->player->y;
                        ACT_START_X = (int)self->player->x;
                        ACT_START_Z = (int)self->player->z;
                        ACT_HIDING = 1;
                        ACT_SEND(0x2186, 0, 0);
                        ACT_CHAT(EQ_ARRAY(const char,0x00540E5C), 0x10E);
                        EQ_METHOD(0, void, 0x004255CE, (EQ_Character*), self)();
                    }
                    else
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x00540E34), 0x10E);
                    }
                }
                else
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540E0C), 0x10E);
                }
                break;
            case 0x2A:
                if (self->player->sneaking != 0)
                {
                    self->player->sneaking = 0;
                    appearance.spawn_id = self->player->spawn_id;
                    appearance.type = 0x0F;
                    appearance.parameter = 0;
                    ACT_SEND(0x20F5, &appearance, 0x0C);
                    ACT_CHAT(EQ_ARRAY(const char,0x00540DF8), 0x10E);
                }
                else if (ACT_ATTACKING == 0 && ACT_RANGED_ATTACKING == 0)
                {
                    if (self->player->actor_info->unknown0x099 != 0)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x00540DD8), 0x10E);
                    }
                    else
                    {
                        ACT_SEND(0x2185, 0, 0);
                        ACT_CHAT(EQ_ARRAY(const char,0x00540DC0), 0x10E);
                    }
                }
                else
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540D94), 0x10E);
                }
                break;
            case 0x49:
            {
                TauntMessage_guess taunt;
                float distance;
                u16 race;

                if (target == 0 || target->type != 1)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540D6C), 0x10E);
                    return;
                }
                distance = ACT_DISTANCE(self->player, target);
                if (EQ_CDECL(float, 0x004A491F, (EQPlayer*, EQPlayer*))(self->player, ACT_TARGET) < distance)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540D40), 0x10E);
                    return;
                }
                race = ACT_TARGET->race;
                if (race != 0x8D && race != 0x48 && race != 0x49)
                {
                    SOC_FMT(message, EQ_ARRAY(const char,0x00540D10),
                        EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), pinstCEverQuest_guess)(ACT_TARGET->name));
                    ACT_CHAT(message, 0x10E);
                    taunt.source_id = self->player->spawn_id;
                    taunt.target_id = ACT_TARGET->spawn_id;
                    taunt.skill = self->skills[0x49];
                    ACT_SEND(0x213B, &taunt, 0x0C);
                }
                else
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540CEC), 0x10E);
                }
                break;
            }
            case 0x47:
            {
                const char* text;

                if (target == 0)
                {
                    text = EQ_ARRAY(const char,0x00540CC4);
                }
                else if (target == g_local_player_guess || target == g_control_player_guess)
                {
                    text = EQ_ARRAY(const char,0x00540C94);
                }
                else if (g_local_player_guess->stand_state == 100)
                {
                    ACT_ACTOR_U8(self->player, 0x188) = 6;
                    ACT_SEND(0x219C, 0, 0);
                    return;
                }
                else
                {
                    text = EQ_ARRAY(const char,0x00540CB4);
                }
                ACT_CHAT(text, 0x10E);
                ACT_ACTOR_U32(self->player, 0x174) = 0;
                ACT_ACTOR_U8(self->player, 0x188) = 0;
                break;
            }
            case 0x43:
                if (target == 0)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540C68), 0x10E);
                    return;
                }
                if (target->type != 1)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540C3C), 0x10E);
                    return;
                }
                if (ACT_ATTACKING == 0 && ACT_RANGED_ATTACKING == 0)
                {
                    if (ACT_DISTANCE(self->player, target) > ACT_RANGE_LIMIT)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x00540C10), 0x10E);
                        return;
                    }
                    if (self->player->invis > 0)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x00540BD8), 0x10E);
                        return;
                    }
                    request.target_id = ACT_TARGET->spawn_id;
                    request.source_id = self->player->spawn_id;
                    request.unknown0x09 = 0xFF;
                    request.unknown0x0C = 0;
                    request.skill = self->skills[0x43];
                    SOC_CPY(request.text, EQ_ARRAY(const char,0x0058FBA4));
                    pinstEQ_Main_guess->pendingWorldRequests_guess++;
                    ACT_SEND(0x2125, &request, 0x12);
                }
                else
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540BB0), 0x10E);
                }
                break;
            case 0x3E:
                ACT_SEND(0x2188, 0, 0);
                break;
            case 0x37:
            {
                AnimationMessage_guess animation;
                short slot;
                short bagSlot;

                if (ACT_ACTOR_U8(self->player, 0x187) != 0)
                {
                    if (self->player->type != 0)
                        return;
                    ACT_TOKEN(EQ_ARRAY(const char,0x00540B94));
                    return;
                }
                if (self->player->actor_info->unknown0x099 != 0)
                {
                    if (self->player->type != 0)
                        return;
                    ACT_TOKEN(EQ_ARRAY(const char,0x00540B70));
                    return;
                }
                if (self->player->type == 0)
                {
                    if (self->inventory_items[0] != 0)
                    {
                        ACT_TOKEN(EQ_ARRAY(const char,0x00540B48));
                        return;
                    }
                    if (ACT_ATTACKING != 0 || ACT_RANGED_ATTACKING != 0)
                    {
                        ACT_TOKEN(EQ_ARRAY(const char,0x00540A68));
                        return;
                    }
                    if (!EQ_METHOD(3, bool, 0x00427C70, (EQ_Character*, short, short*, short*), self)(0x24, &slot, &bagSlot))
                    {
                        ACT_TOKEN(EQ_ARRAY(const char,0x00540B14));
                        return;
                    }
                    if (bagSlot != -1 || (slot != 0x0D && slot != 0x0E))
                    {
                        ACT_TOKEN(EQ_ARRAY(const char,0x00540A94));
                        return;
                    }
                    if (!EQ_METHOD(3, bool, 0x00427C70, (EQ_Character*, short, short*, short*), self)(0x25, 0, 0))
                    {
                        ACT_TOKEN(EQ_ARRAY(const char,0x00540AE0));
                        return;
                    }
                    if (self->player != 0)
                    {
                        EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, char*), g_display_guess)(self->player,
                            EQ_CDECL(int, 0x004A1DCE, (short))(slot), self->inventory_items[slot]->idfile);
                    }
                    ACT_TOKEN(EQ_ARRAY(const char,0x00540ACC));
                    ACT_CURRENT_SKILL = 0x37;
                    ACT_SKILL_TICKS = 0;
                    ACT_START_Y = (int)self->player->y;
                    ACT_START_X = (int)self->player->x;
                    ACT_START_Z = (int)self->player->z;
                }
                if (self->stand_state == 100)
                {
                    animation.spawn_id = self->player->spawn_id;
                    animation.animation = 5;
                    animation.speed = 0.5f;
                    EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(
                        self->player, 5, 1, 0, 0.5f, 1);
                    ACT_SEND(0x209F, &animation, 0x0C);
                }
                ACT_ACTOR_U32(self->player, 0x16C) = EQ_CDECL(int, 0x004A461E, (void))() + 2000;
                ACT_ACTOR_U8(self->player, 0x187) = 1;
                ACT_SEND(0x218F, 0, 0);
                break;
            }
            case 0x35:
            {
                EQPlayer* spawn;
                float range;
                short count;
                short inGroup;
                int index;
                int member;
                u8 trackingSkill;

                if (g_zone_sky_type_guess != 2 && g_zone_sky_type_guess != 1 && g_zone_sky_type_guess != 5)
                {
                    trackingSkill = self->skills[0x35];
                    range = (float)(int)(trackingSkill >> 1) + 500.0f;
                    if (trackingSkill > 100)
                        range += (float)(trackingSkill - 100) * 4.0f;
                }
                else
                {
                    trackingSkill = self->skills[0x35];
                    range = (float)(int)trackingSkill * 10.0f + 500.0f;
                    if (trackingSkill > 100)
                        range += (float)(trackingSkill - 100) * 10.0f;
                }
                ((TrackingWindow_guess*)HWM_EQMAIN)->Prepare();
                HWM_EQMAIN[0xFE0] = 0;
                pinstEQ_Main_guess->trackWindowOpen_guess = 1;
                pinstEQ_Main_guess->trackSelected_guess = 0;
                HWM_EQMAIN[0x8FBAC] = 0;
                for (index = 0; index < 1000; index++)
                {
                    ACT_TRACK_ENTRIES[index].player = 0;
                    ACT_TRACK_ENTRIES[index].distance = -1.0f;
                    ACT_TRACK_ENTRIES[index].index = -1;
                    ACT_TRACK_ENTRIES[index].unknown0x0C = 0;
                }
                count = 0;
                if (ACT_GROUP_ACTIVE != 0)
                {
                    for (member = 0; member < 5; member++)
                    {
                        if (ACT_GROUP_NAMES[member][0] != 0 && ACT_GROUP_PLAYERS[member] != 0)
                        {
                            index = count;
                            ACT_TRACK_ENTRIES[index].distance = ACT_DISTANCE(ACT_GROUP_PLAYERS[member], g_control_player_guess);
                            ACT_TRACK_ENTRIES[index].player = ACT_GROUP_PLAYERS[member];
                            count++;
                            ACT_TRACK_ENTRIES[index].index = index;
                        }
                    }
                }
                for (spawn = HWM_PLAYER_TOP; spawn != 0; spawn = spawn->next)
                {
                    if (spawn != g_control_player_guess && spawn->type < 2
                        && spawn->race != 0x8F && spawn->race != 0x8D && spawn->race != 0x48
                        && spawn->race != 0x49 && spawn->race != 0x72 && spawn->race != 0x8E
                        && EQ_METHOD(2, int, 0x00410C80, (void*, EQPlayer*, EQPlayer*), g_display_guess)(
                               g_control_player_guess, spawn) == 0)
                    {
                        inGroup = 0;
                        if (ACT_GROUP_ACTIVE != 0)
                        {
                            for (member = 0; member < 5; member++)
                            {
                                if (ACT_GROUP_NAMES[member][0] != 0 && ACT_GROUP_PLAYERS[member] == spawn)
                                    inGroup = 1;
                            }
                        }
                        if (count < 1000 && inGroup == 0)
                        {
                            index = count;
                            ACT_TRACK_ENTRIES[index].distance = ACT_DISTANCE(spawn, g_control_player_guess);
                            if (ACT_TRACK_ENTRIES[index].distance <= range)
                            {
                                ACT_TRACK_ENTRIES[index].player = spawn;
                                count++;
                                ACT_TRACK_ENTRIES[index].index = index;
                            }
                        }
                    }
                }
                ACT_SEND(0x2184, 0, 0);
                pinstEQ_Main_guess->trackCount_guess = count;
                EQ_METHOD(0, void, 0x0046A760, (void*), HWM_EQMAIN)();
                EQ_METHOD(0, void, 0x0046AA06, (void*), HWM_EQMAIN)();
                ((TrackingWindow_guess*)HWM_EQMAIN)->Show();
                break;
            }
            case 0x30:
            {
                u8 targetType;
                int levelDifference;

                if (target == 0)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540A38), 0x10E);
                    return;
                }
                if (target->gm != 0)
                    return;
                targetType = target->type;
                if (targetType >= 2)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x005409E8), 0x10E);
                    return;
                }
                if (self->unknown0x0076 == 1)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x005409C0), 0x10E);
                    return;
                }
                if (ACT_ATTACKING != 0 || ACT_RANGED_ATTACKING != 0)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540848), 0x10E);
                    return;
                }
                if (targetType == 0)
                {
                    if (target == self->player)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x00540984), 0x10E);
                        return;
                    }
                    if (target->pvp == 0 && g_pvpRuleset_guess != 4)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x00540938), 0x10E);
                        return;
                    }
                    if (self->player->pvp == 0 && g_pvpRuleset_guess != 4)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x00540908), 0x10E);
                        return;
                    }
                }
                if (g_pvpRuleset_guess == 4 && targetType == 0)
                {
                    if (EQ_CDECL(int, 0x004B21FC, (EQPlayer*, EQPlayer*, void*))(target, self->player, 0) == 0)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x005408D0), 0x10E);
                        return;
                    }
                    levelDifference = (int)ACT_TARGET->level - (int)self->player->level;
                    if (levelDifference < -5 || levelDifference > 5 || ACT_TARGET->level < 6)
                    {
                        ACT_CHAT(EQ_ARRAY(const char,0x0054089C), 0x10E);
                        return;
                    }
                }
                if (ACT_DISTANCE(self->player, ACT_TARGET) > ACT_RANGE_LIMIT)
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x00540870), 0x10E);
                    return;
                }
                request.target_id = ACT_TARGET->spawn_id;
                request.source_id = self->player->spawn_id;
                request.unknown0x09 = 0xFF;
                request.skill = self->skills[0x30];
                request.unknown0x0C = 0;
                SOC_CPY(request.text, EQ_ARRAY(const char,0x0058FBA4));
                pinstEQ_Main_guess->pendingWorldRequests_guess++;
                ACT_SEND(0x20AD, &request, 0x12);
                if (ACT_CURRENT_SKILL == 0x1D || ACT_HIDING != 0)
                {
                    ACT_HIDING = 0;
                    ACT_CHAT(EQ_ARRAY(const char,0x00540E74), 0x10E);
                    ACT_CURRENT_SKILL = -1;
                }
                ACT_REMOVE_AFFECT(self, ACT_GET_AFFECT(self, 0x0C, 0));
                ACT_REMOVE_AFFECT(self, ACT_GET_AFFECT(self, 0x5E, 0));
                ACT_REMOVE_AFFECT(self, ACT_GET_AFFECT(self, 0x1C, 0));
                ACT_REMOVE_AFFECT(self, ACT_GET_AFFECT(self, 0x1D, 0));
                EQ_METHOD(0, void, 0x004255CE, (EQ_Character*), self)();
                break;
            }
            }
        }
        else
        {
            pinstEQ_Main_guess->combatSkillReady_guess = 0;
            switch (skill)
            {
            case 8:
            case 0x15:
            case 0x17:
            case 0x1A:
            case 0x1E:
            case 0x26:
            case 0x34:
                ACT_DO_ATTACK(self->player, skill);
                break;
            case 0x0A:
                shield = self->inventory_items[0x0E];
                if ((shield != 0 && shield->type == 0 && ((EQ_Equipment*)shield)->item_type == 8)
                    || (self->inventory_items[0x0D] != 0
                        && (self->inventory_items[0x0D]->item_nr == 0x2B2A
                            || self->inventory_items[0x0D]->item_nr == 0x2773
                            || self->inventory_items[0x0D]->item_nr == 0x382F)))
                {
                    ACT_DO_ATTACK(self->player, skill);
                }
                else
                {
                    ACT_CHAT(EQ_ARRAY(const char,0x0054081C), 0x10E);
                }
                break;
            case 0x6F:
                if (self->skills[0x6F] == 0xFF)
                    return;
                ACT_DO_ATTACK(self->player, 0x0A);
                break;
            }
        }
    }
    else
    {
        switch (skill)
        {
        case 0x69:
            if (self->skills[0x69] == 0xFF)
                return;
            if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                return;
            if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                return;
            if (ACT_DISPLAY_TIME < ACT_ACTOR_U32(self->player, 0xF8))
            {
                ACT_CHAT(EQ_ARRAY(const char,0x005407FC), 0x10E);
                return;
            }
            ACT_CAST(0x58);
            break;
        case 0x6B:
            if (self->skills[0x6B] == 0xFF)
                return;
            if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                return;
            if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                return;
            if (ACT_DISPLAY_TIME < ACT_ACTOR_U32(self->player, 0xF8))
            {
                ACT_CHAT(EQ_ARRAY(const char,0x005407FC), 0x10E);
                return;
            }
            ACT_CAST(0x57);
            break;
        case 0x72:
            if (self->skills[0x72] == 0xFF)
                return;
            if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                return;
            if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                return;
            if (ACT_DISPLAY_TIME < ACT_ACTOR_U32(self->player, 0xF8))
            {
                ACT_CHAT(EQ_ARRAY(const char,0x005407FC), 0x10E);
                return;
            }
            ACT_CAST(0xA48);
            break;
        }
    }
}

// FUNCTION: TRILOGY 0x0041ec4f
// Processes an intimidation animation event and sends it to other clients when appropriate.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// @early-stop: structural match improved to 0.7407; case block layout and thiscall argument setup still differ
void __fastcall EQ_Character_DoIntimidationEvent_guess(EQ_Character* profile, int unused_edx)
{
    EQPlayer* pEVar4;
    u8 eventType;
    struct IntimidationMessage_guess
    {
        u32 spawn_id;
        int animation;
        u32 unknown;
    } packet;

    pEVar4 = profile->player;
    if (pEVar4 == 0)
        return;
    if (pEVar4->type == '\0')
    {
        if (pEVar4->stand_state == 'd')
        {
            eventType = pEVar4->actor_info->unk_0x188;
            if (eventType < 1)
                return;
            if (*(u32*)(g_displayManager_guess + 0xbc)
                < pEVar4->actor_info->unk_0x174)
                return;
            if ((ACT_TARGET == 0)
                || (ACT_TARGET == pEVar4)
                || (ACT_TARGET == g_control_player_guess))
            {
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((const char*)0x005410d8, 0x10e);
                profile->player->actor_info->unk_0x174 = 0;
            }
            else
            {
                switch (eventType)
                {
                case 1:
                    pEVar4->actor_info->unk_0x174 = 0;
                    pEVar4->actor_info->unk_0x188 = 0;
                    return;
                case 2:
                    packet.animation = ((profile->class_ != 7) - 1 & 0x2c) + 2;
                    break;
                case 3:
                    packet.animation = ((profile->class_ != 7) - 1 & 0x26) + 7;
                    break;
                case 4:
                    packet.animation = ((profile->class_ != 7) - 1 & 0x2a) + 5;
                    break;
                case 5:
                    packet.animation = ((profile->class_ != 7) - 1 & 0xfffffffb) + 6;
                    break;
                case 6:
                    packet.animation = 0x2b;
                    break;
                default:
                    return;
                }
                pEVar4->actor_info->unk_0x188
                    = pEVar4->actor_info->unk_0x188 - 1;
                pEVar4->actor_info->unk_0x174
                    = *(u32*)(g_displayManager_guess + 0xbc) + 1000;
                packet.spawn_id = pEVar4->spawn_id;
                packet.unknown = 0x3f99999a;
                EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(pEVar4, packet.animation, 1, 0,
                        EQ_GLOBAL(float, 0x00535194), 1);
                send_message_guess(
                    g_world_connection_guess, 0x209f, &packet, 0xc, 1);
                return;
            }
            pEVar4 = profile->player;
        }
    }
    else
    {
        pEVar4 = profile->player;
        if (pEVar4 == 0)
            return;
    }
    pEVar4->actor_info->unk_0x188 = 0;
    return;
}

typedef float (__cdecl *FloatFastFn_guess_t)(float value);

// FUNCTION: TRILOGY 0x0041ee15
// Advances a fishing attempt, including bite effects and stopping when movement interrupts it.
// @early-stop: float slots shifted (the int->float temp takes ebp-0x14 and topZ lands at ebp-4), |z-top| is stored before the sign test, and there is no mov eax,edi before the cos fld.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, one fishing tick. |speed| > 0.01 stops the fishing with a chat line. Otherwise, once the time at
// actor+0x16C has passed, the state at actor+0x187 decides: 1 casts 16..32 units ahead for a bite (when the spot
// is within 30 units of the player's z and its env is not 0, 3 or 7: a particle effect and a sound if the top z
// is known, state 6 and the next time 9 s on); 2..5 set the state to 0 at once; 6 and a missed bite set it to 0
// after the ACT_CURRENT_SKILL 0x37 -> -1 step; any other state is traced first.
void EQ_Character::DoFishingEvent(void)
{
    EQPlayer* pEVar4;
    EQActorInfo_guess* actorInfo;
    u8 tmp;
    int result;
    float hookPos[3];
    float topZ;
    float hookZ;
    float diffZ;
    float hookY;
    float hookX;

    pEVar4 = this->player;
    if (pEVar4 == 0)
        return;
    if (pEVar4->actor_info == 0)
        return;
    if (EQ_CDECL(double, 0x0052330A, (double))(pEVar4->speed) > 0.01)
    {
        if (EQ_GLOBAL(u16, 0x006B6772) == 0x37)
            EQ_GLOBAL(u16, 0x006B6772) = -1;
        this->player->actor_info->unk_0x187 = 0;
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)("You stop fishing and go on your way.", 0x10e);
        return;
    }
    actorInfo = (EQActorInfo_guess *)this->player->actor_info;
    if (*(u32*)(g_displayManager_guess + 0xbc) < actorInfo->unk_0x16C)
        return;
    tmp = actorInfo->unk_0x187;
    switch (tmp)
    {
    case 1:
        result = EQ_CDECL(int, 0x004A46F6, (int, int))(16, 32);
        pEVar4 = this->player;
        topZ = (float)result;
        hookY = (float)(EQ_GLOBAL(FloatFastFn_guess_t, 0x006E524C)(pEVar4->heading) * topZ + pEVar4->y);
        pEVar4 = this->player;
        hookX = (float)(EQ_GLOBAL(FloatFastFn_guess_t, 0x006E5250)(pEVar4->heading) * topZ + pEVar4->x);
        hookZ = (float)(EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), g_display_guess)(hookY, hookX, pEVar4->z) + 3.0f);
        topZ = EQ_METHOD(3, float, 0x00414705, (void*, float, float, float), g_display_guess)(hookY, hookX, hookZ);
        diffZ = this->player->z - topZ;
        if (diffZ < 0.0f)
            diffZ = -diffZ;
        EQ_CDECL(void, 0x004A3D01, (const char*, double, double, double, double, double, double))(
            "hook xyz = %f, %f, %f, my_z = %f, top_z = %f, diff_z = %f\n", hookY, hookX, hookZ,
            this->player->z, topZ, diffZ);
        if (diffZ <= 30.0f
            && (tmp = EQ_METHOD(4, u8, 0x004147BB, (void*, float, float, float, int), g_display_guess)(hookY, hookX, hookZ, 0)) != 0
            && tmp != 3 && tmp != 7)
        {
            if (topZ != -1e27f)
            {
                hookPos[0] = hookY;
                hookPos[1] = hookX;
                hookPos[2] = topZ;
                EQ_CDECL(void, 0x004C7F71, (int, void*, struct EQPlayer*, int, int, float*, int, int))(
                    2, EQ_ARRAY(char, 0x0062E2D8), this->player, 0, 0, hookPos, 0, 500);
                EQ_METHOD(5, void, 0x0040E3EA, (void*, int, int, float, float, float), g_display_guess)(0xff, 0x63, hookY, hookX, topZ);
            }
            this->player->actor_info->unk_0x187 = 6;
            result = EQ_CDECL(int, 0x004A461E, (void))();
            this->player->actor_info->unk_0x16C = result + 9000;
            return;
        }
        break;
    /* 2..5 each clear the state and return. They stay separate labels: grouped labels become a range
       check (a compare tree), not the original's dec chain. */
    case 2:
        actorInfo->unk_0x187 = 0;
        return;
    case 3:
        actorInfo->unk_0x187 = 0;
        return;
    case 4:
        actorInfo->unk_0x187 = 0;
        return;
    case 5:
        actorInfo->unk_0x187 = 0;
        return;
    case 6:
        break;
    default:
        EQ_CDECL(void, 0x004A3D01, (const char*, int))("Unknown fishing event! %u", tmp);
        break;
    }
    if (EQ_GLOBAL(u16, 0x006B6772) == 0x37)
        EQ_GLOBAL(u16, 0x006B6772) = -1;
    actorInfo = (EQActorInfo_guess *)this->player->actor_info;
    actorInfo->unk_0x187 = 0;
    return;
}

// FUNCTION: TRILOGY 0x004228E2
// Nearest corpse, or nearest NPC of given body types that is not a player's pet, for spell effects.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall (`this` in ECX, EDX unused). Nearest spawn to this character for search mode 0 (corpses: types 2 and 3)
// or 1 (NPCs of type 1 with one of the three body types, whose pet owner is not this character and not a player: no
// owner, an unknown owner, or a non-player owner). Other modes, or no player record, return 0. Returns the spawn or
// 0; only a distance below 10000 counts.
int __fastcall EQ_Character_FindClosest_guess(EQ_Character* eqChar, int unused_edx,
    int searchMode, int param_3, int param_4, int param_5)
{
    int spawnBodyType;
    int iVar2;
    unsigned int ownerId;
    EQPlayer* pPlayer;
    float dist;
    float fClosest;
    EQPlayer* pOwner;

    pPlayer = g_player_top_guess;
    iVar2 = 0;
    fClosest = 10000.0f;
    if (eqChar->player == 0)
    {
        return 0;
    }
    if (searchMode == 0)
    {
        for (; pPlayer != 0; pPlayer = pPlayer->next)
        {
            if (pPlayer->type == 2 || pPlayer->type == 3)
            {
                dist = EQ_METHOD(7, float, 0x00412984, (void*, float, float, float, float, float, float, float), g_display_guess)(
                        eqChar->player->y, eqChar->player->x, eqChar->player->z,
                        pPlayer->y, pPlayer->x, pPlayer->z, 0.0f);
                if (dist < fClosest)
                {
                    fClosest = dist;
                    iVar2 = (int)pPlayer;
                }
            }
        }
    }
    else if (searchMode == 1)
    {
        for (; pPlayer != 0; pPlayer = pPlayer->next)
        {
            if (pPlayer->type == 1)
            {
                spawnBodyType = (int)pPlayer->body_type;
                if (spawnBodyType == param_3 || spawnBodyType == param_4 || spawnBodyType == param_5)
                {
                    ownerId = pPlayer->pet_owner_id;
                    /* ownerId is unsigned, so "<= 0" is "== 0", but VC6 emits the original's test/jbe for this form */
                    if (ownerId != eqChar->player->spawn_id &&
                        (ownerId <= 0 ||
                        (pOwner = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int), g_player_top_guess)(ownerId)) == 0 ||
                         pOwner->type != 0))
                    {
                        dist = EQ_METHOD(7, float, 0x00412984, (void*, float, float, float, float, float, float, float), g_display_guess)(
                                eqChar->player->y, eqChar->player->x, eqChar->player->z,
                                pPlayer->y, pPlayer->x, pPlayer->z, 0.0f);
                        if (dist < fClosest)
                        {
                            fClosest = dist;
                            iVar2 = (int)pPlayer;
                        }
                    }
                }
            }
        }
    }
    return iVar2;
}

typedef struct DeathSource_guess
{
    u8 unknown0x00[4];
    u32 killer_id;
    u8 unknown0x08[0x14];
    u8 skill;
    u8 unknown0x1d[3];
    s16 spell_id;
} DeathSource_guess;

// FUNCTION: TRILOGY 0x00422a48
// Reports the character's death to the server and game, including the killer and attack details.
void __fastcall EQ_Character_SendDeathMessage_guess(EQ_Character* character, int unused_edx,
    const DeathSource_guess* source, s16 damage)
{
    MsgDeath death;
    u32 killerId;

    if (character->unknown0x0076 != 4)
    {
        if (character->unknown0x0076 != 7)
        {
            character->unknown0x0076 = 4;
        }
        death.level = character->level;
        death.spell_id = source->spell_id;
        death.skill = source->skill;
        killerId = source->killer_id;
        death.damage = damage;
        death.spawn_id = character->player->spawn_id;
        death.is_pc = 0;
        death.killer_id = killerId;
        if (character->player->type == 0)
        {
            death.is_pc = 1;
        }
        send_message_guess
            (g_world_connection_guess, 0x204a, &death, 0x14, 1);
        EQ_METHOD(1, void, 0x004C555F, (void*, MsgDeath*), pinstCEverQuest_guess)(&death);
    }
}

// FUNCTION: TRILOGY 0x00423deb
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::TotalOnPerson, low)
// __thiscall. The coins carried, in copper.
int EQ_Character::TotalOnPerson(void)
{
    return ((platinum * 10 + gold) * 10 + silver) * 10 + copper;
}

// FUNCTION: TRILOGY 0x00423e16
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::ItemSold, low)
// __thiscall. Adds a price in copper to the carried coins, split into platinum, gold, silver, copper.
void EQ_Character::ItemSold_guess(int price)
{
    int goldDivisor = 100;
    int silverDivisor = 10;
    int platinumAmount = price / 1000;
    price -= platinumAmount * 1000;

    int goldAmount = price / goldDivisor;
    price -= goldAmount * goldDivisor;

    int silverAmount = price / silverDivisor;
    platinum += platinumAmount;
    gold += goldAmount;
    silver += silverAmount;
    price -= silverAmount * silverDivisor;
    copper += price;
}

struct FallDamagePacket_guess
{
    u32 spawnId;
    u32 unknown04;
    u16 unknown08;
    u8 unknown0A[2];
    int damage;
    float unknown10;
    float unknown14;
    float unknown18;
    u8 flags;
    u8 unknown1D[3];
    u16 unknown20;
    u8 unknown22;
    u8 unknown23;
};

class EQ_Character_FallDamage_guess : public EQ_Character
{
public:
    short TakeFallDamage(float verticalVelocity);
};

#define FALL_DAMAGE_CAP 10000

// FUNCTION: TRILOGY 0x0042444c
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
short EQ_Character_FallDamage_guess::TakeFallDamage(float verticalVelocity)
{
    FallDamagePacket_guess damagePacket;
    float damageModifier = 0.0f;
    short noDamage = 0;
    short damageAmount = 0;
    float weightModifier;
    float adjustedVelocity;

    weightModifier = (float)(short)EQ_METHOD(0, int, 0x0041BC98, (EQ_Character*), this)()
        * EQ_GLOBAL(float, 0x00535224);
    damagePacket.unknown23 = (u8)noDamage;
    adjustedVelocity = (weightModifier + verticalVelocity) - EQ_GLOBAL(float, 0x00535198);
    if (adjustedVelocity >= EQ_GLOBAL(float, 0x0053536C))
    {
        damageAmount = FALL_DAMAGE_CAP;
    }
    else
    {
        if (adjustedVelocity > EQ_GLOBAL(float, 0x0053510C))
        {
            damageModifier = adjustedVelocity * EQ_GLOBAL(float, 0x00535124);
            damageAmount = (short)(int)(damageModifier * adjustedVelocity);
            if (damageAmount > noDamage)
            {
                if (EQ_METHOD(1, int, 0x004246A9, (EQ_Character*, int), this)(0x27) != 0)
                {
                    damageAmount -= skills[0x27];
                    send_message_guess
                        (g_world_connection_guess, 0x21AB, 0, 0, 1);
                }
                if (damageAmount > noDamage)
                    goto applyFallDamage;
            }
        }
        damageAmount = noDamage;
        return damageAmount;
    }

applyFallDamage:
    EQ_CDECL(void, 0x004A3D01, (const char*, int, double, double, double, double, double))
        ((const char*)0x00541FFC, (int)damageAmount, (double)player->dz, (double)damageModifier,
            (double)verticalVelocity, (double)weightModifier, (double)adjustedVelocity);

    damagePacket.spawnId = player->spawn_id;
    damagePacket.unknown20 = 0xFFFF;
    damagePacket.unknown10 = 0.0f;
    damagePacket.unknown14 = 0.0f;
    damagePacket.unknown18 = 0.0f;
    damagePacket.unknown04 = 0;
    damagePacket.unknown08 = 0;
    damagePacket.flags = 0xFC;
    damagePacket.damage = damageAmount;
    damageAmount = (short)EQ_METHOD(4, int, 0x004247F0,
        (EQ_Character*, int, FallDamagePacket_guess*, int, int), this)
        ((int)damageAmount, &damagePacket, 1, -1);
    return damageAmount;
}

#undef FALL_DAMAGE_CAP

  // FUNCTION: TRILOGY 0x004246a9
// Delegates to HasSkill. The low-confidence TAKP HasSkill2 name has no separate behavior.
/* fake match: callee is __thiscall, called as __stdcall; ECX still holds this, so no dummy EDX is loaded. */
int EQ_Character::HasSkillPassthrough_guess(int skill)
{
    // Preserve the callee's full EAX; the nearby typed HasSkill declaration returns only AL.
    return EQ_STDCALL(int, 0x0041A3E9, (int))(skill);
}

// FUNCTION: TRILOGY 0x004246b5
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// static (__cdecl). 1 when the character's spawn is an NPC (type 1) of level 53 or more.
int EQ_Character::IsHighLevelNPC_guess(EQ_Character* ch)
{
    if (ch != 0 && ch->player != 0 && ch->player->type == 1 && ch->level >= 53)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004246dd
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall (implemented as __fastcall because the method is not declared in the header).
void __fastcall EQ_Character_StunMe_guess(EQ_Character* ch, int unused_edx, u32 duration)
{
    short spell_affect;
    int current_time;
    u32* stun_timer;
    EQEnvironment_guess* environment;

    if (ch->player != 0 && ch->player->actor_info != 0
        && EQ_CDECL(int, 0x004246B5, (EQ_Character*))(ch) == 0
        && (spell_affect = EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), ch)(0x28)) <= 0)
    {
        if (duration < 1000)
        {
            EQ_CDECL(void, 0x004A3D01, (const char*))( (const char*)0x00542074);
            if (ch->player == 0)
                return;
            if (ch->player->type != 0)
                return;
            duration = 1000;
        }
        if (ch->player->actor_info->stunTimer_guess == 0)
        {
            current_time = EQ_CDECL(int, 0x004A461E, (void))();
            stun_timer = &ch->player->actor_info->stunTimer_guess;
            if (*stun_timer < current_time + duration)
                *stun_timer = current_time + duration;
            ch->unknown0x0076 = 1;
            environment = g_environment_guess;
            if (environment != 0)
                EQ_METHOD(1, void, 0x004E1F74, (EQEnvironment_guess*, int), environment)(6);
            ch->player->speed = 0.0f;
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((const char*)0x00542060, 0x111);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004247b1
// Ends the character's stun and displays the recovery message.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// __thiscall (no declaration in the header, so a __fastcall free function). Ends the stun: resets the
// environment state, clears the dword at actor_info+0xC0 and the life state byte, prints the message.
void __fastcall CDisplay_SetGenericEnvironment_StunRelated_guess(EQ_Character* ch)
{
    if (g_environment_guess != 0)
        g_environment_guess->SetState_guess(-1);
    ch->player->actor_info->stunTimer_guess = 0;
    ch->unknown0x0076 = 0;
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)("You are no longer stunned.", 0x111);
}

// FUNCTION: TRILOGY 0x004247f0
// Applies incoming damage to the character: absorbing buffs, HP update, stun chance and the hit report.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
short __fastcall EQ_Character_ApplyDamage_guess(EQ_Character* self, int unused_edx,
                                                 int damage, int* damage_data,
                                                 char report_hit, int unused_stack)
{
    int minimum_hp;
    int current_hp;
    struct DamageHitMessage_guess
    {
        int source_id;
        int target_id;
        u8 skill;
        short spell_id;
        int damage;
        int parameter_4;
        int parameter_5;
        int parameter_6;
    } hit_message;
    char non_melee_message[256];
    u8 value;
    short spell_id;
    int attacker_id;
    EQPlayer* attacker;
    EQPlayer* target;
    int spell;
    float* damage_multiplier;
    int affect;
    int count;
    int amount;
    bool should_stun;

    if (self->player == 0 || self->player->stand_state == 'f'
        || self->player->linkdead != 0 || self->unknown0x0076 == 4)
        return 0;

    if (self->race == 0x8d || self->race == 0x48
        || self->race == 0x49 || self->race == 0x72)
        return 0;

    attacker = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int),
        HWM_PLAYER_TOP)(damage_data[1]);
    spell_id = (short)damage_data[8];
    if (EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), self)(0x28) > 0
        && spell_id != 0x3d6)
        damage = -5;

    if (self->player == 0 || self->player->type != 1)
        minimum_hp = -10;
    else
        minimum_hp = 0;

    if (self->player != 0
        && EQ_METHOD(1, int, 0x004199C0, (EQ_Character*, int), self)(self->player->type) < minimum_hp)
        return 0;

    if (damage < 1)
        return (short)damage;

    if (self->player != 0 && self->player->type == 0
        && (char)damage_data[7] == -0x19)
    {
        spell_id = (short)damage_data[8];
        spell = 0;
        if (spell_id > 0 && spell_id < 3000)
            spell = (int)EQ_ARRAY(void*, 0x005FE9A0)[spell_id];
        target = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int),
            HWM_PLAYER_TOP)(damage_data[1]);
        if (spell != 0 && *(u8*)((u8*)spell + 0x21f) == 0
            && target != 0 && target->type == 0)
        {
            __asm fild damage
            if (target->class_ == 3 || target->class_ == 8
                || target->class_ == 4 || target->class_ == 5)
            {
                if ((short)damage_data[8] == 0x58)
                    damage_multiplier = EQ_ARRAY(float, 0x00535370);
                else
                    damage_multiplier = EQ_ARRAY(float, 0x00535164);
            }
            else
            {
                value = target->class_;
                count = 0x27;
                if (value >= 1 && value <= 0x0f)
                    count = *((u8*)((u8*)spell + 0x234) + value);
                if (count < 0x0e)
                    damage_multiplier = EQ_ARRAY(float, 0x0053537C);
                else if (count < 0x18)
                    damage_multiplier = EQ_ARRAY(float, 0x00535378);
                else if (count < 0x27)
                    damage_multiplier = EQ_ARRAY(float, 0x00535370);
                else
                    damage_multiplier = EQ_ARRAY(float, 0x00535374);
            }
            __asm {
                mov eax, damage_multiplier
                fld dword ptr [eax]
                fld st(1)
                fmul st(0), st(1)
            }
            spell_id = EQ_CDECL(short, 0x004EF0CF, (void))();
            __asm {
                fstp st(0)
                fstp st(0)
            }
            damage = (int)spell_id;
            if (damage < 1)
                damage = 1;
        }
    }

    if (self->player != 0 && self == EQ_GLOBAL(EQ_Character*, 0x006E4DA8)
        && ((u8)damage_data[7] == 0xf6
            || ((u8)damage_data[7] > 0xf9 && (u8)damage_data[7] < 0xfe))
        && EQ_GLOBAL(u8, 0x006B63F8) == 0
        && DAT_006b6494 == 0)
    {
        send_message_guess
            (g_world_connection_guess, 0x201e, damage_data, 0x24, 1);
    }

    if (EQ_CDECL(int, 0x004A42D4, (int*))(damage_data) == 0)
    {
        SOC_FMT
            (non_melee_message, EQ_ARRAY(char, 0x005420C4), damage);
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int),
            EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(non_melee_message, 0x11b);
    }

    affect = (int)EQ_METHOD(2, SpellBuff*, 0x0042381B,
        (EQ_Character*, u8, short*), self)(0x1f, 0);
    if (affect != 0)
    {
        EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), self)((void*)affect);
        EQ_METHOD(0, void, 0x004247B1, (EQ_Character*), self)();
    }

    affect = (int)EQ_METHOD(2, SpellBuff*, 0x0042381B,
        (EQ_Character*, u8, short*), self)(0x40, 0);
    if (affect != 0)
    {
        count = 10;
        if (*(short*)(affect + 4) == 0x12f)
        {
            value = self->level;
            if (value < 0x32 && value < 0x2d && value < 0x28)
            {
                if (value < 0x1f)
                {
                    if (0x18 < value)
                        count = 0xc;
                }
                else
                    count = 2;
            }
            else
                count = 1;
        }
        else if (*(short*)(affect + 4) == 0x26b)
            count = 0x14;
        count = EQ_CDECL(int, 0x004A46F6, (int, int))(0, count - 1);
        if (count == 0)
        {
            EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), self)((void*)affect);
            EQ_METHOD(0, void, 0x004247B1, (EQ_Character*), self)();
        }
    }

    affect = (int)EQ_METHOD(2, SpellBuff*, 0x0042381B,
        (EQ_Character*, u8, short*), self)(0x3b, 0);
    if (affect != 0 && damage_data[1] != 0
        && damage_data[1] != (int)self->player->spawn_id
        && EQ_CDECL(int, 0x004A42D4, (int*))(damage_data) != 0)
    {
        spell_id = *(short*)(affect + 4);
        if (spell_id < 0 || spell_id > 3000)
            spell = 0;
        else
            spell = (int)EQ_ARRAY(void*, 0x005FE9A0)[spell_id];
        if (spell != 0 && EQ_METHOD(1, short, 0x004238A2,
            (EQ_Character*, int), self)(0x3b) < 0)
        {
            spell_id = EQ_METHOD(1, short, 0x004238A2,
                (EQ_Character*, int), self)(0x3b);
            EQ_CDECL(int, 0x005232FF, (int))(spell_id);
        }
    }

    if ((char)damage_data[7] == -4 || (char)damage_data[7] == -5
        || (char)damage_data[7] == -6)
        goto apply_to_hit_points;

    affect = (int)EQ_METHOD(2, SpellBuff*, 0x0042381B,
        (EQ_Character*, u8, short*), self)(0x37, 0);
    if (affect == 0)
    {
        if ((char)damage_data[7] == -0x19)
            affect = (int)EQ_METHOD(2, SpellBuff*, 0x0042381B,
                (EQ_Character*, u8, short*), self)(0x4e, 0);
        if (affect == 0)
            goto apply_to_hit_points;
    }

    damage = *(short*)(affect + 8) - damage;
    if (damage < -0x7fff)
        damage = -0x7fff;
    *(short*)(affect + 8) = (short)damage;
    if (damage < 1)
    {
        damage = EQ_CDECL(int, 0x005232FF, (int))((int)(short)damage);
        attacker_id = (int)attacker;
        if ((char)damage_data[7] != -0x19)
            attacker_id = 0;
        EQ_METHOD(2, void, 0x0041976F, (EQ_Character*, int, int), self)(-damage, attacker_id);
        EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), self)((void*)affect);
    }
    else
        damage = -6;
    goto update_hp;

apply_to_hit_points:
    attacker_id = (int)attacker;
    if ((char)damage_data[7] != -0x19)
        attacker_id = 0;
    EQ_METHOD(2, void, 0x0041976F, (EQ_Character*, int, int), self)(-damage, attacker_id);

update_hp:
    current_hp = EQ_METHOD(1, int, 0x004199C0,
        (EQ_Character*, int), self)(self->player->type);
    if (self->player != 0)
    {
        self->player->cur_hp = current_hp;
        self->player->max_hp = EQ_METHOD(1, int, 0x00419A2F,
            (EQ_Character*, int), self)(self->player->type);
    }
    if (current_hp < 1 && self->player != 0 && (char)self->unknown0x0076 < 3)
        EQ_METHOD(0, void, 0x004B3E1C, (EQPlayer*), self->player)();

    if (current_hp > 0)
    {
        should_stun = (char)damage_data[7] == 10;
        if (attacker != 0 && (char)damage_data[7] == 0x1e
            && attacker->level > 0x36 && attacker->class_ == 1)
            should_stun = true;
        if (should_stun && self->race != 10 && attacker != 0 && self->race != 0x5d)
        {
            count = EQ_METHOD(1, int, 0x0041A75E,
                (EQ_Character*, int), self)(attacker->level);
            amount = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 100);
            if (amount < 1)
                amount = 1;
            if (amount < 99 && amount <= count)
            {
                amount = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 9);
                if (amount > 1 && self->player->actor_info->unknown0x248 != -1
                    && (self->player->actor_info->unknown0x248 < 0x35b
                        || self->player->actor_info->unknown0x248 > 0x3ff)
                    && EQ_CDECL(int, 0x004246B5, (EQ_Character*))(self) == 0)
                    EQ_METHOD(1, void, 0x0042798F, (EQ_Character*, int), self)(1);
            }
            else
                EQ_METHOD(1, void, 0x004246DD, (EQ_Character*, int), self)(2000);
        }
    }

    if (minimum_hp >= current_hp)
    {
        EQ_METHOD(2, void, 0x00422A48, (EQ_Character*, int*, int), self)(damage_data, damage);
        return (short)damage;
    }

    attacker_id = damage_data[1];
    amount = minimum_hp;
    if (attacker_id == damage_data[0] && self->player != 0 && self->player->type == 1)
    {
        spell_id = (short)damage_data[8];
        amount = (int)spell_id;
        if ((spell_id < 1 || spell_id > 2999) && damage > 5000)
            report_hit = 0;
    }
    if (report_hit != 0)
    {
        hit_message.source_id = damage_data[0];
        hit_message.target_id = damage_data[1];
        hit_message.skill = (u8)damage_data[7];
        hit_message.spell_id = (short)damage_data[8];
        hit_message.damage = damage;
        hit_message.parameter_4 = damage_data[4];
        hit_message.parameter_5 = damage_data[5];
        hit_message.parameter_6 = damage_data[6];
        EQ_GLOBAL(int, 0x006B6460) = attacker_id;
        if (EQ_GLOBAL(u8, 0x006B63F8) == 0 && DAT_006b6494 == 0)
            send_message_guess
                (g_world_connection_guess, 0x2058, &hit_message, 0x1c, 1);
        EQ_METHOD(2, void, 0x004C5E50, (CEverQuest*, void*, int),
            EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(&hit_message, 1);
    }
    return (short)damage;
}

// FUNCTION: TRILOGY 0x004251b9
// Sends the server the current data for one character spell effect slot.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
void __fastcall EQ_Character_NotifyPCAffectChange_guess(EQ_Character* ch, int unused_edx, int affectSlot)
{
    struct NotifyPCAffectMessage
    {
        int affect_id;
        u8 affect_data[12];
        int affect_slot;
    } message;

    if (*(int*)((u8*)ch + 0xa0) != 0)
    {
        message.affect_id = *(int*)(*(u8**)((u8*)ch + 0xa0) + 0x80);
        EQ_CDECL(void*, 0x00521B10, (void*, const void*, unsigned int))(
            message.affect_data, (u8*)ch + 0x284 + affectSlot * 10, 10);
        message.affect_slot = affectSlot;
        send_message_guess(
            g_world_connection_guess, 0x2132, &message, 0x14, 1);
    }
}

// FUNCTION: TRILOGY 0x0042577F
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// The two flag bytes are written as AND 0 / OR 0xff, the encoding the original uses for them.
bool __fastcall EQ_Character_CanISeeInvis_guess(EQ_Character* ch, int unused_edx)
{
    short affectTotal;
    short tmp;
    bool result;

    affectTotal = EQ_METHOD(1, short, 0x004238A2, (void*, u8), (void*)ch)(0xd);
    result = 0 < affectTotal;
    tmp = EQ_METHOD(1, short, 0x004238A2, (void*, u8), (void*)ch)(0x42);
    if (0 < tmp)
    {
        ch->innateSkills_guess[13] &= 0;
    }
    else
    {
        u8* spawn = *(u8**)((u8*)ch + 0xa0);
        if (spawn != 0)
        {
            tmp = *(short*)(spawn + 0x9a);
            if (tmp != 6 && tmp != 0x82)
            {
                ch->innateSkills_guess[13] |= 0xff;
            }
        }
    }
    tmp = EQ_METHOD(1, short, 0x004238A2, (void*, u8), (void*)ch)(0x41);
    if (0 < tmp)
    {
        ch->innateSkills_guess[6] &= 0;
    }
    else
    {
        u8* spawn = *(u8**)((u8*)ch + 0xa0);
        if (spawn != 0)
        {
            tmp = *(short*)(spawn + 0x9a);
            if (tmp == 6 || tmp == 1 || tmp == 3 || tmp == 2)
            {
                ch->innateSkills_guess[6] |= 0xff;
            }
        }
    }
    return result;
}

// FUNCTION: TRILOGY 0x00425817
// Checks whether the character can breathe underwater using race and spell effects.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
bool __fastcall EQ_Character_CanIBreathe_guess(EQ_Character* ch, int unused_edx)
{
    short temp;

    if ((((*(u8*)(*(int*)(*(int*)((u8*)ch + 0xa0) + 0x70) + 0x98) >= 5) &&
          (temp = *(short*)((u8*)ch + 0x34), temp != 0x4a)) && (temp != 0x18)) && (temp != 0x23))
    {
        // the EDX slot passes this function's EDX on unchanged: the original never writes EDX before the call
        temp = EQ_THISCALL(short, 0x004238A2, (void*, int, u8))((void*)ch, unused_edx, 0xe);
        if (0 < temp)
        {
            return 1;
        }
        ch->cannotBreathe_guess = 1;
        return 0;
    }
    ch->cannotBreathe_guess = 0;
    return 1;
}

struct EnvironmentDamage_guess
{
    u32 source_spawn_id;
    u32 unknown0x04;
    u16 unknown0x08;
    int amount;
    float unknown0x10;
    float unknown0x14;
    float unknown0x18;
    u8 damage_type;
    u8 unknown0x1d[3];
    u16 unknown0x20;
    u8 unknown0x22;
    u8 trailing_flag;
};

// FUNCTION: TRILOGY 0x00425945
// Applies periodic fire or cold environmental damage according to surroundings and resistances.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
void __fastcall EQ_Character_ProcessEnvironment_guess(EQ_Character* character, int unused_edx)
{
    EQ_Character* current_character = character;
    u8 environment_class;
    short damage_amount;
    EnvironmentDamage_guess damage;

    damage.trailing_flag = 0;
    environment_class = current_character->player->actor_info->unknown0x099;
    if (environment_class > 2 && environment_class != 5)
    {
        if (environment_class == 3 || environment_class == 7)
        {
            damage_amount = (short)(50 - (short)(EQ_METHOD(1, int, 0x0041A962, (EQ_Character*, u8), current_character)(current_character->level) / 3));
            if (damage_amount < 10)
                damage_amount = 10;
            if (damage_amount > 0)
            {
                damage.source_spawn_id = current_character->player->spawn_id;
                damage.unknown0x04 = 0;
                damage.unknown0x10 = 0.0f;
                damage.unknown0x08 = 0;
                damage.unknown0x20 = 0xffff;
                damage.unknown0x14 = 0.0f;
                damage.amount = (int)damage_amount;
                damage.unknown0x18 = 0.0f;
                damage.damage_type = 0xfa;
                EQ_METHOD(4, void, 0x004247F0, (EQ_Character*, int, EnvironmentDamage_guess*, int, int), current_character)((int)damage_amount, &damage, 1, 0xffffffff);
            }
        }
        if (current_character->player->actor_info->unknown0x099 == 4 || current_character->player->actor_info->unknown0x099 == 8)
        {
            damage_amount = (short)(50 - (short)(EQ_METHOD(1, int, 0x0041AB4B, (EQ_Character*, u8), current_character)(current_character->level) / 3));
            if (damage_amount < 10)
                damage_amount = 10;
            if (damage_amount > 0)
            {
                damage.source_spawn_id = current_character->player->spawn_id;
                damage.unknown0x04 = 0;
                damage.unknown0x10 = 0.0f;
                damage.unknown0x08 = 0;
                damage.unknown0x20 = 0xffff;
                damage.unknown0x14 = 0.0f;
                damage.amount = (int)damage_amount;
                damage.unknown0x18 = 0.0f;
                damage.damage_type = 0xf6;
                EQ_METHOD(4, void, 0x004247F0, (EQ_Character*, int, EnvironmentDamage_guess*, int, int), current_character)((int)damage_amount, &damage, 1, 0xffffffff);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00425A6A
// Updates hunger and thirst, auto-eats when possible, and reports food and drink status.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, callee pops 4. Hunger and thirst tick. Flag 0xF1 on the player sets both stat words
// (0xAF8, 0xAFC) to 32000. Otherwise, when the last tick (player->0x70->0x60) is older than 46000 ms
// (92000 if the byte at 0x36 is 7) and no lock is set: name checks, thirsty and hungry AutoEat checks,
// tick stamp, then the food/drink status line.
// @early-stop: the 0x111 colour lives in EDI, not EBP (original loads it right after the first strcmpi); dummy EDX xors remain.
void __fastcall EQ_Character_ProcessHungerandThirst_guess(EQ_Character* ch, int unused_edx, int now)
{
    u8* player;
    u32 limit;
    char* name;
    int cmp;
    u32 tick;
    short status;

    player = *(u8**)((u8*)ch + 0xa0);
    if (player != 0 && *(player + 0x98) == 0 && ch != 0)
    {
        if (*(player + 0xf1) != 0)
        {
            *(int*)((u8*)ch + 0xafc) = 32000;
            *(int*)((u8*)ch + 0xaf8) = 32000;
        }
        else
        {
            limit = 46000;
            if (*((u8*)ch + 0x36) == 7)
            {
                limit = 92000;
            }
            if (EQ_GLOBAL(u8, 0x006B63F8) == 0 && DAT_006b6494 == 0
                && (u32)(now - *(int*)(*(u8**)(player + 0x70) + 0x60)) > limit)
            {
                name = (char*)ch + 0x974;
                cmp = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, (const char*)0x0053DF4C);
                if (cmp != 0)
                {
                    cmp = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, (const char*)0x00542294);
                    if (cmp != 0)
                    {
                        EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, (const char*)0x0053DF64);
                    }
                }
                if (*(int*)((u8*)ch + 0xafc) < 3000 && HWM_EQMAIN != 0
                    && pinstEQ_Main_guess->pendingServerReplies_guess == 0
                    && pinstEQ_Main_guess->pendingWorldRequests_guess == 0
                    && EQ_METHOD(1, int, 0x00428193, (void*, int), (void*)ch)(0) == 0)
                {
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00542280, 0x111);
                }
                if (*(int*)((u8*)ch + 0xaf8) < 3000 && HWM_EQMAIN != 0
                    && pinstEQ_Main_guess->pendingServerReplies_guess == 0
                    && pinstEQ_Main_guess->pendingWorldRequests_guess == 0
                    && EQ_METHOD(1, int, 0x00428193, (void*, int), (void*)ch)(1) == 0)
                {
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00542270, 0x111);
                }
                tick = EQ_CDECL(u32, 0x004A461E, (void))();
                *(u32*)(*(u8**)(*(u8**)((u8*)ch + 0xa0) + 0x70) + 0x60) = tick;
                status = EQ_METHOD(0, short, 0x00428405, (void*), (void*)ch)();
                switch (status)
                {
                case 1:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00542258, 0x111);
                    break;
                case 2:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00542240, 0x111);
                    break;
                default:
                    return;
                case 4:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00542228, 0x111);
                    break;
                case 5:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00542208, 0x111);
                    break;
                case 6:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005421E0, 0x111);
                    break;
                case 8:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005421C8, 0x111);
                    break;
                case 9:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005421A0, 0x111);
                    break;
                case 10:
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00542180, 0x111);
                }
            }
        }
    }
}

// FUNCTION: TRILOGY 0x00425c48
// Totals a requested effect from proc spells on the character's equipped items.
int __cdecl EQ_Character_ProcEffectTotal_guess(int eqCharacter, u32 effectId)
{
    u8 slotEffect;
    int item;
    short change;
    int spell;
    int amount;
    int remaining;
    int maxChange;
    int* itemPtr;
    u8* effectPtr;
    int total;
    u8 effectIndex;

    maxChange = 0;
    total = 0;
    itemPtr = (int*)(eqCharacter + 0xe4);
    remaining = 0x15;
    do
    {
        item = *itemPtr;
        if ((item != 0) && (*(s8*)(item + 0x81) == 0))
        {
            spell = (int)*(short*)(item + 0xce);
            if (((*(u8*)(item + 0xd8) >= 2) &&
                (((*(s8*)(item + 0xc0) == 2 && (0 < spell)) && (spell < 3000)))) &&
                ((spell = EQ_ARRAY(int, 0x005FE9A0)[spell], spell != 0 && (*(int*)(spell + 0x1bc) != 0))))
            {
                effectIndex = 0;
                effectPtr = (u8*)(spell + 0x222);
                do
                {
                    slotEffect = *effectPtr;
                    if (slotEffect != 0xfe)
                    {
                        if (effectId == 0xf)
                        {
                            if (slotEffect == 0xf)
                            {
                                change = EQ_METHOD(4, short, 0x0042351A, (void*, int, u8, u8, int), (void*)eqCharacter)(spell, *(u8*)(item + 0xc4), effectIndex, item + 0xd8);
                                amount = (int)change;
                                if (amount > maxChange)
                                {
                                    maxChange = amount;
                                    total = amount;
                                }
                            }
                        }
                        else if (slotEffect == effectId)
                        {
                            change = EQ_METHOD(4, short, 0x0042351A, (void*, int, u8, u8, int), (void*)eqCharacter)(spell, *(u8*)(item + 0xc4), effectIndex, item + 0xd8);
                            total = total + change;
                        }
                    }
                    effectIndex++;
                    effectPtr = effectPtr + 1;
                } while (effectIndex < 0xc);
            }
        }
        itemPtr = itemPtr + 1;
        remaining = remaining + -1;
    } while (remaining != 0);
    return total;
}

typedef struct PassageHpUpdate_guess
{
    u32 spawn_id;               /* 0x00 */
    u16 type;                   /* 0x04 */
    u8  unknown0x06[2];         /* 0x06 */
    s32 value;                  /* 0x08 */
} PassageHpUpdate_guess;

typedef struct PassageDeath_guess
{
    u32 spawn_id;               /* 0x00 */
    u32 killer_id;              /* 0x04 */
    u16 unknown0x08;            /* 0x08 */
    u8  unknown0x0A[2];         /* 0x0A */
    s32 damage;                 /* 0x0C */
    f32 unknown0x10;            /* 0x10 */
    f32 unknown0x14;            /* 0x14 */
    f32 unknown0x18;            /* 0x18 */
    u8  skill;                  /* 0x1C */
    u8  unknown0x1D[3];         /* 0x1D */
    s16 spell_id;               /* 0x20 */
    u8  unknown0x22;            /* 0x22 */
    u8  unknown0x23;            /* 0x23 */
} PassageDeath_guess;

#define PT_FIELD(type, base, offset)    (*(type*)((char*)(base) + (offset)))
#define PT_EQMAIN(type, offset)         PT_FIELD(type, g_eqmain_guess, offset)
#define PT_GEM_STATE(gem)               (((u32*)(g_eqmain_guess + 0xE60))[gem])
#define PT_GEM_READY(gem)               (((u32*)(g_eqmain_guess + 0xE80))[gem])
#define PT_UI_DIRTY                     PT_EQMAIN(int, 0xEB8)
#define PT_SPELLS                       EQ_ARRAY(char*, 0x005FE9A0)
#define PT_STATIC_GUARD                 EQ_GLOBAL(u8, 0x00591A88)
#define PT_SERVER_FLAG                  EQ_GLOBAL(u8, 0x006B6405)
#define PT_LAST_SERVER_FLAG             EQ_GLOBAL(u32, 0x00591AC4)
#define PT_NEXT_MINUTE                  EQ_GLOBAL(u32, 0x00591C78)
#define PT_TARGET                       EQ_GLOBAL(EQPlayer*, 0x006E4DAC)
#define PT_LAST_TARGET                  EQ_GLOBAL(EQPlayer*, 0x00591D90)
#define PT_LAST_710C                    EQ_GLOBAL(u8, 0x00591D94)
#define PT_LAST_710D                    EQ_GLOBAL(u8, 0x00591D95)
#define PT_LAST_FE4                     EQ_GLOBAL(u8, 0x00591D9C)
#define PT_LAST_MINUTE                  EQ_GLOBAL(u32, 0x006B644C)
#define PT_POISON_START                 EQ_GLOBAL(u32, 0x006B6478)
#define PT_POISON_ITEM                  EQ_GLOBAL(u32, 0x006B647C)
#define PT_CAMP_TIME                    EQ_GLOBAL(u32, 0x006B6590)
#define PT_ENVIRONMENT                  EQ_GLOBAL(EQEnvironment_guess*, 0x006E4DDC)
#define PT_FIST                         EQ_GLOBAL(EQ_Equipment*, 0x005FE850)
#define PT_SKILL_TIMERS                 EQ_ARRAY(u32, 0x006E0FEC)
#define PT_SKILL_READY                  EQ_ARRAY(u8, 0x006E117C)
#define PT_HIDDEN                       EQ_GLOBAL(u8, 0x006B65D8)
#define PT_ABILITY                      EQ_GLOBAL(s16, 0x006B6772)
#define PT_TRACK_TARGET                 EQ_GLOBAL(EQPlayer*, 0x006B6780)
#define PT_DISPLAY_TIME                 PT_FIELD(u32, g_displayManager_guess, 0xBC)
#define PT_CAMP_SECONDS                 EQ_GLOBAL(u32, 0x00591AA4)
#define PT_BOAT_COUNTER                 EQ_GLOBAL(int, 0x00591D8C)
#define PT_BOAT_SENT                    EQ_GLOBAL(int, 0x00591D98)
#define PT_MACRO_STATE                  EQ_GLOBAL(int, 0x0053F230)
#define PT_SEND(opcode, data, size)     send_message_guess(g_world_connection_guess, (opcode), (data), (size), 1)
#define PT_DSP_CHAT(text, color) \
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((text), (color))
#define PT_BARD_CAST_BARD() \
    EQ_METHOD(0, int, 0x0046A72F, (char*), g_eqmain_guess)()
#define PT_STOP_SPELL_CAST(obj, flag) \
    EQ_METHOD(1, void, 0x0042798F, (void*, int), (void*)(obj))((flag))
#define PT_MOUSE_UP(obj, button) \
    EQ_METHOD(3, int, 0x004501D9, (void*, int, short, short), (void*)(obj))( \
        (button), EQ_GLOBAL(short, 0x006B6414), EQ_GLOBAL(short, 0x006B6416))
#define PT_SET_ENV_STATE(state) \
    EQ_METHOD(1, void, 0x004E1F74, (EQEnvironment_guess*, int), PT_ENVIRONMENT)((state))
#define PT_CUR_HP(arg)                  EQ_METHOD(1, int, 0x004199C0, (EQ_Character*, int), this)((arg))
#define PT_MAX_HP(arg)                  EQ_METHOD(1, int, 0x00419A2F, (EQ_Character*, int), this)((arg))
#define PT_MODIFY_CUR_HP(delta)         EQ_METHOD(2, void, 0x0041976F, (EQ_Character*, int, int), this)((delta), 0)
#define PT_TOTAL_SPELL_AFFECTS(kind)    EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), this)((kind))
#define PT_GET_SPELL_AFFECT(effect)     EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, int, short*), this)((effect), 0)
#define PT_HAS_SKILL(skill)             EQ_METHOD(1, u8, 0x004246A9, (EQ_Character*, int), this)((skill))
#define PT_CUR_MANA()                   EQ_METHOD(0, short, 0x00419BA3, (EQ_Character*), this)()
#define PT_WEAPON_DELAY(player, delay, ranged) \
    EQ_METHOD(2, u32, 0x004B2B65, (EQPlayer*, u32, int), (player))((delay), (ranged))
#define PT_SKILL_BASE_DELAY(weapon) \
    ((weapon)->item_type == 22 ? PT_FIELD(u32, g_skill_records_guess[(weapon)->delay], 0x20) : (weapon)->delay * 100)
#define PT_WEAPON_USES_DELAY(weapon) \
    ((weapon)->item_type <= 5 || (weapon)->item_type == 13 || (weapon)->item_type == 22)

// FUNCTION: TRILOGY 0x00425d5a
// Local player's per-frame tick, __thiscall on EQ_Character; most work is time-gated against the current time.
// Order: static-guard setup, UI-refresh flags, a once-a-minute update, the 8 spell-gem timers; then it returns
// early unless player->type is 0 (the own character).
// Then speed and position checks, tracking and hide handling, camp countdown, 1000 and 6000 tick updates on
// actor_info (+0x58, +0x5c), an 18000 tick stamp, and the 6000 tick intoxication countdown.
// Last: the death check (hp below -10), the pending spell cast against actor_info+0xc8, and the macro step.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
void EQ_Character::DoPassageOfTime(void)
{
    char text[96];
    char direction[16];
    PassageDeath_guess death;
    PassageHpUpdate_guess hp_update;
    int poison_request[2];
    u32 now;
    u32 flag_value;
    u32 value;
    short gem;
    short slot;
    short skill;
    int index;
    int count;
    int boat_cleared;
    u32 killer_id;
    int alive;
    u32 since_save;
    EQPlayer* player;
    EQPlayer* track;
    EQPlayer* spawn;
    EQPlayer* ride;
    void* actor;
    EQ_Item* item;
    EQ_Item* cast_item;
    EQ_Item** item_ptr;
    EQ_Container* bag;
    EQ_Equipment* weapon;
    int poison_slot;
    u32 attack_delay;
    u32 secondary_delay;
    u32 last_used;
    u32 elapsed;
    u32 previous_seconds;
    u32 seconds;
    float bearing;
    float relative;
    float diff;
    float forward;
    double side;
    double angle;
    char* name;
    short affect;
    short hp;
    short max_mana;
    short mana_regen;
    u8 meditate;
    int hp_before;
    int hp_mod;
    int regen;
    int hunger;
    int found;
    int* group_entry;
    u16 caster;
    void* regen_buff;
    void* stun;
    int proc;
    int drunk_scale;
    int damage;
    int item_slot;
    int own_item;
    char launched;
    short spell;
    char* spell_record;
    int line;
    int macro;
    int delay;
    int page;
    int text_offset;
    char* rest;

    if ((PT_STATIC_GUARD & 1) == 0)
    {
        PT_STATIC_GUARD |= 1;
        PT_LAST_SERVER_FLAG = PT_SERVER_FLAG;
    }
    boat_cleared = 0;
    killer_id = 0;
    alive = 1;
    now = EqGetTime();
    if ((PT_STATIC_GUARD & 2) == 0)
    {
        PT_STATIC_GUARD |= 2;
        PT_NEXT_MINUTE = now + 60000;
    }
    if (PT_LAST_SERVER_FLAG != PT_SERVER_FLAG)
    {
        PT_LAST_SERVER_FLAG = PT_SERVER_FLAG;
        flag_value = PT_SERVER_FLAG;
        if (PT_SERVER_FLAG > 1)
        {
            flag_value = 0;
        }
        PT_SEND(0x201F, &flag_value, 4);
    }
    if (PT_TARGET != PT_LAST_TARGET)
    {
        PT_LAST_TARGET = PT_TARGET;
        if (PT_TARGET == 0)
        {
            value = 0;
        }
        else
        {
            value = PT_TARGET->spawn_id;
        }
        PT_SEND(0x2162, &value, 4);
    }
    if ((u8)g_attacking_guess != PT_LAST_710C)
    {
        PT_LAST_710C = g_attacking_guess;
        value = (u8)g_attacking_guess;
        PT_SEND(0x2160, &value, 4);
    }
    if ((u8)g_rangedAttacking_guess != PT_LAST_710D)
    {
        PT_LAST_710D = g_rangedAttacking_guess;
        value = (u8)g_rangedAttacking_guess;
        PT_SEND(0x2151, &value, 4);
    }
    if (g_eqmain_guess != 0 && PT_EQMAIN(u32, 0xFE4) != PT_LAST_FE4)
    {
        PT_LAST_FE4 = PT_EQMAIN(u8, 0xFE4);
        value = PT_EQMAIN(u32, 0xFE4);
        PT_SEND(0x2158, &value, 4);
    }
    if (now - PT_LAST_MINUTE > 60000)
    {
        PT_LAST_MINUTE = now;
        this->unknown0x1040++;
        if (PT_SERVER_FLAG > 1)
        {
            PT_SEND(0x216D, EQ_ARRAY(u8, 0x006B6405), 1);
        }
    }

    death.unknown0x23 = 0;
    for (gem = 0; gem < 8; gem++)
    {
        if (PT_GEM_STATE(gem) != 0 && PT_GEM_STATE(gem) < 6 && PT_EQMAIN(int, 0x8) == 0
            && PT_EQMAIN(s16, 0xEA0) == -1 && g_local_player_guess != 0
            && g_local_player_guess->actor_info != 0 && PT_GEM_READY(gem) != 0
            && g_local_player_guess->actor_info->unknown0x248 == -1 && PT_BARD_CAST_BARD() == 0)
        {
            PT_GEM_STATE(gem) = 5;
        }
        if (PT_EQMAIN(int, 0x8) == 0 && PT_GEM_STATE(gem) != 0)
        {
            if (PT_GEM_STATE(gem) != 4)
            {
                PT_GEM_STATE(gem)++;
                PT_UI_DIRTY = 1;
            }
            if (PT_GEM_STATE(gem) == 4 && PT_EQMAIN(s16, 0xEA0) == gem && g_pc_guess != 0)
            {
                sprintf(text, "You forget %s.", PT_SPELLS[g_pc_guess->spell_memorized[gem]]);
                PT_DSP_CHAT(text, 0x108);
                g_pc_guess->spell_memorized[gem] = -1;
                PT_EQMAIN(s16, 0xEA0) = -1;
                PT_GEM_STATE(gem) = 0;
                if (PT_BARD_CAST_BARD() != 0)
                {
                    PT_STOP_SPELL_CAST(g_pc_guess, 0);
                }
                PT_UI_DIRTY = 1;
            }
            if (PT_GEM_STATE(gem) >= 8)
            {
                PT_EQMAIN(s16, 0xEA0) = -1;
                PT_GEM_STATE(gem) = 0;
                PT_UI_DIRTY = 1;
            }
        }
    }

    if (this->unknown0x0076 == 4)
    {
        return;
    }
    player = this->player;
    if (player == 0 || player->actor_info == 0)
    {
        EQTRACE_guess("me is NULL in DoPassageOfTime()!!\n");
        return;
    }
    if (player->type != 0)
    {
        return;
    }
    if (EQ_CDECL(double, 0x0052330A, (double))(player->speed) > 0.001f)
    {
        PT_FIELD(u8, this->player->actor_info, 0x18A) = 0;
    }
    if (this == 0)
    {
        return;
    }
    since_save = now - PT_FIELD(u32, this->player->actor_info, 0x74);
    if (g_eqmain_guess != 0)
    {
        if (PT_POISON_START != 0 && PT_CAMP_TIME == 0 && PT_EQMAIN(int, 0xE5C) == 0
            && PT_EQMAIN(int, 0x94) == 0)
        {
            if (PT_POISON_START > now)
            {
                PT_POISON_START = now;
            }
            if (now - PT_POISON_START > 10000)
            {
                poison_slot = -1;
                for (slot = 0; slot < 30; slot++)
                {
                    item = this->inventory_items[slot];
                    if (item != 0)
                    {
                        if (item->unique_id == PT_POISON_ITEM)
                        {
                            poison_slot = slot;
                        }
                        else if (item->type == 1)
                        {
                            bag = (EQ_Container*)item;
                            count = bag->slots;
                            for (index = 0; index < count; index++)
                            {
                                if (bag->contents[index] != 0 && bag->contents[index]->unique_id == PT_POISON_ITEM)
                                {
                                    poison_slot = index + (slot * 5 + 15) * 2;
                                    break;
                                }
                            }
                        }
                    }
                    if (poison_slot >= 0)
                    {
                        break;
                    }
                }
                if (poison_slot < 0)
                {
                    PT_DSP_CHAT("Your attempt to apply poison has been cancelled.", 0x10E);
                }
                else
                {
                    poison_request[0] = poison_slot;
                    PT_EQMAIN(int, 0xE5C) = 1;
                    PT_EQMAIN(int, 0x94) = 1;
                    PT_SEND(0x21BA, poison_request, 8);
                }
                PT_POISON_START = 0;
                PT_POISON_ITEM = 0;
            }
        }
        if (EQ_GLOBAL(u8, 0x006B648B) != 0 && now - EQ_GLOBAL(u32, 0x006B64A4) > 750)
        {
            if (EQ_GLOBAL(u8, 0x006B645A) == 0 && PT_EQMAIN(int, 0x94) == 0 && PT_EQMAIN(int, 0xE5C) == 0
                && PT_FIELD(int, EQ_GLOBAL(char*, 0x00604984), 0x10) == 0
                && PT_FIELD(int, EQ_GLOBAL(char*, 0x006018C0), 0x10) == 0
                && PT_FIELD(int, EQ_GLOBAL(char*, 0x006E4D84), 0x10) == 0)
            {
                if (EQ_GLOBAL(int, 0x006B649C) == 0)
                {
                    EQ_GLOBAL(int, 0x006B649C) = 1;
                    if (PT_MOUSE_UP(EQ_GLOBAL(void*, 0x006E4E5C), 0) == 0
                        && PT_MOUSE_UP(EQ_GLOBAL(void*, 0x006E4E58), 0) == 0
                        && PT_MOUSE_UP(g_eqmain_guess, 0) != 0)
                    {
                        PT_EQMAIN(s16, 0x5F188) = -1;
                    }
                }
            }
            else
            {
                EQ_GLOBAL(int, 0x006B649C) = 0;
            }
        }
        if (EQ_GLOBAL(u8, 0x006B648A) != 0 && now - EQ_GLOBAL(u32, 0x006B64A0) > 750)
        {
            if (EQ_GLOBAL(u8, 0x006B645A) == 0 && PT_EQMAIN(int, 0x94) == 0
                && PT_FIELD(int, EQ_GLOBAL(char*, 0x00604984), 0x10) == 0
                && PT_FIELD(int, EQ_GLOBAL(char*, 0x006018C0), 0x10) == 0
                && PT_FIELD(int, EQ_GLOBAL(char*, 0x006E4D84), 0x10) == 0)
            {
                if (EQ_GLOBAL(int, 0x006B6498) == 0)
                {
                    EQ_GLOBAL(int, 0x006B6498) = 1;
                    if (PT_MOUSE_UP(EQ_GLOBAL(void*, 0x006E4E5C), 1) == 0
                        && PT_MOUSE_UP(EQ_GLOBAL(void*, 0x006E4E58), 1) == 0
                        && PT_MOUSE_UP(g_eqmain_guess, 1) != 0)
                    {
                        PT_EQMAIN(s16, 0x5F188) = -1;
                    }
                }
            }
            else
            {
                EQ_GLOBAL(int, 0x006B6498) = 0;
            }
        }
        for (index = 0; index < 8; index++)
        {
            actor = this->player->actor_info;
            if (now > ((u32*)((char*)actor + 0xD4))[index] && now > PT_FIELD(u32, actor, 0x15C))
            {
                if (PT_GEM_READY(index) == 0)
                {
                    if (PT_BARD_CAST_BARD() == 0)
                    {
                        PT_GEM_STATE(index) = 5;
                        PT_UI_DIRTY = 1;
                    }
                    PT_GEM_READY(index) = 1;
                }
            }
            else if (PT_GEM_READY(index) != 0)
            {
                PT_GEM_STATE(index) = 4;
                PT_GEM_READY(index) = 0;
                PT_UI_DIRTY = 1;
            }
        }
    }

    if (this->player->type == 0)
    {
        u32* stun_end = (u32*)((char*)this->player->actor_info + 0xC0);
        if (*stun_end != 0 && now > *stun_end)
        {
            *stun_end = 0;
            this->unknown0x0076 = 0;
            PT_DSP_CHAT("You are unstunned.", 0x111);
            if (PT_ENVIRONMENT != 0)
            {
                PT_SET_ENV_STATE(-1);
            }
        }
        if (PT_CUR_HP(0) > 0 && this->unknown0x0076 == 3)
        {
            EQ_METHOD(0, void, 0x004B3DBA, (EQPlayer*), this->player)();
        }
        if (PT_CUR_HP(0) < 1 && (s8)this->unknown0x0076 < 3)
        {
            EQ_METHOD(0, void, 0x004B3E1C, (EQPlayer*), this->player)();
        }
    }

    if (g_eqmain_guess != 0 && this->player != 0)
    {
        if (this->player->stand_state != 100 && g_attacking_guess != 0)
        {
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(8, 1, 0);
        }
        weapon = (EQ_Equipment*)this->inventory_items[13];
        attack_delay = 0;
        if (weapon != 0 && weapon->delay < 1)
        {
            weapon = 0;
        }
        if (weapon == 0)
        {
            weapon = PT_FIST;
            weapon->item_type = 22;
            weapon->delay = 28;
        }
        if (weapon != 0 && PT_WEAPON_USES_DELAY(weapon))
        {
            attack_delay = PT_WEAPON_DELAY(this->player, PT_SKILL_BASE_DELAY(weapon), 0);
        }
        weapon = (EQ_Equipment*)this->inventory_items[11];
        if (weapon != 0 && weapon->delay < 1)
        {
            weapon = 0;
        }
        secondary_delay = 0;
        if (weapon != 0 && PT_WEAPON_USES_DELAY(weapon))
        {
            secondary_delay = PT_WEAPON_DELAY(this->player, PT_SKILL_BASE_DELAY(weapon), weapon->item_type == 5);
        }
        actor = this->player->actor_info;
        if (PT_FIELD(u8, actor, 0x89) == 11 && secondary_delay != 0)
        {
            attack_delay = secondary_delay;
        }
        if (now - PT_FIELD(u32, actor, 0x64) >= attack_delay)
        {
            if (PT_EQMAIN(int, 0xEA4) == 0)
            {
                PT_UI_DIRTY = 1;
            }
            PT_EQMAIN(int, 0xEA4) = 1;
            if (PT_EQMAIN(int, 0xEAC) == 0)
            {
                PT_UI_DIRTY = 1;
            }
            PT_EQMAIN(int, 0xEAC) = 1;
        }
        for (skill = 0; skill < 74; skill++)
        {
            if (skill == 32)
            {
                if (now >= PT_SKILL_TIMERS[32] && PT_SKILL_READY[32] == 0)
                {
                    PT_SKILL_READY[32] = 1;
                    PT_UI_DIRTY = 1;
                }
            }
            else
            {
                last_used = PT_SKILL_TIMERS[skill];
                if (last_used == 0xFFFFFFFF || last_used < now)
                {
                    elapsed = now - last_used;
                }
                else
                {
                    elapsed = 0;
                }
                if (elapsed > PT_FIELD(u32, g_skill_records_guess[skill], 0x20) && PT_SKILL_READY[skill] == 0)
                {
                    if ((skill != 42 || this->player->sneaking == 0) && (skill != 29 || PT_HIDDEN == 0))
                    {
                        PT_SKILL_READY[skill] = 1;
                    }
                    PT_UI_DIRTY = 1;
                }
            }
        }
        weapon = (EQ_Equipment*)this->inventory_items[14];
        if (weapon != 0 && weapon->delay < 1)
        {
            weapon = 0;
        }
        if (weapon == 0)
        {
            weapon = PT_FIST;
            weapon->item_type = 22;
            weapon->delay = 28;
        }
        if (weapon != 0 && PT_WEAPON_USES_DELAY(weapon))
        {
            attack_delay = PT_WEAPON_DELAY(this->player, PT_SKILL_BASE_DELAY(weapon), 0);
        }
        if (now - PT_FIELD(u32, this->player->actor_info, 0xCC) >= attack_delay)
        {
            if (PT_EQMAIN(int, 0xEA8) == 0)
            {
                PT_UI_DIRTY = 1;
            }
            PT_EQMAIN(int, 0xEA8) = 1;
        }
        weapon = (EQ_Equipment*)this->inventory_items[11];
        if (weapon != 0 && weapon->delay < 1)
        {
            weapon = 0;
        }
        if (weapon != 0 && PT_WEAPON_USES_DELAY(weapon))
        {
            PT_WEAPON_DELAY(this->player, PT_SKILL_BASE_DELAY(weapon), weapon->item_type == 5);
        }
        attack_delay = PT_WEAPON_DELAY(this->player, PT_FIELD(u32, this->player->actor_info, 0x164), 0);
        if (now - PT_FIELD(u32, this->player->actor_info, 0x160) >= attack_delay)
        {
            if (PT_EQMAIN(int, 0xEB0) == 0)
            {
                PT_UI_DIRTY = 1;
            }
            PT_EQMAIN(int, 0xEB0) = 1;
        }
    }

    if (PT_ABILITY == 53)
    {
        track = PT_TRACK_TARGET;
        if (track == 0)
        {
            PT_DSP_CHAT("You have lost or do not have a tracking target.", 0x10E);
            PT_ABILITY = -1;
        }
        else
        {
            if (PT_DISPLAY_TIME - EQ_GLOBAL(u32, 0x006B677C) > 2000)
            {
                EQ_GLOBAL(u32, 0x006B677C) = PT_DISPLAY_TIME;
                random_range_guess(1, 100);
                if (g_pc_guess->skills[53] >= 0)
                {
                    bearing = EQ_CDECL(float, 0x004A47F9, (float, float, float, float))(
                        this->player->y, this->player->x, track->y, track->x);
                    relative = this->player->heading;
                    if (bearing == relative)
                    {
                        relative = 0.0f;
                    }
                    else if (bearing > relative)
                    {
                        diff = bearing - relative;
                        if (diff > 256.0f)
                        {
                            diff = 512.0f - bearing + relative;
                        }
                        relative = diff;
                    }
                    else if (bearing < relative)
                    {
                        diff = relative - bearing;
                        if (diff > 256.0f)
                        {
                            diff = 512.0f - relative + bearing;
                        }
                        relative = diff;
                    }
                    angle = this->player->heading * 6.28f * 0.001953125f + 1.57f;
                    forward = (float)(EQ_CDECL(double, 0x00523484, (double))(angle) * 100.0);
                    side = EQ_CDECL(double, 0x005233D4, (double))(angle) * 100.0;
                    if ((track->x - this->player->x) * side + (track->y - this->player->y) * forward < 0.0f)
                    {
                        strcpy(direction, "right");
                    }
                    else
                    {
                        strcpy(direction, "left");
                    }
                    name = EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), pinstCEverQuest_guess)(track->name);
                    if (relative >= 0.0f && relative <= 32.0f)
                    {
                        sprintf(text, "%s is straight ahead.", name);
                    }
                    else if (relative > 32.0f && relative <= 96.0f)
                    {
                        sprintf(text, "%s is ahead and to the %s.", name, direction);
                    }
                    else if (relative > 96.0f && relative <= 160.0f)
                    {
                        sprintf(text, "%s is to the %s.", name, direction);
                    }
                    else if (relative > 160.0f && relative <= 224.0f)
                    {
                        sprintf(text, "%s is behind and to the %s.", name, direction);
                    }
                    else if (relative > 224.0f && relative <= 256.0f)
                    {
                        sprintf(text, "%s is behind you.", name);
                    }
                    PT_DSP_CHAT(text, 0x10E);
                }
            }
            if (PT_DISPLAY_TIME > EQ_GLOBAL(u32, 0x006B6778))
            {
                PT_ABILITY = -1;
                PT_TRACK_TARGET = 0;
                PT_DSP_CHAT("You have lost your tracking target.", 0x10E);
            }
        }
    }

    if (PT_HIDDEN != 0 || PT_ABILITY == 29)
    {
        player = this->player;
        if ((ACT_START_Y != (int)player->y || ACT_START_X != (int)player->x
                || ACT_START_Z != (int)player->z)
            && (this->class_ != 9 || player == 0 || player->sneaking == 0))
        {
            if (PT_ABILITY == 29)
            {
                PT_ABILITY = -1;
            }
            PT_HIDDEN = 0;
            EQ_METHOD(0, void, 0x004255CE, (EQ_Character*), this)();
            PT_DSP_CHAT("You have moved and are no longer hidden!!", 0x10E);
        }
    }

    if (PT_CAMP_TIME != 0)
    {
        if ((this->player == 0 && (s8)this->unknown0x0076 > 0)
            || (this->player != 0 && this->player->stand_state != 110))
        {
            PT_DSP_CHAT("Your attempt to camp has been interrupted.", 0x111);
            PT_CAMP_TIME = 0;
            EQ_GLOBAL(int, 0x006E4E8C) = 0;
        }
        if (PT_CAMP_TIME != 0)
        {
            if (now >= PT_CAMP_TIME)
            {
                EQ_GLOBAL(int, 0x006B7120) = 8;
                PT_SEND(0x2150, 0, 0);
                EQ_GLOBAL(u8, 0x006B7111) = 1;
                return;
            }
            previous_seconds = PT_CAMP_SECONDS;
            seconds = (PT_CAMP_TIME - now) / 1000;
            PT_CAMP_SECONDS = seconds;
            if (seconds != 0 && previous_seconds != seconds && seconds % 5 == 0)
            {
                sprintf(text, "It will take about %d more seconds to prepare your camp.", seconds);
                PT_DSP_CHAT(text, 0x111);
            }
        }
    }

    if (now - PT_FIELD(u32, this->player->actor_info, 0x58) > 1000 && this->player->type == 0)
    {
        affect = PT_TOTAL_SPELL_AFFECTS(0x63);
        if (affect >= 0)
        {
            affect = PT_TOTAL_SPELL_AFFECTS(3);
        }
        EQ_METHOD(1, void, 0x00420046, (EQ_Character*, short), this)(affect);
        EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_displayManager_guess)(0);
        hp = (short)PT_CUR_HP(0);
        if (hp != EQ_GLOBAL(s16, 0x005401D2))
        {
            EQ_GLOBAL(s16, 0x005401D2) = hp;
            hp_update.value = hp;
            hp_update.spawn_id = this->player->spawn_id;
            hp_update.type = 0x11;
            PT_SEND(0x20F5, &hp_update, 0xC);
        }
        hp = (short)PT_MAX_HP(0);
        if (hp != EQ_GLOBAL(s16, 0x005401D4))
        {
            EQ_GLOBAL(s16, 0x005401D4) = hp;
        }
        PT_FIELD(u32, this->player->actor_info, 0x58) = now;
        EQ_GLOBAL(u8, 0x006B710F) = 1;
        EQ_METHOD(0, void, 0x00425867, (EQ_Character*), this)();
        EQ_METHOD(0, void, 0x00425945, (EQ_Character*), this)();
        EQ_METHOD(0, void, 0x0041EC4F, (EQ_Character*), this)();
        if (PT_FIELD(u8, this->player->actor_info, 0x187) != 0)
        {
            EQ_METHOD(0, void, 0x0041EE15, (EQ_Character*), this)();
        }
        ride = this->player->actor_info->ride_target_guess;
        if (ride == 0)
        {
            PT_BOAT_COUNTER++;
            if (PT_BOAT_COUNTER >= 2)
            {
                PT_BOAT_COUNTER = 0;
                this->boat[0] = 0;
                boat_cleared = 1;
            }
        }
        else
        {
            strcpy(this->boat, ride->name);
            PT_BOAT_COUNTER = 0;
        }
        ride = this->player->actor_info->ride_target_guess;
        if (ride == 0)
        {
            if (boat_cleared != 0)
            {
                if (PT_BOAT_SENT != 0)
                {
                    PT_SEND(0x21BC, 0, 0);
                }
                PT_BOAT_SENT = 0;
            }
        }
        else
        {
            if (PT_BOAT_SENT == 0)
            {
                PT_SEND(0x21BB, ride->name, strlen(ride->name) + 4);
            }
            PT_BOAT_SENT = 1;
        }
    }

    player = this->player;
    if (player == 0)
    {
        return;
    }
    actor = player->actor_info;
    if (actor == 0)
    {
        return;
    }
    if (now - PT_FIELD(u32, actor, 0x5C) > 6000)
    {
        regen_buff = PT_GET_SPELL_AFFECT(0x65);
        if (this->player->type == 0)
        {
            if (this->player->gm != 0)
            {
                found = 0;
                group_entry = (int*)((char*)pinstCEverQuest_guess + 0x4C);
                for (count = 25; count != 0; count--)
                {
                    if (*group_entry == -1)
                    {
                        found = 1;
                    }
                    group_entry += 0x1D;
                }
                if (found != 0)
                {
                    PT_SEND(0x2011, 0, 0);
                }
            }
            EQ_GLOBAL(u8, 0x006E11E0)++;
            if (EQ_GLOBAL(u8, 0x006E11E0) > 100)
            {
                EQ_METHOD(2, void, 0x004CE8C0, (CEverQuest*, int, int), pinstCEverQuest_guess)(1, 1);
            }
            EQ_METHOD(0, void, 0x00480A94, (EQ_Character*), this)();
            hp_before = PT_CUR_HP(this->player->type);
            alive = hp_before >= 1;
            affect = PT_TOTAL_SPELL_AFFECTS(0x64);
            if (affect != 0)
            {
                PT_MODIFY_CUR_HP(affect);
            }
            if (regen_buff != 0)
            {
                PT_MODIFY_CUR_HP(1);
            }
            hp_mod = PT_TOTAL_SPELL_AFFECTS(0);
            if (hp_mod < 0)
            {
                caster = EQ_METHOD(2, u16, 0x00423C9C, (EQ_Character*, int, int), this)(0, -1);
                spawn = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, caster);
                if (spawn != 0 && spawn->type == 0)
                {
                    killer_id = caster;
                    if (this->player != 0 && this->player->type == 0)
                    {
                        if (this->player == spawn)
                        {
                            hp_mod = hp_mod * 75 / 100;
                        }
                        else
                        {
                            hp_mod = hp_mod * 70 / 100;
                        }
                        if (hp_mod > -1)
                        {
                            hp_mod = -1;
                        }
                    }
                }
                stun = PT_GET_SPELL_AFFECT(0x1F);
                if (stun != 0)
                {
                    EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), this)(stun);
                    EQ_METHOD(0, void, 0x004247B1, (EQ_Character*), this)();
                }
            }
            if (hp_mod != 0)
            {
                EQTRACE_guess("hp_mod = %d\n", hp_mod);
            }
            PT_MODIFY_CUR_HP(hp_mod);
            if (PT_CUR_HP(this->player->type) < PT_MAX_HP(this->player->type))
            {
                regen = (this->stand_state == 110) + 1;
                if (this->class_ == 7 && this->player != 0 && this->player->actor_info != 0 && this->level > 50
                    && PT_FIELD(u8, this->player->actor_info, 0x18A) != 0)
                {
                    regen = 2;
                }
                if (this->hunger == 0 || this->thirst == 0)
                {
                    regen = 0;
                }
                if (this->expansions_guess != 0 && this->level > 50)
                {
                    regen++;
                }
                if (this->expansions_guess != 0 && this->level > 55)
                {
                    regen++;
                }
                if (this->expansions_guess != 0 && this->level > 59)
                {
                    regen++;
                }
                if (this->innateSkills_guess[10] != 0xFF)
                {
                    if (this->level > 50)
                    {
                        regen++;
                    }
                    if (this->level > 55)
                    {
                        regen++;
                    }
                    regen *= 2;
                }
                if (this->unknown0x0076 == 3 && (PT_CUR_HP(this->player->type) < -5 || regen < 1))
                {
                    regen = -1;
                    PT_DSP_CHAT("You are bleeding to death!", 0xD);
                }
                if (this->stand_state == 110 && this->level > 19
                    && PT_DISPLAY_TIME - PT_FIELD(u32, this->player->actor_info, 0x70) > 60000)
                {
                    regen++;
                    if (this->level > 49)
                    {
                        regen++;
                    }
                    if (this->innateSkills_guess[10] != 0xFF)
                    {
                        regen++;
                        if (this->level > 49)
                        {
                            regen++;
                        }
                    }
                }
                if (this->player->type == 0 && regen + hp_mod == 0 && PT_CUR_HP(0) < 1)
                {
                    regen--;
                }
                PT_MODIFY_CUR_HP(regen);
                PT_CUR_HP(this->player->type);
                if (PT_CUR_HP(this->player->type) > 0 && this->unknown0x0076 == 3)
                {
                    EQ_METHOD(0, void, 0x004B3DBA, (EQPlayer*), this->player)();
                }
            }
        }
        max_mana = (short)EQ_METHOD(0, s32, 0x00419BCA, (EQ_Character*), this)();
        if (PT_CUR_MANA() < max_mana)
        {
            if (this->player->type == 0)
            {
                hunger = this->hunger;
                if (hunger != 0 && this->thirst != 0)
                {
                    mana_regen = 1;
                }
                else
                {
                    mana_regen = 0;
                }
                player = this->player;
                if (player != 0 && player->stand_state == 110 && hunger > 0 && this->thirst > 0)
                {
                    mana_regen = 2;
                }
                if (this->class_ != 8 && player->stand_state == 110 && hunger > 0 && this->thirst > 0
                    && ((g_eqmain_guess != 0 && PT_EQMAIN(int, 0xFE4) != 0)
                        || (this->level > 34 && PT_HAS_SKILL(0x1F) != 0)))
                {
                    meditate = PT_HAS_SKILL(0x1F);
                    EQ_METHOD(2, void, 0x004501A1, (CharacterProfile*, int, float), g_pc_guess)(0x1F, 1.0f);
                    mana_regen = 3;
                    if (meditate > 1)
                    {
                        mana_regen = 4;
                    }
                    if (meditate > 14)
                    {
                        mana_regen += meditate / 15;
                    }
                }
            }
            else
            {
                mana_regen = PT_HAS_SKILL(0x1F) < 100 ? 4 : 6;
            }
            if (regen_buff != 0 && this->class_ != 8)
            {
                mana_regen++;
            }
            proc = EQ_CDECL(int, 0x00425C48, (EQ_Character*, int))(this, 0xF);
            if (proc != 0 && this->class_ != 8)
            {
                mana_regen += proc;
            }
            this->mana += mana_regen;
            if (mana_regen < 0)
            {
                PT_CUR_MANA();
            }
        }
        EQ_METHOD(0, void, 0x0042520E, (EQ_Character*), this)();
        PT_FIELD(u32, this->player->actor_info, 0x5C) = EqGetTime();
    }

    if (since_save > 18000)
    {
        if (*(int*)this->unknown0x0AE4 > 0 && this->player != 0 && this->player->actor_info != 0)
        {
            if ((u32)time(0) - PT_FIELD(u32, this->player->actor_info, 0xC4) > 300)
            {
                PT_FIELD(u32, this->player->actor_info, 0xC4) = now;
                (*(int*)this->unknown0x0AE4)--;
            }
        }
        PT_FIELD(u32, this->player->actor_info, 0x74) = EqGetTime();
    }
    if (this->player == 0)
    {
        return;
    }
    if (now - EQ_GLOBAL(u32, 0x005918F8) > 6000 && g_offlineMode_guess == 0)
    {
        EQ_GLOBAL(u32, 0x005918F8) = now;
        if (this->intoxication > 0)
        {
            this->intoxication--;
            actor = this->player->actor_info;
            if (this->intoxication < 40)
            {
                PT_FIELD(s16, actor, 0x7A) = -1;
                PT_FIELD(f32, this->player->actor_info, 0x80) = 0.35f;
                PT_FIELD(s16, this->player->actor_info, 0x7E) = 1;
                if (PT_ENVIRONMENT != 0)
                {
                    PT_SET_ENV_STATE(-1);
                }
            }
            else
            {
                if (PT_FIELD(s16, actor, 0x7A) == -1)
                {
                    PT_FIELD(s16, actor, 0x7A) = 0;
                }
                drunk_scale = this->intoxication >> 2;
                PT_FIELD(f32, this->player->actor_info, 0x80) = drunk_scale * 0.01f + 0.35f;
                PT_FIELD(s16, this->player->actor_info, 0x7E) = (this->intoxication >> 3) + 1;
                if (PT_ENVIRONMENT != 0)
                {
                    PT_SET_ENV_STATE(5);
                }
            }
        }
        if (this->intoxication >= 70)
        {
            g_activeView_guess = 0;
            EQ_METHOD(1, void, 0x0040E4C6, (void*, short), g_display_guess)(
                (short)((int)EQ_GLOBAL(float, 0x006B6F2C) + this->intoxication - 70));
        }
        if (this->intoxication >= 100)
        {
            player = this->player;
            if (player->dy == 0.0f)
            {
                player->dy = 0.001f;
            }
            if (this->player->dx == 0.0f)
            {
                this->player->dx = 0.001f;
            }
        }
    }
    EQ_METHOD(1, void, 0x00425A6A, (EQ_Character*, int), this)(now);
    if (g_offlineMode_guess == 0 && DAT_006b6494 == 0 && PT_CUR_HP(this->player->type) < -10
        && PT_FIELD(int, this->player->actor_info, 0x68) == 0)
    {
        player = this->player;
        death.killer_id = 0;
        death.spawn_id = player->spawn_id;
        death.unknown0x08 = 0;
        damage = hp_before - PT_CUR_HP(player->type);
        if (damage < -0x7FFF)
        {
            damage = -0x7FFF;
        }
        if (damage > 0x7FFF)
        {
            damage = 0x7FFF;
        }
        death.spell_id = -1;
        death.unknown0x10 = 0.0f;
        death.damage = damage;
        death.skill = 0x17;
        death.unknown0x14 = 0.0f;
        death.unknown0x18 = 0.0f;
        if (alive != 0)
        {
            death.killer_id = killer_id;
        }
        EQ_METHOD(2, void, 0x00422A48, (EQ_Character*, PassageDeath_guess*, int), this)(&death, 0);
        if (this->player == 0)
        {
            return;
        }
    }

    player = this->player;
    if (player == 0)
    {
        return;
    }
    actor = player->actor_info;
    if (actor == 0)
    {
        return;
    }
    spell = PT_FIELD(s16, actor, 0x248);
    if (spell != -1 && now > PT_FIELD(u32, actor, 0xC8) && (g_offlineMode_guess != 0 || DAT_006b6494 != 0))
    {
        item_slot = PT_FIELD(int, actor, 0x2F0);
        own_item = 0;
        launched = 0;
        if (EQ_METHOD(1, char, 0x00423EDB, (EQ_Character*, short), this)(spell) != 0)
        {
            actor = this->player->actor_info;
            cast_item = PT_FIELD(EQ_Item*, actor, 0x2EC);
            if (cast_item != 0 && item_slot > 0 && item_slot < 30 && this->inventory_items[item_slot] != 0
                && this->inventory_items[item_slot]->type == 0 && this->inventory_items[item_slot] == cast_item)
            {
                own_item = 1;
            }
            if (cast_item != 0 && own_item == 0)
            {
                PT_STOP_SPELL_CAST(this, 1);
                PT_FIELD(u32, this->player->actor_info, 0xC8) = 0;
                PT_FIELD(EQ_Item*, this->player->actor_info, 0x2EC) = 0;
                PT_FIELD(s16, this->player->actor_info, 0x248) = -1;
                PT_FIELD(int, this->player->actor_info, 0x2F0) = -1;
                return;
            }
            if (cast_item != 0)
            {
                item_ptr = &this->inventory_items[item_slot];
            }
            else
            {
                item_ptr = 0;
            }
            if (EQ_METHOD(3, char, 0x00420016, (EQ_Character*, u8, short, EQ_Item**), this)(
                    PT_FIELD(u8, actor, 0x24A), PT_FIELD(s16, actor, 0x248), item_ptr) != 0)
            {
                launched = 1;
            }
            player = this->player;
            if (player == 0)
            {
                return;
            }
            actor = player->actor_info;
            if (actor == 0)
            {
                return;
            }
            PT_FIELD(EQ_Item*, actor, 0x2EC) = 0;
            PT_FIELD(int, this->player->actor_info, 0x2F0) = -1;
            if (this->class_ == 8)
            {
                spell = PT_FIELD(s16, this->player->actor_info, 0x248);
                if (spell > 0 && spell < 3000 && PT_SPELLS[spell] != 0 && PT_FIELD(s16, PT_SPELLS[spell], 0x1C8) != 0
                    && PT_FIELD(u8, PT_SPELLS[spell], 0x23C) < 61)
                {
                    PT_STOP_SPELL_CAST(this, 1);
                }
            }
        }
        PT_FIELD(u32, this->player->actor_info, 0xC8) = 0;
        if (EQ_METHOD(2, int, 0x0041F0C0, (EQ_Character*, char*, int), this)(
                PT_SPELLS[PT_FIELD(s16, this->player->actor_info, 0x248)], this->class_) != 0)
        {
            if (launched != 0)
            {
                actor = this->player->actor_info;
                PT_FIELD(u32, actor, 0xC8) = PT_DISPLAY_TIME + 6000;
                spell_record = PT_SPELLS[PT_FIELD(s16, actor, 0x248)];
                if (spell_record != 0 && PT_FIELD(u32, spell_record, 0x1B8) != 0)
                {
                    PT_STOP_SPELL_CAST(this, 0);
                }
            }
            else
            {
                PT_STOP_SPELL_CAST(this, 1);
            }
        }
        else
        {
            PT_STOP_SPELL_CAST(this, 0);
        }
    }

    if (PT_MACRO_STATE == -1)
    {
        return;
    }
    line = PT_MACRO_STATE & 7;
    macro = (PT_MACRO_STATE >> 3) & 0x1FFF;
    delay = PT_MACRO_STATE >> 16;
    if (now - EQ_GLOBAL(u32, 0x00591D80) <= 100)
    {
        return;
    }
    EQ_GLOBAL(u32, 0x00591D80) = now;
    if (delay > 0)
    {
        delay--;
    }
    else
    {
        page = (macro / 12 * 12 + macro % 12) * 0x136;
        text_offset = line * 0x3C + page;
        do
        {
            if (strlen(g_eqmain_guess + text_offset + 0x5F19F) != 0)
            {
                break;
            }
            line++;
            text_offset += 0x3C;
        } while (line <= 4);
        if (line < 5)
        {
            text_offset = page + line * 0x3C;
            if (_strnicmp(g_eqmain_guess + text_offset + 0x5F19F, "/pause", 4) == 0)
            {
                delay = atoi(g_eqmain_guess + text_offset + 0x5F1A6);
                if (delay > 600)
                {
                    delay = 600;
                }
                if (delay < 1)
                {
                    delay = 1;
                }
                rest = EQ_CDECL(char*, 0x00522A30, (const char*, const char*))(
                    g_eqmain_guess + text_offset + 0x5F19F, (const char*)0x005422BC);
                if (rest != 0 && strlen(rest) > 1)
                {
                    EQ_METHOD(2, void, 0x004DB9B5, (CEverQuest*, EQPlayer*, char*), pinstCEverQuest_guess)(
                        g_local_player_guess, rest + 1);
                }
            }
            else
            {
                delay = 1;
                EQ_METHOD(2, void, 0x004DB9B5, (CEverQuest*, EQPlayer*, char*), pinstCEverQuest_guess)(
                    g_local_player_guess, g_eqmain_guess + text_offset + 0x5F19F);
            }
            line++;
        }
    }
    if (line < 5)
    {
        if (delay < 0)
        {
            delay = 0;
        }
        if (macro >= 0 && line >= 0)
        {
            PT_MACRO_STATE = ((delay << 13) | macro) << 3 | line;
            return;
        }
    }
    PT_MACRO_STATE = -1;
}

#undef PT_FIELD
#undef PT_EQMAIN
#undef PT_GEM_STATE
#undef PT_GEM_READY
#undef PT_UI_DIRTY
#undef PT_SPELLS
#undef PT_STATIC_GUARD
#undef PT_SERVER_FLAG
#undef PT_LAST_SERVER_FLAG
#undef PT_NEXT_MINUTE
#undef PT_TARGET
#undef PT_LAST_TARGET
#undef PT_LAST_710C
#undef PT_LAST_710D
#undef PT_LAST_FE4
#undef PT_LAST_MINUTE
#undef PT_POISON_START
#undef PT_POISON_ITEM
#undef PT_CAMP_TIME
#undef PT_ENVIRONMENT
#undef PT_FIST
#undef PT_SKILL_TIMERS
#undef PT_SKILL_READY
#undef PT_HIDDEN
#undef PT_ABILITY
#undef PT_TRACK_TARGET
#undef PT_DISPLAY_TIME
#undef PT_CAMP_SECONDS
#undef PT_BOAT_COUNTER
#undef PT_BOAT_SENT
#undef PT_MACRO_STATE
#undef PT_SEND
#undef PT_DSP_CHAT
#undef PT_BARD_CAST_BARD
#undef PT_STOP_SPELL_CAST
#undef PT_MOUSE_UP
#undef PT_SET_ENV_STATE
#undef PT_CUR_HP
#undef PT_MAX_HP
#undef PT_MODIFY_CUR_HP
#undef PT_TOTAL_SPELL_AFFECTS
#undef PT_GET_SPELL_AFFECT
#undef PT_HAS_SKILL
#undef PT_CUR_MANA
#undef PT_WEAPON_DELAY
#undef PT_SKILL_BASE_DELAY
#undef PT_WEAPON_USES_DELAY

// FUNCTION: TRILOGY 0x0042798f
// Stops the character's current spell or song using the spell id stored on its actor.
void __fastcall EQ_Character_StopSpellCast2_guess(EQ_Character* profile, int unused_edx, int stopMode)
{
    if ((profile->player != 0) &&
        (profile->player->actor_info != 0))
    {
        EQ_THISCALL(void, 0x004279B4, (void*, int, int, short))(
            profile, unused_edx, stopMode,
            profile->player->actor_info->unknown0x248);
    }
    return;
}

// FUNCTION: TRILOGY 0x00427c70
// Finds an inventory item by equipment type, including items stored in bags.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range; TAKP: FindItemBySkillId)
// __thiscall. Looks for an equipment item of item type `type` in inventory slots 0..29 and inside the
// containers there. Sets *slot (and *bag_slot for an item in a container) and returns true; both stay -1
// when nothing is found.
bool EQ_Character::FindItemByType_guess(short type, short* slot, short* bag_slot)
{
    short i;
    short j;

    if (slot != 0)
        *slot = -1;
    if (bag_slot != 0)
        *bag_slot = -1;
    for (i = 0; i <= 29; i++)
    {
        EQ_Item* item = inventory_items[i];
        if (item == 0)
            continue;
        if (item->type == EQ_ITEM_EQUIPMENT)
        {
            if (((EQ_Equipment*)item)->item_type == type)
            {
                if (slot != 0)
                    *slot = i;
                return true;
            }
        }
        else if (item->type == EQ_ITEM_CONTAINER)
        {
            for (j = 0; j < ((EQ_Container*)item)->slots; j++)
            {
                EQ_Item* inner = ((EQ_Container*)item)->contents[j];
                if (inner != 0 && inner->type == EQ_ITEM_EQUIPMENT && ((EQ_Equipment*)inner)->item_type == type)
                {
                    if (slot != 0)
                        *slot = i;
                    if (bag_slot != 0)
                        *bag_slot = j;
                    return true;
                }
            }
        }
    }
    return false;
}

// FUNCTION: TRILOGY 0x00427d2a
// Finds an inventory item by item number, including items stored in bags.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range; TAKP: FindItemBySpellId)
// __thiscall. Like FindItemByType, by item number (EQ_Item.item_nr) and any item kind; a container is
// matched itself first, then searched.
bool EQ_Character::FindItemByNumber_guess(short item_nr, short* slot, short* bag_slot)
{
    short i;
    short j;

    if (slot != 0)
        *slot = -1;
    if (bag_slot != 0)
        *bag_slot = -1;
    for (i = 0; i <= 29; i++)
    {
        EQ_Item* item = inventory_items[i];
        if (item == 0)
            continue;
        if (item->item_nr == item_nr)
        {
            if (slot != 0)
                *slot = i;
            return true;
        }
        if (item->type == EQ_ITEM_CONTAINER)
        {
            for (j = 0; j < ((EQ_Container*)item)->slots; j++)
            {
                EQ_Item* inner = ((EQ_Container*)item)->contents[j];
                if (inner != 0 && inner->item_nr == item_nr)
                {
                    if (slot != 0)
                        *slot = i;
                    if (bag_slot != 0)
                        *bag_slot = j;
                    return true;
                }
            }
        }
    }
    return false;
}

// FUNCTION: TRILOGY 0x00427dd3
// Checks whether the character has enough of a required spell component in inventory.
// Count matching inventory items, including stacks inside containers. A matching container itself
// satisfies the request immediately; the final count is compared as a signed short.
bool EQ_Character::HasEnoughItemsForSpell_guess(short item_id, short required)
{
    int count = 0;
    bool result = false;
    int slot;

    for (slot = 0; slot < 30; slot++)
    {
        EQ_Item* item = inventory_items[slot];
        if (item != 0)
        {
            if (item->type == EQ_ITEM_CONTAINER)
            {
                EQ_Container* bag = (EQ_Container*)item;
                int contents_slot;
                if (item_id == item->item_nr)
                    return true;
                for (contents_slot = 0; contents_slot < bag->slots; contents_slot++)
                {
                    EQ_Item* content = bag->contents[contents_slot];
                    if (content != 0 && content->item_nr == item_id)
                    {
                        if (content->type == EQ_ITEM_EQUIPMENT &&
                            EQ_CDECL(int, 0x004A1D83, (EQ_Equipment*))((EQ_Equipment*)content))
                        {
                            EQ_Equipment* reread = (EQ_Equipment*)bag->contents[contents_slot];
                            count += 1 + ((int)reread->charges - 1) / reread->stackable;
                        }
                        else
                            count++;
                    }
                }
            }
            else if (item->item_nr == item_id)
            {
                if (item->type == EQ_ITEM_EQUIPMENT &&
                    EQ_CDECL(int, 0x004A1D83, (EQ_Equipment*))((EQ_Equipment*)item))
                {
                    EQ_Equipment* reread = (EQ_Equipment*)inventory_items[slot];
                    count += 1 + ((int)reread->charges - 1) / reread->stackable;
                }
                else
                    count++;
            }
        }
    }
    if ((s16)count >= required)
        result = true;
    return result;
}

// FUNCTION: TRILOGY 0x00427ee7
// Consumes a requested item or charge from inventory and updates the affected slot.
// @early-stop: TEST AL vs MOV CL, contents EAX vs EDI, PUSH/MOV order, tail dec sharing
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall (`ret 8`), written as a __fastcall free function because EQ_Character.h declares no
// ExpendItemCharge. itemTypeAndSlot's low word is the item type and its high word receives the found slot
// (FindItemByType/FindItemByNumber write it through a pointer). itemNumber's low word == -1 selects the
// search by type. Expends one charge of the found item (a stack is deleted when its last charge goes).
char __fastcall EQ_Character_ExpendItemCharge_guess(EQ_Character* self, int unused_edx, int itemTypeAndSlot, int itemNumber)
{
    char found;
    u8 chargeCount;
    EQ_Item* usedItem;
    EQ_Item* slotItem;
    u8* dat_e44;
    int temp[3];
    short bagSlot;
    short* slot = (short*)&itemTypeAndSlot + 1;

    if ((short)itemNumber != -1)
        found = EQ_METHOD(3, bool, 0x00427D2A, (EQ_Character*, int, short*, short*), self)(itemNumber, slot, &bagSlot);
    else
        found = EQ_METHOD(3, bool, 0x00427C70, (EQ_Character*, int, short*, short*), self)(itemTypeAndSlot, slot, &bagSlot);
    if (found == 0)
        return found;
    dat_e44 = HWM_EQMAIN;
    if (dat_e44 != 0)
        *(int*)(dat_e44 + 0xeb8) = 1;
    if (bagSlot != -1)
    {
        slotItem = self->inventory_items[*slot];
        if (slotItem == 0)
            return found;
        if (slotItem->type != EQ_ITEM_CONTAINER)
            return found;
        usedItem = ((EQ_Container*)slotItem)->contents[bagSlot];
        if (usedItem == 0)
            return found;
        if (EQ_CDECL(int, 0x004A1D83, (EQ_Equipment*))((EQ_Equipment*)usedItem) != 0)
        {
            temp[1] = -1;
            temp[2] = -1;
            temp[0] = bagSlot + (*slot + 3) * 10;
            send_message_guess(g_world_connection_guess, 0x2146, temp, 0xc, 1);
            chargeCount = ((EQ_Equipment*)usedItem)->charges;
            if (chargeCount <= 1)
            {
                EQ_CDECL(void, 0x005218BB, (void*))(((EQ_Container*)slotItem)->contents[bagSlot]);
                ((EQ_Container*)slotItem)->contents[bagSlot] = 0;
                return found;
            }
            ((EQ_Equipment*)usedItem)->charges = chargeCount - 1;
            return found;
        }
        EQ_CDECL(void, 0x005218BB, (void*))(((EQ_Container*)slotItem)->contents[bagSlot]);
        ((EQ_Container*)slotItem)->contents[bagSlot] = 0;
        temp[1] = -1;
        temp[2] = 0;
        temp[0] = bagSlot + (*slot + 3) * 10;
        send_message_guess(g_world_connection_guess, 0x212C, temp, 0xc, 1);
        return found;
    }
    slotItem = self->inventory_items[*slot];
    if (slotItem == 0)
        return found;
    if (EQ_CDECL(int, 0x004A1D83, (EQ_Equipment*))((EQ_Equipment*)slotItem) == 0)
    {
        usedItem = self->inventory_items[*slot];
        if (usedItem->type == EQ_ITEM_CONTAINER)
        {
            if (usedItem != 0)
            {
                EQ_METHOD(0, void, 0x0043405A, (void*), (void*)usedItem)();
                EQ_CDECL(void, 0x005218BB, (void*))(usedItem);
            }
        }
        else
            EQ_CDECL(void, 0x005218BB, (void*))(usedItem);
        self->inventory_items[*slot] = 0;
        *(s16*)((u8*)self + 0xa4 + *slot * 2) = -1;
        temp[1] = -1;
        temp[0] = *slot;
        temp[2] = 0;
        send_message_guess(g_world_connection_guess, 0x212C, temp, 0xc, 1);
        switch (*slot)
        {
        case 0:
            *(u8*)((u8*)g_display_guess + 0x3c) = 0;
            return found;
        case 2:
            EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, void*), g_display_guess)(*(void**)((u8*)self + 0xa0), 0, EQ_ARRAY(char, 0x0053D860));
            return found;
        case 13:
            EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, void*), g_display_guess)(*(void**)((u8*)self + 0xa0), 7, EQ_ARRAY(char, 0x0053D860));
            return found;
        case 14:
            EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, void*), g_display_guess)(*(void**)((u8*)self + 0xa0), 8, EQ_ARRAY(char, 0x0053D860));
            return found;
        default:
            return found;
        }
    }
    usedItem = self->inventory_items[*slot];
    temp[0] = *slot;
    temp[1] = -1;
    temp[2] = -1;
    send_message_guess(g_world_connection_guess, 0x2146, temp, 0xc, 1);
    chargeCount = ((EQ_Equipment*)usedItem)->charges;
    if (chargeCount <= 1)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(self->inventory_items[*slot]);
        self->inventory_items[*slot] = 0;
        switch (*slot)
        {
        case 0:
            *(u8*)((u8*)g_display_guess + 0x3c) = 0;
            return found;
        case 2:
            EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, void*), g_display_guess)(*(void**)((u8*)self + 0xa0), 0, EQ_ARRAY(char, 0x0053D860));
            return found;
        case 13:
            EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, void*), g_display_guess)(*(void**)((u8*)self + 0xa0), 7, EQ_ARRAY(char, 0x0053D860));
            return found;
        case 14:
            EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, void*), g_display_guess)(*(void**)((u8*)self + 0xa0), 8, EQ_ARRAY(char, 0x0053D860));
            return found;
        default:
            return found;
        }
    }
    ((EQ_Equipment*)usedItem)->charges = chargeCount - 1;
    return found;
}

// FUNCTION: TRILOGY 0x00428193
// Consumes one carried food or drink item when the hunger and thirst system requests it.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall (ECX = self), written as a __fastcall free function because EQ_Character.h declares no
// AutoEat; callee pops isFood. Sets the dword at 0xEB8 of the global at 0x006E4E44 to 1 when that global is
// set, then scans inventory slots 22..29 (inventory_items at 0xE0) and the containers there: food (item type
// 14) when isFood != 0, drink (type 15) when isFood == 0. The match loses one charge (its slot is cleared
// when it had one left), opcode 0x2156 is sent with (slot code, -1, -1, 1 = food or 2 = drink) and it
// returns 1; returns 0 when nothing matches. A slot code is the slot itself, or (slot + 3) * 10 + index for
// an item in a container.
// fake match: the found sites read self back from the saved copy packet[4] instead of a register, so VC6 keeps containerSlot in
// a register and the frame stays 0x18 as in the original (with self held in a register it spills containerSlot and the frame grows)
// @early-stop: self is precoloured to ECX (fastcall param), so the join reloads it and VC6 CSEs the +0x81 byte into AL
int __fastcall EQ_Character_AutoEat_guess(EQ_Character* self, int unused_edx, char isFood)
{
    u8* dat_e44;
    int temp[5];
    int inventorySlot;
    int outerItem;
    int innerItem;
    short containerSlot;
    int* itemPointer;
    int* obj;

    dat_e44 = (u8*)EQ_GLOBAL(void*, 0x006E4E44);
    temp[4] = (int)self;
    if (dat_e44 != 0)
        *(int*)(dat_e44 + 0xeb8) = 1;
    for (inventorySlot = 0x16; (short)inventorySlot <= 0x1d; inventorySlot++)
    {
        outerItem = *(int*)((u8*)self + 0xe0 + (short)inventorySlot * 4);
        if (outerItem != 0 && *(char*)(outerItem + 0x81) == 0)
        {
            if (isFood != 0)
            {
                if (*(char*)(outerItem + 0xc2) == 0xe)
                {
                    obj = (int*)EQ_GLOBAL(void*, 0x006018C0);
                    if (obj != 0 && obj[4] != 0 && (int*)obj[0x7c] != 0 &&
                        *(int*)obj[0x7c] == outerItem)
                        (*(void (__fastcall **)(int*))(*obj + 8))(obj);
                    temp[1] = -1;
                    temp[2] = -1;
                    temp[0] = (short)inventorySlot;
                    temp[3] = 1;
                    itemPointer = (int*)((u8*)temp[4] + 0xe0 + (short)inventorySlot * 4);
                    if (*(char*)(outerItem + 0xda) == 1)
                    {
                        EQ_CDECL(void, 0x005218BB, (void*))((void*)*itemPointer);
                        *itemPointer = 0;
                    }
                    else
                        *(char*)(outerItem + 0xda) = *(char*)(outerItem + 0xda) - 1;
                    send_message_guess(g_world_connection_guess, 0x2156, temp, 0x10, 1);
                    return 1;
                }
            }
            else if (*(char*)(outerItem + 0xc2) == 0xf)
            {
                obj = (int*)EQ_GLOBAL(void*, 0x006018C0);
                if (obj != 0 && obj[4] != 0 && (int*)obj[0x7c] != 0 &&
                    *(int*)obj[0x7c] == outerItem)
                    (*(void (__fastcall **)(int*))(*obj + 8))(obj);
                temp[1] = -1;
                temp[2] = -1;
                temp[0] = (short)inventorySlot;
                temp[3] = 2;
                itemPointer = (int*)((u8*)temp[4] + 0xe0 + (short)inventorySlot * 4);
                if (*(char*)(outerItem + 0xda) == 1)
                {
                    EQ_CDECL(void, 0x005218BB, (void*))((void*)*itemPointer);
                    *itemPointer = 0;
                }
                else
                    *(char*)(outerItem + 0xda) = *(char*)(outerItem + 0xda) - 1;
                send_message_guess(g_world_connection_guess, 0x2156, temp, 0x10, 1);
                return 1;
            }
        }
        else if (outerItem != 0 && *(u8*)(outerItem + 0x81) == 1)
        {
            containerSlot = 0;
            while (containerSlot < (short)*(u8*)(outerItem + 0xd5))
            {
                innerItem = *(int*)(outerItem + 0xac + containerSlot * 4);
                if (innerItem != 0 && *(char*)(innerItem + 0x81) == 0)
                {
                    if (isFood != 0)
                    {
                        if (*(char*)(innerItem + 0xc2) == 0xe)
                        {
                            obj = (int*)EQ_GLOBAL(void*, 0x006018C0);
                            if (obj != 0 && obj[4] != 0 && (int*)obj[0x7c] != 0 &&
                                *(int*)obj[0x7c] == *(int*)(outerItem + 0xac + containerSlot * 4))
                                (*(void (__fastcall **)(int*))(*obj + 8))(obj);
                            temp[1] = -1;
                            temp[2] = -1;
                            temp[0] = containerSlot + ((short)inventorySlot + 3) * 10;
                            temp[3] = 1;
                            itemPointer = (int*)(outerItem + 0xac + containerSlot * 4);
                            if (*(char*)(innerItem + 0xda) == 1)
                            {
                                EQ_CDECL(void, 0x005218BB, (void*))((void*)*itemPointer);
                                *itemPointer = 0;
                            }
                            else
                                *(char*)(innerItem + 0xda) = *(char*)(innerItem + 0xda) - 1;
                            send_message_guess(g_world_connection_guess, 0x2156, temp, 0x10, 1);
                            return 1;
                        }
                    }
                    else if (*(char*)(innerItem + 0xc2) == 0xf)
                    {
                        obj = (int*)EQ_GLOBAL(void*, 0x006018C0);
                        if (obj != 0 && obj[4] != 0 && (int*)obj[0x7c] != 0 &&
                            *(int*)obj[0x7c] == *(int*)(outerItem + 0xac + containerSlot * 4))
                            (*(void (__fastcall **)(int*))(*obj + 8))(obj);
                        temp[1] = -1;
                        temp[2] = -1;
                        temp[0] = containerSlot + ((short)inventorySlot + 3) * 10;
                        temp[3] = 2;
                        itemPointer = (int*)(outerItem + 0xac + containerSlot * 4);
                        if (*(char*)(innerItem + 0xda) == 1)
                        {
                            EQ_CDECL(void, 0x005218BB, (void*))((void*)*itemPointer);
                            *itemPointer = 0;
                        }
                        else
                            *(char*)(innerItem + 0xda) = *(char*)(innerItem + 0xda) - 1;
                        send_message_guess(g_world_connection_guess, 0x2156, temp, 0x10, 1);
                        return 1;
                    }
                }
                containerSlot = containerSlot + 1;
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00428405
// Tracks whether carried food and drink are missing or nearly depleted.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range; TAKP: CalculateHungerThirstStatus)
// __thiscall. Counts the charges of food (item type 14) and drink (15) in inventory slots 22..29 and
// their containers. Returns flags: 1 = no food, 2 = one food left, 4 = no drink, 8 = one drink left.
int EQ_Character::FoodDrinkStatus_guess(void)
{
    short food = 0;
    short drink = 0;
    int status = 0;
    int i;
    short j;

    for (i = 22; i < 30; i++)
    {
        EQ_Item* item = inventory_items[i];
        if (item == 0)
            continue;
        if (item->type == EQ_ITEM_EQUIPMENT)
        {
            if (((EQ_Equipment*)item)->item_type == 14)
                food += ((EQ_Equipment*)item)->charges;
            if (((EQ_Equipment*)item)->item_type == 15)
                drink += ((EQ_Equipment*)item)->charges;
        }
        else if (item->type == EQ_ITEM_CONTAINER)
        {
            for (j = 0; j < ((EQ_Container*)item)->slots; j++)
            {
                EQ_Item* inner = ((EQ_Container*)item)->contents[j];
                if (inner != 0 && inner->type == EQ_ITEM_EQUIPMENT)
                {
                    if (((EQ_Equipment*)inner)->item_type == 14)
                        food += ((EQ_Equipment*)inner)->charges;
                    if (((EQ_Equipment*)inner)->item_type == 15)
                        drink += ((EQ_Equipment*)inner)->charges;
                }
            }
        }
    }
    if (food == 0)
        status = 1;
    else if (food < 2)
        status = 2;
    if (drink == 0)
        status += 4;
    else if (drink < 2)
        status += 8;
    return status;
}

// FUNCTION: TRILOGY 0x004284fa
// ORIGINAL FILE: unknown (queue subsystem: EverQuest/EQMisc; static EQ_Character method from TAKP name)
// Preserves the binary's always-zero return and clamps *maximum to [1, damage bonus].
int EQ_Character::CalculateDamageBonus_guess(EQ_Character* character, int unused, int* maximum)
{
    u8 level;
    u8 cls;
    int bonus;

    if (character->player == 0)
        return 0;
    if (character->player->type != 0)
        return 0;

    level = character->level;
    if (level > 0x27)
    {
        cls = character->class_;
        if (cls == 2 || cls == 6 || cls == 10)
            bonus = 0x50;
        else if (cls <= 0x0A || cls > 0x0E)
            bonus = 200;
        else
            bonus = 0x28;
    }
    else if (level > 0x1D)
    {
        cls = character->class_;
        if (cls == 2 || cls == 6 || cls == 10)
            bonus = 0x1A;
        else if (cls <= 0x0A || cls > 0x0E)
            bonus = 0x3C;
        else
            bonus = 0x12;
    }
    else if (level > 0x13)
    {
        cls = character->class_;
        if (cls == 2 || cls == 6 || cls == 10)
            bonus = 0x14;
        else if (cls <= 0x0A || cls > 0x0E)
            bonus = 0x1E;
        else
            bonus = 0x0C;
    }
    else if (level > 9)
    {
        cls = character->class_;
        if (cls == 2 || cls == 6 || cls == 10)
            bonus = 0x0C;
        else if (cls <= 0x0A || cls > 0x0E)
            bonus = 0x0E;
        else
            bonus = 10;
    }
    else
    {
        cls = character->class_;
        if (cls == 2 || cls == 6 || cls == 10)
            bonus = 9;
        else if (cls > 0x0A && cls <= 0x0E)
            bonus = 6;
        else
            bonus = 10;
    }

    if (*maximum < 1)
        *maximum = 1;
    if (*maximum > bonus)
        *maximum = bonus;
    return 0;
}

struct CharCreateInput_guess
{
    virtual void Slot0() = 0;
    virtual void AddButtons() = 0;
    virtual void DelButtons() = 0;
    void HandleInput_guess(short eventType, short key);

    u8 unknown0x0004[8];
    int dirty;
    u8 unknown0x0010[0x650];
    EQ_Character* pc;
    int awaitingApproval;
    u8 unknown0x0668[0x132C];
    u8 baseStats[7];
    u8 maxStats[7];
    u8 featurePage;
    u8 unknown0x19A3[3];
    short pointsLeft;
    short startingPoints;
    u8 unknown0x19AA[0x1FE];
    short mode;
    short deity;
    u8 unknown0x1BAC[6];
    short hoverButton;
    u8 unknown0x1BB4[4];
    short statStep;
    short statKey;
    u8 unknown0x1BBC[4];
    u8 face;
    u8 unknown0x1BC1[0xF];
    short city;
    u16 race;
    u8 class_;
    u8 gender;
    u8 statIndex;
    u8 unknown0x1BD7[9];
    int nameSubmitted;
    int finished;
    u8 unknown0x1BE8[4];
    int hoverOnly;
    short message;
    u8 unknown0x1BF2[2];
};
C_ASSERT(sizeof(CharCreateInput_guess) == 0x1BF4);
C_ASSERT(offsetof(CharCreateInput_guess, pc) == 0x660);
C_ASSERT(offsetof(CharCreateInput_guess, baseStats) == 0x1994);
C_ASSERT(offsetof(CharCreateInput_guess, mode) == 0x1BA8);
C_ASSERT(offsetof(CharCreateInput_guess, race) == 0x1BD2);
C_ASSERT(offsetof(CharCreateInput_guess, message) == 0x1BF0);

#define CCI_DISPLAY             EQ_GLOBAL(char*, 0x006E4DE0)
#define CCI_SCREEN              ((CharCreateInput_guess*)EQ_GLOBAL(char*, 0x006E4E70))
#define CCI_SOUND_MANAGER       EQ_GLOBAL(void*, 0x006ECDF4)
#define CCI_MUSIC_DISABLED      EQ_GLOBAL(int, 0x006EFE1C)
#define CCI_IKSAR_ALLOWED       EQ_GLOBAL(int, 0x006E2058)
#define CCI_LUCLIN_ALLOWED      EQ_GLOBAL(int, 0x006E2060)
#define CCI_CAMERA_INDEX        EQ_GLOBAL(int, 0x00590154)
#define CCI_CAMERA(index)       (EQ_ARRAY(f32, 0x006B6F18) + (index) * 7)
#define CCI_RANDOM(lo, hi)      EQ_CDECL(int, 0x004A46F6, (int, int))((lo), (hi))
#define CCI_SET_VIEW_ANGLE(a)   EQ_METHOD(1, void, 0x0040E4C6, (void*, short), CCI_DISPLAY)(a)
#define CCI_PLAY_ANIMATION(player, anim, movement) \
    EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), CCI_DISPLAY)((player), (anim), (movement), 0, 1.0f, 1)
#define CCI_CHECK_CLASS(cls) \
    EQ_METHOD(2, u8, 0x0041C663, (EQ_Character*, short, short), this->pc)((u8)this->race, (cls))
#define CCI_WORSHIP(va, row) \
    EQ_METHOD(2, bool, va, (EQ_Character*, u8, u8), this->pc)(this->class_, (u8)(row))
#define CCI_CITY_OK(c) \
    EQ_METHOD(4, bool, 0x0041C957, (EQ_Character*, short, short, short, short), this->pc)((short)(c), this->race, this->deity, this->class_)
#define CCI_BASE_ATTR(stat) \
    EQ_METHOD(3, u8, 0x0041C5AA, (EQ_Character*, short, short, int), this->pc)(this->class_, this->race, (stat))
#define CCI_CLASS_MIN(stat) \
    EQ_METHOD(2, u8, 0x0041CB5F, (EQ_Character*, short, u8), this->pc)(this->race, (stat))
#define CCI_MUSIC(selection) \
    if (CCI_MUSIC_DISABLED == 0 && CCI_SOUND_MANAGER != 0) \
        EQ_METHOD(1, void, 0x004E4EE6, (void*, int), CCI_SOUND_MANAGER)(selection)
#define CCI_CAN_WORSHIP(row) \
    raceId = this->race + 1; \
    if (raceId == 1) \
        allowed = CCI_WORSHIP(0x0041C6BB, row); \
    else if (raceId == 5) \
        allowed = CCI_WORSHIP(0x0041C7AB, row); \
    else if (raceId == 6) \
        allowed = CCI_WORSHIP(0x0041C7D7, row); \
    else if (raceId == 7) \
        allowed = CCI_WORSHIP(0x0041C803, row); \
    else if (raceId == 8) \
        allowed = CCI_WORSHIP(0x0041C83E, row); \
    else if (raceId == 10) \
        allowed = CCI_WORSHIP(0x0041C896, row); \
    else if (raceId == 9) \
        allowed = CCI_WORSHIP(0x0041C86A, row); \
    else if (raceId == 3) \
        allowed = CCI_WORSHIP(0x0041C739, row); \
    else if (raceId == 12) \
        allowed = CCI_WORSHIP(0x0041C8F4, row); \
    else if (raceId == 11) \
        allowed = CCI_WORSHIP(0x0041C8C2, row); \
    else if (raceId == 4) \
        allowed = CCI_WORSHIP(0x0041C779, row); \
    else if (raceId == 2) \
        allowed = CCI_WORSHIP(0x0041C6F7, row); \
    else if (raceId == 0x80) \
        allowed = CCI_WORSHIP(0x0041C920, row); \
    else if (raceId == 0x82) \
        allowed = CCI_WORSHIP(0x0041C94C, row); \
    else \
        allowed = false
#define CCI_FIND_DEITY() \
    for (deity = 0; deity < 17; deity++) \
    { \
        CCI_CAN_WORSHIP(deity); \
        if (allowed) \
        { \
            this->deity = (short)deity; \
            break; \
        } \
    }
#define CCI_PLACE_PLAYER() \
    evil = EQ_METHOD(3, bool, 0x00431509, (void*, int, int, int), g_char_select_guess)(this->race + 1, this->class_ + 1, this->deity); \
    g_local_player_guess->y = 0.0f; \
    if (!evil) \
    { \
        g_local_player_guess->x = 0.0f; \
        height = 7.0f; \
    } \
    else \
    { \
        g_local_player_guess->x = -315.0f; \
        height = 6.0f; \
    } \
    g_local_player_guess->z = height; \
    EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*), CCI_DISPLAY)(g_local_player_guess)
#define CCI_PICK_CITY() \
    for (city = 0; city < 14; city++) \
    { \
        if (CCI_CITY_OK(city)) \
        { \
            this->city = (short)city; \
            break; \
        } \
    } \
    if (this->race + 1 == 1 && CCI_CITY_OK(1) && CCI_CITY_OK(4)) \
    { \
        this->city = 4; \
        this->city = (CCI_RANDOM(1, 100) > 50) ? 4 : 1; \
    } \
    if (this->race + 1 == 7 && CCI_CITY_OK(1) && CCI_CITY_OK(4) && CCI_CITY_OK(9) && CCI_CITY_OK(10)) \
    { \
        this->city = 10; \
        roll = CCI_RANDOM(1, 100); \
        if (roll < 26) \
            this->city = 1; \
        else if (roll < 51) \
            this->city = 4; \
        else \
            this->city = (roll > 75) ? 10 : 9; \
    }
#define CCI_ROLL_STATS() \
    this->pc->STR = CCI_BASE_ATTR(0) + CCI_CLASS_MIN(0); \
    this->pc->DEX = CCI_BASE_ATTR(1) + CCI_CLASS_MIN(1); \
    this->pc->AGI = CCI_BASE_ATTR(2) + CCI_CLASS_MIN(2); \
    this->pc->STA = CCI_BASE_ATTR(3) + CCI_CLASS_MIN(3); \
    this->pc->INT = CCI_BASE_ATTR(4) + CCI_CLASS_MIN(4); \
    this->pc->WIS = CCI_BASE_ATTR(5) + CCI_CLASS_MIN(5); \
    this->pc->CHA = CCI_BASE_ATTR(6) + CCI_CLASS_MIN(6); \
    character = this->pc; \
    this->baseStats[0] = character->STR; \
    this->baseStats[1] = character->DEX; \
    this->baseStats[2] = character->AGI; \
    this->baseStats[3] = character->STA; \
    this->baseStats[4] = character->INT; \
    this->baseStats[5] = character->WIS; \
    this->baseStats[6] = character->CHA; \
    this->maxStats[0] = character->STR + 25; \
    this->maxStats[1] = character->DEX + 25; \
    this->maxStats[2] = character->AGI + 25; \
    this->maxStats[3] = character->STA + 25; \
    this->maxStats[4] = character->INT + 25; \
    this->maxStats[5] = character->WIS + 25; \
    this->maxStats[6] = character->CHA + 25; \
    if (this->maxStats[0] > 150) \
        this->maxStats[0] = 150; \
    if (this->maxStats[1] > 150) \
        this->maxStats[1] = 150; \
    if (this->maxStats[2] > 150) \
        this->maxStats[2] = 150; \
    if (this->maxStats[3] > 150) \
        this->maxStats[3] = 150; \
    if (this->maxStats[4] > 150) \
        this->maxStats[4] = 150; \
    if (this->maxStats[5] > 150) \
        this->maxStats[5] = 150; \
    if (this->maxStats[6] > 150) \
        this->maxStats[6] = 150; \
    for (stat = 0; stat < 7; stat++) \
        this->pointsLeft -= CCI_BASE_ATTR(stat)
#define CCI_STEP_STAT(field, index) \
    if (this->statStep > 0) \
    { \
        if (this->pc->field >= this->maxStats[index]) \
            return; \
        this->pc->field++; \
        this->pointsLeft--; \
    } \
    else if (this->statStep < 0) \
    { \
        if (this->pc->field <= this->baseStats[index]) \
            return; \
        this->pc->field--; \
        this->pointsLeft++; \
    }

// FUNCTION: TRILOGY 0x0042a26f
// Character creation clicks: race, class, gender, face, stat points, then deity, start city and final confirm.
// Input handler of the character creation screen (0x006E4E70, vtable 0x005353A8), __thiscall, pops both shorts:
// mode 0 race/class/stats/name/looks, 1 deity, 2 starting city, 3 welcome text (buttons go to hover while hoverOnly).
void CharCreateInput_guess::HandleInput_guess(short eventType, short key)
{
    char welcome[0xA00];
    EQ_Character* character;
    EQPlayer* player;
    char* actor;
    f32* camera;
    f32 height;
    int deity;
    int city;
    int raceId;
    int roll;
    int stat;
    int tries;
    int faceCount;
    u8 classId;
    u8 statIndex;
    bool allowed;
    bool evil;

    if (this->awaitingApproval != 0)
        return;
    *(short*)(g_char_select_guess + 0xA4) = 0;
    if (this->mode == 0)
    {
        if (this->hoverOnly != 0)
        {
            if (key >= 0 && key <= 13)
                return;
            if (key >= 0x1F && key <= 0x2A)
                return;
            if (key == 0x2E)
                return;
            if (key == 0x30)
                return;
            if (eventType == 0 || eventType == 2)
                this->hoverButton = key;
            return;
        }
        if ((key >= 0x1F && key <= 0x2A)
            || (key == 0x2E && (*(int*)(CCI_DISPLAY + 0x88) != 0 || *(int*)(CCI_DISPLAY + 0x94) != 0))
            || (key == 0x30 && *(int*)(CCI_DISPLAY + 0x94) != 0))
        {
            this->pointsLeft = this->startingPoints;
            switch (key)
            {
            case 0x1F:
                this->race = 2;
                break;
            case 0x20:
                this->race = 6;
                break;
            case 0x21:
                this->race = 8;
                break;
            case 0x22:
                this->race = 3;
                break;
            case 0x23:
                this->race = 12;
                break;
            case 0x24:
                this->race = 7;
                break;
            case 0x25:
                this->race = 11;
                break;
            case 0x26:
                this->race = 5;
                break;
            case 0x27:
                this->race = 1;
                break;
            case 0x28:
                this->race = 10;
                break;
            case 0x29:
                this->race = 9;
                break;
            case 0x2A:
                this->race = 4;
                break;
            case 0x2E:
                if (CCI_IKSAR_ALLOWED == 0)
                    CCI_SCREEN->message = 8;
                this->race = 0x80;
                break;
            case 0x30:
                if (CCI_LUCLIN_ALLOWED == 0)
                    CCI_SCREEN->message = 9;
                this->race = 0x82;
                break;
            }
            if (CCI_SCREEN->message >= 8 && CCI_SCREEN->message <= 9)
                CCI_SCREEN->message = 0;
            g_local_player_guess->gender = this->gender;
            g_local_player_guess->race = this->race;
            this->race--;
            switch (this->race)
            {
            case 1:
                CCI_MUSIC(4);
                break;
            case 5:
                CCI_MUSIC(5);
                break;
            case 2:
                CCI_MUSIC(10);
                break;
            case 11:
                CCI_MUSIC(13);
                break;
            case 10:
                CCI_MUSIC(11);
                break;
            case 4:
                CCI_MUSIC(7);
                break;
            case 9:
                CCI_MUSIC(2);
                break;
            case 3:
                CCI_MUSIC(8);
                break;
            case 6:
                CCI_MUSIC(6);
                break;
            case 7:
                CCI_MUSIC(12);
                break;
            case 8:
                CCI_MUSIC(3);
                break;
            default:
                CCI_MUSIC(9);
                break;
            }
            if (!CCI_CHECK_CLASS(this->class_))
            {
                this->class_ = 0;
                do
                {
                    if (CCI_CHECK_CLASS(this->class_))
                        break;
                    this->class_++;
                } while (this->class_ <= 14);
            }
            this->face = (u8)CCI_RANDOM(0, 7);
            EQ_METHOD(4, void, 0x00430381, (void*, u8, short, u8, u8), g_char_select_guess)(this->gender, this->race, this->face, this->class_);
            CCI_FIND_DEITY();
            CCI_PICK_CITY();
            CCI_PLACE_PLAYER();
            this->dirty = 1;
            CCI_ROLL_STATS();
            return;
        }
        if (key != 0x30)
        {
            if ((key >= 0 && key <= 13) || key == 0x31)
            {
                switch (key)
                {
                case 0:
                    classId = 8;
                    break;
                case 1:
                    classId = 2;
                    break;
                case 2:
                    classId = 6;
                    break;
                case 3:
                    classId = 14;
                    break;
                case 4:
                    classId = 13;
                    break;
                case 5:
                    classId = 7;
                    break;
                case 6:
                    classId = 11;
                    break;
                case 7:
                    classId = 3;
                    break;
                case 8:
                    classId = 4;
                    break;
                case 9:
                    classId = 9;
                    break;
                case 10:
                    classId = 5;
                    break;
                case 11:
                    classId = 10;
                    break;
                case 12:
                    classId = 1;
                    break;
                case 13:
                    classId = 12;
                    break;
                case 0x31:
                    classId = 15;
                    break;
                }
                if (!CCI_CHECK_CLASS((short)(classId - 1)))
                    return;
                switch (key)
                {
                case 0:
                    this->class_ = 8;
                    break;
                case 1:
                    this->class_ = 2;
                    break;
                case 2:
                    this->class_ = 6;
                    break;
                case 3:
                    this->class_ = 14;
                    break;
                case 4:
                    this->class_ = 13;
                    break;
                case 5:
                    this->class_ = 7;
                    break;
                case 6:
                    this->class_ = 11;
                    break;
                case 7:
                    this->class_ = 3;
                    break;
                case 8:
                    this->class_ = 4;
                    break;
                case 9:
                    this->class_ = 9;
                    break;
                case 10:
                    this->class_ = 5;
                    break;
                case 11:
                    this->class_ = 10;
                    break;
                case 12:
                    this->class_ = 1;
                    break;
                case 13:
                    this->class_ = 12;
                    break;
                case 0x31:
                    if (CCI_LUCLIN_ALLOWED == 0)
                        CCI_SCREEN->message = 10;
                    this->class_ = 15;
                    break;
                }
                if (CCI_SCREEN->message == 10 && this->class_ != 15)
                    CCI_SCREEN->message = 0;
                this->class_--;
                CCI_FIND_DEITY();
                CCI_PLACE_PLAYER();
                this->pointsLeft = this->startingPoints;
                this->dirty = 1;
                CCI_ROLL_STATS();
                return;
            }
            if (key >= 0x0E && key <= 0x1B)
            {
                this->statKey = key;
                this->dirty = 1;
                statIndex = (u8)((key - 14) / 2);
                this->statIndex = statIndex;
                if (key % 2 != 0)
                {
                    this->statStep = 1;
                    if (this->pointsLeft == 0)
                        return;
                }
                else
                    this->statStep = -1;
                switch (statIndex)
                {
                case 0:
                    CCI_STEP_STAT(STR, 0)
                    break;
                case 1:
                    CCI_STEP_STAT(STA, 3)
                    break;
                case 2:
                    CCI_STEP_STAT(AGI, 2)
                    break;
                case 3:
                    CCI_STEP_STAT(DEX, 1)
                    break;
                case 4:
                    CCI_STEP_STAT(WIS, 5)
                    break;
                case 5:
                    CCI_STEP_STAT(INT, 4)
                    break;
                case 6:
                    CCI_STEP_STAT(CHA, 6)
                    break;
                }
                return;
            }
        }
        switch (key)
        {
        case 0x1C:
            if (EQ_METHOD(1, int, 0x004D1112, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))((char*)this->pc) != 0)
            {
                if (this->pointsLeft > 0)
                    this->message = 2;
                else
                {
                    this->message = 0;
                    this->DelButtons();
                    this->mode = 1;
                    this->dirty = 1;
                    this->AddButtons();
                }
            }
            else
                this->message = 1;
            return;
        case 0x1D:
            this->message = 0;
            this->finished = 1;
            this->dirty = 1;
            CCI_SET_VIEW_ANGLE(0x5A);
            return;
        case 0x1E:
            SOC_CPY((char*)CCI_SCREEN->pc,
                EQ_CDECL(char*, 0x004AF3D7, (short, char))((short)(this->race + 1), this->gender));
            return;
        case 0x2B:
            raceId = this->race + 1;
            this->gender = 0;
            this->dirty = 1;
            if (raceId == 2)
                faceCount = 7;
            else if (raceId == 1 || raceId == 4)
                faceCount = 6;
            else if (raceId == 6)
                faceCount = 5;
            else
                faceCount = 3;
            this->face = (u8)CCI_RANDOM(0, faceCount);
            this->face = (u8)CCI_RANDOM(0, 7);
            EQ_METHOD(4, void, 0x00430381, (void*, u8, short, u8, u8), g_char_select_guess)(this->gender, this->race, this->face, this->class_);
            CCI_PLACE_PLAYER();
            return;
        case 0x2C:
            this->gender = 1;
            this->dirty = 1;
            this->face = (u8)CCI_RANDOM(0, 3);
            this->face = (u8)CCI_RANDOM(0, 7);
            EQ_METHOD(4, void, 0x00430381, (void*, u8, short, u8, u8), g_char_select_guess)(this->gender, this->race, this->face, this->class_);
            CCI_PLACE_PLAYER();
            return;
        case 0x2D:
            this->featurePage = this->featurePage == 0;
            if (this->featurePage != 0)
            {
                CCI_SET_VIEW_ANGLE(0x46);
                player = g_local_player_guess;
                if (*(int*)(CCI_DISPLAY + 0x94) != 0
                    && (actor = player->actor_info->anim_set_guess) != 0
                    && actor[0x39C] == 1)
                {
                    CCI_PLAY_ANIMATION(player, 0x47, 0);
                    camera = CCI_CAMERA(CCI_CAMERA_INDEX);
                    camera[0] = 1.8f;
                    camera[2] = 0.0f;
                    if (g_local_player_guess->race != 0x82)
                        return;
                    camera[0] = 2.4f;
                    return;
                }
                CCI_PLAY_ANIMATION(player, 0x20, 1);
                player = g_local_player_guess;
                camera = CCI_CAMERA(CCI_CAMERA_INDEX);
                if (player->gender != 0)
                {
                    if (player->race == 10 || player->race == 9)
                        camera[2] = -1.0f;
                    else
                        camera[2] = -0.5f;
                }
                else if (player->race == 10 || player->race == 9)
                    camera[2] = -1.0f;
                else if (player->race == 8 || player->race == 11 || player->race == 12)
                    camera[2] = -0.5f;
                else
                    camera[2] = 0.0f;
                if (player->gender == 0 || player->race == 3 || player->race == 5)
                    camera[0] = 1.5f;
                else
                    camera[0] = 1.3f;
                if (player->race == 8 || player->race == 12 || player->race == 11)
                {
                    if (player->gender == 0)
                        camera[0] = 2.5f;
                    else
                        camera[0] = 1.5f;
                }
                if (player->race == 10 || player->race == 9)
                {
                    if (player->gender == 0)
                        camera[0] = 4.0f;
                    else
                        camera[0] = 3.0f;
                    if (player->race == 10 && player->gender != 0)
                        camera[0] = 4.0f;
                }
                else if (player->race == 0x80)
                {
                    camera[0] = 2.5f;
                    camera[2] = 0.5f;
                }
                if (player->race != 0x82)
                    return;
                camera[0] = 2.4f;
                return;
            }
            CCI_SET_VIEW_ANGLE(0x5A);
            player = g_local_player_guess;
            if (player->race == 10 || player->race == 9 || player->race == 0x80)
                CCI_CAMERA(CCI_CAMERA_INDEX)[0] = 7.0f;
            else
                CCI_CAMERA(CCI_CAMERA_INDEX)[0] = 6.0f;
            CCI_CAMERA(CCI_CAMERA_INDEX)[2] = -(*(f32*)&player->unk_0x78 * 0.33f);
            return;
        default:
            if (key >= 0x46 && key <= 0x47)
                EQ_METHOD(2, void, 0x0042A109, (void*, int, int), this)(key - 0x46, 0);
            else if (key >= 0x58 && key <= 0x5B)
            {
                if (this->featurePage == 0)
                    return;
                if (*(int*)(CCI_DISPLAY + 0x94) == 0)
                    return;
                actor = g_local_player_guess->actor_info->anim_set_guess;
                if (actor == 0)
                    return;
                if (actor[0x39C] != 1)
                    return;
                this->featurePage = (u8)key - 0x57;
            }
            else if (key >= 0x5C && key <= 0x5D)
                EQ_METHOD(2, void, 0x0042A109, (void*, int, int), this)(key - 0x5C, 1);
            return;
        }
    }
    else if (this->mode == 1)
    {
        if (this->hoverOnly != 0)
        {
            if (eventType == 0 || eventType == 2)
                this->hoverButton = key;
            return;
        }
        if (key >= 0x32 && key <= 0x42)
        {
            CCI_CAN_WORSHIP(key - 0x32);
            if (allowed)
                this->deity = key - 0x32;
        }
        CCI_PLACE_PLAYER();
        switch (key)
        {
        case 0x1C:
            this->DelButtons();
            this->mode = 2;
            this->dirty = 1;
            CCI_PICK_CITY();
            this->AddButtons();
            return;
        case 0x1D:
            this->message = 0;
            this->DelButtons();
            this->mode = 0;
            this->dirty = 1;
            this->AddButtons();
            return;
        }
    }
    else if (this->mode == 2)
    {
        if (this->hoverOnly != 0)
        {
            if (eventType == 0 || eventType == 2)
                this->hoverButton = key;
            return;
        }
        if (key == 0x1C)
        {
            if (CCI_CITY_OK(this->city))
            {
                this->message = 0;
                this->DelButtons();
                this->mode = 3;
                SOC_CPY(welcome,
                    EQ_CDECL(char*, 0x00431625, (char*, int, int, int, int))(welcome, this->race, this->class_, this->deity, this->city));
                EQ_METHOD(1, void, 0x0042C4D3, (void*, char*), this)(welcome);
            }
            else
            {
                this->message = 3;
                this->DelButtons();
                this->mode = 2;
            }
            this->AddButtons();
        }
        else if (key == 0x1D)
        {
            this->DelButtons();
            this->mode = 1;
            this->dirty = 1;
            this->message = 0;
            this->nameSubmitted = 0;
            this->AddButtons();
        }
        else if (key == 0x2F)
            this->city = 12;
        else if (key == 0x48)
        {
            tries = 0;
            do
            {
                tries++;
                this->city++;
                if (this->city >= 14)
                    this->city = 0;
            } while (!CCI_CITY_OK(this->city) && tries < 15);
        }
        else if (key == 0x49)
        {
            tries = 0;
            do
            {
                tries++;
                this->city--;
                if (this->city < 0)
                    this->city = 13;
            } while (!CCI_CITY_OK(this->city) && tries < 15);
        }
        else if (key > 0x49 && key <= 0x55)
            this->city = key - 0x4A;
        if (this->race + 1 == 0x82)
            this->city = 13;
    }
    else if (this->mode == 3)
    {
        if (this->hoverOnly != 0)
        {
            if (eventType == 0 || eventType == 2)
                this->hoverButton = key;
            return;
        }
        switch (key)
        {
        case 0x56:
            if (this->message < 5)
            {
                this->DelButtons();
                this->mode = 2;
                this->dirty = 1;
                this->AddButtons();
            }
            break;
        case 0x57:
            if (this->message < 5)
            {
                CCI_SET_VIEW_ANGLE(0x5A);
                this->nameSubmitted = 1;
                this->dirty = 1;
            }
            break;
        }
    }
}

#undef CCI_STEP_STAT
#undef CCI_ROLL_STATS
#undef CCI_PICK_CITY
#undef CCI_PLACE_PLAYER
#undef CCI_FIND_DEITY
#undef CCI_CAN_WORSHIP
#undef CCI_MUSIC
#undef CCI_CLASS_MIN
#undef CCI_BASE_ATTR
#undef CCI_CITY_OK
#undef CCI_WORSHIP
#undef CCI_CHECK_CLASS
#undef CCI_PLAY_ANIMATION
#undef CCI_SET_VIEW_ANGLE
#undef CCI_RANDOM
#undef CCI_CAMERA
#undef CCI_CAMERA_INDEX
#undef CCI_LUCLIN_ALLOWED
#undef CCI_IKSAR_ALLOWED
#undef CCI_MUSIC_DISABLED
#undef CCI_SOUND_MANAGER
#undef CCI_SCREEN
#undef CCI_DISPLAY

// FUNCTION: TRILOGY 0x00436285
// Recognizes bard instrument items for item display.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::IsMage, medium)
// __thiscall. The only caller (EQ_ItemData::draw) passes an item as `this`, not a character: true for item
// types 23..26 (EQ_Equipment.item_type at 0xC2; next to EQ_Equipment::IsScroll, 20..22).
int EQ_Character::IsInstrument_guess(void)
{
    EQ_Equipment* item = (EQ_Equipment*)this;

    if (item->item_type >= 23 && item->item_type <= 26)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004e1f74
// Updates the game's environment state when the current state permits the change.
// ORIGINAL FILE: unknown (queue subsystem EverQuest/EQ_Character, from the call graph)
// __thiscall on the object at 0x006E4DDC. Sets the state at +0xC, unless the current one is 4 or more
// (only -1 overrides it then).
void EQEnvironment_guess::SetState_guess(int state)
{
    if (this->state < 4 || state == -1)
        this->state = state;
}

// FUNCTION: TRILOGY 0x004196d4
// Initializes a fresh character with profile data and its default stand state.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::EQ_Character, high)
// Clears the first 0xB18 bytes of the profile, fills the shared 0.1 .. 2.0 table once, and sets the few
// non-zero defaults (stand state 100).
EQ_Character::EQ_Character()
{
    int i;

    memset(this, 0, 0xB18);
    if (g_tenths_table_ready_guess == 0)
    {
        for (i = 0; i < 20; i++)
            g_tenths_table_guess[i] = (i + 1.0) * 0.1;
        g_tenths_table_ready_guess = 1;
    }
    unknown0x0A47 = -1;
    name[0] = '\0';
    player = 0;
    unknown0x0A54 = 5.5f;
    unknown0x0A48 = -1.0f;
    unknown0x0A4C = 3.0f;
    unknown0x0A50 = 2.5f;
    boat[0] = '\0';
    stand_state = 100;
}

// FUNCTION: TRILOGY 0x00419D0F
// Armor class: worn AC, monk weight modifier, rogue/beastlord agility, Iksar natural AC, Defense skill, buffs.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall. The armor class: the AC of the worn items (inventory slots 1..20, EQ_Equipment::AC), plus the
// class, race, level, agility, skill and spell bonuses. cap != 0 caps the total at the class and level limit
// (0x15E, 0x177, 0x193 or 0x1AE). The return is a short, so EAX keeps the high word of `player`.
// @early-stop: no EBX=5 register constant and no duplicated level+5 arms (VC6 CSE); each EQ_THISCALL adds xor edx,edx.
s16 EQ_Character_guess::ac_guess(int cap)
{
    EQ_Item** ppEVar5;
    EQ_Item* pEVar2;
    EQPlayer* pEVar9;
    u8 charLevel;
    u8 classId;
    s16 scratch;
    u32 skillValue;
    int temp;
    int acCap;
    int tmp;
    int agi;
    int acc;
    int agiBonus;
    int weightLimit;
    int thr;

    acc = 0;
    agiBonus = 0;
    ppEVar5 = inventory_items + 1;
    tmp = 0x14;
    do
    {
        pEVar2 = *ppEVar5;
        if (pEVar2 != 0 && pEVar2->type == EQ_ITEM_EQUIPMENT)
            acc = acc + ((EQ_Equipment*)pEVar2)->AC;
        ppEVar5 = ppEVar5 + 1;
        tmp = tmp - 1;
    } while (tmp != 0);

    tmp = EQ_CDECL(int, 0x00419CF5, (EQ_Character*))((EQ_Character*)this);
    if (tmp == 0)
        acc = (acc << 2) / 3;

    if (cap != 0 && player != 0 && player->type == 0)
    {
        tmp = (int)level * 6 + 0x19;
        if (acc > tmp)
            acc = tmp;
    }

    charLevel = level;
    weightLimit = 0x1e;
    thr = 0xe;
    if (0xe < charLevel)
    {
        weightLimit = 0x20;
        thr = 0xf;
    }
    if (0x1d < charLevel)
    {
        weightLimit = 0x22;
        thr = 0x10;
    }
    if (0x2c < charLevel)
    {
        weightLimit = 0x24;
        thr = 0x11;
    }
    if (0x32 < charLevel)
    {
        weightLimit = 0x26;
        thr = 0x12;
    }
    if (0x36 < charLevel)
    {
        weightLimit = 0x28;
        thr = 0x14;
    }
    if (0x3b < charLevel)
    {
        weightLimit = 0x2d;
        thr = 0x18;
    }

    if (class_ == 7 && (scratch = EQ_METHOD(0, s16, 0x0041BC98, (CharacterProfile*), (CharacterProfile*)this)()) < weightLimit - 1)
    {
        double f;

        f = (double)(level + 5);
        if ((int)scratch > thr)
        {
            double x;

            x = (double)((int)scratch - thr) * EQ_GLOBAL(double, 0x005352B8);
            if (x > EQ_GLOBAL(double, 0x005352B0))
                x = EQ_GLOBAL(double, 0x005352B0);
            f = (EQ_GLOBAL(double, 0x005352B0) - x) * EQ_GLOBAL(double, 0x00535238) * f;
            if (f < EQ_GLOBAL(double, 0x00535100))
                f = 0.0;
        }
        scratch = (s16)(f * EQ_GLOBAL(double, 0x005352A8) * EQ_GLOBAL(double, 0x005352A0));
        acc = acc + scratch;
    }
    if (acc < 0)
        acc = 0;

    if (class_ == 7 && (int)(scratch = EQ_METHOD(0, s16, 0x0041BC98, (CharacterProfile*), (CharacterProfile*)this)()) > weightLimit + 1)
    {
        double f;
        double q;

        f = (double)(level + 5);
        q = ((double)(int)scratch - ((double)weightLimit - EQ_GLOBAL(double, 0x00535298))) * EQ_GLOBAL(double, 0x00535238);
        if (q > EQ_GLOBAL(double, 0x00535290))
            q = 1.0;
        scratch = (s16)(f * EQ_GLOBAL(double, 0x005352A8) * EQ_GLOBAL(double, 0x005352A0) * q);
        acc = acc - scratch;
    }

    if (class_ == 9)
    {
        agi = EQ_METHOD(0, u8, 0x0041B722, (CharacterProfile*), (CharacterProfile*)this)();
        if (agi > 0x4b && level > 0x1d)
        {
            tmp = level - 0x1a;
            if (agi >= 0x50)
            {
                if (agi < 0x55)
                    tmp = tmp * 2;
                else if (agi < 0x5a)
                    tmp = tmp * 3;
                else if (agi < 100)
                    tmp = tmp * 4;
                else
                    tmp = tmp * 5;
            }
            tmp = tmp / 4;
            if (tmp > 0xc)
                tmp = 0xc;
            acc = acc + tmp;
        }
    }

    if (class_ == 0xf)
    {
        agi = EQ_METHOD(0, u8, 0x0041B722, (CharacterProfile*), (CharacterProfile*)this)();
        if (10 < level)
        {
            tmp = level - 6;
            if (agi >= 0x50)
            {
                if (agi < 0x55)
                    tmp = tmp * 2;
                else if (agi < 0x5a)
                    tmp = tmp * 3;
                else if (agi < 100)
                    tmp = tmp * 4;
                else
                    tmp = tmp * 5;
            }
            tmp = tmp / 5;
            if (tmp > 0x10)
                tmp = 0x10;
            acc = acc + tmp;
        }
    }

    if (race == 0x80)
    {
        temp = level;
        if (0x23 < temp)
            temp = 0x23;
        if (temp < 10)
            temp = 10;
        acc = acc + temp;
    }

    if (EQ_CDECL(int, 0x00419CF5, (EQ_Character*))((EQ_Character*)this))
    {
        skillValue = EQ_METHOD(1, u32, 0x00419867, (CharacterProfile*, u32), (CharacterProfile*)this)(0xf);
        tmp = (int)skillValue / 2;
    }
    else
    {
        skillValue = EQ_METHOD(1, u32, 0x00419867, (CharacterProfile*, u32), (CharacterProfile*)this)(0xf);
        tmp = (int)skillValue / 3;
    }
    acc = acc + tmp;

    charLevel = EQ_METHOD(0, u8, 0x0041B722, (CharacterProfile*), (CharacterProfile*)this)();
    if (0x46 < charLevel)
    {
        temp = EQ_METHOD(0, u8, 0x0041B722, (CharacterProfile*), (CharacterProfile*)this)();
        agiBonus = (temp & 0xff) / 0x14;
    }
    acc = acc + agiBonus;

    if (EQ_CDECL(int, 0x00419CF5, (EQ_Character*))((EQ_Character*)this))
    {
        scratch = EQ_METHOD(1, s16, 0x004238A2, (CharacterProfile*, int), (CharacterProfile*)this)(1);
        acc = acc + (int)scratch / 3;
    }
    else
    {
        scratch = EQ_METHOD(1, s16, 0x004238A2, (CharacterProfile*, int), (CharacterProfile*)this)(1);
        acc = acc + (int)scratch / 4;
    }
    if (acc < 0)
        acc = 0;

    pEVar9 = player;
    if (pEVar9 != 0 && pEVar9->type == 0)
    {
        classId = class_;
        acCap = 0x15e;
        if (classId == 1 && 0x32 < level)
            acCap = 0x1ae;
        if ((classId == 3 || classId == 5 || classId == 8) && 0x32 < level)
            acCap = 0x193;
        if ((classId == 4 || classId == 9 || classId == 7 || classId == 0xf) && 0x32 < level)
            acCap = 0x177;
        if (acc > acCap && cap != 0)
            acc = acCap;
    }
    return (s16)acc;
}

// FUNCTION: TRILOGY 0x0041A0C5
// Calculates the character's to-hit rating for a chosen attack skill.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// The skill reaches HasSkillPassthrough as 16 bits, so the pushed dword keeps the upper half of Skill(0x21)'s result, as in the original.
u16 EQ_Character_guess::compute_tohit(u8 skill)
{
    int tohit;
    EQPlayer* spawn;

    if (this == 0)
    {
        EQ_CDECL(void, 0x004A3D01, (const char*))( (const char*)0x005401D8);
        return 0;
    }

    tohit = EQ_METHOD(1, u32, 0x00419867, (CharacterProfile*, u32), (CharacterProfile*)this)(0x21);
    tohit += 7;
    if (EQ_METHOD(1, bool, 0x004246A9, (CharacterProfile*, u16), (CharacterProfile*)this)((u16)skill))
        tohit += EQ_METHOD(1, u32, 0x00419867, (CharacterProfile*, u32), (CharacterProfile*)this)(skill);

    spawn = player;
    if (spawn->type == 0)
    {
        int half_intoxication = intoxication / 2;
        if (half_intoxication > EQ_GLOBAL(double, 0x005352C8))
        {
            double scale = EQ_GLOBAL(double, 0x005352C0) - half_intoxication;
            scale *= EQ_GLOBAL(double, 0x00535238);
            if (scale > EQ_GLOBAL(double, 0x00535290))
                scale = 1.0;
            tohit = (s16)(int)(tohit * scale);
        }
        else if (spawn != 0 && spawn->actor_info != 0
            && spawn->actor_info->unk_0x2F4 != 0)
        {
            tohit += (level << 1) / 5;
        }
    }

    if (tohit < 1)
        tohit = 1;
    return (u16)tohit;
}

// FUNCTION: TRILOGY 0x0041A1A8
// Supplies the Agility-based portion of a character's defense rating.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, no args. The agility part of the defense bonus, banded by Agi (the value is read from Agi_guess
// several times, one call per band). Below 40: (Agi - 40) * 25 / 40. 40 to 74: 0 when Agi is 60 or less, else
// from 200 - Agi. 75 and up: from 200 - Agi (clamped at 0) with the base picked by the level at +0x38.
s16 EQ_Character_guess::defense_agility_bonus(void)
{
    u8 agility;
    s16 base;
    s16 scaled;
    u32 result;

    agility = EQ_METHOD(0, int, 0x0041B722, (EQ_Character_guess*), (EQ_Character_guess*)this)();
    if (agility < 0x28)
    {
        agility = EQ_METHOD(0, int, 0x0041B722, (EQ_Character_guess*), (EQ_Character_guess*)this)();
        result = (int)((agility - 0x28) * 0x19) / 0x28;
        return result;
    }
    agility = EQ_METHOD(0, int, 0x0041B722, (EQ_Character_guess*), (EQ_Character_guess*)this)();
    if (agility > 0x4a)
    {
        agility = EQ_METHOD(0, int, 0x0041B722, (EQ_Character_guess*), (EQ_Character_guess*)this)();
        scaled = 200 - (u16)agility;
        if (scaled < 0)
            scaled = 0;
        if (level < 7)
        {
            scaled = scaled / 5;
            base = 0x23;
        }
        else if (level < 0x14)
        {
            scaled = scaled / 5;
            base = 0x37;
        }
        else if (level < 0x28)
        {
            scaled = scaled / 5;
            base = 0x46;
        }
        else
        {
            scaled = scaled / 5;
            base = 0x50;
        }
    }
    else
    {
        result = EQ_METHOD(0, int, 0x0041B722, (EQ_Character_guess*), (EQ_Character_guess*)this)();
        if ((u8)result > 0x3c)
        {
            agility = EQ_METHOD(0, int, 0x0041B722, (EQ_Character_guess*), (EQ_Character_guess*)this)();
            scaled = (s16)(200 - (u16)agility) / 5;
            base = 0x1c;
        }
        else
            return 0;
    }
    result = ((int)(s16)(base - scaled) << 1) / 3;
    return result;
}

// FUNCTION: TRILOGY 0x0041A26B
// Calculates a character's defense rating from Defense skill and Agility, with intoxication effects.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, no args. The defense value: the Defense skill (0x0F) times 400 / 225, plus the agility bonus
// (defense_agility_bonus). A player (type 0) with intoxication / 2 above the double at 0x005352C8 (20.0)
// has the sum scaled by the factor (the double at 0x005352C0 - intoxication / 2) * the double at 0x00535238,
// which is 1.0 above the double at 0x00535290. At least 1. 0 (with an EQTRACE) when `this` is NULL.
short EQ_Character_guess::compute_defense(void)
{
    short skill;
    short defense;
    int intox_half;

    if (this == 0)
    {
        EQ_CDECL(void, 0x004A3D01, (const char*))((const char*)0x005401D8);
        return 0;
    }
    skill = (short)EQ_METHOD(1, u32, 0x00419867, (EQ_Character_guess*, u32), (EQ_Character_guess*)this)(0xf);
    if (skill < 0)
        skill = 0;
    defense = EQ_METHOD(0, short, 0x0041A1A8, (EQ_Character_guess*), (EQ_Character_guess*)this)();
    defense = defense + (short)((skill * 400) / 0xe1);
    if (player->type == 0)
    {
        intox_half = (int)intoxication / 2;
        // The factor is never stored: it stays unrounded on the FPU stack, so its expression is written twice.
        if ((double)intox_half > EQ_GLOBAL(double, 0x005352C8))
            defense = (short)((double)defense *
                ((EQ_GLOBAL(double, 0x005352C0) - (double)intox_half) * EQ_GLOBAL(double, 0x00535238) > EQ_GLOBAL(double, 0x00535290)
                    ? 1.0 : (EQ_GLOBAL(double, 0x005352C0) - (double)intox_half) * EQ_GLOBAL(double, 0x00535238)));
    }
    if (defense < 1)
        defense = 1;
    return defense;
}

// FUNCTION: TRILOGY 0x0041a32c
// Calculates the character's attack rating for a skill, including relevant stat and spell bonuses.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, 1 arg. The offense with a skill (0xff: the skill of the equipment item in the weapon slot, 0x1c when
// the slot holds none): the skill's value when HasSkillPassthrough accepts it, plus (2 * stat - 150) / 3 when the
// stat is at least 75 (the stat is Dex for skills 7 and 0x33, else ComputeStr_guess), plus the spell affects; the total
// is at least 1. Callers read AX only, so the result is the low word as s16.
s16 EQ_Character_guess::offense_guess(u8 skill)
{
    EQ_Item* weapon;
    char has_skill;
    s16 offense;
    int weapon_bonus;
    u8 stat;
    int spell_affects;

    offense = 0;
    weapon_bonus = 0;
    if (skill == 0xff)
    {
        weapon = inventory_items[0x0d];
        if (weapon != 0 && weapon->type == 0)
            skill = EQ_METHOD(1, u8, 0x004B422A, (EQPlayer*, u8), (EQPlayer*)player)(((EQ_Equipment*)weapon)->item_type);
        else
            skill = 0x1c;
    }

    has_skill = (char)EQ_METHOD(1, int, 0x004246A9, (CharacterProfile*, short), (CharacterProfile*)this)((short)skill);
    if (has_skill != '\0')
        offense = (s16)EQ_METHOD(1, u32, 0x00419867, (CharacterProfile*, u32), (CharacterProfile*)this)((u32)skill);

    if (skill != 7 && skill != 0x33)
    {
        if (EQ_METHOD(0, u8, 0x0041B0C3, (CharacterProfile*), (CharacterProfile*)this)() >= 0x4b)
        {
            stat = EQ_METHOD(0, u8, 0x0041B0C3, (CharacterProfile*), (CharacterProfile*)this)();
            weapon_bonus = ((int)stat * 2 - 0x96) / 3;
        }
    }
    else
    {
        if ((u8)EQ_METHOD(0, s32, 0x0041B48F, (CharacterProfile*), (CharacterProfile*)this)() >= 0x4b)
        {
            stat = (u8)EQ_METHOD(0, s32, 0x0041B48F, (CharacterProfile*), (CharacterProfile*)this)();
            weapon_bonus = ((int)stat * 2 - 0x96) / 3;
        }
    }

    spell_affects = EQ_METHOD(1, int, 0x004238A2, (CharacterProfile*, u8), (CharacterProfile*)this)(2);
    offense = (s16)(offense + (s16)(spell_affects + weapon_bonus));
    if (offense < 1)
        offense = 1;
    return offense;
}

// FUNCTION: TRILOGY 0x0041a3e9
// Reports the character's usable skill rank for actions and skill requirements.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (Mac: EQ_Character::HasSkill, low)
// @early-stop: level/highest regs (AL/CL vs CL/DL), LEA vs ADD plus else, low ternary NEG/SBB vs CMP/SBB/NOT
// __thiscall. The usable level of a skill: the trained value when the class can learn it (skill record
// byte +0x68 for the class) and the character's level reaches the record's minimum (+0x28; a player
// below its highest level counts up to 2 levels more); 1 when allowed but untrained; otherwise the
// racial innate level. Skills 100+ are 0.
u8 EQ_Character::HasSkill(u8 skill)
{
    int effective_level;
    int innate;
    u8 trained;
    char* record;

    if (skill < 100)
    {
        if (player != 0 && player->type == 0 && level < highest_level)
        {
            effective_level = highest_level;
            if (effective_level >= level + 2)
                effective_level = level + 2;
        }
        else
        {
            effective_level = level;
        }
        innate = GetInnateSkill_guess(skill, race);
        trained = skills[skill];
        if (trained >= 0xFE)
            return innate;
        record = g_skill_records_guess[skill] + class_;
        if (record[0x68] == 0)
            return innate;
        if (effective_level >= (u8)record[0x28])
        {
            if (trained < 1)
                return 1;
            return trained;
        }
        return trained != 0 ? innate : 0;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0041A4FC
// Initializes racial and class languages and applies the character's Intelligence learning bonus.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall, no return value. Clears the language skills (languages[0..31] as eight dwords), sets the
// race and class starting values ('d' = 100, '_' = 95, 0x19 = 25), then adds the Int bonus (Int_guess
// minus 75, halved, capped at 30, 0 below 75) to every nonzero language under 100, capped at 100.
// Skill 0x49 is reset to 0 when it is 0xfe or 0xff for classes 1, 3 and 4.
/* fake match: the first Int check masks bit 0 like the second (the asm is test eax,0xfffffffe); a plain x < 0 compiles to jns */
void EQ_Character_guess::InitMyLanguages(void)
{
    u8 classId;
    u8 langValue;
    int charRace;
    s16 intBonus;
    u32 intValue;
    int tmp;
    int count;
    u8* langPtr;

    classId = class_;
    if (((classId == 1) || (classId == 3) || (classId == 4)) && (skills[0x49] >= 0xfe))
    {
        skills[0x49] = 0;
    }
    langPtr = languages;
    for (count = 0; count < 8; count++)
    {
        ((u32*)langPtr)[count] = 0;
    }
    if (class_ == 9)
    {
        languages[10] = 'd';
    }
    charRace = race;
    switch (charRace)
    {
    case 2:
        languages[1] = 'd';
        *langPtr = 'd';
        break;
    case 3:
        languages[2] = 'd';
        *langPtr = 'd';
        break;
    case 5:
        languages[3] = 'd';
        *langPtr = 'd';
        languages[4] = 0x19;
        languages[0xc] = 0x19;
        break;
    case 6:
        languages[4] = 'd';
        languages[3] = 0x19;
        *langPtr = 'd';
        languages[0xc] = 0x19;
        languages[0x17] = 'd';
        break;
    case 4:
    case 7:
        languages[3] = 'd';
        *langPtr = 'd';
        break;
    case 8:
        languages[5] = 'd';
        languages[8] = 0x19;
        *langPtr = 'd';
        break;
    case 9:
        languages[6] = 'd';
        *langPtr = '_';
        languages[0x17] = 'd';
        break;
    case 10:
        languages[7] = 'd';
        *langPtr = '_';
        languages[0x17] = 'd';
        break;
    case 11:
        languages[9] = 'd';
        *langPtr = 'd';
        break;
    case 12:
        languages[8] = 'd';
        languages[5] = 0x19;
        *langPtr = 'd';
        break;
    case 1:
        *langPtr = 'd';
        break;
    case 0x80:
        languages[0x12] = 'd';
        languages[0x17] = 'd';
        *langPtr = '_';
        break;
    case 0x82:
        unknown0x0096[0] = 'd';
        *langPtr = 'd';
        languages[2] = 0x19;
        languages[0x10] = 'd';
        break;
    }
    intValue = EQ_METHOD(0, s32, 0x0041B5CC, (CharacterProfile*), (CharacterProfile*)this)();
    if ((int)((u8)intValue - 0x4b & 0xfffffffe) < 0)
    {
        intBonus = 0;
    }
    else
    {
        intValue = EQ_METHOD(0, s32, 0x0041B5CC, (CharacterProfile*), (CharacterProfile*)this)();
        if ((int)((u8)intValue - 0x4b & 0xfffffffe) > 0x3c)
        {
            intBonus = 0x1e;
        }
        else
        {
            intValue = EQ_METHOD(0, s32, 0x0041B5CC, (CharacterProfile*), (CharacterProfile*)this)();
            intBonus = (short)((int)((u8)intValue - 0x4b) >> 1);
        }
    }
    EQ_CDECL(void, 0x004A3D01, (const char*, int))((const char*)0x0054020C, (int)intBonus);
    count = 0x20;
    do
    {
        langValue = *langPtr;
        if ((langValue > 0) && (langValue < 100))
        {
            tmp = (u32)langValue + (int)intBonus;
            if (100 < tmp)
                tmp = 100;
            *langPtr = (u8)tmp;
        }
        langPtr = langPtr + 1;
        count = count + -1;
    } while (count != 0);
    return;
}

// FUNCTION: TRILOGY 0x0041a6d6
// Adjusts combat and resistance values for the level difference with another character.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// __thiscall. A modifier from the level difference to `other_level`: diff^2 / 2 with the sign of diff
// (diff capped at 10 for a player against level 21+, at -9 for an NPC), or, with g_pvpRuleset_guess == 4 and
// a player, 2 * |diff| with diff clamped to -30..30 (the sign still from the capped diff).
int EQ_Character::LevelDiffModifier_guess(int other_level)
{
    // capped is recomputed, not copied from diff: a copy makes VC6 compare through diff's register (eax)
    int capped = level - other_level;
    EQPlayer* spawn = player;
    int diff = level - other_level;
    int result;

    if (spawn != 0 && spawn->type == 0)
    {
        if (other_level > 20 && capped > 10)
            capped = 10;
    }
    else if (player != 0 && player->type == 1 && capped < -9)
        capped = -9;
    if (g_pvpRuleset_guess == 4 && spawn != 0 && spawn->type == 0)
    {
        if (diff > 30)
            diff = 30;
        if (diff < -30)
            diff = -30;
        result = abs(diff) * 2;
    }
    else
        result = capped * capped / 2;
    if (capped < 0)
        result = -result;
    return result;
}

// FUNCTION: TRILOGY 0x0041a75e
// Stun chance (percent): the level-difference modifier plus 55, compared with rand 0..100 by the stun check.
// __thiscall. Returns the level-difference modifier shifted by 55. The low-confidence TAKP
// name "no_op_return_zero" is contradicted by the Trilogy body.
int EQ_Character::LevelDiffWithBase_guess(int other_level)
{
    return LevelDiffModifier_guess(other_level) + 0x37;
}

// FUNCTION: TRILOGY 0x0041af14
// Calculates poison resistance against a target level, including racial, class, item and spell bonuses.
// @early-stop: `volatile` accumulator keeps 7 updates as load/op/store instead of the original's RMW, two loop bottoms stay inverted.
/* fake match: `volatile` on sum only pins the accumulator to its stack slot the way the
   original compiles it; it changes no behaviour in this single-threaded function. */
int EQ_Character::GetPoisonResist_guess(int other_level)
{
    unsigned char level;
    EQ_Item* inner;
    short spell_affects;
    int scratch;
    int* slot;
    int* content;
    int item;
    int found = 0;
    int count = 0x14;
    volatile int sum = 0xf;
    int inv_slot;
    int inner_count;

    // races 8 (Dwarf) and 11 (Halfling) start at 20, others at 15
    if ((this->race == 0xb) || (this->race == 8))
    {
        sum = count;
    }
    // class 7 (Monk): +1 per level over 50
    if ((this->class_ == 7) && (0x32 < this->level))
    {
        sum = sum + (unsigned int)this->level - 0x32;
    }
    if (((this->player != 0) &&
        (scratch = *(int*)((char*)this->player + 0x70), scratch != 0)) &&
        (*(int*)(scratch + 0x2dc) == 0x1e))
    {
        // +3, +1 above 34, +1 above 40, then +1 per level above 45 (max 5)
        level = this->level;
        sum += 3;
        if (0x22 < level)
        {
            sum++;
        }
        if (0x28 < level)
        {
            sum++;
        }
        if (0x2d < level)
        {
            scratch = level - 0x2d;
            if (5 < scratch)
            {
                scratch = 5;
            }
            sum += scratch;
        }
    }
    slot = (int*)&this->inventory_items[1];
    do
    {
        scratch = *slot;
        if ((scratch != 0) && (((EQ_Item*)scratch)->type == EQ_ITEM_EQUIPMENT))
        {
            sum += ((EQ_Equipment*)scratch)->PR;
        }
        slot = slot + 1;
        count = count + -1;
    } while (count != 0);
    // first item_type 14 equipment in slots 22..29, or in their bags
    inv_slot = 0x16;
    slot = (int*)&this->inventory_items[22];
    do
    {
        item = *slot;
        if (item != 0)
        {
            if (((EQ_Item*)item)->type == EQ_ITEM_EQUIPMENT)
            {
                if (((EQ_Equipment*)item)->item_type == 14)
                {
                    sum += ((EQ_Equipment*)item)->PR;
                    break;
                }
            }
            else if ((*slot != 0) && (((EQ_Item*)*slot)->type == EQ_ITEM_CONTAINER))
            {
                inner_count = ((EQ_Container*)item)->slots;
                scratch = 0;
                if (inner_count > 0)
                {
                    content = (int*)((EQ_Container*)item)->contents;
                    do
                    {
                        inner = (EQ_Item*)*content;
                        if (((inner != 0) && (inner->type == EQ_ITEM_EQUIPMENT)) &&
                            (((EQ_Equipment*)inner)->item_type == 14))
                        {
                            sum += ((EQ_Equipment*)inner)->PR;
                            found = true;
                            break;
                        }
                        scratch = scratch + 1;
                        content = content + 1;
                    } while (scratch < inner_count);
                }
                if (found)
                    break;
            }
        }
        inv_slot = inv_slot + 1;
        slot = slot + 1;
    } while (inv_slot <= 0x1d);
    spell_affects = EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), this)(0x30);
    // item is reused from here on as the running total
    item = EQ_METHOD(1, int, 0x0041A6D6, (EQ_Character*, int), this)(other_level);
    item = spell_affects + sum + item;
    if ((this->player != 0) &&
        (*((char*)this->player + 0x98) == '\0'))
    {
        // class 9 (Rogue): +8, plus 1 per level above 49
        if (this->class_ == 9)
        {
            item += 8;
            if (0x31 < this->level)
            {
                item = item + -0x31 + (unsigned int)this->level;
            }
        }
        // class 5 (Shadow Knight): +4, plus 1 per level above 49
        if (this->class_ == 5)
        {
            item += 4;
            if (0x31 < this->level)
            {
                item = item + -0x31 + (unsigned int)this->level;
            }
        }
    }
    return item;
}

// FUNCTION: TRILOGY 0x0041b0c3
// Computes the effective STR: the base stat plus item and spell bonuses, clamped to 1..255.
int __fastcall ComputeStr_guess(EQ_Character* eqChar)
{
    int bagItem;
    int found;
    short statBonusShort;
    int* itemPtr;
    unsigned int baseStr;
    int* bagSlotPtr;
    int tmp;
    int scratch;
    int inner_count;
    int slotIndex;
    int statBonus;
    char* this_bytes = (char*)eqChar;

    statBonus = 0;
    found = false;
    itemPtr = (int*)(this_bytes + 0xe4);
    tmp = 0x14;
    do
    {
        scratch = *itemPtr;
        if ((scratch != 0) && (*(char*)(scratch + 0x81) == '\0'))
            statBonus = statBonus + *(char*)(scratch + 0xac);
        itemPtr++;
        tmp--;
    } while (tmp != 0);

    for (slotIndex = 0x16, itemPtr = (int*)(this_bytes + 0x138);
         slotIndex <= 0x1d; slotIndex++, itemPtr++)
    {
        tmp = *itemPtr;
        if (tmp != 0)
        {
            if (*(unsigned char*)(tmp + 0x81) == 0)
            {
                if (*(char*)(tmp + 0xc2) == '\x0e')
                {
                    statBonus = statBonus + *(char*)(tmp + 0xac);
                    break;
                }
                continue;
            }
            else if ((*itemPtr != 0) && (*(unsigned char*)(*itemPtr + 0x81) == 1))
            {
                inner_count = (int)(unsigned int)*(unsigned char*)(tmp + 0xd5);
                scratch = 0;
                if (inner_count > 0)
                {
                    for (bagSlotPtr = (int*)(tmp + 0xac); scratch < inner_count;
                         scratch++, bagSlotPtr++)
                    {
                        bagItem = *bagSlotPtr;
                        if (((bagItem != 0) && (*(char*)(bagItem + 0x81) == '\0'))
                            && (*(char*)(bagItem + 0xc2) == '\x0e'))
                        {
                            statBonus = statBonus + *(char*)(bagItem + 0xac);
                            found = true;
                            break;
                        }
                    }
                }
                if (found)
                    break;
            }
        }
    }

    tmp = *(int*)(this_bytes + 0xa0);
    if ((tmp != 0) && (*(char*)(tmp + 0x98) == '\0'))
    {
        if (*(short*)(this_bytes + 0x104c) > 0x37)
        {
            statBonusShort = (short)((*(short*)(this_bytes + 0x104c) - 0x28) / 0x10);
            if (0 < statBonusShort)
                statBonus = statBonus + statBonusShort;
        }
        else
        {
            if ((*(int*)(tmp + 0x70) != 0)
                && (*(int*)(*(int*)(tmp + 0x70) + 0x2f4) != 0))
                statBonus = statBonus + 9 + *(unsigned char*)(this_bytes + 0x38) / 10;
        }
    }

    statBonusShort = EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), eqChar)(4);
    scratch = (int)statBonusShort;
    baseStr = EQ_METHOD(1, u8, 0x0041B9E0, (EQ_Character*, char), eqChar)(*(char*)(this_bytes + 0x77));
    tmp = (baseStr & 0xff) + scratch + statBonus;
    if (tmp < 0)
    {
        tmp = 1;
    }
    else
    {
        baseStr = EQ_METHOD(1, u8, 0x0041B9E0, (EQ_Character*, char), eqChar)(*(char*)(this_bytes + 0x77));
        tmp = (baseStr & 0xff) + scratch + statBonus;
        if (tmp > 0xff)
            tmp = 0xff;
        else
        {
            baseStr = EQ_METHOD(1, u8, 0x0041B9E0, (EQ_Character*, char), eqChar)(*(char*)(this_bytes + 0x77));
            tmp = (baseStr & 0xff) + scratch + statBonus;
            if (tmp > 0xff)
                tmp = 0xff;
            else if (tmp < 1)
                tmp = 1;
        }
    }
    return tmp;
}

// FUNCTION: TRILOGY 0x0041b258
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
/* fake match: inner_slot = 0 comes before the count > 0 guard and the inner loop is a while; that order gives the original's EDI/EDX split and bottom-tested loop */
int EQ_Character_guess::Sta_guess(void)
{
    int total = 0;
    int slot;
    int found = 0;
    short intoxication_bonus;
    int count;
    int inner_count;
    int inner_slot;
    EQ_Item** item_slot;
    EQ_Item** inner_item_slot;
    EQ_Item* item;
    EQ_Item* inner_item;
    int spell_affect;
    int result;

    item_slot = inventory_items + 1;
    count = 0x14;
    do
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
            total += ((EQ_Equipment*)item)->STA;
        item_slot++;
        count--;
    } while (count != 0);

    slot = 0x16;
    item_slot = inventory_items + 0x16;
    while (slot <= 0x1D)
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
        {
            if (((EQ_Equipment*)item)->item_type == 0x0E)
            {
                total += ((EQ_Equipment*)item)->STA;
                break;
            }
        }
        else if (*item_slot != 0 && (*item_slot)->type == EQ_ITEM_CONTAINER)
        {
            inner_count = ((EQ_Container*)item)->slots;
            inner_slot = 0;
            if (inner_count > 0)
            {
                inner_item_slot = ((EQ_Container*)item)->contents;
                while (inner_slot < inner_count)
                {
                    inner_item = *inner_item_slot;
                    if (inner_item != 0
                        && inner_item->type == EQ_ITEM_EQUIPMENT
                        && ((EQ_Equipment*)inner_item)->item_type == 0x0E)
                    {
                        total += ((EQ_Equipment*)inner_item)->STA;
                        found = 1;
                        break;
                    }
                    inner_slot++;
                    inner_item_slot++;
                }
            }
            if (found)
                break;
        }
        slot++;
        item_slot++;
    }

    if (player != 0 && player->type == 0 && intoxication > 0x37)
    {
        intoxication_bonus = (short)((intoxication - 0x28) / 0x10);
        if (intoxication_bonus > 0)
            total += intoxication_bonus;
    }
    spell_affect = (short)EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, char), (EQ_Character*)this)(7);
    result = (u8)STA + spell_affect + total;
    if (result < 1)
        result = 1;
    if (0xFF < result)
        result = 0xFF;
    return result;
}

// FUNCTION: TRILOGY 0x0041b38f
// Calculates effective Charisma from base stats, equipment, food and Charisma spell effects.
int EQ_Character_guess::Cha_guess(void)
{
    int total = 0;
    int slot;
    int found = 0;
    int count;
    int inner_count;
    int inner_slot;
    EQ_Item** item_slot;
    EQ_Item** inner_item_slot;
    EQ_Item* item;
    EQ_Item* inner_item;
    int spell_affect;
    int result;

    item_slot = inventory_items + 1;
    count = 0x14;
    do
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
            total += ((EQ_Equipment*)item)->CHA;
        item_slot++;
        count--;
    } while (count != 0);

    slot = 0x16;
    item_slot = inventory_items + 0x16;
    while (slot <= 0x1D)
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
        {
            if (((EQ_Equipment*)item)->item_type == 0x0E)
            {
                total += ((EQ_Equipment*)item)->CHA;
                break;
            }
        }
        else if (*item_slot != 0 && (*item_slot)->type == EQ_ITEM_CONTAINER)
        {
            inner_count = ((EQ_Container*)item)->slots;
            inner_slot = 0;
            if (inner_count > 0)
            {
                inner_item_slot = ((EQ_Container*)item)->contents;
                while (inner_slot < inner_count)
                {
                    inner_item = *inner_item_slot;
                    if (inner_item != 0
                        && inner_item->type == EQ_ITEM_EQUIPMENT
                        && ((EQ_Equipment*)inner_item)->item_type == 0x0E)
                    {
                        total += ((EQ_Equipment*)inner_item)->CHA;
                        found = 1;
                        break;
                    }
                    inner_slot++;
                    inner_item_slot++;
                }
            }
            if (found)
                break;
        }
        slot++;
        item_slot++;
    }

    spell_affect = (short)EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, char), (EQ_Character*)this)(10);
    result = (u8)CHA + spell_affect + total;
    if (result < 1)
        result = 1;
    if (0xFF < result)
        result = 0xFF;
    return result;
}

// FUNCTION: TRILOGY 0x0041b48f
// Calculates effective Dexterity from base stats, gear, food and spells, with intoxication penalties.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall. The character's Dex: the DEX of equipment in inventory slots 1..20, plus the DEX of the first
// equipment with item_type 14 in slots 22..29 (a container there is searched too), minus the intoxication
// penalty of a player spawn (type 0), plus TotalSpellAffects(5) and the base DEX through ApplyFatigue_guess.
// Clamped to 1..255.
/* fake match: slot <= 0x1D, not < 0x1E: VC6 emits the jle against 0x1d that the original has; < 0x1E gives cmp 0x1e; jl */
int EQ_Character_guess::Dex_guess(void)
{
    int total = 0;
    int found = 0;
    int count;
    int slot;
    int inner_count;
    int inner_slot;
    EQ_Item** item_slot;
    EQ_Item** inner_item_slot;
    EQ_Item* item;
    EQ_Item* inner_item;
    int spell_affect;
    int intoxication_factor;
    u8 dex_stat;
    int result;

    item_slot = inventory_items + 1;
    count = 0x14;
    do
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
            total += ((EQ_Equipment*)item)->DEX;
        item_slot++;
        count--;
    } while (count != 0);

    slot = 0x16;
    item_slot = inventory_items + 0x16;
    for (; slot <= 0x1D; slot++, item_slot++)
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
        {
            if (((EQ_Equipment*)item)->item_type == 0x0E)
            {
                total += ((EQ_Equipment*)item)->DEX;
                break;
            }
        }
        else if (*item_slot != 0 && (*item_slot)->type == EQ_ITEM_CONTAINER)
        {
            inner_count = ((EQ_Container*)item)->slots;
            inner_slot = 0;
            if (inner_count > 0)
            {
                inner_item_slot = ((EQ_Container*)item)->contents;
                for (; inner_slot < inner_count; inner_slot++, inner_item_slot++)
                {
                    inner_item = *inner_item_slot;
                    if (inner_item != 0
                        && inner_item->type == EQ_ITEM_EQUIPMENT
                        && ((EQ_Equipment*)inner_item)->item_type == 0x0E)
                    {
                        total += ((EQ_Equipment*)inner_item)->DEX;
                        found = 1;
                        break;
                    }
                }
            }
            if (found)
                break;
        }
    }

    if (player != 0 && player->type == 0
        && (0x37 < intoxication
            && (intoxication_factor = (intoxication - 0x28) / 0x10,
                0 < intoxication_factor)))
    {
        total -= intoxication_factor;
    }
    spell_affect = (short)EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, char), (EQ_Character*)this)(5);
    dex_stat = EQ_METHOD(1, u8, 0x0041B9E0, (EQ_Character*, char), (EQ_Character*)this)(DEX);
    result = (dex_stat & 0xff) + spell_affect + total;
    if (result < 1)
        result = 1;
    if (0xFF < result)
        result = 0xFF;
    return result;
}

// FUNCTION: TRILOGY 0x0041b5cc
// Calculates effective Intelligence from base stats, gear, food and spells, adjusted by intoxication.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// __thiscall. The character's Int: its base INT, plus the INT of equipment in inventory slots 1..20, plus the
// INT of the first equipment with item_type 14 in slots 22..29 (a container there is searched too), plus
// TotalSpellAffects(8). A player spawn (type 0) that is intoxicated scales it by max(0.25, 1 - 0.01 * intoxication).
// Clamped to 1..255.
// @early-stop: the sum is kept in [ebp-8] (the original reuses found's slot [ebp-0xc]) and is loaded after the clamped factor, not before it (5 instructions)
int EQ_Character_guess::Int_guess(void)
{
    int total = 0;
    int found = 0;
    int count;
    int inner_count;
    int slot;
    int inner_slot;
    EQ_Item** item_slot;
    EQ_Item** inner_item_slot;
    EQ_Item* item;
    EQ_Item* inner_item;
    int spell_affect;
    int result;
    double intoxication_factor;

    item_slot = inventory_items + 1;
    count = 0x14;
    do
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
            total += ((EQ_Equipment*)item)->INT;
        item_slot++;
        count--;
    } while (count != 0);

    slot = 0x16;
    item_slot = inventory_items + 0x16;
    while (slot <= 0x1D)
    {
        item = *item_slot;
        if (item != 0)
        {
            if (item->type == EQ_ITEM_EQUIPMENT)
            {
                if (((EQ_Equipment*)item)->item_type == 0x0E)
                {
                    total += ((EQ_Equipment*)item)->INT;
                    break;
                }
            }
            else if (*item_slot != 0 && (*item_slot)->type == EQ_ITEM_CONTAINER)
            {
                inner_count = ((EQ_Container*)item)->slots;
                inner_slot = 0;
                if (inner_count > 0)
                {
                    inner_item_slot = ((EQ_Container*)item)->contents;
                    while (inner_slot < inner_count)
                    {
                        inner_item = *inner_item_slot;
                        if (inner_item != 0
                            && inner_item->type == EQ_ITEM_EQUIPMENT
                            && ((EQ_Equipment*)inner_item)->item_type == 0x0E)
                        {
                            total += ((EQ_Equipment*)inner_item)->INT;
                            found = 1;
                            break;
                        }
                        inner_slot++;
                        inner_item_slot++;
                    }
                }
                if (found)
                    break;
            }
        }
        slot++;
        item_slot++;
    }

    spell_affect = (short)EQ_METHOD(1, short, 0x004238A2, (EQ_Character_guess*, char), (EQ_Character_guess*)this)(8);
    result = (u8)INT + spell_affect + total;
    if (player != 0 && player->type == 0 && intoxication != 0)
    {
        intoxication_factor = EQ_GLOBAL(double, 0x00535290)
            - (int)intoxication * EQ_GLOBAL(double, 0x005352D8);
        if (intoxication_factor < EQ_GLOBAL(double, 0x005352D0))
            intoxication_factor = EQ_GLOBAL(double, 0x005352D0);
        result = (int)(result * intoxication_factor);
    }
    if (result < 1)
        result = 1;
    if (0xFF < result)
        result = 0xFF;
    return result;
}

// FUNCTION: TRILOGY 0x0041b722
// Calculates effective Agility with equipment, food, spell effects and encumbrance scaling.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
int EQ_Character_guess::Agi_guess(void)
{
    int total = 0;
    int found = 0;
    int count;
    int inner_count;
    int inner_slot;
    EQ_Item** item_slot;
    EQ_Item** inner_item_slot;
    EQ_Item* item;
    EQ_Item* inner_item;
    int spell_affect;
    int intoxication_factor;
    float value;

    item_slot = inventory_items + 1;
    count = 0x14;
    do
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
            total += ((EQ_Equipment*)item)->AGI;
        item_slot++;
        count--;
    } while (count != 0);

    {
        int slot = 0x16;
        item_slot = inventory_items + 0x16;
        // inventory slots 22..29
        while (slot <= 0x1D)
        {
            item = *item_slot;
            if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
            {
                if (((EQ_Equipment*)item)->item_type == 0x0E)
                {
                    total += ((EQ_Equipment*)item)->AGI;
                    break;
                }
            }
            else if (*item_slot != 0 && (*item_slot)->type == EQ_ITEM_CONTAINER)
            {
                inner_count = ((EQ_Container*)item)->slots;
                inner_slot = 0;
                if (inner_count > 0)
                {
                    inner_item_slot = ((EQ_Container*)item)->contents;
                    while (inner_slot < inner_count)
                    {
                        inner_item = *inner_item_slot;
                        if (inner_item != 0
                            && inner_item->type == EQ_ITEM_EQUIPMENT
                            && ((EQ_Equipment*)inner_item)->item_type == 0x0E)
                        {
                            total += ((EQ_Equipment*)inner_item)->AGI;
                            found = 1;
                            break;
                        }
                        inner_slot++;
                        inner_item_slot++;
                    }
                }
                if (found)
                    break;
            }
            slot++;
            item_slot++;
        }
    }

    if (player != 0 && player->type == 0
        && (0x37 < intoxication
            && (intoxication_factor = (intoxication - 0x28) / 0x10,
                0 < intoxication_factor)))
    {
        total -= intoxication_factor;
    }
    spell_affect = (short)EQ_METHOD(1, short, 0x004238A2, (EQ_Character_guess*, char), (EQ_Character_guess*)this)(6);
    value = (float)EQ_METHOD(1, u8, 0x0041B9E0, (EQ_Character_guess*, char), (EQ_Character_guess*)this)(AGI) + spell_affect + total;
    value = EQ_METHOD(0, float, 0x0041BB36, (EQ_Character_guess*), (EQ_Character_guess*)this)() * value;
    if (!(value >= EQ_GLOBAL(float, 0x005344A4)))
        value = 1.0f;
    if (value > EQ_GLOBAL(float, 0x00535230))
        value = EQ_GLOBAL(float, 0x00535230);
    return (int)value;
}

// FUNCTION: TRILOGY 0x0041b88a
// Computes effective Wisdom from gear and spells, with food and intoxication effects.
// @early-stop: the sum is kept in [ebp-8] (the original reuses found's slot [ebp-0xc]) and is loaded after the clamped factor, not before it (5 instructions)
int __fastcall Wis_guess(CharacterProfile* profile)
{
    int total = 0;
    int found = 0;
    int count;
    int inner_count;
    int slot;
    int inner_slot;
    EQ_Item** item_slot;
    EQ_Item** inner_item_slot;
    EQ_Item* item;
    EQ_Item* inner_item;
    int spell_affect;
    int result;
    double intoxication_factor;

    item_slot = profile->inventory_items + 1;
    count = 0x14;
    do
    {
        item = *item_slot;
        if (item != 0 && item->type == EQ_ITEM_EQUIPMENT)
            total += ((EQ_Equipment*)item)->WIS;
        item_slot++;
        count--;
    } while (count != 0);

    slot = 0x16;
    item_slot = profile->inventory_items + 0x16;
    while (slot <= 0x1D)
    {
        item = *item_slot;
        if (item != 0)
        {
            if (item->type == EQ_ITEM_EQUIPMENT)
            {
                if (((EQ_Equipment*)item)->item_type == 0x0E)
                {
                    total += ((EQ_Equipment*)item)->WIS;
                    break;
                }
            }
            else if (*item_slot != 0 && (*item_slot)->type == EQ_ITEM_CONTAINER)
            {
                inner_count = ((EQ_Container*)item)->slots;
                inner_slot = 0;
                if (inner_count > 0)
                {
                    inner_item_slot = ((EQ_Container*)item)->contents;
                    while (inner_slot < inner_count)
                    {
                        inner_item = *inner_item_slot;
                        if (inner_item != 0
                            && inner_item->type == EQ_ITEM_EQUIPMENT
                            && ((EQ_Equipment*)inner_item)->item_type == 0x0E)
                        {
                            total += ((EQ_Equipment*)inner_item)->WIS;
                            found = 1;
                            break;
                        }
                        inner_slot++;
                        inner_item_slot++;
                    }
                }
                if (found)
                    break;
            }
        }
        slot++;
        item_slot++;
    }

    spell_affect = (short)EQ_METHOD(1, short, 0x004238A2, (EQ_Character_guess*, char), (EQ_Character_guess*)profile)(9);
    result = (u8)profile->WIS + spell_affect + total;
    if (profile->player != 0 && profile->player->type == 0 && profile->intoxication != 0)
    {
        intoxication_factor = EQ_GLOBAL(double, 0x00535290)
            - (int)profile->intoxication * EQ_GLOBAL(double, 0x005352D8);
        if (intoxication_factor < EQ_GLOBAL(double, 0x005352D0))
            intoxication_factor = EQ_GLOBAL(double, 0x005352D0);
        result = (int)(result * intoxication_factor);
    }
    if (result < 1)
        result = 1;
    if (0xFF < result)
        result = 0xFF;
    return result;
}

// FUNCTION: TRILOGY 0x0041b9e0
// Applies the fatigue penalty to a player's effective attribute value.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// Returns statValue less min(10, fatigue - Sta_guess()) as a byte, when the profile has a player spawn of type 0,
// CCH_Screen is 0, g_pc_guess is set and fatigue is at least Sta_guess(); otherwise returns statValue.
// @early-stop: the original keeps an EBP frame with no stack locals (push ebp; mov ebp,esp); VC6 /O1 omits it here, so the params are [esp+N] not [ebp+N]
/* fake match: `statValue -= temp; return statValue;` keeps the subtraction a byte op (`return statValue - temp` gives a dword sub) */
char __fastcall ApplyFatigue_guess(EQ_Character* eqChar, int edx, char statValue)
{
    char curSta;
    u8 staThreshold;
    u8 temp;

    if (eqChar->player == 0 || eqChar->player->type != 0
        || EQ_GLOBAL(int, 0x006E4E70) != 0 || g_pc_guess == 0)
        return statValue;
    staThreshold = EQ_METHOD(0, u8, 0x0041B258, (EQ_Character*), eqChar)();
    if (eqChar->fatigue < staThreshold)
        return statValue;
    curSta = EQ_METHOD(0, char, 0x0041B258, (EQ_Character*), eqChar)();
    temp = eqChar->fatigue - curSta;
    if (10 < temp)
        temp = 10;
    statValue -= temp;
    return statValue;
}

// FUNCTION: TRILOGY 0x0041ba44
// Thunk to the character's full Strength calculation.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
u16 __fastcall EQ_Character_Str_guess(EQ_Character* profile, int edx)
{
    return EQ_FASTCALL(u8, 0x0041B0C3, (void))();
}

// FUNCTION: TRILOGY 0x0041bb36
// Returns the movement and Agility factor for carried weight, adjusted for low health.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
// @early-stop: only the (weight - strength) / (2 * strength) x87 form differs (fdivr st(1) + fstp st(1) vs fdivp).
/* fake match: the early `return healthFactor` in the not-encumbered arm raises healthFactor's use count, which fixes the stack slot order */
float __fastcall EQ_Character_encum_factor_guess(EQ_Character* profile, int edx)
{
    float weight;
    float strength;
    float curHp;
    int maxHp;
    float encumbranceFactor;
    float healthFactor;

    weight = (short)EQ_METHOD(0, int, 0x0041BC98, (void*), profile)();
    strength = (short)EQ_METHOD(0, int, 0x0041BA44, (void*), profile)();
    healthFactor = 1.0f;
    curHp = EQ_METHOD(1, int, 0x004199C0, (void*, int), 
        profile)( profile->player->type);
    maxHp = EQ_METHOD(1, int, 0x00419A2F, (void*, int), 
        profile)( profile->player->type);
    if (curHp < maxHp && (3 < profile->level))
    {
        if (curHp / maxHp < EQ_GLOBAL(float, 0x0053513C) && curHp < EQ_GLOBAL(float, 0x00535190))
            healthFactor = 0.5f;
        else if (curHp / maxHp < EQ_GLOBAL(float, 0x0053516C) && curHp < EQ_GLOBAL(float, 0x0053515C))
            healthFactor = 0.75f;
    }
    if (weight <= strength)
    {
        if (EQ_GLOBAL(int, 0x006B648C) < 0)
            EQ_GLOBAL(int, 0x006B648C) = 0;
        else if (EQ_GLOBAL(int, 0x006B648C) == 1)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), 
                EQ_GLOBAL(void*, 0x006EFE2C))( (const char*)0x00540260, 0xD);
            EQ_GLOBAL(int, 0x006B648C) = 0;
        }
        return healthFactor;
    }
    else
    {
        if ((EQ_GLOBAL(int, 0x006B648C) < 0)
            || (EQ_GLOBAL(int, 0x006B648C) == 0))
        {
            EQ_GLOBAL(int, 0x006B648C) = 1;
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), 
                EQ_GLOBAL(void*, 0x006EFE2C))( (const char*)0x0054024C, 0xD);
        }
        if (strength == 0.0f)
            EQ_CDECL(void, 0x004A3D01, (const char*))((const char*)0x0054021C);
        else
        {
            encumbranceFactor = healthFactor - (weight - strength) / (strength + strength);
            if (encumbranceFactor < EQ_GLOBAL(float, 0x0053510C))
                encumbranceFactor = 0.0f;
            return encumbranceFactor;
        }
    }
    return healthFactor;
}

// FUNCTION: TRILOGY 0x0041bc98
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (from Mac debug map)
// Weight of equipped and carried items, container contents, and carried/cursor coins.
int EQ_Character::cur_encumbrance(void)
{
    float items = 0.0f;
    int item_count;
    EQ_Item** item_ptr;
    EQ_Item** gear_ptr;
    int gear_count;
    int coins;
    float total;

    item_ptr = inventory_items;
    item_count = 30;
    do
    {
        EQ_Item* item = *item_ptr;
        if (item != 0)
        {
            items += (float)item->weight * 0.1f;
            if (item->type == EQ_ITEM_CONTAINER)
            {
                EQ_Container* bag = (EQ_Container*)item;
                int contents_slot;
                for (contents_slot = 0; contents_slot < bag->slots; contents_slot++)
                {
                    EQ_Item* content = bag->contents[contents_slot];
                    if (content != 0)
                    {
                        items += ((100.0f - (float)bag->weight_reduction) * 0.01f) *
                                 ((float)content->weight * 0.1f);
                    }
                }
            }
        }
        item_ptr++;
    } while (--item_count != 0);

    coins = copper_cursor;
    int reduction = 0;
    coins += silver_cursor;
    coins += gold_cursor;
    coins += platinum_cursor;
    coins += copper;
    coins += silver;
    coins += gold;
    coins += platinum;
    total = (float)coins * 0.025f;

    gear_ptr = inventory_items + 22;
    gear_count = 8;
    do
    {
        short id;
        int bonus;
        if (*gear_ptr != 0 && (id = (*gear_ptr)->item_nr) >= 0x4331 && id <= 0x433a)
        {
            bonus = (id - 0x4330) * 10;
            if (reduction < bonus)
                reduction = bonus;
        }
        if (*gear_ptr != 0 && (id = (*gear_ptr)->item_nr) >= 0x433b && id <= 0x4344)
        {
            bonus = (id - 0x433a) * 10;
            if (reduction < bonus)
                reduction = bonus;
        }
        if (*gear_ptr != 0 && (id = (*gear_ptr)->item_nr) >= 0x4345 && id <= 0x434e)
        {
            bonus = (id - 0x4344) * 10;
            if (reduction < bonus)
                reduction = bonus;
        }
        gear_ptr++;
    } while (--gear_count != 0);

    if (reduction > 0 && reduction <= 100)
        total = (float)(100 - reduction) * total * 0.01f;
    total += items;
    if (total > 32766.0f)
        total = 32766.0f;
    if (total < 0.0f)
        total = 0.0f;
    return (int)total;
}

// FUNCTION: TRILOGY 0x0041be53
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// __thiscall. The race name (race -1 = this character's) copied into the static buffer at 0x00591AAC.
char* EQ_Character::RaceName_guess(short race_id)
{
    if (race_id == -1)
        race_id = race;
    switch (race_id)
    {
    case 1:
        strcpy(g_race_name_buffer_guess, "Human");
        break;
    case 2:
        strcpy(g_race_name_buffer_guess, "Barbarian");
        break;
    case 3:
        strcpy(g_race_name_buffer_guess, "Erudite");
        break;
    case 4:
        strcpy(g_race_name_buffer_guess, "Wood Elf");
        break;
    case 5:
        strcpy(g_race_name_buffer_guess, "High Elf");
        break;
    case 6:
        strcpy(g_race_name_buffer_guess, "Dark Elf");
        break;
    case 7:
        strcpy(g_race_name_buffer_guess, "Half Elf");
        break;
    case 8:
        strcpy(g_race_name_buffer_guess, "Dwarf");
        break;
    case 9:
        strcpy(g_race_name_buffer_guess, "Troll");
        break;
    case 10:
        strcpy(g_race_name_buffer_guess, "Ogre");
        break;
    case 11:
        strcpy(g_race_name_buffer_guess, "Halfling");
        break;
    case 12:
        strcpy(g_race_name_buffer_guess, "Gnome");
        break;
    case 128:
        strcpy(g_race_name_buffer_guess, "Iksar");
        break;
    case 130:
        strcpy(g_race_name_buffer_guess, "Vah Shir");
        break;
    default:
        strcpy(g_race_name_buffer_guess, "Unknown");
        break;
    }
    return g_race_name_buffer_guess;
}

// FUNCTION: TRILOGY 0x0041bf2e
// Supplies the class label shown in Who results, resolving the self-class shorthand.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character.cpp (address range)
// __thiscall. The class name (0xFFFF = this character's class). Player classes 1..15 and unknown values
// are copied into the static buffer at 0x00591A8C; the NPC classes 16..32 (Banker, the GMs, Merchant)
// return the literal itself.
char* EQ_Character::ClassName_guess(u16 class_id)
{
    if (class_id == 0xFFFF)
        class_id = class_;
    switch ((short)class_id)
    {
    case 1:
        strcpy(g_class_name_buffer_guess, "Warrior");
        break;
    case 2:
        strcpy(g_class_name_buffer_guess, "Cleric");
        break;
    case 3:
        strcpy(g_class_name_buffer_guess, "Paladin");
        break;
    case 5:
        strcpy(g_class_name_buffer_guess, "Shadow Knight");
        break;
    case 4:
        strcpy(g_class_name_buffer_guess, "Ranger");
        break;
    case 6:
        strcpy(g_class_name_buffer_guess, "Druid");
        break;
    case 7:
        strcpy(g_class_name_buffer_guess, "Monk");
        break;
    case 8:
        strcpy(g_class_name_buffer_guess, "Bard");
        break;
    case 9:
        strcpy(g_class_name_buffer_guess, "Rogue");
        break;
    case 10:
        strcpy(g_class_name_buffer_guess, "Shaman");
        break;
    case 11:
        strcpy(g_class_name_buffer_guess, "Necromancer");
        break;
    case 12:
        strcpy(g_class_name_buffer_guess, "Wizard");
        break;
    case 13:
        strcpy(g_class_name_buffer_guess, "Magician");
        break;
    case 14:
        strcpy(g_class_name_buffer_guess, "Enchanter");
        break;
    case 15:
        strcpy(g_class_name_buffer_guess, "Beastlord");
        break;
    case 32:
        return "Merchant";
    case 16:
        return "Banker";
    case 17:
        return "Warrior GM";
    case 18:
        return "Cleric GM";
    case 19:
        return "Paladin GM";
    case 21:
        return "Shadowknight GM";
    case 20:
        return "Ranger GM";
    case 22:
        return "Druid GM";
    case 23:
        return "Monk GM";
    case 24:
        return "Bard GM";
    case 25:
        return "Rogue GM";
    case 26:
        return "Shaman GM";
    case 27:
        return "Necromancer GM";
    case 28:
        return "Wizard GM";
    case 29:
        return "Magician GM";
    case 30:
        return "Enchanter GM";
    case 31:
        return "Beastlord GM";
    default:
        strcpy(g_class_name_buffer_guess, "Unknown");
        break;
    }
    return g_class_name_buffer_guess;
}

class EQ_CharacterTitle_guess
{
public:
    char* ClassTitle_guess(short class_id, short level, u32 unused);
};

// FUNCTION: TRILOGY 0x0041C0F7
// Class name as /who shows it: the plain name below level 51, else a per-level-band name (51-54, 55-59, 60+).
char* EQ_CharacterTitle_guess::ClassTitle_guess(short class_id, short level, u32 unused)
{
    if (class_id == -1)
    {
        level = *(u8*)((u8*)this + 0x38);
        class_id = *(u8*)((u8*)this + 0x36);
    }

    if (level < 0x33)
    {
        switch (class_id)
        {
        case 1:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054046C));
            break;
        case 2:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540464));
            break;
        case 3:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054045C));
            break;
        case 5:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054044C));
            break;
        case 4:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540444));
            break;
        case 6:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054043C));
            break;
        case 7:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540434));
            break;
        case 8:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054042C));
            break;
        case 9:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540424));
            break;
        case 10:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054041C));
            break;
        case 11:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540410));
            break;
        case 12:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540408));
            break;
        case 13:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005403FC));
            break;
        case 14:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005403F0));
            break;
        case 15:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005403E4));
            break;
        case 32:
            return EQ_ARRAY(char, 0x005403D8);
        case 16:
            return EQ_ARRAY(char, 0x005403D0);
        case 17:
            return EQ_ARRAY(char, 0x005403C4);
        case 18:
            return EQ_ARRAY(char, 0x005403B8);
        case 19:
            return EQ_ARRAY(char, 0x005403AC);
        case 21:
            return EQ_ARRAY(char, 0x0054039C);
        case 20:
            return EQ_ARRAY(char, 0x00540390);
        case 22:
            return EQ_ARRAY(char, 0x00540384);
        case 23:
            return EQ_ARRAY(char, 0x0054037C);
        case 24:
            return EQ_ARRAY(char, 0x00540374);
        case 25:
            return EQ_ARRAY(char, 0x00540368);
        case 26:
            return EQ_ARRAY(char, 0x0054035C);
        case 27:
            return EQ_ARRAY(char, 0x0054034C);
        case 28:
            return EQ_ARRAY(char, 0x00540340);
        case 29:
            return EQ_ARRAY(char, 0x00540334);
        case 30:
            return EQ_ARRAY(char, 0x00540324);
        case 31:
            return EQ_ARRAY(char, 0x00540314);
        default:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540280));
            break;
        }
    }
    else
    {
        switch (class_id)
        {
        case 1:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540708));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406FC));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406F4));
            break;
        case 2:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406EC));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406E4));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406D8));
            break;
        case 3:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406CC));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406C4));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406B8));
            break;
        case 5:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406B0));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005406A4));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540698));
            break;
        case 4:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054068C));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540680));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540678));
            break;
        case 6:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054066C));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540660));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540654));
            break;
        case 7:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540648));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540640));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540634));
            break;
        case 8:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540628));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054061C));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540610));
            break;
        case 9:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540608));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405FC));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405F0));
            break;
        case 10:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405E8));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405DC));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405D4));
            break;
        case 11:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405CC));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405C4));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405BC));
            break;
        case 12:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405B0));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x005405A8));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054059C));
            break;
        case 13:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054058C));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540580));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540574));
            break;
        case 14:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540568));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x0054055C));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540550));
            break;
        case 15:
            if (level < 0x37)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540544));
            else if (level < 0x3c)
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540538));
            else
                SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540528));
            break;
        default:
            SOC_CPY(EQ_ARRAY(char, 0x00591D48), EQ_ARRAY(char, 0x00540280));
            break;
        }
    }

    return EQ_ARRAY(char, 0x00591D48);
}

// FUNCTION: TRILOGY 0x0041c5aa
// Supplies the class-based starting stat adjustment during character creation.
// ORIGINAL FILE: EverQuest/EQ_Character.cpp (from Mac debug map)
u8 EQ_Character_guess::BaseAttr_guess(short classId, u32 selectedRace, u32 attributeId)
{
    u32 result;
    char* msg;

    if (classId < 0 || classId > 0xe)
    {
        result = classId;
        msg = (char*)0x00540714;
        EQ_CDECL(void, 0x004A3D01, (char*, u32))(msg, result);
        DbgLog_guess(msg, result);
        return 0;
    }
    else if ((short)selectedRace < 0 || ((short)selectedRace > 0xb && (short)selectedRace != 0x7f && (short)selectedRace != 0x81))
    {
        result = (short)selectedRace;
        msg = (char*)0x00540740;
        EQ_CDECL(void, 0x004A3D01, (char*, u32))(msg, result);
        DbgLog_guess(msg, result);
        return 0;
    }
    else if ((u8)attributeId <= 6)
    {
        int min = EQ_STDCALL(u8, 0x0041CB5F, (u32, u32))(selectedRace, attributeId);
        short entry = g_creation_values_guess[(u8)attributeId][classId];
        if (entry != 0)
        {
            int sum = entry + min;
            if (sum > min)
            {
                u8 diff = (u8)sum - (u8)min;
                return diff;
            }
        }
        return 0;
    }
    else
    {
        result = (u8)attributeId;
        msg = (char*)0x0054076c;
        EQ_CDECL(void, 0x004A3D01, (char*, u32))(msg, result);
        DbgLog_guess(msg, result);
        return 0;
    }
}
