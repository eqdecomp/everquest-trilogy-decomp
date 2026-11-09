// EQ_CharCreateSelS.cpp - character creation and selection predicates.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_CharCreateSelS.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_actor_lists.h"
#include "trilogy/player.h"
#include "trilogy/display.h"
#include "trilogy/layout/eq_charcreate.h"

typedef int (__stdcall *EQ_SetRectFn_guess)(void *, int, int, int, int);

// Owns the EQ_Overlay base subobject while the character select constructor runs: the base
// constructor runs in the guard's constructor, and the base destructor runs on unwind until disarm().
class EQ_CharSelS_ctor_guard_guess
{
public:
    EQ_CharSelS_ctor_guard_guess(char* object) : self(object)
    {
        EQ_METHOD(0, void, 0x00450049, (char*), self)();
    }

    ~EQ_CharSelS_ctor_guard_guess()
    {
        if (self)
            EQ_METHOD(0, void, 0x0045005D, (char*), self)();
    }

    void disarm()
    {
        self = 0;
    }

private:
    char* self;
};

// FUNCTION: TRILOGY 0x00429534
// Reads the race value at +0x1bd2 and compares it as an unsigned 16-bit value.
int EQ_CharCreateS::IsEvilRace_guess(void)
{
    u16 raw_race = this->race;
    int result = 0;
    int race = (int)raw_race + 1;
    if (race == 10 || race == 9 || race == 6 || race == 0x80)
        result = 1;
    return result;
}

// FUNCTION: TRILOGY 0x00429559
// Tests whether the character creation class ID is 5 or 11.
int EQ_CharCreateS::IsClassNecromancerOrShadowKnight_guess(void)
{
    u8 raw_class = this->class_;
    int result = 0;
    int class_id = (int)raw_class + 1;
    if (class_id == 11 || class_id == 5)
        result = 1;
    return result;
}

// FUNCTION: TRILOGY 0x0042c3f3
// Character creation mouse move: in mode 2, slides a 10-px knob to the mouse x, clamped to the selected entry's limits
// ORIGINAL FILE: EverQuest/EQ_CharCreateSelS.cpp (from Mac debug map)
/* fake match: named xRight takes ebx, which puts the SetRect pointer in ebp as in the original; an inline xLeft + 10 CSEs into ebp instead */
void EQ_CharCreateS::MouseMove_guess(short param_2, short unused)
{
    short limit;
    int xLeft;
    int xRight;
    EQ_SetRectFn_guess setRect;

    if ((this->alignmentDragging_guess != 0) &&
        (this->mode == 2) &&
        (this->deity >= 0))
    {
        param_2 = param_2 - 7;
        if (0x1ea < param_2)
            param_2 = 0x1ea;
        if (param_2 < 0x174)
            param_2 = 0x174;

        limit = EQ_METHOD(1, s32, 0x0041CB50, (void *, u8), *(void **)((char *)this + 0x660))(*(u8 *)((char *)this + 0x1baa));
        if (param_2 > limit)
            param_2 = EQ_METHOD(1, s32, 0x0041CB50, (void *, u8), *(void **)((char *)this + 0x660))(*(u8 *)((char *)this + 0x1baa));

        limit = EQ_METHOD(1, s32, 0x0041CB41, (void *, u8), *(void **)((char *)this + 0x660))(*(u8 *)((char *)this + 0x1baa));
        if (param_2 < limit)
            param_2 = EQ_METHOD(1, s32, 0x0041CB41, (void *, u8), *(void **)((char *)this + 0x660))(*(u8 *)((char *)this + 0x1baa));

        xLeft = param_2;
        xRight = xLeft + 10;
        setRect = EQ_GLOBAL(EQ_SetRectFn_guess, 0x00534248);
        setRect((char *)this + 0x4b0, xLeft, 0x185, xRight, 399);
        setRect(*(void **)((char *)this + 0x1bcc), xLeft, 0x185, xRight, 399);
    }
    return;
}

// FUNCTION: TRILOGY 0x0042c4d3
// Formats character creation welcome text into display lines.
// ORIGINAL FILE: EverQuest/EQ_CharCreateSelS.cpp (from Mac debug map)
// @early-stop: the scan byte load is [eax + edi], the original's [edi + eax]; every operand form tried gave [eax + edi]
void EQ_CharCreateS::transformText_guess(char *inputText)
{
    char segmentText[1024];
    char *destination;
    int index;
    int found_space;
    unsigned int length;
    int list_index;

    *(char *)((char *)this + 0x668) = 0;
    this->welcomeWorkText_guess[0] = 0;
    this->welcomeTailText_guess[0] = 0;
    destination = (char *)this + 0x868;
    length = EQ_CDECL(u32, 0x005223D0, (const char *))(inputText);
    index = 0;
    if ((int)length > 0)
    {
        while (index < (int)length)
        {
            if (inputText[index] == '@')
            {
                EQ_CDECL(char *, 0x005231D0, (char *, const char *, u32))
                    (destination, inputText, (u32)index);
                *((char *)this + index + 0x868) = 0;
                EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                    (inputText, inputText + index + 1);
                break;
            }
            index = index + 1;
        }
    }

    EQ_CDECL(char *, 0x005219C0, (char *, const char *))
        ((char *)this + 0x1268, inputText);
    this->welcomeLineCount_guess = 0;
    length = EQ_CDECL(u32, 0x005223D0, (const char *))(destination);
    if ((int)length < 0x5a)
    {
        list_index = this->welcomeLineCount_guess;
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))
            ((char *)this + list_index * 0x6e + 0x1468, destination);
        this->welcomeLineCount_guess =
            this->welcomeLineCount_guess + 1;
    }
    else
    {
        length = EQ_CDECL(u32, 0x005223D0, (const char *))(destination);
        if (0x5a < length)
        {
            /* fake match: the original loads inputText into the split register before the loop and overwrites it
               before any read; VC6 only emits that load when the copy is kept */
            char *cut = inputText;

            do
            {
                int split;

                found_space = 0;
                split = 0x5a;
                do
                {
                    if ((!found_space) && (destination[split] == ' '))
                    {
                        found_space = 1;
                        cut = (char *)split;
                    }
                    split = split - 1;
                } while (0x4e < split);
                if (!found_space)
                {
                    cut = (char *)0x5a;
                }

                EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                    (segmentText, (const char *)0x0058fba4);
                EQ_CDECL(char *, 0x005231D0, (char *, const char *, u32))
                    (segmentText, destination, (u32)cut);
                segmentText[(int)cut] = '\0';
                EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                    (destination, (char *)this + (int)cut + 0x869);
                list_index = this->welcomeLineCount_guess;
                EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                    ((char *)this + list_index * 0x6e + 0x1468, segmentText);
                *(int *)((char *)this + 0x1990) =
                    this->welcomeLineCount_guess + 1;
                length = EQ_CDECL(u32, 0x005223D0, (const char *))(destination);
            } while (0x5a < length);
        }

        length = EQ_CDECL(u32, 0x005223D0, (const char *))(destination);
        if (length > 0)
        {
            list_index = this->welcomeLineCount_guess;
            EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                ((char *)this + list_index * 0x6e + 0x1468, destination);
            this->welcomeLineCount_guess =
                this->welcomeLineCount_guess + 1;
        }
    }
    return;
}

// ---------------------------------------------------------------------------------------------------------
// 0x0042c654 (character-creation help/description panel): callee, global and field macros. Callees are reached
// by address only; strings are the .rdata literals at the addresses the listing pushes.
//   - text:    CCH_Text(str, x, y, font, color) = WriteTextHD on the text object (the last arg is always 1).
//              Strings are CCH_S(0x00542c2c) like the neighbouring rewrites ((const char *)0x0058fba4).
//   - colors:  CCH_Color04 / CCH_Color08 are the ints at 0x006efe04 / 0x006efe08 (5th WriteTextHD arg).
//   - images:  CCH_ImageDraw(EQ_GLOBAL(void *, va), CCH_AT(off)) = EQ_Image::draw(image object read from the
//              global at va, position struct at this+off, 1).
//   - overlay: CCH_OvlHandle(CCH_OvlXX) = *(CCH_OvlXX + 4), the first arg of DrawOverlay / drawPartial.
//   - fields of `this` (the EQ_CharCreateS screen): CCH_MODE (+0x1BA8 page), CCH_DEITY (+0x1BAA selected
//     deity), CCH_HOVER (+0x1BB2 id under the cursor, -1 = none), CCH_CITY (+0x1BD0), CCH_RACE (+0x1BD2 u16),
//     CCH_CLASS (+0x1BD4 u8), CCH_CHAR (+0x660 the EQ_Character), CCH_AT(off) = (char *)this + off.
//   - locals: buf (2048 B text buffer), rc (the RECT, as int[4]), fontsz (4th WriteTextHD arg of the city
//     page: 10, or 13 for "You may not begin here.").
// ---------------------------------------------------------------------------------------------------------
#define CCH_S(va)                   ((const char *)(va))
#define CCH_AT(off)                 ((char *)this + (off))
#define CCH_MODE                    (this->mode)
#define CCH_DEITY                   (this->deity)
#define CCH_HOVER                   (this->hoverButton)
#define CCH_CITY                    (this->city)
#define CCH_RACE                    (*(u16 *)((char *)this + 0x1BD2))
#define CCH_RACE_BYTE               (*(u8 *)((char *)this + 0x1BD2))
#define CCH_CLASS                   (*(u8 *)((char *)this + 0x1BD4))
#define CCH_CHAR                    (*(void **)((char *)this + 0x660))
#define g_display_guess                 EQ_GLOBAL(void *, 0x006E4DE0)
#define CCH_DispFlag94()            (g_cdisplay_guess->fastFileHandle6_guess != 0)
#define g_eqtexto_guess                 EQ_GLOBAL(void *, 0x006E4E68)
#define CCH_Screen                  EQ_GLOBAL(void *, 0x006E4E70)
#define CCH_Ovl74                   EQ_GLOBAL(void *, 0x006E4E74)
#define CCH_Ovl78                   EQ_GLOBAL(void *, 0x006E4E78)
#define CCH_Ovl7C                   EQ_GLOBAL(void *, 0x006E4E7C)
#define CCH_Ovl80                   EQ_GLOBAL(void *, 0x006E4E80)
#define CCH_Ovl84                   EQ_GLOBAL(void *, 0x006E4E84)
#define CCH_Ovl88                   EQ_GLOBAL(void *, 0x006E4E88)
#define CCH_OvlHandle(p)            (*(void **)((char *)(p) + 4))
#define CCH_Color04                 EQ_GLOBAL(int, 0x006EFE04)
#define CCH_Color08                 EQ_GLOBAL(int, 0x006EFE08)
#define g_everquest_guess               EQ_GLOBAL(void *, 0x006EFE2C)
#define CCH_YonMode0                EQ_GLOBAL(float, 0x005351A4)
#define CCH_YonOther                EQ_GLOBAL(float, 0x00535190)
#define CCH_FmtD                    CCH_S(0x0053E16C)
#define CCH_SetRect(rc, l, t, r, b) EQ_GLOBAL(EQ_SetRectFn_guess, 0x00534248)((rc), (l), (t), (r), (b))
#define CCH_IsActive()              EQ_CDECL(int, 0x004504AC, (void))()
#define CCH_SetYon(disp, yon)       EQ_METHOD(1, void, 0x0040E49F, (void *, float), (disp))((yon))
#define CCH_OverlayDraw(self, flag) EQ_METHOD(1, void, 0x00450081, (void *, int), (self))((flag))
#define CCH_DrawPartial(self, ov, rc, pos) \
    EQ_METHOD(4, void, 0x00450125, (void *, void *, void *, void *, int), (self))((ov), (rc), (pos), 1)
#define CCH_DrawOverlay(ov, rc1, rc2) \
    EQ_METHOD(4, void, 0x0040B492, (void *, void *, void *, void *, int), g_display_guess)((ov), (rc1), (rc2), 1)
#define CCH_ImageDraw(img, pos)     EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), (img))((pos), 1)
#define CCH_Text(str, x, y, font, color) \
    EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), g_eqtexto_guess)((str), (x), (y), (font), (color), 1)
#define CCH_strlen(s)               EQ_CDECL(u32, 0x005223D0, (const char *))(s)
#define CCH_strcpy(d, s)            EQ_CDECL(char *, 0x005219C0, (char *, const char *))((d), (s))
#define CCH_strcat(d, s)            EQ_CDECL(char *, 0x005219D0, (char *, const char *))((d), (s))
#define SOC_FMT                 EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
#define CCH_CheckClass(ch, race, cls) \
    EQ_METHOD(2, u8, 0x0041C663, (void *, short, int), (ch))((race), (cls))
#define CCH_CreationTableValue(ch, cls, col) \
    EQ_METHOD(2, short, 0x0041C648, (void *, short, int), (ch))((cls), (col))
#define CCH_CityCanStart(ch, city, race, deity, cls) \
    EQ_METHOD(4, u8, 0x0041C957, (void *, short, short, short, short), (ch))((city), (race), (deity), (cls))
#define CCH_GetRaceName(id)         EQ_METHOD(1, const char *, 0x004C9105, (void *, int), g_everquest_guess)((id))
#define CCH_GetClassDesc(id)        EQ_METHOD(1, const char *, 0x004C9D4E, (void *, int), g_everquest_guess)((id))
#define CCH_IsEvilRace(self)        EQ_METHOD(0, int, 0x00429534, (void *), (self))()
#define CCH_IsClassNecro(self)      EQ_METHOD(0, int, 0x00429559, (void *), (self))()
/* XCanWorship(character, class, deity index): one per race; the deity button loop picks by race id + 1 */
#define CCH_HumanCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C6BB, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_BarbarianCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C6F7, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_EruditeCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C739, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_ElfCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C779, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_HighElfCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C7AB, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_DarkElfCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C7D7, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_HalfElfCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C803, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_DwarfCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C83E, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_TrollCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C86A, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_OgreCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C896, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_HalflingCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C8C2, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_GnomeCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C8F4, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_IksarCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C920, (void *, u8, u8), (ch))((cls), (dty))
#define CCH_VahShirCanWorship(ch, cls, dty) EQ_METHOD(2, u8, 0x0041C94C, (void *, u8, u8), (ch))((cls), (dty))

// FUNCTION: TRILOGY 0x0042c654
// Draws the active character-creation step: race/class choice, deity lore, start city, or confirmation
// Draws the right-hand text panel of the character-creation screen. CCH_MODE is the page: 0 race/class
// (hover id picks one of 50 help images, then the stat table), 1 deity (17 deity paragraphs + the deity
// buttons), 2 starting city (14 city names), 3 confirm (name, RACE/CLASS, the "many enemies" notice).
void EQ_CharCreateS::DrawHelpPanel_guess(void)
{
    char buf[2048];     // EBP-0x81C
    int rc[4];          // EBP-0x1C  (RECT)
    int fontsz;         // 4th WriteTextHD arg on the city page: 10, or 13 for "You may not begin here."

    // REGION R01 0x0042C654-0x0042CDD5: T1 prologue, EQ_Overlay::draw, 50-way race/class help (JT 42C6CF) + two small switches (JT 42
    if (!CCH_IsActive())
        return;
    CCH_SetYon(g_display_guess, CCH_MODE == 0 ? CCH_YonMode0 : CCH_YonOther);
    if (CCH_MODE == 0)
    {
        int i;

        CCH_OverlayDraw(this, 1);
        // SWITCH jmp=0x0042C6CF table=0x0042FC7C count=50 how=cmp bias=0: hover id -> help image
        if (CCH_HOVER != -1)
        {
            switch (CCH_HOVER)
            {
            case 0: // 0x0042C6D6
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3360), CCH_AT(0x80));
                break;
            case 1: // 0x0042C6E9
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3364), CCH_AT(0x90));
                break;
            case 2: // 0x0042C6FC
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3368), CCH_AT(0xa0));
                break;
            case 3: // 0x0042C70F
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B336C), CCH_AT(0xb0));
                break;
            case 4: // 0x0042C722
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3370), CCH_AT(0xc0));
                break;
            case 5: // 0x0042C735
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3374), CCH_AT(0xd0));
                break;
            case 6: // 0x0042C748
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3378), CCH_AT(0xe0));
                break;
            case 7: // 0x0042C75B
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B337C), CCH_AT(0xf0));
                break;
            case 8: // 0x0042C76E
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3380), CCH_AT(0x100));
                break;
            case 9: // 0x0042C781
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3384), CCH_AT(0x110));
                break;
            case 0xA: // 0x0042C794
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3388), CCH_AT(0x120));
                break;
            case 0xB: // 0x0042C7A7
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B338C), CCH_AT(0x130));
                break;
            case 0xC: // 0x0042C7BA
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3390), CCH_AT(0x140));
                break;
            case 0xD: // 0x0042C7CD
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B335C), CCH_AT(0x150));
                break;
            case 0xE: // 0x0042C7E0
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x160));
                break;
            case 0xF: // 0x0042C7EC
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x170));
                break;
            case 0x10: // 0x0042C7F8
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x180));
                break;
            case 0x11: // 0x0042C804
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x190));
                break;
            case 0x12: // 0x0042C810
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x1a0));
                break;
            case 0x13: // 0x0042C81C
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x1b0));
                break;
            case 0x14: // 0x0042C828
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x1c0));
                break;
            case 0x15: // 0x0042C834
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x1d0));
                break;
            case 0x16: // 0x0042C840
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x1e0));
                break;
            case 0x17: // 0x0042C84C
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x1f0));
                break;
            case 0x18: // 0x0042C858
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x200));
                break;
            case 0x19: // 0x0042C864
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x210));
                break;
            case 0x1A: // 0x0042C870
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x220));
                break;
            case 0x1B: // 0x0042C87C
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x230));
                break;
            case 0x1C: // 0x0042C888
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33A4), CCH_AT(0x240));
                break;
            case 0x1D: // 0x0042C89B
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33A8), CCH_AT(0x250));
                break;
            case 0x1E: // 0x0042C8AE
                if (CCH_DispFlag94())
                    CCH_SetRect(rc, 1, 0x1b3, 0x66, 0x1c5);
                else
                    CCH_SetRect(rc, 1, 0x196, 0x66, 0x1a8);
                CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl74), rc, CCH_AT((CCH_HOVER + 8) << 4));
                break;
            case 0x1F: // 0x0042C909
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B332C), CCH_AT(0x270));
                break;
            case 0x20: // 0x0042C91C
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3330), CCH_AT(0x280));
                break;
            case 0x21: // 0x0042C92F
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3334), CCH_AT(0x290));
                break;
            case 0x22: // 0x0042C942
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3338), CCH_AT(0x2a0));
                break;
            case 0x23: // 0x0042C955
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B333C), CCH_AT(0x2b0));
                break;
            case 0x24: // 0x0042C968
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3340), CCH_AT(0x2c0));
                break;
            case 0x25: // 0x0042C97B
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3344), CCH_AT(0x2d0));
                break;
            case 0x26: // 0x0042C98E
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3348), CCH_AT(0x2e0));
                break;
            case 0x27: // 0x0042C9A1
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B334C), CCH_AT(0x2f0));
                break;
            case 0x28: // 0x0042C9B4
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3350), CCH_AT(0x300));
                break;
            case 0x29: // 0x0042C9C7
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3354), CCH_AT(0x310));
                break;
            case 0x2A: // 0x0042C9DA
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3358), CCH_AT(0x320));
                break;
            case 0x2B: // 0x0042C9EA
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x330));
                break;
            case 0x2C: // 0x0042C9F3
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(0x340));
                break;
            case 0x2D: // 0x0042CA03
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33C4), CCH_AT(0x350));
                break;
            case 0x2E: // 0x0042CA13
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33C8), CCH_AT(0x360));
                break;
            case 0x30: // 0x0042CA23
                if (CCH_DispFlag94())
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B339C), CCH_AT(0x380));
                break;
            case 0x31: // 0x0042CA40
                if (CCH_DispFlag94())
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33A0), CCH_AT(0x390));
                break;
            case 0x2F: // -> 0x0042CA60 (the join)
                break;
            default:
                break;
            }
        }
        CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3394), CCH_AT(this->gender ? 0x340 : 0x330));
        {
            short nameLen = (short)CCH_strlen(*(char **)((char *)CCH_Screen + 0x660));

            CCH_strcpy(buf, *(char **)((char *)CCH_Screen + 0x660));
            buf[nameLen] = '_';
            buf[nameLen + 1] = 0;
            CCH_Text(buf, 0xFA - (short)((CCH_strlen(buf) * 9) >> 1), 0x1AE, 10, CCH_Color08);
        }
        // SWITCH jmp=0x0042CB22 table=0x0042FD44 count=12 how=cmp bias=-1: race id + 1 (0x80 and 0x82 are compare-chain outliers of the same switch)
        switch ((int)CCH_RACE + 1)
        {
        case 2: // 0x0042CB29
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B332C), CCH_AT(0x270));
            break;
        case 6: // 0x0042CB3C
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3330), CCH_AT(0x280));
            break;
        case 8: // 0x0042CB4F
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3334), CCH_AT(0x290));
            break;
        case 3: // 0x0042CB62
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3338), CCH_AT(0x2a0));
            break;
        case 0xC: // 0x0042CB75
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B333C), CCH_AT(0x2b0));
            break;
        case 7: // 0x0042CB88
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3340), CCH_AT(0x2c0));
            break;
        case 0xB: // 0x0042CB9B
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3344), CCH_AT(0x2d0));
            break;
        case 5: // 0x0042CBAE
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3348), CCH_AT(0x2e0));
            break;
        case 1: // 0x0042CBBE
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B334C), CCH_AT(0x2f0));
            break;
        case 0xA: // 0x0042CBCE
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3350), CCH_AT(0x300));
            break;
        case 9: // 0x0042CBDE
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3354), CCH_AT(0x310));
            break;
        case 4: // 0x0042CBEE
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3358), CCH_AT(0x320));
            break;
        case 0x80: // 0x0042CBFE
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33C8), CCH_AT(0x360));
            break;
        case 0x82: // 0x0042CC0E
            if (CCH_DispFlag94())
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B339C), CCH_AT(0x380));
            break;
        default:
            break;
        }
        // SWITCH jmp=0x0042CC5E table=0x0042FD74 count=15 how=cmp bias=0: class index loop, image drawn when CheckClass says no
        for (i = 0; i < 15; i++)
        {
            if (!CCH_CheckClass(CCH_CHAR, (short)CCH_RACE_BYTE, i))
            {
                switch (i)
                {
                case 7: // 0x0042CC65
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33CC), CCH_AT(0x80));
                    break;
                case 1: // 0x0042CC78
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33D0), CCH_AT(0x90));
                    break;
                case 5: // 0x0042CC8B
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33D4), CCH_AT(0xa0));
                    break;
                case 0xD: // 0x0042CC9E
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33D8), CCH_AT(0xb0));
                    break;
                case 0xC: // 0x0042CCB1
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33DC), CCH_AT(0xc0));
                    break;
                case 6: // 0x0042CCC4
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33E0), CCH_AT(0xd0));
                    break;
                case 0xA: // 0x0042CCD7
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33E4), CCH_AT(0xe0));
                    break;
                case 2: // 0x0042CCEA
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33E8), CCH_AT(0xf0));
                    break;
                case 3: // 0x0042CCFA
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33EC), CCH_AT(0x100));
                    break;
                case 8: // 0x0042CD0A
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33F0), CCH_AT(0x110));
                    break;
                case 4: // 0x0042CD1A
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33F4), CCH_AT(0x120));
                    break;
                case 9: // 0x0042CD2A
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33F8), CCH_AT(0x130));
                    break;
                case 0: // 0x0042CD3A
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33FC), CCH_AT(0x140));
                    break;
                case 0xB: // 0x0042CD4A
                    CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3400), CCH_AT(0x150));
                    break;
                case 0xE: // 0x0042CD5A
                    if (CCH_DispFlag94())
                        CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3404), CCH_AT(0x390));
                    break;
                default:
                    break;
                }
            }
        }
        // SWITCH jmp=0x0042CD95 table=0x0042FDB0 count=15 how=cmp bias=0: class id (byte) -> image
        switch (CCH_CLASS)
        {
        case 7: // 0x0042CD9C
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3360), CCH_AT(0x80));
            break;
        case 1: // 0x0042CDAF
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3364), CCH_AT(0x90));
            break;
        case 5: // 0x0042CDC2
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3368), CCH_AT(0xa0));
            break;
    // END REGION R01

    // REGION R02 0x0042CDD5-0x0042D35A: T2 AGNOSTIC + deity table header (CreationTableValue x7, sprintf x8, JT 42CD95)
        case 0xD: // 0x0042CDD5
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B336C), CCH_AT(0xb0));
            break;
        case 0xC: // 0x0042CDE8
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3370), CCH_AT(0xc0));
            break;
        case 6: // 0x0042CDFB
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3374), CCH_AT(0xd0));
            break;
        case 0xA: // 0x0042CE0E
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3378), CCH_AT(0xe0));
            break;
        case 2: // 0x0042CE21
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B337C), CCH_AT(0xf0));
            break;
        case 3: // 0x0042CE31
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3380), CCH_AT(0x100));
            break;
        case 8: // 0x0042CE41
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3384), CCH_AT(0x110));
            break;
        case 4: // 0x0042CE51
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3388), CCH_AT(0x120));
            break;
        case 9: // 0x0042CE61
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B338C), CCH_AT(0x130));
            break;
        case 0: // 0x0042CE71
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3390), CCH_AT(0x140));
            break;
        case 0xB: // 0x0042CE81
            CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B335C), CCH_AT(0x150));
            break;
        case 0xE: // 0x0042CE91
            if (CCH_DispFlag94())
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33A0), CCH_AT(0x390));
            break;
        default:
            break;
        }
        {
            short w;

            SOC_FMT(buf, CCH_FmtD, this->pointsLeft);
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24B - w, 0x23, 0, CCH_Color08);
            SOC_FMT(buf, CCH_FmtD, *((u8 *)CCH_CHAR + 0x77));
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24C - w, 0x53, 7, CCH_Color08);
            CCH_Text(buf, 0x24C - w, 0x53, CCH_CreationTableValue(CCH_CHAR, CCH_CLASS, 0) > 0 ? 0xE : 0xA, CCH_Color08);
            SOC_FMT(buf, CCH_FmtD, *((u8 *)CCH_CHAR + 0x78));
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24C - w, 0x7B, CCH_CreationTableValue(CCH_CHAR, CCH_CLASS, 3) > 0 ? 0xE : 0xA, CCH_Color08);
            SOC_FMT(buf, CCH_FmtD, *((u8 *)CCH_CHAR + 0x7C));
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24C - w, 0xA4, CCH_CreationTableValue(CCH_CHAR, CCH_CLASS, 2) > 0 ? 0xE : 0xA, CCH_Color08);
            SOC_FMT(buf, CCH_FmtD, *((u8 *)CCH_CHAR + 0x7A));
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24C - w, 0xCD, CCH_CreationTableValue(CCH_CHAR, CCH_CLASS, 1) > 0 ? 0xE : 0xA, CCH_Color08);
            SOC_FMT(buf, CCH_FmtD, *((u8 *)CCH_CHAR + 0x7D));
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24C - w, 0xF6, CCH_CreationTableValue(CCH_CHAR, CCH_CLASS, 5) > 0 ? 0xE : 0xA, CCH_Color08);
            SOC_FMT(buf, CCH_FmtD, *((u8 *)CCH_CHAR + 0x7B));
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24C - w, 0x11E, CCH_CreationTableValue(CCH_CHAR, CCH_CLASS, 4) > 0 ? 0xE : 0xA, CCH_Color08);
            SOC_FMT(buf, CCH_FmtD, *((u8 *)CCH_CHAR + 0x79));
            w = (short)((CCH_strlen(buf) * 9) >> 1);
            CCH_Text(buf, 0x24C - w, 0x147, CCH_CreationTableValue(CCH_CHAR, CCH_CLASS, 6) > 0 ? 0xE : 0xA, CCH_Color08);
        }
    }
    else if (CCH_MODE == 1)
    {
        CCH_SetRect(rc, 0, 0, 0x280, 0x1E0);
        CCH_DrawOverlay(CCH_OvlHandle(CCH_Ovl78), rc, rc);
        if (CCH_HOVER >= 0)
        {
            if (CCH_HOVER == 0x1C)
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33A4), CCH_AT(0x240));
            else if (CCH_HOVER == 0x1D)
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B33A8), CCH_AT(0x250));
        }
        // SWITCH jmp=0x0042D300 table=0x0042FDEC count=17 how=cmp bias=0: selected deity (< 0 = none: straight to the button loop)
        if (CCH_DEITY >= 0)
        {
            switch (CCH_DEITY)
            {
            case 0: // 0x0042D307
                CCH_Text(CCH_S(0x005426CC), 0x118, 0x3C, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00545490), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00545460), 0x118, 0x5F, 10, CCH_Color04);
                break;
    // END REGION R02

    // REGION R03 0x0042D35A-0x0042D8BC: T3 deity ids -0x23F..-0x23D (BERTOXXULOUS)
            case 1: // 0x0042D35A
                CCH_Text(CCH_S(0x0054543c), 0x118, 0x3c, 10, CCH_Color04);  // 0x0042D376
                CCH_Text(CCH_S(0x00545410), 0x118, 0x50, 10, CCH_Color04);  // 0x0042D392
                CCH_Text(CCH_S(0x005453e4), 0x118, 0x5f, 10, CCH_Color04);  // 0x0042D3AE
                CCH_Text(CCH_S(0x005453b8), 0x118, 0x6e, 10, CCH_Color04);  // 0x0042D3CA
                CCH_Text(CCH_S(0x0054538c), 0x118, 0x7d, 10, CCH_Color04);  // 0x0042D3E6
                CCH_Text(CCH_S(0x00545360), 0x118, 0x8c, 10, CCH_Color04);  // 0x0042D405
                CCH_Text(CCH_S(0x00545334), 0x118, 0x9b, 10, CCH_Color04);  // 0x0042D424
                CCH_Text(CCH_S(0x00545308), 0x118, 0xaa, 10, CCH_Color04);  // 0x0042D443
                CCH_Text(CCH_S(0x005452dc), 0x118, 0xb9, 10, CCH_Color04);  // 0x0042D462
                CCH_Text(CCH_S(0x005452b0), 0x118, 0xc8, 10, CCH_Color04);  // 0x0042D481
                CCH_Text(CCH_S(0x00545284), 0x118, 0xd7, 10, CCH_Color04);  // 0x0042D4A0
                CCH_Text(CCH_S(0x00545258), 0x118, 0xe6, 10, CCH_Color04);  // 0x0042D4BF
                CCH_Text(CCH_S(0x00545228), 0x118, 0xf5, 10, CCH_Color04);  // 0x0042D4DE
                CCH_Text(CCH_S(0x005451fc), 0x118, 0x104, 10, CCH_Color04);  // 0x0042D4FD
                CCH_Text(CCH_S(0x005451d0), 0x118, 0x113, 10, CCH_Color04);  // 0x0042D51C
                CCH_Text(CCH_S(0x005451a4), 0x118, 0x122, 10, CCH_Color04);  // 0x0042D53B
                CCH_Text(CCH_S(0x00545174), 0x118, 0x131, 10, CCH_Color04);  // 0x0042D554
                break;
            case 2: // 0x0042D559
                CCH_Text(CCH_S(0x00545154), 0x118, 0x3c, 10, CCH_Color04);  // 0x0042D575
                CCH_Text(CCH_S(0x00545128), 0x118, 0x50, 10, CCH_Color04);  // 0x0042D591
                CCH_Text(CCH_S(0x005450f8), 0x118, 0x5f, 10, CCH_Color04);  // 0x0042D5AD
                CCH_Text(CCH_S(0x005450cc), 0x118, 0x6e, 10, CCH_Color04);  // 0x0042D5C9
                CCH_Text(CCH_S(0x005450a4), 0x118, 0x7d, 10, CCH_Color04);  // 0x0042D5E5
                CCH_Text(CCH_S(0x00545078), 0x118, 0x8c, 10, CCH_Color04);  // 0x0042D604
                CCH_Text(CCH_S(0x0054504c), 0x118, 0x9b, 10, CCH_Color04);  // 0x0042D623
                CCH_Text(CCH_S(0x00545024), 0x118, 0xaa, 10, CCH_Color04);  // 0x0042D642
                CCH_Text(CCH_S(0x00544ff8), 0x118, 0xb9, 10, CCH_Color04);  // 0x0042D661
                CCH_Text(CCH_S(0x00544fcc), 0x118, 0xc8, 10, CCH_Color04);  // 0x0042D680
                CCH_Text(CCH_S(0x00544fa0), 0x118, 0xd7, 10, CCH_Color04);  // 0x0042D69F
                CCH_Text(CCH_S(0x00544f74), 0x118, 0xe6, 10, CCH_Color04);  // 0x0042D6BE
                CCH_Text(CCH_S(0x00544f48), 0x118, 0xf5, 10, CCH_Color04);  // 0x0042D6DD
                CCH_Text(CCH_S(0x00544f18), 0x118, 0x104, 10, CCH_Color04);  // 0x0042D6FC
                CCH_Text(CCH_S(0x00544f08), 0x118, 0x114, 10, CCH_Color04);  // 0x0042D715
                break;
            case 3: // 0x0042D71A
                CCH_Text(CCH_S(0x00544eec), 0x118, 0x3c, 10, CCH_Color04);  // 0x0042D736
                CCH_Text(CCH_S(0x00544ebc), 0x118, 0x50, 10, CCH_Color04);  // 0x0042D752
                CCH_Text(CCH_S(0x00544e94), 0x118, 0x5f, 10, CCH_Color04);  // 0x0042D76E
                CCH_Text(CCH_S(0x00544e6c), 0x118, 0x6e, 10, CCH_Color04);  // 0x0042D78A
                CCH_Text(CCH_S(0x00544e44), 0x118, 0x7d, 10, CCH_Color04);  // 0x0042D7A6
                CCH_Text(CCH_S(0x00544e18), 0x118, 0x8c, 10, CCH_Color04);  // 0x0042D7C5
                CCH_Text(CCH_S(0x00544df0), 0x118, 0x9b, 10, CCH_Color04);  // 0x0042D7E4
                CCH_Text(CCH_S(0x00544dc4), 0x118, 0xaa, 10, CCH_Color04);  // 0x0042D803
                CCH_Text(CCH_S(0x00544d9c), 0x118, 0xb9, 10, CCH_Color04);  // 0x0042D822
                CCH_Text(CCH_S(0x00544d70), 0x118, 0xc8, 10, CCH_Color04);  // 0x0042D841
                CCH_Text(CCH_S(0x00544d48), 0x118, 0xd7, 10, CCH_Color04);  // 0x0042D860
                CCH_Text(CCH_S(0x00544d1c), 0x118, 0xe6, 10, CCH_Color04);  // 0x0042D87F
                CCH_Text(CCH_S(0x00544cf0), 0x118, 0xf5, 10, CCH_Color04);  // 0x0042D89E
                CCH_Text(CCH_S(0x00544cc8), 0x118, 0x104, 10, CCH_Color04);  // 0x0042D8B7
                break;
    // END REGION R03

    // REGION R04 0x0042D8BC-0x0042DEB9: T4 (INNORUUK ...)
            case 4: // 0x0042D8BC
                CCH_Text(CCH_S(0x00544ca8), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544c7c), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544c50), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544c20), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544bf4), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544bc8), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544b98), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544b6c), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544b3c), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544b10), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544ae4), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544ab8), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544a8c), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544a60), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544a30), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544a04), 0x118, 0x122, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005449f8), 0x118, 0x131, 10, CCH_Color04);
                break;
            case 5: // 0x0042DABB
                CCH_Text(CCH_S(0x005449e0), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005449b4), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054498c), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544960), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054493c), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544914), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005448e4), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005448b8), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544888), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544864), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544838), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544810), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005447e0), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005447b4), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544788), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544758), 0x118, 0x122, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054474c), 0x118, 0x131, 10, CCH_Color04);
                break;
            case 6: // 0x0042DCBA
                CCH_Text(CCH_S(0x00544728), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544700), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005446d0), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005446a4), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544674), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544644), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544614), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005445e8), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005445bc), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054458c), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544564), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544538), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544510), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005444e4), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005444bc), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544494), 0x118, 0x122, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544470), 0x118, 0x131, 10, CCH_Color04);
                break;
    // END REGION R04

    // REGION R05 0x0042DEB9-0x0042E459: T5 (MITHANIEL MARR)
            case 7: // 0x0042DEB9
                CCH_Text(CCH_S(0x0054444c), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544420), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005443f4), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005443c4), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544398), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544374), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544348), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054431c), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005442f4), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005442c4), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054429c), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544270), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544244), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054421c), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005441f4), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005441d0), 0x118, 0x122, 10, CCH_Color04);
                break;
            case 8: // 0x0042E099
                CCH_Text(CCH_S(0x005441b0), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544188), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054415c), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544130), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544100), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005440d8), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005440ac), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544080), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544054), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00544028), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543ffc), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543fd0), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543fa0), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543f78), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543f4c), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543f24), 0x118, 0x122, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543f0c), 0x118, 0x131, 10, CCH_Color04);
                break;
            case 9: // 0x0042E298
                CCH_Text(CCH_S(0x00543ef4), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543ecc), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543ea0), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543e74), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543e48), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543e20), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543df4), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543dc4), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543d98), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543d6c), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543d44), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543d1c), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543cf4), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543cc8), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543cc0), 0x118, 0x113, 10, CCH_Color04);
                break;
    // END REGION R05

    // REGION R06 0x0042E459-0x0042EA75: T6 (QUELLIOUS)
            case 0xA: // 0x0042E459
                CCH_Text(CCH_S(0x543ca4), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543c78), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543c4c), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543c20), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543bf0), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543bc0), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543b90), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543b64), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543b3c), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543b0c), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543ae0), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543ab4), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543a88), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543a5c), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543a30), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543a04), 0x118, 0x122, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5439dc), 0x118, 0x131, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5439cc), 0x118, 0x140, 10, CCH_Color04);
                break;
            case 0xB: // 0x0042E677
                CCH_Text(CCH_S(0x5439ac), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543980), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543954), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543928), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5438fc), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5438d0), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5438a4), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543878), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543848), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x54381c), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5437f0), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5437c0), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543794), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x54376c), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543744), 0x118, 0x113, 10, CCH_Color04);
                break;
            case 0xC: // 0x0042E838
                CCH_Text(CCH_S(0x543720), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5436f4), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5436c8), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x54369c), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543674), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543648), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x54361c), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5435f4), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5435cc), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5435a4), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543578), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543550), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543524), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543500), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5434d4), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x5434a8), 0x118, 0x122, 10, CCH_Color04);
                CCH_Text(CCH_S(0x54347c), 0x118, 0x131, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543450), 0x118, 0x140, 10, CCH_Color04);
                CCH_Text(CCH_S(0x543430), 0x118, 0x14f, 10, CCH_Color04);
                break;
    // END REGION R06

    // REGION R07 0x0042EA75-0x0042F015: T7 (THE TRIBUNAL)
            case 0xD: // 0x0042EA75
                CCH_Text(CCH_S(0x00543410), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005433e0), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005433b4), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543388), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054335c), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543330), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543308), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005432dc), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005432b4), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543284), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543258), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x0054322c), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543204), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005431d8), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005431ac), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543198), 0x118, 0x122, 10, CCH_Color04);
                break;
            case 0xE: // 0x0042EC55
                CCH_Text(CCH_S(0x0054317c), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543150), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543124), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005430f8), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005430cc), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x005430a0), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543070), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543048), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00543020), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542ff4), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542fcc), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542fa0), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542f70), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542f48), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542f1c), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542f0c), 0x118, 0x122, 10, CCH_Color04);
                break;
            case 0xF: // 0x0042EE35
                CCH_Text(CCH_S(0x00542ef4), 0x118, 0x3c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542ec4), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542e94), 0x118, 0x5f, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542e68), 0x118, 0x6e, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542e38), 0x118, 0x7d, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542e08), 0x118, 0x8c, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542dd8), 0x118, 0x9b, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542dac), 0x118, 0xaa, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542d7c), 0x118, 0xb9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542d54), 0x118, 0xc8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542d24), 0x118, 0xd7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542cf4), 0x118, 0xe6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542cc8), 0x118, 0xf5, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542c9c), 0x118, 0x104, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542c6c), 0x118, 0x113, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542c48), 0x118, 0x122, 10, CCH_Color04);
                break;
    // END REGION R07

    // REGION R08 0x0042F015-0x0042F852: T8 (RALLOS ZEK .. last deities; JT 42F1C3; CityCanStart x6, drawPartial x7)
            case 0x10: // 0x0042F015
                CCH_Text(CCH_S(0x00542C2C), 0x118, 0x3C, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542C04), 0x118, 0x50, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542BD4), 0x118, 0x5F, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542BA8), 0x118, 0x6E, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542B7C), 0x118, 0x7D, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542B50), 0x118, 0x8C, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542B24), 0x118, 0x9B, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542AF4), 0x118, 0xAA, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542ACC), 0x118, 0xB9, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542AA0), 0x118, 0xC8, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542A70), 0x118, 0xD7, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542A40), 0x118, 0xE6, 10, CCH_Color04);
                CCH_Text(CCH_S(0x00542A28), 0x118, 0xF5, 10, CCH_Color04);
                break;
            default:
                break;
            }
        }
        // The deity text cases join here (0x0042F19E, also the switch default; VC6 tail-merged the last WriteTextHD
        // of the cases at 0x0042F193, each case writes its own): the deity button loop, ids 0x32..0x42. The page ends after it.
        {
            int i;
            int yofs;                           // EBP-0x4   name text offset, set per button by the switch below
            char *name = (char *)this + 0x19AA; // EBP-0xC   deity name strings, 0x1E apart
            char *pos = (char *)this + 0x3A4;   // ESI       button position structs, 0x10 apart
            for (i = 0x32; i < 0x43; i++, name += 0x1E, pos += 0x10)
            {
                u8 ok;
                int race1 = (int)CCH_RACE + 1;
                // SWITCH jmp=0x0042F1C3 table=0x0042FE30 count=17 how=cmp bias=-50: button id -> yofs
                switch (i)
                {
                case 0x32: // 0x0042F1CA
                    yofs = 0x3C;
                    break;
                case 0x33: case 0x34: // 0x0042F1D3
                    yofs = 0xA;
                    break;
                case 0x36: // 0x0042F1DC
                    yofs = 0xD;
                    break;
                case 0x37: // 0x0042F1E5
                    yofs = 0x1E;
                    break;
                case 0x39: case 0x3A: // 0x0042F1EE
                    yofs = 0x12;
                    break;
                case 0x3B: // 0x0042F1F7
                    yofs = 0x1C;
                    break;
                case 0x35: case 0x3C: // 0x0042F200
                    yofs = 0x28;
                    break;
                case 0x3D: // 0x0042F209
                    yofs = 9;
                    break;
                case 0x3E: // 0x0042F212
                    yofs = 0xE;
                    break;
                case 0x38: case 0x40: // 0x0042F21B
                    yofs = 0xF;
                    break;
                case 0x3F: case 0x41: // 0x0042F224
                    yofs = 0x19;
                    break;
                case 0x42: // 0x0042F22D
                    yofs = 0x2B;
                    break;
                default:
                    break;
                }
                // can this race (id + 1) worship deity i - 0x32 as this class? compare chain in this order; no match = no
                if (race1 == 1)
                    ok = CCH_HumanCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 5)
                    ok = CCH_HighElfCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 6)
                    ok = CCH_DarkElfCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 7)
                    ok = CCH_HalfElfCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 8)
                    ok = CCH_DwarfCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 10)
                    ok = CCH_OgreCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 9)
                    ok = CCH_TrollCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 3)
                    ok = CCH_EruditeCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 12)
                    ok = CCH_GnomeCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 11)
                    ok = CCH_HalflingCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 4)
                    ok = CCH_ElfCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 2)
                    ok = CCH_BarbarianCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 0x80)
                    ok = CCH_IksarCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else if (race1 == 0x82)
                    ok = CCH_VahShirCanWorship(CCH_CHAR, CCH_CLASS, (u8)(i - 0x32));
                else
                    ok = 0;
                {
                    int rt, rb; // button rect top/bottom: selected/hovered 0x1D/0x33, allowed idle 0x4F/0x65, not allowed 0x36/0x4C
                    if (ok)
                    {
                        if (CCH_DEITY == i - 0x32 || CCH_HOVER == i)
                        {
                            rt = 0x1D;
                            rb = 0x33;
                        }
                        else
                        {
                            rt = 0x4F;
                            rb = 0x65;
                        }
                    }
                    else
                    {
                        rt = 0x36;
                        rb = 0x4C;
                    }
                    *(void **)EQ_GLOBAL(void *, 0x006B341C) = CCH_OvlHandle(CCH_Ovl78);
                    CCH_SetRect((int *)EQ_GLOBAL(void *, 0x006B341C) + 1, 0x20, rt, 0xE7, rb);
                }
                CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B341C), pos - 4);
                CCH_strcpy(buf, name);
                yofs += 0x20;
                CCH_Text(buf, (short)yofs, *(int *)pos + 2, 0, CCH_Color08); // arg order as the original pushes it: yofs second, pos+2 third
            }
        }
    }
    else if (CCH_MODE == 2)
    {
        int c;
        char *pos;
        int y;
        CCH_SetRect(rc, 0, 0, 0x280, 0x1E0);
        CCH_DrawOverlay(CCH_OvlHandle(CCH_Ovl84), rc, rc);
        // city marker on the scale: y = 0x1AE - city * 0x2E / 12 (as a short)
        y = (short)(0x1AE - (int)CCH_CITY * 0x2E / 12);
        CCH_SetRect(rc, 0x1C8, y, 0x1D2, y + 0xB);
        CCH_ImageDraw(EQ_GLOBAL(void *, 0x006B3410), rc);
        if (CCH_HOVER == 0x1C)
            CCH_SetRect(rc, 0xD3, 0x140, 0x10F, 0x15C);
        else
            CCH_SetRect(rc, 0x231, 0x1B6, 0x26D, 0x1D2);
        CCH_DrawPartial(this, pinstEQ_CharCreateS_guess->displayOverlay_guess, rc, CCH_AT(0x240));
        if (CCH_HOVER == 0x1D)
        {
            CCH_SetRect(rc, 0xD3, 0x123, 0x10F, 0x13F);
            CCH_DrawPartial(this, pinstEQ_CharCreateS_guess->displayOverlay_guess, rc, CCH_AT(0x250));
        }
        // city icons 0x4A..0x52 (city ids 0..8): selected, startable, or nothing
        pos = CCH_AT(0x520);
        for (c = 0x4A; c <= 0x52; c++, pos += 0x10)
        {
            if (c - 0x4A == CCH_CITY)
                CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl80), pos, pos);
            else if (CCH_CityCanStart(CCH_CHAR, c - 0x4A, CCH_RACE, CCH_DEITY, CCH_CLASS))
                CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl7C), pos, pos);
        }
        // city 12 (only when the display flag at +0x88 is set), then 10, 9 and 11
        if (g_cdisplay_guess->fastFileHandle3_guess != 0)
        {
            if (CCH_CITY == 0xC)
                CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl80), CCH_AT(0x370), CCH_AT(0x370));
            else if (CCH_CityCanStart(CCH_CHAR, 0xC, CCH_RACE, CCH_DEITY, CCH_CLASS))
                CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl7C), CCH_AT(0x370), CCH_AT(0x370));
        }
        if (CCH_CITY == 10)
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl80), CCH_AT(0x5C0), CCH_AT(0x5C0));
        else if (CCH_CityCanStart(CCH_CHAR, 10, CCH_RACE, CCH_DEITY, CCH_CLASS))
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl7C), CCH_AT(0x5C0), CCH_AT(0x5C0));
        else
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl84), CCH_AT(0x5C0), CCH_AT(0x5C0));
        if (CCH_CITY == 9)
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl80), CCH_AT(0x5B0), CCH_AT(0x5B0));
        else if (CCH_CityCanStart(CCH_CHAR, 9, CCH_RACE, CCH_DEITY, CCH_CLASS))
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl7C), CCH_AT(0x5B0), CCH_AT(0x5B0));
        else
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl84), CCH_AT(0x5B0), CCH_AT(0x5B0));
        if (CCH_CITY == 0xB)
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl80), CCH_AT(0x5D0), CCH_AT(0x5D0));
        else if (CCH_CityCanStart(CCH_CHAR, 0xB, CCH_RACE, CCH_DEITY, CCH_CLASS))
            CCH_DrawPartial(this, CCH_OvlHandle(CCH_Ovl7C), CCH_AT(0x5D0), CCH_AT(0x5D0));
        fontsz = 10;
        if (CCH_CityCanStart(CCH_CHAR, CCH_CITY, CCH_RACE, CCH_DEITY, CCH_CLASS))
            CCH_strcpy(buf, CCH_S(0x00542A14));     // "You may begin here."
        else
        {
            CCH_strcpy(buf, CCH_S(0x005429FC));     // "You may not begin here."
            fontsz = 13;
        }
        // SWITCH jmp=0x0042F84B table=0x0042FE74 count=14 how=cmp bias=0: city id (default: nothing more to draw)
        switch (CCH_CITY)
        {
    // END REGION R08

    // REGION R09 0x0042F852-0x0042FBF2: T9 city names (JT 42F84B; IsEvilRace, IsClassNecromancerOrEnchanter x3)
        case 0: // 0x0042F852
            CCH_Text(CCH_S(0x005429DC), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 1: // 0x0042F86B
            CCH_Text(CCH_S(0x005429C4), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 2: // 0x0042F884
            CCH_Text(CCH_S(0x005429A4), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 3: // 0x0042F89D
            CCH_Text(CCH_S(0x00542980), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 4: // 0x0042F8B6
            CCH_Text(CCH_S(0x00542968), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 5: // 0x0042F8CF
            CCH_Text(CCH_S(0x00542948), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 6: // 0x0042F8E8
            CCH_Text(CCH_S(0x0054292C), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 7: // 0x0042F901
            CCH_Text(CCH_S(0x00542910), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 8: // 0x0042F91A
            CCH_Text(CCH_S(0x005428F0), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 9: // 0x0042F930
            CCH_Text(CCH_S(0x005428D0), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 0xA: // 0x0042F946
            CCH_Text(CCH_S(0x005428AC), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 0xB: // 0x0042F95C
            CCH_Text(CCH_S(0x0054288C), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 0xC: // 0x0042F972
            CCH_Text(CCH_S(0x00542870), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        case 0xD: // 0x0042F988
            CCH_Text(CCH_S(0x00542848), 0x1E, 0x17C, fontsz, CCH_Color04);
            break;
        default:
            return;
        }
        CCH_Text(buf, 0x1E, 0x18B, fontsz, CCH_Color04);
    }
    else if (CCH_MODE == 3)
    {
        CCH_SetRect(rc, 0, 0, 0x280, 0x1E0);
        CCH_DrawOverlay(CCH_OvlHandle(CCH_Ovl88), rc, rc);
        CCH_DrawPartial(this, pinstEQ_CharCreateS_guess->displayOverlay_guess, CCH_AT(0x5E0), CCH_AT(0x5E0));
        CCH_DrawPartial(this, pinstEQ_CharCreateS_guess->displayOverlay_guess, CCH_AT(0x5F0), CCH_AT(0x5F0));
        if (CCH_HOVER >= 0)
        {
            if (CCH_HOVER == 0x56)
            {
                CCH_SetRect(rc, 0xD3, 0x123, 0x10F, 0x13F);
                CCH_DrawPartial(this, pinstEQ_CharCreateS_guess->displayOverlay_guess, rc, CCH_AT(0x5E0));
            }
            else if (CCH_HOVER == 0x57)
            {
                CCH_SetRect(rc, 0xD3, 0x140, 0x10F, 0x15C);
                CCH_DrawPartial(this, pinstEQ_CharCreateS_guess->displayOverlay_guess, rc, CCH_AT(0x5F0));
            }
        }
        if (CCH_AT(0x868)[0] != 0)
        {
            int k;

            for (k = 0; k < *(int *)CCH_AT(0x1990); k++)
                CCH_Text(CCH_AT(0x1468 + 0x6E * k), 0x23, 0x32 + 0x14 * k, 10, CCH_Color04);
            CCH_Text(CCH_AT(0x1268), 0x23, *(int *)CCH_AT(0x1990) * 20 + 0x46, 10, CCH_Color04);
        }
        else
        {
            CCH_strcpy(buf, CCH_S(0x00542840));             // "RACE: "
            CCH_strcat(buf, CCH_GetRaceName((short)(CCH_RACE + 1)));
            CCH_Text(buf, 0x23, 0x32, 10, CCH_Color04);
            CCH_strcpy(buf, CCH_S(0x00542838));             // "CLASS: "
            CCH_strcat(buf, CCH_GetClassDesc(CCH_CLASS + 1));
            CCH_Text(buf, 0x23, 100, 10, CCH_Color04);
            if (CCH_IsEvilRace(this) && !CCH_IsClassNecro(this))
                CCH_strcpy(buf, CCH_S(0x005427DC));         // "Due to the race of this character, ..."
            else if (!CCH_IsEvilRace(this) && CCH_IsClassNecro(this))
    // END REGION R09

    // REGION R10 0x0042FBF2-0x0042FC7C: T10 tail notice ("Due to the class of this character, you will have many enemies...")
                CCH_strcpy(buf, CCH_S(0x00542780));         // "..."
            else if (CCH_IsEvilRace(this) && CCH_IsClassNecro(this))
                CCH_strcpy(buf, CCH_S(0x0054271C));         // "Due to the race and class of this character, ..."
            else
                return;
            CCH_Text(buf, 0x23, 200, 10, CCH_Color04);
            CCH_strcpy(buf, CCH_S(0x005426E4));             // "If you are certain you wish to proceed, choose NEXT."
            CCH_Text(buf, 0x23, 0xDC, 10, CCH_Color04);
    // END REGION R10
        }
    }
}

// FUNCTION: TRILOGY 0x0042feac
// Initializes the character selection screen layout and clickable regions.
// ORIGINAL FILE: EverQuest/EQ_CharCreateSelS.cpp (from Mac debug map)
// @early-stop: SetRect pointer lands in EBP and the 0x280/0x133 constants in EDI (original EDI/EBP), so SetRect/AddButton push and lea order differ; EQ_THISCALL dummy-EDX xors remain
char* EQ_CharSelS::construct_guess(void)
{
    char* self = (char*)this;
    int index;
    u32* valuePtr;
    EQ_SetRectFn_guess setRect;
    EQ_CharSelS_ctor_guard_guess cleanup(self);

    *(u32*)self = 0x005353c4;
    EQ_GLOBAL(int, 0x006B676C) = 0;
    EQ_GLOBAL(u32, 0x006B6764) =
        EQ_CDECL(u32, 0x004A461E, (void))();
    setRect = EQ_GLOBAL(EQ_SetRectFn_guess, 0x00534248);

    *(short*)(self + 0x8c) = -1;
    *(short*)(self + 0x82) = -1;
    *(short*)(self + 0x84) = -1;
    *(short*)(self + 0x8e) = -1;
    *(short*)(self + 0x80) = -1;
    *(short*)(self + 0xa0) = -1;
    *(short*)(self + 0xa2) = -1;
    *(u16*)(self + 0xa4) = 0;
    *(u32*)(self + 0x10) = 1;
    *(u32*)(self + 0x88) = 0;
    *(u32*)(self + 0x90) = 0;
    *(u32*)(self + 0x94) = 0;
    *(u32*)(self + 0x98) = 0;
    *(u32*)(self + 0x9c) = 0;

    setRect(self + 0x24, 0, 0, 0x280, 0x1e0);
    setRect(self + 0x14, 0, 0, 0x280, 0x1e0);
    *(u32*)(self + 0x70) = 0;
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        (self + 0x34, (const char*)0x0053e950);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        (self + 0x52, (const char*)0x005454bc);

    setRect(self + 0xa8, 0x30, 0x30, 0x133, 0x53);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(0, self + 0xa8, -1, -1, 0, 0);
    setRect(self + 0xb8, 0x30, 0x5f, 0x133, 0x82);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(1, self + 0xb8, -1, -1, 0, 0);
    setRect(self + 0xc8, 0x30, 0x8e, 0x133, 0xb1);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(2, self + 0xc8, -1, -1, 0, 0);
    setRect(self + 0xd8, 0x30, 0xbd, 0x133, 0xe0);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(3, self + 0xd8, -1, -1, 0, 0);
    setRect(self + 0xe8, 0x30, 0xec, 0x133, 0x10f);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(4, self + 0xe8, -1, -1, 0, 0);
    setRect(self + 0xf8, 0x30, 0x11b, 0x133, 0x13e);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(5, self + 0xf8, -1, -1, 0, 0);
    setRect(self + 0x108, 0x30, 0x14a, 0x133, 0x16d);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(6, self + 0x108, -1, -1, 0, 0);
    setRect(self + 0x118, 0x30, 0x179, 0x133, 0x19c);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(7, self + 0x118, -1, -1, 0, 0);
    setRect(self + 0x128, 0x30, 0x1ae, 0x8b, 0x1c9);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(8, self + 0x128, -1, -1, 0, 0);
    setRect(self + 0x138, 0xb1, 0x1ae, 0xec, 0x1c9);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(9, self + 0x138, -1, -1, 0, 0);
    setRect(self + 0x148, 0xf8, 0x1ae, 0x133, 0x1c9);
    EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), self)(10, self + 0x148, -1, -1, 0, 0);

    valuePtr = (u32*)(self + 0x1e8);
    for (index = 8; index != 0; index = index - 1)
    {
        *valuePtr = 0;
        valuePtr = valuePtr + 1;
    }
    EQ_METHOD(2, void, 0x00450168, (char*, int, int), self)(1, 0);
    EQ_METHOD(0, void, 0x004301E9, (char*), self)();
    EQ_GLOBAL(int, 0x006B70E4) = 1;
    cleanup.disarm();
    return self;
}

// FUNCTION: TRILOGY 0x00430d81
// Draws the character-select screen: the eight character names, the chosen one's zone or a status message.
// ORIGINAL FILE: EverQuest/EQ_CharCreateSelS.cpp (from Mac debug map)
// @early-stop: this and the loop index swap EBX/EDI and the zone temps differ; the savedScreen reload the fixtures need flips them
void __fastcall EQ_CharSelS_draw_guess(int screen)
{
    char ch;
    short itemIndex;
    int tmp;
    int textWidth;
    int nameOffset;
    char text[256];
    int savedScreen;
    int slotIndex;
    size_t textLen;
    int charIndex;

    savedScreen = screen;
    tmp = EQ_METHOD(0, int, 0x004504AC, (void *), (void *)screen)();
    if (tmp == 0)
    {
        return;
    }

    EQ_METHOD(1, void, 0x0040E49F, (void *, float), g_display_guess)(CCH_YonMode0);
    EQ_METHOD(1, void, 0x00450081, (void *, int), (void *)screen)(1);
    tmp = screen + 0xa8;
    nameOffset = 0;
    slotIndex = 0;
    do
    {
        textWidth = EQ_CDECL(int, 0x00522920, (const char *, const char *))
            ((char *)(EQ_GLOBAL(int, 0x006EFE2C) + nameOffset + 0x22400),
             (const char *)0x005454f0);
        if (textWidth == 0)
        {
            if (*(short *)((char *)screen + 0x84) == slotIndex)
            {
                EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B3310))((void *)tmp, 1);
            }
            else
            {
                EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B330C))((void *)tmp, 1);
            }
        }
        else if (*(short *)((char *)screen + 0x80) == slotIndex)
        {
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B3308))((void *)tmp, 1);
            EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                (text, (char *)(EQ_GLOBAL(int, 0x006EFE2C) + nameOffset + 0x22400));
            textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(text);
            textWidth = 0;
            charIndex = 0;
            if (0 < (int)textLen)
            {
                do
                {
                    ch = text[charIndex];
                    if (ch < '\0')
                    {
                        EQ_CDECL(void, 0x004F8220, (const char *, const char *, int, int))
                            ((const char *)0x0054591c, text, (int)ch, (int)ch);
                    }
                    else
                    {
                        textWidth = textWidth + *(int *)(EQ_GLOBAL(int, 0x006EFDEC) + ch * 4);
                    }
                    charIndex = charIndex + 1;
                } while (charIndex < (int)textLen);
            }
            EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, int, int, int), g_eqtexto_guess)(text,
                 (int)(short)(0xb1 - (short)(textWidth / 2)),
                 *(int *)(tmp + 4) + 10, 0, CCH_Color08, 1);
            screen = savedScreen;
        }
        else
        {
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B3304))((void *)tmp, 1);
            EQ_CDECL(char *, 0x005219C0, (char *, const char *))
                (text, (char *)(EQ_GLOBAL(int, 0x006EFE2C) + nameOffset + 0x22400));
            textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(text);
            textWidth = 0;
            charIndex = 0;
            if (0 < (int)textLen)
            {
                do
                {
                    if ('\0' < text[charIndex])
                    {
                        textWidth = textWidth + *(int *)(EQ_GLOBAL(int, 0x006EFDEC) +
                            text[charIndex] * 4);
                    }
                    charIndex = charIndex + 1;
                } while (charIndex < (int)textLen);
            }
            EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, int, int, int), g_eqtexto_guess)(text,
                 (int)(short)(0xb1 - (short)(textWidth / 2)),
                 *(int *)(tmp + 4) + 10, 0, CCH_Color08, 1);
        }
        slotIndex = slotIndex + 1;
        nameOffset = nameOffset + 0x1e;
        tmp = tmp + 0x10;
    } while (nameOffset < 0xf0);

    tmp = 0;
    if (EQ_GLOBAL(int, 0x006B61B4) != 0)
    {
        EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, int, int, int), g_eqtexto_guess)((char *)0x00545900, 0x18b, 0x1b9, 10,
             CCH_Color08, 1);
    }
    else if (*(int *)((char *)EQ_GLOBAL(void *, 0x006E4E6C) + 0x90) != 0 &&
             *(short *)((char *)EQ_GLOBAL(void *, 0x006E4E6C) + 0x84) == -1 &&
             *(short *)((char *)EQ_GLOBAL(void *, 0x006E4E6C) + 0x80) != -1)
    {
        itemIndex = EQ_METHOD(0, short, 0x004307FC, (void *), EQ_GLOBAL(void *, 0x006E4E6C))();
        if (itemIndex > 0)
        {
            EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, int, int, int), g_eqtexto_guess)((char *)0x005458ec, 0x1a9, 0x1b9, 10,
                 CCH_Color08, 1);
        }
    }
    else if (*(short *)((char *)screen + 0x80) != -1)
    {
        EQ_METHOD(2, char *, 0x004BEA23, (void *, char *, char *), EQ_GLOBAL(void *, 0x006E4D54))(
             (char *)(EQ_GLOBAL(int, 0x006EFE2C) +
                 *(short *)((char *)screen + 0x80) * 0x14 + 0x22554),
             text);
        textLen = EQ_CDECL(size_t, 0x005223D0, (const char *))(text);
        nameOffset = 0;
        if (0 < (int)textLen)
        {
            do
            {
                if ('\0' < text[tmp])
                {
                    nameOffset = nameOffset + *(int *)(EQ_GLOBAL(int, 0x006EFDEC) +
                        text[tmp] * 4);
                }
                tmp = tmp + 1;
            } while (tmp < (int)textLen);
        }
        tmp = (int)(short)(0x1e9 - (short)(nameOffset / 2));
        EQ_METHOD(6, void, 0x0048D21A, (void *, char *, int, int, int, int, int), g_eqtexto_guess)(text, tmp, 0x1b9, 10,
             CCH_Color08, 1);
    }

    itemIndex = *(short *)((char *)screen + 0x84);
    if (itemIndex != -1)
    {
        switch (itemIndex)
        {
        case 8:
            screen += 0x128;
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B3318))((void *)screen, 1);
            break;
        case 9:
            screen += 0x138;
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B331C))((void *)screen, 1);
            break;
        case 10:
            screen += 0x148;
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B3320))((void *)screen, 1);
            break;
        default:
            break;
        }
    }
    return;
}

class EQ_CharSelPrompt_guess
{
public:
    virtual void unknown0();
    virtual void show();
    virtual void refresh();
};

// FUNCTION: TRILOGY 0x004310f2
// Char-select pick: builds the preview model from a slot's saved character, or opens the delete-confirm prompt.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_CharCreateSelS.cpp (from Mac debug map)
void __fastcall HandleCharSelectChoice_guess(EQ_CharSelS* screen, int unusedEdx, short inputCode, short selectionIndex)
{
    EQ_CharSelS* savedScreen;
    char itemText[32];
    int cameraRace;
    int cameraStyle;
    int itemId;
    char* itemName;
    short material;
    int materialIndex;
    bool isEvil;
    f32 zCoordinate;
    char* everquestData;

    savedScreen = screen;
    screen->inputCode = inputCode;
    screen->resetValue = 0;
    if (screen->operationInProgress == 0)
    {
        if (EQ_GLOBAL(int, 0x006B6490) == 0)
        {
            EQ_GLOBAL(EQ_CharSelPrompt_guess*, 0x006E4DF0)->refresh();
            if (inputCode == 0)
            {
                if (selectionIndex < 8)
                {
                    EQ_GLOBAL(int, 0x006B61B4) = 0;
                    everquestData = (char*)g_everquest_guess;
                    if (EQ_CDECL(int, 0x00522920, (const char*, const char*))
                        (everquestData + selectionIndex * 0x1e + 0x22400,
                         EQ_ARRAY(char, 0x005454F0)) == 0)
                    {
                        screen->emptySelection = 1;
                    }
                    else
                    {
                        screen->selectedCharacter = selectionIndex;
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->appearance[5] =
                            *(u8*)(everquestData + selectionIndex + 0x2288a);
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->appearance[1] =
                            *(u8*)(everquestData + selectionIndex + 0x22862);
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->appearance[4] =
                            *(u8*)(everquestData + selectionIndex + 0x22880);
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->appearance[0] =
                            *(u8*)(everquestData + selectionIndex + 0x22858);
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->appearance[2] =
                            *(u8*)(everquestData + selectionIndex + 0x2286c);
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->appearance[3] =
                            *(u8*)(everquestData + selectionIndex + 0x22876);

                        cameraRace = ((int)g_everquest_guess & 0xffff0000) |
                            (u16)(*(s16*)(everquestData + selectionIndex * 2 + 0x22540) - 1);
                        cameraStyle = (cameraRace & 0xffffff00) |
                            *(u8*)(everquestData + selectionIndex + 0x2261c);
                        EQ_METHOD(4, void, 0x00430381, (void*, int, int, int, int),
                            EQ_GLOBAL(void*, 0x006E4E6C))(
                                cameraStyle, cameraRace, 0,
                                *(u8*)(everquestData + selectionIndex + 0x22536));

                        materialIndex = 0;
                        do
                        {
                            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->equipment[materialIndex] = 0;
                            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->equip_colors[materialIndex] = 0;
                            materialIndex = materialIndex + 1;
                        } while (materialIndex < 9);

                        material = 0;
                        do
                        {
                            if (material == 0)
                            {
                                EQ_METHOD(6, void, 0x00407334,
                                    (void*, void*, int, int, int, u32, int),
                                    g_display_guess)(
                                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
                                        *(u8*)(everquestData + selectionIndex * 9 + 0x22630),
                                        0, 0,
                                        *(u32*)(everquestData +
                                            (selectionIndex * 9 + 0x89a3) * 4), 0);
                            }
                            else
                            {
                                materialIndex = (int)material;
                                EQ_METHOD(5, void, 0x0040798E,
                                    (void*, void*, int, int, int, u32),
                                    g_display_guess)(
                                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C), materialIndex,
                                        *(u8*)(everquestData + materialIndex +
                                            selectionIndex * 9 + 0x22630),
                                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->equipment[materialIndex],
                                        *(u32*)(everquestData +
                                            (materialIndex + (selectionIndex + 0xf4b) * 9) * 4));
                                screen = savedScreen;
                            }
                            material = material + 1;
                        } while (material < 9);

                        everquestData = (char*)g_everquest_guess;
                        itemId = *(int*)(everquestData + selectionIndex * 4 + 0x22808);
                        if (itemId < 1)
                        {
                            itemName = EQ_ARRAY(char, 0x0053D860);
                        }
                        else
                        {
                            EQ_CDECL(int, 0x005224FB, (char*, const char*, int))
                                (itemText, EQ_ARRAY(char, 0x0053D7D4), itemId);
                            itemName = itemText;
                        }
                        EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, char*),
                            g_display_guess)(
                                EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 7, itemName);

                        everquestData = (char*)g_everquest_guess;
                        itemId = *(int*)(everquestData + selectionIndex * 4 + 0x22830);
                        if (itemId < 1)
                        {
                            itemName = EQ_ARRAY(char, 0x0053D860);
                        }
                        else
                        {
                            EQ_CDECL(int, 0x005224FB, (char*, const char*, int))
                                (itemText, EQ_ARRAY(char, 0x0053D7D4), itemId);
                            itemName = itemText;
                        }
                        EQ_METHOD(3, void, 0x0040B6E0, (void*, void*, int, char*),
                            g_display_guess)(
                                EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 8, itemName);

                        EQ_METHOD(3, void, 0x00406EDC, (void*, void*, int, int),
                            g_display_guess)(
                                EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
                                *(u8*)(everquestData + selectionIndex + 0x22626),
                                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->face);
                        isEvil = EQ_METHOD(3, bool, 0x00431509, (void*, int, int, int),
                            EQ_GLOBAL(void*, 0x006E4E6C))(
                                *(u16*)(everquestData + screen->selectedCharacter * 2 + 0x22540),
                                *(u8*)(everquestData + screen->selectedCharacter + 0x22536),
                                *(s16*)(everquestData + screen->selectedCharacter * 2 + 0x227f4));
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y = 0.0f;
                        if (isEvil == false)
                        {
                            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x = 0.0f;
                            zCoordinate = EQ_GLOBAL(f32, 0x005353B8);
                        }
                        else
                        {
                            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x = EQ_GLOBAL(f32, 0x005353C0);
                            zCoordinate = EQ_GLOBAL(f32, 0x005353BC);
                        }
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z = zCoordinate;
                        EQ_METHOD(1, void, 0x00410ABF, (void*, void*),
                            g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
                    }
                }
                else if (selectionIndex == 8)
                {
                    screen->enter_world = 1;
                    if (EQ_GLOBAL(int, 0x006EFE1C) == 0 &&
                        EQ_GLOBAL(int, 0x006ECDF4) != 0)
                    {
                        EQ_METHOD(1, void, 0x004E4EE6, (void*, int),
                            EQ_GLOBAL(void*, 0x006ECDF4))(0xf);
                    }
                }
                else if (selectionIndex == 9)
                {
                    if (screen->selectedCharacter >= 0)
                    {
                        EQ_CDECL(int, 0x005224FB, (char*, const char*, const char*))
                            ((char*)EQ_GLOBAL(void*, 0x006E4DF0) + 0xb1,
                             EQ_ARRAY(char, 0x00545960),
                             (char*)g_everquest_guess +
                                screen->selectedCharacter * 0x1e + 0x22400);
                        *((u8*)EQ_GLOBAL(void*, 0x006E4DF0) + 0xb0) = 1;
                        EQ_GLOBAL(EQ_CharSelPrompt_guess*, 0x006E4DF0)->show();
                        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DF0) + 0xac) =
                            EQ_CDECL(int, 0x004A461E, (void))() + 10000;
                    }
                }
                else if (selectionIndex == 10)
                {
                    screen->quit = 1;
                }
            }
        }
    }
    else if (inputCode == 0)
    {
        screen->selectionCode = selectionIndex;
    }
    return;
}

// FUNCTION: TRILOGY 0x00431509
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_CharCreateSelS.cpp (from Mac debug map)
// The original returns its result in AL and removes all three stack arguments.
bool EQ_CharSelS::IsEvil(int race, int class_id, int deity)
{
    bool result = false;
    if (race == 9 || race == 10 || race == 6 || race == 0x80)
        result = true;
    if (class_id == 0x0B || class_id == 5)
        result = true;
    if (deity == 1 || deity == 3 || deity == 4 ||
        deity == 0xC9 || deity == 0xCB || deity == 0xCE)
        result = true;
    return result;
}

// Copies the welcome text for a start city into buf and returns buf. The texts are the binary's own strings,
// referenced by address (as EQAnimate.cpp does); EQ_STRCPY calls the game's strcpy.
#define EQ_STRCPY(dst, src) EQ_CDECL(char *, 0x005219C0, (char *, const char *))((dst), (src))

// FUNCTION: TRILOGY 0x00431625
// Copies the character-creation welcome text for the chosen race (home city), class, deity and start city.
// __cdecl, returns buf. start: 0 = Qeynos/Freeport, 1 = Halas, 2 = Erudin, 3 = Kelethin, 4 = Felwithe,
// 5 = Neriak, 6 = Qeynos/Freeport/Kelethin, 7 = Kaladim, 8 = Grobb, 9 = Oggok, 10 = Rivervale,
// 11 = Ak'Anon, 0x7F = Cabilis, 0x81 = Vah Shir. side: 1 = Qeynos, 4 = Freeport.
// @early-stop: class-register allocation and branch-tail layout still differ (ratio 0.8963).
/* fake match: incremented start and one-based class values preserve the original's comparison shape. */
char *EQ_CharSelS_GetWelcomeText_guess(char *welcome, int start, int cls, int deity, int side)
{
    ++start;
    *welcome = 0;
    if (start == 1)
    {
        int clsOneBased = cls + 1;

        if (clsOneBased == 1)
        {
            if (deity == 1)
                EQ_STRCPY(welcome, (char *)0x00557C60);
            else
            {
                if (side == 1)
                    EQ_STRCPY(welcome, (char *)0x00557A9C);
                if (side != 4)
                    /* fake match: skip the side-4 copy and continue at the next class test. */
                    goto nextClassTest;
                EQ_STRCPY(welcome, (char *)0x00557884);
            }
        }
    nextClassTest:
        if (clsOneBased == 2)
        {
            if (deity == 7)
                EQ_STRCPY(welcome, (char *)0x00557644);
            if (deity == 6)
                EQ_STRCPY(welcome, (char *)0x00557420);
            if (deity == 5)
                EQ_STRCPY(welcome, (char *)0x00557214);
            if (deity == 8)
                EQ_STRCPY(welcome, (char *)0x00557010);
            if (deity == 1)
                EQ_STRCPY(welcome, (char *)0x00556D54);
            if (deity == 4)
                EQ_STRCPY(welcome, (char *)0x00556B08);
        }
        if (clsOneBased == 3)
        {
            if (deity == 7)
                EQ_STRCPY(welcome, (char *)0x005568C8);
            if (deity == 6)
                EQ_STRCPY(welcome, (char *)0x005566A4);
            if (deity == 5)
                EQ_STRCPY(welcome, (char *)0x005564A4);
            if (deity == 8)
                EQ_STRCPY(welcome, (char *)0x0055626C);
        }
        if (clsOneBased == 4)
            EQ_STRCPY(welcome, (char *)0x00556014);
        if (clsOneBased == 5)
        {
            if (side == 1)
                EQ_STRCPY(welcome, (char *)0x00555D60);
            if (side == 4)
                EQ_STRCPY(welcome, (char *)0x00555B1C);
        }
        if (clsOneBased == 6)
        {
            if (deity == 5)
                EQ_STRCPY(welcome, (char *)0x00555890);
            if (deity == 0x0E)
                EQ_STRCPY(welcome, (char *)0x00555654);
        }
        if (clsOneBased == 7)
        {
            if (side == 1)
                EQ_STRCPY(welcome, (char *)0x0055545C);
            if (side == 4)
                EQ_STRCPY(welcome, (char *)0x0055522C);
        }
        if (clsOneBased == 8)
        {
            if (side == 1)
                EQ_STRCPY(welcome, (char *)0x00554FB8);
            if (side == 4)
                EQ_STRCPY(welcome, (char *)0x00554D54);
        }
        if (clsOneBased == 9)
        {
            if (side == 1)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x00554AD8);
                else
                    EQ_STRCPY(welcome, (char *)0x005548D8);
            }
            else if (side == 4)
            {
                if (deity == 4)
                    EQ_STRCPY(welcome, (char *)0x005546E4);
                else
                    EQ_STRCPY(welcome, (char *)0x00554520);
            }
        }
        if (clsOneBased == 11)
        {
            if (side == 1)
                EQ_STRCPY(welcome, (char *)0x005541EC);
            if (side == 4)
                EQ_STRCPY(welcome, (char *)0x00553F5C);
        }
        if (clsOneBased == 12)
        {
            if (side == 1)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x00553C10);
                else
                    EQ_STRCPY(welcome, (char *)0x00553978);
            }
            else if (side == 4)
            {
                if (deity == 4)
                    EQ_STRCPY(welcome, (char *)0x005536DC);
                else
                    EQ_STRCPY(welcome, (char *)0x0055349C);
            }
        }
        if (clsOneBased == 13)
        {
            if (side == 1)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x00553178);
                else
                    EQ_STRCPY(welcome, (char *)0x00552F0C);
            }
            else if (side == 4)
            {
                if (deity == 4)
                    EQ_STRCPY(welcome, (char *)0x00552C98);
                else
                    EQ_STRCPY(welcome, (char *)0x00552A84);
            }
        }
        if (clsOneBased != 14)
            return welcome;
        if (side == 1)
        {
            if (deity == 1)
                EQ_STRCPY(welcome, (char *)0x00552740);
            else
                EQ_STRCPY(welcome, (char *)0x005524BC);
        }
        else if (side == 4)
        {
            if (deity == 4)
                EQ_STRCPY(welcome, (char *)0x0055222C);
            else
                EQ_STRCPY(welcome, (char *)0x00552000);
        }
        return welcome;
    }
    else
    {

        if (start == 6)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 1)
                EQ_STRCPY(welcome, (char *)0x00551DBC);
            if (clsOneBased == 5)
                EQ_STRCPY(welcome, (char *)0x00551A34);
            if (clsOneBased == 9)
                EQ_STRCPY(welcome, (char *)0x00551838);
            if (clsOneBased == 2)
                EQ_STRCPY(welcome, (char *)0x005515C8);
            if (clsOneBased == 11)
                EQ_STRCPY(welcome, (char *)0x00551238);
            if (clsOneBased != 0xC && clsOneBased != 0xD && clsOneBased != 0xE)
                return welcome;
            EQ_STRCPY(welcome, (char *)0x00550FF4);
            return welcome;
        }
        else if (start == 8)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 2)
                EQ_STRCPY(welcome, (char *)0x00550E90);
            if (clsOneBased == 3)
                EQ_STRCPY(welcome, (char *)0x00550CDC);
            if (clsOneBased == 9)
                EQ_STRCPY(welcome, (char *)0x00550B58);
            if (clsOneBased != 1)
                return welcome;
            EQ_STRCPY(welcome, (char *)0x005509A8);
            return welcome;
        }
        else if (start == 3)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 5)
                EQ_STRCPY(welcome, (char *)0x00550768);
            if (clsOneBased == 2)
            {
                if (deity == 9)
                    EQ_STRCPY(welcome, (char *)0x005504B0);
                else if (deity == 3)
                    EQ_STRCPY(welcome, (char *)0x00550224);
                else
                    EQ_STRCPY(welcome, (char *)0x0054FF8C);
            }
            if (clsOneBased == 3)
            {
                if (deity == 9)
                    EQ_STRCPY(welcome, (char *)0x0054FCBC);
                else
                    EQ_STRCPY(welcome, (char *)0x0054FAA8);
            }
            if (clsOneBased == 11)
                EQ_STRCPY(welcome, (char *)0x0054F818);
            if (clsOneBased == 0xC)
                EQ_STRCPY(welcome, (char *)0x0054F4C0);
            if (clsOneBased == 0xD)
                EQ_STRCPY(welcome, (char *)0x0054F194);
            if (clsOneBased != 0xE)
                return welcome;
            EQ_STRCPY(welcome, (char *)0x0054EE48);
            return welcome;
        }
        else if (start == 0xC)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 1)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x0054EB24);
                else
                    EQ_STRCPY(welcome, (char *)0x0054E7F8);
            }
            if (clsOneBased == 2)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x0054E500);
                else
                    EQ_STRCPY(welcome, (char *)0x0054E21C);
            }
            if (clsOneBased == 9)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x0054DF04);
                else
                    EQ_STRCPY(welcome, (char *)0x0054DC20);
            }
            if (clsOneBased == 0xE)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x0054D934);
                else
                    EQ_STRCPY(welcome, (char *)0x0054D6A8);
            }
            if (clsOneBased == 11)
                EQ_STRCPY(welcome, (char *)0x0054D3DC);
            if (clsOneBased == 0xC)
            {
                if (deity == 1)
                    EQ_STRCPY(welcome, (char *)0x0054D0E4);
                else
                    EQ_STRCPY(welcome, (char *)0x0054CE4C);
            }
            if (clsOneBased != 0xD)
                return welcome;
            if (deity == 1)
            {
                EQ_STRCPY(welcome, (char *)0x0054CB7C);
                return welcome;
            }
            else
            {
                EQ_STRCPY(welcome, (char *)0x0054C90C);
                return welcome;
            }
        }
        else if (start == 7)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 9)
            {
                if (side == 1)
                {
                    if (deity == 1)
                        EQ_STRCPY(welcome, (char *)0x0054C694);
                    else
                        EQ_STRCPY(welcome, (char *)0x0054C494);
                }
                else if (side == 4)
                    EQ_STRCPY(welcome, (char *)0x00554520);
                else
                    EQ_STRCPY(welcome, (char *)0x0054C250);
            }
            if (clsOneBased == 3)
            {
                if (deity == 5)
                    EQ_STRCPY(welcome, (char *)0x005564A4);
                if (deity == 6)
                    EQ_STRCPY(welcome, (char *)0x005566A4);
                if (deity == 7)
                    EQ_STRCPY(welcome, (char *)0x005568C8);
                if (deity == 8)
                    EQ_STRCPY(welcome, (char *)0x0055626C);
                if (deity == 0x0E)
                    EQ_STRCPY(welcome, (char *)0x0054BFE8);
            }
            if (clsOneBased == 8)
            {
                if (side == 1)
                    EQ_STRCPY(welcome, (char *)0x00554FB8);
                else if (side == 4)
                    EQ_STRCPY(welcome, (char *)0x00554D54);
                else
                    EQ_STRCPY(welcome, (char *)0x0054BDE8);
            }
            if (clsOneBased == 1)
            {
                if (side == 1)
                {
                    if (deity == 1)
                        EQ_STRCPY(welcome, (char *)0x0054BB2C);
                    else
                        EQ_STRCPY(welcome, (char *)0x00557A9C);
                }
                else if (side == 4)
                {
                    if (deity == 4)
                        EQ_STRCPY(welcome, (char *)0x0054B8C8);
                    else
                        EQ_STRCPY(welcome, (char *)0x00557884);
                }
                else
                    EQ_STRCPY(welcome, (char *)0x0054B740);
            }
            if (clsOneBased == 6)
            {
                if (side == 1)
                {
                    if (deity == 0x0E)
                        EQ_STRCPY(welcome, (char *)0x00555654);
                    else
                        EQ_STRCPY(welcome, (char *)0x00555890);
                }
                else
                    EQ_STRCPY(welcome, (char *)0x0054B5C4);
            }
            if (clsOneBased != 4)
                return welcome;
            if (side == 1)
                EQ_STRCPY(welcome, (char *)0x00556014);
            else
                EQ_STRCPY(welcome, (char *)0x0054B3D4);
            return welcome;
        }
        else if (start == 5)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 0xD)
                EQ_STRCPY(welcome, (char *)0x0054B0EC);
            if (clsOneBased == 0xE)
                EQ_STRCPY(welcome, (char *)0x0054ADE4);
            if (clsOneBased == 0xC)
                EQ_STRCPY(welcome, (char *)0x0054AAD4);
            if (clsOneBased == 2)
                EQ_STRCPY(welcome, (char *)0x0054A874);
            if (clsOneBased != 3)
                return welcome;
            EQ_STRCPY(welcome, (char *)0x0054BFE8);
            return welcome;
        }
        else if (start == 2)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 1)
            {
                EQ_STRCPY(welcome, (char *)0x0054A6B0);
                return welcome;
            }
            else if (clsOneBased == 10)
            {
                EQ_STRCPY(welcome, (char *)0x0054A444);
                return welcome;
            }
            else if (clsOneBased == 9)
            {
                EQ_STRCPY(welcome, (char *)0x0054A150);
                return welcome;
            }
            else
            {
                if (clsOneBased != 0xF)
                    return welcome;
                EQ_STRCPY(welcome, (char *)0x00549E14);
                return welcome;
            }
        }
        else if (start == 11)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 1)
                EQ_STRCPY(welcome, (char *)0x00549BE0);
            if (clsOneBased == 9)
                EQ_STRCPY(welcome, (char *)0x00549978);
            if (clsOneBased == 6)
                EQ_STRCPY(welcome, (char *)0x0054976C);
            if (clsOneBased != 2)
                return welcome;
            EQ_STRCPY(welcome, (char *)0x00549554);
            return welcome;
        }
        else if (start == 4)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 1)
            {
                EQ_STRCPY(welcome, (char *)0x0054B740);
                return welcome;
            }
            else if (clsOneBased == 8)
            {
                EQ_STRCPY(welcome, (char *)0x00549354);
                return welcome;
            }
            else if (clsOneBased == 4)
            {
                EQ_STRCPY(welcome, (char *)0x00549160);
                return welcome;
            }
            else if (clsOneBased == 6)
            {
                EQ_STRCPY(welcome, (char *)0x0054B5C4);
                return welcome;
            }
            else
            {
                if (clsOneBased != 9)
                    return welcome;
                EQ_STRCPY(welcome, (char *)0x00548F18);
                return welcome;
            }
        }
        else if (start == 10)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 1)
            {
                EQ_STRCPY(welcome, (char *)0x00548C4C);
                return welcome;
            }
            else if (clsOneBased == 10)
            {
                EQ_STRCPY(welcome, (char *)0x00548A0C);
                return welcome;
            }
            else if (clsOneBased == 5)
            {
                EQ_STRCPY(welcome, (char *)0x00548798);
                return welcome;
            }
            else
            {
                if (clsOneBased != 0xF)
                    return welcome;
                EQ_STRCPY(welcome, (char *)0x00548444);
                return welcome;
            }
        }
        else if (start == 9)
        {
            int clsOneBased = cls + 1;
            if (clsOneBased == 1)
            {
                EQ_STRCPY(welcome, (char *)0x00548200);
                return welcome;
            }
            else if (clsOneBased == 10)
            {
                EQ_STRCPY(welcome, (char *)0x00547FF0);
                return welcome;
            }
            else if (clsOneBased == 5)
            {
                EQ_STRCPY(welcome, (char *)0x00547D50);
                return welcome;
            }
            else
            {
                if (clsOneBased != 0xF)
                    return welcome;
                EQ_STRCPY(welcome, (char *)0x00547A20);
                return welcome;
            }
        }
        else if (start == 0x80)
        {
            int clsOneBased = cls + 1;
            switch (clsOneBased)
            {
            case 1:
                EQ_STRCPY(welcome, (char *)0x0054637C);
                return welcome;
            case 5:
                EQ_STRCPY(welcome, (char *)0x0054671C);
                return welcome;
            case 7:
                EQ_STRCPY(welcome, (char *)0x00546AC0);
                return welcome;
            case 10:
                EQ_STRCPY(welcome, (char *)0x00546E04);
                return welcome;
            case 11:
                EQ_STRCPY(welcome, (char *)0x00547210);
                return welcome;
            case 0xF:
                EQ_STRCPY(welcome, (char *)0x005475FC);
                return welcome;
            default:
                    return welcome;
            }
        }
        else
        {
            if (start != 0x82)
                return welcome;
            int clsOneBased = cls + 1;
            switch (clsOneBased)
            {
            case 1:
                EQ_STRCPY(welcome, (char *)0x005459A8);
                return welcome;
            case 8:
                EQ_STRCPY(welcome, (char *)0x00545B74);
                return welcome;
            case 9:
                EQ_STRCPY(welcome, (char *)0x00545DB0);
                return welcome;
            case 10:
                EQ_STRCPY(welcome, (char *)0x00545F90);
                return welcome;
            case 0x0F:
                EQ_STRCPY(welcome, (char *)0x00546138);
                return welcome;
            default:
                    return welcome;
            }
        }
        }
    return welcome;
}
