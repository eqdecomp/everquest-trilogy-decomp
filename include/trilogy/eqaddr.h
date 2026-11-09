/*
 * eqaddr.h - the one way rewrites reach anything of eqgame.exe's that is not part of the rewrite:
 * through its fixed virtual address, never through an extern.
 *
 * Rewrites are linked on their own (the Unicorn test DLL, the hook DLL), so the client's globals, its
 * functions that are not rewritten yet and the eqgfx_dx7.dll entry points it keeps in pointers are all
 * named by a macro that expands to the VA. Every such macro uses one of these, always (type, va) in that
 * order, so the expansion is a plain cast of a constant and VC6 emits the same `mov eax, [va]`,
 * `call dword ptr [va]` or `call va` as the original:
 *
 *   EQ_GLOBAL(type, va)            the object at va, an lvalue:   *(type *)va
 *                                  #define g_pc_guess  EQ_GLOBAL(struct CharacterProfile *, 0x006E4DA8)
 *   EQ_ASM_GLOBAL(va)              the object at va as an __asm memory operand (the token must also appear in
 *                                  an EQ_GLOBAL or EQ_ARRAY, which declares its match symbol)
 *   EQ_ASM_CALL(reg, name, va), EQ_ASM_JMP(reg, name, va)  a client function from __asm: `call name` in a match
 *                                  build (name: an EXTERNAL-declared symbol), through reg elsewhere, because the
 *                                  DLL stub for name would clobber EAX, which EH prologues and epilogues use
 *   EQ_ARRAY(type, va)             the address va as a type *:    (type *)va   (arrays, objects used by
 *                                  address; a 2-D table takes a row typedef as `type`)
 *                                  #define g_ignore_list_guess  EQ_ARRAY(char, 0x006E24D4)
 *   EQ_CDECL(ret, va, (params))    a client function whose code is at va (direct `call va`)
 *   EQ_STDCALL(ret, va, (params))
 *   EQ_CDECL_CPP(ret, va, (params)) the same, for a C++ client function called from C++ code (keeps the
 *   EQ_STDCALL_CPP(ret, va, (params)) EH states around the call in a match build, see below)
 *   EQ_THISCALL(ret, va, (this_type, int, ...))
 *                                  a __thiscall method: VC6 has no __thiscall keyword, so it goes through
 *                                  __fastcall with a dummy EDX argument (pass 0); `this` lands in ECX and
 *                                  the callee pops the stack arguments, as with __thiscall:
 *                                  EQ_THISCALL(void, 0x004C4EE7, (CEverQuest *, int))(this, 0)
 *                                  C only: C++ uses EQ_METHOD, a real __thiscall call (no dummy EDX):
 *   EQ_METHOD(ret, va, (this_type, ...), obj)(...)
 *                                  EQ_METHOD(void, 0x004C4EE7, (CEverQuest *), this)()   (eqthiscall.h)
 *   EQ_FASTCALL(ret, va, (params)) a real __fastcall function: first two dword arguments in ECX, EDX
 *   function pointer variables     (eqgfx_dx7.dll entry points, run-time filled callbacks): a typedef for
 *                                  the pointer type plus EQ_GLOBAL, so a call is `call dword ptr [va]`:
 *                                  typedef float (__cdecl *t3dFloatSqrt_t)(float value);
 *                                  #define t3dFloatSqrt  EQ_GLOBAL(t3dFloatSqrt_t, 0x006E5258)
 *
 * Where these go: eqgfx.h for the eqgfx_dx7.dll pointers, the subsystem's own header for globals only it
 * uses (display.h, eqswitch.h), globals.h for the rest. When a function is rewritten its call sites keep
 * working (the hook redirects the VA); move its declaration to the rewrite's header then.
 *
 * Macros only, C and C++ (VC6). Not included from trilogy.h (build_gdt.py parses that one).
 */
#ifndef TRILOGY_EQADDR_H
#define TRILOGY_EQADDR_H

#ifdef EQD_MATCH
/* Match build (tools/compare/compare.py --match, objects only, never linked into a DLL): the client's
 * addresses become external symbols named after the VA as written, so a call compiles to `call rel32` and
 * a global access to an absolute address, each with a relocation, like the original. The compare tool
 * generates their declarations (eqd_match_syms.h, force-included) and maps the names back to VAs. */
#define EQ_GLOBAL(type, va)                 (*(type *)&eqd_va_##va)
#define EQ_ARRAY(type, va)                  ((type *)&eqd_va_##va)
#define EQ_FUNC(ret, conv, va, params)      ((ret (conv *) params)eqd_fn_##va)
#define EQ_ASM_GLOBAL(va)                   eqd_va_##va
#define EQ_ASM_CALL(reg, name, va)          call name
#define EQ_ASM_JMP(reg, name, va)           jmp name
#else
#define EQ_GLOBAL(type, va)                 (*(type *)(va))
#define EQ_ARRAY(type, va)                  ((type *)(va))
#define EQ_FUNC(ret, conv, va, params)      ((ret (conv *) params)(va))
#define EQ_ASM_GLOBAL(va)                   ds:[va]
#define EQ_ASM_CALL(reg, name, va)          mov reg, va __asm call reg
#define EQ_ASM_JMP(reg, name, va)           mov reg, va __asm jmp reg
#endif
#define EQ_CDECL(ret, va, params)           EQ_FUNC(ret, __cdecl, va, params)
#define EQ_STDCALL(ret, va, params)         EQ_FUNC(ret, __stdcall, va, params)
/* A real __fastcall function (the second argument is passed in EDX). */
#define EQ_FASTCALL(ret, va, params)        EQ_FUNC(ret, __fastcall, va, params)
/* params must start with (this_type, int): the int is the EDX slot, pass 0. C++ code uses EQ_METHOD. */
#define EQ_THISCALL(ret, va, params)        EQ_FUNC(ret, __fastcall, va, params)
/* EQ_CDECL_CPP / EQ_STDCALL_CPP: the same call, to a client function VC6 must treat as C++ (it may throw).
 * In a match build EQ_CDECL's target is an extern "C" declaration, and VC6 /GX takes extern "C" functions as
 * non-throwing: around such a call in C++ code it leaves out the EH state stores and may merge the argument
 * cleanup of neighbouring calls, which an original calling a C++ function keeps. The _CPP form names the
 * static member f of eqd_cc<va> instead (C++ linkage, only declared; the match build maps its decorated name
 * to va, compare/coff.py). Use it where the original calls a C++ function from C++ code with an EH frame;
 * outside a C++ match build it is EQ_CDECL / EQ_STDCALL. */
#if defined(EQD_MATCH) && defined(__cplusplus)
template<unsigned VA> struct eqd_cc { static void __cdecl f(void); };
#define EQ_FUNC_CPP(ret, conv, va, params)  ((ret (conv *) params)&eqd_cc<va>::f)
#else
#define EQ_FUNC_CPP(ret, conv, va, params)  EQ_FUNC(ret, conv, va, params)
#endif
#define EQ_CDECL_CPP(ret, va, params)       EQ_FUNC_CPP(ret, __cdecl, va, params)
#define EQ_STDCALL_CPP(ret, va, params)     EQ_FUNC_CPP(ret, __stdcall, va, params)
#ifdef __cplusplus
#include "eqthiscall.h"
#endif

#endif /* TRILOGY_EQADDR_H */
