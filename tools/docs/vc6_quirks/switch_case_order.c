// QUIRK: vc6q-switch-case-order
// KEYWORDS: switch, jump table, block order, case order, layout
// With a jump table (/O2 here) the case bodies are laid out in source order, so the table entries show
// the original's case order. A /O1 compare chain (< 8 cases) lays them out the same whatever the source
// order.
// VARIANTS: UP DOWN UP_O1 DOWN_O1
// FLAGS@UP: /O2
// FLAGS@DOWN: /O2
// EXPECT: f differs UP DOWN
// EXPECT@UP: f has push 0xa\n(.*\n){3}push 0xb
// EXPECT@DOWN: f has push 0xd\n(.*\n){3}push 0xc
// EXPECT: f same UP_O1 DOWN_O1
extern int g(int);
int f(int x)
{
    switch (x) {
#if defined(UP) || defined(UP_O1)
    case 0: return g(10);
    case 1: return g(11);
    case 2: return g(12);
    case 3: return g(13);
#else
    case 3: return g(13);
    case 2: return g(12);
    case 1: return g(11);
    case 0: return g(10);
#endif
    default: return 0;
    }
}
