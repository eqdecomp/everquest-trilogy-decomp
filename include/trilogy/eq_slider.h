/*
 * eq_slider.h - EQ_HSlider and EQ_HSlider2, the old-UI horizontal sliders
 * (Mac: Source/PC Source/EverQuest/EQ_Slider.cpp).
 *
 * Distinct EQ_Overlay-derived classes: ctors 0x00485DE2 / 0x004867FE store different vtables.
 * Activate 0x0048629A stores int* coins at +0x1D0; 0x00486CB6 stores a byte maximum there.
 * Keep their constructor EH shells separate. The base bytes remain padding in these data views.
 * Layout: partial; new rectangles come from the constructors' SetRect stores and region registration.
 * The thumb is 10 pixels wide and the track is 56 (0x38) pixels long. Total sizes not recovered: no
 * sizeof checks, do not instantiate or take sizeof().
 * C++ only (VC6); needs <windows.h> for RECT. Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_SLIDER_H
#define TRILOGY_EQ_SLIDER_H

#include <windows.h>
#include "common.h"

#define SLIDER_THUMB_WIDTH_GUESS    10
#define SLIDER_TRACK_LENGTH_GUESS   0x38

class EQ_HSlider
{
public:
    u8      unknown0x000[0x80];  /* 0x000  EQ_Overlay base */
    RECT    backgroundRect_guess; /* 0x080  ctor, registered region 0 */
    RECT    finishRect_guess;   /* 0x090  ctor, registered region 1 */
    u8      unknown0x0A0[0x08];  /* 0x0A0 */
    RECT    track_guess;         /* 0x0A8  left/right bound the thumb */
    u8      unknown0x0B8[0x08];  /* 0x0B8 */
    RECT    thumb_copy_guess;    /* 0x0C0  CopyRect target */
    RECT    region5Rect_guess;  /* 0x0D0  ctor, registered region 5 */
    u8      unknown0x0E0[0xE0];  /* 0x0E0 */
    RECT*   thumb_guess;         /* 0x1C0 */
    int     dragging_guess;      /* 0x1C4  MouseMove does nothing unless set */
    u8      unknown0x1C8[0x08];  /* 0x1C8 */
    int*    range_guess;         /* 0x1D0  value = offset * *range / 56 */
    u8      mode_guess;         /* 0x1D4  Activate's u8 mode argument */
    u8      unknown0x1D5[0x03];  /* 0x1D5 */
    int     value_guess;         /* 0x1D8 */

    /* 0x004866E1  adjusts the value by delta and positions the thumb. */
    void    AdjustSliderPosition_guess(short delta);
    /* 0x00486772  name: mac. `y` is unused. */
    void    MouseMove_guess(short x, short y);
};
C_ASSERT(offsetof(EQ_HSlider, track_guess) == 0xA8);
C_ASSERT(offsetof(EQ_HSlider, thumb_copy_guess) == 0xC0);
C_ASSERT(offsetof(EQ_HSlider, backgroundRect_guess) == 0x80);
C_ASSERT(offsetof(EQ_HSlider, finishRect_guess) == 0x90);
C_ASSERT(offsetof(EQ_HSlider, region5Rect_guess) == 0xD0);
C_ASSERT(offsetof(EQ_HSlider, thumb_guess) == 0x1C0);
C_ASSERT(offsetof(EQ_HSlider, range_guess) == 0x1D0);
C_ASSERT(offsetof(EQ_HSlider, value_guess) == 0x1D8);

class EQ_HSlider2
{
public:
    u8      unknown0x000[0x80];  /* 0x000  EQ_Overlay base */
    RECT    backgroundRect_guess; /* 0x080  ctor, registered region 0 */
    RECT    finishRect_guess;   /* 0x090  ctor, registered region 1 */
    u8      unknown0x0A0[0x08];  /* 0x0A0 */
    RECT    track_guess;         /* 0x0A8 */
    u8      unknown0x0B8[0x08];  /* 0x0B8 */
    RECT    thumb_copy_guess;    /* 0x0C0 */
    RECT    region5Rect_guess;  /* 0x0D0  ctor, registered region 5 */
    u8      unknown0x0E0[0xE0];  /* 0x0E0 */
    RECT*   thumb_guess;         /* 0x1C0 */
    int     dragging_guess;      /* 0x1C4 */
    u8      unknown0x1C8[0x06];  /* 0x1C8 */
    s16     pos_guess;           /* 0x1CE  0..max */
    u8      max_guess;           /* 0x1D0  AdjustSliderPosition divides by it */
    u8      unknown0x1D1[0x23];  /* 0x1D1 */
    s16     left_value_guess;    /* 0x1F4  = pos */
    s16     right_value_guess;   /* 0x1F6  = max - pos */

    /* 0x0048745D  name: takp (descriptive), "EQ_HSlider2_AdjustSliderPosition". */
    void    AdjustSliderPosition_guess(short delta);
    /* 0x00487523  name: mac. `y` is unused. */
    void    MouseMove_guess(short x, short y);
};
C_ASSERT(offsetof(EQ_HSlider2, backgroundRect_guess) == 0x80);
C_ASSERT(offsetof(EQ_HSlider2, finishRect_guess) == 0x90);
C_ASSERT(offsetof(EQ_HSlider2, region5Rect_guess) == 0xD0);
C_ASSERT(offsetof(EQ_HSlider2, thumb_guess) == 0x1C0);
C_ASSERT(offsetof(EQ_HSlider2, pos_guess) == 0x1CE);
C_ASSERT(offsetof(EQ_HSlider2, max_guess) == 0x1D0);
C_ASSERT(offsetof(EQ_HSlider2, left_value_guess) == 0x1F4);
C_ASSERT(offsetof(EQ_HSlider2, right_value_guess) == 0x1F6);

#endif /* TRILOGY_EQ_SLIDER_H */
