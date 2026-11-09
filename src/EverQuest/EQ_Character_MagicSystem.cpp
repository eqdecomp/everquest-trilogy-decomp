// EQ_Character_MagicSystem.cpp - the spell side of EQ_Character: buffs, spell effects, durations, casting.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/player.h"
#include "trilogy/character.h"
#include "trilogy/spell.h"
#include "trilogy/eq_character.h"
#include "trilogy/globals.h"
#include "trilogy/eqgfx.h"
#include "trilogy/eqswitch.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/display.h"

// FUNCTION: TRILOGY 0x00415EFB
// 3D-view click: left picks a target or reads a message board; right opens a banker, merchant or guildmaster.
// @early-stop: 0.7777; saved-register allocation, call setup, and block layout still differ.
/* fake match: keep the original overlapping packet scratch offsets in VC6's stack frame. */
int __fastcall MouseUp(void* this_, int edx, int button, int screenX, int screenY)
{
    u8 classId;
    EQPlayer* target;
    short effectSlot;
    int i;
    int j;
    int value;
    void* object;
    u32* vtable;
    u32 opcode;
    void* data;
    u32 size;
    struct LocalBuffers
    {
        char message[128];
        struct LocalPacketBuffer
        {
            u32 words[27];
            float tail;
            u32 spare[34];
        } packet;
    } buffers;
    u32 msgWord0;
    int msgWord1;
    u32 msgWord2;
    u32 merchantSpawnId;
    u32 selfSpawnId;
    struct LocalOutgoingPackets
    {
        struct LocalSpawnPacket
        {
            u32 spawn_id;
            char code;
        } spawn;
        struct LocalVisibilityPacket
        {
            u32 spawn_id;
            u16 count;
            u32 invis;
        } visibility;
    } outgoing_packets;
    u32 clickedSpawnId;
    int clickedObject;
    int pickedEntry;
    float distance;
    EQPlayer* local_10;
    EQPlayer* rightPick;

    if (EQ_METHOD(0, int, 0x004504AC, (void*), this_)() != 0)
    {
        pickedEntry = 0;
        clickedObject = 0;
        rightPick = 0;
        local_10 = 0;
        if (((short)screenX <= (short)EQ_GLOBAL(u16, 0x006B63E4)) &&
            ((short)screenY <= (short)EQ_GLOBAL(u16, 0x006B63E6)) &&
            ((short)screenX >= (short)EQ_GLOBAL(u16, 0x006B63E0)) &&
            ((short)screenY >= (short)EQ_GLOBAL(u16, 0x006B63E2)))
        {
    if ((short)button == 0)
    {
        local_10 = (EQPlayer*)EQ_METHOD(2, void*, 0x004DD010, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(screenX, screenY);
    }
    if ((short)button == 1)
    {
        rightPick = (EQPlayer*)EQ_METHOD(2, void*, 0x004DD010, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(screenX, screenY);
    }

    if (local_10 != 0)
    {
        if (((local_10->actor_info == 0) ||
            (((u32)(*(int*)((char*)g_display_guess + 0xBC) -
                *(int*)((char*)local_10->actor_info + 0x19C)) >= 12000 &&
                (((local_10->type < 1 && (local_10->cur_hp >= 1)) || (local_10->speed != 0.0f)))))) &&
            (local_10 != EQ_GLOBAL(EQPlayer*, 0x006E4DA0)))
        {
            local_10 = 0;
        }
        if (((local_10 != 0) && (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) != local_10)) &&
            (EQ_METHOD(2, int, 0x00410C80, (void*, EQPlayer*, EQPlayer*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4DA0), local_10) != 0))
        {
            local_10 = 0;
        }
    }
    if (rightPick != 0)
    {
        if (((rightPick->actor_info == 0) ||
            (((u32)(*(int*)((char*)g_display_guess + 0xBC) -
                *(int*)((char*)rightPick->actor_info + 0x19C)) >= 12000 &&
                (((rightPick->type < 1 && (rightPick->cur_hp >= 1)) || (rightPick->speed != 0.0f)))))) &&
            (rightPick != EQ_GLOBAL(EQPlayer*, 0x006E4DA0)))
        {
            rightPick = 0;
        }
        if (((rightPick != 0) && (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) != rightPick)) &&
            (EQ_METHOD(2, int, 0x00410C80, (void*, EQPlayer*, EQPlayer*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4DA0), rightPick) != 0))
        {
            rightPick = 0;
        }
    }

    target = local_10;
    if (target != 0)
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = target;
        EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
        pinstEQ_Main_guess->unk_0x8FBB6 = 0xFFFF;
        if (pinstEQ_Main_guess->inspectActive_guess != '\0')
        {
            object = EQ_GLOBAL(void*, 0x006E4E44);
            vtable = *(u32**)object;
            ((void (__fastcall *)(void*))vtable[2])(object);
            pinstEQ_Main_guess->screenMode_u8_guess =
                pinstEQ_Main_guess->prevScreenMode_u8_guess;
            object = EQ_GLOBAL(void*, 0x006E4E44);
            vtable = *(u32**)object;
            ((void (__fastcall *)(void*))vtable[1])(object);
        }
    }

    if (((rightPick != 0) && (rightPick->race == 0x8D)) && (rightPick->type == 1))
    {
        clickedSpawnId = rightPick->spawn_id;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) == rightPick)
        {
            clickedObject = clickedObject & 0xFFFFFF00;
            send_message_guess
                (g_world_connection_guess, 0x2126, &clickedSpawnId, 8, 1);
            EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
            rightPick->speed = 0.0f;
            rightPick->delta_heading = 0.0f;
            return 1;
        }
        target = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->ride_target_guess;
        if (target == 0)
            return 1;
        if (target != rightPick)
            return 1;
        data = &clickedSpawnId;
        size = 8;
        clickedObject = (clickedObject & 0xFFFFFF00) | 1;
        opcode = 0x2126;
        /* fake match: reuse the original common packet-send tail. */
        goto sendPacket;
    }

    if (((target != 0) && (EQ_GLOBAL(u8, 0x006B63F8) == 0)) &&
        ((DAT_006b6494 == 0) &&
        ((target->type == 0) && (EQ_GLOBAL(u8, 0x005FE968) != 0))))
    {
        short slot;
        int* list;
        char* enabled;
        slot = 0;
        list = EQ_ARRAY(int, 0x005FE954);
        enabled = EQ_ARRAY(char, 0x005FE8BD);
        do
        {
            if (((*enabled != '\0') && (*list != 0)) &&
                ((target = local_10), local_10 == (EQPlayer*)*list))
            {
                pinstEQ_Main_guess->groupSelected_guess = (char)slot;
                EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
            }
            slot = slot + 1;
            enabled = enabled + 0x1E;
            list = list + 1;
        } while (slot < 5);
    }

    if (((short)button == 0) && (target == 0))
    {
        pickedEntry = EQ_METHOD(2, int, 0x004DCEE4, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(screenX, screenY);
        if (pickedEntry == 0)
        {
            clickedObject = EQ_METHOD(2, int, 0x004DCFE8, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(screenX, screenY);
            if (clickedObject != 0)
                EQ_GLOBAL(int, 0x006E4DB0) = clickedObject;
        }
        else
        {
            if ((*(char*)(pickedEntry + 0x1C) == '7') &&
                (pinstEQ_Main_guess->boardRequestPending_guess == '\0'))
            {
                distance = EQ_METHOD(7, float, 0x004129F3, (void*, float, float, float, float, float, float, float), g_display_guess)(
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y,
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x,
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z,
                        *(float*)(pickedEntry + 0x34), *(float*)(pickedEntry + 0x38),
                        *(float*)(pickedEntry + 0x3C), 1.0f);
                if (distance < 20.0f)
                {
                    while ((value = EQ_GLOBAL(int, 0x006E4D70)) != 0)
                    {
                        EQ_METHOD(0, void, 0x004906B1, (void*), (void*)value)();
                        EQ_CDECL(void, 0x005218BB, (void*))((void*)value);
                    }
                    j = 0x138;
                    i = 0x14;
                    do
                    {
                        *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + j) = 0;
                        j = j + 4;
                        i = i - 1;
                    } while (i != 0);
                    *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xF2) = 0;
                    *(u8*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xDAE) = 0;
                    *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xF4) = 0;
                    *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFC) = 0;
                    *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xDAA) = 0x0B;
                    *(u8*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x100) = 1;
                    outgoing_packets.spawn.code = *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xDAA) - 9;
                    outgoing_packets.spawn.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                    send_message_guess
                        (g_world_connection_guess, 0x2108,
                            &outgoing_packets.spawn.spawn_id, 8, 1);
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0053EA08, 0x0F);
                    target = local_10;
                }
                else
                {
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0053E9D0, 0x0D);
                }
            }
            EQ_GLOBAL(int, 0x006E4DB4) = pickedEntry;
        }
    }

    if (rightPick == 0)
    {
        if (target != 0)
            return 1;
        if (pickedEntry != 0)
            return 1;
        if (clickedObject != 0)
            return 1;
        return 0;
    }
    if ((short)button != 1)
        return 1;

    if (rightPick->type < 2)
    {
        classId = rightPick->class_;
        if (classId == 0x10)
        {
            distance = EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(rightPick,
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 1.0f);
            if (distance <= 20.0f)
            {
                if (EQ_GLOBAL(int*, 0x006E4E44) == 0)
                    return 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x68 != 0)
                    return 1;
                if (EQ_GLOBAL(int, 0x006B6590) != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x25] != 0)
                    return 1;
                if (*(int*)((char*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info + 0x18C) != 0)
                    return 1;
                if (EQ_GLOBAL(void*, 0x006E4DBC) != 0)
                    return 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis != '\0')
                {
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis = '\0';
                    button = -1;
                    object = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x0C,
                            (short*)&button);
                    EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(object);
                    EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)(short)button);
                    outgoing_packets.visibility.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                    outgoing_packets.visibility.count = 3;
                    outgoing_packets.visibility.invis = (u32)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis;
                    send_message_guess
                        (g_world_connection_guess, 0x20F5,
                            &outgoing_packets.visibility.spawn_id, 0x0C, 1);
                    EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x397] = EQ_GLOBAL(int*, 0x006E4E44)[0x397] + 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 1)
                {
                    msgWord0 = 0xFFFFFFFC;
                    msgWord1 = EQ_GLOBAL(int*, 0x006E4E44)[0x397];
                    msgWord2 = 0x0C;
                    send_message_guess
                        (g_world_connection_guess, 0x2147, &msgWord0, 0x5C, 1);
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x397] = 1;
                if ((g_game_state_guess == 2) &&
                    ((g_pcloudFlagA_guess == 0) || (EQ_GLOBAL(int, 0x006B63FC) == 0x280)))
                {
                    EQ_GLOBAL(int, 0x00590170) = 2;
                    g_game_state_guess = 1;
                    if (EQ_GLOBAL(int*, 0x006E4E44) != 0)
                        EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
                    EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
                }
                object = EQ_GLOBAL(void*, 0x006E4E44);
                vtable = *(u32**)object;
                ((void (__fastcall *)(void*))vtable[2])(object);
                pinstEQ_Main_guess->prevScreenMode_guess =
                    (char)EQ_GLOBAL(int*, 0x006E4E44)[0x3F8];
                pinstEQ_Main_guess->screenMode_u8_guess = 4;
                EQ_GLOBAL(int*, 0x006E4E44)[0x1A17A] = 1;
                object = EQ_GLOBAL(void*, 0x006E4E44);
                vtable = *(u32**)object;
                ((void (__fastcall *)(void*))vtable[1])(object);
                EQ_GLOBAL(EQPlayer*, 0x006E4DBC) = rightPick;
                object = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(rightPick->name);
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (buffers.message, (const char*)0x0053E9AC, object);
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buffers.message);
                if (EQ_GLOBAL(u8, 0x006B710C) != '\0')
                {
                    EQ_GLOBAL(u8, 0x006B710C) = '\0';
                    EQ_GLOBAL(u8, 0x006B710D) = 0;
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0053E998, 0x0D);
                    EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
                }
                if (EQ_GLOBAL(int, 0x006EFE1C) != 0)
                    return 1;
                if (EQ_GLOBAL(int, 0x006ECDF4) == 0)
                    return 1;
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x13);
                return 1;
            }
        }
        else if ((classId < 0x11) || (0x1F < classId))
        {
            if (classId == 0x20)
            {
            distance = EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(rightPick,
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 1.0f);
            if (distance <= 20.0f)
            {
                if (EQ_GLOBAL(int*, 0x006E4E44) == 0)
                    return 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x68 != 0)
                    return 1;
                if (EQ_GLOBAL(int, 0x006B6590) != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x25] != 0)
                    return 1;
                if (*(int*)((char*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info + 0x18C) != 0)
                    return 1;
                if (EQ_GLOBAL(int, 0x006E4DB8) != 0)
                    return 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis != '\0')
                {
                    object = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x0C, &effectSlot);
                    EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(object);
                    if (effectSlot != -1)
                        EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)effectSlot);
                    object = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x1C, &effectSlot);
                    EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(object);
                    if (effectSlot != -1)
                        EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)effectSlot);
                    object = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x1D, &effectSlot);
                    EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(object);
                    if (effectSlot != -1)
                        EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)effectSlot);
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis = '\0';
                    outgoing_packets.visibility.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                    outgoing_packets.visibility.count = 3;
                    outgoing_packets.visibility.invis = (u32)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis;
                    send_message_guess
                        (g_world_connection_guess, 0x20F5,
                            &outgoing_packets.visibility.spawn_id, 0x0C, 1);
                    EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x397] = EQ_GLOBAL(int*, 0x006E4E44)[0x397] + 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 1)
                {
                    msgWord0 = 0xFFFFFFFC;
                    msgWord1 = EQ_GLOBAL(int*, 0x006E4E44)[0x397];
                    msgWord2 = 0x0E;
                    send_message_guess
                        (g_world_connection_guess, 0x2147, &msgWord0, 0x5C, 1);
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x397] = 1;
                merchantSpawnId = rightPick->spawn_id;
                selfSpawnId = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                if (EQ_GLOBAL(u8, 0x006B710C) != '\0')
                {
                    EQ_GLOBAL(u8, 0x006B710C) = '\0';
                    EQ_GLOBAL(u8, 0x006B710D) = 0;
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0053E998, 0x0D);
                    EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
                }
                data = &merchantSpawnId;
                size = 0x10;
                opcode = 0x200B;
                /* fake match: reuse the original common packet-send tail. */
                goto sendPacket;
            }
        }
            else
            {
                if (g_game_state_guess == 3)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44) == 0)
                    return 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x68 != 0)
                    return 1;
                if (EQ_GLOBAL(int, 0x006B6590) != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x25] != 0)
                    return 1;
                if (g_cdisplay_guess->itemOnCursor_guess == '\0')
                {
                    if (rightPick != EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                        EQ_CDECL(void, 0x004A6A08, (EQPlayer*, const char*))
                            (rightPick, (const char*)0x0053E89C);
                    if (EQ_GLOBAL(int, 0x006E20E4) != 0)
                        EQ_METHOD(1, void, 0x004D9CAC, (void*, EQPlayer*), EQ_GLOBAL(void*, 0x006EFE2C))(rightPick);
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
                return 1;
            }
        }
        else
        {
            distance = EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(rightPick,
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 1.0f);
            if (distance <= 20.0f)
            {
                if (g_pc_guess->class_ + 0x10 == (u32)rightPick->class_)
                {
                if (EQ_GLOBAL(int*, 0x006E4E44) == 0)
                    return 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x68 != 0)
                    return 1;
                if (EQ_GLOBAL(int, 0x006B6590) != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 0)
                    return 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x25] != 0)
                    return 1;
                if (*(int*)((char*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info + 0x18C) != 0)
                    return 1;
                if (EQ_GLOBAL(int, 0x006E4DC4) != 0)
                    return 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis != '\0')
                {
                    object = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x0C, &effectSlot);
                    EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(object);
                    if (effectSlot != -1)
                        EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)effectSlot);
                    object = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x1C, &effectSlot);
                    EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(object);
                    if (effectSlot != -1)
                        EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)effectSlot);
                    object = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x1D, &effectSlot);
                    EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(object);
                    if (effectSlot != -1)
                    EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)effectSlot);
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis = '\0';
                    outgoing_packets.visibility.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                    outgoing_packets.visibility.count = 3;
                    outgoing_packets.visibility.invis = (u32)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis;
                    send_message_guess
                        (g_world_connection_guess, 0x20F5,
                            &outgoing_packets.visibility.spawn_id, 0x0C, 1);
                    EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x397] = EQ_GLOBAL(int*, 0x006E4E44)[0x397] + 1;
                if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 1)
                {
                msgWord0 = 0xFFFFFFFC;
                msgWord1 = EQ_GLOBAL(int*, 0x006E4E44)[0x397];
                msgWord2 = 0x0E;
                    send_message_guess
                    (g_world_connection_guess, 0x2147, &msgWord0, 0x5C, 1);
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x397] = 1;
                buffers.packet.words[0] = rightPick->spawn_id;
                buffers.packet.words[1] = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                for (i = 0; i < 0x19; i = i + 1)
                    buffers.packet.words[i + 2] = 0;
                buffers.packet.tail = 0.0f;
                if (EQ_GLOBAL(u8, 0x006B710C) != '\0')
                {
                    EQ_GLOBAL(u8, 0x006B710C) = '\0';
                    EQ_GLOBAL(u8, 0x006B710D) = 0;
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0053E998, 0x0D);
                    EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
                }
                data = buffers.packet.words;
                size = 0x94;
                opcode = 0x209C;
                /* fake match: reuse the original common packet-send tail. */
                goto sendPacket;
            }
                else
                {
                    object = EQ_METHOD(1, char*, 0x004C9D4E, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))((u8)(rightPick->class_ - 0x10));
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                        (buffers.message, (const char*)0x0053E960, object);
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(buffers.message, 10);
                    return 1;
                }
        }
    }
    }
    else
    {
        value = EQ_METHOD(2, int, 0x004DD440, (void*, EQPlayer*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rightPick, 0);
        if (value != 0)
            return 1;
    }

    if (((((g_game_state_guess != 3) &&
        (EQ_GLOBAL(int*, 0x006E4E44) != 0)) &&
        (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x68 == 0)) &&
        ((EQ_GLOBAL(int, 0x006B6590) == 0) &&
        (EQ_GLOBAL(int*, 0x006E4E44)[0x397] == 0))) &&
        (EQ_GLOBAL(int*, 0x006E4E44)[0x25] == 0))
        {
        if ((g_cdisplay_guess->itemOnCursor_guess == '\0') &&
            (rightPick != EQ_GLOBAL(EQPlayer*, 0x006E4D8C)))
        {
            EQ_CDECL(void, 0x004A6A08, (EQPlayer*, const char*))
                (rightPick, (const char*)0x0053E89C);
        }
        if (EQ_GLOBAL(int, 0x006E20E4) != 0)
            EQ_METHOD(1, void, 0x004D9CAC, (void*, EQPlayer*), EQ_GLOBAL(void*, 0x006EFE2C))(rightPick);
        EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
    }
    return 1;

sendPacket:
    send_message_guess
        (g_world_connection_guess, opcode, data, size, 1);
    return 1;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0041f0c0
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (Mac: EQ_Character::BardCastBard, low)
// __thiscall, `this` unused. 1 when the caster class is 8 (bard) and the spell has a bard level of 1..60.
int EQ_Character::BardCastBard_guess(EQ_Spell_guess* spell, int cls)
{
    if (spell != 0 && cls == 8 && spell->class_level[7] > 0 && spell->class_level[7] < 61)
        return 1;
    return 0;
}

class EQ_Character_SpellFizzled_guess : public CharacterProfile
{
public:
    bool SpellFizzled(u8 slot, EQ_Spell_guess* spell);
};

// FUNCTION: TRILOGY 0x0041f0e7
// Rolls whether a spell cast fizzles (casting stat, skill, spell level); a fizzle costs mana and says so.
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
// @early-stop: class-8 select compiles to setcc arithmetic (orig branches); spell is re-cached in EBX after the threshold (orig reloads into EAX), so tmp spills to the param slot; chat string is selected into EAX before 0x121 is pushed; EQ_THISCALL dummy-EDX xors are extra
/* fake match: the threshold is its own s16 (BX in the asm), and rawChance <= 0x5f reuses the 0x5f still in EDI for the compare. */
bool EQ_Character_SpellFizzled_guess::SpellFizzled(u8 slot, EQ_Spell_guess* spell)
{
    u8 bVar1;
    char usableSkill;
    int classId;
    int charisma;
    u32 specialSkill;
    int tmp;
    s32 result;
    int lossCap;
    int randomPenalty;
    s16 acc;
    s16 threshold;
    s16 the_percent;
    char* chatLine;
    u8 local_5;

    local_5 = spell->baseDifficulty_guess;
    if (this->player == 0)
        return false;
    if (this->player->type != '\0')
        return false;
    if (slot > 7)
        return false;
    if (local_5 > 0)
    {
        if (spell == EQ_GLOBAL(EQ_Spell_guess*, 0x005FE9C4) && this->class_ != 2)
            local_5 = 0;
        classId = this->class_;
        if (*(u8*)((char*)spell + 0x234 + classId) >= 0x38)
            local_5 = 0;
        switch (classId)
        {
        case 8:
            classId = (u8)EQ_METHOD(0, s32, 0x0041B48F, (CharacterProfile*), (CharacterProfile*)this)();
            charisma = (u8)EQ_METHOD(0, s32, 0x0041B38F, (CharacterProfile*), (CharacterProfile*)this)();
            acc = (charisma + classId) / 2;
            if (0xf < local_5)
                local_5 = 0xf;
            break;
        case 2:
        case 6:
        case 10:
        case 15:
            bVar1 = EQ_METHOD(0, u8, 0x0041B88A, (CharacterProfile*), (CharacterProfile*)this)();
            acc = bVar1;
            break;
        case 3:
        case 4:
            bVar1 = EQ_METHOD(0, u8, 0x0041B88A, (CharacterProfile*), (CharacterProfile*)this)();
            acc = bVar1;
            if (0x28 < *(u8*)((char*)spell + 0x234 + this->class_))
                local_5 = 0;
            break;
        case 5:
            result = EQ_METHOD(0, s32, 0x0041B5CC, (CharacterProfile*), (CharacterProfile*)this)();
            bVar1 = (u8)result;
            acc = bVar1;
            if (0x28 < *(u8*)((char*)spell + 0x234 + this->class_))
                local_5 = 0;
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            result = EQ_METHOD(0, s32, 0x0041B5CC, (CharacterProfile*), (CharacterProfile*)this)();
            bVar1 = (u8)result;
            acc = bVar1;
            break;
        default:
            acc = 0;
            break;
        }
        if (acc > 0)
        {
            s16 rawChance;
            threshold = *(u8*)((char*)spell + 0x234 + this->class_) - 1;
            if (0x32 < threshold)
                threshold = 0x32;
            usableSkill = EQ_METHOD(1, char, 0x0041A3E9, (CharacterProfile*, u8), (CharacterProfile*)this)(spell->castingSkill_guess);
            if (usableSkill != '\0')
                the_percent = (s16)EQ_METHOD(1, u32, 0x00419867, (CharacterProfile*, u32), (CharacterProfile*)this)((u32)spell->castingSkill_guess);
            else
                the_percent = 0;
            randomPenalty = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 10);
            rawChance = ((((int)acc / 10 + (0x12 - threshold) * 5) - (u16)local_5) + the_percent) - randomPenalty;
            acc = this->class_ == '\b' ? 1 : 5;
            if (rawChance >= acc)
            {
                acc = 0x5f;
                if (rawChance <= 0x5f)
                    acc = rawChance;
            }
        }
    }
    else
    {
        acc = 0x5f;
    }
    specialSkill = EQ_CDECL(int, 0x004197CD, (int))
        (spell->castingSkill_guess);
    if (specialSkill != 0)
    {
        s16 rawChance;
        rawChance = 0;
        specialSkill = EQ_METHOD(1, u32, 0x00419867, (CharacterProfile*, u32), (CharacterProfile*)this)(specialSkill);
        if (0 < (int)specialSkill && (int)specialSkill < 0xfc)
            rawChance = (s16)((int)specialSkill / 10) + 1;
        acc = acc + rawChance;
        if (0x62 < acc)
            acc = 0x62;
    }
    the_percent = EQ_CDECL(int, 0x004A473E, (void))();
    tmp = (int)EQ_METHOD(2, void*, 0x0042381B, (CharacterProfile*, u8, s16*), (CharacterProfile*)this)(0x60, 0);
    if (tmp != 0)
        the_percent = acc + 1;
    if (the_percent > acc)
    {
        tmp = ((int)*(s16*)((char*)spell + 0x1c8) << 2) / 10;
        result = EQ_METHOD(0, s32, 0x00419BCA, (CharacterProfile*), (CharacterProfile*)this)();
        lossCap = (int)(s16)result / 8;
        if (tmp > lossCap)
            tmp = lossCap;
        this->mana = this->mana - (s16)tmp;
        if (this->mana < 0)
            this->mana = 0;
        EQ_CDECL(void, 0x004A3D01, (const char*, int, int))
            (EQ_ARRAY(char, 0x005411CC), (int)acc, (int)the_percent);
        tmp = EQ_METHOD(2, int, 0x0041F0C0, (CharacterProfile*, EQ_Spell_guess*, int), (CharacterProfile*)this)(spell, this->class_);
        if (tmp == 0)
            chatLine = EQ_ARRAY(char, 0x005411B8);
        else
            chatLine = EQ_ARRAY(char, 0x00541188);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(chatLine, 0x121);
        return true;
    }
    return false;
}

// FUNCTION: TRILOGY 0x0041f381
// Begins casting a memorized or item-click spell: checks caster state, item, level and target, sends the cast
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
// @early-stop: type 6/7/8 handlers, stack slots, and final call tails still differ.
// The class declaration predates this recovered method, so keep its thiscall register layout in a
// file-local fastcall entry (ECX=this, EDX unused, four stack arguments).
/* fake match: preserve the recovered error-label gotos to retain VC6 block layout. */
/* fake match: the 132-byte message buffer reproduces the original 0xa4 stack frame. */
bool __fastcall EQ_Character_CastSpell_guess(EQ_Character* self, int edx,
    u32 slot, u32 spellIdValue, int* itemRef, u16 itemSlot)
{
    u8 effectType;
    float verticalDelta;
    int verticalOutOfRange;
    char conditionResult;
    short shortValue;
    int integerValue;
    int* intPointer;
    u32 durationValue;
    int currentEntity;
    u32 spellPtr;
    char* messageText;
    u32 messageValue;
    char messageBuffer[132];
    struct VisibilityMessage
    {
        u32 spawnId;
        u16 type;
        u16 unused;
        u32 mode;
    };
    union SpellIdScratch
    {
        u32 value;
        struct
        {
            short spellId;
            short affectId;
        } parts;
    } spellIdScratch;
    u16 castSlot;
    short spellId;
    u32 spawnPayload;
    u32 payloadWord;
    u32 payloadTail;
    int itemPtr;

    if (*(int*)((char*)self + 0xa0) == 0)
        return false;
    if (*(int*)(*(int*)((char*)self + 0xa0) + 0x70) == 0)
        return false;
    itemPtr = itemRef == 0 ? 0 : *itemRef;
    currentEntity = itemPtr;
    if (EQ_GLOBAL(void*, 0x006E4E44) != 0 &&
        (integerValue = EQ_METHOD(0, int, 0x0047BA1F, (void*), EQ_GLOBAL(void*, 0x006E4E44))(), integerValue == 0) &&
        self->class_ != 8)
    {
        if (itemRef != 0)
            return false;
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(char, 0x00541714), 0x121);
        return false;
    }
    if (*(int*)((char*)self + 0xa0) != 0 &&
        *(char*)(*(int*)((char*)self + 0xa0) + 0xa1) != 'd')
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
             EQ_ARRAY(char, 0x005416EC), 0x121);
        if (*(char*)(*(int*)((char*)self + 0xa0) + 0xa1) == 'f')
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                EQ_ARRAY(char, 0x005410A8), 0x121);
            return false;
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(char, 0x005416EC), 0x121);
        return false;
    }
    shortValue = EQ_METHOD(1, short, 0x004238A2, (EQ_Character*, int), self)(0x28);
    if (0 < shortValue)
    {
        EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
            (self, EQ_ARRAY(char, 0x005416C0), 0x121);
        return false;
    }
    if (*(int*)((char*)self + 0xa0) != 0 &&
        *(char*)(*(int*)((char*)self + 0xa0) + 0x98) == '\0' &&
        self->unknown0x0076 == 1)
    {
        EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
            (self, EQ_ARRAY(char, 0x00541698), 0x121);
        return false;
    }
    EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), self)(
        EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), self)(0x0C, 0));
    EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), self)(
        EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), self)(0x5E, 0));
    EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), self)(
        EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), self)(0x1C, 0));
    EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), self)(
        EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), self)(0x1D, 0));
    messageText = (char*)(*(int*)((char*)self + 0xA0) + 0x9E);
    if (*messageText != '\0')
    {
        VisibilityMessage local_message;
        *messageText = '\0';
        local_message.spawnId = *(u32*)(*(int*)((char*)self + 0xA0) + 0x80);
        local_message.type = 3;
        local_message.mode =
            (u32)*(u8*)(*(int*)((char*)self + 0xA0) + 0x9E);
        messageValue = (u32)EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(0x0C,
                 &spellIdScratch.parts.affectId);
        EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))((void*)messageValue);
        if (spellIdScratch.parts.affectId != -1)
        {
            EQ_METHOD(1, void, 0x004251B9, (EQ_Character*, int), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(
                 spellIdScratch.parts.affectId);
        }
        messageValue = (u32)EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(0x5E,
                 &spellIdScratch.parts.affectId);
        EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))((void*)messageValue);
        if (spellIdScratch.parts.affectId != -1)
        {
            EQ_METHOD(1, void, 0x004251B9, (EQ_Character*, int), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(
                 spellIdScratch.parts.affectId);
        }
        messageValue = (u32)EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(0x1C,
                 &spellIdScratch.parts.affectId);
        EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))((void*)messageValue);
        if (spellIdScratch.parts.affectId != -1)
        {
            EQ_METHOD(1, void, 0x004251B9, (EQ_Character*, int), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(
                 spellIdScratch.parts.affectId);
        }
        messageValue = (u32)EQ_METHOD(2, void*, 0x0042381B, (EQ_Character*, u8, short*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(0x1D,
                 &spellIdScratch.parts.affectId);
        EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, void*), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))((void*)messageValue);
        if (spellIdScratch.parts.affectId != -1)
        {
            EQ_METHOD(1, void, 0x004251B9, (EQ_Character*, int), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(
                 spellIdScratch.parts.affectId);
        }
        send_message_guess
            (g_world_connection_guess, 0x20F5, &local_message, 0x0C, 1);
        EQ_METHOD(1, void, 0x00410B68, (void*, void*), g_display_guess)(
             (void*)*(int*)((char*)self + 0xA0));
    }
    EQ_METHOD(0, void, 0x004D03CE, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    if (currentEntity != 0)
    {
        *(u32*)((char*)self + 0xAA0) = 0;
        if (*(char*)(currentEntity + 0x81) != '\0')
            return false;
        if (*(char*)(currentEntity + 0xC2) == '\x14')
            return false;
        shortValue = *(short*)(*(int*)(*(int*)((char*)self + 0xA0) + 0x70) + 0x248);
        if (shortValue != -1)
        {
            if (shortValue <= 0)
                return false;
            if (shortValue >= 3000)
                return false;
            integerValue = EQ_METHOD(2, int, 0x0041F0C0, (EQ_Character*, EQ_Spell_guess*, int), self)((EQ_Spell_guess*)EQ_ARRAY(u32, 0x005FE9A0)[shortValue],
                     self->class_);
            if (integerValue == 0)
                return false;
        }
        integerValue = *(int*)((char*)self + 0xA0);
        if (*(char*)(integerValue + 0x98) == '\0')
        {
            switch (*(u8*)(currentEntity + 0xC0))
            {
            case 5:
                intPointer = (int*)EQ_METHOD(1, int, 0x0041BA72, (EQ_Character*, void*), self)((void*)currentEntity);
                if (intPointer == 0)
                {
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x0054166C), 0x0D);
                    return false;
                }
                break;
            case 4:
                verticalOutOfRange = false;
                intPointer = (int*)((char*)self + 0xE4);
                integerValue = 0x15;
                do
                {
                    if (currentEntity == *intPointer)
                        verticalOutOfRange = true;
                    intPointer = intPointer + 1;
                    integerValue = integerValue - 1;
                } while (integerValue != 0);
                if (!verticalOutOfRange)
                {
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x0054160C), 0x0D);
                    return false;
                }
                break;
            default:
                goto checkItemSpell;
            }
            if (*(u8*)(currentEntity + 0xC4) > self->level)
            {
                EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                    (self, EQ_ARRAY(char, 0x0054163C), 0x0D);
                return false;
            }
        }
checkItemSpell:
        if (*(u8*)(currentEntity + 0xD8) < 2)
            return false;
        shortValue = *(short*)(currentEntity + 0xDC);
        spellIdScratch.parts.spellId = shortValue;
        if (shortValue < 0)
            return false;
        if (shortValue >= 3000)
            return false;
        spellPtr = EQ_ARRAY(u32, 0x005FE9A0)[shortValue];
        if (spellPtr == 0)
            return false;
        if (*(char*)(currentEntity + 0xDA) == '\0')
        {
            EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                (self, EQ_ARRAY(char, 0x005415F4), 0x111);
            return false;
        }
    }
    else
    {
        currentEntity = *(int*)((char*)self + 0xA0);
        if (*(short*)(*(int*)(currentEntity + 0x70) + 0x248) != -1)
        {
            EQ_CDECL(int, 0x005229E8, (const char*))
                (EQ_ARRAY(char, 0x005415D0));
            return false;
        }
        if (0x21 < (u8)slot)
            return false;
        if ((u8)slot <= 7)
            spellIdScratch.value = (u32)*(u16*)((char*)self + 0x952 + ((u8)slot & 0xFF) * 2);
        else
            spellIdScratch.value = spellIdValue;
        spellPtr = EQ_ARRAY(u32, 0x005FE9A0)[spellIdScratch.parts.spellId];
        if (spellPtr == 0)
            return false;
        if ((u8)slot <= 7 &&
            *(u8*)((char*)spellPtr +
                    self->class_ + 0x234)
                > self->level)
        {
            messageText = EQ_ARRAY(char, 0x0054159C);
            goto chatFail;
        }
        if (currentEntity != 0 && *(char*)(currentEntity + 0x98) != '\x01' && currentEntity != 0 &&
            *(char*)(currentEntity + 0x98) == '\0')
        {
            *(u32*)((char*)self + 0xAA0) = 0;
        }
    }
    if (g_pvpRuleset_guess == 4 &&
        (currentEntity = *(int*)((char*)self + 0xA0), currentEntity != 0) &&
        *(char*)(currentEntity + 0x98) == '\0' &&
        (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0 &&
         EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type == '\0') &&
        *(char*)(spellPtr + 0x21F) == '\x01' &&
        *(char*)(currentEntity + 0xF1) == '\0' &&
        *(char*)(spellPtr + 0x22E) == '\x05' &&
        (conditionResult = EQ_CDECL(char, 0x0048A11F, (void*, int))
            ((void*)spellPtr, 0x1B), conditionResult == '\0') &&
        (currentEntity = EQ_CDECL(int, 0x004B21FC,
            (EQPlayer*, EQPlayer*, EQ_Spell_guess*))
                (EQ_GLOBAL(EQPlayer*, 0x006E4DAC),
                 (EQPlayer*)*(int*)((char*)self + 0xA0),
                 (EQ_Spell_guess*)spellPtr), currentEntity != 0))
    {
        EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
            (self, EQ_ARRAY(char, 0x00541570), 0x121);
        return false;
    }
    currentEntity = EQ_METHOD(2, int, 0x0041F0C0, (EQ_Character*, EQ_Spell_guess*, int), self)((EQ_Spell_guess*)spellPtr,
             self->class_);
    if (currentEntity == 0)
    {
        if (*(char*)((char*)self + 0x994) != 'd')
        {
            EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                (self, EQ_ARRAY(char, 0x00541510), 0x121);
            return false;
        }
    }
    else if (*(char*)((char*)self + 0x994) == 's' ||
        *(char*)((char*)self + 0x994) == 'x')
    {
        EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
            (self, EQ_ARRAY(char, 0x00541544), 0x121);
        return false;
    }
    currentEntity = *(int*)((char*)self + 0xA0);
    if (currentEntity == 0)
    {
        EQ_CDECL(void, 0x004A3D01, (const char*))
            (EQ_ARRAY(char, 0x005414E8));
        return false;
    }
    if (*(int*)((char*)self + 0xAB8) != 1 && itemPtr == 0 &&
        (shortValue = EQ_METHOD(0, short, 0x00419BA3, (EQ_Character*), self)(), shortValue < *(short*)(spellPtr + 0x1C8)))
    {
        EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
            (self, EQ_ARRAY(char, 0x005414C0), 0x121);
        return false;
    }
    effectType = *(u8*)(spellPtr + 0x22E);
    if (effectType == 5)
    {
checkTarget:
        if (itemPtr != 0 && *(char*)(itemPtr + 0x81) == '\0' &&
            *(char*)(itemPtr + 0xC2) == '\x15' &&
            1 < *(u8*)(itemPtr + 0xD8))
        {
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = EQ_GLOBAL(EQPlayer*, 0x006E4DA0);
            pinstEQ_Main_guess->uiDirty_u32_guess = 1;
        }
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0)
        {
            if (*(float*)(spellPtr + 0x1A0) < EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(
                     EQ_GLOBAL(EQPlayer*, 0x006E4DAC), (EQPlayer*)currentEntity, 0.0f))
            {
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (messageBuffer, EQ_ARRAY(char, 0x005413C4));
                EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                    (self, messageBuffer, 0x121);
                return false;
            }
            switch (*(u8*)(spellPtr + 0x22E))
            {
            case 0x0B:
                if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) == '\0' &&
                    (integerValue = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type,
                     integerValue != 0x18 && integerValue != 0x1B && integerValue != 8 && integerValue != 0x1C))
                {
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x00541398), 0x121);
                    return false;
                }
                break;
            case 0x0E:
                if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) == '\0' &&
                    (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type != 1 ||
                     EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->pet_owner_id !=
                        *(u32*)(*(int*)((char*)self + 0xA0) + 0x80)))
                {
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x00541370), 0x121);
                    return false;
                }
                break;
            case 0x0C:
                break;
            case 10:
                if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) == '\0' &&
                    EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type != 3 &&
                    EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type != 8)
                {
                    messageValue = 0x121;
                    messageText = EQ_ARRAY(char, 0x00541314);
                    goto failWithMessage;
                }
                break;
            case 0x10:
                if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) == '\0' &&
                    EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type != 0x19)
                {
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x005412EC), 0x121);
                    return false;
                }
                break;
            case 0x11:
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type != 9)
                {
                    if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) != '\0')
                        return false;
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x005412C0), 0x121);
                    return false;
                }
                break;
            case 0x0F:
                if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) == '\0' &&
                    EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type < 2)
                {
                    messageValue = 0x121;
                    messageText = EQ_ARRAY(char, 0x00541244);
                    goto failWithMessage;
                }
                break;
            case 9:
                if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) != '\0' ||
                    EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type == 0x15)
                    break;
                messageValue = 0x121;
                messageText = EQ_ARRAY(char, 0x00541220);
                goto failWithMessage;
            case 0x0D:
            case 0x14:
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) ==
                    (EQPlayer*)*(int*)((char*)self + 0xA0))
                {
                    messageValue = 0x121;
                    messageText = EQ_ARRAY(char, 0x00541204);
                    goto failWithMessage;
                }
                break;
            case 0x12:
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type != 0x1E)
                {
                    if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) != '\0')
                        return false;
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x00541294), 0x121);
                    return false;
                }
                break;
            case 0x13:
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->body_type != 10)
                {
                    if (*(char*)(*(int*)((char*)self + 0xA0) + 0x98) != '\0')
                        return false;
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x00541268), 0x121);
                    return false;
                }
                break;
            }
            goto beginCast;
        }
    }
    else
    {
        if (effectType == 6)
        {
            conditionResult = EQ_CDECL(char, 0x0048A11F, (void*, int))
                ((void*)spellPtr, 0x5B);
            if (conditionResult != '\0')
            {
                integerValue = *(int*)((char*)self + 0xA0);
                if (integerValue == 0)
                    return false;
                if (*(char*)(integerValue + 0x98) == '\x01')
                    return false;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0 ||
                    EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type != '\0' ||
                    (*(char*)(integerValue + 0xF1) == '\0' &&
                     (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->char_info_guess != 0 &&
                      (integerValue = EQ_METHOD(1, int, 0x0047EFCE, (CharacterProfile*, EQ_Character*), (CharacterProfile*)EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->char_info_guess)(self), integerValue == 0))))
                {
                    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                        (self, EQ_ARRAY(char, 0x005413F0), 0x121);
                    return false;
                }
            }
            goto beginCast;
        }
        if (effectType == 7)
        {
            if (itemPtr != 0)
                return false;
            if (*(int*)((char*)self + 0xE0) == 0)
            {
                messageValue = 0x121;
                messageText = EQ_ARRAY(char, 0x00541440);
                goto failWithMessage;
            }
            goto beginCast;
        }
        if (effectType != 8)
        {
            if (effectType < 9 || (0x12 < effectType && effectType != 0x14))
                goto beginCast;
            goto checkTarget;
        }
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
        {
            messageValue = 0x121;
            messageText = EQ_ARRAY(char, 0x00541490);
            goto failWithMessage;
        }
        else
        {
            verticalDelta = (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->z -
                     EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->height_offset) -
                    (*(float*)(*(int*)((char*)self + 0xA0) + 0x3C) -
                     *(float*)(*(int*)((char*)self + 0xA0) + 0x7C));
            if (verticalDelta < EQ_GLOBAL(float, 0x0053510C))
                verticalOutOfRange = -verticalDelta > EQ_GLOBAL(float, 0x00535300);
            else
                verticalOutOfRange = verticalDelta > EQ_GLOBAL(float, 0x00535300);
            if (*(float*)(spellPtr + 0x1A0) < EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(
                     EQ_GLOBAL(EQPlayer*, 0x006E4DAC), (EQPlayer*)currentEntity, 0.0f) ||
                verticalOutOfRange)
            {
                messageValue = (u32)EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                         EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
                EQ_CDECL(int, 0x005224FB,
                    (char*, const char*, ...))
                        (messageBuffer, EQ_ARRAY(char, 0x00541470), (char*)messageValue);
                EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                    (self, messageBuffer, 0x121);
                return false;
            }
            goto beginCast;
        }
    }
    messageValue = 0x121;
    messageText = EQ_ARRAY(char, 0x00541490);
failWithMessage:
    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
        (self, messageText, (int)messageValue);
    return false;

chatFail:
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(messageText, 0x121);
    return false;

beginCast:
    if (*(int*)((char*)self + 0xAB8) != 1 && itemPtr == 0 &&
        EQ_GLOBAL(u8, 0x006B63F8) != '\0' &&
        (conditionResult = EQ_METHOD(2, char, 0x0041F0E7, (EQ_Character*, u32, u32), self)(slot, spellPtr), conditionResult != '\0'))
    {
        return false;
    }
    *(u32*)(*(int*)(currentEntity + 0x70) + 0x2EC) = 0;
    *(u32*)(*(int*)(currentEntity + 0x70) + 0x2F0) = 0xFFFFFFFF;
    if (itemPtr == 0)
    {
        durationValue = *(u32*)(spellPtr + 0x1B0);
        if (0x32 < self->level &&
            ((*(char*)((char*)self + 0x36) == '\x04' ||
              *(char*)((char*)self + 0x36) == '\x05' ||
              *(char*)((char*)self + 0x36) == '\x03') &&
             (durationValue >= 3000 && *(char*)(spellPtr + 0x21F) == '\0')))
        {
            payloadTail = 0;
            payloadWord = durationValue;
            durationValue = (u32)((double)payloadWord *
                (EQ_GLOBAL(float, 0x0053515C) -
                 ((int)self->level - 0x32) *
                    EQ_GLOBAL(float, 0x005351D0)) *
                EQ_GLOBAL(float, 0x005352E8));
        }
        *(u32*)(*(int*)(currentEntity + 0x70) + 200) =
            g_cdisplay_guess->time_ms_guess + durationValue;
        *(u16*)(*(int*)(currentEntity + 0x70) + 0x248) = (u16)spellIdScratch.parts.spellId;
        *(u8*)(*(int*)(currentEntity + 0x70) + 0x24A) = (u8)slot;
        EQ_GLOBAL(u16, 0x006E1E0A) = (u16)*(u8*)(spellPtr + 0x21F);
        spawnPayload = *(u32*)(currentEntity + 0x80);
        payloadWord = (payloadWord & 0xFFFF0000) | (u16)spellIdScratch.parts.spellId;
        payloadTail = durationValue;
        if (EQ_GLOBAL(u8, 0x006B63F8) != '\0' ||
            DAT_006b6494 != 0)
        {
            EQ_METHOD(1, void, 0x004C86F8, (void*, void*), EQ_GLOBAL(void*, 0x006EFE2C))(&spawnPayload);
        }
    }
    else if (EQ_GLOBAL(u8, 0x006B63F8) != '\0' ||
        DAT_006b6494 != 0)
    {
        EQ_METHOD(3, bool, 0x00420016, (EQ_Character*, u32, u32, int*), self)(slot, spellIdValue, itemRef);
    }
    currentEntity = *(int*)((char*)self + 0xA0);
    if (currentEntity != 0)
    {
        *(u32*)(*(int*)(currentEntity + 0x70) + 0x2B4) = *(u32*)(currentEntity + 0x34);
        *(u32*)(*(int*)(*(int*)((char*)self + 0xA0) + 0x70) + 0x2B8) =
            *(u32*)(*(int*)((char*)self + 0xA0) + 0x38);
    }
    if (EQ_GLOBAL(u8, 0x006B63F8) == '\0' &&
        DAT_006b6494 == 0)
    {
        castSlot = (u16)(u8)slot;
        spellId = spellIdScratch.parts.spellId;
        spawnPayload = (spawnPayload & 0xFFFF0000) | itemSlot;
        payloadTail = EQ_CDECL(u32, 0x004E04A2, (void*, s32))
            ((void*)EQ_ARRAY(u32, 0x005FE9A0)[spellId], 600);
        EQ_CDECL(int, 0x004A0792, (u8*))((u8*)&payloadTail);
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
            payloadWord = 0;
        else
            payloadWord = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id;
        if ((u8)slot < 8)
        {
            *(int*)(*(int*)(*(int*)((char*)self + 0xA0) + 0x70) +
                0xD4 + ((u8)slot & 0xFF) * 4) =
                g_cdisplay_guess->time_ms_guess + 10000;
            *(int*)(*(int*)(*(int*)((char*)self + 0xA0) + 0x70) + 0x15C) =
                g_cdisplay_guess->time_ms_guess + 10000;
        }
        if ((u8)slot == 9)
        {
            *(int*)(*(int*)(*(int*)((char*)self + 0xA0) + 0x70) + 0xF8) =
                g_cdisplay_guess->time_ms_guess + 10000;
        }
        send_message_guess
            (g_world_connection_guess, 0x217E, &castSlot, 0x10, 1);
        currentEntity = EQ_METHOD(2, int, 0x0041F0C0, (EQ_Character*, EQ_Spell_guess*, int), self)((EQ_Spell_guess*)spellPtr,
                 self->class_);
        if (currentEntity == 0 && EQ_GLOBAL(void*, 0x006E4E44) != 0)
        {
            pinstEQ_Main_guess->pendingServerReplies_guess =
                pinstEQ_Main_guess->pendingServerReplies_guess + 1;
            intPointer = (int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x94);
            ++*intPointer;
        }
    }
    return true;
}

// FUNCTION: TRILOGY 0x00420016
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (Mac: EQ_Character::LaunchSpell, low)
// __thiscall, 3 args, always true in this build.
bool EQ_Character::LaunchSpell_guess(int slot, int spellIdValue, int itemRef)
{
    return true;
}

// FUNCTION: TRILOGY 0x0042001b
// Checks whether a spell effect slot has a meaningful effect.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (address range; TAKP: IsSpellEffectValid)
// static (__cdecl). Whether effect slot `slotIndex` of the spell does something: not unused (0xFE), and not
// effect 10 with a base value of 0.
int EQ_Character::IsSpellEffectValid_guess(EQ_Spell_guess* spell, int slotIndex)
{
    u8 effect = spell->effect_id[slotIndex];

    if (effect == EQ_SPELL_EFFECT_NONE)
        return 0;
    if (effect == 10 && spell->base[slotIndex] == 0)
        return 0;
    return 1;
}

// FUNCTION: TRILOGY 0x00420046
// Applies a spell's movement speed change to the character.
void __fastcall ApplySpellMovementRate_guess(EQ_Character* character, int edx_unused, short affectValue)
{
    int tmp;
    float* movementRatePtr;

    if ((*(int*)((char*)character + 0xa0) != 0) &&
        (tmp = *(int*)(*(int*)((char*)character + 0xa0) + 0x70), tmp != 0))
    {
        if (affectValue <= -1000)
        {
            *(float*)(tmp + 0xb8) = (float)(int)affectValue;
            *(float*)(*(int*)((char*)character + 0xa0) + 0x48) = 0.0f;
            *(float*)(*(int*)((char*)character + 0xa0) + 0x4c) = 0.0f;
            *(float*)(*(int*)((char*)character + 0xa0) + 0x50) = 0.0f;
            *(float*)(*(int*)((char*)character + 0xa0) + 0x58) = 0.0f;
            EQ_THISCALL(void, 0x004B117B, (EQPlayer*, int))
                ((EQPlayer*)*(int*)((char*)character + 0xa0), edx_unused);
        }
        else
        {
            if (affectValue < -0x41)
                affectValue = -0x41;
            tmp = EQ_CDECL(int, 0x005232FF, (int))((int)affectValue);
            float movement_rate = (float)tmp * EQ_GLOBAL(float, 0x00535224);
            *(float*)(*(int*)(*(int*)((char*)character + 0xa0) + 0x70) + 0xb8) =
                movement_rate * *(float*)(*(int*)((char*)character + 0xa0) + 0xec);
            if (affectValue < 0)
            {
                movementRatePtr = (float*)(*(int*)(*(int*)((char*)character + 0xa0) + 0x70) + 0xb8);
                *movementRatePtr = -*movementRatePtr;
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0042010b
// Applies levitation to the character.
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
// Starts fly mode (levitate). Called from HandleSpecialPCAffects_guess for a buff whose spell has effect 0x39
// (57). Sets EQPlayer::levitate to 2, writes two actor-info fields, then sends OP_SpawnAppearance (0x20F5)
// type 19 (levitate) with the new value. Does nothing unless the character has an EQPlayer with actor info.
void __fastcall eqspa_levitate_guess(EQ_Character* character, int edx_unused)
{
    EQPlayer* player;
    struct MsgSpawnAppearance_guess
    {
        u32 spawn_id;
        short type;
        int parameter;
    } msg;

    player = character->player;
    if ((player != 0) && (player->actor_info != 0))
    {
        player->levitate = 2;
        character->player->actor_info->unknown0x088 = 0;
        character->player->actor_info->unknown0x078 = 1;
        msg.type = 0x13;
        msg.spawn_id = character->player->spawn_id;
        msg.parameter = (int)character->player->levitate;
        send_message_guess
            (g_world_connection_guess, 0x20f5, &msg, 0xc, 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x00420184
// Finds a corpse by proximity or name and reports its direction or search result.
void __fastcall eqspa_locate_corpse(EQ_Character* character, int edx_unused)
{
    size_t corpseNameLength;
    int nameCompare;
    EQPlayer* pEVar4;
    char* chatLine;
    int chatColor;

    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
    {
        pEVar4 = EQ_METHOD(4, EQPlayer*, 0x004228E2, (EQ_Character*, int, int, int, int), character)(0, -1, -1, -1);
        if (pEVar4 != 0)
        {
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = pEVar4;
showCorpseDirection:
            *(float*)(*(int*)((char*)character + 0xA0) + 0x40) =
                EQ_CDECL(float, 0x004A47F9, (float, float, float, float))(
                    *(float*)(*(int*)((char*)character + 0xA0) + 0x34),
                    *(float*)(*(int*)((char*)character + 0xA0) + 0x38),
                    pEVar4->y, pEVar4->x);
            chatColor = 0x108;
            chatLine = EQ_ARRAY(char, 0x0054178C);
            goto showCorpseResult;
        }
    }
    else
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
             EQ_ARRAY(char, 0x005417B4), 0x108);
        for (pEVar4 = EQ_GLOBAL(EQPlayer*, 0x006E4D58); pEVar4 != 0; pEVar4 = pEVar4->next)
        {
            if ((pEVar4->type == '\x02') || (pEVar4->type == '\x03'))
            {
                corpseNameLength = EQ_CDECL(size_t, 0x005223D0, (const char*))
                    (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
                nameCompare = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))
                    (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name, pEVar4->name, corpseNameLength);
                if (nameCompare == 0)
                {
                    EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = pEVar4;
                    goto showCorpseDirection;
                }
            }
        }
    }
    chatColor = 0x121;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
        chatLine = EQ_ARRAY(char, 0x00541740);
    else
        chatLine = EQ_ARRAY(char, 0x00541760);
showCorpseResult:
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(chatLine, chatColor);
    return;
}

// FUNCTION: TRILOGY 0x00420274
// Checks whether a target is inside a hard-coded region in selected zones.
/* fake match: result lives in EBX and every failed check breaks to one shared return, the exit layout the original has. */
int __fastcall IsInBindableRegion_guess(EQ_Character* character, int edx_unused, EQPlayer* targetPlayer)
{
    int result = 0;
    int compareResult;
    char* mapName;

    if (targetPlayer == 0)
        return 0;

    mapName = (char*)character + 0x974;
    do
    {
        compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))
            (mapName, EQ_ARRAY(char, 0x00541800));
        if (compareResult == 0)
        {
            if (targetPlayer->y <= -2250.0f)
                break;
            if (targetPlayer->y >= -2100.0f)
                break;
            if (targetPlayer->x >= 1560.0f)
                break;
            if (targetPlayer->x > 1180.0f)
                result = 1;
        }
        else
        {
            compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                (mapName, EQ_ARRAY(char, 0x005417F4));
            if (compareResult == 0)
            {
                if (targetPlayer->y <= -2250.0f)
                    break;
                if (targetPlayer->y >= -1200.0f)
                    break;
                if (targetPlayer->x >= 275.0f)
                    break;
                if (targetPlayer->x > -550.0f)
                    result = 1;
            }
            else
            {
                compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                    (mapName, EQ_ARRAY(char, 0x005417E8));
                if (compareResult == 0)
                {
                    if (targetPlayer->y <= -720.0f)
                        break;
                    if (targetPlayer->y >= -550.0f)
                        break;
                    if (targetPlayer->x >= -73.0f)
                        break;
                    if (targetPlayer->x > -260.0f)
                        result = 1;
                }
                else
                {
                    compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                        (mapName, EQ_ARRAY(char, 0x005417E0));
                    if (compareResult == 0)
                    {
                        if (targetPlayer->y <= -4410.0f)
                            break;
                        if (targetPlayer->y >= -2397.0f)
                            break;
                        if (targetPlayer->x >= 3492.0f)
                            break;
                        if (targetPlayer->x > 1194.0f)
                            result = 1;
                    }
                    else
                    {
                        compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                            (mapName, EQ_ARRAY(char, 0x005417D4));
                        if (compareResult == 0)
                        {
                            if (targetPlayer->y <= 2344.0f)
                                break;
                            if (targetPlayer->y >= 3538.0f)
                                break;
                            if (targetPlayer->x >= 3852.0f)
                                break;
                            if (targetPlayer->x > 2011.0f)
                                result = 1;
                        }
                        else
                        {
                            compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                                (mapName, EQ_ARRAY(char, 0x0053DF0C));
                            if (compareResult != 0)
                                break;
                            if (targetPlayer->y >= -320.0f && targetPlayer->y <= 535.0f
                                && targetPlayer->x >= -930.0f && targetPlayer->x <= -200.0f
                                && targetPlayer->z >= -10.0f && targetPlayer->z <= 60.0f)
                            {
                                result = 1;
                                break;
                            }
                            if (targetPlayer->y < 535.0f)
                                break;
                            if (targetPlayer->y > 1000.0f)
                                break;
                            if (targetPlayer->x < -800.0f)
                                break;
                            if (targetPlayer->x > -200.0f)
                                break;
                            if (targetPlayer->z < -10.0f)
                                break;
                            if (targetPlayer->z <= 120.0f)
                                result = 1;
                        }
                    }
                }
            }
        }
    } while (0);
    return result;
}

struct HitSpellMsg_guess
{
    u32     target_id;
    u32     caster_id;
    s16     caster_level;
    u16     unknown0x0A;
    s32     bard_modifier;
    u32     unknown0x10;
    f32     heading;
    u8      unknown0x18[6];
    u16     amount;
    s16     spell_id;
};
C_ASSERT(offsetof(HitSpellMsg_guess, amount) == 0x1E);

struct HitSpellRecord_guess
{
    char    name[0x40];
    char    teleport_zone[0x160];
    f32     range;
    f32     aoe_range;
    f32     push_back;
    f32     push_up;
    u8      unknown0x1B0[0xC];
    u32     duration_formula;
    u32     duration;
    u32     impact_duration;
    u16     mana;
    s16     base[12];
    u8      unknown0x1E2[0x3D];
    u8      good_effect;
    u8      activated;
    u8      resist_type;
    u8      effect_id[12];
    u8      target_type;
    u8      unknown0x22F[0xD];
    u8      bard_level;
};
C_ASSERT(offsetof(HitSpellRecord_guess, base) == 0x1CA);
C_ASSERT(offsetof(HitSpellRecord_guess, effect_id) == 0x222);
C_ASSERT(offsetof(HitSpellRecord_guess, bard_level) == 0x23C);

struct HitSpellBuff_guess
{
    u8      visible;
    u8      level;
    u8      bard_modifier;
    u8      activated;
    s16     spell_id;
    s16     duration;
    s16     counters;
};

struct HitItemStats_guess
{
    s8      str;
    s8      sta;
    s8      cha;
    s8      dex;
    s8      intelligence;
    s8      agi;
    s8      wis;
    s8      sv_magic;
    s8      sv_fire;
    s8      sv_cold;
    s8      sv_disease;
    s8      sv_poison;
    s8      hp;
    s8      mana;
    s8      ac;
    u8      unknown0xBB[4];
    u8      damage;
    u8      unknown0xC0[2];
    u8      skill;
    u8      magic;
};
C_ASSERT(offsetof(HitItemStats_guess, magic) == 0xC3 - 0xAC);

struct HitCorpseMsg_guess
{
    u32     corpse_id;
    char    zone[32];
    f32     y;
    f32     x;
    f32     z;
    s32     amount;
    u8      flag;
    u8      unknown0x35[0x1F];
    u32     unknown0x54;
    char    caster_name[32];
    s16     spell_id;
    char    corpse_name[34];
    u32     unknown0x9C;
};
C_ASSERT(sizeof(HitCorpseMsg_guess) == 0xA0);

struct HitIllusion_guess
{
    char    name[30];
    char    name2[32];
    s16     race;
    u8      gender;
    u8      unknown0x41;
    s16     texture;
    s16     unknown0x44;
    s16     unknown0x46;
};
C_ASSERT(sizeof(HitIllusion_guess) == 0x48);

typedef void (__fastcall *HitWindowMethod_guess)(void* window);

class EQ_Character_HitBySpell_guess : public CharacterProfile
{
public:
    void HitBySpell(HitSpellMsg_guess* msg);
};

#define HBS_EVERQUEST           EQ_GLOBAL(void*, 0x006EFE2C)
#define HBS_TARGET              EQ_GLOBAL(EQPlayer*, 0x006E4DAC)
#define HBS_SOUND_MANAGER       EQ_GLOBAL(void*, 0x006ECDF4)
#define HBS_ZONE_LIST           EQ_GLOBAL(void*, 0x006E4D54)
#define HBS_ZONE_TYPE           EQ_GLOBAL(u8, 0x006B66F4)
#define HBS_SAFE_Y              EQ_GLOBAL(float, 0x006B6734)
#define HBS_SAFE_X              EQ_GLOBAL(float, 0x006B6738)
#define HBS_SAFE_Z              EQ_GLOBAL(float, 0x006B673C)
#define HBS_MEZ_MESSAGE         EQ_GLOBAL(int, 0x00591D84)
#define HBS_REFRESH_LOCK        EQ_GLOBAL(char, 0x006B658C)
#define HBS_NO_FLOOR            -1.0e27f
#define HBS_EMPTY               EQ_ARRAY(char, 0x0058FBA4)
#define HBS_DASHES              EQ_ARRAY(char, 0x00541B70)
#define HBS_SPELL(id)           ((HitSpellRecord_guess*)g_spells_guess[id])
#define HBS_STATS(item)         ((HitItemStats_guess*)((item) + 1))
#define HBS_ACTOR_CASTERS(actor) ((s16*)((u8*)(actor) + 0x344))
#define HBS_STRCPY              EQ_CDECL(char*, 0x005219C0, (char*, const char*))
#define HBS_STRCAT              EQ_CDECL(char*, 0x005219D0, (char*, const char*))
#define HBS_STRLEN              EQ_CDECL(size_t, 0x005223D0, (const char*))
#define HBS_STRCMP              EQ_CDECL(int, 0x00522920, (const char*, const char*))
#define HBS_SPRINTF             EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
#define HBS_PRINT_CHAT(text) \
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), HBS_EVERQUEST)(text)
#define HBS_DSP_CHAT(text, color) \
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), HBS_EVERQUEST)(text, color)
#define HBS_TOKEN_MSG(who, text, color) \
    EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(who, text, color)
#define HBS_ZONE_PC(zone, reason) \
    EQ_METHOD(3, void, 0x004DC1B5, (void*, char*, const char*, int), HBS_EVERQUEST)(zone, reason, 0)
#define HBS_GET_AFFECT(effect) \
    EQ_METHOD(2, HitSpellBuff_guess*, 0x0042381B, (void*, u8, short*), this)(effect, 0)
#define HBS_REMOVE_AFFECT(buff) \
    EQ_METHOD(1, void, 0x00424E22, (void*, HitSpellBuff_guess*), this)(buff)
#define HBS_TOTAL_AFFECTS(effect) \
    EQ_METHOD(1, short, 0x004238A2, (void*, int), this)(effect)
#define HBS_EFFECT_VALUE(spell, level, slot) \
    EQ_METHOD(4, short, 0x0042351A, (void*, HitSpellRecord_guess*, u8, u8, int), this)(spell, level, slot, 0)
#define HBS_BEARING             EQ_CDECL(float, 0x004A47F9, (float, float, float, float))
#define HBS_PLAY_SOUND(p) \
    EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)((p)->y, (p)->x, (p)->z, 0x89)
#define HBS_FLOOR(p)            CDisplay_GetPlayerFloorHeight_guess(g_display_guess, p, (p)->y, (p)->x, (p)->z, 1)
#define HBS_UPDATE_ACTOR(p)     CDisplay_UpdatePlayerActor_guess(g_display_guess, p)
#define HBS_STAT(field, plusFormat, plainFormat) \
    stat = stats->field; \
    if (stat != 0) \
    { \
        HBS_SPRINTF(statText, stat > 0 ? plusFormat : plainFormat, (int)stat); \
        statCount++; \
        HBS_STRCAT(message, statText); \
    }
#define HBS_STAT_FLUSH() \
    if (statCount >= 4) \
    { \
        HBS_PRINT_CHAT(message); \
        statCount = 0; \
        HBS_STRCPY(message, HBS_EMPTY); \
    }

// FUNCTION: TRILOGY 0x00420505
// Applies a spell landing on this character (CharacterProfile this); the argument is the hit message.
// Phases: reject if the player, actor or spell id (3000 or above) is invalid, resolve the caster, handle special
// ids; clear sneak and invisibility for harmful spells; apply push back/up to dx, dy, dz from the hit heading.
// If duration_formula is 0 the effect is instant: loops over 12 effect slots and switches on the effect id.
// Otherwise a buff slot comes from 0x00422dd6, the caster is recorded and the per-slot effects are applied.
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
void EQ_Character_HitBySpell_guess::HitBySpell(HitSpellMsg_guess* hitData)
{
    char languageText[256];
    char zoneName[128];
    u8 spellInfo[0xAC];
    HitCorpseMsg_guess corpseMsg;
    char statText[56];
    HitIllusion_guess illusion;
    char message[128];
    short bodyType3;
    short bodyType2;
    u8 casterLevel;
    CharacterProfile* profile;
    short bodyType1;
    float teleportZ;
    int actorTag;
    u32 duration;
    float distance;
    EQPlayer* player;
    float scale;
    HitSpellRecord_guess* spell;
    u8 slot;
    EQPlayer* caster;
    int spellIndex;
    int buffsLeft;
    int modifier;
    float newY;
    float newX;
    int stackSlot;
    short spellId;
    int value;
    float angle;
    short statCount;
    EQPlayer* current;
    EQPlayer* found;
    EQSwitch* trap;
    EQ_Item* item;
    HitItemStats_guess* stats;
    HitSpellBuff_guess* buff;
    HitSpellBuff_guess* affect;
    HitSpellRecord_guess* other;
    EQActorInfo_guess* actor;
    char* window;
    u8* effect;
    char* lore;
    char* zone;
    int slotIndex;
    int bonus;
    int roll;
    int category;
    int stunTime;
    short result;
    short chance;
    short threshold;
    short tries;
    short skill;
    char ok;
    s8 stat;
    float floor;

    bodyType1 = -1;
    bodyType2 = -1;
    bodyType3 = -1;
    if (this->player == 0)
        return;
    if (this->player->actor_info == 0)
        return;
    spellId = hitData->spell_id;
    if (spellId > 2999)
        return;
    spellIndex = spellId;
    spell = HBS_SPELL(spellIndex);
    if (spell == 0)
        return;
    caster = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), g_player_top_guess)(hitData->caster_id);
    if (caster == 0)
    {
        EQTRACE_guess("Caster is NULL in HitBySpell()!\n");
        return;
    }
    if (HBS_TOTAL_AFFECTS(0x28) > 0 && spellId != 0x3D6)
        return;
    casterLevel = caster->level;
    if (spellId >= 0x4E4 && spellId <= 0x4F2 && hitData->caster_level > 0 && hitData->caster_level <= 0x3C)
        casterLevel = (u8)hitData->caster_level;
    if (caster == this->player && spell->effect_id[0] == 0x17)
        return;
    if ((spellId == 0x29A || spellId == 0x2A2) && this->level < 0x2E)
    {
        if (this->player == 0)
            return;
        if (this->player->type != 0)
            return;
        HBS_TOKEN_MSG(this, "You cannot pass through the portal.", 0x121);
        return;
    }
    if (EQ_CDECL(int, 0x004A331F, (int, u8*, int))(spellIndex, spellInfo, 1) == 0)
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0xCB, 0, 0);
    if (this->player->type == 1 && caster->type == 0
        && EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))(this->player, caster->y, caster->x, caster->z) == 0)
    {
        HBS_TOKEN_MSG(caster->char_info_guess, "You cannot see your target.", 0x121);
        return;
    }
    if (spell->target_type != 0xD || caster != this->player)
    {
        if (spell->good_effect == 1 && caster->type == 0 && this->player->type == 0
            && (spell->effect_id[0] == 0xC || spell->effect_id[0] == 0x1C || spell->effect_id[0] == 0x1D
                || spellId == 0x628
                || (EQ_CDECL(char, 0x0048A11F, (HitSpellRecord_guess*, int))(spell, 0x1B) != 0 && caster->gm == 0))
            && EQ_METHOD(1, int, 0x0047EFCE, (void*, char*), this)(caster->name) == 0)
            return;
        if (EQ_METHOD(2, char, 0x004B2561, (EQPlayer*, EQPlayer*, int), caster)(this->player, spellIndex) == 0)
            return;
        if (spell->good_effect == 0)
        {
            if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && HBS_SOUND_MANAGER != 0)
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int), HBS_SOUND_MANAGER)(0x11);
        }
        else
        {
            if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && HBS_SOUND_MANAGER != 0)
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int), HBS_SOUND_MANAGER)(0x10);
        }
    }
    if (spell->good_effect == 0)
    {
        HBS_REMOVE_AFFECT(HBS_GET_AFFECT(0xC));
        HBS_REMOVE_AFFECT(HBS_GET_AFFECT(0x1C));
        HBS_REMOVE_AFFECT(HBS_GET_AFFECT(0x1D));
        this->player->sneaking = 0;
        this->player->actor_info->unk_0x18A = 0;
        this->player->invis = 0;
    }
    player = this->player;
    EQ_METHOD(1, int, 0x004199C0, (void*, int), this)(player->type);
    if (spell->push_back != 0.0f || spell->push_up != 0.0f)
    {
        current = this->player;
        current->dy = t3dFloatFastCosine(hitData->heading) * spell->push_back + current->dy;
        current = this->player;
        current->dx = t3dFloatFastSine(hitData->heading) * spell->push_back + current->dx;
        this->player->dz = spell->push_up + this->player->dz;
        if (this->player == g_local_player_guess && g_local_player_guess != g_control_player_guess)
            EQ_GLOBAL(u8, 0x006B710F) = 1;
        else if (this->player == g_control_player_guess)
            EQ_GLOBAL(u8, 0x006B710E) = 1;
    }
    duration = EQ_METHOD(3, u32, 0x00424591, (void*, HitSpellRecord_guess*, u8, char), this)(spell, casterLevel, 0);
    if (spellId == 0x598)
    {
        affect = (HitSpellBuff_guess*)this->buffs;
        buffsLeft = 15;
        do
        {
            if (affect->visible != 0)
            {
                bonus = affect->spell_id;
                if (bonus >= 0 && bonus <= 2999 && bonus != 0x598 && bonus != 0x561 && bonus != 0x63E
                    && bonus != 0x63F && bonus != 0x6AE)
                {
                    other = HBS_SPELL(bonus);
                    if (other != 0 && other->bard_level > 0x3C && other->good_effect == 1
                        && (other->effect_id[0] == 4 || other->effect_id[0] == 5))
                        HBS_REMOVE_AFFECT(affect);
                }
            }
            affect++;
            buffsLeft--;
        } while (buffsLeft != 0);
    }
    if (spellId == 0x5A7)
    {
        affect = (HitSpellBuff_guess*)this->buffs;
        buffsLeft = 15;
        do
        {
            if (affect->visible != 0)
            {
                bonus = affect->spell_id;
                if (bonus >= 0 && bonus <= 2999 && bonus != 0x5A7)
                {
                    other = HBS_SPELL(bonus);
                    if (other != 0 && other->bard_level > 0x3C && other->good_effect == 1
                        && (other->effect_id[0] == 1 || other->effect_id[1] == 0x45
                            || other->effect_id[2] == 0x45 || other->effect_id[3] == 1))
                        HBS_REMOVE_AFFECT(affect);
                }
            }
            affect++;
            buffsLeft--;
        } while (buffsLeft != 0);
    }
    if (spell->duration_formula == 0)
    {
        profile = 0;
        if (this->player->type == 0)
            profile = this;
        slot = 0;
        do
        {
            if (this->player == 0)
                return;
            slotIndex = slot;
            if (EQ_CDECL(int, 0x0042001B, (HitSpellRecord_guess*, int))(spell, slotIndex) != 0)
            {
                value = HBS_EFFECT_VALUE(spell, casterLevel, slot);
                if (spellId == 0x57 && caster->level > 0x28)
                {
                    bonus = (caster->level - 0x28) * 0x2C;
                    if (bonus < 0)
                        bonus = 0;
                    value += bonus;
                }
                if (spellId == 0x58 && caster->level > 0x28)
                {
                    bonus = (caster->level - 0x28) * 20;
                    if (bonus < 0)
                        bonus = 0;
                    if (bonus > 400)
                        bonus = 400;
                    actor = caster->actor_info;
                    value -= bonus;
                    if (actor != 0 && actor->unknown0x2DC == 0x1A)
                    {
                        value = value * 6 / 4;
                        actor->unknown0x2DC = 0;
                    }
                }
                if (spellId == 0x2E6 && caster->level > 0x2D)
                {
                    bonus = (caster->level - 0x2D) * 10;
                    if (bonus < 0)
                        bonus = 0;
                    if (bonus > 200)
                        bonus = 200;
                    value -= bonus;
                    if (caster->actor_info != 0 && caster->actor_info->unknown0x2DC == 0x1D)
                        value = value * 6 / 5;
                }
                if ((spell->target_type == 0x14 || spell->target_type == 0xD) && caster == this->player)
                {
                    if (spell->effect_id[slotIndex] == 0)
                    {
                        value = hitData->amount;
                        if (hitData->amount & 0x8000)
                            value |= 0xFFFF0000;
                    }
                    else
                        value = -(int)hitData->amount;
                }
                else if (EQ_METHOD(2, int, 0x0041F0C0, (void*, HitSpellRecord_guess*, int), this)(spell, caster->class_) != 0
                         && spell->effect_id[slotIndex] != 0x1B
                         && (modifier = hitData->bard_modifier) > 0 && modifier < 0x1F)
                {
                    value = (short)(modifier * 0.1f * value);
                }
                current = this->player;
                if (current == 0)
                    return;
                effect = &spell->effect_id[slotIndex];
                switch (*effect)
                {
                case 0x18:
                    if (current->type == 0)
                        EQ_METHOD(1, void, 0x00480DFB, (void*, int), this)(this->fatigue + value);
                    break;
                case 0x14:
                    if (value < 0)
                    {
                        affect = HBS_GET_AFFECT(0x14);
                        if (affect != 0)
                        {
                            chance = (casterLevel - affect->level + 10) * 5;
                            threshold = 5;
                            if (chance >= 5)
                            {
                                threshold = 0x5F;
                                if (chance <= 0x5F)
                                    threshold = chance;
                            }
                            if (threshold >= number_percent_guess())
                                HBS_REMOVE_AFFECT(affect);
                        }
                    }
                    break;
                case 0x15:
                    break;
                case 0xF:
                    if (this->class_ == 8)
                        break;
                    if (current->type == 1)
                    {
                        if (this->level > 0x34 && this->level <= 0x36)
                            value /= 2;
                        if (this->level > 0x36)
                        {
                            value /= 3;
                            if (value < -0x69)
                                value = -0x69;
                        }
                    }
                    this->mana += value;
                    EQ_METHOD(0, void, 0x00419BA3, (void*), this)();
                    break;
                case 0x19:
                    if (current->type != 0)
                        return;
                    if (profile == 0)
                        return;
                    if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(this->zone, "timo", 4) != 0)
                    {
                        if (caster == this->player || caster->gm != 0)
                        {
                            if (HBS_ZONE_TYPE == 1)
                                category = 0;
                            else
                            {
                                category = EQ_METHOD(1, int, 0x004BFB2E, (void*, int), HBS_ZONE_LIST)(
                                    EQ_METHOD(1, int, 0x004C05AE, (void*, char*), HBS_ZONE_LIST)(this->zone));
                                if (g_pvpRuleset_guess == 4)
                                    category = 1;
                            }
                            if (caster->gm != 0 || category == 0 || HBS_ZONE_TYPE == 2 || HBS_ZONE_TYPE == 3
                                || HBS_ZONE_TYPE == 4
                                || EQ_METHOD(1, int, 0x00420274, (void*, EQPlayer*), this)(player) != 0)
                            {
                                HBS_STRCPY(profile->bind_zone, this->zone);
                                profile->bind_location[0][1] = player->y;
                                profile->bind_location[1][1] = player->x;
                                profile->bind_location[2][1] = player->z;
                                EQ_GLOBAL(int, 0x006B7120) = 7;
                                EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), HBS_EVERQUEST)(0, 1);
                                break;
                            }
                        }
                        else if (HBS_ZONE_TYPE == 2 || HBS_ZONE_TYPE == 3 || HBS_ZONE_TYPE == 4
                                 || EQ_METHOD(1, int, 0x00420274, (void*, EQPlayer*), this)(player) != 0)
                        {
                            HBS_STRCPY(profile->bind_zone, this->zone);
                            profile->bind_location[0][1] = player->y;
                            profile->bind_location[1][1] = player->x;
                            profile->bind_location[2][1] = player->z;
                            return;
                        }
                    }
                    HBS_TOKEN_MSG(this, "You cannot seem to develop an affinity for this place. Try a city.", 0x121);
                    return;
                case 0x1A:
                    if (current->type != 0)
                        return;
                    if (profile == 0)
                        return;
                    if (random_range_guess(1, 100) < 0x62)
                    {
                        if (spellId != 0x58E || EQ_METHOD(1, int, 0x0047EFCE, (void*, char*), profile)(caster->name) != 0)
                        {
                            player->actor_info->ride_target_guess = 0;
                            profile->boat[0] = 0;
                            this->y = player->y = profile->bind_location[0][1];
                            this->x = player->x = profile->bind_location[1][1];
                            this->z = player->z = profile->bind_location[2][1];
                            HBS_PLAY_SOUND(player);
                            if (HBS_STRCMP(this->zone, profile->bind_zone) != 0)
                                HBS_ZONE_PC(profile->bind_zone, "gate spell");
                            else
                            {
                                floor = HBS_FLOOR(player);
                                if (floor != HBS_NO_FLOOR)
                                {
                                    player->actor_info->floor_z = floor;
                                    player->z = floor + player->height_offset;
                                }
                                HBS_UPDATE_ACTOR(player);
                            }
                        }
                    }
                    else
                        HBS_TOKEN_MSG(this, "Your gate is too unstable, and collapses.", 0x121);
                    break;
                case 0x1B:
                    break;
                case 0x21:
                    break;
                case 0x23:
                case 0x24:
                    if (value < 0)
                    {
                        affect = HBS_GET_AFFECT(*effect);
                        if (affect != 0)
                        {
                            other = HBS_SPELL(affect->spell_id);
                            if (other != 0)
                            {
                                if ((short)(HBS_EFFECT_VALUE(other, affect->level, slot) + value) < 7)
                                {
                                    affect->counters += value;
                                    if (affect->counters < 1)
                                        HBS_REMOVE_AFFECT(affect);
                                }
                            }
                            else
                                EQTRACE_guess("temp_spell is NULL!!\n");
                        }
                    }
                    break;
                case 0x34:
                    if (bodyType1 == -1)
                    {
                        bodyType1 = 3;
                        bodyType2 = 8;
                        HBS_STRCPY(message, "You sense undead in this direction.");
                    }
                case 0x36:
                    if (bodyType1 == -1)
                    {
                        bodyType1 = 0x15;
                        HBS_STRCPY(message, "You sense an animal in this direction.");
                    }
                case 0x35:
                    if (bodyType1 == -1)
                    {
                        bodyType1 = 0x1B;
                        bodyType2 = 8;
                        bodyType3 = 0x1C;
                        HBS_STRCPY(message, "You sense a summoned being in this direction.");
                    }
                    found = EQ_METHOD(4, EQPlayer*, 0x004228E2, (void*, int, int, int, int), this)(
                        1, bodyType1, bodyType2, bodyType3);
                    if (found != 0)
                    {
                        HBS_TARGET = found;
                        this->player->heading = HBS_BEARING(this->player->y, this->player->x, found->y, found->x);
                        HBS_DSP_CHAT(message, 0x108);
                    }
                    else
                        HBS_DSP_CHAT("You don't sense anything.", 0x121);
                    break;
                case 0x33:
                    for (trap = g_switchList_guess; trap != 0; trap = trap->next)
                    {
                        if (EQ_METHOD(0, short, 0x004BDEE6, (EQSwitch*), trap)() != 0)
                            break;
                    }
                    if (trap != 0)
                    {
                        this->player->heading = HBS_BEARING(this->player->y, this->player->x, trap->pos.y, trap->pos.x);
                        HBS_DSP_CHAT("You sense a trap in this direction.", 0x10E);
                    }
                    else
                        HBS_DSP_CHAT("You don't sense any traps.", 0x10E);
                    break;
                case 0x29:
                    break;
                case 0x2A:
                    teleportZ = current->z;
                    tries = 0;
                    do
                    {
                        distance = (float)random_range_guess(4, (int)spell->range + 5);
                        angle = (float)random_range_guess(0, 0x1FF);
                        current = this->player;
                        newY = t3dFloatFastCosine(angle) * distance + current->y;
                        newX = t3dFloatFastSine(angle) * distance + current->x;
                        ok = EQ_METHOD(6, char, 0x00410997, (void*, EQPlayer*, float, float, float, float, float),
                            g_display_guess)(current, newY, newX, teleportZ, angle, distance);
                    } while (ok == 0 && ++tries <= 10000);
                    if (ok != 0)
                    {
                        HBS_PLAY_SOUND(player);
                        this->y = this->player->y = newY;
                        this->x = this->player->x = newX;
                        this->z = this->player->z = teleportZ;
                        floor = HBS_FLOOR(this->player);
                        if (floor != HBS_NO_FLOOR)
                        {
                            this->player->actor_info->floor_z = floor;
                            this->player->z = floor + this->player->height_offset;
                        }
                        HBS_UPDATE_ACTOR(this->player);
                    }
                    else
                        HBS_DSP_CHAT("Nothing happens.", 0x111);
                    break;
                case 0x26:
                    HBS_STRCPY(message, "You are not holding an item!");
                    if (this->inventory_items[0] != 0)
                    {
                        HBS_STRCPY(message, "The item you are holding does not appear to be magical.");
                        item = this->inventory_items[0];
                        if (item->type == 0 && (HBS_STATS(item)->magic & 1) != 0)
                            HBS_STRCPY(message, "The item you are holding is magical.");
                    }
                    HBS_PRINT_CHAT(message);
                    break;
                case 0x38:
                    current->heading = 0.0f;
                    this->player->delta_heading = 0.0f;
                    break;
                case 0x48:
                    if (HBS_TARGET == 0 || HBS_TARGET->type < 2)
                    {
                        HBS_DSP_CHAT("Nothing happens.", 0x111);
                        break;
                    }
                    send_message_guess(g_world_connection_guess, 0x2106, &HBS_TARGET->spawn_id, 4, 1);
                    HBS_SPRINTF(message, "%s looks as if it's been freshened up a bit.", HBS_TARGET->name);
                    HBS_DSP_CHAT(message, 0x111);
                    break;
                case 0x47:
                    break;
                case 0x44:
                    if (HBS_TARGET != 0 && HBS_TARGET->type == 1 && HBS_TARGET->pet_owner_id == current->spawn_id
                        && (HBS_TARGET->body_type == 0x1B || HBS_TARGET->body_type == 8 || HBS_TARGET->body_type == 0x1C))
                        this->mana += (int)(HBS_TARGET->cur_hp * 0.1f);
                    break;
                case 0x3E:
                    HBS_STRCPY(message, "You are not holding an item!");
                    if (this->inventory_items[0] == 0)
                    {
                        HBS_PRINT_CHAT(message);
                        break;
                    }
                    HBS_PRINT_CHAT(HBS_DASHES);
                    item = this->inventory_items[0];
                    if (item->type == 0)
                    {
                        stats = HBS_STATS(item);
                        HBS_PRINT_CHAT(item->name);
                        HBS_PRINT_CHAT(HBS_DASHES);
                        skill = EQ_METHOD(1, u8, 0x004B422A, (EQPlayer*, u8), g_local_player_guess)(stats->skill);
                        if (skill != 0x1C)
                        {
                            HBS_SPRINTF(message, EQ_ARRAY(char, 0x00541B64), g_skill_records_guess[skill]);
                            HBS_PRINT_CHAT(message);
                        }
                        if (stats->damage > 0)
                        {
                            if (stats->ac != 0)
                                HBS_SPRINTF(message, EQ_ARRAY(char, 0x00541B50), (int)stats->damage, (int)stats->ac);
                            else
                                HBS_SPRINTF(message, EQ_ARRAY(char, 0x00541B48), (int)stats->damage);
                        }
                        else
                            HBS_SPRINTF(message, EQ_ARRAY(char, 0x00541B40), (int)stats->ac);
                        HBS_PRINT_CHAT(message);
                        statCount = 0;
                        HBS_STRCPY(message, HBS_EMPTY);
                        HBS_STAT(str, EQ_ARRAY(char, 0x00541B30), EQ_ARRAY(char, 0x00541B24))
                        HBS_STAT(dex, EQ_ARRAY(char, 0x00541B14), EQ_ARRAY(char, 0x00541B08))
                        HBS_STAT(sta, EQ_ARRAY(char, 0x00541AF8), EQ_ARRAY(char, 0x00541AEC))
                        HBS_STAT_FLUSH()
                        HBS_STAT(cha, EQ_ARRAY(char, 0x00541ADC), EQ_ARRAY(char, 0x00541AD0))
                        HBS_STAT_FLUSH()
                        HBS_STAT(wis, EQ_ARRAY(char, 0x00541AC0), EQ_ARRAY(char, 0x00541AB4))
                        HBS_STAT_FLUSH()
                        HBS_STAT(intelligence, EQ_ARRAY(char, 0x00541AA4), EQ_ARRAY(char, 0x00541A98))
                        HBS_STAT_FLUSH()
                        HBS_STAT(agi, EQ_ARRAY(char, 0x00541A88), EQ_ARRAY(char, 0x00541A7C))
                        HBS_STAT_FLUSH()
                        HBS_STAT(hp, EQ_ARRAY(char, 0x00541A70), EQ_ARRAY(char, 0x00541A64))
                        HBS_STAT_FLUSH()
                        HBS_STAT(mana, EQ_ARRAY(char, 0x00541A54), EQ_ARRAY(char, 0x00541A44))
                        HBS_STAT_FLUSH()
                        HBS_STAT(sv_fire, EQ_ARRAY(char, 0x00541A30), EQ_ARRAY(char, 0x00541A20))
                        HBS_STAT_FLUSH()
                        HBS_STAT(sv_disease, EQ_ARRAY(char, 0x00541A0C), EQ_ARRAY(char, 0x005419F8))
                        HBS_STAT_FLUSH()
                        HBS_STAT(sv_cold, EQ_ARRAY(char, 0x005419E4), EQ_ARRAY(char, 0x005419D4))
                        HBS_STAT_FLUSH()
                        HBS_STAT(sv_magic, EQ_ARRAY(char, 0x005419C0), EQ_ARRAY(char, 0x005419AC))
                        HBS_STAT_FLUSH()
                        HBS_STAT(sv_poison, EQ_ARRAY(char, 0x00541998), EQ_ARRAY(char, 0x00541984))
                        if (HBS_STRLEN(message) > 0)
                        {
                            HBS_PRINT_CHAT(message);
                            HBS_STRCPY(message, HBS_EMPTY);
                        }
                    }
                    HBS_PRINT_CHAT(HBS_DASHES);
                    break;
                case 0x3D:
                    HBS_STRCPY(message, "You are not holding an item!");
                    item = this->inventory_items[0];
                    if (item != 0)
                    {
                        lore = item->lore;
                        if (item->lore[0] == '~' || item->lore[0] == '*' || item->lore[0] == '#'
                            || item->lore[0] == '&' || item->lore[0] == '!')
                        {
                            lore = item->lore + 1;
                            if (item->lore[1] == '~' || item->lore[1] == '*' || item->lore[1] == '#'
                                || item->lore[1] == '&' || item->lore[0] == '!')
                                lore = item->lore + 2;
                        }
                        HBS_SPRINTF(message, "Item Lore: %s.", lore);
                        if (this->inventory_items[0]->type == 2)
                        {
                            HBS_SPRINTF(languageText, "This item appears to be written in %s.",
                                EQ_METHOD(1, char*, 0x004C8FCE, (void*, short), HBS_EVERQUEST)(
                                    (short)HBS_STATS(this->inventory_items[0])->cha));
                            HBS_PRINT_CHAT(languageText);
                        }
                    }
                    HBS_DSP_CHAT(message, 0x108);
                    break;
                case 0x3C:
                    if (HBS_TARGET == 0)
                    {
                        HBS_DSP_CHAT("Nothing happens.", 0x111);
                        break;
                    }
                    EQ_METHOD(1, void, 0x004CF46A, (void*, char*), HBS_EVERQUEST)(HBS_TARGET->name);
                    break;
                case 0x4A:
                    EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), current)(0x73);
                    break;
                case 0x53:
                case 0x58:
                    roll = random_range_guess(1, 100);
                    if (*effect == 'S')
                        roll = 0;
                    if (roll == 1)
                        break;
                    if (this->player->type != 0)
                        return;
                    if (profile == 0)
                        return;
                    zone = spell->teleport_zone;
                    if (HBS_STRLEN(zone) > 0)
                    {
                        player->actor_info->ride_target_guess = 0;
                        profile->boat[0] = 0;
                        this->y = player->y = (float)spell->base[0];
                        this->x = player->x = (float)spell->base[1];
                        this->z = player->z = (float)spell->base[2];
                        this->heading = player->heading = (float)spell->base[3];
                        HBS_STRCPY(zoneName, zone);
                        if ((spellId == 0x61E || spellId == 0x61F || spellId == 0x65A || spellId == 0x65B
                                || spellId == 0x65C)
                            && EQ_CDECL(int, 0x00524240, (const char*, const char*))(zoneName, "same") == 0)
                        {
                            HBS_STRCPY(zoneName, g_pc_guess->zone);
                            player->y = HBS_SAFE_Y;
                            this->y = HBS_SAFE_Y;
                            player->x = HBS_SAFE_X;
                            this->x = HBS_SAFE_X;
                            player->z = HBS_SAFE_Z;
                            this->z = HBS_SAFE_Z;
                            player->heading = 0.0f;
                            this->heading = 0.0f;
                            HBS_ZONE_PC(zoneName, "portal spell");
                        }
                        HBS_PLAY_SOUND(player);
                        if (HBS_STRCMP(this->zone, zoneName) != 0)
                            HBS_ZONE_PC(zoneName, "portal spell");
                        else
                        {
                            if (spellId != 0x5C4)
                            {
                                floor = HBS_FLOOR(player);
                                if (floor != HBS_NO_FLOOR)
                                {
                                    player->actor_info->floor_z = floor;
                                    player->z = floor + player->height_offset;
                                }
                            }
                            HBS_UPDATE_ACTOR(player);
                        }
                    }
                    break;
                case 0x51:
                    if (current->type != 0)
                        return;
                    if (profile == 0)
                        return;
                    if (HBS_TARGET != 0 && HBS_TARGET->type >= 2)
                    {
                        corpseMsg.corpse_id = HBS_TARGET->spawn_id;
                        HBS_STRCPY(corpseMsg.zone, this->zone);
                        corpseMsg.y = HBS_TARGET->y;
                        corpseMsg.x = HBS_TARGET->x;
                        corpseMsg.z = HBS_TARGET->z;
                        HBS_STRCPY(corpseMsg.caster_name, this->name);
                        corpseMsg.amount = value;
                        corpseMsg.spell_id = spellId;
                        HBS_STRCPY(corpseMsg.corpse_name, HBS_TARGET->name);
                        corpseMsg.flag = 0;
                        corpseMsg.unknown0x9C = 0;
                        corpseMsg.unknown0x54 = 0;
                        send_message_guess(g_world_connection_guess, 0x212A, &corpseMsg, 0xA0, 1);
                    }
                    break;
                case 0x4D:
                    EQ_METHOD(0, void, 0x00420184, (void*), this)();
                    break;
                case 0x5B:
                case 0x6A:
                case 0x6B:
                case 0x6C:
                    break;
                default:
                    EQ_CDECL(int, 0x005229E8, (const char*, ...))("Unknown instant affect %d for %s\n", (int)*effect, spell);
                    break;
                }
            }
        } while (++slot < 12);
        return;
    }
    else
    {
        EQ_METHOD(1, u8, 0x00423DAD, (void*, short), this)(spellId);
        buff = EQ_METHOD(4, HitSpellBuff_guess*, 0x00422DD6, (void*, short, EQPlayer*, int*, int), this)(
            spellId, caster, &stackSlot, 0);
        if (buff == 0)
            return;
        buff->counters = 0;
        scale = 1.0f;
        if (this->player != 0 && (actor = this->player->actor_info) != 0)
            HBS_ACTOR_CASTERS(actor)[stackSlot] = (s16)caster->spawn_id;
        if ((spell->target_type == 0xD && caster == this->player)
            || (spell->target_type == 0x14
                && (caster == this->player || EQ_METHOD(1, int, 0x0047EFCE, (void*, char*), this)(caster->name) != 0)))
            buff->visible = 4;
        else
            buff->visible = 2;
        slot = 0;
        buff->spell_id = spellId;
        buff->level = casterLevel;
        buff->duration = (short)duration;
        buff->bard_modifier = 10;
        buff->activated = spell->activated;
        do
        {
            if (this->player == 0)
                return;
            slotIndex = slot;
            if (EQ_CDECL(int, 0x0042001B, (HitSpellRecord_guess*, int))(spell, slotIndex) != 0)
            {
                value = HBS_EFFECT_VALUE(spell, casterLevel, slot);
                if (caster->class_ == 8 && spell->effect_id[slotIndex] != 0x3A
                    && (modifier = hitData->bard_modifier) > 0 && modifier < 0x1F)
                {
                    scale = modifier * 0.1f;
                    value = (short)(value * scale);
                }
                effect = &spell->effect_id[slotIndex];
                switch (*effect)
                {
                case 0x17:
                    EQ_METHOD(2, void, 0x004B3FB0, (EQPlayer*, int, u32), this->player)(2, caster->spawn_id);
                    break;
                case 0x16:
                    if (this->player == caster)
                        return;
                    EQ_METHOD(2, void, 0x004B3FB0, (EQPlayer*, int, u32), this->player)(1, caster->spawn_id);
                    break;
                case 0xD:
                    EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_display_guess)(this->player);
                    break;
                case 0x3:
                    if (HBS_TOTAL_AFFECTS(99) == 0)
                    {
                        if (scale > 1.0f)
                            buff->bard_modifier = (u8)hitData->bard_modifier;
                        EQ_METHOD(1, void, 0x00420046, (void*, int), this)(HBS_TOTAL_AFFECTS(3));
                    }
                    break;
                case 0xC:
                case 0x1C:
                case 0x1D:
                    EQ_METHOD(0, void, 0x004255CE, (void*), this)();
                    break;
                case 0x39:
                    EQ_METHOD(0, void, 0x0042010B, (void*), this)();
                    break;
                case 0x23:
                case 0x24:
                    if (value > 0)
                    {
                        buff->counters = (short)value;
                        break;
                    }
                    if (value < 0 && random_range_guess(1, 100) <= 10)
                    {
                        affect = HBS_GET_AFFECT(*effect);
                        if (affect != 0)
                        {
                            other = HBS_SPELL(affect->spell_id);
                            if (other != 0)
                            {
                                result = HBS_EFFECT_VALUE(other, affect->level, slot);
                                if (result > 0 && (short)(result + value) < 7)
                                {
                                    affect->counters += value;
                                    if (affect->counters < 1)
                                    {
                                        HBS_REMOVE_AFFECT(affect);
                                        if (this->player != 0 && this->player->type == 0)
                                            HBS_TOKEN_MSG(this, "Your affliction has been cured by a magical enchantment", 0x111);
                                    }
                                }
                            }
                            else
                                EQTRACE_guess("temp_spell is NULL!!\n");
                        }
                    }
                    break;
                case 0x14:
                case 0x22:
                    this->player->actor_info->unknown0x189 = 1;
                    break;
                case 0x1F:
                    if (g_offlineMode_guess != 0 || DAT_006b6494 != 0)
                    {
                        stunTime = (short)duration * 6000;
                        HBS_MEZ_MESSAGE = 0;
                        if (spellId == 0x69C)
                            HBS_MEZ_MESSAGE = 1;
                        else if (spellId == 0x69B)
                            HBS_MEZ_MESSAGE = 2;
                        if (EQ_CDECL(int, 0x004246B5, (void*))(this) == 0 && this->unknown0x0037 != 0x1D
                            && this->unknown0x0037 != 4 && this->unknown0x0037 != 0x1A
                            && this->unknown0x0037 != 0x1E && this->unknown0x0037 != 9)
                            EQ_METHOD(1, void, 0x004246DD, (void*, int), this)(stunTime);
                        else if (caster->type == 0 && caster->char_info_guess != 0)
                            HBS_TOKEN_MSG(caster->char_info_guess,
                                HBS_MEZ_MESSAGE == 1 ? "Your target cannot be mesmerized."
                                                     : "Your target cannot be mesmerized (with this spell).", 0x121);
                        HBS_MEZ_MESSAGE = 0;
                    }
                    break;
                case 0x3A:
                    actorTag = 0;
                    if (this->pk == 1)
                        break;
                    HBS_STRCPY(illusion.name, this->name);
                    HBS_STRCPY(illusion.name2, this->name);
                    if (value < 0xD || value == 0x46 || value == 0x80 || value == 0x82)
                    {
                        if (spell->impact_duration > 0)
                        {
                            if (spell->impact_duration == 4)
                            {
                                if (this->gender == 0)
                                    illusion.gender = 1;
                                else if (this->gender == 1)
                                    illusion.gender = 0;
                            }
                            else
                                illusion.gender = (u8)spell->impact_duration - 1;
                        }
                        else
                            illusion.gender = this->gender;
                    }
                    else
                        illusion.gender = 2;
                    illusion.race = (short)value;
                    illusion.texture = (short)spell->aoe_range;
                    if (spellId == 0x597)
                    {
                        illusion.texture = 0;
                        if (this->race == 9 || this->race == 0x80)
                            illusion.texture = 1;
                        else if (this->race == 2)
                            illusion.texture = 2;
                    }
                    illusion.unknown0x44 = -1;
                    illusion.unknown0x46 = -1;
                    if ((short)value == 0x8F)
                        actorTag = 8;
                    else if ((short)value == 0x8E)
                        actorTag = 7;
                    if (illusion.texture == 0
                        && EQ_METHOD(1, int, 0x004B4531, (EQPlayer*, int), this->player)((short)value) != 0)
                        illusion.texture = -1;
                    if (actorTag != 0
                        && EQ_METHOD(3, char, 0x0040C17D, (void*, char*, int, EQPlayer*), g_display_guess)(
                            zoneName, actorTag, this->player) != 1)
                        goto clearBuff;
                    break;
                case 0x49:
                    if (HBS_TARGET == 0)
                    {
                        HBS_TOKEN_MSG(this, EQ_ARRAY(char, 0x00541490), 0x121);
                        goto clearBuff;
                    }
                    EQ_METHOD(1, void, 0x00413ED6, (void*, char*), g_display_guess)(HBS_TARGET->name);
                    break;
                case 0x42:
                    this->innateSkills_guess[13] = 0;
                    break;
                case 0x41:
                    this->innateSkills_guess[6] = 0;
                    break;
                case 0x40:
                    if (this->player->stand_state == 100)
                    {
                        EQTRACE_guess(EQ_ARRAY(char, 0x00541810), (int)(short)duration, 0x1D4C);
                        this->player->delta_heading = 32.0f;
                    }
                    break;
                case 0x43:
                    break;
                case 0x4D:
                    EQ_METHOD(0, void, 0x00420184, (void*), this)();
                    break;
                default:
                    if (scale > 1.0f)
                        buff->bard_modifier = (u8)hitData->bard_modifier;
                    break;
                case 0x63:
                    EQ_METHOD(1, void, 0x00420046, (void*, int), this)(value);
                    break;
                case 0x57:
                    EQ_METHOD(1, void, 0x0040E4C6, (void*, int), g_display_guess)(value);
                    break;
                case 0x37:
                case 0x4E:
                    buff->counters = (short)value;
                    break;
                case 0x6A:
                case 0x6C:
                    break;
                }
            }
        } while (++slot < 12);
        window = g_eqmain_guess;
        if (window == 0)
            return;
        if (HBS_REFRESH_LOCK != 0)
            return;
        HBS_REFRESH_LOCK = 1;
        if (window[0xFE0] < 0)
            return;
        if (window[0xFE0] > 3)
            return;
        ((HitWindowMethod_guess)(*(u32**)window)[2])(window);
        window = g_eqmain_guess;
        ((HitWindowMethod_guess)(*(u32**)window)[1])(window);
        return;
clearBuff:
        buff->visible = 0;
        buff->spell_id = -1;
        buff->level = 0;
        buff->duration = 0;
        buff->bard_modifier = 0;
        buff->counters = 0;
        return;
    }
}

#undef HBS_EVERQUEST
#undef HBS_TARGET
#undef HBS_SOUND_MANAGER
#undef HBS_ZONE_LIST
#undef HBS_ZONE_TYPE
#undef HBS_SAFE_Y
#undef HBS_SAFE_X
#undef HBS_SAFE_Z
#undef HBS_MEZ_MESSAGE
#undef HBS_REFRESH_LOCK
#undef HBS_NO_FLOOR
#undef HBS_EMPTY
#undef HBS_DASHES
#undef HBS_SPELL
#undef HBS_STATS
#undef HBS_ACTOR_CASTERS
#undef HBS_STRCPY
#undef HBS_STRCAT
#undef HBS_STRLEN
#undef HBS_STRCMP
#undef HBS_SPRINTF
#undef HBS_PRINT_CHAT
#undef HBS_DSP_CHAT
#undef HBS_TOKEN_MSG
#undef HBS_ZONE_PC
#undef HBS_GET_AFFECT
#undef HBS_REMOVE_AFFECT
#undef HBS_TOTAL_AFFECTS
#undef HBS_EFFECT_VALUE
#undef HBS_BEARING
#undef HBS_PLAY_SOUND
#undef HBS_FLOOR
#undef HBS_UPDATE_ACTOR
#undef HBS_STAT
#undef HBS_STAT_FLUSH

// FUNCTION: TRILOGY 0x004226c1
// Applies active buffs' movement, levitation, view angle, and related character effects.
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
void EQ_Character::HandleSpecialPCAffects_guess(int mode)
{
    char effectSlot;
    int spell;
    char* buff;
    u32 affectValue;
    int remaining;

    spell = *(int*)((char*)this + 0xa0);
    if ((((spell != 0) && (*(char*)(spell + 0x98) == '\0')) && (*(int*)(spell + 0x70) != 0))
        && (EQ_GLOBAL(char, 0x006B63F8) == '\0'))
    {
        buff = (char*)this + 0x284;
        remaining = 0xf;
        do
        {
            if (*buff != '\0')
            {
                spell = (int)*(short*)(buff + 4);
                if ((spell < 0) || (2999 < spell))
                    spell = 0;
                else
                    spell = (int)EQ_ARRAY(void*, 0x005FE9A0)[spell];

                if (spell != 0)
                {
                    if (mode != 0
                        && (mode == 2
                            ? (EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x43)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x1f)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x3a)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x49)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x40))
                            : (EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x43)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x1f)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x3a)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x16)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x17)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x49)
                               || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x40))))
                    {
                        EQ_METHOD(1, void, 0x00424E22, (void*, char*), this)(buff);
                    }
                    else
                    {
                        effectSlot = EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 3);
                        if (effectSlot != '\0')
                        {
                            affectValue = (u32)EQ_METHOD(4, int, 0x0042351A, (void*, void*, u8, u8, int), this)((void*)spell, buff[1], (u8)(effectSlot - 1), 0);
                            EQ_METHOD(1, void, 0x00420046, (void*, u32), this)(affectValue);
                        }
                        effectSlot = EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 99);
                        if (effectSlot != '\0')
                        {
                            affectValue = (u32)EQ_METHOD(4, int, 0x0042351A, (void*, void*, u8, u8, int), this)((void*)spell, buff[1], (u8)(effectSlot - 1), 0);
                            EQ_METHOD(1, void, 0x00420046, (void*, u32), this)(affectValue);
                        }
                        if (EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x22) != '\0'
                            || EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x14) != '\0')
                            *(u8*)(*(int*)(*(int*)((char*)this + 0xa0) + 0x70) + 0x189) = 1;
                        effectSlot = EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x39);
                        if (effectSlot != '\0')
                            EQ_METHOD(0, void, 0x0042010B, (void*), this)();

                        effectSlot = EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x57);
                        if (effectSlot != '\0')
                        {
                            affectValue = (u32)EQ_METHOD(4, int, 0x0042351A, (void*, void*, u8, u8, int), this)((void*)spell, buff[1], (u8)(effectSlot - 1), 0);
                            EQ_METHOD(1, void, 0x0040E4C6, (void*, u32), g_display_guess)(affectValue);
                        }
                        effectSlot = EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x41);
                        if (effectSlot != '\0')
                            this->innateSkills_guess[6] = 0;

                        effectSlot = EQ_CDECL(char, 0x0048A11F, (void*, int))((void*)spell, 0x42);
                        if (effectSlot != '\0')
                            this->innateSkills_guess[13] = 0;
                    }
                }
            }
            buff = buff + 10;
            remaining = remaining + -1;
        } while (remaining != 0);

        affectValue = (u32)EQ_METHOD(1, int, 0x004238A2, (void*, int), this)(99);
        if (0 <= (short)affectValue)
            affectValue = (u32)EQ_METHOD(1, int, 0x004238A2, (void*, int), this)(3);
        EQ_METHOD(1, void, 0x00420046, (void*, u32), this)(affectValue);
    }
    return;
}

// FUNCTION: TRILOGY 0x00422d68
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (address range; TAKP: IsSpellCausingDeath)
// static (__cdecl). 1 for a fixed list of 14 spell ids (ProcessAffects).
int EQ_Character::IsSpellCausingDeath_guess(short spell)
{
    int id = spell;

    if (id == 0x158 || id == 0x163 || id == 0x1C4 || id == 0x1C5 || id == 0xF2 || id == 0x200 || id == 0x27C
        || id == 0x653 || id == 0x65F || id == 0x786 || id == 0x792 || id == 0x6E7 || id == 0x2E2 || id == 0x6D7)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x0042351a
// @early-stop: call setup, several operand widths, and return-value assembly still differ.
// This fastcall helper has the original thiscall ABI: character in ECX, the unused EDX slot, and four stack arguments.
int __fastcall CalcAffectChange(EQ_Character* character, int unused_edx, EQ_Spell_guess* spell, u32 casterLevel, u8 effectIndex, int durationData)
{
    u32 value = 0;
    short* basePart = &spell->base[effectIndex];
    int effectAddress;
    int spellBase = EQ_CDECL(int, 0x005232FF, (int))((int)*basePart);
    effectAddress = (int)spell + effectIndex;
    char formula = *(char*)(effectAddress + 0x212);
    union
    {
        int value;
        u16 words[2];
    } effectType;
    union
    {
        u32 value;
        u16 words[2];
    } durationResult;
    int adjustedAmount;
    effectType.value = (int)formula + -100;
    switch (effectType.value)
    {
    case 0:
        value = 0;
        break;
    case 1:
        value = (u16)((u8)casterLevel >> 1);
        break;
    case 2:
        value = (u8)casterLevel;
        break;
    case 3:
        value = (u8)casterLevel;
        value <<= 1;
        break;
    case 4:
        value = (u8)casterLevel * 3;
        break;
    case 5:
        value = (u8)casterLevel;
        value <<= 2;
        break;
    case 7:
        if (spell->duration_formula == 0 || durationData == 0)
            break;
        durationResult.value = EQ_METHOD(3, u32, 0x00424591, (EQ_Character*, EQ_Spell_guess*, u8, char), character)(spell, (u8)casterLevel, 0);
        durationResult.words[0] -= *(short*)(durationData + 6);
        value = durationResult.value;
        if ((short)spellBase >= 0)
            value = -value;
        break;
    case 8:
        if (spell->duration_formula == 0 || durationData == 0)
            break;
        durationResult.value = EQ_METHOD(3, u32, 0x00424591, (EQ_Character*, EQ_Spell_guess*, u8, char), character)(spell, (u8)casterLevel, 0);
        durationResult.words[0] -= *(short*)(durationData + 6);
        value = durationResult.value;
        value <<= 1;
        if ((short)spellBase >= 0)
            value = -value;
        break;
    case 20:
        if (spell->duration_formula == 0 || durationData == 0)
            break;
        durationResult.value = EQ_METHOD(3, u32, 0x00424591, (EQ_Character*, EQ_Spell_guess*, u8, char), character)(spell, (u8)casterLevel, 0);
        durationResult.words[0] -= *(short*)(durationData + 6);
        value = durationResult.value;
        value *= 5;
        // as original: case 20 jumps into case 22's sign test, as the original's jmp to its shared cmp does
        goto lab_0042366c;
    case 22:
        if (spell->duration_formula == 0 || durationData == 0)
            break;
        durationResult.value = EQ_METHOD(3, u32, 0x00424591, (EQ_Character*, EQ_Spell_guess*, u8, char), character)(spell, (u8)casterLevel, 0);
        durationResult.words[0] -= *(short*)(durationData + 6);
        value = durationResult.value;
        value *= 0xc;
lab_0042366c:
        if ((short)spellBase >= 0)
            value = -value;
        break;
    case 9:
        value = (u8)casterLevel >> 2;
        break;
    case 10:
        value = (u8)casterLevel / 6;
        break;
    case 19:
        value = (u8)casterLevel >> 3;
        break;
    case 21:
        value = (u8)casterLevel / 3;
        break;
    case 11:
        adjustedAmount = (u8)casterLevel - 0x10;
        if ((short)adjustedAmount <= 0)
            break;
        value = adjustedAmount * 3;
        value <<= 1;
        break;
    case 12:
        adjustedAmount = (u8)casterLevel - 0x18;
        if ((short)adjustedAmount > 0)
            value = adjustedAmount * 8;
        break;
    case 13:
        adjustedAmount = (u8)casterLevel - 0x22;
        if ((short)adjustedAmount <= 0)
            break;
        value = adjustedAmount * 5;
        value <<= 1;
        break;
    case 14:
        adjustedAmount = (u8)casterLevel - 0x2c;
        if ((short)adjustedAmount > 0)
            value = adjustedAmount * 0xf;
        break;
    case 15:
        adjustedAmount = (u8)casterLevel - 0x0f;
        if ((short)adjustedAmount > 0)
            value = adjustedAmount * 7;
        break;
    case 16:
        adjustedAmount = (u8)casterLevel - 0x18;
        if ((short)adjustedAmount <= 0)
            break;
        value = adjustedAmount * 5;
        value <<= 1;
        break;
    case 17:
        adjustedAmount = (u8)casterLevel - 0x22;
        if ((short)adjustedAmount > 0)
            value = adjustedAmount * 0xd;
        break;
    case 18:
        adjustedAmount = (u8)casterLevel - 0x2c;
        if ((short)adjustedAmount <= 0)
            break;
        value = adjustedAmount * 5;
        value <<= 2;
        break;
    default:
        effectType.words[0] = (u8)casterLevel;
        value = (u32)(u16)(short)formula * (u32)effectType.value;
        break;
    }

    if (*(char*)(effectAddress + 0x222) == 11 && (short)spellBase < 100)
        spellBase -= value;
    else
        spellBase += value;

    {
        short maximum = (&spell->maximumEffectValue_guess[0])[effectIndex];
        if (maximum != 0)
        {
            int absoluteMaximum = (int)EQ_CDECL(int, 0x005232FF, (int))((int)maximum);
            int resultAsShort = (short)spellBase;
            if (resultAsShort > absoluteMaximum)
                spellBase = absoluteMaximum;
        }
    }

    {
        short* returnPart = basePart;
        int finalValue = spellBase;
        if (*returnPart < 0 || *(char*)(effectAddress + 0x212) < 0)
        {
            finalValue = -spellBase;
            returnPart = (short*)finalValue;
        }
        return (int)(((u32)returnPart & 0xffff0000) | (u16)finalValue);
    }
}

// FUNCTION: TRILOGY 0x0042381b
// Finds the first active buff that grants a requested spell effect.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (Mac: EQ_Character::GetPCSpellAffect, low)
// __thiscall. The first buff whose spell has effect `effect` in one of its slots; *slot = its index (-1 and
// 0 returned when there is none). Buffs with a spell id outside 0..2999 or without a spell are skipped.
SpellBuff* EQ_Character::GetPCSpellAffect_guess(u8 effect, short* slot)
{
    short i;
    short j;

    if (slot != 0)
        *slot = -1;
    for (i = 0; i < 15; i++)
    {
        if (buffs[i].visible != 0 && buffs[i].spell_id <= 2999 && buffs[i].spell_id >= 0)
        {
            EQ_Spell_guess* spell = g_spells_guess[buffs[i].spell_id];
            if (spell != 0)
            {
                for (j = 0; j < 12; j++)
                {
                    u8 e = spell->effect_id[j];
                    if (e != EQ_SPELL_EFFECT_NONE && e == effect)
                    {
                        if (slot != 0)
                            *slot = i;
                        return &buffs[i];
                    }
                }
            }
        }
    }
    return 0;
}

/* The known profile has the player pointer at 0xA0 and buffs at 0x284.  This local view lets this
 * not-yet-declared member keep the client's thiscall layout without changing the shared headers. */
struct EQ_Character_MagicSystem_guess
{
    u8 unknown0x00[0xA0];
    EQPlayer* player;
    u8 unknown0x0A4[0x1E0];
    SpellBuff buffs[15];

    int TotalSpellAffects_guess(char effect);
    short GetBuffEffectLevel_guess(char effect, u32 spell);
};

// FUNCTION: TRILOGY 0x004238a2
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
// @early-stop: register allocation, stack-slot placement, and final result packing still differ (ratio 0.516, 267 ndiff).
/* fake match: the packed union and empty arm preserve VC6 argument and branch shape. */
int EQ_Character_MagicSystem_guess::TotalSpellAffects_guess(char effect)
{
    u8 charLevel;
    short temp16;
    char* effectIdPtr;
    int itemAffect;
    u16 highWord;
    u8* buffCursor;
    int entry;
    int scratch;
    int remaining;
    int temp1;
    int buffHasteMax;
    int itemHasteMax;
    int temp2;
    int buffMoveMax;
    int buffMoveMin;
    int temp3;
    int* wornSlotCursor;
    int itemSlowMin;
    int itemMoveMin;
    int itemMoveMax;
    int total;
    /* The byte index shares its stack word with the helper's dword argument. */
    union
    {
        u32 value;
        u8 index;
    } effectIndex;
    union
    {
        u32 value;
        u8 level;
    } packedEffectArgument;

    temp2 = 999;
    buffHasteMax = 0;
    temp1 = 0;
    buffMoveMin = 0;
    itemMoveMin = 0;
    buffMoveMax = 0;
    itemMoveMax = 0;
    temp3 = 0;
    itemHasteMax = 0;
    itemSlowMin = 999;
    buffCursor = (u8*)((char*)this + 0x285);
    remaining = 0xf;
    do
    {
        if (buffCursor[-1] != 0 && (temp16 = *(short*)(buffCursor + 3), temp16 < 3000) && temp16 > 0 &&
            (entry = (int)g_spells_guess[temp16], entry != 0))
        {
            effectIndex.value = effectIndex.value & 0xffffff00;
            effectIdPtr = (char*)(entry + 0x222);
            do
            {
                if (*effectIdPtr != -2)
                {
                    if (effect == '\v' && *effectIdPtr == 'b')
                    {
                        packedEffectArgument.value = (int)(buffCursor - 1);
                        packedEffectArgument.level = *buffCursor;
                        temp16 = EQ_METHOD(4, s16, 0x0042351A, (void*, EQ_Spell_guess*, int, u32, void*), this)((EQ_Spell_guess*)entry,
                            packedEffectArgument.value,
                             effectIndex.value, buffCursor - 1);
                        if (temp1 < temp16)
                            temp1 = (int)temp16;
                    }
                    if (*effectIdPtr == effect)
                    {
                        packedEffectArgument.value = (int)(buffCursor - 1);
                        packedEffectArgument.level = *buffCursor;
                        temp16 = EQ_METHOD(4, s16, 0x0042351A, (void*, EQ_Spell_guess*, int, u32, void*), this)((EQ_Spell_guess*)entry,
                             packedEffectArgument.value,
                             effectIndex.value, buffCursor - 1);
                        scratch = (int)temp16;
                        if (effect == '\v')
                        {
                            if (scratch >= 100)
                            {
                                if (buffHasteMax < scratch)
                                    buffHasteMax = scratch;
                            }
                            else if (scratch < temp2 && scratch > 0)
                                temp2 = scratch;
                        }
                        if (buffCursor[1] > 10)
                        {
                            float multiplier = (float)buffCursor[1] * EQ_GLOBAL(float, 0x005351C4);
                            temp16 = (short)(scratch * multiplier);
                            scratch = (int)temp16;
                            EQTRACE_guess(EQ_ARRAY(char, 0x00541DDC), (double)multiplier);
                        }
                        if (effect == '\003')
                        {
                            if (scratch < 0)
                            {
                                if (scratch < buffMoveMin)
                                    buffMoveMin = scratch;
                            }
                            else if (scratch > 0 && buffMoveMax < scratch)
                                buffMoveMax = scratch;
                        }
                        if (scratch < 0 &&
                            (((*effectIdPtr == '2' || *effectIdPtr == '1') || *effectIdPtr == '0' ||
                              *effectIdPtr == '.' || *effectIdPtr == '/') &&
                            (*(char*)(*(int*)((char*)this + 0xa0) + 0x98) == '\0' &&
                              g_pvpRuleset_guess != 0)))
                            scratch = scratch + scratch / 2;
                        if (buffCursor[-1] == 4 && effect != '\v')
                            total = total - scratch;
                        else
                            total = total + scratch;
                    }
                }
                ++effectIndex.index;
                effectIdPtr = effectIdPtr + 1;
            }
            while (effectIndex.index < 0xc);
        }
        buffCursor = buffCursor + 10;
        remaining = remaining - 1;
    }
    while (remaining != 0);

    wornSlotCursor = (int*)((char*)this + 0xe4);
    remaining = 0x15;
    do
    {
        entry = *wornSlotCursor;
        if (entry != 0 && *(char*)(entry + 0x81) == '\0' &&
            (temp16 = *(short*)(entry + 0xce), *(u8*)(entry + 0xd8) >= 2) &&
            (*(char*)(entry + 0xc0) == '\x02' && 0 < temp16 && temp16 < 3000 &&
             (scratch = (int)g_spells_guess[temp16], scratch != 0 &&
              *(int*)(scratch + 0x1bc) != 0)))
        {
            effectIndex.value = effectIndex.value & 0xffffff00;
            effectIdPtr = (char*)(scratch + 0x222);
            do
            {
                if (*effectIdPtr != -2 && *effectIdPtr == effect)
                {
                    packedEffectArgument.value = entry + 0xd8;
                    packedEffectArgument.level = *(u8*)(entry + 0xc4);
                    temp16 = EQ_METHOD(4, s16, 0x0042351A, (void*, EQ_Spell_guess*, int, u32, void*), this)((EQ_Spell_guess*)scratch,
                         packedEffectArgument.value,
                         effectIndex.value, (void*)(entry + 0xd8));
                    itemAffect = (int)temp16;
                    if (effect == '\x0f' && itemAffect <= temp3)
                    {
                    }
                    else
                    {
                        if (effect == '\x0f')
                            temp3 = itemAffect;
                        else
                        total = total + itemAffect;
                        if (effect == '\003')
                        {
                            if (itemAffect < 0)
                            {
                                if (itemAffect < itemMoveMin)
                                    itemMoveMin = itemAffect;
                            }
                            else if (itemAffect > 0 && itemMoveMax < itemAffect)
                                itemMoveMax = itemAffect;
                        }
                        else if (effect == '\v')
                        {
                            if (itemAffect >= 100)
                            {
                                if (*(u8*)((char*)this + 0x38) < 0x1a && 0x6e < itemAffect)
                                    itemAffect = 0x6e;
                                if (itemHasteMax < itemAffect)
                                    itemHasteMax = itemAffect;
                            }
                        else if (itemAffect < itemSlowMin && itemAffect > 0)
                                itemSlowMin = itemAffect;
                        }
                    }
                }
                ++effectIndex.index;
                effectIdPtr = effectIdPtr + 1;
            }
            while (effectIndex.index < 0xc);
        }
        wornSlotCursor = wornSlotCursor + 1;
        remaining = remaining - 1;
    }
    while (remaining != 0);

    if (effect == '\x0f')
        total = total + temp3;
    if (effect == '\003')
    {
        if (buffMoveMin == 0 && itemMoveMin == 0)
        {
            highWord = (u16)((u32)buffMoveMax >> 0x10);
            if (buffMoveMax < 1 && itemMoveMax < 1)
                total = (u32)highWord << 0x10;
            else
            {
                total = buffMoveMax;
                if (buffMoveMax < itemMoveMax)
                    total = ((u32)highWord << 0x10) | (u16)itemMoveMax;
            }
        }
        else
        {
            total = buffMoveMin;
            if (itemMoveMin <= buffMoveMin)
                total = ((u32)(u16)((u32)buffMoveMin >> 0x10) << 0x10) | (u16)itemMoveMin;
        }
    }
    else if (effect == '\v')
    {
        if (itemSlowMin < temp2 && itemSlowMin != 999)
            temp2 = itemSlowMin;
        total = temp2;
        if (100 < temp2)
        {
            charLevel = *(u8*)((char*)this + 0x38);
            total = itemHasteMax + buffHasteMax;
            entry = charLevel + 0x7d;
            if (99 < buffHasteMax && 99 < itemHasteMax)
                total = total - 100;
            if (100 < temp1 && 0x31 < charLevel)
            {
                if (0x6e < temp1)
                    temp1 = 0x6e;
                total = total - 100 + temp1;
            }
            if (0x32 < charLevel)
                entry = 0xb9;
            if (0x3b < charLevel)
                entry = 200;
            scratch = *(int*)((char*)this + 0xa0);
            if (scratch != 0 && *(char*)(scratch + 0x98) == '\x01' &&
                (entry = 0xfa, *(int*)(scratch + 0x108) != 0))
                entry = charLevel + 0x6e;
            if (entry < total)
                total = entry;
        }
    }
    return total;
}

// FUNCTION: TRILOGY 0x00423c9c
// Reads an unspecified actor-side value for a matching active buff.
// @early-stop: the spell filter stays on the stack; the original caches it in EDX and repeats its range check.
/* fake match: the byte/dword union keeps the effect index in VC6's EBP-8 slot for the helper call. */
short EQ_Character_MagicSystem_guess::GetBuffEffectLevel_guess(char effect, u32 spell)
{
    short i;
    union EffectIndex
    {
        u32 argument;
        u8 value;
    } j;
    short spell_id;
    EQ_Spell_guess* sp;

    if (player == 0 || player->actor_info == 0)
        return 0;
    for (i = 0; i < 15; i++)
    {
        if (buffs[i].visible == 0)
            continue;
        spell_id = buffs[i].spell_id;
        if (spell_id > 2999)
            spell_id = 2999;
        else if (spell_id >= 3000 || spell_id <= 0)
            continue;
        sp = g_spells_guess[spell_id];
        if (sp == 0)
            continue;
        if ((s32)spell > 0 && (s32)spell < 3000)
        {
            if (spell == (u32)spell_id)
                return *(short*)((char*)player->actor_info + 0x344 + i * 2);
            continue;
        }
        for (j.value = 0; j.value < 12; j.value++)
        {
            u8 e = sp->effect_id[j.value];
            if (e == EQ_SPELL_EFFECT_NONE)
                continue;
            if (e != (u8)effect)
                continue;
            if ((short)EQ_STDCALL(int, 0x0042351A, (EQ_Spell_guess*, u8, u32, SpellBuff*))(
                    sp, buffs[i].level, j.argument, &buffs[i]) < 0 ||
                effect == 0x16 || effect == 0x17 || effect == 0x39)
                return *(short*)((char*)player->actor_info + 0x344 + i * 2);
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00423dad
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (address range; TAKP: CRaid_IsPlayerInRaidLooterList, wrong)
// __thiscall. The caster level of the first active buff of spell `spell`, 0 when there is none.
u8 EQ_Character::GetBuffLevel_guess(short spell)
{
    short i;

    for (i = 0; i < 15; i++)
    {
        if (buffs[i].visible != 0 && buffs[i].spell_id == spell)
            return buffs[i].level;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00423e6f
// Identifies spells that skip all casting requirement checks.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (address range)
// static (__cdecl), called from CastingRequirementsMet. 1 for spell ids 0x2F8..0x300, 0x728..0x75B and a
// list of single ids.
int EQ_Character::IsSpecialSpell_guess(short spell)
{
    if ((spell >= 0x2F8 && spell <= 0x300) || (spell >= 0x728 && spell <= 0x75B) || spell == 0x57 || spell == 0x58
        || spell == 0x1DC || spell == 0x337 || spell == 0x335 || spell == 0x336 || spell == 0x5BF || spell == 0x5B9
        || spell == 0x5BA || spell == 0x5BB || spell == 0x7AD || spell == 0x7AE)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x00423edb
// Bards need the song's instrument; other casters must stand, pass channeling when disturbed, have components
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
// __thiscall, `self` in ECX. Written as a __fastcall free function (not declared in eq_character.h yet): EDX
// is the `unused` dummy, the spell is the one stack argument, the callee pops 4. Returns AL: true when the cast
// may go on (or nothing to check), false when it is refused or interrupted.
// fake match: the exits are gotos to two shared return blocks at the end, the layout VC6 gives the original.
// @early-stop: spellRec is in ECX/stack not ESI, frame is 0x3e4 not 0x3e8, float tests lay out differently.
bool __fastcall CastingRequirementsMet(EQ_Character* self, int unused, int spell)
{
    char* actor;        // self+0xa0, the actor (read once after the spell checks)
    char* info;         // actor+0x70
    char* spellRec;     // g_spells_guess[sid] (local_14, EBP-0x10)
    char* name;
    bool found;         // FindItemByType result
    char hasEffect;
    u8 channelingSkill;
    short slot;         // FindItemByType slot (EBP+0xa)
    char flag;          // a required item is missing (param_2._3_1_, EBP+0xb)
    int instrument;     // uVar13
    short sid;          // sVar12, the 16-bit spell id, then the loop index
    short itemNr;
    short* componentCursor;
    short* checkCursor;
    int temp;
    int spellLevel;
    int charLevel;
    float dy;           // local_10
    int missing;        // local_8, entries in missingItems
    int scratch;
    int channeling;
    int reliable;
    int rolls;
    int rollIndex;
    float dx;
    float absDelta;
    short missingItems[102];
    char msgText[256];
    char msgCopy[512];

    if (EQ_CDECL(int, 0x00423E6F, (int))(spell) != 0)
        goto allow;
    sid = (short)spell;
    if (sid <= 0 || sid >= 3000 || (spellRec = (char*)g_spells_guess[sid]) == 0)
    {
        EQ_CDECL(void, 0x004A3D01, (const char*))(EQ_ARRAY(char, 0x00541FCC));
        goto refuse;
    }
    actor = *(char**)((char*)self + 0xa0);
    if ((*(float*)(actor + 0x48) == 0.0f && (sid == 0x57 || sid == 0x58 || sid == 0xa48)) || *(char*)(actor + 0x98) != 0)
        goto allow;
    missing = 0;
    flag = 0;
    missingItems[0] = -1;
    missingItems[1] = -1;
    missingItems[2] = -1;
    missingItems[3] = -1;
    missingItems[4] = -1;
    missingItems[5] = -1;
    if (*((char*)self + 0x36) == 8)
    {
        if (actor == 0
            || (info = *(char**)(actor + 0x70)) == 0
            || *(int*)(info + 0x2dc) != 0x1d)
        {
            if (*(char**)((char*)self + 0x114) == 0 || *(short*)(*(char**)((char*)self + 0x114) + 0x82) != 0x503e)
            {
                hasEffect = *(char*)(spellRec + 0x230);
                switch (hasEffect)
                {
                case 0x0c:
                    instrument = 0x19;
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(msgText, EQ_ARRAY(char, 0x00541EF4));
                    break;
                case 0x31:
                    instrument = 0x18;
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(msgText, EQ_ARRAY(char, 0x00541F28));
                    break;
                case 0x36:
                    instrument = 0x17;
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(msgText, EQ_ARRAY(char, 0x00541F60));
                    break;
                case 0x46:
                    instrument = 0x1a;
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(msgText, EQ_ARRAY(char, 0x00541F94));
                    break;
                default:
                    instrument = -1;
                    break;
                }
                *(short*)(*(char**)(actor + 0x70) + 0x24c) = 10;
                if ((short)instrument != 0)
                {
                    found = EQ_METHOD(3, bool, 0x00427C70, (void*, int, short*, short*), self)(instrument, &slot, 0);
                    if (!found || (slot != 0xd && slot != 0xe))
                    {
                        if (*(short*)(spellRec + 0x20e) != -1)
                        {
                            EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self, msgText, 0x121);
                            goto refuse;
                        }
                    }
                    else
                    {
                        temp = *(int*)((char*)self + 0xe0 + slot * 4);
                        if (temp != 0)
                        {
                            if (EQ_METHOD(1, u8, 0x0041A3E9, (void*, int), self)((int)*(u8*)(spellRec + 0x230)) == 0)
                            {
                                EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self, EQ_ARRAY(char, 0x00541EB8), 0x121);
                                goto refuse;
                            }
                            *(u16*)(*(char**)(*(char**)((char*)self + 0xa0) + 0x70) + 0x24c) = *(u8*)(temp + 0xbe);
                        }
                    }
                }
            }
            else if (actor != 0 && *(char**)(actor + 0x70) != 0)
            {
                *(short*)(*(char**)(actor + 0x70) + 0x24c) = 0x12;
            }
        }
        else
        {
            *(short*)(info + 0x24c) = 0x1c;
        }
        goto allow;
    }
    else
    {
        if (*((char*)self + 0x994) != 'd')
        {
            EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self, EQ_ARRAY(char, 0x00541510), 0x121);
            goto refuse;
        }
        info = *(char**)(actor + 0x70);
        dx = *(float*)(actor + 0x34) - *(float*)(info + 0x2b4);
        dy = *(float*)(actor + 0x38) - *(float*)(info + 0x2b8);
        if (*(int*)(info + 0x18c) != 0)
        {
            dx = *(float*)(actor + 0x4c);
            dy = *(float*)(actor + 0x50);
        }
        if (self->castRequirementState_guess != 0
            || (*(int*)(info + 0x18c) == 0 && ((dx < 0.0f ? -dx : dx) > 0.1 || (dy < 0.0f ? -dy : dy) > 0.1)))
        {
            if (*(float*)(*(char**)((char*)self + 0xa0) + 0x48) != 0.0f)
                goto refuse;
            absDelta = dx < 0.0f ? -dx : dx;
            if (absDelta > 1.0)
                goto refuse;
            absDelta = dy < 0.0f ? -dy : dy;
            if (absDelta > 1.0)
                goto refuse;
            rolls = (self->castRequirementState_guess * 3) / 4 + 1;
            if (rolls < 1)
                rolls = 1;
            channelingSkill = EQ_METHOD(1, u8, 0x004246A9, (void*, int), self)(0xd);
            if (channelingSkill == 0)
                goto refuse;
            rollIndex = 0;
            if (0 < rolls)
            {
                channeling = channelingSkill;
                do
                {
                    scratch = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 0x186);
                    charLevel = self->level;
                    spellLevel = *(u8*)(spellRec + 0x234 + self->class_);
                    temp = 0;
                    if (charLevel > spellLevel + 5)
                        temp = (charLevel - spellLevel) * 6 / 2 + 0x23;
                    temp = channeling + charLevel + temp;
                    if (0x172 < temp)
                        temp = 0x172;
                    if (temp < scratch && 0x26 < scratch)
                        goto refuse;
                    rollIndex = rollIndex + 1;
                } while (rollIndex < rolls);
            }
            EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))(self, EQ_ARRAY(char, 0x00541E7C), 0x108);
        }
        sid = 0;
        do
        {
            hasEffect = EQ_CDECL(char, 0x0048A11F, (void*, int))(spellRec, 0x21);
            if (hasEffect != 0
                || (hasEffect = EQ_CDECL(char, 0x0048A11F, (void*, int))(spellRec, 0x47), hasEffect != 0)
                || (hasEffect = EQ_CDECL(char, 0x0048A11F, (void*, int))(spellRec, 0x6a), hasEffect != 0)
                || (hasEffect = EQ_CDECL(char, 0x0048A11F, (void*, int))(spellRec, 0x6c), hasEffect != 0))
                break;
            itemNr = *(short*)(spellRec + 0x20e + sid * 2);
            if (itemNr != -1 && !EQ_METHOD(3, bool, 0x00427D2A, (void*, int, short*, short*), self)((int)itemNr, 0, 0))
            {
                temp = (short)missing;
                missing = missing + 1;
                flag = 1;
                missingItems[temp] = itemNr;
            }
            sid = sid + 1;
        } while (sid < 2);
        componentCursor = (short*)(spellRec + 0x1fe);
        scratch = 4;
        checkCursor = componentCursor;
        do
        {
            if (*checkCursor != -1 && !EQ_METHOD(2, bool, 0x00427DD3, (void*, short, short), self)(*checkCursor, checkCursor[4]))
            {
                temp = (short)missing;
                missing = missing + 1;
                flag = 1;
                missingItems[temp] = *checkCursor;
            }
            checkCursor = checkCursor + 1;
            scratch = scratch - 1;
        } while (scratch != 0);
        if (flag == 0)
        {
            scratch = 4;
            do
            {
                if (*componentCursor != -1)
                {
                    sid = 0;
                    if (0 < componentCursor[4])
                    {
                        do
                        {
                            hasEffect = EQ_METHOD(2, char, 0x00427EE7, (void*, int, short), self)(-1, *componentCursor);
                            if (hasEffect == 0)
                            {
                                temp = (short)missing;
                                missing = missing + 1;
                                flag = 1;
                                missingItems[temp] = *componentCursor;
                                break;
                            }
                            sid = sid + 1;
                        } while (sid < componentCursor[4]);
                    }
                }
                componentCursor = componentCursor + 1;
                scratch = scratch - 1;
            } while (scratch != 0);
            if (flag == 0)
                goto allow;
        }
        reliable = 1;
        send_message_guess(g_world_connection_guess, 0x212f, missingItems, 0xd0, reliable);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00541E4C), 0x121);
        name = EQ_METHOD(1, char*, 0x004D0436, (void*, void*), EQ_GLOBAL(void*, 0x006EFE2C))(self);
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(msgText, EQ_ARRAY(char, 0x00541E2C), name);
        temp = EQ_GLOBAL(int, 0x006B6EE4);
        if (*((char*)self + 0x36) == 8)
            temp = EQ_GLOBAL(int, 0x006B6EEC);
        if (temp != 3)
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00541E0C), 0x121);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(msgCopy, msgText);
    }
refuse:
    return false;
allow:
    return true;
}

// FUNCTION: TRILOGY 0x00424591
// Calculates how many buff ticks a spell lasts at the caster's level.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (Mac: EQ_Character::SpellDuration, low)
// __thiscall, `this` unused. Buff duration in ticks for a caster level: formula 1..12 of the spell, capped by
// its duration when that is nonzero; formula 50 = -1 (permanent), 0 or no spell = 0, any other formula
// -2 when `permanent_ok` else 0.
u32 EQ_Character::SpellDuration_guess(EQ_Spell_guess* spell, u8 casterLevel, char permanent_ok)
{
    u32 ticks = 0;

    if (spell != 0 && spell->duration_formula != 0)
    {
        switch (spell->duration_formula)
        {
        case 1:
            ticks = casterLevel / 2 < 1 ? 1 : casterLevel / 2;
            break;
        case 2:
            ticks = (casterLevel / 2 < 1 ? 1 : casterLevel / 2) + 5;
            break;
        case 3:
            ticks = casterLevel * 30;
            break;
        case 4:
            ticks = 50;
            break;
        case 5:
            ticks = 2;
            break;
        case 6:
            ticks = casterLevel / 2 + 2;
            break;
        case 7:
            ticks = casterLevel;
            break;
        case 12:
            ticks = casterLevel / 4;
            if (ticks < 1)
                ticks = 1;
            break;
        case 8:
            ticks = casterLevel + 10;
            break;
        case 9:
            ticks = casterLevel * 2 + 10;
            break;
        case 10:
            ticks = casterLevel * 3 + 10;
            break;
        case 11:
            ticks = (casterLevel + 3) * 30;
            break;
        case 50:
            return 0xFFFFFFFF;
        default:
            return permanent_ok ? 0xFFFFFFFE : 0;
        }
        if (spell->duration > 0 && spell->duration < ticks)
            ticks = spell->duration;
    }
    return ticks;
}

// FUNCTION: TRILOGY 0x00424e22
// Removes one of the player's active buffs.
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
// __thiscall EQ_Character::RemovePCAffect (Mac, medium): RemovePCAffectex_guess (0x00424E30) with flag 1;
// `this` (ECX) passes through untouched. Written as a __fastcall free function because eq_character.h
// declares no member for it.
void __fastcall EQ_Character_RemovePCAffect_guess(EQ_Character* self, int edx_unused, int spellBuff)
{
    EQ_THISCALL(void, 0x00424E30, (EQ_Character*, int, int, int))(self, edx_unused, spellBuff, 1);
}

// FUNCTION: TRILOGY 0x00424e30
// Ends a buff: wear-off text, clears the slot, undoes levitate/movement/visibility, tells the server if asked
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
// @early-stop: switch and branch layout improved; stack frame and register allocation still differ
void __fastcall EQ_Character_RemovePCAffectex_guess(EQ_Character* self, int edx_unused, u8* buffSlot, int notifyServer)
{
    char firstEffect;
    u8 effectId;
    short buffSpellId;
    u8* buff;
    u32 wearOffLen;
    u8* buffCursor;
    int temp;
    int moveRate;
    u8* effectCursor;
    int spellRec;
    int removedSpellId;

    buff = buffSlot;
    if (buffSlot == 0)
    {
        return;
    }
    buffSpellId = *(short*)(buff + 4);
    if (buffSpellId < 0 || buffSpellId >= 3000)
    {
        spellRec = 0;
    }
    else
    {
        spellRec = (int)EQ_ARRAY(void*, 0x005FE9A0)[buffSpellId];
    }
    if (spellRec != 0
        && (wearOffLen = EQ_CDECL(u32, 0x005223D0, (const char*))((const char*)(spellRec + 0x160)), wearOffLen > 0))
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)(spellRec + 0x160), 0x108);
    }
    buffSlot = 0;
    buffCursor = (u8*)self + 0x284;
    while (buff != buffCursor)
    {
        buffSlot = (u8*)((int)buffSlot + 1);
        buffCursor += 10;
        if ((int)buffSlot >= 15)
        {
            return;
        }
        else
        {
            continue;
        }
    }
    if (spellRec != 0)
    {
        firstEffect = *(char*)(spellRec + 0x222);
        if (firstEffect == '\x17' || firstEffect == '\x16' || *(char*)(spellRec + 0x21f) == '\0')
        {
            EQ_METHOD(2, void, 0x00423C9C, (EQ_Character*, int, int), self)((int)(((u32)buffCursor & 0xffffff00) | (u8)firstEffect),
                (int)*(short*)(buff + 4));
        }
    }
    removedSpellId = (int)*(short*)(buff + 4);
    *buff = 0;
    *(short*)(buff + 4) = -1;
    buff[1] = 0;
    buff[3] = 0;
    buff[2] = 0;
    *(short*)(buff + 6) = 0;
    *(short*)(buff + 8) = 0;
    if (*(int*)((u8*)self + 0xa0) != 0
        && (temp = *(int*)(*(int*)((u8*)self + 0xa0) + 0x70), temp != 0))
    {
        *(short*)(temp + 0x344 + (int)buffSlot * 2) = 0;
    }
    if (spellRec == 0)
    {
        return;
    }
    if (*(int*)((u8*)self + 0xa0) != 0)
    {
        effectCursor = (u8*)(spellRec + 0x222);
        spellRec = 0xc;
        do
        {
            effectId = *effectCursor;
            switch (effectId)
            {
            case 0x39:
                *(short*)(*(int*)((u8*)self + 0xa0) + 0xf2) = 0;
                *(u8*)(*(int*)(*(int*)((u8*)self + 0xa0) + 0x70) + 0x88) = 1;
                *(short*)(*(int*)(*(int*)((u8*)self + 0xa0) + 0x70) + 0x78) = -1;
                break;
            case 3:
                *(float*)(*(int*)(*(int*)((u8*)self + 0xa0) + 0x70) + 0xb8) = 0.0f;
                moveRate = EQ_METHOD(1, int, 0x004238A2, (EQ_Character*, int), self)(99);
                if ((short)moveRate < 0)
                {
                    EQ_METHOD(1, void, 0x00420046, (EQ_Character*, int), self)(moveRate);
                }
                else
                {
                    moveRate = EQ_METHOD(1, int, 0x004238A2, (EQ_Character*, int), self)(3);
                    if ((short)moveRate != 0)
                    {
                        EQ_METHOD(1, void, 0x00420046, (EQ_Character*, int), self)(moveRate);
                    }
                }
                break;
            case 0xc:
            case 0x1c:
            case 0x1d:
                EQ_METHOD(0, void, 0x004255CE, (EQ_Character*), self)();
                break;
            case 0xd:
                EQ_METHOD(1, void, 0x00410B68, (void*, int), g_display_guess)(0);
                if (EQ_GLOBAL(void*, 0x006E4DAC) != 0
                    && EQ_GLOBAL(void*, 0x006E4DAC) != EQ_GLOBAL(void*, 0x006E4D8C)
                    && (temp = EQ_METHOD(2, int, 0x00410C80, (void*, void*, void*), g_display_guess)(
                            EQ_GLOBAL(void*, 0x006E4D8C), EQ_GLOBAL(void*, 0x006E4DAC)), temp != 0))
                {
                    EQ_GLOBAL(void*, 0x006E4DAC) = 0;
                }
                break;
            case 0x14:
                if (*(int*)((u8*)self + 0xa0) != 0)
                {
                    *(u8*)(*(int*)(*(int*)((u8*)self + 0xa0) + 0x70) + 0x189) = 0;
                }
                pinstEQ_Main_guess->uiDirty_guess = 1;
                break;
            case 0x1f:
                EQ_METHOD(0, void, 0x004247B1, (EQ_Character*), self)();
                break;
            case 0x41:
            case 0x42:
                temp = *(int*)((u8*)self + 0xa0);
                if (temp != 0 && *(char*)(temp + 0x98) == '\0')
                {
                    EQ_METHOD(2, void, 0x0041D17E, (EQ_Character*, short, u8), self)(*(short*)(temp + 0x9a), self->class_);
                    EQ_METHOD(0, void, 0x004122AD, (void*), g_display_guess)();
                }
                break;
            case 0x43:
                if (EQ_GLOBAL(void*, 0x006E4DA0) != 0)
                {
                    EQ_METHOD(1, void, 0x004B443E, (void*, u8), EQ_GLOBAL(void*, 0x006E4DA0))(
                        *(u8*)(*(int*)((u8*)EQ_GLOBAL(void*, 0x006E4D8C) + 0x70) + 0x98));
                }
                EQ_GLOBAL(void*, 0x006E4DA0) = *(void**)((u8*)self + 0xa0);
                EQ_METHOD(1, void, 0x00413ED6, (void*, int), g_display_guess)(*(int*)((u8*)self + 0xa0) + 1);
                break;
            case 0x49:
                EQ_METHOD(1, void, 0x00413ED6, (void*, int), g_display_guess)(*(int*)((u8*)self + 0xa0) + 1);
                break;
            case 0x57:
                temp = (int)*(float*)((int)EQ_ARRAY(float, 0x006B6F2C)
                    + g_activeView_guess * 0x1c);
                EQ_METHOD(1, void, 0x0040E4C6, (void*, int), g_display_guess)(temp);
                break;
            case 99:
                *(float*)(*(int*)(*(int*)((u8*)self + 0xa0) + 0x70) + 0xb8) = 0.0f;
                moveRate = EQ_METHOD(1, int, 0x004238A2, (EQ_Character*, int), self)(3);
                if ((short)moveRate != 0)
                {
                    EQ_METHOD(1, void, 0x00420046, (EQ_Character*, int), self)(moveRate);
                }
                break;
            }
            effectCursor++;
            spellRec--;
        }
        while (spellRec != 0);
    }
    if (notifyServer != 0 && buffSlot != (u8*)0xffffffff)
    {
        struct RemovePCAffectMessage
        {
            u32 spawnId;
            u8 affectData[4];
            short spellId;
            u16 affectTail0;
            u16 affectTail1;
            u16 unused;
            int slot;
        } removeMsg;

        removeMsg.spawnId = *(u32*)(*(int*)((u8*)self + 0xa0) + 0x80);
        EQ_CDECL(void*, 0x00521B10, (void*, const void*, unsigned int))(
            removeMsg.affectData, (u8*)self + 0x284 + (int)buffSlot * 10, 10);
        removeMsg.slot = (int)buffSlot;
        removeMsg.spellId = (short)removedSpellId;
        send_message_guess(
            &EQ_GLOBAL(void*, 0x006B3194), 0x2132, &removeMsg, 0x14, 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x0042520e
// Buff tick: counts down the 15 buffs, removes expired ones, applies mana/fatigue changes, ticks item effects
// @early-stop: frame and switch shape match closely; cursor registers, stack slots, and inventory store scheduling still differ
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
void __fastcall EQ_Character_ProcessAffects_guess(EQ_Character* self, int edx_unused)
{
    u8 effectId;
    EQActorInfo_guess* actorInfo;
    EQ_Item* pEVar3;
    short temp;
    short curMana;
    u16 newDuration;
    EQPlayer* pEVar7;
    short* durationPtr;
    int scratch;
    u8 eyeName[32];
    u8 charName[25];
    int msgData[4];
    EQ_Item** ptr;
    int spell;
    int effectIndex;

    durationPtr = (short*)((char*)&self->buffs[0].duration);
    msgData[3] = 0xf;
    do
    {
        if (((SpellBuff*)((char*)durationPtr - 6))->visible >= 2)
        {
            temp = *durationPtr;
            ptr = (EQ_Item**)durationPtr;
            if (temp == 0)
            {
                EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, SpellBuff*), self)((SpellBuff*)((char*)durationPtr - 6));
            }
            else
            {
                short spellId = ((SpellBuff*)((char*)durationPtr - 6))->spell_id;
                if (spellId <= 0 || spellId >= 3000)
                {
                    spell = 0;
                }
                else
                {
                    if (spellId > 2999)
                    {
                        scratch = 2999;
                    }
                    else
                    {
                        scratch = (int)spellId;
                    }
                    spell = EQ_ARRAY(int, 0x005FE9A0)[scratch];
                }
                if (temp != -1)
                {
                    *(short*)durationPtr = temp - 1;
                }
                if (spell != 0)
                {
                    effectIndex = 0;
                    do
                    {
                        effectId = *(u8*)(spell + 0x222 + effectIndex);
                        switch (effectId)
                        {
                        case 0x39:
                            if ((short)*durationPtr != -1 && (short)*durationPtr < 2)
                            {
                                temp = *(s16*)((char*)durationPtr - 2);
                                if (temp <= 0 || temp >= 3000 ||
                                    *(u8*)(EQ_ARRAY(int, 0x005FE9A0)[temp] + 0x23c) <= 0 ||
                                    *(u8*)(EQ_ARRAY(int, 0x005FE9A0)[temp] + 0x23c) >= 0x3d)
                                {
                                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00542134), 0x121);
                                }
                            }
                            break;

                        case 0x18:
                            if (self->player != 0 && self->player->type == 0)
                            {
                                temp = (short)EQ_METHOD(4, int, 0x0042351A, (EQ_Character*, int, u8, int, SpellBuff*), self)(spell,
                                        ((SpellBuff*)((char*)durationPtr - 6))->level, effectIndex,
                                        (SpellBuff*)((char*)durationPtr - 6));
                                EQ_METHOD(1, void, 0x00480DFB, (EQ_Character*, int), self)(
                                        (int)temp + (int)(char)self->fatigue);
                            }
                            break;

                        case 0xf:
                            if (self->class_ != '\b')
                            {
                                temp = (short)EQ_METHOD(4, int, 0x0042351A, (EQ_Character*, int, u8, int, SpellBuff*), self)(spell,
                                        ((SpellBuff*)((char*)durationPtr - 6))->level, effectIndex,
                                        (SpellBuff*)((char*)durationPtr - 6));
                                if (temp < 0 && *(char*)(spell + 0x21f) == '\x01')
                                {
                                    curMana = EQ_METHOD(0, short, 0x00419BA3, (EQ_Character*), self)();
                                    scratch = EQ_CDECL(int, 0x005232FF, (int))(temp);
                                    durationPtr = (short*)ptr;
                                    if (curMana < scratch)
                                    {
                                        EQ_METHOD(1, void, 0x00424E22, (EQ_Character*, SpellBuff*), self)(
                                                (SpellBuff*)((char*)ptr - 6));
                                    }
                                }
                                self->mana = self->mana + temp;
                                EQ_METHOD(0, short, 0x00419BA3, (EQ_Character*), self)();
                            }
                            break;

                        case 0x3a:
                            if ((short)*durationPtr != -1 && (short)*durationPtr < 2)
                            {
                                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005420F8), 0x121);
                            }
                            break;

                        case 3:
                        case 99:
                            actorInfo = self->player->actor_info;
                            if (actorInfo->unk_0xB8 <=
                                EQ_ARRAY(float, 0x0053510C)[0])
                            {
                                EQ_CDECL(void, 0x00422D68, (short))(
                                    *(s16*)((char*)durationPtr - 2));
                            }
                            break;

                        case 0x43:
                            if (EQ_GLOBAL(void*, 0x006E4DA0) ==
                                EQ_GLOBAL(void*, 0x006E4D8C))
                            {
                                EQ_CDECL(void, 0x004A0841, (char*, EQ_Character*, int))(
                                    (char*)charName, self, 0x18);
                                EQ_CDECL(int, 0x005224FB,
                                    (char*, const char*, const char*))(
                                        (char*)eyeName, EQ_ARRAY(char, 0x005420EC),
                                        (char*)charName);
                                pEVar7 = EQ_METHOD(1, EQPlayer*, 0x004B146A, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))((char*)eyeName);
                                if (pEVar7 != 0)
                                {
                                    if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) != 0)
                                    {
                                        EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->speed = 0.0f;
                                        EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->delta_heading = 0.0f;
                                    }
                                    EQ_GLOBAL(void*, 0x006E4DA0) = pEVar7;
                                    EQ_METHOD(1, void, 0x00413ED6, (void*, const char*), g_display_guess)(pEVar7->name);
                                }
                            }
                            break;

                        case 0x4d:
                            EQ_METHOD(0, void, 0x00420184, (EQ_Character*), self)();
                            break;

                        default:
                            break;
                        }
                        effectIndex = effectIndex + 1;
                    }
                    while (effectIndex < 0xc);
                }
            }
        }
        durationPtr = (short*)((char*)durationPtr + 10);
        msgData[3] = msgData[3] - 1;
        if (msgData[3] == 0)
        {
            ptr = self->inventory_items;
            scratch = 1;
            do
            {
                ptr = ptr + 1;
                pEVar3 = *ptr;
                if (pEVar3 != 0 && pEVar3->type == 0 && pEVar3[1].lore[9] == '\x02')
                {
                    temp = *(short*)(pEVar3[1].lore + 0xf);
                    if (temp != -2 && temp != -1)
                    {
                        if (temp == 0)
                        {
                            if (pEVar3[1].lore[0xb] == '\x01')
                            {
                                pEVar3[1].lore[9] = '\0';
                                pEVar3[1].lore[0xd] = -1;
                                pEVar3[1].lore[0xe] = -1;
                                pEVar3[1].lore[10] = '\0';
                                pEVar3[1].lore[0xf] = '\0';
                                pEVar3[1].lore[0x10] = '\0';
                                pEVar3[1].lore[0xc] = '\0';
                                pEVar3[1].lore[0xb] = '\0';
                            }
                            else
                            {
                                pEVar3[1].lore[0xb] = pEVar3[1].lore[0xb] - 1;
                                newDuration = (u16)EQ_METHOD(3, u32, 0x00424591, (EQ_Character*, void*, u8, char), self)(
                                        EQ_ARRAY(void*, 0x005FE9A0)
                                            [*(short*)(pEVar3[1].lore + 0xd)],
                                        pEVar3[1].lore[10], 1);
                                *(u16*)(pEVar3[1].lore + 0xf) = newDuration;
                                msgData[1] = -1;
                                msgData[2] = -1;
                                msgData[0] = scratch;
                                EQ_CDECL(void, 0x004E1C45,
                                    (void**, u32, void*, u32, int))(
                                        &EQ_GLOBAL(void*, 0x006B3194), 0x2146,
                                        msgData, 0xc, 1);
                            }
                        }
                        else
                        {
                            *(short*)(pEVar3[1].lore + 0xf) = temp - 1;
                        }
                    }
                }
                scratch = scratch + 1;
            }
            while (scratch <= 0x1d);
            return;
        }
    }
    while (true);
}

// FUNCTION: TRILOGY 0x004255ce
// Updates the character's invisibility or concealment state and appearance.
// @early-stop: frame pads shift the locals (test build has no ebp slot); without them the fixture's out-pointer check fails
void __fastcall UpdateMyVisibleStatus(EQ_Character* self, int edx_unused)
{
    char previousVisibilityMode;
    int spawn;
    short spellEffectTotal;
    char* visibilityModePtr;
    u32 spellBuff;
    /* fake match: fakePad16 and fakePad32 only shape the frame. The fixture build has no ebp frame, so
       without them local_6 (the GetPCSpellAffect out-parameter) sits 4 bytes above the original's and the
       pointer check fails. Nothing reads or writes the pads. */
    struct
    {
        struct SpawnAppearanceMessage_guess
        {
            u32 spawnId;
            u16 type;
            u16 unused;
            u32 mode;
        } localFrame;
        u16 fakePad16;
        short local_6;
        u32 fakePad32;
    } frame;

    visibilityModePtr = (char*)(*(int*)((char*)self + 0xa0) + 0x9e);
    previousVisibilityMode = *visibilityModePtr;
    *visibilityModePtr = '\0';
    if ((self->class_ < 0x10) || (0x20 < self->class_))
    {
        if (EQ_GLOBAL(u8, 0x006B65D8) != 0)
            *(u8*)(*(int*)((char*)self + 0xa0) + 0x9e) = 1;

        if (*(char*)(*(int*)((char*)self + 0xa0) + 0x9e) == '\0')
        {
            spellEffectTotal = EQ_THISCALL(short, 0x004238A2, (EQ_Character*, int, int))(self, 0, 0xc);
            if (0 < spellEffectTotal)
                *(u8*)(*(int*)((char*)self + 0xa0) + 0x9e) = 1;
            else
            {
                spellEffectTotal = EQ_THISCALL(short, 0x004238A2, (EQ_Character*, int, int))(self, 0, 0x1c);
                if (0 < spellEffectTotal)
                    *(u8*)(*(int*)((char*)self + 0xa0) + 0x9e) = 3;
                else
                {
                    spellEffectTotal = EQ_THISCALL(short, 0x004238A2, (EQ_Character*, int, int))(self, 0, 0x1d);
                    if (0 < spellEffectTotal)
                        *(u8*)(*(int*)((char*)self + 0xa0) + 0x9e) = 2;
                }
            }
        }

        spawn = *(int*)((char*)self + 0xa0);
        if (*(char*)(spawn + 0x9e) != previousVisibilityMode)
        {
            frame.localFrame.spawnId = *(u32*)(spawn + 0x80);
            frame.localFrame.type = 3;
            frame.localFrame.mode = (u32)*(u8*)(spawn + 0x9e);
            if (*(u8*)(spawn + 0x9e) != 1)
            {
                spellBuff = (u32)EQ_THISCALL(void*, 0x0042381B,
                    (void*, int, u8, short*))(EQ_GLOBAL(void*, 0x006E4DA8), 0, 0xc,
                    &frame.local_6);
                EQ_THISCALL(void, 0x00424E22, (void*, int, int))(
                    EQ_GLOBAL(void*, 0x006E4DA8), 0, (int)spellBuff);
                if (frame.local_6 != -1)
                {
                    EQ_THISCALL(void, 0x004251B9, (EQ_Character*, int, int))(
                        self, 0, (int)frame.local_6);
                }
            }
            if (*(char*)(*(int*)((char*)self + 0xa0) + 0x9e) != '\x03')
            {
                spellBuff = (u32)EQ_THISCALL(void*, 0x0042381B,
                    (void*, int, u8, short*))(EQ_GLOBAL(void*, 0x006E4DA8), 0, 0x1c,
                    &frame.local_6);
                EQ_THISCALL(void, 0x00424E22, (void*, int, int))(
                    EQ_GLOBAL(void*, 0x006E4DA8), 0, (int)spellBuff);
                if (frame.local_6 != -1)
                {
                    EQ_THISCALL(void, 0x004251B9, (EQ_Character*, int, int))(
                        self, 0, (int)frame.local_6);
                }
            }
            if (*(char*)(*(int*)((char*)self + 0xa0) + 0x9e) != '\x02')
            {
                spellBuff = (u32)EQ_THISCALL(void*, 0x0042381B,
                    (void*, int, u8, short*))(EQ_GLOBAL(void*, 0x006E4DA8), 0, 0x1d,
                    &frame.local_6);
                EQ_THISCALL(void, 0x00424E22, (void*, int, int))(
                    EQ_GLOBAL(void*, 0x006E4DA8), 0, (int)spellBuff);
                if (frame.local_6 != -1)
                {
                    EQ_THISCALL(void, 0x004251B9, (EQ_Character*, int, int))(
                        self, 0, (int)frame.local_6);
                }
            }
            send_message_guess(
                g_world_connection_guess, 0x20f5, &frame.localFrame, 0xc, 1);
        }
        EQ_THISCALL(void, 0x00410B68, (void*, int, void*))(
            g_display_guess, 0, (void*)*(int*)((char*)self + 0xa0));
    }
    return;
}

// FUNCTION: TRILOGY 0x004279b4
// Ends an active spell cast or bard song and clears its running state.
// ORIGINAL FILE: EverQuest/EQ_Character_MagicSystem.cpp (from Mac debug map)
void __fastcall EQ_Character_StopSpellCast_guess(EQ_Character* self, int edx_unused,
                                                  char stopMode, short castSpellId)
{
    struct StopSpellCastBuffers
    {
        u32 spawnId;
        char message[512];
        char chatText[128];
        char songSuffix[16];
    } buffers;
    int isSong = true;
    short spellId;
    size_t messageLength;
    char* strippedName;
    int spellOffset;
    short* songSpell;
    int spell;

    if (*(int*)((char*)self + 0xA0) != 0
        && *(int*)((char*)*(void**)((char*)self + 0xA0) + 0x70) != 0)
    {
        if (castSpellId <= 0 || castSpellId >= 3000)
            spell = 0;
        else
            spell = EQ_ARRAY(int, 0x005FE9A0)[castSpellId];

        if (EQ_METHOD(2, int, 0x0041F0C0, (EQ_Character*, void*, int), self)((void*)spell, self->class_) == 0)
        {
            isSong = 0;
            if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
            {
                if (pinstEQ_Main_guess->pendingServerReplies_guess > 0)
                    pinstEQ_Main_guess->pendingServerReplies_guess =
                        pinstEQ_Main_guess->pendingServerReplies_guess - 1;
                else
                    pinstEQ_Main_guess->pendingServerReplies_guess = 0;
                if (pinstEQ_Main_guess->pendingWorldRequests_guess > 0)
                    pinstEQ_Main_guess->pendingWorldRequests_guess =
                        pinstEQ_Main_guess->pendingWorldRequests_guess - 1;
                else
                    pinstEQ_Main_guess->pendingWorldRequests_guess = 0;
            }
            if (self->class_ == 8)
                return;
        }

        if (spell != 0)
        {
            if (stopMode == 0 && EQ_GLOBAL(u8, 0x006B63F8) == 0
                && DAT_006b6494 == 0)
            {
                send_message_guess
                    ((void**)0x006B3194, 0x217F, 0, 0, 1);
            }
            EQ_GLOBAL(short, 0x006E1E0A) = -1;
            *(short*)((char*)*(void**)((char*)*(void**)((char*)self + 0xA0) + 0x70)
                + 0x248) = -1;
            *(u8*)((char*)*(void**)((char*)*(void**)((char*)self + 0xA0) + 0x70)
                + 0x24A) = 0;
            *(int*)((char*)*(void**)((char*)*(void**)((char*)self + 0xA0) + 0x70)
                + 0xC8) = 0;

            if (isSong != 0)
            {
                buffers.songSuffix[0] = 0;
                spellOffset = 0xD4;
                songSpell = &self->spell_memorized[0];
                do
                {
                    spellId = *songSpell;
                    if (spellId <= 0 || spellId >= 3000
                        || EQ_ARRAY(void*, 0x005FE9A0)[spellId] == 0)
                        *(int*)((char*)*(void**)((char*)*(void**)((char*)self + 0xA0)
                            + 0x70) + spellOffset) = 0;
                    else if ((u32)*(int*)((char*)EQ_ARRAY(void*, 0x005FE9A0)[spellId]
                        + 0x1B8) > 0)
                    {
                        // Keep slots whose spell has a nonzero effect.
                    }
                    else
                        *(int*)((char*)*(void**)((char*)*(void**)((char*)self + 0xA0)
                            + 0x70) + spellOffset) = 0;
                    songSpell++;
                    spellOffset += 4;
                } while (spellOffset < 0xF4);
                *(int*)((char*)*(void**)((char*)*(void**)((char*)self + 0xA0) + 0x70)
                    + 0x15C) = 0;
                if (stopMode == 1)
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (buffers.songSuffix, EQ_ARRAY(char, 0x00542500));
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (buffers.chatText, EQ_ARRAY(char, 0x005424EC), buffers.songSuffix);
                if (EQ_GLOBAL(int, 0x006B6EEC) != 3)
                {
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(buffers.chatText, 0x108);
                }
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (buffers.chatText, EQ_ARRAY(char, 0x005424D8), (char*)self,
                     buffers.songSuffix);
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (buffers.message, buffers.chatText);
                buffers.spawnId = g_local_player_guess->spawn_id;
                messageLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(buffers.message);
                send_message_guess
                    ((void**)0x006B3194, 0x21D3, &buffers.spawnId, messageLength + 8, 1);
            }
            else if (stopMode == 1)
            {
                strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, EQ_Character*), EQ_GLOBAL(void*, 0x006EFE2C))(self);
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (buffers.chatText, EQ_ARRAY(char, 0x00541E2C), strippedName);
                if (EQ_GLOBAL(int, 0x006B6EE4) != 3)
                {
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                         EQ_ARRAY(char, 0x005424BC), 0x121);
                }
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (buffers.message, buffers.chatText);
                buffers.spawnId = g_local_player_guess->spawn_id;
                messageLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(buffers.message);
                send_message_guess
                    ((void**)0x006B3194, 0x21D3, &buffers.spawnId, messageLength + 8, 1);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0046a72f
// Provides the local singing state used by bard song playback controls.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (Mac: EQ_Character::BardCastBard, low)
// static (__cdecl), the overload without arguments. 1 when the local character is a bard (class 8) and its
// spawn's actor has the value at +0x248 set (not -1).
int EQ_Character::BardCastBard_guess(void)
{
    if (g_pc_guess != 0 && g_local_player_guess != 0 && g_local_player_guess->actor_info != 0
        && g_pc_guess->class_ == 8 && g_local_player_guess->actor_info->unknown0x248 != -1)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004a0792
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Character_MagicSystem.cpp (address range; TAKP: swapEndianness32_Swizzle_EQ_Spell)
// static (__cdecl). Reorders the 4 bytes at `data` (b0 b1 b2 b3 -> b1 b3 b0 b2). Returns 1.
int EQ_Character::Swizzle_guess(u8* data)
{
    struct Bytes4
    {
        u8 b[4];
    } copy;

    copy = *(Bytes4*)data;
    data[2] = copy.b[0];
    data[0] = copy.b[1];
    data[1] = copy.b[3];
    const unsigned char & temp = copy.b[2];
    data[3] = temp;
    return 1;
}

// FUNCTION: TRILOGY 0x004a331f
// Prepares a spell's effect and combat values for spell-hit processing.
int CopySpellInfo_guess(int spellIndex, int* spellInfo)
{
    int spellData;
    int copied;
    int copyOffset;

    EQ_CDECL(void*, 0x00521960, (void*, int, unsigned int))(spellInfo, 0, 0xac);
    if (((spellIndex < 1) || (spellIndex >= 0xbb8)) || (spellData = EQ_ARRAY(int, 0x005FE9A0)[spellIndex], spellData == 0))
    {
        copied = 0;
    }
    else
    {
        *spellInfo = spellIndex;
        spellInfo[1] = *(int*)(spellData + 0x1a0);
        spellInfo[2] = *(int*)(spellData + 0x1a8);
        spellInfo[3] = *(int*)(spellData + 0x1ac);
        spellInfo[4] = *(int*)(spellData + 0x1b0);
        spellInfo[5] = *(int*)(spellData + 0x1b4);
        spellInfo[6] = *(int*)(spellData + 0x1b8);
        spellInfo[7] = *(int*)(spellData + 0x1bc);
        spellInfo[8] = *(int*)(spellData + 0x1c0);
        spellInfo[9] = *(int*)(spellData + 0x1c4);
        *(u16*)(spellInfo + 10) = *(u16*)(spellData + 0x1c8);
        for (copyOffset = 0; copyOffset < 12; copyOffset++)
        {
            ((u16*)((int)spellInfo + 0x2a))[copyOffset] = ((u16*)(spellData + 0x1ca))[copyOffset];
            ((u16*)((int)spellInfo + 0x42))[copyOffset] = ((u16*)(spellData + 0x1e2))[copyOffset];
            ((u8*)((int)spellInfo + 0x6e))[copyOffset] = ((u8*)(spellData + 0x212))[copyOffset];
            ((u8*)((int)spellInfo + 0x7a))[copyOffset] = ((u8*)(spellData + 0x222))[copyOffset];
        }
        for (copyOffset = 0; copyOffset < 4; copyOffset++)
        {
            ((u16*)((int)spellInfo + 0x5a))[copyOffset] = ((u16*)(spellData + 0x1fe))[copyOffset];
            ((u16*)((int)spellInfo + 0x62))[copyOffset] = ((u16*)(spellData + 0x206))[copyOffset];
        }
        *(u16*)((int)spellInfo + 0x6a) = *(u16*)(spellData + 0x20e);
        *(u16*)(spellInfo + 0x1b) = *(u16*)(spellData + 0x210);
        *(u8*)((int)spellInfo + 0x86) = *(u8*)(spellData + 0x22e);
        *(u8*)((int)spellInfo + 0x87) = *(u8*)(spellData + 0x22f);
        *(u8*)(spellInfo + 0x22) = *(u8*)(spellData + 0x230);
        copyOffset = 0;
        do
        {
            *(u8*)((int)spellInfo + copyOffset + 0x89) = *(u8*)(spellData + 0x234 + copyOffset);
            copyOffset = copyOffset + 1;
        } while (copyOffset < 0x20);
        copied = 1;
    }
    return copied;
}

// FUNCTION: TRILOGY 0x004dd010
// Finds the player character under the cursor for click targeting and interaction.
// Picks the actor at screen (x, y) with CDisplay::PickActorOnScreen_guess (0x0041125D, display at
// 0x006E4DE0), then returns EQPlayer::is_player_actor (0x004B14BF)'s result for that actor.
// Callee pops 8 bytes, ECX is ignored.
void* __stdcall ClickedPlayer(short x, short y)
{
    void* actor;

    actor = EQ_METHOD(3, void*, 0x0041125D, (void*, int, int, int), g_display_guess)((int)x, (int)y, 0);
    return EQ_METHOD(1, void*, 0x004B14BF, (void*, void*), EQ_GLOBAL(void*, 0x006E4D58))(actor);
}

// FUNCTION: TRILOGY 0x004dd440
// Starts a loot interaction with a nearby corpse.
int __fastcall RequestLootCorpse_guess(void* everquest, int unused_edx, EQPlayer* corpse, int unused)
{
    struct MsgSpawnAppearance_guess
    {
        u32 spawn_id;           /* 0x00 */
        s16 type;               /* 0x04 */
        u16 unknown0x06;        /* 0x06 */
        u32 parameter;          /* 0x08 */
    } visibilityMessage;
    float maximumDistance;
    short affectSlot;
    void* affect;
    u32 cancelMessage[0x17];

    if (corpse != 0 && corpse->type >= 2
        && EQ_GLOBAL(EQPlayer*, 0x006E4D8C) != 0
        && EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x68 == 0
        && *(int*)((u8*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info + 0x18C) == 0
        && EQ_GLOBAL(u8*, 0x006E4E44) != 0
        && pinstEQ_Main_guess->pendingServerReplies_guess == 0
        && pinstEQ_Main_guess->pendingWorldRequests_guess == 0
        && EQ_GLOBAL(int, 0x006B6590) == 0
        && EQ_GLOBAL(int, 0x006E4DC0) == 0)
    {
        maximumDistance = corpse->height_offset * 1.6f + 14.0f;
        if (EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(corpse, EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
                    EQ_GLOBAL(float, 0x00535194)) > maximumDistance)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), everquest)(EQ_ARRAY(const char, 0x005708CC), 0x0D);
            return 0;
        }

        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis != 0)
        {
            affect = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x0C, &affectSlot);
            EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(affect);
            if (affectSlot != -1)
                EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)affectSlot);

            affect = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x1C, &affectSlot);
            EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(affect);
            if (affectSlot != -1)
                EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)affectSlot);

            affect = EQ_METHOD(2, void*, 0x0042381B, (void*, u8, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x1D, &affectSlot);
            EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(affect);
            if (affectSlot != -1)
                EQ_METHOD(1, void, 0x004251B9, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))((int)affectSlot);

            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis = 0;
            visibilityMessage.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
            visibilityMessage.type = 3;
            visibilityMessage.parameter = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->invis;
            send_message_guess
                (g_world_connection_guess, 0x20F5, &visibilityMessage, 0x0C, 1);
            EQ_METHOD(1, void, 0x00410B68, (void*, EQPlayer*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
        }

        ++pinstEQ_Main_guess->pendingServerReplies_guess;
        if (pinstEQ_Main_guess->pendingServerReplies_guess != 1)
        {
            cancelMessage[0] = 0xFFFFFFFC;
            cancelMessage[1] = pinstEQ_Main_guess->pendingServerReplies_guess;
            cancelMessage[2] = 0x0B;
            send_message_guess
                (g_world_connection_guess, 0x2147, cancelMessage, 0x5C, 1);
        }
        pinstEQ_Main_guess->pendingServerReplies_guess = 1;
        EQ_GLOBAL(EQPlayer*, 0x006E4DC0) = corpse;
        send_message_guess
            (g_world_connection_guess, 0x204E, (u8*)corpse + 0x80, 4, 1);
        return 1;
    }
    return 0;
}
