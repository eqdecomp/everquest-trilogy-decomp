// unit_10021B49.c - eqgfx_dx7.dll, the address run 0x10021B49..0x10021BBF (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// Local layout guess: a holder whose first field points at a reference counted
// object holding a value at +0x8. No header defines these yet.
struct SharedValue10021B49_guess {
    void *vtable; // 0x0
    int refCount; // 0x4
    int value; // 0x8
};

struct SharedHolder10021B49_guess {
    struct SharedValue10021B49_guess *object; // 0x0
};

struct FileLine10021B49_guess {
    const char *file; // 0x0
    int line; // 0x4
};

#define operator_new_guess EQ_CDECL(void *, 0x10068C72, (unsigned int))
#define verifyConditionOrFatalError_guess EQ_CDECL(void, 0x100549A0, (void *, const char *, const char *))
#define g_allocCount_guess EQ_GLOBAL(unsigned int, 0x100FC014)
#define g_allocLog_guess EQ_ARRAY(struct FileLine10021B49_guess, 0x100FBC10)
#define EQGFX_SHARED_VTABLE_GUESS 0x100762b4
#define EQGFX_SHARE_FILE_GUESS ((const char *)0x10087048)
#define EQGFX_ALLOC_ERROR_GUESS ((const char *)0x10087020)
#define EQGFX_NEW_SHARED_EXPR_GUESS ((const char *)0x10087060)
#define EQGFX_ALLOC_LOG_CAPACITY_GUESS 0x40
#define EQGFX_NEW_SHARED_LINE_GUESS 0x102
#define EQGFX_SHARED_OBJECT_SIZE_GUESS 0xc

// FUNCTION: EQGFX 0x10021b49
// Lets file users share an opened renderer resource.
// Shared holder constructor: clears the holder, wraps a nonzero value in a new
// reference counted object and takes the first reference.
struct SharedHolder10021B49_guess *__fastcall ConstructSharedHolder_guess(
    struct SharedHolder10021B49_guess *holder, int unusedEdx, int value)
{
    struct SharedValue10021B49_guess *newObject;
    unsigned int count;

    holder->object = 0;
    if (value != 0)
    {
        newObject = operator_new_guess(EQGFX_SHARED_OBJECT_SIZE_GUESS);
        if (newObject != 0)
        {
            newObject->refCount = 0;
            newObject->value = value;
            newObject->vtable = (void *)EQGFX_SHARED_VTABLE_GUESS;
        }
        else
        {
            newObject = 0;
        }
        count = g_allocCount_guess;
        if (count < EQGFX_ALLOC_LOG_CAPACITY_GUESS)
        {
            g_allocLog_guess[count].file = EQGFX_SHARE_FILE_GUESS;
            g_allocLog_guess[count].line = EQGFX_NEW_SHARED_LINE_GUESS;
            g_allocCount_guess = count + 1;
        }
        if (newObject == 0)
            verifyConditionOrFatalError_guess(newObject, EQGFX_ALLOC_ERROR_GUESS, EQGFX_NEW_SHARED_EXPR_GUESS);
        holder->object = newObject;
        newObject->refCount++;
    }
    return holder;
}
