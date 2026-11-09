// FUNCTION: TRILOGY 0x004e270b
// Measures distance from the local player to a position using the largest axis separation.
// @early-stop: EAX vs ECX for player ptr, FST vs FSTP for first delta, third delta stores to [ebp+0x10] vs stack-only

#include "trilogy/eqplayer.h"

float __stdcall GetMaxCoordinateDistance_guess(float yCoordinate, float xCoordinate, float zCoordinate)
{
    float xDistance;

    yCoordinate = yCoordinate - g_local_player_guess->y;
    if (yCoordinate < EQ_GLOBAL(float, 0x0053510C))
    {
        yCoordinate = -yCoordinate;
    }
    xDistance = xCoordinate - g_local_player_guess->x;
    if (xDistance < EQ_GLOBAL(float, 0x0053510C))
    {
        xDistance = -xDistance;
    }
    zCoordinate = zCoordinate - g_local_player_guess->z;
    if (zCoordinate < EQ_GLOBAL(float, 0x0053510C))
    {
        zCoordinate = -zCoordinate;
    }
    if (yCoordinate < xDistance)
    {
        xCoordinate = xDistance;
    }
    else
    {
        xCoordinate = yCoordinate;
    }
    if (xDistance < zCoordinate)
    {
        xCoordinate = zCoordinate;
    }
    return xCoordinate * EQ_GLOBAL(float, 0x00535194);
}
