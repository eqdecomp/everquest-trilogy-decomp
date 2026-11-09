// unit_10008D60.cpp - eqmain.dll, the address run 0x10008D60..0x1000E6D0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).
// Third-party code: this file reconstructs, from the binary, functions of zlib 1.1.2 (Copyright (C) 1995-1998
// Jean-loup Gailly and Mark Adler) that eqmain.dll links. It is ALTERED: written from the compiled code, not
// the original zlib source (names and layouts follow zlib's). See THIRD_PARTY.md.

#include <stdlib.h>
#include <string.h>

#include "trilogy/eqaddr.h"

#ifdef EQD_MATCH
/* The match build declares eqd_va_* only for EQ_GLOBAL() written out in the source; these EQ_ARRAY() globals are not, and
   the whole-file match build stops at the first one (line 1149). Same convention as EQ_Overlay.cpp. */
extern char eqd_va_0x10046608;
extern char eqd_va_0x10046620;
extern char eqd_va_0x10046660;
extern char eqd_va_0x10046680;
extern char eqd_va_0x10046694;
extern char eqd_va_0x100466AC;
extern char eqd_va_0x1004663C;
extern char eqd_va_0x10040EB0;
extern char eqd_va_0x10041250;
extern char eqd_va_0x10046740;
extern char eqd_va_0x10047740;
extern char eqd_va_0x10047930;
extern char eqd_va_0x10041264;
extern char eqd_va_0x100416E4;
extern char eqd_va_0x1004195C;
extern char eqd_va_0x10041118;
extern char eqd_va_0x10041A5C;
extern char eqd_va_0x1004175C;
extern char eqd_va_0x1004185C;
extern char eqd_va_0x1004118C;
extern char eqd_va_0x10041AD0;
#endif

// The record the callback is read from. Only the two fields this function reads are named; the rest is not known.
struct CallbackRec_guess
{
    char unknown_00[0x24];          // 0x00 .. 0x23: not read here
    int (__cdecl *fn)(int, int);    // 0x24: the callback
    int arg;                        // 0x28: its first argument
};

// The rectangle ExtTextOutA clips to, kept in the frame with the Windows RECT layout: left is the running x, top
// the row's top, right the segment's clip edge, bottom the row's bottom.
struct TextRect_guess
{
    int left;       // 0x00
    int top;        // 0x04
    int right;      // 0x08
    int bottom;     // 0x0C
};

// GDI's SIZE: GetTextExtentPoint32A writes the measured text's extent here.
struct TextSize_guess
{
    int cx;         // 0x00
    int cy;         // 0x04
};

// The line record 0x10009080 builds (12 bytes): its style byte, the line's width so far and its 0xa0-byte text buffer.
struct TextLine_guess
{
    unsigned char style;    // 0x00
    int width;              // 0x04
    char *text;             // 0x08
};

// GDI32 imports, called through their import slots (`call dword ptr [slot]`), and this image's global used by
// 0x10008D60 (named by VA).
typedef int (__stdcall *SetTextColor_guess_t)(int hdc, int color);
typedef int (__stdcall *SetBkMode_guess_t)(int hdc, int mode);
typedef int (__stdcall *SelectObject_guess_t)(int hdc, int obj);
typedef int (__stdcall *ExtTextOutA_guess_t)(int hdc, int x, int y, unsigned int options, TextRect_guess *rect,
                                             const char *text, unsigned int count, int *dx);
typedef int (__stdcall *GetTextExtentPoint32A_guess_t)(int hdc, const char *text, int count, TextSize_guess *size);
#define PTR_SetTextColor_guess  EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)
#define PTR_SetBkMode_guess     EQ_GLOBAL(SetBkMode_guess_t, 0x10040014)
#define PTR_SelectObject_guess  EQ_GLOBAL(SelectObject_guess_t, 0x10040028)
#define PTR_ExtTextOutA_guess   EQ_GLOBAL(ExtTextOutA_guess_t, 0x10040024)
#define PTR_GetTextExtentPoint32A_guess EQ_GLOBAL(GetTextExtentPoint32A_guess_t, 0x10040038)
#define g_parent_guess EQ_GLOBAL(unsigned int, 0x1004CEB8)
// The address 0x10046608 (a one-byte global, passed by address): the row WrapTextLines_guess hands over for a blank line.
#define g_chat_suffix_guess EQ_ARRAY(char, 0x10046608)

// Slots 0x44 and 0x68 of an object's vtable (the object's first dword). __stdcall: the callee pops its two
// arguments, the caller does not.
typedef void (__stdcall *Slot44_guess_t)(int *obj, int **out);  // 0x44: writes its result to *out (the DC here)
typedef void (__stdcall *Slot68_guess_t)(int *obj, int *dc);    // 0x68: ends the drawing with the DC

// FUNCTION: EQMAIN 0x10008D60
// Draws the full front-end text block within its screen box.
// @early-stop: the frame is 0x18 instead of 0x14; VC6 assigns block/x to EBP/EBX instead of EBX/EBP.
// __cdecl. Draws a text block. The box comes first (BltFastSrcRect_guess), then obj2 = block[0]->+0xc gives the DC through
// its slot 0x44, which writes it over textBlock; then the text colour and background mode, and the rows from
// block[0xa] down: each row selects its font and draws its segments left to right (EQMain_SetTextColorFromCode_guess with the
// segment's colour byte, then ExtTextOutA clipped to the segment's rect). Rows stop once a row's bottom reaches
// block[4]. Ends with obj2's slot 0x68 and the DC. Returns nothing (the callers never read EAX).
void __cdecl DrawTextBlock_guess(int *textBlock)
{
    int value;
    int row;
    int segmentNode;
    unsigned char *segment;
    int currentLeft;
    int *block;
    unsigned int textLength;
    const char *pCVar10;
    TextRect_guess rc;

    EQ_CDECL(void, 0x10006C20, (unsigned int, int, int, int, int, int, int, int, int))(
        g_parent_guess, textBlock[1], textBlock[2], textBlock[0xc], 0, 0, textBlock[3] - textBlock[1],
        textBlock[4] - textBlock[2], 0);
    block = textBlock;
    rc.top = block[2];
    rc.bottom = block[0xf] + rc.top;
    (*(Slot44_guess_t *)(**(int **)(*block + 0xc) + 0x44))(*(int **)(*block + 0xc), &textBlock);
    PTR_SetTextColor_guess((int)textBlock, block[0xd]);
    PTR_SetBkMode_guess((int)textBlock, 1);
    row = block[10];
    do {
        if (row == 0 || rc.bottom >= block[4]) {
            (*(Slot68_guess_t *)(**(int **)(*block + 0xc) + 0x68))(*(int **)(*block + 0xc), textBlock);
            return;
        }
        segmentNode = *(int *)(row + 4);
        PTR_SelectObject_guess((int)textBlock, *(int *)(segmentNode + 4));
        rc.left = block[1];
        currentLeft = rc.left;
        for (segmentNode = *(int *)(segmentNode + 0xc); segmentNode != 0; segmentNode = *(int *)(segmentNode + 8)) {
            segment = *(unsigned char **)(segmentNode + 4);
            EQ_CDECL(void, 0x10008AC0, (int, unsigned char))((int)textBlock, *segment);
            if (*(int *)(segmentNode + 8) != 0) {
                rc.right = block[3];
                value = *(int *)(segment + 4) + 2 + currentLeft;
                if (value <= rc.right)
                    rc.right = value;
            } else {
                rc.right = block[3];
            }
            pCVar10 = *(const char **)(segment + 8);
            textLength = (unsigned int)strlen(pCVar10);
            PTR_ExtTextOutA_guess((int)textBlock, currentLeft + 2, rc.top, 4, &rc, pCVar10, textLength, 0);
            rc.left = rc.right;
            currentLeft = rc.left;
        }
        value = block[0xf];
        rc.top = rc.top + value;
        rc.bottom = rc.bottom + value;
        row = *(int *)(row + 8);
    } while (1);
}

// FUNCTION: EQMAIN 0x10008ED0
// Draws only the text row keyed to the requested screen line.
// @early-stop: frame is 0x18 instead of 0x14; VC6 keeps the block pointer and gate in different registers and lays out the calls differently.
// __cdecl. Draws the rows of a text block whose line key is y. The gate block[0x10] (when set) needs its index (+0x68)
// at or past its last one (+0x64 - 1); y must lie in [key, min(block[6], block[0xb] + key)), key being the first row's
// line key. Then the rows from block[0xa] on, each block[0xf] lower than the last, draw when their key is y: the DC
// comes from block[0]->+0xc's slot 0x44 over textBlock, then the text colour, background mode and font are set, each
// segment is drawn left to right (EQMain_SetTextColorFromCode_guess with its colour byte, then ExtTextOutA clipped to its rect), and
// slot 0x68 ends the draw. Returns nothing.
void __cdecl DrawTextBlockLineAt_guess(int *textBlock, int rowNumber)
{
    int *block;
    int *row;
    int *line;
    int *item;
    int *seg;
    int *obj2;
    int key;
    int limit;
    int seg_right;
    const char *text;
    TextRect_guess rc;
    int y;

    block = textBlock;
    if (block[0x10] != 0 &&
        *(int *)(block[0x10] + 0x68) < *(int *)(block[0x10] + 0x64) - 1)
        return;

    row = (int *)block[0xa];
    line = (int *)row[1];
    key = line[0];
    limit = block[0xb] + key;
    if (block[6] < limit)
        limit = block[6];
    y = rowNumber;
    if (y < key || y >= limit)
        return;

    rc.top = block[2];
    rc.bottom = block[2] + block[0xf];
    while (row != 0) {
        line = (int *)row[1];
        if (y == line[0]) {
            obj2 = *(int **)(block[0] + 0xc);
            (*(Slot44_guess_t *)(*(int *)obj2 + 0x44))(obj2, &textBlock);
            PTR_SetTextColor_guess((int)textBlock, block[0xd]);
            PTR_SetBkMode_guess((int)textBlock, 1);
            PTR_SelectObject_guess((int)textBlock, line[1]);
            rc.left = block[1];
            for (item = (int *)line[3]; item != 0; item = (int *)item[2]) {
                seg = (int *)item[1];
                EQ_CDECL(void, 0x10008AC0, (int, unsigned char))((int)textBlock, *(unsigned char *)seg);
                if (item[2] != 0) {
                    seg_right = seg[1] + rc.left + 2;
                    if (block[3] < seg_right)
                        rc.right = block[3];
                    else
                        rc.right = seg_right;
                } else {
                    rc.right = block[3];
                }
                text = (const char *)seg[2];
                PTR_ExtTextOutA_guess((int)textBlock, rc.left + 2, rc.top, 4, &rc, text, strlen(text), 0);
                rc.left = rc.right;
            }
            obj2 = *(int **)(block[0] + 0xc);
            (*(Slot68_guess_t *)(*(int *)obj2 + 0x68))(obj2, textBlock);
        }
        rc.top += block[0xf];
        rc.bottom += block[0xf];
        row = (int *)row[2];
    }
}

// FUNCTION: EQMAIN 0x10009060
// Moves the text block to its next row and redraws the block.
// __cdecl. Moves the block's current row (block[0xa]) on to the row's +0x8 link, then draws the block through
// 0x10008D60. Returns nothing (the caller never reads EAX).
void __cdecl StepTextBlockRow_guess(int *block)
{
    block[10] = *(int *)(block[10] + 8);
    EQ_CDECL(void, 0x10008D60, (int *))(block);
}

// FUNCTION: EQMAIN 0x10009080
// Appends text to a scrolling text box: pixel-width word wrap, <x> style tags, scrolls and drops the oldest row
// __cdecl. Word-wraps the text at str into the unit's rows. A "<x>" tag starts a new line and sets its style byte
// (the low byte of block, reused as a local). Each character is measured with GetTextExtentPoint32A on the DC the
// unit's object hands out (slot 0x44, released through slot 0x68). A line that outgrows the width is cut back to its
// last space and flushed through DrawTextBlockLineAt_guess; a line that reaches 0xa0 characters gets a fresh buffer; the last line
// is flushed at the end. Returns nothing (the caller never reads EAX).
void __cdecl WrapTextIntoRows_guess(int *block, char *str)
{
    unsigned char *tagPtr;
    char ch;
    int *node;
    int *blk;
    int *list;
    unsigned int *buf;
    unsigned int initialStyle;
    void *nodeData;
    char *tmp;
    char *cut;
    char *cur;
    int value;
    TextLine_guess *line;
    int *rec;
    char *ptr;
    int *dcHandle;
    int widthSum;
    char *dst;
    int nChars;
    TextSize_guess sz;

    blk = block;
    rec = 0;
    line = 0;
    initialStyle = EQ_CDECL(unsigned int, 0x10008BB0, (int))(block[0xd]);
    *(unsigned char *)&block = (unsigned char)initialStyle;
    ptr = str;
    cur = str;
    if (*str != '\0')
    {
        do
        {
            if (*cur == '<' && cur[2] == '>')
            {
                tagPtr = (unsigned char *)(cur + 1);
                cur = cur + 3;
                line = 0;
                *(unsigned char *)&block = *tagPtr;
                ptr = cur;
                if (*cur == '\0')
                    break;
            }
            if (rec == 0)
            {
                rec = (int *)EQ_CDECL(void *, 0x1003342E, (size_t))(0x18);
                rec[0] = blk[6];
                blk[6] = blk[6] + 1;
                rec[1] = blk[0xe];
                rec[2] = blk[0xf];
                EQ_CDECL(void, 0x10028330, (int *))(rec + 3);
                EQ_CDECL(void, 0x10028340, (int *, int, int *))(blk + 7, 0, rec);
                widthSum = 0;
                if (blk[10] == 0)
                    blk[10] = blk[7];
                if (blk[0xb] < blk[6] &&
                    ((value = blk[0x10]) == 0 || *(int *)(value + 100) - 1 <= *(int *)(value + 0x68)))
                    EQ_CDECL(void, 0x10009060, (int *))(blk);
                if (blk[5] < blk[6])
                {
                    if (blk[10] == blk[7])
                        EQ_CDECL(void, 0x10009060, (int *))(blk);
                    list = (int *)(*(int *)(blk[7] + 4) + 0xc);
                    node = (int *)*list;
                    while (node != 0)
                    {
                        EQ_CDECL(void, 0x10033568, (void *))(*(void **)(node[1] + 8));
                        nodeData = (void *)EQ_CDECL(int, 0x10028430, (int *, int *))(list, node);
                        EQ_CDECL(void, 0x10033568, (void *))(nodeData);
                        node = (int *)*list;
                    }
                    nodeData = (void *)EQ_CDECL(int, 0x10028430, (int *, int *))(blk + 7, (int *)blk[7]);
                    EQ_CDECL(void, 0x10033568, (void *))(nodeData);
                    cur = ptr;
                }
            }
            value = nChars;
            if (line == 0)
            {
                line = (TextLine_guess *)EQ_CDECL(void *, 0x1003342E, (size_t))(0xc);
                line->style = *(unsigned char *)&block;
                buf = (unsigned int *)EQ_CDECL(void *, 0x1003342E, (size_t))(0xa0);
                line->text = (char *)buf;
                line->width = 0;
                for (value = 0x28; value != 0; value = value - 1)
                {
                    *buf = 0;
                    buf = buf + 1;
                }
                EQ_CDECL(void, 0x10028340, (int *, int, int *))(rec + 3, 0, (int *)line);
                dst = line->text;
                value = 0;
            }
            (*(Slot44_guess_t *)(**(int **)(*blk + 0xc) + 0x44))(*(int **)(*blk + 0xc), &dcHandle);
            PTR_SelectObject_guess((int)dcHandle, rec[1]);
            PTR_GetTextExtentPoint32A_guess((int)dcHandle, cur, 1, &sz);
            (*(Slot68_guess_t *)(**(int **)(*blk + 0xc) + 0x68))(*(int **)(*blk + 0xc), dcHandle);
            widthSum = widthSum + sz.cx;
            nChars = value + 1;
            if ((blk[3] - blk[1]) - 4 < widthSum)
            {
                tmp = EQ_CDECL(char *, 0x10033670, (char *, int))(line->text, 0x20);
                if (tmp != 0)
                {
                    if (str < cur)
                        tmp = cur - 1;
                    else
                        tmp = 0;
                    ch = *tmp;
                    while (ch != ' ')
                    {
                        if (str < cur)
                        {
                            ptr = cur - 1;
                            if (str < ptr)
                                cur = cur - 2;
                            else
                                cur = 0;
                        }
                        else
                        {
                            ptr = 0;
                            cur = 0;
                        }
                        ch = *cur;
                        cur = ptr;
                    }
                    tmp = line->text;
                    cut = tmp + strlen(tmp) - 1;
                    ch = *cut;
                    while (ch != ' ')
                    {
                        if (tmp < cut)
                            cut = cut - 1;
                        else
                            cut = 0;
                        ch = *cut;
                    }
                    *cut = '\0';
                }
                EQ_CDECL(void, 0x10008ED0, (int *, int))(blk, rec[0]);
                line = 0;
                rec = 0;
            }
            else if (nChars < 0xa0)
            {
                line->width = line->width + sz.cx;
                ch = *cur;
                if (ch != '\r' && ch != '\t')
                {
                    *dst = ch;
                    dst = dst + 1;
                }
                cur = cur + 1;
                ptr = cur;
            }
            else
            {
                line = 0;
            }
        } while (*cur != '\0');
        if (rec != 0)
            EQ_CDECL(void, 0x10008ED0, (int *, int))(blk, rec[0]);
    }
}

// FUNCTION: EQMAIN 0x10009370
// Loads newline-separated text into a text block.
// __cdecl. Hands the text at str to WrapTextIntoRows_guess (0x10009080) one line at a time. A '\n' that starts a line
// hands over the address 0x10046608 instead (a blank row); otherwise the line runs to the next '\n', which becomes a NUL,
// and the line is handed over. Stops after the line that has no '\n'. Returns nothing (the caller never reads EAX).
void __cdecl WrapTextLines_guess(int *block, char *src)
{
    char ch;
    char *eol;
    char *_Str;
    char *pcNext;

    do
    {
        ch = *src;
        _Str = src;
        while (ch == '\n')
        {
            EQ_CDECL(void, 0x10009080, (int *, char *))(block, g_chat_suffix_guess);
            eol = _Str + 1;
            _Str = _Str + 1;
            ch = *eol;
        }
        pcNext = 0;
        eol = EQ_CDECL(char *, 0x10033670, (char *, int))(_Str, '\n');
        if (eol != 0)
        {
            pcNext = eol + 1;
            *eol = '\0';
        }
        EQ_CDECL(void, 0x10009080, (int *, char *))(block, _Str);
        src = pcNext;
    } while (pcNext != 0);
}

// FUNCTION: EQMAIN 0x100093D0
// Lays a string out as width-limited display rows in a text block.
// @early-stop: one instruction is reordered in the no-text-before-cursor branch (zero EAX before moving the cursor to EDI).
// __cdecl. Lays the text at str out as records on block's row list (+0x1c), the wrap of WrapTextIntoRows_guess
// (0x10009080) without its tags and flushes. A record (0x18 bytes: its number, font and colour, and its segment list at
// +0xc) takes characters while the running width stays within (block[3] - block[1]) - 4; an overflow cuts the segment
// back to its last space (if any) and closes the record: the text resumes after that space, or at the overflowing
// character. A segment takes at most 0xa0 characters. The DC comes from slot 0x44 of block[0]->+0xc (selected with
// block[0xe]) and slot 0x68 of that object ends it.
// Returns nothing (the caller never reads EAX).
void __cdecl WrapTextIntoRecords_guess(int *block, char *str)
{
    int *blk;
    int *rec;
    TextLine_guess *line;
    unsigned int *buf;
    unsigned int initialStyle;
    char *tmp;
    char *cut;
    char ch;
    int i;
    TextSize_guess sz;
    char *out;
    int widthSum;
    int *hdc;
    int nChars;
    char *text;

    blk = block;
    rec = 0;
    line = 0;
    initialStyle = EQ_CDECL(unsigned int, 0x10008BB0, (int))(blk[0xd]);
    *(unsigned char *)&block = (unsigned char)initialStyle;
    text = str;
    (*(Slot44_guess_t *)(**(int **)(*blk + 0xc) + 0x44))(*(int **)(*blk + 0xc), &hdc);
    PTR_SelectObject_guess((int)hdc, blk[0xe]);
    if (*text != '\0')
    {
        do
        {
            if (rec == 0)
            {
                rec = (int *)EQ_CDECL(void *, 0x1003342E, (size_t))(0x18);
                rec[0] = blk[6];
                blk[6] = blk[6] + 1;
                rec[1] = blk[0xe];
                rec[2] = blk[0xf];
                EQ_CDECL(void, 0x10028330, (int *))(rec + 3);
                EQ_CDECL(void, 0x10028340, (int *, int, int *))(blk + 7, 0, rec);
                widthSum = 0;
                if (blk[10] == 0)
                    blk[10] = blk[7];
            }
            if (line == 0)
            {
                line = (TextLine_guess *)EQ_CDECL(void *, 0x1003342E, (size_t))(0xc);
                line->style = *(unsigned char *)&block;
                buf = (unsigned int *)EQ_CDECL(void *, 0x1003342E, (size_t))(0xa0);
                line->text = (char *)buf;
                line->width = 0;
                for (i = 0x28; i != 0; i = i - 1)
                {
                    *buf = 0;
                    buf = buf + 1;
                }
                EQ_CDECL(void, 0x10028340, (int *, int, int *))(rec + 3, 0, (int *)line);
                out = line->text;
                nChars = 0;
            }
            PTR_GetTextExtentPoint32A_guess((int)hdc, text, 1, &sz);
            widthSum = widthSum + sz.cx;
            nChars = nChars + 1;
            if (widthSum > (blk[3] - blk[1]) - 4)
            {
                tmp = EQ_CDECL(char *, 0x10033670, (char *, int))(line->text, 0x20);
                if (tmp != 0)
                {
                    if (str >= text)
                        tmp = 0;
                    else
                        tmp = text - 1;
                    ch = *tmp;
                    while (ch != ' ')
                    {
                        if (str >= text)
                        {
                            text = 0;
                            tmp = text;
                        }
                        else
                        {
                            text = text - 1;
                            if (str >= text)
                                tmp = 0;
                            else
                                tmp = text - 1;
                        }
                        ch = *tmp;
                    }
                    tmp = line->text;
                    cut = tmp + strlen(tmp) - 1;
                    ch = *cut;
                    while (ch != ' ')
                    {
                        if (tmp >= cut)
                            cut = 0;
                        else
                            cut = cut - 1;
                        ch = *cut;
                    }
                    *cut = '\0';
                }
                rec = 0;
                line = 0;
            }
            else
            {
                if (nChars >= 0xa0)
                {
                    line = 0;
                }
                else
                {
                    line->width = line->width + sz.cx;
                    ch = *text;
                    if (ch != '\r' && ch != '\t')
                    {
                        *out = ch;
                        out = out + 1;
                    }
                    text = text + 1;
                }
            }
        } while (*text != '\0');
    }
    (*(Slot68_guess_t *)(**(int **)(*blk + 0xc) + 0x68))(*(int **)(*blk + 0xc), hdc);
}

// FUNCTION: EQMAIN 0x100095E0
// Adds the next line of screen text to a text block.
// __cdecl. Wraps the next line of str onto block's rows and returns the text after it. Each '\r' or '\n' at the
// start wraps as an empty line (the string at 0x10046608); when the byte after it is also '\r' or '\n' it is skipped too.
// The text up to the next '\n' is then cut there and wrapped, and the pointer past that '\n' is returned. With no '\n'
// the rest is wrapped and NULL returned.
char *__cdecl WrapTextNextLine_guess(int *block, char *str)
{
    char ch;
    char *eol;
    char *next;

    while (ch = *str, ch == '\n' || ch == '\r')
    {
        EQ_CDECL(void, 0x100093D0, (int *, char *))(block, g_chat_suffix_guess);
        ch = str[1];
        str = str + 1;
        if (ch == '\n' || ch == '\r')
            str = str + 1;
    }
    eol = EQ_CDECL(char *, 0x10033670, (char *, int))(str, 10);
    if (eol != 0)
    {
        next = eol + 1;
        *eol = '\0';
        EQ_CDECL(void, 0x100093D0, (int *, char *))(block, str);
    }
    else
    {
        next = 0;
        EQ_CDECL(void, 0x100093D0, (int *, char *))(block, str);
    }
    return next;
}

// FUNCTION: EQMAIN 0x10009650
// Computes the running checksum used to validate stream data.
// __cdecl. Adler-32 of len bytes continuing from adler (s1 in its low 16 bits, s2 in its high 16). A NULL buf returns 1.
// Blocks of at most 0x15b0 bytes: whole groups of 16 first (each byte adds into s1, then s1 into s2), the rest one at a
// time; s1 and s2 are reduced mod 0xfff1 after each block. Returns (s2 << 16) | s1.
unsigned int __cdecl calculate_adler32_checksum_guess(unsigned int adler, unsigned char *buf, unsigned int len)
{
    unsigned int s1;
    unsigned int s2;
    int k;
    unsigned int n;

    s1 = adler & 0xffff;
    s2 = adler >> 16;
    if (buf == 0)
    {
        return 1;
    }
    while (len > 0)
    {
        k = len;
        if (len >= 0x15b0)
        {
            k = 0x15b0;
        }
        len -= k;
        if (k >= 16)
        {
            n = (unsigned int)k >> 4;
            k += n * -16;
            do
            {
                s1 += buf[0];
                s2 += s1;
                s1 += buf[1];
                s2 += s1;
                s1 += buf[2];
                s2 += s1;
                s1 += buf[3];
                s2 += s1;
                s1 += buf[4];
                s2 += s1;
                s1 += buf[5];
                s2 += s1;
                s1 += buf[6];
                s2 += s1;
                s1 += buf[7];
                s2 += s1;
                s1 += buf[8];
                s2 += s1;
                s1 += buf[9];
                s2 += s1;
                s1 += buf[10];
                s2 += s1;
                s1 += buf[11];
                s2 += s1;
                s1 += buf[12];
                s2 += s1;
                s1 += buf[13];
                s2 += s1;
                s1 += buf[14];
                s2 += s1;
                s1 += buf[15];
                s2 += s1;
                buf += 16;
            } while (--n);
        }
        if (k != 0)
        {
            do
            {
                s1 += *buf++;
                s2 += s1;
            } while (--k);
        }
        s1 %= 0xfff1;
        s2 %= 0xfff1;
    }
    return (s2 << 16) | s1;
}

// FUNCTION: EQMAIN 0x10009780
// Copies compressed bytes from the pending buffer into the output stream.
// __cdecl, 1 stack arg (the queue unit; its owner is unknown). Copies n = min(unit[+0x10], source[+0x14]) bytes (unsigned
// compare) from the source's cursor source[+0x10] to the unit's write pointer unit[+0xc], then moves both on by n, takes
// n off unit[+0x10] and source[+0x14] and adds it to unit[+0x14]. The source is the object at unit[+0x1c]. When the
// source is then empty its cursor goes back to source[+0x8]. Nothing at all happens when n is 0, so an empty source
// keeps its cursor.
void __cdecl QueueUnitCopyFromSource_guess(int strm)
{
    unsigned int n;
    int source;

    n = *(unsigned int *)(*(int *)(strm + 0x1c) + 0x14);
    if (n > *(unsigned int *)(strm + 0x10))
    {
        n = *(unsigned int *)(strm + 0x10);
    }
    if (n != 0)
    {
        memcpy((void *)*(int *)(strm + 0xc), (void *)*(int *)(*(int *)(strm + 0x1c) + 0x10), n);
        *(unsigned int *)(strm + 0xc) = *(int *)(strm + 0xc) + n;
        *(unsigned int *)(*(int *)(strm + 0x1c) + 0x10) = *(int *)(*(int *)(strm + 0x1c) + 0x10) + n;
        *(unsigned int *)(strm + 0x14) = *(int *)(strm + 0x14) + n;
        *(unsigned int *)(strm + 0x10) = *(int *)(strm + 0x10) - n;
        *(unsigned int *)(*(int *)(strm + 0x1c) + 0x14) = *(int *)(*(int *)(strm + 0x1c) + 0x14) - n;
        source = *(int *)(strm + 0x1c);
        if (*(int *)(source + 0x14) == 0)
        {
            *(int *)(source + 0x10) = *(int *)(source + 8);
        }
    }
}

// FUNCTION: EQMAIN 0x10009800
// Compresses input as stored blocks and sends completed bytes to the output stream.
// @early-stop: one `mov edx, ecx` replaces the original reload from [esi + 0x64]; VC6 forwards the updated strstart value.
// __cdecl, 2 stack args (the queue unit, laid out like zlib 1.1's deflate state; the flush mode, 0 none or 4 finish).
// zlib 1.1's deflate_stored. Its stream is unit[+0x00]. While the lookahead (+0x6c) is 1 or less the window is filled
// (0x10009960); an empty lookahead returns 0 for flush 0, else the last block is cut. strstart (+0x64) takes the lookahead,
// and a stored block is cut (0x1000CD90, then the pending output is copied out by 0x10009780) when strstart reaches
// block_start (+0x54) + max_block, max_block being 0xffff or pending_buf_size (+0x0c) - 5 whichever is smaller, or when
// it is w_size (+0x24) - 262 past block_start. A cut that leaves no room in the output (avail_out, stream +0x10) returns 0.
// The last cut returns 2 for flush 4 and 0 otherwise when the output is full; with room left it returns 3 for flush 4
// and 1 otherwise.
unsigned int __cdecl QueueUnitDeflateStored_guess(int *s, int flush)
{
    unsigned int max_block;
    unsigned int lookahead;
    unsigned int old_strstart;
    unsigned int strstart;
    unsigned int max_start;
    int block_start;
    unsigned char *buf;

    max_block = 0xffff;
    if (s[3] - 5U < 0xffff)
    {
        max_block = s[3] - 5U;
    }
    do
    {
        lookahead = s[0x1b];
        if (lookahead <= 1)
        {
            EQ_CDECL(void, 0x10009960, (int *))(s);
            lookahead = s[0x1b];
            if (lookahead == 0 && flush == 0)
            {
                return 0;
            }
            if (s[0x1b] == 0)
            {
                block_start = s[0x15];
                buf = block_start >= 0 ? (unsigned char *)(s[0xc] + block_start) : 0;
                EQ_CDECL(void, 0x1000CD90, (int *, unsigned char *, unsigned int, int))(
                    s, buf, s[0x19] - block_start, flush == 4);
                s[0x15] = s[0x19];
                EQ_CDECL(void, 0x10009780, (int))(*s);
                if (*(int *)(*s + 0x10) == 0)
                {
                    return ((unsigned int)(flush != 4) - 1U) & 2U;
                }
                return flush == 4 ? 3 : 1;
            }
        }
        old_strstart = s[0x19];
        s[0x1b] = 0;
        s[0x19] = old_strstart + lookahead;
        block_start = s[0x15];
        max_start = block_start + max_block;
        if (s[0x19] == 0 || s[0x19] >= max_start)
        {
            strstart = s[0x19];
            s[0x19] = max_start;
            s[0x1b] = strstart - max_start;
            buf = block_start >= 0 ? (unsigned char *)(s[0xc] + block_start) : 0;
            EQ_CDECL(void, 0x1000CD90, (int *, unsigned char *, unsigned int, int))(s, buf, max_start - block_start, 0);
            s[0x15] = s[0x19];
            EQ_CDECL(void, 0x10009780, (int))(*s);
            if (*(int *)(*s + 0x10) == 0)
            {
                return 0;
            }
        }
        block_start = s[0x15];
        if ((unsigned int)(s[0x19] - block_start) >= s[9] - 0x106U)
        {
            buf = block_start >= 0 ? (unsigned char *)(s[0xc] + block_start) : 0;
            EQ_CDECL(void, 0x1000CD90, (int *, unsigned char *, unsigned int, int))(s, buf, s[0x19] - block_start, 0);
            s[0x15] = s[0x19];
            EQ_CDECL(void, 0x10009780, (int))(*s);
            if (*(int *)(*s + 0x10) == 0)
            {
                return 0;
            }
        }
    } while (1);
}

// FUNCTION: EQMAIN 0x10009960
// Loads source bytes into the compressor's input window.
// __cdecl, 1 stack arg (the queue unit, laid out like zlib 1.1's deflate state; its source stream is unit[0]). While the
// lookahead (unit[0x1b]) is below 262 and the stream has bytes, the window is filled after the lookahead at strstart
// (unit[0x19]). Once strstart reaches 2 * w_size(unit[9]) - 262 the upper half slides down by w_size: the block and
// match starts move with it, and each head or prev link drops by w_size (0 when it was below w_size). The room is
// then read by QueueUnitReadBytes 0x10009A90, and ins_h (unit[0x10]) is rehashed once 3 bytes are ahead. Returns nothing.
void __cdecl QueueUnitFillWindow_guess(int *s)
{
    unsigned int wsize;
    unsigned int value;
    unsigned int tmp;
    unsigned short *p;
    unsigned int scratch;
    unsigned int n;
    int hashLeft;

    wsize = s[9];
    do
    {
        scratch = (s[0xd] - s[0x19]) - s[0x1b];
        tmp = s[0x19];
        if (scratch == 0 && tmp == 0 && s[0x1b] == 0)
        {
            scratch = wsize;
        }
        else if (scratch == 0xffffffff)
        {
            scratch = 0xfffffffe;
        }
        else
        {
            if (tmp >= s[9] + -0x106 + wsize)
            {
                memcpy((void *)s[0xc], (void *)(s[0xc] + wsize), wsize);
                s[0x1a] = s[0x1a] - wsize;
                hashLeft = s[0x11];
                s[0x19] = s[0x19] - wsize;
                s[0x15] = s[0x15] - wsize;
                p = (unsigned short *)(s[0xf] + hashLeft * 2);
                do
                {
                    p = p + -1;
                    if (*p >= wsize)
                    {
                        value = *p - wsize;
                    }
                    else
                    {
                        value = 0;
                    }
                    hashLeft = hashLeft - 1;
                    *p = value;
                } while (hashLeft != 0);
                p = (unsigned short *)(s[0xe] + wsize * 2);
                tmp = wsize;
                do
                {
                    p = p + -1;
                    if (*p >= wsize)
                    {
                        value = *p - wsize;
                    }
                    else
                    {
                        value = 0;
                    }
                    tmp = tmp - 1;
                    *p = value;
                } while (tmp != 0);
                scratch = scratch + wsize;
            }
        }
        if (((int *)*s)[1] == 0)
        {
            return;
        }
        n = EQ_CDECL(unsigned int, 0x10009A90, (int *, void *, unsigned int))((int *)*s, (void *)(s[0xc] + s[0x19] + s[0x1b]), scratch);
        s[0x1b] = s[0x1b] + n;
        tmp = s[0x1b];
        if (tmp >= 3)
        {
            scratch = (unsigned int)*(unsigned char *)(s[0x19] + s[0xc]);
            s[0x10] = scratch;
            s[0x10] = ((scratch << s[0x14]) ^ (unsigned int)((unsigned char *)(s[0x19] + s[0xc]))[1]) & s[0x13];
        }
    } while (tmp < 0x106 && ((int *)*s)[1] != 0);
}

// FUNCTION: EQMAIN 0x10009A90
// Supplies source bytes to the compressor while maintaining its checksum.
// __cdecl. Copies up to bufferSize bytes from the cursor (strm[0]) to buf and takes them off the count in
// strm[1]; the cursor and strm[2] move on by the same amount. Unless the object at strm[7] has a nonzero
// flag at +0x18, the Adler-32 of the bytes copied, continuing from strm[0xc], is stored in strm[0xc] first.
// Returns the number of bytes copied, 0 when none are left.
unsigned int __cdecl QueueUnitReadBytes_guess(int *strm, void *buf, unsigned int bufferSize)
{
    unsigned int len;

    len = strm[1];
    if (len > bufferSize)
    {
        len = bufferSize;
    }
    if (len == 0)
    {
        return 0;
    }
    strm[1] = strm[1] - len;
    if (*(int *)(strm[7] + 0x18) == 0)
    {
        strm[0xc] = EQ_CDECL(unsigned int, 0x10009650, (unsigned int, unsigned char *, unsigned int))(strm[0xc], (unsigned char *)strm[0], len);
    }
    memcpy(buf, (void *)strm[0], len);
    strm[0] = strm[0] + len;
    strm[2] = strm[2] + len;
    return len;
}

// FUNCTION: EQMAIN 0x1000A3E0
// zlib inflate_blocks_reset: frees pending trees/codes, resets bit buffer, window and check value.
// __cdecl. Writes unit[0xf] to *out when out is not null, then runs the pending callback: rec->fn(rec->arg, unit[3])
// for states 4 and 5, InvokeCallback 0x1000BA30(unit[1], rec) for state 6. Sets the state to 0, points unit[0xc] and
// unit[0xd] at the current row (unit[10]), clears unit[7] and unit[8], and when the unit has a begin function
// (unit[0xe]) calls it with three zeros, keeping the result in unit[0xf] and rec+0x30. Returns nothing.
void __cdecl QueueUnitReset_guess(int *s, CallbackRec_guess *z, int *c)
{
    int result;

    if (c != 0)
    {
        *c = s[0xf];
    }
    if (*s == 4 || *s == 5)
    {
        z->fn(z->arg, s[3]);
    }
    if (*s == 6)
    {
        EQ_CDECL(void, 0x1000BA30, (int, CallbackRec_guess *))(s[1], z);
    }
    *s = 0;
    s[0xd] = s[10];
    s[0xc] = s[10];
    s[7] = 0;
    s[8] = 0;
    if (s[0xe] != 0)
    {
        result = ((int (__cdecl *)(int, int, int))s[0xe])(0, 0, 0);
        s[0xf] = result;
        *(int *)((char *)z + 0x30) = result;   // rec+0x30: CallbackRec_guess names no field there
    }
}

// FUNCTION: EQMAIN 0x1000A460
// Creates a buffer-backed stream unit; its specific front-end use is unclear.
// __cdecl. Creates a queue unit from the record rec. The unit is 0x40 bytes from rec's allocator (rec+0x20, which
// CallbackRec_guess doesn't name: alloc(rec->arg, count, size)). A 0x5A0-byte buffer goes in unit[9], a `w`-byte
// buffer in unit[10] (unit[0xb] is its end), and c is the begin function (unit[0xe]). The unit is then reset with
// QueueUnitReset 0x1000A3E0 (no out pointer). When an allocation fails, the pieces already got go back through
// rec->fn(rec->arg, ptr) and the result is 0. Returns the unit.
int *__cdecl QueueUnitCreate_guess(CallbackRec_guess *z, int c, int w)
{
    int *s;
    int allocation;

    s = (int *)((int (__cdecl *)(int, int, int))*(int *)((char *)z + 0x20))(z->arg, 1, 0x40);
    if (s == 0)
    {
        return s;
    }
    allocation = ((int (__cdecl *)(int, int, int))*(int *)((char *)z + 0x20))(z->arg, 8, 0x5a0);
    s[9] = allocation;
    if (allocation == 0)
    {
        z->fn(z->arg, (int)s);
        return 0;
    }
    allocation = ((int (__cdecl *)(int, int, int))*(int *)((char *)z + 0x20))(z->arg, 1, w);
    s[10] = allocation;
    if (allocation == 0)
    {
        z->fn(z->arg, s[9]);
        z->fn(z->arg, (int)s);
        return 0;
    }
    s[0xb] = allocation + w;
    s[0xe] = c;
    *s = 0;
    EQ_CDECL(void, 0x1000A3E0, (int *, CallbackRec_guess *, int *))(s, z, 0);
    return s;
}

// FUNCTION: EQMAIN 0x1000A500
// Decompresses block by block: reads each header, copies stored blocks, builds the tables of Huffman blocks
// __cdecl. zlib 1.1.x's inflate_blocks (infblock.c): the block state machine (mode 0 TYPE .. 9 BAD) over the input z
// gives and the window s holds. Every exit is LEAVE: the bit buffer, counts and pointers go back to s and z, then
// inflate_flush (0x1000CB20) returns its result. s offsets: mode 0x00, left/table/codes 0x04, index 0x08, blens 0x0C,
// bb 0x10, tb 0x14, last 0x18, bitk 0x1C, bitb 0x20, hufts 0x24, window 0x28, end 0x2C, read 0x30, write 0x34. z offsets:
// next_in 0x00, avail_in 0x04, total_in 0x08, msg 0x18, zalloc 0x20, zfree 0x24, opaque 0x28. Ghidra's local names are
// kept: s = s, k = the bit count (puVar7 in the decompile), acc = the bit buffer, value = the bit buffer
// inside DTREE and CODES (a temporary elsewhere), n = avail_in, p = next_in, slot = the window room.
int __cdecl inflate_blocks_guess(unsigned int *q, int *param_2, int param_3)
{
    unsigned int *s;
    unsigned int *readPtr;
    unsigned int *windowPtr;
    unsigned int tmp;
    unsigned int fillValue;
    unsigned int k;
    unsigned int value;
    unsigned char t;
    int old;
    int scratch;
    unsigned int n;
    unsigned char *p;
    unsigned int acc;
    unsigned int slot;
    unsigned int other;
    unsigned int td;
    unsigned int tl;
    unsigned int bd;
    unsigned int bl;
    unsigned int c;
    unsigned int dynTd;
    unsigned int dynTl;

    s = q;
    p = (unsigned char *)*param_2;
    n = param_2[1];
    acc = q[8];
    k = q[7];
    readPtr = (unsigned int *)q[0xd];
    if (readPtr < (unsigned int *)q[0xc])
    {
        slot = q[0xc] + (-1 - (int)readPtr);
        q = readPtr;
    }
    else
    {
        slot = q[0xb] - (int)readPtr;
        q = readPtr;
    }
dispatch_mode:
    value = acc;
    switch (*s)
    {
    case 0:
        goto mode_type;
    case 1:
        for (; k < 0x20; k += 8)
        {
            if (n == 0)
            {
                s[8] = acc;
                s[7] = k;
                scratch = *param_2;
                param_2[1] = 0;
                *param_2 = (int)p;
                param_2[2] = (int)((int)p + (param_2[2] - scratch));
                s[0xd] = (unsigned int)q;
                return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
            }
            param_3 = 0;
            n = n - 1;
            acc = acc | (unsigned int)*p << (k & 0x1f);
            p = p + 1;
        }
        value = acc & 0xffff;
        if ((~acc >> 0x10) != value)
        {
            *s = 9;
            param_2[6] = (int)EQ_ARRAY(char, 0x10046660);
            s[8] = acc;
            s[7] = k;
            scratch = *param_2;
            *param_2 = (int)p;
            param_2[1] = n;
            param_2[2] = (int)((int)p + (param_2[2] - scratch));
            s[0xd] = (unsigned int)q;
            return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -3);
        }
        k = 0;
        s[1] = value;
        acc = 0;
        if (value != 0)
        {
            *s = 2;
            goto dispatch_mode;
        }
        break;
    case 2:
        if (n == 0)
        {
            s[8] = acc;
            s[7] = k;
            scratch = *param_2;
            *param_2 = (int)p;
            param_2[1] = 0;
            param_2[2] = (int)((int)p + (param_2[2] - scratch));
            s[0xd] = (unsigned int)q;
            return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
        }
        if (slot == 0)
        {
            if (q == (unsigned int *)s[0xb])
            {
                readPtr = (unsigned int *)s[0xc];
                windowPtr = (unsigned int *)s[10];
                if (readPtr != windowPtr)
                {
                    if (windowPtr < readPtr)
                    {
                        slot = (int)readPtr + (-1 - (int)windowPtr);
                    }
                    else
                    {
                        slot = (int)s[0xb] - (int)windowPtr;
                    }
                    q = windowPtr;
                    if (slot != 0) goto stored_copy;
                }
            }
            s[0xd] = (unsigned int)q;
            scratch = EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
            q = (unsigned int *)s[0xd];
            readPtr = (unsigned int *)s[0xc];
            if (q < readPtr)
            {
                slot = (int)readPtr + (-1 - (int)q);
            }
            else
            {
                slot = s[0xb] - (int)q;
            }
            other = s[0xb];
            if ((q == (unsigned int *)other) && (windowPtr = (unsigned int *)s[10], readPtr != windowPtr))
            {
                q = windowPtr;
                if (windowPtr < readPtr)
                {
                    slot = (int)readPtr + (-1 - (int)windowPtr);
                }
                else
                {
                    slot = (int)other - (int)windowPtr;
                }
            }
            if (slot == 0)
            {
                s[8] = acc;
                s[7] = k;
                old = *param_2;
                param_2[1] = n;
                *param_2 = (int)p;
                param_2[2] = (int)((int)p + (param_2[2] - old));
                s[0xd] = (unsigned int)q;
                return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, scratch);
            }
        }
stored_copy:
        param_3 = 0;
        value = s[1];
        if (n < s[1])
        {
            value = n;
        }
        if (slot < value)
        {
            value = slot;
        }
        memcpy(q, p, value);
        slot = slot - value;
        p = p + value;
        tmp = s[1];
        n = n - value;
        q = (unsigned int *)((int)q + value);
        s[1] = tmp - value;
        if (tmp - value != 0) goto dispatch_mode;
        break;
    case 3:
        for (; k < 0xe; k += 8)
        {
            if (n == 0)
            {
                s[8] = acc;
                s[7] = k;
                scratch = *param_2;
                *param_2 = (int)p;
                param_2[1] = 0;
                param_2[2] = (int)((int)p + (param_2[2] - scratch));
                s[0xd] = (unsigned int)q;
                return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
            }
            n = n - 1;
            param_3 = 0;
            acc = acc | (unsigned int)*p << (k & 0x1f);
            p = p + 1;
        }
        s[1] = acc & 0x3fff;
        if ((0x1d < (acc & 0x1f)) || (0x3a0 < (acc & 0x3e0)))
        {
            *s = 9;
            param_2[6] = (int)EQ_ARRAY(char, 0x1004663C);
            s[8] = acc;
            s[7] = k;
            scratch = *param_2;
            param_2[1] = n;
            *param_2 = (int)p;
            param_2[2] = (int)((int)p + (param_2[2] - scratch));
            s[0xd] = (unsigned int)q;
            return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -3);
        }
        value = ((int (__cdecl *)(int, int, int))param_2[8])(param_2[10],
            ((acc & 0x3fff) >> 5 & 0x1f) + 0x102 + (acc & 0x1f), 4);
        s[3] = value;
        if (value == 0)
        {
            s[8] = acc;
            s[7] = k;
            scratch = *param_2;
            param_2[1] = n;
            *param_2 = (int)p;
            param_2[2] = (int)((int)p + (param_2[2] - scratch));
            s[0xd] = (unsigned int)q;
            return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -4);
        }
        acc = acc >> 0xe;
        k = k - 0xe;
        s[2] = 0;
        *s = 4;
        goto mode_btree;
    case 4:
mode_btree:
        if (s[2] < (s[1] >> 10) + 4)
        {
            do
            {
                for (; k < 3; k += 8)
                {
                    if (n == 0)
                    {
                        s[8] = acc;
                        s[7] = k;
                        scratch = *param_2;
                        *param_2 = (int)p;
                        param_2[2] = (int)((int)p + (param_2[2] - scratch));
                        param_2[1] = 0;
                        s[0xd] = (unsigned int)q;
                        return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
                    }
                    n = n - 1;
                    param_3 = 0;
                    acc = acc | (unsigned int)*p << (k & 0x1f);
                    p = p + 1;
                }
                value = acc & 7;
                k = k - 3;
                acc = acc >> 3;
                *(unsigned int *)(s[3] + EQ_ARRAY(int, 0x10040EB0)[s[2]] * 4) = value;
                value = s[2];
                s[2] = value + 1;
            } while (value + 1 < (s[1] >> 10) + 4);
        }
        value = s[2];
        while (value < 0x13)
        {
            *(unsigned int *)(s[3] + EQ_ARRAY(int, 0x10040EB0)[s[2]] * 4) = 0;
            value = s[2] + 1;
            s[2] = value;
        }
        s[4] = 7;
        scratch = EQ_CDECL(int, 0x1000C3E0, (unsigned int *, unsigned int *, unsigned int *, unsigned int, int *))
            ((unsigned int *)s[3], s + 4, s + 5, s[9], param_2);
        value = acc;
        if (scratch == 0)
        {
            s[2] = 0;
            *s = 5;
mode_dtree:
            if (s[2] < (s[1] >> 5 & 0x1f) + 0x102 + (s[1] & 0x1f))
            {
                do
                {
                    for (; k < s[4]; k += 8)
                    {
                        if (n == 0) goto leave_need_input;
                        n = n - 1;
                        param_3 = 0;
                        value = value | (unsigned int)*p << (k & 0x1f);
                        p = p + 1;
                    }
                    t = *(unsigned char *)(s[5] + 1 + (EQ_ARRAY(unsigned int, 0x10047930)[s[4]] & value) * 8);
                    tmp = (unsigned int)t;
                    c = *(unsigned int *)(s[5] + (EQ_ARRAY(unsigned int, 0x10047930)[s[4]] & value) * 8 + 4);
                    if (c < 0x10)
                    {
                        k = k - tmp;
                        value = value >> (t & 0x1f);
                        *(unsigned int *)(s[3] + s[2] * 4) = c;
                        tmp = s[2] + 1;
                    }
                    else
                    {
                        slot = 7;
                        if (c != 0x12)
                        {
                            slot = c - 0xe;
                        }
                        other = slot + tmp;
                        for (; k < other; k += 8)
                        {
                            if (n == 0)
                            {
leave_need_input:
                                s[8] = value;
                                s[7] = k;
                                scratch = *param_2;
                                *param_2 = (int)p;
                                param_2[2] = (int)((int)p + (param_2[2] - scratch));
                                param_2[1] = 0;
                                s[0xd] = (unsigned int)q;
                                return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
                            }
                            n = n - 1;
                            param_3 = 0;
                            value = value | (unsigned int)*p << (k & 0x1f);
                            p = p + 1;
                        }
                        value = value >> (t & 0x1f);
                        acc = (-(unsigned int)(c != 0x12) & 0xfffffff8) + 0xb + (EQ_ARRAY(unsigned int, 0x10047930)[slot] & value);
                        value = value >> (slot & 0x1f);
                        k = k - (slot + tmp);
                        slot = s[2];
                        if ((s[1] >> 5 & 0x1f) + 0x102 + (s[1] & 0x1f) < acc + slot)
                        {
bad_repeat:
                            ((void (__cdecl *)(int, int))param_2[9])(param_2[10], s[3]);
                            *s = 9;
                            param_2[6] = (int)EQ_ARRAY(char, 0x10046620);
                            s[8] = value;
                            s[7] = k;
                            scratch = *param_2;
                            param_2[1] = n;
                            *param_2 = (int)p;
                            param_2[2] = (int)((int)p + (param_2[2] - scratch));
                            s[0xd] = (unsigned int)q;
                            return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -3);
                        }
                        tmp = slot;
                        if (c == 0x10)
                        {
                            if (slot == 0) goto bad_repeat;
                            fillValue = *(unsigned int *)((s[3] - 4) + slot * 4);
                        }
                        else
                        {
                            fillValue = 0;
                        }
                        do
                        {
                            tmp = tmp + 1;
                            *(unsigned int *)((s[3] - 4) + tmp * 4) = fillValue;
                            acc = acc - 1;
                        } while (acc != 0);
                    }
                    s[2] = tmp;
                } while (s[2] < (s[1] >> 5 & 0x1f) + 0x102 + (s[1] & 0x1f));
            }
            s[5] = 0;
            other = 9;
            slot = 6;
            acc = EQ_CDECL(int, 0x1000C960, (unsigned int, unsigned int, unsigned int *, unsigned int *, unsigned int *, unsigned int *, unsigned int *, unsigned int, int *))
                ((s[1] & 0x1f) + 0x101, (s[1] >> 5 & 0x1f) + 1, (unsigned int *)s[3], &other, &slot, &dynTl, &dynTd, s[9], param_2);
            ((void (__cdecl *)(int, int))param_2[9])(param_2[10], s[3]);
            if (acc == 0)
            {
                tmp = EQ_CDECL(unsigned int, 0x1000B250, (unsigned int, unsigned int, unsigned int, unsigned int, int *))(other, slot, dynTl, dynTd, param_2);
                if (tmp == 0)
                {
                    s[8] = value;
                    s[7] = k;
                    param_2[1] = n;
                    param_2[2] = (int)((int)p + (param_2[2] - *param_2));
                    *param_2 = (int)p;
                    s[0xd] = (unsigned int)q;
                    return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -4);
                }
                s[1] = tmp;
                *s = 6;
mode_codes:
                s[8] = value;
                s[7] = k;
                scratch = *param_2;
                param_2[1] = n;
                *param_2 = (int)p;
                param_2[2] = (int)((int)p + (param_2[2] - scratch));
                s[0xd] = (unsigned int)q;
                scratch = EQ_CDECL(int, 0x1000B290, (unsigned int *, int *, int))(s, param_2, param_3);
                if (scratch == 1)
                {
                    param_3 = 0;
                    EQ_CDECL(void, 0x1000BA30, (unsigned int, int *))(s[1], param_2);
                    acc = s[8];
                    q = (unsigned int *)s[0xd];
                    p = (unsigned char *)*param_2;
                    n = param_2[1];
                    k = s[7];
                    if (q < (unsigned int *)s[0xc])
                    {
                        slot = (int)s[0xc] + (-1 - (int)q);
                    }
                    else
                    {
                        slot = s[0xb] - (int)q;
                    }
                    if (s[6] == 0)
                    {
                        *s = 0;
                        goto dispatch_mode;
                    }
                    if (7 < k)
                    {
                        k = k - 8;
                        n = n + 1;
                        p = p - 1;
                    }
                    *s = 7;
mode_dry:
                    s[0xd] = (unsigned int)q;
                    scratch = EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
                    q = (unsigned int *)s[0xd];
                    if ((unsigned int *)s[0xc] == q)
                    {
                        *s = 8;
mode_done:
                        s[8] = acc;
                        s[7] = k;
                        param_2[1] = n;
                        param_2[2] = (int)((int)p + (param_2[2] - *param_2));
                        *param_2 = (int)p;
                        s[0xd] = (unsigned int)q;
                        return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, 1);
                    }
                    s[7] = k;
                    s[8] = acc;
                    old = *param_2;
                    param_2[1] = n;
                    *param_2 = (int)p;
                    param_2[2] = (int)((int)p + (param_2[2] - old));
                    s[0xd] = (unsigned int)q;
                }
                return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, scratch);
            }
        }
        else
        {
            ((void (__cdecl *)(int, int))param_2[9])(param_2[10], s[3]);
            acc = scratch;
        }
        if (acc == -3)
        {
            *s = 9;
        }
        s[8] = value;
        s[7] = k;
        scratch = *param_2;
        param_2[1] = n;
        *param_2 = (int)p;
        param_2[2] = (int)((int)p + (param_2[2] - scratch));
        s[0xd] = (unsigned int)q;
        return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, acc);
    case 5:
        goto mode_dtree;
    case 6:
        goto mode_codes;
    case 7:
        goto mode_dry;
    case 8:
        goto mode_done;
    case 9:
        s[8] = acc;
        s[7] = k;
        scratch = *param_2;
        param_2[1] = n;
        *param_2 = (int)p;
        param_2[2] = (int)((int)p + (param_2[2] - scratch));
        s[0xd] = (unsigned int)q;
        return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -3);
    default:
        s[8] = acc;
        s[7] = k;
        scratch = *param_2;
        param_2[1] = n;
        *param_2 = (int)p;
        param_2[2] = (int)((int)p + (param_2[2] - scratch));
        s[0xd] = (unsigned int)q;
        return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -2);
    }
    *s = -(unsigned int)(s[6] != 0) & 7;
    goto dispatch_mode;
mode_type:
    for (; k < 3; k += 8)
    {
        if (n == 0)
        {
            s[8] = acc;
            s[7] = k;
            param_2[1] = 0;
            param_2[2] = (int)((int)p + (param_2[2] - *param_2));
            *param_2 = (int)p;
            s[0xd] = (unsigned int)q;
            return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, param_3);
        }
        n = n - 1;
        param_3 = 0;
        acc = acc | (unsigned int)*p << (k & 0x1f);
        p = p + 1;
    }
    s[6] = acc & 1;
    switch ((acc & 7) >> 1)
    {
    case 0:
        *s = 1;
        value = (k - 3) & 7;
        acc = (acc >> 3) >> (unsigned char)value;
        k = (k - 3) - value;
        break;
    case 1:
        EQ_CDECL(void, 0x1000CAF0, (unsigned int *, unsigned int *, unsigned int *, unsigned int *, int *))(&bl, &bd, &tl, &td, param_2);
        value = EQ_CDECL(unsigned int, 0x1000B250, (unsigned int, unsigned int, unsigned int, unsigned int, int *))(bl, bd, tl, td, param_2);
        s[1] = value;
        if (value == 0)
        {
            s[8] = acc;
            s[7] = k;
            param_2[1] = n;
            param_2[2] = (int)((int)p + (param_2[2] - *param_2));
            *param_2 = (int)p;
            s[0xd] = (unsigned int)q;
            return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -4);
        }
        acc = acc >> 3;
        k = k - 3;
        *s = 6;
        break;
    case 2:
        acc = acc >> 3;
        k = k - 3;
        *s = 3;
        break;
    case 3:
        *s = 9;
        param_2[6] = (int)EQ_ARRAY(char, 0x10046680);
        s[8] = acc >> 3;
        s[7] = k - 3;
        param_2[1] = n;
        param_2[2] = (int)((int)p + (param_2[2] - *param_2));
        *param_2 = (int)p;
        s[0xd] = (unsigned int)q;
        return EQ_CDECL(int, 0x1000CB20, (unsigned int *, int *, int))(s, param_2, -3);
    }
    goto dispatch_mode;
}

// FUNCTION: EQMAIN 0x1000B210
// zlib inflate_blocks_free: resets the blocks state, then zfrees its window, its hufts and the state
// __cdecl. Resets the unit with QueueUnitReset 0x1000A3E0 (no out pointer), then calls rec->fn(rec->arg, unit[10]),
// rec->fn(rec->arg, unit[9]) and rec->fn(rec->arg, unit) in that order. Returns 0.
int __cdecl QueueUnitFlush_guess(int *s, CallbackRec_guess *z)
{
    EQ_CDECL(void, 0x1000A3E0, (int *, CallbackRec_guess *, int *))(s, z, 0);
    z->fn(z->arg, s[10]);
    z->fn(z->arg, s[9]);
    z->fn(z->arg, (int)s);
    return 0;
}

// The record the node allocator is read from. Only the two fields this function reads are named; the rest is not known.
struct AllocRec_guess
{
    char unknown_00[0x20];                  // 0x00 .. 0x1F: not read here
    void *(__cdecl *alloc)(int, int, int);  // 0x20: the allocator, called as (arg, 1, 0x1C)
    char unknown_24[4];                     // 0x24 .. 0x27: not read here
    int arg;                                // 0x28: its first argument
};

// The 0x1C-byte node the allocator returns. Only the fields this function writes are named.
struct QueueNode_guess
{
    int unknown_00;         // 0x00: zeroed here
    char unknown_04[0xC];   // 0x04 .. 0x0F: not written here
    unsigned char byte_10;  // 0x10: the first byte argument
    unsigned char byte_11;  // 0x11: the second byte argument
    char unknown_12[2];     // 0x12 .. 0x13: not written here
    int value_14;           // 0x14: the third argument
    int value_18;           // 0x18: the fourth argument
};

// FUNCTION: EQMAIN 0x1000B250
// zlib inflate_codes_new: allocates the 0x1C-byte codes state (mode START) holding bl, bd, tl and td
// __cdecl. Allocates a 0x1C-byte node with rec->alloc(rec->arg, 1, 0x1C). When the allocator returns a node, stores
// byte10 and byte11 as bytes at +0x10 and +0x11, zeroes the dword at +0x00 and stores value14 and value18 at +0x14 and
// +0x18. Returns the node, or 0 when the allocator returned 0 (the callers read EAX).
QueueNode_guess * __cdecl QueueNodeNew_guess(unsigned char byte10, unsigned char byte11, int value14, int value18,
                                             AllocRec_guess *allocRec)
{
    QueueNode_guess *node;

    node = (QueueNode_guess *)allocRec->alloc(allocRec->arg, 1, 0x1C);
    if (node != 0)
    {
        node->byte_10 = byte10;
        node->byte_11 = byte11;
        node->unknown_00 = 0;
        node->value_14 = value14;
        node->value_18 = value18;
    }
    return node;
}

// zlib's inflate_huft as this function reads it, 8 bytes per entry. exop 0 is a literal (base is the byte), 0x10 | n a
// length or distance code with n extra bits (base is its base), 1..15 a link to a subtable of 2^exop entries (base is
// the subtable's offset in entries), 0x40 an invalid code and 0x60 the end of the block. bits is what the entry consumes.
struct CodesHuft_guess
{
    unsigned char exop;             // 0x00
    unsigned char bits;             // 0x01
    unsigned char unknown_02[0x02]; // 0x02 .. 0x03: not read here
    unsigned int base;              // 0x04
};

// zlib's inflate_codes_statef (0x1C bytes): the mode the switch runs on, the match length, the operand of the mode, the
// index widths of the two tables and the tables. These stand-ins are separate from InflateHuft_guess, z_stream
// and inflate_blocks_state further down the file (the same offsets), so this function does not depend on them.
struct CodesState_guess
{
    int mode;                       // 0x00: 0 START, 1 LEN, 2 LENEXT, 3 DIST, 4 DISTEXT, 5 COPY, 6 LIT, 7 WASH, 8 END, 9 BADCODE
    unsigned int len;               // 0x04: bytes of the match still to copy
    union
    {
        struct
        {
            CodesHuft_guess *tree;  // 0x08: the table being decoded
            unsigned int need;      // 0x0C: bits its index takes
        } code;
        struct
        {
            unsigned int get;       // 0x08: extra bits still to read
            unsigned int dist;      // 0x0C: the distance, to which the extra bits are added
        } copy;
        unsigned int lit;           // 0x08: the literal to write
    } sub;
    unsigned char lbits;            // 0x10: index bits of the length table
    unsigned char dbits;            // 0x11: index bits of the distance table
    unsigned char unknown_12[0x02]; // 0x12 .. 0x13: not read here
    CodesHuft_guess *ltree;         // 0x14: the length/literal table
    CodesHuft_guess *dtree;         // 0x18: the distance table
};

// The z_stream this function reads and writes (zlib's z_stream): the input cursor and its counts, and the error text.
/* z_stream: the actor layout is z_stream (generated: eqmain/layout/z_stream.h); the old local view's members map by offset. */
#include "eqmain/layout/z_stream.h"

// The inflate state this function reads and writes: the codes state, the bit buffer and its count, and the sliding window
// with its end, read and write pointers.
/* inflate_blocks_state: the actor layout is inflate_blocks_state (generated: eqmain/layout/inflate_blocks_state.h); the old local view's members map by offset. */
#include "eqmain/layout/inflate_blocks_state.h"

// FUNCTION: EQMAIN 0x1000B290
// Decodes a compressed block's literals and match copies into the window, resumable mid-code across calls
// __cdecl. zlib's inflate_codes: the codes state machine. Runs the mode switch (0..9) over the bit buffer and the window
// until the block ends, the input runs out or a code is invalid, then returns the result of 0x1000CB20 (zlib's
// inflate_flush by role) in EAX, which the callers read. The fast loop is 0x1000BA50 (inflate_fast by role). A mode
// outside 0..9 takes the default exit with -2.
int __cdecl inflate_codes_guess(inflate_blocks_state *s, z_stream *z, int r)
{
    CodesState_guess *c;            // piVar2: the codes state
    unsigned int b;                 // param_1: the bit buffer
    unsigned int k;                 // uVar10: bits in the bit buffer
    unsigned char *p;               // pbVar11: the next input byte
    unsigned int n;                 // local_c: input bytes left
    unsigned char *q;               // puVar9: the window write pointer
    unsigned int m;                 // param_2: bytes from q to the window end, or to the read pointer
    unsigned int j;                 // uVar7 as the bits a LENEXT/DISTEXT reads
    unsigned int e;                 // uVar7 as a table entry's exop
    CodesHuft_guess *t;             // local_8: the table entry
    unsigned char *f;               // local_4: the copy's source
    unsigned char *w;               // puVar8: the write position once a wrap or a flush has been checked
    unsigned char *rp;              // puVar3: the read pointer or the window pointer, as the wrap uses it
    unsigned char *old;             // iVar6: next_in before this store
    int len;                        // iVar6: the copy length
    int ofs;                        // iVar6: the copy's source offset
    int flushed;                    // iVar6: the result of the flush in WASH

    c = s->codes;
    n = z->avail_in;
    p = z->next_in;
    k = s->bitk;
    q = s->write;
    if (q < s->read)
    {
        m = (unsigned int)(s->read - q - 1);
    }
    else
    {
        m = (unsigned int)(s->end - q);
    }
    b = s->bitb;

    for (;;)
    {
        switch (c->mode)
        {
        case 0:
            if (0x101 < m && 9 < n)
            {
                s->bitb = b;
                s->bitk = k;
                old = z->next_in;
                z->avail_in = n;
                z->total_in += (unsigned int)(p - old);
                z->next_in = p;
                s->write = q;
                r = EQ_CDECL(int, 0x1000BA50, (unsigned int, unsigned int, CodesHuft_guess *, CodesHuft_guess *,
                                               inflate_blocks_state *, z_stream *))(
                    c->lbits, c->dbits, c->ltree, c->dtree, s, z);
                n = z->avail_in;
                b = s->bitb;
                p = z->next_in;
                k = s->bitk;
                q = s->write;
                if (q < s->read)
                {
                    m = (unsigned int)(s->read - q - 1);
                }
                else
                {
                    m = (unsigned int)(s->end - q);
                }
                if (r != 0)
                {
                    // 9 (BADCODE) for an error; 7 (WASH) for Z_STREAM_END (r == 1)
                    c->mode = (-(unsigned int)(r != 1) & 2) + 7;
                    break;
                }
            }
            c->mode = 1;
            c->sub.code.need = c->lbits;
            c->sub.code.tree = c->ltree;
            // falls through
        case 1:
            for (; k < c->sub.code.need; k += 8)
            {
                if (n == 0)
                {
leave_no_input:
                    s->bitk = k;
                    s->bitb = b;
                    old = z->next_in;
                    z->avail_in = 0;
                    z->total_in += (unsigned int)(p - old);
                    z->next_in = p;
                    s->write = q;
                    return EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, r);
                }
                n--;
                r = 0;
                b |= (unsigned int)*p << (k & 0x1f);
                p++;
            }
            t = c->sub.code.tree + (EQ_ARRAY(unsigned int, 0x10047930)[c->sub.code.need] & b);
            b >>= (t->bits & 0x1f);
            k -= t->bits;
            e = t->exop;
            if (e == 0)
            {
                c->sub.lit = t->base;
                c->mode = 6;
            }
            else if ((e & 0x10) == 0)
            {
                if ((e & 0x40) == 0)
                {
next_table:
                    c->sub.code.need = e;
                    c->sub.code.tree = t + t->base;
                }
                else
                {
                    if ((e & 0x20) == 0)
                    {
                        c->mode = 9;
                        z->msg = EQ_ARRAY(char, 0x100466AC);
leave_bad_code:
                        s->bitb = b;
                        s->bitk = k;
                        z->avail_in = n;
                        z->total_in += (unsigned int)(p - z->next_in);
                        z->next_in = p;
                        s->write = q;
                        return EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, -3);
                    }
                    c->mode = 7;
                }
            }
            else
            {
                c->sub.copy.get = e & 0xf;
                c->len = t->base;
                c->mode = 2;
            }
            break;
        case 2:
            j = c->sub.copy.get;
            for (; k < j; k += 8)
            {
                if (n == 0)
                {
                    goto leave_no_input;
                }
                n--;
                r = 0;
                b |= (unsigned int)*p << (k & 0x1f);
                p++;
            }
            c->len += EQ_ARRAY(unsigned int, 0x10047930)[j] & b;
            b >>= (j & 0x1f);
            k -= j;
            c->mode = 3;
            c->sub.code.need = c->dbits;
            c->sub.code.tree = c->dtree;
            // falls through
        case 3:
            for (; k < c->sub.code.need; k += 8)
            {
                if (n == 0)
                {
                    goto leave_no_input;
                }
                n--;
                r = 0;
                b |= (unsigned int)*p << (k & 0x1f);
                p++;
            }
            t = c->sub.code.tree + (EQ_ARRAY(unsigned int, 0x10047930)[c->sub.code.need] & b);
            b >>= (t->bits & 0x1f);
            k -= t->bits;
            e = t->exop;
            if ((e & 0x10) == 0)
            {
                if ((e & 0x40) != 0)
                {
                    c->mode = 9;
                    z->msg = EQ_ARRAY(char, 0x10046694);
                    goto leave_bad_code;
                }
                goto next_table;
            }
            c->sub.copy.get = e & 0xf;
            c->sub.copy.dist = t->base;
            c->mode = 4;
            break;
        case 4:
            j = c->sub.copy.get;
            for (; k < j; k += 8)
            {
                if (n == 0)
                {
                    goto leave_no_input;
                }
                n--;
                r = 0;
                b |= (unsigned int)*p << (k & 0x1f);
                p++;
            }
            c->sub.copy.dist += EQ_ARRAY(unsigned int, 0x10047930)[j] & b;
            b >>= (j & 0x1f);
            k -= j;
            c->mode = 5;
            // falls through
        case 5:
            if ((unsigned int)(q - s->window) < c->sub.copy.dist)
            {
                ofs = (int)((s->end - s->window) - c->sub.copy.dist);
            }
            else
            {
                ofs = -(int)c->sub.copy.dist;
            }
            f = q + ofs;
            len = c->len;
            while (len != 0)
            {
                w = q;
                if (m == 0)
                {
                    if (q == s->end)
                    {
                        rp = s->read;
                        w = s->window;
                        if (rp != w)
                        {
                            if (w < rp)
                            {
                                m = (unsigned int)(rp - w - 1);
                            }
                            else
                            {
                                m = (unsigned int)(s->end - w);
                            }
                            q = w;
                            if (m != 0)
                            {
                                goto copy_byte;
                            }
                        }
                    }
                    s->write = q;
                    r = EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, r);
                    w = s->write;
                    q = s->read;
                    if (w < q)
                    {
                        m = (unsigned int)(q - w - 1);
                    }
                    else
                    {
                        m = (unsigned int)(s->end - w);
                    }
                    if (w == s->end && (rp = s->window, q != rp))
                    {
                        w = rp;
                        if (rp < q)
                        {
                            m = (unsigned int)(q - rp - 1);
                        }
                        else
                        {
                            m = (unsigned int)(s->end - rp);
                        }
                    }
                    if (m == 0)
                    {
                        goto leave_window_full;
                    }
                }
copy_byte:
                q = w + 1;
                r = 0;
                *w = *f;
                f++;
                m--;
                if (f == s->end)
                {
                    f = s->window;
                }
                len = c->len - 1;
                c->len = len;
            }
            c->mode = 0;
            break;
        case 6:
            w = q;
            if (m == 0)
            {
                if (q == s->end)
                {
                    rp = s->read;
                    w = s->window;
                    if (rp != w)
                    {
                        if (w < rp)
                        {
                            m = (unsigned int)(rp - w - 1);
                        }
                        else
                        {
                            m = (unsigned int)(s->end - w);
                        }
                        q = w;
                        if (m != 0)
                        {
                            goto write_literal;
                        }
                    }
                }
                s->write = q;
                r = EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, r);
                w = s->write;
                q = s->read;
                if (w < q)
                {
                    m = (unsigned int)(q - w - 1);
                }
                else
                {
                    m = (unsigned int)(s->end - w);
                }
                if (w == s->end && (rp = s->window, q != rp))
                {
                    w = rp;
                    if (rp < q)
                    {
                        m = (unsigned int)(q - rp - 1);
                    }
                    else
                    {
                        m = (unsigned int)(s->end - rp);
                    }
                }
                if (m == 0)
                {
leave_window_full:
                    s->bitb = b;
                    s->bitk = k;
                    old = z->next_in;
                    z->avail_in = n;
                    z->total_in += (unsigned int)(p - old);
                    z->next_in = p;
                    s->write = w;
                    return EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, r);
                }
            }
write_literal:
            r = 0;
            *w = (unsigned char)c->sub.lit;
            q = w + 1;
            m--;
            c->mode = 0;
            break;
        case 7:
            s->write = q;
            flushed = EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, r);
            q = s->write;
            if (s->read != q)
            {
                s->bitk = k;
                s->bitb = b;
                z->avail_in = n;
                z->total_in += (unsigned int)(p - z->next_in);
                z->next_in = p;
                s->write = q;
                return EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, flushed);
            }
            c->mode = 8;
            // falls through
        case 8:
            goto mode_end;
        case 9:
            s->bitb = b;
            s->bitk = k;
            z->avail_in = n;
            z->total_in += (unsigned int)(p - z->next_in);
            z->next_in = p;
            s->write = q;
            return EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, -3);
        default:
            s->bitb = b;
            s->bitk = k;
            z->avail_in = n;
            z->total_in += (unsigned int)(p - z->next_in);
            z->next_in = p;
            s->write = q;
            return EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, -2);
        }
    }
mode_end:
    s->bitb = b;
    s->bitk = k;
    old = z->next_in;
    z->avail_in = n;
    z->total_in += (unsigned int)(p - old);
    z->next_in = p;
    s->write = q;
    return EQ_CDECL(int, 0x1000CB20, (inflate_blocks_state *, z_stream *, int))(s, z, 1);
}

// FUNCTION: EQMAIN 0x1000BA30
// Frees a codes state through the stream's zfree callback (zlib inflate_codes_free by role).
// __cdecl. Calls rec->fn(rec->arg, value): the callback gets the record's +0x28 value first and value second. Its
// return value is left in EAX, which the callers never read.
void __cdecl InvokeCallback_guess(int value, CallbackRec_guess* rec)
{
    rec->fn(rec->arg, value);
}

// This image's globals (eqmain.dll, not eqgame.exe): their types and names are unknown, so they are named by VA.
#define g_eqmain_10046738_guess  EQ_GLOBAL(unsigned int, 0x10046738)
#define g_eqmain_1004673c_guess  EQ_GLOBAL(unsigned int, 0x1004673C)
#define g_eqmain_10046740_guess  EQ_ARRAY(unsigned char, 0x10046740)
#define g_eqmain_10047740_guess  EQ_ARRAY(unsigned char, 0x10047740)
#define g_zlibVersionChar_guess  EQ_GLOBAL(char, 0x1004660C)
// zlib's inflate_mask ((1 << n) - 1 for n in 0..16) and the two messages inflate_fast_guess stores in z->msg.
#define g_inflateMask_guess  EQ_ARRAY(unsigned int, 0x10047930)
#define g_eqmain_10046694_guess  EQ_ARRAY(char, 0x10046694)
#define g_eqmain_100466ac_guess  EQ_ARRAY(char, 0x100466AC)

// One decode-table entry, 8 bytes (zlib's inflate_huft). exop is the operation: 0 a literal (base is the byte), 0x10 | n
// a length or distance code with n extra bits (base is its base), 1..15 a link to a subtable indexed by that many bits
// (base is the subtable's offset in entries), 0x40 an invalid code, 0x60 the end of the block. bits is the number of
// bits the entry consumes.
struct InflateHuft_guess
{
    unsigned char exop;             // 0x00
    unsigned char bits;             // 0x01
    unsigned char unknown_02[0x02]; // 0x02 .. 0x03: the pad of zlib's union, not used here
    unsigned int base;              // 0x04
};

// The z_stream the fast loop reads and writes (zlib's z_stream, 0x38 bytes): the input cursor and its counts, and the
// error text. The other fields are not named here. (The stream z_stream below is a partial stand-in of the same
// struct; this one is separate so this function does not depend on it.)
/* z_stream: the actor layout is z_stream (generated: eqmain/layout/z_stream.h); the old local view's members map by offset. */
#include "eqmain/layout/z_stream.h"

// The inflate state the fast loop reads and writes: the bit buffer and its count, and the sliding window with its end,
// read and write pointers. Offsets as the code uses them; the rest of the state is not named.
/* inflate_blocks_state: the actor layout is inflate_blocks_state (generated: eqmain/layout/inflate_blocks_state.h); the old local view's members map by offset. */
#include "eqmain/layout/inflate_blocks_state.h"

// FUNCTION: EQMAIN 0x1000BA50
// Fast path of decompression: expands literal bytes and back-references into the window while room remains.
// __cdecl. zlib 1.1.x's inflate_fast (inffast.c): decodes literal and length/distance codes from z's input into s's
// window while at least 258 window bytes and 10 input bytes are left. The bit buffer and the pointers are loaded from s
// and z on entry and stored back on every return. Returns 1 at an end-of-block code, -3 (Z_DATA_ERROR, with z->msg
// set) at an invalid code, and 0 when it stops for lack of input or window space.
int __cdecl inflate_fast_guess(unsigned int bl, unsigned int bd, InflateHuft_guess *tl, InflateHuft_guess *td,
                               inflate_blocks_state *s, z_stream *z)
{
    InflateHuft_guess *t;           // temporary pointer
    unsigned int e;                 // extra bits or operation
    unsigned long b;                // bit buffer
    unsigned int k;                 // bits in the bit buffer
    unsigned char *p;               // input pointer
    unsigned int n;                 // bytes available at p
    unsigned char *q;               // window write pointer
    unsigned int m;                 // bytes to the end of the window or to the read pointer
    unsigned int ml;                // mask for the literal/length table
    unsigned int md;                // mask for the distance table
    unsigned int c;                 // bytes to copy
    unsigned int d;                 // distance back to copy from
    unsigned char *r;               // copy source pointer

    // load input, output and bit values
    p = z->next_in;
    n = z->avail_in;
    b = s->bitb;
    k = s->bitk;
    q = s->write;
    m = (unsigned int)(q < s->read ? s->read - q - 1 : s->end - q);

    // initialize masks
    ml = g_inflateMask_guess[bl];
    md = g_inflateMask_guess[bd];

    // do until not enough input or output space for the fast loop
    do
    {
        // get literal/length code
        while (k < 20)
        {
            b |= (unsigned long)(n--, *p++) << k;
            k += 8;
        }
        if ((e = (t = tl + ((unsigned int)b & ml))->exop) == 0)
        {
            b >>= t->bits;
            k -= t->bits;
            *q++ = (unsigned char)t->base;
            m--;
            continue;
        }
        do
        {
            b >>= t->bits;
            k -= t->bits;
            if (e & 16)
            {
                // get extra bits for the length
                e &= 15;
                c = t->base + ((unsigned int)b & g_inflateMask_guess[e]);
                b >>= e;
                k -= e;

                // decode the distance code
                while (k < 15)
                {
                    b |= (unsigned long)(n--, *p++) << k;
                    k += 8;
                }
                e = (t = td + ((unsigned int)b & md))->exop;
                do
                {
                    b >>= t->bits;
                    k -= t->bits;
                    if (e & 16)
                    {
                        // get extra bits to add to the distance base
                        e &= 15;
                        while (k < e)
                        {
                            b |= (unsigned long)(n--, *p++) << k;
                            k += 8;
                        }
                        d = t->base + ((unsigned int)b & g_inflateMask_guess[e]);
                        b >>= e;
                        k -= e;

                        // do the copy
                        m -= c;
                        if ((unsigned int)(q - s->window) >= d)
                        {
                            // offset before the destination: just copy
                            r = q - d;
                            *q++ = *r++;
                            c--;
                            *q++ = *r++;
                            c--;
                        }
                        else
                        {
                            // offset after the destination: the source may wrap past the window end
                            e = d - (q - s->window);
                            r = s->end - e;
                            if (c > e)
                            {
                                c -= e;
                                do
                                {
                                    *q++ = *r++;
                                } while (--e);
                                r = s->window;
                            }
                        }
                        do
                        {
                            *q++ = *r++;
                        } while (--c);
                        break;
                    }
                    else if ((e & 64) == 0)
                    {
                        t += t->base;
                        e = (t += ((unsigned int)b & g_inflateMask_guess[e]))->exop;
                    }
                    else
                    {
                        z->msg = g_eqmain_10046694_guess;
                        c = z->avail_in - n;
                        c = (k >> 3) < c ? k >> 3 : c;
                        n += c;
                        p -= c;
                        k -= c << 3;
                        s->bitb = b;
                        s->bitk = k;
                        z->avail_in = n;
                        z->total_in += p - z->next_in;
                        z->next_in = p;
                        s->write = q;
                        return -3;
                    }
                } while (1);
                break;
            }
            if ((e & 64) == 0)
            {
                t += t->base;
                if ((e = (t += ((unsigned int)b & g_inflateMask_guess[e]))->exop) == 0)
                {
                    b >>= t->bits;
                    k -= t->bits;
                    *q++ = (unsigned char)t->base;
                    m--;
                    break;
                }
            }
            else if (e & 32)
            {
                c = z->avail_in - n;
                c = (k >> 3) < c ? k >> 3 : c;
                n += c;
                p -= c;
                k -= c << 3;
                s->bitb = b;
                s->bitk = k;
                z->avail_in = n;
                z->total_in += p - z->next_in;
                z->next_in = p;
                s->write = q;
                return 1;
            }
            else
            {
                z->msg = g_eqmain_100466ac_guess;
                c = z->avail_in - n;
                c = (k >> 3) < c ? k >> 3 : c;
                n += c;
                p -= c;
                k -= c << 3;
                s->bitb = b;
                s->bitk = k;
                z->avail_in = n;
                z->total_in += p - z->next_in;
                z->next_in = p;
                s->write = q;
                return -3;
            }
        } while (1);
    } while (m >= 258 && n >= 10);

    // not enough input or output: store the state and return
    c = z->avail_in - n;
    c = (k >> 3) < c ? k >> 3 : c;
    n += c;
    p -= c;
    k -= c << 3;
    s->bitb = b;
    s->bitk = k;
    z->avail_in = n;
    z->total_in += p - z->next_in;
    z->next_in = p;
    s->write = q;
    return 0;
}

// The unit record ResetUnitRecord_guess clears. QueueUnitReset_guess takes the same record as its second argument (the
// CallbackRec_guess above: +0x24 fn, +0x28 arg); only the fields this function touches are named here.
struct UnitRecord_guess
{
    char unknown_00[0x08];          // 0x00 .. 0x07: not touched here
    int field_08;                   // 0x08: cleared
    char unknown_0c[0x08];          // 0x0C .. 0x13: not touched here
    int field_14;                   // 0x14: cleared
    int field_18;                   // 0x18: cleared
    unsigned int *unit;             // 0x1C: the unit whose flag word is set; its +0x14 object goes to the callee
};

// FUNCTION: EQMAIN 0x1000BDE0
// Clears a stream record and restarts its associated unit.
// __cdecl. Clears the record's +0x08, +0x14 and +0x18, sets the unit's flag word (+0x00) to 7 when its +0x0C is
// nonzero and to 0 when it is zero, then calls QueueUnitReset_guess with the unit's +0x14 object, the record and a null
// third argument. Returns 0; returns -2 without calling anything when the record or its unit is null.
int __cdecl ResetUnitRecord_guess(UnitRecord_guess *rec)
{
    unsigned int *unit;

    if (rec != 0 && (unit = rec->unit) != 0)
    {
        rec->field_14 = 0;
        rec->field_08 = 0;
        rec->field_18 = 0;
        *unit = unit[3] ? 7 : 0;
        EQ_CDECL(void, 0x1000A3E0, (int *, UnitRecord_guess *, int *))((int *)rec->unit[5], rec, 0);
        return 0;
    }
    return -2;
}

// FUNCTION: EQMAIN 0x1000BE30
// Frees the stream's decoder state (blocks, then z->state via zfree) and clears it (zlib inflateEnd by role).
// __cdecl. Releases the unit record: when the record has a unit (+0x1C) and a callback (+0x24), calls QueueUnitFlush_guess with
// the unit's +0x14 object and the record (only when that object is not null), then calls the callback with (+0x28, the
// unit) and clears +0x1C; returns 0. Returns -2 without calling anything when the record, its unit or its callback is
// null. The callback fields are read through a CallbackRec_guess view of the same record.
int __cdecl ReleaseUnitRecord_guess(UnitRecord_guess *rec)
{
    int *blocks;

    if (rec != 0 && rec->unit != 0 && ((CallbackRec_guess *)rec)->fn != 0)
    {
        blocks = (int *)rec->unit[5];
        if (blocks != 0)
        {
            EQ_CDECL(void, 0x1000B210, (int *, UnitRecord_guess *))(blocks, rec);
        }
        ((CallbackRec_guess *)rec)->fn(((CallbackRec_guess *)rec)->arg, (int)rec->unit);
        rec->unit = 0;
        return 0;
    }
    return -2;
}

// FUNCTION: EQMAIN 0x1000BE80
// Prepares a stream record and its output window for buffered data processing.
// __cdecl. Sets up the record's unit (+0x1C), a 0x18-byte block from the +0x20 allocator (a null allocator is set to
// 0x1000E580 and a null +0x24 callback to 0x1000E5A0). Returns -6 unless version is set, starts with '1' (the "1.1.2" at
// 0x1004660C) and size is 0x38, and -2 when rec is null. A negative bits is negated and sets the unit's +0x0C flag.
// Bits outside 8..15 release the record with ReleaseUnitRecord_guess and return -2. Otherwise the unit's +0x10 is bits and its +0x14
// window is QueueUnitCreate_guess(rec, flag ? 0 : 0x10009650, 1 << bits): a null window releases the record and returns -4, else
// ResetUnitRecord_guess resets the record and the result is 0. A null unit returns -4 with nothing else called.
int __cdecl InitUnitRecord_guess(UnitRecord_guess *rec, int bits, char *version, int size)
{
    int unitState;
    int *blocks;

    if (version == 0 || *version != g_zlibVersionChar_guess || size != 0x38)
    {
        return -6;
    }
    if (rec == 0)
    {
        return -2;
    }
    rec->field_18 = 0;
    if (((AllocRec_guess *)rec)->alloc == 0)
    {
        ((AllocRec_guess *)rec)->alloc = EQ_CDECL(void *, 0x1000E580, (int, int, int));
        ((AllocRec_guess *)rec)->arg = 0;
    }
    if (((CallbackRec_guess *)rec)->fn == 0)
    {
        ((CallbackRec_guess *)rec)->fn = EQ_CDECL(int, 0x1000E5A0, (int, int));
    }
    unitState = (int)((AllocRec_guess *)rec)->alloc(((AllocRec_guess *)rec)->arg, 1, 0x18);
    rec->unit = (unsigned int *)unitState;
    if (unitState == 0)
    {
        return -4;
    }
    ((unsigned int *)unitState)[5] = 0;
    rec->unit[3] = 0;
    if (bits < 0)
    {
        bits = -bits;
        rec->unit[3] = 1;
    }
    if (bits >= 8 && bits <= 15)
    {
        rec->unit[4] = bits;
        blocks = EQ_CDECL(int *, 0x1000A460, (UnitRecord_guess *, unsigned int (__cdecl *)(unsigned int, unsigned char *, unsigned int), int))(rec, (rec->unit[3] != 0) ? 0 : EQ_CDECL(unsigned int, 0x10009650, (unsigned int, unsigned char *, unsigned int)), 1 << bits);
        rec->unit[5] = (unsigned int)blocks;
        if (rec->unit[5] == 0)
        {
            EQ_CDECL(int, 0x1000BE30, (UnitRecord_guess *))(rec);
            return -4;
        }
        EQ_CDECL(int, 0x1000BDE0, (UnitRecord_guess *))(rec);
        return 0;
    }
    EQ_CDECL(int, 0x1000BE30, (UnitRecord_guess *))(rec);
    return -2;
}

// The head dword of a table entry as the original stores it: op, bits, and the high word the dword store carries along.
union HuftHead_guess
{
    unsigned int dw;
    struct
    {
        unsigned char e;    // 0x00: op
        unsigned char b;    // 0x01: bits
        unsigned short hi;  // 0x02: high word, carried through unchanged
    } f;
};

// FUNCTION: EQMAIN 0x1000BF90
// Provides the standard 15-bit window setup used by the one-shot decoder.
// __cdecl. Calls InitUnitRecord_guess (0x1000BE80) with bits 0xf, passing rec, version and size through, and returns its result.
int __cdecl InitUnitRecordBits15_guess(UnitRecord_guess *rec, char *version, int size)
{
    return EQ_CDECL(int, 0x1000BE80, (UnitRecord_guess *, int, char *, int))(rec, 0xf, version, size);
}

// The inflate state, laid out like zlib's internal_state. Only the fields the state machine below reads and writes.
struct InflateState_guess
{
    int mode;                   // 0x00: 0 METHOD, 1 FLAG, 2..5 DICT4..DICT1, 6 DICT0, 7 BLOCKS, 8..11 CHECK4..CHECK1, 12 DONE, 13 BAD
    union
    {
        unsigned int method;    // 0x04: METHOD and FLAG: the CMF byte
        struct
        {
            unsigned int was;   // 0x04: CHECK: the computed check value, written by QueueUnitReset_guess
            unsigned int need;  // 0x08: CHECK and DICT: the value read from the stream
        } check;
        unsigned int marker;    // 0x04: BAD: 5 (inflateSync can't help) or 0 (it may try)
    } sub;
    int nowrap;                 // 0x0C: no zlib header and no check value
    int wbits;                  // 0x10: log2 of the window size (8..15)
    void *blocks;               // 0x14: the inflate_blocks state (inflate_blocks_guess, QueueUnitReset_guess)
};

// The z_stream this state machine reads and writes (zlib's z_stream, 0x38 bytes), with the state pointer named. Separate from
// z_stream (the fast loop's view) and from z_stream (the trees' view), so neither depends on this.
/* z_stream: the actor layout is z_stream (generated: eqmain/layout/z_stream.h); the old local view's members map by offset. */
#include "eqmain/layout/z_stream.h"

// FUNCTION: EQMAIN 0x1000BFB0
// Decompresses a stream as input arrives: checks its header, expands its blocks, then verifies its checksum.
// __cdecl. zlib's inflate() shape: reads the zlib header (CMF, FLG and, when FLG has PRESET_DICT, a 4-byte dictionary id),
// runs the deflate blocks through inflate_blocks_guess (inflate_blocks) and then reads and checks the 4-byte check value unless nowrap.
// f is the flush value reduced to the no-progress result: Z_FINISH (4) gives -5 (Z_BUF_ERROR), any other flush 0 (Z_OK).
// Returns 2 (Z_NEED_DICT) once the dictionary id is read, 1 (Z_STREAM_END) when done, -2 (Z_STREAM_ERROR) for a NULL stream
// an unknown mode or a missing dictionary, -3 (Z_DATA_ERROR) in the BAD mode, otherwise the current r (-5 to start with).
int __cdecl Inflate_guess(z_stream *z, int f)
{
    int r;
    unsigned int b;

    if (z == 0 || z->inflateState_guess == 0 || z->next_in == 0)
        return -2;
    f = (f == 4) ? -5 : 0;
    r = -5;
    for (;;)
    {
        switch (z->inflateState_guess->mode)
        {
        case 0:                 // METHOD: the CMF byte
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.method = *z->next_in++;
            if ((z->inflateState_guess->sub.method & 0xf) != 8)
            {
                z->inflateState_guess->mode = 13;
                z->msg = EQ_ARRAY(char, 0x1004671C);
                z->inflateState_guess->sub.marker = 5;
                break;
            }
            if ((z->inflateState_guess->sub.method >> 4) + 8 > (unsigned int)z->inflateState_guess->wbits)
            {
                z->inflateState_guess->mode = 13;
                z->msg = EQ_ARRAY(char, 0x10046708);
                z->inflateState_guess->sub.marker = 5;
                break;
            }
            z->inflateState_guess->mode = 1;
            // fall through
        case 1:                 // FLAG: the FLG byte, checked against CMF
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            b = *z->next_in++;
            if (((z->inflateState_guess->sub.method << 8) + b) % 31)
            {
                z->inflateState_guess->mode = 13;
                z->msg = EQ_ARRAY(char, 0x100466F0);
                z->inflateState_guess->sub.marker = 5;
                break;
            }
            if ((b & 0x20) == 0)
            {
                z->inflateState_guess->mode = 7;
                break;
            }
            z->inflateState_guess->mode = 2;
            goto dict4;
        case 7:                 // BLOCKS: the deflate blocks
            r = EQ_CDECL(int, 0x1000A500, (void *, z_stream *, int))(z->inflateState_guess->blocks, z, r);
            if (r == -3)
            {
                z->inflateState_guess->mode = 13;
                z->inflateState_guess->sub.marker = 0;
                break;
            }
            if (r == 0)
                r = f;
            if (r != 1)
                return r;
            r = f;
            EQ_CDECL(void, 0x1000A3E0, (void *, z_stream *, unsigned int *))
                (z->inflateState_guess->blocks, z, &z->inflateState_guess->sub.check.was);
            if (z->inflateState_guess->nowrap)
            {
                z->inflateState_guess->mode = 12;
                break;
            }
            z->inflateState_guess->mode = 8;
            // fall through
        case 8:                 // CHECK4: the check value, most significant byte first
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need = (unsigned int)*z->next_in++ << 24;
            z->inflateState_guess->mode = 9;
            // fall through
        case 9:                 // CHECK3
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need += (unsigned int)*z->next_in++ << 16;
            z->inflateState_guess->mode = 10;
            // fall through
        case 10:                // CHECK2
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need += (unsigned int)*z->next_in++ << 8;
            z->inflateState_guess->mode = 11;
            // fall through
        case 11:                // CHECK1: the last byte, then the compare
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need += *z->next_in++;
            if (z->inflateState_guess->sub.check.was != z->inflateState_guess->sub.check.need)
            {
                z->inflateState_guess->mode = 13;
                z->msg = EQ_ARRAY(char, 0x100466D8);
                z->inflateState_guess->sub.marker = 5;
                break;
            }
            z->inflateState_guess->mode = 12;
            return 1;
        case 2:                 // DICT4: the dictionary id, most significant byte first
        dict4:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need = (unsigned int)*z->next_in++ << 24;
            z->inflateState_guess->mode = 3;
            // fall through
        case 3:                 // DICT3
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need += (unsigned int)*z->next_in++ << 16;
            z->inflateState_guess->mode = 4;
            // fall through
        case 4:                 // DICT2
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need += (unsigned int)*z->next_in++ << 8;
            z->inflateState_guess->mode = 5;
            // fall through
        case 5:                 // DICT1: the last byte; the caller has to set the dictionary
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            z->inflateState_guess->sub.check.need += *z->next_in++;
            z->adler = z->inflateState_guess->sub.check.need;
            z->inflateState_guess->mode = 6;
            return 2;
        case 6:                 // DICT0: no dictionary was set
            z->inflateState_guess->mode = 13;
            z->msg = EQ_ARRAY(char, 0x100466C8);
            z->inflateState_guess->sub.marker = 0;
            return -2;
        case 12:                // DONE
            return 1;
        case 13:                // BAD
            return -3;
        default:
            return -2;
        }
    }
}

// The stream the trees are built on, laid out like zlib's z_stream. Only the error text and the allocator fields are used.
/* z_stream: the actor layout is z_stream (generated: eqmain/layout/z_stream.h); the old local view's members map by offset. */
#include "eqmain/layout/z_stream.h"

// FUNCTION: EQMAIN 0x1000C3E0
// Builds the lookup table for reading code lengths in a dynamic compressed block.
// __cdecl. zlib's inflate_trees_bits shape: builds the bit-length tree of a dynamic block from its 19 code lengths
// (HuftBuild_guess). The 19-entry work array comes from z->zalloc and goes back through z->zfree; z->msg gets the error
// text for an over-subscribed tree (-3) and for an incomplete one (-5, or a zero lookup depth). Returns -4 when the work
// array can't be allocated, otherwise HuftBuild_guess's result, with -3 for an incomplete tree.
int __cdecl InflateTreesBits_guess(int *codeLens, unsigned int *bitsOut, unsigned int *tableOut, int space,
                                   z_stream *z)
{
    unsigned int *work;
    int rc;
    unsigned int spaceUsed;

    spaceUsed = 0;
    work = (unsigned int *)z->zalloc(z->opaque, 0x13, 4);
    if (work == 0)
    {
        return -4;
    }
    rc = EQ_CDECL(int, 0x1000C490, (int *, unsigned int, unsigned int, int, int, unsigned int *, unsigned int *, int,
                                    unsigned int *, unsigned int *))
        (codeLens, 0x13, 0x13, 0, 0, tableOut, bitsOut, space, &spaceUsed, work);
    if (rc == -3)
    {
        z->msg = (char *)0x10047864;
        z->zfree(z->opaque, work);
        return rc;
    }
    if (rc == -5 || *bitsOut == 0)
    {
        z->msg = (char *)0x10047840;
        rc = -3;
    }
    z->zfree(z->opaque, work);
    return rc;
}

// FUNCTION: EQMAIN 0x1000C490
// Shared table builder: turns code lengths into a Huffman lookup table for the decompressor.
// __cdecl. Builds the Huffman decoding table for numCodes code lengths (zlib's huft_build shape). Entries are 8 bytes,
// taken from space after the *spaceUsed already used (0x5A0 at most); the first table goes to *result and its lookup
// bits to *bitsOut. Returns 0 (also for an all-zero code), -3 for an over-subscribed or incomplete code, -4 when the
// space runs out, -5 for an incomplete code longer than one bit.
int __cdecl HuftBuild_guess(int *codeLens, unsigned int numCodes, unsigned int simpleCount, int baseVals,
                            int extraVals, unsigned int *result, unsigned int *bitsOut, int space,
                            unsigned int *spaceUsed, unsigned int *work)
{
    unsigned int c[47];      // [0x00..0x0F] length counts, [0x10..0x1F] offsets, [0x20..0x2E] table stack
    HuftHead_guess r;
    unsigned int rWord;
    unsigned short rHigh;
    unsigned int tmp;
    unsigned int doubled;
    unsigned int value;
    unsigned int slot;
    unsigned int j;
    unsigned int g;
    unsigned int a;
    unsigned int other;
    unsigned int l;
    unsigned int k;
    int level;
    unsigned int *p;
    int kMinus1;
    unsigned int *cPtr;
    unsigned int rBase;
    unsigned int i;
    unsigned int z;
    unsigned int q;
    unsigned char wByte;
    int acc;
    int w;
    int scratch;
    int dummyCodes;
    int *lenPtr;
    unsigned int *ptr;
    unsigned int *entry;

    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
    c[3] = 0;
    c[4] = 0;
    c[5] = 0;
    c[6] = 0;
    c[7] = 0;
    c[8] = 0;
    c[9] = 0;
    c[10] = 0;
    c[11] = 0;
    c[12] = 0;
    c[13] = 0;
    c[14] = 0;
    c[15] = 0;
    lenPtr = codeLens;
    g = numCodes;
    do
    {
        dummyCodes = *lenPtr;
        lenPtr = lenPtr + 1;
        g = g - 1;
        c[dummyCodes] = c[dummyCodes] + 1;
    } while (g != 0);
    if (c[0] == numCodes)
    {
        *result = 0;
        *bitsOut = 0;
    }
    else
    {
        k = 1;
        ptr = c;
        do
        {
            ptr = ptr + 1;
            if (*ptr != 0)
                break;
            k = k + 1;
        } while (k < 0x10);
        l = *bitsOut;
        if (*bitsOut < k)
        {
            l = k;
        }
        g = 0xf;
        ptr = c + 0xf;
        do
        {
            if (*ptr != 0)
                break;
            g = g - 1;
            ptr = ptr + -1;
        } while (g != 0);
        if (g < l)
        {
            l = g;
        }
        *bitsOut = l;
        dummyCodes = 1 << ((unsigned char)k & 0x1f);
        if (k < g)
        {
            ptr = c + k;
            slot = k;
            do
            {
                value = *ptr;
                if ((int)(dummyCodes - value) < 0)
                {
                    return -3;
                }
                slot = slot + 1;
                ptr = ptr + 1;
                dummyCodes = (dummyCodes - value) * 2;
            } while (slot < g);
        }
        dummyCodes = dummyCodes - c[g];
        if (dummyCodes < 0)
        {
            return -3;
        }
        c[0x11] = 0;
        c[g] = c[g] + dummyCodes;
        w = 0;
        scratch = g - 1;
        if (scratch != 0)
        {
            acc = 0;
            do
            {
                w = w + *(int *)((char *)c + acc + 4);
                scratch = scratch + -1;
                *(int *)((char *)c + acc + 0x48) = w;
                acc = acc + 4;
            } while (scratch != 0);
        }
        slot = 0;
        do
        {
            w = *codeLens;
            codeLens = codeLens + 1;
            if (w != 0)
            {
                value = c[w + 0x10];
                work[value] = slot;
                c[w + 0x10] = value + 1;
            }
            slot = slot + 1;
        } while (slot < numCodes);
        w = -(int)l;
        slot = c[g + 0x10];
        p = work;
        i = 0;
        c[0x10] = 0;
        level = -1;
        c[0x20] = 0;
        q = 0;
        z = 0;
        if ((int)k <= (int)g)
        {
            kMinus1 = k - 1;
            cPtr = c + k;
            do
            {
                value = *cPtr;
                rWord = r.dw;
                while (value != 0)
                {
                    a = value - 1;
                    r.f.hi = (unsigned short)(rWord >> 16);
                    rHigh = r.f.hi;
                    scratch = w;
                    while (r.dw = rWord, scratch = scratch + (int)l, scratch < (int)k)
                    {
                        w = w + (int)l;
                        other = g - w;
                        if (l < g - w)
                        {
                            other = l;
                        }
                        j = k - w;
                        tmp = 1 << ((unsigned char)j & 0x1f);
                        if ((value < tmp) &&
                            (acc = tmp + (-1 - a), ptr = cPtr, j < other))
                        {
                            while (j = j + 1, j < other)
                            {
                                tmp = ptr[1];
                                doubled = acc * 2;
                                if (doubled < tmp || doubled - tmp == 0)
                                    break;
                                acc = doubled - tmp;
                                ptr = ptr + 1;
                            }
                        }
                        z = 1 << ((unsigned char)j & 0x1f);
                        other = *spaceUsed;
                        tmp = z + other;
                        if (0x5a0 < tmp)
                        {
                            return -4;
                        }
                        *spaceUsed = tmp;
                        q = space + other * 8;
                        c[level + 0x21] = q;
                        if (level + 1 == 0)
                        {
                            *result = q;
                        }
                        else
                        {
                            c[level + 0x11] = i;
                            r.f.hi = (unsigned short)(r.dw >> 16);
                            r.f.e = (unsigned char)j;
                            r.f.b = (unsigned char)l;
                            j = i >> (((char)w - (char)l) & 0x1f);
                            other = c[level + 0x20];
                            rBase = ((int)(q - other) >> 3) - j;
                            *(unsigned int *)(other + j * 8) = r.dw;
                            *(unsigned int *)(other + 4 + j * 8) = rBase;
                        }
                        other = i;
                        level = level + 1;
                        rWord = r.dw;
                        rHigh = r.f.hi;
                    }
                    wByte = (unsigned char)w;
                    if (p < work + slot)
                    {
                        rBase = *p;
                        if (rBase < simpleCount)
                        {
                            r.f.e = (unsigned char)((-(int)(rBase < 0x100) & 0xa0U) + 0x60);
                        }
                        else
                        {
                            scratch = (rBase - simpleCount) * 4;
                            r.f.e = (unsigned char)(*(char *)(scratch + extraVals) + 'P');
                            rBase = *(unsigned int *)(scratch + baseVals);
                        }
                        p = p + 1;
                    }
                    else
                    {
                        r.f.e = 0xc0;
                    }
                    r.f.b = (unsigned char)((char)k - wByte);
                    r.f.hi = rHigh;
                    scratch = 1 << (((char)k - wByte) & 0x1f);
                    value = other >> (wByte & 0x1f);
                    if (value < z)
                    {
                        entry = (unsigned int *)(q + value * 8);
                        do
                        {
                            value = value + scratch;
                            *entry = r.dw;
                            entry[1] = rBase;
                            entry = entry + scratch * 2;
                        } while (value < z);
                    }
                    j = 1 << ((unsigned char)kMinus1 & 0x1f);
                    value = other & j;
                    while (value != 0)
                    {
                        other = other ^ j;
                        j = j >> 1;
                        value = other & j;
                    }
                    other = other ^ j;
                    ptr = c + level + 0x10;
                    value = a;
                    rWord = r.dw;
                    i = other;
                    if (((1 << (wByte & 0x1f)) - 1U & other) != c[level + 0x10])
                    {
                        do
                        {
                            level = level + -1;
                            w = w - (int)l;
                            ptr = ptr + -1;
                        } while (((1 << ((unsigned char)w & 0x1f)) - 1U & other) != *ptr);
                    }
                }
                cPtr = cPtr + 1;
                k = k + 1;
                kMinus1 = kMinus1 + 1;
                r.dw = rWord;
            } while ((int)k <= (int)g);
        }
        if ((dummyCodes != 0) && (g != 1))
        {
            return -5;
        }
    }
    return 0;
}

// FUNCTION: EQMAIN 0x1000C960
// Builds the literal and distance lookup tables for decoding a dynamic block.
// __cdecl. zlib's inflate_trees_dynamic shape: builds the literal/length tree from the first numLit code lengths (257
// simple codes) and the distance tree from the numDist lengths after them, both in the 288-entry work array from
// z->zalloc. Returns -4 when the work array can't be allocated and 0 when both trees are built. Otherwise returns -3
// with z->msg set to the failed tree's error text; a -4 from HuftBuild_guess is returned as it is, with no text.
// @early-stop: register choice: VC6 gives rc EBX and z ESI (original z EBX, rc ESI); the -5 and -4 tails are laid out differently.
int __cdecl InflateTreesDynamic_guess(unsigned int numLit, unsigned int numDist, int *codeLens, unsigned int *litBits,
                                      unsigned int *distBits, unsigned int *litTable, unsigned int *distTable, int space,
                                      z_stream *z)
{
    int rc;
    unsigned int spaceUsed;
    unsigned int *work;

    spaceUsed = 0;
    work = (unsigned int *)z->zalloc(z->opaque, 0x120, 4);
    if (work == 0)
    {
        return -4;
    }
    rc = EQ_CDECL(int, 0x1000C490, (int *, unsigned int, unsigned int, int, int, unsigned int *, unsigned int *, int,
                                    unsigned int *, unsigned int *))
        (codeLens, numLit, 0x101, 0x10040f30, 0x10040fac, litTable, litBits, space, &spaceUsed, work);
    if (rc == 0)
    {
        if (*litBits != 0)
        {
            rc = EQ_CDECL(int, 0x1000C490, (int *, unsigned int, unsigned int, int, int, unsigned int *, unsigned int *, int,
                                            unsigned int *, unsigned int *))
                (codeLens + numLit, numDist, 0, 0x10041028, 0x100410a0, distTable, distBits, space, &spaceUsed, work);
            if (rc == 0)
            {
                if (*distBits != 0 || numLit <= 0x101)
                {
                    z->zfree(z->opaque, work);
                    return 0;
                }
            }
            else
            {
                if (rc == -3)
                {
                    z->msg = (char *)0x10047910;
                    z->zfree(z->opaque, work);
                    return rc;
                }
                if (rc == -5)
                {
                    z->msg = (char *)0x100478F4;
                    rc = -3;
                    z->zfree(z->opaque, work);
                    return rc;
                }
                if (rc == -4)
                {
                    // as original: -4 skips the msg and rc = -3 stores, straight to the zfree/return tail
                    goto freeWorkAndReturn;
                }
            }
            z->msg = (char *)0x100478D0;
            rc = -3;
freeWorkAndReturn:
            z->zfree(z->opaque, work);
            return rc;
        }
    }
    else
    {
        if (rc == -3)
        {
            z->msg = (char *)0x100478AC;
            z->zfree(z->opaque, work);
            return rc;
        }
        if (rc == -4)
        {
            // as original: -4 skips the msg and rc = -3 stores, straight to the zfree/return tail
            goto freeWorkAndReturnFromLiteralError;
        }
    }
    z->msg = (char *)0x1004788C;
    rc = -3;
freeWorkAndReturnFromLiteralError:
    z->zfree(z->opaque, work);
    return rc;
}

// FUNCTION: EQMAIN 0x1000CAF0
// Supplies preset bit depths and table addresses for compressed-stream decoding.
// __cdecl. Stores two globals' values through the first two pointers and the addresses of two other globals
// through the last two; returns 0.
int __cdecl FetchUnitGlobals_guess(unsigned int* out1, unsigned int* out2, unsigned int* out3, unsigned int* out4)
{
    *out1 = g_eqmain_10046738_guess;
    *out2 = g_eqmain_1004673c_guess;
    *out3 = (unsigned int)g_eqmain_10046740_guess;
    *out4 = (unsigned int)g_eqmain_10047740_guess;
    return 0;
}

// The ring the bytes come from. The data runs from head to tail and wraps at end; base is the first byte. hook is
// called on each run with its state, and the result becomes the new state. Only these fields are read here.
/* inflate_blocks_state: the actor layout is inflate_blocks_state (generated: eqmain/layout/inflate_blocks_state.h); the old local view's members map by offset. */
#include "eqmain/layout/inflate_blocks_state.h"

// The output stream, laid out like zlib's z_stream (next_out, avail_out, total_out; 0x30 is its adler). Only these
// fields are read here.
/* z_stream: the actor layout is z_stream (generated: eqmain/layout/z_stream.h); the old local view's members map by offset. */
#include "eqmain/layout/z_stream.h"

// FUNCTION: EQMAIN 0x1000CB20
// Makes pending decoded bytes available in the stream's output buffer.
// @early-stop: first-run cursor update uses ESI instead of EDI; one wrap-path store remains different
// __cdecl. Copies pending bytes from the ring into the stream's output: at most avail_out of them, first the run from
// head to tail (to the end when the data wraps), then, if that run reached the end, the run from base to tail. Each
// run goes to the ring's hook with the hook's state (called even for an empty run); the hook's result is kept as the
// new state and in the stream's hook_result. The ring's head moves past what was copied. Returns flush, with -5
// (zlib's Z_BUF_ERROR) turned to 0 once any byte moved.
int __cdecl inflate_flush_guess(inflate_blocks_state *s, z_stream *strm, int flush)
{
    unsigned char *tail;
    unsigned char *out;
    unsigned char *head;
    unsigned char *next;
    unsigned int n;
    unsigned int avail;
    unsigned int hook_result;

    out = strm->next_out;
    tail = s->write;
    head = s->read;
    if (head > tail)
    {
        tail = s->end;
    }
    avail = strm->avail_out;
    n = (unsigned int)tail - (unsigned int)head;
    if (n > avail)
    {
        n = avail;
    }
    if (n != 0 && flush == -5)
    {
        flush = 0;
    }
    strm->avail_out = avail - n;
    strm->total_out += n;
    if (s->checkfn != 0)
    {
        hook_result = s->checkfn(s->check, head, n);
        s->check = hook_result;
        strm->adler = hook_result;
    }
    memcpy(out, head, n);
    next = head + n;
    out += n;
    if (next == s->end)
    {
        tail = s->window;
        if (s->write == s->end)
        {
            s->write = tail;
        }
        n = (unsigned int)s->write - (unsigned int)tail;
        avail = strm->avail_out;
        if (n > avail)
        {
            n = avail;
        }
        if (n != 0 && flush == -5)
        {
            flush = 0;
        }
        strm->avail_out = avail - n;
        strm->total_out += n;
        if (s->checkfn != 0)
        {
            hook_result = s->checkfn(s->check, tail, n);
            s->check = hook_result;
            strm->adler = hook_result;
        }
        head = tail;
        memcpy(out, head, n);
        out += n;
        next = tail + n;
    }
    strm->next_out = out;
    s->read = next;
    return flush;
}

// The Huffman heap of zlib's deflate_state, as the sift reads it: heap[1] .. heap[heap_len] hold node numbers, depth[]
// their tie-break depths. Only these fields are named here.
/* zlib deflate_state and its tree types (deflate_state -> deflate_state, CtData_guess -> ct_data, StaticTreeDesc_guess, TreeDesc_guess -> tree_desc): generated in eqmain/layout/deflate_state.h; the old local views' members map by offset. */
#include "eqmain/layout/deflate_state.h"

// zlib's ct_data: one 4-byte node of the Huffman tree. Only freq is read here.
/* CtData_guess (ct_data): generated in eqmain/layout/deflate_state.h. */
#include "eqmain/layout/deflate_state.h"

// FUNCTION: EQMAIN 0x1000CC60
// Resets symbol counts so the next compression block starts fresh.
// __cdecl. zlib's init_block (a guess from the deflate_state offsets). Clears the freq of the 286 literal/length nodes
// at 0x8c, the 30 distance nodes at 0x980 and the 19 bit-length nodes at 0xa74 (4-byte nodes: only each node's u16 freq
// is written), sets the END_BLOCK node's freq (0x48c) to 1, then clears the four 32-bit counters at 0x16a4, 0x16a0,
// 0x16ac and 0x1698, in that order. Returns nothing.
void __cdecl init_block_guess(deflate_state *s)
{
    unsigned short *p;
    int n;

    n = 0x11e;
    p = (unsigned short *)((char *)s + 0x8c);
    do
    {
        *p = 0;
        p = p + 2;
        n = n - 1;
    } while (n != 0);
    p = (unsigned short *)((char *)s + 0x980);
    n = 0x1e;
    do
    {
        *p = 0;
        p = p + 2;
        n = n - 1;
    } while (n != 0);
    p = (unsigned short *)((char *)s + 0xa74);
    n = 0x13;
    do
    {
        *p = 0;
        p = p + 2;
        n = n - 1;
    } while (n != 0);
    s->static_len = 0;
    s->opt_len = 0;
    s->matches = 0;
    s->last_lit = 0;
    *(unsigned short *)((char *)s + 0x48c) = 1;
}

// FUNCTION: EQMAIN 0x1000CCD0
// Writes a completed input block as raw bytes with a stored-block header.
// __cdecl. zlib's _tr_stored_block (a guess from the deflate_state offsets). Sends the 3-bit block type `eof` through the
// bit buffer: when bi_valid (0x16b8) is above 13 the 16-bit bi_buf (0x16b4) fills, so its two bytes go to pending_buf (0x8)
// at pending (0x14) and the bits of eof that did not fit are kept for the next byte; else the 3 bits are ORed in. Then
// compressed_len (0x16a8) becomes its value + 3 bits rounded up to a byte, plus the 4-byte LEN/NLEN
// header and stored_len bytes (in bits); then copy_block (0x1000E440) copies the stored block with its header. Returns nothing.
void __cdecl tr_stored_block_guess(deflate_state *s, unsigned char *buf, int stored_len, int eof)
{
    int bi_valid;
    bi_valid = s->bi_valid;
    if (bi_valid > 13)
    {
        s->bi_buf |= (unsigned short)(eof << bi_valid);
        (s->pending_buf)[s->pending] = s->bi_buf_bytes_guess[0];
        s->pending = s->pending + 1;
        (s->pending_buf)[s->pending] =
            (unsigned char)(s->bi_buf >> 8);
        bi_valid = s->bi_valid;
        s->pending = s->pending + 1;
        s->bi_buf = (unsigned short)eof >> (16 - bi_valid);
        s->bi_valid = bi_valid - 13;
    }
    else
    {
        s->bi_buf |= (unsigned short)(eof << bi_valid);
        s->bi_valid = bi_valid + 3;
    }
    s->compressed_len = ((s->compressed_len + 10) & 0xfffffff8) + 0x20 + stored_len * 8;
    EQ_CDECL(void, 0x1000E440, (deflate_state *, unsigned char *, int, int))(s, buf, stored_len, 1);
}

// The static half of zlib's tree_desc (static_tree_desc). Only the static tree and its element count are read here.
/* StaticTreeDesc_guess: generated in eqmain/layout/deflate_state.h. */
#include "eqmain/layout/deflate_state.h"

// zlib's tree_desc: the tree being built, the largest code with a non-zero freq (written here) and the static half.
/* TreeDesc_guess (tree_desc): generated in eqmain/layout/deflate_state.h. */
#include "eqmain/layout/deflate_state.h"

// FUNCTION: EQMAIN 0x1000CD90
// Chooses and emits a stored, fixed-code, or dynamic-code form for a completed block.
// @early-stop: len is in EAX here (ECX in the original), so the join's += swaps its registers; the 2nd put-byte prints [ecx+ebx].
// __cdecl. zlib's _tr_flush_block (a guess from the deflate_state offsets). At level (0x7c) above 0 it runs detect_data_type
// (0x1000E320) when data_type (0x1c) is 2, builds the literal and distance trees (0x1000CFC0) and the bit-length tree
// (0x1000D590), and takes the shorter of opt_len (0x16a0) and static_len (0x16a4), each + 10 >> 3 in bytes; at level 0 both
// are stored_len + 5. A stored block (0x1000CCD0, no length added) when stored_len + 4 fits and buf is set. Else the static
// trees when the two lengths are equal, or the dynamic trees (0x1000D6F0 sends them, 0x1000DEE0 compresses), each after its
// 3-bit header (eof + 2 or eof + 4) through the bit buffer as in tr_stored_block_guess, and compressed_len (0x16a8) gains the
// tree length + 3. Then init_block (0x1000CC60); when eof, bi_windup (0x1000E3C0) and + 7. Returns compressed_len >> 3.
unsigned int __cdecl tr_flush_block_guess(deflate_state *s, unsigned char *buf, int stored_len, int eof)
{
    int max_blindex;
    unsigned int opt_lenb;
    unsigned int static_lenb;
    unsigned int len;

    max_blindex = 0;
    if (s->level > 0)
    {
        if (s->data_type == 2)
        {
            EQ_CDECL(void, 0x1000E320, (deflate_state *))(s);
        }
        EQ_CDECL(void, 0x1000CFC0, (deflate_state *, TreeDesc_guess *))(s, &s->l_desc);
        EQ_CDECL(void, 0x1000CFC0, (deflate_state *, TreeDesc_guess *))(s, &s->d_desc);
        max_blindex = EQ_CDECL(int, 0x1000D590, (deflate_state *))(s);
        opt_lenb = (s->opt_len + 10) >> 3;
        static_lenb = (s->static_len + 10) >> 3;
        if (static_lenb <= opt_lenb)
        {
            opt_lenb = static_lenb;
        }
    }
    else
    {
        opt_lenb = static_lenb = stored_len + 5;
    }
    /* fake match: the stored-block test is computed into a local first, as the original evaluates it */
    int stored_fits = stored_len + 4U <= opt_lenb && buf != 0;
    if (stored_fits)
    {
        EQ_CDECL(void, 0x1000CCD0, (deflate_state *, unsigned char *, int, int))(s, buf, stored_len, eof);
    }
    else
    {
        if (static_lenb == opt_lenb)
        {
            if (s->bi_valid > 13)
            {
                s->bi_buf |= (unsigned short)((eof + 2) << s->bi_valid);
                (s->pending_buf)[s->pending] = s->bi_buf_bytes_guess[0];
                s->pending = s->pending + 1;
                (s->pending_buf)[s->pending] = (unsigned char)(s->bi_buf >> 8);
                s->pending = s->pending + 1;
                s->bi_buf = (unsigned short)(eof + 2) >> (16 - s->bi_valid);
                s->bi_valid = s->bi_valid - 13;
            }
            else
            {
                s->bi_buf |= (unsigned short)((eof + 2) << s->bi_valid);
                s->bi_valid = s->bi_valid + 3;
            }
            EQ_CDECL(void, 0x1000DEE0, (deflate_state *, CtData_guess *, CtData_guess *))(s, EQ_ARRAY(CtData_guess, 0x10041264), EQ_ARRAY(CtData_guess, 0x100416E4));
            len = s->static_len;
        }
        else
        {
            if (s->bi_valid > 13)
            {
                s->bi_buf |= (unsigned short)((eof + 4) << s->bi_valid);
                (s->pending_buf)[s->pending] = s->bi_buf_bytes_guess[0];
                s->pending = s->pending + 1;
                (s->pending_buf)[s->pending] = (unsigned char)(s->bi_buf >> 8);
                s->pending = s->pending + 1;
                s->bi_buf = (unsigned short)(eof + 4) >> (16 - s->bi_valid);
                s->bi_valid = s->bi_valid - 13;
            }
            else
            {
                s->bi_buf |= (unsigned short)((eof + 4) << s->bi_valid);
                s->bi_valid = s->bi_valid + 3;
            }
            EQ_CDECL(void, 0x1000D6F0, (deflate_state *, int, int, int))(s, s->l_desc.max_code + 1, s->d_desc.max_code + 1, max_blindex + 1);
            EQ_CDECL(void, 0x1000DEE0, (deflate_state *, CtData_guess *, CtData_guess *))(s, &s->dyn_ltree[0], &s->dyn_dtree[0]);
            len = s->opt_len;
        }
        s->compressed_len += len + 3;
    }
    EQ_CDECL(void, 0x1000CC60, (deflate_state *))(s);
    if (eof != 0)
    {
        EQ_CDECL(void, 0x1000E3C0, (deflate_state *))(s);
        s->compressed_len += 7;
    }
    return s->compressed_len >> 3;
}

// FUNCTION: EQMAIN 0x1000CFC0
// Builds variable-length codes for one set of symbols in a compressed block.
// __cdecl. zlib's build_tree (a guess from the deflate_state and tree_desc offsets). Heaps the tree's nodes with a non-zero
// freq (the others get len 0); when fewer than two are used, nodes 0 and 1 are forced in (freq 1, depth 0, opt_len--, and
// static_len less their static len). desc->max_code is the largest code used. The two least nodes are merged, again and
// again, into the nodes elems, elems + 1, ... (freq is their sum, depth one more than the deeper one, dads set), and the
// root goes to heap[heap_max]. gen_bitlen (0x1000D2E0) and gen_codes (0x1000D510) then fill in the lengths and codes.
// Returns nothing.
void __cdecl build_tree_guess(deflate_state *s, TreeDesc_guess *desc)
{
    CtData_guess *tree;
    CtData_guess *stree;
    int elems;
    int n;
    int m;
    int max_code;
    int node;

    tree = desc->dyn_tree;
    stree = desc->stat_desc->static_tree;
    elems = desc->stat_desc->elems;
    max_code = -1;
    s->heap_len = 0;
    s->heap_max = 573;
    for (n = 0; n < elems; n++)
    {
        if (tree[n].freq != 0)
        {
            s->heap[++(s->heap_len)] = max_code = n;
            s->depth[n] = 0;
        }
        else
        {
            tree[n].dad = 0;    // the u16 at +0x02 is zlib's len (the dl union with dad)
        }
    }
    while (s->heap_len < 2)
    {
        node = s->heap[++(s->heap_len)] = (max_code < 2 ? ++max_code : 0);
        tree[node].freq = 1;
        s->depth[node] = 0;
        (s->opt_len)--;
        if (stree != 0)
        {
            s->static_len -= stree[node].dad;
        }
    }
    desc->max_code = max_code;
    for (n = s->heap_len / 2; n >= 1; n--)
    {
        EQ_CDECL(void, 0x1000D200, (deflate_state *, CtData_guess *, int))(s, tree, n);
    }
    node = elems;
    do
    {
        n = s->heap[1];
        s->heap[1] = s->heap[s->heap_len--];
        EQ_CDECL(void, 0x1000D200, (deflate_state *, CtData_guess *, int))(s, tree, 1);
        m = s->heap[1];
        s->heap[--(s->heap_max)] = n;
        s->heap[--(s->heap_max)] = m;
        tree[node].freq = tree[m].freq + tree[n].freq;
        s->depth[node] = (unsigned char)(((unsigned int)s->depth[n] >= (unsigned int)s->depth[m]
            ? (unsigned int)s->depth[n] : (unsigned int)s->depth[m]) + 1);
        tree[n].dad = tree[m].dad = (unsigned short)node;
        s->heap[1] = node++;
        EQ_CDECL(void, 0x1000D200, (deflate_state *, CtData_guess *, int))(s, tree, 1);
    } while (s->heap_len >= 2);
    s->heap[--(s->heap_max)] = s->heap[1];
    EQ_CDECL(void, 0x1000D2E0, (deflate_state *, TreeDesc_guess *))(s, desc);
    EQ_CDECL(void, 0x1000D510, (CtData_guess *, int, unsigned short *))(tree, max_code, &s->bl_count[0]);
}

// FUNCTION: EQMAIN 0x1000D200
// Keeps the lowest-frequency symbols ready to combine into compression codes.
// __cdecl. zlib's pqdownheap: sifts heap[k] down. v = heap[k] moves down while its smaller son is smaller than v. The
// son is the right one when that one compares smaller (lower freq, or equal freq and depth <=), else the left one.
// Stops when v is smaller than that son (lower freq, or equal freq and depth <=) or when no son is left; v is stored
// in the last slot reached.
void __cdecl pqdownheap_guess(deflate_state *s, CtData_guess *tree, int k)
{
    int v;
    int j;

    v = s->heap[k];
    j = k << 1;
    while (j <= s->heap_len)
    {
        if (j < s->heap_len)
        {
            if (tree[s->heap[j + 1]].freq < tree[s->heap[j]].freq ||
                (tree[s->heap[j + 1]].freq == tree[s->heap[j]].freq && s->depth[s->heap[j + 1]] <= s->depth[s->heap[j]]))
            {
                j++;
            }
        }
        if (tree[v].freq < tree[s->heap[j]].freq ||
            (tree[v].freq == tree[s->heap[j]].freq && s->depth[v] <= s->depth[s->heap[j]]))
        {
            break;
        }
        s->heap[k] = s->heap[j];
        k = j;
        j <<= 1;
    }
    s->heap[k] = v;
}

// FUNCTION: EQMAIN 0x1000D2E0
// Assigns bounded code lengths to symbols in a compression tree.
// __cdecl. zlib's gen_bitlen. Each heap node from heap_max + 1 up to 573 takes its parent's length + 1, the parent being
// the node named by its Dad word, which its own Len then overwrites; a length over max_length is clamped to it and counted
// in overflow. Each leaf (node <= max_code) is counted by length in bl_count (0xb34) and adds freq * (len + extra bits) to
// opt_len (0x16a0), plus freq * (static len + extra bits) to static_len (0x16a4) when a static tree is set. With overflow,
// one leaf moves down per step (overflow -= 2) from the nearest length below max_length that has leaves; then the counts
// are dealt out from max_length down over the leaves at the top of the heap, and each leaf whose length changed adds
// (new - old) * freq to opt_len. Returns nothing.
void __cdecl gen_bitlen_guess(deflate_state *s, TreeDesc_guess *desc)
{
    CtData_guess *tree;
    int max_code;
    CtData_guess *stree;
    int *extra;
    int base;
    int max_length;
    int h;
    int n;
    int m;
    int bits;
    int xbits;
    unsigned short f;
    int overflow;

    tree = desc->dyn_tree;
    max_code = desc->max_code;
    stree = desc->stat_desc->static_tree;
    extra = desc->stat_desc->extra_bits;
    base = desc->stat_desc->extra_base;
    max_length = *(int *)((char *)desc->stat_desc + 0x10);  // 0x10: max_length, not in StaticTreeDesc_guess
    overflow = 0;

    for (bits = 0; bits <= 15; bits++)
        (&s->bl_count[0])[bits] = 0;
    tree[s->heap[s->heap_max]].dad = 0;     // the root's Len (dad and len share the word)

    for (h = s->heap_max + 1; h < 573; h++)
    {
        n = s->heap[h];
        bits = tree[tree[n].dad].dad + 1;   // the parent's Len + 1
        if (bits > max_length)
        {
            bits = max_length;
            overflow++;
        }
        tree[n].dad = (unsigned short)bits;
        if (n > max_code)
            continue;   // not a leaf node

        (&s->bl_count[0])[bits]++;
        xbits = 0;
        if (n >= base)
            xbits = extra[n - base];
        f = tree[n].freq;
        *(unsigned long *)((char *)s + 0x16a0) += (unsigned long)f * (bits + xbits);
        if (stree)
            *(unsigned long *)((char *)s + 0x16a4) += (unsigned long)f * (stree[n].dad + xbits);
    }
    if (overflow == 0)
        return;

    do
    {
        bits = max_length - 1;
        while ((&s->bl_count[0])[bits] == 0)
            bits--;
        (&s->bl_count[0])[bits]--;        // move one leaf down the tree
        (&s->bl_count[0])[bits + 1] += 2; // and one overflow item as its brother
        (&s->bl_count[0])[max_length]--;
        overflow -= 2;
    } while (overflow > 0);

    for (bits = max_length; bits != 0; bits--)
    {
        n = (&s->bl_count[0])[bits];
        while (n != 0)
        {
            m = s->heap[--h];
            if (m > max_code)
                continue;
            if ((unsigned int)tree[m].dad != (unsigned int)bits)
            {
                *(unsigned long *)((char *)s + 0x16a0) += ((unsigned long)bits - tree[m].dad) * tree[m].freq;
                tree[m].dad = (unsigned short)bits;
            }
            n--;
        }
    }
}

// FUNCTION: EQMAIN 0x1000D510
// Assigns bit patterns to symbols after their compression code lengths are set.
// __cdecl. Canonical Huffman code assignment. next_code[1..15] is built from the 15 shorts at bl_count (16-bit:
// next_code[1] = 2 * bl_count[0], next_code[n] = 2 * (next_code[n-1] + bl_count[n-1])). Each of the max_code + 1
// entries {code at +0, len at +2} with len != 0 takes next_code[len]++ as its code, bit-reversed over len bits.
// A negative max_code does nothing. next_code[0] is never set or read.
void __cdecl gen_huffman_codes_guess(unsigned short *tree, int max_code, unsigned short *bl_count)
{
    unsigned short next_code[16];
    unsigned short code;
    unsigned int len;
    int bits;
    unsigned int code_arg;
    int n;

    code = 0;
    for (bits = 1; bits <= 15; bits++) {
        next_code[bits] = code = (code + bl_count[bits - 1]) << 1;
    }
    if (max_code >= 0) {
        n = max_code + 1;
        do {
            len = (unsigned int)(unsigned short)tree[1];
            if (len != 0) {
                code_arg = next_code[len]++;
                len = EQ_CDECL(unsigned int, 0x1000E3A0, (unsigned int, int))(code_arg, len);
                *tree = (short)len;
            }
            tree = tree + 2;
            n = n - 1;
        } while (n != 0);
    }
}

// FUNCTION: EQMAIN 0x1000D590
// Prepares the code-length table used to describe compressed data blocks.
// __cdecl. Bit-length code count of a deflate state (the shape of zlib's build_bl_tree; the name is a guess). The
// literal/length tree (+0x8c, max_code at +0xb14) and the distance tree (+0x980, max_code at +0xb20) are run-length
// scanned into the bit-length tree (+0xa74) by scan_tree_guess (0x1000D600); build_tree (0x1000CFC0) then builds it
// from the descriptor at +0xb28. max_blindex starts at 18 and steps down while above 2, stopping at the first index
// whose table entry (0x10041250) names a bit-length tree length (the short at +0xa76 + 4 * entry) that is nonzero;
// with none it ends at 2. opt_len (+0x16a0) gains max_blindex * 3 + 0x11.
// @early-stop: tail register choice: the original has the field in EDX and the lea in ECX; VC6 has the field in ECX and reuses EAX (max_blindex) for the lea.
void __cdecl build_bl_tree_guess(int *state)
{
    int max_blindex;

    EQ_CDECL(void, 0x1000D600, (int, int, int))((int)state, (int)(state + 0x23), state[0x2c5]);
    EQ_CDECL(void, 0x1000D600, (int, int, int))((int)state, (int)(state + 0x260), state[0x2c8]);
    EQ_CDECL(void, 0x1000CFC0, (int *, int *))(state, state + 0x2ca);
    for (max_blindex = 0x12; max_blindex >= 3; max_blindex = max_blindex - 1) {
        if (*(short *)((int)state + (unsigned int)EQ_ARRAY(unsigned char, 0x10041250)[max_blindex] * 4 + 0xa76) != 0) {
            break;
        }
    }
    state[0x5a8] = state[0x5a8] + max_blindex * 3 + 0x11;
}

// FUNCTION: EQMAIN 0x1000D600
// Counts repeated code lengths to help shorten a block's decoding-table description.
// __cdecl. Run-length scan of a code-length tree: the lengths of entries 0..max_code ({code at +0, len at +2}) are
// grouped into runs, and each run updates the short counters at state + 0xa74 + 4 * len. A run shorter than its
// minimum (3; 4 for a new nonzero length) adds its count to its length; otherwise a nonzero run adds 1 to its length
// (not again when it repeats the previous run's length) and 1 to +0xab4, and a zero run adds 1 to +0xab8 (up to 10)
// or +0xabc. Runs end at 7 (6 once a length repeats, 138 for zeros). Entry max_code + 1's len is set to 0xffff
// first, even when max_code < 0. The 2nd argument's slot is reused as the loop count.
void __cdecl scan_tree_guess(int state, int tree, int max_code)
{
    int curlen;
    int nextlen;
    int max_count;
    unsigned short *lenp;
    int min_count;
    int count;
    int prevlen;

    nextlen = *(unsigned short *)(tree + 2);
    count = 0;
    prevlen = -1;
    max_count = 7;
    min_count = 4;
    if (nextlen == 0) {
        max_count = 0x8a;
        min_count = 3;
    }
    *(unsigned short *)(tree + 6 + max_code * 4) = 0xffff;
    if (max_code >= 0) {
        lenp = (unsigned short *)(tree + 6);
        tree = max_code + 1;
        do {
            curlen = nextlen;
            nextlen = *lenp;
            count = count + 1;
            if (count >= max_count || curlen != nextlen) {
                if (count < min_count) {
                    *(short *)(state + 0xa74 + curlen * 4) += (short)count;
                }
                else if (curlen != 0) {
                    if (curlen != prevlen) {
                        *(short *)(state + 0xa74 + curlen * 4) += 1;
                    }
                    *(short *)(state + 0xab4) += 1;
                }
                else {
                    if (count <= 10) {
                        *(short *)(state + 0xab8) += 1;
                    }
                    else {
                        *(short *)(state + 0xabc) += 1;
                    }
                }
                count = 0;
                prevlen = curlen;
                if (nextlen == 0) {
                    max_count = 0x8a;
                    min_count = 3;
                }
                else if (curlen == nextlen) {
                    max_count = 6;
                    min_count = 3;
                }
                else {
                    max_count = 7;
                    min_count = 4;
                }
            }
            lenp += 2;
            tree = tree - 1;
        } while (tree != 0);
    }
}

// zlib's bl_order: the order the bl_tree code lengths are sent in (19 bytes, read-only).
#define g_bitLengthOrder_guess  EQ_ARRAY(unsigned char, 0x10041250)

// FUNCTION: EQMAIN 0x1000D6F0
// Supplies the decoder with the code tables needed to read a compressed data block.
// __cdecl. zlib's send_all_trees: the header of a dynamic block. lcodes - 257 (5 bits), dcodes - 1 (5 bits) and
// blcodes - 4 (4 bits) go first, then each bl_tree code length in bl_order (3 bits; bl_tree is 4-byte entries from
// +0xa74 with the length at +2), then the literal tree (+0x8c) and the distance tree (+0x980) through send_tree_guess
// (send_tree_guess). Bits go through the 16-bit bit buffer at +0x16b4 (valid bits at +0x16b8); a full word goes out to
// pending_buf (+0x8) at pending (+0x14), low byte first.
void __cdecl send_all_trees_guess(deflate_state *state, int lcodes, int dcodes, int blcodes)
{
    int rank;
    int val;

    if (state->bi_valid > 11) {
        val = lcodes - 257;
        unsigned char **pendingBuffer = &state->pending_buf;
        state->bi_buf = state->bi_buf | (unsigned short)val << state->bi_valid;
        (*pendingBuffer)[state->pending] = state->bi_buf_bytes_guess[0];
        state->pending = state->pending + 1;
        (*pendingBuffer)[state->pending] = (unsigned char)(state->bi_buf >> 8);
        state->pending = state->pending + 1;
        state->bi_buf = (unsigned short)val >> (16 - state->bi_valid);
        state->bi_valid = state->bi_valid - 11;
    }
    else {
        state->bi_buf = state->bi_buf | (unsigned short)(lcodes - 257) << state->bi_valid;
        state->bi_valid = state->bi_valid + 5;
    }
    if (state->bi_valid > 11) {
        val = dcodes - 1;
        state->bi_buf = state->bi_buf | (unsigned short)val << state->bi_valid;
        ((unsigned char *)*(int *)((char *)state + 8))[state->pending] = state->bi_buf_bytes_guess[0];
        state->pending = state->pending + 1;
        ((unsigned char *)*(int *)((char *)state + 8))[state->pending] = (unsigned char)(state->bi_buf >> 8);
        state->pending = state->pending + 1;
        state->bi_buf = (unsigned short)val >> (16 - state->bi_valid);
        state->bi_valid = state->bi_valid - 11;
    }
    else {
        state->bi_buf = state->bi_buf | (unsigned short)(dcodes - 1) << state->bi_valid;
        state->bi_valid = state->bi_valid + 5;
    }
    if (state->bi_valid > 12) {
        val = blcodes - 4;
        state->bi_buf = state->bi_buf | (unsigned short)val << state->bi_valid;
        ((unsigned char *)*(int *)((char *)state + 8))[state->pending] = state->bi_buf_bytes_guess[0];
        state->pending = state->pending + 1;
        ((unsigned char *)*(int *)((char *)state + 8))[state->pending] = (unsigned char)(state->bi_buf >> 8);
        state->pending = state->pending + 1;
        state->bi_buf = (unsigned short)val >> (16 - state->bi_valid);
        state->bi_valid = state->bi_valid - 12;
    }
    else {
        state->bi_buf = state->bi_buf | (unsigned short)(blcodes - 4) << state->bi_valid;
        state->bi_valid = state->bi_valid + 4;
    }
    for (rank = 0; rank < blcodes; rank++) {
        if (state->bi_valid > 13) {
            val = *(unsigned short *)((char *)state + 0xa76 + (unsigned int)g_bitLengthOrder_guess[rank] * 4);
            state->bi_buf = state->bi_buf | (unsigned short)val << state->bi_valid;
            ((unsigned char *)*(int *)((char *)state + 8))[state->pending] = state->bi_buf_bytes_guess[0];
            state->pending = state->pending + 1;
            ((unsigned char *)*(int *)((char *)state + 8))[state->pending] = (unsigned char)(state->bi_buf >> 8);
            state->pending = state->pending + 1;
            state->bi_buf = (unsigned short)val >> (16 - state->bi_valid);
            state->bi_valid = state->bi_valid - 13;
        }
        else {
            state->bi_buf = state->bi_buf |
                 (unsigned short)*(unsigned short *)((char *)state + 0xa76 + (unsigned int)g_bitLengthOrder_guess[rank] * 4) << state->bi_valid;
            state->bi_valid = state->bi_valid + 3;
        }
    }
    EQ_CDECL(void, 0x1000D960, (int, int, int))((int)state, (int)state + 0x8c, lcodes - 1);
    EQ_CDECL(void, 0x1000D960, (int, int, int))((int)state, (int)state + 0x980, dcodes - 1);
}

// FUNCTION: EQMAIN 0x1000D960
// Writes one Huffman tree's code lengths into the compressed output as run-length codes (dynamic block header).
// __cdecl. Sends the code-length tree that scan_tree_guess counted: entries 0..max_code are grouped into the same runs
// (ended at 7, 6 once a length repeats, 138 for zeros), and each run goes out as bl_tree codes through the 16-bit bit
// buffer at +0x16b4 (valid bits at +0x16b8). bl_tree is 4-byte entries from +0xa74: code at +0, bit count at +2. A run
// shorter than its minimum is its length repeated; a nonzero run is its length once (not again when it repeats the
// previous run's length), then code 16 with 2 extra bits; a zero run is code 17 (up to 10 zeros, 3 extra bits) or
// code 18 (11 and more, 7 extra bits). Each full word goes to pending_buf (+0x8) at pending (+0x14), low byte first.
// Entry max_code + 1 is only read: it is the guard scan_tree_guess set.
void __cdecl send_tree_guess(deflate_state *state, int tree, int max_code)
{
    unsigned short first_len;
    unsigned int curlen;
    unsigned int nextlen;
    int max_count;
    unsigned short *lenp;
    int min_count;
    int count;
    unsigned int prevlen;
    int left;
    unsigned int len;
    unsigned short code;
    int bits;

    first_len = *(unsigned short *)(tree + 2);
    count = 0;
    prevlen = 0xffffffff;
    max_count = 7;
    min_count = 4;
    if (first_len == 0) {
        max_count = 0x8a;
        min_count = 3;
    }
    if (-1 < max_code) {
        lenp = (unsigned short *)(tree + 6);
        curlen = (unsigned int)first_len;
        left = max_code + 1;
        do {
            nextlen = (unsigned int)*lenp;
            count = count + 1;
            if (max_count <= count || curlen != nextlen) {
                if (count < min_count) {
                    do {
                        bits = state->bi_valid;
                        len = (unsigned int)*(unsigned short *)((char *)state + 0xa76 + curlen * 4);
                        if ((int)(0x10 - len) < bits) {
                            code = *(unsigned short *)((char *)state + 0xa74 + curlen * 4);
                            state->bi_buf =
                                 state->bi_buf | code << bits;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[0];
                            state->pending = state->pending + 1;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[1];
                            state->pending = state->pending + 1;
                            state->bi_buf =
                                 code >> (0x10 - state->bi_valid);
                            state->bi_valid = state->bi_valid - 0x10 + len;
                        }
                        else {
                            state->bi_buf =
                                 state->bi_buf |
                                 *(short *)((char *)state + 0xa74 + curlen * 4) << bits;
                            state->bi_valid = bits + len;
                        }
                    } while (--count != 0);
                }
                else if (curlen == 0) {
                    if (count < 11) {
                        bits = state->bi_valid;
                        len = (unsigned int)*(unsigned short *)((char *)state + 0xaba);
                        if ((int)(0x10 - len) < bits) {
                            code = *(unsigned short *)((char *)state + 0xab8);
                            state->bi_buf =
                                 state->bi_buf | code << bits;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[0];
                            state->pending = state->pending + 1;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[1];
                            state->pending = state->pending + 1;
                            state->bi_buf =
                                 code >> (0x10 - state->bi_valid);
                            state->bi_valid = state->bi_valid - 0x10 + len;
                        }
                        else {
                            state->bi_buf =
                                 state->bi_buf |
                                 *(short *)((char *)state + 0xab8) << bits;
                            state->bi_valid = bits + len;
                        }
                        bits = state->bi_valid;
                        if (bits < 14) {
                            state->bi_buf =
                                 state->bi_buf | (unsigned short)((count - 3) << bits);
                            state->bi_valid = bits + 3;
                        }
                        else {
                            state->bi_buf =
                                 state->bi_buf | (unsigned short)((count - 3) << bits);
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[0];
                            state->pending = state->pending + 1;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[1];
                            bits = state->bi_valid;
                            state->pending = state->pending + 1;
                            state->bi_valid = bits - 13;
                            state->bi_buf = (unsigned short)(count - 3) >> (0x10 - bits);
                        }
                    }
                    else {
                        bits = state->bi_valid;
                        len = (unsigned int)*(unsigned short *)((char *)state + 0xabe);
                        if ((int)(0x10 - len) < bits) {
                            code = *(unsigned short *)((char *)state + 0xabc);
                            state->bi_buf =
                                 state->bi_buf | code << bits;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[0];
                            state->pending = state->pending + 1;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[1];
                            state->pending = state->pending + 1;
                            state->bi_buf =
                                 code >> (0x10 - state->bi_valid);
                            state->bi_valid = state->bi_valid - 0x10 + len;
                        }
                        else {
                            state->bi_buf =
                                 state->bi_buf |
                                 *(short *)((char *)state + 0xabc) << bits;
                            state->bi_valid = bits + len;
                        }
                        bits = state->bi_valid;
                        if (bits < 10) {
                            state->bi_buf =
                                 state->bi_buf | (unsigned short)((count - 11) << bits);
                            state->bi_valid = bits + 7;
                        }
                        else {
                            state->bi_buf =
                                 state->bi_buf | (unsigned short)((count - 11) << bits);
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[0];
                            state->pending = state->pending + 1;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[1];
                            bits = state->bi_valid;
                            state->pending = state->pending + 1;
                            state->bi_valid = bits - 9;
                            state->bi_buf = (unsigned short)(count - 11) >> (0x10 - bits);
                        }
                    }
                }
                else {
                    if (curlen != prevlen) {
                        bits = state->bi_valid;
                        len = (unsigned int)*(unsigned short *)((char *)state + 0xa76 + curlen * 4);
                        count = count - 1;
                        if ((int)(0x10 - len) < bits) {
                            code = *(unsigned short *)((char *)state + 0xa74 + curlen * 4);
                            state->bi_buf =
                                 state->bi_buf | code << bits;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[0];
                            state->pending = state->pending + 1;
                            *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                                 state->bi_buf_bytes_guess[1];
                            state->pending = state->pending + 1;
                            state->bi_buf =
                                 code >> (0x10 - state->bi_valid);
                            state->bi_valid = state->bi_valid - 0x10 + len;
                        }
                        else {
                            state->bi_buf =
                                 state->bi_buf |
                                 *(short *)((char *)state + 0xa74 + curlen * 4) << bits;
                            state->bi_valid = bits + len;
                        }
                    }
                    bits = state->bi_valid;
                    len = (unsigned int)*(unsigned short *)((char *)state + 0xab6);
                    if ((int)(0x10 - len) < bits) {
                        code = *(unsigned short *)((char *)state + 0xab4);
                        state->bi_buf =
                             state->bi_buf | code << bits;
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                             state->bi_buf_bytes_guess[0];
                        state->pending = state->pending + 1;
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                             state->bi_buf_bytes_guess[1];
                        state->pending = state->pending + 1;
                        state->bi_buf =
                             code >> (0x10 - state->bi_valid);
                        state->bi_valid = state->bi_valid - 0x10 + len;
                    }
                    else {
                        state->bi_buf =
                             state->bi_buf | *(short *)((char *)state + 0xab4) << bits;
                        state->bi_valid = bits + len;
                    }
                    bits = state->bi_valid;
                    if (bits < 15) {
                        state->bi_buf =
                             state->bi_buf | (unsigned short)((count - 3) << bits);
                        state->bi_valid = bits + 2;
                    }
                    else {
                        state->bi_buf =
                             state->bi_buf | (unsigned short)((count - 3) << bits);
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                             state->bi_buf_bytes_guess[0];
                        state->pending = state->pending + 1;
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) =
                             state->bi_buf_bytes_guess[1];
                        bits = state->bi_valid;
                        state->pending = state->pending + 1;
                        state->bi_valid = bits - 14;
                        state->bi_buf = (unsigned short)(count - 3) >> (0x10 - bits);
                    }
                }
                count = 0;
                prevlen = curlen;
                if (nextlen == 0) {
                    max_count = 0x8a;
                    min_count = 3;
                }
                else if (curlen == nextlen) {
                    max_count = 6;
                    min_count = 3;
                }
                else {
                    max_count = 7;
                    min_count = 4;
                }
            }
            lenp = lenp + 2;
            left = left - 1;
            curlen = nextlen;
        } while (left != 0);
    }
}

#define g_length_code_guess  EQ_ARRAY(unsigned char, 0x1004195C)
#define g_extraLbits_guess  EQ_ARRAY(int, 0x10041118)
#define g_baseLength_guess  EQ_ARRAY(int, 0x10041A5C)
#define g_dist_code_guess  EQ_ARRAY(unsigned char, 0x1004175C)
#define g_distCodeHigh_guess  EQ_ARRAY(unsigned char, 0x1004185C)
#define g_extraDbits_guess  EQ_ARRAY(int, 0x1004118C)
#define g_baseDist_guess  EQ_ARRAY(int, 0x10041AD0)

// FUNCTION: EQMAIN 0x1000DEE0
// Encodes a data block's contents in compact form.
// __cdecl. zlib's compress_block (a guess from the deflate_state offsets). For each symbol lx < last_lit: a literal when
// d_buf[lx] is 0 (the byte is l_buf[lx], sent with ltree), else a match: its length code (ltree entry 257 + code, code =
// _length_code[l_buf[lx]]) and its distance code (dtree), each followed by its extra bits. The bits go through the 16-bit
// buffer at +0x16b4 (valid bits at +0x16b8); each full word goes to pending_buf (+0x8) at pending (+0x14), low byte first.
// Then END_BLOCK (ltree entry 256) is sent and its bit count stored at +0x16b0. ltree and dtree are 4-byte entries: code
// at +0, bit count at +2. Returns nothing.
void __cdecl compress_block_guess(deflate_state *state, int ltree, int dtree)
{
    unsigned int lx;        // the next symbol: its index in d_buf and l_buf (local_4)
    unsigned int dist;      // the match distance, 0 for a literal; then the distance less 1
    unsigned int lc;        // the literal byte, or the match length less 3; then the length less its base
    unsigned int code;      // the length code, then the distance code
    unsigned int len;       // the bit count of the code being sent
    int bits;               // the valid bits in the bit buffer
    unsigned short val;     // the code being sent
    int extra;              // the extra bit count of the code

    lx = 0;
    if (*(int *)((char *)state + 0x1698) != 0) {
        do {
            dist = *(unsigned short *)(*(int *)((char *)state + 0x169c) + lx * 2);
            lc = *(unsigned char *)(*(int *)((char *)state + 0x1690) + lx);
            lx = lx + 1;
            if (dist == 0) {
                len = *(unsigned short *)(ltree + lc * 4 + 2);
                bits = state->bi_valid;
                if ((int)(0x10 - len) < bits) {
                    val = *(unsigned short *)(ltree + lc * 4);
                    state->bi_buf = state->bi_buf | val << bits;
                    *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[0];
                    state->pending = state->pending + 1;
                    *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[1];
                    state->pending = state->pending + 1;
                    state->bi_buf = val >> (0x10 - bits);
                    bits = bits - 0x10 + len;
                    goto storeBitCount;
                }
                else {
                    state->bi_buf = state->bi_buf | *(unsigned short *)(ltree + lc * 4) << bits;
                    bits = bits + len;
                    goto storeBitCount;
                }
            }
            else {
                code = g_length_code_guess[lc];
                len = *(unsigned short *)(ltree + code * 4 + 0x406);
                bits = state->bi_valid;
                if ((int)(0x10 - len) < bits) {
                    val = *(unsigned short *)(ltree + code * 4 + 0x404);
                    state->bi_buf = state->bi_buf | val << bits;
                    *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[0];
                    state->pending = state->pending + 1;
                    *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[1];
                    state->pending = state->pending + 1;
                    state->bi_buf = val >> (0x10 - bits);
                    state->bi_valid = bits - 0x10 + len;
                }
                else {
                    state->bi_buf = state->bi_buf | *(unsigned short *)(ltree + code * 4 + 0x404) << bits;
                    state->bi_valid = bits + len;
                }
                extra = g_extraLbits_guess[code];
                if (extra != 0) {
                    lc = lc - g_baseLength_guess[code];
                    bits = state->bi_valid;
                    if ((0x10 - extra) < bits) {
                        state->bi_buf = state->bi_buf | lc << bits;
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[0];
                        state->pending = state->pending + 1;
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[1];
                        state->pending = state->pending + 1;
                        state->bi_buf = (unsigned short)lc >> (0x10 - bits);
                        state->bi_valid = bits - 0x10 + extra;
                    }
                    else {
                        state->bi_buf = state->bi_buf | lc << bits;
                        state->bi_valid = bits + extra;
                    }
                }
                dist = dist - 1;
                if (dist < 0x100)
                    code = g_dist_code_guess[dist];
                else
                    code = g_distCodeHigh_guess[dist >> 7];
                bits = state->bi_valid;
                len = *(unsigned short *)(dtree + code * 4 + 2);
                if ((int)(0x10 - len) < bits) {
                    val = *(unsigned short *)(dtree + code * 4);
                    state->bi_buf = state->bi_buf | val << bits;
                    *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[0];
                    state->pending = state->pending + 1;
                    *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[1];
                    state->pending = state->pending + 1;
                    state->bi_buf = val >> (0x10 - bits);
                    state->bi_valid = bits - 0x10 + len;
                }
                else {
                    state->bi_buf = state->bi_buf | *(unsigned short *)(dtree + code * 4) << bits;
                    state->bi_valid = bits + len;
                }
                extra = g_extraDbits_guess[code];
                if (extra != 0) {
                    dist = dist - g_baseDist_guess[code];
                    bits = state->bi_valid;
                    if ((0x10 - extra) < bits) {
                        state->bi_buf = state->bi_buf | dist << bits;
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[0];
                        state->pending = state->pending + 1;
                        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[1];
                        state->pending = state->pending + 1;
                        state->bi_valid = bits + extra - 0x10;
                        state->bi_buf = (unsigned short)dist >> (0x10 - bits);
                    }
                    else {
                        state->bi_buf = state->bi_buf | dist << bits;
                        bits = bits + extra;
storeBitCount:
                        state->bi_valid = bits;
                    }
                }
            }
        } while (lx < state->last_lit);
    }
    len = *(unsigned short *)(ltree + 0x402);
    bits = state->bi_valid;
    if ((int)(0x10 - len) < bits) {
        val = *(unsigned short *)(ltree + 0x400);
        state->bi_buf = state->bi_buf | val << bits;
        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[0];
        state->pending = state->pending + 1;
        *(unsigned char *)(*(int *)((char *)state + 8) + state->pending) = state->bi_buf_bytes_guess[1];
        state->pending = state->pending + 1;
        state->bi_buf = val >> (0x10 - bits);
        state->bi_valid = bits - 0x10 + len;
    }
    else {
        state->bi_buf = state->bi_buf | *(unsigned short *)(ltree + 0x400) << bits;
        state->bi_valid = bits + len;
    }
    *(unsigned int *)((char *)state + 0x16b0) = *(unsigned short *)(ltree + 0x402);
}

// FUNCTION: EQMAIN 0x1000E320
// Marks data as text-like when printable byte frequencies dominate.
// @early-stop: the first two loops use different registers, and the final range check is folded.
// __cdecl. Sums the 16-bit field at the start of each 4-byte entry: entries 0..6 (from +0x8c) and 128..255 (from
// +0x28c) go into one sum, entries 7..127 (from +0xa8) into another. Sets the bool at +0x1c to whether the first sum
// is at most a quarter of the second.
void __cdecl table_sums_flag_guess(int state)
{
    unsigned short freq;
    unsigned short *freqPtr;
    int n;
    unsigned int binFreq;
    unsigned int asciiFreq;
    int i;

    asciiFreq = 0;
    binFreq = 0;
    freqPtr = (unsigned short *)(state + 0x8c);
    for (n = 7; n != 0; n--) {
        freq = *freqPtr;
        freqPtr = freqPtr + 2;
        binFreq = binFreq + freq;
    }
    i = 0x80;
    freqPtr = (unsigned short *)(state + 0xa8);
    for (n = 0x79; n != 0; n--) {
        freq = *freqPtr;
        freqPtr = freqPtr + 2;
        asciiFreq = asciiFreq + freq;
    }
    while (i < 0x100) {
        freq = *(unsigned short *)(state + i * 4 + 0x8c);
        binFreq = binFreq + freq;
        i++;
    }
    *(bool *)(state + 0x1c) = binFreq <= asciiFreq >> 2;
}

// FUNCTION: EQMAIN 0x1000E3A0
// Reverses selected bits of a value for use in encoded data.
// __cdecl. Reverses the low `count` bits of `value` (bit 0 moves to bit count-1); the loop runs once even for a
// count of 0 or less.
unsigned int __cdecl reverse_low_bits_guess(unsigned int value, int count)
{
    unsigned int acc;

    acc = 0;
    do {
        acc = acc | (value & 1);
        value = value >> 1;
        acc = acc << 1;
        count = count - 1;
    } while (0 < count);
    return acc >> 1;
}

// The bit writer: pending bits sit in the 16-bit accumulator at 0x16b4 (its low byte first), their count at 0x16b8, and
// the bytes go out through buf at pos. Only these fields are read here.
struct BitWriter_guess
{
    char unknown_00[0x08];          // 0x00 .. 0x07: not read here
    unsigned char *buf;             // 0x08
    char unknown_0c[0x08];          // 0x0c .. 0x13: not read here
    int pos;                        // 0x14
    char unknown_18[0x169c];        // 0x18 .. 0x16b3: not read here
    union
    {
        unsigned short acc;         // 0x16b4: the pending bits
        unsigned char acc_b[2];     // 0x16b4: the low byte, 0x16b5: the high byte
    };
    char unknown_16b6[0x02];        // 0x16b6 .. 0x16b7: not read here
    int nbits;                      // 0x16b8: how many of acc's bits are pending
};

// FUNCTION: EQMAIN 0x1000E3C0
// Finishes pending compressed output before the data stream continues.
// __cdecl. Flushes the bit writer's pending bits: with more than 8 pending, both bytes of acc go out; with 1..8, its
// low byte; with none, nothing. Either way acc and nbits end cleared.
void __cdecl bitwriter_flush_guess(BitWriter_guess *w)
{
    if (8 < w->nbits) {
        w->buf[w->pos++] = (unsigned char)w->acc;
        w->buf[w->pos++] = (unsigned char)(w->acc >> 8);
    } else if (0 < w->nbits) {
        w->buf[w->pos++] = (unsigned char)w->acc;
    }
    w->acc = 0;
    w->nbits = 0;
}

// FUNCTION: EQMAIN 0x1000E440
// Keeps a data block intact when it is sent without compression.
// __cdecl. Flushes the bit writer's pending bits, then sets the int at 0x16b0 (not named in BitWriter_guess) to 8. With
// `header` non-zero, first puts len's low byte, its high byte, then their complements into buf at pos (LEN, NLEN).
// Then copies `len` bytes of `data` into buf at pos.
void __cdecl bitwriter_put_stored_guess(BitWriter_guess *w, unsigned char *data, int len, int header)
{
    EQ_CDECL(void, 0x1000E3C0, (BitWriter_guess *))(w);
    *(int *)((char *)w + 0x16b0) = 8;
    if (header != 0) {
        w->buf[w->pos++] = (unsigned char)len;
        w->buf[w->pos++] = (unsigned char)((unsigned short)len >> 8);
        w->buf[w->pos++] = (unsigned char)(unsigned short)~len;
        w->buf[w->pos++] = (unsigned char)((unsigned short)~len >> 8);
    }
    while (len--) {
        w->buf[w->pos++] = *data++;
    }
}

// The z_stream inflate_once_guess sets up on its stack (zlib's z_stream, 0x38 bytes). Only the fields it writes or reads
// are named; the rest is not touched here.
/* z_stream: the actor layout is z_stream (generated: eqmain/layout/z_stream.h); the old local view's members map by offset. */
#include "eqmain/layout/z_stream.h"

// FUNCTION: EQMAIN 0x1000E4E0
// Unpacks a whole compressed buffer into a caller buffer in one call; its one caller reads pack files.
// __cdecl. zlib 1.1.2's one-shot inflate: sets up a z_stream on the stack over the input (source, sourceLen bytes) and the
// output (dest, *destLen bytes), calls inflateInit_ with "1.1.2" and 0x38, then inflate(Z_FINISH) and inflateEnd. On
// Z_STREAM_END (1) it stores the bytes produced in *destLen and returns inflateEnd's result. Otherwise it returns the
// inflate result after inflateEnd, or -5 (Z_BUF_ERROR) when that result is 0. A non-zero inflateInit_ result is returned as is.
unsigned int __cdecl inflate_once_guess(unsigned char *dest, unsigned int *destLen, unsigned char *source,
                                        unsigned int sourceLen)
{
    unsigned int uVar1;
    unsigned int inflateErr;
    z_stream strm;

    strm.next_in = source;
    strm.avail_in = sourceLen;
    strm.next_out = dest;
    strm.avail_out = *destLen;
    strm.zalloc = 0;
    strm.zfree = 0;
    uVar1 = EQ_CDECL(unsigned int, 0x1000BF90, (z_stream *, char *, int))(&strm, &g_zlibVersionChar_guess, 0x38);
    if (uVar1 == 0) {
        inflateErr = EQ_CDECL(unsigned int, 0x1000BFB0, (z_stream *, int))(&strm, 4);
        if (inflateErr != 1) {
            EQ_CDECL(unsigned int, 0x1000BE30, (z_stream *))(&strm);
            uVar1 = 0xfffffffb;
            if (inflateErr != 0) {
                return inflateErr;
            }
        } else {
            *destLen = strm.total_out;
            uVar1 = EQ_CDECL(unsigned int, 0x1000BE30, (z_stream *))(&strm);
        }
    }
    return uVar1;
}

// FUNCTION: EQMAIN 0x1000E580
// __cdecl. An allocation callback: calloc(count, size); the first argument (an opaque context) is unused.
void* __cdecl CallocCallback_guess(void* context, size_t count, size_t size)
{
    return calloc(count, size);
}
