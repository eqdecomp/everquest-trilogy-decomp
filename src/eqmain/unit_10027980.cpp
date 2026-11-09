// EXTERNAL: EQMAIN 0x100338E5 ??2@YAPAXI@Z
// unit_10027980.cpp - eqmain.dll, the address run 0x10027980..0x10028750 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <string.h>
#include "trilogy/eqaddr.h"
#include <string.h>

/* LogFont_guess: the actor layout is LogFont_guess (generated: eqmain/layout/font_cache.h); the old local view's members map by offset. */
#include "eqmain/layout/font_cache.h"

typedef void *(__stdcall *CreateFontIndirectA_fn)(const void *);

// FUNCTION: EQMAIN 0x10027980
// Creates the front-end text font represented by the requested LOGFONT style.
// __thiscall (this in ECX, one stack argument, popped by the callee; written as __fastcall with an EDX dummy).
// Copies the 60-byte LOGFONTA at param to record+4 (rep movsd of 15 dwords), creates the font from param with
// CreateFontIndirectA (imported through the pointer at 0x10040034), stores the handle in record[0] and returns
// whether it is non-null.
bool __fastcall CreateFontIntoRecord_guess(unsigned int *record, int /* EDX dummy */, const LogFont_guess *logFont)
{
    void *font;

    *(LogFont_guess *)(record + 1) = *logFont;
    font = EQ_GLOBAL(CreateFontIndirectA_fn, 0x10040034)(logFont);
    *(void **)record = font;
    return font != 0;
}

// FUNCTION: EQMAIN 0x100279B0
// Lets the font cache recognize an existing style and face name.
// __thiscall (this in ECX, one stack argument, popped by the callee; written as __fastcall with an EDX dummy).
// self is a list entry: a link at +0x00, the key at +0x04 (five dwords at +0x04..+0x17, eight bytes at
// +0x18..+0x1F) and the name at +0x20. param is a key: dwords at +0x00..+0x13, bytes at +0x14..+0x1B, name at +0x1C.
// Returns 1 when the 13 key fields are equal and the names are equal (strcmp: unsigned bytes up to the NUL; /O2
// expands the intrinsic inline, two bytes per step, as the original does).
int __fastcall EntryMatchesKey_guess(unsigned char *self, int /* EDX dummy */, unsigned char *param)
{
    if (((FontCacheEntry_guess*)self)->key_guess.words[0] == *(unsigned int *)param &&
        ((FontCacheEntry_guess*)self)->key_guess.words[1] == *(unsigned int *)(param + 0x04) &&
        ((FontCacheEntry_guess*)self)->key_guess.words[2] == *(unsigned int *)(param + 0x08) &&
        ((FontCacheEntry_guess*)self)->key_guess.words[3] == *(unsigned int *)(param + 0x0C) &&
        ((FontCacheEntry_guess*)self)->key_guess.words[4] == *(unsigned int *)(param + 0x10) &&
        self[0x18] == param[0x14] &&
        self[0x19] == param[0x15] &&
        self[0x1A] == param[0x16] &&
        self[0x1B] == param[0x17] &&
        self[0x1C] == param[0x18] &&
        self[0x1D] == param[0x19] &&
        self[0x1E] == param[0x1A] &&
        self[0x1F] == param[0x1B])
    {
        if (strcmp((const char *)(self + 0x20), (const char *)(param + 0x1C)) == 0)
            return 1;
    }
    return 0;
}

struct FontManagerCreation_guess
{
    int value;

    // EXTERNAL: EQMAIN 0x10027B50 FontManagerCreation_guess::FontManagerCreation_guess
    FontManagerCreation_guess();
};

// FUNCTION: EQMAIN 0x10027AA0
// Makes the front-end font manager available for text rendering.
void __stdcall CreateGlobalFontManager_guess(void)
{
    if (EQ_GLOBAL(int *, 0x1012C7B8) == 0)
    {
        EQ_GLOBAL(int *, 0x1012C7B8) = (int *)new FontManagerCreation_guess;
        if (EQ_GLOBAL(int *, 0x1012C7B8) == 0)
        {
            EQ_CDECL_CPP(void, 0x10027940, (const char *))(EQ_ARRAY(const char, 0x100487EC));
            EQ_STDCALL(void, 0x10035E98, (int))(-1);
        }
    }
}

// FUNCTION: EQMAIN 0x10027B20
// Releases the front-end font manager and its queued entries during cleanup.
// __stdcall, no arguments (the callee pops nothing, so the code is the same as __cdecl). If the queue pointer in the
// global at 0x1012C7B8 is non-null, the queue is destroyed: DestroyQueue_guess (0x10027B70, __thiscall, this in ECX)
// runs on it, FUN_10033800 (0x10033800, __cdecl, one argument) frees it, and the global is cleared to 0. Returns nothing.
void __stdcall DestroyGlobalFontManager_guess(void)
{
    int *object;

    object = EQ_GLOBAL(int *, 0x1012C7B8);
    if (object != 0)
    {
        EQ_METHOD(0, void, 0x10027B70, (int *), object)();
        EQ_CDECL(void, 0x10033800, (void *))(object);
        EQ_GLOBAL(int *, 0x1012C7B8) = 0;
    }
}

// FUNCTION: EQMAIN 0x10027B50
// The game purpose of this 12-byte record is unclear.
// __thiscall (this in ECX, no stack arguments; written as __fastcall with one argument, ECX). Allocates a 12-byte
// record with operator_new (0x100338E5, __cdecl) and stores its pointer at this+0x00, even when the allocation
// failed (the pointer is then 0). A non-null record is cleared by ClearRecord3_guess (0x10028330, __cdecl, one
// argument: sets record+0x08 to 0). Returns this in EAX.
unsigned int *__fastcall AllocRecord3_guess(unsigned int *self)
{
    void *record;

    record = EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x0C);
    *(void **)self = record;
    if (record != 0)
        EQ_CDECL(void, 0x10028330, (void *))(record);
    return self;
}

// FUNCTION: EQMAIN 0x10027B70
// The queue's game purpose is unclear; this empties it during front-end cleanup.
// __thiscall (this in ECX, no stack arguments; written as __fastcall with one argument, ECX). Does nothing when the
// queue pointer at this+0x00 is null. Otherwise, while the queue's count at +0x08 is non-zero, pops its head with
// list3_unlink_free_guess (0x10028430, __cdecl, (queue, head); returns the item, or 0); a non-null item goes to
// DeleteGdiObjectAndClearWord_guess (0x10027960, __fastcall, ECX) and is freed with FUN_10033800 (0x10033800, __cdecl). Then
// list3_free_all_guess (0x10028480, __cdecl) runs on the queue and FUN_10033800 frees it. this+0x00 is not cleared, so it
// dangles afterwards. Returns nothing.
void __fastcall DestroyQueue_guess(int *self)
{
    int *queue;
    int count;
    unsigned int *item;

    queue = (int *)*self;
    if (queue != 0)
    {
        count = queue[2];
        while (count != 0)
        {
            item = (unsigned int *)EQ_CDECL(int, 0x10028430, (int *, int *))(queue, (int *)*queue);
            if (item != 0)
            {
                EQ_METHOD(0, void, 0x10027960, (void *), item)();
                EQ_CDECL(void, 0x10033800, (void *))(item);
            }
            queue = (int *)*self;
            count = queue[2];
        }
        EQ_CDECL(void, 0x10028480, (int *))((int *)*self);
        EQ_CDECL(void, 0x10033800, (void *))((void *)*self);
    }
    return;
}

// FUNCTION: EQMAIN 0x10027BD0
// Requests a front-end text font using explicit size, weight, and face settings.
// __thiscall (this in ECX, fourteen stack arguments, popped by the callee: RET 0x38; written as __fastcall with an
// EDX dummy). Builds a LOGFONTA from the arguments: five dwords at +0x00..+0x10, eight bytes at +0x14..+0x1B, then
// the face name at +0x1C, copied with strcpy (no bounds check: a name of 32 or more characters overruns the
// buffer into the return address). Then FontCache_GetOrCreate (0x10027C80) gets the LOGFONTA on the same this, and
// its result is returned in EAX unchanged.
unsigned int __fastcall FontCache_GetOrCreateByParams_guess(unsigned char *self, int /* EDX dummy */, int height,
                                                            int width, int escapement, int orientation, int fontWeight,
                                                            unsigned char italic, unsigned char underline,
                                                            unsigned char strike_out, unsigned char char_set,
                                                            unsigned char out_precision, unsigned char clip_precision,
                                                            unsigned char quality, unsigned char pitch_and_family,
                                                            const char *face_name)
{
    LogFont_guess local;

    local.words[0] = height;
    local.words[1] = width;
    local.words[2] = escapement;
    local.words[3] = orientation;
    local.words[4] = fontWeight;
    ((unsigned char *)&local)[0x14] = italic;
    ((unsigned char *)&local)[0x15] = underline;
    ((unsigned char *)&local)[0x16] = strike_out;
    ((unsigned char *)&local)[0x17] = char_set;
    ((unsigned char *)&local)[0x18] = out_precision;
    ((unsigned char *)&local)[0x19] = clip_precision;
    ((unsigned char *)&local)[0x1A] = quality;
    ((unsigned char *)&local)[0x1B] = pitch_and_family;
    strcpy((char *)&local + 0x1C, face_name);
    // The original still has this in EDX at the call (MOV EDX,ECX at entry), so the dummy is this too.
    return EQ_THISCALL(unsigned int, 0x10027C80, (unsigned char *, int, LogFont_guess *))(self, (int)self, &local);
}

// FUNCTION: EQMAIN 0x10027C80
// Supplies a shared font handle for a LOGFONT style, reusing matching entries.
// __thiscall (this in ECX, one stack argument, popped by the callee: RET 4; written as __fastcall with an EDX dummy).
// self: dword +0 is a list struct whose dword +0 is the first node. A node: entry at +0x04, next node at +0x08.
// An entry is 0x44 bytes: handle dword at +0x00, LOGFONTA key at +0x04..+0x3F (EntryMatchesKey_guess), refcount
// short at +0x40. param is a LOGFONTA (0x3C bytes). The first entry whose key matches gets its refcount bumped and
// its handle returned. Otherwise a new entry (operator new 0x44) goes through ClearRecordHead_guess, CreateFontIntoRecord_guess makes it
// from a copy of param, its refcount is bumped, list3_insert_guess links it into the list, and its handle is returned.
// 0 when the allocation or ClearRecordHead_guess fails.
unsigned int __fastcall FontCache_GetOrCreate_guess(unsigned char *self, int /* EDX dummy */, LogFont_guess *logFont)
{
    unsigned char *node;
    unsigned char *entry;
    LogFont_guess local;

    for (node = *(unsigned char **)*(unsigned char **)self; node != 0; node = *(unsigned char **)(node + 0x08))
    {
        entry = *(unsigned char **)(node + 0x04);
        if (entry != 0 && EQ_METHOD(1, unsigned char, 0x100279B0, (unsigned char *, LogFont_guess *), entry)(logFont))
        {
            ++*(short *)(entry + 0x40);
            return *(unsigned int *)entry;
        }
    }
    entry = (unsigned char *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x44);
    entry = entry != 0 ?
        (unsigned char *)EQ_METHOD(0, void *, 0x10027950, (void *), entry)() :
        (unsigned char *)0;
    if (entry == 0)
    {
        return 0;
    }
    local = *logFont;
    EQ_METHOD(1, void, 0x10027980, (unsigned char *, LogFont_guess *), entry)(&local);
    ++*(short *)(entry + 0x40);
    EQ_CDECL(void, 0x10028340, (void *, int, void *))(*(void **)self, 0, entry);
    return *(unsigned int *)entry;
}

// FUNCTION: EQMAIN 0x10027D20
// Retrieves the style settings for a font handle used by front-end text drawing.
// __thiscall (this in ECX, two stack arguments, popped by the callee: RET 8; written as __fastcall with an EDX dummy).
// self: dword +0 is a list struct whose dword +0 is the first node. A node: entry at +0x04, next node at +0x08.
// An entry: handle dword at +0x00, LOGFONTA key at +0x04..+0x3F. The first entry whose handle dword is `handle` (a
// null entry never matches) is found. Found: out gets the LOGFONTA fields (dwords 0..4 = lfHeight..lfWeight, bytes
// 0x14..0x1B = lfItalic..lfPitchAndFamily, the face name by strcpy at 0x1C). Each field is read from a fresh copy of
// the key, as the original does. Returns 1 when found, 0 otherwise (out untouched).
int __fastcall FontCache_GetLogFontByHandle_guess(unsigned char *self, int /* EDX dummy */, int handle, int *font)
{
    unsigned char *node;
    unsigned char *entry;
    LogFont_guess *key;
    LogFont_guess local;

    for (node = *(unsigned char **)*(unsigned char **)self; node != 0; node = *(unsigned char **)(node + 0x08))
    {
        entry = *(unsigned char **)(node + 0x04);
        if (entry != 0 && *(int *)entry == handle)
        {
            key = (LogFont_guess *)(entry + 0x04);
            local = *key;
            font[0] = local.words[0];
            local = *key;
            font[1] = local.words[1];
            local = *key;
            font[2] = local.words[2];
            local = *key;
            font[3] = local.words[3];
            local = *key;
            font[4] = local.words[4];
            local = *key;
            *(unsigned char *)(font + 5) = *((unsigned char *)&local + 0x14);
            local = *key;
            *((unsigned char *)font + 0x15) = *((unsigned char *)&local + 0x15);
            local = *key;
            *((unsigned char *)font + 0x16) = *((unsigned char *)&local + 0x16);
            local = *key;
            *((unsigned char *)font + 0x17) = *((unsigned char *)&local + 0x17);
            local = *key;
            *((unsigned char *)font + 0x18) = *((unsigned char *)&local + 0x18);
            local = *key;
            *((unsigned char *)font + 0x19) = *((unsigned char *)&local + 0x19);
            local = *key;
            *((unsigned char *)font + 0x1a) = *((unsigned char *)&local + 0x1a);
            local = *key;
            *((unsigned char *)font + 0x1b) = *((unsigned char *)&local + 0x1b);
            local = *key;
            strcpy((char *)(font + 7), (char *)&local + 0x1c);
            return 1;
        }
    }
    return 0;
}

typedef unsigned int (__stdcall *DIDevGetDataFn_guess)(void *, unsigned int, void *, unsigned int *, unsigned int);
typedef unsigned int (__stdcall *DIDevAcquireFn_guess)(void *);

typedef unsigned int (__stdcall *ComSlot8Fn_guess)(void *);     // vtable slot 8 (0x20): result not used
typedef unsigned int (__stdcall *ComReleaseFn_guess)(void *);   // IUnknown::Release, vtable slot 2 (0x08)

#define g_com_1012c7bc_guess  EQ_GLOBAL(void *, 0x1012C7BC)     /* DAT_1012c7bc: a COM object */

typedef unsigned int (__stdcall *ComAddRefFn_guess)(void *);                                       // IUnknown::AddRef, vtable slot 1 (0x04)
typedef unsigned int (__stdcall *DICreateDeviceFn_guess)(void *, const void *, void **, void *);   // IDirectInputA::CreateDevice, slot 3 (0x0C)
typedef unsigned int (__stdcall *DISetCoopFn_guess)(void *, int, unsigned int);                    // IDirectInputDevice::SetCooperativeLevel, slot 13 (0x34)
typedef unsigned int (__stdcall *DISetFormatFn_guess)(void *, const void *);                       // IDirectInputDevice::SetDataFormat, slot 11 (0x2C)
typedef unsigned int (__stdcall *DISetPropertyFn_guess)(void *, const void *, const void *);       // IDirectInputDevice::SetProperty, slot 6 (0x18)

struct Guid_guess
{
    unsigned int words[4];          // 0x00..0x0C: GUID_SysKeyboard, copied whole
};

struct DIPropDword_guess            // DIPROPDWORD: DIPROPHEADER (0x10 bytes) + dwData
{
    unsigned int dwSize;            // 0x00: 0x14
    unsigned int dwHeaderSize;      // 0x04: 0x10
    unsigned int dwObj;             // 0x08: 0
    unsigned int dwHow;             // 0x0C: DIPH_DEVICE
    unsigned int dwData;            // 0x10: DIPROP_BUFFERSIZE value, 32
};

#define g_keyboard_guid_guess    EQ_GLOBAL(Guid_guess, 0x100405A0)                                                   /* GUID_SysKeyboard (.rdata) */
#define fopen_guess              EQ_CDECL(void *, 0x1003407F, (const char *, const char *))                           /* fopen */
#define fread_guess              EQ_CDECL(unsigned int, 0x100342A2, (void *, unsigned int, unsigned int, void *))    /* fread */
#define fwrite_guess             EQ_CDECL(unsigned int, 0x10034169, (const void *, unsigned int, unsigned int, void *)) /* fwrite */
#define fclose_guess             EQ_CDECL(int, 0x100343B9, (void *))                                                  /* fclose */
#define DirectInputCreate_guess  EQ_STDCALL(int, 0x10033320, (int, unsigned int, void **, void *))                    /* DirectInputCreateA thunk */
#define ReleaseHeld_guess(object) EQ_METHOD(0, void, 0x100280A0, (void *), (object))                                         /* ReleaseHeldAndGlobal_guess: this in ECX, EDX dummy */

// FUNCTION: EQMAIN 0x10027EA0
// Enables front-end keyboard input and loads its configured key mappings.
// __thiscall (this in ECX, two stack arguments, popped by the callee: RET 8; written as __fastcall with an EDX dummy).
// Keyboard.txt: opened with the mode at 0x1004858C and read into the key tables 0x10048818 and 0x10048918, plus 0x1012C7C0
// unless the flags byte at +0x0C has 0x10. When it does not open, the mode at 0x10048A18 creates it from the two tables.
// Global 0x1012C7BC: an IDirectInputA, made by DirectInputCreateA when null, else AddRef'd. self+4 gets the keyboard
// device from CreateDevice(GUID_SysKeyboard); then SetCooperativeLevel(hwnd, 10), SetDataFormat(0x100430A8),
// SetProperty(DIPROP_BUFFERSIZE, 32) and Acquire. Returns 1 or 2 with nothing released, 3, 4, 5 or Acquire's HRESULT
// after ReleaseHeldAndGlobal_guess releases the device, 0 when all succeed.
int __fastcall InitKeyboardDevice_guess(unsigned char *self, int /* EDX dummy */, int hInstance, int hwnd)
{
    DIPropDword_guess prop;
    Guid_guess guid;
    void *file;
    void **pdev;
    void *dev;
    int hr;

    guid = g_keyboard_guid_guess;
    file = fopen_guess((const char *)0x10048A1C, (const char *)0x1004858C);
    if (file == 0)
    {
        file = fopen_guess((const char *)0x10048A1C, (const char *)0x10048A18);
        if (file != 0)
        {
            fwrite_guess((const void *)0x10048818, 0x100, 1, file);
            fwrite_guess((const void *)0x10048918, 0x100, 1, file);
            fclose_guess(file);
        }
    }
    else
    {
        fread_guess((void *)0x10048818, 0x100, 1, file);
        fread_guess((void *)0x10048918, 0x100, 1, file);
        if ((*((unsigned char *)file + 0x0C) & 0x10) == 0)
            fread_guess((void *)0x1012C7C0, 0x100, 1, file);
        fclose_guess(file);
    }
    if (g_com_1012c7bc_guess != 0)
    {
        ((ComAddRefFn_guess)(*(void ***)g_com_1012c7bc_guess)[1])(g_com_1012c7bc_guess);
    }
    else
    {
        hr = DirectInputCreate_guess(hInstance, 0x500, &g_com_1012c7bc_guess, 0);
        if (hr != 0)
            return 1;
    }
    pdev = (void **)(self + 0x04);
    hr = ((DICreateDeviceFn_guess)(*(void ***)g_com_1012c7bc_guess)[3])(g_com_1012c7bc_guess, &guid, pdev, 0);
    if (hr != 0)
        return 2;
    dev = *pdev;
    hr = ((DISetCoopFn_guess)(*(void ***)dev)[13])(dev, hwnd, 10);
    if (hr != 0)
    {
        ReleaseHeld_guess(self)();
        return 3;
    }
    dev = *pdev;
    hr = ((DISetFormatFn_guess)(*(void ***)dev)[11])(dev, (const void *)0x100430A8);
    if (hr != 0)
    {
        ReleaseHeld_guess(self)();
        return 4;
    }
    dev = *pdev;
    prop.dwSize = 0x14;
    prop.dwHeaderSize = 0x10;
    prop.dwObj = 0;
    prop.dwHow = 0;
    prop.dwData = 0x20;
    hr = ((DISetPropertyFn_guess)(*(void ***)dev)[6])(dev, (const void *)1, &prop);
    if (hr != 0)
    {
        ReleaseHeld_guess(self)();
        return 5;
    }
    dev = *pdev;
    hr = ((DIDevAcquireFn_guess)(*(void ***)dev)[7])(dev);
    if (hr != 0)
    {
        ReleaseHeld_guess(self)();
        return hr;
    }
    return 0;
}

// FUNCTION: EQMAIN 0x100280A0
// Releases the keyboard device and DirectInput reference during input cleanup.
// __thiscall (this in ECX, no stack arguments, callee pops nothing; written as __fastcall with an EDX dummy).
// self: +4 a COM object. When it is not null: slot 8 (0x20), then Release (slot 2) on it, then +4 is cleared.
// Global 0x1012C7BC: a COM object. When it is not null: Release (slot 2), and the global is cleared when Release
// returns 0 (the original stores the returned EAX, which is 0 there). Returns nothing: callers never read EAX.
void __fastcall ReleaseHeldAndGlobal_guess(unsigned char *self, int /* EDX dummy */)
{
    void *obj;
    unsigned int result;

    obj = *(void **)(self + 0x04);
    if (obj != 0)
    {
        ((ComSlot8Fn_guess)(*(void ***)obj)[8])(obj);
        obj = *(void **)(self + 0x04);
        ((ComReleaseFn_guess)(*(void ***)obj)[2])(obj);
        *(void **)(self + 0x04) = 0;
    }

    obj = g_com_1012c7bc_guess;
    if (obj != 0)
    {
        result = ((ComReleaseFn_guess)(*(void ***)obj)[2])(obj);
        if (result == 0)
        {
            g_com_1012c7bc_guess = (void *)result;
        }
    }
}

// FUNCTION: EQMAIN 0x100280E0
// Converts a key press or release into a front-end key event with modifier state.
// __thiscall (this in ECX, one stack argument, popped by the callee: RET 4; written as __fastcall with an EDX dummy).
// self: +0 modifier bits (0x01 LAlt, 0x02 RAlt, 0x04 LShift, 0x08 RShift); +4 the DirectInput keyboard device;
// +8 + key the modifier bits saved when that key went down. Reads one buffered event (GetDeviceData, vtable slot 10,
// count 1). Keys 0x38, 0xB8, 0x2A, 0x36 set or clear their modifier bit and return 0. Any other key: event[0] = key;
// press: event[1] = modifier bits, and the key's slot saves them; release: event[1] = saved bits | 0x80. Returns 1.
// No event returns 0. DIERR_INPUTLOST (0x8007001E) reacquires the device (vtable slot 7); the reacquire result, or hr
// for any other error, is left in EAX. The return is a byte (callers read AL only): the error and no-event paths
// clear AL and keep the rest of EAX, so EAX ends as hr & 0xFFFFFF00, or the reacquire result & 0xFFFFFF00.
unsigned char __fastcall DIKeyboard_ReadOneEvent_guess(unsigned char *self, int /* EDX dummy */, unsigned char *event)
{
    unsigned int hr;
    unsigned int local[5];      // local[0]: count (1 in, events read out); local[1..4]: one DIDEVICEOBJECTDATA
    void *obj;

    event[0] = 0;
    event[1] = 0;
    obj = *(void **)(self + 4);
    local[0] = 1;
    hr = ((DIDevGetDataFn_guess)(*(void ***)obj)[10])(obj, 0x10, &local[1], &local[0], 0);
    if (hr == 0)
    {
        if (local[0] > 0)
        {
            switch (local[1])
            {
            case 0x38:
                if (((unsigned char *)local)[8] & 0x80)
                {
                    *self |= 0x01;
                }
                else
                {
                    *self &= 0xfe;
                }
                return 0;
            case 0xb8:
                if (((unsigned char *)local)[8] & 0x80)
                {
                    *self |= 0x02;
                }
                else
                {
                    *self &= 0xfd;
                }
                return 0;
            case 0x2a:
                if (((unsigned char *)local)[8] & 0x80)
                {
                    *self |= 0x04;
                }
                else
                {
                    *self &= 0xfb;
                }
                return 0;
            case 0x36:
                if (((unsigned char *)local)[8] & 0x80)
                {
                    *self |= 0x08;
                }
                else
                {
                    *self &= 0xf7;
                }
                return 0;
            default:
                event[0] = (unsigned char)local[1];
                if (!(((unsigned char *)local)[8] & 0x80))
                {
                    event[1] = 0x80;
                    event[1] = self[8 + (unsigned char)local[1]] | 0x80;
                    return 1;
                }
                else
                {
                    event[1] = *self;
                    self[8 + (unsigned char)local[1]] = *self;
                    return 1;
                }
            }
        }
    }
    else if (hr == 0x8007001e)
    {
        obj = *(void **)(self + 4);
        ((DIDevAcquireFn_guess)(*(void ***)obj)[7])(obj);
    }
    return 0;
}

// FUNCTION: EQMAIN 0x100282E0
// Selects the character mapping for a key according to its keyboard flags.
// __stdcall (one stack argument, popped by the callee: RET 4). Callers set ECX, but the body never reads it.
// Flags byte param[1]: any of 0x31 set returns 0; else 0x0C set returns DAT_10048918[param[0]]; else 0x02 set
// returns DAT_1012C7C0[param[0]], and otherwise DAT_10048818[param[0]].
// @early-stop: VC6 needs the cursor step to keep the parameter in EAX and flags in CL; emitted INC/DEC shift branches.
unsigned char __stdcall TableLookupByFlags_guess(unsigned char *param)
{
    unsigned char *cursor;
    unsigned char flags;

    /* fake match: stepping the cursor over the flag byte and back makes VC6 keep the base in EAX and flags in CL. */
    cursor = param;
    flags = *++cursor;
    --cursor;
    if ((flags & 0x31) == 0)
    {
        if ((flags & 0xc) == 0)
        {
            if ((flags & 2) != 0)
            {
                return EQ_ARRAY(unsigned char, 0x1012C7C0)[*cursor];
            }
            return EQ_ARRAY(unsigned char, 0x10048818)[*cursor];
        }
        return EQ_ARRAY(unsigned char, 0x10048918)[*cursor];
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10028330
// Clears a three-dword record (12 callers across the front end). __cdecl; the record's layout is unknown.
void __cdecl ClearRecord3_guess(unsigned int* record)
{
    record[0] = 0;
    record[1] = 0;
    record[2] = 0;
}
