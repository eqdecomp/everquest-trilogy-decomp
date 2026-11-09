// XWnd.cpp - window class of the Suite UI library.

#include "trilogy/common.h"
#include "trilogy/player.h"
#include "trilogy/globals.h"

struct XWndArea_guess
{
    char pad_0x00[0x4c];
    float minimum_x;
    float minimum_y;
    char pad_0x54[0x10];
    float maximum_x;
    float maximum_y;
    char pad_0x6c[0x4c];
    float alternate_minimum_x;
    float alternate_minimum_y;
    char pad_0xc0[0x10];
    float alternate_maximum_x;
    float alternate_maximum_y;
    char pad_0xd8[0x10];
    int use_alternate_bounds;

    unsigned short GetPointBoundsStatus_guess(const float *point);
    unsigned short AreaContainsPoint_guess(const float *point);
};

struct XWndKeyMsg_guess
{
    int a;      // 0x0
    int b;      // 0x4
    int c;      // 0x8
};

// Local stand-in: only the vtable slot and the fields this function touches are laid out.
class CXWnd_guess
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
    virtual void Slot16_guess(int *arg);    // vtable +0x40
    void HandleKeyboardMsg_guess(const XWndKeyMsg_guess *msg);
    void HandleKeyboardMsgAndNotify_guess(const XWndKeyMsg_guess *msg);
    void SetBytes7C_guess(const unsigned char *message);

    char pad_0x04_guess[0x30];  // 0x4
    int arg_guess[3];           // 0x34 (passed to slot 16)
    int key_a;                  // 0x40
    int key_b;                  // 0x44
    int key_c;                  // 0x48
    char pad_0x4c_guess[0x30];
    unsigned char key_message_byte_0;
    unsigned char key_message_byte_1;
    unsigned char key_message_byte_2;
    unsigned char key_message_byte_3;
    void FUN_00404f87_guess(const XWndKeyMsg_guess *msg);
};

class XWndColor_guess
{
public:
    void SetColor_guess(const unsigned char *color);

    char pad_0x00_guess[0x7c];
    unsigned char color_copy_7c[4];
    char pad_0x80_guess[0x6c];
    unsigned char color_copy_ec[4];
};

// FUNCTION: TRILOGY 0x00401905
// ORIGINAL FILE: Suite/Suite/XWnd.cpp (from Mac debug map)
// __thiscall. Stores the 3-dword message at +0x40 and passes this+0x34 to vtable slot 0x40.
void CXWnd_guess::HandleKeyboardMsg_guess(const XWndKeyMsg_guess *msg)
{
    key_a = msg->a;
    key_b = msg->b;
    key_c = msg->c;
    Slot16_guess(arg_guess);
}

// FUNCTION: TRILOGY 0x00401926
// Copies four bytes into the byte fields at +0x7C..+0x7F (the offset SetColor_guess also writes).
void CXWnd_guess::SetBytes7C_guess(const unsigned char *message)
{
    key_message_byte_3 = message[3];
    key_message_byte_2 = message[2];
    key_message_byte_1 = message[1];
    key_message_byte_0 = message[0];
}

struct XWndRect_guess
{
    char unused_0x00[0x4c];
    float left;
    float top;
    char unused_0x54[0x10];
    float right;
    float bottom;

    bool ContainsPoint_guess(float *point);
};

// FUNCTION: TRILOGY 0x00401944
// Tests whether a point falls inside the rectangle's bounds.
bool XWndRect_guess::ContainsPoint_guess(float *point)
{
    if (point[0] >= left)
    {
        if (point[1] >= top)
        {
            if (point[0] <= right)
            {
                if (point[1] <= bottom)
                    return true;
            }
        }
    }
    return false;
}

// Local stand-in: 12 floats, embedded at +0x4C in the callers' object (layout unknown, not in include/).
struct XWndFloat12_guess
{
    float v[12];    // 0x0 .. 0x2C

    void Add_guess(const XWndFloat12_guess *o);
};

// FUNCTION: TRILOGY 0x00404F17
// __thiscall. Element-wise this->v[i] += o->v[i] for 12 floats.
void XWndFloat12_guess::Add_guess(const XWndFloat12_guess *o)
{
    v[0] += o->v[0];
    v[1] += o->v[1];
    v[2] += ((float)(o->v[2]));
    float temp = v[3];
    v[3] = o->v[3] + temp;
    v[4] += o->v[4];
    v[5] += o->v[5];
    v[6] += o->v[6];
    v[7] += o->v[7];
    v[8] += o->v[8];
    float temp2 = v[9];
    v[9] = o->v[9] + temp2;
    v[10] += o->v[10];
    v[11] += o->v[11];
}

// FUNCTION: TRILOGY 0x00404F87
// Passes the 3-dword arg to 0x00401905, then adds a fixed 12-float pattern to the 12 floats at +0x4C.
void CXWnd_guess::FUN_00404f87_guess(const XWndKeyMsg_guess *msg)
{
    XWndFloat12_guess local;

    HandleKeyboardMsg_guess(msg);
    EQ_METHOD(0, void, 0x0040186A, (XWndFloat12_guess *), &local)();
    local.v[0] = 1.0f;
    local.v[1] = 1.0f;
    local.v[3] = EQ_GLOBAL(float, 0x005345BC);
    local.v[4] = 1.0f;
    local.v[6] = EQ_GLOBAL(float, 0x005345BC);
    local.v[7] = EQ_GLOBAL(float, 0x005345BC);
    local.v[9] = 1.0f;
    local.v[10] = EQ_GLOBAL(float, 0x005345BC);
    ((XWndFloat12_guess *)((char *)this + 0x4C))->Add_guess(&local);
}

// FUNCTION: TRILOGY 0x004056DC
// __thiscall. Stores the key message via HandleKeyboardMsg_guess, then calls vtable slot 0x40 with this+0x34.
void CXWnd_guess::HandleKeyboardMsgAndNotify_guess(const XWndKeyMsg_guess *msg)
{
    HandleKeyboardMsg_guess(msg);
    Slot16_guess(arg_guess);
}

// FUNCTION: TRILOGY 0x004056f7
// Updates two stored window colors from four input bytes, forcing the second copy's fourth byte to 255.
void XWndColor_guess::SetColor_guess(const unsigned char *color)
{
    color_copy_7c[3] = color[3];
    color_copy_7c[2] = color[2];
    color_copy_7c[1] = color[1];
    color_copy_7c[0] = color[0];
    color_copy_ec[3] = color[3];
    color_copy_ec[2] = color[2];
    color_copy_ec[1] = color[1];
    color_copy_ec[0] = color[0];
    color_copy_ec[3] |= 0xff;
    return;
}

#define XWND_IS_NAN(value) \
    ((((*(const unsigned long *)&(value)) & 0x7f800000UL) == 0x7f800000UL) && \
     (((*(const unsigned long *)&(value)) & 0x007fffffUL) != 0))
#define XWND_COMPARE_STATUS(left, right) \
    ((unsigned short)(((left) < (right)) || XWND_IS_NAN(left) || XWND_IS_NAN(right)) << 8 | \
     (unsigned short)(XWND_IS_NAN(left) || XWND_IS_NAN(right)) << 10 | \
     (unsigned short)(((left) == (right)) || XWND_IS_NAN(left) || XWND_IS_NAN(right)) << 14)

// FUNCTION: TRILOGY 0x0040573a
// Determines how a point relates to the area's standard and alternate bounds.
unsigned short XWndArea_guess::GetPointBoundsStatus_guess(const float *point)
{
    unsigned short comparison_status;

    if (point[0] >= alternate_minimum_x &&
        point[1] >= alternate_minimum_y &&
        point[0] <= alternate_maximum_x)
    {
        if (point[1] <= alternate_maximum_y)
        {
            comparison_status = XWND_COMPARE_STATUS(point[1], alternate_maximum_y);
            return (comparison_status & 0xff00) | 1;
        }
    }

    if (!(point[0] >= minimum_x))
        return XWND_COMPARE_STATUS(point[0], minimum_x);

    if (!(point[1] >= minimum_y))
        return XWND_COMPARE_STATUS(point[1], minimum_y);

    if (point[0] > maximum_x)
        return XWND_COMPARE_STATUS(point[0], maximum_x);

    if (point[1] > maximum_y)
        return XWND_COMPARE_STATUS(point[1], maximum_y);

    comparison_status = XWND_COMPARE_STATUS(point[1], maximum_y);
    return (comparison_status & 0xff00) | 1;
}

#undef XWND_COMPARE_STATUS
#undef XWND_IS_NAN

// FUNCTION: TRILOGY 0x004057a7
// Tests whether a point lies within the window area's currently selected bounds.
unsigned short XWndArea_guess::AreaContainsPoint_guess(const float *point)
{
    __asm
    {
        mov edx, dword ptr [esp + 4]
        cmp dword ptr [ecx + 0e8h], 0
        fld dword ptr [edx]
        jnz alternate_bounds
        fcomp dword ptr [ecx + 4ch]
        fnstsw ax
        sahf
        jc false_result
        fld dword ptr [edx + 4]
        fcomp dword ptr [ecx + 50h]
        fnstsw ax
        sahf
        jc false_result
        fld dword ptr [edx]
        fcomp dword ptr [ecx + 64h]
        fnstsw ax
        sahf
        ja false_result
        fld dword ptr [edx + 4]
        fcomp dword ptr [ecx + 68h]
        jmp last_y_compare
    alternate_bounds:
        fcomp dword ptr [ecx + 0b8h]
        fnstsw ax
        sahf
        jnc alternate_y_minimum
    false_result:
        xor al, al
        jmp return_result
    alternate_y_minimum:
        fld dword ptr [edx + 4]
        fcomp dword ptr [ecx + 0bch]
        fnstsw ax
        sahf
        jc false_result
        fld dword ptr [edx]
        fcomp dword ptr [ecx + 0d0h]
        fnstsw ax
        sahf
        ja false_result
        fld dword ptr [edx + 4]
        fcomp dword ptr [ecx + 0d4h]
    last_y_compare:
        fnstsw ax
        sahf
        ja false_result
        mov al, 1
    return_result:
        ret 4
    }
}

// FUNCTION: TRILOGY 0x004E2058
// Returns the local player's absolute actor-field difference, used as a sound distance.
// ORIGINAL FILE: Suite/Suite/XWnd.cpp (from Mac debug map)
// Ignores `this` (the one caller sets ECX anyway); the Mac name is doubtful. Returns |actor_info[0x4C] -
// actor_info[0x48]| of the local player, i.e. |second float - floor_z|, left on the x87 stack.
float CXWnd_BringChildWndToTop_guess(void)
{
    float d;

    d = g_local_player_guess->actor_info->fallStartZ_guess
        - g_local_player_guess->actor_info->floor_z;
    return d < 0.0f ? -d : d;
}
