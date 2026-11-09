// EQ_TextIO.cpp - the chat window's shared display state and text buffer.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_TextIO.cpp (from Mac debug map)

#include <string.h>

#include "trilogy/eqaddr.h"
#include "trilogy/eq_overlay.h"
#include "trilogy/eq_textio.h"

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef EQTRACE_guess
// EXTERNAL: TRILOGY 0x004A3D01 EQTRACE_guess
void __cdecl EQTRACE_guess(const char* format, ...);
#undef LogMessage_guess
// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char* format, ...);
#define LogMessage_guess DbgLog_guess

/* Globals observed directly in the target's instructions. These remain raw address references until
 * the EQ_TextIO layout and display-buffer types are recovered. */
#define g_num_display_buffers_guess EQ_GLOBAL(int, 0x006B6508)
#define g_current_display_guess     EQ_GLOBAL(int, 0x006D6FDC)
#define g_display_data_guess        EQ_ARRAY(char, 0x006B772B)
#define g_display_offsets_guess     EQ_ARRAY(short, 0x006D6BF4)
#define g_display_current_data_guess EQ_GLOBAL(char *, 0x006D6B2C)
#define g_display_current_offset_guess EQ_GLOBAL(short *, 0x006D6B90)
#define g_text_buffer_guess         EQ_GLOBAL(char *, 0x006E20CC)
#define g_text_buffer_size_guess    EQ_GLOBAL(int, 0x006E20D0)
#define g_text_buffer_end_guess     EQ_GLOBAL(char *, 0x006E20D4)
#define g_text_buffer_current_guess EQ_GLOBAL(char *, 0x006E20E0)
#define g_text_buffer_read_guess    EQ_GLOBAL(char *, 0x006E20D8)
#define g_text_buffer_write_guess   EQ_GLOBAL(char *, 0x006E20DC)

typedef int (__stdcall *SetRect_guess_t)(int *, int, int, int, int);
typedef int (__stdcall *CopyRect_guess_t)(int *, int *);
#define g_CopyRect_guess EQ_GLOBAL(CopyRect_guess_t, 0x00534240)

typedef void *(__stdcall *CreateICA_guess_t)(const char *, const char *, const char *, void *);
typedef int (__stdcall *DeleteDC_guess_t)(void *);
typedef int (__stdcall *SetRectTextIO_guess_t)(int *, int, int, int, int);
#define g_CreateICA_guess EQ_GLOBAL(CreateICA_guess_t, 0x0053401C)
#define g_DeleteDC_guess  EQ_GLOBAL(DeleteDC_guess_t, 0x00534020)
#define g_SetRectTextIO_guess EQ_GLOBAL(SetRectTextIO_guess_t, 0x00534248)

typedef void *EQ_TextIO_ctor_layout_guess;
class EQ_TextIO
{
public:
    EQ_TextIO(int unused);
};

// Guards the base EQ_Overlay constructed by the constructor below: its destructor undoes that base when
// overlay is set. At file scope so VC6 expands the destructor inline (a local class's is left as a call).
struct EQ_TextIOBaseCleanup_guess
{
    void *overlay;

    ~EQ_TextIOBaseCleanup_guess()
    {
        if (overlay != 0)
            EQ_METHOD(0, void, 0x0045005D, (void *), overlay)();
    }
};

// FUNCTION: TRILOGY 0x0048a278
// Sets up the chat window's default overlay, background, and shared display fonts.
// ORIGINAL FILE: EverQuest/EQ_TextIO.cpp (from Mac debug map)
// @early-stop: EH 'this' store sits after the base call and push edi precedes mov esi,ecx; xor edx before load_guess; push 0x6f order
EQ_TextIO::EQ_TextIO(int unused)
{
    EQ_METHOD(0, void, 0x00450049, (void *), this)();
    EQ_TextIOBaseCleanup_guess cleanup;
    cleanup.overlay = this;
    *(short*)((char *)this + 0x1e8) = -1;
    *(short*)((char *)this + 0x1e2) = -1;
    *(void **)this = (void *)0x00535660;
    *(int *)((char *)this + 0x10) = 0;
    *(int *)((char *)this + 0x200) = 0;
    *(int *)((char *)this + 0x204) = 0;
    *(unsigned char *)((char *)this + 0x1f0) = 1;
    *(unsigned short *)((char *)this + 0x1e6) = 0;
    *(unsigned char *)((char *)this + 0x1f1) = 0;
    *(unsigned short *)((char *)this + 0x80) = 0x9b;
    *(unsigned short *)((char *)this + 0x82) = 0x1ab;
    *(unsigned short *)((char *)this + 0x84) = 0x9b;
    *(unsigned short *)((char *)this + 0x86) = 0x1c2;
    *(int *)((char *)this + 0x88) = 0;
    *(int *)((char *)this + 0x16c) = 0;
    *(int *)((char *)this + 0x170) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(int *)((char *)this + 0x1f8) = 0;
    *(int *)((char *)this + 0x78) = 0;
    strcpy((char *)this + 0x34, (const char *)0x0053e950);
    strcpy((char *)this + 0x52, (const char *)0x0055d6c4);
    SetRectTextIO_guess_t setRect = g_SetRectTextIO_guess;
    setRect((int *)((char *)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((int *)((char *)this + 0x14), 0, 0, 0x280, 0x1e0);
    EQ_METHOD(2, void, 0x00450168, (void *, int, int), this)(1, 0);
    EQ_METHOD(0, void, 0x0048A6C4, (void *), this)();
    setRect((int *)((char *)this + 0x24), 0x8c, 0x147, 0x208, 0x1dc);
    *(short*)((char *)this + 0x1e4) = -1;
    *(int *)((char *)this + 0x1ec) = 1;
    *(unsigned short *)((char *)this + 0x1e0) = 0;
    setRect((int *)((char *)this + 0x8c), 0x1fd, 0x15e, 0x207, 0x1c5);
    setRect((int *)((char *)this + 0x9c), 0x6f, 0x1b7, 0x1fb, 0x1df);
    void *hdc = g_CreateICA_guess((const char *)0x0055d6bc, 0, 0, 0);
    if (hdc == 0)
        DbgLog_guess( (const char *)0x0055d678 );
    if (EQ_GLOBAL(int, 0x006EFDF4) == 0)
    {
        EQ_GLOBAL(int, 0x006EFDF4) = EQ_CDECL(int, 0x004EAF53, (int, int, const char *))
            (10, 100, (const char *)0x0053da68);
        EQ_GLOBAL(int, 0x006EFDD8) = EQ_CDECL(int, 0x004EAFD1, (void *, int, int))
            (hdc, EQ_GLOBAL(int, 0x006EFDF4), 10);
    }
    if (EQ_GLOBAL(int, 0x006EFDF8) == 0)
    {
        EQ_GLOBAL(int, 0x006EFDF8) = EQ_CDECL(int, 0x004EAF53, (int, int, const char *))
            (12, 100, (const char *)0x0053da68);
        EQ_GLOBAL(int, 0x006EFDDC) = EQ_CDECL(int, 0x004EAFD1, (void *, int, int))
            (hdc, EQ_GLOBAL(int, 0x006EFDF8), 12);
    }
    if (EQ_GLOBAL(int, 0x006EFDFC) == 0)
    {
        EQ_GLOBAL(int, 0x006EFDFC) = EQ_CDECL(int, 0x004EAF53, (int, int, const char *))
            (14, 100, (const char *)0x0053da68);
        EQ_GLOBAL(int, 0x006EFDE0) = EQ_CDECL(int, 0x004EAFD1, (void *, int, int))
            (hdc, EQ_GLOBAL(int, 0x006EFDFC), 14);
    }
    if (EQ_GLOBAL(int, 0x006EFE00) == 0)
    {
        EQ_GLOBAL(int, 0x006EFE00) = EQ_CDECL(int, 0x004EAF53, (int, int, const char *))
            (15, 100, (const char *)0x0053da68);
        EQ_GLOBAL(int, 0x006EFDE4) = EQ_CDECL(int, 0x004EAFD1, (void *, int, int))
            (hdc, EQ_GLOBAL(int, 0x006EFE00), 15);
    }
    if (EQ_GLOBAL(int, 0x006EFE04) == 0)
    {
        EQ_GLOBAL(int, 0x006EFE04) = EQ_CDECL(int, 0x004EAF53, (int, int, const char *))
            (16, 100, (const char *)0x0053da68);
        EQ_GLOBAL(int, 0x006EFDE8) = EQ_CDECL(int, 0x004EAFD1, (void *, int, int))
            (hdc, EQ_GLOBAL(int, 0x006EFE04), 16);
    }
    if (EQ_GLOBAL(int, 0x006EFE08) == 0)
    {
        EQ_GLOBAL(int, 0x006EFE08) = EQ_CDECL(int, 0x004EAF53, (int, int, const char *))
            (20, 100, (const char *)0x0053da68);
        EQ_GLOBAL(int, 0x006EFDEC) = EQ_CDECL(int, 0x004EAFD1, (void *, int, int))
            (hdc, EQ_GLOBAL(int, 0x006EFE08), 20);
    }
    if (EQ_GLOBAL(int, 0x006EFE0C) == 0)
    {
        EQ_GLOBAL(int, 0x006EFE0C) = EQ_CDECL(int, 0x004EAF53, (int, int, const char *))
            (24, 700, (const char *)0x0053da68);
        EQ_GLOBAL(int, 0x006EFDF0) = EQ_CDECL(int, 0x004EAFD1, (void *, int, int))
            (hdc, EQ_GLOBAL(int, 0x006EFE0C), 24);
    }
    g_DeleteDC_guess(hdc);
    EQ_GLOBAL(int, 0x006B70E4) = 1;
    cleanup.overlay = 0;
}

// Sets the vtable on construction; destroys the base EQ_Overlay on every exit.
struct EQ_TextIODtorBaseCleanup_guess
{
    void *obj_guess;

    EQ_TextIODtorBaseCleanup_guess(void *obj) : obj_guess(obj)
    {
        *(unsigned int *)obj = 0x00535660;
    }
    ~EQ_TextIODtorBaseCleanup_guess()
    {
        /* fake match: the pointer local makes VC6 set the EH state to -1 before the base call. */
        void (__fastcall *destroy_guess)(void *) = EQ_FUNC(void, __fastcall, 0x0045005D, (void *));
        destroy_guess(obj_guess);
    }
};

/* fake match: a pointer-local call while the guard lives makes VC6 keep the EH frame; constant-address calls don't. */
// FUNCTION: TRILOGY 0x0048a539
// Releases the chat overlay buttons and shared font resources.
void __fastcall ReleaseChatResources_guess(EQ_TextIO_guess *self)
{
    EQ_TextIODtorBaseCleanup_guess baseCleanup(self);
    /* fake match: this pointer-local call is what frames the body. */
    void (__fastcall *delButtons_guess)(void *) = EQ_FUNC(void, __fastcall, 0x00450463, (void *));
    delButtons_guess(self);

    if (EQ_GLOBAL(void *, 0x006EFDD8) != 0) {
        if (EQ_GLOBAL(void *, 0x006EFDF4) != 0)
            EQ_CDECL(void, 0x004EAFBF, (void *))(EQ_GLOBAL(void *, 0x006EFDF4));
        EQ_GLOBAL(void *, 0x006EFDF4) = 0;
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(void *, 0x006EFDD8));
        EQ_GLOBAL(void *, 0x006EFDD8) = 0;
    }
    if (EQ_GLOBAL(void *, 0x006EFDDC) != 0) {
        if (EQ_GLOBAL(void *, 0x006EFDF8) != 0)
            EQ_CDECL(void, 0x004EAFBF, (void *))(EQ_GLOBAL(void *, 0x006EFDF8));
        EQ_GLOBAL(void *, 0x006EFDF8) = 0;
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(void *, 0x006EFDDC));
        EQ_GLOBAL(void *, 0x006EFDDC) = 0;
    }
    if (EQ_GLOBAL(void *, 0x006EFDE0) != 0) {
        if (EQ_GLOBAL(void *, 0x006EFDFC) != 0)
            EQ_CDECL(void, 0x004EAFBF, (void *))(EQ_GLOBAL(void *, 0x006EFDFC));
        EQ_GLOBAL(void *, 0x006EFDFC) = 0;
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(void *, 0x006EFDE0));
        EQ_GLOBAL(void *, 0x006EFDE0) = 0;
    }
    if (EQ_GLOBAL(void *, 0x006EFDE4) != 0) {
        if (EQ_GLOBAL(void *, 0x006EFE00) != 0)
            EQ_CDECL(void, 0x004EAFBF, (void *))(EQ_GLOBAL(void *, 0x006EFE00));
        EQ_GLOBAL(void *, 0x006EFE00) = 0;
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(void *, 0x006EFDE4));
        EQ_GLOBAL(void *, 0x006EFDE4) = 0;
    }
    if (EQ_GLOBAL(void *, 0x006EFDE8) != 0) {
        if (EQ_GLOBAL(void *, 0x006EFE04) != 0)
            EQ_CDECL(void, 0x004EAFBF, (void *))(EQ_GLOBAL(void *, 0x006EFE04));
        EQ_GLOBAL(void *, 0x006EFE04) = 0;
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(void *, 0x006EFDE8));
        EQ_GLOBAL(void *, 0x006EFDE8) = 0;
    }
    if (EQ_GLOBAL(void *, 0x006EFDEC) != 0) {
        if (EQ_GLOBAL(void *, 0x006EFE08) != 0)
            EQ_CDECL(void, 0x004EAFBF, (void *))(EQ_GLOBAL(void *, 0x006EFE08));
        EQ_GLOBAL(void *, 0x006EFE08) = 0;
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(void *, 0x006EFDEC));
        EQ_GLOBAL(void *, 0x006EFDEC) = 0;
    }
    if (EQ_GLOBAL(void *, 0x006EFDF0) != 0) {
        if (EQ_GLOBAL(void *, 0x006EFE0C) != 0)
            EQ_CDECL(void, 0x004EAFBF, (void *))(EQ_GLOBAL(void *, 0x006EFE0C));
        EQ_GLOBAL(void *, 0x006EFE0C) = 0;
        EQ_CDECL(void, 0x005230DD, (void *))(EQ_GLOBAL(void *, 0x006EFDF0));
        EQ_GLOBAL(void *, 0x006EFDF0) = 0;
    }
}

// FUNCTION: TRILOGY 0x0048a6c4
// Registers source regions for the chat window's controls and text panels.
// ORIGINAL FILE: EverQuest/EQ_TextIO.cpp (from Mac debug map)
void __fastcall DefineImages_guess(EQ_TextIO_guess *self)
{
    int rect[4];

    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x1d, 0x28, 0x45, 0x50);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x68, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x45, 0x28, 0x6d, 0x50);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x69, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x1d, 0, 0x45, 0x28);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x7a, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x45, 0, 0x6d, 0x28);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x7b, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x6d, 0, 0x95, 0x28);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x9a, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x95, 0, 0xbd, 0x28);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x9b, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x1d, 0x78, 0x45, 0xa0);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x9c, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x45, 0x78, 0x6d, 0xa0);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x9d, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x6d, 0x78, 0x95, 0xa0);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x9e, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x95, 0x78, 0xbd, 0xa0);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x9f, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x8d, 0xe5, 0xb5, 0x10d);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0xa0, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xb5, 0xe5, 0xdd, 0x10d);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0xa1, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0, 0x10d, 0x18c, 0x135);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0xce, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0, 0x136, 0x18c, 0x15e);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0xcf, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0, 0x15f, 0x18c, 0x187);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0xd0, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x10d, 0, 0x20a, 0x41);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x13f, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x10d, 0x42, 0x23b, 0x83);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x140, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x10d, 0x84, 0x23b, 0xc5);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x141, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x10d, 0xc6, 0x1d9, 0x107);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x142, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0xab, 0x10d, 0xbd);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x143, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x1e4, 199, 0x24b, 0xd5);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x144, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0, 0, 0xdd, 0xa1);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x16e, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x4e, 0x1b, 0x91, 0x2e);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x16f, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x4e, 0x2f, 0x91, 0x42);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x170, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x4e, 0x43, 0x91, 0x56);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x171, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x4e, 0x57, 0x91, 0x6a);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x172, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x93, 0x1b, 0xd6, 0x2e);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x173, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x93, 0x2f, 0xd6, 0x42);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x174, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0, 0x10d, 0x12);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x175, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0x39, 0x10d, 0x4b);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x176, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0x4c, 0x10d, 0x5e);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x177, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0x5f, 0x10d, 0x71);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x178, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0x72, 0x10d, 0x84);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x179, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0x85, 0x10d, 0x97);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x17a, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0x98, 0x10d, 0xaa);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x17b, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0xde, 0xbe, 0x10d, 0xd0);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x17c, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0, 0xaf, 0x3a, 0xd5);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x1c2, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x3b, 0xa2, 0x75, 0xd5);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x1c3, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0x76, 0xa2, 0xb0, 0xd5);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x1c4, *(unsigned int *)((char *)self + 4), rect);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(rect, 0, 0xd5, 0x3a, 0xf8);
    EQ_METHOD(3, void, 0x004502A1, (EQ_TextIO_guess *, int, unsigned int, int *), self)(0x1c5, *(unsigned int *)((char *)self + 4), rect);
}

// FUNCTION: TRILOGY 0x0048acad
// Lays out the chat controls and positions the text scroll region for the active display mode.
// @early-stop: EBP frame (SetRect pointer spilled to a slot), dummy EDX on __fastcall calls, mode-2 DAT reloads, SetRect arg order
void __fastcall SetTextIOLayout_guess(EQ_TextIO_guess *self)
{
    int i;
    int xLeft;
    int top;
    int width;
    int right;
    int bottom;

    if (EQ_GLOBAL(unsigned char, 0x006EFC70) != 1)
    {
        SetRect_guess_t setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);

        EQ_THISCALL(void, 0x00450463, (EQ_TextIO_guess *, int))(self, 0);
        *(int *)((char *)self + 0x10) = 1;
        *(int *)((char *)self + 0x70) = 0;
        g_num_display_buffers_guess = (((EQ_GLOBAL(int, 0x0058FC6C) != 2) - 1) & 0xe) + 0xb;
        self->languageCount_guess = 0;
        if (EQ_GLOBAL(unsigned char *, 0x006E4DA8) != 0)
        {
            for (i = 0x10; i != 0; i--)
                *(unsigned int *)((char *)self + 0x17c + (0x10 - i) * 4) = 0xffffffff;
            i = 0;
            do
            {
                if (EQ_GLOBAL(unsigned char *, 0x006E4DA8)[i + 0x7e] != 0)
                {
                    *(short *)((char *)self + 0x17c + self->languageCount_guess * 2) = (short)i;
                    self->languageCount_guess = self->languageCount_guess + 1;
                }
                i++;
            } while (i < 0x20);
        }

        if (self->channelPickerOpen != 0)
        {
            setRect((int *)((char *)self + 0xac), 0xef, 0x164, 0x18b, 0x176);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 0, (int *)((char *)self + 0xac), -1, -1, 0, 0);
            setRect((int *)((char *)self + 0xbc), 0xb7, 0x192, 0xe6, 0x19c);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 1, (int *)((char *)self + 0xbc), -1, -1, 0, 0);
            setRect((int *)((char *)self + 0xcc), 0xb7, 0x1a0, 0xe6, 0x1aa);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 2, (int *)((char *)self + 0xcc), -1, -1, 0, 0);
            setRect((int *)((char *)self + 0xdc), 0xb7, 0x1ae, 0xe6, 0x1b8);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 3, (int *)((char *)self + 0xdc), -1, -1, 0, 0);
            setRect((int *)((char *)self + 0xec), 0x14f, 0x192, 0x1c7, 0x19c);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 4, (int *)((char *)self + 0xec), -1, -1, 0, 0);
            setRect((int *)((char *)self + 0xfc), 0x14f, 0x1a0, 0x1c7, 0x1aa);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 5, (int *)((char *)self + 0xfc), -1, -1, 0, 0);
            setRect((int *)((char *)self + 0x10c), 0x14f, 0x1ae, 0x1c7, 0x1b8);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 6, (int *)((char *)self + 0x10c), -1, -1, 0, 0);
            setRect((int *)((char *)self + 0x11c), 0xea, 0x1ac, 0x149, 0x1b8);
            self->languageCount_guess = 0;
            for (i = 0x10; i != 0; i--)
                *(unsigned int *)((char *)self + 0x17c + (0x10 - i) * 4) = 0xffffffff;
            i = 0;
            do
            {
                if (EQ_GLOBAL(unsigned char *, 0x006E4DA8)[i + 0x7e] != 0)
                {
                    *(short *)((char *)self + 0x17c + self->languageCount_guess * 2) = (short)i;
                    self->languageCount_guess = self->languageCount_guess + 1;
                }
                i++;
            } while (i < 0x20);
        }
        else
        {
            short pageHeight;
            short scrollHeight;
            short scrollPosition;
            short pageCount;

            if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
            {
                *(unsigned int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900f4) = 0;
                xLeft = *(int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900e4);
                *(unsigned int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900f4) = 0;
                top = *(int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900e8);
                *(unsigned int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900f4) = 0;
                width = *(int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900ec);
                *(unsigned int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900f4) = 0;
                bottom = *(int *)(EQ_GLOBAL(char *, 0x006E4E44) + 0x900f0);
                right = width + xLeft;
                setRect((int *)((char *)self + 0xac), right - 0xd, top, right, top + 0x13);
                bottom = bottom + top;
                setRect((int *)((char *)self + 0xbc), right - 0xd, bottom - 0x1b, right, bottom - 8);
                setRect((int *)((char *)self + 0xcc), right - 0xc, bottom - 0x27, right - 2, bottom - 0x1c);
                setRect((int *)((char *)self + 0xdc), xLeft, top, xLeft + 8, top + 8);
                EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                    (self, 0, 3, (int *)((char *)self + 0xdc), -1, -1, 0, 0);
                setRect((int *)((char *)self + 0xec), right - 8, bottom - 8, right, bottom);
                EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                    (self, 0, 4, (int *)((char *)self + 0xec), -1, -1, 0, 0);
            }
            else
            {
                setRect((int *)((char *)self + 0xac), 0x1fd, 0x14d, 0x207, 0x15d);
                setRect((int *)((char *)self + 0xbc), 0x1fd, 0x1c6, 0x207, 0x1d7);
                setRect((int *)((char *)self + 0xcc), 0x1fd, 0x1ba, 0x207, 0x1c5);
            }

            *(short *)((char *)self + 0x174) = *(short *)((char *)self + 0xd8) - *(short *)((char *)self + 0xd0);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 0, (int *)((char *)self + 0xac), -1, -1, 0, 0);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 1, (int *)((char *)self + 0xbc), -1, -1, 0, 0);
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 2, (int *)((char *)self + 0xcc), -1, -1, 0, 0);
            pageHeight = *(short *)((char *)self + 0xb8) + 1;
            self->scrollTopLimit = pageHeight;
            *(int *)((char *)self + 0x88) = 0;
            scrollHeight = (*(short *)((char *)self + 0xc0) - *(short *)((char *)self + 0x174)) - 1;
            self->scrollBottomLimit = scrollHeight;
            scrollPosition = scrollHeight - pageHeight;
            i = *(int *)((char *)self + 0x70);
            self->scrollTravel = scrollPosition;
            for (; i != 0; i = *(int *)(i + 0x20))
            {
                if (*(short *)(i + 0x14) == 2)
                    *(int *)((char *)self + 0x88) = i + 4;
            }
            if (self->firstLayoutPending_guess != 0)
                self->firstLayoutPending_guess = 0;
            pageCount = (short)(EQ_GLOBAL(int, 0x006E0FEA) - 1);
            if (pageCount > 0)
                scrollPosition = (short)((scrollPosition * g_current_display_guess) / (int)pageCount);
            else
                scrollPosition = 0;
            *(int *)(*(int *)((char *)self + 0x88) + 4) = (int)scrollHeight - (int)scrollPosition;
            *(int *)(*(int *)((char *)self + 0x88) + 0xc) =
                *(int *)(*(int *)((char *)self + 0x88) + 4) + (int)self->scrollThumbHeight_guess;
            g_CopyRect_guess((int *)((char *)self + 0xcc), *(int **)((char *)self + 0x88));
        }

        setRect((int *)((char *)self + 0x14c), 0x6e, 0x140, 0x20a, 0x1e0);
        if (EQ_GLOBAL(int, 0x0058FC6C) == 1)
        {
            EQ_THISCALL(void, 0x004503BE, (EQ_TextIO_guess *, int, int, int *, int, int, int, int))
                (self, 0, 10, (int *)((char *)self + 0x14c), -1, -1, 0, 0);
        }
        self->redrawPass_guess = 1;
    }
}

// FUNCTION: TRILOGY 0x0048b1d1
// Clears two state dwords around the overlay button cleanup call.
void EQ_TextIO_guess::ResetTextIO_guess(void)
{
    *(unsigned int *)((char *)this + 0x10) &= 0;
    EQ_METHOD(0, void, 0x00450463, (EQ_TextIO_guess *), this)();
    *(unsigned int *)((char *)this + 0x70) &= 0;
}

class EQ_TextIO_guess_guess
{
public:
    void MouseMove_guess(short param_2, short param_3);
};
typedef void (__fastcall *EQ_TextIO_vtable_method_guess_t)(EQ_TextIO_guess_guess *);

// FUNCTION: TRILOGY 0x0048b1e3
// Chat window mouse drag: moves or resizes the window (UI mode 2), else drags the scroll thumb to scroll text.
// ORIGINAL FILE: EverQuest/EQ_TextIO.cpp (from Mac debug map)
// @early-stop: frame/register allocation and main state-access scheduling still differ from the original.
void EQ_TextIO_guess_guess::MouseMove_guess(short param_2, short param_3)
{
    int tmp;
    int iVar2;
    int *ptr;
    int value;

    if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
    {
        int temp4;

        temp4 = 4;
        if (EQ_GLOBAL(int, 0x006B63FC) == 800)
            temp4 = 2;
        else if (EQ_GLOBAL(int, 0x006B63FC) == 0x400)
            temp4 = 3;
        else if (EQ_GLOBAL(int, 0x006B63FC) != 0x500)
            temp4 = ((EQ_GLOBAL(int, 0x006B63FC) != 0x640) - 1 & 4) + 1;

        ptr = (int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900E4);
        iVar2 = ptr[6];
        ptr[4] = 0;
        value = EQ_GLOBAL(int, 0x006E4E44);
        if (iVar2 != 0)
        {
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            tmp = EQ_GLOBAL(int, 0x006E4E44);
            if (*(int *)(tmp + 0x900EC) + (int)param_2 > EQ_GLOBAL(int, 0x006B63FC))
            {
                *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
                param_2 = EQ_GLOBAL(int, 0x006B63FC) - *(int *)(tmp + 0x900EC);
            }

            value = EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            tmp = EQ_GLOBAL(int, 0x006E4E44);
            if (*(int *)(value + 0x900F0) + (int)param_3 > EQ_GLOBAL(int, 0x006B6400))
            {
                *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
                param_3 = EQ_GLOBAL(int, 0x006B6400) - *(int *)(tmp + 0x900F0);
            }

            value = EQ_GLOBAL(int, 0x006E4E44);
            if (param_2 < 4)
                param_2 = 4;
            if (param_3 < 6)
                param_3 = 6;

            ptr = (int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900E4);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 1;
            *ptr = param_2 - 3;
            *(int *)(value + 0x900E8) = param_3 - 5;
            ptr = (int *)(temp4 * 0x20 + 0x90644 + EQ_GLOBAL(int, 0x006E4E44));
            ptr[4] = 1;
            *ptr = param_2 - 3;
            ptr[1] = param_3 - 5;

            ((EQ_TextIO_vtable_method_guess_t)(*(int *)(*(int *)this + 8)))(this);
            ((EQ_TextIO_vtable_method_guess_t)(*(int *)(*(int *)this + 4)))(this);
            return;
        }

        ptr = (int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x90100);
        *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
        if (*ptr != 0)
        {
            value = EQ_GLOBAL(int, 0x006E4E44);

            if ((int)param_2 >= EQ_GLOBAL(int, 0x006B63FC) - 5)
                param_2 = (short)EQ_GLOBAL(int, 0x006B63FC) - 5;
            if ((int)param_3 >= EQ_GLOBAL(int, 0x006B6400) - 5)
                param_3 = (short)EQ_GLOBAL(int, 0x006B6400) - 5;

            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            tmp = EQ_GLOBAL(int, 0x006E4E44);
            if ((int)param_2 < *(int *)(value + 0x900E4) + 100)
            {
                *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
                param_2 = (short)*(int *)(tmp + 0x900E4) + 100;
            }

            value = EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            tmp = EQ_GLOBAL(int, 0x006E4E44);
            if ((int)param_3 < *(int *)(value + 0x900E8) + 0x3C)
            {
                *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
                param_3 = (short)*(int *)(tmp + 0x900E8) + 0x3C;
            }

            value = EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            tmp = EQ_GLOBAL(int, 0x006E4E44);
            if ((int)param_2 > *(int *)(value + 0x900E4) + 0x2A8)
            {
                *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
                param_2 = (short)*(int *)(tmp + 0x900E4) + 0x2A8;
            }

            value = EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            tmp = EQ_GLOBAL(int, 0x006E4E44);
            if ((int)param_3 > *(int *)(value + 0x900E8) + 300)
            {
                *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
                param_3 = (short)*(int *)(tmp + 0x900E8) + 300;
            }

            value = EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            value = *(int *)(value + 0x900E8);
            ptr = (int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900E4);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            iVar2 = EQ_GLOBAL(int, 0x006E4E44);
            tmp = *ptr;
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 1;
            *(int *)(iVar2 + 0x900EC) = (param_2 - tmp) + 4;
            *(int *)(iVar2 + 0x900F0) = (param_3 - value) + 4;

            value = EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            iVar2 = EQ_GLOBAL(int, 0x006E4E44);
            tmp = *(int *)(value + 0x900E8);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            value = EQ_GLOBAL(int, 0x006E4E44);
            iVar2 = *(int *)(iVar2 + 0x900E4);
            *(int *)(temp4 * 0x20 + 0x90654 + EQ_GLOBAL(int, 0x006E4E44)) = 1;
            value = temp4 * 0x20 + 0x90644 + value;
            *(int *)(value + 8) = (param_2 - iVar2) + 4;
            *(int *)(value + 0xC) = (param_3 - tmp) + 4;

            temp4 = EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0x900F4) = 0;
            EQ_GLOBAL(int, 0x0057D3D0) = *(int *)(temp4 + 0x900EC) - 0x28;
            if (0x280 < EQ_GLOBAL(int, 0x0057D3D0))
                EQ_GLOBAL(int, 0x0057D3D0) = 0x280;

            ((EQ_TextIO_vtable_method_guess_t)(*(int *)(*(int *)this + 8)))(this);
            ((EQ_TextIO_vtable_method_guess_t)(*(int *)(*(int *)this + 4)))(this);
            return;
        }
    }

    if (*(int *)((char *)this + 0x200) != 0 && *(char *)((char *)this + 0x1F1) == '\0')
    {
        if (*(short *)((char *)this + 0x178) < param_3)
            param_3 = *(short *)((char *)this + 0x178);
        if (param_3 < *(short *)((char *)this + 0x176))
            param_3 = *(short *)((char *)this + 0x176);

        int oldScrollPosition = g_current_display_guess;
        *(int *)(*(int *)((char *)this + 0x88) + 4) = (int)param_3;
        *(int *)(*(int *)((char *)this + 0x88) + 0xC) =
            (int)*(short *)((char *)this + 0x174) + (int)param_3;
        g_CopyRect_guess((int *)((char *)this + 0xCC), (int *)*(int *)((char *)this + 0x88));

        g_current_display_guess =
            (int)(short)((*(short *)((char *)this + 0x17A) - param_3) +
                         *(short *)((char *)this + 0x176)) *
            (int)EQ_GLOBAL(short, 0x006E0FEA) /
            (int)*(short *)((char *)this + 0x17A);
        value = (int)EQ_GLOBAL(short, 0x006E0FEA) - 1;
        if (value < g_current_display_guess)
            g_current_display_guess = value;
        if (g_current_display_guess < 0)
            g_current_display_guess = 0;

        if (g_current_display_guess != oldScrollPosition)
        {
            EQ_METHOD(0, void, 0x0048CA78, (EQ_TextIO_guess_guess *), this)();
            EQ_METHOD(1, void, 0x0048B9AC, (EQ_TextIO_guess_guess *, int), this)(0);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0048b5e7
// Chat window clicks: move/resize toggles (saves ChatWinX/Y/W/H), language/channel picker, scroll buttons.
// @early-stop: profile-state address folding and stack-argument EDX shims still differ
/* fake match: retain both window-base expressions so VC6 emits separate field stores. */
void __fastcall EQ_TextIO_HandleTextIOCommand_guess(EQ_TextIO_guess *textIO, int edx, short action, short controlId)
{
    char *iVar2;
    int obj;
    char section[16];

    if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
    {
        iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
        iVar2 += 0x900E4;
        if (*(int *)(iVar2 + 0x14) == 0)
        {
            *(unsigned int *)(iVar2 + 0x10) = 0;
            return;
        }
        *(unsigned int *)(iVar2 + 0x10) = 0;
    }

    if (textIO->channelPickerOpen != '\0')
    {
        if (textIO->mouseDispatchActive_guess != 0)
            return;
    }
    else if (textIO->mouseDispatchActive_guess != 0)
    {
        textIO->pressedControlId_guess = controlId;
        if (action != 0)
            return;
        if (controlId != 2)
            return;
        textIO->scrollThumbDragging_guess = 1;
        return;
    }

    if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
    {
        if (EQ_GLOBAL(int, 0x006B63FC) == 800)
            strcpy(section, EQ_ARRAY(char, 0x0055A7D8));
        else if (EQ_GLOBAL(int, 0x006B63FC) == 0x400)
            strcpy(section, EQ_ARRAY(char, 0x0055A7CC));
        else if (EQ_GLOBAL(int, 0x006B63FC) == 0x500)
            strcpy(section, EQ_ARRAY(char, 0x0055A7C0));
        else if (EQ_GLOBAL(int, 0x006B63FC) == 0x640)
            strcpy(section, EQ_ARRAY(char, 0x0055A7B4));
        else
            strcpy(section, EQ_ARRAY(char, 0x0055A944));
        if (EQ_GLOBAL(int, 0x006E2094) != 0)
        {
            iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(iVar2 + 0x900F4) = 1;
            iVar2 += 0x900E4;
            *(unsigned int *)(iVar2 + 0x18) = 0;
            obj = EQ_GLOBAL(int, 0x006E4E44);
            obj += 0x900E4;
            *(unsigned int *)(iVar2 + 0x10) = 1;
            *(unsigned int *)(obj + 0x1C) = 0;
        }
        else if (controlId == 3)
        {
            iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(iVar2 + 0x900F4) = 0;
            iVar2 += 0x900E4;
            if (*(int *)(iVar2 + 0x18) != 0)
            {
                obj = EQ_GLOBAL(int, 0x006E4E44);
                obj += 0x900E4;
                *(unsigned int *)(obj + 0x10) = 1;
                *(unsigned int *)(obj + 0x18) = 0;
                iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
                *(unsigned int *)(iVar2 + 0x900F4) = 0;
                iVar2 += 0x900E4;
                obj = EQ_GLOBAL(int, 0x006E4E44);
                EQ_METHOD(3, void, 0x0045056D, (void *, const char *, const char *, int), (void *)obj)(section,
                     EQ_ARRAY(char, 0x0055A82C), *(int *)iVar2);
                iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
                *(unsigned int *)(iVar2 + 0x900F4) = 0;
                iVar2 += 0x900E4;
                obj = EQ_GLOBAL(int, 0x006E4E44);
                EQ_METHOD(3, void, 0x0045056D, (void *, const char *, const char *, int), (void *)obj)(section,
                     EQ_ARRAY(char, 0x0055A838), *(int *)(iVar2 + 4));
            }
            else
            {
                obj = EQ_GLOBAL(int, 0x006E4E44);
                obj += 0x900E4;
                *(unsigned int *)(obj + 0x10) = 1;
                *(unsigned int *)(obj + 0x18) = 1;
            }
        }
        else if (controlId == 4)
        {
            iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
            *(unsigned int *)(iVar2 + 0x900F4) = 0;
            iVar2 += 0x900E4;
            if (*(int *)(iVar2 + 0x1C) != 0)
            {
                iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
                iVar2 += 0x900E4;
                *(unsigned int *)(iVar2 + 0x10) = 1;
                *(unsigned int *)(iVar2 + 0x1C) = 0;
                EQ_METHOD(0, void, 0x0048CF56, (int *), textIO)();
                iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
                *(unsigned int *)(iVar2 + 0x900F4) = 0;
                iVar2 += 0x900E4;
                obj = EQ_GLOBAL(int, 0x006E4E44);
                EQ_METHOD(3, void, 0x0045056D, (void *, const char *, const char *, int), (void *)obj)(section,
                     EQ_ARRAY(char, 0x0055A814), *(int *)(iVar2 + 8));
                iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
                *(unsigned int *)(iVar2 + 0x900F4) = 0;
                iVar2 += 0x900E4;
                obj = EQ_GLOBAL(int, 0x006E4E44);
                EQ_METHOD(3, void, 0x0045056D, (void *, const char *, const char *, int), (void *)obj)(section,
                     EQ_ARRAY(char, 0x0055A820), *(int *)(iVar2 + 0xC));
            }
            else
            {
                iVar2 = (char *)EQ_GLOBAL(int, 0x006E4E44);
                iVar2 += 0x900E4;
                *(unsigned int *)(iVar2 + 0x10) = 1;
                *(unsigned int *)(iVar2 + 0x1C) = 1;
            }
        }
    }

    if (((EQ_GLOBAL(int, 0x006E4E44) != 0) &&
         (EQ_GLOBAL(int, 0x006B6590) == 0)) &&
        (pinstEQ_Main_guess->pendingServerReplies_guess == 0) &&
        ((pinstEQ_Main_guess->pendingWorldRequests_guess == 0) &&
         ((action == 1) && ((controlId != 0) && (controlId != 1)))))
    {
        if (textIO->channelPickerOpen != '\0')
        {
            if (EQ_GLOBAL(int, 0x00590170) == 2)
            {
                EQ_GLOBAL(int, 0x0058FC6C) = 2;
                *(unsigned int *)(EQ_GLOBAL(int, 0x006E4E44) + 0xEB8) = 1;
                EQ_METHOD(1, void, 0x0040E58B, (void *, int), EQ_GLOBAL(void *, 0x006E4DE0))(0);
            }
            textIO->channelPickerOpen = 0;
        }
        else
        {
            if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
            {
                EQ_GLOBAL(int, 0x00590170) = 2;
                EQ_GLOBAL(int, 0x0058FC6C) = 1;
                EQ_METHOD(1, void, 0x0040E58B, (void *, int), EQ_GLOBAL(void *, 0x006E4DE0))(0);
            }
            textIO->channelPickerOpen = 1;
        }
        ((EQ_TextIO_vtable_method_guess_t)(*(int *)(*(int *)textIO + 8)))
            ((EQ_TextIO_guess_guess *)textIO);
        ((EQ_TextIO_vtable_method_guess_t)(*(int *)(*(int *)textIO + 4)))
            ((EQ_TextIO_guess_guess *)textIO);
    }

    if (textIO->channelPickerOpen != '\0')
    {
        if (action == 0)
        {
            switch (controlId)
            {
            case 0:
                if (EQ_GLOBAL(short, 0x006B642A) < 0x1F)
                    EQ_GLOBAL(short, 0x006B642A) = EQ_GLOBAL(short, 0x006B642A) + 1;
                else
                    EQ_GLOBAL(short, 0x006B642A) = 0;
                if (*(short *)((char *)textIO + (int)EQ_GLOBAL(short, 0x006B642A) * 2 + 0x17C) == -1)
                    EQ_GLOBAL(short, 0x006B642A) = 0;
                break;
            case 1:
                EQ_GLOBAL(char, 0x006B6428) = 8;
                break;
            case 2:
                EQ_GLOBAL(char, 0x006B6428) = 2;
                break;
            case 3:
                EQ_GLOBAL(char, 0x006B6428) = 7;
                break;
            case 4:
                EQ_GLOBAL(char, 0x006B6428) = 5;
                break;
            case 5:
                EQ_GLOBAL(char, 0x006B6428) = 4;
                break;
            case 6:
                EQ_GLOBAL(char, 0x006B6428) = 0;
                break;
            }
        }
    }
    else if (action == 0)
    {
        switch (controlId)
        {
        case 0:
            EQ_METHOD(0, void, 0x0048CC53, (int *), textIO)();
            break;
        case 1:
            EQ_METHOD(0, void, 0x0048CBDB, (int *), textIO)();
            break;
        }
    }
    else if (action == 1)
    {
        switch (controlId)
        {
        case 0:
            EQ_METHOD(0, void, 0x0048CB40, (int *), textIO)();
            break;
        case 1:
            EQ_METHOD(0, void, 0x0048CAF2, (int *), textIO)();
            break;
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0048b9a0
// Thunk that forwards its input value to the chat window display routine.
// ORIGINAL FILE: EverQuest/EQ_TextIO.cpp (from Mac debug map)
void __fastcall EQ_TextIO_dsp_input_guess(EQ_TextIO_guess *self, int edx, int inputText)
{
    EQ_STDCALL(void, 0x0048B9AC, (int))(inputText);
}

#define EQ_TEXTIO_DRAW_EXPLICIT(rect_, pos_) \
    EQ_THISCALL(void, 0x004500BE, (void *, int, void *, int, int *)) \
        (self, 0, (void *)(*(int *)(EQ_GLOBAL(int, 0x006E4E1C) + 4)), \
         (int)(rect_), (pos_))
#define EQ_TEXTIO_DRAW_IMAGE1(image_, rect_) \
    EQ_THISCALL(void, 0x004384B2, (void *, int, int *)) \
        ((void *)(image_), 0, (int *)(rect_))
#define EQ_TEXTIO_DRAW_IMAGE2(image_, rect_, flag_) \
    EQ_THISCALL(void, 0x004384B2, (void *, int, int *, int)) \
        ((void *)(image_), 0, (int *)(rect_), (flag_))
#define EQ_TEXTIO_DRAW_PARTIAL(object_, overlay_, src_, dst_, flag_) \
    EQ_THISCALL(void, 0x00450125, (void *, int, void *, int, int *, int)) \
        ((void *)(object_), 0, (void *)(*(int *)(overlay_)), (int)(src_), (dst_), (flag_))

// FUNCTION: TRILOGY 0x0048b9ac
// __fastcall on EQ_TextIO; redraws the chat window with the typed text passed as the third argument.
// Skips drawing unless the old UI is active (0x004504ac) and the video mode is not 3 (or is 2 with its flag set).
// When channelPickerOpen is 0, sets the history/input text positions, selects font size and widths by video mode,
// then draws the frame, visible lines and the input line with clipping and wrapping.
// Otherwise draws the language/channel picker.
// ORIGINAL FILE: EverQuest/EQ_TextIO.cpp (from Mac debug map)
void __fastcall EQ_TextIO_draw_guess(EQ_TextIO_guess *self, int edx, int inputText)
{
    int height;
    char *xRight;
    int tmp;
    size_t sVar3;
    int *linePtr;
    int xLeft;
    int *lprc;
    size_t textLen;
    int textIO;
    int temp;
    int acc;
    int updateDisplayIndex;
    int clipThreshold;
    char languageText[2112];
    char visibleText[2112];
    int edgeRect[4];
    int left;
    int lineHeight;
    int charWidths;
    int drawRectA[4];
    int drawRectB[4];
    char *other;
    int game_state;

    textIO = (int)self;
    tmp = EQ_CDECL(int, 0x004504AC, (void))();
    temp = EQ_GLOBAL(int, 0x006E4E44);
    if (((tmp != 0) && (EQ_GLOBAL(int, 0x0058FC6C) != 3)) &&
        ((EQ_GLOBAL(int, 0x0058FC6C) != 2) ||
         (*(int *)(temp + 0x900F4) = 0, *(int *)(temp + 0x900F8) != 0)))
    {
        if (((EQ_TextIO_guess*)textIO)->channelPickerOpen == '\0')
        {
            ((EQ_TextIO_guess*)textIO)->historyTextX_guess = 0x9B;
            ((EQ_TextIO_guess*)textIO)->historyTextY_guess = 0x1AB;
            ((EQ_TextIO_guess*)textIO)->inputTextX_guess = 0x9B;
            ((EQ_TextIO_guess*)textIO)->inputTextY_guess = 0x1C2;
            charWidths = EQ_GLOBAL(int, 0x006EFDE0);
            temp = 0xC;
            if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
            {
                if (EQ_GLOBAL(int, 0x006E20BC) == 0)
                {
                    temp = 0xB;
                    charWidths = EQ_GLOBAL(int, 0x006EFDDC);
                }
                else if (EQ_GLOBAL(int, 0x006E20BC) == 1)
                    temp = 0xC;
                else if (EQ_GLOBAL(int, 0x006E20BC) == 2)
                {
                    temp = 0xD;
                    charWidths = EQ_GLOBAL(int, 0x006EFDE4);
                }
                else if (EQ_GLOBAL(int, 0x006E20BC) == 3)
                {
                    temp = 0xF;
                    charWidths = EQ_GLOBAL(int, 0x006EFDE8);
                }
                else if (EQ_GLOBAL(int, 0x006E20BC) == 4)
                {
                    temp = 0x10;
                    charWidths = EQ_GLOBAL(int, 0x006EFDEC);
                }
                else if (EQ_GLOBAL(int, 0x006E20BC) == 5)
                {
                    temp = 0x11;
                    charWidths = EQ_GLOBAL(int, 0x006EFDEC);
                }
            }
            lineHeight = 0xC;
            clipThreshold = 0x118;
            if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
            {
                g_num_display_buffers_guess = 0x19;
                ((EQ_TextIO_guess*)textIO)->redrawPass_guess = 1;
                game_state = EQ_GLOBAL(int, 0x006E4E44);
                linePtr = (int *)(game_state + 0x900E4);
                *(int *)(game_state + 0x900F4) = 0;
                left = *linePtr;
                tmp = *(int *)(game_state + 0x900E8);
                clipThreshold = tmp + 2;
                *(int *)(game_state + 0x900F4) = 0;
                acc = *(int *)(game_state + 0x900EC);
                *(int *)(game_state + 0x900F4) = 0;
                height = *(int *)(game_state + 0x900F0);
                *(int *)(game_state + 0x900F4) = 0;
                lineHeight = EQ_GLOBAL(int, 0x006B650C) + temp;
                if (EQ_GLOBAL(int, 0x006B650C) + temp < temp)
                    lineHeight = temp;
                xRight = (char *)(acc + left);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)
                    ((int *)(textIO + 0x15C), left, tmp,
                     acc + left, height + tmp);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)
                    ((int *)(textIO + 0x24), left, tmp,
                     (int)xRight, height + tmp);
                acc = (int)xRight - 0xC;
                xLeft = (int)xRight - 0xD;
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)
                    (edgeRect, xLeft, tmp, acc, height + tmp);
                ((EQ_TextIO_guess*)textIO)->historyTextX_guess = (short)left + 0x10;
                ((EQ_TextIO_guess*)textIO)->inputTextX_guess = (short)left + 0x20;
                ((EQ_TextIO_guess*)textIO)->inputTextY_guess = (short)height - 0x14 + (short)tmp;
                ((EQ_TextIO_guess*)textIO)->historyTextY_guess =
                    (((short)height + (short)lineHeight * -2) - (short)temp) - 4 + (short)tmp;

#define EQ_TEXTIO_DRAW_SPAN(diff_, cap_, left_, top_, right_, bottom_) \
                for (temp = (diff_); 0 < temp; temp = temp - tmp) \
                { \
                    tmp = temp; \
                    if ((cap_) < temp) tmp = (cap_); \
                    EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, (left_), (top_), (right_), (bottom_)); \
                    EQ_TEXTIO_DRAW_EXPLICIT(drawRectA, drawRectB); \
                    if (tmp < 1) tmp = 1; \
                }
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0x164) - *(int *)(textIO + 0x15C),
                    200, 0x70, 0x1DE, tmp + 0x71, 0x1DF);
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0x164) - *(int *)(textIO + 0x15C),
                    200, 0x70, 0x1DE, tmp + 0x71, 0x1DF);
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0x168) - *(int *)(textIO + 0x160),
                    100, 0x13F, 0x102, 0x140, tmp + 0x103);
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0x168) - *(int *)(textIO + 0x160),
                    100, 0x13F, 0x102, 0x140, tmp + 0x103);
                EQ_TEXTIO_DRAW_SPAN(edgeRect[3] - edgeRect[1],
                    100, 0x13F, 0x102, 0x140, tmp + 0x103);
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0xE8) - *(int *)(textIO + 0xE0),
                    100, 0x13F, 0x102, 0x140, tmp + 0x103);
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0xE4) - *(int *)(textIO + 0xDC),
                    200, 0x70, 0x1DE, tmp + 0x71, 0x1DF);
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0xF8) - *(int *)(textIO + 0xF0),
                    100, 0x13F, 0x102, 0x140, tmp + 0x103);
                EQ_TEXTIO_DRAW_SPAN(*(int *)(textIO + 0xF4) - *(int *)(textIO + 0xEC),
                    200, 0x70, 0x1DE, tmp + 0x71, 0x1DF);
#undef EQ_TEXTIO_DRAW_SPAN
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0x124, 0xCD, 0x132, 0xE1);
                EQ_TEXTIO_DRAW_EXPLICIT(drawRectA, drawRectB);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0x133, 0xCD, 0x141, 0xE1);
                EQ_TEXTIO_DRAW_EXPLICIT(drawRectA, drawRectB);
                lprc = drawRectA;
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(lprc, 0x134, 0xE6, 0x140, 0xF2);
                EQ_TEXTIO_DRAW_EXPLICIT(drawRectA, drawRectB);
            }
            else
            {
                g_num_display_buffers_guess = 0xB;
                temp = ((EQ_TextIO_guess*)textIO)->redrawPass_guess;
                if (temp != 0)
                {
                    if (temp < 4)
                    {
                        ((EQ_TextIO_guess*)textIO)->redrawAll_guess = 1;
                        ((EQ_TextIO_guess*)textIO)->redrawPass_guess = temp + 1;
                    }
                    else
                        ((EQ_TextIO_guess*)textIO)->redrawPass_guess = 0;
                }
                if (((EQ_TextIO_guess*)textIO)->redrawAll_guess == 0)
                    return;
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)
                    ((int *)(textIO + 0x24), 0x78, 0x14A, 0x20A, 0x1D9);
                lprc = (int *)EQ_GLOBAL(int, 0x006B341C) + 1;
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(lprc, 0x6F, 0x140, 0x20A, 0x1E0);
                *(int *)EQ_GLOBAL(int, 0x006B341C) = pinstEQ_Main_guess->overlaySprite_int_guess;
                EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B341C),
                    EQ_GLOBAL(int, 0x006B341C) + 4, 1);
                EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B3640),
                    textIO + 0xCC, 1);
                if (((EQ_TextIO_guess*)textIO)->pressedControlId_guess == 0)
                {
                    EQ_GLOBAL(SetRect_guess_t, 0x00534248)
                        (drawRectB, *(int *)(textIO + 0xAC),
                         *(int *)(textIO + 0xB0) - 1,
                         *(int *)(textIO + 0xB4), *(int *)(textIO + 0xB8));
                    EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B3638), drawRectB, 1);
                }
                if (((EQ_TextIO_guess*)textIO)->pressedControlId_guess == 1)
                    EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B363C),
                        textIO + 0xBC, 1);
            }
            temp = EQ_GLOBAL(int, 0x0055D674);
            updateDisplayIndex = 0;
            if (EQ_GLOBAL(char, 0x006B6404) == '\0')
            {
                if (((EQ_TextIO_guess*)textIO)->textDisplayMode_guess == 1)
                {
                    tmp = 0;
                    if (0 < g_num_display_buffers_guess)
                    {
                        linePtr = (int *)0x006D6B2C;
                        other = (char *)(-lineHeight);
                        acc = lineHeight * 2;
                        do
                        {
                            temp = tmp;
                            if (((EQ_TextIO_guess*)textIO)->historyTextY_guess + acc < clipThreshold)
                            {
                                updateDisplayIndex = 1;
                                break;
                            }
                            if ((*linePtr != 0) &&
                                (temp = strcmp((char *)*linePtr, (const char *)0x0055D6D4), temp != 0))
                            {
                                lprc = (int *)((int)((EQ_TextIO_guess*)textIO)->historyTextX_guess - 5);
                                EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))((char *)*linePtr,
                                     (int)lprc, ((EQ_TextIO_guess*)textIO)->historyTextY_guess + acc,
                                     0, 0, 1);
                            }
                            tmp = tmp + 1;
                            acc = acc + (int)other;
                            linePtr = linePtr + 1;
                            temp = EQ_GLOBAL(int, 0x0055D674);
                        } while (tmp < g_num_display_buffers_guess);
                    }
                }
                else
                {
                    tmp = 0;
                    if ((g_num_display_buffers_guess != 2) &&
                        (-1 < g_num_display_buffers_guess - 2))
                    {
                        acc = 0;
                        linePtr = (int *)0x006D6B2C;
                        do
                        {
                            temp = tmp;
                            if (((EQ_TextIO_guess*)textIO)->historyTextY_guess - acc < clipThreshold)
                            {
                                updateDisplayIndex = 1;
                                break;
                            }
                            if ((*linePtr != 0) &&
                                (temp = strcmp((char *)*linePtr, (const char *)0x0055D6D4), temp != 0))
                            {
                                lprc = (int *)((int)((EQ_TextIO_guess*)textIO)->historyTextX_guess - 5);
                                EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))((char *)*linePtr,
                                     (int)lprc, ((EQ_TextIO_guess*)textIO)->historyTextY_guess - acc,
                                     0, 0, 1);
                            }
                            tmp = tmp + 1;
                            acc = acc + lineHeight;
                            linePtr = linePtr + 1;
                            temp = EQ_GLOBAL(int, 0x0055D674);
                        } while (tmp < g_num_display_buffers_guess - 2);
                    }
                }
            }
            else
            {
                tmp = 0;
                if (((EQ_TextIO_guess*)textIO)->textDisplayMode_guess == 0)
                {
                    if (0 < g_num_display_buffers_guess)
                    {
                        linePtr = (int *)0x006D6B2C;
                        other = (char *)(-lineHeight);
                        acc = lineHeight * 2;
                        do
                        {
                            temp = tmp;
                            if (((EQ_TextIO_guess*)textIO)->historyTextY_guess + acc < clipThreshold)
                            {
                                updateDisplayIndex = 1;
                                break;
                            }
                            if ((*linePtr != 0) &&
                                (temp = strcmp((char *)*linePtr, (const char *)0x0055D6D4), temp != 0))
                            {
                                lprc = (int *)((int)((EQ_TextIO_guess*)textIO)->historyTextX_guess - 5);
                                EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))((char *)*linePtr,
                                     (int)lprc, ((EQ_TextIO_guess*)textIO)->historyTextY_guess + acc,
                                     0, 0, 1);
                            }
                            tmp = tmp + 1;
                            acc = acc + (int)other;
                            linePtr = linePtr + 1;
                            temp = EQ_GLOBAL(int, 0x0055D674);
                        } while (tmp < g_num_display_buffers_guess);
                    }
                }
                else if ((g_num_display_buffers_guess != 2) &&
                         (-1 < g_num_display_buffers_guess - 2))
                {
                    acc = 0;
                    linePtr = (int *)0x006D6B2C;
                    do
                    {
                        temp = tmp;
                        if (((EQ_TextIO_guess*)textIO)->historyTextY_guess - acc < clipThreshold)
                        {
                            updateDisplayIndex = 1;
                            break;
                        }
                        if ((*linePtr != 0) &&
                            (temp = strcmp((char *)*linePtr, (const char *)0x0055D6D4), temp != 0))
                        {
                            lprc = (int *)((int)((EQ_TextIO_guess*)textIO)->historyTextX_guess - 5);
                            EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))((char *)*linePtr,
                                 (int)lprc, ((EQ_TextIO_guess*)textIO)->historyTextY_guess - acc,
                                 0, 0, 1);
                        }
                        tmp = tmp + 1;
                        acc = acc + lineHeight;
                        linePtr = linePtr + 1;
                        temp = EQ_GLOBAL(int, 0x0055D674);
                    } while (tmp < g_num_display_buffers_guess - 2);
                }
            }
            if (updateDisplayIndex != 0)
                EQ_GLOBAL(int, 0x0055D674) = temp;
            if (EQ_GLOBAL(int, 0x0058FC6C) != 2)
            {
                if (((EQ_TextIO_guess*)textIO)->textDisplayMode_guess == 1)
                    EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B35EC), textIO + 0x9C, 1);
                if (((EQ_TextIO_guess*)textIO)->textDisplayMode_guess == 2)
                    EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B35F0), textIO + 0x9C, 1);
                if (((EQ_TextIO_guess*)textIO)->textDisplayMode_guess == 3)
                    EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B35F4), textIO + 0x9C, 1);
            }
            other = (char *)EQ_GLOBAL(int, 0x0057D3D0);
            if (0x260 < EQ_GLOBAL(int, 0x0057D3D0))
                other = (char *)0x260;
            if (EQ_GLOBAL(char, 0x006B6404) != '\0')
                return;
            if (((EQ_TextIO_guess*)textIO)->textDisplayMode_guess != 3)
                return;
            EQ_GLOBAL(int, 0x006EFDC4) = 0;
            if (inputText != 0)
            {
                temp = *(int *)(textIO + 0x170);
                tmp = temp;
                if (((EQ_TextIO_guess*)textIO)->inputCursor <= temp)
                {
                    do
                    {
                        acc = 0;
                        EQ_GLOBAL(int, 0x006EFDC4) = 0;
                        lprc = 0;
                        for (tmp = temp; -1 < tmp; tmp = tmp - 1)
                        {
                            if ('\0' < *(char *)(tmp + inputText))
                            {
                                acc = acc + *(int *)(charWidths +
                                    *(char *)(tmp + inputText) * 4);
                                EQ_GLOBAL(int, 0x006EFDC4) = acc;
                            }
                            if ((int)other <= acc)
                                break;
                            lprc = (int *)((int)lprc + 1);
                        }
                        if (tmp <= ((EQ_TextIO_guess*)textIO)->inputCursor)
                        {
                            *(int *)(textIO + 0x170) = temp;
                            break;
                        }
                        temp = temp + (-1 - (tmp - ((EQ_TextIO_guess*)textIO)->inputCursor) / 2);
                    } while (-1 < temp);
                }
                if (tmp < 0)
                    tmp = 0;
                else if (temp < tmp)
                    tmp = temp;
                if ((int)lprc < 1)
                    visibleText[0] = '\0';
                else
                    EQ_CDECL(void, 0x004A0841, (char *, char *, int))
                        (visibleText, (char *)(inputText + tmp), (int)lprc);
                other = visibleText;
                EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))(visibleText,
                     ((EQ_TextIO_guess*)textIO)->inputTextX_guess - 0xF,
                     ((EQ_TextIO_guess*)textIO)->historyTextY_guess - 0xF, 0xA, 0, 1);
                sVar3 = ((EQ_TextIO_guess*)textIO)->inputCursor - tmp;
                if ((int)sVar3 < 0)
                    sVar3 = 0;
                textLen = strlen(visibleText);
                if ((int)textLen < (int)sVar3)
                    sVar3 = textLen;
                temp = 0;
                tmp = 0;
                EQ_GLOBAL(int, 0x006EFDC4) = 0;
                if (0 < (int)sVar3)
                {
                    do
                    {
                        if ('\0' < visibleText[tmp])
                        {
                            temp = temp + *(int *)(charWidths + visibleText[tmp] * 4);
                            EQ_GLOBAL(int, 0x006EFDC4) = temp;
                        }
                        tmp = tmp + 1;
                    } while (tmp < (int)sVar3);
                }
            }
            other = (char *)0x0055D6D0;
        }
        else
        {
            EQ_GLOBAL(SetRect_guess_t, 0x00534248)
                ((int *)EQ_GLOBAL(int, 0x006B341C) + 1, 0x6F, 0x140, 0x20A, 0x1E0);
            *(int *)EQ_GLOBAL(int, 0x006B341C) = pinstEQ_Main_guess->overlaySprite_int_guess;
            EQ_TEXTIO_DRAW_IMAGE2(EQ_GLOBAL(int, 0x006B341C),
                EQ_GLOBAL(int, 0x006B341C) + 4, 1);
            EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectB, 0, 0, 0x15C, 0xA0);
            EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0x8A, 0x140, 0x280, 0x1E0);
            EQ_TEXTIO_DRAW_PARTIAL(self, (void *)(EQ_GLOBAL(int, 0x006E4DFC) + 4),
                drawRectB, drawRectA, 1);
            temp = *(short *)(textIO + 0x17C +
                (short)EQ_GLOBAL(short, 0x006B642A) * 2);
            strcpy(languageText,
                 EQ_METHOD(1, char *, 0x004C8FCE, (void *, short), EQ_GLOBAL(void *, 0x006EFE2C))((short)temp));
            sVar3 = strlen(languageText);
            temp = 0;
            if (0 < (int)sVar3)
            {
                do
                {
                    temp = temp + 1;
                } while (temp < (int)sVar3);
            }
            EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))(languageText, 0,
                 *(int *)(textIO + 0xB0), 0,
                 EQ_GLOBAL(int, 0x006EFE08), 1);
            if (EQ_GLOBAL(char, 0x006B6428) != 8)
            {
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectB, 0xB7, 0x192, 0xC1, 0x19C);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0x2D, 0x6E, 0x37, 0x78);
                EQ_TEXTIO_DRAW_PARTIAL(self, (void *)(EQ_GLOBAL(int, 0x006E4DFC) + 4),
                    drawRectB, drawRectA, 1);
            }
            if (EQ_GLOBAL(char, 0x006B6428) == 2)
            {
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectB, 0x2D, 0x52, 0x37, 0x5C);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0xB7, 0x1A0, 0xC1, 0x1AA);
                EQ_TEXTIO_DRAW_PARTIAL(self, (void *)(EQ_GLOBAL(int, 0x006E4DFC) + 4),
                    drawRectB, drawRectA, 1);
            }
            if (EQ_GLOBAL(char, 0x006B6428) == 7)
            {
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectB, 0x2D, 0x52, 0x37, 0x5C);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0xB7, 0x1AE, 0xC1, 0x1B8);
                EQ_TEXTIO_DRAW_PARTIAL(self, (void *)(EQ_GLOBAL(int, 0x006E4DFC) + 4),
                    drawRectB, drawRectA, 1);
            }
            if (EQ_GLOBAL(char, 0x006B6428) == 5)
            {
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectB, 0x2D, 0x52, 0x37, 0x5C);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0x1BD, 0x192, 0x1C7, 0x19C);
                EQ_TEXTIO_DRAW_PARTIAL(self, (void *)(EQ_GLOBAL(int, 0x006E4DFC) + 4),
                    drawRectB, drawRectA, 1);
            }
            if (EQ_GLOBAL(char, 0x006B6428) == 4)
            {
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectB, 0x2D, 0x52, 0x37, 0x5C);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0x1BD, 0x1A0, 0x1C7, 0x1AA);
                EQ_TEXTIO_DRAW_PARTIAL(self, (void *)(EQ_GLOBAL(int, 0x006E4DFC) + 4),
                    drawRectB, drawRectA, 1);
            }
            if (EQ_GLOBAL(char, 0x006B6428) == 0)
            {
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectB, 0x2D, 0x52, 0x37, 0x5C);
                EQ_GLOBAL(SetRect_guess_t, 0x00534248)(drawRectA, 0x1BD, 0x1AE, 0x1C7, 0x1B8);
                EQ_TEXTIO_DRAW_PARTIAL(self, (void *)(EQ_GLOBAL(int, 0x006E4DFC) + 4),
                    drawRectB, drawRectA, 1);
            }
            other = (char *)0x006B642C;
        }
        other = (char *)0x0055D6D0;
        EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, int, int, int), EQ_GLOBAL(void *, 0x006E4E68))(other,
             ((EQ_TextIO_guess*)textIO)->inputTextX_guess - 0xF,
             ((EQ_TextIO_guess*)textIO)->historyTextY_guess - 0xF, 0xA, 0, 1);
    }
    return;
}

#undef EQ_TEXTIO_DRAW_EXPLICIT
#undef EQ_TEXTIO_DRAW_IMAGE1
#undef EQ_TEXTIO_DRAW_IMAGE2
#undef EQ_TEXTIO_DRAW_PARTIAL

// FUNCTION: TRILOGY 0x0048ca78
// Selects the active chat history position and prepares the available entries for display.
// ORIGINAL FILE: EverQuest/EQ_TextIO.cpp (from Mac debug map)
void EQ_TextIO_guess::up_display(void)
{
    int i;
    int display_index;
    char **offset_slot;

    this->redrawPass_guess = 1;
    display_index = g_current_display_guess;
    g_display_current_data_guess = g_display_data_guess + display_index * 0x100;
    g_display_current_offset_guess = g_display_offsets_guess + display_index;

    i = 1;
    if (g_num_display_buffers_guess > i)
    {
        offset_slot = (char **)0x006D6B94;
        do
        {
            if (g_current_display_guess + i < 499)
            {
                offset_slot[-25] = (char *)(g_display_data_guess +
                                            (g_current_display_guess + i) * 0x100);
                *offset_slot = (char *)(g_display_offsets_guess +
                                        g_current_display_guess + i);
            }
            else
            {
                offset_slot[-25] = 0;
                *offset_slot = 0;
            }

            i++;
            offset_slot++;
        }
        while (i < g_num_display_buffers_guess);
    }
}

// FUNCTION: TRILOGY 0x0048caf2
// Returns the chat view to the newest text and redraws it.
void EQ_TextIO_guess::ResetTextBufferAndUpdateDisplay_guess(void)
{
    int** rect_slot;
    int* rectangle;

    g_current_display_guess &= 0;
    EQ_METHOD(0, void, 0x0048CA78, (EQ_TextIO_guess*), this)();

    rect_slot = (int**)((char*)this + 0x88);
    (*rect_slot)[1] = (int)this->scrollBottomLimit;
    rectangle = *rect_slot;
    rectangle[3] = (int)this->scrollThumbHeight_guess + rectangle[1];
    g_CopyRect_guess((int*)((char*)this + 0xcc), *rect_slot);
    EQ_METHOD(1, void, 0x0048B9AC, (EQ_TextIO_guess*, int), this)(0);
}

// FUNCTION: TRILOGY 0x0048cb40
// Moves the chat view to the oldest available history and redraws it.
// fake match: the explicit continue keeps each strcmp argument push on the loop back edge.
void EQ_TextIO_guess::ResetChatBufferToTop_guess(void)
{
    int** rect_slot;
    short offset;
    int i;
    char* display;
    int result;
    int divisor;

    i = 0;
    display = g_display_data_guess;
    while (1)
    {
        result = strcmp(display, EQ_ARRAY(char, 0x0055D6D4));
        if (result == 0)
            break;
        display = display + 0x100;
        i = i + 1;
        if ((int)display < 0x006d6b2b)
            continue;
        break;
    }

    g_current_display_guess = i - 1;
    EQ_METHOD(0, void, 0x0048CA78, (EQ_TextIO_guess*), this)();
    divisor = EQ_GLOBAL(short, 0x006E0FEA) - 1;
    if (divisor != 0)
        offset = (short)((this->scrollTravel * g_current_display_guess) /
                         divisor);
    else
        offset = 0;
    rect_slot = (int**)((char*)this + 0x88);
    (*rect_slot)[1] = (int)this->scrollBottomLimit - (int)offset;
    (*rect_slot)[3] = (int)this->scrollThumbHeight_guess + (*rect_slot)[1];
    g_CopyRect_guess((int*)((char*)this + 0xcc), *rect_slot);
    EQ_METHOD(1, void, 0x0048B9AC, (EQ_TextIO_guess*, int), this)(0);
}

// FUNCTION: TRILOGY 0x0048cbdb
// Moves the chat view one entry toward newer messages.
/* fake match: the divisor local makes VC6 load and dec it in ECX (movsx ecx; dec ecx) like the original */
void EQ_TextIO_guess::ScrollTextBufferDown_guess(void)
{
    int* rectPointerField;
    short offset;
    int divisor;

    if (0 < g_current_display_guess)
        g_current_display_guess = g_current_display_guess - 1;
    EQ_METHOD(0, void, 0x0048CA78, (EQ_TextIO_guess*), this)();
    divisor = EQ_GLOBAL(short, 0x006E0FEA) - 1;
    if (divisor != 0)
        offset = (short)((this->scrollTravel * g_current_display_guess) / divisor);
    else
        offset = 0;
    rectPointerField = (int*)((char*)this + 0x88);
    *(int*)(*rectPointerField + 4) = (int)this->scrollBottomLimit - (int)offset;
    *(int*)(*rectPointerField + 0xc) = (int)this->scrollThumbHeight_guess + *(int*)(*rectPointerField + 4);
    g_CopyRect_guess((int*)((char*)this + 0xcc), (int*)*rectPointerField);
    EQ_METHOD(1, void, 0x0048B9AC, (EQ_TextIO_guess*, int), this)(0);
}

// FUNCTION: TRILOGY 0x0048cc53
// Moves the chat view one entry toward older messages when history is available.
void EQ_TextIO_guess::scrollTextBufferUp_guess(void)
{
    int* rectRef;
    short scrollOffset;
    int cmpResult;
    int divisor;

    if (g_current_display_guess < 499)
    {
        cmpResult = strcmp(&EQ_ARRAY(char, 0x006B782B)[g_current_display_guess * 0x100], (const char*)0x0055d6d4);
        if (cmpResult != 0)
            g_current_display_guess = g_current_display_guess + 1;
    }
    EQ_METHOD(0, void, 0x0048CA78, (EQ_TextIO_guess*), this)();
    divisor = EQ_GLOBAL(short, 0x006E0FEA) - 1;
    if (divisor != 0)
        scrollOffset = (short)((this->scrollTravel * g_current_display_guess) / divisor);
    else
        scrollOffset = 0;
    rectRef = (int*)((char*)this + 0x88);
    *(int*)(*rectRef + 4) = (int)this->scrollBottomLimit - (int)scrollOffset;
    *(int*)(*rectRef + 0xc) = (int)this->scrollThumbHeight_guess + *(int*)(*rectRef + 4);
    g_CopyRect_guess((int*)((char*)this + 0xcc), (int*)*rectRef);
    EQ_METHOD(1, void, 0x0048B9AC, (EQ_TextIO_guess*, int), this)(0);
}

// FUNCTION: TRILOGY 0x0048cce7
// Word-wraps a chat message to the chat window width (continuation lines indented) and adds each line.
// @early-stop: switch dispatch, register/stack choices, and helper-call setup still differ.
void __fastcall EQ_TextIO_ProcessTextWithWordWrapping_guess(
    EQ_TextIO_guess* self, int /* edxDummy */, char* text, int color, int flag)
{
    short charIndex;
    short sVar2;
    size_t textLen;
    int scanIndex;
    int temp;
    int iVar6;
    char* _Str = text;
    int continuation;
    int* charWidths;

    self->redrawPass_guess = 1;
    EQ_GLOBAL(int, 0x00592288) = 0;
    if (text != (char*)0)
    {
        if (flag != 0)
        {
            EQ_METHOD(2, void, 0x0048CFD9, (EQ_TextIO_guess*, char*, int), self)(text, (short)color);
        }
        textLen = strlen(text);
        charWidths = (int*)EQ_GLOBAL(int, 0x006EFDE0);
        charIndex = 0;
        if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
        {
            switch (EQ_GLOBAL(int, 0x006E20BC))
            {
            case 0:
                charWidths = (int*)EQ_GLOBAL(int, 0x006EFDDC);
                break;
            case 1:
                break;
            case 2:
                charWidths = (int*)EQ_GLOBAL(int, 0x006EFDE4);
                break;
            case 3:
                charWidths = (int*)EQ_GLOBAL(int, 0x006EFDE8);
                break;
            case 4:
            case 5:
                charWidths = (int*)EQ_GLOBAL(int, 0x006EFDEC);
                break;
            }
        }
        if (charIndex < (short)textLen)
        {
            do
            {
                if ((short)textLen <= 0)
                    return;
                if (_Str == text)
                {
                    temp = EQ_GLOBAL(int, 0x0057D3D0);
                    continuation = 0;
                }
                else
                {
                    continuation = 1;
                    temp = EQ_GLOBAL(int, 0x0057D3D0) + charWidths[0x20] * -4;
                }
                if (EQ_GLOBAL(int, 0x00592288) > temp)
                {
                    scanIndex = (int)charIndex;
                    iVar6 = 0;
                    temp = scanIndex + -10;
                    sVar2 = 0;
                    if (scanIndex > temp)
                    {
                        do
                        {
                            if (((-1 < scanIndex) && ((short)iVar6 == 0)) && (*(_Str + scanIndex) == ' '))
                                iVar6 = scanIndex + 1;
                            sVar2 = (short)iVar6;
                            scanIndex = scanIndex + -1;
                        } while (temp < scanIndex);
                    }
                    if (sVar2 == 0)
                        sVar2 = charIndex + -1;
                    EQ_METHOD(5, void, 0x0048CE57, (EQ_TextIO_guess*, char*, int, int, int, int), self)(_Str, (int)sVar2, continuation, color, flag);
                    EQ_GLOBAL(int, 0x00592288) = 0;
                    charIndex = 0;
                    _Str = _Str + sVar2;
                    textLen = strlen(_Str);
                }
                if ('\0' < _Str[charIndex])
                    EQ_GLOBAL(int, 0x00592288) = EQ_GLOBAL(int, 0x00592288) + charWidths[_Str[charIndex]];
                charIndex = charIndex + 1;
                sVar2 = (short)textLen;
            } while (charIndex < sVar2);
            if (0 < sVar2)
            {
                EQ_METHOD(5, void, 0x0048CE57, (EQ_TextIO_guess*, char*, int, int, int, int), self)(_Str, (int)sVar2, continuation, color, flag);
            }
        }
    }
    return;
}

class EQ_TextIO_AddTextToChatBuffer_owner_guess
{
public:
    void AddTextToChatBuffer_guess(char* text, size_t textLength, int continuation,
                                   unsigned short displayOffset, int redraw);
};

// FUNCTION: TRILOGY 0x0048ce57
// Adds chat text to history and redraws the window when requested.
void EQ_TextIO_AddTextToChatBuffer_owner_guess::AddTextToChatBuffer_guess(
    char* text, size_t textLength, int continuation, unsigned short displayOffset, int redraw)
{
    char* destination;
    short* offset;

    *(int*)((char*)this + 0x1ec) = 1;
    offset = (short*)0x006d6fda;
    destination = (char*)0x006d6a2b;
    do
    {
        strcpy(destination, destination - 0x100);
        *offset = offset[-1];
        destination -= 0x100;
        offset--;
    }
    while ((int)destination > 0x006b772b);

    if (continuation == 0)
    {
        strncpy((char*)0x006b772b, text, textLength);
        ((char*)0x006b772b)[textLength] = 0;
    }
    else
    {
        strncpy((char*)0x006b772f, text, textLength);
        *(char*)0x006b772b = ' ';
        *(char*)0x006b772c = ' ';
        *(char*)0x006b772d = ' ';
        *(char*)0x006b772e = ' ';
        ((char*)0x006b772f)[textLength] = 0;
    }

    EQ_GLOBAL(short, 0x006D6BF4) = displayOffset;
    ++EQ_GLOBAL(short, 0x006E0FEA);
    if (EQ_GLOBAL(short, 0x006E0FEA) > 0x1f2)
        EQ_GLOBAL(short, 0x006E0FEA) = 0x1f2;
    if (redraw != 0)
        EQ_METHOD(1, void, 0x0048B9AC, (void*, int), (void*)this)(0);
}

// FUNCTION: TRILOGY 0x0048cf22
// Empties the raw chat text ring buffer that the chat history is rebuilt from.
void EQ_TextIO_ClearTextBuffer_guess(void)
{
    if (g_text_buffer_guess != 0)
    {
        memset(g_text_buffer_guess, 0, g_text_buffer_size_guess);
        g_text_buffer_write_guess = g_text_buffer_read_guess = g_text_buffer_guess + 4;
        g_text_buffer_current_guess = g_text_buffer_guess;
    }
}

class EQ_TextIO_UpdateBuffer_owner_guess
{
public:
    void UpdateBufferFromRawTextLines(void);
};

// FUNCTION: TRILOGY 0x0048cf56
// Rebuilds chat history from queued raw text lines using the current display layout.
// ORIGINAL FILE: EverQuest/EQ_TextIO.cpp (from Mac debug map)
void EQ_TextIO_UpdateBuffer_owner_guess::UpdateBufferFromRawTextLines(void)
{
    char* displayLine;
    int iVar2;
    short* offsetEntry;
    unsigned int recordValue;
    char* lineCursor;

    lineCursor = 0;
    if (g_text_buffer_guess != 0)
    {
        offsetEntry = g_display_offsets_guess;
        displayLine = g_display_data_guess + 1;
        do
        {
            displayLine[-1] = 0x40;
            *displayLine = 0;
            *offsetEntry = 0;
            displayLine = displayLine + 0x100;
            offsetEntry = offsetEntry + 1;
        } while ((int)displayLine < 0x006D6B2C);
        EQ_GLOBAL(short, 0x006E0FEA) = 0;
        g_current_display_guess = 0;
        while (1)
        {
            iVar2 = EQ_METHOD(3, int, 0x0048D0C4, (void*, char*, unsigned int*, unsigned int*), this)(lineCursor, (unsigned int*)&lineCursor, &recordValue);
            if (iVar2 == 0 || lineCursor == 0)
                break;
            EQ_METHOD(3, void, 0x0048CCE7, (void*, char*, unsigned int, int), this)(lineCursor, recordValue, 0);
        }
        EQ_METHOD(0, void, 0x0048CAF2, (void*), this)();
    }
}

class GetRawTextLine_owner_guess
{
public:
    int EQ_TextIO_ProcessTextForChatBuffer_guess(char* param_1, int param_2);
    int GetRawTextLine(char* param_1, unsigned int* param_2, unsigned int* param_3);
};

// FUNCTION: TRILOGY 0x0048cfd9
// Queues raw chat text and its value tag for later processing.
int GetRawTextLine_owner_guess::EQ_TextIO_ProcessTextForChatBuffer_guess(char* text, int value)
{
    char* requiredEnd;
    size_t textLength;
    int hasRawLine;
    char valueOut[4];
    unsigned int* readCursor;

    if (g_text_buffer_guess == 0)
        return 0;
    textLength = strlen(text);
    requiredEnd = &g_text_buffer_current_guess[textLength] + 6;
    if (g_text_buffer_write_guess < g_text_buffer_read_guess ||
        requiredEnd >= g_text_buffer_end_guess)
    {
        if (requiredEnd >= g_text_buffer_end_guess)
            g_text_buffer_current_guess = g_text_buffer_guess;
        readCursor = (unsigned int*)g_text_buffer_read_guess;
        char *temp = (g_text_buffer_current_guess + textLength + 6);
        if ((unsigned int*)temp >= (readCursor - 1))
        {
            do
            {
                hasRawLine = EQ_METHOD(3, int, 0x0048D0C4, (GetRawTextLine_owner_guess*, char*, unsigned int*, unsigned int*), this)((char*)readCursor, (unsigned int*)&readCursor, (unsigned int*)valueOut);
                if (hasRawLine == 0)
                    return 0;
            } while (((unsigned int*)(g_text_buffer_current_guess + textLength + 6) >= (readCursor - 1)) &&
                     ((g_text_buffer_read_guess <= (char*)readCursor) ||
                      (readCursor - 1 != (unsigned int*)g_text_buffer_guess)));
            g_text_buffer_read_guess = (char*)readCursor;
        }
    }
    *(unsigned int*)g_text_buffer_current_guess = (unsigned int)value;
    g_text_buffer_write_guess = g_text_buffer_current_guess = g_text_buffer_current_guess + 4;
    strcpy(g_text_buffer_write_guess, text);
    g_text_buffer_current_guess += textLength;
    *g_text_buffer_current_guess = 0;
    *(g_text_buffer_current_guess + 1) = 0x7e;
    ++g_text_buffer_current_guess;
    return 1;
}

// FUNCTION: TRILOGY 0x0048D0C4
// Retrieves a chat line and its associated value from the raw text buffer.
int GetRawTextLine_owner_guess::GetRawTextLine(char* rawStartLine, unsigned int* textStartOut, unsigned int* valueOut)
{
    char* recordStart;

    if (g_text_buffer_guess == 0)
    {
        DbgLog_guess(EQ_ARRAY(char, 0x0055D700));
    }
    else
    {
        char* buffer = g_text_buffer_guess;

        if (rawStartLine == 0)
        {
            recordStart = g_text_buffer_read_guess;
            if ((recordStart < buffer + 4) ||
                recordStart > g_text_buffer_end_guess - 2)
            {
                EQTRACE_guess(EQ_ARRAY(char, 0x0055D6D8));
                recordStart = g_text_buffer_guess + 4;
            }
            if ((*recordStart == '\0') && (g_text_buffer_read_guess == g_text_buffer_write_guess))
            {
                return 0;
            }
            recordStart = recordStart - 4;
        }
        else if ((rawStartLine < buffer) || (rawStartLine > g_text_buffer_end_guess - 2))
        {
            return 0;
        }
        else
        {
            char* scan = rawStartLine;

            for (;;)
            {
                if (scan + 2 >= g_text_buffer_end_guess)
                {
                    recordStart = buffer;
                    break;
                }
                if (*scan == '\0')
                {
                    recordStart = scan + 1;
                    if (scan[1] == '~')
                    {
                        if (g_text_buffer_write_guess >= g_text_buffer_read_guess)
                            return 0;
                        if (scan <= g_text_buffer_read_guess)
                            return 0;
                        recordStart = buffer;
                    }
                    break;
                }
                ++scan;
            }
        }
        *valueOut = *(unsigned int*)recordStart;
        *textStartOut = (unsigned int)(recordStart + 4);
        return 1;
    }
    return 0;
}
