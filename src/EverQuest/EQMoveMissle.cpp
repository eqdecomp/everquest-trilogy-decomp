#define CDisplay CDisplayType_guess
#include "trilogy/display.h"
#undef CDisplay

#include "trilogy/eqgfx.h"
#include "trilogy/eqmissile.h"
#include "trilogy/globals.h"
#include "trilogy/net_structs.h"
#include "trilogy/spell.h"

// the old file-local view (layouts/eq_missile.json alias)

struct MissileActorData_guess
{
    void* actor;
    u8 unknown0x04[0x44];
    float floorHeight;
    u8 unknown0x4c[0x18];
    u32 hitCooldownTime;
    u32 expirationTime;
    u8 unknown0x6c[0x2d];
    u8 activity;
    u8 unknown0x9a[0xe2];
    u32 lastMoveTime;
    u8 unknown0x180[0x140];
    u8 spellEffectHit;
    u8 deleteAfterCollision;
};

typedef int (__stdcall *SetWindowTextA_guess_t)(void*, const char*);
typedef int (__cdecl *s3dCollideActorWithWorld_guess_t)(
    void*, void*, void*, float*, void*, void*, void*, float*, void*, float*, int);
typedef int (__cdecl *s3dCollideActorWithWorldAlongUnitDirection_guess_t)(
    void*, void*, void*, float*, float, void*, void*, int, float*, float*, void*);
typedef int (__cdecl *t3dGetRegionFromWorldAndXYZ_guess_t)(void*, float*);

#define MISSILE_TINY_POSITIVE EQ_GLOBAL(float, 0x005344A0)
#define MISSILE_TINY_NEGATIVE EQ_GLOBAL(float, 0x005352F4)
#define MISSILE_FLOOR_SENTINEL EQ_GLOBAL(float, 0x005351F4)
#define MISSILE_ZERO EQ_GLOBAL(float, 0x0053510C)
#define MISSILE_TWO_POINT_FIVE EQ_GLOBAL(float, 0x005351AC)
#define MISSILE_NEGATIVE_ONE EQ_GLOBAL(float, 0x005345BC)

namespace CDisplay
{
int __fastcall MoveMissile(CDisplayType_guess* display, int unusedEdx, EQMissile_guess* missile);
}

// FUNCTION: TRILOGY 0x004adac3
// Moves a missile each frame against the world, hits players near a resting spell missile, deletes it on expiry
// ORIGINAL FILE: EverQuest/EQMoveMissle.cpp (from Mac debug map)
int __fastcall CDisplay::MoveMissile(CDisplayType_guess* display, int unusedEdx, EQMissile_guess* missile)
{
    EQMissile_guess* missileFields;
    MissileActorData_guess* actorData;
    EQPlayer* playerIterator;
    EQPlayer* localPlayer;
    float actorLocation[7];
    float savedPosition[3];
    float collisionPosition[3];
    float collisionValues[0x54 / sizeof(float)];
    char debugBuffer[280];
    float collisionDistance;
    float positionDifferenceY;
    float positionDifferenceX;
    float positionDifferenceZ;
    float actorSetLocation[6];
    unsigned int spellId;
    unsigned int currentTime;
    int result;
    char shouldCollide;

    shouldCollide = 1;
    currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
    display->time_ms_guess = currentTime;
    if (missile == 0)
    {
        EQ_CDECL(int, 0x005229E8, (const char*, ...))
            ("** cur_missile is NULL (in MoveMissile)!!!!\n");
        return 1;
    }

    missileFields = (EQMissile_guess*)missile;
    actorData = (MissileActorData_guess*)missile->actor_info;
    EQ_METHOD(0, void, 0x0040101D, (EQMissile_guess*), missile)();
    if (actorData->actor == 0)
    {
        EQ_CDECL(int, 0x005229E8, (const char*, ...))
            ("** the_actor is NULL for cur_missile (in docamai)!!!!\n");
        return 1;
    }

    t3dGetActorLocation(actorData->actor, actorLocation);
    collisionDistance = actorLocation[6];
    savedPosition[0] = missileFields->y;
    savedPosition[1] = missileFields->x;
    savedPosition[2] = missileFields->z;
    collisionPosition[0] = missileFields->y;
    collisionPosition[1] = missileFields->x;
    collisionPosition[2] = missileFields->z;

    if ((missileFields->velocityY >= MISSILE_TINY_POSITIVE || missileFields->velocityY <= MISSILE_TINY_NEGATIVE) ||
        (missileFields->velocityX >= MISSILE_TINY_POSITIVE || missileFields->velocityX <= MISSILE_TINY_NEGATIVE) ||
        (missileFields->velocityZ >= MISSILE_TINY_POSITIVE || missileFields->velocityZ <= MISSILE_TINY_NEGATIVE))
    {
        float floorHeight;

        floorHeight = EQ_METHOD(3, float, 0x00414245,
            (CDisplayType_guess*, float, float, float), display)
            (actorLocation[0], actorLocation[1], actorLocation[2] + EQ_GLOBAL(float, 0x005344A4));
        if (floorHeight != MISSILE_FLOOR_SENTINEL)
            actorData->floorHeight = floorHeight;
    }

    if (EQ_GLOBAL(int, 0x00590168) != 0 && EQ_GLOBAL(void*, 0x006E4DA4) != 0 &&
        EQ_GLOBAL(void*, 0x006E4DA4) == missile)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (debugBuffer, (const char*)0x0056651C,
             (double)actorLocation[0], (double)actorLocation[1], (double)actorLocation[2],
             (double)actorLocation[3], (double)missileFields->flightPitch_guess,
             (double)actorData->floorHeight, (double)missileFields->velocityY,
             (double)missileFields->velocityX, (double)missileFields->velocityZ);
        EQ_GLOBAL(SetWindowTextA_guess_t, 0x00534258)
            ((void*)EQ_GLOBAL(void*, 0x006B63D8), debugBuffer);
    }

    positionDifferenceY = savedPosition[0] - actorLocation[0];
    positionDifferenceX = savedPosition[1] - actorLocation[1];
    positionDifferenceZ = savedPosition[2] - actorLocation[2];
    if (positionDifferenceY < MISSILE_TINY_POSITIVE && positionDifferenceY > MISSILE_TINY_NEGATIVE &&
        positionDifferenceX < MISSILE_TINY_POSITIVE && positionDifferenceX > MISSILE_TINY_NEGATIVE &&
        positionDifferenceZ < MISSILE_TINY_POSITIVE && positionDifferenceZ > MISSILE_TINY_NEGATIVE)
    {
        shouldCollide = 0;
        if (actorData->activity > 4 && actorData->lastMoveTime == 0)
            actorData->lastMoveTime = currentTime;
    }

    if (missileFields->velocityY < MISSILE_TINY_POSITIVE && missileFields->velocityY > MISSILE_TINY_NEGATIVE &&
        missileFields->velocityX < MISSILE_TINY_POSITIVE && missileFields->velocityX > MISSILE_TINY_NEGATIVE &&
        missileFields->velocityZ < MISSILE_TINY_POSITIVE && missileFields->velocityZ > MISSILE_TINY_NEGATIVE)
    {
        if (missileFields->kind == '\t')
        {
            int signedSpellId;
            EQ_Spell_guess* spell;

            signedSpellId = missileFields->spellId;
            if (signedSpellId >= 3000 || signedSpellId < 0)
                goto delete_missile;
            spellId = (unsigned int)signedSpellId;
            spell = g_spells_guess[spellId];
            if (spell == 0)
                goto delete_missile;

            result = EQ_CDECL(char, 0x0048A11F, (void*, int))(spell, 0x4c);
            if (result == 0)
            {
                localPlayer = g_local_player_guess;
                if (EQ_METHOD(7, float, 0x004129F3,
                        (CDisplayType_guess*, float, float, float, float, float, float, float),
                        EQ_GLOBAL(CDisplayType_guess*, 0x006E4DE0))
                        (missileFields->y, missileFields->x, missileFields->z,
                         localPlayer->y, localPlayer->x, localPlayer->z, 0.0f) <=
                        spell->aoe_range_guess &&
                    EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))
                        (localPlayer, missileFields->y, missileFields->x, missileFields->z) != 0 &&
                    actorData->hitCooldownTime < currentTime)
                {
                    actorData->hitCooldownTime = currentTime + 3000;
                    EQ_METHOD(2, void, 0x004AD0E8, (EQMissile_guess*, void*, int), missile)
                        (localPlayer->actor_info->actor, 1);
                }
            }
            else
            {
                actorData->spellEffectHit = 1;
                playerIterator = g_player_top_guess;
                if (actorData->hitCooldownTime < currentTime)
                {
                    actorData->hitCooldownTime = currentTime + 2000;
                    for (; playerIterator != 0; playerIterator = playerIterator->next)
                    {
                        if (playerIterator->speed != 0.0f &&
                            EQ_METHOD(7, float, 0x004129F3,
                                (CDisplayType_guess*, float, float, float, float, float, float, float),
                                EQ_GLOBAL(CDisplayType_guess*, 0x006E4DE0))
                                (missileFields->y, missileFields->x, missileFields->z,
                                 playerIterator->y, playerIterator->x, playerIterator->z, 0.0f) <=
                                spell->aoe_range_guess * MISSILE_TWO_POINT_FIVE &&
                            EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))
                                (playerIterator, missileFields->y, missileFields->x, missileFields->z) != 0)
                        {
                            EQ_METHOD(2, void, 0x004AD0E8, (EQMissile_guess*, void*, int), missile)
                                (playerIterator->actor_info->actor, 1);
                        }
                    }
                }
            }
        }

        if (actorData->expirationTime < currentTime)
            goto delete_missile;
        goto update_render_position;
    }

    if (actorData->expirationTime < currentTime)
        goto delete_missile;

    if (shouldCollide == 0)
    {
        result = 0;
        collisionDistance = 0.0f;
    }
    else
    {
        result = EQ_GLOBAL(s3dCollideActorWithWorld_guess_t, 0x006E50C0)
            (actorData->actor, display->world_guess, display->render_guess,
             collisionPosition, (void*)0x00414159, (void*)0x004ADA49, missile,
             savedPosition, collisionValues, &collisionDistance, 0);
    }

    if (result == 0)
    {
        if (actorData->deleteAfterCollision == 1)
        {
            missileFields->y = savedPosition[0];
            missileFields->x = savedPosition[1];
            missileFields->z = savedPosition[2];
            goto delete_missile;
        }

        if (collisionDistance > MISSILE_ZERO &&
            EQ_GLOBAL(s3dCollideActorWithWorldAlongUnitDirection_guess_t, 0x006E50C4)
                (actorData->actor, display->world_guess, display->render_guess,
                 collisionValues, collisionDistance, (void*)0x00414159, (void*)0x00414147,
                 0, savedPosition, collisionPosition, &playerIterator) != 0)
        {
            missileFields->y = actorLocation[0];
            missileFields->x = actorLocation[1];
            missileFields->z = actorLocation[2];
        }
        else
        {
            missileFields->y = savedPosition[0];
            missileFields->x = savedPosition[1];
            missileFields->z = savedPosition[2];
        }
    }
    else
    {
        missileFields->y = actorLocation[0];
        missileFields->x = actorLocation[1];
        missileFields->z = actorLocation[2];
    }

update_render_position:
    ((float*)((u8*)display + 0x1c))[0] = missileFields->y;
    ((float*)((u8*)display + 0x1c))[1] = missileFields->x;
    ((float*)((u8*)display + 0x1c))[2] = missileFields->z;
    if (EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)
            (display->world_guess, (float*)((u8*)display + 0x1c)) == 0)
    {
        missileFields->velocityY = 0.0f;
        missileFields->velocityX = 0.0f;
        missileFields->velocityZ = 0.0f;
        if (EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)
                (display->world_guess, actorLocation) == 0)
        {
            missileFields->y = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_y;
            missileFields->x = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_x;
            missileFields->z = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_z;
        }
        else
        {
            missileFields->y = actorLocation[0];
            missileFields->x = actorLocation[1];
            missileFields->z = actorLocation[2];
        }
    }

    if (*(void**)((u8*)missile->actor_info + 4) != 0)
        EQ_METHOD(2, void, 0x004152EB, (CDisplayType_guess*, void*, float*), display)
            (*(void**)((u8*)missile->actor_info + 4), &missileFields->y);

    actorSetLocation[0] = missileFields->y;
    actorSetLocation[1] = missileFields->x;
    actorSetLocation[2] = missileFields->z;
    actorSetLocation[3] = missileFields->heading;
    actorSetLocation[4] = missileFields->pitch;
    actorSetLocation[5] = 0.0f;

    positionDifferenceZ = actorLocation[2] - missileFields->z;
    if (positionDifferenceZ < MISSILE_TINY_POSITIVE && positionDifferenceZ > MISSILE_TINY_NEGATIVE &&
        missileFields->velocityZ < MISSILE_NEGATIVE_ONE)
        missileFields->velocityZ = MISSILE_NEGATIVE_ONE;

    result = t3dSetActorLocation(display->world_guess, actorData->actor, actorSetLocation);
    if (result != 0)
        EQ_CDECL(int, 0x005229E8, (const char*, ...))
            ("* t3dSetActorLocation error_code = %d (for missile)\n", result);
    return 1;

delete_missile:
    EQ_METHOD(0, void, 0x004ACEAD, (EQMissile_guess*), missile)();
    EQ_CDECL(void, 0x005218BB, (void*))(missile);
    return 0;
}
