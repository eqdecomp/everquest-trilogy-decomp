/*
 * physics.h - the movement code of Physics.cpp (Mac: Source/PC Source/EverQuest/Physics.cpp): vehicles
 * (boats) carrying the spawns that stand on them, pushing a spawn along its heading. The
 * eqgfx_dx7.dll entry points it calls are in eqgfx.h.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_PHYSICS_H
#define TRILOGY_PHYSICS_H

#include "common.h"
#include "eqaddr.h"
#include "player.h"
#include "eqgfx.h"

/* 0x00408156  CDisplay::FixHeading (rewritten in Display.cpp; reached through its VA from here). */
#define CDisplay_FixHeading_guess(display, heading) \
    EQ_METHOD(1, float, 0x00408156, (void *, float), (void *)(display))((heading))

void vehicle_premove(EQPlayer* vehicle);                    /* 0x004E0FC7 */
void vehicle_postmove(EQPlayer* vehicle);                   /* 0x004E1109 */
void push_along_heading(EQPlayer* player, float distance);  /* 0x004E1C10 */

#endif /* TRILOGY_PHYSICS_H */
