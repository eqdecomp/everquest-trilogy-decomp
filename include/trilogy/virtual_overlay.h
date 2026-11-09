/*
 * virtual_overlay.h - EQ_Overlay as a polymorphic class, for the derived classes whose constructor or destructor
 * must store the vfptr the way the compiler does (right after the base, ahead of the EH state of the body).
 *
 * EQ_Overlay itself (layout/eq_overlay.h) keeps its vftable as a plain field, and a derived class cannot add a
 * vfptr to it: VC6 would move the base to +4. This class has EQ_Overlay's layout and its client functions; each
 * derived class names its own vftable with `// EXTERNAL: <CLIENT> 0x<va> ??_7Class@@6B@`.
 */
#ifndef TRILOGY_VIRTUAL_OVERLAY_H
#define TRILOGY_VIRTUAL_OVERLAY_H

#include "trilogy/common.h"

class EQ_VirtualOverlay_guess
{
public:
    // EXTERNAL: TRILOGY 0x00450049 EQ_VirtualOverlay_guess::EQ_VirtualOverlay_guess
    EQ_VirtualOverlay_guess();
    // EXTERNAL: TRILOGY 0x0045005D EQ_VirtualOverlay_guess::~EQ_VirtualOverlay_guess
    ~EQ_VirtualOverlay_guess();

    virtual void slot0_guess() = 0;
    virtual void slot1_guess() = 0;
    virtual void Deactivate_guess() = 0;

    // EXTERNAL: TRILOGY 0x00450463 EQ_VirtualOverlay_guess::DelButtons
    void DelButtons();

    void*   displayOverlay_guess; /* 0x004 */
    int     enabled_guess;      /* 0x008 */
    int     redrawAll_guess;    /* 0x00C */
    u32     isActive_guess;     /* 0x010 */
    u8      unknown0x014[0x6C]; /* 0x014 */
};                              /* size 0x080 */

C_ASSERT(sizeof(EQ_VirtualOverlay_guess) == 0x080);

#endif /* TRILOGY_VIRTUAL_OVERLAY_H */
