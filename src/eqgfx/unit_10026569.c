// unit_10026569.c - eqgfx_dx7.dll, the address run 0x10026569..0x10029020 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"
#include <string.h>

typedef struct SearchPathContext_guess
{
    unsigned char unknown_00[0x10];
    const char *searchDirectory;
} SearchPathContext_guess;

typedef unsigned long (__stdcall *SearchPathA_guess_t)(const char *, const char *, const char *, unsigned long, char *, char **);

#define SearchPathA_EQGFX EQ_GLOBAL(SearchPathA_guess_t, 0x10076070)
#define searchPathBuffer_EQGFX EQ_ARRAY(char, 0x100ABE24)
#define s_out_of_memory_EQGFX EQ_ARRAY(const char, 0x100871D0)
#define s_ascii_scanner_invalid_number_EQGFX EQ_ARRAY(const char, 0x1009D8BC)
#define s_ascii_scanner_invalid_hex_EQGFX EQ_ARRAY(const char, 0x1009D888)
#define s_ascii_scanner_read_error_EQGFX EQ_ARRAY(const char, 0x1009D86C)
#define s_binary_scanner_incomplete_token_EQGFX EQ_ARRAY(const char, 0x1009D958)
#define s_binary_scanner_read_error_EQGFX EQ_ARRAY(const char, 0x1009D93C)
#define s_binary_scanner_read_failed_EQGFX EQ_ARRAY(const char, 0x1009D920)
#define s_binary_scanner_invalid_token_EQGFX EQ_ARRAY(const char, 0x1009D8F4)
#define s_unrecognized_file_format_EQGFX EQ_ARRAY(const char, 0x1009D9A8)
#define s_could_not_open_include_file_EQGFX EQ_ARRAY(const char, 0x1009D988)
#define GFX_COMPOSITE_ALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C48, (unsigned int))
#define logParserError_EQGFX EQ_CDECL(void, 0x1002902B, (void *, const char *))
#define openSourceFile_EQGFX EQ_CDECL(void *, 0x100262F0, (const char *, void *))
#define readTextFileChunk_EQGFX EQ_CDECL(int, 0x1002645C, (void *, void *, int))
#define strlen_EQGFX EQ_CDECL(unsigned int, 0x10068160, (const char *))
#define GFX_PATH_COPY_STRING EQ_CDECL(void, 0x10068070, (char *, const char *))
#define hexDigitValue_EQGFX EQ_CDECL(unsigned char, 0x100288BF, (unsigned char))
#define parseTextFileFunction_EQGFX ((void *)0x10026569)
#define releaseTextFileFunction_EQGFX ((void *)0x10029020)
#define TEXT_FILE_PARSER_SIZE 0x3444
#define TEXT_READ_CHUNK_SIZE 0x2000

typedef struct WorldFileReader_guess
{
    void *parseFunction;
    void *releaseFunction;
    void *owner;
    int parseStatus;
    char *searchDirectory;
} WorldFileReader_guess;

typedef unsigned long (__stdcall *GetEnvironmentVariableA_guess_t)(const char *, char *, unsigned long);

#define GFX_WLD_PATH EQ_GLOBAL(const char *, 0x100A35D4)
#define GetEnvironmentVariableA_EQGFX EQ_GLOBAL(GetEnvironmentVariableA_guess_t, 0x10076074)
#define initializeWorldFileReader_EQGFX EQ_CDECL(int, 0x10028F44, (WorldFileReader_guess *, void *))
#define EQGFX_T3D_FREE_MEMORY_GUESS EQ_CDECL(void, 0x10050CBB, (void *))

typedef struct TextCursorState_guess
{
    unsigned char unknown_00[0x14];
    int column;
    unsigned char unknown_18[0x2010];
    char text[1];
} TextCursorState_guess;

#define TEXT_TAB_WIDTH 8
#define t3dHunkKey_EQGFX EQ_ARRAY(const unsigned char, 0x100762C0)

typedef struct FileQueueEntry_guess FileQueueEntry_guess;

struct FileQueueEntry_guess
{
    int (__cdecl *poll)(FileQueueEntry_guess *entry);
    void (__cdecl *release)(FileQueueEntry_guess *entry);
    FileQueueEntry_guess *next;
    void *fileObject;
};

typedef struct ParserContext_guess
{
    unsigned char unknown_00[0x4];
    void *currentSource;
} ParserContext_guess;

typedef struct FileQueueOwner_guess
{
    unsigned char unknown_00[0x8];
    ParserContext_guess *parserContext;
    FileQueueEntry_guess *currentEntry;
    void *unknown_10;
} FileQueueOwner_guess;

typedef struct ParserFactory_guess
{
    unsigned char unknown_00[0x8];
    void *parserContext;
} ParserFactory_guess;

typedef struct BinaryFileParser_guess
{
    void *parseFunction;
    void *releaseFunction;
    unsigned char unknown_08[0x4];
    void *source;
    int unknown_10;
    unsigned char unknown_14[0x4];
    void *owner;
} BinaryFileParser_guess;

typedef struct ParserObject_guess
{
    void *parseFunction;
    void *releaseFunction;
    int unknown_08;
    int value_0c;
    unsigned char unknown_10[0x14];
    int unknown_24;
    int unknown_28;
    int unknown_2c;
    unsigned char unknown_30[0x4];
    int unknown_34;
    int unknown_38;
    int unknown_3c;
} ParserObject_guess;

typedef struct TextFileParser_guess
{
    void *parseFunction;
    void *releaseFunction;
    unsigned char unknown_08[0x4];
    void *fileContext;
    int enabled;
    unsigned char unknown_14[0x4];
    void *parserContext;
    int state;
    unsigned char textStorage[0x2000];
    char *textStart;
    char *textCurrent;
    unsigned char unknown_2028[0x404];
    int tokenStates[0x401];
    int unknown_3430;
    int unknown_3434;
    int tabWidth;
    int unknown_343c;
    unsigned char unknown_3440[0x4];
} TextFileParser_guess;

#define lexInput_EQGFX EQ_GLOBAL(void *, 0x100ABE10)
#define lexOutput_EQGFX EQ_GLOBAL(void *, 0x100ABE14)
#define stdinFile_EQGFX ((void *)0x100A7940)
#define stdoutFile_EQGFX ((void *)0x100A7960)
#define lexLineNumber_EQGFX EQ_GLOBAL(int, 0x1009D810)
#define lexStartStates_EQGFX EQ_ARRAY(int, 0x1008D370)
#define lexNext_EQGFX EQ_ARRAY(int, 0x1008D39C)
#define lexCheck_EQGFX EQ_ARRAY(int, 0x100921D4)
#define lexDefault_EQGFX EQ_ARRAY(int, 0x1009700C)
#define lexBase_EQGFX EQ_ARRAY(int, 0x1009A40C)
#define lexAcceptStart_EQGFX EQ_ARRAY(int, 0x10089F70)
#define lexAcceptList_EQGFX EQ_ARRAY(unsigned int, 0x10087204)
#define lexTrailStates_EQGFX EQ_ARRAY(unsigned char, 0x100ABE00)
#define lexBitMasks_EQGFX EQ_ARRAY(unsigned char, 0x1009D818)
#define s_lex_format_EQGFX EQ_ARRAY(const char, 0x1009D84C)
#define s_token_buffer_overflow_EQGFX EQ_ARRAY(const char, 0x1009D854)
#define s_ascii_scanner_illegal_character_EQGFX EQ_ARRAY(const char, 0x1009D820)
#define logParserMessage_EQGFX EQ_CDECL(void, 0x1002902B, (void *, const char *, const char *))
#define readLexerChar_EQGFX EQ_CDECL(int, 0x100286C6, (TextFileParser_guess *))
#define lexerWrap_EQGFX EQ_CDECL(int, 0x10026565, (void))
#define skipLexerLine_EQGFX EQ_CDECL(void, 0x1002872D, (TextFileParser_guess *))
#define countTokenColumns_EQGFX EQ_CDECL(void, 0x10028741, (TextFileParser_guess *))
#define copyTokenText_EQGFX EQ_CDECL(char *, 0x1002878A, (TextFileParser_guess *))
#define copyQuotedTokenText_EQGFX EQ_CDECL(char *, 0x100287CE, (TextFileParser_guess *))
#define decodeHexToken_EQGFX EQ_CDECL(char *, 0x1002881E, (void *, const char *))
#define memcpy_EQGFX EQ_CDECL(void *, 0x100679F0, (void *, const void *, unsigned int))
#define putc_EQGFX EQ_CDECL(int, 0x100696A2, (int, void *))
#define atoi_EQGFX EQ_CDECL(int, 0x1006965C, (const char *))
#define atof_EQGFX EQ_CDECL(double, 0x1006957A, (const char *))
#define LEX_TOKEN_MAX 0x400
#define LEX_TABLE_LAST 0x138c
#define LEX_JAM_STATE 0xcff
#define LEX_RULE_MASK 0x1ff
#define LEX_TRAIL_SHIFT 9
#define LEX_TRAIL_ROW_SIZE 0x1a0

// FUNCTION: EQGFX 0x10026569
// Generated lexer for the ascii world file; returns the next token id (0 at end of input, -2 on buffer overflow).
// Runs the DFA tables (lexBase/lexDefault, jam state 0xcff) over input read by 0x100286c6, backs up to the last
// accepting state, handles trailing contexts, then switches on the rule number (mask 0x1ff).
// About 480 cases: most call countTokenColumns and return a fixed token id; number, string and hex rules also
// convert the text (atoi, atof, copy and decode helpers) into context->value_0c.
int __cdecl t3dScanAsciiGetToken(TextFileParser_guess *lexer)
{
    ParserObject_guess *context;
    char *text;
    int eofSeen;
    int length;
    int state;
    int *stateSlot;
    char *cursor;
    int c;
    int index;
    int acceptIndex;
    unsigned int *acceptEntry;
    int savedLength;
    int trailRow;
    int *trailSlot;

    context = (ParserObject_guess *)lexer->parserContext;
    text = (char *)lexer->unknown_2028;
    if (lexInput_EQGFX == 0)
    {
        lexInput_EQGFX = stdinFile_EQGFX;
    }
    if (lexOutput_EQGFX == 0)
    {
        lexOutput_EQGFX = stdoutFile_EQGFX;
    }
    length = lexer->unknown_343c;
    eofSeen = 0;
    text[length] = lexer->unknown_3440[0];
restart:
    lexer->unknown_343c = length;
    if (length > 0)
    {
        lexer->tabWidth = text[length - 1];
        do
        {
            length--;
            if (text[length] == '\n')
            {
                lexLineNumber_EQGFX++;
            }
        } while (length > 0);
    }
    lexer->unknown_3430 -= lexer->unknown_343c;
    if (lexer->unknown_3430 > 0)
    {
        memcpy_EQGFX(text, text + lexer->unknown_343c, lexer->unknown_3430);
    }
    length = 0;
    state = lexStartStates_EQGFX[(lexer->tabWidth == '\n') + lexer->unknown_3434];
    stateSlot = lexer->tokenStates;
    *stateSlot = state;
    cursor = text;
    for (;;)
    {
        if (length >= LEX_TOKEN_MAX)
        {
            logParserMessage_EQGFX(context, s_lex_format_EQGFX, s_token_buffer_overflow_EQGFX);
            return -2;
        }
        if (length < lexer->unknown_3430)
        {
            c = *cursor;
        }
        else
        {
            if (eofSeen != 0 || (c = readLexerChar_EQGFX(lexer)) == -1)
            {
                if (length == 0)
                {
                    eofSeen = 0;
                    if (lexerWrap_EQGFX() != 0)
                    {
                        return 0;
                    }
                    goto restart;
                }
                eofSeen = 1;
                break;
            }
            lexer->unknown_3430 = cursor - text + 1;
            *cursor = (char)c;
        }
        for (;;)
        {
            index = lexBase_EQGFX[state] + (unsigned char)c;
            if (index <= LEX_TABLE_LAST && lexCheck_EQGFX[index] == state)
            {
                state = lexNext_EQGFX[index];
                break;
            }
            if (state == LEX_JAM_STATE)
            {
                break;
            }
            state = lexDefault_EQGFX[state];
        }
        stateSlot++;
        length++;
        cursor++;
        *stateSlot = state;
        if (state == LEX_JAM_STATE)
        {
            goto backUp;
        }
        if (lexBase_EQGFX[state] > LEX_TABLE_LAST && lexDefault_EQGFX[state] == LEX_JAM_STATE)
        {
            break;
        }
    }
    if (state != LEX_JAM_STATE)
    {
        length++;
    }
backUp:
    length--;
    while (length > 0)
    {
        acceptIndex = lexAcceptStart_EQGFX[lexer->tokenStates[length]];
        if (acceptIndex < lexAcceptStart_EQGFX[lexer->tokenStates[length] + 1])
        {
            goto accept;
        }
        length--;
    }
    length = 1;
    putc_EQGFX(text[0], lexOutput_EQGFX);
    goto restart;
accept:
    acceptEntry = &lexAcceptList_EQGFX[acceptIndex];
    savedLength = length;
    trailRow = (int)(*acceptEntry >> LEX_TRAIL_SHIFT) - 1;
    if (trailRow >= 0)
    {
        trailSlot = &lexer->tokenStates[length];
        for (;;)
        {
            length--;
            trailSlot--;
            if (length < 0)
            {
                length = savedLength;
                break;
            }
            if (lexBitMasks_EQGFX[*trailSlot & 7] &
                lexTrailStates_EQGFX[trailRow * LEX_TRAIL_ROW_SIZE + ((unsigned int)*trailSlot >> 3)])
            {
                break;
            }
        }
    }
    lexer->unknown_343c = length;
    lexer->unknown_3440[0] = text[length];
    text[length] = '\0';
    switch (*acceptEntry & LEX_RULE_MASK)
    {
    case 0x00:
        skipLexerLine_EQGFX(lexer);
    case 0x1d9:
    case 0x1e2:
        lexer->enabled++;
        break;
    case 0x01:
        countTokenColumns_EQGFX(lexer);
        return 0x9d;
    case 0x02:
        countTokenColumns_EQGFX(lexer);
        return 0x9e;
    case 0x03:
        countTokenColumns_EQGFX(lexer);
        return 0x9f;
    case 0x04:
        countTokenColumns_EQGFX(lexer);
        return 0xa0;
    case 0x05:
        countTokenColumns_EQGFX(lexer);
        return 0xa1;
    case 0x06:
        countTokenColumns_EQGFX(lexer);
        return 0xa2;
    case 0x07:
        countTokenColumns_EQGFX(lexer);
        return 0xa3;
    case 0x08:
        countTokenColumns_EQGFX(lexer);
        return 0xa4;
    case 0x09:
        countTokenColumns_EQGFX(lexer);
        return 0xa5;
    case 0x0a:
        countTokenColumns_EQGFX(lexer);
        return 0x23;
    case 0x0b:
        countTokenColumns_EQGFX(lexer);
        return 0xa6;
    case 0x0c:
        countTokenColumns_EQGFX(lexer);
        return 0xa7;
    case 0x0d:
        countTokenColumns_EQGFX(lexer);
        return 0xa8;
    case 0x0e:
        countTokenColumns_EQGFX(lexer);
        return 0xa9;
    case 0x0f:
        countTokenColumns_EQGFX(lexer);
        return 0xaa;
    case 0x10:
        countTokenColumns_EQGFX(lexer);
        return 0xab;
    case 0x11:
        countTokenColumns_EQGFX(lexer);
        return 0x24;
    case 0x12:
        countTokenColumns_EQGFX(lexer);
        return 0x25;
    case 0x13:
        countTokenColumns_EQGFX(lexer);
        return 0x2;
    case 0x14:
        countTokenColumns_EQGFX(lexer);
        return 0xac;
    case 0x15:
        countTokenColumns_EQGFX(lexer);
        return 0x12d;
    case 0x16:
        countTokenColumns_EQGFX(lexer);
        return 0x12e;
    case 0x17:
        countTokenColumns_EQGFX(lexer);
        return 0x124;
    case 0x18:
        countTokenColumns_EQGFX(lexer);
        return 0x26;
    case 0x19:
        countTokenColumns_EQGFX(lexer);
        return 0x27;
    case 0x1a:
        countTokenColumns_EQGFX(lexer);
        return 0x28;
    case 0x1b:
        countTokenColumns_EQGFX(lexer);
        return 0xad;
    case 0x1c:
        countTokenColumns_EQGFX(lexer);
        return 0xae;
    case 0x1d:
        countTokenColumns_EQGFX(lexer);
        return 0x135;
    case 0x1e:
        countTokenColumns_EQGFX(lexer);
        return 0x29;
    case 0x1f:
        countTokenColumns_EQGFX(lexer);
        return 0xaf;
    case 0x20:
        countTokenColumns_EQGFX(lexer);
        return 0xb0;
    case 0x21:
        countTokenColumns_EQGFX(lexer);
        return 0xb1;
    case 0x22:
        countTokenColumns_EQGFX(lexer);
        return 0xb2;
    case 0x23:
        countTokenColumns_EQGFX(lexer);
        return 0x82;
    case 0x24:
        countTokenColumns_EQGFX(lexer);
        return 0x2a;
    case 0x25:
        countTokenColumns_EQGFX(lexer);
        return 0x2b;
    case 0x26:
        countTokenColumns_EQGFX(lexer);
        return 0x2c;
    case 0x27:
        countTokenColumns_EQGFX(lexer);
        return 0xb3;
    case 0x28:
        countTokenColumns_EQGFX(lexer);
        return 0x2d;
    case 0x29:
        countTokenColumns_EQGFX(lexer);
        return 0xb4;
    case 0x2a:
        countTokenColumns_EQGFX(lexer);
        return 0x127;
    case 0x2b:
        countTokenColumns_EQGFX(lexer);
        return 0xb5;
    case 0x2c:
        countTokenColumns_EQGFX(lexer);
        return 0x129;
    case 0x2d:
        countTokenColumns_EQGFX(lexer);
        return 0x11d;
    case 0x2e:
        countTokenColumns_EQGFX(lexer);
        return 0xb7;
    case 0x2f:
        countTokenColumns_EQGFX(lexer);
        return 0xb8;
    case 0x30:
        countTokenColumns_EQGFX(lexer);
        return 0x2e;
    case 0x31:
        countTokenColumns_EQGFX(lexer);
        return 0x2f;
    case 0x32:
        countTokenColumns_EQGFX(lexer);
        return 0x30;
    case 0x33:
        countTokenColumns_EQGFX(lexer);
        return 0xb9;
    case 0x34:
        countTokenColumns_EQGFX(lexer);
        return 0x137;
    case 0x35:
        countTokenColumns_EQGFX(lexer);
        return 0xba;
    case 0x36:
        countTokenColumns_EQGFX(lexer);
        return 0xbb;
    case 0x37:
        countTokenColumns_EQGFX(lexer);
        return 0xbc;
    case 0x38:
        countTokenColumns_EQGFX(lexer);
        return 0xbd;
    case 0x39:
        countTokenColumns_EQGFX(lexer);
        return 0xbe;
    case 0x3a:
        countTokenColumns_EQGFX(lexer);
        return 0xbf;
    case 0x3b:
        countTokenColumns_EQGFX(lexer);
        return 0x136;
    case 0x3c:
        countTokenColumns_EQGFX(lexer);
        return 0x12f;
    case 0x3d:
        countTokenColumns_EQGFX(lexer);
        return 0xc0;
    case 0x3e:
        countTokenColumns_EQGFX(lexer);
        return 0xc1;
    case 0x3f:
        countTokenColumns_EQGFX(lexer);
        return 0xc2;
    case 0x40:
        countTokenColumns_EQGFX(lexer);
        return 0xc3;
    case 0x41:
        countTokenColumns_EQGFX(lexer);
        return 0x12c;
    case 0x42:
        countTokenColumns_EQGFX(lexer);
        return 0xc4;
    case 0x43:
        countTokenColumns_EQGFX(lexer);
        return 0x0;
    case 0x44:
        countTokenColumns_EQGFX(lexer);
        return 0xc5;
    case 0x45:
        countTokenColumns_EQGFX(lexer);
        return 0xc6;
    case 0x46:
        countTokenColumns_EQGFX(lexer);
        return 0xc7;
    case 0x47:
        countTokenColumns_EQGFX(lexer);
        return 0xc8;
    case 0x48:
        countTokenColumns_EQGFX(lexer);
        return 0xc9;
    case 0x49:
        countTokenColumns_EQGFX(lexer);
        return 0xca;
    case 0x4a:
        countTokenColumns_EQGFX(lexer);
        return 0xcb;
    case 0x4b:
        countTokenColumns_EQGFX(lexer);
        return 0xcc;
    case 0x4c:
        countTokenColumns_EQGFX(lexer);
        return 0xcd;
    case 0x4d:
        countTokenColumns_EQGFX(lexer);
        return 0xce;
    case 0x4e:
        countTokenColumns_EQGFX(lexer);
        return 0xcf;
    case 0x4f:
        countTokenColumns_EQGFX(lexer);
        return 0xd0;
    case 0x50:
        countTokenColumns_EQGFX(lexer);
        return 0xd3;
    case 0x51:
        countTokenColumns_EQGFX(lexer);
        return 0xd4;
    case 0x52:
        countTokenColumns_EQGFX(lexer);
        return 0xd1;
    case 0x53:
        countTokenColumns_EQGFX(lexer);
        return 0xd2;
    case 0x54:
        countTokenColumns_EQGFX(lexer);
        return 0x120;
    case 0x55:
        countTokenColumns_EQGFX(lexer);
        return 0x122;
    case 0x56:
        countTokenColumns_EQGFX(lexer);
        return 0xd5;
    case 0x57:
        countTokenColumns_EQGFX(lexer);
        return 0xd6;
    case 0x58:
        countTokenColumns_EQGFX(lexer);
        return 0xd8;
    case 0x59:
        countTokenColumns_EQGFX(lexer);
        return 0xd7;
    case 0x5a:
        countTokenColumns_EQGFX(lexer);
        return 0xd9;
    case 0x5b:
        countTokenColumns_EQGFX(lexer);
        return 0xda;
    case 0x5c:
        countTokenColumns_EQGFX(lexer);
        return 0x83;
    case 0x5d:
        countTokenColumns_EQGFX(lexer);
        return 0x31;
    case 0x5e:
        countTokenColumns_EQGFX(lexer);
        return 0xdb;
    case 0x5f:
        countTokenColumns_EQGFX(lexer);
        return 0x134;
    case 0x60:
        countTokenColumns_EQGFX(lexer);
        return 0x32;
    case 0x61:
        countTokenColumns_EQGFX(lexer);
        return 0xdc;
    case 0x62:
        countTokenColumns_EQGFX(lexer);
        return 0x119;
    case 0x63:
        countTokenColumns_EQGFX(lexer);
        return 0xdd;
    case 0x64:
        countTokenColumns_EQGFX(lexer);
        return 0xde;
    case 0x65:
        countTokenColumns_EQGFX(lexer);
        return 0x33;
    case 0x66:
        countTokenColumns_EQGFX(lexer);
        return 0xdf;
    case 0x67:
        countTokenColumns_EQGFX(lexer);
        return 0x34;
    case 0x68:
        countTokenColumns_EQGFX(lexer);
        return 0xe0;
    case 0x69:
        countTokenColumns_EQGFX(lexer);
        return 0xe1;
    case 0x6a:
        countTokenColumns_EQGFX(lexer);
        return 0xe2;
    case 0x6b:
        countTokenColumns_EQGFX(lexer);
        return 0xe3;
    case 0x6c:
        countTokenColumns_EQGFX(lexer);
        return 0xe4;
    case 0x6d:
        countTokenColumns_EQGFX(lexer);
        return 0x35;
    case 0x6e:
        countTokenColumns_EQGFX(lexer);
        return 0xe5;
    case 0x6f:
        countTokenColumns_EQGFX(lexer);
        return 0x11b;
    case 0x70:
        countTokenColumns_EQGFX(lexer);
        return 0x36;
    case 0x71:
        countTokenColumns_EQGFX(lexer);
        return 0xe6;
    case 0x72:
        countTokenColumns_EQGFX(lexer);
        return 0xe7;
    case 0x73:
        countTokenColumns_EQGFX(lexer);
        return 0xe8;
    case 0x74:
        countTokenColumns_EQGFX(lexer);
        return 0x37;
    case 0x75:
        countTokenColumns_EQGFX(lexer);
        return 0xe9;
    case 0x76:
        countTokenColumns_EQGFX(lexer);
        return 0x11d;
    case 0x77:
        countTokenColumns_EQGFX(lexer);
        return 0x11c;
    case 0x78:
        countTokenColumns_EQGFX(lexer);
        return 0x38;
    case 0x79:
        countTokenColumns_EQGFX(lexer);
        return 0x12a;
    case 0x7a:
        countTokenColumns_EQGFX(lexer);
        return 0xea;
    case 0x7b:
        countTokenColumns_EQGFX(lexer);
        return 0x39;
    case 0x7c:
        countTokenColumns_EQGFX(lexer);
        return 0x3a;
    case 0x7d:
        countTokenColumns_EQGFX(lexer);
        return 0x3b;
    case 0x7e:
        countTokenColumns_EQGFX(lexer);
        return 0xeb;
    case 0x7f:
        countTokenColumns_EQGFX(lexer);
        return 0x3c;
    case 0x80:
        countTokenColumns_EQGFX(lexer);
        return 0x3d;
    case 0x81:
        countTokenColumns_EQGFX(lexer);
        return 0x3e;
    case 0x82:
        countTokenColumns_EQGFX(lexer);
        return 0x3f;
    case 0x83:
        countTokenColumns_EQGFX(lexer);
        return 0x40;
    case 0x84:
        countTokenColumns_EQGFX(lexer);
        return 0x41;
    case 0x85:
        countTokenColumns_EQGFX(lexer);
        return 0x42;
    case 0x86:
        countTokenColumns_EQGFX(lexer);
        return 0x43;
    case 0x87:
        countTokenColumns_EQGFX(lexer);
        return 0x44;
    case 0x88:
        countTokenColumns_EQGFX(lexer);
        return 0x45;
    case 0x89:
        countTokenColumns_EQGFX(lexer);
        return 0xec;
    case 0x8a:
        countTokenColumns_EQGFX(lexer);
        return 0x47;
    case 0x8b:
        countTokenColumns_EQGFX(lexer);
        return 0x46;
    case 0x8c:
        countTokenColumns_EQGFX(lexer);
        return 0x11e;
    case 0x8d:
        countTokenColumns_EQGFX(lexer);
        return 0x48;
    case 0x8e:
        countTokenColumns_EQGFX(lexer);
        return 0x49;
    case 0x8f:
        countTokenColumns_EQGFX(lexer);
        return 0x4a;
    case 0x90:
        countTokenColumns_EQGFX(lexer);
        return 0x4b;
    case 0x91:
        countTokenColumns_EQGFX(lexer);
        return 0x4c;
    case 0x92:
        countTokenColumns_EQGFX(lexer);
        return 0xed;
    case 0x93:
        countTokenColumns_EQGFX(lexer);
        return 0x4d;
    case 0x94:
        countTokenColumns_EQGFX(lexer);
        return 0x4e;
    case 0x95:
        countTokenColumns_EQGFX(lexer);
        return 0x4f;
    case 0x96:
        countTokenColumns_EQGFX(lexer);
        return 0xee;
    case 0x97:
        countTokenColumns_EQGFX(lexer);
        return 0xef;
    case 0x98:
        countTokenColumns_EQGFX(lexer);
        return 0xf0;
    case 0x99:
        countTokenColumns_EQGFX(lexer);
        return 0xf1;
    case 0x9a:
        countTokenColumns_EQGFX(lexer);
        return 0xf2;
    case 0x9b:
        countTokenColumns_EQGFX(lexer);
        return 0x50;
    case 0x9c:
        countTokenColumns_EQGFX(lexer);
        return 0xf3;
    case 0x9d:
        countTokenColumns_EQGFX(lexer);
        return 0xf4;
    case 0x9e:
        countTokenColumns_EQGFX(lexer);
        return 0xf5;
    case 0x9f:
        countTokenColumns_EQGFX(lexer);
        return 0xf6;
    case 0xa0:
        countTokenColumns_EQGFX(lexer);
        return 0xf7;
    case 0xa1:
        countTokenColumns_EQGFX(lexer);
        return 0xf8;
    case 0xa2:
        countTokenColumns_EQGFX(lexer);
        return 0xf9;
    case 0xa3:
        countTokenColumns_EQGFX(lexer);
        return 0x51;
    case 0xa4:
        countTokenColumns_EQGFX(lexer);
        return 0xfa;
    case 0xa5:
        countTokenColumns_EQGFX(lexer);
        return 0x52;
    case 0xa6:
        countTokenColumns_EQGFX(lexer);
        return 0x53;
    case 0xa7:
        countTokenColumns_EQGFX(lexer);
        return 0xfb;
    case 0xa8:
        countTokenColumns_EQGFX(lexer);
        return 0x12b;
    case 0xa9:
        countTokenColumns_EQGFX(lexer);
        return 0xfc;
    case 0xaa:
        countTokenColumns_EQGFX(lexer);
        return 0x54;
    case 0xab:
        countTokenColumns_EQGFX(lexer);
        lexer->unknown_3434 = 8;
        return 0x55;
    case 0xac:
        countTokenColumns_EQGFX(lexer);
        return 0xfd;
    case 0xad:
        countTokenColumns_EQGFX(lexer);
        return 0xff;
    case 0xae:
        countTokenColumns_EQGFX(lexer);
        return 0xfe;
    case 0xaf:
        countTokenColumns_EQGFX(lexer);
        return 0x56;
    case 0xb0:
        countTokenColumns_EQGFX(lexer);
        return 0x125;
    case 0xb1:
        countTokenColumns_EQGFX(lexer);
        return 0x101;
    case 0xb2:
        countTokenColumns_EQGFX(lexer);
        return 0x57;
    case 0xb3:
        countTokenColumns_EQGFX(lexer);
        return 0x5a;
    case 0xb4:
        countTokenColumns_EQGFX(lexer);
        return 0x102;
    case 0xb5:
        countTokenColumns_EQGFX(lexer);
        return 0x107;
    case 0xb6:
        countTokenColumns_EQGFX(lexer);
        return 0x109;
    case 0xb7:
        countTokenColumns_EQGFX(lexer);
        return 0x108;
    case 0xb8:
        countTokenColumns_EQGFX(lexer);
        return 0x11a;
    case 0xb9:
        countTokenColumns_EQGFX(lexer);
        return 0x103;
    case 0xba:
        countTokenColumns_EQGFX(lexer);
        return 0x58;
    case 0xbb:
        countTokenColumns_EQGFX(lexer);
        return 0x59;
    case 0xbc:
        countTokenColumns_EQGFX(lexer);
        return 0x104;
    case 0xbd:
        countTokenColumns_EQGFX(lexer);
        return 0x105;
    case 0xbe:
        countTokenColumns_EQGFX(lexer);
        return 0x11f;
    case 0xbf:
        countTokenColumns_EQGFX(lexer);
        return 0x121;
    case 0xc0:
        countTokenColumns_EQGFX(lexer);
        return 0x106;
    case 0xc1:
        countTokenColumns_EQGFX(lexer);
        return 0x123;
    case 0xc2:
        countTokenColumns_EQGFX(lexer);
        return 0x10a;
    case 0xc3:
        countTokenColumns_EQGFX(lexer);
        return 0x131;
    case 0xc4:
        countTokenColumns_EQGFX(lexer);
        return 0x132;
    case 0xc5:
        countTokenColumns_EQGFX(lexer);
        return 0x5b;
    case 0xc6:
        countTokenColumns_EQGFX(lexer);
        return 0x10b;
    case 0xc7:
        countTokenColumns_EQGFX(lexer);
        return 0x5c;
    case 0xc8:
        countTokenColumns_EQGFX(lexer);
        return 0x10c;
    case 0xc9:
        countTokenColumns_EQGFX(lexer);
        return 0x128;
    case 0xca:
        countTokenColumns_EQGFX(lexer);
        return 0x5d;
    case 0xcb:
        countTokenColumns_EQGFX(lexer);
        return 0x10d;
    case 0xcc:
        countTokenColumns_EQGFX(lexer);
        return 0x10e;
    case 0xcd:
        countTokenColumns_EQGFX(lexer);
        return 0x130;
    case 0xce:
        countTokenColumns_EQGFX(lexer);
        return 0x5e;
    case 0xcf:
        countTokenColumns_EQGFX(lexer);
        return 0x5f;
    case 0xd0:
        countTokenColumns_EQGFX(lexer);
        return 0x10f;
    case 0xd1:
        countTokenColumns_EQGFX(lexer);
        lexer->unknown_3434 = 4;
        break;
    case 0xd2:
        countTokenColumns_EQGFX(lexer);
        return 0x60;
    case 0xd3:
        countTokenColumns_EQGFX(lexer);
        return 0x61;
    case 0xd4:
        countTokenColumns_EQGFX(lexer);
        return 0x62;
    case 0xd5:
        countTokenColumns_EQGFX(lexer);
        return 0x63;
    case 0xd6:
        countTokenColumns_EQGFX(lexer);
        return 0x64;
    case 0xd7:
        countTokenColumns_EQGFX(lexer);
        return 0x112;
    case 0xd8:
        countTokenColumns_EQGFX(lexer);
        return 0x65;
    case 0xd9:
        countTokenColumns_EQGFX(lexer);
        return 0x111;
    case 0xda:
        countTokenColumns_EQGFX(lexer);
        return 0x113;
    case 0xdb:
        countTokenColumns_EQGFX(lexer);
        return 0x66;
    case 0xdc:
        countTokenColumns_EQGFX(lexer);
        return 0x114;
    case 0xdd:
        countTokenColumns_EQGFX(lexer);
        return 0x67;
    case 0xde:
        countTokenColumns_EQGFX(lexer);
        return 0x115;
    case 0xdf:
        countTokenColumns_EQGFX(lexer);
        return 0x116;
    case 0xe0:
        countTokenColumns_EQGFX(lexer);
        return 0x6d;
    case 0xe1:
        countTokenColumns_EQGFX(lexer);
        return 0x6b;
    case 0xe2:
        countTokenColumns_EQGFX(lexer);
        return 0x126;
    case 0xe3:
        countTokenColumns_EQGFX(lexer);
        return 0x6a;
    case 0xe4:
        countTokenColumns_EQGFX(lexer);
        return 0x6c;
    case 0xe5:
        countTokenColumns_EQGFX(lexer);
        return 0x68;
    case 0xe6:
        countTokenColumns_EQGFX(lexer);
        return 0x117;
    case 0xe7:
        countTokenColumns_EQGFX(lexer);
        return 0x69;
    case 0xe8:
        countTokenColumns_EQGFX(lexer);
        return 0x133;
    case 0xe9:
        countTokenColumns_EQGFX(lexer);
        return 0xb6;
    case 0xea:
        countTokenColumns_EQGFX(lexer);
        return 0x190;
    case 0xeb:
        countTokenColumns_EQGFX(lexer);
        return 0x191;
    case 0xec:
        countTokenColumns_EQGFX(lexer);
        return 0x19b;
    case 0xed:
        countTokenColumns_EQGFX(lexer);
        return 0x19c;
    case 0xee:
        countTokenColumns_EQGFX(lexer);
        return 0x192;
    case 0xef:
        countTokenColumns_EQGFX(lexer);
        return 0x193;
    case 0xf0:
        countTokenColumns_EQGFX(lexer);
        return 0x194;
    case 0xf1:
        countTokenColumns_EQGFX(lexer);
        return 0x195;
    case 0xf2:
        countTokenColumns_EQGFX(lexer);
        return 0x196;
    case 0xf3:
        countTokenColumns_EQGFX(lexer);
        return 0x197;
    case 0xf4:
        countTokenColumns_EQGFX(lexer);
        return 0x198;
    case 0xf5:
        countTokenColumns_EQGFX(lexer);
        return 0x199;
    case 0xf6:
        countTokenColumns_EQGFX(lexer);
        return 0x19a;
    case 0xf7:
        countTokenColumns_EQGFX(lexer);
        return 0x19d;
    case 0xf8:
        countTokenColumns_EQGFX(lexer);
        return 0x19e;
    case 0xf9:
        countTokenColumns_EQGFX(lexer);
        return 0x19f;
    case 0xfa:
        countTokenColumns_EQGFX(lexer);
        return 0x1a3;
    case 0xfb:
        countTokenColumns_EQGFX(lexer);
        return 0x1a4;
    case 0xfc:
        countTokenColumns_EQGFX(lexer);
        return 0x1a1;
    case 0xfd:
        countTokenColumns_EQGFX(lexer);
        return 0x1a2;
    case 0xfe:
        countTokenColumns_EQGFX(lexer);
        return 0x1a5;
    case 0xff:
        countTokenColumns_EQGFX(lexer);
        return 0x1a6;
    case 0x100:
        countTokenColumns_EQGFX(lexer);
        return 0x1a0;
    case 0x101:
        countTokenColumns_EQGFX(lexer);
        return 0x1a7;
    case 0x102:
        countTokenColumns_EQGFX(lexer);
        return 0x1a9;
    case 0x103:
        countTokenColumns_EQGFX(lexer);
        return 0x1a8;
    case 0x104:
        countTokenColumns_EQGFX(lexer);
        return 0x1aa;
    case 0x105:
        countTokenColumns_EQGFX(lexer);
        return 0x1ab;
    case 0x106:
        countTokenColumns_EQGFX(lexer);
        return 0x1ac;
    case 0x107:
        countTokenColumns_EQGFX(lexer);
        return 0x1ad;
    case 0x108:
        countTokenColumns_EQGFX(lexer);
        return 0x1ae;
    case 0x109:
        countTokenColumns_EQGFX(lexer);
        return 0x1af;
    case 0x10a:
        countTokenColumns_EQGFX(lexer);
        return 0x1b0;
    case 0x10b:
        countTokenColumns_EQGFX(lexer);
        return 0x1b1;
    case 0x10c:
        countTokenColumns_EQGFX(lexer);
        return 0x1b2;
    case 0x10d:
        countTokenColumns_EQGFX(lexer);
        return 0x1b3;
    case 0x10e:
        countTokenColumns_EQGFX(lexer);
        return 0x1b4;
    case 0x10f:
        countTokenColumns_EQGFX(lexer);
        return 0x1b5;
    case 0x110:
        countTokenColumns_EQGFX(lexer);
        return 0x1b6;
    case 0x111:
        countTokenColumns_EQGFX(lexer);
        return 0x1b7;
    case 0x112:
        countTokenColumns_EQGFX(lexer);
        return 0x1b8;
    case 0x113:
        countTokenColumns_EQGFX(lexer);
        return 0x1b9;
    case 0x114:
        countTokenColumns_EQGFX(lexer);
        return 0x1ba;
    case 0x115:
        countTokenColumns_EQGFX(lexer);
        return 0x1bb;
    case 0x116:
        countTokenColumns_EQGFX(lexer);
        return 0x1bc;
    case 0x117:
        countTokenColumns_EQGFX(lexer);
        return 0x1bd;
    case 0x118:
        countTokenColumns_EQGFX(lexer);
        return 0x1be;
    case 0x119:
        countTokenColumns_EQGFX(lexer);
        return 0x1bf;
    case 0x11a:
        countTokenColumns_EQGFX(lexer);
        return 0x1c0;
    case 0x11b:
        countTokenColumns_EQGFX(lexer);
        return 0x1c1;
    case 0x11c:
        countTokenColumns_EQGFX(lexer);
        return 0x1c2;
    case 0x11d:
        countTokenColumns_EQGFX(lexer);
        return 0x1c3;
    case 0x11e:
        countTokenColumns_EQGFX(lexer);
        return 0x1c4;
    case 0x11f:
        countTokenColumns_EQGFX(lexer);
        return 0x1c5;
    case 0x120:
        countTokenColumns_EQGFX(lexer);
        return 0x1c6;
    case 0x121:
        countTokenColumns_EQGFX(lexer);
        return 0x1c7;
    case 0x122:
        countTokenColumns_EQGFX(lexer);
        return 0x1c8;
    case 0x123:
        countTokenColumns_EQGFX(lexer);
        return 0x1c9;
    case 0x124:
        countTokenColumns_EQGFX(lexer);
        return 0x1ca;
    case 0x125:
        countTokenColumns_EQGFX(lexer);
        return 0x1cb;
    case 0x126:
        countTokenColumns_EQGFX(lexer);
        return 0x1cc;
    case 0x127:
        countTokenColumns_EQGFX(lexer);
        return 0x1cd;
    case 0x128:
        countTokenColumns_EQGFX(lexer);
        return 0x1ce;
    case 0x129:
        countTokenColumns_EQGFX(lexer);
        return 0x1cf;
    case 0x12a:
        countTokenColumns_EQGFX(lexer);
        return 0x1d0;
    case 0x12b:
        countTokenColumns_EQGFX(lexer);
        return 0x1d1;
    case 0x12c:
        countTokenColumns_EQGFX(lexer);
        return 0x1d2;
    case 0x12d:
        countTokenColumns_EQGFX(lexer);
        return 0x1d3;
    case 0x12e:
        countTokenColumns_EQGFX(lexer);
        return 0x1d4;
    case 0x12f:
        countTokenColumns_EQGFX(lexer);
        return 0x1d5;
    case 0x130:
        countTokenColumns_EQGFX(lexer);
        return 0x1d6;
    case 0x131:
        countTokenColumns_EQGFX(lexer);
        return 0x1d7;
    case 0x132:
        countTokenColumns_EQGFX(lexer);
        return 0x1d8;
    case 0x133:
        countTokenColumns_EQGFX(lexer);
        return 0x1d9;
    case 0x134:
        countTokenColumns_EQGFX(lexer);
        return 0x1da;
    case 0x135:
        countTokenColumns_EQGFX(lexer);
        return 0x1db;
    case 0x136:
        countTokenColumns_EQGFX(lexer);
        return 0x1dc;
    case 0x137:
        countTokenColumns_EQGFX(lexer);
        return 0x1dd;
    case 0x138:
        countTokenColumns_EQGFX(lexer);
        return 0x1de;
    case 0x139:
        countTokenColumns_EQGFX(lexer);
        return 0x1df;
    case 0x13a:
        countTokenColumns_EQGFX(lexer);
        return 0x1e0;
    case 0x13b:
        countTokenColumns_EQGFX(lexer);
        return 0x1e1;
    case 0x13c:
        countTokenColumns_EQGFX(lexer);
        return 0x1e2;
    case 0x13d:
        countTokenColumns_EQGFX(lexer);
        return 0x1e3;
    case 0x13e:
        countTokenColumns_EQGFX(lexer);
        return 0x1e4;
    case 0x13f:
        countTokenColumns_EQGFX(lexer);
        return 0x1e5;
    case 0x140:
        countTokenColumns_EQGFX(lexer);
        return 0x1e6;
    case 0x141:
        countTokenColumns_EQGFX(lexer);
        return 0x1e7;
    case 0x142:
        countTokenColumns_EQGFX(lexer);
        return 0x1e8;
    case 0x143:
        countTokenColumns_EQGFX(lexer);
        return 0x1e9;
    case 0x144:
        countTokenColumns_EQGFX(lexer);
        return 0x1ea;
    case 0x145:
        countTokenColumns_EQGFX(lexer);
        return 0x1eb;
    case 0x146:
        countTokenColumns_EQGFX(lexer);
        return 0x1ec;
    case 0x147:
        countTokenColumns_EQGFX(lexer);
        return 0x1ed;
    case 0x148:
        countTokenColumns_EQGFX(lexer);
        return 0x1ee;
    case 0x149:
        countTokenColumns_EQGFX(lexer);
        return 0x1ef;
    case 0x14a:
        countTokenColumns_EQGFX(lexer);
        return 0x1f0;
    case 0x14b:
        countTokenColumns_EQGFX(lexer);
        return 0x1f1;
    case 0x14c:
        countTokenColumns_EQGFX(lexer);
        return 0x1f2;
    case 0x14d:
        countTokenColumns_EQGFX(lexer);
        return 0x1f3;
    case 0x14e:
        countTokenColumns_EQGFX(lexer);
        return 0x1f4;
    case 0x14f:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x0;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x150:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x6;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x151:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x152:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0xa;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x153:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x12;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x154:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x16;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x155:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x156:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x157:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0xb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x158:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x13;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x159:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x17;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x15a:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x43;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x15b:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x63;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x15c:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x15d:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x6b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x15e:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x53;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x15f:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x73;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x160:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x57;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x161:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x77;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x162:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x107;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x163:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x164:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x10b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x165:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x113;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x166:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x117;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x167:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x143;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x168:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x163;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x169:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x14b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x16a:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x16b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x16b:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x153;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x16c:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x173;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x16d:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x157;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x16e:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x177;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x16f:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x187;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x170:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x183;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x171:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x18b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x172:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x193;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x173:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x197;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x174:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1c3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x175:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1e3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x176:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1cb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x177:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1eb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x178:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1d3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x179:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1f3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x17a:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1d7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x17b:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x1f7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x17c:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x207;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x17d:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x17e:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x20b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x17f:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x213;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x180:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x217;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x181:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x243;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x182:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x263;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x183:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x24b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x184:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x26b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x185:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x253;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x186:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x273;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x187:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x257;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x188:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x277;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x189:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x287;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x18a:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x283;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x18b:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x28b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x18c:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x293;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x18d:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x297;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x18e:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2c3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x18f:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2e3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x190:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2cb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x191:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2eb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x192:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2d3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x193:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2f3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x194:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2d7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x195:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x2f7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x196:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x307;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x197:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x198:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x30b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x199:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x313;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x19a:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x317;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x19b:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x343;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x19c:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x363;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x19d:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x34b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x19e:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x36b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x19f:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x353;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a0:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x373;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a1:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x357;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a2:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x377;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a3:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x407;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a4:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a5:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x40b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a6:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x413;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a7:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x417;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a8:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x443;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1a9:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x463;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1aa:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x44b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1ab:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x46b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1ac:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x453;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1ad:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x473;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1ae:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x457;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1af:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x477;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b0:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x487;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b1:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x483;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b2:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x48b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b3:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x493;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b4:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x497;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b5:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4c3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b6:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4e3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b7:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4cb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b8:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4eb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1b9:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4d3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1ba:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4f3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1bb:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4d7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1bc:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x4f7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1bd:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x507;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1be:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1bf:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x50b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c0:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x513;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c1:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x517;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c2:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x543;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c3:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x563;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c4:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x54b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c5:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x56b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c6:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x553;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c7:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x573;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c8:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x557;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1c9:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x577;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1ca:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x587;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1cb:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x583;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1cc:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x58b;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1cd:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x593;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1ce:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x597;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1cf:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5c3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d0:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5e3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d1:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5cb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d2:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5eb;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d3:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5d3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d4:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5f3;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d5:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5d7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d6:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = 0x5f7;
        lexer->unknown_3434 = 0;
        return 0x70;
    case 0x1d7:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = (int)0x80000000;
        lexer->unknown_3434 = 0;
        return 0x6e;
    case 0x1d8:
        break;
    case 0x1da:
        logParserError_EQGFX(context, s_ascii_scanner_illegal_character_EQGFX);
        return 1;
    case 0x1db:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = (int)copyTokenText_EQGFX(lexer);
        return (context->value_0c != 0) ? 4 : 1;
    case 0x1dc:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = atoi_EQGFX(text);
        return 10;
    case 0x1dd:
    case 0x1de:
    case 0x1df:
        countTokenColumns_EQGFX(lexer);
        *(float *)&context->value_0c = (float)atof_EQGFX(text);
        return 12;
    case 0x1e0:
        countTokenColumns_EQGFX(lexer);
        context->value_0c = (int)copyQuotedTokenText_EQGFX(lexer);
        return (context->value_0c != 0) ? 0x90 : 1;
    case 0x1e1:
        break;
    case 0x1e3:
        logParserError_EQGFX(context, s_ascii_scanner_illegal_character_EQGFX);
        return 1;
    case 0x1e4:
        break;
    case 0x1e5:
        if (lexer->state == 0)
        {
            lexer->state = 1;
            break;
        }
        context->value_0c = (int)decodeHexToken_EQGFX(context, text);
        lexer->state = 0;
        lexer->unknown_3434 = 0;
        return (context->value_0c != 0) ? 0x98 : 1;
    case 0x1e6:
        context->value_0c = (int)copyQuotedTokenText_EQGFX(lexer);
        lexer->state = 0;
        lexer->unknown_3434 = 0;
        return (context->value_0c != 0) ? 0x98 : 1;
    }
    text[lexer->unknown_343c] = lexer->unknown_3440[0];
    length = lexer->unknown_343c;
    goto restart;
}

// FUNCTION: EQGFX 0x10028651
// Prepares parser state for text input.
void * __cdecl createTextFileParser_guess(void *parserContext, void *fileContext)
{
    TextFileParser_guess *parser;

    parser = (TextFileParser_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(TEXT_FILE_PARSER_SIZE);
    if (parser == 0)
    {
        logParserError_EQGFX(parserContext, s_out_of_memory_EQGFX);
        return 0;
    }
    parser->parseFunction = parseTextFileFunction_EQGFX;
    parser->fileContext = fileContext;
    parser->parserContext = parserContext;
    parser->releaseFunction = releaseTextFileFunction_EQGFX;
    parser->enabled = 1;
    parser->state = 0;
    parser->textStart = (char *)parser->textStorage;
    parser->textCurrent = (char *)parser->textStorage;
    parser->unknown_3430 = 0;
    parser->unknown_3434 = 0;
    parser->tabWidth = 10;
    parser->unknown_343c = 0;
    return parser;
}

// FUNCTION: EQGFX 0x100286C6
// Feeds text characters to the world-file lexer.
int __cdecl readLexerChar_guess(TextFileParser_guess *parser)
{
    char *nextChar;
    int result;

    nextChar = parser->textStart;
    if (nextChar < parser->textCurrent)
    {
        result = (int)*nextChar;
        parser->textStart = nextChar + 1;
    }
    else
    {
        result = readTextFileChunk_EQGFX(parser->fileContext, parser->textStorage, TEXT_READ_CHUNK_SIZE);
        if (result < 1)
        {
            if (result < 0)
            {
                logParserError_EQGFX(parser->parserContext, s_ascii_scanner_read_error_EQGFX);
            }
            result = -1;
        }
        else
        {
            parser->textCurrent = (char *)parser->textStorage + result;
            parser->textStart = (char *)parser->textStorage + 1;
            result = (char)parser->textStorage[0];
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1002872D
// Skips the rest of the current text line.
void __cdecl skipLexerLine_guess(TextFileParser_guess *parser)
{
    int ch;

    do
    {
        ch = readLexerChar_EQGFX(parser);
        if (ch == '\n')
        {
            return;
        }
    } while (ch > 0);
}

// FUNCTION: EQGFX 0x10028741
// Tracks text columns across line breaks and tabs for parser positions.
char * advanceTokenColumn_guess(TextCursorState_guess *textState)
{
    char currentCharacter;
    int column;
    char *textPosition;
    char *result;

    textPosition = textState->text;
    currentCharacter = *textPosition;
    result = textPosition;
    while (currentCharacter != '\0')
    {
        currentCharacter = *textPosition;
        result = (char *)(((unsigned long)result & 0xffffff00UL) | (unsigned char)currentCharacter);
        if (currentCharacter == '\n')
        {
            textState->column = 0;
        }
        else if (currentCharacter == '\t')
        {
            column = textState->column;
            result = (char *)(column / TEXT_TAB_WIDTH);
            textState->column = (column - column % TEXT_TAB_WIDTH) + TEXT_TAB_WIDTH;
        }
        else
        {
            textState->column++;
        }
        textPosition++;
        currentCharacter = *textPosition;
    }
    return result;
}

// FUNCTION: EQGFX 0x1002878A
// Preserves the current text value for use by the parser.
// __cdecl. Returns a heap copy of the parser's name string (+0x2028), or NULL after logging out of memory
// through the parser context (+0x18).
char * copyTokenText_guess(TextFileParser_guess *parser)
{
    unsigned int nameLength;
    char *nameCopy;

    nameLength = strlen_EQGFX((const char *)parser->unknown_2028);
    nameCopy = (char *)GFX_COMPOSITE_ALLOCATE_MEMORY(nameLength + 1);
    if (nameCopy == 0)
    {
        logParserError_EQGFX(parser->parserContext, s_out_of_memory_EQGFX);
        return 0;
    }
    GFX_PATH_COPY_STRING(nameCopy, (const char *)parser->unknown_2028);
    return nameCopy;
}

// FUNCTION: EQGFX 0x100287CE
// Preserves the interior text of a delimited parser value.
// __cdecl. Returns a heap copy of the parser's name string (+0x2028) without its first and last character,
// or NULL after logging out of memory through the parser context (+0x18). Names under 2 chars write outside the copy: bug kept.
char * copyQuotedTokenText_guess(TextFileParser_guess *parser)
{
    unsigned int nameLength;
    char *nameCopy;

    nameLength = strlen_EQGFX((const char *)parser->unknown_2028);
    nameCopy = (char *)GFX_COMPOSITE_ALLOCATE_MEMORY(nameLength);
    if (nameCopy == 0)
    {
        logParserError_EQGFX(parser->parserContext, s_out_of_memory_EQGFX);
        nameCopy = 0;
    }
    else
    {
        GFX_PATH_COPY_STRING(nameCopy, (const char *)parser->unknown_2028 + 1);
        nameLength = strlen_EQGFX(nameCopy);
        nameCopy[nameLength - 1] = '\0';
    }
    return nameCopy;
}

// FUNCTION: EQGFX 0x1002881E
// Decodes hexadecimal byte data from text input for the renderer's file parser.
// __cdecl. Decodes a hex string (two digits per byte, high digit first) into a heap copy of half its length. An odd length,
// a failed allocation or a non-hex digit logs the reason through the parser context and returns NULL; a bad digit leaks the copy: bug kept.
char * t3dDecodeHexBytes_guess(void *parserContext, const char *hexText)
{
    int length;
    char *result;
    const char *cursor;
    char *output;
    unsigned char highDigit;
    unsigned char lowDigit;

    length = strlen_EQGFX(hexText);
    if (length % 2 == 0)
    {
        result = (char *)GFX_COMPOSITE_ALLOCATE_MEMORY(length / 2);
        if (result == 0)
        {
            result = (char *)s_out_of_memory_EQGFX;
        }
        else
        {
            if (length < 1)
                return result;
            cursor = hexText + 1;
            output = result;
            while (1)
            {
                highDigit = hexDigitValue_EQGFX(cursor[-1]);
                lowDigit = hexDigitValue_EQGFX(*cursor);
                if (highDigit > 0xf || lowDigit > 0xf)
                    break;
                *output = highDigit * 16 + lowDigit;
                cursor += 2;
                output++;
                if (length <= (int)(cursor - hexText - 1))
                    return result;
            }
            result = (char *)s_ascii_scanner_invalid_hex_EQGFX;
        }
    }
    else
    {
        result = (char *)s_ascii_scanner_invalid_number_EQGFX;
    }
    logParserError_EQGFX(parserContext, result);
    return 0;
}

// FUNCTION: EQGFX 0x100288bf
// Converts one ASCII hex digit to its numeric value for parsing.
// __cdecl ASCII hex digit to value: '0'..'9' -> 0..9, 'A'..'F' and 'a'..'f' -> 10..15, and 0x10 for any
// other byte. Leaf text-parsing helper (t3dDecodeHexBytes_guess, a parser, calls it twice); no include/ struct here.
unsigned char hexDigitValue_guess(unsigned char c)
{
    if (c >= 0x30)
    {
        if (c > 0x39)
        {
            if (c <= 0x40)
                return 0x10;
            if (c > 0x46)
            {
                if (c <= 0x60)
                    return 0x10;
                if (c > 0x66)
                    return 0x10;
                c = c - 0x57;
                return c;
            }
            c = c - 0x37;
            return c;
        }
        c = c - 0x30;
        return c;
    }
    return 0x10;
}

// FUNCTION: EQGFX 0x100288e7
// __cdecl Allocates a 0x1c-byte parser and stores its owner and source; out of memory logs and returns 0.
BinaryFileParser_guess *createBinaryFileParser_guess(void *owner, void *source)
{
    BinaryFileParser_guess *parser;

    parser = (BinaryFileParser_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(0x1c);
    if (parser == 0)
    {
        logParserError_EQGFX(owner, s_out_of_memory_EQGFX);
        return 0;
    }
    parser->unknown_10 = 0;
    parser->source = source;
    parser->parseFunction = (void *)0x1002897D;
    parser->releaseFunction = (void *)0x10029020;
    parser->owner = owner;
    return parser;
}

// FUNCTION: EQGFX 0x10028926
// __cdecl Peeks 6 bytes for the binary magic; rewinds to the saved position and returns 0 on mismatch.
int __cdecl hasBinaryFileSignature_guess(void *source)
{
    char tag[8];
    unsigned int savedPosition;
    int matched;

    savedPosition = EQ_CDECL(unsigned int, 0x10026525, (void *))(source);
    if (EQ_CDECL(unsigned int, 0x1002645C, (void *, void *, unsigned int))(source, tag, 6) == 6
        && memcmp(tag, EQ_ARRAY(char, 0x100762B8), 6) == 0)
        matched = 1;
    else
    {
        matched = 0;
        EQ_CDECL(void, 0x100264CA, (void *, unsigned int, int))(source, savedPosition, 0);
    }
    return matched;
}

// FUNCTION: EQGFX 0x1002897D
// Reads binary input tokens and stores their associated values for parsing.
unsigned int __cdecl t3dScanBinaryGetToken(BinaryFileParser_guess *parser)
{
    BinaryFileParser_guess *parserForReads;
    int readResult;
    int token;
    unsigned int byteCount;
    const char *errorMessage;

    parserForReads = parser;
    readResult = EQ_CDECL(int, 0x1002645C, (void *, void *, unsigned int))(
        parserForReads->source, (char *)&parser + 3, 1);
    if (readResult < 1)
    {
        if (readResult == 0)
            return 0;
        errorMessage = s_binary_scanner_read_failed_EQGFX;
        goto logError;
    }
    token = ((unsigned char *)&parser)[3];
    if (token < 0x80)
    {
        if (token <= 0x0c)
        {
            if (token == 0x0c)
            {
                readResult = EQ_CDECL(int, 0x10028C12, (BinaryFileParser_guess *, float *))(
                    parserForReads, (float *)&((ParserObject_guess *)parserForReads->owner)->value_0c);
                return (-(unsigned int)(readResult != 0) & 0xfffffff5) + 0x0c;
            }
            if (token == 0x00)
                goto invalidToken;
            if (token == 0x03)
                return 0;
            if (token == 0x04)
            {
                readResult = EQ_CDECL(int, 0x10028C3B, (BinaryFileParser_guess *, void **, int))(
                    parserForReads, (void **)&((ParserObject_guess *)parserForReads->owner)->value_0c, 1);
                return (-(unsigned int)(readResult != 0) & 0xfffffffd) + 0x04;
            }
            if (token == 0x08)
            {
                byteCount = 1;
                goto readInteger;
            }
            if (token == 0x09)
            {
                byteCount = 2;
                goto readInteger;
            }
            if (token == 0x0a)
            {
                byteCount = 4;
                goto readInteger;
            }
        }
        else
        {
            if (token == 0x10)
            {
                readResult = EQ_CDECL(int, 0x10028C3B, (BinaryFileParser_guess *, void **, int))(
                    parserForReads, (void **)&((ParserObject_guess *)parserForReads->owner)->value_0c, 1);
                return (-(unsigned int)(readResult != 0) & 0xffffff71) + 0x90;
            }
            if (token == 0x18)
            {
                byteCount = 1;
                goto readHunkString;
            }
            if (token == 0x1c)
            {
                byteCount = 1;
                goto readPlainString;
            }
            if (token == 0x6e)
            {
                ((ParserObject_guess *)parserForReads->owner)->value_0c = (int)0x80000000;
                return 0x6e;
            }
            if (token == 0x6f)
            {
                byteCount = 2;
                goto readExtendedInteger;
            }
            if (token == 0x70)
            {
                byteCount = 4;
                goto readExtendedInteger;
            }
        }
        if (token <= 0x70)
            return token;
    }
    else
    {
        readResult = EQ_CDECL(int, 0x10028D14, (BinaryFileParser_guess *, void *, unsigned int))(
            parserForReads, (char *)&parser + 3, 1);
        if (readResult != 0)
            return 1;
        token = (token & 0x7f) * 0x100 + ((unsigned char *)&parser)[3];
        if (token != 0)
        {
            if (token == 0x90)
            {
                readResult = EQ_CDECL(int, 0x10028C3B, (BinaryFileParser_guess *, void **, int))(
                    parserForReads, (void **)&((ParserObject_guess *)parserForReads->owner)->value_0c, 2);
                return (-(unsigned int)(readResult != 0) & 0xffffff71) + 0x90;
            }
            if (token == 0x98)
            {
                byteCount = 2;
                goto readHunkString;
            }
            if (token == 0x9c)
            {
                byteCount = 2;
                goto readPlainString;
            }
            if (token > 0x7f && token < 0x138)
                return token;
        }
    }
    goto invalidToken;

readInteger:
    readResult = EQ_CDECL(int, 0x10028BAC, (BinaryFileParser_guess *, int *, unsigned int))(
        parserForReads, &((ParserObject_guess *)parserForReads->owner)->value_0c, byteCount);
    return (-(unsigned int)(readResult != 0) & 0xfffffff7) + 0x0a;

readExtendedInteger:
    readResult = EQ_CDECL(int, 0x10028BAC, (BinaryFileParser_guess *, int *, unsigned int))(
        parserForReads, &((ParserObject_guess *)parserForReads->owner)->value_0c, byteCount);
    return (-(unsigned int)(readResult != 0) & 0xffffff91) + 0x70;

readHunkString:
    readResult = EQ_CDECL(int, 0x10028C3B, (BinaryFileParser_guess *, void **, int))(
        parserForReads, (void **)&((ParserObject_guess *)parserForReads->owner)->value_0c, byteCount);
    return (-(unsigned int)(readResult != 0) & 0xffffff69) + 0x98;

readPlainString:
    readResult = EQ_CDECL(int, 0x10028CBC, (BinaryFileParser_guess *, void **, int))(
        parserForReads, (void **)&((ParserObject_guess *)parserForReads->owner)->value_0c, byteCount);
    return (-(unsigned int)(readResult != 0) & 0xffffff65) + 0x9c;

invalidToken:
    errorMessage = s_binary_scanner_invalid_token_EQGFX;
logError:
    logParserError_EQGFX(parserForReads->owner, errorMessage);
    return 1;
}

#pragma intrinsic(memset)

// FUNCTION: EQGFX 0x10028BAC
// __cdecl Reads byteCount bytes and sign-extends to a 32-bit out value; returns 0 on success, 1 on failure.
int __cdecl readBinarySignedInteger_guess(BinaryFileParser_guess *parser, int *outValue, unsigned int byteCount)
{
    unsigned int count = byteCount;
    unsigned char fill;

    if (EQ_CDECL(int, 0x10028D14, (BinaryFileParser_guess *, void *, unsigned int))(parser, &byteCount, count) != 0)
        return 1;
    fill = (unsigned char)((((char *)&byteCount)[count - 1] >= 0) - 1);
    if (count < 4)
        memset((char *)&byteCount + count, fill, 4 - count);
    *outValue = (int)byteCount;
    return 0;
}

#pragma function(memset)

// FUNCTION: EQGFX 0x10028c12
// __cdecl Reads one float through the binary parser into outValue; returns 0 on success, 1 on failure.
int __cdecl readBinaryFloat_guess(BinaryFileParser_guess *parser, float *outValue)
{
    if (EQ_CDECL(int, 0x10028D14, (BinaryFileParser_guess *, void *, unsigned int))(parser, &parser, sizeof(float)) != 0)
        return 1;
    *outValue = *(float *)&parser;
    return 0;
}

// FUNCTION: EQGFX 0x10028c3b
// Decodes an encoded binary string for use by the world-file parser.
// __cdecl Reads a 1- or 2-byte length prefix, then that many bytes into a fresh block and XORs them with the 8-byte key at 0x100762C0; returns 0 on success, 1 on failure.
int __cdecl t3dReadLengthPrefixedXorBytes_guess(BinaryFileParser_guess *parser, void **outBuffer, int width)
{
    unsigned short prefix;
    unsigned int length;
    unsigned int keyIndex;
    unsigned char *cursor;
    void *block;

    keyIndex = 0;
    if (EQ_CDECL(int, 0x10028D14, (BinaryFileParser_guess *, void *, unsigned int))(parser, &prefix, width) == 0)
    {
        length = (unsigned char)prefix;
        if (width > 1)
            length = ((unsigned char)prefix << 8) + ((unsigned char *)&prefix)[1];
        block = GFX_COMPOSITE_ALLOCATE_MEMORY(length + 1);
        *outBuffer = block;
        if (block != 0 && EQ_CDECL(int, 0x10028D14, (BinaryFileParser_guess *, void *, unsigned int))(parser, block, length) == 0)
        {
            cursor = (unsigned char *)*outBuffer;
            for (; length != 0; length = length - 1)
            {
                *cursor = *cursor ^ t3dHunkKey_EQGFX[keyIndex];
                cursor = cursor + 1;
                keyIndex = keyIndex + 1 & 7;
            }
            *cursor = 0;
            return 0;
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x10028cbc
// Supplies a raw binary data block to the world-file parser.
// __cdecl Reads a 1- or 2-byte length prefix then that many bytes into a fresh block; returns 0 on success, 1 on failure.
int __cdecl readBinaryLengthPrefixedBlock_guess(BinaryFileParser_guess *parser, void **outBuffer, int width)
{
    unsigned short prefix;
    unsigned int length;
    void *block;

    if (EQ_CDECL(int, 0x10028D14, (BinaryFileParser_guess *, void *, unsigned int))(parser, &prefix, 4) != 0)
        return 1;
    length = (unsigned char)prefix;
    if (width > 1)
        length = (length << 8) + ((unsigned char *)&prefix)[1];
    block = GFX_COMPOSITE_ALLOCATE_MEMORY(length);
    *outBuffer = block;
    if (block == 0)
        return 1;
    return EQ_CDECL(int, 0x10028D14, (BinaryFileParser_guess *, void *, unsigned int))(parser, block, length) != 0;
}

// FUNCTION: EQGFX 0x10028d14
// __cdecl Reads count bytes into buffer; returns 0 on full read, else logs and returns 1.
int __cdecl readBinaryDataWithErrorHandling_guess(BinaryFileParser_guess *parser, void *buffer, unsigned int count)
{
    unsigned int bytesRead;
    const char *message;

    bytesRead = EQ_CDECL(unsigned int, 0x1002645C, (void *, void *, unsigned int))(parser->source, buffer, count);
    if (bytesRead == count)
        return 0;
    message = s_binary_scanner_incomplete_token_EQGFX;
    if (bytesRead != 0)
        message = s_binary_scanner_read_error_EQGFX;
    logParserError_EQGFX(parser->owner, message);
    return 1;
}

// FUNCTION: EQGFX 0x10028d54
// Creates a source reader with a supplied or environment-configured search directory.
WorldFileReader_guess *__cdecl t3dCreateWorldFileReader_guess(void *owner, void *source, char *searchDirectory)
{
    WorldFileReader_guess *reader;
    unsigned long environmentLength;
    char localSearchDirectory[264];

    reader = (WorldFileReader_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(0x14);
    if (reader == 0)
    {
        logParserError_EQGFX(owner, s_out_of_memory_EQGFX);
    }
    else
    {
        reader->searchDirectory = 0;
        reader->owner = owner;
        reader->parseFunction = (void *)0x10028E56;
        reader->releaseFunction = (void *)0x10028E82;
        reader->parseStatus = initializeWorldFileReader_EQGFX(reader, source);
        if (reader->parseStatus != 0)
        {
            if (searchDirectory == 0)
            {
                EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))(localSearchDirectory, 0, 0x105);
                if (GFX_WLD_PATH == 0)
                    return reader;
                environmentLength = GetEnvironmentVariableA_EQGFX(GFX_WLD_PATH, localSearchDirectory, 0x105);
                if (environmentLength == 0)
                    return reader;
                searchDirectory = localSearchDirectory;
                if (localSearchDirectory == 0)
                    return reader;
            }
            reader->searchDirectory = (char *)GFX_COMPOSITE_ALLOCATE_MEMORY(strlen_EQGFX(searchDirectory) + 1);
            if (reader->searchDirectory == 0)
                return reader;
            GFX_PATH_COPY_STRING(reader->searchDirectory, searchDirectory);
            return reader;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(reader);
    }
    return 0;
}

// FUNCTION: EQGFX 0x10028E20
// Releases the queued file sources when a world-file reader is finished.
// __cdecl (the callee pops 0 bytes; one stack argument). Calls advanceFileQueue_guess while the current entry has a next entry,
// releases the last entry, frees the +0x10 block when set, then frees the owner.
void destroyFileQueueOwner_guess(FileQueueOwner_guess *owner)
{
    FileQueueEntry_guess *entry;

    while (owner->currentEntry->next != 0)
        EQ_CDECL(void, 0x10028FB4, (FileQueueOwner_guess *))(owner);
    entry = owner->currentEntry;
    entry->release(entry);
    if (owner->unknown_10 != 0)
        EQGFX_T3D_FREE_MEMORY_GUESS(owner->unknown_10);
    EQGFX_T3D_FREE_MEMORY_GUESS(owner);
    return;
}

// FUNCTION: EQGFX 0x10028E56
// Finds the next queued file source ready for parser input.
// __cdecl (the callee pops 0 bytes; one stack argument). Polls the current entry; while the poll is <= 0 and the entry has a next,
// calls advanceFileQueue_guess and polls the current entry again. Returns the first positive poll result, or 0 when the queue runs out.
int pollFileQueue_guess(FileQueueOwner_guess *owner)
{
    int pollResult;

    pollResult = owner->currentEntry->poll(owner->currentEntry);
    while (pollResult <= 0)
    {
        if (owner->currentEntry->next == 0)
        {
            pollResult = 0;
            break;
        }
        EQ_CDECL(void, 0x10028FB4, (FileQueueOwner_guess *))(owner);
        pollResult = owner->currentEntry->poll(owner->currentEntry);
    }
    return pollResult;
}

// FUNCTION: EQGFX 0x10028E82
// Adds an included world file to the active parser's source queue.
// __cdecl (the callee pops 0 bytes; two stack arguments). Opens fileName relative to the parser's current source, else on the search path.
// Success links the opened source in as the owner's head entry and returns 0; failure logs "could not open include file" and returns 1.
int _t3dLexOpenIncludeFile(FileQueueOwner_guess *owner, const char *fileName)
{
    void *source;
    char *resolvedName;
    FileQueueEntry_guess *entry;

    source = openSourceFile_EQGFX(fileName, owner->parserContext->currentSource);
    if (source == 0)
    {
        if (owner->parserContext->currentSource == 0)
        {
            resolvedName = EQ_CDECL(char *, 0x10028EF8, (FileQueueOwner_guess *, const char *))(owner, fileName);
            if (resolvedName != 0)
            {
                source = openSourceFile_EQGFX(resolvedName, 0);
                if (source != 0)
                    goto createEntry;
            }
        }
        logParserMessage_EQGFX(owner->parserContext, s_could_not_open_include_file_EQGFX, fileName);
    }
    else
    {
createEntry:
        entry = EQ_CDECL(FileQueueEntry_guess *, 0x10028F44, (FileQueueOwner_guess *, void *))(owner, source);
        if (entry != 0)
        {
            entry->next = owner->currentEntry;
            owner->currentEntry = entry;
            return 0;
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x10028ef8
// Resolves a filename under the configured search directory.
char *findFileOnSearchPath_guess(SearchPathContext_guess *searchContext, const char *fileName)
{
    unsigned long searchResult;
    const char *searchDirectory;
    char *resultBuffer;

    searchDirectory = searchContext->searchDirectory;
    if (searchDirectory != 0)
    {
        resultBuffer = searchPathBuffer_EQGFX;
        searchResult = SearchPathA_EQGFX(searchDirectory, fileName, 0, 0x105, resultBuffer, (char **)&searchContext);
        if (searchResult != 0)
            return resultBuffer;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10028f2e
// Returns the pointer found in a nested record; its higher-level purpose is unclear.
// __cdecl (the callee pops 0 bytes and never reads ECX; one stack argument). Two pointer hops, both at
// +0xc: the argument's +0xc slot points at a record whose +0xc slot is the object pointer this returns
// (a parser accessor, next to getLineNumberFromObject; no include/ struct has this layout).
void *getObjectPointerFromOffset_guess(int object)
{
    return *(void **)(*(int *)(object + 0xc) + 0xc);
}

// FUNCTION: EQGFX 0x10028f39
// Returns the nested record's line number for parser diagnostics.
// __cdecl (the callee pops 0 bytes and never reads ECX; one stack argument). Same two hops as
// getObjectPointerFromOffset, but here the record's +0x10 slot is the line number this returns
// (a parser accessor; no include/ struct has this layout).
int getLineNumberFromObject_guess(int object)
{
    return *(int *)(*(int *)(object + 0xc) + 0x10);
}

// FUNCTION: EQGFX 0x10028f44
// __cdecl picks a binary or text parser for the source, logging on failure.
void *_t3dLexCreateScannerContext(ParserFactory_guess *factory, void *source)
{
    void *parser;

    if (EQ_CDECL(int, 0x10028926, (void *))(source) != 0)
    {
        parser = EQ_CDECL(void *, 0x100288E7, (void *, void *))(factory->parserContext, source);
    }
    else
    {
        if (EQ_CDECL(int, 0x10026565, (void *))(source) == 0)
        {
            logParserError_EQGFX(factory->parserContext, s_unrecognized_file_format_EQGFX);
            return 0;
        }
        parser = EQ_CDECL(void *, 0x10028651, (void *, void *))(factory->parserContext, source);
    }
    if (parser == 0)
    {
        logParserError_EQGFX(factory->parserContext, s_out_of_memory_EQGFX);
    }
    return parser;
}

// FUNCTION: EQGFX 0x10028fb4
// Moves the parser to the next queued file source.
void advanceFileQueue_guess(FileQueueOwner_guess *owner)
{
    FileQueueEntry_guess *entry;
    FileQueueEntry_guess *nextEntry;

    entry = owner->currentEntry;
    nextEntry = entry->next;
    if (nextEntry != 0)
    {
        owner->currentEntry = nextEntry;
        EQ_CDECL(void, 0x1002642D, (void *))(entry->fileObject);
        EQ_CDECL(void, 0x10026411, (void *))(entry->fileObject);
        entry->release(entry);
    }
    return;
}

// FUNCTION: EQGFX 0x10028fdf
// __cdecl Allocates a 0x40-byte parser and stores its two callbacks; out of memory logs and returns 0.
ParserObject_guess *createParserObject_guess(void *parseFunction, void *releaseFunction)
{
    ParserObject_guess *parser;

    parser = (ParserObject_guess *)GFX_COMPOSITE_ALLOCATE_MEMORY(0x40);
    if (parser == 0)
    {
        logParserError_EQGFX(0, s_out_of_memory_EQGFX);
        return 0;
    }
    parser->unknown_08 = 0;
    parser->parseFunction = parseFunction;
    parser->releaseFunction = releaseFunction;
    parser->unknown_24 = 0;
    parser->unknown_28 = 0;
    parser->unknown_2c = 0;
    parser->unknown_34 = 0;
    parser->unknown_38 = 0;
    parser->unknown_3c = 0;
    return parser;
}

// FUNCTION: EQGFX 0x10029020
// Releases allocated parser storage.
// __cdecl (the callee pops 0 bytes; one stack argument). Frees the argument through t3dFreeMemory
// (0x10050CBB); no caller reads EAX.
void freeFileParser_guess(void *memory)
{
    EQGFX_T3D_FREE_MEMORY_GUESS(memory);
}
