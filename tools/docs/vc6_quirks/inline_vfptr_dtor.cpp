// QUIRK: vc6q-inline-vfptr-dtor
// KEYWORDS: EH, destructor, vfptr, inline, out-of-line copy, unwind
// VARIANTS: INLINE OUTLINE
// EXPECT: use has call __EH_prolog
// EXPECT@INLINE: use lacks call \?\?1R@@
// EXPECT@OUTLINE: use has call \?\?1R@@UAE@XZ
// EXPECT: R::~R has ^mov dword ptr \[ecx\], \?\?_7R@@6B@\nret$
// EXPECT: use differs INLINE OUTLINE
extern void work(void *);
#ifdef OUTLINE
struct R { virtual ~R(); };
#else
struct R { virtual ~R() {} };
#endif
void use() { R r; work(&r); }
#ifdef OUTLINE
R::~R() {}
#endif
