// EQ_Loading.cpp - loading overlay constructor.

#include <windows.h>
#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

typedef int (__stdcall *tSetRect_guess_t)(RECT* rect, int left, int top, int right, int bottom);

// EQ_Overlay sub-object of the loading overlay: forwards to EQ_Overlay's ctor and dtor (0x00450049 /
// 0x0045005D). The non-trivial dtor is what gives the derived constructor its EH frame.
class EQ_LoadingS_base_guess
{
public:
    EQ_LoadingS_base_guess()
    {
        EQ_METHOD(0, void, 0x00450049, (void*), this)();
    }

    ~EQ_LoadingS_base_guess()
    {
        EQ_METHOD(0, void, 0x0045005D, (void*), this)();
    }
};

class EQ_LoadingS : public EQ_LoadingS_base_guess
{
public:
    EQ_LoadingS();
};

// FUNCTION: TRILOGY 0x0043aafa
// Sets up the screen shown while the game loads, including the appropriate background bitmap.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Loading.cpp (from Mac debug map)
// @early-stop: VC6 stages the bitmap string in eax and pushes it after the join (original pushes it in each arm); EQ_THISCALL's EDX dummy before the load_guess call.
EQ_LoadingS::EQ_LoadingS()
    : EQ_LoadingS_base_guess()
{
    tSetRect_guess_t set_rect;

    set_rect = EQ_GLOBAL(tSetRect_guess_t, 0x00534248);
    *(u32*)this = 0x0053547c;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    set_rect((RECT*)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    set_rect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));

    char* display = EQ_GLOBAL(char*, 0x006E4DE0);
    const char* bitmap;
    if (*(int*)(display + 0x90) != 0)
        bitmap = EQ_ARRAY(char, 0x0053E0BC);
    else if (*(int*)(display + 0x8c) != 0)
        bitmap = EQ_ARRAY(char, 0x0053E0C8);
    else
        bitmap = EQ_ARRAY(char, 0x00559594);

    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x52, bitmap);
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(0, 0);
    set_rect((RECT*)((char*)this + 0xa8), 0x19, 0x125, 0x259, 0x15d);
    *(u32*)((char*)this + 0x78) = 0;
    *(u32*)((char*)this + 0x84) = 0;
}
