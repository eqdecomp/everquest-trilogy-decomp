/* log.c - the discard log: a printf-style line, time-stamped, appended to discard.log.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/log.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x006EFED0: the log's FILE*, opened on first use and never closed. */
#define g_discard_log_guess  EQ_GLOBAL(FILE *, 0x006EFED0)

// FUNCTION: TRILOGY 0x004f66f0
// Records invalid RDP messages for diagnostics.
// ORIGINAL FILE: Network/rdp/log.c (Mac: discard_log_append, medium)
// __cdecl. Opens discard.log ("a") on first use (returns if that fails), then writes
// "[mm/dd/yy hh:mm:ss] " (GetSystemTime, so UTC) and the formatted message, and flushes.
void discard_log_append(const char *format, ...)
{
    SYSTEMTIME st;
    char stamp[24];
    FILE *file;
    va_list args;

    if (g_discard_log_guess == 0)
    {
        g_discard_log_guess = fopen("discard.log", "a");
        if (g_discard_log_guess == 0)
            return;
    }
    file = g_discard_log_guess;
    GetSystemTime(&st);
    sprintf(stamp, "[%02u/%02u/%02u %02u:%02u:%02u] ", st.wMonth, st.wDay, st.wYear % 100, st.wHour, st.wMinute,
            st.wSecond);
    fprintf(file, stamp);
    va_start(args, format);
    vfprintf(g_discard_log_guess, format, args);
    fflush(g_discard_log_guess);
}
