// unit_100312C0.cpp - eqmain.dll, the address run 0x100312C0..0x10031B40 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).
// Third-party code: this file reconstructs, from the binary, DES functions of libdes 3.24 (Copyright (C)
// 1995-1996 Eric Young) that eqmain.dll links. It is ALTERED: written from the compiled code, not the original
// libdes source (names follow libdes's). See THIRD_PARTY.md.

#include "trilogy/eqaddr.h"
#include <string.h>

// The 16 records of 8 bytes at 0x10049BD0..0x10049C4F, each keyed by its first 4 bytes. The loop's bound,
// 0x10049C50, is an immediate in the original, not a load.
#define eqmain_keyTable_guess  EQ_ARRAY(char, 0x10049BD0)

// The byte table at 0x10041FA8 (eqmain_byteTable_guess in the original), indexed by any byte value: 256 entries.
#define eqmain_byteTable_guess  EQ_ARRAY(unsigned char, 0x10041FA8)

// The 16 round shift counts at 0x10049C50 (eqmain_desShifts_guess in the original): 0 rotates by 1, else by 2. The loop's
// bound, 0x10049C90, is an immediate too.
#define eqmain_desShifts_guess  EQ_ARRAY(unsigned int, 0x10049C50)

// The 8 DES S-boxes of 64 words each, 256 bytes apart: eqmain_desSkb0_guess at 0x100420A8 (eqmain_desSkb0_guess) up to
// eqmain_desSkb7_guess at 0x100427A8 (eqmain_desSkb7_guess).
#define eqmain_desSkb0_guess  EQ_ARRAY(unsigned int, 0x100420A8)
#define eqmain_desSkb1_guess  EQ_ARRAY(unsigned int, 0x100421A8)
#define eqmain_desSkb2_guess  EQ_ARRAY(unsigned int, 0x100422A8)
#define eqmain_desSkb3_guess  EQ_ARRAY(unsigned int, 0x100423A8)
#define eqmain_desSkb4_guess  EQ_ARRAY(unsigned int, 0x100424A8)
#define eqmain_desSkb5_guess  EQ_ARRAY(unsigned int, 0x100425A8)
#define eqmain_desSkb6_guess  EQ_ARRAY(unsigned int, 0x100426A8)
#define eqmain_desSkb7_guess  EQ_ARRAY(unsigned int, 0x100427A8)

// The flag at 0x1013CA78 (eqmain_desCheckKey_guess in the original): nonzero makes 0x10031860 check the key first.
#define eqmain_desCheckKey_guess  EQ_GLOBAL(int, 0x1013CA78)

// The 8-byte DES key at 0x1013CA70 (eqmain_desKey_guess and DAT_1013ca74 in the original): 0x10031860 keys the schedule from
// a stack copy of it, and the same copy is the chain IV of desChainedCrypt_guess.
#define eqmain_desKey_guess  EQ_ARRAY(unsigned int, 0x1013CA70)

// FUNCTION: EQMAIN 0x100312C0
// Prepares the front end's shared key from the supplied text.
// __cdecl, void. Calls 0x10031340 with the caller's buffer and the global DES key (eqmain_desKey_guess) as its second argument.
void __cdecl desSetupWithGlobalKey_guess(unsigned int *input)
{
    EQ_CDECL(void, 0x10031340, (unsigned int *, unsigned int *))(input, eqmain_desKey_guess);
}

// FUNCTION: EQMAIN 0x100312E0
// CBC-encrypts len bytes from source to destination with the global DES key, which is also the IV.
// __cdecl, void. Copies the global DES key to the stack, keys a schedule from the copy (0x10031860), then runs
// desChainedCrypt_guess (0x10031450) with decrypt == 1 over `len` bytes from `source` to `destination`, chained from the copy.
void __cdecl desCryptGlobalKey_guess(unsigned int *source, unsigned char *destination, int len)
{
    unsigned int key[2];        // key[0] = local_88, key[1] = local_84: the stack copy of the key, also the chain IV
    unsigned int schedule[32];  // local_80: the key schedule, at key + 8

    key[0] = eqmain_desKey_guess[0];
    key[1] = eqmain_desKey_guess[1];
    EQ_CDECL(void, 0x10031860, (unsigned int *, unsigned int *))(key, schedule);
    EQ_CDECL(void, 0x10031450, (unsigned int *, unsigned char *, int, unsigned int *, unsigned int *, int))(
        source, destination, len, schedule, key, 1);
}

// FUNCTION: EQMAIN 0x10031340
// Derives the front end's eight-byte key from the supplied text.
// __cdecl, void. Folds the NUL-terminated string at input into the 8-byte key at key (cleared first): byte i
// goes into key[i % 8] shifted left one when i % 16 < 8, else its nibbles and bit pairs are swapped and it goes into
// key[7 - i % 8]. Then odd parity (0x10031800), the key schedule (0x10031860, with the flag at 0x1013CA78 cleared
// around it), 0x10031B40 over the string, the 128-byte schedule zeroed, and odd parity again.
// @early-stop: register allocation, signed-index lowering, and call-argument cleanup still differ under VC6 /O2.
void __cdecl desFoldKeyEncrypt_guess(unsigned char *input, unsigned int *keyBytes)
{
    unsigned char byteValue;
    unsigned char *keyBytePtr;
    unsigned int savedCheckFlag;
    unsigned int signMask;
    int keyByteIndex;
    int inputLength;
    unsigned int inputIndex;
    unsigned int schedule[32];

    *keyBytes = 0;
    keyBytes[1] = 0;
    inputIndex = 0;
    inputLength = strlen((char *)input);
    if (inputLength > 0) {
        do {
            byteValue = *(unsigned char *)(inputIndex + (int)input);
            signMask = (int)inputIndex >> 31;
            if ((int)(((inputIndex ^ signMask) - signMask & 0xf ^ signMask) - signMask) < 8) {
                keyByteIndex = ((inputIndex ^ signMask) - signMask & 7 ^ signMask) - signMask;
                *(unsigned char *)(keyByteIndex + (int)keyBytes) = *(unsigned char *)(keyByteIndex + (int)keyBytes) ^ byteValue << 1;
            }
            else {
                byteValue = (unsigned char)(byteValue << 4 | byteValue >> 4);
                byteValue = (byteValue >> 2 & 0x33) | (byteValue & 0xf3) << 2;
                keyByteIndex = ((inputIndex ^ signMask) - signMask & 7 ^ signMask) - signMask;
                keyBytePtr = (unsigned char *)((int)keyBytes + (7 - keyByteIndex));
                *keyBytePtr = *keyBytePtr ^ (byteValue >> 1 & 0x55 | (byteValue & 0xd5) << 1);
            }
            inputIndex = inputIndex + 1;
        } while ((int)inputIndex < inputLength);
    }
    EQ_CDECL(void, 0x10031800, (int))((int)keyBytes);
    savedCheckFlag = eqmain_desCheckKey_guess;
    eqmain_desCheckKey_guess = 0;
    EQ_CDECL(int, 0x10031860, (unsigned int *, unsigned int *))(keyBytes, schedule);
    eqmain_desCheckKey_guess = savedCheckFlag;
    EQ_CDECL(void, 0x10031B40, (unsigned char *, unsigned char *, int, unsigned int *, unsigned int *))(
        input, (unsigned char *)keyBytes, inputLength, schedule, keyBytes);
    memset(schedule, 0, sizeof(schedule));
    EQ_CDECL(void, 0x10031800, (int))((int)keyBytes);
    return;
}

// FUNCTION: EQMAIN 0x10031450
// Encrypts or decrypts front-end data blocks.
// __cdecl, void. DES over `len` bytes in a chained mode. decrypt == 0 encrypts (DES mode 0), anything else decrypts
// (mode 1). The 8-byte IV at `chainState` (little-endian) starts the chain and is left holding the last plaintext block.
// Encrypt: C = DES(P) ^ previous plaintext. Reads 8 bytes per block (past len on the last), writes the len bytes
// left. Decrypt: P = DES^-1(previous plaintext ^ C). Reads the len bytes left (zero-padded), writes 8 bytes per
// block (past len on the last).
void __cdecl desChainedCrypt_guess(unsigned char *in, unsigned char *out, int len, int schedule,
                                   unsigned char *chainState, int decrypt)
{
    unsigned int block[2];      // block[0] = local_8, block[1] = local_4: the DES block, changed in place
    unsigned int chainLo;
    unsigned int chainHi;
    unsigned int inLo;
    unsigned int inHi;
    unsigned int outLo;
    unsigned int outHi;

    if (decrypt)
    {
        chainLo = chainState[0] | (chainState[1] << 8) | (chainState[2] << 16) | (chainState[3] << 24);
        chainHi = chainState[4] | (chainState[5] << 8) | (chainState[6] << 16) | (chainState[7] << 24);
        while (len > 0)
        {
            if (len >= 8)
            {
                inLo = in[0] | (in[1] << 8) | (in[2] << 16) | (in[3] << 24);
                inHi = in[4] | (in[5] << 8) | (in[6] << 16) | (in[7] << 24);
                in += 8;
            }
            else
            {
                in += len;
                inHi = 0;
                inLo = 0;
                switch (len)
                {
                case 7:
                    inHi |= *--in << 16;
                case 6:
                    inHi |= *--in << 8;
                case 5:
                    inHi |= *--in;
                case 4:
                    inLo = *--in << 24;
                case 3:
                    inLo |= *--in << 16;
                case 2:
                    inLo |= *--in << 8;
                case 1:
                    inLo |= *--in;
                }
            }
            block[1] = chainHi ^ inHi;
            block[0] = chainLo ^ inLo;
            EQ_CDECL(void, 0x10031CE0, (unsigned int *, int, int))(block, schedule, 1);
            chainLo = block[0];
            chainHi = block[1];
            *out++ = (unsigned char)block[0];
            *out++ = (unsigned char)(block[0] >> 8);
            *out++ = (unsigned char)(block[0] >> 16);
            *out++ = (unsigned char)(block[0] >> 24);
            *out++ = (unsigned char)block[1];
            *out++ = (unsigned char)(block[1] >> 8);
            *out++ = (unsigned char)(block[1] >> 16);
            *out++ = (unsigned char)(block[1] >> 24);
            len -= 8;
        }
        chainState[0] = (unsigned char)chainLo;
        chainState[1] = (unsigned char)(chainLo >> 8);
        chainState[2] = (unsigned char)(chainLo >> 16);
        chainState[3] = (unsigned char)(chainLo >> 24);
        chainState[4] = (unsigned char)chainHi;
        chainState[5] = (unsigned char)(chainHi >> 8);
        chainState[6] = (unsigned char)(chainHi >> 16);
        chainState[7] = (unsigned char)(chainHi >> 24);
        return;
    }
    chainLo = chainState[0] | (chainState[1] << 8) | (chainState[2] << 16) | (chainState[3] << 24);
    chainHi = chainState[4] | (chainState[5] << 8) | (chainState[6] << 16) | (chainState[7] << 24);
    while (len > 0)
    {
        inLo = in[0] | (in[1] << 8) | (in[2] << 16) | (in[3] << 24);
        inHi = in[4] | (in[5] << 8) | (in[6] << 16) | (in[7] << 24);
        in += 8;
        block[0] = inLo;
        block[1] = inHi;
        EQ_CDECL(void, 0x10031CE0, (unsigned int *, int, int))(block, schedule, 0);
        outLo = block[0] ^ chainLo;
        outHi = block[1] ^ chainHi;
        if (len >= 8)
        {
            *out++ = (unsigned char)outLo;
            *out++ = (unsigned char)(outLo >> 8);
            *out++ = (unsigned char)(outLo >> 16);
            *out++ = (unsigned char)(outLo >> 24);
            *out++ = (unsigned char)outHi;
            *out++ = (unsigned char)(outHi >> 8);
            *out++ = (unsigned char)(outHi >> 16);
            *out++ = (unsigned char)(outHi >> 24);
        }
        else
        {
            out += len;
            switch (len)
            {
            case 7:
                *--out = (unsigned char)(outHi >> 16);
            case 6:
                *--out = (unsigned char)(outHi >> 8);
            case 5:
                *--out = (unsigned char)outHi;
            case 4:
                *--out = (unsigned char)(outLo >> 24);
            case 3:
                *--out = (unsigned char)(outLo >> 16);
            case 2:
                *--out = (unsigned char)(outLo >> 8);
            case 1:
                *--out = (unsigned char)outLo;
            }
        }
        len -= 8;
        chainLo = inLo;
        chainHi = inHi;
    }
    chainState[0] = (unsigned char)chainLo;
    chainState[1] = (unsigned char)(chainLo >> 8);
    chainState[2] = (unsigned char)(chainLo >> 16);
    chainState[3] = (unsigned char)(chainLo >> 24);
    chainState[4] = (unsigned char)chainHi;
    chainState[5] = (unsigned char)(chainHi >> 8);
    chainState[6] = (unsigned char)(chainHi >> 16);
    chainState[7] = (unsigned char)(chainHi >> 24);
    return;
}

// FUNCTION: EQMAIN 0x10031800
// Prepares key bytes for front-end data protection; the table's exact role is unclear.
// __cdecl, void. Maps each of the 8 bytes at key through eqmain_byteTable_guess, in place. The 8-byte shape
// suggests a DES key; the table is likely odd parity (not checked).
void __cdecl desSetOddParity_guess(unsigned char *key)
{
    unsigned int i;

    i = 0;
    do {
        key[i] = eqmain_byteTable_guess[key[i]];
        ++i;
    } while (i < 8);
    return;
}

// FUNCTION: EQMAIN 0x10031820
// Checks whether a candidate front-end key matches an entry in the built-in key table.
// @early-stop: VC6 /O2 expands memcmp(.., 4) to dword compares, not the original's repe cmpsb (ECX=4, saves EBX/ESI/EDI).
// __cdecl. 1 when the 4 bytes at bytes equal the key of one of the 16 table records, else 0. The caller at
// 0x10031860 reads EAX after the call.
int __cdecl IsTableKey4_guess(char *keyBytes)
{
    int recordAddress;

    // The start is the integer 0x10049BD0 (the table's first record), not eqmain_keyTable_guess: VC6 folds the
    // entry test on an integer constant only, which leaves the bound test at the bottom as in the original. The 16
    // records end exactly at 0x10049C50, so the walk can stop on != as well as on <.
    for (recordAddress = 0x10049BD0; recordAddress != 0x10049C50; recordAddress = recordAddress + 8) {
        if (memcmp((char *)recordAddress, keyBytes, 4) == 0)
            return 1;
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10031860
// Prepares the DES round keys used to protect front-end data.
// __cdecl. libdes-style des_set_key (the string "..." sits at 0x10049C90): expands the 8-byte key at
// keyData into the 16 round records at schedule (8 bytes each, 0x80 in all). With eqmain_desCheckKey_guess nonzero,
// 0x10031B10 must pass (each key byte self-mapped, else -1) and 0x10031820 must fail (weak key, else -2). Returns 0
// once the schedule is written. The callers at 0x100312E0 and 0x10031340 don't read EAX after the call.
int __cdecl DesSetKey_guess(unsigned int *keyData, unsigned int *schedule)
{
    int validationResult;
    unsigned int bitTemp;
    unsigned int lookupBits;
    unsigned int leftKeyHalf;
    unsigned int roundKeyPartA;
    unsigned int roundKeyPartB;
    unsigned int rightKeyHalf;
    unsigned char *keyBytePtr;

    if (eqmain_desCheckKey_guess != 0) {
        validationResult = EQ_CDECL(int, 0x10031B10, (unsigned char *))((unsigned char *)keyData);
        if (validationResult == 0) {
            return 0xffffffff;
        }
        validationResult = EQ_CDECL(int, 0x10031820, (char *))((char *)keyData);
        if (validationResult != 0) {
            return 0xfffffffe;
        }
    }
    keyBytePtr = (unsigned char *)keyData;
    leftKeyHalf = keyBytePtr[0] | keyBytePtr[1] << 8 | keyBytePtr[2] << 16 | keyBytePtr[3] << 24;
    roundKeyPartA = keyBytePtr[4] | keyBytePtr[5] << 8 | keyBytePtr[6] << 16 | keyBytePtr[7] << 24;
    bitTemp = leftKeyHalf & 0xf0f0f0f ^ roundKeyPartA >> 4 & 0xf0f0f0f;
    leftKeyHalf = leftKeyHalf ^ bitTemp;
    roundKeyPartA = roundKeyPartA ^ bitTemp << 4;
    bitTemp = leftKeyHalf & 0xcccc0000 ^ (leftKeyHalf & 0xfffff333) << 0x12;
    leftKeyHalf = leftKeyHalf ^ bitTemp >> 0x12 ^ bitTemp;
    bitTemp = roundKeyPartA & 0xcccc0000 ^ (roundKeyPartA & 0xfffff333) << 0x12;
    roundKeyPartA = roundKeyPartA ^ bitTemp >> 0x12 ^ bitTemp;
    bitTemp = leftKeyHalf & 0x55555555 ^ roundKeyPartA >> 1 & 0x55555555;
    leftKeyHalf = leftKeyHalf ^ bitTemp;
    roundKeyPartA = roundKeyPartA ^ bitTemp * 2;
    bitTemp = roundKeyPartA & 0xff00ff ^ leftKeyHalf >> 8 & 0xff00ff;
    roundKeyPartA = roundKeyPartA ^ bitTemp;
    leftKeyHalf = leftKeyHalf ^ bitTemp << 8;
    bitTemp = leftKeyHalf & 0x55555555 ^ roundKeyPartA >> 1 & 0x55555555;
    leftKeyHalf = leftKeyHalf ^ bitTemp;
    roundKeyPartA = roundKeyPartA ^ bitTemp * 2;
    bitTemp = (leftKeyHalf & 0xf000000f | roundKeyPartA >> 0xc & 0xff0) >> 4 | roundKeyPartA & 0xff00 | (roundKeyPartA & 0xff) << 0x10;
    leftKeyHalf = leftKeyHalf & 0xfffffff;
    keyData = eqmain_desShifts_guess;
    do {
        if (*keyData == 0) {
            roundKeyPartA = leftKeyHalf >> 1 | leftKeyHalf << 0x1b;
            roundKeyPartB = bitTemp >> 1;
            rightKeyHalf = bitTemp << 0x1b;
        }
        else {
            roundKeyPartA = leftKeyHalf >> 2 | leftKeyHalf << 0x1a;
            roundKeyPartB = bitTemp >> 2;
            rightKeyHalf = bitTemp << 0x1a;
        }
        leftKeyHalf = roundKeyPartA & 0xfffffff;
        rightKeyHalf = roundKeyPartB | rightKeyHalf;
        lookupBits = leftKeyHalf >> 1;
        bitTemp = rightKeyHalf & 0xfffffff;
        roundKeyPartB = eqmain_desSkb3_guess[((lookupBits & 0x7000000 | roundKeyPartA & 0xc00000) >> 1 | roundKeyPartA & 0x100000) >> 0x14]
                | eqmain_desSkb2_guess[(roundKeyPartA & 0x1e000 | lookupBits & 0x60000) >> 0xd]
                | eqmain_desSkb1_guess[(lookupBits & 0xf00 | roundKeyPartA & 0xc0) >> 6]
                | eqmain_desSkb0_guess[roundKeyPartA & 0x3f];
        roundKeyPartA = eqmain_desSkb5_guess[(bitTemp >> 1 & 0x1e00 | rightKeyHalf & 0x180) >> 7]
                | eqmain_desSkb7_guess[(bitTemp >> 1 & 0x6000000 | rightKeyHalf & 0x1e00000) >> 0x15]
                | eqmain_desSkb6_guess[bitTemp >> 0xf & 0x3f]
                | eqmain_desSkb4_guess[rightKeyHalf & 0x3f];
        schedule[0] = roundKeyPartB & 0xffff | roundKeyPartA << 0x10;
        schedule[1] = roundKeyPartA >> 0x1c | (roundKeyPartA & 0xffff0000 | roundKeyPartB >> 0x10) << 4;
        keyData = keyData + 1;
        schedule = schedule + 2;
    } while ((int)keyData < 0x10049c90);
    return 0;
}

// FUNCTION: EQMAIN 0x10031B10
// Validates a candidate key using the front end's byte-mapping table.
// @early-stop: VC6 /O2 places both `pop esi` instructions before the return-value constants; the original emits each constant first.
// __cdecl. 1 when each of the 8 bytes at keyBytes equals its own entry in eqmain_byteTable_guess (the entry
// indexed by that byte), else 0. The caller at 0x10031860 (call at 0x10031872) reads EAX after the call.
int __cdecl IsSelfMapped8_guess(unsigned char *keyBytes)
{
    unsigned int index;

    index = 0;
    while (index < 8) {
        if (keyBytes[index] != eqmain_byteTable_guess[keyBytes[index]])
            return 0;
        index = index + 1;
    }
    return 1;
}

// FUNCTION: EQMAIN 0x10031B40
// Produces the checksum used to finish front-end key setup.
// __cdecl. DES CBC checksum in the shape of libdes's des_cbc_cksum (the names are guesses). The 8-byte chain starts
// at ivec, read byte by byte little-endian. Each 8-byte block of input is XORed into the chain and encrypted by
// 0x10031CE0 (des_encrypt, mode 1, key schedule `schedule`). A short last block is zero-padded: its bytes are read
// backwards from its end (libdes c2ln; case 8 is in the switch but a block is never 8 bytes here). When output is
// not NULL the final chain is written there, 8 bytes little-endian. Returns nothing.
// @early-stop: register allocation and byte-load scheduling still differ under VC6 /O2.
void __cdecl des_cbc_cksum_guess(unsigned char *input, unsigned char *result, int inputLength, unsigned int *schedule,
                                 unsigned char *initialVector)
{
    unsigned int tout0;
    unsigned int tout1;
    unsigned int tin0;
    unsigned int tin1;
    unsigned int tin[2];
    int l;
    unsigned char *in;
    unsigned char *out;
    unsigned char *iv;

    l = inputLength;
    in = input;
    out = result;
    iv = initialVector + 4;
    tout1 = (unsigned int)*iv++;
    tout1 |= (unsigned int)*iv++ << 8;
    tout1 |= (unsigned int)*iv++ << 16;
    tout1 |= (unsigned int)*iv++ << 24;
    iv = initialVector;
    tout0 = (unsigned int)*iv++;
    tout0 |= (unsigned int)*iv++ << 8;
    tout0 |= (unsigned int)*iv++ << 16;
    tout0 |= (unsigned int)*iv++ << 24;

    for (; l > 0; l -= 8)
    {
        if (l >= 8)
        {
            tin0 = (unsigned int)*in++;
            tin0 |= (unsigned int)*in++ << 8;
            tin0 |= (unsigned int)*in++ << 16;
            tin0 |= (unsigned int)*in++ << 24;
            tin1 = (unsigned int)*in++;
            tin1 |= (unsigned int)*in++ << 8;
            tin1 |= (unsigned int)*in++ << 16;
            tin1 |= (unsigned int)*in++ << 24;
        }
        else
        {
            in += l;
            tin0 = 0;
            tin1 = 0;
            switch (l)
            {
            case 8:
                tin1 = (unsigned int)*(--in) << 24;
            case 7:
                tin1 |= (unsigned int)*(--in) << 16;
            case 6:
                tin1 |= (unsigned int)*(--in) << 8;
            case 5:
                tin1 |= (unsigned int)*(--in);
            case 4:
                tin0 = (unsigned int)*(--in) << 24;
            case 3:
                tin0 |= (unsigned int)*(--in) << 16;
            case 2:
                tin0 |= (unsigned int)*(--in) << 8;
            case 1:
                tin0 |= (unsigned int)*(--in);
            }
        }
        tin1 ^= tout1;
        tin[1] = tin1;
        tin0 ^= tout0;
        tin[0] = tin0;
        EQ_CDECL(void, 0x10031CE0, (unsigned int *, unsigned int *, int))(tin, schedule, 1);
        tout0 = tin[0];
        tout1 = tin[1];
    }
    if (out != 0)
    {
        *out++ = (unsigned char)(tout0 & 0xff);
        *out++ = (unsigned char)((tout0 >> 8) & 0xff);
        *out++ = (unsigned char)((tout0 >> 16) & 0xff);
        *out++ = (unsigned char)((tout0 >> 24) & 0xff);
        *out++ = (unsigned char)(tout1 & 0xff);
        *out++ = (unsigned char)((tout1 >> 8) & 0xff);
        *out++ = (unsigned char)((tout1 >> 16) & 0xff);
        *out++ = (unsigned char)((tout1 >> 24) & 0xff);
    }
}
