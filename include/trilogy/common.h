/*
 * common.h - basic types and compile-time checks for the Trilogy headers.
 *
 * Target compiler: Visual C++ 6.0 (C and C++). No <stdint.h>.
 * These headers are also parsed by Ghidra (tools\ghidra\scripts\build_gdt.py), which strips the
 * C_ASSERT lines and checks every asserted size and offset against its own layout instead.
 *
 * Conventions for all Trilogy headers:
 *   - every struct lists its offsets in comments and ends with C_ASSERT(sizeof(...) == ...);
 *   - key field offsets get C_ASSERT(offsetof(...) == ...) where the client code was checked;
 *   - each struct says where its layout comes from and whether it was checked against eqgame.exe
 *     ("verified against client ...") or not ("from references only");
 *   - "unknownXXXX" fields are named after their offset.
 * Addresses are eqgame.exe (2001-08-21) virtual addresses.
 */
#ifndef TRILOGY_COMMON_H
#define TRILOGY_COMMON_H

#include <stddef.h>     /* offsetof */

typedef signed char     s8;
typedef unsigned char   u8;
typedef signed short    s16;
typedef unsigned short  u16;
typedef signed long     s32;
typedef unsigned long   u32;
typedef float           f32;

/* Compile-time assertion (same shape as the Platform SDK's winnt.h definition). */
#ifndef C_ASSERT
#define C_ASSERT(e) typedef char __C_ASSERT__[(e) ? 1 : -1]
#endif

#endif /* TRILOGY_COMMON_H */
