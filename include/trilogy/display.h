/*
 * display.h - CDisplay, the client's renderer front end (Mac: Source/PC Source/EverQuest/Display.cpp),
 * plus the Display.cpp globals the rewritten functions touch.
 *
 * Layout: partial, from the rewritten functions only (offsets are what the code reads; the names are
 * guesses). Total size not recovered, so there is no sizeof check: do not instantiate CDisplay or take
 * sizeof() until the layout is filled in. Field offsets are checked with C_ASSERT below.
 *
 * Calling conventions: methods whose callers load ECX and that end in `ret N` are __thiscall members,
 * even when the body never reads `this` (FixHeading, HeadingDiff, the distance functions, ...).
 * `ret` with no stack pop and no use of ECX = static member (__cdecl).
 *
 * C++ only (VC6). Not included from trilogy.h, which build_gdt.py parses as C.
 */
#ifndef TRILOGY_DISPLAY_H
#define TRILOGY_DISPLAY_H

#include "common.h"
#include "eqaddr.h"
#include "player.h"

struct EQSwitch;
struct CharacterProfile;

/* g_userDefinedColors_guess (0x0058FBBC): 34 user colour slots, 4 bytes each, alpha unused here. */
typedef struct UserColor_guess
{
    u8      r;                  /* 0x0 */
    u8      g;                  /* 0x1 */
    u8      b;                  /* 0x2 */
    u8      unused;             /* 0x3 */
} UserColor_guess;
C_ASSERT(sizeof(UserColor_guess) == 4);

/* The game clock (*(EQWorldTime_guess**)0x006E4D54). SetDayPeriod_guess (0x00412445) writes the hour, clears
 * the minute, stamps both clocks and copies 0x0..0x5 to 0x6..0xB. */
typedef struct EQWorldTime_guess
{
    u8      hour;               /* 0x0  compared against 5..18 for day/night doors */
    u8      minute_guess;       /* 0x1 */
    u8      day_guess;          /* 0x2 */
    u8      month_guess;        /* 0x3 */
    u16     year_guess;         /* 0x4 */
    u8      saved_hour_guess;   /* 0x6  copies of 0x0..0x5 */
    u8      saved_minute_guess; /* 0x7 */
    u8      saved_day_guess;    /* 0x8 */
    u8      saved_month_guess;  /* 0x9 */
    u16     saved_year_guess;   /* 0xA */
    u32     set_time_ms_guess;  /* 0xC  EqGetTime() when set */
    u32     set_time_guess;     /* 0x10 time(0) when set */
} EQWorldTime_guess;
C_ASSERT(offsetof(EQWorldTime_guess, saved_year_guess) == 0xA);
C_ASSERT(offsetof(EQWorldTime_guess, set_time_guess) == 0x10);

/* A t3d actor (renderer object); only the fields the client reads. */
typedef struct T3dActor_guess
{
    u8      unknown0x00[0x28];  /* 0x00 */
    void*   region_guess;       /* 0x28  passed to t3dUpdateTouchedRegions */
    u8      unknown0x2C[0x38];  /* 0x2C */
    void*   definition_guess;   /* 0x64  passed to _t3dComputeActorDefinitionBoundingRadius */
} T3dActor_guess;
C_ASSERT(offsetof(T3dActor_guess, definition_guess) == 0x64);

/* 0x005917B8 / 0x005917BC  name sprites shown for players / for NPCs (Turn*NameSprites*, UpdateNameSprite). */
#define g_pc_names_on_guess         EQ_GLOBAL(int, 0x005917B8)
#define g_npc_names_flag_guess      EQ_GLOBAL(int, 0x005917BC)

/* 3-byte tint handed to t3dSetBlitSpriteTint: the caller's {r, g, b} reversed. */
typedef struct BlitTint_guess
{
    u8      b;                  /* 0x0 */
    u8      g;                  /* 0x1 */
    u8      r;                  /* 0x2 */
} BlitTint_guess;
C_ASSERT(sizeof(BlitTint_guess) == 3);

#define NUM_USER_COLORS_GUESS       34
#define g_userDefinedColors_guess   EQ_ARRAY(UserColor_guess, 0x0058FBBC)
#define g_skyLayer_guess            EQ_ARRAY(void, 0x0058FC70)      /* passed by address to t3dSetSkyDualLayer */
#define g_bitsPerPixel_guess        EQ_GLOBAL(int, 0x0053C4C0)      /* .data initial value 16 */
#define g_pWorldTime_guess          EQ_GLOBAL(EQWorldTime_guess*, 0x006E4D54)
#define g_pcloudFlagA_guess         EQ_GLOBAL(int, 0x006E205C)
#define g_pcloudFlagB_guess         EQ_GLOBAL(int, 0x006E2060)

/* 0x006B6F18  the five default camera setups (default_cameras_guess), 0x1C bytes each. Field roles unknown. */
typedef struct DefaultCamera_guess
{
    f32     value[6];           /* 0x00 */
    u8      flag;               /* 0x18 */
    u8      unused[3];          /* 0x19 */
} DefaultCamera_guess;
C_ASSERT(sizeof(DefaultCamera_guess) == 0x1C);

#define g_default_cameras_guess     EQ_ARRAY(DefaultCamera_guess, 0x006B6F18)
#define NUM_DEFAULT_CAMERAS_GUESS   5
/* 0x006B6504  the video mode, 1..4 (640x480 .. 1600x1200): offsets the render window and overlays. */
#define g_video_mode_guess          EQ_GLOBAL(int, 0x006B6504)
/* 0x006B63E0  the render window rectangle (left, top, right, bottom), shorts. */
#define g_render_rect_guess         EQ_ARRAY(s16, 0x006B63E0)
/* 0x006EFC70  byte flag: nonzero = OffsetRenderWindow leaves the camera window alone. */
#define g_no_render_offset_guess    EQ_GLOBAL(char, 0x006EFC70)
/* 0x0058FC6C  the game state (220 xrefs): DrawOverlay offsets overlays in states 1, 5 and 6. */
#define g_game_state_guess          EQ_GLOBAL(int, 0x0058FC6C)
/* 0x0053C670  10 RGB tints (u32 each) for SetPlayerAttachmentTint_guess. */
#define g_attachment_tints_guess    EQ_ARRAY(u32, 0x0053C670)
/* 0x005766E0  ambient light level per weather index (CDisplay +0xB8), floats. */
#define g_weather_ambient_guess     EQ_ARRAY(float, 0x005766E0)
/* 0x005917B0  the actor the camera follows (SetViewActor makes the old one visible again). */
#define g_view_actor_guess          EQ_GLOBAL(void *, 0x005917B0)
/* 0x006D6FE4  the zone id; zone 0x79 forces a light radius of 25 (CreateLight). */
#define g_zone_id_guess             EQ_GLOBAL(int, 0x006D6FE4)

/* DoPoly's colour as handed to t3dDefer2DQuad. */
typedef struct QuadColor_guess
{
    u8      b;                  /* 0x0 */
    u8      g;                  /* 0x1 */
    u8      r;                  /* 0x2 */
    u8      a;                  /* 0x3  always 0 */
} QuadColor_guess;
C_ASSERT(sizeof(QuadColor_guess) == 4);

/* Untyped views of the sound manager (0x006E4DD8, soundmanager.h g_sound_manager_guess) and the sound
 * environment (0x006E4DDC, eq_character.h g_environment_guess), so Display.cpp needs neither class. */
#define g_sound_manager_ptr_guess   EQ_GLOBAL(void *, 0x006E4DD8)
#define g_environment_ptr_guess     EQ_GLOBAL(void *, 0x006E4DDC)

class CDisplay;

/* 0x006E4DE0  the CDisplay instance (globals.h has it untyped as DAT_006e4de0 / g_display_guess). */
#define g_cdisplay_guess            EQ_GLOBAL(CDisplay *, 0x006E4DE0)

/* CDisplay methods not rewritten yet, __thiscall through their fixed VA. */
/* 0x00410D13  sound at (y, x, z), id; callers pass the CDisplay instance in ECX, although the body
 * ignores it. The queue's EQPlayer::PlaySoundA name is a medium-confidence Mac match. */
#define CDisplay_PlaySoundA_guess(y, x, z, id) \
    EQ_METHOD(4, void, 0x00410D13, (CDisplay *, float, float, float, int), g_cdisplay_guess)((y), (x), (z), (id))
#define CDisplay_UpdateItemSlot4_guess(d, player, slot, name, extra) \
    EQ_METHOD(4, void, 0x0040B6F6, (CDisplay *, EQPlayer *, int, char *, int), (d))((player), (slot), (name), (extra))
#define CDisplay_SetSunLight_guess(d) \
    EQ_METHOD(0, void, 0x0040D84F, (CDisplay *), (d))()
#define CDisplay_LightningStrike_guess(d) \
    EQ_METHOD(0, void, 0x0040DAB0, (CDisplay *), (d))()
#define CDisplay_ProcessCloud_guess(d) \
    EQ_METHOD(0, void, 0x0040DFB3, (CDisplay *), (d))()
#define CDisplay_ChangeVideoMode_guess(d, a) \
    EQ_METHOD(1, void, 0x0040E58B, (CDisplay *, int), (d))((a))
#define CDisplay_UpdatePlayerVisibility_guess(d, player) \
    EQ_METHOD(1, void, 0x00410B68, (CDisplay *, EQPlayer *), (d))((player))
#define CDisplay_SetDayPeriod_guess(d, a) \
    EQ_METHOD(1, void, 0x00412445, (CDisplay *, int), (d))((a))
#define CDisplay_SetAmbientLight_guess(d, level) \
    EQ_METHOD(1, void, 0x00412D2C, (CDisplay *, float), (d))((level))
#define CDisplay_SetNameSpriteState_guess(d, player, on) \
    EQ_METHOD(2, void, 0x00411BC8, (CDisplay *, EQPlayer *, int), (d))((player), (on))
#define CDisplay_SetSkyBackground_guess(d) \
    EQ_METHOD(0, void, 0x004124DB, (CDisplay *), (d))()
#define CDisplay_GetEnvironment_guess(d, y, x, z, out) \
    EQ_METHOD(4, u8, 0x004147BB, (CDisplay *, float, float, float, int *), (d))((y), (x), (z), (out))
/* 0x00414523  s3dCollideSphereWithWorld (Mac, high), __thiscall, 10 arguments, returns a bool (AL). */
#define CDisplay_CollideSphereWithWorld_guess(d, y1, x1, z1, y2, x2, z2, a, b, c, flags) \
    EQ_THISCALL(bool, 0x00414523, (CDisplay *, int, float, float, float, float, float, float, int, int, int, int)) \
        ((d), 0, (y1), (x1), (z1), (y2), (x2), (z2), (a), (b), (c), (flags))
#define CDisplay_PlayAnimationTrack_guess(d, player, anim, a, b, speed, c) \
    EQ_METHOD(6, void, 0x0048ED3B, (CDisplay *, EQPlayer *, int, int, int, float, int), (d))((player), (anim), (a), (b), (speed), (c))
#define CDisplay_GetOnActor_guess(d, actor, player) \
    EQ_METHOD(2, void, 0x004AE222, (CDisplay *, void *, EQPlayer *), (d))((actor), (player))
/* 0x0042577F  EQ_Character::CanISeeInvis (Mac, high), __thiscall, returns a bool (AL). */
#define EQ_Character_CanISeeInvis_guess(ch) \
    EQ_METHOD(0, bool, 0x0042577F, (void *), (void *)(ch))()
/* 0x004E286F  __thiscall on *(void **)0x006E4DDC (g_environment_guess): a sound at (y, x, z), C++ EH. */
#define Environment_PlaySoundAt_guess(env, y, x, z, id) \
    EQ_METHOD(4, void, 0x004E286F, (void *, float, float, float, int), (void *)(env))((y), (x), (z), (id))
/* 0x004E363B  __thiscall on the sound manager (0x006E4DD8), one int: works on the wave slot +0xDB0 + 4 * id. */
#define SoundManager_StopWave_guess(mgr, id) \
    EQ_METHOD(1, void, 0x004E363B, (void *, int), (void *)(mgr))((id))

class CDisplay
{
public:
    void*   render_guess;       /* 0x0000  passed to t3dUpdateDisplay / t3dSetBlitSpriteTint */
    void*   world_guess;        /* 0x0004  passed to t3dDestroyPointLight / t3dDestroyParticleCloud */
    void*   camera_guess;       /* 0x0008  t3dSetCameraRenderWindow / t3dSetCameraLocation / s3dSetCameraActorYon */
    int     palette_failed_guess; /* 0x000C  set to 1 when t3dCreatePalette fails */
    char    fogEnabled_guess;   /* 0x0010* */
    u8      unk_0x011;          /* 0x0011* */
    u8      skyLayerMode_guess; /* 0x0012* */
    u8      unknown0x0013[1];   /* 0x0013  */
    union
    {
        u32     skyType_guess;      /* 0x0014* */
        u8      skyType_byte_guess; /* 0x0014* */
    };
    f32     yon;          /* 0x0018  SetYon_guess */
    union
    {
        float   cameraLocation_guess[6]; /* 0x001C* */
        u32     cameraLocation_bits_guess[6]; /* 0x001C* */
    };
    u8      unknown0x0034[4];   /* 0x0034  */
    union
    {
        int     ambientLight_bits_guess; /* 0x0038* */
        float   ambientLight_guess; /* 0x0038* */
    };
    char    itemOnCursor_guess; /* 0x003C* */
    u8      moneyOnCursor_guess; /* 0x003D* */
    char    dragArmed_guess;    /* 0x003E* */
    u8      unknown0x003F[1];   /* 0x003F  */
    char    refList40_guess[0x10]; /* 0x0040* */
    void*   bmInfo50_guess;     /* 0x0050* */
    u8      unknown0x0054[0x24]; /* 0x0054  */
    void*   boxDrawOwner_guess; /* 0x0078  box draw owner (Shutdown3DDisplay_guess frees it at shutdown) */
    int     unk_0x07C;      /* 0x007C  passed to FUN_0040296e */
    void*   fastFileHandle_guess; /* 0x0080  t3dFastFileInit handle; CreateWorldPalette_guess takes it */
    int     fastFileHandle2_guess; /* 0x0084* */
    int     fastFileHandle3_guess; /* 0x0088* */
    int     fastFileHandle4_guess; /* 0x008C* */
    int     fastFileHandle5_guess; /* 0x0090* */
    int     fastFileHandle6_guess; /* 0x0094* */
    u8      unk_0x098;      /* 0x0098  set to 0 by InitDDraw_guess */
    u8      unknown0x0099[3];   /* 0x0099  */
    int     fogMode_guess;      /* 0x009C* */
    float   fogStart_guess;     /* 0x00A0* */
    float   fogEnd_guess;       /* 0x00A4* */
    char    fogRed_guess;       /* 0x00A8* */
    char    fogGreen_guess;     /* 0x00A9* */
    char    fogBlue_guess;      /* 0x00AA* */
    u8      unknown0x00AB[0xD]; /* 0x00AB  */
    u8      weather_ambient_index_guess; /* 0x00B8  index into g_weather_ambient_guess */
    u8      unknown0x00B9[3];   /* 0x00B9 */
    u32     time_ms_guess;      /* 0x00BC  current time in ms (ToggleAutoAttack, StartWeather) */
    char    weather_active_guess; /* 0x00C0 */
    u8      unk_0x0C1;      /* 0x00C1 */
    u8      weather_c2_guess;   /* 0x00C2 */
    u8      alwaysOpenDoors_guess; /* 0x00C3  nonzero: doors with param 2.0 stay open in daytime */
    u8      weather_clouds_guess; /* 0x00C4  nonzero: UpdateWeather processes clouds, else lightning */
    u8      weather_c5_guess;   /* 0x00C5  1 while starting, 0 once stopping */
    u8      weather_type_guess; /* 0x00C6 */
    u8      unknown0x00C7;      /* 0x00C7 */
    s16     weather_fade_guess; /* 0x00C8  0..0x100 */
    u8      unknown0x00CA[2];   /* 0x00CA */
    f32     weather_cc_guess;   /* 0x00CC */
    f32     weather_d0_guess;   /* 0x00D0 */
    u32     weather_d4_time_guess; /* 0x00D4 */
    u32     unknown0x00D8;      /* 0x00D8 */
    u32     weather_dc_time_guess; /* 0x00DC */
    int     rainActive;   /* 0x00E0* */
    int     snowActive_guess;   /* 0x00E4* */
    void*   snowSprite_guess;   /* 0x00E8* */
    void*   rainSprite_guess;   /* 0x00EC* */
    void*   rainCloudDef0_guess; /* 0x00F0* */
    void*   rainCloud0_guess;   /* 0x00F4* */
    void*   rainCloudDef1_guess; /* 0x00F8* */
    void*   rainCloud1_guess;   /* 0x00FC* */
    void*   rainCloudDef2_guess; /* 0x0100* */
    void*   rainCloud2_guess;   /* 0x0104* */
    void*   snowCloudDef0_guess; /* 0x0108* */
    void*   snowCloud0_guess;   /* 0x010C* */
    void*   snowCloudDef1_guess; /* 0x0110* */
    void*   snowCloud1_guess;   /* 0x0114* */
    void*   snowCloudDef2_guess; /* 0x0118* */
    void*   snowCloud2_guess;   /* 0x011C* */
    void*   blood_cloud_def_guess; /* 0x0120  particle cloud definition "BSPURT_PCD" (SpurtBloodOnDag) */
    u32     unk_0x124;      /* 0x0124  cleared by SetViewActor */
    u8      unknown0x0128[4];   /* 0x0128  */
    void*   allocatedBlock12C_guess; /* 0x012C* */
    u8      unknown0x0130[0x2718]; /* 0x0130  */
    void*   light_defs_guess[9];/* 0x2848  light definitions by light type (CreateLight) */
    u8      unknown0x286C[0x10];/* 0x286C */
    void*   hwnd_guess;         /* 0x287C  window handle (t3dInitializeDevice, t3dCreateWorld) */
    f32     view_angle_guess;   /* 0x2880  SetViewAngle_guess */
    u8      unknown0x2884[8];   /* 0x2884  */
    float   nearClip_guess;     /* 0x288C* */
    u8      unknown0x2890[0x400]; /* 0x2890  */
    void*   palette_guess;      /* 0x2C90  t3dCreatePalette */
    int     is3dOn_guess;       /* 0x2C94  CDisplay::is_3dON_guess */
    int     cleanupPending_guess; /* 0x2C98  shutdown (Shutdown3DDisplay_guess) runs FUN_0040AFA1 while this is set */
    int     render_window_guess[4]; /* 0x2C9C  left, top, right, bottom for t3dSetCameraRenderWindow */
    void*   collisionSphere_guess; /* 0x2CAC* */
    float   collisionSphereScale_guess; /* 0x2CB0* */
    void*   light_def_default_guess; /* 0x2CB4  light definition for light type 0 */
    void*   zoneRefList_guess;  /* 0x2CB8* */
    char    visibleActorList_guess[0x10]; /* 0x2CBC* */
    u8      unknown0x2CCC[4];   /* 0x2CCC  */
    int     overlayBmInfo_guess; /* 0x2CD0* */
    u8      unknown0x2CD4[0x24]; /* 0x2CD4  */
    u8      worldDisplayActive; /* 0x2CF8* */
    char    animFlagsA_guess[0x4A]; /* 0x2CF9* */
    char    animFlagsB_guess[0x4A]; /* 0x2D43* */
    char    animFlagsC_guess[0x4A]; /* 0x2D8D* */
    u8      unknown0x2DD7[1];   /* 0x2DD7  */
    void*   sunLightDef_guess;  /* 0x2DD8* */
    void*   sunLight_guess;     /* 0x2DDC* */
    void*   sunDirectionalLight_guess; /* 0x2DE0* */
    void*   fontTexture_guess;  /* 0x2DE4* */
    int     infravision_on_guess; /* 0x2DE8  TurnInfravisionEffectOn/Off */
    u8      infravision_ambient_guess[4]; /* 0x2DEC  constant ambient rgb for infravision */
    char    defaultStrings_guess[0x5C8]; /* 0x2DF0* */

    /* 0x0040628B  name: mac (medium). Render window = screen rect offset for video modes 1..4. */
    void    OffsetRenderWindow();
    /* 0x00406399  name: takp (low, "UpdateCameraAfterModeSwitch"). Render window = screen rect. */
    void    UpdateCameraAfterModeSwitch_guess();
    /* 0x00406777  name: ours. Tints every material palette entry of the dag's DM sprite. */
    int     SetDagMaterialTint_guess(void* dag, u32* tint);
    /* 0x00406802  name: ours. `this` ignored (passed on). */
    int     SetPlayerAttachmentTint_guess(EQPlayer* player, u8 tintIndex, char slot);
    /* 0x004068DC  name: mac (medium). Selects the left/right eye palette entry for this spawn. */
    int     SetEyeMaterial(EQPlayer* player, char rightEye, int materialIndex);
    /* 0x00406A12  name: mac (medium). Finds eye palette entries by material tag. */
    void    FindDefaultEyeMaterialIndexes(EQPlayer* player);
    /* 0x00406B0D  name: mac (high). Replaces a matching material and its tint. */
    int     ReplaceMaterial(char* currentTag, char* dictionaryName, void* hierSprite,
                                  u8* tint, int partialCompare);
    /* 0x004081F2  name: mac (high). `this` ignored. */
    void    DoPoly(u8 r, u8 g, u8 b, u8 mode, int unused, int* rect);
    /* 0x00408D5B  name: mac (low). `this` ignored. */
    void    default_cameras_guess();
    /* 0x00408E86  name: ours (queue had takp "t3dEnableAutoMipMapping", low). */
    void    CreateWorldPalette_guess(void* a, void* b);
    /* 0x0040AF61  name: ours. `this` ignored. */
    void    SetPCloudDensityMultiplier_guess(int level);
    /* 0x0040B492  name: mac (medium). `this` ignored. */
    void    DrawOverlay_guess(void* overlay, int arg2, int* pos, int unused);
    /* 0x0040B6E0  name: mac (low): the 3-argument overload of UpdateItemSlot (0x0040B6F6). */
    void    UpdateItemSlot_guess(EQPlayer* player, int slot, char* name);
    /* 0x0040BA90  name: ours. `this` ignored. Item type -> 0..5. */
    int     GetItemTypeCategory_guess(int type);
    /* 0x0040C305  name: ours (queue: ShowDisplay, no evidence); a jump to ShowDisplay_guess 0x0040E418. */
    void    ShowDisplayThunk_guess();
    /* 0x0040D59A  name: takp (low, CCharacterSelect_SetLoadZoneWeather). */
    void    SetLoadZoneWeather_guess();
    /* 0x0040D98D  name: mac (high). */
    void    StartWeather(u8 type, char instant);
    /* 0x0040D9F4  name: ours (takp: CDisplay_StartWeather2, low). */
    void    UpdateWeather_guess();
    /* 0x0040DA27  name: mac (medium). */
    void    StopWeather(u8 type, char instant);
    /* 0x0040E3EA  name: ours. `this` ignored. */
    void    PlaySoundAtLocation_guess(int unused, int id, float y, float x, float z);
    /* 0x0040E422  name: mac (high). */
    void    SetViewActor(void* actor);
    /* 0x0040E49F  name: mac (low). */
    void    SetYon_guess(float yon);
    /* 0x0040E4B0  name: takp (low, "SetCameraFogMode"); calls s3dSetCameraActorYon. */
    void    SetCameraYon_guess(float yon);
    /* 0x0040E4C6  name: mac (high). */
    void    SetViewAngle_guess(short angle);
    /* 0x0040F71E  name: ours. `this` ignored. */
    void    OverlayClear_guess(void* overlay, int arg);
    /* 0x00410602  name: mac (high). Refreshes a player's renderer light. */
    void    ChangeLight(EQPlayer* player);
    /* 0x00410725  name: mac (medium). */
    void*   CreateLight(u8 type, float radius, float r, float g, float b);
    /* 0x00410D07  name: ours; passes on to UpdatePlayerVisibility (0x00410B68). */
    void    UpdatePlayerVisibility2_guess(EQPlayer* player, int unused);
    /* 0x00410997  name: mac (high). 1 if the path from the player to (y, x, z), stepped back along the heading
     * for `distance`, is clear. */
    int     IsValidTeleport(EQPlayer* player, float y, float x, float z, float heading, float distance);
    /* 0x00410ABF  name: mac (low). */
    void    UpdatePlayerActor_guess(EQPlayer* player);
    /* 0x00410C80  name: mac (high). `this` ignored. */
    int     IsInvisible(EQPlayer* viewer, EQPlayer* target);
    /* 0x0041125D  name: ours. The actor under screen point (x, y). */
    void*   PickActorOnScreen_guess(int x, int y, int getOn);
    /* 0x00411694  name: mac (high). */
    float   SetActorBoundingRadius(T3dActor_guess* actor, float scale, float minRadius);
    /* 0x00411715  name: mac (high). */
    void    DeleteActor(void* actor);
    /* 0x004119A6  name: mac (high). Replaces the DAG sprite selected by `name`. */
    void*   ChangeDag(void* dag, const char* name);
    /* 0x00411AC7  name: ours. Replaces the dag's sprite with a string sprite of `text` (none if empty). */
    void*   SetDagStringSprite_guess(void* dag, void* font, char* text);
    /* 0x00411B7B  name: mac (high). One spawn, or every spawn if NULL. */
    void    UpdateNameSprite(EQPlayer* player);
    /* 0x00412194 / 0x004121CA / 0x004121FD / 0x00412233  names: mac (high). The NPC pair's bodies look swapped
     * relative to the names (Off sets the flag and shows, On clears and hides); names kept. */
    int     TurnCharacterNameSpritesOn();
    int     TurnCharacterNameSpritesOff();
    int     ShowNPCNameSprites_guess();
    int     HideNPCNameSprites_guess();
    /* 0x00412266  name: ours (queue: takp CDisplay_SetPlayerConstantAmbient2); counterpart of 0x004122AD. */
    int     TurnInfravisionEffectOn_guess();
    /* 0x004122AD  name: mac. */
    int     TurnInfravisionEffectOff();
    /* 0x00412333  name: mac. */
    void    SpurtBloodOnDag(void* dag);
    /* 0x00412445  name: mac (high). */
    void    SetDayPeriod_guess(u8 hour);
    /* 0x00412A4E / 0x00412AA0  names: mac (high). `this` ignored; -1e27 without both players. */
    float   PlayerSimpleDistance(EQPlayer* a, EQPlayer* b, float zScale);
    float   PlayerDistance(EQPlayer* a, EQPlayer* b, float zScale);

    /* 0x00407FF1  name: mac (medium). 0 if no actor; else sets the actor's pitch type from race. */
    int     SetPlayerPitchType(EQPlayer* player);
    /* 0x0040827A  name: takp (descriptive). */
    void    DeleteActorFromDisplay_guess(void* cloud);
    /* 0x0040828C  name: takp (descriptive), "EQSwitch_UpdateDoorStateBasedOnWorldTime". */
    void    UpdateDoorStateBasedOnWorldTime_guess(EQSwitch* sw);
    /* 0x00408156  name: mac (high). Wraps a heading into [0, 512), at most 7 steps each way. */
    float   FixHeading_guess(float heading);
    /* 0x004081A3  name: mac (high). Signed shortest difference of two headings (512 = full turn). */
    float   HeadingDiff(float h1, float h2, float* pDiff);
    /* 0x00408E7F  name: mac (low). */
    int     is_3dON_guess();
    /* 0x0040AF4F  name: mac (low). */
    void    SetGammaCorrection_guess(float gamma);
    /* 0x0040B173  name: mac. */
    void    CloseOverlay_guess(void* overlay);
    /* 0x0040BB3F  name: mac (high). */
    static u32  GetUserDefinedColor(int index);
    /* 0x0040BC8C  name: mac (high). */
    static void SetUserDefinedColor(int index, u8 r, u8 g, u8 b);
    /* 0x0040C113  name: mac. */
    void    SetActorScaleFactor(void* actor, float scale, u8 flag);
    /* 0x0040E418  name: mac (medium; caller position suggests DDrawUpdateDisplay). */
    void    ShowDisplay_guess();
    /* 0x0040E4E4  name: mac (high). */
    int     SupportsVidMode(int mode);
    /* 0x0040F6FE  name: mac. */
    void    ClearScreen();
    /* 0x004105E8  name: mac. */
    void    SetActorUserData(void* actor, void* userData);
    /* 0x004108F2  name: mac. Light type -> intensity step. */
    u8      GetIntensity(u8 lightType);
    /* 0x00410970  name: mac. */
    void    DeleteLight(void* light);
    /* 0x004122EA  name: mac. */
    void    SetPlayerConstantAmbient_guess(EQPlayer* player, u8* ambient);
    /* 0x004124BE  name: mac. */
    void    SetSkyLayer(int dual);
    /* 0x00412984  name: mac (high). Manhattan distance; z scaled by 1/zScale, ignored if 0. */
    float   SimpleDistance(float x1, float y1, float z1, float x2, float y2, float z2, float zScale);
    /* 0x004129F3  name: mac (high). Euclidean distance; z scaled by 1/zScale, ignored if 0. */
    float   TrueDistance(float x1, float y1, float z1, float x2, float y2, float z2, float zScale);
    /* 0x00413ED6  name: mac (medium). Selects a named player as the display's view actor. */
    void    SetViewActorByName(const char* name);
    /* 0x00413F8F  name: takp (low). Stores transformed x, y, z at the output pointer. */
    bool    GetActorWorldPosition_guess(void* actor, float* position);
    /* 0x00413FFD  name: mac. */
    void    SetActorSpriteTint(u8* rgb, void* actor);
    /* 0x004141C6  name: takp (low). Renderer callback; no `this` argument. */
    static int IsActorCollidable_guess(void* actor, void* player);
    /* 0x00414245  name: takp (high). */
    float   GetFloorHeight(float y, float x, float z);
    /* 0x0041436D  name: takp (high). The final byte controls clearing actor state before the callback. */
    float   GetPlayerFloorHeight_guess(void* player, float y, float x, float z, char clearActorState);
    /* 0x00414468  name: takp (high). First three words pass through to the renderer without conversion. */
    float   GetCeilingHeight(u32 y, u32 x, u32 z, float radiusMultiplier);
    /* 0x00414619  name: mac (high). Builds a point from the player, then tests the 3D region. */
    int     GetEnvironmentForPlayer_guess(void* player);
    /* 0x004146A3  name: mac (high). Updates the jump cooldown and pushes the player when in a region. */
    void    DoSwimJump(void* player, char doSwimJump);
    /* 0x00414705  name: mac (high). Searches upward in 0.1 steps for the next zone boundary. */
    float   FindZoneTopZ(u32 y, u32 x, float z);
    /* 0x004147BB  name: mac (high). Returns the first matching zone-tag class in AL; writes the TP flag. */
    u8      GetEnvironment_guess(u32 y, u32 x, u32 z, int* hasTPTag);
    /* 0x00414E7A  name: mac. */
    void    PutPlayerOnFloor(EQPlayer* player);
    /* 0x0041527C  name: mac (high). Refreshes renderer floor data and timestamps for every spawn. */
    void    SetupEQPlayers(void);
    /* 0x004A31A5  name: mac (low, BinDiff 0.41): the body is a predicate, not a setter. */
    static int  SetPCloudDensity_guess(CharacterProfile* pc);

    /* EQAnimate.cpp (Mac: Source/PC Source/EverQuest/EQAnimate.cpp); `this` ignored by all of these. */
    /* 0x0048EBF8  name: takp (low). */
    int     getFallbackAnimationId_guess(int anim, EQPlayer* player, char* state);
    /* 0x0048ECCD / 0x0048ECE9 / 0x0048ED05  names: takp (low). */
    bool    isStandingAnimation_guess(int anim);
    bool    isAnimationSitOrStand_guess(int anim);
    bool    isMovementAnimation_guess(int anim);
    /* 0x0048FFAD  name: mac (by signature, manual). */
    void    ChangeAttachmentAnimationTrackSpeeds_guess(EQPlayer* player, int reverse, float speed);
    /* 0x004BFB2E  name: ours. */
    int     GetIdCategory_guess(int id);
    int SwapMaterial(EQPlayer* player, int equip_slot, int new_material, int old_material, int colour);    /* 0x0040798E */
    int SwapFace(EQPlayer* character, int newFace, int currentFace);    /* 0x00406EDC */
    int SetNameSpriteState_guess(EQPlayer* player, int showNameSprite);    /* 0x00411BC8 */
    int LoadSpriteTrack_guess(const char* trackName, const char* altTrackName, short trackId, short blockIndex, u8 slot, int primaryDagIndex);    /* 0x004A01F9 */
    void ApplyPlayerAppearance_guess(EQPlayer* player);    /* 0x00406847 */
};
C_ASSERT(offsetof(CDisplay, fogEnabled_guess) == 0x0010);
C_ASSERT(offsetof(CDisplay, unk_0x011) == 0x0011);
C_ASSERT(offsetof(CDisplay, skyLayerMode_guess) == 0x0012);
C_ASSERT(offsetof(CDisplay, skyType_guess) == 0x0014);
C_ASSERT(offsetof(CDisplay, cameraLocation_guess) == 0x001C);
C_ASSERT(offsetof(CDisplay, ambientLight_bits_guess) == 0x0038);
C_ASSERT(offsetof(CDisplay, itemOnCursor_guess) == 0x003C);
C_ASSERT(offsetof(CDisplay, moneyOnCursor_guess) == 0x003D);
C_ASSERT(offsetof(CDisplay, dragArmed_guess) == 0x003E);
C_ASSERT(offsetof(CDisplay, refList40_guess) == 0x0040);
C_ASSERT(offsetof(CDisplay, bmInfo50_guess) == 0x0050);
C_ASSERT(offsetof(CDisplay, fastFileHandle2_guess) == 0x0084);
C_ASSERT(offsetof(CDisplay, fastFileHandle3_guess) == 0x0088);
C_ASSERT(offsetof(CDisplay, fastFileHandle4_guess) == 0x008C);
C_ASSERT(offsetof(CDisplay, fastFileHandle5_guess) == 0x0090);
C_ASSERT(offsetof(CDisplay, fastFileHandle6_guess) == 0x0094);
C_ASSERT(offsetof(CDisplay, fogMode_guess) == 0x009C);
C_ASSERT(offsetof(CDisplay, fogStart_guess) == 0x00A0);
C_ASSERT(offsetof(CDisplay, fogEnd_guess) == 0x00A4);
C_ASSERT(offsetof(CDisplay, fogRed_guess) == 0x00A8);
C_ASSERT(offsetof(CDisplay, fogGreen_guess) == 0x00A9);
C_ASSERT(offsetof(CDisplay, fogBlue_guess) == 0x00AA);
C_ASSERT(offsetof(CDisplay, rainActive) == 0x00E0);
C_ASSERT(offsetof(CDisplay, snowActive_guess) == 0x00E4);
C_ASSERT(offsetof(CDisplay, snowSprite_guess) == 0x00E8);
C_ASSERT(offsetof(CDisplay, rainSprite_guess) == 0x00EC);
C_ASSERT(offsetof(CDisplay, rainCloudDef0_guess) == 0x00F0);
C_ASSERT(offsetof(CDisplay, rainCloud0_guess) == 0x00F4);
C_ASSERT(offsetof(CDisplay, rainCloudDef1_guess) == 0x00F8);
C_ASSERT(offsetof(CDisplay, rainCloud1_guess) == 0x00FC);
C_ASSERT(offsetof(CDisplay, rainCloudDef2_guess) == 0x0100);
C_ASSERT(offsetof(CDisplay, rainCloud2_guess) == 0x0104);
C_ASSERT(offsetof(CDisplay, snowCloudDef0_guess) == 0x0108);
C_ASSERT(offsetof(CDisplay, snowCloud0_guess) == 0x010C);
C_ASSERT(offsetof(CDisplay, snowCloudDef1_guess) == 0x0110);
C_ASSERT(offsetof(CDisplay, snowCloud1_guess) == 0x0114);
C_ASSERT(offsetof(CDisplay, snowCloudDef2_guess) == 0x0118);
C_ASSERT(offsetof(CDisplay, snowCloud2_guess) == 0x011C);
C_ASSERT(offsetof(CDisplay, allocatedBlock12C_guess) == 0x012C);
C_ASSERT(offsetof(CDisplay, nearClip_guess) == 0x288C);
C_ASSERT(offsetof(CDisplay, collisionSphere_guess) == 0x2CAC);
C_ASSERT(offsetof(CDisplay, collisionSphereScale_guess) == 0x2CB0);
C_ASSERT(offsetof(CDisplay, zoneRefList_guess) == 0x2CB8);
C_ASSERT(offsetof(CDisplay, visibleActorList_guess) == 0x2CBC);
C_ASSERT(offsetof(CDisplay, overlayBmInfo_guess) == 0x2CD0);
C_ASSERT(offsetof(CDisplay, worldDisplayActive) == 0x2CF8);
C_ASSERT(offsetof(CDisplay, animFlagsA_guess) == 0x2CF9);
C_ASSERT(offsetof(CDisplay, animFlagsB_guess) == 0x2D43);
C_ASSERT(offsetof(CDisplay, animFlagsC_guess) == 0x2D8D);
C_ASSERT(offsetof(CDisplay, sunLightDef_guess) == 0x2DD8);
C_ASSERT(offsetof(CDisplay, sunLight_guess) == 0x2DDC);
C_ASSERT(offsetof(CDisplay, sunDirectionalLight_guess) == 0x2DE0);
C_ASSERT(offsetof(CDisplay, fontTexture_guess) == 0x2DE4);
C_ASSERT(offsetof(CDisplay, defaultStrings_guess) == 0x2DF0);
/* 0x0040BB6C  name: mac (high). Returns one default RGB color and persists its three components. */
void GetDefaultUserDefinedColor(int index, u32* red, u32* green, u32* blue);
C_ASSERT(offsetof(CDisplay, world_guess) == 0x0004);
C_ASSERT(offsetof(CDisplay, yon) == 0x0018);
C_ASSERT(offsetof(CDisplay, weather_ambient_index_guess) == 0x00B8);
C_ASSERT(offsetof(CDisplay, time_ms_guess) == 0x00BC);
C_ASSERT(offsetof(CDisplay, alwaysOpenDoors_guess) == 0x00C3);
C_ASSERT(offsetof(CDisplay, weather_fade_guess) == 0x00C8);
C_ASSERT(offsetof(CDisplay, weather_dc_time_guess) == 0x00DC);
C_ASSERT(offsetof(CDisplay, unk_0x124) == 0x0124);
C_ASSERT(offsetof(CDisplay, light_defs_guess) == 0x2848);
C_ASSERT(offsetof(CDisplay, view_angle_guess) == 0x2880);
C_ASSERT(offsetof(CDisplay, palette_guess) == 0x2C90);
C_ASSERT(offsetof(CDisplay, is3dOn_guess) == 0x2C94);
C_ASSERT(offsetof(CDisplay, render_window_guess) == 0x2C9C);
C_ASSERT(offsetof(CDisplay, light_def_default_guess) == 0x2CB4);
C_ASSERT(offsetof(CDisplay, blood_cloud_def_guess) == 0x0120);
C_ASSERT(offsetof(CDisplay, infravision_on_guess) == 0x2DE8);
C_ASSERT(offsetof(CDisplay, infravision_ambient_guess) == 0x2DEC);
C_ASSERT(offsetof(CDisplay, boxDrawOwner_guess) == 0x0078);
C_ASSERT(offsetof(CDisplay, unk_0x07C) == 0x007C);
C_ASSERT(offsetof(CDisplay, fastFileHandle_guess) == 0x0080);
C_ASSERT(offsetof(CDisplay, unk_0x098) == 0x0098);
C_ASSERT(offsetof(CDisplay, hwnd_guess) == 0x287C);
C_ASSERT(offsetof(CDisplay, cleanupPending_guess) == 0x2C98);
C_ASSERT(sizeof(CDisplay) == 0x33B8);

void CaptureScreenshot_guess(void);

#endif /* TRILOGY_DISPLAY_H */
