// QUIRK: vc6q-short-push
// KEYWORDS: short, parameter, push, upper bits, mov cx
// VARIANTS: SHORT INT
// EXPECT@SHORT: forward has mov cx, word ptr \[eax \+ 4\]\npush ecx
// EXPECT@SHORT: forward lacks movsx|movzx
// EXPECT@INT: forward count movsx == 3
// EXPECT: forward differs SHORT INT
#ifdef INT
extern void sink(int, int, int);
#else
extern void sink(short, short, short);
#endif
void forward(const short *p) { sink(p[0], p[1], p[2]); }
