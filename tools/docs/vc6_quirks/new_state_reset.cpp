// QUIRK: vc6q-new-state-reset
// KEYWORDS: EH, new, state, extern "C", linkage, join
// VARIANTS: CLINK CXX
// EXPECT: make has __EH_prolog
// EXPECT@CLINK: make lacks or dword ptr \[ebp - 4\], 0xffffffff
// EXPECT@CXX: make has or dword ptr \[ebp - 4\], 0xffffffff\npush esi\ncall _middle
// EXPECT: make differs CLINK CXX
struct R { R(); int x; };
extern "C" void middle(void *);
#ifdef CLINK
extern "C" void later(void *);
#else
extern void later(void *);
#endif
R *make()
{
    R *p = new R;
    middle(p);
    later(p);
    return p;
}
