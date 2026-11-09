// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x00450463 EQ_Overlay::DelButtons
// EQ_Requestor.cpp - requestor overlay destruction.

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/character.h"
#include "trilogy/player.h"
#include "trilogy/globals.h"
#include "trilogy/layout/eq_requestor.h"
#include <stdio.h>
#include <string.h>

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef send_message_guess
// EXTERNAL: TRILOGY 0x004E1C45 send_message_guess
void __cdecl send_message_guess(void** connection, u32 opcode, void* data, u32 size, int reliable);

struct EQ_RequestorClearState_guess
{
    char reserved_000[0x10];
    int cleared_value_010;
    char reserved_014[0x92];
    unsigned char cleared_byte_a6;
    char reserved_a7;
    short cleared_word_a8;
    char reserved_aa[2];
    int cleared_value_ac;
    unsigned char cleared_byte_b0;
    char text_b1[1];
};

struct EQ_RequestorInitialization_guess
{
    char reserved_000[0x10];
    u32 cleared_value_010;
    char reserved_014[0x160];
    u8 cleared_byte_174;
    u8 reserved_175;
    u16 cleared_word_176;
    u32 cleared_value_178;
    u8 cleared_action_17c;
    char reserved_17d[0x3c];
    char requestor_name[1];
};

struct EQ_RequestorManagerState_guess
{
    char reserved_000[0xeb8];
    int requestor_enabled;
};

class CharacterSelector_guess
    : public EQ_Overlay
{
public:
    CharacterSelector_guess();
};

class EQ_RequestorCallbackTable_guess
{
public:
    virtual void slot0_guess();
    virtual void slot1_guess();
    virtual void expire_guess();
};

struct EQ_RequestorRECT_guess
{
    long left;
    long top;
    long right;
    long bottom;
};

typedef int (__stdcall *SetRect_guess_t)(EQ_RequestorRECT_guess*, int, int, int, int);

struct RequestorIdentityMessage_guess
{
    char request_name[30];
    char player_name[30];
    u32 requestor_type;
    u16 requestor_id;
    u16 padding;
};

struct RequestorTargetMessage_guess
{
    char target_name[30];
    char player_name[30];
    int target_id;
    int target_level;
    int target_status;
};

struct RequestorSurnameMessage_guess
{
    char player_name[30];
    char padding[2];
    int zero;
    char surname[16];
    char tail[4];
};

struct RequestorKillMessage_guess
{
    char target_name[30];
    char target_last_name[30];
    char tail;
};

struct RequestorCurrencyMessage_guess
{
    u32 zero;
    int negative_one;
    float denomination;
    float denomination_copy;
    int amount;
};

struct RequestorKillMessageOverlay_guess
{
    char padding[0x18];
    RequestorKillMessage_guess message;
};

struct RequestorSurnameMessageOverlay_guess
{
    char padding[0x20];
    RequestorSurnameMessage_guess message;
};

struct RequestorIdentityMessageOverlay_guess
{
    char padding[0x58];
    RequestorIdentityMessage_guess message;
};

union RequestorInitialScratch_guess
{
    char bytes[0x9c];
    char translocation_message[0x58];
    RequestorKillMessageOverlay_guess kill;
    RequestorSurnameMessageOverlay_guess surname;
    RequestorIdentityMessageOverlay_guess identity;
};

union RequestorFinalMessage_guess
{
    struct
    {
        u32 zero;
        u32 spawn_id;
        float value;
    } sacrifice;
    struct
    {
        u32 padding;
        u32 spawn_id;
        u32 value_bits;
    } target_level;
};

struct RequestorProcessButtonFrame_guess
{
    char chat_text[512];
    RequestorInitialScratch_guess initial_scratch;
    RequestorTargetMessage_guess target;
    int item_message[3];
    RequestorCurrencyMessage_guess currency;
    RequestorFinalMessage_guess final_message;
};

// EXTERNAL: TRILOGY 0x00535430 ??_7EQ_Requestor@@6B@
// FUNCTION: TRILOGY 0x004347ce
// Destroys the EQ_Requestor overlay and its base.
// ORIGINAL FILE: EverQuest/EQ_Requestor.cpp (from Mac debug map)
EQ_Requestor::~EQ_Requestor()
{
    DelButtons();
}

// EXTERNAL: TRILOGY 0x00535628 ??_7EQ_Requestor2@@6B@
class EQ_Requestor2
    : public EQ_VirtualOverlay_guess
{
public:
    ~EQ_Requestor2();
    void draw_guess();
};

// FUNCTION: TRILOGY 0x0048143c
// Destroys the EQ_Requestor2 overlay and its base.
// ORIGINAL FILE: EverQuest/EQ_Requestor.cpp (from Mac debug map)
EQ_Requestor2::~EQ_Requestor2()
{
    DelButtons();
}

// FUNCTION: TRILOGY 0x00481474
// Displays an active request dialog and sends timeout responses when the reply period expires.
// ORIGINAL FILE: EverQuest/EQ_Requestor.cpp (from Mac debug map)
// @early-stop: EBX/EDI allocation and TEST AL,AL versus CMP AL,BL remain after eight attempts without improvement.
void EQ_Requestor::ExpireRequestor()
{
    char localData[0xa8];
    char* localFrame;
    u32 currentTime;
    if (((int*)this)[4] == 0)
        return;
    localFrame = localData;
    currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
    if (currentTime > (u32)((int*)this)[0x5e])
    {
        if (*(char*)((char*)this + 0x17c) == 0)
        {
            *(short*)(localFrame + 0x40) = *(short*)((char*)this + 0x176);
            strcpy(localFrame + 0x1e,
                EQ_GLOBAL(char*, 0x006E4DA8));
            strcpy(localFrame,
                (char*)this + 0x5b9);
            *(u32*)(localFrame + 0x3c) = 4;
            send_message_guess(g_world_connection_guess, 0x2118, localFrame, 0x44, 1);
        }
        else if (*(char*)((char*)this + 0x17c) == 8)
        {
            strcpy(localFrame + 0x44,
                (char*)this + 0x5b9);
            strcpy(localFrame + 0x62,
                EQ_GLOBAL(char*, 0x006E4DA8));
            *(u32*)(localFrame + 0x80) = 0xfffffffd;
            *(u32*)(localFrame + 0x88) = 0;
            send_message_guess(g_world_connection_guess, 0x216f, localFrame + 0x44, 0x48, 1);
        }
        else if (*(char*)((char*)this + 0x17c) == 9)
        {
            strcpy(localFrame + 0x44,
                (char*)this + 0x5b9);
            strcpy(localFrame + 0x62,
                EQ_GLOBAL(char*, 0x006E4DA8));
            *(u32*)(localFrame + 0x80) = 0xfffffffd;
            *(u32*)(localFrame + 0x88) = 0;
            send_message_guess(g_world_connection_guess, 0x2191, localFrame + 0x44, 0x48, 1);
        }
        else if (*(char*)((char*)this + 0x17c) == 0x14)
        {
            *(u32*)(localFrame + 0x8c) = 0;
            *(u32*)(localFrame + 0x90) =
                g_local_player_guess->spawn_id;
            *(u32*)(localFrame + 0x94) = 0;
            send_message_guess(g_world_connection_guess, 0x21ea, localFrame + 0x8c, 0xc, 1);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0055C4A4);
        }
        else if (*(char*)((char*)this + 0x17c) != 3)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0055C46C);
        }
        ((EQ_RequestorCallbackTable_guess*)this)->expire_guess();
    }
    else
    {
        EQ_RequestorRECT_guess* rect;
        SetRect_guess_t setRect;
        rect = (EQ_RequestorRECT_guess*)(localFrame + 0x98);
        setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
        setRect(rect, 0x1ba, 0, 0x280, 0x75);
        EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_RequestorRECT_guess*, int*, int), this)(*(void**)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4), rect,
             (int*)((char*)this + 0x24), 1);

        if ((char)((int*)this)[0x5f] == 6)
        {
            if (*(short*)((char*)this + 0xa4) == 0)
            {
                setRect(rect, 0x1eb, 0x138, 0x21a, 0x14a);
            }
            else
            {
                setRect(rect, 0x1bb, 0x138, 0x1ea, 0x14a);
            }
            EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_RequestorRECT_guess*, int*, int), this)(*(void**)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4), rect,
                 (int*)((char*)this + 0x80), 1);
        }
        else
        {
            if (*(short*)((char*)this + 0xa4) == 0)
            {
                setRect(rect, 0x1bb, 0x75, 0x1ea, 0x87);
            }
            else
            {
                setRect(rect, 0x21b, 0x75, 0x24a, 0x87);
            }
            EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_RequestorRECT_guess*, int*, int), this)(*(void**)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4), rect,
                 (int*)((char*)this + 0x80), 1);

            if (*(short*)((char*)this + 0xa4) == 1)
            {
                setRect(rect, 0x1eb, 0x75, 0x21a, 0x87);
            }
            else
            {
                setRect(rect, 0x24b, 0x75, 0x27a, 0x87);
            }
            EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_RequestorRECT_guess*, int*, int), this)(*(void**)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4), rect,
                 (int*)((char*)this + 0x90), 1);
        }

        EQ_METHOD(0, void, 0x004816EC, (void*), this)();
        return;
    }
    return;
}

// FUNCTION: TRILOGY 0x004816ec
// Displays wrapped request text, allowing a wider layout for sacrifice requests.
// ORIGINAL FILE: EverQuest/EQ_Requestor.cpp (from Mac debug map)
// @early-stop: characterOffset zero store (and, hoisted above add esp), maximumWidth/baseY slots swapped, lea/dec on text[splitStart] (folded with splitStart + 1), fiadd placed after the DAT push.
void EQ_Requestor::DrawText_guess()
{
    char lineText[256];
    char text[256];
    int maximumWidth;
    int baseY;
    int rowOffset;
    int x;
    int lineCount = 0;
    int characterOffset;
    int width = 0;
    size_t textLength;

    strcpy(text, (char*)this + 0x1b9);
    textLength = strlen(text);
    characterOffset = 0;
    if (*((char*)this + 0x17c) == 0x14)
    {
        maximumWidth = 0xac;
        x = (int)((float)*(int*)((char*)this + 0x24) + EQ_GLOBAL(float, 0x0053511C));
    }
    else
    {
        maximumWidth = 0x8e;
        x = (int)((float)*(int*)((char*)this + 0x24) + EQ_GLOBAL(float, 0x00535638));
    }
    baseY = (int)((float)*(int*)((char*)this + 0x28) + EQ_GLOBAL(float, 0x00535634));
    if ((short)textLength > 0)
    {
        do
        {
            if ((short)textLength <= 0)
                break;
            if (width > maximumWidth)
            {
                int splitStart;
                int lowerOffset;
                int splitCandidate;
                short splitLength;

                splitStart = (short)characterOffset;
                splitCandidate = 0;
                lowerOffset = splitStart - 10;
                if (splitStart > lowerOffset)
                {
                    do
                    {
                        if (((short)splitCandidate == 0) && (text[splitStart] == ' '))
                            splitCandidate = splitStart + 1;
                        splitStart = splitStart - 1;
                    } while (lowerOffset < splitStart);
                }
                splitLength = (short)splitCandidate;
                if ((short)splitCandidate == 0)
                    splitLength = (short)characterOffset;
                strcpy(lineText, EQ_ARRAY(char, 0x0058FBA4));
                strncpy(lineText, text, (size_t)splitLength);
                lineText[splitLength] = '\0';
                strcpy(text, text + splitLength);
                width = 0;
                characterOffset = 0;
                textLength = strlen(text);
                rowOffset = (short)lineCount * 0xf;
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), (void*)EQ_GLOBAL(void*, 0x006E4E68))(lineText, x,
                        (int)((float)rowOffset + (float)baseY),
                        0xa, EQ_GLOBAL(int, 0x006EFDFC), 1);
                lineCount = lineCount + 1;
            }
            if ('\0' < text[(short)characterOffset])
            {
                width = width + *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDE0) +
                    (int)text[(short)characterOffset] * 4);
            }
            characterOffset = characterOffset + 1;
        } while ((short)characterOffset < (short)textLength);
    }
    textLength = strlen(text);
    if (textLength > 0)
    {
        strcpy(lineText, text);
        rowOffset = (short)lineCount * 0xf;
        EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), (void*)EQ_GLOBAL(void*, 0x006E4E68))(lineText, x,
                (int)((float)rowOffset + (float)baseY),
                0xa, EQ_GLOBAL(int, 0x006EFDFC), 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x004818cb
// Acts on the player's yes/no answer to a requestor popup: bind, PvP, sacrifice, translocation, surname, etc.
// ORIGINAL FILE: EverQuest/EQ_Requestor.cpp (from Mac debug map)
void EQ_Requestor::ProcessButton_guess(short requestId, short buttonResult)
{
    RequestorProcessButtonFrame_guess frameStorage;
    RequestorProcessButtonFrame_guess& frame =
        *(RequestorProcessButtonFrame_guess*)((char*)&frameStorage + 4);
    char action;
    s32* coinCount;
    u32 messageOpcode;
    char* messageData;
    u32 messageSize;
    void* packetData;

    if (*(int*)((char*)this + 0xa0) != 0)
    {
        if (requestId != 0)
            return;
        *(short*)((char*)this + 0xa4) = buttonResult;
        return;
    }
    if (buttonResult == 0)
    {
        action = *(char*)((char*)this + 0x17c);
        if (action == '\0')
        {
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.initial_scratch.identity.message.player_name,
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.initial_scratch.identity.message.request_name, (char*)this + 0x5b9);
            frame.initial_scratch.identity.message.requestor_id = *(u16*)((char*)this + 0x176);
            frame.initial_scratch.identity.message.requestor_type = (u32)*(u8*)((char*)this + 0x174);
            messageData = frame.initial_scratch.identity.message.request_name;
            messageSize = 0x44;
            messageOpcode = 0x2118;
            goto send_request_message;
        }
        if (action == '\x01')
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x0055c604, 0xd);
            packetData = 0;
            messageSize = 0;
            messageOpcode = 0x21cf;
            goto send_packet_message;
        }
        if (action == '\x02')
        {
            packetData = (char*)this + 0xa8;
            messageSize = 0xa0;
            *(int*)((char*)this + 0x144) = 1;
            messageOpcode = 0x219b;
            goto send_packet_message;
        }
        if (action == '\x04')
        {
            CharacterProfile* profile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (profile->bind_zone, profile->zone);
            EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bind_location[0][1] =
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y;
            EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bind_location[1][1] =
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x;
            EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bind_location[2][1] =
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z;
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x0055c5e0, 4);
            send_message_guess(g_world_connection_guess, 0x2058, (char*)this + 0x148, 0x1c, 1);
            EQ_METHOD(2, void, 0x004C5E50, (void*, void*, int),
                EQ_GLOBAL(void*, 0x006EFE2C))((char*)this + 0x148, 1);
            EQ_GLOBAL(u32, 0x006B7120) = 7;
            EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int),
                EQ_GLOBAL(void*, 0x006EFE2C))(0, 1);
            goto request_callback;
        }
        else if (action == '\x05')
        {
            CharacterProfile* profile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
            if (profile->inventory_items[0] == 0)
            {
                if (*((char*)g_displayManager_guess + 0x3d) != '\0')
                {
                    frame.currency.negative_one = -1;
                    coinCount = &profile->platinum_cursor;
                    frame.currency.zero = 0;
                    if (*coinCount < 1)
                    {
                        coinCount = &profile->gold_cursor;
                        if (0 < profile->gold_cursor)
                        {
                            *(u32*)&frame.currency.denomination = 2;
                            goto selected_cursor_coin;
                        }
                        coinCount = &profile->silver_cursor;
                        if (0 < profile->silver_cursor)
                        {
                            *(u32*)&frame.currency.denomination = 1;
                            goto selected_cursor_coin;
                        }
                        if (0 < profile->copper_cursor)
                        {
                            *(u32*)&frame.currency.denomination = 0;
                            frame.currency.amount = profile->copper_cursor;
                        }
                    }
                    else
                    {
                        *(u32*)&frame.currency.denomination = 3;
selected_cursor_coin:
                        frame.currency.amount = *coinCount;
                    }
                    if (0 < frame.currency.amount)
                    {
                        *(u32*)&frame.currency.denomination_copy = *(u32*)&frame.currency.denomination;
                        send_message_guess(g_world_connection_guess, 0x212d, &frame.currency, 0x14, 1);
                        *((u8*)g_displayManager_guess + 0x3d) = 0;
                        profile->platinum_cursor = 0;
                        profile->gold_cursor = 0;
                        profile->silver_cursor = 0;
                        profile->copper_cursor = 0;
                    }
                }
            }
            else
            {
                frame.item_message[1] = -1;
                frame.item_message[0] = 0;
                frame.item_message[2] = 0;
                send_message_guess(g_world_connection_guess, 0x212c, frame.item_message, 0xc, 1);
                *((u8*)g_displayManager_guess + 0x3c) = 0;
                EQ_CDECL(void, 0x005218BB, (void*))(profile->inventory_items[0]);
                profile->inventory_items[0] = 0;
            }
            goto request_callback;
        }
        else
        {
            if (action == '\x08')
            {
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (frame.target.target_name, (char*)this + 0x5b9);
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (frame.target.player_name, EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name);
                frame.target.target_id = *(short*)((char*)this + 0x176);
                frame.target.target_status = 0;
                frame.target.target_level = *(int*)((char*)this + 0x170);
                messageData = frame.target.target_name;
                messageSize = 0x48;
                messageOpcode = 0x2193;
                goto send_request_message;
            }
            if (action != '\x09')
            {
                if (action == '\x0a')
                {
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (frame.initial_scratch.surname.message.player_name,
                            EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name);
                    frame.initial_scratch.surname.message.zero = 0;
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (frame.initial_scratch.surname.message.surname, (char*)this + 0x5d7);
                    send_message_guess(g_world_connection_guess, 0x21c4,
                            &frame.initial_scratch.surname.message, 0x38, 1);
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                        EQ_GLOBAL(void*, 0x006EFE2C))
                        ((const char*)0x0055c5ac, 0xf);
                    goto request_callback;
                }
                if (action == '\x0b')
                {
                    frame.final_message.target_level.spawn_id = *(u32*)((char*)this + 0x164);
                    frame.final_message.target_level.value_bits = *(u32*)((char*)this + 0x168);
                    send_message_guess(g_world_connection_guess, 0x21c2,
                            &frame.final_message.target_level.spawn_id, 8, 1);
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, double))
                        (frame.chat_text, (const char*)0x0055c58c,
                            (double)*(float*)&frame.final_message.target_level.value_bits);
                    messageData = frame.chat_text;
                    messageOpcode = 0xf;
                    goto display_request_chat;
                }
                if (action == '\x0c')
                {
                    frame.final_message.target_level.spawn_id = *(u32*)((char*)this + 0x164);
                    *(u8*)&frame.final_message.target_level.value_bits = *(u8*)((char*)this + 0x16c);
                    send_message_guess(g_world_connection_guess, 0x20d2,
                            &frame.final_message.target_level.spawn_id, 8, 1);
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, int))
                        (frame.chat_text, (const char*)0x0055c56c, *(int*)((char*)this + 0x16c));
                    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*),
                        EQ_GLOBAL(void*, 0x006EFE2C))(frame.chat_text);
                    goto request_callback;
                }
                if (action == '\x0d')
                {
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (frame.initial_scratch.kill.message.target_name, (char*)this + 0x17d);
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (frame.initial_scratch.kill.message.target_last_name, (char*)this + 0x19b);
                    send_message_guess(g_world_connection_guess, 0x206c,
                            &frame.initial_scratch.kill.message, 0x3d, 1);
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, const char*))
                        (frame.chat_text, (const char*)0x0055c560, (const char*)this + 0x17d);
                    messageData = frame.chat_text;
                    messageOpcode = 0xd;
                    goto display_request_chat;
                }
                if (action != '\x14')
                {
                    if (action != '\x15')
                    {
                        if (action == '\x16')
                        {
                            packetData = (void*)0x005921a8;
                            messageSize = 0x10;
                            *((int*)((char*)g_eqmain_guess + 0xa4)) = 0;
                            messageOpcode = 0x20a0;
                            *((int*)((char*)g_eqmain_guess + 0x94)) =
                                *((int*)((char*)g_eqmain_guess + 0x94)) + 1;
                            goto send_packet_message;
                        }
                        if (action == '\x17')
                        {
                            send_message_guess(g_world_connection_guess, 0x211a,
                                    &EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id, 2, 1);
                            messageData = (char*)0x0055c500;
                            messageOpcode = 0x111;
                            goto display_request_chat;
                        }
                        if (action != '\x19')
                            goto request_callback;
                        packetData = frame.initial_scratch.translocation_message;
                        messageSize = 0x58;
                        *(u32*)(frame.initial_scratch.translocation_message + 0x54) = 1;
                        messageOpcode = 0x2206;
                        goto send_packet_message;
                    }
                    EQ_METHOD(1, void, 0x004CB7B8, (void*, int),
                        EQ_GLOBAL(void*, 0x006EFE2C))(0);
                    goto request_callback;
                }
                frame.final_message.sacrifice.zero = 0;
                frame.final_message.sacrifice.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                *(u32*)&frame.final_message.sacrifice.value = 1;
                send_message_guess(g_world_connection_guess, 0x21ea, &frame.final_message.sacrifice, 0xc, 1);
                messageData = (char*)0x0055c540;
                messageOpcode = 0x111;
                goto display_request_chat;
            }
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.target.target_name, (char*)this + 0x5b9);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.target.player_name, EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name);
            frame.target.target_id = *(short*)((char*)this + 0x176);
            frame.target.target_status = 0;
            frame.target.target_level = *(int*)((char*)this + 0x170);
            messageData = frame.target.target_name;
            messageSize = 0x48;
            messageOpcode = 0x2194;
            goto send_request_message;
        }
send_packet_message:
        send_message_guess(g_world_connection_guess, messageOpcode, packetData, messageSize, 1);
        goto request_callback;
send_request_message:
        send_message_guess(g_world_connection_guess, messageOpcode, messageData, messageSize, 1);
        goto request_callback;
display_request_chat:
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
            EQ_GLOBAL(void*, 0x006EFE2C))(messageData, messageOpcode);
        goto request_callback;
request_callback:
        ((EQ_RequestorCallbackTable_guess*)this)->expire_guess();
    }
    if (buttonResult != 1)
        return;
    action = *(char*)((char*)this + 0x17c);
    if (action == '\0')
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.initial_scratch.identity.message.player_name,
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.initial_scratch.identity.message.request_name, (char*)this + 0x5b9);
            frame.initial_scratch.identity.message.requestor_id = *(u16*)((char*)this + 0x176);
            frame.initial_scratch.identity.message.requestor_type = 5;
            messageData = frame.initial_scratch.identity.message.request_name;
        messageSize = 0x44;
        messageOpcode = 0x2118;
    }
    else
    {
        if (action == '\x02')
        {
            *(int*)((char*)this + 0x144) = 0;
            send_message_guess(g_world_connection_guess, 0x219b, (char*)this + 0xa8, 0xa0, 1);
            goto button_callback;
        }
        if (action == '\x08' || action == '\x09')
        {
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.target.target_name, (char*)this + 0x5b9);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (frame.target.player_name, EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name);
            frame.target.target_id = *(short*)((char*)this + 0x176);
            frame.target.target_status = -2;
            frame.target.target_level = *(int*)((char*)this + 0x170);
            messageData = frame.target.target_name;
            messageSize = 0x48;
            if (action == '\x08')
                messageOpcode = 0x2193;
            else
                messageOpcode = 0x2194;
        }
        else
        {
            if (action != '\x14')
            {
                if (action == '\x19')
                {
                    *(u32*)(frame.initial_scratch.translocation_message + 0x54) = 0xffffffff;
                    send_message_guess(g_world_connection_guess, 0x2206,
                            frame.initial_scratch.translocation_message, 0x58, 1);
                    messageData = (char*)0x0055c4d0;
                    messageOpcode = 0x111;
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                        EQ_GLOBAL(void*, 0x006EFE2C))(messageData, messageOpcode);
                    goto button_callback;
                }
                goto button_callback;
            }
            frame.final_message.sacrifice.zero = 0;
            frame.final_message.sacrifice.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
            frame.final_message.sacrifice.value = 0.0f;
            send_message_guess(g_world_connection_guess, 0x21ea, &frame.final_message.sacrifice, 0xc, 1);
            messageData = (char*)0x0055c4a4;
            messageOpcode = 0x111;
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                EQ_GLOBAL(void*, 0x006EFE2C))(messageData, messageOpcode);
            goto button_callback;
        }
    }
    send_message_guess(g_world_connection_guess, messageOpcode, messageData, messageSize, 1);
button_callback:
    ((EQ_RequestorCallbackTable_guess*)this)->expire_guess();
    return;
}

struct EQ_RequestorButtonsLayout_guess
{
    char reserved_000[0x10];
    int button_state;
    char reserved_014[0x10];
    EQ_RequestorRECT_guess window_rectangle;
    char reserved_034[0x3c];
    int button_list;
    char reserved_074[0x0c];
    EQ_RequestorRECT_guess first_button_rectangle;
    EQ_RequestorRECT_guess second_button_rectangle;
};

typedef int (__stdcall *EQ_RequestorSetRect_guess_t)(EQ_RequestorRECT_guess*, int, int, int, int);

// FUNCTION: TRILOGY 0x0048201d
// Centers the requestor prompt and places its two buttons.
void __fastcall LayoutRequestorButtons_guess(EQ_RequestorButtonsLayout_guess* requestor)
{
    register EQ_RequestorSetRect_guess_t setRect;
    int buttonBottom;

    requestor->button_state = 1;
    EQ_METHOD(0, void, 0x00450463, (void*), requestor)();
    requestor->button_list = 0;
    setRect = EQ_GLOBAL(EQ_RequestorSetRect_guess_t, 0x00534248);
    setRect(&requestor->window_rectangle,
        ((int)EQ_GLOBAL(short, 0x006B63E4) - (int)EQ_GLOBAL(short, 0x006B63E0)) / 2 - 99 + (int)EQ_GLOBAL(short, 0x006B63E0),
        ((int)EQ_GLOBAL(short, 0x006B63E6) - (int)EQ_GLOBAL(short, 0x006B63E2)) / 2 - 0x3a + (int)EQ_GLOBAL(short, 0x006B63E2),
        ((int)EQ_GLOBAL(short, 0x006B63E4) - (int)EQ_GLOBAL(short, 0x006B63E0)) / 2 - 99 + (int)EQ_GLOBAL(short, 0x006B63E0) + 200,
        ((int)EQ_GLOBAL(short, 0x006B63E6) - (int)EQ_GLOBAL(short, 0x006B63E2)) / 2 - 0x3a + (int)EQ_GLOBAL(short, 0x006B63E2) + 0x96);
    buttonBottom = requestor->window_rectangle.top + 0x65;
    setRect(&requestor->first_button_rectangle,
        requestor->window_rectangle.left + 0x10,
        requestor->window_rectangle.top + 0x54,
        requestor->window_rectangle.left + 0x3e,
        buttonBottom);
    EQ_METHOD(6, void, 0x004503BE, (void*, int, EQ_RequestorRECT_guess*, int, int, int, int), requestor)
        (0, &requestor->first_button_rectangle, -1, -1, 0, 0);
    setRect(&requestor->second_button_rectangle,
        requestor->window_rectangle.left + 0x87,
        requestor->window_rectangle.top + 0x54,
        requestor->window_rectangle.left + 0xb5,
        buttonBottom);
    EQ_METHOD(6, void, 0x004503BE, (void*, int, EQ_RequestorRECT_guess*, int, int, int, int), requestor)
        (1, &requestor->second_button_rectangle, -1, -1, 0, 0);
}

// FUNCTION: TRILOGY 0x004820ed
// Resets the requestor: clears its +0x10 active flag, action/id fields and text; sets 0x006E4E44+0xEB8 = 1.
void __fastcall ResetRequestor_guess(EQ_RequestorInitialization_guess* requestor)
{
    requestor->cleared_value_010 = 0;
    EQ_GLOBAL(EQ_RequestorManagerState_guess*, 0x006E4E44)->requestor_enabled = 1;
    requestor->cleared_byte_174 = 0;
    requestor->cleared_word_176 = 0;
    requestor->cleared_action_17c = 0;
    requestor->cleared_value_178 = 0;
    strcpy(requestor->requestor_name, EQ_ARRAY(char, 0x0058FBA4));
}

// FUNCTION: TRILOGY 0x00482123
// Initializes the options screen overlay and registers its two controls.
CharacterSelector_guess::CharacterSelector_guess()
{
    register SetRect_guess_t setRect;
    register int buttonMinusOne;

    *(u32*)this = 0x0053563c;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    *(u8*)((char*)this + 0xa6) = 0;
    *(u16*)((char*)this + 0xa8) = 0;
    strcpy((char*)this + 0xb1, EQ_ARRAY(char, 0x0058FBA4));
    strcpy((char*)this + 0x1b1, EQ_ARRAY(char, 0x0058FBA4));
    setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
    *(u8*)((char*)this + 0xb0) = 0;
    *(u32*)((char*)this + 0xac) = 0;
    setRect((EQ_RequestorRECT_guess*)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    setRect((EQ_RequestorRECT_guess*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0053E028));
    EQ_METHOD(2, void, 0x00450168, (void*, int, void*), this)(1, *(void**)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 4));
    *(u8*)((char*)this + 0xa6) |= 0xff;
    *(short*)((char*)this + 0xa4) = -1;
    *(u32*)((char*)this + 0x78) = 0;
    *(u32*)((char*)this + 0xa0) = 0;
    setRect((EQ_RequestorRECT_guess*)((char*)this + 0x80), 0x97, 0x86, 0xc5, 0x97);
    setRect((EQ_RequestorRECT_guess*)((char*)this + 0x90), 0x10e, 0x86, 0x13c, 0x97);
    buttonMinusOne = (int)setRect;
    buttonMinusOne |= -1;
    EQ_METHOD(6, void, 0x004503BE, (void*, int, EQ_RequestorRECT_guess*, int, int, int, int), this)(0, (EQ_RequestorRECT_guess*)((char*)this + 0x80),
            buttonMinusOne, buttonMinusOne, 0, 0);
    EQ_METHOD(6, void, 0x004503BE, (void*, int, EQ_RequestorRECT_guess*, int, int, int, int), this)(1, (EQ_RequestorRECT_guess*)((char*)this + 0x90),
            buttonMinusOne, buttonMinusOne, 0, 0);
}

class EQ_Requestor2_guess
{
public:
    void DrawText_guess();
};

// FUNCTION: TRILOGY 0x0048226c
// Draws the second request dialog while active and invokes its timeout handler when overdue.
// ORIGINAL FILE: EverQuest/EQ_Requestor.cpp (from Mac debug map)
void EQ_Requestor2::draw_guess()
{
    char rects[0x20];
    register u32 currentTime;
    register SetRect_guess_t setRect;

    if (((int*)this)[4] != 0)
    {
        currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
        if (((u32*)this)[0x2b] < currentTime)
        {
            ((EQ_RequestorCallbackTable_guess*)this)->expire_guess();
        }
        else
        {
            setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
            setRect((EQ_RequestorRECT_guess*)(rects + 0x14), 0x164, 0xa2, 0x22a, 0x117);
            setRect((EQ_RequestorRECT_guess*)(rects + 4), 0x87, 0x32, 0x280, 0x1e0);
            EQ_THISCALL(void, 0x00450125,
                (void*, int, void*, EQ_RequestorRECT_guess*, EQ_RequestorRECT_guess*, int))
                (this, 0, *(void**)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 4),
                    (EQ_RequestorRECT_guess*)(rects + 0x14),
                    (EQ_RequestorRECT_guess*)(rects + 4), 1);
            if (*(short*)((char*)this + 0xa4) == 0)
            {
                setRect((EQ_RequestorRECT_guess*)(rects + 0x14), 0x165, 0x117, 0x194, 0x129);
                EQ_THISCALL(void, 0x00450125,
                    (void*, int, void*, EQ_RequestorRECT_guess*, void*, int))
                    (this, 0, *(void**)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 4),
                        (EQ_RequestorRECT_guess*)(rects + 0x14), (void*)((char*)this + 0x80), 1);
            }
            if (*(short*)((char*)this + 0xa4) == 1)
            {
                setRect((EQ_RequestorRECT_guess*)(rects + 0x14), 0x195, 0x117, 0x1c4, 0x129);
                EQ_THISCALL(void, 0x00450125,
                    (void*, int, void*, EQ_RequestorRECT_guess*, void*, int))
                    (this, 0, *(void**)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 4),
                        (EQ_RequestorRECT_guess*)(rects + 0x14), (void*)((char*)this + 0x90), 1);
            }
            EQ_THISCALL(void, 0x0048236F, (void*, int))(this, 0);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0048236f
// Displays wrapped text at a fixed screen position for the second request dialog.
// ORIGINAL FILE: EverQuest/EQ_Requestor.cpp (from Mac debug map)
// @early-stop: scan indexing still adds MOVSX EBX,AX; EQ_THISCALL adds two dummy-EDX zeroes.
void EQ_Requestor2_guess::DrawText_guess()
{
    size_t textLength;
    int width;
    short characterOffset;
    int lineCount;
    char text[256];
    char lineText[256];

    width = 0;
    lineCount = 0;
    strcpy(text, (char*)((char*)this + 0xb1));
    textLength = strlen(text);
    characterOffset = 0;
    if ((short)textLength > 0)
    {
        do
        {
            if ((short)textLength > 0)
            {
                if (width > 0x8e)
                {
                    int splitStart;
                    int splitCandidate;
                    int lowerOffset;
                    short splitLength;

                    splitStart = (short)characterOffset;
                    splitCandidate = 0;
                    lowerOffset = splitStart - 10;
                    if (splitStart > lowerOffset)
                    {
                        do
                        {
                            if (((short)splitCandidate == 0) && (text[(short)splitStart] == ' '))
                                splitCandidate = splitStart + 1;
                            splitStart = splitStart - 1;
                        } while (lowerOffset < splitStart);
                        splitLength = (short)splitCandidate;
                        if ((short)splitCandidate == 0)
                            splitLength = (short)characterOffset;
                    }
                    else
                    {
                        splitLength = (short)characterOffset;
                    }
                    strcpy(lineText, EQ_ARRAY(char, 0x0058FBA4));
                    strncpy(lineText, text, (size_t)splitLength);
                    lineText[splitLength] = '\0';
                    strcpy(text, text + splitLength);
                    width = 0;
                    characterOffset = 0;
                    textLength = strlen(text);
                    EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), (void*)EQ_GLOBAL(void*, 0x006E4E68))(lineText, 0x97,
                            (short)lineCount * 0xf + 0x41,
                            10, EQ_GLOBAL(int, 0x006EFDFC), 1);
                    lineCount = lineCount + 1;
                }
                if ('\0' < text[(short)characterOffset])
                {
                    width = width + *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDE0) +
                        (int)text[(short)characterOffset] * 4);
                }
            }
            else
                break;
            characterOffset = characterOffset + 1;
        } while ((short)characterOffset < (short)textLength);
    }
    textLength = strlen(text);
    if (textLength > 0)
    {
        strcpy(lineText, text);
        EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), (void*)EQ_GLOBAL(void*, 0x006E4E68))(lineText, 0x97,
                (short)lineCount * 0xf + 0x41,
                10, EQ_GLOBAL(int, 0x006EFDFC), 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x004824F1
// Handles a UI event; the specific game action is unclear.
// __thiscall (ECX = obj, callee pops 8), written as __fastcall with an EDX dummy: VC6 rejects __thiscall on
// free functions. Owner class unknown. When obj+0xA0 is 0: value == 0 sets CHAR_SELECT_SCREEN+0x9c = 1 and calls
// vtable slot 2, then value == 1 calls slot 2 again. Otherwise, when flag == 0, stores value at obj+0xA4.
void __fastcall CharacterSelector_handleCharacterSelection_guess(int* obj, int unusedEdx,
                                                                 short flag, short value)
{
    class CharacterSelectorVtable_guess
    {
    public:
        virtual void unused0_guess(void) {}
        virtual void unused1_guess(void) {}
        virtual void invoke_guess(void) {}
    };

    if (obj[0x28] != 0)
    {
        if (flag == 0)
        {
            *(short*)(obj + 0x29) = value;
        }
    }
    else
    {
        if (value == 0)
        {
            *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x9c) = 1;
            ((CharacterSelectorVtable_guess*)obj)->invoke_guess();
        }
        if (value == 1)
        {
            ((CharacterSelectorVtable_guess*)obj)->invoke_guess();
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0048254b
// Resets a requestor's text and interaction state.
void __fastcall ClearRequestorState_guess(EQ_RequestorClearState_guess* requestor)
{
    requestor->cleared_value_010 = 0;
    requestor->cleared_byte_a6 = 0;
    requestor->cleared_word_a8 = 0;
    requestor->cleared_byte_b0 = 0;
    requestor->cleared_value_ac = 0;
    strcpy(requestor->text_b1,
                                                     EQ_ARRAY(char, 0x0058FBA4));
    return;
}
