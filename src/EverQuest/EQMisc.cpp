// EQMisc.cpp - free helper functions of the client (money, skills, languages, random numbers, ranges,
// small string helpers, the spawn and profile ciphers), the slash-command handlers (socials, chat
// channels), a few sound object helpers and the vlong big-integer class (include/trilogy/vlong.h).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/player.h"
#include "trilogy/character.h"
#include "trilogy/eqmisc.h"
#include "trilogy/globals.h"
#include "trilogy/eqgfx.h"
#include "trilogy/vlong.h"
#include "trilogy/sound.h"
#include "trilogy/display.h"
#include "trilogy/zone.h"
#include "trilogy/eqaddr.h"
#include "trilogy/everquest.h"
#include "trilogy/layout/eq_main.h"

// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char* formatString, ...);

struct EqMiscOutputRecord_guess
{
    u32 value;
    u8 flagBca;
    u8 flagBc8;
};

struct EqMiscOutputSource_guess
{
    u8 reserved0000[0xb9c];
    u32 value;
    u8 reserved0ba0[0x28];
    char flagBc8;
    char reservedBc9;
    char flagBca;
    u8 reserved0bcb[0x9];
    int hasValue;

    bool FillOutputRecord_guess(EqMiscOutputRecord_guess* output);
};

struct EqMiscCounterState_guess
{
    u8 reserved[0xb58];
    u32 active;
};

struct EqMiscValueSink_guess
{
    u8 reserved[0xbcc];
    u32 cappedValue;

    u32 SetCappedValue_guess(u32* source);
};

// kernel32 QueryPerformanceCounter without <windows.h> (its min/max macros clash with the vlong code).
struct PerfCounter_guess
{
    u32 low;
    s32 high;
};
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(PerfCounter_guess* count);
// The main UI object (g_eqmain_guess) seen through its vtable: slots 1 and 2 (no arguments).
class EQMainVtbl_guess
{
public:
    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
};

struct ProfileSummaryData_guess
{
    u8 unknown0[0x103C];
    time_t birthTime;
    u32 totalMinutes;
};

class ActionStateObject_guess
{
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    u32 unknown04[3];
    int state;
};

#define EQ_MISC_VALUE_COUNT 0x20

struct EqMiscCappedValueObject_guess
{
    u8 unknown0[0xBCC];
    u32 boundedValue;

    void SetBoundedValue_guess(u32 value);
};

// FUNCTION: TRILOGY 0x00415415
// ORIGINAL FILE: unknown (an out-of-line copy of the vlong library's inline operator, emitted in another object)
// x != y; y is copied for the by-value argument of cf.
int vlong_ne_guess(const vlong_guess& x, const vlong_guess& y)
{
    return x.cf(y) != 0;
}

// FUNCTION: TRILOGY 0x0041543b
// ORIGINAL FILE: unknown (out-of-line copy of the vlong library's public_key::encrypt, emitted in another object)
// RSA public key encrypt: plain^e mod m.
vlong_guess vlong_public_key_guess::encrypt(const vlong_guess& plain)
{
    return modexp_guess(plain, e, m);
}

// FUNCTION: TRILOGY 0x0041545d
// ORIGINAL FILE: unknown (an out-of-line copy of the vlong library's inline operator, emitted in another object)
// x < y.
int vlong_lt_guess(const vlong_guess& x, const vlong_guess& y)
{
    return x.cf(y) < 0;
}

// FUNCTION: TRILOGY 0x00496060
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Turns the local player towards loc[0], loc[1] (the trap location of the message) and says so.
void get_bearing_to_trap_guess(float* trapCoordinates)
{
    if (g_local_player_guess != 0)
    {
        g_local_player_guess->heading = EQ_CDECL(float, 0x004A47F9, (float, float, float, float))
            (g_local_player_guess->y, g_local_player_guess->x, trapCoordinates[0], trapCoordinates[1]);
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00541C64), 0x10E);
    }
}

class WindowObjects_guess
{
public:
    vlong_guess first;
    vlong_guess second;

    WindowObjects_guess();
};

// FUNCTION: TRILOGY 0x004a04d4
// Prepares the client's RSA public key for world authentication.
void InitializeVlongPair_guess(void)
{
    EQ_METHOD(0, void, 0x004A0529, (WindowObjects_guess*), EQ_ARRAY(WindowObjects_guess, 0x005FE758))();
}

// FUNCTION: TRILOGY 0x004a04de
// Ensures the client's shutdown routine runs when the program exits.
// __cdecl, no arguments. Registers the exit handler at 0x004A04EA with the CRT's _atexit (0x00523AB6)
// and discards its result.
void RegisterAtExitHandler_guess(void)
{
    EQ_CDECL(int, 0x00523AB6, (void (__cdecl *)(void)))
        ((void (__cdecl *)(void))0x004A04EA);
}

// FUNCTION: TRILOGY 0x004a0529
// Initializes both large-number values that hold the login key.
WindowObjects_guess::WindowObjects_guess()
    : first(0), second(0)
{
}

// FUNCTION: TRILOGY 0x004a055e
// Creates and sends the encrypted session key for world login.
// @early-stop: encrypt/assign take no pushed temp (no EH state store, two dummy xor edx), and the msg fill's count/index registers are swapped
// __cdecl (the caller pops both). keyPacket: the server's key packet, two 0x110-byte vlong blocks (modulus at
// dword 0, exponent at dword 0x44; a block is negative, share, 64 digits, z, n); reused below as a digit count.
// connection: the connection. Stores the public key, picks the 32-bit session key, RSA-encrypts it and sends the
// encrypted block as opcode 0x21e5. Returns 1.
/* fake match: msg[0..1] takes the RSA result (the original's is a pushed temp) and digitBuffer holds malloc's result; both only shape frame and registers */
int GenerateSessionKeys_guess(int keyPacket, void* connection)
{
    vlong_guess a(0);
    vlong_guess b(0);
    unsigned msg[68];
    void* digitBuffer;
    unsigned* keyWords;
    unsigned* modulusDigit;
    vlong_value_guess* V;
    int tmp;
    int digitIndex;

    g_spawn_crypt_key_guess = EQ_CDECL_CPP(int, 0x004A46F6, (int, int))(0, 0xff);
    tmp = EQ_CDECL_CPP(int, 0x004A46F6, (int, int))(0, 0xff);
    g_spawn_crypt_key_guess = g_spawn_crypt_key_guess | tmp << 8;
    tmp = EQ_CDECL_CPP(int, 0x004A46F6, (int, int))(0, 0xff);
    g_spawn_crypt_key_guess = g_spawn_crypt_key_guess | tmp << 0x10;
    tmp = EQ_CDECL_CPP(int, 0x004A46F6, (int, int))(0, 0xff);
    keyWords = (unsigned *)keyPacket;
    g_spawn_crypt_key_guess = g_spawn_crypt_key_guess | tmp << 0x18;

    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.negative = keyWords[0];
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->share = keyWords[1];
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->n = keyWords[0x43];
    keyPacket = EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->n;
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->z = keyWords[0x42];
    digitBuffer = EQ_CDECL(void *, 0x00522FA3, (u32))(0x100);
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->a = (unsigned *)digitBuffer;
    tmp = 0;
    if (0 < keyPacket)
    {
        modulusDigit = keyWords + 2;
        do
        {
            EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->a[tmp] = *modulusDigit;
            modulusDigit = modulusDigit + 1;
            tmp = tmp + 1;
        } while (tmp < keyPacket);
    }

    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.negative = keyWords[0x44];
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->share = keyWords[0x45];
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->n = keyWords[0x87];
    keyPacket = EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->n;
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->z = keyWords[0x86];
    digitBuffer = EQ_CDECL(void *, 0x00522FA3, (u32))(0x100);
    EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->a = (unsigned *)digitBuffer;
    tmp = 0;
    if (0 < keyPacket)
    {
        keyWords = keyWords + 0x46;
        do
        {
            EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->a[tmp] = *keyWords;
            keyWords = keyWords + 1;
            tmp = tmp + 1;
        } while (tmp < keyPacket);
    }

    vlong_guess c(g_spawn_crypt_key_guess);
    EQ_METHOD(2, void, 0x0041543B, (vlong_public_key_guess *, void *, vlong_guess *), &EQ_GLOBAL(vlong_public_key_guess, 0x005FE758))(msg, &c);
    EQ_METHOD(2, void, 0x004E6628, (vlong_guess *, unsigned, unsigned), &a)(msg[0], msg[1]);

    msg[0] = a.negative;
    V = a.value;
    msg[1] = V->share;
    tmp = V->n;
    msg[67] = tmp;
    msg[66] = V->z;
    digitIndex = 0;
    if (0 < tmp)
    {
        do
        {
            msg[2 + digitIndex] = V->a[digitIndex];
            digitIndex = digitIndex + 1;
        } while (digitIndex < tmp);
    }
    send_message_guess((void **)&connection, 0x21e5, msg, 0x110, 1);

    if (EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->a != 0)
    {
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).m.value->a);
    }
    if (EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->a != 0)
    {
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(vlong_public_key_guess, 0x005FE758).e.value->a);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004a07e9
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac debug map; low-confidence Mac name)
// __cdecl. The first argument is unused; forwards arguments two and three as text and color.
void token_string_to_client_guess(const char* unused, const char* text, int color)
{
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(text, color);
}

// FUNCTION: TRILOGY 0x004a07fd
// Submits the player's petition to the world server.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Sends the petition text, with its NUL terminator, on the world connection as OP_Petition. Refused
// (returns 0) when the text is NULL or longer than 1023 characters; the only caller ignores the result.
int UserPetitions(char* petitionText)
{
    size_t textLen;

    if (petitionText != (char*)0)
    {
        // strlen() (0x005223D0) called for nothing: the result is unused. Kept, because the binary makes this call too.
        strlen(petitionText);
        textLen = strlen(petitionText);
        // 0x3ff = 1023, the longest petition text.
        if (textLen <= 0x3ff)
        {
            textLen = strlen(petitionText);
            // 0x200e is OP_Petition (include/trilogy/opcodes.h). The NUL is sent too, hence textLen + 1.
            send_message_guess(g_world_connection_guess, 0x200e, petitionText, textLen + 1, 1);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a0841
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: steve_strncpy, high)
// __cdecl. strncpy that always terminates: dst[count] = 0, so dst needs count + 1 bytes.
void steve_strncpy(char* dst, const char* inputText, int count)
{
    strncpy(dst, inputText, count);
    dst[count] = '\0';
}

// FUNCTION: TRILOGY 0x004a0860
// Formats a slain NPC's name as a corpse label.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: MakeNPCCorpseName, high)
// __cdecl. Strips up to two trailing digits of an NPC name (the spawn number, only when the name is longer
// than 2), keeps at most 14 characters and writes "<name>'s corpse<id>" back over the name. Returns 1.
int MakeNPCCorpseName_guess(char* name, unsigned short id)
{
    char buf[256];
    unsigned int len;

    len = strlen(name);
    if (len > 2)
    {
        if (name[len - 2] >= '0' && name[len - 2] <= '9')
            name[len - 2] = '\0';
        if (name[len - 1] >= '0' && name[len - 1] <= '9')
            name[len - 1] = '\0';
    }
    strncpy(buf, name, 14);
    buf[14] = '\0';
    sprintf(name, "%s's corpse%u", buf, id);
    return 1;
}

// FUNCTION: TRILOGY 0x004a08d7
// Checks whether a character can train the selected language.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: checkLang, high; TAKP: EQ_PC_checkLang)
// __cdecl. Whether the character may train language `lang` (0..24): level against the per-race
// requirement table, plus class exceptions (11: lang 12 from level 8, lang 17 from level 20; 12 and 13:
// lang 11 from level 8; 14: lang 11 always).
// The race indexes the table unsigned (movzx; CharacterProfile.race is a u16), Iksar (128) and Vah Shir
// (130) use columns 13 and 14. Faithful bug: the rows have 14 columns (0..13), so Vah Shir's column 14 is column 0 of the next
// language's row.
int checkLang_guess(CharacterProfile* pc, int lang)
{
    int ok = 0;
    int required;
    unsigned short race;
    unsigned char level;
    char cls;

    if (pc == NULL || lang < 0 || lang >= 25)
        return 0;
    race = pc->race;
    if (race == 128)
        required = g_lang_requirements_guess[lang][13];
    else if (race == 130)
        required = g_lang_requirements_guess[lang][14];
    else
        required = g_lang_requirements_guess[lang][race];
    level = pc->level;
    if ((int)level >= required)
        ok = 1;
    cls = pc->class_;
    if (cls == 11)
    {
        if (level >= 8 && lang == 12)
            ok = 1;
        if (level >= 20 && lang == 17)
            ok = 1;
    }
    if (cls == 12 && level >= 8 && lang == 11)
        ok = 1;
    if (cls == 14 && lang == 11)
        ok = 1;
    if (cls == 13 && level >= 8 && lang == 11)
        ok = 1;
    return ok;
}

// FUNCTION: TRILOGY 0x004a09a4
// Calculates the training price for a skill or language.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: CostToTrain, high)
// __cdecl. Training cost is a cubic of the nonnegative skill value above 10, scaled by the caller and
// the client's constant. With the language flag set it uses the language value at +0x7E; otherwise the
// character must already have the skill and its trained value at +0x9C8 is used.
int CostToTrain(CharacterProfile* character, int use_language, float multiplier, int skill)
{
    int value;
    int cube;
    float amount;

    if (use_language != 0)
    {
        value = character->languages[skill] - 10;
    }
    else if (EQ_METHOD(1, u8, 0x0041A3E9, (CharacterProfile*, int), character)(skill) != 0)
    {
        value = character->skills[skill] - 10;
    }
    else
    {
        value = 0;
    }
    if (value < 0)
        value = 0;
    cube = value * value * value;
    amount = (float)cube * multiplier;
    amount = amount * EQ_GLOBAL(float, 0x00535224);
    return (int)amount;
}

// FUNCTION: TRILOGY 0x004a0a03
// Builds the account text sent during world login.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; name ours, low)
// __cdecl. Writes the three strings one after the other into dst, each with its terminating NUL (the
// format of a three-field packet buffer). Returns 1.
int ConcatenateStringsWithSeparators_guess(char* dst, const char* a, const char* b, const char* c)
{
    unsigned int off;
    unsigned int lb;

    strcpy(dst, a);
    off = strlen(a) + 1;
    strcpy(dst + off, b);
    lb = strlen(b);
    strcpy(dst + off + lb + 1, c);
    return 1;
}

// FUNCTION: TRILOGY 0x004a0a4c
// Checks whether an item and its contents may be dropped, traded, sold, or banked.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
int CanDrop_guess(EQ_Item* item, int mode)
{
    if (item == 0)
    {
        return 0;
    }
    if (mode == 2)
    {
        if (item->nodrop == 2)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00561F14), 0xF);
            return 0;
        }
        if (item->type == 1)
        {
            int i;

            for (i = 0; i < ((EQ_Container*)item)->slots; i++)
            {
                EQ_Item* sub = ((EQ_Container*)item)->contents[i];

                if ((sub != 0) && (sub->nodrop == 2))
                {
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00561E80), 0xF);
                    return 0;
                }
            }
        }
    }
    if (item->nodrop == 0)
    {
        if (mode == 0)
        {
            return 0;
        }
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00561EE4), 0xF);
        return 0;
    }
    if (item->type == 1)
    {
        int i;

        for (i = 0; i < ((EQ_Container*)item)->slots; i++)
        {
            EQ_Item* sub = ((EQ_Container*)item)->contents[i];

            if ((sub != 0) && (sub->nodrop == 0))
            {
                if (mode == 0)
                {
                    return 0;
                }
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00561E0C), 0xF);
                return 0;
            }
        }
    }
    return 1;
}

#undef ClearOutRentables

// FUNCTION: TRILOGY 0x004a0b15
// Removes no-rent items from the character's inventory and bags.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
int ClearOutRentables(int* characterProfile, int freeItems)
{
    int bagItem;
    int profile;
    int result;
    int invItem;
    u16* bagNumber;
    int* contentSlot;
    u16* contentNumber;
    u16* inventoryNumber;
    int contentIndex;
    int local_8;

    profile = (int)characterProfile;
    if (characterProfile == 0)
    {
        result = 0;
    }
    else
    {
        inventoryNumber = (u16*)((int)characterProfile + 0xa4);
        characterProfile = (int*)((int)characterProfile + 0xe0);
        local_8 = 0xb1;
        do
        {
            invItem = *characterProfile;
            if (invItem != 0)
            {
                if (*(char*)(invItem + 0x81) == '\x01')
                {
                    contentIndex = 0;
                    if (*(u8*)(invItem + 0xd5) > 0)
                    {
                        contentNumber = (u16*)(profile + 0x3ba);
                        contentSlot = (int*)(invItem + 0xac);
                        do
                        {
                            bagItem = *contentSlot;
                            if ((bagItem != 0) && (*(char*)(bagItem + 0x7e) == '\0'))
                            {
                                if (freeItems != 0)
                                {
                                    EQ_CDECL(void, 0x005218BB, (void*))( (void*)bagItem );
                                    *contentSlot = 0;
                                }
                                if (local_8 < 0x18d)
                                {
                                    *contentNumber = 0xffff;
                                }
                                else
                                {
                                    *(short*)(profile + (local_8 + contentIndex) * 2) = -1;
                                }
                            }
                            contentIndex = contentIndex + 1;
                            contentNumber = contentNumber + 1;
                            contentSlot = contentSlot + 1;
                        } while (contentIndex < (int)(u32)*(u8*)(invItem + 0xd5));
                    }
                    if (*(char*)(invItem + 0x7e) == '\0')
                    {
                        if (freeItems != 0)
                        {
                            if (invItem != 0)
                            {
                                EQ_METHOD(0, void, 0x0043405A, (EQ_Container*), (EQ_Container*)invItem)();
                                EQ_CDECL(void, 0x005218BB, (void*))( (void*)invItem );
                            }
                            *characterProfile = 0;
                        }
                        bagNumber = (u16*)(profile + 0x3ba);
                        *inventoryNumber = 0xffff;
                        invItem = 0;
                        do
                        {
                            if (local_8 < 0x18d)
                            {
                                *bagNumber = 0xffff;
                            }
                            else
                            {
                                *(short*)(profile + (local_8 + invItem) * 2) = -1;
                            }
                            invItem = invItem + 1;
                            bagNumber = bagNumber + 1;
                        } while (invItem < 10);
                    }
                }
                else if (*(char*)(invItem + 0x7e) == '\0')
                {
                    if (freeItems != 0)
                    {
                        EQ_CDECL(void, 0x005218BB, (void*))( (void*)invItem );
                        *characterProfile = 0;
                    }
                    *inventoryNumber = 0xffff;
                }
            }
            local_8 = local_8 + 10;
            characterProfile = characterProfile + 1;
            inventoryNumber = inventoryNumber + 1;
        } while (local_8 < 0x1dd);
        result = 1;
    }
    return result;
}

// FUNCTION: TRILOGY 0x004a0c61
// Gets a class's base cap for one trained skill.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name, low)
// __cdecl. The class's cap for a skill from the skill record: a dword table at +0x94 when the profile's
// expansion flags (+0xAE0) are set, else the byte table at +0x68. 0 without a character. The skill id is
// a short.
u32 GetSkillLimit2_guess(CharacterProfile* pc, short skill)
{
    if (pc == NULL)
        return 0;
    if (pc->expansions_guess != 0)
        return *(u32*)(g_skill_records_guess[skill] + 0x94 + pc->class_ * 4);
    return *(u8*)(g_skill_records_guess[skill] + 0x68 + pc->class_);
}

// FUNCTION: TRILOGY 0x004a0ca1
// Maximum a skill can reach for a class at a given level, including the class and expansion caps
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: GetSkillLimit, high)
// __cdecl. The class cap for a skill, adjusted for level and expansion. Some skill ids deliberately fall
// through to level * 5; the original table accesses and signed short arithmetic are unchecked.
u32 GetSkillLimit(short level, short class_id, short skill, CharacterProfile* character)
{
    u32 value;
    char* record;
    int skill_id;
    u8 cap;
    int i;

    skill_id = (int)skill;
    record = g_skill_records_guess[skill_id];
    if (*(char*)(record + 0x27) == 0)
        return EQ_CDECL(u32, 0x004A0C61, (CharacterProfile*, short))(character, skill);

    switch (skill)
    {
    case 0: case 1: case 2: case 3: case 0x24:
    {
        switch (class_id)
        {
        case 2: case 6: case 10:
            value = level * 4 + 4;
            break;
        case 7:
            // as original: both sides share the limit, but the level compare remains.
            if (level < 18)
                value = (level + 1) * 5;
            else
                value = (level + 1) * 5;
            break;
        case 11: case 12: case 13: case 14:
            value = (level + 1) * 3;
            break;
        default:
            value = (level + 1) * 5;
            break;
        }
        break;
    }
    case 4: case 5:
        value = (level + 1) * 5;
        break;
    case 13:
    case 14:
        value = (level + 1) * 5;
        break;
    case 0x0f:
    {
        switch (class_id)
        {
        case 1:
            value = (level + 1) * 5;
            break;
        case 2: case 6: case 10:
            value = level * 5;
            break;
        default:
            value = level * 5;
            break;
        case 11: case 12: case 13: case 14:
        {
            if (level < 6)
                value = level * 5;
            else if (level < 11)
                value = level * 4 + 5;
            else
                value = (level + 5) * 3;
            break;
        }
        }
        break;
    }
    case 0x12:
        value = (level + 1) * 5;
        break;
    case 0x16:
    {
        switch (class_id)
        {
        case 7:
            value = level * 7 + 5;
            break;
        case 9:
            value = level * 6 + 5;
            break;
        default:
            value = (level + 1) * 5;
            break;
        }
        break;
    }
    case 0x18:
        value = (level + 1) * 5;
        break;
    case 0x1c:
    {
        if (class_id == 7 || class_id == 15)
        {
            // as original: both sides share the limit, but the level compare remains.
            if (level < 15)
                value = (level + 1) * 5;
            else
                value = (level + 1) * 5;
        }
        else
            value = (level + 1) * 3;
        break;
    }
    case 0x1f:
        value = (level + 1) * 5;
        break;
    case 0x21:
    {
        switch (class_id)
        {
        case 1:
            value = (level + 1) * 5;
            break;
        case 2: case 6: case 10:
            value = level * 5;
            break;
        default:
            value = level * 5;
            break;
        case 7: case 15:
        {
            if (level > 29)
                value = (level + 2) * 5;
            else if (level > 14)
                value = (level + 1) * 5;
            else
                value = level * 5;
            break;
        }
        case 11: case 12: case 13: case 14:
            value = level << 2;
            break;
        }
        break;
    }
    case 0x2b:
        value = (level + 1) * 5;
        break;
    case 0x2c:
        value = (level + 1) * 5;
        break;
    case 0x2d:
        value = (level + 1) * 5;
        break;
    case 0x2e:
        value = (level + 1) * 5;
        break;
    case 0x2f:
        value = (level + 1) * 5;
        break;
    case 0x39: case 0x3a: case 0x3b:
        cap = *(u8*)(record + class_id + 0x28);
        if ((short)(u16)cap > level)
            value = 0;
        else
            value = (level + 1) * 5 - (u32)cap * 4;
        break;
    case 0x13:
    default:
        value = (level + 1) * 5;
        break;
    }
    if (character != 0)
        EQ_CDECL(int, 0x00480E8C, (CharacterProfile*, short, u32*))(character, skill, &value);
    else
    {
        i = *(u8*)(record + class_id + 0x68);
        if ((int)value > i)
            value = i;
    }
    return value;
}

// setskill_guess (0x004A1AB0) called by address: (character, skill, increment).
#define SETSKILL_GUESS(skill, increment) \
    EQ_CDECL(int, 0x004A1AB0, (CharacterProfile*, int, int))(pc, (skill), (increment))

// FUNCTION: TRILOGY 0x004a11e3
// Gives a character the skills and training points of its level, class and race, overwriting the current ones
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Sets the starting trained skills of a new character: training points (level + 1) * 5, every
// existing skill slot from the class's base level (capped by GetSkillLimit2), then a class table of
// setskill calls, a race tweak (skills 0x9e3..0xa01) and the extra skills 0x38..0x45 when the player
// object's byte at +0xf1 is set. Always returns 1.
// @early-stop: skill slots still use indexed access, and VC6 drops two no-op level comparisons.
/* fake match: repeat the record-byte expression so VC6 reuses AL for the original byte arithmetic. */
int setSkillsByLevel(CharacterProfile* pc)
{
    u8* skill_value;
    int i;
    short delta;
    int limit;

    pc->training_points = (pc->level + 1) * 5;
    i = 0;
    skill_value = pc->skills;
    do
    {
        if (*skill_value != 0xff)
        {
            delta = pc->level - *(u8*)(g_skill_records_guess[i] + pc->class_ + 0x28);
            if (delta >= 0)
            {
                *skill_value = (*(u8*)(g_skill_records_guess[i] + pc->class_ + 0x28) + (char)delta + 1) * 2;
                limit = EQ_CDECL(int, 0x004A0C61, (CharacterProfile*, int))(pc, i);
                if ((int)*skill_value > limit)
                    *skill_value = EQ_CDECL(int, 0x004A0C61, (CharacterProfile*, int))(pc, i);
            }
            else
                *skill_value = 0xfe;
        }
        i++;
        skill_value++;
    } while ((short)i < 100);

    switch (pc->class_)
    {
    case 1:
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x01, 5);
        SETSKILL_GUESS(0x03, 5);
        SETSKILL_GUESS(0x02, 5);
        SETSKILL_GUESS(0x24, 5);
        SETSKILL_GUESS(0x0a, 5);
        SETSKILL_GUESS(0x07, 5);
        SETSKILL_GUESS(0x14, 5);
        SETSKILL_GUESS(0x22, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x25, 5);
        SETSKILL_GUESS(0x10, 5);
        SETSKILL_GUESS(0x1e, 5);
        SETSKILL_GUESS(0x16, 5);
        SETSKILL_GUESS(0x49, 5);
        break;
    case 2:
        SETSKILL_GUESS(0x21, 4);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x00, 4);
        SETSKILL_GUESS(0x02, 4);
        SETSKILL_GUESS(0x13, 4);
        SETSKILL_GUESS(0x09, 5);
        SETSKILL_GUESS(0x1f, 4);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        pc->skills[0x2b] = 0x31;
        pc->skills[0x2c] = 0x31;
        pc->skills[0x2d] = 0x31;
        pc->skills[0x2e] = 0x31;
        pc->skills[0x2f] = 0x31;
        break;
    case 3:
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x01, 5);
        SETSKILL_GUESS(0x03, 5);
        SETSKILL_GUESS(0x02, 5);
        SETSKILL_GUESS(0x0a, 5);
        SETSKILL_GUESS(0x14, 5);
        SETSKILL_GUESS(0x22, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x25, 5);
        SETSKILL_GUESS(0x10, 5);
        SETSKILL_GUESS(0x1f, 5);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        SETSKILL_GUESS(0x49, 5);
        break;
    case 4:
        SETSKILL_GUESS(0x35, 5);
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x01, 5);
        SETSKILL_GUESS(0x03, 5);
        SETSKILL_GUESS(0x24, 5);
        SETSKILL_GUESS(0x0a, 5);
        SETSKILL_GUESS(0x07, 5);
        SETSKILL_GUESS(0x16, 5);
        SETSKILL_GUESS(0x14, 5);
        SETSKILL_GUESS(0x22, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x25, 5);
        SETSKILL_GUESS(0x10, 5);
        SETSKILL_GUESS(0x1e, 5);
        SETSKILL_GUESS(0x16, 5);
        SETSKILL_GUESS(0x1f, 5);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        SETSKILL_GUESS(0x49, 5);
        break;
    case 5:
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x01, 5);
        SETSKILL_GUESS(0x03, 5);
        SETSKILL_GUESS(0x02, 5);
        SETSKILL_GUESS(0x0a, 5);
        SETSKILL_GUESS(0x24, 5);
        SETSKILL_GUESS(0x14, 5);
        SETSKILL_GUESS(0x22, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x25, 5);
        SETSKILL_GUESS(0x10, 5);
        SETSKILL_GUESS(0x1f, 5);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        break;
    case 6:
        SETSKILL_GUESS(0x21, 4);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x00, 4);
        SETSKILL_GUESS(0x02, 4);
        SETSKILL_GUESS(0x13, 4);
        SETSKILL_GUESS(0x09, 5);
        SETSKILL_GUESS(0x1f, 5);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        SETSKILL_GUESS(0x32, 5);
        pc->skills[0x2b] = 0x31;
        pc->skills[0x2c] = 0x31;
        pc->skills[0x2d] = 0x31;
        pc->skills[0x2e] = 0x31;
        pc->skills[0x2f] = 0x31;
        break;
    case 7:
        SETSKILL_GUESS(0x16, 7);
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x00, 5);
        SETSKILL_GUESS(0x02, 5);
        SETSKILL_GUESS(0x1c, 5);
        SETSKILL_GUESS(0x33, 5);
        SETSKILL_GUESS(0x19, 5);
        SETSKILL_GUESS(0x20, 5);
        SETSKILL_GUESS(0x1e, 5);
        SETSKILL_GUESS(0x0b, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x14, 5);
        SETSKILL_GUESS(0x10, 5);
        SETSKILL_GUESS(0x25, 5);
        SETSKILL_GUESS(0x27, 5);
        SETSKILL_GUESS(0x26, 5);
        SETSKILL_GUESS(0x34, 5);
        SETSKILL_GUESS(0x17, 5);
        SETSKILL_GUESS(0x15, 5);
        SETSKILL_GUESS(0x1a, 5);
        break;
    case 8:
        SETSKILL_GUESS(0x16, 5);
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x00, 5);
        SETSKILL_GUESS(0x01, 5);
        SETSKILL_GUESS(0x24, 5);
        SETSKILL_GUESS(0x33, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x0c, 5);
        SETSKILL_GUESS(0x29, 5);
        SETSKILL_GUESS(0x31, 5);
        SETSKILL_GUESS(0x36, 5);
        break;
    case 9:
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x01, 5);
        SETSKILL_GUESS(0x03, 5);
        SETSKILL_GUESS(0x07, 5);
        SETSKILL_GUESS(0x24, 5);
        SETSKILL_GUESS(0x33, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x08, 5);
        SETSKILL_GUESS(0x14, 5);
        SETSKILL_GUESS(0x22, 5);
        SETSKILL_GUESS(0x25, 5);
        SETSKILL_GUESS(0x16, 5);
        break;
    case 10:
        SETSKILL_GUESS(0x21, 4);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x00, 4);
        SETSKILL_GUESS(0x02, 4);
        SETSKILL_GUESS(0x13, 4);
        SETSKILL_GUESS(0x09, 5);
        SETSKILL_GUESS(0x1f, 5);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        pc->skills[0x2b] = 0x31;
        pc->skills[0x2c] = 0x31;
        pc->skills[0x2d] = 0x31;
        pc->skills[0x2e] = 0x31;
        pc->skills[0x2f] = 0x31;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        SETSKILL_GUESS(0x21, 3);
        SETSKILL_GUESS(0x0f, 3);
        SETSKILL_GUESS(0x24, 3);
        SETSKILL_GUESS(0x33, 3);
        SETSKILL_GUESS(0x13, 3);
        SETSKILL_GUESS(0x1f, 5);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        pc->skills[0x2b] = 0x31;
        pc->skills[0x2c] = 0x31;
        pc->skills[0x2d] = 0x31;
        pc->skills[0x2e] = 0x31;
        pc->skills[0x2f] = 0x31;
        break;
    case 15:
        SETSKILL_GUESS(0x16, 5);
        SETSKILL_GUESS(0x21, 5);
        SETSKILL_GUESS(0x0f, 5);
        SETSKILL_GUESS(0x00, 5);
        SETSKILL_GUESS(0x02, 5);
        SETSKILL_GUESS(0x1c, 5);
        SETSKILL_GUESS(0x24, 5);
        SETSKILL_GUESS(0x33, 5);
        SETSKILL_GUESS(0x0b, 5);
        SETSKILL_GUESS(0x13, 5);
        SETSKILL_GUESS(0x25, 5);
        SETSKILL_GUESS(0x1e, 5);
        SETSKILL_GUESS(0x1f, 5);
        SETSKILL_GUESS(0x04, 5);
        SETSKILL_GUESS(0x05, 5);
        SETSKILL_GUESS(0x0d, 5);
        SETSKILL_GUESS(0x0e, 5);
        SETSKILL_GUESS(0x12, 5);
        SETSKILL_GUESS(0x18, 5);
        break;
    }

    switch (pc->race)
    {
    case 4:
        pc->skills[0x1b] = 0x32;
        pc->skills[0x1d] = 0x32;
        break;
    case 8:
        pc->skills[0x28] = 0x32;
        break;
    case 11:
        pc->skills[0x2a] = 0x32;
        pc->skills[0x1d] = 0x32;
        break;
    case 12:
        pc->skills[0x39] = 0x32;
        break;
    case 0x80:
        pc->skills[0x32] = 100;
        pc->skills[0x1b] = 0x32;
        break;
    case 0x82:
        pc->skills[0x2a] = 0x32;
        pc->skills[0x1d] = 0x32;
        pc->skills[0x27] = 100;
        break;
    }

    if (*((char*)pc->player + 0xf1) != 0)
    {
        SETSKILL_GUESS(0x38, 5);
        SETSKILL_GUESS(0x39, 5);
        SETSKILL_GUESS(0x3a, 5);
        SETSKILL_GUESS(0x3b, 5);
        SETSKILL_GUESS(0x3c, 5);
        SETSKILL_GUESS(0x3d, 5);
        SETSKILL_GUESS(0x3f, 5);
        SETSKILL_GUESS(0x40, 5);
        SETSKILL_GUESS(0x41, 5);
        SETSKILL_GUESS(0x40, 5);
        SETSKILL_GUESS(0x44, 5);
        SETSKILL_GUESS(0x45, 5);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004a1ab0
// Sets one trained skill's rank for the character's current level.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// @early-stop: VC6 puts the product (lea ecx,[edi+1]) before the class base load; the original loads the base first and does inc edi in place (5 normalised diffs)
// __cdecl. Updates one trained skill from the class's base level, then applies the expansion cap and
// the hard-coded level formula for selected high skills. The level/class threshold used for the first
// eligibility check is read before the expansion helper; the base value is fetched again afterwards.
/* fake match: the goto is the original's jump from skills outside the special set straight to the final clamp, past the shared store */
int setskill_guess(CharacterProfile* pc, u8 skill, short increment)
{
    int maximum;
    int level_delta;
    u8* skill_value;
    int value;
    char* record;
    u8 index;

    maximum = EQ_CDECL(int, 0x004A0C61, (CharacterProfile*, short))(pc, skill);
    index = skill;
    record = g_skill_records_guess[index];
    level_delta = pc->level - (char)*(u8*)(record + pc->class_ + 0x28);
    long temp = pc->expansions_guess;
    if (temp)
        EQ_CDECL(int, 0x00480E8C, (CharacterProfile*, short, u32*))(pc, skill, (u32*)&maximum);

    if (level_delta >= 0)
    {
        record = g_skill_records_guess[index];
        level_delta = *(u8*)(record + pc->class_ + 0x28) + (level_delta + 1) * increment;
        if (level_delta > maximum)
            level_delta = maximum;
        skill_value = &pc->skills[index];
        *skill_value = level_delta;

        if (skill >= 0x13)
        {
            if (skill <= 0x14 || skill == 0x16 || skill == 0x22)
                value = (pc->level + 1) * 5;
            else if (skill == 0x25)
                value = pc->level * 3 + 10;
            else
                goto final_cap;

            if (value > maximum)
                value = maximum;
            *skill_value = value;
        }

    final_cap:
        if (*skill_value > maximum)
            *skill_value = maximum;
    }
    else
        pc->skills[index] = 0xfe;
    return 1;
}

// The `new` of MoveItemQty. Its constructor is the client's item constructor, only declared here, so VC6
// keeps the EH frame around the `new` (it can't prove the constructor doesn't throw).
class MoveItemEquipment_guess : public EQ_Equipment
{
public:
    static void* operator new(size_t size)
    {
        return EQ_CDECL(void*, 0x005218C6, (u32))((u32)size);
    }
    static void operator delete(void* value)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(value);
    }
    // EXTERNAL: TRILOGY 0x00436099 MoveItemEquipment_guess::MoveItemEquipment_guess
    MoveItemEquipment_guess(void);
};

// FUNCTION: TRILOGY 0x004a1b8c
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl, 3 stack arguments. Moves `quantity` charges of a stackable item from *sourceEquipmentPtr to *destinationItemPtr.
// With an empty *destinationItemPtr the source is copied into a new item first; otherwise both must be combinable
// and carry the same item number. Both pointers are written back only when the destination still has
// charges; an item left with 0 charges is freed (its pointer becomes 0). Returns 1 on success.
int MoveItemQty(EQ_Equipment** sourceEquipmentPtr, EQ_Equipment** destinationItemPtr, s8 quantity)
{
    EQ_Equipment* src;
    EQ_Equipment* dst;
    u8* dst_charges;

    src = *sourceEquipmentPtr;
    dst = *destinationItemPtr;
    if (dst == 0)
    {
        dst = new MoveItemEquipment_guess;
        memcpy(dst, src, 0x124);
        src->charges -= (u8)(src->stackable * quantity);
        dst_charges = &dst->charges;
        *dst_charges = (u8)(dst->stackable * quantity);
    }
    else
    {
        if (EQ_CDECL_CPP(int, 0x004A1D83, (EQ_Equipment*))(src) == 0
            || EQ_CDECL_CPP(int, 0x004A1D83, (EQ_Equipment*))(dst) == 0
            || src->item.item_nr != dst->item.item_nr)
            return 0;
        dst_charges = &dst->charges;
        src->charges -= (u8)(src->stackable * quantity);
        *dst_charges += (u8)(dst->stackable * quantity);
    }

    if (src->charges <= 0)
    {
        EQ_CDECL_CPP(void, 0x005218BB, (void*))(src);
        src = 0;
    }
    if (*dst_charges <= 0)
    {
        EQ_CDECL_CPP(void, 0x005218BB, (void*))(dst);
    }
    else
    {
        *sourceEquipmentPtr = src;
        *destinationItemPtr = dst;
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a1c7c
// Fills one matching inventory stack from another and reports the items moved.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
int SwapCombine_guess(EQ_Equipment** src_slot, EQ_Equipment** dst_slot, short slot_a, short slot_b)
{
    EQ_Equipment* src;
    EQ_Equipment* dst;
    short moved;
    struct SwapCombineMessage_guess
    {
        int slot_a;
        int slot_b;
        int moved;
    } message;

    src = *src_slot;
    dst = *dst_slot;
    if (src != 0 && dst != 0 && EQ_CDECL(int, 0x004A1D83, (EQ_Equipment*))(src) != 0
        && src->item.item_nr == dst->item.item_nr)
    {
        short room = dst->stackable * 20 - dst->charges;
        if ((short)src->charges > room)
        {
            src->charges = src->charges - (char)room;
            dst->charges = dst->charges + (char)room;
            moved = room / src->stackable;
        }
        else
        {
            dst->charges = src->charges + dst->charges;
            moved = (short)(src->charges / src->stackable);
            EQ_CDECL(void, 0x005218BB, (EQ_Equipment*))(src);
            src = 0;
        }
        if (slot_a >= 0 && moved > 0)
        {
            message.slot_a = (int)slot_a;
            message.slot_b = (int)slot_b;
            message.moved = (int)moved;
            send_message_guess(
                g_world_connection_guess, 0x212C, &message, 0xC, 1);
        }
        *src_slot = src;
        *dst_slot = dst;
        return moved != 0;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a1d83
// Checks whether an item type can form an inventory stack.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: Combine, name evidence weak; TAKP: CanDropItem)
// __cdecl. 1 for equipment (type 0) whose `stackable` (0xBB, signed) is positive and whose item type is
// one of 14, 15, 17, 18, 19, 27, 28, 37, 38.
int Combine_guess(EQ_Equipment* item)
{
    char type;

    if (item != NULL && item->item.type == 0 && item->stackable > 0)
    {
        type = item->item_type;
        if (type == 14 || type == 15 || type == 17 || type == 18 || type == 19 || type == 27 || type == 28
            || type == 37 || type == 38)
            return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a1dce
// Maps an equipment slot to its character-model texture part.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Equipment slot -> texture slot of the player model: 2 -> 0 (head?), 6/8/17 -> 1, 7 -> 2, 9/10 -> 3,
// 12 -> 4, 18/20 -> 5, 19 -> 6, 13 -> 7, 14 -> 8; every other slot 0.
char GetTextureSlot(short slot)
{
    switch (slot)
    {
    case 2:
        return 0;
    default:
        return 0;
    case 13:
        return 7;
    case 7:
        return 2;
    case 9:
    case 10:
        return 3;
    case 12:
        return 4;
    case 14:
        return 8;
    case 17:
        return 1;
    case 18:
    case 20:
        return 5;
    case 19:
        return 6;
    case 6:
    case 8:
        return 1;
    }
}

// FUNCTION: TRILOGY 0x004a204d
// Updates the character's light from worn and carried items, applying night vision rules.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Chooses the equipment light with the strongest intensity, then applies the small-light
// suppression used while the actor is marked as having night vision.
void UpdateLight_guess(int characterProfile)
{
    int player;
    int actor;
    u8* item_light_ptr;
    char** item_slot;
    int count;
    u8 best_light;
    u8 best_intensity;
    u8 intensity;
    u8 old_light;

    if (characterProfile != 0 && *(int*)(characterProfile + 0xa0) != 0
        && *(int*)(*(int*)(characterProfile + 0xa0) + 0x70) != 0)
    {
        player = *(int*)(characterProfile + 0xa0);
        best_intensity = 0;
        best_light = 0;
        old_light = *(u8*)(player + 0xa2);
        item_slot = (char**)(characterProfile + 0xe4);
        count = 0x15;
        do
        {
            char* item;
            item = *item_slot;
            if (item != NULL && item[0x81] == 0)
            {
                item_light_ptr = (u8*)(item + 0xbd);
                intensity = EQ_METHOD(1, u8, 0x004108F2, (CDisplay*, u8), g_cdisplay_guess)(
                     *item_light_ptr);
                if (intensity > best_intensity)
                {
                    best_intensity = intensity;
                    best_light = *item_light_ptr;
                }
            }
            item_slot++;
            count--;
        } while (count != 0);

        item_slot = (char**)(characterProfile + 0x138);
        count = 8;
        do
        {
            char* item;
            item = *item_slot;
            if (item != NULL && item[0x81] == 0)
            {
                item_light_ptr = (u8*)(item + 0xbd);
                intensity = EQ_METHOD(1, u8, 0x004108F2, (CDisplay*, u8), g_cdisplay_guess)(
                     *item_light_ptr);
                if (*item_light_ptr >= 9 && *item_light_ptr <= 13 && *item_light_ptr > intensity)
                    best_light = *item_light_ptr;
            }
            item_slot++;
            count--;
        } while (count != 0);

        player = *(int*)(characterProfile + 0xa0);
        actor = 0;
        if (player != 0)
            actor = *(int*)(player + 0x70);
        if (actor != 0 && *(char*)(actor + 0x99) != 0 && best_light > 0
            && (best_light <= 2 || best_light == 4 || best_light == 6))
            best_light = 0;
        *(u8*)(player + 0xa2) = best_light;
        if (*(u8*)(*(int*)(characterProfile + 0xa0) + 0xa2) != old_light)
            EQ_METHOD(1, void, 0x00410602, (CDisplay*, int), g_cdisplay_guess)(*(int*)(characterProfile + 0xa0));
    }
}

// FUNCTION: TRILOGY 0x004a2180
// __cdecl. Tests a slot mask and preserves the original item-type, range, and character-race exceptions.
/* fake match: 0x1b/0x1c as an else arm and split 1/2 tests place the shared return 0 where the original has it */
int CanItemEquipInSlot_guess(void* character, u32 slot_mask, short slot, EQ_Equipment* item)
{
    int result;
    u32 slot_bit;
    u8 item_type;
    EQ_Equipment* character_at_114;
    EQ_Equipment* character_at_118;

    result = 0;
    if (character == NULL)
        return 0;

    // x86 masks the shift count to 5 bits, as the original's shl does
    slot_bit = 1u << slot;
    if (slot >= 0x16 && slot <= 0x1d)
        return 1;

    item_type = item->item.type;
    if (item_type == 1)
        return 0;
    if (item_type == 2)
        return 0;

    if (slot == 0xb)
    {
        if (item->item_type != 0x1b && item->item_type != 0x1c)
        {
            if (item->range > 0)
                return 1;
        }
        else
            return 0;
    }
    else if (slot == 0x15)
    {
        if (item->item_type == 0x13)
            return 1;
        if (item->item_type == 0x1b)
            return 1;
        if (item->item_type == 0x1c)
            return 1;
    }

    if (slot_mask & slot_bit)
    {
        result = 1;
        if (slot == 0xd)
        {
            character_at_118 = *(EQ_Equipment**)((char*)character + 0x118);
            if (character_at_118 != NULL)
            {
                if (item->item_type == 1 || item->item_type == 4 || item->item_type == 0x23)
                    result = 0;
                else if (character_at_118->item_type >= 0x17 && character_at_118->item_type <= 0x1a)
                    result = 0;
            }
        }
        if (slot == 0xe)
        {
            if (item->item_type == 1 || item->item_type == 4 || item->item_type == 0x23)
                result = 0;
            else if (EQ_METHOD(0, int, 0x0043624B, (EQ_Equipment*), item)() != 0 &&
                EQ_METHOD(1, u8, 0x0041A3E9, (void*, int), character)(0x16) == 0)
                result = 0;
            else
            {
                character_at_114 = *(EQ_Equipment**)((char*)character + 0x114);
                if (character_at_114 != NULL)
                {
                    if (item->item_type >= 0x17 && item->item_type <= 0x1a)
                        result = 0;
                    else if (character_at_114->item_type == 1 || character_at_114->item_type == 4 ||
                        character_at_114->item_type == 0x23)
                        result = 0;
                }
            }
        }
    }

    return result;
}

// FUNCTION: TRILOGY 0x004a22b6
// Applies a character's level and progress toward the next level.
// @early-stop: packed arguments still stage through the parameter slot and zero EDX; the original pushes partial EAX values directly.
int SetLevelAndExp_guess(char* characterProfile, float levelProgress)
{
    int newLevel;
    int tmp;
    int upperExp;
    u8 levelByte;
    u32 raceArg;
    u32 classArg;
    u32 temp;
    union PackedArg
    {
        u32 dword;
        u16 word[2];
        u8 byte[4];
    } packed;

    tmp = 0x32;
    newLevel = (int)levelProgress;
    levelByte = (u8)newLevel;
    levelProgress = levelProgress - (float)newLevel;
    if (*(int*)(characterProfile + 0xae0) != 0)
        tmp = 0x3c;
    if (newLevel > tmp)
        return 0;

    tmp = 1;
    if (newLevel != 1)
    {
        packed.dword = (u32)newLevel;
        packed.word[0] = *(u16*)(characterProfile + 0x34);
        raceArg = packed.dword;
        packed.byte[0] = *(u8*)(characterProfile + 0x36);
        classArg = packed.dword;
        packed.byte[0] = (u8)(levelByte - 1);
        temp = packed.dword;
        tmp = EQ_METHOD(3, int, 0x004807FC, (void*, int, int, int), characterProfile)((int)temp, (int)classArg, (int)raceArg);
    }

    packed.dword = (u32)tmp;
    packed.word[0] = *(u16*)(characterProfile + 0x34);
    raceArg = packed.dword;
    packed.byte[0] = *(u8*)(characterProfile + 0x36);
    classArg = packed.dword;
    upperExp = EQ_METHOD(3, int, 0x004807FC, (void*, int, int, int), characterProfile)(newLevel, (int)classArg, (int)raceArg);
    temp = (u32)upperExp - (u32)tmp;
    newLevel = (int)((float)temp * levelProgress);
    *(int*)(characterProfile + 0x3c) = newLevel + tmp;
    *(u8*)(characterProfile + 0x38) = levelByte;
    if (levelByte > *(u8*)(characterProfile + 0x1048))
        *(u8*)(characterProfile + 0x1048) = levelByte;
    return 1;
}

// FUNCTION: TRILOGY 0x004a2351
// Auto-inventory for the cursor item: stacks or equips it in a fitting worn slot first, else general inventory
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Auto-places the item held in *cursorItem (cursor) for the character characterProfile: first merges it into
// matching stackable items in the equipment slots, then puts it in the first empty slot (21 down to 1) it can
// be worn in; a leftover goes to AutoPlaceItem_guess, otherwise the inventory windows are marked dirty.
// @early-stop: loop 1 reads profile from [ebp+8] where the original keeps it in ESI (2 insns); VC6 copy-propagates the parameter into those call arguments.
void AutoInventory_guess(int characterProfile, int* cursorItem, int fromSlot)
{
    int local_8;
    struct AutoInventoryMessage_guess
    {
        int local_14;
        int local_10;
        int local_c;
    } message;
    int profile;
    int tmp;
    int* itemSlot;

    local_8 = *cursorItem;
    if (*(char*)(EQ_GLOBAL(int, 0x006E4DE0) + 0x3d) != 0)
        return;
    if (cursorItem == NULL)
        return;
    profile = characterProfile;
    if (profile == 0)
        return;
    if (EQ_CDECL(int, 0x004A1D83, (int))(local_8) != 0)
    {
        itemSlot = (int*)(profile + 0x134);
        tmp = 0x15;
        do
        {
            if (local_8 != 0
                && EQ_CDECL(int, 0x004A2180, (int, u32, int, int))
                    (profile, *(u32*)(local_8 + 0x88), tmp, local_8) != 0
                && EQ_THISCALL(int, 0x0041BA72, (int, int, int))(profile, 0, local_8) != 0
                && *itemSlot != 0
                && *(short*)(*itemSlot + 0x82) == *(short*)(local_8 + 0x82))
            {
                EQ_CDECL(int, 0x004A1C7C, (int*, int*, int, int))(&local_8, itemSlot, fromSlot, tmp);
            }
            tmp = tmp - 1;
            itemSlot = itemSlot - 1;
        } while ((short)tmp >= 1);
    }
    if (local_8 != 0)
    {
        if (*(char*)(local_8 + 0x81) == 0)
        {
            characterProfile = 0x15;
            do
            {
                if (local_8 != 0
                    && EQ_CDECL(int, 0x004A2180, (int, u32, int, int))
                        (profile, *(u32*)(local_8 + 0x88), characterProfile, local_8) != 0
                    && EQ_THISCALL(int, 0x0041BA72, (int, int, int))(profile, 0, local_8) != 0)
                {
                    tmp = (int)(short)characterProfile;
                    itemSlot = (int*)(profile + 0xe0 + tmp * 4);
                    if (*itemSlot == 0)
                    {
                        if ((short)characterProfile < 0x16 && EQ_GLOBAL(int, 0x006E4DD8) != 0)
                            EQ_THISCALL(void, 0x004E34FA, (int, int, int, int))
                                (EQ_GLOBAL(int, 0x006E4DD8), 0, 0x92, 0);
                        EQ_CDECL(void, 0x004A1E2A, (int, int, int, int))(profile, characterProfile, local_8, *itemSlot);
                        *itemSlot = local_8;
                        message.local_14 = (int)(short)fromSlot;
                        message.local_10 = tmp;
                        message.local_c = 0;
                        send_message_guess
                            (&EQ_GLOBAL(void*, 0x006B3194), 0x212c, &message, 0xc, 1);
                        local_8 = 0;
                        if ((short)characterProfile == 0xd)
                        {
                            if (*(int*)(profile + 0xa0) != 0)
                            {
                                tmp = *(int*)(profile + 0x114) + 0x5f;
                                EQ_THISCALL(void, 0x0040B6E0, (int, int, int, int, int))
                                    (EQ_GLOBAL(int, 0x006E4DE0), 0, *(int*)(profile + 0xa0),
                                     EQ_CDECL(int, 0x004A1DCE, (int))(0xd), tmp);
                            }
                            tmp = *(int*)(profile + 0x118);
                            if (tmp != 0)
                            {
                                if (*(int*)(profile + 0xa0) != 0)
                                    EQ_THISCALL(void, 0x0040B6E0, (int, int, int, int, int))
                                        (EQ_GLOBAL(int, 0x006E4DE0), 0, *(int*)(profile + 0xa0),
                                         EQ_CDECL(int, 0x004A1DCE, (int))(0xe), tmp + 0x5f);
                            }
                            else
                            {
                                if (*(int*)(profile + 0xa0) != 0)
                                    EQ_THISCALL(void, 0x0040B6E0, (int, int, int, int, char*))
                                        (EQ_GLOBAL(int, 0x006E4DE0), 0, *(int*)(profile + 0xa0),
                                         EQ_CDECL(int, 0x004A1DCE, (int))(0xe), EQ_ARRAY(char, 0x0053D860));
                            }
                        }
                        else if ((short)characterProfile == 0xe)
                        {
                            if (*(int*)(profile + 0xa0) != 0)
                            {
                                tmp = *(int*)(profile + 0x118) + 0x5f;
                                EQ_THISCALL(void, 0x0040B6E0, (int, int, int, int, int))
                                    (EQ_GLOBAL(int, 0x006E4DE0), 0, *(int*)(profile + 0xa0),
                                     EQ_CDECL(int, 0x004A1DCE, (int))(0xe), tmp);
                            }
                            tmp = *(int*)(profile + 0x114);
                            if (tmp != 0)
                            {
                                if (*(int*)(profile + 0xa0) != 0)
                                    EQ_THISCALL(void, 0x0040B6E0, (int, int, int, int, int))
                                        (EQ_GLOBAL(int, 0x006E4DE0), 0, *(int*)(profile + 0xa0),
                                         EQ_CDECL(int, 0x004A1DCE, (int))(0xd), tmp + 0x5f);
                            }
                            else
                            {
                                if (*(int*)(profile + 0xa0) != 0)
                                    EQ_THISCALL(void, 0x0040B6E0, (int, int, int, int, char*))
                                        (EQ_GLOBAL(int, 0x006E4DE0), 0, *(int*)(profile + 0xa0),
                                         EQ_CDECL(int, 0x004A1DCE, (int))(0xd), EQ_ARRAY(char, 0x0053D860));
                            }
                        }
                        EQ_CDECL(void, 0x004A204D, (int))(profile);
                    }
                }
                characterProfile = characterProfile - 1;
            } while ((short)characterProfile >= 1);
        }
        if (local_8 != 0)
        {
            EQ_CDECL(void, 0x004A2A0F, (int, int*, int, int))(profile, &local_8, fromSlot, 0);
        }
        else
        {
            EQ_THISCALL(void, 0x004C535D, (int, int))(EQ_GLOBAL(int, 0x006EFE2C), 0);
        }
    }
    else
    {
        EQ_THISCALL(void, 0x004C535D, (int, int))(EQ_GLOBAL(int, 0x006EFE2C), 0);
    }
    *cursorItem = local_8;
    if (*(int*)(profile + 0xe0) == 0)
        g_cdisplay_guess->itemOnCursor_guess = 0;
    else
        g_cdisplay_guess->itemOnCursor_guess = 1;
    EQ_THISCALL(void, 0x004C53C2, (int, int, int))(EQ_GLOBAL(int, 0x006EFE2C), 0, 0);
}

// FUNCTION: TRILOGY 0x004a25ff
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac debug map; high-confidence name)
// __cdecl. Checks the item-size and quiver/bandolier rules, optionally printing the original error text.
int CanGoInBag(EQ_Equipment* item, void* container, int show_message)
{
    u8 item_type;
    u8 item_id;
    u8 container_type;
    u8 item_size;
    u8 container_size;
    const char* message;

    if (item == NULL || container == NULL)
        return 0;

    item_type = item->item.type;
    if (item_type == 1)
    {
        message = (const char*)0x005622F8;
    }
    else
    {
        item_size = item->item.size;
        container_size = *(u8*)((char*)container + 0xd7);
        if (item_size > container_size)
        {
            message = (const char*)0x005622C8;
        }
        else
        {
            container_type = *(u8*)((char*)container + 0xd4);
            if (container_type == 2)
            {
                if (item_type != 0)
                {
                    message = (const char*)0x005622A8;
                }
                else
                {
                    item_id = item->item_type;
                    if (item_id != 0x1b && item_id != 0x2b)
                        message = (const char*)0x005622A8;
                    else
                        message = NULL;
                }
            }
            else
            {
                message = NULL;
            }

            if (message == NULL)
            {
                if (container_type != 8)
                    return 1;
                if (item_type == 0 && item->item_type == 0x13)
                    return 1;
                message = (const char*)0x00562284;
            }
        }
    }

    if (show_message == 0)
        return 0;
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(message);
    return 0;
}

// FUNCTION: TRILOGY 0x004a26aa
// Removes other copies of one lore item from inventory and bank.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
int DelLoreItemDup(char* profile, int inventory_slot, int containerIndex, short item_number, int lore_item)
{
    int inventory_index;
    int secondary_count;
    int* inventory_item;
    int* child_item;
    int item;

    if (*(char*)(lore_item + 0x23) != '*')
        return 0;

    inventory_index = 0;
    inventory_item = (int*)(profile + 0xe0);
    do
    {
        item = *inventory_item;
        if (item != 0)
        {
            if (*(char*)(item + 0x81) == 1)
            {
                lore_item = 0;
                if (*(u8*)(item + 0xd5) > 0)
                {
                    child_item = (int*)(item + 0xac);
                    do
                    {
                        if ((inventory_index != inventory_slot || lore_item != containerIndex) &&
                            *child_item != 0 &&
                            *(short*)(*child_item + 0x82) == item_number)
                        {
                            EQ_CDECL(void, 0x005218BB, (void*))((void*)*child_item);
                            *child_item = 0;
                        }
                        lore_item++;
                        child_item++;
                    } while (lore_item < (int)*(u8*)(item + 0xd5));
                }
            }

            item = *inventory_item;
            if (*(short*)(item + 0x82) == item_number && inventory_index != inventory_slot)
            {
                if (*(char*)(item + 0x81) == 1)
                {
                    if (item != 0)
                    {
                        EQ_METHOD(0, void, 0x0043405A, (EQ_Container*), (EQ_Container*)item)();
                        EQ_CDECL(void, 0x005218BB, (void*))((void*)item);
                    }
                }
                else
                {
                    EQ_CDECL(void, 0x005218BB, (void*))((void*)item);
                }
                *inventory_item = 0;
            }
        }
        inventory_index++;
        inventory_item++;
    } while (inventory_index < 0x1e);

    inventory_slot = 8;
    inventory_item = (int*)(profile + 0xf44);
    do
    {
        item = *inventory_item;
        if (item != 0)
        {
            if (*(char*)(item + 0x81) == 1)
            {
                lore_item = 0;
                if (*(u8*)(item + 0xd5) > 0)
                {
                    child_item = (int*)(item + 0xac);
                    do
                    {
                        if (*child_item != 0 && *(short*)(*child_item + 0x82) == item_number)
                        {
                            EQ_CDECL(void, 0x005218BB, (void*))((void*)*child_item);
                            *child_item = 0;
                        }
                        lore_item++;
                        child_item++;
                    } while (lore_item < (int)*(u8*)(item + 0xd5));
                }
            }

            item = *inventory_item;
            if (*(short*)(item + 0x82) == item_number)
            {
                if (*(char*)(item + 0x81) == 1)
                {
                    if (item != 0)
                    {
                        EQ_METHOD(0, void, 0x0043405A, (EQ_Container*), (EQ_Container*)item)();
                        EQ_CDECL(void, 0x005218BB, (void*))((void*)item);
                    }
                }
                else
                {
                    EQ_CDECL(void, 0x005218BB, (void*))((void*)item);
                }
                *inventory_item = 0;
            }
        }
        inventory_item++;
        inventory_slot--;
        if (inventory_slot == 0)
            return 0;
    } while (1);
}

// FUNCTION: TRILOGY 0x004a2816
// Scans carried items and removes duplicate copies of lore items.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
int CheckDupLoreItems(int characterProfile)
{
    int loreItem;
    int containedLoreItem;
    int *containedEntryPtr;
    int *inventoryEntryPtr;
    int containerIndex;
    int local_8;

    local_8 = 0;
    inventoryEntryPtr = (int *)(characterProfile + 0xe0);
    do
    {
        loreItem = *inventoryEntryPtr;
        if (loreItem != 0)
        {
            if (*(char *)(loreItem + 0x81) == '\x01')
            {
                containerIndex = 0;
                if (*(u8 *)(loreItem + 0xd5) > 0)
                {
                    containedEntryPtr = (int *)(loreItem + 0xac);
                    do
                    {
                        containedLoreItem = *containedEntryPtr;
                        if (containedLoreItem != 0)
                        {
                            EQ_CDECL(void, 0x004A26AA, (int, int, int, short, int))
                                (characterProfile, local_8, containerIndex, *(short *)(containedLoreItem + 0x82), containedLoreItem);
                        }
                        containerIndex = containerIndex + 1;
                        containedEntryPtr = containedEntryPtr + 1;
                    } while (containerIndex < (int)(u8)*(u8 *)(loreItem + 0xd5));
                }
            }
            loreItem = *inventoryEntryPtr;
            EQ_CDECL(void, 0x004A26AA, (int, int, int, short, int))
                (characterProfile, local_8, -1, *(short *)(loreItem + 0x82), loreItem);
        }
        local_8 = local_8 + 1;
        inventoryEntryPtr = inventoryEntryPtr + 1;
    } while (local_8 < 0x1e);
    return 0;
}

// FUNCTION: TRILOGY 0x004a28ad
// Checks whether a lore item is already held in inventory, bags, or bank.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
int HasLoreItem(CharacterProfile* pc, short itemNumber, EQ_Item* item)
{
    if (item != NULL && item->lore[0] == '*')
    {
        int slot;
        for (slot = 0; slot < 30; ++slot)
        {
            if (pc->inventory[slot] == itemNumber)
                return 1;
            EQ_Item* held = pc->inventory_items[slot];
            if (held != NULL)
            {
                if (held->item_nr == itemNumber)
                    return 1;
                if (held->type == EQ_ITEM_CONTAINER)
                {
                    int bag = slot == 0 ? 8 : slot - 22;
                    EQ_Container* container = (EQ_Container*)held;
                    // The original permits negative bag indices and counts beyond ten.
                    for (int index = 0; index < container->slots; ++index)
                    {
                        if (pc->bag_inventory[bag * 10 + index] == itemNumber)
                            return 1;
                        if (container->contents[index] != NULL && container->contents[index]->item_nr == itemNumber)
                            return 1;
                    }
                }
            }
        }
        for (int bank = 0; bank < 8; ++bank)
        {
            if (pc->bank_inventory[bank] == itemNumber)
                return 1;
            EQ_Item* held = pc->bank_items[bank];
            if (held != NULL)
            {
                if (held->item_nr == itemNumber)
                    return 1;
                if (held->type == EQ_ITEM_CONTAINER)
                {
                    EQ_Container* container = (EQ_Container*)held;
                    for (int index = 0; index < container->slots; ++index)
                    {
                        if (pc->bank_bag_inventory[bank * 10 + index] == itemNumber)
                            return 1;
                        if (container->contents[index] != NULL && container->contents[index]->item_nr == itemNumber)
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// The ground item node AutoPlaceItem_guess builds when nothing fits: an EQItemList entry (0xF0 bytes, the
// OP_DropItem message is a copy of it). Its constructor is the client's EQItemList constructor, only declared
// here, so VC6 keeps the EH frame around the `new` (it can't prove the constructor doesn't throw).
class GroundItemNode_guess
{
public:
    static void* operator new(size_t size)
    {
        return EQ_CDECL(void*, 0x005218C6, (u32))((u32)size);
    }
    static void operator delete(void* value)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(value);
    }
    // EXTERNAL: TRILOGY 0x004A02E5 GroundItemNode_guess::GroundItemNode_guess
    GroundItemNode_guess(void);
    ~GroundItemNode_guess(void)
    {
        EQ_METHOD(0, void, 0x004A0396, (void*), this)();
    }

    u8 unknown0x00[8];          // 0x00  list links
    s16 item_nr;                // 0x08
    u8 unknown0x0A[30];         // 0x0A
    u32 y;                      // 0x28
    u32 x;                      // 0x2C
    u32 z;                      // 0x30
    u8 unknown0x34[4];          // 0x34
    char actor_name[30];        // 0x38
    s16 items_in_bag[10];       // 0x56
    u8 unknown0x6A[6];          // 0x6A
    s32 cost;                   // 0x70
    u8 properties[10];          // 0x74  the 10 bytes at item + 0xD8
    u8 bag_properties[10][10];  // 0x7E
    u8 unknown0xE2[2];          // 0xE2
    u32 weight;                 // 0xE4
    u8 unknown0xE8[4];          // 0xE8
    u32 icon_and_type;          // 0xEC
};
C_ASSERT(sizeof(GroundItemNode_guess) == 0xF0);

// FUNCTION: TRILOGY 0x004a2a0f
// Stows an item in the general slots or their bags, stacking first; with no room it may drop to the ground
// __cdecl. Puts the item in *pitem into the inventory of `pc`: onto matching stacks in the general slots
// 22..29 and in the bags there, then into the first empty general slot, then into an empty bag slot (slot
// numbers 250 + 10 * bag + index). What is left is dropped on the ground (CanDrop, OP_DropItem 0x2035, the
// item and the contents of a dropped bag are deleted) unless `from_slot` is 10000 or more (then *pitem keeps
// it and 0 is returned). *pitem gets the rest (0 when all is placed or dropped). Returns the last slot used;
// -1 when nothing was placed, after a drop, and when CanDrop refuses (then *pitem is left untouched).
int AutoPlaceItem_guess(CharacterProfile* pc, EQ_Item** itemPtr, short from_slot)
{
    EQ_Item* item;
    short j;
    int result;
    short slot;
    short empty;
    short k;
    EQ_Container* bag;
    EQ_Container* container;
    GroundItemNode_guess* node;
    struct PlaceItemMessage_guess
    {
        int from_slot;
        int to_slot;
        int quantity;
    } message;

    result = -1;
    item = *itemPtr;
    if (item == NULL)
        return -1;
    if (pc == NULL)
        return -1;
    if (EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(item) != 0)
    {
        for (slot = 22; slot <= 29; slot++)
        {
            if (item != NULL && pc->inventory_items[slot] != NULL
                && EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(pc->inventory_items[slot]) != 0
                && EQ_CDECL(int, 0x004A1C7C, (EQ_Item**, EQ_Item**, short, short))
                       (&item, &pc->inventory_items[slot], from_slot, slot) != 0)
                result = slot;
        }
        if (item != NULL)
        {
            if (item->type != EQ_ITEM_CONTAINER)
            {
                for (slot = 22; slot <= 29; slot++)
                {
                    if (item != NULL && (bag = (EQ_Container*)pc->inventory_items[slot]) != NULL
                        && bag->item.type == EQ_ITEM_CONTAINER)
                    {
                        for (j = 0; j < bag->slots; j++)
                        {
                            if (item != NULL)
                            {
                                if (bag->contents[j] != NULL
                                    && EQ_CDECL(int, 0x004A25FF, (EQ_Item*, EQ_Container*, int))(item, bag, 0) != 0
                                    && EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(bag->contents[j]) != 0
                                    && EQ_CDECL(int, 0x004A1C7C, (EQ_Item**, EQ_Item**, short, short))
                                           (&item, &bag->contents[j], from_slot, (short)(slot * 10 + 30 + j)) != 0)
                                    result = slot * 10 + 30 + j;
                            }
                        }
                    }
                }
            }
        }
    }

    if (item != NULL)
    {
        for (empty = 22; empty <= 29; empty++)
        {
            if (pc->inventory_items[empty] == NULL)
            {
                pc->inventory_items[empty] = item;
                item = NULL;
                message.from_slot = from_slot;
                message.to_slot = empty;
                message.quantity = 0;
                send_message_guess
                    (g_world_connection_guess, 0x212C, &message, 0xC, 1);
                EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(pc);
                result = empty;
                break;
            }
        }
        if (item != NULL)
        {
            if (item->type != EQ_ITEM_CONTAINER)
            {
                for (slot = 22; slot <= 29; slot++)
                {
                    if (item != NULL && (bag = (EQ_Container*)pc->inventory_items[slot]) != NULL
                        && bag->item.type == EQ_ITEM_CONTAINER)
                    {
                        for (j = 0; j < bag->slots; j++)
                        {
                            if (item != NULL)
                            {
                                if (bag->contents[j] == NULL
                                    && EQ_CDECL(int, 0x004A25FF, (EQ_Item*, EQ_Container*, int))(item, bag, 0) != 0)
                                {
                                    bag->contents[j] = item;
                                    item = NULL;
                                    message.from_slot = from_slot;
                                    message.to_slot = slot * 10 + 30 + j;
                                    message.quantity = 0;
                                    send_message_guess
                                        (g_world_connection_guess, 0x212C, &message, 0xC, 1);
                                    result = slot * 10 + 30 + j;
                                }
                            }
                        }
                    }
                }
            }
            if (item != NULL)
            {
                if (from_slot >= 10000)
                {
                    *itemPtr = item;
                    return 0;
                }
                if (EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(item, 1) == 0)
                    return -1;
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00562324), 0xF);
                if (pc->player != NULL && item != NULL && item->nodrop != 0)
                {
                    node = new GroundItemNode_guess;
                    result = -1;
                    node->y = pc->player->y_bits_guess;
                    node->x = pc->player->x_bits_guess;
                    node->z = pc->player->z_bits_guess;
                    node->item_nr = item->item_nr;
                    node->properties[0] = 0;
                    node->weight = item->weight;
                    node->icon_and_type = 0;
                    node->cost = item->cost;
                    if (item->type == EQ_ITEM_EQUIPMENT)
                        memcpy(node->properties, (char*)item + 0xD8, 10);
                    for (slot = 0; slot < 10; slot++)
                    {
                        node->items_in_bag[slot] = -1;
                        node->bag_properties[slot][0] = 0;
                    }
                    sprintf(node->actor_name, EQ_ARRAY(char, 0x0055DCC0), item->idfile);
                    if (item->type == EQ_ITEM_CONTAINER)
                    {
                        container = (EQ_Container*)item;
                        for (k = 0; k < container->slots; k++)
                        {
                            if (container->contents[k] != NULL && container->contents[k]->nodrop != 0)
                            {
                                node->items_in_bag[k] = container->contents[k]->item_nr;
                                node->properties[0] = 0;
                                if (container->contents[k]->type == EQ_ITEM_EQUIPMENT)
                                    memcpy(node->bag_properties[k], (char*)container->contents[k] + 0xD8, 10);
                                EQ_CDECL(void, 0x005218BB, (void*))(container->contents[k]);
                                container->contents[k] = NULL;
                            }
                            else if (container->contents[k] != NULL)
                            {
                                EQ_CDECL(void, 0x005218BB, (void*))(container->contents[k]);
                                container->contents[k] = NULL;
                            }
                        }
                    }
                    send_message_guess
                        (g_world_connection_guess, 0x2035, node, 0xF0, 1);
                    delete node;
                    EQ_CDECL(void, 0x005218BB, (void*))(item);
                    item = NULL;
                }
            }
        }
    }
    *itemPtr = item;
    if (pc->inventory_items[0] == NULL)
        *((u8*)g_cdisplay_guess + 0x3C) = 0;
    else
        *((u8*)g_cdisplay_guess + 0x3C) = 1;
    return result;
}

// FUNCTION: TRILOGY 0x004a2f02
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Rejects an existing group name, then inserts into the first slot still empty after numInParty.
int ProcessMSG_WLDGROUP(void* pc, char* member_name)
{
    int i;
    char* member;
    char* members;

    int cond = *member_name != '\0' && pc != NULL;
    if (cond)
    {
        member = members = (char*)pc + 0x1078;
        for (i = 0; i < 6; ++i)
        {
            if (_strcmpi(member_name, member) == 0)
                return 0;
            else
                member += 0x30;
        }
        if (((int)(EQ_METHOD(0, int, 0x00480E23, (void*), pc)())) <= 5)
        {
            member = members;
            for (i = 0; i < 6; ++i)
            {
                if (*member == '\0')
                {
                    strcpy(member, member_name);
                    return 1;
                }
                member += 0x30;
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a2f7c
// Removes a named member from the character's group roster.
// __cdecl. Clears each group member name (6 slots of 0x30 bytes at pc + 0x1078) that equals member_name
// (case-insensitive) and returns 1 if any did. 0 for an empty name or a null pc, with nothing touched.
int RemoveGroupMemberByName_guess(void* pc, char* member_name)
{
    int result;
    char* member;
    int i;

    result = 0;
    if (*member_name == '\0')
        return 0;
    if (pc == NULL)
        return 0;
    member = (char*)pc + 0x1078;
    for (i = 6; i != 0; i--)
    {
        if (_strcmpi(member_name, member) == 0)
        {
            *member = '\0';
            result = 1;
        }
        member += 0x30;
    }
    return result;
}

// FUNCTION: TRILOGY 0x004a2fc7
// @early-stop: VC6 emits a compare and cdecl call/cleanup; the original tests EAX and branches into the continuation.
// __cdecl. Guard for the body at 0x004a2fd0: 0 for a null pc, otherwise the body's result for (pc, names).
int EQ_LoadingS_SetProgressBar2_guess(void* pc, char* names)
{
    return pc != NULL
        ? EQ_CDECL(int, 0x004A2FD0, (void*, char*))(pc, names)
        : 0;
}

// FUNCTION: TRILOGY 0x004a2fd0
// Replaces the character's group roster with names from a group update.
// __cdecl. Copies six strings from names (one every 0x20 bytes, starting at names + 0x20) into the six 0x30-byte
// slots at pc + 0x1078 and returns 1. The original takes the slot base from EAX, not from its first argument; the
// guard (0x004a2fc7) is expected to tail-jump with EAX = pc (not verified), so pc is used here.
int SetGroupMemberNames_guess(void* pc, char* names)
{
    char* dst;
    char* src;
    int i;

    dst = (char*)pc + 0x1078;
    i = 6;
    src = names + 0x20;
    do
    {
        strcpy(dst, src);
        src += 0x20;
        dst += 0x30;
        i--;
    } while (i != 0);
    return 1;
}

// FUNCTION: TRILOGY 0x004a2ffc
// Renames matching members in the character's group roster.
// __cdecl. Scans the six 0x30-byte slots at pc + 0x1078. Every slot whose string equals name (case-insensitive) is
// overwritten with value; the scan goes on after a hit, so one name can replace several slots. Returns 1 when a slot
// matched, else 0. An empty name or a null pc returns 0 before any slot is looked at.
int msg_wldgroup4_guess(int pc, char* name, char* value)
{
    int result;
    char* slot;
    int i;

    result = 0;
    if (*name == 0)
        return 0;
    if (pc == 0)
        return 0;
    slot = (char*)pc + 0x1078;
    i = 6;
    do
    {
        if (_strcmpi(name, slot) == 0)
        {
            strcpy(slot, value);
            result = 1;
        }
        slot += 0x30;
        i--;
    } while (i != 0);
    return result;
}

// FUNCTION: TRILOGY 0x004a3491
// Checks whether a spawn has a clear path to a world position.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name, low)
// __cdecl. 1 when a ray from the spawn's position (one unit higher for races 8, 11 and 12) to (y, x, z) is
// clear: with the collision test switched off (0x005FE794) it is always clear; in the alternative mode
// (0x005FE790) it is clear unless the spawn is an NPC (type 1). 0 without a spawn.
int CastRay_guess(EQPlayer* from, float y, float x, float targetZ)
{
    float hit[3];
    float end[3];
    float start[3];
    unsigned short race;

    if (from == NULL)
        return 0;
    start[0] = from->y;
    start[1] = from->x;
    start[2] = from->z;
    race = from->race;
    if (race == 11 || race == 12 || race == 8)
        start[2] += 1.0f;
    end[0] = y;
    end[1] = x;
    end[2] = targetZ;
    if (g_ray_alt_mode_guess == 0)
    {
        if (g_ray_test_off_guess != 0
            || s3dRaySegmentHitsWorldSolid(g_cdisplay_guess->world_guess, *(void**)g_displayManager_guess, start, end, hit) == 0)
            return 1;
    }
    else if (from->type != 1)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004a3534
// Prepares a spawn's movement state for a client update.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Packs a spawn's physics into the 13-byte PackedPhysics wire form, clamping phys in place: y and x
// are rounded (+0.5) and z is * 10; the heading (phys[3]) is wrapped into 0..511 (at most two steps of 512,
// then 0 if still out of range) and sent as heading / 2; the speed (phys[5]) is clamped to 0..25.5 for NPCs
// (type 1) and to -12.8..12.7 for the others, and sent as speed * 10; phys[6] and phys[8] are clamped to
// -64..63 and phys[7] to -32..31, and go into the deltas as 1/16 units (phys[7] in bits 22..31, phys[8] in
// 11..21, phys[6] in 0..10); phys[9] is the delta heading.
void pack_physics(float* phys, PackedPhysics* packed, EQPlayer* physicsPlayer)
{
    int i;

    packed->y = (short)(phys[0] + 0.5f);
    packed->x = (short)(phys[1] + 0.5f);
    packed->z = (short)(phys[2] * 10.0f);
    for (i = 0; phys[3] < 0.0f && i < 2; i++)
        phys[3] += 512.0f;
    if (phys[3] < 0.0f)
        phys[3] = 0.0f;
    for (i = 0; phys[3] >= 512.0f && i < 2; i++)
        phys[3] -= 512.0f;
    if (512.0f <= phys[3])
        phys[3] = 0.0f;
    packed->heading = (unsigned char)(phys[3] * 0.5f);
    if (physicsPlayer->type == 1)
    {
        if (25.5f < phys[5])
            phys[5] = 25.5f;
        if (phys[5] < 0.0f)
            phys[5] = 0.0f;
    }
    else
    {
        if (12.7f < phys[5])
            phys[5] = 12.7f;
        int cond = phys[5] < -12.8f;
        if (cond)
            phys[5] = -12.8f;
    }
    packed->speed = (char)(phys[5] * 10.0f);
    if (phys[6] < -64.0f)
        phys[6] = -64.0f;
    if (63.0f < phys[6])
        phys[6] = 63.0f;
    if (phys[8] < -64.0f)
        phys[8] = -64.0f;
    if (63.0f < phys[8])
        phys[8] = 63.0f;
    if (phys[7] < -32.0f)
        phys[7] = -32.0f;
    if (31.0f < phys[7])
        phys[7] = 31.0f;
    packed->deltas = ((int)(phys[7] * 16.0f) << 11 | (int)(phys[8] * 16.0f) & 0x7ff) << 11 | (int)(phys[6] * 16.0f) & 0x7ff;
    packed->delta_heading = (char)phys[9];
}

// FUNCTION: TRILOGY 0x004a3717
// Applies received movement state to a spawn.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
void unpack_physics(float* phys, PackedPhysics* packed, EQPlayer* player)
{
    float old_z;
    float new_z;
    EQPlayer* spawn;
    u32 bits;
    u32 delta_x;
    u32 delta_y;
    u32 delta_z;

    spawn = player;
    old_z = phys[2];
    phys[9] = (float)(int)packed->delta_heading;
    phys[3] = (float)packed->heading + (float)packed->heading;
    if (spawn->type == 0 && spawn->actor_info != 0
        && spawn->actor_info->ride_target_guess != 0 && packed->speed == 0)
    {
        phys[5] = 0.0f;
        return;
    }
    if (spawn->levitate != 2)
    {
        phys[2] = (float)(int)packed->z * 0.1f;
    }
    else
    {
        float z_difference;

        new_z = (float)(int)packed->z * 0.1f;
        z_difference = new_z - phys[2];
        // |z_difference| > 5.0 without assigning it: an assignment makes VC6 copy the negated value
        if ((z_difference < 0.0f ? -z_difference : z_difference) > 5.0f)
            phys[2] = new_z;
    }
    phys[0] = (float)(int)packed->y;
    phys[1] = (float)(int)packed->x;
    if (spawn->type == 1)
        phys[5] = (float)(u8)packed->speed;
    else
        phys[5] = (float)(s8)packed->speed;
    phys[5] = phys[5] * 0.1f;
    bits = packed->deltas;
    delta_y = bits & 0x7ff;
    delta_z = (bits >> 11) & 0x7ff;
    delta_x = bits >> 22;
    if (delta_y & 0x400)
        delta_y |= 0xfffff800;
    if (delta_x & 0x200)
        delta_x |= 0xfffffc00;
    if (delta_z & 0x400)
        delta_z |= 0xfffff800;
    phys[6] = (float)(int)delta_y * 0.0625f;
    phys[7] = (float)(int)delta_x * 0.0625f;
    new_z = (float)(int)delta_z * 0.0625f;
    phys[8] = new_z;
    if (EQ_GLOBAL(int, 0x005FE788) == 0)
    {
        if (spawn->race != 0x48 && spawn->race != 0x49)
            return;
        if (new_z != 0.0f)
            DbgLog_guess(
                (const char*)0x005623B8, spawn->name, (double)new_z);
        phys[2] = old_z;
    }
    else if (spawn->race != 0x48 && spawn->race != 0x49)
        return;
    phys[8] = 0.0f;
    return;
}

// FUNCTION: TRILOGY 0x004a38d0
// Restores one incoming spawn update to client-readable form.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue subsystem from the call graph)
// __cdecl. Decrypts one OP_NewSpawn record (42 dwords) in place; the chained key starts at 0x005FE78C.
// The inverse of EQClassic's EncryptSpawnPacket (packet_functions.cpp). Returns 1.
s32 DecryptSpawnPacket_guess(NetSpawn* spawn)
{
    u32* data = (u32*)spawn;
    u32 key = g_spawn_crypt_key_guess;
    u32 v;
    int i;

    for (i = 0; i < 42; i++)
    {
        v = data[i] + key;
        v = ((v << 19) | (v >> 13)) - 0x65E7;
        v = (v << 23) | (v >> 9);
        data[i] = v;
        key = key - 0x65E7 + v;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004a3911
// Restores the batch of spawn updates received when entering a zone.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue subsystem EQHandleMsg, graph)
// __cdecl. Decrypts an OP_ZoneSpawns buffer (size / 4 dwords, same chained cipher as one spawn), then swaps
// dword 0 with dword (size / 4) / 2. The inverse of EQClassic's EncryptZoneSpawnPacket. Returns 1.
s32 DecryptZoneSpawnPacket_guess(u32* data, u32 size)
{
    u32 key = g_spawn_crypt_key_guess;
    u32* p = data;
    int count = size / 4;
    u32 v;
    u32 swap;
    int i;

    for (i = 0; i < count; i++)
    {
        v = *p + key;
        v = ((v << 19) | (v >> 13)) - 0x65E7;
        v = (v << 23) | (v >> 9);
        *p = v;
        p++;
        key = key - 0x65E7 + v;
    }
    swap = data[0];
    data[0] = data[count / 2];
    data[count / 2] = swap;
    return 1;
}

// FUNCTION: TRILOGY 0x004a3977
// Restores incoming character profile data for the client.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name CopyUTF8Character_guess is wrong)
// __cdecl. Decrypts an OP_PlayerProfile buffer in place before it is inflated (HandleWorldMessage case 7):
// size / 4 dwords through a chained cipher (key = 0x005FE78C + 0x65E7, rotations 17 and 25, 0x37A9), then
// swaps dword 0 with dword (size / 4) / 2. The counterpart of EQClassic's EncryptProfilePacket. Returns 1.
s32 DecryptPlayerProfilePacket_guess(u32* data, u32 size)
{
    u32* p = data;
    int count = size / 4;
    u32 key = g_spawn_crypt_key_guess + 0x65E7;
    u32 v;
    u32 swap;
    int i;

    for (i = count; i > 0; i--)
    {
        v = *p + key;
        v = ((v >> 15) | (v << 17)) - 0x37A9;
        v = (v << 25) | (v >> 7);
        *p = v;
        p++;
        key = key - 0x37A9 + v;
    }
    swap = data[0];
    data[0] = data[count / 2];
    data[count / 2] = swap;
    return 1;
}

// FUNCTION: TRILOGY 0x004a39e3
// Updates a player's client spawn state from a server spawn update.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl (returns s32). Fills an EQPlayer from a NetSpawn: with decrypt != 0 the spawn is decrypted in place first
// (DecryptSpawnPacket_guess 0x004a38d0). Copies the stats, the 9 colours and equipment slots, the name ("NameTooLong"
// when strlen > 28) and the last name, zeroes the position and velocity floats (0x34..0x60), then unpacks the physics
// (unpack_physics 0x004a3717) with DAT_005fe788 = 1 around the call. Returns 1, or 0 when either pointer is
// null (nothing written).
// @early-stop: the colour loop folds the destination pointer into a difference (sub edx,ecx; [edx+ecx]); push ebx lands at the loop, not the then-block head; the NameTooLong arm goes through ebx; strcpy cleanups merge into one add esp,0x1c.
s32 UnPackNetPlayer(EQPlayer* ptr, NetSpawn* spawn, s32 decrypt)
{
    s32 result;
    size_t nameLength;
    char* _Str;
    int equipIndex;
    int nextIndex;
    u32* srcColor;
    u32* dstColor;

    if (ptr && spawn)
    {
        if (decrypt != 0)
        {
            EQ_CDECL(s32, 0x004A38D0, (NetSpawn*))(spawn);
        }
        ptr->size = spawn->size;
        ptr->face = spawn->face;
        ptr->walk_speed = spawn->walk_speed;
        ptr->run_speed = spawn->run_speed;
        equipIndex = 0;
        srcColor = spawn->equip_colors;
        dstColor = ptr->equip_colors;
        do
        {
            // face is followed by equipment[9] in both structs, so (&face)[1..9] is equipment[0..8].
            nextIndex = equipIndex + 1;
            *dstColor = *srcColor;
            (&ptr->face)[nextIndex] = (&spawn->face)[nextIndex];
            srcColor = srcColor + 1;
            dstColor = dstColor + 1;
            equipIndex = nextIndex;
        } while (nextIndex < 9);
        ptr->spawn_id = (u32)spawn->spawn_id;
        _Str = spawn->name;
        ptr->body_type = (s32)spawn->body_type;
        ptr->pet_owner_id = (u32)spawn->pet_owner_id;
        ptr->cur_hp = (s32)spawn->hp_percent;
        ptr->max_hp = 100;
        ptr->appearance[0] = spawn->appearance[0];
        ptr->appearance[1] = spawn->appearance[1];
        ptr->appearance[2] = spawn->appearance[2];
        ptr->appearance[3] = spawn->appearance[3];
        ptr->appearance[4] = spawn->appearance[4];
        ptr->appearance[5] = spawn->appearance[5];
        ptr->weaponAnimationType = spawn->unknown0x9E;
        ptr->deity = spawn->deity;
        ptr->guild_id = spawn->guild_id;
        ptr->guild_rank = spawn->guild_rank;
        ptr->race = (u16)spawn->race;
        ptr->type = spawn->type;
        ptr->class_ = spawn->class_;
        ptr->gender = spawn->gender;
        ptr->level = spawn->level;
        ptr->invis = spawn->invis;
        ptr->sneaking = spawn->sneaking;
        ptr->pvp = spawn->pvp;
        ptr->stand_state = spawn->stand_state;
        ptr->light = spawn->light;
        ptr->anon = (s32)spawn->anon;
        ptr->afk = (s32)spawn->afk;
        ptr->unk_0x108 = (s32)spawn->unknown0x58;
        ptr->linkdead = spawn->linkdead;
        ptr->gm = spawn->gm;
        ptr->levitate = (s16)spawn->levitate;
        ptr->texture = spawn->texture;
        ptr->helm = spawn->helm;
        nameLength = strlen(_Str);
        strcpy(ptr->name, (nameLength < 0x1d) ? _Str : EQ_ARRAY(char, 0x005623D4));
        strcpy(ptr->last_name, spawn->last_name);
        result = 1;
        ptr->y = 0.0f;
        ptr->x = 0.0f;
        ptr->z = 0.0f;
        ptr->heading = 0.0f;
        ptr->pitch = 0.0f;
        ptr->speed = 0.0f;
        ptr->dy = 0.0f;
        ptr->dx = 0.0f;
        ptr->dz = 0.0f;
        ptr->delta_heading = 0.0f;
        ptr->unk_0x5C = 0.0f;
        *(f32*)&ptr->unk_0x60 = 0.0f;
        EQ_GLOBAL(s32, 0x005FE788) = 1;
        EQ_CDECL(void, 0x004A3717, (f32*, PackedPhysics*, EQPlayer*))(&ptr->y, &spawn->physics, ptr);
        EQ_GLOBAL(s32, 0x005FE788) = 0;
        return result;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a3c49
// Selects the character animation used for a melee or ranged attack.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name, low)
// __cdecl (returns int). Attack animation for a skill (or the missile's item type when `ranged`): 44 for item type 9;
// ranged: 9 for item types 17 and 27, 10 for 28, else 5; melee by skill: 0/1 -> 5, 2 -> 4, 3 -> 3, 10 -> 7,
// 21/52 -> 46, 23 -> 47, 26 -> 45, 28 -> 8, 30/71 -> 1, 38 -> 11, else 4 for item type 35 and 2 otherwise.
// Always sets *speed to 1.0.
int GetAnimationIdForAttack_guess(int skill, int itemType, int ranged, float* speed)
{
    *speed = 1.0f;
    if (itemType == 9)
    {
        *speed = 1.0f;
        return 44;
    }
    if (ranged == 0)
    {
        switch (skill)
        {
        case 0:
        case 1:
            return 5;
        case 2:
            return 4;
        case 3:
            return 3;
        case 10:
            return 7;
        case 21:
            return 46;
        case 23:
            return 47;
        case 26:
            return 45;
        case 28:
            return 8;
        case 30:
            return 1;
        case 38:
            return 11;
        case 52:
            return 46;
        case 71:
            return 1;
        }
        return itemType == 35 ? 4 : 2;
    }
    switch (itemType)
    {
    case 17:
    case 27:
        return 9;
    case 28:
        return 10;
    }
    return 5;
}

// FUNCTION: TRILOGY 0x004a3d04
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: DoesFileExist, by address)
// __cdecl. 1 when _stat finds the file.
int DoesFileExist(const char* path)
{
    struct _stat st;

    return _stat(path, &st) == 0;
}

// FUNCTION: TRILOGY 0x004a3d1f
// Checks whether the zone named by a teleport entry is available locally.
int DoesZoneFileExist_guess(const char* teleportEntry)
{
    char zone_path[128];
    int result;

    result = EQ_METHOD(2, int, 0x004BFC65, (void*, const char*, char*), EQ_GLOBAL(void*, 0x006E4D54))(teleportEntry, zone_path);
    if (result == 0)
        return 0;

    strcat(zone_path, EQ_ARRAY(char, 0x0053DD04));
    return EQ_CDECL(int, 0x004A3D04, (const char*))(zone_path);
}

// FUNCTION: TRILOGY 0x004a3d5c
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name, low)
// __cdecl. The zone id of the teleport table entry with this index, -1 when there is none.
int GetTeleportTableEntryByID_guess(int index)
{
    int count = g_teleport_count_guess;
    int i;

    for (i = 0; i < count; i++)
    {
        if (g_teleport_table_guess[i].index == index)
        {
            if (i < 0 || i >= count)
                break;
            return g_teleport_table_guess[i].zone_id;
        }
    }
    return -1;
}

// FUNCTION: TRILOGY 0x004a3d98
// Copies a client file as part of startup file repair.
// fake match: reuse the dead sourcePath parameter slot for the allocation buffer.
int copyFileWithRetry_guess(char* sourcePath, char* destinationPath, char* errorBuffer)
{
    typedef char* (__stdcall *GlobalAllocFunction_guess)(u32, u32);
    typedef void* (__stdcall *GlobalFreeFunction_guess)(void*);
    typedef void (__stdcall *SleepFunction_guess)(u32);
    typedef int (__stdcall *MessageBoxAFunction_guess)(void*, const char*, const char*, u32);
    struct StatRecord_guess
    {
        char reserved[0x14];
        u32 file_size;
        char reserved_tail[0x0c];
    } stat_record;
    char* inputPath;
    FILE* file;
    int retryCount;

    inputPath = sourcePath;
    if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))(inputPath, &stat_record) != 0)
    {
        sprintf(errorBuffer, (const char*)0x00562508, inputPath);
        return -1;
    }
    sourcePath = EQ_GLOBAL(GlobalAllocFunction_guess, 0x005341A0)(0x40, stat_record.file_size);
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))
        (inputPath, (const char*)0x0053C2BC);
    if (file == 0)
    {
        sprintf(errorBuffer, (const char*)0x005624DC, inputPath);
        EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(sourcePath);
        return -1;
    }

    if (EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))
            (sourcePath, stat_record.file_size, 1, file) != 1)
    {
        sprintf(errorBuffer, (const char*)0x005624BC, inputPath);
        EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(sourcePath);
        return -1;
    }

    EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
    retryCount = 0;
    do
    {
        file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))
            (destinationPath, (const char*)0x005588DC);
        if (file == 0)
        {
            sprintf(errorBuffer, (const char*)0x00562498, destinationPath);
            EQ_GLOBAL(SleepFunction_guess, 0x00534198)(2000);
            ++retryCount;
            if (retryCount > 3)
            {
                retryCount = 0;
                if (EQ_GLOBAL(MessageBoxAFunction_guess, 0x00534230)
                        (0, errorBuffer, (const char*)0x0056248C, 0x40005) == 2)
                {
                    sprintf(errorBuffer, (const char*)0x00562408, destinationPath);
                    EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(sourcePath);
                    return -1;
                }
            }
        }
    } while (file == 0);

    if (EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))
            (sourcePath, stat_record.file_size, 1, file) == 1)
    {
        EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(sourcePath);
        return 1;
    }

    sprintf(errorBuffer, (const char*)0x005623E0, destinationPath);
    EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
    EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(sourcePath);
    return -1;
}

// FUNCTION: TRILOGY 0x004a3ed7
// Calculates a client file checksum for startup update checks.
// Name follows a low-confidence TAKP-only mapping; original source-file provenance is unknown.
// __cdecl. Sets the output only after stat, open, and a complete one-element read succeed.
int calculateFileCRC32_guess(const char* path, u32* out_crc)
{
    typedef void* (__stdcall *GlobalAllocFunction_guess)(u32, u32);
    typedef void* (__stdcall *GlobalFreeFunction_guess)(void*);
    u8 stat_record[0x24];
    void* data;
    FILE* file;
    u32 crc;

    if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))(path, stat_record) != 0)
        return -1;

    data = EQ_GLOBAL(GlobalAllocFunction_guess, 0x005341A0)(0x40, *(u32*)(stat_record + 0x14));
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(path, (const char*)0x0053C2BC);
    if (file == NULL)
    {
        EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(data);
        return -1;
    }
    if (EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))
        (data, *(u32*)(stat_record + 0x14), 1, file) != 1)
    {
        EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(data);
        return -1;
    }
    EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
    crc = EQ_CDECL(u32, 0x004E04A2, (void*, s32))(data, (s32)*(u32*)(stat_record + 0x14));
    EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(data);
    *out_crc = crc;
    return 1;
}

// FUNCTION: TRILOGY 0x004a3f60
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Sends the SPDat.eff checksum and 256 random dword samples when the file is large enough.
// @early-stop: VC6 gives EBX to the loop counter and spills the limit; the original keeps data in EBX and spills the counter.
int SendSpellChecksum_guess(void)
{
    typedef void* (__stdcall *GlobalAllocFunction_guess)(u32, u32);
    typedef void* (__stdcall *GlobalFreeFunction_guess)(void*);
    struct SpellChecksumPacket_guess
    {
        u32 crc;
        u32 size;
        u32 words[512];
    };
    int result;
    u8 stat_record[0x24];
    char path[0x100];
    SpellChecksumPacket_guess packet;
    void* data;
    FILE* file;
    u32* sample;
    u32 limit;
    int index;

    strcpy(path, (const char*)0x00562528);
    if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))(path, stat_record) != 0)
        return 0;

    data = EQ_GLOBAL(GlobalAllocFunction_guess, 0x005341A0)(0x40, *(u32*)(stat_record + 0x14));
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(path, (const char*)0x0053C2BC);
    if (file == NULL)
    {
        result = 0;
    }
    else
    {
        if (EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))
            (data, *(u32*)(stat_record + 0x14), 1, file) != 1)
        {
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
            result = 0;
        }
        else
        {
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
            packet.crc = EQ_CDECL(u32, 0x004E04A2, (void*, s32))(data, *(u32*)(stat_record + 0x14));
            packet.size = *(u32*)(stat_record + 0x14);
            if ((int)packet.size < 0x400)
                return 0;

            sample = packet.words + 256;
            limit = (packet.size >> 2) - 1;
            int count = 0x100;
            do
            {
                index = EQ_CDECL(int, 0x004A46F6, (int, int))(0, limit);
                sample[-0x100] = index;
                *sample = ((u32*)data)[index];
                sample++;
                count--;
            } while (count != 0);

            result = 1;
            send_message_guess
                ((void**)0x006B3194, 0x2135, &packet.crc, 0x808, 1);
        }
    }
    EQ_GLOBAL(GlobalFreeFunction_guess, 0x00534038)(data);
    return result;
}

// FUNCTION: TRILOGY 0x004a4069
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name EQ_Character_Returns55, low)
// __cdecl. Always 1.
int EQ_Character_Returns55_guess(void)
{
    return 1;
}

// FUNCTION: TRILOGY 0x004a406d
// Pays a character's copper cost from the money in their purse.
// @early-stop: validation block order and branch sense still differ; five instruction-level differences remain.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: HandleMoney, high)
// __cdecl. Pays `copper` coppers from the purse, breaking silver, gold and platinum into the next coin
// down as needed. 0 (nothing changed) when the purse is worth less or the amount is negative. The worth
// is computed in int and can overflow; with nothing left to break the loop does not end (as in the
// original).
int HandleMoney(CharacterProfile* pc, int copper)
{
    if (((pc->platinum * 10 + pc->gold) * 10 + pc->silver) * 10 + pc->copper < copper)
        return 0;
    else if (copper < 0)
        return 0;
    else
    {
        while (pc->copper < copper)
        {
            if (pc->silver != 0)
            {
                pc->silver--;
                pc->copper += 10;
            }
            else if (pc->gold != 0)
            {
                pc->silver = 10;
                pc->gold--;
            }
            else if (pc->platinum != 0)
            {
                pc->gold = 10;
                pc->platinum--;
            }
        }
        pc->copper -= copper;
        return 1;
    }
}

struct ExeChecksumStat_guess
{
    u8 unknown[20];
    size_t size;
};

struct ExeChecksumMessage_guess
{
    u32 crc;
    u32 size;
    int offsets[256];
    u32 words[256];
};

typedef char* (__stdcall *GetCommandLineA_guess_t)(void);
typedef void* (__stdcall *GlobalAlloc_guess_t)(u32, size_t);
typedef void* (__stdcall *GlobalFree_guess_t)(void*);

// FUNCTION: TRILOGY 0x004a4116
// Reports the game executable's checksum to the server for client verification.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
int __cdecl SendExeChecksum_guess(void)
{
    char exePathBuffer[257];
    char filePath[255];
    ExeChecksumStat_guess statData;
    size_t pathLength;
    char* commandLine;
    char* pathChar;
    void* data;
    FILE* file;
    int result;
    int i;
    int readCount;
    ExeChecksumMessage_guess message;

    commandLine = EQ_GLOBAL(GetCommandLineA_guess_t, 0x005341A4)();
    EQ_CDECL(int, 0x00523F0B, (const char*, const char*, ...))
        (commandLine, (const char*)0x00562534, exePathBuffer + 1);
    pathLength = strlen(exePathBuffer + 1);
    if (exePathBuffer[1] == '"')
    {
        i = 0;
        if (0 < (int)pathLength)
        {
            do
            {
                exePathBuffer[i + 1] = exePathBuffer[i + 2];
                i = i + 1;
            } while (i < (int)pathLength);
        }
        pathLength = pathLength - 1;
    }
    if (pathLength != 0)
    {
        if (exePathBuffer[pathLength] == '\\')
        {
            exePathBuffer[pathLength] = '\0';
            pathLength = pathLength - 1;
        }
        if (pathLength != 0)
        {
            if (exePathBuffer[pathLength] == '"')
            {
                exePathBuffer[pathLength] = '\0';
                pathLength = pathLength - 1;
            }
            if (pathLength != 0)
            {
                do
                {
                    if ((int)(pathLength - 1) < 1)
                        break;
                    pathChar = exePathBuffer + pathLength;
                    pathLength = pathLength - 1;
                } while (*pathChar != '\\');
            }
        }
    }
    strcpy(filePath, exePathBuffer + 1);
    if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))(filePath, &statData) != 0)
        return 0;

    data = EQ_GLOBAL(GlobalAlloc_guess_t, 0x005341A0)(0x40, statData.size);
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(filePath, (const char*)0x0053c2bc);
    if (file != 0)
    {
        readCount = EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))
            (data, statData.size, 1, file);
        if (readCount == 1)
        {
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
            message.crc = EQ_CDECL(u32, 0x004E04A2, (void*, s32))(data, statData.size);
            message.size = statData.size;
            i = 0x100;
            do
            {
                message.offsets[0x100 - i] = EQ_CDECL(int, 0x004A46F6, (int, int))
                    (0, (statData.size >> 2) - 1);
                message.words[0x100 - i] = ((u32*)data)[message.offsets[0x100 - i]];
                i = i - 1;
            } while (i != 0);
            result = 1;
            send_message_guess
                (&EQ_GLOBAL(void*, 0x006B3194), 0x2139, &message, 0x808, 1);
        }
        else
        {
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
            result = 0;
        }
    }
    else
    {
        result = 0;
    }
    EQ_GLOBAL(GlobalFree_guess_t, 0x00534038)(data);
    return result;
}

// FUNCTION: TRILOGY 0x004a42aa
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name, low)
// __cdecl. 1 for the boat races 72, 73, 114 and 141.
int isSpecialRace_allBoatTypes_guess(EQPlayer* spawn)
{
    short race = spawn->race;

    if (race == 141 || race == 72 || race == 73 || race == 114)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004a42d4
// Recognizes damage attributed to melee, archery, or throwing skills.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name, low)
// __cdecl. 1 when the damage's skill (the byte at +0x1C of the OP_Damage/OP_Action record) is a melee or
// archery skill: 0..3, 8, 10, 21, 23, 26, 28, 30, 36..38, 51, 52.
int IsWeaponAttackDamageType_guess(void* damage)
{
    switch (*((u8*)damage + 0x1C))
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 8:
    case 10:
    case 21:
    case 23:
    case 26:
    case 28:
    case 30:
    case 36:
    case 37:
    case 38:
    case 51:
    case 52:
        return 1;
    }
    return 0;
}

typedef struct MissileHitMsg_guess
{
    u32 target_id;
    u32 kind;
    u32 skill;
    u32 attacker_id;
} MissileHitMsg_guess;

// FUNCTION: TRILOGY 0x004a4327
// Sets up aimed thrown and ranged attacks, including their skill and animation.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. A missile (or thrown/ranged) hit of attacker on target: aims, computes offense and to-hit, and
// sends OP_Action (0x215F) for kinds other than 0xD/0xE. Returns 0 for bad spawns, else 1.
int DoMissileHit(EQPlayer* attacker, EQPlayer* target, EQ_Equipment* ptr, u8 kind)
{
    EQPlayer* source = attacker;
    void* character;
    EQ_Equipment* item;
    void* held;
    u8 itemType;
    int skill;
    int animation;
    float speed;
    MissileHitMsg_guess msg;

    if (target == 0 || source == 0 || target->type > 1 || source->type > 1)
        return 0;
    character = source->char_info_guess;
    if (character == 0)
        return 0;
    if (kind == 0xB)
        EQ_METHOD(2, int, 0x004B11E8, (EQPlayer*, EQPlayer*, void*), source)(target, 0);
    item = ptr;
    if (item != 0)
        *(int*)&attacker = item->damage;
    else
        *(int*)&attacker = 2;
    if (item != 0 && item->item_type != 9 && (kind == 0xD || kind == 0xE)
        && (item->item_type != 0x16 || item->delay == 0x1C)
        && *(void**)((char*)character + 0xA0) != 0
        && *(u8*)((char*)*(void**)((char*)character + 0xA0) + 0x98) == 0)
        EQ_CDECL(int, 0x004284FA, (void*, void*, int*, int))(character, item, (int*)&attacker, kind);
    itemType = item->item_type;
    if (itemType == 0x16 && source->class_ == 8 && item->delay == 0x1C && source->char_info_guess != 0)
        ptr = *(EQ_Equipment**)((char*)source->char_info_guess + 0x110);
    if (itemType == 0x16 && source->class_ == 0xF && item->delay == 0x1C && source->char_info_guess != 0
        && source->level < 0x1E)
        ptr = *(EQ_Equipment**)((char*)source->char_info_guess + 0x110);
    if (itemType == 0x16 && source->class_ == 7)
    {
        switch (item->delay)
        {
        case 0x15:
        case 0x17:
        case 0x1A:
            break;
        case 0x1C:
            if (source->char_info_guess != 0 && source->level < 0x1E)
                ptr = *(EQ_Equipment**)((char*)source->char_info_guess + 0x110);
            break;
        }
    }
    if (itemType == 0x16 && item->delay == 0xA)
    {
        held = source->char_info_guess;
        if ((source->race == 10 || source->race == 9 || source->race == 2)
            && *(int*)((char*)held + 0x118) == 0 && source->race == *(s16*)((char*)held + 0x34))
            ptr = *(EQ_Equipment**)((char*)held + 0xF8);
    }
    if (itemType != 9 && itemType == 0x16 && item->delay != 8 && item->delay == 0x47)
        ptr = *(EQ_Equipment**)((char*)source->char_info_guess + 0x12C);
    if (item != 0 && itemType != 9 && item->click_effect_type >= 2 && item->effect_type == 2
        && item->click_effect_id > -1 && item->click_effect_id < 3000)
        ptr = EQ_ARRAY(EQ_Equipment*, 0x005FE9A0)[(s16)item->click_effect_id];
    if (itemType == 9)
        skill = 0xE7;
    else
    {
        if (itemType == 0x16)
            skill = item->delay;
        else
            skill = EQ_METHOD(1, u8, 0x004B422A, (EQPlayer*, u8), source)(itemType);
        EQ_METHOD(1, int, 0x0041A32C, (void*, int), character)(skill);
    }
    EQ_METHOD(1, int, 0x0041A0C5, (void*, int), source->char_info_guess)(skill);
    msg.attacker_id = source->spawn_id;
    animation = EQ_CDECL(int, 0x004A3C49, (int, int, int, float*))(skill, itemType, 0, &speed);
    if (skill == 0x47 && source->class_ == 7)
        animation = 0xB;
    else if (kind == 0xE)
        animation = 6;
    if (source->race == 0x80)
    {
        if (animation == 7)
            animation = 6;
        else if (animation == 0x2E && skill == 0x15)
            animation = 7;
    }
    msg.target_id = target->spawn_id;
    msg.kind = kind;
    if (kind != 0xD && kind != 0xE)
    {
        msg.skill = skill;
        send_message_guess(g_world_connection_guess, 0x215F, &msg, 0xC, 1);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004a461e
// Supplies a millisecond clock for gameplay and display timing.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: EqGetTime, medium)
// __cdecl. Milliseconds: the CPU tick counter (GetCpuTicks2) divided by the ticks per ms (0x006EFC68,
// signed 64-bit), truncated to 32 bits.
u32 EqGetTime(void)
{
    return (u32)(GetCpuTicks2() / g_cpu_ticks_per_ms_guess);
}

// FUNCTION: TRILOGY 0x004a4637
// Starts the client's random sequence for gameplay and presentation effects.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name, low)
// __cdecl. Seeds RandNbr's 55-entry table from the performance counter (EqGetTime when there is none):
// t[0] = seed, t[1] = 1, t[i + 2] = t[i] + t[i + 1] (30 bits), indices 0 and 31; then discards
// random_range(5, 100) numbers.
void initializeRandomNumberGenerator_guess(void)
{
    PerfCounter_guess counter;
    u32 seed;
    int i;
    int n;

    if (QueryPerformanceCounter(&counter))
        seed = counter.low;
    else
        seed = EqGetTime();
    g_rand_index_a_guess = 0;
    g_rand_table_guess[0] = seed & 0x3FFFFFFF;
    g_rand_index_b_guess = 31;
    g_rand_table_guess[1] = 1;
    for (i = 2; i < 55; i++)
        g_rand_table_guess[i] = (g_rand_table_guess[i - 1] + g_rand_table_guess[i - 2]) & 0x3FFFFFFF;
    n = random_range_guess(5, 100);
    for (; n > 0; n--)
        RandNbr();
}

// FUNCTION: TRILOGY 0x004a46b0
// Supplies a random draw for gameplay and client effects.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: RandNbr, high)
// @early-stop: index and entry registers remain reversed from the original, with the entry store after the first wrap branch.
// __cdecl. Additive lagged-Fibonacci step over 55 dwords: t[a] = (t[b] + t[a]) & 0x3FFFFFFF, both
// indices advance mod 55; returns the new value >> 6.
int RandNbr(void)
{
    int index_b = g_rand_index_b_guess;
    int index_a = g_rand_index_a_guess;
    u32& entry = g_rand_table_guess[index_a];
    u32 r;

    r = (g_rand_table_guess[index_b] + entry) & 0x3FFFFFFF;
    if (++index_a == 55)
        index_a = 0;
    entry = r;
    if (++index_b == 55)
        index_b = 0;
    g_rand_index_a_guess = index_a;
    g_rand_index_b_guess = index_b;
    return (int)r >> 6;
}

// FUNCTION: TRILOGY 0x004a46f6
// Chooses a random value within a requested range for gameplay rolls.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; name ours, was FUN_004a46f6)
// __cdecl. A random int in lo..hi: RandNbr masked to the next power of two >= hi - lo + 1, retried until in
// range. 0 for (0, 0); lo itself when the range has fewer than two values (also when hi < lo). A range of
// more than 2^30 values overflows the mask and never returns (as in the original).
int random_range_guess(int lo, int hi)
{
    int mask;
    u32 r;

    if (lo == 0 && hi == 0)
        return 0;
    hi += 1 - lo;
    if (hi <= 1)
        return lo;
    mask = 2;
    while (mask < hi)
        mask <<= 1;
    do
        r = RandNbr() & (mask - 1);
    while ((int)r >= hi);
    return r + lo;
}

// FUNCTION: TRILOGY 0x004a473e
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: number_percent, high)
// __cdecl. 1..100: RandNbr's low 7 bits, retried while above 99, plus 1.
int number_percent_guess(void)
{
    int r;

    do
        r = RandNbr() & 0x7F;
    while (r > 99);
    return r + 1;
}

// FUNCTION: TRILOGY 0x004a474d
// Picks one of several alternate results for randomized client behavior.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; name ours, was FUN_004a474d)
// __cdecl, variadic. One of its int arguments at random; the list ends with -1 and at most the first 32
// count. -1 for an empty list.
int random_choice_guess(int first, ...)
{
    {
        int count = 0;
        int v;
        int values[32];
        v = first;
    
        if (v != -1)
        {
            int* arg = &first;
            do
            {
                if (!(count >= 32))
                    {
                }
                else
                    break;
                values[count] = v;
                count++;
                arg++;
                v = *arg;
            }
            while (-1 != v);
        }
        if (count > 0)
            return values[random_range_guess(0, count - 1)];
    }
    return -1;
}

// FUNCTION: TRILOGY 0x004a4797
// Recognizes slash-command prefixes without regard to letter case.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name CXStr_isStringPrefixMatch, low:
// there is no CXStr in Trilogy)
// __cdecl. 1 when `prefix` is a case-insensitive prefix of `text`; 0 when either is empty.
int IsStringPrefix_guess(const char* prefix, const char* text)
{
    if (*prefix == '\0' || *text == '\0')
        return 0;
    return _strnicmp(prefix, text, strlen(prefix)) == 0;
}

// FUNCTION: TRILOGY 0x004a47c8
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: is_number, high)
// __cdecl. 1 for an optional sign followed by digits only; "" is 0 but a lone "+" or "-" is 1.
int is_number(char* numberInput)
{
    if (*numberInput == '\0')
        return 0;
    if (*numberInput == '+' || *numberInput == '-')
        numberInput++;
    while (*numberInput != '\0')
    {
        if (!isdigit(*numberInput))
            return 0;
        numberInput++;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004a47f9
// @early-stop: one quadrant false edge skips the original's second condition; pooled literal locations differ.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl, float in ST0. Bearing from (x1, y1) to (x2, y2), converted to client heading.
float get_bearing(float x1, float y1, float x2, float y2)
{
    double dx = (x1 - x2) >= EQ_GLOBAL(float, 0x0053510C) ? x1 - x2 : -(x1 - x2);
    double dy = (y1 - y2) >= EQ_GLOBAL(float, 0x0053510C) ? y1 - y2 : -(y1 - y2);
    double angle;

    if (!(dx >= EQ_GLOBAL(double, 0x005356D8)))
        dx = EQ_GLOBAL(double, 0x005356D8);

    angle = EQ_CDECL(double, 0x00523FF0, (double, double))(dy, dx);
    angle *= 180.0;
    angle *= 0.3183099014828645;

    if (x1 >= x2)
    {
        if (y2 >= y1)
            angle = 90.0 - angle + 90.0;
        else if (y1 >= y2)
            angle += 180.0;
    }
    else if (x2 >= x1 && y1 >= y2)
        angle = 90.0 - angle + 270.0;

    return get_bearing2_guess((float)angle);
}

// FUNCTION: TRILOGY 0x004a48ca
// Measures distance between two spawns for attack range checks.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: get_range_squared, medium)
// __cdecl, float in ST0. Passes the squared distance (z measured from each spawn's feet: z - height
// offset) to t3dFloatSqrt (the eqgfx pointer at 0x006E5258) and returns what it returns: despite the
// name, the distance itself.
float GetSpawnDistance_guess(EQPlayer* a, EQPlayer* b)
{
    float dy = a->y - b->y;
    float dx = a->x - b->x;
    float dz = (a->z - a->height_offset) - (b->z - b->height_offset);

    return t3dFloatSqrt(dz * dz + dx * dx + dy * dy);
}

// FUNCTION: TRILOGY 0x004a490e
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: get_bearing2, by address)
// __cdecl, float in ST0. Degrees -> the client's 512-step heading (times 511.5 / 360).
float get_bearing2_guess(float degrees)
{
    float heading = degrees * 511.5f;

    return heading * 0.0027777778f;
}

// FUNCTION: TRILOGY 0x004a491f
// Determines how close two characters must be for melee attacks to reach.
// @early-stop: final x87 sum uses a shorter stack cleanup (two instructions fewer).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac: get_melee_range, high)
// __cdecl, float in ST0. 14 without both spawns and actors; else 0.75 * the sum of the actors' radii
// (t3dGetScaledActorBoundingRadius), at least 14, plus the height-offset difference and 2, at most 75.
float get_melee_range(EQPlayer* a, EQPlayer* b)
{
    float dz;
    float abs_dz;
    float ra;
    float rb;
    float range;

    if (a == NULL || b == NULL || a->actor_info == NULL || b->actor_info == NULL)
        return 14.0f;
    ra = t3dGetScaledActorBoundingRadius(a->actor_info->actor);
    rb = t3dGetScaledActorBoundingRadius(b->actor_info->actor);
    dz = a->height_offset - b->height_offset;
    if (dz < 0.0f)
        abs_dz = -dz;
    else
        abs_dz = dz;
    range = (rb + ra) * 0.75f;
    if (range < 14.0f)
        range = 14.0f;
    range = abs_dz + range + 2.0f;
    if (range > 75.0f)
        range = 75.0f;
    return range;
}

// FUNCTION: TRILOGY 0x004a49bd
// Checks whether the player can issue commands or begin actions right now.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (Mac debug map)
// __cdecl. Rejects when the interface is locked, the local spawn's state byte is 0x66, a current
// song is not bardable, or one of the large UI object's command-state words is busy.
int allowslashcommand(void)
{
    EQPlayer* player;
    short spell_id;
    EQ_Main* eq_main;

    if (EQ_GLOBAL(u8, 0x006E20F4) != 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00562588), 0x111);
        return 0;
    }
    player = g_local_player_guess;
    if (player != NULL && player->stand_state == 0x66)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00562564), 0x111);
        return 0;
    }
    spell_id = player->actor_info->unknown0x248;
    if (spell_id > 0 && spell_id < 3000)
    {
        if (EQ_METHOD(2, int, 0x0041F0C0, (void*, void*, int), g_pc_guess)(((void *)(EQ_ARRAY(void*, 0x005FE9A0)[spell_id])), player->class_) == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00562538), 0x111);
            return 0;
        }
    }
    eq_main = (EQ_Main*)g_eqmain_guess;
    if (eq_main->isActive_guess != 0
        && eq_main->merchantOpen_guess == 0
        && eq_main->lootOpen_guess == 0
        && eq_main->tradeOpen_guess == 0
        && eq_main->bankOpen_guess == 0)
        return 1;
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x0055C1C8), 0x111);
    return 0;
}

// FUNCTION: TRILOGY 0x004a4f81
// Hides or reveals the targeted player's appearance in the world.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. The /hideme command: toggles the invisible flag of the current target (no target prints a hint),
// echoes it in chat, calls anonymous_guess when it turns invisible, mirrors the flag into 0x006B63EC when the
// target is the local player (opcode 0x21D4), then sends the appearance update (opcode 0x20F5).
void do_hideme_guess(void)
{
    struct HideMePacket_guess
    {
        u32 spawn_id;
        u16 type;
        u32 value;
    } packet;
    char message[256];

    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == NULL)
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00562944));
        return;
    }
    EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->invis = 1 - EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->invis;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->invis != 0)
    {
        sprintf(message, EQ_ARRAY(char, 0x0056292C),
            EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name));
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(message, 0xF);
        EQ_CDECL(void, 0x004AA52B, (EQPlayer*, char*))(EQ_GLOBAL(EQPlayer*, 0x006E4DAC), EQ_ARRAY(char, 0x0055B250));
    }
    else
    {
        sprintf(message, EQ_ARRAY(char, 0x00562918),
            EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name));
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(message, 0xF);
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == g_local_player_guess)
    {
        EQ_GLOBAL(u32, 0x006B63EC) = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->invis;
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x21D4, &packet, 0xC, 1);
    }
    packet.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id;
    packet.type = 3;
    packet.value = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->invis;
    send_message_guess
        (&EQ_GLOBAL(void*, 0x006B3194), 0x20F5, &packet, 0xC, 1);
}

#define ZONE_NAME_MAX_LENGTH 0x1F

// FUNCTION: TRILOGY 0x004a50aa
// Sends the local player's spawn ID; its gameplay purpose is unclear.
void SendLocalSpawnIdMessage_guess(void)
{
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C) != NULL)
    {
        send_message_guess(
            g_world_connection_guess, 0x20A8, &EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id, 4, 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x004a50d0
// Requests a zone change by name and reports when safe arrival coordinates are unavailable.
void RequestZoneByName_guess(int unused, char* zoneName)
{
    typedef void (__stdcall *SleepFunction)(u32);
    int nameLength;
    int startTime;
    char errorMessage[128];
    struct ZoneRequest_guess
    {
        char prefix[32];
        char zoneName[52];
    } request;

    if (strlen(zoneName) < 1 ||
        strlen(zoneName) > ZONE_NAME_MAX_LENGTH)
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*),
            EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00562978));
        return;
    }
    strcpy(request.zoneName, zoneName);
    strcpy(EQ_ARRAY(char, 0x006E1DE8), zoneName);
    startTime = EQ_CDECL(int, 0x004A461E, (void))();
    EQ_GLOBAL(u8, 0x006E1DE7) &= 0;
    send_message_guess
        (&EQ_GLOBAL(void*, 0x006B3194), 0x214F, request.prefix, 0x54, 1);
    while (EQ_METHOD(0, void, 0x004C4EE7, (CEverQuest*),
        EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(), EQ_GLOBAL(u8, 0x006E1DE7) != 2)
    {
        if (EQ_GLOBAL(u8, 0x006E1DE7) == 1)
        {
            EQ_GLOBAL(u8, 0x006E1DE7) &= 0;
            EQ_GLOBAL(u8, 0x006E1DE8) &= 0;
            nameLength = strlen(zoneName);
            send_message_guess
                (&EQ_GLOBAL(void*, 0x006B3194), 0x2208, zoneName, nameLength + 1, 1);
            return;
        }
        if ((u32)(EQ_CDECL(int, 0x004A461E, (void))() - startTime) > 60000)
            break;
        EQ_GLOBAL(SleepFunction, 0x00534198)(10);
    }
    sprintf(errorMessage, EQ_ARRAY(char, 0x005629AC), zoneName);
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int),
        EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(errorMessage, 0xD);
}
#undef ZONE_NAME_MAX_LENGTH

// FUNCTION: TRILOGY 0x004a51d6
// Sends kill requests for non-player targets and asks for confirmation on player targets.
// __cdecl. The /kill command. An empty argument (the string at 0x0058FBA4) means the current target; any other
// text is a spawn name: a missing spawn or one that isn't a player gets the kill request sent to the server and
// an echo line; a player gets the confirmation dialog. The target works the same way; no target does nothing.
void killcommand_guess(int unused, char* name)
{
    int result;
    EQPlayer* player;
    char message[128];
    struct KillNames_guess
    {
        char victim_name[30];
        char killer_name[34];
    } names;

    result = strcmp(name, EQ_ARRAY(char, 0x0058FBA4));
    if (result != 0)
    {
        player = EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(name);
        if (player != NULL && player->type == 0)
        {
            EQ_METHOD(2, void, 0x004D5629, (void*, const char*, const char*), pinstCEverQuest_guess)(name, g_local_player_guess->name);
        }
        else
        {
            strcpy(names.victim_name, name);
            strcpy(names.killer_name, g_local_player_guess->name);
            send_message_guess
                (&EQ_GLOBAL(void*, 0x006B3194), 0x206C, names.victim_name, 0x3D, 1);
            sprintf(message, EQ_ARRAY(char, 0x0055C560), name);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xD);
        }
        return;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == NULL)
        return;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type == 0)
    {
        EQ_METHOD(2, void, 0x004D5629, (void*, const char*, const char*), pinstCEverQuest_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name, g_local_player_guess->name);
        return;
    }
    strcpy(names.victim_name, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
    strcpy(names.killer_name, g_local_player_guess->name);
    send_message_guess
        (&EQ_GLOBAL(void*, 0x006B3194), 0x206C, names.victim_name, 0x3D, 1);
    sprintf(message, EQ_ARRAY(char, 0x0055C560), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xD);
}

// FUNCTION: TRILOGY 0x004a52e1
// Requests deletion of the named or selected corpse.
/* fake match: the strcpy/send tail is written out in both arms; VC6 merges the two copies back into one, while a shared tail would assign args in the target arm and pin it in ESI */
void delcorpse_guess(EQPlayer* player, char* args)
{
    int result;
    struct CorpseNames_guess
    {
        char local_44[30];
        char local_26[34];
    } names;

    result = strcmp(args, EQ_ARRAY(char, 0x0058FBA4));
    if (result != 0)
    {
        EQPlayer* target;

        target = EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(args);
        if (target == NULL || target->type <= 1)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005629E8));
            return;
        }
        strcpy(names.local_44, args);
        strcpy(names.local_26, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
        send_message_guess
            (g_world_connection_guess, 0x21E9, names.local_44, 0x3D, 1);
        return;
    }
    else
    {
        EQPlayer* target;

        target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
        if (target == NULL || target->type <= 1)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005629E8));
            return;
        }
        strcpy(names.local_44, target->name);
        strcpy(names.local_26, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
        send_message_guess
            (g_world_connection_guess, 0x21E9, names.local_44, 0x3D, 1);
    }
}

// FUNCTION: TRILOGY 0x004a5375
// Applies a requested height change to the selected target or local player.
// __cdecl. Handles the height text: a float. A positive height asks the server to change the target's size
// with OP_SpawnAppearance (type 0x1D, height truncated) while the flags at 0x006B63F8 (byte) and 0x006B6494
// are clear; otherwise it applies the height to the player through 0x004B4836. Either way the change is
// printed first. The target (0x006E4DAC) is used when set, else the local player (0x006E4D8C).
// The local struct is the 0xC-byte layout of MsgSpawnAppearance (net_structs.h, not included here).
void height_guess(int unused, char* height_text)
{
    EQPlayer* player;
    float height;
    char message[512];
    struct MsgSpawnAppearance_guess
    {
        u32 spawn_id;           /* 0x00 */
        s16 type;               /* 0x04 */
        u16 unknown0x06;        /* 0x06 */
        u32 parameter;          /* 0x08 */
    } msg;

    player = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
    height = (float)EQ_CDECL(double, 0x00521AB0, (const char*))(height_text);
    if (player == NULL)
        player = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
    if (height > 0.0f)
    {
        if (EQ_GLOBAL(u8, 0x006B63F8) == 0 && EQ_GLOBAL(u32, 0x006B6494) == 0)
        {
            sprintf(message, EQ_ARRAY(char, 0x00562A40), player->name, player->size, height);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
            msg.spawn_id = player->spawn_id;
            msg.type = 0x1D;
            msg.parameter = (int)height;
            send_message_guess
                (g_world_connection_guess, 0x20F5, &msg, 0xC, 1);
        }
        else
        {
            sprintf(message, EQ_ARRAY(char, 0x00562A1C), player->name, player->size, height);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
            EQ_METHOD(1, void, 0x004B4836, (void*, float), player)(height);
        }
    }
}

// FUNCTION: TRILOGY 0x004a5480
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /who)
// __cdecl command handler: CEverQuest::Who_guess(args).
void cmd_who_guess(EQPlayer* player, char* args)
{
    EQ_METHOD(1, void, 0x004CA5C7, (CEverQuest*, char*), pinstCEverQuest_guess)(args);
}

// FUNCTION: TRILOGY 0x004a5490
// __cdecl. Formats the global message-of-the-day string and displays it in chat colour 15.
void displayMOTD_guess(void)
{
    char message[1024];

    sprintf(message, EQ_ARRAY(char, 0x00562A78), EQ_ARRAY(char, 0x006B7328));
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x0F);
}

// FUNCTION: TRILOGY 0x004a54c8
// Publishes the guild's message of the day.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. /guildmotd: sends the guild message of the day as opcode 0x2203, a fixed 548 bytes (name, guild id,
// motd). Not in a guild (id -1 or 0x200 and up, signed) it prints "..." and sends nothing.
// The motd is "<name> - <text>" when the short at +0x128 is above 0, the text is not empty and the result is
// under 0x200 bytes; otherwise it is empty. The name copy into the 32-byte field is unbounded, as in the original.
void do_guildmotd(int player, char* args)
{
    char message[1024];
    struct GuildMOTDPacket_guess
    {
        char name[32];      // 0x00
        int guildId;        // 0x20
        char motd[512];     // 0x24
    } packet;

    if (*(short*)(player + 0x90) == -1 || *(short*)(player + 0x90) >= 0x200)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00562A90));
        return;
    }
    if (*(short*)(player + 0x128) > 0 && strlen(args) > 0)
    {
        if (strlen((char*)(player + 1)) + 5 + strlen(args) < 0x200)
        {
            sprintf(message, EQ_ARRAY(char, 0x00562AA8), (char*)(player + 1), args);
            message[511] = '\0';
            strcpy(packet.motd, message);
            goto join;
        }
    }
    packet.motd[0] = '\0';
join:
    strcpy(packet.name, (char*)(player + 1));
    packet.guildId = *(short*)(player + 0x90);
    send_message_guess(g_world_connection_guess, 0x2203, &packet, sizeof(packet), 1);
}

// FUNCTION: TRILOGY 0x004a55b8
// Tells the player which zone their bind point is in.
void PrintBindLocation_guess(void)
{
    char message[1024];
    char zoneName[80];
    CharacterProfile* profile;
    void* zoneContext;

    profile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
    if (profile != 0 && (zoneContext = EQ_GLOBAL(void*, 0x006E4D54)) != 0)
    {
        EQ_METHOD(2, void, 0x004BEA23, (void*, const char*, char*), zoneContext)(profile->bind_zone, zoneName);
        sprintf(message, EQ_ARRAY(char, 0x00562AB0), zoneName);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
    }
}

// FUNCTION: TRILOGY 0x004a560f
// Petitions a guild war against a named player or the target for N days (default and maximum 14).
void guildwar_guess(void* unusedArg, char* commandLine)
{
    char firstWord[32];
    char* restText;
    int days;

    restText = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(commandLine, firstWord);
    days = atoi(restText);
    if (days <= 0)
    {
        days = 0x0E;
    }
    if (0x0E < days)
    {
        days = 0x0E;
    }
    if (firstWord[0] == '\0' && EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0)
    {
        strcpy(firstWord, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
    }
    EQ_METHOD(2, void, 0x004CB10B, (void*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(firstWord, days);
}

// FUNCTION: TRILOGY 0x004a566f
// Requests guild peace with a named player or the current target.
void guildpeace_guess(void* objectPtr, char* targetName)
{
    if (*targetName == '\0' && EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0)
    {
        strcpy(targetName, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
    }
    EQ_METHOD(1, void, 0x004CB1A5, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(targetName);
}

struct GuildDeleteDialog_guess
{
    u32* vtable;
    u8 unknown004[0x174];
    u32 timeout;
    u8 confirmationMode;
    u8 unknown17d[0x3c];
    char message[0x80];
};

struct GuildDeleteDisplay_guess
{
    u8 unknown000[0xbc];
    u32 currentTime;
};

// FUNCTION: TRILOGY 0x004a56d8
// Opens the guild disband confirmation for guild leaders.
void BeginGuildDisbandConfirmation_guess(void)
{
    GuildDeleteDialog_guess* confirmationDialog;

    if ((u8)g_pc_guess->guild_rank < 2)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)
            (EQ_ARRAY(char, 0x0056C720), 0x111);
        return;
    }

    confirmationDialog = EQ_GLOBAL(GuildDeleteDialog_guess*, 0x006E4E20);
    confirmationDialog->confirmationMode = 0x17;
    ((void (__fastcall*)(GuildDeleteDialog_guess*))confirmationDialog->vtable[1])(confirmationDialog);
    confirmationDialog->timeout = ((GuildDeleteDisplay_guess*)g_displayManager_guess)->currentTime + 20000;
    sprintf(confirmationDialog->message,
                                                      EQ_ARRAY(char, 0x0056C6E4));
}

// FUNCTION: TRILOGY 0x004a569a
// Copies the first word of the arguments, then calls CEverQuest::GuildInvite(word, rest of the arguments).
void guildinvite_guess(void* objectPtr, char* commandText)
{
    char word[32];
    char* rest;

    word[0] = 0;
    rest = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(commandText, word);
    EQ_METHOD(2, void, 0x004C9EF6, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(word, rest);
}

// FUNCTION: TRILOGY 0x004a5703
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /guildsay)
// __cdecl command handler: CEverQuest::GuildSay_guess(args).
void cmd_guildsay_guess(EQPlayer* player, char* args)
{
    EQ_METHOD(1, void, 0x004CA544, (CEverQuest*, char*), pinstCEverQuest_guess)(args);
}

// FUNCTION: TRILOGY 0x004a5713
// Displays a character's birth date and total play time.
// @early-stop: hoursPart/minutesPart stack homes are swapped, the hours/minutes divides run in the other order, and the strncpy cleanup folds into the sprintf cleanup.
/* fake match: minutes are split off before hours, and minutesPart is zeroed before hoursPart, to get VC6's register and store order. */
void PrintBirthdateAndPlayTime_guess(char* profileName, time_t birthTime, u32 totalMinutes)
{
    u32 leftoverMinutes;
    u32 days;
    u32 minutes;
    char message[512];
    char birthDate[64];
    char daysPart[64];
    char minutesPart[64];
    char hoursPart[64];
    char unitText[64];

    strncpy(birthDate, EQ_CDECL(char*, 0x005213D0, (const time_t*))(&birthTime), 0x18);
    birthDate[24] = 0;   // drop the '\n' that ctime() adds
    sprintf(message, EQ_ARRAY(char, 0x00562B50), profileName, birthDate);
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(message);

    days = totalMinutes / 0x5A0;   // 0x5A0 = 1440 minutes per day
    // a part with no value stays empty
    daysPart[0] = 0;
    minutesPart[0] = 0;
    hoursPart[0] = 0;
    leftoverMinutes = totalMinutes % 0x5A0;
    minutes = leftoverMinutes % 0x3C;   // 0x3C = 60 minutes per hour
    totalMinutes = leftoverMinutes / 0x3C;   // now hours (the parameter is reused)
    if (days != 0)
    {
        if (days > 1)
            sprintf(daysPart, EQ_ARRAY(char, 0x00562B44), days);
        else
            sprintf(daysPart, EQ_ARRAY(char, 0x00562B3C), days);
    }
    if (totalMinutes != 0)
    {
        if (totalMinutes > 1)
            sprintf(unitText, EQ_ARRAY(char, 0x00562B30), totalMinutes);
        else
            sprintf(unitText, EQ_ARRAY(char, 0x00562B24), totalMinutes);
        if (days != 0)
        {
            if (minutes != 0)
                sprintf(hoursPart, EQ_ARRAY(char, 0x00562B1C), unitText);
            else
                sprintf(hoursPart, EQ_ARRAY(char, 0x00562B14), unitText);
        }
        else
            strcpy(hoursPart, unitText);
    }
    if (minutes != 0)
    {
        if (minutes > 1)
            sprintf(unitText, EQ_ARRAY(char, 0x00562B08), minutes);
        else
            sprintf(unitText, EQ_ARRAY(char, 0x00562AFC), minutes);
        if (days != 0)
        {
            if (totalMinutes != 0)
                sprintf(minutesPart, EQ_ARRAY(char, 0x00562AF0), unitText);
            else
                sprintf(minutesPart, EQ_ARRAY(char, 0x00562B14), unitText);
        }
        else
        {
            if (totalMinutes == 0)
                strcpy(minutesPart, unitText);
            else
                sprintf(minutesPart, EQ_ARRAY(char, 0x00562B14), unitText);
        }
    }
    sprintf(message, EQ_ARRAY(char, 0x00562AD0), profileName, daysPart, hoursPart, minutesPart);
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(message);
}

// FUNCTION: TRILOGY 0x004a58da
// Refreshes the active character's profile time details in chat.
void UpdateProfileTimeSummary_guess(void)
{
    ProfileSummaryData_guess* profile = (ProfileSummaryData_guess*)g_pc_guess;
    EQ_CDECL(void, 0x004A5713, (char*, time_t, u32))
        ((char*)profile, profile->birthTime, profile->totalMinutes);
}

// FUNCTION: TRILOGY 0x004a58f5
// Requests an unspecified test server action for eligible GM characters.
void InvokeTestServerClassAction_guess(void)
{
    u8 classId;
    int actionCheck;
    int local_8;

    actionCheck = EQ_CDECL(int, 0x004A31A5, (CharacterProfile*))(g_pc_guess);
    if ((actionCheck != 0) && (g_local_player_guess->gm != '\0'))
    {
        classId = g_pc_guess->class_;
        if ((((classId != '\x01') && (((classId != '\a' && (classId != '\t')) && (classId != '\x03')))) &&
            (((classId != '\x04' && (classId != '\x05')) && (classId != '\b')))) && (classId != '\x0f'))
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00562B98));
            return;
        }
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00562B64));
        local_8 = 10;
        send_message_guess
            (g_world_connection_guess, 0x21E6, &local_8, 4, 1);
    }
    return;
}

// The discipline strings (messages and argument prefixes), one macro per address so the address stays visible.
#define DISC_S_0x00563030 EQ_ARRAY(char, 0x00563030)
#define DISC_S_0x00562FB0 EQ_ARRAY(char, 0x00562FB0)
#define DISC_S_0x00562F88 EQ_ARRAY(char, 0x00562F88)
#define DISC_S_0x00562F08 EQ_ARRAY(char, 0x00562F08)
#define DISC_S_0x00562EA0 EQ_ARRAY(char, 0x00562EA0)
#define DISC_S_0x00562E5C EQ_ARRAY(char, 0x00562E5C)
#define DISC_S_0x00562E1C EQ_ARRAY(char, 0x00562E1C)
#define DISC_S_0x00562DDC EQ_ARRAY(char, 0x00562DDC)
#define DISC_S_0x00562DA0 EQ_ARRAY(char, 0x00562DA0)
#define DISC_S_0x00562CD4 EQ_ARRAY(char, 0x00562CD4)
#define DISC_S_0x00562BF0 EQ_ARRAY(char, 0x00562BF0)
#define DISC_S_0x00562C40 EQ_ARRAY(char, 0x00562C40)
#define DISC_S_0x00562D98 EQ_ARRAY(char, 0x00562D98)
#define DISC_S_0x00562D90 EQ_ARRAY(char, 0x00562D90)
#define DISC_S_0x00562D88 EQ_ARRAY(char, 0x00562D88)
#define DISC_S_0x00562D84 EQ_ARRAY(char, 0x00562D84)
#define DISC_S_0x00562D7C EQ_ARRAY(char, 0x00562D7C)
#define DISC_S_0x00562D74 EQ_ARRAY(char, 0x00562D74)
#define DISC_S_0x00562D6C EQ_ARRAY(char, 0x00562D6C)
#define DISC_S_0x00562D64 EQ_ARRAY(char, 0x00562D64)
#define DISC_S_0x00562D5C EQ_ARRAY(char, 0x00562D5C)
#define DISC_S_0x00562D54 EQ_ARRAY(char, 0x00562D54)
#define DISC_S_0x00562CCC EQ_ARRAY(char, 0x00562CCC)
#define DISC_S_0x00562CC4 EQ_ARRAY(char, 0x00562CC4)
#define DISC_S_0x00562CBC EQ_ARRAY(char, 0x00562CBC)
#define DISC_S_0x00562CB4 EQ_ARRAY(char, 0x00562CB4)
#define DISC_S_0x00562CB0 EQ_ARRAY(char, 0x00562CB0)
#define DISC_S_0x00562CA8 EQ_ARRAY(char, 0x00562CA8)
#define DISC_S_0x00562CA4 EQ_ARRAY(char, 0x00562CA4)
#define DISC_S_0x00562CA0 EQ_ARRAY(char, 0x00562CA0)
#define DISC_S_0x00562C9C EQ_ARRAY(char, 0x00562C9C)
#define DISC_S_0x00562C98 EQ_ARRAY(char, 0x00562C98)
#define DISC_S_0x00562C90 EQ_ARRAY(char, 0x00562C90)
#define DISC_S_0x00562C88 EQ_ARRAY(char, 0x00562C88)
#define DISC_S_0x00562C80 EQ_ARRAY(char, 0x00562C80)
#define DISC_S_0x00562C78 EQ_ARRAY(char, 0x00562C78)
#define DISC_S_0x00562C70 EQ_ARRAY(char, 0x00562C70)
#define DISC_S_0x00562C6C EQ_ARRAY(char, 0x00562C6C)
#define DISC_S_0x00562C68 EQ_ARRAY(char, 0x00562C68)
#define DISC_S_0x00562C3C EQ_ARRAY(char, 0x00562C3C)
#define DISC_S_0x00562C38 EQ_ARRAY(char, 0x00562C38)
#define DISC_S_0x00562C34 EQ_ARRAY(char, 0x00562C34)
#define DISC_S_0x00562C30 EQ_ARRAY(char, 0x00562C30)
#define DISC_S_0x00562C2C EQ_ARRAY(char, 0x00562C2C)
#define DISC_S_0x00562C28 EQ_ARRAY(char, 0x00562C28)
#define DISC_PRINT(text) \
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(DISC_S_##text)
#define DISC_SEND() \
    EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))(g_world_connection_guess, 0x21E6, &disciplineId, 4, 1)
#define DISC_STRNICMP(text, count) \
    _strnicmp(args, DISC_S_##text, count)

// FUNCTION: TRILOGY 0x004a5984
// The /discipline command: asks the server to start a named class discipline when the player's level allows it.
// __cdecl /discipline handler (by its strings). Needs the expansion dword at 0x0AE0 and a class that has disciplines
// (1, 3, 4, 5, 7, 8, 9, 15; classes 3, 4, 5 and 8 also need SetPCloudDensity_guess, 15 the expansion flag 4). An
// empty argument prints the class usage text and still sends discipline 0; otherwise the first match of the
// argument prefixes picks the discipline id and the level it needs, and opcode 0x21E6 carries the id (4 bytes,
// reliable). Too low a level prints a refusal.
void Discipline_guess(int unused, char* args)
{
    u8 classId;
    size_t argLength;
    int requiredLevel;
    int disciplineId;

    if (g_pc_guess->expansions_guess == 0)
    {
        return;
    }
    classId = g_pc_guess->class_;
    if (((classId == 3) || (classId == 4) || (classId == 5) || (classId == 8)) &&
        (EQ_CDECL(int, 0x004A31A5, (CharacterProfile*))(g_pc_guess) == 0))
    {
        return;
    }
    if ((g_pc_guess->class_ == 0x0F) && (EQ_METHOD(0, int, 0x004197BF, (void*), g_pc_guess)() == 0))
    {
        return;
    }
    classId = g_pc_guess->class_;
    if (!((classId == 1) || (classId == 7) || (classId == 9) || (classId == 3) || (classId == 4) || (classId == 5) ||
          (classId == 8) || (classId == 0x0F)))
    {
        DISC_PRINT(0x00563030);
        return;
    }
    argLength = strlen(args);
    classId = g_pc_guess->class_;
    if (argLength < 1)
    {
        if (classId == 1)
        {
            disciplineId = 0;
            DISC_PRINT(0x00562FB0);
        }
        else if (classId == 0x0F)
        {
            disciplineId = 0;
            DISC_PRINT(0x00562F88);
        }
        else if (classId == 7)
        {
            disciplineId = 0;
            DISC_PRINT(0x00562F08);
        }
        else if (classId == 9)
        {
            DISC_PRINT(0x00562EA0);
            disciplineId = 0;
        }
        else if (classId == 3)
        {
            DISC_PRINT(0x00562E5C);
            disciplineId = 0;
        }
        else if (classId == 4)
        {
            DISC_PRINT(0x00562E1C);
            disciplineId = 0;
        }
        else if (classId == 5)
        {
            DISC_PRINT(0x00562DDC);
            disciplineId = 0;
        }
        else if (classId == 8)
        {
            DISC_PRINT(0x00562DA0);
            disciplineId = 0;
        }
        else
        {
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 7)
    {
        disciplineId = 0;
        requiredLevel = 0x3D;
        if (DISC_STRNICMP(0x00562D98, 4) == 0)
        {
            disciplineId = 0x14;
            requiredLevel = 0x3B;
        }
        if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            requiredLevel = 0x1E;
            disciplineId = 0x1E;
        }
        if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            disciplineId = 0x1F;
            requiredLevel = 0x28;
        }
        if (DISC_STRNICMP(0x00562D84, 3) == 0)
        {
            disciplineId = 5;
            requiredLevel = 0x3C;
        }
        if (DISC_STRNICMP(0x00562D7C, 4) == 0)
        {
            disciplineId = 0x0B;
            requiredLevel = 0x33;
        }
        if (DISC_STRNICMP(0x00562D74, 4) == 0)
        {
            disciplineId = 0x0C;
            requiredLevel = 0x34;
        }
        if (DISC_STRNICMP(0x00562D6C, 4) == 0)
        {
            disciplineId = 6;
            requiredLevel = 0x35;
        }
        if (DISC_STRNICMP(0x00562D64, 4) == 0)
        {
            disciplineId = 0x0D;
            requiredLevel = 0x36;
        }
        if (DISC_STRNICMP(0x00562D5C, 4) == 0)
        {
            disciplineId = 0x0E;
            requiredLevel = 0x38;
        }
        if (DISC_STRNICMP(0x00562D54, 4) == 0)
        {
            disciplineId = 0x0F;
            requiredLevel = 0x39;
        }
        if (disciplineId == 0)
        {
            DISC_PRINT(0x00562CD4);
            return;
        }
        if (g_pc_guess->level < requiredLevel)
        {
            DISC_PRINT(0x00562BF0);
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 1)
    {
        if (DISC_STRNICMP(0x00562CCC, 4) == 0)
        {
            requiredLevel = 0x3C;
            if (g_pc_guess->level < 0x3C)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 1;
        }
        else if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            requiredLevel = 0x1E;
            disciplineId = 0x1E;
        }
        else if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            disciplineId = 0x1F;
            requiredLevel = 0x28;
        }
        else if (DISC_STRNICMP(0x00562CC4, 4) == 0)
        {
            requiredLevel = 0x39;
            if (g_pc_guess->level < 0x39)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 2;
        }
        else if (DISC_STRNICMP(0x00562CBC, 4) == 0)
        {
            requiredLevel = 0x37;
            if (g_pc_guess->level < 0x37)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 3;
        }
        else if (DISC_STRNICMP(0x00562CB4, 4) == 0)
        {
            requiredLevel = 0x34;
            if (g_pc_guess->level < 0x34)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 4;
        }
        else if (DISC_STRNICMP(0x00562CB0, 3) == 0)
        {
            requiredLevel = 0x38;
            if (g_pc_guess->level < 0x38)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 6;
        }
        else if (DISC_STRNICMP(0x00562CA8, 4) == 0)
        {
            requiredLevel = 0x35;
            if (g_pc_guess->level < 0x35)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x10;
        }
        else if (DISC_STRNICMP(0x00562CA4, 3) == 0)
        {
            requiredLevel = 0x36;
            if (g_pc_guess->level < 0x36)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x11;
        }
        else if (DISC_STRNICMP(0x00562CA0, 3) == 0)
        {
            requiredLevel = 0x3A;
            if (g_pc_guess->level < 0x3A)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x0E;
        }
        else if (DISC_STRNICMP(0x00562C9C, 3) == 0)
        {
            requiredLevel = 0x3B;
            if (g_pc_guess->level < 0x3B)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x0D;
        }
        else
        {
            DISC_PRINT(0x00562FB0);
            return;
        }
        if (g_pc_guess->level < requiredLevel)
        {
            DISC_PRINT(0x00562BF0);
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 9)
    {
        if (DISC_STRNICMP(0x00562C98, 3) == 0)
        {
            if (g_pc_guess->level < 0x35)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 6;
        }
        else if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            if (g_pc_guess->level < 0x1E)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1E;
        }
        else if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            if (g_pc_guess->level < 0x28)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1F;
        }
        else if (DISC_STRNICMP(0x00562C90, 4) == 0)
        {
            if (g_pc_guess->level < 0x37)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x13;
        }
        else if (DISC_STRNICMP(0x00562C88, 4) == 0)
        {
            if (g_pc_guess->level < 0x3A)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x0F;
        }
        else if (DISC_STRNICMP(0x00562C80, 4) == 0)
        {
            if (g_pc_guess->level < 0x34)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x0E;
        }
        else if (DISC_STRNICMP(0x00562C78, 4) == 0)
        {
            if (g_pc_guess->level < 0x36)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x10;
        }
        else if (DISC_STRNICMP(0x00562C70, 4) == 0)
        {
            if (g_pc_guess->level < 0x39)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x15;
        }
        else
        {
            DISC_PRINT(0x00562EA0);
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 3)
    {
        if (DISC_STRNICMP(0x00562C6C, 3) == 0)
        {
            if (g_pc_guess->level < 0x37)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x16;
        }
        else if (DISC_STRNICMP(0x00562C68, 3) == 0)
        {
            if (g_pc_guess->level < 0x3C)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x17;
        }
        else if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            if (g_pc_guess->level < 0x33)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1E;
        }
        else if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            if (g_pc_guess->level < 0x36)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1F;
        }
        else
        {
            DISC_PRINT(0x00562E5C);
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 0x0F)
    {
        if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            if (g_pc_guess->level < 0x33)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1E;
        }
        else if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            if (g_pc_guess->level < 0x36)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1F;
        }
        else
        {
            DISC_PRINT(0x00562C40);
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 4)
    {
        if (DISC_STRNICMP(0x00562C3C, 3) == 0)
        {
            if (g_pc_guess->level < 0x37)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x18;
        }
        else if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            if (g_pc_guess->level < 0x33)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1E;
        }
        else if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            if (g_pc_guess->level < 0x36)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1F;
        }
        else if (DISC_STRNICMP(0x00562C38, 3) == 0)
        {
            if (g_pc_guess->level < 0x3C)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x19;
        }
        else
        {
            DISC_PRINT(0x00562E1C);
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 5)
    {
        if (DISC_STRNICMP(0x00562C34, 3) == 0)
        {
            if (g_pc_guess->level < 0x37)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1A;
        }
        else if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            if (g_pc_guess->level < 0x33)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1E;
        }
        else if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            if (g_pc_guess->level < 0x36)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1F;
        }
        else if (DISC_STRNICMP(0x00562C30, 3) == 0)
        {
            if (g_pc_guess->level < 0x3C)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1B;
        }
        else
        {
            DISC_PRINT(0x00562DDC);
            return;
        }
        DISC_SEND();
        return;
    }
    if (classId == 8)
    {
        if (DISC_STRNICMP(0x00562C2C, 3) == 0)
        {
            if (g_pc_guess->level < 0x37)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1C;
        }
        else if (DISC_STRNICMP(0x00562D90, 4) == 0)
        {
            if (g_pc_guess->level < 0x33)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1E;
        }
        else if (DISC_STRNICMP(0x00562D88, 5) == 0)
        {
            if (g_pc_guess->level < 0x36)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1F;
        }
        else if (DISC_STRNICMP(0x00562C28, 3) == 0)
        {
            if (g_pc_guess->level < 0x3C)
            {
                DISC_PRINT(0x00562BF0);
                return;
            }
            disciplineId = 0x1D;
        }
        else
        {
            DISC_PRINT(0x00562DA0);
            return;
        }
        DISC_SEND();
    }
}
#undef DISC_PRINT
#undef DISC_SEND
#undef DISC_STRNICMP

// FUNCTION: TRILOGY 0x004a62a6
// Manages the player's friends list by listing, adding, or removing names.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. An empty name lists the friends in the 30 x 32 byte table at 0x006e2114. Otherwise a name over 15 characters
// is refused, a name that strcmpi matches is removed, and any other name goes into a free slot. Every empty slot
// overwrites the free index, so the last empty slot wins.
// @early-stop: sprintf tail merges into the -1 arm (original jumps back into the empty-name arm), remove block sits before the free test (original: after it), strcpy pops fold into add esp,0x14.
void do_friends(int unused, const char* name)
{
    char message[256];
    char* slot;
    int count;
    int i;
    int free_slot;

    count = 0;
    free_slot = -1;
    if (*name == '\0')
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005631D4));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005631C0));
        slot = EQ_ARRAY(char, 0x006E2114);
        do
        {
            if (*slot != '\0')
            {
                count++;
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(slot);
            }
            slot = slot + 0x20;
        } while ((int)slot < 0x006E24D4);
        if (count == 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563198));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563164));
            return;
        }
        sprintf(message, EQ_ARRAY(char, 0x0056314C), count);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
    }
    else
    {
        if (strlen(name) > 15)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056311C), 13);
            return;
        }
        i = 0;
        slot = EQ_ARRAY(char, 0x006E2114);
        do
        {
            if (*slot == '\0')
            {
                free_slot = i;
            }
            else if (_strcmpi(slot, name) == 0)
            {
                sprintf(message, EQ_ARRAY(char, 0x005630A0), name);
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
                EQ_ARRAY(char, 0x006E2114)[i * 0x20] = '\0';
                EQ_METHOD(1, void, 0x004D5A24, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(0);
                return;
            }
            slot = slot + 0x20;
            i++;
        } while ((int)slot < 0x006E24D4);
        if (free_slot == -1)
        {
            sprintf(message, EQ_ARRAY(char, 0x005630C0), 30);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
        }
        else
        {
            strcpy(EQ_ARRAY(char, 0x006E2114) + free_slot * 0x20, name);
            sprintf(message, EQ_ARRAY(char, 0x00563088), name);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
            EQ_METHOD(1, void, 0x004D5A24, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(0);
        }
    }
}

// FUNCTION: TRILOGY 0x004a6431
// Stops the bard's current song or spell.
void StopBardSong_guess(void)
{
    if (g_pc_guess->class_ == 8)
        EQ_METHOD(1, void, 0x0042798F, (void*, int), g_pc_guess)(0);
}

// FUNCTION: TRILOGY 0x004a6445
// __cdecl. Tests corpse visibility and distance before reporting whether it can be summoned here.
int can_summon_corpse_from_here_guess(EQPlayer* corpse)
{
    const char* message;
    float distance;

    if (corpse->actor_info != NULL)
    {
        if (EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))
            (g_local_player_guess, corpse->y, corpse->x, corpse->z) == 0)
            message = EQ_ARRAY(char, 0x00563210);
        else
        {
            distance = EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(g_local_player_guess, corpse, 0.0f);
            if (distance > EQ_GLOBAL(float, 0x005350F4))
                message = EQ_ARRAY(char, 0x005631E4);
            else
                return 1;
        }
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(message);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a64ba
// Sets when no-drop loot prompts appear: always, for unusable items, or never.
/* fake match: volatile preserves the original stack reload in the null arm. */
void lootnodrop_guess(int context, char* commandText)
{
    int comparison;
    char chatMessage[256];

    if (context != 0)
        comparison = *(int*)((char*)context + 0x74);
    else
    {
        volatile int* parameterAddress = &context;
        comparison = *parameterAddress;
    }

    if (EQ_GLOBAL(int, 0x006B6F04) > 2 || EQ_GLOBAL(int, 0x006B6F04) < 0)
        EQ_GLOBAL(int, 0x006B6F04) = 0;

    if (*commandText == '\0')
    {
        if (comparison == 0)
            return;

        sprintf(chatMessage, EQ_ARRAY(char, 0x00563354));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(chatMessage);
        strcpy(chatMessage, EQ_ARRAY(char, 0x0056333C));

        switch (EQ_GLOBAL(int, 0x006B6F04))
        {
        case 0:
            strcat(chatMessage, EQ_ARRAY(char, 0x0056329C));
            break;
        case 1:
            strcat(chatMessage, EQ_ARRAY(char, 0x005632D0));
            break;
        case 2:
            strcat(chatMessage, EQ_ARRAY(char, 0x00563308));
            break;
        default:
            // unreachable: the clamp above keeps the mode in 0..2, and the calls before this switch don't write it
            chatMessage[0] = 0;
            break;
        }
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(chatMessage);
        return;
    }

    strcpy(chatMessage, EQ_ARRAY(char, 0x0056328C));
    comparison = _strnicmp(commandText, EQ_ARRAY(char, 0x00563284), 4);
    if (comparison == 0 || *commandText == '2')
    {
        strcat(chatMessage, EQ_ARRAY(char, 0x00563308));
        EQ_GLOBAL(int, 0x006B6F04) = 2;
    }
    else
    {
        comparison = _strnicmp(commandText, EQ_ARRAY(char, 0x0056327C), 4);
        if (comparison == 0 || *commandText == '1')
        {
            strcat(chatMessage, EQ_ARRAY(char, 0x00563238));
            EQ_GLOBAL(int, 0x006B6F04) = 1;
        }
        else
        {
            comparison = _strnicmp(commandText, EQ_ARRAY(char, 0x00563274), 4);
            if (comparison != 0 && *commandText != '0')
            {
                EQ_METHOD(0, void, 0x004CF3E1, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
                return;
            }
            strcat(chatMessage, EQ_ARRAY(char, 0x0056329C));
            EQ_GLOBAL(int, 0x006B6F04) = 0;
        }
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(chatMessage);
    EQ_METHOD(0, void, 0x004CF3E1, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
}

// FUNCTION: TRILOGY 0x004a6658
// Requests that an eligible corpse be summoned to the player.
// __cdecl, no arguments. Summons a corpse: the target when it is a corpse (type >= 2), else the first corpse
// of the spawn list (0x006E4D58) whose stripped name is the local player's. The request goes to the server
// (opcode 0x20C5, 0x6C bytes: corpse name, player name, zone, truncated y/x/z) when can_summon_corpse_from_here
// (0x004A6445) accepts it.
void do_corpse(void)
{
    struct SummonCorpse_guess
    {
        char corpse_name[30];   /* 0x00 */
        char player_name[31];   /* 0x1E */
        char zone[31];          /* 0x3D */
        int y;                  /* 0x5C */
        int x;                  /* 0x60 */
        int z;                  /* 0x64 */
        int unknown0x68;        /* 0x68 */
    } msg;
    char message[256];
    EQPlayer* corpse;
    EQPlayer* player;
    const char* text;
    int found;

    found = 0;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == NULL || EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type < 2)
    {
        strlen(EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
        corpse = EQ_GLOBAL(EQPlayer*, 0x006E4D58);
        if (corpse != NULL)
        {
            do
            {
                if (corpse != EQ_GLOBAL(EQPlayer*, 0x006E4D8C) && corpse->type >= 2)
                {
                    if (_strcmpi(EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name,
                             EQ_METHOD(1, char*, 0x004D0436, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(corpse->name)) == 0)
                    {
                        found = 1;
                        if (EQ_CDECL(int, 0x004A6445, (EQPlayer*))(corpse) != 0)
                            goto sendSummonRequest;
                    }
                }
                corpse = corpse->next;
            } while (corpse != NULL);
            goto finishSearch;
        }
    }
    else
    {
        if (EQ_CDECL(int, 0x004A6445, (EQPlayer*))(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)) == 0)
            return;
        corpse = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
sendSummonRequest:
        if (corpse != NULL)
        {
            strcpy(msg.corpse_name, corpse->name);
            strcpy(msg.player_name, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
            strcpy(msg.zone, EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->zone);
            player = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
            msg.y = (int)player->y;
            msg.x = (int)player->x;
            msg.z = (int)player->z;
            send_message_guess
                (g_world_connection_guess, 0x20C5, &msg, 0x6C, 1);
            sprintf(message, EQ_ARRAY(char, 0x005633B0),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(corpse->name));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
            return;
        }
finishSearch:
        if (found != 0)
            return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563384));
}

// FUNCTION: TRILOGY 0x004a6825
// Sends an internal client window message; its game purpose is unclear.
void PostClientWindowMessage_guess(void)
{
    typedef int (__stdcall *PostMessageA_guess_t)(void*, u32, u32, s32);

    EQ_GLOBAL(PostMessageA_guess_t, 0x0053422C)
        (EQ_GLOBAL(void*, 0x006B63D8), 0x4C8, 0, 0);
}

// FUNCTION: TRILOGY 0x004a683b
// Sets audio diagnostic logging to a file on or off.
void SetAudioDebugLogToFile_guess(int unused, const char* text)
{
    int comparison;
    const char* message;

    comparison = _strnicmp(text, EQ_ARRAY(char, 0x0055B250), 2);
    if (comparison != 0)
    {
        message = EQ_ARRAY(char, 0x005634E8);
    }
    else
    {
        message = EQ_ARRAY(char, 0x00563538);
    }
    EQ_GLOBAL(char, 0x006EFE38) = comparison == 0;
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
}

// FUNCTION: TRILOGY 0x004a6878
// Sets the voice recognition delay in milliseconds.
void vrdelay_guess(int unused, char* delay_text)
{
    char message[256];

    if (*delay_text == '\0' ||
        EQ_CDECL(int, 0x004A47C8, (char*))(delay_text) == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005636F0), 0x0F);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005636A8), 0x0F);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563634), 0x0F);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005635EC), 0x0F);
        sprintf(message, EQ_ARRAY(char, 0x005635C0), EQ_GLOBAL(int, 0x006B6EF8));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
    }
    else
    {
        EQ_GLOBAL(int, 0x006B6EF8) = atoi(delay_text);
        if (EQ_GLOBAL(int, 0x006B6EF8) < 0)
            EQ_GLOBAL(int, 0x006B6EF8) = 0;
        else if (100 < EQ_GLOBAL(int, 0x006B6EF8))
            EQ_GLOBAL(int, 0x006B6EF8) = 100;
        sprintf(message, EQ_ARRAY(char, 0x00563724), EQ_GLOBAL(int, 0x006B6EF8));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(message);
        EQ_METHOD(0, void, 0x004CF3E1, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    }
}

// FUNCTION: TRILOGY 0x004a6960
// Requests a server roll within the two entered limits.
// __cdecl /random handler (by its strings): "/random a b" sends the smaller and the larger number as opcode
// 0x21E7 (8 bytes, reliable) when both have at most 6 characters, neither is negative and the larger is at
// most 32000; otherwise the reason goes to chat. The first word is copied into 64 bytes without a size limit.
void random_guess(int unused, char* args)
{
    char* secondArg;
    size_t argLength;
    int secondNumber;
    int firstNumber;
    char word[64];
    struct RandomRange_guess
    {
        int low;
        int high;
    } range;
    int local_8;

    secondArg = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(args, word);
    argLength = strlen(secondArg);
    if (argLength <= 6 && (argLength = strlen(word), argLength <= 6))
    {
        secondNumber = atol(secondArg);
        firstNumber = atol(word);
        if (secondNumber >= 0 && firstNumber >= 0)
        {
            if (secondNumber > firstNumber)
            {
                local_8 = secondNumber;
                secondNumber = firstNumber;
            }
            else
                local_8 = firstNumber;
            if (local_8 > 32000)
            {
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563788));
                return;
            }
            range.low = secondNumber;
            range.high = local_8;
            send_message_guess
                (g_world_connection_guess, 0x21e7, &range, 8, 1);
            return;
        }
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563768));
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056374C));
}

// FUNCTION: TRILOGY 0x004a6a08
// Requests a consider on the current or specified living target.
void ConsiderTarget_guess(EQPlayer* target, char* commandText)
{
    u16 targetRace;
    /* The whole 0x1C-byte OP_Consider payload is sent; only the two ids are set, the rest is stack as it is. */
    struct MsgConsider_guess
    {
        u32 player_id;          /* 0x00 */
        u32 target_id;          /* 0x04 */
        u32 faction;            /* 0x08 */
        u32 unknown0x0C[3];     /* 0x0C */
        u32 unworthy;           /* 0x18 */
    } msg;

    if (target == NULL || *commandText == '\0')
        target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
    if (target == NULL)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005637EC));
        return;
    }
    if (target->type >= 2)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005637C4));
        return;
    }
    targetRace = target->race;
    if (targetRace != 0x48 && targetRace != 0x8D && targetRace != 0x49 && targetRace != 0x72)
    {
        msg.player_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
        msg.target_id = target->spawn_id;
        send_message_guess
            (g_world_connection_guess, 0x2137, &msg, sizeof(msg), 1);
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005637A8));
}

// FUNCTION: TRILOGY 0x004a6aa3
// Displays the player's custom emote text after checking its wording.
// __cdecl /emote handler. A lower-case copy of the text is checked for "says" (refused unless the local player is
// a GM); an empty result or two white-space characters in a row print the usage lines, otherwise a trailing letter
// or digit gets the suffix and the text goes to the chat buffer (CEverQuest +0xBA8) before CEverQuest::Emote_guess.
// Bug kept: only the length is checked, so 512 bytes or more overflow the 512-byte copy on the stack.
void emote_guess(int unused, char* text)
{
    char lower[512];
    int len;
    int i;

    len = strlen(text);
    if (len > 0x1ff)
        len = 0;
    strcpy(lower, text);
    _strlwr(lower);
    if (strstr(lower, EQ_ARRAY(char, 0x00563854)) != NULL && EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm == 0)
        len = 0;
    for (i = 0; i < len; i++)
    {
        if (i + 1 < len && isspace(text[i]) && isspace(text[i + 1]))
        {
            len = 0;
            break;
        }
    }
    if (len < 1)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563824));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005637FC));
    }
    else
    {
        if (isalpha(text[len - 1]) || isdigit(text[len - 1]))
            strcat(text, EQ_ARRAY(char, 0x00545998));
        text[strlen(text)] = '\0';
        strcpy((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8, text);
        EQ_METHOD(0, void, 0x004D0952, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    }
}

// FUNCTION: TRILOGY 0x004a6bcd
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name bored_guess is wrong: the
// slash-command table entry at 0x0056048C names it /smile)
// __cdecl command handler: social 0 for the first word of the arguments.
void social_smile_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(0, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6c02
// Sends a frown social to the named player when the player is standing still.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table entry at
// 0x005604A4 names it /frown, as the queue has it)
// __cdecl command handler: social 1 for the first word of the arguments, only while standing (state 100)
// and still (|speed| < 0.1).
void social_frown_guess(EQPlayer* player, char* args)
{
    char target[2112];

    if (g_control_player_guess->stand_state == 100
        && (g_control_player_guess->speed < 0.0f ? -g_control_player_guess->speed : g_control_player_guess->speed) < 0.1f)
    {
        EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(1, target,
            EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)("You must stand up and be still to frown.");
}

// FUNCTION: TRILOGY 0x004a6c76
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name brb_guess is wrong: the
// slash-command table entry at 0x005604B0 names it /laugh)
// __cdecl command handler: social 2 for the first word of the arguments.
void social_laugh_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(2, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6cab
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name burp_guess is wrong: the
// slash-command table entry at 0x005604BC names it /cry)
// __cdecl command handler: social 3 for the first word of the arguments.
void social_cry_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(3, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6ce0
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name bye_guess is wrong: the
// slash-command table entry at 0x005604C8 names it /chuckle)
// __cdecl command handler: social 4 for the first word of the arguments.
void social_chuckle_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(4, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6d15
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name cackle_guess is wrong: the
// slash-command table entry at 0x005604D4 names it /hug)
// __cdecl command handler: social 5 for the first word of the arguments.
void social_hug_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(5, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6d4a
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name calm_guess is wrong: the
// slash-command table entry at 0x005604E0 names it /poke)
// __cdecl command handler: social 6 for the first word of the arguments.
void social_poke_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(6, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6d7f
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name clap_guess is wrong: the
// slash-command table entry at 0x005604EC names it /bow)
// __cdecl command handler: social 7 for the first word of the arguments.
void social_bow_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(7, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6db4
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name comfort_guess is wrong: the
// slash-command table entry at 0x005604F8 names it /bounce)
// __cdecl command handler: social 8 for the first word of the arguments.
void social_bounce_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(8, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6de9
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name congratulate_guess is wrong: the
// slash-command table entry at 0x00560504 names it /panic)
// __cdecl command handler: social 9 for the first word of the arguments.
void social_panic_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(9, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6e1e
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name cough_guess is wrong: the
// slash-command table entry at 0x00560510 names it /nod)
// __cdecl command handler: social 10 for the first word of the arguments.
void social_nod_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(10, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6e53
// Sends a rolling-on-the-floor-laughing social to the named player, with remaining text as its message.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name cringe_guess is wrong: the
// slash-command table entry at 0x0056051C names it /rofl)
// __cdecl command handler: social 11 for the first word of the arguments.
void social_rofl_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(11, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a6e88
// Sends a wave social to the named player when the player is standing still.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table entry at
// 0x00560528 names it /wave, as the queue has it)
// __cdecl command handler: social 12 for the first word of the arguments, only while standing (state 100)
// and still (|speed| < 0.1).
void social_wave_guess(EQPlayer* player, char* args)
{
    char target[2112];

    if (g_control_player_guess->stand_state == 100
        && (g_control_player_guess->speed < 0.0f ? -g_control_player_guess->speed : g_control_player_guess->speed) < 0.1f)
    {
        EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(12, target,
            EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)("You must stand up and be still to wave.");
}

// FUNCTION: TRILOGY 0x004a6efc
// Sends a cheer social to the named player when the player is standing still.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table entry at
// 0x00560534 names it /cheer, as the queue has it)
// __cdecl command handler: social 13 for the first word of the arguments, only while standing (state 100)
// and still (|speed| < 0.1).
void social_cheer_guess(EQPlayer* player, char* args)
{
    char target[2112];

    if (g_control_player_guess->stand_state == 100
        && (g_control_player_guess->speed < 0.0f ? -g_control_player_guess->speed : g_control_player_guess->speed) < 0.1f)
    {
        EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(13, target,
            EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)("You must stand up and be still to cheer.");
}

// FUNCTION: TRILOGY 0x004a6f7b
// Sends a rude gesture social to the named player when the player is standing still.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name rude_guess is wrong: the
// slash-command table entry at 0x00560540, 0x0056054C, 0x00560558, 0x00560564, 0x00560570 names it /gesture (also /rude, /flipoff, /bird, /finger))
// __cdecl command handler: social 14 for the first word of the arguments, only while standing (state 100)
// and still (|speed| < 0.1).
void social_gesture_guess(EQPlayer* player, char* args)
{
    char target[2112];

    if (g_control_player_guess->stand_state == 100
        && (g_control_player_guess->speed < 0.0f ? -g_control_player_guess->speed : g_control_player_guess->speed) < 0.1f)
    {
        EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(14, target,
            EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)("You must stand up and be still to rudely gesture.");
}

// FUNCTION: TRILOGY 0x004a6fef
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name curious_guess is wrong: the
// slash-command table entry at 0x0056057C names it /agree)
// __cdecl command handler: social 15 for the first word of the arguments.
void social_agree_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(15, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7024
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name dance_guess is wrong: the
// slash-command table entry at 0x00560588 names it /amaze)
// __cdecl command handler: social 16 for the first word of the arguments.
void social_amaze_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(16, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7059
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name drool_guess is wrong: the
// slash-command table entry at 0x00560594 names it /apologize)
// __cdecl command handler: social 17 for the first word of the arguments.
void social_apologize_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(17, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a708e
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name duck_guess is wrong: the
// slash-command table entry at 0x005605A0 names it /applaud)
// __cdecl command handler: social 18 for the first word of the arguments.
void social_applaud_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(18, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a70c3
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name eye_guess is wrong: the
// slash-command table entry at 0x005605AC names it /plead)
// __cdecl command handler: social 19 for the first word of the arguments.
void social_plead_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(19, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a70f8
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name gasp_guess is wrong: the
// slash-command table entry at 0x005605B8 names it /bite)
// __cdecl command handler: social 20 for the first word of the arguments.
void social_bite_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(20, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a712d
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name giggle_guess is wrong: the
// slash-command table entry at 0x005605C4 names it /bleed)
// __cdecl command handler: social 21 for the first word of the arguments.
void social_bleed_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(21, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7162
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name glare_guess is wrong: the
// slash-command table entry at 0x005605D0 names it /blink)
// __cdecl command handler: social 22 for the first word of the arguments.
void social_blink_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(22, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7197
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name grin_guess is wrong: the
// slash-command table entry at 0x005605DC names it /blush)
// __cdecl command handler: social 23 for the first word of the arguments.
void social_blush_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(23, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a71cc
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name groan_guess is wrong: the
// slash-command table entry at 0x005605E8 names it /boggle)
// __cdecl command handler: social 24 for the first word of the arguments.
void social_boggle_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(24, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7201
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name grovel_guess is wrong: the
// slash-command table entry at 0x005605F4 names it /bonk)
// __cdecl command handler: social 25 for the first word of the arguments.
void social_bonk_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(25, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7236
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name happy_guess is wrong: the
// slash-command table entry at 0x00560600 names it /bored)
// __cdecl command handler: social 26 for the first word of the arguments.
void social_bored_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(26, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a726b
// Sends a be-right-back social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name hungry_guess is wrong: the
// slash-command table entry at 0x0056060C names it /brb)
// __cdecl command handler: social 27 for the first word of the arguments.
void social_brb_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(27, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a72a0
// Sends a burp social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name introduce_guess is wrong: the
// slash-command table entry at 0x00560618 names it /burp)
// __cdecl command handler: social 28 for the first word of the arguments.
void social_burp_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(28, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a72d5
// Sends a farewell social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name jk_guess is wrong: the
// slash-command table entry at 0x00560624 names it /bye)
// __cdecl command handler: social 29 for the first word of the arguments.
void social_bye_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(29, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a730a
// Sends a cackling social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name kneel_guess is wrong: the
// slash-command table entry at 0x00560630 names it /cackle)
// __cdecl command handler: social 30 for the first word of the arguments.
void social_cackle_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(30, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a733f
// Sends a calming social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name lost_guess is wrong: the
// slash-command table entry at 0x0056063C names it /calm)
// __cdecl command handler: social 31 for the first word of the arguments.
void social_calm_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(31, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7374
// Sends a clapping social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name massage_guess is wrong: the
// slash-command table entry at 0x00560648 names it /clap)
// __cdecl command handler: social 32 for the first word of the arguments.
void social_clap_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(32, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a73a9
// Sends a comforting social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name moan_guess is wrong: the
// slash-command table entry at 0x00560654 names it /comfort)
// __cdecl command handler: social 33 for the first word of the arguments.
void social_comfort_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(33, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a73de
// Sends a congratulatory social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name mourn_guess is wrong: the
// slash-command table entry at 0x00560660 names it /congratulate)
// __cdecl command handler: social 34 for the first word of the arguments.
void social_congratulate_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(34, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7413
// Sends a coughing social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name peer_guess is wrong: the
// slash-command table entry at 0x0056066C names it /cough)
// __cdecl command handler: social 35 for the first word of the arguments.
void social_cough_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(35, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7448
// Sends a cringe social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name point_guess is wrong: the
// slash-command table entry at 0x00560678 names it /cringe)
// __cdecl command handler: social 36 for the first word of the arguments.
void social_cringe_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(36, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a747d
// Sends a curious social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name ponder_guess is wrong: the
// slash-command table entry at 0x00560684 names it /curious)
// __cdecl command handler: social 37 for the first word of the arguments.
void social_curious_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(37, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a74b2
// Sends a dance social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name puzzle_guess is wrong: the
// slash-command table entry at 0x00560690 names it /dance)
// __cdecl command handler: social 38 for the first word of the arguments.
void social_dance_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(38, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a74e7
// Sends a drooling social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name raise_guess is wrong: the
// slash-command table entry at 0x0056069C names it /drool)
// __cdecl command handler: social 39 for the first word of the arguments.
void social_drool_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(39, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a751c
// Sends a ducking social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name ready_guess is wrong: the
// slash-command table entry at 0x005606A8 names it /duck)
// __cdecl command handler: social 40 for the first word of the arguments.
void social_duck_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(40, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7551
// Uses the eye social toward the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name roar_guess is wrong: the
// slash-command table entry at 0x005606B4 names it /eye)
// __cdecl command handler: social 41 for the first word of the arguments.
void social_eye_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(41, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7586
// Sends a gasp social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name salute_guess is wrong: the
// slash-command table entry at 0x005606C0 names it /gasp)
// __cdecl command handler: social 42 for the first word of the arguments.
void social_gasp_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(42, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a75bb
// Sends a giggling social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name shiver_guess is wrong: the
// slash-command table entry at 0x005606CC names it /giggle)
// __cdecl command handler: social 43 for the first word of the arguments.
void social_giggle_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(43, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a75f0
// Sends a glaring social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name shrug_guess is wrong: the
// slash-command table entry at 0x005606D8 names it /glare)
// __cdecl command handler: social 44 for the first word of the arguments.
void social_glare_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(44, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7625
// Sends a grinning social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name sigh_guess is wrong: the
// slash-command table entry at 0x005606E4 names it /grin)
// __cdecl command handler: social 45 for the first word of the arguments.
void social_grin_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(45, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a765a
// Sends a groaning social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name smirk_guess is wrong: the
// slash-command table entry at 0x005606F0 names it /groan)
// __cdecl command handler: social 46 for the first word of the arguments.
void social_groan_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(46, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a768f
// Sends a groveling social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name snarl_guess is wrong: the
// slash-command table entry at 0x005606FC names it /grovel)
// __cdecl command handler: social 47 for the first word of the arguments.
void social_grovel_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(47, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a76c4
// Sends a happy social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name snicker_guess is wrong: the
// slash-command table entry at 0x00560708 names it /happy)
// __cdecl command handler: social 48 for the first word of the arguments.
void social_happy_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(48, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a76f9
// Sends a hungry social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name stare_guess is wrong: the
// slash-command table entry at 0x00560714 names it /hungry)
// __cdecl command handler: social 49 for the first word of the arguments.
void social_hungry_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(49, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a772e
// Sends an introduction social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name tap_guess is wrong: the
// slash-command table entry at 0x00560720 names it /introduce)
// __cdecl command handler: social 50 for the first word of the arguments.
void social_introduce_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(50, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7763
// Sends a just-kidding social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name tease_guess is wrong: the
// slash-command table entry at 0x0056072C names it /jk)
// __cdecl command handler: social 51 for the first word of the arguments.
void social_jk_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(51, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7798
// Sends a kneeling social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name thank_guess is wrong: the
// slash-command table entry at 0x00560738 names it /kneel)
// __cdecl command handler: social 52 for the first word of the arguments.
void social_kneel_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(52, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a77cd
// Sends a lost social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name thirsty_guess is wrong: the
// slash-command table entry at 0x00560744 names it /lost)
// __cdecl command handler: social 53 for the first word of the arguments.
void social_lost_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(53, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7802
// Sends a massage social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name veto_guess is wrong: the
// slash-command table entry at 0x00560750 names it /massage)
// __cdecl command handler: social 54 for the first word of the arguments.
void social_massage_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(54, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7837
// Sends a moaning social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name welcome_guess is wrong: the
// slash-command table entry at 0x0056075C names it /moan)
// __cdecl command handler: social 55 for the first word of the arguments.
void social_moan_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(55, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a786c
// Sends a mourning social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name whine_guess is wrong: the
// slash-command table entry at 0x00560768 names it /mourn)
// __cdecl command handler: social 56 for the first word of the arguments.
void social_mourn_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(56, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a78a1
// Sends a peering social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name whistle_guess is wrong: the
// slash-command table entry at 0x00560774 names it /peer)
// __cdecl command handler: social 57 for the first word of the arguments.
void social_peer_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(57, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a78d6
// Sends a pointing social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name yawn_guess is wrong: the
// slash-command table entry at 0x00560780 names it /point)
// __cdecl command handler: social 58 for the first word of the arguments.
void social_point_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(58, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a790b
// Sends a pondering social to the named player, optionally with extra text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name smack_guess is wrong: the
// slash-command table entry at 0x0056078C names it /ponder)
// __cdecl command handler: social 59 for the first word of the arguments.
void social_ponder_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(59, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7940
// Sends the puzzle social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name pat_guess is wrong: the
// slash-command table entry at 0x00560798 names it /puzzle)
// __cdecl command handler: social 60 for the first word of the arguments.
void social_puzzle_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(60, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7975
// Sends the raise social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name fidget_guess is wrong: the
// slash-command table entry at 0x005607A4 names it /raise)
// __cdecl command handler: social 61 for the first word of the arguments.
void social_raise_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(61, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a79aa
// Sends the ready social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name nudge_guess is wrong: the
// slash-command table entry at 0x005607B0 names it /ready)
// __cdecl command handler: social 62 for the first word of the arguments.
void social_ready_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(62, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a79df
// Sends the roar social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name purr_guess is wrong: the
// slash-command table entry at 0x005607BC names it /roar)
// __cdecl command handler: social 63 for the first word of the arguments.
void social_roar_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(63, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7a14
// Sends the salute social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name kiss_guess is wrong: the
// slash-command table entry at 0x005607C8 names it /salute)
// __cdecl command handler: social 64 for the first word of the arguments.
void social_salute_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(64, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7a49
// Sends the shiver social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name flex_guess is wrong: the
// slash-command table entry at 0x005607D4 names it /shiver)
// __cdecl command handler: social 65 for the first word of the arguments.
void social_shiver_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(65, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7a7e
// Sends the shrug social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7a7e is wrong: the
// slash-command table entry at 0x005607E0 names it /shrug)
// __cdecl command handler: social 66 for the first word of the arguments.
void social_shrug_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(66, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7ab3
// Sends the sigh social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7ab3 is wrong: the
// slash-command table entry at 0x005607EC names it /sigh)
// __cdecl command handler: social 67 for the first word of the arguments.
void social_sigh_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(67, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7ae8
// Sends the smirk social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name apologize_guess is wrong: the
// slash-command table entry at 0x005607F8 names it /smirk)
// __cdecl command handler: social 68 for the first word of the arguments.
void social_smirk_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(68, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7b1d
// Sends the snarl social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7b1d is wrong: the
// slash-command table entry at 0x00560804 names it /snarl)
// __cdecl command handler: social 69 for the first word of the arguments.
void social_snarl_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(69, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7b52
// Sends the snicker social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name applaud_guess is wrong: the
// slash-command table entry at 0x00560810 names it /snicker)
// __cdecl command handler: social 70 for the first word of the arguments.
void social_snicker_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(70, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7b87
// Sends the stare social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name bite_guess is wrong: the
// slash-command table entry at 0x0056081C names it /stare)
// __cdecl command handler: social 71 for the first word of the arguments.
void social_stare_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(71, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7bbc
// Sends the tap social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name blush_guess is wrong: the
// slash-command table entry at 0x00560828 names it /tap)
// __cdecl command handler: social 72 for the first word of the arguments.
void social_tap_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(72, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7bf1
// Sends the tease social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name boggle_guess is wrong: the
// slash-command table entry at 0x00560834 names it /tease)
// __cdecl command handler: social 73 for the first word of the arguments.
void social_tease_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(73, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7c26
// Sends the thank social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7c26 is wrong: the
// slash-command table entry at 0x00560840 names it /thank)
// __cdecl command handler: social 74 for the first word of the arguments.
void social_thank_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(74, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7c5b
// Sends the thirsty social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7c5b is wrong: the
// slash-command table entry at 0x0056084C names it /thirsty)
// __cdecl command handler: social 75 for the first word of the arguments.
void social_thirsty_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(75, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7c90
// Sends the veto social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7c90 is wrong: the
// slash-command table entry at 0x00560858 names it /veto)
// __cdecl command handler: social 76 for the first word of the arguments.
void social_veto_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(76, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7cc5
// Sends the welcome social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7cc5 is wrong: the
// slash-command table entry at 0x00560864 names it /welcome)
// __cdecl command handler: social 77 for the first word of the arguments.
void social_welcome_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(77, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7cfa
// Sends the whine social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7cfa is wrong: the
// slash-command table entry at 0x00560870 names it /whine)
// __cdecl command handler: social 78 for the first word of the arguments.
void social_whine_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(78, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7d2f
// Sends the whistle social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7d2f is wrong: the
// slash-command table entry at 0x0056087C names it /whistle)
// __cdecl command handler: social 79 for the first word of the arguments.
void social_whistle_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(79, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7d64
// Sends the yawn social to the named player, with any remaining text attached.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; queue name FUN_004a7d64 is wrong: the
// slash-command table entry at 0x00560888 names it /yawn)
// __cdecl command handler: social 80 for the first word of the arguments.
void social_yawn_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(3, void, 0x004CC92F, (CEverQuest*, int, char*, char*), pinstCEverQuest_guess)(80, target,
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, target));
}

// FUNCTION: TRILOGY 0x004a7d99
// Switches to a recognized chat channel, or shows channel help when the name is unknown.
// __cdecl, callee pops 0: the channel command. Compares the whole argument (case-insensitive) with the channel
// names. On a match the argument's low byte becomes the channel number (social 1, gsay 2, shout 3, auction 4,
// occs 5, tell 7) and SendChannelToggle_guess sends it. Otherwise the usage line is printed and the channel list requested.
void SelectChatChannel_guess(int unused, char* args)
{
    bool matched;
    bool on;
    char* text;

    text = args;
    matched = false;
    on = true;
    if (_strcmpi(EQ_ARRAY(char, 0x005639B0), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x005639A8), text) == 0)
    {
        *(u8*)&args = 1;
        matched = on;
    }
    if (_strcmpi(EQ_ARRAY(char, 0x005639A0), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x00563998), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x00563994), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x0056398C), text) == 0)
    {
        *(u8*)&args = 7;
        matched = on;
    }
    if (_strcmpi(EQ_ARRAY(char, 0x00563984), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x0056397C), text) == 0)
    {
        *(u8*)&args = 2;
        matched = on;
    }
    if (_strcmpi(EQ_ARRAY(char, 0x00563974), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x0056396C), text) == 0)
    {
        *(u8*)&args = 3;
        matched = on;
    }
    if (_strcmpi(EQ_ARRAY(char, 0x00563964), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x00563958), text) == 0)
    {
        *(u8*)&args = 4;
        matched = on;
    }
    if (_strcmpi(EQ_ARRAY(char, 0x00563954), text) == 0 ||
        _strcmpi(EQ_ARRAY(char, 0x0056394C), text) == 0)
    {
        *(u8*)&args = 5;
        matched = on;
    }
    if (matched)
    {
        EQ_METHOD(1, void, 0x004CC908, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))((int)args);
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                                                                  EQ_ARRAY(char, 0x00563910), 10);
    EQ_METHOD(0, void, 0x004D0BB6, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    return;
}

// FUNCTION: TRILOGY 0x004a7eee
// Hails the selected target when visible, including their name in the chat text.
void hail_guess(void)
{
    u16 targetRace;
    int canSeeTarget;
    char* targetName;

    strcpy((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8, EQ_ARRAY(char, 0x0055A9D8));
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0)
    {
        targetRace = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->race;
        float targetZ = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->z;
        if (targetRace == 0x0B || targetRace == 0x0C || targetRace == 8)
        {
            float adjustedZ = targetZ;
            adjustedZ += EQ_ARRAY(float, 0x005344A4)[0];
            targetZ = adjustedZ;
        }

        canSeeTarget = EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))
            (EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
             EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->y,
             EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->x,
             targetZ);
        if (canSeeTarget != 0)
        {
            targetName = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                 (char*)EQ_GLOBAL(EQPlayer*, 0x006E4DAC) + 1);
            sprintf((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8,
                 EQ_ARRAY(char, 0x005639B8), targetName);
        }
    }
    EQ_METHOD(0, void, 0x004D0A8E, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    pinstCEverQuest_guess->chatInput[0] = 0;
    return;
}

// FUNCTION: TRILOGY 0x004a7f9b
// Routes text to a visible selected target when possible; otherwise submits it through normal chat.
void say_guess(int unused, char* text)
{
    float targetZ;
    u16 targetRace;
    short spellAffect;
    int canSeeTarget;
    EQPlayer* target;
    void* everquest;
    char message[2144];

    strcpy((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8, text);
    target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
    if (target != 0 && target->type == 1)
    {
        targetRace = target->race;
        targetZ = target->z;
        if (targetRace == 0x0B || targetRace == 0x0C || targetRace == 8)
        {
            float originalTargetZ = targetZ;
            targetZ = originalTargetZ + EQ_ARRAY(float, 0x005344A4)[0];
        }

        canSeeTarget = EQ_CDECL(int, 0x004A3491, (EQPlayer*, float, float, float))
            (EQ_GLOBAL(EQPlayer*, 0x006E4D8C), target->y, target->x, targetZ);
        everquest = EQ_GLOBAL(void*, 0x006EFE2C);
        if (canSeeTarget != 0)
        {
            EQ_METHOD(3, void, 0x004D0D77, (void*, int, char*, char*), everquest)(8,
                 (char*)EQ_GLOBAL(EQPlayer*, 0x006E4DAC) + 1,
                 (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8);
            spellAffect = EQ_METHOD(1, short, 0x004238A2, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))(0x4B);
            if (spellAffect != 0)
                return;
            sprintf(message, EQ_ARRAY(char, 0x005639C4),
                 (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(message, 0);
            return;
        }
    }
    else
    {
        everquest = EQ_GLOBAL(void*, 0x006EFE2C);
    }
    EQ_METHOD(0, void, 0x004D0A8E, (void*), everquest)();
    return;
}

// FUNCTION: TRILOGY 0x004a808c
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /gsay)
// __cdecl command handler: copies the text into CEverQuest's chat buffer (+0xBA8) and sends it to the group.
void cmd_gsay_guess(EQPlayer* player, char* args)
{
    strcpy((char*)pinstCEverQuest_guess + 0xBA8, args);
    EQ_METHOD(0, void, 0x004D0B20, (CEverQuest *), pinstCEverQuest_guess)();
}

// FUNCTION: TRILOGY 0x004a80ce
// Calls for help from the player's current zone.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /yell)
// __cdecl command handler: prints "..." (colour 13) and sends OP_YellForHelp with the
// player's spawn id.
void cmd_yell_guess(EQPlayer* player, char* args)
{
    u32 spawn_id;

    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005639D4), 13);
    spawn_id = player->spawn_id;
    send_message_guess(g_world_connection_guess, 0x21DA, &spawn_id, 4, 1);
}

// FUNCTION: TRILOGY 0x004a810c
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /auction)
// __cdecl command handler: copies the text into CEverQuest's chat buffer (+0xBA8) and sends it as an auction.
void cmd_auction_guess(EQPlayer* player, char* args)
{
    strcpy((char*)pinstCEverQuest_guess + 0xBA8, args);
    EQ_METHOD(0, void, 0x004D0C61, (CEverQuest *), pinstCEverQuest_guess)();
}

// FUNCTION: TRILOGY 0x004a812d
// Sends the entered text to the out-of-character chat channel.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /ooc)
// __cdecl command handler: copies the text into CEverQuest's chat buffer (+0xBA8) and sends it out of
// character.
void cmd_ooc_guess(EQPlayer* player, char* args)
{
    strcpy((char*)pinstCEverQuest_guess + 0xBA8, args);
    EQ_METHOD(0, void, 0x004D0CEC, (CEverQuest *), pinstCEverQuest_guess)();
}

// FUNCTION: TRILOGY 0x004a814e
// Sends a private tell to the named player with the remaining text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /tell
// and /msg)
// __cdecl command handler: the first word is the recipient, the rest the message (CEverQuest::send_tell_guess).
void cmd_tell_guess(EQPlayer* player, char* args)
{
    char target[2112];

    EQ_METHOD(2, void, 0x004CF180, (CEverQuest *, char *, char *), pinstCEverQuest_guess)(target, CEverQuest_GrabFirstWord2_guess(pinstCEverQuest_guess, 0, args, target));
}

// FUNCTION: TRILOGY 0x004a8181
// Adds a player to the recent tell recipients list for quick replies.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; TAKP name, low)
// __cdecl. Puts `name` at the front of the 10-entry recent-tells list (0x006E1E0C, 32 bytes each) and resets
// the cycle index. Every entry moves down one (the last falls off); when the name was already in the list
// that older copy is removed instead and the entry that fell off is put back.
void AddNameToTellList(const char* name)
{
    char saved[32];
    int i;

    if (g_tell_list_guess[9][0] != '\0')
        strcpy(saved, g_tell_list_guess[9]);
    else
        saved[0] = '\0';
    for (i = 9; i >= 1; i--)
        strcpy(g_tell_list_guess[i], g_tell_list_guess[i - 1]);
    strcpy(g_tell_list_guess[0], name);
    g_tell_index_guess = 0;
    for (i = 1; i < 10; i++)
    {
        if (_strcmpi(g_tell_list_guess[0], g_tell_list_guess[i]) == 0)
        {
            for (; i < 9; i++)
                strcpy(g_tell_list_guess[i], g_tell_list_guess[i + 1]);
            if (saved[0] != '\0')
                strcpy(g_tell_list_guess[9], saved);
            return;
        }
    }
}

// FUNCTION: TRILOGY 0x004a8235
// Selects a recent tell sender for the reply command.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Moves the recent-tell index forward or backward, skipping empty entries only on the
// backward wrap. The x86 scaled slot loads wrap for negative indexes, so form their addresses in u32.
// @early-stop: the wrap exit needs a sign check before the shared reset (structural 0.9351, ndiff 11).
void CycleNamesInTellList(int direction)
{
    if (EQ_GLOBAL(u8, 0x006E1E0C) == 0)
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005639E8));
        return;
    }
    if (direction == 1)
    {
        if (g_tell_index_guess > 0)
        {
            g_tell_index_guess--;
            return;
        }
        g_tell_index_guess = 9;
        if (EQ_GLOBAL(u8, 0x006E1F2C) != 0)
            return;
        do
            g_tell_index_guess--;
        while (g_tell_index_guess >= 0 && g_tell_index_guess <= 9 &&
               *(u8*)(0x006E1E0C + ((u32)g_tell_index_guess << 5)) == 0);
        if (g_tell_index_guess >= 0)
            return;
    }
    else
    {
        if (g_tell_index_guess < 9)
        {
            if (*(u8*)(0x006E1E2C + ((u32)g_tell_index_guess << 5)) != 0)
            {
                g_tell_index_guess++;
                return;
            }
        }
    }
    g_tell_index_guess = 0;
}

// FUNCTION: TRILOGY 0x004a82b8
void ReplyToTell_guess(int unused, char* tellText)
{
    if (EQ_GLOBAL(u8, 0x006E1E0C) == '\0')
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005639E8);
        return;
    }
    EQ_METHOD(2, void, 0x004CF180, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))((char*)0x006E1E0C, tellText);
    return;
}

// FUNCTION: TRILOGY 0x004a82e7
// Directs the pet to attack, targets it, or sends it a tell.
// __cdecl. The /pet command. The pet is the first spawn whose pet_owner_id is the local player's spawn id; without one
// "..." is printed. "target" targets the pet and prints "Targeting your pet.". The attack
// words (attack, kill, kick, maim, slay, murder, execute, sack, destroy, whack) send "attack <target name>" when the
// target is not the local player (the text itself otherwise); any other text is sent to the pet as a tell.
void pet_command_guess(int unused, char* command)
{
    char message[512];
    EQPlayer* pet;
    EQPlayer* target;
    u32 localSpawnId;
    void* wnd;

    pet = EQ_GLOBAL(EQPlayer*, 0x006E4D58);
    if (pet != 0)
    {
        localSpawnId = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
        while (pet != 0)
        {
            if (pet->pet_owner_id == localSpawnId)
                break;
            pet = pet->next;
        }
    }
    if (pet == 0)
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00563A90));
        return;
    }
    strcpy(message, command);
    if (!(_strcmpi(command, EQ_ARRAY(char, 0x00563A88)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x0053E020)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A80)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A78)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A70)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A68)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A60)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A58)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A50)) == 0 ||
          _strcmpi(command, EQ_ARRAY(char, 0x00563A48)) == 0))
    {
        if (_strcmpi(command, EQ_ARRAY(char, 0x00563A40)) == 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00563A2C));
            wnd = EQ_GLOBAL(void*, 0x006E4E44);
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = pet;
            if (wnd != 0)
                *(u32*)((char*)wnd + 0xEB8) = 1;
            return;
        }
    }
    else
    {
        target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
        if (target != 0 && target != EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
            sprintf(message, EQ_ARRAY(char, 0x00563A20), target->name);
    }
    EQ_METHOD(2, void, 0x004CF180, (CEverQuest *, const char *, const char *), pinstCEverQuest_guess)(pet->name, message);
}

// FUNCTION: TRILOGY 0x004a847d
// Requests that a named character or the current target be summoned to the local player.
// __cdecl. The summon command. Sends opcode 0x20C5 with a 108-byte request (the name, the local player's name, the
// zone, and the local player's y, x and z truncated to ints; the last 4 bytes are never written), then prints
// "Summoning <name>." in colour 13. The name "target" (exact match with the string at 0x0058FBA4) summons the target
// instead, and nothing is sent without a target.
void summon_guess(int unused, char* name)
{
    char message[128];
    struct SummonPacket_guess
    {
        char name[30];          /* 0x00 */
        char gm_name[31];       /* 0x1E the local player, who summons */
        char zone[31];          /* 0x3D */
        int y;                  /* 0x5C */
        int x;                  /* 0x60 */
        int z;                  /* 0x64 */
        u32 unknown0x68;        /* 0x68 never written, so the stack bytes are sent */
    } packet;
    EQPlayer* player;

    if (strcmp(name, EQ_ARRAY(char, 0x0058FBA4)) != 0)
    {
        strcpy(packet.name, name);
        strcpy(packet.gm_name, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
        strcpy(packet.zone, EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->zone);
        player = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
        packet.y = (int)player->y;
        packet.x = (int)player->x;
        packet.z = (int)player->z;
        send_message_guess(g_world_connection_guess, 0x20C5, &packet, 0x6C, 1);
        sprintf(message, EQ_ARRAY(char, 0x00563AB4), name);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xD);
    }
    else
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
            return;
        strcpy(packet.name, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
        strcpy(packet.gm_name, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
        strcpy(packet.zone, EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->zone);
        player = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
        packet.y = (int)player->y;
        packet.x = (int)player->x;
        packet.z = (int)player->z;
        send_message_guess(g_world_connection_guess, 0x20C5, &packet, 0x6C, 1);
        sprintf(message, EQ_ARRAY(char, 0x00563AB4), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xD);
    }
}

// FUNCTION: TRILOGY 0x004a85c2
// Prompts on a blank petition and starts submission of entered petition text.
void SubmitPetition_guess(int unused, char* petitionText)
{
    if (petitionText != 0 && *petitionText != '\0')
    {
        strcpy((char*)EQ_GLOBAL(CEverQuest*, 0x006EFE2C) + 0xBA8, petitionText);
        EQ_METHOD(0, void, 0x004CF226, (CEverQuest*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))();
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
        (EQ_ARRAY(char, 0x00563AC4), 0x0F);
    return;
}

// FUNCTION: TRILOGY 0x004a8610
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (placement inferred from mapped EQMisc neighbours)
// __cdecl. Appends each positive denomination to the caller's text, separated by the mutable client string.
void format_coin_components_guess(int platinum, int gold, int silver, int copper, char* output)
{
    char amount[32];
    const char* separator;

    amount[0] = '\0';
    if (platinum > 0)
    {
        sprintf(amount, "%ld platinum", platinum);
        strcat(output, amount);
    }
    separator = (const char*)0x0055e12c;
    if (gold > 0)
    {
        if (platinum > 0)
            strcat(output, separator);
        sprintf(amount, "%ld gold", gold);
        strcat(output, amount);
    }
    if (silver > 0)
    {
        if (platinum > 0 || gold > 0)
            strcat(output, separator);
        sprintf(amount, "%ld silver", silver);
        strcat(output, amount);
    }
    if (copper > 0)
    {
        if (platinum > 0 || gold > 0 || silver > 0)
            strcat(output, separator);
        sprintf(amount, "%ld copper", copper);
        strcat(output, amount);
    }
}

// FUNCTION: TRILOGY 0x004a86f1
// For a GM character, submits the entered text as a GM say.
int SendGMSay_guess(EQPlayer* player, char* text)
{
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm != 0)
    {
        strcpy((char*)EQ_GLOBAL(CEverQuest*, 0x006EFE2C) + 0x0ba8, text);
        return EQ_METHOD(0, int, 0x004CF28C, (CEverQuest*),
            EQ_GLOBAL(CEverQuest*, 0x006EFE2C))();
    }
    return (int)EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
}

// FUNCTION: TRILOGY 0x004a8721
// Asks the GM search service to locate a player by name or current target.
// __cdecl. GM player search. A name that matches a spawn becomes the target; an empty name uses the current
// target instead. Sends the searched name and the GM's own name as one 108-byte packet (opcode 0x2069), then
// says "Searching for <name>." in chat colour 13.
void do_searchplayer_guess(int unused, char* name)
{
    char message[128];
    char packet[108];       // searched name at 0x00 (30 bytes), the GM's own name at 0x1E (78 bytes): sent as one packet

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm != 0)
    {
        if (strcmp(name, EQ_ARRAY(char, 0x0058FBA4)) != 0)
        {
            if (EQ_METHOD(1, EQPlayer*, 0x004B1435, (EQPlayer*, const char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(name) != 0)
                EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = EQ_METHOD(1, EQPlayer*, 0x004B1435, (EQPlayer*, const char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(name);
            strcpy(packet, name);
            strcpy(packet + 30, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
            EQ_CDECL(void, 0x004E1C45, (void*, u32, void*, u32, int))(
                &EQ_GLOBAL(void*, 0x006B3194), 0x2069, packet, 0x6c, 1);
            sprintf(message, EQ_ARRAY(char, 0x00563B2C), name);
        }
        else
        {
            if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
                return;
            strcpy(packet, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
            strcpy(packet + 30, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
            EQ_CDECL(void, 0x004E1C45, (void*, u32, void*, u32, int))(
                &EQ_GLOBAL(void*, 0x006B3194), 0x2069, packet, 0x6c, 1);
            sprintf(message, EQ_ARRAY(char, 0x00563B2C), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(message, 13);
    }
}

// FUNCTION: TRILOGY 0x004a8819
// Asks the GM search service to find a character's corpse across zones.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (from Mac debug map)
void do_searchcorpse_guess(int unused, char* characterName)
{
    char message[0x60];

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm != 0)
    {
        if (characterName != 0 && *characterName != '\0')
        {
            strcpy(message + 0x20, characterName);
            message[0x40] = 0;
            EQ_CDECL(void, 0x004E1C45, (void*, u32, void*, u32, int))(
                &EQ_GLOBAL(void*, 0x006B3194), 0x21a7, message, 0x60, 1);
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563B68), 0x0f);
        }
        else
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563B40), 0x0f);
        }
    }
}

// FUNCTION: TRILOGY 0x004a887e
// ORIGINAL FILE: unknown (EQMisc placement inferred from mapped neighbours)
// __cdecl. Sets the fast-drop option from an on/off/never word, reports valid choices and saves options.
void set_fast_drop_mode_guess(int unused, const char* choice)
{
    int compare;

    compare = _strcmpi(choice, (const char*)0x0055b250);
    if (compare == 0)
    {
        EQ_GLOBAL(int, 0x006B6EFC) = 1;
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00563CA4));
    }
    else
    {
        compare = _strcmpi(choice, (const char*)0x0055b254);
        if (compare == 0)
        {
            EQ_GLOBAL(int, 0x006B6EFC) = 0;
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00563C44));
        }
        else
        {
            compare = _strcmpi(choice, (const char*)0x0055b248);
            if (compare == 0)
            {
                EQ_GLOBAL(int, 0x006B6EFC) = -1;
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00563C1C));
            }
            else
            {
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00563B94));
                return;
            }
        }
    }
    EQ_METHOD(0, void, 0x004CF3E1, (CEverQuest*), pinstCEverQuest_guess)();
}

// FUNCTION: TRILOGY 0x004a890a
// Shows the available slash commands in the requested help category.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
void do_help(int unused, const char* command_class)
{
    char message[256];
    int compare;
    int class_id;
    char** command;

    compare = _strcmpi(command_class, EQ_ARRAY(char, 0x00563E40));
    if (compare == 0)
    {
        class_id = 1;
    }
    else
    {
        compare = _strcmpi(command_class, EQ_ARRAY(char, 0x00563E38));
        if (compare == 0)
        {
            class_id = 2;
        }
        else
        {
            compare = _strcmpi(command_class, EQ_ARRAY(char, 0x00563E30));
            if (compare == 0)
            {
                class_id = 3;
            }
            else
            {
                compare = _strcmpi(command_class, EQ_ARRAY(char, 0x00563E28));
                if (compare == 0)
                {
                    class_id = 4;
                }
                else
                {
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563D74), 0x111);
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563D48), 0x111);
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563D18), 0x111);
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563CE8), 0x111);
                    return;
                }
            }
        }
    }

    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563E14), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563DC0), 0x111);
    if (**EQ_ARRAY(char*, 0x00560210) != '\0')
    {
        command = EQ_ARRAY(char*, 0x00560210);
        do
        {
            if (*(short*)((char*)command + 0x0a) == class_id)
            {
                sprintf(message, EQ_ARRAY(char, 0x00563DB8), *command);
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(message, 0x111);
            }
            command += 3;
        }
        while (**command != '\0');
    }
}

// FUNCTION: TRILOGY 0x004a8a32
// Publishes the player's Looking For Group status and updates the local name display.
void SetLookingForGroup_guess(int showFeedback, const char* lfgCommand)
{
    struct LfgMessage_guess
    {
        char local_28[32];
        int local_8;
    } lfg_message;
    int compare;

    compare = _strcmpi(lfgCommand, EQ_ARRAY(char, 0x0055B250));
    if (compare == 0)
    {
        if (EQ_GLOBAL(char, 0x005FE968) != '\0')
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563EAC));
            return;
        }
        lfg_message.local_8 = 1;
        if (showFeedback != 0 && EQ_GLOBAL(int, 0x006E2064) != 1)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563E88));
        }
    }
    else
    {
        compare = _strcmpi(lfgCommand, EQ_ARRAY(char, 0x0055B254));
        if (compare == 0)
        {
            lfg_message.local_8 = 0;
            if (showFeedback != 0 && EQ_GLOBAL(int, 0x006E2064) != 0)
            {
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563E60));
            }
        }
        else
        {
            if (showFeedback != 0)
            {
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563E48));
            }
            return;
        }
    }
    if (EQ_GLOBAL(int, 0x006E2064) == lfg_message.local_8 && showFeedback != 0)
        return;
    if (EQ_GLOBAL(char*, 0x006E4DA8) == 0)
        return;
    strcpy(lfg_message.local_28, EQ_GLOBAL(char*, 0x006E4DA8));
    send_message_guess
        (g_world_connection_guess, 0x21f0, lfg_message.local_28, 0x24, 1);
    EQ_GLOBAL(int, 0x006E2064) = lfg_message.local_8;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C) == 0)
        return;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info == 0)
        return;
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x300 = lfg_message.local_8;
    EQ_METHOD(1, void, 0x00411B7B, (CDisplay*, EQPlayer*), g_cdisplay_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
}

// FUNCTION: TRILOGY 0x004a8b39
// Sets the player's server-side approval status on or off.
void SetApprovalMode_guess(int showFeedback, const char* approvalCommand)
{
    struct ApprovalMessage_guess
    {
        char local_28[32];
        int local_8;
    } approval_message;
    int compare;

    compare = _strcmpi(approvalCommand, EQ_ARRAY(char, 0x0055B250));
    if (compare == 0)
    {
        approval_message.local_8 = 0;
        if (showFeedback != 0)
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563EF4));
    }
    else
    {
        compare = _strcmpi(approvalCommand, EQ_ARRAY(char, 0x0055B254));
        if (compare == 0)
        {
            approval_message.local_8 = 1;
            if (showFeedback != 0)
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563EE4));
        }
        else
        {
            if (showFeedback == 0)
                return;
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563EC8));
            return;
        }
    }

    if (((EQ_GLOBAL(int, 0x006E2068) != approval_message.local_8) || (showFeedback == 0)) &&
        (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != 0))
    {
        strcpy(approval_message.local_28, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        send_message_guess
            ((void**)0x006b3194, 0x21ef, approval_message.local_28, 0x24, 1);
        EQ_GLOBAL(int, 0x006E2068) = approval_message.local_8;
        return;
    }
    return;
}

// FUNCTION: TRILOGY 0x004a8bf0
// Sets which player characters are included in tracking: all, none, or group only.
void trackplayers_guess(int unused, const char* mode)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(const char*, const char*, const char*, const char*);
    int compare;

    compare = _strcmpi(mode, EQ_ARRAY(char, 0x0055B250));
    if (compare == 0)
    {
        EQ_GLOBAL(int, 0x006B6758) = 1;
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563F8C));
        EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
            EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00563F74), EQ_ARRAY(char, 0x00563F84), EQ_ARRAY(char, 0x0053DB50));
    }
    else
    {
        compare = _strcmpi(mode, EQ_ARRAY(char, 0x0055B254));
        if (compare == 0)
        {
            EQ_GLOBAL(int, 0x006B6758) = 0;
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563F5C));
            EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00563F74), EQ_ARRAY(char, 0x0053DAFC), EQ_ARRAY(char, 0x0053DB50));
        }
        else
        {
            compare = _strcmpi(mode, EQ_ARRAY(char, 0x00563F54));
            if (compare == 0)
            {
                EQ_GLOBAL(int, 0x006B6758) = 2;
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563F34));
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00563F74), EQ_ARRAY(char, 0x00563F2C), EQ_ARRAY(char, 0x0053DB50));
            }
            else
            {
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563F04));
            }
        }
    }

    EQ_METHOD(0, void, 0x0046AA06, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
}

// FUNCTION: TRILOGY 0x004a8cc3
// Chooses the ordering used for ranger tracking results: normal, distance, consider, or reverse.
void SetTrackSortType_guess(int command_context, char* sort_text)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(const char*, const char*, const char*, const char*);
    CharacterProfile* profile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
    char sort_code;

    if (profile != NULL)
    {
        if (profile->class_ == 4)
        {
            sort_code = *sort_text;
            if (sort_code == 'N' || sort_code == 'n')
            {
                EQ_GLOBAL(int, 0x006B675C) = 0;
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
                    (EQ_ARRAY(char, 0x00563FC0));
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056406C),
                    EQ_ARRAY(char, 0x00558BAC), EQ_ARRAY(char, 0x0053DB50));
                EQ_METHOD(0, void, 0x0046A760, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
                return;
            }
            if (sort_code == 'D' || sort_code == 'd')
            {
                EQ_GLOBAL(int, 0x006B675C) = 1;
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
                    (EQ_ARRAY(char, 0x00563FEC));
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056406C),
                    EQ_ARRAY(char, 0x00563FE0), EQ_ARRAY(char, 0x0053DB50));
                EQ_METHOD(0, void, 0x0046A760, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
                return;
            }
            if (sort_code == 'C' || sort_code == 'c')
            {
                EQ_GLOBAL(int, 0x006B675C) = 2;
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
                    (EQ_ARRAY(char, 0x00564010));
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056406C),
                    EQ_ARRAY(char, 0x00559C80), EQ_ARRAY(char, 0x0053DB50));
                EQ_METHOD(0, void, 0x0046A760, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
                return;
            }
            if (sort_code == 'R' || sort_code == 'r')
            {
                sort_code = sort_text[1];
                if (sort_code == 'D' || sort_code == 'd')
                {
                    EQ_GLOBAL(int, 0x006B675C) = 3;
                    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
                        (EQ_ARRAY(char, 0x00564040));
                    EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                        EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056406C),
                        EQ_ARRAY(char, 0x00564034), EQ_ARRAY(char, 0x0053DB50));
                    EQ_METHOD(0, void, 0x0046A760, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
                    return;
                }
                if (sort_code == 'C' || sort_code == 'c')
                {
                    EQ_GLOBAL(int, 0x006B675C) = 4;
                    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
                        (EQ_ARRAY(char, 0x00564088));
                    EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                        EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056406C),
                        EQ_ARRAY(char, 0x0056407C), EQ_ARRAY(char, 0x0053DB50));
                    EQ_METHOD(0, void, 0x0046A760, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
                    return;
                }
            }
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
                (EQ_ARRAY(char, 0x005640B4));
            return;
        }
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
            (EQ_ARRAY(char, 0x00563FA4));
    }
}

#define TRACKFILTER_GREEN 0x01
#define TRACKFILTER_BLUE 0x02
#define TRACKFILTER_WHITE 0x04
#define TRACKFILTER_YELLOW 0x08
#define TRACKFILTER_RED 0x10

// FUNCTION: TRILOGY 0x004a8e7c
// Toggles whether tracked NPCs of a selected color are shown to a ranger.
void ToggleTrackColorFilter_guess(int unused, char* filter_text)
{
    CharacterProfile* profile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
    int& filter_flags = EQ_GLOBAL(int, 0x006B6760);
    char filter_code;
    const char* message;

    if (profile != NULL)
    {
        if (profile->class_ == 4)
        {
            filter_code = *filter_text;
            if (filter_code == 'G' || filter_code == 'g')
            {
                if ((filter_flags & TRACKFILTER_GREEN) == 0)
                {
                    filter_flags = filter_flags | TRACKFILTER_GREEN;
                    message = EQ_ARRAY(char, 0x005640F8);
                }
                else
                {
                    filter_flags = filter_flags & ~TRACKFILTER_GREEN;
                    message = EQ_ARRAY(char, 0x00564124);
                }
            }
            else if (filter_code == 'B' || filter_code == 'b')
            {
                if ((filter_flags & TRACKFILTER_BLUE) == 0)
                {
                    filter_flags = filter_flags | TRACKFILTER_BLUE;
                    message = EQ_ARRAY(char, 0x0056414C);
                }
                else
                {
                    filter_flags = filter_flags & ~TRACKFILTER_BLUE;
                    message = EQ_ARRAY(char, 0x00564178);
                }
            }
            else if (filter_code == 'W' || filter_code == 'w')
            {
                if ((filter_flags & TRACKFILTER_WHITE) == 0)
                {
                    filter_flags = filter_flags | TRACKFILTER_WHITE;
                    message = EQ_ARRAY(char, 0x005641A0);
                }
                else
                {
                    filter_flags = filter_flags & ~TRACKFILTER_WHITE;
                    message = EQ_ARRAY(char, 0x005641CC);
                }
            }
            else if (filter_code == 'Y' || filter_code == 'y')
            {
                if ((filter_flags & TRACKFILTER_YELLOW) == 0)
                {
                    filter_flags = filter_flags | TRACKFILTER_YELLOW;
                    message = EQ_ARRAY(char, 0x005641F4);
                }
                else
                {
                    filter_flags = filter_flags & ~TRACKFILTER_YELLOW;
                    message = EQ_ARRAY(char, 0x00564220);
                }
            }
            else if (filter_code == 'R' || filter_code == 'r')
            {
                if ((filter_flags & TRACKFILTER_RED) == 0)
                {
                    filter_flags = filter_flags | TRACKFILTER_RED;
                    message = EQ_ARRAY(char, 0x00564248);
                }
                else
                {
                    filter_flags = filter_flags & ~TRACKFILTER_RED;
                    message = EQ_ARRAY(char, 0x00564274);
                }
            }
            else
            {
                message = EQ_ARRAY(char, 0x0056429C);
            }

            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(message);
            EQ_METHOD(0, void, 0x0046AA06, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
        }
        else
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00563FA4));
        }
    }
}

#undef TRACKFILTER_GREEN
#undef TRACKFILTER_BLUE
#undef TRACKFILTER_WHITE
#undef TRACKFILTER_YELLOW
#undef TRACKFILTER_RED

// FUNCTION: TRILOGY 0x004a8fbf
// __cdecl. Accepts signed decimal input, updates MousePointerSpeedMod, and reports the current multiplier.
void mousespeed_guess(int unused, const char* speed_text)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(const char*, const char*, const char*, const char*);
    char message[256];

    if (*speed_text != '\0' && is_number((char*)speed_text) != 0)
    {
        EQ_GLOBAL(int, 0x006B6420) = atoi(speed_text);
        EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
            EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x005643C8), speed_text, EQ_ARRAY(char, 0x0053DB50));
    }
    else
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564360));
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005642F8));
    }
    sprintf(message, EQ_ARRAY(char, 0x005642D0), EQ_GLOBAL(int, 0x006B6420));
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(message);
}

// FUNCTION: TRILOGY 0x004a9050
// ORIGINAL FILE: unknown (EQMisc placement inferred from mapped neighbours)
// __cdecl. Empty input prints usage; every nonempty input writes spell-effect state, with only "off" disabling it.
void cmd_show_spell_effects_guess(int unused, const char* choice)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(const char*, const char*, const char*, const char*);
    char message[256];
    int compare;

    if (*choice != '\0')
    {
        compare = _strcmpi(choice, (const char*)0x0055b254);
        if (compare == 0)
        {
            EQ_GLOBAL(int, 0x006E20E8) = 0;
            EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x005644D8), EQ_ARRAY(char, 0x0053D860), EQ_ARRAY(char, 0x0053DB50));
        }
        else
        {
            EQ_GLOBAL(int, 0x006E20E8) = 1;
            EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x005644D8), EQ_ARRAY(char, 0x005644D4), EQ_ARRAY(char, 0x0053DB50));
        }
    }
    else
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x0056449C));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564438));
    }
    if (EQ_GLOBAL(int, 0x006E20E8) != 0)
        strcpy(message, EQ_ARRAY(char, 0x0056440C));
    else
        strcpy(message, EQ_ARRAY(char, 0x005643E0));
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(message);
}

// FUNCTION: TRILOGY 0x004a9101
// Dispatches the active action-state callback; its game action is unclear.
void InvokeActionStateHandler_guess(void)
{
    ActionStateObject_guess* object;

    object = EQ_GLOBAL(ActionStateObject_guess*, 0x006E4DF8);
    if (object == NULL)
        return;
    if (object->state != 0)
    {
        object->slot2();
        return;
    }
    object->slot1();
    return;
}

// FUNCTION: TRILOGY 0x004a911a
// Requests that the server enable or disable incoming tells.
void SetIncomingTells_guess(int unused, const char* choice)
{
    struct SetTellsPacket_guess
    {
        char data[32];
        int state;
    };
    int compare;
    SetTellsPacket_guess packet;

    compare = _strcmpi(choice, (const char*)0x0055b250);
    if (compare == 0)
    {
        packet.state = 1;
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0056455c);
    }
    else
    {
        compare = _strcmpi(choice, (const char*)0x0055b254);
        if (compare == 0)
        {
            packet.state = 0;
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00564524);
        }
        else
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005644ec);
            return;
        }
    }
    EQ_CDECL(void, 0x004E1C45, (void*, u32, void*, u32, int))(
        (void*)0x006b3194, 0x21de, &packet, 0x24, 1);
}

// FUNCTION: TRILOGY 0x004a9195
// For a GM, forwards an inquiry to the Guide interface.
void ForwardGuideInquiry_guess(int unused, int inquiry)
{
    if (g_local_player_guess->gm != 0)
    {
        EQ_METHOD(1, void, 0x004380C0, (void*, int),
            EQ_GLOBAL(void*, 0x006E4DF4))(inquiry);
    }
}

// FUNCTION: TRILOGY 0x004a91b3
// For a GM, forwards a guide action to its action handler.
void ForwardGuideAction_guess(int unused, int guideAction)
{
    if (g_local_player_guess->gm != 0)
    {
        EQ_METHOD(1, void, 0x0043819F, (void*, int),
            EQ_GLOBAL(void*, 0x006E4DF4))(guideAction);
    }
}

// FUNCTION: TRILOGY 0x004a91d1
// Forwards a guide action request when the local player is a GM.
void ForwardGuideActionRequest_guess(int unused, int action)
{
    if (g_local_player_guess->gm != 0)
    {
        EQ_METHOD(1, void, 0x004382AE, (void*, int),
            EQ_GLOBAL(void*, 0x006E4DF4))(action);
    }
}

// FUNCTION: TRILOGY 0x004a91ef
// Forwards guide edit text when the local player is a GM.
void ForwardGuideEdit_guess(int context, char* text)
{
    if (g_local_player_guess->gm != 0)
    {
        EQ_METHOD(1, void, 0x004383BD, (void*, char*),
            EQ_GLOBAL(void*, 0x006E4DF4))(text);
    }
}

// FUNCTION: TRILOGY 0x004a920d
// Lets a GM teleport to a named player or the current target.
// __cdecl. GM teleport command: a name equal to the string at 0x0058FBA4 goes to the target,
// anything else to the named player (the target is set when found).
void gm_goto_guess(int unused, char* name)
{
    struct GMGotoPacket_guess
    {
        char target_name[30];       /* 0x00 */
        char own_name[78];          /* 0x1E */
    };
    char message[128];
    GMGotoPacket_guess packet;

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm != 0)
    {
        if (strcmp(name, EQ_ARRAY(char, 0x0058FBA4)) != 0)
        {
            if (EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(name) != 0)
            {
                EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(name);
            }
            strcpy(packet.target_name, name);
            strcpy(packet.own_name, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
            send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x206E, packet.target_name, 0x6C, 1);
            sprintf(message, EQ_ARRAY(char, 0x005645B8), packet.target_name);
        }
        else
        {
            if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
            {
                return;
            }
            strcpy(packet.target_name, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
            strcpy(packet.own_name, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
            send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x206E, packet.target_name, 0x6C, 1);
            sprintf(message, EQ_ARRAY(char, 0x0056458C), packet.target_name,
                (double)EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->y,
                (double)EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->x,
                (double)EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->z);
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(message, 0xD);
    }
}

// FUNCTION: TRILOGY 0x004a9334
// Starts closing the current game session when quitting is allowed.
void DoQuit_guess(void)
{
    void* connection;

    if (EQ_GLOBAL(u8, 0x006E20F4) != 0)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005645cc);
        return;
    }
    if (g_local_player_guess->stand_state != 'f')
    {
        EQ_GLOBAL(u8, 0x006B7111) = 1;
        *(u32*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0x223f4) = 1;
        *(u32*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 4) = 0xff;
        EQ_GLOBAL(u32, 0x006B63F4) = 0xcb;
        *(u32*)EQ_GLOBAL(void*, 0x006EFE2C) = 1;
        connection = EQ_GLOBAL(void*, 0x006B3194);
        if (connection != NULL)
        {
            EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))(connection, 10000, 0, 0);
            EQ_GLOBAL(void*, 0x006B3194) = NULL;
        }
        EQ_GLOBAL(u8, 0x006B7111) = 2;
    }
}

// FUNCTION: TRILOGY 0x004a93b9
// ORIGINAL FILE: unknown (EQMisc placement inferred from mapped neighbours)
// __cdecl. Displays x/y only if none of the three expansion flags is set; otherwise it includes z.
void show_location_guess(void* position)
{
    char message[200];

    if (EQ_METHOD(0, int, 0x004197A7, (void*), (void*)g_pc_guess)() == 0
        && EQ_METHOD(0, int, 0x004197B2, (void*), (void*)g_pc_guess)() == 0
        && EQ_METHOD(0, int, 0x004197BF, (void*), (void*)g_pc_guess)() == 0)
    {
        sprintf(message, EQ_ARRAY(char, 0x00564638),
                (double)*(float*)((char*)position + 0x34),
                (double)*(float*)((char*)position + 0x38));
    }
    else
    {
        sprintf(message, EQ_ARRAY(char, 0x00564610),
                (double)*(float*)((char*)position + 0x34),
                (double)*(float*)((char*)position + 0x38),
                (double)*(float*)((char*)position + 0x3c));
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(message);
}

// FUNCTION: TRILOGY 0x004a945b
// Changes rain locally or asks the zone server to change it.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl /rain handler. The first word of the argument is a number or "on" (rain on), "kill" (stop and destroy the
// clouds, local change only) or anything else (rain off). With ask_server zero, or either flag at 0x006B63F8 /
// 0x006B6494 set, the change is made on the weather object at 0x006E4DE0; otherwise it prints a line and sends opcode
// 0x218A to the zoneserver with the rain level (0x17 minus the byte that 0x006E4D54 points to).
// @early-stop: define_cloud spills to the args slot (the original keeps it in ESI with the constant 1), 1 is reloaded in each flag arm, and EQ_THISCALL's dummy EDX xor adds 6 insns (ratio 0.8644).
void do_rain(int ask_server, char* args)
{
    typedef int (__cdecl *tDefineParticleCloud_guess)(void*, const char*, int, int, int, float, int, int, int, int, int,
        float*, float*, float*, float, float, int, float, int, float, u8*, int);
    typedef int (__cdecl *tGetPointerFromDictionary_guess)(void*, const char*);
    typedef void (__cdecl *tDestroyParticleCloud_guess)(void*, void*);
    char word[40];
    int msg[4];
    float top[3];
    float zero[3];
    float low[3];
    float high[3];
    u8 color[4];
    int flag;
    tDefineParticleCloud_guess define_cloud;
    tDestroyParticleCloud_guess destroy_cloud;

    low[0] = EQ_GLOBAL(float, 0x005356F0);
    low[1] = EQ_GLOBAL(float, 0x005356F0);
    low[2] = EQ_GLOBAL(float, 0x00535688);
    high[0] = EQ_GLOBAL(float, 0x005356EC);
    high[1] = EQ_GLOBAL(float, 0x005356EC);
    top[2] = -2.0f;
    high[2] = EQ_GLOBAL(float, 0x00535688);
    top[0] = 0.0f;
    color[2] = 0x80;
    color[1] = 0x80;
    top[1] = 0.0f;
    color[0] = 0x80;
    zero[0] = 0.0f;
    zero[1] = 0.0f;
    zero[2] = 0.0f;
    EQ_THISCALL(char*, 0x004DBC84, (void*, int, char*, char*))(EQ_GLOBAL(void*, 0x006EFE2C), 0, args, word);
    if (EQ_CDECL(int, 0x004A47C8, (char*))(word) == 0)
    {
        flag = 0;
        if (_strnicmp(word, EQ_ARRAY(char, 0x0055B250), 2) == 0)
            flag = 1;
    }
    else
        flag = atoi(word);

    if (ask_server == 0 || EQ_GLOBAL(u8, 0x006B63F8) != 0 || DAT_006b6494 != 0)
    {
        if (_strnicmp(word, EQ_ARRAY(char, 0x0053E020), 4) == 0)
        {
            if (g_cdisplay_guess->rainActive != 0)
            {
                EQ_THISCALL(void, 0x0040DA27, (void*, int, int, int))(g_display_guess, 0, 1, 0);
                destroy_cloud = EQ_GLOBAL(tDestroyParticleCloud_guess, 0x006E5218);
                destroy_cloud(g_cdisplay_guess->world_guess, g_displayManager_guess + 0xF4);
                destroy_cloud(g_cdisplay_guess->world_guess, g_displayManager_guess + 0xFC);
                destroy_cloud(g_cdisplay_guess->world_guess, g_displayManager_guess + 0x104);
                g_cdisplay_guess->rainActive = 0;
            }
        }
        else if (_strnicmp(word, EQ_ARRAY(char, 0x0055B254), 3) == 0
            || flag == 0)
        {
            if (g_cdisplay_guess->rainActive != 0)
                EQ_THISCALL(void, 0x0040DA27, (void*, int, int, int))(g_display_guess, 0, 1, 1);
        }
        else if (g_cdisplay_guess->rainActive == 0)
        {
            EQ_THISCALL(void, 0x0040D98D, (void*, int, int, int))(g_display_guess, 0, 1, 1);
            define_cloud = EQ_GLOBAL(tDefineParticleCloud_guess, 0x006E50B0);
            *(int*)(g_displayManager_guess + 0x100) = define_cloud(
                g_cdisplay_guess->world_guess, (char*)0x00564684,
                1, 0x2100, 1000, 0.0f, 0, 0, 0, 0, 0,
                low, high, zero, 0.0f, 0.0f, 200, EQ_GLOBAL(float, 0x0053515C), -1, 1.0f, color, 0);
            *(int*)(g_displayManager_guess + 0xEC) = EQ_GLOBAL(tGetPointerFromDictionary_guess, 0x006E52E0)(
                g_cdisplay_guess->world_guess, (char*)0x00564670);
            *(int*)(g_displayManager_guess + 0xF8) = define_cloud(
                g_cdisplay_guess->world_guess, (char*)0x00564664,
                4, 0x2100, 1000, 0.0f, 0, 0, 0, 0, 0,
                low, high, zero, 0.0f, 0.0f, 500, EQ_GLOBAL(float, 0x0053515C), -1, EQ_GLOBAL(float, 0x005351D0), 0,
                *(int*)(g_displayManager_guess + 0xEC));
            *(int*)(g_displayManager_guess + 0xF0) = define_cloud(
                g_cdisplay_guess->world_guess, (char*)0x00564658,
                2, 0x203A, 1000, 0.0f, 0, 0, 0, 0, 0,
                low, high, top, 0.0f, 0.0f, 2000, EQ_GLOBAL(float, 0x0053515C), 3, 1.0f, 0, 0);
        }
    }
    else
    {
        msg[0] = 1;
        msg[1] = 0;
        if (_strnicmp(word, EQ_ARRAY(char, 0x0055B254), 3) == 0
            || flag == 0)
        {
            EQ_THISCALL(void, 0x004D05B7, (void*, int, const char*))(EQ_GLOBAL(void*, 0x006EFE2C), 0,
                (char*)0x00564690);
            msg[3] = 0;
            msg[2] = 0;
        }
        else
        {
            EQ_THISCALL(void, 0x004D05B7, (void*, int, const char*))(EQ_GLOBAL(void*, 0x006EFE2C), 0,
                (char*)0x005646B4);
            msg[3] = 0x17 - *EQ_GLOBAL(u8*, 0x006E4D54);
            msg[2] = 1;
        }
        send_message_guess(g_world_connection_guess, 0x218A, msg, 0x10, 1);
    }
}

// FUNCTION: TRILOGY 0x004a97c8
// Changes snowfall locally or asks the zone server to change it.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl /snow handler, the same shape as do_rain. The first word of the argument is a number or "on" (snow on),
// "kill" (stop and destroy the clouds, local change only) or anything else (snow off). With ask_server zero, or either
// flag at 0x006B63F8 / 0x006B6494 set, the change is made on the weather object at 0x006E4DE0; otherwise it prints a
// line and sends opcode 0x218A to the zoneserver with the snow level (0x17 minus the byte that 0x006E4D54 points to).
// @early-stop: define_cloud spills to the args slot (the original keeps it in ESI after the constant 1), so the start arm's register use differs; EQ_THISCALL's dummy EDX xor adds 7 insns (ratio 0.9180).
void do_snow(int ask_server, char* args)
{
    typedef int (__cdecl *tDefineParticleCloud_guess)(void*, const char*, int, int, int, float, int, int, int, int, int,
        float*, float*, float*, float, float, int, float, int, float, u8*, int);
    typedef int (__cdecl *tGetPointerFromDictionary_guess)(void*, const char*);
    typedef void (__cdecl *tDestroyParticleCloud_guess)(void*, void*);
    char word[40];
    int msg[4];
    float top[3];
    float zero[3];
    float low[3];
    float high[3];
    u8 cloud_color[4];
    u8 color[4];
    int flag;
    tDefineParticleCloud_guess define_cloud;
    tDestroyParticleCloud_guess destroy_cloud;

    low[0] = EQ_GLOBAL(float, 0x005356F8);
    low[1] = EQ_GLOBAL(float, 0x005356F8);
    cloud_color[0] = 0xff;
    low[2] = EQ_GLOBAL(float, 0x00535688);
    cloud_color[1] = 0xff;
    cloud_color[2] = 0xff;
    high[0] = EQ_GLOBAL(float, 0x005356F4);
    high[1] = EQ_GLOBAL(float, 0x005356F4);
    color[2] = 0xff;
    color[1] = 0xff;
    high[2] = EQ_GLOBAL(float, 0x00535688);
    color[0] = 0xff;
    top[0] = 0.0f;
    top[1] = 0.0f;
    zero[0] = 0.0f;
    top[2] = -1.0f;
    zero[1] = 0.0f;
    cloud_color[3] = 0;
    flag = 0;
    zero[2] = 0.0f;
    EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(args, word);
    if (EQ_CDECL(int, 0x004A47C8, (char*))(word) != 0)
        flag = atoi(word);
    else
    {
        if (_strnicmp(word, EQ_ARRAY(char, 0x0055B250), 2) == 0)
            flag = 1;
    }

    if (ask_server == 0 || EQ_GLOBAL(u8, 0x006B63F8) != 0 || DAT_006b6494 != 0)
    {
        if (_strnicmp(word, EQ_ARRAY(char, 0x0053E020), 4) == 0)
        {
            if (g_cdisplay_guess->snowActive_guess != 0)
            {
                EQ_METHOD(2, void, 0x0040DA27, (void*, int, int), g_display_guess)(2, 0);
                destroy_cloud = EQ_GLOBAL(tDestroyParticleCloud_guess, 0x006E5218);
                destroy_cloud(g_cdisplay_guess->world_guess, g_displayManager_guess + 0x10C);
                destroy_cloud(g_cdisplay_guess->world_guess, g_displayManager_guess + 0x114);
                destroy_cloud(g_cdisplay_guess->world_guess, g_displayManager_guess + 0x11C);
                g_cdisplay_guess->snowActive_guess = 0;
            }
        }
        else if (_strnicmp(word, EQ_ARRAY(char, 0x0055B254), 3) == 0
            || flag == 0)
        {
            if (g_cdisplay_guess->snowActive_guess != 0)
                EQ_METHOD(2, void, 0x0040DA27, (void*, int, int), g_display_guess)(2, 1);
        }
        else if (g_cdisplay_guess->snowActive_guess == 0)
        {
            EQ_METHOD(2, void, 0x0040D98D, (void*, int, int), g_display_guess)(2, 1);
            define_cloud = EQ_GLOBAL(tDefineParticleCloud_guess, 0x006E50B0);
            *(int*)(g_displayManager_guess + 0x118) = define_cloud(
                g_cdisplay_guess->world_guess, (char*)0x005646F0,
                1, 0x2100, 1000, 0.0f, 0, 0, 0, 0, 0,
                low, high, zero, 0.0f, 0.0f, 500, EQ_GLOBAL(float, 0x0053515C), -1, 1.0f, color, 0);
            *(int*)(g_displayManager_guess + 0x110) = define_cloud(
                g_cdisplay_guess->world_guess, (char*)0x005646E4,
                1, 0x2100, 1000, 0.0f, 0, 0, 0, 0, 0,
                low, high, zero, 0.0f, 0.0f, 200, EQ_GLOBAL(float, 0x0053515C), -1, EQ_GLOBAL(float, 0x005351D0), color, 0);
            *(int*)(g_displayManager_guess + 0xE8) = EQ_GLOBAL(tGetPointerFromDictionary_guess, 0x006E52E0)(
                g_cdisplay_guess->world_guess, (char*)0x0055D608);
            *(int*)(g_displayManager_guess + 0x108) = define_cloud(
                g_cdisplay_guess->world_guess, (char*)0x005646D8,
                3, 0x20BA, 1500, 0.0f, 0, 0, 0, 0, 0,
                low, high, top, 0.0f, 0.0f, 15000, EQ_GLOBAL(float, 0x00535190), 5, EQ_GLOBAL(float, 0x00535154), cloud_color,
                *(int*)(g_displayManager_guess + 0xE8));
        }
    }
    else
    {
        msg[0] = 0;
        msg[1] = 1;
        if (_strnicmp(word, EQ_ARRAY(char, 0x0055B254), 3) == 0
            || flag == 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                (char*)0x005646FC);
            msg[3] = 0;
            msg[2] = 0;
        }
        else
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                (char*)0x00564720);
            msg[3] = 0x17 - *EQ_GLOBAL(u8*, 0x006E4D54);
            msg[2] = 1;
        }
        send_message_guess(g_world_connection_guess, 0x218A, msg, 0x10, 1);
    }
}

// FUNCTION: TRILOGY 0x004a9b7f
// Sends a text message to the world server.
void SendTextMessage_guess(int context, char* message)
{
    size_t message_length;

    message_length = strlen(message);
    send_message_guess(
        g_world_connection_guess, 0x20b7, message, message_length + 1, 1);
    return;
}

// FUNCTION: TRILOGY 0x004a9c33
// Changes the player's AFK state and away message.
// __cdecl /afk command handler (the first argument is unused). The first three characters of the argument
// (any case) set AFK with no away text; any other non-empty argument sets AFK and copies it to the away text
// buffer; an empty argument toggles AFK. Then it prints the new state and sends spawn appearance 0x18.
void cmd_afk_guess(int ignoredArg, char* cmdArgs)
{
    int cmpResult;
    size_t argLength;
    struct AfkMessage_guess
    {
        u32 spawn_id;
        short type;
        int parameter;
    } appearanceMsg;

    cmpResult = _strnicmp(cmdArgs, EQ_ARRAY(char, 0x0055B254), 3);
    if (cmpResult == 0)
    {
        EQ_METHOD(1, void, 0x004B2548, (void*, int), EQ_GLOBAL(void*, 0x006E4D8C))(cmpResult);
        EQ_ARRAY(char, 0x006E1F50)[0] = 0;
    }
    else
    {
        argLength = strlen(cmdArgs);
        if (argLength > 0)
        {
            EQ_METHOD(1, void, 0x004B2548, (void*, int), EQ_GLOBAL(void*, 0x006E4D8C))(1);
            strcpy(EQ_ARRAY(char, 0x006E1F50), cmdArgs);
        }
        else
        {
            cmpResult = 1 - EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->afk;
            EQ_METHOD(1, void, 0x004B2548, (void*, int), EQ_GLOBAL(void*, 0x006E4D8C))(cmpResult);
            EQ_ARRAY(char, 0x006E1F50)[0] = 0;
        }
    }

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->afk != 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(char, 0x00564774), 0x111);
    }
    else
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(char, 0x00564744), 0x111);
    }
    appearanceMsg.type = 0x18;
    appearanceMsg.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
    appearanceMsg.parameter = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->afk;
    send_message_guess(g_world_connection_guess, 0x20f5, &appearanceMsg, 0xc, 1);
}

// FUNCTION: TRILOGY 0x004a9d00
// Starts a loot interaction with the current target when it is a corpse.
void RequestLootTargetCorpse_guess(void)
{
    EQPlayer* target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);

    if (target == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(char, 0x0056479C), 0x111);
        return;
    }
    EQ_METHOD(2, void, 0x004DD440, (void*, EQPlayer*, int), EQ_GLOBAL(void*, 0x006EFE2C))(target, 0);
    return;
}

// FUNCTION: TRILOGY 0x004a9d2e
// Controls visibility of existing corpses, including hiding those outside the group.
// __cdecl. Hides existing corpses except yours for ALL, hides corpses outside the group for ALLBUTGROUP,
// and makes existing corpses visible for any other non-empty argument.
// @early-stop: ALLBUTGROUP loop still differs: the original is a guarded bottom-tested loop (mov edi after the guard); a do-while there makes VC6 keep the constant 1 in EDI.
void cmd_hidecorpses_guess(int unusedArg, char* ptr)
{
    int* actorInfo;
    t3dSetActorInvisible_t visibilityCallback;
    int comparison;
    int strippedName;
    char* currentSpawn;
    char member[64];

    if (*ptr == '\0')
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00564934));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005648BC));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00564870));
    }
    else
    {
        currentSpawn = (char*)EQ_GLOBAL(void*, 0x006E4D58);
        if (_strcmpi(
                ptr, EQ_ARRAY(char, 0x0055974C)) == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00564844), 0x0F);
            for (; currentSpawn != 0; currentSpawn = *(char**)(currentSpawn + 0x68))
            {
                if ((*(char*)(currentSpawn + 0x98) == '\x02') || (*(char*)(currentSpawn + 0x98) == '\x03'))
                {
                    strippedName = (int)EQ_METHOD(1, char*, 0x004D0436, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(currentSpawn + 1);
                    strcpy(member, (char*)strippedName);
                    comparison = _strcmpi(
                        member, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
                    if ((comparison != 0)
                        && ((actorInfo = *(int**)(currentSpawn + 0x70), actorInfo != 0 && (*actorInfo != 0))))
                    {
                        actorInfo[0x6A] = 1;
                        t3dSetActorInvisible((void*)**(int**)(currentSpawn + 0x70), 1);
                    }
                }
            }
        }
        else
        {
            if (_strcmpi(
                    ptr, EQ_ARRAY(char, 0x00564838)) == 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005647EC), 0x0F);
                visibilityCallback = t3dSetActorInvisible;
                for (; currentSpawn != 0; currentSpawn = *(char**)(currentSpawn + 0x68))
                {
                    if ((*(char*)(currentSpawn + 0x98) == '\x02') || (*(char*)(currentSpawn + 0x98) == '\x03'))
                    {
                        strippedName = (int)EQ_METHOD(1, char*, 0x004D0436, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(currentSpawn + 1);
                        strcpy(member, (char*)strippedName);
                        comparison = _strcmpi(
                            member, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
                        if (comparison != 0)
                        {
                            ptr = (char*)0x6;
                            do
                            {
                                comparison = _strcmpi(
                                    member, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
                                if (comparison != 0)
                                {
                                    comparison = EQ_METHOD(1, int, 0x0047EFCE, (void*, char*), EQ_GLOBAL(void*, 0x006E4DA8))(member);
                                    if (comparison == 0)
                                    {
                                        actorInfo = *(int**)(currentSpawn + 0x70);
                                        if ((actorInfo != 0) && (*actorInfo != 0))
                                        {
                                            actorInfo[0x6A] = 1;
                                            visibilityCallback((void*)**(int**)(currentSpawn + 0x70), 1);
                                        }
                                    }
                                    else
                                    {
                                        actorInfo = *(int**)(currentSpawn + 0x70);
                                        if ((actorInfo != 0) && (*actorInfo != 0))
                                        {
                                            actorInfo[0x6A] = 0;
                                            visibilityCallback((void*)**(int**)(currentSpawn + 0x70), 0);
                                        }
                                    }
                                }
                                ptr = (char*)((int)ptr + -1);
                            }
                            while (ptr != 0);
                        }
                    }
                }
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005647C4), 0x0F);
                for (; currentSpawn != 0; currentSpawn = *(char**)(currentSpawn + 0x68))
                {
                    if ((((*(char*)(currentSpawn + 0x98) == '\x02') || (*(char*)(currentSpawn + 0x98) == '\x03'))
                        && (actorInfo = *(int**)(currentSpawn + 0x70), actorInfo != 0)) && (*actorInfo != 0))
                    {
                        actorInfo[0x6A] = 0;
                        t3dSetActorInvisible((void*)**(int**)(currentSpawn + 0x70), 0);
                    }
                }
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004a9f44
// Toggles environmental reverb for game audio.
void ToggleReverbCommand_guess(void)
{
    char* reverb_state = EQ_GLOBAL(char*, 0x006E4DD8);

    if (reverb_state != NULL)
        EQ_METHOD(0, void, 0x004E3E89, (char*), reverb_state)();
}

// FUNCTION: TRILOGY 0x004a9f54
// Turns the option to inspect other players by right-clicking on or off.
void ToggleInspectOthers_guess(int unused, const char* choice)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(const char*, const char*, const char*, const char*);
    int compare;
    const char* setting;

    compare = _strcmpi(choice, EQ_ARRAY(char, 0x0055B250));
    if (compare == 0)
    {
        EQ_GLOBAL(int, 0x006E20E4) = 1;
        setting = EQ_ARRAY(char, 0x00563F84);
    }
    else
    {
        compare = _strcmpi(choice, EQ_ARRAY(char, 0x0055B254));
        if (compare != 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00564A88));
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00564A68));
            goto report_state;
        }
        EQ_GLOBAL(int, 0x006E20E4) = 0;
        setting = EQ_ARRAY(char, 0x0053DAFC);
    }

    EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
        EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00564AD0), setting, EQ_ARRAY(char, 0x0053DB50));

report_state:
    if (EQ_GLOBAL(int, 0x006E20E4) == 0)
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x005649CC));
    else
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00564A1C));
}

// FUNCTION: TRILOGY 0x004a9ff5
// Broadcasts a GM emote message to the current zone.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQMisc.cpp (by address; the slash-command table names it /emotezone)
// __cdecl command handler: sends the text (cut to 511 characters) as OP_GMEmoteZone, a fixed 512 bytes. The
// copy is unbounded (strcpy into the 512-byte buffer, as in the original).
void cmd_emotezone_guess(EQPlayer* player, char* args)
{
    char text[512];

    strcpy(text, args);
    text[511] = '\0';
    send_message_guess(g_world_connection_guess, 0x21E3, text, 0x200, 1);
}

// FUNCTION: TRILOGY 0x004aa033
// Switches left and right audio channels, or restores their normal order.
// __cdecl command handler (/reversesound by its help text): `norm...` clears the reverse-sound flag, `rev...`
// sets it, anything else keeps it; empty input prints the help. Reports the state and saves options2.
void cmd_reversesound_guess(int unused, char* args)
{
    if (*args == '\0')
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564BC8));
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564B9C));
    }
    else if (_strnicmp(args, EQ_ARRAY(char, 0x00564B94), 4) == 0)
    {
        EQ_GLOBAL(int, 0x006B6F08) = 0;
    }
    else if (_strnicmp(args, EQ_ARRAY(char, 0x00564B58), 3) == 0)
    {
        EQ_GLOBAL(int, 0x006B6F08) = 1;
    }
    if (EQ_GLOBAL(int, 0x006B6F08) == 1)
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564AE0));
    else
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564B5C));
    EQ_METHOD(0, void, 0x004CF3E1, (CEverQuest*), pinstCEverQuest_guess)();
}

// FUNCTION: TRILOGY 0x004aa0c4
// Confirms a request to move all characters to a less crowded server.
void ConfirmCharacterMove_guess(int unused, const char* command)
{
    int compare;

    compare = _strcmpi(command, EQ_ARRAY(char, 0x00564D60));
    if (compare == 0)
    {
        send_message_guess(
            g_world_connection_guess, 0x21e4, (void*)compare, compare, 1);
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(
        EQ_ARRAY(char, 0x00564CE0));
    return;
}

// FUNCTION: TRILOGY 0x004AA100
// Turns eqlog.txt logging on or off.
void ToggleLogging_guess(int unused, const char* command)
{
    int compare;

    compare = _strnicmp(
        command, EQ_ARRAY(char, 0x0055B254), 3);
    if (compare == 0)
    {
        g_bazaar_log_on_guess = 0;
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(
            EQ_ARRAY(char, 0x00564D88), 0x111);
        return;
    }

    compare = _strnicmp(
        command, EQ_ARRAY(char, 0x0055B250), 2);
    if (compare == 0)
    {
        g_bazaar_log_on_guess = 1;
    }
    else
    {
        g_bazaar_log_on_guess = 1 - g_bazaar_log_on_guess;
        if (g_bazaar_log_on_guess == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(
                EQ_ARRAY(char, 0x00564D88), 0x111);
            return;
        }
    }
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(
        EQ_ARRAY(char, 0x00564D64), 0x111);
    return;
}

// FUNCTION: TRILOGY 0x004aa172
// Adds a timestamped note to notes.txt and reports whether it was saved.
// __cdecl command handler (/note by its strings): appends "[ctime] text" to notes.txt (mode at 0x00564e48)
// and reports success or failure in chat. The ctime string is copied unbounded into 32 bytes.
void cmd_note_guess(int unused, char* args)
{
    char stamp[32];
    time_t now;
    size_t length;
    FILE* file;

    now = time(0);
    strcpy(stamp, ctime(&now));
    length = strlen(stamp);
    if (stamp[length - 1] == '\n')
        stamp[length - 1] = '\0';
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(EQ_ARRAY(char, 0x00564E3C), EQ_ARRAY(char, 0x00564E48));
    if (file != 0)
    {
        EQ_CDECL(int, 0x00524138, (FILE*, const char*, ...))(file, EQ_ARRAY(char, 0x00564E30), stamp, args);
        EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564E00));
    }
    else
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564DB0));
}

// FUNCTION: TRILOGY 0x004aa276
// Starts following a valid group member and prints safety and pathing guidance.
void auto_follow_guess(void)
{
    char message[256];

    if ((EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
        || (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type != 0))
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564E8C), 0x111);
        return;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == g_local_player_guess)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565088), 0x111);
        return;
    }
    if (EQ_METHOD(1, int, 0x0047EFCE, (void*, char*), g_pc_guess)(
                                                          EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name) == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565054), 0x111);
        return;
    }
    // !(>=) rather than (<): a NaN also takes the "cannot move" branch, as the original's FCOMP/JAE does
    if (!(g_local_player_guess->actor_info->unk_0xB8 >= *EQ_ARRAY(float, 0x005356FC)))
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565020), 0x111);
        return;
    }
    if ((g_local_player_guess->stand_state == 'f')
        || (g_local_player_guess->linkdead != '\0'))
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564EC4));
        return;
    }
    if (g_local_player_guess->stand_state != 'd')
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00540CB4), 0x111);
        return;
    }
    sprintf(
        message, EQ_ARRAY(char, 0x00565000),
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564F7C), 0xD);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00564EF8), 0xD);
    g_local_player_guess->actor_info->follow_player_guess
        = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
}

// FUNCTION: TRILOGY 0x004aa3b8
// Changes the local player's roleplay status and sends the new status to the server.
// __cdecl. Sets, toggles or cycles the local player's roleplay (anon) state, tells the player and the server
// (spawn appearance type 0x15) and saves the profile and options.
void roleplay_guess(EQPlayer* player, char* text)
{
    char message[256];
    struct MsgSpawnAppearance_guess
    {
        u32 spawn_id;           /* 0x00 */
        s16 type;               /* 0x04 */
        u16 unknown0x06;        /* 0x06 */
        u32 parameter;          /* 0x08 */
    } msg;
    s32* anon;

    if (_strnicmp(
            text, EQ_ARRAY(char, 0x0055B254), 3) == 0)
    {
        anon = &g_local_player_guess->anon;
        *anon = (*anon != 2);
    }
    else if (_strnicmp(
                 text, EQ_ARRAY(char, 0x0055B250), 2) == 0)
    {
        anon = &g_local_player_guess->anon;
        *anon = (*anon != 0) + 2;
    }
    else
    {
        switch (g_local_player_guess->anon)
        {
        case 0:
            g_local_player_guess->anon = 2;
            break;
        case 1:
            g_local_player_guess->anon = 3;
            break;
        case 2:
            g_local_player_guess->anon = 0;
            break;
        case 3:
            g_local_player_guess->anon = 1;
            break;
        default:
            g_local_player_guess->anon = 0;
            break;
        }
    }
    if (g_local_player_guess->anon > 1)
    {
        sprintf(
            message, EQ_ARRAY(char, 0x005650C8), 0x111);
    }
    else
    {
        sprintf(
            message, EQ_ARRAY(char, 0x005650A8), 0x111);
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(message);
    msg.type = 0x15;
    msg.spawn_id = player->spawn_id;
    msg.parameter = player->anon;
    send_message_guess(
        &EQ_GLOBAL(void*, 0x006B3194), 0x20F5, &msg, 0xC, 1);
    g_pc_guess->anon = (u8)g_local_player_guess->anon;
    EQ_GLOBAL(int, 0x006B7120) = 5;
    EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), pinstCEverQuest_guess)(0, 1);
    EQ_GLOBAL(int, 0x006B6860) = g_local_player_guess->anon;
    EQ_METHOD(0, void, 0x004CF31A, (void*), pinstCEverQuest_guess)();
    EQ_METHOD(1, void, 0x00411EE6, (void*, EQPlayer*), g_display_guess)(g_local_player_guess);
}

// FUNCTION: TRILOGY 0x004aa52b
// Changes a player's anonymous status and sends the update to the server.
// __cdecl. Sets, clears or toggles the anonymous flag of a player (the local player unless the caller is a GM),
// refused while roleplaying; tells the player and the server (spawn appearance type 0x15) and, for the local
// player, saves the profile and options.
void anonymous_guess(EQPlayer* player, char* text)
{
    char message[256];
    struct MsgSpawnAppearance_guess
    {
        u32 spawn_id;           /* 0x00 */
        s16 type;               /* 0x04 */
        u16 unknown0x06;        /* 0x06 */
        u32 parameter;          /* 0x08 */
    } msg;

    if (player == 0)
    {
        return;
    }
    if (player->type != 0)
    {
        return;
    }
    if (player != g_local_player_guess && g_local_player_guess->gm == 0)
    {
        player = g_local_player_guess;
    }
    if (player->anon > 1)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565118), 0x111);
        return;
    }
    if (_strnicmp(
            text, EQ_ARRAY(char, 0x0055B254), 3) == 0)
    {
        player->anon = 0;
    }
    else if (_strnicmp(
                 text, EQ_ARRAY(char, 0x0055B250), 2) == 0)
    {
        player->anon = 1;
    }
    else
    {
        player->anon = 1 - player->anon;
    }
    if (player->anon != 0)
    {
        if (player == g_local_player_guess)
        {
            sprintf(
                message, EQ_ARRAY(char, 0x0055B2E0));
        }
        else
        {
            sprintf(
                message, EQ_ARRAY(char, 0x00565100),
                EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(player->name));
        }
    }
    else
    {
        if (player == g_local_player_guess)
        {
            sprintf(
                message, EQ_ARRAY(char, 0x0055B2C0));
        }
        else
        {
            sprintf(
                message, EQ_ARRAY(char, 0x005650E4),
                EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(player->name));
        }
    }
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
    msg.type = 0x15;
    msg.spawn_id = player->spawn_id;
    msg.parameter = player->anon;
    send_message_guess(
        &EQ_GLOBAL(void*, 0x006B3194), 0x20F5, &msg, 0xC, 1);
    if (player == g_local_player_guess)
    {
        g_pc_guess->anon = (u8)g_local_player_guess->anon;
        EQ_GLOBAL(int, 0x006B7120) = 5;
        EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), pinstCEverQuest_guess)(0, 1);
        EQ_GLOBAL(int, 0x006B6860) = g_local_player_guess->anon;
        EQ_METHOD(0, void, 0x004CF31A, (void*), pinstCEverQuest_guess)();
    }
    EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), g_display_guess)(player);
}

// FUNCTION: TRILOGY 0x004AA6E7
// Adds or removes a name from the ignore list, or lists ignored players.
void ignore_command_guess(int unused, char* name)
{
    char message[256];
    int found_slot;
    int free_slot;
    int i;
    char* p;
    EQPlayer* player;

    if (*name == '\0')
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565210), 0xD);
        p = g_ignore_list_guess;
        i = 0x1E;
        do
        {
            if (*p != '\0')
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(p, 0xF);
            }
            p += 0x20;
            i--;
        } while (i != 0);
        return;
    }
    found_slot = -1;
    free_slot = -1;
    i = 0;
    p = g_ignore_list_guess;
    do
    {
        if (_strcmpi(p, name) == 0)
        {
            found_slot = i;
        }
        if (*p == '\0')
        {
            free_slot = i;
        }
        i++;
        p += 0x20;
    } while ((short)i < 0x1E);
    if ((short)found_slot >= 0)
    {
        g_ignore_list_guess[(short)found_slot * 0x20] = 0;
        EQ_METHOD(1, void, 0x004D5A24, (void*, int), pinstCEverQuest_guess)(0);
        sprintf(
            message, EQ_ARRAY(char, 0x005651E4), name);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xF);
    }
    else
    {
        if ((short)free_slot >= 0)
        {
            player = EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(name);
            if (player != 0 && player->gm != 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005651C4), 0xF);
                return;
            }
            strcpy(
                g_ignore_list_guess + (short)free_slot * 0x20, name);
            EQ_METHOD(1, void, 0x004D5A24, (void*, int), pinstCEverQuest_guess)(0);
            sprintf(
                message, EQ_ARRAY(char, 0x0056519C), name);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xF);
        }
        else
        {
            sprintf(
                message, EQ_ARRAY(char, 0x00565148), 0x1E);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xF);
        }
    }
}

// FUNCTION: TRILOGY 0x004aa817
// Declines the pending duel challenge and sends the response.
void decline_duel_guess(void)
{
    struct DuelDeclinePacket_guess
    {
        u32 challenger_spawn_id;
        u32 local_spawn_id;
        u8 zero;
    };
    EQPlayer* player;
    char message[128];
    DuelDeclinePacket_guess packet;

    if (EQ_GLOBAL(u8, 0x006B64D0) != 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565298), 0xF);
        return;
    }
    if (EQ_GLOBAL(int, 0x006B64CC) != 0)
    {
        player = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(EQ_GLOBAL(int, 0x006B64CC));
        if (player != 0)
        {
            sprintf(
                message, EQ_ARRAY(char, 0x00565270), (char*)player + 1);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
            packet.challenger_spawn_id = player->spawn_id;
            packet.local_spawn_id = g_local_player_guess->spawn_id;
            packet.zero = 0;
            send_message_guess(
                &EQ_GLOBAL(void*, 0x006B3194), 0x20D0, &packet, 0xC, 1);
        }
        EQ_GLOBAL(int, 0x006B64CC) = 0;
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565238), 0x111);
    return;
}

// FUNCTION: TRILOGY 0x004aa8da
// Challenges the selected PC to a duel or accepts that PC's pending challenge.
// /duel: challenges the target PC, or accepts a challenge it made. Chat colour 0x111 for the target and
// self checks, 0xF for the rest. The target is the global at 0x006E4DAC, the local player 0x006E4D8C.
void duel_guess(void)
{
    struct DuelPacket_guess
    {
        u32 target_spawn_id;
        u32 local_spawn_id;
    };
    char message[128];
    DuelPacket_guess packet;

    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0 && EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type == 0)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == g_local_player_guess)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565424), 0x111);
        }
        else if (EQ_GLOBAL(u8, 0x006B64D0) != 0)
        {
            if (EQ_GLOBAL(u32, 0x006B64C8) == EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005653EC), 0xF);
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005653A8), 0xF);
            }
        }
        else
        {
            if (EQ_GLOBAL(u32, 0x006B64CC) == EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id)
            {
                EQ_GLOBAL(u8, 0x006B64D0) = 1;
                EQ_GLOBAL(u32, 0x006B64C8) = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id;
                sprintf(
                    message, EQ_ARRAY(char, 0x00565370), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xF);
                packet.target_spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id;
                packet.local_spawn_id = g_local_player_guess->spawn_id;
                send_message_guess(
                    g_world_connection_guess, 0x20CF, &packet, 8, 1);
                send_message_guess(
                    g_world_connection_guess, 0x215D, &packet, 8, 1);
                return;
            }
            if (EQ_GLOBAL(u32, 0x006B64CC) != 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565328), 0xF);
            }
            else
            {
                EQ_GLOBAL(u32, 0x006B64C8) = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id;
                sprintf(
                    message, EQ_ARRAY(char, 0x005652F8), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0xF);
                packet.target_spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id;
                packet.local_spawn_id = g_local_player_guess->spawn_id;
                send_message_guess(
                    g_world_connection_guess, 0x20CF, &packet, 8, 1);
                return;
            }
        }
    }
    else
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005652D8), 0x111);
    }
}

// FUNCTION: TRILOGY 0x004aaa71
// Converts the selected player into an NPC-style combat target with a chosen race and level cap.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// /becomenpc <race#> [sex (M, F, N)] [material#] [head#] [face#] [height] [level max]: changes the target PC
// (opcode 0x2091) and makes it killable as an NPC (opcode 0x218C). Does nothing while 0x006B63F8 or 0x006B6494 is set.
void do_becomenpc(int unused, char* commandLine)
{
    struct BecomeNpcPacket_guess
    {
        char target_name[30];
        char player_name[30];
        u8 zero;            // 0x3C
        u16 race;           // 0x3E
        u8 sex;             // 0x40
        u16 material;       // 0x42
        u16 head;           // 0x44
        u16 face;           // 0x46
    };
    struct BecomeNpcLevelPacket_guess
    {
        u32 spawn_id;
        int level;
    };
    struct Frame_guess
    {
        BecomeNpcLevelPacket_guess level_packet;
        int material;
        int head;
        char pad[3];
        u8 sex;
    };
    Frame_guess fr;
    int race;
    short face;

    face = -1;
    fr.material = -1;
    fr.head = -1;
    fr.sex = 2;
    if (EQ_GLOBAL(u8, 0x006B63F8) == 0 && DAT_006b6494 == 0)
    {
        if (*commandLine == '\0')
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00565578));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005654F4));
        }
        else if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005654C8), 0xD);
            return;
        }
        else
        {
            size_t len = strlen(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
            if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type != 0 &&
                isdigit(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name[(short)len - 1]))
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005654AC), 0xD);
                return;
            }
            else
            {
                // Member order sets the stack layout; reordering these changes the frame.
                struct BecomeNpcLocals_guess
                {
                    char race_text[32];
                    char height_text[32];
                    char material_text[32];
                    char face_text[32];
                    char level_text[32];
                    char head_text[32];
                    BecomeNpcPacket_guess packet;
                    char sex_text[32];
                };
                BecomeNpcLocals_guess locals;
                char message[512];
                char* rest;

                rest = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), pinstCEverQuest_guess)(commandLine, locals.race_text);
                rest = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), pinstCEverQuest_guess)(rest, locals.sex_text);
                rest = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), pinstCEverQuest_guess)(rest, locals.material_text);
                rest = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), pinstCEverQuest_guess)(rest, locals.head_text);
                rest = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), pinstCEverQuest_guess)(rest, locals.face_text);
                rest = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), pinstCEverQuest_guess)(rest, locals.height_text);
                EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), pinstCEverQuest_guess)(rest, locals.level_text);
                race = atoi(locals.race_text);
                if (locals.sex_text[0] != '\0')
                {
                    if (_strnicmp(locals.sex_text, EQ_ARRAY(char, 0x005654A8), 1) == 0)
                    {
                        fr.sex = 0;
                    }
                    else if (_strnicmp(locals.sex_text, EQ_ARRAY(char, 0x005654A4), 1) == 0)
                    {
                        fr.sex = 1;
                    }
                }
                if (locals.material_text[0] != '\0')
                {
                    fr.material = atoi(locals.material_text);
                }
                if (locals.head_text[0] != '\0')
                {
                    fr.head = atoi(locals.head_text);
                }
                if (locals.face_text[0] != '\0')
                {
                    face = atoi(locals.face_text);
                }
                strcpy(locals.packet.target_name, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
                strcpy(locals.packet.player_name, g_local_player_guess->name);
                locals.packet.sex = fr.sex;
                locals.packet.material = (u16)fr.material;
                locals.packet.head = (u16)fr.head;
                locals.packet.face = face;
                locals.packet.race = (u16)race;
                locals.packet.zero = 0;
                send_message_guess(
                    &EQ_GLOBAL(void*, 0x006B3194), 0x2091, &locals.packet, 0x48, 1);
                sprintf(message, EQ_ARRAY(char, 0x00565488), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name,
                    EQ_METHOD(1, char*, 0x004C9105, (void*, int), pinstCEverQuest_guess)(race));
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
                if (locals.height_text[0] != '\0')
                {
                    EQ_CDECL(void, 0x004A5375, (EQPlayer*, char*))(
                        EQ_GLOBAL(EQPlayer*, 0x006E4DAC), locals.height_text);
                }
                if (locals.level_text[0] != '\0')
                {
                    fr.level_packet.level = atoi(locals.level_text);
                }
                else
                {
                    fr.level_packet.level = 0x3C;
                }
                EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->actor_info->unk_0x2E4 = fr.level_packet.level;
                fr.level_packet.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->spawn_id;
                sprintf(message, EQ_ARRAY(char, 0x00565440), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
                send_message_guess(
                    &EQ_GLOBAL(void*, 0x006B3194), 0x218C, &fr.level_packet, 8, 1);
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004aad6b
// Sets the player's stance from the command text.
void SetPlayerStance_guess(EQPlayer* player, char* command)
{
    int permission;
    int prefixComparison;
    int stance;

    permission = EQ_CDECL(int, 0x004A49BD, (void))();
    if (permission != 0)
    {
        prefixComparison = _strnicmp(
            command, EQ_ARRAY(char, 0x0055B254), 3);
        if (prefixComparison == 0)
        {
            stance = 100;
        }
        else
        {
            stance = 0x6E;
        }
        EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), player)(stance);
    }
    return;
}

#define SPELLBOOK_MAX_SLOT_INDEX 0x31

// FUNCTION: TRILOGY 0x004aad9b
// Selects a spellbook page and opens the spellbook interface.
void OpenSpellbookPage_guess(int unused, char* command)
{
    int permission;
    int spellbookPage;
    size_t commandLength;
    char spellbookMode;
    char* spellbookUi;

    permission = EQ_CDECL(int, 0x004A49BD, (void))();
    if (permission != 0)
    {
        permission = EQ_METHOD(0, int, 0x0041980E, (CharacterProfile*), g_pc_guess)();
        if (permission != 0)
        {
            commandLength = strlen(command);
            if (commandLength != 0)
            {
                spellbookPage = atoi(command);
                spellbookPage--;
                if ((spellbookPage > SPELLBOOK_MAX_SLOT_INDEX) || (spellbookPage < 0))
                    spellbookPage = 0;

                spellbookUi = g_eqmain_guess;
                *(short*)(spellbookUi + 0x6c560) = (short)(spellbookPage >> 1);
                if (spellbookUi[0xfe0] == 5)
                {
                    EQ_METHOD(1, void, 0x0046E4AA, (char*, int), spellbookUi)(
                        *(short*)(spellbookUi + 0x6c560));
                }
            }

            spellbookUi = g_eqmain_guess;
            spellbookMode = spellbookUi[0xfe0];
            if (spellbookMode != 5)
            {
                spellbookUi[0xfe2] = spellbookMode;
                if (g_game_state_guess == 2)
                {
                    EQ_GLOBAL(int, 0x00590170) = 2;
                    g_game_state_guess = 1;
                    EQ_METHOD(1, void, 0x0040E58B, (CDisplay*, int), g_cdisplay_guess)(0);
                    spellbookUi = g_eqmain_guess;
                    EQ_METHOD(0, void, 0x00465A60, (char*), spellbookUi)();
                    return;
                }

                spellbookUi = g_eqmain_guess;
                if (spellbookUi[0xfe0] != 4)
                {
                    *(int*)(spellbookUi + 0x6c554) = g_cdisplay_guess->time_ms_guess;
                    return;
                }

                EQ_METHOD(0, void, 0x00465A60, (char*), spellbookUi)();
                return;
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004aae78
// Casts the memorized spell in the numbered spell gem.
void CastMemorizedSpell_guess(int unused, char* spellText)
{
    int spellNumber;
    int spellSlot;
    int chatColor;
    int zero;
    short spellId;
    char castResult;
    char* chatText;
    char* spellUi;

    if (EQ_CDECL(int, 0x004A49BD, (void))() == 0)
        return;

    spellNumber = atoi(spellText);
    spellSlot = spellNumber - 1;
    if (spellSlot < 0 || spellSlot > 7)
    {
        chatColor = 0x111;
        chatText = EQ_ARRAY(char, 0x005655E4);
    }
    else
    {
        spellUi = g_eqmain_guess;
        zero = 0;
        if (*(int*)(spellUi + 0xE60 + spellSlot * 4) == zero
            && g_local_player_guess->actor_info->unknown0x248 == -1)
        {
            spellId = g_pc_guess->spell_memorized[spellSlot];
            if (spellId == -1)
                return;
            if (*(short*)(spellUi + 0xFE8) != zero)
                return;
            if (*(int*)(spellUi + 0x94) != zero)
                return;
            if (*(int*)(spellUi + 0xE5C) != zero)
                return;

            castResult = EQ_METHOD(4, char, 0x0041F381, (CharacterProfile*, int, short, int, int), g_pc_guess)(
                spellSlot, spellId, zero, -1);
            if (castResult == 0)
                return;

            spellNumber = 0xE60;
            do
            {
                if (*(int*)(g_eqmain_guess + spellNumber) == zero)
                    *(int*)(g_eqmain_guess + spellNumber) = 1;
                spellSlot = spellNumber + 4;
                *(int*)(g_eqmain_guess + spellSlot + 0x1C) = zero;
                spellNumber = spellSlot;
            } while (spellSlot < 0xE80);
            return;
        }

        if (EQ_CDECL(int, 0x0046A72F, (void))() != 0)
        {
            EQ_METHOD(1, void, 0x0042798F, (CharacterProfile*, int), g_pc_guess)(0);
            return;
        }
        chatColor = 0x108;
        chatText = EQ_ARRAY(char, 0x0055ABF0);
    }

    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(chatText, chatColor);
    return;
}

// FUNCTION: TRILOGY 0x004aaf76
// Activates the numbered ability or combat skill selected by /doability.
void UseAbilityByNumber_guess(int unused, char* abilityText)
{
    int permission;
    size_t textLength;
    int abilityIndex;
    short* abilityValue;

    permission = EQ_CDECL(int, 0x004A49BD, (void))();
    if (permission == 0)
        return;

    textLength = strlen(abilityText);
    if (textLength == 0)
        goto showUsage;

    abilityIndex = atoi(abilityText);
    abilityIndex = abilityIndex - 1;
    if (abilityIndex < 6)
    {
        abilityValue = (short*)(g_eqmain_guess + 0xFF4 + abilityIndex * 2);
    }
    else
    {
        if (abilityIndex >= 10)
            goto showUsage;
        abilityValue = (short*)(g_eqmain_guess + 0xFE0 + abilityIndex * 2);
    }

    if (*abilityValue != -1)
    {
        EQ_METHOD(2, void, 0x0041D790, (void*, int, EQPlayer*), EQ_GLOBAL(void*, 0x006E4DA8))
            ((int)(((unsigned int)abilityValue & 0xFFFFFF00) | (unsigned char)*abilityValue),
                EQ_GLOBAL(EQPlayer*, 0x006E4DAC));
        return;
    }
    return;

showUsage:
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
        EQ_ARRAY(char, 0x0056562C), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
        EQ_ARRAY(char, 0x005655F8), 0x111);
    return;
}

// FUNCTION: TRILOGY 0x004AB008
// Runs /who using the selected target's name.
void WhoTarget_guess(void* command)
{
    EQPlayer* target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
    if (target != NULL)
        EQ_CDECL(void, 0x004A5480, (void*, char*))(command, target->name);
}

// FUNCTION: TRILOGY 0x004ab01f
// The /camp command: camp to the desktop ("desk") or to server select ("serv"), then prepare to camp.
void Camp_guess(int unused, char* command)
{
    int comparison;

    if (EQ_CDECL(int, 0x004A49BD, (void))() != 0)
    {
        EQ_GLOBAL(int, 0x006E4E8C) = 0;
        comparison = _strnicmp(command, EQ_ARRAY(char, 0x00565650), 4);
        if (comparison == 0)
        {
            EQ_GLOBAL(int, 0x006E4E8C) = 1;
        }
        else
        {
            comparison = _strnicmp(command, EQ_ARRAY(char, 0x00565648), 4);
            if (comparison == 0)
                EQ_GLOBAL(int, 0x006E4E8C) = 2;
        }
        EQ_METHOD(0, void, 0x004CB72D, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    }
}

// FUNCTION: TRILOGY 0x004ab07f
// Handles group invitations according to the client's current invitation state.
void HandleInviteCommand_guess(void)
{
    if (EQ_CDECL(int, 0x004A49BD, (void))() == 0)
        return;

    if (pinstEQ_Main_guess->groupInvitePending_guess != 0)
        EQ_METHOD(0, void, 0x004CB362, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    else
        EQ_METHOD(0, void, 0x004CAF5E, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
}

// FUNCTION: TRILOGY 0x004ab0a7
// Runs a command action; its specific in-game effect is unclear from this wrapper.
void RunCommandAction_guess(void)
{
    if (EQ_CDECL(int, 0x004A49BD, (void))() != 0)
        EQ_METHOD(0, void, 0x004CB4FA, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
}

// FUNCTION: TRILOGY 0x004ab0bc
// Sets or toggles a command option; its in-game effect is unclear.
void SetCommandToggle_guess(int unused, char* command)
{
    int comparisonResult;

    comparisonResult = EQ_CDECL(int, 0x004A49BD, (void))();
    if (comparisonResult != 0)
    {
        comparisonResult = _strnicmp(command, (const char*)0x0055B254, 3);
        if (comparisonResult == 0)
        {
            EQ_GLOBAL(u8, 0x006B710D) = 0;
            EQ_GLOBAL(u8, 0x006B710C) = 0;
            return;
        }
        comparisonResult = _strnicmp(command, (const char*)0x0055B250, 2);
        if (comparisonResult == 0)
        {
            EQ_GLOBAL(u8, 0x006B710D) = 1;
            EQ_GLOBAL(u8, 0x006B710C) = 1;
            return;
        }
        EQ_GLOBAL(u8, 0x006B710D) = EQ_GLOBAL(u8, 0x006B710C) != 1;
        EQ_GLOBAL(u8, 0x006B710C) = EQ_GLOBAL(u8, 0x006B710D);
    }
}

// FUNCTION: TRILOGY 0x004ab124
// Lists the languages the character knows or selects the active language.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// @early-stop: Format loop tail: VC6 copy-propagates tmp onto EDI (cmp edi; mov eax,edi sinks past add ebx,4); original compares EAX
void do_language(int unused, char* cmdArgs)
{
    int displayNumber;
    int cmpResult;
    char* languageName;
    size_t argLength;
    int tmp;
    int* listPtr;
    CharacterProfile* pc;
    char chatLine[128];
    int knownLanguages[32];
    int local_8;

    local_8 = 0;
    pc = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
    if (pc != (CharacterProfile*)0)
    {
        listPtr = knownLanguages;
        for (tmp = 0x20; tmp != 0; tmp = tmp + -1)
        {
            *listPtr = -1;
            listPtr = listPtr + 1;
        }
        tmp = 0;
        listPtr = knownLanguages;
        do
        {
            if (*(&pc->languages[0] + tmp) > 0)
            {
                local_8 = local_8 + 1;
                *listPtr = tmp;
                listPtr = listPtr + 1;
            }
            tmp = tmp + 1;
        } while (tmp < 0x20);
    }
    cmpResult = _strcmpi(cmdArgs, EQ_ARRAY(char, 0x005656CC));
    if (cmpResult == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005656B8), 0x111);
        tmp = 0;
        if (0 < local_8)
        {
            listPtr = knownLanguages;
            do
            {
                displayNumber = tmp + 1;
                languageName = EQ_METHOD(1, char*, 0x004C8FCE, (void*, short), EQ_GLOBAL(void*, 0x006EFE2C))((short)*listPtr);
                sprintf(chatLine, EQ_ARRAY(char, 0x005656B0), displayNumber, languageName);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(chatLine, 0x111);
                tmp = displayNumber;
                listPtr = listPtr + 1;
            } while (tmp < local_8);
        }
    }
    else
    {
        argLength = strlen(cmdArgs);
        if (argLength > 0)
        {
            tmp = atoi(cmdArgs);
            tmp = tmp + -1;
            if ((tmp < 0) || (tmp >= local_8))
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565678), 0x111);
            else
                EQ_GLOBAL(short, 0x006B642A) = (short)tmp;
        }
    }
    tmp = EQ_GLOBAL(short, 0x006B642A);
    sprintf(chatLine, EQ_ARRAY(char, 0x00565658),
        EQ_METHOD(1, char*, 0x004C8FCE, (void*, short), EQ_GLOBAL(void*, 0x006EFE2C))((short)knownLanguages[tmp]),
        tmp + 1);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(chatLine, 0x111);
}

// FUNCTION: TRILOGY 0x004ab26d
// Assists the targeted or named player, with an option to turn on auto attack when assisting.
void Assist_guess(int unused, char* command)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(const char*, const char*, const char*, const char*);
    int comparison;
    size_t commandLength;
    EQPlayer* player;
    u32 spawnId;

    if ((EQ_CDECL(int, 0x004A49BD, (void))() != 0) &&
        (EQ_GLOBAL(int, 0x006E4DB8) == 0) &&
        (EQ_GLOBAL(int, 0x006E4DC4) == 0) &&
        (EQ_GLOBAL(int, 0x006E4DBC) == 0))
    {
        commandLength = strlen(command);
        if (commandLength == 0)
        {
            player = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
            if (player == 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    (EQ_ARRAY(char, 0x005656EC), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    (EQ_ARRAY(char, 0x005656D4), 0x111);
                return;
            }
        }
        else
        {
            comparison = _strcmpi(command, EQ_ARRAY(char, 0x0055B250));
            if (comparison == 0)
            {
                EQ_GLOBAL(int, 0x006E206C) = 1;
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))
                    (EQ_ARRAY(char, 0x00565768));
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056578C),
                    EQ_ARRAY(char, 0x00563F84), EQ_ARRAY(char, 0x0053DB50));
            }
            comparison = _strcmpi(command, EQ_ARRAY(char, 0x0055B254));
            if (comparison == 0)
            {
                EQ_GLOBAL(int, 0x006E206C) = 0;
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))
                    (EQ_ARRAY(char, 0x00565744));
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056578C),
                    EQ_ARRAY(char, 0x0053DAFC), EQ_ARRAY(char, 0x0053DB50));
            }
            EQ_METHOD(1, void, 0x004D061A, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(command);
            player = EQ_METHOD(1, EQPlayer*, 0x004B146A, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(command);
            if (player == 0)
                return;
        }
        spawnId = player->spawn_id;
        send_message_guess
            (g_world_connection_guess, 0x2200, &spawnId, 4, 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x004ab3b7
// The /target command: asks the server to target the spawn whose name matches or begins with the typed text.
void target_guess(int ignoredArg, char* targetName)
{
    int allowed;
    size_t nameLength;
    int local_8;
    EQPlayer* pPlayer;

    allowed = EQ_CDECL(int, 0x004A49BD, (void))();
    if (allowed != 0)
    {
        nameLength = strlen(targetName);
        if (nameLength > 0)
        {
            pPlayer = EQ_METHOD(1, EQPlayer*, 0x004B146A, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(targetName);
            if (pPlayer == 0)
                local_8 = 0;
            else
                local_8 = pPlayer->spawn_id;
            send_message_guess
                (g_world_connection_guess, 0x21fe, &local_8, 4, 1);
            return;
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056579C), 0x111);
    }
    return;
}

// FUNCTION: TRILOGY 0x004ab424
// Turns dynamic lighting on or off for the next zone.
void SetDynamicLights_guess(int unused, char* choice)
{
    int comparisonResult;
    const char* statusText;
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(
        const char*, const char*, const char*, const char*);

    comparisonResult = _strnicmp(choice, EQ_ARRAY(char, 0x0055B254), 3);
    if (comparisonResult == 0)
    {
        EQ_GLOBAL(int, 0x006E20B8) &= comparisonResult;
        EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
            EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00565820),
            EQ_ARRAY(char, 0x0053DAFC), EQ_ARRAY(char, 0x0053DB50));
        statusText = EQ_ARRAY(char, 0x005657E8);
    }
    else
    {
        comparisonResult = _strnicmp(choice, EQ_ARRAY(char, 0x0055B250), 2);
        if (comparisonResult == 0)
        {
            EQ_GLOBAL(int, 0x006E20B8) = 1;
        }
        else
        {
            EQ_GLOBAL(int, 0x006E20B8) = (EQ_GLOBAL(int, 0x006E20B8) != 1);
            if (EQ_GLOBAL(int, 0x006E20B8) != 1)
            {
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00565820),
                    EQ_ARRAY(char, 0x0053DAFC), EQ_ARRAY(char, 0x0053DB50));
                statusText = EQ_ARRAY(char, 0x005657E8);
            }
            else
            {
                EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                    EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00565820),
                    EQ_ARRAY(char, 0x00563F84), EQ_ARRAY(char, 0x0053DB50));
                statusText = EQ_ARRAY(char, 0x005657B4);
            }
        }
        if (comparisonResult == 0)
        {
            EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00565820),
                EQ_ARRAY(char, 0x00563F84), EQ_ARRAY(char, 0x0053DB50));
            statusText = EQ_ARRAY(char, 0x005657B4);
        }
    }
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
        (statusText, 0x111);
}

// FUNCTION: TRILOGY 0x004ab4d0
// Sets the chat text size from the command and saves the preference.
void SetChatFontSize_guess(int ignoredParameter, char* sizeArgument)
{
    size_t argumentLength;
    char formattedSize[16];
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(
        const char*, const char*, const char*, const char*);

    argumentLength = strlen(sizeArgument);
    if (argumentLength > 0)
    {
        EQ_GLOBAL(int, 0x006E20BC) = atoi(sizeArgument);
        EQ_METHOD(0, void, 0x0048CF56, (void*), EQ_GLOBAL(void* volatile, 0x006E4DE8))();
        sprintf(formattedSize, EQ_ARRAY(char, 0x0053E16C), EQ_GLOBAL(int, 0x006E20BC));
        EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
            EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00565860), formattedSize,
            EQ_ARRAY(char, 0x0053DB50));
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565834), 0x111);
    return;
}

// FUNCTION: TRILOGY 0x004ab545
// Turns inventory window labels on or off and saves the preference.
void SetInventoryWindowLabels_guess(int unused, char* setting)
{
    int comparison;
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(
        const char*, const char*, const char*, const char*);

    comparison = _strnicmp(
        setting, EQ_ARRAY(char, 0x0055B254), 3);
    if (comparison == 0)
    {
        EQ_GLOBAL(int, 0x006E2074) = 0;
    showDisabled:
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(char, 0x005658A0), 0x111);
        EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
            EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00565890), EQ_ARRAY(char, 0x0053DAFC),
            EQ_ARRAY(char, 0x0053DB50));
        return;
    }

    comparison = _strnicmp(
        setting, EQ_ARRAY(char, 0x0055B250), 2);
    if (comparison == 0)
    {
        EQ_GLOBAL(int, 0x006E2074) = 1;
    }
    else
    {
        EQ_GLOBAL(int, 0x006E2074) = (EQ_GLOBAL(int, 0x006E2074) != 1);
        if (EQ_GLOBAL(int, 0x006E2074) != 1)
            goto showDisabled;
    }

    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
        EQ_ARRAY(char, 0x00565870), 0x111);
    EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
        EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x00565890), EQ_ARRAY(char, 0x00563F84),
        EQ_ARRAY(char, 0x0053DB50));
    return;
}

// FUNCTION: TRILOGY 0x004ab5ec
// Saves a UI window's red, green, blue, and transparency settings.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// __cdecl. Writes "<prefix>WinRed/WinGreen/WinBlue/WinTrans" under [WindowColors] in eqclient.ini through
// EQ_Overlay::WritePrivateProfileWindowValue on the main UI object; the prefix is copied unbounded
// into 128 bytes. Always returns 1.
int WriteWindowColors(char* keyPrefix, int red, int green, int blue, int trans)
{
    char key[128];

    strcpy(key, keyPrefix);
    strcat(key, EQ_ARRAY(char, 0x005658E0));
    EQ_METHOD(3, void, 0x0045056D, (void*, const char*, const char*, int), (void*)g_eqmain_guess)(EQ_ARRAY(char, 0x0055A6B4), key, red);
    strcpy(key, keyPrefix);
    strcat(key, EQ_ARRAY(char, 0x005658D4));
    EQ_METHOD(3, void, 0x0045056D, (void*, const char*, const char*, int), (void*)g_eqmain_guess)(EQ_ARRAY(char, 0x0055A6B4), key, green);
    strcpy(key, keyPrefix);
    strcat(key, EQ_ARRAY(char, 0x005658CC));
    EQ_METHOD(3, void, 0x0045056D, (void*, const char*, const char*, int), (void*)g_eqmain_guess)(EQ_ARRAY(char, 0x0055A6B4), key, blue);
    strcpy(key, keyPrefix);
    strcat(key, EQ_ARRAY(char, 0x005658C0));
    EQ_METHOD(3, void, 0x0045056D, (void*, const char*, const char*, int), (void*)g_eqmain_guess)(EQ_ARRAY(char, 0x0055A6B4), key, trans);
    return 1;
}

// FUNCTION: TRILOGY 0x004ab6be
// Changes or resets the color and transparency of named UI windows.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// /wincolor <window name> <red> <green> <blue> <trans>, or "<window name> reset". "all" sets every window. Each
// match stores the packed colour (trans<<24 | red<<16 | green<<8 | blue) in the UI colour table (the object at
// 0x006E4E44) and writes it to eqclient.ini through WriteWindowColors. Bad input prints the usage lines.
void do_wincolor(int unused, char* args)
{
    int blue;
    int green;
    int result = 0;
    int flag = 0;
    char word[64];
    char word2[64];
    char word3[64];
    char word4[64];
    char word5[64];
    int red;
    int trans = -1;
    unsigned int packed;
    char* rest;

    blue = green = -1;
    if (g_pc_guess == 0)
        return;
    if (EQ_METHOD(0, int, 0x004197B2, (void*), (void*)g_pc_guess)() == 0)
        return;
    if (strlen(args) > 0)
    {
        rest = EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(args, word, 0x40);
        _strlwr(word);
        rest = EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rest, word2, 0x40);
        rest = EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rest, word3, 0x40);
        rest = EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rest, word4, 0x40);
        EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rest, word5, 0x40);
        if (_strnicmp(word2, EQ_ARRAY(char, 0x00565ADC), 5) == 0)
        {
            red = blue = green = 1;
            trans = 2;
        }
        else
        {
            if (is_number(word2) == 0)
                goto usage;
            red = atoi(word2);
            if (is_number(word3) != 0)
                green = atoi(word3);
            if (is_number(word4) != 0)
                blue = atoi(word4);
            if (is_number(word5) != 0)
                trans = atoi(word5);
            if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255 || trans < 0 || trans > 4)
                goto usage;
        }
        packed = ((((trans << 8) | red) << 8 | green) << 8) | blue;
        if (_strnicmp(word, EQ_ARRAY(char, 0x00565AD8), 3) == 0)
            flag = 1;
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565AD0), 4) == 0)
        {
            pinstEQ_Main_guess->chatWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565AC8));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565AC0), 5) == 0)
        {
            pinstEQ_Main_guess->spellsWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565AB8));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565AB0), 6) == 0)
        {
            pinstEQ_Main_guess->playerWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565AA8));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565AA0), 5) == 0)
        {
            pinstEQ_Main_guess->partyWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A98));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00563A40), 5) == 0)
        {
            pinstEQ_Main_guess->targetWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A90));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565A88), 4) == 0)
        {
            pinstEQ_Main_guess->affectWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A80));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565A78), 6) == 0)
        {
            pinstEQ_Main_guess->hotButtonWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A6C));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565A64), 4) == 0)
        {
            pinstEQ_Main_guess->itemDataWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A58));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565A50), 4) == 0)
        {
            pinstEQ_Main_guess->mainMenuWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A44));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565A40), 3) == 0)
        {
            pinstEQ_Main_guess->inventoryWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A34));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (flag != 0 || _strnicmp(word, EQ_ARRAY(char, 0x00565A2C), 5) == 0)
        {
            pinstEQ_Main_guess->trackWindowColor_guess = packed;
            strcpy(word, EQ_ARRAY(char, 0x00565A24));
            result = WriteWindowColors(word, red, green, blue, trans);
        }
        if (result != 0)
            return;
    }
usage:
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005659E4), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565984), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565960), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565924), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005658E8), 0x111);
}

// FUNCTION: TRILOGY 0x004abc57
// Changes a numbered custom chat text color or shows the color category lists.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// @early-stop: EQ_THISCALL dummy-EDX setup and INI write stack/register scheduling still differ.
// /usercolor <number> <red> <green> <blue>, "<number> reset", or "help"/"page1".."page4" for the lists of colour
// numbers. A valid colour is set in the display and written to eqclient.ini ([TextColors] User_<n>_Red/Green/Blue).
// Bad input prints the format lines.
void do_usercolor(int unused, char* args)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(
        const char*, const char*, const char*, const char*);
    int number = -1;
    int red = -1;
    int green = -1;
    int blue = -1;
    char word[64];
    char word2[64];
    char word3[64];
    char word4[64];
    char name[16];
    char key[32];
    char value[16];
    char* rest;

    if (strlen(args) > 0)
    {
        rest = EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(args, word, 0x40);
        rest = EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rest, word2, 0x40);
        rest = EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rest, word3, 0x40);
        EQ_METHOD(3, char*, 0x004DBCE1, (void*, char*, char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(rest, word4, 0x40);
        if (is_number(word) != 0)
            number = atoi(word);
        if (_strcmpi(word2, EQ_ARRAY(char, 0x00565ADC)) == 0)
        {
            EQ_CDECL(void, 0x0040BB6C, (int, int*, int*, int*))(number - 1, &red, &green, &blue);
        }
        else
        {
            if (_strcmpi(word, EQ_ARRAY(char, 0x005656CC)) == 0 ||
                _strcmpi(word, EQ_ARRAY(char, 0x00565FC4)) == 0)
                /* fake match: retain the shared page-list block after color parsing. */
                goto showFirstColorPage;
            if (_strcmpi(word, EQ_ARRAY(char, 0x00565FBC)) == 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565FA4), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565F8C), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565F74), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565F54), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565F28), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565F00), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565EE4), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565EBC), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565EA0), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565E84), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565E58), 0x111);
                return;
            }
            if (_strcmpi(word, EQ_ARRAY(char, 0x00565E50)) == 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565E38), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565E1C), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565E00), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565DE4), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565DCC), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565DB8), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565DA4), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565D8C), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565D74), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565D60), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565D30), 0x111);
                return;
            }
            if (_strcmpi(word, EQ_ARRAY(char, 0x00565D28)) == 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565D14), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565CF8), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565CE4), 0x111);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C94), 0x111);
                return;
            }
            if (is_number(word2) != 0)
            {
                red = atoi(word2);
                if (is_number(word3) != 0)
                    green = atoi(word3);
                if (is_number(word4) != 0)
                    blue = atoi(word4);
            }
        }
        if (red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255 &&
            number >= 0 && number <= 34)
        {
            EQ_CDECL(void, 0x0040BC8C, (int, int, int, int))(number - 1, red, green, blue);
            sprintf(name,
                EQ_ARRAY(char, 0x0053E174), number);
            strcpy(key, name);
            strcat(key, EQ_ARRAY(char, 0x0053E170));
            sprintf(value, EQ_ARRAY(char, 0x0053E16C), red);
            EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                EQ_ARRAY(char, 0x0053E160), key, value, EQ_ARRAY(char, 0x0053DB50));
            strcpy(key, name);
            strcat(key, EQ_ARRAY(char, 0x0053E158));
            sprintf(value, EQ_ARRAY(char, 0x0053E16C), green);
            EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                EQ_ARRAY(char, 0x0053E160), key, value, EQ_ARRAY(char, 0x0053DB50));
            strcpy(key, name);
            strcat(key, EQ_ARRAY(char, 0x0053E150));
            sprintf(value, EQ_ARRAY(char, 0x0053E16C), blue);
            EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
                EQ_ARRAY(char, 0x0053E160), key, value, EQ_ARRAY(char, 0x0053DB50));
            return;
        }
        /* fake match: route all invalid input through the original shared format block. */
        goto showUsage;
showFirstColorPage:
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C6C), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C60), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C54), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C48), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C3C), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C30), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C20), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C14), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565C08), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565BD8), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565BC4), 0x111);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565B98), 0x111);
        return;
    }
showUsage:
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565B64), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565B50), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565960), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565B1C), 0x111);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565AE4), 0x111);
}

// FUNCTION: TRILOGY 0x004ac22e
// Turns server-side filtering of spell and combat messages on or off.
void SetServerFilter_guess(int unused, char* command)
{
    typedef int (__stdcall *tWritePrivateProfileStringA_guess)(
        const char*, const char*, const char*, const char*);
    const char* filterValue;

    if (_strnicmp(
            command, EQ_ARRAY(char, 0x0055B254), 3) == 0)
    {
        EQ_GLOBAL(int, 0x006B6424) = 0;
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int),
            EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056611C), 0x111);
        filterValue = EQ_ARRAY(char, 0x0053D860);
    }
    else
    {
        if (_strnicmp(
                command, EQ_ARRAY(char, 0x0055B250), 2) != 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*),
                EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x005660C8));
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int),
                EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00565FCC), 0x0d);
            return;
        }
        EQ_GLOBAL(int, 0x006B6424) = 1;
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int),
            EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x005660E8), 0x111);
        filterValue = EQ_ARRAY(char, 0x005644D4);
    }

    EQ_GLOBAL(tWritePrivateProfileStringA_guess, 0x00534188)(
        EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0056610C), filterValue,
        EQ_ARRAY(char, 0x0053DB50));
    EQ_METHOD(0, void, 0x004CF829, (CEverQuest*),
        EQ_GLOBAL(CEverQuest*, 0x006EFE2C))();
}

// FUNCTION: TRILOGY 0x004ac2ea
// Locks the interface with a password, or checks the password to unlock it.
void SetSafeLock_guess(int unusedCommandId, char* password)
{
    size_t passwordLength;
    int compareResult;
    const char* response;

    if (*password == '\0')
    {
        if (EQ_GLOBAL(char, 0x006E20F4) == '\0')
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*),
                EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056634C));
            response = EQ_ARRAY(char, 0x0056621C);
        }
        else
        {
            response = EQ_ARRAY(char, 0x005645CC);
        }
    }
    else if (EQ_GLOBAL(char, 0x006E20F4) == '\0')
    {
        passwordLength = strlen(password);
        if (passwordLength < 0x20)
        {
            strcpy(
                &EQ_GLOBAL(char, 0x006E20F4), password);
            response = EQ_ARRAY(char, 0x00566184);
        }
        else
        {
            response = EQ_ARRAY(char, 0x00566140);
        }
    }
    else
    {
        compareResult = _strcmpi(
            password, &EQ_GLOBAL(char, 0x006E20F4));
        if (compareResult == 0)
        {
            EQ_GLOBAL(char, 0x006E20F4) = '\0';
            response = EQ_ARRAY(char, 0x00566208);
        }
        else
        {
            response = EQ_ARRAY(char, 0x005661F4);
        }
    }

    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*),
        EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(response);
    return;
}

// FUNCTION: TRILOGY 0x004ac37b
// Runs an MCI command and displays its returned text or error in chat.
void mcicontrol_guess(int unused, char* command)
{
    typedef unsigned int (__stdcall *mciSendStringA_guess_t)(const char*, char*, unsigned int, void*);
    typedef int (__stdcall *mciGetErrorStringA_guess_t)(unsigned int, char*, unsigned int);
    unsigned int mciError;
    char message[512];
    char result[256];

    if (g_pc_guess != 0)
    {
        if (EQ_CDECL(int, 0x004A31A5, (CharacterProfile*))(g_pc_guess) != 0)
        {
            strcpy(result, EQ_ARRAY(char, 0x0053E658));
            if (strlen(command) > 0)
            {
                mciError = EQ_GLOBAL(mciSendStringA_guess_t, 0x00534274)
                    (command, result, 0x100, 0);
                if ((result[0] != '*') && (result[0] != '\0'))
                {
                    strcpy(message, EQ_ARRAY(char, 0x00566378));
                    strcat(message, result);
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void*, 0x006EFE2C))(message, 0x111);
                }
                if (mciError != 0)
                {
                    strcpy(result, EQ_ARRAY(char, 0x0053E658));
                    EQ_GLOBAL(mciGetErrorStringA_guess_t, 0x00534268)
                        (mciError, result, 0x100);
                    if ((result[0] != '*') && (result[0] != '\0'))
                    {
                        strcpy(message, EQ_ARRAY(char, 0x0056636C));
                        strcat(message, result);
                        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void*, 0x006EFE2C))(message, 0x111);
                    }
                }
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004ac4a9
// Turns player character name labels on or off.
// ORIGINAL FILE: unknown (EQMisc placement inferred from mapped neighbours)
// __cdecl command handler: a parsed word beginning with `off` hides character-name sprites; everything
// else enables them.
void cmd_shownames_guess(int unused, char* args)
{
    char command[32];

    EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, command);
    if (_strnicmp(command, EQ_ARRAY(char, 0x0055B254), 3) == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00566394), 0x111);
        EQ_METHOD(0, int, 0x004121CA, (CDisplay*), (CDisplay*)g_display_guess)();
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00566384), 0x111);
    EQ_METHOD(0, int, 0x00412194, (CDisplay*), (CDisplay*)g_display_guess)();
}

// FUNCTION: TRILOGY 0x004ac517
// Changes NPC name visibility; the sprite state is opposite to the ON/OFF message.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// [Mac, medium] TAKP's command table identifies /shownpcnames. The binary reports the parsed `off` state
// but calls the NPC sprite routines in the opposite direction; preserve that behavior.
void do_shownpcnames(int unused, char* args)
{
    char command[32];

    EQ_METHOD(2, char *, 0x004DBC84, (CEverQuest *, char *, char *), pinstCEverQuest_guess)(args, command);
    if (_strnicmp(command, EQ_ARRAY(char, 0x0055B254), 3) == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005663B8), 0x111);
        EQ_METHOD(0, int, 0x00412233, (CDisplay*), (CDisplay*)g_display_guess)();
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005663A4), 0x111);
    EQ_METHOD(0, int, 0x004121FD, (CDisplay*), (CDisplay*)g_display_guess)();
}

// FUNCTION: TRILOGY 0x004ac585
// Enables or disables dynamic level of detail for the game scene.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
// [Mac, low] Reports the requested level-of-detail state, then forwards its boolean value and the two
// client tuning floats to the s3dSetDynamicLOD entry point.
void setLOD_guess(int enabled)
{
    typedef void (__cdecl *tS3DSetDynamicLOD_guess)(int, float, float);

    if (enabled != 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005663E4), 0x111);
        EQ_GLOBAL(tS3DSetDynamicLOD_guess, 0x006E5138)(1, 1.0f, EQ_GLOBAL(float, 0x0053515C));
    }
    else
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005663CC), 0x111);
        EQ_GLOBAL(tS3DSetDynamicLOD_guess, 0x006E5138)(0, 1.0f, EQ_GLOBAL(float, 0x0053515C));
    }
}

struct EQPlayer_build_token_string_guess_class : public EQPlayer
{
    void SetHeight_guess(float param_2);
};

// FUNCTION: TRILOGY 0x004b4836
// Applies a requested size change to a character or NPC model.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
void EQPlayer_build_token_string_guess_class::SetHeight_guess(float requestedHeight)
{
    this->size = requestedHeight;
    EQ_METHOD(0, void, 0x004BC9BA, (EQPlayer*), this)();
    EQActorInfo_guess* temp = this->actor_info;
    CDisplay* temp2 = g_cdisplay_guess;
    EQ_METHOD(3, void, 0x0040C113, (CDisplay*, void*, float, u8), temp2)(*(void**)temp, temp->actorScale_guess, 1);
}

// FUNCTION: TRILOGY 0x004bcde8
// Constructs the named actor record used by players, missiles and the camera, with its default values.
// __thiscall shape: this in ECX, the name pointer on the stack, callee pops 4. Zeroes a 0x364-byte object
// (memset), copies the name to +0x08, then sets the default fields: the EqGetTime() stamps at +0x54..+0x60,
// +0x74 and +0x19C, the time() stamp at +0xC4, the float defaults, the 0x22 dwords at +0xD4 and the globals
// at 0x006B6410/0x006B640C into +0xA4/+0xA0. Returns this.
void* __fastcall EQObject_ctor_guess(EQActorInfo_guess* self, int edx_unused, const char* objectName)
{
    int i;
    u32 now;
    u32* p;

    memset(self, 0, 0x364);
    self->floor_z = 0.0f;
    self->fallStartZ_guess = 0.0f;
    self->savedZ_bits_guess = 0xEC4ECB8F;
    *(u32*)((char*)self + 0x2E4) = 0;
    now = EQ_CDECL(u32, 0x004A461E, (void))();
    self->timeStamp60_guess = now;
    self->timeStamp5C_guess = now;
    self->timeStamp58_guess = now;
    self->lastPhysicsTime_guess = now;
    self->lastSaveTime_guess = now;
    strcpy((char*)self + 8, objectName);
    self->environment_guess = 0;
    self->unknown0x099 = 0;
    self->unk_0x9A = 0;
    self->unk_0x8C = 0;
    self->unk_0x90 = 0;
    self->unk_0x94 = 0;
    self->unk_0x89 = 0;
    self->unk_0x2D4 = 0.0f;
    self->unk_0x2FC = 0;
    self->unk_0x2D8 = 0;
    *(u32*)((char*)self + 0x2F4) = 0;
    self->unk_0x2F8 = 0;
    *(time_t*)((char*)self + 0xC4) = EQ_CDECL(time_t, 0x005213E9, (time_t*))(0);
    self->unk_0x2E8 = 0;
    *(u32*)((char*)self + 0x68) = 0;
    self->unk_0x64 = 0;
    // -1 sentinels are signed stores: VC6 keeps the -1 in a register for these, not as 'or' immediates.
    self->unknown0x078 = -1;
    self->unk_0x304 = 0;
    self->unk_0x7A = -1;
    self->unk_0x80_bits_guess = 0x3EB33333;
    *(u16*)((char*)self + 0x7E) = 0x20;
    self->unk_0x84 = 0x3DCCCCCD;
    self->unk_0x7C = 0x10;
    self->unk_0xA8 = 0;
    self->unk_0x9C = 0;
    self->unk_0x186 = 0;
    self->unk_0x2C8 = 0;
    self->unk_0x2CC = 0;
    self->unk_0x2D0 = 0;
    *(u32*)((char*)self + 0x18C) = 0;
    self->sound_id_guess = 0;
    self->unk_0x1E0 = 0;
    self->unk_0x1E4 = 0;
    self->unk_0x1E8 = 0;
    self->unk_0x1EC = 0;
    self->unk_0x1F0 = 0;
    self->unk_0x1F4 = 0;
    self->unk_0x1F8 = 0;
    self->unk_0x1FC = 0;
    self->unk_0x200 = 0;
    self->unk_0x204 = 0;
    *(u32*)((char*)self + 0x208) = 0;
    *(u32*)((char*)self + 0x218) = 0;
    self->unk_0x21C = 0;
    self->unk_0x220 = 0;
    *(u32*)((char*)self + 0x20C) = 0;
    self->unk_0x210 = 0;
    self->unk_0x214 = 0;
    self->unk_0x224 = 0;
    self->sound_data_a_guess = 0;
    self->sound_data_b_guess = 0;
    self->unk_0x230 = 0;
    now = EQ_CDECL(u32, 0x004A461E, (void))();
    self->lastActiveTime_guess = now;
    self->unk_0x1A0_bits_guess = 0x43480000;
    self->actorScale_guess = 1.0f;
    *(u8*)((char*)self + 0x1BC) = 0;
    *(u32*)((char*)self + 0x198) = 0x20;
    self->unk_0xB8 = 0.0f;
    *(u32*)((char*)self + 0x244) = 0;
    self->sneak_mod_bits_guess = 0x3F000000;
    self->unk_0xBC = 0.0f;
    self->unk_0x1A8 = 0;
    self->unk_0x1AC = 0;
    self->unk_0x1B0 = 0;
    self->unk_0x1B4 = -1;
    *(u32*)((char*)self + 0x234) = 0;
    *(u32*)((char*)self + 0x238) = 0;
    self->unk_0x23C = 0;
    self->unk_0x240 = 0;
    self->unk_0x1B8 = 0;
    self->unk_0x185 = 0;
    self->unk_0xAC_bits_guess = 0x3FCCCCCD;
    self->unk_0xB0_bits_guess = 0x404CCCCD;
    self->stunTimer_guess = 0;
    // Raw VAs: the EQD_MATCH build has no symbol for these unnamed DAT_ globals (EQ_GLOBAL would not link there).
    self->horizontalDrag_bits_guess = *(u32*)0x006B6410;
    self->horizontalFriction_bits_guess = *(u32*)0x006B640C;
    self->unk_0xCC = 0;
    self->unk_0xD0 = 0;
    p = &self->unk_0xD4[0];
    for (i = 0x22; i != 0; i--)
    {
        *p = 0;
        p++;
    }
    *(u32*)((char*)self + 0x15C) = 0;
    *(u32*)((char*)self + 0x2EC) = 0;
    self->castItemSlot_guess = -1;
    self->lastAttackTime_guess = 0;
    self->unk_0x164 = 0;
    self->unk_0xC8 = 0;
    self->unknown0x248 = -1;
    self->unk_0x24A = 0;
    self->unk_0x24C = 0x0A;
    self->lastSwitchTouchTime_guess = 0;
    self->unk_0x16C = 0;
    self->unk_0x174 = 0;
    self->unk_0x170 = 0;
    self->unk_0x187 = 0;
    self->unk_0x188 = 0;
    self->unknown0x189 = 0;
    self->unk_0x18A = 0;
    self->unk_0x17C = 0;
    self->unk_0x2C4 = 0;
    self->unk_0x6C = 0;
    self->unk_0x70 = 0;
    *(u32*)((char*)self + 0x180) = 0xFE;
    self->unk_0x184 = 0;
    *(u32*)((char*)self + 0x178) = 0;
    *(u32*)((char*)self + 0x250) = 0;
    *(u32*)((char*)self + 0x254) = 0;
    *(u32*)((char*)self + 0x258) = 0;
    *(u32*)((char*)self + 0x25C) = 0;
    *(u32*)((char*)self + 0x260) = 0;
    *(u32*)((char*)self + 0x264) = 0;
    *(u32*)((char*)self + 0x26C) = 0;
    *(u32*)((char*)self + 0x268) = 0;
    *(u32*)((char*)self + 0x270) = 0;
    *(u32*)((char*)self + 0x274) = 0;
    *(u32*)((char*)self + 0x278) = 0;
    *(u32*)((char*)self + 0x27C) = 0;
    *(u32*)((char*)self + 0x280) = 0;
    *(u32*)((char*)self + 0x284) = 0;
    *(u32*)((char*)self + 0x288) = 0;
    *(u32*)((char*)self + 0x28C) = 0;
    *(u32*)((char*)self + 0x290) = 0;
    *(u32*)((char*)self + 0x294) = 0;
    self->left_eye_material_guess = -1;
    self->right_eye_material_guess = -1;
    self->unk_0x2C1 = 0;
    self->unk_0x2A0 = 0;
    *(u32*)((char*)self + 0x2A4) = 0;
    *(u32*)self = 0;
    *(u32*)((char*)self + 4) = 0;
    self->unk_0x2C0 = 0;
    self->unk_0x2BC = 0.0f;
    self->unknown0x088 = 1;
    *(u32*)((char*)self + 0x300) = 0;
    return self;
}

// FUNCTION: TRILOGY 0x004bd3bd
// Checks whether the local player is close enough to hear this positional sound emitter.
// ORIGINAL FILE: unknown (queue subsystem from the call graph)
// __thiscall. Whether the local player is within the emitter's radius, measured as |dz| + the larger of
// |dy|, |dx| + half the smaller (a cheap distance estimate).
bool EQSoundEmitter_guess::IsLocalPlayerInRange_guess()
{
    struct EQPlayer *temp = g_local_player_guess;
    float dy = y - temp->y;
    float dx = x - temp->x;
    float dz;
    float absDz;

    if (dy < 0.0f)
        dy = -dy;
    if (dx < 0.0f)
        dx = -dx;
    if (dy > dx)
        dx = dx * 0.5f;
    else
        dy = dy * 0.5f;
    dz = z - temp->z;
    if (dz < 0.0f)
        absDz = -dz;
    else
        absDz = dz;
    return (absDz + dx + dy <= radius) ? true : false;
}

// FUNCTION: TRILOGY 0x004c535d
// Marks inventory-related windows for refresh after item changes.
// ORIGINAL FILE: unknown (queue subsystem from the call graph)
// Called with ECX = the CEverQuest object (from AutoInventory_guess), which it ignores; no stack arguments.
// When the main UI object (0x006E4E44) exists and the object at 0x006E4E60 has no +0x10 flag, sets five
// window refresh flags (+0x8FB94 .. +0x8FBA4) and calls the UI object's virtual methods 2 and 1.
void MarkInventoryWindowsDirty_guess(void)
{
    if (g_eqmain_guess != NULL && *(int*)(g_eqBook_guess + 0x10) == 0)
    {
        pinstEQ_Main_guess->showEquipment_guess = 1;
        pinstEQ_Main_guess->equipViewFlag0_guess = 1;
        pinstEQ_Main_guess->equipViewFlag2_guess = 1;
        pinstEQ_Main_guess->equipViewFlag3_guess = 1;
        pinstEQ_Main_guess->equipViewFlag1_guess = 1;
        ((EQMainVtbl_guess*)g_eqmain_guess)->vfunc2();
        ((EQMainVtbl_guess*)g_eqmain_guess)->vfunc1();
    }
}

// FUNCTION: TRILOGY 0x004cc908
// Sends the requested chat-channel state to the world server.
void __fastcall SendChannelToggle_guess(CEverQuest* pThis, int unused, u8 channelState)
{
    u8 toggleByte;

    toggleByte = channelState;
    send_message_guess(
        g_world_connection_guess, 0x2070, &toggleByte, 1, 1);
    return;
}

// FUNCTION: TRILOGY 0x004dd038
// Shows the current in-game time and Earth time in chat.
// ORIGINAL FILE: EverQuest/EQMisc.cpp (from Mac debug map)
void __fastcall ShowGameAndEarthTime_guess(CEverQuest* pThis, int unused)
{
    char chatMessage[300];
    time_t local_8;
    size_t messageLength;

    EQ_METHOD(1, void, 0x004BE864, (void*, char*), EQ_GLOBAL(void*, 0x006E4D54))(chatMessage);
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pThis)(chatMessage);
    local_8 = EQ_CDECL(time_t, 0x005213E9, (time_t*))(0);
    sprintf(chatMessage, EQ_ARRAY(char, 0x005708BC), EQ_CDECL(char*, 0x005213D0, (const time_t*))(&local_8));
    messageLength = strlen(chatMessage);
    chatMessage[messageLength] = '\0';
    chatMessage[messageLength - 1] = '\0';
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pThis)(chatMessage);
}

// FUNCTION: TRILOGY 0x004e2859
// ORIGINAL FILE: unknown (queue subsystem from the call graph)
// __cdecl. 1 at night: the world clock's hour is not 5..18.
int IsNightTime_guess(void)
{
    if (g_pWorldTime_guess->hour > 4 && g_pWorldTime_guess->hour < 19)
        return 0;
    return 1;
}

// kernel32 OutputDebugStringA without <windows.h> (the import resolves to kernel32.dll on both sides).
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char* text);

// The sound control object (g_sound_control_guess, soundmanager.h) with the virtual slots the emitter update calls
// typed; soundmanager.h leaves +0x18 and +0x20 without arguments.
class EqSoundControlEmitter_guess
{
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int  Acquire_guess(int sound_id);                   /* +0x18 handle of a sound, -1 = failed */
    virtual int  Release_guess(s32* handle);                    /* +0x1C */
    virtual void Play_guess(int sound_id, void* buffer);        /* +0x20 */
    virtual int  Command_guess(void* handle, void* buffer);     /* +0x24 */
    virtual void vf28();
    virtual int  Stop_guess(s32* handle);                       /* +0x2C */
};
#define g_sound_control_emitter_guess   EQ_GLOBAL(EqSoundControlEmitter_guess *, 0x006E4DD0)

// The sound buffer the emitter update fills in on its stack (EQSoundBuffer_guess layout: vtable 0x00535798, the 3D
// position at 0x04 as y, z, x of the emitter, the changed flags at 0x54). It has an EH lifetime but is never
// destroyed on the normal path, so its destructor is empty.
class EQSoundBufferEmitter_guess
{
public:
    void*   vtable;             /* 0x00 */
    float   pos_y;              /* 0x04 */
    float   pos_z;              /* 0x08 */
    float   pos_x;              /* 0x0C */
    u8      unknown0x10[0x44];  /* 0x10 */
    u32     changed_flags;      /* 0x54 */

    EQSoundBufferEmitter_guess(void) { vtable = (void*)0x00535798; changed_flags = 0; }
    ~EQSoundBufferEmitter_guess(void) {}
    // Inline in the original: the position in DirectSound's axes (EQ y, z, x) and the changed bit.
    void SetFloatPair_guess(float x, float y, float z) { pos_y = y; pos_z = z; pos_x = x; changed_flags |= 1; }
};
C_ASSERT(sizeof(EQSoundBufferEmitter_guess) == 0x58);
C_ASSERT(offsetof(EQSoundBufferEmitter_guess, changed_flags) == 0x54);
// EQSoundBuffer_guess setters (SoundManager.cpp / this file), __thiscall through their VA.
#define EmitterBuffer_SetMode_guess(b, mode) \
    EQ_METHOD(1, void, 0x004ECBC2, (EQSoundBufferEmitter_guess *, u8), (b))((mode))
#define EmitterBuffer_SetVolume_guess(b, vol) \
    EQ_METHOD(1, bool, 0x004ECBA1, (EQSoundBufferEmitter_guess *, int), (b))((vol))
#define EmitterBuffer_SetMinDistance_guess(b, distance) \
    EQ_METHOD(1, bool, 0x004ECBFA, (EQSoundBufferEmitter_guess *, float), (b))((distance))
// EQSoundEmitter_guess::IsLocalPlayerInRange_guess (sound.h), __thiscall through its VA.
#define EmitterIsLocalPlayerInRange_guess(e) \
    EQ_METHOD(0, bool, 0x004BD3BD, (EQSoundEmitter_guess *), (e))()

// The sound manager with the emitter update added (soundmanager.h has no member for it and types 0x0D8, the emitter
// being processed, as a plain dword).
class EqSoundManagerEmitters_guess : public EqSoundManager_guess
{
public:
    void UpdateEmitters_guess(void);
};
// The emitter being processed, which the original re-reads from the manager at every use.
#define EMITTER_guess ((EQSoundEmitter_guess*)unknown0x0D8)

// FUNCTION: TRILOGY 0x004e2d75
// Sound emitter update: plays each emitter's day or night sound while the player is in range, else stops it.
// __thiscall on the sound manager (called by EqSoundManagerUpdate_guess::Update_guess). Sets the night flag (+0x101) and walks the emitter list
// (0x006E4D80, newest first, linked through +4) with the emitter in +0xD8. An emitter of type 0 (+0x38) gets a
// timed repeat: the day / night delay (+0x20 / +0x24) plus up to +0x28 of jitter, volume -(+0x3C / +0x40); type 2 is
// the same with +0x20 only, a minimum distance (+0x4C) and its position in the buffer. Then, with sounds on
// (0x00579324) and the local player in range, a change of day / night stops the handle of the other half (+0x54 day,
// +0x58 night) and the matching sound (+0x30 / +0x34) is started through the sound control object (0x006E4DD0):
// played at once with the buffer when the delay is set, else acquired and commanded. Out of range both handles are
// stopped and released.
void EqSoundManagerEmitters_guess::UpdateEmitters_guess(void)
{
    // VC6 represents these nonvirtual member pointers as code addresses; this preserves the fixed-address thiscall ABI.
    union SetModePointer_guess
    {
        void (__fastcall *address)(EQSoundBufferEmitter_guess*, int, u8);
        void (EQSoundBufferEmitter_guess::*method)(u8);
    };
    union SetVolumePointer_guess
    {
        bool (__fastcall *address)(EQSoundBufferEmitter_guess*, int, int);
        bool (EQSoundBufferEmitter_guess::*method)(int);
    };
    union SetMinDistancePointer_guess
    {
        bool (__fastcall *address)(EQSoundBufferEmitter_guess*, int, float);
        bool (EQSoundBufferEmitter_guess::*method)(float);
    };
    union IsInRangePointer_guess
    {
        bool (__fastcall *address)(EQSoundEmitter_guess*, int);
        bool (EQSoundEmitter_guess::*method)(void);
    };
    SetModePointer_guess setMode = { EQ_THISCALL(void, 0x004ECBC2, (EQSoundBufferEmitter_guess*, int, u8)) };
    SetVolumePointer_guess setVolume = { EQ_THISCALL(bool, 0x004ECBA1, (EQSoundBufferEmitter_guess*, int, int)) };
    SetMinDistancePointer_guess setMinDistance = { EQ_THISCALL(bool, 0x004ECBFA, (EQSoundBufferEmitter_guess*, int, float)) };
    IsInRangePointer_guess isInRange = { EQ_THISCALL(bool, 0x004BD3BD, (EQSoundEmitter_guess*, int)) };
    EQSoundBufferEmitter_guess buf;
    int volume;
    u32 delay;

    unknown0x101 = (u8)EQ_CDECL(int, 0x004E2859, (void))();
    unknown0x0D8 = (u32)g_sound_emitter_head_guess;
    while (unknown0x0D8 != 0)
    {
        if (EMITTER_guess->unknown0x38 == 0)
        {
            if (unknown0x101)
            {
                delay = (u32)EMITTER_guess->unknown0x24;
                volume = -EMITTER_guess->unknown0x40;
            }
            else
            {
                delay = (u32)EMITTER_guess->unknown0x20;
                volume = -EMITTER_guess->unknown0x3C;
            }
            buf.changed_flags = 0;
            if (delay > 0)
            {
                if (g_cdisplay_guess->time_ms_guess < (u32)EMITTER_guess->unknown0x2C)
                {
                    unknown0x0D8 = (u32)EMITTER_guess->prev_guess;
                    continue;
                }
                EMITTER_guess->unknown0x2C = g_cdisplay_guess->time_ms_guess + delay;
                if ((u32)EMITTER_guess->unknown0x28 > 0)
                    EMITTER_guess->unknown0x2C += EQ_CDECL(int, 0x004A46F6, (int, int))(500, EMITTER_guess->unknown0x28);
                (buf.*setMode.method)(0x10);
            }
            else
                (buf.*setMode.method)(0x11);
            if (volume < 0)
                (buf.*setVolume.method)(volume);
            else
                (buf.*setVolume.method)(-2000);
        }
        else if (EMITTER_guess->unknown0x38 == 2)
        {
            volume = -EMITTER_guess->unknown0x3C;
            buf.changed_flags = 0;
            if ((u32)EMITTER_guess->unknown0x20 > 0)
            {
                if (g_cdisplay_guess->time_ms_guess < (u32)EMITTER_guess->unknown0x2C)
                {
                    unknown0x0D8 = (u32)EMITTER_guess->prev_guess;
                    continue;
                }
                EMITTER_guess->unknown0x2C = g_cdisplay_guess->time_ms_guess + (u32)EMITTER_guess->unknown0x20;
                if ((u32)EMITTER_guess->unknown0x28 > 0)
                    EMITTER_guess->unknown0x2C += EQ_CDECL(int, 0x004A46F6, (int, int))(500, EMITTER_guess->unknown0x28);
            }
            else
                (buf.*setMode.method)(1);
            if ((u32)EMITTER_guess->unknown0x44[2] > 2)
                (buf.*setMinDistance.method)((float)(u32)EMITTER_guess->unknown0x44[2]);
            if (volume < 0)
                (buf.*setVolume.method)(volume);
            buf.SetFloatPair_guess(EMITTER_guess->x, EMITTER_guess->y, EMITTER_guess->z);
        }
        else
        {
            unknown0x0D8 = (u32)EMITTER_guess->prev_guess;
            continue;
        }

        if (EQ_GLOBAL(u8, 0x00579324) != 0
            && (EMITTER_guess->*isInRange.method)())
        {
            if (unknown0x101 != unknown0x100)
            {
                if (unknown0x100)
                {
                    if (EMITTER_guess->unknown0x58 != -1)
                    {
                        g_sound_control_emitter_guess->Stop_guess(&EMITTER_guess->unknown0x58);
                        g_sound_control_emitter_guess->Release_guess(&EMITTER_guess->unknown0x58);
                        EMITTER_guess->unknown0x58 = -1;
                    }
                }
                else
                {
                    if (EMITTER_guess->unknown0x54 != -1)
                    {
                        g_sound_control_emitter_guess->Stop_guess(&EMITTER_guess->unknown0x54);
                        g_sound_control_emitter_guess->Release_guess(&EMITTER_guess->unknown0x54);
                        EMITTER_guess->unknown0x54 = -1;
                    }
                }
                unknown0x100 = unknown0x101;
            }
            if (EMITTER_guess->unknown0x38 == 2 || unknown0x101 == 0)
            {
                if (EMITTER_guess->unknown0x54 == -1 && EMITTER_guess->sound_id_guess[0] != 0)
                {
                    if ((u32)EMITTER_guess->unknown0x20 > 0)
                    {
                        EMITTER_guess->unknown0x54 = -1;
                        g_sound_control_emitter_guess->Play_guess(EMITTER_guess->sound_id_guess[0], &buf);
                    }
                    else
                    {
                        EMITTER_guess->unknown0x54 =
                            g_sound_control_emitter_guess->Acquire_guess(EMITTER_guess->sound_id_guess[0]);
                        if (EMITTER_guess->unknown0x54 == -1)
                            OutputDebugStringA(EQ_ARRAY(char, 0x0057BD6C));
                        else
                            g_sound_control_emitter_guess->Command_guess(&EMITTER_guess->unknown0x54, &buf);
                    }
                }
            }
            else
            {
                if (EMITTER_guess->unknown0x58 == -1 && EMITTER_guess->sound_id_guess[1] != 0)
                {
                    if ((u32)EMITTER_guess->unknown0x24 > 0)
                    {
                        EMITTER_guess->unknown0x58 = -1;
                        g_sound_control_emitter_guess->Play_guess(EMITTER_guess->sound_id_guess[1], &buf);
                    }
                    else
                    {
                        EMITTER_guess->unknown0x58 =
                            g_sound_control_emitter_guess->Acquire_guess(EMITTER_guess->sound_id_guess[1]);
                        if (EMITTER_guess->unknown0x58 == -1)
                            OutputDebugStringA(EQ_ARRAY(char, 0x0057BD80));
                        else
                            g_sound_control_emitter_guess->Command_guess(&EMITTER_guess->unknown0x58, &buf);
                    }
                }
            }
        }
        else
        {
            if (EMITTER_guess->unknown0x54 != -1)
            {
                g_sound_control_emitter_guess->Stop_guess(&EMITTER_guess->unknown0x54);
                g_sound_control_emitter_guess->Release_guess(&EMITTER_guess->unknown0x54);
                EMITTER_guess->unknown0x54 = -1;
            }
            if (EMITTER_guess->unknown0x58 != -1)
            {
                g_sound_control_emitter_guess->Stop_guess(&EMITTER_guess->unknown0x58);
                g_sound_control_emitter_guess->Release_guess(&EMITTER_guess->unknown0x58);
                EMITTER_guess->unknown0x58 = -1;
            }
        }
        unknown0x0D8 = (u32)EMITTER_guess->prev_guess;
    }
}
#undef EMITTER_guess

// FUNCTION: TRILOGY 0x004e5f06
// No direct game action; provides a unit of a large number used in session-key exchange.
// First of the vlong functions (0x004E5F06..0x004E6F03): the arbitrary-precision unsigned integer behind the
// session key exchange, see include/trilogy/vlong.h. The Mac map labels the range DPVS (not linked into
// Trilogy); queue subsystem from the address range. All __thiscall.
// Unit i, 0 past the used units.
unsigned flex_unit_guess::get(unsigned i) const
{
    if (i >= n)
        return 0;
    return a[i];
}

// FUNCTION: TRILOGY 0x004e5f1b
// No direct game action; empties the used portion of large-number storage for key calculations.
void flex_unit_guess::clear()
{
    n = 0;
}

// FUNCTION: TRILOGY 0x004e5f20
// No direct game action; prepares storage for large-number key calculations.
flex_unit_guess::flex_unit_guess()
{
    z = 0;
    a = 0;
    n = 0;
}

// FUNCTION: TRILOGY 0x004e5f2d
// No direct game action; releases storage held by a large number used in key calculations.
flex_unit_guess::~flex_unit_guess()
{
    if (a != 0)
        delete[] a;
}

// FUNCTION: TRILOGY 0x004e5f3b
// Grows the allocation to x units (keeps the used ones); never shrinks.
void flex_unit_guess::reserve(unsigned x)
{
    if (x > z)
    {
        unsigned* na = new unsigned[x];
        unsigned i;
        for (i = 0; i < n; i++)
            na[i] = a[i];
        delete[] a;
        a = na;
        z = x;
    }
}

// FUNCTION: TRILOGY 0x004e5f7f
// Sets unit i, growing (zero-filled) for a non-zero x past the end, trimming high zero units otherwise.
void flex_unit_guess::set(unsigned i, unsigned x)
{
    if (i < n)
    {
        a[i] = x;
        if (x == 0)
            while (n != 0 && a[n - 1] == 0)
                n--;
    }
    else if (x != 0)
    {
        unsigned j;
        reserve(i + 1);
        for (j = n; j < i; j++)
            a[j] = 0;
        a[i] = x;
        n = i + 1;
    }
}

// FUNCTION: TRILOGY 0x004e5fe9
// *this = (x * y) mod 2^keep, schoolbook multiplication in 16-bit halves.
void flex_unit_guess::fast_mul(flex_unit_guess& x, flex_unit_guess& y, unsigned keep)
{
    unsigned i;
    unsigned limit = (keep + 31) / 32;
    unsigned min;

    reserve(limit);
    for (i = 0; i < limit; i++)
        a[i] = 0;
    min = x.n;
    if (min > limit)
        min = limit;
    for (i = 0; i < min; i++)
    {
        unsigned m = x.a[i];
        unsigned c = 0;
        unsigned min2 = i + y.n;
        unsigned j;
        if (min2 > limit)
            min2 = limit;
        for (j = i; j < min2; j++)
        {
            unsigned w;
            unsigned v = a[j];
            unsigned p = y.a[j - i];
            v += c;
            c = v < c;
            w = (p & 0xFFFF) * (m & 0xFFFF);
            v += w;
            c += v < w;
            w = (p & 0xFFFF) * (m >> 16);
            c += w >> 16;
            w <<= 16;
            v += w;
            c += v < w;
            w = (p >> 16) * (m & 0xFFFF);
            c += w >> 16;
            w <<= 16;
            v += w;
            c += v < w;
            c += (p >> 16) * (m >> 16);
            a[j] = v;
        }
        while (c != 0 && j < limit)
        {
            a[j] += c;
            c = a[j] < c;
            j++;
        }
    }
    keep %= 32;
    if (keep != 0)
        a[limit - 1] &= (1 << keep) - 1;
    while (limit != 0 && a[limit - 1] == 0)
        limit--;
    n = limit;
}

// FUNCTION: TRILOGY 0x004e6179
// No direct game action; checks whether a large key-exchange number is zero.
int vlong_value_guess::is_zero() const
{
    return n == 0;
}

// FUNCTION: TRILOGY 0x004e6182
// Bit i.
int vlong_value_guess::test(unsigned i) const
{
    return (get(i / 32) & (1 << (i % 32))) != 0;
}

// FUNCTION: TRILOGY 0x004e61a6
// Number of significant bits.
unsigned vlong_value_guess::bits() const
{
    unsigned x = n * 32;

    while (x != 0 && test(x - 1) == 0)
        x--;
    return x;
}

// FUNCTION: TRILOGY 0x004e61ce
// Compares magnitudes: 1, 0 or -1.
int vlong_value_guess::cf(vlong_value_guess& x) const
{
    unsigned i;

    if (n > x.n)
        return 1;
    if (n < x.n)
        return -1;
    i = n;
    while (i != 0)
    {
        i--;
        if (get(i) > x.get(i))
            return 1;
        if (get(i) < x.get(i))
            return -1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e6228
// Shifts left by one bit (grows by a unit when the top bit is set).
void vlong_value_guess::shl()
{
    unsigned carry = 0;
    unsigned N = n;
    unsigned i;

    for (i = 0; i <= N; i++)
    {
        unsigned u = get(i);
        set(i, (u << 1) + carry);
        carry = u >> 31;
    }
}

// FUNCTION: TRILOGY 0x004e6263
// Shifts right by one bit.
void vlong_value_guess::shr()
{
    unsigned carry = 0;
    unsigned i = n;

    while (i != 0)
    {
        unsigned u;
        i--;
        u = get(i);
        set(i, (u >> 1) + carry);
        carry = u << 31;
    }
}

// FUNCTION: TRILOGY 0x004e6298
// Shifts right by x bits (vlong's shr(unsigned)); the loop bound is re-read as set() trims.
void vlong_value_guess::shr_by(unsigned x)
{
    unsigned delta = x / 32;
    unsigned i;

    x %= 32;
    for (i = 0; i < n; i++)
    {
        unsigned u = get(i + delta);
        if (x != 0)
        {
            u >>= x;
            u += get(i + delta + 1) << (32 - x);
        }
        set(i, u);
    }
}

// FUNCTION: TRILOGY 0x004e62ff
// *this += x.
void vlong_value_guess::add(vlong_value_guess& x)
{
    unsigned carry = 0;
    unsigned max = n;
    unsigned i;

    if (max < x.n)
        max = x.n;
    reserve(max);
    for (i = 0; i < max + 1; i++)
    {
        unsigned u = get(i);
        unsigned ux;
        u = u + carry;
        carry = u < carry;
        ux = x.get(i);
        u = u + ux;
        carry += u < ux;
        set(i, u);
    }
}

// FUNCTION: TRILOGY 0x004e636e
// *this -= x (x <= *this assumed); a unit whose subtrahend overflowed with the carry is skipped.
void vlong_value_guess::subtract(vlong_value_guess& x)
{
    unsigned carry = 0;
    unsigned N = n;
    unsigned i;

    for (i = 0; i < N; i++)
    {
        unsigned ux = x.get(i);
        ux += carry;
        if (ux >= carry)
        {
            unsigned u = get(i);
            unsigned nu = u - ux;
            carry = nu > u;
            set(i, nu);
        }
    }
}

// FUNCTION: TRILOGY 0x004e63c0
// No direct game action; starts a large-number value from an initial number for key calculations.
void vlong_value_guess::init(unsigned x)
{
    clear();
    set(0, x);
}

// FUNCTION: TRILOGY 0x004e63d9
// No direct game action; copies a large-number value used in session-key exchange.
void vlong_value_guess::copy(vlong_value_guess& x)
{
    unsigned i;

    clear();
    i = x.n;
    while (i != 0)
    {
        i--;
        set(i, x.get(i));
    }
}

// FUNCTION: TRILOGY 0x004e6408
// No direct game action; initializes sharing state for a large-number key-exchange value.
vlong_value_guess::vlong_value_guess()
{
    share = 0;
}

// FUNCTION: TRILOGY 0x004e6418
// No direct game action; forms a product for the large-number key-exchange arithmetic.
void vlong_value_guess::mul(vlong_value_guess& x, vlong_value_guess& y)
{
    fast_mul(x, y, x.bits() + y.bits());
}

// FUNCTION: TRILOGY 0x004e6447
// ORIGINAL FILE: unknown (vlong library; no Mac symbol)
// *this = x / y, rem = x mod y: shift-and-subtract long division. C++ EH frame (the two locals).
void vlong_value_guess::divide(vlong_value_guess& x, vlong_value_guess& y, vlong_value_guess& rem)
{
    init(0);
    rem.copy(x);
    vlong_value_guess m, s;
    m.copy(y);
    s.init(1);
    while (rem.cf(m) > 0)
    {
        m.shl();
        s.shl();
    }
    while (rem.cf(y) >= 0)
    {
        while (rem.cf(m) < 0)
        {
            m.shr();
            s.shr();
        }
        rem.subtract(m);
        add(s);
    }
}

// FUNCTION: TRILOGY 0x004e6529
// No direct game action; marks the end of a magnitude value used in key-exchange arithmetic.
vlong_value_guess::~vlong_value_guess()
{
}

// FUNCTION: TRILOGY 0x004e652e
// Copy on write: takes a private copy of a shared value.
void vlong_guess::docopy()
{
    if (value->share != 0)
    {
        vlong_value_guess* nv;
        value->share--;
        nv = new vlong_value_guess;
        nv->copy(*value);
        value = nv;
    }
}

// FUNCTION: TRILOGY 0x004e6566
// No direct game action; orders signed values used in session-key exchange.
// Compares signed values: when the signs (zero counts as positive) agree it is vlong_value::cf of the
// magnitudes (also for two negatives: faithful), else the negative one is smaller. `x` is a by-value
// copy, destroyed here.
int vlong_guess::cf(vlong_guess x) const
{
    int neg = negative && !value->is_zero();
    if (neg == (x.negative && !x.value->is_zero()))
        return value->cf(*x.value);
    else if (neg)
        return -1;
    else
        return +1;
}

// FUNCTION: TRILOGY 0x004e65e1
// No direct game action; creates a signed key-exchange value from an initial nonnegative number.
vlong_guess::vlong_guess(unsigned x)
{
    value = new vlong_value_guess;
    negative = 0;
    value->init(x);
}

// FUNCTION: TRILOGY 0x004e6612
// No direct game action; carries an existing signed key-exchange value into another value.
vlong_guess::vlong_guess(const vlong_guess& x)
{
    negative = x.negative;
    value = x.value;
    value->share++;
}

// FUNCTION: TRILOGY 0x004e6628
// No direct game action; replaces a signed key-exchange value with another value.
// operator=: releases the old value, shares the new one. `x` is a by-value copy (so the copy's own
// reference is released at the end), which gives the function its C++ EH frame.
vlong_guess& vlong_guess::assign(vlong_guess x)
{
    if (value->share != 0)
        value->share--;
    else
        delete value;
    value = x.value;
    value->share++;
    negative = x.negative;
    return *this;
}

// FUNCTION: TRILOGY 0x004e6687
// No direct game action; releases a signed key-exchange value when it is no longer needed.
vlong_guess::~vlong_guess()
{
    if (value->share != 0)
        value->share--;
    else
        delete value;
}

// FUNCTION: TRILOGY 0x004e66ab
// No direct game action; adds one signed key-exchange value to another.
vlong_guess& vlong_guess::add_assign(const vlong_guess& x)
{
    if (negative == x.negative)
    {
        docopy();
        value->add(*x.value);
    }
    else if (value->cf(*x.value) >= 0)
    {
        docopy();
        value->subtract(*x.value);
    }
    else
    {
        vlong_guess tmp = *this;
        assign(x);
        add_assign(tmp);
    }
    return *this;
}

// FUNCTION: TRILOGY 0x004e673f
// No direct game action; subtracts one signed key-exchange value from another.
vlong_guess& vlong_guess::sub_assign(const vlong_guess& x)
{
    if (negative != x.negative)
    {
        docopy();
        value->add(*x.value);
    }
    else if (value->cf(*x.value) >= 0)
    {
        docopy();
        value->subtract(*x.value);
    }
    else
    {
        vlong_guess tmp = *this;
        assign(x);
        sub_assign(tmp);
        negative = 1 - negative;
    }
    return *this;
}

// FUNCTION: TRILOGY 0x004e67dc
// __cdecl, result through a hidden first pointer: x - y.
vlong_guess vlong_sub_guess(const vlong_guess& minuend, const vlong_guess& subtrahend)
{
    vlong_guess result = minuend;
    result.sub_assign(subtrahend);
    return result;
}

// FUNCTION: TRILOGY 0x004e6835
// x * y: magnitude product, sign = xor.
vlong_guess vlong_mul_guess(const vlong_guess& x, const vlong_guess& y)
{
    vlong_guess result(0);
    result.value->mul(*x.value, *y.value);
    result.negative = x.negative ^ y.negative;
    return result;
}

// FUNCTION: TRILOGY 0x004e68a1
// x / y (the remainder is discarded), sign = xor.
vlong_guess vlong_div_guess(const vlong_guess& x, const vlong_guess& y)
{
    vlong_guess result(0);
    vlong_value_guess rem;
    result.value->divide(*x.value, *y.value, rem);
    result.negative = x.negative ^ y.negative;
    return result;
}

// FUNCTION: TRILOGY 0x004e6928
// x % y: the remainder of the magnitudes, sign of x.
vlong_guess vlong_mod_guess(const vlong_guess& x, const vlong_guess& y)
{
    vlong_guess result(0);
    vlong_value_guess divide;
    divide.divide(*x.value, *y.value, *result.value);
    result.negative = x.negative;
    return result;
}

// FUNCTION: TRILOGY 0x004e69a9
// Modular inverse of a mod m (extended Euclid), the vlong library's modinv.
vlong_guess modinv_guess(const vlong_guess& a, const vlong_guess& m)
{
    vlong_guess j = 1, i = 0, b = m, c = a, x(0), y(0);
    while (vlong_ne_guess(c, vlong_guess(0)))
    {
        x.assign(vlong_div_guess(b, c));
        y.assign(vlong_sub_guess(b, vlong_mul_guess(x, c)));
        b.assign(c);
        c.assign(y);
        y.assign(j);
        j.assign(vlong_sub_guess(i, vlong_mul_guess(j, x)));
        i.assign(y);
    }
    if (vlong_lt_guess(i, vlong_guess(0)))
        i.add_assign(m);
    return i;
}

// FUNCTION: TRILOGY 0x004e6be3
// Prepares the modular arithmetic context used for the client's RSA encryption.
// monty constructor: R = smallest power of two >= M (N its exponent), R1 = (R - m)^-1 mod m,
// n1 = R - m^-1 mod R. C++ EH frame (six vlong members).
monty_guess::monty_guess(const vlong_guess& M)
    : R(0), R1(0), m(0), n1(0), T(0), k(0)
{
    m.assign(M);
    N = 0;
    R.assign(1);
    while (vlong_lt_guess(R, M))
    {
        R.add_assign(R);
        N += 1;
    }
    R1.assign(modinv_guess(vlong_sub_guess(R, m), m));
    n1.assign(vlong_sub_guess(R, modinv_guess(m, R)));
}

// FUNCTION: TRILOGY 0x004e6d0f
// Montgomery product: x = x * y / R mod m.
void monty_guess::mul(vlong_guess& x, const vlong_guess& y)
{
    T.value->fast_mul(*x.value, *y.value, N * 2);
    k.value->fast_mul(*T.value, *n1.value, N);
    x.value->fast_mul(*k.value, *m.value, N * 2);
    x.add_assign(T);
    x.value->shr_by(N);
    if (vlong_ge_guess(x, m))
        x.sub_assign(m);
}

// FUNCTION: TRILOGY 0x004e6d8c
// x >= y; y is copied for the by-value argument of cf.
int vlong_ge_guess(const vlong_guess& x, const vlong_guess& y)
{
    return x.cf(y) >= 0;
}

// FUNCTION: TRILOGY 0x004e6db2
// x^e mod m by square-and-multiply in the Montgomery domain.
vlong_guess monty_guess::exp(const vlong_guess& x, const vlong_guess& e)
{
    vlong_guess result = vlong_sub_guess(R, m), t = vlong_mod_guess(vlong_mul_guess(x, R), m);
    unsigned bits = e.value->bits();
    unsigned i = 0;
    while (1)
    {
        if (e.value->test(i))
            mul(result, t);
        i += 1;
        if (i == bits)
            break;
        mul(t, t);
    }
    return vlong_mod_guess(vlong_mul_guess(result, R1), m);
}

// FUNCTION: TRILOGY 0x004e6eb0
// modexp: constructs the monty context for m and runs exp.
vlong_guess modexp_guess(const vlong_guess& x, const vlong_guess& e, const vlong_guess& m)
{
    monty_guess me(m);
    return me.exp(x, e);
}

// FUNCTION: TRILOGY 0x004e6f03
// No distinct game-facing purpose is evident for this destructor.
monty_guess::~monty_guess()
{
}

// FUNCTION: TRILOGY 0x004e6f75
// Thunk that initializes the client's draw-handler object.
// Sets ECX to 0x006ECE30 and tail-jumps to initializeObjectWithVTable_guess (0x004E805A),
// so the callee's return goes straight back to our caller.
void InitializeDrawHandler_guess(void)
{
    EQ_METHOD(0, void, 0x004E805A, (void*), (void*)0x006ece30)();
}

// FUNCTION: TRILOGY 0x004e6f7f
// Registers an internal client cleanup callback for process exit; its game-facing purpose is unclear.
// __cdecl, no arguments. Registers the code at 0x004E6F8B (the code right after this function) with the
// CRT's _atexit (0x00523AB6) and returns its result, which the original leaves in EAX.
int EQ_RegisterDtor_guess(void)
{
    return EQ_CDECL(int, 0x00523AB6, (void (*)(void)))((void (*)(void))0x004e6f8b);
}

// FUNCTION: TRILOGY 0x004ecba1
// ORIGINAL FILE: unknown (queue subsystem from the call graph)
// __thiscall. Stores the volume clamped to -10000 .. 0 and marks it changed. Returns true.
bool EQSoundBuffer_guess::SetVolume_guess(int vol)
{
    if (vol < -10000)
        vol = -10000;
    if (vol > 0)
        vol = 0;
    changed_flags |= 0x40;
    volume = vol;
    return true;
}

// FUNCTION: TRILOGY 0x004ecbfa
// ORIGINAL FILE: unknown (queue subsystem from the call graph)
// __thiscall. Stores the value clamped to 0.01 .. 1000 and marks it changed. Returns true.
bool EQSoundBuffer_guess::SetMinDistance_guess(float distance)
{
    if (distance <= 0.01f)
        distance = 0.01f;
    if (distance > 1000.0f)
        distance = 1000.0f;
    changed_flags |= 0x80;
    min_distance = distance;
    return 1;
}

// FUNCTION: TRILOGY 0x004ecc30
// Converts a game sound volume setting to the level used by DirectSound.
// __thiscall in the binary: the caller loads ECX, the callee never reads it, and the double is the only
// argument (the callee pops 8 bytes). Written __fastcall: a double takes no register, so the sequence is the same.
// Linear volume (0..1) to hundredths of a dB (DirectSound's -10000 .. 0 range): 0 at 1.0 and above,
// -10000 at 1e-5 and below, else (int) of the cdecl double function at 0x00524784 (a log; base unconfirmed)
// times the constant at 0x00535870. The two tests use the literals Ghidra shows at 0x00535290 and 0x00535878
// (1.0, 1e-5): VC6 then compares the volume first, as the binary does, and a NaN falls to -10000.
int __fastcall LinearToDsVolume_guess(double linearGain)
{
    if (linearGain >= 1.0)
        return 0;
    if (linearGain <= 1e-5)
        return -10000;
    return (int)(EQ_CDECL(double, 0x00524784, (double))(linearGain) * EQ_GLOBAL(double, 0x00535870));
}

// FUNCTION: TRILOGY 0x004ecc77
// Registers the client's sound-volume helper for shutdown cleanup.
void RegisterExitCleanup_guess(void)
{
    EQ_GLOBAL(u32, 0x006EFEA4) = 0;
    EQ_GLOBAL(void*, 0x006EFE50) = (void*)0x00535798;
    EQ_CDECL(int, 0x00523AB6, (void (__cdecl *)(void)))
        ((void (__cdecl *)(void))0x004ecc94);
}

// The sound buffer (vtable 0x00535798) on the stack of the master volume setter below. The original never destroys it
// on the normal path, so its destructor is empty.
class EQSoundBufferVolume_guess
{
public:
    void*   vtable;             /* 0x00 */
    u8      unknown0x04[0x50];  /* 0x04 */
    u32     changed_flags;      /* 0x54 */

    EQSoundBufferVolume_guess(void) { changed_flags = 0; vtable = (void*)0x00535798; }
    ~EQSoundBufferVolume_guess(void) {}
};
C_ASSERT(sizeof(EQSoundBufferVolume_guess) == 0x58);

// The object with 720 (0x2D0) sound slots from +0x0C and a volume at +0xBD0 (SoundManager.cpp's
// EqDirectSoundSlotsStop_guess is the 16-slot one the per-slot call at 0x004EC6AB belongs to).
class EqSoundSlotsVolume_guess
{
public:
    u8      unknown0x00[0x0C];  /* 0x00 */
    u32     slots[0x2D0];       /* 0x0C */
    u8      unknown0xB4C[0x84]; /* 0xB4C */
    float   volume;             /* 0xBD0 */

    void SetVolume_guess(float value);
};

// FUNCTION: TRILOGY 0x004ee1db
// Applies the chosen sound volume to the client's active sound slots.
// __thiscall, 1 float argument. Clamps the volume to 0 .. 1 and stores it at +0xBD0 only when it was out of range
// (the original keeps the argument on the x87 stack and never stores an in-range value), converts the argument to
// DirectSound hundredths of a dB with 0x004ECC30 (called with the stack buffer as ECX, which it ignores) and passes
// that to 0x004EC6AB of every non-null slot.
// @early-stop: no EH frame for the buffer under /GX (original: mov eax,handler; call __EH_prolog, so the buffer sits at
// -0x64); the EDX dummy of the 0x004EC6AB call costs an xor edx,edx.
void EqSoundSlotsVolume_guess::SetVolume_guess(float value)
{
    int dsVolume;
    u32* slot;
    int count;

    if (value < 0.0)
        volume = 0.0f;
    if (value > 1.0)
        volume = 1.0f;
    EQSoundBufferVolume_guess buffer;
    dsVolume = EQ_FASTCALL(int, 0x004ECC30, (EQSoundBufferVolume_guess*, double))(&buffer, value);
    slot = (u32*)((u8*)this + 0xC);
    count = 0x2D0;
    do
    {
        if (*slot != 0)
            EQ_METHOD(1, void, 0x004EC6AB, (u32, int), *slot)(dsVolume);
        slot++;
        count--;
    } while (count != 0);
}

// FUNCTION: TRILOGY 0x004EE266
// Supplies a miscellaneous client output value and its status flags.
bool EqMiscOutputSource_guess::FillOutputRecord_guess(EqMiscOutputRecord_guess* output)
{
    if (hasValue != 0)
        output->value = value;
    if (flagBca == '\0')
        output->flagBca = 0;
    else
        output->flagBca = 1;
    if (flagBc8 == '\0')
        output->flagBc8 = 0;
    else
        output->flagBc8 = 1;
    return true;
}

// FUNCTION: TRILOGY 0x004ee2a5
// Receives a miscellaneous value from another client component; its game purpose is unclear.
u32 EqMiscValueSink_guess::SetCappedValue_guess(u32* source)
{
    u32 value = *source;
    cappedValue = value;
    if (value >= EQ_MISC_VALUE_COUNT)
        cappedValue = EQ_MISC_VALUE_COUNT - 1;
    return (value & 0xffffff00) | 1;
}

// FUNCTION: TRILOGY 0x004ee2c3
// Updates the bounded value held by this miscellaneous object; its game purpose is unclear.
void EqMiscCappedValueObject_guess::SetBoundedValue_guess(u32 value)
{
    boundedValue = value;
    if (value >= EQ_MISC_VALUE_COUNT)
        boundedValue = EQ_MISC_VALUE_COUNT - 1;
}

// FUNCTION: TRILOGY 0x004EE2DD
// Advances a miscellaneous active-state counter; the game event it tracks is unclear.
void __fastcall IncrementCounterIfActive_guess(EqMiscCounterState_guess* state)
{
    if (state->active != 0)
    {
        if (EQ_GLOBAL(u32, 0x006EFEB0) >= 0x28)
            EQ_GLOBAL(u32, 0x006EFEB0) = 0;
        EQ_GLOBAL(u32, 0x006EFEB0) = EQ_GLOBAL(u32, 0x006EFEB0) + 1;
    }
    return;
}
