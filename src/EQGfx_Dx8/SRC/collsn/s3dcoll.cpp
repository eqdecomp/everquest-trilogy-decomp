#include "trilogy/eqaddr.h"

typedef int (__cdecl *s3dCollideSphereWithWorld_t)(
    unsigned long, unsigned long, float *, float *, float,
    void *, void *, int, float *, unsigned long *, char *, int);

#define s3dCollideSphereWithWorld_guess_ptr \
    EQ_GLOBAL(s3dCollideSphereWithWorld_t, 0x006E50C8)

// FUNCTION: TRILOGY 0x00414523
// Tests whether a sphere can move between two world positions without hitting collision geometry.
// ORIGINAL FILE: EQGfx_Dx8/SRC/collsn/s3dcoll.c (from Mac debug map)
/* fake match: the callback is an else-if pair with the same value; the original shares one block between the two arms, which the || form does not reproduce */
char __fastcall s3dCollideSphereWithWorld(
    void *display, int edx_dummy,
    float startX, float startY, float startZ,
    float destinationX, float destinationY, float destinationZ,
    float *resolvedX, float *resolvedY, float *resolvedZ, char mode)
{
    void *collisionRoutine;
    int collisionResult;
    float startPointData[7]; /* fake match: 7 floats as the EQ location[7] arrays are; element 6 is never used, its 4 bytes are the 0x44 frame */
    unsigned long collisionData[3];
    float destinationPosition[3];
    float resolvedPosition[3];
    char result;

    result = 1;
    if (((unsigned long *)display)[0xb25] != 0)
    {
        startPointData[0] = startX;
        startPointData[1] = startY;
        startPointData[2] = startZ;
        startPointData[3] = 0.0f;
        startPointData[4] = 0.0f;
        startPointData[5] = 0.0f;
        destinationPosition[0] = destinationX;
        destinationPosition[1] = destinationY;
        destinationPosition[2] = destinationZ;
        if (mode == '\x02')
            collisionRoutine = (void *)0x0041420f;
        else if (mode == '\x03')
            collisionRoutine = (void *)0x0041420f;
        else
        {
            collisionRoutine = (void *)0x004a3d01;
            if (mode == '\0')
                collisionRoutine = (void *)0x00414159;
        }
        collisionResult = s3dCollideSphereWithWorld_guess_ptr(
            ((unsigned long *)display)[1], ((unsigned long *)display)[0],
            startPointData, destinationPosition,
            *(float *)(*(unsigned long *)((char *)display + 0x2cac) + 8),
            collisionRoutine, (void *)0x00414147, 0, resolvedPosition,
            collisionData, &mode, 0);
        if (collisionResult == 0)
        {
            if (resolvedX != 0)
                *resolvedX = resolvedPosition[0];
            if (resolvedY != 0)
                *resolvedY = resolvedPosition[1];
            if (resolvedZ != 0)
                *resolvedZ = resolvedPosition[2];
            if (resolvedPosition[0] == destinationX)
            {
                if (resolvedPosition[1] == destinationY)
                {
                    if (resolvedPosition[2] == destinationZ)
                        result = 0;
                }
            }
        }
    }
    return result;
}
