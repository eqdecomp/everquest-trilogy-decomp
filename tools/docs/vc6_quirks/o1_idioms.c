// QUIRK: vc6q-o1-idioms
// KEYWORDS: /O1, /O2, constants, push pop, stack cleanup, call
// Size idioms that tell /O1 code from /O2: a small constant into a register is `push 9; pop eax` (/O2:
// `mov eax, 9`); one stack argument is cleaned with `pop ecx` (/O2: `add esp, 4`). Both use
// `or eax, 0xffffffff` for -1.
// VARIANTS: O1 O2
// FLAGS@O2: /O2
// EXPECT@O1: k9 has ^push 9\npop eax\nret
// EXPECT@O2: k9 has ^mov eax, 9\nret
// EXPECT: km1 has ^or eax, 0xffffffff\nret
// EXPECT@O1: call1 has call _g\npop ecx
// EXPECT@O2: call1 has call _g\nadd esp, 4
extern int g(int);
int k9(void) { return 9; }
int km1(void) { return -1; }
int call1(int x) { return g(x); }
