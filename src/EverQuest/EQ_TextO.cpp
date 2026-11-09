#include "trilogy/eqaddr.h"
#include "trilogy/layout/eq_book.h"

typedef int (__stdcall *SetRect_guess_t)(int *, int, int, int, int);
typedef void (__cdecl *DeferTextA_guess_t)(char *, unsigned int, float *, unsigned int, int);

class EQ_TextO_guess
{
public:
    void WriteTextHD_guess(char *, int, int, short, int, int);
};

// FUNCTION: TRILOGY 0x00418e97
// Renders two adjacent indexed text pages side by side.
void __fastcall EQ_Book_DrawBookText_guess(int book)
{
    short textLineIndex;
    int lineIndex;
    short textEndIndex;
    short nextPageEndIndex;
    int remainingLineCount;
    char pageText[128];
    int textY;
    int temp;
    int line;

    EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
        (pageText, EQ_ARRAY(char, 0x0053E16C),
         (int)((EQ_Book_guess*)book)->pageIndex_guess + 1);
    EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(pageText, 0xe1, 0xf0, 0,
         EQ_GLOBAL(int, 0x006EFDF8), 1);
    EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
        (pageText, EQ_ARRAY(char, 0x0053E16C),
         (int)((EQ_Book_guess*)book)->pageIndex_guess + 2);
    EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(pageText, 400, 0xf0, 0,
         EQ_GLOBAL(int, 0x006EFDF8), 1);
    textLineIndex = ((EQ_Book_guess*)book)->pageIndex_guess * 0xc;
    textEndIndex = textLineIndex + 0xc;
    if ((short)(textLineIndex + 0xc) > ((EQ_Book_guess*)book)->textLineCount) {
        textEndIndex = ((EQ_Book_guess*)book)->textLineCount;
    }
    if (textLineIndex < textEndIndex) {
        lineIndex = (int)textLineIndex;
        textY = lineIndex * 0xf;
        temp = lineIndex * 0x3c + 0x89c + book;
        remainingLineCount = textEndIndex - lineIndex;
        do {
            EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(
                 (char *)temp, 0xa0, textY + lineIndex * -0xf + 0x28, 0,
                 EQ_GLOBAL(int, 0x006EFDF8), 1);
            textY = textY + 0xf;
            temp = temp + 0x3c;
            remainingLineCount = remainingLineCount + -1;
        } while (remainingLineCount != 0);
    }
    textEndIndex = ((EQ_Book_guess*)book)->textLineCount;
    textLineIndex = (((EQ_Book_guess*)book)->pageIndex_guess + 1) * 0xc;
    if (textLineIndex <= textEndIndex) {
        nextPageEndIndex = textLineIndex + 0xc;
        if ((short)(textLineIndex + 0xc) > textEndIndex) {
            nextPageEndIndex = textEndIndex;
        }
        if (textLineIndex < nextPageEndIndex) {
            lineIndex = (int)textLineIndex;
            temp = lineIndex * 0xf;
            line = lineIndex * 0x3c + 0x89c + book;
            remainingLineCount = nextPageEndIndex - lineIndex;
            do {
                EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(
                     (char *)line, 0x154,
                     temp + lineIndex * -0xf + 0x28, 0,
                     EQ_GLOBAL(int, 0x006EFDF8), 1);
                temp = temp + 0xf;
                line = line + 0x3c;
                remainingLineCount = remainingLineCount + -1;
            } while (remainingLineCount != 0);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0046a491
// Displays short text fields above a wrapped body of text.
void __fastcall EQ_Main_DrawPlayerInfoText_guess(void *self)
{
    size_t textLen;
    int scanPos;
    short splitLen;
    int acc;
    int breakPos;
    int x;
    int y;
    short guildIndex;
    char textBuf[2048];
    char lineBuf[16];
    char lineBufTail;
    char *_Str;
    short lineCount;
    short charIndex;

    lineCount = 0;
    x = 0x217;
    y = 0xf0;
    if (EQ_GLOBAL(int, 0x0058FC6C) == 2) {
        x = *(int *)((char *)self + 0x90184);
        x = x + 0xa0;
        *(int *)((char *)self + 0x90194) = 0;
        y = *(int *)((char *)self + 0x90188);
        *(int *)((char *)self + 0x90194) = 0;
        y = y + 0xb9;
    }
    if (*(char *)((char *)self + 0x1410) != '\0') {
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))
            (lineBuf, (char *)self + 0x1410);
        textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(lineBuf);
        if (0x10 < textLen) {
            lineBuf[16] = '\0';
        }
        EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(lineBuf, x, y, 10,
             EQ_GLOBAL(int, 0x006EFDF8), 1);
    }
    if (*(int *)((char *)self + 0x84) != 0) {
        guildIndex = *(short *)(*(int *)((char *)self + 0x84) + 0x90);
        if (guildIndex >= 0) {
            EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                (lineBuf,
                 (char *)EQ_ARRAY(char, 0x006E5494) + guildIndex * 0x3c);
            textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(lineBuf);
            if (0x10 < textLen) {
                lineBuf[16] = '\0';
            }
            EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(lineBuf, x,
                 y + 0xf, 10, EQ_GLOBAL(int, 0x006EFDF8), 1);
        }
    }
    if (*(int *)((char *)self + 0x1020) ==
        *(int *)((char *)EQ_GLOBAL(void *, 0x006E4D8C) + 0x80)) {
        _Str = (char *)EQ_GLOBAL(int, 0x006E4E44) + 0xeda;
    }
    else {
        _Str = (char *)self + 0x1310;
    }
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(textBuf, _Str);
    textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(textBuf);
    acc = 0;
    charIndex = 0;
    if (0 < (short)textLen) {
        do {
            if ((short)textLen < 1) {
                break;
            }
            if (0x55 < acc) {
                scanPos = (int)charIndex;
                breakPos = 0;
                acc = scanPos + -10;
                if (acc < scanPos) {
                    do {
                        if (((short)breakPos == 0) &&
                            (textBuf[scanPos] == ' ')) {
                            breakPos = scanPos + 1;
                        }
                        scanPos = scanPos + -1;
                    } while (acc < scanPos);
                    splitLen = (short)breakPos;
                    if ((short)breakPos == 0) {
                        splitLen = charIndex;
                    }
                }
                else {
                    splitLen = charIndex;
                }
                EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                    (lineBuf, EQ_ARRAY(char, 0x0058FBA4));
                EQ_CDECL(char *, 0x005231D0,
                         (char *, const char *, size_t))
                    (lineBuf, textBuf, (int)splitLen);
                lineBuf[splitLen] = '\0';
                EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                    (textBuf, textBuf + splitLen);
                acc = 0;
                charIndex = 0;
                textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))
                    (textBuf);
                EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(lineBuf, x,
                     y + ((int)lineCount + 2) * 0xf, 10,
                     EQ_GLOBAL(int, 0x006EFDF8), 1);
                lineCount = lineCount + 1;
            }
            if ('\0' < textBuf[charIndex]) {
                acc = acc +
                    *(int *)((char *)EQ_GLOBAL(void *, 0x006EFDDC) +
                             textBuf[charIndex] * 4);
            }
            charIndex = charIndex + 1;
        } while (charIndex < (short)textLen);
    }
    _Str = textBuf;
    textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(_Str);
    if (textLen != 0) {
        EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))(textBuf, x,
             y + ((int)lineCount + 2) * 0xf, 10,
             EQ_GLOBAL(int, 0x006EFDF8), 1);
        lineCount = lineCount + 1;
    }
    *(short *)((char *)self + 0xed8) = lineCount;
    return;
}

// FUNCTION: TRILOGY 0x0048d21a
// Draws a UI text string at screen x,y with a colour index and font, through a text overlay or the renderer
// ORIGINAL FILE: EverQuest/EQ_TextO.cpp (from Mac debug map)
// @early-stop: overlay width and clipping register choices differ; x87 adds load operands in the opposite order
/* fake match: the volatile coordinate alias makes VC6 spill and reload the floats like the original */
void EQ_TextO_guess::WriteTextHD_guess(char *text, int x, int y,
                                       short color, int font_id, int draw_argument)
{
    void *text_object = this;
    unsigned int color_value;
    unsigned int mapped_font;
    unsigned int user_color;
    float coordinates[3];
    volatile float *point = coordinates;

    if (EQ_GLOBAL(int, 0x006E5480) == 0) {
        int width;
        EQ_METHOD(2, void, 0x0040F71E, (void *, void *, int), EQ_GLOBAL(void *, 0x006E4DE0))(*(void **)((char *)text_object + 4), 0xff00ff);
        width = EQ_CDECL(short, 0x0040BE28, (void *, char *, int, int, int, int))
            (*(void **)((char *)text_object + 4), text, 0, 0, (int)color, font_id);
        if (width % 8 != 0)
            width = width + 8 & 0xfffffff8;
        EQ_GLOBAL(SetRect_guess_t, 0x00534248)
            ((int *)((char *)text_object + 0x24), x, y,
             width + x, y + 0x14);
        EQ_GLOBAL(SetRect_guess_t, 0x00534248)
            ((int *)((char *)text_object + 0x14), 0, 0, width, 0x14);
        int left = *(int *)((char *)text_object + 0x24);
        if (0x280 < *(int *)((char *)text_object + 0x2c) - left) {
            *(int *)((char *)text_object + 0x2c) = left + 0x280;
            *(int *)((char *)text_object + 0x1c) = *(int *)((char *)text_object + 0x14) + 0x280;
        }
        if (EQ_GLOBAL(int, 0x0058FC6C) != 2) {
            int right = *(int *)((char *)text_object + 0x2c);
            if (0x280 < right) {
                width = right - 0x280;
                if (width % 8 != 0)
                    width = width + 8 & 0xfffffff8;
                right -= width;
                *(int *)((char *)text_object + 0x1c) -= width;
                *(unsigned int *)((char *)text_object + 0x2c) = right;
            }
        }
        else {
            int right = *(int *)((char *)text_object + 0x2c);
            if (right > EQ_GLOBAL(int, 0x006B63FC)) {
                width = right - EQ_GLOBAL(int, 0x006B63FC);
                if (width % 8 != 0)
                    width = width + 8 & 0xfffffff8;
                right -= width;
                *(int *)((char *)text_object + 0x1c) -= width;
                *(unsigned int *)((char *)text_object + 0x2c) = right;
            }
        }
        *(unsigned int *)((char *)text_object + 0x10) = 1;
        EQ_METHOD(1, void, 0x00450081, (void *, int), text_object)(draw_argument);
        *(unsigned int *)((char *)text_object + 0x10) = 0;
        return;
    }

    mapped_font = 0;
    if (font_id == EQ_GLOBAL(int, 0x006EFDF4))
        mapped_font = EQ_GLOBAL(unsigned int, 0x00591784);
    else if (font_id == EQ_GLOBAL(int, 0x006EFDF8))
        mapped_font = EQ_GLOBAL(unsigned int, 0x00591788);
    else if (font_id == EQ_GLOBAL(int, 0x006EFDFC))
        mapped_font = EQ_GLOBAL(unsigned int, 0x0059178C);
    else if (font_id == EQ_GLOBAL(int, 0x006EFE00))
        mapped_font = EQ_GLOBAL(unsigned int, 0x00591790);
    else if (font_id == EQ_GLOBAL(int, 0x006EFE04))
        mapped_font = EQ_GLOBAL(unsigned int, 0x00591794);
    else if (font_id == EQ_GLOBAL(int, 0x006EFE08))
        mapped_font = EQ_GLOBAL(unsigned int, 0x00591798);
    else if (font_id == EQ_GLOBAL(int, 0x006EFE0C))
        mapped_font = EQ_GLOBAL(unsigned int, 0x0059179C);

    coordinates[0] = (float)x;
    coordinates[1] = (float)y;
    coordinates[2] = 0;
    if (EQ_GLOBAL(int, 0x0058FC6C) == 1
        || EQ_GLOBAL(int, 0x0058FC6C) == 5
        || EQ_GLOBAL(int, 0x0058FC6C) == 6) {
        if (EQ_GLOBAL(int, 0x006B6504) == 1) {
            point[0] = EQ_GLOBAL(float, 0x00534618) + point[0];
            point[1] = EQ_GLOBAL(float, 0x0053518C) + point[1];
        }
        if (EQ_GLOBAL(int, 0x006B6504) == 2) {
            point[0] = EQ_GLOBAL(float, 0x00535188) + point[0];
            point[1] = EQ_GLOBAL(float, 0x00535184) + point[1];
        }
        if (EQ_GLOBAL(int, 0x006B6504) == 3) {
            point[0] = EQ_GLOBAL(float, 0x00535180) + point[0];
            point[1] = EQ_GLOBAL(float, 0x0053517C) + point[1];
        }
        if (EQ_GLOBAL(int, 0x006B6504) == 4) {
            point[0] = EQ_GLOBAL(float, 0x00535178) + point[0];
            point[1] = EQ_GLOBAL(float, 0x00535174) + point[1];
        }
    }

    int color_index = (int)color;
    if (0xff < color) {
        color_index -= 0x100;
        user_color = EQ_CDECL(unsigned int, 0x0040BB3F, (int))(color_index);
        if (EQ_GLOBAL(int, 0x0058FC6C) == 2 && user_color == 0)
            user_color = 0xf0f0f0;
        color_value = user_color & 0xff00 | user_color >> 0x10 & 0xff
            | (user_color | 0xffffff00) << 0x10;
    }
    else {
        switch (color_index) {
        case 10:
            color_value = 0xfff0f0f0;
            break;
        case 0:
            color_value = 0xff000000;
            break;
        case 2:
            color_value = 0xff008000;
            break;
        case 4:
            color_value = 0xff000080;
            break;
        case 5:
            color_value = 0xfff000f0;
            break;
        case 6:
            color_value = 0xff808080;
            break;
        case 7:
            color_value = 0xffe0e0e0;
            break;
        case 0xc:
            color_value = 0xffa0a0a0;
            break;
        case 0xd:
            color_value = 0xfff00000;
            break;
        case 0xe:
            color_value = 0xff00f000;
            break;
        case 0xf:
            color_value = 0xfff0f000;
            break;
        case 0x10:
            color_value = 0xff0000f0;
            break;
        case 0x12:
            color_value = 0xff00f0f0;
            break;
        default:
            color_value = 0xff606060;
            break;
        }
    }

    EQ_GLOBAL(DeferTextA_guess_t, 0x006E51E0)
        (text, mapped_font, &coordinates[0], color_value, 0);
    return;
}
