// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EQPlayer.cpp - EQPlayer, the spawn object (one per player, NPC and corpse in the zone).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (from Mac debug map)

#include <string.h>

#include "trilogy/common.h"
#include "trilogy/player.h"
#include "trilogy/character.h"
#include "trilogy/eqplayer.h"
#include "trilogy/eqswitch.h"
#include "trilogy/item.h"
#include "trilogy/globals.h"
#include "trilogy/spell.h"
#include "trilogy/display.h"
#include "trilogy/eqmissile.h"
#include <stdio.h>
#include <stdlib.h>

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef EQTRACE_guess
// EXTERNAL: TRILOGY 0x004A3D01 EQTRACE_guess
void __cdecl EQTRACE_guess(const char* format, ...);
#undef LogMessage_guess
// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char* format, ...);
#define LogMessage_guess DbgLog_guess
#undef send_message_guess
// EXTERNAL: TRILOGY 0x004E1C45 send_message_guess
void __cdecl send_message_guess(void** connection, u32 opcode, void* data, u32 size, int reliable);

// The 0x124-byte inventory item a spawn allocates (fallback equipment, ammunition split off a stack).
class PlayerEquipment_guess : public EQ_Equipment
{
public:
    // EXTERNAL: TRILOGY 0x00436099 PlayerEquipment_guess::PlayerEquipment_guess
    PlayerEquipment_guess();
};

// The 0x364-byte actor info a spawn builds for its 3D actor.
class PlayerActorInfo_guess : public EQActorInfo_guess
{
public:
    // EXTERNAL: TRILOGY 0x004BCDE8 PlayerActorInfo_guess::PlayerActorInfo_guess
    PlayerActorInfo_guess(const char* actorTag);
};

// FUNCTION: TRILOGY 0x00410d13
// Plays a sound at a position in the zone.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void __stdcall EQPlayer_PlaySoundA_guess(float y, float x, float z, int soundId)
{
    void* environment;

    environment = g_environment_ptr_guess;
    if (environment != 0)
    {
        EQ_METHOD(4, void, 0x004E286F, (void*, float, float, float, int), environment)(y, x, z, soundId);
    }
}

typedef void (__stdcall *CriticalSectionFunction_guess)(void* section);

// FUNCTION: TRILOGY 0x004b03eb
// Constructs a spawn from an image spawn or defaults, links it into the spawn list and creates its 3D actor
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// @early-stop: frame size and EH cleanup, appearance stores, and several loop/register choices still differ.
/* fake match: float alias reproduces the x87 zero store. */
EQPlayer* __fastcall EQPlayer_EQPlayer_guess(EQPlayer* self, int edx_unused,
                                              EQPlayer* image, u8 genderId, u16 raceId,
                                              u8 classId, const char* fallbackName)
{
    int comparison;
    float floorHeight;
    u32 lightArgument;
    EQPlayer* head;
    CharacterProfile* profile;
    EQActorInfo_guess* actorInfo;

    memset(self, 0, 0x134);
    if (fallbackName != 0)
    {
        strcpy(self->name, fallbackName);
    }
    if (image != 0)
    {
        strcpy(self->name, image->name);
    }
    self->char_info_guess = 0;
    self->pet_owner_id = 0;
    self->gm = 0;
    self->unk_0x108 = 0;
    self->deity = 0;
    self->weaponAnimationType = 0;
    self->appearance[0] = 0xff;
    self->appearance[1] = 0xff;
    self->appearance[2] = 0xff;
    self->appearance[3] = 0xff;
    self->appearance[4] = 0xff;
    self->appearance[5] = 0xff;
    EQ_GLOBAL(CriticalSectionFunction_guess, 0x00534194)((void*)0x006B3298);
    head = EQ_GLOBAL(EQPlayer*, 0x006E4D58);
    if (head == 0)
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4D58) = self;
        self->next = 0;
        self->previous_bits_guess = 0;
    }
    else
    {
        head->previous_bits_guess = (u32)self;
        head = EQ_GLOBAL(EQPlayer*, 0x006E4D58);
        self->next = head;
        self->previous_bits_guess = 0;
        EQ_GLOBAL(EQPlayer*, 0x006E4D58) = self;
    }
    self->size = -1.0f;
    if (g_pc_guess != 0)
    {
        if (_strcmpi(self->name, g_pc_guess->name) == 0)
        {
            profile = g_pc_guess;
            self->char_info_guess = profile;
            if (profile == 0)
            {
                EQTRACE_guess((const char*)0x00568340, self->name);
            }
            else
            {
                profile->player = self;
            }
        }
    }
    if (EQ_GLOBAL(int, 0x005FE850) == 0)
    {
        EQ_GLOBAL(EQ_Equipment*, 0x005FE850) = new PlayerEquipment_guess;
    }
    self->texture = 0xff;
    self->helm = 0xff;
    self->unk_0x78 = 0x40b00000;
    *(u32*)self->unknown0xDC = 0x40400000;
    *(u32*)(self->unknown0xDC + 4) = 0x40200000;
    self->walk_speed = 0.46f;
    self->run_speed = 0.7f;
    if (image == 0)
    {
        if (EQ_GLOBAL(char, 0x006B63F8) == 0 && DAT_006b6494 == 0)
        {
            self->spawn_id = 0;
        }
        else
        {
            self->spawn_id = EQ_GLOBAL(u32, 0x0056830C)++;
        }
        self->guild_id = -1;
        self->levitate = 0;
        self->body_type = 1;
        self->y = 0.0f;
        self->gender = genderId;
        self->x = 0.0f;
        self->race = raceId;
        self->heading = 0.0f;
        self->class_ = classId;
        self->pitch = 0.0f;
        self->z = 9.0f;
        self->speed = 0.0f;
        self->max_hp = 0x32;
        self->dy = 0.0f;
        self->cur_hp = 0x32;
        self->linkdead = 0;
        self->dx = 0.0f;
        self->invis = 0;
        self->sneaking = 0;
        self->dz = 0.0f;
        self->stand_state = 'd';
        self->light = 0;
        self->delta_heading = 0.0f;
        self->pvp = 0;
        self->face = 0;
        self->unk_0x5C = 0.0f;
        u8* equipmentSlot;
        u32* colorSlot;
        int slot;
        equipmentSlot = self->equipment;
        colorSlot = self->equip_colors;
        *(float*)&self->unk_0x60 = 0.0f;
        slot = 9;
        do
        {
            *equipmentSlot = 0;
            *colorSlot = 0;
            ++colorSlot;
            ++equipmentSlot;
            --slot;
        } while (slot != 0);
        *(u32*)self->unknown0x10C = 0xffffffff;
        self->afk = 0;
        self->type = 0;
        self->anon = 0;
    }
    else
    {
        self->weaponAnimationType = image->weaponAnimationType;
        self->appearance[0] = image->appearance[0];
        self->appearance[1] = image->appearance[1];
        self->appearance[2] = image->appearance[2];
        self->appearance[3] = image->appearance[3];
        self->appearance[4] = image->appearance[4];
        self->appearance[5] = image->appearance[5];
        self->deity = image->deity;
        self->gm = image->gm;
        self->levitate = image->levitate;
        self->body_type = image->body_type;
        self->spawn_id = image->spawn_id;
        self->linkdead = image->linkdead;
        self->pet_owner_id = image->pet_owner_id;
        self->y = image->y;
        self->x = image->x;
        self->z = image->z;
        self->heading = image->heading;
        self->pitch = image->pitch;
        self->speed = image->speed;
        self->dy = image->dy;
        self->dx = image->dx;
        self->dz = image->dz;
        self->delta_heading = image->delta_heading;
        self->unk_0x5C = image->unk_0x5C;
        self->unk_0x60 = image->unk_0x60;
        self->type = image->type;
        self->class_ = image->class_;
        self->race = image->race;
        self->gender = image->gender;
        self->level = image->level;
        self->walk_speed = image->walk_speed;
        self->run_speed = image->run_speed;
        strcpy(self->name, image->name);
        strcpy(self->last_name, image->last_name);
        strcpy(self->zone, image->zone);
        self->max_hp = image->max_hp;
        self->cur_hp = image->cur_hp;
        self->invis = image->invis;
        self->sneaking = image->sneaking;
        self->pvp = image->pvp;
        self->stand_state = image->stand_state;
        self->light = image->light;
        self->guild_id = image->guild_id;
        self->guild_rank = image->guild_rank;
        self->face = image->face;
        self->texture = image->texture;
        self->helm = image->helm;
        self->size = image->size;
        *(u32*)self->unknown0xDC = *(u32*)image->unknown0xDC;
        *(u32*)(self->unknown0xDC + 4) = *(u32*)(image->unknown0xDC + 4);
        u8* equipmentSlot;
        u32* colorSlot;
        u8* sourceEquipmentSlot;
        u32* sourceColorSlot;
        int slot;
        equipmentSlot = self->equipment;
        colorSlot = self->equip_colors;
        sourceEquipmentSlot = image->equipment;
        sourceColorSlot = image->equip_colors;
        slot = 9;
        do
        {
            *equipmentSlot = *sourceEquipmentSlot;
            *colorSlot = *sourceColorSlot;
            ++colorSlot;
            ++equipmentSlot;
            ++sourceColorSlot;
            ++sourceEquipmentSlot;
            --slot;
        } while (slot != 0);
        self->afk = image->afk;
        self->anon = image->anon;
        *(u32*)self->unknown0x10C = *(u32*)image->unknown0x10C;
    }
    *(u32*)&self->unknown0x92[2] = 0;
    self->unknown0xF8 = 0;
    *(u16*)&self->unknown0x100 = 0;
    self->objectKind_guess = 3;
    {
        char actorTag[40];
        EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), self)(actorTag);
        strcat(actorTag, (const char*)0x0053E4B0);
        actorInfo = new PlayerActorInfo_guess(actorTag);
    }
    self->actor_info = actorInfo;
    EQ_METHOD(0, void, 0x004BC9BA, (EQPlayer*), self)();
    self->actor_info->unk_0x68 = 0;
    EQ_METHOD(1, void, 0x0040F731, (void*, EQPlayer*),
        g_display_guess)(self);
    floorHeight = EQ_METHOD(3, float, 0x00414245,
        (void*, float, float, float), g_display_guess)(self->y, self->x, self->z);
    self->actor_info->floor_z = floorHeight;
    self->actor_info->fallStartZ_guess = EQ_METHOD(4, float, 0x00414468,
        (void*, float, float, float, float), g_display_guess)
        (self->y, self->x, self->z, *(float*)EQ_ARRAY(float, 0x005344A8));
    lightArgument = (u32)self->actor_info & 0xFFFF0000;
    if (self->actor_info->floor_z < *(float*)EQ_ARRAY(float, 0x005351F4))
    {
        lightArgument |= 0x100;
    }
    if (self->actor_info->floor_z != self->actor_info->floor_z)
    {
        lightArgument |= 0x400;
    }
    if (self->actor_info->floor_z == *(float*)EQ_ARRAY(float, 0x005351F4))
    {
        EQTRACE_guess((const char*)0x005454C8, self->name);
        self->actor_info->floor_z = 1.0f;
        lightArgument = (u32)self->actor_info;
    }
    if (self->levitate != 0)
    {
        self->actor_info->unknown0x088 = 0;
    }
    if (self->char_info_guess != 0 && self->type == 0)
    {
        *(u8*)&lightArgument = self->light;
        EQ_METHOD(1, void, 0x00419826, (void*, int), self->char_info_guess)
            ((int)lightArgument);
        self->gm = ((CharacterProfile*)self->char_info_guess)->gm_flag;
    }
    EQ_METHOD(1, void, 0x00410602, (void*, EQPlayer*),
        g_display_guess)(self);
    if (self->race < 0xD6)
    {
        EQ_METHOD(0, void, 0x004B0D95, (EQPlayer*), self)();
        EQ_METHOD(0, void, 0x004B4873, (EQPlayer*), self)();
    }
    ((CDisplay*)g_display_guess)->unk_0x124 = 0;
    EQ_GLOBAL(CriticalSectionFunction_guess, 0x00534190)((void*)0x006B3298);
    if (self->type == 1 && EQ_GLOBAL(int, 0x006E4D4C) != 0 && self->actor_info != 0)
    {
        self->levitate = 1;
        self->actor_info->unknown0x088 = 0;
    }
    actorInfo = self->actor_info;
    if (actorInfo != 0)
    {
        if (self->race == 0x8D)
        {
            self->levitate = 2;
            actorInfo->unknown0x088 = 0;
            actorInfo->unknown0x078 = 1;
            actorInfo->unk_0x7C = 3;
            actorInfo->unk_0x84 = 0x3BA3D70A;
        }
        else if (self->race == 0x48 || self->race == 0x49 || self->race == 0x72)
        {
            actorInfo->unknown0x088 = 0;
        }
        if (self->char_info_guess != 0)
        {
            const u32 now = EQ_CDECL(u32, 0x004A461E, (void))();
            actorInfo = self->actor_info;
            profile = (CharacterProfile*)self->char_info_guess;
            int slot;
            int comparison;
            slot = 0xD4;
            comparison = 10;
            do
            {
                *(u32*)((char*)actorInfo + slot) =
                    *(u32*)((char*)self->char_info_guess + 0xF7C + slot) + now;
                slot += 4;
                --comparison;
            } while (comparison != 0);
            EQ_GLOBAL(u32, 0x006E106C) = *(u32*)profile->unknown0x0ABC + now;
            EQTRACE_guess((const char*)0x00568310, now, EQ_GLOBAL(u32, 0x006E106C),
                 *(u32*)profile->unknown0x0ABC);
            EQ_GLOBAL(u8, 0x006E119C) = 0;
            actorInfo->unk_0x15C = actorInfo->unk_0xD4[8];
        }
    }
    self->unk_0x60 = 0;
    if (self->char_info_guess != 0 && self->type == 0)
    {
        self->unk_0x60 = EQ_GLOBAL(u32, 0x006B6858);
        EQ_METHOD(1, void, 0x004226C1, (void*, int), self->char_info_guess)(1);
    }
    return self;
}

// FUNCTION: TRILOGY 0x004b0b9f
// Removes a spawn from the zone and releases its associated client-side state.
void __fastcall Destroy_guess(EQPlayer* player)
{
    EQPlayer* nextPlayer;
    u32* record;
    int index;
    void* soundManager;
    void* freeBlock;
    EQActorInfo_guess* actorInfo;
    void* affect;
    char* clientBase;
    void* criticalSection;

    CDisplay_SetNameSpriteState_guess(g_cdisplay_guess, player, 0);
    soundManager = g_environment_ptr_guess;
    if (soundManager != 0)
    {
        EQ_METHOD(1, bool, 0x004E25B5, (void*, EQPlayer*), soundManager)(player);
    }
    criticalSection = (void*)0x006b3298;
    EQ_GLOBAL(CriticalSectionFunction_guess, 0x00534194)(criticalSection);
    if (player->previous_bits_guess == 0)
    {
        if (player->next != 0)
        {
            player->next->previous_bits_guess = 0;
            g_player_top_guess = player->next;
            goto unlink_done;
        }
    }
    else
    {
        if (player->next != 0)
        {
            ((EQPlayer*)player->previous_bits_guess)->next = player->next;
            player->next->previous_bits_guess = player->previous_bits_guess;
            goto unlink_done;
        }
        if (player->previous_bits_guess == 0)
            goto unlink_head;
    }
unlink_head:
    if (player->next != 0)
    {
        player->next->previous_bits_guess = 0;
        g_player_top_guess = player->next;
        goto unlink_done;
    }
    nextPlayer = player->next;
    if (nextPlayer != 0)
    {
        nextPlayer->previous_bits_guess = 0;
        g_player_top_guess = player->next;
        goto unlink_done;
    }
    if (player->next == 0 && player->previous_bits_guess != 0)
    {
        ((EQPlayer*)player->previous_bits_guess)->next = 0;
    }
    else
    {
        player->next = 0;
        player->previous_bits_guess = 0;
        g_player_top_guess = 0;
        freeBlock = EQ_GLOBAL(void*, 0x005FE850);
        if (freeBlock != 0)
        {
            operator delete(freeBlock);
            EQ_GLOBAL(void*, 0x005FE850) = 0;
        }
    }
unlink_done:
    if (player->race == 0x48 || player->race == 0x49 || player->race == 0x8d || player->race == 0x72)
    {
        EQ_METHOD(0, void, 0x004B13C0, (EQPlayer*), player)();
    }
    EQ_METHOD(0, void, 0x004B4570, (EQPlayer*), player)();
    EQ_METHOD(0, void, 0x004B13E3, (EQPlayer*), player)();
    if (player->char_info_guess != 0)
    {
        ((CharacterProfile*)player->char_info_guess)->player = 0;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == player)
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = 0;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006B6780) == player)
    {
        EQ_GLOBAL(short, 0x006B6772) = -1;
        EQ_GLOBAL(EQPlayer*, 0x006B6780) = 0;
    }
    if (player == g_control_player_guess)
    {
        if (player == g_local_player_guess)
        {
            g_control_player_guess = 0;
            g_local_player_guess = 0;
        }
        else
        {
            if (g_pc_guess != 0)
            {
                affect = EQ_METHOD(2, void*, 0x0042381B, (CharacterProfile*, u8, short*), g_pc_guess)(0x43, 0);
                EQ_METHOD(1, void, 0x00424E22, (CharacterProfile*, void*), g_pc_guess)(affect);
            }
            g_control_player_guess = g_local_player_guess;
        }
    }
    if (player->type == 0)
    {
        for (index = 0; index < 5; index++)
        {
            if (EQ_ARRAY(EQPlayer*, 0x005FE954)[index] == player)
            {
                EQ_ARRAY(EQPlayer*, 0x005FE954)[index] = 0;
            }
        }
    }
    index = 0;
    clientBase = g_eqmain_guess;
    record = (u32*)(clientBase + 0x685f8);
    while (clientBase == 0 || (EQPlayer*)record[0] != player)
    {
        index++;
        record += 4;
        if (index > 999)
            goto cleanup;
    }
    *(u32*)(clientBase + index * 0x10 + 0x685f8) = 0;
    *(float*)(clientBase + index * 0x10 + 0x685fc) = -1.0f;
    *(u32*)(clientBase + (index + 0x6860) * 0x10) = 0xffffffff;
    *(u32*)(clientBase + index * 0x10 + 0x68604) = 0;
cleanup:
    EQ_METHOD(0, void, 0x004B0EB4, (EQPlayer*), player)();
    actorInfo = player->actor_info;
    if (actorInfo != 0)
    {
        EQ_METHOD(0, void, 0x004BD18D, (EQActorInfo_guess*), actorInfo)();
        operator delete(actorInfo);
        player->actor_info = 0;
    }
    g_cdisplay_guess->unk_0x124 = 0;
    EQ_GLOBAL(CriticalSectionFunction_guess, 0x00534190)(criticalSection);
}

// FUNCTION: TRILOGY 0x004b0d95
// Updates a spawn's visible armor, face, and weapon appearance after its gear changes.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// Redraws the worn look of one spawn on the CDisplay in ECX: head and materials (or the NPC materials),
// the face, then the weapon models in equipment[7] and [8] as "IT%u" item slots. Returns at once when
// actor_info+0x250 is 0.
void __fastcall EQPlayer_DisplayWeaponsAndArmor_guess(EQPlayer* player)
{
    char buf[8];
    int i;
    u8 face;

    if (*(u32*)((char*)player->actor_info + 0x250) == 0)
        return;
    if (player->texture == 0xFF)
    {
        for (i = 0; i <= 6; i++)
        {
            if (i == 0)
            {
                EQ_METHOD(6, void, 0x00407334, (void*, void*, int, int, int, u32, int), g_display_guess)(player, player->equipment[0], 0, 0, player->equip_colors[0], 0);
            }
            else
            {
                EQ_METHOD(5, void, 0x0040798E, (void*, void*, int, int, int, u32), g_display_guess)(player, i, player->equipment[i], 0, player->equip_colors[i]);
            }
        }
        face = player->face;
        EQ_METHOD(3, void, 0x00406EDC, (void*, void*, int, int), g_display_guess)(player, face, 0);
    }
    else
    {
        EQ_METHOD(6, void, 0x00407334, (void*, void*, int, int, int, u32, int), g_display_guess)(player, player->equipment[0], 0, 0, player->equip_colors[0], 0);
        EQ_METHOD(1, void, 0x004060A7, (void*, void*), g_display_guess)(player);
        face = player->face;
        if (face > 0)
        {
            EQ_METHOD(3, void, 0x00406EDC, (void*, void*, int, int), g_display_guess)(player, face, 0);
        }
    }
    if (player->equipment[7] != 0)
    {
        sprintf(buf, EQ_ARRAY(char, 0x0055F0AC), player->equipment[7]);
        EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, char*), g_display_guess)(player, 7, buf);
    }
    if (player->equipment[8] != 0)
    {
        sprintf(buf, EQ_ARRAY(char, 0x0055F0AC), player->equipment[8]);
        EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, char*), g_display_guess)(player, 8, buf);
    }
}

// FUNCTION: TRILOGY 0x004b0eb4
// Clears followers' auto-follow target when the followed spawn is removed.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// Clears the pointer at actor_info+0x2A4 of every other spawn that points at `target`; if that spawn is
// the local player, turns its auto-follow off.
void __fastcall EQPlayer_TurnOffAutoFollowOthers_guess(EQPlayer* target)
{
    EQPlayer* p;
    u32* follow;

    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p != target)
        {
            follow = (u32*)((char*)p->actor_info + 0x2A4);
            if (*follow == (u32)target)
            {
                *follow = 0;
                if (p == g_local_player_guess)
                    p->TurnOffAutoFollow_guess();
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004b0eec
// Moves the local player toward the followed spawn, turning toward it when the path is clear.
// @early-stop: the bearing load for HeadingDiff and the actor reload before the stop test come after their neighbours (VC6 order).
// Auto-follow step for the local player (called from ProcessControls): cancels the follow with a message when
// the followed spawn (actor_info+0x2A4) is dead (stand_state 'x') or more than 200 away; otherwise stores the
// distance (+0x2B0) and bearing (+0x2A8), turns the heading toward it when the ray to it is clear and sets
// the move flags (0x006B6474 / 0x006B6405) from the distance against the two heights' sum.
void __fastcall EQPlayer_AutoFollow_guess(EQPlayer* self)
{
    char message[256];
    struct
    {
        float step;
        float side;
        float heights;
    } local;
    char* target;
    char* info;
    float diff;

    if (self->actor_info == 0)
        return;
    target = self->actor_info->follow_target_guess;
    if (target == 0)
        return;
    if (target[0xA1] == 'x')
    {
        sprintf(message, EQ_ARRAY(char, 0x00568398), target + 1);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
        EQ_METHOD(0, void, 0x004B117B, (EQPlayer*), self)();
        return;
    }
    self->actor_info->targetDistance_guess =
        EQ_METHOD(7, float, 0x004129F3, (void*, float, float, float, float, float, float, float), g_display_guess)(self->y, self->x, self->z,
             *(float*)(target + 0x34), *(float*)(target + 0x38), *(float*)(target + 0x3C), 0.0f);
    if (self->actor_info->targetDistance_guess > 200.0f)
    {
        target = self->actor_info->follow_target_guess;
        sprintf(message, EQ_ARRAY(char, 0x0056836C), target + 1);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
        EQ_METHOD(0, void, 0x004B117B, (EQPlayer*), self)();
        return;
    }
    target = self->actor_info->follow_target_guess;
    local.heights = *(float*)(target + 0x7C) + self->height_offset;
    self->actor_info->follow_heading_guess =
        EQ_CDECL(float, 0x004A47F9, (float, float, float, float))
            (self->y, self->x, *(float*)(target + 0x34), *(float*)(target + 0x38));
    if (EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))
            (self->actor_info->follow_player_guess, self->y, self->x, self->z) != 0)
    {
        info = (char*)self->actor_info;
        if (((EQActorInfo_guess*)info)->targetDistance_guess < 20.0f)
            self->heading = ((EQActorInfo_guess*)info)->follow_heading_guess;
        else
        {
            local.step = 4.0f;
            diff = EQ_METHOD(3, float, 0x004081A3, (void*, float, float, float*), g_display_guess)(self->heading, ((EQActorInfo_guess*)info)->follow_heading_guess, &local.side);
            if (diff < 1.0f)
                self->heading = self->actor_info->follow_heading_guess;
            else
            {
                if (diff >= 4.0f)
                {
                    if (diff > 128.0f)
                        diff = 9.0f;
                    else
                        diff = local.step;
                }
                if ((self->heading < self->actor_info->follow_heading_guess && local.side < 0.0f) ||
                    (self->actor_info->follow_heading_guess < self->heading && local.side > 0.0f))
                    self->heading = self->heading - diff;
                else
                    self->heading = diff + self->heading;
                self->heading = EQ_METHOD(1, float, 0x00408156, (void*, float), g_display_guess)(self->heading);
            }
        }
    }
    info = (char*)self->actor_info;
    if (((EQActorInfo_guess*)info)->targetDistance_guess < local.heights + 15.0f)
    {
        EQ_GLOBAL(u32, 0x006B6474) = 0;
        self->speed = 0.0f;
        return;
    }
    if (((EQActorInfo_guess*)info)->targetDistance_guess < local.heights + 30.0f &&
        (self->speed > *(float*)(((EQActorInfo_guess*)info)->follow_target_guess + 0x48) || self->speed == 0.0f))
    {
        EQ_GLOBAL(u8, 0x006B6405) = 0;
        EQ_GLOBAL(u32, 0x006B6474) = 1;
        return;
    }
    EQ_GLOBAL(u32, 0x006B6474) = 1;
    EQ_GLOBAL(u8, 0x006B6405) = 1;
}

// FUNCTION: TRILOGY 0x004b117b
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// Clears auto-follow after printing the name stored one byte past its pointer.
void EQPlayer::TurnOffAutoFollow_guess(void)
{
    char message[256];
    EQActorInfo_guess* actor;
    u32 target;

    actor = actor_info;
    if (actor != 0 && *(u32*)((char*)actor + 0x2A4) != 0)
    {
        EQ_GLOBAL(u32, 0x006B6474) = 0;
        EQ_GLOBAL(u8, 0x006B6405) = 1;
        speed = 0.0f;
        actor = actor_info;
        target = *(u32*)((char*)actor + 0x2A4);
        sprintf(message, EQ_ARRAY(char, 0x005683BC), (char*)(target + 1));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
        actor = actor_info;
        *(u32*)((char*)actor + 0x2A4) = 0;
    }
}

typedef float (__cdecl *AimArcTangent_t)(float, float, int*);

// FUNCTION: TRILOGY 0x004b11e8
// Aims a player at a target for missile attacks, adjusting facing and vertical angle.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// __thiscall (EQPlayer::AimAtTarget; no declaration in player.h, so a __fastcall free function with the
// EDX dummy), RET 8. Turns this toward target (heading and pitch) when both are within 64 units of the
// bearing; returns how many of the two (0..2) were; at 2 stores the new heading/pitch in this and in
// `out` (+0x18 heading, +0x1C and +0x38 pitch).
// @early-stop: aim sums fadd [mem] not fld+faddp; pitch difference fsubp not copy+fsub+2 fstp; heading else reloads self->heading not [ebp+8]; EDX dummy is xor.
int __fastcall EQPlayer_AimAtTarget_guess(EQPlayer* self, int edx, EQPlayer* target, void* out)
{
    int result = 0;
    float myZ;
    float targetZ;
    float bearing;
    float newHeading;
    float diff;
    float pitch;
    float dist;
    float dz;
    float pitchDiff;
    int arcFlag;
    float aimY;
    float aimX;

    if (self->type != 0 || target == 0 || target == self)
        return 0;
    myZ = self->height_offset * 0.5f + self->z;
    targetZ = target->height_offset * 0.5f + target->z;
    aimY = target->y + (target->dy + target->dy);
    aimX = target->x + (target->dx + target->dx);
    bearing = EQ_CDECL(float, 0x004A47F9, (float, float, float, float))
        (self->y, self->x, aimY, aimX);
    newHeading = self->heading;
    if (bearing != newHeading)
    {
        if (bearing > newHeading)
        {
            diff = bearing - newHeading;
            if (diff > 256.0f)
                diff = 512.0f - bearing + newHeading;
        }
        else if (newHeading > bearing)
        {
            diff = newHeading - bearing;
            if (diff > 256.0f)
                diff = 512.0f - newHeading + bearing;
        }
        else
            diff = self->heading;   // newHeading still equals self->heading here
        if (diff < 64.0f)
        {
            newHeading = bearing;
            result = 1;
        }
    }
    else
    {
        newHeading = bearing;
        result = 1;
    }
    arcFlag = 0;
    dist = EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(target, self, 0.0f);
    if (dist < 1e-5f)
        dist = 1e-5f;
    dz = myZ - targetZ;
    pitch = EQ_GLOBAL(AimArcTangent_t, 0x006E515C)(dist, (dz < 0.0f) ? -dz : dz, &arcFlag);
    pitch = 128.0f - pitch;
    if (pitch < 0.0f)
        pitch = -pitch;
    if (myZ > targetZ)
        pitch = -pitch;
    pitchDiff = (pitch + 128.0f) - (*(float*)&self->unk_0x60 + 128.0f);
    if (((pitchDiff < 0.0f) ? -pitchDiff : pitchDiff) < 64.0f)
        result++;
    else
        pitch = newHeading;
    if (result == 2)
    {
        if (out != 0)
        {
            *(float*)((char*)out + 0x38) = pitch;
            *(float*)((char*)out + 0x1C) = pitch;
            *(float*)((char*)out + 0x18) = newHeading;
        }
        *(float*)&self->unk_0x60 = pitch;
        self->heading = newHeading;
    }
    return result;
}

// FUNCTION: TRILOGY 0x004b13c0
// Detaches any spawn riding this one when it is removed.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (address range)
// __thiscall (the caller, at 0x004B0C55, is the destructor path). Clears the pointer at actor_info+0x18C
// of every other spawn that points at this one.
void EQPlayer::ClearOthersReferenceToMe_guess(void)
{
    EQPlayer* p = g_player_top_guess;

    while (p != 0)
    {
        if (p != this && p->actor_info->ride_target_guess == this)
            p->actor_info->ride_target_guess = 0;
        p = p->next;
    }
}

// FUNCTION: TRILOGY 0x004b13e3
// Removes visual effects owned by a spawn when it leaves the zone.
// __thiscall (the caller, at 0x004B0C63, is the destructor path). Walks the effect list at 0x006E4D74
// (next at +0x54) and destroys and frees every effect whose owner (+0x58) is this spawn. The
// EQEffect destructor (0x00490EF4) unlinks the effect itself.
void __fastcall EQPlayer_DeleteOwnedEffects_guess(EQPlayer* owner)
{
    char* effect;
    char* next;

    effect = EQ_GLOBAL(char*, 0x006E4D74);
    while (effect != 0)
    {
        next = *(char**)(effect + 0x54);
        if (owner == *(EQPlayer**)(effect + 0x58) && effect != 0)
        {
            EQ_METHOD(0, void, 0x00490EF4, (void*), effect)();
            operator delete(effect);
        }
        effect = next;
    }
}

// FUNCTION: TRILOGY 0x004b1416
// Finds the zone spawn matching a spawn ID.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (address range)
// __thiscall that ignores `this` (called on EQPlayer::top). The spawn with that spawn id, or 0.
// Renamed: the TAKP list's HashTable_initializeHashTable (low) does not fit.
EQPlayer* EQPlayer::GetPlayerFromID_guess(int id)
{
    EQPlayer* p;

    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->spawn_id == (u32)id)
            return p;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004b1435
// Finds a zone spawn by its full name, ignoring capitalization.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::get_player, high)
// __thiscall that ignores `this`. The spawn whose name equals `name` (case-insensitive), or 0; an empty
// name finds nothing.
EQPlayer* EQPlayer::get_player_guess(const char* name)
{
    EQPlayer* p;

    if (*name != 0)
    {
        for (p = g_player_top_guess; p != 0; p = p->next)
        {
            if (_strcmpi(p->name, name) == 0)
                return p;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004b146a
// Finds a spawn by exact name or the first name beginning with the supplied text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::get_player_partial_name, medium)
// __thiscall that ignores `this`. The exact match if there is one, else the first spawn whose name starts
// with `name` (case-insensitive; the length is taken as a short), else 0. Unlike the Mac build there is
// no trimName and no "shortest match" rule.
EQPlayer* EQPlayer::get_player_partial_name(const char* name)
{
    EQPlayer* p = g_player_top_guess->get_player_guess(name);
    short len;

    if (p != 0)
        return p;
    len = (short)strlen(name);
    if (len > 0)
    {
        for (p = g_player_top_guess; p != 0; p = p->next)
        {
            if (_strnicmp(p->name, name, len) == 0)
                return p;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004b14bf
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::is_player_actor, medium)
// __thiscall that ignores `this`. The spawn whose t3d actor is `actor`, or 0.
EQPlayer* EQPlayer::is_player_actor(void* actor)
{
    EQPlayer* p;

    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->actor_info->actor == actor)
            return p;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004b14dd
// Adds a race-specific suffix to a spawn's actor tag; what the variant changes is unclear.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (address range)
// __thiscall. Appends a one-letter variant to an actor tag for some races (EBP frame in the original).
// Called once, from EQPlayer::GetActorTag.
void EQPlayer::AppendActorTagSuffix_guess(char* tag)
{
    switch (race)
    {
    case 0x10:
    case 0x44:
    case 0x58:
    case 0x5E:
    case 0x8B:
        strcat(tag, "N");
        break;
    case 0x0D:
    case 0x26:
        strcat(tag, "I");
        break;
    case 0x1D:
        strcat(tag, "R");
        break;
    case 0x11:
    case 0x2A:
    case 0xB1:
        strcat(tag, "L");
        break;
    case 0x9C:
        strcat(tag, "M");
        break;
    case 0xB7:
        strcat(tag, "K");
        break;
    case 0xBD:
        strcat(tag, "G");
        break;
    }
}

struct EQPlayer_guess
{
    u8 unknown0x00[0x9A];
    u16 race;
    u8 gender;
    void GetActorTag(char* tag);
};

// FUNCTION: TRILOGY 0x004b156c
// Writes the model name for the spawn's race plus a gender suffix (e.g. BOAT, SHIP, LAUNCH) for actor lookup
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
/* fake match: the saved race test, case order and separate case 1/default preserve the original branches and table layout. */
void EQPlayer_guess::GetActorTag(char* actorTag)
{
    if (this->race == 0x47 && this->gender == 2)
        this->gender = 0;

    if (this->race == 0xFC)
    {
        strcpy(actorTag, EQ_ARRAY(char, 0x00568640));
        return;
    }
    int usesGenericActorTag = this->race == 0xF4 || this->race == 0xF6 || this->race == 0xF7 ||
        this->race == 0xFA || this->race == 0xFB;
    if (usesGenericActorTag)
    {
        strcpy(actorTag, EQ_ARRAY(char, 0x00568638));
        return;
    }

    switch (this->race)
    {
    case 0x8D: strcpy(actorTag, EQ_ARRAY(char, 0x00568630)); break;
    case 0x48:
        if (this->gender == 0)
            strcpy(actorTag, EQ_ARRAY(char, 0x00568628));
        else
            strcpy(actorTag, EQ_ARRAY(char, 0x00568624));
        break;
    case 0x49: strcpy(actorTag, EQ_ARRAY(char, 0x0056861C)); break;
    case 0x31: strcpy(actorTag, EQ_ARRAY(char, 0x00568618)); break;
    case 0x27: strcpy(actorTag, EQ_ARRAY(char, 0x00568614)); break;
    case 0x28: strcpy(actorTag, EQ_ARRAY(char, 0x00568610)); break;
    case 0x2B: strcpy(actorTag, EQ_ARRAY(char, 0x0056860C)); break;
    case 0x47: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA58)); break;
    case 0x25: strcpy(actorTag, EQ_ARRAY(char, 0x00568608)); break;
    case 0x3C: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9C4)); break;
    case 0x35: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9B4)); break;
    case 0x18: strcpy(actorTag, EQ_ARRAY(char, 0x00568604)); break;
    case 0x1F: strcpy(actorTag, EQ_ARRAY(char, 0x00568600)); break;
    case 0x2A: strcpy(actorTag, EQ_ARRAY(char, 0x0055D960)); break;
    case 0x4A: strcpy(actorTag, EQ_ARRAY(char, 0x005685FC)); break;
    case 0x4B: strcpy(actorTag, EQ_ARRAY(char, 0x005685F8)); break;
    case 0x22: strcpy(actorTag, EQ_ARRAY(char, 0x005685F4)); break;
    case 0x24: strcpy(actorTag, EQ_ARRAY(char, 0x005685F0)); break;
    case 0x0D: strcpy(actorTag, EQ_ARRAY(char, 0x005685EC)); break;
    case 0x43: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA5C)); break;
    case 0x46: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA40)); break;
    case 0x21: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9A4)); break;
    case 0x1A: strcpy(actorTag, EQ_ARRAY(char, 0x005685E8)); break;
    case 0x15:
    case 0x1E: strcpy(actorTag, EQ_ARRAY(char, 0x005685E4)); break;
    case 0x17: strcpy(actorTag, EQ_ARRAY(char, 0x005685E0)); break;
    case 0x37: strcpy(actorTag, EQ_ARRAY(char, 0x005685DC)); break;
    case 0x1B: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9A0)); break;
    case 0x0E: strcpy(actorTag, EQ_ARRAY(char, 0x005685D8)); break;
    case 0x0F: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA48)); break;
    case 0x12: strcpy(actorTag, EQ_ARRAY(char, 0x005685D4)); break;
    case 0x16: strcpy(actorTag, EQ_ARRAY(char, 0x0055D950)); break;
    case 0x19: strcpy(actorTag, EQ_ARRAY(char, 0x0053D890)); break;
    case 0x45: strcpy(actorTag, EQ_ARRAY(char, 0x005685D0)); break;
    case 0x40: strcpy(actorTag, EQ_ARRAY(char, 0x005685CC)); break;
    case 0x44: strcpy(actorTag, EQ_ARRAY(char, 0x005685C8)); break;
    case 0x26: strcpy(actorTag, EQ_ARRAY(char, 0x005685C4)); break;
    case 0x38: strcpy(actorTag, EQ_ARRAY(char, 0x005685C0)); break;
    case 0x3D: strcpy(actorTag, EQ_ARRAY(char, 0x005685BC)); break;
    case 0x2C: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA50)); break;
    case 0x3F: strcpy(actorTag, EQ_ARRAY(char, 0x0055D994)); break;
    case 0x32: strcpy(actorTag, EQ_ARRAY(char, 0x005685B8)); break;
    case 0x4C: strcpy(actorTag, EQ_ARRAY(char, 0x0055D998)); break;
    case 0x4D: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA54)); break;
    case 0x4E: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA4C)); break;
    case 0x4F: strcpy(actorTag, EQ_ARRAY(char, 0x005685B4)); break;
    case 0x11: strcpy(actorTag, EQ_ARRAY(char, 0x005685B0)); break;
    case 0x57: strcpy(actorTag, EQ_ARRAY(char, 0x0055D978)); break;
    case 0x58: strcpy(actorTag, EQ_ARRAY(char, 0x005685AC)); break;
    case 0x59: strcpy(actorTag, EQ_ARRAY(char, 0x005685A8)); break;
    case 0x1C: strcpy(actorTag, EQ_ARRAY(char, 0x005685A4)); break;
    case 0x2F: strcpy(actorTag, EQ_ARRAY(char, 0x0055D974)); break;
    case 0x36: strcpy(actorTag, EQ_ARRAY(char, 0x005685A0)); break;
    case 0x50: strcpy(actorTag, EQ_ARRAY(char, 0x0056859C)); break;
    case 0x51: strcpy(actorTag, EQ_ARRAY(char, 0x00568598)); break;
    case 0x52: strcpy(actorTag, EQ_ARRAY(char, 0x00568594)); break;
    case 0x53: strcpy(actorTag, EQ_ARRAY(char, 0x0055D97C)); break;
    case 0x54: strcpy(actorTag, EQ_ARRAY(char, 0x0055D984)); break;
    case 0x55: strcpy(actorTag, EQ_ARRAY(char, 0x00568590)); break;
    case 0x56: strcpy(actorTag, EQ_ARRAY(char, 0x0055D970)); break;
    case 0x5A: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA44)); break;
    case 0x5B: strcpy(actorTag, EQ_ARRAY(char, 0x0055974C)); break;
    case 0x1D: strcpy(actorTag, EQ_ARRAY(char, 0x0056858C)); break;
    case 0x33: strcpy(actorTag, EQ_ARRAY(char, 0x00568588)); break;
    case 0x5C: strcpy(actorTag, EQ_ARRAY(char, 0x00568584)); break;
    case 0x5D: strcpy(actorTag, EQ_ARRAY(char, 0x00568580)); break;
    case 0x5E: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9F0)); break;
    case 0x5F: strcpy(actorTag, EQ_ARRAY(char, 0x0056857C)); break;
    case 0x60: strcpy(actorTag, EQ_ARRAY(char, 0x00568578)); break;
    case 0x61: strcpy(actorTag, EQ_ARRAY(char, 0x00568574)); break;
    case 0x62: strcpy(actorTag, EQ_ARRAY(char, 0x00568570)); break;
    case 0x41: strcpy(actorTag, EQ_ARRAY(char, 0x0056856C)); break;
    case 0x63: strcpy(actorTag, EQ_ARRAY(char, 0x00568568)); break;
    case 0x64: strcpy(actorTag, EQ_ARRAY(char, 0x00568564)); break;
    case 0x65: strcpy(actorTag, EQ_ARRAY(char, 0x00568560)); break;
    case 0x66: strcpy(actorTag, EQ_ARRAY(char, 0x0056855C)); break;
    case 0x29: strcpy(actorTag, EQ_ARRAY(char, 0x00568558)); break;
    case 0x67: strcpy(actorTag, EQ_ARRAY(char, 0x00568554)); break;
    case 0x68: strcpy(actorTag, EQ_ARRAY(char, 0x00568550)); break;
    case 0x34: strcpy(actorTag, EQ_ARRAY(char, 0x0056854C)); break;
    case 0x69: strcpy(actorTag, EQ_ARRAY(char, 0x00568548)); break;
    case 0x6A: strcpy(actorTag, EQ_ARRAY(char, 0x00568544)); break;
    case 0x6B: strcpy(actorTag, EQ_ARRAY(char, 0x00568540)); break;
    case 0x6D: strcpy(actorTag, EQ_ARRAY(char, 0x0056853C)); break;
    case 0x6E: strcpy(actorTag, EQ_ARRAY(char, 0x00568538)); break;
    case 0x6F: strcpy(actorTag, EQ_ARRAY(char, 0x00568534)); break;
    case 0x70: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA28)); break;
    case 0x71: strcpy(actorTag, EQ_ARRAY(char, 0x00568530)); break;
    case 0x72: strcpy(actorTag, EQ_ARRAY(char, 0x0056852C)); break;
    case 0x73: strcpy(actorTag, EQ_ARRAY(char, 0x00568528)); break;
    case 0x74: strcpy(actorTag, EQ_ARRAY(char, 0x00568524)); break;
    case 0x75: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9EC)); break;
    case 0x20: strcpy(actorTag, EQ_ARRAY(char, 0x00568520)); break;
    case 0x30: strcpy(actorTag, EQ_ARRAY(char, 0x0055D95C)); break;
    case 0x76: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA24)); break;
    case 0x78: strcpy(actorTag, EQ_ARRAY(char, 0x0056851C)); break;
    case 0x2E: strcpy(actorTag, EQ_ARRAY(char, 0x0055D958)); break;
    case 0x79: strcpy(actorTag, EQ_ARRAY(char, 0x00568518)); break;
    case 0x7A: strcpy(actorTag, EQ_ARRAY(char, 0x00568514)); break;
    case 0x7B: strcpy(actorTag, EQ_ARRAY(char, 0x00568510)); break;
    case 0x10: strcpy(actorTag, EQ_ARRAY(char, 0x0056850C)); break;
    case 0x7C: strcpy(actorTag, EQ_ARRAY(char, 0x00568508)); break;
    case 0x7D: strcpy(actorTag, EQ_ARRAY(char, 0x00568504)); break;
    case 0x7E: strcpy(actorTag, EQ_ARRAY(char, 0x00568500)); break;
    case 0x7F: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA34)); break;
    case 0x6C: strcpy(actorTag, EQ_ARRAY(char, 0x005684FC)); break;
    case 0x81: strcpy(actorTag, EQ_ARRAY(char, 0x005684F8)); break;
    case 0x83: strcpy(actorTag, EQ_ARRAY(char, 0x005684F4)); break;
    case 0x84: strcpy(actorTag, EQ_ARRAY(char, 0x005684F0)); break;
    case 0x85: strcpy(actorTag, EQ_ARRAY(char, 0x005684EC)); break;
    case 0x86: strcpy(actorTag, EQ_ARRAY(char, 0x005684E8)); break;
    case 0x87: strcpy(actorTag, EQ_ARRAY(char, 0x005684E4)); break;
    case 0x88: strcpy(actorTag, EQ_ARRAY(char, 0x005684E0)); break;
    case 0x89: strcpy(actorTag, EQ_ARRAY(char, 0x005684DC)); break;
    case 0x8A: strcpy(actorTag, EQ_ARRAY(char, 0x005684D8)); break;
    case 0x3B: strcpy(actorTag, EQ_ARRAY(char, 0x0055D938)); break;
    case 0x8B: strcpy(actorTag, EQ_ARRAY(char, 0x005684D4)); break;
    case 0x2D: strcpy(actorTag, EQ_ARRAY(char, 0x005684D0)); break;
    case 0x8C: strcpy(actorTag, EQ_ARRAY(char, 0x005684CC)); break;
    case 0x13: strcpy(actorTag, EQ_ARRAY(char, 0x005684C8)); break;
    case 0x14: strcpy(actorTag, EQ_ARRAY(char, 0x005684C4)); break;
    case 0x39: strcpy(actorTag, EQ_ARRAY(char, 0x0053E750)); break;
    case 0x3A: strcpy(actorTag, EQ_ARRAY(char, 0x005684C0)); break;
    case 0x3E: strcpy(actorTag, EQ_ARRAY(char, 0x005684BC)); break;
    case 0x42: strcpy(actorTag, EQ_ARRAY(char, 0x005684B8)); break;
    case 0x77: strcpy(actorTag, EQ_ARRAY(char, 0x0055D990)); break;
    case 0x90: strcpy(actorTag, EQ_ARRAY(char, 0x005684B4)); break;
    case 0x91: strcpy(actorTag, EQ_ARRAY(char, 0x005684B0)); break;
    case 0x92: strcpy(actorTag, EQ_ARRAY(char, 0x005684AC)); break;
    case 0x93: strcpy(actorTag, EQ_ARRAY(char, 0x005684A8)); break;
    case 0x94: strcpy(actorTag, EQ_ARRAY(char, 0x005684A4)); break;
    case 0x95: strcpy(actorTag, EQ_ARRAY(char, 0x005684A0)); break;
    case 0x96: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA8C)); break;
    case 0x97: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9BC)); break;
    case 0x98: strcpy(actorTag, EQ_ARRAY(char, 0x0056849C)); break;
    case 0x99: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA7C)); break;
    case 0x9A: strcpy(actorTag, EQ_ARRAY(char, 0x00568498)); break;
    case 0x9B: strcpy(actorTag, EQ_ARRAY(char, 0x0055D934)); break;
    case 0x9C: strcpy(actorTag, EQ_ARRAY(char, 0x00568494)); break;
    case 0x9D: strcpy(actorTag, EQ_ARRAY(char, 0x00568490)); break;
    case 0x9E: strcpy(actorTag, EQ_ARRAY(char, 0x0055D92C)); break;
    case 0x9F: strcpy(actorTag, EQ_ARRAY(char, 0x0056848C)); break;
    case 0xA0: strcpy(actorTag, EQ_ARRAY(char, 0x00568488)); break;
    case 0xA1: strcpy(actorTag, EQ_ARRAY(char, 0x0055841C)); break;
    case 0xA2: strcpy(actorTag, EQ_ARRAY(char, 0x00568484)); break;
    case 0xA3: strcpy(actorTag, EQ_ARRAY(char, 0x00568480)); break;
    case 0xA4: strcpy(actorTag, EQ_ARRAY(char, 0x0056847C)); break;
    case 0xA5: strcpy(actorTag, EQ_ARRAY(char, 0x00568478)); break;
    case 0xA6: strcpy(actorTag, EQ_ARRAY(char, 0x0055D930)); break;
    case 0xA7: strcpy(actorTag, EQ_ARRAY(char, 0x00568474)); break;
    case 0xA8: strcpy(actorTag, EQ_ARRAY(char, 0x0055D96C)); break;
    case 0xA9: strcpy(actorTag, EQ_ARRAY(char, 0x0055D924)); break;
    case 0xAA: strcpy(actorTag, EQ_ARRAY(char, 0x0055D920)); break;
    case 0xAB: strcpy(actorTag, EQ_ARRAY(char, 0x00568470)); break;
    case 0xAC: strcpy(actorTag, EQ_ARRAY(char, 0x0055D98C)); break;
    case 0xAD: strcpy(actorTag, EQ_ARRAY(char, 0x0055D91C)); break;
    case 0xAE: strcpy(actorTag, EQ_ARRAY(char, 0x0055D918)); break;
    case 0xAF: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9B8)); break;
    case 0xB0: strcpy(actorTag, EQ_ARRAY(char, 0x0056846C)); break;
    case 0xB1: strcpy(actorTag, EQ_ARRAY(char, 0x00568468)); break;
    case 0xB2: strcpy(actorTag, EQ_ARRAY(char, 0x00568464)); break;
    case 0xB5: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9AC)); break;
    case 0xB6: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9A8)); break;
    case 0xB7: strcpy(actorTag, EQ_ARRAY(char, 0x00568460)); break;
    case 0xB8: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA74)); break;
    case 0xB9: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA84)); break;
    case 0xBA: strcpy(actorTag, EQ_ARRAY(char, 0x0056845C)); break;
    case 0xBB: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA88)); break;
    case 0xBC: strcpy(actorTag, EQ_ARRAY(char, 0x00568458)); break;
    case 0xBD: strcpy(actorTag, EQ_ARRAY(char, 0x00568454)); break;
    case 0xBE: strcpy(actorTag, EQ_ARRAY(char, 0x00568450)); break;
    case 0xBF: strcpy(actorTag, EQ_ARRAY(char, 0x0056844C)); break;
    case 0xC0: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA70)); break;
    case 0xC1: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA80)); break;
    case 0xC2: strcpy(actorTag, EQ_ARRAY(char, 0x00568448)); break;
    case 0xC3: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA6C)); break;
    case 0xC4: strcpy(actorTag, EQ_ARRAY(char, 0x0055D928)); break;
    case 0xC5: strcpy(actorTag, EQ_ARRAY(char, 0x00568444)); break;
    case 0xC6: strcpy(actorTag, EQ_ARRAY(char, 0x00559F78)); break;
    case 0xC7: strcpy(actorTag, EQ_ARRAY(char, 0x00568440)); break;
    case 0xC8: strcpy(actorTag, EQ_ARRAY(char, 0x0056843C)); break;
    case 0xC9: strcpy(actorTag, EQ_ARRAY(char, 0x00568438)); break;
    case 0xCA: strcpy(actorTag, EQ_ARRAY(char, 0x00568434)); break;
    case 0xCB: strcpy(actorTag, EQ_ARRAY(char, 0x00568430)); break;
    case 0xCC: strcpy(actorTag, EQ_ARRAY(char, 0x0056842C)); break;
    case 0xCD: strcpy(actorTag, EQ_ARRAY(char, 0x00568428)); break;
    case 0xCE: strcpy(actorTag, EQ_ARRAY(char, 0x00568424)); break;
    case 0xCF: strcpy(actorTag, EQ_ARRAY(char, 0x00568420)); break;
    case 0xD0: strcpy(actorTag, EQ_ARRAY(char, 0x0056841C)); break;
    case 0xD1: strcpy(actorTag, EQ_ARRAY(char, 0x00568418)); break;
    case 0xD2: strcpy(actorTag, EQ_ARRAY(char, 0x00568414)); break;
    case 0xD3: strcpy(actorTag, EQ_ARRAY(char, 0x00568410)); break;
    case 0xD4: strcpy(actorTag, EQ_ARRAY(char, 0x0056840C)); break;
    case 0xD5: strcpy(actorTag, EQ_ARRAY(char, 0x00568408)); break;
    case 0x08: strcpy(actorTag, EQ_ARRAY(char, 0x00568404)); break;
    case 0x0A: strcpy(actorTag, EQ_ARRAY(char, 0x0055D9DC)); break;
    case 0x04: strcpy(actorTag, EQ_ARRAY(char, 0x00568400)); break;
    case 0x02: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA18)); break;
    case 0x06: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA68)); break;
    case 0x09: strcpy(actorTag, EQ_ARRAY(char, 0x005683FC)); break;
    case 0x0B: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA04)); break;
    case 0x0C: strcpy(actorTag, EQ_ARRAY(char, 0x005683F8)); break;
    case 0x05: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA64)); break;
    case 0x07: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA60)); break;
    case 0x03: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA1C)); break;
    case 0x80: strcpy(actorTag, EQ_ARRAY(char, 0x005683F4)); break;
    case 0x82: strcpy(actorTag, EQ_ARRAY(char, 0x005683F0)); break;
    case 0x01: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA20)); break;
    default: strcpy(actorTag, EQ_ARRAY(char, 0x0055DA20)); break;
    }

    switch (this->gender)
    {
    case 0:
        if (this->race != 0x48)
            strcat(actorTag, EQ_ARRAY(char, 0x005654A8));
        break;
    case 1:
        strcat(actorTag, EQ_ARRAY(char, 0x005654A4));
        break;
    case 2:
        EQ_METHOD(1, void, 0x004B14DD, (EQPlayer*, char*), (EQPlayer*)this)(actorTag);
        break;
    default:
        EQTRACE_guess("csex = %d\n", this->gender);
        strcat(actorTag, EQ_ARRAY(char, 0x005654A8));
        break;
    }
}

// FUNCTION: TRILOGY 0x004b2198
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (address range)
// static (__cdecl). Deity id 201..216 -> its alliance: 3, 2 or 1; 0 for anything else.
int EQPlayer::WhatDeityAlliance_guess(int deity)
{
    switch (deity)
    {
    case 0xCC:
    case 0xD0:
    case 0xD2:
    case 0xD4:
    case 0xD7:
        return 1;
    case 0xCA:
    case 0xCD:
    case 0xCF:
    case 0xD1:
    case 0xD5:
    case 0xD6:
    case 0xD8:
        return 2;
    case 0xC9:
    case 0xCB:
    case 0xCE:
    case 0xD3:
        return 3;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004b21fc
// Checks whether two spawns' deities place them in different deity alliances.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::NotOnSameDeity, manual match)
// static (__cdecl). 1 when a's deity and b's deity belong to different alliances (a missing spawn counts
// as deity 0, which belongs to none: 0). A GM b (0xF1) without a spell is never "not on the same deity".
int EQPlayer::NotOnSameDeity_guess(EQPlayer* target, EQPlayer* b, EQ_Spell_guess* spell)
{
    int deity_a;
    int deity_b;

    if (target != 0)
    {
        if (b != 0 && b->gm != 0 && spell == 0)
            return 0;
        deity_a = (s16)target->deity;
    }
    else
        deity_a = 0;
    if (b != 0)
        deity_b = (s16)b->deity;
    else
        deity_b = 0;

    switch (deity_a)
    {
    case 0xCC:
    case 0xD0:
    case 0xD2:
    case 0xD4:
    case 0xD7:
        if (deity_b == 0xCC)
            return 0;
        if (deity_b == 0xD0)
            return 0;
        if (deity_b == 0xD4)
            return 0;
        if (deity_b == 0xD2)
            return 0;
        if (deity_b != 0xD7)
            return 1;
        break;
    case 0xCA:
    case 0xCD:
    case 0xCF:
    case 0xD1:
    case 0xD5:
    case 0xD6:
    case 0xD8:
        if (deity_b == 0xCA)
            return 0;
        if (deity_b == 0xCD)
            return 0;
        if (deity_b == 0xCF)
            return 0;
        if (deity_b == 0xD1)
            return 0;
        if (deity_b == 0xD5)
            return 0;
        if (deity_b == 0xD6)
            return 0;
        if (deity_b != 0xD8)
            return 1;
        break;
    case 0xC9:
    case 0xCB:
    case 0xCE:
    case 0xD3:
        if (deity_b == 0xC9)
            return 0;
        if (deity_b == 0xCB)
            return 0;
        if (deity_b == 0xCE)
            return 0;
        if (deity_b != 0xD3)
            return 1;
        break;
    default:
        return 0;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004b2303
// Checks whether two spawns are on different racial teams for player-versus-player rules.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (address range)
// static (__cdecl). 1 when a and b are on different racial teams: {human, barbarian, erudite, vah shir},
// {wood elf, high elf, half elf}, {dark elf, troll, ogre, iksar}, {dwarf, halfling, gnome}. The race is
// the EQ_Character's (0x34) when the spawn has one, else the spawn's. A GM a whose character has no
// pk flag (0xAB8) never is; race 0 or any other race neither.
int EQPlayer::NotOnSameRaceTeam_guess(EQPlayer* a, EQPlayer* b)
{
    CharacterProfile* ca = (CharacterProfile*)a->char_info_guess;
    int race_a;
    int race_b;

    if (ca != 0)
        race_a = ca->race;
    else
        race_a = a->race;
    if (b->char_info_guess != 0)
        race_b = ((CharacterProfile*)b->char_info_guess)->race;
    else
        race_b = b->race;

    if (a->gm != 0 && ca != 0 && ca->pk == 0)
        return 0;

    switch (race_a)
    {
    case 1:
    case 2:
    case 3:
    case 0x82:
        if (race_b == 1)
            return 0;
        if (race_b == 2)
            return 0;
        if (race_b == 3)
            return 0;
        if (race_b != 0x82)
            return 1;
        break;
    case 4:
    case 5:
    case 7:
        if (race_b == 5)
            return 0;
        if (race_b == 4)
            return 0;
        if (race_b == 7)
            break;
        return 1;
    case 8:
    case 11:
    case 12:
        if (race_b == 11)
            return 0;
        if (race_b == 8)
            return 0;
        if (race_b == 12)
            break;
        return 1;
    case 6:
    case 9:
    case 10:
    case 0x80:
        if (race_b == 10)
            return 0;
        if (race_b == 9)
            return 0;
        if (race_b == 6)
            return 0;
        if (race_b == 0x80)
            break;
        return 1;
    default:
        return 0;
    }
    return 0;
}

class EQPlayer_4b23e1_guess : public EQPlayer
{
public:
    void SetWeaponAnimType_guess(char* param_2);
};

// FUNCTION: TRILOGY 0x004b23e1
// Chooses the spawn's weapon animation style from its equipped weapon and actor type.
/* fake match: the last null test re-reads the equipment field; VC6 folds a repeat test of the local. */
void EQPlayer_4b23e1_guess::SetWeaponAnimType_guess(char* text)
{
    u8 itemType;
    int actorValue;
    int value;
    int primaryItem;
    EQ_Equipment* equipment;
    char* animation_set;
    char* char_info;

    this->weaponAnimationType = 0;
    animation_set = this->actor_info->anim_set_guess;
    if (animation_set == 0)
        return;
    if (*(char*)(animation_set + 0x39C) == '\x01')
    {
        char_info = (char*)this->char_info_guess;
        if (((char_info == 0) || (*(int*)(char_info + 0x10C) == 0) || (*text == '\0')) ||
            (value = _strcmpi(text,
                (const char*)(*(int*)(char_info + 0x10C) + 0x5F)), value != 0))
        {
            primaryItem = 0;
            if (char_info != 0)
            {
                value = *(int*)(char_info + 0x114);
                if ((value != 0) && (*(char*)(value + 0x81) == '\0'))
                    primaryItem = value;
                equipment = *(EQ_Equipment**)(char_info + 0x118);
                if (((equipment == 0) || (equipment->item.type != '\0')) ||
                    (*(EQ_Equipment**)(char_info + 0x118) == 0))
                {
                    if (primaryItem != 0)
                    {
                        itemType = *(u8*)(primaryItem + 0xC2);
                        switch (itemType)
                        {
                        case 1:
                            this->weaponAnimationType = 6;
                            break;
                        case 4:
                        case '#':
                            this->weaponAnimationType = 4;
                            break;
                        default:
                            this->weaponAnimationType = 1;
                            break;
                        }
                    }
                }
                else if (equipment->item_type == '\b')
                {
                    this->weaponAnimationType = 2;
                }
                else if (primaryItem != 0)
                {
                    this->weaponAnimationType = EQ_METHOD(0, int, 0x0043624B, (EQ_Equipment*), equipment)() ? 3 : 1;
                }
                else
                {
                    this->weaponAnimationType = 5;
                }
            }
        }
        else
        {
            this->weaponAnimationType = 7;
        }
        if (this->weaponAnimationType == 0)
        {
            actorValue = this->actor_info->unk_0x238;
            if (actorValue == 4)
            {
                this->weaponAnimationType = 7;
            }
            else if (actorValue == 3)
            {
                this->weaponAnimationType = 2;
            }
            else
            {
                value = this->actor_info->unk_0x234;
                if (value != 0)
                {
                    if (actorValue != 0)
                    {
                        this->weaponAnimationType = 3;
                        return;
                    }
                }
                else
                {
                    if (actorValue != 0)
                    {
                        this->weaponAnimationType = 5;
                        return;
                    }
                }
                switch (value)
                {
                case 0:
                    break;
                case 1:
                    this->weaponAnimationType = 4;
                    break;
                default:
                    this->weaponAnimationType = 1;
                    break;
                }
            }
        }
        return;
    }
    return;
}

// FUNCTION: TRILOGY 0x004b2548
// Updates the spawn's AFK state and refreshes its name display.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void EQPlayer::SetAfk_guess(int afk)
{
    this->afk = afk;
    EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), g_display_guess)(this);
}

// FUNCTION: TRILOGY 0x004b2561
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// @early-stop: frame is 0xC, not 0x14, and the shared return-0/return-1 blocks sit at the top, where the original has them at the end and mid-function.
// __thiscall ABI (fastcall stand-in, as IsZoneNameFlagged_guess): ECX is self, EDX is never read, other and
// spellId are the stack arguments (RET 8). Returns AL: 1 when self may attack other with spell spellId (-1 = none).
// No field yet in player.h for EQActorInfo 0x2E4 (int) and 0x9C (u8), or EQ_Spell_guess 0x21F (u8): raw offsets.
/* fake match: the gotos are the original's shared tails (one label per jump target); a structured form lays them out differently */
bool __fastcall EQPlayer_AllowedToAttack_guess(EQPlayer* self, int edx, EQPlayer* other, int spellId)
{
    char selfType;
    int levelSelf;
    int levelOther;
    /* fake match: volatile keeps spell in memory, as the original does; VC6 would give it EBX */
    EQ_Spell_guess* volatile spell;
    int levelDiff;
    int result;
    int value;
    EQPlayer* owner;
    char otherType;
    char found;
    short selfRace;
    short otherRace;
    u8 spellFlag;
    u8 effectId;

    if (other == 0)
        return 0;
    if (self->actor_info == 0)
        return 0;
    if (other->actor_info == 0)
        return 0;
    levelSelf = self->level;
    levelOther = other->level;
    if (self->type == 0 && self->char_info_guess != 0 && ((CharacterProfile*)self->char_info_guess)->pk != 0)
    {
        value = self->actor_info->unk_0x2E4;
        if (value <= 0)
            return 1;
        if (value < levelOther)
            return 0;
        return 1;
    }
    if (other->type == 0 && other->char_info_guess != 0 && ((CharacterProfile*)other->char_info_guess)->pk != 0)
    {
        owner = ((CharacterProfile*)other->char_info_guess)->player;
        if (owner->actor_info == 0)
            return 1;
        value = owner->actor_info->unk_0x2E4;
        if (value <= 0)
            return 1;
        if (value < levelSelf)
            return 0;
        return 1;
    }
    levelDiff = abs(levelSelf - levelOther);
    otherRace = other->race;
    if (otherRace == 0x48)
        return 0;
    selfRace = self->race;
    if (selfRace == 0x48)
        return 0;
    if (otherRace == 0x8D)
        return 0;
    if (selfRace == 0x8D)
        return 0;
    if (otherRace == 0x49)
        return 0;
    if (selfRace == 0x49)
        return 0;
    if (selfRace == 0x72)
        return 0;
    if (self->type != 1)
    {
        if (self->actor_info->unknown0x099 != other->actor_info->unknown0x099 && spellId != 0x6EB)
            return 0;
    }
    if (*((u8*)self->actor_info + 0x9C) != 0 && *((u8*)other->actor_info + 0x9C) != 0)
        return 1;
    if (other == self && spellId > 0 && spellId < 3000)
        return 1;
    if (other->type == 1 && other->pet_owner_id > 0)
    {
        owner = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, other->pet_owner_id);
        if (owner != 0 && owner->type == 0)
        {
            other = owner;
            levelOther = owner->level;
        }
    }
    if (self->type == 1 && self->pet_owner_id > 0)
    {
        owner = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, self->pet_owner_id);
        if (owner != 0 && owner->type == 0)
        {
            self = owner;
            levelSelf = owner->level;
        }
    }
    otherType = other->type;
    if (otherType == 1)
        return 1;
    selfType = self->type;
    if (selfType == 1)
        return 1;
    spell = 0;
    if (spellId > -1 && spellId < 3000)
    {
        spell = g_spells_guess[spellId];
        if (spell == 0)
            return 0;
    }
    if (EQ_GLOBAL(u8, 0x006B64D0) != 0)
    {
        if (spell != 0 && *((u8*)spell + 0x21F) != 0 && other->pvp != self->pvp)
            return 0;
        if (EQ_GLOBAL(int, 0x006B64C8) == other->spawn_id)
            return 1;
        if (EQ_GLOBAL(int, 0x006B64C8) == self->spawn_id)
            return 1;
    }
    if (spellId == -1 && otherType == 0 && selfType == 0 && self->guild_id != -1 && other->guild_id != -1)
        return 1;
    if (spell == 0)
        goto noSpell;
    if (spell->target_type_guess == 0x14)
        return 1;
    spellFlag = *((u8*)spell + 0x21F);
    if (spellFlag != 0)
        goto flaggedSpell;
    if (EQ_GLOBAL(int, 0x00596330) != 2)
        goto tryDeityRule;
raceTeamCheck:
    result = EQ_CDECL(int, 0x004B2303, (EQPlayer*, EQPlayer*, void*))(other, self, spell);
    if (result == 0)
        goto pvpFlagCheck;
    if (levelDiff > 8)
        return 0;
    if (levelSelf < 6)
        return 0;
    if (levelOther < 6)
        return 0;
    return 1;
tryDeityRule:
    if (EQ_GLOBAL(int, 0x00596330) != 4)
        goto pvpFlagCheck;
deityCheck:
    result = EQ_CDECL(int, 0x004B21FC, (EQPlayer*, EQPlayer*, void*))(other, self, spell);
    if (result == 0)
        goto pvpFlagCheck;
    if (levelSelf >= 6 && levelOther >= 6)
        return 1;
    result = EQ_METHOD(1, int, 0x004BFB7C, (void*, char*), EQ_GLOBAL(void*, 0x006E4D54))(other->zone);
    if (result != 0)
        return 0;
    return 1;
noSpell:
    if (EQ_GLOBAL(int, 0x00596330) == 2)
        goto raceTeamCheck;
    if (EQ_GLOBAL(int, 0x00596330) == 4)
        goto deityCheck;
pvpFlagCheck:
    if (other->pvp == 0)
        return 0;
    if (self->pvp == 0)
        return 0;
    if (levelDiff > 4)
        return 0;
    if (levelSelf < 6)
        return 0;
    if (levelOther < 6)
        return 0;
    return 1;
flaggedSpell:
    if (self->gm != 0)
        return 1;
    if (spellFlag <= 1)
        goto lowSpellFlag;
    if (selfType != 0)
        return 0;
    found = EQ_CDECL(char, 0x0048A11F, (void*, int))(spell, 0x51);
    if (found == 0)
        goto groupMemberCheck;
    goto samePvpCheck;
lowSpellFlag:
    if (spellFlag != 1)
        goto samePvpCheck;
    effectId = spell->effect_id[0];
    if (effectId == 0x0C || effectId == 0x1C || effectId == 0x1D)
        goto groupMemberCheck;
    if (spellId == 0x1A8)
        goto groupMemberCheck;
    goto samePvpCheck;
groupMemberCheck:
    if (other->char_info_guess == 0)
        return 0;
    if (EQ_METHOD(1, int, 0x0047EFCE, (void*, char*), other->char_info_guess)(self->name) == 0)
        return 0;
samePvpCheck:
    if (other->pvp != self->pvp)
        return 0;
    if (EQ_GLOBAL(u8, 0x006B64D0) != 0)
        return 0;
    if (EQ_GLOBAL(int, 0x00596330) != 4)
        return 1;
    found = EQ_CDECL(char, 0x0048A11F, (void*, int))(spell, 0x1B);
    if (found != 0)
        return 1;
    result = EQ_CDECL(int, 0x004B21FC, (EQPlayer*, EQPlayer*, void*))(other, self, spell);
    if (result == 0)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004b2975
// Checks whether the target is close enough and in front for an attack to start.
// __thiscall ABI (fastcall stand-in, as EQPlayer_AllowedToAttack_guess): ECX is self, EDX is never read, other and
// range are the stack arguments (RET 8). Returns AL: 1 when self can hit other from here, else 0. Refusals print
// their reason (EQTRACE when self has no EQ_Character); other not allowed, or without actor info, is silent.
// range is the reach past the melee range: the original stores melee range + range back into its argument slot.
u8 __fastcall EQPlayer_CanIHit_guess(EQPlayer* self, int edx, EQPlayer* other, float range)
{
    float tolerance;
    float selfY;
    float selfX;
    float otherY;
    float otherX;
    float angle;

    tolerance = 80.0f;
    if (self->char_info_guess == 0)
    {
        EQTRACE_guess("My_Char is NULL in CanIHit()!\n");
    }
    else if (EQ_Character_TotalSpellAffects_guess(self->char_info_guess, 0x28) > 0)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))("You can't attack while invulnerable!");
    }
    else if (other == self)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))("Try attacking someone other than yourself, it's more productive.", 0xD);
    }
    else if (other == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))("You must first click on the being you wish to attack!", 0xD);
    }
    else
    {
        if (EQ_METHOD(2, bool, 0x004B2561, (EQPlayer*, EQPlayer*, int), self)(other, -1) &&
            self->actor_info != 0 && self->actor_info->actor != 0 &&
            other->actor_info != 0 && other->actor_info->actor != 0)
        {
            selfY = self->y;
            selfX = self->x;
            otherY = other->y;
            otherX = other->x;
            if (EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))(self, otherY, otherX, other->z) == 0)
            {
                EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self->char_info_guess, "You can't hit them from here.", 0xD);
            }
            else
            {
                range = EQ_CDECL(float, 0x004A491F, (EQPlayer*, EQPlayer*))(self, other) + range;
                if (EQ_CDECL(float, 0x004A48CA, (EQPlayer*, EQPlayer*))(self, other) > range)
                {
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self->char_info_guess, "Your target is too far away, get closer!", 0xD);
                }
                else
                {
                    angle = EQ_CDECL(float, 0x004A47F9, (float, float, float, float))(selfY, selfX, otherY, otherX);
                    if (angle > 472.0f && self->heading < 40.0f)
                        angle = self->heading;
                    if (angle < 40.0f && self->heading > 472.0f)
                        angle = self->heading;
                    if (other->type == 0 && self->type == 0)
                    {
                        tolerance = 256.0f;
                        if (angle > 350.0f && self->heading < 150.0f)
                            angle = self->heading;
                        if (angle < 150.0f && self->heading > 350.0f)
                            angle = self->heading;
                    }
                    angle = angle - self->heading;
                    if ((angle < 0.0f ? -angle : angle) > tolerance)
                    {
                        EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self->char_info_guess, "You can't see your target from here.", 0xD);
                    }
                    else
                        return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004b2b65
// Adjusts weapon swing or shot timing to match the character's haste and ranged setup.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::ModifyAttackSpeed, high)
// __thiscall. An attack delay shortened by the haste spell effect (11: delay / (haste% / 100)); for a
// ranged attack also by the first quiver (container of bag type 2 with a weight reduction) in the
// inventory slots 22..29: delay - (int)(reduction * 0.0025 * delay) - 1, unless that is 1000 or less.
u32 EQPlayer::ModifyAttackSpeed(u32 delay, int ranged)
{
    CharacterProfile* ch = (CharacterProfile*)char_info_guess;

    if (delay != 0)
    {
        short haste = EQ_Character_TotalSpellAffects_guess(ch, 0xB);
        if (haste > 0)
            delay = (int)(delay / (haste * 0.01f));
        if (ranged != 0)
        {
            int i;
            for (i = 22; i <= 29; i++)
            {
                EQ_Container* bag = (EQ_Container*)ch->inventory_items[i];
                if (bag != 0 && bag->item.type == EQ_ITEM_CONTAINER && bag->bag_type == 2
                    && bag->weight_reduction > 0)
                {
                    float percent = bag->weight_reduction * 0.0025f;
                    int reduction = (int)(percent * delay);
                    reduction++;
                    if (delay - reduction > 1000)
                        delay -= reduction;
                    break;
                }
            }
        }
    }
    return delay;
}

// The 0x74-byte missile a ranged attack, or a swing with no target, launches; its constructor is the client's.
class PlayerMissile_guess : public EQMissile_guess
{
public:
    // EXTERNAL: TRILOGY 0x004AC67C PlayerMissile_guess::PlayerMissile_guess
    PlayerMissile_guess(EQ_Equipment* item, EQPlayer* shooter, EQPlayer* target, const char* actorTag, u8 kind,
        int flags);
};

// FUNCTION: TRILOGY 0x004b2c1e
// Makes one attack: combat skill, primary/offhand swing (dual wield, double attack) or ranged shot using ammo
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::DoAttack, medium)
// __thiscall, written as a free __fastcall because player.h doesn't declare it: ECX = the attacking player,
// EDX dummy, then slot (0x64 'd' combat skill, 0x65 'e' primary swing without a timer, or
// 0x0b/0x0d/0x0e = ranged/primary/secondary attack with the item in that inventory slot), skill (the skill
// id for 'd'), target (may be 0: a missile is launched instead of a direct hit). The callee pops 12 bytes.
// Returns 1 on every path.
int __fastcall DoAttack_guess(EQPlayer* self, int edx, u8 slot, u8 skill, EQPlayer* target)
{
    float range = 0.0f;
    EQ_Equipment* item;
    CharacterProfile* ch;
    u32* attackTime;
    u32 delay;
    u32 now;
    u32 lastAttack;
    int tooFar;
    EQ_Equipment* quiver;
    int incapacitated;
    u32 msg[3];

    void* charInfo = self->char_info_guess;
    if (charInfo == 0 ||
        (self->actor_info->unknown0x248 != -1 && self->class_ != 8))
        return 1;
    // A stunned, feigning or dying player (and anything not a player or NPC) can't attack.
    incapacitated = 0;
    if (self->type == 0 && ((CharacterProfile*)self->char_info_guess)->unknown0x0076 == 1)
    {
        EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self->char_info_guess,
            (const char*)0x005688C8, 0xd);
        incapacitated = 1;
    }
    else if (self->type == 0 &&
        (((CharacterProfile*)self->char_info_guess)->unknown0x0076 == 2 ||
         ((CharacterProfile*)self->char_info_guess)->unknown0x0076 == 3 ||
         ((CharacterProfile*)self->char_info_guess)->unknown0x0076 == 4))
        incapacitated = 1;
    else if (self->type > 1 ||
        (self->type == 0 && self->char_info_guess != 0 &&
         (((CharacterProfile*)self->char_info_guess)->unknown0x0076 == 7 ||
          ((CharacterProfile*)self->char_info_guess)->unknown0x0076 == 4 ||
          ((CharacterProfile*)self->char_info_guess)->unk_0xAA4 != 0)))
        incapacitated = 1;
    if (incapacitated != 0)
        return incapacitated;

    // Attacking ends these spell effects.
    EQ_METHOD(1, void, 0x00424E22, (void*, void*), self->char_info_guess)(
        EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), self->char_info_guess)(0x0c, 0));
    EQ_METHOD(1, void, 0x00424E22, (void*, void*), self->char_info_guess)(
        EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), self->char_info_guess)(0x1c, 0));
    EQ_METHOD(1, void, 0x00424E22, (void*, void*), self->char_info_guess)(
        EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), self->char_info_guess)(0x1d, 0));
    EQ_METHOD(1, void, 0x00424E22, (void*, void*), self->char_info_guess)(
        EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), self->char_info_guess)(0x5e, 0));
    if (EQ_GLOBAL(char, 0x006B65D8) != 0 || EQ_GLOBAL(short, 0x006B6772) == 0x1d)
    {
        if (EQ_GLOBAL(short, 0x006B6772) == 0x1d)
            EQ_GLOBAL(short, 0x006B6772) = -1;
        EQ_GLOBAL(char, 0x006B65D8) = 0;
        EQ_METHOD(0, void, 0x004255CE, (void*), self->char_info_guess)();
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
            (const char*)0x0054235C);
    }

    if (slot == 0x64)
    {
        char* skillRecord = g_skill_records_guess[skill];
        if (*(short*)(skillRecord + 0x88) < 1)
        {
            EQTRACE_guess((const char*)0x00568898, skillRecord);
            return 1;
        }
        if (skill != 10 || (self->race != 9 && self->race != 10 && self->race != 2))
        {
            CharacterProfile* profile = (CharacterProfile*)self->char_info_guess;
            int allowed = profile != 0 ? EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), profile)(skill) : 0;
            if (allowed == 0)
                return 1;
            // Backstab (skill 10 here) needs a piercer: a type 8 offhand or one of three primaries.
            if (profile != 0 && skill == 10 && self->type == 0 &&
                ((item = (EQ_Equipment*)profile->inventory_items[14]) == 0 || item->item.type != 0 ||
                 item->item_type != 8) &&
                (profile->inventory_items[13] == 0 ||
                 (profile->inventory_items[13]->item_nr != 0x2b2a &&
                  profile->inventory_items[13]->item_nr != 0x2773 &&
                  profile->inventory_items[13]->item_nr != 0x382f)))
                allowed = 0;
            if (allowed == 0)
                return 1;
        }
    }

    ch = (CharacterProfile*)self->char_info_guess;
    if (ch == 0)
    {
        EQTRACE_guess((const char*)0x0056885C, self->name);
        return 1;
    }
    if (EQ_GLOBAL(u8, 0x006B6464) != 0 || slot == 0)
        goto done;

    // Show the weapons the attack uses and pick the timer for the hand.
    switch (slot)
    {
    case 0x0b:
        attackTime = &self->actor_info->unk_0x64;
        if (self->actor_info != 0)
        {
            if (self->actor_info->unk_0x89 != 0x0b)
            {
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                    self, 7, (const char*)0x0053D860);
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                    self, 8, (const char*)0x0053D860);
                if (ch->inventory_items[11] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, ch->inventory_items[11]->idfile);
            }
            self->actor_info->unk_0x89 = 0x0b;
        }
        break;
    case 0x0d:
        attackTime = &self->actor_info->unk_0x64;
        if (self->actor_info != 0)
        {
            if (self->actor_info->unk_0x89 == 0x0b)
            {
                if (ch->inventory_items[13] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 7, ch->inventory_items[13]->idfile);
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 7, (const char*)0x0053D860);
                if (ch->inventory_items[14] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, ch->inventory_items[14]->idfile);
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, (const char*)0x0053D860);
            }
            self->actor_info->unk_0x89 = 0x0d;
        }
        break;
    case 0x0e:
        attackTime = &self->actor_info->unk_0xCC;
        if (self->actor_info != 0)
        {
            if (self->actor_info->unk_0x89 == 0x0b)
            {
                if (ch->inventory_items[13] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 7, ch->inventory_items[13]->idfile);
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 7, (const char*)0x0053D860);
                if (ch->inventory_items[14] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, ch->inventory_items[14]->idfile);
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, (const char*)0x0053D860);
            }
            self->actor_info->unk_0x89 = 0x0e;
        }
        break;
    }

    if (slot == 0x0d || slot == 0x0e || slot == 0x0b)
    {
        // Weapon attack with the item in the slot (hand to hand when the hand is empty).
        item = (EQ_Equipment*)ch->inventory_items[slot];
        if (item != 0)
        {
            if (!EQ_METHOD(0, int, 0x0043624B, (EQ_Equipment*), item)())
                goto done;
        }
        else
        {
            if (slot == 0x0b)
                goto done;
            item = EQ_GLOBAL(EQ_Equipment*, 0x005FE850);
            item->damage = EQ_METHOD(1, s8, 0x0041D5F4, (void*, u8), ch)(0x1c);
            item->item_type = 0x16;
            item->delay = 0x1c;
        }
        if (item == 0 ||
            (!EQ_METHOD(0, int, 0x0043624B, (EQ_Equipment*), item)() && item->item_type != 0x0d && item->item_type != 0x16))
            goto done;
        if (item->item_type == 0x16)
        {
            delay = *(u32*)(g_skill_records_guess[item->delay] + 0x20);
            if (self->class_ == 7 && self->level > 26)
            {
                CharacterProfile* profile = (CharacterProfile*)self->char_info_guess;
                int haste = (self->level - 24) / 4 * 100;
                if (profile != 0 && profile->race == 0x80 && self->type == 0)
                    haste = (self->level - 24) / 6 * 100;
                if (haste > 1000)
                    haste = 1000;
                delay -= haste;
                if (profile != 0 && profile->inventory_items[12] != 0 &&
                    profile->inventory_items[12]->item_nr == 0x299c && self->level > 50)
                    delay = 1600;
            }
            if (self->class_ == 15 && self->level > 26)
            {
                int haste = (self->level - 24) / 5 * 100;
                if (haste > 1000)
                    haste = 1000;
                delay -= haste;
            }
        }
        else
        {
            delay = item->delay * 100;
        }
        if (item->item_type == 5)
            delay = self->ModifyAttackSpeed(delay, 1);
        else
            delay = self->ModifyAttackSpeed(delay, 0);
        now = EqGetTime();
        lastAttack = *attackTime;
        if (now - lastAttack < delay &&
            (self->type != 1 || self->actor_info == 0 || self->actor_info->unk_0x2E0 <= 1))
        {
            if (self->type == 1 && self->actor_info != 0 && self->actor_info->unk_0x2E0 < 2)
                self->actor_info->unk_0x2E0 = 0;
            goto done;
        }
        *attackTime = now;

        if (self->type == 0)
        {
            if (self->stand_state != 'd')
            {
                if (self->stand_state == 'f')
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                        (const char*)0x005410A8, 0xd);
                else
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                        (const char*)0x0056883C, 0xd);
                return 1;
            }
            tooFar = 0;
            if (slot == 0x0b)
            {
                float heightDifference = 0.0f;
                if (target != 0)
                {
                    if (EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(
                            self, target, 0.0f) < 30.0f)
                    {
                        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                            (const char*)0x005687AC, 0xf);
                        *attackTime = lastAttack;
                        return 1;
                    }
                    float dz = (target->z - target->height_offset) - (self->z - self->height_offset);
                    heightDifference = dz < 0.0f ? -dz : dz;
                }
                if (heightDifference > 39.99f && EQ_GLOBAL(int, 0x006E4D4C) == 0)
                {
                    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                        (const char*)0x00568784);
                    tooFar = 1;
                    *attackTime = lastAttack;
                }
                range = (float)item->range;
                quiver = (EQ_Equipment*)ch->inventory_items[21];
                if (item->item_type == 5 && quiver != 0 && quiver->item_type == 0x1b)
                    range += quiver->range;
            }
            if ((!EQ_METHOD(2, char, 0x004B2975, (EQPlayer*, EQPlayer*, float), self)(target, range) ||
                 tooFar != 0) && EQ_GLOBAL(u8, 0x006B63F8) == 0)
            {
                if (target != 0)
                    EQTRACE_guess((const char*)0x005687E0, target->name);
                if (slot == 0x0b)
                    *attackTime = lastAttack;
                goto done;
            }
            // Swinging tires the attacker: light weapons one time in fatigueOdds + 1, medium ones always,
            // the heaviest twice as much.
            {
                int fatigueOdds;
                if (item == 0 || item == EQ_GLOBAL(EQ_Equipment*, 0x005FE850) || item->item.weight < 26)
                    fatigueOdds = 2;
                else if (item->item.weight < 51)
                    fatigueOdds = 1;
                else if (item->item.weight < 101)
                    fatigueOdds = 0;
                else
                    fatigueOdds = -1;
                if (fatigueOdds < 0)
                    EQ_METHOD(1, void, 0x00480DFB, (void*, int), ch)(ch->fatigue + 2);
                else if (fatigueOdds == 0 || random_range_guess(0, fatigueOdds) == 0)
                    EQ_METHOD(1, void, 0x00480DFB, (void*, int), ch)(ch->fatigue + 1);
            }
        }
        else if (self->type == 1 && self->actor_info != 0 && self->actor_info->unk_0x2E0 < 2)
        {
            self->actor_info->unk_0x2E0 = 1;
        }

        if (item != 0 && item != EQ_GLOBAL(EQ_Equipment*, 0x005FE850) && item->click_effect_type >= 2 &&
            (short)item->click_effect_id >= 0 && item->effect_type == 0 && (short)item->click_effect_id < 3000)
        {
            EQ_METHOD(0, int, 0x0041B48F, (void*), ch)();
            random_range_guess(1, 2500);
        }

        switch (slot)
        {
        case 0x0b:
        {
            // Ranged: shoot the ammunition, or throw the ranged item itself.
            u8 ammoState;
            if (ch->inventory_items[11] == 0)
                break;
            ammoState = EQ_METHOD(0, u8, 0x004B3A16, (EQPlayer*), self)();
            if (ammoState == 0)
            {
                if (ch->inventory_items[11] == 0 || target == 0)
                    break;
                EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target,
                    (EQ_Equipment*)ch->inventory_items[11], 0x0b);
                operator delete(ch->inventory_items[11]);
                ch->inventory_items[11] = 0;
                msg[1] = 0xffffffff;
                msg[2] = 0;
                msg[0] = 0x0b;
                send_message_guess(g_world_connection_guess, 0x212c, (void*)msg, 0xc, 1);
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                    self, 8, (const char*)0x0053D860);
            }
            else if (ammoState == 1)
            {
                EQ_Equipment* ammo = (EQ_Equipment*)ch->inventory_items[21];
                u8 found;
                char bagSlot;
                if (ammo == 0 || target == 0)
                    break;
                EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, ammo, 0x0b);
                bagSlot = -1;
                slot = 21;
                if (ammo->charges <= 1)
                {
                    // The last of the stack: look for more of the same in the packs.
                    int i;
                    found = 0;
                    for (i = 22; i <= 29; i++)
                    {
                        if (found == 0 && ch->inventory_items[i] != 0)
                        {
                            if (ch->inventory_items[i]->item_nr == ammo->item.item_nr)
                            {
                                found = 1;
                                slot = (u8)i;
                                ammo = (EQ_Equipment*)ch->inventory_items[i];
                            }
                            else
                            {
                                EQ_Container* bag = (EQ_Container*)ch->inventory_items[i];
                                int j;
                                if (bag == 0 || bag->item.type != 1)
                                    continue;
                                for (j = 0; j < bag->slots; j++)
                                {
                                    if (bag->contents[j] != 0 && bag->contents[j]->item_nr == ammo->item.item_nr)
                                    {
                                        found = 1;
                                        slot = (u8)i;
                                        bagSlot = (char)j;
                                        ammo = (EQ_Equipment*)bag->contents[j];
                                    }
                                }
                            }
                        }
                    }
                }
                ammo->charges--;
                if (bagSlot >= 0)
                    msg[0] = bagSlot + (slot * 5 + 0xf) * 2;
                else
                    msg[0] = slot;
                msg[1] = 0xffffffff;
                msg[2] = 0xffffffff;
                send_message_guess(g_world_connection_guess, 0x2146, (void*)msg, 0xc, 1);
                if (ammo->charges <= 0)
                {
                    EQ_Item** where;
                    if (bagSlot >= 0)
                        where = &((EQ_Container*)ch->inventory_items[slot])->contents[bagSlot];
                    else
                        where = &ch->inventory_items[slot];
                    operator delete(*where);
                    *where = 0;
                }
            }
            else if (ammoState == 2)
            {
                EQ_Equipment* thrown = (EQ_Equipment*)ch->inventory_items[11];
                if (thrown == 0 || thrown->item_type != 0x13 || target == 0)
                    break;
                EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, thrown, 0x0b);
                if (EQ_CDECL(int, 0x004A1D83, (void*))(ch->inventory_items[11]) != 0 && thrown->charges > 0)
                {
                    if (--thrown->charges == 0)
                    {
                        operator delete(ch->inventory_items[11]);
                        ch->inventory_items[11] = 0;
                    }
                    msg[1] = 0xffffffff;
                    msg[2] = 0xffffffff;
                    msg[0] = 0x0b;
                    send_message_guess(g_world_connection_guess, 0x2146, (void*)msg, 0xc, 1);
                }
                else
                {
                    operator delete(ch->inventory_items[11]);
                    ch->inventory_items[11] = 0;
                    msg[1] = 0xffffffff;
                    msg[2] = 0;
                    msg[0] = 0x0b;
                    send_message_guess(g_world_connection_guess, 0x212c, (void*)msg, 0xc, 1);
                }
                if (ch->inventory_items[13] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 7, ch->inventory_items[13]->idfile);
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 7, (const char*)0x0053D860);
                if (ch->inventory_items[11] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, ch->inventory_items[11]->idfile);
                else if (ch->inventory_items[14] != 0)
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, ch->inventory_items[14]->idfile);
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        self, 8, (const char*)0x0053D860);
            }
            break;
        }
        case 0x0d:
            // Primary hand, then a double attack.
            if (target == 0)
                new PlayerMissile_guess(item, self, 0, (const char*)0x0053E4BC, slot, 0);
            else
                EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, item,
                    slot);
            if (EQ_METHOD(1, u8, 0x0041D5BC, (void*, EQPlayer*), ch)(target) == 1)
            {
                if (target == 0)
                    new PlayerMissile_guess(item, self, 0, (const char*)0x0053E4BC, slot, 0);
                else
                    EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, item,
                        slot);
            }
            return 1;
        case 0x0e:
            // Offhand (dual wield), then a double attack with enough double attack skill.
            if (EQ_METHOD(1, u8, 0x0041D55B, (void*, EQPlayer*), ch)(target) != 1)
                break;
            if (target == 0)
                new PlayerMissile_guess(item, self, 0, (const char*)0x0053E4BC, slot, 0);
            else
                EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, item,
                    slot);
            if (ch->skills[20] > 149 && EQ_METHOD(1, u8, 0x0041D5BC, (void*, EQPlayer*), ch)(target) == 1)
            {
                if (target == 0)
                    new PlayerMissile_guess(item, self, 0, (const char*)0x0053E4BC, slot, 0);
                else
                    EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, item,
                        slot);
            }
            break;
        }
    }
    else
    {
        switch (slot)
        {
        case 0x64:
        {
            // Combat skill ('d'): uses the fallback item with the skill's damage, on the skill's own timer.
            u32* skillTime;
            item = EQ_GLOBAL(EQ_Equipment*, 0x005FE850);
            if (item == 0)
                break;
            item->damage = EQ_METHOD(1, s8, 0x0041D5F4, (void*, u8), ch)(skill);
            item->item_type = 0x16;
            item->delay = skill;
            if (skill == 8 &&
                (ch->inventory_items[13] == 0 || ((EQ_Equipment*)ch->inventory_items[13])->item_type != 2) &&
                self->type == 0)
            {
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                    (const char*)0x005687F4);
                break;
            }
            delay = self->actor_info->unk_0x164;
            delay = self->ModifyAttackSpeed(delay, 0);
            skillTime = &self->actor_info->lastAttackTime_guess;
            now = EqGetTime();
            if (now - *skillTime < delay &&
                (self->type != 1 || self->actor_info == 0 || self->actor_info->unk_0x2E0 <= 1))
                break;
            *skillTime = now;
            self->actor_info->unk_0x164 = *(u32*)(g_skill_records_guess[skill] + 0x20);
            if (self->stand_state != 'd')
            {
                if (self->stand_state == 'f')
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                        (const char*)0x005410A8, 0xd);
                else
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                        (const char*)0x0056883C, 0xd);
                return 1;
            }
            if (!EQ_METHOD(2, char, 0x004B2975, (EQPlayer*, EQPlayer*, float), self)(target, 0.0f) &&
                EQ_GLOBAL(u8, 0x006B63F8) == 0)
            {
                if (target != 0)
                    EQTRACE_guess((const char*)0x005687E0, target->name);
                break;
            }
            if (target == 0)
                new PlayerMissile_guess(item, self, 0, (const char*)0x0053E4BC, slot, 0);
            else
                EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, item,
                    slot);
            break;
        }
        case 0x65:
            // 'e': a swing with the primary (hand to hand when it is empty), on no timer.
            item = (EQ_Equipment*)ch->inventory_items[13];
            if (item == 0)
            {
                item = EQ_GLOBAL(EQ_Equipment*, 0x005FE850);
                item->damage = EQ_METHOD(1, s8, 0x0041D5F4, (void*, u8), ch)(0x1c);
                item->item_type = 0x16;
                item->delay = 0x1c;
            }
            if (self->stand_state != 'd')
            {
                if (self->stand_state == 'f')
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                        (const char*)0x005410A8, 0xd);
                else
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                        (const char*)0x0056883C, 0xd);
                return 1;
            }
            if (!EQ_METHOD(2, char, 0x004B2975, (EQPlayer*, EQPlayer*, float), self)(target, 0.0f) &&
                EQ_GLOBAL(u8, 0x006B63F8) == 0)
            {
                if (target != 0)
                    EQTRACE_guess((const char*)0x005687E0, target->name);
                return 1;
            }
            if (target != 0)
            {
                EQ_CDECL(void, 0x004A4327, (EQPlayer*, EQPlayer*, EQ_Equipment*, u8))(self, target, item, 0x0d);
                break;
            }
            new PlayerMissile_guess(item, self, 0, (const char*)0x0053E4BC, 0x0d, 0);
            return 1;
        }
    }
done:
    return 1;
}

// FUNCTION: TRILOGY 0x004b3a16
// Keeps the ranged weapon supplied with matching ammunition and reports when it runs out.
// __thiscall. Returns 0 for non-ammunition item types, 1 when ammunition is loaded,
// and 2 when the item cannot be loaded or ammunition is exhausted.
u8 __fastcall HandleAmmo(EQPlayer* self, int edx)
{
    char itemType;
    int charInfo;
    int rangedItem;
    int ammoItem;
    bool outOfAmmo;
    int newAmmo;
    u32 message[3];

    charInfo = *(int*)((u32)self + 0x74);
    rangedItem = *(int*)((u32)charInfo + 0x10C);
    itemType = *(char*)((u32)rangedItem + 0xC2);
    if (((itemType != '\x13') && (itemType != '\x05')) && (itemType != '\x06'))
    {
        return 0;
    }
    ammoItem = *(int*)((u32)charInfo + 0x134);
    newAmmo = 0;
    outOfAmmo = true;
    if (ammoItem == 0)
    {
        if ((itemType == '\x13') && (*(u8*)((u32)rangedItem + 0xDA) > 1))
        {
            newAmmo = (int)new PlayerEquipment_guess;
            memcpy(
                (void*)newAmmo, (const void*)rangedItem, 0x124);
            message[0] = 0xB;
            message[1] = 0x15;
            message[2] = (u32)(*(u8*)((u32)rangedItem + 0xDA) - 1);
            send_message_guess(
                g_world_connection_guess, 0x212C, message, 0xC, 1);
            *(u8*)((u32)newAmmo + 0xDA) = (u8)(*(u8*)((u32)rangedItem + 0xDA) - 1);
            *(u8*)((u32)rangedItem + 0xDA) = 1;
            *(int*)((u32)charInfo + 0x134) = newAmmo;
            outOfAmmo = false;
        }
    }
    else
    {
        if ((itemType == '\x13') && (*(short*)((u32)ammoItem + 0x82) == *(short*)((u32)rangedItem + 0x82)))
        {
            outOfAmmo = false;
        }
        if ((itemType == '\x05') && (*(char*)((u32)ammoItem + 0xC2) == '\x1b'))
        {
            outOfAmmo = false;
        }
        if ((itemType == '\x06') && (*(char*)((u32)ammoItem + 0xC2) == '\x1c'))
        {
            outOfAmmo = false;
        }
    }
    if (outOfAmmo)
    {
        EQ_THISCALL(void, 0x004D05B7, (void*, int, const char*))(
            pinstCEverQuest_guess, 0, (const char*)0x005688E8);
        return 2;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004b3b47
// Starts the spell's impact visuals and any associated caster animation for a hit event.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// @early-stop: 4 EQ_THISCALL dummy-EDX xors; effectCoordinates store sits after fld/push (G null has no separate join before the compute)
void __fastcall EQPlayer_TriggerSpellEffect_guess(EQPlayer* spawn, int edx, u32* hitData)
{
    typedef float (__cdecl *FloatFunction_guess)(float);
    struct LocalCoordinates_guess
    {
        float local_18;
        float local_14;
        u32 local_10;
    } localCoordinates;
    u8 effectType;
    short spellId;
    int spellData;
    int casterSpawn;
    int usesDragonBreathEffect;
    int targetSpawn;
    float* effectCoordinates;

    effectCoordinates = (float*)0;
    spellId = *(short*)((u32)hitData + 0x20);
    if ((spellId < 0) || (2999 < spellId))
    {
        EQTRACE_guess(
            (const char*)0x00568904, (int)spellId);
        return;
    }
    spellData = (int)EQ_ARRAY(void*, 0x005FE9A0)[spellId];
    if (spellData == 0)
    {
        return;
    }
    if (*(char*)((u32)spellData + 0x22E) == '\0')
    {
        return;
    }
    casterSpawn = EQ_METHOD(1, int, 0x004B1416, (void*, int), spawn)((int)hitData[1]);
    targetSpawn = EQ_METHOD(1, int, 0x004B1416, (void*, int), spawn)((int)*hitData);
    if ((casterSpawn != 0) &&
        (usesDragonBreathEffect = EQ_METHOD(0, int, 0x004875F6, (void*), (void*)spellData)(), usesDragonBreathEffect == 0))
    {
        EQ_GLOBAL(int, 0x006D6FE0) = 1;
        if ((*(char*)((u32)casterSpawn + 0x99) != '\b') || (10 < (int)hitData[3]))
        {
            EQ_METHOD(6, void, 0x0048ED3B, (void*, int, int, int, int, float, int), g_display_guess)(casterSpawn,
                (int)*(u8*)((u32)spellData + 0x254), 1, 0, 1.0f, 1);
        }
        EQ_GLOBAL(int, 0x006D6FE0) = 0;
    }
    effectType = *(u8*)((u32)spellData + 0x222);
    switch (effectType)
    {
    case 'G':
        effectCoordinates = &localCoordinates.local_18;
        if (targetSpawn != 0)
        {
            localCoordinates.local_18 = *(float*)((u32)targetSpawn + 0x34);
            localCoordinates.local_14 = *(float*)((u32)targetSpawn + 0x38);
            localCoordinates.local_10 = *(u32*)((u32)targetSpawn + 0x3C);
            break;
        }
        // fall through: no target spawn, so the origin is this player's own position
    case '!':
    case 'j':
    case 'l':
        effectCoordinates = &localCoordinates.local_18;
        localCoordinates.local_18 = EQ_GLOBAL(FloatFunction_guess, 0x006E524C)(
            spawn->heading) *
            *(float*)((u32)spellData + 0x1A0) +
            spawn->y;
        localCoordinates.local_14 = EQ_GLOBAL(FloatFunction_guess, 0x006E5250)(
            spawn->heading) *
            *(float*)((u32)spellData + 0x1A0) +
            spawn->x;
        localCoordinates.local_10 = spawn->z_bits_guess;
        break;
    }
    EQ_CDECL(void, 0x004C85F4, (int, int, int, int, u32, u32, u32))(
        2, spellData, casterSpawn, (int)spawn, (u32)effectCoordinates, 0, 0);
    return;
}

#define EQPLAYER_SPELL_HIT_TYPE (-0x19)

// FUNCTION: TRILOGY 0x004B3C9D
// Processes a hit on a spawn, applying spell effects or physical-hit knockback and state changes.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
char EQPlayer::IWasHit(u32* hitData)
{
    typedef float (__cdecl *FloatFunction_guess)(float);
    void* character;
    EQPlayer* localPlayer;
    EQPlayer* controlPlayer;
    char result;

    result = 0;
    character = this->char_info_guess;
    if (*(char*)((u32)hitData + 0x1C) == EQPLAYER_SPELL_HIT_TYPE)
    {
        EQ_METHOD(1, void, 0x004B3B47, (void*, u32*), this)(hitData);
        if ((character != 0) && ((*(u8*)((u32)hitData + 0x23) & 4) != 0))
        {
            EQ_METHOD(1, void, 0x00420505, (void*, void*), character)(hitData);
        }
    }
    else if (character != 0)
    {
        *(u8*)(*(u32*)(*(u32*)((u32)character + 0xA0) + 0x70) + 0x18A) = result;
        *(u8*)(*(u32*)((u32)character + 0xA0) + 0x9E) = result;
        *(u8*)(*(u32*)((u32)character + 0xA0) + 0x9F) = result;
        EQ_METHOD(1, void, 0x00424E22, (void*, int), character)(
            EQ_METHOD(2, int, 0x0042381B, (void*, int, int), character)(0x0C, 0));
        EQ_METHOD(1, void, 0x00424E22, (void*, int), character)(
            EQ_METHOD(2, int, 0x0042381B, (void*, int, int), character)(0x1C, 0));
        EQ_METHOD(1, void, 0x00424E22, (void*, int), character)(
            EQ_METHOD(2, int, 0x0042381B, (void*, int, int), character)(0x1D, 0));
        result = 1;
        if (*(float*)((u32)hitData + 0x10) != EQ_GLOBAL(float, 0x0053510C))
        {
            this->dy += EQ_GLOBAL(FloatFunction_guess, 0x006E524C)(
                *(float*)((u32)hitData + 0x14)) * *(float*)((u32)hitData + 0x10);
            this->dx += EQ_GLOBAL(FloatFunction_guess, 0x006E5250)(
                *(float*)((u32)hitData + 0x14)) * *(float*)((u32)hitData + 0x10);
            this->dz += EQ_GLOBAL(FloatFunction_guess, 0x006E5250)(
                *(float*)((u32)hitData + 0x18)) * *(float*)((u32)hitData + 0x10);
            localPlayer = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
            controlPlayer = EQ_GLOBAL(EQPlayer*, 0x006E4DA0);
            if ((this == localPlayer) && (controlPlayer != localPlayer))
            {
                EQ_GLOBAL(u8, 0x006B710F) = result;
            }
            else if (this == controlPlayer)
            {
                EQ_GLOBAL(u8, 0x006B710E) = 1;
            }
        }
    }
    return result;
}

#undef EQPLAYER_SPELL_HIT_TYPE

// FUNCTION: TRILOGY 0x004b3dba
// Returns the local character to conscious standing play after unconsciousness.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void __fastcall EQPlayer_GetConscious_guess(EQPlayer* spawn, int edx)
{
    u32 characterInfo;

    if (spawn == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
    {
        characterInfo = (u32)spawn->char_info_guess;
        if (characterInfo == 0)
        {
            EQTRACE_guess(
                (const char*)0x0056894C, spawn->name);
            return;
        }
        if (*(char*)(characterInfo + 0x76) < '\x03')
        {
            return;
        }
        *(u8*)(characterInfo + 0x76) = 0;
        EQ_METHOD(0, void, 0x004C70CB, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
        EQ_GLOBAL(u32, 0x00590154) = 0;
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00568930, 0xD);
    }
    spawn->stand_state = 'i';
    EQ_METHOD(1, void, 0x004B45CB, (void*, int), spawn)(100);
    return;
}

// FUNCTION: TRILOGY 0x004b3e1c
// Knocks a spawn unconscious and stops its movement, updating the local player's status when applicable.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void EQPlayer::KnockedOut(void)
{
    u32 characterInfo;

    if (EQ_GLOBAL(u8, 0x006B63F8) == 0
        && EQ_GLOBAL(u32, 0x006B6494) == 0)
    {
        EQ_METHOD(1, void, 0x004B45CB, (void*, int), this)(0x73);
        if (this == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            characterInfo = (u32)this->char_info_guess;
            if (characterInfo == 0)
            {
                EQTRACE_guess(
                    (const char*)0x005689A8, this->name);
                return;
            }
            if (*(char*)(characterInfo + 0x76) > '\0')
            {
                return;
            }
            *(u8*)(characterInfo + 0x76) = 3;
            EQ_METHOD(0, void, 0x004C70CB, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
            EQ_GLOBAL(u32, 0x00590154) = 1;
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00568984, 0xF);
        }
    }
    this->delta_heading = 0.0f;
    this->unk_0x5C = 0.0f;
    this->speed = 0.0f;
    return;
}

// FUNCTION: TRILOGY 0x004b3e99
// Turns a dead player or NPC spawn into a corpse and updates its appearance in the zone.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void __fastcall EQPlayer_IDied_guess(EQPlayer* spawn, int edx, void* deathMessage)
{
    EQPlayer* pPlayer;

    if (spawn->type < 2)
    {
        if (EQ_GLOBAL(u32, 0x006E4DDC) != 0)
        {
            EQ_METHOD(1, bool, 0x004E25B5, (void*, EQPlayer*), g_environment_ptr_guess)(spawn);
        }
        if (spawn == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            LogMessage_guess(
                (const char*)0x005689E0);
            pPlayer = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(
                *(int*)((char*)deathMessage + 4));
            if (pPlayer == 0)
            {
                EQ_GLOBAL(u32, 0x00568308) = 1;
            }
            else
            {
                EQ_GLOBAL(u32, 0x00568308) = pPlayer->type;
            }
            *(u32*)((char*)spawn->actor_info + 0x68) = 0;
        }
        else
        {
            *(u32*)((char*)spawn->actor_info + 0x68) =
                EQ_CDECL(u32, 0x004A461E, (void))();
        }
        EQ_METHOD(0, void, 0x004B3E1C, (void*), spawn)();
        if (spawn->type == 0)
        {
            sprintf(
                spawn->name, (const char*)0x00561974, spawn->name,
                (u32)*(u16*)((char*)deathMessage + 8));
        }
        else
        {
            EQ_CDECL(void, 0x004A0860, (char*, u16))(
                spawn->name, *(u16*)((char*)deathMessage + 8));
        }
        spawn->invis = 0;
        spawn->type = 2;
        EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_display_guess)(spawn);
        spawn->levitate = 0;
        spawn->delta_heading = 0.0f;
        spawn->stand_state = 'x';
        spawn->unk_0x5C = 0.0f;
        spawn->speed = 0.0f;
        spawn->dy = 0.0f;
        spawn->dx = 0.0f;
        spawn->dz = 0.0f;
        spawn->actor_info->unknown0x088 = 1;
        if (spawn == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            spawn->actor_info->unk_0x2C0 = 1;
        }
    }
    return;
}

#define EQ_SWITCH_TYPE_DAMAGE_START 0x73

struct EQPlayerTouchingSwitchDamage_guess
{
    u32 spawn_id;
    u32 unknown0x04;
    u16 unknown0x08;
    u16 unknown0x0A;
    int damage;
    float unknown0x10;
    float unknown0x14;
    float unknown0x18;
    u8 message_type;
    u8 unknown0x1D;
    u16 unknown0x1E;
    u16 switch_type;
    u8 unknown0x22;
    u8 unknown0x23;
};

// FUNCTION: TRILOGY 0x004b3fcb
// Triggers special switch effects when a spawn touches them, such as teleporting or trap damage.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void __fastcall EQPlayer_TouchingSwitch_guess(EQPlayer* player, int unused_edx)
{
    EQActorInfo_guess* actor_info;
    EQSwitch* current_switch;
    u8 switch_type;
    u32* last_switch_touch_time;
    EQSwitch** touching_switch;
    u32 current_time;
    u32 switch_delay;
    int roll;
    void* target_info;
    int destination_id;
    u32 teleport_entry;
    int zone_file_exists;
    short switch_damage;
    char was_hit;
    EQPlayerTouchingSwitchDamage_guess damage_message;

    actor_info = player->actor_info;
    touching_switch = (EQSwitch**)((char*)actor_info + 0x1B8);
    current_switch = *touching_switch;
    if (current_switch->state == 4)
    {
        *touching_switch = 0;
        return;
    }

    switch_type = current_switch->type;
    if (switch_type < EQ_SWITCH_TYPE_DAMAGE_START)
    {
        if (switch_type == 0x23)
        {
            current_time = g_cdisplay_guess->time_ms_guess;
            last_switch_touch_time = &actor_info->lastSwitchTouchTime_guess;
            if ((u32)(current_time - *last_switch_touch_time) <= 1000)
                goto clear_touching_switch;

            *last_switch_touch_time = current_time;
            roll = EQ_CDECL(int, 0x004A473E, (void))();
            if (roll <= 50)
                goto clear_touching_switch;

            EQ_METHOD(3, void, 0x004BE44E, (EQSwitch*, u32, int, int), current_switch)(0, 0, 0);
        }
        else if (switch_type == 0x39)
        {
            current_time = g_cdisplay_guess->time_ms_guess;
            last_switch_touch_time = &actor_info->lastSwitchTouchTime_guess;
            if ((u32)(current_time - *last_switch_touch_time) <= 2000)
                goto clear_touching_switch;

            *last_switch_touch_time = current_time;
            if (player->type != 0 || player->char_info_guess == 0)
                goto clear_touching_switch;

            target_info = *(void**)((char*)player->char_info_guess + 0xE0);
            destination_id = -1;
            if (target_info != 0)
                destination_id = (destination_id & 0xFFFF0000) | *(u16*)((char*)target_info + 0x82);

            teleport_entry = EQ_CDECL(u32, 0x004A3D5C, (int))(current_switch->param_guess);
            zone_file_exists = EQ_CDECL(int, 0x004A3D1F, (u32))(teleport_entry);
            if (zone_file_exists != 1)
                goto clear_touching_switch;

            EQTRACE_guess((const char*)0x00568A24);
            EQ_METHOD(3, void, 0x004BE44E, (EQSwitch*, u32, int, int), current_switch)(
                player->spawn_id, destination_id, 0);
        }
        else
        {
            goto clear_touching_switch;
        }
    }
    else
    {
        switch_delay = 1000;
        if ((u32)EQ_METHOD(0, int, 0x004BDDF1, (EQSwitch*), current_switch)() < 1000)
            switch_delay = 400;

        if (EQ_GLOBAL(EQSwitch*, 0x005FE854) == current_switch)
        {
            current_time = g_cdisplay_guess->time_ms_guess;
            last_switch_touch_time = &actor_info->lastSwitchTouchTime_guess;
            if ((u32)(current_time - *last_switch_touch_time) < switch_delay)
                return;
        }

        EQ_GLOBAL(EQSwitch*, 0x005FE854) = current_switch;
        damage_message.spawn_id = player->spawn_id;
        damage_message.unknown0x04 = 0;
        damage_message.unknown0x08 = 200;
        switch_damage = EQ_METHOD(0, short, 0x004BDEE6, (EQSwitch*), current_switch)();
        damage_message.damage = (int)switch_damage;
        damage_message.unknown0x10 = 0.0f;
        damage_message.unknown0x14 = 0.0f;
        damage_message.unknown0x18 = 0.0f;
        damage_message.message_type = 0xFD;
        damage_message.unknown0x1D = 0;
        damage_message.unknown0x1E = 200;
        damage_message.switch_type = (u16)current_switch->type;
        damage_message.unknown0x22 = 0;
        damage_message.unknown0x23 = 0;
        EQTRACE_guess((const char*)0x00568A0C);
        was_hit = EQ_METHOD(1, char, 0x004B3C9D, (EQPlayer*, void*), player)(&damage_message);
        if (was_hit != 0)
        {
            current_time = g_cdisplay_guess->time_ms_guess;
            last_switch_touch_time = &actor_info->lastSwitchTouchTime_guess;
            *last_switch_touch_time = current_time;
        }
    }

clear_touching_switch:
    *touching_switch = 0;
    return;
}

#undef EQ_SWITCH_TYPE_DAMAGE_START

// FUNCTION: TRILOGY 0x004b41a6
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::MyFeetAreOnGround, high)
// __thiscall. The feet (z - height offset, moved by this frame's dz) are at most 0.501 above the floor.
bool EQPlayer::MyFeetAreOnGround(void)
{
    float floor_z = actor_info->floor_z + 0.5f;

    if (z - height_offset + dz > floor_z + 0.001f)
        return false;
    return true;
}

struct EQPlayer_IHaveFallen_guess
{
    char unknown0x00[0x34];
    float y;
    float x;
    float z;
    char unknown0x40[0x34];
    void* character;
    char unknown0x78[0x20];
    unsigned char type;

    void IHaveFallen(float fallDamageAmount);
};

// FUNCTION: TRILOGY 0x004b41ce
// Handles a fall by applying damage and playing the matching landing sound.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void EQPlayer_IHaveFallen_guess::IHaveFallen(float fallDamageAmount)
{
    short damageTaken;
    int soundId;

    if (character != 0)
    {
        damageTaken = EQ_METHOD(1, short, 0x0042444C, (void*, float), character)(fallDamageAmount);
        soundId = (damageTaken >= 1) + 0x72;
        if (type != 0)
            soundId = -1;
        if ((short)soundId != -1)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(y, x, z, soundId);
    }
    return;
}

// FUNCTION: TRILOGY 0x004b422a
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::SkillUsed, high)
// __thiscall that ignores `this`. Skill id -> the attack animation it plays.
u8 EQPlayer::SkillUsed(u8 skill)
{
    switch (skill)
    {
    case 0:
        return 1;
    case 1:
        return 3;
    case 2:
    case 0x23:
        return 0x24;
    case 3:
        return 0;
    case 4:
        return 2;
    case 7:
    case 0x13:
        return 0x33;
    case 9:
        return 0xE7;
    case 5:
    case 6:
    case 0x11:
    case 0x1B:
    case 0x1C:
        return 7;
    }
    return 0x1C;
}

// FUNCTION: TRILOGY 0x004b4292
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// Free fastcall entry preserves the thiscall register/stack layout because the method is not declared in the headers.
void __fastcall bodyEnvironmentChange(EQPlayer* spawn, int edx, u8 environment)
{
    u8 spawnLight;
    u32 characterInfo;
    EQActorInfo_guess* actorInfo;
    int itemCheck;

    characterInfo = (u32)spawn->char_info_guess;
    spawn->actor_info->unk_0x9B = spawn->actor_info->unknown0x099;
    spawn->actor_info->unknown0x099 = environment;
    if (spawn->levitate == 3)
    {
        actorInfo = spawn->actor_info;
        if (actorInfo->unknown0x099 >= 5)
            actorInfo->unknown0x088 = 0;
        else
            actorInfo->unknown0x088 = 1;
    }
    if ((environment == 5 || environment == 6 || environment == 7 || environment == 8
        || (8 < environment && environment <= 10))
        && (*(int*)((u32)spawn->actor_info + 4) != 0
        && ((spawnLight = spawn->light, spawnLight == 1 || spawnLight == 2 || spawnLight == 4 || spawnLight == 6)
        && (spawn == g_local_player_guess && characterInfo != 0))))
    {
        itemCheck = *(int*)(characterInfo + 0x118);
        if (itemCheck != 0)
        {
            itemCheck = strcmp(
                (const char*)(itemCheck + 0x5F), (const char*)0x0053E148);
            if (itemCheck == 0)
            {
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(spawn, 8, (const char*)0x0053E140);
            }
        }
        EQ_METHOD(1, void, 0x00410970, (void*, int), g_display_guess)(*(int*)((u32)spawn->actor_info + 4));
        *(int*)((u32)spawn->actor_info + 4) = 0;
        spawn->light = 0;
        EQ_METHOD(2, void, 0x004D9D4D, (void*, EQPlayer*, u8), pinstCEverQuest_guess)(spawn, 0);
    }
    if (spawn->type == 1 && EQ_GLOBAL(int, 0x006E4D4C) != 0 && spawn->actor_info != 0)
    {
        spawn->actor_info->unknown0x088 = 0;
        spawn->levitate = 1;
    }
    return;
}

// FUNCTION: TRILOGY 0x004b43ab
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// Free fastcall entry preserves the thiscall register/stack layout because the method is not declared in the headers.
// @early-stop: the cmp dl,7 test sits after the 8..10 test (original: before 8); EQ_THISCALL's dummy EDX adds xor edx,edx
void __fastcall feetEnvironmentChange(EQPlayer* spawn, int edx, u8 environment)
{
    u8 previousEnvironment;
    u8* environmentSlot;
    u32 soundId;

    environmentSlot = (u8*)((u32)spawn->actor_info + 0x9A);
    previousEnvironment = *environmentSlot;
    if (previousEnvironment == environment)
        return;
    soundId = 0xffffffff;
    *environmentSlot = environment;
    if (previousEnvironment > 0 && environment == 0)
        soundId = 99;
    // 5/6 and 8..10 both run this check; VC6 merges the two copies into the one block the original has.
    if (environment == 5 || environment == 6)
    {
        if (spawn->dz >= 1.0f || !(spawn->dz > -1.0f))
            soundId = 100;
    }
    else if (environment == 8 || (environment > 8 && environment <= 10))
    {
        if (spawn->dz >= 1.0f || !(spawn->dz > -1.0f))
            soundId = 100;
    }
    else if (environment == 7)
        soundId = 99;
    if ((short)soundId == -1)
        return;
    EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(spawn->y, spawn->x, spawn->z, soundId);
    return;
}

// FUNCTION: TRILOGY 0x004b443e
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// @early-stop: fastcall shim shifts the stack argument and reverses this/wave-ID registers; call wrappers also clear EDX.
// Free fastcall entry preserves the thiscall register/stack layout because the method is not declared in the headers.
void __fastcall headEnvironmentChange_guess(EQPlayer* spawn, int edx, u8 newEnvironment)
{
    u8 previousEnvironment;
    s16 effectId;

    previousEnvironment = spawn->actor_info->environment_guess;
    if (previousEnvironment == newEnvironment)
        return;
    effectId = 0xa1;
    if (previousEnvironment != 5)
    {
        if (previousEnvironment == 6 || (previousEnvironment != 7 && previousEnvironment > 7 && previousEnvironment <= 10))
        {
            if (g_sound_manager_ptr_guess != 0)
            {
                EQ_METHOD(1, void, 0x004E363B, (void*, int), g_sound_manager_ptr_guess)(effectId);
            }
        }
        else if (previousEnvironment == 7)
        {
            if (g_sound_manager_ptr_guess != 0)
            {
                EQ_METHOD(1, void, 0x004E363B, (void*, int), g_sound_manager_ptr_guess)(0xa0);
            }
        }
    }
    else
    {
        if (g_sound_manager_ptr_guess != 0)
        {
            EQ_METHOD(1, void, 0x004E363B, (void*, int), g_sound_manager_ptr_guess)(effectId);
        }
        EQ_GLOBAL(u8, 0x00579324) = 1;
        if (g_environment_ptr_guess != 0)
        {
            EQ_METHOD(0, void, 0x004E25FD, (void*), g_environment_ptr_guess)();
            EQ_METHOD(1, void, 0x004E1F74, (void*, int), g_environment_ptr_guess)(-1);
        }
    }

    spawn->actor_info->environment_guess = newEnvironment;
    if (newEnvironment != 5)
    {
        if (newEnvironment != 6)
        {
            if (newEnvironment != 7)
            {
                if (newEnvironment <= 7)
                    return;
                if (10 < newEnvironment)
                    return;
            }
            else
                effectId = 0xa0;
        }
    }
    else
    {
        EQ_GLOBAL(u8, 0x00579324) = 0;
        if (g_sound_manager_ptr_guess != 0)
        {
            EQ_METHOD(0, void, 0x004A4069, (void*), g_sound_manager_ptr_guess)();
        }
        if (g_environment_ptr_guess != 0)
        {
            EQ_METHOD(1, void, 0x004E1F74, (void*, int), g_environment_ptr_guess)(4);
        }
    }
    if (g_sound_manager_ptr_guess != 0)
    {
        EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(effectId, 0);
    }
    return;
}

// FUNCTION: TRILOGY 0x004b4531
// Gates player character visuals and animation based on whether a race is supported.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::LegalPlayerRace, low)
// __thiscall. 1 for a playable race (1..12, iksar 128, vah shir 130; also anything below 1); race -1
// checks the spawn's own race (unsigned there).
int EQPlayer::LegalPlayerRace_guess(int race)
{
    if (race != -1)
    {
        if (race <= 12 || race == 0x80 || race == 0x82)
            return 1;
    }
    else if (this->race <= 12 || this->race == 0x80 || this->race == 0x82)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004b4868
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::IsRoleplaying, low)
// __thiscall. The anon field (0xFC) is above 1 (role-play flag).
bool EQPlayer::IsRoleplaying_guess(void)
{
    if (anon > 1)
        return true;
    return false;
}

// FUNCTION: TRILOGY 0x004b4873
// __fastcall on EQPlayer (ECX = self). Fills the sound-id table at [self+0x70]+0x1dc..0x220 from the race at
// self+0x9a and the gender/variant byte at self+0x9c.
// Maps the race to a sound group, then stores a fixed set of ids for the default group or one of the special
// groups; group values from 0xd6 up call 0x004B4531 first, and one group also loads sound files via 0x004E37F4.
// Most groups end with the same common stores at +0x218..+0x220 and +0x20c..+0x214.
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
void __fastcall EQPlayer_SetSounds_guess(int self)

{
  char variant;
  unsigned short raceId;
  unsigned short uVar3;
  int tmp;
  char *path;
  unsigned int soundId;

  raceId = *(unsigned short *)(self + 0x9a);
  uVar3 = raceId;
  if (raceId < 0x52) {
    if (raceId == 0x51) {
      uVar3 = 0xb;
    }
    else if ((((raceId == 0x2c) || (raceId == 0x37)) || (raceId == 0x43)) || (raceId == 0x47)) {
      uVar3 = 1;
    }
    else if (raceId == 0x4d) {
      uVar3 = 6;
    }
    else {
      if (raceId != 0x4e) goto checkRaceRange;
      uVar3 = 3;
    }
defaultSounds:
    if ((*(char *)(self + 0x9c) == '\x01') || (raceId == 0xc1)) {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x2a;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x2b;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x2c;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x2d;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x2e;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x2f;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x30;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x31;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x32;
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x20;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x21;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x22;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x23;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x24;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x25;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x26;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x27;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x28;
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0x62;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x82;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0x81;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
  }
  else {
    if (raceId != 0x5a) {
      if (raceId == 0x5c) {
        uVar3 = 9;
      }
      else if (raceId == 0x5d) {
        uVar3 = 10;
      }
      else if (raceId == 0x5e) {
        uVar3 = 8;
      }
      else if (raceId == 0x6a) {
        uVar3 = 5;
      }
      else {
        if (raceId != 0x70) goto checkRaceRange;
        uVar3 = 4;
      }
      goto defaultSounds;
    }
    uVar3 = 2;
checkRaceRange:
    if (uVar3 < 0xd6) goto defaultSounds;
  }
  tmp = EQ_METHOD(1, int, 0x004B4531, (EQPlayer *, int), (EQPlayer *)self)(uVar3);
  if (tmp != 0) {
    if ((uVar3 == 0xb) || (uVar3 == 0xc)) {
      variant = *(char *)(self + 0x9c);
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
      if (variant == '\x01') {
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x3e;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x3f;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x40;
        *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x41;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x42;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x43;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x44;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x45;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x46;
      }
      else {
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x34;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x35;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x36;
        *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x37;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x38;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x39;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x3a;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x3b;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x3c;
      }
commonTail:
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0x62;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x82;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0x81;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    }
    else {
      if ((((uVar3 == 2) || (uVar3 == 9)) || (uVar3 == 10)) || (uVar3 == 8)) {
        variant = *(char *)(self + 0x9c);
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
        if (variant == '\x01') {
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x52;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x53;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x54;
          *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x55;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x56;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x57;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x58;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x59;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x5a;
        }
        else {
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x48;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x49;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x4a;
          *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x4b;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x4c;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x4d;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x4e;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x4f;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x50;
        }
        goto commonTail;
      }
      if (uVar3 == 0x80) {
        variant = *(char *)(self + 0x9c);
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
        if (variant == '\x01') {
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x52;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x17d;
          *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x17d;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x17d;
          if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
            EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578110),0x17d);
          }
          *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x57;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x58;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x17c;
          if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
            EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057810C),0x17c);
          }
          *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x5a;
        }
        else {
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x48;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x17d;
          *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x17d;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x17d;
          if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
            EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578110),0x17d);
          }
          *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x57;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x58;
          *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x17c;
          if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
            EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057810C),0x17c);
          }
          *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x50;
        }
        *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0x62;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x81;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
        *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x82;
      }
    }
    if (*(short *)(self + 0x9a) == 0x37) {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x5b;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x5b;
    }
    goto finish;
  }
  switch(uVar3) {
  case 0xd:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (*(char *)(self + 0x9c) != '\0') goto case60Body;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x8a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x8b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x21f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x21f;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x21f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x21f;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578398),0x21f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x220;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057839C),0x220);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x21e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x21e;
    path = EQ_GLOBAL(char *, 0x00578394);
    break;
  case 0xe:
  case 0x85:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x3f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x3f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x161;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x161;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x161;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x161;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780A0),0x161);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x160;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057809C),0x160);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x15f;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto clear210;
    soundId = 0x15f;
    path = EQ_GLOBAL(char *, 0x00578098);
    goto loadSoundAndClear;
  case 0xf:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xd5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xd5;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xd5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xd5;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E70),0xd5);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xd4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E6C),0xd4);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1c8;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1c8;
      path = EQ_GLOBAL(char *, 0x0057823C);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x10:
    variant = *(char *)(self + 0x9c);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x2a;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x2b;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x2c;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x2d;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x2e;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x2f;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x30;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x31;
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x20;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x21;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x22;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x23;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x24;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x25;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x26;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x27;
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x49;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x4a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x82;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0x81;
    goto clear214;
  case 0x11:
  case 0xa0:
  case 0xa4:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x28;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x28;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x112;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x112;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x112;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x112;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F64),0x112);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x111;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F60),0x111);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x110;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x110;
      path = EQ_GLOBAL(char *, 0x00577F5C);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x12:
  case 0x8c:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x22;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x22;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x109;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x109;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x109;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x109;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F40),0x109);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x108;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F3C),0x108);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x24;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x107;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x107;
      path = EQ_GLOBAL(char *, 0x00577F38);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x13:
  case 0xb8:
  case 0xc3:
  case 0xc4:
  case 0xc6:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    tmp = *(int *)(self + 0x70);
    goto case13Body;
  case 0x14:
  case 0x80:
  case 0x8b:
    variant = *(char *)(self + 0x9c);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x52;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x17d;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x17d;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x17d;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578110),0x17d);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x57;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x58;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x17c;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057810C),0x17c);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x5a;
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0x48;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x17d;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x17d;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x17d;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578110),0x17d);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0x57;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0x58;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x17c;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057810C),0x17c);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0x50;
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0x62;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x81;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x82;
    goto finish;
  case 0x15:
  case 0x1e:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 9;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 10;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 10;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xcc;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xcc;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xcc;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xcc;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E4C),0xcc);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xcb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E48),0xcb);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xca;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0xca;
      path = EQ_GLOBAL(char *, 0x00577E44);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x16:
  case 0x81:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xc9;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xc9;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xc9;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xc9;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0xc9;
      path = EQ_GLOBAL(char *, 0x00577E40);
case16Load:
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(path,soundId);
    }
    goto case16Tail;
  case 0x17:
    variant = *(char *)(self + 0x9c);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0xd;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0xd;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xd8;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xd8;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xd8;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xd8;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E7C),0xd8);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xd7;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E78),0xd7);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xd6;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E74),0xd6);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1cd;
      if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
      soundId = 0x1cd;
      path = EQ_GLOBAL(char *, 0x00578250);
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0xe;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0xe;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xdb;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xdb;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xdb;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xdb;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E88),0xdb);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xda;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E84),0xda);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xd9;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E80),0xd9);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1cc;
      if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
      soundId = 0x1cc;
      path = EQ_GLOBAL(char *, 0x0057824C);
    }
    break;
  case 0x18:
  case 0x69:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1af;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1af;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1af;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1af;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781D8),0x1af);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1ae;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781D4),0x1ae);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1c6;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1c6;
      path = EQ_GLOBAL(char *, 0x00578234);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x19:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x19;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xee;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xee;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xee;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xee;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577ED4),0xee);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xed;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577ED0),0xed);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xec;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577ECC),0xec);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1ba;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1ba;
    path = EQ_GLOBAL(char *, 0x00578204);
    break;
  case 0x1a:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x1a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x1a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xf1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xf1;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xf1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xf1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EE0),0xf1);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xf0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EDC),0xf0);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x1c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x1b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xef;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577ED8),0xef);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0xf2;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EE4),0xf2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0xf3;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0xf3;
    path = EQ_GLOBAL(char *, 0x00577EE8);
    break;
  case 0x1b:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x1d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x1d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xf6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xf6;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xf6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xf6;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EF4),0xf6);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xf5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EF0),0xf5);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x1c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x1b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xf4;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EEC),0xf4);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0xf7;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EF8),0xf7);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0xf8;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0xf8;
    path = EQ_GLOBAL(char *, 0x00577EFC);
    break;
  case 0x1c:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x1e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x1e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xfb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xfb;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xfb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xfb;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F08),0xfb);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xfa;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F04),0xfa);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    goto case68Tail;
  case 0x1d:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x6a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x1f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x1f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xff;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xff;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xff;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xff;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F18),0xff);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xfe;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F14),0xfe);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xfd;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0xfd;
      path = EQ_GLOBAL(char *, 0x00577F10);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x1f:
  case 0x91:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x11;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x12;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xe0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xe0;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xe0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xe0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E9C),0xe0);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xdf;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E98),0xdf);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x10;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x10;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xde;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0xde;
      path = EQ_GLOBAL(char *, 0x00577E94);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x20:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x44;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x16a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780C4),0x16a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x169;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780C0),0x169);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x168;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x168;
      path = EQ_GLOBAL(char *, 0x005780BC);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x21:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    tmp = *(int *)(self + 0x70);
case21Body:
    *(unsigned int *)(tmp + 0x1e0) = 0x21;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x21;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x105;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x105;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x105;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x105;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F30),0x105);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x104;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F2C),0x104);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x103;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F28),0x103);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x106;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x106;
    path = EQ_GLOBAL(char *, 0x00577F34);
    break;
  case 0x22:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xc3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xc3;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xc3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xc3;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E28),0xc3);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xc2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E24),0xc2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xc1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0xc1;
      path = EQ_GLOBAL(char *, 0x00577E20);
      goto loadSoundAndClear;
    }
    goto clear210;
  default:
    goto finish;
  case 0x24:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x34;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x34;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x139;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x139;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x139;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x139;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578000),0x139);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x138;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FFC),0x138);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x137;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x137;
      path = EQ_GLOBAL(char *, 0x00577FF8);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x25:
  case 0x54:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x60;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x60;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x149;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x149;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x149;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x149;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578040),0x149);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x148;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057803C),0x148);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x147;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x147;
    path = EQ_GLOBAL(char *, 0x00578038);
    break;
  case 0x26:
    tmp = *(int *)(self + 0x70);
    if (*(char *)(self + 0xd4) == '\x03') goto case58Body;
    *(unsigned int *)(tmp + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x62;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x62;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x152;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x152;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x152;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x152;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578064),0x152);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x151;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578060),0x151);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x3a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x3a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x150;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x150;
    path = EQ_GLOBAL(char *, 0x0057805C);
    break;
  case 0x27:
    variant = *(char *)(self + 0xd4);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    tmp = *(int *)(self + 0x70);
    if (variant == '\x03') goto case21Body;
    *(unsigned int *)(tmp + 0x1e0) = 0x26;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x26;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x10c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x10c;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x10c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x10c;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F4C),0x10c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x10b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F48),0x10b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x10a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F44),0x10a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1bb;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1bb;
    path = EQ_GLOBAL(char *, 0x00578208);
    break;
  case 0x28:
  case 0x89:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x27;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x27;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x10f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x10f;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x10f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x10f;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F58),0x10f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x10e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F54),0x10e);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x10d;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F50),0x10d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1c5;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1c5;
    path = EQ_GLOBAL(char *, 0x00578230);
    break;
  case 0x29:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x52;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x45;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x45;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x16d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x16d;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x16d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x16d;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780D0),0x16d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x16c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780CC),0x16c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x16b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x16b;
    path = EQ_GLOBAL(char *, 0x005780C8);
    break;
  case 0x2a:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x41;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x41;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x165;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x165;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x165;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x165;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780B0),0x165);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x164;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780AC),0x164);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    goto case78Tail;
  case 0x2b:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 7;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 7;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xc6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xc6;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xc6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xc6;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E34),0xc6);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xc5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E30),0xc5);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xc4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    goto caseBfTail;
  case 0x2e:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x6a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x1f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x1f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1a7;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1a7;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1a7;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1a7;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781B8),0x1a7);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1a6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781B4),0x1a6);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1a5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1a5;
    path = EQ_GLOBAL(char *, 0x005781B0);
    break;
  case 0x2f:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x2a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x29;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x29;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x115;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x115;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x115;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x115;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F70),0x115);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x114;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F6C),0x114);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x113;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F68),0x113);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1bc;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1bc;
    path = EQ_GLOBAL(char *, 0x0057820C);
    break;
  case 0x30:
    variant = *(char *)(self + 0xd4);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x3e;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x3e;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x15e;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x15e;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x15e;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x15e;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578094),0x15e);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x15d;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578090),0x15d);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x15c;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x15c;
        path = EQ_GLOBAL(char *, 0x0057808C);
        goto loadSoundAndClear;
      }
      goto clear210;
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x2c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x2c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x11b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x11b;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x11b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x11b;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F88),0x11b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x11a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F84),0x11a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x119;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x119;
    path = EQ_GLOBAL(char *, 0x00577F80);
    break;
  case 0x31:
  case 0x9e:
    variant = *(char *)(self + 0xd4);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    tmp = *(int *)(self + 0x70);
    if (variant != '\x01') goto caseA5Body;
case13Body:
    *(unsigned int *)(tmp + 0x1e0) = 0x15;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x15;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xe3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xe3;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xe3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xe3;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EA8),0xe3);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xe2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EA4),0xe2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xe1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EA0),0xe1);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0xe5;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0xe5;
    path = EQ_GLOBAL(char *, 0x00577EB0);
    break;
  case 0x32:
  case 0x3f:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x2d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x2d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x11e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x11e;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x11e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x11e;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F94),0x11e);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x11d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F90),0x11d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x11c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x11c;
    path = EQ_GLOBAL(char *, 0x00577F8C);
    break;
  case 0x33:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x2e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x2e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x121;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x121;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x121;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x121;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FA0),0x121);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x120;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F9C),0x120);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x11f;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F98),0x11f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1bf;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1bf;
    path = EQ_GLOBAL(char *, 0x00578218);
    break;
  case 0x34:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x170;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x170;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x170;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x170;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780DC),0x170);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x16f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780D8),0x16f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x46;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x46;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x16e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x16e;
    path = EQ_GLOBAL(char *, 0x005780D4);
    break;
  case 0x35:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x30;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x30;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x12a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x12a;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x12a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x12a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FC4),0x12a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x129;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FC0),0x129);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x128;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FBC),0x128);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1c1;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1c1;
    path = EQ_GLOBAL(char *, 0x00578220);
    break;
  case 0x36:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x31;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x31;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x12d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x12d;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x12d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x12d;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FD0),0x12d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 300;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FCC),300);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 299;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FC8),299);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1ce;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1ce;
    path = EQ_GLOBAL(char *, 0x00578254);
    break;
  case 0x38:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x32;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x133;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x133;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x133;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x133;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FE8),0x133);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x132;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FE4),0x132);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x7a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1c2;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1c2;
    path = EQ_GLOBAL(char *, 0x00578224);
    break;
  case 0x39:
    variant = *(char *)(self + 0x9c);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x7c;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x7c;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1f7;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1f7;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1f7;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1f7;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782F8),0x1f7);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1f8;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782FC),0x1f8);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x3a;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x3a;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1f6;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x1f6;
        path = EQ_GLOBAL(char *, 0x005782F4);
        goto loadSoundAndClear;
      }
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x7d;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x7d;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1fa;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1fa;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1fa;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1fa;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578304),0x1fa);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1fb;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578308),0x1fb);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x3a;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x3a;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1f9;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x1f9;
        path = EQ_GLOBAL(char *, 0x00578300);
        goto loadSoundAndClear;
      }
    }
    goto clear210;
  case 0x3c:
  case 0x9b:
  case 0xa1:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x140;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x140;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x140;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x140;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057801C),0x140);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x13f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578018),0x13f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    if (2.0 < *(float *)(*(int *)(self + 0x70) + 0xb4)) {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x24;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x24;
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x141;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578020),0x141);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057825C),0x1d0);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x142;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x142;
    path = EQ_GLOBAL(char *, 0x00578024);
    break;
  case 0x3d:
  case 0x94:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1c7;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1c7;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1c7;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1c7;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578150),0x1c7);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x18d;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578150),0x18d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x13e;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x13e;
      path = EQ_GLOBAL(char *, 0x00578014);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x40:
  case 0xa2:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x5c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x5c;
    goto caseA7Body;
  case 0x41:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x4b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x5a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x5a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x17d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x17d;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x17d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x17d;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578110),0x17d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x17c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057810C),0x17c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x17b;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578108),0x17b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1b9;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1b9;
    path = EQ_GLOBAL(char *, 0x00578200);
    break;
  case 0x44:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x59;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x59;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x155;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x155;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x155;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x155;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578070),0x155);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x154;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057806C),0x154);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x3b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x3b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x153;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x153;
      path = EQ_GLOBAL(char *, 0x00578068);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x45:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x40;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 399;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 399;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 399;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 399;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578158),399);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x18e;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578154),0x18e);
    }
    goto case86Tail;
  case 0x46:
    variant = *(char *)(self + 0x9c);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x43;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x43;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1b1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781E0),0x1b1);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1b0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x1b0;
        path = EQ_GLOBAL(char *, 0x005781DC);
case46Load:
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(path,soundId);
      }
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x42;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x42;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x94;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x167;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780B8),0x167);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x166;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x166;
        path = EQ_GLOBAL(char *, 0x005780B4);
        goto case46Load;
      }
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0x69;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x69;
    goto finish;
  case 0x48:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x6e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x6e;
    goto case87Tail;
  case 0x49:
  case 0x72:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    goto case87Tail;
  case 0x4a:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x130;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x130;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x130;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x130;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FDC),0x130);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x12f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FD8),0x12f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x12e;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x12e;
      path = EQ_GLOBAL(char *, 0x00577FD4);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x4b:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    goto case8aBody;
  case 0x4c:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    tmp = *(int *)(self + 0x70);
    if (*(char *)(self + 0xd4) != '\x03') goto case77Body;
    *(unsigned int *)(tmp + 0x1e0) = 0x8c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x8c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x222;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x222;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x222;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x222;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783A4),0x222);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x223;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783A8),0x223);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x221;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x221;
      path = EQ_GLOBAL(char *, 0x005783A0);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x4f:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0xc;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xd3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xd3;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xd3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xd3;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E68),0xd3);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xd2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E64),0xd2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xd1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0xd1;
      path = EQ_GLOBAL(char *, 0x00577E60);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x50:
  case 0xa6:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x58;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x58;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x118;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x118;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x118;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x118;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F7C),0x118);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x117;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F78),0x117);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x116;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x116;
      path = EQ_GLOBAL(char *, 0x00577F74);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x52:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x35;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x35;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x13c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x13c;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x13c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x13c;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057800C),0x13c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x13b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578008),0x13b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x13a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578004),0x13a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x13d;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x13d;
    path = EQ_GLOBAL(char *, 0x00578010);
    break;
  case 0x53:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x36;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x36;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x145;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x145;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x145;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x145;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578030),0x145);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x144;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057802C),0x144);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x143;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578028),0x143);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x146;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x146;
    path = EQ_GLOBAL(char *, 0x00578034);
    break;
  case 0x55:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x37;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x57;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x57;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x14c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x14c;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x14c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x14c;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057804C),0x14c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x14b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x14b;
      path = EQ_GLOBAL(char *, 0x00578048);
case55Load:
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(path,soundId);
    }
    goto case93Tail;
  case 0x56:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x39;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x38;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x38;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x14f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x14f;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x14f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x14f;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578058),0x14f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x14e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578054),0x14e);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x14d;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578050),0x14d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1c3;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1c3;
    path = EQ_GLOBAL(char *, 0x00578228);
    break;
  case 0x57:
  case 0x90:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xc0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xc0;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xc0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xc0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E1C),0xc0);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xbf;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E18),0xbf);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xbe;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0xbe;
      path = EQ_GLOBAL(char *, 0x00577E14);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x58:
    tmp = *(int *)(self + 0x70);
case58Body:
    *(unsigned int *)(tmp + 0x1dc) = 0xf;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x5f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x5f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xdd;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xdd;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xdd;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xdd;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E90),0xdd);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xdc;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E8C),0xdc);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x77;
    goto clear210;
  case 0x59:
  case 0x9a:
  case 0x9d:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x17;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x18;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x18;
    goto caseA3Body;
  case 0x5b:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x20;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x20;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x102;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x102;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x102;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x102;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F24),0x102);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x101;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F20),0x101);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x100;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x100;
      path = EQ_GLOBAL(char *, 0x00577F1C);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x5f:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x4d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x4d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x192;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x192;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x192;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x192;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578164),0x192);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x191;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578160),0x191);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 400;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057815C),400);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1b7;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1b7;
    path = EQ_GLOBAL(char *, 0x005781F8);
    break;
  case 0x60:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
case60Body:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0xb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0xb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1ac;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1ac;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1ac;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1ac;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781CC),0x1ac);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xce;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E54),0xce);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1ab;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781C8),0x1ab);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1ad;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1ad;
    path = EQ_GLOBAL(char *, 0x005781D0);
    break;
  case 0x62:
    variant = *(char *)(self + 0x9c);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x180;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x180;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x180;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x180;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057811C),0x180);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x17f;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578118),0x17f);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x17e;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x17e;
        path = EQ_GLOBAL(char *, 0x00578114);
        goto loadSoundAndClear;
      }
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x183;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x183;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x183;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x183;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578128),0x183);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x182;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578124),0x182);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x181;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x181;
        path = EQ_GLOBAL(char *, 0x00578120);
        goto loadSoundAndClear;
      }
    }
    goto clear210;
  case 99:
  case 0x9f:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x56;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x56;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x195;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x195;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x195;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x195;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578170),0x195);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x194;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057816C),0x194);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x193;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578168),0x193);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x196;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x196;
    path = EQ_GLOBAL(char *, 0x00578174);
    break;
  case 100:
    variant = *(char *)(self + 0xd4);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x4f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    if (variant != '\x02') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x199;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x199;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x199;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x199;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578180),0x199);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x198;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057817C),0x198);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x197;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x197;
        path = EQ_GLOBAL(char *, 0x00578178);
        goto loadSoundAndClear;
      }
    }
    goto clear210;
  case 0x65:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (*(char *)(self + 0xd4) == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x85;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x85;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x212;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x212;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x212;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x212;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578364),0x212);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x213;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578368),0x213);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x211;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x211;
        path = EQ_GLOBAL(char *, 0x00578360);
        goto loadSoundAndClear;
      }
      goto clear210;
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x55;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x55;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x19c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x19c;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x19c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x19c;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057818C),0x19c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x19b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578188),0x19b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x19a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578184),0x19a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x19d;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x19d;
    path = EQ_GLOBAL(char *, 0x00578190);
    break;
  case 0x66:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x1a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x1a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1cb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1cb;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1cb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1cb;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578248),0x1cb);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1ca;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578244),0x1ca);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x1c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x1b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1c9;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1c9;
      path = EQ_GLOBAL(char *, 0x00578240);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x67:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x186;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x186;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x186;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x186;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578134),0x186);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x185;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578130),0x185);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x184;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057812C),0x184);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1be;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1be;
    path = EQ_GLOBAL(char *, 0x00578214);
    break;
  case 0x68:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x1e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x1e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xfb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xfb;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xfb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xfb;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F08),0xfb);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xfa;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F04),0xfa);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
case68Tail:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xf9;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577F00),0xf9);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0xfc;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0xfc;
    path = EQ_GLOBAL(char *, 0x00577F0C);
    break;
  case 0x6b:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x2f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x2f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x124;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x124;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x124;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x124;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FAC),0x124);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x123;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FA8),0x123);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x24;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x122;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x122;
      path = EQ_GLOBAL(char *, 0x00577FA4);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x6d:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x3d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x15b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x15b;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x15b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x15b;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578088),0x15b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x15a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578084),0x15a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x159;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x159;
      path = EQ_GLOBAL(char *, 0x00578080);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x6e:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x5d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x5d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x127;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x127;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x127;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x127;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FB8),0x127);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x126;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FB4),0x126);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x125;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FB0),0x125);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1c0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1c0;
    path = EQ_GLOBAL(char *, 0x0057821C);
    break;
  case 0x6f:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x53;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x18c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x18c;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x18c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x18c;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057814C),0x18c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x18b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578148),0x18b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x18a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578144),0x18a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1bd;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1bd;
    path = EQ_GLOBAL(char *, 0x00578210);
    break;
  case 0x71:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x54;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x4c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x4c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x189;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x189;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x189;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x189;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578140),0x189);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x188;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057813C),0x188);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x187;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x187;
      path = EQ_GLOBAL(char *, 0x00578138);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x74:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x176;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x176;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x176;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x176;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780F4),0x176);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x175;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780F0),0x175);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x174;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x174;
    path = EQ_GLOBAL(char *, 0x005780EC);
    break;
  case 0x75:
    soundId = 99;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x44;
    tmp = *(int *)(self + 0x70);
    goto case76Body;
  case 0x76:
    soundId = 100;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x44;
    tmp = *(int *)(self + 0x70);
case76Body:
    *(unsigned int *)(tmp + 0x1e0) = soundId;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = soundId;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x16a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780C4),0x16a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x169;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780C0),0x169);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x168;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780BC),0x168);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1cf;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1cf;
    path = EQ_GLOBAL(char *, 0x00578258);
    break;
  case 0x77:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    tmp = *(int *)(self + 0x70);
case77Body:
    *(unsigned int *)(tmp + 0x1e0) = 0x33;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x33;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x136;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x136;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x136;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x136;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FF4),0x136);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x135;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577FF0),0x135);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x134;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x134;
      path = EQ_GLOBAL(char *, 0x00577FEC);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x78:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x41;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x41;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x165;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x165;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x165;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x165;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780B0),0x165);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x164;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780AC),0x164);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    if (*(short *)(self + 0x9a) == 0x78) {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    }
case78Tail:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x163;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x163;
    path = EQ_GLOBAL(char *, 0x005780A8);
    break;
  case 0x79:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x53;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x51;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x51;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1b4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1b4;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1b4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1b4;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781EC),0x1b4);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1b3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781E8),0x1b3);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1b2;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781E4),0x1b2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1b5;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1b5;
    path = EQ_GLOBAL(char *, 0x005781F0);
    break;
  case 0x7a:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x50;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x50;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1aa;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1aa;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1aa;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1aa;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781C4),0x1aa);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1a9;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781C0),0x1a9);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1a8;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1a8;
      path = EQ_GLOBAL(char *, 0x005781BC);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x7b:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1a0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1a0;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1a0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1a0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057819C),0x1a0);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x19f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578198),0x19f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x19e;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578194),0x19e);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1a1;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1a1;
    path = EQ_GLOBAL(char *, 0x005781A0);
    break;
  case 0x7c:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x5e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x5e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x17a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x17a;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x17a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x17a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578104),0x17a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x178;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if ((EQ_GLOBAL(int, 0x006E4DD8) != 0) && (EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780FC),0x178), EQ_GLOBAL(int, 0x006E4DD8) != 0))
    {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578100),0x179);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x49;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x4a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x177;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x177;
      path = EQ_GLOBAL(char *, 0x005780F8);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x7d:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x48;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x47;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x47;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x173;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x173;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x173;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x173;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780E8),0x173);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x172;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780E4),0x172);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x171;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x171;
      path = EQ_GLOBAL(char *, 0x005780E0);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0x7e:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x4e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x4e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1a4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1a4;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1a4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1a4;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781AC),0x1a4);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1a3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781A8),0x1a3);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1a2;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005781A4),0x1a2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1b6;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1b6;
    path = EQ_GLOBAL(char *, 0x005781F4);
    break;
  case 0x83:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x50;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x50;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 1;
    goto case87Tail;
  case 0x86:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x40;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 399;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 399;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 399;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 399;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578158),399);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0;
case86Tail:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x162;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x162;
      path = EQ_GLOBAL(char *, 0x005780A4);
loadSoundAndClear:
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(path,soundId);
    }
    goto clear210;
  case 0x87:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x24;
case87Tail:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0;
    goto clear210;
  case 0x8a:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x52;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x45;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x45;
case8aBody:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xeb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xeb;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xeb;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xeb;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EC8),0xeb);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xea;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EC4),0xea);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xe9;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto clear210;
    soundId = 0xe9;
    path = EQ_GLOBAL(char *, 0x00577EC0);
    goto loadSoundAndClear;
  case 0x92:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x21;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x21;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x16a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780C4),0x16a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x183;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x183;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578128),0x183);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x169;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780C0),0x169);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    goto case93Tail;
  case 0x93:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x44;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 99;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 99;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x16a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x16a;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005780C4),0x16a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x183;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x183;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578128),0x183);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x169;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x169;
      path = EQ_GLOBAL(char *, 0x005780C0);
      goto case55Load;
    }
case93Tail:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x14a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x14a;
    path = EQ_GLOBAL(char *, 0x00578044);
    break;
  case 0x95:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xc9;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xc9;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E40),0xc9);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x180;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x180;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x180;
      path = EQ_GLOBAL(char *, 0x0057811C);
      goto case16Load;
    }
case16Tail:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 200;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E3C),200);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 8;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 8;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 199;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 199;
    path = EQ_GLOBAL(char *, 0x00577E38);
    break;
  case 0x9c:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x34;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x8e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x228;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x228;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x228;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x228;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783BC),0x228);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x229;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783C0),0x229);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x227;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x227;
      path = EQ_GLOBAL(char *, 0x005783B8);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xa3:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
caseA3Body:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xe8;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xe8;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xe8;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xe8;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EBC),0xe8);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xe7;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EB8),0xe7);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xe6;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EB4),0xe6);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1b8;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1b8;
    path = EQ_GLOBAL(char *, 0x005781FC);
    break;
  case 0xa5:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    tmp = *(int *)(self + 0x70);
caseA5Body:
    *(unsigned int *)(tmp + 0x1e0) = 0x14;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x14;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xe3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xe3;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0xe3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0xe3;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EA8),0xe3);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0xe2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EA4),0xe2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xe1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577EA0),0xe1);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0xe4;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0xe4;
    path = EQ_GLOBAL(char *, 0x00577EAC);
    break;
  case 0xa7:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0;
caseA7Body:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x158;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x158;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x158;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x158;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057807C),0x158);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x157;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578078),0x157);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x3c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x3c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x156;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578074),0x156);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0x1c4;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x1c4;
    path = EQ_GLOBAL(char *, 0x0057822C);
    break;
  case 0xa8:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 4;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x71;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x71;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1dc;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1dc;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1dc;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1dc;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057828C),0x1dc);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1dd;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578290),0x1dd);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1da;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578284),0x1da);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0x1db;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578288),0x1db);
    }
    goto clear214;
  case 0xaa:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x4f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x7a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x7a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1f1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1f1;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1f1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1f1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782E0),0x1f1);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1f2;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782E4),0x1f2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1f0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1f0;
      path = EQ_GLOBAL(char *, 0x005782DC);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xab:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x93;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x93;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x237;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x237;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x237;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x237;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783F8),0x237);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x238;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783FC),0x238);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x236;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x236;
    path = EQ_GLOBAL(char *, 0x005783F4);
    break;
  case 0xac:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x86;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x87;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x215;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x215;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x215;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x215;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578370),0x215);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x216;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578374),0x216);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x214;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x214;
    path = EQ_GLOBAL(char *, 0x0057836C);
    break;
  case 0xad:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x90;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x90;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x22e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x22e;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x22e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x22e;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783D4),0x22e);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x22f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783D8),0x22f);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x22d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x22d;
    path = EQ_GLOBAL(char *, 0x005783D0);
    break;
  case 0xae:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0x37;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x8f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x8f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x22b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x22b;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x22b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x22b;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783C8),0x22b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x22c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783CC),0x22c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x22a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x22a;
    path = EQ_GLOBAL(char *, 0x005783C4);
    break;
  case 0xaf:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x7e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x7e;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1fd;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1fd;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1fd;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1fd;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578310),0x1fd);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1fe;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578314),0x1fe);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1fc;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1fc;
      path = EQ_GLOBAL(char *, 0x0057830C);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xb0:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x8d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x8d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x225;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x225;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x225;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x225;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783B0),0x225);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x226;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783B4),0x226);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x224;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x224;
    path = EQ_GLOBAL(char *, 0x005783AC);
    break;
  case 0xb1:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
caseB1Body:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x73;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x73;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1e2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1e2;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1e2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1e2;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782A4),0x1e2);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1e3;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782A8),0x1e3);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1e1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1e1;
      path = EQ_GLOBAL(char *, 0x005782A0);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xb2:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x70;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x70;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1d8;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1d8;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1d8;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1d8;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057827C),0x1d8);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1d9;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578280),0x1d9);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1d7;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1d7;
      path = EQ_GLOBAL(char *, 0x00578278);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xb5:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x6f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x6f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1d5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1d5;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1d5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1d5;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578270),0x1d5);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1d6;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578274),0x1d6);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1d4;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1d4;
      path = EQ_GLOBAL(char *, 0x0057826C);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xb6:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x7f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x7f;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x200;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x200;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x200;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x200;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057831C),0x200);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x201;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578320),0x201);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1ff;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x1ff;
      path = EQ_GLOBAL(char *, 0x00578318);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xb7:
    variant = *(char *)(self + 0x9c);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (variant == '\x01') {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x78;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x79;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1ee;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1ee;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1ee;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1ee;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782D4),0x1ee);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1ef;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782D8),0x1ef);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1ed;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x1ed;
        path = EQ_GLOBAL(char *, 0x005782D0);
        goto loadSoundAndClear;
      }
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x77;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x77;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x1eb;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x1eb;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1eb;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1eb;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782C8),0x1eb);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1ec;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782CC),0x1ec);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1ea;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        soundId = 0x1ea;
        path = EQ_GLOBAL(char *, 0x005782C4);
        goto loadSoundAndClear;
      }
    }
    goto clear210;
  case 0xb9:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x82;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x82;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x209;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x209;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x209;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x209;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578340),0x209);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x20a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578344),0x20a);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x208;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x208;
      path = EQ_GLOBAL(char *, 0x0057833C);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xbc:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x80;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x80;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x203;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x203;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x203;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x203;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578328),0x203);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x204;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057832C),0x204);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x24;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x202;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x202;
      path = EQ_GLOBAL(char *, 0x00578324);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xbd:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x81;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x81;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x206;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x206;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x206;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x206;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578334),0x206);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x207;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578338),0x207);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x24;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x205;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 0x205;
      path = EQ_GLOBAL(char *, 0x00578330);
      goto loadSoundAndClear;
    }
    goto clear210;
  case 0xbe:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x89;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x89;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x21b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x21b;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578388),0x21b);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x21c;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x21c;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x0057838C),0x21c);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x21d;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00578390),0x21d);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x21a;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0x21a;
    path = EQ_GLOBAL(char *, 0x00578384);
    break;
  case 0xbf:
    variant = *(char *)(self + 0xd4);
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (variant != '\x03') goto caseB1Body;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x74;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x75;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0xc6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0xc6;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x00577E34),0xc6);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x1e5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x1e5;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782B0),0x1e5);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1e6;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782B4),0x1e6);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x1e4;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782AC),0x1e4);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0xc4;
caseBfTail:
    if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
    soundId = 0xc4;
    path = EQ_GLOBAL(char *, 0x00577E2C);
    break;
  case 0xc0:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x7b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x7b;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 500;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 500;
    *(unsigned int *)(*(int *)(self + 0x70) + 500) = 500;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 500;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782EC),500);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x1f5;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005782F0),0x1f5);
    }
    *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 0x25;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
    *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0xe1;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
      soundId = 499;
      path = EQ_GLOBAL(char *, 0x005782E8);
      goto loadSoundAndClear;
    }
clear210:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
clear214:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
    goto finish;
  case 0xc2:
    *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc) = 0;
    if (*(float *)(self + 0xd8) <= 10.0) {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x92;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x92;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x234;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x234;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x234;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x234;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783EC),0x234);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x235;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783F0),0x235);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x233;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
      soundId = 0x233;
      path = EQ_GLOBAL(char *, 0x005783E8);
    }
    else {
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e0) = 0x91;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e4) = 0x91;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1e8) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1ec) = 0x231;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f0) = 0x231;
      *(unsigned int *)(*(int *)(self + 0x70) + 500) = 0x231;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1f8) = 0x231;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783E0),0x231);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x1fc) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x200) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x208) = 0x232;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x204) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
        EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(EQ_GLOBAL(char *, 0x005783E4),0x232);
      }
      *(unsigned int *)(*(int *)(self + 0x70) + 0x218) = 1;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x21c) = 2;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x220) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x20c) = 0x230;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x210) = 0;
      *(unsigned int *)(*(int *)(self + 0x70) + 0x214) = 0;
      if (EQ_GLOBAL(int, 0x006E4DD8) == 0) goto finish;
      soundId = 0x230;
      path = EQ_GLOBAL(char *, 0x005783DC);
    }
  }
  EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)(path,soundId);
finish:
  *(unsigned int *)(*(int *)(self + 0x70) + 0x228) =
       *(unsigned int *)(*(int *)(self + 0x70) + 0x1dc);
  *(unsigned int *)(*(int *)(self + 0x70) + 0x22c) = 0;
  if (EQ_GLOBAL(int, 0x006E4DD8) != 0) {
    tmp = *(int *)(*(int *)(self + 0x70) + 0x228);
    if (0 < tmp) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)((&EQ_GLOBAL(char *, 0x00578400))[tmp],EQ_GLOBAL(int, 0x00568304) + tmp);
    }
    tmp = *(int *)(*(int *)(self + 0x70) + 0x1e0);
    if (0 < tmp) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)((&EQ_GLOBAL(char *, 0x00578400))[tmp],EQ_GLOBAL(int, 0x00568304) + tmp);
    }
    tmp = *(int *)(*(int *)(self + 0x70) + 0x1e4);
    if (0 < tmp) {
      EQ_METHOD(2, void, 0x004E37F4, (void *, char *, int), (void *)g_sound_manager_ptr_guess)((&EQ_GLOBAL(char *, 0x00578400))[tmp],EQ_GLOBAL(int, 0x00568304) + tmp);
    }
  }
  return;
}

// FUNCTION: TRILOGY 0x004BC9BA
// ORIGINAL FILE: EverQuest/EQPlayer.cpp (from Mac debug map)
// @early-stop: 5.0f body is laid out after 7.0f (original: before 4.7f); 0x72 last-test branch sense; height-factor x87 operand order; xor edx before InitInnates.
// The member has no declaration in EQPlayer's header, so retain its ECX ABI as a file-local fastcall entry.
void __fastcall SetHeights(EQPlayer* self, int edx)
{
    short spawnRace;
    int heightRace;
    float factor;
    int sizeRace;
    float* scalePtr;
    int tmp;

    tmp = (int)self->actor_info;
    if (tmp != 0)
    {
        spawnRace = (short)self->race;
        if (spawnRace == 0x8d)
        {
            self->levitate = 2;
            ((EQActorInfo_guess*)tmp)->unknown0x088 = 0;
            *(u16*)((u32)self->actor_info + 0x78) = 1;
            self->actor_info->unk_0x7C = 4;
            self->actor_info->unk_0x84 = 0x3ba3d70a;
        }
        else if (spawnRace == 0x48 || spawnRace == 0x49 || spawnRace == 0x72)
        {
            ((EQActorInfo_guess*)tmp)->unknown0x088 = 0;
        }
    }
    if (self->size <= 0.0f)
    {
        sizeRace = (int)self->race;
        switch (sizeRace)
        {
        case 7:
            self->size = 5.5f;
            break;
        case 0xc:
        case 0x58:
            self->size = 3.0f;
            break;
        case 0xb:
        case 0x51:
            self->size = 3.5f;
            break;
        case 4:
        case 6:
        case 0x4d:
        case 0x70:
            self->size = 5.0f;
            break;
        case 0x2b:
            self->size = 4.7f;
            break;
        case 9:
        case 0x5c:
            self->size = 8.0f;
            break;
        case 1:
            self->size = 6.0f;
            break;
        case 3:
            self->size = 6.0f;
            break;
        case 5:
            self->size = 6.0f;
            break;
        case 0x18:
            self->size = 6.0f;
            break;
        case 0x4e:
            self->size = 6.0f;
            break;
        case 0x80:
            self->size = 6.0f;
            break;
        default:
            self->size = 6.0f;
            break;
        case 2:
        case 0x5a:
        case 0x82:
            self->size = 7.0f;
            break;
        case 8:
        case 0x5e:
            self->size = 4.0f;
            break;
        case 0xa:
        case 0x5d:
            self->size = 9.0f;
            break;
        }
    }
    self->height_offset = 3.125f;
    *(float*)&self->unk_0x78 = 6.2f;
    if (self->actor_info != 0)
    {
        self->actor_info->actorScale_guess = self->size * 0.2f;
        scalePtr = (float*)((u32)self->actor_info + 0xb4);
        self->height_offset = *scalePtr * self->height_offset;
        *(float*)&self->unk_0x78 = *(float*)&self->unk_0x78 * *scalePtr;
    }
    else
    {
        EQTRACE_guess((const char*)0x00568AAC);
    }
    heightRace = self->race;
    switch (heightRace)
    {
    case 0x2a:
    case 0x78:
    case 0xab:
        *(float*)&self->unk_0x78 = 3.9f;
        if (self->type == 0)
            self->height_offset = 2.5f;
        break;
    case 0x31:
    case 0x9e:
    case 0xc4:
        *(float*)&self->unk_0x78 = 34.5f;
        self->height_offset = 20.0f;
        if (self->actor_info != 0)
            self->actor_info->actorScale_guess = 1.0f;
        else
            EQTRACE_guess((const char*)0x00568AAC);
        break;
    case 0x48:
        self->height_offset = 0.1f;
        *(float*)&self->unk_0x78 = 1.0f;
        if (self->actor_info != 0)
            self->actor_info->actorScale_guess = 2.0f;
        else
            EQTRACE_guess((const char*)0x00568AAC);
        break;
    case 0x49:
    case 0x72:
    case 0x8d:
    case 0xfc:
        self->height_offset = 0.1f;
        *(float*)&self->unk_0x78 = 1.0f;
        if (self->actor_info != 0)
            self->actor_info->actorScale_guess = 1.0f;
        else
            EQTRACE_guess((const char*)0x00568AAC);
        break;
    case 0x6c:
        *(float*)&self->unk_0x78 = 3.0f;
        self->height_offset = 3.0f;
        if (self->actor_info != 0)
            self->actor_info->actorScale_guess = 0.05f;
        else
            EQTRACE_guess((const char*)0x00568AAC);
        break;
    case 0x8e:
    case 0x8f:
        *(float*)&self->unk_0x78 = 6.0f;
        self->height_offset = 0.0f;
        if (self->actor_info != 0)
            self->actor_info->actorScale_guess = 1.0f;
        else
            EQTRACE_guess((const char*)0x00568AAC);
        break;
    }
    if (self->actor_info != 0)
    {
        self->actor_info->unk_0xAC_bits_guess = 0x3faccccd;
        tmp = (int)self->actor_info;
        *(float*)((u32)tmp + 0xb0) = *(float*)((u32)tmp + 0xac) * 2.5f;
    }
    if (self->actor_info != 0)
    {
        if (self->height_offset >= 4.0f)
            factor = 0.3f;
        else
            factor = 0.6f;
        self->actor_info->unk_0x1A0 = self->height_offset * factor;
        if (self->actor_info->unk_0x1A0 < 0.3f)
            self->actor_info->unk_0x1A0 = 0.3f;
        if (self->race == 0x48 && EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != 0 &&
            (tmp = _strcmpi(
                (const char*)((u32)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) + 0x974), (const char*)0x005417E0), tmp == 0 ||
            (tmp = _strcmpi(
                (const char*)((u32)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) + 0x974), (const char*)0x0053DF50), tmp == 0 ||
            (tmp = _strcmpi(
                (const char*)((u32)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) + 0x974), (const char*)0x005417D4), tmp == 0 ||
            (tmp = _strcmpi(
                (const char*)((u32)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) + 0x974), (const char*)0x00561B4C), tmp == 0)))))
        {
            *(u32*)((u32)self->actor_info + 0x244) = 1;
        }
        EQ_FUNC(void, __fastcall, 0x004BC963, (void*))(self);
    }
    if (self->char_info_guess != 0)
    {
        EQ_METHOD(2, void, 0x0041D17E, (void*, u16, s16), self->char_info_guess)(self->race, self->class_);
    }
    if (self->height_offset == 0.0f)
        self->height_offset = 0.1f;
    return;
}

// FUNCTION: TRILOGY 0x004bc963
// ORIGINAL FILE: Source/PC Source/EverQuest/EQPlayer.cpp (Mac: EQPlayer::InitSneakMod, medium)
// __thiscall. The sneak modifier: 0.5, plus 0.1 per level above 50 for rogues (EBP frame in the original).
void EQPlayer::InitSneakMod_guess(void)
{
    if (actor_info == 0)
    {
        EQTRACE_guess("ERROR: LocalInfo is NULL in InitSneakMod, can't properly set sneakmod.\n");
        return;
    }
    actor_info->sneak_mod_guess = 0.5f;
    if (class_ == 9 && level > 50)
        actor_info->sneak_mod_guess += (level - 50) * 0.1f;
}

// FUNCTION: TRILOGY 0x004bfb7c
// Checks whether the zone belongs to a flagged set whose gameplay purpose is unknown.
// __thiscall ABI (fastcall stand-in): ECX is never read, the zone name is the only stack argument (RET 4).
// zoneId = ZoneIdFromShortName (0x004C05AE). Returns 1 for the flagged zone ids, else 0; the meaning of the set is unknown.
// The flagged ids are one switch; VC6's compare tree over its case labels matches the original.
int __fastcall IsZoneNameFlagged_guess(void* thisPtr, int edx, const char* zoneName)
{
    int zoneId;

    zoneId = EQ_STDCALL(int, 0x004C05AE, (const char*))(zoneName);
    switch (zoneId)
    {
    case 1: case 2: case 3: case 4: case 6: case 8: case 9: case 10:
    case 19: case 22: case 23: case 24: case 25: case 29: case 30: case 33:
    case 38: case 40: case 41: case 42: case 46: case 47: case 48: case 52:
    case 54: case 55: case 56: case 60: case 61: case 62: case 67: case 68:
    case 75: case 78: case 79: case 82: case 83: case 85:
    case 106: case 113: case 115: case 129: case 150: case 151:
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004E37F4
// Loads a sound file into the selected sound slot for playback.
// __thiscall ABI: ECX is forwarded to LoadNamedWave_guess; both explicit arguments are on the stack.
// edx is the caller's EDX, passed on untouched (the original never writes it).
u8 __fastcall LoadSound_guess(void* thisPtr, int edx, u32 path, int soundId)
{
    int loaded;

    if ((soundId < 0) || (soundId >= 0x2d0))
    {
        EQTRACE_guess((const char*)0x0057BEBC, soundId);
        LogMessage_guess((const char*)0x0057BEBC, soundId);
        loaded = 0;
    }
    else
    {
        loaded = EQ_THISCALL(u8, 0x004E3768, (void*, int, u32, int))(thisPtr, edx, path, soundId);
    }
    return loaded;
}
