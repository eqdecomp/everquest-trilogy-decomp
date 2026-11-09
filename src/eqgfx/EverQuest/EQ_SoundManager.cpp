#include "trilogy/eqaddr.h"

#define EQGFX_PACK_FS_INITIALIZE_ADDRESS 0x10021938
#define EQGFX_NORMALIZE_FILE_PATH_ADDRESS 0x10021D55
#define EQGFX_COPY_STRING_REFERENCE_ADDRESS 0x10021C94
#define EQGFX_DECREMENT_REFERENCE_COUNT_ADDRESS 0x1002190D

class FilePath
{
public:
    FilePath *FilePath_guess(const char *name);
};

namespace
{
    struct PackFSDataInput_guess
    {
        int reference;
        int unknown;

        PackFSDataInput_guess(const char *name)
        {
            EQ_METHOD(1, void, EQGFX_PACK_FS_INITIALIZE_ADDRESS, (void *, const char *), this)(name);
        }

        ~PackFSDataInput_guess()
        {
            if (reference != 0)
                EQ_METHOD(0, void, EQGFX_DECREMENT_REFERENCE_COUNT_ADDRESS, (void *), (void *)reference)();
        }
    };

    struct NormalizedFilePath_guess
    {
        int reference;
        int unknown;

        ~NormalizedFilePath_guess()
        {
            if (reference != 0)
                EQ_METHOD(0, void, EQGFX_DECREMENT_REFERENCE_COUNT_ADDRESS, (void *), (void *)reference)();
        }
    };
}

// FUNCTION: EQGFX 0x10021cc5
// Stores a normalized path for later file access.
FilePath *FilePath::FilePath_guess(const char *name)
{
    void * volatile filePath = this;
    EQ_METHOD(1, void, EQGFX_PACK_FS_INITIALIZE_ADDRESS, (void *, const char *), filePath)(0);
    PackFSDataInput_guess input(name);
    NormalizedFilePath_guess normalized;
    EQ_METHOD(1, void, EQGFX_COPY_STRING_REFERENCE_ADDRESS, (void *, void *), filePath)(
        EQ_METHOD(1, void *, EQGFX_NORMALIZE_FILE_PATH_ADDRESS, (void *, void *), &normalized)(&input));
    return this;
}
