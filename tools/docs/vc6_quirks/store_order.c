// QUIRK: vc6q-store-order
// KEYWORDS: store order, scheduling, zero store, constant, field stores
// Independent field stores mostly keep source order, but stores of zero get pulled together: at /O1 they
// become `and dword ptr [..], 0` and move first; at /O2 one zeroed register serves them, placed from the
// first zero store on. So the original's store order shows the source order only among the non-constant
// stores.
// VARIANTS: ABCD DCBA ABCD_O2 DCBA_O2
// FLAGS@ABCD_O2: /O2
// FLAGS@DCBA_O2: /O2
// EXPECT@ABCD: st has and dword ptr \[eax \+ 4\], 0\nand dword ptr \[eax \+ 0xc\], 0\nmov dword ptr \[eax\], ecx
// EXPECT@DCBA: st has and dword ptr \[eax \+ 0xc\], 0\nand dword ptr \[eax \+ 4\], 0\nmov dword ptr \[eax \+ 8\], ecx
// EXPECT@ABCD_O2: st has mov dword ptr \[eax\], ecx\nxor ecx, ecx\nmov dword ptr \[eax \+ 4\], ecx\nmov dword ptr \[eax \+ 8\], edx\nmov dword ptr \[eax \+ 0xc\], ecx
// EXPECT@DCBA_O2: st has mov dword ptr \[eax \+ 0xc\], ecx\nmov dword ptr \[eax \+ 4\], ecx\n(.*\n)?mov dword ptr \[eax \+ 8\], edx\nmov dword ptr \[eax\], ecx
struct S { int a, b, c, d; };
void st(struct S *p, int x, int y)
{
#if defined(ABCD) || defined(ABCD_O2)
    p->a = x; p->b = 0; p->c = y; p->d = 0;
#else
    p->d = 0; p->c = y; p->b = 0; p->a = x;
#endif
}
