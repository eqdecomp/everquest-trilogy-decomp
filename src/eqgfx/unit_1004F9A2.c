// unit_1004F9A2.c - eqgfx_dx7.dll, the address run 0x1004F9A2..0x100509DC (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <string.h>

// eqgfx_dx7.dll's own globals used here (VAs of this image, not trilogy's).
#define g_100f4000_guess EQ_GLOBAL(int, 0x100F4000) // DAT_100f4000: set to 1 when CPUID ext leaf EDX bit 23 is present
#define g_100f4004_guess EQ_GLOBAL(int, 0x100F4004) // DAT_100f4004: set to 1 when CPUID ext leaf EDX bit 31 is clear
#define g_100f400c_guess EQ_GLOBAL(int, 0x100F400C) // DAT_100f400c: set to 1 when CPUID ext leaf EDX bit 31 is present
#define s_100a360c_guess EQ_ARRAY(char, 0x100A360C) // s_Jul_27_2001_100a360c: build date string "Jul 27 2001"
#define s_100a35a0_guess EQ_ARRAY(char, 0x100A35A0) // s__100a35a0: CPU name string
#define s_100a35c0_guess EQ_ARRAY(char, 0x100A35C0) // s_CYRIX_100a35c0: "CYRIX"
#define s_100a362c_guess EQ_ARRAY(char, 0x100A362C) // s__100a362c: version tag string "1.68"
#define s_100a3634_guess EQ_ARRAY(char, 0x100A3634) // s_Jul_14_1997_100a3634: version date string "Jul 14 1997"
#define g_100f4010_guess EQ_GLOBAL(int, 0x100F4010) // DAT_100f4010
#define s_100a35b0_guess EQ_ARRAY(char, 0x100A35B0) // s_OTHER_100a35b0: "OTHER"
#define s_100a35a8_guess EQ_ARRAY(char, 0x100A35A8) // s_INTEL_100a35a8: "INTEL"
#define s_100a35b8_guess EQ_ARRAY(char, 0x100A35B8) // s_AMD_100a35b8: " AMD "
#define g_100f4014_guess EQ_GLOBAL(unsigned int, 0x100F4014) // DAT_100f4014: CPU speed
#define T3D_DEFAULT_RENDER_METHODS EQ_ARRAY(unsigned char, 0x100FA028) // T3D_DEFAULT_RENDER_METHODS: 64-byte table filled by t3dInitializeDitherTable_guess
#define g_100f4028_guess EQ_ARRAY(unsigned char, 0x100F4028) // DAT_100f4028: 8KB stepped table filled by t3dInitializeColorBlendTables_guess
#define g_100f6028_guess EQ_ARRAY(unsigned char, 0x100F6028) // DAT_100f6028: 16KB clamped table filled by t3dInitializeColorBlendTables_guess
#define g_byteFloatTable_guess EQ_ARRAY(float, 0x100FA068) // g_byteFloatTable_guess: 256-float byte scale table
#define g_frameFractions_guess EQ_ARRAY(float, 0x100FA468) // g_frameFractions_guess: 1024-float linear table
#define g_packedNormalTable_guess EQ_ARRAY(float, 0x100FB468) // g_packedNormalTable_guess: 16-float blend table
#define g_blendB_guess EQ_ARRAY(float, 0x100FB4A8) // g_blendB_guess: 16-float blend table
#define g_blendEnd_guess EQ_ARRAY(float, 0x100FB4E8) // g_blendEnd_guess: end of blend tables
#define g_scaleTable_guess EQ_GLOBAL(float, 0x100FB868) // EQGFX_MESH_SCALE_F_GUESS: scale table base (1.0)
#define g_scaleNext_guess EQ_ARRAY(float, 0x100FB86C) // g_scaleNext_guess: scale table entries
#define g_scaleEnd_guess EQ_ARRAY(float, 0x100FB8A8) // g_render_mode_guess: end of scale table
#define EQGFX_FRAME_POSITION_SCALE EQ_GLOBAL(float, 0x1007629C) // 0.00390625 (1/256)
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280) // 1.0
#define EQGFX_SPAWN_AXIS_EPS_GUESS EQ_GLOBAL(float, 0x1007628C) // 0.0001
#define c_slerpAngleScale EQ_GLOBAL(float, 0x100762F8) // 128.0
#define c_blendStep_guess EQ_GLOBAL(float, 0x100763F4) // 0.06666667 (1/15)
#define c_blendNeg_guess EQ_GLOBAL(float, 0x100763F0) // -0.06666667 (-1/15)
#define EQGFX_MOTION_HALF_GUESS EQ_GLOBAL(float, 0x10076290) // 0.5
#define c_four_guess EQ_GLOBAL(float, 0x100763E8)
#define s_100a35e0_guess EQ_ARRAY(char, 0x100A35E0) // s_Version_1.0.0_(DX7)_Last_Build_D_100a35e0: "..."
#define s_100a3618_guess EQ_ARRAY(char, 0x100A3618) // s_18:14:24_100a3618: build time string "18:14:24"
#define s_100a365c_guess EQ_ARRAY(char, 0x100A365C)
#define s_100a3624_guess EQ_ARRAY(char, 0x100A3624) // s_~*RPE_100a3624: query string "~*RPE"
#define g_100f4020_guess EQ_ARRAY(unsigned char, 0x100F4020) // _t3dgGlobals
#define s_100a3640_guess EQ_ARRAY(char, 0x100A3640) // s_TimeGetTime-cpuSpeed:_%d_100a3640: "..."

// Import table entry of this image (call dword ptr [va], filled by the loader).
typedef unsigned int (__stdcall *TimeGetTimeFn_guess_t)(void);
#define pfn_timeGetTime_guess EQ_GLOBAL(TimeGetTimeFn_guess_t, 0x10076274)
typedef void (__stdcall *OutputDebugStringAFn_guess_t)(const char *);
#define pfn_OutputDebugStringA_guess EQ_GLOBAL(OutputDebugStringAFn_guess_t, 0x100760C4)

// Quaternion as eqgfx reads it: w, x, y, z at 0x0..0xC (no include/ struct for it yet).
typedef struct Quaternion_guess
{
    float w;
    float x;
    float y;
    float z;
} Quaternion_guess;

typedef struct Matrix3x3_guess
{
    float m[9];
} Matrix3x3_guess;

// FUNCTION: EQGFX 0x1004F9A2
// Turns a rotation quaternion into a 3D rotation matrix.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input).
// Converts a quaternion (w, x, y, z at [0..3]) to a 3x3 row-major rotation matrix (9 floats).
void __cdecl t3dQuaternionToMatrix(float *quat, float *matrix)
{
    matrix[0] = ((quat[0] * quat[0] + quat[1] * quat[1]) - quat[2] * quat[2]) - quat[3] * quat[3];
    matrix[1] = (quat[2] * quat[1] + (quat[3] * quat[0])) * 2.0f;
    matrix[2] = (quat[3] * quat[1] - quat[0] * quat[2]) * 2.0f;
    matrix[3] = (quat[2] * quat[1] - quat[3] * quat[0]) * 2.0f;
    matrix[4] = ((quat[0] * quat[0] - quat[1] * quat[1]) + quat[2] * quat[2]) - quat[3] * quat[3];
    matrix[5] = (quat[3] * quat[2] + quat[0] * quat[1]) * 2.0f;
    matrix[6] = (quat[3] * quat[1] + quat[0] * quat[2]) * 2.0f;
    matrix[7] = (quat[3] * quat[2] - quat[0] * quat[1]) * 2.0f;
    matrix[8] = ((quat[0] * quat[0] - quat[1] * quat[1]) - quat[2] * quat[2]) + quat[3] * quat[3];
}

// FUNCTION: EQGFX 0x1004FAA2
// Converts a rotation matrix into quaternion form to represent a 3D orientation.
void __cdecl t3dMatrixToQuaternion(Matrix3x3_guess *matrix, Quaternion_guess *quaternion)
{
    float trace[4];
    int largest;
    float halfRoot;
    float reciprocal;
    float crossTerm;

    trace[0] = matrix->m[4] + matrix->m[0] + matrix->m[8];
    trace[1] = (matrix->m[0] - matrix->m[4]) - matrix->m[8];
    trace[2] = (matrix->m[4] - matrix->m[0]) - matrix->m[8];
    trace[3] = matrix->m[8] - (matrix->m[4] + matrix->m[0]);

    largest = trace[0] < trace[1];
    if (trace[largest] < trace[2])
        largest = 2;
    if (trace[largest] < trace[3])
        largest = 3;

    halfRoot = EQ_CDECL(float, 0x1004EF68, (float))(trace[largest] + EQGFX_FLOAT_ONE) * EQGFX_MOTION_HALF_GUESS;
    reciprocal = EQGFX_FLOAT_ONE / (halfRoot * c_four_guess);

    if (largest == 0)
    {
        quaternion->w = halfRoot;
        quaternion->x = (matrix->m[5] - matrix->m[7]) * reciprocal;
        quaternion->y = (matrix->m[6] - matrix->m[2]) * reciprocal;
        quaternion->z = (matrix->m[1] - matrix->m[3]) * reciprocal;
    }
    else
    {
        if (largest == 1)
        {
            quaternion->x = halfRoot;
            quaternion->y = (matrix->m[3] + matrix->m[1]) * reciprocal;
            quaternion->z = (matrix->m[6] + matrix->m[2]) * reciprocal;
            crossTerm = matrix->m[7] + matrix->m[5];
        }
        else if (largest == 2)
        {
            quaternion->y = halfRoot;
            quaternion->x = (matrix->m[3] + matrix->m[1]) * reciprocal;
            quaternion->z = (matrix->m[7] + matrix->m[5]) * reciprocal;
            crossTerm = matrix->m[6] + matrix->m[2];
        }
        else
        {
            if (largest != 3)
                return;
            quaternion->z = halfRoot;
            quaternion->x = (matrix->m[6] + matrix->m[2]) * reciprocal;
            quaternion->y = (matrix->m[7] + matrix->m[5]) * reciprocal;
            crossTerm = matrix->m[3] + matrix->m[1];
        }
        quaternion->w = crossTerm * reciprocal;
    }
}

// FUNCTION: EQGFX 0x1004FBEB
// Gets heading, pitch and roll in engine rotation units from an orientation quaternion.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input).
// Converts a quaternion (w, x, y, z at [0..3]) to a 3x3 matrix, then passes it and the angle pointers to 0x1004F165.
void __cdecl t3dQuaternionToEuler(float *quat, float *pitch, float *roll, float *heading)
{
    float matrix[9];
    float wSquared;
    float xSquared;
    float wz;
    float yx;
    float zy;
    float wx;

    xSquared = quat[1] * quat[1];
    wSquared = quat[0] * quat[0];
    matrix[0] = ((wSquared + xSquared) - quat[2] * quat[2]) - quat[3] * quat[3];
    yx = quat[2] * quat[1];
    wz = quat[0] * quat[3];
    matrix[1] = (wz + yx) * 2.0f;
    matrix[2] = (quat[3] * quat[1] - quat[0] * quat[2]) * 2.0f;
    matrix[3] = (yx - wz) * 2.0f;
    matrix[4] = (((wSquared - xSquared) + quat[2] * quat[2]) - quat[3] * quat[3]);
    zy = quat[3] * quat[2];
    wx = quat[0] * quat[1];
    matrix[5] = (wx + zy) * 2.0f;
    matrix[6] = (quat[0] * quat[2] + quat[3] * quat[1]) * 2.0f;
    matrix[7] = (zy - wx) * 2.0f;
    matrix[8] = (((wSquared - xSquared) - quat[2] * quat[2]) + quat[3] * quat[3]);
    EQ_CDECL(int, 0x1004F165, (float *, float *, float *, float *))(matrix, heading, pitch, roll);
}

// FUNCTION: EQGFX 0x1004FCCB
// Gets heading, pitch and roll in degrees from an orientation quaternion.
// __cdecl (the callee pops 0 bytes; ECX is not an input). The pitch goes through &quat, so it is read back from the quat slot.
// Scales the three Euler angles from 0x1004FBEB by 0.703125 and stores them as pitch, roll, heading.
void __cdecl t3dQuaternionToEulerDeg(float *quat, float *pitch, float *roll, float *heading)
{
    float rollRaw;
    float headingRaw;

    EQ_CDECL(void, 0x1004FBEB, (float *, float *, float *, float *))(quat, (float *)&quat, &rollRaw, &headingRaw);
    *pitch = *(float *)&quat * 0.703125f;
    *roll = rollRaw * 0.703125f;
    *heading = headingRaw * 0.703125f;
}

// FUNCTION: EQGFX 0x1004FD0F
// Builds a rotation quaternion for an axis and angle.
void __cdecl t3dVectorAngleToQuaternion(float axisX, float axisY, float axisZ, float angle, float *quaternion)
{
    float sineValue;

    angle = angle * EQGFX_MOTION_HALF_GUESS;
    sineValue = EQ_CDECL(float, 0x1004EE37, (float))(angle);
    *quaternion = EQ_CDECL(float, 0x1004EEA3, (float))(angle);
    quaternion[1] = sineValue * axisX;
    quaternion[2] = sineValue * axisY;
    quaternion[3] = sineValue * axisZ;
}

// FUNCTION: EQGFX 0x1004FD59
// Combines two orientation quaternions into one rotation.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input).
// Multiplies two quaternions (w, x, y, z at [0..3]): result = first * second.
void __cdecl t3dMultQuaternion(float *first, float *second, float *result)
{
    result[0] = ((first[0] * second[0] - second[1] * first[1]) - second[2] * first[2]) - second[3] * first[3];
    result[1] = ((second[3] * first[2] - second[2] * first[3]) + first[1] * second[0]) + second[1] * first[0];
    result[2] = ((second[1] * first[3] - second[3] * first[1]) + first[2] * second[0]) + second[2] * first[0];
    result[3] = ((second[2] * first[1] - second[1] * first[2]) + first[3] * second[0]) + second[3] * first[0];
}

// FUNCTION: EQGFX 0x1004FDE1
// Forms a vector-times-orientation quaternion product for 3D rotation.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input).
// Multiplies a vector (x, y, z at [0..2]) by a quaternion (w, x, y, z at [0..3]): result = vector * quaternion.
void __cdecl t3dMultVectorQuaternion(float *vector, float *quaternion, float *result)
{
    result[0] = -(vector[0] * quaternion[1]) - vector[1] * quaternion[2] - vector[2] * quaternion[3];
    result[1] = vector[0] * quaternion[0] + (vector[1] * quaternion[3] - vector[2] * quaternion[2]);
    result[2] = vector[1] * quaternion[0] + (vector[2] * quaternion[1] - vector[0] * quaternion[3]);
    result[3] = vector[2] * quaternion[0] + (vector[0] * quaternion[2] - vector[1] * quaternion[1]);
}

// FUNCTION: EQGFX 0x1004FE4C
// Forms an orientation-times-vector quaternion product for 3D rotation.
// Multiplies a quaternion (w, x, y, z at [0..3]) by a vector (x, y, z at [0..2]) read as (0, vector): result = quaternion * vector.
void __cdecl t3dMultQuaternionVector(float *quaternion, float *vector, float *result)
{
    result[0] = -(vector[0] * quaternion[1]) - quaternion[2] * vector[1] - quaternion[3] * vector[2];
    result[1] = quaternion[0] * vector[0] + (quaternion[2] * vector[2] - quaternion[3] * vector[1]);
    result[2] = quaternion[0] * vector[1] + (quaternion[3] * vector[0] - quaternion[1] * vector[2]);
    result[3] = quaternion[0] * vector[2] + (quaternion[1] * vector[1] - quaternion[2] * vector[0]);
}

// FUNCTION: EQGFX 0x1004FEB7
// Produces the conjugate quaternion used when reversing a rotation.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input).
// Conjugates a quaternion (w, x, y, z at [0..3]): copies w, negates x, y, z.
void __cdecl t3dConjugateQuaternion(float *source, float *result)
{
    result[0] = source[0];
    result[1] = -source[1];
    result[2] = -source[2];
    result[3] = -source[3];
}

// FUNCTION: EQGFX 0x1004fedc
// Rotates a 3D point clockwise by a quaternion orientation.
void __cdecl t3dRotatePointCWAboutQuaternion(float *point, float *quat, float *result)
{
    float diagonal;
    float dot;
    float twiceW;

    diagonal = quat[0] * quat[0] - (quat[3] * quat[3] + quat[2] * quat[2] + quat[1] * quat[1]);
    dot = point[1] * quat[2] + point[2] * quat[3] + point[0] * quat[1];
    dot = dot + dot;
    twiceW = quat[0] + quat[0];
    result[0] = dot * quat[1] + diagonal * point[0] + (point[1] * quat[3] - point[2] * quat[2]) * twiceW;
    result[1] = diagonal * point[1] + dot * quat[2] + (point[2] * quat[1] - quat[3] * point[0]) * twiceW;
    result[2] = diagonal * point[2] + dot * quat[3] + (quat[2] * point[0] - point[1] * quat[1]) * twiceW;
}

// FUNCTION: EQGFX 0x1004ff92
// Rotates a 3D point counterclockwise by a quaternion orientation.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input).
// Rotates a point (x, y, z at [0..2]) counter-clockwise about a quaternion
// (w, x, y, z at [0..3]).
void __cdecl t3dRotatePointCCWAboutQuaternion(float *point, float *quat, float *result)
{
    float diag;
    float dot2;
    float w2;

    diag = quat[0] * quat[0] - ((quat[1] * quat[1] + quat[2] * quat[2]) + quat[3] * quat[3]);
    dot2 = (point[0] * quat[1] + point[2] * quat[3]) + point[1] * quat[2];
    dot2 = dot2 + dot2;
    w2 = quat[0] + quat[0];
    result[0] = (diag * point[0] + dot2 * quat[1]) - (point[1] * quat[3] - point[2] * quat[2]) * w2;
    result[1] = (dot2 * quat[2] + diag * point[1]) - (point[2] * quat[1] - quat[3] * point[0]) * w2;
    result[2] = (dot2 * quat[3] + diag * point[2]) - (quat[2] * point[0] - point[1] * quat[1]) * w2;
}

// FUNCTION: EQGFX 0x10050048
// __cdecl (callee pops 0 bytes): each component times 1 / t3dFloatSqrt(sum of squares).
void __cdecl t3dNormalizeQuaternion(Quaternion_guess *quat)
{
    float inverseLength;

    inverseLength = EQGFX_FLOAT_ONE / EQ_CDECL(float, 0x1004EF68, (float))(
                        quat->w * quat->w + quat->x * quat->x + quat->y * quat->y + quat->z * quat->z);
    quat->w = inverseLength * quat->w;
    quat->x = inverseLength * quat->x;
    quat->y = inverseLength * quat->y;
    quat->z = inverseLength * quat->z;
}

// FUNCTION: EQGFX 0x100500A4
// Interpolates between quaternion orientations for smooth 3D rotation transitions.
void __cdecl t3dSlerpQuaternions(float *start, float *end, float blend, float *result)
{
    float *startQuaternion;
    float dot;
    float adjustedQuaternion[4];
    char angleScratch[4];
    float angleSine;
    float angle;
    float startWeight;
    float endWeight;

    startQuaternion = start;
    if (start[0] == end[0] && start[1] == end[1] && start[2] == end[2] && start[3] == end[3])
    {
        *(Quaternion_guess *)result = *(Quaternion_guess *)start;
    }
    else
    {
        EQ_CDECL(void, 0x10050240, (float *, float *, float *))(start, end, adjustedQuaternion);
        dot = adjustedQuaternion[2] * start[2] + adjustedQuaternion[1] * start[1] +
              adjustedQuaternion[0] * start[0] + adjustedQuaternion[3] * start[3];
        if (!(dot + EQGFX_FLOAT_ONE > EQGFX_SPAWN_AXIS_EPS_GUESS))
        {
            adjustedQuaternion[1] = -start[2];
            adjustedQuaternion[3] = -start[0];
            adjustedQuaternion[0] = start[3];
            adjustedQuaternion[2] = start[1];
            startWeight = EQ_CDECL(float, 0x1004EE37, (float))((1.0f - blend) * c_slerpAngleScale);
            endWeight = EQ_CDECL(float, 0x1004EE37, (float))(blend * c_slerpAngleScale);
        }
        else
        {
            if (!(1.0f - dot > EQGFX_SPAWN_AXIS_EPS_GUESS))
            {
                endWeight = blend;
                startWeight = 1.0f - endWeight;
            }
            else
            {
                angle = EQ_CDECL(float, 0x1004ECF9, (float, float, void *))(
                    EQ_CDECL(float, 0x1004EF68, (float))(1.0f - dot * dot), dot, angleScratch);
                angleSine = EQ_CDECL(float, 0x1004EE37, (float))(angle);
                startWeight = EQ_CDECL(float, 0x1004EE37, (float))((1.0f - blend) * angle) / angleSine;
                endWeight = EQ_CDECL(float, 0x1004EE37, (float))(angle * blend) / angleSine;
            }
        }
        result[0] = endWeight * adjustedQuaternion[0] + startWeight * startQuaternion[0];
        result[1] = endWeight * adjustedQuaternion[1] + startWeight * startQuaternion[1];
        result[2] = startWeight * startQuaternion[2] + endWeight * adjustedQuaternion[2];
        result[3] = endWeight * adjustedQuaternion[3] + startWeight * startQuaternion[3];
    }
    return;
}

// FUNCTION: EQGFX 0x10050240
// Selects the equivalent orientation closest to a reference quaternion.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input).
// Copies the second quaternion (w, x, y, z at [0..3]) to result, negated when
// that puts it in the opposite hemisphere from the first (|a+b|^2 < |a-b|^2,
// i.e. dot < 0). Caller: t3dSlerpQuaternions. Copy is the fall-through so a
// NaN comparison takes the negate path like the original JC.
void __cdecl t3dAdjustQuaternionHemisphere_guess(float *quatA, float *quatB, float *result)
{
    float sumSq;
    float diffSq;

    sumSq = (quatA[0] + quatB[0]) * (quatA[0] + quatB[0]) +
            (quatA[1] + quatB[1]) * (quatA[1] + quatB[1]) +
            (quatA[2] + quatB[2]) * (quatA[2] + quatB[2]) +
            (quatA[3] + quatB[3]) * (quatA[3] + quatB[3]);
    diffSq = (quatA[0] - quatB[0]) * (quatA[0] - quatB[0]) +
             (quatA[1] - quatB[1]) * (quatA[1] - quatB[1]) +
             (quatA[2] - quatB[2]) * (quatA[2] - quatB[2]) +
             (quatA[3] - quatB[3]) * (quatA[3] - quatB[3]);
    if (diffSq <= sumSq) {
        result[0] = quatB[0];
        result[1] = quatB[1];
        result[2] = quatB[2];
        result[3] = quatB[3];
    } else {
        result[0] = -quatB[0];
        result[1] = -quatB[1];
        result[2] = -quatB[2];
        result[3] = -quatB[3];
    }
}

// FUNCTION: EQGFX 0x100502F4
// Answers the "~*RPE" version query with the build stamp and globals address.
void __cdecl GetDataAddresses(void **dataOut, char *versionText)
{
    if (EQ_CDECL(int, 0x10068A20, (const char *, const char *))(versionText, s_100a3624_guess) == 0) {
        EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(versionText, s_100a35e0_guess, s_100a360c_guess, s_100a3618_guess);
        *dataOut = g_100f4020_guess;
        return;
    }
    *dataOut = 0;
}

// FUNCTION: EQGFX 0x10050336
// Initializes the T3D library subsystems in order.
int __cdecl t3dInitializeLibrary(void)
{
    int status;

    if (EQ_CDECL(int, 0x10050613, (void))() != 1)
        return -10;
    status = EQ_CDECL(int, 0x1004EBD5, (void))();
    EQ_CDECL(void, 0x10050369, (void))();
    EQ_CDECL(void, 0x100503E2, (void))();
    EQ_CDECL(void, 0x100504FD, (void))();
    EQ_CDECL(void, 0x10041767, (void))();
    EQ_CDECL(void, 0x1004D534, (void))();
    return status;
}

// FUNCTION: EQGFX 0x10050369
// Builds the 8KB stepped table at DAT_100f4028 and the 16KB clamped table at DAT_100f6028.
void __cdecl t3dInitializeColorBlendTables_guess(void)
{
    unsigned char *dest;
    unsigned int step;
    unsigned int base;
    unsigned int cur;
    unsigned int level;
    unsigned int shade;
    int inner;
    int outerRows;

    dest = g_100f4028_guess;
    step = 0x10;
    level = 1;
    do {
        base = 0;
        step--;
        outerRows = 0x20;
        do {
            cur = base;
            inner = 0x20;
            do {
                shade = cur;
                cur += step;
                *dest = (unsigned char)(shade >> 4);
                dest++;
                inner--;
            } while (inner != 0);
            base += level;
            outerRows--;
        } while (outerRows != 0);
        level++;
    } while (level <= 8);

    dest = g_100f6028_guess;
    step = 1;
    do {
        base = 0;
        do {
            cur = 0;
            inner = 0x20;
            do {
                shade = (cur >> 4) + base;
                if (shade < 0x20)
                    *dest = (unsigned char)shade;
                else
                    *dest = 0x1f;
                dest++;
                cur += step;
                inner--;
            } while (inner != 0);
            base++;
        } while (base < 0x20);
        step++;
    } while (step <= 0x10);
}

// FUNCTION: EQGFX 0x100503E2
// Builds a 64-byte table on the stack and copies it to T3D_DEFAULT_RENDER_METHODS.
void __cdecl t3dInitializeDitherTable_guess(void)
{
    unsigned char table[64];

    table[0] = 0;
    table[1] = 0x80;
    table[2] = 0x20;
    table[3] = 0xa0;
    table[4] = 8;
    table[5] = 0x88;
    table[6] = 0x28;
    table[7] = 0xa8;
    table[8] = 0xc0;
    table[9] = 0x40;
    table[10] = 0xe0;
    table[11] = 0x60;
    table[12] = 0xc8;
    table[13] = 0x48;
    table[14] = 0xf0;
    table[15] = 0x74;
    table[16] = 0x30;
    table[17] = 0xb0;
    table[18] = 0x10;
    table[19] = 0x90;
    table[20] = 0x38;
    table[21] = 0xb8;
    table[22] = 0x24;
    table[23] = 0x98;
    table[24] = 0xf0;
    table[25] = 0x70;
    table[26] = 0xd0;
    table[27] = 0x50;
    table[28] = 0xf8;
    table[29] = 0x78;
    table[30] = 0xd8;
    table[31] = 0x58;
    table[32] = 0x0c;
    table[33] = 0x8c;
    table[34] = 0x2c;
    table[35] = 0xac;
    table[36] = 4;
    table[37] = 0x84;
    table[38] = 0x24;
    table[39] = 0xa4;
    table[40] = 0xcc;
    table[41] = 0x4c;
    table[42] = 0xec;
    table[43] = 0x88;
    table[44] = 0xc4;
    table[45] = 0x44;
    table[46] = 0xe4;
    table[47] = 0x64;
    table[48] = 0x3c;
    table[49] = 0x6c;
    table[50] = 0x1c;
    table[51] = 0x9c;
    table[52] = 0x34;
    table[53] = 0xb4;
    table[54] = 0x14;
    table[55] = 0x94;
    table[56] = 0xfc;
    table[57] = 0x7c;
    table[58] = 0xdc;
    table[59] = 0x5c;
    table[60] = 0xf4;
    table[61] = 0x74;
    table[62] = 0xd4;
    table[63] = 0x54;
    EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(T3D_DEFAULT_RENDER_METHODS, table, 0x40);
}

// FUNCTION: EQGFX 0x100504fd
// Builds the byte, linear, blend and scale tables.
void __cdecl initializeRenderingTables_guess(void)
{
    float *table;
    float value;
    unsigned int index = 0;
    float *entry;

    table = g_byteFloatTable_guess;
    value = 0.0f;
    do {
        *table = value * EQGFX_FRAME_POSITION_SCALE;
        table++;
        value += EQGFX_FLOAT_ONE;
    } while (table < g_frameFractions_guess);
    table = g_frameFractions_guess;
    value = 0.0f;
    do {
        *table = value;
        value += EQGFX_FLOAT_ONE;
        table++;
    } while (table < g_packedNormalTable_guess);
    entry = g_blendB_guess;
    do {
        entry[-16] = (float)index * c_blendStep_guess;
        *entry = (float)(15u - index) * c_blendNeg_guess;
        entry++;
        index++;
    } while (entry < g_blendEnd_guess);
    g_scaleTable_guess = 1.0f;
    for (table = g_scaleNext_guess; table < g_scaleEnd_guess; table++)
        *table = table[-1] * EQGFX_MOTION_HALF_GUESS;
}

// FUNCTION: EQGFX 0x100505A7
// __cdecl. Forwards its three arguments to 0x1004cdbf and returns that call's EAX unchanged.
int __cdecl t3dReadDib(int source, int mode, int flags)
{
    return EQ_CDECL(int, 0x1004CDBF, (int, int, int))(source, mode, flags);
}

// FUNCTION: EQGFX 0x100505BC
// Calls the cleanup at 0x1004EC22 (clears t3d_angle_tree_guess) and returns 0.
int __cdecl t3dCleanUpLibrary(void)
{
    EQ_CDECL(void, 0x1004EC22, (void))();
    return 0;
}

// FUNCTION: EQGFX 0x100505C4
// Returns the T3D version number.
unsigned int __cdecl t3dGetVersionNumber(void)
{
    return 0x16800;
}

// FUNCTION: EQGFX 0x100505CA
// Copies the version tag string into tagOut. Returns 1.
int __cdecl t3dGetVersionTag(char *tagOut)
{
    EQ_CDECL(void, 0x10068070, (char *, const char *))(tagOut, s_100a362c_guess);
    return 1;
}

// FUNCTION: EQGFX 0x100505DE
// Returns the version tag string length including the terminator.
int __cdecl t3dGetVersionTagLength(void)
{
    return strlen(s_100a362c_guess) + 1;
}

// FUNCTION: EQGFX 0x100505EB
// Copies the version date string into dateOut. Returns 1.
int __cdecl t3dGetVersionDate(char *dateOut)
{
    EQ_CDECL(void, 0x10068070, (char *, const char *))(dateOut, s_100a3634_guess);
    return 1;
}

// FUNCTION: EQGFX 0x100505FF
// Copies the build date string into dateOut. Returns 1.
int __cdecl t3dGetBuildDate(char *dateOut)
{
    EQ_CDECL(void, 0x10068070, (char *, const char *))(dateOut, s_100a360c_guess);
    return 1;
}

// FUNCTION: EQGFX 0x10050613
// Detects the CPU vendor and speed; copies the vendor name to the CPU buffer.
unsigned int __fastcall s3dGetCPUInfo(unsigned int fallbackResult)
{
    unsigned int vendor;
    char *cpuName;
    unsigned int result;

    g_100f4004_guess = 0;
    g_100f400c_guess = 0;
    g_100f4010_guess = 0;
    if (EQ_CDECL(unsigned int, 0x100506D8, (void))() == 0) {
        EQ_CDECL(void, 0x10068070, (char *, const char *))(s_100a35a0_guess, s_100a35b0_guess);
        result = 1;
    } else {
        vendor = EQ_CDECL(unsigned int, 0x100506FB, (void))();
        cpuName = s_100a35a0_guess;
        if (vendor == 0) {
            EQ_CDECL(void, 0x10068070, (char *, const char *))(cpuName, s_100a35b0_guess);
            result = 1;
        } else {
            result = fallbackResult;
        }
        if (vendor == 1) {
            result = EQ_CDECL(unsigned int, 0x1005077E, (void))();
            EQ_CDECL(void, 0x10068070, (char *, const char *))(cpuName, s_100a35a8_guess);
        }
        if (vendor == 2) {
            result = EQ_CDECL(unsigned int, 0x100507E5, (void))();
            EQ_CDECL(void, 0x10068070, (char *, const char *))(cpuName, s_100a35b8_guess);
        }
        if (vendor == 3) {
            result = EQ_CDECL(unsigned int, 0x10050844, (void))();
            EQ_CDECL(void, 0x10068070, (char *, const char *))(cpuName, s_100a35c0_guess);
        }
    }
    g_100f4014_guess = EQ_CDECL(unsigned int, 0x1005089F, (void))();
    return result;
}

// FUNCTION: EQGFX 0x100506CC
// Returns the CPU name string.
char * __cdecl t3dGetCPUName(void)
{
    return s_100a35a0_guess;
}

// FUNCTION: EQGFX 0x100506D2
// Returns the CPU speed.
unsigned int __cdecl t3dGetCPUSpeed(void)
{
    return g_100f4014_guess;
}

// FUNCTION: EQGFX 0x100506d8
// Checks whether CPU identification is available on this processor.
// __cdecl (the callee pops 0 bytes; ECX is never touched).
unsigned int __cdecl s3dIsCPUIDSupported_guess(void)
{
    unsigned int changedBits;

    changedBits = 0;
    __asm {
        pushfd
        pop eax
        mov ebx, eax
        xor eax, 0x200000
        push eax
        popfd
        pushfd
        pop eax
        push ebx
        popfd
        xor eax, ebx
        mov changedBits, eax
    }
    return changedBits;
}

// FUNCTION: EQGFX 0x100506fb
// Identifies the CPU vendor via CPUID leaf 0 (1 Intel, 2 AMD, 3 Cyrix, 0 other).
unsigned int __cdecl s3dGetCPUVendor_guess(void)
{
    unsigned int vendorId;

    vendorId = 0;
    __asm {
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx
        cpuid
        cmp ebx, 0x756e6547
        jne checkAmd
        cmp edx, 0x49656e69
        jne checkAmd
        cmp ecx, 0x6c65746e
        jne checkAmd
        mov vendorId, 1
        jmp done
    checkAmd:
        cmp ebx, 0x68747541
        jne checkCyrix
        cmp edx, 0x69746e65
        jne checkCyrix
        cmp ecx, 0x444d4163
        jne checkCyrix
        mov vendorId, 2
        jmp done
    checkCyrix:
        cmp ebx, 0x69727943
        jne unknown
        cmp edx, 0x736e4978
        jne unknown
        cmp ecx, 0x64616574
        jne unknown
        mov vendorId, 3
        jmp done
    unknown:
        mov vendorId, 0
    done:
    }
    return vendorId;
}

// FUNCTION: EQGFX 0x1005077e
// Checks CPU generation and records MMX capability for renderer setup.
// __cdecl (the callee pops 0 bytes; ECX is never touched).
// Runs CPUID leaf 1 and records the family < 5 test and EDX bit 23.
unsigned int __cdecl s3dCheckCPUFamilyAndFeatures_guess(void)
{
    unsigned int versionInfo;
    unsigned int stdFeatures;

    versionInfo = 0;
    stdFeatures = 0;
    __asm {
        mov eax, 1
        mov ebx, 0
        mov ecx, 0
        mov edx, 0
        cpuid
        mov versionInfo, eax
        mov stdFeatures, edx
    }
    if (((versionInfo & 0xf00) >> 8) < 5)
        g_100f4004_guess = 0;
    else {
        g_100f4004_guess = 1;
        if ((stdFeatures & 0x800000) != 0)
            g_100f4000_guess = 1;
    }
    return g_100f4004_guess;
}

// FUNCTION: EQGFX 0x100507e5
// Records extended MMX and 3DNow capabilities for CPU feature selection.
// __cdecl (the callee pops 0 bytes; ECX is never touched).
// Runs CPUID ext leaf 0x80000001 and records EDX bit 23 and bit 31.
unsigned int __cdecl s3dCheckExtendedCPUFeatures_guess(void)
{
    unsigned int extFeatures;

    extFeatures = 0;
    __asm {
        mov eax, 0x80000001
        mov ebx, 0
        mov ecx, 0
        mov edx, 0
        cpuid
        mov extFeatures, edx
    }
    if ((extFeatures & 0x800000) != 0)
        g_100f4000_guess = 1;
    if ((extFeatures & 0x80000000) != 0)
        g_100f400c_guess = 1;
    else
        g_100f4004_guess = 1;
    return 1;
}

// FUNCTION: EQGFX 0x10050844
// Checks whether the CPU exposes the timestamp counter used for speed detection.
// __cdecl (the callee pops 0 bytes; ECX is never touched).
// Runs CPUID ext leaf 0x80000001, records EDX bit 23 and returns EDX bit 4.
unsigned int __cdecl s3dCheckExtendedCPUFeatureBit4_guess(void)
{
    unsigned int extFeatures;
    unsigned int hasFeature;

    extFeatures = 0;
    hasFeature = 0;
    __asm {
        mov eax, 0x80000001
        mov ebx, 0
        mov ecx, 0
        mov edx, 0
        cpuid
        mov extFeatures, edx
    }
    if ((extFeatures & 0x800000) != 0)
        g_100f4000_guess = 1;
    if ((extFeatures & 0x10) != 0)
        hasFeature = 1;
    else
        hasFeature = 0;
    return hasFeature;
}

// FUNCTION: EQGFX 0x1005089f
// Measures CPU speed for Cyrix hardware setup.
// __cdecl (the callee pops 0 bytes; ECX is never touched).
unsigned int __cdecl s3dGetCyrixCPUSpeed_guess(void)
{
    unsigned int startTick;
    unsigned int syncTick;
    unsigned int tscStart;
    unsigned int nowTick;
    unsigned int tscEnd;
    TimeGetTimeFn_guess_t getTime;

    if (EQ_CDECL(int, 0x10068A20, (const char *, const char *))(s_100a35a0_guess, s_100a35c0_guess) == 0)
        return 0;
    getTime = pfn_timeGetTime_guess;
    startTick = getTime();
    do {
        syncTick = getTime();
    } while ((int)(syncTick - startTick) <= 1);
    tscStart = EQ_CDECL(unsigned int, 0x100509DC, (void))();
    nowTick = getTime();
    while ((int)(nowTick - syncTick) <= 0x3ea)
        nowTick = getTime();
    tscEnd = EQ_CDECL(unsigned int, 0x100509DC, (void))();
    return (tscEnd - tscStart) / 1000000;
}

// FUNCTION: EQGFX 0x100508fd
// Measures CPU speed for renderer diagnostics using the millisecond timer.
// __cdecl (the callee pops 0 bytes; ECX is written as scratch, never read).
// Measures the CPU speed like s3dGetCyrixCPUSpeed_guess, sends "..." to OutputDebugStringA and returns it.
unsigned int __cdecl GetCpuSpeed2(void)
{
    char message[64];
    unsigned int startTick;
    unsigned int syncTick;
    unsigned int tscStart;
    unsigned int nowTick;
    unsigned int tscEnd;
    unsigned int cpuSpeed;
    TimeGetTimeFn_guess_t getTime;

    getTime = pfn_timeGetTime_guess;
    startTick = getTime();
    do {
        syncTick = getTime();
    } while ((int)(syncTick - startTick) <= 1);
    tscStart = EQ_CDECL(unsigned int, 0x100509DC, (void))();
    nowTick = getTime();
    while ((int)(nowTick - syncTick) <= 0x3ea)
        nowTick = getTime();
    tscEnd = EQ_CDECL(unsigned int, 0x100509DC, (void))();
    cpuSpeed = (tscEnd - tscStart) / 1000;
    EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(message, s_100a3640_guess, cpuSpeed);
    pfn_OutputDebugStringA_guess(message);
    return cpuSpeed;
}

// FUNCTION: EQGFX 0x1005096c
// Measures CPU speed for renderer diagnostics using the system clock.
unsigned int __cdecl GetCpuSpeed3(void)
{
    char message[64];
    long firstTime;
    long secondTime;
    unsigned int tscStart;
    unsigned int tscEnd;
    unsigned int cpuSpeed;

    firstTime = EQ_CDECL(long, 0x10069FE4, (long *))(0);
    secondTime = EQ_CDECL(long, 0x10069FE4, (long *))(0);
    while (secondTime == firstTime)
        secondTime = EQ_CDECL(long, 0x10069FE4, (long *))(0);
    tscStart = EQ_CDECL(unsigned int, 0x100509DC, (void))();
    do {
        firstTime = EQ_CDECL(long, 0x10069FE4, (long *))(0);
    } while (secondTime == firstTime);
    tscEnd = EQ_CDECL(unsigned int, 0x100509DC, (void))();
    cpuSpeed = (tscEnd - tscStart) / 1000;
    EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(message, s_100a365c_guess, cpuSpeed);
    pfn_OutputDebugStringA_guess(message);
    return cpuSpeed;
}

// FUNCTION: EQGFX 0x100509dc
// Supplies a timestamp-counter reading for CPU speed checks.
// __cdecl (the callee pops 0 bytes; ECX is never touched).
// Returns the low 32 bits of the time-stamp counter (RDTSC); EDX is dropped.
unsigned int __cdecl t3dReadTimestampCounterLow_guess(void)
{
    unsigned int tscLo;

    tscLo = 0;
    __asm {
        rdtsc
        mov tscLo, eax
    }
    return tscLo;
}
