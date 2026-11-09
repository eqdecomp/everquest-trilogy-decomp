// unit_10050AA7.c - eqgfx_dx7.dll, the address run 0x10050AA7..0x10050E01 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <stdio.h>
#include <windows.h>

// eqgfx_dx7.dll's own file names and dump mode (VAs of this image, not trilogy's).
#define s_eq_tmp_guess       EQ_ARRAY(char, 0x100A39C0)   // "eq.tmp": the temporary file dropped first
#define s_memory_txt_guess   EQ_ARRAY(char, 0x100A39B0)   // "...": the file the report goes into
#define s_memory_mode_guess  EQ_ARRAY(char, 0x100A39BC)   // s_memory_mode_guess: the mode memory.txt is opened with
#define s_memory_allocation_failed_guess EQ_ARRAY(char, 0x100A39E0)
#define s_memory_reallocation_failed_guess EQ_ARRAY(char, 0x100A3A30)
#define s_last_error_guess               EQ_ARRAY(char, 0x100A39C8)

// eqgfx_dx7.dll's import slots: the kernel32 pointers its IAT holds at 0x10076078, 0x1007607C, 0x10076080, 0x10076088 and 0x100760AC.
typedef HANDLE (__stdcall *CreateFileMappingA_t)(HANDLE file, LPSECURITY_ATTRIBUTES attributes,
                                                 DWORD protect, DWORD sizeHigh, DWORD sizeLow, LPCSTR name);
typedef HANDLE (__stdcall *OpenFileMappingA_t)(DWORD access, BOOL inheritHandle, LPCSTR name);
typedef DWORD (__stdcall *GetLastError_t)(void);
typedef LPVOID (__stdcall *MapViewOfFile_t)(HANDLE mapping, DWORD access, DWORD offsetHigh, DWORD offsetLow,
                                            DWORD bytes);
typedef BOOL (__stdcall *CloseHandle_t)(HANDLE object);
#define pfnCreateFileMappingA  EQ_GLOBAL(CreateFileMappingA_t, 0x10076078)
#define pfnOpenFileMappingA    EQ_GLOBAL(OpenFileMappingA_t, 0x1007607C)
#define pfnGetLastError        EQ_GLOBAL(GetLastError_t, 0x10076080)
#define pfnMapViewOfFile       EQ_GLOBAL(MapViewOfFile_t, 0x10076088)
#define pfnCloseHandle         EQ_GLOBAL(CloseHandle_t, 0x100760AC)

// Rgb_guess: one color element, 12 bytes (r at 0x00, g at 0x04, b at 0x08). The channel bits are compared unsigned
// against colorClampBits; the channels are read as floats for the scale, and the results are stored back as ints.
typedef struct Rgb_guess
{
    float r;
    float g;
    float b;
} Rgb_guess;

// Vec3_guess: one point or axis, 12 bytes (x at 0x00, y at 0x04, z at 0x08).
typedef struct Vec3_guess
{
    float x;
    float y;
    float z;
} Vec3_guess;

// Matrix_guess: a 3x4 transform, 48 bytes: the images of the x, y and z axes (xAxis at 0x00, yAxis at 0x0C, zAxis at
// 0x18) and the origin (0x24). A point maps to xAxis * x + yAxis * y + zAxis * z + origin.
typedef struct Matrix_guess
{
    Vec3_guess xAxis;
    Vec3_guess yAxis;
    Vec3_guess zAxis;
    Vec3_guess origin;
} Matrix_guess;

// eqgfx_dx7.dll's color globals: the scale (colorScale), the clamp limit as raw float bits (colorClampBits) and
// the copy of the scale that is left behind (lastColorScale).
#define colorScale      EQ_GLOBAL(float, 0x100A3AC0)
#define colorClampBits  EQ_GLOBAL(unsigned int, 0x100A3AC4)
#define lastColorScale  EQ_GLOBAL(float, 0x100A3ACC)
// A clamped channel is set to this int: its bits read as a denormal float, not as 255.0.
#define CHANNEL_MAX     0xff

// FUNCTION: EQGFX 0x10050aa7
// Provides players with troubleshooting guidance after a memory allocation failure.
// __cdecl (the callee pops 0 bytes; ECX is not an input), no arguments, void (callers never read EAX after the
// call). Called when an allocation fails: delete the old eq.tmp, (re)create memory.txt, write the 14-line
// out-of-memory text into it (one string per line, as the original's 14 calls), then close it.
void __cdecl st_write_mem_error_textfile(void)
{
    FILE *file;

    EQ_CDECL(void, 0x1006A154, (char *))(s_eq_tmp_guess);
    file = EQ_CDECL(FILE *, 0x10068412, (char *, char *))(s_memory_txt_guess, s_memory_mode_guess);
    if (file != 0)
    {
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3988));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3948));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3910));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A38D0));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3890));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A384C));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3810));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A37D0));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3790));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A374C));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3710));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A36D8));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A36A0));
        EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))(file, EQ_ARRAY(char, 0x100A3688));
        EQ_CDECL(int, 0x10068328, (FILE *))(file);
    }
}

// FUNCTION: EQGFX 0x10050b77
// Creates named shared storage; its in-game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0, -1 if CreateFileMappingA fails, -2 if MapViewOfFile does.
int __cdecl t3dCreateSharedMemory(HANDLE *mapping, LPVOID *view, DWORD size, LPCSTR name)
{
    int result = 0;
    HANDLE mappingHandle;
    LPVOID mappedView;

    mappingHandle = pfnCreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name);
    *mapping = mappingHandle;
    if (mappingHandle == NULL)
    {
        result = -1;
    }
    else
    {
        mappedView = pfnMapViewOfFile(mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        *view = mappedView;
        if (mappedView == NULL)
        {
            result = -2;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10050bbd
// Connects to existing named shared storage; its in-game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0, -1 if OpenFileMappingA fails, -2 if MapViewOfFile does.
int __cdecl t3dAttachSharedMemory(HANDLE *mapping, LPVOID *view, LPCSTR name)
{
    int result = 0;
    HANDLE mappingHandle;
    LPVOID mappedView;

    mappingHandle = pfnOpenFileMappingA(FILE_MAP_WRITE, TRUE, name);
    *mapping = mappingHandle;
    if (mappingHandle == NULL)
    {
        pfnGetLastError();
        result = -1;
    }
    else
    {
        mappedView = pfnMapViewOfFile(mappingHandle, FILE_MAP_WRITE, 0, 0, 0);
        *view = mappedView;
        if (mappedView == NULL)
        {
            result = -2;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10050C48
// Supplies storage for new data used by the graphics engine.
void *__cdecl t3dAllocateMemory(size_t byteCount)
{
    void *memory;
    DWORD lastError;

    memory = EQ_CDECL(void *, 0x1006A15F, (size_t, size_t))(byteCount, 1);
    if (memory == NULL)
    {
        lastError = pfnGetLastError();
        EQ_CDECL(void, 0x100509EE, (const char *, ...))(s_memory_allocation_failed_guess, byteCount);
        EQ_CDECL(void, 0x100509EE, (const char *, ...))(s_last_error_guess, lastError, lastError);
        EQ_CDECL(void, 0x10050AA7, (void))();
    }
    return memory;
}

// FUNCTION: EQGFX 0x10050C00
// Connects to named shared storage for read-only access; its in-game purpose is unclear.
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0, -1 if OpenFileMappingA fails, -2 if MapViewOfFile does; closes the mapping handle once it is open.
int __cdecl t3dAttachSharedMemoryPointer(LPVOID *view, LPCSTR name)
{
    int result = 0;
    HANDLE mappingHandle;
    LPVOID mappedView;

    mappingHandle = pfnOpenFileMappingA(FILE_MAP_READ, TRUE, name);
    if (mappingHandle == NULL)
    {
        pfnGetLastError();
        result = -1;
    }
    else
    {
        mappedView = pfnMapViewOfFile(mappingHandle, FILE_MAP_READ, 0, 0, 0);
        *view = mappedView;
        if (mappedView == NULL)
        {
            result = -2;
        }
        pfnCloseHandle(mappingHandle);
    }
    return result;
}

// FUNCTION: EQGFX 0x10050C8C
// Resizes storage used by game engine data structures.
void *__cdecl t3dReallocateMemory(void *memory, size_t byteCount)
{
    void *reallocatedMemory;

    reallocatedMemory = EQ_CDECL(void *, 0x1006A29C, (void *, size_t))(memory, byteCount);
    if (reallocatedMemory == NULL)
    {
        EQ_CDECL(void, 0x100509EE, (const char *, ...))(s_memory_reallocation_failed_guess, byteCount);
        EQ_CDECL(void, 0x10050AA7, (void))();
    }
    return reallocatedMemory;
}

// FUNCTION: EQGFX 0x10050cbb
// Releases memory after game engine data is no longer needed.
// __cdecl (the callee pops 0 bytes; ECX is not an input), one pointer argument, void (callers never read EAX
// after the call). Forwards the argument straight to the CRT's _free at 0x100681DB (this image's LIBCMT): the
// whole body is PUSH [ESP+4], CALL _free, POP ECX, RET.
void __cdecl t3dFreeMemory(void *memory)
{
    EQ_CDECL(void, 0x100681DB, (void *))(memory);
}

// FUNCTION: EQGFX 0x10050cc6
// __stdcall, three stack arguments (RET 0Ch), returns int. Always 1; the arguments are never read.
int __stdcall StubReturnOne_guess(int unusedFirst, int unusedSecond, int unusedThird)
{
    return 1;
}

// FUNCTION: EQGFX 0x10050D00
// Applies a 3D transform to a batch of points for rendering.
// __cdecl, no return value. Each point becomes xAxis * x + yAxis * y + zAxis * z + origin, count times (count 0 runs 2^32 times).
void t3dTransformPoints_guess(Vec3_guess *points, Vec3_guess *transformed, Matrix_guess *matrix, int count)
{
    do
    {
        transformed->x = matrix->xAxis.x * points->x + matrix->yAxis.x * points->y
                       + matrix->zAxis.x * points->z + matrix->origin.x;
        transformed->y = matrix->xAxis.y * points->x + matrix->yAxis.y * points->y
                       + matrix->zAxis.y * points->z + matrix->origin.y;
        transformed->z = matrix->xAxis.z * points->x + matrix->yAxis.z * points->y
                       + matrix->zAxis.z * points->z + matrix->origin.z;
        points++;
        transformed++;
        count--;
    } while (count != 0);
}

// FUNCTION: EQGFX 0x10050E01
// __cdecl, no return value. For each enabled element, each of its three channels is clamped or scaled and rounded.
void t3dQuantizeEnabledRgb_guess(Rgb_guess *colors, int count, char *enabled)
{
    float scale;
    unsigned int clampBits;
    float channel;
    int rounded;

    scale = colorScale;
    clampBits = colorClampBits;
    do
    {
        if (*enabled != 0)
        {
            if (*(unsigned int *)&colors->r > clampBits)
            {
                *(unsigned int *)&colors->r = CHANNEL_MAX;
            }
            else
            {
                channel = colors->r;
                // FISTP rounds to nearest (the FPU default); a C cast would truncate, so the conversion stays in asm.
                __asm {
                    fld dword ptr [channel]
                    fmul dword ptr [scale]
                    fistp dword ptr [rounded]
                }
                *(int *)&colors->r = rounded;
            }
            if (*(unsigned int *)&colors->g > clampBits)
            {
                *(unsigned int *)&colors->g = CHANNEL_MAX;
            }
            else
            {
                channel = colors->g;
                __asm {
                    fld dword ptr [channel]
                    fmul dword ptr [scale]
                    fistp dword ptr [rounded]
                }
                *(int *)&colors->g = rounded;
            }
            if (*(unsigned int *)&colors->b > clampBits)
            {
                *(unsigned int *)&colors->b = CHANNEL_MAX;
            }
            else
            {
                channel = colors->b;
                __asm {
                    fld dword ptr [channel]
                    fmul dword ptr [scale]
                    fistp dword ptr [rounded]
                }
                *(int *)&colors->b = rounded;
            }
        }
        colors++;
        enabled++;
        count--;
    } while (count != 0);
    lastColorScale = scale;
}
