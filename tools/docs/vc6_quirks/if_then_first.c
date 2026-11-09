// QUIRK: vc6q-if-then-first
// KEYWORDS: branch, block order, if else, jcc polarity, layout
// The `then` block is laid out first (fall-through) and the condition jumps to the `else` block. Inverting
// the condition and swapping the blocks swaps the layout and the jcc.
// VARIANTS: POS NEG
// EXPECT@POS: f has ^je @\d+\njmp _g\njmp _h
// EXPECT@NEG: f has ^jne @\d+\njmp _h\njmp _g
// EXPECT: f differs POS NEG
extern int g(void);
extern int h(void);
int f(int a)
{
#ifdef POS
    if (a) return g(); else return h();
#else
    if (!a) return h(); else return g();
#endif
}
