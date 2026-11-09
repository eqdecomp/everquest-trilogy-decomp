// QUIRK: vc6q-static-guard
// KEYWORDS: static, guard, function static, local static, atexit, constructor
// A function-local static with a constructor (or a non-constant initialiser) gets a guard bit in a
// compiler byte `?$S<n>@...`: `test byte ptr [guard], 1; jne skip; or byte ptr [guard], 1; <init>`, plus
// `push $E<n>; call _atexit` when it has a destructor. The function's statics share one guard byte, bits
// 1, 2, ... in declaration order.
// EXPECT: two has ^test byte ptr \[_\?\$S1@[^\]]*\], 1\njne @\d+\nor byte ptr \[_\?\$S1@[^\]]*\], 1
// EXPECT: two has test byte ptr \[_\?\$S1@[^\]]*\], 2
// EXPECT: two count call _atexit == 2
// EXPECT: dyn has test byte ptr \[_\?\$S\d+@[^\]]*\], 1
// EXPECT: dyn lacks _atexit
extern int g(int);
struct D { int v; D(); ~D(); };
int two(int x) { static D s; static D t; return s.v + t.v + x; }
int dyn(int x) { static int s = g(1); return s + x; }
