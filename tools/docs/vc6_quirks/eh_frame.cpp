// QUIRK: vc6q-eh-frame
// KEYWORDS: EH, exception, frame, destructor, __EH_prolog, fs:[0], state, /GX
// An EH frame is a source fact: under /GX a local with a destructor that is alive across a call gets one,
// a function without such an object never does. /O1 builds it with `mov eax, <handler>; call
// __EH_prolog` (ebp frame, state at [ebp - 4]: `and [ebp - 4], 0` after the constructor, `or [ebp - 4],
// -1` before the destructor); /O2 writes it out inline (`push -1; push <handler>; mov eax, fs:[0]`,
// state in an esp slot).
// VARIANTS: O1 O2
// FLAGS@O2: /O2
// EXPECT@O1: withdtor has ^mov eax, \.text\$x\ncall __EH_prolog
// EXPECT@O1: withdtor has and dword ptr \[ebp - 4\], 0
// EXPECT@O1: withdtor has or dword ptr \[ebp - 4\], 0xffffffff
// EXPECT@O2: withdtor has ^push -1\npush \.text\$x\nmov eax, dword ptr fs:\[__except_list\]
// EXPECT: nodtor lacks __except_list|__EH_prolog
extern int g(int);
struct D { int v; D(); ~D(); };
struct P { int v; P(); };
int withdtor(int x) { D d; return g(x) + d.v; }
int nodtor(int x) { P p; return g(x) + p.v; }
