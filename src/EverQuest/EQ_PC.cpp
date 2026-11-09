// EQ_PC.cpp - EQ_PC, the local player's character (an EQ_Character with the client-side extras: fatigue,
// air supply, party, starting gear).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (from Mac debug map)

#include <string.h>

#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/player.h"
#include "trilogy/character.h"
#include "trilogy/eq_character.h"
#include "trilogy/globals.h"
#include "trilogy/layout/eq_charcreate.h"
#include "trilogy/layout/eq_main.h"

class EQ_PC_base_guess : public CharacterProfile   // EQ_PC adds no data: the profile is the whole object
{
public:
    EQ_PC_base_guess()
    {
        EQ_METHOD(0, void, 0x004196D4, (void*), (void*)this)();
    }
    ~EQ_PC_base_guess()
    {
        EQ_METHOD(0, void, 0x004197FF, (void*), (void*)this)();
    }
};

class EQ_PC_guess : public EQ_PC_base_guess
{
public:
    EQ_PC_guess(u32* source);
    ~EQ_PC_guess();
    u8 MaxAirSupply_guess(void);
    void ProcessAirSupply(void);
    int GetExpForLevel_guess(u8 param_1, int param_2, int param_3);
    bool TryRemoveBuff_guess(short param_2);
    u32 ApplyDeathExperienceLoss_guess(int param_2);
    bool IsSkillAtMax_guess(short param_2);
};

// FUNCTION: TRILOGY 0x0041ba4e
// Determines the player's maximum air supply from stamina, with a race-based exception.
// ORIGINAL FILE: EverQuest/EQ_PC.cpp (from Mac debug map)
u8 EQ_PC_guess::MaxAirSupply_guess(void)
{
    char stamina;
    u8 maxAir;

    stamina = EQ_METHOD(0, char, 0x0041B258, (EQ_Character*), (EQ_Character*)this)();
    maxAir = stamina - 0x1e;
    if (*(short*)((char*)this + 0x34) == 0x80)
        return 0x7f;
    if (maxAir < 10)
        return 10;
    if (100 < maxAir)
        maxAir = 100;
    return maxAir;
}

struct AirDamage_guess
{
    u32 spawn_id;
    u32 unknown4;
    u16 unknown8;
    u8 padA[2];
    int damage;
    f32 x;
    f32 y;
    f32 z;
    u8 type;
    u8 pad1D[3];
    u16 unknown20;
    u8 pad22;
    u8 unknown23;
};

#define AIR_SUPPLY_OFFSET 0xa45
#define DROWN_DAMAGE_TYPE 0xfb

// FUNCTION: TRILOGY 0x00425867
// ORIGINAL FILE: EverQuest/EQ_PC.cpp (from Mac debug map)
// __thiscall, void. Refill the air supply when breathing, else count it down and drown for a share of max HP.
void EQ_PC_guess::ProcessAirSupply(void)
{
    AirDamage_guess info;
    EQPlayer* me;
    s16 damage;

    info.unknown23 = 0;
    me = this->player;
    if (me == 0)
    {
        EQTRACE_guess("me is NULL in ProcessAirSupply()\n");
    }
    else if (me->type == 0)
    {
        if (EQ_METHOD(0, char, 0x00425817, (void*), (void*)this)() == 1)
        {
            *((char*)this + AIR_SUPPLY_OFFSET) = EQ_METHOD(0, u8, 0x0041BA4E, (void*), (void*)this)();
        }
        else if (*((char*)this + AIR_SUPPLY_OFFSET) < 1)
        {
            *((char*)this + AIR_SUPPLY_OFFSET) = 0;
            damage = (s16)(EQ_METHOD(1, int, 0x00419A2F, (void*, int), (void*)this)(
                               (this->player)->type) *
                           EQ_GLOBAL(double, 0x00535380));
            if (0 < damage)
            {
                info.unknown20 = 0xffff;
                info.x = 0.0f;
                info.spawn_id = (this->player)->spawn_id;
                info.y = 0.0f;
                info.z = 0.0f;
                info.unknown4 = 0;
                info.unknown8 = 0;
                info.damage = damage;
                info.type = DROWN_DAMAGE_TYPE;
                EQ_METHOD(4, void, 0x004247F0, (void*, int, AirDamage_guess*, int, int), (void*)this)(
                    damage, &info, 1, -1);
            }
        }
        else
        {
            *((char*)this + AIR_SUPPLY_OFFSET) = *((char*)this + AIR_SUPPLY_OFFSET) - 1;
        }
    }
}

// FUNCTION: TRILOGY 0x004501a1
// Performs no skill improvement in this build.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (Mac: EQ_PC::CheckSkillImprove, low)
// __thiscall, 2 args, empty in this build (a bare `ret 8`; the linker also uses this body for a dozen
// vtable slots at 0x005353E4..0x0053547C).
void EQ_PC::CheckSkillImprove_guess(int skill, int unused)
{
}

// FUNCTION: TRILOGY 0x0047eb8e
// Builds the local player from a copied profile or as a new level-1 race 1, class 2 character, all stats 50.
// ORIGINAL FILE: EverQuest/EQ_PC.cpp (from Mac debug map)
// @early-stop: EH frame, fastcall EDX shims, and some stack/register scheduling still differ.
// __thiscall, 1 arg (`ret 4`); the constructor returns this. The base EQ_Character is built first
// (EQ_PC_base_guess). source == 0 makes a new character; else the 0x1FA4-byte profile at source is copied
// in and its name, inventory and bank pointers are cleaned up. Both paths end in the same tail.
EQ_PC_guess::EQ_PC_guess(u32* source)
{
    u8* p;
    u32* dst;
    s16* w;
    EQ_Item** item_slot;
    EQ_ItemProperties* props;
    int i;
    int j;

    *(u8*)((u8*)this + 0x1046) = 0x64;
    *(u16*)((u8*)this + 0x1038) = 0x018c;
    *(u8*)this = 0;
    this->primary_model = 0;
    this->secondary_model = 0;
    this->unk_0x11B0 = 0;
    this->unk_0x11B4 = 0;
    this->unk_0x11B5 = 0;
    this->appearanceFeature4_guess |= 0xff;
    this->appearanceFeature5_guess |= 0xff;
    this->appearanceFeature0_guess |= 0xff;
    this->appearanceFeature1_guess |= 0xff;
    this->appearanceFeature2_guess |= 0xff;
    this->appearanceFeature3_guess |= 0xff;
    p = (u8*)this + 0x1214;
    for (i = 0x16; i != 0; i--)
    {
        *(u32*)(p - 0x58) = 0;
        *(u32*)p = 0;
        p += 4;
    }
    if (source != 0)
    {
        *(CharacterProfile*)this = *(CharacterProfile*)source;
        *(u32*)((u8*)this + 0xaa8) = 0;
        this->unk_0xAA4 = 0;
        *(u8*)((u8*)this + 0x10) = 0;
        if (EQ_CDECL(int, 0x00522EFD, (int))(*(char*)this) != 0)
        {
            *(char*)this = (char)EQ_CDECL(int, 0x005238FD, (int))(*(char*)this);
        }
        if (*(char*)this < 'A' || 'Z' < *(char*)this)
        {
            *(char*)this = 'A';
        }
        item_slot = &this->inventory_items[0];
        for (i = 0x1e; i != 0; i--)
        {
            *item_slot = 0;
            item_slot++;
        }
        item_slot = &this->bank_items[0];
        for (i = 8; i != 0; i--)
        {
            *item_slot = 0;
            item_slot++;
        }
        this->unk_0xF64 = 0;
        this->unk_0xF6C = 0;
        this->unk_0xF68 = 0;
        *(u32*)((u8*)this + 0xa0) = 0;
    }
    else
    {
        EQ_METHOD(0, void, 0x004803EF, (void*), (void*)this)();
        dst = &this->spell_slot_refresh[0];
        for (i = 10; i != 0; i--)
        {
            *dst = 0;
            dst++;
        }
        props = &this->bank_props[0];
        w = &this->bank_inventory[0];
        item_slot = &this->bank_items[0];
        for (i = 8; i != 0; i--)
        {
            *item_slot = 0;
            *w = -1;
            item_slot++;
            props->unknown0x00 = 0;
            w++;
            props++;
        }
        w = &this->bank_bag_inventory[0];
        props = &this->bank_bag_props[0];
        for (i = 8; i != 0; i--)
        {
            for (j = 10; j != 0; j--)
            {
                *w = -1;
                props->unknown0x00 = 0;
                w++;
                props++;
            }
        }
        this->pvp = 0;
        this->guild_id = -1;
        *(u8*)((u8*)this + 0x104b) |= 0xff;
        this->intoxication = 0;
        this->CHA = 0x32;
        this->WIS = 0x32;
        this->INT = 0x32;
        this->STA = 0x32;
        this->AGI = 0x32;
        this->DEX = 0x32;
        this->STR = 0x32;
        this->level = 1;
        this->unknown0x0A46 = 0;
        this->highest_level = 1;
        this->face = 0;
        i = 0;
        dst = &this->equip_colors[0];
        do
        {
            *((u8*)this + 0x45 + i) = 0;
            *dst = 0;
            i++;
            dst++;
        } while (i < 9);
        this->exp = 0;
        this->training_points = 0;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)this + 0x1e, (const char*)0x0058fba4);
        *(u8*)((u8*)this + 0xa88) = 0;
        this->unknown0x0076 = 0;
        this->y = 0.0f;
        this->x = 0.0f;
        this->z = 9.0f;
        this->heading = 0.0f;
        p = (u8*)this + 0xef4;
        for (i = 5; i != 0; i--)
        {
            *(f32*)(p + 0x14) = 0.0f;
            *(f32*)p = 0.0f;
            *(f32*)(p + 0x28) = 9.0f;
            p += 4;
        }
        *(u32*)((u8*)this + 0x9a8) = 0;
        *(u32*)((u8*)this + 0x9b8) = 0;
        *(u32*)((u8*)this + 0x998) = 0;
        *(u32*)((u8*)this + 0x9ac) = 0;
        *(u32*)((u8*)this + 0x9bc) = 0;
        *(u32*)((u8*)this + 0x99c) = 0;
        *(u32*)((u8*)this + 0x9b0) = 0;
        *(u32*)((u8*)this + 0x9c0) = 0;
        *(u32*)((u8*)this + 0x9a0) = 0;
        *(u32*)((u8*)this + 0x9b4) = 0;
        *(u32*)((u8*)this + 0x9c4) = 0;
        *(u32*)((u8*)this + 0x9a4) = 0;
        this->unknown0x1040 = 0;
        EQ_GLOBAL(u32, 0x006B644C) = 0;
        *(volatile u32*)((u8*)this + 0x103c) = EQ_CDECL(u32, 0x005213E9, (void*))(0);
        *(u32*)((u8*)this + 0x998) = 0;
        *(u32*)((u8*)this + 0x99c) = 0;
        this->stand_state = 0x64;
        this->anon = 0;
        this->gm_flag = 0;
        this->gender = 0;
        this->class_ = 2;
        this->race = 1;
        EQ_METHOD(2, void, 0x0041CB8B, (void*, int, int), (void*)this)(2, 1);
        EQ_METHOD(2, void, 0x0041D17E, (void*, u16, u16), (void*)this)(this->race, this->class_);
        this->unknown0x0076 = 0;
        dst = (u32*)((u8*)this + 0x752);
        for (i = 0x80; i != 0; i--)
        {
            *dst = 0xffffffff;
            dst++;
        }
        dst = &this->spell_memorized_pairs_guess[0];
        for (i = 0; i < 4; i++)
        {
            *dst++ = 0xffffffff;
        }
        this->mana = (s16)EQ_METHOD(0, int, 0x00419BCA, (void*), (void*)this)();
        p = (u8*)this + 0x28a;
        for (i = 0xf; i != 0; i--)
        {
            *(u8*)(p - 6) = 0;
            *(u16*)p = 0;
            *(u8*)(p - 4) = 0;
            *(s16*)(p - 2) = -1;
            *(u8*)(p - 5) = 0;
            *(u16*)(p + 2) = 0;
            p += 10;
        }
        EQ_METHOD(1, void, 0x0041997D, (void*, int), (void*)this)(0);
    }
    this->unk_0xF64 = 0;
    this->time1 = EQ_CDECL(u32, 0x005213E9, (void*))(0);
    this->unk_0xF7C = 1;
    EQ_METHOD(0, void, 0x0041A4FC, (void*), (void*)this)();
    this->unk_0xF6C = 0;
    this->unk_0xF68 = 0;
    this->unk_0xF86 = 0;
    this->unk_0xF84 = 0;
    this->unk_0xF82 = 0;
    this->unk_0xF80 = 0;
    this->unk_0xF78 = 0;
    this->unk_0xF74 = 0;
}

// FUNCTION: TRILOGY 0x0047ef27
// Releases the local character's worn, carried and bank items as the character profile is discarded.
// ORIGINAL FILE: EverQuest/EQ_PC.cpp (from Mac debug map)
// Frees the items in the 30 inventory slots (0xE0) and the 8 slots at 0xF44; a container's dtor runs first, then operator delete.
/* fake match: the container dtor goes through a pointer and case 1 re-reads the slot for its null test; both are shape only. */
// @early-stop: no `or [ebp-4], -1` before the base dtor call: the base is an inline stand-in whose body is nothrow to VC6.
EQ_PC_guess::~EQ_PC_guess()
{
    EQ_Item** item_slot;
    EQ_Item* item;
    int count;
    /* fake match: a call through a pointer is possibly-throwing to VC6, a call to a fixed address is not; this gives the EH frame. */
    void (__fastcall *dtor)(void*, int);

    item_slot = &this->inventory_items[0];
    count = 0x1e;
    do
    {
        item = *item_slot;
        if (item != 0)
        {
            switch (item->type)
            {
            case 1:
                /* fake match: VC6 folds a null test on `item` (the outer test dominates it) but keeps one on the slot. */
                if (*item_slot == 0)
                    break;
                dtor = EQ_THISCALL(void, 0x0043405A, (void*, int));
                dtor(item, 0);
                /* fall through */
            default:
                EQ_CDECL(void, 0x005218BB, (void*))(item);
            }
        }
        item_slot++;
        count--;
    } while (count != 0);

    EQ_METHOD(0, void, 0x004803EF, (void*), this)();

    item_slot = &this->bank_items[0];
    count = 8;
    do
    {
        item = *item_slot;
        if (item != 0)
        {
            switch (item->type)
            {
            case 1:
                /* fake match: VC6 folds a null test on `item` (the outer test dominates it) but keeps one on the slot. */
                if (*item_slot == 0)
                    break;
                dtor = EQ_THISCALL(void, 0x0043405A, (void*, int));
                dtor(item, 0);
                /* fall through */
            default:
                EQ_CDECL(void, 0x005218BB, (void*))(item);
            }
        }
        item_slot++;
        count--;
    } while (count != 0);
}

// FUNCTION: TRILOGY 0x0047efce
// Recognizes whether a supplied character name belongs to the local player or one of their group members.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (Mac: EQ_PC::IsAGroupMember, low)
// __thiscall. 1 when `name` (ignoring case) is this character or one of the 6 group member names. Values
// below 0x10 are treated as no name.
int EQ_PC::IsAGroupMember(char* member)
{
    int i;

    if ((u32)member >= 0x10 && *member != '\0')
    {
        if (_strcmpi(member, name) == 0)
            return 1;
        for (i = 0; i < 6; i++)
        {
            if (_strcmpi(member, group_members[i]) == 0)
                return 1;
        }
    }
    return 0;
}

#define EQ_PC_COPY_GUESS(dest, src) \
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)(dest), (const char*)(src))
#define EQ_PC_STRNICMP_GUESS(left, right, count) \
    EQ_CDECL(int, 0x005316C0, (const char*, const char*, unsigned int))((left), (right), (count))
#define EQ_PC_STRNCMP_GUESS(left, right, count) \
    EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))((left), (right), (count))
#define EQ_PC_STRCMP_GUESS(left, right) \
    EQ_CDECL(int, 0x00522920, (const char*, const char*))((left), (right))
#define EQ_PC_STRICMP_GUESS(left, right) \
    EQ_CDECL(int, 0x00524240, (const char*, const char*))((left), (right))

// FUNCTION: TRILOGY 0x0047f019
// __fastcall on the profile (pc, edx, raceId, classId); sets the start zone and the five bind points.
// Writes the start zone name at +0x974 and float coordinates at +0x964/+0x968/+0x96c, chosen by race and class
// through a chain of race tests, many with a nested class test.
// At setBind the start zone name picks the bind zone name at +0xb18 and bind coordinates at +0xef4/+0xf08/+0xf1c.
// The tail copies that zone and those coordinates into the other four bind slots, stepping 0x14 bytes per slot.
void __fastcall SetStartZoneAndBind_guess(void* pc, int edx, short raceId, char classId)
{
    char* base = (char*)pc;
    char* _Str1;
    u32 coordValue;
    int tmp;
    int bindZoneDst;
    u32* bindCoord;
    bool isMatch;
    int remaining;

    *(u32*)(base + 0x970) = 0;
    _Str1 = (char*)(base + 0x974);
    *(u32*)(base + 0x964) = 0;
    *(u32*)(base + 0x96c) = 0x41100000;
    *(u32*)(base + 0x968) = 0;
    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
    if (raceId == 0x82)
    {
        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3DC);
        *(u32*)(base + 0x964) = 0xc50e8000;
        *(u32*)(base + 0x968) = 0xc54dc000;
setCoordNeg53:
        *(u32*)(base + 0x96c) = 0xc2540000;
        goto setBind;
    }
    if (raceId == 0xb)
    {
        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0053E470);
        coordValue = 0x40800000;
        *(u32*)(base + 0x96c) = 0x40800000;
        *(u32*)(base + 0x964) = 0xc2ac0000;
        *(u32*)(base + 0x968) = 0xc34b0000;
        if (classId == '\t')
        {
            *(u32*)(base + 0x964) = 0x43030000;
            *(u32*)(base + 0x968) = 0xc2400000;
setCoordNeg7:
            *(u32*)(base + 0x96c) = 0xc0e00000;
            goto setBind;
        }
        if (classId == '\x02')
        {
            *(u32*)(base + 0x964) = 0x43600000;
            *(u32*)(base + 0x968) = 0xc38c8000;
        }
        else
        {
            if (classId != '\x06')
                goto setBind;
            *(u32*)(base + 0x964) = 0x43800000;
            *(u32*)(base + 0x968) = 0xc4204000;
        }
    }
    else if (raceId == 0xc)
    {
        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0053E468);
        coordValue = 0xc1b00000;
        *(u32*)(base + 0x96c) = 0xc1b00000;
        *(u32*)(base + 0x964) = 0x4494e000;
        *(u32*)(base + 0x968) = 0xc4354000;
        if (classId == '\t')
        {
            *(u32*)(base + 0x964) = 0x43ed8000;
            *(u32*)(base + 0x968) = 0xc2580000;
        }
        else if (classId == '\x01')
        {
            *(u32*)(base + 0x964) = 0x44414000;
            *(u32*)(base + 0x968) = 0xc3c98000;
        }
        else
        {
            if (classId == '\x02')
            {
                *(u32*)(base + 0x964) = 0x43420000;
                *(u32*)(base + 0x968) = 0x42580000;
                *(u32*)(base + 0x96c) = 0xc1a00000;
                goto setBind;
            }
            if (classId != '\v')
                goto setBind;
            *(u32*)(base + 0x964) = 0x44a18000;
            *(u32*)(base + 0x968) = 0xc42ec000;
        }
    }
    else
    {
        if (raceId == 4)
        {
            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3D0);
            *(u32*)(base + 0x964) = 0x43e28000;
            *(u32*)(base + 0x968) = 0x43a08000;
            *(u32*)(base + 0x96c) = 0x42a00000;
            if (classId == '\x06')
            {
gfaydarkClass6:
                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3D0);
                *(u32*)(base + 0x964) = 0xc3e20000;
                *(u32*)(base + 0x968) = 0x434a0000;
            }
            else
            {
                if (classId == '\x04')
                {
gfaydarkClass4:
                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3D0);
                    *(u32*)(base + 0x964) = 0xc3918000;
                    *(u32*)(base + 0x968) = 0x43b88000;
                    *(u32*)(base + 0x96c) = 0x42f00000;
                    goto setBind;
                }
                if (classId == '\x01')
                {
gfaydarkClass1:
                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3D0);
                    *(u32*)(base + 0x964) = 0x43e28000;
                    *(u32*)(base + 0x968) = 0x43a08000;
                }
                else
                {
                    if (classId == '\t')
                    {
gfaydarkClass9:
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3D0);
                        *(u32*)(base + 0x964) = 0xc3e90000;
                        *(u32*)(base + 0x968) = 0xc3980000;
                        *(u32*)(base + 0x96c) = 0x43240000;
                        goto setBind;
                    }
                    isMatch = classId == '\b';
gfaydarkClass8Check:
                    if (!isMatch)
                        goto setBind;
                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3D0);
                    *(u32*)(base + 0x964) = 0xc2d80000;
                    *(u32*)(base + 0x968) = 0x430b0000;
                }
            }
            *(u32*)(base + 0x96c) = 0x42a00000;
            goto setBind;
        }
        if (raceId == 8)
        {
            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3C4);
            coordValue = 0x40a00000;
            *(u32*)(base + 0x96c) = 0x40a00000;
            *(u32*)(base + 0x964) = 0x41100000;
            *(u32*)(base + 0x968) = 0xc0400000;
            if (classId == '\t')
            {
                *(u32*)(base + 0x964) = 0x43420000;
                *(u32*)(base + 0x968) = 0xc3230000;
            }
            else
            {
                if (classId == '\x02')
                {
                    *(u32*)(base + 0x964) = 0x42740000;
                    *(u32*)(base + 0x968) = 0x43998000;
                    *(u32*)(base + 0x96c) = 0x41880000;
                    goto setBind;
                }
                if (classId != '\x03')
                    goto setBind;
                *(u32*)(base + 0x964) = 0x42d00000;
                *(u32*)(base + 0x968) = 0x43b40000;
            }
        }
        else if (raceId == 10)
        {
            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3BC);
            coordValue = 0x40800000;
            *(u32*)(base + 0x96c) = 0x40800000;
            *(u32*)(base + 0x964) = 0x43c08000;
            *(u32*)(base + 0x968) = 0x441b0000;
            if ((classId == '\n') || (classId == '\x0f'))
            {
                *(u32*)(base + 0x964) = 0xc34a0000;
                *(u32*)(base + 0x968) = 0x43af8000;
                *(u32*)(base + 0x96c) = 0x428a0000;
                goto setBind;
            }
            if (classId != '\x01')
                goto setBind;
            *(u32*)(base + 0x964) = 0x432f0000;
            *(u32*)(base + 0x968) = 0xc38a0000;
        }
        else
        {
            if (raceId == 9)
            {
                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3B4);
                *(u32*)(base + 0x964) = 0x43ad8000;
                *(u32*)(base + 0x968) = 0xc2860000;
                *(u32*)(base + 0x96c) = 0x40c00000;
                if ((classId == '\n') || (classId == '\x0f'))
                {
                    *(u32*)(base + 0x964) = 0x42fc0000;
                    *(u32*)(base + 0x968) = 0xc2380000;
                    *(u32*)(base + 0x96c) = 0x40800000;
                }
                else if (classId == '\x01')
                {
                    *(u32*)(base + 0x964) = 0x43490000;
                    *(u32*)(base + 0x968) = 0xc3a18000;
                    *(u32*)(base + 0x96c) = 0x41100000;
                }
                goto setBind;
            }
            if (raceId == 6)
            {
                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3AC);
                coordValue = 0xc21c0000;
                *(u32*)(base + 0x96c) = 0xc21c0000;
                *(u32*)(base + 0x964) = 0xc2fa0000;
                *(u32*)(base + 0x968) = 0xc4620000;
                if (classId == '\r')
                {
                    *(u32*)(base + 0x964) = 0x42100000;
                    *(u32*)(base + 0x968) = 0xc47c4000;
                }
                else
                {
                    if (classId != '\f')
                    {
                        if (classId == '\x01')
                        {
                            *(u32*)(base + 0x964) = 0xc1a80000;
                            *(u32*)(base + 0x968) = 0xc4608000;
                            *(u32*)(base + 0x96c) = 0xc1300000;
                            goto setBind;
                        }
                        if (classId == '\x02')
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3A4);
                            *(u32*)(base + 0x964) = 0x4440c000;
                            *(u32*)(base + 0x968) = 0xc4b68000;
                            *(u32*)(base + 0x96c) = 0xc2a20000;
                            goto setBind;
                        }
                        if (classId == '\x05')
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3A4);
                            *(u32*)(base + 0x964) = 0x446dc000;
                            *(u32*)(base + 0x968) = 0xc4bba000;
setCoordNeg80:
                            *(u32*)(base + 0x96c) = 0xc2a00000;
                            goto setBind;
                        }
                        if (classId != '\v')
                        {
                            if (classId == '\t')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3A4);
                                *(u32*)(base + 0x964) = 0x44660000;
                                *(u32*)(base + 0x968) = 0xc4d7a000;
                                *(u32*)(base + 0x96c) = 0xc2860000;
                            }
                            goto setBind;
                        }
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3A4);
                        *(u32*)(base + 0x964) = 0x443c8000;
                        *(u32*)(base + 0x968) = 0xc4604000;
                        goto setCoordNeg53;
                    }
                    *(u32*)(base + 0x964) = 0x43010000;
                    *(u32*)(base + 0x968) = 0xc4508000;
                }
            }
            else
            {
                if (raceId != 2)
                {
                    if (raceId == 5)
                    {
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C390);
                        *(u32*)(base + 0x964) = 0x41b80000;
                        *(u32*)(base + 0x968) = 0x41500000;
                        *(u32*)(base + 0x96c) = 0x40a00000;
                        if (classId == '\x02')
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C390);
                            *(u32*)(base + 0x964) = 0x41b80000;
                            *(u32*)(base + 0x968) = 0x41500000;
                            *(u32*)(base + 0x96c) = 0x40a00000;
                        }
                        if (classId == '\x03')
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C390);
                            *(u32*)(base + 0x964) = 0x41b80000;
                            *(u32*)(base + 0x968) = 0x41500000;
                            *(u32*)(base + 0x96c) = 0x40a00000;
                        }
                        if (classId == '\r')
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C390);
                            *(u32*)(base + 0x964) = 0x42c60000;
                            *(u32*)(base + 0x968) = 0xc39b0000;
                            *(u32*)(base + 0x96c) = 0x40a00000;
                        }
                        if (classId == '\x0e')
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C390);
                            *(u32*)(base + 0x964) = 0x42c60000;
                            *(u32*)(base + 0x968) = 0xc39b0000;
                            *(u32*)(base + 0x96c) = 0x40a00000;
                        }
                        if (classId != '\f')
                            goto setBind;
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C390);
                        *(u32*)(base + 0x964) = 0x42c60000;
                        *(u32*)(base + 0x968) = 0xc39b0000;
                        goto setCoord5;
                    }
                    if (raceId == 3)
                    {
                        if (classId == '\x02')
                        {
                            if (*(short*)(base + 0x1038) == 0xcb)
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C388);
                                *(u32*)(base + 0x964) = 0x44864000;
                                *(u32*)(base + 0x968) = 0x442b0000;
                                *(u32*)(base + 0x96c) = 0xc2180000;
                                goto setBind;
                            }
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C37C);
                            *(u32*)(base + 0x964) = 0x434e0000;
                            *(u32*)(base + 0x968) = 0xc2200000;
                        }
                        else
                        {
                            if (classId == '\x05')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C388);
                                *(u32*)(base + 0x964) = 0x4456c000;
                                *(u32*)(base + 0x968) = 0x44070000;
                                *(u32*)(base + 0x96c) = 0xc2f40000;
                                goto setBind;
                            }
                            if (classId != '\x03')
                            {
                                if (classId == '\r')
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C37C);
                                    *(u32*)(base + 0x964) = 0xc43d4000;
                                    *(u32*)(base + 0x968) = 0xc3790000;
                                }
                                else
                                {
                                    if (classId != '\x0e')
                                    {
                                        if (classId == '\f')
                                        {
                                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C37C);
                                            *(u32*)(base + 0x964) = 0xc4628000;
                                            *(u32*)(base + 0x968) = 0xc3410000;
                                            *(u32*)(base + 0x96c) = 0x42540000;
                                            goto setBind;
                                        }
                                        if (classId != '\v')
                                            goto setBind;
                                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C388);
                                        *(u32*)(base + 0x964) = 0x44748000;
                                        *(u32*)(base + 0x968) = 0x440f4000;
                                        goto setCoordNeg80;
                                    }
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C37C);
                                    *(u32*)(base + 0x964) = 0xc43c8000;
                                    *(u32*)(base + 0x968) = 0xc2e20000;
                                }
                                *(u32*)(base + 0x96c) = 0x42580000;
                                goto setBind;
                            }
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C37C);
                            *(u32*)(base + 0x964) = 0x434f0000;
                            *(u32*)(base + 0x968) = 0xc3a08000;
                        }
                        *(u32*)(base + 0x96c) = 0x41a80000;
                        goto setBind;
                    }
                    if ((raceId == 1) || (raceId == 7))
                    {
                        if (classId == '\x02')
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                            *(u32*)(base + 0x964) = 0xc1900000;
                            *(u32*)(base + 0x968) = 0x41e80000;
                            *(u32*)(base + 0x96c) = 0x40800000;
                        }
                        else
                        {
                            if (classId == '\x06')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0053E760);
                                *(u32*)(base + 0x964) = 0x42340000;
                                *(u32*)(base + 0x968) = 0x41e80000;
                                *(u32*)(base + 0x96c) = 0x40a00000;
                                if ((pinstEQ_CharCreateS_guess->city != 9) ||
                                    (raceId != 7))
                                    goto setBind;
                                goto gfaydarkClass6;
                            }
                            if (classId == '\x04')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0053E760);
                                *(u32*)(base + 0x964) = 0xc35a0000;
                                *(u32*)(base + 0x968) = 0xc3370000;
                                *(u32*)(base + 0x96c) = 0x40a00000;
                                if ((pinstEQ_CharCreateS_guess->city != 9) ||
                                    (raceId != 7))
                                    goto setBind;
                                goto gfaydarkClass4;
                            }
                            if (classId == '\x03')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                *(u32*)(base + 0x964) = 0x430f0000;
                                *(u32*)(base + 0x968) = 0xc3de8000;
                                *(u32*)(base + 0x96c) = 0x40400000;
                                if (pinstEQ_CharCreateS_guess->city == 4)
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C360);
                                    *(u32*)(base + 0x964) = 0x411ccccd;
                                    *(u32*)(base + 0x968) = 0xc202cccd;
                                    *(u32*)(base + 0x96c) = 0xc1c1999a;
                                }
                                if (pinstEQ_CharCreateS_guess->city != 10)
                                    goto setBind;
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C390);
                                *(u32*)(base + 0x964) = 0x41b80000;
                                *(u32*)(base + 0x968) = 0x41500000;
                                goto setCoord5;
                            }
                            if (classId == '\x01')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                *(u32*)(base + 0x964) = 0xc3920000;
                                *(u32*)(base + 0x968) = 0xc37f0000;
                                *(u32*)(base + 0x96c) = 0x40800000;
                                if (pinstEQ_CharCreateS_guess->city == 4)
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C360);
                                    *(u32*)(base + 0x964) = 0x411ccccd;
                                    *(u32*)(base + 0x968) = 0xc202cccd;
                                    *(u32*)(base + 0x96c) = 0xc1c1999a;
                                }
                                if ((pinstEQ_CharCreateS_guess->city != 9) ||
                                    (raceId != 7))
                                    goto setBind;
                                goto gfaydarkClass1;
                            }
                            if (classId == '\t')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                *(u32*)(base + 0x964) = 0x432d0000;
                                *(u32*)(base + 0x968) = 0xc23c0000;
                                *(u32*)(base + 0x96c) = 0x40400000;
                                if (pinstEQ_CharCreateS_guess->city == 4)
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C360);
                                    *(u32*)(base + 0x964) = 0x411ccccd;
                                    *(u32*)(base + 0x968) = 0xc202cccd;
                                    *(u32*)(base + 0x96c) = 0xc1c1999a;
                                }
                                if ((pinstEQ_CharCreateS_guess->city != 9) ||
                                    (raceId != 7))
                                    goto setBind;
                                goto gfaydarkClass9;
                            }
                            if (classId == '\b')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                *(u32*)(base + 0x964) = 0x43dc8000;
                                *(u32*)(base + 0x968) = 0xc39d0000;
                                *(u32*)(base + 0x96c) = 0x40400000;
                                if (pinstEQ_CharCreateS_guess->city == 4)
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C360);
                                    *(u32*)(base + 0x964) = 0x411ccccd;
                                    *(u32*)(base + 0x968) = 0xc202cccd;
                                    *(u32*)(base + 0x96c) = 0xc1c1999a;
                                }
                                if (pinstEQ_CharCreateS_guess->city != 9)
                                    goto setBind;
                                isMatch = raceId == 7;
                                goto gfaydarkClass8Check;
                            }
                            if (classId == '\r')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                *(u32*)(base + 0x964) = 0xc38c8000;
                                *(u32*)(base + 0x96c) = 0;
                                *(u32*)(base + 0x968) = 0xc26c0000;
                            }
                            else
                            {
                                if (classId == '\x0e')
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                    *(u32*)(base + 0x964) = 0x43c10000;
                                    *(u32*)(base + 0x968) = 0xc3c58000;
                                }
                                else if (classId == '\f')
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                    *(u32*)(base + 0x964) = 0x43ae0000;
                                    *(u32*)(base + 0x968) = 0xc3190000;
                                }
                                else if (classId == '\v')
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                    *(u32*)(base + 0x964) = 0x43730000;
                                    *(u32*)(base + 0x968) = 0xc3970000;
                                }
                                else if (classId == '\x05')
                                {
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                    *(u32*)(base + 0x964) = 0x43b20000;
                                    *(u32*)(base + 0x968) = 0xc3868000;
                                }
                                else
                                {
                                    if ((classId != '\a') && (classId != '\n'))
                                        goto setBind;
                                    EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C3E8);
                                    *(u32*)(base + 0x964) = 0x43bb8000;
                                    *(u32*)(base + 0x968) = 0xc40cc000;
                                }
                                *(u32*)(base + 0x96c) = 0x40400000;
                            }
                        }
                        if (pinstEQ_CharCreateS_guess->city == 4)
                        {
                            EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C360);
                            *(u32*)(base + 0x964) = 0x411ccccd;
                            *(u32*)(base + 0x968) = 0xc202cccd;
                            *(u32*)(base + 0x96c) = 0xc1c1999a;
                        }
                        goto setBind;
                    }
                    if (raceId != 0x80)
                        goto setBind;
                    if (classId == '\x01')
                    {
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C36C);
                        *(u32*)(base + 0x964) = 0xc3ac0000;
                        *(u32*)(base + 0x968) = 0xc2f80000;
                    }
                    else if (classId == '\x05')
                    {
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C36C);
                        *(u32*)(base + 0x964) = 0x445c0000;
                        *(u32*)(base + 0x968) = 0xc3a80000;
                    }
                    else if (classId == '\a')
                    {
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C36C);
                        *(u32*)(base + 0x964) = 0x43d40000;
                        *(u32*)(base + 0x968) = 0xc3210000;
                    }
                    else
                    {
                        if (classId != '\n')
                        {
                            if (classId == '\v')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C374);
                                *(u32*)(base + 0x964) = 0x43120000;
                                *(u32*)(base + 0x968) = 0x44430000;
                                goto setCoord5;
                            }
                            if (classId != '\x0f')
                            {
                                EQ_PC_COPY_GUESS(_Str1, (const char*)0x005417F4);
                                *(u32*)(base + 0x964) = 0;
                                *(u32*)(base + 0x968) = 0;
                                goto setCoordNeg7;
                            }
                        }
                        EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C36C);
                        *(u32*)(base + 0x964) = 0x442ec000;
                        *(u32*)(base + 0x968) = 0xc28c0000;
                    }
setCoord5:
                    *(u32*)(base + 0x96c) = 0x40a00000;
                    goto setBind;
                }
                EQ_PC_COPY_GUESS(_Str1, (const char*)0x0055C39C);
                coordValue = 0x40a00000;
                *(u32*)(base + 0x96c) = 0x40a00000;
                *(u32*)(base + 0x964) = 0xc1f00000;
                *(u32*)(base + 0x968) = 0xc1200000;
                if (classId == '\x01')
                {
                    *(u32*)(base + 0x964) = 0xc1f00000;
                    *(u32*)(base + 0x968) = 0xc1200000;
                }
                else if ((classId == '\n') || (classId == '\x0f'))
                {
                    *(u32*)(base + 0x964) = 0x43170000;
                    *(u32*)(base + 0x968) = 0xc3480000;
                }
                else
                {
                    if (classId != '\t')
                        goto setBind;
                    *(u32*)(base + 0x964) = 0x44204000;
                    *(u32*)(base + 0x968) = 0xc3de0000;
                }
            }
        }
    }
    *(u32*)(base + 0x96c) = coordValue;
setBind:
    tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0055C3E8, 6);
    if (tmp == 0)
    {
        EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C358);
        *(u32*)(base + 0xef4) = 0x44218000;
        *(u32*)(base + 0xf08) = 0xc3030000;
        *(u32*)(base + 0xf1c) = 0x40a00000;
    }
    else
    {
        tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0055C350, 7);
        if (tmp == 0)
        {
            EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C360);
            *(u32*)(base + 0xef4) = 0x43770000;
            *(u32*)(base + 0xf08) = 0x43b38000;
            *(u32*)(base + 0xf1c) = 0xc1b00000;
        }
        else
        {
            tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0055C39C, 5);
            if (tmp == 0)
            {
                EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C344);
                *(u32*)(base + 0xef4) = 0x45443000;
                *(u32*)(base + 0xf08) = 0x441d4000;
                *(u32*)(base + 0xf1c) = 0xc2640000;
            }
            else
            {
                tmp = EQ_PC_STRNCMP_GUESS(_Str1, (const char*)0x0055C3D0, 8);
                if (tmp == 0)
                {
                    EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C3D0);
                    *(u32*)(base + 0xef4) = 0x43400000;
                    *(u32*)(base + 0xf08) = 0x42800000;
                    *(u32*)(base + 0xf1c) = 0x420c0000;
                }
                else
                {
                    tmp = EQ_PC_STRCMP_GUESS(_Str1, (const char*)0x0055C390);
                    if (tmp == 0)
                    {
                        EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C3D0);
                        *(u32*)(base + 0xef4) = 0xc4ff0000;
                        *(u32*)(base + 0xf08) = 0xc5092000;
                        *(u32*)(base + 0xf1c) = 0x425c0000;
                    }
                    else
                    {
                        tmp = EQ_PC_STRICMP_GUESS(_Str1, (const char*)0x0055C37C);
                        if (tmp == 0)
                        {
                            EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C340);
                            *(u32*)(base + 0xef4) = 0x450b6000;
                            *(u32*)(base + 0xf08) = 0x42860000;
                            *(u32*)(base + 0xf1c) = 0xc2300000;
                        }
                        else
                        {
                            tmp = EQ_PC_STRICMP_GUESS(_Str1, (const char*)0x0055C388);
                            if (tmp == 0)
                            {
                                EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C388);
                                *(u32*)(base + 0xef4) = 0x44480000;
                                *(u32*)(base + 0xf1c) = 0;
                                *(u32*)(base + 0xf08) = 0x43480000;
                            }
                            else
                            {
                                tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0053E484, 7);
                                if (tmp == 0)
                                {
                                    EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0053DF5C);
                                    *(u32*)(base + 0xef4) = 0x45232000;
                                    *(u32*)(base + 0xf08) = 0xc37b0000;
                                    *(u32*)(base + 0xf1c) = 0x42a80000;
                                }
                                else
                                {
                                    tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0055C338, 6);
                                    if (tmp == 0)
                                    {
                                        EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C32C);
                                        *(u32*)(base + 0xef4) = 0x45000000;
                                        *(u32*)(base + 0xf08) = 0xc441c000;
                                        *(u32*)(base + 0xf1c) = 0x42300000;
                                    }
                                    else
                                    {
                                        tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0053E468, 6);
                                        if (tmp == 0)
                                        {
                                            EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C320);
                                            *(u32*)(base + 0xef4) = 0xc4d08000;
                                            *(u32*)(base + 0xf08) = 0x441f0000;
                                            *(u32*)(base + 0xf1c) = 0xc2d40000;
                                        }
                                        else
                                        {
                                            tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0055C3B4, 5);
                                            if (tmp == 0)
                                            {
                                                EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C314);
                                                *(u32*)(base + 0xef4) = 0xc5168000;
                                                *(u32*)(base + 0xf08) = 0xc3a98000;
                                                *(u32*)(base + 0xf1c) = 0xc1600000;
                                            }
                                            else
                                            {
                                                tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0055C3BC, 5);
                                                if (tmp == 0)
                                                {
                                                    EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C308);
                                                    *(u32*)(base + 0xef4) = 0x4489e000;
                                                    *(u32*)(base + 0xf08) = 0x446d0000;
                                                    *(u32*)(base + 0xf1c) = 0x41f80000;
                                                }
                                                else
                                                {
                                                    tmp = EQ_PC_STRNICMP_GUESS(_Str1, (const char*)0x0053E470, 9);
                                                    if (tmp == 0)
                                                    {
                                                        EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0055C300);
                                                        *(u32*)(base + 0xef4) = 0x43878000;
                                                        *(u32*)(base + 0xf08) = 0xc504e000;
                                                        *(u32*)(base + 0xf1c) = 0xc0400000;
                                                    }
                                                    else
                                                    {
                                                        EQ_PC_COPY_GUESS(base + 0xb18, (const char*)0x0053E754);
                                                        *(u32*)(base + 0xef4) = 0x45979000;
                                                        *(u32*)(base + 0xf08) = 0xc2380000;
                                                        *(u32*)(base + 0xf1c) = 0x40800000;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    tmp = (int)(base + 0xb18);
    if (raceId == 0x80)
    {
        EQ_PC_COPY_GUESS((char*)tmp, (const char*)0x005417F4);
        *(u32*)(base + 0xef4) = 0xc515a000;
        *(u32*)(base + 0xf08) = 0x454ac000;
        *(u32*)(base + 0xf1c) = 0x41400000;
    }
    else if (raceId == 0x82)
    {
        EQ_PC_COPY_GUESS((char*)tmp, (const char*)0x0055C3DC);
        *(u32*)(base + 0xef4) = 0xc50e8000;
        *(u32*)(base + 0xf08) = 0xc54dc000;
        *(u32*)(base + 0xf1c) = 0xc2540000;
    }
    bindCoord = (u32*)(base + 0xf0c);
    bindZoneDst = (int)(base + 0xb2c);
    remaining = 4;
    do
    {
        EQ_PC_COPY_GUESS((char*)bindZoneDst, (char*)tmp);
        bindCoord[-5] = *(u32*)(base + 0xef4);
        *bindCoord = *(u32*)(base + 0xf08);
        bindCoord[5] = *(u32*)(base + 0xf1c);
        bindZoneDst = bindZoneDst + 0x14;
        bindCoord = bindCoord + 1;
        remaining = remaining - 1;
    } while (remaining != 0);
    return;
}

#undef EQ_PC_COPY_GUESS
#undef EQ_PC_STRNICMP_GUESS
#undef EQ_PC_STRNCMP_GUESS
#undef EQ_PC_STRCMP_GUESS
#undef EQ_PC_STRICMP_GUESS

// FUNCTION: TRILOGY 0x00480391
// Scale a signed skill table entry by the selected class; all other classes use the entry as is.
int __stdcall EQ_PC::ScaledSkillValue_guess(u8 index, short class_id)
{
    int value = EQ_ARRAY(short, 0x0055C2BE)[index];
    float scaled = (float)value;

    switch (class_id)
    {
    case 2:
        scaled *= 1.05f;
        break;
    case 9:
        scaled *= 1.2f;
        break;
    case 10:
        scaled *= 1.15f;
        break;
    case 11:
        scaled *= 0.95f;
        break;
    case 128:
        scaled *= 1.2f;
        break;
    }
    return (int)scaled;
}

// FUNCTION: TRILOGY 0x004803ef
// Prepares an empty character inventory, including worn slots, bags, and the cursor bag.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (address range; TAKP: EQ_PC_InitializeInventorySlots)
// __thiscall. Empties the inventory: slots 0..29 (no item, item number -1, first property byte 0), then the
// 9 x 10 container slots (8 bags + the cursor bag; item number -1, first property byte 0).
void EQ_PC::InitializeInventorySlots_guess(void)
{
    int bag;
    s16 i;

    for (i = 0; i < 30; i++)
    {
        inventory_items[i] = 0;
        inventory[i] = -1;
        inv_props[i].unknown0x00 = 0;
    }
    for (bag = 0; bag < 9; bag++)
    {
        for (i = 0; i < 10; i++)
        {
            bag_inventory[bag * 10 + i] = -1;
            bag_props[bag * 10 + i].unknown0x00 = 0;
        }
    }
}

// FUNCTION: TRILOGY 0x00480442
// Resets health, mana, fatigue, and death state as the character returns to standing.
void __fastcall ResetPlayerVitals_guess(CharacterProfile* character, int edx_unused, int keepMana)
{
    s16 maxHp;
    s32 maxMana;

    maxHp = EQ_METHOD(1, s16, 0x00419A2F, (CharacterProfile*, int), character)(0);
    character->cur_hp = maxHp;
    maxMana = EQ_METHOD(0, s32, 0x00419BCA, (CharacterProfile*), character)();
    character->mana = (s16)maxMana;
    character->unknown0x0076 = 0;
    character->fatigue = 0;
    character->stand_state = 'd';
    if (keepMana == 0)
        character->mana = 0;
    EQ_GLOBAL(u8, 0x006B64D0) = 0;
    EQ_GLOBAL(u32, 0x006B64C8) = 0;
    EQ_GLOBAL(u32, 0x006B64CC) = 0;
}

// FUNCTION: TRILOGY 0x0048048e
// Processes death by moving the character to their bind point and resetting local profile state.
void __fastcall SendPlayerDeath_guess(CharacterProfile* pc, int edx_unused, int* ptr)
{
    u32 angle;
    u8* buffSlot;
    int equipIndex;
    int tmp;
    u32* dst;
    u32 msgSpawnId;
    u32 msgExpLoss;
    int slotsLeft;
    int bagIndex;
    int* bagItem;

    pc->boat[0] = 0;
    msgSpawnId = pc->player->spawn_id;
    msgExpLoss = EQ_METHOD(1, u32, 0x00480BCF, (CharacterProfile*, int), pc)((int)ptr);
    send_message_guess
        (g_world_connection_guess, 0x2129, &msgSpawnId, 8, 1);
    pc->y = pc->bind_location[0][1];
    pc->x = pc->bind_location[1][1];
    pc->z = pc->bind_location[2][1];
    pc->heading = 0.0f;
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(pc->zone, pc->bind_zone);
    angle = (u32)(s32)*(f32*)(EQ_ARRAY(u8, 0x006B6F2C) + EQ_GLOBAL(s32, 0x00590154) * 0x1c);
    EQ_METHOD(1, void, 0x0040E4C6, (void*, u32), g_display_guess)(angle);
    if (EQ_GLOBAL(void*, 0x006E4DDC) != 0)
    {
        EQ_METHOD(1, void, 0x004E1F74, (void*, int), EQ_GLOBAL(void*, 0x006E4DDC))(-1);
    }
    if (EQ_GLOBAL(u8, 0x005FE968) != 0)
    {
        if (EQ_GLOBAL(void*, 0x006E4E44) == 0)
            goto clearBuffs;
        *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0xb5) = 0;
        *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0xb4) |= 0xff;
        EQ_METHOD(0, void, 0x004CB4FA, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    }
    if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
    {
        pinstEQ_Main_guess->uiDirty_u32_guess = 1;
    }
clearBuffs:
    buffSlot = (u8*)pc + 0x286;
    tmp = 0xf;
    do
    {
        buffSlot[-2] = 0;
        *buffSlot = 0;
        *(short*)(buffSlot + 2) = -1;
        buffSlot[-1] = 0;
        *(u16*)(buffSlot + 4) = 0;
        *(u16*)(buffSlot + 6) = 0;
        buffSlot = buffSlot + 10;
        tmp = tmp + -1;
    } while (tmp != 0);
    EQ_METHOD(1, void, 0x00480442, (CharacterProfile*, int), pc)((int)ptr);
    *(u16*)((u8*)pc + 0x104c) = 0;
    *(u32*)((u8*)pc + 0x998) = 0;
    *(u32*)((u8*)pc + 0x99c) = 0;
    *(u32*)((u8*)pc + 0x9a0) = 0;
    *(u32*)((u8*)pc + 0x9a4) = 0;
    *(u32*)((u8*)pc + 0x9b8) = 0;
    *(u32*)((u8*)pc + 0x9bc) = 0;
    *(u32*)((u8*)pc + 0x9c0) = 0;
    *(u32*)((u8*)pc + 0x9c4) = 0;
    pc->spell_memorized_pairs_guess[0] = 0xffffffff;
    pc->spell_memorized_pairs_guess[1] = 0xffffffff;
    pc->spell_memorized_pairs_guess[2] = 0xffffffff;
    pc->spell_memorized_pairs_guess[3] = 0xffffffff;
    dst = &pc->spell_slot_refresh[0];
    for (tmp = 10; tmp != 0; tmp = tmp + -1)
    {
        *dst = 0;
        dst = dst + 1;
    }
    equipIndex = 0;
    dst = &pc->equip_colors[0];
    do
    {
        *((u8*)pc + 0x45 + equipIndex) = 0;
        *dst = 0;
        equipIndex = equipIndex + 1;
        dst = dst + 1;
    } while (equipIndex < 9);
    ptr = (int*)((u8*)pc + 0xe0);
    slotsLeft = 0x1e;
    do
    {
        tmp = *ptr;
        if (tmp != 0)
        {
            if ((*(char*)(tmp + 0x81) == '\x01') &&
                (bagIndex = 0, *(char*)(tmp + 0xd5) != '\0'))
            {
                bagItem = (int*)(tmp + 0xac);
                do
                {
                    if (*bagItem != 0)
                    {
                        EQ_CDECL(void, 0x005218BB, (void*))((void*)*bagItem);
                        *bagItem = 0;
                    }
                    bagIndex = bagIndex + 1;
                    bagItem = bagItem + 1;
                } while (bagIndex < (int)*(u8*)(tmp + 0xd5));
            }
            EQ_CDECL(void, 0x005218BB, (void*))((void*)*ptr);
            *ptr = 0;
        }
        ptr = ptr + 1;
        slotsLeft = slotsLeft + -1;
    } while (slotsLeft != 0);
    tmp = 0;
    dst = &pc->equip_colors[0];
    do
    {
        *((u8*)pc + 0x45 + tmp) = 0;
        *dst = 0;
        tmp = tmp + 1;
        dst = dst + 1;
    } while (tmp < 9);
    if (EQ_GLOBAL(void*, 0x006E4DC0) != 0)
    {
        send_message_guess
            (g_world_connection_guess, 0x204f,
                (u8*)EQ_GLOBAL(void*, 0x006E4DC0) + 0x80, 4, 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x004806cb
// Supplies low-level characters with starter gear or restores items selected by their newbie gear flags.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (Mac: EQ_PC::RefitNewbieEQ, low)
// __thiscall. Puts fixed item numbers into inventory slots 22..29: one per set bit of the newbie item flags
// (0xAF0), or, with no flags and below level 4, the starter items (two stacks of 5 in slots 22 and 23, item
// 0x490C in slot 29 unless g_pvpRuleset_guess is set, and a class-dependent item in slot 24).
void EQ_PC::RefitNewbieEQ(void)
{
    u32 flags = newbie_items_guess;

    if (flags != 0)
    {
        if (flags & 0x01)
            inventory[25] = 0x5078;
        if (flags & 0x02)
            inventory[26] = 0x5193;
        if (flags & 0x04)
            inventory[27] = 0x5194;
        if (flags & 0x08)
            inventory[24] = 0x7B70;
        if (flags & 0x10)
            inventory[28] = 0x7B71;
        if (flags & 0x20)
            inventory[29] = 0x7B72;
        if (flags & 0x40)
            inventory[23] = 0x6FBA;
        return;
    }
    if (level > 3)
        return;
    inventory[22] = 0x2707;
    inv_props[22].unknown0x00 = 1;
    inv_props[22].charges = 5;
    inv_props[22].unknown0x06 = 1;
    inventory[23] = 0x2706;
    inv_props[23].unknown0x00 = 1;
    inv_props[23].charges = 5;
    inv_props[23].unknown0x06 = 1;
    if (g_pvpRuleset_guess == 0)
        inventory[29] = 0x490C;
    // One statement per label: shared fall-through labels make VC6 build a compare tree, not the jump table.
    switch (class_)
    {
    case 1:  inventory[24] = 0x270E; break;
    case 3:  inventory[24] = 0x270E; break;
    case 4:  inventory[24] = 0x270E; break;
    case 5:  inventory[24] = 0x270E; break;
    case 8:  inventory[24] = 0x270E; break;
    case 2:  inventory[24] = 0x270F; break;
    case 6:  inventory[24] = 0x270F; break;
    case 10: inventory[24] = 0x270F; break;
    case 9:  inventory[24] = 0x270D; break;
    case 11: inventory[24] = 0x270D; break;
    case 12: inventory[24] = 0x270D; break;
    case 13: inventory[24] = 0x270D; break;
    case 14: inventory[24] = 0x270D; break;
    case 15: inventory[24] = 0x270D; break;
    }
}

// FUNCTION: TRILOGY 0x004807fc
// Calculates the experience threshold for a level using the character's class and race.
int EQ_PC_guess::GetExpForLevel_guess(u8 temp, int index, int classId)
{
    short scaledSkill = (short)EQ_STDCALL(int, 0x00480391, (int, int))(index, classId);
    int value = scaledSkill;

    value *= temp;
    value *= temp;
    value *= temp;

    if (temp >= 0x1e && temp < 0x23)
        return (int)(value * EQ_GLOBAL(double, 0x00535610));
    if (temp >= 0x23 && temp < 0x28)
        return (int)(value * EQ_GLOBAL(double, 0x00535608));
    if (temp >= 0x28 && temp < 0x2d)
        return (int)(value * EQ_GLOBAL(double, 0x00535600));
    if (temp >= 0x2d && temp <= 0x32)
        return (int)(value * EQ_GLOBAL(double, 0x005355F8));
    if (temp == 0x33)
        return (int)(value * EQ_GLOBAL(double, 0x005355F0));
    if (temp == 0x34)
        return (int)(value * EQ_GLOBAL(double, 0x005355E8));
    if (temp == 0x35)
        return (int)(value * EQ_GLOBAL(double, 0x005355E0));
    if (temp == 0x36)
        return (int)(value * EQ_GLOBAL(double, 0x005355D8));
    if (temp == 0x37)
        return (int)(value * EQ_GLOBAL(double, 0x005355D0));
    if (temp == 0x38)
        return (int)(value * EQ_GLOBAL(double, 0x005355C8));
    if (temp == 0x39)
        return (int)(value * EQ_GLOBAL(double, 0x005355C0));
    if (temp == 0x3a)
        return (int)(value * EQ_GLOBAL(double, 0x005355B8));
    if (temp == 0x3b)
        return (int)(value * EQ_GLOBAL(double, 0x005355B0));
    if (temp < 0x3c)
        return value;

    {
        int capBase = temp + temp;
        double cap = capBase + EQ_GLOBAL(double, 0x005355A8);
        double scaled = value * EQ_GLOBAL(double, 0x005355B0);

        if (scaled > cap)
            scaled = cap;
        return (int)scaled;
    }
}


// FUNCTION: TRILOGY 0x00480954
// Lets the player remove a removable buff and updates the server about the changed effect.
#pragma optimize("y", off)
bool EQ_PC_guess::TryRemoveBuff_guess(short buffSlot)
{
    int buff;
    short spellId;
    int spell;
    char found;
    u8 effectLevel;
    u8 requiredLevel;
    u8 buffLevel;

    buff = 0;
    if (buffSlot >= 0 && buffSlot < 0x0f)
        buff = (int)this + 0x284 + buffSlot * 10;

    if (*(int*)((char*)this + 0xa0) == 0 || buff == 0)
        return false;

    spellId = *(short*)(buff + 4);
    if (spellId > 0 && spellId < 3000)
    {
        spell = (int)EQ_ARRAY(void*, 0x005FE9A0)[spellId];
        if (spell != 0)
        {
            found = EQ_CDECL(char, 0x0048A11F, (void*, int))( (void*)spell, 0x65);
            if (found != '\0')
                goto cannot_remove;
        }
    }

    spellId = *(short*)(buff + 4);
    if (spellId <= 0 || spellId >= 3000 || spellId == 0x2b6
        || spellId == 0x5e6 || spellId == 0x6b5)
        goto cannot_remove;

    spell = (int)EQ_ARRAY(void*, 0x005FE9A0)[spellId];
    if (spell == 0 || *(char*)(spell + 0x21f) == '\0')
        goto cannot_remove;

    if (spellId == 0x5a7)
        goto remove_effect;

    found = EQ_CDECL(char, 0x0048A11F, (void*, int))( (void*)spell, 0x39);
    if (found != '\0')
        goto remove_effect;

    if (*(char*)(spell + 0x21f) != '\0')
    {
        found = EQ_CDECL(char, 0x0048A11F, (void*, int))( (void*)spell, 3);
        if (found != '\0')
            goto remove_effect;
    }

    effectLevel = this->level;
    requiredLevel = *(u8*)(spell + this->class_ + 0x234);
    if (effectLevel >= requiredLevel)
    {
        buffLevel = *(u8*)(buff + 1);
        if (effectLevel == buffLevel || (u32)effectLevel == (u32)buffLevel + 1)
            goto remove_effect;

        found = EQ_CDECL(char, 0x0048A11F, (void*, int))( (void*)spell, 0x43);
        if (found != '\0')
            goto remove_effect;
    }

cannot_remove:
    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
        ((void*)this, (const char*)0x0055c3f0, 0xd);
    return false;

remove_effect:
    EQ_METHOD(1, void, 0x00424E22, (void*, void*), (void*)this)((void*)buff);
    EQ_METHOD(1, void, 0x004251B9, (void*, int), (void*)this)((int)buffSlot);
    return true;
}
#pragma optimize("y", on)


// FUNCTION: TRILOGY 0x00480a94
// Applies fatigue from hunger, thirst, movement, and activity, with recovery when fed and hydrated.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (Mac: EQ_PC::ProcessFatigue, low)
// __thiscall. One fatigue tick: 0 while g_offlineMode_guess / DAT_006b6494 are set; otherwise +1 for no food or
// no drink, +1 when g_runMode_guess == 1 and the spawn moves, +1 in animations 5..10; unchanged since the last
// tick with food and drink = recover 10. The last value is kept in g_lastFatigue_guess.
void EQ_PC::ProcessFatigue(void)
{
    u8 anim;

    if (g_offlineMode_guess != 0 || DAT_006b6494 != 0)
    {
        fatigue = 0;
        return;
    }
    if (hunger == 0 || thirst == 0 || g_runMode_guess == 1
        || (anim = player->actor_info->unknown0x099, anim == 5) || anim == 6 || anim == 7 || anim == 9
        || anim == 10 || anim == 8)
    {
        if (hunger == 0 || thirst == 0)
            SetFatigue(fatigue + 1);
        if (g_runMode_guess == 1 && player->speed != 0.0f)
            SetFatigue(fatigue + 1);
        anim = player->actor_info->unknown0x099;
        if (anim == 5 || anim == 6 || anim == 7 || anim == 9 || anim == 10 || anim == 8)
            SetFatigue(fatigue + 1);
    }
    if (fatigue == g_lastFatigue_guess && hunger > 0 && thirst > 0)
    {
        if (fatigue > 10)
            SetFatigue(fatigue - 10);
        else
            SetFatigue(0);
    }
    g_lastFatigue_guess = fatigue;
}
// FUNCTION: TRILOGY 0x00480BCF
// Applies the death experience penalty and lowers the player's level if the loss crosses a level threshold.
u32 EQ_PC_guess::ApplyDeathExperienceLoss_guess(int applyPenalty)
{
    u8* object = (u8*)this;
    char message[80];
    u8 messageData[12] = {0};
    int expStep;
    u32 lossCap;
    char levelsLost = '\0';
    u8 currentLevel;
    int value;
    int lowerLevelExperience;
    u32 experienceLoss;
    u32 experienceValue;
    u32 registerEax = 0;
    u32 argumentSeed = (u32)this;
    u32 argumentPack;
    u32 initialEbx;
    u32 loopEcx;
    float percent;

    __asm mov initialEbx, ebx
    lossCap = 6000000;
    if (*(int*)(object + 0xa0) != 0)
    {
        registerEax = *(u32*)(*(int*)(object + 0xa0) + 0x70);
        if (*(char*)(registerEax + 0x9c) != '\0')
        {
            argumentSeed = (u32)EQ_ARRAY(char, 0x0055C464);
            value = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                ((const char*)(object + 0x974), EQ_ARRAY(char, 0x0055C464));
            if (value != 0)
                return 0;
            registerEax = 0;
        }
    }
    if (g_pvpRuleset_guess == 4 && *(u8*)(object + 0x38) <= 5)
        return 0;
    currentLevel = *(u8*)(object + 0x38);
    if (currentLevel <= 5 || applyPenalty != 1)
        return 0;
    if (currentLevel < 0x19)
    {
        percent = (float)currentLevel * EQ_GLOBAL(float, 0x00535224);
        argumentPack = *(u16*)(object + 0x34);
    }
    else
    {
        percent = 0.25f;
        argumentPack = (argumentSeed & 0xffff0000) | *(u16*)(object + 0x34);
    }

    lowerLevelExperience = EQ_METHOD(3, int, 0x004807FC, (void*, int, int, int), this)((int)((registerEax & 0xffffff00) | (u8)(currentLevel - 2)),
         (int)((argumentPack & 0xffffff00) | *(u8*)(object + 0x36)),
         (int)argumentPack);
    argumentPack = ((u32)lowerLevelExperience & 0xffff0000) | *(u16*)(object + 0x34);
    value = EQ_METHOD(3, int, 0x004807FC, (void*, int, int, int), this)((int)((argumentPack & 0xffffff00) | (u8)(currentLevel - 1)),
         (int)((argumentPack & 0xffffff00) | *(u8*)(object + 0x36)),
         (int)argumentPack);
    expStep = value - lowerLevelExperience;
    experienceLoss = (u32)(int)((float)expStep * percent);
    currentLevel = *(u8*)(object + 0x36);
    experienceLoss >>= 1;
    if (currentLevel != 9 && currentLevel != 1)
    {
        experienceLoss = (experienceLoss * 9) / 10;
        if (EQ_ARRAY(short, 0x0055C2DE)[currentLevel] > 1000)
        {
            lossCap = (u32)(int)((double)EQ_ARRAY(short, 0x0055C2DE)[currentLevel]
                * *(double*)((char*)&EQ_GLOBAL(float, 0x00535224) + 0x3fc)
                * *(double*)((char*)&EQ_GLOBAL(float, 0x00535224) + 0x3f4));
        }
    }
    if (lossCap < experienceLoss)
        experienceLoss = lossCap;
    experienceValue = *(u32*)(object + 0x3c);
    if (experienceLoss < experienceValue)
    {
        experienceValue -= experienceLoss;
        *(u32*)(object + 0x3c) = experienceValue;
    }
    else
        *(u32*)(object + 0x3c) = 0;

    argumentPack = (experienceValue & 0xffff0000) | *(u16*)(object + 0x34);
    experienceValue = EQ_METHOD(3, int, 0x004807FC, (void*, int, int, int), this)((int)((argumentPack & 0xffffff00) | (u8)(*(u8*)(object + 0x38) - 1)),
         (int)((initialEbx & 0xffffff00) | currentLevel), (int)argumentPack);
    __asm mov loopEcx, ecx
    if (experienceValue <= *(u32*)(object + 0x3c))
        return experienceLoss;
    do
    {
        currentLevel = *(u8*)(object + 0x38);
        if (currentLevel < 2)
            break;
        levelsLost++;
        *(u8*)(object + 0x38) = currentLevel - 1;
        argumentPack = (loopEcx & 0xffff0000) | *(u16*)(object + 0x34);
        experienceValue = EQ_METHOD(3, int, 0x004807FC, (void*, int, int, int), this)((int)((experienceValue & 0xffffff00) | (u8)(currentLevel - 2)),
             (int)((argumentPack & 0xffffff00) | *(u8*)(object + 0x36)),
             (int)argumentPack);
        __asm mov loopEcx, ecx
    }
    while (*(u32*)(object + 0x3c) < experienceValue);

    if (levelsLost != '\0')
    {
        if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
        {
            EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), EQ_GLOBAL(void*, 0x006E4DD8))(0x8c, 0);
        }
        if (*(int*)(object + 0xa0) != 0)
            *(u8*)(*(int*)(object + 0xa0) + 0x9d) = *(u8*)(object + 0x38);
        if (levelsLost == '\x01')
        {
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (message, EQ_ARRAY(char, 0x0055C43C),
                 (u32)*(u8*)(object + 0x38));
        }
        else
        {
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (message, EQ_ARRAY(char, 0x0055C410), (u32)(u8)levelsLost,
                 (u32)*(u8*)(object + 0x38));
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(message, 0xd);
        *(u16*)(messageData + 4) = 1;
        *(u32*)(messageData + 0) = *(u32*)(*(int*)(object + 0xa0) + 0x80);
        *(u32*)(messageData + 8) = (u32)*(u8*)(object + 0x38);
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x20f5, messageData, 0xc, 1);
        return experienceLoss;
    }
    return experienceLoss;
}

// FUNCTION: TRILOGY 0x00480dfb
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (Mac: EQ_PC::SetFatigue, low)
// __thiscall. Fatigue clamped to 0..100.
void EQ_PC::SetFatigue(int value)
{
    if (value > 100)
        fatigue = 100;
    else if (value < 0)
        fatigue = 0;
    else
        fatigue = (u8)value;
}

// FUNCTION: TRILOGY 0x00480e23
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_PC.cpp (from Mac debug map)
// Rebuilds the six live party pointers from the saved names; the local player counts as one.
int EQ_PC::numInParty(void)
{
    int count = 1;
    int remaining = 6;
    EQPlayer** members = (EQPlayer**)((char*)this + 0x1198);
    char* member_name = group_members[0];

    do
    {
        EQPlayer* player;
        *members = 0;
        if (EQ_CDECL(int, 0x00524240, (const char*, const char*))(member_name, name) != 0 &&
            member_name[0] != 0)
        {
            player = EQ_METHOD(1, EQPlayer*, 0x004B1435, (EQPlayer*, const char*), g_player_top_guess)(member_name);
            if (player != 0 && player->type == 0)
            {
                *members = player;
                count++;
            }
        }
        members++;
        member_name += 48;
        remaining--;
    } while (remaining != 0);
    return count;
}

// FUNCTION: TRILOGY 0x00480e8c
// Static helper used to update a skill's maximum from its Kunark base and level thresholds.
// @early-stop: the original keeps level in BL and saves EBX at entry; this rewrite spills level and saves EBX only in the Kunark path.
int EQ_PC::GetKunarkThreshold_guess(EQ_PC *pc, short index, u32 *maximum)
{
    u8 *pc_bytes;
    u8 level;
    int base;
    int goal;
    int span;
    int increment;
    int value;

    if (pc != 0)
    {
        pc_bytes = (u8 *)pc;
        level = pc_bytes[0x38];
        if (level >= 0x33 && *(u32 *)(pc_bytes + 0xae0) != 0)
        {
            base = ((u8 *)EQ_ARRAY(u32, 0x006E33A4)[index])[0x68 + pc_bytes[0x36]];
            goal = *(u32 *)((u8 *)EQ_ARRAY(u32, 0x006E33A4)[index] + 0x94 + pc_bytes[0x36] * 4);
            span = goal - base;
            if (span < 0)
                span = 0;
            increment = span / 10;
            if (span % 10 != 0)
                increment++;
            if (increment < 2)
                increment = 2;
            value = (level - 0x32) * increment + base;
            if ((int)value > (int)goal)
                value = goal;
            if ((int)*maximum <= (int)value)
                return 0;
            *maximum = value;
            return 1;
        }
        else
        {
            base = ((u8 *)EQ_ARRAY(u32, 0x006E33A4)[index])[0x68 + pc_bytes[0x36]];
            if ((int)*maximum <= (int)base)
                return 0;
            *maximum = base;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00480f3b
// Whether a skill has reached its class/level cap, counting the tradeskill and spell-specialization limits.
bool EQ_PC_guess::IsSkillAtMax_guess(short skill)
{
    u8 *pc_bytes;
    u8 *row;
    u8 byteValue;
    int tmp;
    u32 value;
    int acc;
    int skillIndex;
    bool overflow;
    u32 temp;

    skillIndex = (int)(short)skill;
    pc_bytes = (u8 *)this;
    row = (u8 *)EQ_ARRAY(u32, 0x006E33A4)[skillIndex];
    if (*(char *)(row + 0x27) == '\0')
    {
        if ((skillIndex < 0x3c) ||
            (0x3d < skillIndex && (skillIndex < 0x3f ||
            (0x41 < skillIndex && (skillIndex < 0x44 || 0x45 < skillIndex)))))
        {
            return false;
        }
        acc = 0xfa;
        tmp = 0;
        do
        {
            if ((tmp != skillIndex) &&
                (tmp == 0x3f || tmp == 0x3d || tmp == 0x45 || tmp == 0x44 ||
                 tmp == 0x40 || tmp == 0x3c || tmp == 0x41) &&
                (200 < *(u8 *)(tmp + 0x9c8 + (int)pc_bytes)))
            {
                acc = 200;
                break;
            }
            tmp = tmp + 1;
        } while (tmp < 100);
        value = (u32)*(u8 *)(skillIndex + 0x9c8 + (int)pc_bytes);
        overflow = ((value ^ (u32)acc) & (value ^ (value - (u32)acc)) & 0x80000000) != 0;
        acc = (int)(value - (u32)acc);
        goto compare;
    }
    if (skillIndex < 0x25)
    {
        if (skillIndex == 0x24)
        {
checkClass:
            byteValue = *(u8 *)(pc_bytes + 0x36);
            if ((byteValue != 2) && (byteValue != 6))
            {
                if (byteValue == 7)
                {
                    byteValue = *(u8 *)(pc_bytes + 0x38);
fromByte:
                    value = (u32)byteValue;
                    goto addOne;
                }
                if (byteValue != 10)
                {
                    if ((10 < byteValue) && (byteValue < 0xf))
                    {
levelPlusOne:
                        tmp = *(u8 *)(pc_bytes + 0x38) + 1;
timesThree:
                        temp = tmp * 3;
                        goto applyCap;
                    }
                    goto defaultCap;
                }
            }
            temp = (u32)*(u8 *)(pc_bytes + 0x38) * 4 + 4;
        }
        else
        {
            if (skillIndex < 0x17)
            {
                if (skillIndex == 0x16)
                {
                    if (*(char *)(pc_bytes + 0x36) == '\a')
                    {
                        temp = (u32)*(u8 *)(pc_bytes + 0x38) * 7 + 5;
                    }
                    else
                    {
                        value = (u32)*(u8 *)(pc_bytes + 0x38);
                        if (*(char *)(pc_bytes + 0x36) != '\t')
                            goto addOne;
                        temp = value * 6 + 5;
                    }
                    goto applyCap;
                }
                if (skillIndex < 0xf)
                {
                    if (((skillIndex < 0xd) && (-1 < skillIndex)) && (skillIndex < 4))
                        goto checkClass;
                }
                else if ((skillIndex == 0xf) && (byteValue = *(u8 *)(pc_bytes + 0x36), byteValue != 1))
                {
                    if (((byteValue == 2) || ((byteValue == 6 || (byteValue == 10)))) ||
                        ((byteValue < 0xb || (0xe < byteValue))))
                        goto levelTimesFive;
                    byteValue = *(u8 *)(pc_bytes + 0x38);
                    if (byteValue < 6)
                    {
                        value = (u32)byteValue;
                        goto timesFive;
                    }
                    if (byteValue < 0xb)
                    {
                        temp = (u32)byteValue * 4 + 5;
                        goto applyCap;
                    }
                    tmp = byteValue + 5;
                    goto timesThree;
                }
            }
            else if (skillIndex != 0x18)
            {
                if (skillIndex == 0x1c)
                {
                    if (*(char *)(pc_bytes + 0x36) != '\a')
                        goto levelPlusOne;
                    byteValue = *(u8 *)(pc_bytes + 0x38);
                    goto fromByte;
                }
                if (((skillIndex != 0x1f) && (skillIndex == 0x21)) &&
                    (byteValue = *(u8 *)(pc_bytes + 0x36), byteValue != 1))
                {
                    if ((byteValue != 2) && (byteValue != 6))
                    {
                        if (byteValue == 7)
                        {
                            byteValue = *(u8 *)(pc_bytes + 0x38);
                            if (byteValue < 0x1e)
                            {
                                value = (u32)byteValue;
                                if (0xe < byteValue)
                                    goto addOne;
                            }
                            else
                            {
                                value = byteValue + 2;
                            }
                            goto timesFive;
                        }
                        if (((byteValue != 10) && (10 < byteValue)) && (byteValue < 0xf))
                        {
                            temp = (u32)*(u8 *)(pc_bytes + 0x38) << 2;
                            goto applyCap;
                        }
                    }
levelTimesFive:
                    value = (u32)*(u8 *)(pc_bytes + 0x38);
                    goto timesFive;
                }
            }
            else
            {
                goto defaultCap;
            }
defaultCap:
            value = (u32)*(u8 *)(pc_bytes + 0x38);
addOne:
            value = value + 1;
timesFive:
            temp = value * 5;
        }
    }
    else
    {
        value = 0x32;
        if (0x32 < skillIndex)
        {
            switch (skillIndex)
            {
            case 0x39:
            case 0x3a:
            case 0x3b:
                byteValue = *(u8 *)(*(u8 *)(pc_bytes + 0x36) + 0x28 + (int)row);
                if (*(u8 *)(pc_bytes + 0x38) < byteValue)
                {
                    temp = 0;
                }
                else
                {
                    temp = (((u32)*(u8 *)(pc_bytes + 0x38) - (u32)byteValue) + 1) * 5;
                }
                break;
            case 0x3c:
            case 0x3d:
            case 0x3f:
            case 0x40:
            case 0x41:
            case 0x44:
            case 0x45:
                tmp = 0;
                temp = 0xfa;
                do
                {
                    if ((tmp != skillIndex) &&
                        (tmp == 0x3f || tmp == 0x3d || tmp == 0x45 || tmp == 0x44 ||
                         tmp == 0x40 || tmp == 0x3c || tmp == 0x41) &&
                        (200 < *(u8 *)(tmp + 0x9c8 + (int)pc_bytes)))
                    {
                        temp = 200;
                        break;
                    }
                    tmp = tmp + 1;
                } while (tmp < 100);
                break;
            default:
                goto defaultCap;
            }
            goto applyCap;
        }
        if (skillIndex == 0x32)
        {
            value = (u32)*(u8 *)(pc_bytes + 0x38);
            if (*(short *)(pc_bytes + 0x34) != 0x80)
                goto addOne;
            value = value + 0x14;
            goto timesFive;
        }
        temp = value;
        if (skillIndex == 0x2b)
        {
            byteValue = *(u8 *)(pc_bytes + 0x9f4);
checkOtherSkill1:
            if (byteValue < 0x33)
            {
                byteValue = *(u8 *)(pc_bytes + 0x9f5);
checkOtherSkill2:
                if (byteValue < 0x33)
                {
                    byteValue = *(u8 *)(pc_bytes + 0x9f6);
checkOtherSkill3:
                    if (byteValue < 0x33)
                    {
                        byteValue = *(u8 *)(pc_bytes + 0x9f7);
                        goto checkLastOtherSkill;
                    }
                }
            }
        }
        else
        {
            if (skillIndex == 0x2c)
            {
                byteValue = *(u8 *)(pc_bytes + 0x9f3);
                goto checkOtherSkill1;
            }
            if (skillIndex == 0x2d)
            {
                if (*(u8 *)(pc_bytes + 0x9f4) < 0x33)
                {
                    byteValue = *(u8 *)(pc_bytes + 0x9f3);
                    goto checkOtherSkill2;
                }
            }
            else
            {
                if (skillIndex == 0x2e)
                {
                    if ((0x32 < *(u8 *)(pc_bytes + 0x9f4)) || (0x32 < *(u8 *)(pc_bytes + 0x9f5)))
                        goto applyCap;
                    byteValue = *(u8 *)(pc_bytes + 0x9f3);
                    goto checkOtherSkill3;
                }
                if (skillIndex != 0x2f)
                    goto defaultCap;
                temp = 0x32;
                if (((0x32 < *(u8 *)(pc_bytes + 0x9f4)) ||
                    (temp = value, 0x32 < *(u8 *)(pc_bytes + 0x9f5))) ||
                    (0x32 < *(u8 *)(pc_bytes + 0x9f6)))
                    goto applyCap;
                byteValue = *(u8 *)(pc_bytes + 0x9f3);
checkLastOtherSkill:
                temp = value;
                if (byteValue < 0x33)
                    goto defaultCap;
            }
        }
    }
applyCap:
    if (*(int *)(pc_bytes + 0xae0) == 0)
    {
        value = (u32)*(u8 *)(*(u8 *)(pc_bytes + 0x36) + 0x68 + (int)row);
        if ((int)value < (int)temp)
            temp = value;
    }
    else
    {
        EQ_CDECL(int, 0x00480E8C, (EQ_PC *, short, u32 *))((EQ_PC *)this, skill, &temp);
    }
    value = (u32)*(u8 *)(skillIndex + 0x9c8 + (int)pc_bytes);
    overflow = ((value ^ temp) & (value ^ (value - temp)) & 0x80000000) != 0;
    acc = (int)(value - temp);
compare:
    return overflow == (acc < 0);
}
