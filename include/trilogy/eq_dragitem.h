/*
 * eq_dragitem.h - EQ_DragItem, the client's drag-item object (Mac: Source/PC Source/EverQuest/EQ_DragItem.cpp,
 * direct high mapping for CalcRedraw2).
 *
 * Method-only declarations: the layout is unrecovered (rectangles at +0x24 and +0x84), so all accesses
 * stay raw offsets. Globals below are the drag cursor/mode/bounds words CalcRedraw2 reads; 0x0058FC6C is
 * also g_game_state_guess (display.h), referenced here under its bundle name.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_DRAGITEM_H
#define TRILOGY_EQ_DRAGITEM_H

#include "common.h"
#include "eqaddr.h"
#include "eq_columns.h"

/* 0x006EFC80 / 0x006EFC84  drag cursor position (24 / 21 xrefs); clamped at zero when negative. */
#define g_dragItemX_guess                EQ_GLOBAL(int, 0x006EFC80)
#define g_dragItemY_guess                EQ_GLOBAL(int, 0x006EFC84)
/* 0x0058FC6C  display mode (== g_game_state_guess in display.h); 2 selects dynamic bounds. */
#define g_game_state_guess                EQ_GLOBAL(int, 0x0058FC6C)
/* 0x006B63FC / 0x006B6400  dynamic size refs; maxima are these minus 12 / 21 (signed, may go negative). */
#define g_screenWidth                EQ_GLOBAL(int, 0x006B63FC)
#define g_screenHeight_guess                EQ_GLOBAL(int, 0x006B6400)

/* class EQ_DragItem (layout + these methods): generated in trilogy/layout/eq_dragitem.h. */
#include "trilogy/layout/eq_dragitem.h"

#endif /* TRILOGY_EQ_DRAGITEM_H */
