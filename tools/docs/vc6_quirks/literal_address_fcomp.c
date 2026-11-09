// QUIRK: vc6q-literal-address-fcomp
// KEYWORDS: float, compare, fcomp, NaN, unordered, EQ_GLOBAL, literal address, test DLL, operand order
// A float compare against a global read through a literal address (`*(float *)0x10076284`, the test/hook
// DLL expansion of EQ_GLOBAL) loads the global first, `fld [va]; fcomp v` with the reversed jump (`ja`);
// through a symbol (the EQD_MATCH expansion, = the original) the source order stays, `fld v; fcomp [sym]`
// with `jb`. Both agree on ordered values; on NaN, `jb` rejects and `ja` falls through (accepts).
// Reading the global through a local pointer keeps the source order in both builds.
// VARIANTS: LIT SYM
// EXPECT@LIT: direct has ^fld dword ptr \[0x10076284\]\nfcomp dword ptr \[esp \+ 8\]\nfnstsw ax\nsahf\nja
// EXPECT@SYM: direct has ^fld dword ptr \[esp \+ 8\]\nfcomp dword ptr \[.*\]\nfnstsw ax\nsahf\njb
// EXPECT@LIT: via_ptr has ^fld dword ptr \[esp \+ 8\]\nfcomp dword ptr \[0x10076284\]\nfnstsw ax\nsahf\njb
// EXPECT@SYM: via_ptr has ^fld dword ptr \[esp \+ 8\]\nfcomp dword ptr \[.*\]\nfnstsw ax\nsahf\njb
// EXPECT: direct differs LIT SYM
#ifdef SYM
extern char eqd_va_0x10076284;
#define MINIMUM (*(float *)&eqd_va_0x10076284)
#else
#define MINIMUM (*(float *)(0x10076284))
#endif

typedef struct { char pad[0x3e8]; float value; } Ctx;

int direct(Ctx *c, float v)
{
    if (v >= MINIMUM) { c->value = v; return 0; }
    return -1;
}

int via_ptr(Ctx *c, float v)
{
    const float *minimum = &MINIMUM;

    if (v >= *minimum) { c->value = v; return 0; }
    return -1;
}
