/*
 * eq_textio.h - partial old-UI text input type used by the EQ_TextIO rewrites.
 * Layout: generated from a layout JSON (not published; 0x208 bytes, an EQ_Overlay subclass; gen_layout_h --extend).
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_TEXTIO_H
#define TRILOGY_EQ_TEXTIO_H

#include "layout/eq_overlay.h"   /* EQ_Overlay, EQ_Rect_guess; no <windef.h> */

class EQ_TextIO_guess : public EQ_Overlay
{
public:
    short   historyTextX_guess; /* 0x080* */
    short   historyTextY_guess; /* 0x082* */
    short   inputTextX_guess;   /* 0x084* */
    short   inputTextY_guess;   /* 0x086* */
    struct tagRECT* scrollThumbRect_guess; /* 0x088* */
    EQ_Rect_guess scrollTrackRect_guess; /* 0x08C* */
    EQ_Rect_guess textModeButtonRect_guess; /* 0x09C* */
    EQ_Rect_guess control0Rect; /* 0x0AC* */
    EQ_Rect_guess control1Rect; /* 0x0BC* */
    EQ_Rect_guess control2Rect; /* 0x0CC* */
    EQ_Rect_guess control3Rect; /* 0x0DC* */
    EQ_Rect_guess control4Rect; /* 0x0EC* */
    EQ_Rect_guess control5Rect_guess; /* 0x0FC* */
    EQ_Rect_guess control6Rect_guess; /* 0x10C* */
    EQ_Rect_guess unk_0x11C;          /* 0x11C* */
    u8      unknown0x12C[0x20]; /* 0x12C  */
    EQ_Rect_guess chatHitRect_guess;  /* 0x14C* */
    EQ_Rect_guess windowFrameRect_guess; /* 0x15C* */
    int     inputCursor;  /* 0x16C* */
    unsigned int inputViewEnd_guess; /* 0x170* */
    short   scrollThumbHeight_guess; /* 0x174* */
    short   scrollTopLimit; /* 0x176* */
    short   scrollBottomLimit; /* 0x178* */
    short   scrollTravel; /* 0x17A* */
    union
    {
        short   languageIds_guess[0x20]; /* 0x17C* */
        unsigned int languageIdsDwords_guess[0x10]; /* 0x17C* */
    };
    u8      unknown0x1BC[0x24]; /* 0x1BC  */
    short   textDisplayMode_guess; /* 0x1E0* */
    short   pressedControlId_guess; /* 0x1E2* */
    unsigned short unk_0x1E4;          /* 0x1E4* */
    unsigned short unk_0x1E6;          /* 0x1E6* */
    unsigned short unk_0x1E8;          /* 0x1E8* */
    u8      unknown0x1EA[2];    /* 0x1EA  */
    int     redrawPass_guess;   /* 0x1EC* */
    char    firstLayoutPending_guess; /* 0x1F0* */
    char    channelPickerOpen; /* 0x1F1* */
    short   languageCount_guess; /* 0x1F2* */
    u8      unknown0x1F4[4];    /* 0x1F4  */
    int     mouseDispatchActive_guess; /* 0x1F8* */
    u8      unknown0x1FC[4];    /* 0x1FC  */
    int     scrollThumbDragging_guess; /* 0x200* */
    int     unk_0x204;          /* 0x204* */

    void ResetTextIO_guess(void);                     /* 0x0048B1D1 */
    void up_display(void);                         /* 0x0048CA78 */
    void ResetTextBufferAndUpdateDisplay_guess(void);    /* 0x0048CAF2 */
    void ResetChatBufferToTop_guess(void);               /* 0x0048CB40 */
    void ScrollTextBufferDown_guess(void);               /* 0x0048CBDB */
    void scrollTextBufferUp_guess(void);                 /* 0x0048CC53 */
};
C_ASSERT(sizeof(EQ_TextIO_guess) == 0x208);

/* 0x006E4DE8  the chat window (globals.h keeps the byte-pointer g_chatWindow_guess for its arithmetic users). */
#define pinstEQ_TextIO_guess        EQ_GLOBAL(EQ_TextIO_guess *, 0x006E4DE8)
C_ASSERT(offsetof(EQ_TextIO_guess, historyTextX_guess) == 0x080);
C_ASSERT(offsetof(EQ_TextIO_guess, historyTextY_guess) == 0x082);
C_ASSERT(offsetof(EQ_TextIO_guess, inputTextX_guess) == 0x084);
C_ASSERT(offsetof(EQ_TextIO_guess, inputTextY_guess) == 0x086);
C_ASSERT(offsetof(EQ_TextIO_guess, scrollThumbRect_guess) == 0x088);
C_ASSERT(offsetof(EQ_TextIO_guess, scrollTrackRect_guess) == 0x08C);
C_ASSERT(offsetof(EQ_TextIO_guess, textModeButtonRect_guess) == 0x09C);
C_ASSERT(offsetof(EQ_TextIO_guess, control0Rect) == 0x0AC);
C_ASSERT(offsetof(EQ_TextIO_guess, control1Rect) == 0x0BC);
C_ASSERT(offsetof(EQ_TextIO_guess, control2Rect) == 0x0CC);
C_ASSERT(offsetof(EQ_TextIO_guess, control3Rect) == 0x0DC);
C_ASSERT(offsetof(EQ_TextIO_guess, control4Rect) == 0x0EC);
C_ASSERT(offsetof(EQ_TextIO_guess, control5Rect_guess) == 0x0FC);
C_ASSERT(offsetof(EQ_TextIO_guess, control6Rect_guess) == 0x10C);
C_ASSERT(offsetof(EQ_TextIO_guess, unk_0x11C) == 0x11C);
C_ASSERT(offsetof(EQ_TextIO_guess, chatHitRect_guess) == 0x14C);
C_ASSERT(offsetof(EQ_TextIO_guess, windowFrameRect_guess) == 0x15C);
C_ASSERT(offsetof(EQ_TextIO_guess, inputCursor) == 0x16C);
C_ASSERT(offsetof(EQ_TextIO_guess, inputViewEnd_guess) == 0x170);
C_ASSERT(offsetof(EQ_TextIO_guess, scrollThumbHeight_guess) == 0x174);
C_ASSERT(offsetof(EQ_TextIO_guess, scrollTopLimit) == 0x176);
C_ASSERT(offsetof(EQ_TextIO_guess, scrollBottomLimit) == 0x178);
C_ASSERT(offsetof(EQ_TextIO_guess, scrollTravel) == 0x17A);
C_ASSERT(offsetof(EQ_TextIO_guess, languageIds_guess) == 0x17C);
C_ASSERT(offsetof(EQ_TextIO_guess, textDisplayMode_guess) == 0x1E0);
C_ASSERT(offsetof(EQ_TextIO_guess, pressedControlId_guess) == 0x1E2);
C_ASSERT(offsetof(EQ_TextIO_guess, unk_0x1E4) == 0x1E4);
C_ASSERT(offsetof(EQ_TextIO_guess, unk_0x1E6) == 0x1E6);
C_ASSERT(offsetof(EQ_TextIO_guess, unk_0x1E8) == 0x1E8);
C_ASSERT(offsetof(EQ_TextIO_guess, redrawPass_guess) == 0x1EC);
C_ASSERT(offsetof(EQ_TextIO_guess, firstLayoutPending_guess) == 0x1F0);
C_ASSERT(offsetof(EQ_TextIO_guess, channelPickerOpen) == 0x1F1);
C_ASSERT(offsetof(EQ_TextIO_guess, languageCount_guess) == 0x1F2);
C_ASSERT(offsetof(EQ_TextIO_guess, mouseDispatchActive_guess) == 0x1F8);
C_ASSERT(offsetof(EQ_TextIO_guess, scrollThumbDragging_guess) == 0x200);
C_ASSERT(offsetof(EQ_TextIO_guess, unk_0x204) == 0x204);

#endif /* TRILOGY_EQ_TEXTIO_H */
