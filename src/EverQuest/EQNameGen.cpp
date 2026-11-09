// EQNameGen.cpp - EverQuest name generation helpers.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQNameGen.cpp (from Mac debug map for 0x004AF747;
// 0x004AF5E7/0x004AF6F9 inferred from mapped address neighbors).

#include <string.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eqnamegen.h"

/* EQ_Book_guess: the actor layout is EQ_Book_guess (generated: trilogy/layout/eq_book.h); the old local view's members map by offset. */
#include "trilogy/layout/eq_book.h"

// FUNCTION: TRILOGY 0x00419010
// Word-wraps the book text into display lines (50 cols, or 30 with `/~ breaks), optionally filtering each line
void __fastcall EQ_Book_FormatBookText_guess(EQ_Book_guess* book, int edxDummy)
{
    char settingByte;
    bool foundBreak;
    size_t tmp;
    size_t scanPos;
    unsigned int remainLen;
    size_t unaff_EBX;
    int charIndex;
    char lineBuf[0x800];
    char remainText[0x800];

    foundBreak = false;
    book->textLineCount = 0;
    tmp = EQ_CDECL(size_t, 0x005223D0, (const char*))(book->text);
    if (tmp != 0)
    {
        settingByte = book->text[tmp - 1];
        if ((settingByte == '\n') || (settingByte == '\r'))
        {
            book->text[tmp - 1] = 0;
            EQ_CDECL(size_t, 0x005223D0, (const char*))(book->text);
        }
    }
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(remainText, book->text);
    tmp = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
    if (*((char*)book->settings + 0xac) == 0)
    {
        if ((int)tmp < 0x33)
            goto emitLastLine;
        remainLen = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
        while (0x32 < remainLen)
        {
            foundBreak = false;
            tmp = 0x32;
            do
            {
                if ((!foundBreak) && (remainText[tmp] == ' '))
                {
                    foundBreak = true;
                    unaff_EBX = tmp;
                }
                tmp = tmp - 1;
            } while (0x28 < (int)tmp);
            if (!foundBreak)
                unaff_EBX = 0x32;
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(lineBuf, (const char*)EQ_ARRAY(char, 0x0058FBA4));
            EQ_CDECL(char*, 0x005231D0, (char*, const char*, size_t))(lineBuf, remainText, unaff_EBX);
            lineBuf[unaff_EBX] = '\0';
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(remainText, remainText + unaff_EBX);
            settingByte = *((char*)book->settings + 0xae);
            if ((settingByte < '\0') || ('\x1f' < settingByte))
            {
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(book->formattedText + book->textLineCount * 0x3c, lineBuf);
            }
            else
            {
                EQ_CDECL(void, 0x004AFF13, (char*, char*, u8))(lineBuf,
                    book->formattedText + book->textLineCount * 0x3c,
                    (u8)((char*)EQ_GLOBAL(char*, 0x006E4DA8))[(int)settingByte + 0x7e]);
            }
            book->textLineCount++;
            remainLen = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
        }
    }
    else
    {
        charIndex = 0;
        if (0 < (int)tmp)
        {
            do
            {
                if ((remainText[charIndex] == '`') || (remainText[charIndex] == '~'))
                    foundBreak = true;
                charIndex = charIndex + 1;
            } while (charIndex < (int)tmp);
            if (foundBreak)
            {
                tmp = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
                do
                {
                    if (tmp == 0)
                        return;
                    foundBreak = false;
                    tmp = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
                    scanPos = 0;
                    if ((int)tmp < 1)
                    {
useDefaultSplit:
                        unaff_EBX = 0x1e;
                    }
                    else
                    {
                        do
                        {
                            if ((!foundBreak) && ((remainText[scanPos] == '`') || (remainText[scanPos] == '~')))
                            {
                                foundBreak = true;
                                unaff_EBX = scanPos;
                            }
                            scanPos = scanPos + 1;
                        } while ((int)scanPos < (int)tmp);
                        if (!foundBreak)
                            goto useDefaultSplit;
                    }
                    if ((int)tmp < (int)unaff_EBX)
                        unaff_EBX = tmp;
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(lineBuf, (const char*)EQ_ARRAY(char, 0x0058FBA4));
                    EQ_CDECL(char*, 0x005231D0, (char*, const char*, size_t))(lineBuf, remainText, unaff_EBX);
                    lineBuf[unaff_EBX] = '\0';
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(remainText, remainText + unaff_EBX + 1);
                    settingByte = *((char*)book->settings + 0xae);
                    if ((settingByte < '\0') || ('\x1f' < settingByte))
                    {
                        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(book->formattedText + book->textLineCount * 0x3c, lineBuf);
                    }
                    else
                    {
                        EQ_CDECL(void, 0x004AFF13, (char*, char*, u8))(lineBuf,
                            book->formattedText + book->textLineCount * 0x3c,
                            (u8)((char*)EQ_GLOBAL(char*, 0x006E4DA8))[(int)settingByte + 0x7e]);
                    }
                    book->textLineCount++;
                    if ((!foundBreak) && ((int)tmp < 0x1f))
                        remainText[0] = '\0';
                    tmp = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
                } while (true);
            }
        }
        if ((int)tmp < 0x1f)
            goto emitLastLine;
        remainLen = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
        while (0x1e < remainLen)
        {
            foundBreak = false;
            tmp = 0x1e;
            do
            {
                if ((!foundBreak) && (remainText[tmp] == ' '))
                {
                    foundBreak = true;
                    unaff_EBX = tmp;
                }
                tmp = tmp - 1;
            } while (0x14 < (int)tmp);
            if (!foundBreak)
                unaff_EBX = 0x1e;
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(lineBuf, (const char*)EQ_ARRAY(char, 0x0058FBA4));
            EQ_CDECL(char*, 0x005231D0, (char*, const char*, size_t))(lineBuf, remainText, unaff_EBX);
            lineBuf[unaff_EBX] = '\0';
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(remainText, remainText + unaff_EBX);
            settingByte = *((char*)book->settings + 0xae);
            if ((settingByte < '\0') || ('\x1f' < settingByte))
            {
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(book->formattedText + book->textLineCount * 0x3c, lineBuf);
            }
            else
            {
                EQ_CDECL(void, 0x004AFF13, (char*, char*, u8))(lineBuf,
                    book->formattedText + book->textLineCount * 0x3c,
                    (u8)((char*)EQ_GLOBAL(char*, 0x006E4DA8))[(int)settingByte + 0x7e]);
            }
            book->textLineCount++;
            remainLen = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
        }
    }
    tmp = EQ_CDECL(size_t, 0x005223D0, (const char*))(remainText);
    if (tmp == 0)
        return;
    settingByte = *((char*)book->settings + 0xae);
    goto joined_r0x00419460;

emitLastLine:
    settingByte = *((char*)book->settings + 0xae);
joined_r0x00419460:
    if ((settingByte < '\0') || ('\x1f' < settingByte))
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(book->formattedText + book->textLineCount * 0x3c, remainText);
    }
    else
    {
        EQ_CDECL(void, 0x004AFF13, (char*, char*, u8))(remainText,
            book->formattedText + book->textLineCount * 0x3c,
            (u8)((char*)EQ_GLOBAL(char*, 0x006E4DA8))[(int)settingByte + 0x7e]);
    }
    book->textLineCount++;
    return;
}

// FUNCTION: TRILOGY 0x004194c3
// Handles page movement in a scrollable text view.
// __thiscall, written as __fastcall with a dummy EDX (self in ECX, gate and action on the stack, RET 8).
// Field meanings are guesses (low). Returns unless gate == 0. With +0x90 set, action is stored at +0x84.
// Otherwise, with settings+0xac clear, a call of vtable slot 2 happens for action == 0; with it set, the
// scroll position at +0x80 moves by 2 (action 0 moves back, 1 forward to the page count of the line
// count at +0x82, 12 lines a page) and action 2 calls vtable slot 2.
/* fake match: each case stores and returns (VC6 merges the store tails); the >= is written reversed for cmp/jge */
void __fastcall ScrollView_updateScrollPosition_guess(char* self, int unused, short gate, short action)
{
    short value;

    if (((EQ_Book_guess*)self)->unk_0x90 != 0)
    {
        if (gate != 0)
            return;
        ((EQ_Book_guess*)self)->pressedControlId_guess = action;
        return;
    }
    if (gate != 0)
        return;
    if (*(char*)(*(char**)(self + 0x37ac) + 0xac) != '\0')
    {
        switch (action)
        {
        case 0:
            if (((EQ_Book_guess*)self)->pageIndex_guess <= 1)
                return;
            value = ((EQ_Book_guess*)self)->pageIndex_guess - 2;
            ((EQ_Book_guess*)self)->pageIndex_guess = value;
            return;
        case 1:
            value = ((EQ_Book_guess*)self)->textLineCount / 0xc;
            if ((int)((EQ_Book_guess*)self)->textLineCount % 0xc != 0)
                value = value + 1;
            if (((EQ_Book_guess*)self)->pageIndex_guess + 2 >= (int)value)
                return;
            value = ((EQ_Book_guess*)self)->pageIndex_guess + 2;
            ((EQ_Book_guess*)self)->pageIndex_guess = value;
            return;
        case 2:
            ((void (__fastcall *)(char*))(*(void***)self)[2])(self);
            return;
        default:
            return;
        }
    }
    else
    {
        if (action == 0)
            ((void (__fastcall *)(char*))(*(void***)self)[2])(self);
    }
    return;
}

// FUNCTION: TRILOGY 0x004af3d7
// Builds a character name from randomized parts chosen for the selected race and gender.
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (from Mac debug map)
char* __cdecl CreateName(int race, char gender)
{
    char checkResult;
    int tmp;
    size_t partLen;
    int copyIndex;
    int partIndex;
    char* partText;
    char candidate[0x800];
    int partPtrs[5];
    size_t partLens[5];
    int partOdds[5];
    char fixedName[16];
    int totalLen;
    int attempts;

    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
        EQ_ARRAY(char, 0x005FE814), (const char*)0x005681B4);
    if ((short)race == 0x80)
    {
        race = 0xd;
    }
    if ((short)race == 0x82)
    {
        race = 0xe;
    }
    if ((short)race < 0 || (short)race > 0xe)
    {
        return EQ_ARRAY(char, 0x005FE814);
    }
    attempts = 0;
    do
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            candidate, (const char*)0x0058FBA4);
        partIndex = 0;
        totalLen = 0;
        do
        {
            partPtrs[partIndex] = 0;
            if (partIndex == 0)
            {
                partOdds[0] = 100;
            }
            else if (partIndex == 1)
            {
                partOdds[1] = 0x50;
            }
            else if (partIndex == 3)
            {
                partOdds[3] = 100;
            }
            else if (partIndex == 4)
            {
                partOdds[4] = gender ? 100 : 0;
            }
            else
            {
                partOdds[partIndex] = 0x50;
            }
            tmp = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 100);
            if ((partOdds[partIndex] == 100) || (tmp < partOdds[partIndex]))
            {
                tmp = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 9);
                partText = EQ_ARRAY(char*, 0x00566838)[
                    partIndex + (tmp + (short)race * 10) * 5];
                partPtrs[partIndex] = (int)partText;
                partLen = EQ_CDECL(size_t, 0x005223D0, (const char*))(partText);
                totalLen = totalLen + partLen;
                partLens[partIndex] = partLen;
            }
            tmp = partPtrs[partIndex];
            if (tmp != 0)
            {
                EQ_CDECL(char*, 0x005219D0, (char*, const char*))(
                    candidate, (char*)tmp);
            }
            tmp = totalLen;
            partIndex = partIndex + 1;
        } while (partIndex < 5);
        attempts = attempts + 1;
        if ((totalLen >= 4) && (totalLen <= 15))
        {
            checkResult = EQ_CDECL(u8, 0x004AF5E7, (char))(candidate[0]);
            if ((checkResult == '\0') &&
                ((checkResult = EQ_CDECL(u8, 0x004AF5E7, (char))(candidate[1]),
                  checkResult == '\0' && (candidate[1] == candidate[2]))))
            {
                fixedName[0] = candidate[0];
                fixedName[1] = EQ_CDECL(u8, 0x004AF841, (void))();
                copyIndex = 2;
                partIndex = tmp + 1;
                for (; copyIndex < partIndex; copyIndex++)
                {
                    fixedName[copyIndex] = candidate[copyIndex - 1];
                }
                fixedName[copyIndex] = '\0';
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
                    candidate, fixedName);
            }
            checkResult = EQ_CDECL(u8, 0x004AF621, (char*))(candidate);
            if ((checkResult == '\0') &&
                ((checkResult = EQ_CDECL(u8, 0x004AF7D7, (char*, short))(
                      candidate, (short)race), checkResult == '\0')))
                break;
        }
    } while (attempts < 0x32);
    EQ_CDECL(void, 0x004A0841, (char*, char*, int))(
        EQ_ARRAY(char, 0x005FE814), candidate, 0xf);
    return EQ_ARRAY(char, 0x005FE814);
}

// FUNCTION: TRILOGY 0x004AF5E7
// Identifies vowels, including y, for name and speech text processing.
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (inferred from mapped address neighbors)
u8 __cdecl IsNameVowel_guess(char c)
{
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y' ||
        c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'Y')
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004AF621
// Returns 1 if a name has a letter tripled, four non-vowels in a row, or one of 25 table fragments (any case).
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (from Mac debug map)
u8 __cdecl NameFailsOnBasicChecks(char *name)
{
    short length;
    short index;
    short row;
    short column;
    char *needle;

    if (name != 0)
        {
    }
    else
        return 1;

    length = (short)strlen(name);
    index = 0;
    if (length > 0)
    {
        do
        {
            if (index + 2 < length && name[index] == name[index + 1] &&
                name[index] == name[index + 2])
                return 1;

            if (index + 3 < length &&
                EQ_CDECL(u8, 0x004AF5E7, (char))(name[index]) == 0 &&
                EQ_CDECL(u8, 0x004AF5E7, (char))(name[index + 1]) == 0 &&
                EQ_CDECL(u8, 0x004AF5E7, (char))(name[index + 2]) == 0 &&
                EQ_CDECL(u8, 0x004AF5E7, (char))(name[index + 3]) == 0)
                return 1;

            ++index;
        } while (index < length);
    }

    row = 0;
    do
    {
        column = 0;
        do
        {
            const int & temp = ((int)row * 5 + (int)column);
            needle = *(char **)(0x005679D0 + temp * 4);
            if (EQ_CDECL(u8, 0x004AF747, (char *, char *, char))(name, needle, 1) != 0)
                return 1;
            ++column;
        } while (column < 5);
        ++row;
    } while (row < 5);

    return 0;
}

// FUNCTION: TRILOGY 0x004AF6F9
// Detects listed profanity in short text being processed for Babel speech.
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (inferred from mapped address neighbors)
u8 __cdecl NameContainsBannedWord_guess(const char *name, int length)
{
    char buf[32];

    if (length <= 0x1F)
    {
        char *row;
        strcpy(buf, name);
        _strlwr(buf);
        row = (char *)0x00567A38;
        do
        {
            if (!strstr(buf, row))
                {
            }
            else
                return 1;
            row += 32;
        } while ((int)row < 0x00567CB8);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004AF747
// Checks whether text contains a fragment, with optional case-insensitive matching.
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (from Mac debug map)
bool __cdecl NameContains_guess(char *searchText, char *needle, char flag)
{
    char buf1[256];
    char buf2[256];
    char *p1;
    char *p2;
    bool result = false;

    if (searchText != 0 && needle != 0)
    {
        strlen(searchText);
        strlen(needle);
        strcpy(buf1, searchText);
        strcpy(buf2, needle);
        if (flag)
        {
            p1 = _strlwr(buf1);
            p2 = _strlwr(buf2);
        }
        else
        {
            p1 = buf1;
            p2 = buf2;
        }
        if (strstr(p1, p2) != 0)
            result = true;
        return result;
    }
    return false;
}

// FUNCTION: TRILOGY 0x004AF7D7
// Checks a candidate name for restricted fragments associated with its race.
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (from Mac debug map)
// @early-stop: the table element is pushed straight from memory (the binary loads it into EAX, then pushes EAX), and the return-1 block sits before the epilogue (after it in the binary)
/* fake match: null test as an if/else; the flat early return makes the outer loop exit jump around the return-1 block (scores 0.886) */
u8 __cdecl NameContains_guess(char *name, short category)
{
    short row;
    short column;

    if (name != 0)
    {
        if (category < 0 || category > 15)
            return 0;

        row = 0;
        do
        {
            column = 0;
            do
            {
                if (EQ_CDECL(u8, 0x004AF747, (char *, char *, char))(name,
                        EQ_ARRAY(char *, 0x005673F0)[(row + category * 5) * 5 + column], 0) != 0)
                    return 1;
                ++column;
            } while (column < 5);
            ++row;
        } while (row < 5);

        return 0;
    }
    else
        return 1;
}

// FUNCTION: TRILOGY 0x004AF841
// Supplies a randomly chosen vowel for character name generation.
u8 __cdecl CreateName3_guess(void)
{
    u8 letter;

    switch (EQ_CDECL(int, 0x004A46F6, (int, int))(0, 5))
    {
    case 0: letter = 'a'; break;
    case 1: letter = 'e'; break;
    case 2: letter = 'i'; break;
    case 3: letter = 'o'; break;
    case 4: letter = 'u'; break;
    case 5: letter = 'y'; break;
    }

    // The random range is 0..5; the original leaves letter uninitialized otherwise.
    return letter;
}

// FUNCTION: TRILOGY 0x004AF87F
// Garbles a message by a 0..100 chance: broken-English swaps (went->did go), dropped words, scrambled letters
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (from Mac debug map)
// @early-stop: first-loop register roles (index/offset/pointer), chain block placement, sentenceLen zero store elided
void __cdecl SpeakBabel(char *srcText, char *dstText, int unused, u8 chance, int param5)
{
    char nextWord[256000];
    char sentence[256000];
    int sentenceLen;
    int run;
    char word[16];
    int index;
    short temp;
    size_t textLen;

    index = 0;
    if (dstText != 0 && srcText != 0)
    {
        if (chance > 99 && param5 != 0)
        {
            EQ_CDECL(char *, 0x005219C0, (char *, const char *))(dstText, srcText);
        }
        else
        {
            *dstText = '\0';
            run = 0;
            sentenceLen = 0;
            {
                int length;

                textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(srcText);
                if (textLen > 0x3e7ff)
                    temp = -0x1801;
                else
                    temp = (short)EQ_CDECL(size_t, 0x005223D0, (const char *))(srcText);
                length = temp;
                if (length > 0)
                {
                    // curPtr walks the source; toSentence maps its address into sentence,
                    // so curPtr[(int)toSentence] is sentence[index].
                    char *curPtr = srcText;
                    char *toSentence = sentence - (int)srcText;

                    sentenceLen = length;
                    do
                    {
                        if (index == 0 || index == length - 1)
                        {
                            curPtr[(int)toSentence] = *curPtr;
                        }
                        else
                        {
                            int isAlpha = EQ_CDECL(int, 0x00522ECF, (int))((int)*curPtr);
                            char ch = *curPtr;

                            if (isAlpha == 0)
                            {
                                if (ch == ' ' || run == 0xf)
                                {
                                    run = 0;
                                    curPtr[(int)toSentence] = ' ';
                                }
                                else if (ch != '\'' && srcText[index + 1] != ' ' &&
                                         EQ_CDECL(int, 0x00522ECF, (int))((int)ch) == 0 &&
                                         EQ_CDECL(int, 0x00522F25, (int))((int)ch) == 0)
                                {
                                    run = 0;
                                    curPtr[(int)toSentence] = ' ';
                                }
                                else if (EQ_CDECL(int, 0x00522F25, (int))((int)ch) != 0 &&
                                         srcText[index - 1] != ' ' && srcText[index + 1] != ' ' &&
                                         EQ_CDECL(int, 0x00522F25, (int))((int)srcText[index - 1]) == 0 &&
                                         EQ_CDECL(int, 0x00522F25, (int))((int)srcText[index + 1]) == 0)
                                {
                                    run = 0;
                                    curPtr[(int)toSentence] = ' ';
                                }
                                else
                                {
                                    curPtr[(int)toSentence] = *curPtr;
                                    run++;
                                }
                            }
                            else
                            {
                                curPtr[(int)toSentence] = ch;
                                run++;
                            }
                        }
                        index++;
                        curPtr++;
                    } while (index < length);
                }
            }
            sentence[sentenceLen] = '\0';
            {
                char *cursor = sentence;

                while (EQ_CDECL(size_t, 0x005223D0, (const char *))(cursor) != 0)
                {
                    cursor = EQ_METHOD(2, char *, 0x004DBC84, (void *, char *, char *), EQ_GLOBAL(void *, 0x006EFE2C))(cursor, nextWord);
                    EQ_CDECL(void, 0x004A0841, (char *, const char *, int))(word, nextWord, 15);

                    if (param5 == 0)
                    {
                        temp = (short)EQ_CDECL(size_t, 0x005223D0, (const char *))(word);
                        if (temp > 0)
                        {
                            int wordLen = temp;

                            if (EQ_CDECL(char, 0x004AF6F9, (char *, int))(word, wordLen) != '\0')
                            {
                                int k;

                                k = 0;
                                if (wordLen > 0)
                                {
                                    do
                                    {
                                        word[k] = (char)EQ_CDECL(int, 0x004A46F6, (int, int))(0, 25) + '`';
                                        k++;
                                    } while (k < wordLen);
                                }
                                if (EQ_CDECL(size_t, 0x005223D0, (const char *))(dstText) == 0)
                                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(dstText, word);
                                else
                                    EQ_CDECL(char *, 0x005219D0, (char *, const char *))(dstText, word);
                                textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(dstText);
                                if (dstText[textLen - 1] != ' ')
                                    EQ_CDECL(char *, 0x005219D0, (char *, const char *))(dstText, EQ_ARRAY(char, 0x0053E89C));
                                continue;
                            }
                        }
                    }

                    u8 roll = EQ_CDECL(u8, 0x004A473E, (void))();
                    if (roll > chance)
                    {
                        if ((short)EQ_CDECL(size_t, 0x005223D0, (const char *))(word) < 8)
                        {
                            if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x0056809C)) == 0 ||
                                EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568258)) == 0 ||
                                EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568254)) == 0)
                            {
                                EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x00568250));
                            }
                            else if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568248)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568240)) == 0)
                            {
                                EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x00568054));
                            }
                            else if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568238)) == 0)
                            {
                                EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x00568230));
                            }
                            else if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568228)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568224)) == 0)
                            {
                                EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x00568220));
                            }
                            else if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568218)) == 0)
                            {
                                EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x00568210));
                            }
                            else if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00564E48)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568108)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x0056819C)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x0056820C)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005680E8)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568208)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568204)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00568200)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681FC)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681F8)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681F4)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681EC)) == 0 ||
                                     EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681E4)) == 0)
                            {
                                EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x0058FBA4));
                            }
                            else if ((int)(roll - chance) > 20)
                            {
                                if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681E0)) == 0 ||
                                    EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x00567F2C)) == 0)
                                {
                                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x0058FBA4));
                                }
                                else if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681DC)) == 0)
                                {
                                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x005681D8));
                                }
                                else if (EQ_CDECL(int, 0x00524240, (const char *, const char *))(word, EQ_ARRAY(char, 0x005681D0)) == 0)
                                {
                                    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(word, EQ_ARRAY(char, 0x005681C4));
                                }
                            }
                        }
                    }

                    temp = (short)EQ_CDECL(size_t, 0x005223D0, (const char *))(word);
                    if (temp > 0)
                    {
                        int i;

                        i = 0;
                        sentenceLen = temp;
                        if (sentenceLen > 0)
                        {
                            do
                            {
                                char *curPtr = &word[i];
                                int isAlpha = EQ_CDECL(int, 0x00522ECF, (int))((int)*curPtr);

                                if (isAlpha != 0 &&
                                    EQ_CDECL(int, 0x004A473E, (void))() > 0x28 &&
                                    (roll = EQ_CDECL(u8, 0x004A473E, (void))(), roll > 5))
                                {
                                    if (roll > chance + 10)
                                    {
                                        if (roll > 98 && i > 0 && word[i - 1] != ' ' && word[i + 1] != ' ')
                                        {
                                            *curPtr = ' ';
                                        }
                                        else if ((int)(roll - chance) > 30)
                                        {
                                            *curPtr = (char)EQ_CDECL(int, 0x004A46F6, (int, int))(0, 25) + '`';
                                        }
                                        else if (EQ_CDECL(u8, 0x004AF5E7, (char))(*curPtr) != 0)
                                        {
                                            switch (EQ_CDECL(int, 0x004A46F6, (int, int))(0, 5))
                                            {
                                            case 0:
                                                *curPtr = 'a';
                                                break;
                                            case 1:
                                                *curPtr = 'e';
                                                break;
                                            case 2:
                                                *curPtr = 'i';
                                                break;
                                            case 3:
                                                *curPtr = 'o';
                                                break;
                                            case 4:
                                                *curPtr = 'u';
                                                break;
                                            case 5:
                                                *curPtr = 'y';
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            char ch;

                                            do
                                            {
                                                ch = (char)EQ_CDECL(int, 0x004A46F6, (int, int))(0, 24) + 'a';
                                                *curPtr = ch;
                                            } while (EQ_CDECL(u8, 0x004AF5E7, (char))(ch) != 0);
                                        }
                                    }
                                }
                                i++;
                            } while (i < sentenceLen);
                        }

                        if (EQ_CDECL(size_t, 0x005223D0, (const char *))(word) != 0)
                        {
                            if (EQ_CDECL(size_t, 0x005223D0, (const char *))(dstText) == 0)
                                EQ_CDECL(char *, 0x005219C0, (char *, const char *))(dstText, word);
                            else
                                EQ_CDECL(char *, 0x005219D0, (char *, const char *))(dstText, word);
                            textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(dstText);
                            if (dstText[textLen - 1] != ' ')
                                EQ_CDECL(char *, 0x005219D0, (char *, const char *))(dstText, EQ_ARRAY(char, 0x0053E89C));
                        }
                    }
                }
            }

            int lastIndex = (int)EQ_CDECL(size_t, 0x005223D0, (const char *))(srcText) - 1;
            if (lastIndex > 0)
            {
                if (EQ_CDECL(size_t, 0x005223D0, (const char *))(dstText) != 0)
                {
                    if (EQ_CDECL(int, 0x00522ECF, (int))((int)srcText[lastIndex]) == 0 &&
                        EQ_CDECL(int, 0x00522F25, (int))((int)srcText[lastIndex]) == 0)
                    {
                        dstText[EQ_CDECL(size_t, 0x005223D0, (const char *))(dstText) - 2] = srcText[lastIndex];
                    }
                    dstText[EQ_CDECL(size_t, 0x005223D0, (const char *))(dstText) - 1] = '\0';
                    char ch = *dstText;
                    *dstText = (ch >= 'a' && ch <= 'z') ? ch - 0x20 : ch;
                }
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004AFF13
// Shows partly understood speech by garbling text according to the supplied language chance.
// ORIGINAL FILE: EverQuest/EQNameGen.cpp (from Mac debug map)
void __cdecl HearBabel(char *source, char *destination, u8 chance)
{
    int index;
    int remaining;
    short length;
    short outputIndex;
    int randomValue;
    u8 percentByte;
    char letter;

    if (destination == 0 || source == 0)
        return;

    length = (short)strlen(source);
    outputIndex = 0;
    if (chance > 99)
    {
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(destination, source);
        return;
    }

    if (length > 0)
    {
        index = 0;
        remaining = (int)length;
        do
        {
            if (EQ_CDECL(int, 0x00522ECF, (int))((int)(signed char)source[index]) == 0 ||
                EQ_CDECL(int, 0x004A473E, (void))() <= 0x32)
            {
                destination[outputIndex] = source[index];
            }
            else
            {
                percentByte = (u8)EQ_CDECL(int, 0x004A473E, (void))();
                if (percentByte <= 5 || percentByte <= chance + 10)
                {
                    destination[outputIndex] = source[index];
                }
                else if (percentByte <= 98 || outputIndex <= 0 ||
                         destination[outputIndex - 1] == ' ' || source[index + 1] == ' ' ||
                         index == (int)length - 1)
                {
                    if (percentByte - chance > 0x1E)
                    {
                        randomValue = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 0x19);
                        destination[outputIndex] = (char)((u8)randomValue + 0x60);
                    }
                    else if (EQ_CDECL(u8, 0x004AF5E7, (char))(source[index]) != 0)
                    {
                        switch (EQ_CDECL(int, 0x004A46F6, (int, int))(0, 5))
                        {
                        case 0:
                            destination[outputIndex] = 0x61;
                            break;
                        case 1:
                            destination[outputIndex] = 0x65;
                            break;
                        case 2:
                            destination[outputIndex] = 0x69;
                            break;
                        case 3:
                            destination[outputIndex] = 0x6F;
                            break;
                        case 4:
                            destination[outputIndex] = 0x75;
                            break;
                        case 5:
                            destination[outputIndex] = 0x79;
                            break;
                        }
                    }
                    else
                    {
                        do
                        {
                            randomValue = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 0x18);
                            letter = (char)randomValue;
                            destination[outputIndex] = (char)(letter + 'a');
                        } while (EQ_CDECL(u8, 0x004AF5E7, (char))(letter + 'a') != 0);
                    }
                }
                else
                {
                    destination[outputIndex] = ' ';
                }
            }

            ++outputIndex;
            ++index;
            --remaining;
        } while (remaining != 0);
    }

    destination[outputIndex] = 0;
}
