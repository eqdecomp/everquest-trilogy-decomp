// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// EQ_Slider.cpp - EQ_HSlider / EQ_HSlider2 (old-UI horizontal sliders) methods.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Slider.cpp (from Mac debug map)

#include "trilogy/eq_slider.h"
#include "trilogy/layout/eq_overlay.h"
#include "trilogy/eqaddr.h"
#include "trilogy/character.h"
#include "trilogy/globals.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/display.h"
#include "trilogy/layout/skill_record.h"
#include "trilogy/item.h"

#pragma comment(lib, "user32.lib")     // CopyRect

// The overlay class with vtable 0x0053563C (constructor 0x00482123, CharacterSelector_guess in
// EQ_Requestor.cpp); only its destructor lives in this file.
class CharacterSelectorOverlay_guess : public EQ_Overlay
{
public:
    ~CharacterSelectorOverlay_guess();
};

// The 0x124-byte inventory item the sliders allocate for a split or bought stack.
class SliderEquipment_guess : public EQ_Equipment
{
public:
    // EXTERNAL: TRILOGY 0x00436099 SliderEquipment_guess::SliderEquipment_guess
    SliderEquipment_guess();
};

// FUNCTION: TRILOGY 0x0048257D
// Releases the overlay's buttons, then runs the base overlay teardown.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
// @early-stop: the raw vtable store is scheduled after the EH-state store (original: before, as a compiler vfptr store).
CharacterSelectorOverlay_guess::~CharacterSelectorOverlay_guess()
{
    *(u32*)this = 0x0053563C;
    DelButtons();
}

// FUNCTION: TRILOGY 0x004825b5
// __fastcall (ECX = skill record, skill id on the stack); fills one skill's static record and returns the record.
// First sets every per-class cap to 0xff and clears the mode and icon fields, then a switch on the skill id
// (cases up to 0x49, default for other ids) writes the name via 0x005219c0, reuse delay, base damage, per-class
// minimum levels and base/expansion caps.
// The fillUnsetCaps tail copies each unset byte cap (+0x68) into its dword cap (+0x94) for 0x20 entries.
int __fastcall InitializeSkillData_guess(int skillRecord, int, short skill)
{
  int skillId;
  u32 capValue;
  u8 *byteCap;
  u32 *dwordCapInit;
  u32 *dwordCap;
  char *skillName;

  skillId = 0;
  dwordCapInit = (u32 *)(skillRecord + 0x94);
  byteCap = (u8 *)(skillRecord + 0x68);
  do {
    *dwordCapInit = 0xffffffff;
    byteCap[skillId] = 0xff;
    skillId = skillId + 1;
    dwordCapInit = dwordCapInit + 1;
  } while (skillId < 0x20);
  skillId = (int)skill;
  ((skill_record*)skillRecord)->activationMode_guess = 0;
  ((skill_record*)skillRecord)->showInAbilities_guess = 0;
  ((skill_record*)skillRecord)->unk_0x090 = 0;
  ((skill_record*)skillRecord)->readyIconIndex = 0x74;
  ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x75;
  ((skill_record*)skillRecord)->levelBasedCap = 1;
  switch(skillId) {
  case 0:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c9a8));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xf0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[8] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xe1;
    goto setCapA4;
  case 1:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c99c));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[8] = 0xe1;
setCapA4:
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xfa;
    goto setFloat90Tenth;
  case 2:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c990));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xf0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xe1;
    goto setCapA4Float90Fifth;
  case 3:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c984));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xe1;
setCapA4Float90Fifth:
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xfa;
    ((skill_record*)skillRecord)->unk_0x090 = 0x3e4ccccd;
    break;
  case 4:
    skillName = ((char *)0x0055c978);
    goto initSpellSchoolSkill;
  case 5:
    skillName = ((char *)0x0055c958);
    goto initSpellSchoolSkill;
  case 6:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c948));
    ((skill_record*)skillRecord)->reuseDelayMs = 60000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x12;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x14;
    *(u8 *)(skillRecord + 0x4a) = 0x14;
    *(u8 *)(skillRecord + 0x4b) = 0x14;
    *(u8 *)(skillRecord + 0x4d) = 0x14;
    *(u8 *)(skillRecord + 0x4c) = 0x14;
    *(u8 *)(skillRecord + 0x4e) = 0x14;
    *(u8 *)(skillRecord + 0x4f) = 2;
    *(u8 *)(skillRecord + 0x50) = 0x14;
    *(u8 *)(skillRecord + 0x51) = 2;
    *(u8 *)(skillRecord + 0x52) = 0x14;
    *(u8 *)(skillRecord + 0x53) = 0x14;
    *(u8 *)(skillRecord + 0x54) = 0x14;
    *(u8 *)(skillRecord + 0x55) = 0x14;
    *(u8 *)(skillRecord + 0x56) = 0x14;
    ((skill_record*)skillRecord)->readyIconIndex = 0x96;
    *(u8 *)(skillRecord + 0x57) = 0x14;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x97;
    break;
  case 7:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c940));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 4;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    *(u8 *)(skillRecord + 0x57) = 4;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xf0;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xf0;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xf0;
    ((skill_record*)skillRecord)->unk_0x090 = 0x3e19999a;
    break;
  case 8:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c934));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x12;
    *(u8 *)(skillRecord + 0x4a) = 0x14;
    *(u8 *)(skillRecord + 0x4b) = 0x12;
    *(u8 *)(skillRecord + 0x4d) = 0x12;
    *(u8 *)(skillRecord + 0x4c) = 0x12;
    *(u8 *)(skillRecord + 0x4e) = 0x14;
    *(u8 *)(skillRecord + 0x4f) = 0x12;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 7;
    *(u8 *)(skillRecord + 0x52) = 0x14;
    *(u8 *)(skillRecord + 0x53) = 0x18;
    *(u8 *)(skillRecord + 0x54) = 0x18;
    *(u8 *)(skillRecord + 0x55) = 0x18;
    *(u8 *)(skillRecord + 0x56) = 0x18;
    ((skill_record*)skillRecord)->readyIconIndex = 0x6e;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x6f;
    ((skill_record*)skillRecord)->unk_0x090 = 0x3e99999a;
    *(u8 *)(skillRecord + 0x57) = 0x18;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xe1;
    goto setByte26To2;
  case 9:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c920));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 2;
    *(u8 *)(skillRecord + 0x4a) = 2;
    *(u8 *)(skillRecord + 0x4b) = 2;
    *(u8 *)(skillRecord + 0x4d) = 2;
    *(u8 *)(skillRecord + 0x4c) = 2;
    *(u8 *)(skillRecord + 0x4e) = 2;
    *(u8 *)(skillRecord + 0x4f) = 2;
    *(u8 *)(skillRecord + 0x50) = 2;
    *(u8 *)(skillRecord + 0x51) = 2;
    *(u8 *)(skillRecord + 0x52) = 2;
    *(u8 *)(skillRecord + 0x53) = 2;
    *(u8 *)(skillRecord + 0x54) = 2;
    *(u8 *)(skillRecord + 0x55) = 2;
    *(u8 *)(skillRecord + 0x56) = 2;
    ((skill_record*)skillRecord)->readyIconIndex = 0x7e;
    *(u8 *)(skillRecord + 0x57) = 2;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0x96;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x96;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x96;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[2] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[6] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[10] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[8] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xd2;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x7f;
    goto setBytes25And26;
  case 10:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c918));
    ((skill_record*)skillRecord)->reuseDelayMs = 8000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 2;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 6;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 6;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 6;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    ((skill_record*)skillRecord)->readyIconIndex = 0x66;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x67;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xaf;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xf0;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->unk_0x090 = 0x3e99999a;
    goto setByte25ThenByte26;
  case 0xb:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c8ec));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0x19;
    *(u8 *)(skillRecord + 0x49) = 0xc;
    *(u8 *)(skillRecord + 0x4a) = 0xc;
    *(u8 *)(skillRecord + 0x4b) = 0xc;
    *(u8 *)(skillRecord + 0x4d) = 0xc;
    *(u8 *)(skillRecord + 0x4c) = 0xc;
    *(u8 *)(skillRecord + 0x4e) = 0xc;
    *(u8 *)(skillRecord + 0x4f) = 0xc;
    *(u8 *)(skillRecord + 0x50) = 0xc;
    *(u8 *)(skillRecord + 0x51) = 0xc;
    *(u8 *)(skillRecord + 0x52) = 0xc;
    *(u8 *)(skillRecord + 0x53) = 0xc;
    *(u8 *)(skillRecord + 0x54) = 0xc;
    *(u8 *)(skillRecord + 0x55) = 0xc;
    *(u8 *)(skillRecord + 0x56) = 0xc;
    *(u8 *)(skillRecord + 0x57) = 0xc;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0xb4;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 200;
    break;
  case 0xc:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c8d8));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0xb;
    goto fillFrom31;
  case 0xd:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c8c4));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 9;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 9;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 9;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 9;
    *(u8 *)(skillRecord + 0x49) = 7;
    *(u8 *)(skillRecord + 0x4a) = 7;
    *(u8 *)(skillRecord + 0x4b) = 7;
    *(u8 *)(skillRecord + 0x4d) = 7;
    *(u8 *)(skillRecord + 0x4c) = 7;
    *(u8 *)(skillRecord + 0x4e) = 7;
    *(u8 *)(skillRecord + 0x4f) = 7;
    *(u8 *)(skillRecord + 0x50) = 7;
    *(u8 *)(skillRecord + 0x51) = 7;
    *(u8 *)(skillRecord + 0x52) = 7;
    *(u8 *)(skillRecord + 0x53) = 7;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xd7;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xd7;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xd7;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xd7;
    *(u8 *)(skillRecord + 0x54) = 7;
    *(u8 *)(skillRecord + 0x55) = 7;
    *(u8 *)(skillRecord + 0x56) = 7;
    *(u8 *)(skillRecord + 0x57) = 7;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[2] = 0xdc;
    ((skill_record*)skillRecord)->expansionCapByClass[10] = 0xdc;
    ((skill_record*)skillRecord)->expansionCapByClass[6] = 0xdc;
    ((skill_record*)skillRecord)->expansionCapByClass[11] = 0xdc;
    ((skill_record*)skillRecord)->expansionCapByClass[13] = 0xdc;
    ((skill_record*)skillRecord)->expansionCapByClass[14] = 0xdc;
    ((skill_record*)skillRecord)->expansionCapByClass[12] = 0xdc;
    break;
  case 0xe:
    skillName = ((char *)0x0055c8b8);
    goto initSpellSchoolSkill;
  case 0xf:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c8b0));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0xf;
    *(u8 *)(skillRecord + 0x4a) = 0x11;
    *(u8 *)(skillRecord + 0x4b) = 0x11;
    *(u8 *)(skillRecord + 0x4d) = 0x11;
    *(u8 *)(skillRecord + 0x4c) = 0x11;
    *(u8 *)(skillRecord + 0x4e) = 0x11;
    *(u8 *)(skillRecord + 0x4f) = 0xf;
    *(u8 *)(skillRecord + 0x50) = 0x11;
    *(u8 *)(skillRecord + 0x51) = 0xf;
    *(u8 *)(skillRecord + 0x52) = 0x11;
    *(u8 *)(skillRecord + 0x53) = 0x11;
    *(u8 *)(skillRecord + 0x54) = 0x11;
    capValue = 0xfc;
    *(u8 *)(skillRecord + 0x55) = 0x11;
    *(u8 *)(skillRecord + 0x56) = 0x11;
    *(u8 *)(skillRecord + 0x57) = 0x11;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xe6;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x91;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x91;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x91;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x91;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xfc;
    goto setSharedCaps;
  case 0x10:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c8a8));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0x23;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0x28;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0x28;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0x23;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x1b;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x1b;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 2;
    *(u8 *)(skillRecord + 0x4a) = 2;
    *(u8 *)(skillRecord + 0x4b) = 2;
    *(u8 *)(skillRecord + 0x4d) = 2;
    *(u8 *)(skillRecord + 0x4c) = 2;
    *(u8 *)(skillRecord + 0x4e) = 2;
    *(u8 *)(skillRecord + 0x4f) = 2;
    *(u8 *)(skillRecord + 0x50) = 2;
    *(u8 *)(skillRecord + 0x51) = 2;
    *(u8 *)(skillRecord + 0x52) = 2;
    *(u8 *)(skillRecord + 0x53) = 2;
    *(u8 *)(skillRecord + 0x54) = 2;
    *(u8 *)(skillRecord + 0x55) = 2;
    *(u8 *)(skillRecord + 0x56) = 2;
    ((skill_record*)skillRecord)->readyIconIndex = 100;
    *(u8 *)(skillRecord + 0x57) = 2;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0x46;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0x46;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x37;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x65;
    goto setBytes25And26;
  case 0x11:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c898));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x15;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x14;
    *(u8 *)(skillRecord + 0x4a) = 0x14;
    *(u8 *)(skillRecord + 0x4b) = 0x14;
    *(u8 *)(skillRecord + 0x4d) = 0x14;
    *(u8 *)(skillRecord + 0x4c) = 0x14;
    *(u8 *)(skillRecord + 0x4e) = 0x14;
    *(u8 *)(skillRecord + 0x4f) = 0x14;
    *(u8 *)(skillRecord + 0x50) = 0x14;
    *(u8 *)(skillRecord + 0x51) = 0x14;
    *(u8 *)(skillRecord + 0x52) = 0x14;
    *(u8 *)(skillRecord + 0x53) = 0x14;
    *(u8 *)(skillRecord + 0x54) = 0x14;
    *(u8 *)(skillRecord + 0x55) = 0x14;
    *(u8 *)(skillRecord + 0x56) = 0x14;
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    ((skill_record*)skillRecord)->readyIconIndex = 0x74;
    *(u8 *)(skillRecord + 0x57) = 0x14;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x75;
    break;
  case 0x12:
    skillName = ((char *)0x0055c88c);
    goto initSpellSchoolSkill;
  case 0x13:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c884));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 6;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 0xf;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 8;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 0xf;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 0xf;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 0x16;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 0x16;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 0x16;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 0x16;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 10;
    *(u8 *)(skillRecord + 0x49) = 0xc;
    *(u8 *)(skillRecord + 0x4a) = 0xc;
    *(u8 *)(skillRecord + 0x4b) = 0xc;
    *(u8 *)(skillRecord + 0x4d) = 0xc;
    *(u8 *)(skillRecord + 0x4c) = 0xc;
    *(u8 *)(skillRecord + 0x4e) = 0xc;
    *(u8 *)(skillRecord + 0x4f) = 0xc;
    *(u8 *)(skillRecord + 0x50) = 0xc;
    *(u8 *)(skillRecord + 0x51) = 0xc;
    *(u8 *)(skillRecord + 0x52) = 0xc;
    *(u8 *)(skillRecord + 0x53) = 0xc;
    *(u8 *)(skillRecord + 0x54) = 0xc;
    *(u8 *)(skillRecord + 0x55) = 0xc;
    *(u8 *)(skillRecord + 0x56) = 0xc;
    *(u8 *)(skillRecord + 0x57) = 0xc;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0x8c;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0x7d;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0x7d;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x89;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x7d;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0x96;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0x7d;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xe6;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xaf;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0x9b;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0x9b;
    ((skill_record*)skillRecord)->expansionCapByClass[8] = 0x9b;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0x9b;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xaa;
    goto setCapB8;
  case 0x14:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c874));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0xf;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0xf;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0xe;
    *(u8 *)(skillRecord + 0x4a) = 0x14;
    *(u8 *)(skillRecord + 0x4b) = 0xe;
    *(u8 *)(skillRecord + 0x4d) = 0xe;
    *(u8 *)(skillRecord + 0x4c) = 0xe;
    *(u8 *)(skillRecord + 0x4e) = 0x14;
    *(u8 *)(skillRecord + 0x4f) = 0xe;
    *(u8 *)(skillRecord + 0x50) = 0xe;
    *(u8 *)(skillRecord + 0x51) = 0xe;
    *(u8 *)(skillRecord + 0x52) = 0x14;
    *(u8 *)(skillRecord + 0x53) = 0x18;
    *(u8 *)(skillRecord + 0x54) = 0x18;
    *(u8 *)(skillRecord + 0x55) = 0x18;
    *(u8 *)(skillRecord + 0x56) = 0x18;
    *(u8 *)(skillRecord + 0x57) = 0x18;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0xcd;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xf5;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xf5;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xeb;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xeb;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xeb;
    break;
  case 0x15:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x005407ac));
    ((skill_record*)skillRecord)->reuseDelayMs = 0x1838;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x19;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 9;
    *(u8 *)(skillRecord + 0x4a) = 9;
    *(u8 *)(skillRecord + 0x4b) = 9;
    *(u8 *)(skillRecord + 0x4d) = 9;
    *(u8 *)(skillRecord + 0x4c) = 9;
    *(u8 *)(skillRecord + 0x4e) = 9;
    *(u8 *)(skillRecord + 0x4f) = 9;
    *(u8 *)(skillRecord + 0x50) = 9;
    *(u8 *)(skillRecord + 0x51) = 9;
    *(u8 *)(skillRecord + 0x52) = 9;
    *(u8 *)(skillRecord + 0x53) = 9;
    *(u8 *)(skillRecord + 0x54) = 9;
    *(u8 *)(skillRecord + 0x55) = 9;
    *(u8 *)(skillRecord + 0x56) = 9;
    ((skill_record*)skillRecord)->readyIconIndex = 0x8a;
    *(u8 *)(skillRecord + 0x57) = 9;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xe1;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x8b;
    goto setFloat90Quarter;
  case 0x16:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c868));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0xd;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0x11;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x11;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0xd;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0x11;
    *(u8 *)(skillRecord + 0x49) = 0x12;
    *(u8 *)(skillRecord + 0x4a) = 0x14;
    *(u8 *)(skillRecord + 0x4b) = 0x12;
    *(u8 *)(skillRecord + 0x4d) = 0x12;
    *(u8 *)(skillRecord + 0x4c) = 0x12;
    *(u8 *)(skillRecord + 0x4e) = 0x14;
    *(u8 *)(skillRecord + 0x4f) = 0x10;
    *(u8 *)(skillRecord + 0x50) = 0x12;
    *(u8 *)(skillRecord + 0x51) = 0x12;
    *(u8 *)(skillRecord + 0x52) = 0x14;
    *(u8 *)(skillRecord + 0x53) = 0x18;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0xd2;
    *(u8 *)(skillRecord + 0x54) = 0x18;
    *(u8 *)(skillRecord + 0x55) = 0x18;
    *(u8 *)(skillRecord + 0x56) = 0x18;
    *(u8 *)(skillRecord + 0x57) = 0x12;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xfc;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[8] = 0xf5;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xf5;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xf5;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xf5;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xf5;
    break;
  case 0x17:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c858));
    ((skill_record*)skillRecord)->reuseDelayMs = 6000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 7;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 9;
    *(u8 *)(skillRecord + 0x4a) = 9;
    *(u8 *)(skillRecord + 0x4b) = 9;
    *(u8 *)(skillRecord + 0x4d) = 9;
    *(u8 *)(skillRecord + 0x4c) = 9;
    *(u8 *)(skillRecord + 0x4e) = 9;
    *(u8 *)(skillRecord + 0x4f) = 9;
    *(u8 *)(skillRecord + 0x50) = 9;
    *(u8 *)(skillRecord + 0x51) = 9;
    *(u8 *)(skillRecord + 0x52) = 9;
    *(u8 *)(skillRecord + 0x53) = 9;
    *(u8 *)(skillRecord + 0x54) = 9;
    *(u8 *)(skillRecord + 0x55) = 9;
    *(u8 *)(skillRecord + 0x56) = 9;
    ((skill_record*)skillRecord)->readyIconIndex = 0x86;
    *(u8 *)(skillRecord + 0x57) = 9;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xe1;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x87;
    goto setFloat90Fifth;
  case 0x18:
    skillName = ((char *)0x0055c84c);
initSpellSchoolSkill:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)skillName);
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 9;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 9;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 9;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 9;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 6;
    *(u8 *)(skillRecord + 0x4b) = 2;
    *(u8 *)(skillRecord + 0x4d) = 2;
    *(u8 *)(skillRecord + 0x4c) = 2;
    *(u8 *)(skillRecord + 0x4e) = 6;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 6;
    *(u8 *)(skillRecord + 0x53) = 6;
    *(u8 *)(skillRecord + 0x54) = 6;
    *(u8 *)(skillRecord + 0x55) = 6;
    *(u8 *)(skillRecord + 0x56) = 6;
    *(u8 *)(skillRecord + 0x57) = 6;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0xeb;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0xeb;
    break;
  case 0x19:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c840));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x11;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    ((skill_record*)skillRecord)->readyIconIndex = 0x90;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x91;
    goto setBytes25And26;
  case 0x1a:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c828));
    ((skill_record*)skillRecord)->reuseDelayMs = 0x2134;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0x19;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    ((skill_record*)skillRecord)->readyIconIndex = 0x80;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x81;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xe1;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->unk_0x090 = 0x3ecccccd;
    goto setByte25ThenByte26;
  case 0x1b:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c820));
    ((skill_record*)skillRecord)->reuseDelayMs = 100000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 3;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 5;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x10;
    *(u8 *)(skillRecord + 0x4a) = 0x10;
    *(u8 *)(skillRecord + 0x4b) = 0x10;
    *(u8 *)(skillRecord + 0x4d) = 0x10;
    *(u8 *)(skillRecord + 0x4c) = 0x10;
    *(u8 *)(skillRecord + 0x4e) = 0x10;
    *(u8 *)(skillRecord + 0x4f) = 0x10;
    *(u8 *)(skillRecord + 0x50) = 0x10;
    *(u8 *)(skillRecord + 0x51) = 0x10;
    *(u8 *)(skillRecord + 0x52) = 0x10;
    *(u8 *)(skillRecord + 0x53) = 0x10;
    *(u8 *)(skillRecord + 0x54) = 0x10;
    *(u8 *)(skillRecord + 0x55) = 0x10;
    *(u8 *)(skillRecord + 0x56) = 0x10;
    ((skill_record*)skillRecord)->readyIconIndex = 0x76;
    *(u8 *)(skillRecord + 0x57) = 0x10;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x37;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x77;
    goto setByte25;
  case 0x1c:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c810));
    ((skill_record*)skillRecord)->reuseDelayMs = 0xdac;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 2;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x19;
    *(u8 *)(skillRecord + 0x4a) = 0x1d;
    *(u8 *)(skillRecord + 0x4b) = 0x19;
    *(u8 *)(skillRecord + 0x4d) = 0x19;
    *(u8 *)(skillRecord + 0x4c) = 0x19;
    *(u8 *)(skillRecord + 0x4e) = 0x1d;
    *(u8 *)(skillRecord + 0x4f) = 0x12;
    *(u8 *)(skillRecord + 0x50) = 0x19;
    *(u8 *)(skillRecord + 0x51) = 0x19;
    *(u8 *)(skillRecord + 0x52) = 0x1b;
    *(u8 *)(skillRecord + 0x53) = 0x22;
    *(u8 *)(skillRecord + 0x54) = 0x22;
    *(u8 *)(skillRecord + 0x55) = 0x22;
    *(u8 *)(skillRecord + 0x56) = 0x22;
    *(u8 *)(skillRecord + 0x57) = 0x14;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xe1;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xe6;
    goto setFloat90Tenth;
  case 0x1d:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c808));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0x23;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0x19;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x19;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 3;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 3;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    ((skill_record*)skillRecord)->readyIconIndex = 0x7c;
    *(u8 *)(skillRecord + 0x57) = 4;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x28;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x7d;
    goto setByte25;
  case 0x1e:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c7f0));
    ((skill_record*)skillRecord)->reuseDelayMs = 8000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 3;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 5;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 5;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    ((skill_record*)skillRecord)->readyIconIndex = 0x84;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0x95;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x95;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0xb4;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xd2;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xcd;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xe6;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x85;
    goto setFloat90Fifth;
  case 0x1f:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c7e4));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 8;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 8;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 8;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0xc;
    *(u8 *)(skillRecord + 0x49) = 10;
    *(u8 *)(skillRecord + 0x4a) = 10;
    *(u8 *)(skillRecord + 0x4b) = 10;
    *(u8 *)(skillRecord + 0x4d) = 10;
    *(u8 *)(skillRecord + 0x4c) = 10;
    *(u8 *)(skillRecord + 0x4e) = 10;
    *(u8 *)(skillRecord + 0x4f) = 10;
    *(u8 *)(skillRecord + 0x50) = 10;
    *(u8 *)(skillRecord + 0x51) = 10;
    *(u8 *)(skillRecord + 0x52) = 10;
    *(u8 *)(skillRecord + 0x53) = 10;
    ((skill_record*)skillRecord)->expansionCapByClass[13] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[14] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[11] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[12] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[2] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[6] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[10] = 0xfc;
    *(u8 *)(skillRecord + 0x54) = 10;
    capValue = 0xe2;
    *(u8 *)(skillRecord + 0x55) = 10;
    *(u8 *)(skillRecord + 0x56) = 10;
    *(u8 *)(skillRecord + 0x57) = 10;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0xf5;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xb9;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xb9;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0xb9;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0xf0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 1;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0xf5;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0xf5;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0xf5;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0xf5;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0xf5;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0xb9;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xe2;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xe2;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xe2;
    goto setCapD0;
  case 0x20:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c7dc));
    ((skill_record*)skillRecord)->reuseDelayMs = 360000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 4;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    ((skill_record*)skillRecord)->readyIconIndex = 0x98;
    *(u8 *)(skillRecord + 0x57) = 4;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x99;
    goto setBytes25And26;
  case 0x21:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c7d4));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0xf;
    *(u8 *)(skillRecord + 0x4a) = 0x12;
    *(u8 *)(skillRecord + 0x4b) = 0xf;
    *(u8 *)(skillRecord + 0x4d) = 0xf;
    *(u8 *)(skillRecord + 0x4c) = 0xf;
    *(u8 *)(skillRecord + 0x4e) = 0x12;
    *(u8 *)(skillRecord + 0x4f) = 0xf;
    *(u8 *)(skillRecord + 0x50) = 0xf;
    *(u8 *)(skillRecord + 0x51) = 0xf;
    *(u8 *)(skillRecord + 0x52) = 0x10;
    *(u8 *)(skillRecord + 0x53) = 0x12;
    *(u8 *)(skillRecord + 0x54) = 0x12;
    *(u8 *)(skillRecord + 0x55) = 0x12;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xfc;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xfc;
    *(u8 *)(skillRecord + 0x56) = 0x12;
    *(u8 *)(skillRecord + 0x57) = 0xf;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xe6;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x8c;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x8c;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x8c;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x8c;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    capValue = 0xe1;
setSharedCaps:
    ((skill_record*)skillRecord)->expansionCapByClass[5] = capValue;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = capValue;
    ((skill_record*)skillRecord)->expansionCapByClass[8] = capValue;
setCapD0:
    ((skill_record*)skillRecord)->expansionCapByClass[15] = capValue;
    break;
  case 0x22:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c7cc));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0x11;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0x11;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0x12;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x35;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0xc;
    *(u8 *)(skillRecord + 0x4a) = 0xc;
    *(u8 *)(skillRecord + 0x4b) = 0xc;
    *(u8 *)(skillRecord + 0x4d) = 0xc;
    *(u8 *)(skillRecord + 0x4c) = 0xc;
    *(u8 *)(skillRecord + 0x4e) = 0xc;
    *(u8 *)(skillRecord + 0x4f) = 0xc;
    *(u8 *)(skillRecord + 0x50) = 0xc;
    *(u8 *)(skillRecord + 0x51) = 0xc;
    *(u8 *)(skillRecord + 0x52) = 0xc;
    *(u8 *)(skillRecord + 0x53) = 0xc;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xe6;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xe6;
    *(u8 *)(skillRecord + 0x54) = 0xc;
    *(u8 *)(skillRecord + 0x55) = 0xc;
    *(u8 *)(skillRecord + 0x56) = 0xc;
    *(u8 *)(skillRecord + 0x57) = 0xc;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0xb9;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xdc;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xcd;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xcd;
    break;
  case 0x23:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c7a8));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x28;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 6;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x10;
    *(u8 *)(skillRecord + 0x4a) = 0x10;
    *(u8 *)(skillRecord + 0x4b) = 0x10;
    *(u8 *)(skillRecord + 0x4d) = 0x10;
    *(u8 *)(skillRecord + 0x4c) = 0x10;
    *(u8 *)(skillRecord + 0x4e) = 0x10;
    *(u8 *)(skillRecord + 0x4f) = 0x10;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 4;
    *(u8 *)(skillRecord + 0x52) = 0x10;
    *(u8 *)(skillRecord + 0x53) = 0x10;
    *(u8 *)(skillRecord + 0x54) = 0x10;
    *(u8 *)(skillRecord + 0x55) = 0x10;
    *(u8 *)(skillRecord + 0x56) = 0x10;
    ((skill_record*)skillRecord)->readyIconIndex = 0x94;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x95;
    *(u8 *)(skillRecord + 0x57) = 0x10;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
setCapB8:
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xd2;
    break;
  case 0x24:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c79c));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xf0;
    ((skill_record*)skillRecord)->expansionCapByClass[4] = 0xf0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0xd2;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x6e;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfa;
    ((skill_record*)skillRecord)->expansionCapByClass[8] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xe1;
    ((skill_record*)skillRecord)->unk_0x090 = 0x3d4ccccd;
    break;
  case 0x25:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c784));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0x19;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0x23;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x23;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x3a;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0x28;
    *(u8 *)(skillRecord + 0x49) = 0xc;
    *(u8 *)(skillRecord + 0x4a) = 0xc;
    *(u8 *)(skillRecord + 0x4b) = 0xc;
    *(u8 *)(skillRecord + 0x4d) = 0xc;
    *(u8 *)(skillRecord + 0x4c) = 0xc;
    *(u8 *)(skillRecord + 0x4e) = 0xc;
    *(u8 *)(skillRecord + 0x4f) = 0xc;
    *(u8 *)(skillRecord + 0x50) = 0xc;
    *(u8 *)(skillRecord + 0x51) = 0xc;
    *(u8 *)(skillRecord + 0x52) = 0xc;
    *(u8 *)(skillRecord + 0x53) = 0xc;
    *(u8 *)(skillRecord + 0x54) = 0xc;
    *(u8 *)(skillRecord + 0x55) = 0xc;
    *(u8 *)(skillRecord + 0x56) = 0xc;
    *(u8 *)(skillRecord + 0x57) = 0xc;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xaf;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x96;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0x7d;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 0xe1;
    ((skill_record*)skillRecord)->expansionCapByClass[5] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[3] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[15] = 0xb9;
    break;
  case 0x26:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c778));
    ((skill_record*)skillRecord)->reuseDelayMs = 0x2134;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 5;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 5;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 9;
    *(u8 *)(skillRecord + 0x4a) = 9;
    *(u8 *)(skillRecord + 0x4b) = 9;
    *(u8 *)(skillRecord + 0x4d) = 9;
    *(u8 *)(skillRecord + 0x4c) = 9;
    *(u8 *)(skillRecord + 0x4e) = 9;
    *(u8 *)(skillRecord + 0x4f) = 9;
    *(u8 *)(skillRecord + 0x50) = 9;
    *(u8 *)(skillRecord + 0x51) = 9;
    *(u8 *)(skillRecord + 0x52) = 9;
    *(u8 *)(skillRecord + 0x53) = 9;
    *(u8 *)(skillRecord + 0x54) = 9;
    *(u8 *)(skillRecord + 0x55) = 9;
    *(u8 *)(skillRecord + 0x56) = 9;
    ((skill_record*)skillRecord)->readyIconIndex = 0x82;
    *(u8 *)(skillRecord + 0x57) = 9;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xe1;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x83;
setFloat90Quarter:
    ((skill_record*)skillRecord)->unk_0x090 = 0x3e800000;
    goto setByte25ThenByte26;
  case 0x27:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c76c));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 3;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x18;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0xc;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 5;
    *(u8 *)(skillRecord + 0x4a) = 5;
    *(u8 *)(skillRecord + 0x4b) = 5;
    *(u8 *)(skillRecord + 0x4d) = 5;
    *(u8 *)(skillRecord + 0x4c) = 5;
    *(u8 *)(skillRecord + 0x4e) = 5;
    *(u8 *)(skillRecord + 0x4f) = 5;
    *(u8 *)(skillRecord + 0x50) = 5;
    *(u8 *)(skillRecord + 0x51) = 5;
    *(u8 *)(skillRecord + 0x52) = 5;
    *(u8 *)(skillRecord + 0x53) = 5;
    *(u8 *)(skillRecord + 0x54) = 5;
    *(u8 *)(skillRecord + 0x55) = 5;
    *(u8 *)(skillRecord + 0x56) = 5;
    *(u8 *)(skillRecord + 0x57) = 5;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x28;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0x5e;
    goto zeroFrom72;
  case 0x28:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c75c));
    ((skill_record*)skillRecord)->reuseDelayMs = 2000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 7;
    *(u8 *)(skillRecord + 0x4a) = 7;
    *(u8 *)(skillRecord + 0x4b) = 7;
    *(u8 *)(skillRecord + 0x4d) = 7;
    *(u8 *)(skillRecord + 0x4c) = 7;
    *(u8 *)(skillRecord + 0x4e) = 7;
    *(u8 *)(skillRecord + 0x4f) = 7;
    *(u8 *)(skillRecord + 0x50) = 7;
    *(u8 *)(skillRecord + 0x51) = 7;
    *(u8 *)(skillRecord + 0x52) = 7;
    *(u8 *)(skillRecord + 0x53) = 7;
    *(u8 *)(skillRecord + 0x54) = 7;
    *(u8 *)(skillRecord + 0x55) = 7;
    *(u8 *)(skillRecord + 0x56) = 7;
    ((skill_record*)skillRecord)->readyIconIndex = 0x70;
    *(u8 *)(skillRecord + 0x57) = 7;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x71;
    goto setByte25;
  case 0x29:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c748));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    goto fillFrom31;
  case 0x2a:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c740));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 8;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x11;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 3;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    ((skill_record*)skillRecord)->readyIconIndex = 0x6a;
    *(u8 *)(skillRecord + 0x57) = 4;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0x71;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x6b;
    goto setByte25;
  case 0x2b:
    skillName = ((char *)0x0055c72c);
    goto initGroup2B;
  case 0x2c:
    skillName = ((char *)0x0055c714);
    goto initGroup2B;
  case 0x2d:
    skillName = ((char *)0x0055c6fc);
    goto initGroup2B;
  case 0x2e:
    skillName = ((char *)0x0055c6e4);
    goto initGroup2B;
  case 0x2f:
    skillName = ((char *)0x0055c6cc);
initGroup2B:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)skillName);
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 0x1e;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x1c;
    *(u8 *)(skillRecord + 0x4a) = 0x1c;
    *(u8 *)(skillRecord + 0x4b) = 0x1c;
    *(u8 *)(skillRecord + 0x4d) = 0x1c;
    *(u8 *)(skillRecord + 0x4c) = 0x1c;
    *(u8 *)(skillRecord + 0x4e) = 0x1c;
    *(u8 *)(skillRecord + 0x4f) = 0x1c;
    *(u8 *)(skillRecord + 0x50) = 0x1c;
    *(u8 *)(skillRecord + 0x51) = 0x1c;
    *(u8 *)(skillRecord + 0x52) = 0x1c;
    *(u8 *)(skillRecord + 0x53) = 0x1c;
    *(u8 *)(skillRecord + 0x54) = 0x1c;
    *(u8 *)(skillRecord + 0x55) = 0x1c;
    *(u8 *)(skillRecord + 0x56) = 0x1c;
    *(u8 *)(skillRecord + 0x57) = 0x1c;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
setBytes73To76:
    ((skill_record*)skillRecord)->baseCapByClass[11] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 200;
    goto zero77;
  case 0x30:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c6bc));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 7;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 4;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    ((skill_record*)skillRecord)->readyIconIndex = 0x92;
    *(u8 *)(skillRecord + 0x57) = 4;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x93;
    goto setByte25;
  case 0x31:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c6a4));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 8;
    goto fillFrom31;
  case 0x32:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c698));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 200;
    goto setByte77;
  case 0x33:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c680));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 2;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 2;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 2;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    *(u8 *)(skillRecord + 0x57) = 2;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0x71;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x71;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0x71;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x71;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0xdc;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0x71;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[1] = 200;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfa;
setFloat90Tenth:
    ((skill_record*)skillRecord)->unk_0x090 = 0x3dcccccd;
    break;
  case 0x34:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c674));
    ((skill_record*)skillRecord)->reuseDelayMs = 0x157c;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 4;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 9;
    *(u8 *)(skillRecord + 0x4a) = 9;
    *(u8 *)(skillRecord + 0x4b) = 9;
    *(u8 *)(skillRecord + 0x4d) = 9;
    *(u8 *)(skillRecord + 0x4c) = 9;
    *(u8 *)(skillRecord + 0x4e) = 9;
    *(u8 *)(skillRecord + 0x4f) = 9;
    *(u8 *)(skillRecord + 0x50) = 9;
    *(u8 *)(skillRecord + 0x51) = 9;
    *(u8 *)(skillRecord + 0x52) = 9;
    *(u8 *)(skillRecord + 0x53) = 9;
    *(u8 *)(skillRecord + 0x54) = 9;
    *(u8 *)(skillRecord + 0x55) = 9;
    *(u8 *)(skillRecord + 0x56) = 9;
    ((skill_record*)skillRecord)->readyIconIndex = 0x88;
    *(u8 *)(skillRecord + 0x57) = 9;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[7] = 0xe1;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x89;
setFloat90Fifth:
    ((skill_record*)skillRecord)->unk_0x090 = 0x3e4ccccd;
setByte25ThenByte26:
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
setByte26To2:
    ((skill_record*)skillRecord)->activationMode_guess = 2;
    break;
  case 0x35:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c668));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x23;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    ((skill_record*)skillRecord)->readyIconIndex = 0x78;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0x7d;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x79;
setByte25:
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    break;
  case 0x36:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c644));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0xe;
    goto fillFrom31;
  case 0x37:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c9b4));
    ((skill_record*)skillRecord)->reuseDelayMs = 12000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->levelBasedCap = 1;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    goto setIds8A8C;
  case 0x38:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c9bc));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 2;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    *(u8 *)(skillRecord + 0x57) = 4;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->expansionCapByClass[9] = 0xfa;
    break;
  case 0x39:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c9c8));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0x10;
    ((skill_record*)skillRecord)->levelBasedCap = 0;
    *(u8 *)(skillRecord + 0x49) = 3;
    *(u8 *)(skillRecord + 0x4a) = 3;
    *(u8 *)(skillRecord + 0x4b) = 3;
    *(u8 *)(skillRecord + 0x4d) = 3;
    *(u8 *)(skillRecord + 0x4c) = 3;
    *(u8 *)(skillRecord + 0x4e) = 3;
    *(u8 *)(skillRecord + 0x4f) = 3;
    *(u8 *)(skillRecord + 0x50) = 3;
    *(u8 *)(skillRecord + 0x51) = 3;
    *(u8 *)(skillRecord + 0x52) = 3;
    *(u8 *)(skillRecord + 0x53) = 3;
    *(u8 *)(skillRecord + 0x54) = 3;
    *(u8 *)(skillRecord + 0x55) = 3;
    *(u8 *)(skillRecord + 0x56) = 3;
    *(u8 *)(skillRecord + 0x57) = 3;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0xfa;
setByte77:
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    break;
  case 0x3a:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c9d4));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 0x10;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 1;
    *(u8 *)(skillRecord + 0x4a) = 1;
    *(u8 *)(skillRecord + 0x4b) = 1;
    *(u8 *)(skillRecord + 0x4d) = 1;
    *(u8 *)(skillRecord + 0x4c) = 1;
    *(u8 *)(skillRecord + 0x4e) = 1;
    *(u8 *)(skillRecord + 0x4f) = 1;
    *(u8 *)(skillRecord + 0x50) = 1;
    *(u8 *)(skillRecord + 0x51) = 1;
    *(u8 *)(skillRecord + 0x52) = 1;
    *(u8 *)(skillRecord + 0x53) = 1;
    *(u8 *)(skillRecord + 0x54) = 1;
    *(u8 *)(skillRecord + 0x55) = 1;
    *(u8 *)(skillRecord + 0x56) = 1;
    *(u8 *)(skillRecord + 0x57) = 1;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    goto setBytes73To76;
  case 0x3b:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c9e0));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 0x19;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 6;
    *(u8 *)(skillRecord + 0x4a) = 6;
    *(u8 *)(skillRecord + 0x4b) = 6;
    *(u8 *)(skillRecord + 0x4d) = 6;
    *(u8 *)(skillRecord + 0x4c) = 6;
    *(u8 *)(skillRecord + 0x4e) = 6;
    *(u8 *)(skillRecord + 0x4f) = 6;
    *(u8 *)(skillRecord + 0x50) = 6;
    *(u8 *)(skillRecord + 0x51) = 6;
    *(u8 *)(skillRecord + 0x52) = 6;
    *(u8 *)(skillRecord + 0x53) = 6;
    *(u8 *)(skillRecord + 0x54) = 6;
    *(u8 *)(skillRecord + 0x55) = 6;
    *(u8 *)(skillRecord + 0x56) = 6;
    *(u8 *)(skillRecord + 0x57) = 6;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    goto zeroFrom73;
  case 0x3c:
    skillName = ((char *)0x0055c92c);
    goto initGroup3C;
  case 0x3d:
    skillName = ((char *)0x0055c68c);
    goto initGroup3C;
  case 0x3e:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c750));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x14;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 8;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 8;
    *(u8 *)(skillRecord + 0x4a) = 8;
    *(u8 *)(skillRecord + 0x4b) = 8;
    *(u8 *)(skillRecord + 0x4d) = 8;
    *(u8 *)(skillRecord + 0x4c) = 8;
    *(u8 *)(skillRecord + 0x4e) = 8;
    *(u8 *)(skillRecord + 0x4f) = 8;
    *(u8 *)(skillRecord + 0x50) = 8;
    *(u8 *)(skillRecord + 0x51) = 8;
    *(u8 *)(skillRecord + 0x52) = 8;
    *(u8 *)(skillRecord + 0x53) = 8;
    *(u8 *)(skillRecord + 0x54) = 8;
    *(u8 *)(skillRecord + 0x55) = 8;
    *(u8 *)(skillRecord + 0x56) = 8;
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    ((skill_record*)skillRecord)->readyIconIndex = 0x9c;
    *(u8 *)(skillRecord + 0x57) = 8;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0x4b;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x9d;
    break;
  case 0x3f:
    skillName = ((char *)0x0055c8f4);
    goto initGroup3C;
  case 0x40:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c834));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->levelBasedCap = 0;
    *(u8 *)(skillRecord + 0x49) = 3;
    *(u8 *)(skillRecord + 0x4a) = 3;
    *(u8 *)(skillRecord + 0x4b) = 3;
    *(u8 *)(skillRecord + 0x4d) = 3;
    *(u8 *)(skillRecord + 0x4c) = 3;
    *(u8 *)(skillRecord + 0x4e) = 3;
    *(u8 *)(skillRecord + 0x4f) = 3;
    *(u8 *)(skillRecord + 0x50) = 3;
    *(u8 *)(skillRecord + 0x51) = 3;
    *(u8 *)(skillRecord + 0x52) = 3;
    *(u8 *)(skillRecord + 0x53) = 3;
    *(u8 *)(skillRecord + 0x54) = 3;
    *(u8 *)(skillRecord + 0x55) = 3;
    *(u8 *)(skillRecord + 0x56) = 3;
    *(u8 *)(skillRecord + 0x57) = 3;
    goto setBytes69To77;
  case 0x41:
    skillName = ((char *)0x0055c8d0);
    goto initGroup3C;
  case 0x42:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c964));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 1;
    *(u8 *)(skillRecord + 0x4a) = 1;
    *(u8 *)(skillRecord + 0x4b) = 1;
    *(u8 *)(skillRecord + 0x4d) = 1;
    *(u8 *)(skillRecord + 0x4c) = 1;
    *(u8 *)(skillRecord + 0x4e) = 1;
    *(u8 *)(skillRecord + 0x4f) = 1;
    *(u8 *)(skillRecord + 0x50) = 1;
    *(u8 *)(skillRecord + 0x51) = 1;
    *(u8 *)(skillRecord + 0x52) = 1;
    *(u8 *)(skillRecord + 0x53) = 1;
    *(u8 *)(skillRecord + 0x54) = 1;
    *(u8 *)(skillRecord + 0x55) = 1;
    *(u8 *)(skillRecord + 0x56) = 1;
    *(u8 *)(skillRecord + 0x57) = 1;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    goto setIds8A8C;
  case 0x43:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c910));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x1c;
    *(u8 *)(skillRecord + 0x4a) = 0x1c;
    *(u8 *)(skillRecord + 0x4b) = 0x1c;
    *(u8 *)(skillRecord + 0x4d) = 0x1c;
    *(u8 *)(skillRecord + 0x4c) = 0x1c;
    *(u8 *)(skillRecord + 0x4e) = 0x1c;
    *(u8 *)(skillRecord + 0x4f) = 0x1c;
    *(u8 *)(skillRecord + 0x50) = 0x1c;
    *(u8 *)(skillRecord + 0x51) = 0x1c;
    *(u8 *)(skillRecord + 0x52) = 0x1c;
    *(u8 *)(skillRecord + 0x53) = 0x1c;
    *(u8 *)(skillRecord + 0x54) = 0x1c;
    *(u8 *)(skillRecord + 0x55) = 0x1c;
    *(u8 *)(skillRecord + 0x56) = 0x1c;
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    ((skill_record*)skillRecord)->readyIconIndex = 0x7a;
    *(u8 *)(skillRecord + 0x57) = 0x1c;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 200;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x7b;
    break;
  case 0x44:
    skillName = ((char *)0x0055c7f8);
initGroup3C:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)skillName);
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->levelBasedCap = 0;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 4;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    *(u8 *)(skillRecord + 0x57) = 4;
setBytes69To77:
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0xfa;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0xfa;
setIds8A8C:
    ((skill_record*)skillRecord)->readyIconIndex = 0x68;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x69;
    break;
  case 0x45:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c794));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    ((skill_record*)skillRecord)->levelBasedCap = 0;
    *(u8 *)(skillRecord + 0x49) = 1;
    *(u8 *)(skillRecord + 0x4a) = 1;
    *(u8 *)(skillRecord + 0x4b) = 1;
    *(u8 *)(skillRecord + 0x4d) = 1;
    *(u8 *)(skillRecord + 0x4c) = 1;
    *(u8 *)(skillRecord + 0x4e) = 1;
    *(u8 *)(skillRecord + 0x4f) = 1;
    *(u8 *)(skillRecord + 0x50) = 1;
    *(u8 *)(skillRecord + 0x51) = 1;
    *(u8 *)(skillRecord + 0x52) = 1;
    *(u8 *)(skillRecord + 0x53) = 1;
    *(u8 *)(skillRecord + 0x54) = 1;
    *(u8 *)(skillRecord + 0x55) = 1;
    *(u8 *)(skillRecord + 0x56) = 1;
    *(u8 *)(skillRecord + 0x57) = 1;
    goto setBytes69To77;
  case 0x46:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c7b4));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 5;
fillFrom31:
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 0x14;
    *(u8 *)(skillRecord + 0x4a) = 0x14;
    *(u8 *)(skillRecord + 0x4b) = 0x14;
    *(u8 *)(skillRecord + 0x4d) = 0x14;
    *(u8 *)(skillRecord + 0x4c) = 0x14;
    *(u8 *)(skillRecord + 0x4e) = 0x14;
    *(u8 *)(skillRecord + 0x4f) = 0x14;
    *(u8 *)(skillRecord + 0x50) = 0x14;
    *(u8 *)(skillRecord + 0x51) = 0x14;
    *(u8 *)(skillRecord + 0x52) = 0x14;
    *(u8 *)(skillRecord + 0x53) = 0x14;
    *(u8 *)(skillRecord + 0x54) = 0x14;
    *(u8 *)(skillRecord + 0x55) = 0x14;
    *(u8 *)(skillRecord + 0x56) = 0x14;
    *(u8 *)(skillRecord + 0x57) = 0x14;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0xeb;
zeroFrom71:
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
zeroFrom72:
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
zeroFrom73:
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
zero77:
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    break;
  case 0x47:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c658));
    ((skill_record*)skillRecord)->reuseDelayMs = 10000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 2;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0x12;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0x1a;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0x16;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0;
    *(u8 *)(skillRecord + 0x49) = 0xc;
    *(u8 *)(skillRecord + 0x4a) = 0xc;
    *(u8 *)(skillRecord + 0x4b) = 0xc;
    *(u8 *)(skillRecord + 0x4d) = 0xc;
    *(u8 *)(skillRecord + 0x4c) = 0xc;
    *(u8 *)(skillRecord + 0x4e) = 0xc;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 6;
    *(u8 *)(skillRecord + 0x51) = 2;
    *(u8 *)(skillRecord + 0x52) = 0xc;
    *(u8 *)(skillRecord + 0x53) = 0xc;
    *(u8 *)(skillRecord + 0x54) = 0xc;
    *(u8 *)(skillRecord + 0x55) = 0xc;
    *(u8 *)(skillRecord + 0x56) = 0xc;
    ((skill_record*)skillRecord)->readyIconIndex = 0x9a;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x9b;
    *(u8 *)(skillRecord + 0x57) = 0xc;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
    ((skill_record*)skillRecord)->unk_0x090 = 0x40200000;
    goto setBytes25And26;
  case 0x48:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c904));
    ((skill_record*)skillRecord)->reuseDelayMs = 0x1d4c;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0;
    *(u8 *)(skillRecord + 0x49) = 0xc;
    *(u8 *)(skillRecord + 0x4a) = 0xc;
    *(u8 *)(skillRecord + 0x4b) = 0xc;
    *(u8 *)(skillRecord + 0x4d) = 0xc;
    *(u8 *)(skillRecord + 0x4c) = 0xc;
    *(u8 *)(skillRecord + 0x4e) = 0xc;
    *(u8 *)(skillRecord + 0x4f) = 0xc;
    *(u8 *)(skillRecord + 0x50) = 0xc;
    *(u8 *)(skillRecord + 0x51) = 0xc;
    *(u8 *)(skillRecord + 0x52) = 0xc;
    *(u8 *)(skillRecord + 0x53) = 0xc;
    *(u8 *)(skillRecord + 0x54) = 0xc;
    *(u8 *)(skillRecord + 0x55) = 0xc;
    *(u8 *)(skillRecord + 0x56) = 0xc;
    ((skill_record*)skillRecord)->readyIconIndex = 0x9e;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0x9f;
    ((skill_record*)skillRecord)->unk_0x090 = 0;
    *(u8 *)(skillRecord + 0x57) = 0xc;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 0;
setBytes25And26:
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    ((skill_record*)skillRecord)->activationMode_guess = 1;
    break;
  case 0x49:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c78c));
    ((skill_record*)skillRecord)->reuseDelayMs = 6000;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->activationMode_guess = 1;
    ((skill_record*)skillRecord)->showInAbilities_guess = 1;
    ((skill_record*)skillRecord)->readyIconIndex = 0xa0;
    ((skill_record*)skillRecord)->inactiveOrPressedIconIndex = 0xa1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 1;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 1;
    *(u8 *)(skillRecord + 0x49) = 4;
    *(u8 *)(skillRecord + 0x4a) = 4;
    *(u8 *)(skillRecord + 0x4b) = 4;
    *(u8 *)(skillRecord + 0x4d) = 4;
    *(u8 *)(skillRecord + 0x4c) = 4;
    *(u8 *)(skillRecord + 0x4e) = 4;
    *(u8 *)(skillRecord + 0x4f) = 4;
    *(u8 *)(skillRecord + 0x50) = 4;
    *(u8 *)(skillRecord + 0x51) = 4;
    *(u8 *)(skillRecord + 0x52) = 4;
    *(u8 *)(skillRecord + 0x53) = 4;
    *(u8 *)(skillRecord + 0x54) = 4;
    *(u8 *)(skillRecord + 0x55) = 4;
    *(u8 *)(skillRecord + 0x56) = 4;
    *(u8 *)(skillRecord + 0x57) = 4;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 200;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0xb4;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0xb4;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0x96;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 100;
    ((skill_record*)skillRecord)->baseCapByClass[9] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[10] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[11] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[12] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[13] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[14] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[15] = 100;
    break;
  default:
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)skillRecord, (const char *)((char *)0x0055c63c));
    ((skill_record*)skillRecord)->reuseDelayMs = 0;
    *(u8 *)(skillRecord + 0x24) = 0;
    ((skill_record*)skillRecord)->baseDamage = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[1] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[2] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[3] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[5] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[4] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[6] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[7] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[8] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[9] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[10] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[11] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[12] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[13] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[14] = 0;
    ((skill_record*)skillRecord)->minimumLevelByClass_guess[15] = 0;
    *(u8 *)(skillRecord + 0x49) = 0;
    *(u8 *)(skillRecord + 0x4a) = 0;
    *(u8 *)(skillRecord + 0x4b) = 0;
    *(u8 *)(skillRecord + 0x4d) = 0;
    *(u8 *)(skillRecord + 0x4c) = 0;
    *(u8 *)(skillRecord + 0x4e) = 0;
    *(u8 *)(skillRecord + 0x4f) = 0;
    *(u8 *)(skillRecord + 0x50) = 0;
    *(u8 *)(skillRecord + 0x51) = 0;
    *(u8 *)(skillRecord + 0x52) = 0;
    *(u8 *)(skillRecord + 0x53) = 0;
    *(u8 *)(skillRecord + 0x54) = 0;
    *(u8 *)(skillRecord + 0x55) = 0;
    *(u8 *)(skillRecord + 0x56) = 0;
    *(u8 *)(skillRecord + 0x57) = 0;
    ((skill_record*)skillRecord)->baseCapByClass[1] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[2] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[3] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[5] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[4] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[6] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[7] = 0;
    ((skill_record*)skillRecord)->baseCapByClass[8] = 0;
    goto zeroFrom71;
  }
  if (skillId < 0) goto fillUnsetCaps;
  if (1 < skillId) {
    if (skillId < 4) {
      *(u8 *)(skillRecord + 0x49) = 0xc;
      *(u8 *)(skillRecord + 0x4a) = 0x11;
      *(u8 *)(skillRecord + 0x4b) = 0xc;
      *(u8 *)(skillRecord + 0x4d) = 0xc;
      *(u8 *)(skillRecord + 0x4c) = 0xc;
      *(u8 *)(skillRecord + 0x4e) = 0x11;
      *(u8 *)(skillRecord + 0x4f) = 0xc;
      *(u8 *)(skillRecord + 0x50) = 0xc;
      *(u8 *)(skillRecord + 0x51) = 0xc;
      *(u8 *)(skillRecord + 0x52) = 0xf;
      *(u8 *)(skillRecord + 0x53) = 0x13;
      *(u8 *)(skillRecord + 0x54) = 0x13;
      *(u8 *)(skillRecord + 0x55) = 0x13;
      *(u8 *)(skillRecord + 0x56) = 0x13;
      *(u8 *)(skillRecord + 0x57) = 0xc;
      goto fillUnsetCaps;
    }
    if (skillId != 0x24) goto fillUnsetCaps;
  }
  *(u8 *)(skillRecord + 0x49) = 0xf;
  *(u8 *)(skillRecord + 0x4a) = 0x12;
  *(u8 *)(skillRecord + 0x4b) = 0xf;
  *(u8 *)(skillRecord + 0x4d) = 0xf;
  *(u8 *)(skillRecord + 0x4c) = 0xf;
  *(u8 *)(skillRecord + 0x4e) = 0xf;
  *(u8 *)(skillRecord + 0x4f) = 0xf;
  *(u8 *)(skillRecord + 0x50) = 0xf;
  *(u8 *)(skillRecord + 0x51) = 0xf;
  *(u8 *)(skillRecord + 0x52) = 0x11;
  *(u8 *)(skillRecord + 0x53) = 0x14;
  *(u8 *)(skillRecord + 0x54) = 0x14;
  *(u8 *)(skillRecord + 0x55) = 0x14;
  *(u8 *)(skillRecord + 0x56) = 0x14;
  *(u8 *)(skillRecord + 0x57) = 0xf;
fillUnsetCaps:
  dwordCap = (u32 *)(skillRecord + 0x94);
  do {
    if (*dwordCap == 0xffffffff) {
      if (*byteCap == 0xffffffff) {
        *byteCap = 0;
      }
      *dwordCap = (u32)*byteCap;
    }
    dwordCap = dwordCap + 1;
    byteCap = byteCap + 1;
  } while ((int)(byteCap + (-0x68 - skillRecord)) < 0x20);
  return skillRecord;
}

class EQ_HSliderCtorBase_guess
{
public:
    EQ_HSliderCtorBase_guess()
    {
        EQ_METHOD(0, void, 0x00450049, (void*), this)();
    }

    ~EQ_HSliderCtorBase_guess()
    {
        EQ_METHOD(0, void, 0x0045005D, (void*), this)();
    }
};

class EQ_HSliderCtor_guess : public EQ_HSliderCtorBase_guess
{
public:
    EQ_HSliderCtor_guess();
    ~EQ_HSliderCtor_guess();
};

// FUNCTION: TRILOGY 0x00485de2
// Initializes the quantity selection window's controls and slider artwork.
// @early-stop: frame and register allocation differ (setRect stays in memory, zero/regionId in other registers; EQ_THISCALL stand-ins add xor edx lines).
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
EQ_HSliderCtor_guess::EQ_HSliderCtor_guess()
{
    typedef BOOL (WINAPI *SetRect_guess_t)(LPRECT, int, int, int, int);
    typedef BOOL (WINAPI *CopyRect_guess_t)(LPRECT, const RECT*);
    EQ_HSlider* slider = (EQ_HSlider*)this;
    u32* self;
    char* element;

    self = (u32*)this;
    *(short*)((char*)self + 0x1cc) = -1;
    *(u32*)self = 0x00535648;
    self[4] = 0;
    self[0x1c] = 0;
    self[0x77] = 0;
    self[0x71] = 0;
    self[0x7b] = 0;
    self[0x72] = 0;
    self[0x7a] = 0;
    self[0x79] = 0;
    *(u16*)((char*)self + 0x1e0) = 0;
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(&slider->backgroundRect_guess, 0xfc, 0x8a, 0x17c, 0xca);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(&slider->finishRect_guess, 0x13e, 0x98, 0x16c, 0xa9);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)((LPRECT)(self + 0x28), 0x10a, 0xb3, 0x11b, 0xbd);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)((LPRECT)(self + 0x2c), 0x15d, 0xb3, 0x16f, 0xbd);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(&slider->thumb_copy_guess, 0x153, 0xb2, 0x15d, 0xbd);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(&slider->region5Rect_guess, 0x113, 0x9b, 0x118, 0xaa);
    {
        u32* rect = self + 0x24;
        int regionId;
        for (regionId = 1; (short)regionId < 6; regionId = regionId + 1)
        {
            EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), this)(regionId, rect, -1, -1, 0, 0);
            rect = rect + 4;
        }
    }
    EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), this)(0, self + 0x20, -1, -1, 0, 0);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)(self + 0xd), (const char*)0x0053e950);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)self + 0x52, (const char*)0x0055ca88);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)((LPRECT)(self + 5), 0, 0, 0xd8, 0x85);
    EQ_GLOBAL(CopyRect_guess_t, 0x00534240)((LPRECT)(self + 9), (RECT*)(self + 0x20));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, 0);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)((LPRECT)(self + 5), 0, 0, 0x80, 0x40);
    element = (char*)self[0x1c];
    self[0x70] = 0;
    for (; element != NULL; element = *(char**)(element + 0x20))
    {
        if (*(short*)(element + 0x14) == 4)
        {
            self[0x70] = (u32)(element + 4);
        }
    }
}

// FUNCTION: TRILOGY 0x00485FBC
// Releases the quantity slider's buttons, then runs the base overlay teardown.
// @early-stop: the raw vtable store is scheduled after the EH-state store (original: before, as a compiler vfptr store).
EQ_HSliderCtor_guess::~EQ_HSliderCtor_guess()
{
    *(u32*)this = 0x00535648;
    EQ_METHOD(0, void, 0x00450463, (void*), this)();
}
#pragma optimize("y", on)

// FUNCTION: TRILOGY 0x00485ff4
// Shows hover help for finishing, decreasing, increasing, or dragging the quantity selection.
// @early-stop: raw bytes differ only by the EQ_THISCALL stand-in's dummy EDX (xor edx,edx before the call); match.sh normalises it.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
void __fastcall EQ_HSlider_AutoHelp_guess(EQ_HSlider *slider, int)
{
    typedef BOOL (WINAPI *PtInRectProc_guess)(const RECT *, POINT);
    POINT point;
    BOOL inRect;
    int element;
    short selector;
    char buffer[128];

    selector = -1;
    if (EQ_GLOBAL(int, 0x006B6864) != 0 &&
        pinstEQ_Main_guess->hoverTicks_guess >= 3)
    {
        point.x = (LONG)EQ_GLOBAL(short, 0x006B6414);
        buffer[0] = 0;
        point.y = (LONG)EQ_GLOBAL(short, 0x006B6416);
        if (*(int *)((char *)slider + 0x10) != 0)
        {
            for (element = *(int *)((char *)slider + 0x70); element != 0;
                element = *(int *)((char *)element + 0x20))
            {
                inRect = EQ_GLOBAL(PtInRectProc_guess, 0x0053423C)
                    ((RECT *)((char *)element + 4), point);
                if (inRect != 0)
                {
                    selector = *(short *)((char *)element + 0x14);
                    break;
                }
            }
            pinstEQ_Main_guess->hoverId_guess = selector;
            if (selector != -1)
            {
                switch (selector)
                {
                case 1:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CA94));
                    break;
                case 2:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CAB0));
                    break;
                case 3:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CAC4));
                    break;
                case 4:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CAD8));
                    break;
                default:
                    *(short*)((char *)EQ_GLOBAL(void *, 0x006E4E44) + 0xebc) = -1;
                    return;
                }
                EQ_METHOD(4, void, 0x0046D4DA, (void *, LONG, LONG, const char *, int), EQ_GLOBAL(void *, 0x006E4E44))(point.x, point.y + 0x14, buffer, 10);
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004860e5
// Displays the first quantity selector's current amount, slider thumb, and pressed buttons.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
// @early-stop: a live local char[80] gets an EBP frame here (original is ESP), so oldTop/deltaX spill and the zero/one constants differ.
/* fake match: "text + 12" puts the sprintf buffer at the original's stack address (this build has an EBP frame); the fixture compares that pointer. */
void __fastcall EQ_HSlider_draw_guess(EQ_HSlider *slider, int)
{
    typedef BOOL (WINAPI *SetRectProc_guess)(LPRECT, int, int, int, int);
    typedef BOOL (WINAPI *CopyRectProc_guess)(LPRECT, const RECT *);
    char text[80];
    RECT *track;
    void *element;
    short selector;
    int oldLeft;
    int oldTop;
    int left;
    int top;
    int deltaX;
    int deltaY;

    if (EQ_METHOD(0, int, 0x004504AC, (void *), slider)() == 0)
        return;

    if (*(int *)((char *)slider + 0x1ec) != 0)
    {
        left = EQ_GLOBAL(int, 0x006EFC80);
        top = EQ_GLOBAL(int, 0x006EFC84);
        pinstEQ_Main_guess->uiDirty_u32_guess = 1;
        oldTop = *(int *)((char *)slider + 0x28);
        oldLeft = *(int *)((char *)slider + 0x24);
        track = (RECT *)((char *)slider + 0x24);
        track->left = left;
        *(int *)((char *)slider + 0x28) = top;
        if (left > 0x200)
            track->left = 0x200;
        if (track->left < 0)
            track->left = 0;
        if (top > 0x1a0)
            *(int *)((char *)slider + 0x28) = 0x1a0;
        if (*(int *)((char *)slider + 0x28) < 0)
            *(int *)((char *)slider + 0x28) = 0;
        left = track->left;
        top = *(int *)((char *)slider + 0x28);
        deltaX = left - oldLeft;
        deltaY = top - oldTop;
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)(track, left, top, left + 0x80, top + 0x40);
        for (element = *(void **)((char *)slider + 0x70); element != 0;
             element = *(void **)((char *)element + 0x20))
        {
            EQ_METHOD(2, void, 0x00419583, (void *, int, int), element)(deltaX, deltaY);
            EQ_GLOBAL(CopyRectProc_guess, 0x00534240)
                ((LPRECT)((*(short *)((char *)element + 0x14) + 8) * 0x10 + (char *)slider),
                 (RECT *)((char *)element + 4));
        }
    }

    EQ_GLOBAL(SetRectProc_guess, 0x00534248)
        ((LPRECT)((char *)EQ_GLOBAL(void *, 0x006B341C) + 4), 0x158, 0xf2, 0x1d8, 0x132);
    *(u32 *)EQ_GLOBAL(void *, 0x006B341C) = *(u32 *)((char *)EQ_GLOBAL(void *, 0x006E4E3C) + 4);
    EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B341C))((char *)slider + 0x80, 1);

    selector = *(short *)((char *)slider + 0x1cc);
    if (selector != -1)
    {
        switch (selector)
        {
        case 1:
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B37C0))((char *)slider + 0x90, 1);
            break;
        case 2:
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B38A8))((char *)slider + 0xa0, 1);
            break;
        case 3:
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B38AC))((char *)slider + 0xb0, 1);
            break;
        }
    }

    EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B3640))((char *)slider + 0xc0, 1);
    EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
        (text + 12, (const char *)0x0053E16C, *(int *)((char *)slider + 0x1d8));
    EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, u32, int), EQ_GLOBAL(void *, 0x006E4E68))(text + 12,
         (int)*(short *)((char *)slider + 0xd0), (int)*(short *)((char *)slider + 0xd4), 7,
         EQ_GLOBAL(u32, 0x006EFDFC), 1);
}

// FUNCTION: TRILOGY 0x0048629a
// Opens quantity selection with the full amount selected, or immediately selects one or all.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
// @early-stop: deltas are scheduled before the SetRect pushes, so deltaX spills to [ebp+0xc] and VC6 adds an ebp frame the original lacks; the loop's EQ_THISCALL adds xor edx,edx
/* fake match: yTop is clamped first, value_guess is zeroed before *range is stored, and (int)vtable is the dummy EDX; they steer VC6 to the original's registers and store order */
void __fastcall EQ_HSlider_Activate_guess(EQ_HSlider *slider, int, int *coins, u8 mode, int source)
{
    typedef BOOL (WINAPI *SetRectProc_guess)(LPRECT, int, int, int, int);
    typedef BOOL (WINAPI *CopyRectProc_guess)(LPRECT, const RECT *);
    typedef void (__fastcall *ActivateCallback_guess)(EQ_HSlider *, int, int, int);
    void *window_state;
    void *ui_state;
    void *profile;
    void *other_state;
    int xLeft;
    int yTop;
    int deltaX;
    int deltaY;
    void *element;
    u32 *vtable;

    window_state = EQ_GLOBAL(void *, 0x006E4E44);
    if ((window_state == 0 || *(int *)((char *)window_state + 0x94) == 0) &&
        (ui_state = g_display_guess, *(u8 *)((char *)ui_state + 0x3d) == 0) &&
        *(u8 *)((char *)ui_state + 0x3e) == 0 &&
        (profile = EQ_GLOBAL(void *, 0x006E4DA8), *(int *)((char *)profile + 0xe0) == 0) &&
        *(u8 *)((char *)ui_state + 0x3c) == 0 &&
        (other_state = EQ_GLOBAL(void *, 0x006018C0), *(int *)((char *)other_state + 0x10) == 0))
    {
        *(int *)((char *)slider + 0x1e4) = source;
        *(int *)((char *)slider + 0x1e8) = 0;
        slider->range_guess = coins;
        slider->mode_guess = mode;
        slider->value_guess = 0;
        slider->value_guess = *coins;
        *(int *)((char *)slider + 0x10) = 1;

        xLeft = EQ_GLOBAL(int, 0x006EFC80);
        yTop = EQ_GLOBAL(int, 0x006EFC84);
        if (0x1a0 < yTop)
            yTop = 0x1a0;
        if (yTop < 0)
            yTop = 0;
        if (0x200 < xLeft)
            xLeft = 0x200;
        if (xLeft < 0)
            xLeft = 0;

        deltaY = yTop - *(int *)((char *)slider + 0x28);
        deltaX = xLeft - *(int *)((char *)slider + 0x24);
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)
            ((LPRECT)((char *)slider + 0x24), xLeft, yTop, xLeft + 0x80, yTop + 0x40);
        for (element = *(void **)((char *)slider + 0x70); element != 0;
             element = *(void **)((char *)element + 0x20))
        {
            EQ_METHOD(2, void, 0x00419583, (void *, int, int), element)(deltaX, deltaY);
            EQ_GLOBAL(CopyRectProc_guess, 0x00534240)
                ((LPRECT)((char *)slider + ((*(short *)((char *)element + 0x14) + 8) * 4) * sizeof(u32)),
                 (RECT *)((char *)element + 4));
        }

        EQ_GLOBAL(SetRectProc_guess, 0x00534248)
            (slider->thumb_guess, *(int *)((char *)slider + 0xc8), slider->thumb_guess->top,
             *(int *)((char *)slider + 0xc8) + 10, slider->thumb_guess->bottom);
        slider->thumb_guess->left = *(int *)((char *)slider + 0xb0) - 10;
        slider->thumb_guess->right = slider->thumb_guess->left + 10;
        EQ_GLOBAL(CopyRectProc_guess, 0x00534240)((LPRECT)((char *)slider + 0xc0), slider->thumb_guess);

        if (EQ_GLOBAL(int, 0x006B70EC) != 0)
        {
            vtable = *(u32 **)slider;
            slider->value_guess = 1;
            ((ActivateCallback_guess)*vtable)(slider, (int)vtable, 0, 1);
        }
        else if (EQ_GLOBAL(int, 0x006B70E8) != 0)
        {
            vtable = *(u32 **)slider;
            slider->value_guess = *slider->range_guess;
            ((ActivateCallback_guess)*vtable)(slider, (int)vtable, 0, 1);
        }
    }
}

// FUNCTION: TRILOGY 0x0048643A
// Completes or cancels quantity selection and reports the selected transfer and any source update.
void __fastcall FinishQuantitySelection_guess(void *slider)
{
    u8 selectionType;
    u32 message[5];
    u32 update[4];

    EQ_METHOD(0, void, 0x004504BA, (void *), slider)();
    if (EQ_GLOBAL(void *, 0x006E4E44) != 0)
        pinstEQ_Main_guess->uiDirty_guess = 1;

    if (*(int *)((char *)slider + 0x1E8) != 0)
    {
        *(int *)((char *)slider + 0x1DC) = 0;
        *(int *)((char *)slider + 0x1E8) = 0;
        *(int *)((char *)slider + 0x1D8) = 0;
    }
    else
    {
        if (0 < *(int *)((char *)slider + 0x1D8) &&
            *(int *)((char *)slider + 0x1D8) <= **(int **)((char *)slider + 0x1D0))
        {
            if (EQ_GLOBAL(int, 0x006E4DD8) != 0)
                EQ_METHOD(2, void, 0x004E34FA, (void *, int, int), g_sound_manager_ptr_guess)(0x8A, 0);

            g_cdisplay_guess->moneyOnCursor_guess =
                *(u8 *)((char *)slider + 0x1D4);
            **(int **)((char *)slider + 0x1D0) -= *(int *)((char *)slider + 0x1D8);

            selectionType = *(u8 *)((char *)slider + 0x1D4);
            if (selectionType == '\x01')
                *(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0x9B8) =
                    *(int *)((char *)slider + 0x1D8);
            else if (selectionType == '\x02')
                *(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0x9BC) =
                    *(int *)((char *)slider + 0x1D8);
            else if (selectionType == '\x03')
                *(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0x9C0) =
                    *(int *)((char *)slider + 0x1D8);
            else if (selectionType == '\x04')
                *(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0x9C4) =
                    *(int *)((char *)slider + 0x1D8);

            message[0] = *(u32 *)((char *)slider + 0x1E4);
            message[1] = 0;
            selectionType = *(u8 *)((char *)slider + 0x1D4);
            switch (selectionType)
            {
            case 1:
                message[2] = 3;
                break;
            case 2:
                message[2] = 2;
                break;
            case 3:
                message[2] = 1;
                break;
            case 4:
                message[2] = 0;
                break;
            }
            message[3] = message[2];
            message[4] = *(u32 *)((char *)slider + 0x1D8);
            send_message_guess(
                g_world_connection_guess, 0x212D, message, 0x14, 1);
        }

        if (*(int *)((char *)slider + 0x1DC) != 0)
        {
            update[0] = g_local_player_guess->spawn_id;
            update[1] = *(short *)((char *)EQ_GLOBAL(void *, 0x006E4DCC) + 0x62);
            update[2] = *(u8 *)((char *)slider + 0x1D4) - 1;
            update[3] = **(u32 **)((char *)slider + 0x1D0);
            send_message_guess(
                g_world_connection_guess, 0x2111, update, 0x10, 1);
        }
        *(int *)((char *)slider + 0x1DC) = 0;
    }
}

// FUNCTION: TRILOGY 0x004865f1
// Handles quantity selector clicks to move the window, adjust the amount, drag the thumb, or finish.
void __fastcall HandleSliderAction_guess(EQ_HSlider *slider, int unusedEdx, short gate, short action)
{
    class SliderCallbackVtable_guess
    {
    public:
        virtual void unused0_guess(void) {}
        virtual void unused1_guess(void) {}
        virtual void invoke_guess(void) {}
    };

    void *everquest;

    if (*(int *)(slider->unknown0x000 + 0x10) == 0)
        return;

    everquest = EQ_GLOBAL(void *, 0x006E4E44);
    if (((everquest == 0 || *(int *)((char *)everquest + 0x94) == 0) &&
         (g_pc_guess->inventory_items[0] == (EQ_Item *)0)) &&
        (*(char *)((char *)g_display_guess + 0x3d) == '\0' &&
         g_cdisplay_guess->dragArmed_guess == '\0'))
    {
        if (gate == 0 && *(int *)slider->unknown0x1C8 != 0)
        {
            if (action > 0 && action < 4)
                *(short *)(slider->unknown0x1C8 + 4) = action;
            if (action != 4)
                return;
            slider->dragging_guess = 1;
            return;
        }

        switch (action)
        {
        case 0:
            *(int *)((char *)slider + 0x1ec) ^= 1;
            if (*(int *)((char *)slider + 0x1ec) == 0)
                return;
            if (*(int *)((char *)EQ_GLOBAL(void *, 0x006E4E48) + 0x90) < slider->thumb_guess->top)
                return;
            *(int *)((char *)slider + 0x1ec) = 0;
            return;
        case 1:
            *(int *)((char *)everquest + 0xeb8) = 1;
            *(int *)((char *)slider + 0x1ec) = 0;
            break;
        case 2:
            EQ_METHOD(1, void, 0x004866E1, (EQ_HSlider *, short), slider)(-1);
            return;
        case 3:
            EQ_METHOD(1, void, 0x004866E1, (EQ_HSlider *, short), slider)(1);
            return;
        default:
            return;
        }
    }
    else
    {
        *(int *)((char *)slider + 0x1e8) = 1;
    }

    ((SliderCallbackVtable_guess *)slider)->invoke_guess();
    return;
}

// FUNCTION: TRILOGY 0x004866e1
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (address neighbour and EQ_HSlider field layout)
// The range is a signed 32-bit value through a pointer; zero divides by zero, as in the original.
void EQ_HSlider::AdjustSliderPosition_guess(short delta)
{
    value_guess += delta;
    if (value_guess < 0)
        value_guess = 0;
    if (value_guess > *range_guess)
        value_guess = *range_guess;
    thumb_guess->left = value_guess * SLIDER_TRACK_LENGTH_GUESS / *range_guess + track_guess.left;
    if (thumb_guess->left > track_guess.right - SLIDER_THUMB_WIDTH_GUESS)
        thumb_guess->left = track_guess.right - SLIDER_THUMB_WIDTH_GUESS;
    if (thumb_guess->left < track_guess.left)
        thumb_guess->left = track_guess.left;
    thumb_guess->right = thumb_guess->left + SLIDER_THUMB_WIDTH_GUESS;
    CopyRect(&thumb_copy_guess, thumb_guess);
}

// FUNCTION: TRILOGY 0x00486772
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
// The clamped x is kept as a short (the original truncates track.right - 10 to 16 bits).
void EQ_HSlider::MouseMove_guess(short x, short y)
{
    if (dragging_guess)
    {
        if (x > track_guess.right - SLIDER_THUMB_WIDTH_GUESS)
            x = (short)(track_guess.right - SLIDER_THUMB_WIDTH_GUESS);
        if (x < track_guess.left)
            x = (short)track_guess.left;
        thumb_guess->left = x;
        thumb_guess->right = x + SLIDER_THUMB_WIDTH_GUESS;
        CopyRect(&thumb_copy_guess, thumb_guess);
        value_guess = (thumb_guess->left - track_guess.left) * *range_guess / SLIDER_TRACK_LENGTH_GUESS;
    }
}

class EQ_HSlider2_guess : public EQ_Overlay
{
public:
    EQ_HSlider2_guess();
};

// FUNCTION: TRILOGY 0x004867fe
// Initializes the second quantity selection window's controls and shared slider artwork.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
EQ_HSlider2_guess::EQ_HSlider2_guess()
{
    EQ_HSlider2 *slider = (EQ_HSlider2 *)this;
    char *self = (char *)this;
    int button_id;
    int element;
    RECT *button_rect;
    typedef BOOL (WINAPI *SetRect_guess_t)(LPRECT, int, int, int, int);
    typedef BOOL (WINAPI *CopyRect_guess_t)(LPRECT, const RECT *);

    *(short*)(self + 0x1cc) = -1;
    *(void **)self = (void *)0x00535654;
    *(u32 *)(self + 0x10) = 0;
    *(u32 *)(self + 0x70) = 0;
    *(u32 *)(self + 0x1c4) = 0;
    *(u32 *)(self + 0x1f8) = 0;
    *(u32 *)(self + 0x1d8) = 0;
    *(u32 *)(self + 0x1dc) = 0;
    *(u16 *)(self + 0x1e0) = 0;
    *(u32 *)(self + 0x1c8) = 0;

    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        (&slider->backgroundRect_guess, 0xfc, 0x8a, 0x17c, 0xca);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        (&slider->finishRect_guess, 0x13e, 0x98, 0x16c, 0xa9);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        ((LPRECT)(self + 0xa0), 0x10a, 0xb3, 0x11b, 0xbd);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        ((LPRECT)(self + 0xb0), 0x15d, 0xb3, 0x16f, 0xbd);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        (&slider->thumb_copy_guess, 0x153, 0xb2, 0x15d, 0xbd);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        (&slider->region5Rect_guess, 0x113, 0x9b, 0x118, 0xaa);

    button_rect = (RECT *)(self + 0x90);
    button_id = 1;
    do
    {
        EQ_METHOD(6, void, 0x004503BE, (void *, int, RECT *, int, int, int, int), this)(button_id, button_rect, -1, -1, 0, 0);
        button_rect = (RECT *)((char *)button_rect + 0x10);
        button_id++;
    } while ((short)button_id < 6);

    EQ_METHOD(6, void, 0x004503BE, (void *, int, RECT *, int, int, int, int), this)(0, (RECT *)(self + 0x80), -1, -1, 0, 0);
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))
        (self + 0x34, (const char *)0x0053e950);
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))
        (self + 0x52, (const char *)0x0055ca88);

    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        ((LPRECT)(self + 0x14), 0, 0, 0xd8, 0x85);
    EQ_GLOBAL(CopyRect_guess_t, 0x00534240)
        ((LPRECT)(self + 0x24), (const RECT *)(self + 0x80));
    EQ_METHOD(2, void, 0x00450168, (void *, int, int), this)(1, 0);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
        ((LPRECT)(self + 0x14), 0, 0, 0x80, 0x40);

    element = *(int *)(self + 0x70);
    *(u32 *)(self + 0x1c0) = 0;
    for (; element != 0; element = *(int *)(element + 0x20))
    {
        if (*(short *)(element + 0x14) == 4)
            *(u32 *)(self + 0x1c0) = element + 4;
    }
}

class EQ_ActivateOverlayEHMarker_guess
{
public:
    ~EQ_ActivateOverlayEHMarker_guess() {}
};

class EQ_ActivateOverlay_guess
{
public:
    ~EQ_ActivateOverlay_guess();
};

// FUNCTION: TRILOGY 0x004869d2
// Cleans up a separate overlay window type; its specific in-game role is unclear.
/* fake match: an empty cleanup marker preserves the original EH state around DelButtons. */
EQ_ActivateOverlay_guess::~EQ_ActivateOverlay_guess()
{
    EQ_ActivateOverlay_guess * volatile unwind_this = this;
    *(volatile u32 *)this = 0x00535654;
    {
        EQ_ActivateOverlayEHMarker_guess eh_marker;
        void (__fastcall *delButtons)(void *) = EQ_THISCALL(void, 0x00450463, (void *));
        *(u32 *)((char *)this + 0x10) = 0;
        delButtons(this);
    }
    EQ_METHOD(0, void, 0x0045005D, (void *), this)();
}

// FUNCTION: TRILOGY 0x00486a0e
// Shows hover help for the second quantity selector's finish, adjustment, and dragging controls.
// @early-stop: raw bytes differ only by the EQ_THISCALL stand-in's dummy EDX (xor edx,edx before push 0xa); match.sh normalises it.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
void __fastcall EQ_HSlider2_AutoHelp_guess(void *self, int unused_edx)
{
    typedef BOOL (WINAPI *PtInRect_guess_t)(const RECT *, POINT);
    POINT point;
    BOOL inRect;
    int element;
    short selector = -1;
    char buffer[128];

    if ((EQ_GLOBAL(int, 0x006B6864) != 0) &&
        (pinstEQ_Main_guess->hoverTicks_guess >= 3))
    {
        point.x = (LONG)EQ_GLOBAL(short, 0x006B6414);
        buffer[0] = 0;
        point.y = (LONG)EQ_GLOBAL(short, 0x006B6416);
        if (*(int *)((char *)self + 0x10) != 0)
        {
            for (element = *(int *)((char *)self + 0x70); element != 0;
                element = *(int *)((char *)element + 0x20))
            {
                inRect = EQ_GLOBAL(PtInRect_guess_t, 0x0053423C)
                    ((RECT *)((char *)element + 4), point);
                if (inRect != 0)
                {
                    selector = *(short *)((char *)element + 0x14);
                    break;
                }
            }
            pinstEQ_Main_guess->hoverId_guess = selector;
            if (selector != -1)
            {
                switch (selector)
                {
                case 1:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CA94));
                    break;
                case 2:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CAB0));
                    break;
                case 3:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CAC4));
                    break;
                case 4:
                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(buffer, EQ_ARRAY(char, 0x0055CAD8));
                    break;
                default:
                    *(short*)((char *)EQ_GLOBAL(void *, 0x006E4E44) + 0xebc) = -1;
                    return;
                }
                EQ_METHOD(4, void, 0x0046D4DA, (void *, LONG, LONG, char *, int), EQ_GLOBAL(void *, 0x006E4E44))(point.x, point.y + 0x14, buffer, 10);
            }
        }
    }
}

// FUNCTION: TRILOGY 0x00486aff
// Displays the second quantity selector's selected amount, slider thumb, and pressed buttons.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
void __fastcall EQ_HSlider2_draw_guess(void *self, int unused_edx)
{
    typedef BOOL (WINAPI *SetRect_guess_t)(LPRECT, int, int, int, int);
    typedef BOOL (WINAPI *CopyRect_guess_t)(LPRECT, const RECT *);
    char text[80];
    RECT *rect;
    int old_left;
    int old_top;
    int x;
    int y;
    int dx;
    int dy;
    int element;
    short style;
    void *image_context;
    void *image;

    if (EQ_CDECL(int, 0x004504AC, (void))() == 0)
        return;

    if (*(int *)((char *)self + 0x1f8) != 0)
    {
        pinstEQ_Main_guess->uiDirty_guess = 1;
        old_left = *(int *)((char *)self + 0x24);
        old_top = *(int *)((char *)self + 0x28);
        rect = (RECT *)((char *)self + 0x24);
        rect->left = EQ_GLOBAL(int, 0x006EFC80);
        *(int *)((char *)self + 0x28) = EQ_GLOBAL(int, 0x006EFC84);
        if (rect->left > 0x200)
            rect->left = 0x200;
        if (rect->left < 0)
            rect->left = 0;
        if (*(int *)((char *)self + 0x28) > 0x1a0)
            *(int *)((char *)self + 0x28) = 0x1a0;
        if (*(int *)((char *)self + 0x28) < 0)
            *(int *)((char *)self + 0x28) = 0;

        x = rect->left;
        y = *(int *)((char *)self + 0x28);
        EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, x, y, x + 0x80, y + 0x40);
        dx = x - old_left;
        dy = y - old_top;
        for (element = *(int *)((char *)self + 0x70); element != 0; element = *(int *)(element + 0x20))
        {
        EQ_METHOD(2, void, 0x00419583, (void *, int, int), (void *)element)(dx, dy);
            EQ_GLOBAL(CopyRect_guess_t, 0x00534240)(
                (LPRECT)((int)self + ((int)*(short *)(element + 0x14) + 8) * 0x10),
                (const RECT *)(element + 4));
        }
    }

    rect = (RECT *)((char *)EQ_GLOBAL(void *, 0x006B341C) + 4);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x158, 0xf2, 0x1d8, 0x132);
    *(int *)EQ_GLOBAL(void *, 0x006B341C) =
        *(int *)((char *)*(void **)((char *)&EQ_GLOBAL(int, 0x006E4E44) - 8) + 4);
    EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B341C))((char *)self + 0x80, 1);

    style = *(short *)((char *)self + 0x1cc);
    if (style != -1)
    {
        switch (style)
        {
        case 1:
            image_context = EQ_GLOBAL(void *, 0x006B37C0);
            image = (char *)self + 0x90;
            break;
        case 2:
            image_context = EQ_GLOBAL(void *, 0x006B38A8);
            image = (char *)self + 0xa0;
            break;
        case 3:
            image_context = EQ_GLOBAL(void *, 0x006B38AC);
            image = (char *)self + 0xb0;
            break;
        default:
            goto draw_thumb;
        }
        EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), image_context)(image, 1);
    }

draw_thumb:
    EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B3640))((char *)self + 0xc0, 1);
    EQ_CDECL(int, 0x005224FB, (char *, const char *, int))(
        text, (const char *)0x0053e16c, (int)*(short *)((char *)self + 0x1ce));
    EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))(text,
        (int)*(short *)((char *)self + 0xd0),
        (int)*(short *)((char *)self + 0xd4), 7,
        EQ_GLOBAL(int, 0x006EFDFC), 1);
}

// FUNCTION: TRILOGY 0x00486cb6
// Opens the quantity slider for an item slot (stack split or merchant count); one-unit case moves it at once.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
// @early-stop: zero/item/CopyRect sit in other registers
/* as original: the gotos are the original's shared exits (LAB_004870af, LAB_00486db1). */
void __fastcall EQ_HSlider2_Activate_guess(EQ_HSlider2 *self, int unused_edx, void **item_ref, int item_slot)
{
    typedef BOOL (WINAPI *SetRect_guess_t)(LPRECT, int, int, int, int);
    typedef BOOL (WINAPI *CopyRect_guess_t)(LPRECT, const RECT *);
    typedef void (__fastcall *ActivateDefault_guess_t)(void *, int);
    typedef void (__fastcall *ActivateSlider_guess_t)(void *, int, int, int);
    void *item;
    void *new_item;
    void *element;
    CopyRect_guess_t copy_rect;
    int message_212c[3];
    int message_2112[7];
    int x;
    int y;
    int dx;
    int dy;
    unsigned char maximum;

    if (item_ref == 0)
        goto default_action;
    *(void **)((char *)self + 0x1f0) = item_ref;
    *(int *)((char *)self + 0x1e4) = item_slot;
    item = *item_ref;
    if (EQ_GLOBAL(void *, 0x006E4E44) != 0 && pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
        goto done;
    if (*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0) != 0)
        goto done;
    if (*(char *)((char *)g_display_guess + 0x3d) != 0)
        goto done;
    if (item == 0)
        goto done;
    if (g_cdisplay_guess->dragArmed_guess != 0)
        goto done;
    if (*(char *)((char *)item + 0x81) != 0)
        goto done;

    *(int *)((char *)self + 0x1d4) = 0;
    if (*(signed char *)((char *)item + 0xbb) < 1)
        goto default_action;
    if (EQ_CDECL(int, 0x004A1D83, (void *))(item) == 0)
        goto done;

    if (pinstEQ_Main_guess->merchantOpen_guess == 0 ||
        pinstEQ_Main_guess->selectedItemSlot_guess >= 0x1e)
    {
        self->max_guess = (unsigned char)(
            ((int)*(unsigned char *)((char *)item + 0xda) - 1) /
            (int)*(signed char *)((char *)item + 0xbb) + 1);
        *(unsigned char *)((char *)self + 0x1d1) =
            *(unsigned char *)((char *)item + 0xda);
    }
    else
    {
        self->max_guess = 0x14;
        *(unsigned char *)((char *)self + 0x1d1) = 1;
    }

    maximum = self->max_guess;
    if (maximum == 0)
        goto default_action;
    if (maximum == 1)
    {
        pinstEQ_Main_guess->uiDirty_guess = 1;
        new_item = new SliderEquipment_guess;
        EQ_CDECL(void *, 0x00521B10, (void *, const void *, unsigned int))(
            new_item, item, 0x124);
        *(void **)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0) = new_item;
        *(unsigned char *)((char *)g_display_guess + 0x3c) = 1;
        operator delete(item);

        message_212c[0] = *(int *)((char *)self + 0x1e4);
        message_212c[1] = 0;
        message_212c[2] = 1;
        EQ_CDECL(void, 0x004E1C45, (void **, unsigned int, void *, unsigned int, int))(
            g_world_connection_guess, 0x212c, message_212c, 0x0c, 1);

        if (pinstEQ_Main_guess->merchantOpen_guess == 0 ||
            *(void **)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0) == 0)
        {
            if (*(int *)((char *)self + 0x1d8) == 0 ||
                pinstEQ_Main_guess->tradeOpen_guess == 0 ||
                EQ_GLOBAL(int, 0x006E4D88) == 0)
            {
                if (*(int *)((char *)self + 0x1dc) != 0)
                {
                    message_2112[0] = *(int *)((char *)EQ_GLOBAL(void *, 0x006E4D8C) + 0x80);
                    message_2112[1] = *(short *)((char *)EQ_GLOBAL(void *, 0x006E4DCC) + 0x62);
                    *(short*)((char *)message_2112 + 0x12) = -1;
                    *(short *)((char *)message_2112 + 0x14) =
                        *(short *)((char *)self + 0x1e0);
                    message_2112[6] = 0;
                    EQ_CDECL(void, 0x004E1C45,
                        (void **, unsigned int, void *, unsigned int, int))(
                            g_world_connection_guess, 0x2112, message_2112, 0x1c, 1);
                }
            }
            else
            {
                pinstEQ_Main_guess->tradeAccepted_guess = 0;
                pinstEQ_Main_guess->tradeOtherAccepted_guess = 0;
            }
        }
        else
        {
            EQ_THISCALL(void, 0x00463B5D, (void *, int, void *, int))(
                EQ_GLOBAL(void *, 0x006E4E44), 0, (char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0, 0);
        }

        **(void ***)((char *)self + 0x1f0) = 0;
        if (*(int *)((char *)self + 0x1e4) > 0 && *(int *)((char *)self + 0x1e4) < 0x1e)
        {
            EQ_THISCALL(void, 0x0040B521, (void *, int, void *, int))(
                g_display_guess, 0,
                EQ_GLOBAL(void *, 0x006E4D8C), *(int *)((char *)self + 0x1e4));
        }
        goto done;
    }

    *(int *)((char *)self + 0x10) = 1;
    x = EQ_GLOBAL(int, 0x006EFC80);
    y = EQ_GLOBAL(int, 0x006EFC84);
    if (x > 0x200)
        x = 0x200;
    if (x < 0)
        x = 0;
    if (y > 0x1a0)
        y = 0x1a0;
    if (y < 0)
        y = 0;
    dx = x - *(int *)((char *)self + 0x24);
    dy = y - *(int *)((char *)self + 0x28);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(
        (LPRECT)((char *)self + 0x24), x, y, x + 0x80, y + 0x40);
    element = *(void **)((char *)self + 0x70);
    copy_rect = EQ_GLOBAL(CopyRect_guess_t, 0x00534240);
    for (; element != 0; element = *(void **)((char *)element + 0x20))
    {
        EQ_THISCALL(void, 0x00419583, (void *, int, int, int))(
            element, 0, dx, dy);
        copy_rect((LPRECT)((char *)self +
                ((int)*(short *)((char *)element + 0x14) + 8) * 0x10),
            (const RECT *)((char *)element + 4));
    }

    *(unsigned short *)((char *)self + 0x1f6) = 0;
    *(unsigned short *)((char *)self + 0x1f4) =
        *(unsigned char *)((char *)self + 0x1d1);
    x = *(int *)((char *)self + 0xc8);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(
        self->thumb_guess, x,
        (self->thumb_guess)->top, x + 10,
        (self->thumb_guess)->bottom);
    copy_rect((LPRECT)((char *)self + 0xc0), self->thumb_guess);
    *(unsigned short *)((char *)self + 0x1ce) =
        *(unsigned char *)((char *)self + 0x1d1);
    (self->thumb_guess)->left =
        (int)self->pos_guess * 0x38 /
        (int)self->max_guess +
        *(int *)((char *)self + 0xa8);
    if ((self->thumb_guess)->left > *(int *)((char *)self + 0xb0) - 10)
        (self->thumb_guess)->left = *(int *)((char *)self + 0xb0) - 10;
    if ((self->thumb_guess)->left < *(int *)((char *)self + 0xa8))
        (self->thumb_guess)->left = *(int *)((char *)self + 0xa8);
    (self->thumb_guess)->right =
        (self->thumb_guess)->left + 10;
    copy_rect((LPRECT)((char *)self + 0xc0), self->thumb_guess);

    if (EQ_GLOBAL(int, 0x006B70EC) != 0)
    {
        *(unsigned short *)((char *)self + 0x1f4) = 1;
        *(unsigned short *)((char *)self + 0x1f6) =
            self->max_guess - 1;
    }
    else if (EQ_GLOBAL(int, 0x006B70E8) == 0)
        goto done;
    else
    {
        *(unsigned short *)((char *)self + 0x1f4) =
            self->max_guess;
        *(unsigned short *)((char *)self + 0x1f6) = 0;
    }

    ((ActivateSlider_guess_t)(*(unsigned int *)(*(unsigned int *)self)))(self, 0, 0, 1);
    goto done;

default_action:
    ((ActivateDefault_guess_t)(*(unsigned int *)(*(unsigned int *)self + 8)))(self, 0);
done:
    return;
}

// FUNCTION: TRILOGY 0x004870ed
// Quantity slider input: steps the count down/up, or on accept buys, sells or splits off the chosen amount.
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
// Window virtual (callee pops 8): word flag + word code. Code 1 buys/sells/moves the stack held in
// the slider's item reference.
// @early-stop: zero and item sit in swapped registers (EBX/EDI), so compares and moves on them differ; the guard's ctor and dtor are out-of-line calls; the default call follows the move block, not the buy call.
/* fake match: the gotos to default_action and move_stack are the shape that matched best. */
void __fastcall EQ_HSlider2_Activate2_guess(EQ_HSlider2 *self, int unused_edx, short flag_guess, short code_guess)
{
    typedef void (__fastcall *ActivateDefault_guess_t)(void *, int);
    void *item;
    EQ_Main *world;
    void *ui_state;
    void **slot;
    int *toggle;
    short code;
    int message_212c[3];
    int message_2112[7];

    item = **(void ***)((char *)self + 0x1f0);
    if (*(int *)((char *)self + 0x10) == 0)
        return;
    world = (EQ_Main *)EQ_GLOBAL(void *, 0x006E4E44);
    if (world != 0 && world->pendingWorldRequests_guess != 0)
        goto default_action;
    if (*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0) != 0)
        goto default_action;
    ui_state = g_display_guess;
    if (*(char *)((char *)ui_state + 0x3d) != '\0')
        goto default_action;
    if (*(char *)((char *)ui_state + 0x3e) != '\0')
        goto default_action;

    if (flag_guess == 0 && *(int *)((char *)self + 0x1c8) != 0)
    {
        code = code_guess;
        if (code >= 1 && code <= 3)
            *(short *)((char *)self + 0x1cc) = code;
        if (code == 4)
            self->dragging_guess = 1;
        return;
    }

    switch (code_guess)
    {
    case 0:
        toggle = (int *)((char *)self + 0x1f8);
        *toggle ^= 1;
        if (*toggle != 0 &&
            *(int *)(*(char **)0x006e4e48 + 0x90) >=
                (self->thumb_guess)->top)
            *toggle = 0;
        return;
    case 1:
        world->uiDirty_guess = 1;
        *(int *)((char *)self + 0x1f8) = 0;
        world = (EQ_Main *)EQ_GLOBAL(void *, 0x006E4E44);
        if (world->merchantOpen_guess != 0 &&
            world->selectedItemSlot_guess < 0x1e)
            EQ_METHOD(1, void, 0x00463A0E, (void *, int), world)((int)self->left_value_guess);
        else
            goto move_stack;
        break;
    case 3:
        EQ_METHOD(1, void, 0x0048745D, (void *, short), self)(1);
        return;
    case 2:
        EQ_METHOD(1, void, 0x0048745D, (void *, short), self)(-1);
        return;
    default:
        return;
    }

default_action:
    ((ActivateDefault_guess_t)(*(unsigned int *)(*(unsigned int *)self + 8)))(self, 0);
    return;

move_stack:
    if (self->left_value_guess > 0)
    {
        void *new_item = 0;

        new_item = new SliderEquipment_guess;
        EQ_CDECL(void *, 0x00521B10, (void *, const void *, unsigned int))(
            new_item, item, 0x124);
        *(void **)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0) = new_item;
        *(unsigned char *)((char *)new_item + 0xda) =
            (char)*(char *)((char *)item + 0xbb) * (char)self->left_value_guess;
        if (*(unsigned char *)((char *)new_item + 0xda) > *(unsigned char *)((char *)self + 0x1d1))
            *(unsigned char *)((char *)new_item + 0xda) = *(unsigned char *)((char *)self + 0x1d1);
        g_cdisplay_guess->itemOnCursor_guess = 1;
    }
    if (self->right_value_guess > 0)
    {
        if (self->left_value_guess != 0)
            *(unsigned char *)((char *)item + 0xda) = *(unsigned char *)((char *)self + 0x1d1) -
                *(unsigned char *)((char *)*(void **)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0) + 0xda);
        else
            *(unsigned char *)((char *)item + 0xda) = *(unsigned char *)((char *)self + 0x1d1);
    }
    else
    {
        operator delete(item);
        item = 0;
    }
    if (self->left_value_guess > 0)
    {
        message_212c[2] = (int)self->left_value_guess;
        message_212c[0] = *(int *)((char *)self + 0x1e4);
        message_212c[1] = 0;
        EQ_CDECL(void, 0x004E1C45, (void **, unsigned int, void *, unsigned int, int))(
            (void **)0x006b3194, 0x212c, message_212c, 0x0c, 1);
    }

    world = (EQ_Main *)EQ_GLOBAL(void *, 0x006E4E44);
    slot = (void **)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xe0);
    if (world->merchantOpen_guess != 0 && *slot != 0)
    {
        EQ_METHOD(2, void, 0x00463B5D, (void *, void *, int), world)(slot, 0);
    }
    else if (*(int *)((char *)self + 0x1d8) != 0 &&
        world->tradeOpen_guess != 0 &&
        EQ_GLOBAL(int, 0x006E4D88) != 0)
    {
        world->tradeAccepted_guess = 0;
        pinstEQ_Main_guess->tradeOtherAccepted_guess = 0;
    }
    else if (*(int *)((char *)self + 0x1dc) != 0)
    {
        message_2112[0] = *(int *)((char *)EQ_GLOBAL(void *, 0x006E4D8C) + 0x80);
        message_2112[1] = *(short *)((char *)EQ_GLOBAL(void *, 0x006E4DCC) + 0x62);
        *(short *)((char *)message_2112 + 0x14) = *(short *)((char *)self + 0x1e0);
        message_2112[6] = 0;
        if (item == 0)
            *(short*)((char *)message_2112 + 0x12) = -1;
        else if (EQ_CDECL(int, 0x004A0A4C, (void *, int))(item, 1) != 0)
        {
            *(short *)((char *)message_2112 + 0x12) = *(short *)((char *)item + 0x82);
            *(unsigned short *)((char *)message_2112 + 0x16) = *(unsigned char *)((char *)item + 0x81);
            message_2112[6] = *(int *)((char *)item + 0x8c);
            if (*(short *)((char *)message_2112 + 0x16) == 0)
                EQ_CDECL(void *, 0x00521B10, (void *, const void *, unsigned int))(
                    (char *)message_2112 + 8, (char *)item + 0xd8, 10);
        }
        EQ_CDECL(void, 0x004E1C45, (void **, unsigned int, void *, unsigned int, int))(
            (void **)0x006b3194, 0x2112, message_2112, 0x1c, 1);
    }

    **(void ***)((char *)self + 0x1f0) = item;
    if (*(int *)((char *)self + 0x1e4) > 0 && *(int *)((char *)self + 0x1e4) < 0x1e)
        EQ_METHOD(2, void, 0x0040B521, (void *, void *, short), g_display_guess)(
            EQ_GLOBAL(void *, 0x006E4D8C), (short)*(int *)((char *)self + 0x1e4));
    goto default_action;
}

// FUNCTION: TRILOGY 0x0048745d
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (address range of the Mac-mapped EQ_HSlider2 functions; TAKP name)
// max_guess == 0 divides by zero, as in the original.
void EQ_HSlider2::AdjustSliderPosition_guess(short delta)
{
    pos_guess += delta;
    if (pos_guess < 0)
        pos_guess = 0;
    if (pos_guess > max_guess)
        pos_guess = max_guess;
    thumb_guess->left = pos_guess * SLIDER_TRACK_LENGTH_GUESS / max_guess + track_guess.left;
    if (thumb_guess->left > track_guess.right - SLIDER_THUMB_WIDTH_GUESS)
        thumb_guess->left = track_guess.right - SLIDER_THUMB_WIDTH_GUESS;
    if (thumb_guess->left < track_guess.left)
        thumb_guess->left = track_guess.left;
    thumb_guess->right = thumb_guess->left + SLIDER_THUMB_WIDTH_GUESS;
    CopyRect(&thumb_copy_guess, thumb_guess);
    left_value_guess = pos_guess;
    right_value_guess = max_guess - pos_guess;
}

// FUNCTION: TRILOGY 0x00487523
// ORIGINAL FILE: EverQuest/EQ_Slider.cpp (from Mac debug map)
void EQ_HSlider2::MouseMove_guess(short x, short y)
{
    if (dragging_guess)
    {
        if (x > track_guess.right - SLIDER_THUMB_WIDTH_GUESS)
            x = (short)(track_guess.right - SLIDER_THUMB_WIDTH_GUESS);
        if (x < track_guess.left)
            x = (short)track_guess.left;
        thumb_guess->left = x;
        thumb_guess->right = x + SLIDER_THUMB_WIDTH_GUESS;
        CopyRect(&thumb_copy_guess, thumb_guess);
        pos_guess = (short)((thumb_guess->left - track_guess.left) * max_guess / SLIDER_TRACK_LENGTH_GUESS);
        if (pos_guess < 0)
            pos_guess = 0;
        if (pos_guess > max_guess)
            pos_guess = max_guess;
        left_value_guess = pos_guess;
        right_value_guess = max_guess - pos_guess;
    }
}
