// EQMissile.cpp - EQMissile, projectiles in flight (arrows, thrown weapons, spell bolts).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMissile.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqmissile.h"
#include "trilogy/eqgfx.h"
#include "trilogy/display.h"
#include "trilogy/player.h"
#include "trilogy/layout/eq_effect.h"

typedef void (__stdcall *EQMissile_CriticalSectionFn_guess)(void* cs);

// File-local stand-in for the 0x364-byte actor info the original builds with operator new and EQObject_ctor_guess
// (0x004BCDE8). Its constructor is the client's, only declared here: VC6 then keeps the EH frame around the new
// expression, as the original has.
class EQMissileActor_guess
{
public:
    // EXTERNAL: TRILOGY 0x004BCDE8 EQMissileActor_guess::EQMissileActor_guess
    EQMissileActor_guess(char* name);
    u8 unknown0x00[0x364];
};

// FUNCTION: TRILOGY 0x004ac67c
// Creates a projectile in flight (arrow, thrown item, spell bolt) and plays its shooter's attack animation
// ORIGINAL FILE: EverQuest/EQMissile.cpp (from Mac debug map)
// Constructor (Mac name EQMissile::EQMissile). __fastcall-style: ECX = the new missile (the callers pass the
// operator new result), six stack arguments, callee pops 0x18, returns this. Links the missile into the list
// at 0x006E4D64 under the critical section 0x005FE7A0. With `proto` (a copy of its fields, target from
// proto+0x40/+0x44) or without it (fields from the source player `src`, the item and the skill).
// [ebp+0x18] is reused for the new actor pointer and the scale float; [ebp+0x1b] is the send flag (a local here).
EQMissile_guess* __fastcall EQMissile_Construct_guess(EQMissile_guess* self, int edx, char* item, char* src,
                                                      EQMissile_guess* proto, char* name, u32 skill_slot, u32 target_id)
{
    char* data;
    char* player;
    char* spell;
    EQPlayer* target;
    char* missileActor;
    char* found;
    char* ch_mod;
    char* text;
    int dmg;
    int extra;
    int has_flag;
    int tmp;
    u8 typeByte;
    u8 send_flag;
    u8 usedSkill;
    u16 tmp16;
    u16 race;
    char effectFound;
    u32 scratch;
    u32 animation;
    float absVal;
    struct
    {
        u32 spawn_id;       /* 0x00 */
        u32 anim;           /* 0x04 */
        float scale;        /* 0x08 */
    } msg;

    data = (char*)self;
    has_flag = 0;
    target = 0;
    EQ_GLOBAL(EQMissile_CriticalSectionFn_guess, 0x00534194)(EQ_ARRAY(void, 0x005FE7A0));

    if (EQ_GLOBAL(char*, 0x006E4D64) == 0)
    {
        EQ_GLOBAL(char*, 0x006E4D64) = data;
        *(char**)(data + 0x08) = 0;
        *(char**)(data + 0x04) = 0;
    }
    else
    {
        *(char**)(EQ_GLOBAL(char*, 0x006E4D64) + 0x04) = data;
        *(char**)(data + 0x08) = EQ_GLOBAL(char*, 0x006E4D64);
        *(char**)(data + 0x04) = 0;
        EQ_GLOBAL(char*, 0x006E4D64) = data;
    }

    player = src;
    spell = 0;
    data[0x00] = 1;
    if (player == 0)
    {
        src = 0;
    }
    else
    {
        src = *(char**)(player + 0x74);
        if (item == 0)
        {
            dmg = 2;
        }
        else
        {
            typeByte = *(u8*)(item + 0xC3);
            dmg = *(u8*)(item + 0xBF);
            extra = *(u8*)(item + 0xC1);
            if ((typeByte & 1) != 0)
            {
                has_flag = 1;
            }
            if (*(char*)&skill_slot == 0x0B && src != 0)
            {
                ch_mod = *(char**)(src + 0x10C);
                if (ch_mod != 0 && (*(char*)(ch_mod + 0xC2) == 5 || *(char*)(ch_mod + 0xC2) == 6))
                {
                    dmg = dmg + *(u8*)(ch_mod + 0xBF);
                    extra = extra + *(u8*)(ch_mod + 0xC1);
                }
                if (*(char**)(src + 0xA0) != 0 && *(char*)(*(char**)(src + 0xA0) + 0x98) == 1)
                {
                    dmg = dmg * 2;
                }
            }
        }
        if (item != 0 && *(u8*)(item + 0xC2) != 9 && (*(char*)&skill_slot == 0x0D || *(char*)&skill_slot == 0x0E))
        {
            EQ_CDECL(int, 0x004284FA, (char*, char*, int*, u8))(src, item, &dmg, (u8)*(char*)&skill_slot);
        }
    }

    if (proto != 0)
    {
        ((EQMissile_guess*)data)->y_bits_guess = proto->y_bits_guess;
        ((EQMissile_guess*)data)->x_bits_guess = proto->x_bits_guess;
        ((EQMissile_guess*)data)->z_bits_guess = proto->z_bits_guess;
        ((EQMissile_guess*)data)->heading_bits_guess = proto->heading_bits_guess;
        ((EQMissile_guess*)data)->pitch_bits_guess = proto->pitch_bits_guess;
        ((EQMissile_guess*)data)->headingRate_bits_guess = proto->headingRate_bits_guess;
        ((EQMissile_guess*)data)->pitchRate_bits_guess = proto->pitchRate_bits_guess;
        ((EQMissile_guess*)data)->flightPitch_bits_guess = proto->flightPitch_bits_guess;
        ((EQMissile_guess*)data)->forwardSpeed_bits_guess = proto->forwardSpeed_bits_guess;
        ((EQMissile_guess*)data)->velocityY_bits_guess = proto->velocityY_bits_guess;
        ((EQMissile_guess*)data)->velocityX_bits_guess = proto->velocityX_bits_guess;
        ((EQMissile_guess*)data)->velocityZ_bits_guess = proto->velocityZ_bits_guess;
        ((EQMissile_guess*)data)->sourceId_guess = proto->sourceId_guess;
        ((EQMissile_guess*)data)->targetId_guess = proto->targetId_guess;
        ((EQMissile_guess*)data)->itemIdFlags_guess = proto->itemIdFlags_guess;
        ((EQMissile_guess*)data)->sourceSkillOrLevel_guess = proto->sourceSkillOrLevel_guess;
        *(u32*)(data + 0x4C) = *(u32*)((char *)proto + 0x4C);
        data[0x50] = proto->light_guess;
        data[0x51] = ((char *)proto)[0x51];
        data[0x52] = ((char *)proto)[0x52];
        data[0x53] = ((char *)proto)[0x53];
        data[0x54] = ((char *)proto)[0x54];
        EQ_CDECL(char*, 0x005219C0, (char*, char*))(data + 0x55, (char *)proto + 0x55);

        missileActor = (char*)new EQMissileActor_guess(data + 0x55);
        skill_slot = (u32)missileActor;
        found = missileActor;
        *(char**)(data + 0x3C) = found;

        found = EQ_METHOD(1, char*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(*(int*)((char *)proto + 0x40));
        *(char**)(*(char**)(data + 0x3C) + 0x2A0) = found;
        found = EQ_METHOD(1, char*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(*(int*)((char *)proto + 0x44));
        *(char**)(*(char**)(data + 0x3C) + 0x2A4) = found;
        *(u8*)(*(char**)(data + 0x3C) + 0x2C0) = 0;

        *(float*)(*(char**)(data + 0x3C) + 0x48) = EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), 
            EQ_GLOBAL(void*, 0x006E4DE0))(((EQMissile_guess*)data)->y, ((EQMissile_guess*)data)->x, ((EQMissile_guess*)data)->z);
        if (*(float*)(*(char**)(data + 0x3C) + 0x48) == -1e27f)
        {
            *(float*)(*(char**)(data + 0x3C) + 0x48) = 1.0f;
        }
        *(u32*)(*(char**)(data + 0x3C) + 0x1DC) = 0;
        *(float*)(*(char**)(data + 0x3C) + 0x2BC) = 0.0f;
        *(u8*)(*(char**)(data + 0x3C) + 0x88) = 0;
        goto finishSetup;
    }

    ((EQMissile_guess*)data)->sourceId_guess = *(u32*)(player + 0x80);
    ((EQMissile_guess*)data)->targetId_guess = target_id;
    if (target_id != 0)
    {
        target = (EQPlayer*)EQ_METHOD(1, char*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))((int)target_id);
    }

    if (name == 0)
    {
        text = item + 0x5F;
    }
    else
    {
        text = name;
    }
    EQ_CDECL(char*, 0x005219C0, (char*, char*))(data + 0x55, text);
    tmp = has_flag;
    ((EQMissile_guess*)data)->weight_guess = *(u8*)(item + 0x7D);
    *(u8*)(data + 0x50) = *(u8*)(item + 0xBD);
    tmp16 = *(u16*)(item + 0x82);
    ((EQMissile_guess*)data)->itemIdFlags_guess = tmp16;
    if (tmp != 0)
    {
        ((EQMissile_guess*)data)->itemIdFlags_guess = tmp16 | 0x8000;
    }
    tmp = dmg;
    ((EQMissile_guess*)data)->spellId = tmp;
    if (*(u8*)(item + 0xC2) == 9)
    {
        tmp = (int)*(short*)(item + 0xCE);
        ((EQMissile_guess*)data)->spellId = tmp;
    }
    if (*(char*)&skill_slot == 0x0B)
    {
        ((EQMissile_guess*)data)->range_guess = (u8)extra;
        typeByte = *(u8*)(item + 0xC2);
        if (typeByte != 0 && (typeByte < 2 || 3 < typeByte))
        {
            goto storeType;
        }
        ((EQMissile_guess*)data)->kind = 7;
    }
    else
    {
        ((EQMissile_guess*)data)->range_guess = 0;
        typeByte = *(u8*)(item + 0xC2);
storeType:
        ((EQMissile_guess*)data)->kind = typeByte;
    }
    usedSkill = EQ_METHOD(1, u8, 0x004B422A, (void*, u8), player)(((EQMissile_guess*)data)->kind);
    ((EQMissile_guess*)data)->skillOrDelay_guess = usedSkill;
    if (((EQMissile_guess*)data)->kind == 9)
    {
        tmp16 = (u16)*(u8*)(src + 0x38);
        ((EQMissile_guess*)data)->sourceSkillOrLevel_guess = tmp16;
    }
    else
    {
        if (*(char**)(player + 0x74) != 0)
        {
            if (((EQMissile_guess*)data)->kind == 0x16)
            {
                ((EQMissile_guess*)data)->skillOrDelay_guess = *(u8*)(item + 0xBE);
            }
            tmp16 = EQ_METHOD(1, u16, 0x0041A32C, (void*, u8), src)(((EQMissile_guess*)data)->skillOrDelay_guess);
            ((EQMissile_guess*)data)->sourceSkillOrLevel_guess = tmp16;
        }
        else
        {
            ((EQMissile_guess*)data)->sourceSkillOrLevel_guess = 0;
        }
    }
    ((EQMissile_guess*)data)->velocityY_bits_guess = *(u32*)(player + 0x4C);
    ((EQMissile_guess*)data)->velocityX_bits_guess = *(u32*)(player + 0x50);
    ((EQMissile_guess*)data)->velocityZ_bits_guess = *(u32*)(player + 0x54);
    ((EQMissile_guess*)data)->y_bits_guess = *(u32*)(player + 0x34);
    ((EQMissile_guess*)data)->x_bits_guess = *(u32*)(player + 0x38);
    race = *(u16*)(player + 0x9A);
    if (race == 0x31 || race == 0x9E || race == 0xC4)
    {
        ((EQMissile_guess*)data)->z = *(float*)(player + 0x3C) + 2.0f;
    }
    else
    {
        ((EQMissile_guess*)data)->z = *(float*)(player + 0x7C) * 0.5f + *(float*)(player + 0x3C);
    }
    if (player != 0 && ((EQMissile_guess*)data)->range_guess != 0)
    {
        tmp = EQ_METHOD(2, int, 0x004B11E8, (void*, char*, char*), player)((char*)target, data);
        if (tmp != 2)
        {
            ((EQMissile_guess*)data)->heading_bits_guess = *(u32*)(player + 0x40);
            ((EQMissile_guess*)data)->flightPitch_bits_guess = *(u32*)(player + 0x60);
            ((EQMissile_guess*)data)->pitch_bits_guess = *(u32*)(player + 0x60);
        }
    }
    if (((EQMissile_guess*)data)->kind == 9)
    {
        tmp = ((EQMissile_guess*)data)->spellId;
        if (tmp < 3000 && tmp >= 0)
        {
            spell = EQ_ARRAY(char*, 0x005FE9A0)[tmp];
            if (spell != 0 && target != 0 && *(char*)(spell + 0x22E) == 8)
            {
                ((EQMissile_guess*)data)->y_bits_guess = target->y_bits_guess;
                ((EQMissile_guess*)data)->x_bits_guess = target->x_bits_guess;
                ((EQMissile_guess*)data)->z_bits_guess = target->z_bits_guess;
            }
        }
    }
    if (((EQMissile_guess*)data)->range_guess != 0)
    {
        ((EQMissile_guess*)data)->y = ((float (__cdecl *)(float))EQ_GLOBAL(void*, 0x006E524C))(((EQMissile_guess*)data)->heading) * 1.5f
            + ((EQMissile_guess*)data)->y;
        ((EQMissile_guess*)data)->x = ((float (__cdecl *)(float))EQ_GLOBAL(void*, 0x006E5250))(((EQMissile_guess*)data)->heading) * 1.5f
            + ((EQMissile_guess*)data)->x;
    }

    missileActor = (char*)new EQMissileActor_guess(data + 0x55);
    skill_slot = (u32)missileActor;
    found = missileActor;
    *(char**)(data + 0x3C) = found;
    *(char**)(found + 0x2A4) = (char*)target;
    *(char**)(*(char**)(data + 0x3C) + 0x2A0) = player;
    send_flag = 1;
    *(u8*)(*(char**)(data + 0x3C) + 0x2C0) = 1;
    *(u32*)(*(char**)(data + 0x3C) + 0x48) = *(u32*)(*(char**)(player + 0x70) + 0x48);
    ((EQMissile_guess*)data)->headingRate_guess = 0.0f;
    ((EQMissile_guess*)data)->pitchRate_guess = 0.0f;
    if (((EQMissile_guess*)data)->range_guess == 0)
    {
        ((EQMissile_guess*)data)->forwardSpeed_guess = 1.0f;
clearActorFloat:
        *(float*)(*(char**)(data + 0x3C) + 0x2BC) = 0.0f;
    }
    else
    {
        typeByte = ((EQMissile_guess*)data)->kind;
        if (typeByte == 9)
        {
            ((EQMissile_guess*)data)->forwardSpeed_guess = 1.5f;
            if (((EQMissile_guess*)data)->spellId == 0x35B)
            {
                ((EQMissile_guess*)data)->forwardSpeed_guess = 0.8f;
            }
            goto clearActorFloat;
        }
        if (typeByte != 0x11 && (typeByte < 0x1B || 0x1C < typeByte))
        {
            ((EQMissile_guess*)data)->pitchRate_guess = -20.0f;
            ((EQMissile_guess*)data)->forwardSpeed_guess = 2.5f;
            goto clearActorFloat;
        }
        ((EQMissile_guess*)data)->pitch = ((EQMissile_guess*)data)->pitch + 128.0f;
        *(float*)(*(char**)(data + 0x3C) + 0x2BC) = 0.0f;
        ((EQMissile_guess*)data)->forwardSpeed_guess = 4.0f;
    }
    *(u8*)(*(char**)(data + 0x3C) + 0x88) = 0;
    if (((EQMissile_guess*)data)->range_guess == 0 && (race == 0x31 || race == 0x9E || race == 0xC4))
    {
        ((EQMissile_guess*)data)->velocityY = ((EQMissile_guess*)data)->velocityY * 3.5f;
        ((EQMissile_guess*)data)->velocityX = ((EQMissile_guess*)data)->velocityX * 3.5f;
        ((EQMissile_guess*)data)->velocityZ = ((EQMissile_guess*)data)->velocityZ * 3.5f;
    }
    if (((EQMissile_guess*)data)->kind == 9)
    {
        tmp = ((EQMissile_guess*)data)->spellId;
        if (tmp < 3000 && tmp >= 0)
        {
            spell = EQ_ARRAY(char*, 0x005FE9A0)[tmp];
        }
        if (spell != 0)
        {
            effectFound = EQ_CDECL(char, 0x0048A11F, (char*, int))(spell, 0x4C);
            if (effectFound != 0)
            {
                send_flag = 0;
            }
        }
    }
    if (((EQMissile_guess*)data)->range_guess == 0)
    {
        send_flag = 0;
    }
    if (spell != 0 && *(char*)(spell + 0x22E) == 8)
    {
        ((EQMissile_guess*)data)->forwardSpeed_guess = 0.0f;
        ((EQMissile_guess*)data)->velocityY = 0.0f;
        ((EQMissile_guess*)data)->velocityX = 0.0f;
        ((EQMissile_guess*)data)->velocityZ = 0.0f;
    }
    if (EQ_GLOBAL(u8, 0x006B63F8) == 0 && EQ_GLOBAL(u32, 0x006B6494) == 0 && send_flag != 0)
    {
        EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))(EQ_ARRAY(void*, 0x006B3194), 0x2045, data, 0x74, 1);
    }

finishSetup:
    scratch = 1;
    absVal = ((EQMissile_guess*)data)->velocityY;
    if (((EQMissile_guess*)data)->velocityY < 0.0f)
    {
        absVal = -absVal;
    }
    if (absVal < 0.001f)
    {
        ((EQMissile_guess*)data)->velocityY = 0.0f;
    }
    absVal = ((EQMissile_guess*)data)->velocityX;
    if (((EQMissile_guess*)data)->velocityX < 0.0f)
    {
        absVal = -absVal;
    }
    if (absVal < 0.001f)
    {
        ((EQMissile_guess*)data)->velocityX = 0.0f;
    }
    absVal = ((EQMissile_guess*)data)->velocityZ;
    if (((EQMissile_guess*)data)->velocityZ < 0.0f)
    {
        absVal = -absVal;
    }
    if (absVal < 0.001f)
    {
        ((EQMissile_guess*)data)->velocityZ = 0.0f;
    }
    *(u32*)(*(char**)(data + 0x3C)) = 0;
    *(float*)&skill_slot = 1.0f;
    if (spell == 0)
    {
        animation = EQ_CDECL(u32, 0x004A3C49, (u8, u8, u8, float*))(data[0x54], data[0x53], data[0x52], (float*)&skill_slot);
    }
    else
    {
        animation = *(u8*)(spell + 0x254);
    }
    if (animation != 0xFE)
    {
        EQ_GLOBAL(u32, 0x006D6FE0) = scratch;
        if (*(float*)&skill_slot < 0.001f)
        {
            *(float*)&skill_slot = 1.0f;
        }
        EQ_METHOD(6, void, 0x0048ED3B, (void*, char*, u32, int, int, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(*(char**)(*(char**)(data + 0x3C) + 0x2A0), animation, 1, 0,
            *(float*)&skill_slot, 1);
        EQ_GLOBAL(u32, 0x006D6FE0) = 0;
    }
    if (((EQMissile_guess*)data)->range_guess == 0)
    {
        if (*(u8*)(*(char**)(data + 0x3C) + 0x2C0) != 0 && EQ_GLOBAL(u8, 0x006B63F8) == 0 &&
            EQ_GLOBAL(u32, 0x006B6494) == 0 && animation != 0xFE)
        {
            scratch = *(u32*)(*(char**)(*(char**)(data + 0x3C) + 0x2A0) + 0x80);
            msg.anim = animation;
            msg.spawn_id = scratch;
            msg.scale = *(float*)&skill_slot;
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))(EQ_ARRAY(void*, 0x006B3194), 0x209F, &msg, 0xC, 1);
        }
    }
    else
    {
        EQ_GLOBAL(u32, 0x006D6FE0) = scratch;
        scratch = EQ_METHOD(2, u32, 0x0048EB3A, (void*, char*, u32), EQ_GLOBAL(void*, 0x006E4DE0))(*(char**)(*(char**)(data + 0x3C) + 0x2A0), animation);
        tmp = (int)((double)scratch * 0.7);
        if (tmp == 0)
        {
            tmp = 0x5DC;
        }
        *(int*)(*(char**)(data + 0x3C) + 0x2C4) = (int)EQ_CDECL(u32, 0x004A461E, (void))() + tmp;
        EQ_GLOBAL(u32, 0x006D6FE0) = 0;
    }
    if (*(char**)(*(char**)(data + 0x3C) + 0x2A0) == 0)
    {
        *(u32*)(*(char**)(data + 0x3C) + 0x2B4) = ((EQMissile_guess*)data)->y_bits_guess;
        scratch = ((EQMissile_guess*)data)->x_bits_guess;
    }
    else
    {
        *(u32*)(*(char**)(data + 0x3C) + 0x2B4) = *(u32*)(*(char**)(*(char**)(data + 0x3C) + 0x2A0) + 0x34);
        scratch = *(u32*)(*(char**)(*(char**)(data + 0x3C) + 0x2A0) + 0x38);
    }
    *(u32*)(*(char**)(data + 0x3C) + 0x2B8) = scratch;
    EQ_GLOBAL(EQMissile_CriticalSectionFn_guess, 0x00534190)(EQ_ARRAY(void, 0x005FE7A0));
    return self;
}

// FUNCTION: TRILOGY 0x004acead
// Unlinks the missile and runs its final impact response when its actor is still present.
// ORIGINAL FILE: EverQuest/EQMissile.cpp (from Mac debug map)
/* fake match: union and chained stores reproduce the original x87 zero and field/local store order. */
void __fastcall EQMissile_Destructor_guess(EQMissile_guess* self)
{
    char* missileBytes;
    u8 actorIsNull;
    int missileType;
    void* missileInfo;

    missileBytes = (char*)self;
    EQ_GLOBAL(EQMissile_CriticalSectionFn_guess, 0x00534194)(EQ_ARRAY(void, 0x005FE7A0));

    if (self->previous_guess != 0 && self->next != 0)
    {
        self->previous_guess->next = self->next;
        self->next->previous_guess = self->previous_guess;
    }
    else if (self->previous_guess == 0 && self->next != 0)
    {
        self->next->previous_guess = 0;
        g_missile_top_guess = self->next;
    }
    else if (self->next == 0 && self->previous_guess != 0)
    {
        self->previous_guess->next = 0;
    }
    else
    {
        self->next = 0;
        self->previous_guess = 0;
        g_missile_top_guess = 0;
    }

    actorIsNull = 0;
    if (*self->actor_info == 0)
    {
        actorIsNull++;
    }
    if (self == EQ_GLOBAL(EQMissile_guess*, 0x006E4DA4))
    {
        EQ_GLOBAL(EQMissile_guess*, 0x006E4DA4) = 0;
    }
    if (*(u8*)(missileBytes + 0x53) == 9 && actorIsNull == 0)
    {
        EQ_METHOD(0, void, 0x004AD088, (EQMissile_guess*), self)();
        missileType = *(int*)(missileBytes + 0x4C);
        if (missileType != 0x36)
        {
            if (missileType == 0x35B)
            {
                const float* maxHitDistance = &EQ_GLOBAL(float, 0x0053570C);

                if (EQ_METHOD(7, float, 0x004129F3,
                          (CDisplay*, float, float, float, float, float, float, float),
                              g_cdisplay_guess)(
                    *(float*)(missileBytes + 0x0C), *(float*)(missileBytes + 0x10),
                    *(float*)(missileBytes + 0x14), EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y,
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z,
                    0.0f) <= *maxHitDistance)
                {
                    struct EQMissileHitMessage_guess
                    {
                        u32 spawn_id;
                        u32 unknown0x04;
                        u16 unknown0x08;
                        u8 unknown0x0A[2];
                        u32 hit_type;
                        union
                        {
                            u32 hit_data;
                            float hit_data_float;
                        };
                        u32 unknown0x14;
                        u32 unknown0x18;
                        u8 unknown0x1C;
                        u8 unknown0x1D;
                        u16 unknown0x1E;
                        u16 unknown0x20;
                        u8 unknown0x22;
                        u8 terminator;
                    } hit;
                    char* hitData;
                    EQPlayer* player;

                    player = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
                    hit.spawn_id = player->spawn_id;
                    hit.terminator = 0;
                    hit.unknown0x04 = *(u32*)(missileBytes + 0x40);
                    hit.unknown0x08 = *(u16*)(missileBytes + 0x4A);
                    hit.unknown0x20 = *(u16*)(missileBytes + 0x4C);
                    hit.unknown0x1C = *(u8*)(missileBytes + 0x54);
                    hit.hit_type = 10;
                    hitData = EQ_ARRAY(char*, 0x005FE9A0)[*(int*)(missileBytes + 0x4C)];
                    if (hitData == 0)
                    {
                        hit.hit_data_float = 0.0f;
                    }
                    else
                    {
                        hit.hit_data = *(u32*)(hitData + 0x1A8);
                    }
                    hit.unknown0x14 = *(u32*)(missileBytes + 0x18);
                    hit.unknown0x1D = 0;
                    hit.unknown0x18 = *(u32*)(missileBytes + 0x1C);
                    hit.unknown0x1E = 0;
                    EQ_METHOD(1, void, 0x004B3C9D, (EQPlayer*, void*), player)(&hit);
                }
            }
        }
        else
        {
            float position[3];

            position[0] = *(float*)(missileBytes + 0x0C) =
                *(float*)(missileBytes + 0x0C) - *(float*)(missileBytes + 0x24);
            position[1] = *(float*)(missileBytes + 0x10) =
                *(float*)(missileBytes + 0x10) - *(float*)(missileBytes + 0x28);
            position[2] = *(float*)(missileBytes + 0x14) =
                *(float*)(missileBytes + 0x14) - *(float*)(missileBytes + 0x2C);
            EQ_CDECL(void, 0x004C85F4, (int, u32, void*, int, float*, int, int))(
                2, EQ_GLOBAL(u32, 0x005FEA78), *(void**)((char*)self->actor_info + 0x2A0), 0,
                position, 0, 0);
        }
    }

    missileInfo = self->actor_info;
    if (missileInfo != 0)
    {
        EQ_METHOD(0, void, 0x004BD18D, (void*), missileInfo)();
        EQ_CDECL(void, 0x005218BB, (void*))(missileInfo);
        self->actor_info = 0;
    }
    EQ_GLOBAL(EQMissile_CriticalSectionFn_guess, 0x00534190)(EQ_ARRAY(void, 0x005FE7A0));
}

// FUNCTION: TRILOGY 0x004ad088
// Removes the effects a missile owns; for spell 0x35B (missile +0x4C) it only detaches them.
// __thiscall-style (ECX = missile), called from EQMissile::~EQMissile_guess. Walks the effect list at
// 0x006E4D74 (next at +0x54); effects owned by `m` (+0x5C) are detached when m's +0x4C is 0x35B,
// otherwise destroyed (~EQEffect 0x00490EF4, which unlinks) and freed.
void __fastcall EQMissile_CleanupMissileEffects_guess(EQMissile_guess* m)
{
    EQEffect_guess* e;
    EQEffect_guess* next;

    e = g_effect_head_guess;
    while (e != 0)
    {
        next = e->next;
        if ((char*)m == e->attachedSource_char_guess)
        {
            if (m->spellId == 0x35B)
            {
                e->attachedSource_char_guess = 0;
            }
            else if (e != 0)
            {
                EQ_METHOD(0, void, 0x00490EF4, (void*), e)();
                EQ_CDECL(void, 0x005218BB, (void*))(e);
            }
        }
        e = next;
    }
}

// FUNCTION: TRILOGY 0x004ad0ca
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMissile.cpp (Mac: EQMissile::is_missile_actor)
// __thiscall that ignores `this` (no direct callers; ret 4). The missile whose t3d actor is `actor`, or 0.
EQMissile_guess* EQMissile_guess::is_missile_actor(void* actor)
{
    EQMissile_guess* m;

    for (m = g_missile_top_guess; m != 0; m = m->next)
    {
        if (*m->actor_info == actor)
            return m;
    }
    return 0;
}

class EQMissile_HitActor_guess : public EQMissile_guess
{
public:
    void HitActor(void* actor, int unused);
};

// FUNCTION: TRILOGY 0x004ad0e8
// Announces when a sentinel effect triggers on a player.
// ORIGINAL FILE: EverQuest/EQMissile.cpp (from Mac debug map)
void EQMissile_HitActor_guess::HitActor(void* actor, int unused)
{
    int spellId;
    char found;
    void* player;
    char message[128];

    if (actor != 0 &&
        *(char*)(*(int*)((char*)this + 0x3c) + 0x2c0) != '\0' &&
        *(char*)((char*)this + 0x53) == '\t')
    {
        player = EQ_METHOD(1, void*, 0x004B14BF, (void*, void*), EQ_GLOBAL(void*, 0x006E4D58))(actor);
        if (player != 0 &&
            (spellId = this->spellId) > 0 &&
            spellId < 3000 &&
            EQ_ARRAY(void*, 0x005FE9A0)[spellId] != 0)
        {
            found = EQ_CDECL(char, 0x0048A11F, (void*, int))
                (EQ_ARRAY(void*, 0x005FE9A0)[spellId], 0x4c);
            if (found != '\0')
            {
                if (player == EQ_GLOBAL(void*, 0x006E4D8C))
                {
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (message, EQ_ARRAY(char, 0x005664C0));
                }
                else
                {
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                        (message, EQ_ARRAY(char, 0x005664A0),
                         EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))((char*)player + 1));
                }
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(message, 4);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004ad1a4
// Spawns a launched projectile's model, light and spell effect in the world once its release delay has passed
// ORIGINAL FILE: EverQuest/EQMissile.cpp (from Mac debug map)
// @early-stop: 3 instruction differences remain; first difference: #284: original `mov esi, dword ptr [esi + 0x3c]` | rewrite `mov eax, dword ptr [esi + 0x3c]`.
void __fastcall EQMissile_ProcessMissile_guess(EQMissile_guess* missile, int edx)
{
    u16 ownerRace;
    char posOk;
    u32 now;
    int result;
    int tmp;
    int scratch;
    int* actorInfo;
    float sine;
    float ownerPos[3];
    int effectType;
    int spell;
    u32 elapsed;
    float elapsedFactor;

    tmp = *(int*)((char*)missile + 0x3c);
    now = EQ_CDECL(u32, 0x004A461E, (void))();
    if (now < *(u32*)((char*)tmp + 0x2c4))
        return;
    if (tmp == 0)
        return;

    spell = 0;
    effectType = 1;
    if (*(int*)((char*)tmp + 0x2a0) == 0)
    {
        result = (int)EQ_METHOD(1, void*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(*(int*)((char*)missile + 0x40));
        *(int*)((char*)missile->actor_info + 0x2a0) = result;
        actorInfo = (int*)missile->actor_info;
        if (actorInfo[0xa8] == 0)
        {
            *(u8*)((char*)actorInfo + 0x2c1) = 1;
            return;
        }
    }

    tmp = EQ_CDECL(int, 0x004A461E, (void))();
    *(int*)((char*)missile->actor_info + 0x68) = tmp + 120000;
    if (*(char*)((char*)missile + 0x53) == '\t')
    {
        int invalidSpell = missile->spellId >= 3000 ||
            (spell = (int)EQ_ARRAY(void*, 0x005FE9A0)[missile->spellId], spell == 0);
        if (invalidSpell)
        {
            *(u8*)((char*)missile->actor_info + 0x2c1) = 1;
            return;
        }
        if (*(u32*)((char*)spell + 0x1c4) > 0)
        {
            scratch = EQ_CDECL(int, 0x004A461E, (void))();
            scratch = scratch + *(int*)((char*)spell + 0x1c4);
        }
        else if (*(char*)((char*)spell + 0x256) == '\x03')
        {
            scratch = EQ_CDECL(int, 0x004A461E, (void))();
            scratch = scratch + 30000;
        }
        else
        {
            scratch = EQ_CDECL(int, 0x004A461E, (void))();
            scratch = scratch + 0x32;
        }
        *(int*)((char*)missile->actor_info + 0x68) = scratch;
        if (*(char*)((char*)spell + 0x22e) != '\x08')
        {
            tmp = *(int*)((char*)missile + 0x3c);
            effectType = 1;
            scratch = *(int*)((char*)tmp + 0x2a0);
            elapsed = EQ_CDECL(u32, 0x004A461E, (void))() - *(int*)(*(int*)((char*)scratch + 0x70) + 0x54);
            ownerRace = *(u16*)((char*)scratch + 0x9a);
            elapsedFactor = (float)elapsed * 0.02f;
            switch (ownerRace)
            {
            case 0x31:
            case 0x9e:
            case 0xc4:
                posOk = EQ_METHOD(2, char, 0x00413F8F, (void*, void*, float*), EQ_GLOBAL(void*, 0x006E4DE0))((void*)*(int*)((char*)tmp + 0x2a0), &ownerPos[0]);
                if (posOk == '\x01')
                {
                    missile->y = missile->velocityY * 32.5f + ownerPos[0];
                    missile->x = missile->velocityX * 32.5f + ownerPos[1];
                    missile->z = ownerPos[2] - 12.0f;
                }
                *(int*)((char*)missile + 0x38) = 0xc2900000;
                sine = t3dFloatFastSine(-72.0f);
                missile->velocityZ = sine * missile->forwardSpeed_guess + missile->velocityZ;
                break;
            }
            if (*(char*)((char*)spell + 0x256) == '\x03')
            {
                missile->velocityZ = 0.0f;
            }
            else
            {
                float fProduct;

                fProduct = elapsedFactor * missile->velocityY;
                missile->y = fProduct + missile->velocityY * 5.5f +
                    missile->y;
                fProduct = elapsedFactor * missile->velocityX;
                missile->x = fProduct + missile->velocityX * 5.5f +
                    missile->x;
                fProduct = elapsedFactor * missile->velocityZ;
                missile->z = fProduct + missile->velocityZ * 5.5f +
                    missile->z;
            }
        }
        else
        {
            tmp = (int)EQ_METHOD(1, void*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(*(int*)((char*)missile + 0x44));
            if (tmp != 0)
            {
                *(int*)((char*)missile + 0x0c) = *(int*)((char*)tmp + 0x34);
                *(int*)((char*)missile + 0x10) = *(int*)((char*)tmp + 0x38);
                *(int*)((char*)missile + 0x14) = *(int*)((char*)tmp + 0x3c);
            }
            effectType = 2;
            missile->velocityY = 0.0f;
            missile->velocityX = 0.0f;
            missile->velocityZ = 0.0f;
            missile->forwardSpeed_guess = 0.0f;
            *(u8*)((char*)missile->actor_info + 0x88) = 0;
        }
    }
    else
    {
        missile->y = missile->velocityY + missile->y;
        missile->x = missile->velocityX + missile->x;
        missile->z = missile->velocityZ + missile->z;
    }

    {
        float fScale;

        fScale = 1.0f;
        if (*(char*)((char*)missile + 0x52) == '\0')
        {
            tmp = *(int*)((char*)missile->actor_info + 0x2a0);
            fScale = 1.5f;
            if (tmp != 0)
            {
                switch (*(u16*)((char*)tmp + 0x9a))
                {
                case 0x31:
                case 0x9e:
                case 0xc4:
                    fScale = 20.5f;
                    break;
                }
            }
        }
        else
        {
            tmp = *(int*)((char*)missile->actor_info + 0x2a0);
            if (tmp != 0)
            {
                switch (*(u16*)((char*)tmp + 0x9a))
                {
                case 0x31:
                case 0x9e:
                case 0xc4:
                    fScale = 1.0f;
                    break;
                }
            }
        }

        *(void**)missile->actor_info = EQ_METHOD(9, void*, 0x004112DF, (void*, const char*, float, float, float, float, float, float, int, int), EQ_GLOBAL(void*, 0x006E4DE0))((const char*)((char*)missile + 0x55),
             missile->y, missile->x,
             missile->z, missile->heading,
             missile->pitch, fScale, 1, 0);
    }
    actorInfo = (int*)missile->actor_info;
    if (*actorInfo == 0)
    {
        *(u8*)((char*)actorInfo + 0x2c1) = 1;
        return;
    }
    t3dSetActorUserData((void*)*actorInfo, missile);
    if (missile->light_guess != '\0')
    {
        result = (int)EQ_METHOD(5, void*, 0x00410725, (void*, char, float, float, float, float), EQ_GLOBAL(void*, 0x006E4DE0))(missile->light_guess, 0.0f,
             missile->y, missile->x,
             missile->z);
        *(int*)((char*)missile->actor_info + 4) = result;
    }
    else
    {
        *(int*)((char*)missile->actor_info + 4) = 0;
    }
    if (spell != 0)
    {
        EQ_CDECL(void, 0x004C85F4, (int, int, void*, int, int, void*, int))
            (effectType, spell, *(void**)((char*)missile->actor_info + 0x2a0), 0, 0,
             missile, *(int*)((char*)spell + 0x1c4));
    }
    *(int*)((char*)missile->actor_info + 0x2a8) = *(int*)((char*)missile + 0x18);
    *(int*)((char*)missile->actor_info + 0x2ac) = *(int*)((char*)missile + 0x38);
    *(int*)((char*)missile->actor_info + 0x2b0) = 0x44fa0000;
    result = EQ_CDECL(u32, 0x004A461E, (void))();
    *(u32*)((char*)missile->actor_info + 0x54) = result;
    *(u32*)((char*)missile->actor_info + 0xd0) = *(u32*)((char*)missile->actor_info + 0x54);
    actorInfo = (int*)missile->actor_info;
    if (*(char*)((char*)missile + 0x53) == '\t')
    {
        actorInfo[0x34] += 2000;
        return;
    }
    actorInfo[0x34] += 500;
}

typedef float (__cdecl* T3dArcTangentFn_guess)(float, float, float*);

// FUNCTION: TRILOGY 0x004ad574
// Guides a homing projectile toward its target, re-aiming more often as it closes in, and strikes on contact
// ORIGINAL FILE: EverQuest/EQMissile.cpp (from Mac debug map)
// __thiscall (ECX = missile). Homing AI: steers the missile's heading (+0x18) and pitch (+0x38) toward
// the target player kept in the actor info (+0x2a4), and hits the target when it is close enough.
// @early-stop: 16.0 (deltaZ) is constant-propagated and hoisted to the FPU; target is loaded into edi not eax+copy.
void __fastcall EQMissile_MissileAI_guess(EQMissile_guess* missile, int edx)
{
    float pitchDiff;
    float deltaZ;
    float headingDiff;
    float tmpFloat;
    float absDiff;
    u32 updateInterval;
    char* info;
    char* target;
    float step;
    float* pitchPtr;
    u32 now;

    updateInterval = 0;
    if (EQ_GLOBAL(char, 0x006B63F8) != '\0')
    {
        *(int*)((char*)missile->actor_info + 0x2a4) = EQ_GLOBAL(int, 0x006E4DAC);
    }
    info = (char*)missile->actor_info;
    if (info != 0 && missile->forwardSpeed_guess != 0.0f &&
        (target = *(char**)(info + 0x2a4)) != 0 && *(u8*)(target + 0xa1) != 'x')
    {
        if (*(int*)(target + 0x80) == *(int*)((char*)missile + 0x40) && EQ_GLOBAL(char, 0x006B63F8) == '\0')
        {
            *(int*)(info + 0x2a4) = 0;
        }
        else
        {
            if (*(float*)(info + 0x2b0) < 60.0f)
            {
                if (*(float*)(info + 0x2b0) < 2.0f && *(int**)(target + 0x70) != 0)
                {
                    EQ_METHOD(2, void, 0x004AD0E8, (void*, void*, int), missile)((void*)**(int**)(target + 0x70), 0);
                    *(u8*)((char*)missile->actor_info + 0x2c1) = 1;
                    return;
                }
            }
            else if (*(float*)(info + 0x2b0) < 150.0f)
            {
                updateInterval = 0x7d;
            }
            else if (*(float*)(info + 0x2b0) < 250.0f)
            {
                updateInterval = 0xfa;
            }
            else
            {
                updateInterval = 500;
                if (!(*(float*)(info + 0x2b0) < 350.0f))
                {
                    updateInterval = 1000;
                }
            }
            now = g_cdisplay_guess->time_ms_guess;
            if ((u32)(now - *(u32*)(info + 0xd0)) > updateInterval)
            {
                tmpFloat = *(float*)(target + 0x7c) * 0.5f + *(float*)(target + 0x3c);
                deltaZ = tmpFloat - missile->z;
                absDiff = deltaZ;
                if (deltaZ < 0.0f)
                {
                    absDiff = -deltaZ;
                }
                *(u32*)(info + 0xd0) = now;
                target = *(char**)((char*)missile->actor_info + 0x2a4);
                *(float*)((char*)missile->actor_info + 0x2a8) =
                    EQ_CDECL(float, 0x004A47F9, (float, float, float, float))
                        (missile->y, missile->x,
                         *(float*)(target + 0x34), *(float*)(target + 0x38));
                target = *(char**)((char*)missile->actor_info + 0x2a4);
                *(float*)((char*)missile->actor_info + 0x2b0) =
                    EQ_METHOD(7, float, 0x004129F3, (void*, float, float, float, float, float, float, float), EQ_GLOBAL(void*, 0x006E4DE0))(missile->y,
                         missile->x, missile->z,
                         *(float*)(target + 0x34), *(float*)(target + 0x38), tmpFloat, 0.0f);
                if (absDiff > 0.5f)
                {
                    tmpFloat = 0.0f;
                    *(float*)((char*)missile->actor_info + 0x2ac) =
                        128.0f - EQ_GLOBAL(T3dArcTangentFn_guess, 0x006E515C)
                            (*(float*)((char*)missile->actor_info + 0x2b0), absDiff, &tmpFloat);
                    if (deltaZ < 0.0f)
                    {
                        pitchPtr = (float*)((char*)missile->actor_info + 0x2ac);
                        *pitchPtr = -*pitchPtr;
                    }
                }
                else
                {
                    *(float*)((char*)missile->actor_info + 0x2ac) = 0.0f;
                }
            }
            info = (char*)missile->actor_info;
            if (*(float*)(info + 0x2b0) < 32.0f)
            {
                missile->heading_bits_guess = *(u32*)(info + 0x2a8);
                missile->flightPitch_bits_guess = *(u32*)(info + 0x2ac);
                return;
            }
            else
            {
                deltaZ = 16.0f;
                absDiff = EQ_METHOD(3, float, 0x004081A3, (void*, float, float, float*), EQ_GLOBAL(void*, 0x006E4DE0))(missile->heading,
                     *(float*)(info + 0x2a8), &headingDiff);
                tmpFloat = EQ_METHOD(3, float, 0x004081A3, (void*, float, float, float*), EQ_GLOBAL(void*, 0x006E4DE0))(missile->flightPitch_guess,
                     *(float*)((char*)missile->actor_info + 0x2ac), &pitchDiff);
                if (absDiff < 1.0f)
                {
                    missile->heading_bits_guess = *(u32*)((char*)missile->actor_info + 0x2a8);
                }
                else
                {
                    step = deltaZ;
                    if (absDiff < 16.0f)
                    {
                        step = absDiff;
                    }
                    if ((missile->heading < *(float*)((char*)missile->actor_info + 0x2a8) &&
                         headingDiff < 0.0f) ||
                        (*(float*)((char*)missile->actor_info + 0x2a8) < missile->heading &&
                         headingDiff > 0.0f))
                    {
                        missile->heading = missile->heading - step;
                    }
                    else
                    {
                        missile->heading = step + missile->heading;
                    }
                    missile->heading =
                        EQ_METHOD(1, float, 0x00408156, (void*, float), EQ_GLOBAL(void*, 0x006E4DE0))(missile->heading);
                }
                if (tmpFloat < 1.0f)
                {
                    missile->flightPitch_bits_guess = *(u32*)((char*)missile->actor_info + 0x2ac);
                }
                else
                {
                    if (tmpFloat < 16.0f)
                    {
                        return;
                    }
                    if (pitchDiff < 0.0f ||
                        *(float*)((char*)missile->actor_info + 0x2ac) < missile->flightPitch_guess)
                    {
                        step = missile->flightPitch_guess - 16.0f;
                    }
                    else
                    {
                        step = missile->flightPitch_guess + 16.0f;
                    }
                    missile->flightPitch_guess = step;
                    missile->flightPitch_guess =
                        EQ_METHOD(1, float, 0x00408156, (void*, float), EQ_GLOBAL(void*, 0x006E4DE0))(missile->flightPitch_guess);
                    return;
                }
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004b4570
// Removes this player's missiles from flight and clears missiles targeting them.
void __fastcall EQPlayer__EQPlayer2__EQMissile_guess(EQPlayer* player)
{
    EQMissile_guess* missile;
    EQMissile_guess* next;
    u32* follow_target;

    missile = g_missile_top_guess;
    while (missile != 0)
    {
        next = missile->next;
        if (missile->targetId_guess == player->spawn_id)
        {
            missile->targetId_guess = 0;
        }
        if (missile->actor_info != 0)
        {
            follow_target = (u32*)((char*)missile->actor_info + 0x2A4);
            if (*follow_target == (u32)player)
            {
                *follow_target = 0;
            }
        }
        if (missile->sourceId_guess == player->spawn_id && missile != 0)
        {
            EQ_METHOD(0, void, 0x004ACEAD, (void*), missile)();
            EQ_CDECL(void, 0x005218BB, (void*))(missile);
        }
        missile = next;
    }
}

