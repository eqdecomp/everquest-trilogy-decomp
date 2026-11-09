/*
 * globals.h - eqgame.exe globals, and functions that are not rewritten yet, reached through their fixed
 * addresses with the eqaddr.h macros (EQ_GLOBAL, EQ_ARRAY, EQ_CDECL, EQ_THISCALL, ...; the convention and
 * examples are in eqaddr.h). The eqgfx_dx7.dll entry points are in eqgfx.h.
 *
 * Names: Ghidra's DAT_xxxxxxxx where nothing is known, `_guess` names where a role is visible; existing
 * names in ghidra-types	rilogy.symbols.csv are reused as is. Each entry gives the VA and the evidence.
 *
 * C and C++ (VC6). Not included from trilogy.h (build_gdt.py parses that one; these are macros only).
 */
#ifndef TRILOGY_GLOBALS_H
#define TRILOGY_GLOBALS_H

#include "common.h"
#include "eqaddr.h"

struct CharacterProfile;
struct EQPlayer;

/* --- objects --------------------------------------------------------------------------------------- */

/* 0x006E4DA8  CharacterProfile* (symbols.csv): the EQ_PC object, the profile is its first 0x1FA4 bytes. */
#define g_pc_guess                  EQ_GLOBAL(struct CharacterProfile *, 0x006E4DA8)
/* 0x006E4D8C  EQPlayer* (symbols.csv): the local player's spawn. */
#define g_local_player_guess        EQ_GLOBAL(struct EQPlayer *, 0x006E4D8C)
/* 0x006B3194  the world connection (symbols.csv); send_message takes its address. */
#define g_world_connection_guess    EQ_ARRAY(void *, 0x006B3194)
/* 0x006E4E44  a large UI object (writes at +0xEB8, +0x6C56A, +0x8FBAD...); refreshVial and the chat input
 * check read it. Probably the EQ_Main instance (HandleMessageBoardMessage's `this` has the same 0xFE0 /
 * 0xFF fields). Kept as raw bytes until the layout is known. */
#define g_eqmain_guess              EQ_GLOBAL(char *, 0x006E4E44)
/* 0x006E4DE0  object whose dword at +0xBC is a time in ms (ToggleAutoAttack compares it with a deadline). */
#define g_displayManager_guess                EQ_GLOBAL(char *, 0x006E4DE0)
/* 0x006E4DE8  UI object; byte +0x1F1 nonzero = "input busy" for UI_ChatInputCheck. */
#define g_chatWindow_guess                EQ_GLOBAL(char *, 0x006E4DE8)
/* 0x006B6404  byte flag, selects 1/3 vs 0/2 in UI_ChatInputCheck. */
#define g_chatInputInactive_guess                EQ_GLOBAL(char, 0x006B6404)

/* 0x005FE9A0  the spell table: EQ_Spell_guess* per spell id 0..2999 (spell.h), 0 = no such spell. */
#define g_spells_guess              EQ_ARRAY(struct EQ_Spell_guess *, 0x005FE9A0)

/* 0x006EFE2C  pinstCEverQuest: the CEverQuest instance (everquest.h; the macro is only usable from C++). */
#define pinstCEverQuest_guess       EQ_GLOBAL(class CEverQuest *, 0x006EFE2C)
/* 0x006E4DA0  the player being controlled (the local player or a mount / eye of Zomm). */
#define g_control_player_guess      EQ_GLOBAL(struct EQPlayer *, 0x006E4DA0)
/* 0x006E4E60  object whose dword +0x10 suppresses the inventory window refresh. */
#define g_eqBook_guess                EQ_GLOBAL(char *, 0x006E4E60)
/* 0x006EFC68  CPU ticks per millisecond (EqGetTime). */
#define g_cpu_ticks_per_ms_guess    EQ_GLOBAL(__int64, 0x006EFC68)

/* --- EverQuest/EQMisc (Phase 1d) -------------------------------------------------------------------- */

/* 0x005FE790 / 0x005FE794  CastRay_guess's switches (alternative "NPCs block" mode, collision test off). */
#define g_ray_alt_mode_guess        EQ_GLOBAL(int, 0x005FE790)
#define g_ray_test_off_guess        EQ_GLOBAL(int, 0x005FE794)
/* 0x006E1E0C  the recent-tells list (10 names of 32 bytes); 0x006E1F4C its cycle index. */
typedef char TellName_guess[32];
#define g_tell_list_guess           EQ_ARRAY(TellName_guess, 0x006E1E0C)
#define g_tell_index_guess          EQ_GLOBAL(int, 0x006E1F4C)

/* --- EverQuest/EverQuest -------------------------------------------------------------------------- */

/* 0x005FE79C  parse position of copyStringUntilDelimiter (reset by readLineFromFile); 0x00561184 the field
 * delimiter character (an int). */
#define g_parse_pos_guess           EQ_GLOBAL(u32, 0x005FE79C)
#define g_parse_delimiter_guess     EQ_GLOBAL(int, 0x00561184)
/* Chat filter settings sent by send_update_filters (0x004CF829): ten dwords at 0x006B6984 and five more. */
#define g_filter_6984_guess         EQ_ARRAY(u32, 0x006B6984)
#define g_filterValue2_guess                EQ_GLOBAL(u32, 0x006B6EE4)
#define g_filterValue1_guess                EQ_GLOBAL(u32, 0x006B6EE8)
#define g_filterValue3_guess                EQ_GLOBAL(u32, 0x006B6EEC)
#define g_filterValue0_guess                EQ_GLOBAL(u32, 0x006B6F00)
#define g_filterValue4_guess                EQ_GLOBAL(u32, 0x006B6424)
/* 0x006B63F8 (byte) / 0x006B6494: SendLightInfo sends nothing while either is set. */
#define g_offlineMode_guess                EQ_GLOBAL(char, 0x006B63F8)
#define DAT_006b6494                EQ_GLOBAL(int, 0x006B6494)
/* 0x006E1DE4 / 0x006E1DE5  IFoundMyVehicle: set by the MSG_LOCATE_VEHICLE reply / "request pending". */
#define g_vehicle_found_guess       EQ_GLOBAL(char, 0x006E1DE4)
#define g_vehicle_pending_guess     EQ_GLOBAL(char, 0x006E1DE5)
/* 0x006E20EC / 0x006E20F0  auto-attack: next allowed time and the delay (toggled 500 <-> 10). */
#define g_nextUpdateTime_guess                EQ_GLOBAL(u32, 0x006E20EC)
#define g_updateInterval_guess                EQ_GLOBAL(u32, 0x006E20F0)
/* 0x006E24D4  ignore list: 30 names of 32 bytes (BeingIgnored scans 0x006E24D4..0x006E2893). */
#define g_ignore_list_guess         EQ_ARRAY(char, 0x006E24D4)
/* 0x006E3538  teleport index table, 0x18-byte TeleportEntry_guess records (zone.h); 0x006E4D50 the count. */
#define g_teleport_table_guess      EQ_ARRAY(struct TeleportEntry_guess, 0x006E3538)
#define g_teleport_count_guess      EQ_GLOBAL(int, 0x006E4D50)
/* Key maps (DirectInput scan code -> character), 256 bytes each. 0x0057D2D0 is the one used for key-name
 * display; 0x0057D0D0 / 0x0057D1D0 are unshifted / shifted; 0x006EFCC4 is loaded at run time and used
 * when 0x006EFCC0 == 2; 0x006EFCB0 is the shift state. */
#define g_keymap_display_guess      EQ_ARRAY(char, 0x0057D2D0)
#define g_keymap_normal_guess       EQ_ARRAY(char, 0x0057D0D0)
#define g_keymap_shifted_guess      EQ_ARRAY(char, 0x0057D1D0)
#define g_keymap_custom_guess       EQ_ARRAY(char, 0x006EFCC4)
#define g_shift_down_guess          EQ_GLOBAL(int, 0x006EFCB0)
#define g_keymap_mode_guess         EQ_GLOBAL(int, 0x006EFCC0)
/* 0x006EFCB4 / 0x006EFCB8 / 0x006EFCBC  more keyboard state flags next to the shift state (cleared with it
 * by ClearModifierKeys 0x004E878C; ctrl / alt?). */
#define g_key_state_b4_guess        EQ_GLOBAL(int, 0x006EFCB4)
#define g_key_state_b8_guess        EQ_GLOBAL(int, 0x006EFCB8)
#define g_key_state_bc_guess        EQ_GLOBAL(int, 0x006EFCBC)

/* --- EverQuest/EQMisc ------------------------------------------------------------------------------ */

/* 0x005FE66C / 0x005FE670 / 0x005FE674  RandNbr's additive generator: two indices into 55 dwords. */
#define g_rand_index_a_guess        EQ_GLOBAL(int, 0x005FE66C)
#define g_rand_index_b_guess        EQ_GLOBAL(int, 0x005FE670)
#define g_rand_table_guess          EQ_ARRAY(u32, 0x005FE674)
/* 0x005FE78C  initial key of the spawn cipher (DecryptSpawnPacket / DecryptZoneSpawnPacket). */
#define g_spawn_crypt_key_guess     EQ_GLOBAL(u32, 0x005FE78C)
/* 0x00560C08  language requirements: [language 0..24][14] minimum level per race column (checkLang). */
typedef int LangRequirementRow_guess[14];
#define g_lang_requirements_guess   EQ_ARRAY(LangRequirementRow_guess, 0x00560C08)
/* 0x006E33A4  skill records by skill id; caps per class at +0x68 (bytes) and +0x94 (dwords). */
#define g_skill_records_guess       EQ_ARRAY(char *, 0x006E33A4)
/* (0x006E5258 and 0x006E52F0, called by get_range_squared / get_melee_range, are the eqgfx_dx7.dll
 * pointers t3dFloatSqrt and t3dGetScaledActorBoundingRadius: see eqgfx.h.) */

/* --- functions not rewritten yet -------------------------------------------------------------------- */

/* 0x004A3D01  EQTRACE (Mac): debug printf, an empty `ret` in this build. */
#define EQTRACE_guess               EQ_CDECL(void, 0x004A3D01, (const char *, ...))
/* 0x004A461E  EqGetTime (Mac): milliseconds. 0x004A3D04  DoesFileExist (TAKP list): nonzero when the
 * file exists. Both are rewritten in EQMisc.cpp and declared in eqmisc.h; files that don't include eqmisc.h
 * keep reaching them through their VA (eqmisc.h #undefs these when it comes second). */
#ifndef TRILOGY_EQMISC_H
#define EqGetTime             EQ_CDECL(u32, 0x004A461E, (void))
#define DoesFileExist         EQ_CDECL(int, 0x004A3D04, (const char *))
/* 0x004A46F6  random_range (rewritten in EQMisc.cpp): lo..hi inclusive. */
#define random_range_guess          EQ_CDECL(int, 0x004A46F6, (int, int))
/* 0x004A473E  number_percent (rewritten in EQMisc.cpp): 1..100 inclusive. */
#define number_percent_guess        EQ_CDECL(int, 0x004A473E, (void))
/* 0x004A47C8  is_number (rewritten in EQMisc.cpp). */
#define is_number             EQ_CDECL(int, 0x004A47C8, (char *))
#endif
/* 0x006B52B8  Columns background object: rectangle at +4..+0x10 sizes the play field. */
#define g_columns_bg_guess          EQ_GLOBAL(char *, 0x006B52B8)
/* 0x004C1703  printf-style log message (vsprintf into a stack buffer, then output). */
#define LogMessage_guess            EQ_CDECL(void, 0x004C1703, (const char *, ...))
/* 0x004E96B9  GetCpuTicks2 (Mac): the CPU tick counter. */
#define GetCpuTicks2          EQ_CDECL(__int64, 0x004E96B9, (void))
/* 0x004E1C45  send_message (Mac). */
#define send_message_guess          EQ_CDECL(void, 0x004E1C45, (void **, u32, void *, u32, int))
/* 0x006E4D58  EQPlayer::top (Mac): head of the spawn list, linked through EQPlayer.next (+0x68). */
#define g_player_top_guess          EQ_GLOBAL(struct EQPlayer *, 0x006E4D58)
/* 0x004B1416  EQPlayer::GetPlayerFromID (rewritten in EQPlayer.cpp), __thiscall on EQPlayer::top: the spawn
 * with that id, or NULL. */
#define EQPlayer_GetPlayerFromID_guess(top, id) \
    EQ_METHOD(1, struct EQPlayer *, 0x004B1416, (struct EQPlayer *, int), (top))((id))
/* 0x004BC963  EQPlayer::InitSneakMod (Mac, medium), __thiscall, no arguments. */
#define EQPlayer_InitSneakMod_guess(player) \
    EQ_METHOD(0, void, 0x004BC963, (struct EQPlayer *), (player))()
/* 0x00419A2F  EQ_Character::Max_HP (Mac, medium), __thiscall (int). */
#define EQ_Character_Max_HP_guess(ch, a) \
    EQ_METHOD(1, s32, 0x00419A2F, (void *, int), (void *)(ch))((a))
/* 0x004A0B15  ClearOutRentables (Mac, high), __cdecl (profile, flag). */
#define ClearOutRentables     EQ_CDECL(void, 0x004A0B15, (struct CharacterProfile *, int))
/* 0x006E4DE0 as the CDisplay instance (g_displayManager_guess above) for its not-yet-rewritten methods:
 * 0x0041436D GetPlayerFloorHeight (Mac, high; -1e27 = none) and 0x00410ABF UpdatePlayerActor (Mac, low). */
#define g_display_guess             EQ_GLOBAL(void *, 0x006E4DE0)
#define CDisplay_GetPlayerFloorHeight_guess(d, p, y, x, z, f) \
    EQ_METHOD(5, float, 0x0041436D, (void *, struct EQPlayer *, float, float, float, int), (d))((p), (y), (x), (z), (f))
#define CDisplay_UpdatePlayerActor_guess(d, p) \
    EQ_METHOD(1, void, 0x00410ABF, (void *, struct EQPlayer *), (d))((p))
/* 0x006B710C / 0x006B710D  byte flags cleared on death; 0x005FE954 five dwords cleared on death;
 * 0x00590154 set to 1 on death. */
#define g_attacking_guess                EQ_GLOBAL(char, 0x006B710C)
#define g_rangedAttacking_guess                EQ_GLOBAL(char, 0x006B710D)
#define g_death_state_guess         EQ_ARRAY(u32, 0x005FE954)
#define g_activeView_guess                EQ_GLOBAL(int, 0x00590154)
/* 0x004C4EE7  CEverQuest::ProcessMBox (Mac), __thiscall, no arguments. */
#define CEverQuest_ProcessMBox_guess(self) \
    EQ_METHOD(0, void, 0x004C4EE7, (void *), (void *)(self))()

#endif /* TRILOGY_GLOBALS_H */
