// QUIRK: vc6q-setcc-compare
// KEYWORDS: setcc, sete, setle, compare, bool, return
// `return a == b;` (and <, <=, != 0, !x) on ints: `xor eax, eax` first, then `cmp` and `setcc al`. The
// explicit `if (a <= b) return 1; return 0;` compiles to the very same code on SP4 (gta2_re's "setle
// without the xor" from the if form doesn't reproduce for plain ints).
// VARIANTS: EXPR IF
// EXPECT: eq has ^mov ecx, dword ptr \[esp \+ 4\]\nxor eax, eax\ncmp ecx, dword ptr \[esp \+ 8\]\nsete al\nret
// EXPECT: ne0 has ^xor eax, eax\ncmp dword ptr \[esp \+ 4\], eax\nsetne al
// EXPECT: le has xor eax, eax\ncmp ecx, dword ptr \[esp \+ 8\]\nsetle al
// EXPECT: le same EXPR IF
int eq(int a, int b) { return a == b; }
int ne0(int a) { return a != 0; }
int le(int a, int b)
{
#ifdef EXPR
    return a <= b;
#else
    if (a <= b) return 1;
    return 0;
#endif
}
