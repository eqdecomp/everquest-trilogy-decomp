// Physics.cpp - spawn movement: vehicles (boats) and the spawns riding them, pushes along the heading.
// ORIGINAL FILE: Source/PC Source/EverQuest/Physics.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/player.h"
#include "trilogy/globals.h"
#include "trilogy/eqplayer.h"
#include "trilogy/physics.h"
#include "trilogy/display.h"

// FUNCTION: TRILOGY 0x00491075
// Applies received movement to a spawn and keeps passengers aligned when that spawn is a vehicle.
int msg_player_physics_guess(u16* msg)
{
    EQPlayer* player;

    if (*(int*)((char*)pinstCEverQuest_guess + 4) == 5)
    {
        player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, *msg);
        if (player == 0)
        {
            send_message_guess
                (g_world_connection_guess, 0x2141, msg, 2, 1);
            return 0;
        }
        EQ_CDECL(void, 0x004E0FC7, (EQPlayer*))(player);
        EQ_CDECL(void, 0x004A3717, (float*, PackedPhysics*, EQPlayer*))
            (&player->y, (PackedPhysics*)(msg + 1), player);
        EQ_CDECL(void, 0x004E1109, (EQPlayer*))(player);
        if (12000 < (u32)(*(int*)((char*)g_displayManager_guess + 0xBC) -
                          *(int*)((char*)player->actor_info + 0x19C)))
        {
            EQ_METHOD(4, void, 0x004AE653, (void*, EQPlayer*, float, float, float), (void*)g_displayManager_guess)(player, player->y, player->x, player->z);
        }
        *(int*)((char*)player->actor_info + 0x19C) =
            *(int*)((char*)g_displayManager_guess + 0xBC);
        EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*), (void*)g_displayManager_guess)(player);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004e0fc7
// Prepares passengers to move with the boat by adjusting their position, facing, and motion relative to it.
// ORIGINAL FILE: Source/PC Source/EverQuest/Physics.cpp (address range)
// static (__cdecl; EBP frame in the original). For a vehicle (race 0x48, 0x49, 0x8D or 0x72): every other
// spawn whose actor_info+0x18C points at it is moved into the vehicle's frame: position relative to the
// vehicle and rotated by its heading/pitch, heading and pitch relative, velocity rotated.
void vehicle_premove(EQPlayer* vehicle)
{
    float m[9];
    EQPlayer* p;

    if (vehicle == 0)
        return;
    if (vehicle->race != 0x48 && vehicle->race != 0x49 && vehicle->race != 0x8D && vehicle->race != 0x72)
        return;
    t3dCalculateAccurateCoefficientsFromHeadingPitchRoll(vehicle->heading, vehicle->pitch, 0.0f, m);
    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->actor_info->ride_target_guess == vehicle && p != vehicle)
        {
            float dy = p->y - vehicle->y;
            float dx = p->x - vehicle->x;
            float dz = p->z - vehicle->z;
            p->y = m[2] * dz + m[1] * dx + m[0] * dy;
            p->x = m[5] * dz + m[4] * dx + m[3] * dy;
            p->z = m[8] * dz + m[7] * dx + m[6] * dy;
            p->heading -= vehicle->heading;
            p->pitch -= vehicle->pitch;
            dy = p->dy;
            dx = p->dx;
            dz = p->dz;
            p->dy = m[2] * dz + m[1] * dx + m[0] * dy;
            p->dx = m[5] * dz + m[4] * dx + m[3] * dy;
            p->dz = m[8] * dz + m[7] * dx + m[6] * dy;
        }
    }
}

// FUNCTION: TRILOGY 0x004e1109
// Updates riders' world positions and facing after their vehicle moves or turns.
// ORIGINAL FILE: Source/PC Source/EverQuest/Physics.cpp (address range)
// static (__cdecl; EBP frame in the original). The inverse of vehicle_premove: the riders' positions are
// rotated back (transposed matrix) and offset by the vehicle, heading and pitch added back (FixHeading),
// velocities rotated back, and the riders' actors moved (EQTRACE when t3dSetActorLocation fails).
void vehicle_postmove(EQPlayer* vehicle)
{
    float location[7];
    float m[9];
    EQPlayer* p;

    if (vehicle == 0)
        return;
    if (vehicle->race != 0x48 && vehicle->race != 0x49 && vehicle->race != 0x8D && vehicle->race != 0x72)
        return;
    t3dCalculateAccurateCoefficientsFromHeadingPitchRoll(vehicle->heading, vehicle->pitch, 0.0f, m);
    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->actor_info->ride_target_guess == vehicle && p != vehicle)
        {
            float nx = m[1] * p->y + m[4] * p->x + m[7] * p->z;
            float nz = m[2] * p->y + m[5] * p->x + m[8] * p->z;
            float ny = m[0] * p->y + m[3] * p->x + m[6] * p->z;
            p->y = ny + vehicle->y;
            p->x = nx + vehicle->x;
            p->z = nz + vehicle->z;
            p->heading = vehicle->heading + p->heading;
            p->heading = CDisplay_FixHeading_guess(g_display_guess, p->heading);
            p->pitch = p->pitch + vehicle->pitch;
            p->pitch = CDisplay_FixHeading_guess(g_display_guess, p->pitch);
            float ndx = m[4] * p->dx + m[7] * p->dz + m[1] * p->dy;
            float ndz = m[5] * p->dx + m[8] * p->dz + m[2] * p->dy;
            float ndy = m[3] * p->dx + m[6] * p->dz + m[0] * p->dy;
            p->dy = ndy;
            p->dx = ndx;
            p->dz = ndz;
            location[0] = p->y;
            location[1] = p->x;
            location[2] = p->z;
            location[3] = p->heading;
            location[4] = p->pitch;
            if (t3dSetActorLocation(g_cdisplay_guess->world_guess, p->actor_info->actor, location) != 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x00576678), p->name);
        }
    }
}

#define PHYSICS_TIME_SCALE EQ_GLOBAL(float, 0x00535130)
#define PHYSICS_MAX_TIME_STEP EQ_GLOBAL(float, 0x005351D0)
#define MISSILE_TINY_POSITIVE EQ_GLOBAL(float, 0x005344A0)
#define PHYSICS_HALF_SPEED EQ_GLOBAL(float, 0x005344A8)
#define PHYSICS_HEADING_HALF_TURN EQ_GLOBAL(float, 0x00535118)
#define PHYSICS_HEADING_PERIOD EQ_GLOBAL(float, 0x00535108)
#define PHYSICS_ZONE_GRAVITY EQ_GLOBAL(float, 0x006B66F0)

// FUNCTION: TRILOGY 0x004e12be
// Advances a spawn, missile or effect by one frame: heading turn, gravity, drag and friction, then position.
// ORIGINAL FILE: EverQuest/Physics.cpp (from Mac debug map)
// @early-stop: prologue register use, stack argument reuse, and later floating point blocks still differ.
void process_physics(EQPlayer* player, int effect, float* physics)
{
    EQActorInfo_guess* actorInfo;
    float* motion;
    float elapsed;
    float forwardMotion;
    float sideMotion;
    float verticalMotion;
    float horizontalDrag;
    float verticalDrag;
    float horizontalFriction;
    float verticalFriction;
    float zoneTop;
    float heightAboveZone;
    float verticalAdjustment;
    float horizontalAdjustment;
    float sineValue;
    float cosineValue;
    float horizontalSpeed;
    u32 currentTime;
    u32 previousTime;
    u32 effectCount;
    s16 spellId;
    s16 levitationAngle;
    s16 levitationTurn;
    s16 movementAngle;
    s16 movementTurn;
    u8 movementClass;
    int wrapCount;

    actorInfo = 0;
    if (player != 0)
    {
        actorInfo = player->actor_info;
        if (actorInfo == 0)
            return;
        effectCount = actorInfo->unk_0x1AC;
        if (effectCount != 0 && g_cdisplay_guess->time_ms_guess > effectCount)
        {
            spellId = actorInfo->unk_0x1B4;
            if (0 < spellId && spellId < 3000)
            {
                EQ_CDECL(void, 0x004C85F4, (int, void*, EQPlayer*, EQPlayer*, int, int, u32))(
                    0, g_spells_guess[spellId], player, player, 0, 0,
                    actorInfo->unk_0x1B0);
            }
            player->actor_info->unk_0x1AC = 0;
            player->actor_info->unk_0x1B4 = -1;
            player->actor_info->unk_0x1B0 = 0;
        }
        if (player->actor_info->lastPhysicsTime_guess < 1)
        {
            currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
            player->actor_info->lastPhysicsTime_guess = currentTime;
            return;
        }
        if (0xF3 < player->race)
            return;
        currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
        previousTime = player->actor_info->lastPhysicsTime_guess;
        elapsed = (float)(currentTime - previousTime) * PHYSICS_TIME_SCALE;
        if (elapsed > 3.0f)
            elapsed = 3.0f;
        if (player == g_local_player_guess)
            EQ_GLOBAL(float, 0x006E2894) = elapsed;
        currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
        player->actor_info->lastPhysicsTime_guess = currentTime;
        actorInfo = player->actor_info;
        motion = &player->y;
        if (player->stand_state == 'x' || *(u32*)((char*)actorInfo + 0x180) == 0x10)
        {
            motion[5] = 0.0f;
            motion[9] = 0.0f;
        }
        if (player->type != 0 && player->race != 0x6C)
            motion[11] = 0.0f;
        EQ_CDECL(void, 0x004E0FC7, (EQPlayer*))(player);
    }
    else if (effect != 0)
    {
        actorInfo = *(EQActorInfo_guess**)((char*)effect + 0x3C);
        if (actorInfo->lastPhysicsTime_guess < 1)
        {
            currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
            actorInfo->lastPhysicsTime_guess = currentTime;
            return;
        }
        currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
        previousTime = actorInfo->lastPhysicsTime_guess;
        elapsed = (float)(currentTime - previousTime) * PHYSICS_TIME_SCALE;
        if (elapsed > 3.0f)
            elapsed = 3.0f;
        currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
        actorInfo->lastPhysicsTime_guess = currentTime;
        motion = (float*)((char*)effect + 0x0C);
    }
    else
    {
        if (physics == 0)
        {
            EQ_CDECL(void, 0x004A3D01, (const char*))(EQ_ARRAY(char, 0x005766B8));
            return;
        }
        if (*(u32*)((char*)physics + 0x74) < 1)
        {
            *(u32*)((char*)physics + 0x74) = EQ_CDECL(u32, 0x004A461E, (void))();
            return;
        }
        currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
        previousTime = *(u32*)((char*)physics + 0x74);
        elapsed = (float)(currentTime - previousTime) * PHYSICS_TIME_SCALE;
        if (elapsed < MISSILE_TINY_POSITIVE)
            elapsed = 0.0f;
        if (elapsed > 3.0f)
            elapsed = 3.0f;
        *(u32*)((char*)physics + 0x74) = EQ_CDECL(u32, 0x004A461E, (void))();
        motion = physics;
    }

    forwardMotion = 0.0f;
    sideMotion = 0.0f;
    verticalMotion = 0.0f;
    horizontalDrag = 0.0f;
    verticalDrag = 0.0f;
    verticalFriction = 0.0f;
    if (EQ_GLOBAL(u8, 0x006B645B) != 0 && player == g_control_player_guess &&
        (motion[9] == 0.0f || motion[5] == 0.0f))
    {
        horizontalAdjustment = EQ_GLOBAL(u8, 0x006B6405) == 0 ? player->walk_speed : player->run_speed;
        horizontalAdjustment = horizontalAdjustment * 0.5f;
        if (motion[5] == 0.0f)
            horizontalAdjustment = EQ_GLOBAL(float, 0x006B711C) * horizontalAdjustment;
        if (EQ_GLOBAL(u8, 0x006B645B) == 1)
            sineValue = CDisplay_FixHeading_guess(g_display_guess, motion[3] + PHYSICS_HEADING_HALF_TURN);
        else
            sineValue = CDisplay_FixHeading_guess(g_display_guess, motion[3] - PHYSICS_HEADING_HALF_TURN);
        cosineValue = t3dFloatFastCosine(sineValue);
        forwardMotion = cosineValue * horizontalAdjustment;
        sideMotion = t3dFloatFastSine(sineValue) * horizontalAdjustment;
    }

    wrapCount = 0;
    motion[3] = elapsed * motion[9] + motion[3];
    while (PHYSICS_HEADING_PERIOD <= motion[3])
    {
        motion[3] = motion[3] - PHYSICS_HEADING_PERIOD;
        if (5 < wrapCount)
        {
            motion[3] = 0.0f;
            break;
        }
        wrapCount = wrapCount + 1;
    }
    wrapCount = 0;
    while (motion[3] < 0.0f)
    {
        motion[3] = motion[3] + PHYSICS_HEADING_PERIOD;
        if (5 < wrapCount)
        {
            motion[3] = 0.0f;
            break;
        }
        wrapCount = wrapCount + 1;
    }

    verticalAdjustment = elapsed * motion[10] + motion[4];
    motion[4] = verticalAdjustment;
    if (verticalAdjustment < EQ_GLOBAL(float, 0x00535748))
        motion[4] = -128.0f;
    if (motion[4] > PHYSICS_HEADING_HALF_TURN)
        motion[4] = 128.0f;
    if (physics != 0 && *(u16*)((char*)physics + 0x7C) == 2)
        motion[0x0B] = motion[4];

    if (motion[5] != 0.0f)
    {
        cosineValue = t3dFloatFastCosine(motion[3]);
        forwardMotion = cosineValue * motion[5] + forwardMotion;
        sineValue = t3dFloatFastSine(motion[3]);
        sideMotion = sineValue * motion[5] + sideMotion;
        if (motion[0x0B] != 0.0f)
        {
            sineValue = t3dFloatFastSine(motion[0x0B]);
            sineValue = motion[5] * sineValue;
            if (player != 0 && player->levitate == 2 && actorInfo->unknown0x099 == 0 &&
                0.0f < sineValue)
                sineValue = 0.0f;
            verticalMotion = sineValue;
        }
    }

    if (actorInfo == 0 || *(&actorInfo->unk_0x185) != 6)
    {
        if (physics == 0 && actorInfo->unknown0x088 == 1)
        {
            motion[8] = motion[8] - PHYSICS_ZONE_GRAVITY * elapsed;
            movementClass = actorInfo->unknown0x099;
            if (movementClass == 0)
            {
                if ((player == 0 ||
                     EQ_METHOD(0, bool, 0x004B41A6, (EQPlayer*), player)() == 0) &&
                    *(&actorInfo->unk_0x185) < 3)
                {
                    horizontalDrag = 0.0f;
                    verticalDrag = 0.0f;
                    horizontalFriction = 0.99f;
                    verticalFriction = 0.99f;
                }
                else
                {
                    horizontalDrag = actorInfo->horizontalDrag_guess;
                    verticalFriction = 0.99f;
                    horizontalFriction = actorInfo->horizontalFriction_guess;
                    verticalDrag = 0.0f;
                }
            }
            else if (movementClass == 5)
            {
                horizontalDrag = 0.025f;
                verticalDrag = 0.8f;
                horizontalFriction = 0.95f;
                verticalFriction = 0.2f;
            }
            else if (movementClass == 6 || movementClass == 7 || movementClass == 8)
            {
                horizontalDrag = 0.02f;
                verticalDrag = 0.8f;
                horizontalFriction = 0.94f;
                verticalFriction = 0.15f;
            }
            else if (movementClass == 9 || movementClass == 10)
            {
                horizontalDrag = 0.025f;
                verticalDrag = 0.8f;
                horizontalFriction = 0.95f;
                verticalFriction = 0.2f;
            }
        }
        else
        {
            horizontalDrag = 0.8f;
            horizontalFriction = 0.625f;
            verticalFriction = 0.625f;
            if (player == 0 || player->levitate != 2)
            {
                verticalDrag = 0.8f;
            }
            else
            {
                movementClass = actorInfo->unknown0x099;
                if (player->race != 0x8D && movementClass != 0)
                {
                    if (movementClass == 5 || movementClass == 9 || movementClass == 10)
                    {
                        horizontalDrag = 0.025f;
                        horizontalFriction = 0.95f;
                        verticalFriction = 0.2f;
                    }
                    else if (movementClass == 6 || movementClass == 7 || movementClass == 8)
                    {
                        horizontalDrag = 0.02f;
                        horizontalFriction = 0.94f;
                        verticalFriction = 0.15f;
                    }
                }
                verticalDrag = 0.8f;
                zoneTop = EQ_METHOD(3, float, 0x00414705, (void*, float, float, float), g_display_guess)(
                    motion[0], motion[1], actorInfo->floor_z);
                if (zoneTop == -1.0e27f)
                    zoneTop = actorInfo->floor_z;
                heightAboveZone = motion[2] - zoneTop;
                if (heightAboveZone <= 15.0f)
                {
                    if (player->race != 0x8D)
                    /* fake match: original jumps directly to shared drag setup. */
                        goto physics_vertical_drag_ready;
                    if (heightAboveZone <= 0.4f)
                    /* fake match: original skips the floor adjustment at this boundary. */
                        goto physics_vertical_drag_ready;
                    verticalAdjustment = elapsed * 0.1f;
                }
                else
                {
                    verticalAdjustment = 0.0f;
                    if (motion[5] != 0.0f)
                        verticalAdjustment = 0.05f;
                    if (heightAboveZone <= 60.0f)
                    {
                        if (heightAboveZone <= 30.0f)
                            verticalAdjustment = elapsed * 0.1f;
                        else
                            verticalAdjustment = (verticalAdjustment + 0.2f) * elapsed;
                    }
                    else
                    {
                        verticalAdjustment = (verticalAdjustment + 0.3f) * elapsed;
                    }
                }
                motion[8] = motion[8] - verticalAdjustment;
            }
        }
    }
    else
    {
        horizontalDrag = 0.8f;
        verticalDrag = 0.8f;
        horizontalFriction = 0.625f;
        verticalFriction = 0.625f;
    }

physics_vertical_drag_ready:
    if (forwardMotion != 0.0f)
        forwardMotion = horizontalDrag * forwardMotion;
    if (sideMotion != 0.0f)
        sideMotion = horizontalDrag * sideMotion;
    if (verticalMotion != 0.0f)
        verticalMotion = verticalDrag * verticalMotion;
    if (player == 0)
    {
        if (effect != 0 && *((char*)effect + 0x52) == 0)
        {
            horizontalFriction = 0.85f;
            verticalFriction = 0.85f;
        }
    }
    else
    {
        if (player->race == 0x48)
        {
            verticalMotion = 0.0f;
            motion[8] = 0.0f;
        }
        if (player->type == 1 && (motion[6] != 0.0f || motion[7] != 0.0f))
            horizontalFriction = horizontalFriction * 0.5f;
    }
    if (motion[6] != 0.0f)
        motion[6] = horizontalFriction * motion[6];
    if (motion[7] != 0.0f)
        motion[7] = horizontalFriction * motion[7];
    if (motion[8] != 0.0f)
        motion[8] = verticalFriction * motion[8];
    motion[6] = forwardMotion + motion[6];
    motion[7] = sideMotion + motion[7];
    motion[8] = verticalMotion + motion[8];

    if (effect == 0)
    {
        if (player != 0 && player->type == 0 && actorInfo->unknown0x099 == 0 &&
            (*(u32*)((char*)actorInfo + 0x180) == 0x19 ||
             *(u32*)((char*)actorInfo + 0x180) == 0x25) &&
            motion[8] > 0.0f && motion[8] < 1.0f)
            motion[8] = 0.0f;
    }
    else
    {
        horizontalSpeed = t3dFloatSqrt(motion[6] * motion[6] + motion[7] * motion[7]);
        if (*((char*)effect + 0x52) == 0 && horizontalSpeed < 0.05f)
        {
            motion[6] = 0.0f;
            motion[7] = 0.0f;
        }
    }

    motion[0] = elapsed * motion[6] + motion[0];
    motion[1] = elapsed * motion[7] + motion[1];
    motion[2] = elapsed * motion[8] + motion[2];
    if (actorInfo != 0)
    {
        if (-1 < actorInfo->unknown0x078)
        {
            if (0x200 < actorInfo->unknown0x078)
                actorInfo->unknown0x078 = 0;
            levitationAngle = actorInfo->unknown0x078;
            sineValue = t3dFloatFastSine((float)(int)levitationAngle);
            motion[2] = (sineValue * actorInfo->unk_0x84_float_guess) * elapsed + motion[2];
            levitationTurn = *(s16*)((char*)actorInfo + 0x7C);
            actorInfo->unknown0x078 = actorInfo->unknown0x078 + levitationTurn;
        }
        movementAngle = actorInfo->unk_0x7A;
        if (-1 < movementAngle && (motion[6] != 0.0f || motion[7] != 0.0f))
        {
            if (0x200 < movementAngle)
                actorInfo->unk_0x7A = 0;
            movementAngle = actorInfo->unk_0x7A;
            sineValue = t3dFloatFastSine((float)(int)movementAngle);
            horizontalAdjustment = sineValue * actorInfo->unk_0x80;
            sineValue = CDisplay_FixHeading_guess(g_display_guess, motion[3] + PHYSICS_HEADING_HALF_TURN);
            cosineValue = t3dFloatFastCosine(sineValue);
            motion[0] = (cosineValue * horizontalAdjustment) + motion[0];
            sineValue = t3dFloatFastSine(sineValue);
            motion[1] = (sineValue * horizontalAdjustment) + motion[1];
            movementTurn = actorInfo->unk_0x7E;
            actorInfo->unk_0x7A = actorInfo->unk_0x7A + movementTurn;
        }
    }
    if (player != 0)
    {
        EQ_METHOD(1, void, 0x00415083, (void*, EQPlayer*), g_display_guess)(player);
        EQ_CDECL(void, 0x004E1109, (EQPlayer*))(player);
    }
    return;
}

// FUNCTION: TRILOGY 0x004e1c10
// ORIGINAL FILE: Source/PC Source/EverQuest/Physics.cpp (address range)
// static (__cdecl). Adds `distance` along the spawn's heading to its dy (cosine) and dx (sine).
void push_along_heading(EQPlayer* player, float distance)
{
    player->dy += t3dFloatFastCosine(player->heading) * distance;
    player->dx += t3dFloatFastSine(player->heading) * distance;
}
