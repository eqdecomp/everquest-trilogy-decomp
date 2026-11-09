/*
 * eq_overlay.h - EQ_Overlay, the old UI's overlay list (Mac: Source/PC Source/EverQuest/EQ_Overlay.cpp).
 *
 * EQ_Overlay and EQ_Main come from generated layout headers (layout/); most accesses are
 * still raw offsets (the element lists hang off +0x70, a RECT lives at element +4, Deactivate clears +0x10). OverlayElement_guess
 * itself (with its rectangle at +4) lives in eq_columns.h; do not redefine it here.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_OVERLAY_H
#define TRILOGY_EQ_OVERLAY_H

#include "common.h"
#include "eq_columns.h"


/* The class (members + methods) is generated from the layout data: layout/eq_overlay.h. */
#include "layout/eq_overlay.h"

/* These two ECX-first helpers operate on the existing OverlayElement_guess image through raw offsets. */
void __fastcall InitializeOverlayElement_guess(OverlayElement_guess* element); /* 0x00419572 */
void __fastcall drawButtonImage_guess(OverlayElement_guess* element);          /* 0x004195A9 */

/* ECX-first derived-overlay destruction thunks; concrete subtype names are unknown. */
void __fastcall EQ_Overlay_derivedDestructor_00450B34_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00450BD1_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00450C6E_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00450D0B_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00450DA8_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00450E45_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00450EE2_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00450F7F_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_0045101C_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_004510C5_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00451162_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_004511FF_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_0045129C_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00451339_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_0045143F_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00451B4C_guess(void* self);
void __fastcall EQ_Overlay_derivedDestructor_00451BE9_guess(void* self);

/* EQ_Main (members + methods, the S4b accessor pinstEQ_Main_guess) is generated from the layout data. */
#include "layout/eq_main.h"

#endif /* TRILOGY_EQ_OVERLAY_H */
