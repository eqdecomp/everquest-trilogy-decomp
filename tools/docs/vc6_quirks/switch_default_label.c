// QUIRK: vc6q-switch-default-label
// KEYWORDS: switch, jump table, default, dec chain, merged labels, /O2
// From gta2_re ("a jump table whose default lands on a case block"): `case 4: default:` counts as three
// cases plus a default, so /O2 makes a `dec eax; je` chain. Writing `case 4:` out with its own (identical)
// body gives the jump table.
// VARIANTS: MERGED SEPARATE
// FLAGS: /O2
// EXPECT@MERGED: f lacks jmp dword ptr
// EXPECT@MERGED: f count ^dec eax == 3
// EXPECT@SEPARATE: f has jmp dword ptr \[eax\*4 \+ @\d+\]
extern int g(int);
int f(int x)
{
    switch (x) {
    case 1: return g(1);
    case 2: return g(2);
    case 3: return g(3);
#ifdef MERGED
    case 4: default: return g(4);
#else
    case 4: return g(4);
    default: return g(4);
#endif
    }
}
