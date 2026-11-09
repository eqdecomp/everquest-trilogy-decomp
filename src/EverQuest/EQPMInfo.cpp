/*
 * EQPMInfo.cpp - EQPMInfo (Mac: Source/PC Source/EverQuest/EQPMInfo.cpp).
 */
#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/display.h"

/* No header for EQPMInfo yet: layout from the code at 0x004BD18D only; size unknown. */
class EQPMInfo
{
public:
    void*   actor;      /* 0x00 */
    void*   light;      /* 0x04 */

    ~EQPMInfo();
};

// FUNCTION: TRILOGY 0x004bd18d
// ORIGINAL FILE: EverQuest/EQPMInfo.cpp (from Mac debug map)
// __thiscall. Deletes the light and the actor through the display, clearing both pointers.
EQPMInfo::~EQPMInfo()
{
    if (light != 0)
    {
        EQ_METHOD(1, void, 0x00410970, (CDisplay*, void*), g_cdisplay_guess)(light);
        light = 0;
    }
    if (actor != 0)
    {
        EQ_METHOD(1, void, 0x00411715, (CDisplay*, void*), g_cdisplay_guess)(actor);
        actor = 0;
    }
}
