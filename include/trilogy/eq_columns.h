/*
 * eq_columns.h - EQ_Columns, the "Columns" falling-blocks game of the old UI
 * (Mac: Source/PC Source/EverQuest/EQ_Columns.cpp), and the overlay element class its Activate resizes.
 *
 * Layout: partial, from the rewritten functions only; names are guesses. The object is a window of the old UI
 * (its base class is not recovered: FindOverlayElementById lists elements through +0x70, a RECT lives at
 * +0x24), the game state starts at +0x80. The play field is 10 columns x 12 rows of 16-byte cells from
 * +0xA0C (cell(col, row) = +0xA0C + col * 0x10 + row * 0xA0), the high score table has 10 entries of 0x28
 * bytes at +0x6D0, the score being played sits right after it at +0x860 in the same layout.
 * Total size not recovered: no sizeof checks; never instantiate or take sizeof().
 *
 * Included by everquest.h for ExecuteCmd's Columns object.
 * C++ only (VC6); uses <windef.h> for RECT without importing Win32 function declarations.
 * Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_COLUMNS_H
#define TRILOGY_EQ_COLUMNS_H

/* windows.h normally supplies the architecture selection before windef.h/winnt.h.
 * Keep only the base types here: EQMisc declares QueryPerformanceCounter itself. */
#if defined(_M_IX86) && !defined(_X86_)
#define _X86_
#endif
#include <windef.h>
#include "common.h"
#include "eqaddr.h"

/* One field cell, 0x10 bytes. color -1 = empty. */
struct ColumnsCell_guess
{
    s32     color;          /* 0x0  block type (a key code such as 0x7E2..0x849), -1 = empty */
    s32     y_offset;       /* 0x4  pixel offset below the cell's row while the block falls (starts at -10) */
    s32     active;         /* 0x8  1 = part of a match / the falling block */
    s32     settled;        /* 0xC  0 while the cell still falls */
};
C_ASSERT(sizeof(ColumnsCell_guess) == 0x10);

/* A high score entry, 0x28 bytes (read from and written to chs.dat as a whole). */
struct ColumnsScore_guess
{
    u32     score;          /* 0x00 */
    s32     level;          /* 0x04 */
    char    name[32];       /* 0x08 */
};
C_ASSERT(sizeof(ColumnsScore_guess) == 0x28);

/* One cell of a match found around a cell: its address and its column / row. */
struct ColumnsMatch_guess
{
    ColumnsCell_guess* cell;    /* 0x0 */
    s32     col;            /* 0x4 */
    s32     row;            /* 0x8 */
};
C_ASSERT(sizeof(ColumnsMatch_guess) == 0xC);

/* An element of the overlay lists (EQ_Overlay_FindOverlayElementById returns one): its rectangle is at +4. */
class OverlayElement_guess
{
public:
    u8      unknown0x00[4];     /* 0x00 */
    RECT    rect_guess;         /* 0x04 */

    void    SetOverlayElementRect_guess(RECT* rect);                       /* 0x00419598 */
};

class EQ_Columns_guess
{
public:
    u8      unknown0x00[0x10];  /* 0x000 */
    s32     active_guess;       /* 0x010  set by Activate */
    u8      unknown0x14[0x10];  /* 0x014 */
    RECT    window_rect_guess;  /* 0x024  Activate: the play area's rectangle */
    u8      unknown0x34[0x4C];  /* 0x034 */
    s32     state_guess;        /* 0x080  copy of next_state at Restart; 1, 4, 5 clear next_state again */
    s32     next_state_guess;   /* 0x084  2 = playing */
    u8      unknown0x88[8];     /* 0x088 */
    RECT    play_rect_guess;    /* 0x090 */
    RECT    rect_a0_guess;      /* 0x0A0  overlay element 1 */
    RECT    rect_b0_guess;      /* 0x0B0  overlay element 2 */
    u8      unknown0xC0[0x610]; /* 0x0C0 */
    ColumnsScore_guess scores_guess[10];    /* 0x6D0 */
    ColumnsScore_guess current_guess;       /* 0x860  score, level, player name */
    u32     fall_time_guess;    /* 0x888  time the block falls next (CDisplay time + the fall delay) */
    u32     timer_88c_guess;    /* 0x88C  deadline for the game-over / special phase */
    u32     effect_end_guess;   /* 0x890  end time of the active special block's effect (counter for 0x82A) */
    u32     saved_time_guess;   /* 0x894  time left of the effect while paused */
    ColumnsMatch_guess matches_guess[22];   /* 0x898  cells of the match being counted */
    s32     block_types_guess[12];  /* 0x9A0  the key codes a new block can take */
    s32     game_state_guess;   /* 0x9D0  3 = new level, 4 = field full, 1 = ... */
    s32     special_guess;      /* 0x9D4  key code of the special block in effect, -1 = none */
    s32     move_guess;         /* 0x9D8  pending horizontal move (set by SetColumnState) */
    s32     fast_drop_guess;    /* 0x9DC  set by SetColumnState(1000) */
    s32     falling_cells_guess;/* 0x9E0  cells still marked active */
    s32     score_rank_guess;   /* 0x9E4  index of the new score in the table, -1 = none */
    s32     width_guess;        /* 0x9E8 */
    s32     height_guess;       /* 0x9EC */
    s32     paused_guess;       /* 0x9F0 */
    s32     cur_col_guess;      /* 0x9F4 */
    s32     cur_row_guess;      /* 0x9F8 */
    s32     cur_block_guess;    /* 0x9FC  -1 = none */
    s32     next_block_guess;   /* 0xA00 */
    s32     steps_guess;        /* 0xA04  blocks spawned in this level */
    s32     level_steps_guess;  /* 0xA08  blocks per level (capped at 400) */
    ColumnsCell_guess cells_guess[120];     /* 0xA0C  [col + row * 10] */

    /* Defined in EverQuest.cpp. */
    void    SetColumnState_guess(int state);                                /* 0x00433330 */

    void    Restart();                                                /* 0x00432264 */
    void    InitializeNextLevel_guess();                               /* 0x00432325 */
    void    Activate_guess();                                               /* 0x004323DB */
    int     GetGamePauseState_guess();                                      /* 0x0043257C */
    void    SetGamePauseState_guess(int paused);                            /* 0x00432583 */
    void    UpdateColumnBlockFallTimer_guess();                             /* 0x00432626 */
    int     GetNextBlock();                                           /* 0x00432663 */
    void    SpawnNewBlock_guess();                                          /* 0x004331E1 */
    void    MoveColumnBlockHorizontally_guess(int dir);                     /* 0x00433357 */
    int     TryMoveColumnCell_guess(int fromCol, int fromRow, int toCol, int toRow, int move); /* 0x004333D6 */
    void    ReadHighScores();                                         /* 0x004334AF */
    void    SaveHighScoresToFile_guess();                                   /* 0x00433518 */
    void    UpdateHighScoreTable_guess();                                   /* 0x0043355C */
    void    AddColumnsScore_guess(int n);                           /* 0x004335FE */
    int     IsSpecialKeyCode_guess(int code);                               /* 0x00433635 */
    void    ProcessKeyCodeAndSetTimer_guess(int code);                      /* 0x00433661 */
    void    MarkColumnCellsAsActive_guess(int col, int count);              /* 0x00433726 */
    void    ResetColumnCell_guess(ColumnsCell_guess* cell);                 /* 0x00433764 */
    void    ProcessMatchedColumns_guess(int count);                         /* 0x00433779 */
    void    CheckAndProcessMatchingCells_guess(int col, int row);           /* 0x00433819 */
    void    UpdateColumnCellPosition_guess(int col, int row, int isBlock);  /* 0x00433C54 */
    void    Update_guess();                                                 /* 0x00433E31 */
    void draw_guess(void);    /* 0x004326A6 */
};
C_ASSERT(offsetof(EQ_Columns_guess, window_rect_guess) == 0x24);
C_ASSERT(offsetof(EQ_Columns_guess, state_guess) == 0x80);
C_ASSERT(offsetof(EQ_Columns_guess, play_rect_guess) == 0x90);
C_ASSERT(offsetof(EQ_Columns_guess, scores_guess) == 0x6D0);
C_ASSERT(offsetof(EQ_Columns_guess, current_guess) == 0x860);
C_ASSERT(offsetof(EQ_Columns_guess, fall_time_guess) == 0x888);
C_ASSERT(offsetof(EQ_Columns_guess, matches_guess) == 0x898);
C_ASSERT(offsetof(EQ_Columns_guess, block_types_guess) == 0x9A0);
C_ASSERT(offsetof(EQ_Columns_guess, game_state_guess) == 0x9D0);
C_ASSERT(offsetof(EQ_Columns_guess, cur_col_guess) == 0x9F4);
C_ASSERT(offsetof(EQ_Columns_guess, cells_guess) == 0xA0C);

/* 0x0045014C  old UI overlay lookup, __thiscall with one stack slot (only the low 16 ID bits are read).
 * The base class is not recovered; the current callers are EQ_Columns windows. */
#define EQ_Overlay_FindOverlayElementById_guess(w, id) \
    EQ_METHOD(1, OverlayElement_guess *, 0x0045014C, (EQ_Columns_guess *, int), (w))((id))
/* 0x00433495  clears the ten score/level/name entries, __thiscall, no stack arguments.
 * The Mac-chain des_set_odd_parity name is wrong for this binary. */
#define EQ_Columns_SetDefaultScores_guess(w) \
    EQ_METHOD(0, void, 0x00433495, (EQ_Columns_guess *), (w))()

#endif /* TRILOGY_EQ_COLUMNS_H */
