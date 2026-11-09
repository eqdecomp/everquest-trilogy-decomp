// QUIRK: vc6q-decl-order-names
// KEYWORDS: regalloc, register, stack slot, declaration order, rename, entropy, slot order
// SP4 "entropy" check: the declaration order and the names of locals do not change the code (registers,
// stack slots), with or without address-taken locals. The order of the *uses* does change slot order.
// See VC6_QUIRKS.md for the measurement on real near misses.
// VARIANTS: BASE RENAMED REORDERED USEORDER
// EXPECT: f same BASE RENAMED
// EXPECT: f same BASE REORDERED
// EXPECT: slots same BASE RENAMED
// EXPECT: slots same BASE REORDERED
// EXPECT: slots differs BASE USEORDER
extern int g(int);
extern int h(int, int);
extern void use(int *);
struct S { int a, b, c, d; };

#ifdef RENAMED
#define X zz_first
#define Y aa_second
#define Z mm_third
#else
#define X x
#define Y y
#define Z z
#endif

int f(struct S *p, int n)
{
#ifdef REORDERED
    int Z; int Y; int X;
#else
    int X; int Y; int Z;
#endif
    X = p->a + n; Y = p->b * 3; Z = p->c - n;
    g(X); g(Y);
    if (n > 3) Z += h(X, Y);
    return X + Y + Z + p->d;
}

int slots(int k)
{
#ifdef REORDERED
    int Z, Y, X;
#else
    int X, Y, Z;
#endif
#ifdef USEORDER
    use(&Z); use(&X); use(&Y);
#else
    use(&X); use(&Y); use(&Z);
#endif
    return X + Y * Z + k;
}
