// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// utf8.cpp - functions the queue files under subsystem "EverQuest/utf8".
// ORIGINAL FILE: unknown. The queue label is a call-graph guess; see include/trilogy/utf8.h.

#include "trilogy/utf8.h"
#include "trilogy/suite_hash_table.h"
// The base/gauge shells are preserved in eq_header_title_base.h.
/* fake match: VC6 treats a call through a pointer variable as possibly throwing and a constant-address call as
   not, so only this form keeps the EH frame and the state store the original has around this member. */
#include "trilogy/layout/eq_header_title.h"
/* fake match: EQSpellGemGauge_guess keeps its constructor call through a pointer local in eq_header_title_base.h;
   VC6 otherwise removes the member EH state stores. */
#include "trilogy/layout/utf8_text_object.h"
#include "trilogy/eqgfx.h"
#include <string.h>
#include <stdarg.h>

// Local stand-in: utf8.h's EQObjectList_guess has no vtable. The real object has a vtable at +0 and
// the list at +0x0C/+0x10; slot 4 (+0x10) returns a char.
class EQVirtualList_guess
{
public:
    virtual void Slot0_guess();
    virtual void Slot1_guess();
    virtual void Slot2_guess();
    virtual void Slot3_guess();
    virtual char Slot4_guess();
};

struct FUN_00404e17Object_guess
{
    unsigned char reserved00[0x2b];
    unsigned char resetByte;
    unsigned char reserved2c[0x54];
    unsigned char enabledByte;
    unsigned char reserved81[0x6b];
    unsigned char stringOwner;
    unsigned char reservedED[0x0f];
    unsigned char stringBuffer;
};

struct FUN_00405502Request_guess
{
    int type;
    float x;
    float y;
};

struct FUN_00405502Object_guess
{
    unsigned char reserved00[0x29];
    unsigned char hoverState;
    unsigned char reserved2a[0x8e];
    float minimumX;
    float minimumY;
    unsigned char reservedC0[0x10];
    float maximumX;
    float maximumY;
    unsigned char reservedD8[0x4c];
    unsigned char selectionState;
};

// FUNCTION: TRILOGY 0x004016EE
// Initializes an object; its specific UI role is unclear.
u32* __fastcall InitializeObjectWithVtable543F0_guess(u32* instance)
{
    EQ_METHOD(0, void, 0x00403CDF, (u32*), instance)();
    *instance = 0x005343F0;
    return instance;
}

// FUNCTION: TRILOGY 0x00401727
// ORIGINAL FILE: unknown
// ECX = object (this); no stack args.
void __fastcall CXWnd_UpdateWindowWithTooltip_guess(EQVirtualList_guess* obj)
{
    if (obj->Slot4_guess() != 0)
        ((EQObjectList_guess*)obj)->CallSlot2OnAll_guess();
}

// Local stand-in: utf8.h's EQVector3_guess declares no addition member (headers are frozen); same layout.
struct EQVector3Sum_guess
{
    f32     x;                  // 0x0
    f32     y;                  // 0x4
    f32     z;                  // 0x8

    EQVector3Sum_guess Plus_guess(const EQVector3Sum_guess& rhs) const;
};

// File-local view of the object whose vector is read when the owner has a transform pointer.
struct EQVectorTransformSource_guess
{
    u8                      unknown0x00[0x4C];
    EQVector3Sum_guess      vector;          // 0x4C
};

// File-local view of the object written by UpdateVectorAndNotifyObjects_guess.  The public headers do not
// yet contain this object's layout; the offsets come from the original instructions.
struct EQVectorListTransform_guess
{
    u8                      unknown0x00[8];
    EQVectorTransformSource_guess* transform; // 0x08
    u32**                   begin;          // 0x0C
    u32**                   end;            // 0x10
    u8                      unknown0x14[0x20];
    u32                     source[3];     // 0x34
    f32                     origin_x;       // 0x40
    f32                     origin_y;       // 0x44
    u32                     unknown0x48;    // 0x48
    u32                     vector[3];      // 0x4C (copied as three raw dwords)
    f32                     result_x;       // 0x58
    u32                     result_y;       // 0x5C
    u32                     result_z;       // 0x60
    f32                     result_x_copy;  // 0x64
    f32                     result_y_copy;  // 0x68
    u32                     result_z_copy;  // 0x6C
    u32                     vector_x_copy;  // 0x70
    f32                     result_y_copy2; // 0x74
    u32                     result_z_copy2; // 0x78
};

// Slot 17 is the callback at vtable displacement +0x44.
class EQSlot17Object_guess
{
public:
    virtual void Slot0_guess();
    virtual void Slot1_guess();
    virtual void Slot2_guess();
    virtual void Slot3_guess();
    virtual void Slot4_guess();
    virtual void Slot5_guess();
    virtual void Slot6_guess();
    virtual void Slot7_guess();
    virtual void Slot8_guess();
    virtual void Slot9_guess();
    virtual void Slot10_guess();
    virtual void Slot11_guess();
    virtual void Slot12_guess();
    virtual void Slot13_guess();
    virtual void Slot14_guess();
    virtual void Slot15_guess();
    virtual void Slot16_guess();
    virtual void Slot17_guess();
};

// These adjacent locals occupy the original 0x3C-byte stack area: a 12-float
// transform block followed by the helper's hidden 12-byte result buffer.
struct EQVectorStackLocals_guess
{
    u8                  unknown0x00[8];
    f32                 matrix[12];
    EQVector3Sum_guess  result;
};

// FUNCTION: TRILOGY 0x00401762
// Updates a transformed point and refreshes objects registered to it.
void __fastcall UpdateVectorAndNotifyObjects_guess(EQVectorListTransform_guess* self, int, u32* sourceVector)
{
    f32 originAdjusted;
    {
        u32* ptr;
        u32 zComponent;
        struct
        {
            f32 matrix[12];
            EQVector3Sum_guess result;
        } local;
    
        self->source[0] = sourceVector[0];
        self->source[1] = sourceVector[1];
        self->source[2] = sourceVector[2];
        EQ_FUNC(void, __fastcall, 0x0040186A, (f32*))(local.matrix);
        local.matrix[0] = 0.0f;
        local.matrix[1] = 0.0f;
        local.matrix[3] = 0.0f;
        local.matrix[4] = 0.0f;
        local.matrix[6] = 0.0f;
        local.matrix[7] = 0.0f;
        local.matrix[9] = 0.0f;
        local.matrix[10] = 0.0f;
        if (self->transform != 0)
        {
            ptr = (u32*)EQ_METHOD(2, EQVector3Sum_guess*, 0x00401835, (EQVector3Sum_guess*, EQVector3Sum_guess*, const EQVector3Sum_guess*), &self->transform->vector)(&local.result,
                                       (const EQVector3Sum_guess*)sourceVector);
            self->vector[0] = ptr[0];
            self->vector[1] = ptr[1];
            zComponent = ptr[2];
        }
        else
        {
            self->vector[0] = sourceVector[0];
            self->vector[1] = sourceVector[1];
            zComponent = sourceVector[2];
        }
        self->vector[2] = zComponent;
        float & temp = self->origin_x;
        originAdjusted = temp + *(f32*)&self->vector[0];
        self->result_y = self->vector[1];
        self->result_x = originAdjusted;
        zComponent = sourceVector[2];
        self->result_x_copy = originAdjusted;
        originAdjusted = self->origin_y + *(f32*)&self->vector[1];
        self->result_z = zComponent;
        self->result_y_copy = originAdjusted;
        zComponent = sourceVector[2];
        self->result_y_copy2 = originAdjusted;
        self->result_z_copy = zComponent;
        self->vector_x_copy = self->vector[0];
        self->result_z_copy2 = sourceVector[2];
        ptr = (u32*)self->begin;
        for (; ptr != (u32*)self->end; ptr = 1 + ptr)
            ((EQSlot17Object_guess*)*ptr)->Slot17_guess();
    }
}

// FUNCTION: TRILOGY 0x00401835
// ORIGINAL FILE: unknown
// rhs + this; the result is built in a local and copied out (no return-value optimisation in VC6).
EQVector3Sum_guess EQVector3Sum_guess::Plus_guess(const EQVector3Sum_guess& rhs) const
{
    EQVector3Sum_guess r;

    r.x = rhs.x + x;
    r.y = rhs.y + y;
    r.z = rhs.z + z;
    return r;
}

// FUNCTION: TRILOGY 0x0040186A
// The purpose of this 12-float data block is unclear.
// ORIGINAL FILE: unknown
// ECX = object (this); no stack args; returns this. Twelve floats as four (0, 0, 1) triples: the triples
// at +0xC..+0x2C are set first (inlined member constructors?), then all four again.
/* fake match: the first pass is overwritten by the second; VC6 /O1 would drop it, so it writes through a volatile view that keeps the original's duplicate stores. */
f32* __fastcall InitBlock48_guess(f32* p)
{
    volatile f32* first_pass = p;
    first_pass[3] = 0.0f;
    first_pass[4] = 0.0f;
    first_pass[5] = 1.0f;
    first_pass[6] = 0.0f;
    first_pass[7] = 0.0f;
    first_pass[8] = 1.0f;
    first_pass[9] = 0.0f;
    first_pass[10] = 0.0f;
    first_pass[11] = 1.0f;
    p[0] = 0.0f;
    p[1] = 0.0f;
    p[2] = 1.0f;
    p[3] = 0.0f;
    p[4] = 0.0f;
    p[5] = 1.0f;
    p[6] = 0.0f;
    p[7] = 0.0f;
    p[8] = 1.0f;
    p[9] = 0.0f;
    p[10] = 0.0f;
    p[11] = 1.0f;
    return p;
}

// Local stand-in: a float[3] temporary built by a constructor (the original passes one by address).
struct EQVector3Temp_guess
{
    f32 x;
    f32 y;
    f32 z;

    EQVector3Temp_guess(f32 a, f32 b, f32 c)
    {
        x = a;
        y = b;
        z = c;
    }
};

// Local stand-in: the object's vtable slot 16 (+0x40) is a thiscall taking the temporary by reference.
class EQVirtualSlot16_guess
{
public:
    virtual void Slot0_guess();
    virtual void Slot1_guess();
    virtual void Slot2_guess();
    virtual void Slot3_guess();
    virtual void Slot4_guess();
    virtual void Slot5_guess();
    virtual void Slot6_guess();
    virtual void Slot7_guess();
    virtual void Slot8_guess();
    virtual void Slot9_guess();
    virtual void Slot10_guess();
    virtual void Slot11_guess();
    virtual void Slot12_guess();
    virtual void Slot13_guess();
    virtual void Slot14_guess();
    virtual void Slot15_guess();
    virtual void Slot16_guess(const EQVector3Temp_guess& v);
};

// FUNCTION: TRILOGY 0x004018D5
// Passes a 2D point to an object's handler; its UI effect is unclear.
// ORIGINAL FILE: unknown
// ECX = object (this); one stack arg (f32* src); callee pops 4; returns nothing. Passes the object's virtual
// slot 16 (+0x40) a temporary built from src[0], src[1] and 1.0f.
void __fastcall ForwardVector2ToVirtualSlot16_guess(void* self, int edx_unused, f32* src)
{
    ((EQVirtualSlot16_guess*)self)->Slot16_guess(EQVector3Temp_guess(src[0], src[1], 1.0f));
}

// FUNCTION: TRILOGY 0x004018fb
// Forwards the object member at +0x34 by reference to virtual slot 16.
void __fastcall ForwardStoredVectorToVirtualSlot16_guess(EQVirtualSlot16_guess* self)
{
    self->Slot16_guess(*(const EQVector3Temp_guess*)((const char*)self + 0x34));
}

// Local stand-in: layout guessed from InitializeBlockFields_guess alone (size >= 0x54, +4 untouched).
struct EQClearedBlock_guess
{
    u32     f0;                 // 0x00
    u32     unknown0x04;        // 0x04
    u8      table[0x40];        // 0x08
    u32     f48;                // 0x48
    u32     f4c;                // 0x4C
    u32     f50;                // 0x50

    // The constructor is InitializeBlockFields_guess below (it zeroes the block and returns it).
    // EXTERNAL: TRILOGY 0x00401BE6 EQClearedBlock_guess::EQClearedBlock_guess
    EQClearedBlock_guess();
};

// FUNCTION: TRILOGY 0x00401be6
// ORIGINAL FILE: unknown
// ECX = object (this); no stack args. Zeroes +0, +8..+0x47 and +0x48..+0x53; returns this.
EQClearedBlock_guess* __fastcall InitializeBlockFields_guess(EQClearedBlock_guess* obj)
{
    obj->f0 = 0;
    memset(obj->table, 0, 0x40);
    obj->f48 = 0;
    obj->f4c = 0;
    obj->f50 = 0;
    return obj;
}

// Stand-in for the base at +0x00 of the object built by EQStaticTextElement_guess::EQStaticTextElement_guess (the int at +0x9C comes after it). Its constructor
// is 0x00403CDF and its destructor is 0x00403E27, the pair EQSpellGemBase_guess uses. Being destructible, it gets the
// EH state store after its construction; the derived constructor's EH frame comes from the pointer call below.
class EQStaticHeaderBase_guess
{
public:
    EQStaticHeaderBase_guess()
    {
        EQ_METHOD(0, void, 0x00403CDF, (void*), this)();
    }
    ~EQStaticHeaderBase_guess()
    {
        EQ_METHOD(0, void, 0x00403E27, (void*), this)();
    }
};

// The object built here is the StaticTextElement_guess below (vtable 0x534444): an int at +0x9C and a sub-object at
// +0xE0 follow the base. This class only carries the constructor.
class EQStaticTextElement_guess : public EQStaticHeaderBase_guess
{
public:
    EQStaticTextElement_guess();
};

// FUNCTION: TRILOGY 0x00401C2E
// Sets up a UI text element with a zeroed text rectangle.
// ECX = object (this); no stack args. A constructor: the base (0x403CDF) runs first, then the sub-object at +0xE0 is
// built (0x401D9A), the vtable is stored, the sub-object gets its text (0x401DE2) and the int at +0x9C is cleared.
// Returns this.
EQStaticTextElement_guess::EQStaticTextElement_guess()
{
    /* fake match: VC6 treats a call through a pointer variable as possibly throwing and a constant-address call as
       not, so only this form keeps the EH frame and the state store the original has around the base. */
    void (__fastcall *subCtor)(void*) = EQ_FUNC(void, __fastcall, 0x00401D9A, (void*));

    subCtor((char*)this + 0xe0);
    *(void**)this = (void*)0x00534444;
    EQ_METHOD(1, void, 0x00401DE2, (void*, const char*), (char*)this + 0xe0)(EQ_ARRAY(char, 0x0053C2A0));
    *(int*)((char*)this + 0x9c) = 0;
}

class StaticTextElement_guess
{
public:
    StaticTextElement_guess* ScalarDeletingDtor_guess(unsigned char flags);
    ~StaticTextElement_guess();
};

// FUNCTION: TRILOGY 0x00401c7f
// Tears down the text element and frees its storage when requested.
StaticTextElement_guess* StaticTextElement_guess::ScalarDeletingDtor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x00401C9B, (void*), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void*))(this);
    return this;
}

// Local stand-ins for the missing class hierarchy (utf8.h has no layout for this object; the base
// cleanup is the Suite StaticHeaderElement destructor at 0x00403e27).
#ifdef EQD_MATCH
extern "C" void __cdecl eqd_fn_0x00403E27();   /* not in the generated match header */
#endif

// Local stand-in for the base cleanup: its constructor writes the class vtable and its destructor
// runs the base subobject destructor with the stored object.
struct StaticTextBaseCleanup_guess
{
    void* obj_guess;

    StaticTextBaseCleanup_guess(void* obj) : obj_guess(obj)
    {
        *(void**)obj = (void*)0x00534444;
    }
    ~StaticTextBaseCleanup_guess()
    {
        EQ_METHOD(0, void, 0x00403E27, (void*), obj_guess)();
    }
};

/* fake match: the base cleanup is a local so VC6 frames the original's state 0/-1 and stores the vtable before it. */
// FUNCTION: TRILOGY 0x00401c9b
// Clears the text element's rectangle and state during teardown.
StaticTextElement_guess::~StaticTextElement_guess()
{
    StaticTextBaseCleanup_guess headerCleanup(this);

    EQ_METHOD(1, void, 0x00401DE2, (void*, const char*), (char*)this + 0xe0)("0,0,0,0");
    *(int*)((char*)this + 0x9c) = 0;
}

// FUNCTION: TRILOGY 0x00401ce5
// If slot 4 is nonzero: creates the +0x9C overlay once, else submits its quad; then calls slot 2 on each child.
void __fastcall CreateOverlayOrSubmitQuad_guess(EQVirtualList_guess* obj)
{
    if (obj->Slot4_guess() != 0)
    {
        if (*(int*)((char*)obj + 0x9c) == 0)
        {
            int resourceHandle = EQ_METHOD(2, int, 0x00402EC0, (void*, void*, void*), *(void**)((char*)obj + 4))((char*)obj + 0xa0, (char*)obj + 0xe0);
            *(int*)((char*)obj + 0x9c) = resourceHandle;
        }
        else
        {
            EQ_METHOD(3, void, 0x00402C98, (void*, void*, void*, int), *(void**)((char*)obj + 4))((char*)obj + 0x4c, (char*)obj + 0x7c,
                *(int*)((char*)obj + 0x9c));
        }
        EQ_METHOD(0, void, 0x00403F3D, (void*), (EQObjectList_guess*)obj)();
    }
}

// FUNCTION: TRILOGY 0x00401D3C
// Applies a graphic's source image or source rectangle setting.
// ORIGINAL FILE: unknown
// __thiscall (ECX = this; name and value are stack args, the callee pops 8). Dispatches on the attribute
// name, case-insensitively: "source" copies the value to this+0xA0, "sourceRect" passes it to ParseCommaListToRectCorners_guess
// on this+0xE0, anything else goes to EQAttrMapOwner_guess::SetAttributeString_guess. Void: no direct callers, EAX evidence inconclusive.
void __fastcall SetAttributeByName_guess(char* self, int edx_unused, char* name, char* value)
{
    int result;

    result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, EQ_ARRAY(char, 0x0053C2B4));
    if (result == 0)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(self + 0xa0, value);
    }
    else
    {
        result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, EQ_ARRAY(char, 0x0053C2A8));
        if (result == 0)
        {
            EQ_METHOD(1, void, 0x00401DE2, (void*, char*), self + 0xe0)(value);
        }
        else
        {
            EQ_METHOD(2, void, 0x00403F58, (void*, char*, char*), self)(name, value);
        }
    }
}

// Local stand-in: eight floats, layout guessed from ZeroFloat8_guess alone.
struct EQFloat8_guess
{
    f32     v[8];               // 0x00
};

// FUNCTION: TRILOGY 0x00401D9A
// Clears an eight-value data block; its specific role is unclear.
// ORIGINAL FILE: unknown
// ECX = object (this); no stack args. Zeroes +8..+0x1C, then all eight floats again (likely two
// inlined constructors/clears); returns this. The first zeroing pass is volatile (see fake match).
/* fake match: VC6 drops the first six zero stores as dead (the second pass overwrites them); a volatile view keeps them. */
EQFloat8_guess* __fastcall ZeroFloat8_guess(EQFloat8_guess* obj)
{
    volatile float* first_pass_guess = obj->v;
    first_pass_guess[2] = 0.0f;
    first_pass_guess[3] = 0.0f;
    first_pass_guess[4] = 0.0f;
    first_pass_guess[5] = 0.0f;
    first_pass_guess[6] = 0.0f;
    first_pass_guess[7] = 0.0f;
    obj->v[0] = 0.0f;
    obj->v[1] = 0.0f;
    obj->v[2] = 0.0f;
    obj->v[3] = 0.0f;
    obj->v[4] = 0.0f;
    obj->v[5] = 0.0f;
    obj->v[6] = 0.0f;
    obj->v[7] = 0.0f;
    return obj;
}

// FUNCTION: TRILOGY 0x00401DE2
// ECX = float output; the string argument is on the stack.
void __fastcall ParseCommaListToRectCorners_guess(float* outputCorners, int edx_dummy_guess, char* sourceText)
{
    int tokenLength;
    int valueCount;
    float* valueCursor;
    char token[16];
    float parsedValues[4];

    parsedValues[0] = 0.0f;
    parsedValues[1] = 0.0f;
    tokenLength = 0;
    valueCount = 0;
    parsedValues[2] = 0.0f;
    parsedValues[3] = 0.0f;
    if (*sourceText != '\0')
    {
        valueCursor = parsedValues;
        do
        {
            if (valueCount == 4)
                break;
            if (*sourceText == ',')
            {
                token[tokenLength] = '\0';
                *valueCursor = (float)EQ_CDECL(double, 0x00521AB0, (const char*))(token);
                tokenLength = 0;
                valueCount = valueCount + 1;
                valueCursor = valueCursor + 1;
            }
            else
            {
                token[tokenLength] = *sourceText;
                tokenLength = tokenLength + 1;
            }
            sourceText = sourceText + 1;
        } while (*sourceText != '\0');
        if (3 < valueCount)
            goto writeCorners;
    }
    token[tokenLength] = '\0';
    parsedValues[valueCount] = (float)EQ_CDECL(double, 0x00521AB0, (const char*))(token);
writeCorners:
    outputCorners[0] = parsedValues[0];
    outputCorners[1] = parsedValues[1];
    outputCorners[2] = parsedValues[2];
    outputCorners[3] = parsedValues[1];
    outputCorners[4] = parsedValues[2];
    outputCorners[5] = parsedValues[3];
    outputCorners[6] = parsedValues[0];
    outputCorners[7] = parsedValues[3];
}

// Local stand-in: layout guessed from InitializeFloat2Block_guess alone (+0..+0x43 untouched, size >= 0x94).
struct EQFloat2_guess
{
    f32     x;                  // 0x0
    f32     y;                  // 0x4
};

struct EQFloat2Block_guess
{
    u8              unknown0x00[0x44];  // 0x00
    EQFloat8_guess  inner;              // 0x44
    EQFloat2_guess  pairs[4];           // 0x64
    EQFloat2_guess  a;                  // 0x84
    EQFloat2_guess  b;                  // 0x8C
    EQFloat2Block_guess* InitializeFloat2Block_guess(void);    /* 0x00401E99 */
};

// FUNCTION: TRILOGY 0x00401E99
// Initializes a multi-vector UI block; its specific purpose is unclear.
// ORIGINAL FILE: unknown
// ECX = object (this); no stack args. Constructs +0x44 via ZeroFloat8_guess, then zeroes the six float
// pairs at +0x64..+0x93 (likely inlined vec2 constructors); returns this.
EQFloat2Block_guess* EQFloat2Block_guess::InitializeFloat2Block_guess(void)
{
    int i;
    EQFloat2_guess* pair;

    EQ_METHOD(0, EQFloat8_guess*, 0x00401D9A, (EQFloat8_guess*), &this->inner)();
    i = 4;
    pair = &this->pairs[0];
    do
    {
        pair->x = 0.0f;
        pair->y = 0.0f;
        pair++;
        i--;
    } while (i != 0);
    pair = &this->a;
    pair->x = 0.0f;
    pair->y = 0.0f;
    pair = &this->b;
    pair->x = 0.0f;
    pair->y = 0.0f;
    return this;
}

// The factory result's second vtable entry is called before it is returned.
class EQFactoryResult_guess
{
public:
    virtual void Slot0_guess();
    virtual void Slot1_guess();
};

// The UI elements CreateUIElement_guess creates: the client's constructors, only declared here.
struct Graphic_guess
{
    char storage_guess[0x100];
    // EXTERNAL: TRILOGY 0x00401C2E Graphic_guess::Graphic_guess
    Graphic_guess();
};
struct Textbox_guess
{
    char storage_guess[0x110];
    // EXTERNAL: TRILOGY 0x00404AD7 Textbox_guess::Textbox_guess
    Textbox_guess();
};
struct Button_guess
{
    char storage_guess[0x9C];
    // EXTERNAL: TRILOGY 0x004016EE Button_guess::Button_guess
    Button_guess();
};
struct Window_guess
{
    char storage_guess[0x128];
    // EXTERNAL: TRILOGY 0x00404FEF Window_guess::Window_guess
    Window_guess();
};
struct Page_guess
{
    char storage_guess[0x9C];
    // EXTERNAL: TRILOGY 0x00404930 Page_guess::Page_guess
    Page_guess();
};

// FUNCTION: TRILOGY 0x0040257F
// Creates the requested graphic, text box, button, window, or page UI element.
int* __stdcall CreateUIElement_guess(const char* name_guess)
{
    union FactoryResult_guess
    {
        int* result_guess;
        Graphic_guess* graphic_guess;
        Textbox_guess* textbox_guess;
        Button_guess* button_guess;
        Window_guess* window_guess;
        Page_guess* page_guess;
    } factory_result_guess;

    factory_result_guess.result_guess = 0;
    if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
            name_guess, EQ_ARRAY(char, 0x0053C2E8), 5) == 0)
        factory_result_guess.graphic_guess = new Graphic_guess;
    else if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 name_guess, EQ_ARRAY(char, 0x0053C2E0), 7) == 0)
        factory_result_guess.textbox_guess = new Textbox_guess;
    else if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 name_guess, EQ_ARRAY(char, 0x0053C2D8), 6) == 0)
        factory_result_guess.button_guess = new Button_guess;
    else if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 name_guess, EQ_ARRAY(char, 0x0053C2D0), 6) == 0)
        factory_result_guess.window_guess = new Window_guess;
    else if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 name_guess, EQ_ARRAY(char, 0x0053C2C8), 4) == 0)
        factory_result_guess.page_guess = new Page_guess;

    if (factory_result_guess.result_guess != 0)
        ((EQFactoryResult_guess*)factory_result_guess.result_guess)->Slot1_guess();
    return factory_result_guess.result_guess;
}

// FUNCTION: TRILOGY 0x00402c83
// Queues a rectangle for rendering.
// ORIGINAL FILE: unknown
void EQUnknownGfxOwner_guess::DeferRect_guess(void* render_guess, u32* pRect_guess)
{
    t3dDeferRect(render_guess, *pRect_guess);
}

typedef int (__cdecl *t3dDeferQuad_guess_t)(void* render_guess, u32 quad_guess);
typedef int (__cdecl *t3dDeferQuadTex_guess_t)(u32 texture_guess, void* quad_guess, void* coordinates_guess,
                                               u32 render_guess);
#define t3dDeferQuad_guess    EQ_GLOBAL(t3dDeferQuad_guess_t, 0x006E51C0)
#define t3dDeferQuadTex_guess EQ_GLOBAL(t3dDeferQuadTex_guess_t, 0x006E51C4)

// FUNCTION: TRILOGY 0x00402c98
// Submits a UI quad for rendering, with a texture when one is available.
void __fastcall DeferQuadOptionalTexture_guess(void* owner_guess, int edx_guess, EQBlock48_guess* block_guess,
                             u32* pRender_guess, u32* pTexture_guess)
{
    if (pTexture_guess != 0)
    {
        // Set_guess ignores EDX; pass the entry EDX, which the original never overwrites, not a literal 0.
        EQ_THISCALL(void, 0x00403738, (EQBlock48_guess*, int, const EQBlock48_guess*))(
            (EQBlock48_guess*)((u8*)owner_guess + 0x1844), edx_guess, block_guess);
        *(float*)((u8*)owner_guess + 0x184c) = block_guess->v[2] * 3.030303e-05f + 0.001f;
        *(float*)((u8*)owner_guess + 0x1858) = block_guess->v[2] * 3.030303e-05f + 0.001f;
        *(float*)((u8*)owner_guess + 0x1864) = block_guess->v[2] * 3.030303e-05f + 0.001f;
        *(float*)((u8*)owner_guess + 0x1870) = block_guess->v[2] * 3.030303e-05f + 0.001f;
        t3dDeferQuadTex_guess(*pTexture_guess, (u8*)owner_guess + 0x1844, (u8*)pTexture_guess + 0x64,
                              *pRender_guess);
    }
    else
    {
        t3dDeferQuad_guess(block_guess, *pRender_guess);
    }
    return;
}

typedef void* (__cdecl *t3dDefineBMInfo_guess_t)(void*, const char*, const char*, int);
typedef void* (__cdecl *t3dOverlayOpen_guess_t)(void*, void*, int);
typedef void* (__cdecl *t3dOverlayCreate_guess_t)(int, int, int);
#define t3dDefineBMInfo_guess EQ_GLOBAL(t3dDefineBMInfo_guess_t, 0x006E51FC)
#define t3dOverlayOpen_guess  EQ_GLOBAL(t3dOverlayOpen_guess_t, 0x006E534C)
#define t3dOverlayCreate_guess EQ_GLOBAL(t3dOverlayCreate_guess_t, 0x006E5340)

class EQOverlayObject_guess
{
public:
    static void* __cdecl operator new(unsigned int size_guess)
    {
        return EQ_CDECL(void*, 0x005218C6, (u32))(size_guess);
    }

    EQOverlayObject_guess()
    {
        EQ_METHOD(0, void, 0x00401E99, (void*), this)();
    }

    u8 storage_guess[0x94];
};
C_ASSERT(sizeof(EQOverlayObject_guess) == 0x94);

struct OverlayAllocation_guess
{
    u8 storage_guess[0x94];
    // EXTERNAL: TRILOGY 0x00401E99 OverlayAllocation_guess::OverlayAllocation_guess
    OverlayAllocation_guess();
};

// Owner of the member below; layout not recovered.
class UiObjectFactory_guess
{
public:
    void* CreateAndConfigureObject_guess(const char* name_guess, void* parameter_guess);    /* 0x00402EC0 */
};

// FUNCTION: TRILOGY 0x00402d2d
// Creates a drawable overlay for the requested dimensions.
// @early-stop: x87 cleanup and bounds argument scheduling still differ.
/* fake match: float return keeps frexp's ST0 for the original m32 comparison without a narrowing store. */
int* __fastcall CreateSizedOverlayBlock_guess(void* owner_guess, int edx_guess, float* size_guess)
{
    struct ScaledBlockCall_guess
    {
        void SetScaled_guess(float* bounds_guess) {}
    };
    typedef void (ScaledBlockCall_guess::*SetScaled_guess_t)(float*);
    union SetScaled_guess_address
    {
        void (__cdecl *function)(float*);
        SetScaled_guess_t method;
    } set_scaled_guess;
    int width_guess;
    int height_guess;
    int exponent_guess;
    float fraction_guess;
    int* overlay_guess;
    int* result_guess;
    float bounds_guess[8];
    float bottom_guess;
    float right_guess;

    result_guess = 0;
    width_guess = (int)size_guess[0];
    height_guess = (int)size_guess[1];

    exponent_guess = 0;
    fraction_guess = EQ_CDECL(float, 0x0052244B, (double, int*))((double)width_guess, &exponent_guess);
    if (fraction_guess == EQ_GLOBAL(f32, 0x005344A8))
        width_guess = 1 << (exponent_guess - 1);
    else
        width_guess = 1 << exponent_guess;

    fraction_guess = EQ_CDECL(float, 0x0052244B, (double, int*))((double)height_guess, &exponent_guess);
    if (fraction_guess == EQ_GLOBAL(f32, 0x005344A8))
        height_guess = 1 << (exponent_guess - 1);
    else
        height_guess = 1 << exponent_guess;

    overlay_guess = (int*)t3dOverlayCreate_guess((int)size_guess[0], (int)size_guess[1], 0);
    if (overlay_guess != 0)
    {
        result_guess = (int*)new OverlayAllocation_guess;
        *(void**)result_guess = overlay_guess;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)result_guess + 4, (const char*)0x0058FBA4);
        *(float*)((u8*)result_guess + 0x8c) = (float)width_guess;
        *(float*)((u8*)result_guess + 0x90) = (float)height_guess;
        EQ_FASTCALL(void, 0x004038E9, (void*, float, float))(
            result_guess, (float)*(int*)overlay_guess, (float)*((int*)overlay_guess + 1));
        EQ_FUNC(EQFloat8_guess*, __fastcall, 0x00401D9A, (EQFloat8_guess*))(
            (EQFloat8_guess*)bounds_guess);
        bottom_guess = (float)*((int*)overlay_guess + 1)
            - *(volatile f32*)&EQ_GLOBAL(f32, 0x005344A4);
        set_scaled_guess.function = EQ_FUNC(void, __cdecl, 0x0040383B, (float*));
        right_guess = (float)*(int*)overlay_guess - *(volatile f32*)&EQ_GLOBAL(f32, 0x005344A4);
        bounds_guess[0] = 0.0f;
        bounds_guess[1] = 0.0f;
        bounds_guess[2] = right_guess;
        bounds_guess[3] = 0.0f;
        bounds_guess[4] = right_guess;
        bounds_guess[5] = bottom_guess;
        bounds_guess[6] = 0.0f;
        bounds_guess[7] = bottom_guess;
        (((ScaledBlockCall_guess*)result_guess)->*set_scaled_guess.method)(bounds_guess);
    }
    return result_guess;
}

struct OverlayObject_guess
{
    void* overlay;
    char name[0x88];
    float width;
    float height;

    // EXTERNAL: TRILOGY 0x00401E99 OverlayObject_guess::OverlayObject_guess
    OverlayObject_guess();

};

// FUNCTION: TRILOGY 0x00402EC0
// Opens a named bitmap as a UI overlay and applies its draw settings.
// @early-stop: x87 stack handling, local slots, and register scheduling still differ.
void* UiObjectFactory_guess::CreateAndConfigureObject_guess(const char* name_guess, void* parameter_guess)
{
    void* owner_guess = (void*)this;
    int size_guess;
    void* overlay_guess;
    int exponent_guess;
    double fraction_guess;
    int one_guess;
    int width_guess;
    int height_guess;
    OverlayObject_guess* object_guess;
    void* result_guess;

    one_guess = 1;
    overlay_guess = 0;
    size_guess = *(int*)((u8*)owner_guess + 4);
    result_guess = 0;
    if (size_guess != 0)
    {
        overlay_guess = t3dDefineBMInfo_guess(*(void**)owner_guess, name_guess, name_guess, size_guess);
        overlay_guess = t3dOverlayOpen_guess(*(void**)owner_guess, *(void**)overlay_guess, one_guess);
    }
    width_guess = *(int*)overlay_guess;
    height_guess = *((int*)overlay_guess + 1);
    exponent_guess = 0;
    fraction_guess = EQ_CDECL(double, 0x0052244B, (double, int*))(
        (double)width_guess, &exponent_guess);
    width_guess = one_guess << (fraction_guess == EQ_GLOBAL(double, 0x005344B0)
        ? exponent_guess - 1 : exponent_guess);

    fraction_guess = EQ_CDECL(double, 0x0052244B, (double, int*))(
        (double)height_guess, &exponent_guess);
    height_guess = one_guess << (fraction_guess == EQ_GLOBAL(double, 0x005344B0)
        ? exponent_guess - 1 : exponent_guess);

    object_guess = 0;
    if (overlay_guess != 0)
    {
        object_guess = new OverlayObject_guess;
        result_guess = object_guess;
        object_guess->overlay = overlay_guess;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            object_guess->name, name_guess);
        object_guess->width = (float)width_guess;
        object_guess->height = (float)height_guess;
        EQ_FASTCALL(void, 0x004038E9, (void*, float, float))(
            object_guess, (float)*(int*)overlay_guess, (float)*((int*)overlay_guess + 1));
        EQ_METHOD(1, void, 0x0040383B, (void*, void*), object_guess)(parameter_guess);
    }
    return result_guess;
}

typedef int (__cdecl *t3dCreateFont_guess_t)(const char* name, int first_value, int second_value, int flags);
#define t3dCreateFont_guess    EQ_GLOBAL(t3dCreateFont_guess_t, 0x006E51D8)

// FUNCTION: TRILOGY 0x00402FF4
// Shares a resource record keyed by a name and two settings.
// @early-stop: found path keeps node in EAX (VC6 CSEs node+8 into ESI); EQ_THISCALL xor edx on 3 calls; xor esi/add esp and test/cmp order; state store order in the new block; no raw record store; marker dtor is an out-of-line call
/* fake match: the constructor is called through a pointer variable, which VC6 treats as possibly throwing, so the marker local gets the EH frame. */
void* __fastcall GetOrCreateRefCountedRecord_guess(void* owner_guess, int edx_guess, const char* name_guess,
                              int first_value_guess, int second_value_guess)
{
    int key_size_guess;
    char* key_guess;
    char* key_copy_guess;
    int font_handle_guess;
    EQClearedBlock_guess* record_guess = 0;
    void* map_entry_guess;

    key_size_guess = EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
        (char*)owner_guess + 0x44, (const char*)0x0053C320, name_guess, first_value_guess, second_value_guess);
    key_guess = (char*)EQ_CDECL(void*, 0x005218C6, (unsigned int))(key_size_guess + 1);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
        key_guess, (char*)owner_guess + 0x44);
    {
        int lookup_guess[2];

        EQ_THISCALL(void, 0x00404387, (void*, int, int*, char**))(
            (char*)owner_guess + 0x28, 0, lookup_guess, &key_guess);
        void** node_guess = (void**)lookup_guess[0];
        if (node_guess != 0)
        {
            /* node_guess[2] is the node's record pointer at +0x08 */
            ++*(int*)node_guess[2];
            EQ_CDECL(void, 0x005218BB, (void*))(key_guess);
            return node_guess[2];
        }
    }
    font_handle_guess = t3dCreateFont_guess(name_guess, first_value_guess, second_value_guess, 1);
    if (font_handle_guess != 0)
    {
        record_guess = new EQClearedBlock_guess;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            (char*)record_guess + 8, name_guess);
        ++record_guess->f0;
        record_guess->f48 = first_value_guess;
        record_guess->f4c = second_value_guess;
        record_guess->f50 = font_handle_guess;
        key_copy_guess = key_guess;
        map_entry_guess = EQ_THISCALL(void*, 0x004033E1, (void*, int, char**))(
            (char*)owner_guess + 0x28, 0, &key_copy_guess);
        *(void**)((u8*)map_entry_guess + 4) = record_guess;
    }
    return record_guess;
}

// eqgfx.h has no slot for t3dDeferTextOnOverlayW (GetProcAddress slot 0x006E51E8, loader 0x004DDE87).
typedef int (__cdecl *t3dDeferTextOnOverlayW_guess_t)(wchar_t* text, u32 a_guess, u32 b_guess, u32 c_guess,
                                                      u32 d_guess, u32 e_guess);
#define t3dDeferTextOnOverlayW_guess    EQ_GLOBAL(t3dDeferTextOnOverlayW_guess_t, 0x006E51E8)

// FUNCTION: TRILOGY 0x00403129
// Displays formatted wide text on an overlay.
// ORIGINAL FILE: unknown
// __cdecl, variadic. Formats into the 0x1000-wchar buffer at owner+0x844 (_vsnwprintf 0x0052254D) and, on
// success, terminates it and hands it to t3dDeferTextOnOverlayW. No header layout for owner/info yet.
void __cdecl FormatAndDispatchWideText_guess(u8* owner_guess, u32* pRender_guess, u8* info_guess, u32 arg_guess, u32* pArg_guess,
                          const wchar_t* fmt, ...)
{
    int n;
    va_list args;

    va_start(args, fmt);
    n = EQ_CDECL(int, 0x0052254D, (wchar_t*, size_t, const wchar_t*, va_list))(
        (wchar_t*)(owner_guess + 0x844), 0x1000, fmt, args);
    if (n >= 0)
    {
        ((wchar_t*)(owner_guess + 0x844))[n] = 0;
        t3dDeferTextOnOverlayW_guess((wchar_t*)(owner_guess + 0x844), *(u32*)(info_guess + 0x50), arg_guess,
                                     *pArg_guess, *(u32*)(info_guess + 0x4c), *pRender_guess);
    }
}

// FUNCTION: TRILOGY 0x00403738
// Copies a 12-float data block; its specific role is unclear.
// ORIGINAL FILE: unknown
void EQBlock48_guess::Set_guess(const EQBlock48_guess& src)
{
    v[0] = src.v[0];
    v[1] = src.v[1];
    v[2] = src.v[2];
    v[3] = src.v[3];
    v[4] = src.v[4];
    v[5] = src.v[5];
    v[6] = src.v[6];
    v[7] = src.v[7];
    v[8] = src.v[8];
    v[9] = src.v[9];
    v[10] = src.v[10];
    v[11] = src.v[11];
}

// Local stand-in for EQScaledBlock_guess::SetScaled_guess's owner: an 8-float block at +0x44 (copied by 0x004038B4), its
// scaled copy at +0x64 and the two divisors at +0x8C/+0x90. No header layout yet.
struct EQScaledBlock_guess
{
    u8      pad00_guess[0x44];                  // 0x00
    float   src_guess[8];                       // 0x44
    float   scaled_guess[8];                    // 0x64
    u8      pad84_guess[0x08];                  // 0x84
    float   divX_guess;                         // 0x8C
    float   divY_guess;                         // 0x90

    void    SetScaled_guess(const void* src);
};

// FUNCTION: TRILOGY 0x0040383b
// ORIGINAL FILE: unknown
// __thiscall, one stack arg (RET 4). Copies the block, then divides even entries by +0x8C, odd by +0x90.
void EQScaledBlock_guess::SetScaled_guess(const void* src)
{
    EQ_METHOD(1, void, 0x004038B4, (float*, const void*), src_guess)(src);
    scaled_guess[0] = src_guess[0] / divX_guess;
    scaled_guess[1] = src_guess[1] / divY_guess;
    scaled_guess[2] = src_guess[2] / divX_guess;
    scaled_guess[3] = src_guess[3] / divY_guess;
    scaled_guess[4] = src_guess[4] / divX_guess;
    scaled_guess[5] = src_guess[5] / divY_guess;
    scaled_guess[6] = src_guess[6] / divX_guess;
    scaled_guess[7] = src_guess[7] / divY_guess;
}

// Local stand-in: 8 dwords copied member by member (EQBlock32_guess::Set_guess); no header layout yet.
struct EQBlock32_guess
{
    u32     v[8];                               // 0x00

    void    Set_guess(const EQBlock32_guess& src);
};

// FUNCTION: TRILOGY 0x004038b4
// ORIGINAL FILE: unknown
// __thiscall, one stack arg (RET 4); returns nothing.
void EQBlock32_guess::Set_guess(const EQBlock32_guess& src)
{
    v[0] = src.v[0];
    v[1] = src.v[1];
    v[2] = src.v[2];
    v[3] = src.v[3];
    v[4] = src.v[4];
    v[5] = src.v[5];
    v[6] = src.v[6];
    v[7] = src.v[7];
}

// Local stand-in for EQSizedQuad_guess::SetSize_guess's owner: four corner points at +0x44, their scaled copies at +0x64,
// the size at +0x84 and the two divisors at +0x8C/+0x90. No header layout yet.
struct EQSizedQuad_guess
{
    u8      pad00_guess[0x44];                  // 0x00
    float   pts_guess[8];                       // 0x44
    float   scaled_guess[8];                    // 0x64
    float   width_guess;                        // 0x84
    float   height_guess;                       // 0x88
    float   divX_guess;                         // 0x8C
    float   divY_guess;                         // 0x90

    void    SetSize_guess(float width, float height);
};

// FUNCTION: TRILOGY 0x004038e9
// Sets the displayed size of a UI quad.
// ORIGINAL FILE: unknown
// __thiscall, two stack args (RET 8). Stores the size, moves the right/bottom corners to
// left + width - 1 / top + height - 1, then rescales all eight coordinates (x by +0x8C, y by +0x90).
void EQSizedQuad_guess::SetSize_guess(float width, float height)
{
    float right;
    float bottom;

    width_guess = width;
    right = width + pts_guess[0] - 1.0f;
    height_guess = height;
    pts_guess[2] = right;
    pts_guess[4] = right;
    bottom = height + pts_guess[1] - 1.0f;
    pts_guess[5] = bottom;
    pts_guess[7] = bottom;
    scaled_guess[0] = pts_guess[0] / divX_guess;
    scaled_guess[1] = pts_guess[1] / divY_guess;
    right = right / divX_guess;
    scaled_guess[2] = right;
    scaled_guess[3] = pts_guess[3] / divY_guess;
    scaled_guess[4] = right;
    bottom = bottom / divY_guess;
    scaled_guess[5] = bottom;
    scaled_guess[6] = pts_guess[6] / divX_guess;
    scaled_guess[7] = bottom;
}

// The string-keyed hash table of the attribute map (an STL hash_map with chained buckets) and the
// (node, table) iterator its find returns through a hidden pointer. Bucket initialisation is 0x004043DB.
struct EQStrHashNode_guess
{
    EQStrHashNode_guess*    next;               // 0x00
    char*                   key;                // 0x04
};

class EQStrHashMap_guess;

struct EQStrHashIter_guess
{
    EQStrHashNode_guess*    node;               // 0x00
    EQStrHashMap_guess*     table;              // 0x04
};

class EQStrHashMap_guess
{
public:
    SuiteHasher_guess hash;                 // 0x00
    SuiteKeyEqual_guess equals;             // 0x01
    SuiteKeyExtractor_guess getKey;         // 0x02
    SuiteBucketVector_guess buckets;        // 0x04
    SuiteAllocProxy_guess<unsigned int> elementCount; // 0x10

    EQStrHashMap_guess(unsigned int bucketCount, const SuiteHasher_guess& hasher,
        const SuiteKeyEqual_guess& keyEqual, const SuiteAllocator_guess& allocator)
        : hash(hasher), equals(keyEqual), getKey(SuiteKeyExtractor_guess()), buckets(allocator), elementCount(allocator, 0)
    {
        InitializeBuckets_guess(bucketCount);
    }
    // EXTERNAL: TRILOGY 0x004043DB EQStrHashMap_guess::InitializeBuckets_guess
    void InitializeBuckets_guess(unsigned int requestedCount);
    EQStrHashIter_guess* FindNodeByKey_guess(EQStrHashIter_guess* out, char** key);
};

// The hash_map itself: its table with 100 initial buckets.
class EQStringMap_guess
{
public:
    EQStrHashMap_guess table;               // 0x00

    EQStringMap_guess();
    // Clears the table (0x0040345A), then the bucket vector frees its storage; the client's out-of-line copy is
    // 0x00403DF1.
    ~EQStringMap_guess()
    {
        EQ_METHOD(0, void, 0x0040345A, (void*), this)();
    }
};

// FUNCTION: TRILOGY 0x004042a6
// Starts an empty string-keyed hash_map with storage for 100 table entries.
EQStringMap_guess::EQStringMap_guess()
    : table(100, SuiteHasher_guess(), SuiteKeyEqual_guess(), SuiteAllocator_guess())
{
}

// Members of the 0x9C-byte static header element (constructor 0x00403CDF). Their constructors and destructors are
// the client's out-of-line functions (several are also defined in this file or BoxDraw.cpp under stand-in names).

// +0x0C: a pointer vector whose end-of-storage proxy (allocator plus pointer) has an out-of-line constructor.
class EQHeaderEndProxy_guess : public SuiteAllocator_guess
{
public:
    void* data;                             // 0x00

    // EXTERNAL: TRILOGY 0x004043D0 EQHeaderEndProxy_guess::EQHeaderEndProxy_guess
    EQHeaderEndProxy_guess(const SuiteAllocator_guess& allocator, void* value);
};

class EQHeaderVector_guess
{
public:
    void* first;                            // 0x00
    void* last;                             // 0x04
    EQHeaderEndProxy_guess endOfStorage;    // 0x08

    EQHeaderVector_guess(const SuiteAllocator_guess& allocator)
        : first(0), last(0), endOfStorage(allocator, 0)
    {
    }
    // Returns the storage to the Suite pool (pool_free_guess); the client's out-of-line copy is 0x0040428D.
    ~EQHeaderVector_guess()
    {
        EQ_METHOD(0, void, 0x00401167, (void*), this)();
    }
};

// +0x18: the text gauge buffer (AllocateAndClearBuffer_guess 0x00404979 builds it).
class EQHeaderGauge_guess
{
public:
    u32     vtable;                         // 0x00
    char*   buffer;                         // 0x04
    int     size;                           // 0x08
    u32     f0c;                            // 0x0C

    // EXTERNAL: TRILOGY 0x00404979 EQHeaderGauge_guess::EQHeaderGauge_guess
    EQHeaderGauge_guess();
    // EXTERNAL: TRILOGY 0x004049CA EQHeaderGauge_guess::~EQHeaderGauge_guess
    ~EQHeaderGauge_guess();
};

// +0x2C: eight floats, built as 0, 0, 0, 0, 1, 0, 0, 1.
class EQHeaderGeometry_guess
{
public:
    float   values[8];                      // 0x00

    EQHeaderGeometry_guess()
    {
        values[0] = 0.0f;
        values[1] = 0.0f;
        values[2] = 0.0f;
        values[3] = 0.0f;
        values[4] = 1.0f;
        values[5] = 0.0f;
        values[6] = 0.0f;
        values[7] = 1.0f;
    }
    void SetValues_guess(float v2, float v3, float v4, float v5, float v6, float v7)
    {
        values[2] = v2;
        values[3] = v3;
        values[4] = v4;
        values[5] = v5;
        values[6] = v6;
        values[7] = v7;
    }
};

// +0x4C: the twelve-float block (InitBlock48_guess 0x0040186A builds it).
class EQHeaderBlock48_guess
{
public:
    float   values[12];                     // 0x00

    // EXTERNAL: TRILOGY 0x0040186A EQHeaderBlock48_guess::EQHeaderBlock48_guess
    EQHeaderBlock48_guess();
};

// +0x7C: a four-byte colour, built black.
class EQHeaderColor_guess
{
public:
    u8      c[4];                           // 0x00

    EQHeaderColor_guess()
    {
        c[0] = 0;
        c[1] = 0;
        c[2] = 0;
        c[3] = 0;
    }
    void Set_guess(u8 c3, u8 c2, u8 c1, u8 c0)
    {
        c[3] = c3;
        c[2] = c2;
        c[1] = c1;
        c[0] = c0;
    }
};

// The static header element: the 0x9C-byte base of the header/text/spell-gem controls. The vtable at +0 is
// stored raw by the constructor body.
class EQStaticHeaderElement_guess
{
public:
    u32     vtable_guess;                   // 0x00
    u32     unknown04_guess;                // 0x04
    u32     unknown08_guess;                // 0x08
    EQHeaderVector_guess list_guess;        // 0x0C
    EQHeaderGauge_guess gauge_guess;        // 0x18
    u8      enabled_guess;                  // 0x28
    u8      unknown29_guess;                // 0x29
    u8      unknown2a_guess;                // 0x2A
    u8      unknown2b_guess;                // 0x2B
    EQHeaderGeometry_guess geometry_guess;  // 0x2C
    EQHeaderBlock48_guess block_guess;      // 0x4C
    EQHeaderColor_guess color_guess;        // 0x7C
    u32     unknown80_guess;                // 0x80
    char*   text_guess;                     // 0x84
    EQStringMap_guess attributes_guess;     // 0x88

    EQStaticHeaderElement_guess();
};

// FUNCTION: TRILOGY 0x00403cdf
// Initializes text display defaults, including empty content, default geometry, and white color.
EQStaticHeaderElement_guess::EQStaticHeaderElement_guess()
    : list_guess(SuiteAllocator_guess())
{
    vtable_guess = (u32)EQ_ARRAY(u8, 0x005344B8);
    unknown04_guess = 0;
    unknown08_guess = 0;
    text_guess = new char[2];
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(text_guess, EQ_ARRAY(char, 0x0058FBA4));
    enabled_guess = 1;
    unknown29_guess = 0;
    unknown2a_guess = 0;
    geometry_guess.SetValues_guess(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    color_guess.Set_guess(0xff, 0xff, 0xff, 0xff);
}

// FUNCTION: TRILOGY 0x00403f3d
// Runs the third callback on each listed object; the callback's specific effect is unclear.
// ORIGINAL FILE: unknown
void EQObjectList_guess::CallSlot2OnAll_guess()
{
    EQSlot2Object_guess** p;

    for (p = begin; p != end; p++)
        (*p)->Slot2_guess();
}

// Local stand-ins: the attribute target (SetAttribute 0x00404038 is declared on EQAttributeTarget_guess
// further down) with the string map at +0x88, and the by-value argument pair of 0x004042F6.
struct EQAttrMapOwner_guess
{
    void SetAttributeString_guess(char* name, char* value);
};

struct EQMapNodePair_guess
{
    void* node;
    int next;
};

// FUNCTION: TRILOGY 0x00403F58
// Stores an accepted UI setting so it can be looked up by attribute name later.
// __thiscall, callee pops 8: sets the attribute name/value; when accepted, replaces (or inserts) the
// strdup'd name/value pair in the map at +0x88.
void EQAttrMapOwner_guess::SetAttributeString_guess(char* attributeName, char* settingText)
{
    EQMapNodePair_guess iter;
    void* key;
    char* copy;
    int entry;

    if (EQ_METHOD(2, int, 0x00404038, (void*, char*, char*), this)(attributeName, settingText) != 0)
    {
        EQ_METHOD(2, int*, 0x00404387, (void*, EQMapNodePair_guess*, char**), (char*)this + 0x88)(&iter, &attributeName);
        if (iter.node != 0)
        {
            EQ_CDECL(void, 0x005218BB, (void*))(*(void**)((char*)iter.node + 8));
            key = *(void**)((char*)iter.node + 4);
            EQ_METHOD(1, void, 0x004042F6, (void*, EQMapNodePair_guess), (char*)this + 0x88)(iter);
        }
        else
        {
            key = EQ_CDECL(void*, 0x005218C6, (u32))(EQ_CDECL(u32, 0x005223D0, (char*))(attributeName) + 1);
            EQ_CDECL(char*, 0x005219C0, (void*, char*))(key, attributeName);
        }
        copy = (char*)EQ_CDECL(void*, 0x005218C6, (u32))(EQ_CDECL(u32, 0x005223D0, (char*))(settingText) + 1);
        EQ_CDECL(char*, 0x005219C0, (char*, char*))(copy, settingText);
        iter.next = 0;
        iter.node = key;
        entry = EQ_METHOD(1, int, 0x0040455A, (void*, EQMapNodePair_guess*), (char*)this + 0x88)(&iter);
        *(char**)(entry + 4) = copy;
    }
}

// Local stand-in: owner of a string-keyed hash map at +0x88 (0x00404387) and a default value at +0x84;
// no header layout yet. Only referenced from data at 0x00534508 (likely a vtable).
struct EQStrLookupOwner_guess
{
    u32 Lookup_guess(char* key);
};

// FUNCTION: TRILOGY 0x00404006
// Looks up a UI setting by name and returns its stored value or the default.
// __thiscall, callee pops 4: finds `key` in the map at +0x88 and returns the found node's +0x08 value,
// or the default at +0x84 when there is no match.
u32 EQStrLookupOwner_guess::Lookup_guess(char* key)
{
    int iter[2];

    EQ_METHOD(2, int*, 0x00404387, (void*, int*, char**), (char*)this + 0x88)(iter, &key);
    if (iter[0] != 0)
        return *(u32*)(iter[0] + 8);
    return *(u32*)((char*)this + 0x84);
}

// Local stand-in: attribute dispatcher with callbacks in the object's vtable.
class EQAttributeTarget_guess
{
public:
    virtual void Slot00_guess();
    virtual void Slot01_guess();
    virtual void Slot02_guess();
    virtual void Slot03_guess();
    virtual void Slot04_guess();
    virtual void Slot05_guess();
    virtual void Slot06_guess();
    virtual void Slot07_guess();
    virtual void Slot08_guess();
    virtual void Slot09_guess();
    virtual void Slot10_guess();
    virtual void Slot11_guess();
    virtual void Slot12_guess();
    virtual void Slot13_guess();
    virtual void Slot14_guess();
    virtual void Slot15_guess();
    virtual void LocationChanged_guess(void* value);
    virtual void Slot17_guess();
    virtual void SizeChanged_guess(void* value);
    virtual void ColorChanged_guess(void* value);

    int SetAttribute_guess(char* name, char* value);
};

// FUNCTION: TRILOGY 0x00404038
// Applies a UI target's name, visibility, location, size, or color setting.
int EQAttributeTarget_guess::SetAttribute_guess(char* name, char* value)
{
    int result;

    result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, EQ_ARRAY(char, 0x0053C354));
    if (result == 0)
    {
        EQ_METHOD(1, void, 0x004049DA, (void*, char*), (char*)this + 0x18)(value);
    }
    else
    {
        result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, EQ_ARRAY(char, 0x0053C34C));
        if (result == 0)
        {
            result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(value, EQ_ARRAY(char, 0x0053C344));
            if (result == 0)
                *((u8*)this + 0x28) &= (u8)result;
            else
                *((u8*)this + 0x28) = 1;
        }
        else
        {
            result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, EQ_ARRAY(char, 0x0053C338));
            if (result == 0)
            {
                EQ_METHOD(1, void, 0x004041F9, (void*, char*), (char*)this + 0x34)(value);
                LocationChanged_guess((char*)this + 0x34);
            }
            else
            {
                result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, EQ_ARRAY(char, 0x0053C330));
                if (result == 0)
                {
                    EQ_METHOD(1, void, 0x004041F9, (void*, char*), (char*)this + 0x40)(value);
                    SizeChanged_guess((char*)this + 0x40);
                }
                else
                {
                    result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(name, EQ_ARRAY(char, 0x0053C328));
                    if (result == 0)
                        {
                    }
                    else
                        return 1;
                    EQ_METHOD(1, void, 0x0040414F, (void*, const char*), (char*)this + 0x7c)(value);
                    ColorChanged_guess((char*)this + 0x7c);
                }
            }
        }
    }
    return 0;
}

// Local stand-in: 4-byte colour (callers pass ECX = EDI / ESI+0xAC); no header layout yet.
struct EQColorBytes_guess
{
    u8      bytes_guess[4];                     // 0x00

    void ParseCommaList_guess(const char* text);
};

// FUNCTION: TRILOGY 0x0040414f
// Loads a UI color from comma-separated integer components.
// ORIGINAL FILE: unknown
// Parses up to four comma-separated integers ("a,b,c,d") with atoi and stores their low bytes in
// reverse order (first value -> +3, fourth -> +0). Missing values are 0. No bounds check on the
// 16-byte token buffer (original bug kept).
// @early-stop: loop reuses *text from CL (original re-reads it, cmp mem at the bottom) and misses the mov [ebp+8],ecx param store before the loop.
void EQColorBytes_guess::ParseCommaList_guess(const char* text)
{
    char token[16];
    int values[4];
    int len;
    int count;
    int* out;

    len = 0;
    count = 0;
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
    if (*text != '\0')
    {
        out = values;
        while (*text != '\0')
        {
            if (count == 4)
                break;
            if (*text == ',')
            {
                token[len] = '\0';
                *out = EQ_CDECL(int, 0x00522781, (const char*))(token);
                len = 0;
                count++;
                out++;
            }
            else
            {
                token[len] = *text;
                len++;
            }
            text++;
        }
    }
    if (4 > count)
    {
        token[len] = '\0';
        values[count] = EQ_CDECL(int, 0x00522781, (const char*))(token);
    }
    bytes_guess[3] = (u8)values[0];
    bytes_guess[2] = (u8)values[1];
    bytes_guess[1] = (u8)values[2];
    bytes_guess[0] = (u8)values[3];
}

// Local stand-in: a child object whose vtable slot 13 (+0x34) takes one argument; the callee pops it.
// Slots 0-12 are placeholders that only put slot 13 at the right offset.
class EQSlot13Object_guess
{
public:
    virtual void Slot0_guess();
    virtual void Slot1_guess();
    virtual void Slot2_guess();
    virtual void Slot3_guess();
    virtual void Slot4_guess();
    virtual void Slot5_guess();
    virtual void Slot6_guess();
    virtual void Slot7_guess();
    virtual void Slot8_guess();
    virtual void Slot9_guess();
    virtual void Slot10_guess();
    virtual void Slot11_guess();
    virtual void Slot12_guess();
    virtual void Slot13_guess(u32 arg);
};

// Local stand-in: the owner keeps its child pointer at +8. Nothing at +0..+7 is known, and this method does not read it.
struct EQComOwner_guess
{
    u8                      unknown0x00[0x08];  // 0x00
    EQSlot13Object_guess*   child;              // 0x08

    void releaseComInterface_guess(u32 arg);
};
C_ASSERT(offsetof(EQComOwner_guess, child) == 0x08);

// FUNCTION: TRILOGY 0x004041e6
// Forwards a request to the child object; its specific game effect is unclear.
// One stack argument, callee pops it (RET 4). If this+8 is not null, calls the child's vtable slot 13 (+0x34)
// with the argument. Returns nothing.
void EQComOwner_guess::releaseComInterface_guess(u32 arg)
{
    if (child != 0)
    {
        child->Slot13_guess(arg);
    }
}

// Local stand-in: this object is three floats, and the method writes the parsed values there.
struct EQFloat3Output_guess
{
    float values[3];

    void ParseCommaList_guess(char* text);
};

// FUNCTION: TRILOGY 0x004041f9
// Parses a three-number UI attribute such as location or size.
// ORIGINAL FILE: unknown
// Parses comma-separated floats into this three-float output. Extra input after the third comma is ignored;
// the 16-byte token buffer has no bounds check in the original.
void EQFloat3Output_guess::ParseCommaList_guess(char* text)
{
    int len;
    float* current;
    int count;
    double parsed;
    char token[16];
    float values[3];
    float* output;

    values[0] = 0.0f;
    len = 0;
    count = 0;
    values[1] = 0.0f;
    values[2] = 0.0f;
    output = (float*)this;
    if (*text != '\0')
    {
        current = values;
        do
        {
            if (count == 3)
                break;
            if (*text == ',')
            {
                token[len] = '\0';
                parsed = EQ_CDECL(double, 0x00521AB0, (const char*))(token);
                *current = (float)parsed;
                len = 0;
                count = count + 1;
                current = current + 1;
                output = (float*)this;
            }
            else
            {
                token[len] = *text;
                len = len + 1;
            }
            text = text + 1;
        } while (*text != '\0');
        if (2 < count)
            goto writeValues;
    }
    token[len] = '\0';
    parsed = EQ_CDECL(double, 0x00521AB0, (const char*))(token);
    values[count] = (float)parsed;
    output = (float*)this;
writeValues:
    output[0] = values[0];
    output[1] = values[1];
    output[2] = values[2];
    return;
}

// FUNCTION: TRILOGY 0x00404387
// Finds a matching string key in a case-insensitive settings table.
// ORIGINAL FILE: EverQuest/utf8.cpp (from Mac debug map)
// __thiscall, callee pops 8: hash-map find. Hashes *key with 0x004037D4 (case-insensitive), walks that
// bucket's chain for a node whose key matches case-insensitively, and stores (node or 0, this) in *out.
// The Mac name looks wrong for this body. Returns out.
EQStrHashIter_guess* EQStrHashMap_guess::FindNodeByKey_guess(EQStrHashIter_guess* iter, char** key)
{
    unsigned int count;
    unsigned int hash;
    EQStrHashNode_guess* node;

    count = (EQStrHashNode_guess**)buckets.last - (EQStrHashNode_guess**)buckets.first;
    hash = EQ_STDCALL(unsigned int, 0x004037D4, (char**))(key);
    for (node = ((EQStrHashNode_guess**)buckets.first)[hash % count]; node != 0; node = node->next)
    {
        if (_strcmpi(node->key, *key) == 0)
            break;
    }
    iter->table = this;
    iter->node = node;
    return iter;
}

// Owner type unknown (sole caller 0x00403d05 passes ESI+0x14); local stand-in until a header layout exists.
struct EQDwordHolder_guess
{
    u32     value_guess;                        // 0x00

    EQDwordHolder_guess* SetValue_guess(u32 unused_guess, u32 value);
};

// FUNCTION: TRILOGY 0x004043d0
// ORIGINAL FILE: unknown
// First stack argument is ignored; callee pops 8. Returns this (EAX = ECX in the original).
EQDwordHolder_guess* EQDwordHolder_guess::SetValue_guess(u32 unused_guess, u32 value)
{
    value_guess = value;
    return this;
}

// FUNCTION: TRILOGY 0x00404930
// Completes shared initialization by installing this object's concrete behavior.
u32* __fastcall InitializeObjectWithVtable5450C_guess(u32* instance)
{
    EQ_METHOD(0, void, 0x00403CDF, (u32*), instance)();
    *instance = 0x0053450c;
    return instance;
}



// FUNCTION: TRILOGY 0x00404979
// Prepares an empty character buffer for a UI control; its specific use is unclear.
// ORIGINAL FILE: unknown
// ECX = object (this); no stack args. Sets the vtable pointer, size 0x40, and a zeroed 0x82-byte buffer
// (memset covers size + 1 bytes only). Returns this.
EQGaugeBuf_guess* __fastcall AllocateAndClearBuffer_guess(EQGaugeBuf_guess* obj)
{
    obj->f0c = 0;
    obj->vtable = 0x00534560;
    obj->size = 0x40;
    obj->buffer = new char[0x82];
    memset(obj->buffer, 0, obj->size + 1);
    return obj;
}

// kernel32 MultiByteToWideChar without <windows.h>.
extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int codePage, unsigned long flags,
    const char* src, int srcLen, unsigned short* dst, int dstLen);

// Local stand-in: wide-string buffer guessed from AssignAnsiStringToWideBuffer_guess alone (+0x00 not touched here).
struct EQWideBuf_guess
{
    u32             f00;                        // 0x00
    unsigned short* buffer;                     // 0x04
    u32             capacity;                   // 0x08 (chars incl. NUL)
    u32             length;                     // 0x0C
};

// FUNCTION: TRILOGY 0x004049DA
// Loads display text from an ANSI string into a wide-character buffer.
// ORIGINAL FILE: unknown
// __thiscall (ECX = buffer object), one stack arg. Converts an ANSI string into the wide buffer, growing it
// when strlen + 1 exceeds the capacity. The terminator is written at index strlen + 1 (one past the
// converted NUL); the length field is only updated on growth.
void __fastcall AssignAnsiStringToWideBuffer_guess(EQWideBuf_guess* obj, int unusedEdx, const char* ansiText)
{
    u32 count;

    count = strlen(ansiText) + 1;
    if (count > obj->capacity)
    {
        delete obj->buffer;
        obj->buffer = new unsigned short[count + 1];
        obj->capacity = count;
        obj->length = count - 1;
    }
    MultiByteToWideChar(0, 0, ansiText, -1, obj->buffer, obj->capacity * 2);
    obj->buffer[count] = 0;
}

// FUNCTION: TRILOGY 0x00404A36
// Copies one UI text value into its paired buffer.
// __thiscall (ECX = destination buffer object), one stack arg (source buffer object). Copies the source's wide
// string into the destination. When the source length exceeds the capacity, the old block is freed and a new one
// of 2 * length + 2 bytes is allocated; the capacity is set to length and the length field to length - 1.
void __fastcall CopyWideBuffer_guess(EQWideBuf_guess* obj, int unusedEdx, EQWideBuf_guess* src)
{
    if (src->length > obj->capacity)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(obj->buffer);
        obj->buffer = (unsigned short*)EQ_CDECL(void*, 0x005218C6, (u32))(src->length * 2 + 2);
        obj->capacity = src->length;
        obj->length = src->length - 1;
    }
    EQ_CDECL(unsigned short*, 0x0052278C, (unsigned short*, const unsigned short*))(obj->buffer, src->buffer);
}

// FUNCTION: TRILOGY 0x00404A7F
// Adds a typed character to the UI text field.
// __thiscall (ECX = buffer object), one stack arg (a wide char, pushed as a dword). Stores the char at index length
// and writes the NUL after it. When length + 2 exceeds the capacity, the block is replaced first: the capacity grows
// by 16 chars, the new block is 2 * capacity + 2 bytes, the old string is copied into it and the old block is freed.
void __fastcall AppendWideChar_guess(EQWideBuf_guess* obj, int unusedEdx, unsigned short wideChar)
{
    unsigned short* oldBuf;

    if (obj->capacity < obj->length + 2U)
    {
        oldBuf = obj->buffer;
        obj->capacity = obj->capacity + 0x10;
        obj->buffer = (unsigned short*)EQ_CDECL(void*, 0x005218C6, (u32))(obj->capacity * 2 + 2);
        EQ_CDECL(unsigned short*, 0x0052278C, (unsigned short*, const unsigned short*))(obj->buffer, oldBuf);
        EQ_CDECL(void, 0x005218BB, (void*))(oldBuf);
    }
    obj->buffer[obj->length] = wideChar;
    obj->length++;
    obj->buffer[obj->length] = 0;
}



// FUNCTION: TRILOGY 0x00404AD7
// Initializes a control and clears its display state; its specific role is unclear.
// ECX = object (this); no stack args. A constructor: the base init (0x403CDF) and the two gauge members (0x404979) run
// first, then the vtable is stored and the fields are cleared. Returns this.
EQSpellGemControl_guess::EQSpellGemControl_guess()
    : EQSpellGemBase_guess()
{
    vtable_guess = 0x00534564;
    memset(unknowna4_guess, 0, 0x40);
    unknown9c_guess = 0;
    unknowna0_guess = 0;
    unknowne4_guess = 0;
    unknowne8_guess = 0x10;
    unknown10c_guess = 0;
    unknown80_guess = 1;
    unknown2b_guess = 0;
}

// FUNCTION: TRILOGY 0x00404BF8
// Updates an active text display and formats its pending contents.
// @early-stop: A44/B40 loads sit after the cmp (original loads both first); ternary arg uses mov eax, not push
void __fastcall UpdateAndDispatchText_guess(int* textObject)
{
    char isActive;
    f32 local[12];

    isActive = ((EQVirtualList_guess*)textObject)->Slot4_guess();
    if (isActive == '\0')
        return;
    /* the create runs only while slot 0x27 is empty; a failed create leaves it 0 and skips the conversion */
    if (textObject[0x27] != 0 ||
        (textObject[0x27] = EQ_METHOD(3, int, 0x00402FF4, (void*, int*, int, int), (void*)textObject[1])(textObject + 0x29, textObject[0x39], textObject[0x3a])) != 0)
    {
        if ((textObject[0x28] != 0) && (*(char*)((char*)textObject + 0x80) != '\0'))
        {
            EQ_METHOD(0, void, 0x0040186A, (void*), local)();
            local[0] = 2.0f;
            local[1] = 0.0f;
            /* 2.0f as bits: a second float 2.0f here makes VC6 share one constant with local[0] */
            *(u32*)&local[9] = 0x40000000;
            local[3] = *(f32*)((char*)textObject + 0x40) - EQ_GLOBAL(f32, 0x005344A4);
            local[4] = 0.0f;
            local[6] = local[3];
            local[7] = *(f32*)((char*)textObject + 0x44) - EQ_GLOBAL(f32, 0x005344A4);
            local[10] = local[7];
            EQ_CDECL(void, 0x00403129, (void*, int, int, f32*, int*, int))
                ((void*)textObject[1], textObject[0x28], textObject[0x27], local, textObject + 0x1f,
                 *(char*)((char*)textObject + 0x2B) != '\0' ? textObject[0x40] : textObject[0x3c]);
            *(char*)((char*)textObject + 0x80) = 0;
        }
    }
    EQ_METHOD(3, void, 0x00402C98, (void*, int*, int*, int), (void*)textObject[1])(textObject + 0x13, textObject + 0x1f, textObject[0x28]);
    EQ_METHOD(0, void, 0x00403F3D, (void*), textObject)();
    return;
}

// FUNCTION: TRILOGY 0x00404CE9
// Applies font, font size, editability, text, and size settings to a UI text element.
// __fastcall stand-in for a __thiscall (this in ECX, EDX unused, two stack args, callee pops 8). Sets one
// attribute: font / fontsize / editable / text / size. Any other name goes to EQAttrMapOwner_guess::SetAttributeString_guess.
void __fastcall SetTextAttribute_guess(int* textElement, int unusedEdx, char* attributeName, char* settingText)
{
    int compareResult;

    compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C374));
    if (compareResult == 0)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)textElement + 0xa4, settingText);
    }
    else
    {
        compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C368));
        if (compareResult == 0)
        {
            *(int*)((char*)textElement + 0xe4) = EQ_CDECL(int, 0x00522781, (const char*))(settingText);
        }
        else
        {
            compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C35C));
            if (compareResult == 0)
            {
                compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))(settingText, EQ_ARRAY(char, 0x0053C344));
                if (compareResult == 0)
                {
                    *((u8*)textElement + 0x10c) &= (u8)compareResult;
                    *((u8*)textElement + 0x2a) &= (u8)compareResult;
                }
                else
                {
                    *((u8*)textElement + 0x10c) = 1;
                    *((u8*)textElement + 0x2a) = 1;
                }
            }
            else
            {
                compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C2C0));
                if (compareResult == 0)
                {
                    EQ_METHOD(1, void, 0x004049DA, (void*, char*), (char*)textElement + 0xec)(settingText);
                    *((u8*)textElement + 0x80) = 1;
                }
                else
                {
                    compareResult = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C330));
                    if (compareResult == 0)
                    {
                        EQ_METHOD(2, void, 0x00403F58, (void*, char*, char*), textElement)(attributeName, settingText);
                        EQ_METHOD(1, void, 0x00404DE3, (void*, f32*), textElement)((f32*)((char*)textElement + 0x40));
                    }
                    else
                    {
                        EQ_METHOD(2, void, 0x00403F58, (void*, char*, char*), textElement)(attributeName, settingText);
                    }
                }
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00404DE3
// Recreates an overlay using the text element's updated dimensions.
void __fastcall RecreateInstance_guess(int* objectState, int unusedEdx, f32* dimensions)
{
    int createdHandle;

    EQ_METHOD(1, void, 0x00402E9C, (void*, int), (void*)objectState[1])(objectState[0x28]);
    f32* payload = dimensions;
    f32 value = payload[1];
    value += EQ_GLOBAL(f32, 0x005345B8);
    payload[1] = value;
    createdHandle = EQ_METHOD(1, int, 0x00402D2D, (void*, f32*), (void*)objectState[1])(payload);
    objectState[0x28] = createdHandle;
    return;
}

// FUNCTION: TRILOGY 0x00404E17
// Processes text input, including backspace, Enter, and character insertion.
void __fastcall HandleTextBufferKeyEvent_guess(FUN_00404e17Object_guess* object, int unusedEdx, int* event)
{
    short selector;

    if (event[0] == 0x15)
    {
        selector = (short)event[3];
        if (selector == 8)
        {
            EQ_METHOD(0, void, 0x00404EB3, (void*), (void*)&object->stringBuffer)();
        }
        else if (selector == 0xd)
        {
            object->resetByte = 0;
            EQ_METHOD(1, void, 0x00404A36, (void*, void*), (void*)&object->stringOwner)((void*)&object->stringBuffer);
            EQ_METHOD(0, void, 0x00404E80, (void*), (void*)&object->stringBuffer)();
            object->enabledByte = 1;
            return;
        }
        else
            EQ_METHOD(1, void, 0x00404A7F, (void*, short), (void*)&object->stringBuffer)(selector);
        object->enabledByte = 1;
    }
    return;
}

// FUNCTION: TRILOGY 0x00404E80
// Clears a text input buffer for a fresh entry.
// ECX = object (this); no stack args. Frees the buffer at +4, allocates twice the size at +8 for a new one,
// clears +0xC, stores the new buffer at +4, sets +8 to 0x40 and memsets 0x41 bytes of the buffer to 0.
// Returns nothing.
void __fastcall ResetWideBuffer_guess(int* bufferState, int unusedEdx)
{
    void* _Dst;

    EQ_CDECL(void, 0x005218BB, (void*))((void*)bufferState[1]);
    _Dst = EQ_CDECL(void*, 0x005218C6, (unsigned int))(bufferState[2] << 1);
    bufferState[3] = 0;
    bufferState[1] = (int)_Dst;
    bufferState[2] = 0x40;
    EQ_CDECL(void*, 0x00521960, (void*, int, unsigned int))(_Dst, 0, 0x41);
    return;
}

// FUNCTION: TRILOGY 0x00404EB3
// Removes the last character from the current text entry.
// ECX = object (this); no stack args. If the count at +0xC is not zero, decrements it and clears the
// u16 at the new count's index in the buffer at +4. Returns nothing.
void __fastcall RemoveLastWideChar_guess(int* obj, int unusedEdx)
{
    int newCount;

    if (obj[3] != 0)
    {
        newCount = obj[3] - 1;
        obj[3] = newCount;
        ((u16*)obj[1])[newCount] = 0;
    }
    return;
}

// FUNCTION: TRILOGY 0x00404EC7
// Updates a vector block and accumulates values; its in-game effect is unclear.
void __fastcall UpdateVectorAndAccumulateValues_guess(void* obj, int unusedEdx, void* sourceVector)
{
    f32 local[12];
    f32* destination;

    EQ_METHOD(1, void, 0x00401762, (void*, void*), obj)(sourceVector);
    EQ_METHOD(0, void, 0x0040186A, (void*), local)();
    destination = (f32*)((char*)obj + 0x4C);
    local[0] = 1.0f;
    local[1] = 1.0f;
    local[3] = EQ_GLOBAL(f32, 0x005345BC);
    local[4] = 1.0f;
    local[6] = EQ_GLOBAL(f32, 0x005345BC);
    local[7] = EQ_GLOBAL(f32, 0x005345BC);
    local[9] = 1.0f;
    local[10] = EQ_GLOBAL(f32, 0x005345BC);
    EQ_METHOD(1, void, 0x00404F17, (void*, f32*), destination)(local);
}





// FUNCTION: TRILOGY 0x00404FEF
// Initializes the default title text shown in a window header.
// ECX = object (this); no stack args. A constructor: the base (0x403CDF) and the gauge (0x404979) run first, then the
// fields are set and the title is written into the gauge (0x4049DA). Returns this.
// @early-stop: the vtable store and the tail's float and flag stores come out in a different order (VC6 schedules them).
/* fake match: the gauge (in its constructor) and the +0xB8 vector are called through pointer locals; that is what makes VC6 emit the state stores. */
EQStaticHeaderTitle_guess::EQStaticHeaderTitle_guess()
{
    /* fake match: the vector construction at +0xB8 is a call through a pointer variable, so VC6 stores the state after
       the gauge before it, as the original does. The one at +0xF4 is a plain call: no state change follows the gauge. */
    void (__fastcall *vector)(void*) = EQ_THISCALL(void, 0x0040186A, (void*));

    titleColor[0] = 0;
    titleColor[1] = 0;
    titleColor[2] = 0;
    titleColor[3] = 0;
    vector(&minimumX_guess);
    color_copy_ec[0] = 0;
    color_copy_ec[1] = 0;
    color_copy_ec[2] = 0;
    color_copy_ec[3] = 0;
    EQ_METHOD(0, void, 0x0040186A, (void*), contentQuad_guess)();
    vtable_guess = 0x005345C0;
    titleColor[3] |= 0xFF;
    titleColor[2] |= 0xFF;
    titleColor[1] |= 0xFF;
    titleColor[0] |= 0xFF;
    fontName_guess = (char*)0x0053C37C;
    EQ_METHOD(1, void, 0x004049DA, (void*, const char*), &gauge_guess)((const char*)0x0053C390);
    box_table_guess = 0;
    titleCorner0Z_guess = 1.0f;
    titleCorner1Z_guess = 1.0f;
    titleCorner2Z_guess = 1.0f;
    titleCorner3Z_guess = 1.0f;
    color_copy_ec[3] |= 0xFF;
    color_copy_ec[2] = 0;
    color_copy_ec[1] = 0;
    color_copy_ec[0] = 0;
    callbackValue = 0x0B;
    titleOverlay_guess = 0;
    selectionState_guess = 0;
    fill = 1;
}

// FUNCTION: TRILOGY 0x00405193
// If slot 4 returns nonzero, builds 4-corner quads from +0x40/+0x44 and passes them to the object at +4.
void __fastcall BuildQuads_guess(void* obj, int unusedEdx)
{
    char isActive;
    int handle;
    EQVector3_guess* vectorDifference;
    bool creationFailed;
    int* ptr;
    struct LocalData_guess
    {
        f32 rect[12];
        EQVector3_guess vector;
        u32 local_14;
        f32 local_10;
        u32 local_c;
        int local_8;
    } localData;
    int* self;

    self = (int*)obj;
    isActive = ((EQVirtualList_guess*)obj)->Slot4_guess();
    if (isActive != '\0')
    {
        if (0 < self[0x3a])
        {
            if (((char)self[0x20] != '\0') && (self[0x2c] != 0))
            {
                EQ_METHOD(0, void, 0x0040186A, (void*), localData.rect)();
                localData.rect[0] = 2.0f;
                localData.rect[1] = 0.0f;
                localData.rect[3] = *(f32*)((char*)obj + 0x40) - EQ_GLOBAL(f32, 0x005344A4);
                localData.rect[4] = 0.0f;
                localData.rect[6] = localData.rect[3];
                localData.rect[7] = *(f32*)((char*)obj + 0x44) - EQ_GLOBAL(f32, 0x005344A4);
                localData.rect[9] = 2.0f;
                localData.rect[10] = localData.rect[7];
                EQ_CDECL(void, 0x00403129, (void*, int, int, f32*, int*, int))
                    (*(void**)((char*)obj + 4), self[0x3c], self[0x2c], localData.rect,
                        self + 0x2b,
                        self[0x28]);
            }
            ptr = self + 0x2e;
            EQ_METHOD(3, void, 0x00402C98, (void*, void*, void*, int), *(void**)((char*)obj + 4))(ptr, self + 0x2b, self[0x3c]);
            EQ_METHOD(3, void, 0x00402C98, (void*, void*, void*, int), *(void**)((char*)obj + 4))(ptr, self + 0x3b, 0);
            EQ_METHOD(2, void, 0x00402C83, (void*, void*, void*), *(void**)((char*)obj + 4))(ptr, self + 0x3b);
            creationFailed = false;
            if (self[0x2c] == 0)
            {
                handle = EQ_METHOD(3, int, 0x00402FF4, (void*, int, int, int), *(void**)((char*)obj + 4))(self[0x2d], self[0x3a] + 1, 0);
                self[0x2c] = handle;
                creationFailed = handle == 0;
            }
            if (!creationFailed)
            {
                EQ_METHOD(0, void, 0x0040186A, (void*), localData.rect)();
                if (self[0x3c] == 0)
                {
                    localData.local_14 = 0;
                    localData.local_10 = 0.0f;
                    localData.local_c = 0x3f800000;
                    vectorDifference = EQ_METHOD(2, EQVector3_guess*, 0x004053FB, (EQVector3_guess*, EQVector3_guess*, const EQVector3_guess*), (EQVector3_guess*)((char*)obj + 0xD0))(&localData.vector,
                            (const EQVector3_guess*)ptr);
                    localData.local_14 = *(u32*)vectorDifference;
                    localData.local_c = *((u32*)vectorDifference + 2);
                    localData.local_10 = vectorDifference->y + EQ_GLOBAL(f32, 0x005345B8);
                    handle = EQ_METHOD(1, int, 0x00402D2D, (void*, void*), *(void**)((char*)obj + 4))(&localData.local_14);
                    self[0x3c] = handle;
                }
                if ((char)self[0x20] != '\0')
                {
                    localData.local_8 = self[0x3a] + -1;
                    localData.rect[0] = 2.0f;
                    localData.rect[7] = (f32)localData.local_8;
                    localData.rect[3] = *(f32*)((char*)obj + 0x40) - EQ_GLOBAL(f32, 0x005344A4);
                    localData.rect[1] = 0.0f;
                    localData.rect[9] = 2.0f;
                    localData.rect[4] = 0.0f;
                    localData.rect[6] = localData.rect[3];
                    localData.rect[10] = localData.rect[7];
                    EQ_CDECL(void, 0x00403129, (void*, int, int, f32*, int*, int))
                        (*(void**)((char*)obj + 4), self[0x3c], self[0x2c], localData.rect,
                            self + 0x2b,
                            self[0x28]);
                }
                EQ_METHOD(1, void, 0x00403738, (void*, void*), localData.rect)(ptr);
                localData.rect[1] = localData.rect[1] - 1.0f;
                localData.rect[4] = localData.rect[4] - 1.0f;
                localData.rect[10] = localData.rect[10] + 1.0f;
                localData.rect[7] = localData.rect[7] + 1.0f;
                EQ_METHOD(3, void, 0x00402C98, (void*, void*, void*, int), *(void**)((char*)obj + 4))(localData.rect, self + 0x2b,
                        self[0x3c]);
            }
        }
        if ((char)self[0x49] == '\0')
        {
            if (*(char*)((char*)obj + 0x125) == '\0')
            {
                ptr = self + 0x1f;
            }
            else
            {
                EQ_METHOD(3, void, 0x00402C98, (void*, void*, void*, int), *(void**)((char*)obj + 4))(self + 0x3d, self + 0x1f, 0);
                ptr = self + 0x3b;
            }
            EQ_METHOD(2, void, 0x00402C83, (void*, void*, void*), *(void**)((char*)obj + 4))(self + 0x3d, ptr);
            EQ_METHOD(0, void, 0x00403F3D, (void*), obj)();
        }
    }
}

// FUNCTION: TRILOGY 0x004053fb
// ORIGINAL FILE: unknown
// The result is built in a local and copied out (no return-value optimisation in VC6).
EQVector3_guess EQVector3_guess::Minus_guess(const EQVector3_guess& rhs) const
{
    EQVector3_guess r;

    r.x = x - rhs.x;
    r.y = y - rhs.y;
    r.z = z - rhs.z;
    return r;
}

// Local stand-in for the object as the original calls it: slot 16 (+0x40) takes one pointer.
// The callback data begins at +0x34 and the callback value is at +0xE8.
class EQVirtualSlot16Ptr_guess
{
public:
    virtual void Slot0_guess();
    virtual void Slot1_guess();
    virtual void Slot2_guess();
    virtual void Slot3_guess();
    virtual void Slot4_guess();
    virtual void Slot5_guess();
    virtual void Slot6_guess();
    virtual void Slot7_guess();
    virtual void Slot8_guess();
    virtual void Slot9_guess();
    virtual void Slot10_guess();
    virtual void Slot11_guess();
    virtual void Slot12_guess();
    virtual void Slot13_guess();
    virtual void Slot14_guess();
    virtual void Slot15_guess();
    virtual void Slot16_guess(void* arg);

    u8 unknown_004[0x30];
    u8 callbackData[0xb4];
    int callbackValue;
};

// FUNCTION: TRILOGY 0x00405430
// Applies title bar size, fill, title text, and color settings.
// __thiscall (this in ECX, name and value on the stack, callee pops 8). Applies one attribute by name
// (case-insensitive). Every branch falls through to the same return.
void __fastcall SetTitleBarAttribute_guess(void* titleBar, int, char* attributeName, char* settingText)
{
    int result;
    u8* selfBytes;

    selfBytes = (u8*)titleBar;
    result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C3C4));
    if (result == 0)
    {
        result = EQ_CDECL(int, 0x00522781, (const char*))(settingText);
        *(int*)(selfBytes + 0xe8) = result;
        ((EQVirtualSlot16Ptr_guess*)titleBar)->Slot16_guess(selfBytes + 0x34);
    }
    else
    {
        result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C3BC));
        if (result == 0)
        {
            result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(settingText, EQ_ARRAY(char, 0x0053C344));
            if (result == 0)
                *(selfBytes + 0x125) &= (u8)result;
            else
                *(selfBytes + 0x125) = 1;
        }
        else
        {
            result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C3B4));
            if (result == 0)
            {
                EQ_METHOD(1, void, 0x004049DA, (void*, char*), selfBytes + 0x9c)(settingText);
            }
            else
            {
                result = EQ_CDECL(int, 0x00524240, (const char*, const char*))(attributeName, EQ_ARRAY(char, 0x0053C3A8));
                if (result == 0)
                {
                    EQ_METHOD(1, void, 0x0040414F, (void*, char*), selfBytes + 0xac)(settingText);
                }
                else
                {
                    EQ_METHOD(2, void, 0x00403F58, (void*, char*, char*), titleBar)(attributeName, settingText);
                }
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004054ec
// Invokes a callback for a new value; the callback's game or UI purpose is unclear.
void __fastcall SetCallbackValueAndInvokeSlot16_guess(EQVirtualSlot16Ptr_guess* target, int unusedEdx, int value)
{
    target->callbackValue = value;
    target->Slot16_guess(target->callbackData);
}

// FUNCTION: TRILOGY 0x00405502
// Updates hover and selection state for pointer requests inside a bounded UI area.
void __fastcall HandlePointerRequest_guess(FUN_00405502Object_guess* self, int unusedEdx, FUN_00405502Request_guess* request)
{
    int requestType;

    requestType = request->type;
    if (requestType == 3)
    {
        if (request->x >= self->minimumX && request->y >= self->minimumY &&
            request->x <= self->maximumX && request->y <= self->maximumY)
        {
            self->selectionState = self->selectionState == 0;
            goto deleteRequest;
        }
    }
    else if (requestType == 0)
    {
        if (request->x >= self->minimumX && request->y >= self->minimumY &&
            request->x <= self->maximumX && request->y <= self->maximumY)
        {
            self->hoverState = 1;
        }
    }
    else if (requestType == 1)
    {
        self->hoverState = 0;
        goto deleteRequest;
    }
    EQ_STDCALL(void, 0x004041E6, (void*))(request);
    return;

deleteRequest:
    EQ_CDECL(void, 0x005218BB, (void*))(request);
    return;
}

// FUNCTION: TRILOGY 0x004055b4
// __fastcall, ECX = self, one stack arg (callee pops 4). Calls UpdateVectorAndNotifyObjects_guess(source), copies the block at +0x4C into
// +0xF4 (Set_guess), updates the floats at +0x4C..+0x104 from the counter at +0xE8, then calls slot 17 on each object
// in the list at +0x0C.
/* fake match: keep the original x87 load/add order for the two float updates. */
void __fastcall UpdateBlockState_guess(void* self, int edx_guess, void* source)
{
    f32* block;
    f32 nextCounterValue;
    f32 counterValue;
    u32* callbackEntry;
    u8* selfBytes;
    u8* sourceBytes;
    int counter_guess;

    selfBytes = (u8*)self;
    sourceBytes = (u8*)source;
    EQ_THISCALL(void, 0x00401762, (void*, int, void*))(self, edx_guess, source);
    block = (f32*)(selfBytes + 0x4c);
    EQ_METHOD(1, void, 0x00403738, (void*, const void*), selfBytes + 0xf4)(block);
    f32 one_guess = EQ_GLOBAL(f32, 0x005344A4);
    *(u32*)(selfBytes + 0xb8) = *(u32*)block;
    *(u32*)(selfBytes + 0xbc) = ((u32*)block)[1];
    *(u32*)(selfBytes + 0xc0) = ((u32*)block)[2];
    counter_guess = *(int*)(selfBytes + 0xe8);
    f32 currentBlockValue = *block;
    *block = currentBlockValue + one_guess;
    nextCounterValue = (f32)(counter_guess + 1);
    callbackEntry = *(u32**)(selfBytes + 0x0c);
    *(f32*)(selfBytes + 0x50) = nextCounterValue + *(f32*)(selfBytes + 0x50);
    counterValue = (f32)counter_guess;
    *(f32*)(selfBytes + 0xf8) = counterValue + *(f32*)(selfBytes + 0xf8);
    *(u32*)(selfBytes + 0xc4) = *(u32*)(selfBytes + 0x58);
    *(u32*)(selfBytes + 0xc8) = *(u32*)(selfBytes + 0x5c);
    *(u32*)(selfBytes + 0xcc) = *(u32*)(sourceBytes + 8);
    *(f32*)(selfBytes + 0x58) = *(f32*)(selfBytes + 0x58) - one_guess;
    *(f32*)(selfBytes + 0x5c) = nextCounterValue + *(f32*)(selfBytes + 0x5c);
    *(f32*)(selfBytes + 0x104) = counterValue + *(f32*)(selfBytes + 0x104);
    *(u32*)(selfBytes + 0xd0) = *(u32*)(selfBytes + 0xc4);
    *(f32*)(selfBytes + 0xd4) = counterValue + *(f32*)(selfBytes + 0xc8);
    *(u32*)(selfBytes + 0xd8) = *(u32*)(sourceBytes + 8);
    *(u32*)(selfBytes + 0xdc) = *(u32*)(selfBytes + 0xb8);
    *(f32*)(selfBytes + 0x64) = *(f32*)(selfBytes + 0x64) - one_guess;
    *(f32*)(selfBytes + 0x68) = *(f32*)(selfBytes + 0x68) - one_guess;
    *(f32*)(selfBytes + 0xe0) = counterValue + *(f32*)(selfBytes + 0xbc);
    *(u32*)(selfBytes + 0xe4) = *(u32*)(sourceBytes + 8);
    f32 current70Guess = *(f32*)(selfBytes + 0x70);
    *(f32*)(selfBytes + 0x70) = current70Guess + one_guess;
    *(f32*)(selfBytes + 0x74) = *(f32*)(selfBytes + 0x74) - one_guess;
    for (; callbackEntry != *(u32**)(selfBytes + 0x10); callbackEntry = callbackEntry + 1)
        ((EQSlot17Object_guess*)*callbackEntry)->Slot17_guess();
}
