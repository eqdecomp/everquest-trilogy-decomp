// QUIRK: vc6q-inline-o1
// KEYWORDS: inline, inline budget, out-of-line copy, COMDAT, /O1, /O2, Ob1
// Both /O1 and /O2 imply /Ob1: only functions marked `inline` (or defined in a class body) are expanded; a
// plain `static` helper stays a call however small. An `inline` leaf is expanded at /O1, but at /O1 an
// inline whose body itself expands two inline calls is *not* expanded: the call stays and an out-of-line
// COMDAT copy of the inline is emitted in the same object (a function in the original with no caller of
// its own in the source). /O2 expands it. So an out-of-line copy of a small inline in a /O1 file is
// expected, not a sign of different source.
// VARIANTS: O1 O2
// FLAGS@O2: /O2
// EXPECT: usestatic has call \?helper@@YAHH@Z
// EXPECT: useleaf has ^mov eax, dword ptr \[esp \+ 4\]\nlea eax, \[eax \+ eax\*2 \+ 3\]\nret
// EXPECT@O1: usemid has call \?mid@@YAHH@Z
// EXPECT@O1: mid has call \?g@@YAHH@Z
// EXPECT@O2: usemid lacks call \?mid
// EXPECT@O2: usemid count call \?g@@YAHH@Z == 2
extern int g(int);
static int helper(int a) { return a * 3 + 1; }
inline int leaf(int a) { return a * 3 + 1; }
inline int gleaf(int a) { return g(a) + 1; }
inline int mid(int a) { return gleaf(a) * 2 + gleaf(a + 1); }
int usestatic(int x) { return helper(x) + 2; }
int useleaf(int x) { return leaf(x) + 2; }
int usemid(int x) { return mid(x); }
