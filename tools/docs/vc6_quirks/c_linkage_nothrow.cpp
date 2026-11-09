// QUIRK: vc6q-c-linkage-nothrow
// KEYWORDS: EH, state, extern "C", linkage, nothrow, cdecl, /GX, add esp, EQ_CDECL, EQ_CDECL_CPP
// Under /GX VC6 takes an extern "C" function as non-throwing, also when it is called through a cast of its
// address (the match build's EQ_CDECL: `((ret (__cdecl *) params)eqd_fn_<va>)`). Around such a call in a
// function with an EH frame it leaves out the state stores it emits around a call that may throw
// (`mov byte ptr [ebp - 4], n` before, `and byte ptr [ebp - 4], 0` after). The same cast of a function with
// C++ linkage (EQ_CDECL_CPP: the static member of eqd_cc<va>, eqaddr.h) keeps them.
// VARIANTS: CLINK CXX
// EXPECT@CLINK: use count mov byte ptr \[ebp - 4\], 1 == 0
// EXPECT@CXX: use has mov byte ptr \[ebp - 4\], 1\npush dword ptr \[ebp \+ 0xc\]\ncall \?f@\?\$eqd_cc@\$0EABAAA@@@SAXXZ\nand byte ptr \[ebp - 4\], 0
// EXPECT: use differs CLINK CXX
extern "C" void __cdecl eqd_fn_0x00401000(void);
template<unsigned VA> struct eqd_cc { static void __cdecl f(void); };
#ifdef CLINK
#define CALLEE ((void (__cdecl *)(int, int))eqd_fn_0x00401000)
#else
#define CALLEE ((void (__cdecl *)(int, int))&eqd_cc<0x00401000>::f)
#endif

struct R { int v; R(); ~R(); };

int use(int a, int b)
{
    R r;
    CALLEE(a, b);
    {
        R s;
        CALLEE(b, a);
    }
    CALLEE(a, a);
    return r.v;
}
