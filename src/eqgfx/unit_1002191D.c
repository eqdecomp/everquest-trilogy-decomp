// unit_1002191D.c - eqgfx_dx7.dll, the address run 0x1002191D..0x10021937 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"

// Local layout guess for t3dFastFileTell's argument: a pointer (0x0) to a data record whose +0x8 slot is the
// fast file handle; the handle's first dword (0x0) points at its read-ahead buffer. No include/ struct has this
// layout (the same handle/buffer shape as unit_10052D00.c's PackFsFile_guess/PackFsBuffer_guess).
typedef struct T3dFastFileHandle_guess
{
    void *buffer_guess; // 0x0
} T3dFastFileHandle_guess;

typedef struct T3dFastFileData_guess
{
    char pad_0000[0x8];
    T3dFastFileHandle_guess *handle_guess; // 0x8
} T3dFastFileData_guess;

typedef struct T3dFastFile_guess
{
    T3dFastFileData_guess *data_guess; // 0x0
} T3dFastFile_guess;

// getFilePosition, 0x10052d00: __fastcall (the fast file handle arrives in ECX, no stack arguments). One-pointer
// form: no dummy-EDX signature, so the call is MOV ECX,handle / CALL like the original's.
#define getFilePosition_guess EQ_THISCALL(int, 0x10052D00, (T3dFastFileHandle_guess *))

// FUNCTION: EQGFX 0x1002191d
// Reports where fast-file reading currently stands.
// __cdecl (the callee pops 0 bytes), one stack argument. -1 when the file or its buffer is missing, else the
// position the read-ahead buffer is at (getFilePosition, 0x10052D00).
int __cdecl t3dFastFileTell(T3dFastFile_guess *file)
{
    int result;
    T3dFastFileHandle_guess *handle;

    result = -1;
    if (file != 0)
    {
        handle = file->data_guess->handle_guess;
        if (handle->buffer_guess != 0)
            result = getFilePosition_guess(handle);
    }
    return result;
}
