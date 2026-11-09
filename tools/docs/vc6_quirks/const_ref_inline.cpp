// QUIRK: vc6q-const-ref-inline
// KEYWORDS: inline, const reference, by value, struct copy, movsd, frame
// A struct passed by value to an inline helper is still copied into a frame temporary (`movsd` x4, an ebp
// frame) after the expansion; through `const V&` the expansion reads the fields in place. A struct copy in
// the original around an inlined helper means a by-value parameter.
// VARIANTS: BYVAL BYREF
// EXPECT@BYVAL: use has movsd dword ptr es:\[edi\], dword ptr \[esi\]
// EXPECT@BYREF: use has ^mov ecx, dword ptr \[esp \+ 4\]\nmov eax, dword ptr \[ecx \+ 4\]\nadd eax, dword ptr \[ecx\]\nret
struct V { int a, b, c, d; };
#ifdef BYVAL
inline int sum(V v) { return v.a + v.b; }
#else
inline int sum(const V &v) { return v.a + v.b; }
#endif
int use(V *p) { return sum(*p); }
