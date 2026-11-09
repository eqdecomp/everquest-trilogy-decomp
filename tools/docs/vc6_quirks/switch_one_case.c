// QUIRK: vc6q-switch-one-case
// KEYWORDS: switch, compare, dec, branch
// From gta2_re: a one-case `switch` loads the value and tests it with `dec eax`; the `if` compares memory
// against the constant. Same at /O1 and /O2.
// VARIANTS: SWITCH IF
// EXPECT@SWITCH: f has ^mov eax, dword ptr \[esp \+ 4\]\ndec eax\nje @\d+
// EXPECT@IF: f has ^cmp dword ptr \[esp \+ 4\], 1\njne @\d+
extern int g(int);
int f(int x)
{
#ifdef SWITCH
    switch (x) { case 1: return g(5); }
    return 0;
#else
    if (x == 1) return g(5);
    return 0;
#endif
}
