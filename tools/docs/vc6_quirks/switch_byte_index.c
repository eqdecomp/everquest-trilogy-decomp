// QUIRK: vc6q-switch-byte-index
// KEYWORDS: switch, jump table, index table, sparse switch, binary search, /O2
// Four sparse cases (0, 3, 6, R) at /O2: while the span fits a byte index table (R = 200) VC6 builds
// `mov cl, byte ptr [eax + table]` + a dword table with one entry per distinct body (+ default); at
// R = 300 it switches to a compare tree (`cmp eax, K; jg` / `je`).
// VARIANTS: R200 R300
// FLAGS: /O2
// EXPECT@R200: f has mov cl, byte ptr \[eax \+ @\d+\]\njmp dword ptr \[ecx\*4 \+ @\d+\]
// EXPECT@R200: f count ^dd @ == 5
// EXPECT@R300: f lacks jmp dword ptr
// EXPECT@R300: f has ^jg @\d+
extern int g(int);
#ifdef R200
#define R 200
#else
#define R 300
#endif
int f(int x)
{
    switch (x) {
    case 0: return g(1);
    case 3: return g(2);
    case 6: return g(3);
    case R: return g(4);
    default: return 0;
    }
}
