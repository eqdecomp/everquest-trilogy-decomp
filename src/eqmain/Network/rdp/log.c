// eqmain: the same original object as trilogy's src/Network/rdp/log.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* log.c - the discard log: a printf-style line, time-stamped, appended to discard.log.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/log.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x1013C9E8: the log's FILE*, opened on first use and never closed. */
#define g_discard_log_guess  EQ_GLOBAL(FILE *, 0x1013C9E8)
/* 0x1003407F: fopen-like library wrapper (19 bytes), the same call the sibling files make. */
#define eqmain_fn_1003407f_guess  EQ_CDECL(FILE *, 0x1003407F, (const char *, const char *))
/* This image's own strings: 0x100498DC "discard.log", 0x100498E8 "a", 0x100498B8 the stamp format. */
#define g_discard_log_name_guess  EQ_ARRAY(char, 0x100498DC)
#define g_discard_log_mode_guess  EQ_ARRAY(char, 0x100498E8)
#define g_discard_stamp_fmt_guess EQ_ARRAY(char, 0x100498B8)

// FUNCTION: EQMAIN 0x1002fa10
// Appends a UTC-timestamped, printf-formatted line to a log file opened on first use, then flushes it.
// Cloned from trilogy discard_log_append_guess 0x004F66F0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Opens discard.log ("a") on first use (returns if that fails), then writes
// "[mm/dd/yy hh:mm:ss] " (GetSystemTime, so UTC) and the formatted message, and flushes.
void discard_log_append_guess(const char *format, ...)
{
    SYSTEMTIME st;
    char stamp[24];
    FILE *file;
    va_list args;

    if (g_discard_log_guess == 0)
    {
        g_discard_log_guess = eqmain_fn_1003407f_guess(g_discard_log_name_guess, g_discard_log_mode_guess);
        if (g_discard_log_guess == 0)
            return;
    }
    file = g_discard_log_guess;
    GetSystemTime(&st);
    sprintf(stamp, g_discard_stamp_fmt_guess, st.wMonth, st.wDay, st.wYear % 100, st.wHour, st.wMinute,
            st.wSecond);
    fprintf(file, stamp);
    va_start(args, format);
    vfprintf(g_discard_log_guess, format, args);
    fflush(g_discard_log_guess);
}
