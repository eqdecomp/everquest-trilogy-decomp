#ifndef EQGFX_BLIT_SURFACE_LIFETIME_GUESS_H
#define EQGFX_BLIT_SURFACE_LIFETIME_GUESS_H

#define BLIT_SURFACE_INITIALIZE_ADDRESS_GUESS 0x10065F79
#define BLIT_SURFACE_PREPARE_ADDRESS_GUESS 0x1006603B
#define BLIT_SURFACE_DESTROY_ADDRESS_GUESS 0x100660D4

#ifdef EQD_MATCH
extern "C" void eqd_fn_0x1005C37A(void);
#endif

// A temporary descriptor owns the surface data released by 0x100660D4.
struct BlitSurfaceLifetime_guess {
    unsigned char descriptor[0x4a0];
    BlitSurfaceLifetime_guess(void *surface, int *rectangle) {
        EQ_METHOD(2, void, BLIT_SURFACE_INITIALIZE_ADDRESS_GUESS, (void *, void *, int *), this)(surface, rectangle);
    }
    BlitSurfaceLifetime_guess(int handle, int flags, int width, int height, int option4, int option5) {
        EQ_METHOD(6, void, BLIT_SURFACE_PREPARE_ADDRESS_GUESS, (void *, int, int, int, int, int, int), this)(handle, flags, width, height, option4, option5);
    }
    ~BlitSurfaceLifetime_guess() {
        EQ_METHOD(0, void, BLIT_SURFACE_DESTROY_ADDRESS_GUESS, (void *), this)();
    }
};

#endif
