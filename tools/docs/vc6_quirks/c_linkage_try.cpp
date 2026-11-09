// QUIRK: vc6q-c-linkage-try
// KEYWORDS: EH, try, catch, rethrow, extern "C", linkage, STL, realloc
// VARIANTS: CLINK CXX
// EXPECT@CLINK: insert lacks __EH_prolog
// EXPECT@CXX: insert has __EH_prolog
// EXPECT@CXX: insert has and dword ptr \[ebp - 4\], 0
// EXPECT@CLINK: insert lacks _release|__CxxThrowException
// EXPECT: insert differs CLINK CXX
#ifdef CLINK
extern "C" void copy(void *, const void *);
#else
extern void copy(void *, const void *);
#endif
extern "C" void *alloc(unsigned);
extern "C" void release(void *);
void *insert(const void *src)
{
    void *p = alloc(16);
    try { copy(p, src); }
    catch (...) { release(p); throw; }
    return p;
}
