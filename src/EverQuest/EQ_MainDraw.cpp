#include <stddef.h>

#include "trilogy/eqaddr.h"

struct EQMainDrawText_guess
{
    int truncateTextToWidth_guess(char* param_2, short param_3, short param_4);
};

typedef int (__stdcall *SetRect_guess_t)(int*, int, int, int, int);

// Owner of the member below; layout not recovered.
class EQ_LoadingS_guess
{
public:
    void WriteTextHD_guess(void);    /* 0x0043ACA9 */
};

// FUNCTION: TRILOGY 0x0043aca9
// @early-stop: the 0xc/7 search result becomes a neg/sbb select (the original keeps a branch into a shared 7 block), setRect and the 0x1e0 constant swap EBX/EDI at entry, EQ_THISCALL's dummy-EDX xors and mov ecx placement
/* fake match: setRect holds the SetRect import pointer hoisted once (the original's `mov edi, [SetRect]`), and entryMatched is volatile so the search flag stays in memory as the original's spill does */
void EQ_LoadingS_guess::WriteTextHD_guess(void)
{
    void* param_1 = (void*)this;
    int entryIndex;
    unsigned int uVar7;
    unsigned int* candidateCursor;
    char* candidateMarker;
    char text[128];
    int pos[4];
    int arg[4];
    int volatile entryMatched;
    int yOffset;
    int* visibleEntryIndices;
    int y;
    unsigned int* entryFlags;
    SetRect_guess_t setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);

    if (EQ_GLOBAL(int, 0x0058fc6c) != 2)
    {
        setRect(arg, 0x143, 0, 0x1ba, 0x1e0);
        setRect(pos, 0, 0, 0x280, 0x1e0);
        EQ_METHOD(4, void, 0x00450125, (void*, void*, int*, int*, int), param_1)(*(void**)((char*)EQ_GLOBAL(void*, 0x006e4e1c) + 4), arg, pos, 1);
    }
    if (*(short*)((char*)param_1 + 0x5f188) == 0x50)
    {
        EQ_METHOD(2, void, 0x004384b2, (void*, void*, int), EQ_GLOBAL(void*, 0x006b3498))(
             (char*)param_1 + (int)*(char*)((char*)param_1 + 0xfe0) * 0x1900 + 0x6ce0c, 1);
    }
    else if (EQ_GLOBAL(int, 0x0058fc6c) == 2)
    {
        setRect(arg, 0x169, 0x175, 400, 0x19c);
        EQ_METHOD(4, void, 0x00450125, (void*, void*, int*, int*, int), param_1)(*(void**)((char*)EQ_GLOBAL(void*, 0x006e4e1c) + 4), arg,
             (int*)((char*)param_1 + (int)*(char*)((char*)param_1 + 0xfe0) * 0x1900 + 0x6ce0c), 1);
    }
    if (*(short*)((char*)param_1 + 0x5f188) == 0x51)
    {
        EQ_METHOD(2, void, 0x004384b2, (void*, void*, int), EQ_GLOBAL(void*, 0x006b38dc))(
             (char*)param_1 + (int)*(char*)((char*)param_1 + 0xfe0) * 0x1900 + 0x6ce1c, 1);
    }
    else if (EQ_GLOBAL(int, 0x0058fc6c) == 2)
    {
        setRect(arg, 0x160, 0x1b2, 0x19b, 0x1cd);
        EQ_METHOD(4, void, 0x00450125, (void*, void*, int*, int*, int), param_1)(*(void**)((char*)EQ_GLOBAL(void*, 0x006e4e1c) + 4), arg,
             (int*)((char*)param_1 + (int)*(char*)((char*)param_1 + 0xfe0) * 0x1900 + 0x6ce1c), 1);
    }

    entryIndex = *(int*)((char*)param_1 + 0x6c4e0);
    yOffset = 0;
    visibleEntryIndices = (int*)((char*)param_1 + 0x6c488);
    y = 0x23;
    entryFlags = (unsigned int*)((char*)param_1 + entryIndex * 0x10 + 0x68604);
    do
    {
        if (entryFlags[-3] != 0)
        {
            uVar7 = *entryFlags;
            if ((uVar7 & 1) == 0)
            {
                if (entryIndex == *(int*)((char*)param_1 + 0x6c4d8))
                    uVar7 = 0x12;
                else if ((uVar7 & 2) == 0)
                {
                    if ((uVar7 & 0x100) != 0)
                        uVar7 = 0xe;
                    else if ((uVar7 & 0x80) != 0)
                        uVar7 = 0x10;
                    else if ((uVar7 & 0x40) != 0)
                        uVar7 = 10;
                    else
                        uVar7 = ((int)(char)uVar7 & 0x20U | 0xd0) >> 4;
                }
                else
                {
                    entryMatched = 0;
                    if (EQ_GLOBAL(char, 0x005fe968) != 0)
                    {
                        candidateCursor = EQ_ARRAY(unsigned int, 0x005fe954);
                        candidateMarker = EQ_ARRAY(char, 0x005fe8bd);
                        do
                        {
                            if (*candidateMarker != 0 && *candidateCursor != 0 && *candidateCursor == entryFlags[-3])
                                entryMatched = 1;
                            candidateMarker += 0x1e;
                            candidateCursor++;
                        } while ((int)candidateMarker < 0x005fe953);
                        if (entryMatched != 0)
                            uVar7 = 0xc;
                        else
                            uVar7 = 7;
                    }
                    else
                        uVar7 = 7;
                }
                if ((*entryFlags & 4) != 0)
                {
                    EQ_CDECL(char*, 0x005219c0, (char*, const char*))(text, EQ_ARRAY(char, 0x00559640));
                    EQ_CDECL(char*, 0x005219d0, (char*, const char*))
                        (text,
                         EQ_METHOD(1, char*, 0x004d02fd, (void*, char*), EQ_GLOBAL(void*, 0x006efe2c))((char*)(entryFlags[-3] + 1)));
                }
                else
                {
                    EQ_CDECL(char*, 0x005219c0, (char*, const char*))
                        (text,
                         EQ_METHOD(1, char*, 0x004d02fd, (void*, char*), EQ_GLOBAL(void*, 0x006efe2c))((char*)(entryFlags[-3] + 1)));
                }
                if (EQ_GLOBAL(int, 0x0058fc6c) == 2)
                {
                    EQ_CDECL(char*, 0x005219c0, (char*, const char*))
                        (text,
                         EQ_METHOD(3, char*, 0x00457110, (void*, char*, short, short), param_1)(text, 100, 0));
                    *(int*)((char*)param_1 + 0x90274) = 0;
                    EQ_METHOD(6, void, 0x0048d21a, (void*, char*, int, int, unsigned int, int, int), EQ_GLOBAL(void*, 0x006e4e68))(text,
                         *(int*)((char*)param_1 + 0x90264) + 10,
                         yOffset + 4 + *(int*)((char*)param_1 + 0x90268),
                         uVar7, EQ_GLOBAL(int, 0x006efdfc), 1);
                }
                else
                {
                    EQ_CDECL(char*, 0x005219c0, (char*, const char*))
                        (text,
                         EQ_METHOD(3, char*, 0x00457110, (void*, char*, short, short), param_1)(text, 0x5a, 0));
                    EQ_METHOD(6, void, 0x0048d21a, (void*, char*, int, int, unsigned int, int, int), EQ_GLOBAL(void*, 0x006e4e68))(text, 0xf, y, uVar7,
                         EQ_GLOBAL(int, 0x006efdfc), 1);
                }
                *visibleEntryIndices++ = entryIndex;
                yOffset += 0xc;
                y += 0xf;
            }
        }
        entryIndex++;
        entryFlags += 4;
    } while (entryIndex < 1000 && y < 0x14f);
    EQ_METHOD(2, void, 0x004384b2, (void*, void*, int), EQ_GLOBAL(void*, 0x006b3640))(
         (char*)param_1 + (int)*(char*)((char*)param_1 + 0xfe0) * 0x1900 + 0x6cfcc, 1);
}

typedef int (__stdcall *CopyRect_guess_t)(int*, int*);

#define DG_THIS ((int)self)
#define DG_CHAR(o) (*(char*)((char*)self + (o)))
#define DG_SHORT(o) (*(short*)((char*)self + (o)))
#define DG_INT(o) (*(int*)((char*)self + (o)))
#define DG_UINT(o) (*(unsigned int*)((char*)self + (o)))
#define DG_PC EQ_GLOBAL(char*, 0x006e4da8)
#define DG_PCSPELL(i) (*(short*)(DG_PC + 0x952 + (i) * 2))
#define DG_SETRECT EQ_GLOBAL(SetRect_guess_t, 0x00534248)
#define g_CopyRect_guess EQ_GLOBAL(CopyRect_guess_t, 0x00534240)
#define DG_WRITE(text, x, y, color, font) \
    EQ_METHOD(6, void, 0x0048d21a, (void*, char*, int, int, unsigned int, int, int), EQ_GLOBAL(void*, 0x006e4e68))( (char*)(text), (x), (y), (color), (font), 1)
#define DG_WRITE16(text, x, y, color, font) \
    EQ_METHOD(6, void, 0x0048d21a, (void*, char*, int, int, short, int, int), EQ_GLOBAL(void*, 0x006e4e68))( (char*)(text), (x), (y), (color), (font), 1)
#define DG_DRAW(img, rect) \
    EQ_METHOD(2, void, 0x004384b2, (void*, void*, int), (void*)(img))((void*)(rect), 1)
#define DG_DRAWIMAGE(rect, v) EQ_CDECL(void, 0x00438423, (void*, int))((void*)(rect), (v))
#define DG_STRCPY(d, s) EQ_CDECL(char*, 0x005219c0, (char*, const char*))((d), (const char*)(s))
#define DG_SPRINTF EQ_CDECL(int, 0x005224fb, (char*, const char*, ...))
#define DG_TRUNC(text, w) \
    EQ_METHOD(3, char*, 0x00457110, (void*, char*, short, short), self)((char*)(text), (w), 0)
#define DG_PARTIAL(a, b, c) \
    EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)( (void*)(a), (void*)(b), (void*)(c), 1)
#define DG_ITEMIMG(idx) EQ_ARRAY(int, 0x006b32b4)[idx]
#define DG_ITEMDEF(idx) EQ_ARRAY(char*, 0x006e33a4)[idx]
#define DG_EQSLOT(pcoff, rectoff) \
    if (*(int*)(DG_PC + (pcoff)) != 0) \
        DG_DRAW(DG_ITEMIMG(*(short*)(*(int*)(DG_PC + (pcoff)) + 0x84)), \
                DG_CHAR(0xfe0) * 0x1900 + (rectoff) + DG_THIS);
#define DG_COIN(label, rectoff, valexpr) \
    iVar9 = DG_CHAR(0xfe0) * 0x1900 + DG_THIS; \
    DG_WRITE(label, *(int*)(iVar9 + (rectoff)) + 10, *(int*)(iVar9 + (rectoff) + 4), 10, DG_FONT); \
    DG_SPRINTF(local_6bc, (const char*)0x0053e16c, valexpr); \
    iVar9 = DG_CHAR(0xfe0) * 0x1900; \
    DG_WRITE(local_6bc, *(int*)(iVar9 + DG_THIS + (rectoff)) + 0x14, \
             *(int*)(iVar9 + (rectoff) + 4 + DG_THIS) + 0x14, 10, DG_FONT);
#define DG_UPV(a, b) \
    EQ_METHOD(3, void, 0x004500fd, (void*, int, int, int), self)((int)(a), (int)(b), 1)
#define DG_IR(va, off) (*(int*)(EQ_GLOBAL(char*, va) + (off)))
#define DG_STATS_BLOCK(off, enc, str, hp, mx, def) \
    if (DG_CHAR(0x8fbd2) == 0) \
    { \
        DG_CHAR(0x8fbd2) = DG_PC[0x38]; \
        DG_SHORT(0x8fbd4) = (short)(hp); \
        DG_SHORT(0x8fbd6) = (short)(mx); \
        DG_SHORT(0x8fbd8) = (short)(def); \
        DG_SHORT(0x8fbdc) = (short)(off); \
        DG_SHORT(0x8fbe2) = (short)local_628; \
        DG_SHORT(0x8fbe0) = (short)local_624; \
        DG_SHORT(0x8fbe4) = (short)local_620; \
        DG_SHORT(0x8fbe6) = (short)local_61c; \
        DG_SHORT(0x8fbe8) = (short)local_618; \
        DG_SHORT(0x8fbea) = (short)local_614; \
        DG_SHORT(0x8fbec) = (short)local_610; \
        DG_CHAR(0x8fbee) = (char)local_60c; \
        DG_CHAR(0x8fbef) = (char)local_608; \
        DG_CHAR(0x8fbf0) = (char)local_604; \
        DG_CHAR(0x8fbf1) = (char)local_600; \
        DG_CHAR(0x8fbf2) = (char)local_5fc; \
        DG_SHORT(0x8fbf4) = (short)(enc); \
        DG_SHORT(0x8fbf6) = (short)(str); \
    } \
    if ((unsigned char)DG_CHAR(0x8fbd2) != *(unsigned char*)(DG_PC + 0x38)) DG_INT(0xeb8) = 1; \
    if (DG_SHORT(0x8fbd4) != (short)(hp)) DG_INT(0xeb8) = 1; \
    if (DG_SHORT(0x8fbd6) != (short)(mx)) DG_INT(0xeb8) = 1; \
    if (DG_SHORT(0x8fbd8) != (short)(def)) DG_INT(0xeb8) = 1; \
    if (DG_SHORT(0x8fbdc) != (short)(off)) DG_INT(0xeb8) = 1; \
    if ((int)DG_SHORT(0x8fbe2) != local_628) DG_INT(0xeb8) = 1; \
    if ((int)DG_SHORT(0x8fbe0) != local_624) DG_INT(0xeb8) = 1; \
    if ((int)DG_SHORT(0x8fbe4) != local_620) DG_INT(0xeb8) = 1; \
    if ((int)DG_SHORT(0x8fbe6) != local_61c) DG_INT(0xeb8) = 1; \
    if ((int)DG_SHORT(0x8fbe8) != local_618) DG_INT(0xeb8) = 1; \
    if ((int)DG_SHORT(0x8fbea) != local_614) DG_INT(0xeb8) = 1; \
    if ((int)DG_SHORT(0x8fbec) != local_610) DG_INT(0xeb8) = 1; \
    if (DG_CHAR(0x8fbee) != local_60c) DG_INT(0xeb8) = 1; \
    if (DG_CHAR(0x8fbef) != local_608) DG_INT(0xeb8) = 1; \
    if (DG_CHAR(0x8fbf0) != local_604) DG_INT(0xeb8) = 1; \
    if (DG_CHAR(0x8fbf1) != local_600) DG_INT(0xeb8) = 1; \
    if (DG_CHAR(0x8fbf2) != local_5fc) DG_INT(0xeb8) = 1; \
    if (DG_SHORT(0x8fbf4) != (short)(enc)) DG_INT(0xeb8) = 1; \
    if (DG_SHORT(0x8fbf6) != (short)(str)) DG_INT(0xeb8) = 1; \
    DG_CHAR(0x8fbd2) = DG_PC[0x38]; \
    DG_SHORT(0x8fbd4) = (short)(hp); \
    DG_SHORT(0x8fbd6) = (short)(mx); \
    DG_SHORT(0x8fbd8) = (short)(def); \
    DG_SHORT(0x8fbdc) = (short)(off); \
    DG_SHORT(0x8fbe2) = (short)local_628; \
    DG_SHORT(0x8fbe0) = (short)local_624; \
    DG_SHORT(0x8fbe4) = (short)local_620; \
    DG_SHORT(0x8fbe6) = (short)local_61c; \
    DG_SHORT(0x8fbe8) = (short)local_618; \
    DG_SHORT(0x8fbea) = (short)local_614; \
    DG_SHORT(0x8fbec) = (short)local_610; \
    DG_CHAR(0x8fbee) = (char)local_60c; \
    DG_CHAR(0x8fbef) = (char)local_608; \
    DG_CHAR(0x8fbf0) = (char)local_604; \
    DG_CHAR(0x8fbf1) = (char)local_600; \
    DG_CHAR(0x8fbf2) = (char)local_5fc; \
    DG_SHORT(0x8fbf4) = (short)(enc); \
    DG_SHORT(0x8fbf6) = (short)(str);
#define DG_FONTO EQ_GLOBAL(int, 0x006efe00)
#define DG_IMGSET(ovl) *EQ_GLOBAL(int*, 0x006b341c) = (int)DG_OVL(ovl)
#define DG_IMGRECT(a, b, c, d) DG_SETRECT(EQ_GLOBAL(int*, 0x006b341c) + 1, a, b, c, d)
#define DG_D341C(rect) DG_DRAW(EQ_GLOBAL(void*, 0x006b341c), rect)
#define DG_DR(off) (DG_CHAR(0xfe0) * 0x1900 + (off) + DG_THIS)
#define DG_RX(off) DG_INT(DG_CHAR(0xfe0) * 0x1900 + (off))
#define DG_RY(off) DG_INT(DG_CHAR(0xfe0) * 0x1900 + (off) + 4)
#define DG_WTEXT(text, off, dx, dy, color) \
    DG_WRITE(text, DG_RX(off) + (dx), DG_RY(off) + (dy), color, DG_FONTO)
#define DG_SW(zerocond, off, x0, c0, s0, x1, c1, s1) \
    if (zerocond) \
    { \
        iVar19 = DG_RX(off) + (x0); \
        iVar9 = DG_RY(off); \
        uVar11 = (c0); \
        puVar26 = (s0); \
    } \
    else \
    { \
        iVar19 = DG_RX(off) + (x1); \
        iVar9 = DG_RY(off); \
        uVar11 = (c1); \
        puVar26 = (s1); \
    } \
    DG_WRITE(puVar26, iVar19, iVar9 - 1, uVar11, DG_FONTO);
#define DG_ONOFF(globalexpr, off) DG_SW((globalexpr) == 0, off, 5, 0xd, 0x00559eb0, 8, 7, 0x00559eb4)
#define DG_KEY(text, n) \
    iVar9 = DG_CHAR(0xfe0) * 0x1900; \
    DG_WRITE(text, DG_INT(iVar9 + 0x6d0dc + (n) * 0x10), \
             DG_INT(iVar9 + 0x6d0e0 + (n) * 0x10) - 0xf, 10, DG_FONTO);
#define DG_KEYLAST(text) \
    iVar9 = DG_CHAR(0xfe0) * 0x1900; \
    iVar19 = DG_INT(iVar9 + 0x6d160) - 0xf; \
    uVar11 = DG_INT(iVar9 + 0x6d15c); \
    pcVar32 = (char*)(text);
#define DG_SLIDER(slotval, divisor, off) \
    DG_SETRECT(local_6f0, (int)(short)(((int)(short)(slotval) * 0x36) / (divisor)) + 0x1c, \
               DG_INT(DG_CHAR(0xfe0) * 0x1900 + (off)) - 1, 0x280, 0x1e0); \
    DG_DRAW(EQ_GLOBAL(void*, 0x006b3640), local_6f0);
#define DG_FONT EQ_GLOBAL(int, 0x006efdfc)
#define DG_FONT2 EQ_GLOBAL(int, 0x006efdf8)
#define DG_FONT3 EQ_GLOBAL(int, 0x006efdf4)
#define DG_COMBINE(item) EQ_CDECL(int, 0x004a1d83, (void*))((void*)(item))
#define DG_SLOTRECT(slot, off) \
    DG_SETRECT(local_6f0, DG_INT((DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + (off)) + 3, \
               DG_INT((DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + (off) + 4) + 3, 0x280, 0x1e0)
#define DG_STACKCOUNT(slot, off, item) \
    iVar19 = (DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + DG_THIS; \
    iVar9 = *(int*)(iVar19 + (off) + 0xc); \
    iVar19 = *(int*)(iVar19 + (off) + 8); \
    DG_SETRECT(local_6f0, iVar19 - 0x12, iVar9 - 0xe, iVar19, iVar9); \
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), local_6f0); \
    DG_SPRINTF(local_6bc, (const char*)0x0053e16c, \
               (int)((unsigned char)((char*)(item))[0xda] - 1) / (int)((char*)(item))[0xbb] + 1); \
    iVar9 = (DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + DG_THIS; \
    DG_WRITE(local_6bc, *(int*)(iVar9 + (off) + 8) - 0xd, *(int*)(iVar9 + (off) + 0xc) - 9, 10, DG_FONT3);
#define DG_ITEMLOOP(start, count, off, itemlv) \
    local_6f4 = 0; \
    local_6fc = (start); \
    local_6e0 = (count); \
    do \
    { \
        if ((itemlv) != 0) \
        { \
            DG_SLOTRECT(local_6f4, off); \
            DG_DRAW(DG_ITEMIMG(*(short*)((itemlv) + 0x84)), local_6f0); \
            if (DG_COMBINE(itemlv) != 0) \
            { \
                DG_STACKCOUNT(local_6f4, off, itemlv) \
            } \
        } \
        local_6f4 = local_6f4 + 1; \
        local_6fc = local_6fc + 4; \
        local_6e0 = local_6e0 - 1; \
    } while (local_6e0 != 0);
#define DG_OVL(va) (*(void**)((char*)EQ_GLOBAL(void*, va) + 4))
#if 0
// literal EQ_GLOBAL uses for the addresses that DG_OVL / DG_IR take as macro arguments
EQ_GLOBAL(void*, 0x006e4e00) EQ_GLOBAL(void*, 0x006e4e04) EQ_GLOBAL(void*, 0x006e4e08)
EQ_GLOBAL(void*, 0x006e4e0c) EQ_GLOBAL(void*, 0x006e4e10) EQ_GLOBAL(void*, 0x006e4e1c)
EQ_GLOBAL(void*, 0x006e4e24) EQ_GLOBAL(void*, 0x006e4e2c) EQ_GLOBAL(void*, 0x006e4e34)
EQ_GLOBAL(void*, 0x006e4e3c) EQ_GLOBAL(void*, 0x006e4e40) EQ_GLOBAL(void*, 0x006e4e44)
EQ_GLOBAL(void*, 0x006e4de8)
EQ_GLOBAL(char*, 0x006e4e48) EQ_GLOBAL(char*, 0x006b36c8) EQ_GLOBAL(char*, 0x006b36cc)
EQ_GLOBAL(char*, 0x006b36d0) EQ_GLOBAL(char*, 0x006b3804) EQ_GLOBAL(char*, 0x006b3844)
EQ_GLOBAL(char*, 0x006b38b0) EQ_GLOBAL(char*, 0x006b38b4)
#endif
#define DG_GEM_DRAW(v, rect) \
    switch (v) \
    { \
    case 1: case 7: DG_DRAW(EQ_GLOBAL(void*, 0x006b51f8), rect); break; \
    case 2: case 6: DG_DRAW(EQ_GLOBAL(void*, 0x006b51fc), rect); break; \
    case 3: case 5: DG_DRAW(EQ_GLOBAL(void*, 0x006b5200), rect); break; \
    case 4: DG_DRAW(EQ_GLOBAL(void*, 0x006b5204), rect); break; \
    }

#define DG_TEXT(text, x, y, color, font) \
    EQ_METHOD(6, void, 0x0048d21a, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006e4e68)) \
        ((char*)(text), (x), (y), (color), (font), 1)
#define DG_PART(ovl, src, dst) \
    EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)((ovl), (void*)(src), (void*)(dst), 1)
#define DG_TRIM(name) \
    EQ_METHOD(1, char*, 0x004d02fd, (void*, char*), EQ_GLOBAL(void*, 0x006efe2c))((char*)(name))
#define DG_LANG(id) EQ_METHOD(1, char*, 0x004c8fce, (void*, short), EQ_GLOBAL(void*, 0x006efe2c))((short)(id))
#define DG_SLOT(off) DG_INT(DG_CHAR(0xfe0) * 0x640 + (off))
#define DG_FONT4 EQ_GLOBAL(int, 0x006efe04)
#define DG_FONT5 EQ_GLOBAL(int, 0x006efe08)
/* experience needed for a level: EQ_PC::EQ_PC_guess::GetExpForLevel_guess(level, class, race) on g_pc */
#define DG_EXPLVL(lvl) \
    EQ_METHOD(3, int, 0x004807fc, (void*, char, char, short), DG_PC)((char)(lvl), DG_PC[0x36], *(short*)(DG_PC + 0x34))
#define DG_EXP_PROGRESS() \
    EQ_METHOD(2, void, 0x0046a177, (void*, int, int), self)( \
        *(int*)(DG_PC + 0x3c) - DG_EXPLVL(DG_PC[0x38] - 1), DG_EXPLVL(DG_PC[0x38]) - DG_EXPLVL(DG_PC[0x38] - 1))
/* message board entry date "MM/DD/YY HH:MMgh" from bytes 0xc..0x15 of a message */
#define DG_MSGDATE(m) \
    f_6bc[0] = (m)[0xe]; f_6bc[1] = (m)[0xf]; f_6bc[2] = '/'; f_6bc[3] = (m)[0x10]; f_6bc[4] = (m)[0x11]; \
    f_6bc[5] = '/'; f_6bc[6] = (m)[0xc]; f_6bc[7] = (m)[0xd]; f_6bc[8] = ' '; f_6bc[9] = (m)[0x12]; \
    f_6bc[10] = (m)[0x13]; f_6bc[11] = ':'; f_6bc[12] = (m)[0x14]; f_6bc[13] = (m)[0x15]; f_6bc[14] = 'g'; \
    f_6bc[15] = 'h'; f_6bc[16] = 0;
#define DG_MSG(i) (*(char**)((char*)self + 0x138 + (i) * 4))

#undef f_6b8
#undef f_6b7
#undef f_6b6
#undef f_6b5
#undef f_6b4
#undef f_6b3
#undef f_6b2
#undef f_6b1
#undef f_6b0
#undef f_6af
#undef f_6ae
#undef f_6ad
#undef f_6ac

// FUNCTION: TRILOGY 0x0043b010
// Old-UI full-screen draw, __thiscall on EQ_Main; the page drawn is chosen by the byte at this+0xfe0 and the
// video-mode global 0x0058fc6c. Returns at once if 0x006efc70 is 1 or the active check 0x004504ac fails.
// [this+0xc] is the full-redraw flag and this+0xeb8 a counter; later parts redraw only when their own flag is set.
// A switch at 0x00446da7 handles pages 0-12: item windows (inventory, trade, merchant, bags, worn gear, hot keys),
// options and key-binding pages, player/target names with the experience bar, bug report, boards and help.
// The pieces are joined by gotos: L_00446D83 is the switch, L_0044FE15 the shared EQ_Image draw and epilogue.
// ORIGINAL FILE: EverQuest/EQ_MainDraw.cpp (from Mac debug map)
/* EQ_Main::draw_guess; `this` (EQ_Main) arrives in ECX, kept in ESI (self). */
void __fastcall EQ_Main_draw_guess(int self)
{
    // ---- frame: 0x6fc bytes; declared ONCE here, no piece declares locals (VC6 lays them out itself,
    // so the order here doesn't matter; sizes/types do). Offsets are entry-ESP relative (f_N = Ghidra local_N = entry-N).
    // Stack addressing in the asm: after the prologue ESP = B = entry-0x70c, so [ESP+N] with k dwords pushed
    // since is entry-0x70c+N-4k. Several "address taken" slots below are such LEAs misread with pushes
    // outstanding, not real locals: f_718/f_708/f_700 lie below the frame, f_6ec/f_6e8/f_6e4 are inside the
    // f_6f0 rect, f_6c8..f_6be inside the f_6cc rect, f_6b8..f_658 inside the f_6bc text buffer (f_6bc + 0x6bc - N).
    // Recompute the real slot from the asm before using one of those.
    char f_718[16]; /* TODO type: address taken */       // entry-0x718; P01
    char f_708[8]; /* TODO type: address taken */        // entry-0x708; P01
    char f_700[4]; /* TODO type: address taken */        // entry-0x700; P01,P03,P07
    int f_6fc;                                           // entry-0x6fc Ghidra local_6fc; P01,P02,P03,P04,P05,P06,P07 (int; cast where a piece holds a pointer)
    int f_6f8;                                           // entry-0x6f8 Ghidra local_6f8; P01,P02,P03,P06,P07
    int f_6f4;                                           // entry-0x6f4 Ghidra local_6f4; P01,P02,P03,P06,P07
    int f_6f0[4];                                        // entry-0x6f0 Ghidra local_6f0 (RECT left, top, right, bottom); P01,P02,P03,P04,P06,P07
    char f_6ec[4]; /* TODO type: address taken */        // entry-0x6ec; P02,P03,P06,P07
    char f_6e8[4]; /* TODO type: address taken */        // entry-0x6e8; P01,P03,P07
    char f_6e4[4]; /* TODO type: address taken */        // entry-0x6e4; P01,P03
    int f_6e0;                                           // entry-0x6e0 Ghidra local_6e0; P01,P02,P03,P04,P06,P07 (int; P01: help line counter)
    short * f_6dc;                                       // entry-0x6dc Ghidra local_6dc; P02,P03,P04,P05,P06,P07
    short * f_6d8;                                       // entry-0x6d8 Ghidra local_6d8; P01,P02,P03,P04,P05,P06,P07 (P03 switches on it as an int)
    int f_6d4;                                           // entry-0x6d4 Ghidra local_6d4; P01,P02,P03,P06,P07
    int f_6d0;                                           // entry-0x6d0 Ghidra local_6d0; P01,P03,P06,P07
    int f_6cc[4];                                        // entry-0x6cc Ghidra local_6cc (RECT); P01,P02,P03,P04,P06,P07
    int f_6c8; /* TODO type */                           // entry-0x6c8; P02
    char f_6c4[4]; /* TODO type: address taken */        // entry-0x6c4; P02
    char f_6c0[2]; /* TODO type: address taken */        // entry-0x6c0; P03
    char f_6be[2]; /* TODO type: address taken */        // entry-0x6be; P01
    char f_6bc[128];                                     // entry-0x6bc Ghidra local_6bc: the text buffer, entry-0x6bc..entry-0x63c; P01,P02,P03,P04,P06,P07
#define f_6b8 (f_6bc[4])                                 // entry-0x6b8 Ghidra local_6b8 (byte of f_6bc); P01,P03,P06,P07
#define f_6b7 (f_6bc[5])
#define f_6b6 (f_6bc[6])
#define f_6b5 (f_6bc[7])
#define f_6b4 (f_6bc[8])                                 // entry-0x6b4 Ghidra local_6b4; P01,P02,P03,P04,P06,P07
#define f_6b3 (f_6bc[9])
#define f_6b2 (f_6bc[10])
#define f_6b1 (f_6bc[11])
#define f_6b0 (f_6bc[12])                                // entry-0x6b0 Ghidra local_6b0; P01,P03,P07
#define f_6af (f_6bc[13])
#define f_6ae (f_6bc[14])
#define f_6ad (f_6bc[15])
#define f_6ac (f_6bc[16])                                // entry-0x6ac Ghidra local_6ac; P01,P03,P04,P06,P07
    char f_6a8[4]; /* TODO type: address taken */        // entry-0x6a8; P03,P07
    char f_6a4[4]; /* TODO type: address taken */        // entry-0x6a4; P03,P04,P07
    char f_6a0[4]; /* TODO type: address taken */        // entry-0x6a0; P02,P03,P07
    char f_69c[4]; /* TODO type: address taken */        // entry-0x69c; P01,P03,P04,P07
    char f_698[4]; /* TODO type: address taken */        // entry-0x698; P03,P07
    char f_694[4]; /* TODO type: address taken */        // entry-0x694; P01,P02
    char f_690[4]; /* TODO type: address taken */        // entry-0x690; P03,P07
    char f_68c[8]; /* TODO type: address taken */        // entry-0x68c; P01,P03,P07
    char f_684[8]; /* TODO type: address taken */        // entry-0x684; P01
    char f_67c[12]; /* TODO type: address taken */       // entry-0x67c; P01
    char f_670[12]; /* TODO type: address taken */       // entry-0x670; P01
    char f_664[12]; /* TODO type: address taken */       // entry-0x664; P01
    char f_658[28]; /* TODO type: address taken */       // entry-0x658; P01
    int f_63c;                                           // entry-0x63c Ghidra local_63c; P03,P06,P07
    unsigned int f_628;                                  // entry-0x628 Ghidra local_628; P03,P07
    unsigned int f_624;                                  // entry-0x624 Ghidra local_624; P03,P07
    unsigned int f_620;                                  // entry-0x620 Ghidra local_620; P03,P07
    struct EQPlayer * f_61c;                             // entry-0x61c Ghidra local_61c; P03,P07
    unsigned int f_618;                                  // entry-0x618 Ghidra local_618; P03,P07
    unsigned int f_614;                                  // entry-0x614 Ghidra local_614; P03,P07
    unsigned int f_610;                                  // entry-0x610 Ghidra local_610; P03,P07
    int f_60c;                                           // entry-0x60c Ghidra local_60c; P03,P07
    int f_608;                                           // entry-0x608 Ghidra local_608; P03,P07
    int f_604;                                           // entry-0x604 Ghidra local_604; P03,P07
    int f_600;                                           // entry-0x600 Ghidra local_600; P03,P07
    int f_5fc;                                           // entry-0x5fc Ghidra local_5fc; P03,P07
    int f_5f8;                                           // entry-0x5f8 Ghidra local_5f8; P03,P06,P07
    int f_5f4;                                           // entry-0x5f4 Ghidra local_5f4; P03
    char f_5f0[64];                                      // entry-0x5f0 Ghidra local_5f0; P01,P02,P04,P06
    int f_5b0[4];                                        // entry-0x5b0 Ghidra local_5b0 (RECT); P02
    char f_5a0[128];                                     // entry-0x5a0 Ghidra local_5a0; P02,P06
    int f_520[4];                                        // entry-0x520 Ghidra local_520 (RECT); P02
    int f_510[4];                                        // entry-0x510 Ghidra local_510 (RECT); P02
    int f_500[4];                                        // entry-0x500 Ghidra local_500 (RECT); P02
    int f_4f0[4];                                        // entry-0x4f0 Ghidra local_4f0 (RECT); P02
    int f_4e0[4];                                        // entry-0x4e0 Ghidra local_4e0 (RECT); P02
    int f_4d0[4];                                        // entry-0x4d0 Ghidra local_4d0 (RECT); P02
    int f_4c0[4];                                        // entry-0x4c0 Ghidra local_4c0 (RECT); P02
    int f_4b0[4];                                        // entry-0x4b0 Ghidra local_4b0 (RECT); P02
    int f_4a0[4];                                        // entry-0x4a0 Ghidra local_4a0 (RECT); P02
    int f_490[4];                                        // entry-0x490 Ghidra local_490 (RECT); P02
    char f_484[4]; /* TODO type: address taken */        // entry-0x484; P01
    char f_480[128];                                     // entry-0x480 Ghidra local_480; P01
    char f_400[1024];                                    // entry-0x400 Ghidra local_400; P01
    // ---- values carried in registers into other pieces. reg: definition -> pieces reading it
    //   EBX: POP EBX @0043B01E (= 1 for the whole function) -> every piece: write the constant 1
    //   ESI: MOV ESI,ECX @0043B01C (this) -> every piece: self (DG_THIS, DG_INT(off), ...)
    //   EDI: MOV EDI,[PTR_SetRect_00534248] @00441B39 (P01) -> P01,P04,P05,P06,P07: `setRect`
    //   EDI: MOV EDI,[PTR_SetRect_00534248] @0043B093 (P02) -> P03: P02 does `setRect = DG_SETRECT;` there
    //   EAX: DEC EAX @004458EB (P05) -> P04 at L_004458F2; SUB EAX,0x6f @00446121 (P04, own use);
    //        CALL draw_guess @0043DB81..0043DCF7 (P02) -> P03: carry it in `carryEax`
    SetRect_guess_t setRect;                             // EDI: the SetRect import pointer
    int carryEax;                                        // EAX values handed from one piece to another
    // ---- P01's register values (no frame slot in the original)
    int clipTop;                                         // 0044E05B/0044E256: EDX, clipped top
    int clipBottom;                                      // EAX, clipped bottom
    short wrapPos;                                       // 0044ED7B: SI, position in f_400
    short wrapLen;                                       // BP, strlen(f_400)
    short wrapScan;                                      // DX, backwards scan for a blank
    short wrapBreak;                                     // DI, break position
    int wrapWidth;                                       // EDI, pixel width so far
    short textX;                                         // 0044FB30: BP, centred x
    short helpLine;                                      // 0044FD4C: help line index
    short helpEnd;                                       // 0044FCE2: BP, last help line
    // ---- switch selectors: P01 @00446DA7 (int)(char)DG_CHAR(0xfe0); P02 @0043D928 / P06 @00446EE3 the
    // short in AX; P03 @00441087 (int)f_6d8; P04 @0044212D (unsigned char)DG_CHAR(0x8fbac) (cases 1..8),
    // @00441E74 byte-table switch on the int in EAX; P05 @004441C9 (unsigned char)DG_CHAR(0x100d).
    // L_0044FE15 is the shared `call EQ_Image::draw` + epilogue tail (VC6 cross-jumping): a piece that
    // jumps there writes its own draw call and then `goto L_0044FE15;` (the label sits on the return).

    // REGION P01 0x0043B010-0x0044FE25: 8528 B, entry / spine (prologue, top-level flow, epilogue)
    //   defines L_0043B010, L_00446D83, L_0044FE15, L_0044FE1A; exits L_0043B077, L_00441B4D, L_00446DAE, L_004472D6, L_00447D0D, L_004487EF +2; returns
    //   switch scaffold for this piece (paste where the dispatch is):
    //   // SWITCH @00446DA7 (in P01): table 0x0044FF1D, 13 entries (cmp), bias 0; selector: POP ECX ; CMP EAX,0xc ; POP ECX ; JA caseD_7 ; JMP dword ptr [EAX*0x4 + switchdataD_0044ff1d]
    //   switch (/* TODO selector (register EAX) */ 0) {
    //   case 0: goto L_00446DAE; /* P06 */
    //   case 1: goto L_004472D6; /* P06 */
    //   case 2: goto L_00447D0D; /* P06 */
    //   case 3: goto L_004487EF; /* P06 */
    //   case 4: goto L_00448F0D; /* P07 */
    //   case 5: goto L_0044D2CB; /* P06 */
    //   case 6: goto L_0044DDD2; /* P01 */
    //   case 7: goto L_0044FE1A; /* P01 */
    //   case 8: goto L_0044E923; /* P01 */
    //   case 9: goto L_0044EEAB; /* P01 */
    //   case 0xA: goto L_0044F6E6; /* P01 */
    //   case 0xB: goto L_0044F9A2; /* P01 */
    //   case 0xC: goto L_0044FCE2; /* P01 */
    //   default: goto L_0044FE1A; /* P01 */
    //   }
L_0043B010:
    if (EQ_GLOBAL(char, 0x006efc70) == 1)
        return;
    if (EQ_METHOD(0, int, 0x004504ac, (void*), self)() == 0)
        return;
    if (DG_CHAR(0xfe0) >= 8)
        DG_INT(0xeb8) = 1;
    if (DG_INT(0x685e4) != 0)
        DG_INT(0xeb8) = 1;
    DG_STRCPY(f_5f0, 0x0055a07c);
    if (EQ_GLOBAL(int, 0x0058fc6c) != 1)
        goto L_0043B077;

    // 00441AE2
    if (EQ_GLOBAL(int, 0x006e5480) == 0)
    {
        if (DG_INT(0xeb8) != 0)
        {
            if (DG_INT(0xeb8) > 3)
                DG_INT(0xeb8) = 0;
            else
            {
                DG_INT(0xc) = 1;
                DG_INT(0xeb8)++;
            }
        }
    }
    else
        DG_INT(0xc) = 1;
    if (DG_INT(0xc) != 0 && EQ_GLOBAL(char*, 0x006e4de8) != 0)
        *(int*)(EQ_GLOBAL(char*, 0x006e4de8) + 0x1ec) = 1;
    DG_STRCPY(f_5f0, 0x00559f00);
    setRect = DG_SETRECT;
    if (DG_CHAR(0xfe0) <= 3 || DG_CHAR(0xfe0) == 5)
        goto L_00441B4D;

L_00446D83:
    DG_STRCPY(f_5f0, 0x00559734);
    switch (DG_CHAR(0xfe0))
    {
    case 0: goto L_00446DAE;
    case 1: goto L_004472D6;
    case 2: goto L_00447D0D;
    case 3: goto L_004487EF;
    case 4: goto L_00448F0D;
    case 5: goto L_0044D2CB;

    case 6: // 0044DDD2: player / target names, experience bar, coins
        if (DG_INT(0x5f178) != 0 || DG_INT(0x5f17c) != 0)
            DG_INT(0xeb8) = 1;
        DG_STRCPY(f_5f0, 0x005596dc);
        if (DG_INT(0xc) == 0)
        {
            if (DG_SLOT(0x84068) != 0)
                *(int*)(EQ_GLOBAL(char*, 0x006e4de8) + 0x1ec) = 1;
        }
        if (DG_INT(0xc) != 0)
        {
            *(int*)(EQ_GLOBAL(char*, 0x006e4de8) + 0x1ec) = 1;
            DG_DRAW(EQ_GLOBAL(void*, 0x006b38b0), DG_DR(0x6c90c));
            DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4dc4) + 1));
            DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
            DG_TEXT(f_6bc, DG_RX(0x6c92c) + 5, 0xe, 10, DG_FONT);
            DG_SPRINTF(f_6bc, (const char*)0x0053e16c, (int)*(short*)(DG_PC + 0x40));
            DG_TEXT(f_6bc, 0x38, 0x181, 7, DG_FONT);
            DG_DRAW(EQ_GLOBAL(void*, 0x006b38b4), DG_DR(0x6c91c));
            DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4d8c) + 1));
            DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
            DG_TEXT(f_6bc, DG_RX(0x6c9cc) + 5, 0xe, 10, DG_FONT);
            DG_SPRINTF(f_6bc, (const char*)0x0053e16c, (int)*(unsigned char*)(DG_PC + 0x38));
            DG_TEXT(f_6bc, 0x253, 0x26, 7, DG_FONT);
            DG_EXP_PROGRESS();
        }
        else
        {
            if (DG_SLOT(0x8400c) != 0 || DG_SLOT(0x89dcc) != 0)
            {
                DG_SLOT(0x89dcc) = 0;
                f_6d4 = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xa8);
                f_6fc = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xa4);
                clipTop = DG_RX(0x6c90c);
                f_6d0 = f_6fc - clipTop;
                if (f_6d0 < 0)
                {
                    f_6d0 = 0;
                    f_6fc = clipTop;
                }
                f_6f8 = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xac) - clipTop;
                if (f_6f8 > DG_RX(0x6c914) - clipTop)
                    f_6f8 = DG_RX(0x6c914) - clipTop;
                clipTop = DG_RX(0x6c910);
                f_6f4 = f_6d4 - clipTop;
                if (f_6f4 < 0)
                {
                    f_6f4 = 0;
                    f_6d4 = clipTop;
                }
                clipBottom = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xb0) - clipTop;
                if (clipBottom > DG_RX(0x6c918) - clipTop)
                    clipBottom = DG_RX(0x6c918) - clipTop;
                f_6d0 += *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 4);
                if (f_6d0 > *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0xc))
                    f_6d0 = *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0xc);
                f_6f8 += *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 4);
                if (f_6f8 > *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0xc))
                    f_6f8 = *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0xc);
                clipTop = f_6f4 + *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 8);
                if (clipTop > *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0x10))
                    clipTop = *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0x10);
                clipBottom += *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 8);
                if (clipBottom > *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0x10))
                    clipBottom = *(int*)(EQ_GLOBAL(char*, 0x006b38b0) + 0x10);
                if (f_6f8 > f_6d0 && clipBottom > clipTop)
                {
                    setRect(f_6f0, f_6d0, clipTop, f_6f8, clipBottom);
                    setRect(f_6cc, f_6fc, f_6d4, f_6fc + 1, f_6d4 + 1);
                    DG_PART(DG_OVL(0x006e4e2c), f_6f0, f_6cc);
                }
                DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4dc4) + 1));
                DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
                DG_TEXT(f_6bc, DG_RX(0x6c92c) + 5, 0xe, 10, DG_FONT);
                DG_SPRINTF(f_6bc, (const char*)0x0053e16c, (int)*(short*)(DG_PC + 0x40));
                DG_TEXT(f_6bc, 0x38, 0x181, 7, DG_FONT);
            }
            if (DG_SLOT(0x84010) != 0 || DG_SLOT(0x89dd0) != 0)
            {
                DG_SLOT(0x89dd0) = 0;
                f_6d4 = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xa8);
                f_6fc = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xa4);
                clipTop = DG_RX(0x6c91c);
                f_6d0 = f_6fc - clipTop;
                if (f_6d0 < 0)
                {
                    f_6d0 = 0;
                    f_6fc = clipTop;
                }
                f_6f8 = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xac) - clipTop;
                if (f_6f8 > DG_RX(0x6c924) - clipTop)
                    f_6f8 = DG_RX(0x6c924) - clipTop;
                clipTop = DG_RX(0x6c920);
                f_6f4 = f_6d4 - clipTop;
                if (f_6f4 < 0)
                {
                    f_6f4 = 0;
                    f_6d4 = clipTop;
                }
                clipBottom = *(int*)(EQ_GLOBAL(char*, 0x006e4e48) + 0xb0) - clipTop;
                if (clipBottom > DG_RX(0x6c928) - clipTop)
                    clipBottom = DG_RX(0x6c928) - clipTop;
                f_6d0 += *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 4);
                if (f_6d0 > *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0xc))
                    f_6d0 = *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0xc);
                f_6f8 += *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 4);
                if (f_6f8 > *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0xc))
                    f_6f8 = *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0xc);
                clipTop = f_6f4 + *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 8);
                if (clipTop > *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0x10))
                    clipTop = *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0x10);
                clipBottom += *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 8);
                if (clipBottom > *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0x10))
                    clipBottom = *(int*)(EQ_GLOBAL(char*, 0x006b38b4) + 0x10);
                if (f_6f8 > f_6d0 && clipBottom > clipTop)
                {
                    setRect(f_6f0, f_6d0, clipTop, f_6f8, clipBottom);
                    setRect(f_6cc, f_6fc, f_6d4, f_6fc + 1, f_6d4 + 1);
                    DG_PART(DG_OVL(0x006e4e2c), f_6f0, f_6cc);
                }
                DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4d8c) + 1));
                DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
                DG_TEXT(f_6bc, DG_RX(0x6c9cc) + 5, 0xe, 10, DG_FONT);
                DG_SPRINTF(f_6bc, (const char*)0x0053e16c, (int)*(unsigned char*)(DG_PC + 0x38));
                DG_TEXT(f_6bc, 0x253, 0x26, 7, DG_FONT);
            }
        }
        if (DG_INT(0xc) != 0 || DG_SLOT(0x84040) != 0)
            DG_EXP_PROGRESS();
        if (DG_INT(0xc) != 0 || DG_SLOT(0x84060) != 0 || DG_INT(0x5f178) != 0 || DG_SLOT(0x89e20) != 0)
        {
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3640), DG_DR(0x6ca5c));
            DG_SLOT(0x89e20) = 0;
        }
        /* tests 0x89e20 again but clears 0x89e24 (original behaviour) */
        if (DG_INT(0xc) != 0 || DG_SLOT(0x84064) != 0 || DG_INT(0x5f17c) != 0 || DG_SLOT(0x89e20) != 0)
        {
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3640), DG_DR(0x6ca6c));
            DG_SLOT(0x89e24) = 0;
        }
        if (DG_SHORT(0x5f188) >= 0 && DG_SHORT(0x5f188) != DG_SHORT(0x5f18a))
        {
            switch (DG_SHORT(0x5f188))
            {
            case 8:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b38d0), DG_DR(0x6c98c));
                break;
            case 9:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b37c0), DG_DR(0x6c99c));
                break;
            case 10:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3638), DG_DR(0x6c9ac));
                break;
            case 11:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b363c), DG_DR(0x6c9bc));
                break;
            case 19:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3638), DG_DR(0x6ca3c));
                break;
            case 20:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b363c), DG_DR(0x6ca4c));
                break;
            }
        }
        else
        {
            switch (DG_SHORT(0x5f18a))
            {
            case 8:
                setRect(f_6f0, 0x198, 0x19d, 0x1fc, 0x1ae);
                DG_PART(DG_OVL(0x006e4e2c), f_6f0, DG_DR(0x6c98c));
                break;
            case 9:
                setRect(f_6f0, 0x1b6, 0x1bb, 0x1e4, 0x1cc);
                DG_PART(DG_OVL(0x006e4e2c), f_6f0, DG_DR(0x6c99c));
                break;
            case 10:
                setRect(f_6f0, 0x1ef, 0x21, 0x1f9, 0x32);
                DG_PART(DG_OVL(0x006e4e2c), f_6f0, DG_DR(0x6c9ac));
                break;
            case 11:
                setRect(f_6f0, 0x1ef, 0x154, 0x1f9, 0x165);
                DG_PART(DG_OVL(0x006e4e2c), f_6f0, DG_DR(0x6c9bc));
                break;
            case 19:
                setRect(f_6f0, 0x267, 0x8a, 0x271, 0x9b);
                DG_PART(DG_OVL(0x006e4e2c), f_6f0, DG_DR(0x6ca3c));
                break;
            case 20:
                setRect(f_6f0, 0x267, 0x1bd, 0x271, 0x1ce);
                DG_PART(DG_OVL(0x006e4e2c), f_6f0, DG_DR(0x6ca4c));
                break;
            }
        }
        EQ_METHOD(0, void, 0x004667f6, (void*), self)();
        EQ_METHOD(0, void, 0x0046776c, (void*), self)();
        setRect(f_6f0, 0xbc, 0x188, 0x1ea, 0x1c4);
        DG_PART(DG_OVL(0x006e4de8), f_6f0, (char*)self + 0x7336c);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, *(int*)(DG_PC + 0x998));
        DG_TEXT(f_6bc, DG_INT(0x72f3c) + 10, DG_INT(0x72f40) + 0x14, 10, DG_FONT);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, *(int*)(DG_PC + 0x99c));
        DG_TEXT(f_6bc, DG_INT(0x72f4c) + 10, DG_INT(0x72f50) + 0x14, 10, DG_FONT);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, *(int*)(DG_PC + 0x9a0));
        DG_TEXT(f_6bc, DG_INT(0x72f5c) + 10, DG_INT(0x72f60) + 0x14, 10, DG_FONT);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, *(int*)(DG_PC + 0x9a4));
        DG_TEXT(f_6bc, DG_INT(0x72f6c) + 10, DG_INT(0x72f70) + 0x14, 10, DG_FONT);
        return;

    case 8: // 0044E923: bug / feedback report
        setRect(f_6f0, 0, 0, 0x280, 0x1e0);
        DG_PART(DG_OVL(0x006e4e10), f_6f0, f_6f0);
        if (DG_INT(0xa8) == 0)
        {
            if (DG_SHORT(0x5f188) != 0 && DG_CHAR(0x8fc41) == 0)
            {
                setRect(f_6f0, 4, 10, 0x33, 0x1c);
                DG_PART(DG_OVL(0x006e4e40), f_6f0, DG_DR(0x6c90c));
                DG_TEXT(0x005596d4, DG_RX(0x6c90c) + 10, DG_RX(0x6c910), 0, DG_FONT2);
                DG_TEXT(0x005596c4, DG_RX(0x6c90c) + 5, DG_RX(0x6c910) + 7, 0, DG_FONT2);
            }
            else
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3674), DG_DR(0x6c90c));
                DG_TEXT(0x005596d0, DG_RX(0x6c90c) + 0xf, DG_RX(0x6c910), 10, DG_FONT2);
                DG_TEXT(0x005596c4, DG_RX(0x6c90c) + 5, DG_RX(0x6c910) + 7, 10, DG_FONT2);
            }
            if (DG_SHORT(0x5f188) != 1 && DG_CHAR(0x8fc42) == 0)
            {
                setRect(f_6f0, 4, 10, 0x33, 0x1c);
                DG_PART(DG_OVL(0x006e4e40), f_6f0, DG_DR(0x6c91c));
                DG_TEXT(0x005596b8, DG_RX(0x6c91c) + 5, DG_RX(0x6c920), 0, DG_FONT2);
                DG_TEXT(0x005596ac, DG_RX(0x6c91c) + 0xf, DG_RX(0x6c920) + 7, 0, DG_FONT2);
            }
            else
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3674), DG_DR(0x6c91c));
                DG_TEXT(0x005596b0, DG_RX(0x6c91c) + 10, DG_RX(0x6c920), 10, DG_FONT2);
                DG_TEXT(0x005596ac, DG_RX(0x6c91c) + 0xf, DG_RX(0x6c920) + 7, 10, DG_FONT2);
            }
        }
        if (DG_SHORT(0x5f188) == 2)
        {
            setRect(f_6f0, 0xde, 0xbe, 0x10d, 0xd0);
            DG_PART(DG_OVL(0x006e4de8), f_6f0, DG_DR(0x6c92c));
        }
        else
        {
            setRect(f_6f0, 6, 0x84, 0x35, 0x96);
            DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6c92c));
        }
        if (DG_SHORT(0x5f188) == 3)
        {
            setRect(f_6f0, 0xde, 0xab, 0x10d, 0xbd);
            DG_PART(DG_OVL(0x006e4de8), f_6f0, DG_DR(0x6c93c));
        }
        else
        {
            setRect(f_6f0, 0x16a, 0x84, 0x199, 0x96);
            DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6c93c));
        }
        EQ_GLOBAL(char, 0x006b6404) = 0;
        DG_SPRINTF(f_6bc, (const char*)0x005596a0, DG_PC);
        DG_TEXT(f_6bc, 0x1e, 0x177, 10, DG_FONT);
        if (DG_INT(0xa8) == 0)
            DG_TEXT(0x00559690, 0x109, 0x28, 10, DG_FONT4);
        else
            DG_TEXT(0x00559678, 0xf5, 0x28, 10, DG_FONT4);
        DG_STRCPY(f_400, (char*)self + 0x8fc43);
        wrapLen = (short)EQ_CDECL(int, 0x005223d0, (const char*))(f_400);
        wrapWidth = 0;
        wrapPos = 0;
        f_6fc = 0;
        for (; wrapPos < wrapLen && wrapLen > 0; wrapPos++)
        {
            if (wrapWidth > 0x21c)
            {
                wrapBreak = 0;
                for (wrapScan = wrapPos; wrapScan > wrapPos - 10; wrapScan--)
                {
                    if (wrapBreak == 0 && f_400[wrapScan] == ' ')
                        wrapBreak = wrapScan + 1;
                }
                if (wrapBreak == 0)
                    wrapBreak = wrapPos;
                DG_STRCPY(f_6bc, 0x0058fba4);
                EQ_CDECL(char*, 0x005231d0, (char*, const char*, int))(f_6bc, f_400, wrapBreak);
                f_6bc[wrapBreak] = 0;
                DG_STRCPY(f_400, f_400 + wrapBreak);
                wrapWidth = 0;
                wrapPos = 0;
                wrapLen = (short)EQ_CDECL(int, 0x005223d0, (const char*))(f_400);
                DG_TEXT(f_6bc, 0x28, (short)f_6fc * 0xf + 0x50, 10, DG_FONT);
                f_6fc++;
            }
            if (f_400[wrapPos] > 0)
                wrapWidth += EQ_GLOBAL(int*, 0x006efde0)[f_400[wrapPos]];
        }
        if (EQ_CDECL(unsigned int, 0x005223d0, (const char*))(f_400) == 0)
            return;
        DG_TEXT(f_400, 0x28, (short)f_6fc * 0xf + 0x50, 10, DG_FONT);
        return;

    case 9: // 0044EEAB: message board list
        setRect(f_6f0, 0, 0, 0x280, 0x1e0);
        DG_PART(DG_OVL(0x006e4e04), f_6f0, f_6f0);
        setRect(f_6f0, 0xe7, 10, 0x1c6, 0x25);
        setRect(f_6cc, 0x7b, 0, 0x15b, 0x1c);
        DG_PART(DG_OVL(0x006e4e00), f_6cc, f_6f0);
        DG_TEXT(0x00559668, 0x10a, 0xf, 10, DG_FONT5);
        if (DG_SHORT(0x5f188) != 0 && DG_SHORT(0xdac) != 0)
            setRect(f_6f0, 0x67, 0x74, 0xcc, 0x86);
        else
            setRect(f_6f0, 1, 0x74, 0x66, 0x86);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c90c));
        if (DG_SHORT(0x5f188) != 1 && DG_SHORT(0xdac) != 1)
            setRect(f_6f0, 0x67, 0x87, 0xcc, 0x99);
        else
            setRect(f_6f0, 1, 0x87, 0x66, 0x99);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c91c));
        if (DG_SHORT(0x5f188) != 2 && DG_SHORT(0xdac) != 2)
            setRect(f_6f0, 0x67, 0x9a, 0xcc, 0xac);
        else
            setRect(f_6f0, 1, 0x9a, 0x66, 0xac);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c92c));
        if (DG_SHORT(0x5f188) != 3 && DG_SHORT(0xdac) != 3)
            setRect(f_6f0, 0x67, 0xad, 0xcc, 0xbf);
        else
            setRect(f_6f0, 1, 0xad, 0x66, 0xbf);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c93c));
        if (DG_SHORT(0x5f188) == 4)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3638), DG_DR(0x6c94c));
        else
            DG_PART(DG_OVL(0x006e4e00), DG_DR(0x6c94c), DG_DR(0x6c94c));
        if (DG_SHORT(0x5f188) == 5)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b363c), DG_DR(0x6c95c));
        else
            DG_PART(DG_OVL(0x006e4e00), DG_DR(0x6c95c), DG_DR(0x6c95c));
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3640), DG_DR(0x6c96c));
        if (DG_SHORT(0x5f188) == 7)
            setRect(f_6f0, 1, 0, 0x3d, 0x1c);
        else
            setRect(f_6f0, 0x3e, 0, 0x7a, 0x1c);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c97c));
        if (DG_CHAR(0xdae) != 0)
        {
            if (DG_SHORT(0x5f188) == 8)
                setRect(f_6f0, 1, 0x1d, 0x3d, 0x39);
            else
                setRect(f_6f0, 0x7b, 0x1d, 0xb7, 0x39);
        }
        else
            setRect(f_6f0, 0x3e, 0x1d, 0x7a, 0x39);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c98c));
        if (DG_SHORT(0x5f188) != 9 && DG_SHORT(0xdaa) != 9)
            setRect(f_6f0, 0x67, 0xc0, 0xcc, 0xd2);
        else
            setRect(f_6f0, 1, 0xc0, 0x66, 0xd2);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c99c));
        if (DG_SHORT(0x5f188) != 10 && DG_SHORT(0xdaa) != 10)
            setRect(f_6f0, 0x67, 0xf9, 0xcc, 0x10b);
        else
            setRect(f_6f0, 1, 0xf9, 0x66, 0x10b);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c9ac));
        if (DG_SHORT(0x5f188) != 11 && DG_SHORT(0xdaa) != 11)
            setRect(f_6f0, 0x67, 0xd3, 0xcc, 0xe5);
        else
            setRect(f_6f0, 1, 0xd3, 0x66, 0xe5);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c9bc));
        if (DG_SHORT(0x5f188) != 12 && DG_SHORT(0xdaa) != 12)
            setRect(f_6f0, 0x67, 0x10c, 0xcc, 0x11e);
        else
            setRect(f_6f0, 1, 0x10c, 0x66, 0x11e);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c9cc));
        if (DG_SHORT(0x5f188) != 13 && DG_SHORT(0xdaa) != 13)
            setRect(f_6f0, 0x67, 0xe6, 0xcc, 0xf8);
        else
            setRect(f_6f0, 1, 0xe6, 0x66, 0xf8);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c9dc));
        if (DG_SHORT(0x5f188) != 14 && DG_SHORT(0xdaa) != 14)
            setRect(f_6f0, 0x67, 0x11f, 0xcc, 0x131);
        else
            setRect(f_6f0, 1, 0x11f, 0x66, 0x131);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c9ec));
        if (DG_SHORT(0x5f188) == 15)
            setRect(f_6f0, 1, 0x57, 0x3d, 0x73);
        else
            setRect(f_6f0, 0x3e, 0x57, 0x7a, 0x73);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c9fc));
        if (DG_CHAR(0x100) != 0)
        {
            DG_TEXT(0x0053ea08, 0x41, DG_RX(0x6ca10) + 5, 0, DG_FONT);
            return;
        }
        for (f_6f8 = 0; (short)f_6f8 < 0x12; f_6f8++)
        {
            if (DG_MSG(f_6f8) != 0)
            {
                DG_STRCPY(f_6bc, 0x0058fba4);
                DG_MSGDATE(DG_MSG(f_6f8))
                if (DG_SHORT(0xfc) == (short)f_6f8)
                    f_6fc = 4;
                else
                    f_6fc = 0;
                DG_TEXT(f_6bc, 0x41, DG_INT((DG_CHAR(0xfe0) * 400 + f_6f8) * 0x10 + 0x6ca10) + 5, f_6fc, DG_FONT);
                DG_TEXT(DG_MSG(f_6f8) + 0x1c, 0xa0, DG_INT((DG_CHAR(0xfe0) * 400 + f_6f8) * 0x10 + 0x6ca10) + 5,
                        f_6fc, DG_FONT);
                DG_TEXT(DG_LANG(*(unsigned char*)(DG_MSG(f_6f8) + 0x3a)), 0xe6,
                        DG_INT((DG_CHAR(0xfe0) * 400 + f_6f8) * 0x10 + 0x6ca10) + 5, f_6fc, DG_FONT);
                DG_TEXT(DG_MSG(f_6f8) + 0x3b, 0x14a, DG_INT((DG_CHAR(0xfe0) * 400 + f_6f8) * 0x10 + 0x6ca10) + 5,
                        f_6fc, DG_FONT);
            }
        }
        return;

    case 10: // 0044F6E6: message board, read a message
        setRect(f_6f0, 0, 0, 0x280, 0x1e0);
        DG_PART(DG_OVL(0x006e4e04), f_6f0, f_6f0);
        if (DG_SHORT(0x5f188) == 0)
            setRect(f_6f0, 0xf5, 0x1d, 0x131, 0x39);
        else
            setRect(f_6f0, 0xb8, 0x1d, 0xf4, 0x39);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c90c));
        if (DG_CHAR(0xdae) != 0)
        {
            if (DG_SHORT(0x5f188) == 1)
                setRect(f_6f0, 1, 0x1d, 0x3d, 0x39);
            else
                setRect(f_6f0, 0x7b, 0x1d, 0xb7, 0x39);
        }
        else
            setRect(f_6f0, 0x3e, 0x1d, 0x7a, 0x39);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c91c));
        if (DG_SHORT(0x5f188) == 2)
            setRect(f_6f0, 1, 0x3a, 0x3d, 0x56);
        else
            setRect(f_6f0, 0x3e, 0x3a, 0x7a, 0x56);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c92c));
        if (DG_MSG(DG_SHORT(0xfc)) == 0)
            return;
        DG_STRCPY(f_6bc, 0x0058fba4);
        DG_MSGDATE(DG_MSG(DG_SHORT(0xfc)))
        DG_TEXT(f_6bc, 0x4b, 0x5a, 0, DG_FONT);
        DG_TEXT(DG_MSG(DG_SHORT(0xfc)) + 0x3b, 0x4b, 0x78, 0, DG_FONT);
        EQ_METHOD(0, void, 0x0047d4e5, (void*), self)();
        DG_TEXT(DG_MSG(DG_SHORT(0xfc)) + 0x1c, 0x12c, 0x14f, 0, DG_FONT);
        return;

    case 11: // 0044F9A2: message board, write a message
        setRect(f_6f0, 0, 0, 0x280, 0x1e0);
        DG_PART(DG_OVL(0x006e4e04), f_6f0, f_6f0);
        if (DG_SHORT(0x5f188) == 0)
            setRect(f_6f0, 1, 0, 0x3d, 0x1c);
        else
            setRect(f_6f0, 0x3e, 0, 0x7a, 0x1c);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c90c));
        if (DG_CHAR(0xdae) != 0)
        {
            if (DG_SHORT(0x5f188) == 1)
                setRect(f_6f0, 1, 0x1d, 0x3d, 0x39);
            else
                setRect(f_6f0, 0x7b, 0x1d, 0xb7, 0x39);
        }
        else
            setRect(f_6f0, 0x3e, 0x1d, 0x7a, 0x39);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c91c));
        if (DG_SHORT(0x5f188) == 2)
            setRect(f_6f0, 1, 0x9a, 0x66, 0xac);
        else
            setRect(f_6f0, 0x67, 0x9a, 0xcc, 0xac);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c92c));
        DG_STRCPY(f_6bc, DG_LANG(DG_SHORT(0x6c65e + DG_SHORT(0xf8) * 2)));
        f_6fc = 0;
        wrapLen = (short)EQ_CDECL(int, 0x005223d0, (const char*))(f_6bc);
        for (wrapPos = 0; wrapPos < wrapLen; wrapPos++)
        {
            if (f_6bc[wrapPos] > 0)
                f_6fc += *(unsigned short*)(EQ_GLOBAL(char*, 0x006efdec) + f_6bc[wrapPos] * 4);
        }
        textX = (short)(0x14c - ((short)f_6fc >> 1));
        if (textX < 0)
            textX = 0;
        setRect(f_6f0, 0xe7, 0x1b1, 0x280, 0x1e0);
        setRect(f_6cc, 0x7b, 0, 0x15b, 0x1c);
        DG_PART(DG_OVL(0x006e4e00), f_6cc, f_6f0);
        DG_TEXT(f_6bc, textX, 0x1b6, 10, DG_FONT5);
        if (DG_SHORT(0x5f188) == 3)
            setRect(f_6f0, 1, 0x3a, 0x3d, 0x56);
        else
            setRect(f_6f0, 0x3e, 0x3a, 0x7a, 0x56);
        DG_PART(DG_OVL(0x006e4e00), f_6f0, DG_DR(0x6c93c));
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, (int)*(short*)(EQ_GLOBAL(char*, 0x006e4d54) + 4));
        DG_STRCPY(f_480, f_6bc + 2);
        DG_SPRINTF(f_6bc, (const char*)0x0055964c, EQ_GLOBAL(unsigned char*, 0x006e4d54)[3],
                   EQ_GLOBAL(unsigned char*, 0x006e4d54)[2], f_480, EQ_GLOBAL(unsigned char*, 0x006e4d54)[0],
                   EQ_GLOBAL(unsigned char*, 0x006e4d54)[1]);
        DG_TEXT(f_6bc, 0x4b, 0x5a, 0, DG_FONT);
        DG_STRCPY(f_6bc, (char*)self + 0x188);
        EQ_CDECL(char*, 0x005219d0, (char*, const char*))(f_6bc, (const char*)0x005459a4);
        DG_TEXT(f_6bc, 0x4b, 0x78, DG_CHAR(0xfe) != 0 ? 4 : 0, DG_FONT);
        EQ_METHOD(0, void, 0x0047d4e5, (void*), self)();
        DG_TEXT(DG_PC, 0x12c, 0x14f, 0, DG_FONT);
        return;

    case 12: // 0044FCE2: help
        helpEnd = DG_SHORT(0x1434) + 0x12;
        if (helpEnd > DG_INT(0x1430))
            helpEnd = (short)DG_INT(0x1430);
        setRect(f_6f0, 0, 0, 0x280, 0x1e0);
        DG_PART(DG_OVL(0x006e4e10), f_6f0, f_6f0);
        DG_TEXT(0x00559644, 0x118, 0x1e, 10, DG_FONT5);
        f_6e0 = 0;
        for (helpLine = DG_SHORT(0x1434); helpLine < helpEnd; helpLine++)
        {
            DG_TEXT((char*)self + 0x1438 + helpLine * 0x80, 0x28, (short)f_6e0 * 0xf + 0x46, 10, DG_FONT2);
            f_6e0++;
        }
        setRect(f_6f0, 0x9d, 0x1c3, 0xcc, 0x1d5);
        DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6c90c));
        if (DG_SHORT(0x5f188) == 0)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b37c0), DG_DR(0x6c90c));
        break;
    }
L_0044FE15:
L_0044FE1A:
    return;
    // END REGION P01

    // REGION P02 0x0043B077-0x0043DCFC: 11397 B, arm of JZ @0043B071 (not taken): CMP dword ptr [DAT_0058fc6c],EBX
    //   defines L_0043B077; exits L_0043DCFC; falls into P03
    //   switch scaffold for this piece (paste where the dispatch is):
    //   // SWITCH @0043D928 (in P02): table 0x0044FE25, 9 entries (cmp), bias -227; selector: MOVSX EAX,AX ; ADD EAX,0xffffff1d ; CMP EAX,0x8 ; JA default ; JMP dword ptr [EAX*0x4 + switchdataD_0044fe25]
    //   switch (/* TODO selector (register EAX) */ 0) {
    //   case 0xE3: goto L_0043D92F; /* P02 */
    //   case 0xE4: goto L_0043D942; /* P02 */
    //   case 0xE5: goto L_0043D955; /* P02 */
    //   case 0xE6: goto L_0043D968; /* P02 */
    //   case 0xE7: goto L_0043D97B; /* P02 */
    //   case 0xE8: goto L_0043D9A2; /* P02 */
    //   case 0xE9: goto L_0043D9B5; /* P02 */
    //   case 0xEA: goto L_0043D9C5; /* P02 */
    //   case 0xEB: goto L_0043D9EB; /* P02 */
    //   default: goto L_0043DA3F; /* P02 */
    //   }
    // f_6c8 holds what the asm keeps in EBP (a rect origin, a loop index); f_6f4 the text width.
#define P02_GEM ((int)DG_SHORT((DG_CHAR(0x6841d) * 10 + f_6c8) * 4 + 0x68420))
#define P02_GEMI(off) DG_INT((DG_CHAR(0xfe0) * 400 + f_6c8) * 4 + (off))
#define P02_RECT(off) ((DG_CHAR(0xfe0) * 400 + f_6c8) * 0x10 + DG_THIS + (off))
#define P02_RX(off) DG_INT((DG_CHAR(0xfe0) * 400 + f_6c8) * 0x10 + (off))
#define P02_TEXT16(text, x, y, color, font) \
    EQ_METHOD(6, void, 0x0048d21a, (void*, char*, int, int, short, int, int), EQ_GLOBAL(void*, 0x006e4e68)) \
        ((char*)(text), (x), (y), (short)(color), (font), 1)
#define P02_DRAWRET(img, rect) \
    EQ_METHOD(2, int, 0x004384b2, (void*, void*, int), (void*)(img))((void*)(rect), 1)
L_0043B077:
    DG_INT(0xc) = 1;
    EQ_METHOD(0, void, 0x0046835a, (void*), self)();
    DG_STRCPY(f_5f0, 0x0055a070);
    setRect = DG_SETRECT;
    DG_INT(0x900d4) = 0;
    if (DG_INT(0x900d8) != 0)
    {
        DG_INT(0x900d4) = 0;
        f_6fc = DG_INT(0x900c4);
        f_6c8 = DG_INT(0x900c8);
        setRect(f_5b0, f_6fc, f_6c8, DG_INT(0x900cc) + f_6fc, DG_INT(0x900d0) + f_6c8);
        f_6f4 = f_6fc + 0x2c;
        setRect(f_520, f_6fc + 4, f_6c8 + 0x10, f_6f4, f_6c8 + 0x38);
        setRect(f_510, f_6fc + 0x30, f_6c8 + 0x10, f_6fc + 0x58, f_6c8 + 0x38);
        setRect(f_500, f_6fc + 4, f_6c8 + 0x3c, f_6f4, f_6c8 + 0x64);
        setRect(f_4f0, f_6fc + 0x30, f_6c8 + 0x3c, f_6fc + 0x58, f_6c8 + 0x64);
        setRect(f_4e0, f_6fc + 4, f_6c8 + 0x68, f_6f4, f_6c8 + 0x90);
        setRect(f_4d0, f_6fc + 0x30, f_6c8 + 0x68, f_6fc + 0x58, f_6c8 + 0x90);
        setRect(f_4c0, f_6fc + 4, f_6c8 + 0x94, f_6f4, f_6c8 + 0xbc);
        setRect(f_4b0, f_6fc + 0x30, f_6c8 + 0x94, f_6fc + 0x58, f_6c8 + 0xbc);
        setRect(f_4a0, f_6fc + 4, f_6c8 + 0xc0, f_6f4, f_6c8 + 0xe8);
        setRect(f_490, f_6fc + 0x30, f_6c8 + 0xc0, f_6fc + 0x58, f_6c8 + 0xe8);
        setRect(f_6f0, 0x120, 0xf4, 0x130, 0x100);
        setRect(f_6cc, f_6fc + 0x16, f_6c8 + 1, f_6fc + 0x17, f_6c8 + 2);
        EQ_METHOD(3, void, 0x004500be, (void*, void*, int*, int*), self)(
            *(void**)(EQ_GLOBAL(char*, 0x006e4e1c) + 4), f_6f0, f_6cc);
        setRect(f_6f0, 0x131, 0xf4, 0x141, 0x100);
        f_6dc = (short*)(f_6fc + 0x36);
        setRect(f_6cc, (int)f_6dc, f_6c8 + 1, f_6fc + 0x37, f_6c8 + 2);
        EQ_METHOD(3, void, 0x004500be, (void*, void*, int*, int*), self)(
            *(void**)(EQ_GLOBAL(char*, 0x006e4e1c) + 4), f_6f0, f_6cc);
        // the original walks the ten slot rects as one array (f_6e0 = pointer, f_6d8 = 10 down to 0); the
        // skeleton declares them separately and VC6 doesn't lay them out contiguously, so name each one
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_520);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_510);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_500);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_4f0);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_4e0);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_4d0);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_4c0);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_4b0);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_4a0);
        EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)(f_490);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, DG_CHAR(0x6841d) + 1);
        if (DG_CHAR(0x6841d) < 9)
            setRect(f_6f0, f_6f4, f_6c8, (int)f_6dc, f_6c8 + 9);
        else
            setRect(f_6f0, f_6fc + 0x26, f_6c8, f_6fc + 0x3a, f_6c8 + 9);
        DG_TEXT(f_6bc, f_6f0[0], f_6f0[1], 10, DG_FONT);
        f_6c8 = 0;
        f_6e0 = 10;
        do
        {
            if (DG_CHAR((DG_CHAR(0x6841d) * 10 + f_6c8) * 4 + 0x6841e) == 7)
            {
                if (DG_INT(0xc) != 0 || P02_GEMI(0x8432c) != 0 || P02_GEMI(0x8a0ec) != 0
                    || DG_SHORT(P02_GEM * 2 + 0x6c594) != DG_PCSPELL(P02_GEM) || DG_INT(P02_GEM * 4 + 0xe60) != 0)
                {
                    setRect(f_6f0, 0x51, 0x2d, 0x78, 0x54);
                    DG_UPV(f_6f0, P02_RECT(0x6d58c));
                    if (DG_PCSPELL(P02_GEM) != -1)
                    {
                        setRect(f_6f0, P02_RX(0x6d58c) + 7, P02_RX(0x6d590) + 9, 0x280, 0x1e0);
                        if (DG_UINT(P02_GEM * 4 + 0xe60) > 0 && DG_UINT(P02_GEM * 4 + 0xe60) < 8)
                        {
                            DG_GEM_DRAW(DG_UINT(P02_GEM * 4 + 0xe60), f_6f0)
                        }
                        else
                            DG_DRAWIMAGE(f_6f0, *(short*)(EQ_ARRAY(char*, 0x005fe9a0)[DG_PCSPELL(P02_GEM)] + 0x1fc));
                    }
                }
                DG_SHORT(P02_GEM * 2 + 0x6c594) = DG_PCSPELL(P02_GEM);
            }
            f_6c8++;
        } while (--f_6e0 != 0);
        f_6c8 = 0;
        f_6e0 = 10;
        do
        {
            P02_GEMI(0x8a0ec) = 0;
            if (DG_CHAR(0x6841d) > 9)
                DG_CHAR(0x6841d) = 0;
            EQ_METHOD(1, void, 0x0047bc1f, (void*, int), self)(f_6c8);
            f_6c8++;
        } while (--f_6e0 != 0);
    }

    // 0043B5B6
    DG_STRCPY(f_5f0, 0x0055a064);
    if (DG_PC[0x36] != 1 && DG_PC[0x36] != 7 && DG_PC[0x36] != 9)
    {
        DG_INT(0x900b4) = 0;
        if (DG_INT(0x900b8) != 0)
        {
            DG_INT(0x900b4) = 0;
            setRect(f_5b0, DG_INT(0x900a4), DG_INT(0x900a8), DG_INT(0x900ac) + DG_INT(0x900a4),
                    DG_INT(0x900b0) + DG_INT(0x900a8));
            setRect(f_6cc, 0x120, 0x101, 0x13e, 0x11a);
            f_6c8 = 0xd4;
            f_6e0 = 8;
            do
            {
                g_CopyRect_guess(f_6f0, (int*)P02_RECT(0x6c90c));
                f_6f0[1]--;
                f_6f0[0]++;
                DG_PART(*(void**)(EQ_GLOBAL(char*, 0x006e4e1c) + 4), f_6cc, f_6f0);
                f_6c8++;
            } while (--f_6e0 != 0);
            f_6c8 = 0;
            f_6d8 = (short*)0x952;
            f_6e0 = DG_THIS + 0xe60;
            f_6fc = 8;
            do
            {
                if (*(short*)((int)f_6d8 + DG_PC) != -1)
                {
                    if (*(unsigned int*)f_6e0 > 0 && *(unsigned int*)f_6e0 < 8)
                    {
                        DG_GEM_DRAW(*(unsigned int*)f_6e0, P02_RECT(0x6d64c))
                    }
                    else
                        DG_DRAWIMAGE(P02_RECT(0x6d64c),
                                     *(short*)(EQ_ARRAY(char*, 0x005fe9a0)[*(short*)((int)f_6d8 + DG_PC)] + 0x1fc));
                }
                f_6e0 += 4;
                f_6d8 = (short*)((int)f_6d8 + 2);
                f_6c8++;
            } while (--f_6fc != 0);
        }
    }

    // 0043B7F2: player name box
    DG_INT(0x90054) = 0;
    if (DG_INT(0x90058) != 0)
    {
        DG_INT(0x90054) = 0;
        f_6c8 = DG_INT(0x90048);
        f_6dc = (short*)(DG_INT(0x90044) + 9);
        setRect(f_6f0, (int)f_6dc, f_6c8 + 5, EQ_GLOBAL(int, 0x006b63fc), EQ_GLOBAL(int, 0x006b6400));
        setRect(f_6cc, (int)f_6dc, f_6c8 + 0x11, EQ_GLOBAL(int, 0x006b63fc), EQ_GLOBAL(int, 0x006b6400));
        DG_STRCPY(f_6bc, DG_TRUNC(EQ_GLOBAL(char*, 0x006e4d8c) + 1, 0x4b));
        if (EQ_GLOBAL(char*, 0x006e4dac) != 0 && EQ_GLOBAL(char*, 0x006e4dac) == EQ_GLOBAL(char*, 0x006e4d8c))
        {
            DG_TEXT(f_6bc, f_6f0[0], f_6f0[1], 10, DG_FONT);
            DG_STRCPY(f_6bc, DG_TRUNC(DG_PC + 0x1e, 0x4b));
            DG_TEXT(f_6bc, f_6cc[0], f_6cc[1], 10, DG_FONT);
        }
        else if (EQ_CDECL(int, 0x00522920, (const char*, const char*))(DG_PC, (const char*)0x005fe968) == 0)
        {
            DG_TEXT(f_6bc, f_6f0[0], f_6f0[1], 0xf, DG_FONT);
            DG_STRCPY(f_6bc, DG_TRUNC(DG_PC + 0x1e, 0x4b));
            DG_TEXT(f_6bc, f_6cc[0], f_6cc[1], 0xf, DG_FONT);
        }
        else
        {
            DG_TEXT(f_6bc, f_6f0[0], f_6f0[1], 7, DG_FONT);
            DG_STRCPY(f_6bc, DG_TRUNC(DG_PC + 0x1e, 0x4b));
            DG_TEXT(f_6bc, f_6cc[0], f_6cc[1], 7, DG_FONT);
        }
    }

    // 0043B991: vials, target box
    EQ_METHOD(0, void, 0x00468df1, (void*), self)();
    DG_INT(0x90094) = 0;
    if (DG_INT(0x90098) != 0)
    {
        DG_INT(0x90094) = 0;
        f_6c8 = DG_INT(0x90084);
        f_6dc = (short*)DG_INT(0x90088);
        setRect(f_6f0, f_6c8 + 0x24, (int)f_6dc + 1, EQ_GLOBAL(int, 0x006b63fc), EQ_GLOBAL(int, 0x006b6400));
        DG_TEXT(0x0055a05c, f_6f0[0], f_6f0[1], 10, DG_FONT);
        if (EQ_GLOBAL(char*, 0x006e4dac) != 0)
        {
            DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4dac) + 1));
            DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x60));
            setRect(f_6f0, f_6c8 + 0xa, (int)f_6dc + 0x12, EQ_GLOBAL(int, 0x006b63fc), EQ_GLOBAL(int, 0x006b6400));
            DG_TEXT(f_6bc, f_6f0[0], f_6f0[1], 10, DG_FONT);
        }
    }

    // 0043BA7D
    switch (DG_CHAR(0xfe0))
    {
    case 4:
        if (EQ_GLOBAL(int, 0x0058fc6c) != 2 || *(char*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xfe0) == 4)
            goto L_0043D81D;
        goto L_0043DCFC;

    case 3: // 0043BAC9: inventory
        DG_INT(0x90254) = 0;
        if (DG_INT(0x90258) != 0)
        {
            if (DG_SLOT(0x84398) != 0 || DG_SLOT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3644), DG_DR(0x6d73c));
                DG_SLOT(0x8a158) = 0;
            }
            if (DG_SLOT(0x8439c) != 0 || DG_SLOT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3648), DG_DR(0x6d74c));
                DG_SLOT(0x8a15c) = 0;
            }
            if (DG_SLOT(0x843a0) != 0 || DG_SLOT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b362c), DG_DR(0x6d75c));
                DG_SLOT(0x8a160) = 0;
            }
            if (DG_INT(0xc) != 0 || DG_SLOT(0x8a180) != 0 || DG_SLOT(0x843c0) != 0)
            {
                for (f_6c8 = 0; f_6c8 < 12; f_6c8++)
                    P02_GEMI(0x89eb0) = 1;
            }
            if (DG_SHORT(0x5f188) >= 0)
            {
                switch (DG_SHORT(0x5f188))
                {
                case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e:
                case 0x3f: case 0x40: case 0x41: case 0x42: case 0x43: case 0x44:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3674),
                            (DG_CHAR(0xfe0) * 400 + DG_SHORT(0x5f188)) * 0x10 + DG_THIS + 0x6c90c);
                    break;
                case 0x4e:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b38a8), DG_DR(0x6cdec));
                    break;
                case 0x4f:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b38ac), DG_DR(0x6cdfc));
                    break;
                case 0xe3:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3668), DG_DR(0x6d73c));
                    break;
                case 0xe4:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b366c), DG_DR(0x6d74c));
                    break;
                case 0xe5:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3670), DG_DR(0x6d75c));
                    break;
                }
            }
            f_6c8 = 0;
            f_6d8 = (short*)12;
            do
            {
                if (DG_INT(0xc) != 0 || P02_GEMI(0x840f0) != 0 || DG_SHORT(0x5f18a) == f_6c8 + 0x39
                    || P02_GEMI(0x89eb0) != 0)
                {
                    if (DG_SHORT(0x5f188) != f_6c8 + 0x39)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b3848), P02_RECT(0x6cc9c));
                    P02_GEMI(0x89eb0) = 0;
                    f_6e0 = (short)EQ_CDECL(int, 0x005223d0, (const char*))(
                        (char*)self + (DG_CHAR(0x6841c) * 12 + f_6c8) * 0x136 + 0x5f196);
                    f_6f4 = 0;
                    f_6fc = 0;
                    if ((short)f_6e0 > 0)
                    {
                        f_6fc = f_6e0;
                        f_6dc = (short*)((char*)self + (DG_CHAR(0x6841c) * 12 + f_6c8) * 0x136 + 0x5f196);
                        do
                        {
                            if (*(char*)f_6dc > 0)
                                f_6f4 += *(unsigned short*)(EQ_GLOBAL(char*, 0x006efddc) + *(char*)f_6dc * 4);
                            f_6dc = (short*)((char*)f_6dc + 1);
                        } while (--f_6e0 != 0);
                    }
                    f_6f4 = (short)f_6f4 >> 1;
                    if ((short)f_6f4 > 0x14)
                        f_6f4 = 0x14;
                    P02_TEXT16((char*)self + (DG_CHAR(0x6841c) * 12 + f_6c8) * 0x136 + 0x5f196,
                               P02_RX(0x6cc9c) - (short)f_6f4 + 0x17, P02_RX(0x6cca0) + 5,
                               DG_CHAR((DG_CHAR(0x6841c) * 12 + f_6c8) * 0x136 + 0x5f2cb), DG_FONT2);
                }
                f_6c8++;
                f_6d8 = (short*)((int)f_6d8 - 1);
            } while (f_6d8 != 0);
            if (DG_SHORT(0x5f188) != 0x4e)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b38ec), DG_DR(0x6cdec));
            if (DG_SHORT(0x5f188) != 0x4f)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b38f0), DG_DR(0x6cdfc));
            DG_SPRINTF(f_6bc, (const char*)0x0053e16c, DG_CHAR(0x6841c) + 1);
            if (DG_CHAR(0x6841c) < 9)
                f_6e0 = DG_RX(0x6cdec) + 0x1b;
            else
                f_6e0 = DG_RX(0x6cdec) + 0x18;
            f_6c8 = DG_RY(0x6cdec) - 1;
            setRect(f_6f0, DG_RX(0x6cdec) + 0x14, f_6c8 - 1, DG_RX(0x6cdec) + 0x15, f_6c8);
            DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), f_6f0);
            DG_TEXT(f_6bc, f_6e0, f_6c8, 10, DG_FONT);
        }
        // 0043BFD4
        if (DG_INT(0x5f18c) != 0)
        {
            f_6f4 = EQ_GLOBAL(int, 0x006b63fc) / 2 - 0xd0;
            f_6c8 = EQ_GLOBAL(int, 0x006b6400) / 2 - 0x50;
            setRect(f_6f0, f_6f4, f_6c8, f_6f4 + 10, f_6c8 + 10);
            DG_DRAW(EQ_GLOBAL(void*, 0x006b38e8), f_6f0);
            switch (DG_CHAR(0x5f195))
            {
            case 0: DG_TEXT(0x0055a058, f_6f4 + 0xb, f_6c8 + 0x23, 10, DG_FONT); break;
            case 1: DG_TEXT(0x0055a058, f_6f4 + 0xb, f_6c8 + 0x37, 10, DG_FONT); break;
            case 2: DG_TEXT(0x0055a058, f_6f4 + 0xb, f_6c8 + 0x4b, 10, DG_FONT); break;
            case 3: DG_TEXT(0x0055a058, f_6f4 + 0xb, f_6c8 + 0x5f, 10, DG_FONT); break;
            case 4: DG_TEXT(0x0055a058, f_6f4 + 0xb, f_6c8 + 0x73, 10, DG_FONT); break;
            case 5: DG_TEXT(0x0055a058, f_6f4 + 0x63, f_6c8 + 0xb, 10, DG_FONT); break;
            }
            DG_TEXT((char*)self + 0x682e6, f_6f4 + 0x68, f_6c8 + 0xb, 10, DG_FONT);
            P02_TEXT16(0x0055a054, f_6f4 + 0x130, f_6c8 + 0xb, DG_CHAR(0x6841b), DG_FONT);
            f_6f4 += 0x15;
            DG_TEXT((char*)self + 0x682ef, f_6f4, f_6c8 + 0x21, 10, DG_FONT);
            DG_TEXT((char*)self + 0x6832b, f_6f4, f_6c8 + 0x35, 10, DG_FONT);
            DG_TEXT((char*)self + 0x68367, f_6f4, f_6c8 + 0x49, 10, DG_FONT);
            DG_TEXT((char*)self + 0x683a3, f_6f4, f_6c8 + 0x5d, 10, DG_FONT);
            DG_TEXT((char*)self + 0x683df, f_6f4, f_6c8 + 0x71, 10, DG_FONT);
        }
        // 0043C1C1
        if (DG_SHORT(0x5f188) == 0x4c)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b38a4), DG_DR(0x6cdcc));
        if (DG_SHORT(0x5f188) != 0x4d)
            goto L_0043DCFC;
        carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b37c0), DG_DR(0x6cddc));
        goto L_0043DCFC;

    case 2: // 0043C21E
        DG_INT(0x90254) = 0;
        if (DG_INT(0x90258) != 0)
        {
            if (DG_SLOT(0x84398) != 0 || DG_SLOT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3644), DG_DR(0x6d73c));
                DG_SLOT(0x8a158) = 0;
            }
            if (DG_SLOT(0x8439c) != 0 || DG_SLOT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b362c), DG_DR(0x6d74c));
                DG_SLOT(0x8a15c) = 0;
            }
            if (DG_SLOT(0x843a0) != 0 || DG_SLOT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3630), DG_DR(0x6d75c));
                DG_SLOT(0x8a160) = 0;
            }
            if (DG_INT(0xc) != 0 || DG_SLOT(0x8a180) != 0 || (char*)self + 0xe094c != 0)
            {
                for (f_6c8 = 0; f_6c8 < 6; f_6c8++)
                    P02_GEMI(0x8a1a4) = 1;
            }
        }
        // 0043C393
        if (DG_INT(0x685d4) != 0)
        {
            f_6f8 = 0;
            if (DG_INT(0x685b8) > 0)
            {
                f_6c8 = 0;
                do
                {
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b36c4), P02_RECT(0x6dd5c));
                    setRect(f_6f0, P02_RX(0x6dd5c) + 0xf, P02_RX(0x6dd60) + 0xc, 0x280, 0x1e0);
                    f_6e0 = DG_SHORT(f_6c8 * 2 + 0x6c65e);
                    if ((short)f_6e0 > 0x64)
                    {
                        switch ((short)f_6e0)
                        {
                        case 0x69: DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), f_6f0); break;
                        case 0x6b: DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), f_6f0); break;
                        case 0x6f: DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), f_6f0); break;
                        }
                    }
                    else
                        DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8a)), f_6f0);
                    f_6f8++;
                    f_6c8 = (short)f_6f8;
                } while (f_6c8 < DG_INT(0x685b8));
            }
            if (DG_INT(0x685b8) != 0)
            {
                setRect(f_6f0, 0x72, 0xd0, 0x8a, 0x112);
                setRect(f_6cc, DG_RX(0x6df3c), DG_RX(0x6df48), 0x280, 0x1e0);
                DG_PART(*(void**)(EQ_GLOBAL(char*, 0x006e4e40) + 4), f_6f0, f_6cc);
                if (DG_SHORT(0x5f188) == 0x163)
                    setRect(f_6f0, 0x72, 0xa0, 0x8a, 0xb8);
                else
                    setRect(f_6f0, 0x72, 0xb8, 0x8a, 0xd0);
                DG_PART(*(void**)(EQ_GLOBAL(char*, 0x006e4e40) + 4), f_6f0, DG_DR(0x6df3c));
            }
        }
        // 0043C55F
        DG_INT(0x90254) = 0;
        if (DG_INT(0x90258) != 0)
        {
            f_6f8 = 0;
            do
            {
                f_6c8 = (short)f_6f8;
                if (DG_INT(0xc) != 0 || P02_GEMI(0x843ec) != 0 || P02_GEMI(0x8a1ac) != 0
                    || DG_SHORT(0x5f18a) == f_6c8 + 0xf8 || DG_SHORT(0x5f188) == f_6c8 + 0xf8)
                {
                    P02_GEMI(0x8a1ac) = 0;
                    f_6e0 = DG_SHORT(f_6c8 * 2 + 0xfec);
                    if ((short)f_6e0 != -1)
                    {
                        if ((short)f_6e0 > 0x64)
                        {
                            if ((short)f_6e0 == 0x6f)
                            {
                                if (DG_SHORT(0x5f188) != f_6c8 + 0xf8 && *(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xeb0) != 0)
                                    DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), P02_RECT(0x6d88c));
                                else
                                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3480), P02_RECT(0x6d88c));
                            }
                            else if ((short)f_6e0 == 0x6b)
                            {
                                if (DG_SHORT(0x5f188) != f_6c8 + 0xf8
                                    && *(unsigned int*)(EQ_GLOBAL(char*, 0x006e4de0) + 0xbc)
                                           >= *(unsigned int*)(*(char**)(EQ_GLOBAL(char*, 0x006e4d8c) + 0x70) + 0xf8))
                                    DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), P02_RECT(0x6d88c));
                                else
                                    DG_DRAW(EQ_GLOBAL(void*, 0x006b34e8), P02_RECT(0x6d88c));
                            }
                            else if ((short)f_6e0 == 0x69)
                            {
                                if (DG_SHORT(0x5f188) != f_6c8 + 0xf8
                                    && *(unsigned int*)(EQ_GLOBAL(char*, 0x006e4de0) + 0xbc)
                                           >= *(unsigned int*)(*(char**)(EQ_GLOBAL(char*, 0x006e4d8c) + 0x70) + 0xf8))
                                    DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), P02_RECT(0x6d88c));
                                else
                                    DG_DRAW(EQ_GLOBAL(void*, 0x006b34f0), P02_RECT(0x6d88c));
                            }
                        }
                        else if (DG_ITEMDEF(f_6e0)[0x26] == 2)
                        {
                            if (DG_SHORT(0x5f188) != f_6c8 + 0xf8 && *(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xeb0) != 0)
                                DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_6e0) + 0x8a)), P02_RECT(0x6d88c));
                            else
                                DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_6e0) + 0x8c)), P02_RECT(0x6d88c));
                        }
                        else if (DG_SHORT(0x5f188) != f_6c8 + 0xf8 && EQ_ARRAY(char, 0x006e117c)[f_6e0] != 0)
                            DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_6e0) + 0x8a)), P02_RECT(0x6d88c));
                        else
                            DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_6e0) + 0x8c)), P02_RECT(0x6d88c));
                    }
                    else if ((short)f_6f8 == 0)
                    {
                        if (DG_SHORT(0x5f188) == 0xf8)
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b36b4), DG_DR(0x6d88c));
                        else
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b369c), DG_DR(0x6d88c));
                    }
                    else if ((short)f_6f8 == 1)
                    {
                        if (DG_SHORT(0x5f188) == 0xf9)
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b36b8), DG_DR(0x6d89c));
                        else
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b36a0), DG_DR(0x6d89c));
                    }
                    else if ((short)f_6f8 == 2)
                    {
                        if (DG_SHORT(0x5f188) == 0xfa)
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b36bc), DG_DR(0x6d8ac));
                        else
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b36a4), DG_DR(0x6d8ac));
                    }
                    else if ((short)f_6f8 == 3)
                    {
                        if (DG_SHORT(0x5f188) == 0xfb)
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b36c0), DG_DR(0x6d8bc));
                        else
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b36a8), DG_DR(0x6d8bc));
                    }
                }
                f_6f8++;
            } while ((short)f_6f8 < 4);
            if (DG_SHORT(0x5f188) >= 0)
            {
                switch (DG_SHORT(0x5f188))
                {
                case 0xe3: DG_DRAW(EQ_GLOBAL(void*, 0x006b3668), DG_DR(0x6d73c)); break;
                case 0xe4: DG_DRAW(EQ_GLOBAL(void*, 0x006b3670), DG_DR(0x6d74c)); break;
                case 0xe5: DG_DRAW(EQ_GLOBAL(void*, 0x006b3628), DG_DR(0x6d75c)); break;
                case 0xf6: DG_DRAW(EQ_GLOBAL(void*, 0x006b36ac), DG_DR(0x6d86c)); break;
                case 0xf7: DG_DRAW(EQ_GLOBAL(void*, 0x006b36b0), DG_DR(0x6d87c)); break;
                }
            }
            if (*(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xea4) == 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36ac), DG_DR(0x6d86c));
            if (*(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xea4) == 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36b0), DG_DR(0x6d87c));
        }
        // 0043CAB6
        if (DG_INT(0x685d4) != 0 && DG_SHORT(0x5f188) >= 0x145 && DG_SHORT(0x5f188) <= 0x162)
        {
            setRect(f_6f0, DG_INT((DG_CHAR(0xfe0) * 400 + DG_SHORT(0x5f188)) * 0x10 + 0x6c90c) + 0xf,
                    DG_INT((DG_CHAR(0xfe0) * 400 + DG_SHORT(0x5f188)) * 0x10 + 0x6c910) + 0xc, 0x280, 0x1e0);
            f_6e0 = DG_SHORT(DG_SHORT(0x5f188) * 2 + 0x6c3d4);
            if ((short)f_6e0 > 0x64)
            {
                switch ((short)f_6e0)
                {
                case 0x69: DG_DRAW(EQ_GLOBAL(void*, 0x006b34f0), f_6f0); break;
                case 0x6b: DG_DRAW(EQ_GLOBAL(void*, 0x006b34e8), f_6f0); break;
                case 0x6f: DG_DRAW(EQ_GLOBAL(void*, 0x006b3480), f_6f0); break;
                }
            }
            else
                DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8c)), f_6f0);
        }
        // 0043CB91
        DG_INT(0x90254) = 0;
        if (DG_INT(0x90258) == 0)
            goto L_0043DCFC;
        if (DG_INT(0xc) != 0 || DG_SLOT(0x843e4) != 0 || DG_SLOT(0x8a1a4) != 0)
        {
            if (DG_SHORT(0x5f188) != 0xf6)
            {
                DG_IMGSET(0x006e4e40);
                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 8, 0xa7, 0x2f, 0xce);
                DG_D341C(DG_DR(0x6d86c));
            }
            if (*(char**)(DG_PC + 0x114) != 0)
                DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x114) + 0x84)), DG_DR(0x6d86c));
        }
        if (DG_INT(0xc) == 0 && DG_SLOT(0x843e8) == 0 && DG_SLOT(0x8a1a8) == 0)
            goto L_0043DCFC;
        if (DG_SHORT(0x5f188) != 0xf7)
        {
            DG_IMGSET(0x006e4e40);
            setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x39, 0xa7, 0x60, 0xce);
            DG_D341C(DG_DR(0x6d87c));
        }
        if (*(char**)(DG_PC + 0x10c) != 0)
            DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x10c) + 0x84)), DG_DR(0x6d87c));
        if (DG_COMBINE(*(char**)(DG_PC + 0x10c)) == 0)
            goto L_0043DCFC;
        setRect(f_6f0, DG_RX(0x6d884) - 0x13, DG_RX(0x6d888) - 0xf, DG_RX(0x6d884), DG_RX(0x6d888));
        DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), f_6f0);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c,
                   ((unsigned char)(*(char**)(DG_PC + 0x10c))[0xda] - 1) / (*(char**)(DG_PC + 0x10c))[0xbb] + 1);
        DG_TEXT(f_6bc, DG_RX(0x6d87c) + 0x1a, DG_RX(0x6d880) + 0x1d, 10, DG_FONT3);
        goto L_0043DCFC;

    case 1: // 0043CDD8
        DG_INT(0x90254) = 0;
        if (DG_INT(0x90258) != 0)
        {
            if (DG_SLOT(0x84398) != 0 || DG_SLOT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3648), DG_DR(0x6d73c));
                DG_SLOT(0x8a158) = 0;
            }
            if (DG_SLOT(0x8439c) != 0 || DG_SLOT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b362c), DG_DR(0x6d74c));
                DG_SLOT(0x8a15c) = 0;
            }
            if (DG_SLOT(0x843a0) != 0 || DG_SLOT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3630), DG_DR(0x6d75c));
                DG_SLOT(0x8a160) = 0;
            }
            if (DG_INT(0xc) != 0 || DG_SLOT(0x8a180) != 0 || (char*)self + 0xe094c != 0)
            {
                for (f_6c8 = 0; f_6c8 < 6; f_6c8++)
                    P02_GEMI(0x8a18c) = 1;
            }
        }
        // 0043CF4D
        if (DG_INT(0x685d8) != 0)
        {
            f_6f8 = 0;
            if (DG_INT(0x685bc) > 0)
            {
                f_6c8 = 0;
                do
                {
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b36c4), P02_RECT(0x6dd5c));
                    setRect(f_6f0, P02_RX(0x6dd5c) + 0xf, P02_RX(0x6dd60) + 0xc, 0x280, 0x1e0);
                    f_6e0 = DG_SHORT(f_6c8 * 2 + 0x6c65e);
                    if ((short)f_6e0 > 0x64)
                    {
                        switch ((short)f_6e0)
                        {
                        case 0x69: DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), f_6f0); break;
                        case 0x6b: DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), f_6f0); break;
                        case 0x6f: DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), f_6f0); break;
                        case 0x72:
                            DG_IMGSET(0x006e4e40);
                            setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x223, 0x30, 0x24b, 0x58);
                            DG_D341C(f_6f0);
                            DG_TEXT(0x005589e0, f_6f0[0] + 2, f_6f0[1] + 8, 0, DG_FONT2);
                            DG_TEXT(0x00540678, f_6f0[0] + 6, f_6f0[1] + 0x12, 0, DG_FONT2);
                            break;
                        }
                    }
                    else
                        DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8a)), f_6f0);
                    f_6f8++;
                    f_6c8 = (short)f_6f8;
                } while (f_6c8 < DG_INT(0x685bc));
            }
            if (DG_INT(0x685bc) != 0)
            {
                setRect(f_6f0, 0x72, 0xd0, 0x8a, 0x112);
                setRect(f_6cc, DG_RX(0x6df3c), DG_RX(0x6df48), 0x280, 0x1e0);
                DG_PART(*(void**)(EQ_GLOBAL(char*, 0x006e4e40) + 4), f_6f0, f_6cc);
                if (DG_SHORT(0x5f188) == 0x163)
                    setRect(f_6f0, 0x72, 0xa0, 0x8a, 0xb8);
                else
                    setRect(f_6f0, 0x72, 0xb8, 0x8a, 0xd0);
                DG_PART(*(void**)(EQ_GLOBAL(char*, 0x006e4e40) + 4), f_6f0, DG_DR(0x6df3c));
            }
        }
        // 0043D1C0
        DG_INT(0x90254) = 0;
        if (DG_INT(0x90258) != 0)
        {
            f_6c8 = 0;
            f_6fc = 0x006b37c8;
            f_6e0 = DG_THIS + 0xff4;
            f_6d8 = (short*)6;
            do
            {
                if (DG_INT(0xc) != 0 || P02_GEMI(0x843cc) != 0 || DG_SHORT(0x5f18a) == f_6c8 + 0xf0
                    || P02_GEMI(0x8a18c) != 0 || DG_SHORT(0x5f188) == f_6c8 + 0xf0)
                {
                    P02_GEMI(0x8a18c) = 0;
                    if (*(short*)f_6e0 == -1)
                    {
                        if (DG_SHORT(0x5f188) == f_6c8 + 0xf0)
                            DG_DRAW(*(void**)(f_6fc + 0x18), P02_RECT(0x6d80c));
                        else
                            DG_DRAW(*(void**)f_6fc, P02_RECT(0x6d80c));
                    }
                    else if (*(short*)f_6e0 > 0x64)
                    {
                        if (*(short*)f_6e0 == 0x6f)
                        {
                            if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && *(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xeb0) != 0)
                                DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), P02_RECT(0x6d80c));
                            else
                                DG_DRAW(EQ_GLOBAL(void*, 0x006b3480), P02_RECT(0x6d80c));
                        }
                        else if (*(short*)f_6e0 == 0x6b)
                        {
                            if (DG_SHORT(0x5f188) != f_6c8 + 0xf0
                                && *(unsigned int*)(EQ_GLOBAL(char*, 0x006e4de0) + 0xbc)
                                       >= *(unsigned int*)(*(char**)(EQ_GLOBAL(char*, 0x006e4d8c) + 0x70) + 0xf8))
                                DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), P02_RECT(0x6d80c));
                            else
                                DG_DRAW(EQ_GLOBAL(void*, 0x006b34e8), P02_RECT(0x6d80c));
                        }
                        else if (*(short*)f_6e0 == 0x69)
                        {
                            if (DG_SHORT(0x5f188) != f_6c8 + 0xf0
                                && *(unsigned int*)(EQ_GLOBAL(char*, 0x006e4de0) + 0xbc)
                                       >= *(unsigned int*)(*(char**)(EQ_GLOBAL(char*, 0x006e4d8c) + 0x70) + 0xf8))
                                DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), P02_RECT(0x6d80c));
                            else
                                DG_DRAW(EQ_GLOBAL(void*, 0x006b34f0), P02_RECT(0x6d80c));
                        }
                        else if (*(short*)f_6e0 == 0x72)
                        {
                            if (DG_SHORT(0x5f188) != f_6c8 + 0xf0
                                && *(unsigned int*)(EQ_GLOBAL(char*, 0x006e4de0) + 0xbc)
                                       >= *(unsigned int*)(*(char**)(EQ_GLOBAL(char*, 0x006e4d8c) + 0x70) + 0xf8))
                            {
                                DG_IMGSET(0x006e4e40);
                                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x223, 0x30, 0x24b, 0x58);
                            }
                            else
                            {
                                DG_IMGSET(0x006e4e40);
                                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x24b, 0x30, 0x273, 0x58);
                            }
                            DG_D341C(P02_RECT(0x6d80c));
                            DG_TEXT(0x005589e0, P02_RX(0x6d80c) + 2, P02_RX(0x6d810) + 8, 0, DG_FONT2);
                            DG_TEXT(0x00540678, P02_RX(0x6d80c) + 6, P02_RX(0x6d810) + 0x12, 0, DG_FONT2);
                        }
                    }
                    else
                    {
                        f_6dc = (short*)(int)*(short*)f_6e0;
                        if (DG_ITEMDEF((int)f_6dc)[0x26] == 2)
                        {
                            if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && *(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xeb0) != 0)
                                DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((int)f_6dc) + 0x8a)), P02_RECT(0x6d80c));
                            else
                                DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((int)f_6dc) + 0x8c)), P02_RECT(0x6d80c));
                        }
                        else if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && EQ_ARRAY(char, 0x006e117c)[(int)f_6dc] != 0)
                            DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((int)f_6dc) + 0x8a)), P02_RECT(0x6d80c));
                        else
                            DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((int)f_6dc) + 0x8c)), P02_RECT(0x6d80c));
                    }
                }
                f_6e0 += 2;
                f_6fc += 4;
                f_6c8++;
                f_6d8 = (short*)((int)f_6d8 - 1);
            } while (f_6d8 != 0);
            if (DG_SHORT(0x5f188) >= 0 && DG_SHORT(0x5f188) != DG_SHORT(0x5f18a))
            {
                switch (DG_SHORT(0x5f188))
                {
                case 0xe3: DG_DRAW(EQ_GLOBAL(void*, 0x006b366c), DG_DR(0x6d73c)); break;
                case 0xe4: DG_DRAW(EQ_GLOBAL(void*, 0x006b3670), DG_DR(0x6d74c)); break;
                case 0xe5: DG_DRAW(EQ_GLOBAL(void*, 0x006b3628), DG_DR(0x6d75c)); break;
                }
            }
        }
        // 0043D699
        if (DG_INT(0x685d8) == 0 || DG_SHORT(0x5f188) < 0x145 || DG_SHORT(0x5f188) > 0x162)
            goto L_0043DCFC;
        setRect(f_6f0, DG_INT((DG_CHAR(0xfe0) * 400 + DG_SHORT(0x5f188)) * 0x10 + 0x6c90c) + 0xf,
                DG_INT((DG_CHAR(0xfe0) * 400 + DG_SHORT(0x5f188)) * 0x10 + 0x6c910) + 0xc, 0x280, 0x1e0);
        f_6e0 = DG_SHORT(DG_SHORT(0x5f188) * 2 + 0x6c3d4);
        if ((short)f_6e0 > 0x64)
        {
            switch ((short)f_6e0)
            {
            case 0x69: carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b34f0), f_6f0); break;
            case 0x6b: carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b34e8), f_6f0); break;
            case 0x6f: carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b3480), f_6f0); break;
            case 0x72:
                DG_IMGSET(0x006e4e40);
                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x24b, 0x30, 0x273, 0x58);
                DG_D341C(f_6f0);
                DG_TEXT(0x005589e0, f_6f0[0] + 2, f_6f0[1] + 8, 0, DG_FONT2);
                DG_TEXT(0x00540678, f_6f0[0] + 6, f_6f0[1] + 0x12, 0, DG_FONT2);
                break;
            }
        }
        else
            carryEax = P02_DRAWRET(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8c)), f_6f0);
        goto L_0043DCFC;

    case 0: // 0043D81D (screen 4 comes here too: the rects are this-relative, the slots are not)
L_0043D81D:
        DG_INT(0x90254) = 0;
        if (DG_INT(0x90258) == 0)
            goto L_0043DCFC;
        if (DG_INT(0x84398) != 0 || DG_INT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
        {
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3644), (char*)self + 0x6d73c);
            DG_SLOT(0x8a158) = 0;
        }
        if (DG_INT(0x8439c) != 0 || DG_INT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
        {
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3648), (char*)self + 0x6d74c);
            DG_SLOT(0x8a15c) = 0;
        }
        if (DG_INT(0x843a0) != 0 || DG_INT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
        {
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3630), (char*)self + 0x6d75c);
            DG_INT(0x8a160) = 0;
        }
        if (DG_SHORT(0x5f188) >= 0 && DG_SHORT(0x5f18a) != DG_SHORT(0x5f188))
        {
            switch (DG_SHORT(0x5f188))
            {
            case 0xe3: DG_DRAW(EQ_GLOBAL(void*, 0x006b3668), (char*)self + 0x6d73c); break;
            case 0xe4: DG_DRAW(EQ_GLOBAL(void*, 0x006b366c), (char*)self + 0x6d74c); break;
            case 0xe5: DG_DRAW(EQ_GLOBAL(void*, 0x006b3628), (char*)self + 0x6d75c); break;
            case 0xe6: DG_DRAW(EQ_GLOBAL(void*, 0x006b3690), (char*)self + 0x6d76c); break;
            case 0xe7:
                if (DG_CHAR(0xb5) != 0)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3680), (char*)self + 0x6d77c);
                else
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3614), (char*)self + 0x6d77c);
                break;
            case 0xe8: DG_DRAW(EQ_GLOBAL(void*, 0x006b3604), (char*)self + 0x6d78c); break;
            case 0xe9: DG_DRAW(EQ_GLOBAL(void*, 0x006b367c), (char*)self + 0x6d79c); break;
            case 0xea:
                if (*(char*)(EQ_GLOBAL(char*, 0x006e4da0) + 0xa1) == 0x6e)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b35fc), (char*)self + 0x6d7ac);
                else
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3688), (char*)self + 0x6d7ac);
                break;
            case 0xeb:
                if (EQ_GLOBAL(char, 0x006b6405) != 0)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b368c), (char*)self + 0x6d7bc);
                else
                {
                    DG_IMGSET(0x006e4e40);
                    setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 1, 0x1cd, 0x66, 0x1df);
                    DG_D341C((char*)self + 0x6d7bc);
                }
                break;
            }
        }
        // 0043DA3F
        f_6c8 = 0x212;
        if (DG_INT(0x843a4) != 0 || DG_SHORT(0x5f18a) == 0xe6 || DG_INT(0xc) != 0)
        {
            if (DG_SHORT(0x5f188) != 0xe6)
            {
                DG_IMGSET(0x006e4e44);
                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, f_6c8, 0x14b, 0x277, 0x15d);
                DG_D341C((char*)self + 0x6d76c);
            }
        }
        if (DG_INT(0x843a8) != 0 || DG_SHORT(0x5f18a) == 0xe7 || DG_INT(0xc) != 0)
        {
            if (DG_SHORT(0x5f188) != 0xe7)
            {
                if (DG_CHAR(0xb5) != 0)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3678), (char*)self + 0x6d77c);
                else
                {
                    DG_IMGSET(0x006e4e44);
                    setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, f_6c8, 0x162, 0x277, 0x174);
                    DG_D341C((char*)self + 0x6d77c);
                }
            }
        }
        if (DG_INT(0x843ac) != 0 || DG_SHORT(0x5f18a) == 0xe8 || DG_INT(0xc) != 0)
        {
            if (DG_SHORT(0x5f188) != 0xe8)
            {
                DG_IMGSET(0x006e4e44);
                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, f_6c8, 0x179, 0x277, 0x18b);
                carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b341c), (char*)self + 0x6d78c);
            }
        }
        if (DG_INT(0x843b4) != 0 || DG_SHORT(0x5f18a) == 0xea || DG_INT(0xc) != 0)
        {
            if (DG_SHORT(0x5f188) != 0xea)
            {
                if (*(char*)(EQ_GLOBAL(char*, 0x006e4da0) + 0xa1) == 0x6e)
                    carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b35f8), (char*)self + 0x6d7ac);
                else
                {
                    DG_IMGSET(0x006e4e44);
                    setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, f_6c8, 0x1a7, 0x277, 0x1b9);
                    carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b341c), (char*)self + 0x6d7ac);
                }
            }
        }
        if (DG_INT(0x843b0) != 0 || DG_SHORT(0x5f18a) == 0xe9 || DG_INT(0xc) != 0)
        {
            if (DG_SHORT(0x5f188) != 0xe9)
            {
                DG_IMGSET(0x006e4e44);
                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, f_6c8, 0x190, 0x277, 0x1a2);
                carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b341c), (char*)self + 0x6d79c);
            }
        }
        if (DG_INT(0x843b8) == 0 && DG_SHORT(0x5f18a) != 0xeb && DG_INT(0xc) == 0)
            goto L_0043DCFC;
        if (DG_SHORT(0x5f188) == 0xeb)
            goto L_0043DCFC;
        if (EQ_GLOBAL(char, 0x006b6405) != 0)
        {
            DG_IMGSET(0x006e4e44);
            setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, f_6c8, 0x1be, 0x277, 0x1d0);
        }
        else
        {
            DG_IMGSET(0x006e4e40);
            setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x200, 0x1c0, 0x265, 0x1d2);
        }
        carryEax = P02_DRAWRET(EQ_GLOBAL(void*, 0x006b341c), (char*)self + 0x6d7bc);
        break;

    default:
        goto L_0043DCFC;
    }
#undef P02_GEM
#undef P02_GEMI
#undef P02_RECT
#undef P02_RX
#undef P02_TEXT16
#undef P02_DRAWRET
    // END REGION P02

    // REGION P03 0x0043DCFC-0x00441AE2: 15846 B, arm of JZ @0043B9AC (taken): CMP dword ptr [ESI + 0x90098],0x0
    //   defines L_0043DCFC; exits -; returns
    //   switch scaffold for this piece (paste where the dispatch is):
    //   // SWITCH @00441087 (in P03): table 0x0044FE49, 21 entries (cmp), bias 0; selector: JNZ LAB_0044113e ; MOV EAX,dword ptr [ESP + local_6d8] ; CMP EAX,0x14 ; JA default ; JMP dword ptr [EAX*0x4 + switchdataD_0044fe49]
    //   switch (/* TODO selector (register EAX) */ 0) {
    //   case 0: case 3: goto L_0044109C; /* P03 */
    //   case 1: goto L_0044108E; /* P03 */
    //   case 2: goto L_00441095; /* P03 */
    //   case 4: goto L_004410A3; /* P03 */
    //   case 5: goto L_004410AA; /* P03 */
    //   case 6: goto L_004410B1; /* P03 */
    //   case 7: goto L_004410B8; /* P03 */
    //   case 8: case 9: goto L_004410BF; /* P03 */
    //   case 0xA: goto L_004410C6; /* P03 */
    //   case 0xB: goto L_004410CD; /* P03 */
    //   case 0xC: goto L_004410D4; /* P03 */
    //   case 0xD: goto L_004410DB; /* P03 */
    //   case 0xE: case 0xF: goto L_004410E2; /* P03 */
    //   case 0x10: goto L_004410E9; /* P03 */
    //   case 0x11: goto L_004410F0; /* P03 */
    //   case 0x12: goto L_004410F7; /* P03 */
    //   case 0x13: goto L_004410FE; /* P03 */
    //   case 0x14: goto L_00441105; /* P03 */
    //   default: goto L_00441116; /* P03 */
    //   }
    // f_6fc/f_6f4/f_6e0 drive the slot loops (item pointer or offset, slot index, count) as in the asm;
    // f_6d4/f_6d0 stand in for the EBP index and EDI pointer of the 0x12e6 and last held-item loops.
#define P03_RX(slot, off) DG_INT((DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + (off))
#define P03_SLOT(slot, off) ((DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + DG_THIS + (off))
#define P03_COUNT(item) \
    (((int)*(unsigned char*)((char*)(item) + 0xda) - 1) / (int)*(signed char*)((char*)(item) + 0xbb) + 1)
#define P03_STACK(slot, off, item, dx, dy, tx, ty) \
    setRect(f_6f0, P03_RX(slot, (off) + 8) - (dx), P03_RX(slot, (off) + 0xc) - (dy), \
            P03_RX(slot, (off) + 8), P03_RX(slot, (off) + 0xc)); \
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), f_6f0); \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, P03_COUNT(item)); \
    DG_TEXT(f_6bc, P03_RX(slot, (off) + 8) - (tx), P03_RX(slot, (off) + 0xc) - (ty), 10, DG_FONT3);
#define P03_ITEMLOOP(base, count, off, item) \
    f_6f4 = 0; \
    f_6fc = (base); \
    f_6e0 = (count); \
    do \
    { \
        if ((item) != 0) \
        { \
            setRect(f_6f0, P03_RX(f_6f4, off) + 3, P03_RX(f_6f4, (off) + 4) + 3, 0x280, 0x1e0); \
            DG_DRAW(DG_ITEMIMG(*(short*)((item) + 0x84)), f_6f0); \
            if (DG_COMBINE(item) != 0) \
            { \
                P03_STACK(f_6f4, off, item, 0x12, 0xe, 0xd, 9) \
            } \
        } \
        f_6f4++; \
        f_6fc += 4; \
    } while (--f_6e0 != 0);
#define P03_CMP3(v, base) ((int)(v) > (int)(base) ? 0xe : ((int)(v) < (int)(base) ? 0xd : 7))
#define P03_NUM(val, x, y) \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, x, y, 7, DG_FONT);
#define P03_BANK(off, val) \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, DG_RX(off) + 0x28, DG_RY(off) + 5, 10, DG_FONT);
#define P03_STAT(label, dy, val, pcoff) \
    DG_TEXT(label, f_6f8 + 0x19, f_63c + (dy), 7, DG_FONT); \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, f_6f8 + 0x37, f_63c + (dy), P03_CMP3(val, *(unsigned char*)(DG_PC + (pcoff))), DG_FONT);
#define P03_RESIST(label, dy, val, base) \
    DG_TEXT(label, f_6f8 + 0xf, f_63c + (dy), 7, DG_FONT); \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, f_6f8 + 0x41, f_63c + (dy), P03_CMP3(val, base), DG_FONT);
#define P03_COIN(label, off, val) \
    DG_TEXT(label, DG_RX(off) + 0xa, DG_RY(off), 10, DG_FONT); \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, DG_RX(off) + 0x14, DG_RY(off) + 0x14, 10, DG_FONT);
#define P03_HILITE() \
    if (DG_INT(8) == 0) \
        DG_CHAR(0x6c904) ^= 1; \
    if (DG_CHAR(0x6c904) != 0) \
        setRect(f_6f0, 0x167, 0x8a, 0x196, 0xb9); \
    else \
        setRect(f_6f0, 0x167, 0xb9, 0x196, 0xe8);
#define P03_HELD (*(int*)DG_INT(0x6c658))
#define P03_HELDOK (DG_SHORT(0x6c65c) >= 0x1e && DG_INT(0x6c658) != 0 && P03_HELD != 0)
#define P03_DEC(p) (((p) = (short*)((int)(p) - 1)) != 0)
#define P03_STATS_STORE() \
    DG_CHAR(0x8fbd2) = DG_PC[0x38]; \
    DG_SHORT(0x8fbd4) = (short)f_6d0; \
    DG_SHORT(0x8fbd6) = (short)f_6d4; \
    DG_SHORT(0x8fbd8) = (short)(int)f_6d8; \
    DG_SHORT(0x8fbdc) = (short)f_6e0; \
    DG_SHORT(0x8fbe2) = (short)f_628; \
    DG_SHORT(0x8fbe0) = (short)f_624; \
    DG_SHORT(0x8fbe4) = (short)f_620; \
    DG_SHORT(0x8fbe6) = (short)(int)f_61c; \
    DG_SHORT(0x8fbe8) = (short)f_618; \
    DG_SHORT(0x8fbea) = (short)f_614; \
    DG_SHORT(0x8fbec) = (short)f_610; \
    DG_CHAR(0x8fbee) = (char)f_60c; \
    DG_CHAR(0x8fbef) = (char)f_608; \
    DG_CHAR(0x8fbf0) = (char)f_604; \
    DG_CHAR(0x8fbf1) = (char)f_600; \
    DG_CHAR(0x8fbf2) = (char)f_5fc; \
    DG_SHORT(0x8fbf4) = (short)f_6f4; \
    DG_SHORT(0x8fbf6) = (short)f_6fc;
L_0043DCFC:
    DG_INT(0x90194) = 0;
    if (DG_INT(0x90198) != 0)
    {
        if (DG_INT(0x685e0) != 0 || DG_INT(0x685dc) != 0)
        {
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3804), DG_DR(0x6e02c));
            setRect(f_6f0, 0xe9, 0x145, 0xf0, 0x1e0);
            setRect(f_6cc, 0x70, 0x145, 0x77, 0x1e0);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, f_6cc);
        }
        else if (DG_INT(0x685e4) != 0 || DG_INT(0x685e8) != 0)
        {
            if (DG_INT(0x685e4) != 0)
            {
                if (EQ_GLOBAL(char*, 0x006e4d88) != 0 && *(EQ_GLOBAL(char*, 0x006e4d88) + 0x98) == 1)
                {
                    setRect(f_6f0, 0x190, 0, 0x207, 0x1e0);
                    setRect(f_6cc, 0, 0, 0x280, 0x1e0);
                    DG_PART(DG_OVL(0x006e4e0c), f_6f0, f_6cc);
                }
                else
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b38d4), DG_DR(0x6e03c));
            }
            else
            {
                setRect(f_6f0, 0x79, 0, 0xf0, 0x1e0);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6d48c));
            }
            setRect(f_6f0, 0xe9, 0x13b, 0xf0, 0x1e0);
            setRect(f_6cc, 0x70, 0x13b, 0x77, 0x1e0);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, f_6cc);
        }

        // 0043DE66
        if (DG_INT(0x685e0) == 0 && DG_INT(0x685dc) == 0)
        {
            if (DG_INT(0x685e4) != 0)
            {
                if (EQ_GLOBAL(char*, 0x006e4d88) != 0 && *(EQ_GLOBAL(char*, 0x006e4d88) + 0x98) == 1)
                {
                    // 0043DEA3: merchant
                    setRect(f_6f0, 0x190, 0, 0x207, 0x1e0);
                    setRect(f_6cc, 0, 0, 0x280, 0x1e0);
                    DG_PART(DG_OVL(0x006e4e0c), f_6f0, f_6cc);
                    DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(EQ_GLOBAL(char*, 0x006e4d88) + 1), 0x5a));
                    DG_TEXT(f_6bc, 10, 0xf, 0xe, DG_FONT);
                    P03_ITEMLOOP(DG_THIS + 0x6c50c, 4, 0x6dc6c, *(char**)f_6fc)
                    P03_NUM(DG_INT(0x6c52c), 0x32, 0x10f)
                    P03_NUM(DG_INT(0x6c530), 0x32, 0x120)
                    P03_NUM(DG_INT(0x6c534), 0x32, 0x131)
                    P03_NUM(DG_INT(0x6c538), 0x32, 0x142)
                    if (DG_SHORT(0x5f188) != -1)
                    {
                        if (DG_SHORT(0x5f188) == 0x143)
                        {
                            setRect(f_6f0, 0x168, 0x140, 0x190, 0x168);
                            DG_PART(DG_OVL(0x006e4e0c), f_6f0, DG_DR(0x6dd3c));
                        }
                        if (DG_SHORT(0x5f188) == 0x144)
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b38dc), DG_DR(0x6dd4c));
                    }
                    if (DG_INT(0x685ec) != 0)
                    {
                        setRect(f_6f0, 0x168, 0x140, 0x190, 0x168);
                        DG_PART(DG_OVL(0x006e4e0c), f_6f0, DG_DR(0x6dd3c));
                    }
                }
                else
                {
                    // 0043E220: trade
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b38d4), DG_DR(0x6e03c));
                    DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(EQ_GLOBAL(char*, 0x006e4d88) + 1), 0x5a));
                    DG_TEXT(f_6bc, 10, 0xf, DG_INT(0x685f0) != 0 ? 0xe : 0xd, DG_FONT);
                    DG_STRCPY(f_6bc, DG_TRUNC(EQ_GLOBAL(char*, 0x006e4d8c) + 1, 0x5a));
                    DG_TEXT(f_6bc, 0x73, 0xf, DG_INT(0x685ec) != 0 ? 0xe : 0xd, DG_FONT);
                    P03_ITEMLOOP(DG_THIS + 0x6c4ec, 8, 0x6dbcc, *(char**)f_6fc)
                    P03_ITEMLOOP(DG_THIS + 0x6c50c, 8, 0x6dc6c, *(char**)f_6fc)
                    P03_NUM(DG_INT(0x6c53c), 0x2d, 0xf6)
                    P03_NUM(DG_INT(0x6c540), 0x2d, 0x107)
                    P03_NUM(DG_INT(0x6c544), 0x2d, 0x118)
                    P03_NUM(DG_INT(0x6c548), 0x2d, 0x129)
                    P03_NUM(DG_INT(0x6c52c), 0x78, 0xf6)
                    P03_NUM(DG_INT(0x6c530), 0x78, 0x107)
                    P03_NUM(DG_INT(0x6c534), 0x78, 0x118)
                    P03_NUM(DG_INT(0x6c538), 0x78, 0x129)
                    if (DG_SHORT(0x5f188) != -1)
                    {
                        if (DG_SHORT(0x5f188) == 0x143)
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b38d8), DG_DR(0x6dd3c));
                        if (DG_SHORT(0x5f188) == 0x144)
                            DG_DRAW(EQ_GLOBAL(void*, 0x006b38dc), DG_DR(0x6dd4c));
                    }
                    if (DG_INT(0x685ec) != 0)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b38d8), DG_DR(0x6dd3c));
                }
            }
            else if (DG_INT(0x685e8) != 0)
            {
                // 0043E7D1: bank or house chest
                setRect(f_6f0, 0x79, 0, 0xf0, 0x1e0);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6d48c));
                if (DG_CHAR(0x101) != 0)
                    DG_STRCPY(f_6bc, 0x0055a048);
                else
                    DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(EQ_GLOBAL(char*, 0x006e4dbc) + 1), 0x5a));
                DG_TEXT(f_6bc, 10, 0xf, 10, DG_FONT);
                if (DG_CHAR(0x101) != 0)
                {
                    P03_ITEMLOOP(DG_THIS + 0x108, 8, 0x6d3ac, *(char**)f_6fc)
                }
                else
                {
                    P03_ITEMLOOP(0xf44, 8, 0x6d3ac, *(char**)(f_6fc + (int)DG_PC))
                }
                if (DG_CHAR(0x101) != 0)
                {
                    P03_BANK(0x6d44c, DG_INT(0x128))
                    P03_BANK(0x6d45c, DG_INT(0x12c))
                    P03_BANK(0x6d46c, DG_INT(0x130))
                    P03_BANK(0x6d47c, DG_INT(0x134))
                }
                else
                {
                    P03_BANK(0x6d44c, *(int*)(DG_PC + 0x9a8))
                    P03_BANK(0x6d45c, *(int*)(DG_PC + 0x9ac))
                    P03_BANK(0x6d46c, *(int*)(DG_PC + 0x9b0))
                    P03_BANK(0x6d47c, *(int*)(DG_PC + 0x9b4))
                }
            }
            else
            {
                // 0043EDB5
                if (DG_CHAR(0x100c) == 0)
                    goto L_0043F971;
                if (DG_INT(0x84) != 0)
                {
                    DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(DG_INT(0x84) + 1), 0x5a));
                    DG_TEXT(f_6bc, DG_RX(0x6df5c) + 5, DG_RY(0x6df5c), 10, DG_FONT);
                }
                EQ_METHOD(0, void, 0x0046a491, (void*), self)();
            }
        }
        else
        {
            // 0043EE36: loot / give windows
            DG_SLOT(0x8a394) = 0;
            if (DG_INT(0x685e0) != 0)
            {
                f_6f8 = 0;
                f_6d4 = 0;
                do
                {
                    DG_INT((DG_CHAR(0xfe0) * 400 + f_6d4) * 4 + 0x8a1dc) = 0;
                    DG_SLOT(0x8a260) = 1;
                    DG_SLOT(0x8a264) = 1;
                    f_6f4 = (short)(f_6f8 + DG_CHAR(0x6c592) * 2) * 4 + DG_THIS + 0x6c5e0;
                    if (*(int*)f_6f4 != 0)
                    {
                        setRect(f_6f0, P03_RX(f_6d4, 0x6d94c) + 3, P03_RX(f_6d4, 0x6d950) + 3, 0x280, 0x1e0);
                        DG_DRAWIMAGE(f_6f0, *(short*)(*(char**)f_6f4 + 0x84));
                        if (DG_COMBINE(*(char**)f_6f4) != 0)
                        {
                            P03_STACK(f_6d4, 0x6d94c, *(char**)f_6f4, 0x12, 0xe, 0xd, 9)
                        }
                    }
                    f_6f8++;
                    f_6d4++;
                } while ((short)f_6f8 < 0xc);
                DG_SLOT(0x8a1d8) = 0;
                DG_SLOT(0x8a260) = 1;
                DG_SLOT(0x8a264) = 1;
                if (DG_INT(0x6c658) != 0 && P03_HELD != 0)
                {
                    setRect(f_6f0, DG_RX(0x6d93c) + 3, DG_RY(0x6d93c) + 3, 0x280, 0x1e0);
                    DG_DRAWIMAGE(f_6f0, *(short*)(P03_HELD + 0x84));
                    if (DG_COMBINE(P03_HELD) != 0)
                    {
                        P03_STACK(0, 0x6d93c, P03_HELD, 0x12, 0xe, 0xd, 9)
                    }
                }
            }
            else if (DG_INT(0x685dc) != 0)
            {
                // 0043F1F8
                f_6f8 = 0;
                f_6d4 = 0;
                do
                {
                    DG_INT((DG_CHAR(0xfe0) * 400 + f_6d4) * 4 + 0x8a218) = 0;
                    DG_SLOT(0x8a260) = 1;
                    DG_SLOT(0x8a264) = 1;
                    f_6f4 = (short)(f_6f8 + DG_CHAR(0x6c592) * 2) * 4 + DG_THIS + 0x6c5e0;
                    if (*(int*)f_6f4 != 0)
                    {
                        setRect(f_6f0, P03_RX(f_6d4, 0x6da3c) + 3, P03_RX(f_6d4, 0x6da40) + 3, 0x280, 0x1e0);
                        DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)f_6f4 + 0x84)), f_6f0);
                        if (DG_COMBINE(*(char**)f_6f4) != 0)
                        {
                            P03_STACK(f_6d4, 0x6da3c, *(char**)f_6f4, 0x12, 0xe, 0xd, 9)
                        }
                    }
                    f_6f8++;
                    f_6d4++;
                } while ((short)f_6f8 < 0x10);
            }

            // 0043F15D
            if (DG_INT(0x685e0) != 0 && DG_INT(0x685dc) == 0 && DG_INT(0x6c658) != 0)
            {
                f_6f8 = 0;
                f_6e0 = 0;
                do
                {
                    f_6d8 = (short*)P03_HELD;
                    if (f_6d8 != 0 && (int)f_6d8 == DG_INT((short)(f_6f8 + DG_CHAR(0x6c592) * 2) * 4 + 0x6c5e0))
                    {
                        P03_HILITE()
                        DG_PART(DG_OVL(0x006e4e24), f_6f0, P03_SLOT(f_6e0, 0x6d94c));
                        DG_SLOT(0x844a0) = 1;
                        DG_SLOT(0x844a4) = 1;
                    }
                    f_6f8++;
                    f_6e0++;
                } while ((short)f_6f8 < 0xc);
            }
            if (DG_INT(0x685e0) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 - 1);
            if (DG_INT(0x685dc) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 - 1);
            DG_SLOT(0x8a260) = 0;
            if ((short)f_6fc >= 0)
            {
                f_6e0 = (short)f_6fc * 4 + DG_THIS + 0x6c5e0;
                f_6d8 = (short*)((short)f_6fc + 1);
                do
                {
                    if (*(int*)f_6e0 != 0)
                        DG_SLOT(0x8a260) = 1;
                    f_6e0 -= 4;
                } while (P03_DEC(f_6d8));
            }
            if (DG_SLOT(0x8a260) != 0)
            {
                DG_SLOT(0x8a260) = 0;
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3864), DG_DR(0x6db5c));
            }
            if (DG_INT(0x685e0) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 + 0xc);
            if (DG_INT(0x685dc) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 + 0x10);
            DG_SLOT(0x8a264) = 0;
            if ((short)f_6fc < 0x1e)
            {
                f_6e0 = (short)f_6fc * 4 + DG_THIS + 0x6c5e0;
                f_6d4 = 0x1e - (short)f_6fc;
                do
                {
                    if (*(int*)f_6e0 != 0)
                        DG_SLOT(0x8a264) = 1;
                    f_6e0 += 4;
                } while (--f_6d4 != 0);
            }
            if (DG_SLOT(0x8a264) != 0)
            {
                DG_SLOT(0x8a264) = 0;
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3868), DG_DR(0x6db6c));
            }

            // 0043F5C1: window title
            if (DG_INT(0x685e0) != 0 && EQ_GLOBAL(char*, 0x006e4db8) != 0)
            {
                DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4db8) + 1));
                DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
            }
            if (DG_INT(0x685dc) != 0)
            {
                if (EQ_GLOBAL(char*, 0x006e4dc0) != 0)
                {
                    DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4dc0) + 1));
                    DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
                }
                else
                    DG_STRCPY(f_6bc, 0x0055a040);
            }
            DG_TEXT(f_6bc, 10, 0xd, 10, DG_FONT);
            if (DG_INT(0x685e0) != 0)
            {
                if (DG_SHORT(0x5f188) == 0x112)
                {
                    if (P03_HELDOK)
                        setRect(f_6f0, 0x168, 0x77, 0x1b6, 0x89);
                    else
                        setRect(f_6f0, 0x168, 0x64, 0x1b6, 0x76);
                }
                else if (P03_HELDOK)
                    setRect(f_6f0, 0x168, 0x51, 0x1b6, 0x63);
                else
                    setRect(f_6f0, 0x106, 0x1c2, 0x154, 0x1d4);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6da2c));
            }
            if (DG_INT(0x685e0) != 0 && DG_INT(0xa0) == 0)
            {
                if (P03_HELDOK)
                    setRect(f_6f0, 0x168, 0x77, 0x1b6, 0x89);
                else
                    setRect(f_6f0, 0x168, 0x64, 0x1b6, 0x76);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6da2c));
            }
            if (DG_SHORT(0x5f188) != -1)
            {
                switch (DG_SHORT(0x5f188))
                {
                case 0x110:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3808), DG_DR(0x6da0c));
                    break;
                case 0x111:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b380c), DG_DR(0x6da1c));
                    break;
                case 0x123:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b3808), DG_DR(0x6db3c));
                    break;
                case 0x124:
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b380c), DG_DR(0x6db4c));
                    break;
                }
            }
            else
            {
                if (DG_INT(0x685e0) != 0)
                {
                    setRect(f_6f0, 0x1af, 0x1b2, 0x1d6, 0x1d3);
                    DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6da0c));
                    setRect(f_6f0, 0x1e0, 0x1b2, 0x207, 0x1d3);
                    DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6da1c));
                }
                if (DG_INT(0x685dc) != 0)
                {
                    setRect(f_6f0, 0x1af, 0x1b2, 0x1d6, 0x1d3);
                    DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6db3c));
                    setRect(f_6f0, 0x1e0, 0x1b2, 0x207, 0x1d3);
                    DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6db4c));
                }
            }
        }

        // 0043F968
        if (DG_CHAR(0x100c) == 0)
        {
L_0043F971:
            if (DG_SHORT(0x5f188) != 0x21)
            {
                setRect(f_6f0, 0x8e, 0xe8, 0xdd, 0xfb);
                DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb1c));
            }
        }
        if (DG_SHORT(0x5f188) >= 0)
        {
            switch (DG_SHORT(0x5f188))
            {
            case 0x1e:
                if (DG_INT(0x8fb90) == 0 && DG_INT(0x685e0) == 0 && DG_INT(0x685dc) == 0 && DG_INT(0x685e4) == 0
                    && DG_INT(0x685e8) == 0)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b36d8), DG_DR(0x6caec));
                break;
            case 0x1f:
                if (DG_INT(0x8fb90) != 0 && DG_INT(0x685e0) == 0 && DG_INT(0x685dc) == 0 && DG_INT(0x685e4) == 0
                    && DG_INT(0x685e8) == 0)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b36dc), DG_DR(0x6cafc));
                break;
            case 0x21:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36e4), DG_DR(0x6cb1c));
                break;
            case 0x22:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b37c0), DG_DR(0x6cb2c));
                break;
            }
        }
        if (DG_SHORT(0x5f188) != 0x22)
        {
            setRect(f_6f0, 0x9d, 0x1c3, 0xcc, 0x1d5);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb2c));
        }
        if (DG_SHORT(0x5f188) != 0x1e)
        {
            if (DG_INT(0x8fb90) != 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36d8), DG_DR(0x6caec));
            else
            {
                setRect(f_6f0, 8, 0xc, 0x39, 0x1f);
                DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6caec));
            }
        }
        if (DG_SHORT(0x5f188) != 0x1f)
        {
            if (DG_INT(0x8fb90) != 0)
            {
                setRect(f_6f0, 0x3e, 0xc, 0x6e, 0x1f);
                DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cafc));
            }
            else
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36dc), DG_DR(0x6cafc));
        }
        if (DG_INT(0x8fb90) != 0)
        {
            // 0043FBF8: spell book page
            DG_DRAW(EQ_GLOBAL(void*, 0x006b36d8), DG_DR(0x6caec));
            DG_SLOT(0x89e44) = 0;
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3640), DG_DR(0x6cbac));
            if (DG_SHORT(0x5f188) == 0x28)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3638), DG_DR(0x6cb8c));
            else
            {
                setRect(f_6f0, 0x61, 0x23, 0x6b, 0x34);
                DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb8c));
            }
            if (DG_SHORT(0x5f188) == 0x29)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b363c), DG_DR(0x6cb9c));
            else
            {
                setRect(f_6f0, 0x61, 0x1c1, 0x6b, 0x1d2);
                DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb9c));
            }
            EQ_METHOD(0, void, 0x00465ade, (void*), self)();
        }
        else
        {
            // 0043FD2C: character stats page
            f_6e0 = EQ_METHOD(0, int, 0x0045755e, (void*), self)();
            f_6f4 = EQ_METHOD(0, int, 0x0041bc98, (void*), DG_PC)();
            f_6fc = EQ_METHOD(0, int, 0x0041ba44, (void*), DG_PC)();
            f_6d0 = EQ_METHOD(1, int, 0x004199c0, (void*, int), DG_PC)(0);
            f_6d4 = EQ_METHOD(1, int, 0x00419a2f, (void*, int), DG_PC)(0);
            f_6d8 = (short*)EQ_METHOD(0, int, 0x004575e4, (void*), self)();
            f_628 = (unsigned char)EQ_METHOD(0, int, 0x0041b258, (void*), DG_PC)();
            f_624 = (unsigned char)EQ_METHOD(0, int, 0x0041b0c3, (void*), DG_PC)();
            f_620 = (unsigned char)EQ_METHOD(0, int, 0x0041b722, (void*), DG_PC)();
            f_61c = (struct EQPlayer*)(unsigned char)EQ_METHOD(0, int, 0x0041b48f, (void*), DG_PC)();
            f_618 = (unsigned char)EQ_METHOD(0, int, 0x0041b5cc, (void*), DG_PC)();
            f_614 = (unsigned char)EQ_METHOD(0, int, 0x0041b38f, (void*), DG_PC)();
            f_610 = (unsigned char)EQ_METHOD(0, int, 0x0041b88a, (void*), DG_PC)();
            f_60c = EQ_METHOD(1, int, 0x0041af14, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_608 = EQ_METHOD(1, int, 0x0041a7a0, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_604 = EQ_METHOD(1, int, 0x0041ad1c, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_600 = EQ_METHOD(1, int, 0x0041a962, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_5fc = EQ_METHOD(1, int, 0x0041ab4b, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            if (DG_CHAR(0x8fbd2) == 0)
            {
                P03_STATS_STORE()
            }
            if (DG_CHAR(0x8fbd2) != DG_PC[0x38])
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbd4) != (short)f_6d0)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbd6) != (short)f_6d4)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbd8) != (short)(int)f_6d8)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbdc) != (short)f_6e0)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe2) != (int)f_628)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe0) != (int)f_624)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe4) != (int)f_620)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe6) != (int)f_61c)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe8) != (int)f_618)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbea) != (int)f_614)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbec) != (int)f_610)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbee) != f_60c)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbef) != f_608)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbf0) != f_604)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbf1) != f_600)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbf2) != f_5fc)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbf4) != (short)f_6f4)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbf6) != (short)f_6fc)
                DG_INT(0xeb8) = 1;
            P03_STATS_STORE()
            DG_INT(0x90194) = 0;
            DG_INT(0x90194) = 0;
            f_6f8 = DG_INT(0x90184) + 0xf0;
            f_63c = DG_INT(0x90188) + 0x14;
            DG_DRAW(EQ_GLOBAL(void*, 0x006b36dc), DG_DR(0x6cafc));
            DG_SLOT(0x89e48) = 0;
            DG_SLOT(0x8a3a0) = 0;
            DG_SPRINTF(f_6bc, (const char*)0x0053e678, DG_PC);
            f_5f4 = f_6f8 + 5;
            DG_TEXT(f_6bc, f_5f4, f_63c + 5, 7, DG_FONT);
            DG_SPRINTF(f_6bc, (const char*)0x0053e16c, *(unsigned char*)(DG_PC + 0x38));
            f_6dc = (short*)(f_63c + 0x12);
            DG_TEXT(f_6bc, f_5f4, (int)f_6dc, 7, DG_FONT);
            DG_TEXT(EQ_METHOD(1, char*, 0x004c9d4e, (void*, short), EQ_GLOBAL(void*, 0x006efe2c))(
                        (short)*(unsigned char*)(DG_PC + 0x36)),
                    f_6f8 + 0x16, (int)f_6dc, 7, DG_FONT);
            DG_TEXT(EQ_METHOD(1, char*, 0x004c9c6d, (void*, short), EQ_GLOBAL(void*, 0x006efe2c))(
                        *(short*)(DG_PC + 0x1038)),
                    f_5f4, f_63c + 0x1f, 7, DG_FONT);
            f_6dc = (short*)(int)EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(0x45);
            f_5f4 = (int)f_6dc + EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(0);
            f_6dc = (short*)(int)EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(0x37);
            f_5f8 = (int)f_6dc + EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(1);
            f_6dc = (short*)(int)EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(2);
            DG_TEXT(0x0055a03c, f_6f8 + 0xf, f_63c + 0x30, 7, DG_FONT);
            DG_SPRINTF(f_6bc, (const char*)0x0055a034, (int)(short)f_6d0, (int)(short)f_6d4);
            DG_TEXT(f_6bc, f_6f8 + 0x28, f_63c + 0x30, f_5f4 > 0 ? 0xe : (f_5f4 < 0 ? 0xd : 7), DG_FONT);
            DG_TEXT(0x0055a030, f_6f8 + 0xf, f_63c + 0x3d, 7, DG_FONT);
            DG_SPRINTF(f_6bc, (const char*)0x0055a028, (int)(short)(int)f_6d8);
            DG_TEXT(f_6bc, f_6f8 + 0x28, f_63c + 0x3d, f_5f8 > 0 ? 0xe : (f_5f8 < 0 ? 0xd : 7), DG_FONT);
            DG_TEXT(0x0055a024, f_6f8 + 0xf, f_63c + 0x4a, 7, DG_FONT);
            DG_SPRINTF(f_6bc, (const char*)0x0055a028, (int)(short)f_6e0);
            f_6e0 = 7;
            if ((int)f_6dc > 0)
                f_6e0 = 0xe;
            else if ((int)f_6dc < 0)
                f_6e0 = 0xd;
            DG_TEXT(f_6bc, f_6f8 + 0x28, f_63c + 0x4a, f_6e0, DG_FONT);
            DG_TEXT(0x0055a018, f_6f8 + 0x19, f_63c + 0x5a, f_6e0, DG_FONT);
            DG_EXP_PROGRESS();
            P03_STAT(0x0055a014, 0x78, f_624, 0x77)
            P03_STAT(0x0055a010, 0x87, f_628, 0x78)
            P03_STAT(0x0055a00c, 0x96, f_620, 0x7c)
            P03_STAT(0x0055a008, 0xa5, (int)f_61c, 0x7a)
            P03_STAT(0x0055a004, 0xb4, f_610, 0x7d)
            P03_STAT(0x0055a000, 0xc3, f_618, 0x7b)
            P03_STAT(0x00559ffc, 0xd2, f_614, 0x79)
            P03_RESIST(0x00559ff4, 0xe6, f_60c, EQ_METHOD(0, int, 0x0041aee8, (void*), DG_PC)())
            P03_RESIST(0x00559fec, 0xf5, f_608, EQ_METHOD(0, int, 0x0041a76d, (void*), DG_PC)())
            P03_RESIST(0x00559fe4, 0x104, f_604, EQ_METHOD(0, int, 0x0041aced, (void*), DG_PC)())
            P03_RESIST(0x00559fdc, 0x113, f_600, EQ_METHOD(0, int, 0x0041a92b, (void*), DG_PC)())
            P03_RESIST(0x00559fd4, 0x122, f_5fc, EQ_METHOD(0, int, 0x0041ab14, (void*), DG_PC)())
            DG_TEXT(0x00559fcc, f_6f8 + 0x20, f_63c + 0x136, 7, DG_FONT);
            DG_TEXT(0x00559fc0, f_6f8 + 0x14, f_63c + 0x143, 7, DG_FONT);
            DG_SPRINTF(f_6bc, (const char*)0x0055a034, (int)(short)f_6f4, (int)(short)f_6fc);
            DG_TEXT(f_6bc, f_6f8 + 0x23, f_63c + 0x150, (short)f_6f4 > (short)f_6fc ? 0xd : 7, DG_FONT);
        }

        // 00440D8E: bags
        if (DG_CHAR(0x100c) == 0)
        {
            f_6dc = (short*)8;
            f_6e0 = DG_THIS + 0x72e6c;
            do
            {
                EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)((int*)f_6e0);
                f_6e0 += 0x10;
            } while (P03_DEC(f_6dc));
            f_6fc = 0x16;
            f_6f4 = 0x138;
            f_6dc = (short*)8;
            do
            {
                if (*(char**)(f_6f4 + (int)DG_PC) != 0)
                {
                    if (*(short*)(*(char**)(f_6f4 + (int)DG_PC) + 0x84) >= 0x1f4
                        && *(short*)(*(char**)(f_6f4 + (int)DG_PC) + 0x84) <= 0xbba)
                        DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(f_6f4 + (int)DG_PC) + 0x84)), P03_SLOT(f_6fc, 0x6c90c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b3e30), P03_SLOT(f_6fc, 0x6c90c));
                    if (DG_COMBINE(*(char**)(f_6f4 + (int)DG_PC)) != 0)
                    {
                        P03_STACK(f_6fc, 0x6c90c, *(char**)(f_6f4 + (int)DG_PC), 0xf, 0xb, 0xa, 6)
                    }
                }
                f_6fc++;
                f_6f4 += 4;
            } while (P03_DEC(f_6dc));
        }
        if (DG_INT(0x685e0) != 0 && DG_INT(0x685dc) == 0 && DG_INT(0x6c658) != 0)
        {
            f_6e0 = 0x16;
            f_6d8 = (short*)0x138;
            f_6dc = (short*)8;
            do
            {
                if (P03_HELD != 0 && P03_HELD == *(int*)((int)f_6d8 + (int)DG_PC))
                {
                    P03_HILITE()
                    setRect(f_6cc, P03_RX(f_6e0, 0x6c90c) - 3, P03_RX(f_6e0, 0x6c910) - 3, 0x280, 0x1e0);
                    DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
                }
                f_6e0++;
                f_6d8 = (short*)((int)f_6d8 + 4);
            } while (P03_DEC(f_6dc));
        }

        // 0044104C: equipment slot frames and labels
        f_6d8 = 0;
        f_6e0 = DG_THIS + 0x72d1c;
        f_6dc = (short*)0x15;
        do
        {
            EQ_METHOD(1, void, 0x0046af87, (void*, int*), self)((int*)f_6e0);
            if (EQ_GLOBAL(int, 0x006e2074) == 1)
            {
                switch ((int)f_6d8)
                {
                case 1:
                    DG_STRCPY(f_6bc, 0x00559fb8);
                    break;
                case 2:
                    DG_STRCPY(f_6bc, 0x00559fb0);
                    break;
                case 0:
                case 3:
                    DG_STRCPY(f_6bc, 0x00559fac);
                    break;
                case 4:
                    DG_STRCPY(f_6bc, 0x00559fa4);
                    break;
                case 5:
                    DG_STRCPY(f_6bc, 0x00559f9c);
                    break;
                case 6:
                    DG_STRCPY(f_6bc, 0x00559f94);
                    break;
                case 7:
                    DG_STRCPY(f_6bc, 0x00558c74);
                    break;
                case 8:
                case 9:
                    DG_STRCPY(f_6bc, 0x00559f8c);
                    break;
                case 0xa:
                    DG_STRCPY(f_6bc, 0x00559f84);
                    break;
                case 0xb:
                    DG_STRCPY(f_6bc, 0x00559f7c);
                    break;
                case 0xc:
                    DG_STRCPY(f_6bc, 0x00559f78);
                    break;
                case 0xd:
                    DG_STRCPY(f_6bc, 0x00559f74);
                    break;
                case 0xe:
                case 0xf:
                    DG_STRCPY(f_6bc, 0x00559f6c);
                    break;
                case 0x10:
                    DG_STRCPY(f_6bc, 0x00559f64);
                    break;
                case 0x11:
                    DG_STRCPY(f_6bc, 0x00559f5c);
                    break;
                case 0x12:
                    DG_STRCPY(f_6bc, 0x00559f54);
                    break;
                case 0x13:
                    DG_STRCPY(f_6bc, 0x00559f4c);
                    break;
                case 0x14:
                    DG_STRCPY(f_6bc, 0x00559f44);
                    break;
                }
                DG_TEXT(f_6bc, *(int*)f_6e0 + 2, *(int*)(f_6e0 + 4) + 2, 10, DG_FONT3);
            }
            f_6d8 = (short*)((int)f_6d8 + 1);
            f_6e0 += 0x10;
        } while (P03_DEC(f_6dc));
        if (DG_CHAR(0x100c) != 0)
        {
            f_6d4 = 1;
            f_6d0 = 0x12e6;
            f_6dc = (short*)0x15;
            do
            {
                if (DG_SHORT(f_6d0) >= 0x1f4)
                    DG_DRAW(DG_ITEMIMG(DG_SHORT(f_6d0)), P03_SLOT(f_6d4, 0x6c90c));
                f_6d4++;
                f_6d0 += 2;
            } while (P03_DEC(f_6dc));
            return;
        }

        // 004411AB: worn equipment
        DG_TEXT(0x00559f38, DG_RX(0x6cb0c) + 0xa, DG_RY(0x6cb0c) + 0x32, 7, DG_FONT);
        DG_TEXT(0x00559f30, DG_RX(0x6cb0c) + 0x1b, DG_RY(0x6cb0c) + 0x41, 7, DG_FONT);
        f_6e0 = 1;
        f_6d8 = (short*)0xe4;
        f_6dc = (short*)5;
        do
        {
            if (*(char**)((int)f_6d8 + (int)DG_PC) != 0)
                DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)((int)f_6d8 + (int)DG_PC) + 0x84)), P03_SLOT(f_6e0, 0x6c90c));
            f_6e0++;
            f_6d8 = (short*)((int)f_6d8 + 4);
        } while (P03_DEC(f_6dc));
        DG_EQSLOT(0xfc, 0x6c97c)
        DG_EQSLOT(0x104, 0x6c99c)
        DG_EQSLOT(0x108, 0x6c9ac)
        DG_EQSLOT(0x110, 0x6c9cc)
        DG_EQSLOT(0x11c, 0x6c9fc)
        DG_EQSLOT(0x120, 0x6ca0c)
        DG_EQSLOT(0xf8, 0x6c96c)
        DG_EQSLOT(0x100, 0x6c98c)
        DG_EQSLOT(0x124, 0x6ca1c)
        DG_EQSLOT(0x128, 0x6ca2c)
        DG_EQSLOT(0x12c, 0x6ca3c)
        DG_EQSLOT(0x130, 0x6ca4c)
        DG_EQSLOT(0x114, 0x6c9dc)
        DG_EQSLOT(0x118, 0x6c9ec)
        if (*(char**)(DG_PC + 0x10c) != 0)
        {
            DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x10c) + 0x84)), DG_DR(0x6c9bc));
            if (DG_COMBINE(*(char**)(DG_PC + 0x10c)) != 0)
            {
                P03_STACK(0, 0x6c9bc, *(char**)(DG_PC + 0x10c), 0xf, 0xb, 0xa, 6)
            }
        }
        if (*(char**)(DG_PC + 0x134) != 0)
        {
            DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x134) + 0x84)), DG_DR(0x6ca5c));
            if (DG_COMBINE(*(char**)(DG_PC + 0x134)) != 0)
            {
                P03_STACK(0, 0x6ca5c, *(char**)(DG_PC + 0x134), 0xf, 0xb, 0xa, 6)
            }
        }
        P03_COIN(0x00559f24, 0x6cb3c, *(int*)(DG_PC + 0x998))
        P03_COIN(0x00559f1c, 0x6cb4c, *(int*)(DG_PC + 0x99c))
        P03_COIN(0x00559f14, 0x6cb5c, *(int*)(DG_PC + 0x9a0))
        P03_COIN(0x00559f0c, 0x6cb6c, *(int*)(DG_PC + 0x9a4))
        if (DG_INT(0x685e0) != 0 && DG_INT(0x6c658) != 0)
        {
            f_6d4 = 1;
            f_6e0 = 0xe4;
            f_6dc = (short*)0x15;
            do
            {
                if (P03_HELD != 0 && P03_HELD == *(int*)(f_6e0 + (int)DG_PC))
                {
                    P03_HILITE()
                    setRect(f_6cc, P03_RX(f_6d4, 0x6c90c) - 3, P03_RX(f_6d4, 0x6c910) - 3, 0x280, 0x1e0);
                    DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
                }
                f_6e0 += 4;
                f_6d4++;
            } while (P03_DEC(f_6dc));
        }
    }

    // 00441AC2
    DG_INT(0x90274) = 0;
    if (DG_INT(0x90278) == 0)
        return;
    EQ_METHOD(0, void, 0x0043aca9, (void*), self)();
    return;
#undef P03_RX
#undef P03_SLOT
#undef P03_COUNT
#undef P03_STACK
#undef P03_ITEMLOOP
#undef P03_CMP3
#undef P03_NUM
#undef P03_BANK
#undef P03_STAT
#undef P03_RESIST
#undef P03_COIN
#undef P03_HILITE
#undef P03_HELD
#undef P03_HELDOK
#undef P03_DEC
#undef P03_STATS_STORE
    // END REGION P03

    // REGION P04 0x00441B4D-0x00446D83: 14531 B, arm of JLE @00441B43 (taken): CMP AL,0x3
    //   defines L_00441B4D, L_004458F2, L_0044612A, L_00446815; exits L_00443F7F, L_00446D83
    //   switch scaffold for this piece (paste where the dispatch is):
    //   // SWITCH @00441E74 (in P04): table 0x0044FE9D, 9 entries (bytetable), bias -210; selector: ADD EAX,0xffffff2e ; CMP EAX,0xf ; JA caseD_d4 ; MOVZX EAX,byte ptr [EAX + switchdataD_0044fec1] ; JMP dword ptr [EAX*0x4 + switchdataD_0044fe9d]
    //   switch (/* TODO selector (register EAX) */ 0) {
    //   case 0xD2: goto L_00441F1F; /* P04 */
    //   case 0xD3: goto L_00441F3D; /* P04 */
    //   case 0xD4: case 0xD5: case 0xD6: case 0xD7: case 0xD8: case 0xD9: case 0xDA: case 0xDB: goto L_00441F7C; /* P04 */
    //   case 0xDC: goto L_00441E7B; /* P04 */
    //   case 0xDD: goto L_00441E9C; /* P04 */
    //   case 0xDE: goto L_00441EBD; /* P04 */
    //   case 0xDF: goto L_00441EDE; /* P04 */
    //   case 0xE0: goto L_00441F01; /* P04 */
    //   case 0xE1: goto L_00441F5B; /* P04 */
    //   default: goto L_00441F7C; /* P04 */
    //   }
    //   switch scaffold for this piece (paste where the dispatch is):
    //   // SWITCH @0044212D (in P04): table 0x0044FED1, 8 entries (cmp), bias -1; selector: MOVZX EAX,byte ptr [ESI + 0x8fbac] ; DEC EAX ; CMP EAX,0x7 ; JA default ; JMP dword ptr [EAX*0x4 + switchdataD_0044fed1]
    //   switch (/* TODO selector (register EAX) */ 0) {
    //   case 1: goto L_00442134; /* P04 */
    //   case 2: goto L_004423D5; /* P04 */
    //   case 3: goto L_00442EF2; /* P04 */
    //   case 4: goto L_00443AAB; /* P04 */
    //   case 5: goto L_00443F7F; /* P05 */
    //   case 6: goto L_00445924; /* P04 */
    //   case 7: goto L_00446166; /* P04 */
    //   case 8: goto L_0044636C; /* P04 */
    //   default: goto L_00446D83; /* P01 */
    //   }
    // P04: the side panels (target / player names, buttons, vials) and the options pages
    // (main+0x8fbac = page 1..8), or the spell gems when no page is open. EBP is 0 at entry;
    // f_6d4 is the EBP loop index of the gem loops; carryEax at L_0044612A = selector - 0x6f.
#define P4_IMG EQ_GLOBAL(int*, 0x006b341c)
#define P4_SETIMG() *P4_IMG = (int)DG_OVL(0x006e4e1c)
#define P4_RECT(a, b, c, d) setRect(P4_IMG + 1, a, b, c, d)
#define P4_DRAW(off) DG_DRAW(P4_IMG, DG_DR(off))
#define P4_T(text, off, dx, dy, color) DG_TEXT(text, DG_RX(off) + (dx), DG_RY(off) + (dy), color, DG_FONTO)
#define P4_ONOFF(flag, off) \
    if ((flag) != 0) \
        P4_T(0x00559eb4, off, 8, -1, 7); \
    else \
        P4_T(0x00559eb0, off, 5, -1, 0xd);
#define P4_SEL(off, img) \
    setRect(f_6f0, DG_RX(off) + 1, DG_RY(off), 0x280, 0x1e0); \
    DG_DRAW(img, f_6f0);
#define P4_SLIDE(x, off) \
    setRect(f_6f0, (short)(x) + 0x1c, DG_RX(off) - 1, 0x280, 0x1e0); \
    DG_DRAW(EQ_GLOBAL(void*, 0x006b3640), f_6f0);
#define P4_TAB() \
    setRect(f_6f0, 9, 10, 0x6d, 0x1b); \
    DG_DRAW(P4_IMG, f_6f0);
#define P4_SLOT(i, off) ((DG_CHAR(0xfe0) * 400 + (i)) * 0x10 + (off) + DG_THIS)
#define P4_SLOT4(i, off) DG_INT((DG_CHAR(0xfe0) * 400 + (i)) * 4 + (off))
#define P4_SPSLOT (*(short*)((char*)self + (DG_CHAR(0x6841d) * 10 + f_6d4) * 4 + 0x68420))
#define P4_SPELLIMG(id) (*(short*)(EQ_ARRAY(char*, 0x005fe9a0)[id] + 0x1fc))
L_00441B4D:
    if (DG_INT(0xc) != 0)
        EQ_METHOD(1, void, 0x00450081, (void*, int), self)(1);
    else if (EQ_GLOBAL(int, 0x006b676c) == 0)
        DG_UPV(EQ_GLOBAL(char*, 0x006e4e48) + 0xa4, EQ_GLOBAL(char*, 0x006e4e48) + 0xa4);
    if ((DG_SLOT(0x843c4) != 0 || DG_SLOT(0x8a184) != 0) && EQ_GLOBAL(int, 0x006e4de8) != 0)
    {
        DG_SLOT(0x8a184) = 0;
        *(int*)(EQ_GLOBAL(char*, 0x006e4de8) + 0x1ec) = 1;
    }
    if (DG_INT(0xc) != 0 || DG_SLOT(0x843fc) != 0)
    {
        DG_UPV(DG_DR(0x6d8cc), DG_DR(0x6d8cc));
        DG_STRCPY(f_6bc, DG_TRUNC(EQ_GLOBAL(char*, 0x006e4d8c) + 1, 0x4b));
        if (EQ_GLOBAL(int, 0x006e4dac) != 0 && EQ_GLOBAL(int, 0x006e4dac) == EQ_GLOBAL(int, 0x006e4d8c))
        {
            DG_TEXT(f_6bc, 0x21d, 0xd, 10, DG_FONT);
            DG_STRCPY(f_6bc, DG_TRUNC(DG_PC + 0x1e, 0x4b));
            DG_TEXT(f_6bc, 0x21d, 0x19, 10, DG_FONT);
        }
        else if (EQ_CDECL(int, 0x00522920, (const char*, const char*))(DG_PC, EQ_ARRAY(char, 0x005fe968)) == 0)
        {
            DG_TEXT(f_6bc, 0x21d, 0xd, 0xf, DG_FONT);
            DG_STRCPY(f_6bc, DG_TRUNC(DG_PC + 0x1e, 0x4b));
            DG_TEXT(f_6bc, 0x21d, 0x19, 0xf, DG_FONT);
        }
        else
        {
            DG_TEXT(f_6bc, 0x21d, 0xd, 7, DG_FONT);
            DG_STRCPY(f_6bc, DG_TRUNC(DG_PC + 0x1e, 0x4b));
            DG_TEXT(f_6bc, 0x21d, 0x19, 7, DG_FONT);
        }
    }
    // 00441D28
    if (DG_SHORT(0x5f188) != 0xe1 && (DG_INT(0xc) != 0 || DG_SLOT(0x84390) != 0 || DG_SHORT(0x5f18a) == 0xe1))
    {
        if (EQ_GLOBAL(char, 0x006b658c) != 0)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3618), DG_DR(0x6d71c));
        else
            DG_UPV(DG_DR(0x6d71c), DG_DR(0x6d71c));
    }
    EQ_METHOD(0, void, 0x00468df1, (void*), self)();
    if ((DG_INT(0xc) != 0 || DG_SLOT(0x84394) != 0 || DG_SLOT(0x8a154) != 0) && EQ_GLOBAL(char*, 0x006e4dac) != 0)
    {
        DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4dac) + 1));
        DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
        DG_TEXT(f_6bc, 0x215, DG_RX(0x6d730), 10, DG_FONT);
    }
    // 00441E3F: the pressed button
    if (DG_SHORT(0x5f188) >= 0 && DG_SHORT(0x5f188) != DG_SHORT(0x5f18a))
    {
        switch (DG_SHORT(0x5f188))
        {
        case 0xdc:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3658), DG_DR(0x6d6cc));
            break;
        case 0xdd:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3654), DG_DR(0x6d6dc));
            break;
        case 0xde:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b365c), DG_DR(0x6d6ec));
            break;
        case 0xdf:
            setRect(f_6f0, 0xc, 0x101, 0x50, 0x112);
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3660), f_6f0);
            break;
        case 0xe0:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3664), DG_DR(0x6d70c));
            break;
        case 0xd2:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b38a8), DG_DR(0x6d62c));
            break;
        case 0xd3:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b38ac), DG_DR(0x6d63c));
            break;
        case 0xe1:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3618), DG_DR(0x6d71c));
            break;
        }
    }
    // 00441F7C: the hovered button
    if (DG_SHORT(0x5f18a) >= 0)
    {
        switch (DG_SHORT(0x5f18a))
        {
        case 0xd2:
            DG_UPV(DG_DR(0x6d62c), DG_DR(0x6d62c));
            break;
        case 0xd3:
            DG_UPV(DG_DR(0x6d63c), DG_DR(0x6d63c));
            break;
        case 0xdc:
            DG_UPV(DG_DR(0x6d6cc), DG_DR(0x6d6cc));
            break;
        case 0xdd:
            DG_UPV(DG_DR(0x6d6dc), DG_DR(0x6d6dc));
            break;
        case 0xde:
            DG_UPV(DG_DR(0x6d6ec), DG_DR(0x6d6ec));
            break;
        case 0xdf:
            DG_UPV(DG_DR(0x6d6fc), DG_DR(0x6d6fc));
            break;
        case 0xe0:
            DG_UPV(DG_DR(0x6d70c), DG_DR(0x6d70c));
            break;
        }
    }
    DG_INT(0x5f16c) = 0;
    if (DG_SHORT(0x8fbb4) != 0)
        DG_INT(0x5f16c) = 1;
    if (DG_INT(0x685f4) != 0)
    {
        DG_INT(0x5f16c) = 1;
        EQ_METHOD(0, void, 0x0043aca9, (void*), self)();
        goto L_00446D83;
    }
    if (DG_CHAR(0x8fbac) != 0)
    {
        *(int*)(EQ_GLOBAL(char*, 0x006e4de8) + 0x1ec) = 1;
        DG_STRCPY(f_5f0, 0x00559ef4);
        if (DG_INT(0xc) != 0 || DG_SLOT(0x841c0) != 0)
        {
            DG_INT(0x5f16c) = 1;
            P4_SETIMG();
            P4_RECT(0, 0, 0x77, 0x1e0);
            P4_DRAW(0x6cfdc);
        }
        switch ((unsigned char)DG_CHAR(0x8fbac))
        {
        case 1: // 00442134: the page tabs
            if (DG_INT(0x5f16c) == 0)
                break;
            P4_SETIMG();
            P4_RECT(0x77, 0, 0xdd, 0x12);
            P4_DRAW(0x6cf7c);
            P4_RECT(0x77, 0x13, 0xdd, 0x25);
            P4_DRAW(0x6cf8c);
            P4_RECT(0x77, 0x26, 0xdd, 0x38);
            P4_DRAW(0x6cf9c);
            P4_RECT(0x77, 0x39, 0xdd, 0x4b);
            P4_DRAW(0x6cfac);
            P4_RECT(0x77, 0x4c, 0xdd, 0x5e);
            P4_DRAW(0x6cfbc);
            P4_RECT(0x77, 0xfa, 0xa7, 0x10c);
            P4_DRAW(0x6cfec);
            if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
                break;
            switch (DG_SHORT(0x5f188))
            {
            case 0x67:
                P4_RECT(0xdd, 1, 0x143, 0x12);
                P4_DRAW(0x6cf7c);
                break;
            case 0x68:
                P4_RECT(0xdd, 0x13, 0x143, 0x25);
                P4_DRAW(0x6cf8c);
                break;
            case 0x69:
                P4_RECT(0xdd, 0x26, 0x143, 0x38);
                P4_DRAW(0x6cf9c);
                break;
            case 0x6a:
                P4_RECT(0xdd, 0x39, 0x143, 0x4b);
                P4_DRAW(0x6cfac);
                break;
            case 0x6b:
                P4_RECT(0xdd, 0x4c, 0x143, 0x5e);
                P4_DRAW(0x6cfbc);
                break;
            case 0x6e:
                goto L_00446815;
            }
            break;

        case 2: // 004423D5: sound / interface options
            if (DG_INT(0x5f16c) == 0)
                break;
            P4_SETIMG();
            P4_RECT(0x78, 0x80, 0xdc, 0x91);
            P4_TAB();
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d16c);
            P4_RECT(0x77, 0x5f, 0xdd, 0x6c);
            P4_DRAW(0x6d17c);
            P4_DRAW(0x6d25c);
            P4_RECT(0x77, 0xdf, 0x95, 0xec);
            P4_DRAW(0x6d18c);
            P4_RECT(0x77, 0xdf, 0x95, 0xec);
            P4_DRAW(0x6d19c);
            P4_RECT(0x78, 0xed, 0xdf, 0xf9);
            P4_DRAW(0x6d1ac);
            P4_SLIDE(((int)EQ_GLOBAL(short, 0x006b6894) * 0x36) / 0x14, 0x6d1d0);
            P4_RECT(0x78, 0xed, 0xdf, 0xf9);
            P4_DRAW(0x6d1dc);
            P4_SLIDE(((int)EQ_GLOBAL(short, 0x006b688c) * 0x36) / 0x14, 0x6d200);
            P4_RECT(0x78, 0xed, 0xdf, 0xf9);
            P4_DRAW(0x6d20c);
            P4_SLIDE(((int)EQ_GLOBAL(short, 0x006b6890) * 0x36) / 0x14, 0x6d230);
            P4_RECT(0x77, 0xfa, 0xa7, 0x10c);
            P4_DRAW(0x6cfec);
            P4_T(0x00559ee4, 0x6d1ac, 3, -0xf, 10);
            P4_T(0x00559ed8, 0x6d1dc, 0xf, -0xf, 10);
            P4_T(0x00559ecc, 0x6d20c, 0xf, -0xf, 10);
            P4_T(0x00559ec0, 0x6d16c, -0xc, -0x19, 10);
            P4_T(0x00559eb8, 0x6d16c, -9, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b68ac), 0x6d16c)
            P4_T(0x00559ea8, 0x6d17c, 0x21, -0xf, 10);
            if (EQ_GLOBAL(int, 0x006b6868) == 0)
                P4_T(0x00559e9c, 0x6d17c, 0x15, -1, 0xd);
            else if (EQ_GLOBAL(int, 0x006b6868) == 1)
                P4_T(0x00559e90, 0x6d17c, 0x12, -1, 7);
            else if (EQ_GLOBAL(int, 0x006b6868) == 2)
                P4_T(0x00559e84, 0x6d17c, 0x17, -1, 7);
            P4_T(0x00559e78, 0x6d18c, -0x12, -0x12, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6860), 0x6d18c)
            P4_T(0x00559e6c, 0x6d19c, -0xf, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6864), 0x6d19c)
            P4_RECT(0x77, 0xdf, 0x95, 0xec);
            P4_DRAW(0x6d23c);
            P4_T(0x00559e5c, 0x6d23c, -0x1b, -0x12, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b686c), 0x6d23c)
            P4_RECT(0x77, 0xdf, 0x95, 0xec);
            P4_DRAW(0x6d24c);
            P4_T(0x00559e4c, 0x6d24c, -0x21, -0x12, 10);
            P4_ONOFF(*(int*)(DG_PC + 0xab4), 0x6d24c)
            P4_T(0x00559e3c, 0x6d25c, 5, -0x12, 10);
            if (EQ_GLOBAL(int, 0x006b6efc) == 1)
                P4_T(0x00559e30, 0x6d25c, 0x15, -1, 0xd);
            else if (EQ_GLOBAL(int, 0x006b6efc) == 0)
                P4_T(0x00559e28, 0x6d25c, 0x1a, -1, 7);
            else
                P4_T(0x00559e20, 0x6d25c, 0x20, -1, 7);
            if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
                break;
            switch (DG_SHORT(0x5f188))
            {
            case 0x6e:
                goto L_00446815;
            case 0x8a:
                P4_SEL(0x6d1ac, EQ_GLOBAL(void*, 0x006b38a8))
                break;
            case 0x8b:
                P4_SEL(0x6d1bc, EQ_GLOBAL(void*, 0x006b38ac))
                break;
            case 0x8d:
                P4_SEL(0x6d1dc, EQ_GLOBAL(void*, 0x006b38a8))
                break;
            case 0x8e:
                P4_SEL(0x6d1ec, EQ_GLOBAL(void*, 0x006b38ac))
                break;
            case 0x90:
                P4_SEL(0x6d20c, EQ_GLOBAL(void*, 0x006b38a8))
                break;
            case 0x91:
                P4_SEL(0x6d21c, EQ_GLOBAL(void*, 0x006b38ac))
                break;
            }
            break;

        case 3: // 00442EF2: video options
            if (DG_INT(0x5f16c) == 0)
                break;
            P4_SETIMG();
            P4_RECT(0x78, 0x93, 0xdc, 0xa5);
            P4_TAB();
            P4_RECT(0x78, 0xed, 0xdf, 0xf9);
            P4_DRAW(0x6d26c);
            P4_SLIDE((short)EQ_GLOBAL(float, 0x006b685c) * 0x36 / 8, 0x6d290);
            P4_RECT(0x77, 0x5f, 0xdd, 0x6d);
            P4_DRAW(0x6d29c);
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d2ac);
            P4_RECT(0x77, 0xfa, 0xa7, 0x10c);
            P4_DRAW(0x6cffc);
            P4_RECT(0xa7, 0xfa, 0xd7, 0x10c);
            P4_DRAW(0x6d00c);
            P4_T(0x00559e18, 0x6d26c, 0x1e, -0x19, 10);
            P4_T(0x00559e0c, 0x6d26c, 0xf, -0xf, 10);
            P4_T(0x00559e08, 0x6d29c, 0x23, -0xf, 10);
            switch (EQ_GLOBAL(int, 0x006b6870))
            {
            case 0:
                P4_T(0x00559ddc, 0x6d29c, 0x14, -1, 0xd);
                break;
            case 1:
                P4_T(0x00559de8, 0x6d29c, 0xc, -1, 7);
                break;
            case 2:
                P4_T(0x00559df8, 0x6d29c, 0xa, -1, 7);
                break;
            }
            P4_T(0x00559dd0, 0x6d2ac, -0xa, -0x19, 10);
            P4_T(0x00559dc8, 0x6d2ac, -6, -0xf, 10);
            switch (EQ_GLOBAL(int, 0x006b6874))
            {
            case 0:
                P4_T(0x00559eb0, 0x6d2ac, 5, -1, 0xd);
                break;
            case 1:
                P4_T(0x00559db8, 0x6d2ac, 4, -1, 7);
                break;
            case 2:
                P4_T(0x00559dbc, 0x6d2ac, 5, -1, 7);
                break;
            case 3:
                P4_T(0x00559dc0, 0x6d2ac, 2, -1, 7);
                break;
            }
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d2dc);
            P4_DRAW(0x6d2ec);
            P4_T(0x00559dac, 0x6d2dc, -0xa, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6884), 0x6d2dc)
            P4_T(0x00559da0, 0x6d2ec, -0xc, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6888), 0x6d2ec)
            P4_RECT(0x77, 0x5f, 0xdd, 0x6c);
            P4_DRAW(0x6d2fc);
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d30c);
            P4_T(0x00559d94, 0x6d2fc, 0xf, -0xf, 10);
            if (EQ_GLOBAL(int, 0x006e20c0) == 0)
                P4_T(0x00559d88, 0x6d2fc, 0x1b, -1, 7);
            else if (EQ_GLOBAL(int, 0x006e20c0) == 1)
                P4_T(0x00559d7c, 0x6d2fc, 0x1b, -1, 7);
            else if (EQ_GLOBAL(int, 0x006e20c0) == 2)
                P4_T(0x00559d70, 0x6d2fc, 0x1d, -1, 7);
            else if (EQ_GLOBAL(int, 0x006e20c0) == 3)
                P4_T(0x00559d64, 0x6d2fc, 0x1d, -1, 7);
            else if (EQ_GLOBAL(int, 0x006e20c0) == 4)
                P4_T(0x00559d58, 0x6d2fc, 0x1d, -1, 7);
            P4_T(0x00559d48, 0x6d30c, -0x1a, -0x19, 10);
            P4_T(0x00559d3c, 0x6d30c, -0x14, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6880), 0x6d30c)
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d31c);
            P4_T(0x00559d30, 0x6d31c, -0xc, -0x19, 10);
            P4_T(0x00559d28, 0x6d31c, -6, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6878), 0x6d31c)
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d32c);
            P4_T(0x00559d20, 0x6d32c, -9, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b68b0), 0x6d32c)
            if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
                break;
            switch (DG_SHORT(0x5f188))
            {
            case 0x6f:
                goto L_004458F2;
            case 0x70:
                P4_RECT(0xa7, 0x10d, 0xd7, 0x11f);
                P4_DRAW(0x6d00c);
                break;
            case 0x96:
                P4_SEL(0x6d26c, EQ_GLOBAL(void*, 0x006b38a8))
                break;
            case 0x97:
                P4_SEL(0x6d27c, EQ_GLOBAL(void*, 0x006b38ac))
                break;
            }
            break;

        case 4: // 00443AAB: mouse options
            if (DG_INT(0x5f16c) == 0)
                break;
            P4_SETIMG();
            P4_RECT(0x78, 0xa6, 0xdc, 0xb8);
            P4_TAB();
            P4_RECT(0x78, 0xed, 0xdf, 0xf9);
            P4_DRAW(0x6d35c);
            P4_SLIDE((short)(EQ_GLOBAL(int, 0x006b689c) - 1) * 0x36 / 7, 0x6d360);
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d37c);
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d38c);
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            P4_DRAW(0x6d39c);
            P4_RECT(0x77, 0xfa, 0xa7, 0x10c);
            P4_DRAW(0x6cfec);
            P4_T(0x00559d18, 0x6d35c, 0x1e, -0x19, 10);
            P4_T(0x00559d0c, 0x6d35c, 0xf, -0xf, 10);
            P4_T(0x00559cfc, 0x6d37c, -0x14, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b68a0), 0x6d37c)
            P4_T(0x00559cf0, 0x6d38c, -0x19, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b68a4), 0x6d38c)
            P4_T(0x00559ce4, 0x6d39c, -0x16, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b68a8), 0x6d39c)
            if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
                break;
            switch (DG_SHORT(0x5f188))
            {
            case 0x6e:
                goto L_00446815;
            case 0xa5:
                P4_SEL(0x6d35c, EQ_GLOBAL(void*, 0x006b38a8))
                break;
            case 0xa6:
                P4_SEL(0x6d36c, EQ_GLOBAL(void*, 0x006b38ac))
                break;
            }
            break;

        case 5:
            goto L_00443F7F;

        L_004458F2: // the second page button (0x6f); EBP = 0xa7 here
            P4_RECT(0x77, 0x10d, 0xa7, 0x11f);
            P4_DRAW(0x6cffc);
            break;

        case 6: // 00445924: chat filters
            if (DG_INT(0x5f16c) == 0)
                break;
            P4_SETIMG();
            P4_RECT(0x78, 0xcc, 0xdd, 0xde);
            P4_TAB();
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            for (f_6d4 = 0x72; f_6d4 < 0x7c; f_6d4++)
                DG_DRAW(P4_IMG, P4_SLOT(f_6d4, 0x6c90c));
            P4_RECT(0x77, 0xfa, 0xa7, 0x10c);
            P4_DRAW(0x6cffc);
            P4_RECT(0xa7, 0xfa, 0xd7, 0x10c);
            P4_DRAW(0x6d00c);
            P4_T(0x00559800, 0x6d02c, -0x12, -0xf, 10);
            P4_T(0x0055998c, 0x6d03c, -8, -0xf, 10);
            P4_T(0x005597f4, 0x6d04c, -0x12, -0xf, 10);
            P4_T(0x005597ec, 0x6d05c, -7, -0xf, 10);
            P4_T(0x005597e0, 0x6d06c, -0xf, -0xf, 10);
            P4_T(0x005597d0, 0x6d07c, -0x25, -0xf, 10);
            P4_T(0x005597c4, 0x6d08c, -0xf, -0xf, 10);
            P4_T(0x005597b4, 0x6d09c, -0x1d, -0xf, 10);
            P4_T(0x005597a8, 0x6d0ac, -0x15, -0xf, 10);
            P4_T(0x00559798, 0x6d0bc, -0x22, -0xf, 10);
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6984), 0x6d02c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6988), 0x6d03c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b698c), 0x6d04c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6990), 0x6d05c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6994), 0x6d06c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b6998), 0x6d07c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b699c), 0x6d08c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b69a0), 0x6d09c)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b69a4), 0x6d0ac)
            P4_ONOFF(EQ_GLOBAL(int, 0x006b69a8), 0x6d0bc)
            if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
                break;
            carryEax = DG_SHORT(0x5f188) - 0x6f;
            if (carryEax == 0)
                goto L_004458F2;
        L_0044612A:
            if (--carryEax != 0)
                break;
            P4_RECT(0xa7, 0x10d, 0xd7, 0x11f);
            P4_DRAW(0x6d00c);
            break;

        case 7: // 00446166: clip plane
            P4_SETIMG();
            P4_RECT(0x78, 0x93, 0xdc, 0xa5);
            P4_TAB();
            P4_T(0x0055978c, 0x6d33c, 0xf, -0xf, 10);
            P4_RECT(0x78, 0xed, 0xdf, 0xf9);
            P4_DRAW(0x6d33c);
            P4_SLIDE(((int)EQ_GLOBAL(short, 0x006b68b8) * 0x36) / 0x14, 0x6d340);
            P4_RECT(0x77, 0xfa, 0xa7, 0x10c);
            P4_DRAW(0x6cfec);
            if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
                break;
            switch (DG_SHORT(0x5f188))
            {
            case 0x6e:
                goto L_00446815;
            case 0xa3:
                P4_SEL(0x6d33c, EQ_GLOBAL(void*, 0x006b38a8))
                break;
            case 0xa4:
                P4_SEL(0x6d34c, EQ_GLOBAL(void*, 0x006b38ac))
                break;
            }
            break;

        case 8: // 0044636C: spell message filters
            P4_SETIMG();
            P4_RECT(0x78, 0xcc, 0xdd, 0xde);
            P4_TAB();
            P4_RECT(0x77, 0xdf, 0x96, 0xed);
            for (f_6d4 = 0x72; f_6d4 < 0x76; f_6d4++)
                DG_DRAW(P4_IMG, P4_SLOT(f_6d4, 0x6c90c));
            P4_T(0x00559780, 0x6d02c, -0x12, -0xf, 10);
            P4_T(0x00559774, 0x6d03c, -0x14, -0xf, 10);
            P4_T(0x00559768, 0x6d04c, -0x14, -0xf, 10);
            P4_T(0x00559758, 0x6d05c, -0x23, -0xf, 10);
            switch (EQ_GLOBAL(int, 0x006b6ee4))
            {
            case 0:
                P4_T(0x0055974c, 0x6d02c, 5, -1, 7);
                break;
            case 1:
                P4_T(0x00559750, 0x6d02c, 6, -1, 7);
                break;
            case 2:
                P4_T(0x00559754, 0x6d02c, 4, -1, 7);
                break;
            case 3:
                P4_T(0x00559eb0, 0x6d02c, 5, -1, 7);
                break;
            }
            if (EQ_GLOBAL(int, 0x006b6ee8) != 0)
                P4_T(0x00559eb0, 0x6d03c, 5, -1, 0xd);
            else
                P4_T(0x00559eb4, 0x6d03c, 8, -1, 7);
            switch (EQ_GLOBAL(int, 0x006b6eec))
            {
            case 0:
                P4_T(0x0055974c, 0x6d04c, 5, -1, 7);
                break;
            case 1:
                P4_T(0x00559750, 0x6d04c, 6, -1, 7);
                break;
            case 2:
                P4_T(0x00559754, 0x6d04c, 4, -1, 7);
                break;
            case 3:
                P4_T(0x00559eb0, 0x6d04c, 5, -1, 7);
                break;
            }
            if (EQ_GLOBAL(int, 0x006b6f00) != 0)
                P4_T(0x00559eb0, 0x6d05c, 5, -1, 0xd);
            else
                P4_T(0x00559eb4, 0x6d05c, 8, -1, 7);
            P4_RECT(0x77, 0xfa, 0xa7, 0x10c);
            P4_DRAW(0x6cfec);
            if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
                break;
            if (DG_SHORT(0x5f188) != 0x6e)
                break;
        L_00446815: // the first page button (0x6e); EBP = 0xa7 here
            P4_RECT(0x77, 0x10d, 0xa7, 0x11f);
            P4_DRAW(0x6cfec);
            break;
        }
        goto L_00446D83;
    }
    // 00446847: no options page: the spell gems
    DG_STRCPY(f_5f0, 0x00559740);
    if (DG_CHAR(0xfe0) != 5)
        EQ_METHOD(0, void, 0x00465128, (void*), self)();
    if (DG_PC[0x36] == 1 || DG_PC[0x36] == 7 || DG_PC[0x36] == 9)
    {
        setRect(f_6f0, 0x127, 10, 0x16e, 0x1c);
        setRect(f_6cc, 10, 0x102, 0x280, 0x1e0);
        DG_PART(DG_OVL(0x006e4e40), f_6f0, f_6cc);
    }
    for (f_6d4 = 0; f_6d4 < 8; f_6d4++)
    {
        if (DG_PCSPELL(f_6d4) != -1)
        {
            if (DG_SHORT(0x6c594 + f_6d4 * 2) != DG_PCSPELL(f_6d4) || DG_INT(0xc) != 0
                || P4_SLOT4(f_6d4, 0x8435c) != 0 || DG_UINT(0xe60 + f_6d4 * 4) != 0)
            {
                DG_UPV(P4_SLOT(f_6d4, 0x6d64c), P4_SLOT(f_6d4, 0x6d64c));
                if (DG_UINT(0xe60 + f_6d4 * 4) > 0 && DG_UINT(0xe60 + f_6d4 * 4) < 8)
                {
                    DG_GEM_DRAW(DG_UINT(0xe60 + f_6d4 * 4), P4_SLOT(f_6d4, 0x6d64c))
                }
                else
                    DG_DRAWIMAGE(P4_SLOT(f_6d4, 0x6d64c), P4_SPELLIMG(DG_PCSPELL(f_6d4)));
                DG_SHORT(0x6c594 + f_6d4 * 2) = DG_PCSPELL(f_6d4);
            }
        }
    }
    if (DG_INT(0xc) != 0 || DG_SLOT(0x84014) != 0)
    {
        DG_PART(DG_OVL(0x006e4e44), DG_DR(0x6c92c), DG_DR(0x6c92c));
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, DG_CHAR(0x6841d) + 1);
        DG_TEXT(f_6bc, DG_CHAR(0x6841d) < 9 ? 0x3b : 0x37, 0x134, 10, DG_FONT);
    }
    for (f_6d4 = 0; f_6d4 < 6; f_6d4++)
    {
        if (DG_CHAR((DG_CHAR(0x6841d) * 10 + f_6d4) * 4 + 0x6841e) == 7)
        {
            if (DG_INT(0xc) != 0 || P4_SLOT4(f_6d4, 0x8432c) != 0 || P4_SLOT4(f_6d4, 0x8a0ec) != 0
                || DG_SHORT(0x6c594 + P4_SPSLOT * 2) != DG_PCSPELL(P4_SPSLOT) || DG_INT(0xe60 + P4_SPSLOT * 4) != 0)
            {
                setRect(f_6f0, 0x51, 0x2d, 0x78, 0x54);
                DG_UPV(f_6f0, P4_SLOT(f_6d4, 0x6d58c));
                if (DG_PCSPELL(P4_SPSLOT) != -1)
                {
                    setRect(f_6f0, *(int*)P4_SLOT(f_6d4, 0x6d58c) + 7, *(int*)P4_SLOT(f_6d4, 0x6d590) + 9,
                            *(int*)P4_SLOT(f_6d4, 0x6d58c) + 0x64, *(int*)P4_SLOT(f_6d4, 0x6d590) + 0x64);
                    if (DG_UINT(0xe60 + P4_SPSLOT * 4) > 0 && DG_UINT(0xe60 + P4_SPSLOT * 4) < 8)
                    {
                        DG_GEM_DRAW(DG_UINT(0xe60 + P4_SPSLOT * 4), f_6f0)
                    }
                    else
                        DG_DRAWIMAGE(f_6f0, P4_SPELLIMG(DG_PCSPELL(P4_SPSLOT)));
                }
            }
            DG_SHORT(0x6c594 + P4_SPSLOT * 2) = DG_PCSPELL(P4_SPSLOT);
        }
    }
    for (f_6d4 = 0; f_6d4 < 6; f_6d4++)
    {
        if (DG_INT(0xc) != 0 || P4_SLOT4(f_6d4, 0x8432c) != 0 || P4_SLOT4(f_6d4, 0x8a0ec) != 0
            || DG_SHORT(0x5f18a) == f_6d4 + 200 || DG_SHORT(0x5f188) == f_6d4 + 200)
        {
            P4_SLOT4(f_6d4, 0x8a0ec) = 0;
            EQ_METHOD(1, void, 0x0047bc1f, (void*, int), self)(f_6d4);
        }
    }
    goto L_00446D83;
#undef P4_IMG
#undef P4_SETIMG
#undef P4_RECT
#undef P4_DRAW
#undef P4_T
#undef P4_ONOFF
#undef P4_SEL
#undef P4_SLIDE
#undef P4_TAB
#undef P4_SLOT
#undef P4_SLOT4
#undef P4_SPSLOT
#undef P4_SPELLIMG
    // END REGION P04

    // REGION P05 0x00443F7F-0x004458F2: 6515 B, switch 0044212D case 5
    //   defines L_00443F7F; exits L_004458F2, L_0044612A, L_00446815, L_00446D83
    //   switch scaffold for this piece (paste where the dispatch is):
    //   // SWITCH @004441C9 (in P05): table 0x0044FEF1, 11 entries (cmp), bias 0; selector: MOVZX EAX,byte ptr [ESI + 0x100d] ; CMP EAX,0xa ; MOV EBP,0xa7 ; JA default ; JMP dword ptr [EAX*0x4 + switchdataD_0044fef1]
    //   switch (/* TODO selector (register EAX) */ 0) {
    //   case 0: goto L_004441D0; /* P05 */
    //   case 1: goto L_004443CA; /* P05 */
    //   case 2: goto L_004445C4; /* P05 */
    //   case 3: goto L_004447BE; /* P05 */
    //   case 4: goto L_004449B8; /* P05 */
    //   case 5: goto L_00444BB2; /* P05 */
    //   case 6: goto L_00444DAC; /* P05 */
    //   case 7: goto L_00444FA6; /* P05 */
    //   case 8: goto L_004451A0; /* P05 */
    //   case 9: goto L_0044539A; /* P05 */
    //   case 0xA: goto L_00445608; /* P05 */
    //   default: goto L_004457CF; /* P05 */
    //   }
    // P05: options page 5, the key bindings. main+0x100d = the key page (0..10, 9 keys a page, 7 on
    // page 10), main+0x8fbad = the selected key on it (1-based, 0 = none). EBP (the tab slot index of
    // the first loops) is f_6d4, as in P04; f_6fc is the binding loop index.
#define P5_IMG EQ_GLOBAL(int*, 0x006b341c)
#define P5_RECT(a, b, c, d) setRect(P5_IMG + 1, a, b, c, d)
#define P5_SLOT(i, off) ((DG_CHAR(0xfe0) * 400 + (i)) * 0x10 + (off) + DG_THIS)
#define P5_TABS() \
    DG_DRAW(P5_IMG, P5_SLOT(f_6d4, 0x6c90c)); \
    *(int*)P5_SLOT(f_6d4, 0x6c910) += 0xe; \
    *(int*)P5_SLOT(f_6d4, 0x6c918) += 0xe; \
    DG_DRAW(P5_IMG, P5_SLOT(f_6d4, 0x6c90c)); \
    *(int*)P5_SLOT(f_6d4, 0x6c910) -= 0xe; \
    *(int*)P5_SLOT(f_6d4, 0x6c918) -= 0xe;
#define P5_KEY(text, n) \
    DG_TEXT(text, DG_RX(0x6d0dc + (n) * 0x10), DG_RY(0x6d0dc + (n) * 0x10) - 0xf, 10, DG_FONTO)
#define P5_PAGE(t0, t1, t2, t3, t4, t5, t6, t7, t8) \
    P5_KEY(t0, 0); \
    P5_KEY(t1, 1); \
    P5_KEY(t2, 2); \
    P5_KEY(t3, 3); \
    P5_KEY(t4, 4); \
    P5_KEY(t5, 5); \
    P5_KEY(t6, 6); \
    P5_KEY(t7, 7); \
    P5_KEY(t8, 8); \
    P5_RECT(0x77, 0xfa, 0xa7, 0x10c); \
    DG_DRAW(P5_IMG, DG_DR(0x6cffc)); \
    P5_RECT(0xa7, 0xfa, 0xd7, 0x10c); \
    DG_DRAW(P5_IMG, DG_DR(0x6d00c));
#define P5_PAGENO ((unsigned char)DG_CHAR(0x100d))
#define P5_SELKEY (DG_CHAR(0xfe0) * 400 - P5_PAGENO * 9 + (unsigned char)DG_CHAR(0x8fbad))
#define P5_BINDING(i) ((i) + P5_PAGENO * 9 - 0x7c)
#define P5_KEYTEXT EQ_CDECL(void, 0x004505a0, (int, int, int))
L_00443F7F:
    if (DG_INT(0x5f16c) == 0)
        goto L_00446D83;
    *P5_IMG = (int)DG_OVL(0x006e4e1c);
    P5_RECT(0x78, 0xb9, 0xdc, 0xcb);
    setRect(f_6f0, 9, 10, 0x6d, 0x1b);
    DG_DRAW(P5_IMG, f_6f0);
    P5_RECT(0x77, 0x5f, 0xdd, 0x6d);
    if (DG_CHAR(0x100d) == 10)
    {
        for (f_6d4 = 0x7d; f_6d4 < 0x84; f_6d4++)
        {
            P5_TABS()
        }
    }
    else
    {
        for (f_6d4 = 0x7d; f_6d4 < 0x86; f_6d4++)
        {
            P5_TABS()
        }
    }
    // 004441B4: the key names of the page
    switch (P5_PAGENO)
    {
    case 0:
        P5_PAGE(0x00559cd4, 0x00559ccc, 0x00559cbc, 0x00559cb0, 0x00559ca4, 0x00559c98, 0x00559ce4, 0x00559c8c,
                0x00559c80)
        break;
    case 1:
        P5_PAGE(0x00559c78, 0x00559c70, 0x00559c64, 0x00559c5c, 0x00559c4c, 0x00559c40, 0x00559c34, 0x00559c28,
                0x00559c20)
        break;
    case 2:
        P5_PAGE(0x00559c14, 0x00559c04, 0x00559bf8, 0x00559bec, 0x00559bdc, 0x00559bcc, 0x00559bbc, 0x00559bac,
                0x00559b9c)
        break;
    case 3:
        P5_PAGE(0x00559b90, 0x00559b84, 0x00559b74, 0x00559b64, 0x00559b54, 0x00559b44, 0x00559b34, 0x00559b24,
                0x00559b14)
        break;
    case 4:
        P5_PAGE(0x00559b04, 0x00559af4, 0x00559ae4, 0x00559ad4, 0x00559ac8, 0x00559abc, 0x00559ab0, 0x00559aa4,
                0x00559a98)
        break;
    case 5:
        P5_PAGE(0x00559a8c, 0x00559a80, 0x00559a74, 0x00559a68, 0x00559a58, 0x00559a4c, 0x00559a40, 0x00559a34,
                0x00559a28)
        break;
    case 6:
        P5_PAGE(0x00559a1c, 0x00559a10, 0x00559a04, 0x005599f8, 0x005599f0, 0x005599e4, 0x005599d4, 0x005599c4,
                0x005599b4)
        break;
    case 7:
        P5_PAGE(0x005599a8, 0x0055999c, 0x00559994, 0x0055998c, 0x00559984, 0x00559980, 0x00559970, 0x00559968,
                0x00559960)
        break;
    case 8:
        P5_PAGE(0x00559954, 0x00559948, 0x00559940, 0x00559934, 0x00559928, 0x0055991c, 0x00559910, 0x00559900,
                0x005598f4)
        break;
    case 9:
        P5_PAGE(0x005598e8, 0x005598dc, 0x005598d0, 0x005598c8, 0x005598bc, 0x005598ac, 0x0055989c, 0x0055988c,
                0x0055987c)
        break;
    case 10:
        P5_KEY(0x0055986c, 0);
        P5_KEY(0x0055985c, 1);
        P5_KEY(0x0055984c, 2);
        P5_KEY(0x0055983c, 3);
        P5_KEY(0x0055982c, 4);
        P5_KEY(0x0055981c, 5);
        P5_KEY(0x0055980c, 6);
        P5_RECT(0x77, 0xfa, 0xa7, 0x10c);
        DG_DRAW(P5_IMG, DG_DR(0x6cfec));
        break;
    }
    // 004457CF: the marker of the selected key
    if (DG_CHAR(0x8fbad) != 0)
    {
        if (DG_INT(0x8fbb0) == 0)
            DG_TEXT(0x0055a058, DG_INT(P5_SELKEY * 0x10 + 0x6d0cc) + 1, DG_INT(P5_SELKEY * 0x10 + 0x6d0d0) - 1, 10,
                    DG_FONTO);
        else
            DG_TEXT(0x0055a058, DG_INT(P5_SELKEY * 0x10 + 0x6d0cc) + 1, DG_INT(P5_SELKEY * 0x10 + 0x6d0d0) + 0xd, 10,
                    DG_FONTO);
    }
    // 00445868: the two bindings of each key
    for (f_6fc = 0x7d; f_6fc < 0x86; f_6fc++)
    {
        if (P5_BINDING(f_6fc) <= 0x61)
        {
            P5_KEYTEXT(f_6fc, P5_BINDING(f_6fc), 0);
            P5_KEYTEXT(f_6fc, P5_BINDING(f_6fc), 1);
        }
    }
    // 004458C2: the pressed page button
    if (DG_SHORT(0x5f188) < 0 || DG_SHORT(0x5f188) == DG_SHORT(0x5f18a))
        goto L_00446D83;
    carryEax = DG_SHORT(0x5f188) - 0x6e;
    if (carryEax == 0)
        goto L_00446815;
    if (--carryEax != 0)
        goto L_0044612A;
    goto L_004458F2;
#undef P5_IMG
#undef P5_RECT
#undef P5_SLOT
#undef P5_TABS
#undef P5_KEY
#undef P5_PAGE
#undef P5_PAGENO
#undef P5_SELKEY
#undef P5_BINDING
#undef P5_KEYTEXT
    // END REGION P05

    // REGION P06 0x00446DAE-0x0044DDD2: 11366 B, switch 00446DA7 case 0; switch 00446DA7 case 1; switch 00446DA7 case 2 (+2 more 
    //   defines L_00446DAE, L_004472D6, L_00447D0D, L_004487EF, L_0044D2CB; exits L_0044FE15; returns
    //   switch scaffold for this piece (paste where the dispatch is):
    //   // SWITCH @00446EE3 (in P06): table 0x0044FF51, 9 entries (cmp), bias -227; selector: MOVSX EAX,AX ; ADD EAX,0xffffff1d ; CMP EAX,0x8 ; JA default ; JMP dword ptr [EAX*0x4 + switchdataD_0044ff51]
    //   switch (/* TODO selector (register EAX) */ 0) {
    //   case 0xE3: goto L_00446EEA; /* P06 */
    //   case 0xE4: goto L_00446F0B; /* P06 */
    //   case 0xE5: goto L_00446F2C; /* P06 */
    //   case 0xE6: goto L_00446F4D; /* P06 */
    //   case 0xE7: goto L_00446F6E; /* P06 */
    //   case 0xE8: goto L_00446FB1; /* P06 */
    //   case 0xE9: goto L_00446FD2; /* P06 */
    //   case 0xEA: goto L_00446FF3; /* P06 */
    //   case 0xEB: goto L_00447038; /* P06 */
    //   default: goto L_004470A8; /* P06 */
    //   }
    // P06: the screen panels 0..3 and 5 after the side panels (the same drawing as P02's screens, without
    // the 0x90258 gate). f_6c8 holds what the asm keeps in EBP (a loop index, a pc offset, a y);
    // wrapScan / wrapBreak / wrapLen / wrapPos / wrapWidth are the spell name wrap's registers.
#define P06_RECT(i, off) ((DG_CHAR(0xfe0) * 400 + (i)) * 0x10 + DG_THIS + (off))
#define P06_RX(i, off) DG_INT((DG_CHAR(0xfe0) * 400 + (i)) * 0x10 + (off))
#define P06_GEMI(i, off) DG_INT((DG_CHAR(0xfe0) * 400 + (i)) * 4 + (off))
#define P06_TEXT16(text, x, y, color, font) \
    EQ_METHOD(6, void, 0x0048d21a, (void*, char*, int, int, short, int, int), EQ_GLOBAL(void*, 0x006e4e68)) \
        ((char*)(text), (x), (y), (short)(color), (font), 1)
#define P06_STRLEN(s) EQ_CDECL(unsigned int, 0x005223d0, (const char*))((const char*)(s))
#define P06_READY \
    (*(unsigned int*)(EQ_GLOBAL(char*, 0x006e4de0) + 0xbc) \
     >= *(unsigned int*)(*(char**)(EQ_GLOBAL(char*, 0x006e4d8c) + 0x70) + 0xf8))
#define P06_EB0 (*(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xeb0))
#define P06_ELAPSED (*(unsigned int*)(EQ_GLOBAL(char*, 0x006e4de0) + 0xbc) - DG_UINT(0x6c564))
#define P06_E48(off) (*(int*)(EQ_GLOBAL(char*, 0x006e4e48) + (off)))
#define P06_I844(off) (*(int*)(EQ_GLOBAL(char*, 0x006b3844) + (off)))
#define P06_SPELL(i) EQ_ARRAY(char*, 0x005fe9a0)[i]
#define P06_SLOTSPELL (*(short*)(DG_PC + 0x752 + DG_SHORT(0xfea) * 2))
#define P06_NAME(i) ((char*)self + (DG_CHAR(0x6841c) * 12 + (i)) * 0x136 + 0x5f196)
#define P06_BAG (*(char**)(DG_PC + f_6c8))
#define P06_I ((short)f_6f8)
#define P06_WIDTH(c) (*(unsigned short*)(EQ_GLOBAL(char*, 0x006efddc) + (c) * 4))
#define P06_SUMMON(x, y) \
    DG_TEXT(0x005589e0, (x) + 2, (y) + 8, 0, DG_FONT2); \
    DG_TEXT(0x00540678, (x) + 6, (y) + 0x12, 0, DG_FONT2);

    // ---- screen 0
L_00446DAE:
    if (DG_SLOT(0x84398) != 0 || DG_SLOT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3644), DG_DR(0x6d73c));
        DG_SLOT(0x8a158) = 0;
    }
    if (DG_SLOT(0x8439c) != 0 || DG_SLOT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3648), DG_DR(0x6d74c));
        DG_SLOT(0x8a15c) = 0;
    }
    if (DG_SLOT(0x843a0) != 0 || DG_SLOT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3630), DG_DR(0x6d75c));
        DG_SLOT(0x8a160) = 0;
    }
    if (DG_SHORT(0x5f188) >= 0 && DG_SHORT(0x5f18a) != DG_SHORT(0x5f188))
    {
        switch (DG_SHORT(0x5f188))
        {
        case 0xe3: DG_DRAW(EQ_GLOBAL(void*, 0x006b3668), DG_DR(0x6d73c)); break;
        case 0xe4: DG_DRAW(EQ_GLOBAL(void*, 0x006b366c), DG_DR(0x6d74c)); break;
        case 0xe5: DG_DRAW(EQ_GLOBAL(void*, 0x006b3628), DG_DR(0x6d75c)); break;
        case 0xe6: DG_DRAW(EQ_GLOBAL(void*, 0x006b3690), DG_DR(0x6d76c)); break;
        case 0xe7:
            if (DG_CHAR(0xb5) != 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3680), DG_DR(0x6d77c));
            else
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3614), DG_DR(0x6d77c));
            break;
        case 0xe8: DG_DRAW(EQ_GLOBAL(void*, 0x006b3604), DG_DR(0x6d78c)); break;
        case 0xe9: DG_DRAW(EQ_GLOBAL(void*, 0x006b367c), DG_DR(0x6d79c)); break;
        case 0xea:
            if (*(char*)(EQ_GLOBAL(char*, 0x006e4da0) + 0xa1) == 0x6e)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b35fc), DG_DR(0x6d7ac));
            else
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3688), DG_DR(0x6d7ac));
            break;
        case 0xeb:
            if (EQ_GLOBAL(char, 0x006b6405) != 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b368c), DG_DR(0x6d7bc));
            else
            {
                DG_IMGSET(0x006e4e40);
                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 1, 0x1cd, 0x66, 0x1df);
                DG_D341C(DG_DR(0x6d7bc));
            }
            break;
        }
    }
    // 004470A8
    if ((DG_SLOT(0x843a8) != 0 || DG_SHORT(0x5f18a) == 0xe7 || DG_INT(0xc) != 0)
        && DG_SHORT(0x5f188) != 0xe7 && DG_CHAR(0xb5) != 0)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3678), DG_DR(0x6d77c));
    if ((DG_SLOT(0x843ac) != 0 || DG_SHORT(0x5f18a) == 0xe8 || DG_INT(0xc) != 0) && DG_SHORT(0x5f188) != 0xe8)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3600), DG_DR(0x6d78c));
    if ((DG_SLOT(0x843b4) != 0 || DG_SHORT(0x5f18a) == 0xea || DG_INT(0xc) != 0) && DG_SHORT(0x5f188) != 0xea)
    {
        if (*(char*)(EQ_GLOBAL(char*, 0x006e4da0) + 0xa1) == 0x6e)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b35f8), DG_DR(0x6d7ac));
        else
            DG_UPV(DG_DR(0x6d7ac), DG_DR(0x6d7ac));
    }
    if ((DG_SLOT(0x843b8) != 0 || DG_SHORT(0x5f18a) == 0xeb || DG_INT(0xc) != 0) && DG_SHORT(0x5f188) != 0xeb)
    {
        if (EQ_GLOBAL(char, 0x006b6405) != 0)
            DG_UPV(DG_DR(0x6d7bc), DG_DR(0x6d7bc));
        else
        {
            DG_IMGSET(0x006e4e40);
            setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x200, 0x1c0, 0x265, 0x1d2);
            DG_D341C(DG_DR(0x6d7bc));
        }
    }
    if (DG_SHORT(0x5f18a) >= 0)
    {
        switch (DG_SHORT(0x5f18a))
        {
        case 0xe6: DG_UPV(DG_DR(0x6d76c), DG_DR(0x6d76c)); break;
        case 0xe7: DG_UPV(DG_DR(0x6d77c), DG_DR(0x6d77c)); break;
        case 0xe9: DG_UPV(DG_DR(0x6d79c), DG_DR(0x6d79c)); break;
        }
    }
    DG_STRCPY(f_5f0, 0x00559728);
    return;

    // ---- screen 1
L_004472D6:
    if (DG_SLOT(0x84398) != 0 || DG_SLOT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3648), DG_DR(0x6d73c));
        DG_SLOT(0x8a158) = 0;
    }
    if (DG_SLOT(0x8439c) != 0 || DG_SLOT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b362c), DG_DR(0x6d74c));
        DG_SLOT(0x8a15c) = 0;
    }
    if (DG_SLOT(0x843a0) != 0 || DG_SLOT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3630), DG_DR(0x6d75c));
        DG_SLOT(0x8a160) = 0;
    }
    if (DG_INT(0xc) != 0 || DG_SLOT(0x8a180) != 0 || (char*)self + 0xe094c != 0)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3620), DG_DR(0x6d7dc));
        for (f_6f8 = 0; f_6f8 < 6; f_6f8++)
            P06_GEMI(f_6f8, 0x8a18c) = 1;
    }
    // 00447442: the trade window
    if (DG_INT(0x685d8) != 0)
    {
        f_6f8 = 0;
        if (DG_INT(0x685bc) > 0)
        {
            f_6c8 = 0;
            do
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36c4), P06_RECT(f_6c8, 0x6dd5c));
                setRect(f_6f0, P06_RX(f_6c8, 0x6dd5c) + 0xf, P06_RX(f_6c8, 0x6dd60) + 0xc, 0x280, 0x1e0);
                f_6e0 = DG_SHORT(f_6c8 * 2 + 0x6c65e);
                if ((short)f_6e0 > 0x64)
                {
                    switch ((short)f_6e0)
                    {
                    case 0x69: DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), f_6f0); break;
                    case 0x6b: DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), f_6f0); break;
                    case 0x6f: DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), f_6f0); break;
                    case 0x72:
                        DG_IMGSET(0x006e4e40);
                        setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x223, 0x30, 0x24b, 0x58);
                        DG_D341C(f_6f0);
                        P06_SUMMON(f_6f0[0], f_6f0[1])
                        break;
                    }
                }
                else
                    DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8a)), f_6f0);
                f_6f8++;
                f_6c8 = (short)f_6f8;
            } while (f_6c8 < DG_INT(0x685bc));
        }
        if (DG_INT(0x685bc) != 0)
        {
            setRect(f_6f0, 0x72, 0xd0, 0x8a, 0x112);
            setRect(f_6cc, DG_RX(0x6df3c), DG_RX(0x6df48), 0x280, 0x1e0);
            DG_PART(DG_OVL(0x006e4e40), f_6f0, f_6cc);
            if (DG_SHORT(0x5f188) == 0x163)
                setRect(f_6f0, 0x72, 0xa0, 0x8a, 0xb8);
            else
                setRect(f_6f0, 0x72, 0xb8, 0x8a, 0xd0);
            DG_PART(DG_OVL(0x006e4e40), f_6f0, DG_DR(0x6df3c));
        }
    }
    // 004476B3: the six give slots
    f_6c8 = 0;
    f_6e0 = 0x006b37c8;
    f_6d8 = (short*)((char*)self + 0xff4);
    f_6dc = (short*)6;
    do
    {
        if (DG_INT(0xc) != 0 || P06_GEMI(f_6c8, 0x843cc) != 0 || DG_SHORT(0x5f18a) == f_6c8 + 0xf0
            || P06_GEMI(f_6c8, 0x8a18c) != 0 || DG_SHORT(0x5f188) == f_6c8 + 0xf0)
        {
            P06_GEMI(f_6c8, 0x8a18c) = 0;
            if (*f_6d8 == -1)
            {
                if (DG_SHORT(0x5f188) == f_6c8 + 0xf0)
                    DG_DRAW(*(void**)(f_6e0 + 0x18), P06_RECT(f_6c8, 0x6d80c));
                else
                    DG_DRAW(*(void**)f_6e0, P06_RECT(f_6c8, 0x6d80c));
            }
            else if (*f_6d8 > 0x64)
            {
                if (*f_6d8 == 0x6f)
                {
                    if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && P06_EB0 != 0)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), P06_RECT(f_6c8, 0x6d80c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b3480), P06_RECT(f_6c8, 0x6d80c));
                }
                else if (*f_6d8 == 0x6b)
                {
                    if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && P06_READY)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), P06_RECT(f_6c8, 0x6d80c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34e8), P06_RECT(f_6c8, 0x6d80c));
                }
                else if (*f_6d8 == 0x69)
                {
                    if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && P06_READY)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), P06_RECT(f_6c8, 0x6d80c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34f0), P06_RECT(f_6c8, 0x6d80c));
                }
                else if (*f_6d8 == 0x72)
                {
                    if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && P06_READY)
                    {
                        DG_IMGSET(0x006e4e40);
                        setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x223, 0x30, 0x24b, 0x58);
                    }
                    else
                    {
                        DG_IMGSET(0x006e4e40);
                        setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x24b, 0x30, 0x273, 0x58);
                    }
                    DG_D341C(P06_RECT(f_6c8, 0x6d80c));
                    P06_SUMMON(P06_RX(f_6c8, 0x6d80c), P06_RX(f_6c8, 0x6d810))
                }
            }
            else
            {
                f_63c = *f_6d8;
                if (DG_ITEMDEF(f_63c)[0x26] == 2)
                {
                    if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && P06_EB0 != 0)
                        DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_63c) + 0x8a)), P06_RECT(f_6c8, 0x6d80c));
                    else
                        DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_63c) + 0x8c)), P06_RECT(f_6c8, 0x6d80c));
                }
                else if (DG_SHORT(0x5f188) != f_6c8 + 0xf0 && EQ_ARRAY(char, 0x006e117c)[f_63c] != 0)
                    DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_63c) + 0x8a)), P06_RECT(f_6c8, 0x6d80c));
                else
                    DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF(f_63c) + 0x8c)), P06_RECT(f_6c8, 0x6d80c));
            }
        }
        f_6d8++;
        f_6e0 += 4;
        f_6c8++;
        f_6dc = (short*)((int)f_6dc - 1);
    } while (f_6dc != 0);
    if (DG_SHORT(0x5f188) >= 0 && DG_SHORT(0x5f188) != DG_SHORT(0x5f18a))
    {
        switch (DG_SHORT(0x5f188))
        {
        case 0xe3: DG_DRAW(EQ_GLOBAL(void*, 0x006b366c), DG_DR(0x6d73c)); break;
        case 0xe4: DG_DRAW(EQ_GLOBAL(void*, 0x006b3670), DG_DR(0x6d74c)); break;
        case 0xe5: DG_DRAW(EQ_GLOBAL(void*, 0x006b3628), DG_DR(0x6d75c)); break;
        }
    }
    // 00447B80: the item held over a trade slot
    if (DG_INT(0x685d8) != 0 && DG_SHORT(0x5f188) >= 0x145 && DG_SHORT(0x5f188) <= 0x162)
    {
        setRect(f_6f0, P06_RX(DG_SHORT(0x5f188), 0x6c90c) + 0xf, P06_RX(DG_SHORT(0x5f188), 0x6c910) + 0xc, 0x280, 0x1e0);
        f_6e0 = DG_SHORT(DG_SHORT(0x5f188) * 2 + 0x6c3d4);
        if ((short)f_6e0 > 0x64)
        {
            switch ((short)f_6e0)
            {
            case 0x69: DG_DRAW(EQ_GLOBAL(void*, 0x006b34f0), f_6f0); break;
            case 0x6b: DG_DRAW(EQ_GLOBAL(void*, 0x006b34e8), f_6f0); break;
            case 0x6f: DG_DRAW(EQ_GLOBAL(void*, 0x006b3480), f_6f0); break;
            case 0x72:
                DG_IMGSET(0x006e4e40);
                setRect(EQ_GLOBAL(int*, 0x006b341c) + 1, 0x24b, 0x30, 0x273, 0x58);
                DG_D341C(f_6f0);
                P06_SUMMON(f_6f0[0], f_6f0[1])
                break;
            }
        }
        else
            DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8c)), f_6f0);
    }
    DG_STRCPY(f_5f0, 0x0055971c);
    return;

    // ---- screen 2
L_00447D0D:
    if (DG_SLOT(0x84398) != 0 || DG_SLOT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3644), DG_DR(0x6d73c));
        DG_SLOT(0x8a158) = 0;
    }
    if (DG_SLOT(0x8439c) != 0 || DG_SLOT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b362c), DG_DR(0x6d74c));
        DG_SLOT(0x8a15c) = 0;
    }
    if (DG_SLOT(0x843a0) != 0 || DG_SLOT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3630), DG_DR(0x6d75c));
        DG_SLOT(0x8a160) = 0;
    }
    if (DG_INT(0xc) != 0 || DG_SLOT(0x8a180) != 0 || (char*)self + 0xe094c != 0)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3620), DG_DR(0x6d7dc));
        for (f_6f8 = 0; f_6f8 < 6; f_6f8++)
            P06_GEMI(f_6f8, 0x8a1a4) = 1;
    }
    // 00447E79: the merchant window
    if (DG_INT(0x685d4) != 0)
    {
        f_6f8 = 0;
        if (DG_INT(0x685b8) > 0)
        {
            f_6c8 = 0;
            do
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36c4), P06_RECT(f_6c8, 0x6dd5c));
                setRect(f_6f0, P06_RX(f_6c8, 0x6dd5c) + 0xf, P06_RX(f_6c8, 0x6dd60) + 0xc, 0x280, 0x1e0);
                f_6e0 = DG_SHORT(f_6c8 * 2 + 0x6c65e);
                if ((short)f_6e0 > 0x64)
                {
                    switch ((short)f_6e0)
                    {
                    case 0x69: DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), f_6f0); break;
                    case 0x6b: DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), f_6f0); break;
                    case 0x6f: DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), f_6f0); break;
                    }
                }
                else
                    DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8a)), f_6f0);
                f_6f8++;
                f_6c8 = (short)f_6f8;
            } while (f_6c8 < DG_INT(0x685b8));
        }
        if (DG_INT(0x685b8) != 0)
        {
            setRect(f_6f0, 0x72, 0xd0, 0x8a, 0x112);
            setRect(f_6cc, DG_RX(0x6df3c), DG_RX(0x6df48), 0x280, 0x1e0);
            DG_PART(DG_OVL(0x006e4e40), f_6f0, f_6cc);
            if (DG_SHORT(0x5f188) == 0x163)
                setRect(f_6f0, 0x72, 0xa0, 0x8a, 0xb8);
            else
                setRect(f_6f0, 0x72, 0xb8, 0x8a, 0xd0);
            DG_PART(DG_OVL(0x006e4e40), f_6f0, DG_DR(0x6df3c));
        }
    }
    // 00448045: the four coin slots
    f_6f8 = 0;
    do
    {
        if (DG_INT(0xc) != 0 || P06_GEMI(P06_I, 0x843ec) != 0 || P06_GEMI(P06_I, 0x8a1ac) != 0
            || DG_SHORT(0x5f18a) == P06_I + 0xf8 || DG_SHORT(0x5f188) == P06_I + 0xf8)
        {
            P06_GEMI(P06_I, 0x8a1ac) = 0;
            f_6e0 = DG_SHORT(P06_I * 2 + 0xfec);
            if ((short)f_6e0 == -1)
            {
                if (P06_I == 0)
                {
                    if (DG_SHORT(0x5f188) == 0xf8)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b36b4), DG_DR(0x6d88c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b369c), DG_DR(0x6d88c));
                }
                else if (P06_I == 1)
                {
                    if (DG_SHORT(0x5f188) == 0xf9)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b36b8), DG_DR(0x6d89c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b36a0), DG_DR(0x6d89c));
                }
                else if (P06_I == 2)
                {
                    if (DG_SHORT(0x5f188) == 0xfa)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b36bc), DG_DR(0x6d8ac));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b36a4), DG_DR(0x6d8ac));
                }
                else if (P06_I == 3)
                {
                    if (DG_SHORT(0x5f188) == 0xfb)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b36c0), DG_DR(0x6d8bc));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b36a8), DG_DR(0x6d8bc));
                }
            }
            else if ((short)f_6e0 > 0x64)
            {
                if ((short)f_6e0 == 0x6f)
                {
                    if (DG_SHORT(0x5f188) != P06_I + 0xf8 && P06_EB0 != 0)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b347c), P06_RECT(P06_I, 0x6d88c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b3480), P06_RECT(P06_I, 0x6d88c));
                }
                else if ((short)f_6e0 == 0x6b)
                {
                    if (DG_SHORT(0x5f188) != P06_I + 0xf8 && P06_READY)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34e4), P06_RECT(P06_I, 0x6d88c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34e8), P06_RECT(P06_I, 0x6d88c));
                }
                else if ((short)f_6e0 == 0x69)
                {
                    if (DG_SHORT(0x5f188) != P06_I + 0xf8 && P06_READY)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34ec), P06_RECT(P06_I, 0x6d88c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b34f0), P06_RECT(P06_I, 0x6d88c));
                }
            }
            else if (DG_ITEMDEF((short)f_6e0)[0x26] == 2)
            {
                if (DG_SHORT(0x5f188) != P06_I + 0xf8 && P06_EB0 != 0)
                    DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8a)), P06_RECT(P06_I, 0x6d88c));
                else
                    DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8c)), P06_RECT(P06_I, 0x6d88c));
            }
            else if (DG_SHORT(0x5f188) != P06_I + 0xf8 && EQ_ARRAY(char, 0x006e117c)[(short)f_6e0] != 0)
                DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8a)), P06_RECT(P06_I, 0x6d88c));
            else
                DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8c)), P06_RECT(P06_I, 0x6d88c));
        }
        f_6f8++;
    } while (P06_I < 4);
    if (DG_SHORT(0x5f188) >= 0)
    {
        switch (DG_SHORT(0x5f188))
        {
        case 0xe3: DG_DRAW(EQ_GLOBAL(void*, 0x006b3668), DG_DR(0x6d73c)); break;
        case 0xe4: DG_DRAW(EQ_GLOBAL(void*, 0x006b3670), DG_DR(0x6d74c)); break;
        case 0xe5: DG_DRAW(EQ_GLOBAL(void*, 0x006b3628), DG_DR(0x6d75c)); break;
        case 0xf6: DG_DRAW(EQ_GLOBAL(void*, 0x006b36ac), DG_DR(0x6d86c)); break;
        case 0xf7: DG_DRAW(EQ_GLOBAL(void*, 0x006b36b0), DG_DR(0x6d87c)); break;
        }
    }
    if (*(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xea4) == 0)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b36ac), DG_DR(0x6d86c));
    if (*(int*)(EQ_GLOBAL(char*, 0x006e4e44) + 0xea4) == 0)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b36b0), DG_DR(0x6d87c));
    // 0044858A: the item held over a merchant slot
    if (DG_INT(0x685d4) != 0 && DG_SHORT(0x5f188) >= 0x145 && DG_SHORT(0x5f188) <= 0x162)
    {
        setRect(f_6f0, P06_RX(DG_SHORT(0x5f188), 0x6c90c) + 0xf, P06_RX(DG_SHORT(0x5f188), 0x6c910) + 0xc, 0x280, 0x1e0);
        f_6e0 = DG_SHORT(DG_SHORT(0x5f188) * 2 + 0x6c3d4);
        if ((short)f_6e0 > 0x64)
        {
            switch ((short)f_6e0)
            {
            case 0x69: DG_DRAW(EQ_GLOBAL(void*, 0x006b34f0), f_6f0); break;
            case 0x6b: DG_DRAW(EQ_GLOBAL(void*, 0x006b34e8), f_6f0); break;
            case 0x6f: DG_DRAW(EQ_GLOBAL(void*, 0x006b3480), f_6f0); break;
            }
        }
        else
            DG_DRAW(DG_ITEMIMG(*(short*)(DG_ITEMDEF((short)f_6e0) + 0x8c)), f_6f0);
    }
    // 00448665: the cursor and secondary slots
    if (DG_INT(0xc) != 0 || DG_SLOT(0x843e4) != 0 || DG_SLOT(0x8a1a4) != 0)
    {
        if (*(char**)(DG_PC + 0x114) != 0)
            DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x114) + 0x84)), DG_DR(0x6d86c));
    }
    if (DG_INT(0xc) != 0 || DG_SLOT(0x843e8) != 0 || DG_SLOT(0x8a1a8) != 0)
    {
        if (*(char**)(DG_PC + 0x10c) != 0)
            DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x10c) + 0x84)), DG_DR(0x6d87c));
        if (DG_COMBINE(*(char**)(DG_PC + 0x10c)) != 0)
        {
            setRect(f_6f0, DG_RX(0x6d884) - 0x13, DG_RX(0x6d888) - 0xf, DG_RX(0x6d884), DG_RX(0x6d888));
            DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), f_6f0);
            DG_SPRINTF(f_6bc, (const char*)0x0053e16c,
                       ((unsigned char)(*(char**)(DG_PC + 0x10c))[0xda] - 1) / (*(char**)(DG_PC + 0x10c))[0xbb] + 1);
            DG_TEXT(f_6bc, DG_RX(0x6d87c) + 0x1a, DG_RX(0x6d880) + 0x1d, 10, DG_FONT3);
        }
    }
    DG_STRCPY(f_5f0, 0x0055970c);
    return;

    // ---- screen 3
L_004487EF:
    if (DG_SLOT(0x84398) != 0 || DG_SLOT(0x8a158) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe3)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3644), DG_DR(0x6d73c));
        DG_SLOT(0x8a158) = 0;
    }
    if (DG_SLOT(0x8439c) != 0 || DG_SLOT(0x8a15c) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe4)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3648), DG_DR(0x6d74c));
        DG_SLOT(0x8a15c) = 0;
    }
    if (DG_SLOT(0x843a0) != 0 || DG_SLOT(0x8a160) != 0 || DG_INT(0xc) != 0 || DG_SHORT(0x5f18a) == 0xe5)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b362c), DG_DR(0x6d75c));
        DG_SLOT(0x8a160) = 0;
    }
    if (DG_INT(0xc) != 0 || DG_SLOT(0x8a180) != 0 || DG_SLOT(0x843c0) != 0)
    {
        DG_DRAW(EQ_GLOBAL(void*, 0x006b361c), DG_DR(0x6d7dc));
        for (f_6f8 = 0; f_6f8 < 12; f_6f8++)
            P06_GEMI(f_6f8, 0x89eb0) = 1;
    }
    if (DG_SHORT(0x5f188) >= 0)
    {
        switch (DG_SHORT(0x5f188))
        {
        case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e:
        case 0x3f: case 0x40: case 0x41: case 0x42: case 0x43: case 0x44:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3674), P06_RECT(DG_SHORT(0x5f188), 0x6c90c));
            break;
        case 0x4e: DG_DRAW(EQ_GLOBAL(void*, 0x006b38a8), DG_DR(0x6cdec)); break;
        case 0x4f: DG_DRAW(EQ_GLOBAL(void*, 0x006b38ac), DG_DR(0x6cdfc)); break;
        case 0xe3: DG_DRAW(EQ_GLOBAL(void*, 0x006b3668), DG_DR(0x6d73c)); break;
        case 0xe4: DG_DRAW(EQ_GLOBAL(void*, 0x006b366c), DG_DR(0x6d74c)); break;
        case 0xe5: DG_DRAW(EQ_GLOBAL(void*, 0x006b3670), DG_DR(0x6d75c)); break;
        }
    }
    // 00448A65: the twelve hot keys
    f_6c8 = 0;
    f_5f8 = 12;
    do
    {
        if (DG_INT(0xc) != 0 || P06_GEMI(f_6c8, 0x840f0) != 0 || DG_SHORT(0x5f18a) == f_6c8 + 0x39
            || P06_GEMI(f_6c8, 0x89eb0) != 0)
        {
            if (DG_SHORT(0x5f188) != f_6c8 + 0x39)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3848), P06_RECT(f_6c8, 0x6cc9c));
            P06_GEMI(f_6c8, 0x89eb0) = 0;
            f_6e0 = (short)P06_STRLEN(P06_NAME(f_6c8));
            f_6f4 = 0;
            if ((short)f_6e0 > 0)
            {
                f_6dc = (short*)(int)(short)f_6e0;
                f_6d8 = (short*)P06_NAME(f_6c8);
                do
                {
                    if (*(char*)f_6d8 > 0)
                        f_6f4 += P06_WIDTH(*(char*)f_6d8);
                    f_6d8 = (short*)((char*)f_6d8 + 1);
                    f_6dc = (short*)((int)f_6dc - 1);
                } while (f_6dc != 0);
            }
            f_6f4 = (short)f_6f4 >> 1;
            if ((short)f_6f4 > 0x14)
                f_6f4 = 0x14;
            P06_TEXT16(P06_NAME(f_6c8), P06_RX(f_6c8, 0x6cc9c) - (short)f_6f4 + 0x17, P06_RX(f_6c8, 0x6cca0) + 5,
                       DG_CHAR((DG_CHAR(0x6841c) * 12 + f_6c8) * 0x136 + 0x5f2cb), DG_FONT2);
        }
        f_6c8++;
    } while (--f_5f8 != 0);
    if (DG_SHORT(0x5f188) != 0x4e)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b38ec), DG_DR(0x6cdec));
    if (DG_SHORT(0x5f188) != 0x4f)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b38f0), DG_DR(0x6cdfc));
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, DG_CHAR(0x6841c) + 1);
    if (DG_CHAR(0x6841c) < 9)
        f_6dc = (short*)(DG_RX(0x6cdec) + 0x1b);
    else
        f_6dc = (short*)(DG_RX(0x6cdec) + 0x18);
    f_6c8 = DG_RY(0x6cdec) - 2 + 1;
    setRect(f_6f0, DG_RX(0x6cdec) + 0x14, f_6c8 - 1, DG_RX(0x6cdec) + 0x15, f_6c8);
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), f_6f0);
    DG_TEXT(f_6bc, (int)f_6dc, f_6c8, 10, DG_FONT);
    // 00448CEC: the hot key editor
    if (DG_INT(0x5f18c) != 0)
    {
        setRect(f_6f0, 0x6f, 0x140, 0x79, 0x14a);
        DG_DRAW(EQ_GLOBAL(void*, 0x006b38e8), f_6f0);
        switch (DG_CHAR(0x5f195))
        {
        case 0: DG_TEXT(0x0055a058, 0x7a, 0x163, 10, DG_FONT); break;
        case 1: DG_TEXT(0x0055a058, 0x7a, 0x177, 10, DG_FONT); break;
        case 2: DG_TEXT(0x0055a058, 0x7a, 0x18b, 10, DG_FONT); break;
        case 3: DG_TEXT(0x0055a058, 0x7a, 0x19f, 10, DG_FONT); break;
        case 4: DG_TEXT(0x0055a058, 0x7a, 0x1b3, 10, DG_FONT); break;
        case 5: DG_TEXT(0x0055a058, 0xd2, 0x14b, 10, DG_FONT); break;
        }
        DG_TEXT((char*)self + 0x682e6, 0xd7, 0x14b, 10, DG_FONT);
        P06_TEXT16(0x0055a054, 0x19f, 0x14b, DG_CHAR(0x6841b), DG_FONT);
        DG_TEXT((char*)self + 0x682ef, 0x84, 0x161, 10, DG_FONT);
        DG_TEXT((char*)self + 0x6832b, 0x84, 0x175, 10, DG_FONT);
        DG_TEXT((char*)self + 0x68367, 0x84, 0x189, 10, DG_FONT);
        DG_TEXT((char*)self + 0x683a3, 0x84, 0x19d, 10, DG_FONT);
        DG_TEXT((char*)self + 0x683df, 0x84, 0x1b1, 10, DG_FONT);
    }
    if (DG_SHORT(0x5f188) == 0x4c)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b38a4), DG_DR(0x6cdcc));
    if (DG_SHORT(0x5f188) == 0x4d)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b37c0), DG_DR(0x6cddc));
    DG_STRCPY(f_5f0, 0x005596fc);
    return;

    // ---- screen 5: the spell book
L_0044D2CB:
    DG_STRCPY(f_5f0, 0x005596ec);
    DG_DRAW(EQ_GLOBAL(void*, 0x006b3810), DG_DR(0x6e0cc));
    if (DG_INT(0xc) != 0)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3844), DG_DR(0x6e0fc));
    else if (DG_SLOT(0x84608) != 0 || DG_SLOT(0x8a3c8) != 0)
    {
        // 0044D350: redraw the part of the book under the moved window, clipped to both
        DG_SLOT(0x8a3c8) = 0;
        f_6d4 = P06_E48(0xa8);
        f_6fc = P06_E48(0xa4);
        f_6d0 = f_6fc - DG_RX(0x6e0fc);
        if (f_6d0 < 0)
        {
            f_6d0 = 0;
            f_6fc = DG_RX(0x6e0fc);
        }
        f_6f8 = P06_E48(0xac) - DG_RX(0x6e0fc) + 1;
        if (f_6f8 > DG_RX(0x6e104) - DG_RX(0x6e0fc))
            f_6f8 = DG_RX(0x6e104) - DG_RX(0x6e0fc);
        f_6f4 = f_6d4 - DG_RX(0x6e100);
        if (f_6f4 < 0)
        {
            f_6f4 = 0;
            f_6d4 = DG_RX(0x6e100);
        }
        clipBottom = P06_E48(0xb0) - DG_RX(0x6e100) + 1;
        if (clipBottom > DG_RX(0x6e108) - DG_RX(0x6e100))
            clipBottom = DG_RX(0x6e108) - DG_RX(0x6e100);
        f_6d0 += P06_I844(4);
        if (f_6d0 > P06_I844(0xc))
            f_6d0 = P06_I844(0xc);
        f_6f8 += P06_I844(4);
        if (f_6f8 > P06_I844(0xc))
            f_6f8 = P06_I844(0xc);
        clipTop = f_6f4 + P06_I844(8);
        if (clipTop > P06_I844(0x10))
            clipTop = P06_I844(0x10);
        clipBottom += P06_I844(8);
        if (clipBottom > P06_I844(0x10))
            clipBottom = P06_I844(0x10);
        if (f_6f8 > f_6d0 && clipBottom > clipTop)
        {
            setRect(f_6f0, f_6d0, clipTop, f_6f8, clipBottom);
            setRect(f_6cc, f_6fc, f_6d4, f_6fc + 1, f_6d4 + 1);
            DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
        }
    }
    // 0044D47F
    if (DG_SHORT(0x5f188) >= 0)
    {
        switch (DG_SHORT(0x5f188))
        {
        case 0x35:
            setRect(f_6f0, 0, 0x154, 0x20, 0x173);
            DG_PART(DG_OVL(0x006e4e08), f_6f0, DG_DR(0x6cc5c));
            break;
        case 0x36:
            setRect(f_6f0, 0x20, 0x154, 0x41, 0x173);
            DG_PART(DG_OVL(0x006e4e08), f_6f0, DG_DR(0x6cc6c));
            break;
        case 0x37: DG_DRAW(EQ_GLOBAL(void*, 0x006b3818), DG_DR(0x6cc7c)); break;
        case 0x38: DG_DRAW(EQ_GLOBAL(void*, 0x006b37c0), DG_DR(0x6cc8c)); break;
        }
    }
    if (DG_SHORT(0x5f188) != 0x38 && (DG_SLOT(0x840ec) != 0 || DG_SHORT(0x5f18a) == 0x38))
    {
        setRect(f_6f0, 0x9d, 0x1c3, 0xcc, 0x1d5);
        DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cc8c));
    }
    EQ_METHOD(0, void, 0x00457617, (void*), self)();
    EQ_METHOD(0, void, 0x004577c6, (void*), self)();
    if (DG_INT(0x6c564) != 0)
    {
        // 0044D5DC: the page turn animation
        if (DG_INT(0x6c55c) != 0)
        {
            if (P06_ELAPSED < 100)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3838), DG_DR(0x6e14c));
            else if (P06_ELAPSED < 200)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3834), DG_DR(0x6e13c));
            else if (P06_ELAPSED < 300)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3830), DG_DR(0x6e12c));
            else if (P06_ELAPSED < 400)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b382c), DG_DR(0x6e11c));
        }
        else if (P06_ELAPSED < 100)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b382c), DG_DR(0x6e11c));
        else if (P06_ELAPSED < 200)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3830), DG_DR(0x6e12c));
        else if (P06_ELAPSED < 300)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3834), DG_DR(0x6e13c));
        else if (P06_ELAPSED < 400)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3838), DG_DR(0x6e14c));
        if (P06_ELAPSED >= 400)
        {
            DG_INT(0x6c564) = 0;
            switch (DG_SHORT(0x6c560) % 3)
            {
            case 0: DG_DRAW(EQ_GLOBAL(void*, 0x006b3828), DG_DR(0x6e15c)); break;
            case 1: DG_DRAW(EQ_GLOBAL(void*, 0x006b383c), DG_DR(0x6e16c)); break;
            case 2: DG_DRAW(EQ_GLOBAL(void*, 0x006b3840), DG_DR(0x6e17c)); break;
            }
        }
    }
    else
    {
        // 0044D743: the page and its ten spells
        switch (DG_SHORT(0x6c560) % 3)
        {
        case 0: DG_DRAW(EQ_GLOBAL(void*, 0x006b3828), DG_DR(0x6e15c)); break;
        case 1: DG_DRAW(EQ_GLOBAL(void*, 0x006b383c), DG_DR(0x6e16c)); break;
        case 2: DG_DRAW(EQ_GLOBAL(void*, 0x006b3840), DG_DR(0x6e17c)); break;
        }
        f_6f8 = 0;
        f_6d4 = 0;
        do
        {
            DG_SHORT(0xfea) = DG_SHORT(0x6c560) * 10 + (short)f_6f8;
            if (DG_SHORT(0xfea) <= 0xfa && P06_SLOTSPELL != -1)
            {
                DG_DRAWIMAGE(P06_RECT(f_6d4, 0x6cbbc), *(short*)(P06_SPELL(P06_SLOTSPELL) + 0x1fa));
                f_6f4 = 0;
                f_6e0 = 0;
                f_6d8 = (short*)P06_STRLEN(P06_SPELL(P06_SLOTSPELL));
                DG_STRCPY(f_5a0, P06_SPELL(P06_SLOTSPELL));
                f_6fc = 0;
                if ((short)(int)f_6d8 > 0)
                {
                    do
                    {
                        if ((short)(int)f_6d8 <= 0)
                            break;
                        if (f_6e0 > 0x3c)
                        {
                            // 0044D8B2: wrap at the last blank of the last 15 characters
                            wrapBreak = 0;
                            for (wrapScan = (short)f_6f4; wrapScan > (short)f_6f4 - 0xf; wrapScan--)
                            {
                                if (wrapBreak == 0 && f_5a0[wrapScan] == ' ')
                                    wrapBreak = wrapScan + 1;
                            }
                            if (wrapBreak == 0)
                                wrapBreak = (short)f_6f4;
                            DG_STRCPY(f_6bc, 0x0058fba4);
                            EQ_CDECL(char*, 0x005231d0, (char*, const char*, unsigned int))(f_6bc, f_5a0, wrapBreak);
                            f_6bc[wrapBreak] = 0;
                            DG_STRCPY(f_5a0, f_5a0 + wrapBreak);
                            f_6e0 = 0;
                            f_6f4 = 0;
                            f_6d8 = (short*)P06_STRLEN(f_5a0);
                            wrapWidth = 0;
                            wrapLen = (short)P06_STRLEN(f_6bc);
                            if (wrapLen > 0)
                            {
                                f_6dc = (short*)(int)wrapLen;
                                wrapPos = 0;
                                do
                                {
                                    if (f_6bc[wrapPos] > 0)
                                        wrapWidth += P06_WIDTH(f_6bc[wrapPos]);
                                    wrapPos++;
                                    f_6dc = (short*)((int)f_6dc - 1);
                                } while (f_6dc != 0);
                            }
                            DG_TEXT(f_6bc, P06_RX(f_6d4, 0x6cbbc) - (short)wrapWidth / 2 + 0x14,
                                    P06_RX(f_6d4, 0x6cbc0) + (short)f_6fc * 8 + 0x28, 0, DG_FONT2);
                            f_6fc++;
                        }
                        if (f_5a0[(short)f_6f4] > 0)
                            f_6e0 += *(int*)(EQ_GLOBAL(char*, 0x006efddc) + f_5a0[(short)f_6f4] * 4);
                        f_6f4++;
                    } while ((short)f_6f4 < (short)(int)f_6d8);
                }
                // 0044DA0E: the rest of the name
                if (P06_STRLEN(f_5a0) != 0)
                {
                    f_6e0 = 0;
                    wrapLen = (short)P06_STRLEN(f_5a0);
                    if (wrapLen > 0)
                    {
                        f_6dc = (short*)(int)wrapLen;
                        wrapPos = 0;
                        do
                        {
                            if (f_5a0[wrapPos] > 0)
                                f_6e0 += P06_WIDTH(f_5a0[wrapPos]);
                            wrapPos++;
                            f_6dc = (short*)((int)f_6dc - 1);
                        } while (f_6dc != 0);
                    }
                    DG_TEXT(f_5a0, P06_RX(f_6d4, 0x6cbbc) - (short)f_6e0 / 2 + 0x14,
                            P06_RX(f_6d4, 0x6cbc0) + (short)f_6fc * 8 + 0x28, 0, DG_FONT2);
                }
            }
            // 0044DAC4: the selected spell
            if (DG_INT(0xed4) >= 0 && DG_INT(0xed4) == DG_SHORT(0xfea))
            {
                if (DG_INT(0x8) == 0)
                    DG_CHAR(0x6c904) ^= 1;
                if (DG_CHAR(0x6c904) != 0)
                    setRect(f_6f0, 0x167, 0x8a, 0x196, 0xb9);
                else
                    setRect(f_6f0, 0x167, 0xb9, 0x196, 0xe8);
                setRect(f_6cc, P06_RX(f_6d4, 0x6cbbc) - 3, P06_RX(f_6d4, 0x6cbc0) - 3, 0x280, 0x1e0);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
            }
            f_6f8++;
            f_6d4++;
        } while ((short)f_6f8 < 10);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, DG_SHORT(0x6c560) * 2 + 1);
        DG_TEXT(f_6bc, 0xf4, 0xe6, 0, DG_FONT2);
        DG_SPRINTF(f_6bc, (const char*)0x0053e16c, DG_SHORT(0x6c560) * 2 + 2);
        DG_TEXT(f_6bc, 0x18c, 0xe6, 0, DG_FONT2);
    }
    // 0044DC0E
    if (DG_INT(0xfe4) != 0)
        DG_DRAW(EQ_GLOBAL(void*, 0x006b3818), DG_DR(0x6cc7c));
    // the eight bag slots (pc+0x138..)
    f_6fc = 0;
    f_6c8 = 0x138;
    f_6dc = (short*)8;
    do
    {
        if ((DG_INT(0xc) != 0 || P06_GEMI(f_6fc, 0x84064) != 0) && P06_BAG != 0)
        {
            DG_DRAW(DG_ITEMIMG(*(short*)(P06_BAG + 0x84)), P06_RECT(f_6fc, 0x6ca6c));
            if (DG_COMBINE(P06_BAG) != 0)
            {
                setRect(f_6f0, P06_RX(f_6fc, 0x6ca74) - 0xf, P06_RX(f_6fc, 0x6ca78) - 0xb,
                        P06_RX(f_6fc, 0x6ca74), P06_RX(f_6fc, 0x6ca78));
                DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), f_6f0);
                DG_SPRINTF(f_6bc, (const char*)0x0053e16c, ((unsigned char)P06_BAG[0xda] - 1) / P06_BAG[0xbb] + 1);
                DG_TEXT(f_6bc, P06_RX(f_6fc, 0x6ca74) - 10, P06_RX(f_6fc, 0x6ca78) - 6, 10, DG_FONT3);
            }
        }
        f_6fc++;
        f_6c8 += 4;
        f_6dc = (short*)((int)f_6dc - 1);
    } while (f_6dc != 0);
    if (DG_CHAR(0x8fbac) != 0)
        return;
    if (DG_INT(0x685f4) != 0)
        return;
    EQ_METHOD(0, void, 0x00465128, (void*), self)();
    setRect(f_6f0, 0xc, 0x101, 0x50, 0x112);
    DG_DRAW(EQ_GLOBAL(void*, 0x006b3660), f_6f0);
    goto L_0044FE15;
#undef P06_RECT
#undef P06_RX
#undef P06_GEMI
#undef P06_TEXT16
#undef P06_STRLEN
#undef P06_READY
#undef P06_EB0
#undef P06_ELAPSED
#undef P06_E48
#undef P06_I844
#undef P06_SPELL
#undef P06_SLOTSPELL
#undef P06_NAME
#undef P06_BAG
#undef P06_I
#undef P06_WIDTH
#undef P06_SUMMON
    // END REGION P06

    // REGION P07 0x00448F0D-0x0044D2CB: 17342 B, switch 00446DA7 case 4
    //   defines L_00448F0D; exits -; returns
    // P03's inventory windows again, as dirty-rect redraws: most parts draw only when [this+0xc] (full
    // redraw) or their own flag is set, and the backgrounds come back through the dirty rect at
    // [0x006e4e48]+0xa4 clipped to each part. clipTop/clipBottom/carryEax hold the asm's register values.
#define P07_RX(slot, off) DG_INT((DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + (off))
#define P07_SLOT(slot, off) ((DG_CHAR(0xfe0) * 400 + (slot)) * 0x10 + DG_THIS + (off))
#define P07_IDX(slot, off) DG_INT((DG_CHAR(0xfe0) * 400 + (slot)) * 4 + (off))
#define P07_COUNT(item) \
    (((int)*(unsigned char*)((char*)(item) + 0xda) - 1) / (int)*(signed char*)((char*)(item) + 0xbb) + 1)
#define P07_STACK(slot, off, item, dx, dy, tx, ty) \
    setRect(f_6f0, P07_RX(slot, (off) + 8) - (dx), P07_RX(slot, (off) + 0xc) - (dy), \
            P07_RX(slot, (off) + 8), P07_RX(slot, (off) + 0xc)); \
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37fc), f_6f0); \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, P07_COUNT(item)); \
    DG_TEXT(f_6bc, P07_RX(slot, (off) + 8) - (tx), P07_RX(slot, (off) + 0xc) - (ty), 10, DG_FONT3);
#define P07_DEC(p) (((p) = (short*)((int)(p) - 1)) != 0)
#define P07_ITEMLOOP(base, count, off, item) \
    f_6f4 = 0; \
    f_6fc = (base); \
    f_6dc = (short*)(count); \
    do \
    { \
        if ((item) != 0) \
        { \
            setRect(f_6f0, P07_RX(f_6f4, off) + 3, P07_RX(f_6f4, (off) + 4) + 3, 0x280, 0x1e0); \
            DG_DRAW(DG_ITEMIMG(*(short*)((item) + 0x84)), f_6f0); \
            if (DG_COMBINE(item) != 0) \
            { \
                P07_STACK(f_6f4, off, item, 0x12, 0xe, 0xd, 9) \
            } \
        } \
        f_6f4++; \
        f_6fc += 4; \
    } while (P07_DEC(f_6dc));
#define P07_CMP3(v, base) ((int)(v) > (int)(base) ? 0xe : ((int)(v) < (int)(base) ? 0xd : 7))
#define P07_NUM(val, x, y) \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, x, y, 7, DG_FONT);
#define P07_BANK(off, val) \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, DG_RX(off) + 0x28, DG_RY(off) + 5, 10, DG_FONT);
#define P07_STAT(y, val, pcoff) \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, 0x46, y, P07_CMP3(val, *(unsigned char*)(DG_PC + (pcoff))), DG_FONT);
#define P07_RESIST(y, val, base) \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, 0x55, y, P07_CMP3(val, base), DG_FONT);
#define P07_COIN(off, val) \
    DG_SPRINTF(f_6bc, (const char*)0x0053e16c, val); \
    DG_TEXT(f_6bc, DG_RX(off) + 0xa, DG_RY(off) + 0x14, 10, DG_FONT);
#define P07_HILITE() \
    if (DG_INT(8) == 0) \
        DG_CHAR(0x6c904) ^= 1; \
    if (DG_CHAR(0x6c904) != 0) \
        setRect(f_6f0, 0x167, 0x8a, 0x196, 0xb9); \
    else \
        setRect(f_6f0, 0x167, 0xb9, 0x196, 0xe8);
#define P07_HELD (*(int*)DG_INT(0x6c658))
#define P07_HELDOK (DG_SHORT(0x6c65c) >= 0x1e && DG_INT(0x6c658) != 0 && P07_HELD != 0)
#define P07_NOWINDOW \
    (DG_INT(0x685e0) == 0 && DG_INT(0x685dc) == 0 && DG_INT(0x685e4) == 0 && DG_INT(0x685e8) == 0)
#define P07_STATS_STORE() \
    DG_CHAR(0x8fbd2) = DG_PC[0x38]; \
    DG_SHORT(0x8fbd4) = (short)(int)f_6d8; \
    DG_SHORT(0x8fbd6) = (short)f_6e0; \
    DG_SHORT(0x8fbd8) = (short)f_6fc; \
    DG_SHORT(0x8fbdc) = (short)f_6f4; \
    DG_SHORT(0x8fbe2) = (short)f_628; \
    DG_SHORT(0x8fbe0) = (short)f_624; \
    DG_SHORT(0x8fbe4) = (short)f_620; \
    DG_SHORT(0x8fbe6) = (short)(int)f_61c; \
    DG_SHORT(0x8fbe8) = (short)f_618; \
    DG_SHORT(0x8fbea) = (short)f_614; \
    DG_SHORT(0x8fbec) = (short)f_610; \
    DG_CHAR(0x8fbee) = (char)f_60c; \
    DG_CHAR(0x8fbef) = (char)f_608; \
    DG_CHAR(0x8fbf0) = (char)f_604; \
    DG_CHAR(0x8fbf1) = (char)f_600; \
    DG_CHAR(0x8fbf2) = (char)f_5fc; \
    DG_SHORT(0x8fbf4) = (short)f_6d0; \
    DG_SHORT(0x8fbf6) = (short)f_6d4;
L_00448F0D:
    if (DG_INT(0xc) != 0)
    {
        // full redraw: the window backgrounds
        if (DG_INT(0x685e0) != 0 || DG_INT(0x685dc) != 0)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b3804), DG_THIS + 0x7442c);
        else if (DG_INT(0x685e4) != 0)
        {
            if (EQ_GLOBAL(char*, 0x006e4d88) != 0 && *(EQ_GLOBAL(char*, 0x006e4d88) + 0x98) == 1)
            {
                setRect(f_6f0, 0x190, 0, 0x207, 0x1e0);
                setRect(f_6cc, 0, 0, 0x280, 0x1e0);
                DG_PART(DG_OVL(0x006e4e0c), f_6f0, f_6cc);
            }
            else
                DG_DRAW(EQ_GLOBAL(void*, 0x006b38d4), DG_THIS + 0x7443c);
        }
        else if (DG_INT(0x685e8) != 0)
        {
            setRect(f_6f0, 0x79, 0, 0xf0, 0x1e0);
            DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6d48c));
        }
        else
        {
            DG_DRAW(EQ_GLOBAL(void*, 0x006b36c8), DG_THIS + 0x7446c);
            if (DG_INT(0x8fb90) == 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36d0), DG_DR(0x6e05c));
            if (DG_INT(0x8fb90) != 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36d8), DG_DR(0x6caec));
            else
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36dc), DG_DR(0x6cafc));
        }
        if (DG_CHAR(0x100c) == 0)
            DG_DRAW(EQ_GLOBAL(void*, 0x006b36cc), DG_DR(0x6e04c));
    }

    // 0044909B
    if (DG_INT(0x685e0) == 0 && DG_INT(0x685dc) == 0)
    {
        if (DG_INT(0x685e4) != 0)
        {
            if (EQ_GLOBAL(char*, 0x006e4d88) != 0 && *(EQ_GLOBAL(char*, 0x006e4d88) + 0x98) == 1)
            {
                // 004490D8: merchant
                setRect(f_6f0, 0x190, 0, 0x207, 0x1e0);
                setRect(f_6cc, 0, 0, 0x280, 0x1e0);
                DG_PART(DG_OVL(0x006e4e0c), f_6f0, f_6cc);
                DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(EQ_GLOBAL(char*, 0x006e4d88) + 1), 0x5a));
                DG_TEXT(f_6bc, 10, 0xf, 0xe, DG_FONT);
                P07_ITEMLOOP(DG_THIS + 0x6c50c, 4, 0x6dc6c, *(char**)f_6fc)
                P07_NUM(DG_INT(0x6c52c), 0x32, 0x10f)
                P07_NUM(DG_INT(0x6c530), 0x32, 0x120)
                P07_NUM(DG_INT(0x6c534), 0x32, 0x131)
                P07_NUM(DG_INT(0x6c538), 0x32, 0x142)
                if (DG_SHORT(0x5f188) != -1)
                {
                    if (DG_SHORT(0x5f188) == 0x143)
                    {
                        setRect(f_6f0, 0x168, 0x140, 0x190, 0x168);
                        DG_PART(DG_OVL(0x006e4e0c), f_6f0, DG_DR(0x6dd3c));
                    }
                    if (DG_SHORT(0x5f188) == 0x144)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b38dc), DG_DR(0x6dd4c));
                }
                if (DG_INT(0x685ec) != 0)
                {
                    setRect(f_6f0, 0x168, 0x140, 0x190, 0x168);
                    DG_PART(DG_OVL(0x006e4e0c), f_6f0, DG_DR(0x6dd3c));
                }
            }
            else
            {
                // 0044945D: trade
                DG_DRAW(EQ_GLOBAL(void*, 0x006b38d4), DG_DR(0x6e03c));
                DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(EQ_GLOBAL(char*, 0x006e4d88) + 1), 0x5a));
                DG_TEXT(f_6bc, 10, 0xf, DG_INT(0x685f0) != 0 ? 0xe : 0xd, DG_FONT);
                DG_STRCPY(f_6bc, DG_TRUNC(EQ_GLOBAL(char*, 0x006e4d8c) + 1, 0x5a));
                DG_TEXT(f_6bc, 0x73, 0xf, DG_INT(0x685ec) != 0 ? 0xe : 0xd, DG_FONT);
                P07_ITEMLOOP(DG_THIS + 0x6c4ec, 8, 0x6dbcc, *(char**)f_6fc)
                P07_ITEMLOOP(DG_THIS + 0x6c50c, 8, 0x6dc6c, *(char**)f_6fc)
                P07_NUM(DG_INT(0x6c53c), 0x2d, 0xf6)
                P07_NUM(DG_INT(0x6c540), 0x2d, 0x107)
                P07_NUM(DG_INT(0x6c544), 0x2d, 0x118)
                P07_NUM(DG_INT(0x6c548), 0x2d, 0x129)
                P07_NUM(DG_INT(0x6c52c), 0x78, 0xf6)
                P07_NUM(DG_INT(0x6c530), 0x78, 0x107)
                P07_NUM(DG_INT(0x6c534), 0x78, 0x118)
                P07_NUM(DG_INT(0x6c538), 0x78, 0x129)
                if (DG_SHORT(0x5f188) != -1)
                {
                    if (DG_SHORT(0x5f188) == 0x143)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b38d8), DG_DR(0x6dd3c));
                    if (DG_SHORT(0x5f188) == 0x144)
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b38dc), DG_DR(0x6dd4c));
                }
                if (DG_INT(0x685ec) != 0)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b38d8), DG_DR(0x6dd3c));
            }
        }
        else if (DG_INT(0x685e8) != 0)
        {
            // 00449A0C: bank or house chest
            setRect(f_6f0, 0x79, 0, 0xf0, 0x1e0);
            DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6d48c));
            if (DG_CHAR(0x101) != 0)
                DG_STRCPY(f_6bc, 0x0055a048);
            else
                DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(EQ_GLOBAL(char*, 0x006e4dbc) + 1), 0x5a));
            DG_TEXT(f_6bc, 10, 0xf, 10, DG_FONT);
            if (DG_CHAR(0x101) != 0)
            {
                P07_ITEMLOOP(DG_THIS + 0x108, 8, 0x6d3ac, *(char**)f_6fc)
            }
            else
            {
                P07_ITEMLOOP(0xf44, 8, 0x6d3ac, *(char**)(f_6fc + (int)DG_PC))
            }
            if (DG_CHAR(0x101) != 0)
            {
                P07_BANK(0x6d44c, DG_INT(0x128))
                P07_BANK(0x6d45c, DG_INT(0x12c))
                P07_BANK(0x6d46c, DG_INT(0x130))
                P07_BANK(0x6d47c, DG_INT(0x134))
            }
            else
            {
                P07_BANK(0x6d44c, *(int*)(DG_PC + 0x9a8))
                P07_BANK(0x6d45c, *(int*)(DG_PC + 0x9ac))
                P07_BANK(0x6d46c, *(int*)(DG_PC + 0x9b0))
                P07_BANK(0x6d47c, *(int*)(DG_PC + 0x9b4))
            }
        }
        else
        {
            // 00449FF0: no window
            if (DG_CHAR(0x100c) != 0)
            {
                setRect(f_6f0, 0x1de, 0, 0x257, 0x1e0);
                setRect(f_6cc, 0x207, 0, 0x280, 0x1e0);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
                if (DG_INT(0x84) != 0)
                {
                    DG_STRCPY(f_6bc, DG_TRUNC(DG_TRIM(DG_INT(0x84) + 1), 0x5a));
                    DG_TEXT(f_6bc, DG_RX(0x6df5c) + 5, DG_RY(0x6df5c), 10, DG_FONT);
                }
                EQ_METHOD(0, void, 0x0046a491, (void*), self)();
            }
            // 0044A0B3: restore the background under the dirty rect
            if (DG_SLOT(0x845e4) != 0)
            {
                f_6d4 = DG_IR(0x006e4e48, 0xa8);
                f_6fc = DG_IR(0x006e4e48, 0xa4);
                clipTop = DG_RX(0x6e06c);
                carryEax = f_6fc - clipTop;
                if (carryEax < 0)
                {
                    carryEax = 0;
                    f_6fc = clipTop;
                }
                f_6f8 = DG_IR(0x006e4e48, 0xac) - clipTop;
                if (f_6f8 > DG_RX(0x6e074) - clipTop)
                    f_6f8 = DG_RX(0x6e074) - clipTop;
                clipTop = DG_RX(0x6e070);
                f_6f4 = f_6d4 - clipTop;
                if (f_6f4 < 0)
                {
                    f_6f4 = 0;
                    f_6d4 = clipTop;
                }
                clipBottom = DG_IR(0x006e4e48, 0xb0) - clipTop;
                if (clipBottom > DG_RX(0x6e078) - clipTop)
                    clipBottom = DG_RX(0x6e078) - clipTop;
                carryEax += DG_IR(0x006b36c8, 4);
                if (carryEax > DG_IR(0x006b36c8, 0xc))
                    carryEax = DG_IR(0x006b36c8, 0xc);
                f_6f8 += DG_IR(0x006b36c8, 4);
                if (f_6f8 > DG_IR(0x006b36c8, 0xc))
                    f_6f8 = DG_IR(0x006b36c8, 0xc);
                f_63c = DG_IR(0x006b36c8, 8);
                clipTop = f_6f4 + f_63c;
                if (clipTop > DG_IR(0x006b36c8, 0x10))
                    clipTop = DG_IR(0x006b36c8, 0x10);
                clipBottom += f_63c;
                if (clipBottom > DG_IR(0x006b36c8, 0x10))
                    clipBottom = DG_IR(0x006b36c8, 0x10);
                if (f_6f8 >= carryEax && clipBottom >= clipTop)
                {
                    setRect(f_6f0, carryEax, clipTop, f_6f8, clipBottom);
                    setRect(f_6cc, f_6fc, f_6d4, f_6fc + 1, f_6d4 + 1);
                    DG_PART(DG_OVL(0x006e4e3c), f_6f0, f_6cc);
                }
            }
            // 0044A1FC
            if (DG_INT(0x8fb90) == 0 && DG_SLOT(0x845e0) != 0)
            {
                f_6d4 = DG_IR(0x006e4e48, 0xa8);
                f_6fc = DG_IR(0x006e4e48, 0xa4);
                carryEax = DG_RX(0x6e05c);
                f_6d0 = f_6fc - carryEax;
                if (f_6d0 < 0)
                {
                    f_6d0 = 0;
                    f_6fc = carryEax;
                }
                f_6f8 = DG_IR(0x006e4e48, 0xac) - carryEax;
                if (f_6f8 > DG_RX(0x6e064) - carryEax)
                    f_6f8 = DG_RX(0x6e064) - carryEax;
                clipTop = DG_RX(0x6e060);
                f_6f4 = f_6d4 - clipTop;
                if (f_6f4 < 0)
                {
                    f_6f4 = 0;
                    f_6d4 = clipTop;
                }
                carryEax = DG_IR(0x006e4e48, 0xb0) - clipTop + 2;
                if (carryEax > DG_RX(0x6e068) - clipTop + 2)
                    carryEax = DG_RX(0x6e068) - clipTop + 2;
                f_6d0 += DG_IR(0x006b36d0, 4);
                if (f_6d0 > DG_IR(0x006b36d0, 0xc))
                    f_6d0 = DG_IR(0x006b36d0, 0xc);
                f_6f8 += DG_IR(0x006b36d0, 4);
                if (f_6f8 > DG_IR(0x006b36d0, 0xc))
                    f_6f8 = DG_IR(0x006b36d0, 0xc);
                clipTop = f_6f4 + DG_IR(0x006b36d0, 8);
                if (clipTop > DG_IR(0x006b36d0, 0x10))
                    clipTop = DG_IR(0x006b36d0, 0x10);
                carryEax += DG_IR(0x006b36d0, 8);
                if (carryEax > DG_IR(0x006b36d0, 0x10))
                    carryEax = DG_IR(0x006b36d0, 0x10);
                if (f_6f8 >= f_6d0 && carryEax >= clipTop)
                {
                    setRect(f_6f0, f_6d0, clipTop, f_6f8, carryEax);
                    setRect(f_6cc, f_6fc, f_6d4, f_6fc + 1, f_6d4 + 1);
                    DG_PART(DG_OVL(0x006e4e3c), f_6f0, f_6cc);
                }
            }
        }
    }
    else
    {
        // 0044A541: loot / give windows
        if (DG_SLOT(0x845d4) != 0 || DG_SLOT(0x8a394) != 0)
        {
            DG_SLOT(0x8a394) = 0;
            f_6d4 = DG_IR(0x006e4e48, 0xa8);
            f_6fc = DG_IR(0x006e4e48, 0xa4);
            clipTop = DG_RX(0x6e02c);
            carryEax = f_6fc - clipTop;
            if (carryEax < 0)
            {
                carryEax = 0;
                f_6fc = clipTop;
            }
            f_6f8 = DG_IR(0x006e4e48, 0xac) - clipTop;
            if (f_6f8 > DG_RX(0x6e034) - clipTop)
                f_6f8 = DG_RX(0x6e034) - clipTop;
            clipTop = DG_RX(0x6e030);
            f_6f4 = f_6d4 - clipTop;
            if (f_6f4 < 0)
            {
                f_6f4 = 0;
                f_6d4 = clipTop;
            }
            clipBottom = DG_IR(0x006e4e48, 0xb0) - clipTop;
            if (clipBottom > DG_RX(0x6e038) - clipTop)
                clipBottom = DG_RX(0x6e038) - clipTop;
            carryEax += DG_IR(0x006b3804, 4);
            if (carryEax > DG_IR(0x006b3804, 0xc))
                carryEax = DG_IR(0x006b3804, 0xc);
            f_6f8 += DG_IR(0x006b3804, 4);
            if (f_6f8 > DG_IR(0x006b3804, 0xc))
                f_6f8 = DG_IR(0x006b3804, 0xc);
            f_63c = DG_IR(0x006b3804, 8);
            clipTop = f_6f4 + f_63c;
            if (clipTop > DG_IR(0x006b3804, 0x10))
                clipTop = DG_IR(0x006b3804, 0x10);
            clipBottom += f_63c;
            if (clipBottom > DG_IR(0x006b3804, 0x10))
                clipBottom = DG_IR(0x006b3804, 0x10);
            if (f_6f8 > carryEax && clipBottom > clipTop)
            {
                setRect(f_6f0, carryEax, clipTop, f_6f8, clipBottom);
                setRect(f_6cc, f_6fc, f_6d4, f_6fc, f_6d4);
                if (DG_INT(0x685dc) != 0)
                    DG_PART(DG_OVL(0x006e4e34), f_6f0, f_6cc);
                if (DG_INT(0x685e0) != 0)
                    DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
            }
        }
        if (DG_INT(0x685e0) != 0)
        {
            // 0044A6DB: loot slots
            f_6f8 = 0;
            f_6fc = 0;
            do
            {
                if (P07_IDX(f_6fc, 0x8a1dc) != 0 || P07_IDX(f_6fc, 0x8441c) != 0 || DG_INT(0xc) != 0)
                {
                    P07_IDX(f_6fc, 0x8a1dc) = 0;
                    DG_SLOT(0x8a260) = 1;
                    DG_SLOT(0x8a264) = 1;
                    f_6f4 = (short)(f_6f8 + DG_CHAR(0x6c592) * 2) * 4 + DG_THIS + 0x6c5e0;
                    if (*(int*)f_6f4 != 0 && (P07_IDX(f_6fc, 0x8441c) != 0 || DG_INT(0xc) != 0))
                    {
                        setRect(f_6f0, P07_RX(f_6fc, 0x6d94c) + 3, P07_RX(f_6fc, 0x6d950) + 3, 0x280, 0x1e0);
                        DG_DRAWIMAGE(f_6f0, *(short*)(*(char**)f_6f4 + 0x84));
                        if (DG_COMBINE(*(char**)f_6f4) != 0)
                        {
                            P07_STACK(f_6fc, 0x6d94c, *(char**)f_6f4, 0x12, 0xe, 0xd, 9)
                        }
                    }
                }
                f_6f8++;
                f_6fc++;
            } while ((short)f_6f8 < 0xc);
            // 0044A8BA: the held item
            if (DG_SLOT(0x8a1d8) != 0 || DG_SLOT(0x84418) != 0 || DG_INT(0xc) != 0)
            {
                DG_SLOT(0x8a1d8) = 0;
                DG_SLOT(0x8a260) = 1;
                DG_SLOT(0x8a264) = 1;
                if (DG_INT(0x6c658) != 0 && P07_HELD != 0 && (DG_SLOT(0x84418) != 0 || DG_INT(0xc) != 0))
                {
                    setRect(f_6f0, DG_RX(0x6d93c) + 3, DG_RY(0x6d93c) + 3, 0x280, 0x1e0);
                    DG_DRAWIMAGE(f_6f0, *(short*)(P07_HELD + 0x84));
                    if (DG_COMBINE(P07_HELD) != 0)
                    {
                        P07_STACK(0, 0x6d93c, P07_HELD, 0x12, 0xe, 0xd, 9)
                    }
                }
            }
        }
        else if (DG_INT(0x685dc) != 0)
        {
            // 0044AA62: give slots
            f_6f8 = 0;
            f_6fc = 0;
            do
            {
                if (P07_IDX(f_6fc, 0x8a218) != 0 || P07_IDX(f_6fc, 0x84458) != 0 || DG_INT(0xc) != 0)
                {
                    P07_IDX(f_6fc, 0x8a218) = 0;
                    DG_SLOT(0x8a260) = 1;
                    DG_SLOT(0x8a264) = 1;
                    f_6f4 = (short)(f_6f8 + DG_CHAR(0x6c592) * 2) * 4 + DG_THIS + 0x6c5e0;
                    if (*(int*)f_6f4 != 0 && (P07_IDX(f_6fc, 0x84458) != 0 || DG_INT(0xc) != 0))
                    {
                        setRect(f_6f0, P07_RX(f_6fc, 0x6da3c) + 3, P07_RX(f_6fc, 0x6da40) + 3, 0x280, 0x1e0);
                        DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)f_6f4 + 0x84)), f_6f0);
                        if (DG_COMBINE(*(char**)f_6f4) != 0)
                        {
                            P07_STACK(f_6fc, 0x6da3c, *(char**)f_6f4, 0x12, 0xe, 0xd, 9)
                        }
                    }
                }
                f_6f8++;
                f_6fc++;
            } while ((short)f_6f8 < 0x10);
        }

        // 0044AC4B: highlight the held item's loot slot
        if (DG_INT(0x685e0) != 0 && DG_INT(0x685dc) == 0 && DG_INT(0x6c658) != 0)
        {
            f_6f8 = 0;
            f_6e0 = 0;
            do
            {
                f_6d8 = (short*)P07_HELD;
                if (f_6d8 != 0 && (int)f_6d8 == DG_INT((short)(f_6f8 + DG_CHAR(0x6c592) * 2) * 4 + 0x6c5e0))
                {
                    P07_HILITE()
                    DG_PART(DG_OVL(0x006e4e24), f_6f0, P07_SLOT(f_6e0, 0x6d94c));
                    DG_SLOT(0x844a0) = 1;
                    DG_SLOT(0x844a4) = 1;
                }
                f_6f8++;
                f_6e0++;
            } while ((short)f_6f8 < 0xc);
        }
        // 0044AD5F: page arrows
        if (DG_INT(0xc) != 0 || DG_SLOT(0x844a0) != 0 || DG_SLOT(0x8a260) != 0)
        {
            if (DG_INT(0x685e0) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 - 1);
            if (DG_INT(0x685dc) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 - 1);
            DG_SLOT(0x8a260) = 0;
            if ((short)f_6fc >= 0)
            {
                clipTop = (short)f_6fc * 4 + DG_THIS + 0x6c5e0;
                f_6dc = (short*)((short)f_6fc + 1);
                do
                {
                    if (*(int*)clipTop != 0)
                        DG_SLOT(0x8a260) = 1;
                    clipTop -= 4;
                } while (P07_DEC(f_6dc));
            }
            if (DG_SLOT(0x8a260) != 0)
            {
                DG_SLOT(0x8a260) = 0;
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3864), DG_DR(0x6db5c));
            }
        }
        if (DG_INT(0xc) != 0 || DG_SLOT(0x844a4) != 0 || DG_SLOT(0x8a264) != 0)
        {
            if (DG_INT(0x685e0) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 + 0xc);
            if (DG_INT(0x685dc) != 0)
                f_6fc = (short)(DG_CHAR(0x6c592) * 2 + 0x10);
            DG_SLOT(0x8a264) = 0;
            if ((short)f_6fc < 0x1e)
            {
                clipTop = (short)f_6fc * 4 + DG_THIS + 0x6c5e0;
                carryEax = 0x1e - (short)f_6fc;
                do
                {
                    if (*(int*)clipTop != 0)
                        DG_SLOT(0x8a264) = 1;
                    clipTop += 4;
                } while (--carryEax != 0);
            }
            if (DG_SLOT(0x8a264) != 0)
            {
                DG_SLOT(0x8a264) = 0;
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3868), DG_DR(0x6db6c));
            }
        }

        // 0044AF31: window title
        if (DG_SLOT(0x845d4) != 0 || DG_INT(0xc) != 0)
        {
            if (DG_INT(0x685e0) != 0 && EQ_GLOBAL(char*, 0x006e4db8) != 0)
            {
                DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4db8) + 1));
                DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
            }
            if (DG_INT(0x685dc) != 0)
            {
                if (EQ_GLOBAL(char*, 0x006e4dc0) != 0)
                {
                    DG_STRCPY(f_6bc, DG_TRIM(EQ_GLOBAL(char*, 0x006e4dc0) + 1));
                    DG_STRCPY(f_6bc, DG_TRUNC(f_6bc, 0x5a));
                }
                else
                    DG_STRCPY(f_6bc, 0x0055a040);
            }
            DG_TEXT(f_6bc, 10, 0xd, 10, DG_FONT);
        }
        // 0044B008
        if (DG_INT(0x685e0) != 0)
        {
            if (DG_SHORT(0x5f18a) == 0x112 || DG_SHORT(0x5f188) == 0x112 || DG_SLOT(0x84454) != 0
                || DG_INT(0xc) != 0)
            {
                if (DG_SHORT(0x5f188) == 0x112)
                {
                    if (P07_HELDOK)
                        setRect(f_6f0, 0x168, 0x77, 0x1b6, 0x89);
                    else
                        setRect(f_6f0, 0x168, 0x64, 0x1b6, 0x76);
                }
                else if (P07_HELDOK)
                    setRect(f_6f0, 0x168, 0x51, 0x1b6, 0x63);
                else
                    setRect(f_6f0, 0x106, 0x1c2, 0x154, 0x1d4);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6da2c));
            }
        }
        if (DG_INT(0x685e0) != 0 && DG_INT(0xa0) == 0)
        {
            if (P07_HELDOK)
                setRect(f_6f0, 0x168, 0x77, 0x1b6, 0x89);
            else
                setRect(f_6f0, 0x168, 0x64, 0x1b6, 0x76);
            DG_PART(DG_OVL(0x006e4e24), f_6f0, DG_DR(0x6da2c));
        }
        if (DG_SHORT(0x5f188) != -1)
        {
            switch (DG_SHORT(0x5f188))
            {
            case 0x110:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3808), DG_DR(0x6da0c));
                break;
            case 0x111:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b380c), DG_DR(0x6da1c));
                break;
            case 0x123:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3808), DG_DR(0x6db3c));
                break;
            case 0x124:
                DG_DRAW(EQ_GLOBAL(void*, 0x006b380c), DG_DR(0x6db4c));
                break;
            }
        }
        else
        {
            if (DG_INT(0x685e0) != 0 && (DG_SHORT(0x5f18a) == 0x110 || DG_SLOT(0x8444c) != 0))
            {
                setRect(f_6f0, 0x1af, 0x1b2, 0x1d6, 0x1d3);
                DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6da0c));
            }
            if (DG_INT(0x685e0) != 0 && (DG_SHORT(0x5f18a) == 0x111 || DG_SLOT(0x84450) != 0))
            {
                setRect(f_6f0, 0x1e0, 0x1b2, 0x207, 0x1d3);
                DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6da1c));
            }
            if (DG_INT(0x685dc) != 0 && (DG_SHORT(0x5f18a) == 0x123 || DG_SLOT(0x84498) != 0))
            {
                setRect(f_6f0, 0x1af, 0x1b2, 0x1d6, 0x1d3);
                DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6db3c));
            }
            if (DG_INT(0x685dc) != 0 && (DG_SHORT(0x5f18a) == 0x124 || DG_SLOT(0x8449c) != 0))
            {
                setRect(f_6f0, 0x1e0, 0x1b2, 0x207, 0x1d3);
                DG_PART(DG_OVL(0x006e4e34), f_6f0, DG_DR(0x6db4c));
            }
        }
    }

    // 0044A357
    if (DG_SLOT(0x845dc) != 0 && DG_CHAR(0x100c) == 0)
    {
        f_6d4 = DG_IR(0x006e4e48, 0xa8);
        f_6fc = DG_IR(0x006e4e48, 0xa4);
        clipTop = DG_RX(0x6e04c);
        f_6d0 = f_6fc - clipTop;
        if (f_6d0 < 0)
        {
            f_6d0 = 0;
            f_6fc = clipTop;
        }
        f_6f8 = DG_IR(0x006e4e48, 0xac) - clipTop;
        if (f_6f8 > DG_RX(0x6e054) - clipTop)
            f_6f8 = DG_RX(0x6e054) - clipTop;
        clipTop = DG_RX(0x6e050);
        f_6f4 = f_6d4 - clipTop;
        if (f_6f4 < 0)
        {
            f_6f4 = 0;
            f_6d4 = clipTop;
        }
        clipBottom = DG_IR(0x006e4e48, 0xb0) - clipTop;
        if (clipBottom > DG_RX(0x6e058) - clipTop)
            clipBottom = DG_RX(0x6e058) - clipTop;
        f_6d0 += DG_IR(0x006b36cc, 4);
        if (f_6d0 > DG_IR(0x006b36cc, 0xc))
            f_6d0 = DG_IR(0x006b36cc, 0xc);
        f_6f8 += DG_IR(0x006b36cc, 4);
        if (f_6f8 > DG_IR(0x006b36cc, 0xc))
            f_6f8 = DG_IR(0x006b36cc, 0xc);
        f_63c = DG_IR(0x006b36cc, 8);
        f_6f4 += f_63c;
        if (f_6f4 > DG_IR(0x006b36cc, 0x10))
            f_6f4 = DG_IR(0x006b36cc, 0x10);
        clipBottom += f_63c;
        if (clipBottom > DG_IR(0x006b36cc, 0x10))
            clipBottom = DG_IR(0x006b36cc, 0x10);
        if (f_6f8 > f_6d0 && clipBottom > f_6f4)
        {
            setRect(f_6f0, f_6d0, f_6f4, f_6f8, clipBottom);
            setRect(f_6cc, f_6fc, f_6d4, f_6fc + 1, f_6d4 + 1);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, f_6cc);
        }
    }
    // 0044A4D9
    if (DG_CHAR(0x100c) == 0)
    {
        if (DG_SHORT(0x5f188) != 0x27 && (DG_SLOT(0x840a8) != 0 || DG_SLOT(0x89e68) != 0 || DG_INT(0xc) != 0))
        {
            if (DG_INT(0x8fba4) != 0)
                setRect(f_6f0, 0x159, 0x17e, 0x1bd, 0x18f);
            else
                setRect(f_6f0, 0x82, 8, 0xe6, 0x19);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb7c));
        }
        // 0044B41A
        if (DG_SHORT(0x5f188) != 0x21 && (DG_SLOT(0x84090) != 0 || DG_SHORT(0x5f18a) == 0x21 || DG_INT(0xc) != 0))
        {
            setRect(f_6f0, 0x8e, 0xe8, 0xdd, 0xfb);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb1c));
        }
    }
    // 0044B499
    if ((DG_SLOT(0x843c4) != 0 || DG_SLOT(0x8a184) != 0) && EQ_GLOBAL(int, 0x006e4de8) != 0)
    {
        DG_SLOT(0x8a184) = 0;
        *(int*)(EQ_GLOBAL(char*, 0x006e4de8) + 0x1ec) = 1;
    }
    if (DG_SHORT(0x5f188) >= 0)
    {
        switch (DG_SHORT(0x5f188))
        {
        case 0x1e:
            if (DG_INT(0x8fb90) == 0 && P07_NOWINDOW)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36d8), DG_DR(0x6caec));
            break;
        case 0x1f:
            if (DG_INT(0x8fb90) != 0 && P07_NOWINDOW)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36dc), DG_DR(0x6cafc));
            break;
        case 0x21:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b36e4), DG_DR(0x6cb1c));
            break;
        case 0x22:
            DG_DRAW(EQ_GLOBAL(void*, 0x006b37c0), DG_DR(0x6cb2c));
            break;
        case 0x27:
            setRect(f_6f0, 0x159, 0x17e, 0x1bd, 0x18f);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb7c));
            break;
        }
    }
    // 0044B629: the slot the pointer left
    if (DG_SHORT(0x5f18a) != -1)
    {
        if (DG_SLOT(0x84084) != 0 || DG_SHORT(0x5f18a) == 0x1e)
        {
            if (DG_INT(0x8fb90) != 0)
            {
                if (P07_NOWINDOW)
                    DG_DRAW(EQ_GLOBAL(void*, 0x006b36d8), DG_DR(0x6caec));
            }
            else if (P07_NOWINDOW)
            {
                setRect(f_6f0, 10, 0xc, 0x39, 0x1f);
                DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6caec));
            }
        }
        if (DG_SLOT(0x84088) != 0 || DG_SHORT(0x5f18a) == 0x1f)
        {
            if (DG_INT(0x8fb90) != 0)
            {
                if (P07_NOWINDOW)
                {
                    setRect(f_6f0, 0x3e, 0xc, 0x6c, 0x1f);
                    DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cafc));
                }
            }
            else if (P07_NOWINDOW)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36dc), DG_DR(0x6cafc));
        }
        if (DG_SLOT(0x84094) != 0 || DG_SHORT(0x5f18a) == 0x22)
        {
            setRect(f_6f0, 0x9d, 0x1c3, 0xcc, 0x1d5);
            DG_PART(DG_OVL(0x006e4e3c), f_6f0, DG_DR(0x6cb2c));
        }
    }
    // 0044B838
    if (P07_NOWINDOW)
    {
        if (DG_INT(0x8fb90) != 0)
        {
            // spell book page
            if (DG_INT(0xc) != 0 || DG_SLOT(0x84084) != 0 || DG_SLOT(0x89e44) != 0)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36d8), DG_DR(0x6caec));
                DG_SLOT(0x89e44) = 0;
            }
            if (DG_INT(0xc) != 0 || DG_SLOT(0x840b4) != 0 || DG_INT(0x5f174) != 0)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3640), DG_DR(0x6cbac));
            if (DG_SHORT(0x5f188) == 0x28)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b3638), DG_DR(0x6cb8c));
            if (DG_SHORT(0x5f188) == 0x29)
                DG_DRAW(EQ_GLOBAL(void*, 0x006b363c), DG_DR(0x6cb9c));
            if (DG_SHORT(0x5f18a) == 0x28)
                DG_PART(DG_OVL(0x006e4e3c), DG_DR(0x6cb8c), DG_DR(0x6cb8c));
            if (DG_SHORT(0x5f18a) == 0x29)
                DG_PART(DG_OVL(0x006e4e3c), DG_DR(0x6cb9c), DG_DR(0x6cb9c));
            EQ_METHOD(0, void, 0x00465ade, (void*), self)();
        }
        else
        {
            // 0044B9DB: character stats page
            f_6f4 = EQ_METHOD(0, int, 0x0045755e, (void*), self)();
            f_6d0 = EQ_METHOD(0, int, 0x0041bc98, (void*), DG_PC)();
            f_6d4 = EQ_METHOD(0, int, 0x0041ba44, (void*), DG_PC)();
            f_6d8 = (short*)EQ_METHOD(1, int, 0x004199c0, (void*, int), DG_PC)(0);
            f_6e0 = EQ_METHOD(1, int, 0x00419a2f, (void*, int), DG_PC)(0);
            f_6fc = EQ_METHOD(0, int, 0x004575e4, (void*), self)();
            f_628 = (unsigned char)EQ_METHOD(0, int, 0x0041b258, (void*), DG_PC)();
            f_624 = (unsigned char)EQ_METHOD(0, int, 0x0041b0c3, (void*), DG_PC)();
            f_620 = (unsigned char)EQ_METHOD(0, int, 0x0041b722, (void*), DG_PC)();
            f_61c = (struct EQPlayer*)(unsigned char)EQ_METHOD(0, int, 0x0041b48f, (void*), DG_PC)();
            f_618 = (unsigned char)EQ_METHOD(0, int, 0x0041b5cc, (void*), DG_PC)();
            f_614 = (unsigned char)EQ_METHOD(0, int, 0x0041b38f, (void*), DG_PC)();
            f_610 = (unsigned char)EQ_METHOD(0, int, 0x0041b88a, (void*), DG_PC)();
            f_60c = EQ_METHOD(1, int, 0x0041af14, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_608 = EQ_METHOD(1, int, 0x0041a7a0, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_604 = EQ_METHOD(1, int, 0x0041ad1c, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_600 = EQ_METHOD(1, int, 0x0041a962, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            f_5fc = EQ_METHOD(1, int, 0x0041ab4b, (void*, int), DG_PC)(*(unsigned char*)(DG_PC + 0x38));
            if (DG_CHAR(0x8fbd2) == 0)
            {
                P07_STATS_STORE()
            }
            if (DG_CHAR(0x8fbd2) != DG_PC[0x38])
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbd4) != (short)(int)f_6d8)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbd6) != (short)f_6e0)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbd8) != (short)f_6fc)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbdc) != (short)f_6f4)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe2) != (int)f_628)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe0) != (int)f_624)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe4) != (int)f_620)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe6) != (int)f_61c)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbe8) != (int)f_618)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbea) != (int)f_614)
                DG_INT(0xeb8) = 1;
            if ((int)DG_SHORT(0x8fbec) != (int)f_610)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbee) != f_60c)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbef) != f_608)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbf0) != f_604)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbf1) != f_600)
                DG_INT(0xeb8) = 1;
            if (DG_CHAR(0x8fbf2) != f_5fc)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbf4) != (short)f_6d0)
                DG_INT(0xeb8) = 1;
            if (DG_SHORT(0x8fbf6) != (short)f_6d4)
                DG_INT(0xeb8) = 1;
            P07_STATS_STORE()
            if (DG_INT(0xc) != 0 || DG_SLOT(0x84088) != 0 || DG_SLOT(0x89e48) != 0)
            {
                DG_DRAW(EQ_GLOBAL(void*, 0x006b36dc), DG_DR(0x6cafc));
                DG_SLOT(0x89e48) = 0;
            }
            if (DG_INT(0xc) != 0 || DG_SLOT(0x845e0) != 0 || DG_SLOT(0x8a3a0) != 0)
            {
                DG_SLOT(0x8a3a0) = 0;
                DG_SPRINTF(f_6bc, (const char*)0x0053e678, DG_PC);
                DG_TEXT(f_6bc, 0xf, 0x29, 7, DG_FONT);
                DG_SPRINTF(f_6bc, (const char*)0x0053e16c, *(unsigned char*)(DG_PC + 0x38));
                DG_TEXT(f_6bc, 0x3c, 0x43, 7, DG_FONT);
                DG_TEXT(EQ_METHOD(1, char*, 0x004c9d4e, (void*, short), EQ_GLOBAL(void*, 0x006efe2c))(
                            (short)*(unsigned char*)(DG_PC + 0x36)),
                        0x17, 0x52, 7, DG_FONT);
                DG_TEXT(EQ_METHOD(1, char*, 0x004c9c6d, (void*, short), EQ_GLOBAL(void*, 0x006efe2c))(
                            *(short*)(DG_PC + 0x1038)),
                        0x17, 0x61, 7, DG_FONT);
                clipTop = EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(0x45);
                clipTop += EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(0);
                f_63c = EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(0x37);
                f_6dc = (short*)(f_63c + EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(1));
                f_5f8 = EQ_METHOD(1, short, 0x004238a2, (void*, int), DG_PC)(2);
                DG_SPRINTF(f_6bc, (const char*)0x0055a034, (int)(short)(int)f_6d8, (int)(short)f_6e0);
                DG_TEXT(f_6bc, 0x32, 0x70, clipTop > 0 ? 0xe : (clipTop < 0 ? 0xd : 7), DG_FONT);
                DG_SPRINTF(f_6bc, (const char*)0x0055a028, (int)(short)f_6fc);
                DG_TEXT(f_6bc, 0x32, 0x7b, (int)f_6dc > 0 ? 0xe : ((int)f_6dc < 0 ? 0xd : 7), DG_FONT);
                DG_SPRINTF(f_6bc, (const char*)0x0055a028, (int)(short)f_6f4);
                DG_TEXT(f_6bc, 0x32, 0x86, f_5f8 > 0 ? 0xe : (f_5f8 < 0 ? 0xd : 7), DG_FONT);
                DG_EXP_PROGRESS();
                P07_STAT(0xe6, f_624, 0x77)
                P07_STAT(0xf1, f_628, 0x78)
                P07_STAT(0xfc, f_620, 0x7c)
                P07_STAT(0x107, (int)f_61c, 0x7a)
                P07_STAT(0x112, f_610, 0x7d)
                P07_STAT(0x11d, f_618, 0x7b)
                P07_STAT(0x128, f_614, 0x79)
                P07_RESIST(0x15a, f_60c, EQ_METHOD(0, int, 0x0041aee8, (void*), DG_PC)())
                P07_RESIST(0x165, f_608, EQ_METHOD(0, int, 0x0041a76d, (void*), DG_PC)())
                P07_RESIST(0x170, f_604, EQ_METHOD(0, int, 0x0041aced, (void*), DG_PC)())
                P07_RESIST(0x17b, f_600, EQ_METHOD(0, int, 0x0041a92b, (void*), DG_PC)())
                P07_RESIST(0x186, f_5fc, EQ_METHOD(0, int, 0x0041ab14, (void*), DG_PC)())
                DG_SPRINTF(f_6bc, (const char*)0x0055a034, (int)(short)f_6d0, (int)(short)f_6d4);
                DG_TEXT(f_6bc, 0x2d, 0x1ba, (short)f_6d0 > (short)f_6d4 ? 0xd : 7, DG_FONT);
            }
        }
    }

    // 0044C686: bags
    if (DG_CHAR(0x100c) == 0)
    {
        f_6fc = 0x16;
        f_6f4 = 0x138;
        f_6dc = (short*)8;
        do
        {
            if (P07_IDX(f_6fc, 0x89dcc) != 0 || P07_IDX(f_6fc, 0x8400c) != 0 || DG_INT(0xc) != 0)
            {
                P07_IDX(f_6fc, 0x89dcc) = 0;
                if (*(char**)(f_6f4 + (int)DG_PC) != 0)
                {
                    if (*(short*)(*(char**)(f_6f4 + (int)DG_PC) + 0x84) >= 0x1f4
                        && *(short*)(*(char**)(f_6f4 + (int)DG_PC) + 0x84) <= 0xbba)
                        DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(f_6f4 + (int)DG_PC) + 0x84)), P07_SLOT(f_6fc, 0x6c90c));
                    else
                        DG_DRAW(EQ_GLOBAL(void*, 0x006b3e30), P07_SLOT(f_6fc, 0x6c90c));
                    if (DG_COMBINE(*(char**)(f_6f4 + (int)DG_PC)) != 0)
                    {
                        P07_STACK(f_6fc, 0x6c90c, *(char**)(f_6f4 + (int)DG_PC), 0xf, 0xb, 0xa, 6)
                    }
                }
            }
            f_6fc++;
            f_6f4 += 4;
        } while (P07_DEC(f_6dc));
    }
    // 0044C840
    if (DG_INT(0x685e0) != 0 && DG_INT(0x685dc) == 0 && DG_INT(0x6c658) != 0)
    {
        f_6e0 = 0x16;
        f_6d8 = (short*)0x138;
        f_6dc = (short*)8;
        do
        {
            if (P07_HELD != 0 && P07_HELD == *(int*)((int)f_6d8 + (int)DG_PC))
            {
                P07_HILITE()
                setRect(f_6cc, P07_RX(f_6e0, 0x6c90c) - 3, P07_RX(f_6e0, 0x6c910) - 3, 0x280, 0x1e0);
                DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
            }
            f_6e0++;
            f_6d8 = (short*)((int)f_6d8 + 4);
        } while (P07_DEC(f_6dc));
    }
    if (DG_CHAR(0x100c) != 0)
    {
        // 0044C95E: someone else's equipment
        DG_DRAW(EQ_GLOBAL(void*, 0x006b37b0), DG_DR(0x6e07c));
        DG_DRAW(EQ_GLOBAL(void*, 0x006b37b4), DG_DR(0x6e08c));
        DG_DRAW(EQ_GLOBAL(void*, 0x006b37b8), DG_DR(0x6e09c));
        DG_DRAW(EQ_GLOBAL(void*, 0x006b37bc), DG_DR(0x6e0ac));
        f_6d4 = 1;
        f_6d0 = 0x12e6;
        f_6dc = (short*)0x15;
        do
        {
            if (DG_SHORT(f_6d0) >= 0x1f4)
                DG_DRAW(DG_ITEMIMG(DG_SHORT(f_6d0)), P07_SLOT(f_6d4, 0x6c90c));
            f_6d4++;
            f_6d0 += 2;
        } while (P07_DEC(f_6dc));
        return;
    }
    // 0044CA33: worn equipment
    if (DG_INT(0x8fba4) == 0)
        return;
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37b0), DG_DR(0x6e07c));
    f_6e0 = 1;
    f_6d8 = (short*)0xe4;
    f_6dc = (short*)5;
    do
    {
        if (*(char**)((int)f_6d8 + (int)DG_PC) != 0)
            DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)((int)f_6d8 + (int)DG_PC) + 0x84)), P07_SLOT(f_6e0, 0x6c90c));
        f_6e0++;
        f_6d8 = (short*)((int)f_6d8 + 4);
    } while (P07_DEC(f_6dc));
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37b4), DG_DR(0x6e08c));
    DG_EQSLOT(0xfc, 0x6c97c)
    DG_EQSLOT(0x104, 0x6c99c)
    DG_EQSLOT(0x108, 0x6c9ac)
    DG_EQSLOT(0x110, 0x6c9cc)
    DG_EQSLOT(0x11c, 0x6c9fc)
    DG_EQSLOT(0x120, 0x6ca0c)
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37b8), DG_DR(0x6e09c));
    DG_EQSLOT(0xf8, 0x6c96c)
    DG_EQSLOT(0x100, 0x6c98c)
    DG_EQSLOT(0x124, 0x6ca1c)
    DG_EQSLOT(0x128, 0x6ca2c)
    DG_EQSLOT(0x12c, 0x6ca3c)
    DG_EQSLOT(0x130, 0x6ca4c)
    DG_DRAW(EQ_GLOBAL(void*, 0x006b37bc), DG_DR(0x6e0ac));
    DG_EQSLOT(0x114, 0x6c9dc)
    DG_EQSLOT(0x118, 0x6c9ec)
    if (*(char**)(DG_PC + 0x10c) != 0)
    {
        DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x10c) + 0x84)), DG_DR(0x6c9bc));
        if (DG_COMBINE(*(char**)(DG_PC + 0x10c)) != 0)
        {
            P07_STACK(0, 0x6c9bc, *(char**)(DG_PC + 0x10c), 0xf, 0xb, 0xa, 6)
        }
    }
    if (*(char**)(DG_PC + 0x134) != 0)
    {
        DG_DRAW(DG_ITEMIMG(*(short*)(*(char**)(DG_PC + 0x134) + 0x84)), DG_DR(0x6ca5c));
        if (DG_COMBINE(*(char**)(DG_PC + 0x134)) != 0)
        {
            P07_STACK(0, 0x6ca5c, *(char**)(DG_PC + 0x134), 0xf, 0xb, 0xa, 6)
        }
    }
    setRect(f_6f0, 0xbc, 0x188, 0x1ea, 0x1c4);
    DG_PART(DG_OVL(0x006e4de8), f_6f0, DG_DR(0x6cf6c));
    P07_COIN(0x6cb3c, *(int*)(DG_PC + 0x998))
    P07_COIN(0x6cb4c, *(int*)(DG_PC + 0x99c))
    P07_COIN(0x6cb5c, *(int*)(DG_PC + 0x9a0))
    P07_COIN(0x6cb6c, *(int*)(DG_PC + 0x9a4))
    if (DG_INT(0x685e0) == 0)
        return;
    if (DG_INT(0x6c658) == 0)
        return;
    f_6d4 = 1;
    f_6e0 = 0xe4;
    f_6dc = (short*)0x15;
    do
    {
        if (P07_HELD != 0 && P07_HELD == *(int*)(f_6e0 + (int)DG_PC))
        {
            P07_HILITE()
            setRect(f_6cc, P07_RX(f_6d4, 0x6c90c) - 3, P07_RX(f_6d4, 0x6c910) - 3, 0x280, 0x1e0);
            DG_PART(DG_OVL(0x006e4e24), f_6f0, f_6cc);
        }
        f_6e0 += 4;
        f_6d4++;
    } while (P07_DEC(f_6dc));
    return;
#undef P07_RX
#undef P07_SLOT
#undef P07_IDX
#undef P07_COUNT
#undef P07_STACK
#undef P07_DEC
#undef P07_ITEMLOOP
#undef P07_CMP3
#undef P07_NUM
#undef P07_BANK
#undef P07_STAT
#undef P07_RESIST
#undef P07_COIN
#undef P07_HILITE
#undef P07_HELD
#undef P07_HELDOK
#undef P07_NOWINDOW
#undef P07_STATS_STORE
    // END REGION P07

}

// FUNCTION: TRILOGY 0x00457110
// @early-stop: the selected-table pointer and short index still occupy each other's original register/stack locations.
int EQMainDrawText_guess::truncateTextToWidth_guess(char* text, short maxWidth, short fontSize)
{
    short charIndex = 0;
    int textWidth = 0;
    size_t textLength;
    const int* charWidths;
    char* output = (char*)this + 0x5A6;

    ((char*)this)[0x5A6] = 0;
    if (text != 0 && *text != '\0' && maxWidth >= 0)
    {
        if (fontSize != 10 && fontSize != 12 && fontSize != 14 && fontSize != 16)
            fontSize = 14;

        switch (fontSize)
        {
        default:
            charWidths = EQ_GLOBAL(int*, 0x006EFDE0);
            break;
        case 16:
            charWidths = EQ_GLOBAL(int*, 0x006EFDE8);
            break;
        case 12:
            charWidths = EQ_GLOBAL(int*, 0x006EFDDC);
            break;
        case 10:
            charWidths = EQ_GLOBAL(int*, 0x006EFDD8);
            break;
        }

        textLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(text);
        if ((short)textLength > 0)
        {
            while (1)
            {
                if (textWidth >= maxWidth)
                    break;
                ((char*)this)[0x5A6 + (int)charIndex] = text[(int)charIndex];
                textWidth += charWidths[text[(int)charIndex]];
                charIndex++;
                if (charIndex >= (short)textLength)
                    break;
            }
        }
        ((char*)this)[0x5A6 + charIndex] = 0;
    }
    return (int)output;
}
