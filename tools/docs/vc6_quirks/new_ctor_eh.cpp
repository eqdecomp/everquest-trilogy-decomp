// QUIRK: vc6q-new-ctor-eh
// KEYWORDS: EH, new, constructor, throw(), definition order, frame, extern "C", linkage
// `new T(x)` gets an EH frame (to free the memory if the constructor throws) unless VC6 knows the
// constructor cannot throw: e.g. declared `throw()`, or an inline/earlier definition with a body VC6 can
// prove non-throwing. Definition order alone is insufficient: an earlier body calling only extern "C"
// functions drops the frame, while one calling a declared C++ function retains it (from EQ).
// VARIANTS: LATER NOTHROW EARLIER EARLIER_C EARLIER_CPP
// EXPECT@LATER: make has call __EH_prolog
// EXPECT@NOTHROW: make lacks call __EH_prolog
// EXPECT@EARLIER: make lacks call __EH_prolog
// EXPECT@EARLIER_C: make lacks call __EH_prolog
// EXPECT@EARLIER_CPP: make has call __EH_prolog
// EXPECT: make differs LATER EARLIER
// EXPECT: make differs EARLIER_C EARLIER_CPP
#ifdef EARLIER_C
extern "C" void init(int *);
#else
extern void init(int *);
#endif
#ifdef NOTHROW
struct Q { int v; Q(int x) throw(); };
#else
struct Q { int v; Q(int x); };
#endif

#ifdef EARLIER
Q::Q(int x) { v = x * 2; }
#endif
#if defined(EARLIER_C) || defined(EARLIER_CPP)
Q::Q(int x) { init(&v); v += x; }
#endif

Q *make(int x) { return new Q(x); }

#ifdef LATER
Q::Q(int x) { v = x * 2; }
#endif
