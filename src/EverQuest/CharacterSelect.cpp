#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

class CCharacterSelect_guess
{
public:
    static void __fastcall DeleteCharacter_guess(int param_1);
};

// FUNCTION: TRILOGY 0x004d2f41
// ORIGINAL FILE: EverQuest/CharacterSelect.cpp (from Mac debug map)
void __fastcall CCharacterSelect_guess::DeleteCharacter_guess(int offset)
{
    size_t nameLength;

    EQ_METHOD(1, void, 0x00431568, (char *, char *), EQ_GLOBAL(char *, 0x006E4E6C))(
        (char *)(*(short *)((char *)EQ_GLOBAL(char *, 0x006E4E6C) + 0x80) * 0x1e + 0x22400 + offset));
    nameLength = EQ_CDECL(size_t, 0x005223D0, (const char *))(
        (char *)(*(short *)((char *)EQ_GLOBAL(char *, 0x006E4E6C) + 0x80) * 0x1e + 0x22400 + offset));
    EQ_CDECL(void, 0x004E1C45, (void **, u32, void *, u32, int))(
        EQ_ARRAY(void *, 0x006B3194), 0x205a,
        (void *)(*(short *)((char *)EQ_GLOBAL(char *, 0x006E4E6C) + 0x80) * 0x1e + 0x22400 + offset),
        nameLength + 1, 1);
    EQ_GLOBAL(int, 0x006B6490) = 1;
    return;
}
