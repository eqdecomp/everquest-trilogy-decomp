#include "trilogy/common.h"
#include "trilogy/spell.h"
#include "trilogy/eq_spell.h"
#include "trilogy/eqaddr.h"
#include "trilogy/display.h"

#include <stdio.h>

// FUNCTION: TRILOGY 0x00422acf
// Matches fixed pairs of spell IDs for special pair handling.
int __cdecl SpellPairOverridesStacking_guess(short spellId1, short spellId2)
{
    if ((spellId1 == 0x598 && spellId2 == 0x6ae) ||
        (spellId2 == 0x598 && spellId1 == 0x6ae) ||
        (spellId1 == 0x598 && spellId2 == 0x63e) ||
        (spellId2 == 0x598 && spellId1 == 0x63e) ||
        (spellId1 == 0x598 && spellId2 == 0x63f))
        return 1;
    if (spellId2 == 0x598)
    {
        if (spellId1 == 0x63f)
            return 1;
    }
    else if (spellId2 == 0x5a5)
    {
        if (spellId1 == 0xd2 || spellId1 == 0x2b || spellId1 == 0x2c || spellId1 == 0x5fe)
            return 1;
    }
    else
    {
        if ((spellId2 == 0xd2 || spellId2 == 0x2b || spellId2 == 0x2c || spellId2 == 0x5fe) &&
            spellId1 == 0x5a5)
            return 1;
    }
    if (spellId2 == 0x598)
    {
        if (spellId1 == 0x561)
            return 1;
    }
    else if (spellId2 == 0x561 && spellId1 == 0x598)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x00422b8e
// Applies special rules to two spell IDs using pair exceptions and spell-record data.
int __cdecl CheckSpellPair_guess(short spellId1, short spellId2)
{
    int overrides;
    u8* spell;

    if (((spellId1 == 0x158 || spellId1 == 0x163 || spellId1 == 0x1C4 || spellId1 == 0x1C5) &&
         (spellId2 == 0x158 || spellId2 == 0x163 || spellId2 == 0x1C4 || spellId2 == 0x1C5)) ||
        ((spellId1 == 0x168 || spellId1 == 0x1C3 || spellId1 == 6) &&
         (spellId2 == 0x168 || spellId2 == 0x1C3 || spellId2 == 6)) ||
        ((spellId1 == 0x108 || spellId1 == 99 || spellId1 == 0x103 || spellId1 == 0x299) &&
         (spellId2 == 0x108 || spellId2 == 99 || spellId2 == 0x103 || spellId2 == 0x299)))
        return 1;

    overrides = SpellPairOverridesStacking_guess(spellId1, spellId2);
    if (overrides != 0)
        return 0;

    if (spellId1 == 0x598)
    {
        if (spellId2 == 0x598)
            return 0;
        spell = (u8*)EQ_ARRAY(EQ_Spell_guess *, 0x005FE9A0)[spellId2];
        if (spell[0x23C] > 0x3C && spell[0x21F] == 1)
        {
            if (spell[0x222] == 4)
                return 1;
            if (spell[0x222] == 5)
                return 1;
        }
    }
    else
    {
        if (spellId1 == 0x5A7 && spellId2 == 0x5A7)
            return 0;
        if (spellId2 == 0x598)
        {
            spell = (u8*)EQ_ARRAY(EQ_Spell_guess *, 0x005FE9A0)[spellId1];
            if (spell[0x23C] > 0x3C && spell[0x21F] == 1)
            {
                if (spell[0x222] == 4)
                    return 1;
                if (spell[0x222] == 5)
                    return 1;
            }
        }
    }

    if (spellId2 == 0x5A7)
    {
        spell = (u8*)EQ_ARRAY(EQ_Spell_guess *, 0x005FE9A0)[spellId1];
        if (spell[0x23C] > 0x3C && spell[0x21F] == 1)
        {
            if (spell[0x222] == 1)
                return 1;
            if (spell[0x223] == 'E')
                return 1;
            if (spell[0x224] == 'E')
                return 1;
            if (spell[0x225] == 1)
                return 1;
        }
    }

    if (spellId1 == 0x5A7)
    {
        spell = (u8*)EQ_ARRAY(EQ_Spell_guess *, 0x005FE9A0)[spellId2];
        if (spell[0x23C] > 0x3C && spell[0x21F] == 1)
        {
            if (spell[0x222] == 1)
                return 1;
            if (spell[0x223] == 'E')
                return 1;
            if (spell[0x224] == 'E')
                return 1;
            if (spell[0x225] == 1)
                return 1;
        }
    }
    return 0;
}

class EQ_Character_SpellStacking_guess
{
public:
    int FindAffectSlot_guess(int param_2, void* param_3, int* param_4, int param_5);
};

// FUNCTION: TRILOGY 0x00422dd6
// Buff stacking: picks the slot a landing spell takes (same spell, a weaker buff it overrides, or a free slot)
// @early-stop: frame remains 0x18 versus 0x14; register allocation and block layout still differ.
/* fake match: the packed byte-lane counter and shared goto exits mirror the target's frame and block layout. */
int EQ_Character_SpellStacking_guess::FindAffectSlot_guess(int param_2, void* param_3, int* outSlot, int checkOnly)
{
    short buffSpellId;
    char ch;
    int tmp;
    int value;
    short* buffSpellIdPtr;
    short slot;
    int temp;
    bool bVar15;
    u8* self;
    *outSlot = -1;
    if (param_3 == 0)
        return 0;
    if ((short)param_2 < 0 || (short)param_2 > 2999)
        return 0;
    temp = (int)EQ_ARRAY(EQ_Spell_guess*, 0x005FE9A0)[(short)param_2];
    if (temp == 0)
        return 0;
    self = (u8*)this;
    tmp = EQ_THISCALL(int, 0x0041F0C0, (void*, int, void*, int))
        (this, 0, (void*)temp, *(u8*)((char*)param_3 + 0x99));
    if (tmp != 0)
    {
            slot = 0;
            do
            {
                if (((((*(char*)(self + 0x284 + slot * 10) != '\0') &&
                      (buffSpellId = *(short*)(self + slot * 10 + 0x288), 0 <= buffSpellId)) &&
                     ((buffSpellId < 3000 &&
                      ((tmp = (int)EQ_ARRAY(EQ_Spell_guess*, 0x005FE9A0)[buffSpellId]) != 0 &&
                       (0x3c < *(u8*)(tmp + 0x23c)))))) &&
                    (*(char*)(tmp + 0x21f) == '\0')) &&
                   (((*(char*)(temp + 0x21f) != '\0') &&
                     ((ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                         ((void*)temp, 3), ch != '\0') &&
                      ((ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                          ((void*)tmp, 3), ch != '\0') ||
                       (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                           ((void*)tmp, 99), ch != '\0'))))))
                    {
                        *outSlot = -1;
                        return 0;
                    }
                slot = slot + 1;
            }
            while (slot < 0xf);
    }
    slot = 0;
    tmp = temp;
        do
        {
            if (*(char*)(self + 0x284 + slot * 10) != '\0')
            {
                buffSpellId = *(short*)(self + slot * 10 + 0x288);
                tmp = (tmp & 0xffff0000) | (u16)buffSpellId;
                value = EQ_CDECL(int, 0x00422B8E, (int, int))(tmp, param_2);
                if (value != 0)
                {
                    temp = slot * 10;
                    if ((((*(u8*)(self + temp + 0x285) <= *(u8*)((char*)param_3 + 0x9d)) &&
                          (buffSpellId = *(short*)(self + temp + 0x288), buffSpellId <= (short)param_2)) &&
                        ((short)param_2 != 0x5a7)) &&
                       (((buffSpellId != 0x5a7) && ((short)param_2 != 0x598)) && (buffSpellId != 0x598)))
                    {
                        *outSlot = (int)slot;
                        if (checkOnly == 0)
                        {
                            bVar15 = (short)param_2 == *(short*)(self + temp + 0x288);
                            if (!bVar15)
                            {
                                EQ_THISCALL(void, 0x00424E30, (void*, int, void*, int))
                                    (this, 0, (void*)(self + temp + 0x284), 0);
                            }
                        }
                        temp = *outSlot;
                        goto returnBuff;
                    }
                    {
                        *outSlot = -1;
                        return 0;
                    }
                }
                if (buffSpellId == (short)param_2)
                {
                    if (((*(u8*)(self + slot * 10 + 0x285) <= *(u8*)((char*)param_3 + 0x9d)) &&
                         (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                             ((void*)temp, 0x43), ch == '\0')) &&
                        (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                            ((void*)temp, 0x65), ch == '\0'))
                    {
                        *outSlot = (int)slot;
                        return (int)(self + slot * 10 + 0x284);
                    }
                    {
                        *outSlot = -1;
                        return 0;
                    }
                }
            }
            slot = slot + 1;
        }
        while (slot < 0xf);
        {
        u8 byteTmp;
        u8 newEffect;
        short existingValue;
        short newValue;
        u32 buffSpellIdArg;
        u8* buffSpell;
        u32 effectIndex;
        u32 effectPos;
        slot = 0;
        do
        {
            value = (int)slot;
            tmp = (int)(self + value * 10);
            if (*(char*)(self + 0x284 + value * 10) == '\0')
            {
emptySlot:
                if (*outSlot == -1)
                {
takeSlot:
                    *outSlot = (int)slot;
                }
            }
            else
            {
                buffSpellId = *(short*)(tmp + 0x288);
                buffSpellIdArg = ((u32)(u16)((u32)(value * 5) >> 16) << 16) | (u16)buffSpellId;
                if ((buffSpellId < 0) || (2999 < buffSpellId))
                    buffSpell = 0;
                else
                    buffSpell = (u8*)EQ_ARRAY(EQ_Spell_guess*, 0x005FE9A0)[buffSpellId];
                if (buffSpell == 0)
                {
                    *(u8*)(tmp + 0x284) = 0;
                    *(short*)(tmp + 0x288) = -1;
                    *(u8*)(tmp + 0x285) = 0;
                    *(u16*)(self + (value * 5 + 0x145) * 2) = 0;
                    *(u8*)(tmp + 0x286) = 0;
                    *(u16*)(tmp + 0x28c) = 0;
                    goto emptySlot;
                }
                if ((*(char*)((char*)param_3 + 0x99) == '\b') &&
                    (0x3c < *(u8*)(buffSpell + 0x23c)))
                {
                    if ((((*(char*)(temp + 0x21f) != '\0') &&
                          (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                              ((void*)temp, 3), ch != '\0')) &&
                         (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                             ((void*)buffSpell, 3), ch != '\0')) ||
                        (((*(char*)(temp + 0x21f) != '\0') &&
                          (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                              ((void*)temp, 3), ch != '\0')) &&
                         (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                             ((void*)buffSpell, 99), ch != '\0')))
                        {
                            *outSlot = -1;
                            return 0;
                        }
                }
                else
                {
                    if ((((*(char*)(temp + 0x21f) != '\0') &&
                          (((*(char*)(buffSpell + 0x21f) != '\0') &&
                            (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                                ((void*)temp, 3), ch != '\0')) &&
                           (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                               ((void*)buffSpell, 3), ch != '\0')))) &&
                        (((*(u8*)(buffSpell + 0x23c) != 0) &&
                          (*(u8*)(buffSpell + 0x23c) < 0x3d)) &&
                         (0x3c < *(u8*)(temp + 0x23c))))
                        {
                            *outSlot = -1;
                            return 0;
                        }
                    effectIndex = 0;
                    *(u8*)&effectIndex = 0;
                    do
                    {
                        effectPos = effectIndex & 0xff;
                        ch = *(char*)(effectPos + 0x222 + temp);
                        if ((ch == ',') || (ch == '-'))
                            {
                                *outSlot = -1;
                                return 0;
                            }
                        value = EQ_CDECL(int, 0x00422ACF, (int, int))(buffSpellIdArg, param_2);
                        if (((value == 0) &&
                             (((((byteTmp = *(u8*)(effectPos + 0x222 + buffSpell),
                                  byteTmp != 0xf && (byteTmp != 0)) && (byteTmp != 100)) ||
                                ((*(u8*)(buffSpell + 0x23c) == 0) ||
                                 (0x3c < *(u8*)(buffSpell + 0x23c)))) ||
                               (*(u8*)(temp + 0x23c) < 0x3d))) &&
                            (newEffect = *(u8*)(effectPos + 0x222 + temp), byteTmp == newEffect)))
                        {
                            if (newEffect < 0x25)
                            {
                                if (newEffect < 0x23)
                                {
                                    if (newEffect < 2)
                                    {
                                        if (-1 < *(short*)(temp + 0x1ca + effectPos * 2))
                                            goto compareEffect;
                                    }
                                    else
                                    {
                                        if (newEffect == 10)
                                        {
                                            value = effectPos * 2 + 0x1ca;
                                            if (*(short*)(value + temp) == 0)
                                                goto nextEffect;
                                            bVar15 = *(short*)(value + buffSpell) == 0;
                                        }
                                        else
                                        {
                                            bVar15 = newEffect == 0x18;
                                        }
                                        if (!bVar15)
                                        {
compareEffect:
                                            if (((((*(char*)(temp + 0x21f) != '\0') &&
                                                  ((*(char*)(buffSpell + 0x21f) == '\0') ||
                                                   (ch = EQ_CDECL(char, 0x0048A11F, (void*, int))
                                                       ((void*)buffSpell, 0x3a), ch != '\0')))) ||
                                                (*(char*)(effectPos + 0x222 + buffSpell) == 'e')) ||
                                                (((0x306 < buffSpellId && (buffSpellId < 0x312)) ||
                                                  (buffSpellId == 0x2f4)) ||
                                                 (((buffSpellId == 0x2f5) ||
                                                   ((0x4af < buffSpellId && (buffSpellId < 0x4e3)))) ||
                                                  (((0x76b < buffSpellId && (buffSpellId < 0x785)) ||
                                                    (buffSpellId == 0x344))))))
                                                {
                                                    *outSlot = -1;
                                                    return 0;
                                                }
                                            existingValue = EQ_THISCALL(short, 0x0042351A,
                                                (void*, int, void*, int, int, int))
                                                (this, 0, (void*)buffSpell,
                                                 (int)((buffSpellIdArg & 0xffffff00) |
                                                     *(u8*)(tmp + 0x285)), effectIndex, 0);
                                            newValue = EQ_THISCALL(short, 0x0042351A,
                                                (void*, int, void*, int, int, int))
                                                (this, 0, (void*)temp,
                                                 (int)(((u32)param_3 & 0xffffff00) |
                                                     *(u8*)((char*)param_3 + 0x9d)),
                                                 effectIndex, 0);
                                            if ((((short)param_2 == 0x654) ||
                                                 ((short)param_2 == 0x718)) ||
                                                (((short)param_2 == 0x341) || (buffSpellId == 0x716)))
                                                newValue = -1;
                                            if ((((buffSpellId == 0x654) || (buffSpellId == 0x718)) ||
                                                 (buffSpellId == 0x341)) || (buffSpellId == 0x716))
                                                existingValue = -1;
                                            if (((existingValue < 0) && (0 < newValue)) ||
                                                ((0 < existingValue) && (newValue < 0)))
                                            {
                                                ch = *(char*)(effectPos + 0x222 + temp);
                                                if (ch == '\x03')
                                                {
                                                    if (-1 < newValue)
                                                        {
                                                            *outSlot = -1;
                                                            return 0;
                                                        }
                                                }
                                                else if (((ch == '\0') && (existingValue < 0)) &&
                                                         (0 < newValue))
                                                    {
                                                        *outSlot = -1;
                                                        return 0;
                                                    }
                                                *outSlot = (int)slot;
                                                if (checkOnly != 0)
                                                    goto returnFound;
                                                temp = slot * 10;
                                                bVar15 = (short)param_2 ==
                                                    *(short*)(self + temp + 0x288);
                                                goto removeOld;
                                            }
                                            bVar15 = (short)param_2 == 0x344;
                                            if (*(char*)(effectPos + 0x222 + temp) == '\v')
                                            {
                                                if ((99 < newValue) || (existingValue <= newValue))
                                                {
                                                    if (existingValue < 0x65)
                                                        goto checkOverwrite;
                                                    if (99 < newValue)
                                                        goto comparePositive;
                                                }
overwriteOk:
                                                bVar15 = 1;
                                            }
                                            else
                                            {
                                                if ((existingValue < 0) && (newValue < existingValue))
                                                    goto overwriteOk;
                                                if (0 < existingValue)
                                                {
comparePositive:
                                                    if (existingValue < newValue)
                                                        goto overwriteOk;
                                                }
                                            }
checkOverwrite:
                                            if (!bVar15)
                                                {
                                                    *outSlot = -1;
                                                    return 0;
                                                }
                                            if ((*(char*)(temp + 0x21f) == '\0') &&
                                                (*(char*)(buffSpell + 0x21f) == '\0'))
                                            {
                                                *outSlot = (int)slot;
                                                if (checkOnly != 0)
                                                    goto returnFound;
                                                temp = slot * 10;
                                                bVar15 = (short)param_2 ==
                                                      *(short*)(self + temp + 0x288);
                                                goto removeOld;
                                            }
                                            if ((*outSlot == -1) ||
                                                  (*(char*)(self + 0x284 + *outSlot * 10) == '\0'))
                                                goto takeSlot;
                                            break;
                                        }
                                    }
                                }
                            }
                            else if (newEffect != 0x4f)
                            {
                                bVar15 = newEffect == 0xfe;
                                if (!bVar15)
                                {
                                    goto compareEffect;
                                }
                            }
                        }
nextEffect:
                        byteTmp = (char)effectIndex + 1;
                        *(u8*)&effectIndex = byteTmp;
                    }
                    while (byteTmp < 0xc);
                }
            }
            slot = slot + 1;
        }
        while (slot < 0xf);
        }
        if (*outSlot != -1)
        {
            if (checkOnly == 0)
            {
                temp = (int)(self + *outSlot * 10);
                if ((*(char*)(temp + 0x284) != '\0') &&
                    ((short)param_2 != *(short*)(temp + 0x288)))
                {
                    EQ_THISCALL(void, 0x00424E30, (void*, int, void*, int))
                            (this, 0, (void*)(temp + 0x284), 0);
                }
            }
            temp = *outSlot;
            goto returnBuff;
        }
        if (((*(char*)(temp + 0x21f) == '\0') && (*(int*)(self + 0xa0) != 0)) &&
            (*(char*)(*(int*)(self + 0xa0) + 0xf1) == '\0'))
        {
            temp = 0;
            buffSpellIdPtr = (short*)(self + 0x288);
            do
            {
                slot = *buffSpellIdPtr;
                if ((slot < 0) || (2999 < slot))
                    tmp = 0;
                else
                    tmp = (int)EQ_ARRAY(EQ_Spell_guess*, 0x005FE9A0)[slot];
                if ((tmp != 0) && (*(char*)(tmp + 0x21f) != '\0'))
                {
                    if (checkOnly == 0)
                    {
                        EQ_THISCALL(void, 0x00424E30, (void*, int, void*, int))
                            (this, 0, (void*)(self + 0x284 + temp * 10), 0);
                    }
                    *outSlot = temp;
                    goto returnBuff;
                }
                temp = temp + 1;
                buffSpellIdPtr = buffSpellIdPtr + 5;
            }
            while (temp < 0xf);
        }
    return 0;

removeOld:
    if (!bVar15)
    {
        EQ_THISCALL(void, 0x00424E30, (void*, int, void*, int))
            (this, 0, (void*)(self + temp + 0x284), 0);
    }
returnFound:
    temp = *outSlot;
    goto returnBuff;

returnBuff:
    return (int)(self + 0x284 + temp * 10);
}

// FUNCTION: TRILOGY 0x004875f6
// Reports whether the spell type is 0x3D through 0x40; the meaning of those values is unclear.
// ORIGINAL FILE: EverQuest/EQ_Spell.cpp (from Mac debug map)
int EQ_Spell_guess::SpellUsesDragonBreathEffect()
{
    s32 spellType = this->effectIndex_guess;
    if (spellType >= 0x3D)
    {
        if (spellType <= 0x40)
            return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0048760d
// Loads one spell record from Spdat.eff and sets its effect-data pointer.
int EQ_Spell_guess::LoadSpellData_guess(short spellIndex)
{
    FILE* file;
    int readCount;
    s32 effectCount;

    file = fopen((const char*)0x0055CB0C, (const char*)0x0053C2BC);
    if (file == 0)
        return 0;

    if (spellIndex != 0 && fseek(file, (long)spellIndex * 0x260L, SEEK_SET) != 0)
    {
        fclose(file);
        return 0;
    }

    readCount = fread(this, 0x260, 1, file);
    if (readCount != 1)
    {
        fclose(file);
        return 0;
    }

    fclose(file);
    effectCount = this->effectIndex_guess;
    if (effectCount < 0 || effectCount >= 0x100)
        *(u32*)((char*)this + 0x258) = 0;
    else
        *(u32*)((char*)this + 0x258) = 0x00604990u + (u32)effectCount * 0xAE8u;
    return 1;
}

#define SPELL_U8(off)   (*(u8*)(self + (off)))
#define SPELL_U16(off)  (*(u16*)(self + (off)))
#define SPELL_S32(off)  (*(s32*)(self + (off)))
#define SPELL_U32(off)  (*(u32*)(self + (off)))
#define SPELL_F32(off)  (*(float*)(self + (off)))
#define SPELL_STR(va)   ((const char*)(va))
#define SOC_CPY    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
#define SOC_FMT   EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))

// FUNCTION: TRILOGY 0x004876a8
// __fastcall (ECX = self spell record, spellId on the stack); returns self.
// Clears the once flag at 0x00569b58 (calling 0x0048a13e), then tries 0x0048760d to load the record by id.
// If that fails, sets common defaults and switches on the id: about 30 built-in records fill the names at +0 and
// +0x20, text blocks at +0x60..+0x160, class level bytes +0x235..+0x242 and the numeric fields.
// The done label clamps the value at +0x1c4 to 14000 unless the byte at +0x222 is 'L'.
// ORIGINAL FILE: EverQuest/EQ_Spell.cpp (from Mac debug map)
void* __fastcall EQ_Spell_EQ_Spell_guess(char* self, int edxDummy, int spellId)
{
    if (EQ_GLOBAL(int, 0x00569B58) != 0)
    {
        EQ_GLOBAL(int, 0x00569B58) = 0;
        EQ_CDECL(int, 0x0048A13E, (void))();
    }
    if (EQ_METHOD(1, int, 0x0048760D, (void*, int), self)(spellId) != 0)
        goto done;

    SPELL_U8(0x21F) = 1;
    SPELL_U16(0x1FE) = 0xFFFF;
    SPELL_U16(0x200) = 0xFFFF;
    SPELL_F32(0x1A4) = 0.0f;
    SPELL_U16(0x202) = 0xFFFF;
    SPELL_U16(0x204) = 0xFFFF;
    SPELL_U16(0x20E) = 0xFFFF;
    SPELL_U16(0x210) = 0xFFFF;
    SPELL_U16(0x1FC) = 500;
    SPELL_U8(0x220) = 0;
    SPELL_U32(0x1B0) = 1000;
    SPELL_U8(0x254) = 0x2B;
    SPELL_U32(0x258) = 0;
    SPELL_S32(0x25C) = -1;
    SPELL_U8(0x21E) = 0;
    SPELL_U32(0x1C4) = 0;

    switch ((short)spellId)
    {
    case 0x10:
        SOC_CPY(self, SPELL_STR(0x0055D5C8));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x212) = 0xFF;
        SPELL_U16(0x1FA) = 0x21F;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 0x1A;
        SPELL_U8(0x221) = 1;
        SPELL_U16(0x1CA) = 0xFFEC;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 0x14;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D57C));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D560));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    case 0xE:
        SOC_CPY(self, SPELL_STR(0x0055D558));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x21F;
        SPELL_U32(0x1B8) = 0;
        SPELL_S32(0x25C) = 1;
        SPELL_U32(0x258) = 0x00605478;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 6;
        SPELL_U8(0x221) = 1;
        SPELL_U16(0x1CA) = 0xFFF6;
        SPELL_U8(0x212) = 0x66;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 0xD;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 2;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D540));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D530));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    case 0xD:
        SOC_CPY(self, SPELL_STR(0x0055D51C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0055D510));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x5DC;
        SPELL_U16(0x1FC) = 0x3ED;
        SPELL_U32(0x1B8) = 2;
        SPELL_U32(0x258) = 0x00605478;
        SPELL_U32(0x1BC) = 0;
        SPELL_S32(0x25C) = 1;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 0xC;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 5000;
        SPELL_U8(0x212) = 100;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 0x1E;
        SOC_FMT(self + 0x60, SPELL_STR(0x0058FBA4));
        SOC_FMT(self + 0xA0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D4F4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D4DC));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 0xC:
        SOC_CPY(self, SPELL_STR(0x0055D4D4));
        SOC_CPY(self + 0x20, SPELL_STR(0x0055D510));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x258) = 0x00605478;
        SPELL_S32(0x25C) = 1;
        SPELL_U16(0x1FA) = 0x5DC;
        SPELL_U16(0x1FC) = 0x3ED;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 8;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 10;
        SPELL_U8(0x212) = 5;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 0xF;
        SPELL_U8(0x237) = 0x19;
        SPELL_U8(0x23E) = 0xF;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D4BC));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D4A8));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 0xB:
        SOC_CPY(self, SPELL_STR(0x0055D49C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U32(0x258) = 0x00605F60;
        SPELL_U16(0x1FA) = 0x5DD;
        SPELL_U16(0x1FC) = 0x3EE;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_S32(0x25C) = 2;
        SPELL_U32(0x1BC) = 2;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_U32(0x1B8) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0x14;
        SPELL_U8(0x212) = 100;
        SPELL_U8(0x222) = 1;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 2;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D488));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D478));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D460));
        break;

    case 7:
        SOC_CPY(self, SPELL_STR(0x0055D450));
        SOC_CPY(self + 0x20, SPELL_STR(0x0055D510));
        SPELL_F32(0x1A4) = SPELL_F32(0x1A0) = 30.0f;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_U16(0x1FC) = 0x3ED;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U8(0x254) = 0x28;
        SPELL_S32(0x25C) = 6;
        SPELL_U32(0x258) = 0x00608B00;
        SPELL_U16(0x1FA) = 0x5E2;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_U16(0x1C8) = 0;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 1;
        SPELL_U8(0x212) = 2;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 2;
        SPELL_U8(0x22F) = 0;
        SPELL_U32(0x1B0) = 3000;
        SPELL_U8(0x23C) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D438));
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D420));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D3F0));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D3BC));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 1:
        SOC_CPY(self, SPELL_STR(0x0055D3B0));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x23C) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x213;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 2;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0x32;
        SPELL_U8(0x212) = 100;
        SPELL_U8(0x222) = 0x57;
        SPELL_U8(0x22E) = 6;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x235) = 1;
        SPELL_U8(0x236) = 1;
        SPELL_U8(0x237) = 1;
        SPELL_U8(0x239) = 1;
        SPELL_U8(0x238) = 1;
        SPELL_U8(0x23A) = 1;
        SPELL_U8(0x23B) = 1;
        SPELL_U8(0x23D) = 1;
        SPELL_U8(0x23E) = 1;
        SPELL_U8(0x23F) = 1;
        SPELL_U8(0x240) = 1;
        SPELL_U8(0x241) = 1;
        SPELL_U8(0x242) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D390));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D374));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D34C));
        break;

    case 0x11:
        SOC_CPY(self, SPELL_STR(0x0055D33C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0055D510));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x258) = 0x00605478;
        SPELL_S32(0x25C) = 1;
        SPELL_U16(0x1FA) = 0x5DC;
        SPELL_U16(0x1FC) = 0x3ED;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 8;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 10;
        SPELL_U8(0x212) = 2;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 1;
        SPELL_U8(0x237) = 10;
        SPELL_U8(0x23A) = 1;
        SPELL_U8(0x23E) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0058FBA4));
        SOC_FMT(self + 0xA0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D320));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D308));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 0x28:
        SOC_CPY(self, SPELL_STR(0x0055D2FC));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x213;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 2;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0xFFF6;
        SPELL_U8(0x212) = 100;
        SPELL_U8(0x222) = 4;
        SPELL_U8(0x22E) = 0xD;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x242) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D2E4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D2D4));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D2B0));
        break;

    case 0x27:
        SOC_CPY(self, SPELL_STR(0x0055D2A4));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U16(0x1FA) = 0x213;
        SPELL_U32(0x1B8) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x1BC) = 4;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0xFFFF;
        SPELL_U8(0x212) = 100;
        SPELL_U8(0x222) = 0xB;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x242) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D28C));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D274));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D254));
        break;

    case 0x26:
        SOC_CPY(self, SPELL_STR(0x0055D244));
        SOC_CPY(self + 0x20, SPELL_STR(0x0055D230));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x220;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 80.0f;
        SPELL_U16(0x1C8) = 5;
        SPELL_U8(0x221) = 1;
        SPELL_U16(0x1CA) = 0x1E;
        SPELL_U8(0x212) = 0x66;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 0;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x21E) = 5;
        SPELL_U8(0x242) = 0xF;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D208));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D1E0));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x24:
        SOC_CPY(self, SPELL_STR(0x0055D1D8));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_F32(0x1A0) = 0.0f;
        SPELL_U16(0x1FA) = 0x21E;
        SPELL_U32(0x1B8) = 0;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x1BC) = 0;
        SPELL_U16(0x1C8) = 5;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0x1A;
        SPELL_U8(0x22E) = 6;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 5;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 5;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 5;
        SPELL_U8(0x23F) = 5;
        SPELL_U8(0x240) = 5;
        SPELL_U8(0x241) = 5;
        SPELL_U8(0x242) = 5;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 0x23:
        SOC_CPY(self, SPELL_STR(0x0055D1C8));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_F32(0x1A0) = 0.0f;
        SPELL_U16(0x1FA) = 0x21E;
        SPELL_U32(0x1B8) = 0;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x1BC) = 0;
        SPELL_U16(0x1C8) = 5;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0x19;
        SPELL_U8(0x22E) = 6;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 2;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 2;
        SPELL_U8(0x23F) = 2;
        SPELL_U8(0x240) = 2;
        SPELL_U8(0x241) = 2;
        SPELL_U8(0x242) = 2;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 0x1A:
        SOC_CPY(self, SPELL_STR(0x0055D1B8));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x212;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 2;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 10;
        SPELL_U8(0x212) = 0x66;
        SPELL_U8(0x222) = 1;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x238) = 10;
        SPELL_U8(0x23A) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D198));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D17C));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D15C));
        break;

    case 0x12:
        SOC_CPY(self, SPELL_STR(0x0055D14C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U16(0x1FA) = 0x213;
        SPELL_U32(0x258) = 0x00605F60;
        SPELL_U32(0x1B8) = 0;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_S32(0x25C) = 2;
        SPELL_U32(0x1BC) = 2;
        SPELL_U8(0x222) = 2;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 5;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 10;
        SPELL_U8(0x212) = 0x65;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D138));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D128));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D110));
        break;

    case 0x29:
        SOC_CPY(self, SPELL_STR(0x0055D108));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x21F;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 2;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 1;
        SPELL_U16(0x1CA) = 0xFFFF;
        SPELL_U8(0x212) = 0x65;
        SPELL_U8(0x222) = 4;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x242) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D0F4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D0E8));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D2B0));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x50:
        SOC_CPY(self, SPELL_STR(0x0055D0D8));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U16(0x1FA) = 0x5DE;
        SPELL_U16(0x1FC) = 0x3EF;
        SPELL_S32(0x25C) = 5;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x1BC) = 1;
        SPELL_U16(0x1CA) = 1;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_U32(0x258) = 0x00608018;
        SPELL_U32(0x1B8) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0xD;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x240) = 3;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D0C4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D0B0));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D094));
        break;

    case 0x4B:
        SOC_CPY(self, SPELL_STR(0x0055D084));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x21F;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 1;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 5;
        SPELL_U16(0x1CA) = 0xFFFE;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x23E) = 4;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055D070));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055D048));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055D030));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x3A:
        SOC_CPY(self, SPELL_STR(0x0055D018));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SOC_CPY(self + 0x40, SPELL_STR(0x0055D008));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_S32(0x25C) = 4;
        SPELL_U32(0x258) = 0x00607530;
        SPELL_U16(0x1FA) = 0x5E0;
        SPELL_U16(0x1FC) = 0x3F1;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 10.0f;
        SPELL_U16(0x1C8) = 0xF;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0x21;
        SPELL_U8(0x22E) = 6;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 5;
        SPELL_U8(0x238) = 1;
        SPELL_U8(0x23A) = 5;
        SPELL_U8(0x23E) = 5;
        SPELL_U8(0x23F) = 5;
        SPELL_U8(0x240) = 5;
        SPELL_U8(0x241) = 5;
        SPELL_U8(0x242) = 5;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 1;
        break;

    case 0x36:
        SOC_CPY(self, SPELL_STR(0x0055CFF8));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E4BC));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_U16(0x1FC) = 0x3F2;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U8(0x254) = 0x2C;
        SPELL_U16(0x1FA) = 0x5E1;
        SPELL_U32(0x1B8) = 3;
        SPELL_S32(0x25C) = 0;
        SPELL_U32(0x258) = 0x00604990;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 80.0f;
        SPELL_U16(0x1C8) = 10;
        SPELL_U8(0x221) = 2;
        SPELL_U16(0x1CA) = 0xFFEC;
        SPELL_U8(0x212) = 0x66;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 0;
        SPELL_U8(0x22F) = 0;
        SPELL_U32(0x1B0) = 3000;
        SPELL_U8(0x21E) = 6;
        SPELL_U8(0x238) = 1;
        SPELL_U8(0x240) = 10;
        SPELL_U8(0x241) = 2;
        SOC_FMT(self + 0x60, SPELL_STR(0x0058FBA4));
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CFD0));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CFA8));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x32:
        SOC_CPY(self, SPELL_STR(0x0055CF9C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SOC_CPY(self + 0x40, SPELL_STR(0x0058FBA4));
        SPELL_F32(0x1A0) = 0.0f;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_S32(0x25C) = 4;
        SPELL_U32(0x258) = 0x00607530;
        SPELL_U16(0x1FA) = 0x5E0;
        SPELL_U16(0x1FC) = 0x3F1;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_U16(0x1C8) = 5;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0x2707;
        SPELL_U8(0x212) = 0x65;
        SPELL_U8(0x222) = 0x20;
        SPELL_U8(0x22E) = 6;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x236) = 9;
        SPELL_U8(0x238) = 1;
        SPELL_U8(0x23A) = 0xF;
        SPELL_U8(0x23E) = 5;
        SPELL_U8(0x241) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 1;
        break;

    case 0x30:
        SOC_CPY(self, SPELL_STR(0x0055CF8C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x220;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 80.0f;
        SPELL_U16(0x1C8) = 5;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0x1B;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x242) = 10;
        SOC_CPY(self + 0x60, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0xA0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x120, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 0x2A:
        SOC_CPY(self, SPELL_STR(0x0055CF7C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U16(0x1FA) = 0x21E;
        SPELL_U32(0x1B8) = 0;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x1BC) = 1;
        SPELL_U16(0x1CA) = 1;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0xC;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x240) = 6;
        SPELL_U8(0x242) = 5;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CF70));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CF64));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055CF4C));
        break;

    case 0x55:
        SOC_CPY(self, SPELL_STR(0x0055CF3C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E4BC));
        SPELL_F32(0x1A4) = SPELL_F32(0x1A0) = 30.0f;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x5DF;
        SPELL_U16(0x1FC) = 0x3F0;
        SPELL_S32(0x25C) = 3;
        SPELL_U32(0x258) = 0x00606A48;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_U32(0x1C4) = 10000;
        SPELL_U16(0x1C8) = 0xC;
        SPELL_U8(0x221) = 2;
        SPELL_U16(0x1CA) = 0xFFFB;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 8;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x238) = 1;
        SPELL_U8(0x23A) = 0x14;
        SPELL_U8(0x242) = 0x14;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CF0C));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CEDC));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    default:
        SOC_CPY(self, SPELL_STR(0x0055CECC));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_F32(0x1A0) = 0.0f;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x21E;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0;
        SPELL_U16(0x1C8) = 0;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 1;
        SPELL_U8(0x22F) = 0;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CE9C));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CE78));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055CE5C));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x35B:
        SOC_CPY(self, SPELL_STR(0x0055CE50));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E4BC));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_U16(0x1FC) = 0x247;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x23B;
        SPELL_S32(0x25C) = 0;
        SPELL_U32(0x258) = 0x00604990;
        SPELL_U32(0x1B8) = 2;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 80.0f;
        SPELL_U16(0x1C8) = 0;
        SPELL_U8(0x221) = 2;
        SPELL_U16(0x1CA) = 0xFFF6;
        SPELL_U8(0x212) = 0x66;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 0;
        SPELL_U8(0x254) = 0x2C;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x21E) = 8;
        SOC_CPY(self + 0x60, SPELL_STR(0x0055CE3C));
        SOC_CPY(self + 0xA0, SPELL_STR(0x0055CE2C));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CE04));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CFA8));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x5E:
        SOC_CPY(self, SPELL_STR(0x0055CDF8));
        SOC_CPY(self + 0x20, SPELL_STR(0x0055D510));
        SPELL_F32(0x1A0) = 0.0f;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x258) = 0x00605478;
        SPELL_S32(0x25C) = 1;
        SPELL_U16(0x1FA) = 0x5DC;
        SPELL_U16(0x1FC) = 0x3ED;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 0x32;
        SPELL_U16(0x1C8) = 0;
        SPELL_U8(0x221) = 5;
        SPELL_U16(0x1CA) = 0;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0x2C;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SOC_FMT(self + 0x60, SPELL_STR(0x0058FBA4));
        SOC_FMT(self + 0xA0, SPELL_STR(0x0058FBA4));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CDBC));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CD8C));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055CD48));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x5D:
        SOC_CPY(self, SPELL_STR(0x0055CD3C));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x212) = 0xFF;
        SPELL_U16(0x1FA) = 0x5E1;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FC) = 0x3F2;
        SPELL_U32(0x1B8) = 0;
        SPELL_S32(0x25C) = 3;
        SPELL_U32(0x258) = 0x00606A48;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 80.0f;
        SPELL_U16(0x1C8) = 6;
        SPELL_U8(0x221) = 2;
        SPELL_U16(0x1CA) = 0xFFFA;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x23F) = 1;
        SPELL_U8(0x240) = 1;
        SPELL_U8(0x241) = 1;
        SPELL_U8(0x242) = 1;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CD18));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CCF8));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x58:
        SOC_CPY(self, SPELL_STR(0x0055CCEC));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x212) = 0xFF;
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x21F;
        SPELL_U32(0x1B8) = 720000;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 10.0f;
        SPELL_U16(0x1C8) = 0;
        SPELL_U8(0x221) = 1;
        SPELL_U16(0x1CA) = 0xFFFA;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SOC_CPY(self + 0x60, SPELL_STR(0x0055CCBC));
        SOC_CPY(self + 0xA0, SPELL_STR(0x0055CC8C));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CC6C));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CC40));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        SPELL_U8(0x21F) = 0;
        break;

    case 0x57:
        SOC_CPY(self, SPELL_STR(0x0055CC34));
        SOC_CPY(self + 0x20, SPELL_STR(0x0053E89C));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23A) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_U8(0x242) = 0xFF;
        SPELL_U16(0x1FA) = 0x21E;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U32(0x1B8) = 720000;
        SPELL_U32(0x1BC) = 0;
        SPELL_F32(0x1A0) = 10.0f;
        SPELL_U16(0x1C8) = 0;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 5000;
        SPELL_U8(0x212) = 100;
        SPELL_U8(0x222) = 0;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SOC_CPY(self + 0x60, SPELL_STR(0x0055CC04));
        SOC_CPY(self + 0xA0, SPELL_STR(0x0055CBD4));
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CBBC));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CBA8));
        SOC_CPY(self + 0x160, SPELL_STR(0x0058FBA4));
        break;

    case 0x56:
        SOC_CPY(self, SPELL_STR(0x0055CB94));
        SOC_CPY(self + 0x20, SPELL_STR(0x0055D510));
        SPELL_U8(0x235) = 0xFF;
        SPELL_U8(0x236) = 0xFF;
        SPELL_U8(0x237) = 0xFF;
        SPELL_U8(0x239) = 0xFF;
        SPELL_U8(0x238) = 0xFF;
        SPELL_U8(0x23B) = 0xFF;
        SPELL_U8(0x23C) = 0xFF;
        SPELL_U8(0x23D) = 0xFF;
        SPELL_U8(0x23E) = 0xFF;
        SPELL_U8(0x23F) = 0xFF;
        SPELL_U8(0x240) = 0xFF;
        SPELL_U8(0x241) = 0xFF;
        SPELL_F32(0x1A8) = 0.0f;
        SPELL_U16(0x1FA) = 0x21E;
        SPELL_U32(0x1B8) = 0;
        SPELL_U32(0x1BC) = 2;
        SPELL_F32(0x1A0) = 30.0f;
        SPELL_U16(0x1C8) = 4;
        SPELL_U8(0x221) = 0;
        SPELL_U16(0x1CA) = 1;
        SPELL_U8(0x212) = 0;
        SPELL_U8(0x222) = 0xE;
        SPELL_U8(0x22E) = 5;
        SPELL_U8(0x22F) = 0;
        SPELL_U8(0x23A) = 0x14;
        SPELL_U8(0x242) = 0x14;
        SOC_FMT(self + 0x60, SPELL_STR(0x0055D5B8), self);
        SOC_FMT(self + 0xA0, SPELL_STR(0x0055D5AC), self);
        SOC_CPY(self + 0xE0, SPELL_STR(0x0055CB60));
        SOC_CPY(self + 0x120, SPELL_STR(0x0055CB38));
        SOC_CPY(self + 0x160, SPELL_STR(0x0055CB18));
        break;
    }

done:
    if (SPELL_U32(0x1C4) > 14000 && SPELL_U8(0x222) != 'L')
        SPELL_U32(0x1C4) = 14000;
    return self;
}

// FUNCTION: TRILOGY 0x00489c5c
// Creates seven particle cloud definitions using the snowflake sprite.
// ORIGINAL FILE: EverQuest/EQ_Spell.cpp (from Mac debug map)
int __cdecl CreatePartEffects(void)
{
    typedef void* (__cdecl *GetPointerFromDictionary_guess_t)(void*, const char*);
    typedef void* (__cdecl *DefineParticleCloud_guess_t)(void*, const char*, int, int, int, float, void*,
        int, int, int, int, int, int, float*, float, float, int, float, int, float, u8*, void*);
    u32* particleIds;
    int count;
    void* particleDefinition;
    DefineParticleCloud_guess_t defineParticleCloud;
    float localVector[3];

    *localVector = 0.0f;
    particleIds = EQ_ARRAY(u32, 0x006B3198);
    for (count = 0x40; count != 0; count = count - 1)
    {
        *particleIds = 0;
        particleIds = particleIds + 1;
    }
    localVector[1] = 0.0f;
    localVector[2] = -1.0f;
    particleDefinition = EQ_GLOBAL(GetPointerFromDictionary_guess_t, 0x006E52E0)(
        g_cdisplay_guess->world_guess, (const char*)0x0055D608);
    defineParticleCloud = EQ_GLOBAL(DefineParticleCloud_guess_t, 0x006E50B0);
    if (particleDefinition != 0)
    {
        EQ_GLOBAL(u32, 0x006B3198) = (u32)defineParticleCloud(
            g_cdisplay_guess->world_guess, (const char*)0x0055D600, 3, 0x701, 500,
            0.0f, EQ_ARRAY(void, 0x0055CB00), 0, 0, 3, 0x640, 0, 0, localVector, 0, 20.0f, 1000, 3.0f,
            0x14, 0.05f, 0, particleDefinition);
        particleDefinition = EQ_GLOBAL(GetPointerFromDictionary_guess_t, 0x006E52E0)(
            g_cdisplay_guess->world_guess, (const char*)0x0055D608);
        if (particleDefinition != 0)
        {
            EQ_GLOBAL(u32, 0x006B319C) = (u32)defineParticleCloud(
                g_cdisplay_guess->world_guess, (const char*)0x0055D5F8, 3, 0x701,
                1000, 0.0f, EQ_ARRAY(void, 0x0055CB00), 0, 0, 1, 0x640, 0, 0, 0, 5.0f, 0.0f, 1000,
                3.0f, 2, 0.05f, 0, particleDefinition);
            localVector[0] = 0.0f;
            localVector[1] = 0.0f;
            localVector[2] = 1.0f;
            particleDefinition = EQ_GLOBAL(GetPointerFromDictionary_guess_t, 0x006E52E0)(
                g_cdisplay_guess->world_guess, (const char*)0x0055D608);
            if (particleDefinition != 0)
            {
                EQ_GLOBAL(u32, 0x006B31A0) = (u32)defineParticleCloud(
                    g_cdisplay_guess->world_guess, (const char*)0x0055D5F0, 3, 0x701,
                    500, 5.0f, EQ_ARRAY(void, 0x0055CB00), 0, 0, 3, 0x640, 0, 0, localVector, 0, 20.0f,
                    1000, 7.0f, 0x14, 0.05f, 0, particleDefinition);
                localVector[0] = 0.0f;
                localVector[2] = -1.0f;
                localVector[1] = 0.0f;
                particleDefinition = EQ_GLOBAL(GetPointerFromDictionary_guess_t, 0x006E52E0)(
                    g_cdisplay_guess->world_guess, (const char*)0x0055D608);
                if (particleDefinition != 0)
                {
                    EQ_GLOBAL(u32, 0x006B31A4) = (u32)defineParticleCloud(
                        g_cdisplay_guess->world_guess, (const char*)0x0055D5E8, 3,
                        0x701, 1000, 0.0f, EQ_ARRAY(void, 0x0055CB00), 0, 0, 4, 5000, 0, 0, localVector,
                        20.0f, 0.0f, 1000, 7.0f, 2, 0.2f, 0, particleDefinition);
                    localVector[0] = 0.0f;
                    localVector[1] = 0.0f;
                    localVector[2] = 1.0f;
                    particleDefinition = EQ_GLOBAL(GetPointerFromDictionary_guess_t, 0x006E52E0)(
                        g_cdisplay_guess->world_guess, (const char*)0x0055D608);
                    if (particleDefinition != 0)
                    {
                        EQ_GLOBAL(u32, 0x006B31A8) = (u32)defineParticleCloud(
                            g_cdisplay_guess->world_guess, (const char*)0x0055D5E0,
                            3, 0x701, 1000, 5.0f, EQ_ARRAY(void, 0x0055CB00), 0, 0, 4, 5000, 0, 0,
                            localVector, 4.0f, 0.0f, 1000, 7.0f, 2, 0.2f, 0, particleDefinition);
                        localVector[1] = 0.0f;
                        localVector[0] = -1.0f;
                        localVector[2] = 0.0f;
                        particleDefinition = EQ_GLOBAL(GetPointerFromDictionary_guess_t, 0x006E52E0)(
                            g_cdisplay_guess->world_guess, (const char*)0x0055D608);
                        if (particleDefinition != 0)
                        {
                            EQ_GLOBAL(u32, 0x006B31AC) = (u32)defineParticleCloud(
                                g_cdisplay_guess->world_guess,
                                (const char*)0x0055D5D8, 3, 0x701, 2000, 0.0f,
                                EQ_ARRAY(void, 0x0055CB00), 0, 0, 3, 5000, 0, 0, localVector, 0, 20.0f,
                                2000, 20.0f, 1, 0.2f, 0, particleDefinition);
                            localVector[0] = 0.0f;
                            localVector[1] = 0.0f;
                            localVector[2] = 1.0f;
                            particleDefinition = EQ_GLOBAL(GetPointerFromDictionary_guess_t, 0x006E52E0)(
                                g_cdisplay_guess->world_guess,
                                (const char*)0x0055D608);
                            if (particleDefinition != 0)
                            {
                                EQ_GLOBAL(u32, 0x006B31B0) = (u32)defineParticleCloud(
                                    g_cdisplay_guess->world_guess,
                                    (const char*)0x0055D5D0, 3, 0x701, 1000, 5.0f,
                                    EQ_ARRAY(void, 0x0055CB00), 0, 0, 2, 5000, 0, 0, localVector,
                                    5.0f, 0.0f, 1000, 7.0f, 2, 0.2f, 0, particleDefinition);
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0048a06b
// Loads spell effect records from spells.eff, creating the file if it cannot be opened.
// ORIGINAL FILE: EverQuest/EQ_Spell.cpp (from Mac debug map)
int __cdecl GetEffFiles_guess(void)
{
    FILE* file;
    int readCount;

    file = fopen(EQ_ARRAY(char, 0x0055D62C), EQ_ARRAY(char, 0x0053C2BC));
    if (file != 0)
    {
        readCount = fread(EQ_ARRAY(void, 0x00604990), 0xAE8, 0x100, file);
        if (readCount != 0x100)
            EQ_CDECL(int, 0x005229E8, (const char*))(EQ_ARRAY(char, 0x0055D61C));
        fclose(file);
        return 1;
    }

    WriteSpellEffects();
    return 0;
}

// FUNCTION: TRILOGY 0x0048a0c2
// Saves the spell effect table to spells.eff.
// ORIGINAL FILE: EverQuest/EQ_Spell.cpp (from Mac debug map)
int __cdecl WriteSpellEffects(void)
{
    FILE* f;
    int n;

    f = fopen(EQ_ARRAY(char, 0x0055D62C), EQ_ARRAY(char, 0x005588DC));
    if (f != 0)
    {
        n = fwrite(EQ_ARRAY(void, 0x00604990), 0xAE8, 0x100, f);
        if (n != 0x100)
            EQ_CDECL(int, 0x005229E8, (const char*, ...))(EQ_ARRAY(char, 0x0055D658));
        fclose(f);
        return 1;
    }
    EQ_CDECL(int, 0x005229E8, (const char*, ...))(EQ_ARRAY(char, 0x0055D638));
    return 0;
}

// FUNCTION: TRILOGY 0x0048a11f
// Finds an effect in a spell's effect list and returns its one-based slot.
// ORIGINAL FILE: EverQuest/EQ_Spell.cpp (inferred from mapped address neighbors)
u8 __cdecl FindSpellEffectSlot_guess(u8 *spell, u32 effect)
{
    int i;

    i = 0;
    while (i < 12) {
        if ((u32)spell[0x222 + i] == effect)
            return i + 1;
        i = i + 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0048a13e
// Initializes the spell effect table, then loads its data file.
// ORIGINAL FILE: EverQuest/EQ_Spell.cpp (from Mac debug map)
// @early-stop: VC6 keeps record+4 in ECX (original keeps chunk there), so chunk-relative operands and item-loop store order differ
int EQ_Spell_guess::InitializeEffectTable_guess(void)
{
    u32* record;
    u32* chunk;
    u32* item;
    float* itemFloat;
    u32* itemFlag;
    u16* fieldWord;
    float* fieldFloat;
    u8* fieldByte;
    u32* fieldFlag;
    int chunkIndex;
    int itemIndex;
    int fieldIndex;

    record = EQ_ARRAY(u32, 0x00604994);
    do
    {
        record[-1] = 0;
        record[0] = 0;
        chunk = record + 0x90;
        for (chunkIndex = 0; chunkIndex < 3; chunkIndex++)
        {
            *(u8*)(chunk - 0x77) = 0;
            itemFloat = (float*)chunk;
            itemFlag = chunk - 0x8F;
            item = chunk - 0x6C;
            for (itemIndex = 0; itemIndex < 3; itemIndex++)
            {
                *(u8*)itemFlag = 0;
                item[-3] = 0xFFFFFFFF;
                item[0] = 0xFFFFFFFF;
                *((u8*)item + 0x196) = 0;
                *((u8*)item + 0x194) = 0;
                *((u8*)item + 0x195) = 0;
                *(float*)(item + 0x68) = 0.0f;
                item[0x7A] = 0;
                itemFlag += 8;
                *(float*)(item + 0x7D) = 0.0f;
                item[0x80] = 0;
                *(float*)(item + 0x83) = 0.0f;
                itemFloat[-1] = 0.0f;
                *(float*)(item + 0x74) = 0.0f;
                *(float*)(item + 0x77) = 0.0f;
                itemFloat[0] = 0.0f;
                itemFloat[1] = 0.0f;
                itemFloat += 3;
                item++;
            }
            chunk[-9] = 0xFFFFFFFF;
            chunk[-8] = 0xFFFFFFFF;
            fieldWord = (u16*)chunk + 0x6A;
            fieldFloat = (float*)chunk + 0x4D;
            fieldByte = (u8*)chunk + 0x69;
            fieldFlag = chunk - 0x69;
            for (fieldIndex = 0; fieldIndex < 12; fieldIndex++)
            {
                fieldWord[-0xC] = 0;
                fieldWord[0] = 0;
                fieldFloat[-0x12] = 0.0f;
                fieldWord[0x24] = 0;
                fieldFloat[0] = 1.0f;
                *(u8*)fieldFlag = 0;
                fieldWord++;
                fieldFloat[-0x2A] = 0.0f;
                fieldByte[-1] = 0;
                fieldByte[0] = 0;
                fieldByte[1] = 0;
                fieldFloat++;
                fieldByte += 3;
                fieldFlag += 8;
            }
            chunk += 0xE8;
        }
        record += 0x2BA;
    } while ((int)record < 0x006B3194);

    GetEffFiles_guess();
    return 1;
}
