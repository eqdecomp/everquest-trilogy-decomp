/*
 * everquest.h - CEverQuest, the client's main game object (Mac: Source/PC Source/EverQuest/EverQuest.cpp),
 * and the free functions of the EverQuest/EverQuest subsystem.
 *
 * The single instance is pinstCEverQuest (*(CEverQuest**)0x006EFE2C): every caller of the methods below
 * loads ECX from it. Data members: a layout JSON (not published; 0x22898 bytes, gen_layout_h --extend);
 * most of the object is still padding.
 *
 * C++ only (VC6). Not included from trilogy.h, which build_gdt.py parses as C.
 */
#ifndef TRILOGY_EVERQUEST_H
#define TRILOGY_EVERQUEST_H

#include "common.h"
#include "eqaddr.h"
#include "zone.h"         /* TeleportEntry_guess (GetTPCoordsFromIndex) */
#include "soundmanager.h" /* EqSoundManager_guess, EqSoundControl_guess */
#include "eq_columns.h"   /* EQ_Columns_guess (ExecuteCmd) */
#include "net_structs.h"  /* MsgCharacterSelect (charSelectSlots_guess) */

struct EQPlayer;

/* One of the 25 0x74-byte petition queue entries at CEverQuest+0x4C (layouts/ceverquest.json types). */
struct EverQuestEntry_guess
{
    int     id;                 /* 0x00 */
    int     rank_guess;         /* 0x04 */
    int     flag_guess;         /* 0x08 */
    u32     sequence_guess;     /* 0x0C */
    char    petitionerName_guess[0x20]; /* 0x10 */
    char    text0x30_guess[0x20]; /* 0x30 */
    int     kind_guess;         /* 0x50 */
    char    text0x54_guess[0x20]; /* 0x54 */
};
C_ASSERT(sizeof(EverQuestEntry_guess) == 0x74);

class CEverQuest
{
public:
    int     exitState_guess;    /* 0x00000* */
    int     gameState_guess;    /* 0x00004* */
    u8      unknown0x00008[0x44]; /* 0x00008  */
    EverQuestEntry_guess petitions_guess[0x19]; /* 0x0004C* */
    int     petitionKind_guess; /* 0x00BA0* */
    int     petitionCount_guess; /* 0x00BA4* */
    char    chatInput[0x840]; /* 0x00BA8* */
    int     nameBufferIndex_guess; /* 0x013E8* */
    char    chatInputChanged_guess; /* 0x013EC* */
    char    nameBuffers_guess[0x40][0x840]; /* 0x013ED* */
    u8      unknown0x223ED[3];  /* 0x223ED  */
    int     inCharacterCreation_guess; /* 0x223F0* */
    int     disconnected_guess; /* 0x223F4* */
    short   timedFogColor_guess[3]; /* 0x223F8* */
    u8      unknown0x223FE[2];  /* 0x223FE  */
    MsgCharacterSelect charSelectSlots_guess; /* 0x22400* */
    struct HWND__* window_guess;       /* 0x22894* */

    /* 0x00431568  name: mac (high). Builds the character option filename from the login name. */
    void RemoveCharacterOptionFile(char* filePrefix);
    /* 0x004CA381  name: mac (medium). Reports whether a player is a guild member, officer or leader. */
    void GuildStatus(char* targetName);
    /* 0x004CF31A  name: mac (low). Saves the profile and option blocks to the first custom option file. */
    void saveOptions_guess(void);
    /* 0x004CF3E1  name: mac (high). Saves the fixed 0x34-byte options2 block when its gates allow it. */
    void saveOptions2_guess(void);
    /* 0x004C1B48  name: mac. False for the scan codes that can't be bound (Esc, Enter, /, 0xDB..0xDC). */
    int IsValidKeyMap(int scanCode);
    /* 0x004C70CB  name: mac (low). Forces the HP/mana vial to redraw. */
    void refreshVial_guess(void);
    /* 0x004C8F39  name: mac. Body type id -> display name. */
    char* GetBodyTypeDesc(short bodyType);
    /* 0x004C8FCE  name: eqclassic (Mac: GetLangDesc). Language id -> display name. */
    char* GetLanguageName(short languageId);
    /* 0x004C9105  name: eqclassic (races.h). Race id -> display name. */
    char* GetRaceName(short race);
    /* 0x004CE8B9  name: mac (low). Always 1. */
    short IHaveTicket_guess(int unused);
    /* 0x004CFF2D  name: mac. 1 when `name` is on the ignore list (case-insensitive). */
    int BeingIgnored(char* name);
    /* 0x004D5968  name: mac + beta (EQClassic: SendLocateVehicle). MSG_LOCATE_VEHICLE round trip. */
    bool IFoundMyVehicle(void);
    /* 0x004DBCE1  name: mac. Copies the first (optionally quoted) word, returns the rest. */
    char* GrabFirstWord(char* text, char* word, int wordSize);
    /* 0x004DBD4B  name: mac. Copies the characters up to the first white space. */
    void CopyFirstWord(char* text, char* word);

    /* Phase 1d */
    /* 0x004C8F02  item types 23..26 -> instrument name, else "". */
    char* GetInstrumentDesc_guess(short itemType);
    /* 0x004C9C6D  deity id -> name ("Agnostic" for 396 and unknown ids). */
    char* GetDeityDesc_guess(short deity);
    /* 0x004C9D4E  class id 1..32 -> name (guildmasters, merchant, banker), else "UNKNOWN CLASS". */
    char* GetClassDesc_guess(short classId);
    /* 0x004C707D  riposte sound id for two spawns. */
    int GetSoundEffectForRiposte_guess(EQPlayer* attacker, EQPlayer* defender);
    /* 0x004CAEC9  (EQHandleMsg.cpp) copies the profile name into 0x005FE968 when that is "" or "PENDING". */
    void SetPendingName_guess(int unused);
    /* 0x004CF7A1  sets the chat input cursor, clamped. */
    void SetChatCursor_guess(int pos);
    /* 0x004D1112  new character name check (4..15 characters, no spaces, not "type..."). */
    int IsValidName_guess(char* name);
    /* 0x004DBC84  GrabFirstWord without a size limit. */
    char* GrabFirstWord2_guess(char* text, char* word);
    /* 0x004D024E / 0x004D02FD / 0x004D0436  name cleanup into the rotating name buffers (below). */
    char* StripShipName(char* name);
    char* trimName_guess(char* name);
    char* stripName_guess(char* name);
    /* 0x004CF829  sends the chat filters (opcode 0x21FF). */
    void send_update_filters_guess(void);
    /* 0x004CFF5E  sends the autosplit setting as spawn appearance 0x1C. */
    void SendAutoSplit_guess(void);
    /* 0x004D0BB6  sends opcode 0x2071. */
    void reqChannel(void);
    /* 0x004D9D4D  sends the light as spawn appearance 5. */
    void SendLightInfo(EQPlayer* unused, u8 light);
    /* 0x004C70EC  the part of ReportDeath that resets the local player (and clears rentables). */
    void ReportDeath_ClearOutRentables_guess(int unused, char keepRentables);
    /* 0x004D055E  shows a chat line in a colour. */
    void dsp_chat_guess(const char* text, int color);
    /* 0x004D05B7  dsp_chat with colour 0x111 while the chat window exists. */
    void print_chat_guess(const char* text);
    /* 0x004CB72D  /camp, second step: asks to camp (opcode OP_Camp) and starts the 30 second timer; sitting is required. */
    void PrepareToCamp_guess(void);
    CEverQuest* InitDisplay_guess(int arg);    /* 0x00405B18 */
    void loadOptions_guess(void);    /* 0x004CF8C3 */
    void SavePC_guess(int announce, int useLocation);    /* 0x004CE8C0 */
};
C_ASSERT(sizeof(CEverQuest) == 0x22898);
C_ASSERT(offsetof(CEverQuest, exitState_guess) == 0x00000);
C_ASSERT(offsetof(CEverQuest, gameState_guess) == 0x00004);
C_ASSERT(offsetof(CEverQuest, petitions_guess) == 0x0004C);
C_ASSERT(offsetof(CEverQuest, petitionKind_guess) == 0x00BA0);
C_ASSERT(offsetof(CEverQuest, petitionCount_guess) == 0x00BA4);
C_ASSERT(offsetof(CEverQuest, chatInput) == 0x00BA8);
C_ASSERT(offsetof(CEverQuest, nameBufferIndex_guess) == 0x013E8);
C_ASSERT(offsetof(CEverQuest, chatInputChanged_guess) == 0x013EC);
C_ASSERT(offsetof(CEverQuest, nameBuffers_guess) == 0x013ED);
C_ASSERT(offsetof(CEverQuest, inCharacterCreation_guess) == 0x223F0);
C_ASSERT(offsetof(CEverQuest, disconnected_guess) == 0x223F4);
C_ASSERT(offsetof(CEverQuest, timedFogColor_guess) == 0x223F8);
C_ASSERT(offsetof(CEverQuest, charSelectSlots_guess) == 0x22400);
C_ASSERT(offsetof(CEverQuest, window_guess) == 0x22894);

/* 0x004D061A  CEverQuest::? (824 bytes, not rewritten): the first step of dsp_chat, __thiscall (text). */
#define CEverQuest_ParseChatLine_guess(eq, text) \
    EQ_METHOD(1, void, 0x004D061A, (CEverQuest *, const char *), (eq))((text))
/* 0x0048CCE7  EQ_TextIO::ProcessTextWithWordWrapping (TAKP, low), __thiscall on the chat window. */
#define EQ_TextIO_ProcessTextWithWordWrapping_guess(wnd, text, color, flag) \
    EQ_METHOD(3, void, 0x0048CCE7, (char *, const char *, int, int), (wnd))((text), (color), (flag))
/* 0x006E2050  1 = bazaar logging on. 0x006E4E6C  the character select screen object. */
#define g_bazaar_log_on_guess       EQ_GLOBAL(int, 0x006E2050)
#define g_char_select_guess         EQ_GLOBAL(char *, 0x006E4E6C)

/* CEverQuest fields used by the rewrites (the class has no data members yet): 64 rotating 0x840-byte name
 * buffers from +0x13ED, the next one's index at +0x13E8 (StripShipName, trimName, stripName). */
#define CEQ_NAME_INDEX(eq)          (*(int *)((char *)(eq) + 0x13E8))
#define CEQ_NAME_BUFFER(eq, i)      ((char *)(eq) + 0x13ED + (i) * 0x840)

/* The character select screen at 0x006E4E6C. */
#include "trilogy/layout/eq_charsel.h"

/* The sound manager (0x006E4DD8) and the sound control object (0x006E4DD0): EqSoundManager_guess and
 * EqSoundControl_guess in soundmanager.h. */

/* 0x005FE968  a name buffer ("PENDING" until the profile arrives); 0x006E4E70 the character creation
 * screen object (+0x1BF0 = name error flag, a short); 0x006EE7E4 the 64 x 64-byte "evil process" names. */
/* 0x006EFE34  eqmain.dll's module handle; 0x006EE750 its dll_main export (LoadEQMainDLL). */
#define g_eqmain_module_guess       EQ_GLOBAL(struct HINSTANCE__ *, 0x006EFE34)
#define g_eqmain_dll_main_guess     EQ_GLOBAL(void *, 0x006EE750)
#define g_name_buffer_guess         EQ_ARRAY(char, 0x005FE968)
#define g_char_create_guess         EQ_GLOBAL(char *, 0x006E4E70)
#define g_evil_process_names_guess  EQ_ARRAY(char, 0x006EE7E4)

/* 0x006E3388  CRITICAL_SECTION guarding the input queue at 0x006018E0: head/tail indices (0x006018E0,
 * 0x006018E4) and 1024 twelve-byte entries from 0x006018EC. */
#define g_input_queue_lock_guess    EQ_ARRAY(struct _RTL_CRITICAL_SECTION, 0x006E3388)
#define g_input_queue_head_guess    EQ_GLOBAL(u32, 0x006018E0)
#define g_input_queue_tail_guess    EQ_GLOBAL(u32, 0x006018E4)
#define g_input_queue_guess         EQ_ARRAY(u32, 0x006018EC)

/* CEverQuest methods not rewritten yet, __thiscall through EQ_THISCALL (pass 0 for EDX); used by the EQMisc
 * slash-command handlers. Names from the Mac map via the queue (TAKP for print_chat). */
/* 0x004DBC84  copies the first (optionally quoted) word of `text` into `word`, returns the rest (the
 * two-argument sibling of GrabFirstWord). */
#define CEverQuest_GrabFirstWord2_guess EQ_THISCALL(char *, 0x004DBC84, (CEverQuest *, int, char *, char *))
/* 0x004CC92F  send_social (Mac, medium): sends social `id` at `target`. */
#define CEverQuest_send_social_guess    EQ_THISCALL(void, 0x004CC92F, (CEverQuest *, int, int, char *, char *))
/* 0x004D05B7  print_chat (TAKP print_chat_dsp_chat_2): prints a line to the chat window. */
#define CEverQuest_print_chat_guess     EQ_THISCALL(void, 0x004D05B7, (CEverQuest *, int, const char *))
/* 0x004D055E  dsp_chat(text, colour). */
#define CEverQuest_dsp_chat_guess       EQ_THISCALL(void, 0x004D055E, (CEverQuest *, int, const char *, int))
/* 0x004CA5C7 Who(text), 0x004CA544 GuildSay(text). */
#define CEverQuest_Who_guess            EQ_THISCALL(void, 0x004CA5C7, (CEverQuest *, int, char *))
#define CEverQuest_GuildSay_guess       EQ_THISCALL(void, 0x004CA544, (CEverQuest *, int, char *))
/* 0x004D0B20 send_gsay / 0x004D0C61 send_auction / 0x004D0CEC send_ooc: send the chat buffer at +0xBA8. */
#define CEverQuest_send_gsay_guess      EQ_THISCALL(void, 0x004D0B20, (CEverQuest *, int))
#define CEverQuest_send_auction_guess   EQ_THISCALL(void, 0x004D0C61, (CEverQuest *, int))
#define CEverQuest_send_ooc_guess       EQ_THISCALL(void, 0x004D0CEC, (CEverQuest *, int))
/* 0x004CF180  send_tell(target, text). */
#define CEverQuest_send_tell_guess      EQ_THISCALL(void, 0x004CF180, (CEverQuest *, int, char *, char *))

/* Free functions, EverQuest/EverQuest (EverQuest.cpp) */
void GetTPCoordsFromIndex(int index, float* y, float* x, float* z, float* heading, int* zoneId);
int GetKeyCharacter_guess(int scanCode);
int ScanCodeToChar_guess(int scanCode);
int UI_ChatInputCheck_guess(void);
void ToggleAutoAttack_guess(void);
int NameHasApostrophe_guess(char* name);
int IsTradeskill_guess(int skill);                                       /* 0x004A07BB */
int __fastcall GetAnimationDuration_guess(void* display, int unusedEdx, EQPlayer* player, int anim); /* 0x0048EB3A */
int __cdecl printf_guess(const char* format, ...);                       /* 0x004C17BC */
#define bazaarlog_guess             EQ_CDECL(void, 0x004C15CD, (const char *, ...))
void msg_teleport_table_guess(int unused1, int unused2, short* data);    /* 0x004C1823 */
void ClearInputQueue_guess(void);                                        /* 0x004C2F7C */
void ClearModifierKeys_guess(void);                                      /* 0x004E878C */
int copyStringUntilDelimiter_guess(char* text, char* field);             /* 0x004A30B5 */
int parseIntFromString_guess(char* text);                                /* 0x004A3113 */
void msg_teleport_index(char* filePrefix);                          /* 0x004A31CF */
int GetDagName_guess(char* actorTag, char* out, int point);              /* 0x004C7CDB */
int LogException_guess(struct _EXCEPTION_POINTERS* ep, char* where);     /* 0x004E884C */
int LoadEQMainDLL_guess(void);                                           /* 0x004E8CDB */
double parseFloatFromString_guess(char* text);                           /* 0x004A313C */
int readLineFromFile_guess(struct _iobuf* file, char* line);             /* 0x004A3165 */

#endif /* TRILOGY_EVERQUEST_H */
