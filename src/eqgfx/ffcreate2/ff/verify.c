// CFLAGS: /O2
// src/eqgfx/ffcreate2/ff/verify.c - eqgfx_dx7.dll fast-file verify helpers.

// Local layout guess: a handle whose first field points at file data
// holding the file size at +0x2008. No header defines these yet.
struct EqgfxFileData_guess {
    char pad_0000[0x2008];
    int fileSize_guess; // 0x2008
};

struct EqgfxFileHandle_guess {
    struct EqgfxFileData_guess *data_guess; // 0x0
};

// FUNCTION: EQGFX 0x10052a80
// __fastcall (this in ECX, no stack args). Returns the file size.
int __fastcall getFileSize_guess(struct EqgfxFileHandle_guess *handle)
{
    return handle->data_guess->fileSize_guess;
}
