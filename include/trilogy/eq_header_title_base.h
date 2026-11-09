#ifndef TRILOGY_EQ_HEADER_TITLE_BASE_H
#define TRILOGY_EQ_HEADER_TITLE_BASE_H

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// Local stand-in: layout guessed from AllocateAndClearBuffer_guess alone (vtable 0x00534560 = GaugeControl_guess in BoxDraw.cpp).
struct EQGaugeBuf_guess
{
    u32     vtable;                             // 0x00
    char*   buffer;                             // 0x04
    int     size;                               // 0x08
    u32     f0c;                                // 0x0C
};

// Stand-in for the 0x9C-byte base at +0x00. Its constructor is 0x00403CDF (EQStaticHeaderElement_guess in utf8.cpp)
// and its destructor is 0x00403E27 (SpellGemControlHeader_guess in BoxDraw.cpp). Being destructible, it gets the EH
// state store after its construction, and the derived constructor's EH frame covers it.
class EQSpellGemBase_guess
{
public:
    u32     vtable_guess;                   // 0x00 (the derived constructor stores its vtable here)
    u8      unknown04_guess[0x27];          // 0x04
    u8      unknown2b_guess;                // 0x2B
    u8      unknown2c_guess[0x54];          // 0x2C
    u8      unknown80_guess;                // 0x80
    u8      unknown81_guess[0x1B];          // 0x81

    EQSpellGemBase_guess()
    {
        EQ_METHOD(0, void, 0x00403CDF, (void*), this)();
    }
    ~EQSpellGemBase_guess()
    {
        EQ_METHOD(0, void, 0x00403E27, (void*), this)();
    }
};

// Gauge member at +0x9C of EQStaticHeaderTitle_guess. Its constructor is 0x00404979 and its destructor is 0x004049CA
// (as the spell-gem gauge members; a guess for this one).
class EQStaticHeaderGauge_guess : public EQGaugeBuf_guess
{
public:
    EQStaticHeaderGauge_guess()
    {
        /* fake match: VC6 treats a call through a pointer variable as possibly throwing and a constant-address call as
           not, so only this form keeps the EH frame and the state store the original has around this member. */
        void (__fastcall *ctor)(void*) = EQ_THISCALL(void, 0x00404979, (void*));

        ctor(this);
    }
    ~EQStaticHeaderGauge_guess()
    {
        EQ_METHOD(0, void, 0x004049CA, (void*), this)();
    }
};

// Stand-in for the 0x10-byte gauge member at +0xEC and +0xFC (constructor 0x00404979, destructor 0x004049CA as
// SpellGemGaugeMember_guess in BoxDraw.cpp).
class EQSpellGemGauge_guess : public EQGaugeBuf_guess
{
public:
    EQSpellGemGauge_guess()
    {
        /* fake match: VC6 treats a call through a pointer variable as possibly throwing and a constant-address call as
           not, so only this form keeps the EH frame and the state store the original has around this member. */
        void (__fastcall *ctor)(void*) = EQ_FUNC(void, __fastcall, 0x00404979, (void*));
        ctor(this);
    }
    ~EQSpellGemGauge_guess()
    {
        EQ_METHOD(0, void, 0x004049CA, (void*), this)();
    }
};

C_ASSERT(sizeof(EQSpellGemBase_guess) == 0x9C);
C_ASSERT(sizeof(EQStaticHeaderGauge_guess) == 0x10);

#endif
