// EQ_CreateBS.cpp - eqmain.dll, the DES block core at 0x10031CE0..0x10032282 (the subsystem and the function
// name are queue guesses: the body encrypts or decrypts one block, it registers nothing).
// Third-party code: this file reconstructs, from the binary, DES functions of libdes 3.24 (Copyright (C)
// 1995-1996 Eric Young) that eqmain.dll links. It is ALTERED: written from the compiled code, not the original
// libdes source (names follow libdes's). See THIRD_PARTY.md.

#include "trilogy/eqaddr.h"

// The 8 DES S-P tables of 64 words each, 256 bytes apart: eqmain_desSp0_guess at 0x100428A8 (eqmain_desSp0_guess) up to
// eqmain_desSp7_guess at 0x10042FA8 (eqmain_desSp7_guess). They follow unit_100312C0.cpp's eqmain_desSkb0..7_guess block.
// The names are guesses; each round ORs one word from each table into the round output.
#define eqmain_desSp0_guess  EQ_ARRAY(unsigned int, 0x100428A8)
#define eqmain_desSp1_guess  EQ_ARRAY(unsigned int, 0x100429A8)
#define eqmain_desSp2_guess  EQ_ARRAY(unsigned int, 0x10042AA8)
#define eqmain_desSp3_guess  EQ_ARRAY(unsigned int, 0x10042BA8)
#define eqmain_desSp4_guess  EQ_ARRAY(unsigned int, 0x10042CA8)
#define eqmain_desSp5_guess  EQ_ARRAY(unsigned int, 0x10042DA8)
#define eqmain_desSp6_guess  EQ_ARRAY(unsigned int, 0x10042EA8)
#define eqmain_desSp7_guess  EQ_ARRAY(unsigned int, 0x10042FA8)

// FUNCTION: EQMAIN 0x10031CE0
// DES (libdes des_encrypt): encrypts or decrypts one 64-bit block in place with a 16-round key schedule.
// __cdecl, void. One DES block in place: param_1 is the block's two words, schedule the 128-byte key schedule
// (16 pairs of words, round 1 first). encrypt nonzero encrypts with the pairs in order; 0 decrypts with them
// from the last pair back. Initial permutation, 16 rounds through eqmain_desSp0..7_guess, final permutation.
void __cdecl DesCryptBlock_guess(unsigned int *param_1, int schedule, int encrypt)
{
    unsigned int right;
    unsigned int oddBoxInput;
    unsigned int left;
    unsigned int keyedWord;
    unsigned int *subkeyPtr;
    int keyOffset;
    int nextOffset;
    unsigned int evenBoxInput;

    keyOffset = schedule;
    right = *param_1 & 0xf0f0f0f ^ param_1[1] >> 4 & 0xf0f0f0f;
    keyedWord = *param_1 ^ right;
    right = param_1[1] ^ right << 4;
    oddBoxInput = right & 0xffff ^ keyedWord >> 0x10;
    right = right ^ oddBoxInput;
    keyedWord = keyedWord ^ oddBoxInput << 0x10;
    oddBoxInput = keyedWord & 0x33333333 ^ right >> 2 & 0x33333333;
    keyedWord = keyedWord ^ oddBoxInput;
    right = right ^ oddBoxInput << 2;
    oddBoxInput = right & 0xff00ff ^ keyedWord >> 8 & 0xff00ff;
    right = right ^ oddBoxInput;
    keyedWord = keyedWord ^ oddBoxInput << 8;
    oddBoxInput = keyedWord & 0x55555555 ^ right >> 1 & 0x55555555;
    right = right ^ oddBoxInput * 2;
    left = right * 2 | right >> 0x1f;
    right = (keyedWord ^ oddBoxInput) >> 0x1f | (keyedWord ^ oddBoxInput) * 2;
    if (encrypt != 0) {
        schedule = 4;
        subkeyPtr = (unsigned int *)(keyOffset + 8);
        do {
            keyedWord = subkeyPtr[-2] ^ right;
            oddBoxInput = (subkeyPtr[-1] ^ right) >> 4;
            left = left ^ (eqmain_desSp6_guess[keyedWord >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | (subkeyPtr[-1] ^ right) << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[keyedWord >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[keyedWord >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[keyedWord & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            keyedWord = *subkeyPtr ^ left;
            oddBoxInput = (subkeyPtr[1] ^ left) >> 4;
            right = right ^ (eqmain_desSp6_guess[keyedWord >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | (subkeyPtr[1] ^ left) << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[keyedWord >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[keyedWord >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[keyedWord & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            keyedWord = subkeyPtr[2] ^ right;
            oddBoxInput = (subkeyPtr[3] ^ right) >> 4;
            left = left ^ (eqmain_desSp6_guess[keyedWord >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | (subkeyPtr[3] ^ right) << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[keyedWord >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[keyedWord >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[keyedWord & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            keyedWord = subkeyPtr[4] ^ left;
            oddBoxInput = (subkeyPtr[5] ^ left) >> 4;
            right = right ^ (eqmain_desSp6_guess[keyedWord >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | (subkeyPtr[5] ^ left) << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[keyedWord >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[keyedWord >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[keyedWord & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            schedule = schedule + -1;
            subkeyPtr = subkeyPtr + 8;
        } while (schedule != 0);
    }
    else {
        keyOffset = 0x78;
        do {
            evenBoxInput = *(unsigned int *)(keyOffset + schedule) ^ right;
            keyedWord = *(unsigned int *)(keyOffset + 4 + schedule) ^ right;
            oddBoxInput = keyedWord >> 4;
            left = left ^ (eqmain_desSp6_guess[evenBoxInput >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | keyedWord << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[evenBoxInput >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[evenBoxInput >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[evenBoxInput & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            evenBoxInput = *(unsigned int *)(keyOffset + -8 + schedule) ^ left;
            keyedWord = *(unsigned int *)(keyOffset + -4 + schedule) ^ left;
            oddBoxInput = keyedWord >> 4;
            right = right ^ (eqmain_desSp6_guess[evenBoxInput >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | keyedWord << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[evenBoxInput >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[evenBoxInput >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[evenBoxInput & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            evenBoxInput = *(unsigned int *)(keyOffset + -0x10 + schedule) ^ right;
            keyedWord = *(unsigned int *)(keyOffset + -0xc + schedule) ^ right;
            oddBoxInput = keyedWord >> 4;
            nextOffset = keyOffset + -0x20;
            left = left ^ (eqmain_desSp6_guess[evenBoxInput >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | keyedWord << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[evenBoxInput >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[evenBoxInput >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[evenBoxInput & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            evenBoxInput = *(unsigned int *)(keyOffset + -0x18 + schedule) ^ left;
            keyedWord = *(unsigned int *)(keyOffset + -0x14 + schedule) ^ left;
            oddBoxInput = keyedWord >> 4;
            right = right ^ (eqmain_desSp6_guess[evenBoxInput >> 0x18 & 0x3f] |
                             eqmain_desSp7_guess[(oddBoxInput | keyedWord << 0x1c) >> 0x18 & 0x3f] |
                             eqmain_desSp4_guess[evenBoxInput >> 0x10 & 0x3f] |
                             eqmain_desSp5_guess[(oddBoxInput & 0x3f0000) >> 0x10] |
                             eqmain_desSp2_guess[evenBoxInput >> 8 & 0x3f] |
                             eqmain_desSp3_guess[(oddBoxInput & 0x3f00) >> 8] |
                             eqmain_desSp0_guess[evenBoxInput & 0x3f] |
                             eqmain_desSp1_guess[oddBoxInput & 0x3f]);
            keyOffset = nextOffset;
        } while (0 < nextOffset);
    }
    oddBoxInput = ((left >> 1 | left << 0x1f) & 0x55555555) ^ (((right >> 1 | right << 0x1f) >> 1) & 0x55555555);
    left = (left >> 1 | left << 0x1f) ^ oddBoxInput;
    oddBoxInput = (right >> 1 | right << 0x1f) ^ oddBoxInput * 2;
    right = oddBoxInput & 0xff00ff ^ left >> 8 & 0xff00ff;
    oddBoxInput = oddBoxInput ^ right;
    left = left ^ right << 8;
    right = left & 0x33333333 ^ oddBoxInput >> 2 & 0x33333333;
    left = left ^ right;
    oddBoxInput = oddBoxInput ^ right << 2;
    right = oddBoxInput & 0xffff ^ left >> 0x10;
    oddBoxInput = oddBoxInput ^ right;
    left = left ^ right << 0x10;
    right = left & 0xf0f0f0f ^ oddBoxInput >> 4 & 0xf0f0f0f;
    *param_1 = right ^ left;
    param_1[1] = right << 4 ^ oddBoxInput;
    return;
}
