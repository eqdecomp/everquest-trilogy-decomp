/*
 * item.h - items as the Trilogy client keeps them in memory.
 *
 * The client allocates an item object and memcpy()s the network message over it, so the in-memory
 * layout and the wire layout are the same bytes (no vtable at offset 0):
 *   EQ_Equipment  operator new(0x124), memcpy 0x124  (OP_PlayerItem handler, 0x00496743 case 0x2164;
 *                 CEverQuest::msg_snd_ieq 0x0049509B; OP_ItemTradeIn 0x00496213)
 *   EQ_Container  operator new(0xDC),  memcpy 0xDC   (OP_PlayerContainer, CEverQuest::msgStartIcon 0x00494AF1)
 *   EQ_Note       operator new(0xD0),  memcpy 0xD0   (OP_PlayerBook, CEverQuest::msgStartInotes 0x00494D6B)
 * OP_CompressedItems (0x21F6, CEverQuest::HandleCompressedItems 0x00495456) packs entries of 0x126
 * bytes: a u16 opcode followed by 0x124 bytes of item, whatever its kind.
 * All three share the first 0xAC bytes (EQ_Item). The type byte at 0x81 selects the kind
 * (0 = equipment, 1 = container, 2 = note/book); the client tests it everywhere.
 *
 * Sources: EQClassic Item_Struct (292 bytes, SHORT_CONTAINER 220, SHORT_BOOK 208) agrees with the
 * client on all three sizes. ShowEQ 3.0.4 itemStruct (244), containerPlayerStruct (216) and
 * bookPlayerStruct (205) do not: ShowEQ has the stats block 4 bytes early (STR at 168; the client
 * reads DEX at 0xAF = 175, EQClassic's offset) and ends early.
 */
#ifndef TRILOGY_ITEM_H
#define TRILOGY_ITEM_H

#include "common.h"

#pragma pack(push, 1)

/* Item kinds, EQ_Item.type (0x81). */
#define EQ_ITEM_EQUIPMENT   0
#define EQ_ITEM_CONTAINER   1
#define EQ_ITEM_NOTE        2

/* Common item header, 0xAC bytes. Verified against client (fields marked *): EQClassic layout,
 * ShowEQ agrees up to 0x90. */
typedef struct EQ_Item
{
    char    name[35];           /* 0x00  printed with %s from the item pointer */
    char    lore[60];           /* 0x23  */
    char    idfile[6];          /* 0x5F* "IT63": 0x004C71A3 atoi()s idfile+2 of the held weapons */
    s16     flag;               /* 0x65  */
    u8      unknown0x67[22];    /* 0x67  */
    u8      weight;             /* 0x7D  */
    s8      norent;             /* 0x7E  */
    s8      nodrop;             /* 0x7F  */
    u8      size;               /* 0x80  */
    u8      type;               /* 0x81* EQ_ITEM_EQUIPMENT / _CONTAINER / _NOTE */
    s16     item_nr;            /* 0x82* "Got a bogus item" when 0 (or < 1000 for equipment) */
    s16     icon_nr;            /* 0x84* reset to 500 on "BAD IMAGE NUMBER" */
    s16     equip_slot;         /* 0x86* inventory slot the item goes to (0..29, 30..339 bags,
                                            2000..2007 bank, 2030.. bank bags) */
    u32     equipable_slots;    /* 0x88  */
    s32     cost;               /* 0x8C* EQ_Item::ValueBuy (0x00438550) */
    u8      unknown0x90[16];    /* 0x90  */
    u32     unique_id;          /* 0xA0* matched against the id in OP 0x21AE/0x21B4 deletes */
    s8      unknown0xA4;        /* 0xA4  EQClassic: second no-drop flag */
    u8      unknown0xA5[7];     /* 0xA5  */
#ifdef __cplusplus
    /* EQ_Item methods (src/EverQuest/EQ_Item.cpp), __thiscall. Not seen by build_gdt.py (C). */
    s16 ValueSRent_plat_guess(void);                    /* 0x004384E5 */
    s16 ValueSRent_gold_guess(void);                    /* 0x004384FC */
    s16 ValueSRent_silver_guess(void);                  /* 0x0043851B */
    s16 ValueSRent_copper_guess(void);                  /* 0x00438538 */
    int ValueBuy(float rate, int count);          /* 0x00438550 */
    int ValueSell_guess(float rate, int count);         /* 0x004385A7 */
#endif
} EQ_Item;                      /* 0xAC */
C_ASSERT(sizeof(EQ_Item) == 0xAC);
C_ASSERT(offsetof(EQ_Item, idfile) == 0x5F);
C_ASSERT(offsetof(EQ_Item, type) == 0x81);
C_ASSERT(offsetof(EQ_Item, item_nr) == 0x82);
C_ASSERT(offsetof(EQ_Item, equip_slot) == 0x86);
C_ASSERT(offsetof(EQ_Item, cost) == 0x8C);
C_ASSERT(offsetof(EQ_Item, unique_id) == 0xA0);

/* The 10 bytes at 0xD8 that the player profile saves per inventory slot (CharacterProfile
 * .inv_props etc.; 0x004C71A3 copies item+0xD8, 10 bytes, when the item is not a container).
 * Verified against client (charges at +2, click effect at +4); EQClassic ItemProperties_Struct. */
typedef struct EQ_ItemProperties
{
    u8      unknown0x00;        /* 0x00  EQ_Equipment.click_effect_type */
    u8      unknown0x01;        /* 0x01  */
    s8      charges;            /* 0x02  -1 = unlimited */
    u8      unknown0x03[3];     /* 0x03  */
    u16     unknown0x06;        /* 0x06* EQ_PC::RefitNewbieEQ (0x004806CB) sets 1 */
    u8      unknown0x08[2];     /* 0x08  */
} EQ_ItemProperties;            /* 0x0A */
C_ASSERT(sizeof(EQ_ItemProperties) == 0x0A);

/* EQ_Equipment: 0x124 bytes. Verified against client: size (operator new/memcpy 0x124) and the
 * fields marked *. Layout from EQClassic Item_Struct.common. */
typedef struct EQ_Equipment
{
    EQ_Item item;               /* 0x00  */
    s8      STR;                /* 0xAC  */
    s8      STA;                /* 0xAD  */
    s8      CHA;                /* 0xAE  */
    s8      DEX;                /* 0xAF* EQ_Character::Dex (0x0041B48F) */
    s8      INT;                /* 0xB0* EQ_Character::Int (0x0041B5CC) */
    s8      AGI;                /* 0xB1  */
    s8      WIS;                /* 0xB2  */
    s8      MR;                 /* 0xB3  */
    s8      FR;                 /* 0xB4  */
    s8      CR;                 /* 0xB5  */
    s8      DR;                 /* 0xB6  */
    s8      PR;                 /* 0xB7  */
    s8      HP;                 /* 0xB8  */
    s8      MANA;               /* 0xB9* EQ_Character::Max_Mana (0x00419BCA) */
    s8      AC;                 /* 0xBA* EQ_Character::ac (0x00419D0F) */
    s8      stackable;          /* 0xBB* signed: movsx everywhere (charges / it), `> 0` in Combine
                                            (0x004A1D83) and EQ_ItemData::draw (jle) */
    u8      unknown0xBC;        /* 0xBC  */
    u8      light;              /* 0xBD* EQ_Character::DoLight (0x00419826) */
    u8      delay;              /* 0xBE  */
    s8      damage;             /* 0xBF* EQ_Equipment::IsWeapon (0x0043624B): weapon if != 0 */
    s8      click_type;         /* 0xC0  */
    u8      range;              /* 0xC1  */
    u8      item_type;          /* 0xC2* skill/type: 13 = rejected as bogus, 20..22 = scroll
                                            (IsScroll 0x00436270); type 14 items also count for
                                            stats from the general inventory (Dex, Int) */
    u8      magic;              /* 0xC3  */
    s8      click_level2;       /* 0xC4  */
    u8      material;           /* 0xC5  */
    u8      unknown0xC6[2];     /* 0xC6  */
    u32     color;              /* 0xC8  */
    u8      unknown0xCC[2];     /* 0xCC  */
    short   proc_spell_id;      /* 0xCE  */
    union
    {
        u32     classes;            /* 0xD0  class bitmask */
        u16     classes_u16_guess;  /* 0xD0  */
    };
    union
    {
        u32     races;              /* 0xD4  race bitmask */
        u16     races_u16_guess;    /* 0xD4  */
    };
    u8      click_effect_type;  /* 0xD8* nonzero + charges 0 = "BAD CHARGES ON STACKABLE" */
    u8      level;              /* 0xD9  */
    u8      charges;            /* 0xDA* unsigned: the client reads it movzx (45 sites, no movsx) */
    u8      effect_type;        /* 0xDB  */
    u16     click_effect_id;    /* 0xDC* set to 0xFFFF by HandleWorldMessage (jump case 0x43) */
    u8      unknown0xDE[2];     /* 0xDE  */
    s16     unknown0xE0;        /* 0xE0* written by the same case */
    u8      unknown0xE2[2];     /* 0xE2  */
    f32     sell_rate_guess;    /* 0xE4* EQ_Item::ValueBuy (0x00438550): multiplies the price when > 1.0 */
    int     cast_time;          /* 0xE8  */
    u8      unknown0xEC[16];    /* 0xEC  */
    u8      skill_mod_id;       /* 0xFC* EQ_Character::Skill (0x00419867): skill modified */
    u8      unknown0xFD;        /* 0xFD  */
    s16     skill_mod_percent;  /* 0xFE* ... by this percentage */
    short   baneRace_guess;     /* 0x100* */
    short   baneBodyType_guess; /* 0x102* */
    u8      baneDamage_guess;   /* 0x104* */
    u8      unknown0x105[3];    /* 0x105  */
    s8      recommended_level;  /* 0x108 */
    u8      unknown0x109[2];    /* 0x109  */
    u8      elementalDamageType_guess; /* 0x10B* */
    u8      elementalDamage_guess; /* 0x10C* */
    u8      unknown0x10D[1];    /* 0x10D  */
    short   factionIds_guess[4]; /* 0x10E* */
    short   factionAmounts_guess[4]; /* 0x116* */
    u8      unknown0x11E[2];    /* 0x11E  */
    u32     deity;              /* 0x120 bitmask */
#ifdef __cplusplus
    int IsWeapon(void);                          /* 0x0043624B */
    int IsScroll(void);                          /* 0x00436270 */
#endif
} EQ_Equipment;                 /* 0x124 */
C_ASSERT(sizeof(EQ_Equipment) == 0x124);
C_ASSERT(offsetof(EQ_Equipment, DEX) == 0xAF);
C_ASSERT(offsetof(EQ_Equipment, INT) == 0xB0);
C_ASSERT(offsetof(EQ_Equipment, MANA) == 0xB9);
C_ASSERT(offsetof(EQ_Equipment, AC) == 0xBA);
C_ASSERT(offsetof(EQ_Equipment, stackable) == 0xBB);
C_ASSERT(offsetof(EQ_Equipment, light) == 0xBD);
C_ASSERT(offsetof(EQ_Equipment, damage) == 0xBF);
C_ASSERT(offsetof(EQ_Equipment, item_type) == 0xC2);
C_ASSERT(offsetof(EQ_Equipment, click_effect_type) == 0xD8);
C_ASSERT(offsetof(EQ_Equipment, charges) == 0xDA);
C_ASSERT(offsetof(EQ_Equipment, click_effect_id) == 0xDC);
C_ASSERT(offsetof(EQ_Equipment, sell_rate_guess) == 0xE4);
C_ASSERT(offsetof(EQ_Equipment, skill_mod_id) == 0xFC);
C_ASSERT(offsetof(EQ_Equipment, skill_mod_percent) == 0xFE);
C_ASSERT(offsetof(EQ_Equipment, baneRace_guess) == 0x100);
C_ASSERT(offsetof(EQ_Equipment, baneBodyType_guess) == 0x102);
C_ASSERT(offsetof(EQ_Equipment, baneDamage_guess) == 0x104);
C_ASSERT(offsetof(EQ_Equipment, elementalDamageType_guess) == 0x10B);
C_ASSERT(offsetof(EQ_Equipment, elementalDamage_guess) == 0x10C);
C_ASSERT(offsetof(EQ_Equipment, factionIds_guess) == 0x10E);
C_ASSERT(offsetof(EQ_Equipment, factionAmounts_guess) == 0x116);

/* EQ_Container: 0xDC bytes. Verified against client: size and the fields marked *. The contents
 * pointers are live in memory; on the wire they are junk and the client zeroes them after the
 * memcpy (with open). EQClassic Item_Struct.common.container agrees (BagType 0xD4, BagSlots 0xD5,
 * open 0xD6, BagSize 0xD7, weightReduction 0xD8). */
typedef struct EQ_Container
{
    EQ_Item item;               /* 0x00  */
    EQ_Item *contents[10];      /* 0xAC* */
    u8      bag_type;           /* 0xD4* world containers: the object type from OP_ClickObjectAck */
    u8      slots;              /* 0xD5* */
    u8      open;               /* 0xD6* cleared on receipt */
    u8      size_capacity;      /* 0xD7* set to 10 for world containers */
    u8      weight_reduction;   /* 0xD8* */
    u8      unknown0xD9[3];     /* 0xD9  */
#ifdef __cplusplus
    EQ_Container();                                    /* 0x00434040 */
    ~EQ_Container();                                   /* 0x0043405A */
    bool Close_guess(void);                            /* 0x0043407E */
#endif
} EQ_Container;                 /* 0xDC */
C_ASSERT(sizeof(EQ_Container) == 0xDC);
C_ASSERT(offsetof(EQ_Container, contents) == 0xAC);
C_ASSERT(offsetof(EQ_Container, slots) == 0xD5);
C_ASSERT(offsetof(EQ_Container, open) == 0xD6);

/* EQ_Note (books, notes, scrolls with text): 0xD0 bytes. Verified against client: size, and file
 * at 0xAF (sent as the OP_ReadBook request, 0x0041800A / 0x0045CB52). Rest EQClassic. */
typedef struct EQ_Note
{
    EQ_Item item;               /* 0x00  */
    s8      book;               /* 0xAC  1 = book, 0 = note */
    char    book_type;          /* 0xAD  */
    u8      unknown0xAE;        /* 0xAE  */
    char    file[15];           /* 0xAF* text file name on the server */
    u8      unknown0xBE[18];    /* 0xBE  */
} EQ_Note;                      /* 0xD0 */
C_ASSERT(sizeof(EQ_Note) == 0xD0);
C_ASSERT(offsetof(EQ_Note, file) == 0xAF);

#pragma pack(pop)

#endif /* TRILOGY_ITEM_H */
