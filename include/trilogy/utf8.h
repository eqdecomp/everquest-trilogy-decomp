/*
 * utf8.h - small types used by the functions the queue files under subsystem "EverQuest/utf8".
 *
 * CAUTION: that label is a call-graph guess (queue `subsystem_how` = "graph round N"); only
 * UTF8_strlwr (0x00404387) is Mac-mapped to utf8.cpp. The functions here sit at 0x00401000..0x00405FFF,
 * have no Mac/TAKP names, and are not string code. Their original file is unknown; move them (and these
 * types) once the real file is identified.
 *
 * Layouts: partial, from the rewritten functions only. C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_UTF8_H
#define TRILOGY_UTF8_H

#include "common.h"

/* 3 floats, returned by value (hidden result pointer) from Minus_guess. */
struct EQVector3_guess
{
    f32     x;                  /* 0x0 */
    f32     y;                  /* 0x4 */
    f32     z;                  /* 0x8 */

    /* 0x004053FB  name: manual (FUN_004053fb). this - rhs. */
    EQVector3_guess Minus_guess(const EQVector3_guess& rhs) const;
};
C_ASSERT(sizeof(EQVector3_guess) == 0x0C);

/* 12 dwords copied member by member (the caller reads +8 as a float). */
struct EQBlock48_guess
{
    f32     v[12];              /* 0x00 */

    /* 0x00403738  name: manual (FUN_00403738). Member-wise copy, returns nothing. */
    void    Set_guess(const EQBlock48_guess& src);
};
C_ASSERT(sizeof(EQBlock48_guess) == 0x30);

/* An object with a virtual method in vtable slot 2 (+8), taking no arguments. */
class EQSlot2Object_guess
{
public:
    virtual void Slot0_guess();
    virtual void Slot1_guess();
    virtual void Slot2_guess();
};

/* Holds an array of EQSlot2Object_guess* from begin (+0x0C) to end (+0x10). */
struct EQObjectList_guess
{
    u8      unknown0x00[0x0C];  /* 0x00 */
    EQSlot2Object_guess** begin;/* 0x0C */
    EQSlot2Object_guess** end;  /* 0x10 */

    /* 0x00403F3D  name: manual (FUN_00403f3d). Calls slot 2 on every element, in order. */
    void    CallSlot2OnAll_guess();
};
C_ASSERT(offsetof(EQObjectList_guess, begin) == 0x0C);
C_ASSERT(offsetof(EQObjectList_guess, end) == 0x10);

/* The object at +4 of BuildQuads_guess's `this`; its type is unknown. */
struct EQUnknownGfxOwner_guess
{
    /* 0x00402C83  name: manual (FUN_00402c83). Forwards to t3dDeferRect; `this` is unused. */
    void    DeferRect_guess(void* render_guess, u32* pRect_guess);
};

#endif /* TRILOGY_UTF8_H */
