/*
 * eq_bboard.h - EQBBoard, the bulletin-board object (Mac: Source/PC Source/EverQuest/EQBBoard.cpp).
 *
 * Method-only: layout unrecovered except the list links at +0 and +4; the constructor also writes
 * board data through +0x3B. All accesses stay raw offsets; do not infer or assert a total size.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_BBOARD_H
#define TRILOGY_EQ_BBOARD_H

#include "common.h"
#include "eqaddr.h"

#define g_bboard_head_guess EQ_GLOBAL(void *, 0x006E4D70)

class EQBBoard
{
public:
    EQBBoard(const void* record);                                       /* 0x0049062D */
    ~EQBBoard();                                                        /* 0x004906B1 */
};

#endif /* TRILOGY_EQ_BBOARD_H */
