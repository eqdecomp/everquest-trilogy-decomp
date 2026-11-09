/*
 * eqmisc.h - the free helper functions of Source/PC Source/EverQuest/EQMisc.cpp (Mac debug map), all
 * __cdecl. Addresses and name sources are in src/EverQuest/EQMisc.cpp.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQMISC_H
#define TRILOGY_EQMISC_H

#include "common.h"
#include "item.h"
#include "player.h"
#include "character.h"

/* globals.h VA macros for functions declared here (when globals.h came first). */
#undef is_number

void token_string_to_client_guess(const char* unused, const char* text, int color); /* 0x004A07E9 */
void steve_strncpy(char* dst, const char* src, int count);           /* 0x004A0841 */
int MakeNPCCorpseName_guess(char* name, unsigned short id);                 /* 0x004A0860 */
int checkLang_guess(CharacterProfile* pc, int lang);                        /* 0x004A08D7 */
int CostToTrain(CharacterProfile* character, int use_language, float multiplier, int skill); /* 0x004A09A4 */
u32 GetSkillLimit2_guess(CharacterProfile* pc, short skill);                /* 0x004A0C61 */
u32 GetSkillLimit(u32 value, short class_id, short skill, CharacterProfile* character); /* 0x004A0CA1 */
int setskill_guess(CharacterProfile* pc, u8 skill, short increment);        /* 0x004A1AB0 */
int Combine_guess(EQ_Equipment* item);                                      /* 0x004A1D83 */
s32 DecryptSpawnPacket_guess(NetSpawn* spawn);                              /* 0x004A38D0 */
s32 DecryptZoneSpawnPacket_guess(u32* data, u32 size);                      /* 0x004A3911 */
int HandleMoney(CharacterProfile* pc, int copper);                    /* 0x004A406D */
int RandNbr(void);                                                    /* 0x004A46B0 */
int is_number(char* text);                                            /* 0x004A47C8 */
float get_bearing(float x1, float y1, float x2, float y2);             /* 0x004A47F9 */
float GetSpawnDistance_guess(EQPlayer* a, EQPlayer* b);                    /* 0x004A48CA */
float get_melee_range(EQPlayer* a, EQPlayer* b);                      /* 0x004A491F */

/* Added in Phase 1d. EqGetTime and DoesFileExist were VA macros in globals.h;
 * that header leaves them out once this one is included (and the #undefs cover the other include order),
 * so files without eqmisc.h keep calling them through their VA (the hook redirects it). */
#undef EqGetTime
#undef DoesFileExist
#undef random_range_guess
#undef number_percent_guess
int ConcatenateStringsWithSeparators_guess(char* dst, const char* a, const char* b, const char* c); /* 0x004A0A03 */
char GetTextureSlot(short slot);                                      /* 0x004A1DCE */
int CanItemEquipInSlot_guess(void* character, u32 slot_mask, short slot, EQ_Equipment* item); /* 0x004A2180 */
int CanGoInBag(EQ_Equipment* item, void* container, int show_message); /* 0x004A25FF */
int HasLoreItem(CharacterProfile* pc, short itemNumber, EQ_Item* item); /* 0x004A28AD */
int ProcessMSG_WLDGROUP(void* pc, char* member_name);                 /* 0x004A2F02 */
int CastRay_guess(EQPlayer* from, float y, float x, float z);               /* 0x004A3491 */
s32 DecryptPlayerProfilePacket_guess(u32* data, u32 size);                  /* 0x004A3977 */
int GetAnimationIdForAttack_guess(int skill, int itemType, int ranged, float* speed); /* 0x004A3C49 */
int DoesFileExist(const char* path);                                  /* 0x004A3D04 */
int GetTeleportTableEntryByID_guess(int index);                             /* 0x004A3D5C */
int calculateFileCRC32_guess(const char* path, u32* out_crc);               /* 0x004A3ED7 */
int EQ_Character_Returns55_guess(void);                                     /* 0x004A4069 */
int isSpecialRace_allBoatTypes_guess(EQPlayer* spawn);                      /* 0x004A42AA */
int IsWeaponAttackDamageType_guess(void* damage);                              /* 0x004A42D4 */
u32 EqGetTime(void);                                                  /* 0x004A461E */
void initializeRandomNumberGenerator_guess(void);                           /* 0x004A4637 */
int random_range_guess(int lo, int hi);                                     /* 0x004A46F6 */
int number_percent_guess(void);                                             /* 0x004A473E */
int random_choice_guess(int first, ...);                                    /* 0x004A474D */
int IsStringPrefix_guess(const char* prefix, const char* text);             /* 0x004A4797 */
float get_bearing2_guess(float degrees);                                    /* 0x004A490E */
int allowslashcommand(void);                                          /* 0x004A49BD */
void cmd_who_guess(EQPlayer* player, char* args);                           /* 0x004A5480 */
void displayMOTD_guess(void);                                               /* 0x004A5490 */
void cmd_guildsay_guess(EQPlayer* player, char* args);                      /* 0x004A5703 */
int can_summon_corpse_from_here_guess(EQPlayer* corpse);                    /* 0x004A6445 */
void social_smile_guess(EQPlayer* player, char* args);                      /* 0x004A6BCD */
void social_frown_guess(EQPlayer* player, char* args);                      /* 0x004A6C02 */
void social_laugh_guess(EQPlayer* player, char* args);                      /* 0x004A6C76 */
void social_cry_guess(EQPlayer* player, char* args);                        /* 0x004A6CAB */
void social_chuckle_guess(EQPlayer* player, char* args);                    /* 0x004A6CE0 */
void social_hug_guess(EQPlayer* player, char* args);                        /* 0x004A6D15 */
void social_poke_guess(EQPlayer* player, char* args);                       /* 0x004A6D4A */
void social_bow_guess(EQPlayer* player, char* args);                        /* 0x004A6D7F */
void social_bounce_guess(EQPlayer* player, char* args);                     /* 0x004A6DB4 */
void social_panic_guess(EQPlayer* player, char* args);                      /* 0x004A6DE9 */
void social_nod_guess(EQPlayer* player, char* args);                        /* 0x004A6E1E */
void social_rofl_guess(EQPlayer* player, char* args);                       /* 0x004A6E53 */
void social_wave_guess(EQPlayer* player, char* args);                       /* 0x004A6E88 */
void social_cheer_guess(EQPlayer* player, char* args);                      /* 0x004A6EFC */
void social_gesture_guess(EQPlayer* player, char* args);                    /* 0x004A6F7B */
void social_agree_guess(EQPlayer* player, char* args);                      /* 0x004A6FEF */
void social_amaze_guess(EQPlayer* player, char* args);                      /* 0x004A7024 */
void social_apologize_guess(EQPlayer* player, char* args);                  /* 0x004A7059 */
void social_applaud_guess(EQPlayer* player, char* args);                    /* 0x004A708E */
void social_plead_guess(EQPlayer* player, char* args);                      /* 0x004A70C3 */
void social_bite_guess(EQPlayer* player, char* args);                       /* 0x004A70F8 */
void social_bleed_guess(EQPlayer* player, char* args);                      /* 0x004A712D */
void social_blink_guess(EQPlayer* player, char* args);                      /* 0x004A7162 */
void social_blush_guess(EQPlayer* player, char* args);                      /* 0x004A7197 */
void social_boggle_guess(EQPlayer* player, char* args);                     /* 0x004A71CC */
void social_bonk_guess(EQPlayer* player, char* args);                       /* 0x004A7201 */
void social_bored_guess(EQPlayer* player, char* args);                      /* 0x004A7236 */
void social_brb_guess(EQPlayer* player, char* args);                        /* 0x004A726B */
void social_burp_guess(EQPlayer* player, char* args);                       /* 0x004A72A0 */
void social_bye_guess(EQPlayer* player, char* args);                        /* 0x004A72D5 */
void social_cackle_guess(EQPlayer* player, char* args);                     /* 0x004A730A */
void social_calm_guess(EQPlayer* player, char* args);                       /* 0x004A733F */
void social_clap_guess(EQPlayer* player, char* args);                       /* 0x004A7374 */
void social_comfort_guess(EQPlayer* player, char* args);                    /* 0x004A73A9 */
void social_congratulate_guess(EQPlayer* player, char* args);               /* 0x004A73DE */
void social_cough_guess(EQPlayer* player, char* args);                      /* 0x004A7413 */
void social_cringe_guess(EQPlayer* player, char* args);                     /* 0x004A7448 */
void social_curious_guess(EQPlayer* player, char* args);                    /* 0x004A747D */
void social_dance_guess(EQPlayer* player, char* args);                      /* 0x004A74B2 */
void social_drool_guess(EQPlayer* player, char* args);                      /* 0x004A74E7 */
void social_duck_guess(EQPlayer* player, char* args);                       /* 0x004A751C */
void social_eye_guess(EQPlayer* player, char* args);                        /* 0x004A7551 */
void social_gasp_guess(EQPlayer* player, char* args);                       /* 0x004A7586 */
void social_giggle_guess(EQPlayer* player, char* args);                     /* 0x004A75BB */
void social_glare_guess(EQPlayer* player, char* args);                      /* 0x004A75F0 */
void social_grin_guess(EQPlayer* player, char* args);                       /* 0x004A7625 */
void social_groan_guess(EQPlayer* player, char* args);                      /* 0x004A765A */
void social_grovel_guess(EQPlayer* player, char* args);                     /* 0x004A768F */
void social_happy_guess(EQPlayer* player, char* args);                      /* 0x004A76C4 */
void social_hungry_guess(EQPlayer* player, char* args);                     /* 0x004A76F9 */
void social_introduce_guess(EQPlayer* player, char* args);                  /* 0x004A772E */
void social_jk_guess(EQPlayer* player, char* args);                         /* 0x004A7763 */
void social_kneel_guess(EQPlayer* player, char* args);                      /* 0x004A7798 */
void social_lost_guess(EQPlayer* player, char* args);                       /* 0x004A77CD */
void social_massage_guess(EQPlayer* player, char* args);                    /* 0x004A7802 */
void social_moan_guess(EQPlayer* player, char* args);                       /* 0x004A7837 */
void social_mourn_guess(EQPlayer* player, char* args);                      /* 0x004A786C */
void social_peer_guess(EQPlayer* player, char* args);                       /* 0x004A78A1 */
void social_point_guess(EQPlayer* player, char* args);                      /* 0x004A78D6 */
void social_ponder_guess(EQPlayer* player, char* args);                     /* 0x004A790B */
void social_puzzle_guess(EQPlayer* player, char* args);                     /* 0x004A7940 */
void social_raise_guess(EQPlayer* player, char* args);                      /* 0x004A7975 */
void social_ready_guess(EQPlayer* player, char* args);                      /* 0x004A79AA */
void social_roar_guess(EQPlayer* player, char* args);                       /* 0x004A79DF */
void social_salute_guess(EQPlayer* player, char* args);                     /* 0x004A7A14 */
void social_shiver_guess(EQPlayer* player, char* args);                     /* 0x004A7A49 */
void social_shrug_guess(EQPlayer* player, char* args);                      /* 0x004A7A7E */
void social_sigh_guess(EQPlayer* player, char* args);                       /* 0x004A7AB3 */
void social_smirk_guess(EQPlayer* player, char* args);                      /* 0x004A7AE8 */
void social_snarl_guess(EQPlayer* player, char* args);                      /* 0x004A7B1D */
void social_snicker_guess(EQPlayer* player, char* args);                    /* 0x004A7B52 */
void social_stare_guess(EQPlayer* player, char* args);                      /* 0x004A7B87 */
void social_tap_guess(EQPlayer* player, char* args);                        /* 0x004A7BBC */
void social_tease_guess(EQPlayer* player, char* args);                      /* 0x004A7BF1 */
void social_thank_guess(EQPlayer* player, char* args);                      /* 0x004A7C26 */
void social_thirsty_guess(EQPlayer* player, char* args);                    /* 0x004A7C5B */
void social_veto_guess(EQPlayer* player, char* args);                       /* 0x004A7C90 */
void social_welcome_guess(EQPlayer* player, char* args);                    /* 0x004A7CC5 */
void social_whine_guess(EQPlayer* player, char* args);                      /* 0x004A7CFA */
void social_whistle_guess(EQPlayer* player, char* args);                    /* 0x004A7D2F */
void social_yawn_guess(EQPlayer* player, char* args);                       /* 0x004A7D64 */
void cmd_gsay_guess(EQPlayer* player, char* args);                          /* 0x004A808C */
void cmd_yell_guess(EQPlayer* player, char* args);                          /* 0x004A80CE */
void cmd_auction_guess(EQPlayer* player, char* args);                       /* 0x004A810C */
void cmd_ooc_guess(EQPlayer* player, char* args);                           /* 0x004A812D */
void cmd_tell_guess(EQPlayer* player, char* args);                          /* 0x004A814E */
void AddNameToTellList(const char* name);                             /* 0x004A8181 */
void CycleNamesInTellList(int direction);                             /* 0x004A8235 */
void format_coin_components_guess(int platinum, int gold, int silver, int copper, char* output); /* 0x004A8610 */
void set_fast_drop_mode_guess(int unused, const char* choice);              /* 0x004A887E */
void mousespeed_guess(int unused, const char* speed_text);                   /* 0x004A8FBF */
void cmd_show_spell_effects_guess(int unused, const char* choice);            /* 0x004A9050 */
void show_location_guess(void* position);                                    /* 0x004A93B9 */
void cmd_emotezone_guess(EQPlayer* player, char* args);                     /* 0x004A9FF5 */
void cmd_shownames_guess(int unused, char* args);                            /* 0x004AC4A9 */
void do_shownpcnames(int unused, char* args);                         /* 0x004AC517 */
void setLOD_guess(int enabled);                                              /* 0x004AC585 */
void MarkInventoryWindowsDirty_guess(void);                                 /* 0x004C535D */
int IsNightTime_guess(void);                                                /* 0x004E2859 */

#endif /* TRILOGY_EQMISC_H */
