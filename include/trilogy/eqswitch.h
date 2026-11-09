/*
 * eqswitch.h - EQSwitch, a door / lever / platform (Mac: Source/PC Source/EverQuest/EQSwitch.cpp).
 *
 * The live switches form a singly linked list whose head is *(EQSwitch**)0x006E4D78.
 * Layout: from a layout JSON (not published; 0x90 bytes; offsets are what the code reads, the names
 * are guesses).
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQSWITCH_H
#define TRILOGY_EQSWITCH_H

#include "common.h"
#include "eqaddr.h"

/* Five dwords copied as a block by ResetSwitchPosition (position + heading + one more). */
typedef struct SwitchPos_guess
{
    union
    {
        f32     y;                  /* 0x00 */
        unsigned int y_bits_guess;       /* 0x000* */
    };
    union
    {
        f32     x;                  /* 0x04 */
        unsigned int x_bits_guess;       /* 0x004* */
    };
    union
    {
        f32     z;                  /* 0x08 */
        unsigned int z_bits_guess;       /* 0x008* */
    };
    union
    {
        f32     heading;            /* 0x0C */
        unsigned int heading_bits_guess; /* 0x00C* */
    };
    union
    {
        f32     auxiliary_guess;        /* 0x10 */
        unsigned int auxiliary_bits_guess; /* 0x010* */
    };
} SwitchPos_guess;
C_ASSERT(sizeof(SwitchPos_guess) == 0x14);

struct EQSwitch
{
    unsigned char objectKind_guess;   /* 0x00* */
    u8      unknown0x01[3];     /* 0x01  */
    EQSwitch* prev_guess;         /* 0x04* */
    EQSwitch* next;             /* 0x08  list link (head: g_switchList_guess) */
    u8      id_guess;           /* 0x0C  sent as the door id in 0x210F */
    char    name_guess[0xF];    /* 0x0D* */
    u8      type;               /* 0x1C  door/lever kind: speed, damage and timing switch on it */
    char    state;              /* 0x1D  0 closed, 1 open, 2/3 moving (guess) */
    u8      unknown0x1E[2];     /* 0x1E */
    SwitchPos_guess default_pos;/* 0x20 */
    SwitchPos_guess pos;        /* 0x34 */
    unsigned char defaultState_guess; /* 0x48* */
    unsigned char toggleInputOnUse_guess; /* 0x49* */
    u8      triggerMode_guess;        /* 0x4A  type 0x35: state-change time is (this == 0) */
    u8      unknown0x4B[1];     /* 0x4B  */
    u8      requiredSkill_guess; /* 0x4C* */
    char    linkedInputSlots_guess[5]; /* 0x4D* */
    char    inputStates_guess[5]; /* 0x52* */
    char    requiredInputStates_guess[5]; /* 0x57* */
    u8      unknown0x5C[2];     /* 0x5C  */
    unsigned short keyItem_guess;      /* 0x5E* */
    u8      unknown0x60[2];     /* 0x60  */
    s16     param_guess;        /* 0x62  -2/-3: no state-change time; types 0x28/0x2D: time if > 0 */
    char    linkedSwitchIds_guess[5]; /* 0x64* */
    u8      unknown0x69[0x17];  /* 0x69  */
    void*   actor;              /* 0x80  matched by FindSwitchByActor_guess */
    u32     stateChangeTime_guess; /* 0x84* */
    float   currentSpeed_guess; /* 0x88* */
    u8      unknown0x8C[4];     /* 0x8C  */

    /* 0x004BDCA8  name: manual (FUN_004bdca8). Called on the list head; walks the global list. */
    EQSwitch* FindSwitchByActor_guess(void* actor);
    /* 0x004BDCC7  TAKP name evidence is low; walks the global list by the byte id. */
    EQSwitch* FindSwitchById_guess(u8 id);
    /* 0x004BDCE3  name: takp (descriptive). */
    void    ResetSwitchPosition_guess(u8 mode);
    /* 0x004BDD62  name: mac. Top speed by type; *pAccel gets the acceleration. */
    float   TopSpeed_guess(float* pAccel);
    /* 0x004BDDF1  name: takp (descriptive). Milliseconds until the switch changes state back. */
    int     GetSwitchStateChangeTime_guess();
    /* 0x004BDEE6  name: mac. */
    int     GetSwitchDamage();
    /* 0x004BE42C  TAKP name evidence is low; damage and signed-short input gate. */
    char    GetSwitchDamage2_guess(s16 value);
    /* EQSwitch.cpp (was the file-local EQSwitch view). */
    void    ChangeState(char state, int unused);
    bool    SwitchWasOpened(short keyItem, short skill, void* player);
    void    UseSwitch(int playerID, short packetArg, short value);
};
C_ASSERT(sizeof(EQSwitch) == 0x90);
C_ASSERT(offsetof(EQSwitch, type) == 0x1C);
C_ASSERT(offsetof(EQSwitch, default_pos) == 0x20);
C_ASSERT(offsetof(EQSwitch, pos) == 0x34);
C_ASSERT(offsetof(EQSwitch, triggerMode_guess) == 0x4A);
C_ASSERT(offsetof(EQSwitch, param_guess) == 0x62);
C_ASSERT(offsetof(EQSwitch, actor) == 0x80);
C_ASSERT(offsetof(EQSwitch, objectKind_guess) == 0x00);
C_ASSERT(offsetof(EQSwitch, prev_guess) == 0x04);
C_ASSERT(offsetof(EQSwitch, name_guess) == 0x0D);
C_ASSERT(offsetof(EQSwitch, defaultState_guess) == 0x48);
C_ASSERT(offsetof(EQSwitch, toggleInputOnUse_guess) == 0x49);
C_ASSERT(offsetof(EQSwitch, requiredSkill_guess) == 0x4C);
C_ASSERT(offsetof(EQSwitch, linkedInputSlots_guess) == 0x4D);
C_ASSERT(offsetof(EQSwitch, inputStates_guess) == 0x52);
C_ASSERT(offsetof(EQSwitch, requiredInputStates_guess) == 0x57);
C_ASSERT(offsetof(EQSwitch, keyItem_guess) == 0x5E);
C_ASSERT(offsetof(EQSwitch, linkedSwitchIds_guess) == 0x64);
C_ASSERT(offsetof(EQSwitch, stateChangeTime_guess) == 0x84);
C_ASSERT(offsetof(EQSwitch, currentSpeed_guess) == 0x88);

#define g_switchList_guess  EQ_GLOBAL(EQSwitch*, 0x006E4D78)

#endif /* TRILOGY_EQSWITCH_H */
