// QUIRK: vc6q-signed-div-pow2
// KEYWORDS: arithmetic, modulo, division, power of two, idiv, /O1, /O2
// Signed `% 8` and `/ 4`: /O1 (size) keeps `push 8; cdq; pop ecx; idiv ecx`; /O2 uses the branchy
// `and eax, 0x80000007; jns; dec; or eax, -8; inc` for the modulo and `cdq; and edx, 3; add; sar` for the
// division. An `idiv` by a constant power of two in the original therefore points at /O1 (or a signed
// operand), the `and 0x80000007` form at /O2. Unsigned operands give `and` / `shr` at both levels.
// VARIANTS: O1 O2
// FLAGS@O2: /O2
// EXPECT@O1: mod8 has push 8\ncdq\npop ecx\nidiv ecx\nmov eax, edx
// EXPECT@O1: div4 has push 4\ncdq\npop ecx\nidiv ecx
// EXPECT@O2: mod8 has and eax, 0x80000007\njns @\d+\ndec eax\nor eax, 0xfffffff8\ninc eax
// EXPECT@O2: div4 has cdq\nand edx, 3\nadd eax, edx\nsar eax, 2
// EXPECT: umod8 has and eax, 7
// EXPECT: umod8 lacks idiv
int mod8(int a) { return a % 8; }
int div4(int a) { return a / 4; }
unsigned umod8(unsigned a) { return a % 8; }
