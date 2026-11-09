/*
 * eq_image.h - EQ_Image, the client's image/overlay-rect object (Mac: Source/PC Source/EverQuest/EQ_Image.cpp,
 * direct low mapping for draw_guess; neighbors 00438458/004384CE inferred).
 *
 * Method-only declarations: the layout is unrecovered (a dword at +0, a RECT at +4, rect fields read at
 * +4/+8/+0xC/+0x10 by draw_guess), so all accesses stay raw offsets. RECT comes from eq_columns.h.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_IMAGE_H
#define TRILOGY_EQ_IMAGE_H

#include "common.h"
#include "eq_columns.h"

class EQ_Image_guess
{
public:
    EQ_Image_guess(RECT* source);                                     /* 0x00438458 */
    void draw_guess(u32 arg1, u32 arg2, u32 arg3, u32 arg4);          /* 0x00438475 */
    void draw_guess(u32 arg1, u32 arg2);                              /* 0x004384B2 */
    void UpdateImageRect_guess(RECT* source);                         /* 0x004384CE */
};

void DrawImage(u32 arg, int imageId); /* 0x00438423 */

#endif /* TRILOGY_EQ_IMAGE_H */
