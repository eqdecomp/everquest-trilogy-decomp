// EQDoCamAi.cpp - CDisplay camera AI.

#include "trilogy/display.h"
#include "trilogy/eqgfx.h"

/* display.h has the CDisplay layout, but this method is not declared there yet. */
class CDisplay_guess
{
public:
    void DoCamAi(void* actor);
};

typedef int (__cdecl *t3dSetCameraLocation_DoCamAi_guess_t)(void*, void*, float*);
typedef int (__cdecl *t3dGetRegionFromWorldAndXYZ_DoCamAi_guess_t)(void*, float*);
typedef float (__cdecl *t3dAngleArcTangentFloat_DoCamAi_guess_t)(float, float, float*);

// FUNCTION: TRILOGY 0x004906ff
// Places the camera on the view actor: at its position in 1st person, else offset from it and kept out of walls
// ORIGINAL FILE: EverQuest/EQDoCamAi.cpp (from Mac debug map)
// @early-stop: hideActor stays in EAX (original pushes 0/1 per branch); switch is a range check, not the dec chain; clamp stores not shared; baseZ/destX slots swapped; xor edx/mov ecx order from EQ_THISCALL.
void CDisplay_guess::DoCamAi(void* viewActor)
{
    float destZ;
    u32 actorHandle;
    u32 tmp;
    float startY;
    float deltaX;
    float deltaY;
    int result;
    int cameraOffset;
    float* cameraPos;
    bool isMode2;
    float temp;
    u32 hideActor;
    CDisplay* display = (CDisplay*)this;
    u8* actorBytes = (u8*)viewActor;
    CDisplay* displayBytes = (CDisplay *)this;
    u32* actorInfo;

    actorInfo = *(u32**)(actorBytes + 0x70);
    tmp = EQ_GLOBAL(u32, 0x00590154);
    actorHandle = *actorInfo;
    if (EQ_GLOBAL(u32, 0x00590154) == 0)
    {
        result = (int)actorInfo[0x60];
        if ((result < 1) || (0xb < result))
        {
            if ((result == 0x24) || (result == 0x21))
            {
                float position[3];
                position[2] = 0.0f;
                position[1] = 0.0f;
                position[0] = 0.0f;
                EQ_METHOD(2, bool, 0x00413F8F, (CDisplay*, void*, float*), display)(viewActor, position);
                displayBytes->cameraLocation_guess[0] = position[0];
                displayBytes->cameraLocation_bits_guess[1] = *(u32*)&position[1];
                displayBytes->cameraLocation_bits_guess[2] = *(u32*)&position[2];
            }
            else
            {
                displayBytes->cameraLocation_bits_guess[0] = *(u32*)(actorBytes + 0x34);
                displayBytes->cameraLocation_bits_guess[1] = *(u32*)(actorBytes + 0x38);
                if (*(char*)(actorBytes + 0xa1) == 'o')
                {
                    displayBytes->cameraLocation_bits_guess[2] = *(u32*)(actorBytes + 0x3c);
                }
                else
                {
                    displayBytes->cameraLocation_guess[2] =
                        (*(float*)(actorBytes + 0x3c) - *(float*)(actorBytes + 0x7c)) + *(float*)(actorBytes + 0x78);
                }
            }
            hideActor = 1;
        }
        else
        {
            if (*(float*)(actorBytes + 0x60) < EQ_GLOBAL(float, 0x00535684))
            {
                u32 actorLocation[7]; /* fake match: 7th dword is never written; it shapes the frame so the array sits at [ebp-0x30] and the 3-float array at [ebp-0x14] */

                actorLocation[0] = *(u32*)(actorBytes + 0x34);
                actorLocation[1] = *(u32*)(actorBytes + 0x38);
                actorLocation[2] = *(u32*)(actorBytes + 0x3c);
                ((float*)actorLocation)[5] = 0.0f;
                actorLocation[3] = *(u32*)(actorBytes + 0x40);
                ((float*)actorLocation)[4] = EQ_GLOBAL(float, 0x00535638) + *(float*)(actorBytes + 0x60);
                t3dSetActorLocation(displayBytes->world_guess, (void*)actorHandle, (float*)actorLocation);
            }
            float position[3];
            EQ_METHOD(2, bool, 0x00413F8F, (CDisplay*, void*, float*), display)(viewActor, position);
            cameraPos = &displayBytes->cameraLocation_guess[0];
            *cameraPos = position[0];
            displayBytes->cameraLocation_bits_guess[1] = *(u32*)&position[1];
            displayBytes->cameraLocation_bits_guess[2] = *(u32*)&position[2];
            switch (*(unsigned short*)(actorBytes + 0x9a))
            {
            case 9:
            case 10:
                *cameraPos = (float)(t3dFloatFastCosine(*(float*)(actorBytes + 0x40)) + *cameraPos);
                displayBytes->cameraLocation_guess[1] = t3dFloatFastSine(*(float*)(actorBytes + 0x40)) + displayBytes->cameraLocation_guess[1];
                break;
            case 2:
                *cameraPos = (float)(t3dFloatFastCosine(*(float*)(actorBytes + 0x40)) * EQ_GLOBAL(float, 0x005344A8) + *cameraPos);
                displayBytes->cameraLocation_guess[1] = t3dFloatFastSine(*(float*)(actorBytes + 0x40)) * EQ_GLOBAL(float, 0x005344A8) + displayBytes->cameraLocation_guess[1];
                break;
            default:
                break;
            }
            hideActor = 0;
        }
        cameraPos = &displayBytes->cameraLocation_guess[0];
        t3dSetActorInvisible((void*)actorHandle, (int)hideActor);
        displayBytes->cameraLocation_bits_guess[3] = *(u32*)(actorBytes + 0x40);
        displayBytes->cameraLocation_guess[4] = *(float*)(actorBytes + 0x60) + *(float*)(actorBytes + 0x44);
        displayBytes->cameraLocation_guess[5] = 0.0f;
        result = EQ_GLOBAL(t3dSetCameraLocation_DoCamAi_guess_t, 0x006E539C)(
            displayBytes->world_guess, displayBytes->camera_guess, cameraPos);
        if (result != 0)
        {
            EQ_CDECL(void, 0x004C1703, (const char*, ...))(
                (const char*)EQ_ARRAY(char, 0x0055DC78), (double)*cameraPos,
                (double)displayBytes->cameraLocation_guess[1], (double)displayBytes->cameraLocation_guess[2], result);
        }
        tmp = *(u32*)(*(u32*)((u8 *)displayBytes + 8) + 4);
        if (tmp > *(u32*)(*(u32*)((u8 *)displayBytes + 4) + 0x18))
        {
            EQ_CDECL(void, 0x004C1703, (const char*, ...))(
                (const char*)EQ_ARRAY(char, 0x0055DC34), tmp, (double)*cameraPos,
                (double)displayBytes->cameraLocation_guess[1], (double)displayBytes->cameraLocation_guess[2]);
        }
        result = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_DoCamAi_guess_t, 0x006E52E8)(
            displayBytes->world_guess, cameraPos);
        if (!((result == 0) || ((result == (int)EQ_GLOBAL(u32, 0x005922E8) &&
                                (EQ_GLOBAL(u32, 0x00590154) == EQ_GLOBAL(u8, 0x005922E4))))))
        {
            EQ_GLOBAL(u8, 0x0058FBB8) = 0;
            EQ_GLOBAL(u32, 0x005922E8) = (u32)result;
            EQ_GLOBAL(u8, 0x0058FBB8) = CDisplay_GetEnvironment_guess(
                display, *cameraPos, displayBytes->cameraLocation_guess[1], displayBytes->cameraLocation_guess[2], 0);
            if (EQ_GLOBAL(u8, 0x0055DBA8) != EQ_GLOBAL(u8, 0x0058FBB8))
            {
                EQ_GLOBAL(u8, 0x0055DBA8) = EQ_GLOBAL(u8, 0x0058FBB8);
            }
        }
    }
    else
    {
        float baseZ;
        float destX;

        baseZ = (*(float*)(actorBytes + 0x3c) - *(float*)(actorBytes + 0x7c)) + *(float*)(actorBytes + 0x78);
        if (*(int*)((u8*)EQ_GLOBAL(void*, 0x006EFE2C) + 4) == 5)
        {
            cameraOffset = (int)EQ_GLOBAL(u32, 0x00590154) * 0x1c;
            if ((!(*(float*)((u8*)g_default_cameras_guess + cameraOffset) == EQ_GLOBAL(float, 0x0053510C)) &&
                 (*(float*)((u8*)g_default_cameras_guess + cameraOffset) < EQ_GLOBAL(float, 0x005345B8))))
                *(float*)((u8*)g_default_cameras_guess + cameraOffset) = EQ_GLOBAL(float, 0x005345B8);
            else if (EQ_GLOBAL(float, 0x005351F8) < *(float*)((u8*)g_default_cameras_guess + cameraOffset))
                *(float*)((u8*)g_default_cameras_guess + cameraOffset) = EQ_GLOBAL(float, 0x005351F8);
            if (EQ_GLOBAL(float, 0x0053510C) <= *(float*)((u8*)g_default_cameras_guess + cameraOffset + 8))
            {
                if (!(*(float*)((u8*)g_default_cameras_guess + cameraOffset + 8) <= EQ_GLOBAL(float, 0x005352FC)))
                    *(float*)((u8*)g_default_cameras_guess + cameraOffset + 8) = EQ_GLOBAL(float, 0x005352FC);
            }
            else
            {
                *(float*)((u8*)g_default_cameras_guess + cameraOffset + 8) = 0.0f;
            }
        }
        temp = EQ_METHOD(1, float, 0x00408156, (CDisplay*, float), display)(*(float*)((u8*)g_default_cameras_guess + tmp * 0x1c + 4));
        tmp = EQ_GLOBAL(u32, 0x00590154);
        isMode2 = tmp == 2;
        *(float*)((u8*)g_default_cameras_guess + tmp * 0x1c + 4) = temp;
        if (isMode2)
        {
            cameraOffset = (int)EQ_GLOBAL(u32, 0x00590154) * 0x1c;
            startY = *(float*)(actorBytes + 0x38);
            destX = (float)(*(float*)(actorBytes + 0x34) +
                              -((*(float*)(actorBytes + 0x78) + *(float*)((u8*)g_default_cameras_guess + cameraOffset)) *
                                t3dFloatFastCosine(*(float*)(actorBytes + 0x40))));
            temp = startY + -((*(float*)(actorBytes + 0x78) + *(float*)((u8*)g_default_cameras_guess + cameraOffset)) *
                               t3dFloatFastSine(*(float*)(actorBytes + 0x40)));
            destZ = baseZ + *(float*)((u8*)g_default_cameras_guess + cameraOffset + 8);
        }
        else
        {
            cameraOffset = (int)EQ_GLOBAL(u32, 0x00590154) * 0x1c;
            temp = t3dFloatFastCosine(temp);
            startY = *(float*)(actorBytes + 0x38);
            destX = (float)(temp * *(float*)((u8*)g_default_cameras_guess + cameraOffset) +
                              *(float*)(actorBytes + 0x34));
            temp = t3dFloatFastSine(*(float*)((u8*)g_default_cameras_guess + cameraOffset + 4));
            temp = temp * *(float*)((u8*)g_default_cameras_guess + cameraOffset) + startY;
            destZ = baseZ + *(float*)((u8*)g_default_cameras_guess + cameraOffset + 8);
        }
        cameraPos = &displayBytes->cameraLocation_guess[0];
        EQ_METHOD(10, bool, 0x00414523, (CDisplay*, float, float, float, float, float, float, float*, float*, float*, int), display)(*(float*)(actorBytes + 0x34), startY, *(float*)(actorBytes + 0x3c),
            destX, temp, destZ, cameraPos, &displayBytes->cameraLocation_guess[1],
            &displayBytes->cameraLocation_guess[2], 3);
        if (*(float*)((u8*)g_default_cameras_guess + EQ_GLOBAL(u32, 0x00590154) * 0x1c + 0x0c) ==
            EQ_GLOBAL(float, 0x0053510C))
        {
            displayBytes->cameraLocation_bits_guess[3] = *(u32*)(actorBytes + 0x40);
        }
        else
        {
            temp = EQ_CDECL(float, 0x004A47F9, (float, float, float, float))(
                *cameraPos, displayBytes->cameraLocation_guess[1], *(float*)(actorBytes + 0x34), *(float*)(actorBytes + 0x38));
            displayBytes->cameraLocation_guess[3] = temp;
        }
        if (*(float*)((u8*)g_default_cameras_guess + EQ_GLOBAL(u32, 0x00590154) * 0x1c + 0x10) ==
            EQ_GLOBAL(float, 0x005344A0))
        {
            deltaX = *(float*)(actorBytes + 0x34) - *cameraPos;
            deltaY = *(float*)(actorBytes + 0x38) - displayBytes->cameraLocation_guess[1];
            startY = *(float*)(actorBytes + 0x3c) - displayBytes->cameraLocation_guess[2];
            temp = t3dFloatSqrt(deltaX * deltaX + deltaY * deltaY);
            temp = EQ_GLOBAL(t3dAngleArcTangentFloat_DoCamAi_guess_t, 0x006E515C)(startY, temp, &destX);
            displayBytes->cameraLocation_guess[4] = temp;
        }
        else
        {
            displayBytes->cameraLocation_bits_guess[4] = *(u32*)((u8*)g_default_cameras_guess +
                EQ_GLOBAL(u32, 0x00590154) * 0x1c + 0x10);
        }
        displayBytes->cameraLocation_guess[5] = 0.0f;
        result = EQ_GLOBAL(t3dSetCameraLocation_DoCamAi_guess_t, 0x006E539C)(
            displayBytes->world_guess, displayBytes->camera_guess, cameraPos);
        if (result != 0)
        {
            EQ_CDECL(void, 0x004C1703, (const char*, ...))(
                (const char*)EQ_ARRAY(char, 0x0055DBEC), (double)*cameraPos,
                (double)displayBytes->cameraLocation_guess[1], (double)displayBytes->cameraLocation_guess[2], result);
        }
        tmp = *(u32*)(*(u32*)((u8 *)displayBytes + 8) + 4);
        if (tmp > *(u32*)(*(u32*)((u8 *)displayBytes + 4) + 0x18))
        {
            EQ_CDECL(void, 0x004C1703, (const char*, ...))(
                (const char*)EQ_ARRAY(char, 0x0055DBAC), tmp, (double)*cameraPos,
                (double)displayBytes->cameraLocation_guess[1], (double)displayBytes->cameraLocation_guess[2]);
        }
        result = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_DoCamAi_guess_t, 0x006E52E8)(
            displayBytes->world_guess, cameraPos);
        if (!((result == 0) || ((result == (int)EQ_GLOBAL(u32, 0x005922EC) &&
                                (EQ_GLOBAL(u32, 0x00590154) == EQ_GLOBAL(u8, 0x005922E4))))))
        {
            EQ_GLOBAL(u8, 0x0058FBB8) = 0;
            EQ_GLOBAL(u8, 0x0058FBB8) = CDisplay_GetEnvironment_guess(
                display, *cameraPos, displayBytes->cameraLocation_guess[1], displayBytes->cameraLocation_guess[2], 0);
            EQ_GLOBAL(u32, 0x005922EC) = (u32)result;
            if (EQ_GLOBAL(u8, 0x0055DBA8) != EQ_GLOBAL(u8, 0x0058FBB8))
            {
                EQ_GLOBAL(u8, 0x0055DBA8) = EQ_GLOBAL(u8, 0x0058FBB8);
            }
        }
    }
    EQ_GLOBAL(u8, 0x005922E4) = (u8)EQ_GLOBAL(u32, 0x00590154);
}
