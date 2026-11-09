// EQ_ItemData.cpp - EQ_ItemData subsystem.

#include <string.h>
#include <stdio.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_overlay.h"
#include "trilogy/globals.h"
#include "trilogy/character.h"

class EQ_ItemData_guess
{
public:
    EQ_ItemData_guess();
    void draw_guess(void);
};

// The EQ_Overlay base of EQ_ItemData_guess, built as a local with the same ctor/dtor shape as EQ_Main14OverlayBase_guess.
// `object` is set before the base constructor runs, as in a derived constructor. Unwinding destroys the base; a normal
// return clears `object` first, so the destructor does nothing on that path.
class EQ_ItemDataOverlayBase_guess
{
public:
    EQ_Overlay* object;

    EQ_ItemDataOverlayBase_guess(EQ_Overlay* value) : object(value)
    {
        EQ_METHOD(0, void, 0x00450049, (void*), (void*)value)();
    }

    ~EQ_ItemDataOverlayBase_guess()
    {
        if (object != 0)
            EQ_METHOD(0, void, 0x0045005D, (void*), (void*)object)();
    }
};

// FUNCTION: TRILOGY 0x00438967
// Initializes the item lore window used to show item and spell details.
// ORIGINAL FILE: EverQuest/EQ_ItemData.cpp (from Mac debug map)
EQ_ItemData_guess::EQ_ItemData_guess()
{
    typedef int (__stdcall *SetRect_guess_t)(void*, int, int, int, int);

    char* self = (char*)this;
    SetRect_guess_t setRect;
    EQ_ItemDataOverlayBase_guess base((EQ_Overlay*)this);
    setRect = (SetRect_guess_t)EQ_GLOBAL(void*, 0x00534248);
    *(short*)(self + 0x94) = -1;
    *(u32*)self = 0x00535470;
    *(u32*)(self + 0x10) = 0;
    *(u32*)(self + 0x90) = 0;
    *(u32*)(self + 0x70) = 0;
    *(u32*)(self + 0x0c) = 0;
    setRect((void*)(self + 0x24), 0x8c, 0x140, 0x96, 400);
    setRect((void*)(self + 0x14), 0, 0, 0x157, 0x9e);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        (self + 0x34, (const char*)0x0053E950);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        (self + 0x52, (const char*)0x00559208);
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), (void*)self)(1, 0);
    *(short*)(self + 0x96) = -1;
    *(u32*)(self + 0x78) = 0;
    *(u8*)(self + 0xa0) = 0;
    base.object = 0;
}

// FUNCTION: TRILOGY 0x00438a1e
// Releases this display object's overlay resources during teardown.
void __fastcall ReleaseOverlayResources_guess(void* self)
{
    *(u32*)self = 0x00535470;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x00438a29
// Handles a display state change; its specific in-game purpose is unclear.
void __fastcall FUN_00438a29(void* self)
{
    if (*(u32*)((u8*)self + 0x10) == 0)
    {
        *(u8*)((u8*)self + 0xa0) = 0;
        if (EQ_GLOBAL(u32, 0x0058FC6C) == 2)
        {
            if (EQ_GLOBAL(void*, 0x006E4DA8) == 0 ||
                EQ_METHOD(0, u32, 0x004197B2, (void*), EQ_GLOBAL(void*, 0x006E4DA8))() == 0)
            {
                EQ_GLOBAL(u32, 0x00590170) = EQ_GLOBAL(u32, 0x0058FC6C);
                EQ_GLOBAL(u32, 0x0058FC6C) = 1;
                pinstEQ_Main_guess->uiDirty_u32_guess = 1;
                EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
            }
            else
            {
                u8* client = (u8*)EQ_GLOBAL(void*, 0x006E4E44);
                u32* flags = (u32*)(client + 0x90224);
                flags[4] = 1;
                flags[5] = 1;
            }
            *(u8*)((u8*)self + 0xa0) = 1;
        }
        *(u32*)((u8*)self + 0x9c) = 0;
        *(u32*)((u8*)self + 0x10) = 1;
        EQ_GLOBAL(u8, 0x006B658E) = 1;
        return;
    }
    return;
}

// FUNCTION: TRILOGY 0x00438aaf
// __cdecl: appends text to a buffer, separated by two spaces (plain copy when the buffer is empty).
void AppendText_guess(char* dst, const char* src)
{
    if (*dst == '\0') {
        strcpy(dst, src);
        return;
    }
    sprintf(dst, "%s  %s", dst, src);
}

#define STRCPY_guess        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
#define STRCAT_guess        EQ_CDECL(char*, 0x005219D0, (char*, const char*))
#define STRLEN_guess        EQ_CDECL(size_t, 0x005223D0, (const char*))
#define SPRINTF_guess       EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
#define APPEND_guess        EQ_CDECL(void, 0x00438AAF, (char*, const char*))
#define WRITE_guess(buf, px, py) \
    EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))( (buf), (px), (py), flags, EQ_GLOBAL(int, 0x006EFDFC), 1)
#define ITEM_guess          (*(char**)((char*)this + 0x90))
#define STATE_guess         EQ_GLOBAL(int, 0x0058FC6C)
#define DRAW_guess(object) EQ_METHOD(1, void, 0x00450081, (void*, int), (object))
#define EMPTY_guess         EQ_ARRAY(char, 0x0058FBA4)
#define STAT_guess(off, neg, pos) \
    ch = *(char*)(ITEM_guess + (off)); \
    if (ch != '\0') { \
        SPRINTF_guess(tmp, ch > 0 ? (pos) : (neg), (int)ch); \
        count++; \
        STRCAT_guess(line, tmp); \
    }
#define FLUSH_guess \
    if (count >= 4) { \
        WRITE_guess(line, x, y + n * 10); \
        n++; \
        count = 0; \
        STRCPY_guess(line, EMPTY_guess); \
    }
#define FLAGS_guess \
    ch = *(char*)(ITEM_guess + 0x23); \
    if (ch == '*') { \
        APPEND_guess(line, EQ_ARRAY(char, 0x005583EC)); \
        ch = *(char*)(ITEM_guess + 0x24); \
    } else if (ch == '~') { \
        APPEND_guess(line, EQ_ARRAY(char, 0x005583DC)); \
        ch = *(char*)(ITEM_guess + 0x24); \
    } \
    if (ch == '#') \
        APPEND_guess(line, EQ_ARRAY(char, 0x005583D0)); \
    else if (ch == '&') \
        APPEND_guess(line, EQ_ARRAY(char, 0x005583C4)); \
    if (*(char*)(ITEM_guess + 0x7f) == '\0') \
        APPEND_guess(line, EQ_ARRAY(char, 0x005583BC)); \
    if (*(char*)(ITEM_guess + 0x7e) == '\0') \
        APPEND_guess(line, EQ_ARRAY(char, 0x005583B4));
#define SIZENAME_guess(off) \
    ch = *(char*)(ITEM_guess + (off)); \
    if (ch == '\0') \
        name_ptr = EQ_ARRAY(char, 0x00558594); \
    else if (ch == 1) \
        name_ptr = EQ_ARRAY(char, 0x0055858C); \
    else if (ch == 2) \
        name_ptr = EQ_ARRAY(char, 0x00558584); \
    else if (ch == 3) \
        name_ptr = EQ_ARRAY(char, 0x0055857C); \
    else \
        name_ptr = EQ_ARRAY(char, 0x00558574); \
    STRCPY_guess(sizeName, name_ptr);

// FUNCTION: TRILOGY 0x00438ad4
// Writes the info lines of the shown item (name, slots, charges, skill, classes, races, deities) or spell.
// ORIGINAL FILE: EverQuest/EQ_ItemData.cpp (from Mac debug map)
// __thiscall: the item/spell info window. this+0x90 = the item shown (0 = none), this+0x94 = the spell id when a
// spell is shown (-1 = an item, +10000 = a spell without the cast lines), this+0x96 = guild id. Writes the lines
// one by one with WriteTextHD (g_text 0x006E4E68) starting at (x, y), 10 pixels apart.
void EQ_ItemData_guess::draw_guess(void)
{
    typedef void (__fastcall *Slot2_guess_t)(void*);
    int flags;
    int x;
    int a;
    int b;
    int c;
    int y;
    short n;
    short count;
    int i;
    u16 skill;
    char ch;
    char* name_ptr;
    char** spell;
    short* guild;
    char sizeName[8];
    char line[128];
    char tmp[128];
    char dir[20];
    char text2[128];
    char name[128];

    if (EQ_CDECL(int, 0x004504AC, (void))() == 0)
        return;
    if (STATE_guess == 3)
        ((Slot2_guess_t)(*(u32**)this)[2])(this);
    if (STATE_guess == 2) {
        int* r = (int*)(g_eqmain_guess + 0x90224);
        r[4] = 0;
        x = r[0] + 10;
        flags = 10;
        r = (int*)(g_eqmain_guess + 0x90224);
        r[4] = 0;
        y = r[1] + 5;
    } else {
        x = 0xa5;
        y = 0x154;
        flags = 0;
    }
    EQ_GLOBAL(u8, 0x006B6404) = 1;
    *(u32*)(g_chatWindow_guess + 0x1ec) = 1;
    n = *(short*)((char*)this + 0x94);
    if (n == -1) {
        guild = (short*)((char*)this + 0x96);
        if (pinstEQ_Main_guess->inspectActive_guess != '\0' && *guild >= 0) {
            if (STATE_guess == 1)
                DRAW_guess(this)( 1);
            STRCPY_guess(name, g_eqmain_guess + 0x1024 + *guild * 0x20);
            SPRINTF_guess(line, EQ_ARRAY(char, 0x0053E678), name);
            goto write_y;
        }
        if (*(int*)((char*)this + 0x90) == 0)
            return;
        if (STATE_guess == 1)
            DRAW_guess(this)( 1);
        STRCPY_guess(name, ITEM_guess);
        if (STRLEN_guess(name) > 0x2d)
            name[0x2d] = '\0';
        SPRINTF_guess(line, EQ_ARRAY(char, 0x0053E678), name);
        n = 1;
        WRITE_guess(line, x, y);
        STRCPY_guess(line, EMPTY_guess);
        if (*(char*)(ITEM_guess + 0x81) == '\0') {
            if (*(u8*)(ITEM_guess + 0xc3) & 1)
                APPEND_guess(line, EQ_ARRAY(char, 0x005583F8));
            FLAGS_guess
            if (line[0] != '\0') {
                WRITE_guess(line, x, y + 10);
                n = 2;
            }
            SPRINTF_guess(text2, EQ_ARRAY(char, 0x00559544));
            STRCPY_guess(line, EMPTY_guess);
            b = 0;
            do {
                if (*(u32*)(ITEM_guess + 0x88) & (1 << EQ_ARRAY(u8, 0x00557F2C)[(short)b * 14])) {
                    SPRINTF_guess(tmp, EQ_ARRAY(char, 0x0053E678), EQ_ARRAY(char, 0x00557F20) + (short)b * 14);
                    APPEND_guess(line, tmp);
                }
                if ((short)b == 3 || (short)b == 9 || (short)b == 15)
                    b++;
                b++;
            } while ((short)b < 0x16);
            if (line[0] != '\0') {
                APPEND_guess(text2, line);
                WRITE_guess(text2, x, y + n * 10);
                n++;
            }
            if (*(char*)(ITEM_guess + 0xc0) == 3 || *(char*)(ITEM_guess + 0xc2) == 0x15) {
                SPRINTF_guess(line, EQ_ARRAY(char, 0x00559518), *(u8*)(ITEM_guess + 0xda));
                WRITE_guess(line, x, y + n * 10);
                n++;
            } else if (*(u8*)(ITEM_guess + 0xd8) >= 2 && *(u8*)(ITEM_guess + 0xd8) <= 4 &&
                       *(char*)(ITEM_guess + 0xbb) > 0) {
                if (*(u8*)(ITEM_guess + 0xda) > 0)
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x00558568), *(u8*)(ITEM_guess + 0xda));
                else
                    STRCPY_guess(line, EQ_ARRAY(char, 0x00559534));
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            if (*(char*)(ITEM_guess + 0xc2) == 0x14) {
                a = g_pc_guess->class_;
                if (*(u32*)(ITEM_guess + 0xd0) & (int)(short)(1 << (a - 1)))
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x00559504),
                                  *(u8*)(EQ_ARRAY(char*, 0x005FE9A0)[*(short*)(ITEM_guess + 0xce)] + 0x234 + a));
                else
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x005594F0), EQ_ARRAY(char, 0x0055957C));
                WRITE_guess(line, x, y + n * 10);
                n++;
                SPRINTF_guess(line, EQ_ARRAY(char, 0x00541B64),
                              g_skill_records_guess
                                  [*(u8*)(EQ_ARRAY(char*, 0x005FE9A0)[*(short*)(ITEM_guess + 0xce)] + 0x230)]);
                WRITE_guess(line, x, y + n * 10);
                n++;
                SPRINTF_guess(line, EQ_ARRAY(char, 0x00559560),
                              (int)*(short*)(EQ_ARRAY(char*, 0x005FE9A0)[*(short*)(ITEM_guess + 0xce)] + 0x1c8));
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            if (EQ_METHOD(0, int, 0x0043624B, (void*), ITEM_guess)() != 0 ||
                *(char*)(ITEM_guess + 0xc2) == 0x1b || *(char*)(ITEM_guess + 0xc2) == 0x1c) {
                skill = EQ_METHOD(1, u8, 0x004B422A, (void*, int), EQ_GLOBAL(void*, 0x006E4D8C))(*(u8*)(ITEM_guess + 0xc2));
                ch = *(char*)(ITEM_guess + 0xc2);
                if (ch == 0x1b)
                    skill = 7;
                if (ch == 0x1c)
                    skill = 7;
                if (ch == 0x13)
                    skill = 0x33;
                if (skill != 0x1c) {
                    if (ch == 0x23)
                        SPRINTF_guess(line, EQ_ARRAY(char, 0x00558540), *(u8*)(ITEM_guess + 0xbe));
                    else
                        SPRINTF_guess(line, EQ_ARRAY(char, 0x00558524),
                                      g_skill_records_guess[(short)skill], *(u8*)(ITEM_guess + 0xbe));
                    WRITE_guess(line, x, y + n * 10);
                    n++;
                }
            }
            if (*(char*)(ITEM_guess + 0xc2) == 0x28) {
                a = (int)g_local_player_guess->heading;
                STRCPY_guess(dir, EMPTY_guess);
                if ((short)a < 0x1e0) {
                    if ((short)a > 0x1a0)
                        name_ptr = EQ_ARRAY(char, 0x00540F1C);
                    else if ((short)a > 0x160)
                        name_ptr = EQ_ARRAY(char, 0x00540F14);
                    else if ((short)a > 0x120)
                        name_ptr = EQ_ARRAY(char, 0x00540F08);
                    else if ((short)a > 0xe0)
                        name_ptr = EQ_ARRAY(char, 0x00540F00);
                    else if ((short)a > 0xa0)
                        name_ptr = EQ_ARRAY(char, 0x00540EF4);
                    else if ((short)a > 0x60)
                        name_ptr = EQ_ARRAY(char, 0x00540EEC);
                    else if ((short)a > 0x20)
                        name_ptr = EQ_ARRAY(char, 0x00540EE0);
                    else if ((short)a >= 0)
                        name_ptr = EQ_ARRAY(char, 0x00540ED8);
                    else
                        name_ptr = 0;
                } else {
                    name_ptr = EQ_ARRAY(char, 0x00540ED8);
                }
                if (name_ptr != 0)
                    STRCPY_guess(dir, name_ptr);
                SPRINTF_guess(line, EQ_ARRAY(char, 0x005594D8), dir);
                WRITE_guess(line, x, y + n * 10);
                n++;
            } else if (EQ_METHOD(0, int, 0x00436285, (void*), ITEM_guess)() != 0) {
                STRCPY_guess(line, EQ_METHOD(1, char*, 0x004C8F02, (void*, short), EQ_GLOBAL(void*, 0x006EFE2C))(*(u8*)(ITEM_guess + 0xc2)));
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            if (*(u8*)(ITEM_guess + 0xbf) > 0) {
                if (*(char*)(ITEM_guess + 0xba) != '\0')
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x00541B50), *(u8*)(ITEM_guess + 0xbf),
                                  (int)*(char*)(ITEM_guess + 0xba));
                else
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x00541B48), *(u8*)(ITEM_guess + 0xbf));
                WRITE_guess(line, x, y + n * 10);
                n++;
            } else if (*(char*)(ITEM_guess + 0xba) != '\0') {
                SPRINTF_guess(line, EQ_ARRAY(char, 0x00541B40), (int)*(char*)(ITEM_guess + 0xba));
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            if (*(u8*)(ITEM_guess + 0x10b) != 0 && *(u8*)(ITEM_guess + 0x10c) != 0) {
                switch (*(u8*)(ITEM_guess + 0x10b)) {
                case 1:
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x00559488), *(u8*)(ITEM_guess + 0x10c));
                    break;
                case 2:
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x00559498), *(u8*)(ITEM_guess + 0x10c));
                    break;
                case 3:
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x005594A8), *(u8*)(ITEM_guess + 0x10c));
                    break;
                case 4:
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x005594B8), *(u8*)(ITEM_guess + 0x10c));
                    break;
                case 5:
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x005594C8), *(u8*)(ITEM_guess + 0x10c));
                    break;
                }
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            if ((*(short*)(ITEM_guess + 0x102) != 0 || *(short*)(ITEM_guess + 0x100) != 0) &&
                *(u8*)(ITEM_guess + 0x104) > 0) {
                STRCPY_guess(line, EMPTY_guess);
                if (*(short*)(ITEM_guess + 0x100) != 0 && *(short*)(ITEM_guess + 0x102) == 0) {
                    SPRINTF_guess(line, EQ_ARRAY(char, 0x00559478),
                                  EQ_METHOD(1, char*, 0x004C9105, (void*, short), EQ_GLOBAL(void*, 0x006EFE2C))(*(short*)(ITEM_guess + 0x100)),
                                  *(u8*)(ITEM_guess + 0x104));
                } else if (*(short*)(ITEM_guess + 0x102) != 0) {
                    if (*(short*)(ITEM_guess + 0x100) == 0) {
                        SPRINTF_guess(line, EQ_ARRAY(char, 0x00559464),
                                      EQ_METHOD(1, char*, 0x004C8F39, (void*, short), EQ_GLOBAL(void*, 0x006EFE2C))(*(short*)(ITEM_guess + 0x102)),
                                      *(u8*)(ITEM_guess + 0x104));
                    } else if (*(short*)(ITEM_guess + 0x102) != 0 && *(short*)(ITEM_guess + 0x100) != 0) {
                        SPRINTF_guess(line, EQ_ARRAY(char, 0x00559450),
                                      EQ_METHOD(1, char*, 0x004C8F39, (void*, short), EQ_GLOBAL(void*, 0x006EFE2C))(*(short*)(ITEM_guess + 0x102)),
                                      EQ_METHOD(1, char*, 0x004C9105, (void*, short), EQ_GLOBAL(void*, 0x006EFE2C))(*(short*)(ITEM_guess + 0x100)),
                                      *(u8*)(ITEM_guess + 0x104));
                    }
                }
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            i = 0;
            {
                short* faction = (short*)(ITEM_guess + 0x116);
                int k = 4;
                do {
                    if (faction[-4] != 0 && *faction > 0)
                        i++;
                    faction++;
                    k--;
                } while (k != 0);
            }
            if (i != 0) {
                SPRINTF_guess(line, EQ_ARRAY(char, 0x0055942C));
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            count = 0;
            STRCPY_guess(line, EMPTY_guess);
            STAT_guess(0xac, EQ_ARRAY(char, 0x00541B24), EQ_ARRAY(char, 0x00541B30))
            STAT_guess(0xaf, EQ_ARRAY(char, 0x00541B08), EQ_ARRAY(char, 0x00541B14))
            STAT_guess(0xad, EQ_ARRAY(char, 0x00541AEC), EQ_ARRAY(char, 0x00541AF8))
            FLUSH_guess
            STAT_guess(0xae, EQ_ARRAY(char, 0x00541AD0), EQ_ARRAY(char, 0x00541ADC))
            FLUSH_guess
            STAT_guess(0xb2, EQ_ARRAY(char, 0x00541AB4), EQ_ARRAY(char, 0x00541AC0))
            FLUSH_guess
            STAT_guess(0xb0, EQ_ARRAY(char, 0x00541A98), EQ_ARRAY(char, 0x00541AA4))
            FLUSH_guess
            STAT_guess(0xb1, EQ_ARRAY(char, 0x00541A7C), EQ_ARRAY(char, 0x00541A88))
            FLUSH_guess
            STAT_guess(0xb8, EQ_ARRAY(char, 0x00541A64), EQ_ARRAY(char, 0x00541A70))
            FLUSH_guess
            STAT_guess(0xb9, EQ_ARRAY(char, 0x00541A44), EQ_ARRAY(char, 0x00541A54))
            FLUSH_guess
            STAT_guess(0xb4, EQ_ARRAY(char, 0x00541A20), EQ_ARRAY(char, 0x00541A30))
            FLUSH_guess
            STAT_guess(0xb6, EQ_ARRAY(char, 0x005419F8), EQ_ARRAY(char, 0x00541A0C))
            FLUSH_guess
            STAT_guess(0xb5, EQ_ARRAY(char, 0x005419D4), EQ_ARRAY(char, 0x005419E4))
            FLUSH_guess
            STAT_guess(0xb3, EQ_ARRAY(char, 0x005419AC), EQ_ARRAY(char, 0x005419C0))
            FLUSH_guess
            ch = *(char*)(ITEM_guess + 0xb7);
            if (ch != '\0') {
                SPRINTF_guess(tmp, ch > 0 ? EQ_ARRAY(char, 0x00541998) : EQ_ARRAY(char, 0x00541984), (int)ch);
                STRCAT_guess(line, tmp);
            }
            if (STRLEN_guess(line) != 0) {
                WRITE_guess(line, x, y + n * 10);
                n++;
                STRCPY_guess(line, EMPTY_guess);
            }
            if (*(char*)(ITEM_guess + 0xc2) != 0x14 && *(u8*)(ITEM_guess + 0xd8) >= 2 &&
                *(u8*)(ITEM_guess + 0xd8) <= 4 && *(short*)(ITEM_guess + 0xdc) >= 0 &&
                *(short*)(ITEM_guess + 0xdc) < 3000) {
                SPRINTF_guess(line, EQ_ARRAY(char, 0x00559420),
                              EQ_ARRAY(char*, 0x005FE9A0)[*(short*)(ITEM_guess + 0xdc)]);
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            SIZENAME_guess(0x80)
            ch = *(char*)(ITEM_guess + 0xc2);
            if (ch == 0x0e || ch == 0x0f) {
                a = *(int*)(ITEM_guess + 0xe8);
                if (ch == 0x0e) {
                    if (a < 6)
                        name_ptr = EQ_ARRAY(char, 0x0055940C);
                    else if (a < 0x15)
                        name_ptr = EQ_ARRAY(char, 0x005593FC);
                    else if (a < 0x1f)
                        name_ptr = EQ_ARRAY(char, 0x005593E4);
                    else if (a < 0x29)
                        name_ptr = EQ_ARRAY(char, 0x005593C4);
                    else if (a < 0x33)
                        name_ptr = EQ_ARRAY(char, 0x005593AC);
                    else if (a < 0x3d)
                        name_ptr = EQ_ARRAY(char, 0x00559390);
                    else
                        name_ptr = EQ_ARRAY(char, 0x00559374);
                    SPRINTF_guess(line, name_ptr);
                } else if (ch == 0x0f) {
                    if (a < 6)
                        name_ptr = EQ_ARRAY(char, 0x00559358);
                    else if (a < 0x15)
                        name_ptr = EQ_ARRAY(char, 0x00559344);
                    else if (a < 0x1f)
                        name_ptr = EQ_ARRAY(char, 0x00559328);
                    else if (a < 0x29)
                        name_ptr = EQ_ARRAY(char, 0x0055930C);
                    else if (a < 0x33)
                        name_ptr = EQ_ARRAY(char, 0x005592F0);
                    else if (a < 0x3d)
                        name_ptr = EQ_ARRAY(char, 0x005592D4);
                    else
                        name_ptr = EQ_ARRAY(char, 0x005592B8);
                    SPRINTF_guess(line, name_ptr);
                }
                WRITE_guess(line, x, y + n * 10);
                n++;
            }
            if (*(u8*)(ITEM_guess + 0xc1) != 0) {
                a = *(u8*)(ITEM_guess + 0x7d);
                SPRINTF_guess(line, EQ_ARRAY(char, 0x00559294), (double)((float)a * 0.1f),
                              *(u8*)(ITEM_guess + 0xc1), sizeName);
            } else {
                a = *(u8*)(ITEM_guess + 0x7d);
                SPRINTF_guess(line, EQ_ARRAY(char, 0x0055927C), (double)((float)a * 0.1f), sizeName);
            }
            WRITE_guess(line, x, y + n * 10);
            n++;
            STRCPY_guess(line, EQ_ARRAY(char, 0x0055851C));
            i = 0;
            a = 0;
            c = 0;
            b = 0;
            do {
                if (*(u32*)(ITEM_guess + 0xd0) & (int)(short)(1 << i)) {
                    switch (i) {
                    case 0:
                        name_ptr = EQ_ARRAY(char, 0x00558514);
                        break;
                    case 1:
                        name_ptr = EQ_ARRAY(char, 0x0055850C);
                        break;
                    case 2:
                        name_ptr = EQ_ARRAY(char, 0x00558504);
                        break;
                    case 3:
                        name_ptr = EQ_ARRAY(char, 0x005584F4);
                        break;
                    case 4:
                        name_ptr = EQ_ARRAY(char, 0x005584FC);
                        break;
                    case 5:
                        name_ptr = EQ_ARRAY(char, 0x005584EC);
                        break;
                    case 6:
                        name_ptr = EQ_ARRAY(char, 0x005584E4);
                        break;
                    case 7:
                        name_ptr = EQ_ARRAY(char, 0x005584DC);
                        break;
                    case 8:
                        name_ptr = EQ_ARRAY(char, 0x005584D4);
                        break;
                    case 9:
                        name_ptr = EQ_ARRAY(char, 0x005584CC);
                        break;
                    case 10:
                        name_ptr = EQ_ARRAY(char, 0x005584C4);
                        break;
                    case 11:
                        name_ptr = EQ_ARRAY(char, 0x005584BC);
                        break;
                    case 12:
                        name_ptr = EQ_ARRAY(char, 0x005584B4);
                        break;
                    case 13:
                        name_ptr = EQ_ARRAY(char, 0x005584AC);
                        break;
                    case 14:
                        name_ptr = EQ_ARRAY(char, 0x005584A4);
                        break;
                    default:
                        goto class_default;
                    }
                    SPRINTF_guess(tmp, name_ptr);
                class_default:
                    a++;
                    c++;
                    STRCAT_guess(line, tmp);
                    if ((short)a == 6 && b == 0) {
                        STRCPY_guess(text2, line);
                        STRCPY_guess(line, EQ_ARRAY(char, 0x00559270));
                        a = 0;
                        b = 1;
                    }
                }
                i++;
            } while (i + 1 <= 15);
            if ((short)a == 0 && b == 0) {
                WRITE_guess(EQ_ARRAY(char, 0x00558494), x, y + n * 10);
                n++;
            }
            if ((short)c < 0xf) {
                if (b != 0) {
                    WRITE_guess(text2, x, y + n * 10);
                    n++;
                }
            class_tail:
                if ((short)a != 0) {
                    WRITE_guess(line, x, y + n * 10);
                    n++;
                }
            } else {
                if (b == 0)
                    goto class_tail;
                WRITE_guess(EQ_ARRAY(char, 0x00558488), x, y + n * 10);
                n++;
            }
            b = 0;
            STRCPY_guess(line, EQ_ARRAY(char, 0x00558480));
            c = 0;
            a = 0;
            i = 0;
            do {
                if (*(u32*)(ITEM_guess + 0xd4) & (int)(short)(1 << i)) {
                    switch (i) {
                    case 0:
                        name_ptr = EQ_ARRAY(char, 0x00558478);
                        break;
                    case 1:
                        name_ptr = EQ_ARRAY(char, 0x00558470);
                        break;
                    case 2:
                        name_ptr = EQ_ARRAY(char, 0x00558468);
                        break;
                    case 3:
                        name_ptr = EQ_ARRAY(char, 0x00558460);
                        break;
                    case 4:
                        name_ptr = EQ_ARRAY(char, 0x00558458);
                        break;
                    case 5:
                        name_ptr = EQ_ARRAY(char, 0x00558450);
                        break;
                    case 6:
                        name_ptr = EQ_ARRAY(char, 0x00558448);
                        break;
                    case 7:
                        name_ptr = EQ_ARRAY(char, 0x00558440);
                        break;
                    case 8:
                        name_ptr = EQ_ARRAY(char, 0x00558438);
                        break;
                    case 9:
                        name_ptr = EQ_ARRAY(char, 0x00558430);
                        break;
                    case 10:
                        name_ptr = EQ_ARRAY(char, 0x00558428);
                        break;
                    case 11:
                        name_ptr = EQ_ARRAY(char, 0x00558420);
                        break;
                    default:
                        goto race_default;
                    }
                    SPRINTF_guess(tmp, name_ptr);
                race_default:
                    c++;
                    a++;
                    STRCAT_guess(line, tmp);
                    if ((short)c == 6 && b == 0) {
                        STRCPY_guess(text2, line);
                        STRCPY_guess(line, EQ_ARRAY(char, 0x00559270));
                        c = 0;
                        b = 1;
                    }
                }
                i++;
            } while (i + 1 <= 12);
            if (*(u8*)(ITEM_guess + 0xd5) & 0x10) {
                STRCAT_guess(line, EQ_ARRAY(char, 0x0055841C));
                c++;
                a++;
            }
            if (*(u8*)(ITEM_guess + 0xd5) & 0x20) {
                STRCAT_guess(line, EQ_ARRAY(char, 0x00559268));
                c++;
                a++;
            }
            if ((short)a == 0) {
                WRITE_guess(EQ_ARRAY(char, 0x00558410), x, y + n * 10);
                n++;
            }
            if ((short)a < 0xe) {
                if (b != 0) {
                    WRITE_guess(text2, x, y + n * 10);
                    n++;
                }
            race_tail:
                if ((short)c != 0) {
                    WRITE_guess(line, x, y + n * 10);
                    n++;
                }
            } else {
                if (b == 0)
                    goto race_tail;
                WRITE_guess(EQ_ARRAY(char, 0x00558404), x, y + n * 10);
                n++;
            }
            if (*(int*)(ITEM_guess + 0x120) == 0)
                return;
            a = 0;
            STRCPY_guess(line, EQ_ARRAY(char, 0x00559260));
            i = 2;
            do {
                if (*(u32*)(ITEM_guess + 0x120) & (1 << (i - 1))) {
                    APPEND_guess(line, EQ_METHOD(1, char*, 0x004C9C6D, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(i + 199));
                    a++;
                }
                if (a == 5) {
                    WRITE_guess(line, x, y + n * 10);
                    n++;
                    STRCPY_guess(line, EMPTY_guess);
                    a = 0;
                }
                i++;
            } while (i < 0x12);
            if (line[0] == '\0')
                return;
        } else if (*(char*)(ITEM_guess + 0x81) == 1) {
            WRITE_guess(EQ_METHOD(0, char, 0x00434096, (void*), ITEM_guess)() != '\0' ? EQ_ARRAY(char, 0x005585B0)
                                                                                  : EQ_ARRAY(char, 0x0055859C),
                        x, y + 10);
            n = 2;
            STRCPY_guess(line, EMPTY_guess);
            FLAGS_guess
            if (line[0] != '\0') {
                WRITE_guess(line, x, y + 0x14);
                n = 3;
            }
            SIZENAME_guess(0xd7)
            a = *(u8*)(ITEM_guess + 0x7d);
            SPRINTF_guess(line, EQ_ARRAY(char, 0x0055923C), (double)((float)a * 0.1f), *(u8*)(ITEM_guess + 0xd8));
            WRITE_guess(line, x, y + n * 10);
            SPRINTF_guess(line, EQ_ARRAY(char, 0x00559218), *(u8*)(ITEM_guess + 0xd5), sizeName);
            n++;
        } else {
            if (*(char*)(ITEM_guess + 0x81) != 2)
                return;
            if (*(char*)(ITEM_guess + 0xac) != '\0') {
                STRCPY_guess(line, EQ_ARRAY(char, 0x005585FC));
                STRCAT_guess(line, *(char*)(ITEM_guess + 0xad) != '\0' ? EQ_ARRAY(char, 0x005585F4)
                                                                       : EQ_ARRAY(char, 0x005585EC));
            } else {
                STRCPY_guess(line, EQ_ARRAY(char, 0x005585DC));
                STRCPY_guess(line, *(char*)(ITEM_guess + 0xad) != '\0' ? EQ_ARRAY(char, 0x005585D0)
                                                                       : EQ_ARRAY(char, 0x005585C4));
            }
            WRITE_guess(line, x, y + 10);
            n = 2;
            STRCPY_guess(line, EMPTY_guess);
            FLAGS_guess
            if (line[0] != '\0') {
                WRITE_guess(line, x, y + 0x14);
                n = 3;
            }
            SIZENAME_guess(0x80)
            a = *(u8*)(ITEM_guess + 0x7d);
            SPRINTF_guess(line, EQ_ARRAY(char, 0x0055927C), (double)((float)a * 0.1f), sizeName);
        }
        y = y + n * 10;
        goto write_y;
    }
    if (n >= 10000)
        n -= 10000;
    if (STATE_guess == 1)
        DRAW_guess(this)( 1);
    spell = EQ_ARRAY(char*, 0x005FE9A0) + n;
    SPRINTF_guess(line, EQ_ARRAY(char, 0x00559588), *spell);
    WRITE_guess(line, x, y);
    if (*(short*)((char*)this + 0x94) >= 10000)
        return;
    a = *(u8*)(*spell + 0x234 + g_pc_guess->class_);
    if (a > 0x3c) {
        SPRINTF_guess(line, EQ_ARRAY(char, 0x0053E678), EQ_ARRAY(char, 0x0055957C));
        y += 0x14;
        goto write_y;
    }
    SPRINTF_guess(line, EQ_ARRAY(char, 0x00559570), a);
    WRITE_guess(line, x, y + 0x14);
    switch (*(u8*)(*spell + 0x230)) {
    case 4:
    case 5:
    case 0xd:
    case 0xe:
    case 0x12:
    case 0x18:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
    case 0x2f:
        SPRINTF_guess(line, EQ_ARRAY(char, 0x00541B64), g_skill_records_guess[*(u8*)(*spell + 0x230)]);
        WRITE_guess(line, x, y + 0x1e);
        SPRINTF_guess(line, EQ_ARRAY(char, 0x00559560), (int)*(short*)(*spell + 0x1c8));
        WRITE_guess(line, x, y + 0x28);
        a = 5;
        break;
    default:
        SPRINTF_guess(line, EQ_ARRAY(char, 0x00541B64), g_skill_records_guess[*(u8*)(*spell + 0x230)]);
        WRITE_guess(line, x, y + 0x1e);
        a = 4;
        break;
    }
    if (g_pc_guess->class_ == 8)
        return;
    SPRINTF_guess(line, EQ_ARRAY(char, 0x0055954C), (double)((float)*(u32*)(*spell + 0x1b0) * 0.001f));
    y = y + (short)a * 10;
write_y:
    WRITE_guess(line, x, y);
}

#undef STRCPY_guess
#undef STRCAT_guess
#undef STRLEN_guess
#undef SPRINTF_guess
#undef APPEND_guess
#undef WRITE_guess
#undef ITEM_guess
#undef STATE_guess
#undef DRAW_guess
#undef EMPTY_guess
#undef STAT_guess
#undef FLUSH_guess
#undef FLAGS_guess
#undef SIZENAME_guess
