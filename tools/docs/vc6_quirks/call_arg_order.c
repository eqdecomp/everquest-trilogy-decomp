// QUIRK: vc6q-call-order
// KEYWORDS: evaluation order, arguments, call order, operand order
// Arguments are evaluated right to left: `k(g(), h())` calls h first. In `a() + b()` VC6 picks the call
// order itself: both spellings give the same code (gta2_re: force an order with a temporary).
// VARIANTS: HK KH
// EXPECT: args has ^call _h\npush eax\ncall _g\npush eax\ncall _kk
// EXPECT: sum same HK KH
extern int g(void);
extern int h(void);
extern int k(void);
extern int kk(int, int);
int args(void) { return kk(g(), h()); }
int sum(void)
{
#ifdef HK
    return h() + k();
#else
    return k() + h();
#endif
}
