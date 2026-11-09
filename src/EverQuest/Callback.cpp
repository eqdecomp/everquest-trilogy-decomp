#include "trilogy/display.h"
#include "trilogy/eqgfx.h"

// FUNCTION: TRILOGY 0x00405854
// ORIGINAL FILE: EverQuest/Callback.cpp (from Mac debug map)
// __cdecl. Handles actor callback types 0x3e, 0x40 and 0x41.
int SpriteCallBack(int* callbackData, int unusedArg, int actorPtr)
{
    int result;
    float floatResult;

    result = *callbackData;
    if (result != 0x3e)
    {
        if ((0x3f < result) && (result <= 0x41))
        {
            return 1;
        }
        return 0;
    }
    else
    {
        floatResult = EQ_METHOD(3, float, 0x00411694, (CDisplay*, void*, float, float), g_cdisplay_guess)((void*)actorPtr, 1.0f, 0.0f);
        result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, unsigned int))
            (*(const char**)(*(int*)(actorPtr + 100) + 8),
             (const char*)EQ_ARRAY(char, 0x0053C3E0), 6);
        if (result == 0)
        {
            t3dSetActorUserData((void*)actorPtr, (void*)6);
        }
        else
        {
            result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, unsigned int))
                (*(const char**)(*(int*)(actorPtr + 100) + 8),
                 (const char*)EQ_ARRAY(char, 0x0053C3D4), 10);
            if (result == 0)
            {
                t3dSetActorUserData((void*)actorPtr, (void*)8);
            }
            else if (floatResult > EQ_GLOBAL(float, 0x00534618))
            {
                t3dSetActorUserData((void*)actorPtr, (void*)4);
            }
            else
            {
                t3dSetActorUserData((void*)actorPtr, (void*)7);
            }
        }
        return 1;
    }
}
