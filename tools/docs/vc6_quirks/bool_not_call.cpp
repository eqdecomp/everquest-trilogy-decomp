// QUIRK: vc6q-bool-not-call
// KEYWORDS: bool, setcc, sete, neg sbb, not, ternary
// From gta2_re: on a `bool` call result, `!f()` gives `neg al; sbb eax, eax; inc eax`, while
// `f() ? false : true` gives `test al, al; sete al` at /O2. At /O1 the ternary gives the byte form
// `neg al; sbb al, al; inc al`, so the two spellings differ at both levels. (An `int` result gives
// neg/sbb/inc for both spellings.)
// VARIANTS: NOT TERN NOT_O2 TERN_O2
// FLAGS@NOT_O2: /O2
// FLAGS@TERN_O2: /O2
// EXPECT@NOT: nb has neg al\nsbb eax, eax\ninc eax
// EXPECT@NOT_O2: nb has neg al\nsbb eax, eax\ninc eax
// EXPECT@TERN: nb has neg al\nsbb al, al\ninc al
// EXPECT@TERN_O2: nb has test al, al\nsete al
bool hb();
bool nb()
{
#if defined(NOT) || defined(NOT_O2)
    return !hb();
#else
    return hb() ? false : true;
#endif
}
