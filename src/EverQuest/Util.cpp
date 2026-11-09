#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/soundmanager.h"

class EqSoundControlUnload_guess
{
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0C();
    virtual void UnloadWave_guess(int index);
};

// FUNCTION: TRILOGY 0x004e3833
// Despite its misleading name, this unloads a sound-manager wave by ID.
// @early-stop: the error path's two cdecl cleanups merge into one add esp,0x10; the original pops each call's args right after it
// ORIGINAL FILE: EverQuest/Util.cpp (from Mac debug map)
void __fastcall TextFileReader_TextFileReader_guess(EqSoundManager_guess* self, int unused_edx, int index)
{
    EqSoundControlUnload_guess* control = (EqSoundControlUnload_guess*)EQ_GLOBAL(void*, 0x006E4DD0);
    if (control != 0)
    {
        if (!(index < 0 || 0x2D0 <= index))
        {
            control->UnloadWave_guess(index);
            self->waves[index] = 0;
        }
        else
        {
            const char* msg = (const char*)0x0057BEF0;
            EQ_CDECL(void, 0x004A3D01, (const char*, int))(msg, index);
            EQ_CDECL(void, 0x004C1703, (const char*, ...))(msg, index);
        }
    }
}
