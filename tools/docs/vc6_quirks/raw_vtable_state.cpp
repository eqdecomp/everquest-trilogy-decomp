// QUIRK: vc6q-raw-vtable-state
// KEYWORDS: EH, vtable, vfptr, state, scheduling, constructor, destructor, /O1
// VARIANTS: RAW POLY
// EXPECT: D::D has call __EH_prolog
// EXPECT: D::D has and dword ptr \[ebp - 4\], 0\npush esi\nmov dword ptr \[esi\],
// EXPECT: D::~D has call __EH_prolog
// EXPECT@RAW: D::~D has and dword ptr \[ebp - 4\], 0\npush esi\nmov dword ptr \[esi\], \?table@@3PAHA
// EXPECT@POLY: D::~D has mov dword ptr \[esi\], \?\?_7D@@6B@\nand dword ptr \[ebp - 4\], 0\npush esi
// EXPECT: D::~D differs RAW POLY
extern void work(void *);
extern int table[];
struct B {
    B();
#ifdef POLY
    virtual ~B();
#else
    ~B();
    int *vfptr;
#endif
};
struct D : B { D(); ~D(); };
D::D()
{
#ifndef POLY
    vfptr = table;
#endif
    work(this);
}
D::~D()
{
#ifndef POLY
    vfptr = table;
#endif
    work(this);
}
