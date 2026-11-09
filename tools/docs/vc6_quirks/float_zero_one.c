// QUIRK: vc6q-fldz-return
// KEYWORDS: x87, float, fldz, fld1, constant, return, ternary
// A `return 0.0f;` / `return 1.0f;` statement loads the constant from .rdata (`fld dword ptr [__real@...]`);
// the same value reached through `?:` or an assignment gives `fldz` / `fld1` (found in the DX7 review).
// VARIANTS: RET TERN ONE
// EXPECT@RET: f has fld dword ptr \[
// EXPECT@RET: f lacks fldz
// EXPECT@TERN: f has fldz
// EXPECT@ONE: f has fld1
float f(int i, int n, float x)
{
#if defined(RET)
    if (i >= n)
        return 0.0f;
    return x;
#elif defined(TERN)
    return i >= n ? 0.0f : x;
#else
    return i >= n ? 1.0f : x;
#endif
}
