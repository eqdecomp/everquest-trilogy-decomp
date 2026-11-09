#include "trilogy/eqaddr.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define debugSequence EQ_GLOBAL(unsigned short, 0x100FBBCC)
#define debugMessageFormat EQ_ARRAY(char, 0x100A367C)
#define debugFileName EQ_ARRAY(char, 0x100A3674)
#define GFX_TEXTURE_LOG_MODE EQ_ARRAY(char, 0x10086170)
#define debugFileFormat EQ_ARRAY(char, 0x100A3670)

// FUNCTION: EQGFX 0x100509ee
// Writes a timestamped debug message to the configured log file.
int __cdecl DbgLog_guess(const char *format, ...)
{
    char output[2048];
    char message[2048];
    char timeText[32];
    time_t now;
    char *last;
    FILE *file;
    va_list args;
    unsigned int sequence;

    now = EQ_CDECL(time_t, 0x10069FE4, (time_t *))(0);

    va_start(args, format);
    EQ_CDECL(int, 0x1006A0D9, (char *, const char *, va_list))(message, format, args);
    va_end(args);

    EQ_CDECL(void, 0x10068070, (char *, char *))(timeText,
        EQ_CDECL(char *, 0x1006A0C0, (time_t *))(&now));
    last = &timeText[EQ_CDECL(size_t, 0x10068160, (const char *))(timeText) - 1];
    if (*last == '\n')
        *last = '\0';

    sequence = (unsigned short)debugSequence;
    debugSequence++;
    EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(
        output, debugMessageFormat, timeText, sequence, message);
    file = EQ_CDECL(FILE *, 0x10068412, (char *, char *))(debugFileName, GFX_TEXTURE_LOG_MODE);
    if (file != 0)
    {
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, debugFileFormat, output);
        EQ_CDECL(int, 0x10068328, (FILE *))(file);
    }

    return 0;
}
