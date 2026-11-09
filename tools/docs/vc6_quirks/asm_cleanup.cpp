// QUIRK: vc6q-asm-cleanup
// KEYWORDS: inline, __asm, cleanup, optimisation, frame
// VARIANTS: ASM EMPTY
// EXPECT: sum differs ASM EMPTY
// EXPECT: sum lacks __EH_prolog|__except_list
// EXPECT@ASM: sum has ^push ebp\nmov ebp, esp
// EXPECT@ASM: sum has mov edx, dword ptr \[ebp \+ 0xc\]
// EXPECT@ASM: sum has nop\npop ebp\nret$
// EXPECT@EMPTY: sum has ^mov edx, dword ptr \[esp \+ 8\]
// EXPECT@EMPTY: sum lacks ebp|nop|__EH_prolog
struct G {
    ~G() {
#ifdef ASM
        __asm nop
#endif
    }
};
int sum(int *p, int n)
{
    G g;
    int v = 0;
    for (int i = 0; i < n; ++i) v += p[i];
    return v;
}
