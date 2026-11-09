/*
 * vlong.h - the client's arbitrary-precision unsigned integer (0x004E5F06 .. 0x004E6F03), used for the
 * session key exchange (GenerateSessionKeys_guess 0x004A055E). Its layout and member set match the
 * freely published "vlong" RSA class (flex_unit / vlong_value / vlong); names below follow that shape
 * with a `_guess` suffix on the classes. Layout verified against the client's code (offsets in the
 * functions at the addresses given); no Mac symbols for any of it (the Mac map labels this range DPVS,
 * which Trilogy doesn't link).
 *
 * All members are __thiscall and non-inline (defined in src/EverQuest/EQMisc.cpp).
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_VLONG_H
#define TRILOGY_VLONG_H

#include "common.h"

/* An array of 32-bit units, little-endian (unit 0 lowest). */
class flex_unit_guess
{
public:
    unsigned* a;            /* 0x00 units */
    unsigned z;             /* 0x04 units allocated */
    unsigned n;             /* 0x08 units used (a[n - 1] != 0 after set) */

    unsigned get(unsigned i) const;                                        /* 0x004E5F06 */
    void clear();                                                          /* 0x004E5F1B */
    flex_unit_guess();                                                     /* 0x004E5F20 */
    ~flex_unit_guess();                                                    /* 0x004E5F2D */
    void reserve(unsigned x);                                              /* 0x004E5F3B */
    void set(unsigned i, unsigned x);                                      /* 0x004E5F7F */
    void fast_mul(flex_unit_guess& x, flex_unit_guess& y, unsigned keep);  /* 0x004E5FE9 */
};
C_ASSERT(sizeof(flex_unit_guess) == 0x0C);

/* A reference-counted value (share = number of extra owners). */
class vlong_value_guess : public flex_unit_guess
{
public:
    unsigned share;         /* 0x0C */

    int is_zero() const;                                                   /* 0x004E6179 */
    int test(unsigned i) const;                                            /* 0x004E6182 */
    unsigned bits() const;                                                 /* 0x004E61A6 */
    int cf(vlong_value_guess& x) const;                                    /* 0x004E61CE */
    void shl();                                                            /* 0x004E6228 */
    void shr();                                                            /* 0x004E6263 */
    void shr_by(unsigned x);                                               /* 0x004E6298 (shr(unsigned)) */
    void add(vlong_value_guess& x);                                        /* 0x004E62FF */
    void subtract(vlong_value_guess& x);                                   /* 0x004E636E */
    void init(unsigned x);                                                 /* 0x004E63C0 */
    void copy(vlong_value_guess& x);                                       /* 0x004E63D9 */
    void divide(vlong_value_guess& x, vlong_value_guess& y, vlong_value_guess& rem); /* 0x004E6447 */
    vlong_value_guess();                                                   /* 0x004E6408 */
    void mul(vlong_value_guess& x, vlong_value_guess& y);                  /* 0x004E6418 */
    inline ~vlong_value_guess();                                           /* 0x004E6529 */
};
C_ASSERT(sizeof(vlong_value_guess) == 0x10);

/* A signed handle on a shared value. */
class vlong_guess
{
public:
    vlong_value_guess* value;   /* 0x00 */
    int negative;               /* 0x04 */

    void docopy();                                                         /* 0x004E652E */
    int cf(vlong_guess x) const;                                           /* 0x004E6566 (x by value) */
    vlong_guess(unsigned x);                                               /* 0x004E65E1 */
    vlong_guess(const vlong_guess& x);                                     /* 0x004E6612 */
    vlong_guess& assign(vlong_guess x);                                    /* 0x004E6628 operator= (x by value) */
    ~vlong_guess();                                                        /* 0x004E6687 */
    vlong_guess& add_assign(const vlong_guess& x);                         /* 0x004E66AB operator+= */
    vlong_guess& sub_assign(const vlong_guess& x);                         /* 0x004E673F operator-= */
};
C_ASSERT(sizeof(vlong_guess) == 0x08);

/* Free operators and helpers (all __cdecl, the result through a hidden first pointer). The library's
 * operators are named functions here (and assign/add_assign/sub_assign above): the tools' FUNCTION annotation
 * cannot name an `operator` definition. Each comment gives the operator it stands for. */
vlong_guess vlong_mul_guess(const vlong_guess& x, const vlong_guess& y);    /* 0x004E6835 operator* */
vlong_guess vlong_div_guess(const vlong_guess& x, const vlong_guess& y);    /* 0x004E68A1 operator/ */
vlong_guess vlong_mod_guess(const vlong_guess& x, const vlong_guess& y);    /* 0x004E6928 operator% */
vlong_guess vlong_sub_guess(const vlong_guess& x, const vlong_guess& y);    /* 0x004E67DC operator- */
int vlong_ne_guess(const vlong_guess& x, const vlong_guess& y);           /* 0x00415415 operator!= */
int vlong_lt_guess(const vlong_guess& x, const vlong_guess& y);           /* 0x0041545D operator< */
int vlong_ge_guess(const vlong_guess& x, const vlong_guess& y);           /* 0x004E6D8C operator>= */
vlong_guess modinv_guess(const vlong_guess& a, const vlong_guess& m);      /* 0x004E69A9 */
vlong_guess modexp_guess(const vlong_guess& x, const vlong_guess& e, const vlong_guess& m); /* 0x004E6EB0 */

/* Montgomery modular multiplication/exponentiation context (the "monty" class of the vlong library). */
class monty_guess
{
public:
    vlong_guess R;          /* 0x00 */
    vlong_guess R1;         /* 0x08 */
    vlong_guess m;          /* 0x10 */
    vlong_guess n1;         /* 0x18 */
    vlong_guess T;          /* 0x20 */
    vlong_guess k;          /* 0x28 */
    unsigned N;             /* 0x30 bits of R */

    void mul(vlong_guess& x, const vlong_guess& y);                        /* 0x004E6D0F */
    vlong_guess exp(const vlong_guess& x, const vlong_guess& e);           /* 0x004E6DB2 */
    monty_guess(const vlong_guess& M);                                     /* 0x004E6BE3 */
    ~monty_guess();                                                        /* 0x004E6F03 */
};
C_ASSERT(sizeof(monty_guess) == 0x34);

/* An RSA public key (modulus, exponent). */
class vlong_public_key_guess
{
public:
    vlong_guess m;          /* 0x00 */
    vlong_guess e;          /* 0x08 */

    vlong_guess encrypt(const vlong_guess& plain);                         /* 0x0041543B */
};
C_ASSERT(sizeof(vlong_public_key_guess) == 0x10);

#endif /* TRILOGY_VLONG_H */
