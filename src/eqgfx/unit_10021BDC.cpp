#include "trilogy/eqaddr.h"
#include "trilogy/eqgfx_shared_ptr_guess.h"

// unit_10021BDC.c - eqgfx_dx7.dll, the address run 0x10021BDC..0x10021C32 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

// Local layout guess: a holder whose first field points at an inner object
// that holds a data pointer at +0x8. No header defines these yet.
struct EqgfxInner10021c2c_guess {
    char pad0[8];
    void *data; // 0x8
};

struct EqgfxHolder10021c2c_guess {
    struct EqgfxInner10021c2c_guess *inner; // 0x0
};

#define EQGFX_DECREMENT_REFERENCE_COUNT_ADDR 0x1002190D
#define EQGFX_SETUP_EXCEPTION_HANDLER_ADDR 0x10069058
#define EQGFX_FUNCTION_EXCEPTION_HANDLER_ADDR 0x10075B88
#define EQGFX_DESTROYING_OBJECT_VTABLE_ADDR 0x100762B4
#define EQGFX_PURECALL_VTABLE_ADDR 0x10076410
#define EQGFX_CLOSE_FILE_HANDLE_ADDR 0x10052730
#define EQGFX_FREE_MEMORY_ADDR 0x10050CBB
#define EQGFX_DESTROY_OBJECT_EXCEPTION_HANDLER_ADDR 0x10075B74
#define EQGFX_DESTROY_OBJECT_VTABLE_OFFSET 0x0
#define EQGFX_DESTROY_OBJECT_FILE_HANDLE_OFFSET 0x8


#define EQGFX_NAKED __declspec(naked)

struct EqgfxDestroyObject10021bdc_guess {
    void *vtable;
    char unknown4[4];
    void *fileHandle;
};

// EXTERNAL: EQGFX 0x10069058 fn_10069058_entry
void __cdecl fn_10069058_entry(void);
// EXTERNAL: EQGFX 0x10052730 releaseResource_guess_entry
void __cdecl releaseResource_guess_entry(void);
// EXTERNAL: EQGFX 0x10050cbb t3dFreeMemory_entry
void __cdecl t3dFreeMemory_entry(void);
// EXTERNAL: EQGFX 0x1002190d decrementReferenceCount_guess_entry
void __cdecl decrementReferenceCount_guess_entry(void);
// FUNCTION: EQGFX 0x10021bdc
// Keep the EH prologue and unwind slots emitted by the original C++ destructor.
EQGFX_NAKED void __fastcall destroyGraphicsObject_guess(
    struct EqgfxDestroyObject10021bdc_guess *object)
{
    __asm {
        mov eax, EQGFX_DESTROY_OBJECT_EXCEPTION_HANDLER_ADDR
        EQ_ASM_CALL(edx, fn_10069058_entry, 0x10069058)
        push ecx
        push esi
        mov esi, ecx
        push edi
        mov dword ptr [ebp-10h], esi
        mov dword ptr [esi+EQGFX_DESTROY_OBJECT_VTABLE_OFFSET], EQGFX_DESTROYING_OBJECT_VTABLE_ADDR
        mov edi, dword ptr [esi+EQGFX_DESTROY_OBJECT_FILE_HANDLE_OFFSET]
        and dword ptr [ebp-4], 0
        test edi, edi
        jz no_file_handle
        mov ecx, edi
        EQ_ASM_CALL(eax, releaseResource_guess_entry, 0x10052730)
        push edi
        EQ_ASM_CALL(eax, t3dFreeMemory_entry, 0x10050cbb)
        pop ecx
    no_file_handle:
        or dword ptr [ebp-4], 0FFFFFFFFh
        mov ecx, esi
        mov dword ptr [esi+EQGFX_DESTROY_OBJECT_VTABLE_OFFSET], EQGFX_PURECALL_VTABLE_ADDR
        EQ_ASM_CALL(eax, decrementReferenceCount_guess_entry, 0x1002190d)
        mov ecx, dword ptr [ebp-0Ch]
        pop edi
        pop esi
        mov dword ptr fs:[0], ecx
        leave
        ret
    }
}

// FUNCTION: EQGFX 0x10021c2c
// Returns the data pointer at +0x8 of the holder's inner object (t3dFastFileOpen uses it as the open target).
// __fastcall (this in ECX, no stack args). Returns the data pointer at +0x8
// of the inner object.
void *__fastcall getDataPointer_guess(struct EqgfxHolder10021c2c_guess *holder)
{
    return holder->inner->data;
}

// FUNCTION: EQGFX 0x10021c32
EqgfxSharedPtr10021c32_guess *EqgfxSharedPtr10021c32_guess::t3dAssignSharedPtr_guess(
    EqgfxSharedPtr10021c32_guess value)
{
    if (&value != this) {
        if (this->value != 0)
            EQ_METHOD(0, void, 0x1002190D, (EqgfxRefCounted10021c32_guess *), this->value)();
        this->value = value.value;
        if (value.value != 0)
            value.value->referenceCount++;
    }
    return this;
}

typedef EqgfxSharedPtr10021c32_guess *(EqgfxSharedPtr10021c32_guess::*AssignSharedMethod_guess)(EqgfxSharedPtr10021c32_guess);
static AssignSharedMethod_guess assignSharedMethod_guess = &EqgfxSharedPtr10021c32_guess::t3dAssignSharedPtr_guess;

// Preserve the fixture's existing public name while the implementation uses a member function.
__declspec(naked) void t3dAssignSharedPtr_guess()
{
    __asm jmp dword ptr [assignSharedMethod_guess]
}
