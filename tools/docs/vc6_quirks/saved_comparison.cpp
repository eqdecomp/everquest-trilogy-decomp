// QUIRK: vc6q-saved-comparison
// KEYWORDS: branch, block order, compound comparison, int, temporary, /O1
// VARIANTS: SAVED DIRECT
// EXPECT: choose differs SAVED DIRECT
// EXPECT: choose count ^cmp ax, == 5
// EXPECT: choose lacks ^set[a-z]+ |^xor
// EXPECT@SAVED: choose has cmp ax, 0xfb\njne @12\njmp \?yes@@YAXXZ\njmp \?no@@YAXXZ$
// EXPECT@DIRECT: choose has cmp ax, 0xfb\nje @12\njmp \?no@@YAXXZ\njmp \?yes@@YAXXZ$
// EXPECT@SAVED: choose count ^je @ == 4
// EXPECT@DIRECT: choose count ^je @ == 5
extern void yes();
extern void no();
void choose(unsigned short race)
{
#ifdef SAVED
    int generic = race == 0xf4 || race == 0xf6 || race == 0xf7 || race == 0xfa || race == 0xfb;
    if (generic)
#else
    if (race == 0xf4 || race == 0xf6 || race == 0xf7 || race == 0xfa || race == 0xfb)
#endif
        yes();
    else
        no();
}
