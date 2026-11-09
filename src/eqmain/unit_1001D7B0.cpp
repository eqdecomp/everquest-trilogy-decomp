// unit_1001D7B0.cpp - eqmain.dll, the address run 0x1001D7B0..0x1001DB40 (a queue unit; no Mac map covers eqmain.dll,
// so the original object and its name are unknown). RunNewsScreen_guess and LoadSelServerOverlay_guess are in this file so far.

#include "trilogy/eqaddr.h"
#include <stdio.h>
#include <string.h>

// This image's addresses for the function below (file-local).
#define g_selserver_obj_guess    EQ_GLOBAL(void *, 0x100F10D0)          // DAT_100f10d0: the overlay handle; the new one is stored here
#define g_selserver_arg_guess    EQ_GLOBAL(int, 0x100F115C)             // g_banner_obj_guess: the 4th argument of BltFastSrcRect_guess
#define g_draw_obj_guess         EQ_GLOBAL(void *, 0x1004CEB8)          // g_eqmain_ceb8_guess: the draw object (first argument of both calls)
#define g_selserver_bmp_guess    EQ_ARRAY(char, 0x100483FC)             // "..."
#define g_eqmain_drive_base_guess         EQ_ARRAY(char, 0x1004CE40)             // g_eqmain_drive_base_guess: a string; its address is pushed

// This image's addresses for RunNewsScreen_guess (file-local).
#define g_news_bmp_guess         EQ_ARRAY(char, 0x10046240)            // "..."
#define g_str_888_guess         EQ_GLOBAL(char *, 0x10055888)         // g_str_888_guess: a string pointer (third argument of eqmain_draw_text_guess)
#define g_eqmain_dlg_arg_guess       EQ_GLOBAL(int, 0x1004CEC8)            // g_eqmain_dlg_arg_guess: a dword (eighth argument of EQMain_CreateRecord_guess)
#define g_eqmain_flag_ce78_guess        EQ_GLOBAL(unsigned char, 0x1004CE78)  // g_eqmain_flag_ce78_guess: 1 while the news screen runs

// FUNCTION: EQMAIN 0x1001D7B0
// Shows the news page from its text file or fallback and lets the player scroll through it.
// ORIGINAL FILE: unknown
// __cdecl, callee pops 0 bytes; returns AL = result (1 once RdpReceiveScreenDispatch_guess has returned 0x12). eqmain_draw_text_guess takes five arguments
// here (Ghidra shows three). filePath is reused for LoadFileString_guess's result and pHVar5 uses imageName's slot for eqmain_create_button_guess's HDC;
// the only frame is the saved-ECX slot (result, done). The loop ends when done is set: RdpReceiveScreenDispatch_guess returned 0x12, or the
// event is message 4 with code 0x1b.
// @early-stop: the type-1 null-link guard adds two instructions; the type-0 compare and cleanup argument use different registers.
unsigned char __cdecl RunNewsScreen_guess(char *filePath, char *fallbackText, char *imageName)
{
    unsigned char result;
    bool done;
    int *textBlock;
    unsigned int *slider;
    void *pHVar5;
    char *pHVar8;
    int *pEvent;
    int tmp;
    int lineNode;
    int lineIndex;

    done = false;
    result = 0;
    textBlock = EQ_CDECL(int *, 0x10007AE0, (char *, int))(g_news_bmp_guess, 0x10);
    EQ_CDECL(unsigned int, 0x10006C20, (void *, int, int, int, int, int, int, int, unsigned int))
        (g_draw_obj_guess, 0, 0, (int)textBlock, 0, 0, *textBlock, textBlock[1], 0);
    EQ_CDECL(void, 0x10007A80, (int *))(textBlock);
    EQ_CDECL(void, 0x100056C0, (void *, char *, int, int))(g_draw_obj_guess, imageName, 0, 0);
    pHVar5 = EQ_CDECL(void *, 0x10001520, (void *, int, int, unsigned char, int))
        (g_draw_obj_guess, 0, 0, 0x11, 2);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))(pHVar5, 0, g_str_888_guess, 1, 0);
    slider = EQ_CDECL(unsigned int *, 0x10006950, (void *, int, int, int, int, int))
        (g_draw_obj_guess, 0x265, 0x3d, 0x10, 0x170, 0);
    textBlock = EQ_CDECL(int *, 0x10008C10, (void *, int, int, int, int, int, int, int, unsigned int *))
        (g_draw_obj_guess, 0x34, 0x4a, 0x212, 0x146, 0x1f40, 0x22d69, g_eqmain_dlg_arg_guess, slider);
    filePath = EQ_CDECL(char *, 0x100239D0, (char *))(filePath);
    if (filePath == 0)
    {
        EQ_CDECL(void, 0x10009370, (int *, char *))(textBlock, fallbackText);
    }
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    pHVar8 = filePath;
    do
    {
        if (pHVar8 != 0)
        {
            tmp = textBlock[10];
            pHVar8 = EQ_CDECL(char *, 0x100095E0, (int *, char *))(textBlock, pHVar8);
            textBlock[10] = tmp;
            if (pHVar8 == 0)
            {
                EQ_CDECL(void, 0x100239C0, (void *))(filePath);
            }
            if (textBlock[10] == 0)
            {
                textBlock[10] = textBlock[7];
            }
            slider[0x1a] = **(unsigned int **)(textBlock[10] + 4);
            tmp = (textBlock[6] - textBlock[0xb]) + 2;
            if (tmp < 1)
            {
                tmp = 1;
            }
            slider[0x19] = tmp;
            EQ_CDECL(void, 0x100067E0, (int))((int)slider);
            EQ_CDECL(void, 0x10008D60, (int *))(textBlock);
        }
        if (EQ_STDCALL(int, 0x100220F0, (void))() == 0x12)
        {
            result = 1;
            done = true;
        }
        pEvent = EQ_STDCALL(int *, 0x10003BD0, (void))();
        if (pEvent != 0)
        {
            tmp = *pEvent;
            switch (tmp)
            {
            case 0:
                if (pHVar5 == (void *)pEvent[2])
                {
                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))(4, 0x1b, 0, 0);
                }
                break;
            case 1:
                if ((unsigned int *)pEvent[2] == slider)
                {
                    tmp = (int)slider[0x1a];
                    lineNode = textBlock[7];
                    lineIndex = 0;
                    if (0 < tmp && lineNode != 0)
                    {
                        do
                        {
                            lineNode = *(int *)(lineNode + 8);
                            lineIndex = lineIndex + 1;
                        } while (lineNode != 0 && lineIndex < tmp);
                    }
                    textBlock[10] = lineNode;
                    EQ_CDECL(void, 0x10008D60, (int *))(textBlock);
                }
                break;
            case 4:
                if ((char)pEvent[2] == '\x1b')
                {
                    EQ_STDCALL(void, 0x10003A00, (void))();
                    done = true;
                }
                break;
            }
            EQ_CDECL(void, 0x10033568, (void *))(pEvent);
        }
    } while (!done);
    g_eqmain_flag_ce78_guess = 0;
    EQ_CDECL(void, 0x10001700, (void *))(pHVar5);
    EQ_CDECL(void, 0x10008CE0, (void *))(textBlock);
    EQ_CDECL(void, 0x10006AE0, (void *))(slider);
    EQ_CDECL(void, 0x100039E0, (int))(0);
    return result;
}

// This image's addresses for CheckNewsChecksum_guess (file-local).
#define g_news_path_guess        EQ_ARRAY(char, 0x10055530)            // g_news_path_guess: the news path, lowercased in place
#define g_str_test_guess         EQ_ARRAY(char, 0x100482C0)            // "test"
#define g_str_testeqnews_guess   EQ_ARRAY(char, 0x10048188)            // "..."
#define g_str_eqnews_guess       EQ_ARRAY(char, 0x10048170)            // "..."
#define g_str_equchk_guess       EQ_ARRAY(char, 0x100482B4)            // "..."
#define g_str_txnews2_guess      EQ_ARRAY(char, 0x1004817C)            // "..."
#define g_eqmain_fopen_mode_guess        EQ_ARRAY(char, 0x100480A4)            // g_eqmain_fopen_mode_guess: fopen mode for reading (guess from use)
#define g_str_10047f74_guess       EQ_ARRAY(char, 0x10047F74)            // DAT_10047f74: fopen mode for writing (guess from use)
#define g_eqmain_flag_1d20_guess   EQ_GLOBAL(unsigned char, 0x100F1D20) // g_eqmain_flag_1d20_guess: 1 when the news path contains "test"
#define g_news_ptr_guess         EQ_GLOBAL(char *, 0x10055700)         // g_news_ptr_guess: a pointer (second argument of RunNewsScreen_guess)

// FUNCTION: EQMAIN 0x1001D9F0
// Checks for updated front-end news and opens the news page when its text changes.
// ORIGINAL FILE: unknown
// __stdcall, no args, returns a bool in AL. The news path (g_news_path_guess) is lowercased and searched for "test"; that
// picks testeqnews.txt (and sets g_eqmain_flag_1d20_guess) or eqnews.txt. LoadFileString_guess loads the file named in newsPath, and the
// signed bytes of the loaded buffer are summed up to its NUL. equchk.txt holds the sum from the last run. When the two
// differ, the news screen runs (RunNewsScreen_guess with txnews2.bmp), its result is returned, and the new sum is written
// back to equchk.txt.
// @early-stop: the Identity_guess argument pop is flushed at the join (the original keeps it pending into LoadFileString_guess's add esp,8), sumText/newsPath slots are swapped, and the walker sits in EDX (original EAX).
bool __stdcall CheckNewsChecksum_guess(void)
{
    char tmp;
    char *str;
    char *cursor;
    FILE *pFVar4;
    int checksum;
    int stored;
    bool result;
    char sumText[32];
    char newsPath[32];

    result = false;
    str = EQ_CDECL(char *, 0x10035FD0, (char *, char *))
        (EQ_CDECL(char *, 0x10035C00, (char *))(g_news_path_guess), g_str_test_guess);
    if (str != 0)
    {
        g_eqmain_flag_1d20_guess = 1;
        str = EQ_CDECL(char *, 0x100210F0, (char *))(g_str_testeqnews_guess);
    }
    else
    {
        str = EQ_CDECL(char *, 0x100210F0, (char *))(g_str_eqnews_guess);
    }
    strcpy(newsPath, str);
    str = EQ_CDECL(char *, 0x100239D0, (char *))(newsPath);
    if (str == 0)
    {
        return false;
    }
    checksum = 0;
    tmp = *str;
    cursor = str;
    while (tmp != '\0')
    {
        checksum = checksum + tmp;
        tmp = cursor[1];
        ++cursor;
    }
    EQ_CDECL(void, 0x100239C0, (void *))(str);
    stored = 0;
    pFVar4 = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(g_str_equchk_guess, g_eqmain_fopen_mode_guess);
    if (pFVar4 != 0)
    {
        EQ_CDECL(size_t, 0x100342A2, (void *, size_t, size_t, FILE *))(sumText, 0x20, 1, pFVar4);
        stored = EQ_CDECL(int, 0x1003507E, (char *))(sumText);
        EQ_CDECL(int, 0x100343B9, (FILE *))(pFVar4);
    }
    if (checksum != stored)
    {
        tmp = EQ_CDECL(char, 0x1001D7B0, (char *, char *, char *, int))(newsPath, g_news_ptr_guess, g_str_txnews2_guess, 1);
        if (tmp != '\0')
        {
            result = true;
        }
        pFVar4 = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(g_str_equchk_guess, g_str_10047f74_guess);
        if (pFVar4 != 0)
        {
            EQ_CDECL(char *, 0x10035089, (int, char *, int))(checksum, sumText, 10);
            EQ_CDECL(size_t, 0x10034169, (void *, size_t, size_t, FILE *))(sumText, 0x20, 1, pFVar4);
            EQ_CDECL(int, 0x100343B9, (FILE *))(pFVar4);
        }
    }
    return result;
}

// FUNCTION: EQMAIN 0x1001DB40
// Creates the server selection overlay graphic and its label.
// ORIGINAL FILE: unknown
// __stdcall, no args (callee pops 0), no return value: the RET leaves EAX as eqmain_draw_text_guess returned it, and no caller reads
// EAX. eqmain_object_destroy_guess (cdecl, one arg) runs first; its argument is popped by the ADD ESP,0x44 after the next two calls. Then
// BltFastSrcRect_guess (cdecl, nine args) on the draw object, eqmain_create_widget_guess (cdecl, seven args) builds the new handle from the draw
// object and "...", the handle is stored in DAT_100f10d0, and eqmain_draw_text_guess (cdecl) gets that handle, 0xe8c9cc
// and &g_eqmain_drive_base_guess, plus two zero args that it never reads (its pop is 0x14, five args).
void __stdcall LoadSelServerOverlay_guess(void)
{
    EQ_CDECL(void, 0x10001700, (void *))(g_selserver_obj_guess);
    EQ_CDECL(unsigned int, 0x10006C20, (void *, int, int, int, int, int, int, int, unsigned int))
        (g_draw_obj_guess, 0x74, 0x16a, g_selserver_arg_guess, 0, 0, 0x17c, 0x14, 0);
    g_selserver_obj_guess = EQ_CDECL(void *, 0x10001560, (void *, int, int, unsigned char, char *, char *, char *))
        (g_draw_obj_guess, 0xf0, 0x168, 0, g_selserver_bmp_guess, g_selserver_bmp_guess, g_selserver_bmp_guess);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (g_selserver_obj_guess, 0xe8c9cc, g_eqmain_drive_base_guess, 0, 0);
}
