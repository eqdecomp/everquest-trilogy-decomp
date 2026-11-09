// EQAnimate.cpp - the animation part of CDisplay: animation ids, fallbacks, attachment track speeds.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (from Mac debug map)

#include <string.h>

#include "trilogy/common.h"
#include "trilogy/player.h"
#include "trilogy/display.h"
#include "trilogy/eqanimate.h"

#define EQ_STRNICMP(a, b, n) EQ_CDECL(int, 0x005316C0, (const char*, const char*, unsigned int))((a), (b), (n))
#define EQ_STRCPY(dst, src) EQ_CDECL(char*, 0x005219C0, (char*, const char*))((dst), (src))

// FUNCTION: TRILOGY 0x0048dbb9
// Maps a model's DAG-name prefix to the model code whose animation tracks it borrows (ELF, DWM, TRK, ...).
// @early-stop: cross-jump owners of the M, O and RK suffix stores differ; the IKM block is out of line.
/* fake match: a shared byte result reproduces the original one-exit epilogue. */
char __stdcall mapCharacterPrefixToRaceCode_guess(char* dagName, char* raceCode, char humanDefault)
{
    char result = 0;

    EQ_STRCPY(raceCode, dagName);
    /* fake match: the ELF test is negated so its stores come last, the original's block order */
    if (
        !(EQ_STRNICMP(dagName, (char*)0x55da8c, 3) == 0
        || EQ_STRNICMP(dagName, (char*)0x55da88, 3) == 0
        || EQ_STRNICMP(dagName, (char*)0x55da84, 3) == 0
        || EQ_STRNICMP(dagName, (char*)0x55da80, 3) == 0)
    )
    {
            if (
            EQ_STRNICMP(dagName, (char*)0x55da7c, 3) == 0
            || EQ_STRNICMP(dagName, (char*)0x55da78, 3) == 0
        )
        {
            *raceCode = 0x44;
            raceCode[1] = 0x57;
            raceCode[2] = 0x4d;
            result = 1;
        }
        else if (
            EQ_STRNICMP(dagName, (char*)0x55da74, 3) == 0
            || EQ_STRNICMP(dagName, (char*)0x55da70, 3) == 0
            || EQ_STRNICMP(dagName, (char*)0x55da6c, 3) == 0
            || EQ_STRNICMP(dagName, (char*)0x559f78, 3) == 0
        )
        {
            *raceCode = 0x54;
            raceCode[1] = 0x52;
            raceCode[2] = 0x4b;
        }
        else
        {
            if (
                EQ_STRNICMP(dagName, (char*)0x55da68, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da64, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da60, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da5c, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da58, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da54, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da50, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da4c, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da48, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da44, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da40, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da3c, 3) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da38, 3) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da34, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da30, 3) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da2c, 3) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da28, 2) != 0
                && EQ_STRNICMP(dagName, (char*)0x55da24, 2) != 0
            )
            {
                if (
                    EQ_STRNICMP(dagName, (char*)0x55da20, 2) != 0
                    && EQ_STRNICMP(dagName, (char*)0x55da1c, 2) != 0
                    && EQ_STRNICMP(dagName, (char*)0x55da18, 2) != 0
                )
                {
                    if (
                        EQ_STRNICMP(dagName, (char*)0x55da14, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55da10, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55da0c, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55da08, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55da04, 2) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55da00, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9fc, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9f8, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9f4, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9f0, 2) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9ec, 2) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9e8, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9e4, 3) == 0
                    )
                    {
                        *raceCode = 0x44;
                        raceCode[1] = 0x57;
                        result = 1;
                    }
                    else if (
                        EQ_STRNICMP(dagName, (char*)0x55d9e0, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9dc, 2) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9d8, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9d4, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9d0, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9cc, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9c8, 3) == 0
                    )
                    {
                        *raceCode = 0x4f;
                        raceCode[1] = 0x47;
                        raceCode[2] = 0x46;
                        result = 1;
                    }
                    /* fake match: negated for the original's block order, as the ELF test above */
                    else if (
                        !(EQ_STRNICMP(dagName, (char*)0x55d9c4, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9c0, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9bc, 3) == 0
                        || EQ_STRNICMP(dagName, (char*)0x55d9b8, 3) == 0)
                    )
                    {
                            if (
                            EQ_STRNICMP(dagName, (char*)0x55d9b4, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d9b0, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d9ac, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d9a8, 3) == 0
                        )
                        {
                            *raceCode = 0x47;
                            raceCode[1] = 0x4e;
                            raceCode[2] = 0x4e;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d9a4, 3) == 0
                        )
                        {
                            *raceCode = 0x47;
                            raceCode[1] = 0x4f;
                            raceCode[2] = 0x42;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d9a0, 3) == 0
                        )
                        {
                            *raceCode = 0x46;
                            raceCode[1] = 0x52;
                            raceCode[2] = 0x4f;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d99c, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d998, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d994, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d990, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d98c, 3) == 0
                        )
                        {
                            *raceCode = 0x4c;
                            raceCode[1] = 0x49;
                            raceCode[2] = 0x4d;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d988, 3) == 0
                        )
                        {
                            *raceCode = 0x43;
                            raceCode[1] = 0x50;
                            raceCode[2] = 0x4d;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d984, 3) == 0
                        )
                        {
                            *raceCode = 0x53;
                            raceCode[1] = 0x4e;
                            raceCode[2] = 0x41;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d980, 3) == 0
                        )
                        {
                            *raceCode = 0x46;
                            raceCode[1] = 0x41;
                            raceCode[2] = 0x46;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d97c, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d978, 3) == 0
                        )
                        {
                            *raceCode = 0x52;
                            raceCode[1] = 0x41;
                            raceCode[2] = 0x54;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d974, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d970, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d96c, 3) == 0
                        )
                        {
                            *raceCode = 0x44;
                            raceCode[1] = 0x52;
                            raceCode[2] = 0x4b;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d968, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d964, 3) == 0
                        )
                        {
                            *raceCode = 0x47;
                            raceCode[1] = 0x49;
                            raceCode[2] = 0x41;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d960, 2) == 0
                        )
                        {
                            *raceCode = 0x57;
                            raceCode[1] = 0x4f;
                            raceCode[2] = 0x4c;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d95c, 3) == 0
                        )
                        {
                            *raceCode = 0x57;
                            raceCode[1] = 0x45;
                            raceCode[2] = 0x52;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d958, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d954, 3) == 0
                        )
                        {
                            *raceCode = 0x47;
                            raceCode[1] = 0x41;
                            raceCode[2] = 0x52;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d950, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d94c, 3) == 0
                        )
                        {
                            *raceCode = 0x53;
                            raceCode[1] = 0x50;
                            raceCode[2] = 0x49;
                        }
                        else if (
                            EQ_STRNICMP(dagName, (char*)0x55d948, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d944, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d940, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55841c, 3) == 0
                            || EQ_STRNICMP(dagName, (char*)0x55d93c, 3) == 0
                        )
                        {
                            *raceCode = 0x49;
                            raceCode[1] = 0x4b;
                            raceCode[2] = 0x4d;
                            result = 1;
                        }
                        else
                        {
                            if (
                                EQ_STRNICMP(dagName, (char*)0x55d938, 3) == 0
                            )
                            {
                                *raceCode = 0x4b;
                                raceCode[1] = 0x47;
                                raceCode[2] = 0x4f;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d934, 3) == 0
                            )
                            {
                                *raceCode = 0x53;
                                raceCode[1] = 0x52;
                                raceCode[2] = 0x57;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d930, 3) == 0
                            )
                            {
                                *raceCode = 0x52;
                                raceCode[1] = 0x45;
                                raceCode[2] = 0x41;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d92c, 3) == 0
                                || EQ_STRNICMP(dagName, (char*)0x55d928, 3) == 0
                            )
                            {
                                *raceCode = 0x44;
                                raceCode[1] = 0x52;
                                raceCode[2] = 0x41;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d924, 3) == 0
                            )
                            {
                                *raceCode = 0x52;
                                raceCode[1] = 0x48;
                                raceCode[2] = 0x49;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d920, 3) == 0
                            )
                            {
                                *raceCode = 0x44;
                                raceCode[1] = 0x4d;
                                raceCode[2] = 0x4c;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d91c, 3) == 0
                            )
                            {
                                *raceCode = 0x53;
                                raceCode[1] = 0x43;
                                raceCode[2] = 0x41;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d918, 3) == 0
                            )
                            {
                                *raceCode = 0x53;
                                raceCode[1] = 0x50;
                                raceCode[2] = 0x45;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d914, 3) == 0
                                || EQ_STRNICMP(dagName, (char*)0x55d910, 3) == 0
                            )
                            {
                                *raceCode = 0x46;
                                raceCode[1] = 0x53;
                                raceCode[2] = 0x47;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d90c, 3) == 0
                            )
                            {
                                *raceCode = 0x56;
                                raceCode[1] = 0x52;
                                raceCode[2] = 0x4d;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d908, 3) == 0
                            )
                            {
                                *raceCode = 0x41;
                                raceCode[1] = 0x56;
                                raceCode[2] = 0x49;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d904, 3) == 0
                            )
                            {
                                *raceCode = 0x54;
                                raceCode[1] = 0x45;
                                raceCode[2] = 0x4e;
                            }
                            else if (
                                EQ_STRNICMP(dagName, (char*)0x55d900, 3) == 0
                                || EQ_STRNICMP(dagName, (char*)0x55d8fc, 3) == 0
                            )
                            {
                                *raceCode = 0x57;
                                raceCode[1] = 0x41;
                                raceCode[2] = 0x4c;
                            }
                        }
                    }
                    else
                    {
                        *raceCode = 0x45;
                        raceCode[1] = 0x4c;
                        raceCode[2] = 0x4d;
                        result = 1;
                    }
                }
                else if (humanDefault != '\0')
                {
                    *raceCode = 0x48;
                    raceCode[1] = 0x55;
                    result = 1;
                }
                else
                {
                    *raceCode = 0x45;
                    raceCode[1] = 0x4c;
                    result = 1;
                }
            }
            else
            {
                *raceCode = 0x45;
                raceCode[1] = 0x4c;
                result = 1;
            }
        }
    }
    else
    {
        *raceCode = 0x45;
        raceCode[1] = 0x4c;
        raceCode[2] = 0x46;
        result = 1;
    }
    return result;
}

// The t3d entry points are GetProcAddress'd into data slots at start-up (eqgfx_dx7.dll), all __cdecl.
typedef int* (__cdecl *t3dGetActorSprite_guess_t)(void* actor, int unknown1, int unknown2);
typedef int (__cdecl *t3dIsHierarchicalSpriteValid_guess_t)(int* sprite);
typedef void* (__cdecl *t3dGetHierarchicalSpriteDefinition_guess_t)(int* sprite);
typedef unsigned int (__cdecl *t3dGetHierarchicalSpriteDefinitionNumDags_guess_t)(void* definition);
typedef void* (__cdecl *t3dGetHierarchicalSpriteDagByName_guess_t)(int* sprite, const char* name);
typedef void* (__cdecl *t3dGetHierarchicalSpriteDagByIndex_guess_t)(int* sprite, unsigned int index);
typedef int (__cdecl *t3dGetObjectTag_guess_t)(void* object, char* buffer);

// FUNCTION: TRILOGY 0x0048e6ac
// Loads an actor's animation tracks: for every DAG, the track of each of the 74 animations and its variants.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (from Mac debug map)
// __thiscall (`this` in ECX, EDX unused), callee pops 12 bytes. Fills the animation set `animSetPtr` for the actor
// with sprite handle `actorHandle`: its definition and DAG count, the tunic DAG, then for each of the 74 animations
// (20-byte prefixes at this+0x2df0) the track names the sprite has, with the track kind in each track's +0x1221.
void __fastcall CDisplay_InitAnimationTracks_guess(void* self, int ignored_edx, int animSetPtr, void* actorHandle, EQPlayer* spawn)
{
    char raceCode[128];
    char tunicDagName[128];
    char dagTag[128];
    char altDagName[128];
    char trackName[128];
    char altTrackName[128];
    char dagName[128];
    int* hierarchicalSprite;
    int display;
    int slotNumber;
    int primaryDagIndex;
    int* trackEntry;
    int animIndex;
    unsigned int slot;
    int animSet;
    int tmp;
    int curDag;
    char mapResult;
    char temp;
    unsigned char nextSlot;
    void* ptr;
    unsigned short numDags;
    size_t nameLen;
    short* numDagsPtr;

    primaryDagIndex = -1;
    animSet = animSetPtr;
    display = (int)self;
    if (actorHandle == 0)
    {
        EQ_CDECL(void, 0x004F8220, (const char*))((const char*)0x0055dad0);
    }
    else if ((spawn != 0) && (spawn->actor_info != 0))
    {
        hierarchicalSprite = EQ_GLOBAL(t3dGetActorSprite_guess_t, 0x006E5288)(actorHandle, 0, 0);
        tmp = EQ_GLOBAL(t3dIsHierarchicalSpriteValid_guess_t, 0x006E5330)(hierarchicalSprite);
        if (tmp != 0)
        {
            ptr = EQ_GLOBAL(t3dGetHierarchicalSpriteDefinition_guess_t, 0x006E52CC)(hierarchicalSprite);
            *(void**)(animSet + 0x28) = ptr;
            numDags = (unsigned short)EQ_GLOBAL(t3dGetHierarchicalSpriteDefinitionNumDags_guess_t, 0x006E52D8)(ptr);
            *(unsigned short*)(animSet + 0x2c) = numDags;
            EQ_CDECL(void, 0x004A0841, (char*, const char*, int))(dagName, (char*)spawn->actor_info + 8, 3);
            if ((*(char*)(display + 0x98) == 1) || (spawn->race == 0x82))
            {
                tmp = EQ_METHOD(1, int, 0x004B4531, (void*, int), spawn)(-1);
                if ((tmp != 0) && (spawn->actor_info->hier_sprite_int_guess != 0))
                {
                    if (*(int*)((char*)spawn->actor_info + 0x270) == 0)
                    {
                        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(tunicDagName, (const char*)0x0053e380, dagName);
                        ptr = EQ_GLOBAL(t3dGetHierarchicalSpriteDagByName_guess_t, 0x006E52C8)(
                            *(int**)((char*)spawn->actor_info + 0x250), tunicDagName);
                        spawn->actor_info->tunic_point_dag = ptr;
                    }
                    if (*(int*)((char*)spawn->actor_info + 0x270) != 0)
                    {
                        *(char*)(animSet + 0x39c) = 1;
                    }
                }
            }
            mapResult = EQ_STDCALL(char, 0x0048DBB9, (char*, char*, char))(dagName, raceCode, *(char*)(animSet + 0x39c));
            numDagsPtr = (short*)(animSet + 0x2c);
            animSetPtr = 0;
            if (0 < *numDagsPtr)
            {
                do
                {
                    if (0x90 < (short)animSetPtr)
                    {
                        return;
                    }
                    curDag = (int)(short)animSetPtr;
                    ptr = EQ_GLOBAL(t3dGetHierarchicalSpriteDagByIndex_guess_t, 0x006E52C4)(hierarchicalSprite, curDag);
                    tmp = EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(ptr, dagTag);
                    if (tmp != -0xc2c2)
                    {
                        nameLen = EQ_CDECL(size_t, 0x005223D0, (const char*))(dagTag);
                        nameLen = (size_t)(short)((short)nameLen + -3);
                        EQ_CDECL(char*, 0x005231D0, (char*, const char*, size_t))(dagName, dagTag, nameLen);
                        dagName[nameLen] = '\0';
                        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(altDagName, (const char*)0x0055dac8, raceCode, dagName + 3);
                        tmp = EQ_STRNICMP(dagName + 3, (char*)0x0055dac4, 2);
                        if (((((tmp == 0) || (tmp = EQ_STRNICMP(dagName + 3, (char*)0x0055da64, 2), tmp == 0)) ||
                              (tmp = EQ_STRNICMP(dagName + 3, (char*)0x0055dac0, 2), tmp == 0)) ||
                             ((tmp = EQ_STRNICMP(dagName + 3, (char*)0x0055dabc, 2), tmp == 0 ||
                              (tmp = EQ_STRNICMP(dagName + 3, (char*)0x0055dab8, 2), tmp == 0)))) ||
                            (tmp = EQ_STRNICMP(dagName + 3, (char*)0x0055dab4, 2), tmp == 0))
                        {
                            *(int*)(animSet + 0x158 + curDag * 4) = 1;
                        }
                        else
                        {
                            tmp = EQ_STRNICMP(dagName + 3, (char*)0x0053d888, 2);
                            if (tmp == 0)
                            {
                                *(int*)(animSet + 0x158 + curDag * 4) = 1;
                                primaryDagIndex = curDag;
                            }
                            else
                            {
                                *(int*)(animSet + 0x158 + curDag * 4) = 0;
                            }
                        }
                        tmp = display;
                        if (((*(char*)(display + 0x98) == 1) &&
                             (curDag = EQ_METHOD(1, int, 0x004B4531, (void*, int), spawn)(-1), curDag == 0)) &&
                            ((mapResult == 1 && (curDag = EQ_METHOD(1, int, 0x004BFB2E, (void*, int), EQ_GLOBAL(void*, 0x006E4D54))(g_zone_id_guess), 2 < curDag))))
                        {
                            *(char*)(animSet + 0x39c) = 1;
                        }
                        animIndex = 0;
                        trackEntry = (int*)(animSet + 0x30);
                        tmp = tmp + 0x2df0;
                        do
                        {
                            if (*(char*)(animSet + 0x39c) == 0)
                            {
loadBaseTrack:
                                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(trackName, (const char*)0x0055da90, (char*)tmp, dagName);
                                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(altTrackName, (const char*)0x0055da90, (char*)tmp, altDagName);
                                temp = EQ_METHOD(6, char, 0x004A01F9, (void*, char*, char*, int, int, unsigned int, int), (void*)animSet)(trackName, altTrackName, animSetPtr, animIndex, 0, primaryDagIndex);
                                if (temp != 0)
                                {
                                    *(char*)(*trackEntry + 0x1221) = 0;
                                }
                            }
                            else
                            {
                                slot = slot & 0xffffff00;
                                slotNumber = 0;
                                do
                                {
                                    temp = (char)slot + 'A';
                                    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(trackName, (const char*)0x0055daa8, (char*)tmp, (int)temp, dagName);
                                    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(altTrackName, (const char*)0x0055daa8, (char*)tmp, (int)temp, altDagName);
                                    temp = EQ_METHOD(6, char, 0x004A01F9, (void*, char*, char*, int, int, unsigned int, int), (void*)animSet)(trackName, altTrackName, animSetPtr, animIndex, slot, primaryDagIndex);
                                    if (temp == 0)
                                    {
                                        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(trackName, (const char*)0x0055da9c, (char*)tmp, slotNumber, dagName);
                                        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(altTrackName, (const char*)0x0055da9c, (char*)tmp, slotNumber, altDagName);
                                        temp = EQ_METHOD(6, char, 0x004A01F9, (void*, char*, char*, int, int, unsigned int, int), (void*)animSet)(trackName, altTrackName, animSetPtr, animIndex, slot, primaryDagIndex);
                                        if (temp != 0)
                                        {
                                            *(char*)(*trackEntry + 0x1221) = 2;
                                        }
                                    }
                                    else
                                    {
                                        *(char*)(*trackEntry + 0x1221) = 1;
                                    }
                                    nextSlot = (char)slot + 1;
                                    slot = (slot & 0xffffff00) | nextSlot;
                                    slotNumber = slotNumber + 1;
                                } while (nextSlot < 8);
                                if (temp == 0)
                                {
                                    goto loadBaseTrack;
                                }
                            }
                            animIndex = animIndex + 1;
                            trackEntry = trackEntry + 1;
                            tmp = tmp + 0x14;
                        } while ((short)animIndex < 0x4a);
                    }
                    animSetPtr = animSetPtr + 1;
                } while ((short)animSetPtr < *(short*)(animSet + 0x2c));
            }
        }
    }
}

#undef EQ_STRNICMP
#undef EQ_STRCPY

// FUNCTION: TRILOGY 0x0048ebb3
// Chooses an available alternate state for a spawn's requested animation.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (address range)
// __stdcall. When the spawn's animation set has alternate tracks and one exists for this animation, the
// alternate state to use: 0 if `state` is set, else 6, provided that entry is present; otherwise `state`
// unchanged.
char __stdcall toggleAnimationState_guess(int anim, EQPlayer* player, char state)
{
    char* set = player->actor_info->anim_set_guess;

    if (set[0x39C] == 1)
    {
        char* tracks = ((char**)(set + 0x30))[anim];
        if (tracks != 0)
        {
            char alt = state == 0 ? 6 : 0;
            if (*(int*)(tracks + (u8)alt * 0x244 + 4) != 0)
                return alt;
        }
    }
    return state;
}

// FUNCTION: TRILOGY 0x0048ebf8
// Selects a substitute animation when the requested one has no usable track.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (address range)
// __thiscall that ignores `this`. When toggleAnimationState changes *state, stores it and keeps `anim`;
// otherwise the animation to play instead of `anim` (0x20 or, with g_useAltFallbackAnim_guess == 1, 0x2B by default).
int CDisplay::getFallbackAnimationId_guess(int anim, EQPlayer* player, char* state)
{
    char s = toggleAnimationState_guess(anim, player, *state);

    if (s != *state)
    {
        *state = s;
        return anim;
    }
    switch (anim)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
        return 5;
    case 0xA:
    case 0x15:
    case 0x19:
        return 0x25;
    case 0xB:
        return 1;
    case 0xC:
    case 0xE:
    case 0xF:
        return 0xD;
    case 0xD:
        return 0xC;
    case 0x25:
        if (player != 0 && player->race == 0x48)
            return 0x12;
        return 0x11;
    case 0x11:
        return 0x12;
    case 0x12:
        return 0x11;
    case 0x1A:
        return 0x20;
    case 0x2A:
    case 0x2C:
        return 0x2B;
    case 0x48:
        return 0x1A;
    }
    if (g_useAltFallbackAnim_guess == 1 && anim != 0x2B)
        return 0x2B;
    return 0x20;
}

// FUNCTION: TRILOGY 0x0048eccd
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (address range)
// __thiscall that ignores `this`. Animations 0x20, 0x26, 0x47.
bool CDisplay::isStandingAnimation_guess(int anim)
{
    if (anim == 0x20 || anim == 0x26 || anim == 0x47)
        return true;
    return false;
}

// FUNCTION: TRILOGY 0x0048ece9
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (address range)
// __thiscall that ignores `this`. Animations 0x1A, 0x48, 0x49.
bool CDisplay::isAnimationSitOrStand_guess(int anim)
{
    if (anim == 0x1A || (anim > 0x47 && anim <= 0x49))
        return true;
    return false;
}

// FUNCTION: TRILOGY 0x0048ed05
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (address range)
// __thiscall that ignores `this`. Animations 0x20, 1..0x11, 0x2A..0x2F, 0xFE, 0x22.
// @early-stop: original has the true block first (one shared mov al,1 at the top, backward jumps); VC6 merges the true sites and keeps the bottom copy.
bool CDisplay::isMovementAnimation_guess(int anim)
{
    bool moving;

    if (anim == 0x20 || (anim >= 1 && anim <= 0x10) || (anim >= 0x2A && anim <= 0x2F) || anim == 0xFE
        || anim == 0x11)
        moving = true;
    else
        moving = anim == 0x22;
    return moving;
}

// FUNCTION: TRILOGY 0x0048ed3b
// Forwards the spawn's animation request with its animation-set track selection.
// ORIGINAL FILE: EverQuest/EQAnimate.cpp (from Mac debug map)
#undef CDisplay_PlayAnimationTrack_guess
void __fastcall CDisplay_PlayAnimationTrack_guess(
    CDisplay* display, void* edxDummy, EQPlayer* player, int animation,
    int oneCycle, int temp, float speed, int param7)
{
    char* animSet;
    u32 track;

    /* Only the low byte is written; the upper three bytes of the slot keep the display pointer from the prologue. */
    ((u8*)&track)[0] = 0;
    if (player != 0 && player->actor_info != 0
        && (animSet = player->actor_info->anim_set_guess) != 0)
    {
        if (animSet[0x39C] != 0)
            ((u8*)&track)[0] = player->weaponAnimationType;

        EQ_THISCALL(void, 0x0048ED8D,
            (CDisplay*, int, EQPlayer*, int, int, int, float, int, u32))
            (display, (int)player, player, animation, oneCycle, temp, speed, param7, track);
    }
}

// FUNCTION: TRILOGY 0x0048ed8d
// __fastcall on CDisplay; plays an animation on a spawn: player, animation, oneCycle, reverse, speed, param7, track.
// Returns without effect for an out-of-range animation (0x4a or more), race 0xd6 or more, missing actor_info,
// or dead/lying players unless the animation is 0x10.
// Then picks a fallback animation from stand state and weapon type, finds the model's dag tracks, and for each
// dag either restarts the current track or fades to the new one with its speed, reverse and cycle settings.
// Finishes by updating the sound for the animation.
// @early-stop: register assignment and shared late blocks still differ after eight non-improving iterations.
// fake match: retained the existing label gotos for shared animation decisions and the common epilogue.
void __fastcall CDisplay_PlayAnimationTrack2_guess(
    CDisplay* display, void* edxDummy, EQPlayer* player, int animation,
    int oneCycle, int reverse, float speed, int param7, u32 track)
{
    typedef int (__cdecl *GetActorSpriteFn)(void*, int, int);
    typedef int (__cdecl *GetHierarchicalSpriteDagByIndexFn)(int, u32);
    typedef int (__cdecl *GetDagNumCyclesFn)(void*, int);
    typedef int (__cdecl *DagHasPendingTrackFn)(int);
    typedef int (__cdecl *GetDagPendingNumCyclesFn)(void*, int);
    typedef void (__cdecl *SetDagSpeedFn)(int, float);
    typedef void (__cdecl *SetDagReverseFn)(int, int);
    typedef void (__cdecl *SetDagPendingSpeedFn)(int, float);
    typedef void (__cdecl *SetDagPendingReverseFn)(int, int);
    typedef u32 (__cdecl *GetWorldTimeFn)(void*);
    typedef void (__cdecl *DagFadeToNewTrackFn)(int, u32, void*, int);
    typedef void (__cdecl *SetDagPendingOneCycleFn)(int, int);
    typedef void (__cdecl *SetDagPendingInterpolateFn)(int, int);
    typedef void (__cdecl *SetDagTrackFn)(int, u32);
    typedef int (__cdecl *RestartDagFromThisTimeFn)(int, u32);
    typedef void (__cdecl *SetDagOneCycleFn)(int, int);
    typedef void (__cdecl *SetDagInterpolateFn)(int, int);

    bool updateSound;
    float soundZ;
    char* standState;
    char cVar6;
    u8 curDagIndex;
    short sVar8;
    u32 actorSprite;
    u32 worldTime;
    u32 dagTrack;
    int animTracks;
    int* fieldPtr;
    void* actorInfo;
    int tmp;
    u32 trackIndex;
    int dagIndex;
    int scratch;
    bool cond;
    bool atLimit;
    int plainDagAnim;
    int flaggedDagAnim;
    int slot;
    u8 trackByte;
    u32 highTrack;

    if (animation < 0)
        return;
    if (animation >= 0x4a)
        return;
    if (player == 0)
        return;
    if (player->race >= 0xd6)
        return;
    fieldPtr = (int*)player->actor_info;
    if (fieldPtr == 0)
        return;
    if (fieldPtr[0x94] == 0)
        return;
    if (((player->cur_hp < 1) || (player->stand_state == 's') || (player->stand_state == 'x')) &&
        (animation != 0x10))
        return;
    scratch = fieldPtr[0x5e];
    if (scratch == 0)
        return;
    if (*fieldPtr == 0)
        return;
    plainDagAnim = animation;
    flaggedDagAnim = animation;
    tmp = (*(char*)(scratch + 0x39c) == '\x01') ? 8 : 1;
    if ((int)(track & 0xff) >= tmp)
        track &= 0xffffff00;
    if ((int)((EQActorInfo_guess*)fieldPtr)->unk_0x184 >= tmp)
        ((EQActorInfo_guess*)fieldPtr)->unk_0x184 = 0;
    standState = (char*)&player->stand_state;
    highTrack = track << 0x18;
    trackByte = (u8)track;
    if (*standState == 'n')
    {
        if (animation == 0x1a)
        {
            if (player->race == 0x3c)
                return;
            animation = 0x49;
            if ((player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)) &&
                (EQ_GLOBAL(int, 0x00590154) == 0))
                return;
        }
        else
        {
            if (animation == 0x20)
            {
                if (player->race == 0x3c)
                    return;
                animation = 0x26;
                if ((player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)) &&
                    (EQ_GLOBAL(int, 0x00590154) == 0))
                    return;
            }
            else if (0x26 < animation)
            {
                if (animation <= 0x29)
                    return;
                if (animation == 0x47)
                {
                    if (player->race == 0x3c)
                        return;
                    animation = 0x26;
                    if ((player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)) &&
                        (EQ_GLOBAL(int, 0x00590154) == 0))
                        return;
                }
                else if (animation == 0x48)
                {
                    if (player->race == 0x3c)
                        return;
                    animation = 0x49;
                    if ((player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)) &&
                        (EQ_GLOBAL(int, 0x00590154) == 0))
                        return;
                }
            }
        }
    }
    if (((player->speed != 0.0f) && (player->body_type == 1)) &&
        (*(char*)((u32)display + animation + 0x2d43) != '\0'))
        plainDagAnim = 0;
    tmp = *(int*)(scratch + 0x30 + animation * 4);
    if ((tmp == 0) || (*(int*)(tmp + (track & 0xff) * 0x244 + 4) == 0))
    {
        if (animation == 0x26)
            return;
        if (animation == 0x49)
            return;
        animation = EQ_METHOD(3, int, 0x0048EBF8, (CDisplay*, int, EQPlayer*, char*), display)(animation, player, (char*)&track);
    }
    tmp = *(int*)(scratch + 0x30 + animation * 4);
    if ((tmp == 0) || (*(int*)(tmp + (track & 0xff) * 0x244 + 4) == 0))
        animation = 0x20;
    if (animation == 0x20)
    {
        actorInfo = player->actor_info;
        tmp = ((EQActorInfo_guess*)actorInfo)->unk_0x198;
        if (tmp == 0x20)
        {
            oneCycle = 0;
            speed = 1.0f;
            cVar6 = (char)EQ_METHOD(1, bool, 0x0048ED05, (CDisplay*, int), display)(((EQActorInfo_guess*)actorInfo)->animationId_guess);
            if (cVar6 == '\0')
                animation = 0x47;
        }
        else
        {
            cVar6 = (char)EQ_METHOD(1, bool, 0x0048ED05, (CDisplay*, int), display)(((EQActorInfo_guess*)actorInfo)->animationId_guess);
            if (cVar6 == '\0')
                player->actor_info->unk_0x198 = 0x47;
            else
                player->actor_info->unk_0x198 = 0x20;
            oneCycle = 1;
            speed = 1.0f;
            animation = tmp;
        }
    }
    else if ((animation == 0x1a) &&
        ((cVar6 = (char)EQ_METHOD(1, bool, 0x0048ED05, (CDisplay*, int), display)(player->actor_info->animationId_guess), cVar6 == '\0')))
    {
        animation = 0x48;
        if ((*(int*)(scratch + 0x150) == 0) ||
            (*(int*)(*(int*)(scratch + 0x150) + (track & 0xff) * 0x244 + 4) == 0))
            animation = EQ_METHOD(3, int, 0x0048EBF8, (CDisplay*, int, EQPlayer*, char*), display)(0x48, player, (char*)&track);
    }
    tmp = *(int*)(scratch + 0x30 + animation * 4);
    if ((tmp == 0) || (*(int*)(tmp + (track & 0xff) * 0x244 + 4) == 0))
    {
        animation = 0x20;
        if ((*(int*)(scratch + 0xb0) == 0) ||
            (*(int*)(*(int*)(scratch + 0xb0) + (track & 0xff) * 0x244 + 4) == 0))
        {
            track &= 0xffffff00;
            highTrack = 0;
            trackByte = 0;
            if (*(int*)(scratch + 0xb0) == 0)
                return;
            if (*(int*)(*(int*)(scratch + 0xb0) + 4) == 0)
                return;
        }
    }
    if (player->stand_state != 'd')
    {
        if (animation == 0x20)
            return;
        if (animation == 0x22)
            return;
        if (animation == 0x47)
            return;
        if (animation == 0x1a)
            return;
        if (animation == 0x48)
            return;
        if ((animation >= 0xc) && (animation <= 0xf))
            return;
    }
    if ((player != EQ_GLOBAL(EQPlayer*, 0x006E4D8C)) &&
        (*(int*)((u32)player->actor_info + 0x18c) != 0))
    {
        if (animation == 0x15)
            return;
        if (animation == 0x19)
            return;
        if (animation == 0x25)
            return;
    }
    if ((player->actor_info->unknown0x099 >= 5) && (animation == 0x24))
        return;
    actorSprite = (u32)EQ_GLOBAL(GetActorSpriteFn, 0x006E5288)
        ((void*)*(u32*)player->actor_info, 0, 0);
    tmp = player->actor_info->animationId_guess;
    if (((tmp <= 0) || (tmp >= 0x4a)) ||
        ((tmp = *(int*)(scratch + 0x30 + tmp * 4), tmp == 0)))
        curDagIndex = 0;
    else
        curDagIndex = *(u8*)(tmp + 0x1220);
    slot = (int)EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)(actorSprite, (u32)curDagIndex);
    if ((slot == 0) &&
        (slot = (int)EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)(actorSprite, 1),
            slot == 0))
        return;
    if (animation == 0x21)
    {
        if (player->race == 0x3c)
        {
            reverse = 1 - reverse;
            goto checkSameTrack;
        }
    }
    else
    {
checkSameTrack:
        cond = false;
        if ((animation != 0x10) && (animation != 0x21) && (animation != 0x2c) &&
            (*(char*)((u32)player->actor_info + 0x184) == (u8)track))
        {
            if (((char)EQ_METHOD(1, bool, 0x0048ECCD, (CDisplay*, int), display)(animation) != '\0') &&
                ((char)EQ_METHOD(1, bool, 0x0048ECE9, (CDisplay*, int), display)(player->actor_info->animationId_guess) != '\0'))
                cond = true;
            else if ((param7 == 0) && (player->actor_info->animationId_guess < 0x4a))
                cond = true;
            else
            {
                tmp = player->actor_info->animationId_guess;
                if ((tmp == animation) ||
                    ((tmp < 0x4a) &&
                        (*(char*)((u32)display + tmp + 0x2cf9) == '\0')))
                    cond = true;
            }
        }
        if (cond)
        {
            tmp = EQ_GLOBAL(GetDagNumCyclesFn, 0x006E52B0)
                (display->world_guess, slot);
            if ((tmp == 0) ||
                ((tmp = EQ_GLOBAL(DagHasPendingTrackFn, 0x006E51B0)(slot), tmp != 0) &&
                 ((tmp = EQ_GLOBAL(GetDagPendingNumCyclesFn, 0x006E52B4)
                    (display->world_guess, slot), tmp == 0))))
            {
            if (animation < 0x22)
            {
                if (0 < animation)
                {
                    if (animation < 0xc)
                        player->actor_info->unk_0x198 = animation;
                    else if ((0x10 < animation) && (animation < 0x1a))
                        player->actor_info->unk_0x198 = 0x20;
                }
            }
            else if (animation == 0x24)
                player->actor_info->unk_0x198 = 0x20;
            else if ((0x2c < animation) && (animation < 0x30))
                player->actor_info->unk_0x198 = animation;
            if (player->body_type != 1)
                return;
            if (*(char*)((u32)display + animation + 0x2d8d) == '\0')
                return;
            tmp = (int)display + player->actor_info->animationId_guess;
            cVar6 = *(char*)(tmp + 0x2d43);
            if (cVar6 == '\0')
                return;
            if (animation == 0x20)
            {
                if (*(char*)(tmp + 0x2d8d) != '\x01')
                    return;
                slot = (int)EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)(actorSprite, 1);
                if (slot == 0)
                    return;
                tmp = EQ_GLOBAL(GetDagNumCyclesFn, 0x006E52B0)
                    (display->world_guess, slot);
                if (tmp == 0)
                    return;
                tmp = EQ_GLOBAL(DagHasPendingTrackFn, 0x006E51B0)(slot);
                if ((tmp != 0) &&
                    ((tmp = EQ_GLOBAL(GetDagPendingNumCyclesFn, 0x006E52B4)
                        (display->world_guess, slot), tmp == 0)))
                    return;
            }
            else if (cVar6 != '\x01')
                return;
            flaggedDagAnim = 0;
            }
        }
    }
    tmp = player->actor_info->animationId_guess;
    if (((((tmp == animation) && (oneCycle == 0)) && (animation != 0x16)) &&
        (player->actor_info->unk_0x184 == (u8)track)) &&
        ((player->body_type != 1) ||
         ((*(int*)(scratch + 0x30 + tmp * 4) != 0) &&
          ((actorInfo = player->actor_info,
            tmp = EQ_GLOBAL(DagHasPendingTrackFn, 0x006E52BC)(slot),
            tmp == *(int*)(*(int*)(scratch + 0x30 + ((EQActorInfo_guess*)actorInfo)->animationId_guess * 4) +
                ((u32)((EQActorInfo_guess*)actorInfo)->unk_0x184 * 0x91 + (u32)curDagIndex) * 4))))))
    {
        tmp = EQ_GLOBAL(DagHasPendingTrackFn, 0x006E51B0)(slot);
        if (tmp == 0)
        {
            sVar8 = 0;
            if (0 < *(short*)(scratch + 0x2c))
            {
                do
                {
                    tmp = (int)EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)
                        (actorSprite, (int)sVar8);
                    if (tmp != 0)
                    {
                        EQ_GLOBAL(SetDagSpeedFn, 0x006E53C8)(tmp, speed);
                        EQ_GLOBAL(SetDagReverseFn, 0x006E53C4)(tmp, reverse);
                    }
                    sVar8 = sVar8 + 1;
                } while (sVar8 < *(short*)(scratch + 0x2c));
            }
        }
        else
        {
            sVar8 = 0;
            if (0 < *(short*)(scratch + 0x2c))
            {
                do
                {
                    tmp = (int)EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)
                        (actorSprite, (int)sVar8);
                    if (tmp != 0)
                    {
                        EQ_GLOBAL(SetDagPendingSpeedFn, 0x006E53C0)(tmp, speed);
                        EQ_GLOBAL(SetDagPendingReverseFn, 0x006E53BC)(tmp, reverse);
                    }
                    sVar8 = sVar8 + 1;
                } while (sVar8 < *(short*)(scratch + 0x2c));
            }
        }
        EQ_METHOD(3, void, 0x0048FFAD, (CDisplay*, EQPlayer*, int, float), display)(player, reverse, speed);
    }
    else
    {
        slot = 1;
        tmp = (int)EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)(actorSprite, 0);
        if ((tmp == 0) ||
            ((tmp = EQ_GLOBAL(DagHasPendingTrackFn, 0x006E52BC)(tmp), tmp == 0)))
            slot = 0;
        worldTime = EQ_GLOBAL(GetWorldTimeFn, 0x006E530C)(display->world_guess);
        sVar8 = 0;
        if (0 < *(short*)(scratch + 0x2c))
        {
            do
            {
                if (0x90 < sVar8)
                    break;
                dagIndex = (int)sVar8;
                tmp = (int)EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)
                    (actorSprite, (u32)dagIndex);
                if (plainDagAnim == flaggedDagAnim)
                {
                    animTracks = *(int*)(scratch + 0x30 + animation * 4);
                    if (animTracks != 0)
                    {
                        trackIndex = track & 0xff;
                        goto lookupTrack;
                    }
noTrack:
                    dagTrack = 0;
                    goto applyTrack;
                }
                else if (*(int*)(scratch + 0x158 + dagIndex * 4) == 0)
                {
                    if (plainDagAnim != 0)
                    {
                        animTracks = *(int*)(scratch + 0x30 + plainDagAnim * 4);
                        if (animTracks == 0)
                            goto noTrack;
                        trackIndex = highTrack >> 0x18;
                        goto lookupTrack;
                    }
                }
                else if (flaggedDagAnim != 0)
                {
                    animTracks = *(int*)(scratch + 0x30 + flaggedDagAnim * 4);
                    if (animTracks == 0)
                        goto noTrack;
                    trackIndex = (u32)trackByte;
                    goto lookupTrack;
                }
                goto LAB_0048f_loop_next;
lookupTrack:
                dagTrack = *(u32*)(animTracks + (trackIndex * 0x91 + dagIndex) * 4);
applyTrack:
                if (tmp != 0)
                {
                    if (slot == 0)
                    {
                        EQ_GLOBAL(SetDagTrackFn, 0x006E53D0)(tmp, dagTrack);
                        EQ_GLOBAL(RestartDagFromThisTimeFn, 0x006E5364)(tmp, worldTime);
                        EQ_GLOBAL(SetDagOneCycleFn, 0x006E53B0)(tmp, oneCycle);
                        EQ_GLOBAL(SetDagInterpolateFn, 0x006E53AC)(tmp, 1);
                        EQ_GLOBAL(SetDagSpeedFn, 0x006E53C8)(tmp, speed);
                        EQ_GLOBAL(SetDagReverseFn, 0x006E53C4)(tmp, reverse);
                    }
                    else
                    {
                        EQ_GLOBAL(DagFadeToNewTrackFn, 0x006E51AC)
                            (tmp, dagTrack, display->world_guess, 100);
                        EQ_GLOBAL(SetDagPendingOneCycleFn, 0x006E53B8)(tmp, oneCycle);
                        EQ_GLOBAL(SetDagPendingInterpolateFn, 0x006E53B4)(tmp, 1);
                        EQ_GLOBAL(SetDagPendingSpeedFn, 0x006E53C0)(tmp, speed);
                        EQ_GLOBAL(SetDagPendingReverseFn, 0x006E53BC)(tmp, reverse);
                    }
                }
LAB_0048f_loop_next:
                sVar8 = sVar8 + 1;
            } while (sVar8 < *(short*)(scratch + 0x2c));
        }
        EQ_METHOD(7, void, 0x0048FCBD, (CDisplay*, EQPlayer*, int, int, int, float, int, u8), display)(player, animation, oneCycle, reverse, speed, slot, (u8)track);
    }
    updateSound = false;
    scratch = 0;
    if (player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
    {
        if ((((animation == 0x11) || (animation == 0x12)) || (animation == 0x16)) ||
            (animation == 0x17))
        {
checkInterval500:
            fieldPtr = (int*)((u32)player->actor_info + 0x230);
            tmp = *(int*)((u32)EQ_GLOBAL(void*, 0x006E4DE0) + 0xbc);
            if (((u32)(tmp - *fieldPtr) < 0x1f5) || (player->sneaking != '\0'))
                goto afterSoundCheck;
        }
        else
        {
            if (animation == 0x25)
            {
                actorInfo = player->actor_info;
checkInterval550:
                fieldPtr = (int*)((u32)actorInfo + 0x230);
                tmp = *(int*)((u32)EQ_GLOBAL(void*, 0x006E4DE0) + 0xbc);
                cond = (u32)(tmp - *fieldPtr) < 0x226;
                atLimit = tmp - *fieldPtr == 0x226;
            }
            else
            {
                if (animation != 0x19)
                    goto enableSound;
                actorInfo = player->actor_info;
checkInterval600:
                fieldPtr = (int*)((u32)actorInfo + 0x230);
                tmp = *(int*)((u32)EQ_GLOBAL(void*, 0x006E4DE0) + 0xbc);
                cond = (u32)(tmp - *fieldPtr) < 600;
                atLimit = tmp - *fieldPtr == 600;
            }
            if (cond || atLimit)
                goto afterSoundCheck;
        }
        *fieldPtr = tmp;
        updateSound = true;
    }
    else
    {
        if ((animation == 0x16) || (animation == 0x17))
            goto checkInterval500;
        if (animation == 0x25)
        {
            tmp = EQ_METHOD(1, int, 0x004B4531, (EQPlayer*, int), player)(-1);
            actorInfo = player->actor_info;
            if (tmp != 0)
                goto checkInterval550;
        }
        else
        {
            if (animation != 0x19)
                goto enableSound;
            tmp = EQ_METHOD(1, int, 0x004B4531, (EQPlayer*, int), player)(-1);
            actorInfo = player->actor_info;
            if (tmp != 0)
                goto checkInterval600;
        }
        if (((EQActorInfo_guess*)actorInfo)->unk_0x218 != 0)
        {
enableSound:
            updateSound = true;
        }
    }
afterSoundCheck:
    if (animation != player->actor_info->animationId_guess)
    {
        *(int*)((u32)player->actor_info + 0x22c) = 0;
        *(int*)((u32)player->actor_info + 0x228) = *(int*)((u32)player->actor_info + 0x1dc);
    }
    if (updateSound)
    {
        if (animation == 0x19)
        {
            scratch = EQ_METHOD(1, int, 0x004B4531, (EQPlayer*, int), player)(-1);
            if (scratch != 0)
            {
                scratch = 0x65;
                goto playSound;
            }
            goto noSound;
        }
        if (animation == 0x25)
        {
            scratch = EQ_METHOD(1, int, 0x004B4531, (EQPlayer*, int), player)(-1);
            if (scratch != 0)
            {
                scratch = 0x66;
                goto playSound;
            }
            goto noSound;
        }
        if (animation == 0x1a)
        {
            scratch = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 100);
            actorInfo = player->actor_info;
            if (scratch < 0x32)
                actorSprite = ((EQActorInfo_guess*)actorInfo)->unk_0x1E0;
            else
                actorSprite = ((EQActorInfo_guess*)actorInfo)->unk_0x1E4;
            ((EQActorInfo_guess*)actorInfo)->sound_data_b_guess = actorSprite;
            goto noSound;
        }
        if (animation == 0x11)
        {
            if (player->sneaking == '\0')
            {
                if (player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                    scratch = 0x5d;
                else
                {
                    actorInfo = player->actor_info;
                    scratch = ((EQActorInfo_guess*)actorInfo)->unk_0x218;
                    if (scratch == 0)
                        goto checkSoundId;
                    if (scratch != 0x5d)
                    {
storeSoundField:
                        *(int*)((u32)actorInfo + 0x228) = scratch;
                        goto noSound;
                    }
                }
                sVar8 = (short)EQ_CDECL(int, 0x004A46F6, (int, int))(0, 3);
                switch (sVar8)
                {
                case 0:
                    scratch = 0x5d;
                    goto checkSoundId;
                case 1:
                    scratch = 0x5c;
                    goto playSound;
                case 2:
                    scratch = 0x97;
                    goto playSound;
                case 3:
                    scratch = 0x98;
                    goto playSound;
                default:
                    goto playSound;
                }
            }
            goto noSound;
        }
        if (animation == 0x12)
        {
            if (player->sneaking != '\0')
                goto noSound;
            if (player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                scratch = 0x5f;
            else
            {
                actorInfo = player->actor_info;
                scratch = *(int*)((u32)actorInfo + 0x21c);
                if (scratch == 0)
                    goto checkSoundId;
                if (scratch != 0x5f)
                    goto storeSoundField;
            }
            sVar8 = (short)EQ_CDECL(int, 0x004A46F6, (int, int))(0, 3);
            if (sVar8 != 0)
            {
                if (sVar8 == 1)
                    scratch = 0x5e;
                else if (sVar8 == 2)
                    scratch = 0x99;
                else if (sVar8 == 3)
                    scratch = 0x9a;
                goto playSound;
            }
            scratch = 0x5f;
checkSoundId:
            if (scratch < 0x20)
                goto noSound;
            goto testSoundId;
        }
        if ((animation == 0x13) || (animation == 0x14))
        {
            scratch = *(int*)((u32)player->actor_info + 0x1e8);
            goto checkSoundId;
        }
        if (animation == 0x16)
        {
            if (player->sneaking != '\0')
                goto noSound;
            scratch = *(int*)((u32)player->actor_info + 0x220);
            goto checkSoundId;
        }
        if (animation == 0x17)
        {
            if (player->sneaking != '\0')
                goto noSound;
            fieldPtr = (int*)((u32)player->actor_info + 0x224);
            if (*fieldPtr == 0)
            {
                *fieldPtr = 1;
                scratch = 0x60;
            }
            else
            {
                *fieldPtr = 0;
                scratch = 0x61;
            }
        }
        else if (animation == 0x21)
        {
            if (player->sneaking != '\0')
                goto noSound;
            scratch = 0x62;
            if (player->race == 0x3c)
                scratch = 0x1d0;
        }
        else if (animation == 0x24)
        {
            if (player->sneaking != '\0')
                goto noSound;
            scratch = 0x62;
        }
        else if (animation == 0xf)
        {
            scratch = *(int*)((u32)player->actor_info + 0x204);
            goto checkSoundId;
        }
        else if (animation == 0x10)
        {
            if (player->type == '\0')
            {
                if ((player->stand_state == 'x') || (0 < player->cur_hp))
                    scratch = player->actor_info->unk_0x208;
            }
            else if (player->body_type < 0x41)
                scratch = player->actor_info->unk_0x208;
            if ((player != EQ_GLOBAL(EQPlayer*, 0x006E4D8C)) &&
                (EQ_GLOBAL(int, 0x006E4DDC) != 0))
                EQ_METHOD(1, bool, 0x004E25B5, (void*, EQPlayer*), g_environment_ptr_guess)(player);
            goto checkSoundId;
        }
        else if (animation == 1)
        {
            scratch = 0x7e;
            goto playSound;
        }
        else if (animation == 2)
        {
            scratch = 0x7a;
checkActorField234:
            if (player->actor_info->unk_0x234 != 5)
                goto playSound;
        }
        else if (animation == 3)
        {
            scratch = 0x77;
            goto checkActorField234;
        }
        else if (animation == 4)
        {
            scratch = 0x79;
            goto playSound;
        }
        else if (animation == 5)
        {
            tmp = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 3);
            if ((tmp == 1) || (player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)))
                scratch = player->actor_info->unk_0x20C;
            tmp = player->actor_info->unk_0x234;
            if (tmp == 5)
                scratch = 0x96;
            else if (tmp == 0)
            {
                if (scratch == 0)
                {
                    tmp = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 5);
                    switch (tmp)
                    {
                    case 1: scratch = 0x7e; break;
                    case 2: scratch = 0x7a; break;
                    case 3: scratch = 0x82; break;
                    case 4: scratch = 0x81; break;
                    case 5: scratch = 0x76; break;
                    }
                }
            }
            else
            {
                EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(player->y, player->x, player->z, 0x76);
                if (scratch == 0x82)
                    scratch = 0;
            }
            if ((player->race == 0x31) || (player->race == 0x7a))
                scratch = player->actor_info->unk_0x20C;
            goto checkSoundId;
        }
        else if (animation == 6)
        {
            scratch = player->actor_info->unk_0x238;
            if (scratch != 5)
            {
                if (scratch == 3)
                    scratch = 0x7c;
                else
                    scratch = (-(u32)(scratch != 0) & 0xfffffff5) + 0x81;
                goto playSound;
            }
useSound96:
            scratch = 0x96;
        }
        else
        {
            if (animation == 7)
                scratch = ((player->actor_info->unk_0x238 != 3) - 1 & 5) + 0x77;
            else if (animation == 8)
            {
                tmp = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 3);
                if ((tmp == 1) || (player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)))
                    scratch = player->actor_info->unk_0x20C;
                tmp = player->actor_info->unk_0x234;
                if (tmp == 5)
                    goto useSound96;
                if (tmp != 0)
                {
playExtraSound76:
                    EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(player->y, player->x, player->z, 0x76);
                    if (scratch == 0x82)
                        scratch = 0;
                    goto checkSoundId;
                }
                if (scratch != 0)
                    goto checkSoundId;
                scratch = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 5);
                switch (scratch)
                {
                case 1: scratch = 0x7e; break;
                case 2: scratch = 0x7a; break;
                case 3: scratch = 0x82; break;
                case 4: scratch = 0x81; break;
                case 5: scratch = 0x76; break;
                default: goto noSound;
                }
                goto checkSoundId;
            }
            else if (animation == 9)
                scratch = 0x74;
            else if (animation == 10)
            {
                tmp = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 3);
                if ((tmp == 1) || (player == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)))
                    scratch = player->actor_info->unk_0x20C;
                if (player->actor_info->unk_0x234 != 0)
                    goto playExtraSound76;
                tmp = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 5);
                switch (tmp)
                {
                case 1:
                    scratch = 0x7e;
                    goto checkSoundId;
                case 2:
                    scratch = 0x7a;
                    break;
                case 3:
                    scratch = 0x82;
                    break;
                case 4:
                    scratch = 0x81;
                    break;
                case 5:
                    scratch = 0x76;
                    break;
                default:
                    goto checkSoundId;
                }
            }
            else
            {
                if (animation != 0xb)
                {
                    if ((animation == 0x2a) || (animation == 0x2b) || (animation == 0x2c))
                        scratch = *(int*)((u32)player->actor_info + 0x214);
                    else
                    {
                        if (animation == 0x2d)
                            goto useSound80;
                        if (animation == 0x2e)
                        {
                            scratch = 0x82;
                            goto checkSoundId;
                        }
                        if (animation != 0x2f)
                            goto noSound;
                        scratch = 0x82;
                    }
                    goto checkSoundId;
                }
useSound80:
                scratch = 0x80;
            }
            goto playSound;
        }
    }
    else
    {
noSound:
        scratch = 0;
testSoundId:
        if (scratch == 0)
            goto saveAnimation;
    }
playSound:
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0)
    {
        if ((scratch < 0x5c) || ((0x5f < scratch) && ((scratch < 0x97) || (0x9a < scratch))))
            soundZ = player->z;
        else
            soundZ = player->z - player->height_offset;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(player->y, player->x, soundZ, scratch);
    }
saveAnimation:
    player->actor_info->animationId_guess = animation;
    player->actor_info->unk_0x184 = (u8)track;
    return;
}

// FUNCTION: TRILOGY 0x0048fcbd
// Matches attached model tracks to the spawn's selected animation.
// @early-stop: TRACK base folds to a lea (original keeps name in EBX across the calls), so EBX holds 1 from entry; constant pushes differ.
/* fake match: tagScratch and name share one struct so they pack into a 256-byte block like the original; the idx <= 3 test is redundant but kept. */
void __fastcall CDisplay_UpdateAttachmentAnimations_guess(
    CDisplay* display, void* edxDummy, EQPlayer* player, int animation,
    int oneCycle, int reverseOption, float playbackSpeed, int param7, u8 trackIndex)
{
    typedef int* (__cdecl *GetDagSpriteFn)(void*);
    typedef u32 (__cdecl *GetWorldTimeFn)(void*);
    typedef void* (__cdecl *GetHierarchicalSpriteDefinitionFn)(int*);
    typedef u32 (__cdecl *GetHierarchicalSpriteDefinitionNumDagsFn)(void*);
    typedef void* (__cdecl *GetHierarchicalSpriteDagByIndexFn)(int*, u32);
    typedef void (__cdecl *GetObjectTagFn)(void*, char*);
    typedef int (__cdecl *GetPointerFromDictionaryFn)(void*, const char*);
    typedef void (__cdecl *DagFadeToNewTrackFn)(void*, int, void*, int);
    typedef void (__cdecl *SetDagValueFn)(void*, int);
    typedef void (__cdecl *SetDagSpeedFn)(void*, float);
    typedef void (__cdecl *SetDagTrackFn)(void*, int);
    typedef void (__cdecl *RestartDagFromThisTimeFn)(void*, u32);

    void* attachments[4];
    void* model;
    void* entry;
    int* sprite;
    void* dag;
    struct
    {
        char tagScratch[125];
        char name[131];
    } buffers;
    size_t baseLength;
    u32 numDags;
    u32 dagIndex;
    u32 worldTime;
    char* actor;
    int track;
    int localC;
    char* displayBytes;
    EQActorInfo_guess* info;
    char* text;
    /* The original reuses the player argument slot as the attachment index. */
    u32 attachmentIndex;

    displayBytes = (char*)display;
    info = player->actor_info;
    attachments[0] = info->attach_a_guess[0];
    attachments[1] = info->attach_a_guess[1];
    attachments[2] = info->attach_b_guess[0];
    attachments[3] = info->attach_b_guess[1];
    actor = info->anim_set_guess;
    /* the strings start 3 bytes into the name buffer */
    text = buffers.name + 3;

    for (attachmentIndex = 0; attachmentIndex < 4; attachmentIndex++)
    {
        if (attachments[attachmentIndex] == 0 || (attachmentIndex == 1 && animation == 5))
            continue;
        sprite = EQ_GLOBAL(GetDagSpriteFn, 0x006E52B8)(attachments[attachmentIndex]);
        if (*sprite != 0x14)
            continue;
        worldTime = EQ_GLOBAL(GetWorldTimeFn, 0x006E530C)(display->world_guess);
        numDags = EQ_GLOBAL(GetHierarchicalSpriteDefinitionNumDagsFn, 0x006E52D8)(
            EQ_GLOBAL(GetHierarchicalSpriteDefinitionFn, 0x006E52CC)(sprite));
        if (numDags < 1)
            continue;
        if (attachmentIndex > 1)
        {
            /* the loop never passes 3, but the original tests it again here */
            if (attachmentIndex <= 3)
            {
                model = *(void**)(actor + 0x30 + animation * 4);
                entry = *(void**)((char*)model
                    + (*(u8*)((char*)model + 0x1220) + trackIndex * 0x91) * 4);
                if (entry == 0)
                    continue;
                EQ_CDECL(void, 0x004A0841, (char*, const char*, unsigned int))(
                    text, *(const char**)((char*)entry + 8), 7);
                localC = 1;
            }
        }
        else
        {
            if (animation == 6 && attachmentIndex == 1)
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(text, displayBytes + 0x2E54);
            else
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(text, displayBytes + (animation * 5 + 0xB7C) * 4);
            localC = 0;
        }

        baseLength = EQ_CDECL(size_t, 0x005223D0, (char*))(text);
        for (dagIndex = 1; dagIndex < numDags; dagIndex++)
        {
            dag = EQ_GLOBAL(GetHierarchicalSpriteDagByIndexFn, 0x006E52C4)(sprite, dagIndex);
            if (dag == 0)
                continue;
            if (localC != 0)
            {
                EQ_GLOBAL(GetObjectTagFn, 0x006E52DC)(dag, buffers.tagScratch);
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
                    buffers.name + baseLength + 3, buffers.tagScratch + 6);
            }
            else
                EQ_GLOBAL(GetObjectTagFn, 0x006E52DC)(dag, buffers.name + baseLength + 3);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
                buffers.name + EQ_CDECL(size_t, 0x005223D0, (char*))(text), (char*)0x0055DAFC);
            track = EQ_GLOBAL(GetPointerFromDictionaryFn, 0x006E52E0)(display->world_guess, text);
            if (track == 0)
            {
                if (dagIndex != 1 || localC != 0)
                    continue;
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(text, displayBytes + 0x2DF0);
                EQ_GLOBAL(GetObjectTagFn, 0x006E52DC)(dag,
                    buffers.name + EQ_CDECL(size_t, 0x005223D0, (char*))(text) + 3);
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
                    buffers.name + EQ_CDECL(size_t, 0x005223D0, (char*))(text), (char*)0x0055DAFC);
                track = EQ_GLOBAL(GetPointerFromDictionaryFn, 0x006E52E0)(display->world_guess, text);
                if (track == 0)
                    continue;
            }
            if (param7 != 0)
            {
                EQ_GLOBAL(DagFadeToNewTrackFn, 0x006E51AC)(dag, track, display->world_guess, 100);
                EQ_GLOBAL(SetDagValueFn, 0x006E53B8)(dag, oneCycle);
                EQ_GLOBAL(SetDagValueFn, 0x006E53B4)(dag, 1);
                EQ_GLOBAL(SetDagSpeedFn, 0x006E53C0)(dag, playbackSpeed);
                EQ_GLOBAL(SetDagValueFn, 0x006E53BC)(dag, reverseOption);
            }
            else
            {
                EQ_GLOBAL(SetDagTrackFn, 0x006E53D0)(dag, track);
                EQ_GLOBAL(RestartDagFromThisTimeFn, 0x006E5364)(dag, worldTime);
                EQ_GLOBAL(SetDagValueFn, 0x006E53B0)(dag, oneCycle);
                EQ_GLOBAL(SetDagValueFn, 0x006E53AC)(dag, 1);
                EQ_GLOBAL(SetDagSpeedFn, 0x006E53C8)(dag, playbackSpeed);
                EQ_GLOBAL(SetDagValueFn, 0x006E53C4)(dag, reverseOption);
            }
        }
    }
}

// FUNCTION: TRILOGY 0x0048ffad
// Sets playback speed and direction for animations on attached models.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (Mac: CDisplay::ChangeAttachmentAnimationTrackSpeeds, manual: signature)
// __thiscall that ignores `this` (EBP frame in the original). Sets speed and direction of every dag but the
// first of the hierarchical sprites attached to the spawn's four attachment points; a dag with a pending
// track gets them as pending values.
void CDisplay::ChangeAttachmentAnimationTrackSpeeds_guess(EQPlayer* player, int reverse, float speed)
{
    void* attachments[4];
    void** next;
    int count;
    EQActorInfo_guess* info = player->actor_info;

    count = 4;
    attachments[0] = info->attach_a_guess[0];
    attachments[1] = info->attach_a_guess[1];
    attachments[2] = info->attach_b_guess[0];
    attachments[3] = info->attach_b_guess[1];
    next = attachments;
    do
    {
        if (*next != 0)
        {
            int* sprite = t3dGetDagSprite(*next);
            if (*sprite == 0x14)
            {
                u32 num = t3dGetHierarchicalSpriteDefinitionNumDags(t3dGetHierarchicalSpriteDefinition(sprite));
                u32 i;
                for (i = 1; i < num; i++)
                {
                    void* dag = t3dGetHierarchicalSpriteDagByIndex(sprite, i);
                    if (dag != 0)
                    {
                        if (t3dDagHasPendingTrack(dag) == 0)
                        {
                            t3dSetDagSpeed(dag, speed);
                            t3dSetDagReverse(dag, reverse);
                        }
                        else
                        {
                            t3dSetDagPendingSpeed(dag, speed);
                            t3dSetDagPendingReverse(dag, reverse);
                        }
                    }
                }
            }
        }
        next++;
} while (--count != 0);
}

class CDisplay_guess : public CDisplay
{
public:
    void ProcessAnimation(EQPlayer* player, int processAnimation, int animationMode);
};

// FUNCTION: TRILOGY 0x00490082
// Picks a spawn's animation from its motion (walk/run by speed, turning, rising/falling, idle) and plays it.
// ORIGINAL FILE: EverQuest/EQAnimate.cpp (from Mac debug map)
void CDisplay_guess::ProcessAnimation(EQPlayer* player, int processAnimation, int animationMode)
{
    u8* actorInfo = (u8*)player->actor_info;
    int animationId;
    int randomNumber;
    int randomLimit;
    int reverse;
    int movement;
    int useAlternate;
    char animation;
    float animationSpeed;
    float movementSpeed;

    animationId = *(int*)(actorInfo + 0x180);
    if (processAnimation != 0)
    {
        if (*(int*)(actorInfo + 0x244) == 1)
            goto ProcessActorDefault;
        if (time_ms_guess == *(u32*)(actorInfo + 0x17C))
        {
            if (player->stand_state == 'o')
            {
                animation = 0x18;
                movement = 1;
                reverse = 0;
                animationSpeed = 10000.0f;
                useAlternate = 1;
                goto PlayAnimation;
            }
            if ((actorInfo[0x99] > 4) || (animationId == 0x25) || (animationId == 0x19))
                goto ProcessAnimationDisabled;
            useAlternate = 1;
        }
        else
        {
            if ((actorInfo[0x99] > 4) && (player->race != 0x18)
                && (player->race != 0x3D) && (player->race != 0x4A))
                goto ProcessAnimationDisabled;
            if (animationMode == 1)
            {
                if (player->race == 0x31)
                {
                    animation = 0x13;
                    movement = 1;
                    reverse = 0;
                    animationSpeed = 1.5f;
                    useAlternate = 1;
                }
                else
                {
                    if (player->delta_heading <= 0.0f)
                    {
                        if (0.0f <= player->delta_heading)
                            return;
                        animation = player->stand_state;
                        reverse = 0;
                    }
                    else
                    {
                        animation = player->stand_state;
                        reverse = 1;
                    }
                    useAlternate = 1;
                    movement = 1;
                    animationSpeed = 1.0f;
                    if (animation == 'o')
                        animation = 0x16;
                    else
                        animation = 0x22;
                }
                goto PlayAnimation;
            }
            if (1000 < (u32)(time_ms_guess - *(u32*)(actorInfo + 0x17C)))
            {
                if (player->stand_state == 'o')
                    return;
                randomNumber = EQ_CDECL(int, 0x004A473E, (void))();
                *(u32*)(actorInfo + 0x17C) = time_ms_guess;
                if (EQ_METHOD(1, bool, 0x0048ECCD, (CDisplay_guess*, int), this)(animationId) == 0)
                    return;
                if (EQ_GLOBAL(u8, 0x006B710C) != 0)
                    return;
                if (EQ_GLOBAL(u8, 0x006B710D) != 0)
                    return;
                randomLimit = ((player->pet_owner_id != 0) ? 0x13 : 0) + 0x50;
                if (player->type == 1)
                {
                    if (randomNumber <= randomLimit)
                        goto ProcessActorDefault;
                    if ((player->race == 0x31) || (player->race == 0x9E) || (player->race == 0xC4))
                        animationSpeed = 1.5f;
                    else
                        animationSpeed = 1.0f;
                }
                else
                {
                    useAlternate = 0;
                    animationSpeed = 1.0f;
                    if (randomNumber <= randomLimit)
                        goto ProcessDefault;
                }
                useAlternate = 0;
                movement = 1;
                reverse = 0;
                animation = 0x1A;
                goto PlayAnimation;
            }
            if (player->stand_state == 'o')
                return;
            if (animationId == 0x25)
                return;
            if (animationId == 0x19)
                return;
            useAlternate = 0;
        }
        goto ProcessDefault;

ProcessActorDefault:
        useAlternate = 0;
        goto ProcessDefault;
ProcessAnimationDisabled:
        useAlternate = 0;
        animation = 0x19;
        movement = 0;
        reverse = 0;
        animationSpeed = 1.0f;
        goto PlayAnimation;
ProcessDefault:
        animationSpeed = 1.0f;
        movement = 0;
        reverse = 0;
        animation = ' ';
        goto PlayAnimation;
    }

    *(u32*)(actorInfo + 0x17C) = 0;
    useAlternate = 0;
    movement = 1;
    reverse = 0;
    movementSpeed = 1.0f;
    if (*(int*)(actorInfo + 0x244) == 1)
    {
        if ((g_zone_id_guess != 0x54) || (player->y <= -4800.0f))
        {
            movement = 0;
            animation = 0x12;
            animationSpeed = 1.0f;
            if (animationId != 0x11)
            {
                if (animationId == 0x12)
                {
                    animationSpeed = 1.0f;
                    if (0.0f < player->speed)
                        goto ProcessAirborneEnd;
                    movementSpeed = 0.5f;
                    reverse = 0x01000000;
                }
                animation = 0x11;
                movement = 1;
                animationSpeed = movementSpeed;
            }
        }
        else
        {
            animation = ' ';
            animationSpeed = movementSpeed;
        }
ProcessAirborneEnd:
        useAlternate = 0;
        reverse = reverse >> 24;
        goto PlayAnimation;
    }
    if (actorInfo[0x185] == 6)
    {
        if (player->dz <= 0.0f)
        {
            if (0.0f <= player->dz)
                return;
            reverse = 1;
        }
        else
            reverse = 0;
        useAlternate = 1;
        animationSpeed = 1.0f;
        movement = 1;
        animation = 0x17;
        goto PlayAnimation;
    }
    if (((0.001f <= player->dy) || (player->dy <= -0.001f)
         || (0.001f <= player->dx)) || (player->dx <= -0.001f))
    {
        movementSpeed = t3dFloatSqrt(player->dy * player->dy + player->dx * player->dx);
        movementSpeed = movementSpeed * 0.5f + 1.0f;
        movement = 0;
        useAlternate = 1;
        reverse = 0;
        if (actorInfo[0x99] < 5)
        {
            animation = -1;
            if (actorInfo[0x9A] < 5)
            {
                animation = (((player->stand_state != 'o') - 1U) & 5) + 0x11;
                if (player->dz < *(float*)(actorInfo + 0xAC))
                {
                    if (-3.2f <= player->dz)
                    {
                        if (player->walk_speed < player->speed)
                            goto ProcessWalking;
                        if (player->dz != 0.0f)
                            useAlternate = 0;
                    }
                    else
                    {
                        animation = 0x15;
                        useAlternate = 1;
                    }
                }
                else
                {
                    animation = 0x13;
                    movement = 1;
                    movementSpeed = 1.0f;
                }
            }
            else if ((animationId != 0x19) && (animationId != 0x25))
            {
                animation = 0x11;
                if (player->walk_speed < player->speed)
                {
ProcessWalking:
                    animation = 0x12;
                }
            }
            if ((player->speed < 0.0f) && (animationId != 0x25) && (animationId != 0x19))
                reverse = 0x01000000;
            if (animation == -1)
                return;
            animationSpeed = movementSpeed;
            reverse = reverse >> 24;
            goto PlayAnimation;
        }
        animation = 0x25;
        if ((player->race == 0x31) && (animationMode == 1))
            animation = 0x13;
        useAlternate = 1;
        animationSpeed = movementSpeed;
        movement = 0;
    }
    else if (actorInfo[0x99] < 5)
    {
        if (4 < actorInfo[0x9A])
            return;
        if (player->dz < *(float*)(actorInfo + 0xAC) * 0.5f)
        {
            if (player->dz < -2.25f)
            {
                animation = 0x15;
                movement = 0;
                goto ProcessGroundAnimation;
            }
            if ((player->race != 0x31) || (animationMode != 1))
                goto ProcessGroundDefault;
            animation = 0x13;
        }
        else
        {
            animation = 0x14;
            movement = 1;
            goto ProcessGroundAnimation;
        }
        animationSpeed = 1.0f;
    }
    else
    {
        animation = 0x19;
        if ((player->race == 0x31) && (animationMode == 1))
            animation = 0x13;
        useAlternate = 0;
        animationSpeed = 1.0f;
        movement = 1;
    }
    reverse = 0;
    goto PlayAnimation;

ProcessGroundAnimation:
    useAlternate = 1;
    animationSpeed = 1.0f;
    goto ProcessGroundPlay;

ProcessGroundDefault:
    useAlternate = 1;
    animationSpeed = 1.0f;
ProcessGroundPlay:
    reverse = 0;

PlayAnimation:
    EQ_METHOD(6, void, 0x0048ED3B, (CDisplay*, EQPlayer*, int, int, int, float, int), this)(player, animation, movement, reverse, animationSpeed, useAlternate);
}

// FUNCTION: TRILOGY 0x00490574
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (address range)
// Constructor: zeroes the block (0x1220 bytes of dwords plus two flag bytes).
SpriteTrackBlock_guess::SpriteTrackBlock_guess()
{
    int i;

    for (i = 0; i < 0x488; i++)
        data[i] = 0;
    flag_a = 0;
    flag_b = 0;
}

// File-local stand-in for SpriteTrackBlock_guess as the original's TU saw it: its constructor (0x00490574) is
// defined elsewhere, so `new` gets an EH frame. Here it is only declared, at its client address.
class SpriteTrackBlockNew_guess
{
public:
    u32     data[0x488];        /* 0x0000 */
    u8      flag_a;             /* 0x1220 */
    u8      flag_b;             /* 0x1221 */

    // EXTERNAL: TRILOGY 0x00490574 SpriteTrackBlockNew_guess::SpriteTrackBlockNew_guess
    SpriteTrackBlockNew_guess();
};

// FUNCTION: TRILOGY 0x004a01f9
// Registers a named sprite track for use in a character animation.
// __thiscall on the CDisplay (callers: InitAnimationTracks_guess). Looks up a track by name (the second name
// when the first is not found), makes the SpriteTrackBlock_guess at blocks[blockIndex] (CDisplay + 0x30) if it
// is missing, stores the track at data[slot * 0x91 + trackId] and, when the block's flag_a is still 0 or
// trackId == primary, makes trackId the block's flag_a if the track has more than one frame. Returns 1 if found.
// @early-stop: EDI copy of display lands after the trackName push; ctor call goes via the stand-in wrapper; EH state reset.
int CDisplay::LoadSpriteTrack_guess(const char* trackName, const char* altTrackName, short trackId, short blockIndex, u8 slot, int primaryDagIndex)
{
    typedef u32 (__cdecl *GetTrackNumFramesFn)(void*);

    t3dGetPointerFromDictionary_t lookup;
    void* track;
    SpriteTrackBlockNew_guess** block;

    lookup = t3dGetPointerFromDictionary;
    track = lookup(g_cdisplay_guess->world_guess, trackName);
    if (track != 0)
    {
        block = (SpriteTrackBlockNew_guess**)((u32)this + 0x30) + blockIndex;
        if (*block == 0)
            *block = new SpriteTrackBlockNew_guess;
    }
    else
    {
        track = lookup(g_cdisplay_guess->world_guess, altTrackName);
        if (track != 0)
        {
            block = (SpriteTrackBlockNew_guess**)((u32)this + 0x30) + blockIndex;
            if (*block == 0)
                *block = new SpriteTrackBlockNew_guess;
        }
        else
            return 0;
    }
    (*block)->data[slot * 0x91 + trackId] = (u32)track;
    if ((*block)->flag_a < 1 || primaryDagIndex == trackId)
    {
        if (EQ_GLOBAL(GetTrackNumFramesFn, 0x006E5304)(track) > 1)
            (*block)->flag_a = (u8)trackId;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004bfb2e
// Groups selected ids into categories; the game's meaning of those categories is unclear.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQAnimate.cpp (queue label; graph tie)
// __thiscall that ignores `this` (called from EQ_Character::HitBySpell with ECX = [0x006E4D54]). Id ranges:
// 0x4E, 0x4F, 0x51..0x61, 0x66..0x6D -> 1; 0x6E..0x81 -> 2; 0x96..0xB3 -> 3; else 0.
int CDisplay::GetIdCategory_guess(int id)
{
    if (id == 0x4E || id == 0x4F || (id >= 0x51 && id <= 0x61) || (id >= 0x66 && id <= 0x6D))
        return 1;
    if (id >= 0x6E && id <= 0x81)
        return 2;
    if (id >= 0x96 && id <= 0xB3)
        return 3;
    return 0;
}
