// QUIRK: vc6q-switch-table-threshold
// KEYWORDS: switch, jump table, dec chain, case count, /O1, /O2
// Dense cases 0..N-1 with a default. A jump table needs 8 cases at /O1 but only 4 at /O2; below that
// VC6 tests with a `sub eax, 0` / `dec eax; je` chain.
// VARIANTS: O1_7 O1_8 O2_3 O2_4
// FLAGS@O2_3: /O2
// FLAGS@O2_4: /O2
// EXPECT@O1_7: f lacks jmp dword ptr \[eax\*4
// EXPECT@O1_7: f count ^dec eax == 6
// EXPECT@O1_8: f has jmp dword ptr \[eax\*4 \+ @\d+\]
// EXPECT@O1_8: f count ^dd @ == 8
// EXPECT@O2_3: f lacks jmp dword ptr \[eax\*4
// EXPECT@O2_4: f has cmp eax, 3\nja @\d+\njmp dword ptr \[eax\*4 \+ @\d+\]
extern int g(int);

#if defined(O2_3)
#define N 3
#elif defined(O2_4)
#define N 4
#elif defined(O1_7)
#define N 7
#else
#define N 8
#endif

int f(int x)
{
    switch (x) {
    case 0: return g(1);
    case 1: return g(4);
    case 2: return g(7);
#if N > 3
    case 3: return g(10);
#endif
#if N > 4
    case 4: return g(13);
    case 5: return g(16);
    case 6: return g(19);
#endif
#if N > 7
    case 7: return g(22);
#endif
    default: return -1;
    }
}
