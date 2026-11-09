/*
 * opcodes.h - message opcodes of the Trilogy eqgame.exe (2001-08-21).
 *
 * Values are what the client compares against: the u16 at the start of every message, read
 * little-endian (OP_PlayerProfile = 0x202D arrives as the bytes 2D 20). EQClassic and ShowEQ write
 * the same opcodes byte-swapped (0x2d20). Names are ours; where EQClassic has a natural name we use
 * it. Each line records the references that list the opcode and what we found in the client:
 *   handled      a constant in CEverQuest::HandleWorldMessage (0x00496743; the jump table at
 *                0x0049FCB8 covers 0x200F..0x2126) or one of its sub-handlers
 *   sent by X    passed to send_message (0x004E1C45) as a constant by function X
 *   constant in  only seen as an immediate in that function (weaker evidence)
 *   not found    neither; kept because a reference lists it
 * Dropped EQClassic aliases the client contradicts or that duplicate a value under a worse name:
 * OP_CreateObject/OP_ClickObject (0x202D/0x202C are the profile and a player update),
 * EQClassic's OP_EnvDamage=0x2058 (that is OP_Action; our OP_EnvDamage is EQClassic's
 * OP_ENVDAMAGE2 = 0x201E), OP_TradeRequest, OP_PoisonComplete, OP_HarmTouch,
 * OP_GMBecomeNPC, OP_Social_Action, OP_TimeOfDaySound, OP_Sell_Message, OP_MessageUnstuned.
 *
 * Opcodes the client handles or sends that no reference names (hex):
 *   200A 2016 201D 2023 2025 203C 203E 2048 204C 205D 205E 206A
 *   206B 2070 2071 2078 2079 207A 207B 207C 207D 207E 207F 20B4
 *   20BE 20C1 20C7 20C9 20D2 20D4 20D8 20FD 2102 2104 2106 210F
 *   2110 2111 2112 2113 2115 211A 211E 2122 2127 2130 2134 2135
 *   2138 2139 2141 214A 214B 2150 2152 215A 215E 216D 2181 218A
 *   2193 2194 21A9 21AA 21AE 21B0 21B2 21B4 21B5 21B6 21C5 21D2
 *   21E2 21EF 21F2 21F9 2204 400E
 */
#ifndef TRILOGY_OPCODES_H
#define TRILOGY_OPCODES_H

enum EQOpcode
{
    OP_SendLoginInfo             = 0x1858,  // EQClassic; client: sent by 004E013C
    OP_DropCoin                  = 0x2007,  // EQClassic + ShowEQ; client: handled; sent by 004CBA05
    OP_PickupCoin                = 0x2008,  // EQClassic + ShowEQ; client: handled; sent by 004CBA05
    OP_ShopRequest               = 0x200B,  // EQClassic + ShowEQ; client: handled
    OP_ShopItem                  = 0x200C,  // EQClassic + ShowEQ; client: handled
    OP_ShopContainer             = 0x200D,  // client only (EQ_Container (0xdc) merchant entry); client: handled
    OP_Petition                  = 0x200E,  // EQClassic; client: sent by 004A07FD
    OP_PetitionClientUpdate      = 0x200F,  // EQClassic; client: handled
    OP_PetitionRefresh           = 0x2011,  // EQClassic; client: sent by 00425D5A, 004D5D0F
    OP_SystemMessage             = 0x2014,  // EQClassic + ShowEQ; client: handled
    OP_EmoteText                 = 0x2015,  // EQClassic + ShowEQ; client: handled; sent by 004CC92F, 004D0952
    OP_PartyChannelMessage       = 0x2017,  // EQClassic; client: handled
    OP_EnvDamage                 = 0x201E,  // EQClassic; client: sent by 004247F0
    OP_MovementUpdate            = 0x201F,  // EQClassic; client: sent by 00425D5A
    OP_Jump                      = 0x2020,  // EQClassic; client: sent by 004D5D0F
    OP_MoveSpawn                 = 0x2021,  // client only (spawn id + y/x/z floats, snaps the spawn there); client: handled
    OP_ResurrectFailed           = 0x2022,  // EQClassic; client: handled
    OP_SafePoint                 = 0x2024,  // EQClassic; client: sent by 00415083
    OP_ShopPlayerBuy             = 0x2027,  // EQClassic; client: handled; sent by 00463A0E
    OP_ShopPlayerSell            = 0x2028,  // EQClassic + ShowEQ; client: handled; sent by 00463B5D
    OP_ShopBook                  = 0x2029,  // client only (EQ_Note (0xd0) merchant entry); client: handled
    OP_ZoneEntry                 = 0x202A,  // EQClassic + ShowEQ; client: handled; sent by 004D4C19
    OP_DeleteSpawn               = 0x202B,  // EQClassic + ShowEQ; client: handled; sent by 004D5D0F, 004DC1B5
    OP_PlayerUpdate              = 0x202C,  // client only (replaces an EQPlayer image (0x134 bytes) by name); client: handled
    OP_PlayerProfile             = 0x202D,  // EQClassic + ShowEQ; client: handled
    OP_Save                      = 0x202E,  // EQClassic; client: constant in 004CE8C0
    OP_ContainerTradeIn          = 0x2030,  // client only (EQ_Container (0xdc)); client: handled
    OP_ItemTradeIn               = 0x2031,  // EQClassic + ShowEQ; client: handled
    OP_SummonUnknownItem         = 0x2034,  // EQClassic; client: handled
    OP_DropItem                  = 0x2035,  // EQClassic + ShowEQ; client: handled; sent by 004A2A0F, 004CB7B8
    OP_PickupItem                = 0x2036,  // EQClassic + ShowEQ; client: handled; sent by 004CBA05
    OP_ShopEnd                   = 0x2037,  // EQClassic; client: handled; sent by 00464AB6
    OP_ShopDelItem               = 0x2038,  // EQClassic; client: handled
    OP_Summon                    = 0x203D,  // EQClassic; client: handled; sent by 00496743
    OP_MoneyTrade                = 0x203F,  // EQClassic; client: handled
    OP_GroupInvite               = 0x2040,  // EQClassic; client: handled; sent by 004CAF5E
    OP_GroupDeclineInvite        = 0x2041,  // EQClassic; client: handled; sent by 004CB248, 004CB4FA
    OP_GroupFollow               = 0x2042,  // EQClassic; client: handled; sent by 004CB362
    OP_GroupQuit                 = 0x2044,  // EQClassic; client: sent by 004CB4FA
    OP_SpawnProjectile           = 0x2045,  // EQClassic; client: handled; sent by 004AC67C
    OP_CastOn                    = 0x2046,  // EQClassic + ShowEQ; client: handled
    OP_SendCharInfo              = 0x2047,  // EQClassic; client: handled
    OP_CharacterCreate           = 0x2049,  // EQClassic; client: sent by 004D1166
    OP_Death                     = 0x204A,  // EQClassic + ShowEQ; client: handled; sent by 00422A48
    OP_LootRequest               = 0x204E,  // EQClassic; client: sent by 004DD440
    OP_EndLootRequest            = 0x204F,  // EQClassic; client: handled; sent by 00464C8D, 0048048E
    OP_MoneyOnCorpse             = 0x2050,  // EQClassic + ShowEQ; client: handled
    OP_ItemOnCorpse              = 0x2052,  // EQClassic + ShowEQ; client: handled
    OP_ContainerOnCorpse         = 0x2053,  // client only (EQ_Container (0xdc) loot entry); client: handled
    OP_BookOnCorpse              = 0x2056,  // client only (EQ_Note (0xd0) loot entry); client: handled
    OP_Action                    = 0x2058,  // EQClassic + ShowEQ; client: handled; sent by 004247F0, 004818CB
    OP_DeleteCharacter           = 0x205A,  // EQClassic; client: sent by 004D2F41
    OP_NewZone                   = 0x205B,  // EQClassic + ShowEQ; client: handled
    OP_GMFindPlayer              = 0x2069,  // EQClassic; client: handled; sent by 004A8721
    OP_GMKill                    = 0x206C,  // EQClassic; client: handled; sent by 004818CB, 004A51D6
    OP_GMKick                    = 0x206D,  // EQClassic; client: handled; sent by 004A4EBB
    OP_GMGoto                    = 0x206E,  // EQClassic; client: handled; sent by 004A920D
    OP_QuestCompletedMoney       = 0x2080,  // EQClassic; client: handled
    OP_NameApproval              = 0x208B,  // EQClassic; client: handled; sent by 004D1166
    OP_GMNameApprovalWindow      = 0x208C,  // EQClassic; client: handled
    OP_ClickDoor                 = 0x208D,  // EQClassic; client: constant in 004BE44E
    OP_OpenDoor                  = 0x208E,  // EQClassic + ShowEQ; client: handled
    OP_Illusion                  = 0x2091,  // EQClassic + ShowEQ; client: handled; sent by 004AAA71
    OP_WearChange                = 0x2092,  // EQClassic + ShowEQ; client: handled; sent by 0040B6F6, 004A1E2A, 004D56B0
    OP_BindWound                 = 0x2093,  // EQClassic; client: handled; sent by 0041D790
    OP_Forage                    = 0x2094,  // EQClassic; client: constant in 0041D790
    OP_SpawnDoor                 = 0x2095,  // EQClassic + ShowEQ; client: handled
    OP_UpdateDoor                = 0x2098,  // EQClassic; client: constant in 00436946
    OP_GroupExp                  = 0x209A,  // EQClassic; client: handled
    OP_DespawnDoor               = 0x209B,  // EQClassic; client: handled
    OP_ClassTraining             = 0x209C,  // EQClassic + ShowEQ; client: handled
    OP_ClassEndTraining          = 0x209D,  // EQClassic; client: sent by 0040C30A, 0045CB52, 0047D796 ...
    OP_Attack                    = 0x209F,  // EQClassic (also OP_UseAbility) + ShowEQ; client: handled; sent by 0041D790, 0041EC4F, 004AC67C ...
    OP_LootItem                  = 0x20A0,  // EQClassic; client: handled; sent by 00457CD6
    OP_SpawnPositionUpdate       = 0x20A1,  // EQClassic + ShowEQ; client: handled
    OP_ZoneUnavailable           = 0x20A2,  // EQClassic; client: handled
    OP_ZoneChange                = 0x20A3,  // EQClassic + ShowEQ; client: sent by 004DBF94
    OP_GMServers                 = 0x20A8,  // EQClassic; client: constant in 00436946
    OP_BeginCast                 = 0x20A9,  // EQClassic + ShowEQ; client: handled
    OP_DisarmComplete            = 0x20AA,  // EQClassic; client: handled; sent by 00493FE8
    OP_FeignDeath                = 0x20AC,  // EQClassic; client: handled
    OP_PickPockets               = 0x20AD,  // EQClassic; client: handled; sent by 0041D790, 00494485
    OP_LoseControl               = 0x20B0,  // EQClassic; client: handled
    OP_HPUpdate                  = 0x20B2,  // EQClassic + ShowEQ; client: handled
    OP_BugReport                 = 0x20B3,  // EQClassic; client: constant in 0046360C
    OP_InspectRequest            = 0x20B5,  // EQClassic; client: handled; sent by 004D9CAC
    OP_InspectAnswer             = 0x20B6,  // EQClassic + ShowEQ; client: handled
    OP_ConsentRequest            = 0x20B7,  // EQClassic; client: not found
    OP_GMSummon                  = 0x20C5,  // EQClassic; client: handled; sent by 004A6658, 004A847D
    OP_GMNameChange              = 0x20CB,  // EQClassic; client: handled; sent by 004A4D37
    OP_ReadBook                  = 0x20CE,  // EQClassic + ShowEQ; client: handled; sent by 0041800A, 00457CD6, 0045CB52
    OP_RequestDuel               = 0x20CF,  // EQClassic; client: handled; sent by 004AA8DA
    OP_DuelResponse              = 0x20D0,  // EQClassic; client: handled; sent by 00496598, 004AA817
    OP_GiveItem                  = 0x20D1,  // EQClassic; client: handled; sent by 004CBA05
    OP_ConsentPlayer             = 0x20D5,  // EQClassic; client: handled
    OP_NPCIsBusy                 = 0x20D6,  // EQClassic; client: handled; sent by 00496743
    OP_CraftingStation           = 0x20D7,  // EQClassic; client: handled; sent by 004176A0
    OP_ForceLogOut               = 0x20D9,  // EQClassic; client: handled; sent by 004DA043
    OP_Click_Give                = 0x20DA,  // EQClassic; client: handled
    OP_CancelTrade               = 0x20DB,  // EQClassic; client: handled; sent by 004571EB, 00496743
    OP_CloseTrade                = 0x20DC,  // EQClassic; client: handled
    OP_ItemToTrade               = 0x20DF,  // EQClassic + ShowEQ; client: handled
    OP_TradeCoins                = 0x20E4,  // EQClassic; client: handled
    OP_TradeAccepted             = 0x20E6,  // EQClassic; client: handled; sent by 00496743
    OP_TimeOfDay                 = 0x20F2,  // EQClassic + ShowEQ; client: handled
    OP_ClientUpdate              = 0x20F3,  // EQClassic + ShowEQ; client: handled; sent by 004D5D0F
    OP_WhoAll                    = 0x20F4,  // EQClassic; client: sent by 004CA5C7
    OP_SpawnAppearance           = 0x20F5,  // EQClassic + ShowEQ; client: handled; sent by 00415EFB, 0041D790, 0041F381 ...
    OP_ObjectDisplayOnly         = 0x20F6,  // EQClassic; client: handled
    OP_StationItem               = 0x20FB,  // EQClassic; client: handled
    OP_StationBook               = 0x20FC,  // client only (EQ_Note (0xd0) into the open world container); client: handled
    OP_TradeSkillCombine         = 0x2105,  // EQClassic; client: handled; sent by 0041800A
    OP_ChannelMessage            = 0x2107,  // EQClassic + ShowEQ; client: handled; sent by 00496743
    OP_MBRetrieveMessages        = 0x2108,  // EQClassic; client: sent by 00415EFB
    OP_MBRetrieveMessage         = 0x2109,  // EQClassic; client: sent by 0045CB52
    OP_MBSendMessageToClient     = 0x210A,  // EQClassic; client: handled
    OP_MBMessageDetail           = 0x210B,  // EQClassic; client: handled
    OP_MBPostMessage             = 0x210C,  // EQClassic; client: sent by 0045CB52
    OP_MBEraseMessage            = 0x210D,  // EQClassic; client: sent by 0045CB52
    OP_MBDataSent_OpenMessageBoard = 0x210E,  // EQClassic; client: handled
    OP_CorpseDragPermission      = 0x2114,  // EQClassic; client: handled
    OP_DoorSays                  = 0x2116,  // EQClassic; client: handled
    OP_GuildInvite               = 0x2117,  // EQClassic; client: handled; sent by 004C9EF6
    OP_GuildInviteAccept         = 0x2118,  // EQClassic; client: sent by 00491272
    OP_GuildRemove               = 0x2119,  // EQClassic; client: sent by 004CA148
    OP_Beg                       = 0x2125,  // EQClassic; client: handled; sent by 0041D790
    OP_CommandBoat               = 0x2126,  // EQClassic; client: handled; sent by 00415EFB, 004D6F06
    OP_GuildNameRequest          = 0x2128,  // client only (asks for the name of an unknown guild id (4 bytes)); client: sent by 0049286E, 00492ADA, 00496743 ...
    OP_PlayerDeath               = 0x2129,  // EQClassic; client: sent by 0048048E
    OP_RezzRequest               = 0x212A,  // EQClassic; client: handled; sent by 00420505
    OP_GuildCreateResponse       = 0x212B,  // EQClassic; client: handled
    OP_MoveItem                  = 0x212C,  // EQClassic; client: handled; sent by 00427EE7, 00457CD6, 0045CB52 ...
    OP_MoveCoin                  = 0x212D,  // EQClassic; client: sent by 00457CD6, 0045CB52, 004818CB ...
    OP_ItemMissing               = 0x212F,  // EQClassic; client: handled; sent by 00423EDB
    OP_SplitMoney                = 0x2131,  // EQClassic; client: sent by 004CFD5F
    OP_Buff                      = 0x2132,  // EQClassic; client: handled; sent by 00424E30, 004251B9
    OP_GuildChannelMessage       = 0x2133,  // EQClassic; client: handled; sent by 00496743
    OP_Weather                   = 0x2136,  // EQClassic; client: handled
    OP_Consider                  = 0x2137,  // EQClassic + ShowEQ; client: handled; sent by 004A6A08
    OP_Taunt                     = 0x213B,  // EQClassic; client: constant in 0041D790
    OP_TradeMoneyUpdate          = 0x213D,  // EQClassic + ShowEQ; client: handled
    OP_TradeItemToSlot           = 0x213E,  // client only (same layout as OP_ItemToTrade, item goes to an inventory slot); client: handled
    OP_ClassTrainSkill           = 0x2140,  // EQClassic; client: sent by 00464468
    OP_ClassEndTrainingAck       = 0x2143,  // ShowEQ only (closeGM); client: not found
    OP_LootComplete              = 0x2144,  // EQClassic; client: handled
    OP_ShopEndConfirm            = 0x2145,  // EQClassic + ShowEQ; client: handled
    OP_ConsumeItem               = 0x2146,  // EQClassic; client: handled; sent by 0042520E, 00427EE7, 0046AB71 ...
    OP_ClientError               = 0x2147,  // EQClassic; client: sent by 00415EFB, 004176A0, 004571EB ...
    OP_NewSpawn                  = 0x2149,  // EQClassic + ShowEQ; client: handled
    OP_TeleportPC                = 0x214D,  // EQClassic; client: handled
    OP_GMZoneRequest             = 0x214F,  // EQClassic; client: handled; sent by 004A50D0
    OP_AutoAttack                = 0x2151,  // EQClassic; client: sent by 00425D5A
    OP_PlayerSave                = 0x2154,  // EQClassic; client: constant in 004C71A3
    OP_PlayerSave2               = 0x2155,  // EQClassic; client: constant in 004C71A3
    OP_ConsumeFoodDrink          = 0x2156,  // EQClassic; client: sent by 00428193, 0046AB71
    OP_Stamina                   = 0x2157,  // EQClassic + ShowEQ; client: handled
    OP_Medding                   = 0x2158,  // EQClassic; client: sent by 00425D5A
    OP_Stun                      = 0x215B,  // EQClassic; client: handled
    OP_DuelResponse2             = 0x215D,  // EQClassic; client: sent by 004AA8DA
    OP_CombatAbility             = 0x215F,  // EQClassic (also OP_CombatAbility); client: sent by 004A4327
    OP_AutoAttack2               = 0x2160,  // EQClassic; client: sent by 00425D5A
    OP_ZoneSpawns                = 0x2161,  // EQClassic + ShowEQ; client: handled
    OP_ClientTarget              = 0x2162,  // EQClassic + ShowEQ; client: sent by 00425D5A
    OP_PlayerItem                = 0x2164,  // EQClassic + ShowEQ; client: handled
    OP_PlayerBook                = 0x2165,  // ShowEQ only (PlayerBook); client: handled
    OP_PlayerContainer           = 0x2166,  // ShowEQ only (PlayerContainer); client: handled
    OP_GMSurname                 = 0x216E,  // EQClassic; client: handled; sent by 004A4C6D
    OP_GuildWar                  = 0x216F,  // EQClassic; client: handled; sent by 004CB10B
    OP_SummonedItem              = 0x2178,  // EQClassic + ShowEQ; client: handled
    OP_SummonedContainer         = 0x2179,  // EQClassic (as SummonedContainer); client: handled
    OP_SummonedBook              = 0x217A,  // client only (EQ_Note (0xd0) handler next to 0x2178/0x2179); client: handled
    OP_GuildUpdate               = 0x217B,  // EQClassic + ShowEQ; client: handled
    OP_CastSpell                 = 0x217E,  // EQClassic + ShowEQ; client: sent by 0041F381
    OP_ManaChange                = 0x217F,  // EQClassic + ShowEQ; client: handled; sent by 004279B4
    OP_SpecialMesg               = 0x2180,  // EQClassic + ShowEQ; client: handled
    OP_MemorizeSpell             = 0x2182,  // EQClassic + ShowEQ; client: handled; sent by 00457617, 004577C6, 00457CD6 ...
    OP_BindWoundSeq              = 0x2183,  // ShowEQ only (bindWound); client: handled
    OP_Track                     = 0x2184,  // EQClassic; client: sent by 0041D790
    OP_Sneak                     = 0x2185,  // EQClassic; client: sent by 0041D790
    OP_Hide                      = 0x2186,  // EQClassic; client: sent by 0041D790
    OP_SenseHeading              = 0x2187,  // EQClassic (also OP_SenseHeading); client: constant in 0041D790
    OP_SenseTraps                = 0x2188,  // EQClassic; client: constant in 0041D790
    OP_SkillUpdate               = 0x2189,  // EQClassic + ShowEQ; client: handled
    OP_PKAcknowledge             = 0x218C,  // EQClassic; client: handled; sent by 004AAA71
    OP_PetitionCheckout          = 0x218E,  // EQClassic; client: handled
    OP_Fishing                   = 0x218F,  // EQClassic; client: constant in 0041D790
    OP_InitiateConsume           = 0x2190,  // EQClassic; client: handled
    OP_GuildPeace                = 0x2191,  // EQClassic; client: handled; sent by 004CB1A5
    OP_GuildList                 = 0x2192,  // EQClassic; client: handled; sent by 004DA043
    OP_GuildLeader               = 0x2195,  // EQClassic; client: sent by 004CA495
    OP_GroupDelete               = 0x2197,  // EQClassic; client: sent by 00491632, 004CB4FA
    OP_LevelUpdate               = 0x2198,  // EQClassic + ShowEQ; client: handled
    OP_ExpUpdate                 = 0x2199,  // EQClassic + ShowEQ; client: handled
    OP_SplitResponse             = 0x219A,  // EQClassic; client: handled; sent by 004DD0C2
    OP_RezzAnswer                = 0x219B,  // EQClassic; client: sent by 004818CB
    OP_InstillDoubt              = 0x219C,  // EQClassic; client: constant in 0041D790
    OP_Mend                      = 0x219D,  // EQClassic; client: constant in 0041D790
    OP_PetitionCheckIn           = 0x219E,  // EQClassic; client: constant in 00437792
    OP_PetitionDelete            = 0x21A0,  // EQClassic; client: constant in 00437792
    OP_CorpseInZone              = 0x21A7,  // EQClassic; client: handled; sent by 004A8819
    OP_SafeFallSuccess           = 0x21AB,  // EQClassic; client: constant in 0042444C
    OP_ConsentResponse           = 0x21B7,  // EQClassic; client: not found
    OP_ApplyPoison               = 0x21BA,  // EQClassic; client: handled; sent by 00425D5A
    OP_GetOnBoat                 = 0x21BB,  // EQClassic; client: sent by 00425D5A
    OP_GetOffBoat                = 0x21BC,  // EQClassic; client: sent by 00425D5A
    OP_SkillChangeResp           = 0x21BE,  // EQClassic; client: handled
    OP_AbilChangeResp            = 0x21BF,  // EQClassic; client: handled
    OP_GMQuestResponse           = 0x21C0,  // EQClassic; client: handled
    OP_XPGiveResponse            = 0x21C1,  // EQClassic; client: handled
    OP_XPSetResponse             = 0x21C2,  // EQClassic; client: handled; sent by 004818CB
    OP_SetServerFilterAck        = 0x21C3,  // EQClassic; client: handled
    OP_Surname                   = 0x21C4,  // EQClassic; client: handled; sent by 004818CB
    OP_SwapSpell                 = 0x21CE,  // EQClassic; client: handled; sent by 0045CB52
    OP_PlayerKiller              = 0x21CF,  // EQClassic; client: handled
    OP_GMPetitionWindow          = 0x21D0,  // EQClassic; client: handled; sent by 004380C0
    OP_InterruptCast             = 0x21D3,  // EQClassic + ShowEQ; client: handled; sent by 004279B4
    OP_GMHideMe                  = 0x21D4,  // EQClassic; client: sent by 004A4F81
    OP_ExpansionInfo             = 0x21D8,  // EQClassic; client: handled
    OP_YellForHelp               = 0x21DA,  // EQClassic; client: handled; sent by 004A80CE
    OP_ServerMOTD                = 0x21DD,  // EQClassic; client: handled
    OP_GMToggle                  = 0x21DE,  // EQClassic; client: sent by 004A911A
    OP_GMGetSkillInfo            = 0x21E0,  // EQClassic; client: handled
    OP_GMEmoteZone               = 0x21E3,  // EQClassic; client: sent by 004A9FF5
    OP_UseDiscipline             = 0x21E6,  // EQClassic; client: sent by 004A58F5, 004A5984
    OP_Random                    = 0x21E7,  // EQClassic + ShowEQ; client: sent by 004A6960
    OP_SetDataRate               = 0x21E8,  // EQClassic; client: sent by 004D4C19
    OP_GMDelCorpse               = 0x21E9,  // EQClassic; client: sent by 004A52E1
    OP_Sacrifice                 = 0x21EA,  // EQClassic; client: handled; sent by 00481474, 004818CB
    OP_RezzComplete              = 0x21EC,  // EQClassic; client: handled
    OP_LFG                       = 0x21F0,  // EQClassic; client: handled; sent by 004A8A32
    OP_DisarmTraps               = 0x21F3,  // EQClassic; client: constant in 0041D790
    OP_SenseTrap                 = 0x21F4,  // EQClassic; client: handled
    OP_CompressedItems           = 0x21F6,  // EQClassic + ShowEQ; client: handled
    OP_CompressedDoorSpawn       = 0x21F7,  // ShowEQ only (CompressedDoorSpawn); client: handled
    OP_BackSlashTarget           = 0x21FE,  // EQClassic; client: handled; sent by 004AB3B7
    OP_SetServerFilter           = 0x21FF,  // EQClassic; client: sent by 004CF829
    OP_AssistTarget              = 0x2200,  // EQClassic; client: handled
    OP_GuildMOTD                 = 0x2203,  // EQClassic; client: sent by 004A54C8, 004D5D0F
    OP_CleanStation              = 0x2205,  // EQClassic; client: handled
    OP_Translocate               = 0x2206,  // EQClassic; client: handled; sent by 004818CB
    OP_Camp                      = 0x2207,  // EQClassic; client: sent by 004CB72D
    OP_GMZoneRequest2            = 0x2208,  // EQClassic; client: sent by 004A50D0
    OP_WebUpdate                 = 0x223C,  // EQClassic; client: not found
    OP_GroupUpdate               = 0x4026,  // EQClassic + ShowEQ; client: handled
    OP_EnterWorld                = 0x8001,  // EQClassic; client: sent by 004D2FB6, 004DA043
    OP_ZoneServerInfo            = 0x8004,  // EQClassic; client: handled
    OP_ZoneUnavail               = 0x8005   // EQClassic; client: handled
};

#endif /* TRILOGY_OPCODES_H */
