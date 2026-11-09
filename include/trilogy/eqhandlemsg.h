/*
 * eqhandlemsg.h - the functions of the EverQuest/EQHandleMsg subsystem (Mac: Source/PC Source/EverQuest/
 * EQHandleMsg.cpp, by the queue's call-graph labels): message handlers called from
 * CEverQuest::HandleWorldMessage (0x00496743) and their helpers. Addresses and name sources are in
 * src/EverQuest/EQHandleMsg.cpp.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQHANDLEMSG_H
#define TRILOGY_EQHANDLEMSG_H

#include "common.h"
#include "eqaddr.h"

struct MsgHPUpdate;
struct MsgLevelUpdate;
struct MsgMoveSpawn;

/* Empty methods the linker shares between classes (identical COMDATs folded): HandleWorldMessage calls them
 * on the EQ_PC object, the old UI on its windows; 0x00480E87 also sits in two vtables (0x00535818,
 * 0x00535820). `this` is never read. */
class EQFoldedStubs_guess
{
public:
    void Nop_guess(int unused);                                          /* 0x0040240F */
    int ReturnZero_guess(int unused1, int unused2, int unused3);         /* 0x00480E87 */
};

/* zlib 1.1.2's z_stream (0x38 bytes; library code, linked from zlib at 0x004F8430..). Layout from zlib.h. */
typedef struct ZStream_guess
{
    u8*     next_in;            /* 0x00 */
    u32     avail_in;           /* 0x04 */
    u32     total_in;           /* 0x08 */
    u8*     next_out;           /* 0x0C */
    u32     avail_out;          /* 0x10 */
    u32     total_out;          /* 0x14 */
    char*   msg;                /* 0x18 */
    void*   state;              /* 0x1C */
    void*   zalloc;             /* 0x20 */
    void*   zfree;              /* 0x24 */
    void*   opaque;             /* 0x28 */
    int     data_type;          /* 0x2C */
    u32     adler;              /* 0x30 */
    u32     reserved;           /* 0x34 */
} ZStream_guess;
C_ASSERT(sizeof(ZStream_guess) == 0x38);
/* 0x00596378  the z_stream DecompressBuffer uses. */
#define g_inflate_stream_guess      EQ_ARRAY(ZStream_guess, 0x00596378)
/* zlib 1.1.2 (library): inflateInit2_ 0x004F84D0, inflate 0x004F8600, inflateEnd 0x004F8480. */
#define inflateInit2__guess         EQ_CDECL(int, 0x004F84D0, (ZStream_guess *, int, const char *, int))
#define inflate_guess               EQ_CDECL(int, 0x004F8600, (ZStream_guess *, int))
#define inflateEnd_guess            EQ_CDECL(int, 0x004F8480, (ZStream_guess *))

u32 DecompressBuffer_guess(u8* in, u8* out, u32 inLen, u32 outSize);       /* 0x0049100C */
int stripfirstword(char* text);                                     /* 0x004911D3 */
void msg_move_spawn_guess(struct MsgMoveSpawn* msg);                      /* 0x00491B5D */
void msg_vehicle_reset(void);                                       /* 0x0049341C */
int UseItemCharges_guess(int slot, int flag, int delta);                  /* 0x00491DD7 */
void ReportMissingComponents_guess(int unused1, int unused2, const u8* message);      /* __cdecl, 0x0049231B */
bool ParseNextLine_guess(char* text, char* line, u32* pos, int lineSize);  /* 0x00491137 */
void msg_hp_update_guess(struct MsgHPUpdate* msg);                        /* 0x004954F6 */
void HandleSkillIncrease_guess(const s32* message);                               /* __cdecl, 0x0049553F */
void HandlePoisonApplicationResult_guess(const s32* message);                               /* __cdecl, 0x004955B1 */
void msg_ability_change(const u32* message);                         /* 0x00495808 */
void msg_sacrifice(const u32* message);                              /* 0x004958B9 */
void SetPlayerMoney_guess(s32* coins);                                    /* 0x004960AE */
void msg_consent_verify(int message);                                /* 0x004966B6 */
void __fastcall HandleGroupInviteResponse_guess(void* receiver, int unusedEdx, int message); /* thiscall adapter, 0x004CAEFB */
int ExchangeField618_guess(char* object, int value);                      /* 0x004F46A0 */
void msg_level_update_guess(void* connection, u32 opcode, struct MsgLevelUpdate* msg, u32 size); /* 0x0049494E */
void fillEvilProcessNames_guess(short* msg);                              /* 0x004E87FD */

#endif /* TRILOGY_EQHANDLEMSG_H */
