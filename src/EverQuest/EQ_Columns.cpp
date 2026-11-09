// EQ_Columns.cpp - EQ_Columns, the "Columns" falling-blocks game of the old UI (hidden in the client).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Columns.cpp (from Mac debug map)
// The object lives at 0x006E4DF8 (CEverQuest_ExecuteCmd feeds it through SetColumnState, which stays in
// EverQuest.cpp at 0x00433330 where the queue first put it). Field layout: include/trilogy/eq_columns.h.

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "trilogy/common.h"
#include "trilogy/player.h"
#include "trilogy/display.h"
#include "trilogy/globals.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_columns.h"

#pragma comment(lib, "user32.lib")     // CopyRect, SetRect

// FUNCTION: TRILOGY 0x00419598
// ORIGINAL FILE: unknown (the TAKP name is descriptive; the address is far from the rest of EQ_Columns)
// __thiscall. Copies the rectangle into the element (at +4).
void OverlayElement_guess::SetOverlayElementRect_guess(RECT* rect)
{
    CopyRect(&rect_guess, rect);
}

// FUNCTION: TRILOGY 0x00432264
// Starts a new game: clears the field and the state, picks the next block and begins level 0.
void EQ_Columns_guess::Restart()
{
    int col;
    int row;

    effect_end_guess = 0;
    state_guess = next_state_guess;
    score_rank_guess = -1;
    special_guess = -1;
    current_guess.score = 0;
    current_guess.level = -1;
    falling_cells_guess = 0;
    cur_col_guess = 0;
    cur_row_guess = 0;
    cur_block_guess = -1;
    next_block_guess = GetNextBlock();
    for (col = 0; col < 10; col++)
    {
        for (row = 0; row < 12; row++)
            ResetColumnCell_guess(&cells_guess[col + row * 10]);
    }
    InitializeNextLevel_guess();
    SetGamePauseState_guess(0);
    if (state_guess == 1 || state_guess == 4 || state_guess == 5)
        next_state_guess = 0;
}

// FUNCTION: TRILOGY 0x00432325
// Next level: scores the previous one, sets the block count and the fall speed.
void EQ_Columns_guess::InitializeNextLevel_guess()
{
    if (current_guess.level > 0)
        AddColumnsScore_guess(2);
    current_guess.level++;
    move_guess = 0;
    steps_guess = 0;
    level_steps_guess = current_guess.level * 2 + 0x32;
    if (level_steps_guess > 400)
        level_steps_guess = 400;
    next_state_guess = 2;
    game_state_guess = 3;
    UpdateColumnBlockFallTimer_guess();
    timer_88c_guess = g_cdisplay_guess->time_ms_guess + 2000;
    if (special_guess == 0x82A && effect_end_guess < 4)
    {
        effect_end_guess++;
        timer_88c_guess = g_cdisplay_guess->time_ms_guess + 0xFA;
        return;
    }
    special_guess = -1;
}

// FUNCTION: TRILOGY 0x004323db
// ORIGINAL FILE: EverQuest/EQ_Columns.cpp (from Mac debug map)
// Fills the block table and lays out the play field and the two overlay elements from the background's size.
void EQ_Columns_guess::Activate_guess()
{
    OverlayElement_guess* element;
    typedef BOOL (WINAPI *SetRectProc_guess)(LPRECT, int, int, int, int);
    SetRectProc_guess set_rect;

    active_guess = 1;
    block_types_guess[0] = 0x801;
    block_types_guess[1] = 0x806;
    block_types_guess[2] = 0x849;
    block_types_guess[3] = 0x803;
    block_types_guess[4] = 0x80B;
    block_types_guess[5] = 0x7E6;
    block_types_guess[6] = 0x7E2;
    block_types_guess[7] = 0x824;
    block_types_guess[8] = 0x843;
    block_types_guess[9] = 0x7E3;
    block_types_guess[10] = 0x7E8;
    block_types_guess[11] = 0x82A;
    if (g_columns_bg_guess != 0)
    {
        width_guess = *(int*)(g_columns_bg_guess + 0xC) - *(int*)(g_columns_bg_guess + 4);
        height_guess = *(int*)(g_columns_bg_guess + 0x10) - *(int*)(g_columns_bg_guess + 8);
        element = EQ_Overlay_FindOverlayElementById_guess(this, 0);
        set_rect = EQ_GLOBAL(SetRectProc_guess, 0x00534248);
        if (element != 0)
        {
            set_rect(&play_rect_guess, 0x7D, 10, width_guess * 10 + 0x87, height_guess * 12 + 0x23);
            EQ_METHOD(1, void, 0x00419598, (OverlayElement_guess *, RECT *), element)(&play_rect_guess);
        }
        element = EQ_Overlay_FindOverlayElementById_guess(this, 2);
        if (element != 0)
        {
            set_rect(&rect_b0_guess, play_rect_guess.right, play_rect_guess.top, play_rect_guess.right + 0x38,
                     play_rect_guess.top + 0x18);
            EQ_METHOD(1, void, 0x00419598, (OverlayElement_guess *, RECT *), element)(&rect_b0_guess);
        }
        element = EQ_Overlay_FindOverlayElementById_guess(this, 1);
        if (element != 0)
        {
            set_rect(&rect_a0_guess, play_rect_guess.right, play_rect_guess.bottom - 0x18,
                     play_rect_guess.right + 0x38, play_rect_guess.bottom);
            EQ_METHOD(1, void, 0x00419598, (OverlayElement_guess *, RECT *), element)(&rect_a0_guess);
        }
        set_rect(&window_rect_guess, play_rect_guess.left, play_rect_guess.top, rect_a0_guess.right,
                 rect_a0_guess.bottom);
    }
    else
    {
        width_guess = 0x28;
        height_guess = 0x28;
    }
}

// FUNCTION: TRILOGY 0x0043257c
// Returns the Columns game's pause state.
// ORIGINAL FILE: unknown (name ours)
int EQ_Columns_guess::GetGamePauseState_guess()
{
    return paused_guess;
}

// FUNCTION: TRILOGY 0x00432583
// Pause (nonzero) or resume: while paused the time left of the special block's effect is kept in saved_time.
void EQ_Columns_guess::SetGamePauseState_guess(int paused)
{
    paused_guess = paused;
    if (paused == 0)
    {
        UpdateColumnBlockFallTimer_guess();
        timer_88c_guess = g_cdisplay_guess->time_ms_guess + 2000;
        if (special_guess != -1 && special_guess != 0x82A)
            effect_end_guess = saved_time_guess + g_cdisplay_guess->time_ms_guess;
    }
    else if (special_guess != -1 && special_guess != 0x82A)
    {
        saved_time_guess = effect_end_guess - g_cdisplay_guess->time_ms_guess;
        if (saved_time_guess > 24000)
            saved_time_guess = 12000;
    }
    move_guess = 0;
    fast_drop_guess = 0;
}

// FUNCTION: TRILOGY 0x00432626
// The block falls every 45 - 2 * level ms (at least 2).
void EQ_Columns_guess::UpdateColumnBlockFallTimer_guess()
{
    int delay = 0x2D - current_guess.level * 2;

    if (delay < 2)
        fall_time_guess = g_cdisplay_guess->time_ms_guess + 2;
    else
        fall_time_guess = g_cdisplay_guess->time_ms_guess + delay;
}

// FUNCTION: TRILOGY 0x00432663
// Chooses the next block at random, or repeats 0x824/0x7E3 while either is active.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Columns.cpp (Mac: GetNextBlock, medium)
// The next block type: 25% of the time one of the first six, else one of the last six. While the special
// block 0x824 or 0x7E3 is in effect the current block repeats instead.
int EQ_Columns_guess::GetNextBlock()
{
    int idx;

    if (special_guess != 0x824 && special_guess != 0x7E3)
    {
        if (number_percent_guess() < 0x1A)
            idx = random_range_guess(0, 5);
        else
            idx = random_range_guess(6, 11);
        return block_types_guess[idx];
    }
    cur_block_guess = special_guess;
    return special_guess;
}

// FUNCTION: TRILOGY 0x004326a6
// Draws the EQ Gems window each frame: help text, the field with wave/score/next/special, or high scores.
// ORIGINAL FILE: EverQuest/EQ_Columns.cpp (from Mac debug map)
// @early-stop: 0.9479 similarity; the fastcall dummy-EDX shim, cdecl cleanup scheduling, equivalent LEA operand order, and instruction-count shifts remain.
/* fake match: reuse columnCells for the high-score index to match the original local lifetime and stack slot. */
void EQ_Columns_guess::draw_guess(void)
{
    int state;
    int cellColor;
    int color;
    int col;
    int row;
    int x;
    int y;
    int textY;
    int *columnCells;
    int *cell;
    int *score;
    char text[256];
    RECT rect;

    if (this->active_guess != 0)
    {
        state = this->next_state_guess;
        if (state == 1)
        {
            EQ_METHOD(0, void, 0x00433E31, (void*), (void*)this)();
            columnCells = (int*)((char*)this + 0xA10);
            col = 0;
            do
            {
                row = 0;
                cell = columnCells;
                do
                {
                    cellColor = cell[-1];
                    if (cellColor >= 0)
                    {
                        x = this->width_guess * col + 6 + this->play_rect_guess.left;
                        y = this->height_guess * row + this->play_rect_guess.top + 0x15 + *cell;
                        ((BOOL (__stdcall *)(LPRECT, int, int, int, int))EQ_GLOBAL(u32, 0x00534248))
                            (&rect, x, y, 0, 0);
                        EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), EQ_ARRAY(void*, 0x006B32B4)[cellColor])(&rect, 1);
                    }
                    ++row;
                    cell += 0x28;
                } while (row < 0xC);
                ++col;
                columnCells += 4;
            } while (col < 10);

            EQ_METHOD(0, void, 0x00433129, (void*), (void*)this)();
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (text, EQ_ARRAY(char, 0x0055889C), this->current_guess.level,
                 this->current_guess.score);
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 2, 10, EQ_GLOBAL(int, 0x006EFE00), 1);

            if (this->special_guess != 0x843)
            {
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(EQ_ARRAY(char, 0x00558894),
                     this->play_rect_guess.right + 5, this->rect_b0_guess.bottom + 5,
                     10, EQ_GLOBAL(int, 0x006EFE00), 1);
                ((BOOL (__stdcall *)(LPRECT, int, int, int, int))EQ_GLOBAL(u32, 0x00534248))
                    (&rect, this->play_rect_guess.right + 5,
                     this->rect_b0_guess.bottom + 0x13, 0, 0);
                EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), EQ_ARRAY(void*, 0x006B32B4)[this->next_block_guess])(&rect, 1);
            }
            if (this->special_guess != -1)
            {
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(EQ_ARRAY(char, 0x0055888C),
                     this->play_rect_guess.right + 5,
                     this->rect_a0_guess.top - this->height_guess - 0x13,
                     10, EQ_GLOBAL(int, 0x006EFE00), 1);
                ((BOOL (__stdcall *)(LPRECT, int, int, int, int))EQ_GLOBAL(u32, 0x00534248))
                    (&rect, this->play_rect_guess.right + 5,
                     this->rect_a0_guess.top - this->height_guess - 5, 0, 0);
                EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), EQ_ARRAY(void*, 0x006B32B4)[this->special_guess])(&rect, 1);
            }
            if (this->paused_guess != 0)
            {
                EQ_METHOD(4, void, 0x0046D4DA, (void*, short, short, const char*, int), EQ_GLOBAL(void*, 0x006E4E44))(
                     (short)(this->play_rect_guess.left + 0x8C),
                     (short)(this->play_rect_guess.top + 0x8C),
                     EQ_ARRAY(char, 0x00558884), 0x0F);
            }
        }
        else if (state == 0)
        {
            EQ_METHOD(0, void, 0x00433129, (EQ_Columns_guess*), this)();
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x00558870));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 10, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x0055883C));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x19, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x00558810));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x28, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x005587E4));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x37, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x005587B4));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x46, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (text, EQ_ARRAY(char, 0x0055877C), 4);
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x69, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (text, EQ_ARRAY(char, 0x00558748), 3);
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x78, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x00558734));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x87, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x00558700));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0x96, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (text, EQ_ARRAY(char, 0x005586E0));
            EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, this->play_rect_guess.left + 5,
                 this->play_rect_guess.top + 0xB4, 10, EQ_GLOBAL(int, 0x006EFE04), 1);
        }
        else
        {
            if (state == 3)
            {
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (text, EQ_ARRAY(char, 0x005586D8), this->current_guess.level);
                color = EQ_CDECL(int, 0x004A46F6, (int, int))(2, 0x12);
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                     this->play_rect_guess.left + 0x82,
                     this->play_rect_guess.top + 0x82, color,
                     EQ_GLOBAL(int, 0x006EFE08), 1);
                EQ_METHOD(0, void, 0x00433E31, (void*), (void*)this)();
            }
            else if (state == 4)
            {
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (text, EQ_ARRAY(char, 0x005586CC));
                color = EQ_CDECL(int, 0x004A46F6, (int, int))(10, 0x0D);
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                     this->play_rect_guess.left + 0x73,
                     this->play_rect_guess.top + 0x82, color,
                     EQ_GLOBAL(int, 0x006EFE08), 1);
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (text, EQ_ARRAY(char, 0x0055889C), this->current_guess.level,
                     this->current_guess.score);
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                     this->play_rect_guess.left + 5,
                     this->play_rect_guess.top + 2, 10,
                     EQ_GLOBAL(int, 0x006EFE00), 1);
                if (this->score_rank_guess != -1)
                {
                    this->next_state_guess = 2;
                    this->game_state_guess = 5;
                    this->timer_88c_guess =
                        *(int*)((char*)g_display_guess + 0xBC) + 2000;
                }
            }
            else if (state == 5)
            {
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (text, EQ_ARRAY(char, 0x005586C0));
                color = EQ_CDECL(int, 0x004A46F6, (int, int))(10, 0x0D);
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                     this->play_rect_guess.left + EQ_GLOBAL(int, 0x00591EFC) + 2,
                     this->play_rect_guess.top + 2, color,
                     EQ_GLOBAL(int, 0x006EFE00), 1);
                ++EQ_GLOBAL(int, 0x00591EFC);
                if (this->play_rect_guess.left + EQ_GLOBAL(int, 0x00591EFC) + 2 >
                    this->play_rect_guess.right - 0x5A)
                    EQ_GLOBAL(int, 0x00591EFC) = 0;

                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (text, EQ_ARRAY(char, 0x005586B8));
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                     this->play_rect_guess.right - 0x8A,
                     this->play_rect_guess.top + 0x16, 10,
                     EQ_GLOBAL(int, 0x006EFE00), 1);
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (text, EQ_ARRAY(char, 0x005586B0));
                EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                     this->play_rect_guess.right - 0x30,
                     this->play_rect_guess.top + 0x16, 10,
                     EQ_GLOBAL(int, 0x006EFE00), 1);

                columnCells = (int*)0;
                textY = 0x28;
                score = (int*)((char*)this + 0x6D0);
                do
                {
                    if (columnCells == (int*)this->score_rank_guess)
                        color = EQ_CDECL(int, 0x004A46F6, (int, int))(2, 0x12);
                    else
                        color = 10;
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                        (text, EQ_ARRAY(char, 0x0053E678), (char*)(score + 2));
                    EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                         this->play_rect_guess.left + 2,
                         textY + this->play_rect_guess.top, color,
                         EQ_GLOBAL(int, 0x006EFE00), 1);
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                        (text, EQ_ARRAY(char, 0x0053E16C), score[0]);
                    EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                         this->play_rect_guess.right - 0x8A,
                         textY + this->play_rect_guess.top, color,
                         EQ_GLOBAL(int, 0x006EFE00), 1);
                    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                        (text, EQ_ARRAY(char, 0x0053E16C), score[1]);
                    EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                         this->play_rect_guess.right - 0x30,
                         textY + this->play_rect_guess.top, color,
                         EQ_GLOBAL(int, 0x006EFE00), 1);
                    textY += 0x14;
                    columnCells = (int*)((int)columnCells + 1);
                    score += 10;
                } while (textY < 0xF0);
            }
            EQ_METHOD(0, void, 0x00433129, (void*), (void*)this)();
        }

        if (this->next_state_guess == 2)
        {
            state = this->game_state_guess;
            this->next_state_guess = state;
            if (state == -1)
                this->next_state_guess = 1;
            else if (state == 3)
                this->fall_time_guess =
                    *(int*)((char*)g_display_guess + 0xBC) + 2000;
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00432f3e
// Routes accepted Columns commands to pause, interface, or restart actions.
void __fastcall HandleCommand_guess(EQ_Columns_guess* self, int unusedEdx, short inputGate, short command)
{
    class EQColumnsVtable_guess
    {
    public:
        virtual void unused0(void) {}
        virtual void unused1(void) {}
        virtual void invoke(void) {}
    };

    if (self->active_guess != 0)
    {
        if (inputGate == 0)
        {
            if (command == 0)
            {
                if (self->next_state_guess == 1)
                    EQ_METHOD(1, void, 0x00432583, (EQ_Columns_guess*, int), self)(1 - self->paused_guess);
            }
            else if (command == 1)
            {
                ((EQColumnsVtable_guess*)self)->invoke();
            }
            else if (command == 2)
            {
                EQ_METHOD(0, void, 0x00432264, (EQ_Columns_guess*), self)();
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00432f84
// Renders a single frame element for the Columns overlay.
// ORIGINAL FILE: EverQuest/EQ_Columns.cpp (from Mac debug map)
void __fastcall EQ_Columns_drawFrame_guess(EQ_Columns_guess* self, int edx, int* pos)
{
    BOOL (__stdcall *setRect)(LPRECT, int, int, int, int);
    int bottom;
    int top;
    int xRight;
    RECT overlayRect;
    RECT clipRect;
    int height;

    setRect = (BOOL (__stdcall *)(LPRECT, int, int, int, int))EQ_GLOBAL(u32, 0x00534248);
    bottom = pos[3];
    top = pos[1];
    xRight = (pos[2] - *pos) + 1;
    height = bottom - top;
    setRect(&clipRect, 0, 0, xRight, 1);
    if ((EQ_GLOBAL(s32, 0x0058FC6C) == 2) && (EQ_GLOBAL(u16, 0x006B63E4) != 0x280))
    {
        EQ_METHOD(3, void, 0x004500BE, (EQ_Columns_guess*, void*, int, int*), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, pos);
    }
    else
    {
        EQ_METHOD(4, void, 0x00450125, (EQ_Columns_guess*, void*, int, int*, int), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, pos, 1);
    }
    setRect(&clipRect, 0x27f, 0, 0x280, bottom - top);
    if ((EQ_GLOBAL(s32, 0x0058FC6C) == 2) && (EQ_GLOBAL(u16, 0x006B63E4) != 0x280))
    {
        EQ_METHOD(3, void, 0x004500BE, (EQ_Columns_guess*, void*, int, int*), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, pos);
    }
    else
    {
        EQ_METHOD(4, void, 0x00450125, (EQ_Columns_guess*, void*, int, int*, int), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, pos, 1);
    }
    setRect(&clipRect, 0, 0, xRight, 1);
    setRect(&overlayRect, *pos, pos[3], 2000, 2000);
    if ((EQ_GLOBAL(s32, 0x0058FC6C) == 2) && (EQ_GLOBAL(u16, 0x006B63E4) != 0x280))
    {
        EQ_METHOD(3, void, 0x004500BE, (EQ_Columns_guess*, void*, int, int*), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, (int*)&overlayRect);
    }
    else
    {
        EQ_METHOD(4, void, 0x00450125, (EQ_Columns_guess*, void*, int, int*, int), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, (int*)&overlayRect, 1);
    }
    setRect(&clipRect, 0x27f, 0, 0x280, height);
    setRect(&overlayRect, pos[2], pos[1], 2000, 2000);
    if ((EQ_GLOBAL(s32, 0x0058FC6C) == 2) && (EQ_GLOBAL(u16, 0x006B63E4) != 0x280))
    {
        EQ_METHOD(3, void, 0x004500BE, (EQ_Columns_guess*, void*, int, int*), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, (int*)&overlayRect);
    }
    else
    {
        EQ_METHOD(4, void, 0x00450125, (EQ_Columns_guess*, void*, int, int*, int), self)((void*)*(u32*)(EQ_GLOBAL(u32, 0x006E4E04) + 4), (int)&clipRect, (int*)&overlayRect, 1);
    }
    return;
}


// FUNCTION: TRILOGY 0x00433129
// Draws the Columns menu frames and labels them Done, Start, or Restart.
// ORIGINAL FILE: EverQuest/EQ_Columns.cpp (from Mac debug map)
// Walks the list at +0x70 (next at +0x20) when the flag at +0x88 is set. Each node's frame is drawn; types 1 and 2
// also get a label: type 1 is "Done" at +0xA0/+0xA4, type 2 is "Start" (or "Restart" when +0x84 is set) at +0xB0/+0xB4, each plus 5.
/* fake match: WriteTextHD is called in each switch arm, because VC6 cross-jumps the three identical call tails into the one call the original has. */
void __fastcall EQ_Columns_drawBorders_guess(EQ_Columns_guess* self, int edx)
{
    int node;

    if (*(int*)((char*)self + 0x88) != 0)
    {
        for (node = *(int*)((char*)self + 0x70); node != 0; node = *(int*)(node + 0x20))
        {
            EQ_METHOD(1, void, 0x00432F84, (EQ_Columns_guess*, int*), self)((int*)((*(short*)(node + 0x14) + 9) * 0x10 + (int)self));
            switch (*(short*)(node + 0x14))
            {
            case 1:
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))((char*)0x005588b4,
                     *(int*)((char*)self + 0xa0) + 5, *(int*)((char*)self + 0xa4) + 5, 10,
                     EQ_GLOBAL(u32, 0x006EFDFC), 1);
                break;
            case 2:
                if (*(int*)((char*)self + 0x84) == 0)
                {
                    EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))((char*)0x005588c4,
                         *(int*)((char*)self + 0xb0) + 5, *(int*)((char*)self + 0xb4) + 5, 10,
                         EQ_GLOBAL(u32, 0x006EFDFC), 1);
                }
                else
                {
                    EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))((char*)0x005588bc,
                         *(int*)((char*)self + 0xb0) + 5, *(int*)((char*)self + 0xb4) + 5, 10,
                         EQ_GLOBAL(u32, 0x006EFDFC), 1);
                }
                break;
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004331e1
// Spawns the queued block in a random column, ending the game if both sampled spots are occupied.
void EQ_Columns_guess::SpawnNewBlock_guess()
{
    int column;
    int nextBlock;

    steps_guess++;
    column = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 9);
    cur_row_guess = 0;
    cur_col_guess = column;
    if (cells_guess[column].color != -1)
    {
        column = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 9);
        cur_col_guess = column;
        if (cells_guess[column + cur_row_guess * 10].color != -1)
        {
            next_state_guess = 2;
            game_state_guess = 4;
            UpdateHighScoreTable_guess();
            return;
        }
    }
    cur_block_guess = next_block_guess;
    nextBlock = GetNextBlock();
    next_block_guess = nextBlock;
    cells_guess[cur_col_guess + cur_row_guess * 10].color = cur_block_guess;
    cells_guess[cur_col_guess + cur_row_guess * 10].active = 1;
    cells_guess[cur_col_guess + cur_row_guess * 10].settled = 0;
    cells_guess[cur_col_guess + cur_row_guess * 10].y_offset = -10;
    if (special_guess == 0x7e3 && cur_block_guess == 0x7e3)
    {
        CDisplay_PlaySoundA_guess(g_local_player_guess->y,
             g_local_player_guess->x, g_local_player_guess->z, 0x6b);
    }
    return;
}

// FUNCTION: TRILOGY 0x00433357
// Moves the falling block one column (the special blocks 0x80B and 0x849 mirror or randomise the direction).
void EQ_Columns_guess::MoveColumnBlockHorizontally_guess(int dir)
{
    int newCol;

    if (special_guess != 0x7E3 && (dir != 0 || special_guess == 0x849))
    {
        if (special_guess == 0x80B)
        {
            dir = -dir;
        }
        else if (special_guess == 0x849)
        {
            int r = random_range_guess(0, 4);
            if (r < 0 || r > 1)
                dir = 1;
            else
                dir = -1;
        }
        newCol = cur_col_guess + dir;
        if (TryMoveColumnCell_guess(cur_col_guess, cur_row_guess, newCol, cur_row_guess, 1))
            cur_col_guess = newCol;
        move_guess = 0;
    }
}

// FUNCTION: TRILOGY 0x004333d6
// Tests or moves a block cell into an in-bounds empty or settled grid cell.
// __thiscall, 5 stack arguments. Whether the target cell is on the field and empty (or settled); with `move`
// set the cell at (fromCol, fromRow) is moved there.
int EQ_Columns_guess::TryMoveColumnCell_guess(int fromCol, int fromRow, int toCol, int toRow, int move)
{
    if (toCol >= 0 && toRow >= 0 && toCol < 10 && toRow < 12)
    {
        toCol = toCol + toRow * 10;
        if (cells_guess[toCol].color <= -1 || cells_guess[toCol].settled != 0)
        {
            if (move != 0)
            {
                int fromIndex = fromCol + fromRow * 10;
                cells_guess[toCol].color = cells_guess[fromIndex].color;
                cells_guess[toCol].active = cells_guess[fromIndex].active;
                cells_guess[toCol].settled = cells_guess[fromIndex].settled;
                cells_guess[toCol].y_offset = cells_guess[fromIndex].y_offset;
                ResetColumnCell_guess(&cells_guess[fromIndex]);
            }
            return 1;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004334af
// Loads the Columns rankings from chs.dat, applying defaults and 'solo' names where needed.
// ORIGINAL FILE: EverQuest/EQ_Columns.cpp (from Mac debug map)
// Reads chs.dat (10 entries of 0x28 bytes); an entry with a score but no name gets "solo". Without the file
// the defaults are filled in. fopen 0x005223BC (mode at 0x0053C2BC), fread 0x00522038, fclose 0x00521FBB.
void EQ_Columns_guess::ReadHighScores()
{
    FILE* f = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))("chs.dat", (const char*)0x0053C2BC);
    int i;

    if (f == 0)
    {
        EQ_Columns_SetDefaultScores_guess(this);
    }
    else
    {
        for (i = 0; i < 10; i++)
        {
            EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(&scores_guess[i], 0x28, 1, f);
            if (scores_guess[i].score > 0 && scores_guess[i].name[0] == '\0')
                strcpy(scores_guess[i].name, "solo");
        }
        EQ_CDECL(int, 0x00521FBB, (FILE*))(f);
    }
}

// FUNCTION: TRILOGY 0x00433518
// Saves the ten Columns high score entries to chs.dat.
void EQ_Columns_guess::SaveHighScoresToFile_guess()
{
    FILE* f = fopen("chs.dat", "wb");
    int i;

    if (f != 0)
    {
        for (i = 0; i < 10; i++)
            fwrite(&scores_guess[i], 0x28, 1, f);
        fclose(f);
    }
}

// FUNCTION: TRILOGY 0x0043355c
// Inserts a qualifying game score in rank order, shifting lower entries down.
// @early-stop: j stays in memory ([esp+0x10]); the original keeps j in EAX around its decrement and reload
void EQ_Columns_guess::UpdateHighScoreTable_guess()
{
    int i;
    int j;
    ColumnsScore_guess* score;
    ColumnsScore_guess* p;

    for (i = 0, score = scores_guess; i < 10; i++, score++)
    {
        if (current_guess.score >= score->score)
        {
            j = 9;

            if (i != j && i < j)
            {
                p = &scores_guess[9];

                do
                {
                    j--;
                    if (j >= 0)
                    {
                        p->score = p[-1].score;
                        p->level = p[-1].level;
                        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(p[-1].name, p->name);
                    }
                    p--;
                } while (i < j);
            }
            if (g_pc_guess != 0)
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(current_guess.name, (char*)g_pc_guess);
            scores_guess[i] = current_guess;
            score_rank_guess = i;
            EQ_METHOD(0, void, 0x00433518, (EQ_Columns_guess*), this)();
            return;
        }
    }
}

// FUNCTION: TRILOGY 0x004335fe
// Adds (n * n + 1) * 225 to the score: doubled with the special block 0x7E2, negated with 0x803.
void EQ_Columns_guess::AddColumnsScore_guess(int n)
{
    int add = n * n * 0xE1 + 0xE1;

    if (special_guess == 0x803)
        add = -add;
    else if (special_guess == 0x7E2)
        add = add * 2;
    current_guess.score += add;
}

// FUNCTION: TRILOGY 0x00433635
// __thiscall that ignores `this`. The block types with an effect (listed in the Mac source as "special").
int EQ_Columns_guess::IsSpecialKeyCode_guess(int code)
{
    int result = 0;

    switch (code)
    {
    case 0x801:
    case 0x803:
    case 0x806:
    case 0x80B:
    case 0x843:
    case 0x849:
        result = 1;
    }
    return result;
}

// FUNCTION: TRILOGY 0x00433661
// Makes `code` the special block in effect and sets the end of its effect (24 s, 12 s, 6 s, none for 0x82A).
void EQ_Columns_guess::ProcessKeyCodeAndSetTimer_guess(int code)
{
    int effectEndTime;

    if (code == 0x824 && special_guess == 0x824)
        return;
    CDisplay_PlaySoundA_guess(g_local_player_guess->y, g_local_player_guess->x, g_local_player_guess->z, 0x70);
    special_guess = code;
    switch (code)
    {
    case 0x7E2:
    case 0x803:
        effectEndTime = g_cdisplay_guess->time_ms_guess + 24000;
        break;
    case 0x7E6:
        timer_88c_guess = g_cdisplay_guess->time_ms_guess + 2000;
        // fall through
    case 0x806:
        effectEndTime = g_cdisplay_guess->time_ms_guess + 6000;
        break;
    case 0x82A:
        effect_end_guess = 0;
        return;
    default:
        effectEndTime = g_cdisplay_guess->time_ms_guess + 12000;
        break;
    }
    effect_end_guess = effectEndTime;
}

// FUNCTION: TRILOGY 0x00433726
// Marks the cells of column `col` from row count - 1 up to row 0 as active (falling) when they hold a block.
void EQ_Columns_guess::MarkColumnCellsAsActive_guess(int col, int count)
{
    int row = count - 1;
    s32* cell;      // points at the cell's active field, so its color is cell[-2]

    if (row >= 0)
    {
        cell = &cells_guess[col + row * 10].active;
        row++;
        do
        {
            if (cell[-2] != -1)
            {
                *cell = 1;
                falling_cells_guess++;
            }
            cell -= 40;
            row--;
        } while (row != 0);
    }
}

// FUNCTION: TRILOGY 0x00433764
// __thiscall that ignores `this`.
void EQ_Columns_guess::ResetColumnCell_guess(ColumnsCell_guess* cell)
{
    cell->color = -1;
    cell->active = 0;
    cell->settled = 0;
    cell->y_offset = 0;
}

// FUNCTION: TRILOGY 0x00433779
// Handles groups of three or more matching blocks, plays a sound, and updates their matched cells.
// A line of `count` equal blocks was found (more than two score): sound, special block, and the cells are
// marked to fall, each adding a growing score.
void EQ_Columns_guess::ProcessMatchedColumns_guess(int count)
{
    int code;
    int bonus;
    int i;

    if (count > 2)
    {
        code = matches_guess[0].cell->color;
        CDisplay_PlaySoundA_guess(g_local_player_guess->y, g_local_player_guess->x, g_local_player_guess->z, 0x89);
        if (EQ_METHOD(1, int, 0x00433635, (EQ_Columns_guess*, int), this)(code) != 0)
            bonus = count - 2;
        else
            bonus = count - 3;
        if (bonus > 0)
            EQ_METHOD(1, void, 0x00433661, (EQ_Columns_guess*, int), this)(code);
        for (i = 0; i < count; i++)
        {
            EQ_METHOD(2, void, 0x00433726, (EQ_Columns_guess*, int, int), this)(matches_guess[i].col, matches_guess[i].row);
            EQ_METHOD(1, void, 0x004335FE, (EQ_Columns_guess*, int), this)(i);
            if (i > 0)
                EQ_METHOD(1, void, 0x00433764, (EQ_Columns_guess*, ColumnsCell_guess*), this)(matches_guess[i].cell);
        }
    }
}

// FUNCTION: TRILOGY 0x00433819
// Finds and scores the lines of equal blocks through cell col,row (vertical, horizontal, both diagonals).
// @early-stop: entry register allocation and scan pointer layout still differ
void EQ_Columns_guess::CheckAndProcessMatchingCells_guess(int col, int row)
{
    ColumnsCell_guess *cell;
    int color;
    int hasRun;
    int rowBase;
    int nextRow;
    int prevRow;
    int *clearPtr;
    int *matchPtr;
    int sideCol;
    int *scanCell;
    int rightCol;
    int diagRowBase;
    int cellOffset;
    int cellRow = row;
    int cellCol = col;
    int scanRow;

    rowBase = cellRow * 10;
    nextRow = (rowBase + cellCol) * 0x10;
    cell = &cells_guess[row * 10 + col];
    hasRun = 0;
    // Only a block (color >= 1) starts a run.
    if (cell->color >= 1)
    {
        // Clear the match list (22 slots). 'row' is then the match count.
        clearPtr = (int *)&matches_guess[0].col;
        row = 0x16;
        do
        {
            clearPtr[-1] = 0;
            *clearPtr = 0;
            clearPtr[1] = 0;
            clearPtr = clearPtr + 3;
            row = row + -1;
        } while (row != 0);
        color = cell->color;
        matches_guess[0].cell = cell;
        nextRow = cellRow + 1;
        row = 1;
        matches_guess[0].col = cellCol;
        matches_guess[0].row = cellRow;
        // Vertical line: cells below, then above.
        if (nextRow < 0xc)
        {
            matchPtr = (int *)(((int)cellCol + nextRow * 10) * 0x10 + 0xa0c + (int)this);
            prevRow = nextRow;
            scanCell = (int *)&matches_guess[1].col;
            do
            {
                if (*matchPtr != color)
                    break;
                row = row + 1;
                scanCell[-1] = (int)matchPtr;
                *scanCell = cellCol;
                scanCell[1] = prevRow;
                scanCell = scanCell + 3;
                prevRow = prevRow + 1;
                matchPtr = matchPtr + 0x28;
            } while (prevRow < 0xc);
        }
        prevRow = cellRow + -1;
        if (prevRow >= 0)
        {
            matchPtr = (int *)&matches_guess[row].col;
            scanCell = (int *)((cellCol + prevRow * 10) * 0x10 + 0xa0c + (int)this);
            scanRow = prevRow;
            do
            {
                if (*scanCell != color)
                    break;
                row = row + 1;
                matchPtr[-1] = (int)scanCell;
                *matchPtr = cellCol;
                matchPtr[1] = scanRow;
                matchPtr = matchPtr + 3;
                scanRow = scanRow + -1;
                scanCell = scanCell + -0x28;
            } while (scanRow >= 0);
        }
        EQ_METHOD(1, void, 0x00433779, (EQ_Columns_guess*, int), this)(row);
        if (2 < row)
            hasRun = 1;
        clearPtr = (int *)&matches_guess[0].col;
        rightCol = 0x16;
        do
        {
            clearPtr[-1] = 0;
            *clearPtr = 0;
            clearPtr[1] = 0;
            clearPtr = clearPtr + 3;
            rightCol = rightCol + -1;
        } while (rightCol != 0);
        rightCol = cellCol + 1;
        row = 1;
        matches_guess[0].cell = cell;
        matches_guess[0].col = cellCol;
        matches_guess[0].row = cellRow;
        // Horizontal line: cells right, then left.
        if (rightCol < 10)
        {
            matchPtr = (int *)&matches_guess[1].col;
            sideCol = rightCol;
            scanCell = (int *)((rowBase + rightCol) * 0x10 + 0xa0c + (int)this);
            do
            {
                if (*scanCell != color)
                    break;
                row = row + 1;
                matchPtr[-1] = (int)scanCell;
                *matchPtr = sideCol;
                matchPtr[1] = cellRow;
                matchPtr = matchPtr + 3;
                sideCol = sideCol + 1;
                scanCell = scanCell + 4;
            } while (sideCol < 10);
        }
        sideCol = cellCol + -1;
        if (sideCol >= 0)
        {
            matchPtr = (int *)&matches_guess[row].col;
            scanCell = (int *)((rowBase + sideCol) * 0x10 + 0xa0c + (int)this);
            col = sideCol;
            do
            {
                if (*scanCell != color)
                    break;
                row = row + 1;
                matchPtr[-1] = (int)scanCell;
                *matchPtr = col;
                matchPtr[1] = cellRow;
                matchPtr = matchPtr + 3;
                col = col - 1;
                scanCell = scanCell + -4;
            } while (col >= 0);
        }
        EQ_METHOD(1, void, 0x00433779, (EQ_Columns_guess*, int), this)(row);
        if (2 < row)
            hasRun = 1;
        clearPtr = (int *)&matches_guess[0].col;
        rowBase = 0x16;
        do
        {
            clearPtr[-1] = 0;
            *clearPtr = 0;
            clearPtr[1] = 0;
            clearPtr = clearPtr + 3;
            rowBase = rowBase + -1;
        } while (rowBase != 0);
        matches_guess[0].cell = cell;
        row = 1;
        matches_guess[0].col = cellCol;
        matches_guess[0].row = cellRow;
        // Rising diagonal: up-right, then down-left.
        if (prevRow >= 0)
        {
            matchPtr = (int *)&matches_guess[1].col;
            diagRowBase = prevRow * 10;
            rowBase = rightCol;
            scanRow = prevRow;
            do
            {
                cellOffset = (diagRowBase + rowBase) * 0x10;
                if (*(int *)(cellOffset + 0xa0c + (int)this) != color)
                    break;
                row = row + 1;
                matchPtr[-1] = cellOffset + 0xa0c + (int)this;
                *matchPtr = rowBase;
                matchPtr[1] = scanRow;
                matchPtr = matchPtr + 3;
                rowBase = rowBase + 1;
                if (9 < rowBase)
                    break;
                scanRow = scanRow + -1;
                diagRowBase = diagRowBase + -10;
            } while (-1 < scanRow);
        }
        if (nextRow < 0xc)
        {
            diagRowBase = nextRow * 10;
            matchPtr = (int *)&matches_guess[row].col;
            rowBase = sideCol;
            scanRow = nextRow;
            do
            {
                cellOffset = (diagRowBase + rowBase) * 0x10;
                if (*(int *)(cellOffset + 0xa0c + (int)this) != color)
                    break;
                row = row + 1;
                matchPtr[-1] = cellOffset + 0xa0c + (int)this;
                *matchPtr = rowBase;
                matchPtr[1] = scanRow;
                matchPtr = matchPtr + 3;
                rowBase = rowBase + -1;
                if (rowBase < 0)
                    break;
                scanRow = scanRow + 1;
                diagRowBase = diagRowBase + 10;
            } while (diagRowBase < 0x78);
        }
        EQ_METHOD(1, void, 0x00433779, (EQ_Columns_guess*, int), this)(row);
        if (2 < row)
            hasRun = 1;
        clearPtr = (int *)&matches_guess[0].col;
        rowBase = 0x16;
        do
        {
            clearPtr[-1] = 0;
            *clearPtr = 0;
            clearPtr[1] = 0;
            clearPtr = clearPtr + 3;
            rowBase = rowBase + -1;
        } while (rowBase != 0);
        matches_guess[0].col = cellCol;
        matches_guess[0].cell = cell;
        row = 1;
        matches_guess[0].row = cellRow;
        // Falling diagonal: up-left, then down-right.
        if (prevRow >= 0)
        {
            matchPtr = (int *)&matches_guess[1].col;
            cellCol = prevRow * 10;
            do
            {
                cellRow = (sideCol + cellCol) * 0x10;
                if (*(int *)(cellRow + 0xa0c + (int)this) != color)
                    break;
                row = row + 1;
                matchPtr[-1] = cellRow + 0xa0c + (int)this;
                *matchPtr = sideCol;
                matchPtr[1] = prevRow;
                matchPtr = matchPtr + 3;
                sideCol = sideCol + -1;
                if (sideCol < 0)
                    break;
                prevRow = prevRow + -1;
                cellCol = cellCol + -10;
            } while (prevRow >= 0);
        }
        if (nextRow < 0xc)
        {
            cellCol = nextRow * 10;
            matchPtr = (int *)&matches_guess[row].col;
            do
            {
                cellRow = (rightCol + cellCol) * 0x10;
                if (*(int *)(cellRow + 0xa0c + (int)this) != color)
                    break;
                row = row + 1;
                matchPtr[-1] = cellRow + 0xa0c + (int)this;
                *matchPtr = rightCol;
                matchPtr[1] = nextRow;
                matchPtr = matchPtr + 3;
                rightCol = rightCol + 1;
                if (9 < rightCol)
                    break;
                cellCol = cellCol + 10;
                nextRow = nextRow + 1;
            } while (cellCol < 0x78);
        }
        EQ_METHOD(1, void, 0x00433779, (EQ_Columns_guess*, int), this)(row);
        if (2 < row)
            hasRun = 1;
        // Any line of 3 or more (this cell included) resets this cell.
        if (hasRun)
        {
            EQ_METHOD(1, void, 0x00433764, (EQ_Columns_guess*, ColumnsCell_guess*), this)(cell);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00433c54
// Advances falling blocks and cells; when blocked, settles them and checks for matches.
void EQ_Columns_guess::UpdateColumnCellPosition_guess(int col, int row, int isBlock)
{
    int moveResult;
    int value;
    int *cellCounter;
    int cellIndex;

    if (special_guess == 0x7e8)
    {
        cellIndex = col + row * 10;
        value = (cellIndex + 0xa1) * 0x10;
        *(int *)((char *)this + value) += 1;
    }
    else
    {
        cellIndex = col + row * 10;
        value = (cellIndex + 0xa1) * 0x10;
        *(int *)((char *)this + value) += 2;
    }
    cellCounter = (int *)((char *)this + value);
    if (((fast_drop_guess == 0) && (isBlock != 0)) && (special_guess != 0x7e3))
    {
        if (special_guess == 0x806)
        {
            *cellCounter = *cellCounter + 0xc;
        }
    }
    else
    {
        *cellCounter = *cellCounter + 0x12;
    }
    if (0 < *cellCounter)
    {
        value = row + 1;
        moveResult = EQ_METHOD(5, int, 0x004333D6, (EQ_Columns_guess*, int, int, int, int, int), this)(col, row, col, value, 1);
        if (moveResult != 0)
        {
            *(int *)((char *)this + (col + 0xa1 + value * 10) * 0x10) =
                1 - height_guess;
            if (isBlock != 0)
            {
                cur_row_guess = value;
            }
        }
        else
        {
            if (((special_guess == 0x7e3) && (isBlock != 0)) && (cur_block_guess == 0x7e3))
            {
                if (row < 0xb)
                {
                    EQ_METHOD(1, void, 0x00433764, (EQ_Columns_guess*, ColumnsCell_guess*), this)((ColumnsCell_guess*)((char *)this + cellIndex * 0x10 + 0xaac));
                    EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
                         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y,
                         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x + 0.5f,
                         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z + 5.0f, 0x6e);
                    return;
                }
                EQ_METHOD(1, void, 0x00433764, (EQ_Columns_guess*, ColumnsCell_guess*), this)((ColumnsCell_guess*)((char *)this + cellIndex * 0x10 + 0xa0c));
                EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
                     EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y,
                     EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x + 0.5f,
                     EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z + 5.0f, 0x6e);
            }
            *cellCounter = 0;
            *(int *)((char *)this + cellIndex * 0x10 + 0xa14) = 0;
            if (isBlock != 0)
            {
                cur_block_guess = -1;
                cur_col_guess = 0;
                cur_row_guess = 0;
            }
            else
            {
                cellCounter = (int *)&falling_cells_guess;
                *cellCounter = *cellCounter + -1;
                if (*cellCounter < 0)
                {
                    falling_cells_guess = 0;
                }
            }
            EQ_METHOD(2, void, 0x00433819, (EQ_Columns_guess*, int, int), this)(col, row);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00433e31
// Updates Columns gameplay timers, active effects, falling blocks, and level transitions.
// ORIGINAL FILE: EverQuest/EQ_Columns.cpp (from Mac debug map)
// @early-stop: entry zero for rowHadBlock hoisted (paused test uses EBP); last-loop spill is bottomCell not cell; now temp in EAX not ECX; inner loop cmp/jg not test/jge
void EQ_Columns_guess::Update_guess()
{
    u32 now;
    int rowHadBlock;
    int col;
    int *bottomCell;
    int row;
    int *cell;

    col = 0;
    if (paused_guess == 0)
    {
        row = special_guess;
        if (row != -1)
        {
            if (row == 0x82a)
            {
                if (g_cdisplay_guess->time_ms_guess > timer_88c_guess)
                {
                    EQ_METHOD(0, void, 0x00432325, (EQ_Columns_guess *), this)();
                }
            }
            else
            {
                if ((row == 0x7e6) &&
                    (g_cdisplay_guess->time_ms_guess > timer_88c_guess))
                {
                    rowHadBlock = false;
                    bottomCell = (int *)((char *)this + 0x10ec);
                    do
                    {
                        if (*bottomCell != -1)
                        {
                            rowHadBlock = true;
                        }
                        EQ_METHOD(2, void, 0x00433726, (EQ_Columns_guess *, int, int), this)(col, 0xb);
                        EQ_METHOD(1, void, 0x00433764, (EQ_Columns_guess *, ColumnsCell_guess *), this)(
                                (ColumnsCell_guess *)bottomCell);
                        col = col + 1;
                        bottomCell = bottomCell + 4;
                    } while (col < 10);
                    if (rowHadBlock)
                    {
                        EQ_METHOD(4, void, 0x00410D13, (void *, float, float, float, int), g_display_guess)(
                             EQ_GLOBAL(EQPlayer *, 0x006E4D8C)->y,
                             EQ_GLOBAL(EQPlayer *, 0x006E4D8C)->x,
                             EQ_GLOBAL(EQPlayer *, 0x006E4D8C)->z, 99);
                    }
                }
                if (g_cdisplay_guess->time_ms_guess >
                    effect_end_guess)
                {
                    special_guess = -1;
                }
            }
        }
        if (g_cdisplay_guess->time_ms_guess < fall_time_guess)
        {
            if (fall_time_guess -
                g_cdisplay_guess->time_ms_guess <= 0x168)
            {
                return;
            }
        }
        else
        {
            if (special_guess == 0x801)
            {
                *(int *)((char *)this + 0x860) =
                    *(int *)((char *)this + 0x860) + -1;
            }
            if (next_state_guess == 3)
            {
                now = g_cdisplay_guess->time_ms_guess;
                if ((now > timer_88c_guess) ||
                    (8000 < timer_88c_guess - now))
                {
                    next_state_guess = 2;
                    timer_88c_guess = now + 2000;
                    game_state_guess = 1;
                }
            }
            else if (cur_block_guess == -1)
            {
                EQ_METHOD(0, void, 0x004331E1, (EQ_Columns_guess *), this)();
            }
            else if (steps_guess >= level_steps_guess)
            {
                EQ_METHOD(0, void, 0x00432325, (EQ_Columns_guess *), this)();
            }
            else
            {
                EQ_METHOD(1, void, 0x00433357, (EQ_Columns_guess *, int), this)(move_guess);
                EQ_METHOD(3, void, 0x00433C54, (EQ_Columns_guess *, int, int, int), this)(cur_col_guess, cur_row_guess, 1);
                col = 0;
                if (0 < falling_cells_guess)
                {
                    bottomCell = (int *)((char *)this + 0x10f4);
                    do
                    {
                        row = 0xb;
                        cell = bottomCell;
                        do
                        {
                            if ((*cell != 0) &&
                                ((row != cur_row_guess) ||
                                 (col != cur_col_guess)))
                            {
                                EQ_METHOD(3, void, 0x00433C54, (EQ_Columns_guess *, int, int, int), this)(col, row, 0);
                            }
                            cell = cell + -0x28;
                            row = row + -1;
                        } while (-1 < row);
                        col = col + 1;
                        bottomCell = bottomCell + 4;
                    } while (col < 10);
                }
            }
        }
        EQ_METHOD(0, void, 0x00432626, (EQ_Columns_guess *), this)();
    }
    return;
}
