#include "trilogy/display.h"
#include "trilogy/eqgfx.h"

/* eqgfx_dx7.dll entry points this function uses that eqgfx.h doesn't declare yet. */
typedef unsigned int (__cdecl *t3dGetActorUserData_guess_t)(void* actor);
typedef int* (__cdecl *t3dGetActorCollisionVolume_guess_t)(void* actor);
typedef float (__cdecl *t3dGetSphereRadius_guess_t)(int* volume);
#define t3dGetActorUserData_guess           EQ_GLOBAL(t3dGetActorUserData_guess_t, 0x006E528C)
#define t3dGetActorCollisionVolume_guess    EQ_GLOBAL(t3dGetActorCollisionVolume_guess_t, 0x006E526C)
#define t3dGetSphereRadius_guess            EQ_GLOBAL(t3dGetSphereRadius_guess_t, 0x006E52F4)

/* display.h reaches this function through its VA under this name; the definition below replaces it. */
#undef CDisplay_GetOnActor_guess

// FUNCTION: TRILOGY 0x004ae222
// Positions and aligns a player to use a nearby ladder.
// ORIGINAL FILE: EverQuest/EQMovePlayer.cpp (from Mac debug map)
/* Puts the player on the ladder actor (only for an actor whose user data is 6): when the player is already
 * on one it just turns it to the actor's heading; otherwise within 25 units it moves the player to the
 * actor's side (or, failing that, in front of it), clears the velocities, sets state 6 and the floor height. */
void __fastcall CDisplay_GetOnActor_guess(CDisplay* display, int unusedEdx,
                                          void* actor, EQPlayer* player)
{
    float location[7];
    float newY;
    float newX;
    float newZ;
    float heading;
    float radius;
    float boundingRadius;
    float top;
    float diff;
    float floorZ;
    int* volume;
    unsigned int userData;

    if (actor != 0 && player != 0 && (userData = t3dGetActorUserData_guess(actor)) < 0x11 && userData == 6)
    {
        t3dGetActorLocation(actor, location);
        if (*((u8*)player->actor_info + 0x185) == 6)
        {
            diff = EQ_METHOD(3, float, 0x004081A3, (CDisplay*, float, float, float*), display)(location[3], player->heading, 0);
            if (diff < 256.0f)
            {
                player->heading = location[3];
            }
            else
            {
                player->heading = EQ_METHOD(1, float, 0x00408156, (CDisplay*, float), display)(location[3] + 256.0f);
            }
        }
        else
        {
            volume = t3dGetActorCollisionVolume_guess(player->actor_info->actor);
            if (*volume == 0x2a)
                radius = t3dGetSphereRadius_guess(volume);
            else
                radius = 2.0f;
            if (radius < 2.0f)
                radius = 2.0f;
            radius = radius + 0.5f;
            boundingRadius = t3dGetScaledActorBoundingRadius(actor);
            top = location[2] + boundingRadius;
            if (EQ_METHOD(7, float, 0x004129F3, (CDisplay*, float, float, float, float, float, float, float), display)(player->y, player->x, player->z, location[0], location[1], top, 0.0f) > 25.0f)
            {
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))("You are too far away from the ladder, get closer.");
            }
            else if (boundingRadius * 0.7f + location[2] < player->z)
            {
                newZ = 1.5f < player->height_offset ? top - player->height_offset : 1.5f;
                heading = EQ_METHOD(1, float, 0x00408156, (CDisplay*, float), display)(location[3] + 256.0f);
                newY = t3dFloatFastCosine(heading) * radius + location[0];
                newX = t3dFloatFastSine(heading) * radius + location[1];
                if (EQ_METHOD(4, char, 0x004AE535, (CDisplay*, EQPlayer*, float, float, float), display)(player, newY, newX, newZ) != 0)
                {
                    player->y = newY;
                    player->x = newX;
                    player->z = newZ;
                    player->heading = location[3];
                }
                else
                {
                    heading = location[3];
                    newY = t3dFloatFastCosine(heading) * radius + location[0];
                    newX = t3dFloatFastSine(heading) * radius + location[1];
                    if (EQ_METHOD(4, char, 0x004AE535, (CDisplay*, EQPlayer*, float, float, float), display)(player, newY, newX, newZ) == 0)
                        return;
                    player->y = newY;
                    player->x = newX;
                    player->z = newZ;
                    player->heading = EQ_METHOD(1, float, 0x00408156, (CDisplay*, float), display)(location[3] + 256.0f);
                }
                EQ_METHOD(1, void, 0x00410ABF, (CDisplay*, EQPlayer*), display)(player);
                radius = radius + 2.0f;
                player->dy = t3dFloatFastCosine(player->heading) * radius;
                player->dx = t3dFloatFastSine(player->heading) * radius;
                player->unk_0x60 = 0xc2960000;
                player->dz = 0.0f;
                player->speed = 0.0f;
                *((u8*)player->actor_info + 0x185) = 6;
                player->actor_info->ride_target_guess = 0;
                player->actor_info->unk_0x1B8 = 0;
                floorZ = EQ_METHOD(5, float, 0x0041436D, (CDisplay*, EQPlayer*, float, float, float, int), display)(player, player->y, player->x, player->z + 1.0f, 1);
                if (floorZ != -1.0e27f)
                    player->actor_info->floor_z = floorZ;
            }
        }
    }
}

#define STAND_STATE_LEVITATE_GUESS 0x6f
#define STANCE_STAND 100
#define NO_FLOOR -1.0e27f
#define g_ceiling_blocked_guess EQ_GLOBAL(u8, 0x006B640A)
#define g_control_player_guess EQ_GLOBAL(EQPlayer*, 0x006E4DA0)

// FUNCTION: TRILOGY 0x004ae535
// Checks clearance for player movement and restores standing when overhead space permits.
// ORIGINAL FILE: EverQuest/EQMovePlayer.cpp (from Mac debug map)
// Only the controlled player is checked: 0 when there is no floor or ceiling at y/x/z or the gap above the
// floor is too small for the player (state 'o' needs one height, else two), else 1. Also stores the ceiling.
char __fastcall CDisplay_CanIFitHere_guess(CDisplay* display, int unusedEdx,
                                           EQPlayer* player, float y, float x, float z)
{
    float floorZ;
    float ceilingZ;
    float feetZ;
    float headroom;
    float topZ;

    if (player == g_control_player_guess)
    {
        floorZ = EQ_METHOD(5, float, 0x0041436D, (CDisplay*, EQPlayer*, float, float, float, int), display)(player, y, x, z - 0.5f, 0);
        if (floorZ == NO_FLOOR)
            return 0;
        ceilingZ = EQ_METHOD(4, float, 0x00414468, (CDisplay*, float, float, float, float), display)(y, x, z, 0.5f);
        player->actor_info->fallStartZ_guess = ceilingZ;
        if (ceilingZ == NO_FLOOR)
            return 0;
        feetZ = floorZ + player->height_offset;
        if (player->stand_state == STAND_STATE_LEVITATE_GUESS)
        {
            headroom = player->height_offset;
            topZ = feetZ;
        }
        else
        {
            headroom = player->height_offset + player->height_offset;
            topZ = feetZ + player->height_offset;
        }
        if (topZ + 0.5f > ceilingZ)
        {
            g_ceiling_blocked_guess = 1;
            return 0;
        }
        if (g_ceiling_blocked_guess != 0 && feetZ + player->height_offset + 0.5f < ceilingZ)
        {
            g_ceiling_blocked_guess = 0;
            EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), player)(STANCE_STAND);
        }
        if (ceilingZ - floorZ < headroom)
            return 0;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004ae653
// Refreshes the floor height beneath a player at their current position.
// ORIGINAL FILE: EverQuest/EQMovePlayer.cpp (from Mac debug map)

void __fastcall CDisplay_DoFloorCheck_guess(CDisplay* display, int unusedEdx,
                                            EQPlayer* player, int unused2,
                                            int unused3, int unused4)
{
    EQActorInfo_guess* info;
    float floorZ;

    if (display->is3dOn_guess != 0)
    {
        info = player->actor_info;
        if (info != 0 && info->actor != 0)
        {
            info->ride_target_guess = 0;
            floorZ = EQ_METHOD(5, float, 0x0041436D, (CDisplay*, EQPlayer*, float, float, float, int), g_cdisplay_guess)(player, player->y, player->x, player->z, 1);
            if (floorZ != -1.0e27f ||
                (floorZ = EQ_METHOD(5, float, 0x0041436D, (CDisplay*, EQPlayer*, float, float, float, int), g_cdisplay_guess)(player, player->y, player->x, player->z, 1),
                 floorZ != -1.0e27f))
            {
                player->actor_info->floor_z = floorZ;
            }
        }
    }
}

typedef struct MovePlayerInfoState_guess
{
    void* actor;
    u8 unused004[0x64];
    int switchState;
    u8 unused06c[0x20];
    int headRegion;
    int bodyRegion;
    int zoneRegion;
    u8 unused098;
    u8 movementState;
    u8 unused09a[2];
    u8 feetState;
    u8 unused09d[3];
    float movementFactor;
    float movementFactor2;
    u8 unused0a8[0x14];
    float moveAmount;
    u8 unused0c0[0xb0];
    u32 movementTime;
    u8 unused174[8];
    u32 idleTime;
    u8 unused180[0x38];
    void* switchTarget;
    u8 unused1bc[0x4c];
    void* animationTrack;
} MovePlayerInfoState_guess;

typedef int (__cdecl *t3dSetPointLightXYZ_guess_t)(void* world, void* light, float* location);
typedef int (__cdecl *t3dGetRegionFromWorldAndXYZ_guess_t)(void* world, float* location);
typedef int (__cdecl *t3dGetZonesContainingWorldXYZPosition_guess_t)(void* world, float* location, void* zones);
typedef void* (__cdecl *t3dGetZoneUserData_guess_t)(void* zone);
typedef int (__cdecl *t3dGetObjectTag_guess_t)(void* zone, char* tag);
typedef int (__cdecl *s3dCollideActorWithWorld_guess_t)(void* actor, void* world, void* render,
                                                         float* hitLocation, int callback1, int callback2,
                                                         EQPlayer* player, float* oldLocation,
                                                         float* scratchLocation, float* distance,
                                                         int flags);
typedef int (__cdecl *s3dCollideActorWithWorldAlongUnitDirection_guess_t)(
    void* actor, void* world, void* render, float* hitLocation, float distance,
    int callback1, int callback2, EQPlayer* player, float* oldLocation,
    float* scratchLocation, EQPlayer** playerSlot);
typedef int (__stdcall *SetWindowTextA_guess_t)(void* window, const char* text);

class CDisplay_guess : public CDisplay
{
public:
    int MovePlayer(EQPlayer* player);
};

#define MOVE_PLAYER_MAX_RACE 0xF3
#define MOVE_PLAYER_REGION_TAG_LENGTH 0x14
#define MOVE_PLAYER_REGION_ERROR (-0xC2C2)
#define CDisplay CDisplay_guess

// FUNCTION: TRILOGY 0x004AE6DF
// Moves one spawn for the frame (this = CDisplay, the argument is the player); reads and writes the actor state.
// Phases: stamp the time from 0x004a461e and return if the player is null; look up the region and zone tags at
// the position, which can trigger teleports, PvP notices and environment changes.
// Then collides the actor with the world through s3dCollideActorWithWorld, updates floor height, light and
// animation track, and finally places the actor at the resulting location.
// ORIGINAL FILE: EverQuest/EQMovePlayer.cpp (from Mac debug map)
int CDisplay::MovePlayer(EQPlayer* player)
{
    MovePlayerInfoState_guess* actorState;
    EQActorInfo_guess* actorInfo;
    u16 race;
    u32 playerTime;
    u32 actor;
    u32 region;
    int result;
    int zoneCount;
    int zoneIndex;
    int randomValue;
    int movementResult;
    int updateResult;
    int floorState;
    int actorLight;
    u8 environment;
    char teleportRequired;
    char pvpZone;
    char noMove;
    char headingChanged;
    char stopped;
    char collided;
    char tagState;
    float verticalOffset;
    float heightOffset;
    float floorZ;
    float localDistance;
    float collisionDistance;
    float vehicleY;
    float vehicleX;
    float oldY;
    float oldX;
    float oldZ;
    float previousY;
    float previousX;
    float previousZ;
    float deltaY;
    float deltaX;
    float deltaZ;
    float savedHeading;
    float currentDebugDz;
    float* displayLocation;
    void* zones;
    void** zoneList;
    void* zone;
    void* userData;
    void* savedAnimationTrack;
    char debugText[256];
    char teleportTag[256];
    char regionTag[0x21];
    float actorLocation[7];
    float oldActorLocation[7];
    float pointLightLocation[3];

    teleportRequired = 0;
    pvpZone = 0;
    noMove = 0;
    stopped = 0;
    tagState = 0;
    headingChanged = 0;
    environment = 0;
    teleportTag[0] = 0;
    playerTime = EQ_CDECL(u32, 0x004A461E, (void))();
    this->time_ms_guess = playerTime;
    if (player == 0)
    {
        EQ_CDECL(int, 0x005229E8, (const char*, ...))((const char*)0x0056680C);
        EQ_CDECL(void, 0x004C1703, (const char*, ...))((const char*)0x0056680C);
        return 1;
    }

    race = player->race;
    if (race > MOVE_PLAYER_MAX_RACE)
    {
        actorInfo = player->actor_info;
        if ((int)actorInfo->light_guess == 0)
            return 1;
        pointLightLocation[0] = player->y;
        pointLightLocation[1] = player->x;
        pointLightLocation[2] = player->z;
        EQ_GLOBAL(t3dSetPointLightXYZ_guess_t, 0x006E5430)
            (this->world_guess, actorInfo->light_guess, pointLightLocation);
        return 1;
    }

    actorInfo = player->actor_info;
    actorState = (MovePlayerInfoState_guess*)actorInfo;
    actor = (u32)actorInfo->actor;
    heightOffset = player->height_offset;
    localDistance = *(float*)&player->unk_0x78;
    noMove = 1;
    if (race == 0x48 || race == 0x49 || race == 0x8D || race == 0x72 || player->body_type > 0x40)
        noMove = 0;

    if (player->stand_state == 'o')
        verticalOffset = 0.0f;
    else if (race == 2 || (race > 8 && race < 0xB))
        verticalOffset = heightOffset * 0.5f;
    else if (heightOffset < 7.25f)
        verticalOffset = heightOffset * 0.75f;
    else
        verticalOffset = 5.4375f;

    if (actorState->switchTarget != 0)
    {
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
            EQ_METHOD(0, void, 0x004B3FCB, (EQPlayer*), player)();
        else
            actorState->switchTarget = 0;
    }

    if (player->type > 1 && actorState->switchState == 0)
    {
        player->stand_state = 'x';
        player->pitch = 0.0f;
        actorState->switchState = (int)this->time_ms_guess;
        savedAnimationTrack = actorState->animationTrack;
        actorState->animationTrack = 0;
        EQ_METHOD(6, void, 0x0048ED3B,
                  (CDisplay*, EQPlayer*, int, int, int, float, int), this)
            (player, 0x10, 1, 0, 100.5f, 1);
        actorState->animationTrack = savedAnimationTrack;
    }

    if (EQ_GLOBAL(int, 0x005917B0) == 0)
        EQ_CDECL(void, 0x004C1703, (const char*, ...))((const char*)0x005667E4);

    if (actor == 0)
    {
        EQ_CDECL(int, 0x005229E8, (const char*, ...))((const char*)0x005667A8);
        EQ_CDECL(int, 0x005229E8, (const char*, ...))((const char*)0x00566790,
                                                        player->name, player->race, player->gender);
        EQ_CDECL(void, 0x004C1703, (const char*, ...))((const char*)0x005667A8);
        EQ_CDECL(void, 0x004C1703, (const char*, ...))((const char*)0x00566790,
                                                        player->name, player->race, player->gender);
        return 1;
    }

    t3dGetActorLocation((void*)actor, actorLocation);
    previousY = player->y;
    previousX = player->x;
    previousZ = player->z;
    oldY = previousY;
    oldX = previousX;
    oldZ = previousZ;
    for (zoneIndex = 0; zoneIndex != 7; ++zoneIndex)
        oldActorLocation[zoneIndex] = actorLocation[zoneIndex];

    displayLocation = this->cameraLocation_guess;
    displayLocation[0] = player->y;
    displayLocation[1] = player->x;
    displayLocation[2] = (player->z - heightOffset) + EQ_GLOBAL(float, 0x00535154);
    region = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)
        (this->world_guess, displayLocation);
    environment = 0;
    if (region != 0 && region != (u32)actorState->zoneRegion)
    {
        actorState->zoneRegion = region;
        EQ_GLOBAL(t3dGetZonesContainingWorldXYZPosition_guess_t, 0x006E5310)
            (this->world_guess, displayLocation, this->zoneRefList_guess);
        zones = this->zoneRefList_guess;
        zoneCount = *(int*)((char*)zones + 4);
        zoneIndex = 0;
        if (zoneCount != 0)
        {
            do
            {
                zones = this->zoneRefList_guess;
                zoneList = *(void***)((char*)zones + 0xC);
                if (zoneList != 0)
                {
                    zone = zoneList[zoneIndex];
                    userData = EQ_GLOBAL(t3dGetZoneUserData_guess_t, 0x006E5314)(zone);
                    region = EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(zone, regionTag);
                    if ((int)region == MOVE_PLAYER_REGION_ERROR)
                    {
                        EQ_CDECL(int, 0x005229E8, (const char*, ...))((const char*)0x00566778);
                        EQ_CDECL(void, 0x004C1703, (const char*, ...))((const char*)0x00566778);
                    }
                    if (EQ_CDECL(unsigned int, 0x005223D0, (const char*))(regionTag) < MOVE_PLAYER_REGION_TAG_LENGTH &&
                        userData != 0)
                        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(regionTag, (const char*)userData);

                    if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
                            (regionTag, (const char*)0x0053E750, 2) == 0)
                        environment = 0;
                    else if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
                                 (regionTag, (const char*)0x0053E74C, 2) == 0)
                        environment = 5;
                    else if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
                                 (regionTag, (const char*)0x0053E748, 2) == 0)
                        environment = 7;
                    else if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
                                 (regionTag, (const char*)0x0053E744, 2) == 0)
                        environment = 6;
                    else if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
                                 (regionTag, (const char*)0x0053E740, 2) == 0)
                        environment = 8;
                    else if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
                                 (regionTag, (const char*)0x0053E73C, 2) == 0)
                        environment = 9;
                    else if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
                                 (regionTag, (const char*)0x0053E738, 2) == 0)
                        environment = 10;

                    if (regionTag[2] == 'P')
                        pvpZone = 1;
                    if (regionTag[3] == 'T' && regionTag[4] == 'P')
                    {
                        teleportRequired = 1;
                        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(teleportTag, regionTag);
                    }
                    if (regionTag[0x20] == 'S')
                        tagState = 1;
                }
                ++zoneIndex;
                zones = this->zoneRefList_guess;
                zoneCount = *(int*)((char*)zones + 4);
            } while ((u32)zoneIndex < (u32)zoneCount);
        }

        if (tagState == 0)
        {
            CDisplay* globalDisplay = g_cdisplay_guess;
            if (globalDisplay->rainActive == 0)
            {
                if (globalDisplay->snowActive_guess != 0 &&
                    actorState->movementTime < this->time_ms_guess)
                {
                    randomValue = EQ_CDECL(int, 0x004A46F6, (int))(1);
                    if (999 < randomValue)
                        actorState->moveAmount = 0.0f;
                    else
                        actorState->moveAmount = 0.24f;
                    actorState->movementTime = randomValue + 1000 + this->time_ms_guess;
                }
            }
            else if (actorState->movementTime < this->time_ms_guess)
            {
                randomValue = EQ_CDECL(int, 0x004A46F6, (int))(1);
                if (randomValue < 600)
                    actorState->moveAmount = 0.1f;
                else
                    actorState->moveAmount = 0.0f;
                actorState->movementTime = randomValue + 1000 + this->time_ms_guess;
            }
            actorState->movementFactor = EQ_GLOBAL(float, 0x006B640C);
            actorState->movementFactor2 = EQ_GLOBAL(float, 0x006B6410);
        }
        else
        {
            actorState->movementFactor = 0.99f;
            actorState->movementFactor2 = 0.02f;
        }

        if (teleportRequired &&
            EQ_METHOD(3, char, 0x0041496F, (CDisplay*, EQPlayer*, char*, int), this)
                (player, teleportTag, -1) != 0)
            return 1;

        if (pvpZone && player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x00566750);

        actorState->feetState = pvpZone;
        EQ_METHOD(1, void, 0x004B43AB, (EQPlayer*, int), player)(environment);
    }

    displayLocation[0] = player->y;
    displayLocation[1] = player->x;
    displayLocation[2] = player->z;
    result = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)
        (this->world_guess, displayLocation);
    if (result != 0 && result != actorState->bodyRegion)
    {
        actorState->bodyRegion = result;
        environment = EQ_METHOD(4, u8, 0x004147BB,
            (CDisplay*, float, float, float, int*), this)
            (displayLocation[0], displayLocation[1], displayLocation[2], 0);
        EQ_METHOD(1, void, 0x004B4292, (EQPlayer*, int), player)(environment);
    }

    if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
    {
        displayLocation[0] = player->y;
        displayLocation[1] = player->x;
        displayLocation[2] = (player->z - heightOffset) + localDistance;
        result = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)
            (this->world_guess, displayLocation);
        if (result != 0 && result != actorState->headRegion)
        {
            actorState->headRegion = result;
            environment = EQ_METHOD(4, u8, 0x004147BB,
                (CDisplay*, float, float, float, int*), this)
                (displayLocation[0], displayLocation[1], displayLocation[2], 0);
            EQ_METHOD(1, void, 0x004B443E, (EQPlayer*, int), player)(environment);
        }
    }

    if (player->delta_heading != 0.0f || actorLocation[3] != player->heading)
        headingChanged = 1;

    if (EQ_GLOBAL(int, 0x00590168) != 0 && EQ_GLOBAL(int, 0x006E4DA4) == 0 &&
        player == EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
    {
        currentDebugDz = EQ_GLOBAL(float, 0x005FE810);
        if (currentDebugDz <= player->dz)
        {
            if (0.0f < player->dz)
                EQ_GLOBAL(float, 0x005FE810) = 0.0f;
        }
        else
            EQ_GLOBAL(float, 0x005FE810) = player->dz;

        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (debugText, (const char*)0x005666F8,
             (double)actorLocation[0], (double)actorLocation[1], (double)actorLocation[2],
             (double)actorLocation[3], (double)*(float*)&player->unk_0x60,
             (double)actorInfo->floor_z, (double)player->dy, (double)player->dx,
             (double)player->dz, (double)EQ_GLOBAL(float, 0x005FE810));
        EQ_GLOBAL(SetWindowTextA_guess_t, 0x00534258)
            (EQ_GLOBAL(void*, 0x006B63D8), debugText);
    }

    deltaY = previousY - actorLocation[0];
    deltaX = previousX - actorLocation[1];
    deltaZ = previousZ - actorLocation[2];
    if (0.001f <= deltaY || deltaY <= -0.001f ||
        0.001f <= deltaX || deltaX <= -0.001f ||
        0.001f <= deltaZ || deltaZ <= -0.001f)
        actorInfo->unk_0x185 = 0;
    else if (actorState->movementState > 4)
    {
        if (actorState->idleTime == 0)
            actorState->idleTime = this->time_ms_guess;
        if (player->speed == 0.0f)
            stopped = 1;
    }

    if (deltaY < 0.2f && -0.2f < deltaY && deltaX < 0.2f && -0.2f < deltaX &&
        deltaZ < 0.2f && -0.2f < deltaZ && actorState->movementState < 5)
    {
        if (actorState->idleTime == 0)
            actorState->idleTime = this->time_ms_guess;
        if (player->speed == 0.0f)
            stopped = 1;
    }

    if (noMove == 0)
    {
        movementResult = 0;
        collisionDistance = 0.0f;
        floorState = 0;
        if (player->race == 0x8D)
        {
            localDistance = 6.0f;
            if (player->speed < 0.0f)
                localDistance = -11.0f;
            vehicleY = t3dFloatFastCosine(actorLocation[3]) * localDistance + previousY;
            vehicleX = t3dFloatFastSine(actorLocation[3]) * localDistance + previousX;
            floorZ = EQ_METHOD(5, float, 0x0041436D,
                (CDisplay*, EQPlayer*, float, float, float, char), this)
                (player, vehicleY, vehicleX,
                 previousZ + EQ_GLOBAL(float, 0x005344A4), 1);
            if (floorZ == -1.0e27f)
                floorZ = previousZ;
            collided = EQ_METHOD(10, bool, 0x00414523,
                (CDisplay*, float, float, float, float, float, float,
                 float*, float*, float*, int), this)
                (actorLocation[0], actorLocation[1], actorLocation[2],
                 vehicleY, vehicleX, floorZ, &previousY, &previousX, &previousZ, 2);
            environment = EQ_METHOD(4, u8, 0x004147BB,
                (CDisplay*, float, float, float, int*), g_cdisplay_guess)
                (vehicleY, vehicleX, floorZ + 0.01f, 0);
            if (collided || environment == 0)
            {
                EQ_CDECL(void, 0x004E0FC7, (EQPlayer*))(player);
                oldY = oldActorLocation[0];
                player->y = oldActorLocation[0];
                player->heading = oldActorLocation[3];
                oldX = oldActorLocation[1];
                player->x = oldActorLocation[1];
                oldZ = oldActorLocation[2];
                player->z = oldActorLocation[2];
                player->speed = 0.0f;
                player->dy = 0.0f;
                player->dx = 0.0f;
                player->dz = 0.0f;
                EQ_CDECL(void, 0x004E1109, (EQPlayer*))(player);
                movementResult = 1;
            }
            else
                movementResult = 2;
        }

        if (noMove == 0)
        {
            player->y = oldY;
            player->x = oldX;
            player->z = oldZ;
        }
    }
    else
    {
        if (verticalOffset != 0.0f)
        {
            actorLocation[2] += verticalOffset;
            previousZ += verticalOffset;
            t3dSetActorLocation(this->world_guess, (void*)actor, actorLocation);
            actorLocation[2] -= verticalOffset;
        }
        result = EQ_GLOBAL(s3dCollideActorWithWorld_guess_t, 0x006E50C0)
            ((void*)actor, this->world_guess, this->render_guess,
             &oldY, 0x00414159, 0x004AE1B4, player,
             &oldX, displayLocation, &collisionDistance, 0);
        movementResult = result;
    }

    if (noMove != 0 && movementResult == 0)
    {
        if (0.0f < collisionDistance)
        {
            result = EQ_GLOBAL(s3dCollideActorWithWorldAlongUnitDirection_guess_t, 0x006E50C4)
                ((void*)actor, this->world_guess, this->render_guess,
                 displayLocation, collisionDistance, 0x00414159, 0x00414147,
                 player, &oldY, &oldX, &player);
            if (result == 0)
            {
                player->y = oldY;
                player->x = oldX;
                player->z = oldZ;
            }
            else
            {
                player->y = oldActorLocation[0];
                player->x = oldActorLocation[1];
                player->z = oldActorLocation[2];
            }
            movementResult = 1;
        }
        else
        {
            player->y = oldY;
            player->x = oldX;
            player->z = oldZ;
        }
    }
    else if (noMove != 0)
    {
        player->y = oldActorLocation[0];
        player->x = oldActorLocation[1];
        player->z = oldActorLocation[2];
    }

    displayLocation[0] = player->y;
    displayLocation[1] = player->x;
    displayLocation[2] = player->z;
    result = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)
        (this->world_guess, displayLocation);
    if (result == 0 ||
        EQ_METHOD(4, char, 0x004AE535,
            (CDisplay*, EQPlayer*, float, float, float), this)
            (player, displayLocation[0], displayLocation[1], displayLocation[2]) == 0)
    {
        player->y = oldActorLocation[0];
        player->dy = 0.0f;
        player->x = oldActorLocation[1];
        player->dx = 0.0f;
        player->z = oldActorLocation[2];
        player->dz = 0.0f;
    }

    EQ_METHOD(4, void, 0x004AE653,
        (CDisplay*, EQPlayer*, float, float, float), this)
        (player, player->y, player->x, player->z);
    if (noMove != 0 && verticalOffset != 0.0f)
        player->z -= verticalOffset;
    EQ_METHOD(1, void, 0x00414E7A, (CDisplay*, EQPlayer*), this)(player);

    actorLight = (int)player->actor_info->light_guess;
    if (actorLight != 0)
        EQ_METHOD(2, void, 0x004152EB, (CDisplay*, int, float*), this)
            (actorLight, &player->y);

    t3dGetActorLocation((void*)actorInfo->actor, actorLocation);
    previousY = player->y;
    previousX = player->x;
    previousZ = player->z;
    if (player->race == 0x6C)
        savedHeading = EQ_METHOD(1, float, 0x00408156, (CDisplay*, float), this)
            (actorLocation[3] + EQ_GLOBAL(float, 0x00535120));
    else
        savedHeading = player->heading;

    if (oldActorLocation[2] - previousZ < 0.001f &&
        -0.001f < oldActorLocation[2] - previousZ && player->dz < -1.0f)
        player->dz = -1.0f;

    updateResult = t3dSetActorLocation(this->world_guess, (void*)actorInfo->actor, actorLocation);
    if (updateResult != 0)
        EQ_CDECL(int, 0x005229E8, (const char*, ...))((const char*)0x005666B0,
            updateResult, "", (double)previousY, (double)previousX, (double)previousZ,
            (double)actorInfo->floor_z);

    if (this->unk_0x011 == 1 && player->type < 2)
        EQ_METHOD(3, void, 0x00490082, (CDisplay*, EQPlayer*, int, int), this)
            (player, stopped, headingChanged);
    if (actor == (u32)EQ_GLOBAL(int, 0x005917B0))
        EQ_METHOD(1, void, 0x004906FF, (CDisplay*, EQPlayer*), this)(player);

    return 1;
}

#undef CDisplay
