// StmlWnd.cpp - the STML window of the Suite UI library: petition queue, petition and GM guide views.

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// EQ_TextO::WriteTextHD_guess (0x0048D21A): __thiscall on the EQ_TextO global, args (text, x, y, colour, font, 1).
#define STML_WRITE_TEXT(text, x, y, colour, font) \
    EQ_METHOD(6, void, 0x0048D21A, (void *, void *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))((text), (x), (y), (colour), (font), 1)

// The font argument: DG_FONT for most rows, DG_FONT2 for the petition text rows of mode 4.
#define STML_FONT_DFC EQ_GLOBAL(int, 0x006EFDFC)
#define STML_FONT_DF8 EQ_GLOBAL(int, 0x006EFDF8)

#define STML_DRAW_BORDERS(object) EQ_METHOD(0, void, 0x00437ECA, (void *), (object))   /* EQ_Guide::drawBorders_guess */
#define SOC_FMT      EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
#define SOC_CPY       EQ_CDECL(char *, 0x005219C0, (char *, const char *))
#define STML_STRLEN       EQ_CDECL(size_t, 0x005223D0, (const char *))
#define STML_CTIME        EQ_CDECL(char *, 0x005213D0, (const s32 *))
#define STML_GET_TIME     EQ_CDECL(u32, 0x004A461E, (void))
#define STML_GET_CLASS(object) EQ_METHOD(1, char *, 0x004C9D4E, (void *, short), (object))   /* CEverQuest::GetClassDesc_guess */
#define STML_GET_RACE(object) EQ_METHOD(1, char *, 0x004C9105, (void *, short), (object))   /* CEverQuest::GetRaceName_guess */

// Local stand-in for the text rows of the frame. They sit back to back in this order (0x0 .. 0x57F), and a
// row that overflows spills into the next row up: texts of 64 bytes or more are copied into 64-byte rows
// and the rest lands in the following one. That spill is part of the window's behaviour.
struct StmlRows_guess
{
    char local_594[512];    // 0x000 (EBP-0x590): urgency word
    char local_394[128];    // 0x200 (EBP-0x390)
    char local_314[64];     // 0x280 (EBP-0x310)
    char local_2d4[64];     // 0x2C0 (EBP-0x2D0)
    char local_294[64];     // 0x300 (EBP-0x290)
    char local_254[64];     // 0x340 (EBP-0x250)
    char local_214[64];     // 0x380 (EBP-0x210): sprintf buffer, first piece of a long text
    char local_1d4[448];    // 0x3C0 (EBP-0x1D0): spill of local_214, second piece of a long text
};

// Owner of the member below; layout not recovered.
class CStmlWnd_guess
{
public:
    void Draw_guess(void);    /* 0x00436946 */
};

// FUNCTION: TRILOGY 0x00436946
// Draws the GM petition window's current view: petition queue, petition details, timed prompt or guide entry
// ORIGINAL FILE: Suite/Suite/StmlWnd.cpp (from Mac debug map)
// __thiscall (this in ECX, no stack arguments). Draws the STML window for its mode at this+0x8C:
// 0 petition, 1 queue list, 2 petition detail, 3 name approval prompt, 4 GM guide detail.
void CStmlWnd_guess::Draw_guess(void)
{
    void * self = (void *)this;
    int colour;             // local_8 (EBP-0x4)
    int *pos;               // local_c (EBP-0x8): x/y pair pointer (mode 1), ctime text (modes 2 and 4)
    s32 entry;              // local_10 (EBP-0xc): queue index (mode 1), ctime value (modes 2 and 4)
    int entryOffset;        // local_14 (EBP-0x10)
    StmlRows_guess rows;
    void *everquest;
    int kind;
    int now;
    size_t len;

    if (*(int *)((char *)self + 0x10) == 0)
    {
        return;
    }
    if (*(int *)((char *)self + 0x8C) == 0)
    {
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C7C), *(int *)((char *)self + 0x2028) + 10,
            *(int *)((char *)self + 0x202C) + 10, 10, STML_FONT_DFC);
        STML_DRAW_BORDERS(self)();
    }
    if (*(int *)((char *)self + 0x8C) == 1)
    {
        entry = 0;
        if (0 < *(int *)((char *)EQ_GLOBAL(void *, 0x006EFE2C) + 0xBA4))
        {
            entryOffset = 0;
            pos = (int *)((char *)self + 0x2028);
            everquest = EQ_GLOBAL(void *, 0x006EFE2C);
            do
            {
                if (-1 < *(int *)((char *)everquest + entryOffset + 0x4C))
                {
                    kind = *(int *)((char *)everquest + entryOffset + 0x50);
                    colour = 0xE;
                    if (kind == 1)
                    {
                        colour = 0xF;
                    }
                    if (1 < kind)
                    {
                        colour = 0xD;
                    }
                    STML_WRITE_TEXT((char *)everquest + entryOffset + 0x5C, pos[0] + 10, pos[1] + 5,
                        colour, STML_FONT_DFC);
                    STML_WRITE_TEXT((char *)EQ_GLOBAL(void *, 0x006EFE2C) + entryOffset + 0xA0, pos[0] + 10,
                        pos[1] + 15, colour, STML_FONT_DFC);
                    everquest = EQ_GLOBAL(void *, 0x006EFE2C);
                }
                entry = entry + 1;
                entryOffset = entryOffset + 0x74;
                pos = pos + 4;
            } while (entry < *(int *)((char *)everquest + 0xBA4));
        }
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C74), *(int *)((char *)self + 0x2658) + 10,
            *(int *)((char *)self + 0x265C) + 10, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558C5C),
            *(int *)((char *)EQ_GLOBAL(void *, 0x006EFE2C) + 0xBA0));
        STML_WRITE_TEXT(rows.local_214, 0x8B, 0x14, 0x12, STML_FONT_DFC);
        STML_DRAW_BORDERS(self)();
    }
    if (*(int *)((char *)self + 0x8C) == 2)
    {
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C50), *(int *)((char *)self + 0x2028) + 10,
            *(int *)((char *)self + 0x202C) + 10, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C48), *(int *)((char *)self + 0x2038) + 10,
            *(int *)((char *)self + 0x203C) + 10, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C3C), *(int *)((char *)self + 0x2048) + 10,
            *(int *)((char *)self + 0x204C) + 10, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C30), *(int *)((char *)self + 0x2058) + 10,
            *(int *)((char *)self + 0x205C) + 10, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C24), *(int *)((char *)self + 0x2068) + 10,
            *(int *)((char *)self + 0x206C) + 10, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C18), *(int *)((char *)self + 0x2078) + 10,
            *(int *)((char *)self + 0x207C) + 10, (*(int *)((char *)self + 0xA0) == 0) ? 10 : 15, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C10), *(int *)((char *)self + 0x2088) + 10,
            *(int *)((char *)self + 0x208C) + 10, (*(int *)((char *)self + 0xA4) == 0) ? 10 : 15, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558C00), *(int *)((char *)self + 0x2098) + 10,
            *(int *)((char *)self + 0x209C) + 10, (*(int *)((char *)self + 0xA8) == 0) ? 10 : 15, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558BF0), *(int *)((char *)self + 0x20A8) + 10,
            *(int *)((char *)self + 0x20AC) + 10, (*(int *)((char *)self + 0xAC) == 0) ? 10 : 15, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558BE4), *(int *)((char *)self + 0x20B8) + 10,
            *(int *)((char *)self + 0x20BC) + 10, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558BD4), *(int *)((char *)self + 0x20C8) + 10,
            *(int *)((char *)self + 0x20CC) + 10, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558BC4), *(int *)((char *)self + 0x20D8) + 10,
            *(int *)((char *)self + 0x20DC) + 10, 10, STML_FONT_DFC);
        STML_DRAW_BORDERS(self)();
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558BB4), *(int *)((char *)self + 0x118));
        STML_WRITE_TEXT(rows.local_214, 0xFA, 10, 10, STML_FONT_DFC);
        SOC_CPY(rows.local_594, EQ_ARRAY(char, 0x00558BAC));
        colour = 0xE;
        if (*(int *)((char *)self + 0x11C) == 1)
        {
            SOC_CPY(rows.local_594, EQ_ARRAY(char, 0x00558BA4));
            colour = 0xF;
        }
        if (*(int *)((char *)self + 0x11C) == 2)
        {
            SOC_CPY(rows.local_594, EQ_ARRAY(char, 0x00558B98));
            colour = 0xD;
        }
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B8C), rows.local_594);
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0x23, colour, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B7C), (char *)self + 0x120);
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0x3C, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B6C), (char *)self + 0x140);
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0x55, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B60), (char *)self + 0x160);
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0x6E, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B50), (char *)self + 0x180);
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0x87, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B44), *(int *)((char *)self + 0x1A0));
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0xA0, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B38),
            STML_GET_CLASS(EQ_GLOBAL(void *, 0x006EFE2C))(*(short *)((char *)self + 0x1A4)));
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0xB9, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B2C),
            STML_GET_RACE(EQ_GLOBAL(void *, 0x006EFE2C))(*(short *)((char *)self + 0x1A8)));
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0xD2, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B1C), *(int *)((char *)self + 0x1B0));
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0xEB, 10, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B0C), *(int *)((char *)self + 0x1B4));
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0x104, 10, STML_FONT_DFC);
        entry = *(int *)((char *)self + 0x1B8);
        pos = (int *)STML_CTIME(&entry);
        len = STML_STRLEN((char *)pos);
        *((char *)pos + (len - 1)) = '\0';
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558B00), (char *)pos);
        STML_WRITE_TEXT(rows.local_214, 0xFA, 0x11D, 10, STML_FONT_DFC);
    }
    if (*(int *)((char *)self + 0x8C) == 3)
    {
        now = STML_GET_TIME();
        if ((u32)(now - *(int *)((char *)self + 0x80)) > 25000)
        {
            *(int *)((char *)self + 0x80) = 0;
            *(char *)((char *)self + 0x116) = 1;
            ((void (__fastcall *)(void *, int))(*(void ***)self)[2])(self, 0);
            if (*(int *)((char *)self + 0x88) == -1)
            {
                return;
            }
            *(int *)((char *)self + 0x8C) = *(int *)((char *)self + 0x88);
            ((void (__fastcall *)(void *, int))(*(void ***)self)[1])(self, 0);
            *(int *)((char *)self + 0x88) = -1;
            return;
        }
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558AE4), 0x10F, 10, 0xF, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558AA8), (char *)self + 0xB4, (char *)self + 0xD2);
        STML_WRITE_TEXT(rows.local_214, 0x90, 0x19, 0xF, STML_FONT_DFC);
        SOC_FMT(rows.local_214, EQ_ARRAY(char, 0x00558A90),
            STML_GET_RACE(EQ_GLOBAL(void *, 0x006EFE2C))(*(short *)((char *)self + 0xF0)),
            STML_GET_CLASS(EQ_GLOBAL(void *, 0x006EFE2C))(*(short *)((char *)self + 0xF4)));
        STML_WRITE_TEXT(rows.local_214, 0x90, 0x41, 0xF, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A88), 0x90, 0x69, 10, STML_FONT_DFC);
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A80), 0x144, 0x69, 10, STML_FONT_DFC);
        STML_DRAW_BORDERS(self)();
    }
    if (*(int *)((char *)self + 0x8C) != 4)
    {
        return;
    }
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A78), *(int *)((char *)self + 0x2028) + 10,
        *(int *)((char *)self + 0x202C) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT((char *)self + (*(int *)((char *)self + 0xDBC) + 9) * 0x188,
        *(int *)((char *)self + 0x20C8) + 10, *(int *)((char *)self + 0x20CC) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A74), *(int *)((char *)self + 0x2038) + 10,
        *(int *)((char *)self + 0x203C) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT((char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xDE8,
        *(int *)((char *)self + 0x20D8) + 10, *(int *)((char *)self + 0x20DC) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A6C), *(int *)((char *)self + 0x2048) + 10,
        *(int *)((char *)self + 0x204C) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT((char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xE08,
        *(int *)((char *)self + 0x20E8) + 10, *(int *)((char *)self + 0x20EC) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A64), *(int *)((char *)self + 0x2058) + 10,
        *(int *)((char *)self + 0x205C) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT((char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xE28,
        *(int *)((char *)self + 0x20F8) + 10, *(int *)((char *)self + 0x20FC) + 10, 10, STML_FONT_DFC);
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A5C), *(int *)((char *)self + 0x2068) + 10,
        *(int *)((char *)self + 0x206C) + 10, 10, STML_FONT_DFC);
    entry = *(int *)((char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xE48);
    pos = (int *)STML_CTIME(&entry);
    len = STML_STRLEN((char *)pos);
    *((char *)pos + (len - 1)) = '\0';
    STML_WRITE_TEXT(pos, *(int *)((char *)self + 0x2108) + 10, *(int *)((char *)self + 0x210C) + 10,
        10, STML_FONT_DFC);
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A54), *(int *)((char *)self + 0x2078) + 10,
        *(int *)((char *)self + 0x207C) + 10, 10, STML_FONT_DFC);
    switch (*(int *)((char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xE4C))
    {
    case 0:
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A34), *(int *)((char *)self + 0x2118) + 10,
            *(int *)((char *)self + 0x211C) + 10, 10, STML_FONT_DFC);
        break;
    case 1:
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A3C), *(int *)((char *)self + 0x2118) + 10,
            *(int *)((char *)self + 0x211C) + 10, 10, STML_FONT_DFC);
        break;
    case 2:
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A44), *(int *)((char *)self + 0x2118) + 10,
            *(int *)((char *)self + 0x211C) + 10, 10, STML_FONT_DFC);
        break;
    case 3:
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A4C), *(int *)((char *)self + 0x2118) + 10,
            *(int *)((char *)self + 0x211C) + 10, 10, STML_FONT_DFC);
        break;
    }
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A2C), *(int *)((char *)self + 0x2088) + 10,
        *(int *)((char *)self + 0x208C) + 10, 10, STML_FONT_DFC);
    len = STML_STRLEN((char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xE50);
    if ((int)len < 0x40)
    {
        STML_WRITE_TEXT((char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xE50,
            *(int *)((char *)self + 0x2128) + 10, *(int *)((char *)self + 0x212C) + 10, 10, STML_FONT_DF8);
    }
    else
    {
        SOC_CPY(rows.local_214, (char *)self + *(int *)((char *)self + 0xDBC) * 0x188 + 0xE50);
        SOC_CPY(rows.local_294, rows.local_1d4);
        rows.local_1d4[0] = 0;
        len = STML_STRLEN(rows.local_294);
        if ((int)len < 0x40)
        {
            STML_WRITE_TEXT(rows.local_214, *(int *)((char *)self + 0x2128) + 10,
                *(int *)((char *)self + 0x212C) + 10, 10, STML_FONT_DF8);
            STML_WRITE_TEXT(rows.local_294, *(int *)((char *)self + 0x2138) + 10,
                *(int *)((char *)self + 0x213C) + 10, 10, STML_FONT_DF8);
        }
        else
        {
            SOC_CPY(rows.local_314, rows.local_254);
            rows.local_254[0] = 0;
            len = STML_STRLEN(rows.local_314);
            if (0x3F < (int)len)
            {
                SOC_CPY(rows.local_394, rows.local_2d4);
                rows.local_2d4[0] = 0;
            }
            STML_WRITE_TEXT(rows.local_214, *(int *)((char *)self + 0x2128) + 10,
                *(int *)((char *)self + 0x212C) + 10, 10, STML_FONT_DF8);
            STML_WRITE_TEXT(rows.local_294, *(int *)((char *)self + 0x2138) + 10,
                *(int *)((char *)self + 0x213C) + 10, 10, STML_FONT_DF8);
            STML_WRITE_TEXT(rows.local_314, *(int *)((char *)self + 0x2148) + 10,
                *(int *)((char *)self + 0x214C) + 10, 10, STML_FONT_DF8);
            STML_WRITE_TEXT(rows.local_394, *(int *)((char *)self + 0x2158) + 10,
                *(int *)((char *)self + 0x215C) + 10, 10, STML_FONT_DF8);
        }
    }
    if (*(int *)((char *)self + 0xDC0) > -1)
    {
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A24), *(int *)((char *)self + 0x2098) + 10,
            *(int *)((char *)self + 0x209C) + 10, 10, STML_FONT_DFC);
    }
    STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558A1C), *(int *)((char *)self + 0x20A8) + 10,
        *(int *)((char *)self + 0x20AC) + 10, 10, STML_FONT_DFC);
    if (*(int *)((char *)self + 0xDC4) > -1)
    {
        STML_WRITE_TEXT(EQ_ARRAY(char, 0x00558894), *(int *)((char *)self + 0x20B8) + 10,
            *(int *)((char *)self + 0x20BC) + 10, 10, STML_FONT_DFC);
    }
    STML_DRAW_BORDERS(self)();
    return;
}
