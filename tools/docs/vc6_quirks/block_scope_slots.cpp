// QUIRK: vc6q-block-scope-slots
// KEYWORDS: stack, stack slot, frame size, block scope, switch, locals
// Locals declared in disjoint blocks (e.g. braces around each switch arm) share a stack slot; the same
// locals at function scope each get their own: frame 0x10 vs 0x20 here. If the original's frame is
// smaller than yours, scope the temporaries; if two arms use distinct slots, hoist them.
// VARIANTS: SCOPED FUNC
// EXPECT@SCOPED: f has sub esp, 0x10$
// EXPECT@FUNC: f has sub esp, 0x20$
void use(char *);
void f(int k)
{
#ifdef SCOPED
    switch (k) {
    case 0: { char a[16]; use(a); break; }
    case 1: { char b[16]; use(b); break; }
    }
#else
    char a[16];
    char b[16];
    switch (k) {
    case 0: use(a); break;
    case 1: use(b); break;
    }
#endif
}
