// unit_1004E6D2.c - eqgfx_dx7.dll, the address run 0x1004E6D2..0x1004F96F (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#define EQ_NAKED  __declspec(naked)

#include "trilogy/eqaddr.h"

#define t3d_angle_tree_guess       EQ_GLOBAL(unsigned char *, 0x100F3FF4)  // start-up block; tree root at +0x6680
#define EQGFX_EFFECT_ANGLE_WRAP_AMOUNT            EQ_GLOBAL(float, 0x100762FC)
#define t3d_half_constant          EQ_GLOBAL(float, 0x10076290)            // 0.5f in .rdata: the power table's ratio
#define EQGFX_ZERO_F_GUESS          EQ_GLOBAL(float, 0x10076284)            // 0.0f in .rdata: keyDivisor's zero test
#define t3d_angle_units_per_degree EQ_GLOBAL(float, 0x100763E4)            // 1.4222224f in .rdata: 512 table units per 360 degrees
#define T3D_RANDOM_SCALE_FIRST_ADDRESS EQ_GLOBAL(double, 0x100763D0)
#define T3D_RANDOM_SCALE_SECOND_ADDRESS EQ_GLOBAL(double, 0x100763C8)
#define t3d_cosine_angle_step           EQ_GLOBAL(double, 0x100763C0)
#define T3D_COSINE_SAMPLE_SCALE_ADDRESS EQ_GLOBAL(double, 0x10076388)

typedef struct t3dAngleNode_guess
{
    int           sineOverCos;                  // 0x00, (sineTable[i] << 16) / cosTable[i], 16.16 fixed point
    int           cosOverSine;                  // 0x04, (cosTable[i] << 16) / sineTable[i]; node 0 holds T3D_FIXED_INFINITY
    float         key;                          // 0x08, keyDividend[i] / keyDivisor[i]; large bits when keyDivisor[i] is 0
    float         ratio;                        // 0x0c, keyDivisor[i] / keyDividend[i]; node 0 holds large bits
    struct t3dAngleNode_guess *lower;           // 0x10, taken when ratio < key
    struct t3dAngleNode_guess *higher;          // 0x14, taken when ratio > key
} t3dAngleNode_guess;                           // 0x18

#define T3D_ANGLE_NODE_LIMIT  0x80              // the angle tree's node array: indices [0, 0x7f] are valid
#define T3D_FIXED_INFINITY    0x7fffffff        // 16.16 infinity: node 0's cosOverSine
#define T3D_LARGE_FLOAT_BITS  0x7149F2CA        // float bits of a large sentinel (about 1e30)

typedef struct t3dMatrix3x3_guess
{
    float m[9];                                 // row-major: element (row, column) at m[row * 3 + column]
} t3dMatrix3x3_guess;                           // 0x24

typedef struct t3dMatrix4x3_guess
{
    float m[12];                                // row-major 4x3: element (row, column) at m[row * 3 + column]; row 3 is the translation
} t3dMatrix4x3_guess;                           // 0x30

typedef struct t3dPowerEntry_guess
{
    int           pow2;                         // 0x00, 2^k: 1, 2, 4, ... 0x400
    int           odd;                          // 0x04, 2k - 1: 1, 3, 5, ... (entry 0 holds 0)
    float         halfPow;                      // 0x08, 0.5^k: 1.0, 0.5, 0.25, ...
} t3dPowerEntry_guess;                          // 0x0c

typedef struct t3dStartupBlock_guess
{
    int                  cosTable[0x200];       // 0x0000, 512 ints; partner of the sine table (name unconfirmed)
    int                  sineTable[0x200];      // 0x0800, 512 ints, the sine table (T3D_SINE_TABLE_OFFSET)
    union
    {
        float            keyDivisor[0x80];      // 0x1000, 128 floats
        float            cosineInterpolationTable[0x200]; // same base, 512 float samples (t3dFloatFastCosine)
    };
    union
    {
        float            keyDividend[0x80];     // 0x1800, 128 floats
        float            sineInterpolationTable[0x200]; // same base, 512 float samples
    };
    short                firstTable[0x1000];    // 0x2000, 4096 shorts: initializeShortTablePair_guess's FUN_10069984(i) * scale (T3D_FIRST_TABLE_OFFSET)
    short                secondTable[0x1000];   // 0x4000, 4096 shorts: initializeShortTablePair_guess's FUN_10069984(2i) * scale (T3D_SECOND_TABLE_OFFSET)
    unsigned char        reciprocal_table[0x80]; // 0x6000, one byte per index 0..0x7f, filled by initializeReciprocalTable_guess
    t3dAngleNode_guess   nodes[T3D_ANGLE_NODE_LIMIT]; // 0x6080, the angle tree's node array (root at +0x6680)
    int                  randomValues[0x400][2];
    t3dPowerEntry_guess  powers[11];            // 0x8c80, entries 0..10; the table ends at 0x8d04
} t3dStartupBlock_guess;                        // 0x8d04

#define T3D_SINE_TABLE_OFFSET     0x800                                    // 512 int entries
#define T3D_SINE_INDEX_MASK       0x1ff

// FUNCTION: EQGFX 0x1004e6d2
// __cdecl, int: the sine table entry for the angle's bits 16..24 (the integer part of a fix16 angle).
int __cdecl t3dFix16FastSine(int angle)
{
    unsigned char *startupBlock = t3d_angle_tree_guess;
    int index = (angle >> 0x10) & T3D_SINE_INDEX_MASK;
    return *(int *)(startupBlock + T3D_SINE_TABLE_OFFSET + index * 4);
}

// FUNCTION: EQGFX 0x1004e6ec
// __cdecl, int: the same table index as the sine, but read from the start-up block's +0x0 (no +0x800 offset).
int __cdecl t3dFix16FastCosine(int angle)
{
    unsigned char *startupBlock = t3d_angle_tree_guess;
    int index = (angle >> 0x10) & T3D_SINE_INDEX_MASK;
    return *(int *)(startupBlock + index * 4);
}

// FUNCTION: EQGFX 0x1004e702
// Gets the cotangent of a fixed-point angle for renderer orientation math.
// __cdecl, int: cotangent of a 16.16 angle from the angle tree's node array; quadrant bits 0x80/0x100 negate the mirrored node.
int __cdecl t3dFix16FastCotangent(int angle)
{
    int index = (angle >> 0x10) & T3D_SINE_INDEX_MASK;
    int quadrantBits = index & 0x180;
    if (quadrantBits != 0x100 && quadrantBits != 0x80)
        return ((t3dStartupBlock_guess *)t3d_angle_tree_guess)->nodes[index & 0x7f].cosOverSine;
    return -((t3dStartupBlock_guess *)t3d_angle_tree_guess)->nodes[(~index) & 0x7f].cosOverSine;
}

#define T3D_FIRST_TABLE_OFFSET    0x2000        // startup block's firstTable: 4096 shorts
#define T3D_SECOND_TABLE_OFFSET   0x4000        // startup block's secondTable: 4096 shorts
#define T3D_SHORT_TABLES_END      0x6000        // the reciprocal table's offset: the fill loop's end
#define T3D_FIRST_SCALE_ADDRESS EQ_GLOBAL(double, 0x100763B8)     // .rdata double: scales FUN_10069984(i)
#define T3D_SECOND_SCALE_ADDRESS EQ_GLOBAL(double, 0x100763B0)     // .rdata double: scales FUN_10069984(2i)
#define T3D_ST0_TO_SHORT          EQ_CDECL(short, 0x1004EC36, (void))  // ST0 to integer; the stored word is AX

#define T3D_SAFE_DIV_CALL_OUTPUT_DEBUG_STRING_A \
    __asm _emit 0xff __asm _emit 0x15 __asm _emit 0xc4 __asm _emit 0x60 __asm _emit 0x07 __asm _emit 0x10  // call [0x100760c4]: OutputDebugStringA's import slot, without the assembler's DS prefix
#define T3D_SAFE_DIV_ZERO_MESSAGE           100a355ch      // "..."
#define T3D_SAFE_DIV_POSITIVE_MESSAGE       100a3534h      // "..."
#define T3D_SAFE_DIV_NUMERATOR_MESSAGE      100a3508h      // "..."
#define T3D_SAFE_DIV_DENOMINATOR_MESSAGE    100a34d8h      // "..."
#define T3D_SAFE_DIV_NEGATIVE_MESSAGE       100a34b0h      // "..."

// FUNCTION: EQGFX 0x1004e752
// Divides fixed-point quantities with saturation for overflow and division by zero.
// __cdecl, int: (numerator << 16) / denominator in 16.16, hand-written asm with PUSHAD/POPAD around each OutputDebugStringA.
// Overflow and divide by zero saturate to 0x7fffffff / 0x80000001; EDX:EAX is the full result, callers read EAX.
EQ_NAKED int __cdecl fixedPointSafeDivide_guess(int numerator, int denominator)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 0x8]
        mov ebx, dword ptr [ebp + 0xc]
        or ebx, ebx
        jnz denominatorNonZero
        pushad
        push T3D_SAFE_DIV_ZERO_MESSAGE
        T3D_SAFE_DIV_CALL_OUTPUT_DEBUG_STRING_A
        popad
        or eax, eax
        js divideByZeroNegative
        jz done
        mov eax, 0x7fffffff
        jmp done
divideByZeroNegative:
        mov eax, 0x80000001
        jmp done
denominatorNonZero:
        or eax, eax
        jz done
        mov ecx, eax
        or ecx, ebx
        js eitherNegative
        test ebx, 0xffff0000
        jnz shiftDivide
        test eax, 0xffff8000
        jz shiftDivide
        cmp eax, 0x1000000
        jnc positiveCheck
        cmp ebx, 0x200
        jnc shiftDivide
positiveCheck:
        mov esi, eax
        xor edx, edx
        idiv ebx
        test eax, 0xffff8000
        mov eax, esi
        jz shiftDivide
        pushad
        push T3D_SAFE_DIV_POSITIVE_MESSAGE
        T3D_SAFE_DIV_CALL_OUTPUT_DEBUG_STRING_A
        popad
        mov eax, 0x7fffffff
        jmp done
eitherNegative:
        mov esi, eax
        mov edi, ebx
        or eax, eax
        jns numeratorPositive
        neg eax
        jno numeratorPositive
        pushad
        push T3D_SAFE_DIV_NUMERATOR_MESSAGE
        T3D_SAFE_DIV_CALL_OUTPUT_DEBUG_STRING_A
        popad
        mov eax, 0x7fffffff
        mov esi, 0x80000001
numeratorPositive:
        or ebx, ebx
        jns denominatorPositive
        neg ebx
        jno denominatorPositive
        pushad
        push T3D_SAFE_DIV_DENOMINATOR_MESSAGE
        T3D_SAFE_DIV_CALL_OUTPUT_DEBUG_STRING_A
        popad
        mov ebx, 0x7fffffff
        mov edi, 0x80000001
denominatorPositive:
        test ebx, 0xffff0000
        jnz signedRestore
        test eax, 0xffff8000
        jz signedRestore
        xor edx, edx
        idiv ebx
        test eax, 0xffff8000
        jz signedRestore
        xor esi, edi
        js negativeOverflow
        pushad
        push T3D_SAFE_DIV_POSITIVE_MESSAGE
        T3D_SAFE_DIV_CALL_OUTPUT_DEBUG_STRING_A
        popad
        mov eax, 0x7fffffff
        jmp done
negativeOverflow:
        pushad
        push T3D_SAFE_DIV_NEGATIVE_MESSAGE
        T3D_SAFE_DIV_CALL_OUTPUT_DEBUG_STRING_A
        popad
        mov eax, 0x80000001
        jmp done
signedRestore:
        mov eax, esi
        mov ebx, edi
shiftDivide:
        mov edx, eax
        sar edx, 0x10
        shl eax, 0x10
        idiv ebx
done:
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// FUNCTION: EQGFX 0x1004e88a
// Builds two renderer math lookup tables; their specific use is unclear.
// __cdecl, int: firstTable[i] = FUN_10069984(i) and secondTable[i] = FUN_10069984(2i), each times a .rdata double and rounded by t3dFloatTruncateToInt_guess,
// for i in [0, 0xfff]. Always returns 1.
int __cdecl initializeShortTablePair_guess(void)
{
    unsigned int index = 0;
    unsigned int entryOffset = T3D_SECOND_TABLE_OFFSET;
    short entry;

    do
    {
        // FUN_10069984 leaves its float in ST0; the asm scales it there, t3dFloatTruncateToInt_guess reads it.
        EQ_CDECL(void, 0x10069984, (double))((double)index);
        __asm
        {
            fmul qword ptr EQ_ASM_GLOBAL(0x100763B8)
        }
        entry = T3D_ST0_TO_SHORT();
        *(short *)(t3d_angle_tree_guess + entryOffset - T3D_FIRST_TABLE_OFFSET) = entry;
        EQ_CDECL(void, 0x10069984, (double))((double)(index * 2));
        __asm
        {
            fmul qword ptr EQ_ASM_GLOBAL(0x100763B0)
        }
        entry = T3D_ST0_TO_SHORT();
        index++;
        *(short *)(t3d_angle_tree_guess + entryOffset) = entry;
        entryOffset += 2;
    } while (entryOffset < T3D_SHORT_TABLES_END);
    return 1;
}

// FUNCTION: EQGFX 0x1004e904
// Prepares sine and cosine samples for renderer rotations.
int __cdecl initializeCosineTable_guess(void)
{
    t3dStartupBlock_guess *startupBlock;
    float *cosineSample;
    unsigned int iteration = 0;
    unsigned int tableIndex = 0x80;
    int convertedValue;

    startupBlock = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    cosineSample = startupBlock->cosineInterpolationTable;
    do
    {
        EQ_CDECL(void, 0x10069C44, (double))
            ((double)iteration * t3d_cosine_angle_step);
        __asm
        {
            fld st(0)
            fmul qword ptr EQ_ASM_GLOBAL(0x10076388)
        }
        convertedValue = EQ_CDECL(int, 0x1004EC36, (void))();
        startupBlock->cosTable[iteration] = convertedValue;
        __asm
        {
            mov eax, cosineSample
            fstp dword ptr [eax]
        }
        if (tableIndex == 0x200)
            tableIndex = 0;
        iteration++;
        startupBlock->sineTable[tableIndex] = startupBlock->cosTable[iteration - 1];
        startupBlock->sineInterpolationTable[tableIndex] = *cosineSample;
        cosineSample++;
        tableIndex++;
    } while (iteration < 0x200);
    return 1;
}

// FUNCTION: EQGFX 0x1004e99d
// Organizes angle lookup nodes for the renderer's angle calculations.
// __cdecl, int: recursively links the node-array slice [first, last] -- each midpoint node's lower/higher take
// its subranges' midpoints; any midpoint outside [0, 0x7f] makes the whole build return -0x2835.
int __cdecl buildBinaryTree_guess(int nodeArray, int first, int last)
{
    int result = 1;
    if (first <= last)
    {
        int mid = (first + last + 1) >> 1;
        if (mid < 0 || mid >= T3D_ANGLE_NODE_LIMIT)
            result = -0x2835;
        else
        {
            t3dAngleNode_guess *node = (t3dAngleNode_guess *)(nodeArray + mid * 0x18);
            node->lower = (t3dAngleNode_guess *)(nodeArray + ((mid + first) >> 1) * 0x18);
            result = buildBinaryTree_guess(nodeArray, first, mid - 1);
            if (result >= 0)
            {
                node->higher = (t3dAngleNode_guess *)(nodeArray + ((mid + last + 1) >> 1) * 0x18);
                result = buildBinaryTree_guess(nodeArray, mid + 1, last);
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1004EA13
// Prepares sine and tangent ratios for renderer angle calculations.
// __cdecl, int (no arguments; ECX is not an input). Fills the 128 angle nodes (block + 0x6080) from the start-up
// block's tables, then returns buildBinaryTree_guess(nodes, 0, 0x7f)'s result: EAX carries it out.
int __cdecl initializeNodeArray_guess(void)
{
    t3dStartupBlock_guess *block;
    t3dAngleNode_guess *node;
    unsigned int index;

    block = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    index = 0;
    do
    {
        node = &block->nodes[index];
        node->sineOverCos = (int)(((__int64)block->sineTable[index] << 16) / block->cosTable[index]);
        if (block->keyDivisor[index] == EQGFX_ZERO_F_GUESS)
            *(int *)&node->key = T3D_LARGE_FLOAT_BITS;
        else
            node->key = block->keyDividend[index] / block->keyDivisor[index];
        if (index == 0)
        {
            block->nodes[0].cosOverSine = T3D_FIXED_INFINITY;
            *(int *)&block->nodes[0].ratio = T3D_LARGE_FLOAT_BITS;
        }
        else
        {
            node->cosOverSine = (int)(((__int64)block->cosTable[index] << 16) / block->sineTable[index]);
            node->ratio = block->keyDivisor[index] / block->keyDividend[index];
        }
        index++;
    } while (index < T3D_ANGLE_NODE_LIMIT);
    return EQ_CDECL(int, 0x1004E99D, (int, int, int))((int)block->nodes, 0, T3D_ANGLE_NODE_LIMIT - 1);
}

// FUNCTION: EQGFX 0x1004EB0F
// Prepares paired index-derived values for renderer math; their specific use is unclear.
int __cdecl initializeRandomValues_guess(void)
{
    t3dStartupBlock_guess *block;
    int *outputValues;
    unsigned int index;
    __int64 indexValue;
    __int64 squaredIndexValue;

    block = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    outputValues = &block->randomValues[0][0];
    *outputValues = -1;
    outputValues[1] = -1;
    outputValues += 2;
    *outputValues = -1;
    outputValues[1] = -1;
    outputValues += 2;
    index = 2;
    do
    {
        indexValue = index;
        __asm
        {
            fild qword ptr [indexValue]
            fdivr qword ptr EQ_ASM_GLOBAL(0x100763D0)
        }
        *outputValues++ = EQ_CDECL(int, 0x1004EC36, (void))();
        squaredIndexValue = index * index;
        __asm
        {
            fild qword ptr [squaredIndexValue]
            fdivr qword ptr EQ_ASM_GLOBAL(0x100763C8)
        }
        *outputValues++ = EQ_CDECL(int, 0x1004EC36, (void))();
        index++;
    } while (index < 0x400);
    return 1;
}

// FUNCTION: EQGFX 0x1004EB7F
// Prepares power-of-two scaling values for renderer calculations; exact use is unclear.
// __cdecl, void (no arguments; ECX is not an input). Fills the start-up block's power table: entry 0 is
// {1, 0, 1.0f}, entries 1..10 are {2^k, 2k - 1, 0.5^k}; the table ends at +0x8d04.
void __cdecl initializePowerTable_guess(void)
{
    t3dStartupBlock_guess *block;
    t3dPowerEntry_guess *entry;
    t3dPowerEntry_guess *end;
    float halfPow;
    int pow2;
    int odd;

    block = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    block->powers[0].odd = 0;
    entry = block->powers;
    end = entry + 11;
    entry->halfPow = 1.0f;
    halfPow = t3d_half_constant;
    entry->pow2 = 1;
    entry++;
    pow2 = 2;
    odd = 1;
    for (; entry < end; entry++)
    {
        entry->halfPow = halfPow;
        halfPow = halfPow * t3d_half_constant;
        entry->pow2 = pow2;
        entry->odd = odd;
        pow2 = pow2 * 2;
        odd = odd + 2;
    }
}

// FUNCTION: EQGFX 0x1004EBD5
// __cdecl, int (no arguments). Allocates the start-up block then runs the init chain, returning the first failure.
int __cdecl initializeMathTables_guess(void)
{
    int status;

    t3d_angle_tree_guess = (unsigned char *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(t3dStartupBlock_guess));
    if (t3d_angle_tree_guess == 0)
        return -0x2800;
    status = EQ_CDECL(int, 0x1004E88A, (void))();
    if (status >= 0)
    {
        status = EQ_CDECL(int, 0x1004E904, (void))();
        if (status >= 0)
        {
            status = EQ_CDECL(int, 0x1004EA13, (void))();
            if (status >= 0)
            {
                status = EQ_CDECL(int, 0x1004EB0F, (void))();
                if (status >= 0)
                {
                    status = EQ_CDECL(int, 0x1004EC98, (void))();
                    if (status >= 0)
                        status = EQ_CDECL(int, 0x1004EB7F, (void))();
                }
            }
        }
    }
    return status;
}

// FUNCTION: EQGFX 0x1004ec22
// Releases the renderer's shared math tables and angle lookup data.
// __cdecl (the callee pops 0 bytes; ECX is not an input), no arguments, void (callers never read EAX after
// the call). Frees the angle-arc tangent tree's start-up block and clears the pointer (t3dCleanUpLibrary).
void __cdecl _t3dFreeMathGlobals(void)
{
    EQ_CDECL(void, 0x10050CBB, (void *))(t3d_angle_tree_guess);
    t3d_angle_tree_guess = 0;
}

// FUNCTION: EQGFX 0x1004ec36
// Converts floating point renderer values to whole numbers by dropping the fraction.
// __cdecl, no arguments: pops the x87 input from ST0 and returns its int64 in EDX:EAX. FISTP rounds it, then EAX alone
// may move by one, picked by the sign of the rounding error (x - r); a carry or borrow never reaches EDX (bug kept).
EQ_NAKED __int64 __cdecl t3dFloatTruncateToInt_guess(void)
{
    __asm {
        lea ecx, [esp - 0x8]
        sub esp, 0x10
        and ecx, 0xfffffff8
        fld st(0)
        fistp qword ptr [ecx]
        fild qword ptr [ecx]
        mov edx, dword ptr [ecx + 0x4]
        mov eax, dword ptr [ecx]
        test eax, eax
        jz low_zero
    fraction:
        fsubp st(1), st
        test edx, edx
        jz high_zero
        fstp dword ptr [ecx]
        mov ecx, dword ptr [ecx]
        add esp, 0x10
        xor ecx, 0x80000000
        add ecx, 0x7fffffff
        adc eax, 0x0
        ret
    high_zero:
        fstp dword ptr [ecx]
        mov ecx, dword ptr [ecx]
        add esp, 0x10
        add ecx, 0x7fffffff
        sbb eax, 0x0
        ret
    low_zero:
        test edx, 0x7fffffff
        jnz fraction
        fstp dword ptr [ecx]
        fstp dword ptr [ecx]
        add esp, 0x10
        ret
    }
}

// FUNCTION: EQGFX 0x1004ec8d
// Rounds floating point values to whole-number renderer values.
// __cdecl, void (no caller reads EAX after the call), and frame-less: eqgfx_dx7.dll has no __ftol, its
// float -> int conversions are inline FISTP, so the whole function is the store. EAX takes the destination
// pointer first, FLD loads the float argument, FISTP stores the int in the FPU's current rounding mode
// (round to nearest by default) and RET closes it.
EQ_NAKED void __cdecl t3dFloatRoundToInt_guess(float value, int *out)
{
    __asm {
        mov eax, dword ptr [esp + 0x8]
        fld dword ptr [esp + 0x4]
        fistp dword ptr [eax]
        ret
    }
}

#define t3d_startup_block          EQ_GLOBAL(t3dStartupBlock_guess *, 0x100F3FF4)  // start-up block pointer (the same slot as t3d_sine_base)
#define EQGFX_FLOAT_ONE           EQ_GLOBAL(float, 0x10076280)            // 1.0f in .rdata: the reciprocal's numerator

// FUNCTION: EQGFX 0x1004ec98
// Prepares reciprocal lookup values used by renderer math.
// __cdecl, returns 0. table[i] = (bits(1/f(x)) + 0x2000) >> 15, x = float of bits (i | 0x1f80) << 17, f = FUN_10069984; then table[0x40] |= 0xff.
int __cdecl initializeReciprocalTable_guess(void)
{
    int index = 0;
    unsigned char *table;
    union
    {
        unsigned int bits;
        float value;
    } sample, reciprocal;

    table = t3d_startup_block->reciprocal_table;
    do
    {
        sample.bits = (index | 0x1f80) << 17;
        reciprocal.value = EQGFX_FLOAT_ONE / EQ_CDECL(float, 0x10069984, (double))(sample.value);
        *table = (unsigned char)((reciprocal.bits + 0x2000) >> 15);
        table++;
        index++;
    } while (index < 0x80);
    t3d_startup_block->reciprocal_table[0x40] |= 0xff;
    return 0;
}

#define t3d_float_ratio_max_guess  EQ_GLOBAL(float, 0x10076294)            // 1e30f, the ratio clamp
#define t3d_float_ratio_min_guess  EQ_GLOBAL(float, 0x100763D8)            // -1e30f
typedef void (__stdcall *t3dDebugBreak_guess_t)(void);
#define t3d_debug_break_guess      EQ_GLOBAL(t3dDebugBreak_guess_t, 0x100760BC)

// FUNCTION: EQGFX 0x1004ecf9
// Converts a 2D direction into an angle in 512 units per turn for orientation and visibility.
// @early-stop: 38 instruction differences remain; first difference: #16: original `jb @32` | rewrite `jb @33`.
// fake match: quadrant zero marks the axis exits so they share the final integer-to-float return.
// __cdecl, result in ST0: signs of (dy, dx) pick a quadrant, the clamped dy/|dx| walks the tangent tree at
// t3d_angle_tree_guess + 0x6680 (0x18-byte nodes: key +0x8, children +0x10/+0x14); *pFound marks an exact key hit.
float __cdecl t3dAngleArcTangentFloat(float dy, float dx, int *pFound)
{
    t3dAngleNode_guess *node;
    t3dAngleNode_guess *child;
    const float *zero = &EQGFX_ZERO_F_GUESS;
    unsigned int angle;
    int quadrant;
    float ratio;

    if (dy >= *zero)
    {
        if (!(dx <= *zero))
        {
            quadrant = 1;
        }
        else
        {
            quadrant = 2;
            if (dx == *zero)
            {
                quadrant = 0;
                angle = 0x80;
                *pFound = 1;
            }
            else
                dx = -dx;
        }
    }
    else
    {
        if (!(dx >= *zero))
        {
            quadrant = 3;
        }
        else
        {
            quadrant = 4;
            if (dx == *zero)
            {
                quadrant = 0;
                angle = 0x180;
                *pFound = 1;
            }
            else
                dx = -dx;
        }
    }
    if (quadrant != 0)
    {
        ratio = dy / dx;
        if (ratio > t3d_float_ratio_max_guess)
            ratio = t3d_float_ratio_max_guess;
        else if (ratio < t3d_float_ratio_min_guess)
            ratio = t3d_float_ratio_min_guess;

        node = (t3dAngleNode_guess *)(t3d_angle_tree_guess + 0x6680);
        for (;;)
        {
            if (ratio < node->key)
                child = node->lower;
            else if (!(ratio <= node->key)) // Unordered ratios also use the higher branch.
                child = node->higher;
            else
            {
                *pFound = 1;
                break;
            }

            if (child == node)
            {
                *pFound = 0;
                break;
            }
            node = child;
        }

        angle = ((int)node - (int)t3d_angle_tree_guess - 0x6080) / 0x18;
        switch (quadrant)
        {
        case 1:
            if (ratio < node->key)
                angle--;
            break;
        case 2:
            angle = 0x100 - angle;
            if (ratio > node->key)
                angle--;
            break;
        case 3:
            angle += 0x100;
            if (ratio < node->key)
                angle--;
            break;
        case 4:
            angle = 0x200 - angle;
            if (ratio > node->key)
                angle--;
            break;
        default:
            t3d_debug_break_guess();
        }
    }
    return (float)angle;
}

// FUNCTION: EQGFX 0x1004EE37
// Gets an angle's sine for 3D rotation and visibility calculations.
/* fake match: compute the fraction before sampling to keep the original x87 stack order. */
float __cdecl t3dFloatSine(float value)
{
    unsigned int wholeIndex;
    int tableIndex;
    float fraction;
    float *samples;
    float lowerSample;
    float upperSample;

    __asm fld value
    wholeIndex = EQ_CDECL(int, 0x1004EC36, (void))();
    if ((int)wholeIndex < 0)
        wholeIndex--;

    fraction = value - (float)(int)wholeIndex;
    samples = ((t3dStartupBlock_guess *)t3d_angle_tree_guess)->sineInterpolationTable;
    tableIndex = wholeIndex & 0x1ff;
    lowerSample = samples[tableIndex];
    if (tableIndex < 0x1ff)
        upperSample = samples[tableIndex + 1];
    else
        upperSample = samples[0];

    return (upperSample - lowerSample) * fraction + lowerSample;
}

// FUNCTION: EQGFX 0x1004EE87
// __cdecl, one float argument: the sineInterpolationTable sample at the callee's int index (9 bits), left in ST0.
float __cdecl t3dFloatFastSine(float value)
{
    unsigned int tableIndex;
    t3dStartupBlock_guess *startupBlock;

    __asm fld value
    tableIndex = EQ_CDECL(int, 0x1004EC36, (void))();
    startupBlock = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    return startupBlock->sineInterpolationTable[tableIndex & T3D_SINE_INDEX_MASK];
}

// FUNCTION: EQGFX 0x1004EEA3
// Gets an angle's cosine for 3D rotation and visibility calculations.
// @early-stop: the x87 helper input, integer spill, and frame epilog still differ.
/* fake match: the rounding helper consumes ST0 and has no C argument. */
float __cdecl t3dFloatCosine(float value)
{
    int roundedAngle;
    int sampleIndex;
    float absoluteAngle;
    float fractionalAngle;
    float firstSample;
    float secondSample;
    t3dStartupBlock_guess *startupBlock;

    absoluteAngle = value >= EQGFX_ZERO_F_GUESS ? value : -value;
    __asm fld absoluteAngle
    roundedAngle = EQ_CDECL(int, 0x1004EC36, (void))();
    startupBlock = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    sampleIndex = roundedAngle & T3D_SINE_INDEX_MASK;
    fractionalAngle = absoluteAngle - roundedAngle;
    firstSample = startupBlock->cosineInterpolationTable[sampleIndex];
    if (sampleIndex < T3D_SINE_INDEX_MASK)
        secondSample = startupBlock->cosineInterpolationTable[sampleIndex + 1];
    else
        secondSample = startupBlock->cosineInterpolationTable[0];

    return (secondSample - firstSample) * fractionalAngle + firstSample;
}

// FUNCTION: EQGFX 0x1004EEFA
// __cdecl, one float argument: the cosineInterpolationTable sample at the callee's int index (9 bits), left in ST0.
float __cdecl t3dFloatFastCosine(float value)
{
    unsigned int tableIndex;
    t3dStartupBlock_guess *startupBlock;

    __asm fld value
    tableIndex = EQ_CDECL(int, 0x1004EC36, (void))();
    startupBlock = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    return startupBlock->cosineInterpolationTable[tableIndex & T3D_SINE_INDEX_MASK];
}

// FUNCTION: EQGFX 0x1004ef16
// Gets the cotangent of a float angle for renderer orientation math.
// __cdecl, float: cotangent of a float angle from the angle tree's node array; quadrant bits 0x80/0x100 negate the mirrored node.
float __cdecl t3dFloatFastCotangent(float value)
{
    int tableIndex;
    int quadrantBits;

    __asm fld value
    tableIndex = EQ_CDECL(int, 0x1004EC36, (void))() & T3D_SINE_INDEX_MASK;
    quadrantBits = tableIndex & 0x180;
    if (quadrantBits != 0x100 && quadrantBits != 0x80)
        return ((t3dStartupBlock_guess *)t3d_angle_tree_guess)->nodes[tableIndex & 0x7f].ratio;
    return -((t3dStartupBlock_guess *)t3d_angle_tree_guess)->nodes[(~tableIndex) & 0x7f].ratio;
}

// eqgfx_dx7.dll's own .rdata floats and the seed-table base in .data: read through the address, like the
// original's memory operands (t3d_angle_tree_guess is filled in at start-up).
#define t3d_float_three_guess      EQ_GLOBAL(float, 0x100763DC)             // 3.0f, the Newton-step constant
#define EQGFX_MOTION_HALF_GUESS       EQ_GLOBAL(float, 0x10076290)             // 0.5f

// FUNCTION: EQGFX 0x1004ef68
// Finds lengths from squared values in renderer geometry and collision calculations.
// __cdecl (the callee pops 0 bytes; ECX is not an input), one float argument, the result left in ST0 like
// every float in this image: a bit-hack seed plus two Newton steps on the inverse square root, the second
// step multiplied back through the argument. The seed's exponent is 0x5f000000 minus half the input's
// exponent, its top mantissa byte a table lookup (index: the input's bits 23..17, exponent LSB + mantissa
// bits) at 0x6000 past t3d_angle_tree_guess.
float __cdecl t3dFloatSqrt(float value)
{
    unsigned char temp;
    float seed;
    unsigned int bits = *(unsigned int *)&value;

    temp = t3d_angle_tree_guess[((bits >> 17) & 0x7f) + 0x6000];
    *(unsigned int *)&seed =
        (0x5f000000 - ((0xff & (bits >> 23)) * 0x400000)) & 0xff800000 |
        ((unsigned int)temp * 32768);
    seed = EQGFX_MOTION_HALF_GUESS * ((t3d_float_three_guess - seed * seed * value) * seed);
    return EQGFX_MOTION_HALF_GUESS * ((t3d_float_three_guess - seed * seed * value) * seed) * value;
}

// FUNCTION: EQGFX 0x1004efda
// Supplies inverse length for normalizing surface normals and 3D vectors.
// __cdecl, one float argument, the result left in ST0 like every float in this image: the same bit-hack seed
// and two Newton steps as t3dFloatSqrt, without the second step multiplied back through the argument.
float __cdecl t3dFloatInvSqrt_C(float value)
{
    float seed;
    unsigned int bits = *(unsigned int *)&value;

    *(unsigned int *)&seed =
        (0x5f000000 - (((bits >> 23) & 0xff) * 0x400000)) & 0xff800000 |
        (unsigned int)t3d_angle_tree_guess[((bits >> 17) & 0x7f) + 0x6000] << 15;
    seed = (3.0f - seed * seed * value) * seed * 0.5f;
    seed = (3.0f - seed * seed * value) * seed * 0.5f;
    return seed;
}

#define T3D_ANGLE_UNITS_PER_RADIAN  81.48733f   // 512 / (2 * pi): one turn is 512 units
#define T3D_ATAN2_CALL(y, x)        EQ_CDECL(float, 0x10069FD0, (double, double))((y), (x))

// FUNCTION: EQGFX 0x1004f049
// Reads heading, pitch, and roll from a 3D rotation matrix in standard angle units.
// __cdecl: heading, pitch and roll of a 3x3 matrix in angle units, from the table arctangent; returns 1, or -1 with the angles zeroed.
int __cdecl t3dCalculateHeadingPitchRollFromCoefficients(t3dMatrix3x3_guess *coefficients, float *heading, float *pitch, float *roll)
{
    int result;
    /* fake match: the locals sit in an inner block for the original's stack layout */
    {
        float x;
        float z;
        float y;
        float horizontal;
        int angleScratch;

        result = 1;
        x = coefficients->m[0];
        z = coefficients->m[2];
        y = coefficients->m[1];
        horizontal = EQ_CDECL(float, 0x1004EF68, (float))(x * x + y * y);
        if (z == 0.0f && horizontal == 0.0f)
        {
            *heading = 0.0f;
            *pitch = 0.0f;
            *roll = 0.0f;
            result = -1;
        }
        else
        {
            *pitch = t3dAngleArcTangentFloat(z, horizontal, &angleScratch);
            if (horizontal == 0.0f)
            {
                *roll = 0.0f;
                *heading = t3dAngleArcTangentFloat(-coefficients->m[3], coefficients->m[4], &angleScratch);
            }
            else
            {
                horizontal = 1.0f / horizontal;
                *heading = t3dAngleArcTangentFloat(horizontal * y, horizontal * x, &angleScratch);
                *roll = t3dAngleArcTangentFloat(horizontal * coefficients->m[5], horizontal * coefficients->m[8], &angleScratch);
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1004f165
// __cdecl: heading, pitch and roll (512 units per turn) of a 3x3 matrix; returns 1, or -1 with the angles zeroed.
int __cdecl t3dCalculateAccurateHeadingPitchRollFromCoefficients(t3dMatrix3x3_guess *coefficients, float *heading, float *pitch, float *roll)
{
    int result;
    float x;
    float z;
    float y;
    float horizontal;

    result = 1;
    x = coefficients->m[0];
    z = coefficients->m[2];
    y = coefficients->m[1];
    horizontal = EQ_CDECL(float, 0x1004EF68, (float))(x * x + y * y);
    if (z == 0.0f && horizontal == 0.0f)
    {
        result = -1;
        *heading = 0.0f;
        *pitch = 0.0f;
        *roll = 0.0f;
    }
    else
    {
        *pitch = T3D_ATAN2_CALL(z, horizontal) * T3D_ANGLE_UNITS_PER_RADIAN;
        if (horizontal == 0.0f)
        {
            *roll = 0.0f;
            *heading = T3D_ATAN2_CALL(-coefficients->m[3], coefficients->m[4]) * T3D_ANGLE_UNITS_PER_RADIAN;
        }
        else
        {
            horizontal = 1.0f / horizontal;
            *heading = T3D_ATAN2_CALL(horizontal * y, horizontal * x) * T3D_ANGLE_UNITS_PER_RADIAN;
            *roll = T3D_ATAN2_CALL(horizontal * coefficients->m[5], horizontal * coefficients->m[8]) * T3D_ANGLE_UNITS_PER_RADIAN;
        }
        if (*heading < 0.0f)
            *heading = *heading + 512.0f;
        if (*pitch < 0.0f)
            *pitch = *pitch + 512.0f;
        if (*roll < 0.0f)
            *roll = *roll + 512.0f;
    }
    return result;
}

// FUNCTION: EQGFX 0x1004f2dd
// Builds a rotation matrix from heading, pitch and roll for sprite-part orientation.
// __cdecl, int: the 3x3 rotation matrix of heading, pitch and roll (512 units per turn) into coefficients; always returns 1.
int __cdecl t3dCalculateCoefficientsFromHeadingPitchRoll(float heading, float pitch, float roll, t3dMatrix3x3_guess *coefficients)
{
    int headingIndex;
    int pitchIndex;
    int rollIndex;
    t3dStartupBlock_guess *startupBlock;
    float sinHeading;
    float cosHeading;
    float negSinPitch;
    float cosPitch;
    float sinRoll;
    float cosRoll;
    float cosRollNegSinPitch;

    __asm fld heading
    headingIndex = EQ_CDECL(int, 0x1004EC36, (void))() & T3D_SINE_INDEX_MASK;
    __asm fld pitch
    pitchIndex = EQ_CDECL(int, 0x1004EC36, (void))() & T3D_SINE_INDEX_MASK;
    __asm fld roll
    rollIndex = EQ_CDECL(int, 0x1004EC36, (void))() & T3D_SINE_INDEX_MASK;

    startupBlock = (t3dStartupBlock_guess *)t3d_angle_tree_guess;
    sinHeading = startupBlock->sineInterpolationTable[headingIndex];
    cosHeading = startupBlock->cosineInterpolationTable[headingIndex];
    negSinPitch = -startupBlock->sineInterpolationTable[pitchIndex];
    cosPitch = startupBlock->cosineInterpolationTable[pitchIndex];
    sinRoll = startupBlock->sineInterpolationTable[rollIndex];
    cosRoll = startupBlock->cosineInterpolationTable[rollIndex];
    cosRollNegSinPitch = cosRoll * negSinPitch;

    coefficients->m[0] = cosPitch * cosHeading;
    coefficients->m[1] = cosPitch * sinHeading;
    coefficients->m[2] = -negSinPitch;
    coefficients->m[3] = sinRoll * negSinPitch * cosHeading - cosRoll * sinHeading;
    coefficients->m[4] = cosRoll * cosHeading + sinRoll * negSinPitch * sinHeading;
    coefficients->m[5] = sinRoll * cosPitch;
    coefficients->m[6] = cosRollNegSinPitch * cosHeading + sinRoll * sinHeading;
    coefficients->m[7] = cosRollNegSinPitch * sinHeading - sinRoll * cosHeading;
    coefficients->m[8] = cosRoll * cosPitch;
    return 1;
}

// FUNCTION: EQGFX 0x1004f3c6
// Builds particle and spawn orientation matrices from heading, pitch, and roll.
int __cdecl t3dCalculateAccurateCoefficientsFromHeadingPitchRoll(float heading, float pitch, float roll, float *coefficients)
{
    float sinHeading;
    float cosHeading;
    float negSinPitch;
    float cosPitch;
    float sinRoll;
    float cosRoll;

    sinHeading = EQ_CDECL(float, 0x1004EE37, (float))(heading);
    cosHeading = EQ_CDECL(float, 0x1004EEA3, (float))(heading);
    negSinPitch = -EQ_CDECL(float, 0x1004EE37, (float))(pitch);
    cosPitch = EQ_CDECL(float, 0x1004EEA3, (float))(pitch);
    sinRoll = EQ_CDECL(float, 0x1004EE37, (float))(roll);
    cosRoll = EQ_CDECL(float, 0x1004EEA3, (float))(roll);

    coefficients[0] = cosPitch * cosHeading;
    coefficients[1] = cosPitch * sinHeading;
    coefficients[2] = -negSinPitch;
    coefficients[3] = sinRoll * negSinPitch * cosHeading - cosRoll * sinHeading;
    coefficients[4] = sinRoll * negSinPitch * sinHeading + cosRoll * cosHeading;
    coefficients[5] = sinRoll * cosPitch;
    coefficients[6] = sinRoll * sinHeading + cosRoll * negSinPitch * cosHeading;
    coefficients[7] = cosRoll * negSinPitch * sinHeading - sinRoll * cosHeading;
    coefficients[8] = cosRoll * cosPitch;
    return 1;
}

// FUNCTION: EQGFX 0x1004f4a1
// Measures alignment between two 3D directions for renderer geometry.
// __cdecl: dot product of two float[3] vectors, returned in ST0; the z and y products are summed before the x product, as the original does.
float __cdecl t3dDotProduct(float *vectorA, float *vectorB)
{
    return vectorA[2] * vectorB[2] + vectorA[1] * vectorB[1] + vectorA[0] * vectorB[0];
}

// FUNCTION: EQGFX 0x1004f4be
// __cdecl: cross product of two float[3] vectors into result[3]; always returns 0.
int __cdecl t3dCrossProduct(float *vectorA, float *vectorB, float *result)
{
    result[0] = (vectorB[2] * vectorA[1]) - vectorB[1] * vectorA[2];
    result[1] = vectorA[2] * vectorB[0] - vectorB[2] * vectorA[0];
    result[2] = vectorA[0] * vectorB[1] - vectorB[0] * vectorA[1];
    return 0;
}

// FUNCTION: EQGFX 0x1004f4fb
// Combines two 3D orientation matrices into one.
// __cdecl: out = m1 * m2 for row-major 3x3 matrices, always returning 0; every element's products and their
// order match the original's evaluation order.
int __cdecl t3dMultiply3x3Matrices(t3dMatrix3x3_guess *m1, t3dMatrix3x3_guess *m2, t3dMatrix3x3_guess *out)
{
    out->m[0] = (m2->m[0] * m1->m[0] + m1->m[2] * m2->m[6]) + m1->m[1] * m2->m[3];
    out->m[1] = (m1->m[1] * m2->m[4] + m2->m[1] * m1->m[0]) + m1->m[2] * m2->m[7];
    out->m[2] = (m1->m[0] * m2->m[2] + m1->m[1] * m2->m[5]) + m1->m[2] * m2->m[8];
    out->m[3] = (m2->m[6] * m1->m[5] + m1->m[4] * m2->m[3]) + m2->m[0] * m1->m[3];
    out->m[4] = (m1->m[4] * m2->m[4] + m2->m[1] * m1->m[3]) + m2->m[7] * m1->m[5];
    out->m[5] = (m2->m[8] * m1->m[5] + m1->m[3] * m2->m[2]) + m1->m[4] * m2->m[5];
    out->m[6] = (m2->m[0] * m1->m[6] + m2->m[3] * m1->m[7]) + m2->m[6] * m1->m[8];
    out->m[7] = (m2->m[4] * m1->m[7] + m2->m[1] * m1->m[6]) + m2->m[7] * m1->m[8];
    out->m[8] = (m2->m[8] * m1->m[8] + m2->m[5] * m1->m[7]) + m1->m[6] * m2->m[2];
    return 0;
}

// FUNCTION: EQGFX 0x1004f5e4
// Combines 3D transforms while applying both rotation and translation.
// __cdecl: out = m1 * m2 for row-major 4x3 matrices (row 3 is the translation), always returning 0; every element's
// products and their order match the original's evaluation order, and out is stored element by element (it may alias m1).
int __cdecl t3dMultiply3x4Matrices(t3dMatrix4x3_guess *m1, t3dMatrix4x3_guess *m2, t3dMatrix4x3_guess *out)
{
    float m2_00 = m2->m[0];
    float m2_01 = m2->m[1];
    float m2_10 = m2->m[3];
    float m2_20 = m2->m[6];
    float m2_21 = m2->m[7];
    float m2_02 = m2->m[2];
    float m2_22 = m2->m[8];
    float m2_11 = m2->m[4];
    float m2_12 = m2->m[5];

    out->m[0] = (m2_10 * m1->m[1] + m2_20 * m1->m[2]) + m2_00 * m1->m[0];
    out->m[1] = (m2_11 * m1->m[1] + m2_21 * m1->m[2]) + m2_01 * m1->m[0];
    out->m[2] = (m2_12 * m1->m[1] + m2_22 * m1->m[2]) + m2_02 * m1->m[0];
    out->m[3] = (m2_20 * m1->m[5] + m2_00 * m1->m[3]) + m2_10 * m1->m[4];
    out->m[4] = (m2_21 * m1->m[5] + m2_01 * m1->m[3]) + m2_11 * m1->m[4];
    out->m[5] = (m2_22 * m1->m[5] + m2_02 * m1->m[3]) + m2_12 * m1->m[4];
    out->m[6] = (m2_10 * m1->m[7] + m2_20 * m1->m[8]) + m2_00 * m1->m[6];
    out->m[7] = (m2_11 * m1->m[7] + m2_21 * m1->m[8]) + m2_01 * m1->m[6];
    out->m[8] = (m2_12 * m1->m[7] + m2_22 * m1->m[8]) + m2_02 * m1->m[6];
    out->m[9] = ((m2_10 * m1->m[10] + m2_20 * m1->m[11]) + m2_00 * m1->m[9]) + m2->m[9];
    out->m[10] = ((m2_11 * m1->m[10] + m2_21 * m1->m[11]) + m2_01 * m1->m[9]) + m2->m[10];
    out->m[11] = ((m2_12 * m1->m[10] + m2_22 * m1->m[11]) + m2_02 * m1->m[9]) + m2->m[11];
    return 0;
}

// FUNCTION: EQGFX 0x1004f749
// __cdecl: out = xyz * matrix, the row vector times a row-major 3x3 matrix; always returning 0.
int __cdecl t3dMultiplyXYZBy3x3Matrix(float *xyz, t3dMatrix3x3_guess *matrix, float *out)
{
    out[0] = matrix->m[6] * xyz[2] + matrix->m[3] * xyz[1] + xyz[0] * matrix->m[0];
    out[1] = matrix->m[1] * xyz[0] + matrix->m[7] * xyz[2] + matrix->m[4] * xyz[1];
    out[2] = matrix->m[2] * xyz[0] + matrix->m[8] * xyz[2] + matrix->m[5] * xyz[1];
    return 0;
}

// FUNCTION: EQGFX 0x1004f79e
// __cdecl: out = matrix * xyz, each matrix row dotted with xyz (column vector); always returning 0.
int __cdecl t3dPostMultiplyXYZBy3x3Matrix(float *xyz, t3dMatrix3x3_guess *matrix, float *out)
{
    out[0] = matrix->m[2] * xyz[2] + matrix->m[1] * xyz[1] + xyz[0] * matrix->m[0];
    out[1] = matrix->m[3] * xyz[0] + matrix->m[5] * xyz[2] + matrix->m[4] * xyz[1];
    out[2] = matrix->m[6] * xyz[0] + matrix->m[8] * xyz[2] + matrix->m[7] * xyz[1];
    return 0;
}

// FUNCTION: EQGFX 0x1004f7f3
// __cdecl: out = [x y z 1] * matrix (4x3 row-major, row 3 = translation); always returning 0.
int __cdecl t3dMultiplyXYZBy4x3Matrix(float *xyz, t3dMatrix4x3_guess *matrix, float *out)
{
    out[0] = matrix->m[6] * xyz[2] + matrix->m[3] * xyz[1] + xyz[0] * matrix->m[0] + matrix->m[9];
    out[1] = matrix->m[1] * xyz[0] + matrix->m[7] * xyz[2] + matrix->m[4] * xyz[1] + matrix->m[10];
    out[2] = matrix->m[2] * xyz[0] + matrix->m[8] * xyz[2] + matrix->m[5] * xyz[1] + matrix->m[11];
    return 0;
}

// FUNCTION: EQGFX 0x1004f851
// __cdecl: unitVector = vector * (1 / |vector|), the reciprocal length from t3dFloatInvSqrt_C; always returning 0.
int __cdecl t3dNormalize(float *vector, float *unitVector)
{
    float inverseLength;

    inverseLength = EQ_CDECL(float, 0x1004EFDA, (float))(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
    unitVector[0] = inverseLength * vector[0];
    unitVector[1] = inverseLength * vector[1];
    unitVector[2] = inverseLength * vector[2];
    return 0;
}

// FUNCTION: EQGFX 0x1004f89a
// Converts heading, pitch, and roll into a quaternion for 3D object rotation.
void __cdecl t3dEulerToQuaternion(float xAngle, float yAngle, float zAngle, float *quaternion)
{
    float angleX;
    float sineX;
    float cosineX;
    float sineY;
    float cosineY;
    float sineZ;
    float cosineZ;
    float cosineProduct;
    float sineProduct;

    angleX = (EQGFX_EFFECT_ANGLE_WRAP_AMOUNT - xAngle) * EQGFX_MOTION_HALF_GUESS;
    sineX = EQ_CDECL(float, 0x1004EE37, (float))(angleX);
    cosineX = EQ_CDECL(float, 0x1004EEA3, (float))(angleX);
    yAngle = yAngle * EQGFX_MOTION_HALF_GUESS;
    sineY = EQ_CDECL(float, 0x1004EE37, (float))(yAngle);
    cosineY = EQ_CDECL(float, 0x1004EEA3, (float))(yAngle);
    yAngle = zAngle * EQGFX_MOTION_HALF_GUESS;
    sineZ = EQ_CDECL(float, 0x1004EE37, (float))(yAngle);
    cosineZ = EQ_CDECL(float, 0x1004EEA3, (float))(yAngle);
    cosineProduct = cosineZ * cosineY;
    sineProduct = sineZ * sineY;
    quaternion[0] = cosineProduct * cosineX + sineProduct * sineX;
    quaternion[1] = cosineZ * sineY * cosineX - sineZ * cosineY * sineX;
    quaternion[2] = cosineProduct * sineX + sineProduct * cosineX;
    quaternion[3] = sineZ * cosineY * cosineX - cosineZ * sineY * sineX;
}

// FUNCTION: EQGFX 0x1004f96f
// Scales the three Euler angles from degrees to table units, then forwards them to t3dEulerToQuaternion.
void __cdecl t3dEulerToQuaternionDeg(float xAngleDeg, float yAngleDeg, float zAngleDeg, float *quaternion)
{
    EQ_CDECL(void, 0x1004F89A, (float, float, float, float *))(
        xAngleDeg * t3d_angle_units_per_degree,
        yAngleDeg * t3d_angle_units_per_degree,
        zAngleDeg * t3d_angle_units_per_degree,
        quaternion);
}
